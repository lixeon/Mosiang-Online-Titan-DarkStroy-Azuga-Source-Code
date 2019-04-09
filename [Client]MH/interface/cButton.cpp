// cButton.cpp: implementation of the cButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cButton.h"
#include "..\Audio\MHAudioManager.h"
#include "../Input/Mouse.h"
#include "cFont.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cButton::cButton()
{
	*m_szText=0;
	m_fgBasicColor=RGB_HALF(255,255,255);
	m_fgOverColor=RGB_HALF(255,255,255);
	m_fgPressColor=RGB_HALF(255,255,255);
	m_fgCurColor=RGB_HALF(255,255,255);
	m_nCurTextLen = 0;
	m_type = WT_BUTTON;
	m_textXY.x = 5;
	m_textXY.y = 4;
	
	m_bClickInside	= FALSE;
	m_BtnTextAniX	= 1;
	m_BtnTextAniY	= 1;
	
	m_bSound = FALSE;

	m_shadowTextXY.x = 1;
	m_shadowTextXY.y = 0;
	m_shadowColor = RGB_HALF(10,10,10);
	m_bShadow = FALSE;
}

cButton::~cButton()
{
	// 리소스 릴리즈는 리소스 메니저에서
	//SAFE_DELETE(m_pPressImage);
	//SAFE_DELETE(m_pOverImage);
}

void cButton::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, cImage * overImage, cImage * pressImage, cbFUNC Func, LONG ID)
{
	cWindow::Init(x,y,wid,hei,basicImage,ID);
	m_type = WT_BUTTON;

	if( overImage )		m_OverImage = *overImage;
	if( pressImage )	m_PressImage = *pressImage;

	if(Func != NULL)
		cbWindowFunc = Func;
}

DWORD cButton::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we |= cWindow::ActionEvent(mouseInfo);
	
	if( m_bDisable )
	{
		m_bClickInside	= FALSE;
		m_pCurImage		= &m_BasicImage;
		m_fgCurColor	= m_fgBasicColor;
		return we;
	}
	

	LONG x			= mouseInfo->GetMouseX();
	LONG y			= mouseInfo->GetMouseY();

	BOOL LeftUp		= mouseInfo->LButtonUp();
	BOOL LeftPress	= mouseInfo->LButtonPressed();

	if( we & ( WE_LBTNCLICK | WE_LBTNDBLCLICK ) )
	{
		m_bClickInside	= TRUE;
	}

	if( LeftUp )
	{
		if( m_bClickInside && PtInWindow( mouseInfo->GetMouseEventX(), mouseInfo->GetMouseEventY() ) )
		{
//			if( m_bSound )
//			{
//				AUDIOMGR->Play( m_sndIdx, HERO);
//			}

			(*cbWindowFunc)(m_ID, m_pParent, WE_BTNCLICK);
			we |= WE_BTNCLICK;
		}

		m_bClickInside = FALSE;
	}

//	if( PtInWindow(x, y) )
	if( we & WE_MOUSEOVER )
	{
		if( LeftPress && m_bClickInside )
		{
			m_pCurImage		= &m_PressImage;
			m_fgCurColor	= m_fgPressColor;
		}
		else
		{
			m_pCurImage		= &m_OverImage;
			m_fgCurColor	= m_fgOverColor;
		}
	}
	else
	{
		if( LeftPress && m_bClickInside ) //안에서 클릭하여 밖으로 나간경우
		{
			m_pCurImage		= &m_OverImage;
			m_fgCurColor	= m_fgOverColor;
		}
		else
		{
			m_pCurImage		= &m_BasicImage;
			m_fgCurColor	= m_fgBasicColor;
		}
	}

	return we;
}

void cButton::Render()
{
	if( m_bActive == FALSE ) return;
	
	cWindow::Render();

	if( m_nCurTextLen != 0 )
	{
		LONG TxtPosX;

		if( m_nAlign & TXT_LEFT )
		{
			TxtPosX = (LONG)m_absPos.x + m_textXY.x;
		}
		else if( m_nAlign & TXT_RIGHT )
		{
			TxtPosX = (LONG)m_absPos.x + m_width - m_textXY.x 
				      - CFONT_OBJ->GetTextExtentEx( m_wFontIdx, m_szText, strlen(m_szText) );
		}
		else //TXT_CENTER
		{
			TxtPosX = (LONG)m_absPos.x + ( m_width
				      - CFONT_OBJ->GetTextExtentEx( m_wFontIdx, m_szText, strlen(m_szText) ) ) / 2;
		}

		RECT rect={ TxtPosX, (LONG)m_absPos.y+m_textXY.y, 1, 1 };

		if( m_pCurImage == (&m_PressImage) )
		{
			OffsetRect( &rect, m_BtnTextAniX, m_BtnTextAniY );
		}

		if( m_bShadow )
		{
			RECT rcShadow = rect;
			OffsetRect( &rcShadow, m_shadowTextXY.x, m_shadowTextXY.y );
			CFONT_OBJ->RenderFont(m_wFontIdx, m_szText, m_nCurTextLen,&rcShadow,RGBA_MERGE(m_shadowColor, m_alpha * m_dwOptionAlpha / 100));
		}

		CFONT_OBJ->RenderFont(m_wFontIdx, m_szText,m_nCurTextLen,&rect,RGBA_MERGE(m_fgCurColor, m_alpha * m_dwOptionAlpha / 100));

	}
}

void cButton::SetText(char * text, DWORD basicColor, DWORD overColor, DWORD pressColor)
{
	ASSERT(MAX_TEXT_SIZE>strlen(text));
	m_nCurTextLen = strlen(text);
	strncpy(m_szText, text, m_nCurTextLen + 1);
	m_fgCurColor = m_fgBasicColor = m_fgOverColor = m_fgPressColor = basicColor;
	if(overColor)
		m_fgOverColor = overColor;
	if(pressColor)
		m_fgPressColor = pressColor;
}

void cButton::SetTextValue(DWORD value)
{
	wsprintf(m_szText, "%d", value);
	m_nCurTextLen = strlen(m_szText);
}
