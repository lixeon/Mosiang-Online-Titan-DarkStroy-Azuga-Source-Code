// EffectTriggerGravityMoveUnitDesc.cpp: implementation of the CEffectTriggerGravityMoveUnitDesc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectTriggerGravityMoveUnitDesc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL EFFECTPOSITIONDESC::GetEffectPos(CObject* pTarget,CObject* pOperator,MAINTARGET* pMainTarget,VECTOR3* pRtPos)
{
	VECTOR3 basisPos;
	VECTOR3 maintargetpos;
	basisPos.x = 0;
	basisPos.y = 0;
	basisPos.z = 0;
	float fRot = 0;

	CObject* pMainTargetObj = NULL;
	BOOL bMainTargetRt = GetMainTargetPos(pMainTarget,&maintargetpos,&pMainTargetObj);
	if(m_PositionBasis == 0)
	{
		if(pTarget->IsInited() == FALSE)
			return FALSE;
		pTarget->GetPosition(&basisPos);
	}
	else if(m_PositionBasis == 1)
	{
		if(pOperator->IsInited() == FALSE)
			return FALSE;
		pOperator->GetPosition(&basisPos);
	}
	else if(m_PositionBasis == 2)
	{
		if(bMainTargetRt == FALSE)
			return FALSE;
		basisPos = maintargetpos;
	}
	
	if(m_RotateBasis == 0)
		fRot = pTarget->GetAngle();
	else if(m_RotateBasis == 1)
		fRot = pOperator->GetAngle();
	else if(m_RotateBasis == 2)
		fRot = pMainTargetObj ? pMainTargetObj->GetAngle() : 0;

	TransToRelatedCoordinate(pRtPos,&m_Pos,fRot);

	
	*pRtPos = *pRtPos + basisPos;

	return TRUE;
}

// 임시 -_-;
BOOL IsVillage()	{ return 0;}

void CEffectTriggerGravityMoveUnitDesc::CalcGravityMoveInfo(CObject* pTarget,CObject* pOperator,MAINTARGET* pMainTarget,GRAVITYMOVEINFO* pInfo)
{
	m_PosFrom.GetEffectPos(pTarget,pOperator,pMainTarget, &pInfo->m_InitPos);
	m_Velocity.GetEffectPos(pTarget,pOperator,pMainTarget, &pInfo->m_Velocity);
	pInfo->m_Target = m_PosTo;
	pInfo->m_pTargetObj = pTarget;

	pInfo->m_RemainTime = m_Duration*0.001f;
}


CEffectTriggerGravityMoveUnitDesc::CEffectTriggerGravityMoveUnitDesc(DWORD dwTime,DWORD dwUnitNum)
:	CEffectTriggerUnitDesc(dwTime,dwUnitNum)
{
	m_bTurn = TRUE;

}

CEffectTriggerGravityMoveUnitDesc::~CEffectTriggerGravityMoveUnitDesc()
{

}

void CEffectTriggerGravityMoveUnitDesc::ParseScript(CMHFile* pFile)
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
			CMD_CS("#POSFROM")				
				m_PosFrom.m_PositionBasis = pFile->GetByte();
				m_PosFrom.m_RotateBasis = pFile->GetByte();

				m_PosFrom.m_Pos.x = pFile->GetFloat();
				m_PosFrom.m_Pos.y = pFile->GetFloat();
				m_PosFrom.m_Pos.z = pFile->GetFloat();
				
			CMD_CS("#POSTO")				
				m_PosTo.m_PositionBasis = pFile->GetByte();
				m_PosTo.m_RotateBasis = pFile->GetByte();

				m_PosTo.m_Pos.x = pFile->GetFloat();
				m_PosTo.m_Pos.y = pFile->GetFloat();
				m_PosTo.m_Pos.z = pFile->GetFloat();
				
			CMD_CS("#VELOCITY")				
				m_Velocity.m_PositionBasis = pFile->GetByte();
				m_Velocity.m_PositionBasis = 3;
				m_Velocity.m_RotateBasis = pFile->GetByte();

				m_Velocity.m_Pos.x = pFile->GetFloat();
				m_Velocity.m_Pos.y = pFile->GetFloat();
				m_Velocity.m_Pos.z = pFile->GetFloat();

			CMD_CS("#DURATION")	
				m_Duration = pFile->GetDword();
			
			CMD_CS("#TURN")
				m_bTurn = pFile->GetBool();
		CMD_EN
	}
}
BOOL CEffectTriggerGravityMoveUnitDesc::Operate(CEffect* pEffect)
{	
	return pEffect->GetEffectUnit(m_dwUnitNum)->GravityMove(pEffect->GetEffectParam(),this);
}
