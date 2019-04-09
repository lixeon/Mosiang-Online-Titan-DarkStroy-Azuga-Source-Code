// RenderTriBuffer.cpp: implementation of the CRenderTriBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include "RenderTriBuffer.h"
#include "CoD3DDevice.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRenderTriBuffer::CRenderTriBuffer()
{
	memset(this,0,sizeof(CRenderTriBuffer));

}

BOOL CRenderTriBuffer::Initialize(CoD3DDevice* pRenderer,DWORD dwMaxTriItemNum,DWORD dwMaxTriNum)
{
	m_pRenderer = pRenderer;
	m_pTriItemPool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(m_pTriItemPool,sizeof(IVERTEX_TRI_ITEM),dwMaxTriItemNum/4+4,dwMaxTriItemNum);

	m_pVertexHeap = VHPCreateHeap();
	m_dwVertexMassMemorySize = dwMaxTriNum*3*sizeof(IVERTEX);
	m_pVertexMassMemory = new char[m_dwVertexMassMemorySize];
	memset(m_pVertexMassMemory,0,m_dwVertexMassMemorySize);

#ifdef _DEBUG
	VHPInitialize(m_pVertexHeap,m_pVertexMassMemory,m_dwVertexMassMemorySize,dwMaxTriItemNum,0,0,TRUE);
#else
	VHPInitialize(m_pVertexHeap,m_pVertexMassMemory,m_dwVertexMassMemorySize,dwMaxTriItemNum,0,0,FALSE);
#endif

	return TRUE;
}

IVERTEX_TRI_ITEM* CRenderTriBuffer::AllocBuffer(IVERTEX** ppIVList,DWORD dwFacesNum,DWORD dwRenderFlag)
{
	IVERTEX_TRI_ITEM*	pItem;
	
	pItem = (IVERTEX_TRI_ITEM*)LALAlloc(m_pTriItemPool);
	if (!pItem)
		goto lb_return;

	pItem->dwMaxFacesNum = dwFacesNum;
	pItem->dwVerticesNum = dwFacesNum*3;
	pItem->dwVerticesSize = pItem->dwVerticesNum*sizeof(IVERTEX);
	
#ifdef _DEBUG
	pItem->pIVList = (IVERTEX*)VHPDBGAlloc(m_pVertexHeap,NULL,pItem->dwVerticesSize,FALSE);
#else
	pItem->pIVList = (IVERTEX*)VHPAlloc(m_pVertexHeap,NULL,pItem->dwVerticesSize,FALSE);
#endif
	if (!pItem->pIVList)
	{
		LALFree(m_pTriItemPool,pItem);
		goto lb_return;

	}
	*ppIVList = pItem->pIVList;
	pItem->bRender = FALSE;
	pItem->dwRenderFlag = dwRenderFlag;

	pItem->pNext = NULL;
	pItem->pPrv = NULL;

	Link(pItem,&m_pItemListHead[NOT_RENDER_INDEX]);

	m_dwCurrentItemNum++;
	
lb_return:
#ifdef _DEBUG
	if (pItem->pNext == pItem)
		__asm int 3
#endif
	return pItem;
}

void CRenderTriBuffer::Link(IVERTEX_TRI_ITEM* pItem,IVERTEX_TRI_ITEM** ppLinkHeadItem)
{
	if (!*ppLinkHeadItem)
		*ppLinkHeadItem = pItem;
	else
	{
		pItem->pNext = *ppLinkHeadItem;
		(*ppLinkHeadItem)->pPrv = pItem;
	}

}
void CRenderTriBuffer::UnLink(IVERTEX_TRI_ITEM* pItem)
{
	IVERTEX_TRI_ITEM* pPrv = pItem->pPrv;
	IVERTEX_TRI_ITEM* pNext = pItem->pNext;
	if (pPrv)
		pPrv->pNext = pNext;

	if (pNext)
		pNext->pPrv = pPrv;

	if (m_pItemListHead[NOT_RENDER_INDEX] == pItem)
		m_pItemListHead[NOT_RENDER_INDEX] = NULL;

	if (m_pItemListHead[RENDER_INDEX] == pItem)
		m_pItemListHead[RENDER_INDEX] = NULL;
}
void CRenderTriBuffer::EnableRender(IVERTEX_TRI_ITEM* pItem,void* pMtlHandle,DWORD dwRenderFacesNum)
{
#ifdef _DEBUG
	if (pItem->pNext == pItem)
		__asm int 3
#endif

	if (dwRenderFacesNum < pItem->dwMaxFacesNum)
		pItem->dwRenderFacesNum = dwRenderFacesNum;


	if (pItem->bRender)
		return;

	UnLink(pItem);

#ifdef _DEBUG
	IVERTEX_TRI_ITEM* pCur;
	pCur = m_pItemListHead[NOT_RENDER_INDEX];
	while (pCur)
	{
		if (pCur == pItem)
			__asm int 3
		pCur = pCur->pNext;
	}
#endif

	Link(pItem,&m_pItemListHead[RENDER_INDEX]);
	pItem->bRender = TRUE;
	pItem->dwRenderFacesNum = dwRenderFacesNum;
	pItem->pMtlHandle = pMtlHandle;

#ifdef _DEBUG
	if (pItem->pNext == pItem)
		__asm int 3
#endif


}

void CRenderTriBuffer::DisableRender(IVERTEX_TRI_ITEM* pItem)
{
#ifdef _DEBUG
	if (pItem->pNext == pItem)
		__asm int 3
#endif
	if (!pItem->bRender)
		return;

	UnLink(pItem);
#ifdef _DEBUG
	IVERTEX_TRI_ITEM* pCur;
	pCur = m_pItemListHead[RENDER_INDEX];
	while (pCur)
	{
		if (pCur == pItem)
			__asm int 3
		pCur = pCur->pNext;
	}
#endif
	Link(pItem,&m_pItemListHead[NOT_RENDER_INDEX]);
	pItem->bRender = FALSE;

#ifdef _DEBUG
	if (pItem->pNext == pItem)
		__asm int 3
#endif

}
void CRenderTriBuffer::FreeBuffer(IVERTEX_TRI_ITEM* pItem)
{
#ifdef _DEBUG
	if (pItem->pNext == pItem)
		__asm int 3
#endif

	UnLink(pItem);

#ifdef _DEBUG
	IVERTEX_TRI_ITEM*	pCur = m_pItemListHead[RENDER_INDEX];
	while (pCur)
	{
		if (pCur == pItem)
			__asm int 3
		pCur = pCur->pNext;
	}
	pCur = m_pItemListHead[NOT_RENDER_INDEX];
	while (pCur)
	{
		if (pCur == pItem)
			__asm int 3
		pCur = pCur->pNext;
	}
#endif 

#ifdef _DEBUG
	VHPDBGFree(m_pVertexHeap,pItem->pIVList);
#else
	VHPFree(m_pVertexHeap,pItem->pIVList);
#endif

	LALFree(m_pTriItemPool,pItem);

	m_dwCurrentItemNum--;

#ifdef _DEBUG
	if (pItem->pNext == pItem)
		__asm int 3
#endif

}

void CRenderTriBuffer::Process()
{
	IVERTEX_TRI_ITEM*	pCur = m_pItemListHead[RENDER_INDEX];

	char	txt[512];
	while (pCur)
	{
#ifdef _DEBUG
		if (pCur->pNext == pCur)
			__asm int 3
		if (!pCur->bRender)
			__asm int 3

#endif
		m_pRenderer->RenderTriIvertex(pCur->pIVList,pCur->pMtlHandle,pCur->dwRenderFacesNum,pCur->dwRenderFlag);
	//	wsprintf(txt,"%x %x facesNum:%d renderFlag:%d\n",pCur->pIVList,pCur->pMtlHandle,pCur->dwRenderFacesNum,pCur->dwRenderFlag);
	//	OutputDebugString(txt);

		pCur = pCur->pNext;
	}
	

	
/*	char* pBuffer = m_Buffer.GetBufferPtr();

	IVERTEX_TRI_ITEM*	pTriItem;
	
	for (DWORD i=0; i<m_dwCurrentItemNum; i++)
	{
		pTriItem = (IVERTEX_TRI_ITEM*)pBuffer;
		pBuffer += ((IVERTEX_TRI_ITEM*)pBuffer)->dwItemSize;
	}
	m_dwCurrentItemNum = 0;
	m_Buffer.Clear();*/
}

CRenderTriBuffer::~CRenderTriBuffer()
{
	

	if (m_pTriItemPool)
	{
		ReleaseStaticMemoryPool(m_pTriItemPool);
		m_pTriItemPool = NULL;
	}
	
	if (m_pVertexHeap)
	{
#ifdef _DEBUG
		VHPHeapCheck(m_pVertexHeap);
		VHPLeakCheck(m_pVertexHeap);
#endif
		VHPReleaseHeap(m_pVertexHeap);
		m_pVertexHeap = NULL;

		delete [] m_pVertexMassMemory;
		m_pVertexMassMemory = NULL;
	}
}