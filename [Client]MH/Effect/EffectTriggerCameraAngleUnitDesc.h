// EffectTriggerAngleUnitDesc.h: interface for the CEffectTriggerAngleUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFECTTRIGGERANGLEUNITDESC_H__74355483_456A_47B2_9F29_9B05B7CAEB73__INCLUDED_)
#define AFX_EFFECTTRIGGERANGLEUNITDESC_H__74355483_456A_47B2_9F29_9B05B7CAEB73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectTriggerUnitDesc.h"

class CEffectTriggerCameraAngleUnitDesc : public CEffectTriggerUnitDesc  
{
	float m_TargetAngle;
	DWORD m_Duration;

public:
	CEffectTriggerCameraAngleUnitDesc(DWORD dwTime,DWORD dwUnitNum);
	virtual ~CEffectTriggerCameraAngleUnitDesc();

	void ParseScript(CMHFile* pFile);
	BOOL Operate(CEffect* pEffect);

};

#endif // !defined(AFX_EFFECTTRIGGERANGLEUNITDESC_H__74355483_456A_47B2_9F29_9B05B7CAEB73__INCLUDED_)
