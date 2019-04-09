// SkillObjectTargetList_One.h: interface for the CSkillObjectTargetList_One class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTTARGETLIST_ONE_H__059660FE_E657_4250_81E2_F221E68285C8__INCLUDED_)
#define AFX_SKILLOBJECTTARGETLIST_ONE_H__059660FE_E657_4250_81E2_F221E68285C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillObjectTargetList.h"

class CSkillObjectTargetList_One : public CSkillObjectTargetList  
{
	void DoRelease(CSkillObject* pSkillObject);

	virtual void DoInitTargetList(CSkillObject* pSkillObject,CTargetList* pTList);
public:
	CSkillObjectTargetList_One(SKILLINFO* pSkillInfo);
	virtual ~CSkillObjectTargetList_One();

	
	virtual BYTE AddTargetObject(CObject* pObject,BYTE bTargetKind);
	virtual BYTE RemoveTargetObject(DWORD ObjectID);

	virtual BOOL IsInTargetArea(CObject* pObject);
	virtual void SetSkillObjectPosition(VECTOR3* pPos);

};

#endif // !defined(AFX_SKILLOBJECTTARGETLIST_ONE_H__059660FE_E657_4250_81E2_F221E68285C8__INCLUDED_)
