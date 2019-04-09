// SkillObjectTargetList_Fixed.cpp: implementation of the CSkillObjectTargetList_Fixed class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillObjectTargetList_Fixed.h"
#include "Object.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillObjectTargetList_Fixed::CSkillObjectTargetList_Fixed(SKILLINFO* pSkillInfo)
: CSkillObjectTargetList(pSkillInfo)
{

}

CSkillObjectTargetList_Fixed::~CSkillObjectTargetList_Fixed()
{

}

void CSkillObjectTargetList_Fixed::DoRelease(CSkillObject* pSkillObject)
{

}

BOOL CSkillObjectTargetList_Fixed::IsInTargetArea(CObject* pObject)
{
	if(pObject->GetState() == eObjectState_Die)
		return FALSE;

	return FALSE;
}

void CSkillObjectTargetList_Fixed::SetSkillObjectPosition(VECTOR3* pPos)
{
	// DO Nothing	
}