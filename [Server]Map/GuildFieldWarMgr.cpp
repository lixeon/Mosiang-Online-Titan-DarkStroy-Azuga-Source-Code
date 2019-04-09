// GuildFieldWarMgr.cpp: implementation of the CGuildFieldWarMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildFieldWarMgr.h"
#include "UserTable.h"
#include "Player.h"
#include "Guild.h"
#include "GuildManager.h"
#include "Network.h"
#include "LootingManager.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "MapDBMsgParser.h"

extern int g_nServerSetNum;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGuildFieldWar::CGuildFieldWar()
{	
	m_dwGuildIdx = 0;
	m_EnemyGuildTable.Initialize( 20 );
	m_dwVictoryCount = m_dwDrawCount = m_dwLooseCount = 0;
}

CGuildFieldWar::~CGuildFieldWar()
{
	Release();
}

void CGuildFieldWar::Release()
{
	sGFWENEMY* pData = NULL;
	m_EnemyGuildTable.SetPositionHead();
	while( pData = m_EnemyGuildTable.GetData() )
		delete pData;
	m_EnemyGuildTable.RemoveAll();
}

void CGuildFieldWar::RegistGuildFieldWar( DWORD dwEnemyGuildIdx, DWORD dwMoney )
{
	if( m_EnemyGuildTable.GetData( dwEnemyGuildIdx ) )
		return;

	sGFWENEMY* pData = new sGFWENEMY;
	pData->dwEnemyGuildIdx = dwEnemyGuildIdx;
	pData->dwGFWMoney = dwMoney;
	m_EnemyGuildTable.Add( pData, dwEnemyGuildIdx );
}

void CGuildFieldWar::UnRegistGuildFieldWar( DWORD dwEnemyGuildIdx )
{
	sGFWENEMY* pData = m_EnemyGuildTable.GetData( dwEnemyGuildIdx );
	if( !pData )	return;

	delete pData;
	m_EnemyGuildTable.Remove( dwEnemyGuildIdx );
}

BOOL CGuildFieldWar::IsEnemy( DWORD dwEnemyGuildIdx )
{
	if( m_EnemyGuildTable.GetData( dwEnemyGuildIdx ) )
		return TRUE;
	return FALSE;
}

void CGuildFieldWar::AddPlayer( CPlayer* pPlayer )
{
	MSG_GUILDFIELDWAR_INFO Msg;
	Msg.Category = MP_GUILD_FIELDWAR;
	Msg.Protocol = MP_GUILD_FIELDWAR_PROC;

	sGFWENEMY* pData = NULL;
	m_EnemyGuildTable.SetPositionHead();
	while( pData = m_EnemyGuildTable.GetData() )
	{
		CGuild* pGuild = GUILDMGR->GetGuild( pData->dwEnemyGuildIdx );
		if( pGuild )
		{
			Msg.AddEnemyGuildInfo( pGuild->GetGuildInfo(), pData->dwGFWMoney );
		}
	}

	if( Msg.dwEnemyGuildNum )
		pPlayer->SendMsg( (MSGBASE*)&Msg, Msg.GetMsgLength() );	

	MSG_DWORD3 Record;
	Record.Category = MP_GUILD_FIELDWAR;
	Record.Protocol = MP_GUILD_FIELDWAR_RECORD;
	Record.dwData1 = m_dwVictoryCount;
	Record.dwData2 = m_dwDrawCount;
	Record.dwData3 = m_dwLooseCount;
	pPlayer->SendMsg( (MSGBASE*)&Record, sizeof(Record) );
}

BOOL CGuildFieldWar::IsGuildFieldWar()
{
	if( m_EnemyGuildTable.GetDataNum() > 0 )
		return TRUE;
	return FALSE;
}

void CGuildFieldWar::UpdateGuildFieldWarRecord()
{
	UpdateGuildFieldWarRecordToDB( m_dwGuildIdx, m_dwVictoryCount, m_dwDrawCount, m_dwLooseCount );
}

DWORD CGuildFieldWar::GetGuildFieldWarMoney( DWORD dwEnemyGuildIdx )
{
	sGFWENEMY* pData = m_EnemyGuildTable.GetData( dwEnemyGuildIdx );
	if( !pData )	return 0;
	return pData->dwGFWMoney;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGFWCheck::CGFWCheck()
{
	m_dwGuildIdx = 0;
	m_GFWDenyTable.Initialize( 50 );
	m_GFWEndTable.Initialize( 20 );
}

CGFWCheck::~CGFWCheck()
{
	Release();
}

void CGFWCheck::Release()
{
	sGFWTIME* pData = NULL;
	m_GFWDenyTable.SetPositionHead();
	while( pData = m_GFWDenyTable.GetData() )
		delete pData;
	m_GFWDenyTable.RemoveAll();

	pData = NULL;
	m_GFWEndTable.SetPositionHead();
	while( pData = m_GFWEndTable.GetData() )
		delete pData;
	m_GFWEndTable.RemoveAll();
}

int CGFWCheck::CanGuildFieldWar( DWORD dwEnemyGuildIdx )
{
	stTIME CurTime;
	CurTime.value = GetCurTime();
	sGFWTIME* pDenyTime = m_GFWDenyTable.GetData( dwEnemyGuildIdx );
	sGFWTIME* pEndTime = m_GFWEndTable.GetData( dwEnemyGuildIdx );

	if( pDenyTime && pEndTime == NULL )
	{
		if( CurTime > pDenyTime->Time )
		{
			pDenyTime->Time = CurTime;
			pDenyTime->dwCount = 0;
			return 0;
		}
		else if( pDenyTime->dwCount > 2 )
			return 4;
		else
			return 0;		
	}
	else if( pDenyTime == NULL && pEndTime )
	{
		if( CurTime > pEndTime->Time )	return 0;
		else							return 5;
	}
	else if( pDenyTime && pEndTime )
	{
		if( CurTime > pEndTime->Time )	return 0;
		else							return 5;

		if( CurTime > pDenyTime->Time )
		{
			pDenyTime->Time = CurTime;
			pDenyTime->dwCount = 0;
			return 0;
		}
		else if( pDenyTime->dwCount > 2 )
			return 4;
		else
			return 0;		
	}

	return 0;	// can
}

void CGFWCheck::RegistDenyTime( DWORD dwEnemyGuildIdx )
{
	sGFWTIME* pData = m_GFWDenyTable.GetData( dwEnemyGuildIdx );
	if( !pData )
	{
		pData = new sGFWTIME;
		pData->dwEnemyGuildIdx = dwEnemyGuildIdx;
		m_GFWDenyTable.Add( pData, dwEnemyGuildIdx );

		stTIME time;
		time.SetTime( 0, 0, 1, 0, 0, 0 );
		pData->Time.value = GetCurTime();
		pData->Time += time;

		pData->dwCount = 1;
	}
	else
	{
		++pData->dwCount;		
	}
}

void CGFWCheck::RegistEndTime( DWORD dwEnemyGuildIdx )
{
	sGFWTIME* pData = m_GFWEndTable.GetData( dwEnemyGuildIdx );
	if( !pData )
	{
		pData = new sGFWTIME;
		pData->dwEnemyGuildIdx = dwEnemyGuildIdx;
		m_GFWEndTable.Add( pData, dwEnemyGuildIdx );
	}

	stTIME time;
	time.SetTime( 0, 0, 1, 0, 0, 0 );
	pData->Time.value = GetCurTime();
	pData->Time += time;

	sGFWTIME* pDeny = m_GFWDenyTable.GetData( dwEnemyGuildIdx );
	if( pDeny )
	{
		pDeny->Time.SetTime( 0, 0, 0, 0, 0, 0 );
		pDeny->dwCount = 0;
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuildFieldWarMgr::CGuildFieldWarMgr()
{
	m_GuildFieldWarTable.Initialize( 100 );
	m_GFWCheckTable.Initialize( 100 );
}

CGuildFieldWarMgr::~CGuildFieldWarMgr()
{
	Release();
}

void CGuildFieldWarMgr::Init()
{
//	LoadGuildFieldWar( 0 );
}

void CGuildFieldWarMgr::Release()
{
	CGuildFieldWar* pData = NULL;
	m_GuildFieldWarTable.SetPositionHead();
	while( pData = m_GuildFieldWarTable.GetData() )
		delete pData;
	m_GuildFieldWarTable.RemoveAll();

	CGFWCheck* pCheck = NULL;
	m_GFWCheckTable.SetPositionHead();
	while( pCheck = m_GFWCheckTable.GetData() )
		delete pCheck;
	m_GFWCheckTable.RemoveAll();
}

void CGuildFieldWarMgr::RegistGuildFieldWarFromRecord( DWORD dwGuildIdx, DWORD dwVictory, DWORD dwDraw, DWORD dwLoose )
{
	CGuildFieldWar* pGuildFieldWar = m_GuildFieldWarTable.GetData( dwGuildIdx );
	if( pGuildFieldWar == NULL )
	{
		pGuildFieldWar = new CGuildFieldWar;
		pGuildFieldWar->Init( dwGuildIdx );
		m_GuildFieldWarTable.Add( pGuildFieldWar, dwGuildIdx );
	}
	pGuildFieldWar->SetGuildFieldWarRecord( dwVictory, dwDraw, dwLoose );
}

void CGuildFieldWarMgr::UpdateGuildFieldWarRecord( BOOL bDB, WORD wFlag, DWORD dwGuildIdx )
{
	CGuildFieldWar* pGuildFieldWar = m_GuildFieldWarTable.GetData( dwGuildIdx );
	if( pGuildFieldWar == NULL )	return;
	switch( wFlag )
	{
	case 0:		pGuildFieldWar->LooseGuildFieldWar();		break;	// loose
	case 1:		pGuildFieldWar->VictoryGuildFieldWar();		break;	// victory
	case 2:		pGuildFieldWar->DrawGuildFieldWar();		break;	// draw	
	}

	if( bDB )
		pGuildFieldWar->UpdateGuildFieldWarRecord();
}

BOOL CGuildFieldWarMgr::RegistGuildFieldWar( DWORD dwGuildIdx1, DWORD dwGuildIdx2, DWORD dwMoney )
{
	CGuildFieldWar* pGuildFieldWar1 = m_GuildFieldWarTable.GetData( dwGuildIdx1 );
	if( pGuildFieldWar1 == NULL )
	{
		pGuildFieldWar1 = new CGuildFieldWar;
		pGuildFieldWar1->Init( dwGuildIdx1 );
		m_GuildFieldWarTable.Add( pGuildFieldWar1, dwGuildIdx1 );
	}
	pGuildFieldWar1->RegistGuildFieldWar( dwGuildIdx2, dwMoney );

	CGuildFieldWar* pGuildFieldWar2 = m_GuildFieldWarTable.GetData( dwGuildIdx2 );
	if( pGuildFieldWar2 == NULL )
	{
		pGuildFieldWar2 = new CGuildFieldWar;
		pGuildFieldWar2->Init( dwGuildIdx2 );
		m_GuildFieldWarTable.Add( pGuildFieldWar2, dwGuildIdx2 );
	}
	pGuildFieldWar2->RegistGuildFieldWar( dwGuildIdx1, dwMoney );

	return TRUE;
}

BOOL CGuildFieldWarMgr::UnRegistGuildFieldWar( DWORD dwGuildIdx1, DWORD dwGuildIdx2 )
{
	CGuildFieldWar* pGuildFieldWar1 = m_GuildFieldWarTable.GetData( dwGuildIdx1 );
	if( pGuildFieldWar1 )
		pGuildFieldWar1->UnRegistGuildFieldWar( dwGuildIdx2 );

	CGuildFieldWar* pGuildFieldWar2 = m_GuildFieldWarTable.GetData( dwGuildIdx2 );
	if( pGuildFieldWar2 )
		pGuildFieldWar2->UnRegistGuildFieldWar( dwGuildIdx1 );

	return TRUE;
}

BOOL CGuildFieldWarMgr::JudgeGuildFieldWar( CPlayer* pPlayer, CPlayer* pAttacker )
{
	// pPlayer is Die...
	if( pAttacker->GetObjectKind() != eObjectKind_Player )	return FALSE;
	if( IsEnemy( pPlayer, pAttacker ) == FALSE )	return FALSE;

	DWORD dwRankInGuild = pPlayer->GetGuildMemberRank();
	if( dwRankInGuild == GUILD_MASTER )
	{
		DWORD dwGuildIdxWinner = pAttacker->GetGuildIdx();
		DWORD dwGuildIdxLoser = pPlayer->GetGuildIdx();

		GuildFieldWarEnd( dwGuildIdxWinner, dwGuildIdxLoser );

//		if( g_pServerSystem->GetNation() == eNATION_CHINA || g_nServerSetNum != 4 )
//			LOOTINGMGR->AutoLooting( pPlayer, pAttacker );		
	}
	else
	{
//		if( g_pServerSystem->GetNation() == eNATION_CHINA || g_nServerSetNum != 4 )
//			LOOTINGMGR->AutoLooting( pPlayer, pAttacker );
	}

	pPlayer->SetDieForGFW( TRUE );
	
	return TRUE;
}

BOOL CGuildFieldWarMgr::IsEnemy( CPlayer* pOper, CPlayer* pTarget )
{
	DWORD dwGuild1 = pOper->GetGuildIdx();
	DWORD dwGuild2 = pTarget->GetGuildIdx();
	if( dwGuild1 == 0 || dwGuild2 == 0 )	return FALSE;
	
	CGuildFieldWar* pGuildFieldWar = m_GuildFieldWarTable.GetData( dwGuild1 );
	if( pGuildFieldWar == NULL )	return FALSE;
	if( !pGuildFieldWar->IsEnemy( dwGuild2 ) )	return FALSE;

	return TRUE;
}

BOOL CGuildFieldWarMgr::IsEnemyInField( CPlayer* pOper, CPlayer* pTarget )
{
	if( g_pServerSystem->GetMap()->IsVillage() )	return FALSE;

	DWORD dwGuild1 = pOper->GetGuildIdx();
	DWORD dwGuild2 = pTarget->GetGuildIdx();
	if( dwGuild1 == 0 || dwGuild2 == 0 )	return FALSE;
	
	CGuildFieldWar* pGuildFieldWar = m_GuildFieldWarTable.GetData( dwGuild1 );
	if( pGuildFieldWar == NULL )	return FALSE;
	if( !pGuildFieldWar->IsEnemy( dwGuild2 ) )	return FALSE;

	return TRUE;
}

void CGuildFieldWarMgr::AddPlayer( CPlayer* pPlayer )
{
	DWORD dwGuildIdx = pPlayer->GetGuildIdx();
	if( dwGuildIdx == 0 )	return;

	// sendmsg
	CGuildFieldWar* pGuildFieldWar = m_GuildFieldWarTable.GetData( dwGuildIdx );
	if( pGuildFieldWar )
		pGuildFieldWar->AddPlayer( pPlayer );
}

BOOL CGuildFieldWarMgr::IsGuildFieldWar( DWORD dwGuildIdx1, DWORD dwGuildIdx2 )
{
	CGuildFieldWar* pGuildFieldWar1 = m_GuildFieldWarTable.GetData( dwGuildIdx1 );
	CGuildFieldWar* pGuildFieldWar2 = m_GuildFieldWarTable.GetData( dwGuildIdx2 );
	if( pGuildFieldWar1 == NULL || pGuildFieldWar2 == NULL )	return FALSE;
	if( pGuildFieldWar1->IsEnemy( dwGuildIdx2 ) && pGuildFieldWar2->IsEnemy( dwGuildIdx1 ) )
		return TRUE;
	return FALSE;
}

BOOL CGuildFieldWarMgr::IsGuildFieldWar( DWORD dwGuildIdx )
{
	CGuildFieldWar* pGuildFieldWar = m_GuildFieldWarTable.GetData( dwGuildIdx );
	if( pGuildFieldWar == NULL )	return FALSE;
	
	return pGuildFieldWar->IsGuildFieldWar();
}

int CGuildFieldWarMgr::CanGuildFieldWar( DWORD dwGuildIdx, DWORD dwEnemyGuildIdx )
{
	CGFWCheck* pData = m_GFWCheckTable.GetData( dwGuildIdx );
	if( !pData )	return 0;	// can

	return pData->CanGuildFieldWar( dwEnemyGuildIdx );
}

void CGuildFieldWarMgr::RegistDeny( DWORD dwGuildIdx, DWORD dwEnemyGuildIdx )
{
	CGFWCheck* pData = m_GFWCheckTable.GetData( dwGuildIdx );
	if( !pData )
	{
		pData = new CGFWCheck;
		pData->Init( dwGuildIdx );
		m_GFWCheckTable.Add( pData, dwGuildIdx );
	}
	pData->RegistDenyTime( dwEnemyGuildIdx );
}

void CGuildFieldWarMgr::RegistEnd( DWORD dwGuildIdx1, DWORD dwGuildIdx2 )
{
	CGFWCheck* pData1 = m_GFWCheckTable.GetData( dwGuildIdx1 );
	if( !pData1 )
	{
		pData1 = new CGFWCheck;
		pData1->Init( dwGuildIdx1 );
		m_GFWCheckTable.Add( pData1, dwGuildIdx1 );
	}
	pData1->RegistEndTime( dwGuildIdx2 );

	CGFWCheck* pData2 = m_GFWCheckTable.GetData( dwGuildIdx2 );
	if( !pData2 )
	{
		pData2 = new CGFWCheck;
		pData2->Init( dwGuildIdx2 );
		m_GFWCheckTable.Add( pData2, dwGuildIdx2 );
	}
	pData2->RegistEndTime( dwGuildIdx1 );
}

DWORD CGuildFieldWarMgr::GetGuildFieldWarMoney( DWORD dwGuildIdx, DWORD dwEnemyGuildIdx )
{
	CGuildFieldWar* pGFW = m_GuildFieldWarTable.GetData( dwGuildIdx );
	if( !pGFW )	return 0;

	return pGFW->GetGuildFieldWarMoney( dwEnemyGuildIdx );
}

void CGuildFieldWarMgr::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch( Protocol )
	{
	case MP_GUILD_FIELDWAR_NACK:				GuildFieldWarNack( pMsg );				break;
	case MP_GUILD_FIELDWAR_DECLARE:				GuildFieldWarDeclare( pMsg );			break;
	case MP_GUILD_FIELDWAR_DECLARE_NACK:												break;
	case MP_GUILD_FIELDWAR_DECLARE_ACCEPT:		GuildFieldWarDeclareAccept( pMsg );		break;
	case MP_GUILD_FIELDWAR_DECLARE_DENY:		GuildFieldWarDeclareDeny( pMsg );		break;
	case MP_GUILD_FIELDWAR_DECLARE_DENY_NOTIFY_TOMAP:	GuildFieldWarDeclareDenyNotifyToMap( pMsg );	break;

	case MP_GUILD_FIELDWAR_START:														break;
	case MP_GUILD_FIELDWAR_START_NOTIFY_TOMAP:	GuildFieldWarStartNotifyToMap( pMsg );	break;
	case MP_GUILD_FIELDWAR_PROC:														break;
	case MP_GUILD_FIELDWAR_END:															break;
	case MP_GUILD_FIELDWAR_END_NOTIFY_TOMAP:	GuildFieldWarEndNotifyToMap( pMsg );	break;

	case MP_GUILD_FIELDWAR_SUGGESTEND:			GuildFieldWarSuggestEnd( pMsg );		break;
	case MP_GUILD_FIELDWAR_SUGGESTEND_NOTIFY_TOMAP:	GuildFieldWarSuggestEndNotifyToMap( pMsg );	break;
	case MP_GUILD_FIELDWAR_SUGGESTEND_NACK:												break;
	case MP_GUILD_FIELDWAR_SUGGESTEND_ACCEPT:	GuildFieldWarSuggestEndAccept( pMsg );	break;
	case MP_GUILD_FIELDWAR_SUGGESTEND_ACCEPT_NOTIFY_TOMAP:	GuildFieldWarSuggestEndAcceptNotifyToMap( pMsg );	break;
	case MP_GUILD_FIELDWAR_SUGGESTEND_DENY:		GuildFieldWarSuggestEndDeny( pMsg );	break;
	case MP_GUILD_FIELDWAR_SUGGESTEND_DENY_NOTIFY_TOMAP:	GuildFieldWarSuggestEndDenyNotifyToMap( pMsg );	break;	

	case MP_GUILD_FIELDWAR_SURRENDER:			GuildFieldWarSurrender( pMsg );			break;
	case MP_GUILD_FIELDWAR_SURRENDER_NACK:												break;
	case MP_GUILD_FIELDWAR_SURRENDER_NOTIFY_TOMAP:	GuildFieldWarSurrenderNotifyToMap( pMsg );	break;
	case MP_GUILD_FIELDWAR_ADDMONEY_TOMAP:		GuildFieldWarAddMoneyToMap( pMsg );		break;
	}
}

void CGuildFieldWarMgr::GuildFieldWarNack( void* pMsg )
{
	MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pPlayer )	return;

	pPlayer->SendMsg( pmsg, sizeof(MSG_BYTE) );
}

void CGuildFieldWarMgr::GuildFieldWarDeclare( void* pMsg )
{
	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

	CGuild* pGuild1 = GUILDMGR->GetGuildFromMasterIdx( pmsg->dwData1 );
	CGuild* pGuild2 = GUILDMGR->GetGuildFromMasterIdx( pmsg->dwData2 );
	if( pGuild1 == NULL || pGuild2 == NULL )	return;

	if( pGuild1->GetLevel() < 3 )	
	{
		SendDeclareNackMsg( pmsg->dwData1, 0 );
		return;
	}
	if( pGuild2->GetLevel() < 3 )
	{
		SendDeclareNackMsg( pmsg->dwData1, 1 );
		return;
	}
	if( pGuild1->GetMemberNum() < 5 )
	{
		SendDeclareNackMsg( pmsg->dwData1, 2 );
		return;
	}
	if( pGuild2->GetMemberNum() < 5 )
	{
		SendDeclareNackMsg( pmsg->dwData1, 3 );
		return;
	}
	int rt = CanGuildFieldWar( pGuild2->GetIdx(), pGuild1->GetIdx() );
	if( rt != 0 )
	{
		SendDeclareNackMsg( pmsg->dwData1, rt );
		return;
	}	
	if( IsGuildFieldWar( pGuild1->GetGuildInfo()->GuildIdx, pGuild2->GetGuildInfo()->GuildIdx ) )
		return;
	//if( g_pServerSystem->GetMapNum() == RUNNINGMAP )
	if( g_pServerSystem->GetMap()->IsMapSame(eRunningMap) )
	{
		SendDeclareNackMsg( pmsg->dwData1, 6 );
		return;
	}
	if( g_pServerSystem->GetMapNum() == PKEVENTMAP )
	{
		SendDeclareNackMsg( pmsg->dwData1, 6 );
		return;
	}
	//if( g_pServerSystem->CheckMapKindIs(eSurvival) )
	if( g_pServerSystem->GetMap()->IsMapKind(eSurvivalMap) )
	{
		SendDeclareNackMsg( pmsg->dwData1, 7 );
		return;
	}
	
	CPlayer* pSender = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData1 );
	CPlayer* pReceiver = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData2 );
	DWORD dwMoney = pmsg->dwData3;
	if( pSender && pReceiver )
	{
		if( dwMoney )
		if( dwMoney > pSender->GetMoney() )
		{
			SendDeclareNackMsg( pmsg->dwData1, 98 );
			return;
		}
		
		MSG_BYTE Wait;
		Wait.Category = MP_GUILD_FIELDWAR;
		Wait.Protocol = MP_GUILD_FIELDWAR_WAIT;
		Wait.bData = MP_GUILD_FIELDWAR_DECLARE;		
		pSender->SendMsg( (MSGBASE*)&Wait, sizeof(Wait) );

		MSG_GUILDFIELDWAR_INFO Declare;
		Declare.Category = MP_GUILD_FIELDWAR;
		Declare.Protocol = MP_GUILD_FIELDWAR_DECLARE;
		Declare.AddEnemyGuildInfo( pGuild1->GetGuildInfo(), pmsg->dwData3 );
		pReceiver->SendMsg( (MSGBASE*)&Declare, sizeof(Declare) );
	}
	else if( pSender && !pReceiver )
	{
		if( dwMoney )
		if( dwMoney > pSender->GetMoney() )
		{
			SendDeclareNackMsg( pmsg->dwData1, 98 );
			return;
		}

		MSG_BYTE Wait;
		Wait.Category = MP_GUILD_FIELDWAR;
		Wait.Protocol = MP_GUILD_FIELDWAR_WAIT;
		Wait.bData = MP_GUILD_FIELDWAR_DECLARE;		
		pSender->SendMsg( (MSGBASE*)&Wait, sizeof(Wait) );
	}
	else if( !pSender && pReceiver )
	{
		MSG_GUILDFIELDWAR_INFO Declare;
		Declare.Category = MP_GUILD_FIELDWAR;
		Declare.Protocol = MP_GUILD_FIELDWAR_DECLARE;
		Declare.AddEnemyGuildInfo( pGuild1->GetGuildInfo(), pmsg->dwData3 );
		pReceiver->SendMsg( (MSGBASE*)&Declare, sizeof(Declare) );
	}
	else if( !pSender && !pReceiver )
	{
		return;
	}
}

void CGuildFieldWarMgr::GuildFieldWarDeclareAccept( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( pPlayer1 == NULL )	return;	
	DWORD dwGuildIdx1 = pPlayer1->GetGuildIdx();
	DWORD dwGuildIdx2 = pmsg->dwData1;				// enemy guildidx
	if( dwGuildIdx1 == 0 || dwGuildIdx2 == 0 )	return;
	if( pPlayer1->GetGuildMemberRank() != GUILD_MASTER )	return;
	if( IsGuildFieldWar( dwGuildIdx1, dwGuildIdx2 ) == TRUE )	return;

	CGuild* pGuild1 = GUILDMGR->GetGuild( dwGuildIdx1 );
	CGuild* pGuild2 = GUILDMGR->GetGuild( dwGuildIdx2 );
	if( pGuild1 == NULL || pGuild2 == NULL )	return;

	// send money msg
	CPlayer* pPlayer2 = (CPlayer*)g_pUserTable->FindUser( pGuild2->GetMasterIdx() );
	SendMoneyMsg( pPlayer1, pPlayer2, pmsg->dwData2, MP_GUILD_FIELDWAR_REMOVEMONEY );

	// send guild1
	MSG_GUILDFIELDWAR_INFO Msg1;
	Msg1.Category = MP_GUILD_FIELDWAR;
	Msg1.Protocol = MP_GUILD_FIELDWAR_START;
	Msg1.AddEnemyGuildInfo( pGuild2->GetGuildInfo(), pmsg->dwData2 );	// money
	pGuild1->SendMsgToAll( (MSGBASE*)&Msg1, sizeof(Msg1) );

	// send guild2
	MSG_GUILDFIELDWAR_INFO Msg2;
	Msg2.Category = MP_GUILD_FIELDWAR;
	Msg2.Protocol = MP_GUILD_FIELDWAR_START;
	Msg2.AddEnemyGuildInfo( pGuild1->GetGuildInfo(), pmsg->dwData2 );	// money
	pGuild2->SendMsgToAll( (MSGBASE*)&Msg2, sizeof(Msg2) );

	// send to othermap
	MSG_DWORD3 Notify;
	Notify.Category = MP_GUILD_FIELDWAR;
	Notify.Protocol = MP_GUILD_FIELDWAR_START_NOTIFY_TOMAP;
	Notify.dwData1 = dwGuildIdx1;
	Notify.dwData2 = dwGuildIdx2;
	Notify.dwData3 = pmsg->dwData2;			// money
	g_Network.Send2AgentServer( (char*)&Notify, sizeof(Notify) );	

	// db insert
	InsertGuildFieldWar( dwGuildIdx1, dwGuildIdx2, pmsg->dwData2 );
	// regist
	RegistGuildFieldWar( dwGuildIdx1, dwGuildIdx2, pmsg->dwData2 );
}

void CGuildFieldWarMgr::GuildFieldWarDeclareDeny( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( pPlayer == NULL )	return;	
	DWORD dwGuildIdx1 = pPlayer->GetGuildIdx();
	DWORD dwGuildIdx2 = pmsg->dwData1;
	if( dwGuildIdx1 == 0 || dwGuildIdx2 == 0 )	return;
	if( pPlayer->GetGuildMemberRank() != GUILD_MASTER )	return;

	CGuild* pGuild = GUILDMGR->GetGuild( dwGuildIdx2 );
	if( pGuild == NULL )	return;

	pPlayer = (CPlayer*)g_pUserTable->FindUser( pGuild->GetMasterIdx() );
	if( pPlayer )
	{
		MSG_DWORD Msg;
		Msg.Category = MP_GUILD_FIELDWAR;
		Msg.Protocol = MP_GUILD_FIELDWAR_DECLARE_DENY;
		Msg.dwData = dwGuildIdx1;

		pPlayer->SendMsg( (MSGBASE*)&Msg, sizeof(Msg) );
	}
	else
	{
		MSG_DWORD2 Notify;
		Notify.Category = MP_GUILD_FIELDWAR;
		Notify.Protocol = MP_GUILD_FIELDWAR_DECLARE_DENY_NOTIFY_TOMAP;
		Notify.dwData1 = pGuild->GetMasterIdx();
		Notify.dwData2 = dwGuildIdx1;
	
		g_Network.Send2AgentServer( (char*)&Notify, sizeof(Notify) );	
	}

	// regist deny
	RegistDeny( dwGuildIdx1, dwGuildIdx2 );
}

void CGuildFieldWarMgr::GuildFieldWarDeclareDenyNotifyToMap( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData1 );
	if( pPlayer == NULL )	return;	

	MSG_DWORD Msg;
	Msg.Category = MP_GUILD_FIELDWAR;
	Msg.Protocol = MP_GUILD_FIELDWAR_DECLARE_DENY;
	Msg.dwData = pmsg->dwData2;

	pPlayer->SendMsg( (MSGBASE*)&Msg, sizeof(Msg) );

	// regist deny
	RegistDeny( pmsg->dwData2, pPlayer->GetGuildIdx() );
}

void CGuildFieldWarMgr::GuildFieldWarStartNotifyToMap( void* pMsg )
{
	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
	
	DWORD dwGuildIdx1 = pmsg->dwData1;
	DWORD dwGuildIdx2 = pmsg->dwData2;
	if( dwGuildIdx1 == 0 || dwGuildIdx2 == 0 )	return;
	if( IsGuildFieldWar( dwGuildIdx1, dwGuildIdx2 ) == TRUE )	return;

	CGuild* pGuild1 = GUILDMGR->GetGuild( dwGuildIdx1 );
	CGuild* pGuild2 = GUILDMGR->GetGuild( dwGuildIdx2 );
	if( pGuild1 == NULL || pGuild2 == NULL )	return;

	// send money msg
	CPlayer* pPlayer2 = (CPlayer*)g_pUserTable->FindUser( pGuild2->GetMasterIdx() );
	if( pPlayer2 )
		SendMoneyMsg( NULL, pPlayer2, pmsg->dwData3, MP_GUILD_FIELDWAR_REMOVEMONEY );

	// send guild1
	MSG_GUILDFIELDWAR_INFO Msg1;
	Msg1.Category = MP_GUILD_FIELDWAR;
	Msg1.Protocol = MP_GUILD_FIELDWAR_START;
	Msg1.AddEnemyGuildInfo( pGuild2->GetGuildInfo(), pmsg->dwData3 );		// money
	pGuild1->SendMsgToAll( (MSGBASE*)&Msg1, sizeof(Msg1) );

	// send guild2
	MSG_GUILDFIELDWAR_INFO Msg2;
	Msg2.Category = MP_GUILD_FIELDWAR;
	Msg2.Protocol = MP_GUILD_FIELDWAR_START;
	Msg2.AddEnemyGuildInfo( pGuild1->GetGuildInfo(), pmsg->dwData3 );		// money
	pGuild2->SendMsgToAll( (MSGBASE*)&Msg2, sizeof(Msg2) );

	// regist
	RegistGuildFieldWar( dwGuildIdx1, dwGuildIdx2, pmsg->dwData3 );
}

void CGuildFieldWarMgr::GuildFieldWarEnd( DWORD dwGuildIdxWinner, DWORD dwGuildIdxLoser )
{
	CGuild* pGuildWinner = GUILDMGR->GetGuild( dwGuildIdxWinner );
	CGuild* pGuildLoser = GUILDMGR->GetGuild( dwGuildIdxLoser );

	if( pGuildWinner == NULL || pGuildLoser == NULL )	return;

	// money
	DWORD dwMoney = GetGuildFieldWarMoney( dwGuildIdxWinner, dwGuildIdxLoser );
	if( dwMoney )
	{
		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pGuildWinner->GetMasterIdx() );
		if( pPlayer )
			SendMoneyMsg( pPlayer, NULL, dwMoney*2, MP_GUILD_FIELDWAR_ADDMONEY );
		else
			SendToAgentAddMoneyMsg( pGuildWinner->GetMasterIdx(), dwMoney*2 );
	}		

	MSG_DWORD2 Msg;
	Msg.Category = MP_GUILD_FIELDWAR;
	Msg.Protocol = MP_GUILD_FIELDWAR_END;

	// send guildwinner
	Msg.dwData1 = 1;	// 0 = lose, 1 = win
	Msg.dwData2 = dwGuildIdxLoser; 
	pGuildWinner->SendMsgToAll( (MSGBASE*)&Msg, sizeof(Msg) );

	// send guildloser
	Msg.dwData1 = 0;
	Msg.dwData2 = dwGuildIdxWinner; 
	pGuildLoser->SendMsgToAll( (MSGBASE*)&Msg, sizeof(Msg) );

	// send to othermap
	MSG_DWORD2 Notify;
	Notify.Category = MP_GUILD_FIELDWAR;
	Notify.Protocol = MP_GUILD_FIELDWAR_END_NOTIFY_TOMAP;
	Notify.dwData1 = dwGuildIdxWinner;		// winner
	Notify.dwData2 = dwGuildIdxLoser;		// loser
	g_Network.Send2AgentServer( (char*)&Notify, sizeof(Notify) );

	// db delete
	DeleteGuildFieldWar( dwGuildIdxWinner, dwGuildIdxLoser );
	// unregist
	UnRegistGuildFieldWar( dwGuildIdxWinner, dwGuildIdxLoser );
//	PenaltyForGuildFieldWarEnd( 0, dwGuildIdxLoser );

	// guildfieldwarrecord
	UpdateGuildFieldWarRecord( TRUE, 0, dwGuildIdxLoser );
	UpdateGuildFieldWarRecord( TRUE, 1, dwGuildIdxWinner );

	// registend
	RegistEnd( dwGuildIdxWinner, dwGuildIdxLoser );	

	SendToAllUser( 0, pGuildWinner->GetGuildName(), pGuildLoser->GetGuildName() );
}

void CGuildFieldWarMgr::GuildFieldWarEndNotifyToMap( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	
	DWORD dwGuildIdxWinner = pmsg->dwData1;
	DWORD dwGuildIdxLoser = pmsg->dwData2;
	if( dwGuildIdxWinner == 0 || dwGuildIdxLoser == 0 )	return;

	CGuild* pGuildWinner = GUILDMGR->GetGuild( dwGuildIdxWinner );
	CGuild* pGuildLoser = GUILDMGR->GetGuild( dwGuildIdxLoser );

	if( pGuildWinner == NULL || pGuildLoser == NULL )	return;

	MSG_DWORD2 Msg;
	Msg.Category = MP_GUILD_FIELDWAR;
	Msg.Protocol = MP_GUILD_FIELDWAR_END;

	// send guildwinner
	Msg.dwData1 = 1;	// 0 = lose, 1 = win
	Msg.dwData2 = dwGuildIdxLoser; 
	pGuildWinner->SendMsgToAll( (MSGBASE*)&Msg, sizeof(Msg) );

	// send guildloser
	Msg.dwData1 = 0;
	Msg.dwData2 = dwGuildIdxWinner; 
	pGuildLoser->SendMsgToAll( (MSGBASE*)&Msg, sizeof(Msg) );

	// unregist
	UnRegistGuildFieldWar( dwGuildIdxWinner, dwGuildIdxLoser );
//	PenaltyForGuildFieldWarEndNotifyToMap( 0, dwGuildIdxLoser );

	// guildfieldwarrecord
	UpdateGuildFieldWarRecord( FALSE, 0, dwGuildIdxLoser );
	UpdateGuildFieldWarRecord( FALSE, 1, dwGuildIdxWinner );

	// registend
	RegistEnd( dwGuildIdxWinner, dwGuildIdxLoser );
}

void CGuildFieldWarMgr::GuildFieldWarSuggestEnd( void* pMsg )
{
	MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
	
	CGuild* pGuild1 = GUILDMGR->GetGuildFromMasterIdx( pmsg->dwData1 );
	CGuild* pGuild2 = GUILDMGR->GetGuildFromMasterIdx( pmsg->dwData2 );
	if( pGuild1 == NULL || pGuild2 == NULL )	return;
	if( IsGuildFieldWar( pGuild1->GetGuildInfo()->GuildIdx, pGuild2->GetGuildInfo()->GuildIdx ) == FALSE )
		return;

	CPlayer* pSender = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData1 );
	CPlayer* pReceiver = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData2 );
	if( pSender && pReceiver )
	{
		MSG_BYTE Wait;
		Wait.Category = MP_GUILD_FIELDWAR;
		Wait.Protocol = MP_GUILD_FIELDWAR_WAIT;
		Wait.bData = MP_GUILD_FIELDWAR_SUGGESTEND;		
		pSender->SendMsg( (MSGBASE*)&Wait, sizeof(Wait) );

		MSG_DWORD Msg;
		Msg.Category = MP_GUILD_FIELDWAR;
		Msg.Protocol = MP_GUILD_FIELDWAR_SUGGESTEND;
		Msg.dwData = pGuild1->GetGuildInfo()->GuildIdx;
		pReceiver->SendMsg( (MSGBASE*)&Msg, sizeof(Msg) );
	}
	else if( !pSender && pReceiver )
	{
		MSG_DWORD Msg;
		Msg.Category = MP_GUILD_FIELDWAR;
		Msg.Protocol = MP_GUILD_FIELDWAR_SUGGESTEND;
		Msg.dwData = pGuild1->GetGuildInfo()->GuildIdx;
		pReceiver->SendMsg( (MSGBASE*)&Msg, sizeof(Msg) );
	}
	else if( pSender && !pReceiver )
	{
		MSG_BYTE Wait;
		Wait.Category = MP_GUILD_FIELDWAR;
		Wait.Protocol = MP_GUILD_FIELDWAR_WAIT;
		Wait.bData = MP_GUILD_FIELDWAR_SUGGESTEND;		
		pSender->SendMsg( (MSGBASE*)&Wait, sizeof(Wait) );
	}
}

void CGuildFieldWarMgr::GuildFieldWarSuggestEndNotifyToMap( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData1 );
	if( pPlayer == NULL )	return;	

	MSG_DWORD Msg;
	Msg.Category = MP_GUILD_FIELDWAR;
	Msg.Protocol = MP_GUILD_FIELDWAR_SUGGESTEND;
	Msg.dwData = pmsg->dwData2;

	pPlayer->SendMsg( (MSGBASE*)&Msg, sizeof(Msg) );
}

void CGuildFieldWarMgr::GuildFieldWarSuggestEndAccept( void* pMsg )
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( pPlayer == NULL )	return;	
	DWORD dwGuildIdx1 = pPlayer->GetGuildIdx();
	DWORD dwGuildIdx2 = pmsg->dwData;
	if( dwGuildIdx1 == 0 || dwGuildIdx2 == 0 )	return;
	if( pPlayer->GetGuildMemberRank() != GUILD_MASTER )	return;
	if( IsGuildFieldWar( dwGuildIdx1, dwGuildIdx2 ) == FALSE )	return;

	CGuild* pGuild1 = GUILDMGR->GetGuild( dwGuildIdx1 );
	CGuild* pGuild2 = GUILDMGR->GetGuild( dwGuildIdx2 );
	if( pGuild1 == NULL || pGuild2 == NULL )	return;

	// money
	DWORD dwMoney = GetGuildFieldWarMoney( dwGuildIdx1, dwGuildIdx2 );
	if( dwMoney )
	{
		SendMoneyMsg( pPlayer, NULL, dwMoney, MP_GUILD_FIELDWAR_ADDMONEY );

		CPlayer* pPlayer2 = (CPlayer*)g_pUserTable->FindUser( pGuild2->GetMasterIdx() );
		if( pPlayer2 )
			SendMoneyMsg( pPlayer2, NULL, dwMoney, MP_GUILD_FIELDWAR_ADDMONEY );
		else
			SendToAgentAddMoneyMsg( pGuild2->GetMasterIdx(), dwMoney );
	}	

	MSG_DWORD2 Msg;
	Msg.Category = MP_GUILD_FIELDWAR;
	Msg.Protocol = MP_GUILD_FIELDWAR_END;
	Msg.dwData1 = 2;		// 2 = draw

	// send guild1
	Msg.dwData2 = dwGuildIdx2;
	pGuild1->SendMsgToAll( (MSGBASE*)&Msg, sizeof(Msg) );

	// send guild2
	Msg.dwData2 = dwGuildIdx1;
	pGuild2->SendMsgToAll( (MSGBASE*)&Msg, sizeof(Msg) );

	// send to othermap
	MSG_DWORD2 Notify;
	Notify.Category = MP_GUILD_FIELDWAR;
	Notify.Protocol = MP_GUILD_FIELDWAR_SUGGESTEND_ACCEPT_NOTIFY_TOMAP;
	Notify.dwData1 = dwGuildIdx1;
	Notify.dwData2 = dwGuildIdx2;

	g_Network.Send2AgentServer( (char*)&Notify, sizeof(Notify) );

	// db delete
	DeleteGuildFieldWar( dwGuildIdx1, dwGuildIdx2 );
	// unregist
	UnRegistGuildFieldWar( dwGuildIdx1, dwGuildIdx2 );

	// guildfieldwarrecord
	UpdateGuildFieldWarRecord( TRUE, 2, dwGuildIdx1 );
	UpdateGuildFieldWarRecord( TRUE, 2, dwGuildIdx2 );

	// registend
	RegistEnd( dwGuildIdx1, dwGuildIdx2 );

	SendToAllUser( 1, pGuild1->GetGuildName(), pGuild2->GetGuildName() );
}

void CGuildFieldWarMgr::GuildFieldWarSuggestEndAcceptNotifyToMap( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	DWORD dwGuildIdx1 = pmsg->dwData1;
	DWORD dwGuildIdx2 = pmsg->dwData2;
	if( dwGuildIdx1 == 0 || dwGuildIdx2 == 0 )	return;

	CGuild* pGuild1 = GUILDMGR->GetGuild( dwGuildIdx1 );
	CGuild* pGuild2 = GUILDMGR->GetGuild( dwGuildIdx2 );

	if( pGuild1 == NULL || pGuild2 == NULL )	return;

	MSG_DWORD2 Msg;
	Msg.Category = MP_GUILD_FIELDWAR;
	Msg.Protocol = MP_GUILD_FIELDWAR_END;
	Msg.dwData1 = 2;		// 2 = draw

	// send guild1
	Msg.dwData2 = dwGuildIdx2;
	pGuild1->SendMsgToAll( (MSGBASE*)&Msg, sizeof(Msg) );

	// send guild2
	Msg.dwData2 = dwGuildIdx1;
	pGuild2->SendMsgToAll( (MSGBASE*)&Msg, sizeof(Msg) );	

	// unregist
	UnRegistGuildFieldWar( dwGuildIdx1, dwGuildIdx2 );

	// guildfieldwarrecord
	UpdateGuildFieldWarRecord( FALSE, 2, dwGuildIdx1 );
	UpdateGuildFieldWarRecord( FALSE, 2, dwGuildIdx2 );

	// registend
	RegistEnd( dwGuildIdx1, dwGuildIdx2 );
}

void CGuildFieldWarMgr::GuildFieldWarSuggestEndDeny( void* pMsg )
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( pPlayer == NULL )	return;	
	DWORD dwGuildIdx1 = pPlayer->GetGuildIdx();
	DWORD dwGuildIdx2 = pmsg->dwData;
	if( dwGuildIdx1 == 0 || dwGuildIdx2 == 0 )	return;
	if( pPlayer->GetGuildMemberRank() != GUILD_MASTER )	return;

	CGuild* pGuild = GUILDMGR->GetGuild( dwGuildIdx2 );
	if( pGuild == NULL )	return;

	pPlayer = (CPlayer*)g_pUserTable->FindUser( pGuild->GetMasterIdx() );
	if( pPlayer )
	{
		MSG_DWORD Msg;
		Msg.Category = MP_GUILD_FIELDWAR;
		Msg.Protocol = MP_GUILD_FIELDWAR_SUGGESTEND_DENY;
		Msg.dwData = dwGuildIdx1;

		pPlayer->SendMsg( (MSGBASE*)&Msg, sizeof(Msg) );
	}
	else
	{
		MSG_DWORD2 Notify;
		Notify.Category = MP_GUILD_FIELDWAR;
		Notify.Protocol = MP_GUILD_FIELDWAR_SUGGESTEND_DENY_NOTIFY_TOMAP;
		Notify.dwData1 = pGuild->GetMasterIdx();
		Notify.dwData2 = dwGuildIdx1;

		g_Network.Send2AgentServer( (char*)&Notify, sizeof(Notify) );
	}
}

void CGuildFieldWarMgr::GuildFieldWarSuggestEndDenyNotifyToMap( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData1 );
	if( pPlayer == NULL )	return;	

	MSG_DWORD Msg;
	Msg.Category = MP_GUILD_FIELDWAR;
	Msg.Protocol = MP_GUILD_FIELDWAR_SUGGESTEND_DENY;
	Msg.dwData = pmsg->dwData2;

	pPlayer->SendMsg( (MSGBASE*)&Msg, sizeof(Msg) );
}

void CGuildFieldWarMgr::GuildFieldWarSurrender( void* pMsg )
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( pPlayer == NULL )	return;	
	DWORD dwGuildIdxSurrender = pPlayer->GetGuildIdx();
	DWORD dwGuildIdxWinner = pmsg->dwData;
	if( dwGuildIdxSurrender == 0 || dwGuildIdxWinner == 0 )	return;
	if( pPlayer->GetGuildMemberRank() != GUILD_MASTER )	return;
	if( IsGuildFieldWar( dwGuildIdxSurrender, dwGuildIdxWinner ) == FALSE )	return;

	CGuild* pGuildSurrender = GUILDMGR->GetGuild( dwGuildIdxSurrender );
	CGuild* pGuildWinner = GUILDMGR->GetGuild( dwGuildIdxWinner );
	if( pGuildSurrender == NULL || pGuildWinner == NULL )	return;

	// money
	DWORD dwMoney = GetGuildFieldWarMoney( dwGuildIdxSurrender, dwGuildIdxWinner );
	if( dwMoney )
	{
		CPlayer* pPlayer2 = (CPlayer*)g_pUserTable->FindUser( pGuildWinner->GetMasterIdx() );
		if( pPlayer2 )
			SendMoneyMsg( pPlayer2, NULL, dwMoney*2, MP_GUILD_FIELDWAR_ADDMONEY );
		else
			SendToAgentAddMoneyMsg( pGuildWinner->GetMasterIdx(), dwMoney*2 );
	}	

	MSG_DWORD2 Msg;
	Msg.Category = MP_GUILD_FIELDWAR;
	Msg.Protocol = MP_GUILD_FIELDWAR_SURRENDER;

	// send guild surrender
	Msg.dwData1 = 0;	// 0 = surrender, 1 = win
	Msg.dwData2 = dwGuildIdxWinner;
	pGuildSurrender->SendMsgToAll( (MSGBASE*)&Msg, sizeof(Msg) );

	// send guildwinner
	Msg.dwData1 = 1;
	Msg.dwData2 = dwGuildIdxSurrender;
	pGuildWinner->SendMsgToAll( (MSGBASE*)&Msg, sizeof(Msg) );

	// send to othermap
	MSG_DWORD2 Notify;
	Notify.Category = MP_GUILD_FIELDWAR;
	Notify.Protocol = MP_GUILD_FIELDWAR_SURRENDER_NOTIFY_TOMAP;
	Notify.dwData1 = dwGuildIdxWinner;		// winner
	Notify.dwData2 = dwGuildIdxSurrender;	// surrender
	g_Network.Send2AgentServer( (char*)&Notify, sizeof(Notify) );

	// db delete
	DeleteGuildFieldWar( dwGuildIdxWinner, dwGuildIdxSurrender );
	// unregist
	UnRegistGuildFieldWar( dwGuildIdxSurrender, dwGuildIdxWinner );
//	PenaltyForGuildFieldWarEnd( 1, dwGuildIdxSurrender );

	// guildfieldwarrecord
	UpdateGuildFieldWarRecord( TRUE, 0, dwGuildIdxSurrender );
	UpdateGuildFieldWarRecord( TRUE, 1, dwGuildIdxWinner );

	// registend
	RegistEnd( dwGuildIdxWinner, dwGuildIdxSurrender );

	SendToAllUser( 2, pGuildWinner->GetGuildName(), pGuildSurrender->GetGuildName() );
}

void CGuildFieldWarMgr::GuildFieldWarSurrenderNotifyToMap( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	
	DWORD dwGuildIdxWinner = pmsg->dwData1;
	DWORD dwGuildIdxSurrender = pmsg->dwData2;
	if( dwGuildIdxWinner == 0 || dwGuildIdxSurrender == 0 )	return;

	CGuild* pGuildWinner = GUILDMGR->GetGuild( dwGuildIdxWinner );
	CGuild* pGuildSurrender = GUILDMGR->GetGuild( dwGuildIdxSurrender );

	if( pGuildWinner == NULL || pGuildSurrender == NULL )	return;

	MSG_DWORD2 Msg;
	Msg.Category = MP_GUILD_FIELDWAR;
	Msg.Protocol = MP_GUILD_FIELDWAR_SURRENDER;

	// send guild surrender
	Msg.dwData1 = 0;	// 0 = surrender, 1 = win
	Msg.dwData2 = dwGuildIdxWinner;
	pGuildSurrender->SendMsgToAll( (MSGBASE*)&Msg, sizeof(Msg) );

	// send guildwinner
	Msg.dwData1 = 1;
	Msg.dwData2 = dwGuildIdxSurrender;
	pGuildWinner->SendMsgToAll( (MSGBASE*)&Msg, sizeof(Msg) );

	// unregist
	UnRegistGuildFieldWar( dwGuildIdxWinner, dwGuildIdxSurrender );
//	PenaltyForGuildFieldWarEndNotifyToMap( 1, dwGuildIdxSurrender );

	UpdateGuildFieldWarRecord( FALSE, 0, dwGuildIdxSurrender );
	UpdateGuildFieldWarRecord( FALSE, 1, dwGuildIdxWinner );

	// registend
	RegistEnd( dwGuildIdxWinner, dwGuildIdxSurrender );
}

void CGuildFieldWarMgr::GuildFieldWarAddMoneyToMap( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData1 );
	if( !pPlayer )	return;

	SendMoneyMsg( pPlayer, NULL, pmsg->dwData2, MP_GUILD_FIELDWAR_ADDMONEY );
}

void CGuildFieldWarMgr::PenaltyForGuildFieldWarEnd( DWORD dwKind, DWORD dwGuildIdxLoser )
{
	CGuild* pGuildLoser = GUILDMGR->GetGuild( dwGuildIdxLoser );
	if( pGuildLoser == NULL )	return;

	CPlayer* pPlayer = NULL;
	switch( dwKind )
	{
	case 0:		// fight
		{
// 임시로 막음...---------------------------------------------------------------------------
/*			pPlayer = (CPlayer*)g_pUserTable->FindUser( pGuildLoser->GetMasterIdx() );
			if( pPlayer )
			{
				EXPTYPE pointForLevel = GAMERESRCMNGR->GetMaxExpPoint( pPlayer->GetLevel() );
				EXPTYPE minusExp = (EXPTYPE)(pointForLevel * 0.1);
				pPlayer->ReduceExpPoint( minusExp, eExpLog_LosebyGuildFieldWar );
			}
			*/
//-------------------------------------------------------------------------------------------
			// level down
/*			pGuildLoser->LevelDown();

			// dbupdate
			GuildLevelUp( pGuildLoser->GetGuildInfo()->GuildIdx, pGuildLoser->GetLevel() );

			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_FIELDWAR;
			Msg.Protocol = MP_GUILD_FIELDWAR_LEVELDOWN;
			Msg.bData = pGuildLoser->GetLevel();
			pGuildLoser->SendMsgToAll( (MSGBASE*)&Msg, sizeof(Msg) );
			*/
		}
		break;
	case 1:		// surrend
		{
// 임시로 막음...---------------------------------------------------------------------------
/*			pPlayer = (CPlayer*)g_pUserTable->FindUser( pGuildLoser->GetMasterIdx() );
			if( pPlayer )
			{
				EXPTYPE pointForLevel = GAMERESRCMNGR->GetMaxExpPoint( pPlayer->GetLevel() );
				EXPTYPE minusExp = (EXPTYPE)(pointForLevel * 0.05);
				pPlayer->ReduceExpPoint( minusExp, eExpLog_LosebyGuildFieldWar );
			}
			*/
//-------------------------------------------------------------------------------------------
		}
		break;
	}
}

void CGuildFieldWarMgr::PenaltyForGuildFieldWarEndNotifyToMap( DWORD dwKind, DWORD dwGuildIdxLoser )
{
	CGuild* pGuildLoser = GUILDMGR->GetGuild( dwGuildIdxLoser );
	if( pGuildLoser == NULL )	return;

	switch( dwKind )
	{
	case 0:		// fight
		{
			// level down
/*			pGuildLoser->LevelDown();

			MSG_BYTE Msg;
			Msg.Category = MP_GUILD_FIELDWAR;
			Msg.Protocol = MP_GUILD_FIELDWAR_LEVELDOWN;
			Msg.bData = pGuildLoser->GetLevel();
			pGuildLoser->SendMsgToAll( (MSGBASE*)&Msg, sizeof(Msg) );
			*/
		}
		break;
	case 1:		// surrend
		{
		}
		break;
	}
}

void CGuildFieldWarMgr::SendDeclareNackMsg( DWORD dwReceiver, DWORD dwState )
{
	MSG_DWORD2 Msg;
	Msg.Category = MP_GUILD_FIELDWAR;
	Msg.Protocol = MP_GUILD_FIELDWAR_DECLARE_NACK;
	Msg.dwData1 = dwReceiver;
	Msg.dwData2 = dwState;

	g_Network.Broadcast2AgentServer( (char*)&Msg, sizeof(Msg) );
}

void CGuildFieldWarMgr::SendMoneyMsg( CPlayer* pPlayer1, CPlayer* pPlayer2, DWORD dwMoney, BYTE Protocol )
{
	if( !dwMoney )	return;

	MSG_DWORD Msg;
	Msg.Category = MP_GUILD_FIELDWAR;
	Msg.Protocol = Protocol;
	Msg.dwData = dwMoney;
	if( pPlayer1 )
	{
		if( Protocol == MP_GUILD_FIELDWAR_ADDMONEY )
			pPlayer1->SetMoney( dwMoney, MONEY_ADDITION );
		else if( Protocol == MP_GUILD_FIELDWAR_REMOVEMONEY )
			pPlayer1->SetMoney( dwMoney, MONEY_SUBTRACTION );
		pPlayer1->SendMsg( (MSGBASE*)&Msg, sizeof(Msg) );
	}
	if( pPlayer2 )
	{
		if( Protocol == MP_GUILD_FIELDWAR_ADDMONEY )
			pPlayer2->SetMoney( dwMoney, MONEY_ADDITION );
		else if( Protocol == MP_GUILD_FIELDWAR_REMOVEMONEY )
			pPlayer2->SetMoney( dwMoney, MONEY_SUBTRACTION );
		pPlayer2->SendMsg( (MSGBASE*)&Msg, sizeof(Msg) );
	}
}

void CGuildFieldWarMgr::SendToAgentAddMoneyMsg( DWORD dwPlayerIdx, DWORD dwMoney )
{
	MSG_DWORD2 Msg;
	Msg.Category = MP_GUILD_FIELDWAR;
	Msg.Protocol = MP_GUILD_FIELDWAR_ADDMONEY_TOMAP;
	Msg.dwData1 = dwPlayerIdx;
	Msg.dwData2 = dwMoney;

	g_Network.Send2AgentServer( (char*)&Msg, sizeof(Msg) );
}







/*
void CGuildFieldWarMgr::GuildFieldWarDeclare( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	
//	CPlayer* pSender = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData1 );
	CPlayer* pReceiver = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData2 );
	if( pReceiver == NULL )	return;

	CGuild* pGuild1 = GUILDMGR->GetGuildFromMasterIdx( pmsg->dwData1 );
	CGuild* pGuild2 = GUILDMGR->GetGuildFromMasterIdx( pmsg->dwData2 );
	if( pGuild1 == NULL || pGuild2 == NULL )	return;
	if( pGuild1->GetLevel() < 3 )	
	{
		SendDeclareNackMsg( pmsg->dwData1, 0 );
		return;
	}
	if( pGuild2->GetLevel() < 3 )
	{
		SendDeclareNackMsg( pmsg->dwData1, 1 );
		return;
	}
	if( pGuild1->GetMemberNum() < 5 )
	{
		SendDeclareNackMsg( pmsg->dwData1, 2 );
		return;
	}
	if( pGuild2->GetMemberNum() < 5 )
	{
		SendDeclareNackMsg( pmsg->dwData1, 3 );
		return;
	}
	int rt = CanGuildFieldWar( pGuild2->GetIdx(), pGuild1->GetIdx() );
	if( rt != 0 )
	{
		SendDeclareNackMsg( pmsg->dwData1, rt );
		return;
	}	
	if( IsGuildFieldWar( pGuild1->GetGuildInfo()->GuildIdx, pGuild2->GetGuildInfo()->GuildIdx ) )
		return;

	SEND_GUILD_CREATE_NOTIFY Declare;
	Declare.Category = MP_GUILD_FIELDWAR;
	Declare.Protocol = MP_GUILD_FIELDWAR_DECLARE;
	Declare.GuildInfo = *pGuild1->GetGuildInfo();
	pReceiver->SendMsg( (MSGBASE*)&Declare, sizeof(Declare) );
}
*/

void CGuildFieldWarMgr::SendToAllUser( DWORD dwFlag, char* pGuildName1, char* pGuildName2 )
{
	MSG_NAME2 Msg;
	Msg.Category = MP_GUILD_FIELDWAR;
	Msg.Protocol = MP_GUILD_FIELDWAR_RESULT_TOALLUSER;
	Msg.dwObjectID = dwFlag;
	SafeStrCpy( Msg.str1, pGuildName1, MAX_NAME_LENGTH +1 );
	SafeStrCpy( Msg.str2, pGuildName2, MAX_NAME_LENGTH +1 );
//	strcpy( Msg.str1, pGuildName1 );
//	strcpy( Msg.str2, pGuildName2 );

	g_Network.Broadcast2AgentServer( (char*)&Msg, sizeof(Msg) );
}
