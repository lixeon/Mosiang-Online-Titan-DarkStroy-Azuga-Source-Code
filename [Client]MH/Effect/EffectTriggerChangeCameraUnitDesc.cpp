// EffectTriggerChangeCameraUnitDesc.cpp: implementation of the CEffectTriggerChangeCameraUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectTriggerChangeCameraUnitDesc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectTriggerChangeCameraUnitDesc::CEffectTriggerChangeCameraUnitDesc(DWORD dwTime,DWORD dwUnitNum)
:	CEffectTriggerUnitDesc(dwTime,dwUnitNum)
{
	m_ToCamera = -1;
}

CEffectTriggerChangeCameraUnitDesc::~CEffectTriggerChangeCameraUnitDesc()
{

}


void CEffectTriggerChangeCameraUnitDesc::ParseScript(CMHFile* pFile)
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
			CMD_CS("#CAMERA")
				m_ToCamera = pFile->GetInt();
			CMD_CS("#DURATION")
				m_Duration = pFile->GetDword();
		CMD_EN
	}
}
BOOL CEffectTriggerChangeCameraUnitDesc::Operate(CEffect* pEffect)
{
	ASSERT(m_ToCamera != -1);
	return pEffect->GetEffectUnit(m_dwUnitNum)->ChangeCamera(pEffect->GetEffectParam(),m_ToCamera,m_Duration);
}
