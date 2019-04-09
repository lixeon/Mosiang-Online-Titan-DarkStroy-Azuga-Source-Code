// ObjectEffectUnit.h: interface for the CObjectEffectUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTEFFECTUNIT_H__D2BE7F53_C68D_488E_BF0C_147CF86DD2A7__INCLUDED_)
#define AFX_OBJECTEFFECTUNIT_H__D2BE7F53_C68D_488E_BF0C_147CF86DD2A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectUnit.h"
#include "..\Engine\EngineEffect.h"
#include "..\Object.h"
#include "EffectTriggerGravityMoveUnitDesc.h"

#define OBJEFFECT_NOTMOVE		0
#define OBJEFFECT_NORMALMOVE	1
#define OBJEFFECT_GRAVITYMOVE	2
#define OBJEFFECT_FIXANGLEANGLE 4

class CObjectEffectUnitDesc;
class CObjectEffectUnit : public CEffectUnit  
{
	struct EFFECTOBJECT
	{
		void Clear()
		{
			m_Object.Release();
			m_bLinked = FALSE;
			m_MoveMode = OBJEFFECT_NOTMOVE;
			m_EstimatedTime = 0;
			m_pAttachedObject = NULL;
		}
		CEngineEffect m_Object;
		VECTOR3 m_CurPosition;
		VECTOR3 m_TargetPos;
		VECTOR3 m_MoveDirection;
		DWORD m_EstimatedTime;
		BOOL m_bLinked;
		BYTE m_MoveMode;		

		CObject* m_pAttachedObject;

		GRAVITYMOVEINFO m_GravityMoveInfo;
	};

	BOOL m_bFading;
	DWORD m_FadeStartTime;
	DWORD m_FadeDuration;
	float m_FadeStart;
	float m_FadeEnd;

	// jsd test	
	DWORD		m_dwPattern;
	float		m_fChangeVel;		//속도변화량
	float		m_fAccel;
	VECTOR3		m_vOriginAngle;		// radian값
	VECTOR3		m_vStartAngle;		// 시작 radian값 (이건 외부에서 받아야?)
	VECTOR3		m_vTotalAngle;		// origin + start
	VECTOR3		m_vChangeAngle;
	VECTOR3		m_vAnglePerTick;	// total / time
	BOOL		m_bTurn;
	float		m_fFlag;
	DWORD		m_dwTime;	
	
	void MakeEffectUnit(EFFECTOBJECT* pEffObject,VECTOR3* pPos,float angle,
					CObjectEffectUnitDesc* pDesc,EFFECTPARAM* pParam,TARGETSET* pObject);
	
	void EffectMoveProcess(EFFECTPARAM* pParam,EFFECTOBJECT* pEffObject,DWORD tickTime);
protected:
	int m_nObjectNum;
	EFFECTOBJECT m_pEffectObjectArray[MAX_TARGET_NUM];


public:
	virtual void Initialize(CEffectUnitDesc* pEffectUnitDesc,EFFECTPARAM* pEffectParam);
	virtual void Release();

	CObjectEffectUnit();	
	virtual ~CObjectEffectUnit();

	virtual void InitEffect(DWORD nTargetNum,TARGETSET* pTargetArray);

	virtual void DoOn(EFFECTPARAM* pParam,TARGETSET* pObject);
	virtual void DoOff(EFFECTPARAM* pParam,TARGETSET* pObject);
	virtual void DoMove(EFFECTPARAM* pParam,TARGETSET* pObject,CEffectTriggerMoveUnitDesc* pMoveDesc);
	virtual void DoGravityMove(EFFECTPARAM* pParam,TARGETSET* pObject,CEffectTriggerGravityMoveUnitDesc* pGravityMoveDesc);
	virtual void DoAttach(EFFECTPARAM* pParam,TARGETSET* pObject,char* AttachPartName);
	virtual void DoDetach(EFFECTPARAM* pParam,TARGETSET* pObject);
	virtual void DoLink(EFFECTPARAM* pParam,TARGETSET* pObject);
	virtual void DoFade(EFFECTPARAM* pParam,TARGETSET* pObject,DWORD FadeTime,float Start,float End);
	
	virtual void DoAnimate(EFFECTPARAM* pParam,TARGETSET* pObject,int AnimationNum,BOOL bRepeat);
	
	void DoProcess(DWORD tickTime,EFFECTPARAM* pParam);

	// jsd test
	void	CalPatternPos( EFFECTOBJECT* pEffObject, DWORD tickTime );

	friend class CObjectEffectUnitDesc;
};

#endif // !defined(AFX_OBJECTEFFECTUNIT_H__D2BE7F53_C68D_488E_BF0C_147CF86DD2A7__INCLUDED_)
