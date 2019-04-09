// SkillObjectAttachUnit_Attack.h: interface for the CSkillObjectAttachUnit_Attack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTATTACHUNIT_ATTACK_H__C3E047C0_823F_42F5_97DE_C61AB9C98985__INCLUDED_)
#define AFX_SKILLOBJECTATTACHUNIT_ATTACK_H__C3E047C0_823F_42F5_97DE_C61AB9C98985__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillObjectAttachUnit.h"

class CSkillObjectAttachUnit_AttackUp : public CSkillObjectAttachUnit  
{
	float m_PhyAttackUp;

public:
	CSkillObjectAttachUnit_AttackUp();
	virtual ~CSkillObjectAttachUnit_AttackUp();

//	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel);
	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);
	
	virtual void GetPhyAttackPowerMin(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown);
	virtual void GetPhyAttackPowerMax(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown);
};

class CSkillObjectAttachUnit_AttackDown : public CSkillObjectAttachUnit  
{
	float m_AttAttackDown;
	float m_PhyAttackDown;

public:
	CSkillObjectAttachUnit_AttackDown();
	virtual ~CSkillObjectAttachUnit_AttackDown();

//	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel);
	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);
	
	virtual void GetPhyAttackPowerMin(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown);
	virtual void GetPhyAttackPowerMax(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown);
};


class CSkillObjectAttachUnit_AttackLastUpRate : public CSkillObjectAttachUnit  
{
	float m_fAttackLastUpRate;

public:
	CSkillObjectAttachUnit_AttackLastUpRate();
	virtual ~CSkillObjectAttachUnit_AttackLastUpRate();

//	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel);
	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);
	
	virtual void GetAttackDamage(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown);
};


class CSkillObjectAttachUnit_AttackLastPhyUp : public CSkillObjectAttachUnit  
{
	DWORD m_fAttackPhyLastUp;

public:
	CSkillObjectAttachUnit_AttackLastPhyUp();
	virtual ~CSkillObjectAttachUnit_AttackLastPhyUp();

//	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel);
	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);
	
	virtual void GetAttackPhyDamagePlus(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown);
};


class CSkillObjectAttachUnit_AttackLastAttrUp : public CSkillObjectAttachUnit  
{

	DWORD m_fAttackAttLastUp;

public:
	CSkillObjectAttachUnit_AttackLastAttrUp();
	virtual ~CSkillObjectAttachUnit_AttackLastAttrUp();

//	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel);
	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);
	
	virtual void GetAttackAttDamagePlus(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown);
};




#endif // !defined(AFX_SKILLOBJECTATTACHUNIT_ATTACK_H__C3E047C0_823F_42F5_97DE_C61AB9C98985__INCLUDED_)
