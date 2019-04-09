#include "log.h"

CLogManager* g_pLogManager = NULL;

CLogManager::CLogManager()
{
	m_pBuffer = 0;
	m_dwLogNum = 0;
	m_dwBufferPosition = 0;
	m_dwLimitedSize = 0;
	m_dwLineCount = 0;
}
BOOL CLogManager::Initialize(char* szFileName,DWORD dwLimitedSize)
{

	m_pBuffer = new char[dwLimitedSize+100];
	if (!m_pBuffer)
		return FALSE;

	DWORD len = lstrlen(szFileName);
	memcpy(m_szFileName,szFileName,len);
	*(m_szFileName + len) = 0;

	FILE* fp = fopen(m_szFileName,"wt");
	fclose(fp);

	m_dwLimitedSize = dwLimitedSize;

	InitializeCriticalSection(&m_csLog);

	return TRUE;
}
BOOL CLogManager::WriteLog(char* pMsg,DWORD dwLen)
{

	if (dwLen >= MAX_LOG_SIZE)
		return FALSE;


	Lock();
	
	if (m_dwBufferPosition  + dwLen + 5 >= m_dwLimitedSize)
		Flush();

	char* pWrite = m_pBuffer + m_dwBufferPosition;

	*(DWORD*)pWrite = dwLen+1;
	pWrite += 4;
	memcpy(pWrite,pMsg,dwLen);
	*(pWrite+dwLen) = 0;
	
	m_dwLogNum++;

	m_dwBufferPosition += (dwLen + 4 + 1);
	
	UnLock();

	return TRUE;
}

BOOL CLogManager::Flush()
{
	DWORD	dwLen;
	char*	pRead;

	Lock();

	pRead = m_pBuffer;
	
	FILE* fp = fopen(m_szFileName,"at");
	if (!fp)
		return FALSE;

	for (DWORD i=0; i<m_dwLogNum; i++)
	{
		dwLen = *(DWORD*)pRead;

		pRead += 4;
		fprintf(fp,"%8u : %s \n",m_dwLineCount,pRead);
		pRead += dwLen;
		m_dwLineCount++;
	}
	fclose(fp);
	m_dwBufferPosition = 0;
	m_dwLogNum = 0;

	UnLock();

	return TRUE;
}
CLogManager::~CLogManager()
{
	if (m_dwLogNum)
		Flush();

	if (m_pBuffer)
	{
		delete [] m_pBuffer;
		m_pBuffer = NULL;
	}
	DeleteCriticalSection(&m_csLog);
}