// SkillObjectTargetList_Area.cpp: implementation of the CSkillObjectTargetList_Area class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObjectTargetList_Area.h"

#include "Object.h"
#include "SkillArea.h"

#ifdef _MAPSERVER_
#include "SkillManager_server.h"
#else
#include "SkillManager_client.h"
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillObjectTargetList_Area::CSkillObjectTargetList_Area(SKILLINFO* pSkillInfo)
: CSkillObjectTargetList(pSkillInfo)
{
	m_pSkillArea = NULL;
	m_SkillAreaIndex = pSkillInfo->TargetAreaIdx;
	m_bIsFixed = pSkillInfo->TargetAreaFix ? FALSE : TRUE;
}

CSkillObjectTargetList_Area::~CSkillObjectTargetList_Area()
{

}

void CSkillObjectTargetList_Area::DoRelease(CSkillObject* pSkillObject)
{
	if(m_pSkillArea)
	{
		m_pSkillArea->RemoveTileAttrByAreaData(pSkillObject->GetOperator());
		SKILLMGR->ReleaseSkillArea(m_pSkillArea);
		m_pSkillArea = NULL;
	}
}

void CSkillObjectTargetList_Area::SetSkillArea(CSkillArea* pSkillArea)
{
	m_pSkillArea = pSkillArea;
}

BOOL CSkillObjectTargetList_Area::IsInTargetArea(CObject* pObject)
{
	if(pObject->GetState() == eObjectState_Die)
		return FALSE;

	if(m_bIsFixed)
		return IsInTargetList(pObject);
	else
	{
		VECTOR3 pos;
		pObject->GetPosition(&pos);
		return m_pSkillArea->IsInDamageTile(&pos);
	}
}

void CSkillObjectTargetList_Area::SetSkillObjectPosition(VECTOR3* pPos)
{
	m_pSkillArea->SetCenterPos(pPos);
}

void CSkillObjectTargetList_Area::DoInitTargetList(CSkillObject* pSkillObject,CTargetList* pTList)
{
	CSkillArea* pSkillArea = SKILLMGR->GetSkillArea(pSkillObject->GetDirectionIndex(),m_SkillAreaIndex);
	VECTOR3 pos;
	pSkillObject->GetPosition(&pos);
	pSkillArea->SetCenterPos(&pos);
	SetSkillArea(pSkillArea);

#ifdef _MAPSERVER_
	pSkillArea->AddTileAttrByAreaData(pSkillObject->GetOperator());
#else
	if(pSkillObject->GetID() != TEMP_SKILLOBJECT_ID)
	{
//		pSkillArea->StartEffectOnDamageArea(1,pSkillObject);
		pSkillArea->AddTileAttrByAreaData(pSkillObject->GetOperator());
	}
#endif
}


BYTE CSkillObjectTargetList_Area::AddTargetObject(CObject* pObject,BYTE bTargetKind)
{
	return CSkillObjectTargetList::AddTargetObject(pObject,bTargetKind);
}
BYTE CSkillObjectTargetList_Area::RemoveTargetObject(DWORD ObjectID)
{
	return CSkillObjectTargetList::RemoveTargetObject(ObjectID);
}


void CSkillObjectTargetList_Area::GetSkillObjectPosition(VECTOR3* pPos)
{
#ifdef _MAPSERVER_
	m_pSkillArea->GetCenterPos(pPos);
#endif
}