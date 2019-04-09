// EffectUnit.h: interface for the CEffectUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EFFECTUNIT_H__32A18E60_BD08_46D0_ACC2_D2A61E3409FC__INCLUDED_)
#define AFX_EFFECTUNIT_H__32A18E60_BD08_46D0_ACC2_D2A61E3409FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEffectUnitDesc;

#include "Effect.h"
#include "..\MHFile.h"
#include "..\Object.h"

class CEffectTriggerMoveUnitDesc;
class CEffectTriggerGravityMoveUnitDesc;

class CEffectUnit  
{
protected:

	CEffectUnitDesc* m_pEffectUnitDesc;

	BOOL m_bInited;
	DWORD m_nCurTargetNum;
	
	virtual void DoProcess(DWORD tickTime,EFFECTPARAM* pParam)	{}

	virtual void GetObjectSet(EFFECTPARAM* pParam,TARGETSET** ppObject,int* objectnum);

public:
	CEffectUnit();
	virtual ~CEffectUnit();
	
	virtual void Initialize(CEffectUnitDesc* pEffectUnitDesc,EFFECTPARAM* pEffectParam);
	virtual void Release() = 0;

	void GetEffectStartPosition(VECTOR3* pRtPos,CObject* pObject,EFFECTPARAM* pParam);
	
	friend class CEffectUnitDesc;

	//////////////////////////////////////////////////////////////////////////
	virtual void InitEffect(DWORD nTargetNum,TARGETSET* pTargetArray);

	virtual BOOL On(EFFECTPARAM* pParam);
	virtual BOOL Off(EFFECTPARAM* pParam);
	virtual BOOL Move(EFFECTPARAM* pParam,CEffectTriggerMoveUnitDesc* pMoveDesc);
	virtual BOOL Attach(EFFECTPARAM* pParam,char* AttachPartName);
	virtual BOOL Detach(EFFECTPARAM* pParam);

	virtual BOOL GravityMove(EFFECTPARAM* pParam,CEffectTriggerGravityMoveUnitDesc* pGravityMoveDesc);

	virtual BOOL CameraRotate(EFFECTPARAM* pParam,float fRotateAngle,DWORD Duration);
	virtual BOOL CameraAngle(EFFECTPARAM* pParam,float fTargetAngle,DWORD Duration);
	virtual BOOL CameraZoom(EFFECTPARAM* pParam,float fZoomDistance,DWORD Duration);
	virtual BOOL ChangeCamera(EFFECTPARAM* pParam,int Camera,DWORD Duration);
	
	virtual BOOL Animate(EFFECTPARAM* pParam,int AnimationNum,BOOL bRepeat);

	virtual BOOL SetBaseMotion(EFFECTPARAM* pParam,int BaseAnimationNum);

	virtual BOOL Link(EFFECTPARAM* pParam);
	virtual BOOL Fade(EFFECTPARAM* pParam,DWORD FadeTime,float Start,float End);


	//////////////////////////////////////////////////////////////////////////
	virtual void DoOn(EFFECTPARAM* pParam,TARGETSET* pObject)		{}
	virtual void DoOff(EFFECTPARAM* pParam,TARGETSET* pObject)	{}
	virtual void DoMove(EFFECTPARAM* pParam,TARGETSET* pObject,CEffectTriggerMoveUnitDesc* pMoveDesc)	{}
	virtual void DoAttach(EFFECTPARAM* pParam,TARGETSET* pObject,char* AttachPartName)	{}
	virtual void DoDetach(EFFECTPARAM* pParam,TARGETSET* pObject)	{}
	virtual void DoGravityMove(EFFECTPARAM* pParam,TARGETSET* pObject,CEffectTriggerGravityMoveUnitDesc* pGravityMoveDesc)	{}

	virtual void DoCameraRotate(EFFECTPARAM* pParam,TARGETSET* pObject,float fRotateAngle,DWORD Duration)	{}
	virtual void DoCameraAngle(EFFECTPARAM* pParam,TARGETSET* pObject,float fTargetAngle,DWORD Duration)	{}
	virtual void DoCameraZoom(EFFECTPARAM* pParam,TARGETSET* pObject,float fZoomDistance,DWORD Duration)	{}
	virtual void DoChangeCamera(EFFECTPARAM* pParam,TARGETSET* pObject,int Camera,DWORD Duration)		{}

	virtual void DoAnimate(EFFECTPARAM* pParam,TARGETSET* pObject,int AnimationNum,BOOL bRepeat)			{}

	virtual void DoSetBaseMotion(EFFECTPARAM* pParam,TARGETSET* pObject,int BaseAnimationNum)		{}

	virtual void DoLink(EFFECTPARAM* pParam,TARGETSET* pObject)		{};	
	virtual void DoFade(EFFECTPARAM* pParam,TARGETSET* pObject,DWORD FadeTime,float Start,float End)	{};

	void Process(DWORD tickTime,EFFECTPARAM* pParam);
};

#endif // !defined(AFX_EFFECTUNIT_H__32A18E60_BD08_46D0_ACC2_D2A61E3409FC__INCLUDED_)

