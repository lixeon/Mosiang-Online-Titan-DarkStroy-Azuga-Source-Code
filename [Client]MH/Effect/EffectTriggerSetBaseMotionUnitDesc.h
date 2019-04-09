// EffectTriggerSetBaseMotionUnitDesc.h: interface for the CEffectTriggerSetBaseMotionUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFECTTRIGGERSETBASEMOTIONUNITDESC_H__5F6915EB_487C_4E5E_B036_00945A94018D__INCLUDED_)
#define AFX_EFFECTTRIGGERSETBASEMOTIONUNITDESC_H__5F6915EB_487C_4E5E_B036_00945A94018D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectTriggerUnitDesc.h"

class CEffectTriggerSetBaseMotionUnitDesc : public CEffectTriggerUnitDesc  
{
	int m_BaseMotionNum;
public:
	CEffectTriggerSetBaseMotionUnitDesc(DWORD dwTime,DWORD dwUnitNum);
	virtual ~CEffectTriggerSetBaseMotionUnitDesc();

	void ParseScript(CMHFile* pFile);
	BOOL Operate(CEffect* pEffect);

};

#endif // !defined(AFX_EFFECTTRIGGERSETBASEMOTIONUNITDESC_H__5F6915EB_487C_4E5E_B036_00945A94018D__INCLUDED_)
