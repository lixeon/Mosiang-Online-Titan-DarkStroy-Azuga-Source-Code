#include "stdafx.h"
#include "SkillObjectFirstUnit_Job.h"
#include "SkillInfo.h"
#include "..\[CC]Header\CommonCalcFunc.h"
#include "Object.h"

#ifdef _MAPSERVER_
#include "TacticManager.h"
#include "AttackManager.h"
#include "StateMachinen.h"
#include "SkillObject_Server.h"
#include "SkillManager_server.h"
#include "UserTable.h"
#include "Battle.h"
#include "ItemManager.h"
#include "MHTimeManager.h"
#include "ItemDrop.h"
#endif

CSkillObjectFirstUnit_Job::CSkillObjectFirstUnit_Job(SKILLINFO* pSkillInfo)
: CSkillObjectFirstUnit(pSkillInfo)
{
#ifdef _MAPSERVER_
	m_pSkillInfo = NULL;
#endif
}

CSkillObjectFirstUnit_Job::~CSkillObjectFirstUnit_Job()
{

}

void CSkillObjectFirstUnit_Job::Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper)
{
#ifdef _MAPSERVER_
	m_pSkillInfo = pSkillInfo;	
	m_bJobSkillSuccess = FALSE;
	m_bJobSkillFail = FALSE;
	m_pPlayer = NULL;
	m_pMonster = NULL;
#endif
	CSkillObjectFirstUnit::Init(pSkillInfo,SkillLevel);
}

#ifdef _MAPSERVER_
int CSkillObjectFirstUnit_Job::ExcuteFirstUnit(CObject* pOperator,CSkillObjectTargetList* pTargetList,
							MSG_SKILLOBJECT_ADD* pAddMsg,float SkillTreeAmp)
{
	CTargetListIterator iter(&pAddMsg->TargetList);
	STLIST* pList;
	RESULTINFO DamageInfo;
	int nCount = 0;

	CPlayer* pPlayer = (CPlayer*)pOperator;
	WORD wSkillLevel = pPlayer->GetMugongLevel(m_pSkillInfo->GetSkillIndex());	
	
	pTargetList->SetPositionHead();
	while(pList = pTargetList->GetNextTargetList())
	{
		DamageInfo.Clear();

		CObject* pObject = g_pUserTable->FindUser(pList->pObject->GetID());
		if(pObject == NULL)	//예외처리
		{
			continue;
		}
		CMonster* pMonster = (CMonster*)pObject;		
		WORD wProbability = 100 - SKILLMGR->GetJobSkillProbability(wSkillLevel, pMonster->GetLevel());
		WORD wResultProbability = rand() % 100;

		if(wResultProbability >= wProbability) //전문스킬 성공
		{
			ATTACKMGR->Attack(TRUE,	pOperator,pList->pObject, 0, 0,	0, 0, 0, 0,	0,	&DamageInfo, TRUE, 0, 0);
			++nCount;

			DefaultFirstEffect(pOperator,pList,&DamageInfo);
			iter.AddTargetWithResultInfo(pList->pObject->GetID(),pList->bTargetKind,&DamageInfo);
	
			m_bJobSkillSuccess = TRUE;
			m_pPlayer = pPlayer;
			m_pMonster = pMonster;			
		}
		else	//전문스킬 실패
		{
			m_bJobSkillFail = TRUE;
			m_pPlayer = pPlayer;
		}
	}	
	iter.Release();

	m_pSkillInfo = NULL;

	return nCount;
}

void CSkillObjectFirstUnit_Job::FirstUnitResult()
{
	// 전문스킬이 성공하면 아이탬을 드롭한다.
	if(m_bJobSkillSuccess == TRUE)
	{
		if( (m_pMonster != NULL) && (m_pPlayer != NULL) ) //예외처리
		{
			WORD wMonsterKind = m_pMonster->GetMonsterKind();
			ITEMDROP_OBJ->MoneyItemNoItemPercentCalculator((MONSTEREX_LIST*)m_pMonster->GetSMonsterList(), m_pPlayer, wMonsterKind);
			m_bJobSkillSuccess = FALSE;
			m_pPlayer = NULL;
			m_pMonster = NULL;
		}
	}
	
	if(m_bJobSkillFail == TRUE)
	{
		if(m_pPlayer != NULL)
		{
			MSGBASE msg;
			msg.Category = MP_SKILL;
			msg.Protocol = MP_SKILL_JOB_NACK;			
			m_pPlayer->SendMsg(&msg,sizeof(msg));

			m_bJobSkillFail = FALSE;
			m_pPlayer = NULL;
		}
	}
}

#endif

#ifdef _MHCLIENT_
void CSkillObjectFirstUnit_Job::StartFirstEffect(CObject* pOperator,CTargetList* pTList,MAINTARGET* pMainTarget)
{
	CSkillObjectFirstUnit::StartFirstEffect(pOperator,pTList,pMainTarget);

	// for delete Effect
	pOperator->GetSpecialStateParam1(eSpecialState_AmplifiedPowerPhy);
}
#endif