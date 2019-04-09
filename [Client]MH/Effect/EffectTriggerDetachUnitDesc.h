// EffectTriggerDetachUnitDesc.h: interface for the CEffectTriggerDetachUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFECTTRIGGERDETACHUNITDESC_H__051BCA5F_F0BF_495A_BAE2_18A33C86ADFB__INCLUDED_)
#define AFX_EFFECTTRIGGERDETACHUNITDESC_H__051BCA5F_F0BF_495A_BAE2_18A33C86ADFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectTriggerUnitDesc.h"

class CEffectTriggerDetachUnitDesc : public CEffectTriggerUnitDesc  
{
public:
	CEffectTriggerDetachUnitDesc(DWORD dwTime,DWORD dwUnitNum);
	virtual ~CEffectTriggerDetachUnitDesc();

	void ParseScript(CMHFile* pFile);
	BOOL Operate(CEffect* pEffect);

};

#endif // !defined(AFX_EFFECTTRIGGERDETACHUNITDESC_H__051BCA5F_F0BF_495A_BAE2_18A33C86ADFB__INCLUDED_)
