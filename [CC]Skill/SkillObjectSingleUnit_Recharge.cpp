// SkillObjectSingleUnit_Recharge.cpp: implementation of the CSkillObjectSingleUnit_Recharge class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObjectSingleUnit_Recharge.h"


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

CSkillObjectSingleUnit_Recharge::CSkillObjectSingleUnit_Recharge(DWORD SingleUnitNum)
: CSkillObjectSingleUnit(SingleUnitNum)
{
	m_PNTarget = SKILLRESULTKIND_POSITIVE;
}

CSkillObjectSingleUnit_Recharge::~CSkillObjectSingleUnit_Recharge()
{

}

void CSkillObjectSingleUnit_Recharge::DoInit(CSkillInfo* pSkillInfo,WORD SkillLevel)
{
	m_Attrib = pSkillInfo->GetAttrib();
	m_Recharge = pSkillInfo->GetContinueRecoverNaeRyuk(SkillLevel);
}

void CSkillObjectSingleUnit_Recharge::Operate(SKILLOBJECT_INFO* pSkillObjectInfo,
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
		ATTACKMGR->RecoverLife(pOperator,pObject,(WORD)m_Attrib,(DWORD)m_Recharge,&resultinfo);
#else
		resultinfo.Clear();
		resultinfo.RechargeNaeryuk = 1;
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
void CSkillObjectSingleUnit_Recharge::StartSingleUnitEffect(CObject* pOperator,CTargetList* pTList)
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
		//OBJECTACTIONMGR->Recharge(pObject,pOperator,0,rinfo.RechargeNaeryuk);
	}
	iter.Release();
}

#endif