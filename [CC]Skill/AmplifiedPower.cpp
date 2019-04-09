// AmplifiedPower.cpp: implementation of the CAmplifiedPower class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AmplifiedPower.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAmplifiedPowerPhy::CAmplifiedPowerPhy(CObject* pParent)
: CSpecialState(pParent)
{

}

CAmplifiedPowerPhy::~CAmplifiedPowerPhy()
{

}

WORD CAmplifiedPowerPhy::GetParam1()
{
	WORD power = CSpecialState::GetParam1();
	if(power)
	{
		EndState();
	}
	return power;
}


CAmplifiedPowerAtt::CAmplifiedPowerAtt(CObject* pParent)
: CSpecialState(pParent)
{

}

CAmplifiedPowerAtt::~CAmplifiedPowerAtt()
{

}
