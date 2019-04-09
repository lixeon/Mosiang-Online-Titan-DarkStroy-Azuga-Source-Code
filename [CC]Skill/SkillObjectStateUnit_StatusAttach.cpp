// SkillObjectStateUnit_StatusAttach.cpp: implementation of the CSkillObjectStateUnit_StatusAttach class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObjectStateUnit_StatusAttach.h"

#include "SkillObjectAttachUnit.h"
#include "Object.h"

#ifdef _MAPSERVER_
#include "SkillObject_server.h"
#include "Player.h"
#else
#include "SkillObject_client.h"
#include "StatusIconDlg.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillObjectStateUnit_StatusAttach::CSkillObjectStateUnit_StatusAttach()
{
	m_SkillTime = 0;
}

CSkillObjectStateUnit_StatusAttach::~CSkillObjectStateUnit_StatusAttach()
{
	SAFE_DELETE(m_pAttachUnit);
}

void CSkillObjectStateUnit_StatusAttach::Init(CSkillInfo* pSkillInfo,WORD SkillLevel,CObject* pOper/* =NULL */)
{
	ASSERT(m_pAttachUnit);
	m_pAttachUnit->Init(pSkillInfo,SkillLevel, pOper);
#ifdef _HK_LOCAL_
	if( m_pAttachUnit->GetAttachUnitKind() == eATTACH_UNIT_KIND_DODGE )
		m_SkillTime = 1000; //회피는 지속시간이 1초
	else
#endif
#ifdef _TL_LOCAL_
	if( m_pAttachUnit->GetAttachUnitKind() == eATTACH_UNIT_KIND_DODGE )
		m_SkillTime = 1000; //회피는 지속시간이 1초
	else
#endif
		m_SkillTime = pSkillInfo->GetSkillContinueTime() + pSkillInfo->GetSkillInfo()->SkillAdditionalTime[SkillLevel-1];

#ifdef _MHCLIENT_
	for(int n=0;n<MAX_SKILLMAINTAIN_EFFECT;++n)
	{
		m_StateEffectNum[n] = m_pAttachUnit->GetAttachEffectNum(n);
		m_StateIconNum[n] = m_pAttachUnit->GetStateIconNum(n);
	}
#endif
}

void CSkillObjectStateUnit_StatusAttach::SetAttachUnit(CSkillObjectAttachUnit* pAttachUnit,WORD PNTarget)
{
	m_PNTarget = PNTarget;
	m_pAttachUnit = pAttachUnit;
}

void CSkillObjectStateUnit_StatusAttach::DoStartState(CObject* pObject, CObject* pSkillOperator)
{
#ifdef _MAPSERVER_
	if(pObject->GetInited() == FALSE)
		return;
#else
	if(pObject->IsInited() == FALSE)
		return;
#endif

#ifdef _MHCLIENT_
	OBJECTEFFECTDESC desc[MAX_SKILLMAINTAIN_EFFECT];
	for(int n=0;n<MAX_SKILLMAINTAIN_EFFECT;++n)
	{
		desc[n].SetDesc(m_StateEffectNum[n]);
		if(m_StateIconNum[n])
		{
			if( m_pSkillObject )
				STATUSICONDLG->AddIcon(pObject,m_StateIconNum[n],0,m_SkillTime);
			else
				STATUSICONDLG->AddIcon(pObject,m_StateIconNum[n]);
		}
	}
	if( pObject->GetObjectKind() != eObjectKind_BossMonster && 
		pObject->GetObjectKind() != eObjectKind_FieldBossMonster )
		pObject->AddObjectEffect(SKILLOBJECT_ID_START+m_pSkillObject->GetSkillIdx(),desc,MAX_SKILLMAINTAIN_EFFECT,pSkillOperator);
	
		
#endif
	pObject->AddStatus(m_pAttachUnit);
}

void CSkillObjectStateUnit_StatusAttach::DoEndState(CObject* pObject)
{
#ifdef _MAPSERVER_
	if(pObject->GetInited() == FALSE)
		return;
#else
	if(pObject->IsInited() == FALSE)
		return;
#endif

#ifdef _MHCLIENT_
	pObject->RemoveObjectEffect(SKILLOBJECT_ID_START+m_pSkillObject->GetSkillIdx());
	
	for(int n=0;n<MAX_SKILLMAINTAIN_EFFECT;++n)
	{
		if(m_StateIconNum[n])
			STATUSICONDLG->RemoveIcon(pObject,m_StateIconNum[n]);
	}

#endif
	pObject->RemoveStatus(m_pAttachUnit);
}
