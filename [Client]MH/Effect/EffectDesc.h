// EffectDesc.h: interface for the CEffectDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFECTDESC_H__CF7C295E_CC07_4B6B_9FB0_7F7B75076E3E__INCLUDED_)
#define AFX_EFFECTDESC_H__CF7C295E_CC07_4B6B_9FB0_7F7B75076E3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEffectUnitDesc;
class CEffectTriggerUnitDesc;
class CMHFile;
#include "Effect.h"

class CEngineObject;

class CEffectDesc  
{
	int m_EffectKind;
	StaticString m_FileName;

	DWORD m_MaxEffectUnitDesc;
	DWORD m_MaxEffectTriggerDesc;
	DWORD m_EffectEndTime;
	CEffectUnitDesc** m_ppEffectUnitDescArray;
	CEffectTriggerUnitDesc** m_ppEffectTriggerUnitDesc;
	
	BOOL m_bRepeat;
	DWORD m_NextEffect;
	
	DWORD m_OperatorAnimationTime;

public:
	CEffectDesc();
	virtual ~CEffectDesc();

	BOOL LoadEffectDesc(int EffectKind,CMHFile* pFile);


	BOOL GetEffect(EFFECTPARAM* pParam,CEffect* pOutEffect);

	DWORD GetEffectEndTime()	{	return m_EffectEndTime;	}
		
	int GetMaxEffectUnitDesc()		{	return m_MaxEffectUnitDesc;	}
	int GetMaxEffectTriggerDesc()	{	return m_MaxEffectTriggerDesc;	}

	inline int GetEffectKind()	{	return m_EffectKind;	}

	inline char* GetFileName()	{	return m_FileName;	}

	friend class CEffect;

	DWORD GetOperatorAnimatioEndTime(CEngineObject* pEngineObject);
};

#endif // !defined(AFX_EFFECTDESC_H__CF7C295E_CC07_4B6B_9FB0_7F7B75076E3E__INCLUDED_)
