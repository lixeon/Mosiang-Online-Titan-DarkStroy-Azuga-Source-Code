// Distribute_Damage.cpp: implementation of the CDistribute_Damage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Distribute_Damage.h"
#include "ItemDrop.h"
#include "ItemManager.h"
#include "Player.h"
#include "Monster.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "MapItemDrop.h"	// 맵 아이템 드랍 추가 by Stiner(2008/05/30)-MapItemDrop
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDistribute_Damage::CDistribute_Damage()
{

}

CDistribute_Damage::~CDistribute_Damage()
{

}
/*
void CDistribute_Damage::SendAbil(LEVELTYPE MonsterLevel, PARTY_RECEIVE_MEMBER* pRealMember)
{
	if(pRealMember->count == 0)
		return;

	//적합한 파티 멤버 아이디(로그인해 있고 몬스터 죽은 자리 근처에 있는), 몬스터 레벨, 각자가 준 데미지
	ASSERT(pRealMember->count <= MAX_PARTY_LISTNUM);
	DWORD BigDamage;
	BigDamage = 0;
	int num=0;
	int rate=0;
	for(int i=0; i<pRealMember->count; ++i)
	{
		if(BigDamage < pRealMember->Damage[i])
		{
			num = i;
			BigDamage = pRealMember->Damage[i];
		}
		else if(BigDamage == pRealMember->Damage[i])
		{
			rate = rand() % 2;
			if(rate == 1)
				num = i;
		}
	}
	SendToPersonalAbil((CPlayer*)pRealMember->pPlayer[num],MonsterLevel);
}
*/
void CDistribute_Damage::SendItem( PARTY_RECEIVE_MEMBER* pRealMember, WORD DropItemId, DWORD DropItemRatio, MONSTEREX_LIST * pMonInfo, WORD MonsterKind, LEVELTYPE MaxLevel )
{
	if(pRealMember->count == 0)	return;
	if(ChkGetItemLevel(MaxLevel, pMonInfo->Level) == FALSE)	return;

	if(DropItemId == 0)
	{
		MONEYTYPE money;
		MONEYTYPE partymoney;
		CPlayer* pReceivePlayer = (CPlayer*)pRealMember->pPlayer[0];
		if( !pReceivePlayer )	return;

		/*if( g_pServerSystem->GetMapNum() > TITANMAP_START_INDEX )
		{
			MONSTERDROP_TIMAP* pMonsterDropInfo = GAMERESRCMNGR->GetTitanMapMonsterDropInfo( pMonInfo->Level );
			if( pMonsterDropInfo )
				money = random( pMonsterDropInfo->stDropInfo[pRealMember->count].wMoneyMin, pMonsterDropInfo->stDropInfo[pRealMember->count].wMoneyMax );
		}
		else*/
			money = ITEMDROP_OBJ->MoneyItemNoItemPercentCalculator(pMonInfo, pReceivePlayer, MonsterKind);
			// 맵 아이템 드랍 계산 by Stiner(2008/05/30)-MapItemDrop
			MAPITEMDROP_OBJ->CalculateDropRate( pReceivePlayer, MonsterKind );

		if(money)
		{
			partymoney = money/pRealMember->count;
			for(int i=0; i< pRealMember->count; ++i)
			{
				SendToPersonalMoney((CPlayer*)pRealMember->pPlayer[i], partymoney, MonsterKind);
			}
			return;
		}
	}
	else
	{
		DWORD BigDamage = 0;
		int num=0;
		int rate=0;
		for(int i=0; i<pRealMember->count; ++i)
		{
			if(BigDamage < pRealMember->Damage[i])
			{
				num = i;
			}
			else if(BigDamage == pRealMember->Damage[i])
			{
				rate = rand() % 2;
				if(rate == 1)
					num = i;
			}
		}
		CPlayer* pReceivePlayer = (CPlayer*)pRealMember->pPlayer[num];
		if( !pReceivePlayer )	return;

		//랜덤함수 --; 어떤거 써야하지
		if( DropItemRatio )
		if( ( rand()%100 ) % ( 100 / DropItemRatio ) == 0 )
			ITEMMGR->MonsterObtainItem(pReceivePlayer,DropItemId,MonsterKind);
	}
}

/*
void CDistribute_Damage::SendItem( PARTY_RECEIVE_MEMBER* pRealMember, WORD DropItemId, DWORD DropItemRatio, MONSTEREX_LIST * pMonInfo, WORD MonsterKind, LEVELTYPE MaxLevel )
{
	if(pRealMember->count == 0)
		return;

//	ASSERT(MonsterKind & eObjectKind_Monster );
	MONEYTYPE money;
	MONEYTYPE partymoney;
	DWORD BigDamage;
	BigDamage = 0;
	int num=0;
	int rate=0;
	for(int i=0; i<pRealMember->count; ++i)
	{
		if(BigDamage < pRealMember->Damage[i])
		{
			num = i;
		}
		else if(BigDamage == pRealMember->Damage[i])
		{
			rate = rand() % 2;
			if(rate == 1)
				num = i;
		}
	}

	CPlayer* pReceivePlayer = (CPlayer*)pRealMember->pPlayer[num];
	if(ChkGetItemLevel(pReceivePlayer->GetLevel(), pMonInfo->Level) == FALSE)
		return;
	if(DropItemId == 0)
	{
		money = ITEMDROP_OBJ->MoneyItemNoItemPercentCalculator(pMonInfo, pReceivePlayer, MonsterKind);
		if(money)
		{
			partymoney = money/pRealMember->count;
			for(int i=0; i< pRealMember->count; ++i)
			{
				SendToPersonalMoney((CPlayer*)pRealMember->pPlayer[i], partymoney, MonsterKind);
			}
			return;
		}
	}
	else
	{
		//랜덤함수 --; 어떤거 써야하지
		if( DropItemRatio )
		if( ( rand()%100 ) % ( 100 / DropItemRatio ) == 0 )
			ITEMMGR->MonsterObtainItem(pReceivePlayer,DropItemId,MonsterKind);
	}
}
*/
