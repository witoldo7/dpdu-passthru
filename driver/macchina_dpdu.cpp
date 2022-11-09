#include "pch.h"
#include "macchina_dpdu.h"
#include "Logger.h"
#include "pdu_api.h"
#include "j2534/shim_loader.h"
#include <chrono>
#include <vector>
#include <string>

static std::vector<cPassThruInfo> m_registryList;
static unsigned long m_pDeviceID = 0;

T_PDU_ERROR __stdcall PDUConstruct(CHAR8* OptionStr, void* pAPITag)
{
	LOGGER.logWarn("STUB", "PDUConstruct is unimplimented");
	if (OptionStr != nullptr) {
		LOGGER.logWarn("STUB", "OptionStr: %s", OptionStr);
	}

	LOGGER.logWarn("STUB", "PDUConstruct return");
	return PDU_STATUS_NOERROR;
}

T_PDU_ERROR __stdcall PDUDestruct()
{
	LOGGER.logWarn("STUB", "PDUDestruct is unimplimented");
	return PDU_STATUS_NOERROR;
}

T_PDU_ERROR __stdcall PDUModuleConnect(UNUM32 hMod)
{
	T_PDU_ERROR ret = PDU_STATUS_NOERROR;

	LOGGER.logWarn("PDUModuleConnect", "Connecting to module: %u, library: %s", hMod, m_registryList[hMod].FunctionLibrary.c_str());

	bool sret = shim_loadLibrary(m_registryList[hMod].FunctionLibrary.c_str());
	if (!sret)
	{
		LOGGER.logWarn("PDUModuleConnect", "shim_loadLibrary failed");
		ret = PDU_ERR_MODULE_NOT_CONNECTED;
	}

	if (ret == PDU_STATUS_NOERROR)
	{
		long ptret = _PassThruOpen(NULL, &m_pDeviceID);
		if (ptret != STATUS_NOERROR)
		{
			LOGGER.logWarn("PDUModuleConnect", "_PassThruOpen failed: %d", ptret);
			ret = PDU_ERR_MODULE_NOT_CONNECTED;
		}
	}

	return ret;
}

T_PDU_ERROR __stdcall PDUModuleDisconnect(UNUM32 hMod)
{
	LOGGER.logWarn("STUB", "PDUModuleDisconnect is unimplimented");
	return PDU_STATUS_NOERROR;
}

T_PDU_ERROR __stdcall PDUGetTimestamp(UNUM32 hMod, UNUM32* pTimestamp)
{
	LOGGER.logWarn("STUB", "PDUGetTimestamp is unimplimented");
	return PDU_STATUS_NOERROR;
}

T_PDU_ERROR __stdcall PDUIoCtl(UNUM32 hMod, UNUM32 hCLL, UNUM32 IoCtlCommandId, PDU_DATA_ITEM* pInputData, PDU_DATA_ITEM** pOutputData)
{
	LOGGER.logWarn("PDUIoCtl", "hMod %u, hCLL %u, IoCtlCommandId %u", hMod, hCLL, IoCtlCommandId);
	return PDU_STATUS_NOERROR;
}

T_PDU_ERROR __stdcall PDUGetVersion(UNUM32 hMod, PDU_VERSION_DATA* pVersionData)
{
	LOGGER.logWarn("STUB", "PDUGetVersion is unimplimented");
	return PDU_STATUS_NOERROR;
}

T_PDU_ERROR __stdcall PDUGetStatus(UNUM32 hMod, UNUM32 hCLL, UNUM32 hCoP, T_PDU_STATUS* pStatusCode, UNUM32* pTimestamp, UNUM32* pExtraInfo)
{
	LOGGER.logWarn("STUB", "PDUGetStatus is unimplimented. hMod: %lu, hCll: %lu, hCoP: %lu", hMod, hCLL, hCoP);
	*pStatusCode = T_PDU_STATUS::PDU_MODST_READY;
	if (pExtraInfo != nullptr) {
		*pExtraInfo = 0;
	}
	return PDU_STATUS_NOERROR;
}

T_PDU_ERROR __stdcall PDUGetLastError(UNUM32 hMod, UNUM32 hCLL, T_PDU_ERR_EVT* pErrorCode, UNUM32* phCoP, UNUM32* pTimestamp, UNUM32* pExtraErrorInfo)
{
	LOGGER.logWarn("STUB", "PDUGetLastError is unimplimented");
	return PDU_STATUS_NOERROR;
}

T_PDU_ERROR __stdcall PDUGetResourceStatus(PDU_RSC_STATUS_ITEM* pResourceStatus)
{
	LOGGER.logWarn("STUB", "PDUGetResourceStatus is unimplimented");
	return PDU_STATUS_NOERROR;
}

T_PDU_ERROR __stdcall PDUCreateComLogicalLink(UNUM32 hMod, PDU_RSC_DATA* pRscData, UNUM32 resourceId, void* pCllTag, UNUM32* phCLL, PDU_FLAG_DATA* CllCreateFlag)
{
	LOGGER.logWarn("STUB", "PDUCreateComLogicalLink is unimplimented");
	return PDU_STATUS_NOERROR;
}

T_PDU_ERROR __stdcall PDUDestroyComLogicalLink(UNUM32 hMod, UNUM32 hCLL)
{
	LOGGER.logWarn("STUB", "PDUDestroyComLogicalLink is unimplimented");
	return PDU_STATUS_NOERROR;
}

T_PDU_ERROR __stdcall PDUConnect(UNUM32 hMod, UNUM32 hCLL)
{
	LOGGER.logWarn("STUB", "PDUConnect is unimplimented");
	return PDU_STATUS_NOERROR;
}

T_PDU_ERROR __stdcall PDUDisconnect(UNUM32 hMod, UNUM32 hCLL)
{
	LOGGER.logWarn("STUB", "PDUDisconnect is unimplimented");
	return PDU_STATUS_NOERROR;
}

T_PDU_ERROR __stdcall PDULockResource(UNUM32 hMod, UNUM32 hCLL, UNUM32 LockMask)
{
	LOGGER.logWarn("STUB", "PDULockResource is unimplimented");
	return PDU_STATUS_NOERROR;
}

T_PDU_ERROR __stdcall PDUUnlockResource(UNUM32 hMod, UNUM32 hCLL, UNUM32 LockMask)
{
	LOGGER.logWarn("STUB", "PDUUnlockResource is unimplimented");
	return PDU_STATUS_NOERROR;
}

T_PDU_ERROR __stdcall PDUGetComParam(UNUM32 hMod, UNUM32 hCLL, UNUM32 ParamId, PDU_PARAM_ITEM** pParamItem)
{
	LOGGER.logWarn("STUB", "PDUGetComParam is unimplimented");
	return PDU_STATUS_NOERROR;
}

T_PDU_ERROR __stdcall PDUSetComParam(UNUM32 hMod, UNUM32 hCLL, PDU_PARAM_ITEM* pParamItem)
{
	LOGGER.logWarn("STUB", "PDUSetComParam is unimplimented");
	return PDU_STATUS_NOERROR;
}

T_PDU_ERROR __stdcall PDUStartComPrimitive(UNUM32 hMod, UNUM32 hCLL, UNUM32 CoPType, UNUM32 CoPDataSize, UNUM8* pCoPData, PDU_COP_CTRL_DATA* pCopCtrlData, void* pCoPTag, UNUM32* phCoP)
{
	LOGGER.logWarn("STUB", "PDUStartComPrimitive is unimplimented");
	return PDU_STATUS_NOERROR;
}

T_PDU_ERROR __stdcall PDUCancelComPrimitive(UNUM32 hMod, UNUM32 hCLL, UNUM32 hCoP)
{
	LOGGER.logWarn("STUB", "PDUCancelComPrimitive is unimplimented");
	return PDU_STATUS_NOERROR;
}

T_PDU_ERROR __stdcall PDUGetEventItem(UNUM32 hMod, UNUM32 hCLL, PDU_EVENT_ITEM** pEventItem)
{
	LOGGER.logWarn("STUB", "PDUGetEventItem is unimplimented");
	return PDU_STATUS_NOERROR;
}

T_PDU_ERROR __stdcall PDUDestroyItem(PDU_ITEM* pItem)
{
	LOGGER.logWarn("STUB", "PDUDestroyItem is unimplimented");
	return PDU_STATUS_NOERROR;
}

T_PDU_ERROR __stdcall PDURegisterEventCallback(UNUM32 hMod, UNUM32 hCLL, CALLBACKFNC EventCallbackFunction)
{
	LOGGER.logWarn("STUB", "PDURegisterEventCallback is unimplimented");
	return PDU_STATUS_NOERROR;
}

T_PDU_ERROR __stdcall PDUGetObjectId(T_PDU_OBJT pduObjectType, CHAR8* pShortname, UNUM32* pPduObjectId)
{
	static UNUM32 ctr = 0;
	*pPduObjectId = ctr++;

	LOGGER.logWarn("PDUGetObjectId", "pduObjectType %d, pShortname %s", (int)pduObjectType, pShortname);
	return PDU_STATUS_NOERROR;
}

//storage for PDUGetModuleIds
static std::vector<std::string> m_pduModuleNames;
static std::vector<PDU_MODULE_DATA> m_pduModules;
static CHAR8 m_additionalInfo[32] = "ConnectionType = 'unknown'";

static PDU_MODULE_ITEM m_moduleItem = {
	PDU_IT_MODULE_ID,
	0,
	nullptr,
};

T_PDU_ERROR __stdcall PDUGetModuleIds(PDU_MODULE_ITEM** pModuleIdList)
{
	m_registryList.clear();
	m_pduModules.clear();
	m_pduModuleNames.clear();
	m_moduleItem.NumEntries = 0;

	std::set<cPassThruInfo> reg;
	shim_enumPassThruInterfaces(reg);
	std::copy(reg.begin(), reg.end(), std::back_inserter(m_registryList));

	UNUM32 idx = 0;
	for (auto iface : m_registryList)
	{
		std::ostringstream ss;
		ss << "VendorName='" << iface.Vendor << "' ModuleName='" << iface.Name.c_str() << "' J2534 Standard Version='4.04'";
		std::string str = ss.str();

		m_pduModuleNames.push_back(str);
		LOGGER.logInfo("PDUGetModuleIds", "Interface: %s", str.c_str());

		PDU_MODULE_DATA d = { 1, idx, (CHAR8*)m_pduModuleNames.back().c_str(), m_additionalInfo, PDU_MODST_AVAIL};
		m_pduModules.push_back(d);
		++idx;
	}

	m_moduleItem.pModuleData = &m_pduModules[0];
	m_moduleItem.NumEntries = idx;
	*pModuleIdList = &m_moduleItem;

	return PDU_STATUS_NOERROR;
}

T_PDU_ERROR __stdcall PDUGetResourceIds(UNUM32 hMod, PDU_RSC_DATA* pResourceIdData, PDU_RSC_ID_ITEM** pResourceIdList)
{
	LOGGER.logWarn("STUB", "PDUGetResourceIds is unimplimented");
	return PDU_STATUS_NOERROR;
}

T_PDU_ERROR __stdcall PDUGetConflictingResources(UNUM32 resourceId, PDU_MODULE_ITEM* pInputModuleList, PDU_RSC_CONFLICT_ITEM** pOutputConflictList)
{
	LOGGER.logWarn("STUB", "PDUGetConflictingResources is unimplimented");
	return PDU_STATUS_NOERROR;
}

T_PDU_ERROR __stdcall PDUGetUniqueRespIdTable(UNUM32 hMod, UNUM32 hCLL, PDU_UNIQUE_RESP_ID_TABLE_ITEM** pUniqueRespIdTable)
{
	LOGGER.logWarn("STUB", "PDUGetUniqueRespIdTable is unimplimented");
	return PDU_STATUS_NOERROR;
}

T_PDU_ERROR __stdcall PDUSetUniqueRespIdTable(UNUM32 hMod, UNUM32 hCLL, PDU_UNIQUE_RESP_ID_TABLE_ITEM* pUniqueRespIdTable)
{
	LOGGER.logWarn("STUB", "PDUSetUniqueRespIdTable is unimplimented");
	return PDU_STATUS_NOERROR;
}
