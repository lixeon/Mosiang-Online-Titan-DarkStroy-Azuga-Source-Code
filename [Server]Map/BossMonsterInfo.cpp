// BossMonsterInfo.cpp: implementation of the CBossMonsterInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BossMonsterInfo.h"
#include "MHFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBossMonsterInfo::CBossMonsterInfo()
{
	m_MonsterKind = 0;
	m_RegenMinTime = 0;
	m_RegenMaxTime = 0;
	m_FirstAttackIdx = 0;
	m_MaxEventStateNum = 0;
	m_SummonFileNum = 0;
	m_DieGroupID = 1;
	m_ChangeTargetMinRange = 0;
	m_ChangeTargetMaxRange = 0;
	m_EventState = NULL;

	/// 06. 08. 2차 보스 - 이영준
	/// 타겟팅 방식
	/// 0: 기존 방식 1: 매 공격시 랜덤으로 타겟 변경
	m_TargetingType = 0;

	/// 06. 09. 2차 보스 - 이영준
	/// 소멸시간
	m_DeleteTime = 0;
}

CBossMonsterInfo::~CBossMonsterInfo()
{

}

void CBossMonsterInfo::Release()
{
	SAFE_DELETE_ARRAY(m_EventState);

	
	map<ATTACKIDX,NEXTATTACKINFO*>::iterator iter;
	for(iter = m_mapNextAttackInfo.begin(); iter != m_mapNextAttackInfo.end(); ++iter)
	{
		NEXTATTACKINFO* pNewNextAttackInfo = iter->second;
		delete pNewNextAttackInfo;
		pNewNextAttackInfo = NULL;
	}
	m_mapNextAttackInfo.clear();
}

WORD CBossMonsterInfo::GetMonsterKind()
{
	return m_MonsterKind;
}

BOSSEVENTSTATE* CBossMonsterInfo::GetEventStateInfo()
{
	return m_EventState;
}

void CBossMonsterInfo::SetMonsterKind(WORD MonsterKind)
{
	m_MonsterKind = MonsterKind;
}

void CBossMonsterInfo::SetRegenDelayTime(DWORD minTime, DWORD maxTime)
{
	ASSERT(minTime < maxTime);
	m_RegenMinTime = minTime;
	m_RegenMaxTime = maxTime;
}

DWORD CBossMonsterInfo::CalcRegenDelayTime()
{
	return random(m_RegenMinTime, m_RegenMaxTime) * 60000;
}

void CBossMonsterInfo::SetMaxStateNum(BYTE num)
{
	m_MaxEventStateNum = num;
	
	m_EventState = new BOSSEVENTSTATE[m_MaxEventStateNum];
	
	for(int i=0; i<m_MaxEventStateNum; ++i)
	{
		m_EventState[i].Action = 0;
		m_EventState[i].Condition = 0;
		m_EventState[i].ConditionValue = 0;
		m_EventState[i].Count = 0;
	}
}

void CBossMonsterInfo::AddAttackInfo(DWORD CurAttackIdx, NEXTATTACKINFO* pNextAttackInfo)
{
	m_mapNextAttackInfo[CurAttackIdx] = pNextAttackInfo;
}

ATTACKIDX CBossMonsterInfo::GetNextAttackKind(DWORD CurAttackIdx)
{
	if( m_mapNextAttackInfo[CurAttackIdx] )
		return m_mapNextAttackInfo[CurAttackIdx]->GetNextAttackKind();

	return 0;
}

BYTE CBossMonsterInfo::GetMaxEventStateNum()
{
	return m_MaxEventStateNum;
}

BOOL CBossMonsterInfo::IsAttackEmpty()
{
	return m_mapNextAttackInfo.empty();
}

void CBossMonsterInfo::SetFirstAttackIdx(WORD AttackIdx)
{
	m_FirstAttackIdx = AttackIdx;
}

ATTACKIDX CBossMonsterInfo::GetFirstAttackIdx()
{
	return m_FirstAttackIdx;
}

void CBossMonsterInfo::SetSummonFileNum(WORD num)
{
	m_SummonFileNum= num;
}

WORD CBossMonsterInfo::GetSummonFileNum()
{
	return m_SummonFileNum;
}

void CBossMonsterInfo::SetDieGroupID(WORD GroupID)
{
	m_DieGroupID = GroupID;
}

WORD CBossMonsterInfo::GetDieGroupID()
{
	return m_DieGroupID;
}

BOSSUNGIJOSIK* CBossMonsterInfo::GetUngiInfo()
{
	return &m_BossUngijosik;
}

void CBossMonsterInfo::SetBossUngijosik(DWORD StartTime, DWORD DelayTime, float LifeRate, float ShieldRate)
{
	m_BossUngijosik.dwStartTime = StartTime;
	m_BossUngijosik.dwDelayTime = DelayTime;
	m_BossUngijosik.fLifeRate = LifeRate;
	m_BossUngijosik.fShieldRate = ShieldRate;
}

void CBossMonsterInfo::SetChangeTargetRange(float MinRange, float MaxRange)
{
	m_ChangeTargetMinRange = MinRange;
	m_ChangeTargetMaxRange = MaxRange;
}

float CBossMonsterInfo::GetChangeTargetMinRange()
{
	return m_ChangeTargetMinRange;
}

float CBossMonsterInfo::GetChangeTargetMaxRange()
{
	return m_ChangeTargetMaxRange;
}