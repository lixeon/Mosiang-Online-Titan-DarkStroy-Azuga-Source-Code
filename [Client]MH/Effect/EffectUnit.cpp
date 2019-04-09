// EffectUnit.cpp: implementation of the CEffectUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EffectUnit.h"
#include "EffectUnitDesc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEffectUnit::CEffectUnit()
{

}

CEffectUnit::~CEffectUnit()
{

}

void CEffectUnit::Initialize(CEffectUnitDesc* pEffectUnitDesc,EFFECTPARAM* pEffectParam)
{
	m_pEffectUnitDesc = pEffectUnitDesc;
	m_bInited = FALSE;	
}

void CEffectUnit::InitEffect(DWORD nTargetNum,TARGETSET* pTargetArray)
{
	if(nTargetNum == 0)
	{
		m_bInited = FALSE;		
		return;
	}
	//ASSERT(nTargetNum);
	
	
	m_bInited = TRUE;
}

void CEffectUnit::GetEffectStartPosition(VECTOR3* pRtPos,CObject* pObject,EFFECTPARAM* pParam)
{
	if((pParam->m_dwFlag & EFFECT_FLAG_ABSOLUTEPOS) == 0)
	{
		VRand(pRtPos,&m_pEffectUnitDesc->m_Position,&m_pEffectUnitDesc->m_RandomPos);
		if( m_pEffectUnitDesc->m_bRelatedCoordinate )
			//&& m_pEffectUnitDesc->m_bOneTargetPos == FALSE)		// 메인타겟용일때는 변환없음
		{	// 상대 좌표로 변환
			float angle = pObject->GetAngle();
			VECTOR3 ori = *pRtPos;
			TransToRelatedCoordinate(pRtPos,&ori,angle);
		}
		VECTOR3 tpos;
		if( m_pEffectUnitDesc->m_bOneTargetPos )
		{
			GetMainTargetPos(pParam->GetMainTarget(),&tpos,NULL);
			GetFieldHeight(&tpos);
		}
		else
			pObject->GetPosition(&tpos);
		
		*pRtPos = tpos + *pRtPos + pParam->m_AddPos;
	}
	else
		*pRtPos = pParam->m_AddPos;
}

void CEffectUnit::Process(DWORD tickTime,EFFECTPARAM* pParam)
{
	DoProcess(tickTime,pParam);
	
}

void CEffectUnit::GetObjectSet(EFFECTPARAM* pParam,TARGETSET** ppObject,int* objectnum)
{
	if(m_pEffectUnitDesc->IsDangledToOperator())	// 오퍼레이터 or 타켓
	{
		*objectnum = 1;
		*ppObject = (TARGETSET*)&pParam->m_pOperator;
	}
	else
	{
		if(m_pEffectUnitDesc->m_bOneTargetPos)
		{
			*objectnum = 1;
			*ppObject = (TARGETSET*)&pParam->m_pOperator;
		}
		else
		{
			*objectnum = pParam->m_nTargetNum;
			*ppObject = pParam->m_pTargetSet;
		}
	}
}

BOOL CEffectUnit::On(EFFECTPARAM* pParam)
{
	if(m_bInited == FALSE && m_pEffectUnitDesc->IsDangledToOperator() == FALSE)
		return FALSE;

	int objectnum;
	TARGETSET* pObject;
	GetObjectSet(pParam,&pObject,&objectnum);

	for(int n=0;n<objectnum;++n)
	{
		if(pObject[n].pTarget->IsInited() == FALSE)	// 케릭터가 존재하지 않으면 return
			continue;
		m_nCurTargetNum = n;
		DoOn(pParam,&pObject[n]);
	}
	return TRUE;
}

BOOL CEffectUnit::Off(EFFECTPARAM* pParam)
{
	if(m_bInited == FALSE && m_pEffectUnitDesc->IsDangledToOperator() == FALSE)
		return FALSE;

	int objectnum;
	TARGETSET* pObject;
	GetObjectSet(pParam,&pObject,&objectnum);


	for(int n=0;n<objectnum;++n)
	{
		if(pObject[n].pTarget->IsInited() == FALSE)	// 케릭터가 존재하지 않으면 return
			continue;
		m_nCurTargetNum = n;
		DoOff(pParam,&pObject[n]);
	}
	return TRUE;
}
BOOL CEffectUnit::Move(EFFECTPARAM* pParam,CEffectTriggerMoveUnitDesc* pMoveDesc)
{
	if(m_bInited == FALSE && m_pEffectUnitDesc->IsDangledToOperator() == FALSE)
		return FALSE;

	int objectnum;
	TARGETSET* pObject;
	GetObjectSet(pParam,&pObject,&objectnum);

	for(int n=0;n<objectnum;++n)
	{
		if(pObject[n].pTarget->IsInited() == FALSE)	// 케릭터가 존재하지 않으면 return
			continue;
		m_nCurTargetNum = n;
		DoMove(pParam,&pObject[n],pMoveDesc);
	}
	return TRUE;
}
BOOL CEffectUnit::Attach(EFFECTPARAM* pParam,char* AttachPartName)
{
	if(m_bInited == FALSE && m_pEffectUnitDesc->IsDangledToOperator() == FALSE)
		return FALSE;

	int objectnum;
	TARGETSET* pObject;
	GetObjectSet(pParam,&pObject,&objectnum);


	for(int n=0;n<objectnum;++n)
	{
		if(pObject[n].pTarget->IsInited() == FALSE)	// 케릭터가 존재하지 않으면 return
			continue;
		m_nCurTargetNum = n;
		DoAttach(pParam,&pObject[n],AttachPartName);
	}
	return TRUE;
}
BOOL CEffectUnit::Detach(EFFECTPARAM* pParam)
{
	if(m_bInited == FALSE && m_pEffectUnitDesc->IsDangledToOperator() == FALSE)
		return FALSE;

	int objectnum;
	TARGETSET* pObject;
	GetObjectSet(pParam,&pObject,&objectnum);


	for(int n=0;n<objectnum;++n)
	{
		if(pObject[n].pTarget->IsInited() == FALSE)	// 케릭터가 존재하지 않으면 return
			continue;
		m_nCurTargetNum = n;
		DoDetach(pParam,&pObject[n]);
	}
	return TRUE;
}
BOOL CEffectUnit::GravityMove(EFFECTPARAM* pParam,CEffectTriggerGravityMoveUnitDesc* pGravityMoveDesc)
{
	if(m_bInited == FALSE && m_pEffectUnitDesc->IsDangledToOperator() == FALSE)
		return FALSE;

	int objectnum;
	TARGETSET* pObject;
	GetObjectSet(pParam,&pObject,&objectnum);


	for(int n=0;n<objectnum;++n)
	{
		if(pObject[n].pTarget->IsInited() == FALSE)	// 케릭터가 존재하지 않으면 return
			continue;
		m_nCurTargetNum = n;
		DoGravityMove(pParam,&pObject[n],pGravityMoveDesc);
	}
	return TRUE;
}

BOOL CEffectUnit::CameraRotate(EFFECTPARAM* pParam,float fRotateAngle,DWORD Duration)
{
	if(m_bInited == FALSE && m_pEffectUnitDesc->IsDangledToOperator() == FALSE)
		return FALSE;

	int objectnum;
	TARGETSET* pObject;
	GetObjectSet(pParam,&pObject,&objectnum);


	for(int n=0;n<objectnum;++n)
	{
		if(pObject[n].pTarget->IsInited() == FALSE)	// 케릭터가 존재하지 않으면 return
			continue;
		m_nCurTargetNum = n;
		DoCameraRotate(pParam,&pObject[n],fRotateAngle,Duration);
	}
	return TRUE;
}
BOOL CEffectUnit::CameraAngle(EFFECTPARAM* pParam,float fTargetAngle,DWORD Duration)
{
	if(m_bInited == FALSE && m_pEffectUnitDesc->IsDangledToOperator() == FALSE)
		return FALSE;

	int objectnum;
	TARGETSET* pObject;
	GetObjectSet(pParam,&pObject,&objectnum);


	for(int n=0;n<objectnum;++n)
	{
		if(pObject[n].pTarget->IsInited() == FALSE)	// 케릭터가 존재하지 않으면 return
			continue;
		m_nCurTargetNum = n;
		DoCameraAngle(pParam,&pObject[n],fTargetAngle,Duration);
	}
	return TRUE;
}
BOOL CEffectUnit::CameraZoom(EFFECTPARAM* pParam,float fZoomDistance,DWORD Duration)
{	
	if(m_bInited == FALSE && m_pEffectUnitDesc->IsDangledToOperator() == FALSE)
		return FALSE;

	int objectnum;
	TARGETSET* pObject;
	GetObjectSet(pParam,&pObject,&objectnum);


	for(int n=0;n<objectnum;++n)
	{
		if(pObject[n].pTarget->IsInited() == FALSE)	// 케릭터가 존재하지 않으면 return
			continue;
		m_nCurTargetNum = n;
		DoCameraZoom(pParam,&pObject[n],fZoomDistance,Duration);
	}
	return TRUE;
}
BOOL CEffectUnit::ChangeCamera(EFFECTPARAM* pParam,int Camera,DWORD Duration)
{
	if(m_bInited == FALSE && m_pEffectUnitDesc->IsDangledToOperator() == FALSE)
		return FALSE;
	
	int objectnum;
	TARGETSET* pObject;
	GetObjectSet(pParam,&pObject,&objectnum);


	for(int n=0;n<objectnum;++n)
	{
		if(pObject[n].pTarget->IsInited() == FALSE)	// 케릭터가 존재하지 않으면 return
			continue;
		m_nCurTargetNum = n;
		DoChangeCamera(pParam,&pObject[n],Camera,Duration);
	}
	return TRUE;
}

BOOL CEffectUnit::Animate(EFFECTPARAM* pParam,int AnimationNum,BOOL bRepeat)
{
	if(m_bInited == FALSE && m_pEffectUnitDesc->IsDangledToOperator() == FALSE)
		return FALSE;

	int objectnum;
	TARGETSET* pObject;
	GetObjectSet(pParam,&pObject,&objectnum);


	for(int n=0;n<objectnum;++n)
	{
		if(pObject[n].pTarget->IsInited() == FALSE)	// 케릭터가 존재하지 않으면 return
			continue;
		m_nCurTargetNum = n;
		DoAnimate(pParam,&pObject[n],AnimationNum,bRepeat);
	}
	return TRUE;
}

BOOL CEffectUnit::SetBaseMotion(EFFECTPARAM* pParam,int BaseAnimationNum)
{
	if(m_bInited == FALSE && m_pEffectUnitDesc->IsDangledToOperator() == FALSE)
		return FALSE;

	int objectnum;
	TARGETSET* pObject;
	GetObjectSet(pParam,&pObject,&objectnum);


	for(int n=0;n<objectnum;++n)
	{
		if(pObject[n].pTarget->IsInited() == FALSE)	// 케릭터가 존재하지 않으면 return
			continue;
		m_nCurTargetNum = n;
		DoSetBaseMotion(pParam,&pObject[n],BaseAnimationNum);
	}
	return TRUE;
}

BOOL CEffectUnit::Link(EFFECTPARAM* pParam)
{
	if(m_bInited == FALSE && m_pEffectUnitDesc->IsDangledToOperator() == FALSE)
		return FALSE;

	int objectnum;
	TARGETSET* pObject;
	GetObjectSet(pParam,&pObject,&objectnum);

	for(int n=0;n<objectnum;++n)
	{
		if(pObject[n].pTarget->IsInited() == FALSE)	// 케릭터가 존재하지 않으면 return
			continue;
		m_nCurTargetNum = n;
		DoLink(pParam,&pObject[n]);
	}
	return TRUE;
}

BOOL CEffectUnit::Fade(EFFECTPARAM* pParam,DWORD FadeTime,float Start,float End)
{
	if(m_bInited == FALSE && m_pEffectUnitDesc->IsDangledToOperator() == FALSE)
		return FALSE;

	int objectnum;
	TARGETSET* pObject;
	GetObjectSet(pParam,&pObject,&objectnum);

	for(int n=0;n<objectnum;++n)
	{
		if(pObject[n].pTarget->IsInited() == FALSE)	// 케릭터가 존재하지 않으면 return
			continue;
		m_nCurTargetNum = n;
		DoFade(pParam,&pObject[n],FadeTime,Start,End);
	}
	return TRUE;
}