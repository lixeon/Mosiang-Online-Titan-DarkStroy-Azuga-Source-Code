#include "stdafx.h"
#include "VimuManager.h"
#include "./interface/cWindowManager.h"
#include "WindowIDEnum.h"
#include "cMsgBox.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "MoveManager.h"
#include "GameIn.h"
#include "MainBarDialog.h"
#include "objectstatemanager.h"

#include "BattleSystem_Client.h"

GLOBALTON(CVimuManager);

CVimuManager::CVimuManager()
{
	m_bIsVimuing = FALSE;
}

CVimuManager::~CVimuManager()
{

}

void CVimuManager::Init()
{
	m_bIsVimuing = FALSE;
}

void CVimuManager::ApplyVimu()
{
	if( BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() != eBATTLE_KIND_NONE || m_bIsVimuing )
	{
		//ºñ¹«¸¦ ÇÒ ¼ö ¾ø´Â »óÅÂÀÔ´Ï´Ù. confirm
		return;
	}
	if( HERO->IsPKMode() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(370) );
		return;
	}
	if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Move &&
		HERO->GetState() != eObjectState_Immortal )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(370) );
		return;
	}

	CObject* pOpPlayer = OBJECTMGR->GetSelectedObject();

	if( pOpPlayer == NULL || pOpPlayer == HERO )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(380) );
		return;
	}
	if( pOpPlayer->GetObjectKind() != eObjectKind_Player )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(380) );
		return;
	}
	if( pOpPlayer->GetState() == eObjectState_Die )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(368) );
		return;
	}
	////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 은신/혜안
	// 은신 패널티
	if( pOpPlayer->GetSingleSpecialState(eSingleSpecialState_Hide) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1329) );
		return;
	}
	////////////////////////////////////////////////////////////////////////

//	BYTE cbOpState = pOpPlayer->GetState();
//	if( cbOpState == eObjectState_Deal || cbOpState == eObjectState_Exchange ||
//		cbOpState == eObjectState_StreetStall_Owner || cbOpState == eObjectState_StreetStall_Guest )
//	{
		//»ó´ë°¡ ºñ¹«¸¦ ÇÒ ¼ö ¾ø´Â »óÅÂÀÔ´Ï´Ù. confirm
//		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 381 ) );
//		return;
//	}


	SetVimuing( TRUE );
	HERO->DisableAutoAttack();			//°ø°Ý Ãë¼Ò
	HERO->SetNextAction(NULL);			//½ºÅ³ Ãë¼Ò
	MOVEMGR->HeroMoveStop();
//	HERO->SetState( eObjectState_BattleReady );
	OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_BattleReady);
	

	
//---NETWORK MSG	
	MSG_DWORD msg;
	msg.Category	= MP_BATTLE;
	msg.Protocol	= MP_BATTLE_VIMU_APPLY_SYN;
	msg.dwObjectID	= HERO->GetID();
	msg.dwData		= pOpPlayer->GetID();
	NETWORK->Send( &msg, sizeof(MSG_DWORD) );
}

void CVimuManager::CancelApply()
{
	MSGBASE msg;

	msg.Category	= MP_BATTLE;
	msg.Protocol	= MP_BATTLE_VIMU_WAITING_CANCEL_SYN;
	msg.dwObjectID	= HERO->GetID();

	NETWORK->Send( &msg, sizeof(MSGBASE) );	
}

void CVimuManager::AcceptVimu( BOOL bAccept )
{
	if( bAccept )
	{
		if( CanAcceptVimu( HERO ) )
		{
			HERO->DisableAutoAttack();					//ÀÚµ¿ °ø°Ý Ãë¼Ò
			HERO->SetNextAction(NULL);					//½ºÅ³ Ãë¼Ò
			MOVEMGR->HeroMoveStop();
//			HERO->SetState( eObjectState_BattleReady );	//ÇØÁ¦È®ÀÎ
			OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_BattleReady);


			MSGBASE msg;
			msg.Category	= MP_BATTLE;
			msg.Protocol	= MP_BATTLE_VIMU_ACCEPT_SYN;
			msg.dwObjectID	= HERO->GetID();
			NETWORK->Send( &msg, sizeof(MSGBASE) );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(378) );
		}
		else
		{
			MSG_BYTE msg;
			msg.Category	= MP_BATTLE;
			msg.Protocol	= MP_BATTLE_VIMU_ERROR;
			msg.dwObjectID	= HERO->GetID();
			msg.bData		= 1;	//AcceptÇÒ¼ö ¾ø´Â »óÅÂ
			NETWORK->Send( &msg, sizeof(MSG_BYTE) );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(370) );
			SetVimuing( FALSE );
		}
	}
	else
	{
		MSGBASE msg;
		msg.Category	= MP_BATTLE;
		msg.Protocol	= MP_BATTLE_VIMU_REJECT_SYN;
		msg.dwObjectID	= HERO->GetID();
		NETWORK->Send( &msg, sizeof(MSGBASE) );
	}
}

BOOL CVimuManager::CanAcceptVimu( CPlayer* pAccepter )
{
	if( pAccepter == NULL )					return FALSE;

	if( BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() != eBATTLE_KIND_NONE )	 return FALSE;
	if( pAccepter->IsPKMode() )				return FALSE;
//	if( m_bIsVimuing == TRUE )				return FALSE; //ÇöÀç ÀÌ ÇÔ¼ö´Â HERO¸¸ ÀÎÀÚ·Î ³Ñ¾î¿Â´Ù!
	if( pAccepter->GetState() != eObjectState_None && pAccepter->GetState() != eObjectState_Move
		&& pAccepter->GetState() != eObjectState_Immortal )
		return FALSE;
	
	return TRUE;
}

void CVimuManager::SetVimuing( BOOL bVimuing )
{
	m_bIsVimuing = bVimuing;
	
//--- main bar icon
//	CMainBarDialog* pDlg = GAMEIN->GetMainInterfaceDialog();
//	if( pDlg )
//		pDlg->SetPushBarIcon( OPT_VIMUICON, m_bIsVimuing );
}

void CVimuManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_BATTLE_VIMU_APPLY:	//»ó´ë°¡ ºñ¹«¸¦ ½ÅÃ»ÇØ¿È
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			CPlayer* pApplyer = (CPlayer*)OBJECTMGR->GetObject( msg->dwData );
			if( pApplyer == NULL || m_bIsVimuing == TRUE )
			{
				MSG_BYTE msg;
				msg.Category	= MP_BATTLE;
				msg.Protocol	= MP_BATTLE_VIMU_ERROR;
				msg.dwObjectID	= HERO->GetID();
				msg.bData		= 1;	//AcceptÇÒ¼ö ¾ø´Â »óÅÂ
				NETWORK->Send( &msg, sizeof(MSG_BYTE) );
				return;
			}

			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_VIMUAPPLY, MBT_YESNO,
									CHATMGR->GetChatMsg(373), pApplyer->GetObjectName() );
			if( pMsgBox ) pMsgBox->SetDefaultBtn( -1 );
			
			SetVimuing( TRUE );
		}
		break;

	case MP_BATTLE_VIMU_APPLY_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwData );

			if( pPlayer )
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(371), pPlayer->GetObjectName() );
			
			WINDOWMGR->MsgBox( MBI_VIMUWAITING, MBT_CANCEL, CHATMGR->GetChatMsg(372) );
		}
		break;

	case MP_BATTLE_VIMU_APPLY_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwData );

			SetVimuing( FALSE );
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_BattleReady);
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(368) );
		}
		break;

	case MP_BATTLE_VIMU_ACCEPT:	//»ó´ë°¡ ¹Þ¾ÆµéÀÌ´Ù
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( msg->dwData );

			if( pPlayer )
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(374), pPlayer->GetObjectName() );

			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_VIMUWAITING );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );
		}
		break;

	case MP_BATTLE_VIMU_ACCEPT_ACK:
		{
			
		}
		break;

	case MP_BATTLE_VIMU_ACCEPT_NACK:
		{
			//MSGBASE* pmsg = (MSGBASE*)pMsg;
			//ºñ¹«½ÇÆÐ¸Þ¼¼Áö
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(370) );

			SetVimuing( FALSE );
			if( HERO->GetState() == eObjectState_BattleReady )				
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_BattleReady);
		}
		break;

	case MP_BATTLE_VIMU_ERROR:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
			if( pmsg->bData == 0 )
			{

			}
			else if( pmsg->bData == 1 )		//»ó´ë°¡ accepter°¡º¸³½ error
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(381) );
			}
			else if( pmsg->bData == 2 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(369) );
			}
			else if( pmsg->bData == 3 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(367) );
			}

			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_VIMUWAITING );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );

			SetVimuing( FALSE );
			if( HERO->GetState() == eObjectState_BattleReady )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_BattleReady);
		}
		break;

	case MP_BATTLE_VIMU_REJECT:	//»ó´ë°¡ °ÅÀýÇÏ´Ù
		{
			MSGBASE* msg = (MSGBASE*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(375) );
			
			SetVimuing( FALSE );
			if( HERO->GetState() == eObjectState_BattleReady )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_BattleReady);

			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_VIMUWAITING );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );
		}
		break;

	case MP_BATTLE_VIMU_REJECT_ACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(379) );
			SetVimuing( FALSE );
		}
		break;

	case MP_BATTLE_VIMU_REJECT_NACK:
		{

		}
		break;

	case MP_BATTLE_VIMU_WAITING_CANCEL:	//»ó´ë°¡ ½ÅÃ» Ãë¼Ò
		{
			SetVimuing( FALSE );
			if( HERO->GetState() == eObjectState_BattleReady )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_BattleReady);

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(376) );

			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_VIMUAPPLY );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );
		}
		break;

	case MP_BATTLE_VIMU_WAITING_CANCEL_ACK:
		{
			SetVimuing( FALSE );
			if( HERO->GetState() == eObjectState_BattleReady )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_BattleReady);

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(377) );
		}
		break;

	case MP_BATTLE_VIMU_WAITING_CANCEL_NACK:
		{
			//ºñ¹«°¡ ÀÌ¹Ì ½ÃÀÛµÇ¾î Ãë¼ÒÇÒ ¼ö ¾ø½À´Ï´Ù.
//			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(377) );
		}
		break;

	case MP_BATTLE_DESTROY_NOTIFY:
		{
			SetVimuing( FALSE );			
		}
		break;
	}
}