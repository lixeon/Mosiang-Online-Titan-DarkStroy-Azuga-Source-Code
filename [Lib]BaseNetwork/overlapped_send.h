#pragma once

#include "overlapped.h"

class COverlappedSend : public COverlapped
{
	CRITICAL_SECTION	m_csSend;
public:
	void			Lock()		{EnterCriticalSection(&m_csSend);}
	void			Unlock()	{LeaveCriticalSection(&m_csSend);}
	BOOL			Push(char* pMsg,DWORD dwLen);
	BOOL			SendBuffer();
	void			OnSend(DWORD dwTransferredBytes);
	void			Initialize(DWORD dwIndex,SOCKET s,HANDLE hEvent,DWORD dwSize);
	COverlappedSend();
	~COverlappedSend();
};
