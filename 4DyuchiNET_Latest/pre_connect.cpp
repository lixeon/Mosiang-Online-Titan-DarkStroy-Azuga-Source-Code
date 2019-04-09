#include "pre_connect.h"
#include "connect.h"

CPreConnectPool*	g_pPreConnectPool = NULL;

BOOL StartPreConnectPool(DWORD dwMaxConnectNumAtSameTime,DWORD dwDefaultItemNum)
{
	BOOL result;

	if (g_pPreConnectPool)
		return FALSE;
	
	g_pPreConnectPool = new CPreConnectPool;
	result = g_pPreConnectPool->Initialize(dwDefaultItemNum);
	
	if (!result)
		return result;

	result = StartConnectThread(dwMaxConnectNumAtSameTime);

	return result;
}
void EndPreConnectPool()
{
	EndConnectThread();
	if (g_pPreConnectPool)
	{
		delete g_pPreConnectPool;
		g_pPreConnectPool = NULL;
	}
}
CPreConnectPool::CPreConnectPool()
{
	m_pPreConnectProcessHead = NULL;
	m_pPreConnectProcessTail = NULL;
	m_pPreConnectHead = NULL;
	m_pPreConnectTail = NULL;
	m_dwAvaliableConnectItemNum = 0;
	m_dwUsagedConnectItemNum = 0;
	InitializeCriticalSection(&m_csPreConnect);

}

BOOL CPreConnectPool::Initialize(DWORD dwDefaultItemNum)
{
	PRE_CONNECT* cur;
	PRE_CONNECT* prv;
	
	cur = m_pPreConnectHead = new PRE_CONNECT;
	memset(m_pPreConnectHead,0,sizeof(PRE_CONNECT));
	prv = cur;
	
	for (DWORD i=0; i<dwDefaultItemNum-1; i++)
	{
		cur = prv->pNext = new PRE_CONNECT;
		memset(cur,0,sizeof(PRE_CONNECT));
		prv = cur;
		
	}
	m_pPreConnectTail = cur;
	m_dwAvaliableConnectItemNum = dwDefaultItemNum;
	
	return TRUE;
}

PRE_CONNECT* CPreConnectPool::Alloc()
{
	PRE_CONNECT*	pCon;

lb_start:
	if (m_pPreConnectHead)
	{
		pCon = m_pPreConnectHead;
		m_pPreConnectHead = m_pPreConnectHead->pNext;
		pCon->pNext = NULL;
		m_dwAvaliableConnectItemNum--;
		if (!m_dwAvaliableConnectItemNum)
		{
			m_pPreConnectTail = NULL;
			m_pPreConnectHead = NULL;
		}
		
		m_dwUsagedConnectItemNum++;
	}
	else 
	{
		m_pPreConnectTail = m_pPreConnectHead = new PRE_CONNECT;
		memset(m_pPreConnectHead,0,sizeof(PRE_CONNECT));
		m_dwAvaliableConnectItemNum++;
		goto lb_start;
	}
	return pCon;

}
void CPreConnectPool::Free(PRE_CONNECT* pCon)
{
	PRE_CONNECT*	pNext = NULL;
	memset(pCon,0,sizeof(PRE_CONNECT));
	if (pCon)
	{
		m_dwAvaliableConnectItemNum++;
		m_dwUsagedConnectItemNum--;
		
		if (!m_pPreConnectTail)
		{
			m_pPreConnectHead = m_pPreConnectTail = pCon;
		}
		else 
		{
			pCon->pNext = NULL;
			m_pPreConnectTail->pNext = pCon;
			m_pPreConnectTail = pCon;
		}
	}
}
BOOL CPreConnectPool::InsertPreConnect(CONNECT_ITEM* pCon)
{
	
	EnterCriticalSection(&m_csPreConnect);
	PRE_CONNECT* cur = Alloc();
	
	if (!m_pPreConnectProcessHead)
		m_pPreConnectProcessTail = m_pPreConnectProcessHead = cur;
	else 
	{
		m_pPreConnectProcessTail->pNext = cur;
		m_pPreConnectProcessTail = cur;

	}

	memcpy(cur,pCon,CONNECT_ATOM_SIZE);
	cur->pNext = NULL;
	
	LeaveCriticalSection(&m_csPreConnect);
	
	return TRUE;
}
BOOL CPreConnectPool::ProcessPreConnect()
{
	PRE_CONNECT* cur;
	CConnection* pCon;
	PRE_CONNECT* next;

	EnterCriticalSection(&m_csPreConnect);
	
	cur = m_pPreConnectProcessHead;
		

	while(cur)
	{
		next = cur->pNext;
		if (INVALID_SOCKET != cur->socket)
		{
			pCon = cur->pNetwork->AddConnection(cur->socket);
			if (pCon)
			{
				if (!cur->SuccessConnect)
					cur->pNetwork->OnAccept(pCon->GetConnectionIndex());
				else 
					cur->SuccessConnect(pCon->GetConnectionIndex(),cur->pExt);
				
				pCon->PrepareRead();
			
			}
		}
		else 
		{
			cur->FailConnect(cur->pExt);
		}
		Free(cur);
		cur = next;
	}
	m_pPreConnectProcessHead = m_pPreConnectProcessTail = NULL;

	LeaveCriticalSection(&m_csPreConnect);
	return TRUE;
}
void CPreConnectPool::FreeAll()
{
	PRE_CONNECT* cur;
	PRE_CONNECT* next;

	EnterCriticalSection(&m_csPreConnect);

	cur = m_pPreConnectProcessHead;
		
	while(cur)
	{
		next = cur->pNext;
		Free(cur);
//		printf( "[cur:%p] ", cur );
//		cur = next;
	}
	m_pPreConnectProcessHead = m_pPreConnectProcessTail = NULL;

	cur = m_pPreConnectHead;
	while(cur)
	{
		next = cur->pNext;
		printf("[Release %p] ", cur );
		delete cur;
		cur = next;
	}

	LeaveCriticalSection(&m_csPreConnect);


}
CPreConnectPool::~CPreConnectPool()
{
	FreeAll();
	DeleteCriticalSection(&m_csPreConnect);
}

