#include "RenderTexture.h"
#include "CoD3DDevice.h"


CRenderTexture::CRenderTexture()
{
	memset(this,0,sizeof(CRenderTexture));
}

BOOL CRenderTexture::BeginRender(DWORD dwFlag)
{
	

	BOOL		bResult = FALSE;

	if (!m_bMustUpdate)
		goto lb_return;

	LPDIRECT3DDEVICE8	pDevice;
	pDevice = m_pDevice->GetDevice();


	pDevice->GetRenderTarget(&m_pOldSurface);
	pDevice->GetDepthStencilSurface(&m_pOldZBuffer);
	pDevice->SetRenderTarget(m_pSurface,m_pZBuffer);


	D3DRECT		rect;

	rect.x1 = 1;
	rect.x2 = m_dwWidth-1;
	rect.y1 = 1;
	rect.y2 = m_dwHeight-1;

	pDevice->SetViewport(&m_vp);
	if (dwFlag & RENDER_TEXTURE_CLEAR)
	{
		pDevice->Clear(0,NULL,D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, D3DCOLOR_XRGB(255,255,255), 0.0f, 0 );
		pDevice->Clear(1,&rect,D3DCLEAR_ZBUFFER ,0, 1.0f, 0 );
		
	//	pDevice->Clear(0,NULL,D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, D3DCOLOR_XRGB(255,255,255), 0.0f, 0 );
	//	pDevice->Clear(1,&rect,D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,D3DCOLOR_XRGB(255,0,0,), 0.0f, 0 );

	}
	bResult = TRUE;

lb_return:
	return bResult;

}

void CRenderTexture::EndRender()
{
	if (!m_bMustUpdate)
		goto lb_return;

	LPDIRECT3DDEVICE8	pDevice;
	pDevice = m_pDevice->GetDevice();

	pDevice->SetRenderTarget(m_pOldSurface,m_pOldZBuffer);	
	m_pOldSurface->Release();
	m_pOldZBuffer->Release();

lb_return:
	return;
}
	
BOOL CRenderTexture::Clear(DWORD dwFlag)
{

	BOOL		bResult = FALSE;

	if (!m_bMustUpdate)
		goto lb_return;

	LPDIRECT3DDEVICE8	pDevice;
	pDevice = m_pDevice->GetDevice();


	pDevice->GetRenderTarget(&m_pOldSurface);
	pDevice->GetDepthStencilSurface(&m_pOldZBuffer);
	pDevice->SetRenderTarget(m_pSurface,m_pZBuffer);


	D3DRECT		rect;

	rect.x1 = 1;
	rect.x2 = m_dwWidth-1;
	rect.y1 = 1;
	rect.y2 = m_dwHeight-1;

	pDevice->SetViewport(&m_vp);
	
	pDevice->Clear(0,NULL,D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, D3DCOLOR_XRGB(255,255,255), 0.0f, 0 );
	pDevice->Clear(1,&rect,D3DCLEAR_ZBUFFER ,0, 1.0f, 0 );
	

	pDevice->SetRenderTarget(m_pOldSurface,m_pOldZBuffer);	
	m_pOldSurface->Release();
	m_pOldZBuffer->Release();

	bResult = TRUE;

lb_return:
	return bResult;
}

BOOL CRenderTexture::Create(CoD3DDevice* pDev,DWORD dwWidthHeight)
{
	D3DDISPLAYMODE mode;
	m_pDevice = pDev;
	LPDIRECT3DDEVICE8	pDevice = m_pDevice->GetDevice();

	m_dwWidth = dwWidthHeight;
	m_dwHeight = dwWidthHeight;
	pDevice->GetDisplayMode(&mode);		
	m_pDevice->GetD3DResourceManager()->CreateTexture(dwWidthHeight,dwWidthHeight,1,D3DUSAGE_RENDERTARGET,mode.Format,D3DPOOL_DEFAULT,&m_pTexture);
	m_pTexture->GetSurfaceLevel(0, &m_pSurface);

	pDevice->CreateDepthStencilSurface(dwWidthHeight,dwWidthHeight,D3DFMT_D16,D3DMULTISAMPLE_NONE,&m_pZBuffer);	

	m_vp.X      = 0;
	m_vp.Y      = 0;
	m_vp.Width  = dwWidthHeight;
	m_vp.Height = dwWidthHeight;
	m_vp.MinZ   = 0.0f;
	m_vp.MaxZ   = 1.0f;

	m_pDefaultTexture = pDev->GetDefaultMaterial()->TextureDiffuse.GetTexture();

	return TRUE;
}


void CRenderTexture::Cleanup()
{
	if (m_ppOwner)
		(*m_ppOwner) = NULL;

	if (m_pTexture)
	{
		m_pDevice->GetD3DResourceManager()->Release(m_pTexture);
		m_pTexture = NULL;
	}
	if (m_pSurface)
	{
		m_pSurface->Release();
		m_pSurface = NULL;
	}
	if (m_pZBuffer)
	{
		m_pZBuffer->Release();
		m_pZBuffer = NULL;
	}
}
CRenderTexture::~CRenderTexture()
{
	Cleanup();
}
