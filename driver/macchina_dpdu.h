/*
**
** Copyright (C) 2020 Ashcon Mohseninia
** Author: Ashcon Mohseninia <ashcon50@gmail.com>
**
** This library is free software; you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published
** by the Free Software Foundation, either version 3 of the License, or (at
** your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, <http://www.gnu.org/licenses/>.
**
*/

#pragma once
#include "pdu_api.h"
extern "C"
{
	T_PDU_ERROR __stdcall PDUConstruct(CHAR8* OptionStr, void* pAPITag);
	T_PDU_ERROR __stdcall PDUDestruct();
	T_PDU_ERROR __stdcall PDUModuleConnect(UNUM32 hMod);
	T_PDU_ERROR __stdcall PDUModuleDisconnect(UNUM32 hMod);
	T_PDU_ERROR __stdcall PDUGetTimestamp(UNUM32 hMod, UNUM32* pTimestamp);
	T_PDU_ERROR __stdcall PDUIoCtl(UNUM32 hMod, UNUM32 hCLL, UNUM32 IoCtlCommandId, PDU_DATA_ITEM* pInputData, PDU_DATA_ITEM** pOutputData);
	T_PDU_ERROR __stdcall PDUGetVersion(UNUM32 hMod, PDU_VERSION_DATA* pVersionData);
	T_PDU_ERROR __stdcall PDUGetStatus(UNUM32 hMod, UNUM32 hCLL, UNUM32 hCoP, T_PDU_STATUS* pStatusCode, UNUM32* pTimestamp, UNUM32* pExtraInfo);
	T_PDU_ERROR __stdcall PDUGetLastError(UNUM32 hMod, UNUM32 hCLL, T_PDU_ERR_EVT* pErrorCode, UNUM32* phCoP, UNUM32* pTimestamp, UNUM32* pExtraErrorInfo);
	T_PDU_ERROR __stdcall PDUGetResourceStatus(PDU_RSC_STATUS_ITEM* pResourceStatus);
	T_PDU_ERROR __stdcall PDUCreateComLogicalLink(UNUM32 hMod, PDU_RSC_DATA* pRscData, UNUM32 resourceId, void* pCllTag, UNUM32* phCLL, PDU_FLAG_DATA* CllCreateFlag);
	T_PDU_ERROR __stdcall PDUDestroyComLogicalLink(UNUM32 hMod, UNUM32 hCLL);
	T_PDU_ERROR __stdcall PDUConnect(UNUM32 hMod, UNUM32 hCLL);
	T_PDU_ERROR __stdcall PDUDisconnect(UNUM32 hMod, UNUM32 hCLL);
	T_PDU_ERROR __stdcall PDULockResource(UNUM32 hMod, UNUM32 hCLL, UNUM32 LockMask);
	T_PDU_ERROR __stdcall PDUUnlockResource(UNUM32 hMod, UNUM32 hCLL, UNUM32 LockMask);
	T_PDU_ERROR __stdcall PDUGetComParam(UNUM32 hMod, UNUM32 hCLL, UNUM32 ParamId, PDU_PARAM_ITEM** pParamItem);
	T_PDU_ERROR __stdcall PDUSetComParam(UNUM32 hMod, UNUM32 hCLL, PDU_PARAM_ITEM* pParamItem);
	T_PDU_ERROR __stdcall PDUStartComPrimitive(UNUM32 hMod, UNUM32 hCLL, UNUM32 CoPType, UNUM32 CoPDataSize, UNUM8* pCoPData, PDU_COP_CTRL_DATA* pCopCtrlData, void* pCoPTag, UNUM32* phCoP);
	T_PDU_ERROR __stdcall PDUCancelComPrimitive(UNUM32 hMod, UNUM32 hCLL, UNUM32 hCoP);
	T_PDU_ERROR __stdcall PDUGetEventItem(UNUM32 hMod, UNUM32 hCLL, PDU_EVENT_ITEM** pEventItem);
	T_PDU_ERROR __stdcall PDUDestroyItem(PDU_ITEM* pItem);
	T_PDU_ERROR __stdcall PDURegisterEventCallback(UNUM32 hMod, UNUM32 hCLL, CALLBACKFNC EventCallbackFunction);
	T_PDU_ERROR __stdcall PDUGetObjectId(T_PDU_OBJT pduObjectType, CHAR8* pShortname, UNUM32* pPduObjectId);
	T_PDU_ERROR __stdcall PDUGetModuleIds(PDU_MODULE_ITEM** pModuleIdList);
	T_PDU_ERROR __stdcall PDUGetResourceIds(UNUM32 hMod, PDU_RSC_DATA* pResourceIdData, PDU_RSC_ID_ITEM** pResourceIdList);
	T_PDU_ERROR __stdcall PDUGetConflictingResources(UNUM32 resourceId, PDU_MODULE_ITEM* pInputModuleList, PDU_RSC_CONFLICT_ITEM** pOutputConflictList);
	T_PDU_ERROR __stdcall PDUGetUniqueRespIdTable(UNUM32 hMod, UNUM32 hCLL, PDU_UNIQUE_RESP_ID_TABLE_ITEM** pUniqueRespIdTable);
	T_PDU_ERROR __stdcall PDUSetUniqueRespIdTable(UNUM32 hMod, UNUM32 hCLL, PDU_UNIQUE_RESP_ID_TABLE_ITEM* pUniqueRespIdTable);
}