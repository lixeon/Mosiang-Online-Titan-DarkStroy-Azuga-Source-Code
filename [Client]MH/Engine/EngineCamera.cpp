// EngineCamera.cpp: implementation of the CEngineCamera class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EngineCamera.h"
#include "GraphicEngine.h"
#include "MHMap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VECTOR3 CEngineCamera::m_LastAngle;
VECTOR3 CEngineCamera::m_LastPos;
VECTOR3 gCamPos;

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
	gCamPos = *pPos;
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

void CEngineCamera::SetCameraAngleForce(VECTOR3* pAnglePYR)
{
	m_LastAngle = *pAnglePYR;

	g_pExecutive->GetGeometry()->SetCameraAngleRad(pAnglePYR,0);
}

VECTOR3* CEngineCamera::IsCollisonBetweenCamAndHero(VECTOR3* pCamPos,VECTOR3* pHeroPos, float* pDistCamToHero, float* pDistInterToHero)
{
	static VECTOR3 rtPos;
	VECTOR3 IntersectedPos;
	VECTOR3 dir = *pCamPos - *pHeroPos;

	if(MAP->GetMapDesc()->bFixHeight)
	{
		if(GetCollisonPointWithRay(*pHeroPos,*pCamPos,MAP->GetMapDesc()->FixHeight,IntersectedPos) == FALSE)
			return NULL;
	}
	else
	{
		float fDist;
		if(g_pExecutive->GXMGetHFieldCollisionPointWithRay(&IntersectedPos,&fDist,pHeroPos,&dir) == FALSE)
			return NULL;
	}
/*
	float dist1 = VECTOR3Length(&dir);
	float dist2 = CalcDistance(&IntersectedPos,pHeroPos);
	if(dist2 > dist1)
		return NULL;
*/
	*pDistCamToHero = VECTOR3Length(&dir);
	*pDistInterToHero = CalcDistance(&IntersectedPos,pHeroPos);

	if(*pDistInterToHero > *pDistCamToHero)
		return NULL;

	rtPos = IntersectedPos;

	return &rtPos;
}

void  CEngineCamera::ResetCamera( float Distance, float fFov )
{
	g_pExecutive->GetGeometry()->ResetCamera( &m_LastPos, CAMERA_NEAR, Distance, fFov, 0 );
	g_pExecutive->GetGeometry()->SetCameraAngleRad( &m_LastAngle, 0 );
}