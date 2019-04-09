// EffectTriggerOffUnitDesc.h: interface for the CEffectTriggerOffUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFECTTRIGGEROFFUNITDESC_H__B737BBDC_ABFF_4FCC_88C8_342BA96F0B49__INCLUDED_)
#define AFX_EFFECTTRIGGEROFFUNITDESC_H__B737BBDC_ABFF_4FCC_88C8_342BA96F0B49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectTriggerUnitDesc.h"

class CEffectTriggerOffUnitDesc : public CEffectTriggerUnitDesc  
{
public:
	CEffectTriggerOffUnitDesc(DWORD dwTime,DWORD dwUnitNum);
	virtual ~CEffectTriggerOffUnitDesc();

	void ParseScript(CMHFile* pFile);
	BOOL Operate(CEffect* pEffect);

};

#endif // !defined(AFX_EFFECTTRIGGEROFFUNITDESC_H__B737BBDC_ABFF_4FCC_88C8_342BA96F0B49__INCLUDED_)
