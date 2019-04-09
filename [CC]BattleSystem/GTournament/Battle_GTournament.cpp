// Battle_GTournament.cpp: implementation of the CBattle_GTournament class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Battle_GTournament.h"
#include "Object.h"
#include "ObjectStateManager.h"
#include "Player.h"
#include "../[CC]Header/GameResourceManager.h"


#ifdef _MAPSERVER_
#include "UserTable.h"
#include "SkillManager_Server.h"
#include "CharMove.h"
#include "PackedData.h"
#else
#include "MHMap.h"
#include "TileManager.h"
#include "../[Client]MH/ObjectManager.h"
//#include "./[Client]MH/interface/cWindowManager.h"
#include "../[Client]MH/Interface/cScriptManager.h"
#include "GameIn.h"
#include "GTScoreInfoDialog.h"
#include "ChatManager.h"
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBattle_GTournament::CBattle_GTournament()
{
#ifndef _MAPSERVER_
	m_pCurShowImage	= NULL;
#endif

}


CBattle_GTournament::~CBattle_GTournament()
{

}


void CBattle_GTournament::Initialize(BATTLE_INFO_BASE* pCreateInfo, CBattleTeam* pTeam1, CBattleTeam* pTeam2)
{
	CBattle::Initialize( pCreateInfo, pTeam1, pTeam2 );

	// Player 대기위치
	m_Team1Pos.x = 2826;
	m_Team1Pos.y = 0;
	m_Team1Pos.z = 5833;
	m_Team2Pos.x = 17374;
	m_Team2Pos.y = 0;
	m_Team2Pos.z = 5833;
	m_ObserverPos.x = 10100;
	m_ObserverPos.y = 0;
	m_ObserverPos.z = 5833;

#ifdef _MAPSERVER_

#else
	m_RenderTime = 5000;
#endif
}


void CBattle_GTournament::OnCreate(BATTLE_INFO_BASE* pCreateInfo, CBattleTeam* pTeam1, CBattleTeam* pTeam2)
{
	CBattle::OnCreate( pCreateInfo, pTeam1, pTeam2 );

	//
	BATTLE_INFO_GTOURNAMENT* pInfo = (BATTLE_INFO_GTOURNAMENT*)pCreateInfo;
	((CBattleTeam_GTournament*)pTeam1)->Init( &pInfo->MemberInfo, eBattleTeam1 );
	((CBattleTeam_GTournament*)pTeam2)->Init( &pInfo->MemberInfo, eBattleTeam2 );

	memcpy( &m_GTInfo, pCreateInfo, sizeof(BATTLE_INFO_GTOURNAMENT) );

#ifndef _MAPSERVER_

	m_GuildIdx[0] = pInfo->MemberInfo.TeamGuildIdx[0];
	m_GuildIdx[1] = pInfo->MemberInfo.TeamGuildIdx[1];

	SCRIPTMGR->GetImage( 61, &m_ImageWin, PFT_HARDPATH );
	SCRIPTMGR->GetImage( 62, &m_ImageLose, PFT_HARDPATH );
	SCRIPTMGR->GetImage( 76, &m_ImageStart, PFT_HARDPATH );

	m_vTitlePos.x = (1024 - 512) / 2;	//128 width
	m_vTitlePos.y = 32;
	m_vTitleScale.x = 1.0f;
	m_vTitleScale.y = 1.0f;

	if( pInfo->BattleState == eBATTLE_STATE_READY )
		CreateWaitPlace();
#else

#endif
}


void CBattle_GTournament::OnDestroy()
{
#ifdef _MAPSERVER_
	SKILLMGR->ReleaseAllSkillInBattle(GetBattleID());
	ReleaseAllObject();

	ToEachTeam(pTeam)
		((CBattleTeam_GTournament*)pTeam)->AliveTeamMember();
		((CBattleTeam_GTournament*)pTeam)->ReturnToMap();
	EndToEachTeam
		m_Observer.ReturnToMap();
#else
	GAMEIN->GetGTScoreInfoDlg()->EndBattle();
#endif

	//
}


BOOL CBattle_GTournament::OnTeamMemberDie(int Team,DWORD VictimMemberID,DWORD KillerID)
{
#ifdef _MAPSERVER_
	CObject* pDeadObject = g_pUserTable->FindUser( VictimMemberID );
	if( !pDeadObject )			return FALSE;

	if( Team < 2 )
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
	}
	OBJECTSTATEMGR_OBJ->StartObjectState( pDeadObject, eObjectState_Die, 0 );
	((CPlayer*)pDeadObject)->SetReadyToRevive( FALSE );
#else
//	if( Team < 2 )
//		GAMEIN->GetGTScoreInfoDlg()->SetTeamScore( Team, ((CBattleTeam_GTournament*)m_TeamArray[Team])->GetRemainMember() );
#endif

	return TRUE;
}


void CBattle_GTournament::StartBattle()
{
	m_GTInfo.m_dwEntranceTime = 0;
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
	// 시작
	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(974) );

#endif
}


void CBattle_GTournament::OnTick()
{
	ySWITCH(m_BattleInfo.BattleState)
		yCASE(eBATTLE_STATE_READY)
			if( gTickTime < m_GTInfo.m_dwEntranceTime)
			{
				m_GTInfo.m_dwEntranceTime -= gTickTime;
			}
			else
			{
				m_GTInfo.m_dwEntranceTime = 0;
			}
		yCASE(eBATTLE_STATE_FIGHT)
			if( gTickTime < m_GTInfo.m_dwFightTime)
			{
				m_GTInfo.m_dwFightTime -= gTickTime;
				Judge();
			}
			else
			{
				m_GTInfo.m_dwFightTime = 0;
				Judge();
			}
		yCASE(eBATTLE_STATE_RESULT)
			if( gTickTime < m_GTInfo.m_dwLeaveTime )
			{
				m_GTInfo.m_dwLeaveTime -= gTickTime;
			}
			else
			{
				m_GTInfo.m_dwLeaveTime = 0;

				// 일단 우선 내보낸다.
				SetDestroyFlag();
			}
	yENDSWITCH
}



void CBattle_GTournament::OnFightStart()
{
	CBattle::OnFightStart();

#ifdef _MAPSERVER_

#else
	DestroyWaitPlace();
	GAMEIN->GetGTScoreInfoDlg()->StartBattle();
	m_RenderTime = 5000;
#endif
}



BOOL CBattle_GTournament::Judge()
{
	// 문파원 체크
	if( JudgeMemberExist() )
		return TRUE;

	// 시간이 끝났을때
	if( m_GTInfo.m_dwFightTime == 0 )
	{
		if( ((CBattleTeam_GTournament*)m_TeamArray[eBattleTeam1])->GetRemainMember() > 
			((CBattleTeam_GTournament*)m_TeamArray[eBattleTeam2])->GetRemainMember() )
			Victory( eBattleTeam1, eBattleTeam2 );
		else if( ((CBattleTeam_GTournament*)m_TeamArray[eBattleTeam2])->GetRemainMember() > 
			((CBattleTeam_GTournament*)m_TeamArray[eBattleTeam1])->GetRemainMember() )
			Victory( eBattleTeam2, eBattleTeam1 );
		else
		{
			if( JudgeMemberKill() == FALSE )			// 많이 죽인 Kill 수
			if( JudgeMemberLevel() == FALSE )			// 살아남은 사람의 Level의 합이 낮은쪽
			if( JudgeMemberLevelExp() == FALSE )		// 살아남은 사람의 경험치 총합이 낮은쪽
			{
				// Random
				DWORD data = rand()%2;
				Victory( data, 1-data );
			}
		}
	}

	return FALSE;
}


BOOL CBattle_GTournament::JudgeMemberExist()
{
	if ( ((CBattleTeam_GTournament*)m_TeamArray[eBattleTeam1])->GetRemainMember() == 0 )
	{
		if( ((CBattleTeam_GTournament*)m_TeamArray[eBattleTeam2])->GetRemainMember() == 0 )
		{
			LoseAll();
			return TRUE;
		}
		else
		{
			Victory( eBattleTeam2, eBattleTeam1 );
			return TRUE;
		}
	}

	if( ((CBattleTeam_GTournament*)m_TeamArray[eBattleTeam2])->GetRemainMember() == 0 )
	{
		if( ((CBattleTeam_GTournament*)m_TeamArray[eBattleTeam1])->GetRemainMember() == 0 )
		{
			LoseAll();
			return TRUE;
		}
		else
		{
			Victory( eBattleTeam1, eBattleTeam2 );			
			return TRUE;
		}
	}

	return FALSE;
}


// 3
BOOL CBattle_GTournament::JudgeMemberKill()
{
	DWORD data1 = ((CBattleTeam_GTournament*)m_TeamArray[eBattleTeam1])->GetDieCount();
	DWORD data2 = ((CBattleTeam_GTournament*)m_TeamArray[eBattleTeam2])->GetDieCount();

	if( data1 == data2 )			return FALSE;
	else if( data1 > data2 )
		Victory( eBattleTeam2, eBattleTeam1 );
	else
		Victory( eBattleTeam1, eBattleTeam2 );

	return TRUE;
}
// 4
BOOL CBattle_GTournament::JudgeMemberLevel()
{
	DWORD data1 = ((CBattleTeam_GTournament*)m_TeamArray[eBattleTeam1])->GetTotalLevel();
	DWORD data2 = ((CBattleTeam_GTournament*)m_TeamArray[eBattleTeam2])->GetTotalLevel();

	if( data1 == data2 )			return FALSE;
	else if( data1 > data2 )
		Victory( eBattleTeam2, eBattleTeam1 );
	else
		Victory( eBattleTeam1, eBattleTeam2 );

	return TRUE;
}
// 5
BOOL CBattle_GTournament::JudgeMemberLevelExp()
{
	DOUBLE data1 = ((CBattleTeam_GTournament*)m_TeamArray[eBattleTeam1])->GetTotalLevelExp();
	DOUBLE data2 = ((CBattleTeam_GTournament*)m_TeamArray[eBattleTeam2])->GetTotalLevelExp();

	if( data1 == data2 )			return FALSE;
	else if( data1 > data2 )
		Victory( eBattleTeam2, eBattleTeam1 );
	else
		Victory( eBattleTeam1, eBattleTeam2 );

	return TRUE;
}



void CBattle_GTournament::Victory(int WinnerTeamNum,int LoserTeamNum)
{
	CBattle::Victory( WinnerTeamNum, LoserTeamNum );

	((CBattleTeam_GTournament*)m_TeamArray[WinnerTeamNum])->SetResult( TRUE );
	((CBattleTeam_GTournament*)m_TeamArray[LoserTeamNum])->SetResult( FALSE );

#ifndef _MAPSERVER_
	if( HERO->GetBattleTeam() == WinnerTeamNum )
	{
		m_pCurShowImage = &m_ImageWin;
	}
	else
	{
		m_pCurShowImage = &m_ImageLose;
	}

	m_vTitleScale.x = 1.0f;
	m_vTitleScale.y = 1.0f;
	m_vTitlePos.x = (1024 - 512 * m_vTitleScale.x ) / 2;	//128 = width
	m_vTitlePos.y = 100;
	m_RenderTime = 10000;

	//AutoAttack멈춤
	HERO->DisableAutoAttack();			//공격중 이던거 취소되나?
	HERO->SetNextAction(NULL);			//스킬 쓰던것도 취소되나?
	
/*	if( HERO->GetBattleTeam() == WinnerTeamNum )
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(970) );
	else
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(971) );*/
#else

	MSG_DWORD msg;
	SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_BATEND_TOMAP );
	msg.dwData = m_BattleInfo.BattleID;
	PACKEDDATA_OBJ->SendToBroadCastMapServer( &msg, sizeof(msg) );

#endif

}


void CBattle_GTournament::Draw()
{
	CBattle::Draw();

#ifndef _MAPSERVER_
	


#endif

}


void CBattle_GTournament::LoseAll()
{
	// 양쪽다 진상태.
#ifdef _MAPSERVER_

	m_WinnerTeam = 2;
	m_BattleInfo.BattleState = eBATTLE_STATE_RESULT;
	m_BattleInfo.BattleTime = gCurTime;

	((CBattleTeam_GTournament*)m_TeamArray[eBattleTeam1])->SetResult( FALSE );
	((CBattleTeam_GTournament*)m_TeamArray[eBattleTeam2])->SetResult( FALSE );

	MSG_DWORD msg;
	SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_BATEND_TOMAP );
	msg.dwData = m_BattleInfo.BattleID;
	PACKEDDATA_OBJ->SendToBroadCastMapServer( &msg, sizeof(msg) );

#else

#endif

}


void CBattle_GTournament::OnTeamMemberAdd(int Team,DWORD MemberID,char* Name)
{
#ifdef _MAPSERVER_

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( MemberID );
	if( !pPlayer )		return;

	VECTOR3 vPos = Team==eBattleTeam1?m_Team1Pos:Team==eBattleTeam2?m_Team2Pos:m_ObserverPos;

	CCharMove::SetPosition( pPlayer, &vPos );
	CCharMove::CorrectPlayerPosToServer(pPlayer);

	if( Team < 2 )
	{
		MSG_DWORD2 msg;
		SetProtocol(&msg,MP_GTOURNAMENT,MP_GTOURNAMENT_TEAMMEMBER_ADD);
		msg.Category = MP_GTOURNAMENT;
		msg.dwData1 = Team;
		msg.dwData2 = ((CBattleTeam_GTournament*)m_TeamArray[Team])->GetRemainMember();
		ToEachTeam(pTeam)
			pTeam->SendTeamMsg( &msg, sizeof(msg) );
		EndToEachTeam
			m_Observer.SendTeamMsg( &msg, sizeof(msg) );
	}

#else

#endif
}


DWORD CBattle_GTournament::GetBattleTeamID( CObject* pObject )
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
	for(int n=0; n<eBattleTeam_Max; ++n)
	{
		if( ((CPlayer*)pObject)->GetGuildIdx() == m_GuildIdx[n] )
			return n;
	}
#endif
	return 2;
}


BOOL CBattle_GTournament::IsEnemy(CObject* pOperator,CObject* pTarget)
{
	if( m_BattleInfo.BattleState != eBATTLE_STATE_FIGHT )
		return FALSE;
	if( pOperator->GetObjectKind() != eObjectKind_Player ||
		pTarget->GetObjectKind() != eObjectKind_Player )
		return FALSE;
	if( pOperator->GetBattleID() != pTarget->GetBattleID() )
		return FALSE;
	if( pOperator->GetBattleTeam() == 2 )
		return FALSE;

#ifndef _MAPSERVER_
	if( pTarget->GetBattleTeam() != 2 && pOperator->GetBattleTeam() != pTarget->GetBattleTeam() )
		return TRUE;
#else
	if( m_TeamArray[0]->IsTeamMember( pOperator ) )
		return m_TeamArray[1]->IsTeamMember( pTarget );
	else if( m_TeamArray[1]->IsTeamMember( pOperator ) )
		return m_TeamArray[0]->IsTeamMember( pTarget );
#endif

	return FALSE;
}



BOOL CBattle_GTournament::IsFriend(CObject* pOperator,CObject* pTarget)
{
	if( m_BattleInfo.BattleState != eBATTLE_STATE_FIGHT )
		return TRUE;
	if( pOperator->GetObjectKind() != eObjectKind_Player ||
		pTarget->GetObjectKind() != eObjectKind_Player )
		return FALSE;
	if( pOperator->GetBattleID() != pTarget->GetBattleID() )
		return FALSE;

#ifndef _MAPSERVER_
	if( pOperator->GetBattleTeam() == pTarget->GetBattleTeam() )
		return TRUE;	
#else
	if( m_TeamArray[0]->IsTeamMember( pOperator ) )
		return !m_TeamArray[1]->IsTeamMember( pTarget );
	else if( m_TeamArray[1]->IsTeamMember( pOperator ) )
		return !m_TeamArray[0]->IsTeamMember( pTarget );
#endif

	return TRUE;
}


void CBattle_GTournament::ReturnToMap( DWORD playerID )
{
	m_Observer.ReturnToMap( playerID );
}


#ifdef _MAPSERVER_
BOOL CBattle_GTournament::AddObjectToBattle(CObject* pObject)
{
	BOOL rt = FALSE;
	int n = 0;
	if( m_BattleInfo.BattleState != eBATTLE_STATE_READY )
		goto AddObserver;
	if( m_Observer.IsTeamMember( pObject ) )
		goto AddObserver;
	
	for( n=0; n<eBattleTeam_Max; ++n )
	{
		if( m_TeamArray[n]->IsAddableTeamMember( pObject ) )
		{
			pObject->SetBattle( GetBattleID(), n );
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
BOOL CBattle_GTournament::AddObserverToBattle(CObject* pObject)
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
BOOL CBattle_GTournament::DeleteObjectFromBattle(CObject* pObject)
{
//	CBattle::DeleteObjectFromBattle( pObject );
	
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
		
		MSG_DWORD2 msg;
		SetProtocol(&msg,MP_GTOURNAMENT,MP_GTOURNAMENT_TEAMMEMBER_OUT);
		msg.Category = MP_GTOURNAMENT;
		msg.dwData1 = Team;
		msg.dwData2 = ((CBattleTeam_GTournament*)m_TeamArray[Team])->GetRemainMember();
		ToEachTeam(pTeam)
			pTeam->SendTeamMsg( &msg, sizeof(msg) );
		EndToEachTeam
			m_Observer.SendTeamMsg( &msg, sizeof(msg) );
	}

	return TRUE;
}
DWORD CBattle_GTournament::GetTeamMemberNum(int i)
{
	if( i == 0 || i == 1 )
	{
		if( m_TeamArray[i] )
			return ((CBattleTeam_GTournament*)m_TeamArray[i])->GetRemainMember();
		else
			return 0;
	}
	else if( i == 2 )
		return m_Observer.GetRemainMember();
	
	return 0;
}
void CBattle_GTournament::GetBattleInfo(char* pInfo,WORD* size)
{
	DWORD count = 0;
	ToEachTeam(pTeam)
		if( pTeam )
		{
			m_GTInfo.MemberInfo.TeamGuildIdx[count] = ((CBattleTeam_GTournament*)pTeam)->GetGuildIdx();
			m_GTInfo.MemberInfo.TeamMember[count] = (WORD)pTeam->GetMemberNum();
			m_GTInfo.MemberInfo.TeamDieMember[count] = (WORD)((CBattleTeam_GTournament*)pTeam)->GetDieCount();
			++count;
		}
	EndToEachTeam

	memcpy( pInfo, &m_GTInfo, sizeof(BATTLE_INFO_GTOURNAMENT) );
	*size = sizeof(BATTLE_INFO_GTOURNAMENT);
}
void CBattle_GTournament::ReleaseAllObject()
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


#else

void CBattle_GTournament::Render()
{
	ySWITCH(m_BattleInfo.BattleState)
		yCASE(eBATTLE_STATE_READY)

		yCASE(eBATTLE_STATE_FIGHT)
//		if( gTickTime < m_RenderTime )
//		{
//			m_RenderTime -= gTickTime;
			if( m_pCurShowImage )
				m_pCurShowImage->RenderSprite( &m_vTitleScale, NULL, 0.0f, &m_vTitlePos, 0xffffffff );
//		}
//		else
//			m_RenderTime = 0;
		yCASE(eBATTLE_STATE_RESULT)
		if( gTickTime < m_RenderTime )
		{
			m_RenderTime -= gTickTime;
			if( m_pCurShowImage )
				m_pCurShowImage->RenderSprite( &m_vTitleScale, NULL, 0.0f, &m_vTitlePos, 0xffffffff );
		}
		else
			m_RenderTime = 0;

	yENDSWITCH
}
void CBattle_GTournament::CreateWaitPlace()
{
	// 대기장소 만들기 추가
	CTileManager* pTileManager = MAP->GetTileManager();

	for(int i=eBattleTeam1; i<eBattleTeam_Max; ++i)
	{
		VECTOR3 vPos;
		vPos.y = 0;
		vPos.z = 5833;
		if( i == eBattleTeam1 )		vPos.x = 2826;
		else						vPos.x = 17374;
		VECTOR3 vStgPos;

		float fx, fz = vPos.z;
		float fAdd = -50.0f;
		
		for( fx = vPos.x - 4200.0f ; fx <= vPos.x + 4200.0f + fAdd ; fx += 50.0f )//
		{
			vStgPos.x = fx;
			vStgPos.z = fz - 4200.0f;
			if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
				pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
			vStgPos.z = fz + 4200.0f + fAdd;
			if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
				pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		}
		
		fx = vPos.x;
		for( fz = vPos.z - 4200.0f ; fz <= vPos.z + 4200.0f + fAdd; fz += 50.0f )
		{
			vStgPos.x = fx - 4200.0f;
			vStgPos.z = fz;
			if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
				pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
			vStgPos.x = fx + 4200.0f + fAdd;
			vStgPos.z = fz;
			if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
				pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		}
		
		// 이거 안하면 빙글빙글 돈다.
		fz = vPos.z;
		for( fx = vPos.x - 4250.0f ; fx <= vPos.x + 4250.0f + fAdd ; fx += 50.0f )//
		{
			vStgPos.x = fx;
			vStgPos.z = fz - 4250.0f;
			if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
				pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
			vStgPos.z = fz + 4250.0f + fAdd;
			if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
				pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		}
		
		fx = vPos.x;
		for( fz = vPos.z - 4250.0f ; fz <= vPos.z + 4250.0f + fAdd ; fz += 50.0f )
		{
			vStgPos.x = fx - 4250.0f;
			vStgPos.z = fz;
			if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
				pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
			vStgPos.x = fx + 4250.0f + fAdd;
			vStgPos.z = fz;
			if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
				pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		}
	}

	VECTOR3 Pos1, Pos2;
	Pos1.x = 7000;		// 5604
	Pos1.y = 0;
	Pos1.z = 5833;
	Pos2.x = 13100;		// 14603
	Pos2.y = 0;
	Pos2.z = 5833;
	
	// Effect
	OBJECTEFFECTDESC desc( FindEffectNum("eff_mpato_side_s.beff"), EFFECT_FLAG_ABSOLUTEPOS, &Pos1 );
	HERO->AddObjectEffect( BATTLE_GTTEAM1_EFFECTID, &desc, 1 );
	OBJECTEFFECTDESC desc1( FindEffectNum("eff_mpato_side_s.beff"), EFFECT_FLAG_ABSOLUTEPOS, &Pos2 );
	HERO->AddObjectEffect( BATTLE_GTTEAM2_EFFECTID, &desc1, 1 );
}
void CBattle_GTournament::DestroyWaitPlace()
{
	// 대기장소 해제.
	CTileManager* pTileManager = MAP->GetTileManager();

	for(int i=eBattleTeam1; i<eBattleTeam_Max; ++i)
	{
		VECTOR3 vPos;
		vPos.y = 0;
		vPos.z = 5833;
		if( i == eBattleTeam1 )		vPos.x = 2826;
		else						vPos.x = 17374;
		VECTOR3 vStgPos;
		
		float fx, fz = vPos.z;
		float fAdd = -50.0f;
		
		for( fx = vPos.x - 4200.0f ; fx <= vPos.x + 4200.0f + fAdd ; fx += 50.0f )//
		{
			vStgPos.x = fx;
			vStgPos.z = fz - 4200.0f;
			if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
				pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
			vStgPos.z = fz + 4200.0f + fAdd;
			if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
				pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		}
		
		fx = vPos.x;
		for( fz = vPos.z - 4200.0f ; fz <= vPos.z + 4200.0f + fAdd; fz += 50.0f )
		{
			vStgPos.x = fx - 4200.0f;
			vStgPos.z = fz;
			if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
				pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
			vStgPos.x = fx + 4200.0f + fAdd;
			vStgPos.z = fz;
			if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
				pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		}
		
		//
		fz = vPos.z;
		for( fx = vPos.x - 4250.0f ; fx <= vPos.x + 4250.0f + fAdd ; fx += 50.0f )//
		{
			vStgPos.x = fx;
			vStgPos.z = fz - 4250.0f;
			if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
				pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
			vStgPos.z = fz + 4250.00f + fAdd;
			if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
				pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		}
		
		fx = vPos.x;
		for( fz = vPos.z - 4250.0f ; fz <= vPos.z + 4250.0f + fAdd; fz += 50.0f )
		{
			vStgPos.x = fx - 4250.0f;
			vStgPos.z = fz;
			if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
				pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
			vStgPos.x = fx + 4250.0f + fAdd;
			vStgPos.z = fz;
			if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
				pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		}
	}
	//
	HERO->RemoveObjectEffect( BATTLE_GTTEAM1_EFFECTID );
	HERO->RemoveObjectEffect( BATTLE_GTTEAM2_EFFECTID );
}

BOOL CBattle_GTournament::IsObserver( CObject* pObject )
{
	if( m_Observer.IsTeamMember( pObject ) )
		return TRUE;
	
	return FALSE;
}

#endif