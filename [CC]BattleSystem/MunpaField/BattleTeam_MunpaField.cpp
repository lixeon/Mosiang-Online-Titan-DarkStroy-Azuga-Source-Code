// BattleTeam_MunpaField.cpp: implementation of the CBattleTeam_MunpaField class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BattleTeam_MunpaField.h"
#include "Player.h"

#ifdef _MAPSERVER_
#include "PackedData.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBattleTeam_MunpaField::CBattleTeam_MunpaField()
{
	m_dwBattleID = 0;
}

CBattleTeam_MunpaField::~CBattleTeam_MunpaField()
{

}

void CBattleTeam_MunpaField::Initialize(DWORD dwBattleID, MUNPAFIELD_TEAMINFO* pTeamInfo)
{
	m_dwBattleID = dwBattleID;
	memcpy(&m_TeamInfo,pTeamInfo,sizeof(MUNPAFIELD_TEAMINFO));
}
	
BOOL CBattleTeam_MunpaField::IsAddableTeamMember(CObject* pObject)
{
	ySWITCH(pObject->GetObjectKind())
		yCASE(eObjectKind_Player)
			CPlayer* pPlayer = (CPlayer*)pObject;
			for(int n=0;n<MAX_MUNPAFIELD_MEMBERNUM;++n)
			{
				if(pPlayer->GetID() == m_TeamInfo.TeamMember[n].MemberID)
					return TRUE;
			}
			return FALSE;

		yCASE(eObjectKind_Monster)
			ASSERT(0);

		yCASE(eObjectKind_Npc)
			ASSERT(0);
	yENDSWITCH

	return FALSE;
}

void CBattleTeam_MunpaField::GetTeamInfo(MUNPAFIELD_TEAMINFO* pInfoOut)
{
	memcpy(pInfoOut,&m_TeamInfo,sizeof(MUNPAFIELD_TEAMINFO));	
}

void CBattleTeam_MunpaField::DoDeleteTeamMember(CObject* pObject)
{
#ifdef _MAPSERVER_
	/* YH2DO
	MSG_DWORD2 msg;
	msg.Category = MP_MURIMNET;
	msg.Protocol = MP_MURIMNET_NOTIFYTOMN_PLAYER_LOGOUT;
	msg.dwData1 = m_dwBattleID;
	msg.dwData2 = pObject->GetID();
	

	PACKEDDATA_OBJ->SendToMapServer(pObject->GetAgentNum(),99,&msg,sizeof(msg));
	*/
#endif
}