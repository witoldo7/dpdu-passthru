#pragma once

#include "pdu_api.h"
#include <vector>

class ComPrimitive
{
public:
	ComPrimitive(UNUM32 CoPType, UNUM32 CoPDataSize, UNUM8* pCoPData, PDU_COP_CTRL_DATA* pCopCtrlData, void* pCoPTag);

	UNUM32 getHandle();
	UNUM32 getType();

	long StartComm(unsigned long channelID, PDU_EVENT_ITEM* & pEvt);
	long SendRecv(unsigned long channelID, PDU_EVENT_ITEM*& pEvt);

	void Execute(PDU_EVENT_ITEM*& pEvt);
	void Finish(PDU_EVENT_ITEM*& pEvt);

private:

	UNUM32 m_hCoP;
	UNUM32 m_CoPType;
	std::vector<UNUM8> m_CoPData;
	PDU_COP_CTRL_DATA m_CopCtrlData;
	void* m_pCoPTag;

	static UNUM32 m_hCoPCtr;

};

