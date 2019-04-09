// SkillObjectTargetList_Circle.h: interface for the CSkillObjectTargetList_Circle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTTARGETLIST_CIRCLE_H__5FCE90EF_A88C_40FD_9C2D_1A9803E5F45A__INCLUDED_)
#define AFX_SKILLOBJECTTARGETLIST_CIRCLE_H__5FCE90EF_A88C_40FD_9C2D_1A9803E5F45A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SkillObjectTargetList.h"

class CSkillObjectTargetList_Circle : public CSkillObjectTargetList  
{
	VECTOR3 m_CenterPos;
	float m_Radius;
	float m_RadiusSq;
	BOOL m_bIsFixed;

	void DoRelease(CSkillObject* pSkillObject);

	virtual void DoInitTargetList(CSkillObject* pSkillObject,CTargetList* pTList);
public:
	CSkillObjectTargetList_Circle(SKILLINFO* pSkillInfo);
	virtual ~CSkillObjectTargetList_Circle();

	
	virtual BYTE AddTargetObject(CObject* pObject,BYTE bTargetKind);
	virtual BYTE RemoveTargetObject(DWORD ObjectID);

	virtual BOOL IsInTargetArea(CObject* pObject);
	virtual void SetSkillObjectPosition(VECTOR3* pPos);
	
	virtual void GetSkillObjectPosition(VECTOR3* pPos);

};

#endif // !defined(AFX_SKILLOBJECTTARGETLIST_CIRCLE_H__5FCE90EF_A88C_40FD_9C2D_1A9803E5F45A__INCLUDED_)
