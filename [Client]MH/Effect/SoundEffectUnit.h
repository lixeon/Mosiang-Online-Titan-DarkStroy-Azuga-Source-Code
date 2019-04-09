// SoundEffectUnit.h: interface for the CSoundEffectUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOUNDEFFECTUNIT_H__C958EDDB_D9BD_42A5_8B70_A92182DC5F5F__INCLUDED_)
#define AFX_SOUNDEFFECTUNIT_H__C958EDDB_D9BD_42A5_8B70_A92182DC5F5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectUnit.h"

class CSoundEffectUnit : public CEffectUnit  
{


public:
	virtual void Initialize(CEffectUnitDesc* pEffectUnitDesc,EFFECTPARAM* pEffectParam);
	virtual void Release();

	CSoundEffectUnit();
	virtual ~CSoundEffectUnit();

	virtual void DoOn(EFFECTPARAM* pParam,TARGETSET* pObject);
	virtual void DoOff(EFFECTPARAM* pParam,TARGETSET* pObject);

	void DoProcess(DWORD tickTime,EFFECTPARAM* pParam);

	friend class CSoundEffectUnitDesc;

};

#endif // !defined(AFX_SOUNDEFFECTUNIT_H__C958EDDB_D9BD_42A5_8B70_A92182DC5F5F__INCLUDED_)
