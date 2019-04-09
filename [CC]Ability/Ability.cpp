// Ability.cpp: implementation of the CAbility class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Ability.h"
#include "AbilityInfo.h"
#include "AbilityCommonHeader.h"
#include "AbilityManager.h"
#include "AbilityUse_KyungGong.h"
#include "AbilityUse_Job.h"
#include "AbilityUse_Society.h"
#include "AbilityUse_Pet.h"

#ifdef _MHCLIENT_
#include "ObjectManager.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WORD CAbility::GetIndex()
{
	ASSERT(m_pAbilityInfo);
	return m_pAbilityInfo->GetIdx();
}

BYTE CAbility::GetLevel()
{
	return m_Level;
}

WORD CAbility::GetKind()
{
	return m_pAbilityInfo->GetKind();
}

void CAbility::Init(CAbilityInfo* pInfo, BYTE Level,POSTYPE QuickPos)
{
	m_pAbilityInfo = pInfo;
	m_Level = Level;
	m_bActivating = FALSE;
	m_QuickPosition = QuickPos;
}

void CAbility::IncreaseLevel()
{
	++m_Level;
}

void CAbility::DecreaseLevel()
{
	--m_Level;
}


void CAbility::Use()
{
	ySWITCH(m_pAbilityInfo->GetInfo()->Ability_effect_Kind)
		yCASE(eAUK_KYUNGGONG)
			CAbilityUse_KyungGong use;
			if(m_bActivating == FALSE)
				m_bActivating = use.Activate(GetLevel(),GetAbilityInfo());
			else
				m_bActivating = use.UnActivate(GetLevel(),GetAbilityInfo());

		yCASE(eAUK_JOB)
			CAbilityUse_Job use;
			use.Use(GetLevel(),GetAbilityInfo());
		yCASE(eAUK_PET)
			CAbilityUse_Pet	use;
			use.Use(GetLevel(),GetAbilityInfo());
#ifdef _MHCLIENT_			
		yCASE(eAUK_SOCIETY)
			CAbilityUse_Society use;
			use.Act( HERO, GetAbilityInfo() );
#endif
		yDEFAULT
	yENDSWITCH
}