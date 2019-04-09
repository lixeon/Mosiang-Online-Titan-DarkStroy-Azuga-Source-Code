// ShopItemManager.cpp: implementation of the CShopItemManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserTable.h"
#include "ItemManager.h"
#include "player.h"
#include "ShopItemManager.h"
#include "..\[CC]Header\GameResourceStruct.h"
#include "MapDBMsgParser.h"
#include "AbilityManager.h"
#include "PackedData.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "StatsCalcManager.h"
#include "EventMapMgr.h"
#include "WantedManager.h"
#include "Battle.h"
#include "SiegeWarMgr.h"
#include "WeatherManager.h"
#include "ChangeItemMgr.h"
#include "QuestMapMgr.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern BOOL g_bPlusTime;


CShopItemManager::CShopItemManager()
{
	m_UsingItemPool = new CMemoryPoolTempl<SHOPITEMWITHTIME>;
	m_UsingItemPool->Init( 50, 10, "ShopItemPool" );

	m_MovePointPool = new CMemoryPoolTempl<MOVEDATA>;
	m_MovePointPool->Init( 50, 10, "MovePointPool" );

	m_UsingItemTable.Initialize( 50 );
	m_MovePointTable.Initialize( 30 );
}

CShopItemManager::~CShopItemManager()
{
	// magi82(14) - Memory(071027) 메모리 해제 누락된 부분 수정
	SAFE_DELETE( m_UsingItemPool );
	SAFE_DELETE( m_MovePointPool );
}


void CShopItemManager::Init(CPlayer* pPlayer)
{	
	m_pPlayer = pPlayer;
	m_Updatetime = 0;
	m_Checktime = 0;
	m_DupIncantation = 0;
	m_DupCharm = 0;
	m_DupHerb = 0;
	m_DupSundries = 0;
	m_DupPetEquip = 0;
	m_ProtectItemIdx = 0;
}


void CShopItemManager::Release()
{
	m_UsingItemTable.SetPositionHead();
	SHOPITEMWITHTIME* pShopItem = NULL;
	while( pShopItem = m_UsingItemTable.GetData() )
	{
		m_UsingItemPool->Free( pShopItem );
	}
	m_UsingItemTable.RemoveAll();

	m_MovePointTable.SetPositionHead();
	MOVEDATA* pData = NULL;
	while( pData = m_MovePointTable.GetData() )
	{
		m_MovePointPool->Free( pData );
	}
	m_MovePointTable.RemoveAll();
}
/*
int CShopItemManager::UseShopItem( ITEMBASE* pItemBase, SHOPITEMUSEBASE UseBaseInfo, SHOPITEMBASE* pShopItem )
{
	if( !pItemBase )
		return eItemUseErr_Err;
	
	if( EVENTMAPMGR->IsEventMap() )
		return eItemUseErr_Err;

	// 보스몹이 있는 맵은 기억석으로 저장 할 수 없음.
	if( pItemBase->wIconIdx == eIncantation_MemoryStone &&
		( g_pServerSystem->GetMapNum() == BOSSMONSTER_MAP 		
		|| g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetSiegeMapNum()
		|| g_pServerSystem->GetMapNum() == QUESTMAPNUM1
		|| g_pServerSystem->GetMapNum() == QUESTMAPNUM2
		|| g_pServerSystem->GetMapNum() == QUESTMAPNUM3
		|| g_pServerSystem->GetMapNum() == BOSSMONSTER_2NDMAP 
		|| g_pServerSystem->GetMapNum() == RUNNINGMAP
		|| g_pServerSystem->GetMapNum() == PKEVENTMAP
		|| g_pServerSystem->CheckMapKindIs(eSurvival) ) )
		return eItemUseErr_Err;


	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(UseBaseInfo.ShopItemIdx);
	if(!pItemInfo)		return eItemUseErr_Err;

	//2007. 9. 21. CBH - 샾 아이탬 레벨제한 체크 추가
	if(pItemInfo->NaeRyukRecoverRate > m_pPlayer->GetLevel())
	{
		return eItemUseErr_Err;
	}
	

	// 05.05.20  문파토너먼트시에 사용불가능 아이템
	if( g_pServerSystem->GetMapNum() == Tournament )
	{
		if( pItemBase->wIconIdx == eIncantation_MemoryStone ||
			(pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION && pItemInfo->LimitLevel) )
			return eItemUseErr_Err;
	}

	  

	//공성전에서는 기억석 사용 못함
	if( m_pPlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
	{
		if( pItemBase->wIconIdx == eIncantation_MemoryStone )
			return eItemUseErr_Err;

		if( m_pPlayer->GetBattleTeam() == 2 ) //관람모드
		{
			if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION && pItemInfo->LimitLevel )
				return eItemUseErr_Err;
		}
	}

*/
//	임시로 넣음 - 성대
/*	SHOPITEMWITHTIME* pAddShopItem = NULL;
	if( pItemBase->wIconIdx == 55134 || pItemBase->wIconIdx == 55142 )
	{
		pAddShopItem = m_UsingItemTable.GetData(55134);
		if( !pAddShopItem )
		{
			pAddShopItem = m_UsingItemTable.GetData(55142);
		}
	}
	else
	{
        pAddShopItem = m_UsingItemTable.GetData(pItemBase->wIconIdx);
	}*/
//
/*
	SHOPITEMWITHTIME* pAddShopItem = NULL;
	pAddShopItem = m_UsingItemTable.GetData(pItemBase->wIconIdx);

	if( pAddShopItem )
	{
		if( pItemInfo->SellPrice && pItemInfo->ItemType == 10 )
		{

			// 개인플러스타임 체크
			if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM && pItemInfo->MeleeAttackMin )
			{
				if( pAddShopItem->ShopItem.Remaintime )
					return eItemUseErr_AlreadyUse;
				else
				{
					/*
					stTIME ct;
					ct.value = GetCurTime();
					if( pAddShopItem->ShopItem.BeginTime.GetYear() == ct.GetYear() && 
						pAddShopItem->ShopItem.BeginTime.GetMonth() == ct.GetMonth() &&
						pAddShopItem->ShopItem.BeginTime.GetDay() == ct.GetDay() )
						return eItemuseErr_DontUseToday;
					else
					{
						// 기존에 사용했던 개인플러스타임 삭제
						ShopItemDeleteToDB( m_pPlayer->GetID(), pAddShopItem->ShopItem.ItemBase.dwDBIdx );
						m_UsingItemTable.Remove( pAddShopItem->ShopItem.ItemBase.wIconIdx );
						m_UsingItemPool->Free( pAddShopItem );
						pAddShopItem = NULL;
					}
					*/
/*					// 기존에 사용했던 개인플러스타임 삭제
					ShopItemDeleteToDB( m_pPlayer->GetID(), pAddShopItem->ShopItem.ItemBase.dwDBIdx );
					m_UsingItemTable.Remove( pAddShopItem->ShopItem.ItemBase.wIconIdx );
					m_UsingItemPool->Free( pAddShopItem );
					pAddShopItem = NULL;
				}				
			}
#ifdef _JAPAN_LOCAL_
			else if( pItemInfo->ItemIdx == eIncantation_InvenExtend && m_pPlayer->GetExtraInvenSlot() >= 2 )
				return eItemUseErr_AlreadyUse;
			else if( pItemInfo->ItemIdx == eIncantation_PyogukExtend && m_pPlayer->GetExtraPyogukSlot() >= EXTRA_PYOGUK_SLOT )
				return eItemUseErr_AlreadyUse;
			else if( pItemInfo->ItemIdx == eIncantation_MugongExtend && m_pPlayer->GetExtraMugongSlot() >= 2)
				return eItemUseErr_AlreadyUse;
			else if( pItemInfo->ItemIdx != eIncantation_InvenExtend &&
				pItemInfo->ItemIdx != eIncantation_PyogukExtend &&
				pItemInfo->ItemIdx != eIncantation_MugongExtend)
				return eItemUseErr_AlreadyUse;
		}
#elif defined _HK_LOCAL_
			else if( (pItemInfo->ItemIdx == eIncantation_InvenExtend || pItemInfo->ItemIdx == eIncantation_InvenExtend2) &&
				m_pPlayer->GetExtraInvenSlot() >= 2 )
				return eItemUseErr_AlreadyUse;
			else if( (pItemInfo->ItemIdx == eIncantation_PyogukExtend || pItemInfo->ItemIdx == eIncantation_PyogukExtend2) &&
				m_pPlayer->GetExtraPyogukSlot() >= 3 )
				return eItemUseErr_AlreadyUse;
			else if( (pItemInfo->ItemIdx == eIncantation_MugongExtend || pItemInfo->ItemIdx == eIncantation_MugongExtend2) &&
				m_pPlayer->GetExtraMugongSlot() >= 2)
				return eItemUseErr_AlreadyUse;
			else if( (pItemInfo->ItemIdx == eIncantation_CharacterSlot || pItemInfo->ItemIdx == eIncantation_CharacterSlot2) &&
				m_pPlayer->GetExtraCharacterSlot() >= 3 )
				return eItemUseErr_UseFull;
			else if( pItemInfo->ItemIdx != eIncantation_InvenExtend && pItemInfo->ItemIdx != eIncantation_InvenExtend2 &&
				pItemInfo->ItemIdx != eIncantation_PyogukExtend && pItemInfo->ItemIdx != eIncantation_PyogukExtend2 &&
				pItemInfo->ItemIdx != eIncantation_MugongExtend && pItemInfo->ItemIdx != eIncantation_MugongExtend2 )
				return eItemUseErr_AlreadyUse;
		}
#elif defined _TL_LOCAL_
			else if( pItemInfo->ItemIdx == eIncantation_InvenExtend && m_pPlayer->GetExtraInvenSlot() >= 2 )
				return eItemUseErr_UseFull;
			else if( pItemInfo->ItemIdx == eIncantation_PyogukExtend && m_pPlayer->GetExtraPyogukSlot() >= 3 )
				return eItemUseErr_UseFull;
			else if( pItemInfo->ItemIdx == eIncantation_MugongExtend && m_pPlayer->GetExtraMugongSlot() >= 2)
				return eItemUseErr_UseFull;
			else if( pItemInfo->ItemIdx == eIncantation_CharacterSlot && m_pPlayer->GetExtraCharacterSlot() >= 3 )
				return eItemUseErr_UseFull;
			else if( pItemInfo->ItemIdx != eIncantation_InvenExtend &&
				pItemInfo->ItemIdx != eIncantation_PyogukExtend &&
				pItemInfo->ItemIdx != eIncantation_MugongExtend &&
				pItemInfo->ItemIdx != eIncantation_CharacterSlot )
				return eItemUseErr_AlreadyUse;
		}

#else

			else
				return eItemUseErr_AlreadyUse;
		}
#endif		
	}
#ifdef _HK_LOCAL_
	else
	{
		if( (pItemInfo->ItemIdx == eIncantation_InvenExtend || pItemInfo->ItemIdx == eIncantation_InvenExtend2) &&
			m_pPlayer->GetExtraInvenSlot() >= 2 )
			return eItemUseErr_AlreadyUse;
		else if( (pItemInfo->ItemIdx == eIncantation_PyogukExtend || pItemInfo->ItemIdx == eIncantation_PyogukExtend2) &&
			m_pPlayer->GetExtraPyogukSlot() >= 3 )
			return eItemUseErr_AlreadyUse;
		else if( (pItemInfo->ItemIdx == eIncantation_MugongExtend || pItemInfo->ItemIdx == eIncantation_MugongExtend2) &&
			m_pPlayer->GetExtraMugongSlot() >= 2)
			return eItemUseErr_AlreadyUse;
		else if( (pItemInfo->ItemIdx == eIncantation_CharacterSlot || pItemInfo->ItemIdx == eIncantation_CharacterSlot2) &&
			m_pPlayer->GetExtraCharacterSlot() >= 3 )
			return eItemUseErr_UseFull;
	}
#endif
  /*	if( pItemBase->wIconIdx == eIncantation_MemoryMove15 || 
  		pItemBase->wIconIdx == 55357 || pItemBase->wIconIdx == 55362 )
  	{
  		if( pAddShopItem )
  			goto Not_DupAbleCheck;
  	}*/
/*
	// 중복사용 체크
	if( pItemBase->wIconIdx != 55353 && pItemBase->wIconIdx != 57504 &&
		pItemBase->wIconIdx != 57505 )
	{
		if( IsDupAble( pItemInfo ) == FALSE )
			return eItemUseErr_DontDupUse;	
	}

//Not_DupAbleCheck:

	if( !pAddShopItem )
	{
		pAddShopItem = m_UsingItemPool->Alloc();	
		pAddShopItem->ShopItem.ItemBase = *pItemBase;
		pAddShopItem->ShopItem.Param = pItemInfo->SellPrice;
	}

	// 사용기간이 있는 아이템 Rarity()
	if( pAddShopItem->ShopItem.Param && pItemInfo->ItemType == 10 )
	{
		SYSTEMTIME systime;
		stTIME startime, usetime, endtime;
		GetLocalTime(&systime);
		startime.SetTime(systime.wYear-2000, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, 0);
		pAddShopItem->ShopItem.BeginTime = startime;
		
		if( pItemInfo->SellPrice == eShopItemUseParam_Realtime )
		{
			endtime = startime;
			DWORD day = pItemInfo->Rarity/(24*60);
			DWORD hour = (pItemInfo->Rarity%(24*60))/60;
			DWORD minute = (pItemInfo->Rarity%(24*60))%60;
			usetime.SetTime(0, 0, day, hour, minute, 0);
			endtime += usetime;	
			
			pAddShopItem->ShopItem.Remaintime = endtime.value;
			pAddShopItem->LastCheckTime = gCurTime;
		}
		else if( pItemInfo->SellPrice == eShopItemUseParam_Playtime )
		{
			DWORD remaintime = 0;
			remaintime = pItemInfo->Rarity*60000;
			pAddShopItem->ShopItem.Remaintime = remaintime;
			pAddShopItem->LastCheckTime = gCurTime;
		}
		else if( pItemInfo->SellPrice == eShopItemUseParam_Continue )
		{
			pAddShopItem->ShopItem.Remaintime = 0;
			pAddShopItem->LastCheckTime = 0;
		}
	}
	
	// 사용하면 바로 없어지는 아이템
	if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
	{
		if(EI_TRUE != ITEMMGR->DiscardItem( m_pPlayer, UseBaseInfo.ShopItemPos, UseBaseInfo.ShopItemIdx, 1 ))
		{
			return eItemUseErr_Err;
		}

		// 물약
		MSG_DWORD2	msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_EFFECT;
		msg.dwData1 = m_pPlayer->GetID();

		if( pItemInfo->GenGol > 0 )
			msg.dwData2 = eEffect_ShopItem_Life;
		else if( pItemInfo->MinChub > 0 )
			msg.dwData2 = eEffect_ShopItem_Shield;
		else if( pItemInfo->CheRyuk > 0 )
			msg.dwData2 = eEffect_ShopItem_NaeRuyk;
		else if( pItemInfo->Life > 0 )
			msg.dwData2 = eEffect_ShopItem_MaxLife;
		else if( pItemInfo->Shield > 0 )
			msg.dwData2 = eEffect_ShopItem_MaxShield;
		else if( pItemInfo->NaeRyuk > 0 )
			msg.dwData2 = eEffect_ShopItem_MaxNaeRuyk;
		else
			msg.dwData2 = 0;
		
		if( msg.dwData2 )
			PACKEDDATA_OBJ->QuickSend( m_pPlayer, &msg, sizeof(msg) );	


		// 순간회복
		if( pItemInfo->SellPrice == 0 )
		{
			if( pItemInfo->GenGol > 0 )
				m_pPlayer->AddLife( pItemInfo->GenGol, NULL );
			if( pItemInfo->MinChub > 0 )
				m_pPlayer->AddShield( pItemInfo->MinChub );
			if( pItemInfo->CheRyuk > 0 )
				m_pPlayer->AddNaeRyuk( pItemInfo->CheRyuk, NULL );
		}
		else
		{
			CalcShopItemOption( pAddShopItem->ShopItem.ItemBase.wIconIdx, TRUE );
			m_pPlayer->CalcState();
			UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), pAddShopItem->ShopItem.ItemBase.wIconIdx, pAddShopItem->ShopItem.ItemBase.dwDBIdx, pItemInfo->SellPrice, pAddShopItem->ShopItem.BeginTime.value, pAddShopItem->ShopItem.Remaintime );
			//
			m_UsingItemTable.Add(pAddShopItem, pAddShopItem->ShopItem.ItemBase.wIconIdx);
		}
	}
	// 주문서
	else if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION )
	{
		if( pItemInfo->ItemType == 10 )
		{
			// 레벨 제한 해제
			if( pItemInfo->LimitJob )
			{
				if( pItemInfo->LimitGender == 0 && m_pPlayer->GetLevel() > 50 )
					return eItemUseErr_Err;
				else if( pItemInfo->LimitGender == 1 && (m_pPlayer->GetLevel() < 51 || m_pPlayer->GetLevel() > 70 ) )
					return eItemUseErr_Err;
				else if( pItemInfo->LimitGender == 2 && (m_pPlayer->GetLevel() < 71 || m_pPlayer->GetLevel() > 90 ) )
					return eItemUseErr_Err;
			}

			if(EI_TRUE != ITEMMGR->DiscardItem( m_pPlayer, UseBaseInfo.ShopItemPos, UseBaseInfo.ShopItemIdx, 1 ))
				return eItemUseErr_Err;			

			// 돈+경험치 보호, 레벨 제한 해제
			if( pItemInfo->CheRyuk || pItemInfo->LimitJob )
			{
				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
				UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), pAddShopItem->ShopItem.ItemBase.wIconIdx, pAddShopItem->ShopItem.ItemBase.dwDBIdx, pItemInfo->CheRyuk, pAddShopItem->ShopItem.BeginTime.value, pAddShopItem->ShopItem.Remaintime );
				m_UsingItemTable.Add(pAddShopItem, pAddShopItem->ShopItem.ItemBase.wIconIdx);
			}
#ifdef _JAPAN_LOCAL_
			//창고확장, 인벤확장
			else if( pItemInfo->ItemIdx == eIncantation_InvenExtend )
			{
				if( m_pPlayer->GetExtraInvenSlot() )
				{
					pAddShopItem->ShopItem.Param = m_pPlayer->GetExtraInvenSlot() + 1;
					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pItemInfo->ItemIdx, pAddShopItem->ShopItem.Param );
				}
				else
				{
					UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), pAddShopItem->ShopItem.ItemBase.wIconIdx, pAddShopItem->ShopItem.ItemBase.dwDBIdx, 1, pAddShopItem->ShopItem.BeginTime.value, pAddShopItem->ShopItem.Remaintime );
					m_UsingItemTable.Add(pAddShopItem, pAddShopItem->ShopItem.ItemBase.wIconIdx);
				}
				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
			}
			else if( pItemInfo->ItemIdx == eIncantation_PyogukExtend )
			{
				if( m_pPlayer->GetExtraPyogukSlot() )
				{
					pAddShopItem->ShopItem.Param = m_pPlayer->GetExtraPyogukSlot() + 1;
					ShopItemUseParamUpdateToDB( m_pPlayer->GetUserID(), 0, pItemInfo->ItemIdx, pAddShopItem->ShopItem.Param );
				}
				else
				{
					UsingShopItemUpdateToDB( m_pPlayer->GetUserID(), 0, pAddShopItem->ShopItem.ItemBase.wIconIdx, pAddShopItem->ShopItem.ItemBase.dwDBIdx, 1, pAddShopItem->ShopItem.BeginTime.value, pAddShopItem->ShopItem.Remaintime );
					m_UsingItemTable.Add(pAddShopItem, pAddShopItem->ShopItem.ItemBase.wIconIdx);
				}
				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
			}
			else if( pItemInfo->ItemIdx == eIncantation_MugongExtend )
			{
				if( m_pPlayer->GetExtraMugongSlot() )
				{
					pAddShopItem->ShopItem.Param = m_pPlayer->GetExtraMugongSlot() + 1;
					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pItemInfo->ItemIdx, pAddShopItem->ShopItem.Param );				
				}
				else
				{
					UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), pAddShopItem->ShopItem.ItemBase.wIconIdx, pAddShopItem->ShopItem.ItemBase.dwDBIdx, 1, pAddShopItem->ShopItem.BeginTime.value, pAddShopItem->ShopItem.Remaintime );
					m_UsingItemTable.Add(pAddShopItem, pAddShopItem->ShopItem.ItemBase.wIconIdx);					
				}
				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
			}
			else if( pItemInfo->ItemIdx == eIncantation_CharacterSlot )
			{
				if( m_pPlayer->GetExtraCharacterSlot() )
				{
					pAddShopItem->ShopItem.Param = m_pPlayer->GetExtraCharacterSlot() + 1;
					ShopItemUseParamUpdateToDB( m_pPlayer->GetUserID(), 0, pItemInfo->ItemIdx, pAddShopItem->ShopItem.Param );
					m_pPlayer->SetExtraCharacterSlot( pAddShopItem->ShopItem.Param );
				}
				else
				{
					UsingShopItemUpdateToDB( m_pPlayer->GetUserID(), 0, pAddShopItem->ShopItem.ItemBase.wIconIdx, pAddShopItem->ShopItem.ItemBase.dwDBIdx, 1, pAddShopItem->ShopItem.BeginTime.value, pAddShopItem->ShopItem.Remaintime );
					m_UsingItemTable.Add(pAddShopItem, pAddShopItem->ShopItem.ItemBase.wIconIdx);
					m_pPlayer->SetExtraCharacterSlot( 1 );
				}
			}
#elif defined _HK_LOCAL_
			else if( (pItemInfo->ItemIdx == eIncantation_InvenExtend || pItemInfo->ItemIdx == eIncantation_InvenExtend2))
			{
				if( m_pPlayer->GetExtraInvenSlot() )
				{
					pAddShopItem->ShopItem.Param = m_pPlayer->GetExtraInvenSlot() + 1;
					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pItemInfo->ItemIdx, pAddShopItem->ShopItem.Param );
				}
				else
				{
					UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), pAddShopItem->ShopItem.ItemBase.wIconIdx, pAddShopItem->ShopItem.ItemBase.dwDBIdx, 1, pAddShopItem->ShopItem.BeginTime.value, pAddShopItem->ShopItem.Remaintime );
					m_UsingItemTable.Add(pAddShopItem, pAddShopItem->ShopItem.ItemBase.wIconIdx);
				}
				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
			}
			else if( (pItemInfo->ItemIdx == eIncantation_PyogukExtend || pItemInfo->ItemIdx == eIncantation_PyogukExtend2) )
			{
				if( m_pPlayer->GetExtraPyogukSlot() )
				{
					pAddShopItem->ShopItem.Param = m_pPlayer->GetExtraPyogukSlot() + 1;
					ShopItemUseParamUpdateToDB( m_pPlayer->GetUserID(), 0, pItemInfo->ItemIdx, pAddShopItem->ShopItem.Param );
				}
				else
				{
					UsingShopItemUpdateToDB( m_pPlayer->GetUserID(), 0, pAddShopItem->ShopItem.ItemBase.wIconIdx, pAddShopItem->ShopItem.ItemBase.dwDBIdx, 1, pAddShopItem->ShopItem.BeginTime.value, pAddShopItem->ShopItem.Remaintime );
					m_UsingItemTable.Add(pAddShopItem, pAddShopItem->ShopItem.ItemBase.wIconIdx);
				}
				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
			}
			else if( (pItemInfo->ItemIdx == eIncantation_MugongExtend || pItemInfo->ItemIdx == eIncantation_MugongExtend2) )
			{
				if( m_pPlayer->GetExtraMugongSlot() )
				{
					pAddShopItem->ShopItem.Param = m_pPlayer->GetExtraMugongSlot() + 1;
					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pItemInfo->ItemIdx, pAddShopItem->ShopItem.Param );				
				}
				else
				{
					UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), pAddShopItem->ShopItem.ItemBase.wIconIdx, pAddShopItem->ShopItem.ItemBase.dwDBIdx, 1, pAddShopItem->ShopItem.BeginTime.value, pAddShopItem->ShopItem.Remaintime );
					m_UsingItemTable.Add(pAddShopItem, pAddShopItem->ShopItem.ItemBase.wIconIdx);					
				}
				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
			}
			else if( (pItemInfo->ItemIdx == eIncantation_CharacterSlot || pItemInfo->ItemIdx == eIncantation_CharacterSlot2) )
			{
				if( m_pPlayer->GetExtraCharacterSlot() )
				{
					pAddShopItem->ShopItem.Param = m_pPlayer->GetExtraCharacterSlot() + 1;
					ShopItemUseParamUpdateToDB( m_pPlayer->GetUserID(), 0, pItemInfo->ItemIdx, pAddShopItem->ShopItem.Param );
					m_pPlayer->SetExtraCharacterSlot( pAddShopItem->ShopItem.Param );
				}
				else
				{
					UsingShopItemUpdateToDB( m_pPlayer->GetUserID(), 0, pAddShopItem->ShopItem.ItemBase.wIconIdx, pAddShopItem->ShopItem.ItemBase.dwDBIdx, 1, pAddShopItem->ShopItem.BeginTime.value, pAddShopItem->ShopItem.Remaintime );
					m_UsingItemTable.Add(pAddShopItem, pAddShopItem->ShopItem.ItemBase.wIconIdx);
					m_pPlayer->SetExtraCharacterSlot( 1 );
				}
			}
#elif defined _TL_LOCAL_
			else if( pItemInfo->ItemIdx == eIncantation_InvenExtend )
			{
				if( m_pPlayer->GetExtraInvenSlot() )
				{
					pAddShopItem->ShopItem.Param = m_pPlayer->GetExtraInvenSlot() + 1;
					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pItemInfo->ItemIdx, pAddShopItem->ShopItem.Param );
				}
				else
				{
					UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), pAddShopItem->ShopItem.ItemBase.wIconIdx, pAddShopItem->ShopItem.ItemBase.dwDBIdx, 1, pAddShopItem->ShopItem.BeginTime.value, pAddShopItem->ShopItem.Remaintime );
					m_UsingItemTable.Add(pAddShopItem, pAddShopItem->ShopItem.ItemBase.wIconIdx);
				}
				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
			}
			else if( pItemInfo->ItemIdx == eIncantation_PyogukExtend )
			{
				if( m_pPlayer->GetExtraPyogukSlot() )
				{
					pAddShopItem->ShopItem.Param = m_pPlayer->GetExtraPyogukSlot() + 1;
					ShopItemUseParamUpdateToDB( m_pPlayer->GetUserID(), 0, pItemInfo->ItemIdx, pAddShopItem->ShopItem.Param );
				}
				else
				{
					UsingShopItemUpdateToDB( m_pPlayer->GetUserID(), 0, pAddShopItem->ShopItem.ItemBase.wIconIdx, pAddShopItem->ShopItem.ItemBase.dwDBIdx, 1, pAddShopItem->ShopItem.BeginTime.value, pAddShopItem->ShopItem.Remaintime );
					m_UsingItemTable.Add(pAddShopItem, pAddShopItem->ShopItem.ItemBase.wIconIdx);
				}
				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
			}
			else if( pItemInfo->ItemIdx == eIncantation_MugongExtend )
			{
				if( m_pPlayer->GetExtraMugongSlot() )
				{
					pAddShopItem->ShopItem.Param = m_pPlayer->GetExtraMugongSlot() + 1;
					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pItemInfo->ItemIdx, pAddShopItem->ShopItem.Param );				
				}
				else
				{
					UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), pAddShopItem->ShopItem.ItemBase.wIconIdx, pAddShopItem->ShopItem.ItemBase.dwDBIdx, 1, pAddShopItem->ShopItem.BeginTime.value, pAddShopItem->ShopItem.Remaintime );
					m_UsingItemTable.Add(pAddShopItem, pAddShopItem->ShopItem.ItemBase.wIconIdx);					
				}
				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
			}
			else if( pItemInfo->ItemIdx == eIncantation_CharacterSlot )
			{
				if( m_pPlayer->GetExtraCharacterSlot() )
				{
					pAddShopItem->ShopItem.Param = m_pPlayer->GetExtraCharacterSlot()+1;
					ShopItemUseParamUpdateToDB( m_pPlayer->GetUserID(), 0, pItemInfo->ItemIdx, pAddShopItem->ShopItem.Param );
					m_pPlayer->SetExtraCharacterSlot( pAddShopItem->ShopItem.Param );
				}
				else
				{
					UsingShopItemUpdateToDB( m_pPlayer->GetUserID(), 0, pAddShopItem->ShopItem.ItemBase.wIconIdx, pAddShopItem->ShopItem.ItemBase.dwDBIdx, 1, pAddShopItem->ShopItem.BeginTime.value, pAddShopItem->ShopItem.Remaintime );
					m_UsingItemTable.Add(pAddShopItem, pAddShopItem->ShopItem.ItemBase.wIconIdx);
					m_pPlayer->SetExtraCharacterSlot( 1 );
				}
			}
#endif
			// 스탯 재분배
			else if( pItemInfo->GenGol )
			{
				SHOPITEMWITHTIME* pUsedItem = m_UsingItemTable.GetData( eIncantation_StatePoint );
				if( pUsedItem )
				{
					CalcShopItemOption( pItemInfo->ItemIdx, TRUE, pItemInfo->GenGol );
					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), eIncantation_StatePoint, m_pPlayer->GetShopItemStats()->StatePoint );
				}
				else
				{
					// Insert Using Info
					UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), eIncantation_StatePoint, pAddShopItem->ShopItem.ItemBase.dwDBIdx, pItemInfo->GenGol, pAddShopItem->ShopItem.BeginTime.value, 0 );
					CalcShopItemOption( pItemInfo->ItemIdx, TRUE, pItemInfo->GenGol );
					m_UsingItemTable.Add(pAddShopItem, eIncantation_StatePoint );
				}

			}
			// 스킬포인트 재분배
			else if( pItemInfo->Life )
			{
				SHOPITEMWITHTIME* pUsedItem = m_UsingItemTable.GetData( eIncantation_SkPointRedist );
				if( pUsedItem )
				{
					CalcShopItemOption( pItemInfo->ItemIdx, TRUE, pItemInfo->Life );
					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), eIncantation_SkPointRedist, m_pPlayer->GetShopItemStats()->SkillPoint );
				}
				else
				{
					CalcShopItemOption( pItemInfo->ItemIdx, TRUE, pItemInfo->Life );
					UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), eIncantation_SkPointRedist, pAddShopItem->ShopItem.ItemBase.dwDBIdx,
						m_pPlayer->GetShopItemStats()->SkillPoint, pAddShopItem->ShopItem.BeginTime.value, m_pPlayer->GetShopItemStats()->UseSkillPoint );
					m_UsingItemTable.Add(pAddShopItem, eIncantation_SkPointRedist);
				}
			}
			else if( pItemInfo->ItemIdx == eIncantation_MixUp )
			{
				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
				UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), pAddShopItem->ShopItem.ItemBase.wIconIdx, pAddShopItem->ShopItem.ItemBase.dwDBIdx, pItemInfo->SellPrice, pAddShopItem->ShopItem.BeginTime.value, pAddShopItem->ShopItem.Remaintime );
				m_UsingItemTable.Add(pAddShopItem, pAddShopItem->ShopItem.ItemBase.wIconIdx);
			}
			else
			{
				UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), pAddShopItem->ShopItem.ItemBase.wIconIdx, pAddShopItem->ShopItem.ItemBase.dwDBIdx, pItemInfo->SellPrice, pAddShopItem->ShopItem.BeginTime.value, pAddShopItem->ShopItem.Remaintime );
				m_UsingItemTable.Add(pAddShopItem, pAddShopItem->ShopItem.ItemBase.wIconIdx);
			}
		}
	}
	// 부적
	else if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM ||
		pItemInfo->ItemKind == eSHOP_ITEM_SUNDRIES )
	{
		// 사용제한레벨 추가
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM && pItemInfo->EquipKind )
		{
			if( m_pPlayer->GetMaxLevel() < pItemInfo->EquipKind )
				return eItemUseErr_Err;
		}

		// 체인지아이템
		if( pItemInfo->ItemKind == eSHOP_ITEM_SUNDRIES && pItemInfo->SimMek )
		{
			int err = CHANGEITEMMGR->UseChangeItemFromShopItem( m_pPlayer, pItemInfo->SimMek );
			if(  err == 2 )
			{
				//공간이 부족하다는 메시지를 날린다.
				MSG_WORD msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_CHANGEITEM_NACK;
				msg.wData = NOT_SPACE;
				m_pPlayer->SendMsg(&msg, sizeof(msg));				
				return eItemUseErr_Err;
			}
			else if( err != EI_TRUE )
				return eItemUseErr_Err;
		}
		
		if( pItemInfo->ItemType == 10 )
		if(EI_TRUE != ITEMMGR->DiscardItem( m_pPlayer, UseBaseInfo.ShopItemPos, UseBaseInfo.ShopItemIdx, 1 ))
		{
			return eItemUseErr_Err;
		}

		//
		MSG_DWORD2	msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_EFFECT;
		msg.dwData1 = m_pPlayer->GetID();

		if( pItemInfo->ItemIdx == eSundries_Boom_Butterfly )
		{
			msg.dwData2 = eEffect_ShopItem_Boom1;
			PACKEDDATA_OBJ->QuickSend( m_pPlayer, &msg, sizeof(msg) );	
		}
		else if( pItemInfo->ItemIdx == eSundries_Boom_Star )
		{
			msg.dwData2 = eEffect_ShopItem_Boom2;
			PACKEDDATA_OBJ->QuickSend( m_pPlayer, &msg, sizeof(msg) );	
		}
		//
		
		CalcShopItemOption( pAddShopItem->ShopItem.ItemBase.wIconIdx, TRUE );
		m_pPlayer->CalcState();
		
		if( pItemInfo->SellPrice && pItemInfo->ItemType==10 )
		{
			UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), pAddShopItem->ShopItem.ItemBase.wIconIdx, pAddShopItem->ShopItem.ItemBase.dwDBIdx, pItemInfo->SellPrice, pAddShopItem->ShopItem.BeginTime.value, pAddShopItem->ShopItem.Remaintime );
			m_UsingItemTable.Add(pAddShopItem, pAddShopItem->ShopItem.ItemBase.wIconIdx);
		}
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION )
	{
		CalcShopItemOption( pAddShopItem->ShopItem.ItemBase.wIconIdx, TRUE );
		m_UsingItemTable.Add(pAddShopItem, pAddShopItem->ShopItem.ItemBase.wIconIdx);

		// 다른사람들에게 아이템 착용 정보를 보낸다.
		MSG_DWORD2 msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_PUTON;
		msg.dwData1 = m_pPlayer->GetID();
		msg.dwData2 = pItemInfo->ItemIdx;

		PACKEDDATA_OBJ->QuickSendExceptObjectSelf(m_pPlayer, &msg, sizeof(msg));
		goto LogNotUse;
	}
	//SW051129 Pet
	else if( pItemInfo->ItemKind == eSHOP_ITEM_PET )
	{
		if(pItemInfo->LimitLevel > m_pPlayer->GetMaxLevel())
		{
			return eItemUseErr_Err;
		}
		m_pPlayer->GetPetManager()->SummonPet(pItemBase->dwDBIdx);
		//m_pPlayer->GetPetManager()->SummonPet(PET_ID_START);
		//해당 유저의 펫 매니져에서 해당 팻을 찾아 현재팻으로 설정하고
		//해당 팻의 정보를 그리드에 보낸다.
		goto LogNotUse;
	}
	//SW060406 펫 장착
	else if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP )
	{
		//펫 소환중이 아니면
		if(!m_pPlayer->GetPetManager()->GetCurSummonPet())
		{
			return eItemUseErr_Err;
		}

		m_pPlayer->GetPetManager()->SetPetEquipOption(pItemInfo->ItemIdx, TRUE);
	}

	AddDupParam( pItemInfo );

	//
	LogItemMoney( m_pPlayer->GetID(), m_pPlayer->GetObjectName(), 0, "",
		eLog_ShopItemUse, m_pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
		pAddShopItem->ShopItem.ItemBase.wIconIdx, pAddShopItem->ShopItem.ItemBase.dwDBIdx, pAddShopItem->ShopItem.ItemBase.Position, 0, pAddShopItem->ShopItem.ItemBase.Durability, m_pPlayer->GetPlayerExpPoint());
	
LogNotUse:

	//
	memcpy(pShopItem, &pAddShopItem->ShopItem, sizeof(SHOPITEMBASE));
	
	return eItemUseSuccess;
}
*/
/*
BOOL CShopItemManager::UsedShopItem( ITEMBASE* pItemBase, DWORD Param, stTIME BeginTime, DWORD RemainTime )
{
	if( !pItemBase )
		return FALSE;

	if( m_UsingItemTable.GetData(pItemBase->wIconIdx) )
		return FALSE;

	SHOPITEMWITHTIME* ShopItem = m_UsingItemPool->Alloc();
	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(pItemBase->wIconIdx);
	if( !pItemInfo )
	{
		if( pItemBase->wIconIdx == eIncantation_SkPointRedist )
		{
			SHOPITEMOPTION* pOption = m_pPlayer->GetShopItemStats();
			pOption->SkillPoint = Param;
			pOption->UseSkillPoint = RemainTime;
		}
		else if( pItemBase->wIconIdx == eIncantation_StatePoint )
		{
			SHOPITEMOPTION* pOption = m_pPlayer->GetShopItemStats();
			pOption->StatePoint = (WORD)Param;
			pOption->UseStatePoint = (WORD)RemainTime;
		}
		else
			return FALSE;
	}
	else if( pItemBase->wIconIdx == eIncantation_StatePoint_30 )
	{
		SHOPITEMOPTION* pOption = m_pPlayer->GetShopItemStats();
		pOption->StatePoint = (WORD)RemainTime;
		pOption->UseStatePoint = 30 - (WORD)RemainTime;
		pItemBase->wIconIdx = eIncantation_StatePoint;

		// ItemIdx, Param, RemainTime을 업데이트 시켜야 한다.
		ShopItemAllUseInfoUpdateToDB( pItemBase->dwDBIdx, m_pPlayer->GetID(), eIncantation_StatePoint, pOption->StatePoint, pOption->UseStatePoint );
	}

	ShopItem->ShopItem.ItemBase = *pItemBase;
	ShopItem->ShopItem.Param = Param;
	ShopItem->ShopItem.BeginTime = BeginTime;
	ShopItem->ShopItem.Remaintime = RemainTime;
	ShopItem->LastCheckTime = gCurTime;

	if( pItemInfo && pItemInfo->SellPrice == eShopItemUseParam_Realtime )
	{
		// 종료시간을 체크한다.
		stTIME ctime, rtime;
		ctime.value = GetCurTime();
		rtime.SetTime( ShopItem->ShopItem.Remaintime );
		if( ctime > rtime )
		{
			if( pItemInfo->ItemType == 11 )
			{					
				if(EI_TRUE != ITEMMGR->DiscardItem( m_pPlayer, ShopItem->ShopItem.ItemBase.Position, ShopItem->ShopItem.ItemBase.wIconIdx, 1 ))
				{
					ASSERTMSG(0, "ShoItem - CheckEndTime");
					return FALSE;
				}
				if( pItemInfo->ItemKind == eSHOP_ITEM_EQUIP )
				{
					// 아이템 없앤다.
				}

			}

			ShopItemDeleteToDB( m_pPlayer->GetID(), ShopItem->ShopItem.ItemBase.dwDBIdx );
			
			LogItemMoney( m_pPlayer->GetID(), m_pPlayer->GetObjectName(), ctime.value, "",
				eLog_ShopItemUseEnd, m_pPlayer->GetMoney(eItemTable_Inventory), ShopItem->ShopItem.BeginTime.value, ShopItem->ShopItem.Remaintime,
				ShopItem->ShopItem.ItemBase.wIconIdx, ShopItem->ShopItem.ItemBase.dwDBIdx, ShopItem->ShopItem.ItemBase.Position, 0, ShopItem->ShopItem.ItemBase.Durability, m_pPlayer->GetPlayerExpPoint());

			return FALSE;
		}
	}

	
	m_UsingItemTable.Add(ShopItem, ShopItem->ShopItem.ItemBase.wIconIdx);

#ifdef _JAPAN_LOCAL_
	if( pItemBase->wIconIdx == eIncantation_MugongExtend ||
		pItemBase->wIconIdx == eIncantation_PyogukExtend ||
		pItemBase->wIconIdx == eIncantation_InvenExtend ||
		pItemBase->wIconIdx == eIncantation_CharacterSlot )
		return	TRUE;
#elif defined _HK_LOCAL_
	if( pItemBase->wIconIdx == eIncantation_MugongExtend || pItemBase->wIconIdx == eIncantation_MugongExtend2 ||
		pItemBase->wIconIdx == eIncantation_PyogukExtend || pItemBase->wIconIdx == eIncantation_PyogukExtend2 ||
		pItemBase->wIconIdx == eIncantation_InvenExtend || pItemBase->wIconIdx == eIncantation_InvenExtend2 ||
		pItemBase->wIconIdx == eIncantation_CharacterSlot || pItemBase->wIconIdx == eIncantation_CharacterSlot2 )
		return	TRUE;
#elif defined _TL_LOCAL_
	if( pItemBase->wIconIdx == eIncantation_MugongExtend ||
		pItemBase->wIconIdx == eIncantation_PyogukExtend ||
		pItemBase->wIconIdx == eIncantation_InvenExtend ||
		pItemBase->wIconIdx == eIncantation_CharacterSlot )
		return	TRUE;
#endif // #ifdef _JAPAN_LOCAL_

	if( pItemInfo && pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION && pItemInfo->CheRyuk )
	{
		CalcShopItemOption(pItemBase->wIconIdx, TRUE, ShopItem->ShopItem.Param);
	}
	else
	{
		// 개인플러스타임이 종료되었으면 적용 안시킨다
		if( pItemInfo && pItemInfo->ItemKind == eSHOP_ITEM_CHARM && pItemInfo->MeleeAttackMin && RemainTime==0 )
			return TRUE;
		else
			CalcShopItemOption(pItemBase->wIconIdx, TRUE, ShopItem->ShopItem.Remaintime);
	}

	// 중복체크 파라미터 설정
	AddDupParam( pItemInfo );
	
	return TRUE;
}
*/
void CShopItemManager::DeleteUsingShopItem( DWORD ItemIdx, BOOL bDelete )
{
	SHOPITEMWITHTIME* pShopItem = m_UsingItemTable.GetData( ItemIdx );
	if( !pShopItem )	 return;

	m_UsingItemTable.Remove( pShopItem->ShopItem.ItemBase.wIconIdx );
	m_UsingItemPool->Free( pShopItem );

	if( bDelete )
	{
		ITEMMGR->DiscardItem( m_pPlayer, pShopItem->ShopItem.ItemBase.Position, pShopItem->ShopItem.ItemBase.wIconIdx, 1 );
		CalcShopItemOption( pShopItem->ShopItem.ItemBase.wIconIdx, FALSE );
		SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_USEEND, pShopItem->ShopItem.ItemBase.wIconIdx);
	}

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( (WORD)ItemIdx );
	if( pItemInfo )
		DeleteDupParam( pItemInfo );

	// DB에 셋팅
	ShopItemDeleteToDB( m_pPlayer->GetID(), pShopItem->ShopItem.ItemBase.dwDBIdx );

	// Log추가
	stTIME curtime;
	curtime.value = GetCurTime();
//	LogItemMoney( m_pPlayer->GetID(), m_pPlayer->GetObjectName(), curtime.value, "",
	LogItemMoney( m_pPlayer->GetID(), m_pPlayer->GetObjectName(), 0, "",
		eLog_ShopItemUseEnd, m_pPlayer->GetMoney(eItemTable_Inventory), pShopItem->ShopItem.BeginTime.value, pShopItem->ShopItem.Remaintime,
		pShopItem->ShopItem.ItemBase.wIconIdx, pShopItem->ShopItem.ItemBase.dwDBIdx, pShopItem->ShopItem.ItemBase.Position, 0, pShopItem->ShopItem.ItemBase.Durability, m_pPlayer->GetPlayerExpPoint());
}

void CShopItemManager::DeleteUsingShopItemInfo( DWORD ItemIdx )
{
	SHOPITEMWITHTIME* pShopItem = m_UsingItemTable.GetData( ItemIdx );
	if( !pShopItem )	 return;

	m_UsingItemTable.Remove( pShopItem->ShopItem.ItemBase.wIconIdx );
	m_UsingItemPool->Free( pShopItem );
}

void CShopItemManager::CheckEndTime()
{
	if( m_UsingItemTable.GetDataNum() == 0 )
		return;

	BOOL bUpdate = FALSE;
	m_Updatetime += gTickTime;
	if( m_Updatetime > 600000 )
	{
		m_Updatetime = 0;
		bUpdate = TRUE;
	}

	m_Checktime += gTickTime;
	if( !bUpdate && m_Checktime < 30000 )	
		return;


	stTIME curtime;
	ITEM_INFO* pItemInfo = NULL;
	SHOPITEMWITHTIME* pShopItem = NULL;
	curtime.value = GetCurTime();
	m_UsingItemTable.SetPositionHead();

	cPtrList listRemove;
	cPtrList listDel;

	// 종료시간 조사
	while( pShopItem = m_UsingItemTable.GetData() )
	{
		pItemInfo = ITEMMGR->GetItemInfo( pShopItem->ShopItem.ItemBase.wIconIdx );
		if( !pItemInfo)		continue;

		// 현재시간
		if( pItemInfo->SellPrice == eShopItemUseParam_Realtime )
		{
			stTIME EndTime;
			EndTime.SetTime( pShopItem->ShopItem.Remaintime );
			if( curtime > EndTime )
			{
				CShopInvenSlot* pSlot = (CShopInvenSlot*)m_pPlayer->GetSlot( eItemTable_ShopInven );				
				
				if( pItemInfo->ItemType == 11 )
				{
					BOOL bfind = FALSE;
					// Item Position 확인
					for(int i=0; i<SLOT_SHOPINVEN_NUM/2; i++)
					{
						ITEMBASE* pItemBase = (ITEMBASE*)pSlot->GetItemInfoAbs( i+pSlot->GetStartPos() );
						if( !pItemBase )	continue;

						if( pItemBase->dwDBIdx == pShopItem->ShopItem.ItemBase.dwDBIdx )
						{
							pShopItem->ShopItem.ItemBase.Position = pItemBase->Position;
							bfind = TRUE;
							break;
						}
					}
					// 장비인지 확인
					if( !bfind )
					{
						CShopInvenSlot* pSlot = (CShopInvenSlot*)m_pPlayer->GetSlot( eItemTable_Weared );
						for(int i=0; i<SLOT_WEAR_NUM; i++)
						{
							ITEMBASE* pItemBase = (ITEMBASE*)pSlot->GetItemInfoAbs( i+pSlot->GetStartPos() );
							if( !pItemBase )	continue;

							if( pItemBase->dwDBIdx == pShopItem->ShopItem.ItemBase.dwDBIdx )
							{
								pShopItem->ShopItem.ItemBase.Position = pItemBase->Position;
								bfind = TRUE;
								break;
							}
						}
					}
					if( !bfind )
					{
						CPetWearSlot* pSlot = (CPetWearSlot*)m_pPlayer->GetSlot( eItemTable_PetWeared );
						for(int i=0; i<SLOT_PETWEAR_NUM; i++)
						{
							ITEMBASE* pItemBase = (ITEMBASE*)pSlot->GetItemInfoAbs( i+pSlot->GetStartPos() );
							if( !pItemBase )	continue;

							if( pItemBase->dwDBIdx == pShopItem->ShopItem.ItemBase.dwDBIdx )
							{
								pShopItem->ShopItem.ItemBase.Position = pItemBase->Position;
								bfind = TRUE;
								break;
							}
						}
					}
					// magi82(26)
					if( !bfind )
					{
						CTitanShopItemSlot* pSlot = (CTitanShopItemSlot*)m_pPlayer->GetSlot( eItemTable_TitanShopItem );
						for(int i=0; i<SLOT_TITANSHOPITEM_NUM; i++)
						{
							ITEMBASE* pItemBase = (ITEMBASE*)pSlot->GetItemInfoAbs( i+pSlot->GetStartPos() );
							if( !pItemBase )	continue;

							if( pItemBase->dwDBIdx == pShopItem->ShopItem.ItemBase.dwDBIdx )
							{
								pShopItem->ShopItem.ItemBase.Position = pItemBase->Position;
								bfind = TRUE;
								break;
							}
						}
					}
					if( !bfind )
					{
						char buf[256];
						sprintf(buf, "ShopItem Position NOT find - Makeup or Decoration [%d] [%d]", pShopItem->ShopItem.ItemBase.wIconIdx, pShopItem->ShopItem.ItemBase.Position );
						ASSERTMSG(0, buf);
						continue;
					}
					if(EI_TRUE != ITEMMGR->DiscardItem( m_pPlayer, pShopItem->ShopItem.ItemBase.Position, pShopItem->ShopItem.ItemBase.wIconIdx, 1 ))
					{
						char buf[256];
						sprintf(buf, "ShopItem Discard - Makeup or Decoration [%d] [%d]", pShopItem->ShopItem.ItemBase.wIconIdx, pShopItem->ShopItem.ItemBase.Position );
						ASSERTMSG(0, buf);
						continue;
					}
					
					if( pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION || pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP || pItemInfo->ItemKind == eSHOP_ITEM_TITAN_EQUIP )
					{
						MSG_ITEM_DISCARD_ACK msg;
						msg.Category = MP_ITEM;
						msg.Protocol = MP_ITEM_DISCARD_ACK;
						msg.wItemIdx = pShopItem->ShopItem.ItemBase.wIconIdx;
						msg.TargetPos = pShopItem->ShopItem.ItemBase.Position;
						msg.ItemNum = 1;
						m_pPlayer->SendMsg( &msg, sizeof(msg) );
						
						TakeOffAvatarItem( pShopItem->ShopItem.ItemBase.wIconIdx, pShopItem->ShopItem.ItemBase.Position );
					}

					//2007. 12. 11. CBH - 스킨 아이탬 관련 처리
					if( (pItemInfo->ItemKind == eSHOP_ITEM_NOMALCLOTHES_SKIN) || (pItemInfo->ItemKind == eSHOP_ITEM_COSTUME_SKIN) )
						DiscardSkinItem( pItemInfo->ItemIdx );					
				}
				else
				{
					listDel.AddTail(pShopItem);
				}

				// player에게 셋팅
				CalcShopItemOption( pShopItem->ShopItem.ItemBase.wIconIdx, FALSE );
				SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_USEEND, pShopItem->ShopItem.ItemBase.wIconIdx);				
			}
			else
			{
				stTIME checksum;
				checksum = EndTime;
				checksum -= curtime;

				// 1분 미만
				if( !checksum.GetYear() && !checksum.GetMonth() && !checksum.GetDay() && !checksum.GetHour() && !checksum.GetMinute() )
					SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_ONEMINUTE, pShopItem->ShopItem.ItemBase.wIconIdx);				

				// 프리미엄 아이템 패널티 갱신추가
			}
		}
		// 플레이시간
		else if( pItemInfo->SellPrice == eShopItemUseParam_Playtime )
		{
			// 개인플러스타임이면
			if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM && pItemInfo->MeleeAttackMin )
			{
				// 플러스타임과 겹치면 시간이 안가도록
				if( pShopItem->ShopItem.Remaintime && gEventRate[pItemInfo->MeleeAttackMin] != gEventRateFile[pItemInfo->MeleeAttackMin] )
				{
					pShopItem->LastCheckTime = gCurTime;
					continue;
				}
				else if( pShopItem->ShopItem.Remaintime == 0 )
					continue;
			}
			
			int backuptime = pShopItem->ShopItem.Remaintime;
			int checksum = pShopItem->ShopItem.Remaintime - (gCurTime - pShopItem->LastCheckTime);
			if( checksum <= 0 )
			{				
				pShopItem->ShopItem.Remaintime = 0;
												  
				// player에게 셋팅
				CalcShopItemOption( pShopItem->ShopItem.ItemBase.wIconIdx, FALSE );
				SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_USEEND, pShopItem->ShopItem.ItemBase.wIconIdx);

				/*
				// 개인플러스타임이 삭제하지 않고 보관한다. (같은종류는 하루에 하나만 사용하도록 하기위해)
				if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM && pItemInfo->MeleeAttackMin )				
					ShopItemUpdatetimeToDB( m_pPlayer->GetID(), pShopItem->ShopItem.ItemBase.wIconIdx, 0 );
				else
				{
					ShopItemDeleteToDB( m_pPlayer->GetID(), pShopItem->ShopItem.ItemBase.dwDBIdx );
					listRemove.AddTail(pShopItem);
				}
				*/
				ShopItemDeleteToDB( m_pPlayer->GetID(), pShopItem->ShopItem.ItemBase.dwDBIdx );
				listRemove.AddTail(pShopItem);

				DeleteDupParam( pItemInfo );

				// Log추가
				LogItemMoney( m_pPlayer->GetID(), m_pPlayer->GetObjectName(), 0, "",
					eLog_ShopItemUseEnd, m_pPlayer->GetMoney(eItemTable_Inventory), pShopItem->ShopItem.BeginTime.value, pShopItem->ShopItem.Remaintime,
					pShopItem->ShopItem.ItemBase.wIconIdx, pShopItem->ShopItem.ItemBase.dwDBIdx, pShopItem->ShopItem.ItemBase.Position, 0, pShopItem->ShopItem.ItemBase.Durability, m_pPlayer->GetPlayerExpPoint());
			}
			else if( pShopItem->ShopItem.Remaintime )
			{
				pShopItem->ShopItem.Remaintime -= (gCurTime - pShopItem->LastCheckTime);
				pShopItem->LastCheckTime = gCurTime;
			}

			if( bUpdate && checksum > 0 )
				ShopItemUpdatetimeToDB( m_pPlayer->GetID(), pShopItem->ShopItem.ItemBase.wIconIdx, pShopItem->ShopItem.Remaintime );
			if( backuptime > (MINUTETOSECOND*1000) && pShopItem->ShopItem.Remaintime <= (MINUTETOSECOND*1000) )
				SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_ONEMINUTE, pShopItem->ShopItem.ItemBase.wIconIdx);
		}
	}

	PTRLISTPOS pos = listDel.GetHeadPosition();
	while( pos )
	{
		SHOPITEMWITHTIME* p = (SHOPITEMWITHTIME*)listDel.GetNext(pos);
		DeleteUsingShopItem( p->ShopItem.ItemBase.wIconIdx );
	}
	listDel.RemoveAll();

	pos = listRemove.GetHeadPosition();
	while( pos )
	{
		SHOPITEMWITHTIME* p = (SHOPITEMWITHTIME*)listRemove.GetNext(pos);
		m_UsingItemTable.Remove( p->ShopItem.ItemBase.wIconIdx );
		m_UsingItemPool->Free(p);
	}
	listRemove.RemoveAll();

	m_Checktime = 0;
}


void CShopItemManager::CheckAvatarEndtime()
{
	stTIME curtime;
	ITEM_INFO* pItemInfo = NULL;
	SHOPITEMWITHTIME* pShopItem = NULL;
	curtime.value = GetCurTime();
	m_UsingItemTable.SetPositionHead();

	// 종료시간 조사
	while( pShopItem = m_UsingItemTable.GetData() )
	{
		pItemInfo = ITEMMGR->GetItemInfo( pShopItem->ShopItem.ItemBase.wIconIdx );
		if( !pItemInfo)		continue;

		// 현재시간
		if( pItemInfo->SellPrice == eShopItemUseParam_Realtime )
		{
			stTIME EndTime;
			EndTime.SetTime( pShopItem->ShopItem.Remaintime );
			if( curtime > EndTime )
			{
				// Item 지우고
				if(EI_TRUE != ITEMMGR->DiscardItem( m_pPlayer, pShopItem->ShopItem.ItemBase.Position, pShopItem->ShopItem.ItemBase.wIconIdx, 1 ))
				{
					ASSERTMSG(0, "ShoItem - CheckEndTime");
					continue;
				}
				// player에게 셋팅
				SendMsgDwordToPlayer(MP_ITEM_SHOPITEM_USEEND, pShopItem->ShopItem.ItemBase.wIconIdx);
				ShopItemDeleteToDB( m_pPlayer->GetID(), pShopItem->ShopItem.ItemBase.dwDBIdx );

				// Log추가
				LogItemMoney( m_pPlayer->GetID(), m_pPlayer->GetObjectName(), 0, "",
					eLog_ShopItemUseEnd, m_pPlayer->GetMoney(eItemTable_Inventory), pShopItem->ShopItem.BeginTime.value, pShopItem->ShopItem.Remaintime,
					pShopItem->ShopItem.ItemBase.wIconIdx, pShopItem->ShopItem.ItemBase.dwDBIdx, pShopItem->ShopItem.ItemBase.Position, 0, pShopItem->ShopItem.ItemBase.Durability, m_pPlayer->GetPlayerExpPoint());
			}
		}
	}
}


void CShopItemManager::SendMsgDwordToPlayer( DWORD Msg, DWORD Param )
{
	MSG_DWORD msg;
	msg.Category = MP_ITEM;
	msg.Protocol = (BYTE)Msg;
	msg.dwData = Param;
	
	m_pPlayer->SendMsg(&msg, sizeof(msg));

}


void CShopItemManager::UpdateLogoutToDB()
{
	stTIME curtime;
	SHOPITEMWITHTIME* pShopItem = NULL;
	ITEM_INFO* pItemInfo = NULL;


	curtime.value = GetCurTime();
	m_UsingItemTable.SetPositionHead();

	// 종료시간 조사
	while( pShopItem = m_UsingItemTable.GetData() )
	{
		pItemInfo = ITEMMGR->GetItemInfo( pShopItem->ShopItem.ItemBase.wIconIdx );
		if( !pItemInfo )		continue;

		// 플레이시간
		if( pItemInfo->SellPrice == eShopItemUseParam_Playtime )
		{
			// 개인플러스타임이면
			if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM && pItemInfo->MeleeAttackMin )
			{
				// 플러스타임과 겹치면 시간이 안가도록
				if( pShopItem->ShopItem.Remaintime && gEventRate[pItemInfo->MeleeAttackMin] != gEventRateFile[pItemInfo->MeleeAttackMin] )
				{
					pShopItem->LastCheckTime = gCurTime;
					continue;
				}
				else if( pShopItem->ShopItem.Remaintime == 0 )
					continue;
			}
			
			DWORD checktime = gCurTime - pShopItem->LastCheckTime;
			if( checktime > 30000 )			checktime = 30000;
			//
			if( checktime >= pShopItem->ShopItem.Remaintime )
				pShopItem->ShopItem.Remaintime = 0;
			else
				pShopItem->ShopItem.Remaintime -= checktime;
			//
			ShopItemUpdatetimeToDB( m_pPlayer->GetID(), pShopItem->ShopItem.ItemBase.wIconIdx, pShopItem->ShopItem.Remaintime );
		}
	}
}



// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
// ┃RaMa - 04.11.08                                           ┃
// ┃Sub. ShopItem		                                       ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
BOOL CShopItemManager::CalcShopItemOption( DWORD wIdx, BOOL bAdd, DWORD Param )
{
	if( wIdx == 0 )			return FALSE;
	
	int calc = -1;
	if(bAdd)	calc=1;
	SHOPITEMOPTION* pShopItemOption = m_pPlayer->GetShopItemStats();
	if( !pShopItemOption )	return FALSE;

	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( (WORD)wIdx );
	if(!pItemInfo)
	{
		if( wIdx != eIncantation_SkPointRedist &&
			wIdx != eIncantation_StatePoint )
		{
			char buf[256]= { 0, };
			sprintf(buf, "S ItemInfo가 없다. PlayerIdx[%d] Idx[%d]", m_pPlayer->GetID(), wIdx);
			ASSERTMSG(0, buf);
			return FALSE;
		}
		else
			return FALSE;
	}

/*	if( pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM )
	{
		// 경험치   GenGol
		if( pItemInfo->GenGol>0 )
		{
			pShopItemOption->AddExp += (pItemInfo->GenGol*calc);
			if(pShopItemOption->AddExp < 0)
				pShopItemOption->AddExp=0;			
		}
		// 아이템 드랍확률
		if( pItemInfo->MinChub>0 )
		{
			pShopItemOption->AddItemDrop += (pItemInfo->MinChub*calc);
			if(pShopItemOption->AddItemDrop < 0)
				pShopItemOption->AddItemDrop=0;
		}
		// 죽었을때 경험치와 돈 패널티 없는것		
	}*/
	if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION )
	{
		// 조합확률
		if( pItemInfo->ItemIdx == eIncantation_MixUp )
		{
			pShopItemOption->ItemMixSuccess += (10*calc);
			if(pShopItemOption->ItemMixSuccess < 0)
				pShopItemOption->ItemMixSuccess=0;
		}
		if( pItemInfo->GenGol )
		{
			if( Param > 0 )
				pShopItemOption->StatePoint += (WORD)Param;
			/*
			else
			{
				m_pPlayer->SetPlayerLevelUpPoint( m_pPlayer->GetPlayerLevelUpPoint()+pItemInfo->GenGol );
				pShopItemOption->StatePoint = 0;
				DeleteUsingShopItem( pItemInfo->ItemIdx );
			}
			*/
		}
		else if( pItemInfo->Life )
		{
			if( Param > 0 )
				pShopItemOption->SkillPoint += pItemInfo->Life;
		}
		else if( pItemInfo->CheRyuk )
		{
			if( bAdd )
			{
				if( Param == 0 )
					pShopItemOption->ProtectCount = (char)pItemInfo->CheRyuk;
				else if( Param )
					pShopItemOption->ProtectCount = (char)Param;

				m_ProtectItemIdx = pItemInfo->ItemIdx;
			}
			else
				m_ProtectItemIdx = 0;
		}
		else if( pItemInfo->LimitJob )
		{
			if( bAdd )
				pShopItemOption->EquipLevelFree += (BYTE)pItemInfo->LimitJob;
			else
			{
				pShopItemOption->EquipLevelFree -= (BYTE)pItemInfo->LimitJob;				
			}
			//STATSMGR->CalcItemStats( m_pPlayer );
		}
#ifdef _JAPAN_LOCAL_
		else if( pItemInfo->ItemIdx == eIncantation_InvenExtend )
		{	
			m_pPlayer->SetExtraSlotCount( m_pPlayer->GetExtraPyogukSlot(), 
				m_pPlayer->GetExtraInvenSlot()+1, m_pPlayer->GetExtraMugongSlot() );
		}
		else if( pItemInfo->ItemIdx == eIncantation_PyogukExtend )
		{
			m_pPlayer->SetExtraSlotCount( m_pPlayer->GetExtraPyogukSlot()+1, 
				m_pPlayer->GetExtraInvenSlot(), m_pPlayer->GetExtraMugongSlot() );
		}
		else if( pItemInfo->ItemIdx == eIncantation_MugongExtend )
		{
			m_pPlayer->SetExtraSlotCount( m_pPlayer->GetExtraPyogukSlot(), 
				m_pPlayer->GetExtraInvenSlot(), m_pPlayer->GetExtraMugongSlot()+1 );
		}
		else if( pItemInfo->ItemIdx == eIncantation_CharacterSlot )
		{
			m_pPlayer->SetExtraCharacterSlot( m_pPlayer->GetExtraCharacterSlot() + 1 );
		}
#elif defined _HK_LOCAL_
		else if( (pItemInfo->ItemIdx == eIncantation_InvenExtend || pItemInfo->ItemIdx == eIncantation_InvenExtend2) )
		{	
			m_pPlayer->SetExtraSlotCount( m_pPlayer->GetExtraPyogukSlot(), 
				m_pPlayer->GetExtraInvenSlot()+1, m_pPlayer->GetExtraMugongSlot() );
		}
		else if( (pItemInfo->ItemIdx == eIncantation_PyogukExtend || pItemInfo->ItemIdx == eIncantation_PyogukExtend2) )
		{
			m_pPlayer->SetExtraSlotCount( m_pPlayer->GetExtraPyogukSlot()+1, 
				m_pPlayer->GetExtraInvenSlot(), m_pPlayer->GetExtraMugongSlot() );
		}
		else if( (pItemInfo->ItemIdx == eIncantation_MugongExtend || pItemInfo->ItemIdx == eIncantation_MugongExtend2) )
		{
			m_pPlayer->SetExtraSlotCount( m_pPlayer->GetExtraPyogukSlot(), 
				m_pPlayer->GetExtraInvenSlot(), m_pPlayer->GetExtraMugongSlot()+1 );
		}
		else if( (pItemInfo->ItemIdx == eIncantation_CharacterSlot || pItemInfo->ItemIdx == eIncantation_CharacterSlot2) )
		{
			m_pPlayer->SetExtraCharacterSlot( m_pPlayer->GetExtraCharacterSlot() + 1 );
		}
#elif defined _TL_LOCAL_
		else if( pItemInfo->ItemIdx == eIncantation_InvenExtend )
		{	
			m_pPlayer->SetExtraSlotCount( m_pPlayer->GetExtraPyogukSlot(), 
				m_pPlayer->GetExtraInvenSlot()+1, m_pPlayer->GetExtraMugongSlot() );
		}
		else if( pItemInfo->ItemIdx == eIncantation_PyogukExtend )
		{
			m_pPlayer->SetExtraSlotCount( m_pPlayer->GetExtraPyogukSlot()+1, 
				m_pPlayer->GetExtraInvenSlot(), m_pPlayer->GetExtraMugongSlot() );
		}
		else if( pItemInfo->ItemIdx == eIncantation_MugongExtend )
		{
			m_pPlayer->SetExtraSlotCount( m_pPlayer->GetExtraPyogukSlot(), 
				m_pPlayer->GetExtraInvenSlot(), m_pPlayer->GetExtraMugongSlot()+1 );
		}
		else if( pItemInfo->ItemIdx == eIncantation_CharacterSlot )
		{
			m_pPlayer->SetExtraCharacterSlot( m_pPlayer->GetExtraCharacterSlot() + 1 );
		}
#endif
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM )
	{
		if( pItemInfo->GenGol>0 )
		{
			pShopItemOption->Gengol += (pItemInfo->GenGol*calc);
			if(pShopItemOption->Gengol < 0)
				pShopItemOption->Gengol=0;
			
			if( bAdd == FALSE )
				STATSMGR->CalcCharStats( m_pPlayer );
		}
		if( pItemInfo->MinChub>0 )
		{
			pShopItemOption->Minchub += (pItemInfo->MinChub*calc);
			if(pShopItemOption->Minchub < 0)
				pShopItemOption->Minchub=0;

			if( bAdd == FALSE )
				STATSMGR->CalcCharStats( m_pPlayer );
		}
		if( pItemInfo->CheRyuk>0 )
		{		
			pShopItemOption->Cheryuk += (pItemInfo->CheRyuk*calc);
			if(pShopItemOption->Cheryuk < 0)
				pShopItemOption->Cheryuk=0;

			if( bAdd == FALSE )
				STATSMGR->CalcCharStats( m_pPlayer );
		}
		if( pItemInfo->SimMek>0 )
		{
			pShopItemOption->Simmek += (pItemInfo->SimMek*calc);
			if(pShopItemOption->Simmek < 0)
				pShopItemOption->Simmek=0;

			if( bAdd == FALSE )
				STATSMGR->CalcCharStats( m_pPlayer );
		}
		// 내공데미지   Life
		if( pItemInfo->Life>0 )
		{
			pShopItemOption->NeagongDamage += (char)(pItemInfo->Life*calc);
			if(pShopItemOption->NeagongDamage < 0)
				pShopItemOption->NeagongDamage=0;
		}
		// 외공대미지   Shield
		if( pItemInfo->Shield>0 )
		{
			pShopItemOption->WoigongDamage += (char)(pItemInfo->Shield*calc);
			if(pShopItemOption->WoigongDamage < 0)
				pShopItemOption->WoigongDamage=0;
		}
		// 무공특기치 1씩  Plus_MugongIdx
		if( pItemInfo->NaeRyuk>0 )
		{
			pShopItemOption->AddSung += (pItemInfo->NaeRyuk*calc);
			if(pShopItemOption->AddSung < 0)
				pShopItemOption->AddSung=0;

			if( m_pPlayer->GetInited() )
				ABILITYMGR->UpdateAbilityState(ABILITYUPDATE_ABILITYLEVEL_CHANGED,0,m_pPlayer->GetAbilityGroup());
		}
		// 기본데미지  Plus_Value
		if( pItemInfo->LimitJob>0 )
		{
			pShopItemOption->ComboDamage += (pItemInfo->LimitJob*calc);
			if(pShopItemOption->ComboDamage < 0)
				pShopItemOption->ComboDamage=0;
		}
		// 크리티컬 확률   CriticalPercent
		if( pItemInfo->LimitGender>0 )
		{
			pShopItemOption->Critical += (pItemInfo->LimitGender*calc);
			if(pShopItemOption->Critical < 0)
				pShopItemOption->Critical=0;
		}
		// 크리티컬시 스턴확률    RangeAttackMin
		if( pItemInfo->LimitLevel>0 )
		{
			pShopItemOption->StunByCri += (pItemInfo->LimitLevel*calc);
			if(pShopItemOption->StunByCri < 0)
				pShopItemOption->StunByCri=0;
		}
		// 물리방어력
		if( pItemInfo->LimitGenGol>0 )
		{
			pShopItemOption->RegistPhys += (pItemInfo->LimitGenGol*calc);
			if(pShopItemOption->RegistPhys < 0)
				pShopItemOption->RegistPhys = 0;
		}
		// 속성방어력
		if( pItemInfo->LimitMinChub>0 )
		{
			pShopItemOption->RegistAttr += (pItemInfo->LimitMinChub*calc);
			if(pShopItemOption->RegistAttr < 0)
				pShopItemOption->RegistAttr = 0;
		}
		// 내력소모감소
		if( pItemInfo->LimitCheRyuk>0 )
		{
			// 플러스타임
			if( pItemInfo->MeleeAttackMin )
			{
				if( bAdd && gEventRate[pItemInfo->MeleeAttackMin] == gEventRateFile[pItemInfo->MeleeAttackMin] )
					pShopItemOption->PlustimeNaeruyk = (char)pItemInfo->LimitCheRyuk;
				else
					pShopItemOption->PlustimeNaeruyk = 0;
			}
			else
			{
				pShopItemOption->NeaRyukSpend += (pItemInfo->LimitCheRyuk*calc);
				if(pShopItemOption->NeaRyukSpend < 0 )
					pShopItemOption->NeaRyukSpend = 0;
			}
		}
		// 경험치 증가
		if( pItemInfo->LimitSimMek>0 )
		{
			// 플러스타임
			if( pItemInfo->MeleeAttackMin )
			{				
				if( bAdd && gEventRate[pItemInfo->MeleeAttackMin] == gEventRateFile[pItemInfo->MeleeAttackMin] )
					pShopItemOption->PlustimeExp = (char)pItemInfo->LimitSimMek;
				else
					pShopItemOption->PlustimeExp = 0;
			}
			else
			{
				pShopItemOption->AddExp += (pItemInfo->LimitSimMek*calc);
				if(pShopItemOption->AddExp < 0)
					pShopItemOption->AddExp=0;
			}
		}
		// 수련치 증가
		if( pItemInfo->ItemGrade>0 )
		{
			// 플러스타임
			if( pItemInfo->MeleeAttackMin )
			{
				if( bAdd && gEventRate[pItemInfo->MeleeAttackMin] == gEventRateFile[pItemInfo->MeleeAttackMin] )
					pShopItemOption->PlustimeAbil = (char)pItemInfo->ItemGrade;
				else
					pShopItemOption->PlustimeAbil = 0;
			}
			else
			{
				pShopItemOption->AddAbility += (pItemInfo->ItemGrade*calc);
				if(pShopItemOption->AddAbility < 0)
					pShopItemOption->AddAbility=0;
			}
		}
		// 무공수련치 증가
		if( pItemInfo->RangeType>0 )
		{
			pShopItemOption->AddMugongExp += (pItemInfo->RangeType*calc);
			if(pShopItemOption->AddMugongExp < 0)
				pShopItemOption->AddMugongExp=0;
		}
		// 최대생명력 증가치
		if( pItemInfo->Plus_MugongIdx )
		{
			pShopItemOption->Life += (pItemInfo->Plus_MugongIdx*calc);
			if( pShopItemOption->Life <= 0 )
				pShopItemOption->Life = 0;	
			
			if( bAdd==FALSE )
				STATSMGR->CalcCharLife(m_pPlayer);
		}
		// 최대호신강기 증가치
		if( pItemInfo->Plus_Value )
		{
			pShopItemOption->Shield += (pItemInfo->Plus_Value*calc);
			if( pShopItemOption->Shield <= 0 )
				pShopItemOption->Shield = 0;

			if( bAdd==FALSE )
				STATSMGR->CalcCharShield(m_pPlayer);
		}
		// 최대내력 증가치
		if( pItemInfo->AllPlus_Kind )
		{
			pShopItemOption->Naeryuk += (pItemInfo->AllPlus_Kind*calc);
			if( pShopItemOption->Naeryuk <= 0 )
				pShopItemOption->Naeryuk = 0;

			if( bAdd==FALSE )
				STATSMGR->CalcCharNaeruyk(m_pPlayer);
		}
		// 경공딜레이
		if( pItemInfo->RangeAttackMin )
		{
			pShopItemOption->bKyungGong += (pItemInfo->RangeAttackMin*calc);
		}
		// 경공속도 상승
		if( pItemInfo->RangeAttackMax )
		{
			pShopItemOption->KyungGongSpeed += (pItemInfo->RangeAttackMax*calc);
		}
		// 강화 증폭 수치
		if( pItemInfo->CriticalPercent )
		{
			pShopItemOption->ReinforceAmp += (pItemInfo->CriticalPercent*calc);

			if( m_pPlayer->GetInited() )
				STATSMGR->CalcItemStats( m_pPlayer );
		}
		// 아이템드랍확률 
		if( pItemInfo->PhyDef )
		{
			pShopItemOption->AddItemDrop += (pItemInfo->PhyDef*calc);
		}
		//////////////////////////////////////////////////////////////////////
		// 06. 07. 내공 적중(일격) - 이영준
		if( pItemInfo->NaeRyukRecover > 0 )
		{
			pShopItemOption->Decisive += (pItemInfo->NaeRyukRecover*calc);
			if(pShopItemOption->Decisive < 0)
				pShopItemOption->Decisive = 0;
		}
		//////////////////////////////////////////////////////////////////////
/*		// 내력회복룰 1.5배
		if( pItemInfo->NaeRyukRecoverRate>0 )
		{
			m_ShopItemOption.RecoverRate += (pItemInfo->NaeRyukRecoverRate*calc);
			if( m_ShopItemOption.RecoverRate<=0 )
				m_ShopItemOption.RecoverRate = 0;

		}*/
		// 노점상 꾸미기(노점상꾸미기이면 샵아이템 옵션에 해당 아이템 인덱스를 넣는다.
		if( (DWORD)(pItemInfo->AttrRegist.GetElement_Val(ATTR_FIRE)) > 0 )
		{
			pShopItemOption->dwStreetStallDecoration = pItemInfo->ItemIdx;

			if(bAdd == FALSE)
			{
				pShopItemOption->dwStreetStallDecoration = 0;
			}
		}
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
	{
		// 최대생명력 증가치
		if( pItemInfo->Life > 0 )
		{
			pShopItemOption->Life += (WORD)(pItemInfo->Life*calc);
			if( pShopItemOption->Life <= 0 )
				pShopItemOption->Life = 0;	
			
			if( bAdd==FALSE )
				STATSMGR->CalcCharLife(m_pPlayer);
		}
		// 최대호신강기 증가치
		if( pItemInfo->Shield > 0 )
		{
			pShopItemOption->Shield += (WORD)(pItemInfo->Shield*calc);
			if( pShopItemOption->Shield <= 0 )
				pShopItemOption->Shield = 0;

			if( bAdd==FALSE )
				STATSMGR->CalcCharShield(m_pPlayer);
		}
		// 최대내력 증가치
		if( pItemInfo->NaeRyuk > 0 )
		{
			pShopItemOption->Naeryuk += (pItemInfo->NaeRyuk*calc);
			if( pShopItemOption->Naeryuk <= 0 )
				pShopItemOption->Naeryuk = 0;

			if( bAdd==FALSE )
				STATSMGR->CalcCharNaeruyk(m_pPlayer);
		}
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION )
	{
		// CheckAvatarEquip( pItemInfo->ItemIdx, bAdd );
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_SUNDRIES )
	{
#ifdef _HK_LOCAL_
		// 노점개설
		if( pItemInfo->CheRyuk )
			pShopItemOption->bStreetStall += (pItemInfo->CheRyuk*calc);
#endif
		// 체인지아이템
		/*if( pItemInfo->SimMek )
		{
			CHANGEITEMMGR->UseChangeItemFromShopItem( m_pPlayer, pItemInfo->SimMek );
		}*/
	}
	return TRUE;
}


void CShopItemManager::SendShopItemUseInfo()
{
	SEND_SHOPITEM_USEDINFO msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_USEDINFO;
	msg.ItemCount = 0;

	SHOPITEMWITHTIME* pItem = NULL; 
	m_UsingItemTable.SetPositionHead();
	while( pItem = m_UsingItemTable.GetData() )
	{
		memcpy( &msg.Item[msg.ItemCount], &pItem->ShopItem, sizeof(SHOPITEMBASE) );
		++msg.ItemCount;
	}

	m_pPlayer->SendMsg(&msg, msg.GetSize());
}


BOOL CShopItemManager::AddMovePoint( MOVEDATA* pData )
{
	DWORD ValidCount = MAX_MOVEDATA_PERPAGE;
	if( m_UsingItemTable.GetData( eIncantation_MemoryMoveExtend ) ||
		m_UsingItemTable.GetData( eIncantation_MemoryMoveExtend7 ) ||
		m_UsingItemTable.GetData( eIncantation_MemoryMove2 ) ||
		m_UsingItemTable.GetData( eIncantation_MemoryMoveExtend30 ) )
		ValidCount = MAX_MOVEDATA_PERPAGE*MAX_MOVEPOINT_PAGE;

	if( m_MovePointTable.GetDataNum() >= ValidCount )
		return FALSE;

	MOVEDATA* pNewData = m_MovePointPool->Alloc();
	memcpy(pNewData, pData, sizeof(MOVEDATA));

	m_MovePointTable.Add( pNewData, pNewData->DBIdx );

	return TRUE;
}


BOOL CShopItemManager::DeleteMovePoint( DWORD DBIdx )
{
	MOVEDATA* pData = m_MovePointTable.GetData( DBIdx );

	if( !pData )
		return FALSE;

	m_MovePointTable.Remove( DBIdx );
	m_MovePointPool->Free( pData );

	return TRUE;
}


BOOL CShopItemManager::ReNameMovePoint( DWORD DBIdx, char* newName )
{
	MOVEDATA* pData = m_MovePointTable.GetData( DBIdx );

	if( !pData )
		return FALSE;

	memset(pData->Name, 0, MAX_SAVEDMOVE_NAME);
	strncpy(pData->Name, newName, MAX_SAVEDMOVE_NAME-1);

	return TRUE;
}


void CShopItemManager::ReleseMovePoint()
{
	m_MovePointTable.SetPositionHead();
	MOVEDATA* pData = NULL;
	while( pData = m_MovePointTable.GetData() )
	{
		m_MovePointPool->Free( pData );
	}
	m_MovePointTable.RemoveAll();
}


SHOPITEMWITHTIME* CShopItemManager::GetUsingItemInfo( DWORD ItemIdx )
{
	return m_UsingItemTable.GetData( ItemIdx );
}


void CShopItemManager::AddShopItemUse( SHOPITEMBASE* pShopItem )
{
	SHOPITEMWITHTIME* pAddShopItem = m_UsingItemPool->Alloc();

	memcpy( &pAddShopItem->ShopItem, pShopItem, sizeof(SHOPITEMBASE) );
	pAddShopItem->LastCheckTime = gCurTime;

	m_UsingItemTable.Add( pAddShopItem, pShopItem->ItemBase.wIconIdx );
}



BOOL CShopItemManager::PutOnAvatarItem( WORD ItemIdx, WORD ItemPos, BOOL bCalcStats )
{

	SHOPITEMWITHTIME* pShopItem = m_UsingItemTable.GetData(ItemIdx);
	const ITEMBASE* pItembase = ITEMMGR->GetItemInfoAbsIn( m_pPlayer, ItemPos );
	if( !pItembase )
		return FALSE;

	if( !pShopItem || pShopItem->ShopItem.ItemBase.dwDBIdx != pItembase->dwDBIdx )
		return FALSE;


	WORD* pAvatar = m_pPlayer->GetShopItemStats()->Avatar;
	if(!pAvatar)		return FALSE;
	AVATARITEM* pAvatarEquip = GAMERESRCMNGR->m_AvatarEquipTable.GetData( ItemIdx );
	if(!pAvatarEquip)	return FALSE;
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( ItemIdx );
	if(!pItemInfo)		return FALSE;

	if( pAvatarEquip->Position == eAvatar_Hat && pAvatar[eAvatar_Dress] )
	{
		AVATARITEM* pDressEquip = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pAvatar[eAvatar_Dress] );
		if( pDressEquip )
		{
			if( pDressEquip->Item[eAvatar_Hat] == 0 )
				return FALSE;
		}
	}

	SHOPITEMWITHTIME* pItem = NULL;

	if( pAvatarEquip->Position >= eAvatar_Weared_Gum )
	{
		if( m_pPlayer->GetInited() )
		{
			if( pAvatarEquip->Position != (eAvatar_Weared_Gum+m_pPlayer->GetWeaponEquipType()-1) )
				return FALSE;
		}

		if( pAvatar[pAvatarEquip->Position] > 1 )
		{
			ITEM_INFO* ptInfo = ITEMMGR->GetItemInfo( pAvatar[pAvatarEquip->Position] );
			if(!ptInfo)		return FALSE;

			ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pAvatar[pAvatarEquip->Position], ptInfo->SellPrice );
			if( pItem = m_UsingItemTable.GetData( pAvatar[pAvatarEquip->Position] ) )
				pItem->ShopItem.Param = ptInfo->SellPrice;
		}

		pAvatar[pAvatarEquip->Position] = ItemIdx;
		ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), ItemIdx, eShopItemUseParam_EquipAvatar );
		if( pItem = m_UsingItemTable.GetData( ItemIdx ) )
			pItem->ShopItem.Param = eShopItemUseParam_EquipAvatar;
	}
	
	for(int i=0; i<eAvatar_Weared_Gum; i++)
	{		
		if( i >= eAvatar_Weared_Hair )
		{
			if( !pAvatarEquip->Item[i] )
				pAvatar[i] = 0;
		}
		else
		{
			if( i == pAvatarEquip->Position )
			{
				if( pAvatar[i] )
				{
					// 기존의 아이템이 벗겨졌으면 기본아이템을 다시 셋팅해준다.
					AVATARITEM* pTemp = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pAvatar[i] );
					if( !pTemp )		continue;
					ITEM_INFO* ptInfo = ITEMMGR->GetItemInfo( pAvatar[i] );
					if(!ptInfo)		continue;

					for(int n=eAvatar_Weared_Hair; n<eAvatar_Weared_Gum; n++)
					{
						if( !pTemp->Item[n] )
							pAvatar[n] = 1;
					}

					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pAvatar[i], ptInfo->SellPrice );
					if( pItem = m_UsingItemTable.GetData( pAvatar[i] ) )
						pItem->ShopItem.Param = ptInfo->SellPrice;
				}
				
				pAvatar[i] = ItemIdx;
				
				ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), ItemIdx, eShopItemUseParam_EquipAvatar );
				if( pItem = m_UsingItemTable.GetData( ItemIdx ) )
					pItem->ShopItem.Param = eShopItemUseParam_EquipAvatar;
			}
			
			if( !pAvatarEquip->Item[i] && pAvatar[i] )
			{
				// 기존의 아이템이 벗겨졌으면 기본아이템을 다시 셋팅해준다.
				AVATARITEM* pTemp = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pAvatar[i] );
				if( !pTemp )		continue;
				ITEM_INFO* ptInfo = ITEMMGR->GetItemInfo( pAvatar[i] );
				if(!ptInfo)		continue;
				
				for(int n=eAvatar_Weared_Hair; n<eAvatar_Weared_Gum; n++)
				{
					if( !pTemp->Item[n] )
						pAvatar[n] = 1;
				}				
				
				ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pAvatar[i], ptInfo->SellPrice );
				if( pItem = m_UsingItemTable.GetData( ItemIdx ) )
					pItem->ShopItem.Param = ptInfo->SellPrice;
				pAvatar[i] = 0;
			}
		}
	}

	if( ItemPos )
	{
		SEND_AVATARITEM_INFO msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_INFO;
		msg.PlayerId = m_pPlayer->GetID();
		msg.ItemIdx = ItemIdx;
		msg.ItemPos = ItemPos;
		memcpy( &msg.Avatar, pAvatar, sizeof(WORD)*eAvatar_Max );
		PACKEDDATA_OBJ->QuickSend( m_pPlayer, &msg, sizeof(msg) );
	}

	//
	CalcAvatarOption( bCalcStats );

	return TRUE;
}


BOOL CShopItemManager::TakeOffAvatarItem( WORD ItemIdx, WORD ItemPos, BOOL bCalcStats  )
{
	WORD* pAvatar = m_pPlayer->GetShopItemStats()->Avatar;
	if(!pAvatar)		return FALSE;
	AVATARITEM* pAvatarEquip = GAMERESRCMNGR->m_AvatarEquipTable.GetData( ItemIdx );
	if(!pAvatarEquip)	return FALSE;
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( ItemIdx );
	if(!pItemInfo)		return FALSE;
	

	if( pAvatar[pAvatarEquip->Position] != ItemIdx )
		return FALSE;


	SHOPITEMWITHTIME* pItem = NULL;
	if( pAvatarEquip->Position >= eAvatar_Weared_Gum )
	{
		if( pAvatar[pAvatarEquip->Position] > 1 )
		{
			BOOL bCheck = FALSE;
			ITEM_INFO* ptInfo = ITEMMGR->GetItemInfo( pAvatar[pAvatarEquip->Position] );
			if(!ptInfo)		return FALSE;

			if( pAvatar[eAvatar_Dress] )
			{
				AVATARITEM* pTemp = GAMERESRCMNGR->m_AvatarEquipTable.GetData( pAvatar[eAvatar_Dress] );				
				if( !pTemp )		return FALSE;

				pAvatar[pAvatarEquip->Position] = pTemp->Item[pAvatarEquip->Position];
			}				
			else
			{				
				if(pAvatar[pAvatarEquip->Position] <= 1)	//2007. 10. 12. CBH - 무기 아바타 아이탬 갱신 부분이 빠져있어서 추가
					pAvatar[pAvatarEquip->Position] = 0;
				else
					bCheck = TRUE;
			}

			if(bCheck == FALSE)
			{
				ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pAvatar[pAvatarEquip->Position], ptInfo->SellPrice );
				if( pItem = m_UsingItemTable.GetData( pAvatar[pAvatarEquip->Position] ) )
					pItem->ShopItem.Param = ptInfo->SellPrice;
			}
			
		}
	}


	for(int i=0; i<eAvatar_Max; i++)
	{
		if( pAvatar[i] )
		{
			if( i==pAvatarEquip->Position )
			{
				pAvatar[i] = 0;
				for(int n=eAvatar_Weared_Hair; n<eAvatar_Weared_Gum; n++)
				{
					if( !pAvatarEquip->Item[n] )
						pAvatar[n] = 1;
				}

				ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), ItemIdx, pItemInfo->SellPrice );
				if( pItem = m_UsingItemTable.GetData( ItemIdx ) )
					pItem->ShopItem.Param = pItemInfo->SellPrice;
			}
			if( pAvatarEquip->Item[i] == 0 )
			{
				// 같이 벗겨져야할 아바타 아이템들
				if( i>=eAvatar_Hat && i<eAvatar_Weared_Hair )
				{
					ITEM_INFO* poffinfo = ITEMMGR->GetItemInfo( pAvatar[i] );
					if(!poffinfo)		return FALSE;

					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pAvatar[i], poffinfo->SellPrice );
					if( pItem = m_UsingItemTable.GetData( pAvatar[i] ) )
						pItem->ShopItem.Param = poffinfo->SellPrice;
					pAvatar[i] = 0;
				}
			}
		}
	}
	
	SEND_AVATARITEM_INFO msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_INFO;
	msg.PlayerId = m_pPlayer->GetID();
	msg.ItemIdx = ItemIdx;
	msg.ItemPos = ItemPos;
	memcpy( &msg.Avatar, pAvatar, sizeof(WORD)*eAvatar_Max );
	PACKEDDATA_OBJ->QuickSend( m_pPlayer, &msg, sizeof(msg) );	

	//	
	CalcAvatarOption( bCalcStats );

	return TRUE;
}


void CShopItemManager::CalcAvatarOption( BOOL bCalcStats )
{
	WORD* pAvatar = m_pPlayer->GetShopItemStats()->Avatar;
	if(!pAvatar)		return ;

	AVATARITEMOPTION* pAvatarOption = m_pPlayer->GetAvatarOption();
	memset( pAvatarOption, 0, sizeof(AVATARITEMOPTION) );

	for(int i=0; i<eAvatar_Max; i++)
	{
		if( pAvatar[i] < 2 )					continue;

		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pAvatar[i] );
		if( !pItemInfo )		continue;

		// 근골
		if( pItemInfo->GenGol > 0 )
			pAvatarOption->Gengol += pItemInfo->GenGol;
		// 민첩
		if( pItemInfo->MinChub > 0 )
			pAvatarOption->Minchub += pItemInfo->MinChub;
		// 체력
		if( pItemInfo->CheRyuk > 0 )
			pAvatarOption->Cheryuk += pItemInfo->CheRyuk;
		// 심맥
		if( pItemInfo->SimMek > 0 )
			pAvatarOption->Simmek += pItemInfo->SimMek;
		// 최대생명력
		if( pItemInfo->Life > 0 )
			pAvatarOption->Life += (WORD)pItemInfo->Life;
		// 최대호신강기
		if( pItemInfo->Shield > 0 )
			pAvatarOption->Shield += (WORD)pItemInfo->Shield;
		// 최대내력
		if( pItemInfo->NaeRyuk > 0 )
			pAvatarOption->Naeruyk += pItemInfo->NaeRyuk;
		// 무기공격력
		if( pItemInfo->MeleeAttackMin > 0 )
			pAvatarOption->Attack += pItemInfo->MeleeAttackMin;
		// 크리티컬수치
		if( pItemInfo->CriticalPercent > 0 )
			pAvatarOption->Critical += pItemInfo->CriticalPercent;
		// 반격확율
		if( pItemInfo->Plus_MugongIdx > 0 )
			pAvatarOption->CounterPercent += pItemInfo->Plus_MugongIdx;
		// 반격시 데미지율
		if( pItemInfo->Plus_Value > 0 )
			pAvatarOption->CounterDamage += pItemInfo->Plus_Value;
		// 경공딜레이
		if( pItemInfo->AllPlus_Kind == 1 )
			pAvatarOption->bKyungGong = 1;
		// 내력소모감소
		if( pItemInfo->LimitCheRyuk > 0 )
			pAvatarOption->NeaRyukSpend += pItemInfo->LimitCheRyuk;
		// 내공 데미지
		if( pItemInfo->LimitJob > 0 )
			pAvatarOption->NeagongDamage += pItemInfo->LimitJob;
		// 외공 데미지
		if( pItemInfo->LimitGender > 0 )
			pAvatarOption->WoigongDamage += pItemInfo->LimitGender;
		// 몬스터 물리방어력 감소
		if( pItemInfo->LimitLevel > 0 )
			pAvatarOption->TargetPhyDefDown += pItemInfo->LimitLevel;
		// 몬스터 속성저항력 감소
		if( pItemInfo->LimitGenGol > 0 )
			pAvatarOption->TargetAttrDefDown += pItemInfo->LimitGenGol;
		// 몬스터 공격력 감소
		if( pItemInfo->LimitMinChub > 0 )
			pAvatarOption->TargetAtkDown += pItemInfo->LimitMinChub;
		// 회복량 추가 증가치
		if( pItemInfo->LimitSimMek )
			pAvatarOption->RecoverRate += pItemInfo->LimitSimMek;
		// 경공속도 상승
		if( pItemInfo->ItemGrade )
			pAvatarOption->KyunggongSpeed += pItemInfo->ItemGrade;
		// 묵혼게이지 충전속도
        if( pItemInfo->RangeType )
			pAvatarOption->MussangCharge += pItemInfo->RangeType;
		// 경공사용시 내력소모여부
		if( pItemInfo->EquipKind == 1 )
			pAvatarOption->NaeruykspendbyKG = 1;
		///////////////////////////////////////////////////////////////////////////////////////////////////
		// 06. 07 내공 적중(일격) - 이영준
		if( pItemInfo->NaeRyukRecover > 0 )
			pAvatarOption->Decisive += pItemInfo->NaeRyukRecover;
		///////////////////////////////////////////////////////////////////////////////////////////////////
		// 내력회복시 추가 호신강기 회복량
		if( pItemInfo->RangeAttackMin > 0 )
			pAvatarOption->ShieldRecoverRate += pItemInfo->RangeAttackMin;
		// 묵혼발동시 증가공격력
		if( pItemInfo->RangeAttackMax > 0 )
			pAvatarOption->MussangDamage += pItemInfo->RangeAttackMax;
	}

	/* //SW 벚꽃 이벤트 관련 임시 하드 코드
	// 2005 크리스마스 이벤트
	// 눈내릴때 추가옵션 적용
	if( WEATHERMGR->GetWeatherState() == eWS_Snow )
	{
		if(	(pAvatar[eAvatar_Dress] == EVENT_SHOPITEM_SNOWMAN_DRESS && pAvatar[eAvatar_Hat] == EVENT_SHOPITEM_SNOWMAN_HAT) ||
			(pAvatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP_DRESS && pAvatar[eAvatar_Hat] ==  EVENT_SHOPITEM_RUDOLP_HAT) ||
			(pAvatar[eAvatar_Dress] == EVENT_SHOPITEM_SNOWMAN_DRESS2 && pAvatar[eAvatar_Hat] == EVENT_SHOPITEM_SNOWMAN_HAT2) ||
			(pAvatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP_DRESS2 && pAvatar[eAvatar_Hat] ==  EVENT_SHOPITEM_RUDOLP_HAT2) ||
			(pAvatar[eAvatar_Dress] == EVENT_SHOPITEM_SNOWMAN_DRESS && pAvatar[eAvatar_Hat] == EVENT_SHOPITEM_SNOWMAN_HAT2) ||
			(pAvatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP_DRESS && pAvatar[eAvatar_Hat] ==  EVENT_SHOPITEM_RUDOLP_HAT2) ||
			(pAvatar[eAvatar_Dress] == EVENT_SHOPITEM_SNOWMAN_DRESS2 && pAvatar[eAvatar_Hat] == EVENT_SHOPITEM_SNOWMAN_HAT) ||
			(pAvatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP_DRESS2 && pAvatar[eAvatar_Hat] ==  EVENT_SHOPITEM_RUDOLP_HAT) ||
			pAvatar[eAvatar_Dress] ==  EVENT_SHOPITEM_SNOWMAN1_HK || pAvatar[eAvatar_Dress] ==  EVENT_SHOPITEM_SNOWMAN2_HK ||
			pAvatar[eAvatar_Dress] ==  EVENT_SHOPITEM_SNOWMAN3_HK || pAvatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP1_HK ||
			pAvatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP2_HK || pAvatar[eAvatar_Dress] ==  EVENT_SHOPITEM_RUDOLP3_HK )
		{
			pAvatarOption->Life += 100;
			pAvatarOption->Shield += 100;
			pAvatarOption->Naeruyk += 100;
		}
	}*/

#define SHOPITEM_COS_CAT_HAT		56019
#define SHOPITEM_COS_CAT_DRESS		56020
#define SHOPITEM_COS_WEDDING_MAN	56021
#define SHOPITEM_COS_WEDDING_WOMAN	56022

	if( WEATHERMGR->GetWeatherState() == eWS_Snow )
	{
		if( pAvatar[eAvatar_Hat] == SHOPITEM_COS_CAT_HAT )
		{
			pAvatarOption->Life += 50;
			pAvatarOption->Shield += 50;
			pAvatarOption->Naeruyk += 50;
			pAvatarOption->RecoverRate += 10;
			pAvatarOption->NaeruykspendbyKG = 1;
		}

		if( pAvatar[eAvatar_Dress] == SHOPITEM_COS_CAT_DRESS )
		{
			pAvatarOption->Life += 50;
			pAvatarOption->Shield += 50;
			pAvatarOption->Naeruyk += 50;
			pAvatarOption->KyunggongSpeed += 50;
			pAvatarOption->TargetPhyDefDown += 5;
			pAvatarOption->TargetAttrDefDown += 5;
		}

		if( pAvatar[eAvatar_Dress] == SHOPITEM_COS_WEDDING_MAN ||
			pAvatar[eAvatar_Dress] == SHOPITEM_COS_WEDDING_MAN )
		{
			pAvatarOption->Life += 100;
			pAvatarOption->Shield += 100;
			pAvatarOption->Naeruyk += 100;
			pAvatarOption->RecoverRate += 10;
			pAvatarOption->NaeruykspendbyKG = 1;
			pAvatarOption->KyunggongSpeed += 50;
			pAvatarOption->TargetPhyDefDown += 5;
			pAvatarOption->TargetAttrDefDown += 5;
		}
	}

	if( bCalcStats )
		m_pPlayer->CalcState();
}


void CShopItemManager::DiscardAvatarItem( WORD ItemIdx, WORD ItemPos )
{
	WORD* pAvatar = m_pPlayer->GetShopItemStats()->Avatar;
	if(!pAvatar)		return ;
	AVATARITEM* pAvatarEquip = GAMERESRCMNGR->m_AvatarEquipTable.GetData( ItemIdx );
	if(!pAvatarEquip)	return ;
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( ItemIdx );
	if(!pItemInfo)		return;

	if( pAvatar[pAvatarEquip->Position] != ItemIdx )
		return;

	for(int i=0; i<eAvatar_Max; i++)
	{
		if( i==pAvatarEquip->Position && pAvatar[i] )
		{
			pAvatar[i] = 0;
			for(int n=eAvatar_Weared_Hair; n<eAvatar_Weared_Gum; n++)
			{
				if( !pAvatarEquip->Item[n] )
					pAvatar[n] = 1;
			}
						
			ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), ItemIdx, pItemInfo->SellPrice );
		}
	}

	SEND_AVATARITEM_INFO msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_DISCARD;
	msg.PlayerId = m_pPlayer->GetID();
	msg.ItemIdx = ItemIdx;
	msg.ItemPos = ItemPos;
	memcpy( &msg.Avatar, pAvatar, sizeof(WORD)*eAvatar_Max );
	PACKEDDATA_OBJ->QuickSend( m_pPlayer, &msg, sizeof(msg) );	

	//
	CalcAvatarOption();
}


void CShopItemManager::CalcPlusTime( DWORD dwEventIdx, DWORD dwType )
{	
	ITEM_INFO* pItem = NULL;
	SHOPITEMWITHTIME* pShopItem = NULL;

	//
	m_UsingItemTable.SetPositionHead();

	while( pShopItem = m_UsingItemTable.GetData() )
	{
		pItem = ITEMMGR->GetItemInfo( pShopItem->ShopItem.ItemBase.wIconIdx );
		if( !pItem )		continue;

		if( pItem->ItemKind == eSHOP_ITEM_CHARM )
		{
			switch( dwType )
			{
			case 0:
				{
					if( pShopItem->ShopItem.Remaintime )
					if( gEventRate[pItem->MeleeAttackMin] != gEventRateFile[pItem->MeleeAttackMin] )
						CalcShopItemOption( pItem->ItemIdx, FALSE );
				}
				break;
			case MP_CHEAT_PLUSTIME_ON:
				{
					if( dwEventIdx == pItem->MeleeAttackMin && pShopItem->ShopItem.Remaintime )
					{
						CalcShopItemOption( pItem->ItemIdx, FALSE );
						return;
					}
				}
				break;
			case MP_CHEAT_PLUSTIME_OFF:
				{
					if( dwEventIdx == pItem->MeleeAttackMin && pShopItem->ShopItem.Remaintime )
					{
						pShopItem->LastCheckTime = gCurTime;
						CalcShopItemOption( pItem->ItemIdx, TRUE );
						return;
					}
				}
				break;
			case MP_CHEAT_PLUSTIME_ALLOFF:
				{
					if( pShopItem->ShopItem.Remaintime )
					if( gEventRate[pItem->MeleeAttackMin] != gEventRateFile[pItem->MeleeAttackMin] )
					{
						pShopItem->LastCheckTime = gCurTime;
						CalcShopItemOption( pItem->ItemIdx, TRUE );
					}
				}
				break;
			}
		}
	}	
}


void CShopItemManager::AddDupParam( ITEM_INFO* pItemInfo )
{
	if( !pItemInfo )			return;

	// 부적
	if( pItemInfo->AllPlus_Value )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->AllPlus_Value );
		if( !pDupOption )		return;

		if( pDupOption->Param & eDontDupUse_WoigongDamage )			// 귀신의 부적(외공데미지)
			m_DupCharm |= eDontDupUse_WoigongDamage;
		if( pDupOption->Param & eDontDupUse_NaegongDamage )			// 강시의 부적(내공데미지)
			m_DupCharm |= eDontDupUse_NaegongDamage;	
		if( pDupOption->Param & eDontDupUse_Exppoint )				// 플러스타임 경험치
			m_DupCharm |= eDontDupUse_Exppoint;	
		if( pDupOption->Param & eDontDupUse_Reinforce )				// 강화증폭
			m_DupCharm |= eDontDupUse_Reinforce;	
		if( pDupOption->Param & eDontDupUse_Kyunggong )				// 경공의부적
			m_DupCharm |= eDontDupUse_Kyunggong;
		//2007. 10. 9. CBH - 요괴. 외공, 내공 주문서 중복 처리 추가
		if( pDupOption->Param & eDontDupUse_Ghost )					// 요괴의부적
			m_DupCharm |= eDontDupUse_Ghost;			
		if( pDupOption->Param & eDontDupUse_Woigong )				// 외공의부적
			m_DupCharm |= eDontDupUse_Woigong;			
		if( pDupOption->Param & eDontDupUse_Naegong )				// 내공의부적
			m_DupCharm |= eDontDupUse_Naegong;			
		// 사냥꾼 부적/경험치 부적 추가 by Stiner(2008/06/25)
		if( pDupOption->Param & eDontDupUse_Hunter )				// 사냥꾼 부적
			m_DupCharm |= eDontDupUse_Hunter;
		if( pDupOption->Param & eDontDupUse_ExpDay )				// 경험치 부적
			m_DupCharm |= eDontDupUse_ExpDay;
	}
	
	// 물약
	if( pItemInfo->MugongNum )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->MugongNum );
		if( !pDupOption )		return;

		if( pDupOption->Param & eDontDupUse_Life )				// 생명의물약
			m_DupHerb |= eDontDupUse_Life;
		if( pDupOption->Param & eDontDupUse_Shield )			// 호신의물약
			m_DupHerb |= eDontDupUse_Shield;
		if( pDupOption->Param & eDontDupUse_Naeruyk )			// 내력의물약
			m_DupHerb |= eDontDupUse_Naeruyk;
		if( pDupOption->Param & eDontDupUse_GreateLife )		// 강화생명의물약
			m_DupHerb |= eDontDupUse_GreateLife;
		if( pDupOption->Param & eDontDupUse_GreateShield )		// 강화호신의물약
			m_DupHerb |= eDontDupUse_GreateShield;
		if( pDupOption->Param & eDontDupUse_GreateNaeruyk )		// 강화내력의물약
			m_DupHerb |= eDontDupUse_GreateNaeruyk;
		if( pDupOption->Param & eDontDupUse_EventSatang )		// 이벤트사탕
			m_DupHerb |= eDontDupUse_EventSatang;
		if( pDupOption->Param & eDontDupUse_Doll )				// 
			m_DupHerb |= eDontDupUse_Doll;

		
	}

	// 주문서
	if( pItemInfo->MugongType )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->MugongType );
		if( !pDupOption )		return;

		if( pDupOption->Param & eDontDupUse_MemoryMove )		// 기억이동 주문서
			m_DupIncantation |= eDontDupUse_MemoryMove;			
		if( pDupOption->Param & eDontDupUse_ProtectAll )		// 수호자의 주문서
			m_DupIncantation |= eDontDupUse_ProtectAll;
		if( pDupOption->Param & eDontDupUse_LevelCancel50 )		// 50렙이하 장비해제
			m_DupIncantation |= eDontDupUse_LevelCancel50;
		if( pDupOption->Param & eDontDupUse_LevelCancel70 )		// 70렙이하 장비해제
			m_DupIncantation |= eDontDupUse_LevelCancel70;
		if( pDupOption->Param & eDontDupUse_LevelCancel90 )		// 90렙이하 장비해제
			m_DupIncantation |= eDontDupUse_LevelCancel90;
		if( pDupOption->Param & eDontDupUse_ShowPyoguk )		// 창고소환주문서
			m_DupIncantation |= eDontDupUse_ShowPyoguk;
		if( pDupOption->Param & eDontDupUse_Chase )				// 추적주문서
			m_DupIncantation |= eDontDupUse_Chase;
		if( pDupOption->Param & eDontDupUse_TownMove )			// 마을이동주문서
			m_DupIncantation |= eDontDupUse_TownMove;
	}

	if( pItemInfo->LifeRecover )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->LifeRecover );
		if( !pDupOption )		return;

		if( pDupOption->Param & eDontDupUse_StreeStall )		// 노점개설
		{
			m_DupSundries |= eDontDupUse_StreeStall;
			m_pPlayer->GetShopItemStats()->bStreetStall = 1;
		}
	}
	if( pItemInfo->LifeRecoverRate )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( (DWORD)pItemInfo->LifeRecoverRate );
		if( !pDupOption )		return;

		if( pDupOption->Param & eDupPetEquip_PomanRing )		// 포만의귀걸이
		{
			m_DupPetEquip |= eDupPetEquip_PomanRing;
		}
	}
}


void CShopItemManager::DeleteDupParam( ITEM_INFO* pItemInfo )
{
	if( !pItemInfo )			return;

	// 부적
	if( pItemInfo->AllPlus_Value )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->AllPlus_Value );
		if( !pDupOption )		return;

		if( (pDupOption->Param & eDontDupUse_WoigongDamage) &&
			(m_DupCharm & eDontDupUse_WoigongDamage) )				// 귀신의 부적(외공데미지)
			m_DupCharm ^= eDontDupUse_WoigongDamage;
		if( (pDupOption->Param & eDontDupUse_NaegongDamage) &&
			(m_DupCharm & eDontDupUse_NaegongDamage) )				// 강시의부적(내공데미지)
			m_DupCharm ^= eDontDupUse_NaegongDamage;	
		if( (pDupOption->Param & eDontDupUse_Exppoint) &&
			(m_DupCharm & eDontDupUse_Exppoint) )					// 플러스타임 경험치
			m_DupCharm ^= eDontDupUse_Exppoint;
		if( (pDupOption->Param & eDontDupUse_Reinforce) &&
			(m_DupCharm & eDontDupUse_Reinforce) )					// 강화증폭
			m_DupCharm ^= eDontDupUse_Reinforce;
		if( (pDupOption->Param & eDontDupUse_Kyunggong) &&
			(m_DupCharm & eDontDupUse_Kyunggong) )					// 경공의부적
			m_DupCharm ^= eDontDupUse_Kyunggong;
		//2007. 10. 9. CBH - 요괴. 외공, 내공 주문서 중복 처리 추가
		if( pDupOption->Param & eDontDupUse_Ghost )					// 요괴의부적
			m_DupCharm ^= eDontDupUse_Ghost;			
		if( pDupOption->Param & eDontDupUse_Woigong )				// 외공의부적
			m_DupCharm ^= eDontDupUse_Woigong;			
		if( pDupOption->Param & eDontDupUse_Naegong )				// 내공의부적
			m_DupCharm ^= eDontDupUse_Naegong;			
		// 사냥꾼 부적/경험치 부적 추가 by Stiner(2008/06/25)
		if( pDupOption->Param & eDontDupUse_Hunter )				// 사냥꾼 부적
			m_DupCharm ^= eDontDupUse_Hunter;
		if( pDupOption->Param & eDontDupUse_ExpDay )				// 경험치 부적
			m_DupCharm ^= eDontDupUse_ExpDay;
	}
	
	// 물약
	if( pItemInfo->MugongNum )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->MugongNum );
		if( !pDupOption )		return;

		if( (pDupOption->Param & eDontDupUse_Life) &&
			(m_DupHerb & eDontDupUse_Life) )				// 생명의물약
			m_DupHerb ^= eDontDupUse_Life;
		if( (pDupOption->Param & eDontDupUse_Shield) &&
			(m_DupHerb & eDontDupUse_Shield) )				// 호신의물약
			m_DupHerb ^= eDontDupUse_Shield;
		if( (pDupOption->Param & eDontDupUse_Naeruyk) &&
			(m_DupHerb & eDontDupUse_Naeruyk) )				// 내력의물약
			m_DupHerb ^= eDontDupUse_Naeruyk;
		if( (pDupOption->Param & eDontDupUse_GreateLife) &&
			(m_DupHerb & eDontDupUse_GreateLife) )				// 강화생명의물약
			m_DupHerb ^= eDontDupUse_GreateLife;
		if( (pDupOption->Param & eDontDupUse_GreateShield) &&
			(m_DupHerb & eDontDupUse_GreateShield) )				// 강화호신의물약
			m_DupHerb ^= eDontDupUse_GreateShield;
		if( (pDupOption->Param & eDontDupUse_GreateNaeruyk) &&
			(m_DupHerb & eDontDupUse_GreateNaeruyk) )				// 강화내력의물약
			m_DupHerb ^= eDontDupUse_GreateNaeruyk;
		if( (pDupOption->Param & eDontDupUse_EventSatang) &&
			(m_DupHerb & eDontDupUse_EventSatang) )				// 이벤트사탕
			m_DupHerb ^= eDontDupUse_EventSatang;
		if( (pDupOption->Param & eDontDupUse_Doll) &&
			(m_DupHerb & eDontDupUse_Doll) )				// 이벤트사탕
			m_DupHerb ^= eDontDupUse_Doll;
	}

	// 주문서
	if( pItemInfo->MugongType )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->MugongType );
		if( !pDupOption )		return;

		if( (pDupOption->Param & eDontDupUse_MemoryMove) && 
			(m_DupIncantation & eDontDupUse_MemoryMove) )					// 기억이동 주문서
			m_DupIncantation ^= eDontDupUse_MemoryMove;			
		if( (pDupOption->Param & eDontDupUse_ProtectAll) &&
			(m_DupIncantation & eDontDupUse_ProtectAll) )					// 수호자의 주문서
			m_DupIncantation ^= eDontDupUse_ProtectAll;
	}

	if( pItemInfo->LifeRecover )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->LifeRecover );
		if( !pDupOption )		return;

		if( (pDupOption->Param & eDontDupUse_StreeStall) &&
			(m_DupSundries & eDontDupUse_StreeStall) )					// 노점개설
		{
			m_DupSundries ^= eDontDupUse_StreeStall;
			m_pPlayer->GetShopItemStats()->bStreetStall = 0;
		}
	}
	if( pItemInfo->LifeRecoverRate )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( (DWORD)pItemInfo->LifeRecoverRate );
		if( !pDupOption )		return;

		if( (pDupOption->Param & eDupPetEquip_PomanRing) &&
			(m_DupPetEquip & eDupPetEquip_PomanRing) )					// 포만의귀걸이
		{
			m_DupPetEquip ^= eDupPetEquip_PomanRing;			
		}
	}
}


BOOL CShopItemManager::IsDupAble( ITEM_INFO* pItemInfo )
{
	if( !pItemInfo )			return FALSE;

	// 부적
	if( pItemInfo->AllPlus_Value )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->AllPlus_Value );
		if( !pDupOption )		return FALSE;

		if( (pDupOption->Param & eDontDupUse_WoigongDamage) &&			// 귀신의 부적(외공데미지)
			(m_DupCharm & eDontDupUse_WoigongDamage) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_NaegongDamage) &&			// 강시의 부적(내공데미지)
			(m_DupCharm & eDontDupUse_NaegongDamage) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_Exppoint) &&				// 플러스타임 경험치
			(m_DupCharm & eDontDupUse_Exppoint) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_Reinforce) &&				// 강화증폭
			(m_DupCharm & eDontDupUse_Reinforce) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_Kyunggong) &&				// 경공의부적
			(m_DupCharm & eDontDupUse_Kyunggong) )
			return FALSE;
		//2007. 10. 9. CBH - 요괴. 외공, 내공 주문서 중복 처리 추가
		if( (pDupOption->Param & eDontDupUse_Ghost) &&				// 요괴의부적
			(m_DupCharm & eDontDupUse_Ghost) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_Woigong) &&				// 외공의부적
			(m_DupCharm & eDontDupUse_Woigong) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_Naegong) &&				// 내공의부적
			(m_DupCharm & eDontDupUse_Naegong) )
			return FALSE;		
		// 사냥꾼 부적/경험치 부적 추가 by Stiner(2008/06/25)
		if( (pDupOption->Param & eDontDupUse_Hunter) && 				// 사냥꾼 부적
			(m_DupCharm & eDontDupUse_Hunter) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_ExpDay) &&				// 경험치 부적
			(m_DupCharm & eDontDupUse_ExpDay) )
			return FALSE;
	}
	
	// 물약
	if( pItemInfo->MugongNum )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->MugongNum );
		if( !pDupOption )		return FALSE;

		if( (pDupOption->Param & eDontDupUse_Life) &&			// 생명의물약
			(m_DupHerb & eDontDupUse_Life) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_Shield) &&			// 호신의물약
			(m_DupHerb & eDontDupUse_Shield) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_Naeruyk) &&			// 내력의물약
			(m_DupHerb & eDontDupUse_Naeruyk) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_GreateLife) &&			// 강화생명의물약
			(m_DupHerb & eDontDupUse_GreateLife) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_GreateShield) &&			// 강화호신의물약
			(m_DupHerb & eDontDupUse_GreateShield) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_GreateNaeruyk) &&			// 강화내력의물약
			(m_DupHerb & eDontDupUse_GreateNaeruyk) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_EventSatang) &&			// 이벤트사탕
			(m_DupHerb & eDontDupUse_EventSatang) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_Doll) &&			// 이벤트사탕
			(m_DupHerb & eDontDupUse_Doll) )
			return FALSE;
	}

	// 주문서
	if( pItemInfo->MugongType )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->MugongType );
		if( !pDupOption )		return FALSE;

		if( (pDupOption->Param & eDontDupUse_MemoryMove) &&			// 기억이동 주문서
			(m_DupIncantation & eDontDupUse_MemoryMove) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_ProtectAll) &&			// 수호자의 주문서
			(m_DupIncantation & eDontDupUse_ProtectAll) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_LevelCancel50) &&		// 수호자의 주문서
			(m_DupIncantation & eDontDupUse_LevelCancel50) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_LevelCancel70) &&		// 수호자의 주문서
			(m_DupIncantation & eDontDupUse_LevelCancel70) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_LevelCancel90) &&		// 수호자의 주문서
			(m_DupIncantation & eDontDupUse_LevelCancel90) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_ShowPyoguk) &&			// 창고소환 주문서
			(m_DupIncantation & eDontDupUse_ShowPyoguk) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_Chase) &&				// 추적 주문서
			(m_DupIncantation & eDontDupUse_Chase) )
			return FALSE;
		if( (pDupOption->Param & eDontDupUse_TownMove) &&			// 마을이동 주문서
			(m_DupIncantation & eDontDupUse_TownMove) )
			return FALSE;
	}

	// 잡화류
	if( pItemInfo->LifeRecover )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( pItemInfo->LifeRecover );
		if( !pDupOption )		return FALSE;

		if( (pDupOption->Param & eDontDupUse_StreeStall) &&			// 노점개설
			(m_DupSundries & eDontDupUse_StreeStall) )
			return FALSE;
	}

	if( pItemInfo->LifeRecoverRate )
	{
		SHOPITEMDUP* pDupOption = GAMERESRCMNGR->m_ShopItemDupOptionTable.GetData( (DWORD)pItemInfo->LifeRecoverRate );
		if( !pDupOption )		return FALSE;

		if( (pDupOption->Param & eDupPetEquip_PomanRing) &&			// 포만의귀걸이
			(m_DupPetEquip & eDupPetEquip_PomanRing) )
			return FALSE;
	}

	return TRUE;
}

// 2007. 12. 5. CBH - 선택한 스킨 아이탬 세트 인덱스 처리
DWORD CShopItemManager::PutSkinSelectItem(MSG_DWORD2* pMsg)
{
	SKIN_SELECT_ITEM_INFO* pSkinInfo = NULL;
		 
	DWORD dwSkinIndex = pMsg->dwData1;
	DWORD dwSkinKind = pMsg->dwData2;

	if(dwSkinIndex < 1)
		return eSkinResult_Fail;

	switch(dwSkinKind)
	{
	case eSHOP_ITEM_NOMALCLOTHES_SKIN:
		{
			pSkinInfo = GAMERESRCMNGR->GetNomalClothesSkinList(dwSkinIndex);
		}
		break;
	case eSHOP_ITEM_COSTUME_SKIN:
		{
			pSkinInfo = GAMERESRCMNGR->GetCostumeSkinList(dwSkinIndex);
		}
		break;
	}
	
	if(pSkinInfo == NULL)
		return eSkinResult_Fail;

	//레벨 체크 (일반의복일때만 레벨 체크 한다.)
	if(dwSkinKind == eSHOP_ITEM_NOMALCLOTHES_SKIN)
	{
		if(m_pPlayer->GetLevel() < pSkinInfo->dwLimitLevel)
			return eSkinResult_LevelFail;
	}	

	//딜레이 체크
	if(m_pPlayer->GetSkinDelayResult() == TRUE)
		return eSkinResult_DelayFail;

	//스킨 인덱스의 데이터(3개)를 체크하고 적용한다.
	for(int i = 0 ; i < SKINITEM_LIST_MAX ; i++)
	{
		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pSkinInfo->wEquipItem[i] );
		if(pItemInfo == NULL)
			continue;

		//스킨 부위 정보를 체크하여 적용한다.
		DWORD dwSkinType = pItemInfo->Part3DType;
		DWORD dwEquipIndex = 0;

        if( (dwSkinType == eSkinItem_Hat) || (dwSkinType == 6) ) //모자나 머리띠(6)
			dwEquipIndex = eSkinItem_Hat;
		else if(dwSkinType == eSkinItem_Mask)	//안대, 가면, 복면
			dwEquipIndex = eSkinItem_Mask;
		else if(dwSkinType == eSkinItem_Dress)
			dwEquipIndex = eSkinItem_Dress;
		else if(dwSkinType == eSkinItem_Shoes)
			dwEquipIndex = eSkinItem_Shoes;

		m_pPlayer->GetShopItemStats()->wSkinItem[dwEquipIndex] = pItemInfo->ItemIdx;		

		//코스튬 옷은 신발과 일체형이므로 적용시 신발은 벗겨준다.
		if( (dwSkinType == eSkinItem_Dress) && (dwSkinKind == eSHOP_ITEM_COSTUME_SKIN) )
			m_pPlayer->GetShopItemStats()->wSkinItem[eSkinItem_Shoes] = 0;
	}

	return eSkinResult_Success;
}

// 2007. 12. 11. CBH - 스킨 아이탬 삭제시 관련 처리
void CShopItemManager::DiscardSkinItem(DWORD dwItemIndex)
{
	ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( dwItemIndex );
	if(!pItemInfo)		return;

	//적용되어진 스킨의 종류가 같은 전부 초기화 한다.
	RemoveEquipSkin(pItemInfo->ItemKind);
	CharacterSkinInfoUpdate(m_pPlayer);

	SEND_SKIN_INFO msg;
	msg.Category = MP_ITEMEXT;
	msg.Protocol = MP_ITEMEXT_SKINITEM_DISCARD_ACK;
	msg.dwObjectID = m_pPlayer->GetID();
	memcpy( &msg.wSkinItem, m_pPlayer->GetShopItemStats()->wSkinItem, sizeof(WORD)*eSkinItem_Max );
	PACKEDDATA_OBJ->QuickSend( m_pPlayer, &msg, sizeof(msg) );
}

// 2008. 1. 21. CBH - 장착된 스킨 아이탬 삭제
void CShopItemManager::RemoveEquipSkin(DWORD dwSkinKind)
{
	SKIN_SELECT_ITEM_INFO* pSkinInfo = NULL;
	CYHHashTable<SKIN_SELECT_ITEM_INFO>* pSkinListTable = NULL;
	WORD* wSkinItem = m_pPlayer->GetShopItemStats()->wSkinItem;
		
	//Skin 종류에 따라 해당 테이블을 가져옴
	switch(dwSkinKind)
	{
	case eSHOP_ITEM_NOMALCLOTHES_SKIN:
		{
			pSkinListTable = GAMERESRCMNGR->GetNomalClothesSkinTable();			
		}
		break;
	case eSHOP_ITEM_COSTUME_SKIN:
		{	
			pSkinListTable = GAMERESRCMNGR->GetCostumeSkinTable();
		}
		break;
	}

	if(pSkinListTable == NULL)
		return;

	//같은 종류의 스킨 장비를 찾아 일괄 초기화한다.
	DWORD dwSkinMaxNum = pSkinListTable->GetDataNum();
	pSkinListTable->SetPositionHead();	
	for(int nCount = 0 ; nCount < dwSkinMaxNum ; nCount++)
	{
		pSkinInfo = NULL;
		pSkinInfo = pSkinListTable->GetData();
		if(pSkinInfo == NULL)
			continue;

		for(int i = 0 ; i < eSkinItem_Max ; i++)
		{			
			if(wSkinItem[i] == 0)
				continue;

			for(int j = 0 ; j < SKINITEM_LIST_MAX ; j++)
			{
				if(wSkinItem[i] == pSkinInfo->wEquipItem[j])
				{
					wSkinItem[i] = 0;
				}
			}		
		}		
	}
}

void CShopItemManager::AddUsingShopItem(SHOPITEMWITHTIME* pShopItem, WORD dwItemIndex)
{
	SHOPITEMWITHTIME* pAddShopItem = m_UsingItemPool->Alloc();
	memcpy(pAddShopItem, pShopItem, sizeof(SHOPITEMWITHTIME));

	m_UsingItemTable.Add(pAddShopItem, dwItemIndex);
}

int CShopItemManager::UseShopItem( ITEMBASE* pItemBase, SHOPITEMUSEBASE UseBaseInfo, SHOPITEMBASE* pShopItem )
{
	if( !pItemBase )
		return eItemUseErr_Err;

	if( EVENTMAPMGR->IsEventMap() )
		return eItemUseErr_Err;

	// 보스몹이 있는 맵은 기억석으로 저장 할 수 없음.
	DWORD dwCheckBit = eBossMap | eSiegeWarMap | eQuestRoom | eEventMap | eSurvivalMap;
	if( pItemBase->wIconIdx == eIncantation_MemoryStone &&
		g_pServerSystem->GetMap()->IsMapKind(dwCheckBit) )
		//( g_pServerSystem->GetMapNum() == BOSSMONSTER_MAP 		
		//|| g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetSiegeMapNum()
		//|| g_pServerSystem->GetMapNum() == QUESTMAPNUM1
		//|| g_pServerSystem->GetMapNum() == QUESTMAPNUM2
		//|| g_pServerSystem->GetMapNum() == QUESTMAPNUM3
		//|| g_pServerSystem->GetMapNum() == BOSSMONSTER_2NDMAP 
		//|| g_pServerSystem->GetMapNum() == RUNNINGMAP
		//|| g_pServerSystem->GetMapNum() == PKEVENTMAP
		//|| g_pServerSystem->CheckMapKindIs(eSurvival) ) )
		return eItemUseErr_Err;


	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(UseBaseInfo.ShopItemIdx);
	if(!pItemInfo)		return eItemUseErr_Err;

	//2007. 9. 21. CBH - 샾 아이탬 레벨제한 체크 추가
	if(pItemInfo->NaeRyukRecoverRate > m_pPlayer->GetLevel())
	{
		return eItemUseErr_Err;
	}


	// 05.05.20  문파토너먼트시에 사용불가능 아이템
	//if( g_pServerSystem->GetMapNum() == Tournament )
	if( g_pServerSystem->GetMap()->IsMapKind(eTournament) )
	{
		if( pItemBase->wIconIdx == eIncantation_MemoryStone ||
			(pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION && pItemInfo->LimitLevel) )
			return eItemUseErr_Err;
	}



	//공성전에서는 기억석 사용 못함
	if( m_pPlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_SIEGEWAR )
	{
		if( pItemBase->wIconIdx == eIncantation_MemoryStone )
			return eItemUseErr_Err;

		if( m_pPlayer->GetBattleTeam() == 2 ) //관람모드
		{
			if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION && pItemInfo->LimitLevel )
				return eItemUseErr_Err;
		}
	}


	//	임시로 넣음 - 성대
	/*	SHOPITEMWITHTIME* pAddShopItem = NULL;
	if( pItemBase->wIconIdx == 55134 || pItemBase->wIconIdx == 55142 )
	{
	pAddShopItem = m_UsingItemTable.GetData(55134);
	if( !pAddShopItem )
	{
	pAddShopItem = m_UsingItemTable.GetData(55142);
	}
	}
	else
	{
	pAddShopItem = m_UsingItemTable.GetData(pItemBase->wIconIdx);
	}*/
	//
	SHOPITEMWITHTIME* pUsingShopItem = NULL;
	pUsingShopItem = m_UsingItemTable.GetData(pItemBase->wIconIdx);

	if( pUsingShopItem )
	{
		if( pItemInfo->SellPrice && pItemInfo->ItemType == 10 )
		{

			// 개인플러스타임 체크
			if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM && pItemInfo->MeleeAttackMin )
			{
				if( pUsingShopItem->ShopItem.Remaintime )
					return eItemUseErr_AlreadyUse;
				else
				{
					/*
					stTIME ct;
					ct.value = GetCurTime();
					if( pAddShopItem->ShopItem.BeginTime.GetYear() == ct.GetYear() && 
					pAddShopItem->ShopItem.BeginTime.GetMonth() == ct.GetMonth() &&
					pAddShopItem->ShopItem.BeginTime.GetDay() == ct.GetDay() )
					return eItemuseErr_DontUseToday;
					else
					{
					// 기존에 사용했던 개인플러스타임 삭제
					ShopItemDeleteToDB( m_pPlayer->GetID(), pAddShopItem->ShopItem.ItemBase.dwDBIdx );
					m_UsingItemTable.Remove( pAddShopItem->ShopItem.ItemBase.wIconIdx );
					m_UsingItemPool->Free( pAddShopItem );
					pAddShopItem = NULL;
					}
					*/
					// 기존에 사용했던 개인플러스타임 삭제
					ShopItemDeleteToDB( m_pPlayer->GetID(), pUsingShopItem->ShopItem.ItemBase.dwDBIdx );
					m_UsingItemTable.Remove( pUsingShopItem->ShopItem.ItemBase.wIconIdx );
					m_UsingItemPool->Free( pUsingShopItem );
					pUsingShopItem = NULL;
				}				
			}
#ifdef _JAPAN_LOCAL_
			else if( pItemInfo->ItemIdx == eIncantation_InvenExtend && m_pPlayer->GetExtraInvenSlot() >= 2 )
				return eItemUseErr_AlreadyUse;
			else if( pItemInfo->ItemIdx == eIncantation_PyogukExtend && m_pPlayer->GetExtraPyogukSlot() >= EXTRA_PYOGUK_SLOT )
				return eItemUseErr_AlreadyUse;
			else if( pItemInfo->ItemIdx == eIncantation_MugongExtend && m_pPlayer->GetExtraMugongSlot() >= 2)
				return eItemUseErr_AlreadyUse;
			else if( pItemInfo->ItemIdx != eIncantation_InvenExtend &&
				pItemInfo->ItemIdx != eIncantation_PyogukExtend &&
				pItemInfo->ItemIdx != eIncantation_MugongExtend)
				return eItemUseErr_AlreadyUse;
		}
#elif defined _HK_LOCAL_
			else if( (pItemInfo->ItemIdx == eIncantation_InvenExtend || pItemInfo->ItemIdx == eIncantation_InvenExtend2) &&
				m_pPlayer->GetExtraInvenSlot() >= 2 )
				return eItemUseErr_AlreadyUse;
			else if( (pItemInfo->ItemIdx == eIncantation_PyogukExtend || pItemInfo->ItemIdx == eIncantation_PyogukExtend2) &&
				m_pPlayer->GetExtraPyogukSlot() >= 3 )
				return eItemUseErr_AlreadyUse;
			else if( (pItemInfo->ItemIdx == eIncantation_MugongExtend || pItemInfo->ItemIdx == eIncantation_MugongExtend2) &&
				m_pPlayer->GetExtraMugongSlot() >= 2)
				return eItemUseErr_AlreadyUse;
			else if( (pItemInfo->ItemIdx == eIncantation_CharacterSlot || pItemInfo->ItemIdx == eIncantation_CharacterSlot2) &&
				m_pPlayer->GetExtraCharacterSlot() >= 3 )
				return eItemUseErr_UseFull;
			else if( pItemInfo->ItemIdx != eIncantation_InvenExtend && pItemInfo->ItemIdx != eIncantation_InvenExtend2 &&
				pItemInfo->ItemIdx != eIncantation_PyogukExtend && pItemInfo->ItemIdx != eIncantation_PyogukExtend2 &&
				pItemInfo->ItemIdx != eIncantation_MugongExtend && pItemInfo->ItemIdx != eIncantation_MugongExtend2 )
				return eItemUseErr_AlreadyUse;
		}
#elif defined _TL_LOCAL_
			else if( pItemInfo->ItemIdx == eIncantation_InvenExtend && m_pPlayer->GetExtraInvenSlot() >= 2 )
				return eItemUseErr_UseFull;
			else if( pItemInfo->ItemIdx == eIncantation_PyogukExtend && m_pPlayer->GetExtraPyogukSlot() >= 3 )
				return eItemUseErr_UseFull;
			else if( pItemInfo->ItemIdx == eIncantation_MugongExtend && m_pPlayer->GetExtraMugongSlot() >= 2)
				return eItemUseErr_UseFull;
			else if( pItemInfo->ItemIdx == eIncantation_CharacterSlot && m_pPlayer->GetExtraCharacterSlot() >= 3 )
				return eItemUseErr_UseFull;
			else if( pItemInfo->ItemIdx != eIncantation_InvenExtend &&
				pItemInfo->ItemIdx != eIncantation_PyogukExtend &&
				pItemInfo->ItemIdx != eIncantation_MugongExtend &&
				pItemInfo->ItemIdx != eIncantation_CharacterSlot )
				return eItemUseErr_AlreadyUse;
		}

#else

			else
				return eItemUseErr_AlreadyUse;
		}
#endif		
	}
#ifdef _HK_LOCAL_
	else
	{
		if( (pItemInfo->ItemIdx == eIncantation_InvenExtend || pItemInfo->ItemIdx == eIncantation_InvenExtend2) &&
			m_pPlayer->GetExtraInvenSlot() >= 2 )
			return eItemUseErr_AlreadyUse;
		else if( (pItemInfo->ItemIdx == eIncantation_PyogukExtend || pItemInfo->ItemIdx == eIncantation_PyogukExtend2) &&
			m_pPlayer->GetExtraPyogukSlot() >= 3 )
			return eItemUseErr_AlreadyUse;
		else if( (pItemInfo->ItemIdx == eIncantation_MugongExtend || pItemInfo->ItemIdx == eIncantation_MugongExtend2) &&
			m_pPlayer->GetExtraMugongSlot() >= 2)
			return eItemUseErr_AlreadyUse;
		else if( (pItemInfo->ItemIdx == eIncantation_CharacterSlot || pItemInfo->ItemIdx == eIncantation_CharacterSlot2) &&
			m_pPlayer->GetExtraCharacterSlot() >= 3 )
			return eItemUseErr_UseFull;
	}
#endif
	/*	if( pItemBase->wIconIdx == eIncantation_MemoryMove15 || 
	pItemBase->wIconIdx == 55357 || pItemBase->wIconIdx == 55362 )
	{
	if( pAddShopItem )
	goto Not_DupAbleCheck;
	}*/

	// 중복사용 체크
	if( pItemBase->wIconIdx != 55353 && pItemBase->wIconIdx != 57504 &&
		pItemBase->wIconIdx != 57505 )
	{
		if( IsDupAble( pItemInfo ) == FALSE )
			return eItemUseErr_DontDupUse;	
	}

	//Not_DupAbleCheck:

	SHOPITEMWITHTIME AddShopItem;
	AddShopItem.ShopItem.ItemBase = *pItemBase;
	AddShopItem.ShopItem.Param = pItemInfo->SellPrice;

	// 사용기간이 있는 아이템 Rarity()
	if( AddShopItem.ShopItem.Param && pItemInfo->ItemType == 10 )
	{
		SYSTEMTIME systime;
		stTIME startime, usetime, endtime;
		GetLocalTime(&systime);
		startime.SetTime(systime.wYear-2000, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, 0);
		AddShopItem.ShopItem.BeginTime = startime;

		if( pItemInfo->SellPrice == eShopItemUseParam_Realtime )
		{
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

			AddShopItem.ShopItem.Remaintime = endtime.value;
			AddShopItem.LastCheckTime = gCurTime;
		}
		else if( pItemInfo->SellPrice == eShopItemUseParam_Playtime )
		{
			DWORD remaintime = 0;
			remaintime = pItemInfo->Rarity*60000;
			AddShopItem.ShopItem.Remaintime = remaintime;
			AddShopItem.LastCheckTime = gCurTime;
		}
		else if( pItemInfo->SellPrice == eShopItemUseParam_Continue )
		{
			AddShopItem.ShopItem.Remaintime = 0;
			AddShopItem.LastCheckTime = 0;
		}
	}

	// 사용하면 바로 없어지는 아이템
	if( pItemInfo->ItemKind == eSHOP_ITEM_HERB )
	{
		if(EI_TRUE != ITEMMGR->DiscardItem( m_pPlayer, UseBaseInfo.ShopItemPos, UseBaseInfo.ShopItemIdx, 1 ))
		{
			return eItemUseErr_Err;
		}

		// 물약
		MSG_DWORD2	msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_EFFECT;
		msg.dwData1 = m_pPlayer->GetID();

		if( pItemInfo->GenGol > 0 )
			msg.dwData2 = eEffect_ShopItem_Life;
		else if( pItemInfo->MinChub > 0 )
			msg.dwData2 = eEffect_ShopItem_Shield;
		else if( pItemInfo->CheRyuk > 0 )
			msg.dwData2 = eEffect_ShopItem_NaeRuyk;
		else if( pItemInfo->Life > 0 )
			msg.dwData2 = eEffect_ShopItem_MaxLife;
		else if( pItemInfo->Shield > 0 )
			msg.dwData2 = eEffect_ShopItem_MaxShield;
		else if( pItemInfo->NaeRyuk > 0 )
			msg.dwData2 = eEffect_ShopItem_MaxNaeRuyk;
		else
			msg.dwData2 = 0;

		if( msg.dwData2 )
			PACKEDDATA_OBJ->QuickSend( m_pPlayer, &msg, sizeof(msg) );	


		// 순간회복
		if( pItemInfo->SellPrice == 0 )
		{
			if( pItemInfo->GenGol > 0 )
				m_pPlayer->AddLife( pItemInfo->GenGol, NULL );
			if( pItemInfo->MinChub > 0 )
				m_pPlayer->AddShield( pItemInfo->MinChub );
			if( pItemInfo->CheRyuk > 0 )
				m_pPlayer->AddNaeRyuk( pItemInfo->CheRyuk, NULL );
		}
		else
		{
			CalcShopItemOption( AddShopItem.ShopItem.ItemBase.wIconIdx, TRUE );
			m_pPlayer->CalcState();
			UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, pItemInfo->SellPrice, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );			
			AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
		}
	}
	// 주문서
	else if( pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION )
	{
		if( pItemInfo->ItemType == 10 )
		{
			// 레벨 제한 해제
			if( pItemInfo->LimitJob )
			{
				if( pItemInfo->LimitGender == 0 && m_pPlayer->GetLevel() > 50 )
					return eItemUseErr_Err;
				else if( pItemInfo->LimitGender == 1 && (m_pPlayer->GetLevel() < 51 || m_pPlayer->GetLevel() > 70 ) )
					return eItemUseErr_Err;
				else if( pItemInfo->LimitGender == 2 && (m_pPlayer->GetLevel() < 71 || m_pPlayer->GetLevel() > 90 ) )
					return eItemUseErr_Err;
			}

			if(EI_TRUE != ITEMMGR->DiscardItem( m_pPlayer, UseBaseInfo.ShopItemPos, UseBaseInfo.ShopItemIdx, 1 ))
				return eItemUseErr_Err;			

			// 돈+경험치 보호, 레벨 제한 해제
			if( pItemInfo->CheRyuk || pItemInfo->LimitJob )
			{
				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
				UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, pItemInfo->CheRyuk, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
				AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
			}
#ifdef _JAPAN_LOCAL_
			//창고확장, 인벤확장
			else if( pItemInfo->ItemIdx == eIncantation_InvenExtend )
			{
				if( m_pPlayer->GetExtraInvenSlot() )
				{
					AddShopItem.ShopItem.Param = m_pPlayer->GetExtraInvenSlot() + 1;
					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pItemInfo->ItemIdx, AddShopItem.ShopItem.Param );
				}
				else
				{
					UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, 1, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
					AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
				}
				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
			}
			else if( pItemInfo->ItemIdx == eIncantation_PyogukExtend )
			{
				if( m_pPlayer->GetExtraPyogukSlot() )
				{
					AddShopItem.ShopItem.Param = m_pPlayer->GetExtraPyogukSlot() + 1;
					ShopItemUseParamUpdateToDB( m_pPlayer->GetUserID(), 0, pItemInfo->ItemIdx, AddShopItem.ShopItem.Param );
				}
				else
				{
					UsingShopItemUpdateToDB( m_pPlayer->GetUserID(), 0, AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, 1, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
					AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
				}
				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
			}
			else if( pItemInfo->ItemIdx == eIncantation_MugongExtend )
			{
				if( m_pPlayer->GetExtraMugongSlot() )
				{
					AddShopItem.ShopItem.Param = m_pPlayer->GetExtraMugongSlot() + 1;
					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pItemInfo->ItemIdx, AddShopItem.ShopItem.Param );				
				}
				else
				{
					UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, 1, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
					AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
				}
				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
			}
			else if( pItemInfo->ItemIdx == eIncantation_CharacterSlot )
			{
				if( m_pPlayer->GetExtraCharacterSlot() )
				{
					AddShopItem.ShopItem.Param = m_pPlayer->GetExtraCharacterSlot() + 1;
					ShopItemUseParamUpdateToDB( m_pPlayer->GetUserID(), 0, pItemInfo->ItemIdx, AddShopItem.ShopItem.Param );
					m_pPlayer->SetExtraCharacterSlot( AddShopItem.ShopItem.Param );
				}
				else
				{
					UsingShopItemUpdateToDB( m_pPlayer->GetUserID(), 0, AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, 1, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
					AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
					m_pPlayer->SetExtraCharacterSlot( 1 );
				}
			}
#elif defined _HK_LOCAL_
			else if( (pItemInfo->ItemIdx == eIncantation_InvenExtend || pItemInfo->ItemIdx == eIncantation_InvenExtend2))
			{
				if( m_pPlayer->GetExtraInvenSlot() )
				{
					AddShopItem.ShopItem.Param = m_pPlayer->GetExtraInvenSlot() + 1;
					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pItemInfo->ItemIdx, AddShopItem.ShopItem.Param );
				}
				else
				{
					UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, 1, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
					AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
				}
				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
			}
			else if( (pItemInfo->ItemIdx == eIncantation_PyogukExtend || pItemInfo->ItemIdx == eIncantation_PyogukExtend2) )
			{
				if( m_pPlayer->GetExtraPyogukSlot() )
				{
					AddShopItem.ShopItem.Param = m_pPlayer->GetExtraPyogukSlot() + 1;
					ShopItemUseParamUpdateToDB( m_pPlayer->GetUserID(), 0, pItemInfo->ItemIdx, AddShopItem.ShopItem.Param );
				}
				else
				{
					UsingShopItemUpdateToDB( m_pPlayer->GetUserID(), 0, AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, 1, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
					AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
				}
				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
			}
			else if( (pItemInfo->ItemIdx == eIncantation_MugongExtend || pItemInfo->ItemIdx == eIncantation_MugongExtend2) )
			{
				if( m_pPlayer->GetExtraMugongSlot() )
				{
					AddShopItem.ShopItem.Param = m_pPlayer->GetExtraMugongSlot() + 1;
					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pItemInfo->ItemIdx, AddShopItem.ShopItem.Param );				
				}
				else
				{
					UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, 1, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
					AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
				}
				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
			}
			else if( (pItemInfo->ItemIdx == eIncantation_CharacterSlot || pItemInfo->ItemIdx == eIncantation_CharacterSlot2) )
			{
				if( m_pPlayer->GetExtraCharacterSlot() )
				{
					AddShopItem.ShopItem.Param = m_pPlayer->GetExtraCharacterSlot() + 1;
					ShopItemUseParamUpdateToDB( m_pPlayer->GetUserID(), 0, pItemInfo->ItemIdx, AddShopItem.ShopItem.Param );
					m_pPlayer->SetExtraCharacterSlot( AddShopItem.ShopItem.Param );
				}
				else
				{
					UsingShopItemUpdateToDB( m_pPlayer->GetUserID(), 0, AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, 1, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
					AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
					m_pPlayer->SetExtraCharacterSlot( 1 );
				}
			}
#elif defined _TL_LOCAL_
			else if( pItemInfo->ItemIdx == eIncantation_InvenExtend )
			{
				if( m_pPlayer->GetExtraInvenSlot() )
				{
					AddShopItem.ShopItem.Param = m_pPlayer->GetExtraInvenSlot() + 1;
					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pItemInfo->ItemIdx, AddShopItem.ShopItem.Param );
				}
				else
				{
					UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, 1, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
					AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
				}
				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
			}
			else if( pItemInfo->ItemIdx == eIncantation_PyogukExtend )
			{
				if( m_pPlayer->GetExtraPyogukSlot() )
				{
					AddShopItem.ShopItem.Param = m_pPlayer->GetExtraPyogukSlot() + 1;
					ShopItemUseParamUpdateToDB( m_pPlayer->GetUserID(), 0, pItemInfo->ItemIdx, AddShopItem.ShopItem.Param );
				}
				else
				{
					UsingShopItemUpdateToDB( m_pPlayer->GetUserID(), 0, AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, 1, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
					AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
				}
				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
			}
			else if( pItemInfo->ItemIdx == eIncantation_MugongExtend )
			{
				if( m_pPlayer->GetExtraMugongSlot() )
				{
					AddShopItem.ShopItem.Param = m_pPlayer->GetExtraMugongSlot() + 1;
					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), pItemInfo->ItemIdx, AddShopItem.ShopItem.Param );				
				}
				else
				{
					UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, 1, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
					AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
				}
				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
			}
			else if( pItemInfo->ItemIdx == eIncantation_CharacterSlot )
			{
				if( m_pPlayer->GetExtraCharacterSlot() )
				{
					AddShopItem.ShopItem.Param = m_pPlayer->GetExtraCharacterSlot()+1;
					ShopItemUseParamUpdateToDB( m_pPlayer->GetUserID(), 0, pItemInfo->ItemIdx, AddShopItem.ShopItem.Param );
					m_pPlayer->SetExtraCharacterSlot( AddShopItem.ShopItem.Param );
				}
				else
				{
					UsingShopItemUpdateToDB( m_pPlayer->GetUserID(), 0, AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, 1, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
					AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
					m_pPlayer->SetExtraCharacterSlot( 1 );
				}
			}
#endif
			// 스탯 재분배
			else if( pItemInfo->GenGol )
			{
				SHOPITEMWITHTIME* pUsedItem = m_UsingItemTable.GetData( eIncantation_StatePoint );
				if( pUsedItem )
				{
					CalcShopItemOption( pItemInfo->ItemIdx, TRUE, pItemInfo->GenGol );
					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), eIncantation_StatePoint, m_pPlayer->GetShopItemStats()->StatePoint );
				}
				else
				{
					// Insert Using Info
					UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), eIncantation_StatePoint, AddShopItem.ShopItem.ItemBase.dwDBIdx, pItemInfo->GenGol, AddShopItem.ShopItem.BeginTime.value, 0 );
					CalcShopItemOption( pItemInfo->ItemIdx, TRUE, pItemInfo->GenGol );
					AddUsingShopItem(&AddShopItem, eIncantation_StatePoint);								
				}

			}
			// 스킬포인트 재분배
			else if( pItemInfo->Life )
			{
				SHOPITEMWITHTIME* pUsedItem = m_UsingItemTable.GetData( eIncantation_SkPointRedist );
				if( pUsedItem )
				{
					CalcShopItemOption( pItemInfo->ItemIdx, TRUE, pItemInfo->Life );
					ShopItemUseParamUpdateToDB( 0, m_pPlayer->GetID(), eIncantation_SkPointRedist, m_pPlayer->GetShopItemStats()->SkillPoint );
				}
				else
				{
					CalcShopItemOption( pItemInfo->ItemIdx, TRUE, pItemInfo->Life );
					UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), eIncantation_SkPointRedist, AddShopItem.ShopItem.ItemBase.dwDBIdx,
						m_pPlayer->GetShopItemStats()->SkillPoint, AddShopItem.ShopItem.BeginTime.value, m_pPlayer->GetShopItemStats()->UseSkillPoint );					
					AddUsingShopItem(&AddShopItem, eIncantation_SkPointRedist);			
				}
			}
			else if( pItemInfo->ItemIdx == eIncantation_MixUp )
			{
				CalcShopItemOption( pItemInfo->ItemIdx, TRUE );
				UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, pItemInfo->SellPrice, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
				AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
			}
			// magi82(41) - 샵아이템 추가(스텟 초기화 주문서)
			// magi82(45) - 스텟 초기화 주문서에 레벨제한을 한다.
#define LEVEL_70		70
#define BASIC_STATUS	12
			else if( pItemInfo->ItemIdx == eIncantation_StatusPointReset_71 ||
					 pItemInfo->ItemIdx == eIncantation_StatusPointReset_1_50 ||
					 pItemInfo->ItemIdx == eIncantation_StatusPointReset_51_70 )
			{
				if( m_pPlayer->GetShopItemStats()->UseStatePoint > 0 )
					return eItemUseErr_Err;

				int nMaxLevel = m_pPlayer->GetMaxLevel();

				if( nMaxLevel < pItemInfo->RangeType || nMaxLevel > pItemInfo->EquipKind )
					return eItemUseErr_Err;

				m_pPlayer->SetGenGol(BASIC_STATUS);
				m_pPlayer->SetMinChub(BASIC_STATUS);
				m_pPlayer->SetCheRyuk(BASIC_STATUS);
				m_pPlayer->SetSimMek(BASIC_STATUS);

				DWORD dwMaxLevel = m_pPlayer->GetMaxLevel();
				int nLevelStatus = 0;
				BYTE bExpFlag = m_pPlayer->GetExpFlag();

				// 레벨 70 초과 시 8포인트를 준다.
				if( LEVEL_70 < dwMaxLevel )
				{
					nLevelStatus = ( (LEVEL_70 - 1) * 6 ) + ( (dwMaxLevel - LEVEL_70) * 8 );
				}
				else
				{
					nLevelStatus = (dwMaxLevel - 1) * 6;
				}
				// 레벨 구간화의 스텟포인트도 준다.
				for( int i = 0; i < 4; i++ )
				{
					if( CheckBit(bExpFlag, i) )
						++nLevelStatus;
				}
				m_pPlayer->SetPlayerLevelUpPoint(nLevelStatus);
				CharacterUpdateResetStatusPoint(m_pPlayer->GetID(), nLevelStatus);
			}
			else
			{
				UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, pItemInfo->SellPrice, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
				AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
			}
		}
	}
	// 부적
	else if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM || pItemInfo->ItemKind == eSHOP_ITEM_PREMIUM ||
		pItemInfo->ItemKind == eSHOP_ITEM_SUNDRIES )
	{
		// 사용제한레벨 추가
		if( pItemInfo->ItemKind == eSHOP_ITEM_CHARM && pItemInfo->EquipKind )
		{
			if( m_pPlayer->GetMaxLevel() < pItemInfo->EquipKind )
				return eItemUseErr_Err;
		}

		// 체인지아이템
		if( pItemInfo->ItemKind == eSHOP_ITEM_SUNDRIES && pItemInfo->SimMek )
		{
			int err = CHANGEITEMMGR->UseChangeItemFromShopItem( m_pPlayer, pItemInfo->SimMek );
			if(  err == 2 )
			{
				//공간이 부족하다는 메시지를 날린다.
				MSG_WORD msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_CHANGEITEM_NACK;
				msg.wData = NOT_SPACE;
				m_pPlayer->SendMsg(&msg, sizeof(msg));				
				return eItemUseErr_Err;
			}
			else if( err != EI_TRUE )
				return eItemUseErr_Err;
		}

		if( pItemInfo->ItemType == 10 )
			if(EI_TRUE != ITEMMGR->DiscardItem( m_pPlayer, UseBaseInfo.ShopItemPos, UseBaseInfo.ShopItemIdx, 1 ))
			{
				return eItemUseErr_Err;
			}

			//
			MSG_DWORD2	msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_EFFECT;
			msg.dwData1 = m_pPlayer->GetID();

			if( pItemInfo->ItemIdx == eSundries_Boom_Butterfly )
			{
				msg.dwData2 = eEffect_ShopItem_Boom1;
				PACKEDDATA_OBJ->QuickSend( m_pPlayer, &msg, sizeof(msg) );	
			}
			else if( pItemInfo->ItemIdx == eSundries_Boom_Star )
			{
				msg.dwData2 = eEffect_ShopItem_Boom2;
				PACKEDDATA_OBJ->QuickSend( m_pPlayer, &msg, sizeof(msg) );	
			}
			//

			CalcShopItemOption( AddShopItem.ShopItem.ItemBase.wIconIdx, TRUE );
			m_pPlayer->CalcState();

			if( pItemInfo->SellPrice && pItemInfo->ItemType==10 )
			{
				UsingShopItemUpdateToDB( 0, m_pPlayer->GetID(), AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, pItemInfo->SellPrice, AddShopItem.ShopItem.BeginTime.value, AddShopItem.ShopItem.Remaintime );
				AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			
			}
	}
	else if( pItemInfo->ItemKind == eSHOP_ITEM_MAKEUP || pItemInfo->ItemKind == eSHOP_ITEM_DECORATION )
	{
		CalcShopItemOption( AddShopItem.ShopItem.ItemBase.wIconIdx, TRUE );
		AddUsingShopItem(&AddShopItem, AddShopItem.ShopItem.ItemBase.wIconIdx);			

		// 다른사람들에게 아이템 착용 정보를 보낸다.
		MSG_DWORD2 msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_AVATAR_PUTON;
		msg.dwData1 = m_pPlayer->GetID();
		msg.dwData2 = pItemInfo->ItemIdx;

		PACKEDDATA_OBJ->QuickSendExceptObjectSelf(m_pPlayer, &msg, sizeof(msg));
		goto LogNotUse;
	}
	//SW051129 Pet
	else if( pItemInfo->ItemKind == eSHOP_ITEM_PET )
	{
		if(pItemInfo->LimitLevel > m_pPlayer->GetMaxLevel())
		{
			return eItemUseErr_Err;
		}
		m_pPlayer->GetPetManager()->SummonPet(pItemBase->dwDBIdx);
		//m_pPlayer->GetPetManager()->SummonPet(PET_ID_START);
		//해당 유저의 펫 매니져에서 해당 팻을 찾아 현재팻으로 설정하고
		//해당 팻의 정보를 그리드에 보낸다.
		goto LogNotUse;
	}
	//SW060406 펫 장착
	else if( pItemInfo->ItemKind == eSHOP_ITEM_PET_EQUIP )
	{
		//펫 소환중이 아니면
		if(!m_pPlayer->GetPetManager()->GetCurSummonPet())
		{
			return eItemUseErr_Err;
		}

		m_pPlayer->GetPetManager()->SetPetEquipOption(pItemInfo->ItemIdx, TRUE);
	}

	AddDupParam( pItemInfo );

	//
	LogItemMoney( m_pPlayer->GetID(), m_pPlayer->GetObjectName(), 0, "",
		eLog_ShopItemUse, m_pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
		AddShopItem.ShopItem.ItemBase.wIconIdx, AddShopItem.ShopItem.ItemBase.dwDBIdx, AddShopItem.ShopItem.ItemBase.Position, 0, AddShopItem.ShopItem.ItemBase.Durability, m_pPlayer->GetPlayerExpPoint());

LogNotUse:

	//
	memcpy(pShopItem, &AddShopItem.ShopItem, sizeof(SHOPITEMBASE));

	return eItemUseSuccess;
}

BOOL CShopItemManager::UsedShopItem( ITEMBASE* pItemBase, DWORD Param, stTIME BeginTime, DWORD RemainTime )
{
	if( !pItemBase )
		return FALSE;

	if( m_UsingItemTable.GetData(pItemBase->wIconIdx) )
		return FALSE;

	SHOPITEMWITHTIME ShopItem;
	ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(pItemBase->wIconIdx);
	if( !pItemInfo )
	{
		if( pItemBase->wIconIdx == eIncantation_SkPointRedist )
		{
			SHOPITEMOPTION* pOption = m_pPlayer->GetShopItemStats();
			pOption->SkillPoint = Param;
			pOption->UseSkillPoint = RemainTime;
		}
		else if( pItemBase->wIconIdx == eIncantation_StatePoint )
		{
			SHOPITEMOPTION* pOption = m_pPlayer->GetShopItemStats();
			pOption->StatePoint = (WORD)Param;
			pOption->UseStatePoint = (WORD)RemainTime;
		}
		else
			return FALSE;
	}
	else if( pItemBase->wIconIdx == eIncantation_StatePoint_30 )
	{
		SHOPITEMOPTION* pOption = m_pPlayer->GetShopItemStats();
		pOption->StatePoint = (WORD)RemainTime;
		pOption->UseStatePoint = 30 - (WORD)RemainTime;
		pItemBase->wIconIdx = eIncantation_StatePoint;

		// ItemIdx, Param, RemainTime을 업데이트 시켜야 한다.
		ShopItemAllUseInfoUpdateToDB( pItemBase->dwDBIdx, m_pPlayer->GetID(), eIncantation_StatePoint, pOption->StatePoint, pOption->UseStatePoint );
	}

	ShopItem.ShopItem.ItemBase = *pItemBase;
	ShopItem.ShopItem.Param = Param;
	ShopItem.ShopItem.BeginTime = BeginTime;
	ShopItem.ShopItem.Remaintime = RemainTime;
	ShopItem.LastCheckTime = gCurTime;
//	int rt = 0;

	if( pItemInfo && pItemInfo->SellPrice == eShopItemUseParam_Realtime )
	{
		// 종료시간을 체크한다.
		stTIME ctime, rtime;
		ctime.value = GetCurTime();
		rtime.SetTime( ShopItem.ShopItem.Remaintime );
		if( ctime > rtime )
		{
			if( pItemInfo->ItemType == 11 )
			{				
/*				rt = ITEMMGR->DiscardItem( m_pPlayer, ShopItem.ShopItem.ItemBase.Position, ShopItem.ShopItem.ItemBase.wIconIdx, 1 );
				if( EI_TRUE != rt )
				{
					char temp[64] = {0, };
					sprintf( temp, "ShoItem - CheckEndTime : %d", rt );
					ASSERTMSG(0,temp);
//					ASSERTMSG(0, "ShoItem - CheckEndTime : %d", rt );
					return FALSE;
				}
				*/
				if(EI_TRUE != ITEMMGR->DiscardItem( m_pPlayer, ShopItem.ShopItem.ItemBase.Position, ShopItem.ShopItem.ItemBase.wIconIdx, 1 ))
				{
//					ASSERTMSG(0, "ShoItem - CheckEndTime");
					return FALSE;
				}
				if( pItemInfo->ItemKind == eSHOP_ITEM_EQUIP )
				{
					// 아이템 없앤다.
				}

			}

			ShopItemDeleteToDB( m_pPlayer->GetID(), ShopItem.ShopItem.ItemBase.dwDBIdx );

			LogItemMoney( m_pPlayer->GetID(), m_pPlayer->GetObjectName(), 0, "",
				eLog_ShopItemUseEnd, m_pPlayer->GetMoney(eItemTable_Inventory), ShopItem.ShopItem.BeginTime.value, ShopItem.ShopItem.Remaintime,
				ShopItem.ShopItem.ItemBase.wIconIdx, ShopItem.ShopItem.ItemBase.dwDBIdx, ShopItem.ShopItem.ItemBase.Position, 0, ShopItem.ShopItem.ItemBase.Durability, m_pPlayer->GetPlayerExpPoint());

			return FALSE;
		}
	}


	AddUsingShopItem(&ShopItem, ShopItem.ShopItem.ItemBase.wIconIdx);

#ifdef _JAPAN_LOCAL_
	if( pItemBase->wIconIdx == eIncantation_MugongExtend ||
		pItemBase->wIconIdx == eIncantation_PyogukExtend ||
		pItemBase->wIconIdx == eIncantation_InvenExtend ||
		pItemBase->wIconIdx == eIncantation_CharacterSlot )
		return	TRUE;
#elif defined _HK_LOCAL_
	if( pItemBase->wIconIdx == eIncantation_MugongExtend || pItemBase->wIconIdx == eIncantation_MugongExtend2 ||
		pItemBase->wIconIdx == eIncantation_PyogukExtend || pItemBase->wIconIdx == eIncantation_PyogukExtend2 ||
		pItemBase->wIconIdx == eIncantation_InvenExtend || pItemBase->wIconIdx == eIncantation_InvenExtend2 ||
		pItemBase->wIconIdx == eIncantation_CharacterSlot || pItemBase->wIconIdx == eIncantation_CharacterSlot2 )
		return	TRUE;
#elif defined _TL_LOCAL_
	if( pItemBase->wIconIdx == eIncantation_MugongExtend ||
		pItemBase->wIconIdx == eIncantation_PyogukExtend ||
		pItemBase->wIconIdx == eIncantation_InvenExtend ||
		pItemBase->wIconIdx == eIncantation_CharacterSlot )
		return	TRUE;
#endif // #ifdef _JAPAN_LOCAL_

	if( pItemInfo && pItemInfo->ItemKind == eSHOP_ITEM_INCANTATION && pItemInfo->CheRyuk )
	{
		CalcShopItemOption(pItemBase->wIconIdx, TRUE, ShopItem.ShopItem.Param);
	}
	else
	{
		// 개인플러스타임이 종료되었으면 적용 안시킨다
		if( pItemInfo && pItemInfo->ItemKind == eSHOP_ITEM_CHARM && pItemInfo->MeleeAttackMin && RemainTime==0 )
			return TRUE;
		else
			CalcShopItemOption(pItemBase->wIconIdx, TRUE, ShopItem.ShopItem.Remaintime);
	}

	// 중복체크 파라미터 설정
	AddDupParam( pItemInfo );

	return TRUE;
}
