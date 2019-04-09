// EffectTriggerAttachUnitDesc.h: interface for the CEffectTriggerAttachUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFECTTRIGGERATTACHUNITDESC_H__EF3BFA90_B769_432B_B47D_2D53F8EE7075__INCLUDED_)
#define AFX_EFFECTTRIGGERATTACHUNITDESC_H__EF3BFA90_B769_432B_B47D_2D53F8EE7075__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectTriggerUnitDesc.h"

class CEffectTriggerAttachUnitDesc : public CEffectTriggerUnitDesc  
{
	StaticString m_AttachPartName;
public:
	CEffectTriggerAttachUnitDesc(DWORD dwTime,DWORD dwUnitNum);
	virtual ~CEffectTriggerAttachUnitDesc();

	void ParseScript(CMHFile* pFile);
	BOOL Operate(CEffect* pEffect);

};

#endif // !defined(AFX_EFFECTTRIGGERATTACHUNITDESC_H__EF3BFA90_B769_432B_B47D_2D53F8EE7075__INCLUDED_)
