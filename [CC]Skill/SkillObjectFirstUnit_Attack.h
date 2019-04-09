// SkillObjectFirstUnit_Attack.h: interface for the CSkillObjectFirstUnit_Attack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTFIRSTUNIT_ATTACK_H__A3023D7E_8ED0_4859_84D1_67AA00890DB5__INCLUDED_)
#define AFX_SKILLOBJECTFIRSTUNIT_ATTACK_H__A3023D7E_8ED0_4859_84D1_67AA00890DB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillObjectFirstUnit.h"
#include "ValueInSung.h"

class CSkillObjectFirstUnit_Attack : public CSkillObjectFirstUnit  
{
	// Common
	BOOL m_bMugong;

	MINMAXWORD m_AttribAttack;
	float m_AttAttackRate;
	float m_PhysicalAttack;	
	
	float m_CriticalRate;
#ifdef _MAPSERVER_
	CSkillInfo* m_pSkillInfo;
#endif
	
public:
	CSkillObjectFirstUnit_Attack(SKILLINFO* pSkillInfo);
	virtual ~CSkillObjectFirstUnit_Attack();

	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);
	
#ifdef _MAPSERVER_
	virtual int ExcuteFirstUnit(CObject* pOperator,CSkillObjectTargetList* pTargetList,
							MSG_SKILLOBJECT_ADD* pAddMsg,float SkillTreeAmp);
#endif
	
#ifdef _MHCLIENT_
	virtual void StartFirstEffect(CObject* pOperator,CTargetList* pTList,MAINTARGET* pMainTarget);
#endif
};




class CSkillObjectFirstUnit_AttackLife : public CSkillObjectFirstUnit  
{
	float m_AttackRate;	
	float m_SuccessRate;
	
#ifdef _MAPSERVER_
	CSkillInfo* m_pSkillInfo;
#endif
	
public:
	CSkillObjectFirstUnit_AttackLife(SKILLINFO* pSkillInfo);
	virtual ~CSkillObjectFirstUnit_AttackLife();

	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);
	
#ifdef _MAPSERVER_
	virtual int ExcuteFirstUnit(CObject* pOperator,CSkillObjectTargetList* pTargetList,
							MSG_SKILLOBJECT_ADD* pAddMsg,float SkillTreeAmp);
#endif
	
#ifdef _MHCLIENT_
	virtual void StartFirstEffect(CObject* pOperator,CTargetList* pTList,MAINTARGET* pMainTarget);
#endif
};


class CSkillObjectFirstUnit_AttackShield : public CSkillObjectFirstUnit  
{
	float m_AttackRate;	
	float m_SuccessRate;
	
#ifdef _MAPSERVER_
	CSkillInfo* m_pSkillInfo;
#endif
	
public:
	CSkillObjectFirstUnit_AttackShield(SKILLINFO* pSkillInfo);
	virtual ~CSkillObjectFirstUnit_AttackShield();

	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);
	
#ifdef _MAPSERVER_
	virtual int ExcuteFirstUnit(CObject* pOperator,CSkillObjectTargetList* pTargetList,
							MSG_SKILLOBJECT_ADD* pAddMsg,float SkillTreeAmp);
#endif
	
#ifdef _MHCLIENT_
	virtual void StartFirstEffect(CObject* pOperator,CTargetList* pTList,MAINTARGET* pMainTarget);
#endif
};


#endif // !defined(AFX_SKILLOBJECTFIRSTUNIT_ATTACK_H__A3023D7E_8ED0_4859_84D1_67AA00890DB5__INCLUDED_)
