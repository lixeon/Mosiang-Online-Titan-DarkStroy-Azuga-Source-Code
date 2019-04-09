// StaticVBPool.h: interface for the CStaticVBPool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATICVBPOOL_H__B8019B4B_0FCD_44B8_ABE5_EEFD86B9B506__INCLUDED_)
#define AFX_STATICVBPOOL_H__B8019B4B_0FCD_44B8_ABE5_EEFD86B9B506__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VBManager.h"
#include "VertexBuffer.h"
#include "../4DyuchiGXGFunc/global.h"
#include "StaticD3DResourcePool.h"


class CStaticVBPool  : public IVBManager
{
	CStaticD3DResourcePool	m_StaticD3DResoucePool;
	CD3DResourceManager*	m_pResourceManager;
	RESORUCE_POOL_TYPE		m_type;


	DWORD					m_dwVertexSize;
	D3DPOOL					m_D3DPOOL;
	DWORD					m_dwFVF;
	DWORD					m_dwAllocUnitVerticesNum;

	void					Cleanup();
	void					ClearMember();
public:
	BOOL			Initialize(CD3DResourceManager* pResourceManager,DWORD dwFVF,DWORD dwUsage,D3DPOOL pool,DWORD dwVertexSize,DWORD dwAllocUnitVerticesNum,DWORD dwMaxVBItemNum,DWORD dwUnitFreeVBItemNum);

	void			__stdcall OnLostDevice();
	void			__stdcall Reset();
	void			__stdcall Release();
	DWORD			__stdcall GetResourceType();

	BOOL			__stdcall AllocVB(DWORD* pdwErrorCode,CVertexBuffer* pVertexBuffer,DWORD dwVerticesNum,DWORD* pdwSize,BOOL* pbEnable,IResourceController* pResourceController);
	void			__stdcall ReleaseVB(void* pHandle);
	void			__stdcall ReleaseBufferAll();
	void			__stdcall SetTimeStamp(void* pHandle);


	DWORD			GetVertexBufferNum() {return m_StaticD3DResoucePool.GetD3DResourceBufferNum();}
	

	CStaticVBPool();
	~CStaticVBPool();

};

#endif // !defined(AFX_STATICVBPOOL_H__B8019B4B_0FCD_44B8_ABE5_EEFD86B9B506__INCLUDED_)
