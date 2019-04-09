// EffectTriggerFadeOutUnitDesc.cpp: implementation of the CEffectTriggerFadeOutUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectTriggerFadeOutUnitDesc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectTriggerFadeOutUnitDesc::CEffectTriggerFadeOutUnitDesc(DWORD dwTime,DWORD dwUnitNum)
:	CEffectTriggerUnitDesc(dwTime,dwUnitNum)
{
	m_FadeStart = 1;
	m_FadeEnd = 0;
}

CEffectTriggerFadeOutUnitDesc::~CEffectTriggerFadeOutUnitDesc()
{

}

void CEffectTriggerFadeOutUnitDesc::ParseScript(CMHFile* pFile)
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
			CMD_CS("#DURATION")
				m_FadeTime = pFile->GetDword();
			CMD_CS("#START")
				m_FadeStart = pFile->GetFloat();
			CMD_CS("#END")
				m_FadeEnd = pFile->GetFloat();
		CMD_EN
	}
}

BOOL CEffectTriggerFadeOutUnitDesc::Operate(CEffect* pEffect)
{
	return pEffect->GetEffectUnit(m_dwUnitNum)->Fade(pEffect->GetEffectParam(),m_FadeTime,m_FadeStart,m_FadeEnd);
}


