// cGuageBar.cpp: implementation of the cGuageBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cGuageBar.h"
#include "cButton.h"
#include "../Input/Mouse.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cGuageBar::cGuageBar()
{
	m_type = WT_GUAGEBAR;

	m_fVertical = FALSE;
	m_barRelPos = 0.0f;
	m_pbarBtn = NULL;

	m_minValue = 0;
	m_maxValue = 0;
	m_curValue = 0;

	m_fBarDrag = FALSE;	

	m_bLock = FALSE;
}

cGuageBar::~cGuageBar()
{
	SAFE_DELETE(m_pbarBtn);
}
void cGuageBar::InitGuageBar(LONG interval, BOOL vertical)
{
	//ù±!!! :  Add(cWindow * btn)ÇÔ¼öº¸´Ù ¸ÕÀú È£ÃâµÇ¾î¾ß ÇÑ´Ù.!!!
	ASSERT(interval>0);
	m_interval = interval;
	m_fVertical = vertical;
}
void cGuageBar::Add(cWindow * btn)
{
	if(!m_pbarBtn)
	{
		m_pbarBtn = (cButton *)btn;
		m_pbarBtn->SetAbsXY((LONG)m_absPos.x+(LONG)m_pbarBtn->GetRelX(), (LONG)m_absPos.y+(LONG)m_pbarBtn->GetRelY());
		if(m_fVertical)
		{
			m_startPos = (LONG)m_pbarBtn->GetRelY();
		}
		else
		{
			m_startPos = (LONG)m_pbarBtn->GetRelX();
		}
		m_interval -= (m_fVertical?m_pbarBtn->GetHeight():m_pbarBtn->GetWidth());
	}
	else
	{
		ASSERT(0);
	}
}
DWORD cGuageBar::ActionEvent(CMouse * mouseInfo)
{
	if(!m_bActive) return WE_NULL;
	DWORD we = WE_NULL;

	//SW050823 SetGuageLock()
	if(m_bLock) return WE_NULL;

//KES if¹®À» ¹Ù²Ù¾î¾ß... Å¬¸¯µÇ¾úÀ»¶§ µå·¡±×½ÃÀÛ.. Å¬¸¯À» ³õ¾ÒÀ»¶§ µå·¡±× ³¡..

	we |= m_pbarBtn->ActionEvent( mouseInfo );

	if( we & WE_LBTNCLICK )
	{
		m_fBarDrag = TRUE;
		m_fDragRelX = mouseInfo->GetMouseEventX() - m_pbarBtn->GetAbsX();
		m_fDragRelY = mouseInfo->GetMouseEventY() - m_pbarBtn->GetAbsY();
	}
	
	if( mouseInfo->LButtonUp() )	//°ÔÀÌÁöÀ§¿¡¼­ ¹öÆ°¾÷À» ÇÏÁö ¾Ê¾Æµµ µÇ¾ßÇÏ¹Ç·Î
		m_fBarDrag = FALSE;

	if(m_fBarDrag)
	{
		int x = mouseInfo->GetMouseX();
		int y = mouseInfo->GetMouseY();

		if(m_fVertical)
		{	
			m_barRelPos = (float)(y - m_absPos.y - m_startPos - m_fDragRelY );
			if(m_barRelPos < 0.0f)
				m_barRelPos = 0.0f;
			else if(m_barRelPos > (float)m_interval)
				m_barRelPos = (float)m_interval;
		}
		else
		{
			m_barRelPos = (float)(x - m_absPos.x - m_startPos - m_fDragRelX);
			if(m_barRelPos < 0.0f)
				m_barRelPos = 0.0f;
			else if(m_barRelPos > (float)m_interval)
				m_barRelPos = (float)m_interval;
		}
		m_curValue = (LONG)(m_minValue + (m_barRelPos/(float)m_interval*(m_maxValue-m_minValue)));
	}

	if(m_fVertical)
		m_pbarBtn->SetAbsY((LONG)(m_absPos.y+m_startPos+(LONG)m_barRelPos));
	else
		m_pbarBtn->SetAbsX((LONG)(m_absPos.x+m_startPos+(LONG)m_barRelPos));	
		

	return we;
}
void cGuageBar::Render()
{
	cWindow::Render();
	if(m_bActive)
	if(m_pbarBtn)
	m_pbarBtn->Render();
}

void cGuageBar::SetAbsXY(LONG x, LONG y)
{
	cWindow::SetAbsXY(x,y);

	if(m_pbarBtn)
	m_pbarBtn->SetAbsXY((LONG)m_absPos.x+(LONG)m_pbarBtn->GetRelX(), (LONG)m_absPos.y+(LONG)m_pbarBtn->GetRelY());
}

void cGuageBar::SetAlpha(BYTE al)
{
	cWindow::SetAlpha(al);
	if(m_pbarBtn)
	m_pbarBtn->SetAlpha(al);
}




//---------------- value calculation function ------------------------------------------------------------------
void cGuageBar::InitValue(LONG minv, LONG maxv, LONG curv)
{
	m_minValue = minv;
	m_maxValue = maxv;
	m_curValue = curv;
	repositioning();
}
void cGuageBar::repositioning()
{
	ASSERT(m_maxValue-m_minValue>0);
//	m_barRelPos = m_interval*(float)m_curValue/(float)(m_maxValue-m_minValue);
//KES 030825
	m_barRelPos = (float)m_interval*(float)(m_curValue-m_minValue)/(float)(m_maxValue-m_minValue);
//	m_barRelPos = m_interval*(float)(m_curValue-m_minValue)*(float)(m_maxValue-m_minValue);
}
LONG cGuageBar::GetCurValue()
{
	return m_curValue;
}

void cGuageBar::SetMinValue(LONG minv)
{
	m_minValue = minv;
	repositioning();
}
void cGuageBar::SetMaxValue(LONG maxv)
{
	m_maxValue = maxv;
	repositioning();
}

void cGuageBar::SetCurValue(LONG val)
{
	m_curValue = val;
	repositioning();
}

void cGuageBar::SetGuageLock(BOOL bDo, DWORD BtnColor)
{
	m_bLock = bDo;
	m_pbarBtn->SetImageRGB(BtnColor);
}
