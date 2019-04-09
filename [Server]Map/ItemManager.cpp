// ItemManager.cpp: implementation of the CItemManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemManager.h"
#include "ItemSlot.h"
#include "Player.h"
#include "Pet.h"
#include "Purse.h"
#include "ItemContainer.h"
#include "MHFile.h"
#include "MapDBMsgParser.h"
#include "UserTable.h"
#include "KyunggongManager.h"
#include "MugongManager.h"
#include "CharacterCalcManager.h"
#include "GuildManager.h"
#include "AbilityManager.h"
#include "CheckRoutine.h"
#include "MHError.h"
#include "PartyManager.h"
#include "LootingManager.h"
#include "QuestManager.h"
#include "BattleSystem_Server.h"
#include "..\[CC]Header\GameResourceStruct.h"
#include "ObjectStateManager.h"
#include "MHMap.h"
#include "ReinforceManager.h"
#include "DissolutionManager.h"
//SW050920
#include "RarenessManager.h"
#include "ChangeItemMgr.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "PackedData.h"
#include "Network.h"
#include "SiegeWarProfitMgr.h"
#include "WantedManager.h"
#include "SurvivalModeManager.h"
#include "TitanItemManager.h"
#include "BobusangManager.h"
#include "Titan.h"

#include "SkillManager_Server.h"
#include "FortWarManager.h"

//#include "Distribute_Random.h"

#ifdef _TL_LOCAL_
#include "PartyWarMgr.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CItemManager::CItemManager()
{
	m_ItemInfoList.Initialize(MAX_ITEM_NUM);
	m_UnfitItemInfoListForHide.Initialize(100);
	m_MixItemInfoList.Initialize(MAX_ITEM_NUM);
	m_HiLevelItemMixRateInfoList.Initialize(50);
	m_ReinforceItemInfoList.Initialize(20);
	m_ItemArrayPool.Init(500, 100, "ItemArrayPool" );
	m_ArrayIndexCreator.Init(MAX_ITEM_NUM*10, 1);
	m_DealerTable.Initialize(32);
	m_AvatarEquipTable.Initialize(50);
	m_Key = 0;
	m_SetItemOptionList.Initialize(MAX_SETITEM_KIND_NUM); //2007. 6. 8. CBH - 세트아이탬 리스트 초기화 추가
#ifdef _HK_LOCAL_
	ZeroMemory( m_nItemMixBal, sizeof( m_nItemMixBal ) );
#endif
}

CItemManager::~CItemManager()
{
	ITEM_INFO * pInfo = NULL;
	m_ItemInfoList.SetPositionHead();
	while(pInfo = m_ItemInfoList.GetData())
		delete pInfo;
	m_ItemInfoList.RemoveAll();
	m_UnfitItemInfoListForHide.RemoveAll();

	DealerData* pDealer = NULL;
	m_DealerTable.SetPositionHead();
	while(pDealer = m_DealerTable.GetData())
	{
		POS pos = pDealer->m_DealItemList.GetFirstPos();
		while( pos )
		{
			DealerItem* pDI = pDealer->m_DealItemList.GetNextPos( pos );
			if( pDI ) delete pDI;
		}

		pDealer->m_DealItemList.DeleteAll();
		delete pDealer;
	}
	m_DealerTable.RemoveAll();
	
	ITEM_MIX_INFO * pItemInfo = NULL;
	m_MixItemInfoList.SetPositionHead();
	while(pItemInfo = m_MixItemInfoList.GetData())
	{
		if(pItemInfo->psResultItemInfo)
		{
			for(int i = 0 ; i < pItemInfo->wResultItemNum ; ++i)
			{
				if(pItemInfo->psResultItemInfo[i].wResItemIdx != 0)
				{
					if(pItemInfo->psResultItemInfo[i].psMaterialItemInfo)
					{
						delete [] pItemInfo->psResultItemInfo[i].psMaterialItemInfo;
						pItemInfo->psResultItemInfo[i].psMaterialItemInfo = NULL;
					}
				}
			}
			delete [] pItemInfo->psResultItemInfo;
			pItemInfo->psResultItemInfo = NULL;
		}
		delete pItemInfo;
		pItemInfo = NULL;
	}
	m_MixItemInfoList.RemoveAll();

/* reinforce change
	ITEM_REINFORCE_INFO * pInfo3 = NULL;
	m_ReinforceItemInfoList.SetPositionHead();
	while(pInfo3 = m_ReinforceItemInfoList.GetData())
		delete pInfo3;
	m_ReinforceItemInfoList.RemoveAll();
*/

	AVATARITEM* pItem = NULL;
	m_AvatarEquipTable.SetPositionHead();
	while(pItem = m_AvatarEquipTable.GetData())
	{
		delete pItem;
	}
	m_AvatarEquipTable.RemoveAll();	

	m_ItemArrayPool.Release();
	m_ArrayIndexCreator.Release();

	////////// 2007. 6. 8. CBH - 세트아이탬 리스트 삭제 //////////
	SET_ITEM_OPTION* pSetItemOption = NULL;
	m_SetItemOptionList.SetPositionHead();
	while(pSetItemOption = m_SetItemOptionList.GetData())
		delete pSetItemOption;
	m_SetItemOptionList.RemoveAll();
	//////////////////////////////////////////////////////////////
}
ITEMOBTAINARRAYINFO *	CItemManager::Alloc(CPlayer * pPlayer, BYTE c, BYTE p, DWORD dwObjectID, DWORD dwFurnisherIdx, WORD wType, WORD ObtainNum, DBResult CallBackFunc, DBResultEx CallBackFuncEx)
{
	//return m_ItemArrayPool.Alloc();
	ITEMOBTAINARRAYINFO * pArrayInfo = m_ItemArrayPool.Alloc();
	pArrayInfo->ItemArray.Init( c, p, dwObjectID );
	pArrayInfo->ItemArray.wObtainCount = ObtainNum;
	pArrayInfo->wObtainArrayID = m_ArrayIndexCreator.GenerateIndex();
	pArrayInfo->CallBack = CallBackFunc;
	pArrayInfo->CallBackEx = CallBackFuncEx;
	pArrayInfo->dwFurnisherIdx = dwFurnisherIdx;
	pArrayInfo->wType = wType;
	pPlayer->AddArray(pArrayInfo);
	return pArrayInfo;
}
void CItemManager::Free(CPlayer * pPlayer, ITEMOBTAINARRAYINFO * info)
{
	info->ItemArray.Clear();
	pPlayer->RemoveArray(info->wObtainArrayID);
	m_ArrayIndexCreator.ReleaseIndex(info->wObtainArrayID);
	m_ItemArrayPool.Free( info );
}

BOOL CItemManager::IsTitanCallItem( WORD wItemIdx )
{
	ITEM_INFO* pItemInfo = GetItemInfo(wItemIdx);

	if(!pItemInfo)	return FALSE;

	if(pItemInfo->ItemKind == eTITAN_ITEM_PAPER )
		return TRUE;
	else
		return FALSE;
}

BOOL CItemManager::IsTitanEquipItem( WORD wItemIdx )
{
	ITEM_INFO* pItemInfo = GetItemInfo(wItemIdx);

	if(!pItemInfo)	return FALSE;

	if(pItemInfo->ItemKind & eTITAN_EQUIPITEM )
		return TRUE;
	else
		return FALSE;
}

BOOL CItemManager::IsPetSummonItem( WORD wItemIdx )
{
	ITEM_INFO* pItemInfo = GetItemInfo(wItemIdx);

	if(!pItemInfo)	return FALSE;

	if(pItemInfo->ItemKind == eQUEST_ITEM_PET || pItemInfo->ItemKind == eSHOP_ITEM_PET)
		return TRUE;
	else
		return FALSE;
}

BOOL CItemManager::IsRareOptionItem( WORD wItemIdx, DWORD dwRareDBIdx )
{
	if(dwRareDBIdx &&  !IsDupItem(wItemIdx))
		return TRUE;
	return FALSE;
}

BOOL CItemManager::IsOptionItem( WORD wItemIdx, DURTYPE wDurability )
{
	if(wDurability != 0 && !IsDupItem(wItemIdx))
		return TRUE;
	return FALSE;
}
BOOL CItemManager::IsDupItem( WORD wItemIdx )
{
	switch( GetItemKind( wItemIdx ) )
	{
	case eYOUNGYAK_ITEM:
	case eYOUNGYAK_ITEM_PET:
	case eYOUNGYAK_ITEM_UPGRADE_PET:
	case eYOUNGYAK_ITEM_TITAN:
	case eEXTRA_ITEM_JEWEL:
	case eEXTRA_ITEM_MATERIAL:
	case eEXTRA_ITEM_METAL:
	case eEXTRA_ITEM_BOOK:
	case eEXTRA_ITEM_HERB:
	case eEXTRA_ITEM_ETC:
	case eEXTRA_ITEM_USABLE:
	case eSHOP_ITEM_CHARM:
	case eSHOP_ITEM_HERB:
		return TRUE;
	case eSHOP_ITEM_SUNDRIES:
		{
			ITEM_INFO* pItem = GetItemInfo( wItemIdx );
			if( !pItem )			return FALSE;
			
			if( pItem->SimMek )			// ShopItem에서 ChangeItem 인것들
				return FALSE;
			else if( pItem->CheRyuk )	// 노점
				return FALSE;
			else if( wItemIdx == eSundries_Shout )
				return FALSE;			
		}
		return TRUE;
	case eSHOP_ITEM_INCANTATION:
		{
			if( wItemIdx == eIncantation_TownMove15 || wItemIdx == eIncantation_MemoryMove15 ||
				wItemIdx == eIncantation_TownMove7 || wItemIdx == eIncantation_MemoryMove7 ||
				wItemIdx == eIncantation_TownMove7_NoTrade || wItemIdx == eIncantation_MemoryMove7_NoTrade ||
				wItemIdx == 55357 || wItemIdx == 55362 || wItemIdx == eIncantation_MemoryMoveExtend || wItemIdx == eIncantation_MemoryMoveExtend7 ||
				wItemIdx == eIncantation_MemoryMove2 || wItemIdx == eIncantation_MemoryMoveExtend30 ||
				wItemIdx == eIncantation_ShowPyoguk || wItemIdx == eIncantation_ChangeName ||
				wItemIdx == eIncantation_ChangeName_Dntrade ||
				wItemIdx == eIncantation_Tracking || wItemIdx == eIncantation_Tracking_Jin ||
				wItemIdx == eIncantation_ChangeJob ||
				wItemIdx == eIncantation_ShowPyoguk7 || wItemIdx == eIncantation_ShowPyoguk7_NoTrade ||
				 wItemIdx == eIncantation_Tracking7 ||  wItemIdx == eIncantation_Tracking7_NoTrade ||
				 wItemIdx== eIncantation_MugongExtend ||	wItemIdx == eIncantation_PyogukExtend ||
				 wItemIdx == eIncantation_InvenExtend ||	wItemIdx == eIncantation_CharacterSlot ||
				 wItemIdx== eIncantation_MugongExtend2 ||	wItemIdx == eIncantation_PyogukExtend2 ||
				 wItemIdx == eIncantation_InvenExtend2 ||	wItemIdx == eIncantation_CharacterSlot2
				)
				return FALSE;

			ITEM_INFO* pItem = GetItemInfo( wItemIdx );
			if( !pItem )			return FALSE;

			if( pItem->LimitLevel && pItem->SellPrice )
				return FALSE;
		}
		return TRUE;
	case eSHOP_ITEM_NOMALCLOTHES_SKIN:
	case eSHOP_ITEM_COSTUME_SKIN:	
		{
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

void CItemManager::SendErrorMsg( CPlayer * pPlayer, MSG_ITEM_ERROR * msg, int msgSize, int ECode )
{
	msg->ECode = ECode;
	pPlayer->SendMsg(msg, msgSize);
}
void CItemManager::SendAckMsg( CPlayer * pPlayer, MSGBASE * msg, int msgSize)
{
	pPlayer->SendMsg(msg, msgSize);
}

void CItemManager::SendGuildErrorMsg( CPlayer * pPlayer, BYTE Protocol, int ECode )
{
	MSG_ITEM_ERROR msg;
	msg.Category = MP_ITEM;
	msg.Protocol = Protocol;
	msg.dwObjectID = pPlayer->GetID();
	msg.ECode = ECode;
	pPlayer->SendMsg(&msg, sizeof(msg));
}

int CItemManager::MoveItem( CPlayer * pPlayer, WORD FromItemIdx, POSTYPE FromPos, WORD ToItemIdx, POSTYPE& ToPos)
{
	CItemSlot * pFromSlot	= pPlayer->GetSlot(FromPos);
	CItemSlot * pToSlot		= pPlayer->GetSlot(ToPos);

	// check hacking
	if( !CheckHackItemMove( pPlayer, pFromSlot, pToSlot ) )	return eItemUseErr_Move;

	if( LOOTINGMGR->IsLootedPlayer( pPlayer->GetID() ) )
	{
		return 10;
	}

	if(FromPos == ToPos)
		return 1; 

	if(!CHKRT->ItemOf(pPlayer, FromPos, FromItemIdx,0,0,CB_EXIST|CB_ICONIDX))
		return 2;

	if(!CHKRT->ItemOf(pPlayer, ToPos, ToItemIdx,0,0,CB_ICONIDX))
		return 3;
	
	// RaMa - 04.01.24  ->아이템몰창고에 아이템 넣는게 가능하도록 변경
	if( pToSlot == pPlayer->GetSlot(eItemTable_Shop) )
	{
		if( (pFromSlot != pPlayer->GetSlot(eItemTable_Shop)) &&
			(pFromSlot != pPlayer->GetSlot(eItemTable_ShopInven)) )
			return 11;
	}

	const ITEMBASE * pItem = pFromSlot->GetItemInfoAbs(FromPos);
	if( !pItem )		return eItemUseErr_Err;
	// 기간제 주문서는 사용중일때 이동불가
	ITEM_INFO* pInfo = GetItemInfo( FromItemIdx );
	if( !pInfo )		return eItemUseErr_Err;

	// 에러체크는 사용하는데서
	ITEM_INFO* pToInfo = GetItemInfo( ToItemIdx );

	// magi82 - Titan(070222)
	if(pInfo->ItemKind & eTITAN_EQUIPITEM)
	{
		// 펫인벤, 창고에서 타이탄 아이템을 장착할순없음
		if((pFromSlot == pPlayer->GetSlot(eItemTable_PetInven) && pToSlot == pPlayer->GetSlot(eItemTable_Titan))
			|| (pFromSlot == pPlayer->GetSlot(eItemTable_Pyoguk) && pToSlot == pPlayer->GetSlot(eItemTable_Titan)))
			return eItemUseErr_Err;

		// magi82 - Titan(071018)
		// 타이탄 장착아이템은 문파창고에 들어갈수 없다.
		if( pToSlot == pPlayer->GetSlot(eItemTable_MunpaWarehouse) )
			return eItemUseErr_Err;
		
		//2007. 10. 25. CBH - 타이탄 장비 착용시 레벨제한 체크 (표국 이동은 레벨제한 체크를 안한다)
		if( pToSlot == pPlayer->GetSlot(eItemTable_Titan) )
		{
			if(pInfo->LimitLevel > pPlayer->GetLevel())
				return eItemUseErr_Err;
		}		
	}

	if( pInfo->ItemType == 11 )
	{
		if( pInfo->ItemKind != eSHOP_ITEM_MAKEUP && pInfo->ItemKind != eSHOP_ITEM_DECORATION && pInfo->ItemKind != eSHOP_ITEM_EQUIP && pInfo->ItemKind != eSHOP_ITEM_PET && pInfo->ItemKind != eSHOP_ITEM_PET_EQUIP  && pInfo->ItemKind != eSHOP_ITEM_TITAN_EQUIP)	// magi82(26)
		{
			if( !(pItem->ItemParam & ITEM_PARAM_SEAL) )
			if( pFromSlot != pToSlot )
				return eItemUseErr_Err;
		}
	}
	if( pInfo->ItemKind == eSHOP_ITEM_EQUIP && pToSlot == pPlayer->GetSlot(eItemTable_Weared) ||
		pInfo->ItemKind == eSHOP_ITEM_PET_EQUIP && pToSlot == pPlayer->GetSlot(eItemTable_PetWeared) ||
		pInfo->ItemKind == eSHOP_ITEM_TITAN_EQUIP && pToSlot == pPlayer->GetSlot(eItemTable_TitanShopItem))	// magi82(26)
	{
		if( pItem->ItemParam & ITEM_PARAM_SEAL )
			return eItemUseErr_Err;
		else
		{
			SHOPITEMWITHTIME* pusingitem = pPlayer->GetShopItemManager()->GetUsingItemInfo( FromItemIdx );
			if( pusingitem )
			{
				if( pusingitem->ShopItem.ItemBase.dwDBIdx != pItem->dwDBIdx )
					return eItemUseErr_Err;
			}
			else
			{
				// magi82(29)
				IsAbleShopEquipItem( pPlayer->GetID(), pItem->dwDBIdx, FromItemIdx, FromPos, ToItemIdx, ToPos );
				return 99;
			}
		}
	}
	//소환펫 다른 슬롯 이동 막기
	if( pFromSlot != pToSlot )
	{
		if( TRUE == IsPetSummonItem(FromItemIdx) )
		if( TRUE == pPlayer->GetPetManager()->IsCurPetSummonItem(pItem->dwDBIdx) )
			return eItemUseErr_Err;
	}

	const ITEMBASE* pToItem = pToSlot->GetItemInfoAbs(ToPos);
	if( pToItem )
	{
		if( pToInfo && pToInfo->ItemType == 11 )
		if( pToInfo->ItemKind != eSHOP_ITEM_MAKEUP && pToInfo->ItemKind != eSHOP_ITEM_DECORATION && pToInfo->ItemKind != eSHOP_ITEM_EQUIP && pToInfo->ItemKind != eSHOP_ITEM_PET && pToInfo->ItemKind != eSHOP_ITEM_PET_EQUIP && pToInfo->ItemKind != eSHOP_ITEM_TITAN_EQUIP )
		{
			if( !(pToItem->ItemParam & ITEM_PARAM_SEAL) )
			if( pFromSlot != pToSlot )
				return eItemUseErr_Err;
		}
		if( pToInfo && (pToInfo->ItemKind & eSHOP_ITEM_EQUIP) )
		{
			if( TP_PYOGUK_START <= FromPos && FromPos < TP_PYOGUK_END )
				return eItemUseErr_Err;
            if( (pToItem->ItemParam & ITEM_PARAM_SEAL) &&
				 TP_WEAR_START <= FromPos && FromPos < TP_WEAR_END )
				 return eItemUseErr_Err;
		}
		
	}
	//소환펫 다른 슬롯 이동 막기
	if( pFromSlot != pToSlot )
	{
		if( TRUE == IsPetSummonItem(ToItemIdx) )
		if( TRUE == pPlayer->GetPetManager()->IsCurPetSummonItem(pToItem->dwDBIdx) )
			return eItemUseErr_Err;
	}

	if(!CanbeMoved(pItem->wIconIdx,ToPos,pPlayer))
	{
		ASSERT(0); //A¡þ￠OoAI¨uⓒ￡¨¡￠c￠?￠®¨u¡ⓒ Aⓒø￠￢￠c Cⓒ￢￠￥A￥i¡I.
		return 9;
	}
	if(!pToSlot->IsEmpty(ToPos))
	{
		pItem = pToSlot->GetItemInfoAbs(ToPos);
		if(pItem)
		{
			// Shop장비아이템과 일반장비아이템의 교환은 따로 체크한다.
			// CanbeMoved에서 하려니 힘들다.
			ITEM_INFO* pInfo = GetItemInfo( FromItemIdx );
			ITEM_INFO* pToInfo = GetItemInfo( ToItemIdx );
			if( !pInfo || !pToInfo )			return 10;
			if( pInfo->ItemKind == eSHOP_ITEM_EQUIP && pToInfo->ItemKind & eEQUIP_ITEM )
			{
				CItemSlot* pslot = pPlayer->GetSlot( eItemTable_Inventory );
				WORD pos = 0;
				if( pslot->GetEmptyCell( &pos ) == 0 )
					return 10;
			}
			else if( pInfo->ItemKind & eEQUIP_ITEM && pToInfo->ItemKind == eSHOP_ITEM_EQUIP )
			{
				CItemSlot* pslot = pPlayer->GetSlot( eItemTable_ShopInven );
				WORD pos = 0;
				if( pslot->GetEmptyCell( &pos ) == 0 )
					return 10;
			}
			else if(CanbeMoved(pItem->wIconIdx, FromPos, pPlayer) == FALSE)
			{
				ASSERT(0); //A¡þ￠OoAI¨uⓒ￡¨¡￠c￠?￠®¨u¡ⓒ Aⓒø￠￢￠c Cⓒ￢￠￥A￥i¡I.
				return 10;
			}
		}
	}
	// From A¡¿AI¨￢i￠?￠®¨u¡ⓒ ¨u¨¡AIAU Ao￠?o
	// 1.
	ITEMBASE DelFromItem;
	if(pFromSlot->DeleteItemAbs(pPlayer, FromPos, &DelFromItem) != EI_TRUE)
	{
		// Ao￠?i¡¤tA ¨u¨¡AIAU(FromPosition)AI ¨uⓒª￠￥U 
		return 5;
	}

	ClearQuickPos(&DelFromItem, ToPos);
	// ToTable￠?￠®¨u¡ⓒ ¨u¨¡AIAU Ao￠?o
	// 2.`
	ITEMBASE DelToItem;
	DelToItem.dwDBIdx = 0;
	if(pToSlot->DeleteItemAbs(pPlayer, ToPos, &DelToItem) != EI_TRUE)
	{
		// 0->1
		DelFromItem.Position = ToPos;
		// 3.
		if(pToSlot->InsertItemAbs(pPlayer, ToPos, &DelFromItem) != EI_TRUE)
		{
			//AIⓒoI ITEM ￥ie¨ui AO￠￥U.
			// 1.8¡¤I RollBack
			DelFromItem.Position = FromPos;
			if(pFromSlot->InsertItemAbs(pPlayer, FromPos, &DelFromItem) != EI_TRUE)
				ASSERT(0);
			return 6;
		}
	}
	else
	{
		// 1<->1
		// PosⓒoUⓒ÷U￥iU ¡Æ¡E¡Æ¡E Insert
		// 4.
		SWAPVALUE(DelFromItem.Position, DelToItem.Position);
		ClearQuickPos(&DelToItem, FromPos);
		if(pToSlot->InsertItemAbs(pPlayer, ToPos, &DelFromItem) != EI_TRUE)
		{
			// 1. RollBack
			// 2. RollBack
			SWAPVALUE(DelFromItem.Position, DelToItem.Position);
			if(pFromSlot->InsertItemAbs(pPlayer, FromPos, &DelFromItem) != EI_TRUE)
				ASSERT(0);
			
			if(pToSlot->InsertItemAbs(pPlayer, ToPos, &DelToItem) != EI_TRUE)
				ASSERT(0);
			return 7;
		}
		// 장착창에서 일반아이템이 떨어질 경우 아이템몰과 구분해줘야 한다.
		ITEM_INFO* pToInfo = GetItemInfo( ToItemIdx );
		if( pToInfo->ItemKind == eSHOP_ITEM_EQUIP )
		{
			if( TP_INVENTORY_START <= FromPos && FromPos < TP_INVENTORY_END )
			{
				pFromSlot = pPlayer->GetSlot( eItemTable_ShopInven );
				pFromSlot->GetEmptyCell( &FromPos );
			}
		}
		else if( pToInfo->ItemKind & eEQUIP_ITEM )
		{
			if( TP_SHOPINVEN_START <= FromPos && FromPos < TP_SHOPINVEN_END )
			{
				pFromSlot = pPlayer->GetSlot( eItemTable_Inventory );
				pFromSlot->GetEmptyCell( &FromPos );
			}
		}
		if(pFromSlot->InsertItemAbs(pPlayer, FromPos, &DelToItem) != EI_TRUE)
		{
			// 1. RollBack
			// 2. RollBack
			// 4. RollBack
			SWAPVALUE(DelFromItem.Position, DelToItem.Position);
			if(pFromSlot->InsertItemAbs(pPlayer, FromPos, &DelFromItem) != EI_TRUE)
				ASSERT(0);
			ClearQuickPos(&DelToItem, ToPos);
			if(pToSlot->InsertItemAbs(pPlayer, ToPos, &DelToItem) != EI_TRUE)
				ASSERT(0);
			return 8;
		}
	}

	// DB￠?￠® DBIndex¡¤I d¨￢￠￢ update
	// position￠￢￠￢ ¨￢?E¡ⓒ￥iE￠￥U
	// ToItemBase->dwDBIdx == 0 AI￠￢e ToItem: ¨uEⓒoUⓒ÷U￠￥U.
	if(TP_GUILDWAREHOUSE_START <= FromPos && FromPos < TP_GUILDWAREHOUSE_END)
	{	//RemoveGuildItemOption 레어옵션도 같이 해준다.
		RemoveGuildItemOption(ToPos, pToSlot->GetItemInfoAbs(ToPos), pPlayer);
	}

    // 06.12.06 RaMa - 버프로남는 아이템의 캐릭터사용정보가 바뀌는 현상
	if( TP_SHOPINVEN_START <= DelFromItem.Position && DelFromItem.Position < TP_SHOPINVEN_END )
	{
		if( pInfo->ItemType == 11 && DelFromItem.ItemParam == 0 )
			ShopItemUpdateCharacterIdx( pPlayer->GetID(), DelFromItem.dwDBIdx );
	}


	//
	// RaMa - 보부상판매아이템중 무기류 - 함수로라도 빼고싶지만 귀찮다.. 나중에 보는사람이 빼줘요.. 아.. 나죽는다.
	BOOL bAvatarChange = FALSE;
	WORD AvatarIdx = 0;
	WORD AvatarPos = 0;
	if( ToPos == 81 )
	{
		if( pInfo->ItemKind == eEQUIP_ITEM_WEAPON || pInfo->ItemKind == eSHOP_ITEM_EQUIP || pInfo->ItemKind == eEQUIP_ITEM_UNIQUE)
		{
			// magi82(12) - Item(071026) 이벤트무기(뿅망치, 벙어리장갑 등) 예외처리를 해줌. 안해주면 메모리 충돌이 일어남
			if( pToInfo && pInfo->WeaponType != pToInfo->WeaponType 
				&& pToInfo->WeaponType != WP_EVENT && pToInfo->WeaponType != WP_EVENT_HAMMER )
			{
				int avatarpos = eAvatar_Weared_Gum+pToInfo->WeaponType-1;
				if( pPlayer->GetShopItemStats()->Avatar[avatarpos] > 1 )
				{
					SHOPITEMWITHTIME* psItem = NULL;
					ITEM_INFO* ptInfo = ITEMMGR->GetItemInfo( pPlayer->GetShopItemStats()->Avatar[avatarpos] );
					if( ptInfo )
					{
						ShopItemUseParamUpdateToDB( 0, pPlayer->GetID(), ptInfo->ItemIdx, ptInfo->SellPrice );
						if( psItem = pPlayer->GetShopItemManager()->GetUsingItemInfo( ptInfo->ItemIdx ) )
							psItem->ShopItem.Param = ptInfo->SellPrice;
					}

					AVATARITEM* pTemp = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pPlayer->GetShopItemStats()->Avatar[eAvatar_Dress] );				
					if( pTemp )
						pPlayer->GetShopItemStats()->Avatar[avatarpos] = pTemp->Item[avatarpos];
					else
						pPlayer->GetShopItemStats()->Avatar[avatarpos] = 1;

					bAvatarChange = TRUE;
					if (psItem)//陋구嶠포굼웰?
					{
						AvatarIdx = psItem->ShopItem.ItemBase.wIconIdx;
						AvatarPos = psItem->ShopItem.ItemBase.Position;
					}
				
				}
			}
		}
	}
	else if( FromPos == 81 )
	{
		if( pInfo->ItemKind == eEQUIP_ITEM_WEAPON || pInfo->ItemKind == eSHOP_ITEM_EQUIP || pInfo->ItemKind == eEQUIP_ITEM_UNIQUE)
		{
			// magi82(12) - Item(071026) 이벤트무기(뿅망치, 벙어리장갑 등) 예외처리를 해줌. 안해주면 메모리 충돌이 일어남
			if( pInfo->WeaponType != WP_EVENT && pInfo->WeaponType != WP_EVENT_HAMMER )
			{
				SHOPITEMWITHTIME* psItem = NULL;
				ITEM_INFO* ptInfo = NULL;
				AVATARITEM* pTemp = NULL;
				int avatarpos = eAvatar_Weared_Gum+pInfo->WeaponType-1;
				if( pPlayer->GetShopItemStats()->Avatar[avatarpos] > 1 )
				{
					ptInfo = ITEMMGR->GetItemInfo( pPlayer->GetShopItemStats()->Avatar[avatarpos] );
					if( ptInfo )
					{
						ShopItemUseParamUpdateToDB( 0, pPlayer->GetID(), ptInfo->ItemIdx, ptInfo->SellPrice );
						if( psItem = pPlayer->GetShopItemManager()->GetUsingItemInfo( ptInfo->ItemIdx ) )
							psItem->ShopItem.Param = ptInfo->SellPrice;
					}

					pTemp = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pPlayer->GetShopItemStats()->Avatar[eAvatar_Dress] );				
					if( pTemp )
						pPlayer->GetShopItemStats()->Avatar[avatarpos] = pTemp->Item[avatarpos];
					else
						pPlayer->GetShopItemStats()->Avatar[avatarpos] = 1;

					bAvatarChange = TRUE;
					AvatarIdx = psItem->ShopItem.ItemBase.wIconIdx;
					AvatarPos = psItem->ShopItem.ItemBase.Position;
				}
			}
		}
	}

	if( bAvatarChange )
	{
		SEND_AVATARITEM_INFO msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_INFO;
		msg.PlayerId = pPlayer->GetID();
		msg.ItemIdx = AvatarIdx;
		msg.ItemPos = AvatarPos;
		memcpy( &msg.Avatar, pPlayer->GetShopItemStats()->Avatar, sizeof(WORD)*eAvatar_Max );
		PACKEDDATA_OBJ->QuickSend( pPlayer, &msg, sizeof(msg) );	

		pPlayer->GetShopItemManager()->CalcAvatarOption();
	}

	pPlayer->GetTitanManager()->MoveTitanEquipItemUpdateToDB(&DelFromItem, &DelToItem, FromPos, ToPos);

	if(ItemMoveUpdateToDBbyTable(pPlayer, DelFromItem.dwDBIdx, DelFromItem.wIconIdx, DelFromItem.Position, DelToItem.dwDBIdx, DelToItem.Position) == TRUE)
	{
		// Log
		ItemMoveLog(FromPos, ToPos, pPlayer, &DelFromItem);
		if(DelToItem.dwDBIdx != 0)
			ItemMoveLog(ToPos, FromPos, pPlayer, &DelToItem);
	}

	return 0;
}

int CItemManager::DivideItem( CPlayer* pPlayer, WORD ItemIdx,  POSTYPE FromPos, POSTYPE ToPos, DURTYPE FromDur, DURTYPE ToDur)
{
	if(FromPos == ToPos)
		return 1;

//KES add 061031
	if(TP_INVENTORY_START <= FromPos && FromPos < TP_WEAR_END)
	{
	}
	else if(TP_SHOPINVEN_START <= FromPos && FromPos < TP_SHOPINVEN_END)
	{
	}
	else
	{
		return 1;
	}
//-----
	
	if(!CHKRT->ItemOf(pPlayer, FromPos, ItemIdx, FromDur+ToDur, 0, CB_EXIST|CB_ICONIDX|CB_DURA))
		return 2;

	// ¨u¨¡AIAU xAcCIAo ¨uE￠￥A A¨uA¨I
	if(CHKRT->ItemOf(pPlayer, ToPos,0,0,0,CB_EXIST))
		return 3;

	if( LOOTINGMGR->IsLootedPlayer( pPlayer->GetID() ) )
	{
		return 10;
	}

	CItemSlot * pFromSlot = pPlayer->GetSlot(FromPos);
	const ITEMBASE * FromItemBase = pFromSlot->GetItemInfoAbs( FromPos );

    if( !IsDupItem( FromItemBase->wIconIdx ) )
	{
		return 4;
	}		

	//////////////////////////////////////////////////////////////////////////
	//itemmaxcheck
//	if( IsDupItem(FromItemBase->wIconIdx) )
//	{
		if( FromItemBase->Durability > MAX_YOUNGYAKITEM_DUPNUM )
			return 1;
		if( FromDur + ToDur > MAX_YOUNGYAKITEM_DUPNUM )
			return 1;
		if( FromDur > MAX_YOUNGYAKITEM_DUPNUM )
			return 1;
		if( ToDur > MAX_YOUNGYAKITEM_DUPNUM )
			return 1;
		if( FromDur == 0 ) return 1;
		if( ToDur == 0 ) return 1;
//	}
	//////////////////////////////////////////////////////////////////////////

	if( pFromSlot->UpdateItemAbs(pPlayer, FromPos, FromItemBase->dwDBIdx, 0, 0, 0, FromDur, UB_DURA, SS_CHKDBIDX ) != EI_TRUE )
	{
		return 7;
	}

//copy item ?
//	if( pFromSlot->UpdateItemAbs(pPlayer, FromPos, 0, 0, 0, 0, FromDur, UB_DURA, SS_PREINSERT ) != EI_TRUE )
//	{
//		return 7;
//	}

	ITEMOBTAINARRAYINFO * pArrayInfo = Alloc(pPlayer, MP_ITEM, MP_ITEM_DIVIDE_ACK, pPlayer->GetID(), pPlayer->GetID(), eLog_ItemObtainDivide, 2, (DBResult)DivideItemDBResult);
	pArrayInfo->ItemArray.AddItem( FromItemBase->dwDBIdx, 
									FromItemBase->wIconIdx, 
									FromDur, 
									FromItemBase->Position, 
									FromItemBase->QuickPosition,
									FromItemBase->ItemParam );

	//updateDB
	ItemUpdatebyTable(pPlayer, FromItemBase->dwDBIdx, FromItemBase->wIconIdx, FromDur, FromItemBase->Position, FromItemBase->QuickPosition);
	
	WORD EmptyCellPos[1];
	EmptyCellPos[0] = ToPos;
	WORD EmptyCellNum = 1;
	return ObtainItemEx(pPlayer, pArrayInfo, FromItemBase->wIconIdx, (WORD)ToDur, EmptyCellPos, EmptyCellNum, EmptyCellNum+1);
}

void CItemManager::DivideItemDBResult(CPlayer* pPlayer, WORD ArrayID)
{
	ObtainItemDBResult(pPlayer, ArrayID);
}

int CItemManager::CombineItem( CPlayer* pPlayer, WORD ItemIdx, POSTYPE FromPos, POSTYPE ToPos, DURTYPE & FromDur, DURTYPE & ToDur)
{
	if(FromPos == ToPos)
		return 1;

	if(!CHKRT->ItemOf(pPlayer, FromPos, ItemIdx,FromDur,0,CB_EXIST|CB_ICONIDX|CB_DURA))
		return 2;

	if(!CHKRT->ItemOf(pPlayer, ToPos, ItemIdx,ToDur,0,CB_EXIST|CB_ICONIDX|CB_DURA))
		return 3;

	if( !IsDupItem(ItemIdx) )
	{
		return 4;
	}

	if( LOOTINGMGR->IsLootedPlayer( pPlayer->GetID() ) )
	{
		return 10;
	}

	CItemSlot * pFromSlot = pPlayer->GetSlot(FromPos);
	CItemSlot * pToSlot = pPlayer->GetSlot(ToPos);

	// A¨uA¨I
	const ITEMBASE * FromItemBase = pFromSlot->GetItemInfoAbs( FromPos );
	const ITEMBASE * ToItemBase = pToSlot->GetItemInfoAbs( ToPos );

	// 봉인추가
	if( (FromItemBase->ItemParam & ITEM_PARAM_SEAL) ||
		(ToItemBase->ItemParam & ITEM_PARAM_SEAL ) )
	{
		return eItemUseErr_Err;
	}
	
	//////////////////////////////////////////////////////////////////////////
	//itemmaxcheck
	if( IsDupItem(FromItemBase->wIconIdx) && FromItemBase->Durability > MAX_YOUNGYAKITEM_DUPNUM )
		return 1;
	if( IsDupItem(ToItemBase->wIconIdx) && ToItemBase->Durability > MAX_YOUNGYAKITEM_DUPNUM )
		return 1;
	//////////////////////////////////////////////////////////////////////////

	
	DURTYPE SumDur = FromItemBase->Durability + ToItemBase->Durability;
	if( SumDur > MAX_YOUNGYAKITEM_DUPNUM )
	{
		if(pFromSlot->UpdateItemAbs(pPlayer, FromPos, 0, 0, 0, 0, (SumDur - MAX_YOUNGYAKITEM_DUPNUM), UB_DURA ) != EI_TRUE )
		{
			return 5;
		}
		if(pToSlot->UpdateItemAbs(pPlayer, ToPos, 0, 0, 0, 0, MAX_YOUNGYAKITEM_DUPNUM, UB_DURA ) != EI_TRUE)
		{
			return 6;
		}

		//DB
		// update2
		ItemCombineUpdateToDB(FromItemBase->dwDBIdx, FromItemBase->Durability, ToItemBase->dwDBIdx, ToItemBase->Durability);

		FromDur		= (SumDur - MAX_YOUNGYAKITEM_DUPNUM);
		ToDur		= MAX_YOUNGYAKITEM_DUPNUM;
	}
	else
	{
		ITEMBASE fromItemOut;
		if(pFromSlot->DeleteItemAbs(pPlayer,  FromPos, &fromItemOut ) != EI_TRUE)
		{
			return 7;
		}
		
		if(pToSlot->UpdateItemAbs(pPlayer, ToPos, 0, 0, 0, 0, SumDur, UB_DURA ) != EI_TRUE)
		{
			return 8;
		}

		//DB
		// delete
		// update
		ItemCombineUpdateToDB(fromItemOut.dwDBIdx, 0, ToItemBase->dwDBIdx, ToItemBase->Durability);
		
		FromDur		= 0;
		ToDur		= SumDur;
	}
	return 0;
}
int CItemManager::DiscardItem(CPlayer* pPlayer, POSTYPE whatPos, WORD whatItemIdx, DURTYPE whatItemNum)
{
	CItemSlot * pSlot = pPlayer->GetSlot(whatPos);
	if( !pSlot )	return 1;

	const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(whatPos);
	
	if(pItemBase->wIconIdx != whatItemIdx) // discard just after move item... 
		return 4;
	//////////////////////////////////////////////////////////////////////////
	//itemmaxcheck
	if( IsDupItem(pItemBase->wIconIdx) && pItemBase->Durability > MAX_YOUNGYAKITEM_DUPNUM )
		return 1;
	//////////////////////////////////////////////////////////////////////////

	if(IsDupItem(pItemBase->wIconIdx) && (pItemBase->Durability > whatItemNum))
	{
		if( pSlot->UpdateItemAbs(pPlayer, whatPos, 0, 0, 0, 0, pItemBase->Durability - whatItemNum, UB_DURA ) != EI_TRUE )
		{
			return 2;
		}

		ItemUpdateToDB(pPlayer->GetID(), pItemBase->dwDBIdx, pItemBase->wIconIdx, 
				pItemBase->Durability, pItemBase->Position, pItemBase->QuickPosition, pItemBase->RareIdx );
	}
	else
	{
		// ItemShop
		ITEM_INFO* pItemInfo = GetItemInfo( pItemBase->wIconIdx );
		if( !pItemInfo )	return eItemUseErr_Discard;

		ITEMBASE DelItemOut;
		SHOPITEMWITHTIME* pUsingItem = pPlayer->GetShopItemManager()->GetUsingItemInfo( whatItemIdx );
		if( pUsingItem )
		{
			//KES modify 060505
			if( pUsingItem->ShopItem.ItemBase.dwDBIdx == pItemBase->dwDBIdx )
			{
				pPlayer->GetShopItemManager()->DeleteUsingShopItem( whatItemIdx );

				if( pItemInfo->ItemType == 11 )
				{
					if( pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION )
						pPlayer->GetShopItemManager()->DiscardAvatarItem( whatItemIdx, whatPos );					
				}
			}
		}
		
		if( pSlot->DeleteItemAbs(pPlayer, whatPos, &DelItemOut, SS_LOCKOMIT ) != EI_TRUE )
			return 3;		

		//db
		ItemDeleteToDB(DelItemOut.dwDBIdx);
		if(IsOptionItem(DelItemOut.wIconIdx, DelItemOut.Durability))
		{
			// 로그추가
			ITEM_OPTION_INFO* pInfo = pPlayer->GetItemOption(DelItemOut.Durability);
			if( pInfo )
			{
				ITEM_OPTION_INFO info = *pInfo;
				info.dwOptionIdx = 0;
				LogItemOption( pPlayer->GetID(), DelItemOut.dwDBIdx, &info );
			}
			//

			if( FALSE == pPlayer->RemoveItemOption(DelItemOut.Durability) )
			{
				char szBuf[128];
				sprintf( szBuf, "DeleteOptItem_Error! PlayerID : %d\t ItemDBIdx : %d\n", pPlayer->GetID()
					,DelItemOut.wIconIdx );
				ASSERTMSG(0, szBuf);
			}
			CharacterItemOptionDelete(DelItemOut.Durability, DelItemOut.dwDBIdx);
		}
		//SW050920 Rare
		if(IsRareOptionItem(DelItemOut.wIconIdx, DelItemOut.RareIdx))
		{
			// 로그추가
			ITEM_RARE_OPTION_INFO* pInfo = pPlayer->GetItemRareOption(DelItemOut.RareIdx);
			if( pInfo )
			{
				ITEM_RARE_OPTION_INFO info = *pInfo;
				info.dwRareOptionIdx = 0;
				LogItemRareOption( pPlayer->GetID(), DelItemOut.dwDBIdx, &info );
			}
			//

			if( FALSE == pPlayer->RemoveItemRareOption(DelItemOut.RareIdx) )
			{
				char szBuf[128];
				sprintf( szBuf, "DeleteRareItem_Error! PlayerID : %d\t ItemDBIdx : %d\n", pPlayer->GetID()
					,DelItemOut.wIconIdx );
				ASSERTMSG(0, szBuf);
			}
			CharacterItemRareOptionDelete(DelItemOut.RareIdx, DelItemOut.dwDBIdx);
		}
		//pet
		if( IsPetSummonItem(DelItemOut.wIconIdx) )
		{
			pPlayer->GetPetManager()->DeletePet(DelItemOut.dwDBIdx);
		}

		if( IsTitanCallItem(DelItemOut.wIconIdx) )
		{
			pPlayer->GetTitanManager()->DeleteTitan(DelItemOut.dwDBIdx);
		}

		if( IsTitanEquipItem(DelItemOut.wIconIdx) )
		{
			pPlayer->GetTitanManager()->DeleteTitanEquip(DelItemOut.dwDBIdx);
		}
		//2007. 12. 11. CBH - 스킨 아이탬 관련 처리
		if( (pItemInfo->ItemKind == eSHOP_ITEM_NOMALCLOTHES_SKIN) || (pItemInfo->ItemKind == eSHOP_ITEM_COSTUME_SKIN) )
			pPlayer->GetShopItemManager()->DiscardSkinItem( whatItemIdx );					
	}

	return 0;
}

int CItemManager::SellItem( CPlayer* pPlayer, POSTYPE whatPos, WORD wSellItemIdx, DURTYPE sellItemDur, DWORD DealerIdx )
{
	if(!CHKRT->StateOf(pPlayer, eObjectState_Deal))
		return 1;

	if(!CHKRT->ItemOf(pPlayer, whatPos, wSellItemIdx,sellItemDur,0,CB_EXIST|CB_ICONIDX|CB_ENOUGHDURA))
		return 2;

	CItemSlot * pSlot = pPlayer->GetSlot(whatPos);
	ITEMBASE Item = *pSlot->GetItemInfoAbs(whatPos);

	//money check
	ITEM_INFO * pItemInfo = GetItemInfo( wSellItemIdx );
	if( !pItemInfo )	return 1;
	MONEYTYPE money = 0;

	//////////////////////////////////////////////////////////////////////////
	//itemmaxcheck
	if( IsDupItem(wSellItemIdx) && sellItemDur > MAX_YOUNGYAKITEM_DUPNUM )
		return 1;
	//////////////////////////////////////////////////////////////////////////
	
	//SW050920 수정
	if( IsDupItem(wSellItemIdx) && !IsOptionItem(wSellItemIdx, sellItemDur) )
		money = pItemInfo->SellPrice * sellItemDur;
	else
		money = pItemInfo->SellPrice;

	money = SWPROFITMGR->AddProfitFromItemSell( money );

	// 요새전
	money = FORTWARMGR->AddProfitMoneyFromItemSell( money );

	if( !pPlayer->IsEnoughAdditionMoney(money) )
		return EI_MAXMONEY;

	if( LOOTINGMGR->IsLootedPlayer( pPlayer->GetID() ) )
	{
		return 10;
	}

	if(Item.dwDBIdx == pPlayer->GetTitanManager()->GetRegistTitanCallItemDBIdx())
	{
		// 등록된 타이탄 증서는 삭제 불가
		return 11;
	}

	//process
	if(EI_TRUE != DiscardItem(pPlayer, whatPos, wSellItemIdx, sellItemDur))
	{
		//sendErroMsg
		return 3;
	}
	
	pPlayer->SetMoney( money, MONEY_ADDITION, 0, eItemTable_Inventory, eMoneyLog_GetStall, DealerIdx );
	
	// Log
	LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), DealerIdx, "",
		eLog_ItemSell, pPlayer->GetMoney(eItemTable_Inventory), 0, money,
		Item.wIconIdx , Item.dwDBIdx, Item.Position, 0, Item.Durability, pPlayer->GetPlayerExpPoint());	
	return 0;
}

int CItemManager::BuyItem( CPlayer* pPlayer, WORD buyItemIdx, WORD buyItemNum, WORD DealerIdx )
{
	if(!CHKRT->StateOf(pPlayer, eObjectState_Deal))
		return 1;
	
	//DealerIdx￠?￠®¨u¡ⓒ ¨¡A￠￥A ¨u¨¡AIAUAI¡Æ￠®??
	if(!CHKRT->DearlerItemOf(DealerIdx, buyItemIdx))
		return 2;

	if( buyItemNum == 0 ) return EI_TRUE;	//0AI￠￢e¡¾¡¿ⓒøE~¡¤a

	//SW070626 보부상NPC
	//보부상에서의 물품갯수 처리.	// 물품 카운트 다운. 대화중 유저에게 정보 보냄.	
	if( DealerIdx == BOBUSANG_wNpcUniqueIdx )
	if( FALSE == BOBUSANGMGR->BuyItem( pPlayer, buyItemIdx, buyItemNum ) )
		return NOT_REMAINITEM;

	if( buyItemNum > MAX_ITEMBUY_NUM )
		buyItemNum = MAX_ITEMBUY_NUM;

	ITEM_INFO * pItemInfo	= GetItemInfo( buyItemIdx );
	if( !pItemInfo )
		return 3;

	// RaMa - 아이템몰 아이템을 NPC에서 구입할때.
	CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	if( pItemInfo->ItemKind & eSHOP_ITEM )
		pSlot = pPlayer->GetSlot(eItemTable_ShopInven);

	WORD obtainItemNum = 0;
	MONEYTYPE ItemPrice = pItemInfo->BuyPrice;
	if(g_pServerSystem->GetMap()->IsVillage() == FALSE)		// 마을이 아니면 1.2배
		ItemPrice = (MONEYTYPE)(ItemPrice*1.2);

	ItemPrice = SWPROFITMGR->AddProfitFromItemBuy( ItemPrice );

	// 요새전
	ItemPrice = FORTWARMGR->AddProfitMoneyFromItemBuy( ItemPrice );

	obtainItemNum = GetCanBuyNumInMoney(pPlayer, buyItemNum, ItemPrice);
	if(obtainItemNum == 0)
		return NOT_MONEY; //￥i¡¤AI ￠￢ⓒ￡AU￠￢¡I￠￥U.
	WORD EmptyCellPos[255];
	WORD EmptyCellNum;
	obtainItemNum = GetCanBuyNumInSpace(pPlayer, pSlot, buyItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum);
	if(obtainItemNum == 0)
		return NOT_SPACE;	//AU￠￢￠c¡Æ￠® ¨uⓒª￠￥U.

	int rt;
	
	if( pItemInfo->ItemKind & eSHOP_ITEM )
		rt = ObtainItemEx(pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_BUY_ACK, pPlayer->GetID(), DealerIdx, eLog_ItemObtainBuy, obtainItemNum, (DBResult)(BuyItemDBResult)), buyItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum, 1);
	else
		rt = ObtainItemEx(pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_BUY_ACK, pPlayer->GetID(), DealerIdx, eLog_ItemObtainBuy, obtainItemNum, (DBResult)(BuyItemDBResult)), buyItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum);

	if( rt == EI_TRUE )
	{
		// ￥i¡¤¡Æe¡ie
		ITEM_INFO * pTargetItem = ITEMMGR->GetItemInfo(buyItemIdx);
		pPlayer->SetMoney( ItemPrice*obtainItemNum, MONEY_SUBTRACTION, 0, eItemTable_Inventory, eMoneyLog_LoseStall, DealerIdx );
	}

	return rt;
}

void CItemManager::BuyItemDBResult(CPlayer * pPlayer, WORD ArrayID )
{
	ObtainItemDBResult(pPlayer, ArrayID);
}

// 필드보스 - 05.12 이영준
// 기존 몬스터는 무조건 아이템 하나씩만 드랍하게 되어있는것을
// 필드보스가 여러개의 아이템을 드랍하므로 수량을 받을수 있게 조정
void CItemManager::MonsterObtainItem(CPlayer * pPlayer, WORD obtainItemIdx, DWORD dwFurnisherIdx, WORD ItemNum)
{
	// 필드보스 아이템 드랍시 버퍼 사이즈때문에 오류나서 확장 2 -> 128
	WORD EmptyCellPos[128];
	WORD EmptyCellNum;
	CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	ITEM_INFO* pInfo = GetItemInfo( obtainItemIdx );
	if( !pInfo )			return;
	if( pInfo->ItemKind & eSHOP_ITEM )
		pSlot = pPlayer->GetSlot(eItemTable_ShopInven);
		
	BOOL bGetTwice = FALSE;
	ITEM_RARE_OPTION_INFO RareOptionInfo;
	BOOL bRare = RAREITEMMGR->GetRare( obtainItemIdx, &RareOptionInfo, pPlayer );
	pPlayer->GetPetManager()->GetPetBuffResultRt(ePB_Item_DoubleChance, &bGetTwice);


	int rt = 0;
	for( int i = 0; i < 1 + bGetTwice; ++i )
	{
		WORD obtainItemNum = GetCanBuyNumInSpace(pPlayer, pSlot, obtainItemIdx, ItemNum, EmptyCellPos, EmptyCellNum);
		if(obtainItemNum == 0)
		{
			//빈 슬롯 없음 경고 메시지
			MSGBASE msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_FULLINVEN_ALERT;
			pPlayer->SendMsg( &msg, sizeof(msg) );

			return;
		}

		if( pInfo->ItemKind & eSHOP_ITEM )
			rt = ObtainItemEx(pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_MONSTER_OBTAIN_NOTIFY, pPlayer->GetID(), dwFurnisherIdx, eLog_ItemObtainMonster, obtainItemNum, (DBResult)(ObtainItemDBResult), NULL), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum, 1);
		//SW050920 Rare
		else if( pInfo->ItemKind & eEQUIP_ITEM )
		{
			if( FALSE == bRare )
			{				
				// 요새전 처리
				if( FORTWARMGR->AddProfitItemFromMonster( pPlayer, obtainItemIdx, 1 ) == FALSE )
                    rt = ObtainItemEx(pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_MONSTER_OBTAIN_NOTIFY, pPlayer->GetID(), dwFurnisherIdx, eLog_ItemObtainMonster, obtainItemNum, (DBResult)(ObtainItemDBResult), NULL), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum, 0);
			}
			else
			{
				rt = ObtainRareItem(pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_MONSTER_RAREOBTAIN_NOTIFY, pPlayer->GetID(), dwFurnisherIdx, eLog_ItemObtainMonster, obtainItemNum, NULL, (DBResultEx)(RareItemDBResult)), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, &RareOptionInfo );			
			}
		}
		else
			rt = ObtainItemEx(pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_MONSTER_OBTAIN_NOTIFY, pPlayer->GetID(), dwFurnisherIdx, eLog_ItemObtainMonster, obtainItemNum, (DBResult)(ObtainItemDBResult), NULL), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum, 0);

		if(rt == 0 && pPlayer->GetPartyIdx())
		{
			PARTYMGR->SendObtainItemMsg(pPlayer, obtainItemIdx);
		}

	}
	
	return;
	//return ObtainItem(pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_MONSTER_OBTAIN_NOTIFY, pPlayer->GetID(), 1, (DBResult)ObtainItemDBResult), obtainItemIdx, 1);
}

int CItemManager::CheatObtainItem( CPlayer* pPlayer, WORD obtainItemIdx, WORD ItemNum )
{
	WORD EmptyCellPos[255];
	WORD EmptyCellNum;
	CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	WORD obtainItemNum = GetCanBuyNumInSpace( pPlayer, pSlot, obtainItemIdx, ItemNum, EmptyCellPos, EmptyCellNum );
	if(obtainItemNum == 0)
		return 1;

	return ObtainItemEx( pPlayer, Alloc(pPlayer, MP_CHEAT, MP_CHEAT_ITEM_ACK, pPlayer->GetID(), 0, eLog_ItemObtainCheat, obtainItemNum, (DBResult)(ObtainItemDBResult)), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum );
}

int CItemManager::CheatObtainRareItem(CPlayer* pPlayer, WORD obtainItemIdx, WORD ItemNum, ITEM_OPTION_INFO* pOptionInfo)
{
	WORD EmptyCellPos[255];
	WORD EmptyCellNum;
	CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	WORD obtainItemNum = GetCanBuyNumInSpace( pPlayer, pSlot, obtainItemIdx, ItemNum, EmptyCellPos, EmptyCellNum );
	if(obtainItemNum == 0)
		return 1;

	ITEM_RARE_OPTION_INFO RareOptionInfo;
	RareOptionInfo.GenGol = pOptionInfo->GenGol;
	RareOptionInfo.MinChub = pOptionInfo->MinChub;
	RareOptionInfo.CheRyuk = pOptionInfo->CheRyuk;
	RareOptionInfo.SimMek = pOptionInfo->SimMek;
	RareOptionInfo.Life = pOptionInfo->Life;
	RareOptionInfo.NaeRyuk = pOptionInfo->NaeRyuk;
	RareOptionInfo.Shield = pOptionInfo->Shield;
	RareOptionInfo.PhyAttack = pOptionInfo->PhyAttack;
	RareOptionInfo.PhyDefense = pOptionInfo->PhyDefense;
	RareOptionInfo.AttrAttack.SetElement_Val(ATTR_TREE, pOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE));
	RareOptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE, pOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE));
	RareOptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH, pOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH));
	RareOptionInfo.AttrAttack.SetElement_Val(ATTR_IRON, pOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON));
	RareOptionInfo.AttrAttack.SetElement_Val(ATTR_WATER, pOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER));
	RareOptionInfo.AttrRegist.SetElement_Val(ATTR_TREE, pOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE));
	RareOptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE, pOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE));
	RareOptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH, pOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH));
	RareOptionInfo.AttrRegist.SetElement_Val(ATTR_IRON, pOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON));
	RareOptionInfo.AttrRegist.SetElement_Val(ATTR_WATER, pOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER));
	
	return ObtainRareItem( pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_MONSTER_RAREOBTAIN_NOTIFY, pPlayer->GetID(), 0, eLog_ItemRareObtainCheat, obtainItemNum, NULL, (DBResultEx)(RareItemDBResult)), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, &RareOptionInfo );	
}

int CItemManager::ObtainRareItem(CPlayer* pPlayer, ITEMOBTAINARRAYINFO* pArrayInfo, WORD whatItemIdx, WORD whatItemNum, WORD * EmptyCellPos, WORD ArrayInfoUnitNum, ITEM_RARE_OPTION_INFO* pRareOptionInfo )
{
	if(IsDupItem(whatItemIdx)) ASSERT(0);

	WORD obtainItemNum = whatItemNum;

	ITEMBASE NewItemBase;
	NewItemBase.dwDBIdx			= 9;
	NewItemBase.wIconIdx		= 9;
	NewItemBase.Position		= 9;
	NewItemBase.QuickPosition	= 9;
	NewItemBase.Durability		= 9;
	NewItemBase.RareIdx			= 9;
	NewItemBase.ItemParam		= 9;	// magi82(28)

	CItemSlot* pSlot = pPlayer->GetSlot(EmptyCellPos[0]);
	BOOL bIsEmpty = pSlot->IsEmpty(EmptyCellPos[0]);
	if( bIsEmpty )
	{
		NewItemBase.Position = EmptyCellPos[0];
		if( pSlot->InsertItemAbs(pPlayer, EmptyCellPos[0], &NewItemBase, SS_PREINSERT) != EI_TRUE )
		{
			ASSERT(0);
			return 1;
		}

		ItemRareInsertToDB( pPlayer->GetID(), whatItemIdx, EmptyCellPos[0], MAKEDWORD(ArrayInfoUnitNum, pArrayInfo->wObtainArrayID), pRareOptionInfo );

		obtainItemNum--;
		ASSERT(!obtainItemNum);
	}
	else
	{
//			pArrayInfo->CallBack( pPlayer, pArrayInfo->wObtainArrayID );
			ASSERT(0);
			return 0;
	}

	return 0;
}

int CItemManager::ObtainItemEx(CPlayer * pPlayer, ITEMOBTAINARRAYINFO * pArrayInfo, WORD whatItemIdx, WORD whatItemNum, WORD * EmptyCellPos, WORD EmptyCellNum, WORD ArrayInfoUnitNum, WORD bSeal)
{
	WORD obtainItemIdx = whatItemIdx;
	WORD obtainItemNum = whatItemNum;
	
//	CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	//POSTYPE startPos = pSlot->GetStartPos();
	//POSTYPE EndPos = startPos +  pSlot->GetSlotNum();
	POSTYPE i = 0;
	
	ITEMBASE NewItemBase;  /* AI￠￥U ¨uⓒi½?i´ⓒo¡¾a¡Æ¨￡8¡¤I ¨uA¨¡A */
	NewItemBase.dwDBIdx			= 9;
	NewItemBase.wIconIdx		= 9;
	NewItemBase.Position		= 9;
	NewItemBase.QuickPosition	= 9;
	NewItemBase.Durability		= 9;
	// magi82(28)
	NewItemBase.ItemParam		= 9;
	NewItemBase.RareIdx			= 9;
	BOOL bDBReturn = FALSE;
	if(IsDupItem(whatItemIdx))
	{	
		for( i = 0 ; i < EmptyCellNum ; ++i )
		{
			CItemSlot * pSlot = pPlayer->GetSlot(EmptyCellPos[i]);
			const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(EmptyCellPos[i]);
			BOOL bIsEmpty = pSlot->IsEmpty(EmptyCellPos[i]);
			if( bIsEmpty )
			{
				bDBReturn = TRUE;
				if( obtainItemNum > MAX_YOUNGYAKITEM_DUPNUM )
				{
					NewItemBase.Position		= EmptyCellPos[i];
					if(pSlot->InsertItemAbs(pPlayer, EmptyCellPos[i], &NewItemBase, SS_PREINSERT) != EI_TRUE)
					{
						return 3;
						ASSERT(0);
					}
					ItemInsertToDB(pPlayer->GetID(), obtainItemIdx, MAX_YOUNGYAKITEM_DUPNUM, EmptyCellPos[i], MAKEDWORD(ArrayInfoUnitNum, pArrayInfo->wObtainArrayID), bSeal);
				//	ItemInsertbyTable(pPlayer, obtainItemIdx, MAX_YOUNGYAKITEM_DUPNUM, EmptyCellPos[i], MAKEDWORD(ArrayInfoUnitNum, pArrayInfo->wObtainArrayID) );
					obtainItemNum -= MAX_YOUNGYAKITEM_DUPNUM;
				}
				else
				{
					NewItemBase.Position		= EmptyCellPos[i]; 
					if(pSlot->InsertItemAbs(pPlayer, EmptyCellPos[i], &NewItemBase, SS_PREINSERT) != EI_TRUE)
					{
						return 4;
						ASSERT(0);
					}
					
					//apply to db
					ItemInsertToDB(pPlayer->GetID(), obtainItemIdx, obtainItemNum, EmptyCellPos[i],  MAKEDWORD(ArrayInfoUnitNum, pArrayInfo->wObtainArrayID), bSeal);
				//	ItemInsertbyTable(pPlayer, obtainItemIdx, obtainItemNum, EmptyCellPos[i], MAKEDWORD(ArrayInfoUnitNum, pArrayInfo->wObtainArrayID));
					obtainItemNum = 0;
				}
			}
			else if( !bIsEmpty &&
				pItemBase->wIconIdx == obtainItemIdx && 
				pItemBase->Durability < MAX_YOUNGYAKITEM_DUPNUM )	//$ 중복 아이템 채우기
			{				
				if( pItemBase->Durability + obtainItemNum > MAX_YOUNGYAKITEM_DUPNUM )
				{
					obtainItemNum = obtainItemNum + (WORD)pItemBase->Durability - MAX_YOUNGYAKITEM_DUPNUM;
					if( pSlot->UpdateItemAbs(pPlayer, pItemBase->Position, 0, 0, 0, 0, MAX_YOUNGYAKITEM_DUPNUM, UB_DURA, SS_PREINSERT)  != EI_TRUE )
					{
						///SendErrorMsg();
						return 1;
					}
					
					pArrayInfo->ItemArray.AddItem( pItemBase->dwDBIdx, 
						pItemBase->wIconIdx, 
						pItemBase->Durability, 
						pItemBase->Position, 
						pItemBase->QuickPosition,
						pItemBase->ItemParam );
					
					// to apply DB
					ItemUpdateToDB(pPlayer->GetID(), pItemBase->dwDBIdx, pItemBase->wIconIdx, 
								pItemBase->Durability, pItemBase->Position, pItemBase->QuickPosition );
					
					// '￠?￠®¨u¡ⓒ UpdateCI￠￥A ¨uⓒª¡Æ¡I￠?￠® ¡Æ¨￡AI ⓒoUⓒ÷i¡Æi 
					// ⓒoUⓒ÷i ¨u¨¡AIAUAC ￠￢¨­￠￢ⓒ￡￠￢￠cAO¨uO￠￢| ¨uo¨ui ￠?I¨u¡ⓒ 
					// ¨u¨¡¡¤￠®AC ⓒoo¡¾¡¿￠￢| ¨uc¡ieCⓒ￢= 
				}
				else
				{
					if( pSlot->UpdateItemAbs(pPlayer, pItemBase->Position, 0, 0, 0, 0, pItemBase->Durability + obtainItemNum, UB_DURA, SS_PREINSERT )  != EI_TRUE )
					{
						//SendErrorMsg();
						return 2;
					}
					
					pArrayInfo->ItemArray.AddItem( pItemBase->dwDBIdx, 
						pItemBase->wIconIdx, 
						pItemBase->Durability, 
						pItemBase->Position, 
						pItemBase->QuickPosition,
						pItemBase->ItemParam );
					
					// to apply DB
					ItemUpdateToDB(pPlayer->GetID(), pItemBase->dwDBIdx, pItemBase->wIconIdx, 
								pItemBase->Durability, pItemBase->Position, pItemBase->QuickPosition );
					
					obtainItemNum = 0;
				}
			}
			else
				ASSERT(0);
		}
		
		if( bDBReturn == FALSE )
		{
			pArrayInfo->CallBack( pPlayer, (WORD)pArrayInfo->wObtainArrayID );
			return 0;
		}
	}
	else	//$ 중복불가 아이템 처리
	{
		for( i = 0 ; i < EmptyCellNum ; ++i )
		{
			CItemSlot * pSlot = pPlayer->GetSlot(EmptyCellPos[i]);
			if( pSlot->IsEmpty(EmptyCellPos[i]) )
			{
				NewItemBase.Position		= EmptyCellPos[i]; 
				if( bSeal )
					NewItemBase.ItemParam = 1;
				if(pSlot->InsertItemAbs(pPlayer, EmptyCellPos[i], &NewItemBase, SS_PREINSERT) != EI_TRUE)
				{
					return 5;
					ASSERT(0);
				}
				--obtainItemNum;
				//CharacterItemInsert(1, pPlayer->GetID(), obtainItemIdx, MAX_YOUNGYAKITEM_DUPNUM, i, pArrayInfo->wInfoID);
//	bool GetRare(DWORD CharacterIdx, WORD ObtainItemIdx, POSTYPE Pos, ITEM_RARE_OPTION_INFO* pRareOptionInfo);

				//obtainItemIdx += 2;
				//DISTRIBUTERAND->ItemChangeAtLv(obtainItemIdx);


				ItemInsertToDB(pPlayer->GetID(), obtainItemIdx, 0, EmptyCellPos[i], MAKEDWORD(ArrayInfoUnitNum, pArrayInfo->wObtainArrayID), bSeal);
			}
		}
	}

	if(obtainItemNum > 0)
	{
		ASSERT(0);
		return 6;
	}

	return 0;
}

void CItemManager::ObtainItemDBResult(CPlayer * pPlayer, WORD ArrayID)
{
	ITEMOBTAINARRAYINFO * pArrayInfo = pPlayer->GetArray(ArrayID);

	MONEYTYPE ChangeMoney = 0;
	for( int i = 0 ; i < pArrayInfo->ItemArray.ItemNum ; ++i )
	{
		ITEMBASE * ItemInfo = pArrayInfo->ItemArray.GetItem( i );
		CItemSlot * pSlot = pPlayer->GetSlot(ItemInfo->Position);
		if(pSlot->InsertItemAbs(pPlayer, ItemInfo->Position, ItemInfo) != EI_TRUE)
		{
//			ASSERT(0);
			continue;
		}

		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( ItemInfo->wIconIdx );
		if(pArrayInfo->wType == eLog_ItemObtainBuy)
		{
			//ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( ItemInfo->wIconIdx );
			ChangeMoney = pInfo->BuyPrice;
		}


		/*
		if( pInfo->ItemKind == eQUEST_ITEM_PET )
		{
			g_pServerSystem->CreateNewPet(pPlayer, pInfo->ItemIdx, ItemInfo->dwDBIdx, pInfo->Plus_MugongIdx);
		}*/

		if( pInfo->ItemKind == eQUEST_ITEM_PET || pInfo->ItemKind == eSHOP_ITEM_PET )
		{
			PET_TOTALINFO* pPetInfo = pPlayer->GetPetManager()->GetPetTotalInfo(ItemInfo->dwDBIdx, eWithNULL );
			if( NULL == pPetInfo )
			{
				g_pServerSystem->CreateNewPet(pPlayer, pInfo->ItemIdx, ItemInfo->dwDBIdx, pInfo->Plus_MugongIdx);
			}
		}

		//SW070127 타이탄
		if( pInfo->ItemKind == eTITAN_ITEM_PAPER )
		{
			TITAN_TOTALINFO* pTitanInfo = pPlayer->GetTitanManager()->GetTitanTotalInfo(ItemInfo->dwDBIdx);
			if( !pTitanInfo )
			{
				//첫생성 & DB등록
				pPlayer->GetTitanManager()->CreateNewTitan(pPlayer, pInfo->ItemIdx, ItemInfo->dwDBIdx, pInfo->Plus_MugongIdx, pInfo->Plus_Value);
			}
		}

		if( pInfo->ItemKind & eTITAN_EQUIPITEM )
		{
			pPlayer->GetTitanManager()->MakeNewEnduranceInfo(pPlayer, ItemInfo);
		}

		LogItemMoney(pArrayInfo->dwFurnisherIdx, "", pPlayer->GetID(), pPlayer->GetObjectName(),
				pArrayInfo->wType, 0, pPlayer->GetMoney(eItemTable_Inventory), ChangeMoney,
				ItemInfo->wIconIdx, ItemInfo->dwDBIdx, 0, ItemInfo->Position, ItemInfo->Durability, pPlayer->GetPlayerExpPoint());


		// 06.09.12 RaMa - 치트로 얻은 아이템 로그 추가
		if( pArrayInfo->wType == eLog_ItemObtainCheat )
		{
			LogGMToolUse( pPlayer->GetID(), eGMLog_Item, eLog_ItemObtainCheat, 
				ItemInfo->wIconIdx, ItemInfo->dwDBIdx );
		}
	}



	//sendmsg
	pPlayer->SendMsg( &pArrayInfo->ItemArray, pArrayInfo->ItemArray.GetSize() );

	ITEMMGR->Free(pPlayer, pArrayInfo);
}

unsigned int CItemManager::GetTotalEmptySlotNum(CItemSlot* pSlot)
{
	//남아 있는 공간을 계산한다.

	POSTYPE startPos = pSlot->GetStartPos();
	POSTYPE EndPos = startPos +  pSlot->GetSlotNum();
	POSTYPE i = 0;

	unsigned int totalemptyNum = 0;
	for( i = startPos ; i < EndPos ; ++i )
	{
		if(pSlot->IsEmpty(i)) totalemptyNum++;
	}

	return totalemptyNum;
}

// rt : ¨￢o¡Æⓒª¡Æ¡I8¡¤I ¡ii ¨uo AO￠￥A ¡Æⓒø¨uo
WORD CItemManager::GetCanBuyNumInSpace(CPlayer * pPlayer, CItemSlot * pSlot, WORD whatItemIdx, DURTYPE whatItemNum, WORD * EmptyCellPos, WORD & EmptyCellNum)
{
	DURTYPE EstItemNum = whatItemNum;
	POSTYPE startPos = pSlot->GetStartPos();
	POSTYPE EndPos = startPos +  pSlot->GetSlotNum();
	POSTYPE i = 0;
	EmptyCellNum = 0;

	if(!IsDupItem(whatItemIdx))
	{
		for( i = startPos ; i < EndPos ; ++i )
		{
			if( EstItemNum == 0 ) break;
			if(pSlot->IsEmpty(i))
			{
				--EstItemNum;
				EmptyCellPos[EmptyCellNum++] = i;
			}
		}
	}
	else
	{
		for( i = startPos ; i < EndPos ; ++i )
		{
			if( EstItemNum == 0 ) break;
			const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(i);
			if( !pSlot->IsEmpty(i) && !pSlot->IsLock(i) &&
				pItemBase->wIconIdx == whatItemIdx &&
				pItemBase->Durability < MAX_YOUNGYAKITEM_DUPNUM )
			{
				if( pItemBase->Durability + EstItemNum > MAX_YOUNGYAKITEM_DUPNUM )
					EstItemNum = EstItemNum + pItemBase->Durability - MAX_YOUNGYAKITEM_DUPNUM;
				else
					EstItemNum = 0;
				EmptyCellPos[EmptyCellNum++] = i;
			}
		}

		for( i = startPos ; i < EndPos ; ++i )
		{
			if( EstItemNum == 0 ) break;
			if(pSlot->IsEmpty(i))
			{
				if( EstItemNum > MAX_YOUNGYAKITEM_DUPNUM )
					EstItemNum -= MAX_YOUNGYAKITEM_DUPNUM;
				else
					EstItemNum = 0;
				EmptyCellPos[EmptyCellNum++] = i;
			}
		}
	}

	return (WORD)(whatItemNum - EstItemNum);
}
// rt : ¨uOAoCN ￥i¡¤8¡¤I ¡ii ¨uo AO￠￥A ¡Æⓒø¨uo
WORD CItemManager::GetCanBuyNumInMoney(CPlayer * pPlayer, WORD buyNum, MONEYTYPE Price)
{
	MONEYTYPE money = pPlayer->GetMoney();

	if( money >= (MONEYTYPE)(buyNum * Price) )	//buynum￠?￠® ¨uⓒi½?i´ⓒo¡¾a¡Æ¨￡(￠￢A￠?iA¡i¡Æ¨￡)AI ￥ie¨ui¡Æ￠®¨u¡ⓒ...ⓒo￠cf¡Æ￠® ¡iy¡¾aAuAI AO￠￥U.
	{
		return buyNum;
	}
	else
	{
		return (WORD)(money / Price);
	}
}
int CItemManager::UseItem( CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx )
{
	if(pPlayer->GetState() == eObjectState_Die)
		return eItemUseErr_Err;
	if(!CHKRT->ItemOf(pPlayer, TargetPos, wItemIdx,0,0,CB_EXIST|CB_ICONIDX))
		return eItemUseErr_Err;
	
	if(pPlayer->GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal );

	CItemSlot * pSlot = pPlayer->GetSlot(TargetPos);
	const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(TargetPos);
	ITEMBASE Item = *pItemBase;
	ITEM_INFO * pItemInfo = GetItemInfo(wItemIdx);

	if(pPlayer->IsHide())
	if(IsUnfitItemInfoListForHide(wItemIdx))
	{
		return eItemUseErr_Err;
	}
	
	//if( g_pServerSystem->CheckMapKindIs(eSurvival) )
	if( g_pServerSystem->GetMap()->IsMapKind(eSurvivalMap) )
	{
		if( !(pItemInfo->ItemKind == eYOUNGYAK_ITEM)
			&& !(pItemInfo->ItemKind & eEQUIP_ITEM)
			&& !(pItemInfo->ItemKind & eSHOP_ITEM_MAKEUP)
			&& !(pItemInfo->ItemKind & eSHOP_ITEM_DECORATION)
			//&& !(pItemInfo->ItemKind & eSHOP_ITEM_SUNDRIES)
			&& !(pItemInfo->ItemKind & eSHOP_ITEM_EQUIP)
			)
			return eItemUseErr_Err;

		//SW061129 홍콩추가요청작업 - 사용갯수제한
		//if( 8000 <= pItemInfo->ItemIdx && pItemInfo->ItemIdx <= 8013 )
		if( pItemInfo->ItemKind == eYOUNGYAK_ITEM )
		{
			if( SVVMODEMGR->GetCurModeState() == eSVVMode_Fight )
			if( FALSE == SVVMODEMGR->AddItemUsingCount(pPlayer) )
				return eItemUseErr_Err;
		}
	}

	if(pItemInfo->ItemKind & eYOUNGYAK_ITEM)
	{
		//태국은.. 마을에서 비무중에 물약 못먹어!
#ifdef _TL_LOCAL_
		if( g_pServerSystem->GetMap()->IsVillage() )
		if( pPlayer->GetBattle()->GetBattleKind() ==eBATTLE_KIND_VIMUSTREET || PARTYWARMGR->IsMemberInPartyWar( pPlayer ) > 0 )
		{
			return eItemUseErr_Err;
		}
#endif

		if( g_pServerSystem->GetMapNum() == PKEVENTMAP )
			return eItemUseErr_Err;

		if(pItemBase->Durability == 0)
			return eItemUseErr_Err;

		////////////////////////////////////////////////////////
		// 06. 06. 2차 전직 - 이영준
		// 혜안의물약
		if(pItemInfo->ItemIdx == 10700)
		{
			if(pPlayer->GetSingleSpecialState(eSingleSpecialState_Detect))
				return eItemUseErr_Err;

            SPECIAL_STATE_INFO* pInfo = SKILLMGR->GetSpecialStateInfo(eSingleSpecialState_DetectItem);

			if(pInfo)
			{
				DWORD time = pInfo->DurationTime;

				pPlayer->StartSpecialState(eSpecialState_DetectItem, time, 0, 0, 0, 0, 0);
			}
			else
				return eItemUseErr_Err;
		}

		if(pItemInfo->ItemKind == eYOUNGYAK_ITEM_TITAN)
		{
			if( FALSE == pPlayer->InTitan() )
				return eItemUseErr_Err;

			int rt = pPlayer->GetTitanManager()->ApplyYoungYakItemToCurTitan(pItemInfo);
			if( rt == eItemUseSuccess )
			{
                if(EI_TRUE != DiscardItem(pPlayer, TargetPos, wItemIdx, 1))
				{
                    return eItemUseErr_Err;
				}
			}
			return rt;
		}

		if(EI_TRUE != DiscardItem(pPlayer, TargetPos, wItemIdx, 1))
		{
			return eItemUseErr_Err;
		}
		
		if(pItemInfo->ItemKind == eYOUNGYAK_ITEM_PET)
		{
			pPlayer->GetPetManager()->FeedUpPet(pItemInfo->LifeRecover);
			return eItemUseSuccess;
		}		

		BYTE bEffectKind = 0;	//비무시 다른캐릭터에게도 이펙트 보내주기 위해

		//2007. 6. 13. CBH - 세트아이탬 회복률 옵션에 따른 처리 추가 수정
		if(pItemInfo->LifeRecover>0)
		{
			WORD plusLife = pItemInfo->LifeRecover;

			if( (pPlayer->GetAvatarOption()->RecoverRate) || (pPlayer->GetSetItemStats()->fLifeRecoverRate) )
			{
				WORD wSetItemLifeRate = (WORD)(pPlayer->GetSetItemStats()->fLifeRecoverRate * 100);
				// 0.01f를 하지 않고 0.01을 한 이유는 0.01f로 float 연산을 하면 0.01이 아니라 0.0099999998로 되므로 정확한 값이 안나온다.
				// 0.01로 하면 0.10000000000000001 로 연산되어서 정상적인 값이 나온다.
				plusLife += (WORD)(plusLife*((pPlayer->GetAvatarOption()->RecoverRate + wSetItemLifeRate)*0.01));
			}

			//2007. 6. 13. CBH - 세트아이탬 체력회복 (고정)
			if(pPlayer->GetSetItemStats()->wLifeRecover > 0)
			{
				plusLife += pPlayer->GetSetItemStats()->wLifeRecover;
			}

			CCharacterCalcManager::StartUpdateLife( pPlayer, plusLife );

			bEffectKind = 1;//생명력 회복 이펙트
		}
 		if(pItemInfo->LifeRecoverRate>0)
		{
			WORD plusLife = (WORD)(pPlayer->GetMaxLife()*pItemInfo->LifeRecoverRate);
			
			if( (pPlayer->GetAvatarOption()->RecoverRate) || (pPlayer->GetSetItemStats()->fLifeRecoverRate) )
			{
				WORD wSetItemLifeRate = (WORD)(pPlayer->GetSetItemStats()->fLifeRecoverRate * 100);
				plusLife += (WORD)(plusLife*((pPlayer->GetAvatarOption()->RecoverRate + wSetItemLifeRate)*0.01));
			}

			//2007. 6. 13. CBH - 세트아이탬 체력회복 (고정)
			if(pPlayer->GetSetItemStats()->wLifeRecover > 0)
			{
				plusLife += pPlayer->GetSetItemStats()->wLifeRecover;
			}

			CCharacterCalcManager::StartUpdateLife( pPlayer, plusLife );

			bEffectKind = 1;//생명력 회복 이펙트
		}

		if(pItemInfo->NaeRyukRecover>0)		//ⓒø¡i¡¤A+(E￠￢¨￢ⓒo) ¡Æid¨uoA￠®	 
		{
			WORD plusNaeryuk = pItemInfo->NaeRyukRecover;

			if( (pPlayer->GetAvatarOption()->RecoverRate) || (pPlayer->GetSetItemStats()->fNaeRyukRecoverRate) )
			{
				WORD wSetItemNaeRyuk = (WORD)(pPlayer->GetSetItemStats()->fNaeRyukRecoverRate * 100);
				plusNaeryuk += (WORD)(plusNaeryuk*((pPlayer->GetAvatarOption()->RecoverRate+wSetItemNaeRyuk)*0.01));
			}

			//2007. 6. 13. CBH - 세트아이탬 내력회복 (고정)
			if(pPlayer->GetSetItemStats()->wNaeRyukRecover > 0)
			{
				plusNaeryuk += pPlayer->GetSetItemStats()->wNaeRyukRecover;
			}

			// 06.08.30. RaMa // 내력회복시 추가 호신강기 회복
			if( pPlayer->GetAvatarOption()->ShieldRecoverRate )
			{
				WORD addshield = (WORD)(pItemInfo->NaeRyukRecover*(pPlayer->GetAvatarOption()->ShieldRecoverRate*0.01));
				CCharacterCalcManager::StartUpdateShield( pPlayer, addshield, 5, 1500 );
			}			

			CCharacterCalcManager::StartUpdateNaeRyuk( pPlayer, plusNaeryuk );

			bEffectKind = 2;//내력 회복 이펙트
		}
		if(pItemInfo->NaeRyukRecoverRate>0)		//ⓒø¡i¡¤A+(E￠￢¨￢ⓒo)%
		{
			WORD plusNaeryuk = (WORD)(pPlayer->GetMaxNaeRyuk()*pItemInfo->NaeRyukRecoverRate);
			
			if( (pPlayer->GetAvatarOption()->RecoverRate) || (pPlayer->GetSetItemStats()->fNaeRyukRecoverRate) )
			{
				WORD wSetItemNaeRyuk = (WORD)(pPlayer->GetSetItemStats()->fNaeRyukRecoverRate * 100);
				plusNaeryuk += (WORD)(plusNaeryuk*((pPlayer->GetAvatarOption()->RecoverRate+wSetItemNaeRyuk)*0.01));
			}

			//2007. 6. 13. CBH - 세트아이탬 내력회복 (고정)
			if(pPlayer->GetSetItemStats()->wNaeRyukRecover > 0)
			{
				plusNaeryuk += pPlayer->GetSetItemStats()->wNaeRyukRecover;
			}

			// 06.08.30. RaMa // 내력회복시 추가 호신강기 회복
			if( pPlayer->GetAvatarOption()->ShieldRecoverRate )
			{
				WORD addshield = (WORD)((pPlayer->DoGetMaxNaeRyuk()*pItemInfo->NaeRyukRecoverRate)*(pPlayer->GetAvatarOption()->ShieldRecoverRate*0.01));
				CCharacterCalcManager::StartUpdateShield( pPlayer, addshield, 5, 1500 );
			}

			CCharacterCalcManager::StartUpdateNaeRyuk( pPlayer, plusNaeryuk );

			bEffectKind = 2;//내력 회복 이펙트

		}

		// 06.09.25 RaMa - 추석이벤트
		if( pItemInfo->ItemIdx == 53216 || pItemInfo->ItemIdx == 53222 )
		{
#ifdef _HK_LOCAL_
			CCharacterCalcManager::StartUpdateShield( pPlayer, 1000, 5, 1500 );
#else
			WORD addshied = (WORD)(pPlayer->GetMaxShield() - pPlayer->GetShield());
			CCharacterCalcManager::StartUpdateShield( pPlayer, addshied, 5, 1500 );
#endif
			bEffectKind = 3;		//호신강기 회복 이펙트
		}

		if(pItemInfo->NaeRyuk>0)
			pPlayer->SetMaxNaeRyuk(pPlayer->GetMaxNaeRyuk()+pItemInfo->NaeRyuk);
		
		if(pItemInfo->GenGol>0)		//¡¾U¡Æn+(￠?￥i¡¾￠￢)	
			pPlayer->AddGenGol(pItemInfo->GenGol);
		
		if(pItemInfo->MinChub>0)		//ⓒoIA￠￢+(￠?￥i¡¾￠￢)	
			pPlayer->AddMinChub(pItemInfo->MinChub);
		
		if(pItemInfo->CheRyuk>0)		//A¨u¡¤A+(￠?￥i¡¾￠￢)	
			pPlayer->AddCheRyuk(pItemInfo->CheRyuk);
		
		if(pItemInfo->SimMek>0)		//¨oE￠￢¨¡+(￠?￥i¡¾￠￢)	
			pPlayer->AddSimMek(pItemInfo->SimMek);
		if(pItemInfo->Life>0)			//¡iy￠￢i¡¤A+(￠?￥i¡¾￠￢)	
		{
			pPlayer->SetMaxLife(pPlayer->GetMaxLife()+pItemInfo->Life);
			ASSERTMSG(0, "AI¡ÆA f￠￥e¡¤I Aⓒø￠￢￠c ¨uE￥iCAoAI~ ~( -0-)~");
		}
		//for event - hs//
		if( pItemInfo->ItemIdx == 53102 || pItemInfo->ItemIdx == 53232 || pItemInfo->ItemIdx == 53234 )
		{
			WORD wPlus = 1000;
#ifdef _HK_LOCAL_
			if( pItemInfo->ItemIdx == 53232 )
				wPlus = 400;
			else if( pItemInfo->ItemIdx == 53234 )
				wPlus = 400;
#else
			if( pItemInfo->ItemIdx == 53232 )
				wPlus = 2000;
			else if( pItemInfo->ItemIdx == 53234 )
				wPlus = 400;
#endif

			if( pPlayer->GetAvatarOption()->RecoverRate )
				wPlus += (WORD)(wPlus*(pPlayer->GetAvatarOption()->RecoverRate*0.1f));

			pPlayer->SetLife(pPlayer->GetLife()+wPlus);
			pPlayer->SetShield(pPlayer->GetShield()+wPlus);
			pPlayer->SetNaeRyuk(pPlayer->GetNaeRyuk()+wPlus);

			bEffectKind = 4;//올 회복 이펙트
		}
		else if(pItemInfo->ItemIdx == EVENTITEM_SHIELD_RECOVER || pItemInfo->ItemIdx == 53094 || pItemInfo->ItemIdx == 53109 )
		{
			pPlayer->SetShield(pPlayer->GetMaxShield());
			bEffectKind = 3;//호신강기 회복 이펙트
		}
		else if(pItemInfo->ItemIdx == EVENTITEM_ALL_RECOVER || pItemInfo->ItemIdx == 53095
			|| pItemInfo->ItemIdx == 53103 || pItemInfo->ItemIdx == 53217 || pItemInfo->ItemIdx == 53223 )
		{
			pPlayer->SetLife(pPlayer->GetMaxLife());
			pPlayer->SetShield(pPlayer->GetMaxShield());
			pPlayer->SetNaeRyuk(pPlayer->GetMaxNaeRyuk());
			bEffectKind = 4;//올 회복 이펙트
		}
		else if(pItemInfo->ItemIdx == 53074 )
		{
			MSG_DWORD2 msgEvent;
			msgEvent.Category = MP_ITEM;
			msgEvent.Protocol = MP_ITEM_EVENTITEM_USE;
			msgEvent.dwData1  = pPlayer->GetID();
			msgEvent.dwData2  = (DWORD)pItemInfo->ItemIdx;

			PACKEDDATA_OBJ->QuickSend( pPlayer, &msgEvent, sizeof(msgEvent) );			
		}
        //for event//
		// 2005 크리스마스 이벤트 코드
		else if(pItemInfo->ItemIdx == EVENT_ITEM_FIRECRACKER )
		{
			MSG_DWORD2 msgEvent;
			msgEvent.Category = MP_ITEM;
			msgEvent.Protocol = MP_ITEM_EVENTITEM_USE;
			msgEvent.dwData1  = pPlayer->GetID();
			msgEvent.dwData2  = (DWORD)pItemInfo->ItemIdx;

			PACKEDDATA_OBJ->QuickSend( pPlayer, &msgEvent, sizeof(msgEvent) );			
		}

		// 06. 03. 화이트데이 이벤트
		else if(pItemInfo->ItemIdx == 53151 )
		{
			MSG_DWORD2 msgEvent;
			msgEvent.Category = MP_ITEM;
			msgEvent.Protocol = MP_ITEM_EVENTITEM_USE;
			msgEvent.dwData1  = pPlayer->GetID();
			msgEvent.dwData2  = (DWORD)pItemInfo->ItemIdx;

			PACKEDDATA_OBJ->QuickSend( pPlayer, &msgEvent, sizeof(msgEvent) );			
		}

		//비무시에 다른 캐릭터에 메세지 보내기
		if( bEffectKind != 0 )
		if( pPlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_VIMUSTREET )	//비무중
		{
/*//상대에게만 보내기
			CPlayer* pOtherPlayer = g_pUserTable->FindUser( pPlayer->GetVimuOpPlayerID() );
			if( pOtherPlayer )
			{
				MSG_DWORDBYTE msg;
				msg.Category	= MP_ITEM;
				msg.Protocol	= MP_ITEM_USE_NOTIFY;
				msg.dwData		= pPlayer->GetID();
				msg.bData		= bEffectKind;

				pOtherPlayer->SendMsg( &msg, sizeof(msg) );
			}
*/
//그리드 전체에 보내기
			MSG_DWORDBYTE msg;
			msg.Category	= MP_ITEM;
			msg.Protocol	= MP_ITEM_USE_NOTIFY;
			msg.dwData		= pPlayer->GetID();
			msg.bData		= bEffectKind;

			PACKEDDATA_OBJ->QuickSend( pPlayer, &msg, sizeof(msg) );
		}
	}
	// ⓒo¡i¡ÆⓒªA¡I
	else if(pItemInfo->ItemKind & eMUGONG_ITEM)
	{
		//LEVEL A¨uA¨I
		if(!CHKRT->LevelOf(pPlayer, pItemInfo->LimitLevel))
			return eItemUseErr_Err;
		if(!CHKRT->ItemStageOf(pPlayer, (BYTE)pItemInfo->LimitJob))
			return eItemUseErr_Err;
#ifdef _JAPAN_LOCAL_
		if( !CHKRT->ItemAttrOf(pPlayer, pItemInfo->wItemAttr) )
			return eItemUseErr_Err;
		if( pItemInfo->wAcquireSkillIdx1 )
		{
			if( !CHKRT->ItemAquireSkillOf( pPlayer, pItemInfo->wAcquireSkillIdx1, pItemInfo->wAcquireSkillIdx2 ) )
				return eItemUseErr_Err;
		}
#endif
		// ⓒo¡i¡Æⓒª￥ii¡¤I￠?¨I¨￢I
		if(CHKRT->MugongOf(pPlayer, pItemInfo->ItemIdx))
			return eItemUseErr_PreInsert;
		
		if(pItemInfo->ItemKind == eMUGONG_ITEM_KYUNGGONG)
		{
			if(KYUNGGONGMGR->IsSetableKyungGong(pPlayer,pItemInfo) == FALSE)
				return eItemUseErr_Err;
			ABILITYMGR->UpdateAbilityState(ABILITYUPDATE_ABILITYUPITEM_USE,pItemInfo->ItemIdx,pPlayer->GetAbilityGroup());
		}

		// magi82 - Titan(070914) 타이탄 무공업데이트
		if( pItemInfo->ItemKind == eMUGONG_ITEM_TITAN )
		{
			if( pPlayer->InTitan() == FALSE )
                return eItemUseErr_Err;
		}

#ifdef _JAPAN_LOCAL_
		if( pItemInfo->wDeleteSkillIdx )
		{
			if( !MUGONGMNGR->DeleteSkill( pPlayer, pItemInfo->wDeleteSkillIdx ) )
				return eItemUseErr_Err;
		}
#endif
		if(!MUGONGMNGR->AddMugong(pPlayer, pItemInfo->MugongNum,pItemInfo->ItemKind))
			return eItemUseErr_Err;

		if(EI_TRUE != DiscardItem(pPlayer, TargetPos, wItemIdx, 1))
		{
			return eItemUseErr_Err;
		}
		
		// Log
		LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
			eLog_ItemUse, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
			Item.wIconIdx, Item.dwDBIdx,  Item.Position, 0,
			Item.Durability, pPlayer->GetPlayerExpPoint());	
	}
	else if(pItemInfo->ItemKind & eEXTRA_ITEM)
	{
		if( pItemInfo->ItemIdx == EVENT_ITEM_SNOW || pItemInfo->ItemIdx == EVENT_ITEM_HARDSNOW )
		{
			pPlayer->SetEventSnowIdx(pItemInfo->ItemIdx);	//싱크 보장이 안된다보고 초기화 안하겠다.
		}
		// ￥ii¡¾¨­¨u¡A ¨￢n¡¾¨­¨u¡ⓒ ¡ic￠?e
		if(EI_TRUE != DiscardItem(pPlayer, TargetPos, wItemIdx, 1))
		{
			return eItemUseErr_Err;
		}
		// Log
		LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
			eLog_ItemUse, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
			Item.wIconIdx, Item.dwDBIdx, Item.Position, 0,
			Item.Durability, pPlayer->GetPlayerExpPoint());
	}
	else if( pItemInfo->ItemKind & eQUEST_ITEM )
	{
		if( pItemInfo->ItemKind == eQUEST_ITEM_PET )
		{
			if(pItemInfo->LimitLevel > pPlayer->GetMaxLevel())
			{
				return eItemUseErr_Err;
			}
			//펫 처음 생성이면 펫 생성하고 -> 퀘스트펫아이템 획득시 펫도 생성으로 변경.
			pPlayer->GetPetManager()->SummonPet(pItemBase->dwDBIdx);
			//pPlayer->GetPetManager()->SummonPet(PET_ID_START);
			return eItemUseSuccess;
		}

		if(EI_TRUE != DiscardItem(pPlayer, TargetPos, wItemIdx, 1))
		{
			return eItemUseErr_Err;
		}
		
		if( pItemInfo->ItemKind == eQUEST_ITEM_START )
		{
			CQuestEvent qe;
			qe.SetValue( eQuestEvent_UseItem, pItemInfo->ItemIdx, pItemInfo->ItemType );
			QUESTMGR->AddQuestEvent( pPlayer, &qe );
		}

		// Log
		LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
			eLog_ItemUse, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
			Item.wIconIdx, Item.dwDBIdx, Item.Position, 0,
			Item.Durability, pPlayer->GetPlayerExpPoint());
	}
	else if( pItemInfo->ItemKind & eTITAN_ITEM )
	{
		if( pItemInfo->ItemKind == eTITAN_ITEM_PAPER )
		{
			if(pPlayer->GetState() != eObjectState_None)
				return eItemUseErr_Err;

			//2007. 10. 19. CBH - 무쌍 모드일때 타이탄 탑승 실패
			CHARACTER_TOTALINFO CTInfo;
			pPlayer->GetCharacterTotalInfo( &CTInfo );
			if(CTInfo.bMussangMode == TRUE)
			{
				return eItemUseErr_Err;
			}

			CTitanManager* pTitanManager = pPlayer->GetTitanManager();
			//pPlayer->GetTitanManager()->RideInTitan(pItemBase->dwDBIdx);
			if(pTitanManager->TitanRecallStartSyn(pItemBase->dwDBIdx) == TRUE)
			{
				pTitanManager->InitTitanRecall();

				DWORD dwRegistTitanItemDBIdx = pPlayer->GetTitanManager()->GetRegistTitanCallItemDBIdx();				
				if( dwRegistTitanItemDBIdx <= 0 )
					return eItemUseErr_Err;
				
				TITAN_TOTALINFO* pTitanInfo = pPlayer->GetTitanManager()->GetTitanTotalInfo(dwRegistTitanItemDBIdx);
				if( pTitanInfo == NULL )
					return eItemUseErr_Err;

				TITANINFO_GRADE* pGrade = GAMERESRCMNGR->GetTitanGradeInfo(pTitanInfo->TitanKind, pTitanInfo->TitanGrade);
				if( pGrade == NULL )
					return eItemUseErr_Err;

				DWORD dwRecallProcessTime = (pGrade->dwRecallSpellTime - pTitanManager->GetTitanShopitemOption()->dwSummonReduceTime) + gCurTime;	// magi82(26)
				pTitanManager->SetRecallProcessTime(dwRecallProcessTime);
				pTitanManager->StartTitanRecall();
			}
			else
			{
				return eItemUseErr_Err;
			}

			return eItemUseSuccess;
		}
	}
	else
	{
		ASSERT(0);
		return eItemUseErr_Err;
	}

	return eItemUseSuccess;
}



/*
(1)	ⓒo¡i¡¾a / ⓒi¹|i±¨ui¡¾￠￢ / ¨u¡¿¨u¨u¨u¡ⓒ￠￢￠c : [fv ¨u¨￢¡Æⓒª E￠c¡¤u] + [¨¡?¡¾a￥ii¡¾¨­]
(2)	¡¾¡¿￠?U : [fv ¨u¨￢¡Æⓒª E￠c¡¤u] + [¨¡?¡¾a￥ii¡¾¨­] * 2
(3)	¨¡Cd
¡Æ￠®)	¨u¨￢¡Æⓒª : [¨u¨￢¡Æⓒª E￠c¡¤u] < random(0~100)
ⓒø¨￡)	¨oC¨¡¨￠ : [¨u¨￢¡Æⓒª E￠c¡¤u] > random(0~100) AI¡Æi ¨u¨￢¡ÆⓒªE￠c¡¤u - random(0~100) < 30
￠￥U)	￠￥e¨oC¨¡¨￠ : [¨u¨￢¡Æⓒª E￠c¡¤u] > random(0~100) AI¡Æi ¨u¨￢¡ÆⓒªE￠c¡¤u - random(0~100) > 30
// Ac¡¤a ¡ief : ¨u¨￢¡Æⓒª, ¨oC¨¡¨￠, ￠￥e¨oC¨¡¨￠
// ¡¾a¨￢¡i¨u¨¡AIAU ¡ief : ¨u¨￢¡Æⓒª, ￠￥e¨oC¨¡¨￠
*/
BOOL CItemManager::EnoughMixMaterial(WORD needItemIdx, WORD needItemNum, MATERIAL_ARRAY * pMaterialArray, WORD wMaterialNum)
{
	DURTYPE deNeedItemNum = needItemNum;
	for(int i = 0 ; i < wMaterialNum ; ++i)
	{
		if(deNeedItemNum == 0) break;

		if(pMaterialArray[i].wItemIdx == needItemIdx)
		{
			if(pMaterialArray[i].Dur < deNeedItemNum)
			{
				//hs vCOAI ¨uE￥iC¨u¡ⓒ ¨uod
				if(!IsDupItem(pMaterialArray[i].wItemIdx))
					deNeedItemNum = deNeedItemNum -1;
				else 
					deNeedItemNum -= pMaterialArray[i].Dur;
			}
			else
			{
				//pMaterialArray[i].Dur -= deNeedItemNum;
				deNeedItemNum = 0;
			}
		}
	}

	if(deNeedItemNum == 0)
		return TRUE;
	return FALSE;
}

BOOL CItemManager::EnoughDissolveSpace(CPlayer * pPlayer, WORD wOptionIdx, LEVELTYPE ItemLevel, WORD wAbilityGrade, MATERIAL_ARRAY * pJewelArray, WORD & wJewelNum)
{/*
	ITEM_OPTION_INFO * pInfo = pPlayer->GetItemOption(wOptionIdx);
	if(!pInfo) return FALSE;

	wJewelNum = 0;

	float var = 0;
	DWORD num = 0;
	BOOL bConvert = FALSE;
	ITEM_REINFORCE_INFO * pRInfo = NULL;
	m_ReinforceItemInfoList.SetPositionHead();
	while(pRInfo = m_ReinforceItemInfoList.GetData())
	{
		switch(pRInfo->ReinforceType)
		{
		case eRJP_GENGOL:
			{
				if(pInfo->GenGol > 0)
				{ 
					var = pInfo->GenGol;
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_MINCHUB:
			{
				if(pInfo->MinChub > 0)
				{ 
					var = pInfo->MinChub;
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_CHERYUK:
			{
				if(pInfo->CheRyuk > 0)
				{ 
					var = pInfo->CheRyuk;
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_SIMMEK:
			{
				if(pInfo->SimMek > 0)
				{ 
					var = pInfo->SimMek;
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_LIFE:
			{
				if(pInfo->Life > 0)
				{ 
					var = pInfo->Life;
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_NAERYUK:
			{
				if(pInfo->NaeRyuk > 0)
				{ 
					var = pInfo->NaeRyuk;
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_SHIELD:
			{
				if(pInfo->Shield > 0)
				{ 
					var = pInfo->Shield;
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_FIREREGIST:
			{
				if(pInfo->AttrRegist.GetElement_Val(ATTR_FIRE) > 0.009)
				{ 
					var = pInfo->AttrRegist.GetElement_Val(ATTR_FIRE);
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_WATERREGIST:
			{
				if(pInfo->AttrRegist.GetElement_Val(ATTR_WATER) > 0.009)
				{ 
					var = pInfo->AttrRegist.GetElement_Val(ATTR_WATER);
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_TREEREGIST:
			{								
				if(pInfo->AttrRegist.GetElement_Val(ATTR_TREE) > 0.009) 
				{ 
					var = pInfo->AttrRegist.GetElement_Val(ATTR_TREE);
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_GOLDREGIST:
			{
				if(pInfo->AttrRegist.GetElement_Val(ATTR_IRON) > 0.009) 
				{ 
					var = pInfo->AttrRegist.GetElement_Val(ATTR_IRON);
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_EARTHREGIST:
			{
				if(pInfo->AttrRegist.GetElement_Val(ATTR_EARTH) > 0.009) 
				{ 
					var = pInfo->AttrRegist.GetElement_Val(ATTR_EARTH);
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_PHYATTACK:
			{
				if(pInfo->PhyAttack > 0) 
				{ 
					var = pInfo->PhyAttack;
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_CRITICAL:
			{
				if(pInfo->CriticalPercent > 0) 
				{ 
					var = pInfo->CriticalPercent;
					bConvert = TRUE;
				}
			}
			break;		
		case eRJP_FIREATTACK:
			{
				if(pInfo->AttrAttack.GetElement_Val(ATTR_FIRE) > 0.009) 
				{ 
					var = pInfo->AttrAttack.GetElement_Val(ATTR_FIRE);
					bConvert = TRUE;
				}
			}
			break;	
		case eRJP_WATERATTACK:
			{
				if(pInfo->AttrAttack.GetElement_Val(ATTR_WATER) > 0.009) 
				{ 
					var = pInfo->AttrAttack.GetElement_Val(ATTR_WATER);
					bConvert = TRUE;
				}
			}
			break;	
		case eRJP_TREEATTACK:
			{
				if(pInfo->AttrAttack.GetElement_Val(ATTR_TREE) > 0.009) 
				{ 
					var = pInfo->AttrAttack.GetElement_Val(ATTR_TREE);
					bConvert = TRUE;
				}
			}
			break;	
		case eRJP_GOLDATTACK:
			{
				if(pInfo->AttrAttack.GetElement_Val(ATTR_IRON) > 0.009) 
				{ 
					var = pInfo->AttrAttack.GetElement_Val(ATTR_IRON);
					bConvert = TRUE;
				}
			}
			break;	
		case eRJP_EARTHATTACK:
			{
				if(pInfo->AttrAttack.GetElement_Val(ATTR_EARTH) > 0.009) 
				{ 
					var = pInfo->AttrAttack.GetElement_Val(ATTR_EARTH);
					bConvert = TRUE;
				}
			}
			break;	
		case eRJP_PHYDEFENSE:
			{
				if(pInfo->PhyDefense > 0)
				{
					var = pInfo->PhyDefense;
					bConvert = TRUE;
				}
			}
			break;	
		default:
			{
				return FALSE;
			}	
		}
		if(bConvert)
		{
			WORD plusNum = var*100;
			WORD seed = (pRInfo->w*(random(0, 100) - ItemLevel/2+wAbilityGrade));
			num = plusNum/seed;


			while(num)
			{
				if(ITEMMGR->IsDupItem(pRInfo->wItemIdx))
				{
					if(num > MAX_YOUNGYAKITEM_DUPNUM)
					{
						num -= MAX_YOUNGYAKITEM_DUPNUM;
						pJewelArray[wJewelNum].wItemIdx = pRInfo->wItemIdx;
						pJewelArray[wJewelNum].Dur = MAX_YOUNGYAKITEM_DUPNUM;
						pJewelArray[wJewelNum].ItemPos = 0;
					}
					else
					{
						num = 0;
						pJewelArray[wJewelNum].wItemIdx = pRInfo->wItemIdx;
						pJewelArray[wJewelNum].Dur = num;
						pJewelArray[wJewelNum].ItemPos = 0;
					}
				}
				else
				{
					--num;
					pJewelArray[wJewelNum].wItemIdx = pRInfo->wItemIdx;
					pJewelArray[wJewelNum].Dur = 0;
					pJewelArray[wJewelNum].ItemPos = 0;
				}

				++wJewelNum;
			}
			bConvert = FALSE;	
		}
	}
		*/
	ASSERT(wJewelNum<=MAX_DISSOLVEGRID_NUM);
	if(!EnoughDissolveInvSpace(pPlayer, wJewelNum))
		return FALSE;
	else
		return TRUE;


	
}

BOOL CItemManager::EnoughDissolveInvSpace(CPlayer * pPlayer, WORD wJewelNum)
{
	for(int i = TP_INVENTORY_START ; i < TP_INVENTORY_END && wJewelNum != 0 ; ++i)
	{
		CItemSlot * pSlot = pPlayer->GetSlot(i);
		BOOL bIsEmpty = pSlot->IsEmpty(i);
		if(bIsEmpty)
		{
			--wJewelNum;
		}
	}

	if(wJewelNum == 0)
		return TRUE;
	else
		return FALSE;
}

int CItemManager::MixItem(CPlayer* pPlayer, WORD wBasicItemIdx, POSTYPE BasicItemPos, POSTYPE ResultIdx, MATERIAL_ARRAY * pMaterialArray, WORD wMaterialNum, WORD ShopItemIdx, WORD ShopItemPos, BOOL bTitan)	// magi82 - Titan(070118)
{
	// magi82 - Titan(070403) 타이탄을 조합할때는 조합스킬을 배웠는지 체크하지 않는다.
	if(!bTitan)
	{
		if(!CHKRT->AbilityOf(pPlayer, eAUKJOB_Mix))
			return 1;
	}

	if(!CHKRT->ItemOf(pPlayer, BasicItemPos, wBasicItemIdx, 0,0, CB_EXIST|CB_ICONIDX))
		return 2;

	// magi82 - UniqueItem(070627)
	// 유니크아이템이고 조합이 불가능한 유니크라면..
	eITEM_KINDBIT eBit = GetItemKind(wBasicItemIdx);
	if(eBit == eEQUIP_ITEM_UNIQUE && GAMERESRCMNGR->GetUniqueItemOptionList(wBasicItemIdx)->MixFlag == 0)
	{
		return 2;
	}

	// ShopItem이 있는가
	ITEM_INFO* pShopItem = NULL;
	if( ShopItemIdx )
	{
		if( !CHKRT->ItemOf(pPlayer, ShopItemPos, ShopItemIdx, 0, 0, CB_EXIST|CB_ICONIDX) )
			return 20;
		pShopItem = GetItemInfo( ShopItemIdx );
		if( !pShopItem )		return 2;
	}

	for(int i = 0 ; i < wMaterialNum ; ++i)
	{
		//본재료가 있는 곳인지 체크 해야한다.
		if( BasicItemPos == pMaterialArray[i].ItemPos )
			return 3;
		if(!CHKRT->ItemOf(pPlayer, pMaterialArray[i].ItemPos, pMaterialArray[i].wItemIdx, pMaterialArray[i].Dur,0, CB_EXIST|CB_ICONIDX|CB_ENOUGHDURA))
			return 3;
	}

	// magi82 - Titan(070118) ////////////////////////////////////////////////
	ITEM_MIX_INFO * pMixInfo = NULL;
	if(bTitan)	// 타이탄 파츠 아이템 조합
        pMixInfo = TITANITEMMGR->GetTitanMixItemInfo(wBasicItemIdx);
	else	// 일반 아이템 조합
		pMixInfo = GetMixItemInfo(wBasicItemIdx);
	//////////////////////////////////////////////////////////////////////////

	if(!pMixInfo)
		return 4;

	// for china strange item
	if( pMixInfo->wResultItemNum <= ResultIdx )	return 4;

	const ITEMBASE * pBasicItemBase = GetItemInfoAbsIn(pPlayer, BasicItemPos);

	if( GetItemKind(pBasicItemBase->wIconIdx) != eYOUNGYAK_ITEM &&
		GetItemKind(pBasicItemBase->wIconIdx) != eEXTRA_ITEM_JEWEL &&
		pBasicItemBase->Durability > 1 )
	{
		return 7;		
	}

	MONEYTYPE money;
	if(IsDupItem(wBasicItemIdx))
		money = pMixInfo->psResultItemInfo[ResultIdx].Money * pBasicItemBase->Durability;
	else
		money = pMixInfo->psResultItemInfo[ResultIdx].Money;

	if( pPlayer->GetMoney() < money )
		return 6;

	// ¨u¡ⓒⓒoi¨￢¨ui¡¾¨u¡ⓒ Ac¡¤a ¨uo reA¨uA¨I
	WORD needItemIdx=0;
	WORD needItemNum=0;

	for(i = 0 ; i < pMixInfo->psResultItemInfo[ResultIdx].wMaterialItemNum ; ++i)
	{
		needItemIdx = pMixInfo->psResultItemInfo[ResultIdx].psMaterialItemInfo[i].wMatItemIdx;
		needItemNum = pMixInfo->psResultItemInfo[ResultIdx].psMaterialItemInfo[i].wMatItemNum;
		if(IsDupItem(wBasicItemIdx))
			needItemNum *= (WORD)pBasicItemBase->Durability;
		if(!EnoughMixMaterial(needItemIdx, needItemNum, pMaterialArray, wMaterialNum))
			return 5;
	}

	// 만들려는 아이템이 레벨이 맞는 아이템인지 체크 
	ITEM_INFO* pNewItemInfo = GetItemInfo( pMixInfo->psResultItemInfo[ResultIdx].wResItemIdx );
	if( ShopItemIdx )
	{
		if( !pNewItemInfo )			return 2;
		if( pNewItemInfo->LimitLevel < pShopItem->GenGol || pNewItemInfo->LimitLevel > pShopItem->MinChub )
			return 23;
	}

//	eITEM_KINDBIT bits = GetItemKind(pMixInfo->wItemIdx);
	DWORD ratio = 0;
	WORD mixGrade = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_ITEMMIX, pPlayer->GetAbilityGroup());
	WORD per = MAX_MIX_PERCENT/100;

	// +10템 조합 기능 추가 작업 by Stiner(2008/06/13)-10+MixItem
	// pMixInfo의 정보에서 결과템이 +10일경우 조합률 계산을 다른 식으로 해 준다.
	ITEM_INFO* pResItemInfo = GetItemInfo(pMixInfo->psResultItemInfo[ResultIdx].wResItemIdx);
	if( pResItemInfo->ItemGrade >= 10 )
	{
		// TODO: 확률 계산
		HILEVEL_ITEM_MIX_RATE_INFO* pRateInfo = m_HiLevelItemMixRateInfoList.GetData( pResItemInfo->ItemGrade );

		ratio = pRateInfo->dwBaseRate + ( mixGrade * 10000 / pRateInfo->dwModulate );
	}
	else
		ratio = (pMixInfo->psResultItemInfo[ResultIdx].SuccessRatio + mixGrade*per);

#ifdef _HK_LOCAL_
	//만약 +1 장착아이템의 경우엔 ratio가 변경된다.
	if( pNewItemInfo )
	if( pNewItemInfo->ItemGrade > 0 && pNewItemInfo->ItemGrade <= 9 )
	if( pNewItemInfo->LimitLevel > 0 && pNewItemInfo->LimitLevel <= 99 )
	{
		if( m_nItemMixBal[pNewItemInfo->LimitLevel-1][pNewItemInfo->ItemGrade-1] )
		{
			ratio = pMixInfo->psResultItemInfo[ResultIdx].SuccessRatio
			+ mixGrade * per * m_nItemMixBal[pNewItemInfo->LimitLevel-1][pNewItemInfo->ItemGrade-1] / 3000;
		}
	}
	//---
#endif

#ifdef _JAPAN_LOCAL_
	ITEM_INFO* pItem = GetItemInfo( wBasicItemIdx );
	if( pItem )
	{
		// 아이템과 플레이어 레벨 검사
		if( pItem->LimitLevel > pPlayer->GetMaxLevel() )	// 높을때
		{
			ratio = ratio / 100 * ( 100 - ( pItem->LimitLevel - pPlayer->GetMaxLevel() ) );
		}
		// 아이템 레벨이 60이상이냐 검사
		if( pItem->LimitLevel > 60 )
		{
			ratio = ratio / 100 * ( 100 - ( pItem->LimitLevel - 60 ) * 2 );
		}
	}
	else
		ratio = 0;
#endif	// _JAPAN_LOCAL_


	//
	DWORD Key = MakeNewKey();
	
	ITEMBASE ItemBase;
	ITEMBASE Item;
	memset( &Item, 0, sizeof(ITEMBASE) );
	char NameKey[MAX_NAME_LENGTH+1];
	sprintf(NameKey, "%d", Key);
	CItemSlot * pSlot = pPlayer->GetSlot(0); //인벤일테니

	// 성공, 실패를 먼저 가린다.
	BOOL bUseMixUp = FALSE;
	BOOL bSuccess = FALSE;
	BOOL bProtection = FALSE;
	DWORD Seed;
	// +10템 조합 기능 추가 작업 by Stiner(2008/06/13)-10+MixItem
	// 결과템이 +10일경우 100만 단위로 Seed값 산출
	if( pResItemInfo->ItemGrade >= 10 )
		Seed = LargeRandom( 0, MAX_MIX_LARGE_PERCENT );
	else
		Seed = random(0, MAX_MIX_PERCENT);

	// RaMa - 04.12.28   -> ShopItem사용시 조합확률을 10%올려준다.
	if( pPlayer->GetShopItemStats()->ItemMixSuccess )
	{
		ratio += (WORD)(MAX_MIX_PERCENT*0.1f);

		// 성공확률을 0으로 바꿔주고 클라이언트에 사용했음을 보내야 한다.
		pPlayer->GetShopItemManager()->CalcShopItemOption( eIncantation_MixUp, FALSE );
		pPlayer->GetShopItemManager()->SendMsgDwordToPlayer( MP_ITEM_SHOPITEM_MIXUP, eIncantation_MixUp );
		pPlayer->GetShopItemManager()->DeleteUsingShopItem( eIncantation_MixUp );

		bUseMixUp = TRUE;
	}

	if(Seed < ratio)		// 성공
		bSuccess = TRUE;

	//흔벎莉북膠틔돨된섬>=10돨珂빅,퓻齡냥묘100%
	if ( pResItemInfo->ItemGrade >= 10)
	{
		bSuccess = TRUE;
	}

	if( !bSuccess && ShopItemIdx )
	{
		bProtection = TRUE;		
	}
	else
	{
		//1. 재료 아이템 삭제
		for(i = 0 ; i < wMaterialNum ; ++i)
		{
			Item = *pSlot->GetItemInfoAbs(pMaterialArray[i].ItemPos);
			if(EI_TRUE != DiscardItem(pPlayer, pMaterialArray[i].ItemPos, pMaterialArray[i].wItemIdx, pMaterialArray[i].Dur))
				return 10;

			LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemDestroyMix, 
				Item.Durability, Item.Durability - pMaterialArray[i].Dur, pMaterialArray[i].Dur, Item.wIconIdx, Item.dwDBIdx, 
				Item.Position, 0, Item.Durability, pPlayer->GetPlayerExpPoint());
		}
		//2.돈 깎음
		pPlayer->SetMoney(money, MONEY_SUBTRACTION);
	}
//

	// ShopItem 삭제
	if( ShopItemIdx )
	{
		const ITEMBASE* pShop = GetItemInfoAbsIn( pPlayer, ShopItemPos );
		if( pShopItem )
		{
			if( EI_TRUE != DiscardItem( pPlayer, ShopItemPos, ShopItemIdx, 1 ) )
				ASSERT(0);
			
			SEND_SHOPITEM_BASEINFO msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_USE_ACK;		
			msg.ShopItemPos = ShopItemPos;
			msg.ShopItemIdx = ShopItemIdx;
			SendAckMsg(pPlayer, &msg, sizeof(msg));
			
			LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ShopItemUse, 
				pPlayer->GetMoney(), pPlayer->GetMoney(), 0, ShopItemIdx, pShop->dwDBIdx, ShopItemPos, 0, 
				Item.Position, pPlayer->GetPlayerExpPoint());
		}
	}
		
	if( bSuccess )
	{//성공

		//1. 기본->결과아이템으로 변경
		ITEM_INFO* pinfo = GetItemInfo( pMixInfo->psResultItemInfo[ResultIdx].wResItemIdx );
		if( !pinfo )
			return 2;

		CItemSlot * pSlot = pPlayer->GetSlot(BasicItemPos);
		if( pinfo->ItemKind & eSHOP_ITEM )
		{
			if( EI_TRUE != DiscardItem( pPlayer, BasicItemPos, wBasicItemIdx, 1 ) )	
				return 2;

			ObtainItemFromChangeItem( pPlayer, pMixInfo->psResultItemInfo[ResultIdx].wResItemIdx, 1 );
	
			LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemMixSuccess, 
				pPlayer->GetMoney(), ResultIdx, money, pinfo->ItemIdx, 0, 0, 0, 1,
				pPlayer->GetPlayerExpPoint());

		}
		else
		{
			WORD flag = UB_ICONIDX;
			if( !IsDupItem( pMixInfo->psResultItemInfo[ResultIdx].wResItemIdx ) )
				flag |= UB_DURA;

			if(EI_TRUE != pSlot->UpdateItemAbs(pPlayer,BasicItemPos, 0, pMixInfo->psResultItemInfo[ResultIdx].wResItemIdx,0,0,0, flag, SS_LOCKOMIT))
				return 6;

			const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(BasicItemPos);

			//!!!SW051017 우선 막자
			if(pItemBase->RareIdx < 0)
			{
				char temp[128];
				sprintf(temp, "RareIdxError!(<0) ItemDBIdx : %d, PlayerID : %d", pItemBase->dwDBIdx, pPlayer->GetID() );
				ASSERTMSG(0, temp);
			}
			if( pItemBase->RareIdx < 0 )
			{
				ItemUpdateToDB(pPlayer->GetID(), pItemBase->dwDBIdx, pItemBase->wIconIdx,
				pItemBase->Durability, pItemBase->Position, pItemBase->QuickPosition, 0 );

			LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemMixSuccess, 
				pPlayer->GetMoney(), ResultIdx, money, pItemBase->wIconIdx, pItemBase->dwDBIdx, pItemBase->Position, pItemBase->Position, pItemBase->Durability,
				pPlayer->GetPlayerExpPoint());			
			}
			else
			{
				ItemUpdateToDB(pPlayer->GetID(), pItemBase->dwDBIdx, pItemBase->wIconIdx,
				pItemBase->Durability, pItemBase->Position, pItemBase->QuickPosition, pItemBase->RareIdx);

			LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemMixSuccess, 
				pPlayer->GetMoney(), ResultIdx, money, pItemBase->wIconIdx, pItemBase->dwDBIdx, pItemBase->Position, pItemBase->Position, pItemBase->Durability,
				pPlayer->GetPlayerExpPoint());
			}
		}

		/*
		ItemUpdateToDB(pPlayer->GetID(), pItemBase->dwDBIdx, pItemBase->wIconIdx,
			pItemBase->Durability, pItemBase->Position, pItemBase->QuickPosition, pItemBase->RareIdx);

		LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemMixSuccess, 
			pPlayer->GetMoney(), ResultIdx, money, pItemBase->wIconIdx, pItemBase->dwDBIdx, pItemBase->Position, pItemBase->Position, pItemBase->Durability,
			pPlayer->GetPlayerExpPoint());
		*/

		return 0;
	}
	else
	{
		// YH 실패시 20%의 확률로 대실패가 나오게 함 [8/24/2004]
		Seed = random(0, MAX_MIX_PERCENT);
		ItemBase = *pSlot->GetItemInfoAbs(BasicItemPos);
	
#ifdef _CHINA_LOCAL_
		eITEM_KINDBIT ItemKind = GetItemKind( ItemBase.wIconIdx );
		if( ItemKind & eEQUIP_ITEM )
		{
			if( ItemKind == eEQUIP_ITEM_WEAPON || ItemKind == eEQUIP_ITEM_DRESS ||
				ItemKind == eEQUIP_ITEM_HAT || ItemKind == eEQUIP_ITEM_SHOES )
			{
				if( ItemBase.wIconIdx % 10 < 3 )
				{
					LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemMixFail, 
						pPlayer->GetMoney(), ResultIdx, money, ItemBase.wIconIdx, ItemBase.dwDBIdx, ItemBase.Position, 
						ItemBase.Position, ItemBase.Durability, pPlayer->GetPlayerExpPoint());
					return 1001;
				}
			}
		}
#endif
/*		if( g_pServerSystem->GetNation() == eNATION_CHINA )
		{
			eITEM_KINDBIT ItemKind = GetItemKind( ItemBase.wIconIdx );
			if( ItemKind & eEQUIP_ITEM )
			{
				if( ItemKind == eEQUIP_ITEM_WEAPON || ItemKind == eEQUIP_ITEM_DRESS ||
					ItemKind == eEQUIP_ITEM_HAT || ItemKind == eEQUIP_ITEM_SHOES )
				{
					if( ItemBase.wIconIdx % 10 < 3 )
					{
						LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemMixFail, 
							pPlayer->GetMoney(), ResultIdx, money, ItemBase.wIconIdx, ItemBase.dwDBIdx, ItemBase.Position, 
							ItemBase.Position, ItemBase.Durability, pPlayer->GetPlayerExpPoint());
						return 1001;
					}
				}
			}
		}*/

		//SW051010 Rare
		float fBicFail_Prob;
		int eBigFail_Kind;
		int eFail_Kind;
		DWORD dwOptionIdx;
		
		if(!ItemBase.RareIdx)
		{
			fBicFail_Prob = 0.2f;
			eBigFail_Kind = eLog_ItemMixBigFail;
			eFail_Kind = eLog_ItemMixFail;
			dwOptionIdx = ItemBase.Durability;		
		}
		else
		{
			fBicFail_Prob = 0.3f;
			eBigFail_Kind = eLog_ItemRareMixBigFail;
			eFail_Kind	= eLog_ItemRareMixFail;
			dwOptionIdx = ItemBase.RareIdx;
		}

//		if( !bUseMixUp && Seed <= MAX_MIX_PERCENT*0.2)
		if( !bUseMixUp && Seed <= MAX_MIX_PERCENT*fBicFail_Prob)
		{// 대실패
			//1. 기본 아이템 삭제
			if(EI_TRUE != DiscardItem(pPlayer, BasicItemPos, wBasicItemIdx, 9999))
				return 8;
			LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eBigFail_Kind, 
				pPlayer->GetMoney(), ResultIdx, money, ItemBase.wIconIdx, ItemBase.dwDBIdx, ItemBase.Position, 0, 
				dwOptionIdx, pPlayer->GetPlayerExpPoint());

			if( bProtection )
				return 21;
			else
				return 1000;
		}
		else
		{//실패
			LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eFail_Kind, 
				pPlayer->GetMoney(), ResultIdx, money, ItemBase.wIconIdx, ItemBase.dwDBIdx, ItemBase.Position, 
				ItemBase.Position, dwOptionIdx, pPlayer->GetPlayerExpPoint());

			if( bProtection )
				return 22;
			else
				return 1001;
		}
	}
	
	return 11;
}

const ITEMBASE * CItemManager::GetItemInfoAbsIn(CPlayer* pPlayer, POSTYPE Pos)
{
	CItemSlot * pSlot = pPlayer->GetSlot(Pos);
	if(!pSlot) return NULL;
	return pSlot->GetItemInfoAbs(Pos);
}

int CItemManager::ReinforceItem(CPlayer* pPlayer, WORD wTargetItemIdx, POSTYPE TargetPos, ITEM_JEWEL_POS_EX * pJewelInfo, WORD wJewelNum)
{
//--특기 수련 여부 체크
	if(!CHKRT->AbilityOf(pPlayer, eAUKJOB_Reinforce))
		return 1;

//--강화할 아이템 존재 체크
	if(!CHKRT->ItemOf(pPlayer, TargetPos, wTargetItemIdx, 0,0, CB_EXIST|CB_ICONIDX))
		return 2;

//---강화할 재료 아이템 존재 체크
	for(int i = 0 ; i < wJewelNum ; ++i)
	{
		if(!CHKRT->ItemOf(pPlayer, pJewelInfo[i].pos, pJewelInfo[i].wItemIdx, pJewelInfo[i].Dur,0, CB_EXIST|CB_ICONIDX))
			return 3;
	}
	
	const ITEMBASE * pTargetItemBase = GetItemInfoAbsIn(pPlayer, TargetPos);

	int eOptKind = eIOK_Normal;	//타겟 아이템의 옵션 종류. 일반(강화)/레어

	if(IsRareOptionItem(pTargetItemBase->wIconIdx, pTargetItemBase->RareIdx))	//레어 강화일 경우
	{
		eOptKind = eIOK_Rare;
	}
	
	int jnum = 0;
	BOOL bFound = FALSE;
	ITEM_JEWEL_POS_EX jewelExInfo[MAX_ITEM_OPTION_NUM];
	for( i = 0 ; i < wJewelNum ; ++i)
	{
		const ITEMBASE * pJewelItemBase = GetItemInfoAbsIn(pPlayer, pJewelInfo[i].pos);
		
		if( REINFORCEMGR->CheckValidMaterial( pTargetItemBase->wIconIdx, pJewelItemBase->wIconIdx, eOptKind ) == FALSE )	//@대상 아이템의 강화 옵션에 맞는 재료인지 확인
			return 8;

		if( REINFORCEMGR->CheckMaterialTotalGravity( pJewelItemBase->wIconIdx, (WORD)pJewelItemBase->Durability, eOptKind) == FALSE )	//재료의 총비중을 체크
			return 20;

		for(int j = 0 ; j < jnum ; ++j)
		{
			if(jewelExInfo[j].wItemIdx == pJewelInfo[i].wItemIdx)
			{
				bFound ^= TRUE;
				break;
			}
		}

		if(bFound)
		{
			jewelExInfo[j].Dur += pJewelItemBase->Durability;
			//++jnum;

			//max 체크 하자
			if( pJewelItemBase->Durability > MAX_YOUNGYAKITEM_DUPNUM || jewelExInfo[j].Dur > 100 )
				return 6;

			bFound ^= TRUE;
		}
		else
		{
			jewelExInfo[jnum].wItemIdx = pJewelInfo[i].wItemIdx;
			jewelExInfo[jnum].pos = pJewelInfo[i].pos;
			jewelExInfo[jnum].Dur = pJewelItemBase->Durability;
			++jnum;
		}
	}
	REINFORCEMGR->InitGravity();	//비중 체크 사용한 총비중 0 초기화.


	ITEM_OPTION_INFO OptionInfo;
	memset(&OptionInfo, 0, sizeof(OptionInfo));
	WORD reinforceGrade = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_ITEMREINFORCE, pPlayer->GetAbilityGroup());

	//특기 체크 추가
	if( reinforceGrade == 0 || reinforceGrade > 30 )
		return 7;

	BOOL bConvert = FALSE;

	ITEM_INFO* pBaseItemInfo = GetItemInfo( pTargetItemBase->wIconIdx );
	if( !pBaseItemInfo )
		return 4;
	
	for(i = 0 ; i < jnum ; ++i)
	{
//		ITEM_REINFORCE_INFO * pReInforceInfo = GetReinforceItemInfo(jewelExInfo[i].wItemIdx);
		
		//sITEM_REINFORCE_INFO * pReInforceInfo = REINFORCEMGR->GetReinforceInfo( jewelExInfo[i].wItemIdx );
		//SW051021
		sITEM_REINFORCE_INFO* pReInforceInfo = NULL;		//pReInforceInfo SW이거 변수명 에러;
		if(eOptKind == eIOK_Normal)
		{
			pReInforceInfo = REINFORCEMGR->GetReinforceInfo( jewelExInfo[i].wItemIdx );
		}
		else if(eOptKind == eIOK_Rare)
		{
			pReInforceInfo = (sITEM_REINFORCE_INFO*)REINFORCEMGR->GetRareReinforceInfo( jewelExInfo[i].wItemIdx );	//(cast)sITEM_RAREREINFORCE_INFO*
		}
		ASSERTMSG(pReInforceInfo, "맵 서버상에 강화 정보가 없다");
//		WORD wValue1 = REINFORCEMGR->GetValueForAbility( reinforceGrade );
//		WORD wValue2 = REINFORCEMGR->GetValueForMaterialNum( jewelExInfo[i].Dur );
//		WORD wValue3 = REINFORCEMGR->GetValueForItemLevel( pBaseItemInfo->LimitLevel );
//		WORD wValue4 = REINFORCEMGR->GetAdjustValue();

//		WORD wAvgValue = wValue1*0.4 + (wValue2*wValue3*wValue4/2500)*0.6;
//		WORD wAvgValue = wValue1*wValue2*wValue3*wValue4*4/(wValue2*wValue3*wValue4+wValue1*wValue3*wValue4+wValue1*wValue2*wValue4+wValue1*wValue2*wValue3);
		
//		float plusValue = (float)wAvgValue * pReInforceInfo->fMaxReinforce / 50.0f;

		int successSeed = random(0, 100) - (pBaseItemInfo->LimitLevel/2) + reinforceGrade;
		if(successSeed < 0)	successSeed = 0;
		float plusValue = pReInforceInfo->fw*jewelExInfo[i].Dur*successSeed/100;
		
#ifdef _JAPAN_LOCAL_
		if( pBaseItemInfo->LimitLevel > pPlayer->GetMaxLevel() )
		{
			int nLevel = pBaseItemInfo->LimitLevel - pPlayer->GetMaxLevel();
			plusValue = plusValue / 100 * (100 - nLevel * nLevel / 5 );
			if( plusValue < 0.0f )	plusValue = 0.0f;
		}
		if( plusValue < 1.0f )	plusValue = 0.0f;	//SW060628 속성값관련.. 
#endif

		switch(pReInforceInfo->ReinforceType)
		{
		case eRJP_GENGOL:
			{
				OptionInfo.GenGol = (WORD)plusValue;
				if(OptionInfo.GenGol > 0) bConvert = TRUE;
			}
			break;
		case eRJP_MINCHUB:
			{
				OptionInfo.MinChub = (WORD)plusValue;
				if(OptionInfo.MinChub > 0) bConvert = TRUE;
			}
			break;
		case eRJP_CHERYUK:
			{
				OptionInfo.CheRyuk = (WORD)plusValue;
				if(OptionInfo.CheRyuk > 0) bConvert = TRUE;
			}
			break;
		case eRJP_SIMMEK:
			{
				OptionInfo.SimMek = (WORD)plusValue;
				if(OptionInfo.SimMek > 0) bConvert = TRUE;
			}
			break;
		case eRJP_LIFE:
			{
				OptionInfo.Life = (DWORD)plusValue;
				if(OptionInfo.Life > 0) bConvert = TRUE;
			}
			break;
		case eRJP_NAERYUK:
			{
				OptionInfo.NaeRyuk = (WORD)plusValue;
				if(OptionInfo.NaeRyuk > 0) bConvert = TRUE;
			}
			break;
		case eRJP_SHIELD:
			{
				OptionInfo.Shield = (DWORD)plusValue;
				if(OptionInfo.Shield > 0) bConvert = TRUE;
			}
			break;
		case eRJP_FIREREGIST:
			{
				plusValue = (float)(floor(100.*(plusValue+0.005))/100.);
				if( (int)(plusValue*100) >= 1 )
				{
					OptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE, plusValue);
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_WATERREGIST:
			{
				plusValue = (float)(floor(100.*(plusValue+0.005))/100.);
				if( (int)(plusValue*100) >= 1 )
				{
					OptionInfo.AttrRegist.SetElement_Val(ATTR_WATER, plusValue);
					bConvert = TRUE;
				}
				
			}
			break;
		case eRJP_TREEREGIST:
			{
				plusValue = (float)(floor(100.*(plusValue+0.005))/100.);
				if( (int)(plusValue*100) >= 1 )
				{
					OptionInfo.AttrRegist.SetElement_Val(ATTR_TREE, plusValue);
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_GOLDREGIST:
			{
				plusValue = (float)(floor(100.*(plusValue+0.005))/100.);
				if( (int)(plusValue*100) >= 1 )
				{
					OptionInfo.AttrRegist.SetElement_Val(ATTR_IRON, plusValue);
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_EARTHREGIST:
			{
				plusValue = (float)(floor(100.*(plusValue+0.005))/100.);
				if( (int)(plusValue*100) >= 1 )
				{
					OptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH, plusValue);
					bConvert = TRUE;
				}
			}
			break;
		case eRJP_PHYATTACK:
			{
				OptionInfo.PhyAttack = (WORD)plusValue;
				if(OptionInfo.PhyAttack > 0) bConvert = TRUE;
			}
			break;
		case eRJP_CRITICAL:
			{
				OptionInfo.CriticalPercent = (WORD)plusValue;
				if(OptionInfo.CriticalPercent > 0) bConvert = TRUE;
			}
			break;		
		case eRJP_FIREATTACK:
			{
				plusValue = (float)(floor(100.*(plusValue+0.005))/100.);
				if( (int)(plusValue*100) >= 1 )
				{
					OptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE, plusValue);
					bConvert = TRUE;
				}
			}
			break;	
		case eRJP_WATERATTACK:
			{
				plusValue = (float)(floor(100.*(plusValue+0.005))/100.);
				if( (int)(plusValue*100) >= 1 )
				{
					bConvert = TRUE;
					OptionInfo.AttrAttack.SetElement_Val(ATTR_WATER, plusValue);
				}
			}
			break;	
		case eRJP_TREEATTACK:
			{
				plusValue = (float)(floor(100.*(plusValue+0.005))/100.);
				if( (int)(plusValue*100) >= 1 )
				{
					bConvert = TRUE;
					OptionInfo.AttrAttack.SetElement_Val(ATTR_TREE, plusValue);
				}
			}
			break;	
		case eRJP_GOLDATTACK:
			{
				plusValue = (float)(floor(100.*(plusValue+0.005))/100.);
				if( (int)(plusValue*100) >= 1 )
				{
					bConvert = TRUE;
					OptionInfo.AttrAttack.SetElement_Val(ATTR_IRON, plusValue);
				}
			}
			break;	
		case eRJP_EARTHATTACK:
			{
				plusValue = (float)(floor(100.*(plusValue+0.005))/100.);
				if( (int)(plusValue*100) >= 1 )
				{
					bConvert = TRUE;
					OptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH, plusValue);
				}
			}
			break;	
		case eRJP_PHYDEFENSE:
			{
				OptionInfo.PhyDefense = (WORD)plusValue;
				if(OptionInfo.PhyDefense > 0) bConvert = TRUE;
			}
			break;	
		default:
			{
				return 3;
			}
		}
	}

	DWORD Key = MakeNewKey();
	
	ITEMBASE Item;
	memset( &Item, 0, sizeof(ITEMBASE) );
	char NameKey[MAX_NAME_LENGTH+1];
	sprintf(NameKey, "%d", Key);
	CItemSlot * pSlot = pPlayer->GetSlot(0); //인벤일테니

	//2. Ac¡¤a ¨u¨¡AIAU ¡ief
	for(i = 0 ; i < wJewelNum ; ++i)
	{
		Item = *pSlot->GetItemInfoAbs(pJewelInfo[i].pos);
		if(EI_TRUE != DiscardItem(pPlayer, pJewelInfo[i].pos, pJewelInfo[i].wItemIdx, 9999))
			return 5;

		LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemDestroyReinforce, 
			pPlayer->GetMoney(), pPlayer->GetMoney(), 0, Item.wIconIdx, Item.dwDBIdx, Item.Position, 0, 
			Item.Durability, pPlayer->GetPlayerExpPoint());
	}
	
	if(bConvert)
	{
		// ¨u¨￢¡Æⓒª
		//3. ￠?E¨uC ¡ief TO DB
		if(pTargetItemBase->Durability != 0)
			CharacterItemOptionDelete(pTargetItemBase->Durability, pTargetItemBase->dwDBIdx);
		
		//4. ￠?ⓒª¡¤a Dura update & preinsert
		CItemSlot * pTargetSlot = pPlayer->GetSlot(TargetPos);
		if( pTargetSlot->UpdateItemAbs(pPlayer, TargetPos, 0, 0, 0, 0, 0, UB_DURA, SS_PREINSERT ) != EI_TRUE )
		{
			return 6; 
		}
		OptionInfo.dwItemDBIdx = pTargetItemBase->dwDBIdx;
		ItemOptionInsertToDB(pPlayer->GetID(), wTargetItemIdx, TargetPos, &OptionInfo);

		LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemReinforceSuccess, 
			pPlayer->GetMoney(), pPlayer->GetMoney(), 0, pTargetItemBase->wIconIdx, pTargetItemBase->dwDBIdx, 
			pTargetItemBase->Position, pTargetItemBase->Position, pTargetItemBase->Durability, pPlayer->GetPlayerExpPoint());
	}
	else            
	{
		// ¨oC¨¡¨￠
		LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemReinforceFail, 
			pPlayer->GetMoney(), pPlayer->GetMoney(), 0, pTargetItemBase->wIconIdx, pTargetItemBase->dwDBIdx, 
			pTargetItemBase->Position, pTargetItemBase->Position, pTargetItemBase->Durability, pPlayer->GetPlayerExpPoint());
		return 99;
	}

	return 0;
}


int CItemManager::ReinforceItemWithShopItem(CPlayer* pPlayer, WORD wTargetItemIdx, POSTYPE TargetPos, WORD wShopItemIdx, POSTYPE ShopItemPos, ITEM_JEWEL_POS_EX * pJewelInfo, WORD wJewelNum)
{
//--특기 수련 여부 체크
	if(!CHKRT->AbilityOf(pPlayer, eAUKJOB_Reinforce))
		return 1;
//--강화할 아이템이 강화아이템인지 체크
	const ITEMBASE* pTargetItemBase = GetItemInfoAbsIn( pPlayer, TargetPos );
	if( !pTargetItemBase )			return 2;
	ITEM_OPTION_INFO* pOptionInfo = pPlayer->GetItemOption( pTargetItemBase->Durability );
	if( pOptionInfo->GetOptionType() == 0 )
		return 3;

//-- 아이템몰 아이템이 있는지 확인
	if( !CHKRT->ItemOf( pPlayer, ShopItemPos, wShopItemIdx, 0, 0, CB_EXIST|CB_ICONIDX ) )
		return 12;
	
//---강화할 재료 아이템 존재 체크
	for(int i = 0 ; i < wJewelNum ; ++i)
	{
		if(!CHKRT->ItemOf(pPlayer, pJewelInfo[i].pos, pJewelInfo[i].wItemIdx, pJewelInfo[i].Dur,0, CB_EXIST|CB_ICONIDX))
			return 13;
	}
//---레벨이 맞는 아이템인지 체크 
	ITEM_INFO* pShopItem = GetItemInfo( wShopItemIdx );
	if( !pShopItem )		return 12;
	ITEM_INFO* pBaseItemInfo = GetItemInfo( wTargetItemIdx );
	if( !pBaseItemInfo )			return 2;
	if( pBaseItemInfo->LimitLevel < pShopItem->GenGol || pBaseItemInfo->LimitLevel > pShopItem->MinChub )
		return 4;

	// 06.09.25 RaMa - 이벤트 아이템이 강화, 레어 금지
	if( pBaseItemInfo->WeaponType > WP_KEY )
		return 12;

	int eOptKind = eIOK_Normal;	//타겟 아이템의 옵션 종류. 일반(강화)/레어

	if(IsRareOptionItem(pTargetItemBase->wIconIdx, pTargetItemBase->RareIdx))	//레어 강화일 경우
	{
		eOptKind = eIOK_Rare;
	}

// 
//	sITEM_REINFORCE_INFO* pReinforceInfo = REINFORCEMGR->GetReinforceInfo( pJewelInfo[0].wItemIdx );
//	if( !pReinforceInfo )			return 2;

	//SW051021
	sITEM_REINFORCE_INFO* pReinforceInfo = NULL;		//pReInforceInfo SW이거 변수명 에러;
	if(eOptKind == eIOK_Normal)
	{
		pReinforceInfo = REINFORCEMGR->GetReinforceInfo( pJewelInfo[0].wItemIdx );
	}
	else if(eOptKind == eIOK_Rare)
	{
		pReinforceInfo = (sITEM_REINFORCE_INFO*)REINFORCEMGR->GetRareReinforceInfo( pJewelInfo[0].wItemIdx );	//(cast)sITEM_RAREREINFORCE_INFO*
	}
	ASSERTMSG(pReinforceInfo, "맵 서버상에 강화 정보가 없다");
	if( !pReinforceInfo )	return 2;

	int jnum = 0;
	BOOL bFound = FALSE;
	DWORD jewelIdx = 0;
	float ReinforceBackup = -1;
	ITEM_JEWEL_POS_EX jewelExInfo[MAX_ITEM_OPTION_NUM];
	for( i = 0 ; i < wJewelNum ; ++i)
	{
		const ITEMBASE * pJewelItemBase = GetItemInfoAbsIn(pPlayer, pJewelInfo[i].pos);
		//
//		if( REINFORCEMGR->CheckValidMaterial( pTargetItemBase->wIconIdx, pJewelItemBase->wIconIdx ) == FALSE )
//			return 11;
		if( REINFORCEMGR->CheckValidMaterial( pTargetItemBase->wIconIdx, pJewelItemBase->wIconIdx, eOptKind ) == FALSE )	//@대상 아이템의 강화 옵션에 맞는 재료인지 확인
			return 11;

		if( REINFORCEMGR->CheckMaterialTotalGravity( pJewelItemBase->wIconIdx, (WORD)pJewelItemBase->Durability, eOptKind) == FALSE )	//재료의 총비중을 체크
			return 20;

		// 강화가 가능한지 살피고 현재 강화된 수치를 리턴받는다.
		float res = REINFORCEMGR->GetReinforceWithMetariel( pOptionInfo, pJewelItemBase->wIconIdx, eOptKind);
		if( res == 0 )						return 11;
		// 혹시나 다르면 실패
		if( ReinforceBackup != res )		
		{
			// 강화된 수치를 저장해놓고
			if( ReinforceBackup == -1 )			ReinforceBackup = res;
			else								return 5;
		}

		// 넣은 보석의 종류가 달라도 실패
		if( jewelIdx && jewelIdx != pJewelItemBase->wIconIdx )
			return 6;
		else if( !jewelIdx )
			jewelIdx = pJewelItemBase->wIconIdx;

		for(int j=0; j<jnum; ++j)
		{
			if(jewelExInfo[j].wItemIdx == pJewelInfo[i].wItemIdx)
			{
				bFound ^= TRUE;
				break;
			}
		}

		if(bFound)
		{
			jewelExInfo[j].Dur += pJewelItemBase->Durability;
			//++jnum;

			//max 체크 하자
			if( pJewelItemBase->Durability > MAX_YOUNGYAKITEM_DUPNUM || jewelExInfo[j].Dur > 100 )
				return 6;

			bFound ^= TRUE;
		}
		else
		{
			jewelExInfo[jnum].wItemIdx = pJewelInfo[i].wItemIdx;
			jewelExInfo[jnum].pos = pJewelInfo[i].pos;
			jewelExInfo[jnum].Dur = pJewelItemBase->Durability;
			++jnum;
		}
	}
	REINFORCEMGR->InitGravity();

	ITEM_OPTION_INFO OptionInfo;
	memset(&OptionInfo, 0, sizeof(OptionInfo));
	WORD reinforceGrade = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_ITEMREINFORCE, pPlayer->GetAbilityGroup());

	//특기 체크 추가
	if( reinforceGrade == 0 || reinforceGrade > 30 )
		return 7;

	BOOL bConvert = FALSE;
	float plusValue = 0;
//	sITEM_REINFORCE_INFO * pReInforceInfo = REINFORCEMGR->GetReinforceInfo( jewelExInfo[0].wItemIdx );
	sITEM_REINFORCE_INFO * pReInforceInfo = NULL;
	if(eOptKind == eIOK_Normal)
	{
		pReInforceInfo = REINFORCEMGR->GetReinforceInfo( jewelExInfo[0].wItemIdx );
	}
	else if(eOptKind == eIOK_Rare)
	{
		pReInforceInfo = (sITEM_REINFORCE_INFO*)REINFORCEMGR->GetRareReinforceInfo( jewelExInfo[0].wItemIdx );	//(cast)sITEM_RAREREINFORCE_INFO*
	}
	else
		ASSERT(0);
	
	int successSeed = random(0, 100) - (pBaseItemInfo->LimitLevel/2) + reinforceGrade;
	if(successSeed < 0)	successSeed = 0;
	plusValue = pReInforceInfo->fw*jewelExInfo[0].Dur*successSeed/100;

	// 수치가 낮은지 확인해 본다.
	switch( pReinforceInfo->ReinforceType )
	{
		case eRJP_FIREREGIST:
		case eRJP_WATERREGIST:
		case eRJP_TREEREGIST:
		case eRJP_GOLDREGIST:
		case eRJP_EARTHREGIST:
		case eRJP_FIREATTACK:
		case eRJP_WATERATTACK:
		case eRJP_TREEATTACK:
		case eRJP_GOLDATTACK:
		case eRJP_EARTHATTACK:
			{
				plusValue = (float)(floor(100.*(plusValue+0.005))/100.);
			}
			break;
	}

	BOOL bProtection = FALSE;
	// BackupData와 비교해서 수치가 작으면 갱신
	if( plusValue <= ReinforceBackup )
	{
		plusValue = ReinforceBackup;
		bProtection = TRUE;
	}

	switch( pReInforceInfo->ReinforceType )
	{
	case eRJP_GENGOL:
		{
			OptionInfo.GenGol = (WORD)plusValue;
			if(OptionInfo.GenGol > 0) bConvert = TRUE;
		}
		break;
	case eRJP_MINCHUB:
		{
			OptionInfo.MinChub = (WORD)plusValue;
			if(OptionInfo.MinChub > 0) bConvert = TRUE;
		}
		break;
	case eRJP_CHERYUK:
		{
			OptionInfo.CheRyuk = (WORD)plusValue;
			if(OptionInfo.CheRyuk > 0) bConvert = TRUE;
		}
		break;
	case eRJP_SIMMEK:
		{
			OptionInfo.SimMek = (WORD)plusValue;
			if(OptionInfo.SimMek > 0) bConvert = TRUE;
		}
		break;
	case eRJP_LIFE:
		{
			OptionInfo.Life = (DWORD)plusValue;
			if(OptionInfo.Life > 0) bConvert = TRUE;
		}
		break;
	case eRJP_NAERYUK:
		{
			OptionInfo.NaeRyuk = (WORD)plusValue;
			if(OptionInfo.NaeRyuk > 0) bConvert = TRUE;
		}
		break;
	case eRJP_SHIELD:
		{
			OptionInfo.Shield = (DWORD)plusValue;
			if(OptionInfo.Shield > 0) bConvert = TRUE;
		}
		break;
	case eRJP_FIREREGIST:
		{
			if( (int)(plusValue*100) >= 1 )
			{
				OptionInfo.AttrRegist.SetElement_Val(ATTR_FIRE, plusValue);
				bConvert = TRUE;
			}
		}
		break;
	case eRJP_WATERREGIST:
		{
			if( (int)(plusValue*100) >= 1 )
			{
				OptionInfo.AttrRegist.SetElement_Val(ATTR_WATER, plusValue);
				bConvert = TRUE;
			}
			
		}
		break;
	case eRJP_TREEREGIST:
		{
			if( (int)(plusValue*100) >= 1 )
			{
				OptionInfo.AttrRegist.SetElement_Val(ATTR_TREE, plusValue);
				bConvert = TRUE;
			}
		}
		break;
	case eRJP_GOLDREGIST:
		{
			if( (int)(plusValue*100) >= 1 )
			{
				OptionInfo.AttrRegist.SetElement_Val(ATTR_IRON, plusValue);
				bConvert = TRUE;
			}
		}
		break;
	case eRJP_EARTHREGIST:
		{
			if( (int)(plusValue*100) >= 1 )
			{
				OptionInfo.AttrRegist.SetElement_Val(ATTR_EARTH, plusValue);
				bConvert = TRUE;
			}
		}
		break;
	case eRJP_PHYATTACK:
		{
			OptionInfo.PhyAttack = (WORD)plusValue;
			if(OptionInfo.PhyAttack > 0) bConvert = TRUE;
		}
		break;
	case eRJP_CRITICAL:
		{
			OptionInfo.CriticalPercent = (WORD)plusValue;
			if(OptionInfo.CriticalPercent > 0) bConvert = TRUE;
		}
		break;		
	case eRJP_FIREATTACK:
		{
			if( (int)(plusValue*100) >= 1 )
			{
				OptionInfo.AttrAttack.SetElement_Val(ATTR_FIRE, plusValue);
				bConvert = TRUE;
			}
		}
		break;	
	case eRJP_WATERATTACK:
		{
			if( (int)(plusValue*100) >= 1 )
			{
				bConvert = TRUE;
				OptionInfo.AttrAttack.SetElement_Val(ATTR_WATER, plusValue);
			}
		}
		break;	
	case eRJP_TREEATTACK:
		{
			if( (int)(plusValue*100) >= 1 )
			{
				bConvert = TRUE;
				OptionInfo.AttrAttack.SetElement_Val(ATTR_TREE, plusValue);
			}
		}
		break;	
	case eRJP_GOLDATTACK:
		{
			if( (int)(plusValue*100) >= 1 )
			{
				bConvert = TRUE;
				OptionInfo.AttrAttack.SetElement_Val(ATTR_IRON, plusValue);
			}
		}
		break;	
	case eRJP_EARTHATTACK:
		{
			if( (int)(plusValue*100) >= 1 )
			{
				bConvert = TRUE;
				OptionInfo.AttrAttack.SetElement_Val(ATTR_EARTH, plusValue);
			}
		}
		break;	
	case eRJP_PHYDEFENSE:
		{
			OptionInfo.PhyDefense = (WORD)plusValue;
			if(OptionInfo.PhyDefense > 0) bConvert = TRUE;
		}
		break;	
	default:
		{
			return 3;
		}
	}

	DWORD Key = MakeNewKey();
	
	ITEMBASE Item;
	memset( &Item, 0, sizeof(ITEMBASE) );
	char NameKey[MAX_NAME_LENGTH+1];
	sprintf(NameKey, "%d", Key);
	CItemSlot * pSlot = pPlayer->GetSlot(0); //인벤일테니

	//2. Ac¡¤a ¨u¨¡AIAU ¡ief
	for(i = 0 ; i < wJewelNum ; ++i)
	{
		Item = *pSlot->GetItemInfoAbs(pJewelInfo[i].pos);
		if(EI_TRUE != DiscardItem(pPlayer, pJewelInfo[i].pos, pJewelInfo[i].wItemIdx, 9999))
			return 5;
		
		LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemDestroyReinforceWithShopItem, 
			pPlayer->GetMoney(), pPlayer->GetMoney(), wShopItemIdx, Item.wIconIdx, Item.dwDBIdx, Item.Position, 0, 
			Item.Durability, pPlayer->GetPlayerExpPoint());
	}

	// 해당 강화 보호의 돌을 지워준다.
	const ITEMBASE* pShop = GetItemInfoAbsIn( pPlayer, ShopItemPos );
	if( pShopItem )
	{
		if( EI_TRUE != DiscardItem( pPlayer, ShopItemPos, wShopItemIdx, 1 ) )
			ASSERT(0);

		SEND_SHOPITEM_BASEINFO msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_USE_ACK;		
		msg.ShopItemPos = ShopItemPos;
		msg.ShopItemIdx = wShopItemIdx;
		SendAckMsg(pPlayer, &msg, sizeof(msg));

		LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ShopItemUse, 
			pPlayer->GetMoney(), pPlayer->GetMoney(), 0, wShopItemIdx, pShop->dwDBIdx, ShopItemPos, 0, 
			Item.Durability, pPlayer->GetPlayerExpPoint());
	}

	if( bProtection )
	{
		LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemReinforceProtectWithShopItem, 
			pPlayer->GetMoney(), pPlayer->GetMoney(), wShopItemIdx, pTargetItemBase->wIconIdx, pTargetItemBase->dwDBIdx, 
			pTargetItemBase->Position, pTargetItemBase->Position, pTargetItemBase->Durability, pPlayer->GetPlayerExpPoint());

		return 10;
	}	
	else if( bConvert )
	{
		// ¨u¨￢¡Æⓒª
		//3. ￠?E¨uC ¡ief TO DB
		if(pTargetItemBase->Durability != 0)
			CharacterItemOptionDelete(pTargetItemBase->Durability, pTargetItemBase->dwDBIdx);
		
		//4. ￠?ⓒª¡¤a Dura update & preinsert
		CItemSlot * pTargetSlot = pPlayer->GetSlot(TargetPos);
		if( pTargetSlot->UpdateItemAbs(pPlayer, TargetPos, 0, 0, 0, 0, 0, UB_DURA, SS_PREINSERT ) != EI_TRUE )
		{
			return 6; 
		}
		OptionInfo.dwItemDBIdx = pTargetItemBase->dwDBIdx;
		ItemOptionInsertToDB(pPlayer->GetID(), wTargetItemIdx, TargetPos, &OptionInfo);

		LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemReinforceSuccessWithShopItem, 
			pPlayer->GetMoney(), pPlayer->GetMoney(), wShopItemIdx, pTargetItemBase->wIconIdx, pTargetItemBase->dwDBIdx, 
			pTargetItemBase->Position, pTargetItemBase->Position, pTargetItemBase->Durability, pPlayer->GetPlayerExpPoint());
	}
	else
	{
		// ¨oC¨¡¨￠
		LogItemMoney(pPlayer->GetID(), NameKey, pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemReinforceFailWithShopItem, 
			pPlayer->GetMoney(), pPlayer->GetMoney(), wShopItemIdx, pTargetItemBase->wIconIdx, pTargetItemBase->dwDBIdx, 
			pTargetItemBase->Position, pTargetItemBase->Position, pTargetItemBase->Durability, pPlayer->GetPlayerExpPoint());
		return 99;
	}

	return 0;
}

void CItemManager::ReinforceItemDBResult(CPlayer * pPlayer, WORD wItemIdx, POSTYPE TargetPos, ITEM_OPTION_INFO * pOptionInfo)
{
	// ￠?ⓒª¡¤a ¨u¨¡AIAU update
	CItemSlot * pTargetSlot = pPlayer->GetSlot(TargetPos);
	if( pTargetSlot->UpdateItemAbs(pPlayer, TargetPos, 0, 0, 0, 0, pOptionInfo->dwOptionIdx, UB_DURA) != EI_TRUE )
	{
		ASSERTMSG(0, "ⓒoI￠?i; ¨￢O¡¤? ¨￢￠￢¨u¨¡￠?a!-¨u¨¡AIAU ¡Æ¡ⓒE¡ⓒ PREINSERT￥iE ¨u¨¡AIAU￠?￠® dura update ￠?￠®¡¤?");
		return ;
	}
	const ITEMBASE * pItemBase = pTargetSlot->GetItemInfoAbs(TargetPos);
	LogItemOption(pPlayer->GetID(), pItemBase->dwDBIdx, pOptionInfo);
	
	MSG_ITEM_REINFORCE_ACK msg;
	msg.Category							= MP_ITEM;
	msg.Protocol							= MP_ITEM_REINFORCE_SUCCESS_ACK;
	msg.dwObjectID							= pPlayer->GetID();
	msg.wTargetItemIdx						= wItemIdx;
	msg.TargetPos							= TargetPos;
	msg.OptionInfo							= *pOptionInfo;
	pPlayer->SendMsg(&msg, sizeof(msg));
}

//SW050920
//void CItemManager::RareItemDBResult(CPlayer* pPlayer, WORD wItemIdx, POSTYPE TargetPos, ITEM_RARE_OPTION_INFO* pRareOptionInfo)
void CItemManager::RareItemDBResult(CPlayer* pPlayer, WORD ArrayID, ITEM_RARE_OPTION_INFO* pRareOptionInfo)
{
	ITEMOBTAINARRAYINFO * pArrayInfo = pPlayer->GetArray(ArrayID);

//	ASSERT(pArrayInfo->ItemArray.ItemNum == 0);
	ITEMBASE * ItemInfo = pArrayInfo->ItemArray.GetItem(0);
	CItemSlot * pSlot = pPlayer->GetSlot(ItemInfo->Position);
	if(pSlot->InsertItemAbs(pPlayer, ItemInfo->Position, ItemInfo) != EI_TRUE)
	{
		ASSERT(0);
		return;
	}
//	if( pTargetSlot->UpdateItemAbs(pPlayer, TargetPos, 0, 0, 0, 0, pRareOptionInfo->dwRareOptionIdx, UB_RARE) != EI_TRUE )
//	{
//		ASSERT(0);
//		return;
//	}
	
	//SW070821 변경 ItemInfo->Durability => ItemInfo->RareIdx,
	LogItemMoney(pArrayInfo->dwFurnisherIdx, "", pPlayer->GetID(), pPlayer->GetObjectName(),
		pArrayInfo->wType, 0, pPlayer->GetMoney(eItemTable_Inventory), 0,
		ItemInfo->wIconIdx, ItemInfo->dwDBIdx, 0, ItemInfo->Position, ItemInfo->RareIdx, pPlayer->GetPlayerExpPoint());

	// 06.09.12 RaMa
	if( pArrayInfo->wType == eLog_ItemRareObtainCheat )
	{
		LogGMToolUse( pPlayer->GetID(), eGMLog_Item, eLog_ItemRareObtainCheat, 
			ItemInfo->wIconIdx, ItemInfo->dwDBIdx );
	}

	LogItemRareOption(pPlayer->GetID(), ItemInfo->dwDBIdx, pRareOptionInfo);

	MSG_ITEM_RAREITEM_GET msg;
	msg.Category = pArrayInfo->ItemArray.Category;	//MP_ITEM
	msg.Protocol = pArrayInfo->ItemArray.Protocol;	//MP_ITEM_MONSTER_RAREOBTAIN_NOTIFY
	msg.dwObjectID = pArrayInfo->ItemArray.dwObjectID;	//pPlayer->GetID();
	msg.RareItemBase = *pArrayInfo->ItemArray.GetItem(0);
	msg.RareInfo = *pRareOptionInfo;

	pPlayer->SendMsg(&msg, sizeof(msg));

	ITEMMGR->Free(pPlayer, pArrayInfo);
}

void CItemManager::TitanUpgradeItemDBResult(CPlayer* pPlayer, WORD ArrayID)
{
	ITEMOBTAINARRAYINFO * pArrayInfo = pPlayer->GetArray(ArrayID);

	ASSERT(pArrayInfo->ItemArray.ItemNum == 0);
	ITEMBASE * ItemInfo = pArrayInfo->ItemArray.GetItem(0);
	CItemSlot * pSlot = pPlayer->GetSlot(ItemInfo->Position);
	if(pSlot->InsertItemAbs(pPlayer, ItemInfo->Position, ItemInfo) != EI_TRUE)
	{
		ASSERT(0);
		return;
	}

	LogItemMoney(pArrayInfo->dwFurnisherIdx, "", pPlayer->GetID(), pPlayer->GetObjectName(),
		pArrayInfo->wType, 0, pPlayer->GetMoney(eItemTable_Inventory), 0,
		ItemInfo->wIconIdx, ItemInfo->dwDBIdx, 0, ItemInfo->Position, ItemInfo->Durability, pPlayer->GetPlayerExpPoint());

	pPlayer->SendMsg( &pArrayInfo->ItemArray, pArrayInfo->ItemArray.GetSize() );

	// 타이탄 증서 파라메터를 DB에 갱신
	ItemInfo->ItemParam |= ITEM_PARAM_TITAN_REGIST;
	ShopItemParamUpdateToDB(pPlayer->GetID(), ItemInfo->dwDBIdx, ItemInfo->ItemParam);

	pPlayer->GetTitanManager()->CreateUpgradeTitan(pPlayer, ItemInfo->wIconIdx, ItemInfo->dwDBIdx);

	pPlayer->GetTitanManager()->SetRegistTitanCallItemDBIdx(ItemInfo->dwDBIdx);

	ITEMMGR->Free(pPlayer, pArrayInfo);
}

int CItemManager::UpgradeItem(CPlayer* pPlayer, WORD & TargetItemIdx, POSTYPE TargetPos, WORD MatItemIdx, POSTYPE MatItemPos)
{
	if(!CHKRT->AbilityOf(pPlayer, eAUKJOB_Upgrade))
		return 1;
	if(!CHKRT->ItemOf(pPlayer, TargetPos, TargetItemIdx, 0,0, CB_EXIST|CB_ICONIDX))
		return 2;

	if(!CHKRT->ItemOf(pPlayer, MatItemPos, MatItemIdx, 0,0, CB_EXIST|CB_ICONIDX))
		return 3;

	ITEM_INFO * pTargetItem = GetItemInfo(TargetItemIdx);
	WORD upgradeGrade = ABILITYMGR->GetAbilityLevel(ABILITYINDEX_ITEMUPGRADE, pPlayer->GetAbilityGroup());

	// ￥ii¡¾¨­AI 30¡¾iAo ￠?8¡I¡¾a￠O¡×ⓒo¡i￠?￠® *10->*5¡¤I ⓒoUⓒ÷¨­.
	int tmpValue = (int)((20 - ( (pTargetItem->LimitLevel / 2) + (pTargetItem->ItemGrade*18) ) + random(0, 300) + upgradeGrade*0.2 ));
	if(tmpValue<0)
		tmpValue -= 100;
	int gradeUp = tmpValue/100;

	if(gradeUp <-2)
	{
		gradeUp = -2;
	}
	else if(gradeUp > 3)
	{
		// ¨u¨¡AIAU ¨ui¨￢o￠￢￠c¨¡¨u ￥ii¡¾¨­AI ¡iy¡¾a¡Æi ⓒø¨￡¨u¡ⓒ ¨￢IAI >3 ¨￢￠￢￠￥U A¡i¡ÆO ⓒø¨￡￠?E
		// IF > 3 , then = 3
		gradeUp = 3;
	}

	int tmpGrade = pTargetItem->ItemGrade + gradeUp;
	
	int var=0;
	if(tmpGrade>9)
		var = 9-pTargetItem->ItemGrade;
	else if(tmpGrade<0)
		var = -pTargetItem->ItemGrade;
	else
		var = gradeUp;

	TargetItemIdx += var;
	// AI¨￢¡IAa￠￢￠c ¨u¡%i¡IAI¨¡￠c & DB 
	
	if(EI_TRUE != DiscardItem(pPlayer, MatItemPos, MatItemIdx, 9999))
		return 5;
	
	// Log 기능 들어가지 않음. 혹시나해서 남겨봄.
	LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
		eLog_ItemDestroyUpgrade, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
		TargetItemIdx , 0, MatItemPos, 0, 0, pPlayer->GetPlayerExpPoint());

	CItemSlot * pSlot = pPlayer->GetSlot(TargetPos);
	pSlot->UpdateItemAbs(pPlayer,TargetPos, 0, TargetItemIdx,0,0,0, UB_ICONIDX);
	const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(TargetPos);

	// db
	ItemUpdateToDB(pPlayer->GetID(), pItemBase->dwDBIdx, pItemBase->wIconIdx,
					pItemBase->Durability, pItemBase->Position, pItemBase->QuickPosition, pItemBase->RareIdx );

	return 0;
}



ITEM_INFO * CItemManager::GetItemInfo(WORD wItemIdx)
{
	return m_ItemInfoList.GetData(wItemIdx);
}
ITEM_MIX_INFO * CItemManager::GetMixItemInfo(WORD wItemIdx)
{
	return m_MixItemInfoList.GetData(wItemIdx);
}
ITEM_REINFORCE_INFO * CItemManager::GetReinforceItemInfo(WORD wItemIdx)
{
	return m_ReinforceItemInfoList.GetData(wItemIdx);
}
void CItemManager::LoadMixResultItemInfo(CMHFile * fp, ITEM_MIX_RES * pResInfo)
{
	pResInfo->wResItemIdx = fp->GetWord();
	SafeStrCpy(pResInfo->szResItemName, fp->GetString(), MAX_ITEMNAME_LENGTH+1 );
	pResInfo->wMixKind = fp->GetWord();
	pResInfo->Money = fp->GetDword();
	pResInfo->SuccessRatio = fp->GetWord();
	pResInfo->wMaterialItemNum = fp->GetWord();
	pResInfo->psMaterialItemInfo = new ITEM_MIX_MATERIAL[pResInfo->wMaterialItemNum];
	for( int i = 0 ; i < pResInfo->wMaterialItemNum ; ++i)
	{
		pResInfo->psMaterialItemInfo[i].wMatItemIdx = fp->GetWord();
		SafeStrCpy(pResInfo->psMaterialItemInfo[i].szMatItemName, fp->GetString(), MAX_ITEMNAME_LENGTH+1);
		pResInfo->psMaterialItemInfo[i].wMatItemNum = fp->GetWord();
	}
}
void CItemManager::LoadMixList()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if(!file.Init("Resource/ItemMixList.bin", "rb"))
		return;
#else
	if(!file.Init("Resource/ItemMixList.txt", "rt"))
		return;
#endif

	ITEM_MIX_INFO * pInfo = NULL;
	WORD wTmpItemIdx = 0;
	WORD wTmpRestIdx = 0;
	while(1)
	{
		if(file.IsEOF())
			break;

		wTmpItemIdx = file.GetWord();
		pInfo = m_MixItemInfoList.GetData(wTmpItemIdx);
		if(!pInfo)
		{
			pInfo = new ITEM_MIX_INFO;
			memset(pInfo, 0, sizeof(ITEM_MIX_INFO));
		}
		pInfo->wItemIdx = wTmpItemIdx;
		SafeStrCpy(pInfo->szItemName, file.GetString(), MAX_ITEMNAME_LENGTH+1);
		pInfo->wResultItemNum = file.GetWord();
		if(!pInfo->psResultItemInfo)
		{
			pInfo->psResultItemInfo = new ITEM_MIX_RES[pInfo->wResultItemNum];
			memset(pInfo->psResultItemInfo, 0, sizeof(ITEM_MIX_RES)*pInfo->wResultItemNum);
		}
		for(wTmpRestIdx = 0 ; wTmpRestIdx < pInfo->wResultItemNum ; ++wTmpRestIdx)
		{	
			if(pInfo->psResultItemInfo[wTmpRestIdx].wResItemIdx == 0)
			{
				LoadMixResultItemInfo(&file, &pInfo->psResultItemInfo[wTmpRestIdx]);
				break;
			}
		}

		if(!m_MixItemInfoList.GetData(pInfo->wItemIdx))
			m_MixItemInfoList.Add(pInfo, pInfo->wItemIdx);
		pInfo = NULL;
	}
	file.Release();
#ifdef _FILE_BIN_
	if(!file.Init("Resource/Server/HiLevelItemMixRate.bin", "rb"))
		return;
#else
	if(!file.Init("Resource/Server/HiLevelItemMixRate.txt", "rt"))
		return;
#endif
	while(1)
	{
		if(file.IsEOF())
			break;

		HILEVEL_ITEM_MIX_RATE_INFO*	pRateInfo = NULL;
		
		pRateInfo = new HILEVEL_ITEM_MIX_RATE_INFO;
		ZeroMemory( pRateInfo, sizeof(HILEVEL_ITEM_MIX_RATE_INFO) );

		pRateInfo->wItemLevel	= file.GetWord();
		pRateInfo->dwBaseRate	= file.GetDword();
		pRateInfo->dwModulate	= file.GetWord();

		m_HiLevelItemMixRateInfoList.Add( pRateInfo, pRateInfo->wItemLevel );
	}
	//////////////////////////////////////////////////////////////////////////
#ifdef _HK_LOCAL_
//LOAD ITEMMIXBAL
	CMHFile file2;
#ifdef _FILE_BIN_
	if(!file2.Init("Resource/ItemMixBal.bin", "rb"))
		return;
#else
	if(!file2.Init("Resource/ItemMixBal.txt", "rt"))
		return;
#endif

//MIXBAL
	for( int i = 0 ; i < 99*9 ; ++i )
	{
		if(file2.IsEOF())
			break;

		m_nItemMixBal[i/9][i%9] = file2.GetInt();
	}

	file2.Release();
#endif
}

void CItemManager::LoadItemList()
{
	ITEM_INFO * pInfo = NULL;
	CMHFile file;
#ifdef _FILE_BIN_
	if(!file.Init("Resource/ItemList.bin", "rb"))
		return;
#else
	if(!file.Init("Resource/ItemList.txt", "rt"))
		return;
#endif	// _FILE_BIN_

	while(1)
	{
		if(file.IsEOF())
			break;

		WORD ItemIdx = file.GetWord();
		if(m_ItemInfoList.GetData(ItemIdx) == NULL)
		{
			pInfo = new ITEM_INFO;
			SetItemInfo(ItemIdx,pInfo,&file);
			m_ItemInfoList.Add(pInfo, pInfo->ItemIdx);
		}
		else
		{
			ASSERTMSG(0, "LoadItemlist");
		}
	}
	file.Release();

#ifdef _FILE_BIN_
	if(!file.Init("Resource/HideItemLock.bin", "rb"))
		return;
#else
	if(!file.Init("Resource/HideItemLock.txt", "rt"))
		return;
#endif	// _FILE_BIN_


	while(1)
	{
		if(file.IsEOF())
			break;

		pInfo = NULL;
		WORD Index = file.GetWord();
		pInfo = m_ItemInfoList.GetData(Index);

		if(pInfo)
			m_UnfitItemInfoListForHide.Add(pInfo, pInfo->ItemIdx);
	}

	file.Release();
}

void CItemManager::ReloadItemList()
{
	ITEM_INFO * pInfo = NULL;
	CMHFile file;
	
#ifdef _FILE_BIN_
	if(!file.Init("Resource/ItemList.bin", "rb"))
		return;
#else
	if(!file.Init("Resource/ItemList.txt", "rt"))
		return;
#endif	// _FILE_BIN_

	while(pInfo = m_ItemInfoList.GetData())
		delete pInfo;
	m_ItemInfoList.RemoveAll();

	while(1)
	{
		if(file.IsEOF())
			break;

		WORD ItemIdx = file.GetWord();
		pInfo = m_ItemInfoList.GetData(ItemIdx);
		if(pInfo)
		{
			SetItemInfo(ItemIdx,pInfo,&file);
		}
		else
		{
			pInfo = new ITEM_INFO;
			SetItemInfo(ItemIdx,pInfo,&file);
			m_ItemInfoList.Add(pInfo, pInfo->ItemIdx);
		}
	}
	file.Release();
}
void CItemManager::LoadReinforceItemInfo()
{/*
	ITEM_REINFORCE_INFO * pInfo = NULL;
	CMHFile file;
#ifdef _FILE_BIN_
	if(!file.Init("Resource/ItemReinforceList.bin", "rb"))
		return ;
#else
	if(!file.Init("Resource/ItemReinforceList.txt", "rt"))
		return ;
#endif

	while(pInfo = m_ReinforceItemInfoList.GetData())
		delete pInfo;
	m_ReinforceItemInfoList.RemoveAll();

	while(1)
	{
		if(file.IsEOF())
			break;

		pInfo = new ITEM_REINFORCE_INFO;
		pInfo->wItemIdx = file.GetWord();
		file.GetString(pInfo->szItemName);
		pInfo->ItemLevel = file.GetWord();
		pInfo->ReinforceType = file.GetWord();
		pInfo->w = file.GetFloat();
		
		ASSERT(m_ReinforceItemInfoList.GetData(pInfo->wItemIdx) == FALSE);
		m_ReinforceItemInfoList.Add(pInfo, pInfo->wItemIdx);
	}
	file.Release();
	*/
}
void CItemManager::SetItemInfo(WORD ItemIdx,ITEM_INFO* pInfo,CMHFile* pFile)
{
	// ItemIdx￠￥A ⓒoI￠￢￠c A¨￠¨uiAa¨uⓒ￢CO
	pInfo->ItemIdx = ItemIdx;
	SafeStrCpy( pInfo->ItemName, pFile->GetString(), MAX_ITEMNAME_LENGTH+1 );
//	strcpy(pInfo->ItemName, pFile->GetString());
	pInfo->ItemTooltipIdx = pFile->GetWord();
	pInfo->Image2DNum = pFile->GetWord();	
	pInfo->ItemKind = pFile->GetWord();			
	pInfo->BuyPrice = pFile->GetDword();			
	pInfo->SellPrice = pFile->GetDword();			
	pInfo->Rarity = pFile->GetDword();			
	pInfo->WeaponType = pFile->GetWord();		
	pInfo->GenGol = pFile->GetWord();			
	pInfo->MinChub = pFile->GetWord();			
	pInfo->CheRyuk = pFile->GetWord();			
	pInfo->SimMek = pFile->GetWord();			
	pInfo->Life = pFile->GetDword();			
	pInfo->Shield = pFile->GetDword();				
	pInfo->NaeRyuk = pFile->GetWord();			
	pInfo->AttrRegist.SetElement_Val(ATTR_FIRE,pFile->GetFloat());
	pInfo->AttrRegist.SetElement_Val(ATTR_WATER,pFile->GetFloat());
	pInfo->AttrRegist.SetElement_Val(ATTR_TREE,pFile->GetFloat());
	pInfo->AttrRegist.SetElement_Val(ATTR_IRON,pFile->GetFloat());
	pInfo->AttrRegist.SetElement_Val(ATTR_EARTH,pFile->GetFloat());
	pInfo->LimitJob = pFile->GetWord();			
	pInfo->LimitGender = pFile->GetWord();		
	pInfo->LimitLevel = pFile->GetLevel();		
	pInfo->LimitGenGol = pFile->GetWord();		
	pInfo->LimitMinChub = pFile->GetWord();		
	pInfo->LimitCheRyuk = pFile->GetWord();		
	pInfo->LimitSimMek = pFile->GetWord();		
	pInfo->ItemGrade = pFile->GetWord();			
	pInfo->RangeType = pFile->GetWord();			
	pInfo->EquipKind = pFile->GetWord();			
	pInfo->Part3DType = pFile->GetWord();		
	pInfo->Part3DModelNum = pFile->GetWord();	
	pInfo->MeleeAttackMin = pFile->GetWord();	
	pInfo->MeleeAttackMax = pFile->GetWord();	
	pInfo->RangeAttackMin = pFile->GetWord();	
	pInfo->RangeAttackMax = pFile->GetWord();	
	pInfo->CriticalPercent = pFile->GetWord();	
	pInfo->AttrAttack.SetElement_Val(ATTR_FIRE,pFile->GetFloat());
	pInfo->AttrAttack.SetElement_Val(ATTR_WATER,pFile->GetFloat());
	pInfo->AttrAttack.SetElement_Val(ATTR_TREE,pFile->GetFloat());
	pInfo->AttrAttack.SetElement_Val(ATTR_IRON,pFile->GetFloat());
	pInfo->AttrAttack.SetElement_Val(ATTR_EARTH,pFile->GetFloat());
	pInfo->PhyDef = pFile->GetWord();			
	pInfo->Plus_MugongIdx = pFile->GetWord();	
	pInfo->Plus_Value = pFile->GetWord();		
	pInfo->AllPlus_Kind = pFile->GetWord();		
	pInfo->AllPlus_Value = pFile->GetWord();		
	pInfo->MugongNum = pFile->GetWord();			
	pInfo->MugongType = pFile->GetWord();		
	pInfo->LifeRecover = pFile->GetWord();		
	pInfo->LifeRecoverRate = pFile->GetFloat();
	pInfo->NaeRyukRecover = pFile->GetWord();	
	pInfo->NaeRyukRecoverRate = pFile->GetFloat();		
	pInfo->ItemType = pFile->GetWord();	
	
#ifdef _JAPAN_LOCAL_
	pInfo->wItemAttr = pFile->GetWord();
	pInfo->wAcquireSkillIdx1 = pFile->GetWord();
	pInfo->wAcquireSkillIdx2 = pFile->GetWord();
	pInfo->wDeleteSkillIdx = pFile->GetWord();
#endif

	pInfo->wSetItemKind = pFile->GetWord();	// 2007. 6. 7. CBH 세트이아탬 관련 추가
}

void CItemManager::NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg )
{	
	MSGBASE * pmsg = (MSGBASE *)pMsg;
    CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);

	if(pPlayer)
	if( BATTLESYSTEM->GetBattle(pPlayer)->GetBattleKind() == eBATTLE_KIND_SURYUN )
	{
		char buf[128];
		sprintf(buf, "수련장에서 아이템 메시지를 보냈습니다. character_Idx: %d, Protocol : %d", pmsg->dwObjectID, pmsg->Protocol);
		ASSERTMSG(0, buf);
		return;
	}

	switch(Protocol)
	{
	case MP_ITEM_MOVE_SYN:
		{
			MSG_ITEM_MOVE_SYN * pmsg = (MSG_ITEM_MOVE_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;
			int rt = 0;
			if(EI_TRUE == (rt = MoveItem(pPlayer, pmsg->wFromItemIdx, pmsg->FromPos, pmsg->wToItemIdx, pmsg->ToPos)))
			{
				MSG_ITEM_MOVE_ACK msg;
				memcpy(&msg, pmsg, sizeof(MSG_ITEM_MOVE_SYN));
				
				msg.Protocol = MP_ITEM_MOVE_ACK;

				SendAckMsg(pPlayer, &msg, sizeof(msg));
			}
			else if( rt != 99 )
			{
				MSG_ITEM_ERROR msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_ERROR_NACK;
				msg.dwObjectID = pPlayer->GetID();
				msg.ECode = eItemUseErr_Move;
				SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
			}
		}
		break;
	case MP_ITEM_COMBINE_SYN:
		{			
			MSG_ITEM_COMBINE_SYN * pmsg = (MSG_ITEM_COMBINE_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;
			int rt = 0;
			if(EI_TRUE == (rt = CombineItem(pPlayer, pmsg->wItemIdx, pmsg->FromPos, pmsg->ToPos, pmsg->FromDur, pmsg->ToDur)))
			{
				MSG_ITEM_COMBINE_ACK msg;
				memcpy(&msg, pmsg, sizeof(MSG_ITEM_COMBINE_SYN));
				msg.Protocol = MP_ITEM_COMBINE_ACK;
				SendAckMsg(pPlayer, &msg, sizeof(msg));
			}
			else
			{
				MSG_ITEM_ERROR msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_ERROR_NACK;
				msg.dwObjectID = pPlayer->GetID();
				msg.ECode = eItemUseErr_Combine;
				SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
			}
		}
		break;
	case MP_ITEM_DIVIDE_SYN:
		{
			
			MSG_ITEM_DIVIDE_SYN * pmsg = (MSG_ITEM_DIVIDE_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;
			int rt = 0;
			if(EI_TRUE == (rt = DivideItem(pPlayer, pmsg->wItemIdx, pmsg->FromPos, pmsg->ToPos, pmsg->FromDur, pmsg->ToDur)))
			{

			}
			else
			{
				MSG_ITEM_ERROR msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_ERROR_NACK;
				msg.dwObjectID = pPlayer->GetID();
				msg.ECode = eItemUseErr_Divide;
				SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
			}
			
		}
		break;
	case MP_ITEM_DISCARD_SYN:
		{
			MSG_ITEM_DISCARD_SYN * pmsg = (MSG_ITEM_DISCARD_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;

			CItemSlot * pSlot = pPlayer->GetSlot(pmsg->TargetPos);
			if(!pSlot) return;
			ITEMBASE Item = *pSlot->GetItemInfoAbs(pmsg->TargetPos);

			int rt = 0;
			if( LOOTINGMGR->IsLootedPlayer( pPlayer->GetID() ) )
			{
				rt = 10;
				MSG_ITEM_ERROR msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_ERROR_NACK;
				msg.dwObjectID = pPlayer->GetID();
				msg.ECode = eItemUseErr_Discard;
				SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
				break;
			}
		
			if(EI_TRUE == (rt = DiscardItem(pPlayer, pmsg->TargetPos, pmsg->wItemIdx, pmsg->ItemNum)))
			{				
				MSG_ITEM_DISCARD_ACK msg;
				memcpy(&msg, pmsg, sizeof(MSG_ITEM_DISCARD_SYN));
				msg.Protocol = MP_ITEM_DISCARD_ACK;
				SendAckMsg(pPlayer, &msg, sizeof(msg));

				// Log
				LogItemMoney(pmsg->dwObjectID, pPlayer->GetObjectName(), 0, "",
				eLog_ItemDiscard, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
				Item.wIconIdx, Item.dwDBIdx,  Item.Position, 0,
				Item.Durability, pPlayer->GetPlayerExpPoint());				
			}
			else
			{
				MSG_ITEM_ERROR msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_DISCARD_NACK;
				msg.dwObjectID = pPlayer->GetID();
				msg.ECode = rt;
				SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
			}
		}
		break;
	case MP_ITEM_BUY_SYN:
		{
			MSG_ITEM_BUY_SYN * pmsg = (MSG_ITEM_BUY_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;

			// checkhacknpc
			if( CheckHackNpc( pPlayer, pmsg->wDealerIdx ) == FALSE )
			{
				MSG_ITEM_ERROR msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_BUY_NACK;
				msg.dwObjectID = pPlayer->GetID();
				SendErrorMsg( pPlayer, &msg, sizeof(msg), NOT_EXIST );
				return;
			}

			//SW070626 보부상NPC
			if( FALSE == CheckDemandItem( pPlayer, pmsg ) )
			{
				MSG_ITEM_ERROR msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_BUY_NACK;
				msg.dwObjectID = pPlayer->GetID();
				SendErrorMsg(pPlayer, &msg, sizeof(msg), NO_DEMANDITEM );
				return;
			}


			int rt = 0;
			if(EI_TRUE == (rt = BuyItem(pPlayer, pmsg->wBuyItemIdx, pmsg->BuyItemNum, pmsg->wDealerIdx)))
			{
			}
			else
			{
				MSG_ITEM_ERROR msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_BUY_NACK;
				msg.dwObjectID = pPlayer->GetID();
				SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
			}
		}
		break;
	case MP_ITEM_SELL_SYN:
		{
			MSG_ITEM_SELL_SYN * pmsg = (MSG_ITEM_SELL_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;

			// checkhacknpc
			if( CheckHackNpc( pPlayer, pmsg->wDealerIdx ) == FALSE )
			{
				MSG_ITEM_ERROR msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SELL_NACK;
				msg.dwObjectID = pPlayer->GetID();
				SendErrorMsg( pPlayer, &msg, sizeof(msg), NOT_EXIST );
				return;
			}

			int rt = 0;
			if(EI_TRUE == (rt = SellItem(pPlayer, pmsg->TargetPos, pmsg->wSellItemIdx, pmsg->SellItemNum, pmsg->wDealerIdx)))
			{
				MSG_ITEM_SELL_ACK msg;
				memcpy(&msg, pmsg, sizeof(MSG_ITEM_SELL_SYN));
				msg.Protocol = MP_ITEM_SELL_ACK; 
				SendAckMsg(pPlayer, &msg, sizeof(msg));
			}
			else
			{
				MSG_ITEM_ERROR msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SELL_NACK;
				msg.dwObjectID = pPlayer->GetID();
				SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
			}

		}
		break;
	case MP_ITEM_USE_SYN:
		{
			MSG_ITEM_USE_SYN * pmsg = (MSG_ITEM_USE_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;
			
			BYTE rt = UseItem(pPlayer, pmsg->TargetPos, pmsg->wItemIdx);
			if(rt == eItemUseSuccess)
			{
				MSG_ITEM_USE_ACK msg;
				memcpy(&msg, pmsg, sizeof(MSG_ITEM_USE_SYN));
				msg.Protocol = MP_ITEM_USE_ACK;
				SendAckMsg(pPlayer, &msg, sizeof(msg));
			}
			else
			{
				MSG_ITEM_ERROR msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_USE_NACK;
				msg.dwObjectID = pPlayer->GetID();
				msg.ECode = rt;
				SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
			}
		}
		break;	

	case MP_ITEM_MIX_ADDITEM_SYN:
		{
			MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( pPlayer == NULL ) return;

			WORD wErrorCode = 0;
			ITEM_MIX_INFO* pMixItemInfo = NULL;
			CItemSlot * pSlot = NULL;
			MSG_ITEM msgAck;

			//AIº￥Aa¸® A×AIºiAC 'A¡AIAo
			if( GetTableIdxPosition( pmsg->ItemInfo.Position ) != eItemTable_Inventory )
			{
				wErrorCode = 1;
				goto go_MIX_ADDITEM_NACK;				
			}			
			//dº¸°¡ ¼­¹o¿I AIA¡CI´AAo
			if(!CHKRT->ItemOf(pPlayer, pmsg->ItemInfo.Position, pmsg->ItemInfo.wIconIdx, 0,0, CB_EXIST|CB_ICONIDX))
			{
				wErrorCode = 2;
				goto go_MIX_ADDITEM_NACK;
			}
			//¶oAI °E·A AO´AAo
			pSlot = pPlayer->GetSlot(eItemTable_Inventory);
			if( pSlot )
			if( pSlot->IsLock( pmsg->ItemInfo.Position ) )
			{
				wErrorCode = 3;
				goto go_MIX_ADDITEM_NACK;
			}
			// ¿E¼C ¾ÆAIAU ¾EμE
			if(ITEMMGR->IsOptionItem( pmsg->ItemInfo.wIconIdx, pmsg->ItemInfo.Durability ) )
			{
				wErrorCode = 4;
				goto go_MIX_ADDITEM_NACK;
			}

			//SW050920 Rare
//			if( ITEMMGR->IsRareOptionItem( pmsg->ItemInfo.wIconIdx, pmsg->ItemInfo.RareIdx ) )
//			{
//				wErrorCode = 5;
//				goto go_MIX_ADDITEM_NACK;
//			}

			// view list
			pMixItemInfo = ITEMMGR->GetMixItemInfo( pmsg->ItemInfo.wIconIdx );
			if(!pMixItemInfo)
			{
				wErrorCode = 5;
				goto go_MIX_ADDITEM_NACK;
			}

			//영약아이템이 아닌 재료아이템일경우 하나밖에 안올라간다.
			if( GetItemKind(pmsg->ItemInfo.wIconIdx) != eYOUNGYAK_ITEM &&
				GetItemKind(pmsg->ItemInfo.wIconIdx) != eEXTRA_ITEM_JEWEL &&
				pmsg->ItemInfo.Durability > 1 )
			{
				wErrorCode = 6;
				goto go_MIX_ADDITEM_NACK;
			}
			
			//////////Ae°uOK
			pSlot->SetLock( pmsg->ItemInfo.Position, TRUE );//¶o; °C´U.

			msgAck.Category		= MP_ITEM;
			msgAck.Protocol		= MP_ITEM_MIX_ADDITEM_ACK;
			msgAck.ItemInfo		= pmsg->ItemInfo;
			pPlayer->SendMsg( &msgAck, sizeof( msgAck ) );			
			break;

go_MIX_ADDITEM_NACK:
			MSG_WORD2 msgNack;
			msgNack.Category	= MP_ITEM;
			msgNack.Protocol	= MP_ITEM_MIX_ADDITEM_NACK;
			msgNack.wData1		= pmsg->ItemInfo.Position;
			msgNack.wData2		= wErrorCode;
			pPlayer->SendMsg( &msgNack, sizeof( msgNack ) );		
		}
		break;

	case MP_ITEM_MIX_RELEASEITEM:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( pPlayer == NULL ) return;
			
			CItemSlot* pSlot = pPlayer->GetSlot(pmsg->wData);
			if(pSlot)
				pSlot->SetLock(pmsg->wData, FALSE);
		}
		break;

	case MP_ITEM_MIX_SYN:
		{
			MSG_ITEM_MIX_SYN * pmsg = (MSG_ITEM_MIX_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;
			int rt = 0;
			if(EI_TRUE == (rt = MixItem(pPlayer, pmsg->wBasicItemIdx, pmsg->BasicItemPos, pmsg->ResultIndex, pmsg->Material, pmsg->wMaterialNum, pmsg->ShopItemIdx, pmsg->ShopItemPos)))
			{
				MSG_ITEM_MIX_ACK msg;
				memcpy(&msg, pmsg, sizeof(MSG_ITEM_MIX_SYN));
				msg.Protocol = MP_ITEM_MIX_SUCCESS_ACK;
				SendAckMsg(pPlayer, &msg, msg.GetSize());
			}
			else
			{
				switch(rt)
				{
				case 1000:
					{
						// ￠￥e¨oC¨¡¨￠
						MSG_ITEM_MIX_ACK msg;
						memcpy(&msg, pmsg, sizeof(MSG_ITEM_MIX_SYN));
						msg.Protocol = MP_ITEM_MIX_BIGFAILED_ACK;
						SendAckMsg(pPlayer, &msg, msg.GetSize());
					}
					break;
				case 1001:
					{
						// ¨oC¨¡¨￠
						MSG_ITEM_MIX_ACK msg;
						memcpy(&msg, pmsg, sizeof(MSG_ITEM_MIX_SYN));
						msg.Protocol = MP_ITEM_MIX_FAILED_ACK;
						SendAckMsg(pPlayer, &msg, msg.GetSize());
					}
					break;
				case 20:
				case 21:
				case 22:
				case 23:
					{
						MSG_DWORD2 msg;
						msg.Category = MP_ITEM;
						msg.Protocol = MP_ITEM_MIX_MSG;
						msg.dwObjectID = pPlayer->GetID();
						msg.dwData1 = rt;
						msg.dwData2 = pmsg->BasicItemPos;
						pPlayer->SendMsg( &msg, sizeof(msg) );
					}
					break;
				default:
					{
						if( rt != 2 )	//2번 ASSERT가 제일 많다. DB느려 서버와 클라이언트의 아이템정보가 꼬인다.
						{
							char temp[256];
							sprintf(temp,"아이템 조합 에러 %d",rt);
							ASSERTMSG(0, temp);
						}
						MSG_ITEM_ERROR msg;
						msg.Category = MP_ITEM;
						msg.Protocol = MP_ITEM_ERROR_NACK;
						msg.dwObjectID = pPlayer->GetID();
						msg.ECode = eItemUseErr_Mix;
						SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
					}
					break;
				}				
			}
		}
		break;
	// magi82 - Titan(070118)
	case MP_ITEM_TPM_ADDITEM_SYN:
		{
			MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( pPlayer == NULL ) return;

            TITANITEMMGR->TitanPartsMakeAddItem(pPlayer, pmsg);
		}
		break;

	case MP_ITEM_TPM_RELEASEITEM:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( pPlayer == NULL ) return;

			CItemSlot* pSlot = pPlayer->GetSlot(pmsg->wData);
			if(pSlot)
				pSlot->SetLock(pmsg->wData, FALSE);
		}
		break;
	case MP_ITEM_TPM_SYN:
		{
			MSG_ITEM_MIX_SYN * pmsg = (MSG_ITEM_MIX_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;
			
			TITANITEMMGR->TitanPartsMake(pPlayer, pmsg);
		}
		break;
	//////////////////////////////////////////////////////////////////////////
	// magi82 - Titan(070130)
	case MP_ITEM_TITANMIX_ADDITEM_SYN:
		{
			MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( pPlayer == NULL ) return;

			TITANITEMMGR->TitanMixAddItem( pPlayer, pmsg );
		}
		break;
	case MP_ITEM_TITANMIX_RELEASEITEM:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( pPlayer == NULL ) return;

			CItemSlot* pSlot = pPlayer->GetSlot(pmsg->wData);
			if(pSlot)
				pSlot->SetLock(pmsg->wData, FALSE);
		}
		break;
	case MP_ITEM_TITANMIX_SYN:
		{
			MSG_TITANMIX * pmsg = (MSG_TITANMIX *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
				return;

			for( int i = 0; i < 4; i++ )
			{
				if(!CHKRT->ItemOf(pPlayer, pmsg->iconInfo[i].itempos, (WORD)pmsg->iconInfo[i].dwItemIdx, 0,0, CB_EXIST|CB_ICONIDX))
					break;
			}

			TITAN_PARTS_KIND* pTitanPartsKind = TITANITEMMGR->GetTitanPartsKind(pmsg->iconInfo[0].dwItemIdx);
			ITEMBASE Item;
			memset( &Item, 0, sizeof(ITEMBASE) );
			CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);

			pPlayer->GetTitanManager()->SetTitanScale( pmsg->wTitanScale );

			// magi1127
			// 파츠들 삭제
			for( int i = 0; i < 4; i++ )
			{
				Item = *pSlot->GetItemInfoAbs(pmsg->iconInfo[i].itempos);
				if(EI_TRUE == DiscardItem(pPlayer, pmsg->iconInfo[i].itempos, (WORD)pmsg->iconInfo[i].dwItemIdx, 1))
				{
					POSTYPE TargetPos = 0;
					WORD wItemIdx = 0;
					DURTYPE ItemNum = 0;

					MSG_ITEM_DISCARD_ACK msg;
					msg.TargetPos = pmsg->iconInfo[i].itempos;
					msg.wItemIdx = (WORD)pmsg->iconInfo[i].dwItemIdx;
					msg.ItemNum = 1;
					msg.Category = MP_ITEM;
					msg.Protocol = MP_ITEM_TITANMIX_ACK_DELETEPARTS;
					SendAckMsg(pPlayer, &msg, sizeof(msg));

					// Log
					LogItemMoney(pmsg->dwObjectID, pPlayer->GetObjectName(), 0, "",
						eLog_ItemDiscard, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
						Item.wIconIdx, Item.dwDBIdx,  Item.Position, 0,
 						Item.Durability, pPlayer->GetPlayerExpPoint());
				}
			}

			// 아이템 생성
			WORD EmptyCellPos[255];
			WORD EmptyCellNum;
			pSlot = pPlayer->GetSlot(eItemTable_Inventory);
			WORD obtainItemNum = GetCanBuyNumInSpace( pPlayer, pSlot, (WORD)pTitanPartsKind->dwResultTitanIdx, 1, EmptyCellPos, EmptyCellNum );
			if(obtainItemNum == 0)
				break;


			ObtainItemEx( pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_TITANMIX_ACK, pPlayer->GetID(), 0, eLog_ItemObtainTitan, obtainItemNum, (DBResult)(ObtainItemDBResult)), (WORD)pTitanPartsKind->dwResultTitanIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum );
			
		}
		break;
		//////////////////////////////////////////////////////////////////////////
		// magi82 - Titan(070201)
	case MP_ITEM_TITANUPGRADE_SYN:
		{
			MSG_TITAN_UPGRADE_SYN* pmsg = (MSG_TITAN_UPGRADE_SYN*)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
				return;


			// 타이탄에 탑승중이면 업그레이트 불가
			if(pPlayer->GetTitanManager()->GetCurRidingTitan() != NULL)
			{
				break;
			}

			// 증서 유무 체크
			if(!CHKRT->ItemOf(pPlayer, pmsg->wBasicItemPos, pmsg->wBasicItemIdx, 0,0, CB_EXIST|CB_ICONIDX))
				break;

			// 각 재료 유무 체크
			for( int i = 0; i < pmsg->wMaterialNum; i++ )
			{
				if(!CHKRT->ItemOf(pPlayer, pmsg->Material[i].ItemPos, pmsg->Material[i].wItemIdx, pmsg->Material[i].Dur,0, CB_EXIST|CB_ICONIDX))
				{
					MSG_BYTE errorMsg;
					ZeroMemory(&errorMsg, sizeof(MSG_BYTE));
					SetProtocol(&errorMsg, MP_ITEM, MP_ITEM_TITANUPGRADE_NACK);
					errorMsg.bData = 1;
					pPlayer->SendMsg( &errorMsg, sizeof( errorMsg ) );
					break;
				}
			}

			TITAN_UPGRADEINFO* pTitanUpgrade = TITANITEMMGR->GetTitanUpgradeInfoItemInfo(pmsg->wBasicItemIdx);

			// 타이탄이 3단계면 더이상 업그레이드할일이 없음
			if(pTitanUpgrade->dwNextTitanIdx == 0)
			{
				MSG_BYTE errorMsg;
				ZeroMemory(&errorMsg, sizeof(MSG_BYTE));
				SetProtocol(&errorMsg, MP_ITEM, MP_ITEM_TITANUPGRADE_NACK);
				errorMsg.bData = 2;
				pPlayer->SendMsg( &errorMsg, sizeof( errorMsg ) );
				break;
			}

			// 돈 깍음
			if(pPlayer->GetMoney() < pTitanUpgrade->dwMoney)
			{
				MSG_BYTE errorMsg;
				ZeroMemory(&errorMsg, sizeof(MSG_BYTE));
				SetProtocol(&errorMsg, MP_ITEM, MP_ITEM_TITANUPGRADE_NACK);
				errorMsg.bData = 3;
				pPlayer->SendMsg( &errorMsg, sizeof( errorMsg ) );
				break;
			}
			pPlayer->SetMoney(pTitanUpgrade->dwMoney, MONEY_SUBTRACTION);

			ITEMBASE Item;
			memset( &Item, 0, sizeof(ITEMBASE) );
			CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);

			// magi1127
			// 파츠들 삭제
			for( int i = 0; i < pmsg->wMaterialNum; i++ )
			{
				Item = *pSlot->GetItemInfoAbs(pmsg->Material[i].ItemPos);
				if(EI_TRUE == DiscardItem(pPlayer, pmsg->Material[i].ItemPos, pmsg->Material[i].wItemIdx, pmsg->Material[i].Dur))
				{
					POSTYPE TargetPos = 0;
					WORD wItemIdx = 0;
					DURTYPE ItemNum = 0;

					MSG_ITEM_DISCARD_ACK msg;
					msg.TargetPos = pmsg->Material[i].ItemPos;
					msg.wItemIdx = pmsg->Material[i].wItemIdx;
					msg.ItemNum = pmsg->Material[i].Dur;
					msg.Category = MP_ITEM;
					msg.Protocol = MP_ITEM_TITANUPGRADE_ACK_DELETEMATERIAL;
					SendAckMsg(pPlayer, &msg, sizeof(msg));

					// Log
					LogItemMoney(pmsg->dwObjectID, pPlayer->GetObjectName(), 0, "",
						eLog_ItemDiscard, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
						Item.wIconIdx, Item.dwDBIdx,  Item.Position, 0,
						Item.Durability, pPlayer->GetPlayerExpPoint());
				}
			}

			/*
			// DB 타이탄 정보 갱신
			ITEMBASE* pBase = GetItemInfoAbsIn(pPlayer, pmsg->wBasicItemPos);
			if(!pBase)
			{
				ASSERT(0);
			}
			else
			{
				pPlayer->GetTitanManager()->UpgradeTitan(pBase->dwDBIdx);
			}*/

			// 원래 있던 타이탄 증서는 삭제
			Item = *pSlot->GetItemInfoAbs(pmsg->wBasicItemPos);
			if(EI_TRUE == DiscardItem(pPlayer, pmsg->wBasicItemPos, pmsg->wBasicItemIdx, 1))
			{
				pPlayer->GetTitanManager()->SetRegistTitanCallItemDBIdx(0);

				MSG_ITEM_DISCARD_ACK msg;
				msg.TargetPos = pmsg->wBasicItemPos;
				msg.wItemIdx = pmsg->wBasicItemIdx;
				msg.ItemNum = 1;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_TITANUPGRADE_ACK_DELETEMATERIAL;
				SendAckMsg(pPlayer, &msg, sizeof(msg));

				// Log
				LogItemMoney(pmsg->dwObjectID, pPlayer->GetObjectName(), 0, "",
					eLog_ItemDiscard, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
					Item.wIconIdx, Item.dwDBIdx,  Item.Position, 0,
					Item.Durability, pPlayer->GetPlayerExpPoint());
			}

			// 업그레이드된 타이탄 증서 생성
			WORD EmptyCellPos[255];
			WORD EmptyCellNum;
			pSlot = pPlayer->GetSlot(eItemTable_Inventory);
			WORD obtainItemNum = GetCanBuyNumInSpace( pPlayer, pSlot, (WORD)pTitanUpgrade->dwNextTitanIdx, 1, EmptyCellPos, EmptyCellNum );
			if(obtainItemNum == 0)
				break;

			ObtainItemEx( pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_TITANUPGRADE_ACK, pPlayer->GetID(), 0, eLog_ItemObtainTitan, obtainItemNum, (DBResult)(TitanUpgradeItemDBResult)), (WORD)pTitanUpgrade->dwNextTitanIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum );
		}
		break;
	case MP_ITEM_TITANUPGRADE_ADDITEM_SYN:
		{
			MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( pPlayer == NULL ) return;

			TITANITEMMGR->TitanupgradeAddItem( pPlayer, pmsg );
		}
		break;
	case MP_ITEM_TITANUPGRADE_RELEASEITEM:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( pPlayer == NULL ) return;

			CItemSlot* pSlot = pPlayer->GetSlot(pmsg->wData);
			if(pSlot)
				pSlot->SetLock(pmsg->wData, FALSE);
		}
		break;
		//////////////////////////////////////////////////////////////////////////
		// magi82 - Titan(070205)
	case MP_ITEM_TITANBREAK_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
				return;
			
			TITANITEMMGR->TitanBreak(pPlayer, pmsg);
		}
		break;
	case MP_ITEM_TITANBREAK_ADDITEM_SYN:
		{
			MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( pPlayer == NULL ) return;

			TITANITEMMGR->TitanBreakAddItem(pPlayer, pmsg);

		}
		break;
	case MP_ITEM_TITANBREAK_RELEASEITEM:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( pPlayer == NULL ) return;

			CItemSlot* pSlot = pPlayer->GetSlot(pmsg->wData);
			if(pSlot)
				pSlot->SetLock(pmsg->wData, FALSE);
		}
		break;
		//////////////////////////////////////////////////////////////////////////
		// magi82 - Titan(070514)
	case MP_ITEM_TITAN_EQUIP_REPAIR_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( pPlayer == NULL ) return;

			pPlayer->GetTitanManager()->SetTitanRepairEquipItem(pPlayer, pmsg);
		}
		break;
		//////////////////////////////////////////////////////////////////////////
		// magi82 - Titan(070619)
	case MP_ITEM_TITAN_EQUIP_REPAIR_TOTAL_SYN:
		{
			MSG_TITAN_REPAIR_TOTAL_EQUIPITEM_SYN* pmsg = (MSG_TITAN_REPAIR_TOTAL_EQUIPITEM_SYN*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( pPlayer == NULL ) return;

			pPlayer->GetTitanManager()->SetTitanRepairTotalEquipItem( pPlayer, pmsg );
		}
		break;
	case MP_ITEM_UPGRADE_SYN:
		{
			MSG_ITEM_UPGRADE_SYN * pmsg = (MSG_ITEM_UPGRADE_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;
			int rt = 0;
			if(EI_TRUE == (rt = UpgradeItem(pPlayer, pmsg->wItemIdx, pmsg->ItemPos, pmsg->wMaterialItemIdx, pmsg->MaterialItemPos)))
			{
				MSG_ITEM_UPGRADE_ACK msg;
				memcpy(&msg, pmsg, sizeof(MSG_ITEM_UPGRADE_SYN));
				msg.Protocol = MP_ITEM_UPGRADE_SUCCESS_ACK;
				SendAckMsg(pPlayer, &msg, sizeof(msg));

			}
			else
			{
				MSG_ITEM_ERROR msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_ERROR_NACK;
				msg.dwObjectID = pPlayer->GetID();
				msg.ECode = eItemUseErr_Upgrade;
				SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
			}
		}
		break;
	case MP_ITEM_REINFORCE_SYN:
		{
			MSG_ITEM_REINFORCE_SYN* pmsg = (MSG_ITEM_REINFORCE_SYN*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser( pmsg->dwObjectID );
			if(pPlayer == NULL)
				return;

			int rt = 0;
			if(EI_TRUE == (rt = ReinforceItem( pPlayer, pmsg->wTargetItemIdx, pmsg->TargetPos, pmsg->JewelWhich, pmsg->wJewelUnit )))
			{

			}
			else
			{
				switch(rt)
				{
				case 99:
					{
						// ¨oC¨¡¨￠
						pmsg->Protocol = MP_ITEM_REINFORCE_FAILED_ACK;
						SendAckMsg(pPlayer, pmsg, pmsg->GetSize());
					}
					break;
				default:
					{
						MSG_ITEM_ERROR msg;
						msg.Category = MP_ITEM;
						msg.Protocol = MP_ITEM_REINFORCE_NACK;
						msg.dwObjectID = pPlayer->GetID();
						SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
					}
					break;
				}

			}

		}
		break;
	case MP_ITEM_REINFORCE_WITHSHOPITEM_SYN:
		{
			MSG_ITEM_REINFORCE_WITHSHOPITEM_SYN* pmsg = (MSG_ITEM_REINFORCE_WITHSHOPITEM_SYN*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
				return;

			int rt = 0;
			if(EI_TRUE == (rt = ReinforceItemWithShopItem( pPlayer, pmsg->wTargetItemIdx, pmsg->TargetPos, pmsg->wShopItemIdx, pmsg->ShopItemPos, pmsg->JewelWhich, pmsg->wJewelUnit )))
			{

			}
			else
			{
				switch(rt)
				{
				case 99:
					{
						// ¨oC¨¡¨￠
						pmsg->Protocol = MP_ITEM_REINFORCE_FAILED_ACK;
						SendAckMsg(pPlayer, pmsg, pmsg->GetSize());
					}
					break;
				default:
					{
						MSG_ITEM_ERROR msg;
						msg.Category = MP_ITEM;
						msg.Protocol = MP_ITEM_REINFORCE_WITHSHOPITEM_NACK;
						msg.dwObjectID = pPlayer->GetID();
						SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
					}
					break;
				}

			}			
		}
		break;
	case MP_ITEM_DEALER_SYN:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
				return;

			// checkhacknpc
			if( CheckHackNpc( pPlayer, pmsg->wData ) == FALSE )
				return;

			// AO¨oA
			MSG_WORD msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_DEALER_ACK;
			msg.dwObjectID = pmsg->dwObjectID;
			msg.wData = pmsg->wData;
			pPlayer->SendMsg(&msg,sizeof(msg));

		}
		break;
	//////////////////////////////////////////////////////////////////////////
	//ⓒo￠c¨¡A		
	case MP_ITEM_GUILD_WAREHOUSE_INFO_SYN:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
			CPlayer* pPlayer  = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)
				return;
			GUILDMGR->GuildWarehouseInfo(pPlayer, pmsg->bData);
		}
		break;	
	case MP_ITEM_GUILD_MOVE_SYN:
		{
			MSG_ITEM_MOVE_SYN * pmsg = (MSG_ITEM_MOVE_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;
			
			if(!CanMovetoGuildWare(pmsg->FromPos, pmsg->ToPos, pPlayer))
			{				
				ASSERT(0); //A¡þ￠OoAI¨uⓒ￡¨¡￠c￠?￠®¨u¡ⓒ Aⓒø￠￢￠c Cⓒ￢￠￥A￥i¡I.
				SendGuildErrorMsg(pPlayer, MP_ITEM_GUILD_MOVE_NACK, 4);
				return;
			}
			int rt = 0;
			if(EI_TRUE == (rt = MoveItem(pPlayer, pmsg->wFromItemIdx, pmsg->FromPos, pmsg->wToItemIdx, pmsg->ToPos)))
			{
			//	AddGuildItemOption(pmsg->FromPos, pmsg->ToPos, pPlayer);
				MSG_ITEM_MOVE_ACK msg;
				memcpy(&msg, pmsg, sizeof(MSG_ITEM_MOVE_SYN));
				msg.Protocol = MP_ITEM_GUILD_MOVE_ACK;
				SendAckMsg(pPlayer, &msg, sizeof(msg));
			}
			else
			{
				SendGuildErrorMsg(pPlayer, MP_ITEM_GUILD_MOVE_NACK, rt);
			}
		}
		break;
	case MP_ITEM_GUILD_WAREHOUSE_LEAVE:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
			CPlayer* pPlayer  = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)
				return;

			GUILDMGR->LeaveWareHouse(pPlayer, pmsg->bData);
		}
		break;
	//////////////////////////////////////////////////////////////////////////
	//C¡I¡¾ⓒo
	case MP_ITEM_PYOGUK_ITEM_INFO_SYN:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);

			if( !pPlayer )	return;

			if( CheckHackNpc( pPlayer, pmsg->wData1, pmsg->wData2 ) == FALSE )
				return;
			//SW 강화/레어 정보 받고 베이스 정보 받는다. 순서의 이유는?

			if( pPlayer->IsGotWarehouseItems() == TRUE )	//아이템을 받는중이거나 이미 받았다.
				return;

			pPlayer->SetGotWarehouseItems( TRUE );
			PyogukItemOptionInfo(pPlayer->GetID(), pPlayer->GetUserID(), 0);
			//PyogukItemRareOptionInfo(pPlayer->GetID(), pPlayer->GetUserID(), 0);
		}
		break;
	case MP_ITEM_PETINVEN_INFO_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);

			if( !pPlayer )	return;

			//!!!펫 소환중인지 체크
			if(!pPlayer->GetPetManager()->GetCurSummonPet())	return;

			PetInvenItemOptionInfo(pPlayer->GetID(), pPlayer->GetUserID(), TP_PETINVEN_START, TP_PETINVEN_END);
		}
		break;

	case MP_ITEM_USE_FOR_QUESTSTART_SYN:
		{
			MSG_ITEM_USE_SYN * pmsg = (MSG_ITEM_USE_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;

			BYTE rt = UseItem(pPlayer, pmsg->TargetPos, pmsg->wItemIdx);
			if(rt == eItemUseSuccess)
			{
				MSG_ITEM_USE_ACK msg;
				memcpy(&msg, pmsg, sizeof(MSG_ITEM_USE_SYN));
				msg.Protocol = MP_ITEM_USE_ACK;
				SendAckMsg(pPlayer, &msg, sizeof(msg));
			}
			else
			{
				MSG_ITEM_ERROR msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_USE_NACK;
				msg.dwObjectID = pPlayer->GetID();
				msg.ECode = eItemUseErr_Quest;
				SendErrorMsg(pPlayer, &msg, sizeof(msg), eItemUseErr_Quest);
			}
		}
		break;
		
	case MP_ITEM_DISSOLUTION_SYN:
		{
			MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer )	return;

			MSGBASE msg;
			msg.Category = MP_ITEM;
			
			if( DISSOLUTIONMGR->ItemDissollution( pPlayer, &pmsg->ItemInfo ) == 0 )
			{
				msg.Protocol = MP_ITEM_DISSOLUTION_ACK;
			}
			else
			{
				msg.Protocol = MP_ITEM_DISSOLUTION_NACK;
			}

			pPlayer->SendMsg( &msg, sizeof(msg) );
		}
		break;
	case MP_ITEM_USE_CHANGEITEM_SYN:	// event item
		{
			MSG_ITEM_USE_SYN* pmsg = (MSG_ITEM_USE_SYN*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( pPlayer == NULL ) return;
			
			int rt = CHANGEITEMMGR->UseChangeItem( pPlayer, pmsg->TargetPos, pmsg->wItemIdx );
			if( rt == 0 )		// not use
			{
				MSG_ITEM_ERROR msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_USE_NACK;
				msg.dwObjectID = pPlayer->GetID();
				msg.ECode = rt;
				SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
			}
		}
		break;

	case MP_ITEM_SHOPITEM_INFO_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);

			if( !pPlayer )
				return;

			pPlayer->SetShopItemInit( FALSE );
			CharacterShopItemInfo(pPlayer->GetID(), 0);
		}
		break;

	case MP_ITEM_SHOPITEM_USE_SYN:
		{
			MSG_SHOPITEM_USE_SYN * pmsg = (MSG_SHOPITEM_USE_SYN *)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;


			SHOPITEMBASE BaseInfo;
			BYTE rt = UseShopItem(pPlayer, pmsg->UseBaseInfo, &BaseInfo);
			if(rt == eItemUseSuccess)
			{
				SEND_SHOPITEM_BASEINFO msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_USE_ACK;
				msg.ShopItemBase = BaseInfo;
				msg.ShopItemPos = pmsg->UseBaseInfo.ShopItemPos;
				msg.ShopItemIdx = pmsg->UseBaseInfo.ShopItemIdx;
				SendAckMsg(pPlayer, &msg, sizeof(msg));
			}
			else
			{
				MSG_ITEM_ERROR msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_USE_NACK;
				msg.ECode = rt;
				SendErrorMsg(pPlayer, &msg, sizeof(msg), rt);
			}
		}
		break;
		
	case MP_ITEM_SHOPITEM_UNSEAL_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;

			MSG_DWORD msg;
			msg.Category = MP_ITEM;
				msg.dwData = pmsg->dwData;

			if( ITEMMGR->ItemUnsealing(pPlayer, (POSTYPE)pmsg->dwData) )
			{
				msg.Protocol = MP_ITEM_SHOPITEM_UNSEAL_ACK;
			}
			else
			{
				msg.Protocol = MP_ITEM_SHOPITEM_UNSEAL_NACK;
			}

			pPlayer->SendMsg( &msg, sizeof(msg) );
		}
		break;
		
	case MP_ITEM_SHOPITEM_CHANGEMAP_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;
		}
		break;

	case MP_ITEM_SHOPITEM_MPINFO:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;

			SavedMovePointInfo( pPlayer->GetID() );
		}
		break;
	case MP_ITEM_SHOPITEM_SAVEPOINT_ADD_SYN:
		{
			SEND_MOVEDATA_WITHITEM* pmsg = (SEND_MOVEDATA_WITHITEM*)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;

			DWORD Validsavenum = MAX_MOVEDATA_PERPAGE;
			if( pPlayer->GetShopItemManager()->GetUsingItemInfo( eIncantation_MemoryMoveExtend ) ||
				pPlayer->GetShopItemManager()->GetUsingItemInfo( eIncantation_MemoryMoveExtend7 ) ||
				pPlayer->GetShopItemManager()->GetUsingItemInfo( eIncantation_MemoryMove2 ) ||
				pPlayer->GetShopItemManager()->GetUsingItemInfo( eIncantation_MemoryMoveExtend30 ) )
				Validsavenum = MAX_MOVEDATA_PERPAGE*MAX_MOVEPOINT_PAGE;

			BYTE rt;
			SHOPITEMBASE ReturnItem;

			if( pPlayer->GetShopItemManager()->GetSavedMPNum() >= Validsavenum )
				goto SAVEPOINT_ADD_FAILED;
			
			rt = UseShopItem( pPlayer, pmsg->Item, &ReturnItem );			
			if( rt==eItemUseSuccess )
			{
				SEND_SHOPITEM_BASEINFO msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_USE_ACK;
				msg.ShopItemBase = ReturnItem;
				msg.ShopItemPos = pmsg->Item.ShopItemPos;
				msg.ShopItemIdx = pmsg->Item.ShopItemIdx;
				pPlayer->SendMsg( &msg, sizeof(msg) );
					
				SavedMovePointInsert( pPlayer->GetID(), pmsg->Data.Name, pmsg->Data.MapNum, pmsg->Data.Point.value );				
			}
			else
			{
SAVEPOINT_ADD_FAILED:
				MSG_ITEM_ERROR msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_USE_NACK;
				msg.ECode = rt;
				pPlayer->SendMsg( &msg, sizeof(msg) );				
			}
		}
		break;
		
	case MP_ITEM_SHOPITEM_SAVEPOINT_UPDATE_SYN:
		{
			SEND_MOVEDATA_SIMPLE* pmsg = (SEND_MOVEDATA_SIMPLE*)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;

			if( pPlayer->GetShopItemManager()->ReNameMovePoint( pmsg->Data.DBIdx, pmsg->Data.Name ) )
			{
				pmsg->Protocol = MP_ITEM_SHOPITEM_SAVEPOINT_UPDATE_ACK;		

				SavedMovePointUpdate( pPlayer->GetID(), pmsg->Data.DBIdx, pmsg->Data.Name );				
			}
			else
			{
				pmsg->Protocol = MP_ITEM_SHOPITEM_SAVEPOINT_UPDATE_NACK;
			}

			pPlayer->SendMsg(pmsg, sizeof(*pmsg));
		}
		break;

	case MP_ITEM_SHOPITEM_SAVEPOINT_DEL_SYN:
		{
			SEND_MOVEDATA_SIMPLE* pmsg = (SEND_MOVEDATA_SIMPLE*)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;

			if( pPlayer->GetShopItemManager()->DeleteMovePoint( pmsg->Data.DBIdx ) )
			{
				pmsg->Protocol = MP_ITEM_SHOPITEM_SAVEPOINT_DEL_ACK;

				SavedMovePointDelete( pPlayer->GetID(), pmsg->Data.DBIdx );
			}
			else
			{
				pmsg->Protocol = MP_ITEM_SHOPITEM_SAVEPOINT_DEL_NACK;
			}

			pPlayer->SendMsg(pmsg, sizeof(*pmsg));
		}
		break;

	case MP_ITEM_SHOPITEM_REVIVEOTHER_SYN:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL) return;

			// 부활 시킬 상대방  RevivePlayer
			CPlayer * pTargetPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwData1);
			if( pTargetPlayer == NULL ||
				pTargetPlayer->GetState() != eObjectState_Die )
			{
				MSG_DWORD msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_NACK;
				msg.dwData = eShopItemErr_Revive_NotDead;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}

			//공성전에서는 부활 안되게..
			if( pPlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
			{
				if( pPlayer->GetBattleTeam() == 2 ) //관람모드
				{
					ITEM_INFO * pItemInfo = GetItemInfo((WORD)pmsg->dwData2);
					if( pItemInfo && ( pItemInfo->ItemKind & eSHOP_ITEM ) )
					{
						if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION && pItemInfo->LimitLevel )
						{
							MSG_DWORD msg;
							msg.Category = MP_ITEM;
							msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_NACK;
							msg.dwData = eShopItemErr_Revive_NotReady;
							pPlayer->SendMsg( &msg, sizeof(msg) );
							return;
						}
					}
				}
			}

			if( IsUseAbleShopItem( pPlayer, (WORD)pmsg->dwData2, (POSTYPE)pmsg->dwData3 ) )
			{
				SEND_REVIVEOTHER msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_SYN;
				//strcpy( msg.TargetName, pPlayer->GetObjectName() );
				SafeStrCpy( msg.TargetName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1 );
				msg.TargetID = pPlayer->GetID();
				pTargetPlayer->SendMsg( &msg, sizeof(msg) );

				pPlayer->SetReviveData( pmsg->dwData1, (WORD)pmsg->dwData2, (POSTYPE)pmsg->dwData3 );
				pPlayer->SetReviveTime( 60000 );
			}
			else
			{
				MSG_DWORD msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_NACK;
				msg.dwData = eShopItemErr_Revive_NotUse;
				pPlayer->SendMsg( &msg, sizeof(msg) );
			}
		}
		break;
	case MP_ITEM_SHOPITEM_REVIVEOTHER_ACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			// 살아나야할 플레이어   RevivePlayer
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)				return;

			// 아이템을 사용한 상대   ItemUsePlayer
			CPlayer * pTargetPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwData1);
			if(pTargetPlayer == NULL)		return;

			if( pPlayer->GetState() != eObjectState_Die )
			{
				MSG_WORD msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_NACK;
				msg.wData = eShopItemErr_Revive_NotDead;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				pTargetPlayer->SendMsg( &msg, sizeof(msg) );

				pTargetPlayer->SetReviveData( 0, 0, 0 );
				pTargetPlayer->SetReviveTime( 0 );
				return;
			}

			REVIVEDATA* pData = pTargetPlayer->GetReviveData();

			if( IsUseAbleShopItem( pTargetPlayer, pData->ItemIdx, pData->ItemPos ) == FALSE )
			{
				MSG_WORD msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_NACK;
				msg.wData = eShopItemErr_Revive_NotUse;
				pTargetPlayer->SendMsg( &msg, sizeof(msg) );

				//
				msg.wData = eShopItemErr_Revive_Fail;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				
				pTargetPlayer->SetReviveData( 0, 0, 0 );
				pTargetPlayer->SetReviveTime( 0 );
				return;
			}


			SHOPITEMUSEBASE ItemUseBase;
			ItemUseBase.ShopItemIdx = pData->ItemIdx;
			ItemUseBase.ShopItemPos = pData->ItemPos;
			SHOPITEMBASE ItemBase;

			ITEM_INFO* pItemInfo = GetItemInfo( pData->ItemIdx );
			if( !pItemInfo )
				goto Revive_Failed;
			if( pItemInfo->ItemKind != eSHOP_ITEM_INCANTATION || !pItemInfo->LimitLevel )
				goto Revive_Failed;

			if( pPlayer->IsAbleReviveOther() )
			{
				SHOPITEMWITHTIME* pShopItem = pTargetPlayer->GetShopItemManager()->GetUsingItemInfo( pData->ItemIdx );
				if( !pShopItem )
				{
					// 기간제
					if( pItemInfo->SellPrice )
						goto Revive_Failed;
					if( EI_TRUE != DiscardItem( pTargetPlayer, ItemUseBase.ShopItemPos, ItemUseBase.ShopItemIdx, 1 ) )
						goto Revive_Failed;
				}

				pPlayer->ReviveShopItem( pData->ItemIdx );
				
				SEND_SHOPITEM_BASEINFO msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_USE_ACK;
				msg.ShopItemBase = ItemBase;
				msg.ShopItemIdx = pData->ItemIdx;
				msg.ShopItemPos = pData->ItemPos;				
				pTargetPlayer->SendMsg(&msg, sizeof(msg));
				
				MSG_DWORD msgb;
				msgb.Category = MP_ITEM;
				msgb.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_ACK;
				msgb.dwData = pPlayer->GetID();
				pTargetPlayer->SendMsg( &msgb, sizeof(msgb) );
				pPlayer->SendMsg( &msgb, sizeof(msgb) );
			}
			else
			{
Revive_Failed:
				MSG_WORD msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_NACK;
				msg.wData = eShopItemErr_Revive_Fail;
				pTargetPlayer->SendMsg( &msg, sizeof(msg) );
				pPlayer->SendMsg( &msg, sizeof(msg) );
			}

			pTargetPlayer->SetReviveData( 0, 0, 0 );
			pTargetPlayer->SetReviveTime( 0 );
		}
		break;
	case MP_ITEM_SHOPITEM_REVIVEOTHER_NACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)				return;

			// 아이템을 사용한 상대		ItemUsePlayer
			CPlayer * pTargetPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwData1);
			if(pTargetPlayer == NULL)		return;

			MSG_DWORD msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_REVIVEOTHER_NACK;
			msg.dwData = pmsg->dwData2;
			pTargetPlayer->SendMsg( &msg, sizeof(msg) );
			
			pTargetPlayer->SetReviveData( 0, 0, 0 );
			pTargetPlayer->SetReviveTime( 0 );
		}
		break;
	case MP_ITEM_SHOPITEM_AVATAR_TAKEOFF:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)				return;

			if( IsUseAbleShopItem( pPlayer, pmsg->wData1, pmsg->wData2 ) )			
			{
				if( pPlayer->GetShopItemManager()->TakeOffAvatarItem( pmsg->wData1, pmsg->wData2 ) == FALSE )
				{
					pmsg->Protocol = MP_ITEM_SHOPITEM_AVATAR_USE_NACK;
					pPlayer->SendMsg( pmsg, sizeof(*pmsg) );
				}
			}
			else
			{
				pmsg->Protocol = MP_ITEM_SHOPITEM_AVATAR_USE_NACK;			
				pPlayer->SendMsg( pmsg, sizeof(*pmsg) );
			}
		}
		break;
	case MP_ITEM_SHOPITEM_AVATAR_CHANGE:
		{
			MSG_WORD2*pmsg = (MSG_WORD2*)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)				return;

			pPlayer->GetShopItemManager()->CalcShopItemOption( pmsg->wData2, TRUE );

			MSG_DWORD2 msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_PUTON;
			msg.dwData1 = pPlayer->GetID();
			msg.dwData2 = pmsg->wData2;
			PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer, &msg, sizeof(msg));			
		}
		break;
	case MP_ITEM_SHOPITEM_AVATAR_USE_SYN:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)				return;
			
			if( pPlayer->GetState() != eObjectState_None && pPlayer->GetState() != eObjectState_Immortal )
				goto _Avataruse_failed;
			
			//사용한 아이탬이 인벤에 들어있는지 체크
			if( !IsUseAbleShopItem( pPlayer, pmsg->wData1, pmsg->wData2 ) )
				goto _Avataruse_failed;


			const ITEMBASE* pItembase = GetItemInfoAbsIn( pPlayer, pmsg->wData2 );
			if( !pItembase )
				goto _Avataruse_failed;

			//2007. 10. 15. CBH - 무기 장비 슬롯에 아이탬이 없으면 실패.
			if(CheckWeaponToShopItem(pPlayer,  pmsg->wData1) == FALSE)
			{
				goto _Avataruse_failed;
			}
/*
			SHOPITEMWITHTIME* pItem = pPlayer->GetShopItemManager()->GetUsingItemInfo( pmsg->wData1 );
			if( pItem && pItem->ShopItem.ItemBase.dwDBIdx == pItembase->dwDBIdx )
			{
				if( pPlayer->GetShopItemManager()->PutOnAvatarItem( pmsg->wData1, pmsg->wData2 ) )
				{
					pmsg->Protocol = MP_ITEM_SHOPITEM_AVATAR_USE_ACK;
					pPlayer->SendMsg( pmsg, sizeof(*pmsg) );
					return;
				}
				else
					goto _Avataruse_failed;
			}
			else
			{

				IsUseAbleShopAvatarItem( pPlayer->GetID(), pItembase->dwDBIdx, pItembase->wIconIdx, pItembase->Position );
				return;
			}
*/
			
			SHOPITEMWITHTIME* pItem = pPlayer->GetShopItemManager()->GetUsingItemInfo( pmsg->wData1 );
			if( pItem )
			{
				if( pItem->ShopItem.ItemBase.dwDBIdx == pItembase->dwDBIdx )
				{
					if( pPlayer->GetShopItemManager()->PutOnAvatarItem( pmsg->wData1, pmsg->wData2 ) )
					{
						pmsg->Protocol = MP_ITEM_SHOPITEM_AVATAR_USE_ACK;
						pPlayer->SendMsg( pmsg, sizeof(*pmsg) );
						return;
					}
					else
						goto _Avataruse_failed;
				}
				else
				{
					goto _Avataruse_failed;
				}
			}
			else
			{
				IsUseAbleShopAvatarItem( pPlayer->GetID(), pItembase->dwDBIdx, pItembase->wIconIdx, pItembase->Position );
				return;
			}
			

_Avataruse_failed:
			pmsg->Protocol = MP_ITEM_SHOPITEM_AVATAR_USE_NACK;
			pPlayer->SendMsg( pmsg, sizeof(*pmsg) );
		}
		break;
	case MP_ITEM_SHOPITEM_CHASEUSE_SYN:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			CPlayer * pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)				return;

			MSGBASE msg;
			msg.Category = MP_ITEM;			

			if( pPlayer->GetShopItemManager()->GetUsingItemInfo( pmsg->wData1 ) )
				msg.Protocol = MP_ITEM_SHOPITEM_CHASEUSE_ACK;
			else
				msg.Protocol = MP_ITEM_SHOPITEM_CHASEUSE_NACK;

			pPlayer->SendMsg( &msg, sizeof(msg) );
		}
		break;
	case MP_ITEM_SHOPITEM_CHASE_SYN:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData2 );
			if( !pPlayer )
			{
				MSG_DWORD msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_CHASE_NACK;
				msg.dwData = pmsg->dwData1;

				g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );
				return;
			}

			if( pmsg->dwData3 == eIncantation_Tracking ||
				pmsg->dwData3 == eIncantation_Tracking7 ||
				pmsg->dwData3 == eIncantation_Tracking7_NoTrade )
			{
				SEND_CHASEINFO msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_CHASE_ACK;
				msg.dwObjectID = pPlayer->GetID();

				SafeStrCpy( msg.WandtedName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1 );
				VECTOR3 pos;
				pPlayer->GetPosition( &pos );
				msg.Pos.Compress( &pos );
				msg.MapNum = GAMERESRCMNGR->GetLoadMapNum();
				if( pPlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_SURYUN )
					msg.EventMapNum = 44;
				else
					msg.EventMapNum = (MAPTYPE)pPlayer->GetChannelID();
				msg.CharacterIdx = pmsg->dwData1;
				g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );

				//
				MSGBASE msg1;
				msg1.Category = MP_ITEM;
				msg1.Protocol = MP_ITEM_SHOPITEM_CHASE_TRACKING;
				pPlayer->SendMsg( &msg1, sizeof(msg1) );
			}
		}
		break;
	case MP_ITEM_SHOPITEM_NCHANGE_SYN:
		{
			SEND_CHANGENAMEBASE* pmsg = (SEND_CHANGENAMEBASE*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer )			return;

			const ITEMBASE* pItemBase = NULL;
			// Item Position 확인
			for(int i=0; i<SLOT_SHOPINVEN_NUM/2; i++)
			{
				pItemBase = GetItemInfoAbsIn( pPlayer, i+TP_SHOPINVEN_START );
				if( !pItemBase )	continue;

				if( pItemBase->dwDBIdx == pmsg->DBIdx )
					break;
			}

			if( i >= SLOT_SHOPINVEN_NUM/2)
			{
				MSG_DWORD msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_NCHANGE_NACK;
				msg.dwData = 6;
				pPlayer->SendMsg( &msg, sizeof(msg) );	
				return;
			}

			CharacterChangeName( pmsg->dwObjectID, pmsg->Name, pmsg->DBIdx );
		}
		break;
	case MP_ITEM_SHOPITEM_CHARCHANGE_SYN:
		{
			SEND_CHARACTERCHANGE_INFO* pmsg = (SEND_CHARACTERCHANGE_INFO*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer )			return;

			MSG_DWORD msg;
			SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_CHARCHANGE_NACK );
			CHARACTER_TOTALINFO CTInfo;
			pPlayer->GetCharacterTotalInfo( &CTInfo );

			for(int i=0; i<eAvatar_Effect; ++i)
			{
				if( pPlayer->GetShopItemStats()->Avatar[i] )
				{
					msg.dwData = 6;
					pPlayer->SendMsg( &msg, sizeof(msg) );
					return;
				}
			}

			ITEMBASE* pItem = (ITEMBASE*)GetItemInfoAbsIn( pPlayer, (POSTYPE)pmsg->Param );
			if( !pItem || (pItem->wIconIdx != eIncantation_CharChange && pItem->wIconIdx != eIncantation_ShapeChange) )
			{
				msg.dwData = 1;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}
			if( pmsg->Info.Height > 1.1f || pmsg->Info.Height < 0.9f ||
				pmsg->Info.Width > 1.1f || pmsg->Info.Width < 0.9f )
			{
				msg.dwData = 2;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}
			if( pmsg->Info.Gender > 2 )
			{
				msg.dwData = 3;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}
			if( pmsg->Info.HairType > 4 || pmsg->Info.FaceType > 4 )
			{
				msg.dwData = 4;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}

			// 
			if( EI_TRUE != DiscardItem( pPlayer, (POSTYPE)pmsg->Param, pItem->wIconIdx, 1 ) )
			{
				msg.dwData = 5;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}
			else
			{
				pPlayer->SetCharChangeInfo( &pmsg->Info );

				SEND_SHOPITEM_BASEINFO msg;
				SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_USE_ACK );
				msg.ShopItemPos = (POSTYPE)pmsg->Param;
				msg.ShopItemIdx = pItem->wIconIdx;
				SendAckMsg( pPlayer, &msg, sizeof(msg) );

				//
				SEND_CHARACTERCHANGE_INFO msg1;
				SetProtocol( &msg1, MP_ITEM, MP_ITEM_SHOPITEM_CHARCHANGE );
				msg1.Param = pPlayer->GetID();
				memcpy( &msg1.Info, &pmsg->Info, sizeof(CHARACTERCHANGE_INFO) );
				if( pItem->wIconIdx == eIncantation_ShapeChange )
				{
					msg1.Info.Gender = CTInfo.Gender;
					msg1.Info.Height = CTInfo.Height;
					msg1.Info.Width = CTInfo.Width;
				}									
				PACKEDDATA_OBJ->QuickSend( pPlayer, &msg1, sizeof(msg1) );				
			}

			// To DB
			if( pItem->wIconIdx == eIncantation_ShapeChange )
				CharacterChangeInfoToDB( pPlayer->GetID(), CTInfo.Gender,
					pmsg->Info.HairType, pmsg->Info.FaceType, CTInfo.Height, CTInfo.Width );
			else			
				CharacterChangeInfoToDB( pPlayer->GetID(), pmsg->Info.Gender, pmsg->Info.HairType, pmsg->Info.FaceType,
					pmsg->Info.Height, pmsg->Info.Width );
			
			MSGBASE msg1;
			SetProtocol( &msg1, MP_ITEM, MP_ITEM_SHOPITEM_CHARCHANGE_ACK );
			pPlayer->SendMsg( &msg1, sizeof(msg1) );

			// Log추가
			LogItemMoney( pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
				eLog_ShopItemUse, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
				pItem->wIconIdx, pItem->dwDBIdx, pItem->Position, 0, pItem->Durability, pPlayer->GetPlayerExpPoint());
		}
		break;
	case MP_ITEM_SHOPITEM_SEAL_SYN:
		{
			MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer )			return;

			MSG_DWORD msg;
			SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_SEAL_NACK );

			if( !IsUseAbleShopItem( pPlayer, (WORD)pmsg->dwData1, (POSTYPE)pmsg->dwData2 ) )
			{
				msg.dwData = 1;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}
			if( !IsUseAbleShopItem( pPlayer, (WORD)pmsg->dwData3, (POSTYPE)pmsg->dwData4 ) )
			{
				msg.dwData = 2;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}

			ITEMBASE* pSealItem = (ITEMBASE*)GetItemInfoAbsIn( pPlayer, (POSTYPE)pmsg->dwData2 );
			ITEMBASE* pTargetItem = (ITEMBASE*)GetItemInfoAbsIn( pPlayer, (POSTYPE)pmsg->dwData4 );
			//SHOPITEMWITHTIME* pAvatarItem = pPlayer->GetShopItemManager()->GetUsingItemInfo( pmsg->dwData3 );
			if( !pSealItem || !pTargetItem )
			{
				msg.dwData = 3;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}
			ITEM_INFO* pItemInfo = GetItemInfo( (WORD)pmsg->dwData3 );
			if( !pItemInfo )
			{
				msg.dwData = 3;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}

			if( pSealItem->wIconIdx != eIncantation_ItemSeal )
			{
				msg.dwData = 4;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}
			if( pItemInfo->ItemKind != eSHOP_ITEM_MAKEUP && pItemInfo->ItemKind != eSHOP_ITEM_DECORATION && pItemInfo->ItemKind != eSHOP_ITEM_PET )
			{
				msg.dwData = 5;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}
			if( pItemInfo->SellPrice != eShopItemUseParam_Forever )
			{
				msg.dwData = 6;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}
			if( pTargetItem->ItemParam & ITEM_PARAM_SEAL )
			{
				msg.dwData = 7;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}

			// SealItem삭제
			if( EI_TRUE != DiscardItem( pPlayer, (POSTYPE)pmsg->dwData2, (WORD)pmsg->dwData1, 1 ) )
			{
				msg.dwData = 9;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}

			LogItemMoney( pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
				eLog_ShopItemUse, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
				pSealItem->wIconIdx, pSealItem->dwDBIdx, pSealItem->Position, 0, pSealItem->Durability, pPlayer->GetPlayerExpPoint());

			// AvatarItem봉인
			DWORD Param = pTargetItem->ItemParam;
			Param |= ITEM_PARAM_SEAL;
			pTargetItem->ItemParam = Param;
			
			pPlayer->GetShopItemManager()->DeleteUsingShopItemInfo( pTargetItem->wIconIdx );
			ShopItemParamUpdateToDB( pPlayer->GetID(), pTargetItem->dwDBIdx, pTargetItem->ItemParam );
			ShopItemDeleteToDB( pPlayer->GetID(), pTargetItem->dwDBIdx );
			
			LogItemMoney( pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
				eLog_ShopItemSeal, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
				pTargetItem->wIconIdx, pTargetItem->dwDBIdx, pTargetItem->Position, 0, pTargetItem->Durability, pPlayer->GetPlayerExpPoint());

			//
			SEND_SHOPITEM_BASEINFO usemsg;
			SetProtocol( &usemsg, MP_ITEM, MP_ITEM_SHOPITEM_USE_ACK );
			usemsg.ShopItemPos = (POSTYPE)pmsg->dwData2;
			usemsg.ShopItemIdx = (WORD)pmsg->dwData1;
			SendAckMsg( pPlayer, &usemsg, sizeof(usemsg) );

			SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_SEAL_ACK );
			pPlayer->SendMsg( &msg, sizeof(msg) );
		}
		break;
	case MP_ITEM_SHOPITEM_JOBCHANGE_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer )			return;

			CItemSlot* pSlot = pPlayer->GetSlot( eItemTable_ShopInven );
			const ITEMBASE* pItem = pSlot->GetItemInfoAbs( (POSTYPE)pmsg->dwData1 );;
			DWORD stage = pPlayer->GetStage();
			DWORD changestage = 0;
			MSG_DWORD msg;

			// 화경, 극마 체크
			if( stage != eStage_Hwa && stage != eStage_Geuk )
			{
				msg.dwData = 1;
				goto JobChangeFailed;
			}
			// 아이템체크
			if( !pSlot || !pItem )
			{
				msg.dwData = 2;
				goto JobChangeFailed;
			}
			if( pItem->wIconIdx != eIncantation_ChangeJob || pItem->dwDBIdx != pmsg->dwData2 )
			{
				msg.dwData = 2;
				goto JobChangeFailed;
			}
			if( EI_TRUE != DiscardItem( pPlayer, pItem->Position, pItem->wIconIdx, 1 ) )
			{
				msg.dwData = 3;
				goto JobChangeFailed;
			}

			ChangeCharacterStageAbility( pPlayer->GetID(), (BYTE)stage, pPlayer->GetAbilityGroup() );

			if( stage == eStage_Hwa )					changestage = eStage_Geuk;
			else if( stage == eStage_Geuk )				changestage = eStage_Hwa;
			pPlayer->SetStage( (BYTE)changestage );


			SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_JOBCHANGE_ACK );
			pPlayer->SendMsg( &msg, sizeof(msg) );

			LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ShopItemUse, 
				stage, changestage, 0, pItem->wIconIdx, pItem->dwDBIdx, pItem->Position, 0, 
				pItem->Position, pPlayer->GetPlayerExpPoint());

			return;

JobChangeFailed:
			SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_JOBCHANGE_NACK );
			pPlayer->SendMsg( &msg, sizeof(msg) );
		}
		break;
	case MP_ITEM_SHOPITEM_REINFORCERESET_SYN:
		{
			MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer )			return;

			MSG_DWORD msg;
			SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_REINFORCERESET_NACK );

			if( !IsUseAbleShopItem( pPlayer, (WORD)pmsg->dwData1, (POSTYPE)pmsg->dwData2 ) )
			{
				msg.dwData = 1;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}

			ITEMBASE* pShopItem = (ITEMBASE*)GetItemInfoAbsIn( pPlayer, (POSTYPE)pmsg->dwData2 );
			ITEMBASE* pTargetItem = (ITEMBASE*)GetItemInfoAbsIn( pPlayer, (POSTYPE)pmsg->dwData4 );
			if( !pShopItem || !pTargetItem )
			{
				msg.dwData = 2;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}
			ITEM_INFO* pItemInfo = GetItemInfo( (WORD)pmsg->dwData3 );
			if( !pItemInfo )
			{
				msg.dwData = 3;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}
			if( pShopItem->wIconIdx != eIncantation_ReinforceReset )
			{
				msg.dwData = 4;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}
			if( !(pItemInfo->ItemKind & eEQUIP_ITEM) || pItemInfo->ItemKind > eEQUIP_ITEM_SHOES )
			{
				msg.dwData = 5;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}
			if( pPlayer->GetItemOption( pTargetItem->Durability ) == NULL )
			{
				msg.dwData = 6;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}

			// ShopItem삭제
			if( EI_TRUE != DiscardItem( pPlayer, (POSTYPE)pmsg->dwData2, (WORD)pmsg->dwData1, 1 ) )
			{
				msg.dwData = 9;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}

			LogItemMoney( pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
				eLog_ShopItemUse, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
				pShopItem->wIconIdx, pShopItem->dwDBIdx, pShopItem->Position, 0, pShopItem->Durability, pPlayer->GetPlayerExpPoint());

			// 강화정보삭제
			if( FALSE == pPlayer->RemoveItemOption( pTargetItem->Durability ) )
			{
				char szBuf[128];
				sprintf( szBuf, "Shop_REINFORCERESET_OptItem_Error! PlayerID : %d\t ItemDBIdx : %d\n", pPlayer->GetID()
					,pTargetItem->dwDBIdx );
				ASSERTMSG(0, szBuf);
			}

			CharacterItemOptionDelete( pTargetItem->Durability, pTargetItem->dwDBIdx );
			ItemUpdateToDB( pPlayer->GetID(), pTargetItem->dwDBIdx, pTargetItem->wIconIdx, 0, pTargetItem->Position, pTargetItem->QuickPosition, pTargetItem->RareIdx );

			LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ShopItem_ReinforceReset, 
				pPlayer->GetMoney(), pPlayer->GetMoney(), 0, pTargetItem->wIconIdx, pTargetItem->dwDBIdx, 
				pTargetItem->Position, pTargetItem->Position, pTargetItem->Durability, pPlayer->GetPlayerExpPoint());

			pTargetItem->Durability = 0;
			//
			SEND_SHOPITEM_BASEINFO usemsg;
			SetProtocol( &usemsg, MP_ITEM, MP_ITEM_SHOPITEM_USE_ACK );
			usemsg.ShopItemPos = (POSTYPE)pmsg->dwData2;
			usemsg.ShopItemIdx = (WORD)pmsg->dwData1;
			SendAckMsg( pPlayer, &usemsg, sizeof(usemsg) );

			SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_REINFORCERESET_ACK );
			pPlayer->SendMsg( &msg, sizeof(msg) );
		}
		break;
	case MP_ITEM_SHOPITEM_RARECREATE_SYN:
		{
			MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer )			return;

			MSG_DWORD msg;
			SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_RARECREATE_NACK );

			if( !IsUseAbleShopItem( pPlayer, (WORD)pmsg->dwData1, (POSTYPE)pmsg->dwData2 ) )
			{
				msg.dwData = 1;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}

			ITEMBASE* pShopItem = (ITEMBASE*)GetItemInfoAbsIn( pPlayer, (POSTYPE)pmsg->dwData2 );
			ITEMBASE* pTargetItem = (ITEMBASE*)GetItemInfoAbsIn( pPlayer, (POSTYPE)pmsg->dwData4 );
			if( !pShopItem || !pTargetItem )
			{
				msg.dwData = 2;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}
			ITEM_INFO* pShopItemInfo = GetItemInfo( (WORD)pmsg->dwData1 );
			ITEM_INFO* pItemInfo = GetItemInfo( (WORD)pmsg->dwData3 );
			if( !pShopItemInfo || !pItemInfo )
			{
				msg.dwData = 3;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}
			if( pShopItem->wIconIdx != eSundries_RareItemCreate50 &&
				pShopItem->wIconIdx != eSundries_RareItemCreate70 &&
				pShopItem->wIconIdx != eSundries_RareItemCreate90 &&
				pShopItem->wIconIdx != eSundries_RareItemCreate99)
			{
				msg.dwData = 4;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}
			if( !(pItemInfo->ItemKind & eEQUIP_ITEM) )
			{
				msg.dwData = 5;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}
			ITEM_OPTION_INFO* pOptionInfo = pPlayer->GetItemOption( pTargetItem->Durability );
			if( pTargetItem->Durability || pOptionInfo->dwOptionIdx )
			{
				msg.dwData = 6;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}
			if( pItemInfo->ItemKind <= eEQUIP_ITEM_SHOES && pTargetItem->wIconIdx % 10 )
			{
				msg.dwData = 7;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}
			if( pItemInfo->LimitLevel < pShopItemInfo->GenGol || pItemInfo->LimitLevel > pShopItemInfo->MinChub )
			{
				msg.dwData = 8;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}
			if( !RAREITEMMGR->IsRareItemAble( pTargetItem->wIconIdx ) )
			{
				msg.dwData = 9;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}

			
			ITEM_RARE_OPTION_INFO rareoption;
			if( RAREITEMMGR->GetRare( pTargetItem->wIconIdx, &rareoption, pPlayer, TRUE ) == FALSE )
			{
				msg.dwData = 10;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}

			// ShopItem삭제
			if( EI_TRUE != DiscardItem( pPlayer, (POSTYPE)pmsg->dwData2, (WORD)pmsg->dwData1, 1 ) )
			{
				msg.dwData = 11;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}

			// DB에 Rare정보 기록
			ShopItemRareInsertToDB( pPlayer->GetID(), pTargetItem->wIconIdx, pTargetItem->Position, pTargetItem->dwDBIdx, &rareoption );

			LogItemMoney( pPlayer->GetID(), pPlayer->GetObjectName(), pTargetItem->dwDBIdx, "",
				eLog_ShopItemUse, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
				pShopItem->wIconIdx, pShopItem->dwDBIdx, pShopItem->Position, 0, pShopItem->Durability, pPlayer->GetPlayerExpPoint());

			// Client로 보냄
			SEND_SHOPITEM_BASEINFO usemsg;
			SetProtocol( &usemsg, MP_ITEM, MP_ITEM_SHOPITEM_USE_ACK );
			usemsg.ShopItemPos = (POSTYPE)pmsg->dwData2;
			usemsg.ShopItemIdx = (WORD)pmsg->dwData1;
			SendAckMsg( pPlayer, &usemsg, sizeof(usemsg) );
		}
		break;
	case MP_ITEM_SHOPITEM_SHOUT_SYN:
		{
			SEND_SHOUTBASE_ITEMINFO* pmsg = (SEND_SHOUTBASE_ITEMINFO*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer )			return;

			MSGBASE nmsg;
			SetProtocol( &nmsg, MP_ITEM, MP_ITEM_SHOPITEM_SHOUT_NACK );

			if( IsUseAbleShopItem( pPlayer, pmsg->ItemIdx, pmsg->ItemPos ) == FALSE )
			{
				pPlayer->SendMsg( &nmsg, sizeof(nmsg) );
				return;
			}

			if( pmsg->ItemIdx == eSundries_Shout_Once || pmsg->ItemIdx == eSundries_Shout_Once_NoTrade )
			{
				if( EI_TRUE != DiscardItem( pPlayer, pmsg->ItemPos, pmsg->ItemIdx, 1 ) )
				{
					pPlayer->SendMsg( &nmsg, sizeof(nmsg) );
					return;
				}

				SEND_SHOPITEM_BASEINFO msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_USE_ACK;				
				msg.ShopItemIdx = pmsg->ItemIdx;
				msg.ShopItemPos = pmsg->ItemPos;					
				ITEMMGR->SendAckMsg(pPlayer, &msg, sizeof(msg));	
			}

			// Agent로 보낸다.
			SEND_SHOUTBASE msg;
			SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_SHOUT_ACK );			
			SafeStrCpy( msg.ShoutMsg[0].Msg, pmsg->Msg, MAX_SHOUT_LENGTH+1 );
			msg.ShoutMsg[0].CharacterIdx = pPlayer->GetID();
			msg.Count = 1;
			g_Network.Send2AgentServer( (char*)&msg, msg.GetSize() );
		}
		break;
	case MP_ITEM_SHOPITEM_SHOUT_ACK:
		{
			SEND_SHOUTRECEIVE* pmsg = (SEND_SHOUTRECEIVE*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->Receive.CharacterIdx );
			if( !pPlayer )			return;

			pmsg->Protocol = MP_ITEM_SHOPITEM_SHOUT_SENDACK;
			pPlayer->SendMsg( pmsg, sizeof(*pmsg) );
		}
		break;
	case MP_ITEM_SHOPITEM_SHOUT_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData );
			if( !pPlayer )			return;

			pPlayer->SendMsg( pmsg, sizeof(*pmsg) );
		}
		break;
	// magi82 - Titan(070307)
	//case MP_ITEM_TITAN_MUGONG_CHANGE_SYN:
	//	{
	//		MSG_WORD3* pmsg = (MSG_WORD3*)pMsg;

	//		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	//		if( !pPlayer )			return;

	//		// TITANITEMMGR->TitanMugongChange(pPlayer, pmsg);	// magi82 - Titan(070611) 타이탄 무공변환 주석처리
	//	}
	//	break;
		// magi82 - Titan(070413)
	case MP_ITEM_TITAN_REGISTER_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( pPlayer == NULL ) return;

			TITANITEMMGR->TitanRegister(pPlayer, pmsg);
		}
		break;
		//////////////////////////////////////////////////////////////////////////
	// magi82 - Titan(070321)
	case MP_ITEM_TITAN_REGISTER_ADDITEM_SYN:
		{
			MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( pPlayer == NULL ) return;

			TITANITEMMGR->TitanRegisterAdditem(pPlayer, pmsg);
		}
		break;
	case MP_ITEM_TITAN_REGISTER_RELEASEITEM:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( pPlayer == NULL ) return;

			CItemSlot* pSlot = pPlayer->GetSlot(pmsg->wData);
			if(pSlot)
				pSlot->SetLock(pmsg->wData, FALSE);
		}
		break;
	//////////////////////////////////////////////////////////////////////////
		// magi82 - Titan(070413)
	case MP_ITEM_TITAN_DISSOLUTION_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( pPlayer == NULL ) return;

			TITANITEMMGR->TitanCancellation(pPlayer, pmsg);
		}
		break;
		//////////////////////////////////////////////////////////////////////////
	// magi82 - Titan(070322)
	case MP_ITEM_TITAN_DISSOLUTION_ADDITEM_SYN:
		{
			MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( pPlayer == NULL ) return;

			TITANITEMMGR->TitanDissolutionAdditem(pPlayer, pmsg);
		}
		break;
	case MP_ITEM_TITAN_DISSOLUTION_RELEASEITEM:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( pPlayer == NULL ) return;

			CItemSlot* pSlot = pPlayer->GetSlot(pmsg->wData);
			if(pSlot)
				pSlot->SetLock(pmsg->wData, FALSE);
		}
		break;
	default:
		break;
	}
}



void CItemManager::NetworkMsgParseExt( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg )
{
	MSGBASE * pmsg = (MSGBASE *)pMsg;	

	switch(Protocol)
	{
		//////////////////////////////////////////////////////////////////////////
		// magi82 - UniqueItem(070703)
	case MP_ITEMEXT_SHOPITEM_CURSE_CANCELLATION_ADDITEM_SYN:
		{
			MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( pPlayer == NULL ) return;

			WORD wErrorCode = 0;
			CItemSlot * pSlot = NULL;

			// 인벤토리 아이템이 아니면 에러
			if( GetTableIdxPosition( pmsg->ItemInfo.Position ) != eItemTable_Inventory )
				wErrorCode = 1;

			// 서버와 클라의 아이템정보가 같은지 체크
			if(!CHKRT->ItemOf(pPlayer, pmsg->ItemInfo.Position, pmsg->ItemInfo.wIconIdx, 0,0, CB_EXIST|CB_ICONIDX))
				wErrorCode = 2;

			pSlot = pPlayer->GetSlot(eItemTable_Inventory);
			if( pSlot )
			{
				// 해당 슬롯이 이미 잠겨있으면 에러
				if( pSlot->IsLock( pmsg->ItemInfo.Position ) )
					wErrorCode = 3;

				// 정상일때 처리
				if(wErrorCode == 0)
				{
					pSlot->SetLock( pmsg->ItemInfo.Position, TRUE );
					MSG_ITEM msgAck;

					msgAck.Category		= MP_ITEMEXT;
					msgAck.Protocol		= MP_ITEMEXT_SHOPITEM_CURSE_CANCELLATION_ADDITEM_ACK;
					msgAck.ItemInfo		= pmsg->ItemInfo;
					pPlayer->SendMsg( &msgAck, sizeof( msgAck ) );			
				}
				else
				{
					MSG_WORD2 msgNack;
					msgNack.Category	= MP_ITEMEXT;
					msgNack.Protocol	= MP_ITEMEXT_SHOPITEM_CURSE_CANCELLATION_ADDITEM_NACK;
					msgNack.wData1		= pmsg->ItemInfo.Position;
					msgNack.wData2		= wErrorCode;
					pPlayer->SendMsg( &msgNack, sizeof( msgNack ) );		
				}
			}
		}
		break;
		// magi82 - UniqueItem(070704)
	case MP_ITEMEXT_SHOPITEM_CURSE_CANCELLATION_RELEASE:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( pPlayer == NULL ) return;

			CItemSlot* pSlot = pPlayer->GetSlot(pmsg->wData);

			if(pSlot)
			{
				pSlot->SetLock(pmsg->wData, FALSE);
			}	
		}
		break;
	case MP_ITEMEXT_SHOPITEM_CURSE_CANCELLATION_SYN:
		{
			MSG_DWORD_WORD2* pmsg = (MSG_DWORD_WORD2*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( pPlayer == NULL ) return;

			MSG_DWORD msg;
			SetProtocol( &msg, MP_ITEMEXT, MP_ITEMEXT_SHOPITEM_CURSE_CANCELLATION_NACK );

			// 아이템이 존재하는지 체크
			if(!CHKRT->ItemOf(pPlayer, pmsg->wData1, (WORD)pmsg->dwData, 0,0, CB_EXIST|CB_ICONIDX))
			{
				msg.dwData = 1;
				goto UniqueItemCurseCancellation;
			}

			// 유니크 아이템 정보
			UNIQUE_ITEM_OPTION_INFO* pInfo = GAMERESRCMNGR->GetUniqueItemOptionList(pmsg->dwData);
			if( !pInfo )
			{
				msg.dwData = 2;
				goto UniqueItemCurseCancellation;
			}

			// 저주가 걸린 유니크 아이템인지 체크
			if( pInfo->dwCurseCancellation == 0 )
			{
				msg.dwData = 2;
				goto UniqueItemCurseCancellation;
			}

			// ShopItem 삭제
			ITEMBASE* pItem = (ITEMBASE*)GetItemInfoAbsIn( pPlayer, (POSTYPE)pmsg->wData2 );
			if( EI_TRUE != DiscardItem( pPlayer, (POSTYPE)pmsg->wData2, pItem->wIconIdx, 1 ) )
			{
				msg.dwData = 3;
				goto UniqueItemCurseCancellation;
			}
			else
			{
				SEND_SHOPITEM_BASEINFO msg;
				SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_USE_ACK );
				msg.ShopItemPos = (POSTYPE)pmsg->wData2;
				msg.ShopItemIdx = pItem->wIconIdx;
				SendAckMsg( pPlayer, &msg, sizeof(msg) );

				// Log추가
				LogItemMoney( pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
					eLog_ShopItemUse, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
					pItem->wIconIdx, pItem->dwDBIdx, pItem->Position, 0, pItem->Durability, pPlayer->GetPlayerExpPoint());
			}

			// 저주받은 유니크 아이템 삭제
			//memset( pItem, 0, sizeof(ITEMBASE) );
			pItem = (ITEMBASE*)GetItemInfoAbsIn( pPlayer, (POSTYPE)pmsg->wData1 );
			if(EI_TRUE == DiscardItem(pPlayer, pmsg->wData1, (WORD)pmsg->dwData, 1))
			{
				WORD wItemIdx = 0;
				DURTYPE ItemNum = 0;

				MSG_ITEM_DISCARD_ACK msg;
				msg.TargetPos = pmsg->wData1;
				msg.wItemIdx = (WORD)pmsg->dwData;
				msg.ItemNum = 1;
				msg.Category = MP_ITEMEXT;
				msg.Protocol = MP_ITEMEXT_SHOPITEM_CURSE_CANCELLATION_DELETEITEM;
				SendAckMsg(pPlayer, &msg, sizeof(msg));

				// Log
				LogItemMoney(pmsg->dwObjectID, pPlayer->GetObjectName(), 0, "",
					eLog_ItemDiscard, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
					pItem->wIconIdx, pItem->dwDBIdx,  pItem->Position, 0,
					pItem->Durability, pPlayer->GetPlayerExpPoint());
			}

			// 저주해제된 유니크 아이템 생성
			WORD EmptyCellPos[255];
			WORD EmptyCellNum;
			CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);
			WORD obtainItemNum = GetCanBuyNumInSpace( pPlayer, pSlot, (WORD)pInfo->dwCurseCancellation, 1, EmptyCellPos, EmptyCellNum );
			if(obtainItemNum == 0)
				break;

			ObtainItemEx( pPlayer, Alloc(pPlayer, MP_ITEMEXT, MP_ITEMEXT_SHOPITEM_CURSE_CANCELLATION_ACK, pPlayer->GetID(), 0, eLog_ItemObtainTitan, obtainItemNum, (DBResult)(ObtainItemDBResult)), (WORD)pInfo->dwCurseCancellation, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum );

UniqueItemCurseCancellation:
			pPlayer->SendMsg( &msg, sizeof(msg) );
			return;
		}
		break;
		// magi82 - UniqueItem(070710)
	case MP_ITEMEXT_UNIQUEITEM_MIX_ADDITEM_SYN:
		{
			MSG_ITEM* pmsg = (MSG_ITEM*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( pPlayer == NULL ) return;

			WORD wErrorCode = 0;
			CItemSlot * pSlot = NULL;

			// 인벤토리 아이템이 아니면 에러
			if( GetTableIdxPosition( pmsg->ItemInfo.Position ) != eItemTable_Inventory )
				wErrorCode = 1;

			// 서버와 클라의 아이템정보가 같은지 체크
			if(!CHKRT->ItemOf(pPlayer, pmsg->ItemInfo.Position, pmsg->ItemInfo.wIconIdx, 0,0, CB_EXIST|CB_ICONIDX))
				wErrorCode = 2;

			pSlot = pPlayer->GetSlot(eItemTable_Inventory);
			if( pSlot )
			{
				// 해당 슬롯이 이미 잠겨있으면 에러
				if( pSlot->IsLock( pmsg->ItemInfo.Position ) )
					wErrorCode = 3;

				// 정상일때 처리
				if(wErrorCode == 0)
				{
					pSlot->SetLock( pmsg->ItemInfo.Position, TRUE );
					MSG_ITEM msgAck;

					msgAck.Category		= MP_ITEMEXT;
					msgAck.Protocol		= MP_ITEMEXT_UNIQUEITEM_MIX_ADDITEM_ACK;
					msgAck.ItemInfo		= pmsg->ItemInfo;
					pPlayer->SendMsg( &msgAck, sizeof( msgAck ) );			
				}
				else
				{
					MSG_WORD2 msgNack;
					msgNack.Category	= MP_ITEMEXT;
					msgNack.Protocol	= MP_ITEMEXT_UNIQUEITEM_MIX_ADDITEM_NACK;
					msgNack.wData1		= pmsg->ItemInfo.Position;
					msgNack.wData2		= wErrorCode;
					pPlayer->SendMsg( &msgNack, sizeof( msgNack ) );		
				}
			}
		}
		break;
	case MP_ITEMEXT_UNIQUEITEM_MIX_RELEASE:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( pPlayer == NULL ) return;

			CItemSlot* pSlot = pPlayer->GetSlot(pmsg->wData);

			if(pSlot)
			{
				pSlot->SetLock(pmsg->wData, FALSE);
			}	
		}
		break;
	case MP_ITEMEXT_UNIQUEITEM_MIX_SYN:
		{
			MSG_UNIQUEITEM_MIX_SYN* pmsg = (MSG_UNIQUEITEM_MIX_SYN*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( pPlayer == NULL ) return;

			WORD	wBasicItemIdx = pmsg->wBasicItemIdx;
			POSTYPE wBasicItemPos = pmsg->wBasicItemPos;
			WORD	wMaterialNum = pmsg->wMaterialNum;
			MATERIAL_ARRAY sMaterial[MAX_MIX_MATERIAL] = { 0, };
			CopyMemory( sMaterial, pmsg->Material, sizeof(MATERIAL_ARRAY) * MAX_MIX_MATERIAL );


			// 원본 아이템이 존재하는가?
			if(!CHKRT->ItemOf(pPlayer, wBasicItemPos, wBasicItemIdx, 0, 0, CB_EXIST|CB_ICONIDX))
				break;

			// 재료 아이템이 모두 존재하는가?
			for( int i = 0; i < wMaterialNum; i++ )
			{
				if(pmsg->wBasicItemPos == sMaterial[i].ItemPos)
					break;

				if(!CHKRT->ItemOf(pPlayer, sMaterial[i].ItemPos, sMaterial[i].wItemIdx, sMaterial[i].Dur, 0, CB_EXIST|CB_ICONIDX))
					break;
			}

			// 유니크 아이템 조합 정보
			UNIQUE_ITEM_MIX_INFO* pInfo = GAMERESRCMNGR->GetUniqueItemMixList(wBasicItemIdx);
			if( !pInfo )
				break;

			DWORD dwNeedItemIdx = 0;
			WORD wNeedItemDur = 0;
			for( int i = 0; i < pInfo->wMaterialKindNum; i++ )
			{
				dwNeedItemIdx = pInfo->sUniqueItemMixMaterial[i].dwMaterialIdx;
				wNeedItemDur = pInfo->sUniqueItemMixMaterial[i].wMaterialNum;

				// 재료가 모두 있는지 체크
                if(!EnoughMixMaterial( (WORD)dwNeedItemIdx, wNeedItemDur, sMaterial, wMaterialNum) )
					break;
			}

			ITEMBASE Item;
			memset( &Item, 0, sizeof(ITEMBASE) );
			CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);
			// 재료 아이템들 삭제
			for( int i = 0; i < wMaterialNum; i++ )
			{
				Item = *pSlot->GetItemInfoAbs(sMaterial[i].ItemPos);
				if(EI_TRUE == ITEMMGR->DiscardItem(pPlayer, sMaterial[i].ItemPos, sMaterial[i].wItemIdx, sMaterial[i].Dur))
				{
					POSTYPE TargetPos = 0;
					WORD wItemIdx = 0;
					DURTYPE ItemNum = 0;

					MSG_ITEM_DISCARD_ACK msg;
					msg.TargetPos = sMaterial[i].ItemPos;
					msg.wItemIdx = sMaterial[i].wItemIdx;
					msg.ItemNum = sMaterial[i].Dur;
					msg.Category =  MP_ITEMEXT;
					msg.Protocol = MP_ITEMEXT_UNIQUEITEM_MIX_DELETEITEM;
					ITEMMGR->SendAckMsg(pPlayer, &msg, sizeof(msg));

					// Log
					LogItemMoney(pmsg->dwObjectID, pPlayer->GetObjectName(), 0, "",
						eLog_ItemDiscard, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
						Item.wIconIdx, Item.dwDBIdx,  Item.Position, 0,
						Item.Durability, pPlayer->GetPlayerExpPoint());
				}
			}

			// Base 아이템 삭제
			Item = *pSlot->GetItemInfoAbs(wBasicItemPos);
			if(EI_TRUE == ITEMMGR->DiscardItem(pPlayer, wBasicItemPos, wBasicItemIdx, 1))
			{
				POSTYPE TargetPos = 0;
				WORD wItemIdx = 0;
				DURTYPE ItemNum = 0;

				MSG_ITEM_DISCARD_ACK msg;
				msg.TargetPos = wBasicItemPos;
				msg.wItemIdx = wBasicItemIdx;
				msg.ItemNum = 1;
				msg.Category =  MP_ITEMEXT;
				msg.Protocol = MP_ITEMEXT_UNIQUEITEM_MIX_DELETEITEM;
				ITEMMGR->SendAckMsg(pPlayer, &msg, sizeof(msg));

				// Log
				LogItemMoney(pmsg->dwObjectID, pPlayer->GetObjectName(), 0, "",
					eLog_ItemDiscard, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
					Item.wIconIdx, Item.dwDBIdx,  Item.Position, 0,
					Item.Durability, pPlayer->GetPlayerExpPoint());
			}

			// 유니크 아이템 조합 확률 계산
			WORD wSeed = random(1, 100);
			WORD wResultItemIdx = 0;
			WORD wMinRate = 0, wMaxRate = 0;

			for( int i = 0; i < 5; i++ )
			{
				wMinRate = wMaxRate;
				wMaxRate += pInfo->sUniqueItemMixResult[i].wResultRate;

				if(wSeed > wMinRate && wSeed <= wMaxRate)
				{
					wResultItemIdx = (WORD)pInfo->sUniqueItemMixResult[i].dwResultIdx;
					break;
				}
			}

			// 조합된 아이템 생성
			// 아이템 생성
			WORD EmptyCellPos[255];
			WORD EmptyCellNum;

			WORD obtainItemNum = ITEMMGR->GetCanBuyNumInSpace( pPlayer, pSlot, wResultItemIdx, 1, EmptyCellPos, EmptyCellNum );
			if(obtainItemNum == 0)
				return;

			ITEMMGR->ObtainItemEx( pPlayer, ITEMMGR->Alloc(pPlayer, MP_ITEMEXT, MP_ITEMEXT_UNIQUEITEM_MIX_ACK, pPlayer->GetID(), 0, eLog_ItemObtainTitan, obtainItemNum, (DBResult)(ITEMMGR->ObtainItemDBResult)), wResultItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum );
		}
		break;
	case MP_ITEMEXT_SKINITEM_SELECT_SYN:	//2007. 12. 5. CBH - 스킨 아이탬 처리
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( pPlayer == NULL ) 
				return;
			
			//일반의복 스킨은 리스트 인덱스로 사용하고 코스튬 스킨은 아이탬 인덱스로 사용			
			if(pmsg->dwData1 > 0)	
			{
				DWORD dwResult = pPlayer->GetShopItemManager()->PutSkinSelectItem(pmsg);

				if(dwResult == eSkinResult_Success)
				{
					pPlayer->InitSkinDelay();	//스킨 딜레이 시간 초기화
					pPlayer->StartSkinDelay();	//스킨 딜레이 시작

					SEND_SKIN_INFO msg;
					msg.Category = MP_ITEMEXT;
					msg.Protocol = MP_ITEMEXT_SKINITEM_SELECT_ACK;
					msg.dwObjectID = pPlayer->GetID();
					memcpy( &msg.wSkinItem, pPlayer->GetShopItemStats()->wSkinItem, sizeof(WORD)*eSkinItem_Max );
					PACKEDDATA_OBJ->QuickSend( pPlayer, &msg, sizeof(msg) );
				}
				else
				{
					SKIN_SELECT_ITEM_INFO* pSkinInfo = GAMERESRCMNGR->GetNomalClothesSkinList(pmsg->dwData1);
					if(pSkinInfo == NULL)
						return;

					MSG_DWORD3 msg;
					SetProtocol( &msg, MP_ITEMEXT, MP_ITEMEXT_SKINITEM_SELECT_NACK );
					msg.dwObjectID = pPlayer->GetID();
					msg.dwData1 = dwResult;						//오류 리턴 결과
					msg.dwData2 = pPlayer->GetSkinDelayTime();	//남은 딜레이 시간
					msg.dwData3 = pSkinInfo->dwLimitLevel;

					pPlayer->SendMsg(&msg, sizeof(MSG_DWORD3));
				}
			}
			else if(pmsg->dwData1 == 0)	//원래대로 버튼 클릭시 처리( pmsg->dwData 값이 0이면 원래대로 )
			{
				pPlayer->InitSkinDelay();	//스킨 딜레이 시간 초기화
				pPlayer->StartSkinDelay();	//스킨 딜레이 시작

				pPlayer->GetShopItemManager()->RemoveEquipSkin(pmsg->dwData2);				

				SEND_SKIN_INFO msg;
				msg.Category = MP_ITEMEXT;
				msg.Protocol = MP_ITEMEXT_SKINITEM_SELECT_ACK;
				msg.dwObjectID = pPlayer->GetID();
				memcpy( &msg.wSkinItem, pPlayer->GetShopItemStats()->wSkinItem, sizeof(WORD)*eSkinItem_Max );
				PACKEDDATA_OBJ->QuickSend( pPlayer, &msg, sizeof(msg) );				
			}
		}
		break;
	case MP_ITEMEXT_SHOPITEM_DECORATION_ON:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( pPlayer == NULL ) 
				return;

			// 다른사람들에게 아이템 착용 정보를 보낸다.
			MSG_DWORD2 msg;
			msg.Category = MP_ITEMEXT;
			msg.Protocol = MP_ITEMEXT_SHOPITEM_DECORATION_ON;
			msg.dwObjectID = pPlayer->GetID();
			msg.dwData1 = pmsg->dwData1;
			msg.dwData2 = pmsg->dwData2;

			PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer, &msg, sizeof(msg));
		}
		break;
	default:
		break;
	}
}


BOOL CItemManager::ItemMoveUpdateToDBbyTable(CPlayer* pPlayer, DWORD dwfromDBIdx, WORD dwfromIconIdx, POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos)
{
	if(TP_INVENTORY_START <= frompos && frompos < TP_WEAR_END)
	{
		ItemMoveUpdateToDB(pPlayer->GetID(), dwfromDBIdx, frompos, dwtoDBIdx, topos);
	}
	else if(TP_GUILDWAREHOUSE_START <= frompos && frompos < TP_GUILDWAREHOUSE_END)
	{
		ItemMoveGuildUpdateToDB(pPlayer->GetID(), pPlayer->GetGuildIdx(), dwfromDBIdx, frompos, dwtoDBIdx, topos);
	}
	else if(TP_PYOGUK_START <= frompos && frompos < TP_PYOGUK_END)
	{
		ItemMovePyogukUpdateToDB(pPlayer->GetID(), pPlayer->GetUserID(), dwfromDBIdx, frompos, dwtoDBIdx, topos);
	}
	else if(TP_SHOPITEM_START <= frompos && frompos < TP_SHOPITEM_END)
	{
		ItemMoveShopUpdateToDB(pPlayer->GetID(), pPlayer->GetUserID(), dwfromDBIdx, frompos, dwtoDBIdx, topos);
	}	
	else if(TP_SHOPINVEN_START <= frompos && frompos < TP_SHOPINVEN_END)
	{
		ItemMoveUpdateToDB(pPlayer->GetID(), dwfromDBIdx, frompos, dwtoDBIdx, topos);
	}
	else if(TP_PETINVEN_START <= frompos && frompos < TP_PETWEAR_END)
	{
		ItemMovePetInvenUpdateToDB(pPlayer->GetID(), dwfromDBIdx, frompos, dwtoDBIdx, topos);
	}
	// magi82 - Titan(070207)
	else if(TP_TITANWEAR_START <= frompos && frompos < TP_TITANWEAR_END)
	{
		// 타이탄 장착아이템이지만 펫과 동일하므로 펫 함수를 씀
		ItemMovePetInvenUpdateToDB(pPlayer->GetID(), dwfromDBIdx, frompos, dwtoDBIdx, topos);
	}
	// magi82 - Titan(070228)
	else if(TP_TITANSHOPITEM_START <= frompos && frompos < TP_TITANSHOPITEM_END)
	{
		// 타이탄 샵아이템이지만 펫과 동일하므로 펫 함수를 씀
		ItemMovePetInvenUpdateToDB(pPlayer->GetID(), dwfromDBIdx, frompos, dwtoDBIdx, topos);
	}
	else
	{
		ASSERT(0);
		return FALSE;
	}
	return TRUE;
}

void CItemManager::ItemUpdatebyTable(CPlayer* pPlayer, DWORD dwDBIdx, WORD wIconIdx, DURTYPE FromDur, POSTYPE Position, POSTYPE QuickPosition)
{
	if(TP_INVENTORY_START <= Position && Position < TP_WEAR_END)
	{
		ItemUpdateToDB( pPlayer->GetID(), dwDBIdx, wIconIdx, FromDur, Position, QuickPosition );
	}
	else if(TP_SHOPINVEN_START <= Position && Position < TP_SHOPINVEN_END)
	{
		ItemUpdateToDB( pPlayer->GetID(), dwDBIdx, wIconIdx, FromDur, Position, QuickPosition );
	}
	else if(TP_PETINVEN_START <= Position && Position < TP_PETINVEN_END )
	{
		ItemUpdateToDB( pPlayer->GetID(), dwDBIdx, wIconIdx, FromDur, Position, QuickPosition );
	}
	else if(TP_GUILDWAREHOUSE_START <= Position && Position < TP_GUILDWAREHOUSE_END )
	{
		ItemUpdateToDB( pPlayer->GetID(), dwDBIdx, wIconIdx, FromDur, Position, QuickPosition );
	}
	else
	{
		ASSERT(0);
	}
}

BOOL CItemManager::CanMovetoGuildWare(POSTYPE FromPos, POSTYPE ToPos, CPlayer* pPlayer)
{
	CItemSlot * pFromSlot		= pPlayer->GetSlot(FromPos);
	CItemSlot * pToSlot			= pPlayer->GetSlot(ToPos);

	//￠￥U￠￢¡IA¡E￠?￠®¨u¡ⓒ ⓒo￠c¨¡AA¡E8¡¤I AI￥i￠?CO￠O¡×
	if(TP_GUILDWAREHOUSE_START <= ToPos && ToPos < TP_GUILDWAREHOUSE_END)
	{
		//ToPos¨u¨¡AIAUAI AO8￠￢e AI￥i￠? ￠￢ⓒªCO
		if(!pToSlot->IsEmpty(ToPos)) 
			return FALSE;
		return TRUE;
	}
	
	return FALSE;
}

void CItemManager::ClearQuickPos(ITEMBASE* pItem, POSTYPE ToPos)
{
	if((TP_GUILDWAREHOUSE_START <= ToPos && ToPos < TP_GUILDWAREHOUSE_END) 
		|| (TP_PYOGUK_START <= ToPos && ToPos < TP_PYOGUK_END))
	{
		pItem->QuickPosition = 0;
		return;
	}
}
DealerData * CItemManager::GetDealer(WORD wDealerIdx)
{
	return m_DealerTable.GetData(wDealerIdx);
}
void CItemManager::LoadDealerItem()
{
	char buff[1024];
	char mapname[256];
	char npcname[256];
//	DWORD DealerKey;
	BOOL IsItem = FALSE;
	WORD ItemIdx;

	//SW070626 보부상NPC
	int	ItemCount = 0;
	
	DealerItem* pItem;
	DealerData* pCurDealer = NULL;
	char seps[]   = "\t\n";
	char* token;
	int count = 0;

	CMHFile fp;
#ifdef _FILE_BIN_
	fp.Init("./Resource/DealItem.bin","rb");
#else
	fp.Init("./Resource/DealItem.txt","rt");
#endif

	while(1)
	{
		if(fp.IsEOF())
		{
			break;
		}
		count++;
		WORD map_num = fp.GetWord();
		fp.GetString(mapname);
		WORD npc_kind = fp.GetWord();
		fp.GetString(npcname);
		WORD npc_index = fp.GetWord();
		WORD point_x = fp.GetWord();
		WORD point_z = fp.GetWord();
		WORD angle = fp.GetWord();
		BYTE tabnum = fp.GetByte();
		BYTE Pos = 0;
		
		fp.GetLine(buff,1024);

		token = strtok( buff, seps );
		if(token == NULL)
			continue;
		token = strtok( NULL, seps );
		ItemIdx = atoi(token);

		//SW070626 보부상NPC
		// 아이템 판매량 추가	-1: 무제한 0: 판매불가 1~: 갯수(5까지..겹치는 아이템 표시부분에 제한이 걸려있다)
		token = strtok( NULL, seps );
		ItemCount = atoi(token);

		pCurDealer = m_DealerTable.GetData(npc_index);
		if(pCurDealer == NULL)
		{
			pCurDealer = new DealerData;
			m_DealerTable.Add(pCurDealer,npc_index);
		}
		
		if( ItemIdx != 0 )
		{
			pItem = new DealerItem;
			ASSERT(tabnum)
			pItem->Tab = tabnum-1;
			pItem->Pos = Pos++;
			pItem->ItemIdx = ItemIdx;
			pItem->ItemCount = ItemCount;
			pCurDealer->m_DealItemList.AddTail(pItem);
		}
		else
		{
			Pos++;
		}

		while( 1 )
		{
			token = strtok( NULL, seps );
			if(token == NULL)
				break;
			token = strtok( NULL, seps );
			ItemIdx = atoi(token);
			
			//SW070626 보부상NPC
			// 아이템 판매량 추가	-1: 무제한 0: 판매불가 1~: 갯수(5까지..겹치는 아이템 표시부분에 제한이 걸려있다)
			token = strtok( NULL, seps );
			ItemCount = atoi(token);

			if( ItemIdx != 0 )
			{
				pItem = new DealerItem;
				pItem->Tab = tabnum-1;
				pItem->Pos = Pos++;
				pItem->ItemIdx = ItemIdx;
				pItem->ItemCount = ItemCount;
				pCurDealer->m_DealItemList.AddTail(pItem);
			}
			else
			{
				Pos++;
			}
		}
	}
	fp.Release();
}

int CItemManager::ObtainItemFromQuest( CPlayer* pPlayer, WORD wItemKind, DWORD dwItemNum )
{
	WORD EmptyCellPos[255];
	WORD EmptyCellNum;
	CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	WORD obtainItemNum = GetCanBuyNumInSpace( pPlayer, pSlot, wItemKind, dwItemNum, EmptyCellPos, EmptyCellNum );
	if( obtainItemNum == 0 )
		return 1;

	return ObtainItemEx( pPlayer, Alloc(pPlayer, MP_QUEST, MP_QUEST_TAKEITEM_ACK,
						 pPlayer->GetID(), 0, eLog_ItemObtainQuest, obtainItemNum, (DBResult)(ObtainItemDBResult)),
						 wItemKind, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum );
	return 0;
}

int CItemManager::ObtainItemFromChangeItem( CPlayer* pPlayer, WORD wItemKind, WORD wItemNum )
{
	WORD EmptyCellPos[255];
	WORD EmptyCellNum;

	CItemSlot* pSlot = pPlayer->GetSlot(eItemTable_Inventory);

	WORD bSeal = 0;
	ITEM_INFO* pItemInfo = GetItemInfo( wItemKind );
	if( !pItemInfo )		return 2;
	if( pItemInfo->ItemKind & eSHOP_ITEM )
	{
		pSlot = pPlayer->GetSlot(eItemTable_ShopInven);
		bSeal = 1;
	}

	
	WORD obtainItemNum = GetCanBuyNumInSpace( pPlayer, pSlot, wItemKind, wItemNum, EmptyCellPos, EmptyCellNum );
	if( obtainItemNum == 0 )
		return 1;

	// 06.12.13 RaMa - 얻은갯수랑 실제 갯수랑 다르면 에러
	if( obtainItemNum != wItemNum )
		return 2;

	return ObtainItemEx( pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_USE_CHANGEITEM_ACK,
						 pPlayer->GetID(), 0, eLog_ItemObtainFromChangeItem, obtainItemNum, (DBResult)(ObtainItemDBResult)),
						 wItemKind, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum, bSeal );
}

BOOL CItemManager::CanbeMoved(WORD wIconIdx,POSTYPE pos, CPlayer* pPlayer)
{
	if(TP_INVENTORY_START<= pos && pos < TP_INVENTORY_END)
	{
#ifdef _JAPAN_LOCAL_

		DWORD GivenInvenCount = TP_INVENTORY_START+(TABCELL_INVENTORY_NUM*GIVEN_INVENTORY_SLOT);
		if( pos >= GivenInvenCount )
		{
			DWORD ExtraSlot = pPlayer->GetExtraInvenSlot();
			if( ExtraSlot == 0 ||
				pos >= GivenInvenCount+(TABCELL_INVENTORY_NUM*ExtraSlot) )
				return FALSE;
		}

#endif	// _JAPAN_LOCAL_
#ifdef _HK_LOCAL_
		DWORD GivenInvenCount = TP_INVENTORY_START+(TABCELL_INVENTORY_NUM*GIVEN_INVENTORY_SLOT);
		if( pos >= GivenInvenCount )
		{
			DWORD ExtraSlot = pPlayer->GetExtraInvenSlot();
			if( ExtraSlot == 0 ||
				pos >= GivenInvenCount+(TABCELL_INVENTORY_NUM*ExtraSlot) )
				return FALSE;
		}

#endif	// _HK_LOCAL_

#ifdef _TL_LOCAL_

		DWORD GivenInvenCount = TP_INVENTORY_START+(TABCELL_INVENTORY_NUM*GIVEN_INVENTORY_SLOT);
		if( pos >= GivenInvenCount )
		{
			DWORD ExtraSlot = pPlayer->GetExtraInvenSlot();
			if( ExtraSlot == 0 ||
				pos >= GivenInvenCount+(TABCELL_INVENTORY_NUM*ExtraSlot) )
				return FALSE;
		}

#endif	// _TL_LOCAL_

		ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(wIconIdx);
		if(!pInfo) return FALSE;
		if( pInfo->ItemKind & eSHOP_ITEM )
			return FALSE;

		return TRUE;
	}
	else if(TP_WEAR_START <= pos && pos < TP_WEAR_END)
	{
		ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(wIconIdx);
		if(!pInfo) return FALSE;
		if( !( pInfo->ItemKind & eEQUIP_ITEM ) )
		{
			if( ( pInfo->ItemKind & eQUEST_ITEM ) )
			{
			}
			else if( ( pInfo->ItemKind == eSHOP_ITEM_EQUIP ) )
			{
			}
			else
				return FALSE;
		}

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
		
		return CanEquip(pInfo, pPlayer);
	}
	else if(TP_PYOGUK_START <= pos && pos < TP_PYOGUK_END)
	{
#ifdef _JAPAN_LOCAL_

		DWORD GivenInvenCount = TP_PYOGUK_START+(TABCELL_PYOGUK_NUM*GIVEN_PYOGUK_SLOT);
		if( pos >= GivenInvenCount )
		{
			DWORD ExtraSlot = pPlayer->GetPyogukNum()-GIVEN_PYOGUK_SLOT;
			if( ExtraSlot == 0 ||
				pos >= GivenInvenCount+(TABCELL_PYOGUK_NUM*ExtraSlot) )
				return FALSE;
		}

#endif	// _JAPAN_LOCAL_

#ifdef _HK_LOCAL_

		DWORD GivenInvenCount = TP_PYOGUK_START+(TABCELL_PYOGUK_NUM*GIVEN_PYOGUK_SLOT);
		if( pos >= GivenInvenCount )
		{
			DWORD ExtraSlot = pPlayer->GetPyogukNum()-GIVEN_PYOGUK_SLOT;
			if( ExtraSlot == 0 ||
				pos >= GivenInvenCount+(TABCELL_PYOGUK_NUM*ExtraSlot) )
				return FALSE;
		}

#endif	// _HK_LOCAL_

#ifdef _TL_LOCAL_

		DWORD GivenInvenCount = TP_PYOGUK_START+(TABCELL_PYOGUK_NUM*GIVEN_PYOGUK_SLOT);
		if( pos >= GivenInvenCount )
		{
			DWORD ExtraSlot = pPlayer->GetPyogukNum()-GIVEN_PYOGUK_SLOT;
			if( ExtraSlot == 0 ||
				pos >= GivenInvenCount+(TABCELL_PYOGUK_NUM*ExtraSlot) )
				return FALSE;
		}

#endif	// _TL_LOCAL_

		return TRUE;
	}
	else if(TP_GUILDWAREHOUSE_START <= pos && pos < TP_GUILDWAREHOUSE_END)
	{
		// 06.12.15 RaMa - 복구끝나면 원래대로 되돌려야 한다.
//		if( pos > 560 )
//			return FALSE;
		return TRUE;
	}
	else if(TP_SHOPITEM_START <= pos && pos < TP_SHOPITEM_END)
	{
		ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(wIconIdx);
		if(!pInfo) return FALSE;
		if( !(pInfo->ItemKind & eSHOP_ITEM) )
			return FALSE;
		return TRUE;
	}
	else if(TP_SHOPINVEN_START <= pos && pos < TP_SHOPINVEN_END)
	{		
		ITEM_INFO * pInfo = ITEMMGR->GetItemInfo(wIconIdx);
		if(!pInfo) return FALSE;
		if( !(pInfo->ItemKind & eSHOP_ITEM) )
			return FALSE;		
		return TRUE;
	}
	//!!!펫 소환 여부는 어디서 체크해줘야할까
	else if(TP_PETINVEN_START <= pos && pos < TP_PETINVEN_END)
	{
		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(wIconIdx);
		if(!pInfo) return FALSE;
		if( pInfo->ItemKind & eSHOP_ITEM )
			return FALSE;
		return TRUE;
	}
	else if(TP_PETWEAR_START <= pos && pos < TP_PETWEAR_END)
	{
		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(wIconIdx);
		if(!pInfo) return FALSE;
		if( pInfo->ItemKind == eSHOP_ITEM_PET_EQUIP )
			return TRUE;
		return FALSE;
	}
	// magi82 - Titan(070207)
	else if(TP_TITANWEAR_START <= pos && pos < TP_TITANWEAR_END)
	{
		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(wIconIdx);
		if(!pInfo) return FALSE;
		if( pInfo->ItemKind & eTITAN_EQUIPITEM )
		{
			if(pos - TP_TITANWEAR_START == pInfo->ItemKind - eTITAN_EQUIPITEM_HELMET)	// 병국씨 이게 뭐어어어어야~~
				return TRUE;
		}
		return FALSE;
	}
	// magi82 - Titan(070227)
	else if(TP_TITANSHOPITEM_START <= pos && pos < TP_TITANSHOPITEM_END)
	{
		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(wIconIdx);
		if(!pInfo) return FALSE;
		if( pInfo->ItemKind == eSHOP_ITEM_TITAN_EQUIP )
			return TRUE;

		return FALSE;
	}
	else
		return FALSE;
}

BOOL CItemManager::CanEquip(ITEM_INFO* pInfo, CPlayer* pPlayer)
{
	if(pInfo->LimitLevel > pPlayer->GetLevel())
	{
		if( pPlayer->GetShopItemStats()->EquipLevelFree )
		{
			int gap = pInfo->LimitLevel - pPlayer->GetLevel();
			gap -= pPlayer->GetShopItemStats()->EquipLevelFree;
			if( gap > 0 )
				return FALSE;
		}
		else
			return FALSE;
	}	
	// ¡¾U¡¤A.ⓒoIA￠￢,¨oE￠￢¨¡,A¨u¡¤A
	if(pInfo->LimitGenGol > pPlayer->GetGenGol())
	{
		return FALSE;
	}
	if(pInfo->LimitMinChub > pPlayer->GetMinChub())
	{
		return FALSE;
	}
	if(pInfo->LimitSimMek > pPlayer->GetSimMek())
	{
		return FALSE;
	}
	if(pInfo->LimitCheRyuk > pPlayer->GetCheRyuk())
	{
		return FALSE;
	}

	// stage
	if( pInfo->LimitJob != eItemStage_Normal )
	{
		if(!CHKRT->ItemStageOf(pPlayer, (BYTE)pInfo->LimitJob))
			return FALSE;
	}		

	return TRUE;	
}

void CItemManager::ItemMoveLog(POSTYPE FromPos, POSTYPE ToPos, CPlayer* pPlayer, ITEMBASE* pItem)
{
	DWORD FromID = 0;
	DWORD ToID = 0;
	WORD Logtype = 0;
	MONEYTYPE FromTotalMoney =0;
	MONEYTYPE ToTotalMoney = 0;
	char FromName[MAX_NAME_LENGTH+1];
	char ToName[MAX_NAME_LENGTH+1];

	if(TP_INVENTORY_START<= FromPos && FromPos < TP_WEAR_END)
	{
		FromID = pPlayer->GetID();
		FromTotalMoney = pPlayer->GetMoney();
		if(TP_PYOGUK_START <= ToPos && ToPos < TP_PYOGUK_END)
		{			
			ToID = pPlayer->GetUserID();
			ToTotalMoney = pPlayer->GetMoney(eItemTable_Pyoguk);
			Logtype = eLog_ItemMoveInvenToPyoguk;
			SafeStrCpy(FromName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
			SafeStrCpy(ToName, "#Pyoguk", MAX_NAME_LENGTH+1);
		}
		else if(TP_GUILDWAREHOUSE_START <= ToPos && ToPos < TP_GUILDWAREHOUSE_END)
		{
			ToID = pPlayer->GetGuildIdx();
			Logtype = eLog_ItemMoveInvenToGuild;
			SafeStrCpy(FromName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
			SafeStrCpy(ToName, "#Munpa", MAX_NAME_LENGTH+1);
		}
		else if(TP_PETINVEN_START <= ToPos && ToPos < TP_PETINVEN_END)
		{
			ToID = pPlayer->GetUserID();
			ToTotalMoney = pPlayer->GetMoney();
			Logtype = eLog_ItemMoveInvenToPetInven;
			SafeStrCpy(FromName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
			SafeStrCpy(ToName, "#PetInven", MAX_NAME_LENGTH+1);
		}
		else
			return;
	}
	else if(TP_PYOGUK_START <= FromPos && FromPos < TP_PYOGUK_END)
	{
		if(TP_INVENTORY_START<= ToPos && ToPos < TP_WEAR_END)
		{
			FromID = pPlayer->GetUserID();
			FromTotalMoney = pPlayer->GetMoney(eItemTable_Pyoguk);
			ToTotalMoney = pPlayer->GetMoney();
			ToID = pPlayer->GetID();
			Logtype = eLog_ItemMovePyogukToInven;
			
			SafeStrCpy(FromName, "#Pyoguk", MAX_NAME_LENGTH+1);
			SafeStrCpy(ToName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
		}
		else if(TP_PYOGUK_START<= ToPos && ToPos < TP_PYOGUK_END)
		{
			return;
		}
		else
		{
			ASSERT(0); return;			
		}

	}
	else if(TP_GUILDWAREHOUSE_START <= FromPos && FromPos < TP_GUILDWAREHOUSE_END)
	{
		if(TP_INVENTORY_START<= ToPos && ToPos < TP_WEAR_END)
		{
			FromID = pPlayer->GetGuildIdx();
			ToID = pPlayer->GetID();
			ToTotalMoney = pPlayer->GetMoney();
			Logtype = eLog_ItemMoveGuildToInven;
			SafeStrCpy(FromName, "#Munpa", MAX_NAME_LENGTH+1);
			SafeStrCpy(ToName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
		}
		else if(TP_GUILDWAREHOUSE_START <= ToPos && ToPos < TP_GUILDWAREHOUSE_END)
		{
			return;
		}
		else
		{
			ASSERT(0);
			return;
		}		
	}
	else if(TP_SHOPITEM_START <= FromPos && FromPos < TP_SHOPITEM_END)
	{
		if(TP_SHOPINVEN_START<= ToPos && ToPos < TP_SHOPINVEN_END)
		{
			FromID = pPlayer->GetID();
			ToID = pPlayer->GetID();
			ToTotalMoney = pPlayer->GetMoney();
			Logtype = eLog_ShopItemMoveToInven;
			SafeStrCpy(FromName, "#ShopHouse", MAX_NAME_LENGTH+1);
			SafeStrCpy(ToName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
		}
		else
		{
			return;
		}		
	}
	else if(TP_SHOPINVEN_START <= FromPos && FromPos < TP_SHOPINVEN_END)
	{
		if(TP_SHOPITEM_START<= ToPos && ToPos < TP_SHOPITEM_END)
		{
			FromID = pPlayer->GetID();
			ToID = pPlayer->GetID();
			ToTotalMoney = pPlayer->GetMoney();
			Logtype = eLog_ShopItemMoveToInven;
			SafeStrCpy(FromName, "#ShopInven", MAX_NAME_LENGTH+1);
			SafeStrCpy(ToName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
		}
		else
		{
			return;
		}		
	}
	else if(TP_PETINVEN_START <= FromPos && FromPos < TP_PETINVEN_END)
	{
		if(TP_INVENTORY_START <= FromPos && FromPos < TP_WEAR_END)
		{
			FromID = pPlayer->GetID();
			ToID = pPlayer->GetUserID();
			ToTotalMoney = pPlayer->GetMoney();
			Logtype = eLog_ItemMovePetInvenToInven;
			SafeStrCpy(FromName, "#PetInven", MAX_NAME_LENGTH+1);
			SafeStrCpy(ToName, pPlayer->GetObjectName(), MAX_NAME_LENGTH+1);
		}
		else
		{
			return;
		}
	}
	else 
		return; 

	LogItemMoney(FromID, FromName, ToID, ToName,
		Logtype, FromTotalMoney, ToTotalMoney, 0,
		pItem->wIconIdx, pItem->dwDBIdx,  FromPos, ToPos, pItem->Durability, pPlayer->GetPlayerExpPoint());
}

WORD CItemManager::GetWeaponKind( WORD wWeapon )
{
	WORD rt = 0;
	ITEM_INFO* pItemInfo = m_ItemInfoList.GetData( wWeapon );
	if( pItemInfo )
		rt = pItemInfo->WeaponType;

	return rt;	
}

void CItemManager::GetItemKindType( WORD wItemIdx, WORD* wKind, WORD* wType )
{
	ITEM_INFO* pItemInfo = m_ItemInfoList.GetData( wItemIdx );
	if( pItemInfo )
	{
		*wKind = pItemInfo->ItemKind;
		*wType = pItemInfo->ItemType;
	}
	else
	{
		*wKind = 0;
		*wType = 0;
	}
}

DWORD CItemManager::MakeNewKey()
{
	if(m_Key == 0)
		++m_Key;
	return m_Key++; 
}

void CItemManager::AddGuildItemOption(POSTYPE FromPos, POSTYPE ToPos, CPlayer* pPlayer)
{
	CItemSlot * pSlot;	
	const ITEMBASE * pItem;

	if(TP_GUILDWAREHOUSE_START <= ToPos && ToPos < TP_GUILDWAREHOUSE_END)
	{
		if(TP_GUILDWAREHOUSE_START <= FromPos && FromPos < TP_GUILDWAREHOUSE_END)
			return;
		pSlot = pPlayer->GetSlot(ToPos);
		pItem = pSlot->GetItemInfoAbs(ToPos);
		if(IsOptionItem(pItem->wIconIdx, pItem->Durability))
			GUILDMGR->AddGuildItemOption(pPlayer->GetItemOption(pItem->Durability));
		//SW050920 Rare
		if(IsRareOptionItem(pItem->wIconIdx, pItem->RareIdx))
			GUILDMGR->AddGuildItemRareOption(pPlayer->GetItemRareOption(pItem->RareIdx));
	}
}

void CItemManager::RemoveGuildItemOption(POSTYPE ToPos, const ITEMBASE* pItem, CPlayer* pPlayer)
{
	if(TP_GUILDWAREHOUSE_START <= ToPos && ToPos < TP_GUILDWAREHOUSE_END)
		return;
	if(IsOptionItem(pItem->wIconIdx, pItem->Durability))
	{
		ITEM_OPTION_INFO * pInfo = GUILDMGR->GetItemOption(pItem->Durability);
		pPlayer->AddItemOption(pInfo);
		GUILDMGR->RemoveGuildItemOption(pItem->Durability);
	}
	//SW050920 Rare
	if(IsRareOptionItem(pItem->wIconIdx, pItem->RareIdx))
	{
		ITEM_RARE_OPTION_INFO* pRareInfo = GUILDMGR->GetItemRareOption(pItem->RareIdx);
		pPlayer->AddItemRareOption(pRareInfo);
		GUILDMGR->RemoveGuildItemRareOption(pItem->RareIdx);
	}
}

int CItemManager::UseShopItem( CPlayer* pPlayer, SHOPITEMUSEBASE Info, SHOPITEMBASE* pShopItem )
{
	if(pPlayer->GetState() == eObjectState_Die)
		return eItemUseErr_Err;
	if(!CHKRT->ItemOf(pPlayer, Info.ShopItemPos, Info.ShopItemIdx,0,0,CB_EXIST|CB_ICONIDX))
		return eItemUseErr_Err;
	
	if(pPlayer->GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal );

	CItemSlot * pSlot = pPlayer->GetSlot(Info.ShopItemPos);
	if( pSlot && pSlot->IsLock(Info.ShopItemPos) )
		return eItemUseErr_Lock;
	const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(Info.ShopItemPos);
	ITEM_INFO * pItemInfo = GetItemInfo(Info.ShopItemIdx);

	if(pPlayer->IsHide())
	if(IsUnfitItemInfoListForHide(Info.ShopItemIdx))
	{
		return eItemUseErr_Err;
	}

	// RaMa - 04.11.04
	if( pItemInfo && pItemInfo->ItemKind & eSHOP_ITEM )
	{
		return pPlayer->GetShopItemManager()->UseShopItem( (ITEMBASE*)pItemBase, Info, pShopItem );
	}
	else
	{
		ASSERT(0);
		return eItemUseErr_Err;
	}

	return eItemUseSuccess;
}


BOOL CItemManager::IsUseAbleShopItem( CPlayer* pPlayer, WORD ItemIdx, POSTYPE ItemPos )
{
	if(pPlayer->GetState() == eObjectState_Die)
		return FALSE;
	if(!CHKRT->ItemOf(pPlayer, ItemPos, ItemIdx,0,0,CB_EXIST|CB_ICONIDX))
		return FALSE;
	
	if(pPlayer->GetState() == eObjectState_Immortal )
		OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal );

	CItemSlot * pSlot = pPlayer->GetSlot(ItemPos);
	if( pSlot && pSlot->IsLock(ItemPos) )
		return FALSE;

	const ITEMBASE* pItemBase = pSlot->GetItemInfoAbs( ItemPos );
	if(!pItemBase)		return FALSE;
	if(pItemBase->wIconIdx != ItemIdx)
		return FALSE;
	
	ITEM_INFO * pItemInfo = GetItemInfo(ItemIdx);
	if( pItemInfo && ( pItemInfo->ItemKind & eSHOP_ITEM ) )
	{
		return TRUE;
	}

	return FALSE;
}


void CItemManager::ShopItemUseUpgrade( ITEM_INFO* pShopItemInfo, CPlayer* pPlayer, POSTYPE ItemPos, WORD wItemIdx)
{
	if(!CHKRT->ItemOf(pPlayer, ItemPos, wItemIdx,0,0,CB_EXIST|CB_ICONIDX))
			return;
	const ITEMBASE* pSrcItem = ITEMMGR->GetItemInfoAbsIn( pPlayer, ItemPos );
	if(!pSrcItem)		return;

	DWORD tdw = pSrcItem->wIconIdx%10;
	if(tdw >= 9)
	{
		// 더이상 강화할 수 없음
		
		return;
	}

	DWORD plusoption = 0;
	// 각각의 확률을 적용하고,

	if( (plusoption+tdw) > 9 )
		plusoption = 1;

	wItemIdx = (WORD)(pSrcItem->wIconIdx + plusoption);


	// 새로운 아이템을 넣는다.
	CItemSlot * pSlot = pPlayer->GetSlot(ItemPos);
	WORD flag = UB_ICONIDX;		
	if( !IsDupItem( (WORD)(pSrcItem->wIconIdx+plusoption) ) )
		flag |= UB_DURA;

	if(EI_TRUE != pSlot->UpdateItemAbs( pPlayer, ItemPos, 0, (WORD)(pSrcItem->wIconIdx+plusoption),0,0,0, flag, SS_LOCKOMIT))
		return;

	const ITEMBASE * pItemBase = pSlot->GetItemInfoAbs(ItemPos);
	ItemUpdateToDB(pPlayer->GetID(), pItemBase->dwDBIdx, pItemBase->wIconIdx,
				pItemBase->Durability, pItemBase->Position, pItemBase->QuickPosition);

	LogItemMoney(pPlayer->GetID(), pPlayer->GetObjectName(), pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ShopItemUse_MixSuccess, 
		pPlayer->GetMoney(), pSrcItem->wIconIdx+plusoption, pPlayer->GetMoney(), pItemBase->wIconIdx, pItemBase->dwDBIdx, pItemBase->Position, pItemBase->Position, pItemBase->Durability,
		pPlayer->GetPlayerExpPoint());		

		/*
	// 마을 이동 주문서
	else if( pShopItemInfo->ItemGrade == 3 )
	{
		// dwParam -> 맵넘버
	}
	// 귀환 주문서
	else if( pShopItemInfo->ItemGrade == 4 )
	{
		// dwParam -> 맵넘버
	}
	// 방파 텔레포트 주문서
	else if( pShopItemInfo->ItemGrade == 5 )
	{
		// dwParam -> 맵넘버
	}
	// 문파 텔레포트 주문서
	else if( pShopItemInfo->ItemGrade == 6 )
	{
		// dwParam -> 맵넘버
	}
	// 돈의 축복
	else if( pShopItemInfo->ItemGrade == 8 )
	{
	}
	// 생명의 축복
	else if( pShopItemInfo->ItemGrade == 9 )
	{
	}
	// 조합 확률 향상 주문서
	else if( pShopItemInfo->ItemGrade == 10 )
	{
	}
	// 부활주문서
	else if( pShopItemInfo->ItemGrade == 11 )
	{
		// dwParam -> 부활 시키려는 상대방의 Idx
	}
	*/
}


// RaMa
BOOL CItemManager::ItemUnsealing(CPlayer* pPlayer, POSTYPE absPos)
{
	ITEMBASE* pItemBase = (ITEMBASE*)GetItemInfoAbsIn(pPlayer, absPos);
	if(!pItemBase)		return FALSE;

	CItemSlot* pItemSlot = pPlayer->GetSlot( absPos );	
	if( pItemSlot )
	if( pItemSlot->IsLock( absPos ) )
		return FALSE;
	
	//ITEMBASE ItemBase = *pItemBase;	
	if( pItemBase->ItemParam & ITEM_PARAM_SEAL )
	{
		ITEM_INFO* pItemInfo = GetItemInfo( pItemBase->wIconIdx );
		if( pItemInfo )
		{
			// 변형이나 꾸미기는 봉인을 해제함과 동시에 사용시작
			// 이동주문서 기간제도 봉인해제와 동시에 사용시작
			// 봉인풀면 사용가능한 아이템
			if( pItemInfo->ItemType == 11 )
			{
				SHOPITEMWITHTIME* pItem = NULL;

				if( pItemInfo->ItemKind != eSHOP_ITEM_PET )
				if( pItem = pPlayer->GetShopItemManager()->GetUsingItemInfo( pItemInfo->ItemIdx ) )
					return FALSE;

				if( pPlayer->GetShopItemManager()->IsDupAble( pItemInfo ) == FALSE )
					return FALSE;
				
				SHOPITEMBASE	ShopItem;
				memcpy( &ShopItem.ItemBase, pItemBase, sizeof(ITEMBASE) );

				SYSTEMTIME systime;
				stTIME startime, usetime, endtime;
				GetLocalTime(&systime);
				startime.SetTime(systime.wYear-2000, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, 0);
				
				endtime = startime;
				// 임시로
				if( pItemInfo->Rarity >= 525600 )	// 365일
				{
					DWORD day = pItemInfo->Rarity/(24*60) - 365;
					usetime.SetTime(1, 0, day, 0, 0, 0);
				}
				else
				{
					DWORD day = pItemInfo->Rarity/(24*60);
					DWORD hour = (pItemInfo->Rarity%(24*60))/60;
					DWORD minute = (pItemInfo->Rarity%(24*60))%60;

					usetime.SetTime(0, 0, day, hour, minute, 0);
				}

				endtime += usetime;

				ShopItem.BeginTime = startime;
				ShopItem.Remaintime = endtime.value;
				ShopItem.ItemBase.ItemParam ^= ITEM_PARAM_SEAL;

				//
				UsingShopItemUpdateToDB( 0, pPlayer->GetID(), ShopItem.ItemBase.wIconIdx, ShopItem.ItemBase.dwDBIdx, pItemInfo->SellPrice, ShopItem.BeginTime.value, ShopItem.Remaintime );
				//				
				ShopItem.Param = pItemInfo->SellPrice;
				pPlayer->GetShopItemManager()->AddShopItemUse( &ShopItem );
				pPlayer->GetShopItemManager()->AddDupParam( pItemInfo );

				if( pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION ||
					pItemInfo->ItemKind == eSHOP_ITEM_EQUIP )
				{					
					//
					SEND_SHOPITEM_USEONE msg;
					msg.Category = MP_ITEM;
					msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_USE;
					msg.ShopItemBase = ShopItem;
					pPlayer->SendMsg( &msg, sizeof(msg) );
				}				
				else
				{
					SEND_SHOPITEM_USEDINFO msg;
					msg.Category = MP_ITEM;
					msg.Protocol = MP_ITEM_SHOPITEM_USEDINFO;
					msg.ItemCount = 1;
					msg.Item[0] = ShopItem;					
					pPlayer->SendMsg( &msg, msg.GetSize() );
				}
				
				
				//
				LogItemMoney( pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
					eLog_ShopItemUse, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
					ShopItem.ItemBase.wIconIdx, ShopItem.ItemBase.dwDBIdx, ShopItem.ItemBase.Position, 0, ShopItem.ItemBase.Durability, pPlayer->GetPlayerExpPoint());
			}

			//SW051129 Pet
			/*
			if( pItemInfo->ItemKind == eSHOP_ITEM_PET )	//GM Cheat 로 인한 아이템몰 펫 아이템에 쓰인다.
			{	//봉인 해제시 첫 생성.
				//단! 재봉인 가능하므로 기존 펫 유무 확인
				//펫 ID 계산( 리턴받은 DBID + PET_ID_START )
				PET_TOTALINFO* pPetInfo = pPlayer->GetPetManager()->GetPetTotalInfo(pItemBase->dwDBIdx, eWithNULL );
				if( NULL == pPetInfo )
				{
					g_pServerSystem->CreateNewPet(pPlayer, pItemInfo->ItemIdx, pItemBase->dwDBIdx,  pItemInfo->Plus_MugongIdx);
				}
			}*/

			pItemBase->ItemParam ^= ITEM_PARAM_SEAL;			
			ShopItemParamUpdateToDB(pPlayer->GetID(), pItemBase->dwDBIdx, pItemBase->ItemParam);			

			return TRUE;
		}
		return FALSE;
	}
	else
		return FALSE;
}
BOOL CItemManager::CheckHackNpc( CPlayer* pPlayer, WORD wNpcIdx, WORD wParam )
{
	if( wParam == eNpcParam_ShowpPyoguk )
		return TRUE;

	//SW070626 보부상NPC
	if( wNpcIdx == BOBUSANG_wNpcUniqueIdx )
	{
		return BOBUSANGMGR->CheckHackBobusangDist(pPlayer);
	}
	else
	{
		STATIC_NPCINFO* pInfo = GAMERESRCMNGR->GetStaticNpcInfo( wNpcIdx );
		if( pInfo == NULL )	return FALSE;

		if( pInfo->MapNum != g_pServerSystem->GetMapNum() )
			return FALSE;

		VECTOR3 vPos;
		pPlayer->GetPosition( &vPos );
		float dist = CalcDistanceXZ( &vPos, &pInfo->vPos );
		if( dist > 3000.0f )	return FALSE;

		return TRUE;
	}
}


int CItemManager::CheatObtainShopItem( CPlayer* pPlayer, WORD obtainItemIdx, DWORD ItemNum )
{
	WORD EmptyCellPos[255];
	WORD EmptyCellNum;
	CItemSlot * pSlot = pPlayer->GetSlot(eItemTable_ShopInven);
	WORD obtainItemNum = GetCanBuyNumInSpace( pPlayer, pSlot, obtainItemIdx, ItemNum, EmptyCellPos, EmptyCellNum );
	if(obtainItemNum == 0)
		return 1;

	return ObtainItemEx( pPlayer, Alloc(pPlayer, MP_CHEAT, MP_CHEAT_ITEM_ACK, pPlayer->GetID(), 0, eLog_ItemObtainCheat, obtainItemNum, (DBResult)(ObtainItemDBResult)), obtainItemIdx, obtainItemNum, EmptyCellPos, EmptyCellNum, EmptyCellNum, 1 );
}

BOOL CItemManager::CheckHackItemMove( CPlayer* pPlayer, CItemSlot* pFromSlot, CItemSlot* pToSlot )
{
	if( pPlayer->GetShopItemManager()->GetUsingItemInfo( eIncantation_ShowPyoguk ) != NULL ||
		pPlayer->GetShopItemManager()->GetUsingItemInfo( eIncantation_ShowPyoguk7 ) != NULL ||
		pPlayer->GetShopItemManager()->GetUsingItemInfo( eIncantation_ShowPyoguk7_NoTrade ) != NULL )
		return TRUE;

	VECTOR3 vPos;
	pPlayer->GetPosition( &vPos );

	CItemSlot* pGuildSlot = pPlayer->GetSlot( eItemTable_MunpaWarehouse );
	CItemSlot* pWareSlot = pPlayer->GetSlot( eItemTable_Pyoguk );

	if( pFromSlot == pGuildSlot || pToSlot == pGuildSlot || pFromSlot == pWareSlot || pToSlot == pWareSlot )
	{
		if( !g_pServerSystem->GetMap()->IsVillage() )	return FALSE;
		if( !GAMERESRCMNGR->IsInWareNpcPosition( &vPos ) )	return FALSE;
	}

	return TRUE;
}

void CItemManager::SetItemOptionsInfoMsg( CPlayer* pItemOwner, ITEMBASE* pItemBase, MSG_LINKITEMOPTIONS* rtMsg )
{
	ITEM_OPTION_INFO*			pOptionInfo;
	ITEM_RARE_OPTION_INFO*		pRareOptionInfo;
	PET_TOTALINFO*				pPetTotalInfo;
	TITAN_TOTALINFO*			pTitanTotalInfo;
	TITAN_ENDURANCE_ITEMINFO*	pTitanEquipEnduranceInfo;

	static char msgTemp[256] = {0,};

	if( IsOptionItem(pItemBase->wIconIdx, pItemBase->Durability) )
	{
		pOptionInfo = pItemOwner->GetItemOption(pItemBase->Durability);

		if(!pOptionInfo)
		{
			sprintf(msgTemp, "Data Absence [OptionInfo] ! PlayerID : %d ItemDBIdx : %d NeedCheck\n"
				, pItemOwner->GetID(), pItemBase->dwDBIdx );
			ASSERTMSG(0, msgTemp);
		}
		else
		{
			rtMsg->eOptionKind |= eOPTS_ItemOption;
			rtMsg->AddableInfo.AddInfo( CAddableInfoList::ItemOption, sizeof(ITEM_OPTION_INFO), pOptionInfo );
		}
	}

	if( IsRareOptionItem(pItemBase->wIconIdx, pItemBase->RareIdx) )
	{
		pRareOptionInfo = pItemOwner->GetItemRareOption(pItemBase->RareIdx);

		if(!pRareOptionInfo)
		{
			sprintf(msgTemp, "Data Absence [RareOptionInfo] ! PlayerID : %d ItemDBIdx : %d NeedCheck\n"
				, pItemOwner->GetID(), pItemBase->dwDBIdx );
			ASSERTMSG(0, msgTemp);
		}
		else
		{
			rtMsg->eOptionKind |= eOPTS_ItemRareOption;
			rtMsg->AddableInfo.AddInfo( CAddableInfoList::ItemRareOption, sizeof(ITEM_RARE_OPTION_INFO), pRareOptionInfo );
		}
	}

	if( IsPetSummonItem(pItemBase->wIconIdx) )
	{
		pPetTotalInfo = pItemOwner->GetPetManager()->GetPetTotalInfo(pItemBase->dwDBIdx);

		if(!pPetTotalInfo)
		{
			sprintf(msgTemp, "Data Absence [PetTotalInfo] ! PlayerID : %d ItemDBIdx : %d NeedCheck\n"
				, pItemOwner->GetID(), pItemBase->dwDBIdx );
			ASSERTMSG(0, msgTemp);
		}
		else
		{
			rtMsg->eOptionKind |= eOPTS_PetTotalInfo;
			rtMsg->AddableInfo.AddInfo( CAddableInfoList::PetTotalInfo, sizeof(PET_TOTALINFO), pPetTotalInfo );
		}

	}

	if( IsTitanCallItem(pItemBase->wIconIdx) )
	{
		pTitanTotalInfo = pItemOwner->GetTitanManager()->GetTitanTotalInfo(pItemBase->dwDBIdx);

		if(!pTitanTotalInfo)
		{
			sprintf(msgTemp, "Data Absence [TitanTotalInfo] ! PlayerID : %d ItemDBIdx : %d NeedCheck\n"
				, pItemOwner->GetID(), pItemBase->dwDBIdx );
			ASSERTMSG(0, msgTemp);
		}
		else
		{
			rtMsg->eOptionKind |= eOPTS_TitanTotalInfo;
			rtMsg->AddableInfo.AddInfo( CAddableInfoList::TitanTotalInfo, sizeof(TITAN_TOTALINFO), pTitanTotalInfo );
		}
	}

	if( IsTitanEquipItem(pItemBase->wIconIdx) )
	{
		pTitanEquipEnduranceInfo = pItemOwner->GetTitanManager()->GetTitanItemEnduranceInfo(pItemBase->dwDBIdx);

		if(!pTitanEquipEnduranceInfo)
		{
			sprintf(msgTemp, "Data Absence [TitanEquipEnduranceInfo] ! PlayerID : %d ItemDBIdx : %d NeedCheck\n"
				, pItemOwner->GetID(), pItemBase->dwDBIdx );
			ASSERTMSG(0, msgTemp);
		}
		else
		{
			rtMsg->eOptionKind |= eOPTS_TitanEquipEndurance;
			rtMsg->AddableInfo.AddInfo( CAddableInfoList::TitanEndrncInfo, sizeof(TITAN_ENDURANCE_ITEMINFO), pTitanEquipEnduranceInfo );
		}
	}

}

///// 2007. 6. 8. CBH - 세트아이탬 관련 함수 추가 ///////////////////////////
BOOL CItemManager::LoadSetItemOption()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if(!file.Init("Resource/setitem_opt.bin", "rb"))
		return FALSE;
#else
	if(!file.Init("Resource/setitem_opt.txt", "rt"))
		return FALSE;
#endif	// _FILE_BIN_

	SET_ITEM_OPTION* pInfo = NULL;
	while(!file.IsEOF())
	{
		ASSERT(!pInfo);
		pInfo = new SET_ITEM_OPTION;

		pInfo->wIndex = file.GetWord();
		pInfo->wSetItemKind = file.GetWord();
		SafeStrCpy( pInfo->szSetItemName, file.GetString(), MAX_NAME_LENGTH+1 );

		pInfo->wGenGol = file.GetWord();
		pInfo->wMinChub = file.GetWord();
		pInfo->wCheRyuk = file.GetWord();
		pInfo->wSimMek = file.GetWord();
		pInfo->dwLife = file.GetDword();
		pInfo->dwShield = file.GetDword();
		pInfo->dwNaeRyuk = file.GetWord();

		pInfo->AttrRegist.SetElement_Val(ATTR_FIRE,file.GetFloat());
		pInfo->AttrRegist.SetElement_Val(ATTR_WATER,file.GetFloat());
		pInfo->AttrRegist.SetElement_Val(ATTR_TREE,file.GetFloat());
		pInfo->AttrRegist.SetElement_Val(ATTR_IRON,file.GetFloat());
		pInfo->AttrRegist.SetElement_Val(ATTR_EARTH,file.GetFloat());
		pInfo->wLimitLevel = file.GetWord();
		SafeStrCpy( pInfo->szEffect, file.GetString(), 100 );
		pInfo->wLimitGenGol = file.GetWord();
		pInfo->wLimitMinChub = file.GetWord();
		pInfo->wLimitCheRyuk = file.GetWord();
		pInfo->wLimitSimMek = file.GetWord();

		pInfo->wMeleeAttackMin = file.GetWord();
		pInfo->wMeleeAttackMax = file.GetWord();
		pInfo->wRangeAttackMin = file.GetWord();
		pInfo->wRangeAttackMax = file.GetWord();
		pInfo->wCriticalPercent = file.GetWord();        		
		pInfo->AttrAttack.SetElement_Val(ATTR_FIRE,file.GetFloat());
		pInfo->AttrAttack.SetElement_Val(ATTR_WATER,file.GetFloat());
		pInfo->AttrAttack.SetElement_Val(ATTR_TREE,file.GetFloat());
		pInfo->AttrAttack.SetElement_Val(ATTR_IRON,file.GetFloat());
		pInfo->AttrAttack.SetElement_Val(ATTR_EARTH,file.GetFloat());

		pInfo->wPhyDef = file.GetWord();
		pInfo->wLifeRecover = file.GetWord();
		pInfo->fLifeRecoverRate = file.GetFloat();
		pInfo->wNaeRyukRecover = file.GetWord();
		pInfo->fNaeRyukRecoverRate = file.GetFloat();
		pInfo->wSetValue = file.GetWord();
		pInfo->wApplicationValue = file.GetWord();

		ASSERT(!m_SetItemOptionList.GetData(pInfo->wIndex));

		m_SetItemOptionList.Add(pInfo, pInfo->wIndex);
		pInfo = NULL;
	}
	file.Release();

	return TRUE;
}

SET_ITEM_OPTION* CItemManager::GetSetItemOption(WORD wSetItemKind, WORD wSetValue)
{
	SET_ITEM_OPTION* pSetItemOption = NULL;

	m_SetItemOptionList.SetPositionHead();
	while(pSetItemOption = m_SetItemOptionList.GetData())
	{
		if(pSetItemOption->wSetItemKind == wSetItemKind)
		{
			if(wSetValue == pSetItemOption->wSetValue)
			{				
				return pSetItemOption;
			}
		}
	}

	return NULL;
}

void CItemManager::RemoveSetItemOption(WORD wIndex, SET_ITEM_OPTION* pSetItemOptionOut)
{
	SET_ITEM_OPTION* pInfo = m_SetItemOptionList.GetData(wIndex);

	if( pInfo == NULL )
	{
		ASSERT(0);		
		return;
	}

	if(pSetItemOptionOut)
		*pSetItemOptionOut = *pInfo;
	SAFE_DELETE( pInfo );
	m_SetItemOptionList.Remove(wIndex);
}

CYHHashTable<SET_ITEM_OPTION>* CItemManager::GetSetItemOptionList()
{
	return &m_SetItemOptionList;
}
/////////////////////////////////////////////////////////////////////////////

BOOL CItemManager::CheckDemandItem( CPlayer* pPlayer, MSG_ITEM_BUY_SYN* pmsg )
{
	BOOL bNotEnough = FALSE;
	if( BOBUSANG_wNpcUniqueIdx == pmsg->wDealerIdx )
	{
		for( WORD i = 0; i < pmsg->slotNum; ++i )
		{
			if(!CHKRT->ItemOf(pPlayer, 
				pmsg->demandItem[i].demandItemPos,
				pmsg->demandItem[i].demandItemIdx,
				pmsg->demandItem[i].demandItemDur,
				0,
				CB_EXIST|CB_ICONIDX)
				)
				bNotEnough |= TRUE;
		}
	}

	if(bNotEnough)
		return FALSE;
	else	// 소모하라.
	{
		BYTE rt = FALSE;
		for( WORD i = 0; i < pmsg->slotNum; ++i )
		{
			//if( EI_TRUE == DiscardItem(pPlayer, pmsg->demandItem[i].demandItemPos, pmsg->demandItem[i].demandItemIdx, pmsg->demandItem[i].demandItemDur) )
			rt = UseItem(pPlayer, pmsg->demandItem[i].demandItemPos, pmsg->demandItem[i].demandItemIdx);
			if( eItemUseSuccess == rt )
			{
				MSG_ITEM_USE_ACK msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_USE_ACK;		//MP_ITEM_DISCARD_ACK 은 갯수 지정이 안되어 중복 아이템이 요구아이템일 때 부적절하다.
				msg.TargetPos = pmsg->demandItem[i].demandItemPos;
				msg.wItemIdx = pmsg->demandItem[i].demandItemIdx;
				SendAckMsg(pPlayer, &msg, sizeof(msg));
			}
			else
			{
				ASSERT(0);
				return FALSE;
			}
		}
	}

	return TRUE;
}

// 2007. 10. 15. CBH - 무기 장착 슬롯의 무기와 장착할 샾아이탬과 체크 ////////////
BOOL CItemManager::CheckWeaponToShopItem(CPlayer* pPlayer, WORD wItemIndex)
{	
	ITEM_INFO * pItemInfo = GetItemInfo(wItemIndex);

	//아이탬이 무기이면
	if(pItemInfo->WeaponType != 0)
	{
		const ITEMBASE * pItem = GetItemInfoAbsIn(pPlayer, TP_WEAR_START+1);
		
		if(pItem->wIconIdx == 0) //장착무기가 없다.
		{
			return FALSE;
		}
	}	

	return TRUE;
}


WORD CItemManager::ObtainItemFromGameEvent( CPlayer* pPlayer, DWORD dwItemIdx, WORD wItemNum )
{
	ITEM_INFO* pItemInfo = GetItemInfo( dwItemIdx );
	if( NULL == pItemInfo )
		return 0;			// 아이템이 없다.
	
	CItemSlot* pSlot = NULL;
	WORD wSeal = 0;
	if( pItemInfo->ItemKind & eSHOP_ITEM )	// 얻는 아이템이 shopitem이냐?
	{		
		pSlot = pPlayer->GetSlot(eItemTable_ShopInven);
		wSeal = 1;
	}
	else
	{
        pSlot = pPlayer->GetSlot(eItemTable_Inventory);
	}

	WORD wEmptyCellPos[255] = {0};
	WORD wEmptyCellNum;

	WORD wCanObtainItemNum = GetCanBuyNumInSpace( pPlayer, pSlot, dwItemIdx, wItemNum, wEmptyCellPos, wEmptyCellNum );
	if( 0 == wCanObtainItemNum )
	{
		//빈 슬롯 없음 경고 메시지
		MSGBASE msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_FULLINVEN_ALERT;
		pPlayer->SendMsg( &msg, sizeof(msg) );
		return 0;			// 빈공간이 없다.
	}

	ObtainItemEx( pPlayer, Alloc(pPlayer, MP_ITEM, MP_ITEM_MONSTER_OBTAIN_NOTIFY,
        pPlayer->GetID(), 0, eLog_ItemObtainFromGameEvent, wCanObtainItemNum, (DBResult)(ObtainItemDBResult)),
        dwItemIdx, wCanObtainItemNum, wEmptyCellPos, wEmptyCellNum, wEmptyCellNum, wSeal );

	return wCanObtainItemNum;
}
//////////////////////////////////////////////////////////////////////////////////
