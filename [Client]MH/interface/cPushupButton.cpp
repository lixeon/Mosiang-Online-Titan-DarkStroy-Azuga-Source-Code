// cPushupButton.cpp: implementation of the cPushupButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cPushupButton.h"
#include "../Input/Mouse.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cPushupButton::cPushupButton()
{
	m_fPushed=FALSE;
	m_fPassive=FALSE;
	m_type = WT_PUSHUPBUTTON;
}

cPushupButton::~cPushupButton()
{
	
}

void cPushupButton::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, cImage * overImage, cImage * pressImage, void (*Func)(LONG lId, void * p, DWORD we), LONG ID)
{
	cButton::Init(x,y,wid,hei,basicImage,overImage,pressImage,NULL,ID);
	m_type = WT_PUSHUPBUTTON;
	if(Func != NULL)
		cbWindowFunc = Func;
}

DWORD cPushupButton::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we = cWindow::ActionEvent(mouseInfo);

	if( m_bDisable ) return we;

	long x = mouseInfo->GetMouseX();
	long y = mouseInfo->GetMouseY();
//KES SMI 031008
	long x2 = mouseInfo->GetMouseEventX();
	long y2 = mouseInfo->GetMouseEventY();	

	if(m_fPassive)
	{
		if( we & ( WE_LBTNCLICK | WE_LBTNDBLCLICK ) )
		{
			if(PtInWindow(x2, y2))
			{
				(m_fPushed?we |= WE_PUSHUP:we |= WE_PUSHDOWN);
				cbWindowFunc(m_ID, m_pParent, (m_fPushed?WE_PUSHUP:WE_PUSHDOWN));
			}
		}		
	}
	else
	{
		if( we & ( WE_LBTNCLICK | WE_LBTNDBLCLICK ) )
		{
			if(PtInWindow(x2, y2))
			{
				m_fPushed ^= TRUE;
				SetPush(m_fPushed);
				if(cbWindowFunc)
				{
					(m_fPushed?we |= WE_PUSHDOWN:we |= WE_PUSHUP);
					(*cbWindowFunc)(m_ID, m_pParent, (m_fPushed?WE_PUSHDOWN:WE_PUSHUP));
				}
			}
		}
	}

	return we;
}

void cPushupButton::SetPush(BOOL val)
{
	m_fPushed = val;
	if(m_fPushed)
	{
		m_pCurImage = &m_PressImage;
		m_fgCurColor = m_fgPressColor;
	}
	else
	{
		m_pCurImage = &m_BasicImage;
		m_fgCurColor = m_fgBasicColor;
	}
}

void cPushupButton::SetPushEx(BOOL val)
{
	SetPush(val);
	
	if(cbWindowFunc)
		(*cbWindowFunc)(m_ID, m_pParent, (m_fPushed?WE_PUSHDOWN:WE_PUSHUP));
}
