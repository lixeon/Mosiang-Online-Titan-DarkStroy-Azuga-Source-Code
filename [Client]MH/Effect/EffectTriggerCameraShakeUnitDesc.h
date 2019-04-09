// EffectTriggerEarthQuakeUnitDesc.h: interface for the CEffectTriggerCameraShakeUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFECTTRIGGEREARTHQUAKEUNITDESC_H__1781F9EF_C913_4F5B_B135_CF6EF9269C42__INCLUDED_)
#define AFX_EFFECTTRIGGEREARTHQUAKEUNITDESC_H__1781F9EF_C913_4F5B_B135_CF6EF9269C42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectTriggerUnitDesc.h"

class CEffectTriggerCameraShakeUnitDesc : public CEffectTriggerUnitDesc  
{
	DWORD m_ShakeNum;
	float m_Amplitude;
public:
	CEffectTriggerCameraShakeUnitDesc(DWORD dwTime,DWORD dwUnitNum);
	virtual ~CEffectTriggerCameraShakeUnitDesc();

	void ParseScript(CMHFile* pFile);
	BOOL Operate(CEffect* pEffect);

};

#endif // !defined(AFX_EFFECTTRIGGEREARTHQUAKEUNITDESC_H__1781F9EF_C913_4F5B_B135_CF6EF9269C42__INCLUDED_)
