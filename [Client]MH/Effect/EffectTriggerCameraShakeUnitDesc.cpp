// EffectTriggerEarthQuakeUnitDesc.cpp: implementation of the CEffectTriggerCameraShakeUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectTriggerCameraShakeUnitDesc.h"

#include "..\MHCamera.h"
#include "..\MHCameraDefine.h"
#include "..\ObjectManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectTriggerCameraShakeUnitDesc::CEffectTriggerCameraShakeUnitDesc(DWORD dwTime,DWORD dwUnitNum)
:	CEffectTriggerUnitDesc(dwTime,dwUnitNum)
{
	m_ShakeNum = 1;
	m_Amplitude = 1;
}

CEffectTriggerCameraShakeUnitDesc::~CEffectTriggerCameraShakeUnitDesc()
{

}


void CEffectTriggerCameraShakeUnitDesc::ParseScript(CMHFile* pFile)
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
			CMD_CS("#SHAKE")
				char* strKind = pFile->GetString();
				m_ShakeNum = atoi(strKind);
				if(m_ShakeNum == 0)	// 글자로 써놓은 경우
				{
					if(strcmp(strKind,"EARTHQUAKE")==0)
						m_ShakeNum = eCSD_Earthquake;
					else if(strcmp(strKind,"VIBRATION")==0)
						m_ShakeNum = eCSD_Vibration;
					else
						ASSERT(0);
				}
			CMD_CS("#AMP")
				m_Amplitude = pFile->GetFloat();
		CMD_EN
	}
	
}
BOOL CEffectTriggerCameraShakeUnitDesc::Operate(CEffect* pEffect)
{	
	if((pEffect->GetEffectFlag() & EFFECT_FLAG_HEROATTACK) == FALSE)
		return TRUE;

	CAMERA->SetShaking(m_ShakeNum,m_Amplitude);
	return TRUE;
}
