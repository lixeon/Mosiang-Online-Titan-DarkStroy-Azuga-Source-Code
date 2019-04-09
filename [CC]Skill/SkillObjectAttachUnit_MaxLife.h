// SkillObjectAttachUnit_MaxLife.h: interface for the CSkillObjectAttachUnit_MaxLife class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTATTACHUNIT_MAXLIFE_H__CACD267E_4063_414D_BB90_FB8E3A724992__INCLUDED_)
#define AFX_SKILLOBJECTATTACHUNIT_MAXLIFE_H__CACD267E_4063_414D_BB90_FB8E3A724992__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillObjectAttachUnit.h"

class CSkillObjectAttachUnit_MaxLifeNaeRyukUp : public CSkillObjectAttachUnit  
{
	WORD m_MaxLifeUp;
	WORD m_MaxNaeRyukUp;
	WORD m_MaxShieldUp;

public:
	CSkillObjectAttachUnit_MaxLifeNaeRyukUp();
	virtual ~CSkillObjectAttachUnit_MaxLifeNaeRyukUp();

//	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel);
	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);
	
	virtual void GetMaxLife(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown);
	virtual void GetMaxNaeRyuk(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown);
	virtual void GetMaxShield(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown);
};

class CSkillObjectAttachUnit_MaxLifeNaeRyukDown : public CSkillObjectAttachUnit  
{
	WORD m_MaxLifeDown;
	WORD m_MaxNaeRyukDown;
	WORD m_MaxShieldDown;

public:
	CSkillObjectAttachUnit_MaxLifeNaeRyukDown();
	virtual ~CSkillObjectAttachUnit_MaxLifeNaeRyukDown();

//	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel);
	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);
	
	virtual void GetMaxLife(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown);
	virtual void GetMaxNaeRyuk(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown);
	virtual void GetMaxShield(DWORD Original,DWORD& CalcedUp,DWORD& CalcedDown);
};


class CSkillObjectAttachUnit_RecoverLifeShield : public CSkillObjectAttachUnit  
{
	DWORD m_RecoverLife;
	DWORD m_RecoverShield;

public:
	CSkillObjectAttachUnit_RecoverLifeShield();
	virtual ~CSkillObjectAttachUnit_RecoverLifeShield();

//	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel);
	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);
	
	virtual void GetRecoverLife(DWORD& CalcedLife);
	virtual void GetRecoverShield(DWORD& CalcedShield);
};



#endif // !defined(AFX_SKILLOBJECTATTACHUNIT_MAXLIFE_H__CACD267E_4063_414D_BB90_FB8E3A724992__INCLUDED_)
