#include "d3dtexture.h"
#include "texture_manager.h"
#include "D3DResourceManager.h"


CBaseTextureContainer::CBaseTextureContainer()
{
	m_pD3DTexture = NULL;
	m_pBackupTexture = NULL;
	m_pTextureManager = NULL;
	
	m_dwRefCount = 0;
	m_dwTexFlag = 0;
	m_bFromFile = FALSE;
	m_pSearchHandle = NULL;
	m_ImageHeader.dwWidth = 0;
	m_ImageHeader.dwHeight = 0;
	m_ImageHeader.dwBPS = 0;
	m_ImageHeader.dwPitch = 0;
	m_dwIndexInSortTable = 0xffffffff;
	m_dwPageFlag = 0;
	

}



BOOL CBaseTextureContainer::CreateImage(IMAGE_HEADER* pHeader,char** ppBits)
{
	LPDIRECT3DTEXTURE8	pTex;
	IDirect3DSurface8*	pSurface;
	BOOL				bResult = FALSE;
	DWORD				dwWidth;
	DWORD				dwHeight;
	DWORD				dwBytesPerPixel;

	if (m_pD3DTexture)
		pTex = m_pD3DTexture;
	else
		pTex = m_pBackupTexture;

	if (!pTex)
		goto lb_return;


	pTex->GetSurfaceLevel(0,&pSurface);

	m_pTextureManager->GetD3DResourceManager()->CreateBitmapWithSurface(ppBits,&dwWidth,&dwHeight,&dwBytesPerPixel,pSurface);
	
	pHeader->dwBPS = 2;
	pHeader->dwHeight = dwHeight;
	pHeader->dwWidth = dwWidth;
	pHeader->dwPitch = dwWidth*2;

	pSurface->Release();

	bResult = TRUE;
lb_return:
	return bResult;
}
BOOL CBaseTextureContainer::SwapToSystemMemory()
{

	BOOL	bResult = FALSE;
	if (!m_pTextureManager->GetD3DResourceManager()->DuplicateTexture(&m_pBackupTexture,D3DPOOL_SCRATCH,m_pD3DTexture,0))
		goto lb_return;


	m_pTextureManager->GetD3DResourceManager()->Release(m_pD3DTexture);
	m_pD3DTexture = NULL;
	m_dwPageFlag = 1;

	bResult = TRUE;
lb_return:
	return bResult;
}
BOOL CBaseTextureContainer::SwapToDriverMemory()
{
	BOOL	bResult = FALSE;
	if (!m_pTextureManager->GetD3DResourceManager()->DuplicateTexture(&m_pD3DTexture,D3DPOOL_MANAGED,m_pBackupTexture,0))
		goto lb_return;


	m_pTextureManager->GetD3DResourceManager()->Release(m_pBackupTexture);
	m_pBackupTexture = NULL;
	m_dwPageFlag = 0;

	bResult = TRUE;
lb_return:
	return bResult;

/*
	IDirect3DSurface8*		pSrc;
	IDirect3DSurface8*		pDest;

	BOOL				bResult = FALSE;
	DWORD				i;
	HRESULT hr;
	D3DSURFACE_DESC		ddsc;
			
	memset(&ddsc,0,sizeof(ddsc));
	m_pD3DTexture->GetLevelDesc(0,&ddsc);
	DWORD	miplevelnum = m_pD3DTexture->GetLevelCount();

	hr = m_pResourceManager->CreateTexture(
		ddsc.Width,
		ddsc.Height,
		miplevelnum,
		ddsc.Usage,
		ddsc.Format,
		D3DPOOL_SCRATCH,
		&m_pBackupTexture);

	if (D3D_OK != hr)
		goto lb_return;


	BYTE*	pOffset = m_pRawData;

	for (i=0; i<miplevel; i++)
	{
		D3DSURFACE_DESC		ddscSub;
		m_pD3DTexture->GetLevelDesc(i,&ddscSub);

		pOffset

		dwTotalSize += ddscSub.Size;

	}
	


	for (i=0; i<miplevelnum; i++)
	{

		pTexSrc->GetSurfaceLevel(i,&pSrc);
		(*ppTexDest)->GetSurfaceLevel(i,&pDest);
		
		m_pD3DDevice->CopyRects(pSrc,NULL,0,pDest,NULL);

		pSrc->Release();
		pDest->Release();

	}
	bResult = TRUE;
lb_return:
	return bResult;*/
	return TRUE;
}

LPDIRECT3DTEXTURE8 CBaseTextureContainer::GetTexture()
{
//	if (m_dwPageFlag)
//		m_pTextureManager->UpdateTexture(this);
	
	return m_pD3DTexture;	
}
void CBaseTextureContainer::SetTexture(LPDIRECT3DDEVICE8 pDevice,DWORD dwTexStageIndex)
{
//	if (m_dwPageFlag)
//		m_pTextureManager->UpdateTexture(this);
	pDevice->SetTexture(dwTexStageIndex,m_pD3DTexture);
}
void CBaseTextureContainer::ReleaseImage(char* pBits)
{
	m_pTextureManager->GetD3DResourceManager()->ReleaseBitmapWithSurface(pBits);
}
DWORD CBaseTextureContainer::AddRef()
{
	m_dwRefCount++;
	return m_dwRefCount;
}
DWORD CBaseTextureContainer::Release()
{
	m_dwRefCount--;
	if (!m_dwRefCount)
		delete this;

	return 0;

}
CBaseTextureContainer::~CBaseTextureContainer()
{
	if (m_pBackupTexture)
	{
		m_pTextureManager->GetD3DResourceManager()->Release(m_pBackupTexture);
		m_pBackupTexture = NULL;
	}
	if (m_pD3DTexture)
	{
		m_pTextureManager->GetD3DResourceManager()->Release(m_pD3DTexture);
		m_pD3DTexture = NULL;
	}
	m_pTextureManager->FreeTexture(this);
}

CTextureContainer::CTextureContainer()
{
	memset(m_szFileName,0,sizeof(m_szFileName));
}