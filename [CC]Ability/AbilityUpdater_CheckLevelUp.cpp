// AbilityUpdater_CheckLevelUp.cpp: implementation of the CAbilityUpdater_CheckLevelUp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AbilityUpdater_CheckLevelUp.h"
#include "AbilityGroup.h"
#include "AbilityCommonHeader.h"
#include "Player.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAbilityUpdater_CheckLevelUp::CAbilityUpdater_CheckLevelUp()
{

}

CAbilityUpdater_CheckLevelUp::~CAbilityUpdater_CheckLevelUp()
{

}

BOOL CAbilityUpdater_CheckLevelUp::GetAbility(CAbilityGroup* pGroup,CAbilityInfo* pAbilityInfo)
{
	BYTE InitLevel = pAbilityInfo->GetInfo()->Ability_AcquireInitLevel;
	if(InitLevel == 0)
		return FALSE;

	pGroup->AddAbility(pAbilityInfo,InitLevel,0);
	return TRUE;
}

void CAbilityUpdater_CheckLevelUp::Update(DWORD& UpdateCmd,DWORD Param,CAbilityGroup* pGroup,CYHHashTable<CAbilityInfo>* pInfoTable)
{
	BOOL bAbilityLevelChanged = FALSE;
	CPlayer* pObj = pGroup->GetOwenerObject();
	CAbilityInfo* pAbilityInfo;
	ABILITY_INFO* pInfo;
	pInfoTable->SetPositionHead();
	while(pAbilityInfo = pInfoTable->GetData())
	{
		// 레벨이 0인 특기에 대해서만 검사 한다.
		if(pGroup->GetAbility(pAbilityInfo->GetIdx()))
			continue;

		pInfo = pAbilityInfo->GetInfo();

		// 케릭의 레벨이 변화했을때
		if( UpdateCmd == ABILITYUPDATE_CHARACTERLEVEL_CHANGED &&
			pInfo->Ability_AcquireKind == eAAK_CharacterLevel )
		{
			LEVELTYPE Level = (LEVELTYPE)Param;
			if(Level >= pInfo->Ability_AcquireParam1)
				bAbilityLevelChanged |= GetAbility(pGroup,pAbilityInfo);
		}

		// 다른 특기의 레벨이 변화했을때
		if( UpdateCmd == ABILITYUPDATE_ABILITYLEVEL_CHANGED &&
			pInfo->Ability_AcquireKind == eAAK_OtherAbility )
		{
			WORD NeedAbilityIdx = pInfo->Ability_AcquireParam1;
			BYTE NeedAbilityLevel = (BYTE)pInfo->Ability_AcquireParam2;
			CAbility* pNeedAbility = pGroup->GetAbility(NeedAbilityIdx);
			if(pNeedAbility == NULL)
				continue;
			if(pNeedAbility->GetLevel() >= NeedAbilityLevel)
				bAbilityLevelChanged |= GetAbility(pGroup,pAbilityInfo);
		}
		
		// 아이탬 썼을경우
		if( UpdateCmd == ABILITYUPDATE_ABILITYUPITEM_USE &&
			pInfo->Ability_AcquireKind == eAAK_Item )
		{
			WORD UsedItemIdx = (WORD)Param;
			WORD NeedItemIdx = pInfo->Ability_AcquireParam1;
			if(UsedItemIdx == NeedItemIdx)
				bAbilityLevelChanged |= GetAbility(pGroup,pAbilityInfo);
		}

		// 퀘스트... -_-a
	}

	if(bAbilityLevelChanged)
	{
		UpdateCmd = ABILITYUPDATE_ABILITYLEVEL_CHANGED;
		Update(UpdateCmd,0,pGroup,pInfoTable);
	}
}
