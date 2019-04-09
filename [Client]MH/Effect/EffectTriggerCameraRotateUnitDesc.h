// EffectTriggerCameraRoateUnitDesc.h: interface for the CEffectTriggerCameraRoateUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFECTTRIGGERCAMERAROATEUNITDESC_H__2C09645C_2740_4F0D_98EF_C7AC98CA71AA__INCLUDED_)
#define AFX_EFFECTTRIGGERCAMERAROATEUNITDESC_H__2C09645C_2740_4F0D_98EF_C7AC98CA71AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectTriggerUnitDesc.h"

class CEffectTriggerCameraRotateUnitDesc : public CEffectTriggerUnitDesc  
{
	float m_RotateAngle;
	DWORD m_Duration;

public:
	CEffectTriggerCameraRotateUnitDesc(DWORD dwTime,DWORD dwUnitNum);
	virtual ~CEffectTriggerCameraRotateUnitDesc();

	void ParseScript(CMHFile* pFile);
	BOOL Operate(CEffect* pEffect);

};

#endif // !defined(AFX_EFFECTTRIGGERCAMERAROATEUNITDESC_H__2C09645C_2740_4F0D_98EF_C7AC98CA71AA__INCLUDED_)
