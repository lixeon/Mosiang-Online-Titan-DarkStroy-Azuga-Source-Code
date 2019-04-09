// GameState.cpp: implementation of the CGameState class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameState.h"
#include "MainGame.h"
#include "..\interface\cwindowmanager.h"
#include "windowidenum.h"
#include "cmsgbox.h"
#include "chatmanager.h"
#include "CharSelect.h"

#ifdef _HACK_SHIELD_
#include "HackShieldManager.h"
#endif

#ifdef _NPROTECT_
#include "NProtectManager.h"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern HWND _g_hWnd;


CGameState::CGameState()
{
	m_bBlockScreen	= FALSE;
	m_bDisconnected = FALSE;
}

CGameState::~CGameState()
{

}
BOOL CGameState::CommonNetworkParser(BYTE Category,BYTE Protocol,void* pMsg)
{
	BOOL rt = FALSE;
	switch(Category) 
	{
	case MP_USERCONN:
		{
			switch(Protocol) 
			{
			case MP_USERCONN_OTHERUSER_CONNECTTRY_NOTIFY:
				{
					WINDOWMGR->MsgBox( MBI_OTHERUSER_CONNECTTRY, MBT_OK, CHATMGR->GetChatMsg(2) );
					rt = TRUE;
				}
				break;
			case MP_USERCONN_CONNECTION_CHECK:
				{
					MSGBASE msg;
					msg.Category = MP_USERCONN;
					msg.Protocol = MP_USERCONN_CONNECTION_CHECK_OK;
					NETWORK->Send(&msg,sizeof(msg));
					rt = TRUE;
				}
				break;
			case MP_USERCONN_DISCONNECTED_BY_OVERLAPLOGIN:
				{
					WINDOWMGR->MsgBox( MBI_DISCONNECTED_OVERLAPPEDLOGIN, 
						MBT_OK, CHATMGR->GetChatMsg(3) );
					rt = TRUE;
				}
				break;
			case MP_USERCONN_CHARACTERSLOT:
				{
					MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

#ifdef _JAPAN_LOCAL_
					CHARSELECT->SetExtraCharacterSlot( pmsg->dwData );					
#endif
#ifdef _HK_LOCAL_
					CHARSELECT->SetExtraCharacterSlot( pmsg->dwData );					
#endif
#ifdef _TL_LOCAL_
					CHARSELECT->SetExtraCharacterSlot( pmsg->dwData );					
#endif

					rt = TRUE;
				}
				break;			
			}
		}
		break;
#ifdef _HACK_SHIELD_
	case MP_HACKSHIELD:
		{
			HACKSHIELDMGR->NetworkMsgParse(Protocol, pMsg);
			rt = TRUE;
		}
		break;
#endif
#ifdef _NPROTECT_
	case MP_NPROTECT:
		{
			NPROTECTMGR->NetworkMsgParse(Protocol, pMsg);
			rt = TRUE;
		}
		break;
#endif
	}

	return rt;
}

void CGameState::SetBlock(BOOL val)
{
	m_bBlockScreen = val;
}

void CGameState::OnDisconnect()
{
	m_bDisconnected = TRUE;
	if( cMsgBox::IsInit() )	//게임정지필요? confirm
	{
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_SERVERDISCONNECT, MBT_OK, CHATMGR->GetChatMsg( 448 ) );
	}
	else
	{
		MessageBox( NULL, "Disconnected To Server.\nThe Game is Closing.", 0, 0 );
//		MAINGAME->SetGameState(eGAMESTATE_END);
		PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
	}
}
