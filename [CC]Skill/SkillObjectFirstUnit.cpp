// SkillObjectFirstUnit.cpp: implementation of the CSkillObjectFirstUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObjectFirstUnit.h"
#include "Object.h"
#include "SkillInfo.h"
#include "..\[CC]Header\CommonCalcFunc.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillObjectFirstUnit::CSkillObjectFirstUnit(SKILLINFO* pSkillInfo)
{
#ifdef _MHCLIENT_
	m_pSkillInfoData = pSkillInfo;
	m_hEff = 0;
#endif
}

CSkillObjectFirstUnit::~CSkillObjectFirstUnit()
{

}

void CSkillObjectFirstUnit::Init( CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pPlayer /* = NULL  */)
{
	m_Attrib = pSkillInfo->GetAttrib();
	m_RecoverStateAbnormal = pSkillInfo->GetSkillInfo()->RecoverStateAbnormal[SkillLevel-1];
	m_StunRate = pSkillInfo->GetStunRate(SkillLevel);
	m_StunTime = pSkillInfo->GetStunTime(SkillLevel);
	m_DispelAttackFeelRate = pSkillInfo->GetDispelAttackFeelRate(SkillLevel);
}

#ifdef _MAPSERVER_

#include "StateMachinen.h"
int CSkillObjectFirstUnit::ExcuteFirstUnit(CObject* pOperator,CSkillObjectTargetList* pTargetList,
												   MSG_SKILLOBJECT_ADD* pAddMsg,float SkillTreeAmp)
{
	STLIST* pList;
	CTargetListIterator iter(&pAddMsg->TargetList);
	pTargetList->SetPositionHead();
	RESULTINFO rinfo;
	
	int nCount = 0;

	while(pList = pTargetList->GetNextTargetList())
	{
		rinfo.Clear();
		
		DefaultFirstEffect(pOperator,pList,&rinfo);

		iter.AddTargetWithResultInfo(pList->pObject->GetID(),pList->bTargetKind,&rinfo);

		++nCount;
	}
	iter.Release();
//TEMP!
//	char buf[128];
//	wsprintf( buf, "none %d", nCount );
//	g_Console.LOG( 4, buf );


	return nCount;
}

void CSkillObjectFirstUnit::DefaultFirstEffect(CObject* pOperator,STLIST* pTarget,RESULTINFO* pRInfo)
{	
	if(m_RecoverStateAbnormal)
		pRInfo->bDispelFromEvil = CheckRandom(m_RecoverStateAbnormal,pOperator->GetLevel(),pTarget->pObject->GetLevel());
	
	// Stun
#ifdef _JAPAN_LOCAL_
	if( CheckSpecialAttack( m_StunRate, pOperator, pTarget->pObject ) == TRUE )
	{
		pRInfo->StunTime = m_StunTime;
		pTarget->pObject->StartSpecialState(eSpecialState_Stun,m_StunTime,0,0,0,FindEffectNum("monster_stun_s.beff"),0);
	}
#else
	if( m_StunRate )
	{
		if(CheckRandom(m_StunRate,pOperator->GetLevel(),pTarget->pObject->GetLevel()) == TRUE)
		{
			pRInfo->StunTime = m_StunTime;
			pTarget->pObject->StartSpecialState(eSpecialState_Stun,m_StunTime,0,0,0,FindEffectNum("monster_stun_s.beff"),0);
		}
	}
#endif
	
	// 공격의욕 상실
	if((pTarget->pObject->GetObjectKind() & eObjectKind_Monster) &&
		m_DispelAttackFeelRate != 0)
	{
		if(CheckRandom(m_DispelAttackFeelRate,pOperator->GetLevel(),pTarget->pObject->GetLevel()) == TRUE)
			GSTATEMACHINE.SetState(pTarget->pObject,eMA_STAND);
	}
	
}

void CSkillObjectFirstUnit::Release()
{
}

#else

#include "ObjectManager.h"
#include "ObjectActionManager.h"

void CSkillObjectFirstUnit::StartFirstEffect(CObject* pOperator,CTargetList* pTList,MAINTARGET* pMainTarget)
{
	DWORD EffectFlag = 0;
	if(pOperator->GetID() == HEROID)
		EffectFlag |= EFFECT_FLAG_HEROATTACK;
	if(m_pSkillInfoData->FatalDamage)
		EffectFlag |= EFFECT_FLAG_FATALDAMAGE;
	
	m_hEff = EFFECTMGR->StartEffectProcessWithTargetList(m_pSkillInfoData->EffectUse,pOperator,pTList,pMainTarget,EffectFlag);

	ASSERTMSG(m_hEff,"무공 시작 이펙트에 오류가 있습니다.");
	if(m_hEff == NULL && pTList)
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

void CSkillObjectFirstUnit::SetTargetArray(CObject* pOperator,CTargetList* pTList,MAINTARGET* pMainTarget)
{
	if(m_hEff)
	{
		if(EFFECTMGR->SetTargetArray(m_hEff,pTList,pMainTarget) == FALSE)
			m_hEff = NULL;
	}

	if(m_hEff == NULL)
	{
		OBJECTACTIONMGR->ApplyTargetList(pOperator,pTList,eDamageKind_Front);
	}
}

void CSkillObjectFirstUnit::Release()
{
	if(m_hEff)
		EFFECTMGR->SetEffectEndFlag(m_hEff);
	m_hEff = NULL;
}

#endif
