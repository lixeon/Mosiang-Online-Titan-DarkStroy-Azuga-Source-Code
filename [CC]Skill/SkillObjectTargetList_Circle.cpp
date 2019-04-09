// SkillObjectTargetList_Circle.cpp: implementation of the CSkillObjectTargetList_Circle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObjectTargetList_Circle.h"
#include "Object.h"
#include "SkillObject_server.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillObjectTargetList_Circle::CSkillObjectTargetList_Circle(SKILLINFO* pSkillInfo)
: CSkillObjectTargetList(pSkillInfo)
{
	m_Radius = pSkillInfo->TargetRange;
	m_RadiusSq = m_Radius*m_Radius;
	m_bIsFixed = pSkillInfo->TargetAreaFix ? FALSE : TRUE;
}

CSkillObjectTargetList_Circle::~CSkillObjectTargetList_Circle()
{

}

void CSkillObjectTargetList_Circle::DoRelease(CSkillObject* pSkillObject)
{

}

BOOL CSkillObjectTargetList_Circle::IsInTargetArea(CObject* pObject)
{
	if(pObject->GetState() == eObjectState_Die)
		return FALSE;

	if(m_bIsFixed)
		return IsInTargetList(pObject);
	else
	{
		VECTOR3 pos;
		pObject->GetPosition(&pos);
		pos = m_CenterPos - pos;
		float distSq = pos.x*pos.x + pos.z*pos.z;
		if(distSq <= m_RadiusSq)
			return TRUE;
		else
			return FALSE;
	}
}

void CSkillObjectTargetList_Circle::SetSkillObjectPosition(VECTOR3* pPos)
{
	m_CenterPos = *pPos;
}


BYTE CSkillObjectTargetList_Circle::AddTargetObject(CObject* pObject,BYTE bTargetKind)
{
	return CSkillObjectTargetList::AddTargetObject(pObject,bTargetKind);
}
BYTE CSkillObjectTargetList_Circle::RemoveTargetObject(DWORD ObjectID)
{
	return CSkillObjectTargetList::RemoveTargetObject(ObjectID);
}


void CSkillObjectTargetList_Circle::DoInitTargetList(CSkillObject* pSkillObject,CTargetList* pTList)
{
	pSkillObject->GetPosition(&m_CenterPos);	
}

void CSkillObjectTargetList_Circle::GetSkillObjectPosition(VECTOR3* pPos)
{
#ifdef _MAPSERVER_
	*pPos = m_CenterPos;
#endif
}