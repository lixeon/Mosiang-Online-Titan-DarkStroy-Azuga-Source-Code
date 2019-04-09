// SummonGroup.cpp: implementation of the CSummonGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SummonGroup.h"
#include "MHFile.h"
#include "RecallManager.h"
#include "BossMonster.h"
#include "CharMove.h"
//#include "MHError.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define ADDSUMMONMON	"#ADD"

CSummonGroup::CSummonGroup()
{
	memset(&m_SeedPos, 0, sizeof(VECTOR3));
}

CSummonGroup::~CSummonGroup()
{
	Release();
}

void CSummonGroup::Release()
{
	vector<SUMMONMONSTERINFO*>::iterator it;
	for(it = m_vecSummonMonster.begin(); it != m_vecSummonMonster.end(); ++it)
	{
		delete *it;
		*it = NULL;
	}
	m_vecSummonMonster.clear();
}

void CSummonGroup::AddSummonMonsterInfo(BYTE ObjectKind, WORD MonsterKind, float x, float z)
{
	m_vecSummonMonster.push_back(new SUMMONMONSTERINFO(ObjectKind, MonsterKind, x, z));
}

void CSummonGroup::SetSeedPos(POSTYPE x, POSTYPE z)
{
	m_SeedPos.x = x;
	m_SeedPos.z = z;
}

void CSummonGroup::RegenGroup(CBossMonster* pBoss)
{
	vector<SUMMONMONSTERINFO*>::iterator it;
	VECTOR3 vPos, ResultSeedPos;
	VECTOR3 SeedPos = GetSeedPos();
	if((SeedPos.x == 0) && (SeedPos.z == 0))
	{
		ResultSeedPos.x = ResultSeedPos.z = 0;
	}
	else
	{
		ResultSeedPos = (*CCharMove::GetPosition(pBoss) + SeedPos) / 2;
	}
	for(it = m_vecSummonMonster.begin(); it != m_vecSummonMonster.end(); ++it)
	{
		vPos.x = (*it)->x + ResultSeedPos.x;
		vPos.z = (*it)->z + ResultSeedPos.z;
		RECALLMGR->RecallRange(pBoss, pBoss->GetGridID(), &vPos, (*it)->MonsterKind, 1, 100, 0, FALSE, TRUE, (*it)->ObjectKind);
		//for test log
		//
/*		if((SeedPos.x == 0) && (SeedPos.z == 0))
		{
			MHERROR->OutputFile("Debug.txt", MHERROR->GetStringArg("BoMul Regen Position. x: %f, z: %f", vPos.x, vPos.z));
		}
		//
*/	}	
}

VECTOR3 CSummonGroup::GetSeedPos()
{
	return m_SeedPos;
}