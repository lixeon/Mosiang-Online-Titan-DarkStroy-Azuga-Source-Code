#include "hashsearch.h"


CHashSearch::CHashSearch()
{
	memset(this,0,sizeof(CHashSearch));

}



BOOL CHashSearch::Initialize(DWORD dwMaxItemNum,DWORD dwUIDNum)
{
	m_dwMaxItemNum = dwMaxItemNum;
	m_dwItemContainerSize = sizeof(ITEM_CONTAINER) - sizeof(DWORD) + sizeof(DWORD)*dwUIDNum;;

	m_dwMaxItemNum = dwMaxItemNum;
	
	
	m_pItemContainerPool = new char[m_dwItemContainerSize*m_dwMaxItemNum];
	memset(m_pItemContainerPool,0,m_dwItemContainerSize*m_dwMaxItemNum);
	
	
	m_ppItemContainerTable = new ITEM_CONTAINER*[m_dwMaxItemNum];
	memset(m_ppItemContainerTable,0,sizeof(ITEM_CONTAINER*)*m_dwMaxItemNum);


	m_ppHashContainerList = new ITEM_CONTAINER*[m_dwMaxItemNum];
	memset(m_ppHashContainerList,0,sizeof(ITEM_CONTAINER*)*m_dwMaxItemNum);

	for (DWORD i=0; i<m_dwMaxItemNum; i++)
	{
		m_ppItemContainerTable[i] = (ITEM_CONTAINER*)(m_pItemContainerPool + m_dwItemContainerSize*i);
	}
	m_dwReservedNodeNum = m_dwMaxItemNum;
	m_dwReservedItemNum = m_dwMaxItemNum;

	return TRUE;
}

ITEM_CONTAINER* CHashSearch::AllocItemContainer()
{

	ITEM_CONTAINER*	pContainer;

	pContainer = m_ppItemContainerTable[0];
	m_ppItemContainerTable[0] = m_ppItemContainerTable[m_dwReservedItemNum-1];
	m_dwReservedItemNum--;

	pContainer->pPrv = NULL;
	pContainer->pNext = NULL;
	pContainer->pItem = NULL;
	

	return pContainer;

}
void CHashSearch::FreeItemContaiener(ITEM_CONTAINER* pContainer)
{
	m_ppItemContainerTable[m_dwReservedItemNum] = pContainer;
	m_dwReservedItemNum++;
}


ITEM_CONTAINER*	CHashSearch::InsertItem(DWORD* pdwUID,DWORD dwUIDNum,void* pItem)
{
	ITEM_CONTAINER*	pNewContainer;
	
	DWORD	dwKey = 0;
	for (DWORD i=0; i<dwUIDNum; i++)
	{
		dwKey += pdwUID[i];
	}
	dwKey = dwKey % m_dwMaxItemNum;

	
	
	pNewContainer = AllocItemContainer();
	if (!pNewContainer)
		goto lb_false;

	pNewContainer->dwKey = dwKey;
	pNewContainer->dwUIDNum = dwUIDNum;
	for (i=0; i<dwUIDNum; i++)
	{
		pNewContainer->dwUID[i] = pdwUID[i];
	}
	
	pNewContainer->pItem = pItem;	// 아이템
	


	if (m_ppHashContainerList[dwKey])
	{
		pNewContainer->pNext = m_ppHashContainerList[dwKey];
		m_ppHashContainerList[dwKey]->pPrv = pNewContainer;
		m_ppHashContainerList[dwKey] = pNewContainer;
	}
	else
	{
		m_ppHashContainerList[dwKey] = pNewContainer;
	}
	m_dwItemNum++;
lb_false:
	return pNewContainer;
}


void* CHashSearch::SearchItem(DWORD* pdwUID,DWORD dwUIDNum)
{
	void*	pItem = NULL;
	DWORD	dwKey = 0;
	for (DWORD i=0; i<dwUIDNum; i++)
	{
		dwKey += pdwUID[i];
	}
	dwKey = dwKey % m_dwMaxItemNum;

	ITEM_CONTAINER*	pCur = m_ppHashContainerList[dwKey];
	
	while (pCur)
	{
		if (pCur->dwUIDNum != dwUIDNum)
			goto lb_skip;

		for (i=0; i<pCur->dwUIDNum; i++)
		{
			if (pCur->dwUID[i] != pdwUID[i])
				goto lb_skip;
		}
		goto lb_find;


lb_skip:
		pCur = pCur->pNext;
	}

	goto lb_return;

lb_find:
	pItem = pCur->pItem;

lb_return:
	return pItem;

}

BOOL CHashSearch::DeleteItem(ITEM_CONTAINER*	pItemContainer)
{
	BOOL	bResult = FALSE;
	ITEM_CONTAINER*		pCur = pItemContainer;
	

lb_find:
	ITEM_CONTAINER*		pNext;
	ITEM_CONTAINER*		pPrv;
	pNext = pCur->pNext;
	pPrv = pCur->pPrv;

	if (pPrv)
		pPrv->pNext = pNext;
	else
	{
		// 이게 맨 처음이다.
		m_ppHashContainerList[pItemContainer->dwKey] = pNext;
	}

	if (pNext)
		pNext->pPrv = pPrv;
	

	FreeItemContaiener(pCur);
	bResult = TRUE;
lb_return:
	return bResult;
}
/*
BOOL CHashSearch::DeleteItem(ITEM_CONTAINER*	pItemContainer)
{
	BOOL	bResult = FALSE;

	
	ITEM_CONTAINER*		pCur = m_ppHashContainerList[pItemContainer->dwKey];
	DWORD	i;

	while (pCur)
	{
		for (i=0; i<pCur->dwUIDNum; i++)
		{
			if (pCur->dwUID[i] != pItemContainer->dwUID[i])
			{
				goto lb_skip;
			}
		}
		goto lb_find;


lb_skip:
		pCur = pCur->pNext;
	}

	goto lb_return;

lb_find:
	ITEM_CONTAINER*		pNext;
	ITEM_CONTAINER*		pPrv;
	pNext = pCur->pNext;
	pPrv = pCur->pPrv;

	if (pPrv)
		pPrv->pNext = pNext;
	else
	{
		// 이게 맨 처음이다.
		m_ppHashContainerList[pItemContainer->dwKey] = pNext;
	}

	if (pNext)
		pNext->pPrv = pPrv;
	

	FreeItemContaiener(pCur);
	bResult = TRUE;
lb_return:
	return bResult;
}*/
void CHashSearch::Cleanup()
{
	
	if (m_pItemContainerPool)
	{
		delete [] m_pItemContainerPool;
		m_pItemContainerPool = NULL;
	}

	if (m_ppItemContainerTable)
	{
		delete [] m_ppItemContainerTable;
		m_ppItemContainerTable = NULL;
	}
	if (m_ppHashContainerList)
	{
		delete [] m_ppHashContainerList;
		m_ppHashContainerList = NULL;
	}
	memset(this,0,sizeof(CHashSearch));

}

CHashSearch::~CHashSearch()
{
	Cleanup();
}
