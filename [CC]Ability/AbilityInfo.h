// AbilityInfo.h: interface for the CAbilityInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABILITYINFO_H__456C80E0_D384_4248_93A7_3F32B1BD9228__INCLUDED_)
#define AFX_ABILITYINFO_H__456C80E0_D384_4248_93A7_3F32B1BD9228__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMHFile;

#include "MHFile.h"
#include "AbilityCommonHeader.h"


struct ABILITY_INFO
{
	WORD Ability_index;
	char Ability_name[20];
	BYTE Ability_Kind;
	WORD Ability_Position;

	WORD Ability_AcquireKind;
	WORD Ability_AcquireParam1;
	WORD Ability_AcquireParam2;

	WORD Ability_IconIndex;
	
	BYTE Ability_AcquireInitLevel;

	WORD Ability_Gradeup_method1;
	WORD Ability_Gradeup_method2;
	WORD Ability_Gradeup_method3;

	WORD Ability_use_type;
	WORD Ability_effect_Kind;
	int Ability_effect_Param1;
	int Ability_effect_Param2;
	int Ability_effect_Param3;

	DWORD Ability_Grade_exp[MAX_ABILITY_LEVEL]; 

	WORD tooltip_text;

	// magi82(42) - 샵아이템 추가(수련치 초기화)
	BYTE Ability_ResetFlag;	// 1이면 리셋 가능, 0이면 리셋 불가능
};

class CAbilityInfo  
{
	ABILITY_INFO m_AbilityInfo;
public:
	CAbilityInfo(CMHFile* pFile);
	~CAbilityInfo();

	BOOL LoadAbilityInfo(CMHFile* pFile);

	WORD GetIdx()		{	return m_AbilityInfo.Ability_index;	}
	BYTE GetKind()	{	return m_AbilityInfo.Ability_Kind; 	}
	WORD GetPosition() { return m_AbilityInfo.Ability_Position;	}
	WORD GetAbsPosition() { return m_AbilityInfo.Ability_Kind*MAX_ABILITY_NUM_PER_GROUP + m_AbilityInfo.Ability_Position + TP_ABILITY_START; }
	
	// magi82(42) - 샵아이템 추가(수련치 초기화)
	// 1이면 리셋 가능, 0이면 리셋 불가능
	BYTE GetResetFlag()	{	return m_AbilityInfo.Ability_ResetFlag; 	}

	DWORD GetAbilityGradeExp(int n)
	{
		if( n <= MAX_ABILITY_LEVEL )
			return m_AbilityInfo.Ability_Grade_exp[n-1];
		else
			return -1;
	}
	WORD GetUseType() { return m_AbilityInfo.Ability_use_type; }

	ABILITY_INFO* GetInfo()	{	return &m_AbilityInfo;	}
};

#endif // !defined(AFX_ABILITYINFO_H__456C80E0_D384_4248_93A7_3F32B1BD9228__INCLUDED_)
