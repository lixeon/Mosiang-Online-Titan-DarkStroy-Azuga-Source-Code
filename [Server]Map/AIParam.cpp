// AIParam.cpp: implementation of the CAIParam class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AIParam.h"
#include "AIDefine.h"
#include "SkillManager_Server.h"
#include "Monster.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAIParam::CAIParam()
{
	
}

CAIParam::~CAIParam()
{


}

void CAIParam::Init()
{
	AttackStartTime			= 0;
	CurAttackKind			= 0;
	CurAttackPatternNum		= 0;
	CurAttackPatternIndex	= 0;

	SearchLastTime			= gCurTime + 5000;	//5ÃÊ°£ ¸®Á¨µÇ°í Àá½ÃÈÄ.
	CollSearchLastTime		= gCurTime + 5000;	//5ÃÊ°£
	PursuitForgiveStartTime	= gCurTime;
	RunawayType				= eRunawayType_None;
	pTarget					= NULL;
	pHelperMonster			= NULL;
	prePursuitForgiveTime	= 0;
}

DWORD CAIParam::GetCurAttackKind()
{
	return CurAttackKind;
}
/*
DWORD CAIParam::RandCurAttackKind(CMonster * pMonster)
{
	BASE_MONSTER_LIST	* apr = pMonster->GetSMonsterList();

	switch(apr->AttackKind)
	{
	case 0:		// ·£´ý
		{
			DWORD	sRate = 0;
			DWORD	eRate = 0;
			DWORD rate = rand() % 100;
			for( DWORD i = 0 ; i < apr->AttackNum ; ++i )
			{
				eRate += apr->AttackRate[i];
				if( sRate <= rate && rate < eRate )
				{
					return (CurAttackKind=i);
				}
				sRate = eRate;
			}

			ASSERT(0);
		}
		break;
	case 1:		// ÆÐÅÏ
		{
			SMONSTER_LIST	* apr2 = (SMONSTER_LIST	*)apr;
			ATTACKPATTERNINFO * pAInfo = &apr2->AttackPatternClass.pAttackPatternInfo[CurAttackPatternNum];
			WORD tmp=CurAttackPatternIndex;
			if(++CurAttackPatternIndex == pAInfo->AttackPatternNum)
				CurAttackPatternIndex = 0;

			return (CurAttackKind=pAInfo->AttackPattern[CurAttackPatternIndex]);
		}
		break;
	}
	
	ASSERT(0);
	return (CurAttackKind=0);
}*/

/*
oid CAIParam::Init(MONSTER_LIST * pList)
{
	StandRate					= pList->StandRate;
	DomainRange					= pList->DomainRange;				
	PursuitForgiveTime			= pList->ForgiveTime;
	PursuitForgiveDistance		= pList->ForgiveDist;	

	AttackNum					= pList->AttackNum;
	AttackIndex					= new DWORD[AttackNum];
	AttackRange					= new DWORD[AttackNum];
	AttackRate					= new DWORD[AttackNum];
	AttackStartTime				= new DWORD[AttackNum];
	AttackDelayTime				= new DWORD[AttackNum];

	
	if( pList->Attack1Index )
	{
		AttackIndex[0] = pList->Attack1Index;
		CSkillInfo * pSkill1Info = SKILLMGR->GetSkillInfo(pList->Attack1Index);
		if( pSkill1Info )
		{
			AttackRange[0]					= (DWORD)pSkill1Info->GetSkillRange();
			AttackRate[0]					= pList->Attack1Percent;
			AttackStartTime[0]				= 0;
			AttackDelayTime[0]				= pSkill1Info->GetDelayTime();
		}		
	}

	if( pList->Attack2Index )
	{	
		AttackIndex[1] = pList->Attack2Index;
		CSkillInfo * pSkill2Info = SKILLMGR->GetSkillInfo(pList->Attack2Index);
		if( pSkill2Info )
		{
			AttackRange[1]					= (DWORD)pSkill2Info->GetSkillRange();
			AttackRate[1]					= pList->Attack2Percent;
			AttackStartTime[1]				= 0;
			AttackDelayTime[1]				= pSkill2Info->GetDelayTime();
		}		
	}


	bForeAttack					= pList->BeforeAttack;				
	SearchPeriodicTime			= 1000; //pList->SearchPeriodicTime;			
	SearchLastTime				= 0;


	TargetSelect				= pList->SearchSelect;			
	SearchRange					= pList->SearchArea;	
	TargetChange				= pList->TargetSelect;
	Gravity						= pList->Gravity;
}
*/
