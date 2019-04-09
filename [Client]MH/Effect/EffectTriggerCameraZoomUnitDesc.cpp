// EffectTriggerCameraZoomUnitDesc.cpp: implementation of the CEffectTriggerCameraZoomUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectTriggerCameraZoomUnitDesc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectTriggerCameraZoomUnitDesc::CEffectTriggerCameraZoomUnitDesc(DWORD dwTime,DWORD dwUnitNum)
:	CEffectTriggerUnitDesc(dwTime,dwUnitNum)
{
}

CEffectTriggerCameraZoomUnitDesc::~CEffectTriggerCameraZoomUnitDesc()
{

}


void CEffectTriggerCameraZoomUnitDesc::ParseScript(CMHFile* pFile)
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
			CMD_CS("#ZOOMDISTANCE")
				m_fZoomDistance = pFile->GetFloat();
			CMD_CS("#DURATION")
				m_Duration = pFile->GetDword();
		CMD_EN
	}
}

BOOL CEffectTriggerCameraZoomUnitDesc::Operate(CEffect* pEffect)
{
	return pEffect->GetEffectUnit(m_dwUnitNum)->CameraZoom(pEffect->GetEffectParam(),m_fZoomDistance,m_Duration);
}
