// ActionTarget.h: interface for the CActionTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLTARGETINFO_H__2AFAB6B4_8DE6_4458_BA2C_49A45C299878__INCLUDED_)
#define AFX_SKILLTARGETINFO_H__2AFAB6B4_8DE6_4458_BA2C_49A45C299878__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHero;
class CSkillInfo;
class CSkillArea;

#define ACTIONTARGET_FLAG_NONE	0x00000000
#define ACTIONTARGET_FLAG_PK	0x00000001

enum eActionTargetKind
{
	eActionTargetKind_None,
	eActionTargetKind_Object,
	eActionTargetKind_Position,
};


class CActionTarget
{
	int m_TargetKind;

	DWORD m_TargetID;
	VECTOR3 m_TargetPosition;

	DWORD m_TargetFlag;

	// for MultiTarget
	static ObjectArray<LPOBJECT> m_ObjectArray;

public:
	CActionTarget();
	~CActionTarget();

	void InitActionTarget(CObject* pObject,DWORD flag);
	void InitActionTarget(VECTOR3* pPos,DWORD flag);

	void Clear();
	void CopyFrom(CActionTarget* pSrc);

	VECTOR3* GetTargetPosition();
	float GetRadius();
	DWORD GetTargetID();
	inline int GetTargetKind()	{	return m_TargetKind;	}
	
	// for MultiTarget	
	void SetTargetObjectsInArea(CHero* pHero,CSkillArea* pSkillArea);
	void SetTargetObjectsInRange(VECTOR3* pCenterPos,float Radius);
	void SetPositionFirstTargetObject();
	CObject* GetNextTargetObject();
	
	// for OneTarget
	void SetOneTarget(CObject* pObject);
	
	void GetMainTarget(MAINTARGET* pRtMainTarget);
	void GetTargetArray(CTargetList* pTList);
	
	void ConvertMainTargetToPosition(CObject* pAttacker,float AttackDist);
};

#endif // !defined(AFX_SKILLTARGETINFO_H__2AFAB6B4_8DE6_4458_BA2C_49A45C299878__INCLUDED_)
