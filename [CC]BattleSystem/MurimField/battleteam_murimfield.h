#ifndef _BATTLETEAM_MURIMFIELD_H
#define _BATTLETEAM_MURIMFIELD_H


#include "../BattleTeam.h"


class CBattleTeam_MurimField : public CBattleTeam
{
protected:
	DWORD					m_dwBattleID;
	MURIMFIELD_TEAMINFO		m_TeamInfo;

public:
	CBattleTeam_MurimField();
	virtual ~CBattleTeam_MurimField();

public:	// virtual func
	virtual BOOL	IsAddableTeamMember( CObject* pObject );
	virtual void	DoDeleteTeamMember( CObject* pObject );

public:
	void					Initialize( DWORD dwBattleID, MURIMFIELD_TEAMINFO* pInfo );	
	MURIMFIELD_TEAMINFO*	GetTeamInfo()									{ return &m_TeamInfo; }
	void					GetTeamInfo( MURIMFIELD_TEAMINFO* pOutInfo )	{ *pOutInfo = m_TeamInfo; }

public:	// friend	
	friend class CBattle_MurimField;

};
	

#endif