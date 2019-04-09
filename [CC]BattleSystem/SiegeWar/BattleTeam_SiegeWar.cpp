// BattleTeam_SiegeWar.cpp: implementation of the CBattleTeam_SiegeWar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleTeam_SiegeWar.h"

#include "Player.h"

#ifdef _MAPSERVER_
#include "Object.h"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBattleTeam_SiegeWar::CBattleTeam_SiegeWar()
{
	m_GuildCount = 0;
	memset( m_GuildList, 0, sizeof(DWORD)*SIEGEWAR_MAXGUILDCOUNT_PERTEAM );
}

CBattleTeam_SiegeWar::~CBattleTeam_SiegeWar()
{

}


void CBattleTeam_SiegeWar::Init( DWORD Count, DWORD* pGuildList )
{
	m_GuildCount = Count;
	memcpy( m_GuildList, pGuildList, sizeof(DWORD)*m_GuildCount );	
}


BOOL CBattleTeam_SiegeWar::IsAddableTeamMember(CObject* pObject)
{
	if( !pObject )			return FALSE;

	if( pObject->GetObjectKind() == eObjectKind_Player )
	{		
		for(DWORD i=0; i<m_GuildCount; ++i)
		{
			if( m_GuildList[i] == ((CPlayer*)pObject)->GetGuildIdx() )
				return TRUE;		
		}
	}
	// 추가처리해야 함
	else if( pObject->GetObjectKind() & eObjectKind_Monster )
		return TRUE;

	return FALSE;
}


void CBattleTeam_SiegeWar::SetTeamInfo( DWORD Count, DWORD* pGuildList)
{
	m_GuildCount = Count;
	memcpy( m_GuildList, pGuildList, sizeof(DWORD)*m_GuildCount );
}


void CBattleTeam_SiegeWar::SendBattlInfoToPlayer( BATTLE_INFO_SIEGEWAR* pInfo, DWORD Team )
{
#ifdef _MAPSERVER_
	CObject* pObject = NULL;
	m_TeamMemberTable.SetPositionHead();
	while( pObject = m_TeamMemberTable.GetData() )
	{
		if( pObject->GetObjectKind() != eObjectKind_Player )		continue;

		pObject->SetBattleTeam( Team );

		SEND_SW_BTGUILDLIST msg;
		SetProtocol( &msg, MP_SIEGEWAR, MP_SIEGEWAR_BATTLEINFO );
		msg.Team = (WORD)Team;
		msg.DefenceCount = (WORD)pInfo->DefenceCount;
		msg.AttackCount = (WORD)pInfo->AttackCount;
		memcpy( msg.GuildList, pInfo->GuildList, sizeof(DWORD)*(msg.DefenceCount+msg.AttackCount) );
		pObject->SendMsg( &msg, msg.GetSize() );		
	}	
#endif
}


void CBattleTeam_SiegeWar::ReturnToMap()
{
#ifdef _MAPSERVER_
	CPlayer* pPlayer = NULL;
	CObject* p = NULL;
	m_TeamMemberTable.SetPositionHead();

	cPtrList list;

	while(p = m_TeamMemberTable.GetData())
	{
		if( p->GetObjectKind() != eObjectKind_Player )
			continue;

		pPlayer = (CPlayer*)p;

		MSG_DWORD msg;
		SetProtocol( &msg, MP_SIEGEWAR, MP_SIEGEWAR_RETURNTOMAP );
		msg.dwData = pPlayer->GetReturnMapNum();
		pPlayer->SendMsg( &msg, sizeof(msg) );

		list.AddTail(pPlayer);
	}

	PTRLISTPOS pos = list.GetHeadPosition();
	while( pos )
	{
		p = (CObject*)list.GetNext(pos);
		if( p->GetObjectKind() != eObjectKind_Player )
			continue;

		pPlayer = (CPlayer*)p;
		g_pServerSystem->RemovePlayer(pPlayer->GetID());
	}
	list.RemoveAll();

	m_TeamMemberTable.RemoveAll();

#endif
}

/*
BOOL CBattleTeam_SiegeWar::IsCooperationMunpa( DWORD GuildIdx )
{
	for(DWORD i=0; i<m_CooperationCount; ++i)
	{
		if( m_pCooperationMunpaArray[i] == GuildIdx )
			return TRUE;
	}
	
	return FALSE;
}


BOOL CBattleTeam_SiegeWar::IsUnion( DWORD UnionIdx )
{
	for(DWORD i=0; i<m_UnionCount; ++i)
	{
		if( m_pUnionArray[i] == UnionIdx )
			return TRUE;
	}
	
	return FALSE;
}
*/

