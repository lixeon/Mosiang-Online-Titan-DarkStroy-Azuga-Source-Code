// SkillObjectTargetList_Fixed.h: interface for the CSkillObjectTargetList_Fixed class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTTARGETLIST_FIXED_H__57117C20_33DD_407C_AF33_4688F6EB5FD0__INCLUDED_)
#define AFX_SKILLOBJECTTARGETLIST_FIXED_H__57117C20_33DD_407C_AF33_4688F6EB5FD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillObjectTargetList.h"

class CSkillObjectTargetList_Fixed : public CSkillObjectTargetList  
{
	void DoRelease(CSkillObject* pSkillObject);
public:
	CSkillObjectTargetList_Fixed(SKILLINFO* pSkillInfo);
	virtual ~CSkillObjectTargetList_Fixed();

	BOOL IsInTargetArea(CObject* pObject);
	void SetSkillObjectPosition(VECTOR3* pPos);
};

#endif // !defined(AFX_SKILLOBJECTTARGETLIST_FIXED_H__57117C20_33DD_407C_AF33_4688F6EB5FD0__INCLUDED_)
