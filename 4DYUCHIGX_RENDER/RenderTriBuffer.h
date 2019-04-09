// RenderTriBuffer.h: interface for the CRenderTriBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RENDERTRIBUFFER_H__9C595BF0_E46D_4160_AFFC_D02CE8E5B1BB__INCLUDED_)
#define AFX_RENDERTRIBUFFER_H__9C595BF0_E46D_4160_AFFC_D02CE8E5B1BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4DyuchiGRX_Common/IRenderer.h"
#include "../4DyuchiGXGFunc/global.h"


struct IVERTEX_TRI_ITEM
{
	BOOL				bRender;
	IVERTEX*			pIVList;
	DWORD				dwVerticesSize;
	DWORD				dwVerticesNum;
	DWORD				dwMaxFacesNum;
	DWORD				dwRenderFacesNum;
	void*				pMtlHandle;
	DWORD				dwRenderFlag;
	
	IVERTEX_TRI_ITEM*	pPrv;
	IVERTEX_TRI_ITEM*	pNext;
	
};

#define IVERTEX_TRI_ITEM_HEADER_SIZE	( sizeof(IVERTEX_TRI_ITEM) - sizeof(IVERTEX) )

#define RENDER_INDEX		0
#define NOT_RENDER_INDEX	1
/*
// various heap
GLOBAL_FUNC_DLL		VHEAP_HANDLE		__stdcall	;
GLOBAL_FUNC_DLL		void				__stdcall	VHPHeapCheck(VHEAP_HANDLE pVHeap);
GLOBAL_FUNC_DLL		void				__stdcall	VHPLeakCheck(VHEAP_HANDLE pVHeap);

GLOBAL_FUNC_DLL		void* 				__stdcall	VHPAllocUnit(VHEAP_HANDLE pVHeap,DWORD dwUnitNum,DWORD* pdwSize);

GLOBAL_FUNC_DLL		void*				__stdcall	VHPDBGAlloc(VHEAP_HANDLE pVHeap,DWORD dwSize);
GLOBAL_FUNC_DLL		void* 				__stdcall	VHPDBGAllocUnit(VHEAP_HANDLE pVHeap,DWORD dwUnitNum,DWORD* pdwSize);
GLOBAL_FUNC_DLL		void				__stdcall 	VHPDBGFree(VHEAP_HANDLE pVHeap,void* pMem);
GLOBAL_FUNC_DLL		void				__stdcall	(VHEAP_HANDLE pVHeap);


GLOBAL_FUNC_DLL STMPOOL_HANDLE	__stdcall	();


GLOBAL_FUNC_DLL void*			__stdcall	LALAlloc(STMPOOL_HANDLE pool);
GLOBAL_FUNC_DLL void			__stdcall	LALFree(STMPOOL_HANDLE pool,void* pMemory);
*/
class CoD3DDevice;
class CRenderTriBuffer  
{
	IVERTEX_TRI_ITEM*	m_pItemListHead[2];
	STMPOOL_HANDLE		m_pTriItemPool;
	VHEAP_HANDLE		m_pVertexHeap;

	

	CoD3DDevice*		m_pRenderer;
	char*				m_pVertexMassMemory;	
	DWORD				m_dwVertexMassMemorySize;
	DWORD				m_dwCurrentItemNum;
	void				Link(IVERTEX_TRI_ITEM* pItem,IVERTEX_TRI_ITEM** ppLinkHeadItem);
	void				UnLink(IVERTEX_TRI_ITEM* pItem);
public:
	BOOL				Initialize(CoD3DDevice* pRenderer,DWORD dwMaxTriItemNum,DWORD dwMaxTriNum);

	IVERTEX_TRI_ITEM*	AllocBuffer(IVERTEX** ppIVList,DWORD dwFacesNum,DWORD dwRenderFlag);
	void				EnableRender(IVERTEX_TRI_ITEM* pItem,void* pMtlHandle,DWORD dwRenderFacesNum);
	void				DisableRender(IVERTEX_TRI_ITEM* pItem);
	void				FreeBuffer(IVERTEX_TRI_ITEM* pItem);

	void				Process();

public:
	CRenderTriBuffer();
	~CRenderTriBuffer();

};









#endif // !defined(AFX_RENDERTRIBUFFER_H__9C595BF0_E46D_4160_AFFC_D02CE8E5B1BB__INCLUDED_)
