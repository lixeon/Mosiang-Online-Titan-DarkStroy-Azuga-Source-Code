// PartyIconMoveInfo.cpp: implementation of the CPartyIconMoveInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartyIconMoveInfo.h"
#include "MiniMapIcon.h"
#include "MoveManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPartyIconMoveInfo::CPartyIconMoveInfo()
{
	m_KyungGongIdx	= 0;
	m_MoveState		= eMoveMode_Walk;
	m_tgMaxIndex	= 0;
	m_tgIndex		= -1;
	m_MoveStartTime	= 0;
	m_MoveTime		= 0.f;
	m_Speed			= 0.f;
	m_DirectionX	= 0.f;
	m_DirectionZ	= 0.f;
}

CPartyIconMoveInfo::~CPartyIconMoveInfo()
{
}


void CPartyIconMoveInfo::Reset()
{
	m_KyungGongIdx	= 0;
	m_MoveState		= eMoveMode_Walk;
	m_tgMaxIndex	= 0;
	m_tgIndex		= -1;
	m_MoveStartTime	= 0;
	m_MoveTime		= 0.f;
	m_Speed			= 0.f;
	m_DirectionX	= 0.f;
	m_DirectionZ	= 0.f;
	
	memset(m_tPos, 0, sizeof(MoveCoord)*MAX_CHARTARGETPOSBUF_SIZE);
}


void CPartyIconMoveInfo::SetMoveInfo(PARTYICON_MOVEINFO* pMoveInfo)
{
	m_cPos = pMoveInfo->cPos;
	m_tgMaxIndex = pMoveInfo->tgCount;
	memset(m_tPos, 0, sizeof(MoveCoord)*MAX_CHARTARGETPOSBUF_SIZE);
	for(int i=0; i<m_tgMaxIndex; i++)
	{
		m_tPos[i].posX = pMoveInfo->tgPos[i].posX;
		m_tPos[i].posZ = pMoveInfo->tgPos[i].posZ;
	}	
	m_KyungGongIdx = pMoveInfo->KyungGongIdx;
	m_MoveState = pMoveInfo->MoveState;
	m_tgIndex = 0;
	
	SetMoveSpeed();
}

void CPartyIconMoveInfo::SetKoungGongIdx(int Index)
{
	m_KyungGongIdx = Index;
	SetMoveSpeed();		
}

void CPartyIconMoveInfo::SetMoveState(int State)
{
	m_MoveState = State;
	SetMoveSpeed();
}

void CPartyIconMoveInfo::SetMoveSpeed()
{
	if(m_KyungGongIdx)
		m_Speed = MOVEMGR->GetKyungGongSpeed(m_KyungGongIdx);
	else
	{
		if(m_MoveState == eMoveMode_Run)
			m_Speed = RUNSPEED;
		else
			m_Speed = WALKSPEED;
	}

	CalMoveUpdate();
}

void CPartyIconMoveInfo::CalMoveUpdate()
{
	if(m_tgIndex == -1)		return;

	MoveCoord TargetPos = m_tPos[m_tgIndex];

	int dx = TargetPos.posX - m_cPos.posX;
	int dz = TargetPos.posZ - m_cPos.posZ;;
	float Distance = sqrtf((float)(dx*dx+dz*dz));

	m_MoveStartTime = gCurTime;
	m_MoveTime = Distance / m_Speed;

	m_DirectionX = dx*(m_Speed/Distance);
	m_DirectionZ = dz*(m_Speed/Distance);
}

/*
void CPartyIconMoveInfo::MoveStop()
{
	m_tgIndex = -1;
	memset(m_tPos, 0, sizeof(MoveCoord)*MAX_CHARTARGETPOSBUF_SIZE);
	m_MoveTime = 0.f;
	m_Speed = 0.f;
	m_KyungGongIdx = 0;
}
*/

void CPartyIconMoveInfo::Process()
{
	if(m_tgIndex == -1)		return;

	MoveCoord TargetPos = m_tPos[m_tgIndex];

	float fMoveTime = ((gCurTime - m_MoveStartTime)*0.001f);
	if(fMoveTime < m_MoveTime)
	{
		m_IconPos.posX = (WORD)(m_cPos.posX + (m_DirectionX * fMoveTime));
		m_IconPos.posZ = (WORD)(m_cPos.posZ + (m_DirectionZ * fMoveTime));
		
		float dx = (float)(TargetPos.posX - m_IconPos.posX);
		float dz = (float)(TargetPos.posZ - m_IconPos.posZ);
		float Distance = sqrtf(dx*dx+dz*dz);
		
		if(Distance < 20.f)
		{
			++m_tgIndex;
			m_IconPos = TargetPos;
			m_cPos = TargetPos;
			
			if(m_tgIndex >= m_tgMaxIndex)			m_tgIndex = -1;
		}
	}
}
