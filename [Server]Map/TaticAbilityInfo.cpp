// TaticAbilityInfo.cpp: implementation of the CTaticAbilityInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TaticAbilityInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaticAbilityInfo::CTaticAbilityInfo(TATIC_ABILITY_INFO* pInfo)
{
	memcpy( &m_Info, pInfo, sizeof(TATIC_ABILITY_INFO) );
}

CTaticAbilityInfo::~CTaticAbilityInfo()
{
}

WORD CTaticAbilityInfo::GetTacticAttackAbilityValue( WORD wLevel )
{
	if( wLevel <= 0 )	return 0;
	else if( wLevel >= MAX_TATIC_ABILITY_NUM )	wLevel = MAX_TATIC_ABILITY_NUM;

	return m_Info.wTypeAttack[wLevel-1];
}

WORD CTaticAbilityInfo::GetTacticRecoverAbilityValue( WORD wLevel )
{
	if( wLevel <= 0 )	return 0;
	else if( wLevel >= MAX_TATIC_ABILITY_NUM )	wLevel = MAX_TATIC_ABILITY_NUM;

	return m_Info.wTypeRecover[wLevel-1];
}

float CTaticAbilityInfo::GetTacticBuffRateValue( WORD wLevel )
{
	if( wLevel <= 0 )	return 0;
	else if( wLevel >= MAX_TATIC_ABILITY_NUM )	wLevel = MAX_TATIC_ABILITY_NUM;

	return m_Info.fTypeBuffRate[wLevel-1];
}

WORD CTaticAbilityInfo::GetTacticBuffValue( WORD wLevel )
{
	if( wLevel <= 0 )	return 0;
	else if( wLevel >= MAX_TATIC_ABILITY_NUM )	wLevel = MAX_TATIC_ABILITY_NUM;

	return m_Info.wTypeBuff[wLevel-1];
}
