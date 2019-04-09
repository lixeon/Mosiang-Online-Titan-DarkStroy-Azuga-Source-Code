// BattleTeam_GTournament.h: interface for the CBattleTeam_GTournament class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BATTLETEAM_GTOURNAMENT_H__9D04FBCB_2F3A_47E9_9101_2AF4FC0B29CF__INCLUDED_)
#define AFX_BATTLETEAM_GTOURNAMENT_H__9D04FBCB_2F3A_47E9_9101_2AF4FC0B29CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "BattleTeam.h"



class CGuild;




class CBattleTeam_GTournament : public CBattleTeam
{
	// Guild Info
	DWORD			m_GuildIdx;
	DWORD			m_MemberCount;
	DWORD			m_DeadMemberCount;
	DWORD			m_DeadMemberLeavecount;


	virtual void DoAddTeamMember(CObject* pObject);
	virtual void DoDeleteTeamMember(CObject* pObject);

public:
	CBattleTeam_GTournament();
	virtual ~CBattleTeam_GTournament();

	//
	void Init( GTTEAM_MEMBERINFO* pInfo, DWORD Team );
	void SetGuildInfo( DWORD Idx )		{	m_GuildIdx = Idx;	}
	DWORD GetGuildIdx()					{	return m_GuildIdx;	}

	virtual BOOL IsAddableTeamMember(CObject* pObject);

	BOOL IsMemberAllDead();
	int GetRemainMember();
	void IncreaseMemberCount()		{	++m_MemberCount;	}
	void IncreaseDieCount()			{	++m_DeadMemberCount;	}
	DWORD GetDieCount()				{	return m_DeadMemberCount;	}
	DWORD GetTotalLevel();
	DOUBLE GetTotalLevelExp();

	void AllPlayerOut();
	void AliveTeamMember();
	
	void ReturnToMap();
	void ReturnToMap( DWORD playerID );
	char* GetGuildName();

	void SetResult( BOOL bWin );
};

#endif // !defined(AFX_BATTLETEAM_GTOURNAMENT_H__9D04FBCB_2F3A_47E9_9101_2AF4FC0B29CF__INCLUDED_)
