// EffectTriggerMoveUnitDesc.cpp: implementation of the CEffectTriggerMoveUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectTriggerMoveUnitDesc.h"
#include "EffectUnit.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectTriggerMoveUnitDesc::CEffectTriggerMoveUnitDesc(DWORD dwTime,DWORD dwUnitNum)
:	CEffectTriggerUnitDesc(dwTime,dwUnitNum)
{
	m_ToPosition.x = 0;
	m_ToPosition.y = 0;
	m_ToPosition.z = 0;
	m_ToRandom.x = 0;
	m_ToRandom.y = 0;
	m_ToRandom.z = 0;
	m_MoveTime = 0;
	m_bMainTarget = 0;
	m_bTurn = 0;
}

CEffectTriggerMoveUnitDesc::~CEffectTriggerMoveUnitDesc()
{

}


void CEffectTriggerMoveUnitDesc::ParseScript(CMHFile* pFile)
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
			CMD_CS("#POSITION")
				m_ToPosition.x = pFile->GetFloat();
				m_ToPosition.y = pFile->GetFloat();
				m_ToPosition.z = pFile->GetFloat();
			CMD_CS("#RANDOM")
				m_ToRandom.x = pFile->GetFloat();
				m_ToRandom.y = pFile->GetFloat();
				m_ToRandom.z = pFile->GetFloat();
			CMD_CS("#DURATION")
				m_MoveTime = pFile->GetDword();
			CMD_CS("#TOMAINTARGET")
				m_bMainTarget = pFile->GetByte();
			CMD_CS("#TURN")
				m_bTurn = pFile->GetBool();
			
		CMD_EN
	}
}
BOOL CEffectTriggerMoveUnitDesc::Operate(CEffect* pEffect)
{	
	return pEffect->GetEffectUnit(m_dwUnitNum)->Move(pEffect->GetEffectParam(),this);
}
