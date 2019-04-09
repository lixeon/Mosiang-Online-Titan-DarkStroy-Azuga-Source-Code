// RenderTexturePool.cpp: implementation of the CRenderTexturePool class.
//
//////////////////////////////////////////////////////////////////////

#include "RenderTexturePool.h"
#include "CoD3DDevice.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRenderTexturePool::CRenderTexturePool()
{
	ClearMember();

}
void CRenderTexturePool::ClearMember()
{
	m_pRenderer = NULL;
	m_pRenderTextureList = NULL;
	m_ppAvailableRenderTextureList = NULL;
	m_ppAllocatedRenderTextureList = NULL;

	m_dwTexelSize = 0;
	m_dwMaxRenderTextureNum = 0;
	m_dwAllocatedRenderTextureNum = 0;
	m_dwAvailableRenderTextureNum = 0;

}


BOOL CRenderTexturePool::Initialize(CoD3DDevice* pRenderer,DWORD dwTexelSize,DWORD dwMaxRenderTextureNum,DWORD dwFlag)
{
	// ½¦µµ¿ì¸Ê »ý¼º

	BOOL		bResult = FALSE;

	m_pRenderer = pRenderer;
	m_dwTexelSize = dwTexelSize;
	m_dwMaxRenderTextureNum = dwMaxRenderTextureNum;
	m_dwAvailableRenderTextureNum = dwMaxRenderTextureNum;
		
	m_pRenderTextureList = new CRenderTexture[dwMaxRenderTextureNum];
	m_ppAvailableRenderTextureList = new CRenderTexture*[dwMaxRenderTextureNum];
	m_ppAllocatedRenderTextureList = new CRenderTexture*[dwMaxRenderTextureNum];

	memset(m_ppAvailableRenderTextureList,0,sizeof(CRenderTexture*)*dwMaxRenderTextureNum);
	memset(m_ppAllocatedRenderTextureList,0,sizeof(CRenderTexture*)*dwMaxRenderTextureNum);

	

	for (DWORD i=0; i<dwMaxRenderTextureNum; i++)
	{
		m_pRenderTextureList[i].Create(pRenderer,dwTexelSize);
		m_ppAvailableRenderTextureList[i] = m_pRenderTextureList+i;
	}

	bResult = TRUE;
lb_return:
	return bResult;

}
BOOL CRenderTexturePool::BeginShadowMap()
{


	for (DWORD i=0; i<m_dwAllocatedRenderTextureNum; i++)
	{
		m_ppAllocatedRenderTextureList[i]->Clear(RENDER_TEXTURE_CLEAR);
	//	m_ppAllocatedRenderTextureList[i]->BeginRender(RENDER_TEXTURE_CLEAR);
	//	m_ppAllocatedRenderTextureList[i]->EndRender();

	}
	return TRUE;
}


void CRenderTexturePool::EndShadowMap()
{
	
}

BOOL CRenderTexturePool::BeginCreateProjectionMap(DWORD dwShadowTextureIndex,DWORD dwFlag)
{
	BOOL	bResult = FALSE;
	
	if (dwShadowTextureIndex >= m_dwAllocatedRenderTextureNum)
		goto lb_return;
		
	if (!m_ppAllocatedRenderTextureList[dwShadowTextureIndex])
		goto lb_return;

	bResult = m_ppAllocatedRenderTextureList[dwShadowTextureIndex]->BeginRender(dwFlag);

lb_return:
	return bResult;
}

BOOL CRenderTexturePool::EndCreateProjectionMap(DWORD dwShadowTextureIndex)
{
	BOOL	bResult = FALSE;

	if (dwShadowTextureIndex >= m_dwAllocatedRenderTextureNum)
		goto lb_return;

	if (!m_ppAllocatedRenderTextureList[dwShadowTextureIndex])
		goto lb_return;

	m_ppAllocatedRenderTextureList[dwShadowTextureIndex]->EndRender();

lb_return:
	return bResult;
}

void CRenderTexturePool::SetMustUpdate(BOOL bMustUpdate)
{
	for (DWORD i=0; i<m_dwAllocatedRenderTextureNum; i++)
	{
		m_ppAllocatedRenderTextureList[i]->SetMustUpdate(bMustUpdate);
	}
}

void CRenderTexturePool::Enable(BOOL bUse)
{
	m_bUseRenderTexure = bUse;
	for (DWORD i=0; i<m_dwMaxRenderTextureNum; i++)
	{
		m_pRenderTextureList[i].Enable(bUse);
	}

}

void CRenderTexturePool::Cleanup()
{

	delete [] m_pRenderTextureList;
	delete [] m_ppAvailableRenderTextureList;
	delete [] m_ppAllocatedRenderTextureList;
	ClearMember();
}
CRenderTexturePool::~CRenderTexturePool()
{
	Cleanup();

}
/*
CRenderTexture* CoD3DDevice::CreateRenderSurface(DWORD dwWidthHeight)
{
	CRenderTexture* pTexture = new CRenderTexture;
	
	if (!pTexture->Create(this,dwWidthHeight))
	{
		delete pTexture;
	}
	return pTexture;
}*/

CRenderTexture*	CRenderTexturePool::AllocRenderTexture(CRenderTexture** ppRenderTextureContainer,BOOL* pbUseRenderTextureContainer)
{
/*
	CRenderTexture*				m_ppRenderTextureList[MAX_RENDER_TEXTURE_NUM];
	DWORD						m_dwDefaultTexelSize;
	DWORD						m_dwMaxRenderTextureNum;
	DWORD						m_dwAllocatedRenderTextureNum;
	DWORD						m_dwAvailableRenderTextureNum;
*/
	CRenderTexture*	pRenderTexture = NULL;


	if (!m_bUseRenderTexure)
		goto lb_return;

	if (!m_dwAvailableRenderTextureNum)
		goto lb_return;


	pRenderTexture = m_ppAvailableRenderTextureList[0];
	m_ppAllocatedRenderTextureList[m_dwAllocatedRenderTextureNum] = pRenderTexture;

	m_ppAvailableRenderTextureList[0] = m_ppAvailableRenderTextureList[m_dwAvailableRenderTextureNum-1];
	m_ppAvailableRenderTextureList[m_dwAvailableRenderTextureNum-1] = NULL;

	m_dwAllocatedRenderTextureNum++;
	m_dwAvailableRenderTextureNum--;

	
	pRenderTexture->MarkAllocated(ppRenderTextureContainer,pbUseRenderTextureContainer);
	pRenderTexture->SetMustUpdate(TRUE);
	

lb_return:
	return pRenderTexture;
}
void CRenderTexturePool::FreeAllRenderTexture()
{
}