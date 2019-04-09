// MovePoint.h: interface for the CMovePoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOVEPOINT_H__1A69E843_D8FF_478B_9549_8FFAF2ED2ED0__INCLUDED_)
#define AFX_MOVEPOINT_H__1A69E843_D8FF_478B_9549_8FFAF2ED2ED0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ENGINE\EngineEffect.h"

#define MOVEPOINT USINGTON(CMovePoint)

class CMovePoint  
{
	BOOL m_bInited;
	CEngineEffect m_PointModel;
	
public:
	CMovePoint();
//	//MAKESINGLETON(CMovePoint);

	virtual ~CMovePoint();

	BOOL Init();
	void Release();

	void SetPoint(VECTOR3* pPos);
};
EXTERNGLOBALTON(CMovePoint)
#endif // !defined(AFX_MOVEPOINT_H__1A69E843_D8FF_478B_9549_8FFAF2ED2ED0__INCLUDED_)
