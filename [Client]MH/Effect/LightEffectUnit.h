// LightEffectUnit.h: interface for the CLightEffectUnit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIGHTEFFECTUNIT_H__77EBEC20_8D45_44C0_B2FD_CA496A1BB07F__INCLUDED_)
#define AFX_LIGHTEFFECTUNIT_H__77EBEC20_8D45_44C0_B2FD_CA496A1BB07F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EffectUnit.h"
#include "..\Engine\EngineLight.h"

class CLightEffectUnit  : public CEffectUnit
{
protected:
	CEngineLight* m_pLight;

	VECTOR3 m_CurPosition;
	BOOL m_bIsMoving;
	VECTOR3 m_TargetPos;
	VECTOR3 m_MoveDirection;
	DWORD m_EstimatedTime;

	DWORD m_dwCloseTime;
	BOOL m_bIsClosing;
	

public:
	virtual void Initialize(CEffectUnitDesc* pEffectUnitDesc,EFFECTPARAM* pEffectParam);
	virtual void Release();
	
	CLightEffectUnit();
	virtual ~CLightEffectUnit();

	friend class CLightEffectUnitDesc;

	virtual void DoOn(EFFECTPARAM* pParam,TARGETSET* pObject);
	virtual void DoOff(EFFECTPARAM* pParam,TARGETSET* pObject);
	virtual void DoMove(EFFECTPARAM* pParam,TARGETSET* pObject,CEffectTriggerMoveUnitDesc* pMoveDesc);
	virtual void DoAttach(EFFECTPARAM* pParam,TARGETSET* pObject,char* AttachPartName);
	virtual void DoDetach(EFFECTPARAM* pParam,TARGETSET* pObject);
	
	void DoProcess(DWORD tickTime,EFFECTPARAM* pParam);
};

#endif // !defined(AFX_LIGHTEFFECTUNIT_H__77EBEC20_8D45_44C0_B2FD_CA496A1BB07F__INCLUDED_)
