// EffectTriggerOnUnitDesc.h: interface for the CEffectTriggerOnUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFECTTRIGGERONUNITDESC_H__2DD5010A_FAC3_436D_8D1A_EEE8E65B757D__INCLUDED_)
#define AFX_EFFECTTRIGGERONUNITDESC_H__2DD5010A_FAC3_436D_8D1A_EEE8E65B757D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectTriggerUnitDesc.h"

class CEffectTriggerOnUnitDesc : public CEffectTriggerUnitDesc  
{
public:
	CEffectTriggerOnUnitDesc(DWORD dwTime,DWORD dwUnitNum);
	virtual ~CEffectTriggerOnUnitDesc();

	void ParseScript(CMHFile* pFile);
	BOOL Operate(CEffect* pEffect);

};

#endif // !defined(AFX_EFFECTTRIGGERONUNITDESC_H__2DD5010A_FAC3_436D_8D1A_EEE8E65B757D__INCLUDED_)
