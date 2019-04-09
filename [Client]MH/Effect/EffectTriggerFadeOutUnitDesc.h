// EffectTriggerFadeOutUnitDesc.h: interface for the CEffectTriggerFadeOutUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFECTTRIGGERFADEOUTUNITDESC_H__8CF64F14_0376_403F_97DD_875BABC5FD9A__INCLUDED_)
#define AFX_EFFECTTRIGGERFADEOUTUNITDESC_H__8CF64F14_0376_403F_97DD_875BABC5FD9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectTriggerUnitDesc.h"

class CEffectTriggerFadeOutUnitDesc : public CEffectTriggerUnitDesc  
{
	DWORD m_FadeTime;
	float m_FadeStart;
	float m_FadeEnd;
public:
	CEffectTriggerFadeOutUnitDesc(DWORD dwTime,DWORD dwUnitNum);
	virtual ~CEffectTriggerFadeOutUnitDesc();

	void ParseScript(CMHFile* pFile);
	BOOL Operate(CEffect* pEffect);

};

#endif // !defined(AFX_EFFECTTRIGGERFADEOUTUNITDESC_H__8CF64F14_0376_403F_97DD_875BABC5FD9A__INCLUDED_)
