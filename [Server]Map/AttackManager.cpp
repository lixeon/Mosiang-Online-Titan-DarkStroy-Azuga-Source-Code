// AttackManager.cpp: implementation of the CAttackManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AttackManager.h"
#include "Network.h"
#include "PackedData.h"
#include "ServerSystem.h"
#include "CharMove.h"
#include "RegenNPC.h"
#include "UserTable.h"
#include "ObjectStateManager.h"

#include "PartyManager.h"
#include "Party.h"
#include "SiegeWarMgr.h"
#include "Player.h"
#include "PetManager.h"
#include "GuildManager.h"

#include "..\[CC]Header\CommonCalcFunc.h"

#include "..\[CC]Header\CommonCalcFunc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define SHIELD_COMBO_DAMAGE			0.5f
#define SHIELD_OUT_MUGONG_DAMAGE	0.7f
#define SHIELD_IN_MUGONG_DAMAGE		0.7f

CAttackManager::CAttackManager()
{
	m_nDamageRate = 100.0f;	// magi82 cheat damage 처음에는 100%의 데미지로 적용(치트키를 쓰면 변경됨)
}

CAttackManager::~CAttackManager()
{

}

void CAttackManager::sendDieMsg(CObject * pAttacker,CObject* pTarget)
{
	MSG_DWORD2 m2c;
	m2c.Category = MP_USERCONN;

	if(pTarget->GetObjectKind() & eObjectKind_Monster)
		m2c.Protocol = MP_USERCONN_MONSTER_DIE;
	else if(pTarget->GetObjectKind() == eObjectKind_Player)
		m2c.Protocol = MP_USERCONN_CHARACTER_DIE;

	m2c.dwObjectID = pAttacker->GetID();
	m2c.dwData1 = pAttacker->GetID();
	m2c.dwData2 = pTarget->GetID();

	PACKEDDATA_OBJ->QuickSend(pTarget,&m2c,sizeof(m2c));
}


DWORD CAttackManager::GetComboPhyDamage(CObject* pAttacker,CObject* pTargetObject,float PhyAttackRate,float fCriticalRate,
										RESULTINFO* pDamageInfo,DWORD AmplifiedPower,float fDecreaseDamageRate )
{
	pDamageInfo->bCritical = m_ATTACKCALC.getCritical(pAttacker,pTargetObject,fCriticalRate);
	double attackPhyDamage = m_ATTACKCALC.getPhysicalAttackPower(pAttacker,PhyAttackRate,pDamageInfo->bCritical);

	// RaMa - 크리티컬일때 스턴확률 추가해야함
	//#ifndef _JAPAN_LOCAL_	//다른곳(getPlayerPhysicalAttackPower) 막았다.
	if(pDamageInfo->bCritical)
	{
		attackPhyDamage *= 1.5f;
		/*
		// 스턴
		if( pAttacker->GetObjectKind() == eObjectKind_Player )
		if((rand()%100) < ((CPlayer*)pAttacker)->GetShopItemStats()->StunByCri)
		{
		// RaMa - 04.11.18
		// 아이템몰에관한 세부계획서는 다음주중( ~11.27)에 받기로 함.
		// pTargetObject->StartSpecialState( eSpecialState_Stun, 10, 0, 0, 0, FindEffectNum("monster_stun_s.beff"), 0 );
		}
		}*/
		//SW060906 신규펫
		if( (pAttacker->GetObjectKind() & eObjectKind_Monster) && pTargetObject->GetObjectKind() == eObjectKind_Player )
		{
			((CPlayer*)pTargetObject)->GetPetManager()->GetPetBuffResultRt(ePB_ReduceCriticalDmg, &attackPhyDamage);
		}

		// magi82 - UniqueItem(070627)
		if( pAttacker->GetObjectKind() == eObjectKind_Player )
		{
			attackPhyDamage += (double)(((CPlayer*)pAttacker)->GetUniqueItemStats()->nCriDamage);
			if(attackPhyDamage < 0.f)
				attackPhyDamage = 1.f;
		}
	}
	//#endif

	attackPhyDamage += AmplifiedPower;

	//041213 KES
	attackPhyDamage *= fDecreaseDamageRate;
	//

	//PvP 일때는 공격력의 50%만
	if( pAttacker->GetObjectKind() == eObjectKind_Player &&
		pTargetObject->GetObjectKind() == eObjectKind_Player )
#ifdef _JAPAN_LOCAL_
		attackPhyDamage *= 0.25f;
#else
		attackPhyDamage *= 0.5f;
#endif

#ifdef _JAPAN_LOCAL_
	DWORD ShieldDamage = pTargetObject->CalcShieldDamage((DWORD)(attackPhyDamage * SHIELD_COMBO_DAMAGE));
	attackPhyDamage -= ShieldDamage;
#else
	// 06. 03 국내무쌍 - 이영준
	DWORD ShieldDamage = 0;
	DWORD ReduceDamage = pTargetObject->CalcShieldDamage((DWORD)(attackPhyDamage * SHIELD_COMBO_DAMAGE), ShieldDamage);
	attackPhyDamage -= ReduceDamage;
#endif

	pDamageInfo->ShieldDamage += ShieldDamage;

	double defencePhyLevel = m_ATTACKCALC.getPhyDefenceLevel(pTargetObject,pAttacker);
	attackPhyDamage *= (1-defencePhyLevel);

	// RaMa - 04.11.24   ->데미지비율
	if(pAttacker->GetObjectKind() == eObjectKind_Player && pTargetObject->GetObjectKind() & eObjectKind_Monster)	
	{
		attackPhyDamage *= gEventRate[eEvent_DamageRate];
	}
	if(pAttacker->GetObjectKind() & eObjectKind_Monster && pTargetObject->GetObjectKind() == eObjectKind_Player)
	{
		attackPhyDamage *= gEventRate[eEvent_DamageReciveRate];
	}

	if(attackPhyDamage < 1)
		attackPhyDamage = 1;

	float fdam = 0.0f;
	// RaMa - 04.11.10    -> ShopItemOption추가
	if(pAttacker->GetObjectKind() == eObjectKind_Player)
	{
		attackPhyDamage *= (((CPlayer*)pAttacker)->GetShopItemStats()->ComboDamage*0.01f+1.0f);

		//SW060719 문파포인트
		if( ((CPlayer*)pAttacker)->GetGuildIdx() )
			GUILDMGR->GetGuildPlustimeRt( ((CPlayer*)pAttacker)->GetGuildIdx(), eGPT_DamageUp, &attackPhyDamage );

		//PET %공격력
		((CPlayer*)pAttacker)->GetPetManager()->GetPetBuffResultRt(ePB_Demage_Percent, &attackPhyDamage);

		// RaMa - 05.10.10 -> 해골가면 옵션추가
		if( pTargetObject->GetObjectKind() & eObjectKind_Monster )
		{
			//if( g_pServerSystem->GetMapNum() == BOSSMONSTER_MAP ||
			//	g_pServerSystem->GetMapNum() == BOSSMONSTER_2NDMAP )	goto CalcEnd;
			if( g_pServerSystem->GetMap()->IsMapKind(eBossMap) )
				goto CalcEnd;

			fdam = ((CPlayer*)pAttacker)->GetAvatarOption()->TargetPhyDefDown*0.01f;
			attackPhyDamage *= (fdam+1.0f);
		}
		// RaMa - 06.11.13 -> 무쌍모드일때 공격력증가 아바타
		if( ((CPlayer*)pAttacker)->IsMussangMode() && ((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage )
			attackPhyDamage *= (((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage*0.01f+1.0f);

		// magi82 - UniqueItem(070627)
		attackPhyDamage += (double)(((CPlayer*)pAttacker)->GetUniqueItemStats()->nPhyDamage);
		if(attackPhyDamage < 0.f)
			attackPhyDamage = 1.f;
	}	
	else if( pAttacker->GetObjectKind() & eObjectKind_Monster )
	{
		// RaMa - 05.10.10 -> 해골가면 옵션추가
		if( pTargetObject->GetObjectKind() == eObjectKind_Player )
		{
			//if( g_pServerSystem->GetMapNum() == BOSSMONSTER_MAP ||
			//	g_pServerSystem->GetMapNum() == BOSSMONSTER_2NDMAP )	goto CalcEnd;
			if( g_pServerSystem->GetMap()->IsMapKind(eBossMap) )
				goto CalcEnd;

			fdam = ((CPlayer*)pTargetObject)->GetAvatarOption()->TargetAtkDown*0.01f;
			attackPhyDamage *= (1.0f-fdam);
		}
	}

CalcEnd:
	return (DWORD)attackPhyDamage;
}



DWORD CAttackManager::GetMugongPhyDamage(CObject* pAttacker,CObject* pTargetObject,float PhyAttackRate,float fCriticalRate,
										 RESULTINFO* pDamageInfo,DWORD AmplifiedPower,float fDecreaseDamageRate )
{
	pDamageInfo->bCritical = m_ATTACKCALC.getCritical(pAttacker,pTargetObject,fCriticalRate);
	double attackPhyDamage = m_ATTACKCALC.getPhysicalAttackPower(pAttacker,PhyAttackRate,pDamageInfo->bCritical );

	// RaMa - 크리티컬일때 스턴확률 추가해야함
	//#ifndef _JAPAN_LOCAL_ //다른곳(getPlayerPhysicalAttackPower) 막았다.
	if(pDamageInfo->bCritical)
	{
		attackPhyDamage *= 1.5f;
		/*
		// 스턴
		if( pAttacker->GetObjectKind() == eObjectKind_Player )
		if((rand()%100) < ((CPlayer*)pAttacker)->GetShopItemStats()->StunByCri)
		{
		// RaMa - 04.11.18
		// 아이템몰에관한 세부계획서는 다음주중( ~11.27)에 받기로 함.
		// pTargetObject->StartSpecialState( eSpecialState_Stun, 10, 0, 0, 0, FindEffectNum("monster_stun_s.beff"), 0 );
		}
		*/
		//SW060906 신규펫
		if( (pAttacker->GetObjectKind() & eObjectKind_Monster) && pTargetObject->GetObjectKind() == eObjectKind_Player )
		{
			((CPlayer*)pTargetObject)->GetPetManager()->GetPetBuffResultRt(ePB_ReduceCriticalDmg, &attackPhyDamage);
		}

		// magi82 - UniqueItem(070627)
		if( pAttacker->GetObjectKind() == eObjectKind_Player )
		{
			attackPhyDamage += (double)(((CPlayer*)pAttacker)->GetUniqueItemStats()->nCriDamage);
			if(attackPhyDamage < 0.f)
				attackPhyDamage = 1.f;
		}
	}
	//#endif

	attackPhyDamage += AmplifiedPower;

	//041213 KES
	attackPhyDamage *= fDecreaseDamageRate;
	//

	//PvP 일때는 공격력의 50%만
	if( pAttacker->GetObjectKind() == eObjectKind_Player &&
		pTargetObject->GetObjectKind() == eObjectKind_Player )
#ifdef _JAPAN_LOCAL_
		attackPhyDamage *= 0.25f;
#else
		attackPhyDamage *= 0.5f;
#endif

#ifdef _JAPAN_LOCAL_
	DWORD ShieldDamage = pTargetObject->CalcShieldDamage((DWORD)(attackPhyDamage * SHIELD_OUT_MUGONG_DAMAGE));
	attackPhyDamage -= ShieldDamage;
#else
	// 06. 03 국내무쌍 - 이영준
	DWORD ShieldDamage = 0;
	DWORD ReduceDamage = pTargetObject->CalcShieldDamage((DWORD)(attackPhyDamage * SHIELD_OUT_MUGONG_DAMAGE), ShieldDamage);
	attackPhyDamage -= ReduceDamage;
#endif

	pDamageInfo->ShieldDamage += ShieldDamage;

	double defencePhyLevel = m_ATTACKCALC.getPhyDefenceLevel(pTargetObject,pAttacker);
	attackPhyDamage *= (1-defencePhyLevel);

	// RaMa - 04.11.24   ->데미지비율
	if(pAttacker->GetObjectKind() == eObjectKind_Player && pTargetObject->GetObjectKind() & eObjectKind_Monster)
	{
		attackPhyDamage *= gEventRate[eEvent_DamageRate];
	}
	if(pAttacker->GetObjectKind() & eObjectKind_Monster && pTargetObject->GetObjectKind() == eObjectKind_Player)
	{
		attackPhyDamage *= gEventRate[eEvent_DamageReciveRate];
	}

	if(attackPhyDamage < 1)
		attackPhyDamage = 1;


	float fdam = 0.0f;
	// RaMa - 04.11.10    -> ShopItemOption추가, AvatarItemOption추가(05.08.16)
	if(pAttacker->GetObjectKind() == eObjectKind_Player)
	{
		fdam = (((CPlayer*)pAttacker)->GetShopItemStats()->WoigongDamage*0.01f) + 
			(((CPlayer*)pAttacker)->GetAvatarOption()->WoigongDamage*0.01f);
		attackPhyDamage *= (fdam+1.0f);

		//SW060719 문파포인트
		if( ((CPlayer*)pAttacker)->GetGuildIdx() )
			GUILDMGR->GetGuildPlustimeRt( ((CPlayer*)pAttacker)->GetGuildIdx(), eGPT_DamageUp, &attackPhyDamage );

		////PET %공격력========================================================
		((CPlayer*)pAttacker)->GetPetManager()->GetPetBuffResultRt(ePB_Demage_Percent, &attackPhyDamage);

		// RaMa - 05.10.10 -> 해골가면 옵션추가
		if( pTargetObject->GetObjectKind() & eObjectKind_Monster )
		{
			//if( g_pServerSystem->GetMapNum() != BOSSMONSTER_MAP &&
			//	g_pServerSystem->GetMapNum() != BOSSMONSTER_2NDMAP )
			if( FALSE == g_pServerSystem->GetMap()->IsMapKind(eBossMap) )
			{
				fdam = ((CPlayer*)pAttacker)->GetAvatarOption()->TargetPhyDefDown*0.01f;
				attackPhyDamage *= (fdam+1.0f);
			}
		}
		// RaMa - 06.11.13 -> 무쌍모드일때 공격력증가 아바타
		if( ((CPlayer*)pAttacker)->IsMussangMode() && ((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage )
			attackPhyDamage *= (((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage*0.01f+1.0f);

		// magi82 - UniqueItem(070627)
		attackPhyDamage += (double)(((CPlayer*)pAttacker)->GetUniqueItemStats()->nPhyDamage);
		if(attackPhyDamage < 0.f)
			attackPhyDamage = 1.f;
	}	
	else if( pAttacker->GetObjectKind() & eObjectKind_Monster )
	{
		// RaMa - 05.10.10 -> 해골가면 옵션추가
		if( pTargetObject->GetObjectKind() == eObjectKind_Player )
		{
			//if( g_pServerSystem->GetMapNum() != BOSSMONSTER_MAP &&
			//	g_pServerSystem->GetMapNum() != BOSSMONSTER_2NDMAP )
			if( FALSE == g_pServerSystem->GetMap()->IsMapKind(eBossMap) )
			{
				fdam = ((CPlayer*)pTargetObject)->GetAvatarOption()->TargetAtkDown*0.01f;
				attackPhyDamage *= (1.0f-fdam);
			}
		}
	}

	//CalcEnd:

	return (DWORD)attackPhyDamage;
}
DWORD CAttackManager::GetMugongAttrDamage(CObject* pAttacker,CObject* pTargetObject,
										  WORD Attrib,DWORD AttAttackMin,DWORD AttAttackMax,float AttAttackRate,
										  float fCriticalRate,RESULTINFO* pDamageInfo,float fDecreaseDamageRate )
{
#ifdef _HK_LOCAL_	//hk block
	pDamageInfo->bDecisive = FALSE;
#else
	pDamageInfo->bDecisive = m_ATTACKCALC.getDecisive(pAttacker,pTargetObject,fCriticalRate);
#endif
	double attackAttrDamage = m_ATTACKCALC.getAttributeAttackPower(pAttacker,Attrib,AttAttackMin,AttAttackMax,AttAttackRate);
	//041213 KES
	attackAttrDamage *= fDecreaseDamageRate;

	if(pDamageInfo->bDecisive)
	{
		attackAttrDamage *= 2.25f;

		//SW060906 신규펫
		if( (pAttacker->GetObjectKind() & eObjectKind_Monster) && pTargetObject->GetObjectKind() == eObjectKind_Player )
		{
			((CPlayer*)pTargetObject)->GetPetManager()->GetPetBuffResultRt(ePB_ReduceCriticalDmg, &attackAttrDamage);
		}

		// magi82 - UniqueItem(070627)
		if( pAttacker->GetObjectKind() == eObjectKind_Player )
		{
			attackAttrDamage += (double)(((CPlayer*)pAttacker)->GetUniqueItemStats()->nCriDamage);
			if(attackAttrDamage < 0.f)
				attackAttrDamage = 1.f;
		}
	}
	//
	//PvP 일때는 공격력의 50%만
#ifdef _JAPAN_LOCAL_	
	//	int nRel = eCAR_None;
	if( pAttacker->GetObjectKind() == eObjectKind_Player &&
		pTargetObject->GetObjectKind() == eObjectKind_Player )
	{
		attackAttrDamage *= 0.25f;

		//		if( Attrib >= 1 && Attrib <= 7 )
		//			nRel = ((CPlayer*)pTargetObject)->WhatIsAttrRelation(Attrib);
	}
#else
	if( pAttacker->GetObjectKind() == eObjectKind_Player &&
		pTargetObject->GetObjectKind() == eObjectKind_Player )
	{
		attackAttrDamage *= 0.5f;
	}
#endif

#ifdef _JAPAN_LOCAL_
	DWORD ShieldDamage = pTargetObject->CalcShieldDamage((DWORD)(attackAttrDamage * SHIELD_IN_MUGONG_DAMAGE));
	attackAttrDamage -= ShieldDamage;
#else
	// 06. 03 국내무쌍 - 이영준
	DWORD ShieldDamage = 0;
	DWORD ReduceDamage = pTargetObject->CalcShieldDamage((DWORD)(attackAttrDamage * SHIELD_IN_MUGONG_DAMAGE), ShieldDamage);
	attackAttrDamage -= ReduceDamage;
#endif

	pDamageInfo->ShieldDamage += ShieldDamage;	

#ifdef _JAPAN_LOCAL_
	float RegVal = ( pTargetObject->GetAttDefense(Attrib) * 3.f ) / ( attackAttrDamage * 2.f );	

	//	if( nRel == eCAR_Weak_Opposite )
	//		RegVal *= 0.5f;

	if( RegVal > 0.99f )		RegVal = 0.99f;
	else if( RegVal < 0.f )		RegVal = 0.f;

#else
	float RegVal = pTargetObject->GetAttDefense(Attrib);

	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	// 속성방어력
	if(pTargetObject->GetObjectKind() == eObjectKind_Player)
	{
		float val = 1 + ((CPlayer*)pTargetObject)->GetSkillStatsOption()->AttDef;

		if( val < 0 )
			val = 0.0f;

		RegVal = RegVal * val;
	}
	//////////////////////////////////////////////////////////////////////////

	if( RegVal > 1 )		RegVal = 1;
	if( RegVal < 0 )		RegVal = 0;

	RegVal *= 0.7f;
#endif
	// RaMa - 05.02.04  -> 심령의 부적
	//	if( pTargetObject->GetObjectKind() == eObjectKind_Player )
	//		RegVal += (((CPlayer*)pTargetObject)->GetShopItemStats()->RegistAttr*0.01f*0.7f);

	double resAttrDamage = (attackAttrDamage * (1 - RegVal) );

	// RaMa - 04.11.24   ->데미지비율
	if(pAttacker->GetObjectKind() == eObjectKind_Player && pTargetObject->GetObjectKind() & eObjectKind_Monster)
	{
		resAttrDamage = (resAttrDamage*gEventRate[eEvent_DamageRate]);
	}
	if(pAttacker->GetObjectKind() & eObjectKind_Monster && pTargetObject->GetObjectKind() == eObjectKind_Player)
	{
		resAttrDamage = (resAttrDamage*gEventRate[eEvent_DamageReciveRate]);
	}

	float fdam = 0.0f;
	// RaMa - 04.11.10    -> ShopItemOption추가, AvatarItemOption추가(05.08.16)
	if(pAttacker->GetObjectKind() == eObjectKind_Player)
	{
		fdam = (((CPlayer*)pAttacker)->GetShopItemStats()->NeagongDamage*0.01f)+
			(((CPlayer*)pAttacker)->GetAvatarOption()->NeagongDamage*0.01f);
		resAttrDamage = (resAttrDamage*(fdam+1.0f));

		//SW060719 문파포인트
		if( ((CPlayer*)pAttacker)->GetGuildIdx() )
			GUILDMGR->GetGuildPlustimeRt( ((CPlayer*)pAttacker)->GetGuildIdx(), eGPT_DamageUp, &resAttrDamage );

		//PET %공격력
		((CPlayer*)pAttacker)->GetPetManager()->GetPetBuffResultRt(ePB_Demage_Percent, &resAttrDamage);

		// RaMa - 05.10.10 -> 해골가면 옵션추가
		if( pTargetObject->GetObjectKind() & eObjectKind_Monster )
		{
			//if( g_pServerSystem->GetMapNum() == BOSSMONSTER_MAP ||
			//	g_pServerSystem->GetMapNum() == BOSSMONSTER_2NDMAP )	goto CalcEnd;
			if( g_pServerSystem->GetMap()->IsMapKind(eBossMap) )
				goto CalcEnd;

			fdam = ((CPlayer*)pAttacker)->GetAvatarOption()->TargetAttrDefDown*0.01f;
			resAttrDamage = (resAttrDamage*(fdam+1.0f));
		}
		// RaMa - 06.11.13 -> 무쌍모드일때 공격력증가 아바타
		if( ((CPlayer*)pAttacker)->IsMussangMode() && ((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage )
			resAttrDamage *= (((CPlayer*)pAttacker)->GetAvatarOption()->MussangDamage*0.01f+1.0f);

		// magi82 - UniqueItem(070627)
		resAttrDamage *= (((CPlayer*)pAttacker)->GetUniqueItemStats()->nAttR * 0.01f + 1.0f);
	}	
	else if( pAttacker->GetObjectKind() & eObjectKind_Monster )
	{
		// RaMa - 05.10.10 -> 해골가면 옵션추가
		if( pTargetObject->GetObjectKind() == eObjectKind_Player )
		{
			//if( g_pServerSystem->GetMapNum() == BOSSMONSTER_MAP ||
			//	g_pServerSystem->GetMapNum() == BOSSMONSTER_2NDMAP )	goto CalcEnd;
			if( g_pServerSystem->GetMap()->IsMapKind(eBossMap) )
				goto CalcEnd;

			fdam = ((CPlayer*)pTargetObject)->GetAvatarOption()->TargetAtkDown*0.01f;
			resAttrDamage = (resAttrDamage*(1.0f-fdam));
		}
	}

CalcEnd:	

	return (DWORD)resAttrDamage;
}

void CAttackManager::Attack(BOOL bMugong, CObject* pAttacker,CObject* pTarget,DWORD AmplifiedPower,
							float PhyAttackRate,
							WORD Attrib,DWORD AttAttackMin,DWORD AttAttackMax,float AttAttackRate,
							float fCriticalRate,
							RESULTINFO* pDamageInfo,BOOL bCounter, float fDecreaseDamageRate,
							WORD AmplifiedPowerAttrib, BOOL bContinueAttack )
{	
	pDamageInfo->Clear();

	//회피	//SW060330 이동 Object->CalcRealDamage(... 에서..
	float fDodgeRate = pTarget->GetDodgeRate();

	if(fDodgeRate != 0)
	{
		if(CheckRandom(fDodgeRate,pTarget->GetLevel(),pAttacker->GetLevel()) == TRUE)
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;
			pDamageInfo->ShieldDamage = 0;
			return;
		}
	}

	if(pTarget->GetObjectKind() == eObjectKind_Player)
	{
		//펫 버프 회피
		BOOL rtDodge = FALSE;
		((CPlayer*)pTarget)->GetPetManager()->GetPetBuffResultRt(ePB_Dodge, &rtDodge);
		if( TRUE == rtDodge )
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;
			pDamageInfo->ShieldDamage = 0;
			return;
		}

		//SW060911 홍콩요청 수정.
		if( ((CPlayer*)pTarget)->GetUserLevel() <= eUSERLEVEL_GM && ((CPlayer*)pTarget)->IsVisible() == FALSE )
		{
			return;
		}
	}

	// 2005 크리스마스 이벤트
	if(( pAttacker->GetObjectKind() == eObjectKind_Player ) && ( pTarget->GetObjectKind() & eObjectKind_Monster ))
	{	// 플레이어가 몬스터 공격시
		CPlayer* pPlayer = (CPlayer*)pAttacker;
		CMonster* pMonster = (CMonster*)pTarget;

		// 만약 이벤트 무기 장착중이라면
		if( pPlayer->GetWeaponEquipType() == WP_EVENT || pPlayer->GetWeaponEquipType() == WP_EVENT_HAMMER )
		{
			// 무공을 사용한것이면 리턴한다.
			if(bMugong)
				return;

			// 지속 공격도 리턴
			if(bContinueAttack)
				return;
		}
		else if( pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA1 || pMonster->GetMonsterKind() == EVENT_MONSTER_SANTA2
			|| pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_SM || pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_SM 
			|| pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_MD || pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_MD 
			|| pMonster->GetMonsterKind() == EVENT_FIELDBOSS_SNOWMAN_LG || pMonster->GetMonsterKind() == EVENT_FIELDSUB_SNOWMAN_LG 
			/*|| pMonster->GetMonsterKind() == EVENT_SNOWMAN_SM
			|| pMonster->GetMonsterKind() == EVENT_SNOWMAN_MD
			|| pMonster->GetMonsterKind() == EVENT_SNOWMAN_LG*/
			)
			return;
	}

	if(pTarget->GetInited() == FALSE)
		return;

	DWORD AttrDamage =0,PhyDamage =0;

	if(pTarget->GetState() == eObjectState_Die || pTarget->GetState() == eObjectState_Immortal)
	{
		return;
	}

	if( pAttacker->GetGridID() != pTarget->GetGridID() )
	{
		//		ASSERT(0);
		return;
	}
	/*
	#ifdef _JAPAN_LOCAL_

	if( pAttacker->GetObjectKind() == eObjectKind_Player )
	{
	CPlayer* pAttackPlayer = (CPlayer*)pAttacker;

	if( pAttackPlayer->GetPartyIdx() )
	{
	CParty* pParty = PARTYMGR->GetParty( pAttackPlayer->GetPartyIdx() );

	if( pParty )
	if( pParty->IsHelpPartyMember( pAttackPlayer->GetID() ) )
	{
	pAttackPlayer->SetPartyHelp( TRUE );	//파티 상생 효과 
	}
	}
	}
	#endif
	*/
	if(PhyAttackRate > 0.000001f)
	{
		if(bMugong)
		{
			PhyDamage = GetMugongPhyDamage(pAttacker,pTarget,PhyAttackRate,fCriticalRate,pDamageInfo,AmplifiedPower,fDecreaseDamageRate );
		}
		else
		{
			PhyDamage = GetComboPhyDamage(pAttacker,pTarget,PhyAttackRate,fCriticalRate,pDamageInfo,AmplifiedPower,fDecreaseDamageRate );
		}
	}

	if(AttAttackMax != 0)
	{
		AttrDamage = GetMugongAttrDamage(pAttacker,pTarget,Attrib,AttAttackMin,AttAttackMax,AttAttackRate,fCriticalRate,pDamageInfo,fDecreaseDamageRate );
	}

	// 임시 테스트
	//	PhyDamage = 10000000;
	//	AttrDamage = 10000000;

	//상극에 대한 1.5배.. 여기가 최적의 위치다.
#ifdef _JAPAN_LOCAL_
	//Attrib 외공에대한 속성
	//AmplifiedPowerAttrib 내공에대한 속성
	if( pAttacker->GetObjectKind() == eObjectKind_Player && pTarget->GetObjectKind() == eObjectKind_Player )
	{
		if( ((CPlayer*)pTarget)->WhatIsAttrRelation(Attrib) == eCAR_Weak_Opposite )
		{
			PhyDamage = PhyDamage + PhyDamage / 2;
		}
		if( ((CPlayer*)pTarget)->WhatIsAttrRelation(AmplifiedPowerAttrib) == eCAR_Weak_Opposite )
		{
			AttrDamage = AttrDamage + AttrDamage /2;
		}
	}		

#endif

	pAttacker->CalcRealAttack(pTarget, PhyDamage, AttrDamage, pDamageInfo, bContinueAttack );

	//== 이상하게 되었따 T_T 일단 임시로 그냥 갑시다!
	DWORD UpAttackDamage = 0;
	if( PhyDamage + AttrDamage < pDamageInfo->RealDamage )
		UpAttackDamage = ( pDamageInfo->RealDamage - ( PhyDamage + AttrDamage ) ) / 2;
	//===

	//SW070811 기획변경 pvp 일 때 10% 데미지.	//attackPhyDamage *= 0.1f;	//attackAttrDamage *= 0.1f;
	// 공성전시에는 데미지 적용
	/*float fsiegedamage = 0.2f;
	#ifdef _HK_LOCAL_
	fsiegedamage = 0.5f;
	#endif*/
	float fsiegedamage = 1.f;
//#ifdef _HK_LOCAL_
#ifdef _KOR_LOCAL_
	fsiegedamage = 0.5f;
#endif
	if( g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetSiegeMapNum() )
	{
		if( pAttacker->GetObjectKind() == eObjectKind_Player && pTarget->GetObjectKind() == eObjectKind_Player )
		{
			// 계산해서 0이 나올때를 위해서 +1
			if( PhyDamage )
				PhyDamage = (DWORD)(PhyDamage*fsiegedamage + 1);
			if( AttrDamage )
				AttrDamage = (DWORD)(AttrDamage*fsiegedamage + 1);
			if( UpAttackDamage )
				UpAttackDamage = (DWORD)(UpAttackDamage*fsiegedamage + 1);
			if( pDamageInfo->RealDamage )
				pDamageInfo->RealDamage = (DWORD)(pDamageInfo->RealDamage*fsiegedamage + 1);
			if( pDamageInfo->ShieldDamage )
				pDamageInfo->ShieldDamage = (DWORD)(pDamageInfo->ShieldDamage*fsiegedamage + 1);
		}
	}

	pTarget->CalcRealDamage(pAttacker,PhyDamage+UpAttackDamage,AttrDamage+UpAttackDamage,pDamageInfo);

	// RaMa - 데미지 이상수치 확인용 코드
	if( (pAttacker->GetObjectKind() & eObjectKind_Monster) && pDamageInfo->RealDamage > 1000000 )
	{
		char buf[256] = { 0, };
		sprintf( buf, "[DAMAGE] Name: %s, bCritical: %d, Damage: %d, ShieldDamage : %d, PhyDamage: %d, AttrDamage : %d, UpAttackDamage : %d ", 
			pAttacker->GetObjectName(), pDamageInfo->bCritical, pDamageInfo->RealDamage, pDamageInfo->ShieldDamage, 
			PhyDamage, AttrDamage, UpAttackDamage );
		ASSERTMSG( 0, buf );
		return;
	}

	if( pAttacker->GetObjectKind() == eObjectKind_Player )
	{
		CPlayer* pAttackPlayer = (CPlayer*)pAttacker;
		/*
		#ifdef _JAPAN_LOCAL_
		pAttackPlayer->SetPartyHelp( FALSE );	//계산 끝났다. 초기화하자.
		#endif
		*/
#ifdef _JAPAN_LOCAL_

		if( pAttackPlayer->GetPartyIdx() )
		{
			CParty* pParty = PARTYMGR->GetParty( pAttackPlayer->GetPartyIdx() );

			if( pParty )
				if( pParty->IsHelpPartyMember( pAttackPlayer->GetID() ) )
				{
					pDamageInfo->RealDamage = pDamageInfo->RealDamage + pDamageInfo->RealDamage / 2;
				}
		}

#endif

		if( pTarget->GetObjectKind() == eObjectKind_Player )
		{
			CPlayer* pTargetPlayer = (CPlayer*)pTarget;

			//PK모드시에 PK시간 갱신
			if( pAttackPlayer->IsPKMode() /*&& pAttackPlayer->IsVimuing() == FALSE*/ ) //비무일리가 없다.
			{
				if( AttrDamage || PhyDamage )
					if( pTargetPlayer->IsPKMode() == FALSE )
						pAttackPlayer->SetPKStartTimeReset();
			}

			// 06.09.25 RaMa 추석이벤트
			if( pAttackPlayer->GetWeaponEquipType() == WP_EVENT_HAMMER )
				pTargetPlayer->IncreaseEventHammerCount();
			//#ifndef _HK_LOCAL_	//hk block christmas
			if( pAttackPlayer->GetWeaponEquipType() == WP_EVENT )
			{
				pTargetPlayer->SummonEventPetRndm();
			}
			//#endif
		}
	}

	if(bCounter)
		pTarget->CalcCounterDamage(Attrib,AttrDamage,PhyDamage,pDamageInfo);
	else
		pDamageInfo->CounterDamage = 0;

	//-----받는 데미지에 따른 체력 내력 흡수

	pTarget->CalcReverseVampiric(pDamageInfo);	//이펙트 처리는 안한다 --;
	//-----


	//	BOOL bVampiric = TRUE;
	//	if(bVampiric)
	//	{
	pAttacker->CalcVampiric(pDamageInfo);
	//	}

	// 06. 03 국내무쌍 - 이영준
#ifndef _JAPAN_LOCAL_
	if( pAttacker->GetObjectKind() == eObjectKind_Player )
	{
		if(((CPlayer*)pAttacker)->IsMussangMode())
			pDamageInfo->RealDamage = (DWORD)(pDamageInfo->RealDamage * 1.05);
	}
#endif

	// magi82 cheat damage //////////////////////////////////////////////////////////////////////////
	float fRate = m_nDamageRate * 0.01f;

	if(pDamageInfo->RealDamage > 0 && ((pDamageInfo->RealDamage * fRate)*10) <= 10)
		pDamageInfo->RealDamage = 1;
	else
		pDamageInfo->RealDamage *= (DWORD)fRate;
	//////////////////////////////////////////////////////////////////////////

	//2007. 10. 30. CBH - 패널티 데미지 처리 추가
	DWORD dwRealDemage = pDamageInfo->RealDamage;

	pDamageInfo->RealDamage = GetPenaltyDemege(pAttacker, pTarget, dwRealDemage);	

	//SW070127 타이탄
	if( pAttacker->GetObjectKind() == eObjectKind_Player )
	{
		CPlayer* pPlayer = (CPlayer*)pAttacker;
		if( pPlayer->InTitan() )
		{
			pPlayer->GetTitanManager()->DoRandomDecrease(AtAtk);
		}
	}

	DWORD newLife = pTarget->Damage(pAttacker,pDamageInfo);

	if(pDamageInfo->CounterDamage != 0)
	{
		// 반격은 무속성 데미지이다. (cf: 무속성은 반격당하지 않으므로 반격의 반격은 있을수 없다.)
		RESULTINFO counterdmginfo;
		PhyDamage = 0;
		AttrDamage = pDamageInfo->CounterDamage;
		counterdmginfo.CounterDamage = 0;
		pAttacker->CalcRealDamage(pAttacker,PhyDamage,AttrDamage,&counterdmginfo);
		DWORD attackerlife = pAttacker->Damage(pTarget,&counterdmginfo);
		if(attackerlife == 0)
		{
			ATTACKMGR->sendDieMsg(pTarget,pAttacker);
			pAttacker->Die(pTarget);
		}
	}

	if(newLife == 0)
	{
		ATTACKMGR->sendDieMsg(pAttacker,pTarget);
		pTarget->Die(pAttacker);
	}
}

void CAttackManager::RecoverLife(CObject* pOperator,CObject* pObject,WORD m_Attrib,DWORD RecoverLifeVal,RESULTINFO* pResultInfo)
{
	DWORD realAddVal;
#ifdef _JAPAN_LOCAL_
	pObject->AddLife(RecoverLifeVal,&realAddVal,FALSE);
#else
	float fPlus = pOperator->GetAttribPlusPercent(m_Attrib);
	DWORD val = (DWORD)(RecoverLifeVal * (1 + fPlus));
	pObject->AddLife(val,&realAddVal,FALSE);
#endif
	pResultInfo->HealLife = (WORD)realAddVal;
}
void CAttackManager::RecoverNaeRyuk(CObject* pOperator,CObject* pObject,WORD m_Attrib,DWORD RecoverNaeRyukVal,RESULTINFO* pResultInfo)
{
	DWORD realAddVal;
#ifdef _JAPAN_LOCAL_
	pObject->AddNaeRyuk(RecoverNaeRyukVal,&realAddVal);
#else
	float fPlus = pOperator->GetAttribPlusPercent(m_Attrib);
	DWORD val = (DWORD)(RecoverNaeRyukVal * (1 + fPlus));
	pObject->AddNaeRyuk(val,&realAddVal);
#endif
	pResultInfo->RechargeNaeryuk = (WORD)realAddVal;
}


void CAttackManager::AttackAbs( CObject* pAttacker, CObject* pTarget, int nAbsKind, float AttackRate, RESULTINFO* pDamageInfo )
{

	if( nAbsKind == 0 ) return;

	pDamageInfo->Clear();

	//회피	//SW060330 이동 Object->CalcRealDamage(... 에서..
	float fDodgeRate = pTarget->GetDodgeRate();

	if(fDodgeRate != 0)
	{
		if(CheckRandom(fDodgeRate,pTarget->GetLevel(),pAttacker->GetLevel()) == TRUE)
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;
			pDamageInfo->ShieldDamage = 0;
			return;
		}
	}

	if(pTarget->GetObjectKind() == eObjectKind_Player)
	{
		//펫 버프 회피
		BOOL rtDodge = FALSE;
		((CPlayer*)pTarget)->GetPetManager()->GetPetBuffResultRt(ePB_Dodge, &rtDodge);
		if( TRUE == rtDodge )
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;
			pDamageInfo->ShieldDamage = 0;
			return;
		}
	}
	if(pTarget->GetInited() == FALSE)
		return;

	if(pTarget->GetState() == eObjectState_Die || pTarget->GetState() == eObjectState_Immortal)
	{
		return;
	}

	if( pAttacker->GetGridID() != pTarget->GetGridID() )
	{
		return;
	}


	//여기가 맞나? KES confirm	//비무는 어떻게 하지?
	if( pAttacker->GetObjectKind() == eObjectKind_Player )
		if( ((CPlayer*)pAttacker)->IsPKMode() )
		{
			if( pTarget->GetObjectKind() == eObjectKind_Player )
				if( AttackRate > 0.0f  )
					if( ((CPlayer*)pTarget)->IsPKMode() == FALSE )
						((CPlayer*)pAttacker)->SetPKStartTimeReset();
		}

		DWORD TargetLife = pTarget->GetLife();
		DWORD MinusLife = 0;
		DWORD TargetShield = pTarget->GetShield();
		DWORD MinusShield = 0;

		if( nAbsKind & eAAK_LIFE )
		{
			MinusLife = (DWORD)(TargetLife * AttackRate);
		}
		else if( nAbsKind & eAAK_SHIELD )	//일단 동시에 체력/호신 을 사용할 수 없다.
		{
			MinusShield = (DWORD)(TargetShield * AttackRate);
		}

		pDamageInfo->RealDamage = MinusLife;
		pDamageInfo->ShieldDamage = MinusShield;

		/*
		BOOL bVampiric = TRUE;
		if(bVampiric)
		{
		pAttacker->CalcVampiric(pDamageInfo);
		}
		*/	
		DWORD newLife = pTarget->Damage(pAttacker,pDamageInfo);
		/*	
		if(pDamageInfo->CounterDamage != 0)
		{
		// 반격은 무속성 데미지이다. (cf: 무속성은 반격당하지 않으므로 반격의 반격은 있을수 없다.)
		RESULTINFO counterdmginfo;
		PhyDamage = 0;
		AttrDamage = pDamageInfo->CounterDamage;
		counterdmginfo.CounterDamage = 0;
		pAttacker->CalcRealDamage(pAttacker,PhyDamage,AttrDamage,&counterdmginfo);
		DWORD attackerlife = pAttacker->Damage(pTarget,&counterdmginfo);
		if(attackerlife == 0)
		{
		ATTACKMGR->sendDieMsg(pTarget,pAttacker);
		pAttacker->Die(pTarget);
		}
		}
		*/
		if(newLife == 0)
		{
			ATTACKMGR->sendDieMsg(pAttacker,pTarget);
			pTarget->Die(pAttacker);
		}
}



void CAttackManager::AttackJinbub( CObject* pAttacker, CObject* pTarget, DWORD AttackPower, 
								  DWORD AttackMin,DWORD AttackMax, RESULTINFO* pDamageInfo,
								  float fDecreaseDamageRate )
{
	pDamageInfo->Clear();

	//회피	//SW060330 이동 Object->CalcRealDamage(... 에서..
	float fDodgeRate = pTarget->GetDodgeRate();

	if(fDodgeRate != 0)
	{
		if(CheckRandom(fDodgeRate,pTarget->GetLevel(),pAttacker->GetLevel()) == TRUE)
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;
			pDamageInfo->ShieldDamage = 0;
			return;
		}
	}

	if(pTarget->GetObjectKind() == eObjectKind_Player)
	{
		//펫 버프 회피
		BOOL rtDodge = FALSE;
		((CPlayer*)pTarget)->GetPetManager()->GetPetBuffResultRt(ePB_Dodge, &rtDodge);
		if( TRUE == rtDodge )
		{
			pDamageInfo->bDodge = TRUE;
			pDamageInfo->RealDamage = 0;
			pDamageInfo->ShieldDamage = 0;
			return;
		}
	}


	if(pTarget->GetInited() == FALSE)
		return;

	if(pTarget->GetState() == eObjectState_Die || pTarget->GetState() == eObjectState_Immortal)
	{
		return;
	}

	if( pAttacker->GetGridID() != pTarget->GetGridID() )
	{
		return;
	}

	if( AttackPower == 0 )
		return;

	//여기가 맞나? KES confirm	//비무는 어떻게 하지?
	if( pAttacker->GetObjectKind() == eObjectKind_Player )
		if( ((CPlayer*)pAttacker)->IsPKMode() )
		{
			if( pTarget )
				if( pTarget->GetObjectKind() == eObjectKind_Player )
					if( ((CPlayer*)pTarget)->IsPKMode() == FALSE )
						((CPlayer*)pAttacker)->SetPKStartTimeReset();
		}

		//==================
		if( AttackMax < AttackMin )
			AttackMin = AttackMax;

		DWORD PlusAttack = random( AttackMin, AttackMax );

		DWORD RealAttack = GetJinbubDamage( pAttacker, pTarget, AttackPower+PlusAttack,
			pDamageInfo, fDecreaseDamageRate );


		// 공성전시에는 데미지 적용
		float fsiegedamage = 0.2f;
//#ifdef _HK_LOCAL_
#ifdef _KOR_LOCAL_
		fsiegedamage = 0.5f;
#endif
		if( g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetSiegeMapNum() )
		{
			if( pAttacker->GetObjectKind() == eObjectKind_Player && pTarget->GetObjectKind() == eObjectKind_Player )
			{
				// 계산해서 0이 나올때를 위해서 +1
				if( RealAttack )
					RealAttack = (WORD)(RealAttack*fsiegedamage + 1);
				if( pDamageInfo->RealDamage )
					pDamageInfo->RealDamage = (DWORD)(pDamageInfo->RealDamage*fsiegedamage + 1);
				if( pDamageInfo->ShieldDamage )
					pDamageInfo->ShieldDamage = (DWORD)(pDamageInfo->ShieldDamage*fsiegedamage + 1);
			}
		}

		//1. 공격력 강화 없다.
		//2.회피/운기중데미지/방어버프 계산 (물리공격이다.)
		pTarget->CalcRealDamage( pAttacker, (WORD)RealAttack, 0, pDamageInfo );
		//3.반격은 없다.
		//4.공격받았을시 피 흡수.
		pTarget->CalcReverseVampiric(pDamageInfo);	//이펙트 처리는 안한다 --;
		//5.피빨기 없다.
		DWORD newLife = pTarget->Damage(pAttacker,pDamageInfo);

		//6.반격은 없다.

		//============
		if(newLife == 0)
		{
			ATTACKMGR->sendDieMsg(pAttacker,pTarget);
			pTarget->Die(pAttacker);
		}
}

DWORD CAttackManager::GetJinbubDamage(CObject* pAttacker,CObject* pTargetObject,DWORD AttackPower,
									  RESULTINFO* pDamageInfo,float fDecreaseDamageRate)
{
	//041213 KES
	DWORD attackPhyDamage = (DWORD)(AttackPower * fDecreaseDamageRate);
	//

	//PvP 일때는 공격력의 50%만
	if( pAttacker->GetObjectKind() == eObjectKind_Player &&
		pTargetObject->GetObjectKind() == eObjectKind_Player )
#ifdef _JAPAN_LOCAL_
		attackPhyDamage = (DWORD)(attackPhyDamage*0.25f);
#else
		attackPhyDamage = (DWORD)(attackPhyDamage*0.5f);
#endif

#ifdef _JAPAN_LOCAL_
	DWORD ShieldDamage = pTargetObject->CalcShieldDamage((DWORD)(attackPhyDamage * SHIELD_OUT_MUGONG_DAMAGE));
	attackPhyDamage -= ShieldDamage;
#else
	// 06. 03 국내무쌍 - 이영준
	DWORD ShieldDamage = 0;
	DWORD ReduceDamage = pTargetObject->CalcShieldDamage((DWORD)(attackPhyDamage * SHIELD_OUT_MUGONG_DAMAGE), ShieldDamage);
	attackPhyDamage -= ReduceDamage;
#endif

	pDamageInfo->ShieldDamage += (DWORD)ShieldDamage;

	double defencePhyLevel = m_ATTACKCALC.getPhyDefenceLevel(pTargetObject,pAttacker);
	attackPhyDamage = (DWORD)(attackPhyDamage*(1-defencePhyLevel));

	// RaMa - 04.11.24   ->데미지비율
	if(pAttacker->GetObjectKind() == eObjectKind_Player && pTargetObject->GetObjectKind() & eObjectKind_Monster)
	{
		// attackPhyDamage *= gDamageRate;
		attackPhyDamage = (DWORD)( attackPhyDamage*gEventRate[eEvent_DamageRate]);
	}
	if(pAttacker->GetObjectKind() & eObjectKind_Monster && pTargetObject->GetObjectKind() == eObjectKind_Player)
	{
		// attackPhyDamage *= gDamageReciveRate;
		attackPhyDamage = (DWORD)(attackPhyDamage*gEventRate[eEvent_DamageReciveRate]);
	}

	if(attackPhyDamage < 1)
		attackPhyDamage = 1;

	return attackPhyDamage;
}

//2007. 10. 30. CBH - 데미지 패널티 계산 함수
DWORD CAttackManager::GetPenaltyDemege(CObject* pAttacker,CObject* pTargetObject, DWORD dwDemage)
{
	DWORD dwResultDemege = dwDemage;

	BYTE attackerKind = pAttacker->GetObjectKind();
	BYTE targetKind = pTargetObject->GetObjectKind();

	if(targetKind & eObjectKind_Monster)
	{
		if(attackerKind == eObjectKind_Player)
		{
			CPlayer* pPlayer = (CPlayer*)pAttacker;
			//타이탄 탑승했을때와 미탑승 했을때 몬스터 데미지 패널티
			if(targetKind == eObjectKind_TitanMonster)
			{
				//타겟이 타이탄 몬스터이고 플레이어가 타이탄 미 탑승시 패널티 데미지 (원래 데미지의 5%만 들어감)
				if(pPlayer->InTitan() == FALSE)
				{
					dwResultDemege = (DWORD)( (float)dwDemage * 0.05f );
				}				
			}
			else
			{
				//타겟이 일반 몬스터이고 플레이어가 타이탄 탑승시 패널티 데미지 (원래 데미지의 50%만 들어감)
				if(pPlayer->InTitan() == TRUE)
				{
					dwResultDemege = (DWORD)( (float)dwDemage * 0.5f );
				}				
			}
		}
	}	

	return dwResultDemege;
}
