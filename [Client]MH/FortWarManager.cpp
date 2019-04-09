#include "stdafx.h"
#include "FortWarManager.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "MiniMapDlg.h"
#include "ObjectStateManager.h"
#include "FortWarDialog.h"
#include "MHMap.h"
#include "ItemManager.h"
#include "InventoryExDialog.h"
#include "cWindowManager.h"


GLOBALTON(CFortWarManager)

CFortWarManager::CFortWarManager(void)
{
	m_nRelationFortWarIDX = 0;
	m_nTaxRate = 0;
}

CFortWarManager::~CFortWarManager(void)
{
}

void CFortWarManager::Init()
{
	m_nRelationFortWarIDX = 0;
	m_nTaxRate = 0;
}

void CFortWarManager::AddSymbolNpc( float x, float z)
{
	CObject* pObject = OBJECTMGR->GetObject( STATICNPC_ID_START + 10000 );
	if( pObject )
		return;

	NPC_TOTALINFO tinfo;
	BASEOBJECT_INFO binfo;
	BASEMOVE_INFO minfo;

	binfo.dwObjectID = STATICNPC_ID_START + 10000;
	binfo.ObjectState = 0;
	SafeStrCpy( binfo.ObjectName, "상징물", MAX_NAME_LENGTH+1 );
	
	tinfo.MapNum = MAP->GetMapNum();
	tinfo.NpcKind = 17;
	tinfo.NpcUniqueIdx = 15001;
	tinfo.NpcJob = FORTWAR_SYMBOL_ROLE;

	minfo.bMoving = FALSE;
	minfo.KyungGongIdx = 0;
	minfo.MoveMode = 0;
	minfo.CurPosition.x = x;
	minfo.CurPosition.y = 0;
	minfo.CurPosition.z = z;

	CNpc* pNpc = OBJECTMGR->AddNpc(&binfo,&minfo,&tinfo);
	if( pNpc == NULL )
		return;

	//방향.....
	float fDir = 0;
	pNpc->SetAngle( DEGTORAD( fDir ) );

	GAMEIN->GetMiniMapDialog()->AddStaticNpcIcon(pNpc);

	TARGETSET set;
	set.pTarget = pNpc;
	EFFECTMGR->StartEffectProcess(eEffect_MonRegen,pNpc,&set,0,pNpc->GetID());
}

void CFortWarManager::RemoveSymbolNpc()
{
	CObject* pObject = OBJECTMGR->GetObject( STATICNPC_ID_START + 10000 );
	if( pObject == NULL )
		return;
	OBJECTMGR->AddGarbageObject( pObject );
}

DWORD CFortWarManager::CalTexRateForSell( DWORD dwMoney )
{
	if( m_nRelationFortWarIDX == 0 ||
		m_nTaxRate == 0 )
		return dwMoney;

//	return ( dwMoney - dwMoney * m_nTaxRate / 100 );
	return ( dwMoney - (DWORD)(((float)dwMoney * m_nTaxRate) / 10000));
}

DWORD CFortWarManager::CalTexRateForBuy( DWORD dwMoney )
{
	if( m_nRelationFortWarIDX == 0 ||
		m_nTaxRate == 0 )
		return dwMoney;

//	return ( dwMoney + dwMoney * m_nTaxRate / 100 );
	return ( dwMoney + (DWORD)(((float)dwMoney * m_nTaxRate) / 10000));
}

void CFortWarManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch(Protocol)
	{		
	case MP_FORTWAR_INFO:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			// 현재맵의 세금 설정
			m_nRelationFortWarIDX = pmsg->dwData1;
			m_nTaxRate = pmsg->dwData2;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1745 ), m_nTaxRate/100 );
//			CHATMGR->AddMsg( CTC_SYSMSG, "[요새전]요새관할구역으로 %d%%의 세금이 부과됩니다.", m_nTaxRate/100 );
		}
		break;
	case MP_FORTWAR_START_BEFORE10MIN:
		{
			MSG_FORTWAR_START* pmsg = (MSG_FORTWAR_START*)pMsg;

//			if( MAP->GetMapNum() == pmsg->wForWarMapNum )
//			if( (gChannelNum+1) == pmsg->dwChannelNum )
//			{
//				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1746 ) );
//				CHATMGR->AddMsg( CTC_SYSMSG, "[요새전]요새전 시작 10분전입니다." );
//			}
//			else
//			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1747 ), MAP->GetMapName(pmsg->wForWarMapNum) );
//				CHATMGR->AddMsg( CTC_SYSMSG, "[요새전]%s의 요새전 시작 10분전입니다.", MAP->GetMapName(pmsg->wForWarMapNum) );
//			}
		}
		break;
	case MP_FORTWAR_START:
		{
			MSG_FORTWAR_START* pmsg = (MSG_FORTWAR_START*)pMsg;

			if( MAP->GetMapNum() == pmsg->wForWarMapNum )
			if( (gChannelNum+1) == pmsg->dwChannelNum )
			{
				if( GAMEIN->GetFWTimeDlg() )
				{
					GAMEIN->GetFWTimeDlg()->SetActiveWithTimeName( TRUE, pmsg->dwFortWarTime, pmsg->EngraveSuccessPlayerName );
				}
//				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1748 ) );
//				CHATMGR->AddMsg( CTC_SYSMSG, "[요새전]요새전이 시작되었습니다." );

				AddSymbolNpc( pmsg->vNpcPos.x, pmsg->vNpcPos.y );
			}
//			else
//			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1749 ), MAP->GetMapName(pmsg->wForWarMapNum) );
//				CHATMGR->AddMsg( CTC_SYSMSG, "[요새전]%s의 요새전이 시작되었습니다.", MAP->GetMapName(pmsg->wForWarMapNum) );
//			}			
		}
		break;
	case MP_FORTWAR_ING:
		{
			MSG_FORTWAR_START* pmsg = (MSG_FORTWAR_START*)pMsg;

			if( GAMEIN->GetFWTimeDlg() )
			{
				GAMEIN->GetFWTimeDlg()->SetActiveWithTimeName( TRUE, pmsg->dwFortWarTime, pmsg->EngraveSuccessPlayerName );
			}
			AddSymbolNpc( pmsg->vNpcPos.x, pmsg->vNpcPos.y );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1750 ) );
//			CHATMGR->AddMsg( CTC_SYSMSG, "[요새전]요새전이 진행중입니다." );
		}
		break;
	case MP_FORTWAR_END:
		{
			MSG_FORTWAR_END* pmsg = (MSG_FORTWAR_END*)pMsg;

			if( GAMEIN->GetFWTimeDlg() )
			{
				GAMEIN->GetFWTimeDlg()->SetActive( FALSE );
			}
			RemoveSymbolNpc();

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1751 ), MAP->GetMapName(pmsg->wForWarMapNum) );
//			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1751 ) );
//			CHATMGR->AddMsg( CTC_SYSMSG, "[요새전]요새전이 종료되었습니다." );			
		}
		break;
	case MP_FORTWAR_ENGRAVE_START_ACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwData1 );
			if( !pPlayer )	return;

            if( pPlayer->GetID() == HEROID )
			{
				if( GAMEIN->GetFWEngraveDlg() )
					GAMEIN->GetFWEngraveDlg()->SetActiveWithTime( TRUE, pmsg->dwData2 );
			}

			if( pPlayer->GetState() == eObjectState_Deal )
                OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
            OBJECTSTATEMGR->StartObjectState(pPlayer, eObjectState_Engrave);
			
            OBJECTEFFECTDESC desc(FindEffectNum("eff_gakin_s.beff"));
            pPlayer->AddObjectEffect( ENGRAVE_EFFECTID, &desc, 1, HERO );
		}
		break;
	case MP_FORTWAR_ENGRAVE_START_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1752 ) );
//			CHATMGR->AddMsg( CTC_SYSMSG, "[요새전]각인을 할 수 없습니다." );
		}
		break;
	case MP_FORTWAR_ENGRAVE_CANCEL_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwData );
			if( !pPlayer )	return;

			if( pPlayer == HERO )
			{
				if( GAMEIN->GetFWEngraveDlg() )
					GAMEIN->GetFWEngraveDlg()->SetActive( FALSE );
			}

			pPlayer->RemoveObjectEffect( ENGRAVE_EFFECTID );
			if( pPlayer->GetState() == eObjectState_Engrave )
				OBJECTSTATEMGR->EndObjectState( pPlayer, eObjectState_Engrave );

			pPlayer->RemoveObjectEffect( ENGRAVE_NACK_EFFECTID );
			OBJECTEFFECTDESC desc(FindEffectNum("eff_gakin_fail.beff"));
			pPlayer->AddObjectEffect( ENGRAVE_NACK_EFFECTID, &desc, 1, HERO );			
		}
		break;
	case MP_FORTWAR_ENGRAVE_CANCEL_NACK:
		{
		}
		break;
	case MP_FORTWAR_ENGRAVE_SUCESS:
		{
			MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwData );
			if( pPlayer != NULL )
			{
                if( pPlayer == HERO )
                {
                    if( GAMEIN->GetFWEngraveDlg() )
                        GAMEIN->GetFWEngraveDlg()->SetActive( FALSE );
				}

                pPlayer->RemoveObjectEffect( ENGRAVE_EFFECTID );
                if( OBJECTSTATEMGR->GetObjectState( pPlayer ) == eObjectState_Engrave )
                    OBJECTSTATEMGR->EndObjectState( pPlayer, eObjectState_Engrave );
                
				pPlayer->RemoveObjectEffect( ENGRAVE_ACK_EFFECTID );
                OBJECTEFFECTDESC desc(FindEffectNum("eff_gakin_suc.beff"));
                pPlayer->AddObjectEffect( ENGRAVE_ACK_EFFECTID, &desc, 1, HERO );
			}
			if( GAMEIN->GetFWTimeDlg() )
                GAMEIN->GetFWTimeDlg()->SetCharacterName( pmsg->Name );
		}
		break;
	case MP_FORTWAR_ENGRAVE_FAIL:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwData );
			if( !pPlayer )	return;

			if( pPlayer == HERO )
			{
				if( GAMEIN->GetFWEngraveDlg() )
					GAMEIN->GetFWEngraveDlg()->SetActive( FALSE );
			}

			pPlayer->RemoveObjectEffect( ENGRAVE_EFFECTID );
			if( OBJECTSTATEMGR->GetObjectState( pPlayer ) == eObjectState_Engrave )
				OBJECTSTATEMGR->EndObjectState( pPlayer, eObjectState_Engrave );

			pPlayer->RemoveObjectEffect( ENGRAVE_ACK_EFFECTID );
			OBJECTEFFECTDESC desc(FindEffectNum("eff_gakin_fail.beff"));
			pPlayer->AddObjectEffect( ENGRAVE_ACK_EFFECTID, &desc, 1, HERO );
		}
		break;	
	//
	case MP_FORTWAR_WAREHOUSE_INFO_ACK:
		{
			MSG_FORTWAR_WAREHOUSE_INFO* pmsg = (MSG_FORTWAR_WAREHOUSE_INFO*)pMsg;

			GAMEIN->GetFWWareHouseDlg()->SetFortWarWareHouseInfo( pmsg, eFortWareHouseDlg );
			GAMEIN->GetFWWareHouseDlg()->SetActive( TRUE );
		}
		break;
	case MP_FORTWAR_WAREHOUSE_INFO_NACK:
		{
		}
		break;
	case MP_FORTWAR_SIEGEWAREHOUSE_INFO_ACK:
		{
			MSG_FORTWAR_WAREHOUSE_INFO* pmsg = (MSG_FORTWAR_WAREHOUSE_INFO*)pMsg;

			GAMEIN->GetFWWareHouseDlg()->SetFortWarWareHouseInfo( pmsg, eSeigeWareHouseDlg );
			GAMEIN->GetFWWareHouseDlg()->SetActive( TRUE );
		}
		break;
	case MP_FORTWAR_SIEGEWAREHOUSE_INFO_NACK:
		{
		}
		break;
	case MP_FORTWAR_WAREHOUSE_ITEM_MOVE_ACK:
		{
            MSG_ITEM_MOVE_ACK* pmsg = (MSG_ITEM_MOVE_ACK*)pMsg;

			CFWWareHouseDialog* pFortWarWareHouseDlg = GAMEIN->GetFWWareHouseDlg();
			CInventoryExDialog* pInvenDlg = GAMEIN->GetInventoryDialog();

			CItem* FromItem = NULL;

			pFortWarWareHouseDlg->DeleteItem( pmsg->FromPos, &FromItem );

			FromItem->SetPosition( pmsg->ToPos );
			pInvenDlg->AddItem( FromItem );

			ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
			pFortWarWareHouseDlg->SetDisable( FALSE );
		}
		break;
	case MP_FORTWAR_WAREHOUSE_ITEM_MOVE_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			
			switch( pmsg->dwData )
			{
			case eFortWarWareError_None:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1753 ) );	break;	// CHATMGR->AddMsg( CTC_SYSMSG, "[요새창고] 사용할 수 없습니다." );	break;
			case eFortWarWareError_NotMap:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1754 ) );	break;	// CHATMGR->AddMsg( CTC_SYSMSG, "[요새창고] 사용할 수 있는 맵이 아닙니다." );	break;
			case eFortWarWareError_NotTime:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1755 ) );	break;	// CHATMGR->AddMsg( CTC_SYSMSG, "[요새창고] 사용할 수 있는 시간이 아닙니다." );	break;
			case eFortWarWareError_NotRight:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1756 ) );	break;	// CHATMGR->AddMsg( CTC_SYSMSG, "[요새창고] 사용할 권한이 없습니다." );	break;
			case eFortWarWareError_NotUse:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1753 ) );	break;	// CHATMGR->AddMsg( CTC_SYSMSG, "[요새창고] 사용할 수 없습니다." );	break;
			}

			WINDOWMGR->BackDragWindow();
			ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
			GAMEIN->GetFWWareHouseDlg()->SetDisable( FALSE );
		}
		break;
	case MP_FORTWAR_WAREHOUSE_ITEM_MOVE_TO_SIEGEHOUSE_SYN:
		{
			MSG_ITEM_MOVE_ACK* pmsg = (MSG_ITEM_MOVE_ACK*)pMsg;

			CFWWareHouseDialog* pFortWarWareHouseDlg = GAMEIN->GetFWWareHouseDlg();

			CItem* pFromItem = NULL;

			pFortWarWareHouseDlg->DeleteItem( pmsg->FromPos, &pFromItem );

			ITEMMGR->ItemDelete( pFromItem );

			ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
			pFortWarWareHouseDlg->SetDisable( FALSE );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1757 ) );
//			CHATMGR->AddMsg( CTC_SYSMSG, "공성창고로 아이템이 이동하였습니다." );
		}
		break;
	case MP_FORTWAR_SIEGEWAREHOUSE_ITEM_MOVE_ACK:
		{
			MSG_ITEM_MOVE_ACK* pmsg = (MSG_ITEM_MOVE_ACK*)pMsg;

			CFWWareHouseDialog* pFortWarWareHouseDlg = GAMEIN->GetFWWareHouseDlg();
			CInventoryExDialog* pInvenDlg = GAMEIN->GetInventoryDialog();

			CItem* FromItem = NULL;

			pFortWarWareHouseDlg->DeleteItem( pmsg->FromPos, &FromItem );

			FromItem->SetPosition( pmsg->ToPos );
			pInvenDlg->AddItem( FromItem );

			ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
			pFortWarWareHouseDlg->SetDisable( FALSE );
		}
		break;
	case MP_FORTWAR_SIEGEWAREHOUSE_ITEM_MOVE_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			switch( pmsg->dwData )
			{
			case eFortWarWareError_None:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1753 ) );	break;	// CHATMGR->AddMsg( CTC_SYSMSG, "[요새창고] 사용할 수 없습니다." );	break;
			case eFortWarWareError_NotMap:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1754 ) );	break;	// CHATMGR->AddMsg( CTC_SYSMSG, "[요새창고] 사용할 수 있는 맵이 아닙니다." );	break;
			case eFortWarWareError_NotTime:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1755 ) );	break;	// CHATMGR->AddMsg( CTC_SYSMSG, "[요새창고] 사용할 수 있는 시간이 아닙니다." );	break;
			case eFortWarWareError_NotRight:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1756 ) );	break;	// CHATMGR->AddMsg( CTC_SYSMSG, "[요새창고] 사용할 권한이 없습니다." );	break;
			case eFortWarWareError_NotUse:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1753 ) );	break;	// CHATMGR->AddMsg( CTC_SYSMSG, "[요새창고] 사용할 수 없습니다." );	break;
			}

			WINDOWMGR->BackDragWindow();
			ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
			GAMEIN->GetFWWareHouseDlg()->SetDisable( FALSE );
		}
		break;
	case MP_FORTWAR_WAREHOUSE_ITEM_INSERT:
		{
			MSG_FORTWAR_WAREITEM_INFO* pmsg = (MSG_FORTWAR_WAREITEM_INFO*)pMsg;

			CFWWareHouseDialog* pFortWarWareHouseDlg = GAMEIN->GetFWWareHouseDlg();
			if( pFortWarWareHouseDlg == NULL )
				return;

			if( pFortWarWareHouseDlg->IsActive() )
			{
				if( pFortWarWareHouseDlg->GetDialogKind() == eFortWareHouseDlg )
                    pFortWarWareHouseDlg->AddItem( &pmsg->ItemInfo );
			}
		}
		break;
	case MP_FORTWAR_WAREHOUSE_ITEM_MOVE_TO_INVEN:
		{
			MSG_FORTWAR_WAREITEM_INFO* pmsg = (MSG_FORTWAR_WAREITEM_INFO*)pMsg;

			CFWWareHouseDialog* pFortWarWareHouseDlg = GAMEIN->GetFWWareHouseDlg();
			if( pFortWarWareHouseDlg == NULL )
				return;

			if( pFortWarWareHouseDlg->IsActive() )
			{
				if( pFortWarWareHouseDlg->GetDialogKind() == eFortWareHouseDlg )
				{
					CItem* pFromItem = NULL;
					pFortWarWareHouseDlg->DeleteItem( pmsg->wFromPos, &pFromItem );
					ITEMMGR->ItemDelete( pFromItem );
				}
			}
		}
		break;
	case MP_FORTWAR_WAREHOUSE_ITEM_MOVE_TO_SIEGEHOUSE:
		{
			MSG_FORTWAR_WAREITEM_INFO* pmsg = (MSG_FORTWAR_WAREITEM_INFO*)pMsg;

			CFWWareHouseDialog* pFortWarWareHouseDlg = GAMEIN->GetFWWareHouseDlg();
			if( pFortWarWareHouseDlg == NULL )
				return;

			if( pFortWarWareHouseDlg->IsActive() )
			{
				if( pFortWarWareHouseDlg->GetDialogKind() == eFortWareHouseDlg )		// 요새창고를 본다면...
				{
					CItem* pFromItem = NULL;
					pFortWarWareHouseDlg->DeleteItem( pmsg->wFromPos, &pFromItem );
					ITEMMGR->ItemDelete( pFromItem );
				}
				else if( pFortWarWareHouseDlg->GetDialogKind() == eSeigeWareHouseDlg )
				{
					pFortWarWareHouseDlg->AddItem( &pmsg->ItemInfo );
				}
			}
		}
		break;
	case MP_FORTWAR_SIEGEWAREHOUSE_ITEM_MOVE_TO_INVEN:
		{
			MSG_FORTWAR_WAREITEM_INFO* pmsg = (MSG_FORTWAR_WAREITEM_INFO*)pMsg;

			CFWWareHouseDialog* pFortWarWareHouseDlg = GAMEIN->GetFWWareHouseDlg();
			if( pFortWarWareHouseDlg == NULL )
				return;

			if( pFortWarWareHouseDlg->IsActive() )
			{
				if( pFortWarWareHouseDlg->GetDialogKind() == eSeigeWareHouseDlg )
				{
					CItem* pFromItem = NULL;
					pFortWarWareHouseDlg->DeleteItem( pmsg->wFromPos, &pFromItem );
					ITEMMGR->ItemDelete( pFromItem );
				}
			}
		}
		break;
	case MP_FORTWAR_WAREHOUSE_PUTOUT_MONEY_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CFWWareHouseDialog* pFortWarWareHouseDlg = GAMEIN->GetFWWareHouseDlg();
			if( pFortWarWareHouseDlg == NULL )
				return;

			if( pFortWarWareHouseDlg->IsActive() )
			{
				if( pFortWarWareHouseDlg->GetDialogKind() == eFortWareHouseDlg )
				{
					pFortWarWareHouseDlg->SetMoney( pmsg->dwData );					
				}
			}
		}
		break;
	case MP_FORTWAR_WAREHOUSE_PUTOUT_MONEY_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			switch( pmsg->dwData )
			{
			case eFortWarWareError_None:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1753 ) );	break;	// CHATMGR->AddMsg( CTC_SYSMSG, "[요새창고] 사용할 수 없습니다." );	break;
			case eFortWarWareError_NotMap:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1754 ) );	break;	// CHATMGR->AddMsg( CTC_SYSMSG, "[요새창고] 사용할 수 있는 맵이 아닙니다." );	break;
			case eFortWarWareError_NotTime:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1755 ) );	break;	// CHATMGR->AddMsg( CTC_SYSMSG, "[요새창고] 사용할 수 있는 시간이 아닙니다." );	break;
			case eFortWarWareError_NotRight:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1756 ) );	break;	// CHATMGR->AddMsg( CTC_SYSMSG, "[요새창고] 사용할 권한이 없습니다." );	break;
			case eFortWarWareError_NotUse:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1753 ) );	break;	// CHATMGR->AddMsg( CTC_SYSMSG, "[요새창고] 사용할 수 없습니다." );	break;
			}
		}
		break;
	case MP_FORTWAR_DROPITEM_TO_FORTWAREHOUSE:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( pmsg->dwData );
			if( pInfo )
			{
                CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1760 ), pInfo->ItemName );
//				CHATMGR->AddMsg( CTC_SYSMSG, "요새창고로 %s 아이템이 드랍되었습니다.", pInfo->ItemName );
			}
		}
		break;
	}
}
