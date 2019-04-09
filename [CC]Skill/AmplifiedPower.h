// AmplifiedPower.h: interface for the CAmplifiedPower class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AMPLIFIEDPOWER_H__14CF367F_7C39_4429_B52D_D490714A9154__INCLUDED_)
#define AFX_AMPLIFIEDPOWER_H__14CF367F_7C39_4429_B52D_D490714A9154__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SpecialState.h"

class CAmplifiedPowerPhy : public CSpecialState
{
public:
	CAmplifiedPowerPhy(CObject* pParent);
	~CAmplifiedPowerPhy();

	virtual WORD GetParam1();
};

class CAmplifiedPowerAtt : public CSpecialState
{
public:
	CAmplifiedPowerAtt(CObject* pParent);
	~CAmplifiedPowerAtt();

};


#endif // !defined(AFX_AMPLIFIEDPOWER_H__14CF367F_7C39_4429_B52D_D490714A9154__INCLUDED_)
