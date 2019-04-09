// SoundEffectUnit.cpp: implementation of the CSoundEffectUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SoundEffectUnit.h"
#include "SoundEffectUnitDesc.h"
#include "..\Audio\MHAudioManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSoundEffectUnit::CSoundEffectUnit()
{

}

CSoundEffectUnit::~CSoundEffectUnit()
{

}

void CSoundEffectUnit::Initialize(CEffectUnitDesc* pEffectUnitDesc,EFFECTPARAM* pEffectParam)
{

	CEffectUnit::Initialize(pEffectUnitDesc,pEffectParam);
}
void CSoundEffectUnit::Release()
{
	CSoundEffectUnitDesc::ReleaseUnit(this);
}

void CSoundEffectUnit::DoOn(EFFECTPARAM* pParam,TARGETSET* pObject)
{
	CSoundEffectUnitDesc* pDesc = (CSoundEffectUnitDesc*)m_pEffectUnitDesc;

	AUDIOMGR->Play( pDesc->m_SoundNum, pObject->pTarget );
}
void CSoundEffectUnit::DoOff(EFFECTPARAM* pParam,TARGETSET* pObject)
{
	CSoundEffectUnitDesc* pDesc = (CSoundEffectUnitDesc*)m_pEffectUnitDesc;

}

void CSoundEffectUnit::DoProcess(DWORD tickTime,EFFECTPARAM* pParam)
{

}