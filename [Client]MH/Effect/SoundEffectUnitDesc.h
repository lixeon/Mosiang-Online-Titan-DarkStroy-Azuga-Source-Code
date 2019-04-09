// SoundEffectUnitDesc.h: interface for the CSoundEffectUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUNDEFFECTUNITDESC_H__D8995C1E_D9EA_4B63_B31F_1207DE1F47FA__INCLUDED_)
#define AFX_SOUNDEFFECTUNITDESC_H__D8995C1E_D9EA_4B63_B31F_1207DE1F47FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectUnitDesc.h"
#include "MemoryPoolTempl.h"
#include "SoundEffectUnit.h"

class CSoundEffectUnitDesc : public CEffectUnitDesc  
{
	static CMemoryPoolTempl<CSoundEffectUnit>* m_pUnitPool;
	WORD m_SoundNum;
public:
	CSoundEffectUnitDesc(BOOL bDangledToOperator);
	virtual ~CSoundEffectUnitDesc();

	void DoParseScript(char* szKey,CMHFile* pFile);
	virtual CEffectUnit* GetEffectUnit(EFFECTPARAM* pEffectParam);

	friend class CSoundEffectUnit;

	
	static void ReleaseUnit(CSoundEffectUnit* pUnit)
	{
		if(m_pUnitPool)
			m_pUnitPool->Free(pUnit);
	}

};

#endif // !defined(AFX_SOUNDEFFECTUNITDESC_H__D8995C1E_D9EA_4B63_B31F_1207DE1F47FA__INCLUDED_)
