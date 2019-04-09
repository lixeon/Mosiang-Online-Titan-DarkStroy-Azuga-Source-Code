#include "stdafx.h"
#include "AbilityCommonHeader.h"

WORD GetAbilityIndex_WithKindPosition(BYTE Kind,BYTE Position)
{
	ASSERT(Kind < eAGK_Max);
	ASSERT(Position < MAX_ABILITY_NUM_PER_GROUP);
	return ABILITY_STARTINDEX_BATTLE + ABILITY_STARTINDEX_INTERVAL*Kind + Position;
}

void GetKindPosition_WithAbilityIndex(WORD AbilityIndex,BYTE* pOutKind,BYTE* pOutPosition)
{
	*pOutKind = (AbilityIndex-ABILITY_STARTINDEX_BATTLE)/ABILITY_STARTINDEX_INTERVAL;
	*pOutPosition = AbilityIndex%ABILITY_STARTINDEX_INTERVAL;
}

BYTE GetAbilityLevel_WithDBValue(BYTE DBValue)
{
	ASSERT(DBValue >= ABILITYLEVEL_DB_KEY);
	ASSERT(DBValue-ABILITYLEVEL_DB_KEY <= ABILITY_MAX_LEVEL);
	
	return DBValue - ABILITYLEVEL_DB_KEY;
}

// 06. 01 이영준 - 단축창 변경
// 이 함수를 사용하지 않는다
/*
WORD GetAbilityQuickPosition(BYTE DBValue)
{
	if(DBValue == ABILITYQUICKPOSITION_NULL)
		return 0;
	
	return (DBValue - ABILITYLEVEL_DB_KEY) + TP_QUICK_START;
}
*/
