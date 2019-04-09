// BattleTeam.h: interface for the CBattleTeam class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLETEAM_H__8EF01BE4_01A7_486D_A0F1_67BED5C19179__INCLUDED_)
#define AFX_BATTLETEAM_H__8EF01BE4_01A7_486D_A0F1_67BED5C19179__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBattleTeam  
{
protected:
	CYHHashTable<CObject> m_TeamMemberTable;

	virtual void DoRelease()							{}
	virtual void DoAddTeamMember(CObject* pObject)		{}
	virtual void DoDeleteTeamMember(CObject* pObject)	{}
public:
	CBattleTeam();
	virtual ~CBattleTeam();

	void Release();
	BOOL AddTeamMember(CObject* pObject);
	BOOL DeleteTeamMember(CObject* pObject);
	DWORD GetMemberNum();
	
	YHTPOSITION GetPositionHead();
	CObject* GetNextTeamMember(YHTPOSITION* ppList);

	void SendTeamMsg(MSGBASE* pmsg,int length);

	virtual BOOL IsAddableTeamMember(CObject* pObject) = 0;
	virtual int GetAliveTeamMemberNum()		{	return m_TeamMemberTable.GetDataNum();	}
	virtual BOOL IsTeamMember(CObject* pObject);
};

#endif // !defined(AFX_BATTLETEAM_H__8EF01BE4_01A7_486D_A0F1_67BED5C19179__INCLUDED_)
