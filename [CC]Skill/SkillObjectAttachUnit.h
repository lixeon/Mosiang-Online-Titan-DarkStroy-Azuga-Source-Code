// SkillObjectAttachUnit.h: interface for the CSkillObjectAttachUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTATTACHUNIT_H__FDF07252_D8F9_4670_A906_A9137E5B36CA__INCLUDED_)
#define AFX_SKILLOBJECTATTACHUNIT_H__FDF07252_D8F9_4670_A906_A9137E5B36CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Status.h"
#include "SkillInfo.h"

class CSkillObject;


enum eATTACH_UNIT_KIND
{
	eATTACH_UNIT_KIND_UNKNOWN,
	eATTACH_UNIT_KIND_DODGE,
};


class CSkillObjectAttachUnit : public CStatus  
{

protected:
#ifdef _MHCLIENT_	
	int m_AttachEffectNum[MAX_SKILLMAINTAIN_EFFECT];
	int m_StateIconNum[MAX_SKILLMAINTAIN_EFFECT];
#endif

	int m_nAttachUnitKind;
	
public:
	CSkillObjectAttachUnit();
	virtual ~CSkillObjectAttachUnit();
	
//	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel) = 0;
	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL) = 0;
#ifdef _MHCLIENT_
	inline WORD GetAttachEffectNum(int n)	{	return m_AttachEffectNum[n];	}
	inline WORD GetStateIconNum(int n)		{	return m_StateIconNum[n];	}
#endif

	int GetAttachUnitKind() { return m_nAttachUnitKind; }
};

#endif // !defined(AFX_SKILLOBJECTATTACHUNIT_H__FDF07252_D8F9_4670_A906_A9137E5B36CA__INCLUDED_)
