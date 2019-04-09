// cGuagen.cpp: implementation of the cGuagen class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cGuagen.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cGuagen::cGuagen()
{
	m_type = WT_GUAGEN;
	m_imgRelPos.x		= 0;
	m_imgRelPos.y		= 0;
	m_fGuageWidth		= 0.f;
	m_fGuagePieceWidth	= 0.f;
	m_fPercentRate		= 0.f;
	m_fGuagePieceHeightScaleY = 1.f;
}

cGuagen::~cGuagen()
{
	
}

void cGuagen::Render()
{	
	cWindow::Render();

	if(m_bActive)
	{
		VECTOR2 imgPosRect = { m_absPos.x + m_imgRelPos.x, m_absPos.y + m_imgRelPos.y };
		VECTOR2 scaleRate;
		scaleRate.x = m_fGuageWidth * m_fPercentRate / m_fGuagePieceWidth;
		scaleRate.y = m_fGuagePieceHeightScaleY;
		m_GuagePieceImage.RenderSprite(&scaleRate, NULL, 0, &imgPosRect,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
	}
}
