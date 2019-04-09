// EffectTriggerSetBaseMotionUnitDesc.cpp: implementation of the CEffectTriggerSetBaseMotionUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectTriggerSetBaseMotionUnitDesc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectTriggerSetBaseMotionUnitDesc::CEffectTriggerSetBaseMotionUnitDesc(DWORD dwTime,DWORD dwUnitNum)
:	CEffectTriggerUnitDesc(dwTime,dwUnitNum)
{
}

CEffectTriggerSetBaseMotionUnitDesc::~CEffectTriggerSetBaseMotionUnitDesc()
{

}


void CEffectTriggerSetBaseMotionUnitDesc::ParseScript(CMHFile* pFile)
{
	char buf[128];
	pFile->GetString(buf);	// skip  '{'
	if(buf[0] != '{')
	{
		ERRORBSGBOX("ERROR[%s]!! { } 가 필요한 트리거입니다!!!",pFile->GetFileName());
		return;
	}

	while(1)
	{
		pFile->GetString(buf);
		CMD_ST(buf)
			CMD_CS("}")
				break;
			CMD_CS("#BASEMOTION")
				m_BaseMotionNum = pFile->GetInt();
		CMD_EN
	}
}
BOOL CEffectTriggerSetBaseMotionUnitDesc::Operate(CEffect* pEffect)
{	
	return pEffect->GetEffectUnit(m_dwUnitNum)->SetBaseMotion(pEffect->GetEffectParam(),m_BaseMotionNum);
}
