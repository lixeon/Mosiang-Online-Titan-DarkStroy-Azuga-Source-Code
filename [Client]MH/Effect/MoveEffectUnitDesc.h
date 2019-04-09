// MoveEffectUnitDesc.h: interface for the CMoveEffectUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOVEEFFECTUNITDESC_H__26550DE5_B70F_4E89_9212_4FBEE94D4B7F__INCLUDED_)
#define AFX_MOVEEFFECTUNITDESC_H__26550DE5_B70F_4E89_9212_4FBEE94D4B7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectUnitDesc.h"
#include "..\MHFile.h"
#include "MemoryPoolTempl.h"
#include "MoveEffectUnit.h"

class CMoveEffectUnitDesc : public CEffectUnitDesc  
{
	static CMemoryPoolTempl<CMoveEffectUnit>* m_pUnitPool;

	DWORD m_Duration;
	BOOL m_bApplyWeight;
	BOOL m_bYMove;
	BOOL m_bTurnChar;
public:
	CMoveEffectUnitDesc(BOOL bDangledToOperator);
	virtual ~CMoveEffectUnitDesc();

	void DoParseScript(char* szKey,CMHFile* pFile);
	virtual CEffectUnit* GetEffectUnit(EFFECTPARAM* pEffectParam);

	friend class CMoveEffectUnit;
	
	static void ReleaseUnit(CMoveEffectUnit* pUnit)
	{
		if(m_pUnitPool)
			m_pUnitPool->Free(pUnit);
	}
};

#endif // !defined(AFX_MOVEEFFECTUNITDESC_H__26550DE5_B70F_4E89_9212_4FBEE94D4B7F__INCLUDED_)
