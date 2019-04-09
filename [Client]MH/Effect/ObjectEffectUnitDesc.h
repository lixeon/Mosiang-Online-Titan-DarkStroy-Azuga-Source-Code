// ObjectEffectUnitDesc.h: interface for the CObjectEffectUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTEFFECTUNITDESC_H__6FF8939C_AC05_4B6C_9D1A_2FA015530C9E__INCLUDED_)
#define AFX_OBJECTEFFECTUNITDESC_H__6FF8939C_AC05_4B6C_9D1A_2FA015530C9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectUnitDesc.h"
#include "ObjectEffectUnit.h"
#include "MemoryPoolTempl.h"

#define OPERATOR	"OPERATOR"
#define TARGET		"TARGET"

class CObjectEffectUnitDesc : public CEffectUnitDesc  
{
	static CMemoryPoolTempl<CObjectEffectUnit>* m_pUnitPool;
	
	StaticString	m_ObjectName;
	StaticString	m_ObjectNameCritical;

	VECTOR3		m_Scale;
	BOOL		m_bRepeat;
	float		m_fAlpha;
	BOOL		m_bApplyHeightField;
	
	WORD		m_StartFrame;
	WORD		m_EndFrame;
	float		m_AngleRad;
	WORD		m_Illusion;
	WORD		m_IllusionMaterial;
		
	// jsd test
	DWORD		m_dwPattern;
	VECTOR3		m_vStartAngle;
	float		m_fChangeVel;
	
public:
	CObjectEffectUnitDesc(BOOL bDangledToOperator);
	virtual ~CObjectEffectUnitDesc();

	void DoParseScript(char* szKey,CMHFile* pFile);
	virtual CEffectUnit* GetEffectUnit(EFFECTPARAM* pEffectParam);

	friend class CObjectEffectUnit;

	
	static void ReleaseUnit(CObjectEffectUnit* pUnit)
	{
		if(m_pUnitPool)
			m_pUnitPool->Free(pUnit);
	}
};

#endif // !defined(AFX_OBJECTEFFECTUNITDESC_H__6FF8939C_AC05_4B6C_9D1A_2FA015530C9E__INCLUDED_)
