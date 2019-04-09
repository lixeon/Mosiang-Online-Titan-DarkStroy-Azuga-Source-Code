// cImageSelf.cpp: implementation of the cImageSelf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cImageSelf.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cImageSelf::cImageSelf()
{
	m_pSurface = NULL;
}

cImageSelf::~cImageSelf()
{
	Release();
}

void cImageSelf::Release()
{
	if(g_pExecutive)
	{
		SAFE_RELEASE(m_pSurface);
	}
}

BOOL cImageSelf::LoadSprite(char * szFilename)
{
	m_pSurface = g_pExecutive->GetRenderer()->CreateSpriteObject(szFilename,0);
	if(!m_pSurface) 
	{
		return FALSE;
	}
	
	IMAGE_HEADER header;
	m_pSurface->GetImageHeader(&header,0);
	m_srcImageSize.x = (float)header.dwWidth;
	m_srcImageSize.y = (float)header.dwHeight;
	m_srcImageRect.left = 0;
	m_srcImageRect.top = 0;
	m_srcImageRect.right = header.dwWidth;
	m_srcImageRect.bottom = header.dwHeight;

	return TRUE;
}

void cImageSelf::SetImageSrcRect(const cImageRect * rt)
{
	memcpy( &m_srcImageRect, rt, sizeof(cImageRect) );
	if(m_srcImageRect.left < 0)	m_srcImageRect.left = 0;
	if(m_srcImageRect.top < 0)	m_srcImageRect.top = 0;
	if(m_srcImageRect.right > (LONG)m_srcImageSize.x)		m_srcImageRect.right = (LONG)m_srcImageSize.x;
	if(m_srcImageRect.bottom > (LONG)m_srcImageSize.y)	m_srcImageRect.bottom = (LONG)m_srcImageSize.y;
}
	
void cImageSelf::RenderSprite( VECTOR2* pv2Scaling, VECTOR2* pv2RotCenter, float fRot,
								VECTOR2* pv2Trans, DWORD dwColor )
{
	if(m_pSurface)
	{
		g_pExecutive->GetGeometry()->RenderSprite(m_pSurface,pv2Scaling,fRot,pv2Trans,&m_srcImageRect,dwColor,0,RENDER_TYPE_DISABLE_TEX_FILTERING);
	}
}

BOOL cImageSelf::GetImageOriginalSize(VECTOR2* pOutSize)
{
	if(!m_pSurface)
		return FALSE;

	IMAGE_HEADER header;
	m_pSurface->GetImageHeader(&header,0);
	pOutSize->x = (float)header.dwWidth;
	pOutSize->y = (float)header.dwHeight;
	
	return TRUE;
}