// BattleTeam_MunpaField.h: interface for the CBattleTeam_MunpaField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLETEAM_MUNPAFIELD_H__E16596DB_E059_4BB3_AAB5_E4217929CE71__INCLUDED_)
#define AFX_BATTLETEAM_MUNPAFIELD_H__E16596DB_E059_4BB3_AAB5_E4217929CE71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BattleTeam.h"

class CBattleTeam_MunpaField : public CBattleTeam  
{
	DWORD m_dwBattleID;

	MUNPAFIELD_TEAMINFO m_TeamInfo;

	MUNPAFIELD_TEAMINFO* GetTeamInfo()
	{
		return &m_TeamInfo;
	}
public:
	CBattleTeam_MunpaField();
	virtual ~CBattleTeam_MunpaField();

	void Initialize(DWORD dwBattleID, MUNPAFIELD_TEAMINFO* pTeamInfo);
	
	virtual BOOL IsAddableTeamMember(CObject* pObject);

	void GetTeamInfo(MUNPAFIELD_TEAMINFO* pInfoOut);
	
	virtual void DoDeleteTeamMember(CObject* pObject);

	friend class CBattle_MunpaField;
};

#endif // !defined(AFX_BATTLETEAM_MUNPAFIELD_H__E16596DB_E059_4BB3_AAB5_E4217929CE71__INCLUDED_)
