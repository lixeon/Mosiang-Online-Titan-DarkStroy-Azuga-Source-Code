// EngineCamera.cpp: implementation of the CEngineCamera class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EngineCamera.h"
#include "GraphicEngine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VECTOR3 CEngineCamera::m_LastAngle;
VECTOR3 CEngineCamera::m_LastPos;

CEngineCamera::CEngineCamera()
{

}

CEngineCamera::~CEngineCamera()
{

}

void CEngineCamera::SetCameraPos(VECTOR3* pPos)
{
	if( pPos->x == m_LastPos.x &&
		pPos->y == m_LastPos.y &&
		pPos->z == m_LastPos.z)
		return;
	m_LastPos = *pPos;
	g_pExecutive->GetGeometry()->SetCameraPos(pPos,0);
}

void CEngineCamera::SetCameraAngle(float AngleXRad,float AngleYRad)
{
	if( AngleXRad == m_LastAngle.x &&
		AngleYRad == m_LastAngle.y)
		return;

	VECTOR3 angle;
	angle.x = AngleXRad;
	angle.y = AngleYRad;
	angle.z = 0;
	SetCameraAngle(&angle);
}

void CEngineCamera::SetCameraAngle(VECTOR3* pAnglePYR)
{
	if( pAnglePYR->x == m_LastAngle.x &&
		pAnglePYR->y == m_LastAngle.y &&
		pAnglePYR->z == m_LastAngle.z)
		return;
	m_LastAngle = *pAnglePYR;

	g_pExecutive->GetGeometry()->SetCameraAngleRad(pAnglePYR,0);
}
