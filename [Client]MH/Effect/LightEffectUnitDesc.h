// LightEffectUnitDesc.h: interface for the CLightEffectUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHTEFFECTUNITDESC_H__952918BB_4FCE_447B_A7C8_B2395357CBAC__INCLUDED_)
#define AFX_LIGHTEFFECTUNITDESC_H__952918BB_4FCE_447B_A7C8_B2395357CBAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectUnitDesc.h"
#include "MemoryPoolTempl.h"
#include "LightEffectUnit.h"

class CLightEffectUnitDesc : public CEffectUnitDesc  
{
	static CMemoryPoolTempl<CLightEffectUnit>* m_pUnitPool;
	DWORD		m_ColorIndex;
	float		m_fRadius;
	
public:
	CLightEffectUnitDesc(BOOL bDangledToOperator);
	virtual ~CLightEffectUnitDesc();

	void DoParseScript(char* szKey,CMHFile* pFile);
	virtual CEffectUnit* GetEffectUnit(EFFECTPARAM* pEffectParam);

	friend class CLightEffectUnit;
	
	static void ReleaseUnit(CLightEffectUnit* pUnit)
	{
		if(m_pUnitPool)
			m_pUnitPool->Free(pUnit);
	}
};

#endif // !defined(AFX_LIGHTEFFECTUNITDESC_H__952918BB_4FCE_447B_A7C8_B2395357CBAC__INCLUDED_)
