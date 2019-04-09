// CharacterManager.cpp: implementation of the CCharacterManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CharacterManager.h"
#include "Player.h"
#include "mapdbmsgparser.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CCharacterManager);
CCharacterManager::CCharacterManager()
{

}

CCharacterManager::~CCharacterManager()
{

}

BOOL CCharacterManager::ArrangeCharLevelPoint(CPlayer * pPlayer, WORD type)
{
	if( pPlayer->GetShopItemStats()->UseStatePoint <= 0 && pPlayer->GetPlayerLevelUpPoint() <= 0)
	{
		pPlayer->SetPlayerLevelUpPoint(0);
		return FALSE;
	}

	if( pPlayer->GetShopItemStats()->UseStatePoint > 0 )
	{
		--pPlayer->GetShopItemStats()->UseStatePoint;
		ShopItemUpdatetimeToDB( pPlayer->GetID(), eIncantation_StatePoint, pPlayer->GetShopItemStats()->UseStatePoint );

		LogItemMoney( pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
			eLog_ShopItemStatsChange, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
			0, 0, pPlayer->GetShopItemStats()->StatePoint, pPlayer->GetShopItemStats()->UseStatePoint, 0, pPlayer->GetPlayerExpPoint());
	}
	else
		pPlayer->SetPlayerLevelUpPoint(pPlayer->GetPlayerLevelUpPoint()-1);

	

	switch(type)
	{
	case GENGOL_POINT:
		{
			pPlayer->AddGenGol(1);
		}
		break;
	case SIMMEK_POINT:
		{
			pPlayer->AddSimMek(1);
		}
		break;
	case MINCHUB_POINT:
		{
			pPlayer->AddMinChub(1);
		}
		break;
	case CHERYUK_POINT:
		{
			pPlayer->AddCheRyuk(1);
		}
		break;
	}

	//SW060906 신규펫 스탯 반올림
	player_calc_stats * char_stats = pPlayer->GetCharStats();
	pPlayer->GetPetManager()->GetPetBuffResultRt(ePB_MasterAllStatRound, char_stats);
	// 

	InsertLogCharacter( pPlayer->GetID(), pPlayer->GetLevel(),
						pPlayer->GetHeroTotalInfo() );

	return TRUE;
}



BOOL CCharacterManager::ArrangeCharMinusPoint(CPlayer * pPlayer, WORD type)
{
	if(pPlayer->GetShopItemStats()->StatePoint <= 0)
	{
		pPlayer->GetShopItemStats()->StatePoint = 0;
		return FALSE;
	}

	int point = 0;
	switch(type)
	{
	case GENGOL_POINT:
		{
			point = pPlayer->GetGenGol() - pPlayer->GetAbilityStats()->StatGen - pPlayer->GetItemStats()->GenGol - 
				pPlayer->GetShopItemStats()->Gengol - pPlayer->GetAvatarOption()->Gengol - pPlayer->GetSetItemStats()->wGenGol - 
				pPlayer->GetUniqueItemStats()->nGengol;

			if( point <= 12 )
				return FALSE;

			pPlayer->AddGenGol(-1);
		}
		break;
	case SIMMEK_POINT:
		{
			point = pPlayer->GetSimMek() - pPlayer->GetAbilityStats()->StatSim - pPlayer->GetItemStats()->SimMaek - 
				pPlayer->GetShopItemStats()->Simmek - pPlayer->GetAvatarOption()->Simmek - pPlayer->GetSetItemStats()->wSimMek - 
				pPlayer->GetUniqueItemStats()->nSimMek;

			if( point <= 12 )
				return FALSE;

			pPlayer->AddSimMek(-1);
		}
		break;
	case MINCHUB_POINT:
		{
			point = pPlayer->GetMinChub() - pPlayer->GetAbilityStats()->StatMin - pPlayer->GetItemStats()->MinChub - 
				pPlayer->GetShopItemStats()->Minchub - pPlayer->GetAvatarOption()->Minchub - pPlayer->GetSetItemStats()->wMinChub - 
				pPlayer->GetUniqueItemStats()->nMinChub;

			if( point <= 12 )
				return FALSE;

			pPlayer->AddMinChub(-1);
		}
		break;
	case CHERYUK_POINT:
		{
			point = pPlayer->GetCheRyuk() - pPlayer->GetAbilityStats()->StatChe - pPlayer->GetItemStats()->CheRyuk - 
				pPlayer->GetShopItemStats()->Cheryuk - pPlayer->GetAvatarOption()->Cheryuk - pPlayer->GetSetItemStats()->wCheRyuk - 
				pPlayer->GetUniqueItemStats()->nCheRyuk;

			if( point <= 12 )
				return FALSE;

			pPlayer->AddCheRyuk(-1);
		}
		break;
	}


	--pPlayer->GetShopItemStats()->StatePoint;
	++pPlayer->GetShopItemStats()->UseStatePoint;

	// magi82 - SHOPITEM(071004) 스텟 재분배 주문서 갱신할때 UserIdx 는 0으로 줘야한다.(처음 갱신할때도 0을 줬으므로..)
	ShopItemUseParamUpdateToDB( 0, pPlayer->GetID(), eIncantation_StatePoint, pPlayer->GetShopItemStats()->StatePoint );	
	ShopItemUpdatetimeToDB( pPlayer->GetID(), eIncantation_StatePoint, pPlayer->GetShopItemStats()->UseStatePoint );


	// 
	InsertLogCharacter( pPlayer->GetID(), pPlayer->GetLevel(),
						pPlayer->GetHeroTotalInfo() );
	LogItemMoney( pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
		eLog_ShopItemStatsChange, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
		0, 0, pPlayer->GetShopItemStats()->StatePoint, pPlayer->GetShopItemStats()->UseStatePoint, 0, pPlayer->GetPlayerExpPoint());

	return TRUE;
}
