// VBHeap.cpp: implementation of the CVBHeap class.
//
//////////////////////////////////////////////////////////////////////

#include "VBHeap.h"
#include "D3DResourceManager.h"
#include "CoD3DDevice.h"
#include "global_variable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//#define _VHPHEAP_DEBUG




CVBHeap::CVBHeap()
{
	ClearMember();
}
void CVBHeap::ClearMember()
{
	m_pRenderer = NULL;
	m_pResourceManager = NULL;
	m_dwUsage = 0;
	m_dwVertexSize = 0;
	m_pMassMemory = NULL;
	m_dwMassMemorySize = 0;
	m_type = RESORUCE_POOL_TYPE_VBHEAP;
}

BOOL CVBHeap::Initialize(CD3DResourceManager* pResourceManager,DWORD dwFVF,DWORD dwUsage,D3DPOOL pool,DWORD dwVertexSize,DWORD dwMaxVerticesNumPerBuffer,DWORD dwMaxBufferNum,DWORD dwMaxSize,DWORD dwFlag)
{
	BOOL	bResult = FALSE;
	m_pResourceManager = pResourceManager;

	
	m_dwFVF			= dwFVF;
	m_dwUsage		= dwUsage;
	m_dwVertexSize	= dwVertexSize;
	m_D3DPOOL		= pool;
	

	bResult = m_D3DRCBufferHeap.Initialize(pResourceManager,dwFVF,dwUsage,pool,m_dwVertexSize,dwMaxVerticesNumPerBuffer,dwMaxBufferNum,dwMaxSize,AllocVBFunc,dwFlag);

lb_return:
	return bResult;
}

void __stdcall CVBHeap::OnLostDevice()
{
	m_D3DRCBufferHeap.OnLostDevice();
}
void __stdcall CVBHeap::Reset()
{
	m_D3DRCBufferHeap.Reset();
}
void __stdcall CVBHeap::Release()
{
	delete this;
}

void __stdcall CVBHeap::SetTimeStamp(void* pHandle)
{
}
BOOL __stdcall CVBHeap::AllocVB(DWORD* pdwErrorCode,CVertexBuffer* pVertexBuffer,DWORD dwVerticesNum,DWORD* pdwSize,BOOL* pbEnable,IResourceController* pResourceController)
{
	BOOL	bResult;
	
	bResult = m_D3DRCBufferHeap.AllocBuffer(pdwErrorCode,(IDirect3DResource8**)&pVertexBuffer->m_pVB,&pVertexBuffer->m_pVBHandle,&pVertexBuffer->m_dwStartIndex,NULL,dwVerticesNum,pbEnable,pResourceController);
	if (!bResult)
		goto lb_return;


	pVertexBuffer->m_dwSize = m_dwVertexSize;
	pVertexBuffer->m_dwVertexShader = m_dwFVF;
	pVertexBuffer->m_pVBManager = this;
	pVertexBuffer->m_dwVerticesNum = dwVerticesNum;
	pVertexBuffer->m_pResourceManager = m_pResourceManager;
	if (pdwSize)
	{
		*pdwSize = dwVerticesNum * m_dwVertexSize;
	}

lb_return:
	return bResult;
}
void __stdcall CVBHeap::ReleaseVB(void* pHandle)
{
	m_D3DRCBufferHeap.ReleaseBuffer(pHandle);

}

DWORD __stdcall CVBHeap::GetResourceType()
{
	return m_type;
}



void __stdcall CVBHeap::ReleaseBufferAll()
{
	m_D3DRCBufferHeap.ReleaseBufferAll();
}

void CVBHeap::Cleanup()
{
	m_D3DRCBufferHeap.Cleanup();
	ClearMember();
}
CVBHeap::~CVBHeap()
{
	Cleanup();
}
