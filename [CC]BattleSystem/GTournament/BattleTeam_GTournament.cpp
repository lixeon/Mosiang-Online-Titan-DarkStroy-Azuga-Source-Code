// BattleTeam_GTournament.cpp: implementation of the CBattleTeam_GTournament class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleTeam_GTournament.h"
#include "Guild.h"

#include "Player.h"

#ifdef _MAPSERVER_
#include "objectstatemanager.h"
#include "GuildTournamentMgr.h"
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBattleTeam_GTournament::CBattleTeam_GTournament()
{
	m_GuildIdx = 0;
	m_MemberCount		= 0;
	m_DeadMemberCount	= 0;
	m_DeadMemberLeavecount	= 0;
}


CBattleTeam_GTournament::~CBattleTeam_GTournament()
{

}


void CBattleTeam_GTournament::Init( GTTEAM_MEMBERINFO* pInfo, DWORD Team )
{
	m_GuildIdx = pInfo->TeamGuildIdx[Team];
	m_MemberCount = pInfo->TeamMember[Team];
	m_DeadMemberCount = pInfo->TeamDieMember[Team];
}


void CBattleTeam_GTournament::DoAddTeamMember(CObject* pObject)
{
#ifdef _MAPSERVER_
#endif

	++m_MemberCount;
}


void CBattleTeam_GTournament::DoDeleteTeamMember(CObject* pObject)
{
#ifdef _MAPSERVER_
	if( pObject->GetState() == eObjectState_Die )
	{
		++m_DeadMemberLeavecount;
	}
#endif
	if( m_MemberCount > 0 )					--m_MemberCount;
}


BOOL CBattleTeam_GTournament::IsAddableTeamMember(CObject* pObject)
{
	if( pObject->GetObjectKind() != eObjectKind_Player )
		return FALSE;

#ifdef _MAPSERVER_
	if( m_GuildIdx == ((CPlayer*)pObject)->GetGuildIdx() )
		return TRUE;	
#endif

	return FALSE;
}


BOOL CBattleTeam_GTournament::IsMemberAllDead()
{
	if( GetMemberNum() <= m_DeadMemberCount )
		return TRUE;

	return FALSE;
}


void CBattleTeam_GTournament::AllPlayerOut()
{
	
}


void CBattleTeam_GTournament::AliveTeamMember()
{
#ifdef _MAPSERVER_
	CPlayer* pPlayer;
	m_TeamMemberTable.SetPositionHead();
	while(pPlayer = (CPlayer*)m_TeamMemberTable.GetData())
	{
		if(pPlayer->GetState() == eObjectState_Die)
			OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer,eObjectState_Die);
		pPlayer->SetLife(pPlayer->GetMaxLife(),FALSE);
		pPlayer->SetShield(pPlayer->GetMaxShield(),FALSE);
		pPlayer->SetNaeRyuk(pPlayer->GetMaxNaeRyuk(),FALSE);
	}
#endif
}


void CBattleTeam_GTournament::ReturnToMap()
{
#ifdef _MAPSERVER_
	CPlayer* pPlayer;
	m_TeamMemberTable.SetPositionHead();

//	DWORD dwRemoveID[1024];
//	int nRemoveCount = 0;
	cPtrList list;

	while(pPlayer = (CPlayer*)m_TeamMemberTable.GetData())
	{
		MSG_DWORD msg;
		msg.Category = MP_GTOURNAMENT;
		msg.Protocol = MP_GTOURNAMENT_RETURNTOMAP;
		msg.dwData = pPlayer->GetReturnMapNum();
		pPlayer->SendMsg( &msg, sizeof(msg) );

		//¿¹¾à. 
		//dwRemoveID[nRemoveCount] = pPlayer->GetID();
		//++nRemoveCount;
		list.AddTail(pPlayer);

//		g_pServerSystem->RemovePlayer( pPlayer->GetID() );
	}

//	for( int i = 0 ; i < nRemoveCount ; ++i )
//	{
//		g_pServerSystem->RemovePlayer( dwRemoveID[i] );
//	}
	PTRLISTPOS pos = list.GetHeadPosition();
	while( pos )
	{
		CPlayer* p = (CPlayer*)list.GetNext(pos);
		g_pServerSystem->RemovePlayer(p->GetID());
	}
	list.RemoveAll();

	m_TeamMemberTable.RemoveAll();

#endif
}

void CBattleTeam_GTournament::ReturnToMap( DWORD playerID )
{
#ifdef _MAPSERVER_
	CPlayer* pPlayer = (CPlayer*)m_TeamMemberTable.GetData( playerID );	
	if( pPlayer )
	{
		MSG_DWORD msg;
		msg.Category = MP_GTOURNAMENT;
		msg.Protocol = MP_GTOURNAMENT_RETURNTOMAP;
		msg.dwData = pPlayer->GetReturnMapNum();
		pPlayer->SendMsg( &msg, sizeof(msg) );
		
		g_pServerSystem->RemovePlayer( playerID );
		m_TeamMemberTable.Remove( playerID );
	}	
#endif
}


void CBattleTeam_GTournament::SetResult( BOOL bWin )
{
#ifdef _MAPSERVER_

	GTMGR->SetResult( m_GuildIdx, bWin );
	
#endif
}


DWORD CBattleTeam_GTournament::GetTotalLevel()
{
	DWORD level = 0;

	CPlayer* pPlayer = NULL;
	m_TeamMemberTable.SetPositionHead();
	while(pPlayer = (CPlayer*)m_TeamMemberTable.GetData())
	{
		level += pPlayer->GetLevel();
	}

	return level;
}


DOUBLE CBattleTeam_GTournament::GetTotalLevelExp()
{
	DOUBLE exp = 0;

#ifndef _MAPSERVER_
#else
	CPlayer* pPlayer = NULL;
	m_TeamMemberTable.SetPositionHead();
	while(pPlayer = (CPlayer*)m_TeamMemberTable.GetData())
	{
		exp += pPlayer->GetPlayerTotalExpPoint();
	}

#endif

	return exp;
}


int CBattleTeam_GTournament::GetRemainMember()
{
	if( m_MemberCount >= (m_DeadMemberCount-m_DeadMemberLeavecount) )
		return m_MemberCount-(m_DeadMemberCount-m_DeadMemberLeavecount);
	else
		return 0;
}

