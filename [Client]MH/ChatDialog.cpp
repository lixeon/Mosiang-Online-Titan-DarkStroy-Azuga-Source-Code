// ChatDialog.cpp: implementation of the CChatDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChatDialog.h"
#include "WindowIdEnum.h"
#include "./Interface/cListDialog.h"
#include "./Interface/cEditBox.h"
#include "./Interface/cPushupButton.h"
#include "cImeEx.h"
#include "GlobalEventFunc.h"
#include "./Input/UserInput.h"
#include "./Input/Mouse.h"
#include "./Input/Keyboard.h"
#include "ChatManager.h"
#include "GameIn.h"
#include "OptionDialog.h"
#include "ShoutchatDialog.h"

#include "./Interface/cFont.h"

#include "input/UserInput.h"

extern HWND _g_hWnd;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChatDialog::CChatDialog()
{
	m_type				= WT_CHATDIALOG;
	m_pChatEditBox		= NULL;

	m_bHideChatDialog	= FALSE;
//	m_bShowMenu			= FALSE;
	m_bShowGuildTab		= FALSE;

	m_cPreWord[0] = 0;
	m_cPreWord[1] = '@';
	m_cPreWord[2] = '#';
	m_cPreWord[3] = '$';
	m_cPreWord[4] = '%';
}

CChatDialog::~CChatDialog()
{

}

void CChatDialog::Linking()
{
	m_pChatEditBox	= (cEditBox *)GetWindowForID(MI_CHATEDITBOX);
	m_pChatEditBox->SetValidCheck( VCM_DEFAULT );
	m_pChatEditBox->SetEditFunc( GAMEIN_ChatFunc );

	m_pSheet[WHOLE]		= (cListDialog*)GetWindowForID(CTI_SHEET1);
	m_pSheet[PARTY]		= (cListDialog*)GetWindowForID(CTI_SHEET2);
	m_pSheet[GUILD]		= (cListDialog*)GetWindowForID(CTI_SHEET3);
	m_pSheet[ALLIANCE]	= (cListDialog*)GetWindowForID(CTI_SHEET4);
	m_pSheet[SHOUT]		= (cListDialog*)GetWindowForID(CTI_SHEET5);

	m_pPBMenu[WHOLE]	= (cPushupButton*)GetWindowForID(CTI_BTN_WHOLE);
	m_pPBMenu[PARTY]	= (cPushupButton*)GetWindowForID(CTI_BTN_PARTY);
	m_pPBMenu[GUILD]	= (cPushupButton*)GetWindowForID(CTI_BTN_MUNPA);
	m_pPBMenu[ALLIANCE]	= (cPushupButton*)GetWindowForID(CTI_BTN_ALLMUNPA);
	m_pPBMenu[SHOUT]	= (cPushupButton*)GetWindowForID(CTI_BTN_WORLD);

	m_pAllShout = (cPushupButton*)GetWindowForID( CTI_BTN_ALLWORLD1 );

	m_pAllShout->SetPush( TRUE );


//	m_lMenuX = (LONG)m_pPBMenu[WHOLE]->GetAbsX();
//	m_lMenuY = (LONG)m_pPBMenu[WHOLE]->GetAbsY();
//	m_lMenuHeight = (LONG)m_pPBMenu[WHOLE]->GetHeight();

	m_pSheet[WHOLE]->SetActive( TRUE );
	m_pSheet[PARTY]->SetActive( FALSE );
	m_pSheet[GUILD]->SetActive( FALSE );
	m_pSheet[ALLIANCE]->SetActive( FALSE );
	m_pSheet[SHOUT]->SetActive( FALSE );

//	m_pPBMenu[WHOLE]->SetActive( TRUE );
//	m_pPBMenu[PARTY]->SetActive( FALSE );
//	m_pPBMenu[GUILD]->SetActive( FALSE );
//	m_pPBMenu[ALLIANCE]->SetActive( FALSE );
//	m_pPBMenu[SHOUT]->SetActive( FALSE );

	m_nCurSheetNum = WHOLE;
	SelectMenu( m_nCurSheetNum );

//메뉴버튼위치
//버튼높이
}

DWORD CChatDialog::ActionEvent( CMouse * mouseInfo )
{
    DWORD we = cDialog::ActionEvent( mouseInfo );

	//임시로 유효영역 늘이기. 구조상 어쩔 수 없다.
	cListDialog* pCurSheet = m_pSheet[m_nCurSheetNum];
	WORD width = pCurSheet->GetWidth();
	WORD height = pCurSheet->cWindow::GetHeight();
	if( KEYBOARD->GetKeyPressed(KEY_CONTROL) )
	{
		pCurSheet->SetWH(410, height);
		if( mouseInfo->LButtonDown() )
		{
			GetSelectedName(mouseInfo);
		}
	}
	else
		pCurSheet->SetWH(14, height);
		

/*	
	if( m_bShowMenu )
	if( mouseInfo->LButtonDown()  ) //다른곳 클릭하면 메뉴 감추기
	{
		if( !(we & WE_MOUSEOVER) )
			ShowMenu( FALSE );
	}
	else
	{
		we |= WE_LBTNCLICK;
	}
*/	
	return we;
}




void CChatDialog::AddMsg( BYTE ChatLimit, DWORD MsgColor, char* str )
{
#ifdef _TL_LOCAL_
	int nLen = strlen( str );
	char buf[256];

	BYTE brk[512];
	int nBrkNum = g_TBreak.FindThaiWordBreak( str, nLen, brk, 512, FTWB_SEPARATESYMBOL );
	int nCutPos = 0;

	int nCount = 0;

	for( int i = 0 ; i < nBrkNum ; ++i )
	{
		++nCount;	//몇단어썼나
		nCutPos += brk[i];
		if( CFONT_OBJ->GetTextExtentEx(0, str, nCutPos ) > CHATLIST_TEXTEXTENT )
		{
			if( nCount == 1 )	//한단어만 썼는데 넘어가버리면 강제로 끊는다.
			{
				for( int k = 1 ; k <= nCutPos ; ++k )
				{
					if( CFONT_OBJ->GetTextExtentEx(0, str, k ) > CHATLIST_TEXTEXTENT )
					{
						nCutPos = nCutPos - brk[i] + k-1; //쓴 글자까지만
						strncpy( buf, str, nCutPos );	//전단계까지
						buf[nCutPos] = 0;
						GetSheet(ChatLimit)->AddItem( buf, MsgColor );
						str += nCutPos;
						brk[i] -= nCutPos;	//나머지부분 계산
						nCutPos = 0;
						nCount = 0; //초기화
						--i;
						break;
					}
				}
			}
			else
			{
				nCutPos -= brk[i];
				strncpy( buf, str, nCutPos );	//전단계까지
				buf[nCutPos] = 0;
				GetSheet(ChatLimit)->AddItem( buf, MsgColor );
				--i;
				str += nCutPos;
				nCutPos = 0;
				nCount = 0; //초기화
			}

			if( *str == ' ' )
			{
				++str;	//라인 첫글자가 스페이스가 아니도록...
				++i;
			}
		}
	}

	if( nCutPos > 0 )
		GetSheet(ChatLimit)->AddItem( str, MsgColor );
#else

	int nLen = strlen( str );

	char buf[256];
	int nCpyNum = 0;

	BOOL btwoline = FALSE;
	while( nLen > CHATLIST_TEXTLEN )
	{
		if( ( str + CHATLIST_TEXTLEN ) != CharNext( CharPrev( str, str + CHATLIST_TEXTLEN ) ) )
			nCpyNum = CHATLIST_TEXTLEN - 1;
		else
			nCpyNum = CHATLIST_TEXTLEN;

		strncpy( buf, str, nCpyNum );
		buf[nCpyNum] = 0;
		GetSheet(ChatLimit)->AddItem( buf, MsgColor );
		nLen -= nCpyNum;
		str += nCpyNum;
		if( *str == ' ' ) ++str;	//라인 첫글자가 스페이스가 아니도록...

		btwoline = TRUE;
	}

#ifdef _HK_LOCAL_

	if( nLen > 0 )
	{
		if( btwoline )
		{
			char buf[256]={ 0, };
			sprintf( buf, "--%s", str );
			GetSheet(ChatLimit)->AddItem( buf, MsgColor );
		}
		else
			GetSheet(ChatLimit)->AddItem( str, MsgColor );
	}
#else

	if( nLen > 0 )
		GetSheet(ChatLimit)->AddItem( str, MsgColor );

#endif // _HK_LOCAL_

#endif // _TL_LOCAL_

}

void CChatDialog::AddMsgAll( DWORD MsgColor, char* str )
{
#ifdef _TL_LOCAL_
	int nLen = strlen( str );
	char buf[256];

	BYTE brk[512];
	int nBrkNum = g_TBreak.FindThaiWordBreak( str, nLen, brk, 512, FTWB_SEPARATESYMBOL );
	int nCutPos = 0;

	for( int i = 0 ; i < nBrkNum ; ++i )
	{
		nCutPos += brk[i];
		if( CFONT_OBJ->GetTextExtentEx(0, str, nCutPos ) > CHATLIST_TEXTEXTENT )
		{
			nCutPos -= brk[i];
			strncpy( buf, str, nCutPos );	//전단계까지
			buf[nCutPos] = 0;
			GetSheet(WHOLE)->AddItem( buf, MsgColor );
			GetSheet(PARTY)->AddItem( buf, MsgColor );
			GetSheet(GUILD)->AddItem( buf, MsgColor );
			--i;
			str += nCutPos;
			nCutPos = 0;
			if( *str == ' ' )
			{
				++str;	//라인 첫글자가 스페이스가 아니도록...
				++i;
			}
		}
	}

	if( nCutPos > 0 )
	{
		GetSheet(WHOLE)->AddItem( str, MsgColor );
		GetSheet(PARTY)->AddItem( str, MsgColor );
		GetSheet(GUILD)->AddItem( str, MsgColor );
	}
#else

	int nLen = strlen( str );

	char buf[128];
	int nCpyNum = 0;

	while( nLen > CHATLIST_TEXTLEN )
	{
		if( ( str + CHATLIST_TEXTLEN ) != CharNext( CharPrev( str, str + CHATLIST_TEXTLEN ) ) )
			nCpyNum = CHATLIST_TEXTLEN - 1;
		else
			nCpyNum = CHATLIST_TEXTLEN;

		strncpy( buf, str, nCpyNum );
		buf[nCpyNum] = 0;
		GetSheet(WHOLE)->AddItem( buf, MsgColor );
		GetSheet(PARTY)->AddItem( buf, MsgColor );
		GetSheet(GUILD)->AddItem( buf, MsgColor );
		nLen -= nCpyNum;
		str += nCpyNum;
		if( *str == ' ' ) ++str;	//라인 첫글자가 스페이스가 아니도록...
	}

	if( nLen > 0 )
	{
		GetSheet(WHOLE)->AddItem( str, MsgColor );
		GetSheet(PARTY)->AddItem( str, MsgColor );
		GetSheet(GUILD)->AddItem( str, MsgColor );
	}

#endif
}

void CChatDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		switch( lId )
		{
		case CTI_BTN_TALLER:
			{
				m_pSheet[WHOLE]->onSize( TRUE );
				m_pSheet[PARTY]->onSize( TRUE );
				m_pSheet[GUILD]->onSize( TRUE );
				m_pSheet[ALLIANCE]->onSize( TRUE );
				m_pSheet[SHOUT]->onSize( TRUE );

				if( GAMEIN->GetShoutchatDlg() )
					GAMEIN->GetShoutchatDlg()->RefreshPosition();
			}
			break;
		case CTI_BTN_SMALLER:
			{
				m_pSheet[WHOLE]->onSize( FALSE );
				m_pSheet[PARTY]->onSize( FALSE );
				m_pSheet[GUILD]->onSize( FALSE );
				m_pSheet[ALLIANCE]->onSize( FALSE );
				m_pSheet[SHOUT]->onSize( FALSE );

				if( GAMEIN->GetShoutchatDlg() )
					GAMEIN->GetShoutchatDlg()->RefreshPosition();
			}
			break;
		case CTI_BTN_OPTION:
			{
//				if( GAMEIN->GetOptionDialog()->IsActive() )
//				{
//					GAMEIN->GetOptionDialog()->SetActive( FALSE );
//				}
//				else
				{
					GAMEIN->GetOptionDialog()->SetActive( TRUE );
					GAMEIN->GetOptionDialog()->SelectTab( 1 );
				}
			}
			break;
		case CTI_BTN_ALLWORLD1:
			{
				if( GAMEIN->GetShoutchatDlg() )
					GAMEIN->GetShoutchatDlg()->SetActive( !GAMEIN->GetShoutchatDlg()->IsActive() );
			}
			break;
		}
	}
	else if( we & WE_PUSHDOWN  )
	{
		switch( lId )
		{
		case CTI_BTN_WHOLE:
			SelectMenu(WHOLE);
			break;
		case CTI_BTN_PARTY:
			SelectMenu(PARTY);
			break;
		case CTI_BTN_MUNPA:
			SelectMenu(GUILD);
			break;
		case CTI_BTN_ALLMUNPA:
#ifdef _JAPAN_LOCAL_
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(710) );
			SelectMenu(m_nCurSheetNum);
#else
			SelectMenu(ALLIANCE);
#endif
			break;
		case CTI_BTN_WORLD:
//			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(710) );
//			SelectMenu(m_nCurSheetNum);
			SelectMenu(SHOUT);
			break;
		case CTI_BTN_ALLWORLD1:
//#ifdef _JAPAN_LOCAL_
//			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(710) );
//			if( m_pAllShout )
//				m_pAllShout->SetPush( FALSE );
//#else
			if( GAMEIN->GetShoutchatDlg() )
				GAMEIN->GetShoutchatDlg()->SetActive( TRUE );
//#endif
			break;
		}

		m_pChatEditBox->SetFocusEdit( TRUE );		
	}
	else if( we & WE_PUSHUP)
	{
		SelectMenu(m_nCurSheetNum);
		m_pChatEditBox->SetFocusEdit( TRUE );
		
		switch( lId )
		{
		case CTI_BTN_ALLWORLD1:	
			if( GAMEIN->GetShoutchatDlg() )
				GAMEIN->GetShoutchatDlg()->SetActive( FALSE );
			break;
		}
	}
	else if( we & WE_LBTNDBLCLICK )
	{
		int k = 0;
	}
}

void CChatDialog::SetEditBoxPreWord()
{
	char buf[256] = {0,};
	
	SafeStrCpy( buf, m_pChatEditBox->GetEditText(), 128 );

	int nLen = strlen( buf );
	if( nLen == 0 )
	{
		buf[0] = m_cPreWord[m_nCurSheetNum];
	}
	else if( buf[0] != '/' )	//귓말중이 아닐때만
	{
		if( m_nCurSheetNum != WHOLE )
		{
			if( IsPreWord( buf[0] ) == FALSE )
			{
				memmove( buf+1, buf, nLen );
			}
			buf[0] = m_cPreWord[m_nCurSheetNum];
		}
		else
		{
			if( IsPreWord( buf[0] ) == TRUE )
			{
				memmove( buf, buf+1, nLen );
			}
		}
	}

	m_pChatEditBox->SetEditText( buf );
}

BOOL CChatDialog::IsPreWord( char c )
{
	for( int i = PARTY ; i <= SHOUT ; ++i )
	{
		if( m_cPreWord[i] == c ) return TRUE;
	}

	return FALSE;		
}

/*
void CChatDialog::ShowMenu( BOOL bShow )
{
	m_bShowMenu = bShow;

	if( bShow )
	{
		LONG y = m_lMenuY;
		
		for( int i = 0 ; i < MAX_CHAT_COUNT ; ++i )
		{
			if( i != m_nCurSheetNum )
			{
				y -= m_lMenuHeight;
				m_pPBMenu[i]->SetAbsXY( m_lMenuX, y );
				m_pPBMenu[i]->SetActive( TRUE );
			}
		}
	}
	else
	{
		for( int i = 0 ; i < MAX_CHAT_COUNT ; ++i )
		{
			if( i != m_nCurSheetNum )
				m_pPBMenu[i]->SetActive( FALSE );
			else
				m_pPBMenu[i]->SetActive( TRUE );

			m_pPBMenu[i]->SetPush( FALSE );
		}
	}
}
*/

void CChatDialog::SelectMenu( int nSheet )
{
	if( nSheet >= MAX_CHAT_COUNT ) return;

	for( int i = 0 ; i < MAX_CHAT_COUNT ; ++i )
	{
		if( nSheet == i )
			m_pPBMenu[i]->SetPush(TRUE);
		else
			m_pPBMenu[i]->SetPush(FALSE);
	}

	m_nCurSheetNum = nSheet;
	SetEditBoxPreWord();

//	if( nSheet < 0 )		//Menu HIDE
//	{
//		ShowMenu( FALSE );
//	}
//	else if( m_nCurSheetNum == nSheet )
//	{
//		ShowMenu( TRUE );
//	}
//	else
//	{
//		m_pPBMenu[nSheet]->SetAbsXY( m_lMenuX, m_lMenuY );
/*
		for( int i = 0 ; i < MAX_CHAT_COUNT ; ++i )
		{
			if( i != nSheet )
			{
				m_pSheet[i]->SetActive( FALSE );
			}
			else
			{
				if( !m_bHideChatDialog )
					m_pSheet[i]->SetActive( TRUE );

				m_pPBMenu[i]->SetAbsXY( m_lMenuX, m_lMenuY );
			}
		}
*/
//		m_nCurSheetNum = nSheet;
//		ShowMenu( FALSE );
//	}
}


void CChatDialog::HideChatDialog( BOOL bHide )
{
	m_bHideChatDialog = bHide;

	if( bHide )
	{
		//m_pSheet[m_nCurSheetNum]->SetActive( FALSE );
		m_pSheet[WHOLE]->SetActive( FALSE );
	}
	else
	{
		//m_pSheet[m_nCurSheetNum]->SetActive( TRUE );
		m_pSheet[WHOLE]->SetActive( TRUE );
	}
}


void CChatDialog::ShowGuildTab( BOOL bShow )
{
//	ShowMenu( FALSE );

//	if( bShow == FALSE )
//	if( m_nCurSheetNum == GUILD )
//	{
//		SelectMenu( WHOLE );
//	}
//
//	m_bShowGuildTab = bShow;
}

BYTE CChatDialog::GetLineNum()
{ 
	return m_pSheet[m_nCurSheetNum]->GetMiddleImageNum();
}

WORD CChatDialog::GetSheetPosY()
{
	return (WORD)m_pSheet[m_nCurSheetNum]->GetAbsY();
}

WORD CChatDialog::GetSheetHeight()
{
	DWORD height = 0;

	height += m_pSheet[m_nCurSheetNum]->GetHeight( LD_TOP );
	height += m_pSheet[m_nCurSheetNum]->GetHeight( LD_DOWN );
	height += ( m_pSheet[m_nCurSheetNum]->GetHeight( LD_MIDDLE )*m_pSheet[m_nCurSheetNum]->GetMiddleImageNum() );

	return (WORD)height;	
}

void CChatDialog::SetAllShoutBtnPushed( BOOL val )
{
	if( m_pAllShout )
		m_pAllShout->SetPush( val );
}

void CChatDialog::GetSelectedName(CMouse* mouseInfo)
{
	memset(m_SelectedName, 0, MAX_NAME_LENGTH+1);

	cListDialog* pCurSheet = m_pSheet[m_nCurSheetNum];

	ITEM* pItem = pCurSheet->GetSelectedITEM(mouseInfo);
	if(!pItem)
		return;

   	if( pItem->rgb != dwChatTextColor[CTC_GENERALCHAT] && 
		pItem->rgb != dwChatTextColor[CTC_PARTYCHAT] && 
		pItem->rgb != dwChatTextColor[CTC_GUILDCHAT] && 
		pItem->rgb != dwChatTextColor[CTC_ALLIANCE] && 
		pItem->rgb != dwChatTextColor[CTC_SHOUT] && 
		pItem->rgb != dwChatTextColor[CTC_WHISPER] )
		return;
		
	int nLen = strlen(pItem->string);

	char* pStr = NULL;
	if( nLen )
		pStr = pItem->string;

	int nCpyNum = 0;

	if( *pStr == '[' )
	{
		++pStr;
	}
	else
	{
		return;
	}

	BOOL bNeedCpy = FALSE;
	BOOL bDevelopid = FALSE;
	for( int i = 0; i < nLen; ++i )
	{
		if( pStr[i] == ']' )
		{
			bNeedCpy = TRUE;			
#ifdef _KOR_LOCAL_
			char tbuf[MAX_NAME_LENGTH+1] = { 0, };
			SafeStrCpy( tbuf, pStr, nCpyNum+1 );

			if( (strncmp( pStr, "[청룡", 5 ) == 0 || strncmp( pStr, "[황룡", 5 ) == 0) &&
				!bDevelopid )
			{
                bDevelopid = TRUE;
				++nCpyNum;
				continue;
			}
#endif
			break;
		}

		++nCpyNum;
	}

	if( nCpyNum > MAX_NAME_LENGTH )
		return;
	
	if( bNeedCpy )
	{
		if( bDevelopid )
			SafeStrCpy(m_SelectedName, pStr, nCpyNum + 2);
		else
			SafeStrCpy(m_SelectedName, pStr, nCpyNum + 1);

		if( OpenClipboard(_g_hWnd) )
		{
			EmptyClipboard();

			HGLOBAL hMem = GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE, sizeof(m_SelectedName)+1 );
			if( hMem )
			{
				LPTSTR pClipData = (LPTSTR) GlobalLock(hMem);
				pClipData[0] = 0;
				SafeStrCpy( pClipData, m_SelectedName, sizeof(m_SelectedName)+1 );
				SetClipboardData( CF_OEMTEXT, hMem );
				GlobalUnlock( hMem );
			}
			CloseClipboard();
			GlobalFree( hMem );	

			//ChatMsg추가
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1630), m_SelectedName );
		}		
	}
	else
		return;

}
