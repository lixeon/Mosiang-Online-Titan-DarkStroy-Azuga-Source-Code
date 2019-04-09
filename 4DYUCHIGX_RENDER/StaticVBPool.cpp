// StaticVBPool.cpp: implementation of the CStaticVBPool class.
//
//////////////////////////////////////////////////////////////////////

#include "StaticVBPool.h"
#include "D3DResourceManager.h"
#include "CoD3DDevice.h"
#include "../4DyuchiGRX_Common/IGeometry.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStaticVBPool::CStaticVBPool()
{
	ClearMember();
}


void CStaticVBPool::ClearMember()
{
	m_pResourceManager = NULL;
	m_dwAllocUnitVerticesNum = 0;
	m_type = RESORUCE_POOL_TYPE_VBSTATICPOOL;

}
BOOL CStaticVBPool::Initialize(CD3DResourceManager* pResourceManager,DWORD dwFVF,DWORD dwUsage,D3DPOOL pool,DWORD dwVertexSize,DWORD dwAllocUnitVerticesNum,DWORD dwMaxVBItemNum,DWORD dwUnitFreeVBItemNum)
{
	BOOL	bResult = FALSE;

	m_pResourceManager = pResourceManager;
	m_dwFVF = dwFVF;
	m_D3DPOOL = pool;
	m_dwVertexSize = dwVertexSize;
	m_dwAllocUnitVerticesNum = dwAllocUnitVerticesNum;
	

	bResult = m_StaticD3DResoucePool.Initialize(pResourceManager,dwFVF,dwUsage,pool,dwVertexSize,dwAllocUnitVerticesNum,dwMaxVBItemNum,dwUnitFreeVBItemNum,AllocVBFunc);

lb_return:
	return bResult;
}

void __stdcall CStaticVBPool::OnLostDevice()
{
	m_StaticD3DResoucePool.OnLostDevice();
}
void __stdcall CStaticVBPool::Reset()
{
	m_StaticD3DResoucePool.Reset();
}
void __stdcall CStaticVBPool::Release()
{
	delete this;
}
BOOL __stdcall CStaticVBPool::AllocVB(DWORD* pdwErrorCode,CVertexBuffer* pVertexBuffer,DWORD dwVerticesNum,DWORD* pdwSize,BOOL* pbEnable,IResourceController* pResourceController)
{

	BOOL	bResult;
	
	bResult = m_StaticD3DResoucePool.AllocBuffer((IDirect3DResource8**)&pVertexBuffer->m_pVB,&pVertexBuffer->m_pVBHandle,&pVertexBuffer->m_dwStartIndex,NULL,dwVerticesNum,pbEnable,pResourceController);
	if (!bResult)
		goto lb_return;

	pVertexBuffer->m_dwSize = m_dwVertexSize;
	pVertexBuffer->m_dwVertexShader = m_dwFVF;
	pVertexBuffer->m_pVBManager = this;
	pVertexBuffer->m_pResourceManager = m_pResourceManager;
	pVertexBuffer->m_dwVerticesNum = m_dwAllocUnitVerticesNum;


lb_return:
	return bResult;
}



void __stdcall CStaticVBPool::ReleaseVB(void* pHandle)
{
	m_StaticD3DResoucePool.ReleaseBuffer(pHandle);	
}
void __stdcall CStaticVBPool::ReleaseBufferAll()
{
	m_StaticD3DResoucePool.ReleaseBufferAll();

}
void CStaticVBPool::SetTimeStamp(void* pHandle)
{
	m_StaticD3DResoucePool.SetTimeStamp(pHandle);
}
DWORD __stdcall CStaticVBPool::GetResourceType()
{
	return m_type;
}

void CStaticVBPool::Cleanup()
{
	m_StaticD3DResoucePool.Cleanup();
}

CStaticVBPool::~CStaticVBPool()
{
	Cleanup();
	ClearMember();
}
