// EffectTriggerCameraZoomUnitDesc.h: interface for the CEffectTriggerCameraZoomUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFECTTRIGGERCAMERAZOOMUNITDESC_H__4FC532EB_C665_4CDB_8524_D33A5342D318__INCLUDED_)
#define AFX_EFFECTTRIGGERCAMERAZOOMUNITDESC_H__4FC532EB_C665_4CDB_8524_D33A5342D318__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectTriggerUnitDesc.h"

class CEffectTriggerCameraZoomUnitDesc : public CEffectTriggerUnitDesc  
{
	float m_fZoomDistance;
	DWORD m_Duration;

public:
	CEffectTriggerCameraZoomUnitDesc(DWORD dwTime,DWORD dwUnitNum);
	virtual ~CEffectTriggerCameraZoomUnitDesc();

	void ParseScript(CMHFile* pFile);
	BOOL Operate(CEffect* pEffect);

};

#endif // !defined(AFX_EFFECTTRIGGERCAMERAZOOMUNITDESC_H__4FC532EB_C665_4CDB_8524_D33A5342D318__INCLUDED_)
