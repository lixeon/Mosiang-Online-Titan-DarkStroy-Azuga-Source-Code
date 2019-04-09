// SkillObjectFirstUnit.h: interface for the CSkillObjectFirstUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTFIRSTUNIT_H__0AF09EAB_1395_4EE0_B4E4_4DD6140AFE03__INCLUDED_)
#define AFX_SKILLOBJECTFIRSTUNIT_H__0AF09EAB_1395_4EE0_B4E4_4DD6140AFE03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillObjectTargetList.h"

#ifdef _MHCLIENT_
#include "Effect/EffectManager.h"
#endif

class CSkillObject;
class CSkillInfo;

class CSkillObjectFirstUnit  
{
protected:
	WORD m_Attrib;
	float m_RecoverStateAbnormal;
	WORD m_StunTime;
	float m_StunRate;

	float m_DispelAttackFeelRate;

#ifdef _MHCLIENT_
	SKILLINFO* m_pSkillInfoData;
	HEFFPROC m_hEff;
#endif


public:
	CSkillObjectFirstUnit(SKILLINFO* pSkillInfo);
	virtual ~CSkillObjectFirstUnit();	

	void Release();

	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);
	
#ifdef _MAPSERVER_
	void DefaultFirstEffect(CObject* pOperator,STLIST* pTarget,RESULTINFO* pRInfo);
	virtual int ExcuteFirstUnit(CObject* pOperator,CSkillObjectTargetList* pTargetList,
		MSG_SKILLOBJECT_ADD* pAddMsg,float SkillTreeAmp);
	virtual void FirstUnitResult()		{};	
#endif

#ifdef _MHCLIENT_
	virtual void StartFirstEffect(CObject* pOperator,CTargetList* pTList,MAINTARGET* pMainTarget);
	virtual void SetTargetArray(CObject* pOperator,CTargetList* pTList,MAINTARGET* pMainTarget);
#endif
};

#endif // !defined(AFX_SKILLOBJECTFIRSTUNIT_H__0AF09EAB_1395_4EE0_B4E4_4DD6140AFE03__INCLUDED_)
