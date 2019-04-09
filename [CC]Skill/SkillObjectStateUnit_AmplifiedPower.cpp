// SkillObjectStateUnit_AmplifiedPower.cpp: implementation of the CSkillObjectStateUnit_AmplifiedPower class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObjectStateUnit_AmplifiedPower.h"
#include "Object.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillObjectStateUnit_AmplifiedPower::CSkillObjectStateUnit_AmplifiedPower()
{

}

CSkillObjectStateUnit_AmplifiedPower::~CSkillObjectStateUnit_AmplifiedPower()
{

}


void CSkillObjectStateUnit_AmplifiedPower::Init( CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper/* =NULL */)
{
	m_SkillTime = pSkillInfo->GetSkillContinueTime() 
				+ pSkillInfo->GetSkillInfo()->SkillAdditionalTime[SkillLevel-1];
	m_Effect = pSkillInfo->GetSkillInfo()->EffectMineOperate;
	m_AmplifiedPowerPhy = pSkillInfo->GetAmplifiedPowerPhy(SkillLevel);
	m_AmplifiedPowerAtt = pSkillInfo->GetAmplifiedPowerAtt(SkillLevel);
	m_AmplifiedPowerAttRate = pSkillInfo->GetAmplifiedPowerAttRate(SkillLevel);
	m_Attrib = pSkillInfo->GetAttrib();

	m_PNTarget = SKILLRESULTKIND_POSITIVE;
	
#ifdef _MHCLIENT_
	if(m_AmplifiedPowerAtt)
	{
		m_StateIconNum[0] = eStatusIcon_AttrCounterFire + m_Attrib-1;
	}
#endif
}

#ifdef _MHCLIENT_
#include "StatusIconDlg.h"
#endif

void CSkillObjectStateUnit_AmplifiedPower::DoStartState(CObject* pObject, CObject* pSkillOperator)
{
	if(m_AmplifiedPowerPhy)
		pObject->StartSpecialState(eSpecialState_AmplifiedPowerPhy,
				m_SkillTime,m_AmplifiedPowerPhy,0,0,m_Effect,eStatusIcon_AmpliedPower);

	if(m_AmplifiedPowerAtt)
		pObject->StartSpecialState(eSpecialState_AmplifiedPowerAtt,
				m_SkillTime,m_AmplifiedPowerAtt,m_Attrib,m_AmplifiedPowerAttRate,m_Effect,0);
	
#ifdef _MHCLIENT_
	if(m_StateIconNum[0])
		STATUSICONDLG->AddIcon(pObject,m_StateIconNum[0],0,m_SkillTime);
#endif
}

void CSkillObjectStateUnit_AmplifiedPower::DoEndState(CObject* pObject)
{
	if(m_AmplifiedPowerPhy)
		pObject->EndSpecialState(eSpecialState_AmplifiedPowerPhy);

	// YH 속성 증폭이 중복 되었을때의 문제 때문에 주석처리
	// DISPEL 같은 기능이 들어갔을시에 문제 소지 있음
//	if(m_AmplifiedPowerAtt)
//		pObject->EndSpecialState(eSpecialState_AmplifiedPowerAtt);
	
#ifdef _MHCLIENT_
	if(m_StateIconNum[0])
		STATUSICONDLG->RemoveIcon(pObject,m_StateIconNum[0]);
#endif
}
