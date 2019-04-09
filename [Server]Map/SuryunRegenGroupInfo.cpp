// SuryunRegenGroupInfo.cpp: implementation of the CSuryunRegenGroupInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SuryunRegenGroupInfo.h"
#include "SuryunRegenGroup.h"
#include "RecallManager.h"
#include "Player.h"
#include "../[CC]Suryun/SuryunDefine.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSuryunRegenGroupInfo::CSuryunRegenGroupInfo()
{
	m_GroupID = -1;
	m_MonsterKind = 0;
	m_Count = 0;
	m_x = 0.f;
	m_z = 0.f;
	m_Range = 0.f;
	m_RegenTime = 0;
	for(int n=0;n<5;++n)
	{
		m_SeqArray[n].fRate = 0;
		m_SeqArray[n].GroupID = -1;
		m_SeqArray[n].GroupName[0] = 0;
	}
}

CSuryunRegenGroupInfo::~CSuryunRegenGroupInfo()
{

}

void CSuryunRegenGroupInfo::TimeRegen(CSuryunRegenGroup* pRegenGroup, DWORD elpsTime, CPlayer* pPlayer, WORD* MonsterKind)
{
	if(ChkRegen(pRegenGroup, elpsTime) == FALSE)
		return;

	DoRegen(pPlayer, GetMonsterIdx(MonsterKind, m_MonsterKind), pRegenGroup);
}

int CSuryunRegenGroupInfo::ChkRatioRegen(int SeqNum,int CurMonsterNum)
{
	if(m_SeqArray[SeqNum].GroupID == -1)
		return -1;
	float ratio = CurMonsterNum/(float)m_Count;
	if(ratio > m_SeqArray[SeqNum].fRate)
		return -1;

	return m_SeqArray[SeqNum].GroupID;
}
void CSuryunRegenGroupInfo::RatioRegen(CSuryunRegenGroup* pRegenGroup, CPlayer* pPlayer, WORD* MonsterKind)
{
	DoRegen(pPlayer, GetMonsterIdx(MonsterKind, m_MonsterKind), pRegenGroup);
}

WORD CSuryunRegenGroupInfo::GetMonsterIdx(WORD* MonsterIdxAry, int MonsterKind)
{
	if(MonsterKind == 10)
		return COMPLETELY_ALL_RECOVER;
	else if(MonsterKind == 11)
		return NORMAL_LIFE_RECOVER;
	else if(MonsterKind == 12)
		return NORMAL_SHIELD_RECOVER;	
	else if(MonsterKind == 13)
		return NORMAL_NAERYUK_RECOVER;
	return MonsterIdxAry[MonsterKind];
}

void CSuryunRegenGroupInfo::DoRegen(CPlayer* pPlayer, WORD MonsterKind, CSuryunRegenGroup* pRegenGroup)
{
	if(pRegenGroup->IsExecuted() == TRUE)
		return;
	int regencount;
	VECTOR3 vPos;
	vPos.x = m_x;
	vPos.z = m_z;
	if(pPlayer)
	{
		regencount = RECALLMGR->RecallRange(pPlayer,pPlayer->GetGridID(),&vPos,MonsterKind,
											m_Count,(DWORD)m_Range, m_GroupID, FALSE, TRUE);

		int notregencount = m_Count - regencount;
		if(notregencount)
		{
			vPos.x = SURYUNENTER_POSITION_X;
			vPos.z = SURYUNENTER_POSITION_Z;
			regencount += RECALLMGR->RecallRange(pPlayer,pPlayer->GetGridID(),&vPos,MonsterKind,
											m_Count,0, m_GroupID, FALSE, TRUE);			
		}
		ASSERT(regencount == m_Count);

		pRegenGroup->SetExecute(m_Count);
	}
}		


BOOL CSuryunRegenGroupInfo::ChkRegen(CSuryunRegenGroup* pRegenGroup, DWORD elpsTime)
{
	if(m_RegenTime == 0)
		return FALSE;
	if(m_RegenTime > elpsTime)
		return FALSE;

	return TRUE;
}

void CSuryunRegenGroupInfo::fromStr(char* pBuf)
{
	char* p = pBuf;
	char* tempBuf;

	int seqCount = 0;
	char seps[]   = " ";
	p = strtok( pBuf, seps );
	while( p != NULL )
	{		
		tempBuf = strtok(NULL,seps);
		if(tempBuf[0] == '}')
			break;

		if(strcmp(tempBuf,"#GROUPNAME") == 0)
		{
			m_GroupID = atoi(strtok(NULL,seps));		// line
			strcpy(m_GroupName,strtok(NULL,seps));		// name
		}
		else if(strcmp(tempBuf,"#MONSTER") == 0)
			m_MonsterKind = atoi(strtok(NULL,seps));
		else if(strcmp(tempBuf,"#COUNT") == 0)
			m_Count = atoi(strtok(NULL,seps));
		else if(strcmp(tempBuf,"#POSITION") == 0)
		{
			m_x = (float)atof(strtok(NULL,seps));
			m_z = (float)atof(strtok(NULL,seps));
		}
		else if(strcmp(tempBuf,"#RANGE") == 0)
			m_Range = (float)atof(strtok(NULL,seps));
		else if(strcmp(tempBuf,"#REGENTIME") == 0)
			m_RegenTime = atoi(strtok(NULL,seps));
		else if(strcmp(tempBuf,"#SEQ") == 0)
		{
			m_SeqArray[seqCount].GroupID = atoi(strtok(NULL,seps));	//line
			strcpy(m_SeqArray[seqCount].GroupName,strtok(NULL,seps));
			m_SeqArray[seqCount].fRate = (float)atof(strtok(NULL,seps));

			++seqCount;
		}
	}
}