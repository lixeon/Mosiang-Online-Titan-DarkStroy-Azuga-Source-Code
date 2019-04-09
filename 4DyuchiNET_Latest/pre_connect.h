#pragma once

#include "../4DyuchiNET_Common/typedef.h"
#include "network.h"
#include "connect_que.h"


struct PRE_CONNECT
{
	SOCKET				socket;
	CNetwork*			pNetwork;

	CONNECTSUCCESSFUNC	SuccessConnect;
	CONNECTFAILFUNC		FailConnect;
	void*				pExt;

	PRE_CONNECT*		pNext;
	PRE_CONNECT*		pPrv;
};

class CPreConnectPool
{
	PRE_CONNECT*			m_pPreConnectProcessHead;
	PRE_CONNECT*			m_pPreConnectProcessTail;
	PRE_CONNECT*			m_pPreConnectHead;
	PRE_CONNECT*			m_pPreConnectTail;
	CRITICAL_SECTION		m_csPreConnect;
	DWORD					m_dwAvaliableConnectItemNum;
	DWORD					m_dwUsagedConnectItemNum;
	

	PRE_CONNECT*			Alloc();
	void					Free(PRE_CONNECT* pCon);
	void					FreeAll();
public:
	BOOL					Initialize(DWORD dwDefaultItemNum);
	BOOL					InsertPreConnect(CONNECT_ITEM* pItem);
	BOOL					ProcessPreConnect();

	CPreConnectPool();
	~CPreConnectPool();
};

BOOL StartPreConnectPool(DWORD dwMaxConnectNumAtSameTime,DWORD dwDefaultItemNum);
void EndPreConnectPool();

extern CPreConnectPool*	g_pPreConnectPool;
