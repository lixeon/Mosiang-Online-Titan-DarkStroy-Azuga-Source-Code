// SkillObjectSingleUnit_Heal.cpp: implementation of the CSkillObjectSingleUnit_Heal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObjectSingleUnit_Heal.h"

#ifdef _MAPSERVER_
#include "PackedData.h"
#include "AttackManager.h"
#include "SkillObject_server.h"
#else
#include "ObjectManager.h"
#include "ObjectActionManager.h"
#include "Gamein.h"
#include "SkillObject_Client.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillObjectSingleUnit_Heal::CSkillObjectSingleUnit_Heal(DWORD SingleUnitNum)
: CSkillObjectSingleUnit(SingleUnitNum)
{
	m_PNTarget = SKILLRESULTKIND_POSITIVE;
}

CSkillObjectSingleUnit_Heal::~CSkillObjectSingleUnit_Heal()
{

}

void CSkillObjectSingleUnit_Heal::DoInit(CSkillInfo* pSkillInfo,WORD SkillLevel)
{	
	m_Heal = pSkillInfo->GetContinueRecoverLife(SkillLevel);
	m_Attrib = pSkillInfo->GetAttrib();
}

void CSkillObjectSingleUnit_Heal::Operate(SKILLOBJECT_INFO* pSkillObjectInfo,
										  CSkillObjectTargetList* pTargetList,float SkillTreeAmp)
{
	CObject* pOperator = m_pSkillObject->GetOperator();
	MSG_SKILL_SINGLE_RESULT msg;
	msg.InitMsg(pSkillObjectInfo->SkillObjectIdx,GetSingleUnitNum());
	CTargetListIterator iter(&msg.TargetList);
	
	CObject* pObject;
	BYTE bTargetKind;
	RESULTINFO resultinfo;
	pTargetList->SetPositionHead();
	while(pTargetList->GetNextTarget(m_PNTarget,&pObject,&bTargetKind))
	{		
#ifdef _MAPSERVER_
		ATTACKMGR->RecoverLife(pOperator,pObject,(WORD)m_Attrib,(DWORD)m_Heal,&resultinfo);
#else
		resultinfo.Clear();
		resultinfo.HealLife = 1;
#endif

		iter.AddTargetWithResultInfo(pObject->GetID(),bTargetKind,&resultinfo);
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
void CSkillObjectSingleUnit_Heal::StartSingleUnitEffect(CObject* pOperator,CTargetList* pTList)
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
		
		OBJECTACTIONMGR->ApplyResult(pObject,pOperator,&rinfo,eDamageKind_ContinueDamage);
		//OBJECTACTIONMGR->Heal(pObject,pOperator,0,rinfo.HealLife);
	}
	iter.Release();
}

#endif