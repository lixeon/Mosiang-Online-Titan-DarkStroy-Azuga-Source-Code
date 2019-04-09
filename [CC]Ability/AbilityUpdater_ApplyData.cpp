// AbilityUpdater_ApplyData.cpp: implementation of the CAbilityUpdater_ApplyData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AbilityUpdater_ApplyData.h"
#include "AbilityGroup.h"
#include "AbilityCommonHeader.h"
#include "Player.h"
#include "CharacterCalcManager.h"
#include "AbilityManager.h"

#ifdef _MHCLIENT_
#include "GameIn.h"
#include "CharacterDialog.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAbilityUpdater_ApplyData::CAbilityUpdater_ApplyData()
{

}

CAbilityUpdater_ApplyData::~CAbilityUpdater_ApplyData()
{

}

void CAbilityUpdater_ApplyData::Update(DWORD& UpdateCmd,DWORD Param,CAbilityGroup* pGroup,CYHHashTable<CAbilityInfo>* pInfoTable)
{
	if(UpdateCmd == ABILITYUPDATE_ABILITYQUICK_CHANGED)
		return;

	CPlayer* pObj = pGroup->GetOwenerObject();
	ABILITY_STATS* pStats = pObj->GetAbilityStats();
	CAbilityInfo* pAbilityInfo;
	CAbility* pAbility;
	ABILITY_INFO* pInfo;
	pStats->Clear();
	pGroup->SetPositionHead();
	while(pAbility = pGroup->GetNextAbility(eAGK_Battle))
	{
		pAbilityInfo = pAbility->GetAbilityInfo();
		pInfo = pAbilityInfo->GetInfo();
		BYTE AbilLevel = pAbility->GetLevel();
		AbilLevel += pObj->GetShopItemStats()->AddSung;
		ABILITY_CALC* pCalcInfo = ABILITYMGR->GetAbilityCalcInfo( AbilLevel );
		if( !pCalcInfo )	continue;

		ySWITCH(pInfo->Ability_effect_Kind)
			yCASE(eAUK_PHYATTACK)
				DWORD upVal = pCalcInfo->dwPhyAttack;
				pStats->SetPhyAttackUp(pInfo->Ability_effect_Param1,upVal);
				
			yCASE(eAUK_ATTRIB)
				WORD Attrib = pInfo->Ability_effect_Param1;
				float attackupVal = pCalcInfo->fAttribAttack;
				float registupVal = pCalcInfo->fAttribRegist;
				pStats->AttAttackUp.SetElement_Val(Attrib,attackupVal);
				pStats->AttRegistUp.SetElement_Val(Attrib,registupVal);

			yCASE(eAUK_MAXLIFE)
				pStats->LifeUp = pCalcInfo->dwLife;

			yCASE(eAUK_DEFENCE)
				pStats->DefenceUp = pCalcInfo->dwDeffence;

			yCASE(eAUK_MAXNEARYUK)
				pStats->NaeRyukUp = pCalcInfo->dwNearyuk;

			yCASE(eAUK_MAXSHIELD)
				pStats->ShieldUp = pCalcInfo->dwShield;
				
			yCASE(eAUK_UNGISPEED)
				pStats->UngiUpVal = pCalcInfo->dwUngi;

			yCASE(eAUK_STAT1)	// 근
				pStats->StatGen = pCalcInfo->dwStat;

			yCASE(eAUK_STAT2)	// 민
				pStats->StatMin = pCalcInfo->dwStat;

			yCASE(eAUK_STAT3)	// 체
				pStats->StatChe = pCalcInfo->dwStat;

			yCASE(eAUK_STAT4)	// 심
				pStats->StatSim = pCalcInfo->dwStat;

			yCASE(eAUK_KYUNGGONG_SPEED)	// 경신법
				pStats->Kyunggong = pCalcInfo->fKyunggong;
				pStats->KyunggongLevel = AbilLevel;

			yCASE(eAUK_NOATTRIB)	// 무허공
				pStats->fNoAttrib = pCalcInfo->fNoAttrib;

			yCASE(eAUK_SKILL_DAMAGE)	// 무공데미지
				pStats->SkillDamage = pCalcInfo->dwSkillDamage;

			yCASE(eAUK_CRITICAL_DAMAGE)	// 일격데미지
				pStats->CriticalDamage = pCalcInfo->dwCriticalDamage;
			yCASE(eAUKJOB_TitanExpert)	//2007. 11. 6. CBH - 타이탄 탑승 시간
				pStats->dwTitanRidingPlusTime = pCalcInfo->dwTitanRidingPlusTime;

		yENDSWITCH
	}

#ifdef _MAPSERVER_
	if(UpdateCmd != ABILITYUPDATE_ABILITYINITED)
	{
		CCharacterCalcManager::CalcMaxLife(pObj);
		CCharacterCalcManager::CalcMaxShield(pObj);
		CCharacterCalcManager::CalcMaxNaeRyuk(pObj);
		((CPlayer*)pObj)->SetChangeAbilityKyungGongLevel( pStats->KyunggongLevel );
	}
#else
	GAMEIN->GetCharacterDialog()->UpdateForStageAbility();
	GAMEIN->GetCharacterDialog()->UpdateData();	
#endif
}

/*
void CAbilityUpdater_ApplyData::Update(DWORD& UpdateCmd,DWORD Param,CAbilityGroup* pGroup,CYHHashTable<CAbilityInfo>* pInfoTable)
{
	if(UpdateCmd == ABILITYUPDATE_ABILITYQUICK_CHANGED)
		return;

	CPlayer* pObj = pGroup->GetOwenerObject();
	ABILITY_STATS* pStats = pObj->GetAbilityStats();
	CAbilityInfo* pAbilityInfo;
	CAbility* pAbility;
	ABILITY_INFO* pInfo;
	pStats->Clear();
	pGroup->SetPositionHead();
	while(pAbility = pGroup->GetNextAbility(eAGK_Battle))
	{
		pAbilityInfo = pAbility->GetAbilityInfo();
		pInfo = pAbilityInfo->GetInfo();
		BYTE AbilLevel = pAbility->GetLevel();
		AbilLevel += pObj->GetShopItemStats()->AddSung;

		ySWITCH(pInfo->Ability_effect_Kind)
			yCASE(eAUK_PHYATTACK)
				DWORD upVal = 0;
				if(0 < AbilLevel)	upVal += min(AbilLevel-0,5) * 2;
				if(5 < AbilLevel)	upVal += min(AbilLevel-5,5) * 3;
				if(10 < AbilLevel)	upVal += min(AbilLevel-10,5) * 4;
				if(15 < AbilLevel)	upVal += min(AbilLevel-15,5) * 5;
				if(20 < AbilLevel)	upVal += min(AbilLevel-20,5) * 7;
				if(25 < AbilLevel)	upVal += min(AbilLevel-25,5) * 9;
			
				pStats->SetPhyAttackUp(pInfo->Ability_effect_Param1,upVal);
				
			yCASE(eAUK_ATTRIB)
				WORD Attrib = pInfo->Ability_effect_Param1;
				float attackupVal = 0;
				float registupVal = 0;
				if(0 < AbilLevel)	attackupVal += min(AbilLevel-0,5) * 0.01;
				if(5 < AbilLevel)	attackupVal += min(AbilLevel-5,5) * 0.02;
				if(10 < AbilLevel)	attackupVal += min(AbilLevel-10,5) * 0.04;
				if(15 < AbilLevel)	attackupVal += min(AbilLevel-15,5) * 0.06;
				if(20 < AbilLevel)	attackupVal += min(AbilLevel-20,5) * 0.08;
				if(25 < AbilLevel)	attackupVal += min(AbilLevel-25,5) * 0.09;
				registupVal = AbilLevel * 0.02f;
			
				pStats->AttAttackUp.SetElement_Val(Attrib,attackupVal);
				pStats->AttRegistUp.SetElement_Val(Attrib,registupVal);

			yCASE(eAUK_MAXLIFE)
				DWORD upVal = 0;
				if(0 < AbilLevel)	upVal += min(AbilLevel-0,5) * 10;
				if(5 < AbilLevel)	upVal += min(AbilLevel-5,5) * 14;
				if(10 < AbilLevel)	upVal += min(AbilLevel-10,5) * 18;
				if(15 < AbilLevel)	upVal += min(AbilLevel-15,5) * 22;
				if(20 < AbilLevel)	upVal += min(AbilLevel-20,5) * 26;
				if(25 < AbilLevel)	upVal += min(AbilLevel-25,5) * 30;
			
				pStats->LifeUp = upVal;

			yCASE(eAUK_DEFENCE)
				DWORD upVal = 0;
				if(0 < AbilLevel)	upVal += min(AbilLevel-0,5) * 5;
				if(5 < AbilLevel)	upVal += min(AbilLevel-5,5) * 7;
				if(10 < AbilLevel)	upVal += min(AbilLevel-10,5) * 9;
				if(15 < AbilLevel)	upVal += min(AbilLevel-15,5) * 11;
				if(20 < AbilLevel)	upVal += min(AbilLevel-20,5) * 13;
				if(25 < AbilLevel)	upVal += min(AbilLevel-25,5) * 19;
			
				pStats->DefenceUp = upVal;

			yCASE(eAUK_MAXNEARYUK)
				DWORD upVal = 0;
				if(0 < AbilLevel)	upVal += min(AbilLevel-0,5) * 10;
				if(5 < AbilLevel)	upVal += min(AbilLevel-5,5) * 14;
				if(10 < AbilLevel)	upVal += min(AbilLevel-10,5) * 18;
				if(15 < AbilLevel)	upVal += min(AbilLevel-15,5) * 22;
				if(20 < AbilLevel)	upVal += min(AbilLevel-20,5) * 26;
				if(25 < AbilLevel)	upVal += min(AbilLevel-25,5) * 30;
			
				pStats->NaeRyukUp = upVal;

			yCASE(eAUK_MAXSHIELD)
				DWORD upVal = 0;
				if(0 < AbilLevel)	upVal += min(AbilLevel-0,5) * 10;
				if(5 < AbilLevel)	upVal += min(AbilLevel-5,5) * 14;
				if(10 < AbilLevel)	upVal += min(AbilLevel-10,5) * 18;
				if(15 < AbilLevel)	upVal += min(AbilLevel-15,5) * 22;
				if(20 < AbilLevel)	upVal += min(AbilLevel-20,5) * 26;
				if(25 < AbilLevel)	upVal += min(AbilLevel-25,5) * 30;
			
				pStats->ShieldUp = upVal;
				
			yCASE(eAUK_UNGISPEED)
				pStats->UngiUpVal = AbilLevel*10;

		yENDSWITCH
	}


#ifdef _MAPSERVER_
	if(UpdateCmd != ABILITYUPDATE_ABILITYINITED)
	{
		CCharacterCalcManager::CalcMaxLife(pObj);
		CCharacterCalcManager::CalcMaxShield(pObj);
		CCharacterCalcManager::CalcMaxNaeRyuk(pObj);
	}
#else
	GAMEIN->GetCharacterDialog()->UpdateData();
#endif
}
*/