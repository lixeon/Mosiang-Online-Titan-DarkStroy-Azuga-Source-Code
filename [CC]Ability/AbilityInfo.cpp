// AbilityInfo.cpp: implementation of the CAbilityInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AbilityInfo.h"
//#include "MHFile.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAbilityInfo::CAbilityInfo(CMHFile* pFile)
{
	LoadAbilityInfo(pFile);
}

CAbilityInfo::~CAbilityInfo()
{

}

BOOL CAbilityInfo::LoadAbilityInfo(CMHFile* pFile)
{	
	m_AbilityInfo.Ability_index = pFile->GetWord();
	//pFile->GetString(m_AbilityInfo.Ability_name);
	SafeStrCpy( m_AbilityInfo.Ability_name, pFile->GetString(), 20 );	
	m_AbilityInfo.Ability_Kind = pFile->GetByte();
	m_AbilityInfo.Ability_Position = pFile->GetWord();

	m_AbilityInfo.Ability_AcquireKind = pFile->GetWord();
	ASSERT(m_AbilityInfo.Ability_AcquireKind < eAAK_Max);
	m_AbilityInfo.Ability_AcquireParam1 = pFile->GetWord();
	m_AbilityInfo.Ability_AcquireParam2 = pFile->GetWord();
	
	m_AbilityInfo.Ability_AcquireInitLevel = pFile->GetByte();

	m_AbilityInfo.Ability_Gradeup_method1 = pFile->GetWord();
	m_AbilityInfo.Ability_Gradeup_method2 = pFile->GetWord();
	m_AbilityInfo.Ability_Gradeup_method3 = pFile->GetWord();
	
	m_AbilityInfo.Ability_IconIndex = pFile->GetWord();

	m_AbilityInfo.Ability_use_type = pFile->GetWord();
	m_AbilityInfo.Ability_effect_Kind = pFile->GetWord();
	m_AbilityInfo.Ability_effect_Param1 = pFile->GetInt();
	m_AbilityInfo.Ability_effect_Param2 = pFile->GetInt();
	m_AbilityInfo.Ability_effect_Param3 = pFile->GetInt();

	pFile->GetDword(m_AbilityInfo.Ability_Grade_exp,30);

	m_AbilityInfo.tooltip_text = pFile->GetWord();

	// magi82(42) - 샵아이템 추가(수련치 초기화)
	m_AbilityInfo.Ability_ResetFlag = pFile->GetByte();

	return TRUE;
}
