// SkillObjectStateUnit_TieUp.cpp: implementation of the CSkillObjectStateUnit_TieUp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObjectStateUnit_TieUp.h"
#include "ObjectStateManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillObjectStateUnit_TieUp::CSkillObjectStateUnit_TieUp()
{
	m_SkillTime = 0;
}

CSkillObjectStateUnit_TieUp::~CSkillObjectStateUnit_TieUp()
{

}

void CSkillObjectStateUnit_TieUp::Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper/* =NULL */)
{
	ASSERT(pSkillInfo->GetTieUpType() != SKILLTIEUP_TYPE_NOTHING);
	m_TieUpType = pSkillInfo->GetTieUpType();
	m_PNTarget = SKILLRESULTKIND_NEGATIVE;

	m_SkillTime = pSkillInfo->GetSkillContinueTime() + pSkillInfo->GetSkillInfo()->SkillAdditionalTime[SkillLevel-1];

#ifdef _MHCLIENT_
	//지속효과표시
	if(m_TieUpType == 2)
	{
		m_StateEffectNum[0] = pSkillInfo->GetSkillInfo()->EffectMineOperate;
		m_StateIconNum[0] = eStatusIcon_TieUpCanMove;
	}
	else if(m_TieUpType == 1)
	{
		m_StateEffectNum[0] = pSkillInfo->GetSkillInfo()->EffectMineOperate;
		m_StateIconNum[0] = eStatusIcon_TieUpCanAttack;
	}
	else if(m_TieUpType == 3)
	{
		m_StateEffectNum[0] = pSkillInfo->GetSkillInfo()->EffectMineOperate;
		m_StateIconNum[0] = eStatusIcon_TieUpAll;
	}
/*
659	결박 (공격)		결박 (2: 공격)
662	결박 (이동)		결박 (1: 이동)
665	결박 (전부)		결박 (3: 전부)
*/
#endif
}

#ifdef _MAPSERVER_

void CSkillObjectStateUnit_TieUp::DoStartState(CObject* pObject, CObject* pSkillOperator)
{
	if(m_TieUpType == SKILLTIEUP_TYPE_CANSKILL)
		OBJECTSTATEMGR_OBJ->StartObjectState(pObject,eObjectState_TiedUp_CanSkill,0);
	else if(m_TieUpType == SKILLTIEUP_TYPE_CANMOVE)
		OBJECTSTATEMGR_OBJ->StartObjectState(pObject,eObjectState_TiedUp_CanMove,0);
	else if(m_TieUpType == SKILLTIEUP_TYPE_TIEDALL)
		OBJECTSTATEMGR_OBJ->StartObjectState(pObject,eObjectState_TiedUp,0);
}

void CSkillObjectStateUnit_TieUp::DoEndState(CObject* pObject)
{	
	if(m_TieUpType == SKILLTIEUP_TYPE_CANSKILL)
		OBJECTSTATEMGR_OBJ->EndObjectState(pObject,eObjectState_TiedUp_CanSkill,0);
	else if(m_TieUpType == SKILLTIEUP_TYPE_CANMOVE)
		OBJECTSTATEMGR_OBJ->EndObjectState(pObject,eObjectState_TiedUp_CanMove,0);
	else if(m_TieUpType == SKILLTIEUP_TYPE_TIEDALL)
		OBJECTSTATEMGR_OBJ->EndObjectState(pObject,eObjectState_TiedUp,0);
}

#else

#include "EFFECT/EffectManager.h"
#include "Object.h"
#include "SkillObject_client.h"
#include "StatusIconDlg.h"

void CSkillObjectStateUnit_TieUp::DoStartState(CObject* pObject, CObject* pSkillOperator)
{
	OBJECTEFFECTDESC desc[MAX_SKILLMAINTAIN_EFFECT];
	for(int n=0;n<MAX_SKILLMAINTAIN_EFFECT;++n)
	{
		desc[n].SetDesc(m_StateEffectNum[n]);
		if(m_StateIconNum[n])
		{
			if( m_pSkillObject )
				STATUSICONDLG->AddIcon(pObject,m_StateIconNum[n],0,m_SkillTime);
			else
				STATUSICONDLG->AddIcon(pObject,m_StateIconNum[n]);
		}
	}

	pObject->AddObjectEffect(SKILLOBJECT_ID_START+m_pSkillObject->GetSkillIdx(),desc,MAX_SKILLMAINTAIN_EFFECT);

	if(m_TieUpType == SKILLTIEUP_TYPE_CANSKILL)
	{
		OBJECTSTATEMGR->StartObjectState(pObject,eObjectState_TiedUp_CanSkill);
	}
	else if(m_TieUpType == SKILLTIEUP_TYPE_CANMOVE)
	{
		OBJECTSTATEMGR->StartObjectState(pObject,eObjectState_TiedUp_CanMove);
	}
	else if(m_TieUpType == SKILLTIEUP_TYPE_TIEDALL)
	{
		OBJECTSTATEMGR->StartObjectState(pObject,eObjectState_TiedUp);
	}
}

void CSkillObjectStateUnit_TieUp::DoEndState(CObject* pObject)
{
	pObject->RemoveObjectEffect(SKILLOBJECT_ID_START+m_pSkillObject->GetSkillIdx());
	for(int n=0;n<MAX_SKILLMAINTAIN_EFFECT;++n)
	{
		if(m_StateIconNum[n])
			STATUSICONDLG->RemoveIcon(pObject,m_StateIconNum[n]);
	}

	if(m_TieUpType == SKILLTIEUP_TYPE_CANSKILL)
		OBJECTSTATEMGR->EndObjectState(pObject,eObjectState_TiedUp_CanSkill);
	else if(m_TieUpType == SKILLTIEUP_TYPE_CANMOVE)
		OBJECTSTATEMGR->EndObjectState(pObject,eObjectState_TiedUp_CanMove);
	else if(m_TieUpType == SKILLTIEUP_TYPE_TIEDALL)
		OBJECTSTATEMGR->EndObjectState(pObject,eObjectState_TiedUp);
}

#endif
