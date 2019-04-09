// cIcon.cpp: implementation of the cIcon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cIcon.h"

#include "cWindowManager.h"
#include "cScriptManager.h"
#include "cButton.h"
#include "./Audio/MHAudioManager.h"
#include "ObjectManager.h"

#include "../Input/Mouse.h"
#include "cIconDialog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cIcon::cIcon()
{
	m_IconType		= 1;
	m_dwData		= 0;
	m_type			= WT_ICON;

	m_bLock			= FALSE;
//	m_pLockImage	= NULL;
	m_TitanEnduranceFlag = FALSE;	// magi82 - Titan(070424)
}


cIcon::~cIcon()
{
	PTRLISTSEARCHSTART(m_LinkDialogList,cIconDialog*,pDlg)
		pDlg->NotifyLinkIconDeleted(this);
	PTRLISTSEARCHEND
}


void cIcon::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)
{
	cDialog::Init(x,y,wid,hei,basicImage,ID);
	RECT capRect={0,0,wid,hei};
	SetCaptionRect(&capRect);
	
	//m_LockImage = WINDOWMGR->GetImageOfFile( "./image/GameIn/98/9810110.tif", 98 );
	
	SCRIPTMGR->GetImage( 65, &m_LockImage );
	SCRIPTMGR->GetImage( 65, &m_TitanEnduranceImage);	// magi82 - Titan(070424)
}


void cIcon::Render()
{
	if( !m_bActive )		return;
	
	if( m_bLock )
		m_LockImage.RenderSprite( NULL, NULL, 0, &m_absPos, 
									RGBA_MERGE(0xffffff, m_alpha * m_dwOptionAlpha / 100));

	cDialog::Render();

	if( m_TitanEnduranceFlag )
		m_TitanEnduranceImage.RenderSprite( NULL, NULL, 0, &m_absPos, RGBA_MERGE(0xffffff, m_alpha * m_dwOptionAlpha / 100));
}

void cIcon::Render(VECTOR2* pPos)
{
	VECTOR2 backup = m_absPos;
	m_absPos = *pPos;
	Render();
	m_absPos = backup;
}


DWORD cIcon::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	we |= cWindow::ActionEvent(mouseInfo);
	if(m_bDisable) return we;
	we |= cDialog::ActionEventComponent(mouseInfo);
//	if( !m_bActive ) return we;

//	long x = mouseInfo->GetMouseX();
//	long y = mouseInfo->GetMouseY();
	
	long x2 = mouseInfo->GetMouseEventX();
	long y2 = mouseInfo->GetMouseEventY();


	BOOL LeftUp = mouseInfo->LButtonUp();
	BOOL LeftPress = mouseInfo->LButtonPressed();
	BOOL click = mouseInfo->LButtonDown();

	if( m_pCloseBtn )
	{
		we = m_pCloseBtn->ActionEvent(mouseInfo);

		if(we & WE_BTNCLICK)
		{
			cbWindowFunc(m_ID, this, WE_CLOSEWINDOW);
			cWindowManager::OnWindowEvent(m_ID, m_pParent, WE_CLOSEWINDOW);	//PPP
			return (we |= WE_CLOSEWINDOW);
		}

		if( m_pCloseBtn->IsClickInside() )
			return we;
	}

	if( mouseInfo->LButtonDown() && !WINDOWMGR->IsMouseDownUsed() )
	{
		if( PtInWindow( x2, y2 ) )
		{
			if( m_bMovable && PtInCaption( x2, y2 ) )
				m_bCaptionHit = TRUE;
			
			we |= WE_TOPWINDOW;
		}
	}

	if( mouseInfo->LButtonUp() )
		m_bCaptionHit = FALSE;

	if( m_bCaptionHit )
	{
		if( mouseInfo->LButtonDrag() )
		{
			if(WINDOWMGR->m_OldDragFlag == FALSE)
			{					
				SetDepend(FALSE);
				
				//first enter
				float X = (float)x2;
				float Y = (float)y2;

				LONG OrigX = (LONG)m_absPos.x;
				LONG OrigY = (LONG)m_absPos.y;
				
				WINDOWMGR->SetDragStart(OrigX, OrigY, DEFAULT_ICONSIZE/2, DEFAULT_ICONSIZE/2, m_ID);
				
				m_bCaptionHit = FALSE;
				
				// taiyo temp//
				AUDIOMGR->Play(57, HERO);
			}
		}
	}
	return we;
}


void cIcon::SetLock( BOOL bLock )
{
	m_bLock = bLock;

//	SetMovable( !bLock );
}

void cIcon::AddLinkIconDialog(cIconDialog* pDialog)
{
	m_LinkDialogList.AddTail(pDialog);
}
void cIcon::RemoveLinkIconDialog(cIconDialog* pDialog)
{
	m_LinkDialogList.Remove(pDialog);
}
