// StaticD3DResourcePool.cpp: implementation of the CStaticD3DResourcePool class.
//
//////////////////////////////////////////////////////////////////////

#include "StaticD3DResourcePool.h"
#include "D3DResourceManager.h"
#include "CoD3DDevice.h"
#include "../4DyuchiGRX_Common/IGeometry.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStaticD3DResourcePool::CStaticD3DResourcePool()
{
	ClearMember();
}
void CStaticD3DResourcePool::ClearMember()
{
	m_pD3DRCBufferPoolHead = NULL;
	m_pItemHead = NULL;
	m_dwD3RCBufferNum = 0;

	m_pResourceManager = NULL;
	m_pD3DRCBufferPoolList = NULL;
	m_pItemList = NULL;

	m_pD3DRCBufferPoolHead = NULL;
	m_pItemHead = NULL;

	m_pSortLinkHead = NULL;
	m_pSortLinkTail = NULL;

	
	m_dwAllocUnitsNum = 0;
	

	m_dwD3RCBufferNum = 0;
	m_dwMaxMemorySize = 0;

	m_dwAllocatedItemNum = 0;
	m_dwReservedItemNum = 0;
}

BOOL CStaticD3DResourcePool::Initialize(CD3DResourceManager* pResourceManager,DWORD dwFVF,DWORD dwUsage,D3DPOOL pool,DWORD dwUnitSize,DWORD dwAllocUnitsNum,DWORD dwMaxItemNum,DWORD dwUnitFreeItemNum,OnAllocBufferFunc pAllocBufferFunc)
{
	BOOL	bResult = FALSE;

	m_pResourceManager = pResourceManager;
	m_pRenderer = pResourceManager->GetRenderer();
	m_D3DPOOL = pool;
	m_dwUsage = dwUsage;
	m_dwFVF = dwFVF;
	m_dwUnitSize = dwUnitSize;
	m_pAllocBufferFunc = pAllocBufferFunc;
	
	STATIC_D3DRC_BUFFER_POOL_LINK*		pCurPool;
	IDirect3DResource8*		pBuffer;
	HRESULT					hr;
	
	m_dwAllocUnitsNum = dwAllocUnitsNum;
	DWORD	dwItemNumPerPool = DEFAULT_LIMITED_VERTEXTBUFFER_INDICES / m_dwAllocUnitsNum;
	DWORD	dwUnitsNumPerPool = dwItemNumPerPool * m_dwAllocUnitsNum;
	DWORD	dwSizePerPool = dwUnitsNumPerPool * dwUnitSize;
	DWORD	dwD3DRCBufferPoolNum = dwMaxItemNum / dwItemNumPerPool;

	if (dwMaxItemNum % dwItemNumPerPool)
		dwD3DRCBufferPoolNum++;

	dwMaxItemNum = dwD3DRCBufferPoolNum*dwItemNumPerPool;
	

	m_pD3DRCBufferPoolList = new STATIC_D3DRC_BUFFER_POOL_LINK[dwD3DRCBufferPoolNum];
	memset(m_pD3DRCBufferPoolList,0,sizeof(STATIC_D3DRC_BUFFER_POOL_LINK)*dwD3DRCBufferPoolNum);

	for (DWORD i=0; i<dwD3DRCBufferPoolNum; i++)
	{
	//(CD3DResourceManager* pResourceManager,IDirect3DResource8** ppBuffer,DWORD dwFVF,D3DPOOL pool,DWORD dwUsage,DWORD dwSize);
		hr = pAllocBufferFunc(m_pResourceManager,&pBuffer,dwFVF,pool,dwUsage,dwSizePerPool);
	//	hr = m_pResourceManager->CreateVertexBuffer(dwSizePerPool,0,dwFVF,pool,&pVB);
		if (hr != D3D_OK)
		{
			__asm int 3
			goto lb_return;
		}

		pCurPool = m_pD3DRCBufferPoolList+i;
		pCurPool->pBuffer = pBuffer;
		pCurPool->dwStartOffset = 0;
		pCurPool->dwAvailableUnitsNum = dwUnitsNumPerPool;

		if (!m_pD3DRCBufferPoolHead)
			m_pD3DRCBufferPoolHead = pCurPool;
		else
		{
			pCurPool->pNext = m_pD3DRCBufferPoolHead;
			m_pD3DRCBufferPoolHead = pCurPool;
		}
				
		m_dwD3RCBufferNum++;
	}

	m_pItemList = new STATIC_D3DRC_BUFFER_ITEM[dwMaxItemNum];
	memset(m_pItemList,0,sizeof(STATIC_D3DRC_BUFFER_ITEM)*dwMaxItemNum);

	STATIC_D3DRC_BUFFER_ITEM*	pCurItem;
	for (i=0; i<dwMaxItemNum; i++)
	{
		pCurItem = m_pItemList+i;
		if (pCurItem->pNext)
			__asm int 3

		if (!m_pItemHead)
			m_pItemHead = pCurItem;
		else
		{
			pCurItem->pNext = m_pItemHead;
			m_pItemHead = pCurItem;
		}

		pCurItem->pBuffer = pCurPool->pBuffer;
		pCurItem->dwStartIndex = pCurPool->dwStartOffset;
		pCurItem->dwUnitsNum = m_dwAllocUnitsNum;
		pCurItem->sortLink.pItem = pCurItem;

		pCurPool->dwStartOffset += m_dwAllocUnitsNum;
		pCurPool->dwAvailableUnitsNum -= m_dwAllocUnitsNum;

		if ( pCurPool->dwAvailableUnitsNum < m_dwAllocUnitsNum )
			pCurPool = pCurPool->pNext;
	}
	
	m_dwReservedItemNum = m_dwMaxItemNum = dwMaxItemNum;

	m_dwUnitFreeItemNum = dwUnitFreeItemNum;
	bResult = TRUE;

lb_return:
	return bResult;
}

void  CStaticD3DResourcePool::OnLostDevice()
{
	if (D3DPOOL_MANAGED == m_D3DPOOL)
		return;

	if (D3DPOOL_SYSTEMMEM == m_D3DPOOL)
		return;

	ReleaseBufferAll();
	Cleanup();
	
}
void  CStaticD3DResourcePool::Reset()
{
	if (D3DPOOL_MANAGED == m_D3DPOOL)
		return;

	if (D3DPOOL_SYSTEMMEM == m_D3DPOOL)
		return;

	Initialize(m_pResourceManager,m_dwFVF,m_dwUsage,m_D3DPOOL,m_dwUnitSize,m_dwAllocUnitsNum,m_dwMaxItemNum,m_dwUnitFreeItemNum,m_pAllocBufferFunc);
}

BOOL CStaticD3DResourcePool::AllocBuffer(
												IDirect3DResource8** ppBuffer,
												void** ppHandle,
												DWORD* pdwStartIndex,
												DWORD* pdwSize,
												DWORD dwNum,
												BOOL* pbEnable,
												IResourceController* pResourceController
											)
{
	BOOL	bResult = FALSE;

	STATIC_D3DRC_BUFFER_ITEM*	pItem;
	if (!m_dwReservedItemNum)
	{
		FreeItem(m_dwUnitFreeItemNum);
//		goto lb_return;
	}

	// 여기서 자주 크래쉬..흠..
#ifdef _DEBUG
	if (!m_pItemHead)
	{
		m_pRenderer->OutputResourcePoolStatus();
		__asm int 3
	}
#endif

	
	pItem = m_pItemHead;
	
	m_pItemHead = pItem->pNext;

	pItem->pbEnable = pbEnable;
	pItem->pResourceController = pResourceController;
	*pItem->pbEnable = TRUE;

	*ppHandle = pItem;
	*pdwStartIndex = pItem->dwStartIndex;
	*ppBuffer = pItem->pBuffer;
	
	pItem->pNext = NULL;

	LinkToSortLink(&m_pSortLinkHead,&m_pSortLinkTail,&pItem->sortLink);

	m_dwAllocatedItemNum++;
	m_dwReservedItemNum--;
	bResult = TRUE;
lb_return:
	return bResult;
}
void CStaticD3DResourcePool::FreeItem(DWORD dwItemNum)
{
#ifdef _DEBUG
	SORT_LINK*	pCur = m_pSortLinkHead;
	DWORD	dwNum = 0;
	while (pCur)
	{
		dwNum++;
		pCur = pCur->pNext;
	}

	if (dwNum != m_dwAllocatedItemNum)
		__asm int 3

	pCur = m_pSortLinkTail;
	while (pCur)
	{
		dwNum--;
		pCur = pCur->pPrv;
	}

	if (dwNum)
		__asm int 3


#endif
	Update();
	if (!m_dwReservedItemNum)
	{
		for (DWORD i=0; i<dwItemNum; i++)
		{
			if (m_pSortLinkTail)
				ReleaseBuffer(m_pSortLinkTail->pItem);
		}
	}
}


void CStaticD3DResourcePool::Update()
{
	if (memcmp(m_pRenderer->INL_GetViewMatrix(),&m_matViewPrv,sizeof(MATRIX4)))
	{
		SORT_LINK*	pCur = m_pSortLinkHead;
		SORT_LINK*	pNext;
		while (pCur)
		{
			pNext = pCur->pNext;
			if (!((STATIC_D3DRC_BUFFER_ITEM*)pCur->pItem)->pResourceController->IsIncludeViewVolume())
				ReleaseBuffer(pCur->pItem);

			pCur = pNext;
		}
		m_matViewPrv = *m_pRenderer->INL_GetViewMatrix();
	}
	
}



void CStaticD3DResourcePool::ReleaseBufferAll()
{
	SORT_LINK*	pCur = m_pSortLinkHead;
	SORT_LINK*	pNext;
	while (pCur)
	{
		pNext = pCur->pNext;
		ReleaseBuffer(pCur->pItem);
		pCur = pNext;
	}

}




void CStaticD3DResourcePool::ReleaseBuffer(void* pHandle)
{
	STATIC_D3DRC_BUFFER_ITEM*		pItem = (STATIC_D3DRC_BUFFER_ITEM*)pHandle;
	if (!pItem->pbEnable)
		return;
	
	if (FALSE == *pItem->pbEnable)
		return;


	*pItem->pbEnable = FALSE;
	pItem->pbEnable = NULL;

	if (pItem->pResourceController)
		pItem->pResourceController->OnOut();
	
	// 소트링크에서 제거.
	UnLinkFromSortLink(&m_pSortLinkHead,&m_pSortLinkTail,&pItem->sortLink);
	
	
	if (!m_pItemHead)
		m_pItemHead = pItem;
	else
	{
		pItem->pNext = m_pItemHead;
		m_pItemHead = pItem;
	}

	m_dwAllocatedItemNum--;
	m_dwReservedItemNum++;
	
}

void CStaticD3DResourcePool::SetTimeStamp(void* pHandle)
{
	/*
	STATIC_D3DRC_BUFFER_ITEM*		pItem = (STATIC_D3DRC_BUFFER_ITEM*)pHandle;

	// 소트링크에서 제거.
	UnLinkFromSortLink(&m_pSortLinkHead,&m_pSortLinkTail,&pItem->sortLink);

	// 링크의 헤드로 올린다
	LinkToSortLink(&m_pSortLinkHead,&m_pSortLinkTail,&pItem->sortLink);
	
	pItem->dwTimeStamp = m_pRenderer->INL_GetTickCount();*/
}

void CStaticD3DResourcePool::Cleanup()
{
	for (DWORD i=0; i<m_dwD3RCBufferNum; i++)
	{
		m_pResourceManager->Release(m_pD3DRCBufferPoolList[i].pBuffer);
	}

	if (m_pD3DRCBufferPoolList)
	{
		delete [] m_pD3DRCBufferPoolList;
		m_pD3DRCBufferPoolList = NULL;
		m_dwD3RCBufferNum = 0;
	}
	if (m_pItemList)
	{

		memset(m_pItemList,0,sizeof(STATIC_D3DRC_BUFFER_ITEM)*m_dwMaxItemNum);
		if (m_pItemList)
		{
			delete [] m_pItemList;
			m_pItemList = NULL;
		}
	}
}
CStaticD3DResourcePool::~CStaticD3DResourcePool()
{
	Cleanup();	
	ClearMember();

}