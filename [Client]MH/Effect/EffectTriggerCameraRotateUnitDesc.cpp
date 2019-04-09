// EffectTriggerCameraRoateUnitDesc.cpp: implementation of the CEffectTriggerCameraRotateUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectTriggerCameraRotateUnitDesc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectTriggerCameraRotateUnitDesc::CEffectTriggerCameraRotateUnitDesc(DWORD dwTime,DWORD dwUnitNum)
:	CEffectTriggerUnitDesc(dwTime,dwUnitNum)
{
}

CEffectTriggerCameraRotateUnitDesc::~CEffectTriggerCameraRotateUnitDesc()
{

}


void CEffectTriggerCameraRotateUnitDesc::ParseScript(CMHFile* pFile)
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
			CMD_CS("#ROTATEANGLE")
				m_RotateAngle = pFile->GetFloat();
			CMD_CS("#DURATION")
				m_Duration = pFile->GetDword();
		CMD_EN
	}
}

BOOL CEffectTriggerCameraRotateUnitDesc::Operate(CEffect* pEffect)
{
	return pEffect->GetEffectUnit(m_dwUnitNum)->CameraRotate(pEffect->GetEffectParam(),m_RotateAngle,m_Duration);
}
