#pragma once

#include "../4DyuchiNET_Common/stdafx.h"


#define MAX_LOG_SIZE 1024

class CLogManager
{
	char*			m_pBuffer;
	DWORD			m_dwLogNum;
	DWORD			m_dwBufferPosition;
	DWORD			m_dwLimitedSize;
	DWORD			m_dwLineCount;
	CRITICAL_SECTION	m_csLog;
	
	char			m_szFileName[_MAX_PATH];

public:
	void			Lock()		{EnterCriticalSection(&m_csLog);}
	void			UnLock()	{LeaveCriticalSection(&m_csLog);}
	BOOL			Initialize(char* szFileName,DWORD dwLimitedSize);
	BOOL			WriteLog(char* pMsg,DWORD dwLen);
	BOOL			Flush();
	
	CLogManager();
	~CLogManager();
};

extern CLogManager* g_pLogManager;

