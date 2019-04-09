// StaticIBPool.h: interface for the CStaticIBPool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATICIBPOOL_H__D57CDEF7_E018_4FDE_87EF_0DF06B276660__INCLUDED_)
#define AFX_STATICIBPOOL_H__D57CDEF7_E018_4FDE_87EF_0DF06B276660__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IBManager.h"
#include "IndexBuffer.h"
#include "../4DyuchiGXGFunc/global.h"
#include "StaticD3DResourcePool.h"

class CStaticIBPool  : public IIBManager
{
	CStaticD3DResourcePool	m_StaticD3DResoucePool;
	CD3DResourceManager*	m_pResourceManager;
	RESORUCE_POOL_TYPE		m_type;


	D3DPOOL					m_D3DPOOL;
	DWORD					m_dwAllocUnitInidicesNum;

	void					Cleanup();
	void					ClearMember();
public:
	
	BOOL			Initialize(CD3DResourceManager* pResourceManager,DWORD dwUsage,D3DPOOL pool,DWORD dwAllocUnitIndicesNum,DWORD dwMaxIBItemNum,DWORD dwFreeIBItemNum);

	void			__stdcall OnLostDevice();
	void			__stdcall Reset();
	void			__stdcall Release();
	DWORD			__stdcall GetResourceType();

	BOOL			__stdcall AllocIB(DWORD* pdwErrorCode,CIndexBuffer* pIndexBuffer,DWORD dwIndicesNum,DWORD* pdwSize,BOOL* pbEnable,IResourceController* pResourceController);
	void			__stdcall ReleaseIB(void* pHandle);
	void			__stdcall ReleaseBufferAll();
	void			__stdcall SetTimeStamp(void* pHandle);


	DWORD			GetInexBufferNum() {return m_StaticD3DResoucePool.GetD3DResourceBufferNum();}
	
	CStaticIBPool();
	~CStaticIBPool();
};

#endif // !defined(AFX_STATICIBPOOL_H__D57CDEF7_E018_4FDE_87EF_0DF06B276660__INCLUDED_)
