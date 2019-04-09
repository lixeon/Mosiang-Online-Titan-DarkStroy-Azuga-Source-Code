// cGuage.cpp: implementation of the cGuage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cGuage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// coded by taiyo
// 2003.10.29.
//!!!!! This cGugae class is deprecated !!!!!
// you had better use cGuagen OR CObjectGuagen class


cGuage::cGuage()
{
	m_type = WT_GUAGE;
	m_imgAbsPos.x = 0;
	m_imgAbsPos.y = 0;
	m_imgRelPos.x = 0;
	m_imgRelPos.y = 0;

	m_curValue=0;
	m_minValue=0;
	m_maxValue=0;
	m_fillPersent = 0;
	
	m_fChanged = FALSE;
}

cGuage::~cGuage()
{
	
}

DWORD cGuage::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if(!m_bActive) return we;

	if(m_fChanged)
	{
#ifndef _TESTCLIENT_
		ASSERT(m_maxValue > m_minValue);
#endif
		if(m_maxValue > m_minValue)
		m_fillPersent = (float)(m_curValue - m_minValue)/(m_maxValue - m_minValue);
	
		m_fChanged = FALSE;
	}

	return we;
}
void cGuage::Render()
{	
	cWindow::Render();

	if(m_bActive)
	{
		if(!m_GuagePieceImage.IsNull())
		{
			VECTOR2 scaleRate;
			scaleRate.x = m_fillPersent;
			scaleRate.y = 1;
			m_GuagePieceImage.RenderSprite(&scaleRate,NULL,0,&m_imgAbsPos,RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100));
		}
	}
}

void cGuage::InitGuage(LONG x, LONG y)
{
	m_imgRelPos.x = x;
	m_imgRelPos.y = y;
}

void cGuage::SetAbsXY(LONG x, LONG y)
{
	cWindow::SetAbsXY(x,y);

	m_imgAbsPos.x = m_absPos.x+m_imgRelPos.x;
	m_imgAbsPos.y = m_absPos.y+m_imgRelPos.y;
}