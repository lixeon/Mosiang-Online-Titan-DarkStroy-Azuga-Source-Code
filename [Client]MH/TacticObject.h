// TacticObject.h: interface for the CTacticObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TACTICOBJECT_H__98333B1F_B4EC_48F6_9D92_A8C782F8BFCA__INCLUDED_)
#define AFX_TACTICOBJECT_H__98333B1F_B4EC_48F6_9D92_A8C782F8BFCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Object.h"

class CTacticStartInfo;
class CHero;

class CTacticObject  : public CObject
{
	DWORD m_StartTime;

	CTacticStartInfo* m_pTacticInfo;
	TACTIC_TOTALINFO m_TotalInfo;
	VECTOR3 m_TacticMemberPosition[MAX_PARTY_LISTNUM];

	HEFFPROC m_hTacticPositionEffect;

	BOOL m_bExcuted;
	BOOL m_bFailed;

public:
	CTacticObject();
	virtual ~CTacticObject();

	void InitTacticObject(TACTIC_TOTALINFO* pTotalInfo);
	void TacticStartEffect();
	void StartJoinTacticEffect(CObject* pObject,int Position);
	void EndJoinTacticEffect(CObject* pObject,int Position);
	
	int CheckJoin(CHero* pHero);	

	void ExcuteTactic();
	
	void SetMotionInState(BYTE State);

	void OnFailedTactic(DWORD TacticFailedReason, BOOL bMsg);
	
	virtual void SetPosition(VECTOR3* pPos);
	virtual void GetPosition(VECTOR3* pPos);
	virtual void SetAngle(float AngleRad);
	virtual float GetAngle();
	
	virtual void ChangeMotion(int motion,BOOL bRoop);
	virtual void ChangeBaseMotion(int motion);
};

#endif // !defined(AFX_TACTICOBJECT_H__98333B1F_B4EC_48F6_9D92_A8C782F8BFCA__INCLUDED_)
