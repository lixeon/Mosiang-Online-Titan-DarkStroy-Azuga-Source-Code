// AIManager.cpp: implementation of the CAIManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AIManager.h"
#include "Player.h"
#include "Monster.h"
#include "CharMove.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAIManager::CAIManager()
{

}

CAIManager::~CAIManager()
{

}
DWORD CAIManager::RandCurAttackKind(BASE_MONSTER_LIST	* apr, CAIParam * aprm)
{
	switch(apr->AttackKind)
	{
	case 0:		// 랜덤
		{
			DWORD	sRate = 0;
			DWORD	eRate = 0;
			DWORD rate = rand() % 99;
			for( DWORD i = 0 ; i < apr->AttackNum ; ++i )
			{
				eRate += apr->AttackRate[i];
				if( sRate <= rate && rate < eRate )
				{
					return (aprm->CurAttackKind=i);
				}
				sRate = eRate;
			}

			ASSERT(0);
		}
		break;
	case 1:		// 패턴
		{
			SMONSTER_LIST	* apr2 = (SMONSTER_LIST	*)apr;
			ATTACKPATTERNINFO * pAInfo = &apr2->AttackPatternClass.pAttackPatternInfo[aprm->CurAttackPatternNum];
			WORD tmp=aprm->CurAttackPatternIndex;
			if(++aprm->CurAttackPatternIndex == pAInfo->AttackPatternNum)
				aprm->CurAttackPatternIndex = 0;

			return (aprm->CurAttackKind=pAInfo->AttackPattern[aprm->CurAttackPatternIndex]);
		}
		break;
	}
	
	ASSERT(0);
	return (aprm->CurAttackKind=0);
}
BOOL CAIManager::IsTargetChange(CPlayer * pAttacker, CMonster * pDefender)
{
	CObject * pTObject = pDefender->GetTObject();
	if( pAttacker == pTObject )	//KES 20051228.. 이걸 체크 안하고 있었다니..OTL
		return FALSE;

	if(pTObject)
	{
		BASE_MONSTER_LIST* pMonsterSInfo = pDefender->GetSMonsterList();
		if(pMonsterSInfo->TargetChange)
		{
			VECTOR3 AttackerPos	= *CCharMove::GetPosition(pAttacker);
			VECTOR3 TObjectPos	= *CCharMove::GetPosition(pTObject);
			VECTOR3 DefenderPos	= *CCharMove::GetPosition(pDefender);
			DWORD	FormerDistance1		= (DWORD)CalcDistanceXZ( &TObjectPos, &DefenderPos );
			DWORD	NewDistance2		= (DWORD)CalcDistanceXZ( &AttackerPos, &DefenderPos );
			
			LONG DistGab = FormerDistance1-NewDistance2;
			if(-50 <= DistGab && DistGab <= 50)
			{
				// 거리가 같으면 Life 체크
				if(pAttacker->GetLife() < pTObject->GetLife())
				{
					return TRUE;
				}
				else
				{
					return FALSE;
				}
			}
			else
			{
				if(DistGab > 0)
					return TRUE;
				else
					return FALSE;
			}

		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return TRUE;
	}
}

BOOL CAIManager::IsRunawayStateChange(CMonster * pDefender, CPlayer * pAttacker)
{
	// 일반 몬스터 일 경우만
	BYTE objectKind = pDefender->GetObjectKind();
	if( (objectKind != eObjectKind_Monster) || (objectKind != eObjectKind_TitanMonster) ) return FALSE;

	MONSTEREX_LIST	* apr = (MONSTEREX_LIST	*)pDefender->GetSMonsterList();
	CAIParam * aprm = &(pDefender)->m_aiParam;
	CStateParam * pr = &(pDefender)->m_stateParam;

	if(aprm->RunawayType == eRunawayType_Terminate) return FALSE;

	// 도망 생명력 조건
	if( (float)pDefender->GetLife() / (float)pDefender->GetMaxLife() < apr->fRunawayLifeRate )
	{

		// 도망 확률
		WORD Seed = random(1, 100);
		if( Seed <= apr->RunawayRate )
		{
			WORD TypeSeed = random(0, 100);
			if( TypeSeed <= apr->RunawayHelpRate )
			{
				// 1도움 요청형
				aprm->pHelperMonster = pDefender->DoFriendSearch(apr->RunawayDistance);
				if(!aprm->pHelperMonster) return FALSE;

				aprm->pTarget = pAttacker;
				ASSERT(aprm->pTarget);
				aprm->RunawayType = eRunawayType_Help;
			}
			else
			{
				// 2단순 도망형
				aprm->RunawayType = eRunawayType_General;
			}
			return TRUE;
		}

	}
	return FALSE;
}
