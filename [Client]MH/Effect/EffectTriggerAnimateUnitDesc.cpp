// EffectTriggerAnimateUnitDesc.cpp: implementation of the CEffectTriggerAnimateUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectTriggerAnimateUnitDesc.h"
#include "EffectUnit.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectTriggerAnimateUnitDesc::CEffectTriggerAnimateUnitDesc(DWORD dwTime,DWORD dwUnitNum)
:	CEffectTriggerUnitDesc(dwTime,dwUnitNum)
{
	m_Motion = 1;
	m_bRepeat = FALSE;
}

CEffectTriggerAnimateUnitDesc::~CEffectTriggerAnimateUnitDesc()
{

}


void CEffectTriggerAnimateUnitDesc::ParseScript(CMHFile* pFile)
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
			CMD_CS("#MOTION")
				m_Motion = pFile->GetInt();
			CMD_CS("#REPEAT")
				m_bRepeat = pFile->GetBool();
		CMD_EN
	}
}
BOOL CEffectTriggerAnimateUnitDesc::Operate(CEffect* pEffect)
{	
	return pEffect->GetEffectUnit(m_dwUnitNum)->Animate(pEffect->GetEffectParam(),m_Motion,m_bRepeat);
}

