// AnimationEffectUnitDesc.h: interface for the CAnimationEffectUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANIMATIONEFFECTUNITDESC_H__A25780B4_06C7_4D64_B85D_347638674D30__INCLUDED_)
#define AFX_ANIMATIONEFFECTUNITDESC_H__A25780B4_06C7_4D64_B85D_347638674D30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectUnitDesc.h"
#include "MemoryPoolTempl.h"
#include "AnimationEffectUnit.h"

class CAnimationEffectUnitDesc : public CEffectUnitDesc  
{
	static CMemoryPoolTempl<CAnimationEffectUnit>* m_pUnitPool;
	DWORD m_MotionNum;
	BOOL m_bRepeat;

	BOOL m_CustumMotion;
	int m_StartFrame;
	int m_EndFrame;

public:
	CAnimationEffectUnitDesc(BOOL bDangledToOperator);
	virtual ~CAnimationEffectUnitDesc();

	void DoParseScript(char* szKey,CMHFile* pFile);
	virtual CEffectUnit* GetEffectUnit(EFFECTPARAM* pEffectParam);

	inline DWORD GetMotionNum()		{	return m_MotionNum;	}

	friend class CAnimationEffectUnit;

	static void ReleaseUnit(CAnimationEffectUnit* pUnit)
	{
		if(m_pUnitPool)
			m_pUnitPool->Free(pUnit);
	}
};

#endif // !defined(AFX_ANIMATIONEFFECTUNITDESC_H__A25780B4_06C7_4D64_B85D_347638674D30__INCLUDED_)
