#include "stdafx.h"
#include "BattleTeam_VimuStreet.h"
#include "Player.h"



CBattleTeam_VimuStreet::CBattleTeam_VimuStreet()
{
	m_dwTeamDie = 0;
}

CBattleTeam_VimuStreet::~CBattleTeam_VimuStreet()
{

}

void CBattleTeam_VimuStreet::SetCharacterID( DWORD id )
{
	m_CharacterID = id;
}
	
BOOL CBattleTeam_VimuStreet::IsAddableTeamMember(CObject* pObject)
{
	if( m_CharacterID == pObject->GetID() )
		return TRUE;

	return FALSE;
}


void CBattleTeam_VimuStreet::DoDeleteTeamMember(CObject* pObject)
{
	++m_dwTeamDie;

#ifdef _MAPSERVER_
	pObject->SetBattle(pObject->GetGridID(), 0);
#endif
}

/*
void CBattleTeam_VimuStreet::DoAddTeamMember(CObject* pObject)
{

}*/