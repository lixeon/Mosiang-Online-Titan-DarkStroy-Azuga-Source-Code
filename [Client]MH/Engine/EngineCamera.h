// EngineCamera.h: interface for the CEngineCamera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENGINECAMERA_H__8E78E49A_A4C3_49DD_AD0D_606A2F1B230E__INCLUDED_)
#define AFX_ENGINECAMERA_H__8E78E49A_A4C3_49DD_AD0D_606A2F1B230E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GraphicEngine.h"

class CEngineCamera  
{
	static VECTOR3 m_LastAngle;
	static VECTOR3 m_LastPos;
public:
	CEngineCamera();
	virtual ~CEngineCamera();

	void SetCameraPos(VECTOR3* pPos);
	void SetCameraAngle(float AngleXRad,float AngleYRad);
	void SetCameraAngle(VECTOR3* pAnglePYR);
	void SetCameraAngleForce(VECTOR3* pAnglePYR);
	
	void ResetCamera( float Distance, float fFov );

	VECTOR3* IsCollisonBetweenCamAndHero(VECTOR3* pCamPos,VECTOR3* pHeroPos,float* pDistCamToHero, float* pDistInterToHero);	//Ãæµ¹ÇÏ¸é ÃÑµ¹µÈ ÁÂÇ¥¸¦ ¾Æ´Ï¸é NULLÀ» ¸®ÅÏ
};

#endif // !defined(AFX_ENGINECAMERA_H__8E78E49A_A4C3_49DD_AD0D_606A2F1B230E__INCLUDED_)
