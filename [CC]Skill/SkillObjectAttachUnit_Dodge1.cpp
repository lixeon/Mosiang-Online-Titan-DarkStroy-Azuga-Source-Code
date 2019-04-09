// SkillObjectAttachUnit_Dodge1.cpp: implementation of the CSkillObjectAttachUnit_Dodge class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObjectAttachUnit_Dodge1.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillObjectAttachUnit_Dodge::CSkillObjectAttachUnit_Dodge()
{
	m_nAttachUnitKind = eATTACH_UNIT_KIND_DODGE;
}

CSkillObjectAttachUnit_Dodge::~CSkillObjectAttachUnit_Dodge()
{

}

void CSkillObjectAttachUnit_Dodge::Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper/* =NULL */)
{
	m_fDodgeRate = pSkillInfo->GetCounterDodgeRate(SkillLevel);

#ifdef _MHCLIENT_
	//지속효과표시
	m_AttachEffectNum[0] = 0;
//	671	물리 공격력 증가		물리 공격 지속 >0
		
	m_StateIconNum[0] = 0;
#endif
}

void CSkillObjectAttachUnit_Dodge::GetDodgeRate(float Original,float& CalcedUp,float& CalcedDown)
{
	if(CalcedUp < m_fDodgeRate)
		CalcedUp = m_fDodgeRate;
}
