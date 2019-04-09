// cImage.cpp: implementation of the cImage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cImage.h"
#include "cResourceManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// ------------------------------------------------------------------------------------------
// cImageBase class member
// ------------------------------------------------------------------------------------------
cImageBase::cImageBase()
{
	m_pSurface = NULL;
}
cImageBase::~cImageBase()
{
	//SAFE_RELEASE(m_pSurface);// ¸®¼Ò½º ¸Å´ÏÀú ¿¡¼­ 
}
/*
void cImageBase::LoadSprite(char * szFilename)
{
	//RESRCMGR->GetImageInfo(szFilename, 0, 0);
}
*/

// ------------------------------------------------------------------------------------------
// cImage class member
// ------------------------------------------------------------------------------------------
cImage::cImage()
{
}
cImage::~cImage()
{
}

void cImage::RenderSprite(VECTOR2* pv2Scaling,VECTOR2* pv2RotCenter,float fRot,
						  VECTOR2* pv2Trans, DWORD dwColor)
{
	VECTOR2 sc;
	if(pv2Scaling)
	{
		sc.x = (float)(m_srcImageRect.right-m_srcImageRect.left)*pv2Scaling->x/m_srcImageSize.x;
		sc.y = (float)(m_srcImageRect.bottom-m_srcImageRect.top)*pv2Scaling->y/m_srcImageSize.y;
	}
	else
	{
		sc.x = (float)(m_srcImageRect.right-m_srcImageRect.left)/m_srcImageSize.x;
		sc.y = (float)(m_srcImageRect.bottom-m_srcImageRect.top)/m_srcImageSize.y;
	}
	
	cImageBase::RenderSprite( &sc, pv2RotCenter, fRot, pv2Trans, &m_srcImageRect, dwColor );
}

/* ÀÌÀü CImage ÄÚµå À±È£¾¾ ÂüÁ¶

  
cImage::cImage()
{
	m_pSurface=NULL;
//	m_x=0;
//	m_y=0;
//	m_width=0;
//	m_height=0;

	m_pRenderer = g_pExecutive->GetRenderer();
	m_pGeometry = g_pExecutive->GetGeometry();
}

cImage::~cImage()
{
	if(g_pExecutive)
	{
		SAFE_RELEASE(m_pSurface);
	}
}

BOOL cImage::LoadSprite(char * szFilename)
{
	SAFE_RELEASE(m_pSurface);
	m_pSurface = m_pRenderer->CreateSpriteObject(szFilename,0);
	if(!m_pSurface) 
	{
		char buffer[255];
		GetCurrentDirectory(255, buffer);
	
		LOGEX(szFilename,PT_MESSAGEBOX);
		return FALSE;
	}

	return TRUE;
	
//	SetWH()
}
void cImage::RenderSprite(VECTOR2* pv2Scaling,VECTOR2* pv2RotCenter,float fRot,VECTOR2* pv2Trans,DWORD dwColor,int iZOrder,DWORD dwFlag)
{
	if(m_pSurface)
	{
		BOOL rt = m_pGeometry->RenderSprite(m_pSurface,pv2Scaling,fRot,pv2Trans,NULL,dwColor,iZOrder,dwFlag);
		
	}
}


void cImage::RenderSpriteEx(VECTOR2* pv2Scaling,VECTOR2* pv2RotCenter,float fRot,VECTOR2* pv2Trans,DWORD dwColor,int iZOrder,DWORD dwFlag)
{
	if(m_pSurface)
	{
		BOOL rt = m_pGeometry->RenderSprite(m_pSurface,pv2Scaling,fRot,pv2Trans,&m_ImgRect,dwColor,iZOrder,dwFlag);
	}
}
void cImage::RenderSpritePart( VECTOR2* pv2Scaling, VECTOR2* pv2RotCenter, float fRot, VECTOR2* pv2Trans, RECT * imgSrcRect, DWORD dwColor )
{
	if(m_pSurface)
	{
		if(imgSrcRect->left != imgSrcRect->right)
			m_pGeometry->RenderSprite(m_pSurface,pv2Scaling,fRot,pv2Trans, imgSrcRect, dwColor,0,RENDER_TYPE_DISABLE_TEX_FILTERING);
		else
			m_pGeometry->RenderSprite(m_pSurface,pv2Scaling,fRot,pv2Trans, NULL, dwColor,0,RENDER_TYPE_DISABLE_TEX_FILTERING);
	}
}

*/