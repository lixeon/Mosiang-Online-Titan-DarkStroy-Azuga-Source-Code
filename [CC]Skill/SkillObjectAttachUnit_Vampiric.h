// ma.h: interface for the CSkillObjectAttachUnit_Vampiric class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MA_H__3D312FB2_2E21_470A_B44D_EABF22BC71E1__INCLUDED_)
#define AFX_MA_H__3D312FB2_2E21_470A_B44D_EABF22BC71E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillObjectAttachUnit.h"

class CSkillObjectAttachUnit_Vampiric : public CSkillObjectAttachUnit  
{
	float m_VampLifeRate;
	float m_VampNaeryukRate;

public:
	CSkillObjectAttachUnit_Vampiric();
	virtual ~CSkillObjectAttachUnit_Vampiric();

//	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel);
	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);
	
	virtual void GetVampiricLife(float& fRate);
	virtual void GetVampiricNaeryuk(float& fRate);
};


class CSkillObjectAttachUnit_ReverseVampiric : public CSkillObjectAttachUnit  
{
	float m_VampLifeRate;
	float m_VampNaeryukRate;

public:
	CSkillObjectAttachUnit_ReverseVampiric();
	virtual ~CSkillObjectAttachUnit_ReverseVampiric();

//	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel);
	virtual void Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper=NULL);
	
	virtual void GetReverseVampiricLife(float& fRate);
	virtual void GetReverseVampiricNaeryuk(float& fRate);
};


#endif // !defined(AFX_MA_H__3D312FB2_2E21_470A_B44D_EABF22BC71E1__INCLUDED_)
