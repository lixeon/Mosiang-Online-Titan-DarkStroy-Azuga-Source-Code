// BossState.cpp: implementation of the CBossState class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BossState.h"
#include "MHError.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBossState::CBossState()
{
	Init();
}

CBossState::~CBossState()
{

}

void CBossState::SetState(int State)
{
	m_NextState = State;
	m_bStateUpdate = TRUE;		
	m_bStateFirst = FALSE;
}

void CBossState::SetStateFirst(BOOL vals)
{
	m_bStateFirst = vals;
}

BOOL CBossState::IsStateFirst()
{
	return m_bStateFirst;
}

int CBossState::GetCurState()
{
	return m_CurState;
}

int CBossState::GetNextState()
{
	return m_NextState;
}

DWORD CBossState::GetCurStateEndTime()
{
	return m_StateEndTime;
}

BOOL CBossState::IsStateUpdated()
{
	return m_bStateUpdate;
}

void CBossState::UpdateState()
{
	if(m_CurState != eBossState_Stand)
	{
		if(m_StateEndTime > gCurTime)
		{
			return;			
		}
	}
//	MHERROR->OutputFile("Debug.txt", MHERROR->GetStringArg("Set CurState : %d, NextState: %d", m_CurState, m_NextState));
	m_CurState = m_NextState;
	m_StateStartTime = gCurTime;
	m_bStateUpdate = FALSE;
	m_NextState = 0;
	m_bStateFirst = TRUE;
}

void CBossState::SetStateStartTime(DWORD time)
{
	m_StateStartTime = time;
}

DWORD CBossState::GetStateStartTime()
{
	return m_StateStartTime;
}

void CBossState::SetStateEndTime(DWORD time)
{
	m_StateEndTime = time;
}

DWORD CBossState::GetStateEndTime()
{
	return m_StateEndTime;
}

void CBossState::Init()
{
	m_CurState = eBossState_Stand;
	m_NextState = 0;
	m_StateStartTime = 0;
	m_StateEndTime = 0;
	m_bStateUpdate = FALSE;
	m_bStateFirst = TRUE;
}

void CBossState::SetNextState(int state)
{
	m_NextState = state;
	m_bStateUpdate = TRUE;
}