// SkillObjectSingleUnit_Attack.cpp: implementation of the CSkillObjectSingleUnit_Attack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObjectSingleUnit_Attack.h"

#ifdef _MAPSERVER_
#include "PackedData.h"
#include "AttackManager.h"
#include "SkillObject_server.h"
#include "SkillManager_server.h"
#include "Battle.h"
#else
#include "ObjectManager.h"
#include "ObjectActionManager.h"
#include "Gamein.h"
#include "SkillObject_Client.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillObjectSingleUnit_Attack::CSkillObjectSingleUnit_Attack(DWORD SingleUnitNum)
: CSkillObjectSingleUnit(SingleUnitNum)
{
	m_PNTarget = SKILLRESULTKIND_NEGATIVE;
}

CSkillObjectSingleUnit_Attack::~CSkillObjectSingleUnit_Attack()
{

}

void CSkillObjectSingleUnit_Attack::DoInit(CSkillInfo* pSkillInfo,WORD SkillLevel)
{
	m_PhysicalAttack = 0;
	m_AttribAttack = pSkillInfo->GetContinueAttAttack(SkillLevel);
	m_Attrib = pSkillInfo->GetAttrib();
	m_AttAttackRate = pSkillInfo->GetContinueAttAttackRate(SkillLevel);
}

void CSkillObjectSingleUnit_Attack::Operate(SKILLOBJECT_INFO* pSkillObjectInfo,
											CSkillObjectTargetList* pTargetList,float SkillTreeAmp)
{
	CObject* pOperator = m_pSkillObject->GetOperator();
	MSG_SKILL_SINGLE_RESULT msg;
	msg.InitMsg(pSkillObjectInfo->SkillObjectIdx,GetSingleUnitNum());

	CTargetListIterator iter(&msg.TargetList);
#ifdef _MAPSERVER_	
	//////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환 추가
	// 스킬공격력
	if(pSkillObjectInfo->Option)
	{
		SKILLOPTION* pSkillOption = SKILLMGR->GetSkillOption(pSkillObjectInfo->Option);

		if(pSkillOption)
		{
			if(pSkillOption->PhyAtk)
				m_PhysicalAttack = m_PhysicalAttack * (1.0f + pSkillOption->PhyAtk);
			if(pSkillOption->AttAtk)
				m_AttAttackRate = m_AttAttackRate * (1.0f + pSkillOption->AttAtk);
		}
	}
	//////////////////////////////////////////////////////////////////////////
#endif
	CObject* pObject;
	BYTE bTargetKind;
	RESULTINFO damageinfo;
	pTargetList->SetPositionHead();
	while(pTargetList->GetNextTarget(m_PNTarget,&pObject,&bTargetKind))	// 공격은 Negative
	{
		if((bTargetKind & SKILLRESULTKIND_NEGATIVE) == FALSE)
			continue;
		
#ifdef _MAPSERVER_
		if(pOperator->GetBattle())
		if(pOperator->GetBattle()->IsEnemy(pOperator,pObject) == FALSE)
		{
			continue;
		}

		ATTACKMGR->Attack(TRUE, pOperator, pObject, 0, m_PhysicalAttack,	m_Attrib,
									m_AttribAttack, m_AttribAttack, m_AttAttackRate,
									0,
									&damageinfo,
									FALSE, 1.0f, 0, TRUE);	// FALSE 는 Single Unit은 카운터 데미지 없음
#else
		damageinfo.Clear();
		damageinfo.RealDamage = rand() % 100 + 1;
		damageinfo.ShieldDamage = rand() % 30;
		damageinfo.CounterDamage = 0;
#endif

		iter.AddTargetWithResultInfo(pObject->GetID(),bTargetKind,&damageinfo);
	}

	iter.Release();

#ifdef _MAPSERVER_
	PACKEDDATA_OBJ->QuickSend(pOperator,&msg,msg.GetMsgLength());
#else
	// for testclient
	GAMEIN->NetworkMsgParse(msg.Category,msg.Protocol,&msg);
#endif
}


#ifdef _MHCLIENT_
void CSkillObjectSingleUnit_Attack::StartSingleUnitEffect(CObject* pOperator,CTargetList* pTList)
{
	CObject* pObject;
	CTargetListIterator iter(pTList);
	RESULTINFO rinfo;
	while(iter.GetNextTarget())
	{
		pObject = OBJECTMGR->GetObject(iter.GetTargetID());
		if(pObject == NULL)
			continue;

		iter.GetTargetData(&rinfo);

		/*
		// 지속효과표시
		ySWITCH(m_Attrib)
			yCASE(ATTR_FIRE)	EFFECTMGR->StartSkillMainTainEffect(650,pObject);
			yCASE(ATTR_TREE)	EFFECTMGR->StartSkillMainTainEffect(653,pObject);
			yCASE(ATTR_NOATTR)	EFFECTMGR->StartSkillMainTainEffect(653,pObject);	// 독
		yENDSWITCH
		// 650	지속데미지 (화)		속성 데미지지속 값>0	속성 값이 1(화)				
		// 653	지속데미지 (목)		속성 데미지지속 값>0	속성 값이 3(목)				
		*/

		OBJECTACTIONMGR->ApplyResult(pObject,pOperator,&rinfo,eDamageKind_ContinueDamage);
		/*
		OBJECTACTIONMGR->Damage(pObject,pOperator,eDamageKind_Front,rinfo.RealDamage,rinfo.ShieldDamage,rinfo.bCritical);
		
		DWORD CounterDamage = rinfo.CounterDamage;
		if(CounterDamage != 0)
			OBJECTACTIONMGR->Damage(pOperator,pObject,eDamageKind_Counter,CounterDamage,0,FALSE);
		
		if(pObject->IsDied())		// 죽었다
		{
			OBJECTACTIONMGR->Die(pObject,pOperator,FALSE);	// 그냥 죽기
		}
		*/
	}
	iter.Release();
}
#endif
