// CameraEffectUnit.h: interface for the CCameraEffectUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAMERAEFFECTUNIT_H__893E1529_5038_4B04_BE57_C57A14BF06D1__INCLUDED_)
#define AFX_CAMERAEFFECTUNIT_H__893E1529_5038_4B04_BE57_C57A14BF06D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectUnit.h"

class CCameraEffectUnit : public CEffectUnit  
{

	float m_AngleX;
	float m_AngleY;
	float m_Dist;
public:	
	virtual void Initialize(CEffectUnitDesc* pEffectUnitDesc,EFFECTPARAM* pEffectParam);
	virtual void Release();

	CCameraEffectUnit();
	virtual ~CCameraEffectUnit();

	virtual BOOL On(EFFECTPARAM* pParam);
	virtual BOOL Off(EFFECTPARAM* pParam);
	virtual BOOL CameraRotate(EFFECTPARAM* pParam,float fRotateAngle,DWORD Duration);
	virtual BOOL CameraZoom(EFFECTPARAM* pParam,float fZoomDistance,DWORD Duration);
	virtual BOOL CameraAngle(EFFECTPARAM* pParam,float fTargetAngle,DWORD Duration);
	virtual BOOL ChangeCamera(EFFECTPARAM* pParam,int Camera,DWORD Duration);


	void DoProcess(DWORD tickTime,EFFECTPARAM* pParam);

	friend class CCameraEffectUnitDesc;

};

#endif // !defined(AFX_CAMERAEFFECTUNIT_H__893E1529_5038_4B04_BE57_C57A14BF06D1__INCLUDED_)
