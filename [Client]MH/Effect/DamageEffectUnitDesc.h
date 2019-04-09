// DamageEffectUnitDesc.h: interface for the CDamageEffectUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DAMAGEEFFECTUNITDESC_H__8553F81D_6DC6_465D_BA17_1E036095A8C1__INCLUDED_)
#define AFX_DAMAGEEFFECTUNITDESC_H__8553F81D_6DC6_465D_BA17_1E036095A8C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectUnitDesc.h"
#include "MemoryPoolTempl.h"
#include "DamageEffectUnit.h"

class CDamageEffectUnitDesc : public CEffectUnitDesc  
{	
	static CMemoryPoolTempl<CDamageEffectUnit>* m_pUnitPool;

	float m_DamageRate;	// 0~1 »çÀÌ [3/17/2003]
	DWORD m_FadeDuration;
	BYTE m_DamageKind;

	float m_MinRangeAttackDist;
	float m_MaxRangeAttackDist;
	DWORD m_RangeAttackDuration;
	VECTOR3 m_RangeAttackPivot;

public:
	CDamageEffectUnitDesc(BOOL bDangledToOperator);
	virtual ~CDamageEffectUnitDesc();

	void DoParseScript(char* szKey,CMHFile* pFile);
	virtual CEffectUnit* GetEffectUnit(EFFECTPARAM* pEffectParam);

	friend class CDamageEffectUnit;
	
	float GetDamageRate()		{	return m_DamageRate;	}

	static void ReleaseUnit(CDamageEffectUnit* pUnit)
	{
		if(m_pUnitPool)
			m_pUnitPool->Free(pUnit);
	}
};

#endif // !defined(AFX_DAMAGEEFFECTUNITDESC_H__8553F81D_6DC6_465D_BA17_1E036095A8C1__INCLUDED_)
