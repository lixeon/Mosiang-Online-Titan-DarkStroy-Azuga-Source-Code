// EffectTriggerAngleUnitDesc.cpp: implementation of the CEffectTriggerCameraAngleUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectTriggerCameraAngleUnitDesc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectTriggerCameraAngleUnitDesc::CEffectTriggerCameraAngleUnitDesc(DWORD dwTime,DWORD dwUnitNum)
:	CEffectTriggerUnitDesc(dwTime,dwUnitNum)
{
}

CEffectTriggerCameraAngleUnitDesc::~CEffectTriggerCameraAngleUnitDesc()
{

}


void CEffectTriggerCameraAngleUnitDesc::ParseScript(CMHFile* pFile)
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
			CMD_CS("#TARGETANGLE")
				m_TargetAngle = pFile->GetFloat();
			CMD_CS("#DURATION")
				m_Duration = pFile->GetDword();
		CMD_EN
	}
}

BOOL CEffectTriggerCameraAngleUnitDesc::Operate(CEffect* pEffect)
{
	return pEffect->GetEffectUnit(m_dwUnitNum)->CameraAngle(pEffect->GetEffectParam(),m_TargetAngle,m_Duration);
}

