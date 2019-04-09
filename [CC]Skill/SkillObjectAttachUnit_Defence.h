// SkillObjectAttachUnit_Defence.h: interface for the CSkillObjectAttachUnit_Defence class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTATTACHUNIT_DEFENCE_H__E73AA099_88ED_4D89_8564_5CAB155BB590__INCLUDED_)
#define AFX_SKILLOBJECTATTACHUNIT_DEFENCE_H__E73AA099_88ED_4D89_8564_5CAB155BB590__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillObjectAttachUnit.h"
#include "ValueInSung.h"

class CSkillObjectAttachUnit_DefenceUp : public CSkillObjectAttachUnit  
{
	WORD m_Attrib;
	float m_AttDefenceUp;
	float m_PhyDefenceUp;

public:
	CSkillObjectAttachUnit_DefenceUp();
	virtual ~CSkillObjectAttachUnit_DefenceUp();

//	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel);
	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);
	
	virtual void GetPhyDefense(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown);
	virtual void GetAttDefense(WORD Attrib,float Original,float& CalcedUp,float& CalcedDown);
};


class CSkillObjectAttachUnit_DefenceDown : public CSkillObjectAttachUnit  
{
	WORD m_Attrib;
	float m_AttDefenceDown;
	float m_PhyDefenceDown;

public:
	CSkillObjectAttachUnit_DefenceDown();
	virtual ~CSkillObjectAttachUnit_DefenceDown();

//	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel);
	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);
	
	virtual void GetPhyDefense(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown);
	virtual void GetAttDefense(WORD Attrib,float Original,float& CalcedUp,float& CalcedDown);
};

class CSkillObjectAttachUnit_DamageDownRate : public CSkillObjectAttachUnit  
{
	float m_fDamageRate;

public:
	CSkillObjectAttachUnit_DamageDownRate();
	virtual ~CSkillObjectAttachUnit_DamageDownRate();

//	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel);
	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);
	
	virtual void GetDamage(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown);
};




#endif // !defined(AFX_SKILLOBJECTATTACHUNIT_DEFENCE_H__E73AA099_88ED_4D89_8564_5CAB155BB590__INCLUDED_)
