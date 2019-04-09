
#include "stdafx.h"
#include "ChatManager.h"
#include "MacroDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cEditBox.h"
#include "./Input/UserInput.h"




void CMacroDialog::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)
{
	cDialog::Init(x,y,wid,hei,basicImage,ID);

	ZeroMemory( m_pMacroKeyEdit, sizeof( m_pMacroKeyEdit ) );
//	ZeroMemory( m_pModeBtn, sizeof( m_pModeBtn ) );	//
	m_pFocusEdit	= NULL;
	m_nCurMacro		= 0;

//	m_nMode = MACROMGR->GetUserSetMode();///
}



DWORD CMacroDialog::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;

	we = cDialog::ActionEvent(mouseInfo);

	char buf[16];

	if( !m_pFocusEdit->IsFocus() )		//KES 030825 ÇÏµåÄÚµùÀ¸·Î Ã¤ÆÃ°ú Ãæµ¹ÇÏ´Â°Í ¸·À½
	{
		CHATMGR->SetPause( FALSE );

		if( m_bCombining )
		{
			ConvertMacroToText( buf, &m_MacroKey[ m_nCurMacro ] );
			m_pFocusEdit->SetEditText( buf );
			m_bCombining = FALSE;
			m_tempMacroKey.nSysKey = MSK_NONE;
		}

		return we;
	}
	else
	{
		CHATMGR->SetPause( TRUE );
	}
	
	WORD wKey;
	int nEvent = MACROMGR->IsUsableKey( KEYBOARD, &wKey, m_nMode );

	if( nEvent == 1 )
	{
		m_tempMacroKey.wKey = wKey;
		
		if( !IsAlreadySet( &m_tempMacroKey ) )
		{
			m_MacroKey[ m_nCurMacro ] = m_tempMacroKey;
			
			ConvertMacroToText( buf, &m_MacroKey[ m_nCurMacro ] );
			m_pFocusEdit->SetEditText( buf );
			
			SetNextEditFocus();
			m_bChanged = TRUE;
		}
	}
	else if( nEvent == 2 )
	{
		m_bCombining = TRUE;

		if( wKey == KEY_CONTROL )			m_tempMacroKey.nSysKey	= MSK_CTRL;
		else if( wKey == KEY_MENU )			m_tempMacroKey.nSysKey = MSK_ALT;
		else if( wKey == KEY_SHIFT )		m_tempMacroKey.nSysKey = MSK_SHIFT;
	
		m_tempMacroKey.wKey = 0;
		ConvertMacroToText( buf, &m_tempMacroKey );
		m_pFocusEdit->SetEditText( buf );
	}
	else
	{
		if( m_bCombining )
		{
			ConvertMacroToText( buf, &m_MacroKey[ m_nCurMacro ] );
			m_pFocusEdit->SetEditText( buf );
			m_bCombining = FALSE;
			m_tempMacroKey.nSysKey = MSK_NONE;
		}
	}
	
//	if( !m_pFocusEdit->IsFocus() )
//		m_pFocusEdit->SetFocusEdit( TRUE );	//´Ù¸¥ÀÛ¾÷À» ÇÏ´õ¶óµµ Ç×»ó Æ÷Ä¿½º...

	return we;
}

void CMacroDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	//¼¼ÆÃÀÌ ¾ÈµÇ´Â ¸ÅÅ©·ÎÀÇ ¿¡µðÆ®´Â Æ÷Ä¿½º¸¦ °®Áö ¸øÇÏµµ·Ï Ã¼Å©Ãß°¡

	//Ã¹¹øÂ° ¿¡µðÆ®ÀÇ ¾ÆÀÌµð´Â MAC_EB_QUICKITEM01
	if( we & WE_SETFOCUSON )
	{
		if( m_bCombining )	//Á¶ÇÕÁß¿£ Æ÷Ä¿½º¸¦ ¹Ù²ÙÁö ¸øÇÑ´Ù
		{
			((cEditBox *)GetWindowForID(lId))->SetFocusEdit( FALSE );	
			m_pFocusEdit->SetFocusEdit( TRUE );
			return;
		}

		m_pFocusEdit->SetFocusEdit( FALSE );

		m_nCurMacro = lId - MAC_EB_QUICKITEM01;
		//ÀÓ½Ã...ÇöÀç ÆÄÆ¼Ã³·³ ´ÙÀÌ¾ó·Î±×¿¡ ¾ø´Â ¸ÅÅ©·Î ¶§¹®¿¡.......
		if( m_nCurMacro >= 11 )		++m_nCurMacro;

		m_pFocusEdit = m_pMacroKeyEdit[m_nCurMacro];
		m_pFocusEdit->SetFocusEdit( TRUE );
	}
	else if( we & WE_BTNCLICK )
	{
		if( lId == MAC_BTN_SAVE )
		{
			UpdateData( TRUE );
			SetActive( FALSE );
		}
		else if( lId == MAC_BTN_DEFAULT )
		{
			UpdateData( FALSE, TRUE );		//...
		}
	}
/*
	else if( we & WE_PUSHUP )
	{
		m_pModeBtn[lId - MAC_BTN_USERCHATMODE]->SetPush( TRUE );
	}
	else if( we & WE_PUSHDOWN )
	{
		if( lId == MAC_BTN_USERCHATMODE )
		{
			m_nMode = 0;
			m_pModeBtn[0]->SetPush( TRUE );
			m_pModeBtn[1]->SetPush( FALSE );
			//if m_bChagned¸é ÀúÀå?
			UpdateData( FALSE );
		}
		else if( lId == MAC_BTN_USERGENERALMODE )
		{
			m_nMode = 1;
			m_pModeBtn[0]->SetPush( FALSE );
			m_pModeBtn[1]->SetPush( TRUE );
			//if m_bChagned¸é ÀúÀå?
			UpdateData( FALSE );
		}
	}
*/
}


void CMacroDialog::SetNextEditFocus()
{
	m_pFocusEdit->SetFocusEdit( FALSE );
	
	while(1)	//ÇöÀç ÆÄÆ¼Ã³·³ ´ÙÀÌ¾ó·Î±×¿¡ ¾ø´Â ¸ÅÅ©·Î ¶§¹®¿¡.......
	{
		++m_nCurMacro;
		if( m_nCurMacro >= ME_COUNT )
			m_nCurMacro = 0;
		
		m_pFocusEdit = m_pMacroKeyEdit[ m_nCurMacro ];
		if( m_pFocusEdit != NULL )
		{
			break;
		}
	}

	m_pFocusEdit->SetFocusEdit( TRUE );
}


void CMacroDialog::SetActive(BOOL val)
{
	if( m_bDisable ) return;

	MACROMGR->SetPause( val );		//¸ÅÅ©·Î ¼¼ÆÃÁß¿£ ¸ÅÅ©·Î Á¤Áö
	CHATMGR->SetPause( val );

	if( val )
	{
//		m_nMode = MACROMGR->GetUserSetMode();		//
//		m_pModeBtn[m_nMode]->SetPush( TRUE );		//
//		m_pModeBtn[!m_nMode]->SetPush( FALSE );		//

		UpdateData( FALSE );
	}
	else
	{
		//if m_bChagned¸é ÀúÀåÇÒÁö ¹°¾îº¸±â?
	}

	cDialog::SetActive(val);
}


void CMacroDialog::UpdateData( BOOL bSave, BOOL bDefault )
{
	if( bSave )
	{
		MACROMGR->SetMacro( m_nMode, m_MacroKey );
//		MACROMGR->SetUserSetMode( m_nMode );

//		if( MACROMGR->GetMacroMode() >= MM_USERSET_CHAT )
//			MACROMGR->SetMacroMode( MM_USERSET_CHAT + m_nMode );

		MACROMGR->SaveUserMacro();
	}
	else
	{
		m_pFocusEdit->SetFocusEdit( FALSE );
		m_pFocusEdit	= m_pMacroKeyEdit[0];
		m_pFocusEdit->SetFocusEdit( TRUE );

		m_nCurMacro		= 0;
		m_bCombining	= FALSE;

		m_tempMacroKey.nSysKey	= MSK_NONE;
		m_tempMacroKey.wKey		= 0;

		if( bDefault )
			MACROMGR->GetDefaultMacro( m_nMode, m_MacroKey );
		else
			MACROMGR->GetMacro( m_nMode, m_MacroKey );

		char buf[16];
		for( int i = 0 ; i < ME_COUNT ; ++i )
		{
			ConvertMacroToText( buf, &m_MacroKey[i] );
			if( m_pMacroKeyEdit[i] )
				m_pMacroKeyEdit[i]->SetEditText( buf );
		}

		m_bChanged		= FALSE;
	}
}


void CMacroDialog::Linking()
{
//	m_pModeBtn[0] = (cPushupButton*)GetWindowForID(MAC_BTN_USERCHATMODE);
//	m_pModeBtn[1] = (cPushupButton*)GetWindowForID(MAC_BTN_USERGENERALMODE);

	m_pMacroKeyEdit[ME_USE_QUICKITEM01] = (cEditBox *)GetWindowForID(MAC_EB_QUICKITEM01);
	m_pMacroKeyEdit[ME_USE_QUICKITEM02] = (cEditBox *)GetWindowForID(MAC_EB_QUICKITEM02);
	m_pMacroKeyEdit[ME_USE_QUICKITEM03] = (cEditBox *)GetWindowForID(MAC_EB_QUICKITEM03);
	m_pMacroKeyEdit[ME_USE_QUICKITEM04] = (cEditBox *)GetWindowForID(MAC_EB_QUICKITEM04);
	m_pMacroKeyEdit[ME_USE_QUICKITEM05] = (cEditBox *)GetWindowForID(MAC_EB_QUICKITEM05);
	m_pMacroKeyEdit[ME_USE_QUICKITEM06] = (cEditBox *)GetWindowForID(MAC_EB_QUICKITEM06);
	m_pMacroKeyEdit[ME_USE_QUICKITEM07] = (cEditBox *)GetWindowForID(MAC_EB_QUICKITEM07);

//	m_pMacroKeyEdit[ME_PAGEUP_QUICKSLOT] = (cEditBox *)GetWindowForID(MAC_EB_PGUP_QUICKSLOT);
//	m_pMacroKeyEdit[ME_PAGEDN_QUICKSLOT] = (cEditBox *)GetWindowForID(MAC_EB_PGDN_QUICKSLOT);

	m_pMacroKeyEdit[ME_TOGGLE_INVENTORYDLG] = (cEditBox *)GetWindowForID(MAC_EB_INVENTORYDLG);
	m_pMacroKeyEdit[ME_TOGGLE_CHARACTERDLG] = (cEditBox *)GetWindowForID(MAC_EB_CHARACTERDLG);
	m_pMacroKeyEdit[ME_TOGGLE_MUGONGDLG]	= (cEditBox *)GetWindowForID(MAC_EB_MUGONGDLG);
//	m_pMacroKeyEdit[ME_TOGGLE_PARTYDLG]		= (cEditBox *)GetWindowForID(MAC_EB_PARTYDLG);
//	m_pMacroKeyEdit[ME_TOGGLE_MUNPADLG]		= (cEditBox *)GetWindowForID(MAC_EB_MUNPADLG);
	m_pMacroKeyEdit[ME_TOGGLE_QUESTDLG]		= (cEditBox *)GetWindowForID(MAC_EB_QUESTDLG);
	m_pMacroKeyEdit[ME_TOGGLE_EXITDLG]		= (cEditBox *)GetWindowForID(MAC_EB_EXITDLG);

//	m_pMacroKeyEdit[ME_CLOSE_ACTIVEDLG]		= (cEditBox *)GetWindowForID(MAC_EB_CLOSE_ACTIVEDLG);

 	m_pMacroKeyEdit[ME_TOGGLE_MOVEMODE]		= (cEditBox *)GetWindowForID(MAC_EB_MOVEMODE);
 	m_pMacroKeyEdit[ME_TOGGLE_PEACEWARMODE]	= (cEditBox *)GetWindowForID(MAC_EB_PEACEMODE);
//	m_pMacroKeyEdit[ME_TOGGLE_UNGIMODE]		= (cEditBox *)GetWindowForID(MAC_EB_UNGIMODE);
//	m_pMacroKeyEdit[ME_TOGGLE_KYUNGGONG]	= (cEditBox *)GetWindowForID(MAC_EB_GYUNGGONG);
	m_pMacroKeyEdit[ME_TOGGLE_AUTOATTACK]	= (cEditBox *)GetWindowForID(MAC_EB_AUTOATTACK);

//	m_pMacroKeyEdit[ME_TOGGLE_HELP]			= (cEditBox *)GetWindowForID(MAC_EB_HELP);

	m_pMacroKeyEdit[ME_TOGGLE_MINIMAP]		= (cEditBox *)GetWindowForID(MAC_EB_MINIMAP);

	m_pMacroKeyEdit[ME_TOGGLE_CAMERAVIEWMODE]	= (cEditBox *)GetWindowForID(MAC_EB_FREEVIEW);
//	m_pMacroKeyEdit[ME_CAMERAMODE_BACKVIEW]		= (cEditBox *)GetWindowForID(MAC_EB_BACKVIEW);
//	m_pMacroKeyEdit[ME_CAMERAMODE_QUARTERVIEW]	= (cEditBox *)GetWindowForID(MAC_EB_QUARTERVIEW);
//	m_pMacroKeyEdit[ME_CAMERAMODE_LOADVIEW]		= (cEditBox *)GetWindowForID(MAC_EB_LOADVIEW);
//	m_pMacroKeyEdit[ME_CAMERAMODE_SAVEVIEW]		= (cEditBox *)GetWindowForID(MAC_EB_SAVEVIEW);

//	m_pMacroKeyEdit[ME_CAMERAMOVE_LEFT]		= (cEditBox *)GetWindowForID(MAC_EB_CAMERAMOVE_LEFT);
//	m_pMacroKeyEdit[ME_CAMERAMOVE_RIGHT]	= (cEditBox *)GetWindowForID(MAC_EB_CAMERAMOVE_RIGHT);
//	m_pMacroKeyEdit[ME_CAMERAMOVE_UP]		= (cEditBox *)GetWindowForID(MAC_EB_CAMERAMOVE_UP);
//	m_pMacroKeyEdit[ME_CAMERAMOVE_DOWN]		= (cEditBox *)GetWindowForID(MAC_EB_CAMERAMOVE_DOWN);	
//	m_pMacroKeyEdit[ME_CAMERAMOVE_ZOOMIN]	= (cEditBox *)GetWindowForID(MAC_EB_CAMERAMOVE_ZOOMIN);	  	
//	m_pMacroKeyEdit[ME_CAMERAMOVE_ZOOMOUT]	= (cEditBox *)GetWindowForID(MAC_EB_CAMERAMOVE_ZOOMOUT);

	m_pMacroKeyEdit[ME_SCREENCAPTURE]		= (cEditBox *)GetWindowForID(MAC_EB_SCREENCAPTURE);
	
	for( int i = ME_USE_QUICKITEM01 ; i <= ME_SCREENCAPTURE ; ++i )
	{
		if( m_pMacroKeyEdit[i] )
			m_pMacroKeyEdit[i]->ShowCaretInReadOnly( TRUE );
	}

	m_pFocusEdit	= m_pMacroKeyEdit[0];
}



#define STARTCONVERT(v)			switch(v) {
#define CONVERT(key,str1,str2)	case key: strcat(str1,str2); break;
#define CONVERTCOPY(key,str1,str2)	case key: strcpy(str1,str2); break;
#define ENDCONVERT				}

void CMacroDialog::ConvertMacroToText( char* str, sMACRO* pMacro )
{
	*str = 0;	//strcatÀ» ¾²±âÀ§ÇØ
	
	switch( pMacro->nSysKey )
	{
	case MSK_CTRL:
		strcpy( str, "CTRL + " );
		break;

	case MSK_ALT:
		strcpy( str, "ALT + " );
		break;

	case MSK_SHIFT:
		strcpy( str, "SHIFT + " );
		break;
	}

	STARTCONVERT(pMacro->wKey)
	CONVERT( KEY_SNAPSHOT, str, "PRINTSCREEN" )
	CONVERT( KEY_ESCAPE, str, "ESC" )
	CONVERT( KEY_0, str, "0" )
	CONVERT( KEY_1, str, "1" )
	CONVERT( KEY_2, str, "2" )
	CONVERT( KEY_3, str, "3" )
	CONVERT( KEY_4, str, "4" )
	CONVERT( KEY_5, str, "5" )
	CONVERT( KEY_6, str, "6" )
	CONVERT( KEY_7, str, "7" )
	CONVERT( KEY_8, str, "8" )
	CONVERT( KEY_9, str, "9" )
	CONVERT( KEY_A, str, "A" )
	CONVERT( KEY_B, str, "B" )
	CONVERT( KEY_C, str, "C" )
	CONVERT( KEY_D, str, "D" )
	CONVERT( KEY_E, str, "E" )
	CONVERT( KEY_F, str, "F" )
	CONVERT( KEY_G, str, "G" )
	CONVERT( KEY_H, str, "H" )
	CONVERT( KEY_I, str, "I" )
	CONVERT( KEY_J, str, "J" )
	CONVERT( KEY_K, str, "K" )
	CONVERT( KEY_L, str, "L" )
	CONVERT( KEY_M, str, "M" )
	CONVERT( KEY_N, str, "N" )
	CONVERT( KEY_O, str, "O" )
	CONVERT( KEY_P, str, "P" )
	CONVERT( KEY_Q, str, "Q" )
	CONVERT( KEY_R, str, "R" )
	CONVERT( KEY_S, str, "S" )
	CONVERT( KEY_T, str, "T" )
	CONVERT( KEY_U, str, "U" )
	CONVERT( KEY_V, str, "V" )
	CONVERT( KEY_W, str, "W" )
	CONVERT( KEY_X, str, "X" )
	CONVERT( KEY_Y, str, "Y" )
	CONVERT( KEY_Z, str, "Z" )
	CONVERT( KEY_F1, str, "F1" )
	CONVERT( KEY_F2, str, "F2" )
	CONVERT( KEY_F3, str, "F3" )
	CONVERT( KEY_F4, str, "F4" )
	CONVERT( KEY_F5, str, "F5" )
	CONVERT( KEY_F6, str, "F6" )
	CONVERT( KEY_F7, str, "F7" )
	CONVERT( KEY_F8, str, "F8" )
	CONVERT( KEY_F9, str, "F9" )
	CONVERT( KEY_F10, str, "F10" )
	CONVERT( KEY_F11, str, "F11" )
	CONVERT( KEY_F12, str, "F12" )
	CONVERT( KEY_SEMICOLON, str, ":" )
	CONVERT( KEY_SLASH, str, "/" )
	CONVERT( KEY_OEM_3, str, "~" )
	CONVERT( KEY_LBRACKET, str, "[" )
	CONVERT( KEY_BACKSLASH, str, "\\" )
	CONVERT( KEY_RBRACKET, str, "]" )
	CONVERT( KEY_APOSTROPHE, str, "'" )
	CONVERT( KEY_MINUS, str, "-" )
	CONVERT( KEY_EQUALS, str, "=" )
	CONVERT( KEY_COMMA, str, "," )
	CONVERT( KEY_PERIOD, str, "." )
	CONVERT( KEY_SPACE, str, "SPACE" )
	CONVERT( KEY_BACK, str, "BACK" )
	CONVERT( KEY_INSERT, str, "INS" )
	CONVERT( KEY_DELETE, str, "DEL" )
	CONVERT( KEY_HOME, str, "HOME" )
	CONVERT( KEY_END, str, "END" )
	CONVERT( KEY_PGUP, str, "PGUP" )
	CONVERT( KEY_PGDN, str, "PGDN" )
	CONVERT( KEY_TAB, str, "TAB" )
	CONVERT( KEY_UP, str, "UP" )
	CONVERT( KEY_DOWN, str, "DN" )
	CONVERT( KEY_LEFT, str, "LEFT" )
	CONVERT( KEY_RIGHT, str, "RIGHT" )
	CONVERT( KEY_PAD0, str, "PAD0" )
	CONVERT( KEY_PAD1, str, "PAD1" )
	CONVERT( KEY_PAD2, str, "PAD2" )
	CONVERT( KEY_PAD3, str, "PAD3" )
	CONVERT( KEY_PAD4, str, "PAD4" )
	CONVERT( KEY_PAD5, str, "PAD5" )
	CONVERT( KEY_PAD6, str, "PAD6" )
	CONVERT( KEY_PAD7, str, "PAD7" )
	CONVERT( KEY_PAD8, str, "PAD8" )
	CONVERT( KEY_PAD9, str, "PAD9" )
	CONVERT( KEY_MULTIPLY, str, "PAD*" )
	CONVERT( KEY_ADD, str, "PAD+" )
	CONVERT( KEY_SUBTRACT, str, "PAD-" )
	CONVERT( KEY_DECIMAL, str, "PAD." )
	CONVERT( KEY_DIVIDE, str, "PAD/" )
	CONVERT( KEY_CAPITAL, str, "CAPS" )
	CONVERTCOPY( KEY_SHIFT, str, "SHIFT" )
	CONVERTCOPY( KEY_MENU, str, "ALT" )
	CONVERTCOPY( KEY_CONTROL, str, "CTRL" )
	ENDCONVERT	

}

BOOL CMacroDialog::IsAlreadySet( sMACRO* pMacro )
{
	for( int i = 0 ; i < ME_COUNT ; ++i )
	{
		if( pMacro->nSysKey == m_MacroKey[i].nSysKey
			&& pMacro->wKey == m_MacroKey[i].wKey )
		{
			return TRUE;
		}
	}
	return FALSE;
}