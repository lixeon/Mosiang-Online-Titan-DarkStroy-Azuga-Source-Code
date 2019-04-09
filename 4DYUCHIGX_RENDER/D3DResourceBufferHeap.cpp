// D3DResourceBufferHeap.cpp: implementation of the CD3DResourceBufferHeap class.
//
//////////////////////////////////////////////////////////////////////

#include "D3DResourceBufferHeap.h"
#include "D3DResourceManager.h"
#include "global_variable.h"
#include "CoD3DDevice.h"
#include "../4DyuchiGRX_Common/IGeometry.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CD3DResourceBufferHeap::CD3DResourceBufferHeap()
{
	m_pRenderer = NULL;
	m_pResourceManager = NULL;
	m_pHeap = NULL;
	m_pHeapItemDescPool = NULL;
	
	m_pSortLinkHead = NULL;
	m_pSortLinkTail = NULL;

	m_dwMaxD3DRCBufferNum = 0;
	m_pD3DRCBufferPoolList = NULL;
	m_dwSizePerD3DRCBufferPool = 0;
	m_dwUnitNumPerD3DRCBufferPool = 0;
	m_dwD3DRCBufferNum = 0;
	m_dwAllocatedItemNum = 0;
	m_dwUnitSize = 0;
	m_dwFreeUnitNum = 0;
	

	m_pMassMemory = NULL;
	m_dwMassMemorySize = NULL;
}



BOOL CD3DResourceBufferHeap::Initialize(CD3DResourceManager* pResourceManager,DWORD dwFVF,DWORD dwUsage,D3DPOOL pool,DWORD dwUnitSize,DWORD dwMaxUnitsNumPerBuffer,DWORD dwMaxBufferNum,DWORD dwMaxSize,OnAllocBufferFunc pAllocBufferFunc,DWORD dwFlag)
{
	BOOL	bResult = FALSE;
	m_pResourceManager = pResourceManager;
	m_pRenderer = pResourceManager->GetRenderer();

	m_dwUnitSize =  dwUnitSize;
	m_pAllocBufferFunc = pAllocBufferFunc;
		
	m_pHeap = VHPCreateHeap();	
	m_D3DPOOL = pool;
	m_dwFVF = dwFVF;
	m_dwUsage = dwUsage;

	
	m_dwFreeUnitNum = dwMaxBufferNum / 5.7f;
	m_dwMaxD3DRCBufferNum = dwMaxBufferNum;
	m_dwUnitNumPerD3DRCBufferPool = dwMaxUnitsNumPerBuffer;
	m_dwSizePerD3DRCBufferPool = m_dwUnitNumPerD3DRCBufferPool*m_dwUnitSize;
	m_dwMassMemorySize = m_dwSizePerD3DRCBufferPool*m_dwMaxD3DRCBufferNum;
	m_pMassMemory = (char*)m_dwUnitSize;

	

#ifdef _DEBUG
	VHPInitialize(m_pHeap,m_pMassMemory,m_dwMassMemorySize,8192,m_dwUnitSize,m_dwUnitNumPerD3DRCBufferPool,TRUE);
#else
	VHPInitialize(m_pHeap,m_pMassMemory,m_dwMassMemorySize,8192,m_dwUnitSize,m_dwUnitNumPerD3DRCBufferPool,FALSE);
#endif

	m_pD3DRCBufferPoolList = new D3DRC_BUFFER_POOL_DESC[m_dwMaxD3DRCBufferNum];
	memset(m_pD3DRCBufferPoolList,0,sizeof(D3DRC_BUFFER_POOL_DESC)*m_dwMaxD3DRCBufferNum);

	m_pHeapItemDescPool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(m_pHeapItemDescPool,sizeof(D3DRC_HEAP_ITEM),8192/4,8192);



	m_dwFlag = dwFlag;
	/*
	if (m_dwFlag & ALAWAYS_KEEP_BUFFER)
	{
		DWORD i;
		for (i=0; i<m_dwMaxD3DRCBufferNum; i++)
		{
			HRESULT hr;
			hr = m_pAllocBufferFunc(
				m_pResourceManager,
				&m_pD3DRCBufferPoolList[i].pBuffer,
				m_dwFVF,
				m_D3DPOOL,
				m_dwUsage,
				m_dwSizePerD3DRCBufferPool);
	
			if (hr != D3D_OK)
				__asm int 3;
	
			m_pD3DRCBufferPoolList[i].dwRefCount++;
			m_dwD3DRCBufferNum++;
		}
	}*/
	bResult = TRUE;
lb_return:

	return bResult;
}

void CD3DResourceBufferHeap::OnLostDevice()
{
	if (D3DPOOL_MANAGED == m_D3DPOOL)
		return;

	if (D3DPOOL_SYSTEMMEM == m_D3DPOOL)
		return;

}
void CD3DResourceBufferHeap::Reset()
{
	if (D3DPOOL_MANAGED == m_D3DPOOL)
		return;

	if (D3DPOOL_SYSTEMMEM == m_D3DPOOL)
		return;
}


void CD3DResourceBufferHeap::SetTimeStamp(void* pHandle)
{
}
BOOL CD3DResourceBufferHeap::AllocBuffer(
												DWORD*	pdwErrorCode,
												IDirect3DResource8** ppBuffer,
												void** ppHandle,
												DWORD* pdwStartIndex,
												DWORD* pdwSize,
												DWORD dwNum,
												BOOL* pbEnable,
												IResourceController* pResourceController
											)
{
	BOOL	bTryAgain = FALSE;
	

	BOOL	bResult = FALSE;
	DWORD	dwSize;

	if (dwNum > m_dwUnitNumPerD3DRCBufferPool)
		goto lb_return;

	D3DRC_HEAP_ITEM*		pHeapItemDesc;
	DWORD					dwStartIndex;
	IDirect3DResource8*		pBuffer;
	
	char*	p;

lb_alloc:
	
#ifdef _DEBUG
	p = (char*)VHPDBGAllocUnit(m_pHeap,pdwErrorCode,dwNum,&dwSize,TRUE);
#else
	p = (char*)VHPAllocUnit(m_pHeap,pdwErrorCode,dwNum,&dwSize,TRUE);
#endif

	
	if (!p)
	{
		if (NOT_USE_AUTO_SCHEDLUE & m_dwFlag)
			goto lb_return;

		if (bTryAgain)
			goto lb_return;
		else
			goto lb_try_again;
	//	FreeItem(m_dwFreeUnitNum);
	//	goto lb_alloc;;
	}
	
	pHeapItemDesc = (D3DRC_HEAP_ITEM*)LALAlloc(m_pHeapItemDescPool);
	if (!pHeapItemDesc)
	{
#ifdef _DEBUG
	VHPDBGFree(m_pHeap,p);
#else
	VHPFree(m_pHeap,p);
#endif
		if (bTryAgain)
			goto lb_return;
		else
			goto lb_try_again;
	}

	DWORD	dwFrontBoundaryIndex,dwRelativeMemAddr;
	dwRelativeMemAddr = (DWORD)p - (DWORD)m_pMassMemory;
	dwFrontBoundaryIndex = dwRelativeMemAddr / m_dwSizePerD3DRCBufferPool;

#ifdef _DEBUG
	DWORD	dwRearBoundaryIndex;
	dwRearBoundaryIndex = (dwRelativeMemAddr + dwSize - 1) / m_dwSizePerD3DRCBufferPool;

	if ( dwFrontBoundaryIndex != dwRearBoundaryIndex )
		__asm int 3

	if (dwFrontBoundaryIndex >= m_dwMaxD3DRCBufferNum)
		__asm int 3

	
#endif

	if (!m_pD3DRCBufferPoolList[dwFrontBoundaryIndex].pBuffer)
	{
		HRESULT hr;
		hr = m_pAllocBufferFunc(
			m_pResourceManager,
			&m_pD3DRCBufferPoolList[dwFrontBoundaryIndex].pBuffer,
			m_dwFVF,
			m_D3DPOOL,
			m_dwUsage,
			m_dwSizePerD3DRCBufferPool);
	
		if (hr != D3D_OK)
			__asm int 3;
	
		m_dwD3DRCBufferNum++;
	}

#ifdef _DEBUG
	if (dwRelativeMemAddr < (dwFrontBoundaryIndex*m_dwSizePerD3DRCBufferPool))
		__asm int 3
#endif
	
	dwStartIndex = ( dwRelativeMemAddr -  (dwFrontBoundaryIndex*m_dwSizePerD3DRCBufferPool)) / m_dwUnitSize;
#ifdef _DEBUG
	if ( (DWORD)(p - m_pMassMemory) % m_dwUnitSize != 0)
	{
		char	txt[512];
		memset(txt,0,512);
		wsprintf(txt,"Fail toCD3DResourceBufferHeap::AllocVB() (p - m_pMassMemory) is not aligned, File : %s , Line : %d \n",__FILE__,__LINE__);
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
	}
#endif


	pHeapItemDesc->pMem = p;
	pHeapItemDesc->dwD3DRCBufferVBPoolIndex = dwFrontBoundaryIndex;
	pHeapItemDesc->pbEnable = pbEnable;
	pHeapItemDesc->pResourceController = pResourceController;
	pHeapItemDesc->sortLink.pItem = pHeapItemDesc;
	*pHeapItemDesc->pbEnable = TRUE;
	
	pBuffer = m_pD3DRCBufferPoolList[dwFrontBoundaryIndex].pBuffer;
	m_pD3DRCBufferPoolList[dwFrontBoundaryIndex].dwRefCount++;

	*ppBuffer = pBuffer;
	*ppHandle = pHeapItemDesc;
	*pdwStartIndex = dwStartIndex;

	LinkToSortLink(&m_pSortLinkHead,&m_pSortLinkTail,&pHeapItemDesc->sortLink);

	if (pdwSize)
		*pdwSize = dwSize;
	
	m_dwAllocatedItemNum++;

	bResult = TRUE;
	goto lb_return;

lb_try_again:
	Update();
	bTryAgain = TRUE;
	goto lb_alloc;
	
lb_return:
	return bResult;
}
void CD3DResourceBufferHeap::ReleaseBuffer(void* pHandle)
{
	D3DRC_HEAP_ITEM*	pHeapItemDesc = (D3DRC_HEAP_ITEM*)pHandle;

	DWORD	dwFrontBoundaryIndex;

#ifdef _DEBUG

	DWORD	dwRelativeMemAddr;
	dwRelativeMemAddr = (DWORD)pHeapItemDesc->pMem - (DWORD)m_pMassMemory;
	dwFrontBoundaryIndex = dwRelativeMemAddr / m_dwSizePerD3DRCBufferPool;

	if (pHeapItemDesc->dwD3DRCBufferVBPoolIndex != dwFrontBoundaryIndex)
		__asm int 3
#endif

	if (!pHeapItemDesc->pbEnable)
		return;		
	
	if (FALSE == *pHeapItemDesc->pbEnable)
		return;

	*pHeapItemDesc->pbEnable = FALSE;
	pHeapItemDesc->pbEnable = NULL;

	if (pHeapItemDesc->pResourceController)
		pHeapItemDesc->pResourceController->OnOut();

	dwFrontBoundaryIndex = pHeapItemDesc->dwD3DRCBufferVBPoolIndex;
	m_pD3DRCBufferPoolList[dwFrontBoundaryIndex].dwRefCount--;
	if (!m_pD3DRCBufferPoolList[dwFrontBoundaryIndex].dwRefCount)
	{
		m_pResourceManager->Release(m_pD3DRCBufferPoolList[dwFrontBoundaryIndex].pBuffer);
		m_pD3DRCBufferPoolList[dwFrontBoundaryIndex].pBuffer = NULL;
		m_dwD3DRCBufferNum--;
	}
//	Unlink...
	UnLinkFromSortLink(&m_pSortLinkHead,&m_pSortLinkTail,&pHeapItemDesc->sortLink);
	LALFree(m_pHeapItemDescPool,pHeapItemDesc);


#ifdef _DEBUG
	VHPDBGFree(m_pHeap,pHeapItemDesc->pMem);
#else
	VHPFree(m_pHeap,pHeapItemDesc->pMem);
#endif
	
	pHeapItemDesc->pMem = (char*)0xcccccccc;

	m_dwAllocatedItemNum--;
}

void CD3DResourceBufferHeap::Update()
{
	if (memcmp(m_pRenderer->INL_GetViewMatrix(),&m_matViewPrv,sizeof(MATRIX4)))
	{
		SORT_LINK*	pCur = m_pSortLinkHead;
		SORT_LINK*	pNext;
		while (pCur)
		{
			pNext = pCur->pNext;
			if (!((D3DRC_HEAP_ITEM*)pCur->pItem)->pResourceController->IsIncludeViewVolume())
				ReleaseBuffer(pCur->pItem);

			pCur = pNext;
		}
		m_matViewPrv = *m_pRenderer->INL_GetViewMatrix();
	}
	
}
void CD3DResourceBufferHeap::ReleaseBufferAll()
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
/*
void CD3DResourceBufferHeap::FreeItem(DWORD dwItemNum)
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
*/
/*
void CD3DResourceBufferHeap::FreeItem(DWORD dwItemNum)
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
	if (!m_dwAllocatedItemNum)
	{
		for (DWORD i=0; i<dwItemNum; i++)
		{
			if (!((D3DRC_HEAP_ITEM*)m_pSortLinkTail->pItem)->pbEnable)
			{
				if (m_pSortLinkTail)
					ReleaseBuffer(m_pSortLinkTail->pItem);
			}
		}
	}
}

void CD3DResourceBufferHeap::Update()
{
	if (memcmp(m_pRenderer->INL_GetViewMatrix(),&m_matViewPrv,sizeof(MATRIX4)))
	{
		SORT_LINK*	pCur = m_pSortLinkHead;
		SORT_LINK*	pNext;
		while (pCur)
		{
			pNext = pCur->pNext;
			if (!((D3DRC_HEAP_ITEM*)pCur->pItem)->pbEnable)
			{
				if (!((D3DRC_HEAP_ITEM*)pCur->pItem)->pControllerStatic->IsIncludeViewVolume())
					ReleaseBuffer(pCur->pItem);
			}

			pCur = pNext;
		}
		m_matViewPrv = *m_pRenderer->INL_GetViewMatrix();
	}
	
}
*/

void CD3DResourceBufferHeap::RemoveD3DRCBuffer()
{
	/*
	if (m_dwFlag & ALAWAYS_KEEP_BUFFER)
	{
		DWORD i;
		for (i=0; i<m_dwMaxD3DRCBufferNum; i++)
		{
			m_pD3DRCBufferPoolList[i].dwRefCount--;
			if (!m_pD3DRCBufferPoolList[i].dwRefCount)
			{
				m_pResourceManager->Release(m_pD3DRCBufferPoolList[i].pBuffer);
				m_pD3DRCBufferPoolList[i].pBuffer = NULL;
				m_dwD3DRCBufferNum--;
			}
		}
		m_dwFlag &= (~ALAWAYS_KEEP_BUFFER);
	}
	*/
}


void CD3DResourceBufferHeap::Cleanup()
{
	if (m_pD3DRCBufferPoolList)
	{
		RemoveD3DRCBuffer();
		delete [] m_pD3DRCBufferPoolList;
		m_pD3DRCBufferPoolList = NULL;
	}
/*	
	if (m_pD3DRCBufferPoolList)
	{
		if (m_dwFlag & ALAWAYS_KEEP_BUFFER)
		{
			DWORD i;
			for (i=0; i<m_dwMaxD3DRCBufferNum; i++)
			{
				m_pD3DRCBufferPoolList[i].dwRefCount--;
				if (!m_pD3DRCBufferPoolList[i].dwRefCount)
				{
					m_pResourceManager->Release(m_pD3DRCBufferPoolList[i].pBuffer);
					m_pD3DRCBufferPoolList[i].pBuffer = NULL;
					m_dwD3DRCBufferNum--;
				}
			}
		}
		delete [] m_pD3DRCBufferPoolList;
		m_pD3DRCBufferPoolList = NULL;
	}*/
	if (m_pHeap)
	{
#ifdef _DEBUG
	VHPHeapCheck(m_pHeap);
	VHPLeakCheck(m_pHeap);
#endif
		VHPReleaseHeap(m_pHeap);
		m_pHeap = NULL;
	}
	if (m_pHeapItemDescPool)
	{
		ReleaseStaticMemoryPool(m_pHeapItemDescPool);
		m_pHeapItemDescPool = NULL;
	}
	
}
CD3DResourceBufferHeap::~CD3DResourceBufferHeap()
{
	Cleanup();

}
