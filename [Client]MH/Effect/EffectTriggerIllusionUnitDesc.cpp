// EffectTriggerIllusionUnitDesc.cpp: implementation of the CEffectTriggerIllusionUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectTriggerIllusionUnitDesc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectTriggerIllusionUnitDesc::CEffectTriggerIllusionUnitDesc(DWORD dwTime,DWORD dwUnitNum)
:	CEffectTriggerUnitDesc(dwTime,dwUnitNum)
{
	m_TargetKind = eTargetKind_Weapon;
	m_SwitchKind = eSwitchKind_End;
	m_IllusionFrame = 3;
	m_MaterialNum = 0;
}

CEffectTriggerIllusionUnitDesc::~CEffectTriggerIllusionUnitDesc()
{

}


void CEffectTriggerIllusionUnitDesc::ParseScript(CMHFile* pFile)
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
			CMD_CS("#TARGET")
				pFile->GetString(buf);
				CMD_ST(buf)
					CMD_CS("WEAPON")
						m_TargetKind = eTargetKind_Weapon;
				CMD_EN

			CMD_CS("#MATERIAL")
				m_MaterialNum = pFile->GetWord();
			
			CMD_CS("#SWITCH")
				pFile->GetString(buf);
				CMD_ST(buf)
					CMD_CS("START")
						m_SwitchKind = eSwitchKind_Start;
					CMD_CS("END")
						m_SwitchKind = eSwitchKind_End;
				CMD_EN

			CMD_CS("#FRAME")
				m_IllusionFrame = pFile->GetDword();

		CMD_EN
	}
}
BOOL CEffectTriggerIllusionUnitDesc::Operate(CEffect* pEffect)
{
	CObject* pOperator = pEffect->GetEffectParam()->m_pOperator;
	if(pOperator->IsInited() == FALSE)
		return TRUE;

	void* pMtl = EFFECTMGR->GetIllusionMaterial(m_MaterialNum);

	ySWITCH(m_SwitchKind)
		yCASE(eSwitchKind_Start)
			pOperator->GetEngineObject()->StartWeaponIllusion(m_IllusionFrame,pMtl);
		yCASE(eSwitchKind_End)
			pOperator->GetEngineObject()->EndWeaponIllusion();

	yENDSWITCH

	return TRUE;
}

