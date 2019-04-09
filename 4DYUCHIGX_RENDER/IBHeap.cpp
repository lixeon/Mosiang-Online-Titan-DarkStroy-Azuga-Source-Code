// IBHeap.cpp: implementation of the CIBHeap class.
//
//////////////////////////////////////////////////////////////////////
#include "D3DResourceManager.h"
#include "CoD3DDevice.h"
#include "global_variable.h"
#include "IBHeap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//#define _VHPHEAP_DEBUG
	

CIBHeap::CIBHeap()
{
	ClearMember();
}
void CIBHeap::ClearMember()
{
	m_D3DRCBufferHeap;
	m_pRenderer;
	m_pResourceManager;

	m_dwUsage;
	m_D3DPOOL;
	m_type = RESORUCE_POOL_TYPE_IBHEAP;
	m_dwItemNum = 0; 

}
BOOL CIBHeap::Initialize(CD3DResourceManager* pResourceManager,DWORD dwUsage,D3DPOOL pool,DWORD dwMaxIndicesNumPerBuffer,DWORD dwMaxBufferNum,DWORD dwMaxSize,DWORD dwFlag)
{
	BOOL	bResult = FALSE;
	m_pResourceManager = pResourceManager;

	
	m_dwUsage = dwUsage;
	m_D3DPOOL = pool;
		
	bResult = m_D3DRCBufferHeap.Initialize(pResourceManager,0,dwUsage,pool,sizeof(WORD),dwMaxIndicesNumPerBuffer,dwMaxBufferNum,dwMaxSize,AllocIBFunc,dwFlag);

lb_return:
	return bResult;
}

void __stdcall CIBHeap::OnLostDevice()
{
	m_D3DRCBufferHeap.OnLostDevice();
}
void __stdcall CIBHeap::Reset()
{
	m_D3DRCBufferHeap.Reset();

}
void __stdcall CIBHeap::Release()
{
	delete this;
}
BOOL __stdcall CIBHeap::AllocIB(DWORD* pdwErrorCode,CIndexBuffer* pIndexBuffer,DWORD dwIndicesNum,DWORD* pdwSize,BOOL* pbEnable,IResourceController* pResourceController)
{
	BOOL	bResult;
	bResult = m_D3DRCBufferHeap.AllocBuffer(pdwErrorCode,(IDirect3DResource8**)&pIndexBuffer->m_pIB,&pIndexBuffer->m_pIBHandle,&pIndexBuffer->m_dwStartIndex,NULL,dwIndicesNum,pbEnable,pResourceController);
	if (!bResult)
		goto lb_return;


	pIndexBuffer->m_pResourceManager = m_pResourceManager;
	pIndexBuffer->m_pIBManager = this;
	pIndexBuffer->m_dwIndicesNum = dwIndicesNum;
	if (pdwSize)
	{
		*pdwSize = dwIndicesNum * sizeof(WORD);
	}
	m_dwItemNum++;
	
lb_return:
	return bResult;
}
void __stdcall CIBHeap::ReleaseIB(void* pHandle)
{
	m_D3DRCBufferHeap.ReleaseBuffer(pHandle);
	m_dwItemNum--;
}
void __stdcall CIBHeap::ReleaseBufferAll()
{
	m_D3DRCBufferHeap.ReleaseBufferAll();

}
void __stdcall CIBHeap::RemoveD3DRCBuffer()
{
	m_D3DRCBufferHeap.RemoveD3DRCBuffer();

}
DWORD __stdcall CIBHeap::GetResourceType()
{
	return m_type;
}

void CIBHeap::Cleanup()
{
	m_D3DRCBufferHeap.Cleanup();
	ClearMember();
}

CIBHeap::~CIBHeap()
{
	Cleanup();

}
