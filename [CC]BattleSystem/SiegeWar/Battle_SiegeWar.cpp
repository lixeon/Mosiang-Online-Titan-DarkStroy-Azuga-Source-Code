// Battle_SiegeWar.cpp: implementation of the CBattle_SiegeWar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Battle_SiegeWar.h"
#include "Object.h"
#include "ObjectStateManager.h"
#include "Player.h"
//#include "../[CC]Header/GameResourceManager.h"
#ifdef _MAPSERVER_
#include "CharMove.h"
#include "SiegeWarMgr.h"
#include "UserTable.h"
#include "skillmanager_server.h"
#include "GridSystem.h"
#else
#include "ObjectManager.h"
#include "MoveManager.h"
#include "GameIn.h"
#include "ChatDialog.h"
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBattle_SiegeWar::CBattle_SiegeWar()
{

}

CBattle_SiegeWar::~CBattle_SiegeWar()
{

}

void CBattle_SiegeWar::Initialize(BATTLE_INFO_BASE* pCreateInfo, CBattleTeam* pTeam1, CBattleTeam* pTeam2)
{
	CBattle::Initialize( pCreateInfo, pTeam1, pTeam2 );

	m_Team1Pos.x = 45574;
	m_Team1Pos.y = 0;
	m_Team1Pos.z = 45252;
	m_Team2Pos.x = 9195;
	m_Team2Pos.y = 0;
	m_Team2Pos.z = 6581;

#ifdef _MAPSERVER_	
#else
#endif
}


void CBattle_SiegeWar::OnCreate(BATTLE_INFO_BASE* pCreateInfo, CBattleTeam* pTeam1, CBattleTeam* pTeam2)
{
	CBattle::OnCreate( pCreateInfo, pTeam1, pTeam2 );

	//
/*	BATTLE_INFO_SIEGEWAR* pInfo = (BATTLE_INFO_SIEGEWAR*)pCreateInfo;
	((CBattleTeam_SiegeWar*)pTeam1)->Init( &pInfo->MemberInfo[0], eBattleTeam1 );
	((CBattleTeam_SiegeWar*)pTeam2)->Init( &pInfo->MemberInfo[1], eBattleTeam2 );*/

	memcpy( &m_BInfo, pCreateInfo, sizeof(BATTLE_INFO_SIEGEWAR) );

#ifndef _MAPSERVER_

#else

#endif
}


void CBattle_SiegeWar::OnDestroy()
{
#ifdef _MAPSERVER_
	SKILLMGR->ReleaseAllSkillInBattle(GetBattleID());
	ReleaseAllObject();

/*	ToEachTeam(pTeam)
		((CBattleTeam_GTournament*)pTeam)->AliveTeamMember();
		((CBattleTeam_GTournament*)pTeam)->ReturnToMap();
	EndToEachTeam
		m_Observer.ReturnToMap();*/
#else
//	GAMEIN->GetGTScoreInfoDlg()->EndBattle();
#endif

	//
}


void CBattle_SiegeWar::OnFightStart()
{
	CBattle::OnFightStart();

#ifdef _MAPSERVER_

#else
#endif
}


void CBattle_SiegeWar::OnTeamMemberAdd(int Team,DWORD MemberID,char* Name)
{
#ifdef _MAPSERVER_

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( MemberID );
	if( !pPlayer )		return;

	VECTOR3 vPos = Team==eBattleTeam1?m_Team1Pos:m_Team2Pos;
		
	int temp;
	temp = rand() % 700 - 350;
	vPos.x = vPos.x + temp;
	temp = rand() % 700 - 350;
	vPos.z = vPos.z + temp;
	vPos.y = 0;

	CCharMove::SetPosition( pPlayer, &vPos );
	CCharMove::CorrectPlayerPosToServer(pPlayer);

#else

#endif
}


BOOL CBattle_SiegeWar::OnTeamMemberDie(int Team,DWORD VictimMemberID,DWORD KillerID)
{
#ifdef _MAPSERVER_
	CObject* pDeadObject = g_pUserTable->FindUser( VictimMemberID );
	if( !pDeadObject )			return FALSE;

	if( pDeadObject->GetObjectKind() == eObjectKind_MapObject || 
		pDeadObject->GetObjectKind() == eObjectKind_CastleGate )
		return FALSE;

/*	if( Team < 2 )
	{
		((CBattleTeam_GTournament*)m_TeamArray[Team])->IncreaseDieCount();

		MSG_DWORD2 msg;
		SetProtocol(&msg,MP_GTOURNAMENT,MP_GTOURNAMENT_TEAMMEMBER_DIE);
		msg.Category = MP_GTOURNAMENT;
		msg.dwData1 = Team;
		msg.dwData2 = ((CBattleTeam_GTournament*)m_TeamArray[Team])->GetRemainMember();
		ToEachTeam(pTeam)
		pTeam->SendTeamMsg( &msg, sizeof(msg) );
		EndToEachTeam
		m_Observer.SendTeamMsg( &msg, sizeof(msg) );
	}*/
	OBJECTSTATEMGR_OBJ->StartObjectState( pDeadObject, eObjectState_Die, 0 );
	((CPlayer*)pDeadObject)->SetReadyToRevive( TRUE );
	
	if( pDeadObject->GetID() == SIEGEWARMGR->GetEngraveIdx() )
		SIEGEWARMGR->CancelEngraveIdx();

#else
//	if( Team < 2 )
//		GAMEIN->GetGTScoreInfoDlg()->SetTeamScore( Team, ((CBattleTeam_GTournament*)m_TeamArray[Team])->GetRemainMember() );
#endif

	return TRUE;
}


BOOL CBattle_SiegeWar::IsEnemy(CObject* pOperator,CObject* pTarget)
{
	if( pTarget->GetObjectKind() == eObjectKind_Npc )
		return FALSE;

	// 옵저버는 적이 아니다.
	if( pOperator->GetBattleTeam() == 2 || pTarget->GetBattleTeam() == 2 )
		return FALSE;

	// 안보여도 적이 아니다.
	if( pTarget->GetObjectKind() == eObjectKind_Player )
	{
#ifdef _MAPSERVER_
		if( ((CPlayer*)pTarget)->IsVisible() == FALSE )
			return FALSE;
#else
		if( ((CPlayer*)pTarget)->GetEngineObject()->IsVisible() == FALSE )
			return FALSE;
#endif
	}

	// 공격자가 사람이면
	if( pOperator->GetObjectKind() == eObjectKind_Player )
	{
		// 같은팀일때
		if( pOperator->GetBattleTeam() == pTarget->GetBattleTeam() )
		{
			// 견제냐
			if( ((CPlayer*)pOperator)->IsRestraintMode() || ((CPlayer*)pTarget)->IsRestraintMode() )
			{
				// 공성이면				
				if( pOperator->GetBattleTeam() == 1 )
				{
					// 우리동맹이 아니면 적
					if( (((CPlayer*)pOperator)->GetGuildUnionIdx() || ((CPlayer*)pTarget)->GetGuildUnionIdx()) )
					{
						if( (((CPlayer*)pOperator)->GetGuildUnionIdx() != ((CPlayer*)pTarget)->GetGuildUnionIdx()) )
							return TRUE;
						else
							return FALSE;
					}
					// 우리문파가 아니면 적
					else if( (((CPlayer*)pOperator)->GetGuildIdx() != ((CPlayer*)pTarget)->GetGuildIdx()) )
						return TRUE;
					else
						return FALSE;
				}
			}
			else
				return FALSE;
		}
		// 서로다른팀일때 타겟이 몬스터라면 적이다.
		else if( pTarget->GetObjectKind() & eObjectKind_Monster )
			return TRUE;
		// 공성 vs 수성 은 적이다.
		else
			return TRUE;
	}
	// 공격자가 몬스터이면
	else if( pOperator->GetObjectKind() & eObjectKind_Monster )
	{
		// 상대가 공성이면
		if( pTarget->GetBattleTeam() == 1 )
			return TRUE;
	}

#ifndef _MAPSERVER_
#else
#endif

	return FALSE;
}


BOOL CBattle_SiegeWar::IsFriend(CObject* pOperator,CObject* pTarget)
{
	if( pTarget->GetObjectKind() == eObjectKind_Npc )
		return TRUE;

	// 옵저버는 적이 아니다.
	if( pOperator->GetBattleTeam() == 2 || pTarget->GetBattleTeam() == 2 )
		return TRUE;

	// 안보여도 적이 아니다.
	if( pTarget->GetObjectKind() == eObjectKind_Player )
	{
#ifdef _MAPSERVER_
		if( ((CPlayer*)pTarget)->IsVisible() == FALSE )
			return FALSE;
#else
		if( ((CPlayer*)pTarget)->GetEngineObject()->IsVisible() == FALSE )
			return FALSE;
#endif
	}
	
	// 사람일때
	if( pOperator->GetObjectKind() == eObjectKind_Player )
	{
		// 같은팀이고
		if( pOperator->GetBattleTeam() == pTarget->GetBattleTeam() )
		{
			// 견제
			if( ((CPlayer*)pOperator)->IsRestraintMode() || ((CPlayer*)pTarget)->IsRestraintMode() )
			{
				// 공성일때
				if( pOperator->GetBattleTeam() == 1 )
				{
					// 우리동맹이 아니면 적
					if( (((CPlayer*)pOperator)->GetGuildUnionIdx() || ((CPlayer*)pTarget)->GetGuildUnionIdx()) )
					{
						if( (((CPlayer*)pOperator)->GetGuildUnionIdx() != ((CPlayer*)pTarget)->GetGuildUnionIdx()) )
							return FALSE;
						else
							return TRUE;
					}
					// 우리문파가 아니면 적
					if(	(((CPlayer*)pOperator)->GetGuildIdx() != ((CPlayer*)pTarget)->GetGuildIdx()) )
						return FALSE;
					else
						return TRUE;
				}
				else
					return TRUE;
			}
			else
				return TRUE;
		}
		// 다른팀일때 몬스터면 친구가 아니다.
		else if( pTarget->GetObjectKind() & eObjectKind_Monster )
			return FALSE;
		// 공성 vs 수성은 전부 친구가 아니다.
		else
			return FALSE;
	}
	// 몬스터면
	else if( pOperator->GetObjectKind() & eObjectKind_Monster )
	{
		// 상대가 공성이면 친구가 아니다.
		if( pTarget->GetBattleTeam() == 1 )
			return FALSE;
	}

#ifndef _MAPSERVER_
#else
#endif

	return TRUE;
}

DWORD CBattle_SiegeWar::GetBattleTeamID( CObject* pObject )
{
#ifdef _MAPSERVER_
	for(int n=0; n<eBattleTeam_Max; ++n)
	{
		if( m_TeamArray[n] && m_TeamArray[n]->IsTeamMember( pObject ) )		
			return n;
	}
	
	if( m_Observer.IsTeamMember( pObject ) )
		return 2;
#else

	for(DWORD i=0; i<m_BInfo.DefenceCount; ++i)
		if( m_BInfo.GuildList[i] == ((CPlayer*)pObject)->GetGuildIdx() )
			return 0;

	for(i=0; i<m_BInfo.AttackCount; ++i)
		if( m_BInfo.GuildList[i+m_BInfo.DefenceCount] == ((CPlayer*)pObject)->GetGuildIdx() )
			return 1;

#endif
	return 2;
}


#ifdef _MAPSERVER_
void CBattle_SiegeWar::GetBattleInfo(char* pInfo,WORD* size)
{
	DWORD count = 0;
	ToEachTeam(pTeam)
		if( pTeam )
		{
			// 동맹, 협력문파 정보등을 추가.

			++count;
		}
	EndToEachTeam

	memcpy( pInfo, &m_BInfo, sizeof(BATTLE_INFO_SIEGEWAR) );
	*size = m_BInfo.GetSize();
}
BOOL CBattle_SiegeWar::AddObjectToBattle(CObject* pObject)
{
	BOOL rt = FALSE;
	int n = 0;
	DWORD Param = 0;	
	if( m_Observer.IsTeamMember( pObject ) )
		goto AddObserver;

	if( pObject->GetObjectKind() == eObjectKind_CastleGate )
	{
		pObject->SetBattle( GetBattleID(), eBattleTeam1, Param );
		pObject->SetGridID( m_BattleInfo.BattleID );
		
		rt = m_TeamArray[0]->AddTeamMember(pObject);
		ASSERT(rt);
		
		return TRUE;
	}

	for( n=0; n<eBattleTeam_Max; ++n )
	{
		if( m_TeamArray[n] )
		if( m_TeamArray[n]->IsAddableTeamMember( pObject ) )
		{
			if( pObject->GetObjectKind() == eObjectKind_Player )
			{
				if( ((CPlayer*)pObject)->GetGuildUnionIdx() )
					Param = ((CPlayer*)pObject)->GetGuildUnionIdx();
				else if( ((CPlayer*)pObject)->GetGuildIdx() )
					Param = ((CPlayer*)pObject)->GetGuildIdx();
			}

			pObject->SetBattle( GetBattleID(), n, Param );
			pObject->SetGridID( m_BattleInfo.BattleID );
			
			rt = m_TeamArray[n]->AddTeamMember(pObject);
			ASSERT(rt);
			
			OnTeamMemberAdd(n,pObject->GetID(),pObject->GetObjectName());

			if(pObject->GetObjectKind() == eObjectKind_Player && rt)
				SendBattleInfo((CPlayer*)pObject);
			
			return TRUE;
		}
	}

AddObserver:
	// Observer
	pObject->SetBattle( GetBattleID(), 2 );
	pObject->SetGridID( m_BattleInfo.BattleID );
	rt = m_Observer.AddTeamMember( pObject );
	
	OnTeamMemberAdd( 2, pObject->GetID(), pObject->GetObjectName() );

	if(pObject->GetObjectKind() == eObjectKind_Player && rt)
		SendBattleInfo((CPlayer*)pObject);

	return TRUE;
}
BOOL CBattle_SiegeWar::DeleteObjectFromBattle(CObject* pObject)
{
	int Team = pObject->GetBattleTeam();
	if( Team == 2 )
	{
		m_Observer.DeleteTeamMember( pObject );
	}
	else if( Team < 2 )
	{
		ToEachTeam(pTeam)
			pTeam->DeleteTeamMember( pObject );
		EndToEachTeam
		
/*		MSG_DWORD2 msg;
		SetProtocol(&msg,MP_GTOURNAMENT,MP_GTOURNAMENT_TEAMMEMBER_OUT);
		msg.Category = MP_GTOURNAMENT;
		msg.dwData1 = Team;
		msg.dwData2 = ((CBattleTeam_GTournament*)m_TeamArray[Team])->GetRemainMember();
		ToEachTeam(pTeam)
			pTeam->SendTeamMsg( &msg, sizeof(msg) );
		EndToEachTeam
			m_Observer.SendTeamMsg( &msg, sizeof(msg) );*/
	}

	return TRUE;
}
DWORD CBattle_SiegeWar::GetTeamMemberNum(int i)
{
	return 0;
}
BOOL CBattle_SiegeWar::AddObserverToBattle(CObject* pObject)
{
	BOOL rt = FALSE;
	
	// Observer
	pObject->SetBattle( GetBattleID(), 2 );
	pObject->SetGridID( m_BattleInfo.BattleID );
	rt = m_Observer.AddTeamMember( pObject );
	
	OnTeamMemberAdd( 2, pObject->GetID(), pObject->GetObjectName() );

	if(pObject->GetObjectKind() == eObjectKind_Player && rt)
		SendBattleInfo((CPlayer*)pObject);

	return TRUE;
}
void CBattle_SiegeWar::ReleaseAllObject()
{
	ToEachTeam(pTeam)
		YHTPOSITION pos = pTeam->GetPositionHead();
		while( CObject* pObject = pTeam->GetNextTeamMember( &pos ) )
		{
			if( pObject->GetObjectKind() == eObjectKind_Player )
				CCharMove::ReleaseMove( pObject );
			else if( pObject->GetObjectKind() & eObjectKind_Monster )
				g_pServerSystem->RemoveMonster( pObject->GetID() );
		}
	EndToEachTeam
	YHTPOSITION pos = m_Observer.GetPositionHead();
	while( CObject* pObject = m_Observer.GetNextTeamMember( &pos ) )
	{
		if( pObject->GetObjectKind() == eObjectKind_Player )
			CCharMove::ReleaseMove( pObject );
		else if( pObject->GetObjectKind() & eObjectKind_Monster )
			g_pServerSystem->RemoveMonster( pObject->GetID() );
	}
}
void CBattle_SiegeWar::SetBattleInfo( BATTLE_INFO_SIEGEWAR* pInfo )
{
	memcpy( &m_BInfo, pInfo, sizeof(BATTLE_INFO_SIEGEWAR) );
	if( m_TeamArray[0] )
		((CBattleTeam_SiegeWar*)m_TeamArray[0])->SetTeamInfo( pInfo->DefenceCount, &pInfo->GuildList[0] );
	if( m_TeamArray[1] )
		((CBattleTeam_SiegeWar*)m_TeamArray[1])->SetTeamInfo( pInfo->AttackCount, &pInfo->GuildList[pInfo->DefenceCount] );
}
void CBattle_SiegeWar::BattleTeamChange()
{
	DWORD curTeam = 0;
	DWORD Playerteam = 0;
	DWORD Changeplayer[2][MAX_SWTEAM_USERCOUNT];
	DWORD ChangePlayerCount[2] = { 0, 0 };

	ToEachTeam(pTeam)
		YHTPOSITION pos = pTeam->GetPositionHead();
		while( CObject* pObject = pTeam->GetNextTeamMember( &pos ) )
		{
			if( pObject->GetObjectKind() == eObjectKind_Player )
			{
				curTeam = pObject->GetBattleTeam();
				Playerteam = SIEGEWARMGR->GetGuildTeamIdx( ((CPlayer*)pObject)->GetGuildIdx() );
				if( Playerteam >= 2 )
				{
					// 플레이어를 옵저버로 보내버린다.
					pObject->SetBattleTeam( 2 );
					continue;
				}
				else if( pObject->GetBattleTeam() != Playerteam )
				{
					Changeplayer[curTeam][ChangePlayerCount[curTeam]] = pObject->GetID();
					++ChangePlayerCount[curTeam];
					pObject->SetBattleTeam( Playerteam );
				}
			}
			else if( pObject->GetObjectKind() & eObjectKind_Monster )
				g_pServerSystem->RemoveMonster( pObject->GetID() );
		}
	EndToEachTeam

	// 팀을 옮겨준다.
	for(DWORD i=0; i<ChangePlayerCount[0]; ++i)
	{
		CObject* pObject = g_pUserTable->FindUser( Changeplayer[0][i] );
		if( !pObject )			continue;

		if( m_TeamArray[0] )
			m_TeamArray[0]->DeleteTeamMember( pObject );
		if( m_TeamArray[1] )
			m_TeamArray[1]->AddTeamMember( pObject );
	}
	for(i=0; i<ChangePlayerCount[1]; ++i)
	{
		CObject* pObject = g_pUserTable->FindUser( Changeplayer[1][i] );
		if( !pObject )			continue;

		if( m_TeamArray[1] )
			m_TeamArray[1]->DeleteTeamMember( pObject );
		if( m_TeamArray[0] )
			m_TeamArray[0]->AddTeamMember( pObject );		
	}
}
void CBattle_SiegeWar::SendBattleInfoToPlayer()
{
	DWORD team = 0;
	ToEachTeam(pTeam)
		((CBattleTeam_SiegeWar*)pTeam)->SendBattlInfoToPlayer( &m_BInfo, team );
		++team;
	EndToEachTeam
}
void CBattle_SiegeWar::BattleTeamPositionChange()
{	
	VECTOR3 vPos;
	DWORD	nTeam = 0;
	
	ToEachTeam(pTeam)
		YHTPOSITION pos = pTeam->GetPositionHead();
		while( CObject* pObject = pTeam->GetNextTeamMember( &pos ) )
		{
			// Position 좌표를 결정해서 뿌려줘야함.
			if( nTeam == 0 )				vPos = m_Team1Pos;
			else if( nTeam == 1 )			vPos = m_Team2Pos;
			CCharMove::SetPosition( pObject, &vPos );
			CCharMove::InitMove( pObject, &vPos );
			//CCharMove::CorrectPlayerPosToServer( pObject );

			g_pServerSystem->GetGridSystem()->ChangeGrid( pObject, &vPos );
			pTeam->DeleteTeamMember( pObject );
		}
		++nTeam;
	EndToEachTeam
}
void CBattle_SiegeWar::ReturnToMapAllPlayer()
{
	ToEachTeam(pTeam)
		((CBattleTeam_SiegeWar*)pTeam)->ReturnToMap();
	EndToEachTeam

	m_Observer.ReturnToMap();
}

#else

void CBattle_SiegeWar::Render()
{
}

void CBattle_SiegeWar::SetBattleInfo( SEND_SW_BTGUILDLIST* pInfo )
{
	m_BInfo.DefenceCount = pInfo->DefenceCount;
	m_BInfo.AttackCount = pInfo->AttackCount;
	memcpy( m_BInfo.GuildList, pInfo->GuildList, sizeof(DWORD)*(pInfo->DefenceCount+pInfo->AttackCount) );	

	DWORD count = 0;
	ToEachTeam(pTeam)
		if( count == 0 )
			((CBattleTeam_SiegeWar*)pTeam)->SetTeamInfo( pInfo->DefenceCount, &pInfo->GuildList[0] );
		else if( count == 1 )
			((CBattleTeam_SiegeWar*)pTeam)->SetTeamInfo( pInfo->AttackCount, &pInfo->GuildList[pInfo->DefenceCount] );
		++count;
	EndToEachTeam
}

BOOL CBattle_SiegeWar::IsDefenceGuild( DWORD GuildIdx )
{
	for(DWORD i=0; i<m_BInfo.DefenceCount; ++i)
	{
		if( m_BInfo.GuildList[i] == GuildIdx )
			return TRUE;
	}

	return FALSE;
}

void CBattle_SiegeWar::SetSiegeWarName( CObject* pObject, DWORD Team, DWORD GuildIdx )
{
	CPlayer* pPlayer = (CPlayer*)pObject;

	if( pPlayer->GetID() == HEROID )
	{
		// 처음 들어올때 Hero는 팀정보가 없는게 아닌가?
		if( Team == eBattleTeam1 )
			pPlayer->SetSiegeName( eSiegeWarNameBox_CastleGuild );
		else if( Team == eBattleTeam2 )
			pPlayer->SetSiegeName( eSiegeWarNameBox_SiegeGuild );

		pPlayer->SetBattleTeam( Team );
	}
	else if( HERO->GetBattleTeam() == eBattleTeam1 )
	{
		if( Team == eBattleTeam1 )
			pPlayer->SetSiegeName( eSiegeWarNameBox_CastleGuild );
		else if( Team == eBattleTeam2 )
			pPlayer->SetSiegeName( eSiegeWarNameBox_Enermy );
		else if( Team == 2 )
		{
		}
	}
	else if( HERO->GetBattleTeam() == eBattleTeam2 )
	{
		if( Team == eBattleTeam1 )
		{
			pPlayer->SetSiegeName( eSiegeWarNameBox_Enermy );
		}
		else if( Team == eBattleTeam2 )
		{
			if( HERO->GetGuildIdx() == GuildIdx )
				pPlayer->SetSiegeName( eSiegeWarNameBox_SiegeGuild );
			else if( HERO->GetGuildUnionIdx() && HERO->GetGuildUnionIdx() == pPlayer->GetGuildUnionIdx() )
				pPlayer->SetSiegeName( eSiegeWarNameBox_SiegeGuild );
			else if( pPlayer->IsRestraintMode() || HERO->IsRestraintMode() )
				pPlayer->SetSiegeName( eSiegeWarNameBox_Enermy );			
			else
				pPlayer->SetSiegeName( eSiegeWarNameBox_SiegeGuild );
		}
		else if( Team == 2 )
		{
		}
	}
	else if( HERO->GetBattleTeam() == 2 )
	{
		if( Team == eBattleTeam1 )
			pPlayer->SetSiegeName( eSiegeWarNameBox_CastleGuild );
		else if( Team == eBattleTeam2 )
			pPlayer->SetSiegeName( eSiegeWarNameBox_SiegeGuild );
		else if( Team == 2 )
		{
		}
	}
}


#endif



void CBattle_SiegeWar::StartBattle()
{
	OnFightStart();

#ifdef _MAPSERVER_

	MSG_DWORD msg;
	msg.Category = MP_BATTLE;
	msg.Protocol = MP_BATTLE_START_NOTIFY;
	msg.dwData = GetBattleID();

	ToEachTeam(pTeam)
		pTeam->SendTeamMsg(&msg,sizeof(msg));
	EndToEachTeam
	m_Observer.SendTeamMsg( &msg, sizeof(msg) );
#else

#endif
}


void CBattle_SiegeWar::EndBattle()
{
#ifdef _MAPSERVER_

#else

#endif
}
