// EffectTriggerUnitDesc.h: interface for the CEffectTriggerUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFECTTRIGGERUNITDESC_H__56966651_1DAA_4B59_8781_32D8849DFFC6__INCLUDED_)
#define AFX_EFFECTTRIGGERUNITDESC_H__56966651_1DAA_4B59_8781_32D8849DFFC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\MHFile.h"
#include "Effect.h"
#include "EffectUnit.h"

class CEffectTriggerUnitDesc  
{
protected:
	DWORD m_dwTime;
	DWORD m_dwUnitNum;
public:
	CEffectTriggerUnitDesc(DWORD dwTime,DWORD dwUnitNum);
	virtual ~CEffectTriggerUnitDesc();

	BOOL Check(DWORD ElapsedTime);
	virtual void ParseScript(CMHFile* pFile) = 0;
	virtual BOOL Operate(CEffect* pEffect) = 0;
	
	inline DWORD GetStartTime()	{	return m_dwTime;	}
	inline DWORD GetUnitNum()	{	return m_dwUnitNum;	}
};

#endif // !defined(AFX_EFFECTTRIGGERUNITDESC_H__56966651_1DAA_4B59_8781_32D8849DFFC6__INCLUDED_)
