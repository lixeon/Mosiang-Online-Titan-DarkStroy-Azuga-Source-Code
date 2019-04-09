// AbilityUse_KyungGong.cpp: implementation of the CAbilityUse_KyungGong class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AbilityUse_KyungGong.h"

#ifdef _MHCLIENT_
#include "ObjectManager.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAbilityUse_KyungGong::CAbilityUse_KyungGong()
{

}

CAbilityUse_KyungGong::~CAbilityUse_KyungGong()
{

}



BOOL CAbilityUse_KyungGong::Activate(BYTE Level,CAbilityInfo* pAbilityInfo)
{
#ifdef _MHCLIENT_
	//abil hack check
 	WORD LimitKind = pAbilityInfo->GetInfo()->Ability_AcquireKind;
 	WORD LimitLevel = pAbilityInfo->GetInfo()->Ability_AcquireParam1;
 
 	if( LimitKind == eAAK_CharacterLevel )
 	{
 		if( LimitLevel > HERO->GetMaxLevel() )
 			return FALSE;
 	}

	WORD KyungGongIdx = pAbilityInfo->GetInfo()->Ability_effect_Param1;
	HERO->SetKyungGongGrade(KyungGongIdx);
	
	HERO->SetKyungGongMode(TRUE);
#endif

	return TRUE;
}

BOOL CAbilityUse_KyungGong::UnActivate(BYTE Level,CAbilityInfo* pAbilityInfo)
{
#ifdef _MHCLIENT_
	
	WORD KyungGongIdx = pAbilityInfo->GetInfo()->Ability_effect_Param1;
	HERO->SetKyungGongGrade(0);
	
	HERO->SetKyungGongMode(FALSE);
#endif

	return FALSE;
}