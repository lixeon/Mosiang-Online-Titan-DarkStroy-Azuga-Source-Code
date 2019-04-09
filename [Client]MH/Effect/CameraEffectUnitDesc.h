// CameraEffectUnitDesc.h: interface for the CCameraEffectUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAMERAEFFECTUNITDESC_H__715728C5_B458_4B63_AB84_098569A6C1D2__INCLUDED_)
#define AFX_CAMERAEFFECTUNITDESC_H__715728C5_B458_4B63_AB84_098569A6C1D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectUnitDesc.h"
#include "MemoryPoolTempl.h"
#include "CameraEffectUnit.h"

class CCameraEffectUnitDesc : public CEffectUnitDesc  
{
	static CMemoryPoolTempl<CCameraEffectUnit>* m_pUnitPool;
	
	int m_CameraNum;
	float m_InitAngleX;
	float m_InitAngleY;
	float m_InitDistance;
	
public:
	CCameraEffectUnitDesc(BOOL bDangledToOperator);
	virtual ~CCameraEffectUnitDesc();

	void DoParseScript(char* szKey,CMHFile* pFile);
	virtual CEffectUnit* GetEffectUnit(EFFECTPARAM* pEffectParam);

	friend class CCameraEffectUnit;

	static void ReleaseUnit(CCameraEffectUnit* pUnit)
	{
		if(m_pUnitPool)
			m_pUnitPool->Free(pUnit);
	}
};

#endif // !defined(AFX_CAMERAEFFECTUNITDESC_H__715728C5_B458_4B63_AB84_098569A6C1D2__INCLUDED_)
