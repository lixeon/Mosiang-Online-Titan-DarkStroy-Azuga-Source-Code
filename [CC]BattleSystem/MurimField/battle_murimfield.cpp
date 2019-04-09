#include "stdafx.h"
#include "Battle_MurimField.h"
#include "BattleTeam_MurimField.h"

#include "../BattleObject.h"
#include "Object.h"
#include "Player.h"

#ifdef _MAPSERVER_		// mapserver

#include "UserTable.h"
#include "ServerTable.h"
#include "CharMove.h"
#include "ObjectStateManager.h"
#include "PackedData.h"

#else					// client

#include "ChatManager.h"
#include "./interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"
#include "ObjectManager.h"
//#include "TileManager.h"
//#include "MHMap.h"
#endif

#define BATTLE_MURIMFIELD_READYTIME		20000
#define	BATTLE_MURIMFIELD_FIGHTTIME		100000
#define BATTLE_MURIMFIELD_RESULTTIME	30000

VECTOR3 team1Po,team2Po,team3Po;


CBattle_MurimField::CBattle_MurimField()
{
	memset( &m_MurimBattleInfo, 0, sizeof(MURIMFIELD_BATTLE_INFO) );

	m_BattleFlag = BATTLE_FLAG_LOGINBATTLE;

	// ÀÓ½Ã
	SetVector3(&team1Po,23000,0,25000);
	SetVector3(&team2Po,27000,0,25000);
	SetVector3(&team3Po,25000,0,25000);

#ifndef _MAPSERVER_
	m_pCurShowImage	= NULL;
#endif
}

CBattle_MurimField::~CBattle_MurimField()
{
}

// Àû,¾Æ±º ±¸º°
BOOL CBattle_MurimField::IsEnemy( CObject* pOperator, CObject* pTarget )
{
	if( m_BattleInfo.BattleState != eBATTLE_STATE_FIGHT )
		return FALSE;
		
	if( pOperator->GetBattleID() != pTarget->GetBattleID() )
		return FALSE;
	
	if( pOperator->GetBattleTeam() == pTarget->GetBattleTeam() )
		return FALSE;
	
	return TRUE;
}

BOOL CBattle_MurimField::IsFriend( CObject* pOperator, CObject* pTarget )
{
	if( m_BattleInfo.BattleState != eBATTLE_STATE_FIGHT )
		return FALSE;

	if( pOperator->GetBattleID() != pTarget->GetBattleID() )
		return FALSE;
	
	if( pOperator->GetBattleTeam() != pTarget->GetBattleTeam() )
		return FALSE;
	
	return TRUE;
}

// event func
void CBattle_MurimField::OnCreate( BATTLE_INFO_BASE* pCreateInfo, CBattleTeam* pTeam1, CBattleTeam* pTeam2 )
{
	CBattle::OnCreate( pCreateInfo, pTeam1, pTeam2 );

	memcpy( &m_MurimBattleInfo, pCreateInfo, sizeof(MURIMFIELD_BATTLE_INFO) );

#ifndef _MAPSERVER_	
	m_ImageNumber.Init( 32, 0 );
	m_ImageNumber.SetFillZero( TRUE );
	m_ImageNumber.SetLimitCipher( 3 );
	m_ImageNumber.SetPosition( 528 + 32, 70 );

	SCRIPTMGR->GetImage( 59, &m_ImageReady, PFT_HARDPATH );
	SCRIPTMGR->GetImage( 60, &m_ImageFight, PFT_HARDPATH );
	SCRIPTMGR->GetImage( 61, &m_ImageWin, PFT_HARDPATH );
	SCRIPTMGR->GetImage( 62, &m_ImageLose, PFT_HARDPATH );
	SCRIPTMGR->GetImage( 76, &m_ImageDraw, PFT_HARDPATH );
	
	m_pCurShowImage = &m_ImageReady;

	m_vImagePos.x = (1024 - 128) / 2;			//128 width
	m_vImagePos.y = 32;
	m_vImageScale.x = m_vImageScale.y = 1.0f;
#endif
}

void CBattle_MurimField::OnFightStart()
{
	CBattle::OnFightStart();

#ifndef _MAPSERVER_
	m_pCurShowImage = &m_ImageFight;
#endif
}

void CBattle_MurimField::OnDestroy()
{
}

void CBattle_MurimField::OnTeamMemberAdd( int Team, DWORD MemberID, char* Name )
{
#ifdef _MAPSERVER_
	CObject* pObject = g_pUserTable->FindUser(MemberID);
	ASSERT(pObject);
	if(pObject->GetObjectKind() == eObjectKind_Player)
	{
		CPlayer* pPlayer = (CPlayer*)pObject;

		if(Team == eBattleTeam1)
		{
			CCharMove::SetPosition(pPlayer,&team1Po);
		}
		else if( Team == eBattleTeam2 )
		{
			CCharMove::SetPosition(pPlayer,&team2Po);
		}
		else
		{
			CCharMove::SetPosition(pPlayer,&team3Po);
		}
		CCharMove::CorrectPlayerPosToServer(pPlayer);
	}	
#else
	CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(451), Name, Team );
#endif
	
	CBattleTeam_MurimField* pTeam = GetTeam(Team);
	MURIMFIELD_TEAMINFO* pTeamInfo = pTeam->GetTeamInfo();
	BATTLE_TEAMMEMBER_INFO* pMemberInfo = pTeamInfo->FindMember(MemberID);

	for(int n=0;n<MAX_MUNPAFIELD_MEMBERNUM;++n)
	{
		if(pTeamInfo->TeamMember[n].MemberID == MemberID)
		{
			pTeamInfo->TeamMember[n].set(MemberID,Name);
			break;
		}
	}
}

BOOL CBattle_MurimField::OnTeamMemberDelete(int Team,DWORD MemberID,char* Name)
{
#ifdef _MHCLIENT_
	CHATMGR->AddMsg( CTC_TOWHOLE, CHATMGR->GetChatMsg(452), Name, Team );
#endif
	//////////////////////////////////////////////////////////////////////////
	// ÀÓ½Ã Å×½ºÆ®¿ë
	CBattleTeam_MurimField* pTeam = GetTeam(Team);
	MURIMFIELD_TEAMINFO* pTeamInfo = pTeam->GetTeamInfo();
	BATTLE_TEAMMEMBER_INFO* pMemberInfo = pTeamInfo->FindMember(MemberID);
	ASSERT(pMemberInfo);
	pMemberInfo->MemberID = 0;
	//////////////////////////////////////////////////////////////////////////

	return TRUE;
}

BOOL CBattle_MurimField::OnTeamMemberDie(int Team,DWORD VictimMemberID,DWORD KillerID)
{
	ASSERT(Team < eBattleTeam_Max);
	
#ifdef _MAPSERVER_
	CObject* pVictimMember = g_pUserTable->FindUser(VictimMemberID);
	CObject* pKiller = g_pUserTable->FindUser(KillerID);
	if(pVictimMember == NULL || pKiller == NULL)
	{
		ASSERT(0);
		return FALSE;
	}
	
	ASSERT(pVictimMember->GetBattleID() == GetBattleID());
	if(pKiller->GetBattleID() != GetBattleID())
		return FALSE;

	OBJECTSTATEMGR_OBJ->StartObjectState( pVictimMember, eObjectState_Die, pKiller->GetID() );
	OBJECTSTATEMGR_OBJ->EndObjectState( pVictimMember, eObjectState_Die, PLAYERREVIVE_TIME );
	
	MSG_DWORD3 msg;
	SetProtocol(&msg,MP_BATTLE,MP_BATTLE_TEAMMEMBER_DIE_NOTIFY);
	msg.dwData1 = Team;
	msg.dwData2 = VictimMemberID;
	msg.dwData3 = KillerID;
	ToEachTeam(pTeam)
		pTeam->SendTeamMsg(&msg,sizeof(msg));
	EndToEachTeam
#endif	

	MURIMFIELD_TEAMINFO* pKillTeam = GetTeam(GetOtherTeamNum(Team))->GetTeamInfo();
	MURIMFIELD_TEAMINFO* pDiedTeam = GetTeam(Team)->GetTeamInfo();

	++pKillTeam->TotalKill;
	++pDiedTeam->TotalDied;

	BATTLE_TEAMMEMBER_INFO* pKillerInfo = pKillTeam->FindMember(KillerID);
	BATTLE_TEAMMEMBER_INFO* pVictimInfo = pDiedTeam->FindMember(VictimMemberID);

	ASSERT(pKillerInfo);
	ASSERT(pVictimInfo);

	++pKillerInfo->MemberKillNum;
	++pVictimInfo->MemberDieNum;


	return TRUE;
}

void CBattle_MurimField::OnTeamMemberRevive(int Team,CObject* pReviver)
{
#ifdef _MAPSERVER_
	VECTOR3 pos;
	if( Team == eBattleTeam1 )
		pos = team1Po;
	else if( Team == eBattleTeam2 )
		pos = team2Po;
	else
		pos = team3Po;

	MOVE_POS msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTER_REVIVE;
	msg.dwObjectID = pReviver->GetID();
	msg.dwMoverID = pReviver->GetID();
	msg.cpos.Compress(&pos);
	
	CCharMove::SetPosition( pReviver, &pos );

	DWORD MaxLife = pReviver->GetMaxLife();
	DWORD MaxNaeryuk = pReviver->GetMaxNaeRyuk();
	pReviver->SetLife((DWORD)(MaxLife));
	pReviver->SetNaeRyuk((DWORD)(MaxNaeryuk));
	
	ToEachTeam(pTeam)
		pTeam->SendTeamMsg(&msg,sizeof(msg));
	EndToEachTeam
#endif
}

void CBattle_MurimField::OnTeamObjectCreate(int Team,CBattleObject* pBattleObject)
{
}

void CBattle_MurimField::OnTeamObjectDestroy(int Team,CBattleObject* pBattleObject)
{
}

void CBattle_MurimField::OnTick()
{
	ySWITCH(m_BattleInfo.BattleState)
		yCASE(eBATTLE_STATE_READY)
			DWORD ElapTime = gCurTime - m_BattleInfo.BattleTime;
			if( ElapTime > BATTLE_MURIMFIELD_READYTIME )
			{
				for( int n = 0; n < eBattleTeam_Max; ++n )
				{
					if( m_TeamArray[n]->GetAliveTeamMemberNum() == 0 )
						return;
				}

				StartBattle();
			}

		yCASE(eBATTLE_STATE_FIGHT)
			Judge();

		yCASE(eBATTLE_STATE_RESULT)
			Result();
	
	yENDSWITCH
}

BOOL CBattle_MurimField::Judge()
{
	switch( m_MurimBattleInfo.wMurimBattleKind )
	{
	case eMurimBattle_MaxKill:
		Judge_MaxKill();
		break;
	}	

	// ¸ðµç °ÔÀÓÀº ±âº»ÀûÀ¸·Î ½Ã°£ Á¦ÇÑ ÀÖÀ½.
	Judge_TimeLimit();

	return FALSE;
}

void CBattle_MurimField::Victory( int WinnerTeamNum, int LoserTeamNum )
{
	CBattle::Victory( WinnerTeamNum, LoserTeamNum );

#ifdef _MAPSERVER_	
	MSG_DWORD2 msg;
	msg.Category = MP_MURIMNET;
	msg.Protocol = MP_MURIMNET_NOTIFYTOMN_GAMEEND;
	msg.dwData1 = GetBattleID();
	msg.dwData2 = WinnerTeamNum;

	g_pServerTable->SetPositionHead();
	SERVERINFO* pAgentInfo = g_pServerTable->GetNextAgentServer();
	if(pAgentInfo == NULL)
	{
		ASSERT(0);
		return;
	}

	PACKEDDATA_OBJ->SendToMapServer( pAgentInfo->dwConnectionIndex, 99, &msg, sizeof(msg) );
#else
	if( HERO->GetBattleTeam() == WinnerTeamNum )
		m_pCurShowImage = &m_ImageWin;
	else if( HERO->GetBattleTeam() == LoserTeamNum )
		m_pCurShowImage = &m_ImageLose;
	else
		m_pCurShowImage = NULL;

	m_vImageScale.x = m_vImageScale.y = 3.0f;
	m_vImagePos.x = (1024 - 128 * m_vImageScale.x ) / 2;	//128 = width
	m_vImagePos.y = ( 768 - 42 * m_vImageScale.y ) / 2;		//42 = height

	//AutoAttack¸ØÃã
//	HERO->DisableAutoAttack();					//°ø°ÝÁß ÀÌ´ø°Å Ãë¼ÒµÇ³ª?
//	HERO->SetNextAction(NULL);			//½ºÅ³ ¾²´ø°Íµµ Ãë¼ÒµÇ³ª?
#endif
}

void CBattle_MurimField::Draw()
{
	CBattle::Draw();

#ifdef _MAPSERVER_
	MSG_DWORD2 msg;
	msg.Category = MP_MURIMNET;
	msg.Protocol = MP_MURIMNET_NOTIFYTOMN_GAMEEND;
	msg.dwData1 = GetBattleID();
	msg.dwData2 = 2;
	
	g_pServerTable->SetPositionHead();
	SERVERINFO* pAgentInfo = g_pServerTable->GetNextAgentServer();
	if(pAgentInfo == NULL)
	{
		ASSERT(0);
		return;
	}

	PACKEDDATA_OBJ->SendToMapServer( pAgentInfo->dwConnectionIndex, 99, &msg, sizeof(msg) );
#else
	m_pCurShowImage = &m_ImageDraw;

	m_vImageScale.x = m_vImageScale.y = 3.0f;
	m_vImagePos.x = (1024 - 128 * m_vImageScale.x ) / 2;	//128 = width
	m_vImagePos.y = ( 768 - 42 * m_vImageScale.y ) / 2;		//42 = height
	
	//AutoAttack¸ØÃã
//	HERO->DisableAutoAttack();					//°ø°ÝÁß ÀÌ´ø°Å Ãë¼ÒµÇ³ª?
//	HERO->SetNextAction(NULL);			//½ºÅ³ ¾²´ø°Íµµ Ãë¼ÒµÇ³ª?
#endif	
}

void CBattle_MurimField::Result()
{
	switch( m_MurimBattleInfo.wMurimBattleReward )
	{
	case 0:
		break;

	case 1:
		Result_Money();
		break;
	}

	// °á°ú º¸´Â ½Ã°£ Ã¼Å©
	DWORD ElapTime = gCurTime - m_BattleInfo.BattleTime;
	if( ElapTime > BATTLE_MURIMFIELD_RESULTTIME )
	{
		MSGBASE msg;
		msg.Category = MP_MURIMNET;
		msg.Protocol = MP_MURIMNET_RETURNTOMURIMNET_ACK;

		ToEachTeam( pTeam )
			pTeam->SendTeamMsg( &msg, sizeof(msg) );
		EndToEachTeam

		SetDestroyFlag();
	}
}

BOOL CBattle_MurimField::Judge_MaxKill()
{
	CBattleTeam_MurimField* pTheTeam = GetTeam(eBattleTeam1);
	CBattleTeam_MurimField* pOtherTeam = GetTeam(eBattleTeam2);
	MURIMFIELD_TEAMINFO* pTheTeamInfo = pTheTeam->GetTeamInfo();
	MURIMFIELD_TEAMINFO* pOtherTeamInfo = pOtherTeam->GetTeamInfo();

//	if( pTheTeamInfo->TotalKill >= m_MurimBattleInfo.dwValue )
	if( pTheTeamInfo->TotalKill >= 10 )
	{
		Victory( eBattleTeam1, eBattleTeam2 );
		return TRUE;
	}
//	if( pOtherTeamInfo->TotalKill >= m_MurimBattleInfo.dwValue )
	if( pOtherTeamInfo->TotalKill >= 10 )
	{
		Victory( eBattleTeam2, eBattleTeam1 );
		return TRUE;
	}	

	return FALSE;
}

BOOL CBattle_MurimField::Judge_TimeLimit()
{
	CBattleTeam_MurimField* pTheTeam = GetTeam(eBattleTeam1);
	CBattleTeam_MurimField* pOtherTeam = GetTeam(eBattleTeam2);
	MURIMFIELD_TEAMINFO* pTheTeamInfo = pTheTeam->GetTeamInfo();
	MURIMFIELD_TEAMINFO* pOtherTeamInfo = pOtherTeam->GetTeamInfo();
	
	DWORD ElapsedTime = gCurTime - m_BattleInfo.BattleTime;
	if( ElapsedTime > BATTLE_MURIMFIELD_FIGHTTIME )
	{
		if( pTheTeamInfo->TotalKill > pOtherTeamInfo->TotalKill )
			Victory( eBattleTeam1, eBattleTeam2 );
		else if( pTheTeamInfo->TotalKill < pOtherTeamInfo->TotalKill )
			Victory( eBattleTeam2, eBattleTeam1 );
		else
			Draw();
	}

	return FALSE;
}

BOOL CBattle_MurimField::Result_Money()
{
	// µ·ÀÌ °É·ÁÀÖ´Â °æ¿ì Ã³¸®ÇÑ´Ù.
	
	return TRUE;
}

#ifdef _MAPSERVER_	// Battle Á¤º¸ °ü·Ã
void CBattle_MurimField::GetBattleInfo( char* pInfo, WORD* size )
{
	MSG_BATTLESETTING_SYN_MURIMFIELD* Info = (MSG_BATTLESETTING_SYN_MURIMFIELD*)pInfo;
	memcpy( Info, &m_BattleInfo, sizeof(BATTLE_INFO_BASE) );

	ToEachTeam( pTeam )
		CBattleTeam_MurimField* pTeamMF = (CBattleTeam_MurimField*)pTeam;
		pTeamMF->GetTeamInfo( &Info->TeamInfo[TeamNumber] );
	EndToEachTeam

	*size = sizeof(MSG_BATTLESETTING_SYN_MURIMFIELD);
}
#endif


#ifdef _MHCLIENT_
#include "interface/cFont.h"
// ÀÎÅÍÆäÀÌ½º´Â ±×¸² ¹× ±âÈ¹ÀÌ ³ª¿À´Â´ë·ç....
void CBattle_MurimField::Render()
{
	// °á°ú ÀÎÅÍÆäÀÌ½º ±¸¼º. ( ÀÓ½Ã )
	if( m_pCurShowImage )
		m_pCurShowImage->RenderSprite( &m_vImageScale, NULL, 0.0f, &m_vImagePos, 0xffffffff );

	DWORD ElapsedTime = gCurTime-m_BattleInfo.BattleTime;
	switch( m_BattleInfo.BattleState )
	{
	case eBATTLE_STATE_READY:
		{
			DWORD RemainTime = BATTLE_MURIMFIELD_READYTIME > ElapsedTime ? BATTLE_MURIMFIELD_READYTIME - ElapsedTime : 0;
			m_ImageNumber.SetNumber( RemainTime / 1000 );
			m_ImageNumber.Render();
		}
		break;

	case eBATTLE_STATE_FIGHT:
		{
			DWORD RemainTime = BATTLE_MURIMFIELD_FIGHTTIME > ElapsedTime ? BATTLE_MURIMFIELD_FIGHTTIME - ElapsedTime : 0;
			m_ImageNumber.SetNumber( RemainTime / 1000 );
			m_ImageNumber.Render();
		}
		break;

	case eBATTLE_STATE_RESULT:
		{
		}
		break;
	}
	
	MURIMFIELD_TEAMINFO *pOurTeam, *pOtherTeam/*, *pObserver */;

	if( HERO->GetBattleTeam() == eBattleTeam1 )
	{
		pOurTeam = GetTeam(eBattleTeam1)->GetTeamInfo();
		pOtherTeam = GetTeam(eBattleTeam2)->GetTeamInfo();
//		pObserver = GetTeam(eBattleTeam_Observer)->GetTeamInfo();
	}
	else if( HERO->GetBattleTeam() == eBattleTeam2 )
	{
		pOurTeam = GetTeam(eBattleTeam2)->GetTeamInfo();
		pOtherTeam = GetTeam(eBattleTeam1)->GetTeamInfo();
//		pObserver = GetTeam(eBattleTeam_Observer)->GetTeamInfo();
	}
	else
	{
		pOurTeam = GetTeam(eBattleTeam1)->GetTeamInfo();
		pOtherTeam = GetTeam(eBattleTeam2)->GetTeamInfo();
//		pObserver = GetTeam(eBattleTeam_Observer)->GetTeamInfo();
	}

	RECT rect;
	char temp[256];
	int pos = 0;
	int n = 0;

	// Total Score
	sprintf( temp, "Total Score is Our Team( %d ) : Other Team( %d )", pOurTeam->TotalKill, pOtherTeam->TotalKill );
	SetRect( &rect, 150, 10, 250, 25 );
	CFONT_OBJ->RenderFont( 0, temp, strlen(temp), &rect, 0xffffffff );

	// Our Team
	sprintf( temp, "- Our Team Member -" );
	SetRect( &rect, 10, 100, 110, 115 );
	CFONT_OBJ->RenderFont( 0, temp, strlen(temp), &rect, 0xff00ffff );	
	for( n = 0; n < MAX_MURIMFIELD_MEMBERNUM; ++n )
	{
		BATTLE_TEAMMEMBER_INFO* pMemberInfo = &pOurTeam->TeamMember[n];
		if( pMemberInfo->MemberID )
		{
			sprintf( temp, "%s (%d-%d)", pMemberInfo->MemberName, pMemberInfo->MemberKillNum, pMemberInfo->MemberDieNum );
			SetRect( &rect, 10, pos*15+115, 110, pos*15+130 );
			CFONT_OBJ->RenderFont( 0, temp, strlen(temp), &rect, 0xff00ffff );
			pos++;
		}
	}
	// Other Team
	sprintf( temp, "- Other Team Member -" );
	SetRect( &rect, 130, 100, 240, 115 );
	CFONT_OBJ->RenderFont( 0, temp, strlen(temp), &rect, 0xffff00ff );
	pos = 0;
	for( n = 0; n < MAX_MURIMFIELD_MEMBERNUM; ++n )
	{
		BATTLE_TEAMMEMBER_INFO* pMemberInfo = &pOtherTeam->TeamMember[n];
		if( pMemberInfo->MemberID )
		{
			sprintf( temp, "%s (%d-%d)", pMemberInfo->MemberName, pMemberInfo->MemberKillNum, pMemberInfo->MemberDieNum );
			SetRect( &rect, 130, pos*15+115, 240, pos*15+130 );	
			CFONT_OBJ->RenderFont( 0, temp, strlen(temp), &rect, 0xffff00ff );
			pos++;
		}
	}
	// Observer
/*	sprintf( temp, "- Observers -" );
	SetRect( &rect, 10, 400, 110, 415 );
	CFONT_OBJ->RenderFont( 0, temp, strlen(temp), &rect, 0xffffffff );
	pos = 0;
	for( n = 0; n < MAX_MURIMFIELD_MEMBERNUM; ++n )
	{
		BATTLE_TEAMMEMBER_INFO* pMemberInfo = &pObserver->TeamMember[n];
		if( pMemberInfo->MemberID )
		{
			sprintf( temp, "Observer %s", pMemberInfo->MemberName );
			SetRect( &rect, 10, pos*15+415, 110, pos*15+430 );	
			CFONT_OBJ->RenderFont( 0, temp, strlen(temp), &rect, 0xffffffff );
			pos++;
		}
	} */
}

#endif