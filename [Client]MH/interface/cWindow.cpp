// cWindow.cpp: implementation of the cWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cWindow.h"
#include "cFont.h"
#include "cWindowManager.h"
#include "../Input/Mouse.h"
#include "MouseCursor.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void defaultWindowFunc(LONG lId, void * p, DWORD we){}

cWindow::cWindow()
{
	m_ID = 0;
	m_wFontIdx=0;
	m_type=WT_WINDOW;
	m_bActive=TRUE;
	m_bDisable = FALSE;
	m_bFocus=TRUE;
	m_bMovable=FALSE;
	m_width=0;
	m_height=0;
	m_pCurImage=NULL;
	m_pToolTipImage=NULL;
	m_bDepend=FALSE;
	m_pParent=NULL;
	m_dwViewCurTime=m_dwViewLastTime=0;
//	memset(m_strToolTipMsg, 0, MAX_TOOLTIPSTR_SIZE);
	m_dwImageRGB = 0xffffff;
	m_alpha = 255;
	m_dwOptionAlpha = 100;
	m_validPos.x = 0;
	m_validPos.y = 0;
	cbWindowFunc=defaultWindowFunc;
	m_scale.x = 1.f;
	m_scale.y = 1.f;
	m_bAlwaysTop = FALSE;	
	m_bDestroy = FALSE;
	m_bSetTopOnActive = TRUE;
}

cWindow::~cWindow()
{
	toolTip.Release();
	//리소스 릴리즈는 메니저에서
	//SAFE_DELETE(m_pBasicImage);
	//SAFE_DELETE(m_pCurImage);
//	if(GetID() == 68)
//		__asm int 3;
}


void cWindow::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * pBasicImage, LONG ID)
{
	SetAbsXY(x,y);
	SetRelXY(x,y);
	SetWH(wid,hei);
	m_ID=ID;
	if(pBasicImage)
	{
		m_BasicImage = *pBasicImage;
		m_pCurImage = &m_BasicImage;
	}
}

BOOL cWindow::PtInWindow(LONG x, LONG y)
{
	if(m_absPos.x+m_validPos.x > x || m_absPos.y+m_validPos.y > y || m_absPos.x+m_validPos.x+m_width < x || m_absPos.y+m_validPos.y+m_height < y)
		return FALSE;
	else
		return TRUE;
}


DWORD cWindow::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive || m_bDestroy ) return we;

	if( !WINDOWMGR->IsMouseOverUsed() && PtInWindow(mouseInfo->GetMouseX(), mouseInfo->GetMouseY() ) )
	{
		//내가 드래그중인 윈도우면 over가 아니다. //다른윈도우에 오버로 무언가 처리해야 할때가 있다.(드래그엔드랍)
		if( !(WINDOWMGR->IsDragWindow() && (cWindow*)WINDOWMGR->GetDragDlg() == this ) )
		{
			we |= WE_MOUSEOVER;
			
			//툴팁이 있는지 먼저 검사...최적화때
			
			m_dwViewCurTime = GetTickCount();
			if(m_dwViewCurTime - m_dwViewLastTime > TOOLTIP_DELAY_TIME)
			{
				m_ttX = mouseInfo->GetMouseX();
				m_ttY = mouseInfo->GetMouseY();
			}
		}
		
		if( !WINDOWMGR->IsDragWindow() )
		{
			// magi82 - Titan(070511) 타이탄 장착아이템 수리할때
			 if( CURSOR->GetCursor() != eCURSOR_TITANREPAIR )
			 {
				 if( m_type & WT_ICON )
					 CURSOR->SetCursor( eCURSOR_ITEMSELECT );
				 else
					 CURSOR->SetCursor( eCURSOR_DEFAULT );
			 }
		}
	}
	else
	{
		m_dwViewLastTime = m_dwViewCurTime = 0;
	}


	if( !WINDOWMGR->IsMouseDownUsed() )
	if( PtInWindow(mouseInfo->GetMouseEventX(), mouseInfo->GetMouseEventY() ) )
	{
		if( mouseInfo->LButtonDown() )
			we |= WE_LBTNCLICK;
		if( mouseInfo->RButtonDown() )
			we |= WE_RBTNCLICK;
		if( mouseInfo->LButtonDoubleClick() )
			we |= WE_LBTNDBLCLICK;
		if( mouseInfo->RButtonDoubleClick() )
			we |= WE_RBTNDBLCLICK;
	}

	return we;
}

void cWindow::ExcuteCBFunc( DWORD we )
{
	if( cbWindowFunc )
		(*cbWindowFunc)(m_ID, m_pParent, we);
}

void cWindow::Render()
{
	if( m_bActive )
	if( m_pCurImage )
	{
		DWORD alpha = m_alpha * m_dwOptionAlpha / 100;
		DWORD dwRGBA = RGBA_MERGE( m_dwImageRGB, alpha );
		
		m_pCurImage->RenderSprite( &m_scale, NULL, 0, &m_absPos, dwRGBA );

		ToolTipRender();
	}
}

void cWindow::ToolTipRender()
{
	if(toolTip.IsValid() && m_dwViewCurTime - m_dwViewLastTime > TOOLTIP_DELAY_TIME)
	{
		//여기서 윈도우 메니져에게 보낼까?
		WINDOWMGR->SetToolTipWindow( &toolTip );

		toolTip.SetXY(m_ttX+TOOLTIP_MOUSESIZE_X, m_ttY+TOOLTIP_MOUSESIZE_Y);
	}
}

void cWindow::SetToolTip( char* msg, DWORD color, cImage * image, DWORD imgColor )
{ 
	if( msg )
	{
		toolTip.Init( 0, color, image, imgColor );	//폰트 아이디는? 무조건 0인가
		toolTip.SetImageAlpha( TOOLTIP_ALPHA );	//200대충.. 조정하기.
		toolTip = msg;
	}
}


void cWindow::AddToolTipLine( char* msg, DWORD color )
{
	toolTip.AddLine( msg, color );
}
