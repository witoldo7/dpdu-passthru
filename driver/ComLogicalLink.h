#pragma once

#include "pdu_api.h"
#include "ComPrimitive.h"

#include <thread>
#include <queue>
#include <mutex>
#include "j2534/j2534_v0404.h"

class ComLogicalLink
{
public:

	ComLogicalLink(UNUM32 hMod, UNUM32 hCLL, unsigned long deviceID, unsigned long protocolID);

	long Connect();
	long Disconnect();

	T_PDU_ERROR GetStatus(T_PDU_STATUS& status);
	T_PDU_ERROR GetStatus(UNUM32 hCoP, T_PDU_STATUS& status);
	T_PDU_ERROR Cancel(UNUM32 hCoP);

	UNUM32 StartComPrimitive(UNUM32 CoPType, UNUM32 CoPDataSize, UNUM8* pCoPData, PDU_COP_CTRL_DATA* pCopCtrlData, void* pCoPTag);
	long StartMsgFilter(unsigned long filterType);
	long IoctlSetConfig(SCONFIG_LIST list);
	void RegisterEventCallback(CALLBACKFNC cb);
	bool GetEvent(PDU_EVENT_ITEM* & pEvt);
	void SignalEvent(PDU_EVENT_ITEM* pEvt);

private:
	void SignalEvents();
	void QueueEvent(PDU_EVENT_ITEM* pEvt);

	void run();

	void ProcessCop(std::shared_ptr<ComPrimitive> cop);
	long StartComm(std::shared_ptr<ComPrimitive> cop);
	long StopComm(std::shared_ptr<ComPrimitive> cop);
	long SendRecv(std::shared_ptr<ComPrimitive> cop);

	CALLBACKFNC m_eventCallbackFnc;

	bool m_running;
	std::thread m_runLoop;

	std::mutex m_eventLock;
	std::queue<PDU_EVENT_ITEM*> m_eventQueue;

	std::mutex m_copLock;
	std::vector<std::shared_ptr<ComPrimitive>> m_copQueue;

	UNUM32 m_hMod;
	UNUM32 m_hCLL;
	T_PDU_STATUS m_status;

	//J2534 specific
	unsigned long m_deviceID;
	unsigned long m_protocolID;
	unsigned long m_channelID;
};

