// StaticIBPool.cpp: implementation of the CStaticIBPool class.
//
//////////////////////////////////////////////////////////////////////

#include "StaticIBPool.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStaticIBPool::CStaticIBPool()
{
	ClearMember();
}


void CStaticIBPool::ClearMember()
{
	m_pResourceManager = NULL;
	m_dwAllocUnitInidicesNum = 0;
	m_type = RESORUCE_POOL_TYPE_IBSTATICPOOL;
}
BOOL CStaticIBPool::Initialize(CD3DResourceManager* pResourceManager,DWORD dwUsage,D3DPOOL pool,DWORD dwAllocUnitIndicesNum,DWORD dwMaxIBItemNum,DWORD dwFreeIBItemNum)
{
	m_pResourceManager = pResourceManager;
	m_dwAllocUnitInidicesNum = dwAllocUnitIndicesNum;
	m_D3DPOOL = pool;

	BOOL	bResult = m_StaticD3DResoucePool.Initialize(pResourceManager,0,dwUsage,pool,sizeof(WORD),dwAllocUnitIndicesNum,dwMaxIBItemNum,dwFreeIBItemNum,AllocIBFunc);
lb_return:
	return bResult;
}

void __stdcall CStaticIBPool::OnLostDevice()
{
	m_StaticD3DResoucePool.OnLostDevice();

}
void __stdcall CStaticIBPool::Reset()
{
	m_StaticD3DResoucePool.Reset();
}
void __stdcall CStaticIBPool::Release()
{
	delete this;
}
BOOL __stdcall CStaticIBPool::AllocIB(DWORD* pdwErrorCode,CIndexBuffer* pIndexBuffer,DWORD dwIndicesNum,DWORD* pdwSize,BOOL* pbEnable,IResourceController* pResourceController)
{
	BOOL	bResult = m_StaticD3DResoucePool.AllocBuffer((IDirect3DResource8**)&pIndexBuffer->m_pIB,&pIndexBuffer->m_pIBHandle,&pIndexBuffer->m_dwStartIndex,NULL,dwIndicesNum,pbEnable,pResourceController);
	if (!bResult)
		goto lb_return;

	pIndexBuffer->m_dwIndicesNum = m_dwAllocUnitInidicesNum;
	pIndexBuffer->m_pIBManager = this;
	pIndexBuffer->m_pResourceManager = m_pResourceManager;
	
lb_return:
	return bResult;

}
void __stdcall CStaticIBPool::ReleaseIB(void* pHandle)
{
	m_StaticD3DResoucePool.ReleaseBuffer(pHandle);
}
void __stdcall CStaticIBPool::SetTimeStamp(void* pHandle)
{
	m_StaticD3DResoucePool.SetTimeStamp(pHandle);

}

void __stdcall CStaticIBPool::ReleaseBufferAll()
{
	m_StaticD3DResoucePool.ReleaseBufferAll();
}
void CStaticIBPool::Cleanup()
{
	m_StaticD3DResoucePool.Cleanup();
	ClearMember();
}

DWORD __stdcall CStaticIBPool::GetResourceType()
{
	return m_type;
}


CStaticIBPool::~CStaticIBPool()
{
	Cleanup();

}
