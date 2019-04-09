// SkillObjectAttachUnit_MaxLife.cpp: implementation of the CSkillObjectAttachUnit_MaxLifeNaeRyukUp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObjectAttachUnit_MaxLife.h"

#ifdef _MHCLIENT_
#include "StatusIconDlg.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// Up
//////////////////////////////////////////////////////////////////////////

CSkillObjectAttachUnit_MaxLifeNaeRyukUp::CSkillObjectAttachUnit_MaxLifeNaeRyukUp()
{
	m_Flag = STATUS_FLAG_LIFE | STATUS_FLAG_NAERYUK;
}

CSkillObjectAttachUnit_MaxLifeNaeRyukUp::~CSkillObjectAttachUnit_MaxLifeNaeRyukUp()
{

}

void CSkillObjectAttachUnit_MaxLifeNaeRyukUp::Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper/* =NULL */)
{
	m_MaxLifeUp = pSkillInfo->GetUpMaxLife(SkillLevel);
	m_MaxNaeRyukUp = pSkillInfo->GetUpMaxNaeRyuk(SkillLevel);
	m_MaxShieldUp = pSkillInfo->GetUpMaxShield(SkillLevel);
#ifdef _MHCLIENT_
	//지속효과표시
	if(m_MaxLifeUp > 0)
	{
		m_AttachEffectNum[0] = pSkillInfo->GetSkillInfo()->EffectMineOperate;
		m_StateIconNum[0] = eStatusIcon_MaxLifeUp;
	}
	if(m_MaxNaeRyukUp > 0)
	{
		m_AttachEffectNum[0] = pSkillInfo->GetSkillInfo()->EffectMineOperate;
		m_StateIconNum[1] = eStatusIcon_MaxNaeryukUp;
	}
	if(m_MaxShieldUp > 0)
	{
		//호신강기 업 이펙트가 없어서 생명력 업을 그대로 씀 040801
		m_AttachEffectNum[0] = pSkillInfo->GetSkillInfo()->EffectMineOperate;
		m_StateIconNum[2] = eStatusIcon_MaxShieldUp;
	}
#endif
}

void CSkillObjectAttachUnit_MaxLifeNaeRyukUp::GetMaxLife(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)
{
	if(CalcedUp < m_MaxLifeUp)
		CalcedUp = m_MaxLifeUp;
}

void CSkillObjectAttachUnit_MaxLifeNaeRyukUp::GetMaxNaeRyuk(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)
{
	if(CalcedUp < m_MaxNaeRyukUp)
		CalcedUp = m_MaxNaeRyukUp;
}

void CSkillObjectAttachUnit_MaxLifeNaeRyukUp::GetMaxShield(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)
{
	if(CalcedUp < m_MaxShieldUp)
		CalcedUp = m_MaxShieldUp;
}


//////////////////////////////////////////////////////////////////////////
// Down
//////////////////////////////////////////////////////////////////////////

CSkillObjectAttachUnit_MaxLifeNaeRyukDown::CSkillObjectAttachUnit_MaxLifeNaeRyukDown()
{
	m_Flag = STATUS_FLAG_LIFE | STATUS_FLAG_NAERYUK;
}

CSkillObjectAttachUnit_MaxLifeNaeRyukDown::~CSkillObjectAttachUnit_MaxLifeNaeRyukDown()
{

}

void CSkillObjectAttachUnit_MaxLifeNaeRyukDown::Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper/* =NULL */)
{
	m_MaxLifeDown = pSkillInfo->GetDownMaxLife(SkillLevel);
	m_MaxNaeRyukDown = pSkillInfo->GetDownMaxNaeRyuk(SkillLevel);
	m_MaxShieldDown = pSkillInfo->GetDownMaxShield(SkillLevel);

#ifdef _MHCLIENT_
	//지속효과표시
	if(m_MaxLifeDown > 0)
	{
//		m_AttachEffectNum[0] = 677;
		m_StateIconNum[0] = eStatusIcon_MaxLifeDown;
	}
	if(m_MaxNaeRyukDown > 0)
	{
//		m_AttachEffectNum[1] = 680;
		m_StateIconNum[1] = eStatusIcon_MaxNaeryukDown;
	}
	if(m_MaxShieldDown > 0)
	{
//		m_AttachEffectNum[2] = 716;
		m_StateIconNum[2] = eStatusIcon_MaxShieldDown;
	}
#endif
}

void CSkillObjectAttachUnit_MaxLifeNaeRyukDown::GetMaxLife(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)
{
	if(CalcedDown < m_MaxLifeDown)
		CalcedDown = m_MaxLifeDown;
}

void CSkillObjectAttachUnit_MaxLifeNaeRyukDown::GetMaxNaeRyuk(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)
{
	if(CalcedDown < m_MaxNaeRyukDown)
		CalcedDown = m_MaxNaeRyukDown;
}

void CSkillObjectAttachUnit_MaxLifeNaeRyukDown::GetMaxShield(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)
{
	if(CalcedDown < m_MaxShieldDown)
		CalcedDown = m_MaxShieldDown;
}

//////////////////////////////////////////////////////////////////////////
// RecoverLifeShield
//////////////////////////////////////////////////////////////////////////

CSkillObjectAttachUnit_RecoverLifeShield::CSkillObjectAttachUnit_RecoverLifeShield()
{
	m_RecoverLife = 0;
	m_RecoverShield = 0;
}

CSkillObjectAttachUnit_RecoverLifeShield::~CSkillObjectAttachUnit_RecoverLifeShield()
{

}

void CSkillObjectAttachUnit_RecoverLifeShield::Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper/* =NULL */)
{
	m_RecoverLife = pSkillInfo->GetContinueRecoverLife(SkillLevel);
	m_RecoverShield = pSkillInfo->GetContinueRecoverShield(SkillLevel);
#ifdef _MHCLIENT_
//	m_AttachEffectNum[0] = FindEffectNum("maintain_defdown_s.beff");
	m_StateIconNum[0] = eStatusIcon_ContinueRecover;
#endif
}

void CSkillObjectAttachUnit_RecoverLifeShield::GetRecoverLife(DWORD& CalcedLife)
{
	CalcedLife = m_RecoverLife;
}

void CSkillObjectAttachUnit_RecoverLifeShield::GetRecoverShield(DWORD& CalcedShield)
{
	CalcedShield = m_RecoverShield;
}
