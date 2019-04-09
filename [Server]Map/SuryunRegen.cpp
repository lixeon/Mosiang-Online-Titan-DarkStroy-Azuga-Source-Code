// SuryunRegen.cpp: implementation of the CSuryunRegen class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../[CC]Suryun/SuryunDefine.h"
#include "SuryunRegen.h"
#include "SuryunRegenInfo.h"
#include "SuryunRegenGroup.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSuryunRegen::CSuryunRegen()
{
	m_StartTime = 0;
	memset(&m_MonsterKindInfo, 0, sizeof(m_MonsterKindInfo));
	m_RegenGroupArray = NULL;
	m_pPlayer = NULL;
}

CSuryunRegen::~CSuryunRegen()
{
	Release();
}

void CSuryunRegen::Release()
{
	if(m_RegenGroupArray)
	{	
		delete [] m_RegenGroupArray;
		m_RegenGroupArray = NULL;
	}
}

void CSuryunRegen::Init(CSuryunRegenInfo* pRegenInfo, WORD* MonsterKindInfo)
{
	for(int i=0; i<MAX_REGENMONSTER_KIND; ++i)
		m_MonsterKindInfo[i] = MonsterKindInfo[i];
	m_RegenInfo = pRegenInfo;
	m_pPlayer = NULL;
	m_RegenGroupArray = new CSuryunRegenGroup[pRegenInfo->GetMaxCount()];
}

void CSuryunRegen::StartProcess()
{
	m_StartTime = gCurTime;
}

void CSuryunRegen::Process()
{
	DWORD elpsTime = gCurTime - m_StartTime;
	for(int i=0; i<m_RegenInfo->GetMaxCount(); ++i)
	{
		if(m_RegenGroupArray[i].IsExecuted() == TRUE)
			continue;		
		m_RegenInfo->TimeRegen(&m_RegenGroupArray[i], i, elpsTime, m_pPlayer, m_MonsterKindInfo);
	}
}

void CSuryunRegen::DecreaseMonster(int GroupID)
{
	if(GroupID >= m_RegenInfo->GetMaxCount())
	{
		ASSERT(0);
		return;
	}
	int CurMonNum = m_RegenGroupArray[GroupID].DecreaseAliveMonsterNum();

	m_RegenInfo->RatioRegen(m_RegenGroupArray,GroupID,CurMonNum,m_pPlayer,m_MonsterKindInfo);
}