#include "ProjectionLight.h"
#include "CoD3DDevice.h"


CProjectionLight::CProjectionLight()
{
	memset(this,0,sizeof(CProjectionLight));
}
BOOL CProjectionLight::Create(CoD3DDevice* pRenderer,DWORD dwRS,DWORD dwColor,char* szFileName,DWORD dwFlag)
{
	BOOL		bResult = TRUE;
	m_pRenderer = pRenderer;

//	szFileName = "psteam.tga";
	if (szFileName)
	{
		if (pRenderer->AllocTexture(&m_LoadedTexture,szFileName,0,FALSE,TRUE,TEXTURE_MAP_TYPE_DIFFUSE))
		{
			m_pSelectedTexture = m_LoadedTexture.GetTexture();
			goto lb_return;
		}
	}
	
	if (GenerateLightMap(pRenderer->GetDevice(),dwRS,dwColor,dwFlag))
	{
		m_pSelectedTexture = m_pGeneratedTexture;
		goto lb_return;
	}
	bResult = TRUE;
lb_return:
	return bResult;
}
BOOL CProjectionLight::GenerateLightMap(LPDIRECT3DDEVICE8 pDevice,DWORD dwRS,DWORD dwColor,DWORD dwFlag)
{
	BOOL		bResult = FALSE;

	DWORD		dwWidth = dwRS*2;

	HRESULT hr = m_pRenderer->GetD3DResourceManager()->CreateTexture(
		dwWidth,
		dwWidth,
		1,
		0,
		D3DFMT_R5G6B5,
		D3DPOOL_MANAGED,
		&m_pGeneratedTexture);

	if (D3D_OK != hr)
	{
		goto lb_return;
	}

	CreateAttenuationMap(m_pGeneratedTexture,dwWidth,dwWidth,dwRS,dwColor);

	bResult = TRUE;

lb_return:
	return bResult;
}


CProjectionLight::~CProjectionLight()
{
	if (m_pGeneratedTexture)
	{
		m_pRenderer->GetD3DResourceManager()->Release(m_pGeneratedTexture);
		m_pGeneratedTexture = NULL;
	}
}

void CProjectionLight::CreateAttenuationMap(LPDIRECT3DTEXTURE8 pTexture,DWORD dwWidth,DWORD dwHeight,DWORD dwRS,DWORD dwColor)
{
	D3DLOCKED_RECT	rect;
	WORD*			p;
	DWORD			x,y;

	int			iRSPow = (int)(dwRS*dwRS);
	int			iCenterX = (int)(dwWidth/2);
	int			iCenterY = (int)(dwHeight/2);
	int light_color[3];
	light_color[0] = (dwColor & 0x000000ff);
	light_color[1] = (dwColor & 0x0000ff00)>>8;
	light_color[2] = (dwColor & 0x00ff0000)>>16;
	
	HRESULT hr = pTexture->LockRect(0,&rect,NULL,0);

	for (y=0; y<dwHeight; y++)
	{
		memset((char*)rect.pBits + rect.Pitch*y,0,dwWidth*sizeof(WORD));
	}

	p = (WORD*)rect.pBits;
	
	for (y=1; y<dwHeight-1; y++)
	{
		for (x=1; x<dwWidth-1; x++)
		{
			int	iDistPow = (iCenterX-(int)x)*(iCenterX-(int)x) + (iCenterY-(int)y)*(iCenterY-(int)y);
			
			p = (WORD*)((char*)rect.pBits + x*2 + y*rect.Pitch);

			if (iDistPow < iRSPow)
			{

				float	fFallOff = (float)(iRSPow - iDistPow) / (float)iRSPow;
			
				*p = (WORD)
					( (int)( (float)light_color[2]*fFallOff )>>3<<11 ) |
					( (int)( (float)light_color[1]*fFallOff )>>2<<5  ) |
					( (int)( (float)light_color[0]*fFallOff )>>3 );
			}
			else
			{
				*p = 0;

			}
		}
	}



	pTexture->UnlockRect(0);

}
