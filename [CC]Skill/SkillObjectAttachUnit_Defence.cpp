// SkillObjectAttachUnit_Defence.cpp: implementation of the CSkillObjectAttachUnit_DefenceUp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObjectAttachUnit_Defence.h"

#ifdef _MHCLIENT_
#include "StatusIconDlg.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Up
//////////////////////////////////////////////////////////////////////////

CSkillObjectAttachUnit_DefenceUp::CSkillObjectAttachUnit_DefenceUp()
{
	m_Flag = STATUS_FLAG_DEFENCE;
}

CSkillObjectAttachUnit_DefenceUp::~CSkillObjectAttachUnit_DefenceUp()
{

}

void CSkillObjectAttachUnit_DefenceUp::Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper/* =NULL */)
{
	m_Attrib = pSkillInfo->GetAttrib();
	m_AttDefenceUp = pSkillInfo->GetUpAttDefense(SkillLevel);
	m_PhyDefenceUp = pSkillInfo->GetUpPhyDefense(SkillLevel);
	
#ifdef _MHCLIENT_
	//지속효과표시
	if(m_AttDefenceUp == 1)
	{
/*
불침지체	
maintain_Sresist01_S.beff	화속성 불침지체
maintain_Sresist02_S.beff	수속성 불침지체
maintain_Sresist03_S.beff	목속성 불침지체
maintain_Sresist04_S.beff	금속성 불침지체
maintain_Sresist05_S.beff	토속성 불침지체
*/
		ySWITCH(m_Attrib)
			yCASE(ATTR_FIRE)	m_AttachEffectNum[0] = pSkillInfo->GetSkillInfo()->EffectMineOperate;
								m_StateIconNum[0] = eStatusIcon_AttrDefenceFire;
			yCASE(ATTR_WATER)	m_AttachEffectNum[0] = pSkillInfo->GetSkillInfo()->EffectMineOperate;
								m_StateIconNum[0] = eStatusIcon_AttrDefenceWater;
			yCASE(ATTR_TREE)	m_AttachEffectNum[0] = pSkillInfo->GetSkillInfo()->EffectMineOperate;
								m_StateIconNum[0] = eStatusIcon_AttrDefenceTree;
			yCASE(ATTR_IRON)	m_AttachEffectNum[0] = pSkillInfo->GetSkillInfo()->EffectMineOperate;
								m_StateIconNum[0] = eStatusIcon_AttrDefenceIron;
			yCASE(ATTR_EARTH)	m_AttachEffectNum[0] = pSkillInfo->GetSkillInfo()->EffectMineOperate;
								m_StateIconNum[0] = eStatusIcon_AttrDefenceEarth;
		yENDSWITCH
	}
	else if(m_AttDefenceUp > 0)
	{
/*
저항력 상승	
maintain_resist01_S.beff	화속성 저항력 상승
maintain_resist02_S.beff	수속성 저항력 상승
maintain_resist03_S.beff	목속성 저항력 상승
maintain_resist04_S.beff	금속성 저항력 상승
maintain_resist05_S.beff	토속성 저항력 상승
maintain_resist06_S.beff	무속성 저항력 상승
*/
		ySWITCH(m_Attrib)
			yCASE(ATTR_FIRE)	m_AttachEffectNum[0] = pSkillInfo->GetSkillInfo()->EffectMineOperate;
								m_StateIconNum[0] = eStatusIcon_AttrDefenceFire;
			yCASE(ATTR_WATER)	m_AttachEffectNum[0] = pSkillInfo->GetSkillInfo()->EffectMineOperate;
								m_StateIconNum[0] = eStatusIcon_AttrDefenceWater;
			yCASE(ATTR_TREE)	m_AttachEffectNum[0] = pSkillInfo->GetSkillInfo()->EffectMineOperate;
								m_StateIconNum[0] = eStatusIcon_AttrDefenceTree;
			yCASE(ATTR_IRON)	m_AttachEffectNum[0] = pSkillInfo->GetSkillInfo()->EffectMineOperate;
								m_StateIconNum[0] = eStatusIcon_AttrDefenceIron;
			yCASE(ATTR_EARTH)	m_AttachEffectNum[0] = pSkillInfo->GetSkillInfo()->EffectMineOperate;
								m_StateIconNum[0] = eStatusIcon_AttrDefenceEarth;
			yCASE(ATTR_NOATTR)	m_AttachEffectNum[0] = pSkillInfo->GetSkillInfo()->EffectMineOperate;
								//m_StateIconNum[0] = eStatusIcon_AttrDefenceEarth;
		yENDSWITCH
	}
	if(m_PhyDefenceUp > 0)
	{
//maintain_defup_S.beff	방어력 상승
		m_AttachEffectNum[1] = FindEffectNum("maintain_defup_S.beff");
		m_StateIconNum[1] = eStatusIcon_PhyDefenceUp;
	}
#endif
}

void CSkillObjectAttachUnit_DefenceUp::GetAttDefense(WORD Attrib,float Original,float& CalcedUp,float& CalcedDown)
{
	if(m_Attrib != ATTR_ALLATTR && m_Attrib != Attrib)
		return;
	
	if(CalcedUp < m_AttDefenceUp)
		CalcedUp = m_AttDefenceUp;
}

void CSkillObjectAttachUnit_DefenceUp::GetPhyDefense(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)
{
	if(CalcedUp < m_PhyDefenceUp*Original)
		CalcedUp = (DWORD)(m_PhyDefenceUp*Original);
}



//////////////////////////////////////////////////////////////////////////
// Down
//////////////////////////////////////////////////////////////////////////

CSkillObjectAttachUnit_DefenceDown::CSkillObjectAttachUnit_DefenceDown()
{
	m_Flag = STATUS_FLAG_DEFENCE;
}

CSkillObjectAttachUnit_DefenceDown::~CSkillObjectAttachUnit_DefenceDown()
{

}

void CSkillObjectAttachUnit_DefenceDown::Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper/* =NULL */)
{
	m_Attrib = pSkillInfo->GetAttrib();
	m_AttDefenceDown = pSkillInfo->GetDownAttDefense(SkillLevel);
	m_PhyDefenceDown = pSkillInfo->GetDownPhyDefense(SkillLevel);
	
	//지속효과표시
	/*
	if(m_AttDefenceDown == 1)
	{
		ySWITCH(m_Attrib)
			yCASE(ATTR_FIRE)	m_AttachEffectNum[0] = FindEffectNum("maintain_Sresist01_s.beff");
								m_StateIconNum[0] = eStatusIcon_AttrDefenceFire;
			yCASE(ATTR_WATER)	m_AttachEffectNum[0] = FindEffectNum("maintain_Sresist02_s.beff");
								m_StateIconNum[0] = eStatusIcon_AttrDefenceWater;
			yCASE(ATTR_TREE)	m_AttachEffectNum[0] = FindEffectNum("maintain_Sresist03_s.beff");
								m_StateIconNum[0] = eStatusIcon_AttrDefenceTree;
			yCASE(ATTR_IRON)	m_AttachEffectNum[0] = FindEffectNum("maintain_Sresist04_s.beff");
								m_StateIconNum[0] = eStatusIcon_AttrDefenceIron;
			yCASE(ATTR_EARTH)	m_AttachEffectNum[0] = FindEffectNum("maintain_Sresist05_s.beff");
								m_StateIconNum[0] = eStatusIcon_AttrDefenceEarth;
		yENDSWITCH
	}
	else if(m_AttDefenceDown > 0)
	{
		ySWITCH(m_Attrib)
			yCASE(ATTR_FIRE)	m_AttachEffectNum[0] = FindEffectNum("maintain_resist01_s.beff");
								m_StateIconNum[0] = eStatusIcon_AttrDefenceFire;
			yCASE(ATTR_WATER)	m_AttachEffectNum[0] = FindEffectNum("maintain_resist02_s.beff");
								m_StateIconNum[0] = eStatusIcon_AttrDefenceWater;
			yCASE(ATTR_TREE)	m_AttachEffectNum[0] = FindEffectNum("maintain_resist03_s.beff");
								m_StateIconNum[0] = eStatusIcon_AttrDefenceTree;
			yCASE(ATTR_IRON)	m_AttachEffectNum[0] = FindEffectNum("maintain_resist04_s.beff");
								m_StateIconNum[0] = eStatusIcon_AttrDefenceIron;
			yCASE(ATTR_EARTH)	m_AttachEffectNum[0] = FindEffectNum("maintain_resist05_s.beff");
								m_StateIconNum[0] = eStatusIcon_AttrDefenceEarth;
		yENDSWITCH
	}
	*/
#ifdef _MHCLIENT_
	if(m_PhyDefenceDown > 0)
	{		
		m_AttachEffectNum[1] = FindEffectNum("maintain_defdown_s.beff");
		m_StateIconNum[1] = eStatusIcon_PhyDefenseDown;
	}
#endif

}

void CSkillObjectAttachUnit_DefenceDown::GetAttDefense(WORD Attrib,float Original,float& CalcedUp,float& CalcedDown)
{
	if(m_Attrib != ATTR_ALLATTR && m_Attrib != Attrib)
		return;
	
	if(CalcedDown < m_AttDefenceDown)
		CalcedDown = m_AttDefenceDown;
}

void CSkillObjectAttachUnit_DefenceDown::GetPhyDefense(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)
{
	if(CalcedDown < m_PhyDefenceDown*Original)
		CalcedDown = (DWORD)(m_PhyDefenceDown*Original);
}


//////////////////////////////////////////////////////////////////////////
// DamageRate
//////////////////////////////////////////////////////////////////////////

CSkillObjectAttachUnit_DamageDownRate::CSkillObjectAttachUnit_DamageDownRate()
{
	m_fDamageRate = 0.0f;
}

CSkillObjectAttachUnit_DamageDownRate::~CSkillObjectAttachUnit_DamageDownRate()
{

}

void CSkillObjectAttachUnit_DamageDownRate::Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper/* =NULL */)
{
	m_fDamageRate = pSkillInfo->GetDamageRate(SkillLevel);
#ifdef _MHCLIENT_
//	m_AttachEffectNum[1] = FindEffectNum("maintain_defdown_s.beff");
	m_StateIconNum[1] = eStatusIcon_DamageDown;
#endif
}


void CSkillObjectAttachUnit_DamageDownRate::GetDamage(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)
{
	CalcedDown = (DWORD)(m_fDamageRate*Original);

	if( CalcedDown > Original )	//데미지 감소량이 본데미지보다 클경우
		CalcedDown = Original;
}
