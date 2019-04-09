// EffectTriggerLinkUnitDesc.h: interface for the CEffectTriggerLinkUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFECTTRIGGERLINKUNITDESC_H__5644C87D_2017_4376_8744_54237089FE37__INCLUDED_)
#define AFX_EFFECTTRIGGERLINKUNITDESC_H__5644C87D_2017_4376_8744_54237089FE37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectTriggerUnitDesc.h"

class CEffectTriggerLinkUnitDesc : public CEffectTriggerUnitDesc  
{
public:
	CEffectTriggerLinkUnitDesc(DWORD dwTime,DWORD dwUnitNum);
	virtual ~CEffectTriggerLinkUnitDesc();

	void ParseScript(CMHFile* pFile);
	BOOL Operate(CEffect* pEffect);

};

#endif // !defined(AFX_EFFECTTRIGGERLINKUNITDESC_H__5644C87D_2017_4376_8744_54237089FE37__INCLUDED_)
