// StatsCalcManager.cpp: implementation of the CStatsCalcManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StatsCalcManager.h"
#include "..\[CC]Header\GameResourceManager.h"

#ifdef _MHCLIENT_
#include "Hero.h"
#include "GameIn.h"
#endif

#ifdef _MAPSERVER_
#include "Player.h"
#include "TitanManager.h"
#endif

#include "CharacterCalcManager.h"
#include "ItemManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStatsCalcManager::CStatsCalcManager()
{

}

CStatsCalcManager::~CStatsCalcManager()
{

}

void CStatsCalcManager::Clear(player_calc_stats * pStats)
{
	memset(pStats, 0, sizeof(player_calc_stats));
}

#ifdef _JAPAN_LOCAL_

float ItemApplyRate[] = 
{
	1,
	0.5f,
	0.4f,
	0.3f,
	0.2f,
	0.1f,
	0.0f,
};

#else

float ItemApplyRate[] = 
{
	1,
	0.6f,
	0.55f,
	0.5f,
	0.45f,
	0.4f,
	0.35f,
	0.30f,
	0.3f,
	0.25f,
	0.2f,
	0.15f,
	0.1f,
	0.05f,
	0.f,
};

#endif

// 장착창에 아이템 장착,해제시
void CStatsCalcManager::CalcItemStats(PLAYERTYPE* pPlayer)
{
	player_calc_stats * item_stats = pPlayer->GetItemStats();
	Clear(item_stats);

//	WORD ItemIdx;
//		for(int part=0;part<eWearedItem_Max;++part)
//		{
//		ItemIdx = pPlayer->GetWearedItemIdx(part);
//		if(ItemIdx == 0)
//			continue;
	
	ASSERT(pPlayer->GetMaxLevel() >= pPlayer->GetLevel());
	
	for(POSTYPE part = TP_WEAR_START ; part < TP_WEAR_END ; ++part )
	{
		const ITEMBASE * pTargetItemBase = ITEMMGR->GetItemInfoAbsIn(pPlayer, part);

		// yh 처음 들어갈때 문제가... -_-a
		if(pTargetItemBase == NULL)
			continue;

		if(pTargetItemBase->dwDBIdx == 0)
			continue;

		ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(pTargetItemBase->wIconIdx);
///		ASSERT(pItemInfo);
		if(pItemInfo == NULL)
			continue;
		
		float ApplyRate = 1;

		int gap = pItemInfo->LimitLevel - pPlayer->GetLevel();
		if(gap > 0)
		{
			if( pPlayer->GetShopItemStats()->EquipLevelFree )
				gap -= pPlayer->GetShopItemStats()->EquipLevelFree;

			if(gap < 0)		gap = 0;
#ifdef _JAPAN_LOCAL_			
			if(gap > 6)		gap = 6;
#else
			if(gap > 13)	gap = 13;
#endif // _JAPAN_LOCAL_
			ApplyRate = ItemApplyRate[gap];
		}

		if(pItemInfo->ItemKind == eEQUIP_ITEM_ARMLET)
		{
			// 호환일경우 최종데미지에 %로 더해줌
			item_stats->MeleeAttackPowerAddRateMin += pItemInfo->MeleeAttackMin*ApplyRate * 0.01f;
			item_stats->MeleeAttackPowerAddRateMax += pItemInfo->MeleeAttackMax*ApplyRate * 0.01f;
			item_stats->RangeAttackPowerAddRateMin += pItemInfo->RangeAttackMin*ApplyRate * 0.01f;
			item_stats->RangeAttackPowerAddRateMax += pItemInfo->RangeAttackMax*ApplyRate * 0.01f;
		}
		else
		{
			// 다른 것들은 그냥 공격계산식에 더해줌
			item_stats->MeleeAttackPowerMin += (WORD)(pItemInfo->MeleeAttackMin*ApplyRate);		
			item_stats->MeleeAttackPowerMax += (WORD)(pItemInfo->MeleeAttackMax*ApplyRate);
			item_stats->RangeAttackPowerMin += (WORD)(pItemInfo->RangeAttackMin*ApplyRate);
			item_stats->RangeAttackPowerMax += (WORD)(pItemInfo->RangeAttackMax*ApplyRate);
		}
		
		item_stats->PhysicalDefense += (WORD)(pItemInfo->PhyDef*ApplyRate);
		item_stats->AttributeAttack.AddATTRIBUTE_VAL(pItemInfo->AttrAttack,ApplyRate);
		item_stats->AttributeResist.AddATTRIBUTE_VAL(pItemInfo->AttrRegist,ApplyRate);
		item_stats->GenGol += (WORD)(pItemInfo->GenGol*ApplyRate);
		item_stats->MinChub += (WORD)(pItemInfo->MinChub*ApplyRate);
		item_stats->CheRyuk += (WORD)(pItemInfo->CheRyuk*ApplyRate);
		item_stats->SimMaek += (WORD)(pItemInfo->SimMek*ApplyRate);
		item_stats->MaxLife += (DWORD)(pItemInfo->Life*ApplyRate);
		item_stats->MaxShield += (DWORD)(pItemInfo->Shield*ApplyRate);
		item_stats->MaxNaeRyuk += (DWORD)(pItemInfo->NaeRyuk*ApplyRate);
		
		if(ITEMMGR->IsOptionItem(pTargetItemBase->wIconIdx, pTargetItemBase->Durability))
		{
			DWORD chrid;
#ifdef _MHCLIENT_
			chrid = HEROID;
			ITEM_OPTION_INFO * pOptionInfo = ITEMMGR->GetItemOption(pTargetItemBase->Durability);
#else
			chrid = pPlayer->GetID();
			ITEM_OPTION_INFO * pOptionInfo = pPlayer->GetItemOption(pTargetItemBase->Durability);
#endif
			if(pOptionInfo)
			{
				float rate = ApplyRate;
				if( pPlayer->GetShopItemStats()->ReinforceAmp )
					rate += (pPlayer->GetShopItemStats()->ReinforceAmp/100.f);

				item_stats->MeleeAttackPowerMin += (WORD)(pOptionInfo->PhyAttack*rate);		
				item_stats->MeleeAttackPowerMax += (WORD)(pOptionInfo->PhyAttack*rate);
				item_stats->RangeAttackPowerMin += (WORD)(pOptionInfo->PhyAttack*rate);
				item_stats->RangeAttackPowerMax += (WORD)(pOptionInfo->PhyAttack*rate);

				item_stats->PhysicalDefense += (WORD)(pOptionInfo->PhyDefense*rate);
				item_stats->AttributeAttack.AddATTRIBUTE_VAL(pOptionInfo->AttrAttack,rate);
				item_stats->AttributeResist.AddATTRIBUTE_VAL(pOptionInfo->AttrRegist,rate);
				item_stats->GenGol += (WORD)(pOptionInfo->GenGol*rate);
				item_stats->MinChub += (WORD)(pOptionInfo->MinChub*rate);
				item_stats->CheRyuk += (WORD)(pOptionInfo->CheRyuk*rate);
				item_stats->SimMaek += (WORD)(pOptionInfo->SimMek*rate);
				item_stats->MaxLife += (DWORD)(pOptionInfo->Life*rate);
				item_stats->MaxShield += (DWORD)(pOptionInfo->Shield*rate);
				item_stats->MaxNaeRyuk += (DWORD)(pOptionInfo->NaeRyuk*rate);
				item_stats->Critical += (DWORD)(pOptionInfo->CriticalPercent*rate);
			}
			else
			{
				ASSERTMSG(pOptionInfo,"GetItemOption() is NULL");
				char temp[128];

				sprintf(temp,"%d  %d  %d",chrid,pTargetItemBase->dwDBIdx,pTargetItemBase->Durability);
			}
		}

		//SW050920 Rare
		if(ITEMMGR->IsRareOptionItem(pTargetItemBase->wIconIdx, pTargetItemBase->RareIdx))
		{
#ifdef _MHCLIENT_
			ITEM_RARE_OPTION_INFO* pRareOptionInfo = ITEMMGR->GetItemRareOption(pTargetItemBase->RareIdx);
#else
			ITEM_RARE_OPTION_INFO* pRareOptionInfo = pPlayer->GetItemRareOption(pTargetItemBase->RareIdx);
#endif
			if(pRareOptionInfo)
			{
				item_stats->MeleeAttackPowerMin += (WORD)(pRareOptionInfo->PhyAttack*ApplyRate);		
				item_stats->MeleeAttackPowerMax += (WORD)(pRareOptionInfo->PhyAttack*ApplyRate);
				item_stats->RangeAttackPowerMin += (WORD)(pRareOptionInfo->PhyAttack*ApplyRate);
				item_stats->RangeAttackPowerMax += (WORD)(pRareOptionInfo->PhyAttack*ApplyRate);

				item_stats->PhysicalDefense += (WORD)(pRareOptionInfo->PhyDefense*ApplyRate);
				item_stats->AttributeAttack.AddATTRIBUTE_VAL(pRareOptionInfo->AttrAttack,ApplyRate);
				item_stats->AttributeResist.AddATTRIBUTE_VAL(pRareOptionInfo->AttrRegist,ApplyRate);
				item_stats->GenGol += (WORD)(pRareOptionInfo->GenGol*ApplyRate);
				item_stats->MinChub += (WORD)(pRareOptionInfo->MinChub*ApplyRate);
				item_stats->CheRyuk += (WORD)(pRareOptionInfo->CheRyuk*ApplyRate);
				item_stats->SimMaek += (WORD)(pRareOptionInfo->SimMek*ApplyRate);
				item_stats->MaxLife += (DWORD)(pRareOptionInfo->Life*ApplyRate);
				item_stats->MaxShield += (DWORD)(pRareOptionInfo->Shield*ApplyRate);
				item_stats->MaxNaeRyuk += (DWORD)(pRareOptionInfo->NaeRyuk*ApplyRate);
//				item_stats->Critical += (DWORD)(pRareOptionInfo->CriticalPercent*ApplyRate);				
			}
			else
			{
				ASSERTMSG(pRareOptionInfo,"GetItemRareOption() is NULL");
			}
		}
	}
/*	
	if(item_stats->MeleeAttackPowerMax == 0)		// 기본 무기 공격력은 5
	{
		item_stats->MeleeAttackPowerMax = 5;
		item_stats->MeleeAttackPowerMin = 5;
	}
	if(item_stats->RangeAttackPowerMax == 0)
	{
		item_stats->RangeAttackPowerMax = 5;
		item_stats->RangeAttackPowerMin = 5;
	}
*/
#ifndef _JAPAN_LOCAL_
	item_stats->AttributeResist.CheckLimit(1);	// 저항력은 최대값이 1

	// 06. 03 국내무쌍 - 이영준
	if(pPlayer->IsMussangMode())
	{
		WORD stat = pPlayer->GetMussangStat();

		item_stats->GenGol += stat;
		item_stats->MinChub += stat;
		item_stats->CheRyuk += stat;
		item_stats->SimMaek += stat;
	}

	//SW060324 펫 버프추가
	pPlayer->GetPetManager()->GetPetBuffResultRt(ePB_MasterAllStatUp, item_stats);

	//SW060906 신규펫 스탯 반올림
	pPlayer->GetPetManager()->GetPetBuffResultRt(ePB_MasterAllStatRound, item_stats);

#endif

	/////////////////// 2007. 6. 11. CBH - 세트아이탬 능력치 추가 ////////////////
	CalcSetItemStats(pPlayer);
	/////////////////////////////////////////////////////////////////////////////

	// magi82 - UniqueItem(070626)
	CalcUniqueItemStats(pPlayer);
	
	// 아이템 수치가 변경되면 캐릭터 수치도 변경
	CalcCharStats(pPlayer);
}

// 캐릭게임In
// 레벨업시 ( 능력치 바뀔때)
// 무공발동시(+능력치) : 구조체를 하나 더 둬야 하나?
// ShopItem사용시의 옵션추가
void CStatsCalcManager::CalcCharStats(PLAYERTYPE* pPlayer)
{
	player_calc_stats * char_stats = pPlayer->GetCharStats();
	Clear(char_stats);

	HERO_TOTALINFO pHeroInfo;
	pPlayer->GetHeroTotalInfo(&pHeroInfo);
	// 아이템 장착한거에 따른 캐릭터 수치 셋팅
	char_stats->MinChub = pHeroInfo.wMinChub;
	char_stats->GenGol = pHeroInfo.wGenGol;

#ifdef _JAPAN_LOCAL_
	char_stats->PhysicalDefense = pPlayer->GetCheRyuk() * 10 / 15;
	char_stats->Critical = ( pPlayer->GetGenGol() / 4 ) + ( pPlayer->GetMinChub() / 6 );
#else
	char_stats->PhysicalDefense = (WORD)(pPlayer->GetCheRyuk()/1.5);
	// 10/27 수치 변경전	char_stats->PhysicalDefense = pHeroInfo.wCheRyuk/3;
	char_stats->Critical = ( pPlayer->GetGenGol()) / 4;
	//////////////////////////////////////////////////////
	// 06. 07 내공 적중(일격) - 이영준
	char_stats->Decisive = ( pPlayer->GetSimMek()) / 4;
	//////////////////////////////////////////////////////
#endif

#ifdef _MAPSERVER_
	CCharacterCalcManager::CalcMaxLife(pPlayer);
	CCharacterCalcManager::CalcMaxShield(pPlayer);
	CCharacterCalcManager::CalcMaxNaeRyuk(pPlayer);
#endif
}


void CStatsCalcManager::CalcCharLife(PLAYERTYPE* pPlayer)
{
#ifdef _MAPSERVER_
	CCharacterCalcManager::CalcMaxLife(pPlayer);
#endif
}
void CStatsCalcManager::CalcCharShield(PLAYERTYPE* pPlayer)
{
#ifdef _MAPSERVER_
	CCharacterCalcManager::CalcMaxShield(pPlayer);
#endif
}
void CStatsCalcManager::CalcCharNaeruyk(PLAYERTYPE* pPlayer)
{
#ifdef _MAPSERVER_
	CCharacterCalcManager::CalcMaxNaeRyuk(pPlayer);
#endif
}

void CStatsCalcManager::CalcTitanItemStats( PLAYERTYPE* pPlayer )
{
	//타이탄 아이템 속성구조체를 가져와
	//장착창 슬롯의 아이템 정보로 갱신한다.

	titan_calc_stats* pItemStat = pPlayer->GetTitanManager()->GetTitanItemStats();

	memset(pItemStat, 0, sizeof(titan_calc_stats));

	for( POSTYPE part = TP_TITANWEAR_START; part < TP_TITANWEAR_END; ++part )
	{
		const ITEMBASE* pTargetItemBase = ITEMMGR->GetItemInfoAbsIn(pPlayer, part);

		if(NULL == pTargetItemBase)
			continue;

		if(0 == pTargetItemBase->dwDBIdx)
			continue;

		// 070621 기획변경
		TITAN_ENDURANCE_ITEMINFO* pInfo = NULL;
		pInfo = (pPlayer->GetTitanManager())->GetTitanItemEnduranceInfo(pTargetItemBase->dwDBIdx);
		if(NULL == pInfo)
			continue;

		if( 0 == pInfo->Endurance )	//내구도 없을시 옵션 비적용.
			continue;

		ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo(pTargetItemBase->wIconIdx);

		if(NULL == pItemInfo)
			continue;

		pItemStat->MaxFuel				+= pItemInfo->Life;
		pItemStat->MaxSpell				+= pItemInfo->NaeRyuk;
		pItemStat->PhysicalDefense		+= pItemInfo->PhyDef;
		pItemStat->Critical				+= pItemInfo->CriticalPercent;
		pItemStat->MeleeAttackPowerMin	+= pItemInfo->MeleeAttackMin;
		pItemStat->MeleeAttackPowerMax	+= pItemInfo->MeleeAttackMax;
		pItemStat->RangeAttackPowerMin	+= pItemInfo->RangeAttackMin;
		pItemStat->RangeAttackPowerMax	+= pItemInfo->RangeAttackMax;
		pItemStat->AttackRange			+= pItemInfo->MugongNum;	//	무공서	연결 무공  번호
		pItemStat->MoveSpeed			+= pItemInfo->AllPlus_Value;//	무공 유형별 무공+ 정도
		pItemStat->AttributeAttack.AddATTRIBUTE_VAL(pItemInfo->AttrAttack,1);
		pItemStat->AttributeResist.AddATTRIBUTE_VAL(pItemInfo->AttrRegist,1);

	}

}

/////////////////// 2007. 6. 11. CBH - 세트아이탬 능력치 추가 ////////////////
void CStatsCalcManager::CalcSetItemStats(CPlayer* pPlayer)
{
	pPlayer->ClearSetitemOption();

	SET_ITEM_INFO setItemInfo[MAX_SETITEM_KIND_NUM];

	memset(setItemInfo, 0, sizeof(SET_ITEM_INFO) * MAX_SETITEM_KIND_NUM);

	//장착된 세트아이탬의 종류와 갯수를 검색한다.
	for(POSTYPE part = TP_WEAR_START ; part < TP_WEAR_END ; ++part )
	{
		const ITEMBASE * pTargetItemBase = ITEMMGR->GetItemInfoAbsIn(pPlayer, part);

		if(pTargetItemBase == NULL)
		{
			continue;
		}

		if(pTargetItemBase->dwDBIdx == 0)
		{
			continue;
		}

		ITEM_INFO * pItemInfo = ITEMMGR->GetItemInfo(pTargetItemBase->wIconIdx);
		
		if(pItemInfo == NULL)
			continue;

		//장착된 세트아이탬의 종류와 갯수정보를 배열에 저장
		if(pItemInfo->wSetItemKind != 0)
		{
			for(unsigned int ui = 0 ; ui < MAX_SETITEM_KIND_NUM ; ui++)
			{
				if(setItemInfo[ui].wKind == pItemInfo->wSetItemKind)
				{
					setItemInfo[ui].wCount++;
					break;
				}
				else
				{
					if(setItemInfo[ui].wKind == 0)
					{
						setItemInfo[ui].wKind = pItemInfo->wSetItemKind;
						setItemInfo[ui].wCount++;
						break;
					}					
				}				
			}
		}		
	}

	//배열에 저장된 세트아이탬 정보를 가지고 스탯 적용
	CYHHashTable<SET_ITEM_OPTION>* setItemOptionList = ITEMMGR->GetSetItemOptionList();

	for(unsigned int ui = 0 ; ui < MAX_SETITEM_KIND_NUM ; ui++)
	{
		if(setItemInfo[ui].wKind != 0)
		{			
			setItemOptionList->SetPositionHead();

			SET_ITEM_OPTION* pSetItemOption = NULL;
			while(pSetItemOption = setItemOptionList->GetData())
			{				
				if(pSetItemOption->wSetItemKind == setItemInfo[ui].wKind)
				{
					WORD wSetValue = pSetItemOption->wSetValue;					

					if(setItemInfo[ui].wCount >= wSetValue)
					{
						pPlayer->AddSetitemOption(pSetItemOption);
					}					
				}				
			}			
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

// magi82 - UniqueItem(070626)
void CStatsCalcManager::CalcUniqueItemStats(PLAYERTYPE* pPlayer)
{
	// 유니크 관련 스텟을 초기화후 다시 세팅
	pPlayer->ClearUniqueItemOption();

	for( POSTYPE pos = TP_WEAR_START; pos < TP_WEAR_END; pos++ )
	{
		const ITEMBASE* pItemBase = ITEMMGR->GetItemInfoAbsIn(pPlayer, pos);
		if( !pItemBase )
		{
			continue;
		}

		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo(pItemBase->wIconIdx);
		if(!pInfo)
		{
			continue;
		}

		// 유니크 아이템이라면..
		if( pInfo->ItemKind == eEQUIP_ITEM_UNIQUE )
		{
			UNIQUE_ITEM_OPTION_INFO* pUniqueItemInfo = GAMERESRCMNGR->GetUniqueItemOptionList(pInfo->ItemIdx);
			if( !pUniqueItemInfo )
			{
				continue;
			}

            pPlayer->AddUniqueItemOption(pUniqueItemInfo);
		}
	}
}
//////////////////////////////////////////////////////////////////////////