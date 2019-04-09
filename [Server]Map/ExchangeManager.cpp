#include "stdafx.h"
#include "ExchangeManager.h"
#include "ExchangeRoom.h"
#include "UserTable.h"
#include "Player.h"
#include "objectstatemanager.h"
#include "itemmanager.h"
#include "CheckRoutine.h"
#include "..\[CC]Header\GameResourceStruct.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "Battle.h"

#define MAX_EXCHANGEROOM	100


CExchangeManager::CExchangeManager()
{
	m_mpExchangeRoom = new CMemoryPoolTempl<CExchangeRoom>;
	m_mpExchangeRoom->Init( MAX_EXCHANGEROOM, 50, "ExchangeRoomPool" );
}

CExchangeManager::~CExchangeManager()
{
	PTRLISTPOS pos = m_PtrList.GetHeadPosition();

	while(pos)
	{
		CExchangeRoom* p = (CExchangeRoom*)m_PtrList.GetNext( pos );
		if( p )	delete p;
	}

	m_PtrList.RemoveAll();	

	SAFE_DELETE( m_mpExchangeRoom );
}

BOOL CExchangeManager::CanApplyExchange( CPlayer* pPlayer, CPlayer* pOpPlayer )
{
/////////


	if( pPlayer->GetState() != eObjectState_None && pPlayer->GetState() != eObjectState_Move
		&& pPlayer->GetState() != eObjectState_Immortal )
		return FALSE;
	if( pPlayer->IsVimuing() )
		return FALSE;

	if( pPlayer->InTitan() )
		return FALSE;

	//if( g_pServerSystem->CheckMapKindIs(eSurvival) )
	if( g_pServerSystem->GetMap()->IsMapKind(eSurvivalMap) )
		return FALSE;

	// RaMa - BattleCheck
	if( pPlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT ||
		pPlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
	{
		if( pPlayer->GetBattleTeam() == 2 )			// Oberver
			return FALSE;
	}
	
//	ASSERT( pPlayer->GetExchangeContainer()->pRoom == NULL );	//Confirm
	if( pPlayer->GetExchangeContainer()->pRoom != NULL )
		return FALSE;

///////////////


	if( pOpPlayer == NULL )
		return FALSE;
	else if( pOpPlayer->GetGameOption()->bNoDeal )
		return FALSE;
	else if( pOpPlayer->GetExchangeContainer()->pRoom != NULL || pOpPlayer->IsVimuing() )
		return FALSE;
	else if( pOpPlayer->GetState() != eObjectState_None && pOpPlayer->GetState() != eObjectState_Move
			 && pOpPlayer->GetState() != eObjectState_Immortal )
		return FALSE;

	return TRUE;
}

CExchangeRoom* CExchangeManager::CreateExchangeRoom( CPlayer* p1, CPlayer* p2 )
{
	CExchangeRoom* pExchangeRoom = m_mpExchangeRoom->Alloc();

	ASSERT( pExchangeRoom );
	if( pExchangeRoom == NULL )
		return NULL;
	
	pExchangeRoom->Init( p1, p2 );
	pExchangeRoom->SetExchangeState( EXCS_WAITING );
	
	m_PtrList.AddTail( pExchangeRoom );

	return pExchangeRoom;
}

void CExchangeManager::DestroyExchangeRoom( CExchangeRoom* pRoom )
{
	if( pRoom == NULL ) return;
	pRoom->Exit();
	m_mpExchangeRoom->Free( pRoom );	
	m_PtrList.Remove( pRoom );
}

void CExchangeManager::UserLogOut( CPlayer* pPlayer )
{
	sEXCHANGECONTAINER* pEC = pPlayer->GetExchangeContainer();
	if( !pEC->pRoom ) return;
	
	if( pEC->pOpPlayer )
	{
		MSG_BYTE CancelMsg;
		CancelMsg.Category	= MP_EXCHANGE;
		CancelMsg.Protocol	= MP_EXCHANGE_CANCEL; //confirm
		CancelMsg.bData		= eEE_USERLOGOUT;

		pEC->pOpPlayer->SendMsg( &CancelMsg, sizeof( MSG_BYTE ) );
		if( pEC->pOpPlayer->GetState() == eObjectState_Exchange )
			OBJECTSTATEMGR_OBJ->EndObjectState(pEC->pOpPlayer, eObjectState_Exchange);

		pEC->pOpPlayer = NULL;
	}

	DestroyExchangeRoom( pEC->pRoom );
	pEC->pRoom = NULL;
}


void CExchangeManager::CancelExchange( CPlayer* pPlayer )
{
	sEXCHANGECONTAINER* pEC = pPlayer->GetExchangeContainer();

	if( !pEC->pRoom ) return;

	MSG_BYTE CancelMsg;
	CancelMsg.Category	= MP_EXCHANGE;
	CancelMsg.Protocol	= MP_EXCHANGE_CANCEL; //confirm
	CancelMsg.bData		= eEE_DIE;

	pPlayer->SendMsg( &CancelMsg, sizeof( MSG_BYTE ) );

	if( pEC->pOpPlayer )
	{
		CancelMsg.bData	= eEE_USERDIE;
		pEC->pOpPlayer->SendMsg( &CancelMsg, sizeof( MSG_BYTE ) );

		if( pEC->pOpPlayer->GetState() == eObjectState_Exchange )
			OBJECTSTATEMGR_OBJ->EndObjectState(pEC->pOpPlayer, eObjectState_Exchange);

		pEC->pOpPlayer = NULL;
	}

	DestroyExchangeRoom( pEC->pRoom );
	pEC->pRoom = NULL;
}


void CExchangeManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch( Protocol )
	{
	case MP_EXCHANGE_APPLY_SYN:
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;

			CPlayer* pPlayer	= (CPlayer*)g_pUserTable->FindUser(msg->dwObjectID);
			CPlayer* pOpPlayer	= (CPlayer*)g_pUserTable->FindUser(msg->dwData);

			ASSERT( pPlayer );
			if( pPlayer == NULL ) break;

			MSG_DWORD msgApplySyn;
			msgApplySyn.Category	= MP_EXCHANGE;
			msgApplySyn.dwData		= msg->dwData;

			if( CanApplyExchange( pPlayer, pOpPlayer ) )
			{
				msgApplySyn.Protocol	= MP_EXCHANGE_APPLY_ACK;
				pPlayer->SendMsg( &msgApplySyn, sizeof( msgApplySyn ) );
			}
			else
			{
				msgApplySyn.Protocol	= MP_EXCHANGE_APPLY_NACK;
				pPlayer->SendMsg( &msgApplySyn, sizeof( msgApplySyn ) );
				break;
			}

			CreateExchangeRoom( pPlayer, pOpPlayer );
	
			MSG_DWORD msgApply;
			msgApply.Category	= MP_EXCHANGE;
			msgApply.Protocol	= MP_EXCHANGE_APPLY;
			msgApply.dwData		= msg->dwObjectID;

			pOpPlayer->SendMsg( &msgApply, sizeof( msgApply ) );
			
			OBJECTSTATEMGR_OBJ->StartObjectState(pPlayer, eObjectState_Exchange, 0);
		}
		break;

	case MP_EXCHANGE_ACCEPT_SYN:
		{
			MSGBASE* msg = (MSGBASE*)pMsg;

			CPlayer* pPlayer1		= (CPlayer*)g_pUserTable->FindUser( msg->dwObjectID );
			if( pPlayer1 == NULL ) break;
			
			CPlayer* pPlayer2		= pPlayer1->GetExchangeContainer()->pOpPlayer;
			CExchangeRoom* pRoom	= pPlayer1->GetExchangeContainer()->pRoom;

			if( pPlayer2 == NULL )
			{
				MSGBASE AcceptNackMsg;
				AcceptNackMsg.Category	= MP_EXCHANGE;
				AcceptNackMsg.Protocol	= MP_EXCHANGE_ACCEPT_NACK;
				pPlayer1->SendMsg( &AcceptNackMsg, sizeof( MSGBASE ) );

				if( pRoom )
					DestroyExchangeRoom( pRoom );

				if( pPlayer1->GetState() == eObjectState_Exchange )
					OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer1, eObjectState_Exchange);

				break;
			}

			if( pRoom == NULL )
			{
				MSGBASE AcceptNackMsg;
				AcceptNackMsg.Category	= MP_EXCHANGE;
				AcceptNackMsg.Protocol	= MP_EXCHANGE_ACCEPT_NACK;
				pPlayer1->SendMsg( &AcceptNackMsg, sizeof( MSGBASE ) );

				AcceptNackMsg.Protocol	= MP_EXCHANGE_CANTAPPLY;
				pPlayer2->SendMsg( &AcceptNackMsg, sizeof( MSGBASE ) );

				if( pPlayer1->GetState() == eObjectState_Exchange )
					OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer1, eObjectState_Exchange);
				if( pPlayer2->GetState() == eObjectState_Exchange )
					OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer2, eObjectState_Exchange);

				break;
			}			


			VECTOR3 vPosApplyer, vPosAccepter;
			pPlayer2->GetPosition( &vPosApplyer );
			pPlayer1->GetPosition( &vPosAccepter );
			float fDist = CalcDistanceXZ( &vPosApplyer, &vPosAccepter );	//confirm

			if( fDist > 1500.0f )
			{
				MSGBASE msgError;
				msgError.Category	= MP_EXCHANGE;
				msgError.Protocol	= MP_EXCHANGE_CANTAPPLY;
				pPlayer1->SendMsg( &msgError, sizeof( MSGBASE ) );
				pPlayer2->SendMsg( &msgError, sizeof( MSGBASE ) );

				if( pPlayer1->GetState() == eObjectState_Exchange )
					OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer1, eObjectState_Exchange);
				if( pPlayer2->GetState() == eObjectState_Exchange )
					OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer2, eObjectState_Exchange);

				DestroyExchangeRoom( pRoom );
				break;
			}
			
			MSG_DWORD AcceptMsg;
			AcceptMsg.Category	= MP_EXCHANGE;
			AcceptMsg.Protocol	= MP_EXCHANGE_ACCEPT;
			AcceptMsg.dwData	= msg->dwObjectID;	//
			pPlayer2->SendMsg( &AcceptMsg, sizeof( MSG_DWORD) );

			MSGBASE StartMsg;
			StartMsg.Category	= MP_EXCHANGE;
			StartMsg.Protocol	= MP_EXCHANGE_START;

			pPlayer1->SendMsg( &StartMsg, sizeof( MSGBASE ) );
			pPlayer2->SendMsg( &StartMsg, sizeof( MSGBASE ) );
			
			pPlayer1->GetExchangeContainer()->pRoom->SetExchangeState( EXCS_DOING );

			OBJECTSTATEMGR_OBJ->StartObjectState(pPlayer1, eObjectState_Exchange, 0);
		}
		break;

	case MP_EXCHANGE_REJECT_SYN:
		{
			MSGBASE* msg = (MSGBASE*)pMsg;

			CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser( msg->dwObjectID );
			if( pPlayer1 == NULL ) break;

			CExchangeRoom* pRoom = pPlayer1->GetExchangeContainer()->pRoom;

			if( pRoom ) 
			if( pRoom->GetExchangeState() == EXCS_DOING )
				break;

			if( pPlayer1->GetState() == eObjectState_Exchange )
				OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer1, eObjectState_Exchange);

			CPlayer* pPlayer2 = pPlayer1->GetExchangeContainer()->pOpPlayer;
			DestroyExchangeRoom( pRoom );

			if( pPlayer2 == NULL ) break;

			MSGBASE RejectMsg;
			RejectMsg.Category	= MP_EXCHANGE;
			RejectMsg.Protocol	= MP_EXCHANGE_REJECT;
			pPlayer2->SendMsg( &RejectMsg, sizeof( MSGBASE ) );

			if( pPlayer2->GetState() == eObjectState_Exchange )
				OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer2, eObjectState_Exchange);
		}
		break;

	case MP_EXCHANGE_CANTAPPLY:
		{
			MSGBASE* msg = (MSGBASE*)pMsg;

			CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser( msg->dwObjectID );
			if( pPlayer1 == NULL ) break;

			if( pPlayer1->GetState() == eObjectState_Exchange )
				OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer1, eObjectState_Exchange);

			CPlayer* pPlayer2		= pPlayer1->GetExchangeContainer()->pOpPlayer;
			CExchangeRoom* pRoom	= pPlayer1->GetExchangeContainer()->pRoom;
			DestroyExchangeRoom( pRoom );

			if( pPlayer2 == NULL ) break;

			MSGBASE CantApplyMsg;
			CantApplyMsg.Category	= MP_EXCHANGE;
			CantApplyMsg.Protocol	= MP_EXCHANGE_CANTAPPLY;
			pPlayer2->SendMsg( &CantApplyMsg, sizeof( MSGBASE ) );

			if( pPlayer2->GetState() == eObjectState_Exchange )
				OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer2, eObjectState_Exchange);
		}
		break;

	case MP_EXCHANGE_WAITING_CANCEL_SYN:
		{
			MSGBASE* msg = (MSGBASE*)pMsg;

			CPlayer* pPlayer1		= (CPlayer*)g_pUserTable->FindUser( msg->dwObjectID );
			if( pPlayer1 == NULL ) break;
			
			CExchangeRoom* pRoom = pPlayer1->GetExchangeContainer()->pRoom;

			if( pRoom != NULL )
			{
				if( pRoom->GetExchangeState() == EXCS_DOING )
				{
					MSGBASE CancelMsg = *msg;

					CancelMsg.Protocol = MP_EXCHANGE_WAITING_CANCEL_NACK;
					pPlayer1->SendMsg( &CancelMsg, sizeof( MSGBASE ) );
					break;
				}
			}
			else
			{
				break;
			}

			MSGBASE CancelMsg = *msg;

			CancelMsg.Protocol = MP_EXCHANGE_WAITING_CANCEL_ACK;
			pPlayer1->SendMsg( &CancelMsg, sizeof( MSGBASE ) );

			if( pPlayer1->GetState() == eObjectState_Exchange )
				OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer1, eObjectState_Exchange);

			CPlayer* pPlayer2		= pPlayer1->GetExchangeContainer()->pOpPlayer;

			DestroyExchangeRoom( pRoom );

			if( pPlayer2 == NULL ) break;	//
			CancelMsg.Protocol = MP_EXCHANGE_WAITING_CANCEL;
			pPlayer2->SendMsg( &CancelMsg, sizeof( MSGBASE ) );
			
			if( pPlayer2->GetState() == eObjectState_Exchange )
				OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer2, eObjectState_Exchange);
		}
		break;

	case MP_EXCHANGE_ADDITEM_SYN:
		{
			MSG_LINKITEM* msg = (MSG_LINKITEM*)pMsg;

			CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser( msg->dwObjectID );
			if( pPlayer1 == NULL ) break;
			CPlayer* pPlayer2 = pPlayer1->GetExchangeContainer()->pOpPlayer;
			if( pPlayer2 == NULL ) break;
		
			CExchangeRoom* pRoom = pPlayer1->GetExchangeContainer()->pRoom;
			if( pRoom == NULL ) break;	

			MSG_LINKITEM ItemMsg = *msg;

			
			CItemSlot* pInventory = NULL;
			eITEMTABLE Table = eItemTable_Inventory;
			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( msg->ItemInfo.wIconIdx );
			if(!pInfo)		return;
			if( pInfo->ItemKind & eSHOP_ITEM )
			{
				Table = eItemTable_ShopInven;
				pInventory = (CShopInvenSlot*)pPlayer1->GetSlot( eItemTable_ShopInven );
			}
			else
				pInventory = (CInventorySlot*)pPlayer1->GetSlot( eItemTable_Inventory );

			if( pInventory == NULL ) return;
			const ITEMBASE* pItemBase = pInventory->GetItemInfoAbs( msg->ItemInfo.Position );
			if( pItemBase == NULL )
			{
				ItemMsg.Protocol = MP_EXCHANGE_ADDITEM_NACK;
				pPlayer1->SendMsg( &ItemMsg, sizeof( MSG_LINKITEM ) );
				return;
			}

			// magi82 - UniqueItem(070628)
			eITEM_KINDBIT bits = GetItemKind(msg->ItemInfo.wIconIdx);
			if( bits == eEQUIP_ITEM_UNIQUE && GAMERESRCMNGR->GetUniqueItemOptionList(msg->ItemInfo.wIconIdx)->ExchangeFlag == 0 )
			{
				ItemMsg.Protocol = MP_EXCHANGE_ADDITEM_NACK;
				pPlayer1->SendMsg( &ItemMsg, sizeof( MSG_LINKITEM ) );
				return;
			}

			if( pInfo->ItemKind == eQUEST_ITEM_PET || pInfo->ItemKind == eSHOP_ITEM_PET )
			{
				if( FALSE == pPlayer1->GetPetManager()->CheckPetDefaultFriendship(pItemBase->dwDBIdx) 
					|| TRUE == pPlayer1->GetPetManager()->IsCurPetSummonItem(pItemBase->dwDBIdx) )
				{
					ASSERT(0); // ClientCheck
					ItemMsg.Protocol = MP_EXCHANGE_ADDITEM_NACK;
					pPlayer1->SendMsg( &ItemMsg, sizeof( MSG_LINKITEM ) );
					return;
				}
			}

			if( pItemBase->dwDBIdx != msg->ItemInfo.dwDBIdx ||
				pItemBase->wIconIdx != msg->ItemInfo.wIconIdx ||
				pItemBase->Durability != msg->ItemInfo.Durability ||
				pItemBase->RareIdx != msg->ItemInfo.RareIdx ||
				pItemBase->Position != msg->ItemInfo.Position ||
				pItemBase->ItemParam != msg->ItemInfo.ItemParam )
			{
				ItemMsg.Protocol = MP_EXCHANGE_ADDITEM_NACK;
				pPlayer1->SendMsg( &ItemMsg, sizeof( MSG_LINKITEM ) );
				return;
			}

			if( ITEMMGR->IsDupItem(pItemBase->wIconIdx) )
			if( pItemBase->Durability > MAX_YOUNGYAKITEM_DUPNUM )
			{
				ItemMsg.Protocol = MP_EXCHANGE_ADDITEM_NACK;
				pPlayer1->SendMsg( &ItemMsg, sizeof( MSG_LINKITEM ) );
				return;
			}

			if( pRoom->IsLocked( pPlayer1->GetExchangeContainer()->nMyIndex ) )
			{
				ItemMsg.Protocol = MP_EXCHANGE_ADDITEM_NACK;
				pPlayer1->SendMsg( &ItemMsg, sizeof( MSG_LINKITEM ) );
				return;
			}

			if( !pRoom->AddItem( pPlayer1->GetExchangeContainer()->nMyIndex, &msg->ItemInfo ) )
			{
				ItemMsg.Protocol = MP_EXCHANGE_ADDITEM_NACK;
				pPlayer1->SendMsg( &ItemMsg, sizeof( MSG_LINKITEM ) );
				return;
			}
			if( !CHKRT->ItemOf( pPlayer1, msg->ItemInfo.Position, msg->ItemInfo.wIconIdx,
				msg->ItemInfo.Durability, msg->ItemInfo.QuickPosition ) )
			{
				ItemMsg.Protocol = MP_EXCHANGE_ADDITEM_NACK;
				pPlayer1->SendMsg( &ItemMsg, sizeof( MSG_LINKITEM ) );

				return;
			}

			ItemMsg.Protocol = MP_EXCHANGE_ADDITEM_ACK;
			pPlayer1->SendMsg( &ItemMsg, sizeof( MSG_LINKITEM ) );
			
			//ItemMsg.Protocol = MP_EXCHANGE_ADDITEM;
			//pPlayer2->SendMsg( &ItemMsg, sizeof( MSG_LINKITEM ) );

			/*MSG_LINKITEM_ADD ItemOptionMsg;
			ItemOptionMsg.Category		= MP_EXCHANGE;
			ItemOptionMsg.Protocol		= MP_EXCHANGE_ADDITEM;
			ItemOptionMsg.dwObjectID	= msg->dwObjectID;
			ItemOptionMsg.ItemInfo		= msg->ItemInfo;
			ItemOptionMsg.wAbsPosition	= msg->wAbsPosition;
			if( ITEMMGR->IsPetSummonItem( msg->ItemInfo.wIconIdx ) )
			{
				ItemOptionMsg.bPetInfo = TRUE;
				ItemOptionMsg.sPetTotalInfo = *pPlayer1->GetPetManager()->GetPetTotalInfo(msg->ItemInfo.dwDBIdx);
			}

			if( ITEMMGR->IsOptionItem( msg->ItemInfo.wIconIdx, msg->ItemInfo.Durability ) )
			{
				ItemOptionMsg.bOptionItem = 1;
				ItemOptionMsg.sOptionInfo = *pPlayer1->GetItemOption( msg->ItemInfo.Durability );
			}

			//SW050920 Rare			
			if( ITEMMGR->IsRareOptionItem( msg->ItemInfo.wIconIdx, msg->ItemInfo.RareIdx ) )
			{
				ItemOptionMsg.bRareOptionItem = TRUE;
				ItemOptionMsg.sRareOptionInfo = *pPlayer1->GetItemRareOption( msg->ItemInfo.RareIdx );
			}
			
			pPlayer2->SendMsg( &ItemOptionMsg, ItemOptionMsg.GetSize() );
			*/
			//SW070308 아이템부가옵션들 정리
			MSG_LINKITEMOPTIONS itemOpts;
			itemOpts.Category		= MP_EXCHANGE;
			itemOpts.Protocol		= MP_EXCHANGE_ADDITEM;
			itemOpts.dwObjectID		= msg->dwObjectID;
			itemOpts.ItemInfo		= msg->ItemInfo;
			itemOpts.wAbsPosition	= msg->wAbsPosition;

			ITEMMGR->SetItemOptionsInfoMsg(pPlayer1, &msg->ItemInfo, (MSG_LINKITEMOPTIONS*)&itemOpts);

			pPlayer2->SendMsg( &itemOpts, itemOpts.GetSize() );
		}
		break;

	case MP_EXCHANGE_DELITEM_SYN:
		{
			MSG_WORD* msg = (MSG_WORD*)pMsg;

			CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser( msg->dwObjectID );
			if( pPlayer1 == NULL ) break;
			CPlayer* pPlayer2 = pPlayer1->GetExchangeContainer()->pOpPlayer;
			if( pPlayer2 == NULL ) break;

			CExchangeRoom* pRoom = pPlayer1->GetExchangeContainer()->pRoom;
			
			if( pRoom == NULL ) break;

			MSG_WORD DelMsg = *msg;

			if( pRoom->IsLocked( pPlayer1->GetExchangeContainer()->nMyIndex ) )
			{
				DelMsg.Protocol = MP_EXCHANGE_DELITEM_NACK;
				pPlayer1->SendMsg( &DelMsg, sizeof( MSG_WORD ) );
				return;
			}

			if( !pRoom->DelItem( pPlayer1->GetExchangeContainer()->nMyIndex, msg->wData-1 ) )
			{
				DelMsg.Protocol = MP_EXCHANGE_DELITEM_NACK;
				pPlayer1->SendMsg( &DelMsg, sizeof( MSG_WORD ) );
				return;
			}

			DelMsg.Protocol = MP_EXCHANGE_DELITEM_ACK;
			pPlayer1->SendMsg( &DelMsg, sizeof( MSG_WORD ) );

			DelMsg.Protocol = MP_EXCHANGE_DELITEM;
			pPlayer2->SendMsg( &DelMsg, sizeof( MSG_WORD ) );
		}
		break;
		
	case MP_EXCHANGE_LOCK_SYN:
		{
			MSGBASE* msg = (MSGBASE*)pMsg;
			CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser( msg->dwObjectID );
			if( pPlayer1 == NULL ) break;
			CPlayer* pPlayer2 = pPlayer1->GetExchangeContainer()->pOpPlayer;
			if( pPlayer2 == NULL ) break;
			//CHECK

			CExchangeRoom* pRoom = pPlayer1->GetExchangeContainer()->pRoom;
			if( !pRoom ) return;		//error

			pRoom->Lock( pPlayer1->GetExchangeContainer()->nMyIndex, TRUE );

			MSGBASE LockMsg = *msg;
			LockMsg.Protocol = MP_EXCHANGE_LOCK_ACK;
			pPlayer1->SendMsg( &LockMsg, sizeof( MSGBASE ) );

			LockMsg.Protocol	= MP_EXCHANGE_LOCK;
			pPlayer2->SendMsg( &LockMsg, sizeof( MSGBASE ) );


		}
		break;

	case MP_EXCHANGE_INPUTMONEY_SYN:
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser( msg->dwObjectID );
			if( pPlayer1 == NULL ) break;
			CPlayer* pPlayer2 = pPlayer1->GetExchangeContainer()->pOpPlayer;
			if( pPlayer2 == NULL ) break;

			CExchangeRoom* pRoom = pPlayer1->GetExchangeContainer()->pRoom;
			if( pRoom == NULL ) break;	//error
			
			//KES
			if( pRoom->IsLocked( pPlayer1->GetExchangeContainer()->nMyIndex ) )
			{
				MSGBASE NackMsg;
				NackMsg.Category = MP_EXCHANGE;
				NackMsg.Protocol = MP_EXCHANGE_INPUTMONEY_NACK;
				NackMsg.dwObjectID = pPlayer1->GetID();
				
				pPlayer1->SendMsg( &NackMsg, sizeof(MSGBASE) );
				return;
			}
			
			DWORD dwMoney = pRoom->InputMoney( pPlayer1->GetExchangeContainer()->nMyIndex, msg->dwData );

			MSG_DWORD MoneyMsg = *msg;
			MoneyMsg.dwData = dwMoney;

			MoneyMsg.Protocol = MP_EXCHANGE_INPUTMONEY_ACK;
			pPlayer1->SendMsg( &MoneyMsg, sizeof( MSG_DWORD ) );

			MoneyMsg.Protocol = MP_EXCHANGE_INPUTMONEY;
			pPlayer2->SendMsg( &MoneyMsg, sizeof( MSG_DWORD ) );
		}
		break;

	case MP_EXCHANGE_UNLOCK_SYN:
		{
#ifndef _HK_LOCAL_
			MSGBASE* msg = (MSGBASE*)pMsg;
			CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser( msg->dwObjectID );
			if( pPlayer1 == NULL ) break;
			CPlayer* pPlayer2 = pPlayer1->GetExchangeContainer()->pOpPlayer;
			if( pPlayer2 == NULL ) break;

			CExchangeRoom* pRoom = pPlayer1->GetExchangeContainer()->pRoom;
			if( pRoom == NULL ) break;		//error

			pRoom->Lock( pPlayer1->GetExchangeContainer()->nMyIndex, FALSE );			

			MSGBASE LockMsg = *msg;			
			LockMsg.Protocol = MP_EXCHANGE_UNLOCK_ACK;
			pPlayer1->SendMsg( &LockMsg, sizeof( MSGBASE ) );

			LockMsg.Protocol	= MP_EXCHANGE_UNLOCK;
			pPlayer2->SendMsg( &LockMsg, sizeof( MSGBASE ) );
#endif
			//±³E? ¹oÆ°..?
		}
		break;

	case MP_EXCHANGE_CANCEL_SYN:
		{
			MSGBASE* msg = (MSGBASE*)pMsg;

			CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser( msg->dwObjectID );
			if( pPlayer1 == NULL ) break;
			CPlayer* pPlayer2 = pPlayer1->GetExchangeContainer()->pOpPlayer;
			if( pPlayer2 == NULL ) break;
			
			CExchangeRoom* pRoom = pPlayer1->GetExchangeContainer()->pRoom;
			if( pRoom == NULL ) break;
			
			DestroyExchangeRoom( pRoom );

			MSGBASE CancelMsg = *msg;
			CancelMsg.Protocol = MP_EXCHANGE_CANCEL_ACK;
			pPlayer1->SendMsg( &CancelMsg, sizeof( MSGBASE ) );

			MSG_BYTE ErrorMsg;
			ErrorMsg.Category	= MP_EXCHANGE;
			ErrorMsg.Protocol	= MP_EXCHANGE_CANCEL;
			ErrorMsg.bData		= eEE_USERCANCEL;
			pPlayer2->SendMsg( &ErrorMsg, sizeof( MSG_BYTE ) );

			if( pPlayer1->GetState() == eObjectState_Exchange )
				OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer1, eObjectState_Exchange);
			if( pPlayer2->GetState() == eObjectState_Exchange )
				OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer2, eObjectState_Exchange);
		}
		break;
	
	case MP_EXCHANGE_EXCHANGE_SYN:
		{
			MSGBASE* msg = (MSGBASE*)pMsg;

			CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser( msg->dwObjectID );
			if( pPlayer1 == NULL ) break;
			CPlayer* pPlayer2 = pPlayer1->GetExchangeContainer()->pOpPlayer;
			if( pPlayer2 == NULL ) break;

			CExchangeRoom* pRoom = pPlayer1->GetExchangeContainer()->pRoom;

			if( pRoom == NULL ) break;	//confirm
			if( pRoom->IsLocked( pPlayer1->GetExchangeContainer()->nMyIndex ) == FALSE )
			{

				MSGBASE ExchangeNackMsg;
				ExchangeNackMsg.Category = MP_EXCHANGE;
				ExchangeNackMsg.Protocol = MP_EXCHANGE_EXCHANGE_NACK;
				pPlayer1->SendMsg( &ExchangeNackMsg, sizeof( MSGBASE ) );
//				pPlayer2->SendMsg( &ExchangeNackMsg, sizeof( MSGBASE ) );
//				DestroyExchangeRoom( pRoom );
//				if( pPlayer1->GetState() == eObjectState_Exchange )
//					OBJECTSTATEMGR_OBJ->StartObjectState(pPlayer1, eObjectState_None, 0);
//					pPlayer1->SetState( eObjectState_None );
//				if( pPlayer2->GetState() == eObjectState_Exchange )
//					OBJECTSTATEMGR_OBJ->StartObjectState(pPlayer2, eObjectState_None, 0);
//					pPlayer2->SetState( eObjectState_None );


				break;
			}

			pRoom->SetExchange( pPlayer1->GetExchangeContainer()->nMyIndex );

			if( pRoom->IsAllExchange() )
			{
				int rt;
				if( ( rt = pRoom->DoExchange() ) == eEE_OK )
				{
					MSGBASE ExchangeMsg;
					ExchangeMsg.Category	= MP_EXCHANGE;
					ExchangeMsg.Protocol	= MP_EXCHANGE_EXCHANGE;
					pPlayer1->SendMsg( &ExchangeMsg, sizeof( MSGBASE ) );
					pPlayer2->SendMsg( &ExchangeMsg, sizeof( MSGBASE ) );
				}
				else
				{

					MSG_BYTE ErrorMsg;
					ErrorMsg.Category	= MP_EXCHANGE;
					ErrorMsg.Protocol	= MP_EXCHANGE_CANCEL;
					ErrorMsg.bData		= rt;
					pPlayer1->SendMsg( &ErrorMsg, sizeof( MSG_BYTE ) );
					pPlayer2->SendMsg( &ErrorMsg, sizeof( MSG_BYTE ) );
				}

				DestroyExchangeRoom( pRoom );

//				if( pPlayer1->GetState() == eObjectState_Exchange )
					OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer1, eObjectState_Exchange );
//				if( pPlayer2->GetState() == eObjectState_Exchange )
					OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer2, eObjectState_Exchange );
			}
		}
		break;
	}
}

CExchangeRoom* CExchangeManager::GetRoomInfo( DWORD dwPlayer1ID, CPlayer** ppPlayer1, CPlayer** ppPlayer2 )
{
	*ppPlayer1 = (CPlayer*)g_pUserTable->FindUser( dwPlayer1ID );
	if( *ppPlayer1 == NULL ) return NULL;

	*ppPlayer2 = (*ppPlayer1)->GetExchangeContainer()->pOpPlayer;

	return (*ppPlayer1)->GetExchangeContainer()->pRoom;
}


