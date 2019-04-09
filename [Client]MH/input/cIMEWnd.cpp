// cIMEWnd.cpp: implementation of the cIMEWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cIMEWnd.h"
#include "../Interface/cEditBox.h"
#include "../cIMEex.h"
#include "../Interface/cWindowManager.h"

extern HWND _g_hWnd;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(cIMEWnd);

cIMEWnd::cIMEWnd()
{
	m_pDockingEx = NULL;
	m_bCompositing = FALSE;
	m_bCtrlPushed = FALSE;
}

cIMEWnd::~cIMEWnd()
{

}

void cIMEWnd::SetDockingEx(cIMEex * box, BOOL bDock )
{
	if( bDock )
	{
		m_pDockingEx = box;
	}
	else
	{
		if( box == m_pDockingEx )
			m_pDockingEx = NULL;
	}
}

BOOL cIMEWnd::OnMsg(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if(!m_pDockingEx ) return FALSE;

	switch(msg)
	{
	case WM_IME_COMPOSITION:
	case WM_IME_CHAR:
		{
			return OnIMEComposition(hWnd, msg, wparam, lparam);
		}
		break;
	case WM_CHAR:
		{
			return OnChar(hWnd, msg, wparam, lparam);
		}
		break;
	case WM_KEYDOWN:
		{
			return OnKey(hWnd, msg, wparam, lparam);
		}
		break;
	case WM_KEYUP:
		{
			if( wparam == VK_CONTROL )
				m_bCtrlPushed = FALSE;
		}
		break;
	}
	return TRUE;
}

BOOL cIMEWnd::OnKey(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(wparam)
	{
	case VK_LEFT:
		if( m_pDockingEx->GetValidCheckMethod() != VCM_NUMBER )
			m_pDockingEx->CaretMoveLeft();
		break;

	case VK_RIGHT:
		m_pDockingEx->CaretMoveRight();
		break;

	case VK_UP:
		m_pDockingEx->CaretMoveUp();
		break;

	case VK_DOWN:
		m_pDockingEx->CaretMoveDown();
		break;

	case VK_HOME:
		if( m_pDockingEx->GetValidCheckMethod() != VCM_NUMBER )
			m_pDockingEx->CaretMoveHome();
		break;

	case VK_END:
		m_pDockingEx->CaretMoveEnd();
		break;

	case VK_RETURN:
		{
			m_pDockingEx->Enter();

//			if( m_pDockingEx->GetParentEdit() )
//				m_pDockingEx->GetParentEdit()->IMEEvent( eIMEE_ENTER );
			if( m_pDockingEx->GetParentEdit() )
				m_pDockingEx->GetParentEdit()->ExcuteCBFunc( WE_RETURN );
		}
		break;

	case VK_DELETE:
		m_pDockingEx->DeleteAfter();
		break;

	case VK_BACK:	//IME COMPOSING 시에는 안들어온다...
		m_pDockingEx->DeleteBefore();
		break;

	case VK_TAB:
		{
			//
			WINDOWMGR->SetNextEditFocus();
		}
		break;

	case VK_CONTROL:
		{
			m_bCtrlPushed = TRUE;
		}
		break;

	case 'V':
		{
			if( m_bCtrlPushed )
			if( IsClipboardFormatAvailable(CF_TEXT) )
			{
				if( OpenClipboard(hWnd) )
				{
					m_pDockingEx->Paste( (char*)GetClipboardData( CF_TEXT ) );
					CloseClipboard();
				}

			}

		}
		break;

	}

	return TRUE;
}

//one byte character
BOOL cIMEWnd::OnChar(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	char szChar[2];
	
	szChar[0] = wparam;
	szChar[1] = 0;

	if( m_pDockingEx->IsValidChar( (unsigned char*)szChar ) )
		m_pDockingEx->Insert( szChar );
	
	m_bCompositing = FALSE;

	if( m_pDockingEx )
	if( m_pDockingEx->IsChanged() )	//
	{
		m_pDockingEx->CheckAfterChange();
	}

	return TRUE;
}

BOOL cIMEWnd::OnIMEComposition(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch(msg)
	{
	case WM_IME_COMPOSITION:
		{
			char szCompText[3];

			if (lParam & GCS_COMPSTR)
			{
				szCompText[0] = wParam>>8;
				szCompText[1] = wParam;
				szCompText[2] = 0;

				if( szCompText[0] == 0 )
				{
					m_bCompositing = FALSE;
					m_pDockingEx->Insert( szCompText );
				}
				else if( m_pDockingEx->IsValidChar( (unsigned char*)szCompText ) )
				{
					if( m_pDockingEx->Insert( szCompText ) )
						m_bCompositing = TRUE;
					else
						m_bCompositing = FALSE;
				}
				else
				{
					m_bCompositing = TRUE;
				}

				m_pDockingEx->SetCompositing( m_bCompositing );
			}
		}
		break;

	case WM_IME_CHAR:
		{
			char szChar[3];
			if(IsDBCSLeadByte((BYTE)(wParam>>8)))
			{
				szChar[0]=HIBYTE(LOWORD(wParam));
				szChar[1]=LOBYTE(LOWORD(wParam));
				szChar[2]=0;
			}
			else
			{
				szChar[0]=(BYTE)wParam;
				szChar[1]=0;
			}

			if( m_pDockingEx->IsValidChar( (unsigned char*)szChar ) )
				m_pDockingEx->Insert( szChar );

			m_pDockingEx->SetCompositing( FALSE );
			m_bCompositing=FALSE;

		}
		break;
	}

	return TRUE;
}

void cIMEWnd::SetCompositing( BOOL bCompositing )
{
	m_pDockingEx->SetCompositing( FALSE );
	m_bCompositing=bCompositing;
}

