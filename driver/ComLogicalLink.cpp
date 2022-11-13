#include "pch.h"
#include "ComLogicalLink.h"
#include "j2534/j2534_v0404.h"
#include "j2534/shim_loader.h"
#include "Logger.h"
#include "pdu_api.h"

#include <thread>
#include <string>
#include <sstream>

ComLogicalLink::ComLogicalLink(UNUM32 hMod, UNUM32 hCLL, unsigned long deviceID, unsigned long protocolID) :
	m_eventCallbackFnc(nullptr), m_hMod(hMod), m_hCLL(hCLL), m_deviceID(deviceID), m_protocolID(protocolID), m_channelID(0), m_running(false)
{
}

long ComLogicalLink::Connect()
{
	long ret = STATUS_NOERROR;
	switch (m_protocolID)
	{
		case ISO14230:
			ret = _PassThruConnect(m_deviceID, m_protocolID, 0, 10400, &m_channelID);
			break;
		default:
			ret = ERR_INVALID_PROTOCOL_ID;
			break;
	}

	if (ret == STATUS_NOERROR)
	{
		m_running = true;
		m_runLoop = std::thread(&ComLogicalLink::run, this);

		PDU_EVENT_ITEM* pEvt = new PDU_EVENT_ITEM;
		pEvt->hCop = PDU_HANDLE_UNDEF;
		pEvt->ItemType = PDU_IT_STATUS;
		pEvt->pCoPTag = nullptr;
		pEvt->pData = new PDU_STATUS_DATA;
		*(PDU_STATUS_DATA*)(pEvt->pData) = PDU_CLLST_ONLINE;

		SignalEvent(pEvt);
	}

	return ret;
}

long ComLogicalLink::Disconnect()
{
	long ret = STATUS_NOERROR;

	ret = _PassThruDisconnect(m_channelID);

	if (ret == STATUS_NOERROR)
	{
		m_running = false;
		m_runLoop.join();

		PDU_EVENT_ITEM* pEvt = new PDU_EVENT_ITEM;
		pEvt->hCop = PDU_HANDLE_UNDEF;
		pEvt->ItemType = PDU_IT_STATUS;
		pEvt->pCoPTag = nullptr;
		pEvt->pData = new PDU_STATUS_DATA;
		*(PDU_STATUS_DATA*)(pEvt->pData) = PDU_CLLST_OFFLINE;

		SignalEvent(pEvt);
	}

	return ret;
}

UNUM32 ComLogicalLink::StartComPrimitive(UNUM32 CoPType, UNUM32 CoPDataSize, UNUM8* pCoPData, PDU_COP_CTRL_DATA* pCopCtrlData, void* pCoPTag)
{
	auto cop = std::shared_ptr<ComPrimitive>(new ComPrimitive(CoPType, CoPDataSize, pCoPData, pCopCtrlData, pCoPTag));

	{
		const std::lock_guard<std::mutex> lock(m_copLock);
		m_copQueue.push(cop);
	}

	return cop->getHandle();
}

long ComLogicalLink::StartMsgFilter(unsigned long filterType)
{
	long ret = STATUS_NOERROR;

	unsigned long filterId = 0;
	PASSTHRU_MSG msg = { m_channelID, 0, 0, 0, 4, 4, {0, 0, 0, 0} };
	ret = _PassThruStartMsgFilter(m_channelID, filterType, &msg, &msg, &msg, &filterId);

	return ret;
}

void ComLogicalLink::RegisterEventCallback(CALLBACKFNC cb)
{
	m_eventCallbackFnc = cb;
}

bool ComLogicalLink::GetEvent(PDU_EVENT_ITEM* & pEvt)
{
	{
		const std::lock_guard<std::mutex> lock(m_eventLock);
		if (m_eventQueue.empty())
		{
			return false;
		}

		pEvt = m_eventQueue.front();
		m_eventQueue.pop();
	}

	return true;
}

void ComLogicalLink::SignalEvent(PDU_EVENT_ITEM* pEvt)
{
	QueueEvent(pEvt);
	SignalEvents();
}

void ComLogicalLink::SignalEvents()
{
	LOGGER.logInfo("ComLogicalLink/SignalEvents", "Signaled events");

	m_eventCallbackFnc(PDU_EVT_DATA_AVAILABLE, m_hMod, m_hCLL, nullptr, nullptr);
}

void ComLogicalLink::QueueEvent(PDU_EVENT_ITEM* pEvt)
{
	if (pEvt == nullptr)
	{
		return;
	}

	LOGGER.logInfo("ComLogicalLink/QueueEvent", "Queued event %p", pEvt);

	auto now = std::chrono::high_resolution_clock::now();
	auto duration = now.time_since_epoch();
	pEvt->Timestamp = (UNUM32)(std::chrono::duration_cast<std::chrono::microseconds>(duration).count());

	{
		const std::lock_guard<std::mutex> lock(m_eventLock);
		m_eventQueue.push(pEvt);
	}
}

void ComLogicalLink::run()
{
	LOGGER.logInfo("ComLogicalLink", "ChannelId %u RunLoop started...", m_channelID);

	while (m_running)
	{
		std::shared_ptr<ComPrimitive> cop;
		{
			const std::lock_guard<std::mutex> lock(m_copLock);
			if (!m_copQueue.empty())
			{
				cop = m_copQueue.front();
				m_copQueue.pop();
			}
		}

		if (cop == nullptr)
		{
			//throttle if no cops to process
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}

		ProcessCop(cop);
	}
}

void ComLogicalLink::ProcessCop(std::shared_ptr<ComPrimitive> cop)
{
	long ret = STATUS_NOERROR;
	PDU_EVENT_ITEM* pEvt;

	cop->Execute(pEvt);
	SignalEvent(pEvt);

	switch (cop->getType())
	{
	case PDU_COPT_STARTCOMM:
		ret = StartComm(cop);
		break;

	case PDU_COPT_SENDRECV:
		ret = SendRecv(cop);
		break;
	}

	cop->Finish(pEvt);
	SignalEvent(pEvt);

	if (ret != STATUS_NOERROR)
	{
		LOGGER.logError("ComLogicalLink/ProcessCop", "Failed processing cop %u coptype %u, ret %u",
			cop->getHandle(), cop->getType(), ret);
	}
}

long ComLogicalLink::StartComm(std::shared_ptr<ComPrimitive> cop)
{
	long ret = STATUS_NOERROR;
	if (m_protocolID == ISO14230)
	{
		PDU_EVENT_ITEM* pEvt;

		ret = cop->StartComm(m_channelID, pEvt);
		if (ret == STATUS_NOERROR)
		{
			QueueEvent(pEvt);

			pEvt = new PDU_EVENT_ITEM;
			pEvt->hCop = PDU_HANDLE_UNDEF;
			pEvt->ItemType = PDU_IT_STATUS;
			pEvt->pCoPTag = nullptr;
			pEvt->pData = new PDU_STATUS_DATA;
			*(PDU_STATUS_DATA*)(pEvt->pData) = PDU_CLLST_COMM_STARTED;
			QueueEvent(pEvt);
		}
	}

	return ret;
}

long ComLogicalLink::SendRecv(std::shared_ptr<ComPrimitive> cop)
{
	long ret = STATUS_NOERROR;
	PDU_EVENT_ITEM* pEvt;

	ret = cop->SendRecv(m_channelID, pEvt);
	if (ret == STATUS_NOERROR)
	{
		QueueEvent(pEvt);
	}

	return ret;
}
