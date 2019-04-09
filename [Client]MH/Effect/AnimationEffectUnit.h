// AnimationEffectUnit.h: interface for the CAnimationEffectUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANIMATIONEFFECTUNIT_H__A9B94683_505B_46EC_8B07_3CC8670408E8__INCLUDED_)
#define AFX_ANIMATIONEFFECTUNIT_H__A9B94683_505B_46EC_8B07_3CC8670408E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectUnit.h"

class CAnimationEffectUnit : public CEffectUnit  
{
public:	
	virtual void Initialize(CEffectUnitDesc* pEffectUnitDesc,EFFECTPARAM* pEffectParam);
	virtual void Release();

	CAnimationEffectUnit();
	virtual ~CAnimationEffectUnit();

	friend class CAnimationEffectUnitDesc;

	virtual void DoOn(EFFECTPARAM* pParam,TARGETSET* pObject);
	
	virtual void DoSetBaseMotion(EFFECTPARAM* pParam,TARGETSET* pObject,int BaseAnimationNum);
	
};

#endif // !defined(AFX_ANIMATIONEFFECTUNIT_H__A9B94683_505B_46EC_8B07_3CC8670408E8__INCLUDED_)
