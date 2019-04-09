#include "stdafx.h"
#include "SITUATION_exe.h"

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Don't Touch me
// Ask taiyo if you wanna change
#ifdef ENCODESITUATION
#undef ENCODESITUATION
#endif
#define ENCODESITUATION(a)	a,


CONDITION_FUNC globalSITUATION[]=
{
	#include "SITUATION_Incl.h"
};

#ifdef ENCODESITUATION
#undef ENCODESITUATION
#endif
#define ENCODESITUATION(a)	BOOL a##(CObject * p)

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!




// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// > Custom code list - 2003. 10. 30. taiyo
// Add user callback function (for SITUATION)
// 
// ex)	
// 1. Add ENCODESITUATION( yourCallBackFunctionName ) code in File"SITUATION_incl.h"
// 2. Your ENCODESITUATION( yourCallBackFunctionName ){ } function is implemented
// 3. Write your situation code!!
// --------------------------------------------------------------------------------------------
// #include header
#include "BossMonster.h"
#include "UserTable.h"
#include "BossMonsterManager.h"
#include "SkillInfo.h"
// #include header

ENCODESITUATION(IsTest)
{
	//bad
	return FALSE;
	//good
	return TRUE;
}
ENCODESITUATION(IsBossMonster65HPLessThan50)
{
/*	CBossMonster * pMonster = BOSSMONSTERMNGR->GetBoss();
	if(!pMonster) 
		return FALSE;
	BASE_MONSTER_LIST	* apr = pMonster->GetSMonsterList();
	
	if( apr->SkillInfo[2]->GetDelayTime() + pMonster->m_aiParam.AttackStartTime < gCurTime )
	{
		if( pMonster->GetLife() <= ((float)pMonster->GetMaxLife() / 2.f) )
		{
			return TRUE;
		}
	}
*/
	return FALSE;
}