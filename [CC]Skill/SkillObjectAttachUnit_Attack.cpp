// SkillObjectAttachUnit_Attack.cpp: implementation of the CSkillObjectAttachUnit_AttackUp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObjectAttachUnit_Attack.h"


#ifdef _MAPSERVER_
#include "AttackManager.h"
#include "skillmanager_server.h"
#include "TacticManager.h"
#endif

#ifdef _MHCLIENT_
#include "StatusIconDlg.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Up
//////////////////////////////////////////////////////////////////////////

CSkillObjectAttachUnit_AttackUp::CSkillObjectAttachUnit_AttackUp()
{
	m_Flag = STATUS_FLAG_ATTACK;
}

CSkillObjectAttachUnit_AttackUp::~CSkillObjectAttachUnit_AttackUp()
{

}

void CSkillObjectAttachUnit_AttackUp::Init( CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper/* =NULL */)
{
	//지속효과표시
	//maintain_attup_S.beff	공격력 상승

#ifdef _MHCLIENT_
	m_PhyAttackUp = pSkillInfo->GetUpPhyAttack(SkillLevel);
	if(m_PhyAttackUp)
	{
		m_AttachEffectNum[0] = pSkillInfo->GetSkillInfo()->EffectMineOperate;
		m_StateIconNum[0] = eStatusIcon_PhyAttackUp;
	}
#endif

#ifdef _MAPSERVER_
	if( pOper )
	if( pOper->GetObjectKind() == eObjectKind_Player )
	if( pSkillInfo->GetSkillKind() == SKILLKIND_JINBUB )
	{
		m_PhyAttackUp = TACTICMGR->GetBuffRateForPhyAttack( (CPlayer*)pOper, pSkillInfo->GetSkillIndex() );
	}
#endif
}

void CSkillObjectAttachUnit_AttackUp::GetPhyAttackPowerMin(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)
{
	if(CalcedUp < m_PhyAttackUp*Original)
		CalcedUp = (DWORD)(m_PhyAttackUp*Original);
}

void CSkillObjectAttachUnit_AttackUp::GetPhyAttackPowerMax(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)
{
	if(CalcedUp < m_PhyAttackUp*Original)
		CalcedUp = (DWORD)(m_PhyAttackUp*Original);
}





//////////////////////////////////////////////////////////////////////////
// Down
//////////////////////////////////////////////////////////////////////////

CSkillObjectAttachUnit_AttackDown::CSkillObjectAttachUnit_AttackDown()
{
	m_Flag = STATUS_FLAG_ATTACK;
}

CSkillObjectAttachUnit_AttackDown::~CSkillObjectAttachUnit_AttackDown()
{

}

void CSkillObjectAttachUnit_AttackDown::Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper/* =NULL */)
{
	m_AttAttackDown = 0;		// 속성 공격력 올려주는건 아직 없음
	m_PhyAttackDown = pSkillInfo->GetDownPhyAttack(SkillLevel);

#ifdef _MHCLIENT_
	//지속효과표시
	m_AttachEffectNum[0] = FindEffectNum("maintain_attackdown_s.beff");;
	m_AttachEffectNum[1] = 0;
	m_AttachEffectNum[2] = 0;
		
	m_StateIconNum[0] = eStatusIcon_PhyAttackDown;
#endif
}

void CSkillObjectAttachUnit_AttackDown::GetPhyAttackPowerMin(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)
{
	if(CalcedDown < m_PhyAttackDown*Original)
		CalcedDown = (DWORD)(m_PhyAttackDown*Original);
}

void CSkillObjectAttachUnit_AttackDown::GetPhyAttackPowerMax(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)
{
	if(CalcedDown < m_PhyAttackDown*Original)
		CalcedDown = (DWORD)(m_PhyAttackDown*Original);
}


//////////////////////////////////////////////////////////////////////////
// LastUpRate //최종공격력에서 %증가
//////////////////////////////////////////////////////////////////////////

CSkillObjectAttachUnit_AttackLastUpRate::CSkillObjectAttachUnit_AttackLastUpRate()
{
	m_fAttackLastUpRate = 0.0f;
}

CSkillObjectAttachUnit_AttackLastUpRate::~CSkillObjectAttachUnit_AttackLastUpRate()
{

}

void CSkillObjectAttachUnit_AttackLastUpRate::Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper/* =NULL */)
{
	m_fAttackLastUpRate = pSkillInfo->GetAttackRate(SkillLevel);
	
#ifdef _MHCLIENT_
	//지속효과표시
//	m_AttachEffectNum[0] = FindEffectNum("maintain_attackdown_s.beff");;
//	m_AttachEffectNum[1] = 0;
//	m_AttachEffectNum[2] = 0;
		
	m_StateIconNum[0] = eStatusIcon_AttackUp;
#endif
}
	
void CSkillObjectAttachUnit_AttackLastUpRate::GetAttackDamage(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)
{
	CalcedUp = (DWORD)(Original*m_fAttackLastUpRate);
}


//////////////////////////////////////////////////////////////////////////
// LastPhyUp //최종 외공 공격력에서 +증가
//////////////////////////////////////////////////////////////////////////

CSkillObjectAttachUnit_AttackLastPhyUp::CSkillObjectAttachUnit_AttackLastPhyUp()
{
	m_fAttackPhyLastUp = 0;
}

CSkillObjectAttachUnit_AttackLastPhyUp::~CSkillObjectAttachUnit_AttackLastPhyUp()
{

}

void CSkillObjectAttachUnit_AttackLastPhyUp::Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper/* =NULL */)
{
	m_fAttackPhyLastUp = (DWORD)pSkillInfo->GetAttackPhyLastUp((int)SkillLevel);
	
#ifdef _MHCLIENT_
	//지속효과표시
//	m_AttachEffectNum[0] = FindEffectNum("maintain_attackdown_s.beff");;
//	m_AttachEffectNum[1] = 0;
//	m_AttachEffectNum[2] = 0;
	if( m_fAttackPhyLastUp > 0 )
		m_StateIconNum[0] = eStatusIcon_AttackPhyLastUp;

#endif
}
	
void CSkillObjectAttachUnit_AttackLastPhyUp::GetAttackPhyDamagePlus(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)
{
	if( Original > 0 )
		CalcedUp = m_fAttackPhyLastUp;
}
	




//////////////////////////////////////////////////////////////////////////
// LastAttrUp //최종 외공 공격력에서 +증가
//////////////////////////////////////////////////////////////////////////

CSkillObjectAttachUnit_AttackLastAttrUp::CSkillObjectAttachUnit_AttackLastAttrUp()
{
	m_fAttackAttLastUp = 0;
}

CSkillObjectAttachUnit_AttackLastAttrUp::~CSkillObjectAttachUnit_AttackLastAttrUp()
{

}

void CSkillObjectAttachUnit_AttackLastAttrUp::Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper/* =NULL */)
{
	m_fAttackAttLastUp = (DWORD)pSkillInfo->GetAttackAttLastUp((int)SkillLevel);
	
#ifdef _MHCLIENT_
	//지속효과표시
//	m_AttachEffectNum[0] = FindEffectNum("maintain_attackdown_s.beff");;
//	m_AttachEffectNum[1] = 0;
//	m_AttachEffectNum[2] = 0;
	if( m_fAttackAttLastUp > 0 )
		m_StateIconNum[0] = eStatusIcon_AttackAttLastUp;

#endif
}
	

void CSkillObjectAttachUnit_AttackLastAttrUp::GetAttackAttDamagePlus(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown)
{
	if( Original > 0 )
		CalcedUp = m_fAttackAttLastUp;
}


