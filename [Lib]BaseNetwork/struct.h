#pragma once

#include "stdafx.h"

struct DESC_BASENETWORK
{
	WORD		wSockEventWinMsgID;
    void		(*ReceivedMsg)(DWORD dwInex,char* pMsg,DWORD dwLen);
	void		(*OnDisconnect)(DWORD dwInex);
	void		(*OnConnect)(DWORD dwInex);
};
