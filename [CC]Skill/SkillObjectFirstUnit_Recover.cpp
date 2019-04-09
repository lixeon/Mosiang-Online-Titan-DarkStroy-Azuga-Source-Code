// SkillObjectFirstUnit_Recover.cpp: implementation of the CSkillObjectFirstUnit_Recover class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObjectFirstUnit_Recover.h"
#include "SkillInfo.h"

#ifdef _MAPSERVER_
#include "AttackManager.h"
#include "skillmanager_server.h"
#include "TacticManager.h"
#endif

#ifdef _MHCLIENT_
#include "ObjectActionManager.h"
#include "ObjectManager.h"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillObjectFirstUnit_Recover::CSkillObjectFirstUnit_Recover(SKILLINFO* pSkillInfo)
: CSkillObjectFirstUnit(pSkillInfo)
{
	m_Attrib = pSkillInfo->Attrib;
}

CSkillObjectFirstUnit_Recover::~CSkillObjectFirstUnit_Recover()
{

}

void CSkillObjectFirstUnit_Recover::Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pPlayer /* = NULL  */)
{
	m_RecoverLife = pSkillInfo->GetSkillInfo()->FirstRecoverLife[SkillLevel-1];
	m_RecoverNaeRyuk = pSkillInfo->GetSkillInfo()->FirstRecoverNaeRyuk[SkillLevel-1];

#ifdef _MAPSERVER_
	if( pPlayer )
	if( pPlayer->GetObjectKind() == eObjectKind_Player )
	if( pSkillInfo->GetSkillKind() == SKILLKIND_JINBUB )
	{
		m_RecoverLife = 0;
		m_RecoverNaeRyuk = 0;
		TACTICMGR->GetRecoverValue( (CPlayer*)pPlayer, pSkillInfo->GetSkillIndex(), m_RecoverLife, m_RecoverNaeRyuk );
	}
#endif

	CSkillObjectFirstUnit::Init(pSkillInfo,SkillLevel);
}

#ifdef _MAPSERVER_

int CSkillObjectFirstUnit_Recover::ExcuteFirstUnit(CObject* pOperator,CSkillObjectTargetList* pTargetList,
												   MSG_SKILLOBJECT_ADD* pAddMsg,float SkillTreeAmp)
{
	CTargetListIterator iter(&pAddMsg->TargetList);
	STLIST* pList;
	RESULTINFO resultinfo;

	int nCount = 0;
	
	CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(pAddMsg->SkillObjectInfo.SkillIdx);
//	if(pSkillInfo->GetSkillKind() == SKILLKIND_JINBUB)
//	{
//		TACTICMGR->GetRecoverValue((CPlayer*)pOperator, pAddMsg->SkillObjectInfo.SkillIdx, m_RecoverLife, m_RecoverNaeRyuk);
//	}

	pTargetList->SetPositionHead();
	while(pList = pTargetList->GetNextTargetList())
	{
		resultinfo.Clear();
		
		if(pList->bTargetKind & SKILLRESULTKIND_POSITIVE)
		{
			ATTACKMGR->RecoverLife(pOperator,pList->pObject,m_Attrib,(DWORD)(m_RecoverLife*SkillTreeAmp),&resultinfo);
			ATTACKMGR->RecoverNaeRyuk(pOperator,pList->pObject,m_Attrib,(DWORD)(m_RecoverNaeRyuk*SkillTreeAmp),&resultinfo);

			++nCount;
		}
		
		DefaultFirstEffect(pOperator,pList,&resultinfo);

		iter.AddTargetWithResultInfo(pList->pObject->GetID(),pList->bTargetKind,&resultinfo);
	}
	iter.Release();
	if(pOperator->GetObjectKind() == eObjectKind_Player)
	{
		if(pSkillInfo->GetSkillKind() == SKILLKIND_JINBUB)
			TACTICMGR->AddGarbageTactic((CPlayer*)pOperator);
	}
//TEMP!
//	char buf[128];
//	wsprintf( buf, "none %d", nCount );
//	g_Console.LOG( 4, buf );

	return nCount;
}

#endif



#ifdef _MHCLIENT_

void CSkillObjectFirstUnit_Recover::StartFirstEffect(CObject* pOperator,CTargetList* pTList,MAINTARGET* pMainTarget)
{
	DWORD EffectFlag = 0;
	if(pOperator->GetID() == HEROID)
		EffectFlag |= EFFECT_FLAG_HEROATTACK;
	if(m_pSkillInfoData->FatalDamage)
		EffectFlag |= EFFECT_FLAG_FATALDAMAGE;
	
	m_hEff = EFFECTMGR->StartEffectProcessWithTargetList(m_pSkillInfoData->EffectUse,pOperator,pTList,pMainTarget,EffectFlag);

	ASSERTMSG(m_hEff,"무공 시작 이펙트에 오류가 있습니다.");
	if(pTList)		// 회복은 이펙트에서 처리 안해주기 때문에 무조건 해야 한다.
	{
		CTargetListIterator iter(pTList);
		CObject* pObject;
		RESULTINFO rinfo;
		while(iter.GetNextTarget())
		{
			pObject = OBJECTMGR->GetObject(iter.GetTargetID());
			iter.GetTargetData(&rinfo);
			if(pObject == NULL)
				continue;
	
			OBJECTACTIONMGR->ApplyResult(pObject,pOperator,&rinfo,eDamageKind_Front);
		}
		iter.Release();
	}
	
}

void CSkillObjectFirstUnit_Recover::SetTargetArray(CObject* pOperator,CTargetList* pTList,MAINTARGET* pMainTarget)
{	
	CTargetListIterator iter(pTList);
	CObject* pObject;
	RESULTINFO rinfo;
	while(iter.GetNextTarget())
	{
		pObject = OBJECTMGR->GetObject(iter.GetTargetID());
		iter.GetTargetData(&rinfo);
		if(pObject == NULL)
			continue;

		OBJECTACTIONMGR->ApplyResult(pObject,pOperator,&rinfo,eDamageKind_Front);
	}
	iter.Release();
	
	EFFECTMGR->SetTargetArray(m_hEff,pTList,pMainTarget);
}
#endif
