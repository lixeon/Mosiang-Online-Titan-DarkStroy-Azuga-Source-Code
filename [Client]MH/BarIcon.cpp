// BarIcon.cpp: implementation of the CBarIcon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BarIcon.h"
#include "GameIn.h"

#include "MainBarDialog.h"
#include "./Input/Mouse.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"

#include "ObjectManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBarIcon::CBarIcon()
{
	m_bCaptionHit = FALSE;
}

CBarIcon::~CBarIcon()
{

}

void CBarIcon::InitInfo(BARICON_INFO * info)
{
	memcpy(&m_Info, info, sizeof(BARICON_INFO));
	m_bAlram = FALSE;

	SCRIPTMGR->GetImage( 92, &m_ImgAlram[0] );
	SCRIPTMGR->GetImage( 93, &m_ImgAlram[1] );
	SCRIPTMGR->GetImage( 94, &m_ImgAlram[2] );

	SCRIPTMGR->GetImage( 108, &m_ImgSpecialAlram[0] );
	SCRIPTMGR->GetImage( 109, &m_ImgSpecialAlram[1] );
	SCRIPTMGR->GetImage( 110, &m_ImgSpecialAlram[2] );
	
	
}
/*
void CBarIcon::SetAlramImage( cImage* pImg )
{
	m_ImgAlram = *pImg;
}
*/
DWORD CBarIcon::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;

	we = cWindow::ActionEvent(mouseInfo);

	long x = mouseInfo->GetMouseX();
	long y = mouseInfo->GetMouseY();
	long x2 = mouseInfo->GetMouseEventX();
	long y2 = mouseInfo->GetMouseEventY();

	if( ( mouseInfo->LButtonDown() || mouseInfo->LButtonDoubleClick() ) && !WINDOWMGR->IsMouseDownUsed() )
	{
		if( PtInWindow( x2, y2 ) )
		{
			if( m_bMovable ) m_bCaptionHit = TRUE;
			we |= WE_TOPWINDOW;
		}
	}

	if( mouseInfo->LButtonUp() )
	{
		if( m_bCaptionHit )
		if( GAMEIN->GetMainInterfaceDialog()->m_OldDragFlag == FALSE )
		if( PtInWindow( x2, y2 ) )
		if( !WINDOWMGR->IsMouseInputProcessed() )
		{
			m_fPushed ^= TRUE;
			GAMEIN->GetMainInterfaceDialog()->PushMenu( GetInfo()->kind );
		}

		m_bCaptionHit = FALSE;
	}
/*
	if( m_bCaptionHit )
	if( mouseInfo->LButtonDrag() )
	if( GAMEIN->GetMainInterfaceDialog()->m_OldDragFlag == FALSE )
	{
		SetAlpha(150);
		LONG OldX = (LONG)(x>m_absPos.x?x-m_absPos.x:m_absPos.x-x);
		LONG OldY = (LONG)(y>m_absPos.y?y-m_absPos.y:m_absPos.y-y);
		GAMEIN->GetMainInterfaceDialog()->SetDragStart((LONG)m_absPos.x, (LONG)m_absPos.y, OldX, OldY, this);
	}
*/
	if( !m_fPushed )
	{
		if( we & WE_MOUSEOVER )
		{
			m_pCurImage = &m_OverImage;
			m_fgCurColor = m_fgOverColor;		
		}
		else
		{
			m_pCurImage = &m_BasicImage;
			m_fgCurColor = m_fgBasicColor;
		}
	}
	else
	{
		m_pCurImage = &m_PressImage;
		m_fgCurColor = m_fgPressColor;		
	}


/*
	if( we & WE_MOUSEOVER )
	{
		if((mouseInfo->GetMouseAxisX() || mouseInfo->GetMouseAxisY()) && mouseInfo->LButtonDrag())
		{
			if(GAMEIN->GetMainInterfaceDialog()->m_OldDragFlag == FALSE)
			{
				SetAlpha(150);
				LONG OldX = (LONG)(x>m_absPos.x?x-m_absPos.x:m_absPos.x-x);
				LONG OldY = (LONG)(y>m_absPos.y?y-m_absPos.y:m_absPos.y-y);
				GAMEIN->GetMainInterfaceDialog()->SetDragStart((LONG)m_absPos.x, (LONG)m_absPos.y, OldX, OldY, this);
			}
		}
		else
		{
			if( GAMEIN->GetMainInterfaceDialog()->m_OldDragFlag == FALSE && mouseInfo->LButtonUp() )
			{
				m_fPushed ^= TRUE;
				GAMEIN->GetMainInterfaceDialog()->PushMenu( GetInfo()->kind );
			}
		}

		if( mouseInfo->LButtonPressed() )
		{
			m_pCurImage = &m_PressImage;
			m_fgCurColor = m_fgPressColor;
		}
		else if( !m_fPushed ) //KES pushed상태에선 오버이미지 없는것이 보기 좋아서.
		{
			m_pCurImage = &m_OverImage;
			m_fgCurColor = m_fgOverColor;
		}
	}
	else
	{
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
*/
	return we;
}

void CBarIcon::Render()
{
	static DWORD dwTime = gCurTime;
	static DWORD dwSpecialTime = gCurTime;
//	static BOOL bAlramRender = FALSE;
	static int n = 0;
	static int nSpecial = 0;
//	static int table[4] = { 0, 1, 2, 1 };

	cPushupButton::Render();

	if( m_bAlram == FALSE ) return;

//-----
/*
	m_ImgAlram[table[n]].RenderSprite( &m_scale, NULL, 0, &m_absPos, 0xffffffff );

	if( gCurTime - dwTime > 100 )
	{
		n = (n+1)%4;

		dwTime = gCurTime;
	}
*/
//----------

	m_ImgAlram[n].RenderSprite( &m_scale, NULL, 0, &m_absPos, 0xffffffff );

	if( gCurTime - dwTime > 150 )
	{
		n = (n+1)%3;

		dwTime = gCurTime;
	}

//----------
	if( !HERO ) return;
	
	if( HERO->GetLevel() <= 10 )
	{
		VECTOR2 vPos;
		vPos.x = m_absPos.x + 5;
		vPos.y = m_absPos.y - 30;

		m_ImgSpecialAlram[nSpecial].RenderSprite( &m_scale, NULL, 0, &vPos, 0xffffffff );
		if( gCurTime - dwSpecialTime > 100 )
		{
			nSpecial = (nSpecial+1)%3;
			dwSpecialTime = gCurTime;
		}
	}

/////////
/*
	if( bAlramRender )
	{
		m_ImgAlram[1].RenderSprite( &m_scale, NULL, 0, &m_absPos, 0xffffffff );

		if( gCurTime - dwTime > 500 )
		{
			bAlramRender = FALSE;
			dwTime = gCurTime;
		}

	}
	else
	{
		if( gCurTime - dwTime > 100 )
		{
			bAlramRender = TRUE;
			dwTime = gCurTime;
		}
	}
*/
}