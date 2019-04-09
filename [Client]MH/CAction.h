// Action.h: interface for the CAction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACTION_H__0533FE2A_575E_45FE_840F_CACEDAE885A6__INCLUDED_)
#define AFX_ACTION_H__0533FE2A_575E_45FE_840F_CACEDAE885A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ActionTarget.h"

enum eActionKind
{
	eActionKind_None,
	eActionKind_Skill,
	eActionKind_Skill_RealExecute,
	eActionKind_TalkToNpc,
	eActionKind_TalkToOwner, // LBS 03.09.30 ³ëÁ¡»ó °ü·Ã
	eActionKind_Move,
	eActionKind_EquipItem,
	eActionKind_TalkToPet,
};

class CAction  
{
	int m_ActionKind;
	float m_Distance;
	CActionTarget m_ActionTarget;
	void* m_pActionInfo;

public:
	CAction();
	~CAction();

	BOOL HasAction();

	void CopyFrom(CAction* pSrc);

	int GetActionKind()	{	return m_ActionKind;	}
	DWORD GetTargetID()	{	return m_ActionTarget.GetTargetID();	}

	void InitSkillAction(CSkillInfo* pSkillInfo,CActionTarget* pTarget);
	void InitSkillActionRealExecute(CSkillInfo* pSkillInfo,CActionTarget* pTarget);
	void InitStallAction( float Distance, CActionTarget* pTarget );
	void InitMoveAction(VECTOR3* pToPos);

	void InitAction(int ActionKind,void* pActionInfo,float Distance,CActionTarget* pTarget);
	void Clear();
	void ExcuteAction(CHero* pOperator);

	BOOL CheckTargetDistance(VECTOR3* pCurPos);
};

#endif // !defined(AFX_ACTION_H__0533FE2A_575E_45FE_840F_CACEDAE885A6__INCLUDED_)
