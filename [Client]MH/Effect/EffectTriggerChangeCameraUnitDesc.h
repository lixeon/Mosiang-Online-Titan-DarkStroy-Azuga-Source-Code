// EffectTriggerChangeCameraUnitDesc.h: interface for the CEffectTriggerChangeCameraUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFECTTRIGGERCHANGECAMERAUNITDESC_H__7CB77C66_1D69_4FB5_AFDB_DB6BF7A5A958__INCLUDED_)
#define AFX_EFFECTTRIGGERCHANGECAMERAUNITDESC_H__7CB77C66_1D69_4FB5_AFDB_DB6BF7A5A958__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectTriggerUnitDesc.h"

class CEffectTriggerChangeCameraUnitDesc : public CEffectTriggerUnitDesc  
{
	int m_ToCamera;
	DWORD m_Duration;
public:
	CEffectTriggerChangeCameraUnitDesc(DWORD dwTime,DWORD dwUnitNum);
	virtual ~CEffectTriggerChangeCameraUnitDesc();

	void ParseScript(CMHFile* pFile);
	BOOL Operate(CEffect* pEffect);

};

#endif // !defined(AFX_EFFECTTRIGGERCHANGECAMERAUNITDESC_H__7CB77C66_1D69_4FB5_AFDB_DB6BF7A5A958__INCLUDED_)
