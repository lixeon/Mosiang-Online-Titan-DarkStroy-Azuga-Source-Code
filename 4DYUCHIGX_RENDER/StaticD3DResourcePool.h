// StaticD3DResourcePool.h: interface for the CStaticD3DResourcePool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATICD3DRESOURCEPOOL_H__FBE05824_F856_4055_9ED4_2B9D635C235A__INCLUDED_)
#define AFX_STATICD3DRESOURCEPOOL_H__FBE05824_F856_4055_9ED4_2B9D635C235A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "d3d_helper.h"
#include "VertexBuffer.h"
#include "../4DyuchiGXGFunc/global.h"


struct STATIC_D3DRC_BUFFER_POOL_LINK
{	
	DWORD						dwAvailableUnitsNum;
	DWORD						dwStartOffset;
	IDirect3DResource8*			pBuffer;
	STATIC_D3DRC_BUFFER_POOL_LINK*			pNext;
};


struct STATIC_D3DRC_BUFFER_ITEM
{
	IDirect3DResource8*			pBuffer;
	DWORD						dwStartIndex;
	DWORD						dwUnitsNum;
	
	STATIC_D3DRC_BUFFER_ITEM*	pNext;
	BOOL*						pbEnable;				
	SORT_LINK					sortLink;
	DWORD						dwTimeStamp;
	IResourceController*		pResourceController;
};



class CoD3DDevice;
class CStaticD3DResourcePool
{

	CoD3DDevice*			m_pRenderer;
	CD3DResourceManager*	m_pResourceManager;

	STATIC_D3DRC_BUFFER_POOL_LINK*		m_pD3DRCBufferPoolList;
	STATIC_D3DRC_BUFFER_ITEM*			m_pItemList;

	STATIC_D3DRC_BUFFER_POOL_LINK*		m_pD3DRCBufferPoolHead;
	STATIC_D3DRC_BUFFER_ITEM*			m_pItemHead;

	SORT_LINK*				m_pSortLinkHead;
	SORT_LINK*				m_pSortLinkTail;

	MATRIX4					m_matViewPrv;
	D3DPOOL					m_D3DPOOL;
	DWORD					m_dwUsage;
	DWORD					m_dwFVF;
	DWORD					m_dwUnitSize;

	
	DWORD					m_dwAllocUnitsNum;
	

	DWORD					m_dwD3RCBufferNum;
	DWORD					m_dwMaxMemorySize;

	DWORD					m_dwAllocatedItemNum;
	DWORD					m_dwReservedItemNum;
	DWORD					m_dwMaxItemNum;
	DWORD					m_dwUnitFreeItemNum;
	OnAllocBufferFunc		m_pAllocBufferFunc;
	
	void					Update();
	void					FreeItem(DWORD dwItemNum);
	void					ClearMember();
public:

	BOOL			Initialize(CD3DResourceManager* pResourceManager,DWORD dwFVF,DWORD dwUsage,D3DPOOL pool,DWORD dwUnitSize,DWORD dwAllocUnitsNum,DWORD dwMaxItemNum,DWORD dwUnitFreeItemNum,OnAllocBufferFunc pAllocFunc);

	void			 OnLostDevice();
	void			 Reset();
	


	BOOL			AllocBuffer(
									IDirect3DResource8** ppBuffer,
									void** ppHandle,
									DWORD* pdwStartIndex,
									DWORD* pdwSize,
									DWORD dwNum,
									BOOL* pbEnable,	
									IResourceController* pResourceController
								);

	void			ReleaseBuffer(void* pHandle);
	void			ReleaseBufferAll();

	void			SetTimeStamp(void* pHandle);

	DWORD			GetD3DResourceBufferNum() {return m_dwD3RCBufferNum;}
	void			Cleanup();

	CStaticD3DResourcePool();
	~CStaticD3DResourcePool();

};


#endif // !defined(AFX_STATICD3DRESOURCEPOOL_H__FBE05824_F856_4055_9ED4_2B9D635C235A__INCLUDED_)
