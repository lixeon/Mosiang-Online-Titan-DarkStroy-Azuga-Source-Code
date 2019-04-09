#ifndef BATTLETEAM_VIMUSTREET_H
#define BATTLETEAM_VIMUSTREET_H


#include "BattleTeam.h"


class CBattleTeam_VimuStreet : public CBattleTeam
{

protected:

	DWORD	m_CharacterID;
	DWORD	m_dwTeamDie;

public:

	CBattleTeam_VimuStreet();
	virtual ~CBattleTeam_VimuStreet();

	void SetCharacterID(DWORD id);
	DWORD GetCharacterID() { return m_CharacterID; }
	virtual BOOL IsAddableTeamMember(CObject* pObject);	

	virtual void DoDeleteTeamMember(CObject* pObject);
//	virtual void DoAddTeamMember(CObject* pObject);

///// TEST
	DWORD GetTeamDieNum() { return m_dwTeamDie; }
	void AddTeamDieNum() { ++m_dwTeamDie; }

};



#endif