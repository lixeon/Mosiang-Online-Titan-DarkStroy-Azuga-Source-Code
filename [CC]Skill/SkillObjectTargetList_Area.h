// SkillObjectTargetList_Area.h: interface for the CSkillObjectTargetList_Area class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTTARGETLIST_AREA_H__38AC7411_485A_479D_8243_F3170547C263__INCLUDED_)
#define AFX_SKILLOBJECTTARGETLIST_AREA_H__38AC7411_485A_479D_8243_F3170547C263__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillObjectTargetList.h"

class CSkillArea;

class CSkillObjectTargetList_Area : public CSkillObjectTargetList  
{
	BOOL m_bIsFixed;
	WORD m_SkillAreaIndex;

	CSkillArea* m_pSkillArea;

	void DoRelease(CSkillObject* pSkillObject);

	virtual void DoInitTargetList(CSkillObject* pSkillObject,CTargetList* pTList);
public:
	CSkillObjectTargetList_Area(SKILLINFO* pSkillInfo);
	virtual ~CSkillObjectTargetList_Area();

	void SetSkillArea(CSkillArea* pSkillArea);

	virtual BYTE AddTargetObject(CObject* pObject,BYTE bTargetKind);
	virtual BYTE RemoveTargetObject(DWORD ObjectID);

	BOOL IsInTargetArea(CObject* pObject);
	void SetSkillObjectPosition(VECTOR3* pPos);

	virtual void GetSkillObjectPosition(VECTOR3* pPos);
};

#endif // !defined(AFX_SKILLOBJECTTARGETLIST_AREA_H__38AC7411_485A_479D_8243_F3170547C263__INCLUDED_)
