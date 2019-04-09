// GuildUnion.cpp: implementation of the CGuildUnion class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildUnion.h"
#include "ObjectManager.h"
#include "GuildManager.h"
#include "GameIn.h"
#include "GuildDialog.h"
#include "GuildCreateDialog.h"
#include "WindowIDEnum.h"
#include "cMsgBox.h"
#include "./Interface/cWindowManager.h"
#include "ChatManager.h"
#include "GuildUnionMarkMgr.h"
#include "GuildFieldWar.h"
#include "GuildFieldWarDialog.h"
#include "MonsterGuageDlg.h"
#include "SiegeWarMgr.h"
#include "MHMap.h"
#include "UnionNoteDlg.h"

extern int g_nServerSetNum;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CGuildUnion)
CGuildUnion::CGuildUnion()
{
	m_dwGuildUnionIdx = 0;
	memset( m_sGuildUnionName, 0, MAX_GUILD_NAME+1 );
	memset( m_GuildInfo, 0, sizeof(sGUILDIDXNAME)*MAX_GUILD_UNION_NUM );

	m_dwTempPlayerId = m_dwTempGuildUnionIdx = m_dwTempGuildIdx = 0;
}

CGuildUnion::~CGuildUnion()
{
}

BOOL CGuildUnion::IsGuildUnionMaster( CPlayer* pPlayer )
{
	if( pPlayer->GetGuildMemberRank() != GUILD_MASTER )	return FALSE;
	if( m_dwGuildUnionIdx == 0 )	return FALSE;
	if( m_dwGuildUnionIdx != pPlayer->GetGuildUnionIdx() )	return FALSE;
	if( m_GuildInfo[0].dwGuildIdx != pPlayer->GetGuildIdx() )	return FALSE;

	return TRUE;
}

char* CGuildUnion::GetGuildUnionNameFromListIdx( int nIdx )
{
	if( nIdx < 0 || nIdx >= MAX_GUILD_UNION_NUM )
	{
		m_dwTempGuildIdx = 0;
		return NULL;
	}
	m_dwTempGuildIdx = m_GuildInfo[nIdx+1].dwGuildIdx;
	if( !m_dwTempGuildIdx )
		return NULL;
	return m_GuildInfo[nIdx+1].sGuildName;
}

void CGuildUnion::Clear()
{
	m_dwGuildUnionIdx = 0;
	memset( m_sGuildUnionName, 0, MAX_GUILD_NAME+1 );
	memset( m_GuildInfo, 0, sizeof(sGUILDIDXNAME)*MAX_GUILD_UNION_NUM );

	m_dwTempPlayerId = m_dwTempGuildUnionIdx = m_dwTempGuildIdx = 0;

	if( HERO )
	{
		HERO->SetGuildUnionIdx( 0 );
		HERO->SetGuildUnionName( "" );
		HERO->SetGuildUnionMarkIdx( 0 );
	}
		
	GAMEIN->GetGuildDlg()->SetGuildUnionName();
}

void CGuildUnion::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch( Protocol )
	{
	case MP_GUILD_UNION_PLAYER_INFO:	MsgGuildUnionPlayerInfo( pMsg );	break;
	case MP_GUILD_UNION_INFO:			MsgGuildUnionInfo( pMsg );			break;
	case MP_GUILD_UNION_CREATE_ACK:		MsgGuildUnionCreateAck( pMsg );		break;
	case MP_GUILD_UNION_DESTROY_ACK:	MsgGuildUnionDestroyAck( pMsg );	break;
	case MP_GUILD_UNION_INVITE:			MsgGuildUnionInvite( pMsg );		break;
	case MP_GUILD_UNION_INVITE_WAIT:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1141) );	break;
	case MP_GUILD_UNION_INVITE_DENY:	MsgGuildUnionInviteDeny( pMsg );	break;
	case MP_GUILD_UNION_ADD_ACK:		MsgGuildUnionAddAck( pMsg );		break;
	case MP_GUILD_UNION_REMOVE_ACK:		MsgGuildUnionRemoveAck( pMsg );		break;
	case MP_GUILD_UNION_SECEDE_ACK:		MsgGuildUnionSecedeAck( pMsg );		break;
	case MP_GUILD_UNION_MARK_REGIST_ACK:	MsgGuildUnionMarkRegistAck( pMsg );	break;
	case MP_GUILD_UNION_MARK_REQUEST_ACK:	MsgGuildUnionMarkRequestAck( pMsg );	break;
	case MP_GUILD_UNION_NOTE_ACK:	
		{
			GAMEIN->GetUnionNoteDlg()->Use();	
			CHATMGR->AddMsg(CTC_TOWHOLE, CHATMGR->GetChatMsg(593));
		}
		break;
	
	// nack
	case MP_GUILD_UNION_CREATE_NACK:	MsgGuildUnionCreateNack( pMsg );	break;
	case MP_GUILD_UNION_DESTROY_NACK:	MsgGuildUnionDestroyNack( pMsg );	break; 
	case MP_GUILD_UNION_INVITE_NACK:	MsgGuildUnionInviteNack( pMsg );	break;
	case MP_GUILD_UNION_REMOVE_NACK:	MsgGuildUnionRemoveNack( pMsg );	break;
	case MP_GUILD_UNION_SECEDE_NACK:	MsgGuildUnionSecedeNack( pMsg );	break;
	case MP_GUILD_UNION_MARK_REGIST_NACK:	MsgGuildUnionMarkRegistNack( pMsg );	break;
	}	
}

void CGuildUnion::MsgGuildUnionPlayerInfo( void* pMsg )
{
	MSG_NAME_DWORD3* pmsg = (MSG_NAME_DWORD3*)pMsg;
	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwData1);
	if( !pPlayer )	return;
	pPlayer->SetGuildUnionIdx( pmsg->dwData2 );
	pPlayer->SetGuildUnionName( pmsg->Name );
	pPlayer->SetGuildUnionMarkIdx( pmsg->dwData3 );

	GAMEIN->GetMonsterGuageDlg()->SetGuildUnionName( pPlayer->GetGuildName(), pPlayer->GetGuildUnionName() );				
}

void CGuildUnion::MsgGuildUnionInfo( void* pMsg )
{
	SEND_GUILD_UNION_INFO* pmsg = (SEND_GUILD_UNION_INFO*)pMsg;

	m_dwGuildUnionIdx = pmsg->dwGuildUnionIdx;
	strncpy( m_sGuildUnionName, pmsg->sGuildUnionName, MAX_GUILD_NAME+1 );
	for( int i = 0; i < pmsg->nMaxGuildNum; ++i )
		m_GuildInfo[i] = pmsg->GuildInfo[i];
	HERO->SetGuildUnionIdx( m_dwGuildUnionIdx );
	HERO->SetGuildUnionName( m_sGuildUnionName );
	HERO->SetGuildUnionMarkIdx( pmsg->dwGuildUnionMarkIdx );
		
	GAMEIN->GetGuildDlg()->SetGuildUnionName();
}

void CGuildUnion::MsgGuildUnionCreateAck( void* pMsg )
{
	MSG_NAME_DWORD2* pmsg = (MSG_NAME_DWORD2*)pMsg;	
	if( pmsg->dwData2 != HERO->GetGuildIdx() )	return;

	m_dwGuildUnionIdx = pmsg->dwData1;
	strncpy( m_sGuildUnionName, pmsg->Name, MAX_GUILD_NAME+1 );
	m_GuildInfo[0].dwGuildIdx = pmsg->dwData2;
	strncpy( m_GuildInfo[0].sGuildName, GUILDMGR->GetGuildName(), MAX_GUILD_NAME+1 );

	HERO->SetGuildUnionIdx( m_dwGuildUnionIdx );
	HERO->SetGuildUnionName( m_sGuildUnionName );
	HERO->SetGuildUnionMarkIdx( 0 );

	GAMEIN->GetGuildDlg()->SetGuildUnionName();

	// chat msg
	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1104), m_sGuildUnionName );
}

void CGuildUnion::MsgGuildUnionDestroyAck( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	
	if( m_dwGuildUnionIdx == pmsg->dwData1 )
	{
		Clear();

		GAMEIN->GetGuildWarInfoDlg()->AddListData();

		// chat msg
		if( pmsg->dwData2 == 0 )
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1105) );
		else if( pmsg->dwData2 == 1 )
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1106) );
		else if( pmsg->dwData2 == 2 )
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1107) );
	}
}

void CGuildUnion::MsgGuildUnionInvite( void* pMsg )
{
	MSG_NAME_DWORD2* pmsg = (MSG_NAME_DWORD2*)pMsg;

	m_dwTempPlayerId = pmsg->dwData1;
	m_dwTempGuildUnionIdx = pmsg->dwData2;
	WINDOWMGR->MsgBox( MBI_GUILD_UNION_INVITE, MBT_YESNO, CHATMGR->GetChatMsg(1108), pmsg->Name );	
}

void CGuildUnion::MsgGuildUnionInviteDeny( void* pMsg )
{
	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1109) );
}

void CGuildUnion::MsgGuildUnionAddAck( void* pMsg )
{
	SEND_GUILD_UNION_INFO* pmsg = (SEND_GUILD_UNION_INFO*)pMsg;
	
	m_dwGuildUnionIdx = pmsg->dwGuildUnionIdx;
	strncpy( m_sGuildUnionName, pmsg->sGuildUnionName, MAX_GUILD_NAME+1 );
	memset( m_GuildInfo, 0, sizeof(sGUILDIDXNAME)*MAX_GUILD_UNION_NUM );
	for( int i = 0; i < pmsg->nMaxGuildNum; ++i )
		m_GuildInfo[i] = pmsg->GuildInfo[i];
	
	if( HERO )
	{
		HERO->SetGuildUnionIdx( m_dwGuildUnionIdx );
		HERO->SetGuildUnionName( m_sGuildUnionName );
		HERO->SetGuildUnionMarkIdx( pmsg->dwGuildUnionMarkIdx );
	}

	GAMEIN->GetGuildDlg()->SetGuildUnionName();
	GAMEIN->GetGuildWarInfoDlg()->AddListData();

	// chat msg
	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1110) );
}

void CGuildUnion::MsgGuildUnionRemoveAck( void* pMsg )
{
	SEND_GUILD_UNION_INFO* pmsg = (SEND_GUILD_UNION_INFO*)pMsg;

	memset( m_GuildInfo, 0, sizeof(sGUILDIDXNAME)*MAX_GUILD_UNION_NUM );
	for( int i = 0; i < pmsg->nMaxGuildNum; ++i )
		m_GuildInfo[i] = pmsg->GuildInfo[i];

	GAMEIN->GetGuildWarInfoDlg()->AddListData();

	// chat msg
	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1111) );
}

void CGuildUnion::MsgGuildUnionSecedeAck( void* pMsg )
{
	SEND_GUILD_UNION_INFO* pmsg = (SEND_GUILD_UNION_INFO*)pMsg;

	memset( m_GuildInfo, 0, sizeof(sGUILDIDXNAME)*MAX_GUILD_UNION_NUM );
	for( int i = 0; i < pmsg->nMaxGuildNum; ++i )
		m_GuildInfo[i] = pmsg->GuildInfo[i];

	GAMEIN->GetGuildWarInfoDlg()->AddListData();

	// chat msg
	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1112) );
}

void CGuildUnion::MsgGuildUnionMarkRequestAck( void* pMsg )
{
	MSG_GUILDUNIONMARK_IMG* pmsg = (MSG_GUILDUNIONMARK_IMG*)pMsg;

	GUILDUNIONMARKMGR->SaveGuildUnionMark( g_nServerSetNum, pmsg->dwGuildUnionIdx, pmsg->dwMarkIdx, pmsg->imgData );
}

void CGuildUnion::MsgGuildUnionMarkRegistAck( void* pMsg )
{
	MSG_GUILDUNIONMARK_IMG* pmsg = (MSG_GUILDUNIONMARK_IMG*)pMsg;

	CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwObjectID );
	if( !pPlayer )	return;

	pPlayer->SetGuildUnionMarkIdx( pmsg->dwMarkIdx );
	GUILDUNIONMARKMGR->SaveGuildUnionMark( g_nServerSetNum, pmsg->dwGuildUnionIdx, pmsg->dwMarkIdx, pmsg->imgData );

	// chat msg
	if( HERO->GetID() != pmsg->dwObjectID )	return;
	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1113) );
}

void CGuildUnion::MsgGuildUnionCreateNack( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	switch( pmsg->dwData1 )
	{
	case eGU_Not_ValidName:			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1136) );	break;
	case eGU_Not_InGuild:			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1127) );	break;
	case eGU_Not_GuildMaster:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1128) );	break;
	case eGU_Not_Level:				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1101) );	break;
	case eGU_Aleady_InGuildUnion:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1102) );	break;
	case eGU_Time:
		{
			stTIME time;
			time.value = pmsg->dwData2;
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1129), time.GetYear()+2000, time.GetMonth(),
				time.GetDay(), time.GetHour(), time.GetMinute() );
		}
		break;
	case eGU_Not_ValidMap:			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1202) );	break;
	default:						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1100) );	break;
	}
}

void CGuildUnion::MsgGuildUnionDestroyNack( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	switch( pmsg->dwData1 )
	{
	case eGU_Not_InGuild:			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1127) );	break;
	case eGU_Not_GuildMaster:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1128) );	break;
	case eGU_Not_InGuildUnion:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1103) );	break;
	case eGU_Not_GuildUnionMaster:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1100) );	break;
	case eGU_Not_ValidMap:			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1202) );	break;
	default:						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1100) );	break;
	}
}

void CGuildUnion::MsgGuildUnionInviteNack( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	switch( pmsg->dwData1 )
	{
	case eGU_Not_InGuild:			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1127) );	break;
	case eGU_Not_GuildMaster:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1128) );	break;
	case eGU_Not_InGuildUnion:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1103) );	break;
	case eGU_Not_GuildUnionMaster:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1100) );	break;
	case eGU_Is_Full:				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1130) );	break;
	case eGU_Other_Not_InGuild:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1131) );	break;
	case eGU_Other_Not_GuildMaster:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1132) );	break;
	case eGU_Other_Aleady_InGuildUnion:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1133) );	break;
	case eGU_Other_Time:
		{
			stTIME time;
			time.value = pmsg->dwData2;
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1134), time.GetYear()+2000, time.GetMonth(),
				time.GetDay(), time.GetHour(), time.GetMinute() );
		}
		break;
	case eGU_Is_GuildFieldWar:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1142) );	break;
	case eGU_Not_ValidMap:			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1202) );	break;
	default:						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1100) );	break;
	}
}

void CGuildUnion::MsgGuildUnionRemoveNack( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	switch( pmsg->dwData1 )
	{
	case eGU_Not_InGuild:			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1127) );	break;
	case eGU_Not_GuildMaster:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1128) );	break;
	case eGU_Not_InGuildUnion:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1103) );	break;
	case eGU_Not_GuildUnionMaster:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1100) );	break;
	case eGU_Other_Not_InGuild:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1131) );	break;
	case eGU_Other_Not_InGuildUnion:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1135) );	break;
	case eGU_Not_ValidMap:			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1202) );	break;
	default:						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1100) );	break;
	}
}

void CGuildUnion::MsgGuildUnionSecedeNack( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	switch( pmsg->dwData1 )
	{
	case eGU_Not_InGuildUnion:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1103) );	break;
	case eGU_Not_InGuild:			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1127) );	break;
	case eGU_Not_GuildMaster:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1128) );	break;
	case eGU_Not_ValidMap:			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1202) );	break;
	default:						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1100) );	break;
	}
}

void CGuildUnion::MsgGuildUnionMarkRegistNack( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
	switch( pmsg->dwData1 )
	{
	case eGU_Not_InGuildUnion:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1103) );	break;
	case eGU_Not_InGuild:			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1127) );	break;
	case eGU_Not_GuildMaster:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1128) );	break;
	case eGU_Not_GuildUnionMaster:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1100) );	break;
	case eGU_Not_ValidMap:			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1202) );	break;
	default:						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1104) );	break;
	}
}

void CGuildUnion::SendCreateGuildUnion( char* pName )
{
	if( !CheckGuildUnionCondition( 0 ) )	return;

	MSG_NAME_DWORD Msg;
	Msg.Category = MP_GUILD_UNION;
	Msg.Protocol = MP_GUILD_UNION_CREATE_SYN;
	Msg.dwObjectID = HEROID;
	Msg.dwData = HERO->GetGuildIdx();
	strncpy( Msg.Name, pName, MAX_GUILD_NAME+1 );

	NETWORK->Send( &Msg, sizeof(Msg) );

	GAMEIN->GetGuildUnionCreateDlg()->SetActive( FALSE );
}

void CGuildUnion::SendDestroyGuildUnion()
{
	if( !CheckGuildUnionCondition( 1 ) )	return;

	MSG_DWORD Msg;
	Msg.Category = MP_GUILD_UNION;
	Msg.Protocol = MP_GUILD_UNION_DESTROY_SYN;
	Msg.dwObjectID = HEROID;
	Msg.dwData = GetGuildUnionIdx();

	NETWORK->Send( &Msg, sizeof(Msg) );
}

void CGuildUnion::SendInviteGuildUnion( CPlayer* pPlayer )
{	
	if( !pPlayer->GetGuildIdx() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1127) );
		return;
	}
	if( GUILDFIELDWAR->IsEnemyGuild( pPlayer->GetGuildName() ) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1139) );
		return;
	}
	if( !CheckGuildUnionCondition( 2 ) )	return;	

	MSG_DWORD2 Msg;
	Msg.Category = MP_GUILD_UNION;
	Msg.Protocol = MP_GUILD_UNION_INVITE;
	Msg.dwObjectID = HEROID;
	Msg.dwData1 = pPlayer->GetID();
	Msg.dwData2 = m_dwGuildUnionIdx;

	NETWORK->Send( &Msg, sizeof(Msg) );
}

void CGuildUnion::SendInviteAcceptDeny( BYTE Protocol )
{
	MSG_DWORD2 Msg;
	Msg.Category = MP_GUILD_UNION;
	Msg.Protocol = Protocol;
	Msg.dwObjectID = HEROID;
	Msg.dwData1 = m_dwTempPlayerId;
	Msg.dwData2 = m_dwTempGuildUnionIdx;

	NETWORK->Send( &Msg, sizeof(Msg) );
}

void CGuildUnion::SendRemoveGuildUnion()
{
	if( !m_dwTempGuildIdx )	return;
	if( !CheckGuildUnionCondition( 3 ) )	return;

	GAMEIN->GetGFWarInfoDlg()->SetActive( FALSE );

	MSG_DWORD2 Msg;
	Msg.Category = MP_GUILD_UNION;
	Msg.Protocol = MP_GUILD_UNION_REMOVE_SYN;
	Msg.dwObjectID = HEROID;
	Msg.dwData1 = m_dwGuildUnionIdx;
	Msg.dwData2 = m_dwTempGuildIdx;
	
	NETWORK->Send( &Msg, sizeof(Msg) );
}

void CGuildUnion::SendSecedeGuildUnion()
{
	if( !CheckGuildUnionCondition( 4 ) )	return;

	MSG_DWORD2 Msg;
	Msg.Category = MP_GUILD_UNION;
	Msg.Protocol = MP_GUILD_UNION_SECEDE_SYN;
	Msg.dwObjectID = HEROID;
	Msg.dwData1 = m_dwGuildUnionIdx;
	Msg.dwData2 = HERO->GetGuildIdx();

	NETWORK->Send( &Msg, sizeof(Msg) );
}

BOOL CGuildUnion::CheckGuildUnionCondition( int nCondition )
{
	if( SIEGEMGR->GetSiegeWarMapNum() == MAP->GetMapNum() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1202) );
		return FALSE;
	}

	CGuild* pGuild = GUILDMGR->GetGuild();
	if( !pGuild )	return FALSE;

	switch( nCondition )
	{
	case 0:		// create
		{			
			if( HERO->GetGuildMemberRank() != GUILD_MASTER )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1100) );
				return FALSE;
			}			
			if( pGuild->GetLevel() < GUILD_5LEVEL )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1101) );
				return FALSE;
			}
			if( m_dwGuildUnionIdx )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1102) );
				return FALSE;
			}
		}
		break;
	case 1:		// destroy
		{
			if( HERO->GetGuildMemberRank() != GUILD_MASTER || m_GuildInfo[0].dwGuildIdx != HERO->GetGuildIdx() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1100) );
				return FALSE;
			}
			if( m_dwGuildUnionIdx == 0 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1103) );
				return FALSE;
			}
		}
		break;
	case 2:		// invite
		{
			if( HERO->GetGuildMemberRank() != GUILD_MASTER || m_GuildInfo[0].dwGuildIdx != HERO->GetGuildIdx() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1100) );
				return FALSE;
			}
			if( m_dwGuildUnionIdx == 0 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1103) );
				return FALSE;
			}			
			if( !IsEmptyPosition() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1115) );
				return FALSE;
			}
		}
		break;
	case 3:		// remove
		{
			if( HERO->GetGuildMemberRank() != GUILD_MASTER || m_GuildInfo[0].dwGuildIdx != HERO->GetGuildIdx() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1100) );
				return FALSE;
			}
			if( m_dwGuildUnionIdx == 0 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1103) );
				return FALSE;
			}
		}
		break;	
	case 4:		// secede
		{
			if( HERO->GetGuildMemberRank() != GUILD_MASTER || m_GuildInfo[0].dwGuildIdx == HERO->GetGuildIdx() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1100) );
				return FALSE;
			}
			if( m_dwGuildUnionIdx == 0 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1103) );
				return FALSE;
			}
		}
		break;
	case 5:		// mark
		{
			if( HERO->GetGuildMemberRank() != GUILD_MASTER || m_GuildInfo[0].dwGuildIdx != HERO->GetGuildIdx() )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1100) );
				return FALSE;
			}
			if( m_dwGuildUnionIdx == 0 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1103) );
				return FALSE;
			}
		}
		break;
	}
	return TRUE;
}

BOOL CGuildUnion::IsEmptyPosition()
{
	for( int i = 0; i < MAX_GUILD_UNION_NUM; ++i )
	{
		if( m_GuildInfo[i].dwGuildIdx == 0 )
			return TRUE;
	}
	return FALSE;
}

BOOL CGuildUnion::IsUnionGuild( char* pGuildName )
{
	for( int i = 0; i < MAX_GUILD_UNION_NUM; ++i )
	{
		if( strcmp( pGuildName, m_GuildInfo[i].sGuildName ) == 0 )
			return TRUE;
	}
	return FALSE;
}

//SW080515 함수 추가.
BOOL CGuildUnion::IsSameUnion( CPlayer* pPlayerA, CPlayer* pPlayerB )
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
