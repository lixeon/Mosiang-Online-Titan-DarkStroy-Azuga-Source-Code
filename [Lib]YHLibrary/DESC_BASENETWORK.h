#pragma once

#include "stdafx.h"

//#pragma pack(push,4)
struct DESC_BASENETWORK
{
	WORD		wSockEventWinMsgID;
    void		(*ReceivedMsg)(DWORD dwInex,char* pMsg,DWORD dwLen);
	void		(*OnDisconnect)(DWORD dwInex);
	void		(*OnConnect)(DWORD dwInex);
};

//#pragma pack(pop)