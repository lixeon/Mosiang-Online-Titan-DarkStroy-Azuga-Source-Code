// GuildUnionManager.cpp: implementation of the CGuildUnionManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildUnionManager.h"
#include "GuildUnion.h"
#include "Guild.h"
#include "GuildManager.h"
#include "GuildMark.h"
#include "MapDBMsgParser.h"
#include "Player.h"
#include "UserTable.h"
#include "Network.h"
#include "SiegeWarMgr.h"
#include "PackedData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuildUnionManager::CGuildUnionManager()
{
	m_GuildUnionTable.Initialize( 200 );
	m_GuildUnionMarkTable.Initialize( 64 );
	m_GuildUnionTimeTable.Initialize( 200 );
}

CGuildUnionManager::~CGuildUnionManager()
{
	CGuildUnion* pData = NULL;
	m_GuildUnionTable.SetPositionHead();
	while( pData = m_GuildUnionTable.GetData() )
		delete pData;
	m_GuildUnionTable.RemoveAll();

	CGuildUnionMark* pMark = NULL;
	m_GuildUnionMarkTable.SetPositionHead();
	while( pMark = m_GuildUnionMarkTable.GetData() )
		delete pMark;
	m_GuildUnionMarkTable.RemoveAll();

	stTIME* pTime = NULL;
	m_GuildUnionTimeTable.SetPositionHead();
	while( pTime = m_GuildUnionTimeTable.GetData() )
		delete pTime;
	m_GuildUnionTimeTable.RemoveAll();
}

/*
void CGuildUnionManager::LoadGuildUnion( DWORD dwUnionIdx, char* pName, DWORD dwGuild0, DWORD dwGuild1, DWORD dwGuild2, DWORD dwGuild3, DWORD dwGuild4, DWORD dwGuild5, DWORD dwGuild6, DWORD dwMarkIdx )
{
	if( !dwUnionIdx )	return;
	if( m_GuildUnionTable.GetData( dwUnionIdx ) ) return;

	CGuildUnion* pUnion = new CGuildUnion;
	pUnion->LoadUnionInfo( dwUnionIdx, pName, dwGuild0, dwGuild1, dwGuild2, dwGuild3, dwGuild4, dwGuild5, dwGuild6, dwMarkIdx );
	m_GuildUnionTable.Add( pUnion, dwUnionIdx );
}*/

void CGuildUnionManager::LoadGuildUnion( DWORD dwUnionIdx, char* pName, DWORD dwMarkIdx, DWORD* pGuildIdxList )
{
	if( !dwUnionIdx )	return;
	if( m_GuildUnionTable.GetData( dwUnionIdx ) ) return;

	CGuildUnion* pUnion = new CGuildUnion;
	pUnion->LoadUnionInfo( dwUnionIdx, pName, dwMarkIdx, pGuildIdxList );
	m_GuildUnionTable.Add( pUnion, dwUnionIdx );
}

void CGuildUnionManager::LoadGuildUnionMark( DWORD dwMarkIdx, DWORD dwGuildUnionIdx, char* pImgData )
{
	if( !dwMarkIdx )		return;
	if( !dwGuildUnionIdx )	return;

	CGuildUnion* pUnion = m_GuildUnionTable.GetData( dwGuildUnionIdx );
	if( !pUnion )	return;

	// 변환
	char Data[GUILDUNIONMARK_BUFSIZE];
	char tempBuf[3] = {0,};
	int curpos = 0;
	for( int n = 0; n < GUILDUNIONMARK_BUFSIZE; ++n )
	{
		strncpy( tempBuf, &pImgData[curpos], 2 ); // "FF"
		Data[n] = HexToByte( tempBuf );
		curpos += 2;
	}

	CGuildUnionMark* pMark = m_GuildUnionMarkTable.GetData( dwGuildUnionIdx );
	if( pMark )
		pMark->Init( dwMarkIdx, Data );
	else
	{
		pMark = new CGuildUnionMark;
		pMark->Init( dwMarkIdx, Data );
		m_GuildUnionMarkTable.Add( pMark, dwGuildUnionIdx );
	}
}

void CGuildUnionManager::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch( Protocol )
	{
	case MP_GUILD_UNION_CREATE_SYN:				MsgGuildUnionCreateSyn( pMsg );				break;
	case MP_GUILD_UNION_CREATE_NOTIFY_TO_MAP:	MsgGuildUnionCreateNotifyToMap( pMsg );		break;	
	case MP_GUILD_UNION_DESTROY_SYN:			MsgGuildUnionDestroySyn( pMsg );			break;
	case MP_GUILD_UNION_DESTROY_NOTIFY_TO_MAP:	MsgGuildUnionDestroyNotifyToMap( pMsg );	break;
	case MP_GUILD_UNION_INVITE:					MsgGuildUnionInvite( pMsg );				break;
	case MP_GUILD_UNION_INVITE_ACCEPT:			MsgGuildUnionInviteAccept( pMsg );			break;
	case MP_GUILD_UNION_INVITE_ACCEPT_NOTIFY_TO_MAP:	MsgGuildUnionInviteAcceptNotifyToMap( pMsg );	break;	
	case MP_GUILD_UNION_INVITE_DENY:			MsgGuildUnionInviteDeny( pMsg );			break;
	case MP_GUILD_UNION_REMOVE_SYN:				MsgGuildUnionRemoveSyn( pMsg );				break;
	case MP_GUILD_UNION_REMOVE_NOTIFY_TO_MAP:	MsgGuildUnionRemoveNotifyToMap( pMsg );		break;
	case MP_GUILD_UNION_SECEDE_SYN:				MsgGuildUnionSecedeSyn( pMsg );				break;
	case MP_GUILD_UNION_SECEDE_NOTIFY_TO_MAP:	MsgGuildUnionSecedeNotifyToMap( pMsg );		break;
	case MP_GUILD_UNION_MARK_REGIST_SYN:		MsgGuildUnionMarkRegistSyn( pMsg );			break;
	case MP_GUILD_UNION_MARK_REGIST_NOTIFY_TO_MAP:	MsgGuildUnionMarkRegistNotifyToMap( pMsg );	break;
	case MP_GUILD_UNION_MARK_REQUEST_SYN:		MsgGuildUnionMarkRequestSyn( pMsg );		break;
	case MP_GUILD_UNION_NOTE_SYN:				MsgGuildUnionNoteSyn( pMsg ); break;
	}
}

void CGuildUnionManager::MsgGuildUnionCreateSyn( void* pMsg )
{
	MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pPlayer )	return;

	// check condition
	CGuild* pGuild = GUILDMGR->GetGuild( pmsg->dwData );
	if( !pGuild )					
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_CREATE_NACK, eGU_Not_InGuild );
		return;
	}
	if( !pGuild->IsMaster( pmsg->dwObjectID ) )
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_CREATE_NACK, eGU_Not_GuildMaster );
		return;
	}
	if( pGuild->GetLevel() < GUILD_5LEVEL )
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_CREATE_NACK, eGU_Not_Level );
		return;
	}
	if( pGuild->GetGuildUnionIdx() )
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_CREATE_NACK, eGU_Aleady_InGuildUnion );
		return;
	}
	DWORD dwTime = CheckEntryTime( pmsg->dwData );
	if( dwTime != 0 )
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_CREATE_NACK, eGU_Time, dwTime );
		return;
	}
	if( !IsValidGuildUnionName( pmsg->Name ) )
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_CREATE_NACK, eGU_Not_ValidName );
		return;
	}

	// db
	GuildUnionCreate( pmsg->dwObjectID, pmsg->dwData, pmsg->Name );	
}

void CGuildUnionManager::GuildUnionCreateResult( DWORD dwGuildUnionIdx, char* pGuildUnionName, DWORD dwMasterGuildIdx )
{
	if( !dwGuildUnionIdx )	return;
	
	// add table
	CGuildUnion* pGuildUnion = m_GuildUnionTable.GetData( dwGuildUnionIdx );
	if( pGuildUnion )
	{
		delete pGuildUnion;
		m_GuildUnionTable.Remove( dwGuildUnionIdx );
	}
	pGuildUnion = new CGuildUnion;
	pGuildUnion->CreateGuildUnion( dwGuildUnionIdx, pGuildUnionName, dwMasterGuildIdx );
	m_GuildUnionTable.Add( pGuildUnion, pGuildUnion->GetGuildUnionIdx() );

	// send msg
	MSG_NAME_DWORD2 Msg1, Msg2;
	Msg1.Category = MP_GUILD_UNION;
	Msg1.Protocol = MP_GUILD_UNION_CREATE_ACK;
	Msg1.dwData1 = dwGuildUnionIdx;
	Msg1.dwData2 = dwMasterGuildIdx;
	strncpy( Msg1.Name, pGuildUnionName, MAX_GUILD_NAME+1 );
	Msg2 = Msg1;
		
	// time
	ClearEntryTime( dwMasterGuildIdx );

	// to curmap
	pGuildUnion->SendMsgToGuildUnionAll( &Msg1, sizeof(Msg1) );	

	// to othermap
	Msg2.Protocol = MP_GUILD_UNION_CREATE_NOTIFY_TO_MAP;
	g_Network.Send2AgentServer( (char*)&Msg2, sizeof(Msg2) );
	
	// SiegeWar
	SIEGEWARMGR->CreateUnionCheck( dwGuildUnionIdx );
	
	LogGuild( dwMasterGuildIdx, dwGuildUnionIdx, eGuildLog_CreateUnion );
}

void CGuildUnionManager::MsgGuildUnionCreateNotifyToMap( void* pMsg )
{
	MSG_NAME_DWORD2* pmsg = (MSG_NAME_DWORD2*)pMsg;

	CGuild* pGuild = GUILDMGR->GetGuild( pmsg->dwData2 );
	if( !pGuild )	return;

	CGuildUnion* pGuildUnion = m_GuildUnionTable.GetData( pmsg->dwData1 );
	if( pGuildUnion )
	{
		delete pGuildUnion;
		m_GuildUnionTable.Remove( pmsg->dwData1 );
	}
	pGuildUnion = new CGuildUnion;
	pGuildUnion->CreateGuildUnion( pmsg->dwData1, pmsg->Name, pmsg->dwData2 );
	m_GuildUnionTable.Add( pGuildUnion, pGuildUnion->GetGuildUnionIdx() );

	// time
	ClearEntryTime( pmsg->dwData2 );

	// to curmap
	pmsg->Protocol = MP_GUILD_UNION_CREATE_ACK;	
	pGuildUnion->SendMsgToGuildUnionAll( pmsg, sizeof(MSG_NAME_DWORD2) );

	// SiegeWar Update
	if( g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetSiegeMapNum() ||
		g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetVillageMapNum() )
	{
		if( SIEGEWARMGR->GetCastleGuildIdx() == pmsg->dwData2 )
		{
			SIEGEWARMGR->SetCastleUnionIdx( pGuildUnion->GetGuildUnionIdx() );
		}
	}
}

void CGuildUnionManager::MsgGuildUnionDestroySyn( void* pMsg )
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pPlayer )	return;

	if( SIEGEWARMGR->GetSiegeMapNum() == g_pServerSystem->GetMapNum() )
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_DESTROY_NACK, eGU_Not_ValidMap );
		return;
	}
	// check condition
	CGuild* pGuild = GUILDMGR->GetGuild( pPlayer->GetGuildIdx() );
	if( !pGuild )					
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_DESTROY_NACK, eGU_Not_InGuild );
		return;
	}
	if( !pGuild->IsMaster( pmsg->dwObjectID ) )
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_DESTROY_NACK, eGU_Not_GuildMaster );
		return;
	}
	CGuildUnion* pGuildUnion = m_GuildUnionTable.GetData( pmsg->dwData );
	if( !pGuildUnion )
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_DESTROY_NACK, eGU_Not_InGuildUnion );
		return;
	}
	if( pGuildUnion->GetMasterGuildIdx() != pGuild->GetIdx() )
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_DESTROY_NACK, eGU_Not_GuildUnionMaster );
		return;
	}

	MSG_DWORD2 Msg1, Msg2;
	Msg1.Category = MP_GUILD_UNION;
	Msg1.Protocol = MP_GUILD_UNION_DESTROY_ACK;
	Msg1.dwData1 = pmsg->dwData;
	Msg1.dwData2 = 0;
	Msg2 = Msg1;
	
	// curmap
	pGuildUnion->SendMsgToGuildUnionAll( &Msg1, sizeof(Msg1) );
	
	// othermap
	Msg2.Protocol = MP_GUILD_UNION_DESTROY_NOTIFY_TO_MAP;
	g_Network.Send2AgentServer( (char*)&Msg2, sizeof(Msg2) );

	// time
	DWORD dwTime = AddEntryTimeForDestroy( pGuildUnion->GetMasterGuildIdx() );

	// dbupdate
	GuildUnionDestroy( pGuildUnion->GetGuildUnionIdx(), pGuildUnion->GetGuildUnionMarkIdx(), pGuildUnion->GetMasterGuildIdx(), dwTime );

	LogGuild( pGuildUnion->GetMasterGuildIdx(), pGuildUnion->GetGuildUnionIdx(), eGuildLog_DestroyUnion );

	// SiegeWar Update
 	if( g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetVillageMapNum() )
 	{
 		if( pGuildUnion->GetGuildUnionIdx() == SIEGEWARMGR->GetCastleUnionIdx() )
 		{
 			SIEGEWARMGR->DestoryCastleUnion();
 		}
 	}

	// delete
	pGuildUnion->Destroy();
	delete pGuildUnion;
	m_GuildUnionTable.Remove( pmsg->dwData );

}
	
void CGuildUnionManager::MsgGuildUnionDestroyNotifyToMap( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	
	CGuildUnion* pGuildUnion = m_GuildUnionTable.GetData( pmsg->dwData1 );
	if( !pGuildUnion )	return;
	
	MSG_DWORD2 Msg;
	Msg.Category = MP_GUILD_UNION;
	Msg.Protocol = MP_GUILD_UNION_DESTROY_ACK;
	Msg.dwData1 = pmsg->dwData1;
	Msg.dwData2 = 0;
	pGuildUnion->SendMsgToGuildUnionAll( &Msg, sizeof(Msg) );

	// time
	AddEntryTimeForDestroy( pGuildUnion->GetMasterGuildIdx() );

	// SiegeWar Update
 	if( g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetSiegeMapNum() ||
 		g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetVillageMapNum() )
 	{
 		if( pGuildUnion->GetGuildUnionIdx() == SIEGEWARMGR->GetCastleUnionIdx() )
 		{
 			SIEGEWARMGR->DestoryCastleUnion();
 		}
 	}

	pGuildUnion->Destroy();
	delete pGuildUnion;
	m_GuildUnionTable.Remove( pmsg->dwData1 );	
}

void CGuildUnionManager::MsgGuildUnionInvite( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	CPlayer* pPlayer2 = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData1 );
	if( !pPlayer1 || !pPlayer2 )	return;

	if( SIEGEWARMGR->GetSiegeMapNum() == g_pServerSystem->GetMapNum() )
	{
		SendNackMsg( pPlayer1, MP_GUILD_UNION_INVITE_NACK, eGU_Not_ValidMap );
		return;
	}
	// check condition
	CGuild* pGuild1 = GUILDMGR->GetGuild( pPlayer1->GetGuildIdx() );
	if( !pGuild1 )
	{
		SendNackMsg( pPlayer1, MP_GUILD_UNION_INVITE_NACK, eGU_Not_InGuild );
		return;
	}
	if( !pGuild1->IsMaster( pmsg->dwObjectID ) )
	{
		SendNackMsg( pPlayer1, MP_GUILD_UNION_INVITE_NACK, eGU_Not_GuildMaster );
		return;
	}
	CGuildUnion* pGuildUnion = m_GuildUnionTable.GetData( pmsg->dwData2 );
	if( !pGuildUnion )
	{
		SendNackMsg( pPlayer1, MP_GUILD_UNION_INVITE_NACK, eGU_Not_InGuildUnion );
		return;
	}
	if( !pGuildUnion->IsMasterGuild( pPlayer1->GetGuildIdx() ) )
	{
		SendNackMsg( pPlayer1, MP_GUILD_UNION_INVITE_NACK, eGU_Not_GuildUnionMaster );
		return;
	}
	if( !pGuildUnion->IsEmptyPosition() )
	{
		SendNackMsg( pPlayer1, MP_GUILD_UNION_INVITE_NACK, eGU_Is_Full );
		return;
	}
	CGuild* pGuild2 = GUILDMGR->GetGuild( pPlayer2->GetGuildIdx() );
	if( !pGuild2 )
	{
		SendNackMsg( pPlayer1, MP_GUILD_UNION_INVITE_NACK, eGU_Other_Not_InGuild );
		return;
	}
	if( !pGuild2->IsMaster( pmsg->dwData1 ) )
	{
		SendNackMsg( pPlayer1, MP_GUILD_UNION_INVITE_NACK, eGU_Other_Not_GuildMaster );
		return;
	}
	if( pGuild2->GetGuildUnionIdx() )
	{
		SendNackMsg( pPlayer1, MP_GUILD_UNION_INVITE_NACK, eGU_Other_Aleady_InGuildUnion );
		return;
	}
	if( pGuildUnion->IsGuildUnion( pGuild2 ) )
	{
		SendNackMsg( pPlayer1, MP_GUILD_UNION_INVITE_NACK, eGU_Other_Aleady_InGuildUnion );
		return;
	}
	DWORD dwTime = CheckEntryTime( pGuild2->GetIdx() );
	if( dwTime != 0 )
	{
		SendNackMsg( pPlayer1, MP_GUILD_UNION_INVITE_NACK, eGU_Other_Time, dwTime );
		return;
	}
	if( pGuildUnion->IsGuildFieldWarWithUnionGuild( pGuild2->GetIdx() ) )
	{
		SendNackMsg( pPlayer1, MP_GUILD_UNION_INVITE_NACK, eGU_Is_GuildFieldWar, dwTime );
		return;
	}
	// 동맹이 가능한지 확인
 	if( FALSE == SIEGEWARMGR->IsPossibleUnion( pGuild1->GetIdx(), pGuild2->GetIdx() ) )
 	{
 		SendNackMsg( pPlayer1, MP_GUILD_UNION_INVITE_NACK, eGU_Is_GuildFieldWar, dwTime );
 		return;
 	}

	MSGBASE Wait;
	Wait.Category = MP_GUILD_UNION;
	Wait.Protocol = MP_GUILD_UNION_INVITE_WAIT;
	pPlayer1->SendMsg( &Wait, sizeof(Wait) );
	
	MSG_NAME_DWORD2 Msg;
	Msg.Category = MP_GUILD_UNION;
	Msg.Protocol = MP_GUILD_UNION_INVITE;
	Msg.dwData1 = pPlayer1->GetID();
	Msg.dwData2 = pGuildUnion->GetGuildUnionIdx();
	strncpy( Msg.Name, pGuildUnion->GetGuildUnionName(), MAX_GUILD_NAME+1 );	
	pPlayer2->SendMsg( &Msg, sizeof(Msg) );
}

void CGuildUnionManager::MsgGuildUnionInviteAccept( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	CPlayer* pPlayer2 = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData1 );
	if( !pPlayer1 || !pPlayer2 )	return;

	// check condition
	CGuild* pGuild1 = GUILDMGR->GetGuild( pPlayer1->GetGuildIdx() );
	if( !pGuild1 )	return;
	if( !pGuild1->IsMaster( pmsg->dwObjectID ) )	return;

	CGuildUnion* pGuildUnion = m_GuildUnionTable.GetData( pmsg->dwData2 );
	if( !pGuildUnion )	return;	
	if( !pGuildUnion->IsEmptyPosition() )	return;

	CGuild* pGuild2 = GUILDMGR->GetGuild( pPlayer2->GetGuildIdx() );
	if( !pGuild2 )	return;
	if( !pGuild2->IsMaster( pmsg->dwData1 ) )	return;
	if( !pGuildUnion->IsGuildUnion( pGuild2 ) )	return;	

	int nIndex = pGuildUnion->AddGuild( pGuild1->GetIdx() );
	if( nIndex == -1 )	return;

	// time
	ClearEntryTime( pGuild1->GetIdx() );

	// to curmap
	SEND_GUILD_UNION_INFO Msg;
	Msg.Category = MP_GUILD_UNION;
	Msg.Protocol = MP_GUILD_UNION_ADD_ACK;
	pGuildUnion->GetGuildUnionInfo( &Msg );	
	pGuildUnion->SendMsgToGuildUnionAll( &Msg, Msg.GetSize() );	

	// to othermap
	MSG_DWORD2 Msg2;
	Msg2.Category = MP_GUILD_UNION;
	Msg2.Protocol = MP_GUILD_UNION_INVITE_ACCEPT_NOTIFY_TO_MAP;
	Msg2.dwData1 = pGuildUnion->GetGuildUnionIdx();
	Msg2.dwData2 = pGuild1->GetIdx();
	g_Network.Send2AgentServer( (char*)&Msg2, sizeof(Msg2) );

	// dbupdate
	GuildUnionAddGuild( pGuildUnion->GetGuildUnionIdx(), nIndex, pGuild1->GetIdx() );

	// SiegeWar Update
 	if( g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetVillageMapNum() )
 	{
 		if( pGuildUnion->GetGuildUnionIdx() == SIEGEWARMGR->GetCastleUnionIdx() )
 		{
 			SIEGEWARMGR->AddAcceptGuild( pPlayer1->GetGuildIdx() );
 		}
 	}

	LogGuild( pGuildUnion->GetMasterGuildIdx(), pGuildUnion->GetGuildUnionIdx(), eGuildLog_AddUnion, pGuild1->GetIdx() );
}

void CGuildUnionManager::MsgGuildUnionInviteAcceptNotifyToMap( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CGuildUnion* pGuildUnion = m_GuildUnionTable.GetData( pmsg->dwData1 );
	if( !pGuildUnion )	return;	
	int nIndex = pGuildUnion->AddGuild( pmsg->dwData2 );
	if( nIndex == -1 )	return;

	// time
	ClearEntryTime( pmsg->dwData2 );

	// to curmap
	SEND_GUILD_UNION_INFO Msg;
	Msg.Category = MP_GUILD_UNION;
	Msg.Protocol = MP_GUILD_UNION_ADD_ACK;
	pGuildUnion->GetGuildUnionInfo( &Msg );	
	pGuildUnion->SendMsgToGuildUnionAll( &Msg, Msg.GetSize() );	

	// SiegeWar Update
 	if( g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetSiegeMapNum() ||
 		g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetVillageMapNum() )
 	{
 		if( pGuildUnion->GetGuildUnionIdx() == SIEGEWARMGR->GetCastleUnionIdx() )
 		{
 			SIEGEWARMGR->AddAcceptGuild( pmsg->dwData2 );
 		}
 	}
}

void CGuildUnionManager::MsgGuildUnionInviteDeny( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CPlayer* pPlayer1 = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	CPlayer* pPlayer2 = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData1 );
	if( !pPlayer1 || !pPlayer2 )	return;

	MSG_DWORD Msg;
	Msg.Category = MP_GUILD_UNION;
	Msg.Protocol = MP_GUILD_UNION_INVITE_DENY;
	Msg.dwData = pPlayer1->GetID();
	pPlayer2->SendMsg( &Msg, sizeof(Msg) );
}

void CGuildUnionManager::MsgGuildUnionRemoveSyn( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pPlayer )	return;

	if( SIEGEWARMGR->GetSiegeMapNum() == g_pServerSystem->GetMapNum() )
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_REMOVE_NACK, eGU_Not_ValidMap );
		return;
	}
	// check condition
	CGuild* pGuild1 = GUILDMGR->GetGuild( pPlayer->GetGuildIdx() );
	if( !pGuild1 )
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_REMOVE_NACK, eGU_Not_InGuild );
		return;
	}
	if( !pGuild1->IsMaster( pmsg->dwObjectID ) )
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_REMOVE_NACK, eGU_Not_GuildMaster );
		return;
	}
	CGuildUnion* pGuildUnion = m_GuildUnionTable.GetData( pmsg->dwData1 );
	if( !pGuildUnion )
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_REMOVE_NACK, eGU_Not_InGuildUnion );
		return;		
	}
	if( pGuildUnion->GetMasterGuildIdx() != pGuild1->GetIdx() )
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_REMOVE_NACK, eGU_Not_GuildUnionMaster );
		return;
	}
	CGuild* pGuild2 = GUILDMGR->GetGuild( pmsg->dwData2 );
	if( !pGuild2 )
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_REMOVE_NACK, eGU_Other_Not_InGuild );
		return;
	}
	if( !pGuildUnion->IsGuildUnion( pGuild2 ) )
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_REMOVE_NACK, eGU_Other_Not_InGuildUnion );
		return;
	}

	int nIndex = pGuildUnion->RemoveGuild( pGuild2 );
	if( nIndex == -1 )	return;

	// time
	ClearEntryTime( pGuild1->GetIdx() );

	// send msg to remove guild
	MSG_DWORD2 GMsg;
	GMsg.Category = MP_GUILD_UNION;
	GMsg.Protocol = MP_GUILD_UNION_DESTROY_ACK;
	GMsg.dwData1 = pmsg->dwData1;
	GMsg.dwData2 = 2;
	pGuild2->SendMsgToAll( &GMsg, sizeof(GMsg) );

	// to curmap
	SEND_GUILD_UNION_INFO Msg;
	Msg.Category = MP_GUILD_UNION;
	Msg.Protocol = MP_GUILD_UNION_REMOVE_ACK;
	pGuildUnion->GetGuildUnionInfo( &Msg );	
	pGuildUnion->SendMsgToGuildUnionAll( &Msg, Msg.GetSize() );	

	// to othermap
	MSG_DWORD2 Msg2;
	Msg2.Category = MP_GUILD_UNION;
	Msg2.Protocol = MP_GUILD_UNION_REMOVE_NOTIFY_TO_MAP;
	Msg2.dwData1 = pmsg->dwData1;
	Msg2.dwData2 = pmsg->dwData2;
	g_Network.Send2AgentServer( (char*)&Msg2, sizeof(Msg2) );

	// dbupdate	
	GuildUnionRemoveGuild( pGuildUnion->GetGuildUnionIdx(), nIndex, pGuild2->GetIdx() );

	//
 	if( g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetVillageMapNum() )
 	{
 		if( pGuildUnion->GetGuildUnionIdx() == SIEGEWARMGR->GetCastleUnionIdx() )
 		{
 			// 공성맵과 마을에 알린다.
 			SIEGEWARMGR->DeleteAcceptGuild( pmsg->dwData2 );
 		}
 	}

	LogGuild( pGuildUnion->GetMasterGuildIdx(), pGuildUnion->GetGuildUnionIdx(), eGuildLog_RemoveUnion, pGuild2->GetIdx() );
}

void CGuildUnionManager::MsgGuildUnionRemoveNotifyToMap( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	
	CGuild* pGuild = GUILDMGR->GetGuild( pmsg->dwData2 );
	if( !pGuild )	return;

	CGuildUnion* pGuildUnion = m_GuildUnionTable.GetData( pmsg->dwData1 );
	if( !pGuildUnion )	return;	
	if( !pGuildUnion->IsGuildUnion( pGuild ) )	return;

	int nIndex = pGuildUnion->RemoveGuild( pGuild );
	if( nIndex == -1 )	return;

	// time
	ClearEntryTime( pGuild->GetIdx() );

	// send msg to remove guild
	MSG_DWORD2 GMsg;
	GMsg.Category = MP_GUILD_UNION;
	GMsg.Protocol = MP_GUILD_UNION_DESTROY_ACK;
	GMsg.dwData1 = pmsg->dwData1;
	GMsg.dwData2 = 2;
	pGuild->SendMsgToAll( &GMsg, sizeof(GMsg) );

	// to curmap
	SEND_GUILD_UNION_INFO Msg;
	Msg.Category = MP_GUILD_UNION;
	Msg.Protocol = MP_GUILD_UNION_REMOVE_ACK;
	pGuildUnion->GetGuildUnionInfo( &Msg );	
	pGuildUnion->SendMsgToGuildUnionAll( &Msg, Msg.GetSize() );	

	// SiegeWar Update
	if( g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetSiegeMapNum() ||
		g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetVillageMapNum() )
	{
		if( pGuildUnion->GetGuildUnionIdx() == SIEGEWARMGR->GetCastleUnionIdx() )
		{
			// 공성맵과 마을에 알린다.
			SIEGEWARMGR->DeleteAcceptGuild( pmsg->dwData2 );
		}
	}
}

void CGuildUnionManager::MsgGuildUnionSecedeSyn( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pPlayer )	return;

	if( SIEGEWARMGR->GetSiegeMapNum() == g_pServerSystem->GetMapNum() )
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_SECEDE_NACK, eGU_Not_ValidMap );
		return;
	}

	CGuildUnion* pGuildUnion = m_GuildUnionTable.GetData( pmsg->dwData1 );
	if( !pGuildUnion )
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_SECEDE_NACK, eGU_Not_InGuildUnion );
		return;
	}	
	DWORD dwGuildIdx = pmsg->dwData2;
	CGuild* pGuild = GUILDMGR->GetGuild( dwGuildIdx );
	if( !pGuild )
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_SECEDE_NACK, eGU_Not_InGuild );
		return;
	}
	if( !pGuild->IsMaster( pmsg->dwObjectID ) )
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_SECEDE_NACK, eGU_Not_GuildMaster );
		return;
	}
	
	int nIndex = pGuildUnion->RemoveGuild( pGuild );
	if( nIndex == -1 )	return;

	// send msg to secede guild
	MSG_DWORD2 GMsg;
	GMsg.Category = MP_GUILD_UNION;
	GMsg.Protocol = MP_GUILD_UNION_DESTROY_ACK;
	GMsg.dwData1 = pmsg->dwData1;
	GMsg.dwData2 = 1;
	pGuild->SendMsgToAll( &GMsg, sizeof(GMsg) );

	// to curmap
	SEND_GUILD_UNION_INFO Msg;
	Msg.Category = MP_GUILD_UNION;
	Msg.Protocol = MP_GUILD_UNION_SECEDE_ACK;
	pGuildUnion->GetGuildUnionInfo( &Msg );	
	pGuildUnion->SendMsgToGuildUnionAll( &Msg, Msg.GetSize() );	

	// to othermap
	MSG_DWORD2 Msg2;
	Msg2.Category = MP_GUILD_UNION;
	Msg2.Protocol = MP_GUILD_UNION_SECEDE_NOTIFY_TO_MAP;
	Msg2.dwData1 = pmsg->dwData1;
	Msg2.dwData2 = pmsg->dwData2;
	g_Network.Send2AgentServer( (char*)&Msg2, sizeof(Msg2) );

	// time
	DWORD dwTime = AddEntryTimeForSecede( pGuild->GetIdx() );

	// dbupdate	
	GuildUnionSecedeGuild( pGuildUnion->GetGuildUnionIdx(), nIndex, dwGuildIdx, dwTime );

	if(	g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetVillageMapNum() )
 	{
 		if( pGuildUnion->GetGuildUnionIdx() == SIEGEWARMGR->GetCastleUnionIdx() )
 		{
 			// 공성맵과 마을에 알린다.			
 			SIEGEWARMGR->DeleteAcceptGuild( pmsg->dwData2 );
 		}
 	}

	LogGuild( pGuildUnion->GetMasterGuildIdx(), pGuildUnion->GetGuildUnionIdx(), eGuildLog_SecedeUnion, pGuild->GetIdx() );
}

void CGuildUnionManager::MsgGuildUnionSecedeNotifyToMap( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CGuildUnion* pGuildUnion = m_GuildUnionTable.GetData( pmsg->dwData1 );
	if( !pGuildUnion )	return;	

	DWORD dwGuildIdx = pmsg->dwData2;
	CGuild* pGuild = GUILDMGR->GetGuild( dwGuildIdx );
	if( !pGuild )	return;

	int nIndex = pGuildUnion->RemoveGuild( pGuild );
	if( nIndex == -1 )	return;

	// time
	AddEntryTimeForSecede( pGuild->GetIdx() );

	// send msg to secede guild
	MSG_DWORD2 GMsg;
	GMsg.Category = MP_GUILD_UNION;
	GMsg.Protocol = MP_GUILD_UNION_DESTROY_ACK;
	GMsg.dwData1 = pmsg->dwData1;
	GMsg.dwData2 = 1;
	pGuild->SendMsgToAll( &GMsg, sizeof(GMsg) );

	// to curmap
	SEND_GUILD_UNION_INFO Msg;
	Msg.Category = MP_GUILD_UNION;
	Msg.Protocol = MP_GUILD_UNION_SECEDE_ACK;
	pGuildUnion->GetGuildUnionInfo( &Msg );	
	pGuildUnion->SendMsgToGuildUnionAll( &Msg, Msg.GetSize() );	

	// SiegeWar Update
 	if( g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetSiegeMapNum() ||
 		g_pServerSystem->GetMapNum() == SIEGEWARMGR->GetVillageMapNum() )
 	{
 		if( pGuildUnion->GetGuildUnionIdx() == SIEGEWARMGR->GetCastleUnionIdx() )
 		{
 			// 공성맵과 마을에 알린다.			
 			SIEGEWARMGR->DeleteAcceptGuild( pmsg->dwData2 );
 		}
 	}
}

void CGuildUnionManager::MsgGuildUnionMarkRegistSyn( void* pMsg )
{
	MSG_GUILDUNIONMARK_IMG* pmsg = (MSG_GUILDUNIONMARK_IMG*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pPlayer )	return;

	CGuildUnion* pGuildUnion = m_GuildUnionTable.GetData( pmsg->dwGuildUnionIdx );
	if( !pGuildUnion )
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_MARK_REGIST_NACK, eGU_Not_InGuildUnion );
		return;
	}	
	CGuild* pGuild = GUILDMGR->GetGuild( pPlayer->GetGuildIdx() );
	if( !pGuild )
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_MARK_REGIST_NACK, eGU_Not_InGuild );
		return;
	}
	if( !pGuild->IsMaster( pmsg->dwObjectID ) )
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_MARK_REGIST_NACK, eGU_Not_GuildMaster );
		return;
	}
	if( pGuildUnion->GetMasterGuildIdx() != pGuild->GetIdx() )
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_MARK_REGIST_NACK, eGU_Not_GuildUnionMaster );
		return;
	}

	GuildUnionMarkRegist( pmsg->dwObjectID, pmsg->dwMarkIdx, pmsg->dwGuildUnionIdx, pmsg->imgData );
}

void CGuildUnionManager::GuildUnionMarkRegistResult( DWORD dwCharacterIdx, DWORD dwMarkIdx, DWORD dwGuildUnionIdx, char* pImgData )
{
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( dwCharacterIdx );
	if( !pPlayer )	return;

	if( !dwMarkIdx )		return;
	if( !dwGuildUnionIdx )	return;
	CGuildUnion* pUnion = m_GuildUnionTable.GetData( dwGuildUnionIdx );
	if( !pUnion )	return;
	pUnion->SetGuildUnionMarkIdx( dwMarkIdx );

	MSG_GUILDUNIONMARK_IMG Msg1, Msg2;
	char tempBuf[3] = {0,};
	int curpos = 0;
	for( int n = 0; n < GUILDUNIONMARK_BUFSIZE; ++n )
	{
		strncpy( tempBuf, &pImgData[curpos], 2 ); // "FF"
		Msg1.imgData[n] = HexToByte( tempBuf );
		curpos += 2;
	}	
	Msg2 = Msg1;

	// send to othermap
	Msg1.Category = MP_GUILD_UNION;
	Msg1.Protocol = MP_GUILD_UNION_MARK_REGIST_NOTIFY_TO_MAP;
	Msg1.dwGuildUnionIdx = dwGuildUnionIdx;
	Msg1.dwMarkIdx = dwMarkIdx;
	g_Network.Send2AgentServer( (char*)&Msg1, sizeof(Msg1) );

	// regist
	CGuildUnionMark* pMark = m_GuildUnionMarkTable.GetData( dwGuildUnionIdx );
	if( pMark )
		pMark->Init( dwMarkIdx, Msg2.imgData );
	else
	{
		pMark = new CGuildUnionMark;
		pMark->Init( dwMarkIdx, Msg2.imgData );
		m_GuildUnionMarkTable.Add( pMark, dwGuildUnionIdx );
	}

	// send to player within
	Msg2.Category = MP_GUILD_UNION;
	Msg2.Protocol = MP_GUILD_UNION_MARK_REGIST_ACK;
	Msg2.dwObjectID = dwCharacterIdx;
	Msg2.dwGuildUnionIdx = dwGuildUnionIdx;
	Msg2.dwMarkIdx = dwMarkIdx;
	PACKEDDATA_OBJ->QuickSend( pPlayer, &Msg2, sizeof(Msg2) );
}

void CGuildUnionManager::MsgGuildUnionMarkRegistNotifyToMap( void* pMsg )
{
	MSG_GUILDUNIONMARK_IMG* pmsg = (MSG_GUILDUNIONMARK_IMG*)pMsg;

	DWORD dwGuildUnionIdx = pmsg->dwGuildUnionIdx;
	DWORD dwMarkIdx = pmsg->dwMarkIdx;
	CGuildUnion* pUnion = m_GuildUnionTable.GetData( dwGuildUnionIdx );
	if( !pUnion )	return;
	pUnion->SetGuildUnionMarkIdx( dwMarkIdx );

	CGuildUnionMark* pMark = m_GuildUnionMarkTable.GetData( dwGuildUnionIdx );
	if( pMark )
		pMark->Init( dwMarkIdx, pmsg->imgData );
	else
	{
		pMark = new CGuildUnionMark;
		pMark->Init( dwMarkIdx, pmsg->imgData );
		m_GuildUnionMarkTable.Add( pMark, dwGuildUnionIdx );
	}
}

void CGuildUnionManager::MsgGuildUnionMarkRequestSyn( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pPlayer )	return;
	CGuildUnion* pUnion = m_GuildUnionTable.GetData( pmsg->dwData1 );
	if( !pUnion )	return;
	CGuildUnionMark* pMark = m_GuildUnionMarkTable.GetData( pmsg->dwData1 );
	if( !pMark )	return;
	
	MSG_GUILDUNIONMARK_IMG Msg;
	Msg.Category = MP_GUILD_UNION;
	Msg.Protocol = MP_GUILD_UNION_MARK_REQUEST_ACK;
	Msg.dwGuildUnionIdx = pmsg->dwData1;
	Msg.dwMarkIdx = pmsg->dwData2;
	memcpy( Msg.imgData, pMark->GetImgData(), GUILDUNIONMARK_BUFSIZE );
	pPlayer->SendMsg( &Msg, sizeof(Msg) );
}


void CGuildUnionManager::MsgGuildUnionNoteSyn( void* pMsg )
{
	MSG_GUILD_SEND_NOTE* msg = (MSG_GUILD_SEND_NOTE*) pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( msg->dwObjectID );
	if( !pPlayer )	return;

	CGuildUnion* pGuildUnion = m_GuildUnionTable.GetData( msg->FromId );
	if( !pGuildUnion )
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_NOTE_NACK, eGU_Not_InGuildUnion );
		return;
	}	
	CGuild* pGuild = GUILDMGR->GetGuild( pPlayer->GetGuildIdx() );
	if( !pGuild )
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_NOTE_NACK, eGU_Not_InGuild );
		return;
	}
	if( !pGuild->IsMaster( msg->dwObjectID ) && !pGuild->IsViceMaster( msg->dwObjectID ))
	{
		SendNackMsg( pPlayer, MP_GUILD_UNION_NOTE_NACK, eGU_Not_GuildMaster );
		return;
	}

	pGuildUnion->SendAllNote(pPlayer, msg->Note);
	
	MSGBASE ack;
	ack.Category = MP_GUILD_UNION;
	ack.Protocol = MP_GUILD_UNION_NOTE_ACK;
	pPlayer->SendMsg(&ack, sizeof(ack));
}

void CGuildUnionManager::SendNackMsg( CPlayer* pPlayer, BYTE Protocol, DWORD dwErr, DWORD dwData /* = 0  */ )
{
	MSG_DWORD2 Msg;
	Msg.Category = MP_GUILD_UNION;
	Msg.Protocol = Protocol;
	Msg.dwData1 = dwErr;
	Msg.dwData2 = dwData;
	pPlayer->SendMsg( &Msg, sizeof(Msg) );
}

void CGuildUnionManager::SendChatMsg( MSG_CHAT_WITH_SENDERID* pMsg )
{
	CGuildUnion* pUnion = m_GuildUnionTable.GetData( pMsg->dwSenderID );
	if( !pUnion )	return;

	MSG_CHAT_WITH_SENDERID Msg;
	Msg = *pMsg;
	pUnion->SendMsgToGuildUnionAll( &Msg, Msg.GetMsgLength() );
}

void CGuildUnionManager::AddPlayer( CPlayer* pPlayer, DWORD dwGuildUnionIdx )
{
	if( !dwGuildUnionIdx )	return;

	CGuildUnion* pUnion = m_GuildUnionTable.GetData( dwGuildUnionIdx );
	if( !pUnion )	return;

	SEND_GUILD_UNION_INFO Msg;
	Msg.Category = MP_GUILD_UNION;
	Msg.Protocol = MP_GUILD_UNION_INFO;
	pUnion->GetGuildUnionInfo( &Msg );
	pPlayer->SendMsg( &Msg, Msg.GetSize() );

	pPlayer->SetGuildUnionInfo( dwGuildUnionIdx, pUnion->GetGuildUnionName(), pUnion->GetGuildUnionMarkIdx() );
}

void CGuildUnionManager::LoginPlayer( CPlayer* pPlayer, DWORD dwGuildUnionIdx )
{
	if( !dwGuildUnionIdx )	return;

	CGuildUnion* pUnion = m_GuildUnionTable.GetData( dwGuildUnionIdx );
	if( !pUnion )	return;

	SEND_GUILD_UNION_INFO Msg;
	Msg.Category = MP_GUILD_UNION;
	Msg.Protocol = MP_GUILD_UNION_INFO;
	pUnion->GetGuildUnionInfo( &Msg );
	pPlayer->SendMsg( &Msg, Msg.GetSize() );
}

void CGuildUnionManager::SetGuildUnionInfoToCharacterTotalInfo( CPlayer* pPlayer, DWORD dwGuildIdx )
{
	if( !dwGuildIdx )	return;

	CGuild* pGuild = GUILDMGR->GetGuild( dwGuildIdx );
	if( !pGuild )	return;
	if( !pGuild->GetGuildUnionIdx() )	return;

	CGuildUnion* pUnion = m_GuildUnionTable.GetData( pGuild->GetGuildUnionIdx() );
	if( !pUnion )	return;

	pPlayer->InitGuildUnionInfo( pUnion->GetGuildUnionIdx(), pUnion->GetGuildUnionName(), pUnion->GetGuildUnionMarkIdx() );
}

BOOL CGuildUnionManager::IsValidGuildUnionName( char* pName )
{
	CGuildUnion* pUnion = NULL;
	m_GuildUnionTable.SetPositionHead();
	while( pUnion = m_GuildUnionTable.GetData() )
	{
		if( strcmp( pUnion->GetGuildUnionName(), pName ) == 0 )
			return FALSE;
	}
	if( strlen(pName) > MAX_GUILD_NAME )
		return FALSE;
	return TRUE;
}

DWORD CGuildUnionManager::CheckEntryTime( DWORD dwGuildIdx )
{
	stTIME* pTime = m_GuildUnionTimeTable.GetData( dwGuildIdx );
	if( !pTime )	return 0;

	stTIME curTime;
	curTime.value = GetCurTime();

	if( curTime.value > pTime->value )
	{
		pTime->value = 0;
		return 0;
	}

	return pTime->value;
}

void CGuildUnionManager::ClearEntryTime( DWORD dwGuildIdx )
{
	stTIME* pTime = m_GuildUnionTimeTable.GetData( dwGuildIdx );
	if( !pTime )	return;
	pTime->value = 0;
}

void CGuildUnionManager::AddEntryTime( DWORD dwGuildIdx, DWORD dwTime )
{
	stTIME* pTime = m_GuildUnionTimeTable.GetData( dwGuildIdx );
	if( pTime )
		pTime->value = dwTime;
	else
	{
		pTime = new stTIME;
		pTime->value = dwTime;

		m_GuildUnionTimeTable.Add( pTime, dwGuildIdx );
	}
}

DWORD CGuildUnionManager::AddEntryTimeForDestroy( DWORD dwGuildIdx )
{
	stTIME time;
	time.SetTime( 0, 0, 5, 0, 0, 0 );

	stTIME* pTime = m_GuildUnionTimeTable.GetData( dwGuildIdx );
	if( pTime )
	{
		pTime->value = GetCurTime();
		*pTime += time;
	}
	else
	{
		pTime = new stTIME;
		pTime->value = GetCurTime();
		*pTime += time;

		m_GuildUnionTimeTable.Add( pTime, dwGuildIdx );
	}

	return pTime->value;		
}

DWORD CGuildUnionManager::AddEntryTimeForSecede( DWORD dwGuildIdx )
{
	stTIME time;
	time.SetTime( 0, 0, 1, 0, 0, 0 );

	stTIME* pTime = m_GuildUnionTimeTable.GetData( dwGuildIdx );
	if( pTime )
	{
		pTime->value = GetCurTime();
		*pTime += time;
	}
	else
	{
		pTime = new stTIME;
		pTime->value = GetCurTime();
		*pTime += time;

		m_GuildUnionTimeTable.Add( pTime, dwGuildIdx );
	}

	return pTime->value;	
}

//반드시 "FF" 이런식으로 입력이 되어야 한다.
BYTE CGuildUnionManager::HexToByte( char* pStr )
{
	int n1 = convertCharToInt(pStr[0]);
	int n2 = convertCharToInt(pStr[1]);

	return n1 * 16 + n2;
}

int CGuildUnionManager::convertCharToInt( char c )
{
	if('0' <= c && c <= '9')
		return c - '0';
	if('A' <= c && c <= 'F')
		return c - 'A' +10;
	if('a' <= c && c <= 'f')
		return c - 'a' +10;
	return 0;
}


//
BOOL CGuildUnionManager::GetGuildListInUnion( DWORD dwUnionIdx, cPtrList* pList )
{
	CGuildUnion* pUnion = m_GuildUnionTable.GetData( dwUnionIdx );
	if( !pUnion )			return FALSE;

	pUnion->GetGuildListInUnion( pList );
	return TRUE;
}


CGuild*	CGuildUnionManager::GetMasterGuildInUnion( DWORD dwUnionIdx )
{
	CGuildUnion* pUnion = m_GuildUnionTable.GetData( dwUnionIdx );
	if( !pUnion )			return NULL;

	return pUnion->GetMasterGuild();
}

//SW080515 함수 추가.
BOOL CGuildUnionManager::IsSameUnion( CPlayer* pPlayerA, CPlayer* pPlayerB )
{
	if( !(pPlayerA && pPlayerB) )
	{
		ASSERT(0);
		return FALSE;
	}

	DWORD GuildUnionIdx = pPlayerA->GetGuildUnionIdx();
	if( GuildUnionIdx )
	if( GuildUnionIdx == pPlayerB->GetGuildUnionIdx() )
		return TRUE;

	return FALSE;
}
