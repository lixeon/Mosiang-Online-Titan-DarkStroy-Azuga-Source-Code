// MoveEffectUnit.h: interface for the CMoveEffectUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOVEEFFECTUNIT_H__B8BF0948_4D58_4CAB_8025_BEC7BDC968C7__INCLUDED_)
#define AFX_MOVEEFFECTUNIT_H__B8BF0948_4D58_4CAB_8025_BEC7BDC968C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectUnit.h"

class CMoveEffectUnit : public CEffectUnit  
{
protected:

public:
	virtual void Initialize(CEffectUnitDesc* pEffectUnitDesc,EFFECTPARAM* pEffectParam);
	virtual void Release();
	
	CMoveEffectUnit();
	virtual ~CMoveEffectUnit();

	friend class CMoveEffectUnitDesc;

	virtual void DoOn(EFFECTPARAM* pParam,TARGETSET* pObject);
	virtual void DoOff(EFFECTPARAM* pParam,TARGETSET* pObject);

	void DoProcess(DWORD tickTime,EFFECTPARAM* pParam);
};

#endif // !defined(AFX_MOVEEFFECTUNIT_H__B8BF0948_4D58_4CAB_8025_BEC7BDC968C7__INCLUDED_)
