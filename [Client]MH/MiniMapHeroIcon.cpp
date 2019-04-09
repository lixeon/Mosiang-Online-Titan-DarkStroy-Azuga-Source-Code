// MiniMapHeroIcon.cpp: implementation of the CMiniMapHeroIcon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MiniMapHeroIcon.h"
#include "Object.h"
#include "./Interface/cImage.h"
#include "MHCamera.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMiniMapHeroIcon::CMiniMapHeroIcon()
{
	m_pIconImage = NULL;

	m_pCameraIconImage = NULL;
	m_fCameraImageAngle = 0.0f;
}

CMiniMapHeroIcon::~CMiniMapHeroIcon()
{

}

void CMiniMapHeroIcon::Update(DWORD MinimapImageWidth,DWORD MinimapImageHeight)
{
	m_HeroImageAngle = gPHI-m_pObject->GetAngle();

	m_fCameraImageAngle = -CAMERA->GetCameraDesc()->m_AngleY.ToRad();

	CMiniMapIcon::Update(MinimapImageWidth,MinimapImageHeight);
}

void CMiniMapHeroIcon::Render(VECTOR2* pMapStartPixel)
{
	VECTOR2 trans;

	if(m_pCameraIconImage)
	{
		trans.x = m_ScreenPos.x + pMapStartPixel->x - m_pCameraIconImage->m_ImageWH.x*0.5f;
		trans.y = m_ScreenPos.y + pMapStartPixel->y - m_pCameraIconImage->m_ImageWH.y*0.5f;

		m_pCameraIconImage->m_pImage->RenderSprite( NULL, NULL, m_fCameraImageAngle, &trans, 0xeeffffff );
	}

	if(m_pIconImage)
	{
		trans.x = m_ScreenPos.x + pMapStartPixel->x - m_pIconImage->m_ImageWH.x*0.5f;
		trans.y = m_ScreenPos.y + pMapStartPixel->y - m_pIconImage->m_ImageWH.y*0.5f;

		m_pIconImage->m_pImage->RenderSprite(NULL,NULL,m_HeroImageAngle,&trans,m_dwColor);
	}
}
