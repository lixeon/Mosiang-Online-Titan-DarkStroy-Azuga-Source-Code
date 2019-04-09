// RenderTexturePool.h: interface for the CRenderTexturePool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RENDERTEXTUREPOOL_H__97EF8E81_5323_48B9_9B2E_809B3F94CB23__INCLUDED_)
#define AFX_RENDERTEXTUREPOOL_H__97EF8E81_5323_48B9_9B2E_809B3F94CB23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RenderTexture.h"

class CoD3DDevice;
class CRenderTexturePool  
{
	CoD3DDevice*				m_pRenderer;
	CRenderTexture*				m_pRenderTextureList;
	CRenderTexture**			m_ppAvailableRenderTextureList;
	CRenderTexture**			m_ppAllocatedRenderTextureList;
	

	DWORD						m_dwTexelSize;
	DWORD						m_dwMaxRenderTextureNum;
	DWORD						m_dwAllocatedRenderTextureNum;
	DWORD						m_dwAvailableRenderTextureNum;
	BOOL						m_bUseRenderTexure;

	void						ClearMember();

public:
	void						Enable(BOOL	bUse);
	void						Cleanup();

	BOOL						BeginShadowMap();
	void						EndShadowMap();

	BOOL						BeginCreateProjectionMap(DWORD dwShadowTextureIndex,DWORD dwFlag);
	BOOL						EndCreateProjectionMap(DWORD dwShadowTextureIndex);
	void						SetMustUpdate(BOOL bMustUpdate);

	BOOL						Initialize(CoD3DDevice* pRenderer,DWORD dwTexelSize,DWORD dwMaxRenderTextureNum,DWORD dwFlag);
	CRenderTexture*				AllocRenderTexture(CRenderTexture** ppRenderTextureContainer,BOOL* pbUseRenderTextureContainer);
	void						FreeAllRenderTexture();
	CRenderTexturePool();
	~CRenderTexturePool();

};

#endif // !defined(AFX_RENDERTEXTUREPOOL_H__97EF8E81_5323_48B9_9B2E_809B3F94CB23__INCLUDED_)
