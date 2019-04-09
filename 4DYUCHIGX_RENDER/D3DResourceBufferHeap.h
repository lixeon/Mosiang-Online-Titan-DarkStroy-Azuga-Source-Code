// D3DResourceBufferHeap.h: interface for the CD3DResourceBufferHeap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_D3DRESOURCEBUFFERHEAP_H__5BD71901_F6F1_459C_83A0_CE3022994D5E__INCLUDED_)
#define AFX_D3DRESOURCEBUFFERHEAP_H__5BD71901_F6F1_459C_83A0_CE3022994D5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "../4DyuchiGXGFunc/global.h"
#include "d3d_helper.h"


#include "VBManager.h"

class CoD3DDevice;
class CD3DResourceManager;

struct D3DRC_BUFFER_POOL_DESC
{
	DWORD					dwRefCount;
	IDirect3DResource8*		pBuffer;
};

enum RESOURCE_BUFFER_HEAP_FLAG
{	

	NOT_USE_AUTO_SCHEDLUE	= 0x00000002
};
struct D3DRC_HEAP_ITEM
{
	DWORD						dwD3DRCBufferVBPoolIndex;
	char*						pMem;
	BOOL*						pbEnable;				
	SORT_LINK					sortLink;
	DWORD						dwTimeStamp;
	IResourceController*		pResourceController;
};

class CD3DResourceBufferHeap
{
	CoD3DDevice*				m_pRenderer;
	CD3DResourceManager*		m_pResourceManager;
	VHEAP_HANDLE				m_pHeap;
	STMPOOL_HANDLE				m_pHeapItemDescPool;
	OnAllocBufferFunc			m_pAllocBufferFunc;
	MATRIX4						m_matViewPrv;
	DWORD						m_dwFlag;
	
	SORT_LINK*					m_pSortLinkHead;
	SORT_LINK*					m_pSortLinkTail;

	DWORD						m_dwMaxD3DRCBufferNum;
	D3DRC_BUFFER_POOL_DESC*		m_pD3DRCBufferPoolList;
	DWORD						m_dwSizePerD3DRCBufferPool;
	DWORD						m_dwUnitNumPerD3DRCBufferPool;
	DWORD						m_dwD3DRCBufferNum;
	DWORD						m_dwAllocatedItemNum;
	DWORD						m_dwFreeUnitNum;
	
	DWORD						m_dwUnitSize;
	D3DPOOL						m_D3DPOOL;
	DWORD						m_dwFVF;
	DWORD						m_dwUsage;
		

	char*						m_pMassMemory;
	DWORD						m_dwMassMemorySize;

	
	void					Update();
//	void					FreeItem(DWORD dwItemNum);

public:
	DWORD					GetD3DResourceBufferNum()	{return m_dwD3DRCBufferNum;}
	BOOL					Initialize(CD3DResourceManager* pResourceManager,DWORD dwFVF,DWORD dwUsage,D3DPOOL pool,DWORD dwUnitSize,DWORD dwMaxUnitsNumPerBuffer,DWORD dwMaxBufferNum,DWORD dwMaxSize,OnAllocBufferFunc pAllocBufferFunc,DWORD dwFlag);

	void					RemoveD3DRCBuffer();
	void					OnLostDevice();
	void					Reset();

	BOOL					AllocBuffer(
												DWORD*	pdwErrorCode,
												IDirect3DResource8** ppBuffer,
												void** ppHandle,
												DWORD* pdwStartIndex,
												DWORD* pdwSize,
												DWORD dwNum,
												BOOL* pbEnable,
												IResourceController* pResourceController
											);
	void					ReleaseBuffer(void* pHandle);
	void					ReleaseBufferAll();
	void					SetTimeStamp(void* pHandle);

	void					Cleanup();

	CD3DResourceBufferHeap();
	~CD3DResourceBufferHeap();

};

#endif // !defined(AFX_D3DRESOURCEBUFFERHEAP_H__5BD71901_F6F1_459C_83A0_CE3022994D5E__INCLUDED_)
