#include "stdafx.h"
#include "ExchangeManager.h"
#include "Item.h"
#include "GameIn.h"
#include "ExchangeItem.h"
#include "./interface/cWindowManager.h"
#include "WindowIdEnum.h"
#include "cMsgBox.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "ItemManager.h"
#include "MoveManager.h"
#include "VimuManager.h"

#include "ExchangeDialog.h"
#include "InventoryExDialog.h"
#include "MainBarDialog.h"
#include "ObjectStateManager.h"
#include "QuickManager.h"
#include "PetManager.h"
#include "TitanManager.h"

#include "Battle.h"
#include "BattleSystem_Client.h"

#include "MHMap.h"

GLOBALTON(CExchangeManager)

CExchangeManager::CExchangeManager()
{
	m_IconIndexCreator.Init(MAX_ITEMICON_NUM, IG_EXCHANGEITEM_STARTINDEX);
	
	m_nLinkedItemNum	= 0;
	m_bLockSyncing		= FALSE;
	m_bExchangeSyncing	= FALSE;
	m_bInventoryActive	= FALSE;
//	m_nExchangeState	= eEXSTT_NONE;
}

CExchangeManager::~CExchangeManager()
{
	m_IconIndexCreator.Release();
}

void CExchangeManager::Init()	//exchangedialog에서 active false 일때 호출한다. CONFIRM
{
	m_nLinkedItemNum	= 0;
	m_bLockSyncing		= FALSE;
	m_bExchangeSyncing	= FALSE;
	m_IsExchanging		= FALSE;
}

void CExchangeManager::ExchangeStart()
{
//---교환신청상태로 만들기
	HERO->DisableAutoAttack();					//자동 공격 취소
	HERO->SetNextAction(NULL);					//스킬 취소
	MOVEMGR->HeroMoveStop();					//제자리 멈춤 메세지
	OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_Exchange);

	m_IsExchanging = TRUE;
//------------------------
}

void CExchangeManager::ExchangeEnd()
{
	if( HERO->GetState() == eObjectState_Exchange )
		OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Exchange);

	m_IsExchanging = FALSE;
}

BOOL CExchangeManager::ApplyExchange()
{
#ifdef _TESTCLIENT_
	return FALSE;
#endif
//---교환신청 가능상태인지 체크
	if( m_IsExchanging  )
	{
		//"이미 교환 신청중 입니다."
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(335) );
		return FALSE;
	}

	if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Move &&
		HERO->GetState() != eObjectState_Immortal )
	{
		//"교환 할 수 없는 상태입니다."
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(336) );
		return FALSE;
	}

	if( VIMUMGR->IsVimuing() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(336) );
		return FALSE;
	}

	if( HERO->InTitan() )
	{
		TITANMGR->CheckNotifyMsg(TNM_CANNOT_DO);
		return FALSE;
	}

	//SW061019 서바이벌모드 관련
	//if( MAP->CheckMapKindIs(eSurvival) )
	if( MAP->IsMapKind(eSurvivalMap) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(336) );
		return FALSE;
	}

	CObject* pOpPlayer = OBJECTMGR->GetSelectedObject();
	if( pOpPlayer == NULL ) 
	{
		//"교환을 하시려면 먼저 원하는 상대를 선택해 주세요."
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(338) );
		return FALSE;
	}

	if( pOpPlayer->GetObjectKind() != eObjectKind_Player || pOpPlayer == HERO ) 
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(338) );
		return FALSE;
	}

	////////////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 은신 / 혜안
	// 은신 패널티
	if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
		return FALSE;
	}
	if( pOpPlayer->GetSingleSpecialState(eSingleSpecialState_Hide) )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
		return FALSE;
	}
	////////////////////////////////////////////////////////////////////////////////

	// Guild Tournament
	CBattle* pBattle = BATTLESYSTEM->GetBattle( HERO );
	if( pBattle && pBattle->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT )
	if( HERO->GetBattleTeam() == 2 )
		return FALSE;

//------------------------------	
	
	ExchangeStart();

//---상대에게 교환신청 메세지 보내기
	MSG_DWORD msgApply;
	msgApply.Category	= MP_EXCHANGE;
	msgApply.Protocol	= MP_EXCHANGE_APPLY_SYN;
	msgApply.dwObjectID	= HERO->GetID();
	msgApply.dwData		= pOpPlayer->GetID();
	NETWORK->Send( &msgApply, sizeof( msgApply ) );
//-----------------------------------

	return TRUE;
}

BOOL CExchangeManager::CanAcceptExchange( CPlayer* pAccepter )
{
	if( pAccepter == NULL )					return FALSE;
	if( VIMUMGR->IsVimuing() )				return FALSE;

	if( pAccepter->GetState() != eObjectState_None && pAccepter->GetState() != eObjectState_Move
		&& pAccepter->GetState() != eObjectState_Immortal )
		return FALSE;
	
	return TRUE;	
}

void CExchangeManager::AcceptExchange( BOOL bAccept )
{
	if( bAccept )
	{
		if( CanAcceptExchange( HERO ) )
		{
			ExchangeStart();	//nack에서 해제확인
			
			MSGBASE msgAccept;
			msgAccept.Category		= MP_EXCHANGE;
			msgAccept.dwObjectID	= HERO->GetID();
			msgAccept.Protocol		= MP_EXCHANGE_ACCEPT_SYN;
			NETWORK->Send( &msgAccept, sizeof( msgAccept ) );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(334) );//교환을 수락하셨습니다.

		}
		else
		{
			MSGBASE msgError;		//차후 에러체크를 추가하자.
			msgError.Category		= MP_EXCHANGE;
			msgError.dwObjectID		= HERO->GetID();
			msgError.Protocol		= MP_EXCHANGE_CANTAPPLY;
			NETWORK->Send( &msgError, sizeof(MSGBASE) );

			m_IsExchanging			 = FALSE;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(336) );//교환할 수 없는상태입니다.
		}
	}
	else
	{
		MSGBASE msgAccept;
		msgAccept.Category		= MP_EXCHANGE;
		msgAccept.dwObjectID	= HERO->GetID();
		msgAccept.Protocol		= MP_EXCHANGE_REJECT_SYN;
		NETWORK->Send( &msgAccept, sizeof( msgAccept ) );

		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(327) );

		ExchangeEnd();
	}	
}

void CExchangeManager::CancelApply()
{
	MSGBASE msg;

	msg.Category	= MP_EXCHANGE;
	msg.Protocol	= MP_EXCHANGE_WAITING_CANCEL_SYN;
	msg.dwObjectID	= HERO->GetID();

	NETWORK->Send( &msg, sizeof(MSGBASE) );
}

BOOL CExchangeManager::LinkItem( CItem* pItem )
{
	if( pItem->GetType() != WT_ITEM ) return FALSE;		//인벤토리 아이템이 아니면 return
	//장착되어 있는 아이템은....안되는것 추가..		//confirm

	if( pItem->GetLinkItem() != NULL ) return FALSE;		//이미 링크된 아이템이면 return
	//링크가 되었는지 안되었는지만 알면 되지 않을까?

	if( m_nLinkedItemNum >= MAX_EXCHANGEITEM ) return FALSE;

	++m_nLinkedItemNum;
	
	pItem->SetLock( TRUE );


	MSG_LINKITEM msg;

	msg.Category	= MP_EXCHANGE;
	msg.Protocol	= MP_EXCHANGE_ADDITEM_SYN;
	msg.dwObjectID	= HERO->GetID();
	msg.ItemInfo	= *pItem->GetItemBaseInfo();
	msg.wAbsPosition = m_nLinkedItemNum;

	NETWORK->Send( &msg, sizeof( MSG_LINKITEM ) );

	return TRUE;
}

void CExchangeManager::UnLinkItem( int nTypeIndex, cIcon* pItem )
{
	if( nTypeIndex == 0 )
	{
		((CExchangeItem*)pItem)->GetLinkItem()->SetLinkPosition( 0 );
		((CExchangeItem*)pItem)->GetLinkItem()->SetLinkItem( NULL );
		((CExchangeItem*)pItem)->GetLinkItem()->SetLock( FALSE );		
	}

	m_IconIndexCreator.ReleaseIndex( pItem->GetID() );
	WINDOWMGR->AddListDestroyWindow( pItem );
}

void CExchangeManager::Lock( BOOL bLock )
{
	MSGBASE msg;
	msg.Category	= MP_EXCHANGE;
	msg.dwObjectID	= HERO->GetID();

	if( bLock )
	{
		msg.Protocol = MP_EXCHANGE_LOCK_SYN;
	}
	else
	{
		msg.Protocol = MP_EXCHANGE_UNLOCK_SYN;
	}

	NETWORK->Send( &msg, sizeof( MSGBASE ) );

	m_bLockSyncing = TRUE;
}


void CExchangeManager::Exchange()
{
	MSGBASE msg;
	msg.Category	= MP_EXCHANGE;
	msg.dwObjectID	= HERO->GetID();
	msg.Protocol	= MP_EXCHANGE_EXCHANGE_SYN;

	NETWORK->Send( &msg, sizeof( MSGBASE ) );
	
	//KES 040801 교환중에 인벤토리 사용 막기
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);

	m_bExchangeSyncing = TRUE;
}


void CExchangeManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{
	case MP_EXCHANGE_APPLY:	//상대가 나에게 신청을 해왔다
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( msg->dwData );
			
			ASSERT(pPlayer->GetObjectKind() == eObjectKind_Player);

			if( pPlayer == NULL || m_IsExchanging == TRUE )
			{
				MSGBASE msgReject;
				msgReject.Category		= MP_EXCHANGE;
				msgReject.dwObjectID	= HERO->GetID();
				msgReject.Protocol		= MP_EXCHANGE_CANTAPPLY;
				NETWORK->Send( &msgReject, sizeof(MSGBASE) );

				//에러메세지
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(336) );	//교환할 수 없는상태입니다.
				break;
			}

			WINDOWMGR->MsgBox( MBI_EXCHANGEAPPLY, MBT_YESNO,
						CHATMGR->GetChatMsg(321), pPlayer->GetObjectName() );

			if( GAMEIN->GetExchangeDialog() )
				GAMEIN->GetExchangeDialog()->SetPlayerName( HERO->GetObjectName(), pPlayer->GetObjectName() );

			m_IsExchanging = TRUE;
		}
		break;

	case MP_EXCHANGE_APPLY_ACK:
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( msg->dwData );
			if( pPlayer == NULL )	//CHECK 교환상대를 찾을 수 없다?
			{
				//교환 신청을 수행하지 못하였습니다.
				ExchangeEnd();
				break;
			}
			ASSERT(pPlayer->GetObjectKind() == eObjectKind_Player);

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(322), pPlayer->GetObjectName() );

			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_EXCHANGEWAITING, MBT_CANCEL, CHATMGR->GetChatMsg(337) );
			if( pMsgBox )
				pMsgBox->SetDefaultBtn(-1);	//디폴트 버튼 없다.
		}
		break;

	case MP_EXCHANGE_APPLY_NACK:
		{			
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(323) );
			ExchangeEnd();
		}
		break;

	case MP_EXCHANGE_ACCEPT_NACK:
		{
			MSGBASE* msg = (MSGBASE*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(326) );			
			ExchangeEnd();
		}
		break;

	case MP_EXCHANGE_ACCEPT:
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( msg->dwData );
			if( pPlayer == NULL ) return;

			ASSERT(pPlayer->GetObjectKind() == eObjectKind_Player);

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(324), pPlayer->GetObjectName() );

			if( GAMEIN->GetExchangeDialog() )
				GAMEIN->GetExchangeDialog()->SetPlayerName( HERO->GetObjectName(), pPlayer->GetObjectName() );

			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_EXCHANGEWAITING );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );
		}
		break;

	case MP_EXCHANGE_REJECT:
		{
			MSGBASE* msg = (MSGBASE*)pMsg;

			ExchangeEnd();
			
			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_EXCHANGEWAITING );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(326) );
		}
		break;

	case MP_EXCHANGE_CANTAPPLY:
		{
			MSGBASE* msg = (MSGBASE*)pMsg;

			ExchangeEnd();

			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_EXCHANGEWAITING );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(323) );
			//315 거리가 너무 멀어 교환할 수 없습니다.
		}
		break;

	case MP_EXCHANGE_WAITING_CANCEL:
		{
			ExchangeEnd();
			
			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_EXCHANGEAPPLY );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );
			
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(326) );
		}
		break;

	case MP_EXCHANGE_WAITING_CANCEL_ACK:
		{
			ExchangeEnd();

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(329) );
		}
		break;

	case MP_EXCHANGE_WAITING_CANCEL_NACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(316) );
		}
		break;	

	case MP_EXCHANGE_START:
		{
			GAMEIN->GetExchangeDialog()->SetActive( TRUE );

			if( !GAMEIN->GetInventoryDialog()->IsActive() )
			{
				GAMEIN->GetInventoryDialog()->SetActive( TRUE );
				m_bInventoryActive = FALSE;
			}
			else
			{
				m_bInventoryActive = TRUE;
			}
		}
		break;

	case MP_EXCHANGE_ADDITEM_ACK:
		{
			MSG_LINKITEM* msg = (MSG_LINKITEM*)pMsg;

			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );		//FAKE MOVE에서 TRUE
			CItem* pItem = (CItem*)GAMEIN->GetInventoryDialog()->GetItemForPos( msg->ItemInfo.Position );
			ASSERT( pItem );
			if( !pItem ) break;

			CExchangeItem* pExItem = new CExchangeItem;
								
			//Init바꾸는게 낫겠다.
			pExItem->SetDBId(msg->ItemInfo.dwDBIdx);
			pExItem->InitItem( pItem, msg->wAbsPosition, m_IconIndexCreator.GenerateIndex() );
			pExItem->SetType( WT_EXCHANGEITEM );
			pExItem->SetDurability( msg->ItemInfo.Durability );
			pExItem->SetRareness( msg->ItemInfo.RareIdx );

			WINDOWMGR->AddWindow( pExItem );

			GAMEIN->GetExchangeDialog()->AddItem( 0, msg->wAbsPosition, pExItem );

			//인벤토리 아이템 세팅
			pItem->SetLinkPosition( m_nLinkedItemNum );
			pItem->SetLinkItem( pExItem );

			//인벤토리 체크
			int nBlankNum = GAMEIN->GetInventoryDialog()->GetBlankNum();

			if( nBlankNum < (int)msg->wAbsPosition - EXCHANGEMGR->GetLinkedItemNum() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(354) );
		//		m_bExchangable = FALSE;
			}
		}
		break;

	case MP_EXCHANGE_ADDITEM_NACK:
		{
			MSG_LINKITEM* msg = (MSG_LINKITEM*)pMsg;
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );		//FAKE MOVE에서 TRUE

			--m_nLinkedItemNum;

			CItem* pItem = GAMEIN->GetInventoryDialog()->GetItemForPos( msg->ItemInfo.Position );
			if( pItem )
				pItem->SetLock( FALSE );			
		}
		break;

	case MP_EXCHANGE_ADDITEM:	//상대가 등록했다.
		{
//			MSG_LINKITEM* msg = (MSG_LINKITEM*)pMsg;
			//MSG_LINKITEM_ADD* msg = (MSG_LINKITEM_ADD*)pMsg;
			MSG_LINKITEMOPTIONS* msg = (MSG_LINKITEMOPTIONS*)pMsg;

			CExchangeItem* pItem = new CExchangeItem;
			cImage tmpImage;
			pItem->Init( 0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, 
						 ITEMMGR->GetIconImage( msg->ItemInfo.wIconIdx, &tmpImage ), m_IconIndexCreator.GenerateIndex() );

			pItem->SetType( WT_EXCHANGEITEM );
			pItem->SetDurability( msg->ItemInfo.Durability );
			pItem->SetRareness( msg->ItemInfo.RareIdx );
			pItem->SetMovable( FALSE );
			pItem->SetData( msg->ItemInfo.wIconIdx );
			pItem->SetItemParam( msg->ItemInfo.ItemParam );
			pItem->SetDBId( msg->ItemInfo.dwDBIdx );	//SW051129 PET 친밀도 툴팁

			/*
			//SW050920 Rare
			//옵션이 있으면 등록
			if( msg->bOptionItem ) ITEMMGR->AddItemOption( &msg->sOptionInfo );
			if( msg->bRareOptionItem ) ITEMMGR->AddItemRareOption( &msg->sRareOptionInfo );
			if( msg->bPetInfo )	PETMGR->AddPetInfo( &msg->sPetTotalInfo );

			if( msg->bOptionItem & msg->bRareOptionItem )
			{
				ITEMMGR->SetToolTipIcon( pItem, &msg->sOptionInfo, &msg->sRareOptionInfo );
			}
			else if( msg->bOptionItem )
			{
				ITEMMGR->SetToolTipIcon( pItem, &msg->sOptionInfo, NULL );
			}
			else if( msg->bRareOptionItem )
			{
				ITEMMGR->SetToolTipIcon( pItem, NULL, &msg->sRareOptionInfo );
			}
			else if( msg->bPetInfo )
			{
				ITEMMGR->SetToolTipIcon( pItem, NULL, NULL, msg->ItemInfo.dwDBIdx );
			}
			else
			{
				ITEMMGR->SetToolTipIcon( pItem );
			}*/
			//SW070308 아이템부가옵션들 정리
			ITEMMGR->GetItemOptionsAndToolTipFromInfoMsg(pItem, msg);

			WINDOWMGR->AddWindow( pItem );

			GAMEIN->GetExchangeDialog()->AddItem( 1, msg->wAbsPosition, pItem );
		}
		break;
		
	case MP_EXCHANGE_DELITEM_ACK:
		{
			MSG_WORD* msg = (MSG_WORD*)pMsg;
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
			GAMEIN->GetExchangeDialog()->DelItem( 0, msg->wData );
			--m_nLinkedItemNum;
		}
		break;

	case MP_EXCHANGE_DELITEM_NACK:
		{
			MSG_WORD* msg = (MSG_WORD*)pMsg;
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
		}
		break;

	case MP_EXCHANGE_DELITEM://상대가 지웠다.
		{
			MSG_WORD* msg = (MSG_WORD*)pMsg;

			//옵션이 있으면 지워준다.
			GAMEIN->GetExchangeDialog()->DelItemOptionInfo( msg->wData );
			GAMEIN->GetExchangeDialog()->DelItemRareOptionInfo( msg->wData );
			GAMEIN->GetExchangeDialog()->DelPetItemInfo( msg->wData );

			GAMEIN->GetExchangeDialog()->DelItem( 1, msg->wData );			
			
		}
		break;
		
	case MP_EXCHANGE_INPUTMONEY_ACK:
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			DWORD dwMoney = GAMEIN->GetExchangeDialog()->InputMoney( 0, msg->dwData );
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
		}
		break;

	case MP_EXCHANGE_INPUTMONEY_NACK:
		{
			//"돈입력에 실패하였습니다."
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
		}
		break;

	case MP_EXCHANGE_INPUTMONEY:
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			GAMEIN->GetExchangeDialog()->InputMoney( 1, msg->dwData );
		}
		break;

	case MP_EXCHANGE_LOCK_ACK:
		{
			m_bLockSyncing = FALSE;
			GAMEIN->GetExchangeDialog()->MyLock( TRUE );	//이미 했으나 한번더 확인
		}
		break;

	case MP_EXCHANGE_LOCK_NACK:
		{
			m_bLockSyncing = FALSE;
			GAMEIN->GetExchangeDialog()->MyLock( FALSE );
		}
		break;

	case MP_EXCHANGE_LOCK:
		{
			GAMEIN->GetExchangeDialog()->OpLock( TRUE );			
		}
		break;

	case MP_EXCHANGE_UNLOCK_ACK:
		{
			m_bLockSyncing = FALSE;			
			GAMEIN->GetExchangeDialog()->MyLock( FALSE );
		}
		break;

	case MP_EXCHANGE_UNLOCK_NACK:
		{
			m_bLockSyncing = FALSE;
			GAMEIN->GetExchangeDialog()->MyLock( TRUE );
		}
		break;

	case MP_EXCHANGE_UNLOCK:	//상대가 언락했을시에.
		{
			GAMEIN->GetExchangeDialog()->OpLock( FALSE );
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(332) );

			if( m_bExchangeSyncing )//교환중이었다면 
			{
				m_bExchangeSyncing = FALSE;
				//인베토리디스에이블 풀기
				GAMEIN->GetInventoryDialog()->SetDisable( FALSE );				
			}			
		}
		break;

	case MP_EXCHANGE_EXCHANGE_ACK:
		{
			//둘다 exchange를 눌렀다. 교환을 시작한다!
			//여기서 옵션을 다지우고...
		}
		break;

	case MP_EXCHANGE_EXCHANGE:
		{
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
			GAMEIN->GetExchangeDialog()->SetActive( FALSE );			
			ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );

			//돈입력버튼 지우기.	CONFIRM
			cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_EXCHANGE_MONEY );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );


			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(325) );

			ExchangeEnd();
		}
		break;

	case MP_EXCHANGE_EXCHANGE_NACK:		//교환버튼을 눌렀는대 상대가 락을풀어 락이 해제되었을때 NACK다.
		{
//			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
//			GAMEIN->GetExchangeDialog()->SetActive( FALSE );
//			GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
//			if( !m_bInventoryActive )
//				GAMEIN->GetInventoryDialog()->SetActive( FALSE );

			//돈입력버튼 지우기.	CONFIRM
//			cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_EXCHANGE_MONEY );
//			if( pDlg )
//				WINDOWMGR->AddListDestroyWindow( pDlg );

//			CHATMGR->AddMsg( CTC_SYSMSG, "락이 걸리지 않은 상태라 교환이 취소되었습니다." );
//			ExchangeEnd();
			ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );
		}
		break;

	case MP_EXCHANGE_CANCEL_ACK:
		{
			////////아이템 옵션 제거 Active False 전에 할것
			GAMEIN->GetExchangeDialog()->DelItemOptionInfoAll();
			GAMEIN->GetExchangeDialog()->DelItemRareOptionInfoAll();
			GAMEIN->GetExchangeDialog()->DelPetItemInfoAll();

			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
			GAMEIN->GetExchangeDialog()->SetActive( FALSE );

			if( !m_bInventoryActive )
			{
				ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );
				GAMEIN->GetInventoryDialog()->SetActive( FALSE );
			}

			cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_EXCHANGE_MONEY );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(329) );

			ExchangeEnd();
		}
		break;
	
	case MP_EXCHANGE_CANCEL_NACK:
		{
			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );
		}
		break;

	case MP_EXCHANGE_CANCEL:
		{
			MSG_BYTE* msg = (MSG_BYTE*)pMsg;

			GAMEIN->GetExchangeDialog()->SetDisable( FALSE );	//체크..임시.
			GAMEIN->GetInventoryDialog()->SetDisable( FALSE );

			//돈입력버튼 지우기.	CONFIRM
			cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_EXCHANGE_MONEY );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );
			

			////////아이템 옵션 제거 : Active False 전에 할것.
			GAMEIN->GetExchangeDialog()->DelItemOptionInfoAll();
			GAMEIN->GetExchangeDialog()->DelItemRareOptionInfoAll();

			//교환이 취소되었때와 교환허락을 하지 않았을시...
			GAMEIN->GetExchangeDialog()->SetActive( FALSE );
			if( !m_bInventoryActive )
				GAMEIN->GetInventoryDialog()->SetActive( FALSE );
			
			//신청메세지들 지우기....
			
			pDlg = WINDOWMGR->GetWindowForID( MBI_EXCHANGEAPPLY);
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );

			pDlg = WINDOWMGR->GetWindowForID( MBI_EXCHANGEWAITING );
			if( pDlg )
				WINDOWMGR->AddListDestroyWindow( pDlg );			

//error msg 출력
			switch( msg->bData )
			{
				case eEE_USERCANCEL:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(326) );
					break;
				case eEE_USERLOGOUT:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(314) );
					break;
				case eEE_USERDIE:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(313) );
					break;
				case eEE_DIE:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(312) );
					break;
				case eEE_NOTENOUGHMONEY:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(311) );
					break;
				case eEE_NOTENOUGHSPACE:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(310) );
					break;
				case eEE_MAXMONEY:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(309) );
					break;
				default:
					break;
			}

			ExchangeEnd();
		}
		break;

	case MP_EXCHANGE_SETMONEY:
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			HERO->SetMoney( msg->dwData );	//+인벤토리
		}
		break;
		
	case MP_EXCHANGE_INSERT:
		{
			MSG_ITEM* msg = (MSG_ITEM*)pMsg;
			BOOL rt = GAMEIN->GetInventoryDialog()->AddItem(&msg->ItemInfo);
			ASSERT(rt);
			//아이템을 넣을때 다시 옵션을 넣는다.?

			QUICKMGR->RefreshQickItem();
		}
		break;

	case MP_EXCHANGE_REMOVE:
		{
			MSG_EXCHANGE_REMOVEITEM* msg = (MSG_EXCHANGE_REMOVEITEM*)pMsg;
			CItem* pOutItem = NULL;
			ITEMMGR->DeleteItem( msg->wAbsPosition, &pOutItem );

			//옵션을 가진 아이템이었다면 옵션이 지워진다. 자동으로.
		}
		break;
	}
}
