// EffectTriggerMoveUnitDesc.h: interface for the CEffectTriggerMoveUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFECTTRIGGERMOVEUNITDESC_H__9F62F229_126F_4853_8FE7_6A2BA8B05339__INCLUDED_)
#define AFX_EFFECTTRIGGERMOVEUNITDESC_H__9F62F229_126F_4853_8FE7_6A2BA8B05339__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectTriggerUnitDesc.h"

class CEffectTriggerMoveUnitDesc : public CEffectTriggerUnitDesc  
{
public:
	VECTOR3 m_ToPosition;
	VECTOR3 m_ToRandom;
	DWORD m_MoveTime;
	BYTE m_bMainTarget;
	BOOL m_bTurn;
	
	CEffectTriggerMoveUnitDesc(DWORD dwTime,DWORD dwUnitNum);
	virtual ~CEffectTriggerMoveUnitDesc();

	void ParseScript(CMHFile* pFile);
	BOOL Operate(CEffect* pEffect);

};

#endif // !defined(AFX_EFFECTTRIGGERMOVEUNITDESC_H__9F62F229_126F_4853_8FE7_6A2BA8B05339__INCLUDED_)
