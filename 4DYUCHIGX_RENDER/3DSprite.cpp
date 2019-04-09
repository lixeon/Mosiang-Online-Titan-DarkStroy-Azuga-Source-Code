#include "3Dsprite.h"
#include "CoD3DDevice.h"
#include "IBHeap.h"
#include "VBHeap.h"

C3DSprite::C3DSprite()
{
	memset((char*)this+4,0,sizeof(C3DSprite)-4);
}
STDMETHODIMP C3DSprite::QueryInterface(REFIID refiid, PPVOID ppv)
{
	*ppv = NULL;
	return E_NOINTERFACE;
}
STDMETHODIMP_(ULONG) C3DSprite::AddRef(void)
{
	m_dwRefCount++;
	return m_dwRefCount;
}
STDMETHODIMP_(ULONG) C3DSprite::Release(void)
{
	m_dwRefCount--;
	if (!m_dwRefCount)
		delete this;

	return 0;
}



BOOL __stdcall C3DSprite::GetImageHeader(IMAGE_HEADER* pImgHeader,DWORD dwFrameIndex)
{
	BOOL			bResult = FALSE;
	IMAGE_HEADER*	pHeader = m_Texture.GetImageHeader();
	if (!pHeader)
		goto lb_return;

	*pImgHeader = *pHeader;
	bResult = TRUE;

lb_return:
	return bResult;


}
BOOL __stdcall C3DSprite::Resize(float fWidth,float fHeight)
{
	m_v4RectSrc[1].x = m_v4RectSrc[0].x + fWidth;
	m_v4RectSrc[2].y = m_v4RectSrc[0].y + fHeight;
	m_v4RectSrc[3].x = m_v4RectSrc[0].x + fWidth;
	m_v4RectSrc[3].y = m_v4RectSrc[0].y + fHeight;

	memcpy(m_v4RectDest,m_v4RectSrc,sizeof(VECTOR4)*4);
		
	m_v2Center.x = (m_v4RectSrc[1].x - m_v4RectSrc[0].x) / 2.0f;
	m_v2Center.y = (m_v4RectSrc[3].y - m_v4RectSrc[0].y) / 2.0f;


	m_VB.SetVertexPosList(m_v4RectDest,4);
	return TRUE;
}

BOOL C3DSprite::Initialize(CoD3DDevice* pRenderer,char* szFileName,DWORD dwXPos,DWORD dwYPos,DWORD dwWidth,DWORD dwHeight,DWORD dwColor,DWORD dwFlag)
{
	m_pRenderer = pRenderer;
	if (!pRenderer->AllocTexture(&m_Texture,szFileName,0,FALSE,FALSE,TEXTURE_MAP_TYPE_DIFFUSE))
		return FALSE;

	m_bIsLockable	=	FALSE;		// 외부에서 LockRect를 호출 못한다.

	WORD	index[6];
	index[0] = 0;
	index[1] = 1;
	index[2] = 2;

	index[3] = 1;
	index[4] = 3;
	index[5] = 2;

	
//	m_VB.CreateVertexBuffer(pRenderer->GetD3DResourceManager(),sizeof(D3DTLVERTEX),D3DUSAGE_WRITEONLY,D3DFVF_D3DTLVERTEX,D3DPOOL_MANAGED,4);
//	m_IB.CreateIndexBuffer(pRenderer->GetD3DResourceManager(),index,D3DUSAGE_WRITEONLY,D3DPOOL_MANAGED,6);
	DWORD	dwErrorCode;
	if (!m_pRenderer->GetVBSpriteHeap()->AllocVB(&dwErrorCode,&m_VB,4,NULL,&m_VB.m_bEnable,NULL))
		__asm int 3

	
	if (!m_pRenderer->GetIBHeapSprite()->AllocIB(&dwErrorCode,&m_IB,6,NULL,&m_IB.m_bEnable,NULL))
		__asm int 3
	
	m_IB.CheckValid();
	m_IB.SetIndexList(index,6);

	IMAGE_HEADER*	pHeader;
	
	pHeader = m_Texture.GetImageHeader();
	
	float	u1,u2,v1,v2;

	TVERTEX	tv[4];

	if (!dwWidth || !dwHeight)
	{
		u1 = 0.0f;
		u2 = 1.0f;
		v1 = 0.0f;
		v2 = 1.0f;
		
		m_v4RectSrc[0].x = 0.0f;
		m_v4RectSrc[0].y = 0.0f;
		
		m_v4RectSrc[1].x = (float)pHeader->dwWidth;
		m_v4RectSrc[1].y = 0.0f;

		m_v4RectSrc[2].x = 0.0f;
		m_v4RectSrc[2].y = (float)pHeader->dwHeight;

		m_v4RectSrc[3].x = (float)pHeader->dwWidth;
		m_v4RectSrc[3].y = (float)pHeader->dwHeight;

		m_dwWidth = pHeader->dwWidth;
		m_dwHeight = pHeader->dwHeight;;

	}
	else
	{
		u1 = (float)dwXPos / (float)pHeader->dwWidth;
		u2 = (float)(dwXPos+dwWidth) / (float)pHeader->dwWidth;

		v1 = (float)(pHeader->dwHeight - (dwYPos+dwHeight)) / (float)pHeader->dwHeight;
		v2 = (float)(pHeader->dwHeight - dwYPos) / (float)pHeader->dwHeight;
		
		
		m_v4RectSrc[0].x = 0.0f;
		m_v4RectSrc[0].y = 0.0f;
		
		m_v4RectSrc[1].x = (float)dwWidth;
		m_v4RectSrc[1].y = 0.0f;

		m_v4RectSrc[2].x = 0.0f;
		m_v4RectSrc[2].y = (float)dwHeight;

		m_v4RectSrc[3].x = (float)dwWidth;
		m_v4RectSrc[3].y = (float)dwHeight;

		m_dwWidth = dwWidth;
		m_dwHeight = dwHeight;
	}


	for (DWORD i=0; i<4; i++)
	{
		m_v4RectSrc[i].z = 0.1f;
		m_v4RectSrc[i].w = 1.0f;
		// m_v4RectSrc[0].z;
	}


	memcpy(m_v4RectDest,m_v4RectSrc,sizeof(VECTOR4)*4);

/*
	tv[0].u = u1;
	tv[0].v = v2;
	
	tv[1].u = u2;
	tv[1].v = v2;
	
	tv[2].u = u1;
	tv[2].v = v1;
	
	tv[3].u = u2;
	tv[3].v = v1;
	*/

	tv[0].u = u1 + (0.5f / (float)pHeader->dwWidth);
	tv[0].v = v2 - (0.5f / (float)pHeader->dwHeight);
	
	tv[1].u = u2 + (0.5f / (float)pHeader->dwWidth);
	tv[1].v = v2 - (0.5f / (float)pHeader->dwHeight);
	
	tv[2].u = u1 + (0.5f / (float)pHeader->dwWidth);
	tv[2].v = v1 - (0.5f / (float)pHeader->dwHeight);
	
	tv[3].u = u2 + (0.5f / (float)pHeader->dwWidth);
	tv[3].v = v1 - (0.5f / (float)pHeader->dwHeight);

/*
	tv[0].u = u1 + (0.5f / (float)pHeader->dwWidth);
	tv[0].v = v2 - (0.5f / (float)pHeader->dwHeight);
	
	tv[1].u = u2 - (0.5f / (float)pHeader->dwWidth);
	tv[1].v = v2 - (0.5f / (float)pHeader->dwHeight);
	
	tv[2].u = u1 + (0.5f / (float)pHeader->dwWidth);
	tv[2].v = v1 + (0.5f / (float)pHeader->dwHeight);
	
	tv[3].u = u2 - (0.5f / (float)pHeader->dwWidth);
	tv[3].v = v1 + (0.5f / (float)pHeader->dwHeight);
*/
	
	memcpy(m_tvRect,tv,sizeof(TVERTEX)*4);

	m_VB.SetTexCoord(tv,TEX1_COORD_OFFSET_RHW_DIFFUSE,4);
	m_VB.SetTLVERTEXList(m_v4RectDest,dwColor,4);
	
	m_v2Center.x = (m_v4RectSrc[1].x - m_v4RectSrc[0].x) / 2.0f;
	m_v2Center.y = (m_v4RectSrc[3].y - m_v4RectSrc[0].y) / 2.0f;
	
	m_fUlength = u2 - u1;
	m_fVlength = v2 - v1;
	return TRUE;
}

BOOL C3DSprite::InitializeNameless(CoD3DDevice* pRenderer, DWORD dwWidth,DWORD dwHeight, TEXTURE_FORMAT TexFormat, DWORD dwFlag)
{

	D3DFORMAT	d3dTexFormat;
	switch(TexFormat)
	{
	case	TEXTURE_FORMAT_A8R8G8B8:
		d3dTexFormat	=	D3DFMT_A8R8G8B8;
		break;
	case	TEXTURE_FORMAT_A4R4G4B4:
		d3dTexFormat	=	D3DFMT_A4R4G4B4;
		break;
	case	TEXTURE_FORMAT_R5G6B5:
		d3dTexFormat	=	D3DFMT_R5G6B5;
		break;
	case	TEXTURE_FORMAT_A1R5G5B5:
		d3dTexFormat	=	D3DFMT_A1R5G5B5;
		break;
	default:
		OutputDebugString("C3DSprite::InitializeNameless()   Invalid TextureFormat\n");
		_asm int 3;
		return	FALSE;
	}


	m_pRenderer = pRenderer;
	if (!pRenderer->GetTextureManager()->AllocRawTexture(&m_Texture,dwWidth, dwHeight, 1, 0,d3dTexFormat,D3DPOOL_MANAGED))
		return FALSE;

	m_bIsLockable	=	TRUE;		// 외부에서 락을 걸 수 있다.

	LPDIRECT3DTEXTURE8	pTex	=	m_Texture.GetTexture();

	WORD	index[6];
	index[0] = 0;
	index[1] = 1;
	index[2] = 2;

	index[3] = 1;
	index[4] = 3;
	index[5] = 2;

	
	DWORD	dwErrorCode;
	if (!m_pRenderer->GetVBSpriteHeap()->AllocVB(&dwErrorCode,&m_VB,4,NULL,&m_VB.m_bEnable,NULL))
		__asm int 3

	if (!m_pRenderer->GetIBHeapSprite()->AllocIB(&dwErrorCode,&m_IB,6,NULL,&m_IB.m_bEnable,NULL))
		__asm int 3
	
	m_IB.CheckValid();
	m_IB.SetIndexList(index,6);

	IMAGE_HEADER*	pHeader;
	
	pHeader = m_Texture.GetImageHeader();
	
	float	u1,u2,v1,v2;

	TVERTEX	tv[4];


	{
//		u1 = (float)dwXPos / (float)pHeader->dwWidth;
		u1	=	0.0f;
//		u2 = (float)(dwXPos+dwWidth) / (float)pHeader->dwWidth;
		u2	=	1.0f;

//		v1 = (float)(pHeader->dwHeight - (dwYPos+dwHeight)) / (float)pHeader->dwHeight;
//		v2 = (float)(pHeader->dwHeight - dwYPos) / (float)pHeader->dwHeight;
		v1	=	1.0f;
		v2	=	0.0f;
		
		
		m_v4RectSrc[0].x = 0.0f;
		m_v4RectSrc[0].y = 0.0f;
		
		m_v4RectSrc[1].x = (float)dwWidth;
		m_v4RectSrc[1].y = 0.0f;

		m_v4RectSrc[2].x = 0.0f;
		m_v4RectSrc[2].y = (float)dwHeight;

		m_v4RectSrc[3].x = (float)dwWidth;
		m_v4RectSrc[3].y = (float)dwHeight;

		m_dwWidth = dwWidth;
		m_dwHeight = dwHeight;
	}


	for (DWORD i=0; i<4; i++)
	{
		m_v4RectSrc[i].z = 0.1f;
		m_v4RectSrc[i].w = 1.0f;
		// m_v4RectSrc[0].z;
	}


	memcpy(m_v4RectDest,m_v4RectSrc,sizeof(VECTOR4)*4);
/*
	tv[0].u = u1 + (0.5f / (float)pHeader->dwWidth);
	tv[0].v = v2 - (0.5f / (float)pHeader->dwHeight);
	
	tv[1].u = u2 + (0.5f / (float)pHeader->dwWidth);
	tv[1].v = v2 - (0.5f / (float)pHeader->dwHeight);
	
	tv[2].u = u1 + (0.5f / (float)pHeader->dwWidth);
	tv[2].v = v1 - (0.5f / (float)pHeader->dwHeight);
	
	tv[3].u = u2 + (0.5f / (float)pHeader->dwWidth);
	tv[3].v = v1 - (0.5f / (float)pHeader->dwHeight);
*/
	tv[0].u	=	tv[2].u	=	u1;
	tv[1].u	=	tv[3].u	=	u2;
	tv[2].v=	tv[3].v	=	v1;
	tv[0].v	=	tv[1].v	=	v2;



	memcpy(m_tvRect,tv,sizeof(TVERTEX)*4);

	m_VB.SetTexCoord(tv,TEX1_COORD_OFFSET_RHW_DIFFUSE,4);
	m_VB.SetTLVERTEXList(m_v4RectDest,0xffffffff,4);
//	m_VB.SetTLVERTEXList(m_v4RectDest,dwColor,4);
	
	m_v2Center.x = (m_v4RectSrc[1].x - m_v4RectSrc[0].x) / 2.0f;
	m_v2Center.y = (m_v4RectSrc[3].y - m_v4RectSrc[0].y) / 2.0f;
	
	m_fUlength = u2 - u1;
	m_fVlength = v2 - v1;
	return TRUE;
}

BOOL __stdcall C3DSprite::Draw(VECTOR2* pv2Scaling,float fRot,VECTOR2* pv2Trans,RECT* pRect,DWORD dwColor,DWORD dwFlag)
{
	LPDIRECT3DDEVICE8 pDevice = m_pRenderer->GetDevice();

	MATRIX4		matRot;
	MATRIX4		matScale;
	MATRIX4		matTranslate;
	MATRIX4		matResult;
	TRANSFORM_STATUS	trStatus;



	if (pv2Scaling)
	{
		VECTOR3	v2Scale;
		v2Scale.x = pv2Scaling->x;
		v2Scale.y = pv2Scaling->y;
		v2Scale.z = 1.0f;
		SetScaleMatrix(&matScale,&v2Scale);

		trStatus.v2Scale = *pv2Scaling;
	}
	else
	{
		trStatus.v2Scale.x = 1.0f;
		trStatus.v2Scale.y = 1.0f;
		SetIdentityMatrix(&matScale);
	}
	
	if (fRot != 0.0f)
	{	

		trStatus.fRot = fRot;
		
		MATRIX4	mat[2];
		SetIdentityMatrix(&mat[0]);
		SetIdentityMatrix(&mat[1]);
		
		mat[0]._41 = -1.0f * (float)m_v2Center.x * trStatus.v2Scale.x;
		mat[0]._42 = -1.0f * (float)m_v2Center.y * trStatus.v2Scale.y;
		

		mat[1]._41 = -1.0f * mat[0]._41;
		mat[1]._42 = -1.0f * mat[0]._42;

		SetRotationZMatrix(&matRot,fRot);
		MatrixMultiply3(&matRot,&mat[0],&matRot,&mat[1]);
	

	}
	else
	{
		SetIdentityMatrix(&matRot);
		trStatus.fRot = 0.0f;
	}

	if (pv2Trans)
	{
		trStatus.v2Pos = *pv2Trans;
	}
	else
	{
		trStatus.v2Pos.x = 1.0f;
		trStatus.v2Pos.y = 1.0f;
	}
	if (pRect)
	{
		

		if (pRect->left < 0)
			pRect->left = 0;

		if (pRect->top < 0)
			pRect->top = 0;

		if (pRect->right > m_dwWidth)
			pRect->right = m_dwWidth;

		if (pRect->bottom > m_dwHeight)
			pRect->bottom = m_dwHeight;
		
		trStatus.rect = *pRect;
		
	}
	else 
	{
		trStatus.rect.left = 0;
		trStatus.rect.right = 0;
		trStatus.rect.top = 0;
		trStatus.rect.bottom = 0;

	}
	SetIdentityMatrix(&matTranslate);
	matTranslate._41 = trStatus.v2Pos.x;
	matTranslate._42 = trStatus.v2Pos.y;
	

	MatrixMultiply3(&matResult,&matScale,&matRot,&matTranslate);

	TransformVector4_VPTR2(m_v4RectDest,m_v4RectSrc,&matResult,4);

	//trustpak 2005/06/28	
	trStatus.dwColor = dwColor;
	///

	
	if (!memcmp(&trStatus,&m_TrStatus,sizeof(TRANSFORM_STATUS)))
		goto lb_render;

	if (pRect)
	{
		TVERTEX		tv[4];
		tv[0].u = m_tvRect[0].u + m_fUlength * ( (float)pRect->left / (float)m_dwWidth );
		tv[0].v = m_tvRect[0].v - m_fVlength * ( (float)pRect->top / (float)m_dwHeight );

		tv[1].u = m_tvRect[1].u - m_fUlength * ( (float)(m_dwWidth - pRect->right) / (float)m_dwWidth );
		tv[1].v = m_tvRect[1].v - m_fVlength * ( (float)pRect->top / (float)m_dwHeight );

		tv[2].u = m_tvRect[2].u + m_fUlength * ( (float)pRect->left / (float)m_dwWidth );
		tv[2].v = m_tvRect[2].v + m_fVlength * ( (float)(m_dwHeight - pRect->bottom) / (float)m_dwHeight );

		tv[3].u = m_tvRect[3].u - m_fUlength * ( (float)(m_dwWidth - pRect->right) / (float)m_dwWidth );
		tv[3].v = m_tvRect[3].v + m_fVlength * ( (float)(m_dwHeight - pRect->bottom) / (float)m_dwHeight );

		m_VB.SetTexCoord(tv,TEX1_COORD_OFFSET_RHW_DIFFUSE,4);
	}

	m_VB.SetTLVERTEXList(m_v4RectDest,dwColor,4);
//	m_VB.SetVertexPosList(m_v4RectDest,4);
//	m_VB.SetVertexColor(dwColor,DIFFUSE_COORD_OFFSET_XYZW,4);
		
	m_TrStatus = trStatus;
	
	
lb_render:
 
	if (dwFlag & RENDER_TYPE_DISABLE_TEX_FILTERING)
	{
		pDevice->SetTextureStageState(0,D3DTSS_MAGFILTER,D3DTEXF_POINT);
		pDevice->SetTextureStageState(0,D3DTSS_MINFILTER,D3DTEXF_POINT);
	}
                                                  


	pDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_FALSE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE ,FALSE);

	
	pDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX,0);
	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,D3DTADDRESS_WRAP );
	pDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,D3DTADDRESS_WRAP );
	pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_DISABLE);
	pDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );

	pDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE  );

	

	pDevice->SetVertexShader(m_VB.GetVertexShader());
	pDevice->SetStreamSource( 0,m_VB.GetVertexBuffer(),m_VB.GetVertexSize());

	
	
	
	DWORD dwAlpha = (dwColor & 0xff000000)>>24;

	DWORD	dwMtlAlpha = 255;
	if (dwFlag & RENDER_TYPE_SPRITE_MUL)
		dwMtlAlpha |= TEX_FLAG_ALPHA_TRANSP;
	else if (dwFlag & RENDER_TYPE_SPRITE_ADD)
		dwMtlAlpha |= TEX_FLAG_ALPHA_TRANSP_ADD;
	else 
		dwMtlAlpha |= TEX_FLAG_ALPHA_TRANSP_FILTER;

	if (RENDER_TYPE_SPRITE_OPASITY & dwFlag || TEX_FLAG_ALPHA_OPASITY & m_Texture.GetTexFlag())
		dwMtlAlpha |= TEX_FLAG_ALPHA_OPASITY;


	m_Texture.SetTexture(pDevice,0);
	BeginAlphaMeshObject(pDevice,1,dwMtlAlpha,dwAlpha);
	
	pDevice->SetIndices(m_IB.GetIndexBuffer(),m_VB.GetStartIndex());
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,0,4,m_IB.GetStartIndex(),2);


	EndAlphaMeshObject(pDevice);

	pDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE ,TRUE);
	

	pDevice->SetTextureStageState(0,D3DTSS_MAGFILTER,D3DTEXF_LINEAR);
	pDevice->SetTextureStageState(0,D3DTSS_MINFILTER,D3DTEXF_LINEAR);


//	pDevice->SetRenderState(D3DRS_FILLMODE,RENDER_MODE_SOLID);



	return TRUE;
}

C3DSprite::~C3DSprite()
{
//	ReleaseImage();

}
BOOL C3DSprite::LockRect(LOCKED_RECT* pOutLockedRect, RECT* pRect, TEXTURE_FORMAT TexFormat)
{
	LPDIRECT3DTEXTURE8	pTexture	=	m_Texture.GetTexture();
	HRESULT		hResult;

#ifdef	_DEBUG
	if( m_bIsLockable != TRUE)
	{
		OutputDebugString("지금 락을 시도한 스프라이트는 락을 걸 수 없는 타입이다.\n");
		_asm int 3;
		return	FALSE;
	}

	// 범위체크.
	if (pRect)
	{
		if( pRect->right > m_dwWidth || pRect->left < 0 || pRect->bottom > m_dwWidth || pRect->top < 0)
		{
			OutputDebugString("pRect is not valid, out of range\n");
			return	FALSE;
		}
	}

	// 텍스쳐포멧 체크.
	D3DSURFACE_DESC		TexDesc;
	hResult	=	pTexture->GetLevelDesc( 0, &TexDesc);
	if( hResult == D3DERR_INVALIDCALL)
	{
		_asm int 3;
		return	FALSE;
	}
	D3DFORMAT	d3dTexFormat;
	switch(TexFormat)
	{
	case	TEXTURE_FORMAT_A8R8G8B8:
		d3dTexFormat	=	D3DFMT_A8R8G8B8;
		break;
	case	TEXTURE_FORMAT_A4R4G4B4:
		d3dTexFormat	=	D3DFMT_A4R4G4B4;
		break;
	case	TEXTURE_FORMAT_R5G6B5:
		d3dTexFormat	=	D3DFMT_R5G6B5;
		break;
	case	TEXTURE_FORMAT_A1R5G5B5:
		d3dTexFormat	=	D3DFMT_A1R5G5B5;
		break;
	default:
		OutputDebugString("C3DSprite::InitializeNameless()   Invalid TextureFormat\n");
		_asm int 3;
		return	FALSE;
	}
	if( d3dTexFormat != TexDesc.Format)
	{
		_asm int 3;
		OutputDebugString("C3DSprite::LockRect() Invalid Texture format\n");
		return	FALSE;
	}

#endif

	D3DLOCKED_RECT	LockedRect;
	hResult	=	pTexture->LockRect( 0, &LockedRect, pRect, 0);
	if( hResult == D3D_OK)
	{
		pOutLockedRect->pBits	=	LockedRect.pBits;
		pOutLockedRect->Pitch	=	LockedRect.Pitch;
		return	TRUE;
	}

	return	FALSE;
}

BOOL C3DSprite::UnlockRect()
{
	LPDIRECT3DTEXTURE8	pTexture	=	m_Texture.GetTexture();
	
//	D3DSURFACE_DESC	ddsc;
//	pTexture->GetLevelDesc(0,&ddsc);

	
	HRESULT	hResult	=	pTexture->UnlockRect(0);
	if( hResult == D3D_OK)
	{
		return	TRUE;
	}


	return	FALSE;

}