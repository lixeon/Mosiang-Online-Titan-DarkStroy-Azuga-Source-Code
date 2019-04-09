// n.h: interface for the CEffectTriggerIllusionUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N_H__B8D17751_6E1D_4561_88B9_76A00FD718CF__INCLUDED_)
#define AFX_N_H__B8D17751_6E1D_4561_88B9_76A00FD718CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectTriggerUnitDesc.h"

class CEffectTriggerIllusionUnitDesc : public CEffectTriggerUnitDesc  
{
	enum TargetKind
	{
		eTargetKind_Weapon,
	};
	enum SwitchKind
	{
		eSwitchKind_Start,
		eSwitchKind_End,
	};
	int m_TargetKind;
	int m_SwitchKind;
	DWORD m_IllusionFrame;
	WORD m_MaterialNum;

public:
	CEffectTriggerIllusionUnitDesc(DWORD dwTime,DWORD dwUnitNum);
	virtual ~CEffectTriggerIllusionUnitDesc();

	void ParseScript(CMHFile* pFile);
	BOOL Operate(CEffect* pEffect);
};

#endif // !defined(AFX_N_H__B8D17751_6E1D_4561_88B9_76A00FD718CF__INCLUDED_)
