// EffectTriggerAnimateUnitDesc.h: interface for the CEffectTriggerAnimateUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFECTTRIGGERANIMATEUNITDESC_H__163B803C_7439_4727_B519_6B127E7788BF__INCLUDED_)
#define AFX_EFFECTTRIGGERANIMATEUNITDESC_H__163B803C_7439_4727_B519_6B127E7788BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectTriggerUnitDesc.h"

class CEffectTriggerAnimateUnitDesc : public CEffectTriggerUnitDesc  
{
	int m_Motion;
	BOOL m_bRepeat;

public:
	CEffectTriggerAnimateUnitDesc(DWORD dwTime,DWORD dwUnitNum);
	virtual ~CEffectTriggerAnimateUnitDesc();

	void ParseScript(CMHFile* pFile);
	BOOL Operate(CEffect* pEffect);

};

#endif // !defined(AFX_EFFECTTRIGGERANIMATEUNITDESC_H__163B803C_7439_4727_B519_6B127E7788BF__INCLUDED_)
