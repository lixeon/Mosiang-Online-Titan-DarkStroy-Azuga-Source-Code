// SkillObjectStateUnit_DummyState.cpp: implementation of the CSkillObjectStateUnit_DummyState class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObjectStateUnit_DummyState.h"

#ifdef _MHCLIENT_
#include "StatusIconDlg.h"
#include "SkillObject_client.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillObjectStateUnit_DummyState::CSkillObjectStateUnit_DummyState()
{
	m_SkillTime = 0;
}

CSkillObjectStateUnit_DummyState::~CSkillObjectStateUnit_DummyState()
{

}

void CSkillObjectStateUnit_DummyState::Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper/* =NULL */)
{
	m_SkillTime = pSkillInfo->GetSkillContinueTime() + pSkillInfo->GetSkillInfo()->SkillAdditionalTime[SkillLevel-1];

	if(pSkillInfo->GetContinueAttAttack(12) != 0)
	{
#ifdef _MHCLIENT_
/*
maintain_damage01_S.beff	화속성 지속데미지
maintain_damage02_S.beff	수속성 지속데미지
maintain_damage03_S.beff	금속성 지속데미지
maintain_gas_S.beff	독속성 지속데미지
*/
		m_StateEffectNum[0] = pSkillInfo->GetSkillInfo()->EffectMineOperate;
		if(pSkillInfo->GetAttrib() == ATTR_FIRE)
		{
			m_StateIconNum[0] = eStatusIcon_FireContinueDamage;
		}
		else if(pSkillInfo->GetAttrib() == ATTR_WATER)
		{
			m_StateIconNum[0] = eStatusIcon_WaterContinueDamage;
		}
		else if(pSkillInfo->GetAttrib() == ATTR_IRON)
		{
			m_StateIconNum[0] = eStatusIcon_IronContinueDamage;
		}
		else if(pSkillInfo->GetAttrib() == ATTR_NOATTR)
		{
			m_StateIconNum[0] = eStatusIcon_PoisonContinueDamage;
		}
		else
		{
			m_StateIconNum[0] = eStatusIcon_PoisonContinueDamage;
		}
#endif
		
		m_PNTarget = SKILLRESULTKIND_NEGATIVE;
	}
	if(pSkillInfo->GetContinueRecoverLife(12) != 0)
	{
#ifdef _MHCLIENT_
		m_StateIconNum[1] = eStatusIcon_HealContinue;
		// 지속효과표시
		m_StateEffectNum[1] = pSkillInfo->GetSkillInfo()->EffectMineOperate;
#endif
		
		m_PNTarget = SKILLRESULTKIND_POSITIVE;
	}

}

#ifdef _MHCLIENT_
#include "StatusIconDlg.h"
#endif

void CSkillObjectStateUnit_DummyState::DoStartState(CObject* pObject, CObject* pSkillOperator)
{
#ifdef _MHCLIENT_
	OBJECTEFFECTDESC desc[MAX_SKILLMAINTAIN_EFFECT];
	for(int n=0;n<MAX_SKILLMAINTAIN_EFFECT;++n)
	{
		if(m_StateIconNum[n])
		{
			if( m_pSkillObject )
				STATUSICONDLG->AddIcon(pObject,m_StateIconNum[n],0,m_SkillTime);
			else
				STATUSICONDLG->AddIcon(pObject,m_StateIconNum[n]);
		}

		
		desc[n].SetDesc(m_StateEffectNum[n]);
	}
	
	pObject->AddObjectEffect(SKILLOBJECT_ID_START+m_pSkillObject->GetSkillIdx(),desc,MAX_SKILLMAINTAIN_EFFECT, m_pSkillObject->GetOperator());
#endif
}

void CSkillObjectStateUnit_DummyState::DoEndState(CObject* pObject)
{
#ifdef _MHCLIENT_
	for(int n=0;n<MAX_SKILLMAINTAIN_EFFECT;++n)
	{
		if(m_StateIconNum[n])
			STATUSICONDLG->RemoveIcon(pObject,m_StateIconNum[n]);
		
	}
	
	pObject->RemoveObjectEffect(SKILLOBJECT_ID_START+m_pSkillObject->GetSkillIdx());
#endif
}
