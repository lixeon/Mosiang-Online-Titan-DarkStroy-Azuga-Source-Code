#include "stdafx.h"
#include "SkillObjectFirstUnit_SingleSpecialState.h"

#ifdef _MAPSERVER_
#include "PackedData.h"
#include "AttackManager.h"
#include "SkillObject_server.h"
#include "SkillManager_server.h"
#else
#include "ObjectManager.h"
#include "ObjectActionManager.h"
#include "Gamein.h"
#include "SkillObject_Client.h"
#include "SkillManager_client.h"
#endif

#include "Player.h"

CSkillObjectFirstUnit_SingleSpecialState::CSkillObjectFirstUnit_SingleSpecialState(SKILLINFO* pSkillInfo)
: CSkillObjectFirstUnit(pSkillInfo)
{
}

CSkillObjectFirstUnit_SingleSpecialState::~CSkillObjectFirstUnit_SingleSpecialState(void)
{
}

void CSkillObjectFirstUnit_SingleSpecialState::Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper)
{
	m_State = pSkillInfo->GetSpecialState();
}

#ifdef _MAPSERVER_
int CSkillObjectFirstUnit_SingleSpecialState::ExcuteFirstUnit(CObject* pOperator,CSkillObjectTargetList* pTargetList,	MSG_SKILLOBJECT_ADD* pAddMsg,float SkillTreeAmp)
{
	if(pOperator->GetSingleSpecialState(m_State))
	{// 현재 상태와 같은것을 썼다면 상태 종료다.
		pOperator->SetSingleSpecialStateUsedTime(m_State, gCurTime);
		pOperator->SetSingleSpecialState(m_State, false);
		SendAck(pOperator, m_State, false);
	}
	else
	{
		SPECIAL_STATE_INFO* pInfo = SKILLMGR->GetSpecialStateInfo(m_State);

		if(!pInfo)
			return SendNack(pOperator, m_State, eSingleSpecialState_Error);

		// 내력 확인
		if(pOperator->GetNaeRyuk() < pInfo->NaeRyuk)
			return SendNack(pOperator, m_State, eSingleSpecialState_Error_NaeRyuk);

		// 사용딜레이 확인
		if(pOperator->GetSingleSpecialStateUsedTime(m_State) + pInfo->DelayTime > gCurTime)
			return SendNack(pOperator, m_State, eSingleSpecialState_Error_Delay);

		pOperator->SetSingleSpecialState(m_State, true);
		SendAck(pOperator, m_State, true);
	}

	return 0;
}
#endif

#ifdef _MHCLIENT_
void CSkillObjectFirstUnit_SingleSpecialState::StartFirstEffect(CObject* pOperator,CTargetList* pTList,MAINTARGET* pMainTarget)
{
	CSkillObjectFirstUnit::StartFirstEffect(pOperator,pTList,pMainTarget);
}
#endif

#ifdef _MAPSERVER_
int CSkillObjectFirstUnit_SingleSpecialState::SendAck(CObject* pOperator, WORD State, bool bVal)
{
	pOperator->SendSingleSpeicalStateAck(State, bVal);
	return 0;
}

int CSkillObjectFirstUnit_SingleSpecialState::SendNack(CObject* pOperator, WORD State, WORD Err)
{
	pOperator->SendSingleSpeicalStateNack(State, Err);
	return 0;
}
#endif