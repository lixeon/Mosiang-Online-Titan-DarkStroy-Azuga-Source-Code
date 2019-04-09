#pragma once

#include "stdafx.h"

struct OVL_INFO
{
	DWORD			m_dwIndex;
	OVERLAPPED*		m_pOvlWrite;
	OVERLAPPED*		m_pOvlRead;
	SOCKET			m_socket;
	DWORD			m_dwFlag;
};
BOOL StartWorkerThread();
void EndWorkerThread();
HANDLE IEGetEvnetHandle(DWORD dwIndex,DWORD dwType);
void IEPostDisconnectEvent(DWORD dwIndex);
void IEPostConnectEvent(SOCKET s);
BOOL IESetOvlInfo(OVL_INFO* pInfo);
void ReleaseMsgQue();
