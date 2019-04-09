// InventoryExDialog.cpp: implementation of the CInventoryExDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InventoryExDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cIconGridDialog.h"
#include "./Interface/cStatic.h"
#include "./Interface/cPushupButton.h"
#include "ObjectStateManager.h"

#include "GameIn.h"

#include "ItemManager.h"
#include "ObjectManager.h"
#include "KyungGongManager.h"
#include "MugongManager.h"
#include "SkillManager_Client.h"
#include "ChatManager.h"
#include "MussangManager.h"

#include "DealItem.h"
#include "cDivideBox.h"
#include "./Audio/MHAudioManager.h"

#include "MugongDialog.h"
#include "DealDialog.h"

#include "./Input/UserInput.h"

#include "PyogukDialog.h"
#include "GuildWarehouseDialog.h"

#include "MainBarDialog.h"
#include "PKManager.h"
#include "cMsgBox.h"
#include "QuestManager.h"
#include "MoveDialog.h"
#include "PointSaveDialog.h"
#include "SkillPointRedist.h"
#include "ChaseinputDialog.h"
#include "ReinforceDlg.h"
#include "MixDialog.h"
#include "CharChangeDlg.h"
#include "SealDialog.h"
#include "ChangeJobDialog.h"
#include "ReinforceResetDlg.h"
#include "NameChangeDialog.h"
#include "NameChangeNotifyDlg.h"
#include "RareCreateDialog.h"
#include "PetUpgradeDialog.h"
#include "PetRevivalDialog.h"
#include "PetWearedExDialog.h"
#include "TitanInventoryDlg.h"
#include "PetInventoryDlg.h"
#include "StreetStallManager.h"
#include "ShoutDialog.h"

#include "StreetStallManager.h"
#include "ShoutDialog.h"

#include "VimuManager.h"
#include "MoveManager.h"
#include "./Interface/cScriptManager.h"

#include "NpcScriptManager.h"
#include "MHMap.h"
#include "SiegeWarMgr.h"
#include "PetManager.h"
#include "TitanManager.h"

#include "GuildNoteDlg.h"
#include "UnionNoteDlg.h"

#include "SkillOptionCleardlg.h"
#include "SurvivalModeManager.h"

#ifdef _TL_LOCAL_
#include "BattleSystem_Client.h"
#include "PartyWar.h"
#endif

#include "SkinSelectDialog.h"
#include "CostumeSkinSelectDialog.h"
#include "UniqueItemCurseCancellationDlg.h"
#include "SkillPointResetDlg.h"
//#include "TitanMugongMixDlg.h"	// magi82 - Titan(070611) 타이탄 무공변환 주석처리
#include "MouseCursor.h"
#include "FortWarDialog.h"
#include "FortWarManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInventoryExDialog::CInventoryExDialog()
{
	m_type				= WT_INVENTORYDIALOG;
	m_pWearedDlg		= NULL;
	m_pMoneyEdit		= NULL;
	m_fSpread			= FALSE;
	m_iInventoryState	= eITEMDEFAULT_STATE;
	m_pItemShopInven	= NULL;
	m_pItemShopBtn		= NULL;
	m_bShopInven		= FALSE;

	m_BtnPushstartTime		= 0;
	m_BtnPushDelayTime		= 500;
	m_bPushTabWithMouseOver = FALSE;
	
	m_pSelectedShopItem = NULL;
}

CInventoryExDialog::~CInventoryExDialog()
{
	m_pWearedDlg		= NULL;
	m_pItemShopInven	= NULL;
	m_pItemShopBtn		= NULL;
}

void CInventoryExDialog::ReleaseInventory()
{
	//해쉬 테이블 참조해서 지운다?
	//그리드 아이콘 dlg -> delete;
	//윈도우에 adddestroylist
}

void CInventoryExDialog::Render()
{
	cDialog::RenderWindow();
	cDialog::RenderComponent();
	cTabDialog::RenderTabComponent();
}
void CInventoryExDialog::Add(cWindow * window)
{
	if(window->GetType() == WT_PUSHUPBUTTON && window->GetID() != IN_SHOPITEMBTN)
		AddTabBtn(curIdx1++, (cPushupButton * )window);
	else if(window->GetType() == WT_ICONGRIDDIALOG && window->GetID() != IN_SHOPITEMGRID)
	{
		AddTabSheet(curIdx2++, window);
		((cIconGridDialog*)window)->SetDragOverIconType( WT_ITEM );
	}
	else 
		cDialog::Add(window);
}


DWORD CInventoryExDialog::ActionEvent(CMouse * mouseInfo)
{
	//아이콘 드래그중일때.. 다른 탭으로 이동해야한다.
	if( WINDOWMGR->IsDragWindow() && WINDOWMGR->GetDragDlg() )
	{
		if( !m_bShopInven && WINDOWMGR->GetDragDlg()->GetType() == WT_ITEM )
		{
			DWORD we = WE_NULL;
			if(!m_bActive) return we;
			we = cDialog::ActionEvent(mouseInfo);

			BOOL bMouseOver = FALSE;
			for( int i = 0 ; i < m_bTabNum ; i++ )
			{
				m_ppPushupTabBtn[i]->ActionEvent( mouseInfo );
				
				if( we & WE_MOUSEOVER )
				if( m_ppPushupTabBtn[i]->IsActive() && m_ppPushupTabBtn[i]->PtInWindow( mouseInfo->GetMouseX(), mouseInfo->GetMouseY() ) )
				{
					bMouseOver = TRUE;
					
					if( m_bPushTabWithMouseOver )
					{
						if( i != m_bSelTabNum )
						{
							SelectTab(i);		//순서지키기(수련창때문..)
							m_bSelTabNum = i;							
						}
					}
					else
					{
						if( m_BtnPushstartTime == 0 )
							m_BtnPushstartTime = gCurTime;
						else if( gCurTime - m_BtnPushstartTime > m_BtnPushDelayTime )
							m_bPushTabWithMouseOver = TRUE;
					}				
				}
			}
			
			if( !bMouseOver )
				m_BtnPushstartTime = 0;
			
			we |= m_ppWindowTabSheet[m_bSelTabNum]->ActionEvent( mouseInfo );
			return we;
		}
	}

	m_BtnPushstartTime		= 0;
	m_bPushTabWithMouseOver = FALSE;
	return cTabDialog::ActionEvent( mouseInfo );	
}


WORD CInventoryExDialog::GetTabIndex( POSTYPE absPos )
{
	return ( absPos - TP_INVENTORY_START ) / TABCELL_INVENTORY_NUM;
}

BOOL CInventoryExDialog::AddItem(ITEMBASE * itemBase)
{
	if( itemBase->dwDBIdx == 0 )
	{
//		DEBUGMSG( 0, "Item DB idx == 0" );
		return FALSE;
	}

	CItem * newItem = ITEMMGR->MakeNewItem( itemBase, "AddItem" );
	if(newItem == NULL)
		return FALSE;
	
	return AddItem( newItem );
}

// 영약 Dura도 증가 시켜 준다.리소스 삭제까지.
BOOL CInventoryExDialog::AddItem(CItem* pItem)
{
	if(pItem == NULL)
	{
		ASSERT(pItem);
		return FALSE;
	}

	//색 변환
	ITEMMGR->RefreshItem( pItem );
	//add 하는 아이템만 refresh 하면 된다?

	/*
	if(TP_INVENTORY_START <= pItem->GetPosition() && pItem->GetPosition() < TP_INVENTORY_END)
	{
		
		// 인벤토리 
		int tabIndex = GetTabIndex( pItem->GetPosition() );
		cIconGridDialog * dlg = (cIconGridDialog *)GetTabSheet(tabIndex);

//		WORD relPos = pItem->GetPosition()-INVENTORY_STARTPOSITION-CELLNUMPERPAGE*tabIndex;
		WORD relPos = pItem->GetPosition()-TP_INVENTORY_START-TABCELL_INVENTORY_NUM*tabIndex;
		CItem * item = (CItem * )dlg->GetIconForIdx(relPos);
		if(item)
		{
			ASSERT(0);
			return FALSE;
		}
		else
		{
			return dlg->AddIcon(relPos, pItem);
		}
	}
	else if(TP_WEAR_START <= pItem->GetPosition() && pItem->GetPosition() < TP_WEAR_END)
	{
		// 장착창
		return m_pWearedDlg->AddItem(pItem->GetPosition()-TP_WEAR_START, pItem);
	}
	else if(TP_SHOPINVEN_START <= pItem->GetPosition() && pItem->GetPosition() < TP_SHOPINVEN_END-TABCELL_SHOPINVEN_NUM)
	{
		return m_pItemShopInven->AddItem( pItem );
	}
	else
		return FALSE;*/

	//SW061207 풀인벤 경고
	BOOL rt = FALSE;

	if(TP_INVENTORY_START <= pItem->GetPosition() && pItem->GetPosition() < TP_INVENTORY_END)
	{

		// 인벤토리 
		int tabIndex = GetTabIndex( pItem->GetPosition() );
		cIconGridDialog * dlg = (cIconGridDialog *)GetTabSheet(tabIndex);

		//		WORD relPos = pItem->GetPosition()-INVENTORY_STARTPOSITION-CELLNUMPERPAGE*tabIndex;
		WORD relPos = pItem->GetPosition()-TP_INVENTORY_START-TABCELL_INVENTORY_NUM*tabIndex;
		CItem * item = (CItem * )dlg->GetIconForIdx(relPos);
		if(item)
		{
			ASSERT(0);
			return FALSE;
		}
		else
		{
			rt = dlg->AddIcon(relPos, pItem);
			ITEMMGR->CheckInvenFullForAlert(CItemManager::eCBS_Inven);
		}
	}
	else if(TP_WEAR_START <= pItem->GetPosition() && pItem->GetPosition() < TP_WEAR_END)
	{
		// 장착창
		return m_pWearedDlg->AddItem(pItem->GetPosition()-TP_WEAR_START, pItem);
	}
	else if(TP_SHOPINVEN_START <= pItem->GetPosition() && pItem->GetPosition() < TP_SHOPINVEN_END-TABCELL_SHOPINVEN_NUM)
	{
		rt = m_pItemShopInven->AddItem( pItem );
		ITEMMGR->CheckInvenFullForAlert(CItemManager::eCBS_SInven);
	}
	else
		return FALSE;

	return rt;
}

BOOL CInventoryExDialog::DeleteItem(POSTYPE Pos,CItem** ppItem)
{
//	if(INVENTORY_STARTPOSITION <= Pos && Pos < INVENTORY_ENDPOSITION)
	if(TP_INVENTORY_START <= Pos && Pos < TP_INVENTORY_END)
	{
		int TabIndex = GetTabIndex( Pos );
		cIconGridDialog * Dlg = (cIconGridDialog *)GetTabSheet(TabIndex);
		if(!Dlg->IsAddable(Pos - TP_INVENTORY_START-TABCELL_INVENTORY_NUM*TabIndex))
			return Dlg->DeleteIcon(Pos-TP_INVENTORY_START-TABCELL_INVENTORY_NUM*TabIndex, (cIcon **)ppItem);
//		if(!Dlg->IsAddable(Pos - INVENTORY_STARTPOSITION-CELLNUMPERPAGE*TabIndex))
//			return Dlg->DeleteIcon(Pos-INVENTORY_STARTPOSITION-CELLNUMPERPAGE*TabIndex, (cIcon **)ppItem);
		else
			return FALSE;
	}
//	else if(WEAR_STARTPOSITION <= Pos && Pos < WEAR_ENDPOSITION)
	else if(TP_WEAR_START <= Pos && Pos < TP_WEAR_END)
	{
//		if(!m_pWearedDlg->IsAddable(Pos - WEAR_STARTPOSITION))
//			return m_pWearedDlg->DeleteItem(Pos-WEAR_STARTPOSITION, (cIcon **)ppItem);
		if(!m_pWearedDlg->IsAddable(Pos - TP_WEAR_START))
			return m_pWearedDlg->DeleteItem(Pos-TP_WEAR_START, (cIcon **)ppItem);
		else
		{
			return FALSE;
		}
	}
	else if(TP_SHOPINVEN_START <= Pos && Pos < TP_SHOPINVEN_END)
	{
		if(!m_pItemShopInven->IsAddable(Pos-TP_SHOPINVEN_START))
			return m_pItemShopInven->DeleteItem(Pos, ppItem);
		else
		{
			return FALSE;
		}
	}
	else
		return FALSE;
}
BOOL CInventoryExDialog::UpdateItemDurabilityAdd(POSTYPE absPos, int dur)
{
//	if(INVENTORY_STARTPOSITION <= absPos && absPos < INVENTORY_ENDPOSITION)
	if(TP_INVENTORY_START <= absPos && absPos < TP_INVENTORY_END)
	{
		int TabIndex = GetTabIndex( absPos );
		cIconGridDialog * Dlg = (cIconGridDialog *)GetTabSheet(TabIndex);
//		WORD relPos = absPos-INVENTORY_STARTPOSITION-CELLNUMPERPAGE*TabIndex;
		WORD relPos = absPos-TP_INVENTORY_START-TABCELL_INVENTORY_NUM*TabIndex;
		CItem * item = (CItem * )Dlg->GetIconForIdx(relPos);
		item->SetDurability(item->GetDurability()+dur);
		return TRUE;
	}
	else if(TP_SHOPINVEN_START <= absPos && absPos < TP_SHOPINVEN_END)
	{
		CItem* pItem = (CItem*)m_pItemShopInven->GetIconForIdx(absPos-TP_SHOPINVEN_START);
		if(pItem)
		{
			pItem->SetDurability(pItem->GetDurability()+dur);
			return TRUE;
		}
		else
			return FALSE;
	}
	else
		return FALSE;
}
CItem * CInventoryExDialog::GetItemForPos(POSTYPE absPos)
{
//	if(INVENTORY_STARTPOSITION <= absPos && absPos < INVENTORY_ENDPOSITION)
	if(TP_INVENTORY_START <= absPos && absPos < TP_INVENTORY_END)
	{
		int TabIndex = GetTabIndex( absPos );
		cIconGridDialog * Dlg = (cIconGridDialog *)GetTabSheet(TabIndex);
		return (CItem *)Dlg->GetIconForIdx(absPos-TP_INVENTORY_START-TABCELL_INVENTORY_NUM*TabIndex);
//		return (CItem *)Dlg->GetIconForIdx(absPos-INVENTORY_STARTPOSITION-CELLNUMPERPAGE*TabIndex);
	}
//	else if(WEAR_STARTPOSITION <= absPos && absPos < WEAR_ENDPOSITION)
	else if(TP_WEAR_START <= absPos && absPos < TP_WEAR_END)
	{
//		return (CItem *)m_pWearedDlg->GetIconForIdx(absPos-WEAR_STARTPOSITION);
		return (CItem *)m_pWearedDlg->GetIconForIdx(absPos-TP_WEAR_START);
	}
	else if(TP_SHOPINVEN_START <= absPos && absPos < TP_SHOPINVEN_END)
	{
		return (CItem*)m_pItemShopInven->GetIconForIdx(absPos-TP_SHOPINVEN_START);
	}
	else
		return NULL;
}

void CInventoryExDialog::UseItem(CItem* pItem)
{
	/////임시  /////////
	CObject* pObjectTemp = OBJECTMGR->GetObject(HERO->GetID());
	if(NULL == pObjectTemp)
	{
#ifdef _GMTOOL_
		printf("[ItemUse Object Error] : [%d]\n", pObjectTemp->GetID());
#endif	
		return;
	}
	///////////////////

	ASSERT(pItem);

	if( WINDOWMGR->IsDragWindow() && WINDOWMGR->GetDragDlg() )
	if( WINDOWMGR->GetDragDlg()->GetType() == WT_ITEM)
		return;
	
//KES...
	if( m_bDisable )	return;		//인벤 디스에이블이면 아이템 사용 불가능
	
//KES EXCHANGE 031002
 	if( pItem->IsLocked() ) return;
//	on
	if( HERO->GetState() == eObjectState_Die )	//죽은 상태에서는 아이템 사용 못한다.
		return;

	if(HERO->IsHide())
	if(ITEMMGR->IsUnfitItemInfoListForHide(pItem->GetItemIdx()))
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
		return;
	}

	eITEM_KINDBIT bits = pItem->GetItemKind();

	//SW061019 서바이벌모드
	//if( MAP->CheckMapKindIs(eSurvival) )
	if( MAP->IsMapKind(eSurvivalMap) )
	{
		if( !(bits == eYOUNGYAK_ITEM)
			&& !(bits & eEQUIP_ITEM)
			&& !(bits & eSHOP_ITEM_MAKEUP)
			&& !(bits & eSHOP_ITEM_DECORATION)
			//&& !(bits & eSHOP_ITEM_SUNDRIES)
			&& !(bits & eSHOP_ITEM_EQUIP)
			)
		{
			//SW061019 서바이벌모드 관련
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(786) );	//786 "아이템 사용에 실패하였습니다."
			return;
		}

		if( 8000 <= pItem->GetItemIdx() && pItem->GetItemIdx() <= 8013 )
		{
			if( SVVMODEMGR->GetCurModeState() == eSVVMode_Fight )
			if( FALSE == SVVMODEMGR->AddItemUsingCount() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(932) );	//932 "이용할 수 없습니다."
				return;
			}
		}
	}

	if(bits & eYOUNGYAK_ITEM)
	{
		// if eventmap
		if( GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter )
			return;

		if( bits == eYOUNGYAK_ITEM_PET )
		{
			//if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
			//	return;		

			//펫 소환 체크
			//if( HEROPET == NULL )
			if( PETMGR->GetCurSummonPet() == NULL )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1263));
				return;
			}
			//펫 스태미나 풀 체크	//소비가능
// 			if( PETMGR->IsCurPetStaminaFull() )
// 			{
// 				return;
// 			}
		}
		//아이템몰로 기획변경	06/02/22
		else if( bits == eYOUNGYAK_ITEM_UPGRADE_PET )
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
				return;

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );

			GAMEIN->GetPetUpgradeDialog()->SetUpgradeItemInfo(pItem->GetPosition(),pItem->GetItemIdx());
			
			BOOL Grade = FALSE;

			if( pItem->GetItemIdx() == 36001 )	Grade = ePUDG_Grade2;
			else if( pItem->GetItemIdx() == 36002 )	Grade = ePUDG_Grade3;
			GAMEIN->GetPetUpgradeDialog()->SetActive(Grade);
			return;
		}

		else if( bits == eYOUNGYAK_ITEM_REVIVAL_PET )
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
				return;

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );

			GAMEIN->GetPetRevivalDialog()->SetShopItemInfo(pItem->GetItemIdx(),pItem->GetPosition());

			BOOL Grade = FALSE;

			if( pItem->GetItemIdx() == 36003 )	Grade = ePRDG_Grade1;
			else if( pItem->GetItemIdx() == 36004 )	Grade = ePRDG_Grade2;
			else if( pItem->GetItemIdx() == 36005 )	Grade = ePRDG_Grade3;

			GAMEIN->GetPetRevivalDialog()->SetActiveWithGradeTxt(Grade);
			return;
		}

		else if( bits == eYOUNGYAK_ITEM_TITAN )
		{
			if( FALSE == HERO->InTitan() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1655));
				return;
			}

            DWORD dwRemainTime = 0;
			float delayRate = HERO->GetDelayGroup()->CheckTitanPotionDelay(&dwRemainTime );
			if( delayRate > 0.0f )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(909));
				return;
			}
		}

		if(pItem->GetItemIdx() == 10700)
		{
			if(HERO->GetSingleSpecialState(eSingleSpecialState_Detect))
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1361));
				return;
			}
		}
#ifdef _TL_LOCAL_
		//태국은 마을에서 비무시에 물약 못먹는다.

		if( MAP->IsVillage() )
		if( BATTLESYSTEM->GetBattle(HERO)->GetBattleKind() == eBATTLE_KIND_VIMUSTREET 
			|| PARTYWAR->IsPartyWar() )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1289) );
			return;
		}

#endif	//--;  펫아이템도 못먹네 비무시.. 어케 해결할까나?
		// 아이템의 사용
		MSG_ITEM_USE_SYN msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_SYN;
		msg.dwObjectID = HEROID;
		msg.TargetPos = pItem->GetPosition();
		msg.wItemIdx = pItem->GetItemIdx();
		/*
		msg.wTableIdx = eItemTable_Inventory;
		msg.wIconIdx = pItem->GetItemIdx();
		msg.wAbsPosition = pItem->GetPosition();
		*/

		NETWORK->Send(&msg,sizeof(msg));

		// debug용
		if( ITEMMGR->m_nItemUseCount != 0 )
		{
			char str[256] ={0,};
			sprintf( str, "ItemIdx = %d 사용시 아이템(%s) 사용횟수(%d)가 잘못되었습니다.", pItem->GetItemIdx(), pItem->GetItemInfo()->ItemName, ITEMMGR->m_nItemUseCount );
			WRITEDEBUGFILE( str );
		}
		ITEMMGR->m_nItemUseCount++;
		//

#ifdef TAIWAN_LOCAL
		if( pItem->GetItemIdx() == 53074 )	//중국 이벤트 임시!! event temp
		{
			CHATMGR->AddMsg( CTC_CHEAT_1, "8888888888888888888888888888888888888888888888888888888888" );
			CHATMGR->AddMsg( CTC_CHEAT_1, "8 \" 카窮禱晷쌘景깎룽窘，                                 8" );
			CHATMGR->AddMsg( CTC_CHEAT_1, "8 更嶠鑒쯤龍퀭뵨소훙툉堂湛陶考考途途북북쳄쳄! \"          8" );
			CHATMGR->AddMsg( CTC_CHEAT_1, "8888888888888888888888888888888888888888888888888888888888" );
		}
#endif
	}
	else if(bits & eCHANGE_ITEM)	// change item
	{
		if( bits == eCHANGE_ITEM_LOCK )
		{
			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( pItem->GetItemIdx() );
			if( !pInfo )	return;
			if( HERO->GetLevel() < pInfo->ItemType )	return;
		}
		MSG_ITEM_USE_SYN msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_CHANGEITEM_SYN;
		msg.dwObjectID = HEROID;
		msg.TargetPos = pItem->GetPosition();
		msg.wItemIdx = pItem->GetItemIdx();
	
		NETWORK->Send(&msg,sizeof(msg));
	}
	else if(bits & eEQUIP_ITEM)
	{
		if( OBJECTSTATEMGR->CheckAvailableState(HERO,eCAS_EQUIPITEM) == FALSE )
		{
			CAction act;
			act.InitAction(eActionKind_EquipItem,(void*)pItem->GetDBIdx(),0,NULL);
			HERO->SetNextAction(&act);
			return;
		}

		// 장착 아이템	
		switch( GetTableIdxPosition( pItem->GetPosition() ) )
		{
		case eItemTable_Inventory:
			{
				if(ITEMMGR->CanEquip(pItem->GetItemIdx()))
				{
					ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
					if(!pInfo) return ;
					
					// 반지2처리
					WORD EquipToPos = pInfo->EquipKind;
					if(pInfo->EquipKind == eWearedItem_Ring1)
					{
//						if(IsExist(EquipToPos+WEAR_STARTPOSITION))
						if(IsExist(EquipToPos+TP_WEAR_START))
						{
//							if(!IsExist(EquipToPos+1+WEAR_STARTPOSITION))
							if(!IsExist(EquipToPos+1+TP_WEAR_START))
								EquipToPos++;
						}
					}
					CItem * pToItem = GetItemForPos( EquipToPos + TP_WEAR_START );
					FakeGeneralItemMove(EquipToPos + TP_WEAR_START, pItem, pToItem);
					/*
					MSG_ITEM_MOVE_SYN msg;
					msg.Category			= MP_ITEM;
					msg.Protocol			= MP_ITEM_MOVE_SYN;
					msg.dwObjectID			= HEROID;

					msg.FromPos				= pItem->GetPosition();
					msg.wFromItemIdx		= pItem->GetItemIdx();
					msg.ToPos				= EquipToPos + TP_WEAR_START;
					msg.wToItemIdx			= 0;

					if(msg.FromPos == msg.ToPos)
						return;
					NETWORK->Send(&msg,sizeof(msg));
					*/
				}
			}
			break;
		case eItemTable_Weared:
			{
				POSTYPE emptyPos;
				if( GetBlankPositionRestrictRef( emptyPos ) )
				{
					CItem * pToItem = GetItemForPos( emptyPos );
					FakeGeneralItemMove(emptyPos, pItem, pToItem);
					/*
					
					MSG_ITEM_MOVE_SYN msg;
					msg.Category			= MP_ITEM;
					msg.Protocol			= MP_ITEM_MOVE_SYN;
					msg.dwObjectID			= HEROID;

					msg.FromPos				= pItem->GetPosition();
					msg.wFromItemIdx		= pItem->GetItemIdx();
					msg.ToPos				= emptyPos;
					msg.wToItemIdx			= 0;

					if(msg.FromPos == msg.ToPos)
						return;

					NETWORK->Send(&msg,sizeof(msg));*/
					
				}
			}
			break;
		}
		
	}
	else if(bits & eMUGONG_ITEM)
	{
		//if(!GAMEIN->GetMugongDialog()->CanBeMoved())
		//	return;

		//2007 7. 2. CBH - 전문기술 관련 추가
		//서적이 전문기술 서적이면 스킬 슬롯에서 검색하여 다른 전문기술이 있는 경우 사용 못하게 막는다. (메세지처리)
		if(bits == eMUGONG_ITEM_JOB)
		{
			if(!MUGONGMGR->SearchJobSkill())
			{
				CHATMGR->AddMsg(CTC_SYSMSG, "전문기술이 이미 있습니다");
				return;
			}
		}

		if(pItem->GetItemIdx() == 10750)
		{
			GAMEIN->GetSkillOptionClearDlg()->SetItem(pItem);
			GAMEIN->GetSkillOptionClearDlg()->SetActive(TRUE);
			return;
		}
		if(MUGONGMGR->GetMugongByMugongIdx(pItem->GetItemInfo()->MugongNum) != NULL)
				return ;

		SKILL_CHANGE_INFO * pSkillChangeInfo = SKILLMGR->GetSkillChangeInfo(pItem->GetItemInfo()->MugongNum);
		if(pSkillChangeInfo)
		{
			if(MUGONGMGR->GetMugongByMugongIdx(pSkillChangeInfo->wTargetMugongIdx))
				return;
		}

		// magi82 - Titan(070914) 타이탄 무공업데이트
		if( bits == eMUGONG_ITEM_TITAN )
		{
			if( HERO->InTitan() == FALSE )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1653));
				return;
			}
		}

		if(bits == eMUGONG_ITEM_JINBUB)
		{
			if(!ITEMMGR->CanConvertToMugong(pItem->GetItemIdx(),MUGONGTYPE_JINBUB))
				return;
		}
		else if(bits == eMUGONG_ITEM_KYUNGGONG)		// 경공일 경우만 경공메니져에서 처리한다.
		{
			//if(!ITEMMGR->CanConvertToMugong(pItem->GetItemIdx(),MUGONGTYPE_KYUNGGONG))
			//	return;
			if(KYUNGGONGMGR->IsSetableKyungGong(HERO,pItem) == FALSE)
				return;
		}
		else if(bits == eMUGONG_ITEM_JOB)	//2007. 10. 28. CBH - 전문기술 무공 추가
		{
			if(!ITEMMGR->CanConvertToMugong(pItem->GetItemIdx(), MUGONGTYPE_JOB))
				return;
		}
		else
		{
			// 무공 등록
			if(!ITEMMGR->CanConvertToMugong(pItem->GetItemIdx(),MUGONGTYPE_NORMAL))
				return;				
		}

		MSG_ITEM_USE_SYN msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_SYN;
		msg.dwObjectID = HEROID;
		msg.wItemIdx = pItem->GetItemIdx();
		msg.TargetPos = pItem->GetPosition();

		NETWORK->Send(&msg,sizeof(msg));

		// debug용
		ITEMMGR->m_nItemUseCount++;
	}
	else if(bits & eEXTRA_ITEM)
	{
		/*
		//현재 : 아이템 등급업에 따른 비급서 사용
		if(m_iInventoryState == eITEMUPGRADE_STATE)
		{
//			GAMEIN->GetChatDialog()->AddChatMsg(WHOLE,RGBA_MAKE(255,0,0,255), "현재 비급서를 사용중입니다.");
			CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(171) );
			return;
		}
		
		if(bits == eEXTRA_ITEM_UPGRADE || bits == eEXTRA_ITEM_UPGRADE+1)
		{		
			MSG_ITEM_USE_SYN msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_USE_SYN;
			msg.dwObjectID = HEROID;
			msg.wItemIdx = pItem->GetItemIdx();
			msg.TargetPos = pItem->GetPosition();
		
			NETWORK->Send(&msg,sizeof(msg));
		}*/
		
		/* 더블클릭으로 사용되어져서 SkillManager 로 이동
		// 2005 크리스마스 이벤트
		if(pItem->GetItemIdx() == EVENT_ITEM_SNOW)
		{
			MSG_ITEM_USE_SYN msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_USE_SYN;
			msg.dwObjectID = HEROID;
			msg.wItemIdx = pItem->GetItemIdx();
			msg.TargetPos = pItem->GetPosition();

			NETWORK->Send(&msg,sizeof(msg));
		}
		*/
/*
		if(bits & eEXTRA_ITEM_USABLE)
		{	
		*/
		if(pItem->GetItemIdx() == 50510)
		{
			GAMEIN->GetGuildNoteDlg()->Show(pItem);
		}

		if(pItem->GetItemIdx() == 50511)
		{
			GAMEIN->GetUnionNoteDlg()->Show(pItem);
		}

		/*
			MSG_ITEM_USE_SYN msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_USE_SYN;
			msg.dwObjectID = HEROID;
			msg.wItemIdx = pItem->GetItemIdx();
			msg.TargetPos = pItem->GetPosition();

			NETWORK->Send(&msg,sizeof(msg));
		}
*/
	}
	else if(bits & eEXTRA_ITEM_UPGRADE)
	{

	}
	else if(bits & eQUEST_ITEM)
	{
		if( bits == eQUEST_ITEM_START )			// 시작 퀘스트 아이템.
		{
			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( pItem->GetItemIdx() );
			if( !pInfo )
				return;
			if( !QUESTMGR->IsQuestStarted( pInfo->ItemType ) )
				m_pQuestMsgBox = WINDOWMGR->MsgBox(MBI_QUEST_NONE, MBT_OK, CHATMGR->GetChatMsg(651));
			else
				m_pQuestMsgBox = WINDOWMGR->MsgBox(MBI_QUESTSTART, MBT_YESNO, CHATMGR->GetChatMsg(650));

			m_pTempQuestItem = pItem;
		}
		else if( bits == eQUEST_ITEM_PET )	//퀘스트에서 얻는 펫 아이템.
		{
			if(2 == HERO->GetBattleTeam())
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1279) );
				return;
			}

			////////////////////////////////////////////////////////////////////////////////
			// 06. 06. 2차 전직 - 이영준
			// 은신 / 혜안
			// 은신 패널티
			if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
				return;
			}
			////////////////////////////////////////////////////////////////////////////////

			ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( pItem->GetItemIdx() );
			if( !pInfo )
				return;

			//재소환 유효시간 체크
			if( FALSE == PETMGR->CheckResummonAvailable() )
			{
				//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1254) );
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1254), DWORD(PETMGR->GetPetResummonRestTime()/1000) );
				return;
			}

			//펫 소환중 다른펫 소환 불가
			if( PETMGR->GetCurSummonPet() && FALSE == PETMGR->IsCurPetSummonItem(pItem->GetDBIdx()) )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1255) );
				return;
			}

			//살아있는 펫 인지 체크
			if( FALSE == PETMGR->CheckPetAlive(pItem->GetDBIdx()) )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1249));
				return;
			}
			//사용(소환)레벨 제한 체크
			if(pInfo->LimitLevel > HERO->GetMaxLevel())
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1242));
				return;
			}

			//캐릭터 이동 정지
			MOVEMGR->HeroMoveStop();

			MSG_ITEM_USE_SYN msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_USE_SYN;
			msg.dwObjectID = HEROID;
			msg.wItemIdx = pItem->GetItemIdx();
			msg.TargetPos = pItem->GetPosition();

			NETWORK->Send(&msg,sizeof(msg));

			// debug용
			ITEMMGR->m_nItemUseCount++;
		}
		else if( bits == eQUEST_ITEM_EQUIP )	// 장착가능한 퀘스트 아이템.
		{
			if( OBJECTSTATEMGR->CheckAvailableState(HERO,eCAS_EQUIPITEM) == FALSE )
			{
				CAction act;
				act.InitAction(eActionKind_EquipItem,(void*)pItem->GetDBIdx(),0,NULL);
				HERO->SetNextAction(&act);
				return;
			}
			
			switch( GetTableIdxPosition( pItem->GetPosition() ) )
			{
			case eItemTable_Inventory:
				{
					if( ITEMMGR->CanEquip( pItem->GetItemIdx() ) )
					{
						ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
						if( !pInfo ) return;

						WORD EquipToPos = pInfo->EquipKind;
						CItem* pToItem = GetItemForPos( EquipToPos + TP_WEAR_START );
						FakeGeneralItemMove(EquipToPos + TP_WEAR_START, pItem, pToItem);
					}
				}	
				break;
			case eItemTable_Weared:
				{
					POSTYPE emptyPos;
					if( GetBlankPositionRestrictRef( emptyPos ) )
					{
						CItem * pToItem = GetItemForPos( emptyPos );
						FakeGeneralItemMove(emptyPos, pItem, pToItem);
					}
				}
				break;
			}
		}
	}
	else if(bits & eSHOP_ITEM)
	{
		if( pItem->GetItemParam() & ITEM_PARAM_SEAL )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(751) );
			return;
		}

		// if eventmap
		if( GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter )
			return;

		ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
		if(!pInfo)		return;

		//2007. 9. 21. CBH - 샾 아이탬 레벨제한 체크 추가
		if(pInfo->NaeRyukRecoverRate > HERO->GetLevel())
		{
			char temp[256] = {0, };
			sprintf( temp, CHATMGR->GetChatMsg(238), (int)pInfo->NaeRyukRecoverRate );
			CHATMGR->AddMsg( CTC_SYSMSG, temp );
			return;
		}


		if( bits == eSHOP_ITEM_EQUIP )
		{
			switch( GetTableIdxPosition( pItem->GetPosition() ) )
			{
			case eItemTable_ShopInven:
				{
					if( ITEMMGR->CanEquip( pItem->GetItemIdx() ) )
					{
						ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
						if(!pInfo) return ;

						// 반지2처리
						WORD EquipToPos = pInfo->EquipKind;
						if(pInfo->EquipKind == eWearedItem_Ring1)
						{
							if(IsExist(EquipToPos+TP_WEAR_START))
							{
								if(!IsExist(EquipToPos+1+TP_WEAR_START))
									EquipToPos++;
							}
						}
						CItem * pToItem = GetItemForPos( EquipToPos + TP_WEAR_START );
						FakeGeneralItemMove(EquipToPos + TP_WEAR_START, pItem, pToItem);
						return;
					}
				}
				break;
			case eItemTable_Weared:
				{
					POSTYPE emptyPos;
					if( m_pItemShopInven->GetBlankPositionRestrictRef( emptyPos ) )
					{
						CItem * pToItem = GetItemForPos( emptyPos );
						FakeGeneralItemMove(emptyPos, pItem, pToItem);
						return;
					}
				}
				break;
			}
		}


		if( pInfo->ItemIdx == eIncantation_MemoryStone )
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
				return;			

			DWORD Validsavenum = MAX_MOVEDATA_PERPAGE;
			if( ITEMMGR->GetUsedItemInfo( eIncantation_MemoryMoveExtend ) ||
				ITEMMGR->GetUsedItemInfo( eIncantation_MemoryMove2 ) ||
				ITEMMGR->GetUsedItemInfo( eIncantation_MemoryMoveExtend7 ) ||
				ITEMMGR->GetUsedItemInfo( eIncantation_MemoryMoveExtend30 ) )
				Validsavenum = MAX_MOVEDATA_PERPAGE*MAX_MOVEPOINT_PAGE;

			if( GAMEIN->GetMoveDialog()->GetSavedCount() >= Validsavenum )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(758) );
				return;
			}

			//if( MAP->GetMapNum() == BOSSMONSTER_MAP || MAP->GetMapNum() == QuestRoom ||
			//	MAP->GetMapNum() == Tournament || MAP->GetMapNum() == BOSSMONSTER_2NDMAP ||
			//	MAP->GetMapNum() == RUNNINGMAP || MAP->GetMapNum() == PKEVENTMAP )
			DWORD dwCheckBit = eBossMap | eQuestRoom | eTournament;
			if( MAP->IsMapKind(dwCheckBit) || MAP->IsMapSame(eRunningMap) )
				return;

			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);

			GAMEIN->GetPointSaveDialog()->SetItemToMapServer( pItem->GetItemIdx(), pItem->GetPosition() );
			GAMEIN->GetPointSaveDialog()->SetDialogStatus( TRUE );
			GAMEIN->GetPointSaveDialog()->SetActive( TRUE );

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
		}
		else if( pInfo->ItemIdx == eIncantation_TownMove || pInfo->ItemIdx == eIncantation_MemoryMove )
		{
			//if( HERO->GetState() != eObjectState_None )
			//SW050907
			if(HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
			{
				return;
			}
			
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);

			GAMEIN->GetMoveDialog()->SetItemToMapServer( pItem->GetItemIdx(), pItem->GetPosition(), TRUE );
			
			if( pInfo->ItemIdx == eIncantation_TownMove )
			{
				GAMEIN->GetMoveDialog()->SetTownMoveView( TRUE );
				GAMEIN->GetMoveDialog()->SetActive( TRUE );
			}
			else
			{
				GAMEIN->GetMoveDialog()->SetTownMoveView( FALSE );

				MSGBASE msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_MPINFO;
				msg.dwObjectID = HERO->GetID();
				NETWORK->Send( &msg, sizeof(msg) );
			}

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
		}
		else if( pInfo->ItemKind == eSHOP_ITEM_INCANTATION && pInfo->LimitLevel )
		{
			//if( HERO->GetState() != eObjectState_None )
			//SW050907 수정
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
			{
				return;
			}


			CObject* pObject = OBJECTMGR->GetSelectedObject();
			if( pObject == NULL || pObject == HERO || pObject->GetObjectKind() != eObjectKind_Player )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(763) );
				return;
			}

			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);

			char buf[128] = { 0, };
			wsprintf( buf, CHATMGR->GetChatMsg(761), pObject->GetObjectName() );
			m_pSelectedShopItem = pItem;

			WINDOWMGR->MsgBox( MBI_OTHERREVIVE, MBT_YESNO, buf );
		}
		else if( pInfo->ItemIdx == eIncantation_CharChange || pInfo->ItemIdx == eIncantation_ShapeChange )
		{			
			/// 2007. 6. 18. CBH - 타이탄맵일때 성별변환 주문서 사용 금지 /////
			if(MAP->IsTitanMap())
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1580) );				
				return;
			}
			////////////////////////////////////////////////////////////////////			

//			if( HERO->GetState() != eObjectState_None )
//				return;
			//SW050907 수정
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
			{
				return;
			}
			
			for(int i=0; i<eAvatar_Effect; ++i)
			{
				if( HERO->GetShopItemStats()->Avatar[i] )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1184) );
					return;
				}
			}

			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);

			GAMEIN->GetCharChangeDlg()->SetItemInfo( pItem->GetPosition() );
			GAMEIN->GetCharChangeDlg()->SetCharacterInfo( HERO->GetCharacterTotalInfo() );
			if( pInfo->ItemIdx == eIncantation_CharChange )
				GAMEIN->GetCharChangeDlg()->SetShapeChange( FALSE );
			else
				GAMEIN->GetCharChangeDlg()->SetShapeChange( TRUE );
			GAMEIN->GetCharChangeDlg()->SetActive( TRUE );

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
			
		}
		else if( pInfo->ItemIdx == eIncantation_ItemSeal )
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
			{
				return;
			}

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );

			if( GAMEIN->GetSealDlg() )
			{
				GAMEIN->GetSealDlg()->SetItemInfo( pItem->GetItemIdx(), pItem->GetPosition() );
				GAMEIN->GetSealDlg()->SetActive( TRUE );
			}
		}
		else if( pInfo->ItemIdx == eIncantation_ReinforceReset )
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
			{
				return;
			}

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );

			if( GAMEIN->GetReinforceResetDlg() )
			{
				GAMEIN->GetReinforceResetDlg()->SetItemInfo( pItem->GetItemIdx(), pItem->GetPosition() );
				GAMEIN->GetReinforceResetDlg()->SetActive( TRUE );
			}
		}		
		else if( pInfo->ItemKind == eSHOP_ITEM_MAKEUP || pInfo->ItemKind == eSHOP_ITEM_DECORATION )
		{
			AVATARITEM* pAvatarItem = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pInfo->ItemIdx );
			if(!pAvatarItem)
				return;

			if( pAvatarItem->Gender < 2 && pAvatarItem->Gender != HERO->GetGender() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(778) );
				return;
			}			
			else
			{
				// 아이템의 사용
				if( HERO->GetState() == eObjectState_Move )
					MOVEMGR->HeroMoveStop();
				//else if( HERO->GetState() != eObjectState_None )
				//SW050907 수정
				else if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(798) );
					return;
				}

				MSG_WORD2 msg;
				msg.Category = MP_ITEM;
				if( pItem->GetUseParam() )
					msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_TAKEOFF;
				else
					msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_USE_SYN;
				msg.dwObjectID = HEROID;
				msg.wData1 = pItem->GetItemIdx();
				msg.wData2 = pItem->GetPosition();				
				NETWORK->Send(&msg,sizeof(msg));
			}
		}
		else if( pInfo->ItemKind == eSHOP_ITEM_INCANTATION && pInfo->Life )
		{
			//if( HERO->GetState() != eObjectState_None )
			//SW050907 수정
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
			{
				GAMEIN->GetSkPointDlg()->SetCurItem( 0, 0 );
				return;
			}
			
			GAMEIN->GetSkPointDlg()->SetCurItem( pItem->GetItemIdx(), pItem->GetPosition() );
			
			char buf[128];
			sprintf( buf, CHATMGR->GetChatMsg(737), pInfo->Life );
			WINDOWMGR->MsgBox( MBI_SKPOINTREDIST_USE, MBT_YESNO, buf );			
		}
		else if( pInfo->ItemIdx == eIncantation_TownMove15 ||
			pInfo->ItemIdx == eIncantation_MemoryMove15 ||
			pInfo->ItemIdx == eIncantation_TownMove7 ||
			pInfo->ItemIdx == eIncantation_TownMove7_NoTrade ||
			pInfo->ItemIdx == eIncantation_MemoryMove7 ||
			pInfo->ItemIdx == eIncantation_MemoryMove7_NoTrade ||
			pInfo->ItemIdx == 55357 ||
			pInfo->ItemIdx == 55362 ||
			pInfo->ItemIdx == eIncantation_MemoryMoveExtend ||
			pInfo->ItemIdx == eIncantation_MemoryMove2 ||
			pInfo->ItemIdx == eIncantation_MemoryMoveExtend7 || 
			pInfo->ItemIdx == eIncantation_MemoryMoveExtend30 )
		{
//			if( HERO->GetState() != eObjectState_None )
//				return;
			//SW050907 수정
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
				return;
			
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);
			
			GAMEIN->GetMoveDialog()->SetItemToMapServer( pItem->GetItemIdx(), pItem->GetPosition(), TRUE );
			
			if( pInfo->ItemIdx == eIncantation_TownMove15 ||
				pInfo->ItemIdx == eIncantation_TownMove7 || 
				pInfo->ItemIdx == eIncantation_TownMove7_NoTrade )
			{
				GAMEIN->GetMoveDialog()->SetTownMoveView( TRUE );
				GAMEIN->GetMoveDialog()->SetActive( TRUE );
			}
			else
			{
				GAMEIN->GetMoveDialog()->SetTownMoveView( FALSE );

				MSGBASE msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_MPINFO;
				msg.dwObjectID = HERO->GetID();
				NETWORK->Send( &msg, sizeof(msg) );
			}

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
		}
		else if( pInfo->ItemIdx == eIncantation_ShowPyoguk
				|| pInfo->ItemIdx == eIncantation_ShowPyoguk7
				|| pInfo->ItemIdx == eIncantation_ShowPyoguk7_NoTrade )
		{
			//if( HERO->GetState() != eObjectState_None )
			//SW050907 수정
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
				return;

			NPCSCRIPTMGR->StartNpcBusiness( 0, eNpcParam_ShowpPyoguk );
		}
		else if( pInfo->ItemIdx == eSundries_RareItemCreate50 ||
			pInfo->ItemIdx == eSundries_RareItemCreate70 ||
			pInfo->ItemIdx == eSundries_RareItemCreate90 ||
			pInfo->ItemIdx == eSundries_RareItemCreate99)
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
				return;

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );

			GAMEIN->GetRareCreateDlg()->SetShopItemInfo( pItem->GetItemIdx(), pItem->GetPosition() );
			GAMEIN->GetRareCreateDlg()->SetActive( TRUE );
			//pItem->SetLock( TRUE );

		}
		else if( pInfo->ItemIdx == eIncantation_PetRevival )
		{//펫 부활 주문서
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
				return;

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );

			GAMEIN->GetPetRevivalDialog()->SetShopItemInfo( pItem->GetItemIdx(), pItem->GetPosition() );
			GAMEIN->GetPetRevivalDialog()->SetActive( TRUE );
		}
		else if( pInfo->ItemIdx == eIncantation_PetGrade2
			|| pInfo->ItemIdx == eIncantation_PetGrade3 )
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
				return;

			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );

			GAMEIN->GetPetUpgradeDialog()->SetUpgradeItemInfo(pItem->GetPosition(),pItem->GetItemIdx());

			BOOL Grade = FALSE;

			if( pItem->GetItemIdx() == eIncantation_PetGrade2 )	Grade = ePUDG_Grade2;
			else if( pItem->GetItemIdx() == eIncantation_PetGrade3 )	Grade = ePUDG_Grade3;
			GAMEIN->GetPetUpgradeDialog()->SetActive(Grade);
			return;
		}
		else if( pInfo->ItemKind == eSHOP_ITEM_SUNDRIES && pInfo->GenGol )
		{
			// 강화보호
			if( pItem->GetItemIdx() >= 55633 && pItem->GetItemIdx() <= 55635 )
			{
				GAMEIN->GetReinforceDialog()->Release();
				GAMEIN->GetReinforceDialog()->SetActiveRecursive( FALSE );
				GAMEIN->GetReinforceDialog()->SetShopItemInfo( pItem->GetItemIdx(), pItem->GetPosition() );
				GAMEIN->GetReinforceDialog()->SetActiveRecursive( TRUE );
				pItem->SetLock( TRUE );
			}
			// 조합보호
			if( pItem->GetItemIdx() >= 55636 && pItem->GetItemIdx() <= 55638 )
			{
				GAMEIN->GetMixDialog()->Release();
				GAMEIN->GetMixDialog()->SetActiveRecursive( FALSE );
				GAMEIN->GetMixDialog()->SetShopItemInfo( pItem->GetItemIdx(), pItem->GetPosition() );
				GAMEIN->GetMixDialog()->SetActiveRecursive( TRUE );
				pItem->SetLock( TRUE );
			}
			return;
		}
		else if( pInfo->ItemKind == eSHOP_ITEM_SUNDRIES && pInfo->CheRyuk )
		{
			if(GAMEIN->GetGameInInitKind() == eGameInInitKind_SuryunEnter || 
				GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
				return;

			if( ( HERO->GetState() == eObjectState_None || HERO->GetState() == eObjectState_Immortal )
					&& !VIMUMGR->IsVimuing() )
			{
				STREETSTALLMGR->ChangeDialogState();
			}
		}
		else if( pInfo->ItemIdx == eIncantation_ChangeJob )
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
				return;
			if( HERO->GetStage() != eStage_Hwa && HERO->GetStage() != eStage_Geuk )
				return;
		
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);
			
			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
			GAMEIN->GetChangeJobDlg()->SetActive( TRUE );
			GAMEIN->GetChangeJobDlg()->SetItemInfo( pItem->GetPosition(), pItem->GetDBIdx() );
		}
		else if( pInfo->ItemIdx == eIncantation_ChangeName || pInfo->ItemIdx == eIncantation_ChangeName_Dntrade )
		{				
			//if( HERO->IsPKMode() || VIMUMGR->IsVimuing() || PKMGR->IsPKLooted() || HERO->GetState() != eObjectState_None )
			//SW050907 수정
			if( HERO->IsPKMode() || VIMUMGR->IsVimuing() || PKMGR->IsPKLooted() || ( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal ) )
				return;

			GAMEIN->GetNameChangeNotifyDlg()->SetActive( TRUE );
			GAMEIN->GetInventoryDialog()->SetDisable( TRUE );
		}
#ifdef _JAPAN_LOCAL_
		else if( pInfo->ItemIdx == eIncantation_PyogukExtend && HERO->GetExtraPyogukSlot() >= MAX_PYOGUKLIST_NUM - GIVEN_PYOGUK_SLOT )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1304) );
			return;
		}
#endif
#ifdef _HK_LOCAL_
		else if( pInfo->ItemIdx == eIncantation_PyogukExtend && HERO->GetExtraPyogukSlot() >= MAX_PYOGUKLIST_NUM - GIVEN_PYOGUK_SLOT )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1304) );
			return;
		}
#endif
#ifdef _TL_LOCAL_
		else if( pInfo->ItemIdx == eIncantation_PyogukExtend && HERO->GetExtraPyogukSlot() >= MAX_PYOGUKLIST_NUM - GIVEN_PYOGUK_SLOT )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1304) );
			return;
		}
		else if( pInfo->ItemIdx == eIncantation_MugongExtend && HERO->GetExtraMugongSlot() >= 2 )
		{
			CHATMGR->AddMsg( CTC_ALERT_YELLOW, CHATMGR->GetChatMsg(1435) );
			return;
		}
#endif
		else if( pInfo->ItemIdx == eSundries_Shout || pInfo->ItemIdx == eSundries_Shout_Once 
			|| pInfo->ItemIdx == eSundries_Shout_Once_NoTrade )
		{
			GAMEIN->GetShoutDlg()->SetActive( TRUE );
			GAMEIN->GetShoutDlg()->SetItemInfo( pInfo->ItemIdx, pItem->GetPosition() );
			return;
		}
		else if( pInfo->ItemIdx == eIncantation_MugongOptionReset )
		{
			if( GAMEIN->GetSkillOptionClearDlg()->IsActive() )
				return;

			pItem->SetLock( TRUE );
			GAMEIN->GetSkillOptionClearDlg()->SetItem( pItem );
			GAMEIN->GetSkillOptionClearDlg()->SetActive( TRUE );
			return;
		}
		else if( pInfo->ItemKind == eSHOP_ITEM_PET_EQUIP )
		{
			//펫소환상태 확인하고
			if( PETMGR->GetCurSummonPet() == NULL )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1263));
				return;
			}
			//펫인벤 활성화 확인하고
//			SW060424 기획팀장님 요청
//			if( FALSE == ((cWindow*)(GAMEIN->GetPetInventoryDialog()))->IsActive() )
//			{
//				return;
//			}
			//빈 장착 슬롯 포즈값 가져와
			CPetWearedExDialog* pPetEquipDlg = ((CPetInventoryDlg*)GAMEIN->GetPetInventoryDialog())->GetPetWearedDlg();

			//이동시켜준다.
			POSTYPE	EmptyPos = 0;
			if(pPetEquipDlg->GetBlankPositionRestrictRef(EmptyPos))
			{
				CItem* pToItem = GAMEIN->GetPetInventoryDialog()->GetItemForPos(EmptyPos);
				FakeGeneralItemMove(EmptyPos, pItem, pToItem);
				return;
			}
			else
			{
				//펫장착슬롯이 Full 이라는 메시지
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1287) );
				return;
			}
		}
		// magi82 - UniqueItem(070709)
		else if(pInfo->ItemIdx == eIncantation_Curse_Cancellation)
		{
			GAMEIN->GetUniqueItemCurseCancellationDlg()->SetShopItemPosion(pItem->GetPosition());

			GAMEIN->GetUniqueItemCurseCancellationDlg()->SetActive( TRUE );
			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
		}
		// magi82(26)
		else if( pInfo->ItemKind == eSHOP_ITEM_TITAN_EQUIP )
		{
			if( HERO->InTitan() == FALSE )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1679) );
				return;
			}

			switch( GetTableIdxPosition(pItem->GetPosition()) )
			{
			case eItemTable_ShopInven:
				{
					POSTYPE	EmptyPos = 0;
					if(GAMEIN->GetTitanInventoryDlg()->GetBlankSlotPosition(EmptyPos))
					{
						CItem* pToItem = GAMEIN->GetTitanInventoryDlg()->GetItemForPos(EmptyPos);
						FakeGeneralItemMove(EmptyPos, pItem, pToItem);
						return;
					}
					else
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1680) );
						return;
					}
				}
				break;
			case eItemTable_TitanShopItem:
				{
					POSTYPE EmptyPos = 0;
					if( m_pItemShopInven->GetBlankPositionRestrictRef(EmptyPos) == TRUE )
					{
						CItem* pToItem = GetItemForPos(EmptyPos);
						FakeGeneralItemMove(EmptyPos, pItem, pToItem);
					}
				}
				break;
			}
		}		
		else if(pInfo->ItemKind == eSHOP_ITEM_NOMALCLOTHES_SKIN)	//2007. 12. 6. CBH - 아이탬몰 의복 스킨 아이탬 사용시 처리
		{
			if( GAMEIN->GetSkinSelectDlg()->IsActive() == FALSE )
				GAMEIN->GetSkinSelectDlg()->SetActive(TRUE);

			return;
		}
		else if(pInfo->ItemKind == eSHOP_ITEM_COSTUME_SKIN)	//2008. 1. 22. CBH - 코스튬 스킨 아이탬 사용시 처리
		{
			if( GAMEIN->GetCostumeSkinSelectDlg()->IsActive() == FALSE )
				GAMEIN->GetCostumeSkinSelectDlg()->SetActive(TRUE);

			return;
		}
		// magi82(41) - 샵아이템 추가(스텟 초기화 주문서)
		// magi82(45) - 스텟 초기화 주문서에 레벨제한을 한다.
		else if(pInfo->ItemIdx == eIncantation_StatusPointReset_71 ||
				pInfo->ItemIdx == eIncantation_StatusPointReset_1_50 ||
				pInfo->ItemIdx == eIncantation_StatusPointReset_51_70)
		{
			if( HERO->GetShopItemStats()->UseStatePoint > 0 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1688) );
				return;
			}

			int nMaxLevel = HERO->GetMaxLevel();
			if( nMaxLevel < pInfo->RangeType || nMaxLevel > pInfo->EquipKind )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1082) );
				return;
			}

			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
			ITEMMGR->SetDisableDialog(TRUE, eItemTable_Shop);

			m_pSelectedShopItem = pItem;
			WINDOWMGR->MsgBox(MBI_STATUS_POINT_RESET, MBT_YESNO, CHATMGR->GetChatMsg(1683));
			return;
		}
		// magi82(42) - 샵아이템 추가(수련치 초기화)
		else if(pInfo->ItemIdx == eIncantation_SkillPoint_Reset)
		{
			if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(734) );
				return;
			}

			GAMEIN->GetSkillPointResetDlg()->SetShopItemPos(pItem->GetPosition());
			GAMEIN->GetSkillPointResetDlg()->SetActive( TRUE );
			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
		}
		else
		{
			if( pInfo->ItemKind == eSHOP_ITEM_CHARM && pInfo->EquipKind )
			{
				if( HERO->GetMaxLevel() < pInfo->EquipKind )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(931) );
					return;
				}
			}
			else if( pInfo->ItemKind == eSHOP_ITEM_INCANTATION && pInfo->LimitJob )
			{
				// 레벨 제한 해제
				if( pInfo->LimitGender == 0 && HERO->GetLevel() > 50 )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1082) );
					return;
				}
				else if( pInfo->LimitGender == 1 && (HERO->GetLevel() <51 || HERO->GetLevel() > 70 ) )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1082) );
					return;
				}
				else if( pInfo->LimitGender == 2 && (HERO->GetLevel() <71 || HERO->GetLevel() > 90 ) )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1082) );
					return;
				}
			}

			if( pInfo->ItemKind == eSHOP_ITEM_PET )
			{
				if(2 == HERO->GetBattleTeam())
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1279) );
					return;
				}

				////////////////////////////////////////////////////////////////////////////////
				// 06. 06. 2차 전직 - 이영준
				// 은신 / 혜안
				// 은신 패널티
				if( HERO->GetSingleSpecialState(eSingleSpecialState_Hide) )
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
					return;
				}
				////////////////////////////////////////////////////////////////////////////////
				
				//재소환 유효시간 체크
				if( FALSE == PETMGR->CheckResummonAvailable() )
				{
					//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1254) );
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1254), DWORD(PETMGR->GetPetResummonRestTime()/1000) );
					return;
				}

				//펫 소환중 다른펫 소환 불가
				if( PETMGR->GetCurSummonPet() && FALSE == PETMGR->IsCurPetSummonItem(pItem->GetDBIdx()) 
					&& !PETMGR->CheckCurSummonPetKindIs(ePK_EventPet) )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1255) );
					return;
				}

				//살아있는 펫 인지 체크
				if( FALSE == PETMGR->CheckPetAlive(pItem->GetDBIdx()) )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1249));
					return;
				}
				//사용(소환)레벨 제한 체크
				if(pInfo->EquipKind > HERO->GetMaxLevel())
				{
					return;
				}
				
				//캐릭터 이동 정지
				MOVEMGR->HeroMoveStop();
			}

			// 아이템의 사용
			MSG_SHOPITEM_USE_SYN msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_USE_SYN;
			msg.dwObjectID = HEROID;
			msg.UseBaseInfo.TargetItemPos = pItem->GetPosition();
			msg.UseBaseInfo.TargetItemIdx = pItem->GetItemIdx();
			msg.UseBaseInfo.ShopItemPos = pItem->GetPosition();
			msg.UseBaseInfo.ShopItemIdx = pItem->GetItemIdx();
			
			NETWORK->Send(&msg,sizeof(msg));
		}
	}
	// magi82 - Titan(070213)
	else if(bits & eTITAN_EQUIPITEM)
	{		
		//if((GAMEIN->GetTitanInventoryDlg()->IsActive()) && (HERO->InTitan()))
		if( HERO->InTitan() )
		{
			// 장착 아이템	
			switch( GetTableIdxPosition( pItem->GetPosition() ) )
			{
			case eItemTable_Inventory:
				{
					ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
					if(!pInfo) return ;					

					DWORD EquipToPos = pInfo->ItemKind - eTITAN_EQUIPITEM_HELMET;

					CItem * pToItem = GAMEIN->GetTitanInventoryDlg()->GetItemForPos( (POSTYPE)(EquipToPos + TP_TITANWEAR_START) );
					FakeGeneralItemMove((POSTYPE)(EquipToPos + TP_TITANWEAR_START), pItem, pToItem);
				}
				break;
			case eItemTable_Titan:
				{
					POSTYPE emptyPos;
					if( GetBlankPositionRestrictRef( emptyPos ) )
					{
						CItem * pToItem = GetItemForPos( emptyPos );
						FakeGeneralItemMove(emptyPos, pItem, pToItem);
					}
				}
				break;
			}
		}
		else
		{
			//2007. 10. 25. CBH - 타이탄 탑승 하지 않았을때 타이탄 장비 착용시 메세지
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1668));
		}
	}
	// magi82 - Titan(070319)
	else if( bits & eTITAN_ITEM )
	{
		if( eTITAN_ITEM_PAPER == bits )
		{
			// 현재 캐릭터의 상태
			if(HERO->GetState() != eObjectState_None)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1548));
				return;
			}

			//사용등록된 인증서인지
			if( FALSE == TITANMGR->IsRegistedTitan(pItem->GetDBIdx()) )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1510) );
				return;
			}

			//현재 탑승중인지
			if( TITANMGR->GetCurRidingTitan() && TITANMGR->GetCallItemDBIdx() != pItem->GetDBIdx() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1549) );
				return;
			}
			
			if( NULL == TITANMGR->GetCurRidingTitan() )
			{
				//재소환 딜레이 검사
				if( FALSE == TITANMGR->CheckRecallAvailable() )
				{
					stTIME stCurTime = TITANMGR->GetTitanResummonTime();
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1545), GetCurTimeToString(&stCurTime) );
					return;
				}

				//"체력이 부족합니다"
				if( FALSE == TITANMGR->CheckMasterLifeAmountForRiding() )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(403) );
					return;
				}
			}

			//2007. 10. 19. CBH - 무쌍모드일때 타이탄 탑승 실패
			if(MUSSANGMGR->IsMussangMode() == TRUE)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1668) );
				return;
			}

			// 070621 기획변경
			/*
			// 장착템 검사
			// 갯수
			if( FALSE == TITANMGR->CheckUsingEquipItemNum() )
			{
				//"장착 아이템이 부족합니다"
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1554) );
				return;
			}
			// 내구도 1% 미만 3개 이상인지
			if( FALSE == TITANMGR->CheckUsingEquipItemEndurance() )
			{
				//"장착 아이템 수리가 필요합니다"
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1555) );
				return;
			}*/

			//캐릭터 이동 정지
			MOVEMGR->HeroMoveStop();

			MSG_ITEM_USE_SYN msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_USE_SYN;
			msg.dwObjectID = HEROID;
			msg.wItemIdx = pItem->GetItemIdx();
			msg.TargetPos = pItem->GetPosition();

			NETWORK->Send(&msg,sizeof(msg));

			// debug용
			ITEMMGR->m_nItemUseCount++;
		}
	}
	else
	{
		ASSERT(0);
	}
}

void CInventoryExDialog::SetActive(BOOL val)
{
	if( !m_bDisable )
	{
		if(val == FALSE)
		{
			CDealDialog * dDlg = GAMEIN->GetDealDialog();
			if(dDlg)
				if(dDlg->IsShow())
					dDlg->SetActive(FALSE);

			CPyogukDialog* pPyogukDlg = GAMEIN->GetPyogukDialog();
			if(pPyogukDlg->IsActive())
				pPyogukDlg->SetActive(FALSE);

			if(GAMEIN->GetGuildWarehouseDlg()->IsActive())
				GAMEIN->GetGuildWarehouseDlg()->SetActive(FALSE);
//			SetAbsX( GetAbsX() - 1 );	//임시.. 렙제로 붉은상태에서 원상태로 보이게 하기 위해

			// magi82 - Titan(070611) 타이탄 무공변환 주석처리
			/*
			if(GAMEIN->GetTitanMugongMixDlg()->IsActive())
				GAMEIN->GetTitanMugongMixDlg()->DeleteItemIcon();
			*/
		}
		else
		{
//			SetAbsX( GetAbsX() + 1 );
		}

		cTabDialog::SetActive(val);





//--- main bar icon
//	CMainBarDialog* pDlg = GAMEIN->GetMainInterfaceDialog();
//	if( pDlg )
//		pDlg->SetPushBarIcon( OPT_INVENTORYDLGICON, m_bActive );


#ifdef _JAPAN_LOCAL_

		if( !val )			return;

		if( HERO == NULL )	return;

		DWORD nActive = GIVEN_INVENTORY_SLOT + HERO->GetExtraInvenSlot();
		for(int i=0; i<4; ++i)
		{
			cWindow* pBtn = GetWindowForID( IN_TABBTN1+i );
			cWindow* pGrid = GetWindowForID( IN_TABDLG1+i );

			if( i < nActive )
			{
				if( pBtn )		pBtn->SetActive( TRUE );
				if( pGrid)		pGrid->SetActive( TRUE );
			}
			else
			{
				//			if( i == nActive )
				//				m_pItemShopBtn->SetAbsXY( pBtn->GetAbsX(), pBtn->GetAbsY() );

				if( pBtn )		pBtn->SetActive( FALSE );
				if( pGrid)		pGrid->SetActive( FALSE );
			}	
		}

		if( m_pItemShopBtn )
		{
			cWindow* pBtn1 = GetWindowForID( IN_TABBTN1 );
			cWindow* pBtn2 = GetWindowForID( IN_TABBTN1+1 );
			if( pBtn1 && pBtn2 )
			{
				m_pItemShopBtn->SetRelXY( pBtn1->GetRelX(),
					pBtn1->GetRelY() + nActive * ( pBtn2->GetRelY() - pBtn1->GetRelY() ) );

				m_pItemShopBtn->SetAbsXY( pBtn1->GetAbsX(),
					pBtn1->GetAbsY() + nActive * ( pBtn2->GetAbsY() - pBtn1->GetAbsY() ) );
			}
		}

#endif	// _JAPAN_LOCAL_

#ifdef _HK_LOCAL_

		if( !val )			return;

		if( HERO == NULL )	return;

		DWORD nActive = GIVEN_INVENTORY_SLOT + HERO->GetExtraInvenSlot();
		for(int i=0; i<4; ++i)
		{
			cWindow* pBtn = GetWindowForID( IN_TABBTN1+i );
			cWindow* pGrid = GetWindowForID( IN_TABDLG1+i );

			if( i < nActive )
			{
				if( pBtn )		pBtn->SetActive( TRUE );
				if( pGrid)		pGrid->SetActive( TRUE );
			}
			else
			{
				//			if( i == nActive )
				//				m_pItemShopBtn->SetAbsXY( pBtn->GetAbsX(), pBtn->GetAbsY() );

				if( pBtn )		pBtn->SetActive( FALSE );
				if( pGrid)		pGrid->SetActive( FALSE );
			}	
		}

		if( m_pItemShopBtn )
		{
			cWindow* pBtn1 = GetWindowForID( IN_TABBTN1 );
			cWindow* pBtn2 = GetWindowForID( IN_TABBTN1+1 );
			if( pBtn1 && pBtn2 )
			{
				m_pItemShopBtn->SetRelXY( pBtn1->GetRelX(),
					pBtn1->GetRelY() + nActive * ( pBtn2->GetRelY() - pBtn1->GetRelY() ) );

				m_pItemShopBtn->SetAbsXY( pBtn1->GetAbsX(),
					pBtn1->GetAbsY() + nActive * ( pBtn2->GetAbsY() - pBtn1->GetAbsY() ) );
			}
		}

#endif	// _HK_LOCAL_

#ifdef _TL_LOCAL_

		if( !val )			return;

		if( HERO == NULL )	return;

		DWORD nActive = GIVEN_INVENTORY_SLOT + HERO->GetExtraInvenSlot();
		for(int i=0; i<4; ++i)
		{
			cWindow* pBtn = GetWindowForID( IN_TABBTN1+i );
			cWindow* pGrid = GetWindowForID( IN_TABDLG1+i );

			if( i < nActive )
			{
				if( pBtn )		pBtn->SetActive( TRUE );
				if( pGrid)		pGrid->SetActive( TRUE );
			}
			else
			{
				//			if( i == nActive )
				//				m_pItemShopBtn->SetAbsXY( pBtn->GetAbsX(), pBtn->GetAbsY() );

				if( pBtn )		pBtn->SetActive( FALSE );
				if( pGrid)		pGrid->SetActive( FALSE );
			}	
		}

		if( m_pItemShopBtn )
		{
			cWindow* pBtn1 = GetWindowForID( IN_TABBTN1 );
			cWindow* pBtn2 = GetWindowForID( IN_TABBTN1+1 );
			if( pBtn1 && pBtn2 )
			{
				m_pItemShopBtn->SetRelXY( pBtn1->GetRelX(),
					pBtn1->GetRelY() + nActive * ( pBtn2->GetRelY() - pBtn1->GetRelY() ) );

				m_pItemShopBtn->SetAbsXY( pBtn1->GetAbsX(),
					pBtn1->GetAbsY() + nActive * ( pBtn2->GetAbsY() - pBtn1->GetAbsY() ) );
			}
		}

#endif	// _TL_LOCAL_

		if( m_bShopInven )
		{
			OnShopItemBtn();
		}
		else
		{
			m_pItemShopBtn->SetPush(FALSE);
			m_pItemShopInven->SetActive(FALSE);
		}
	}
}


void CInventoryExDialog::OnShopItemBtn()
{	
	for(int i=0; i<m_bTabNum; i++)
	{
		m_ppPushupTabBtn[i]->SetPush(FALSE);
		m_ppWindowTabSheet[i]->SetActive(FALSE);				
	}
	
	m_pItemShopBtn->SetPush(TRUE);
	m_pItemShopInven->SetActive(TRUE);
	m_bShopInven = TRUE;

}

void CInventoryExDialog::OffShopItemBtn(LONG lId)
{
	if(m_pItemShopBtn->IsPushed() == FALSE)
		return;

	m_pItemShopBtn->SetPush(FALSE);
	m_pItemShopInven->SetActive(FALSE);

	for(int i=0; i<m_bTabNum; i++)
	{
		if(lId == m_ppPushupTabBtn[i]->GetID())
			SelectTab(i);
	}

	m_bShopInven = FALSE;
}


void CInventoryExDialog::OnActionEvnet(LONG lId, void * p, DWORD we)
{
 	if(/*we == WE_RBTNCLICK ||*/ we == WE_LBTNDBLCLICK)
	{
		// 장착창인지? or 인벤토리인지
		
		CItem * pItem = NULL;
		if(lId == IN_WEAREDDLG)
		{
			pItem = (CItem*)m_pWearedDlg->GetIconForIdx((WORD)m_pWearedDlg->GetCurSelCellPos());
			if( !pItem ) return;
		}
		else if(lId == IN_SHOPITEMGRID)
		{
			pItem = (CItem*)m_pItemShopInven->GetIconForIdx((WORD)m_pItemShopInven->GetCurSelCellPos());
			if( !pItem )			return;			

			//if( pItem->GetItemParam() == 1 )
			if( pItem->GetItemParam() & ITEM_PARAM_SEAL )
			{
				if( pItem->IsLocked() ) return;
				WINDOWMGR->MsgBox(MBI_SHOPITEM_LOCK, MBT_YESNO, CHATMGR->GetChatMsg(774));
				//CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(775));
				return;
			}
		}
		else
		{
			cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum());
			if( !gridDlg ) return;
			pItem = (CItem*)gridDlg->GetIconForIdx((WORD)gridDlg->GetCurSelCellPos());
			if( !pItem ) return;
		}
		
		// magi82 - Titan(070517)
		if(CURSOR->GetCursor() != eCURSOR_TITANREPAIR)
		{
			UseItem(pItem);
		}

	}
	else if(we == WE_LBTNCLICK)
	{
		// magi82 - Titan(070511)
		if( CURSOR->GetCursor() == eCURSOR_TITANREPAIR && lId != IN_WEAREDDLG && lId != IN_SHOPITEMGRID )
		{
			CItem * pItem = NULL;
			cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum());
			if( !gridDlg ) return;
			pItem = (CItem*)gridDlg->GetIconForIdx((WORD)gridDlg->GetCurSelCellPos());
			if( !pItem ) return;

			if(pItem->GetItemKind() & eTITAN_EQUIPITEM)
			{
                TITANMGR->SetTitanRepairItem(pItem);

				// magi82(8) - Titan(071024) 타이탄 장착아이템 개별수리할때 가격 나오게 계산
				TITAN_ENDURANCE_ITEMINFO* pInfo = TITANMGR->GetTitanEnduranceInfo(pItem->GetDBIdx());
				if( pInfo )
				{
					float fEnduranceRate = (float)pInfo->Endurance * 100 / TITAN_EQUIPITEM_ENDURANCE_MAX;
					if( fEnduranceRate > 0.0f && fEnduranceRate < 1.0f )
					{
						fEnduranceRate = 1.0f;
					}
					DWORD dwRepairPrice = (DWORD)( (100 - (DWORD)fEnduranceRate) * (pItem->GetItemInfo()->BuyPrice / 100) * 0.09f );

					WINDOWMGR->MsgBox( MBI_TITAN_REPAIR, MBT_YESNO, CHATMGR->GetChatMsg(1538), dwRepairPrice );
					ITEMMGR->SetDisableDialog(TRUE, eItemTable_Titan);
					ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
				}
			}
			else
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1539) );
			}
		}

		if(m_iInventoryState == eITEMUPGRADE_STATE)
		{
			/*
			CItem * pItemTT = GetCurSelectedItem((lId == IN_WEAREDDLG?WEAR:INV));
			if(!pItemTT) return;

			// 등급업을 할 수 있는 조건

			if(!ITEMMGR->CanGradeUp(pItemTT))
			{
//				GAMEIN->GetChatDialog()->AddChatMsg(WHOLE,RGBA_MAKE(255,0,0,255), "등급업을 할 수 없는 아이템입니다");
				CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(172) );
				return ;
			}

			BYTE TableIdx = ITEMMGR->GetTableIdxForAbsPos(pItemTT->GetPosition());
			if(TableIdx >= eItemTable_Max)
			{
				ASSERT(0);
				return;
			}
			
			MSG_ITEM_UPGRADE_SYN msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_UPGRADE_SYN;
			msg.dwObjectID = HEROID;
			msg.TargetPos = pItemTT->GetPosition();
			msg.wItemIdx = pItemTT->GetItemIdx();

			NETWORK->Send(&msg,sizeof(msg));
			*/
		}
		else if(m_iInventoryState == eITEMDEAL_STATE)
		{		
			if(!GAMEIN->GetDealDialog()->IsShow()) return;
			CItem * pItemTT = GetCurSelectedItem(INV);
			if(!pItemTT)
			{
				GAMEIN->GetDealDialog()->SetMoney(0, 2);
				return;
			}
			
			int itemIDX = pItemTT->GetItemIdx();
			ITEM_INFO * pItem = ITEMMGR->GetItemInfo(itemIDX);
			ASSERT(pItem);
//			GAMEIN->GetDealDialog()->SetMoney(pItem->SellPrice, 0);
			DWORD SellPrice = SWPROFIT->CalTexRateForSell( pItem->SellPrice );
			// 요새전
			SellPrice = FORTWARMGR->CalTexRateForSell( pItem->SellPrice );
			GAMEIN->GetDealDialog()->SetMoney(SellPrice, 0);
		}
	}
}

CItem * CInventoryExDialog::GetCurSelectedItem(BYTE type)
{
	switch(type)
	{
	case INV:
		{
			// INVENTORY
			cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum());
			if(gridDlg->GetCurSelCellPos() == -1) 
				return NULL;
			return (CItem *)gridDlg->GetIconForIdx((WORD)gridDlg->GetCurSelCellPos());
		}
		break;
	case WEAR:
		{
			// WEARED
			if(m_pWearedDlg->GetCurSelCellPos() == -1) 
				return NULL;
			return (CItem *)m_pWearedDlg->GetIconForIdx((WORD)m_pWearedDlg->GetCurSelCellPos());
		}
		break;
	case SHOPIVN:
		{
			return m_pItemShopInven->GetItemForPos((POSTYPE)(m_pItemShopInven->GetCurSelCellPos()+TP_SHOPINVEN_START));
		}
		break;
	default:
		{
			ASSERT(0);
		}
	}
	return NULL;
}

BOOL CInventoryExDialog::GetBlankPositionRestrictRef(WORD & absPos)
{
	for(int i = 0 ; i < GetTabNum() ; ++i)
	{
		cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(i);
		for(int j = 0 ; j < gridDlg->GetCellNum() ; ++j)
		{
			if(gridDlg->IsAddable(j))
			{
//				absPos = INVENTORY_STARTPOSITION+i*CELLNUMPERPAGE+j;
				absPos = TP_INVENTORY_START+i*TABCELL_INVENTORY_NUM+j;
				return TRUE;
			}
		}
		
	}
	return FALSE;
}

int CInventoryExDialog::GetBlankNum()
{
	int nBlankNum = 0;
	BYTE TabNum = GetTabNum();
	for( BYTE i = 0 ; i < TabNum ; ++i )
	{
		cIconGridDialog* gridDlg = (cIconGridDialog*)GetTabSheet(i);

		for( WORD j = 0 ; j < gridDlg->GetCellNum() ; ++j )
		{
			if( gridDlg->IsAddable(j) )
				++nBlankNum;
		}
	}

	return nBlankNum;
}

BOOL CInventoryExDialog::FakeMoveIcon(LONG x, LONG y, cIcon * icon)
{
	ASSERT(icon);
	if( m_bDisable )	return FALSE;

	if( PKMGR->IsPKLooted() )	return FALSE;	//죽었을때 다막을까.. //KES 040801

	if( icon->GetType() == WT_STALLITEM || icon->GetType() == WT_EXCHANGEITEM ) return FALSE;
	
	if(icon->GetType() == WT_ITEM)
	{
		return FakeMoveItem(x, y, (CItem *)icon);
	}
	else if(icon->GetType() == WT_DEALITEM)
	{
		FakeBuyItem(x, y, (CDealItem *)icon);
		AUDIOMGR->Play(58, HERO);
	}
	//else /*특수 무공만 변환가능*/
		//FakeAddMugong(x, y, (CMugongBase *)icon);
	return FALSE;
}

void CInventoryExDialog::FakeBuyItem( LONG mouseX, LONG mouseY, CDealItem * pFromItem )
{
//	if( pFromItem )
//		GAMEIN->GetDealDialog()->FakeBuyItem( pFromItem->GetAbsX(), pFromItem->GetAbsY(), pFromItem->GetItemIdx() );
	if( pFromItem )
		GAMEIN->GetDealDialog()->FakeBuyItem( (LONG)pFromItem->GetAbsX(), (LONG)pFromItem->GetAbsY(), pFromItem );
}

// 단축창 정보 이동
void CInventoryExDialog::MoveItem(POSTYPE FromPos, POSTYPE ToPos )
{
	CItem * FromItem = NULL;
	CItem * ToItem = NULL;

	DeleteItem(FromPos, &FromItem);
	DeleteItem(ToPos, &ToItem);

	if(FromItem)
	{
		FromItem->SetPosition(ToPos);
		if(!AddItem(FromItem))
			assert(0);
	}

	if(ToItem)
	{
		ToItem->SetPosition(FromPos);
		if(!AddItem(ToItem))
			assert(0);
	}
}

BOOL CInventoryExDialog::FakeMoveItem( LONG mouseX, LONG mouseY, CItem * pFromItem )
{
	WORD ToPos=0;
	
	if( !GetPositionForXYRef( 0, mouseX, mouseY, ToPos ) )	// 절대위치 넘어 옴
		return FALSE;

	CItem * pToItem = GetItemForPos( ToPos );
	
	//sound
	if(ITEMMGR->GetTableIdxForAbsPos(ToPos)==eItemTable_Weared)
		AUDIOMGR->Play(59, HERO);
	else
		AUDIOMGR->Play(58, HERO);

//KES EXCHANGE 031002
	if( pToItem )
	if( pToItem->IsLocked() ) return FALSE;
	
	if( pFromItem->IsLocked() ) return FALSE;
	
	if(TP_PYOGUK_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_PYOGUK_END) 
		return FakeGeneralItemMove( ToPos, pFromItem, pToItem );
	else if(TP_GUILDWAREHOUSE_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_GUILDWAREHOUSE_END) 
		return FakeGeneralItemMove( ToPos, pFromItem, pToItem );
	else if(TP_SHOPITEM_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_SHOPITEM_END)
		return m_pItemShopInven->FakeGeneralItemMove( ToPos, pFromItem, pToItem );
	else if(TP_SHOPINVEN_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_SHOPINVEN_END)
	{
		if( ITEMMGR->IsDupItem( pFromItem->GetItemIdx() ) )
		{
			if( IsDivideFunctionKey() && !( pToItem ) )			
				return FakeItemDivide( ToPos, pFromItem, pToItem );
			else if( ( pToItem && pFromItem->GetItemIdx() == pToItem->GetItemIdx() ) )			
				return FakeItemCombine( ToPos, pFromItem, pToItem );
			else
				return m_pItemShopInven->FakeGeneralItemMove( ToPos, pFromItem, pToItem );
		}
	}
	// 요새 & 공성관련
	else if( (TP_SIEGEFORTWARE_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_SIEGEFORTWARE_END) )
	{
        return GAMEIN->GetFWWareHouseDlg()->FakeGeneralItemMove( ToPos, pFromItem, pToItem );
	}


	if( ITEMMGR->IsDupItem( pFromItem->GetItemIdx() ) )
	{
		if( IsDivideFunctionKey() && !( pToItem ) )
		{
			return FakeItemDivide( ToPos, pFromItem, pToItem );
		}
		else if( ( pToItem && pFromItem->GetItemIdx() == pToItem->GetItemIdx() ) )
		{
			return FakeItemCombine( ToPos, pFromItem, pToItem );
		}
	}
	
/*	if( m_bShopInven && TP_SHOPITEM_START <= pFromItem->GetPosition() && pFromItem->GetPosition() < TP_SHOPITEM_END)
		return m_pItemShopInven->FakeGeneralItemMove( ToPos, pFromItem, pToItem );	
	else
		return FakeGeneralItemMove( ToPos, pFromItem, pToItem );*/

	return FakeGeneralItemMove( ToPos, pFromItem, pToItem );
}

BOOL CInventoryExDialog::IsDivideFunctionKey()
{
	return KEYBOARD->GetKeyPressed(KEY_MENU);
}

BOOL CInventoryExDialog::FakeGeneralItemMove( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem )
{
	if( CanBeMoved( pFromItem, ToPos ) == FALSE )
		return FALSE;

	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max) return FALSE;
	if( pToItem && CanBeMoved( pToItem, pFromItem->GetPosition() ) == FALSE )
	{
		return FALSE;
	}
	if(pToItem && (FromIdx == eItemTable_MunpaWarehouse))
	{
		if(FromIdx == eItemTable_GuildWarehouse)
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(53) );
		return FALSE;
	}

	if(pToItem)
	{
		if(FromIdx == eItemTable_Pyoguk && pToItem->GetDBIdx() == TITANMGR->GetRegistedTitanItemDBIdx())
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1578));
			return FALSE;
		}
	}


	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if(ToIdx >= eItemTable_Max) return FALSE;


	MSG_ITEM_MOVE_SYN msg;
	msg.Category			= MP_ITEM;
	msg.Protocol			= MP_ITEM_MOVE_SYN;
	msg.dwObjectID			= HEROID;

	msg.FromPos				= pFromItem->GetPosition();
	msg.wFromItemIdx		= pFromItem->GetItemIdx();
	msg.ToPos				= ToPos;
	msg.wToItemIdx			= (pToItem?pToItem->GetItemIdx():0);

	if( msg.FromPos == msg.ToPos )
		return FALSE;

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_PetInven);

	NETWORK->Send( &msg, sizeof(msg) );
	return TRUE;
}

BOOL CInventoryExDialog::FakeItemDivide( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem )
{
	if( !CanBeMoved( pFromItem, ToPos ) )
		return FALSE;
	if( pFromItem->GetPosition() == ToPos )
		return FALSE;
	if( pFromItem->IsLocked() )
		return FALSE;

	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max) return FALSE;

	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if(ToIdx >= eItemTable_Max) return FALSE;
	
	if(pFromItem->GetDurability() == 1)
	{
		return FakeGeneralItemMove(ToPos, pFromItem, pToItem);
	}

	m_divideMsg.Category			= MP_ITEM;
	m_divideMsg.Protocol			= MP_ITEM_DIVIDE_SYN;
	m_divideMsg.dwObjectID			= HEROID;

	m_divideMsg.wItemIdx = pFromItem->GetItemIdx();
	m_divideMsg.FromPos = pFromItem->GetPosition();
	m_divideMsg.FromDur = pFromItem->GetDurability();
	m_divideMsg.ToPos = ToPos;
	m_divideMsg.ToDur = 0;

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_ShopInven);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_PetInven);

	cDivideBox * pDivideBox = WINDOWMGR->DivideBox( DBOX_DIVIDE_INV, (LONG)pFromItem->GetAbsX(), (LONG)pFromItem->GetAbsY(), OnFakeItemDivideOk, OnFakeItemDivideCancel, this, pFromItem, CHATMGR->GetChatMsg(185) );
	pDivideBox->SetMaxValue( MAX_YOUNGYAKITEM_DUPNUM );
	pDivideBox->SetValue( 1 );	//나누는데 맥스치를 넣기도 0을넣기도 이상... 1이 젤 무난하다.
	//pDivideBox->SetValue( m_divideMsg.FromDur );
	
	return FALSE;
		
}

void CInventoryExDialog::OnFakeItemDivideCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);
}

void CInventoryExDialog::OnFakeItemDivideOk( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CInventoryExDialog * tDlg = ( CInventoryExDialog * )vData1;
	if( param1 == 0 )
	{
		OnFakeItemDivideCancel(0,NULL,0,NULL,NULL);
		return;
	}

	WORD TargetTableIdx = ITEMMGR->GetTableIdxForAbsPos(tDlg->m_divideMsg.FromPos);
	CItem * pTargetItem = ITEMMGR->GetItemofTable(TargetTableIdx, tDlg->m_divideMsg.FromPos);
	if(!pTargetItem)
	{
		OnFakeItemDivideCancel(0,NULL,0,NULL,NULL);
		return ;
	}
	if(pTargetItem->IsLocked())
	{
		OnFakeItemDivideCancel(0,NULL,0,NULL,NULL);
		return ;
	}

	if(tDlg->m_divideMsg.wItemIdx != pTargetItem->GetItemIdx())
	{
		OnFakeItemDivideCancel(0,NULL,0,NULL,NULL);
		return ;
	}
	if(tDlg->m_divideMsg.FromDur != pTargetItem->GetDurability())
	{
		OnFakeItemDivideCancel(0,NULL,0,NULL,NULL);
		return ;
	}
	CItem * pToItem = ITEMMGR->GetItemofTable(ITEMMGR->GetTableIdxForAbsPos(tDlg->m_divideMsg.ToPos), tDlg->m_divideMsg.ToPos);
	if(pToItem)
	{
		OnFakeItemDivideCancel(0,NULL,0,NULL,NULL);
		return ;
	}

	if( tDlg->m_divideMsg.FromDur > param1 )
	{
		tDlg->m_divideMsg.ToDur			= param1;
		tDlg->m_divideMsg.FromDur		= tDlg->m_divideMsg.FromDur - param1;
		NETWORK->Send( &tDlg->m_divideMsg, sizeof(tDlg->m_divideMsg) );
	}
	else
	{
		//아이템 이동
		tDlg->FakeGeneralItemMove(tDlg->m_divideMsg.ToPos, (CItem *)vData2, NULL);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_PetInven);
	}
}

BOOL CInventoryExDialog::FakeItemCombine( POSTYPE ToPos, CItem * pFromItem, CItem * pToItem )
{
	if( pFromItem->GetPosition() == pToItem->GetPosition() )
		return FALSE;
	if( pFromItem->GetItemIdx() != pToItem->GetItemIdx() )
		return FALSE;

	if( CanBeMoved( pFromItem, ToPos ) == FALSE )
		return FALSE;

	if( pToItem && CanBeMoved( pToItem, pFromItem->GetPosition() ) == FALSE )
		return FALSE;

	if( pFromItem->IsLocked() || pToItem->IsLocked() )
		return FALSE;

	BYTE FromIdx = ITEMMGR->GetTableIdxForAbsPos( pFromItem->GetPosition() );
	if(FromIdx >= eItemTable_Max) return FALSE;

	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if(ToIdx >= eItemTable_Max) return FALSE;

	MSG_ITEM_COMBINE_SYN msg;
	msg.Category			= MP_ITEM;
	msg.Protocol			= MP_ITEM_COMBINE_SYN;
	msg.dwObjectID			= HEROID;

	msg.FromPos				= pFromItem->GetPosition();
	msg.ToPos				= pToItem->GetPosition();
	msg.wItemIdx			= pFromItem->GetItemIdx();
	msg.FromDur				= pFromItem->GetDurability();
	msg.ToDur				= pToItem->GetDurability();

	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(TRUE, eItemTable_GuildWarehouse);


	NETWORK->Send( &msg, sizeof(msg) );

	return TRUE;
}	

void CInventoryExDialog::Linking()
{
	if(!m_fSpread)
	{
		m_pWearedDlg = (CWearedExDialog *)GetWindowForID(IN_WEAREDDLG);
		m_pMoneyEdit = (cStatic *)GetWindowForID(IN_MONEYEDIT);
		m_pItemShopInven = (CItemShopInven*)GetWindowForID(IN_SHOPITEMGRID);
		m_pItemShopBtn = (cPushupButton*)GetWindowForID(IN_SHOPITEMBTN);
		m_pItemShopBtn->SetActive(FALSE);
		cImage Image;
		SCRIPTMGR->GetImage(63, &Image, PFT_HARDPATH);
		m_pItemShopBtn->SetToolTip( CHATMGR->GetChatMsg(731), RGB_HALF(255,255,255), &Image, TTCLR_DEFAULT );
		m_pItemShopInven->SetActive(FALSE);
		m_fSpread = TRUE;
	}
}

CItem * CInventoryExDialog::GetItemLike(WORD wItemIdx)
{
	// RaMa - 04.12.09
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(wItemIdx);
	if(pItemInfo->ItemKind & eSHOP_ITEM)
	{
		for(int i=0; i<m_pItemShopInven->GetCellNum(); i++)
		{
			if( !m_pItemShopInven->IsAddable(i) )
			{
				CItem* pItem = (CItem*)m_pItemShopInven->GetIconForIdx(i);
				if(pItem->GetItemIdx() == wItemIdx && !pItem->IsLocked())
					return pItem;

			}
		}
	}

	BYTE TabNum = GetTabNum();
	for( BYTE i = 0 ; i < TabNum ; ++i )
	{
		cIconGridDialog* gridDlg = (cIconGridDialog*)GetTabSheet(i);

		for( WORD j = 0 ; j < gridDlg->GetCellNum() ; ++j )
		{
			if(!gridDlg->IsAddable(j) )
			{
				CItem * pItem = (CItem *)gridDlg->GetIconForIdx(TP_INVENTORY_START+j/*+TABCELL_INVENTORY_NUM*i*/);
				if(pItem->GetItemIdx() == wItemIdx)
					return pItem;
			}
		}
	}
	return NULL;
}
DURTYPE	CInventoryExDialog::GetTotalItemDurability(WORD wItemIdx)
{
	DURTYPE DurSum = 0;

	// RaMa - 04.12.09
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(wItemIdx);
	if(pItemInfo)
	if(pItemInfo->ItemKind & eSHOP_ITEM)
	{
		for(int i=0; i<m_pItemShopInven->GetCellNum(); i++)
		{
			if( !m_pItemShopInven->IsAddable(i) )
			{
				CItem* pItem = (CItem*)m_pItemShopInven->GetIconForIdx(i);
				if(pItem->GetItemIdx() == wItemIdx && !(pItem->GetItemParam()&ITEM_PARAM_SEAL))
					DurSum += pItem->GetDurability();
			}
		}

		return DurSum;
	}

	BYTE TabNum = GetTabNum();
	for( BYTE i = 0 ; i < TabNum ; ++i )
	{
		cIconGridDialog* gridDlg = (cIconGridDialog*)GetTabSheet(i);

		for( WORD j = 0 ; j < gridDlg->GetCellNum() ; ++j )
		{
			if(!gridDlg->IsAddable(j) )
			{
				CItem * pItem = (CItem *)gridDlg->GetIconForIdx(TP_INVENTORY_START+j/*+TABCELL_INVENTORY_NUM*i*/);

				if(pItem->GetItemIdx() == wItemIdx)
					DurSum += pItem->GetDurability();
			}
		}
	}

	return DurSum;
}
// 0(all),1(inv),2
BOOL CInventoryExDialog::GetPositionForXYRef(BYTE param, LONG x, LONG y, WORD& pos)
{
	WORD position=0;										// 상대 위치

	if(param != 2)
	{
		if(m_bShopInven)
		{
			if( m_pItemShopInven->GetPositionForXYRef(x, y, position) )
			{
				pos = position+TP_SHOPINVEN_START;
				return TRUE;
			}		
		}
		else
		{
			cIconGridDialog * gridDlg = (cIconGridDialog *)GetTabSheet(GetCurTabNum());
			if(gridDlg->GetPositionForXYRef(x, y, position))
			{
				//pos = position+INVENTORY_STARTPOSITION+CELLNUMPERPAGE*GetCurTabNum();		// -> 절대 위치로 바꿈
				pos = position+TP_INVENTORY_START+TABCELL_INVENTORY_NUM*GetCurTabNum();		// -> 절대 위치로 바꿈
				return TRUE;
			}
		}
	}

	if(param != 1)
	if(m_pWearedDlg->GetPositionForXYRef(x, y, position))
	{
//		pos = position+WEAR_STARTPOSITION;					// -> 절대 위치로 바꿈
		pos = position+TP_WEAR_START;					// -> 절대 위치로 바꿈
		return TRUE;
	}
	
	return FALSE;
}

BOOL CInventoryExDialog::CheckItemLockForItemIdx(WORD wItemIdx)
{
	BYTE TabNum = GetTabNum();
	for( BYTE i = 0 ; i < TabNum ; ++i )
	{
		cIconGridDialog* gridDlg = (cIconGridDialog*)GetTabSheet(i);

		for( WORD j = 0 ; j < gridDlg->GetCellNum() ; ++j )
		{
			if(!gridDlg->IsAddable(j) )
			{
				CItem * pItem = (CItem *)gridDlg->GetIconForIdx(TP_INVENTORY_START+j);
				
				if( pItem->GetItemIdx() == wItemIdx && pItem->IsLocked() )
					return FALSE;
			}
		}
	}

	return TRUE;
}

void CInventoryExDialog::SetMoney(DWORD Money)
{
	m_pMoneyEdit->SetStaticText(AddComma(Money));
}

BOOL CInventoryExDialog::IsExist(POSTYPE abs_pos)
{
	BOOL bExist = FALSE;
	if(TP_INVENTORY_START <= abs_pos && abs_pos < TP_INVENTORY_END)
	{
		int tabIndex = GetTabIndex( abs_pos );		
		cIconGridDialog * dlg = (cIconGridDialog *)GetTabSheet(tabIndex);
		if(dlg->GetIconForIdx(abs_pos-TABCELL_INVENTORY_NUM*tabIndex-TP_INVENTORY_START))
			bExist = TRUE;
	}
	else if(TP_WEAR_START <= abs_pos && abs_pos < TP_WEAR_END)
	{
		if( m_pWearedDlg->GetIconForIdx(abs_pos - TP_WEAR_START) )
			bExist = TRUE;
	}

	return bExist;
}

BOOL CInventoryExDialog::CanBeMoved(CItem* pItem,POSTYPE pos)
{
	ASSERT(pItem);

	if(pItem)
	{
		POSTYPE fromPos = pItem->GetPosition();
		POSTYPE toPos = pos;
		if( (TP_WEAR_START <= fromPos && fromPos < TP_WEAR_END) ||
			(TP_WEAR_START <= toPos && toPos < TP_WEAR_END) )
		{
			if( OBJECTSTATEMGR->CheckAvailableState(HERO,eCAS_EQUIPITEM) == FALSE )
			{
				CAction act;
				act.InitAction(eActionKind_EquipItem,(void*)pItem->GetDBIdx(),0,NULL);
				HERO->SetNextAction(&act);
				return FALSE;
			}
		}
	}
	if(TP_INVENTORY_START<= pos && pos < TP_INVENTORY_END)
	{
		// 샵이아템은 인벤에 못들어온다.
 		POSTYPE frompos = pItem->GetPosition();
		if( TP_SHOPITEM_START <= frompos && frompos < TP_SHOPITEM_END )
			return FALSE;
		else if( TP_SHOPINVEN_START <= frompos && frompos < TP_SHOPINVEN_END )
			return FALSE;
		else if( TP_PETWEAR_START <= frompos && frompos < TP_PETWEAR_END )
			return FALSE;
		else if( TP_TITANWEAR_START <= frompos && frompos < TP_TITANWEAR_END )
			return TRUE;
		else
			return TRUE;
	}
	else if(TP_WEAR_START <= pos && pos < TP_WEAR_END)
	{
		// 장착창
		// 아이탬이 있으면 FALSE
		/*
		POSTYPE aa = WEAR_STARTPOSITION;
		POSTYPE idx = pos - aa;
		if(m_pWearedDlg->GetIconForIdx(idx))
			return FALSE;
		*/

		// 장착하는 아이탬이 아니면 FALSE
		if( !( pItem->GetItemKind() & eEQUIP_ITEM ) )
		{
			if( pItem->GetItemKind() == eQUEST_ITEM_EQUIP )
			{
			}
			else if( pItem->GetItemKind() == eSHOP_ITEM_EQUIP )
			{
				if( pItem->GetItemParam() & ITEM_PARAM_SEAL )
					return FALSE;
			}
			else
				return FALSE;
		}
		else
		{
			CItem* ptarget = (CItem*)m_pWearedDlg->GetIconForIdx( pos - TP_WEAR_START ); 
			if( ptarget )
			{
				POSTYPE frompos = pItem->GetPosition();
				if( (TP_PYOGUK_START <= frompos && frompos < TP_PYOGUK_END) &&
					ptarget->GetItemKind() & eSHOP_ITEM_EQUIP )
					return FALSE;
			}
		}

		// 들어가야 할자리가 아니면 FALSE
		ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
		if(!pInfo) return FALSE;
		if(pInfo->EquipKind == eWearedItem_Ring1)
		{	
			if(pos-TP_WEAR_START != eWearedItem_Ring1 && pos-TP_WEAR_START != eWearedItem_Ring2)
				return FALSE;
		}
		else
		{
			if(pos-TP_WEAR_START != pInfo->EquipKind)
				return FALSE;
		}
		
		return ITEMMGR->CanEquip(pItem->GetItemIdx());
	}
	else if(TP_PYOGUK_START <= pos && pos < TP_PYOGUK_END)
	{
		return TRUE;
	}
	else if(TP_SHOPINVEN_START <=pos && pos <TP_SHOPINVEN_END)
	{
		POSTYPE frompos = pItem->GetPosition();
		if( TP_SHOPITEM_START <= frompos && frompos < TP_SHOPITEM_END )
			return TRUE;
		else if( TP_SHOPINVEN_START <= frompos && frompos < TP_SHOPINVEN_END )
			return TRUE;
		else if( /*pItem->GetItemKind() == eSHOP_ITEM_EQUIP &&*/ TP_WEAR_START <= frompos && frompos < TP_WEAR_END )
		{
			return TRUE;
		}
		else if( TP_PETWEAR_START <= frompos && frompos < TP_PETWEAR_END )
			return FALSE;
		else if( TP_TITANSHOPITEM_START <= frompos && frompos < TP_TITANSHOPITEM_END )
			return TRUE;
		else
			return FALSE;
	}
	else if(TP_PETWEAR_START <= pos && pos < TP_PETWEAR_END)
	{
		POSTYPE fromPos = pItem->GetPosition();

		if( TP_SHOPINVEN_START <= fromPos && fromPos < TP_SHOPINVEN_END )
		{
			CPetWearedExDialog* pPetEquipDlg = ((CPetInventoryDlg*)GAMEIN->GetPetInventoryDialog())->GetPetWearedDlg();
			if( FALSE == pPetEquipDlg->CheckDuplication( pItem->GetItemIdx() ) )
			{
				return TRUE;
			}
			return FALSE;
		}
		else
			return FALSE;
	}
	else if(TP_TITANWEAR_START <= pos && pos < TP_TITANWEAR_END)
	{
		// 타이탄 장착아이템이 아니면 FALSE
		if( ( pItem->GetItemKind() & eTITAN_EQUIPITEM ) )
		{
			// 들어가야 할자리가 아니면 FALSE
			ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
			if(!pInfo) return FALSE;

			if(pos-TP_TITANWEAR_START != pInfo->ItemKind-eTITAN_EQUIPITEM_HELMET)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1505) );
				return FALSE;
			}

			//2007. 10. 25. CBH - 타이탄 장비 착용시 레벨제한 체크					
			if(pInfo->LimitLevel > HERO->GetLevel())
			{
				char temp[256] = {0, };
				sprintf( temp, CHATMGR->GetChatMsg(238), (int)pInfo->LimitLevel );
				CHATMGR->AddMsg( CTC_SYSMSG, temp );
				return FALSE;
			}

			return TRUE;
		}
	}
	// magi82(26)
	else if(TP_TITANSHOPITEM_START <= pos && pos < TP_TITANSHOPITEM_END)
	{
		if( pItem->GetItemKind() == eSHOP_ITEM_TITAN_EQUIP )
		{
			ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(pItem->GetItemIdx());
			if(!pInfo) return FALSE;

			return TRUE;
		}
	}

	return FALSE;
}

void CInventoryExDialog::ItemUseForQuestStart()
{
	if( m_pTempQuestItem )
	{
		// quest 아이템의 사용
		MSG_ITEM_USE_SYN msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_USE_FOR_QUESTSTART_SYN;
		msg.dwObjectID = HEROID;
		msg.TargetPos = m_pTempQuestItem->GetPosition();
		msg.wItemIdx = m_pTempQuestItem->GetItemIdx();

		NETWORK->Send(&msg,sizeof(msg));
	}
}



void CInventoryExDialog::ItemUnSealingSync()
{
	CItem* pItem = ITEMMGR->GetItemofTable(eItemTable_ShopInven, (POSTYPE)(m_pItemShopInven->GetCurSelCellPos()+TP_SHOPINVEN_START));
	if( !pItem || !(pItem->GetItemParam()&ITEM_PARAM_SEAL) )				return;	
	
	if( pItem->IsLocked() ) return;

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pItem->GetItemIdx() );
	if(!pItemInfo)	return;

	if( pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || 
		pItemInfo->ItemKind == eSHOP_ITEM_DECORATION )
	{
		if( ITEMMGR->GetUsedItemInfo( pItemInfo->ItemIdx ) )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(752) );
			return;
		}
	}

	
	MSG_DWORD msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_UNSEAL_SYN;
	msg.dwObjectID = HERO->GetID();
	msg.dwData = m_pItemShopInven->GetCurSelCellPos()+TP_SHOPINVEN_START;

	NETWORK->Send(&msg, sizeof(msg));
}


void CInventoryExDialog::ItemUnSealing(DWORD absPos)
{
	CItem* pItem = ITEMMGR->GetItemofTable((WORD)eItemTable_ShopInven, (POSTYPE)absPos);

	if( !pItem )		return;

	// 
	ITEMPARAM Param = pItem->GetItemBaseInfo()->ItemParam;
	Param ^= ITEM_PARAM_SEAL;
	pItem->SetItemParam( Param );

	// 툴팁을 다시 정해준다.
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pItem->GetItemIdx() );
	if( pItemInfo && 
		pItemInfo->ItemKind != eSHOP_ITEM_MAKEUP && 
		pItemInfo->ItemKind != eSHOP_ITEM_DECORATION &&
		pItemInfo->ItemKind != eSHOP_ITEM_EQUIP )
		ITEMMGR->SetToolTipIcon( pItem );

	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(776) );
}


void CInventoryExDialog::SetShopItemInvenBtn( BOOL val )
{
	m_pItemShopBtn->SetActive( val );
}

CItem* CInventoryExDialog::GetPriorityItemForCristmasEvent()
{
	CItem* pItem = NULL;

	pItem = GetItemLike( EVENT_ITEM_HARDSNOW );
	if( pItem )
		return pItem;
	else
	{
		pItem = GetItemLike( EVENT_ITEM_SNOW );
	}

	return pItem;
}

DWORD CInventoryExDialog::GetPriorityItemIdx()
{
	CItem* pItem = NULL;
	DWORD Idx = 0;
	pItem = GetPriorityItemForCristmasEvent();

	if( pItem )
	{
		Idx = pItem->GetItemIdx();
	}

	return Idx;
}
