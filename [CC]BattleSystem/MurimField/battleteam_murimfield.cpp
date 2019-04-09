#include "stdafx.h"
#include "BattleTeam_MurimField.h"
#include "Player.h"

#ifdef _MAPSERVER_
#include "PackedData.h"
#endif

CBattleTeam_MurimField::CBattleTeam_MurimField()
{
	m_dwBattleID = 0;
	memset( &m_TeamInfo, 0, sizeof( MURIMFIELD_TEAMINFO ) );
}

CBattleTeam_MurimField::~CBattleTeam_MurimField()
{
}

BOOL CBattleTeam_MurimField::IsAddableTeamMember( CObject* pObject )
{
	ySWITCH(pObject->GetObjectKind())
		yCASE(eObjectKind_Player)
			CPlayer* pPlayer = (CPlayer*)pObject;
			for( int n = 0; n < MAX_MURIMFIELD_MEMBERNUM; ++n )
			{
				if( pPlayer->GetID() == m_TeamInfo.TeamMember[n].MemberID )
					return TRUE;
			}
			return FALSE;

		yCASE( eObjectKind_Monster )
			ASSERT(0);

		yCASE( eObjectKind_Npc )
			ASSERT(0);
	yENDSWITCH

	return FALSE;
}

void CBattleTeam_MurimField::DoDeleteTeamMember( CObject* pObject )
{
#ifdef _MAPSERVER_
//	MSG_DWORD2 msg;
//	msg.Category = MP_MURIMNET;
//	msg.Protocol = MP_MURIMNET_NOTIFYTOMN_PLAYER_LOGOUT;
//	msg.dwData1 = m_dwBattleID;
//	msg.dwData2 = pObject->GetID();

//	PACKEDDATA_OBJ->SendToMapServer( pObject->GetAgentNum(), 99, &msg, sizeof(msg) );
#endif
}

void CBattleTeam_MurimField::Initialize( DWORD dwBattleID, MURIMFIELD_TEAMINFO* pInfo )
{
	m_dwBattleID = dwBattleID;
	m_TeamInfo = *pInfo;
}