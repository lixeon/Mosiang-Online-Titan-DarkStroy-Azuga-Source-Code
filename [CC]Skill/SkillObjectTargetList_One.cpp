// SkillObjectTargetList_One.cpp: implementation of the CSkillObjectTargetList_One class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObjectTargetList_One.h"
#include "Object.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillObjectTargetList_One::CSkillObjectTargetList_One(SKILLINFO* pSkillInfo)
: CSkillObjectTargetList(pSkillInfo)
{
	
}

CSkillObjectTargetList_One::~CSkillObjectTargetList_One()
{

}

void CSkillObjectTargetList_One::DoRelease(CSkillObject* pSkillObject)
{

}

BOOL CSkillObjectTargetList_One::IsInTargetArea(CObject* pObject)
{
	if(pObject->GetState() == eObjectState_Die)
		return FALSE;

	return IsInTargetList(pObject);
}

BYTE CSkillObjectTargetList_One::AddTargetObject(CObject* pObject,BYTE bTargetKind)
{
	ASSERT(0);
	return 0;
}
BYTE CSkillObjectTargetList_One::RemoveTargetObject(DWORD ObjectID)
{
	return CSkillObjectTargetList::RemoveTargetObject(ObjectID);
}

void CSkillObjectTargetList_One::SetSkillObjectPosition(VECTOR3* pPos)
{

}

void CSkillObjectTargetList_One::DoInitTargetList(CSkillObject* pSkillObject,CTargetList* pTList)
{

}