// GuildFieldWar.cpp: implementation of the CGuildFieldWar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildFieldWar.h"
#include "WindowIDEnum.h"
#include "cMsgBox.h"
#include "./Interface/cWindowManager.h"
#include "ObjectManager.h"
#include "ChatManager.h"
#include "GameIn.h"
#include "GuildManager.h"
#include "GuildFieldWarDialog.h"
#include "CharacterDialog.h"
#include "GuildDialog.h"
#include "GuildLevelupDialog.h"
#include "GuildRankDialog.h"
#include "MHMap.h"
#include "GuildUnion.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CGuildFieldWar)

CGuildFieldWar::CGuildFieldWar()
{
	m_EnemyGuildInfoTable.Initialize( 20 );
	m_pTempGuildInfo = NULL;
	m_dwTempEnemyGuildIdx = 0;
	m_dwTempGFWMoney = 0;

	m_dwVictoryCount = m_dwDrawCount = m_dwLooseCount = 0;
}

CGuildFieldWar::~CGuildFieldWar()
{
	Release();
}

void CGuildFieldWar::Init()
{
	Release();
	m_pTempGuildInfo = NULL;
	m_dwTempEnemyGuildIdx = 0;
	m_dwTempGFWMoney = 0;

	m_dwVictoryCount = m_dwDrawCount = m_dwLooseCount = 0;

	if( GAMEIN->GetGuildWarInfoDlg()->IsActive() )
		GAMEIN->GetGuildWarInfoDlg()->SetActive( FALSE );
}

void CGuildFieldWar::Release()
{
	GUILDINFO* pGuildInfo = NULL;
	m_EnemyGuildInfoTable.SetPositionHead();
	while( pGuildInfo = m_EnemyGuildInfoTable.GetData() )
		delete pGuildInfo;
	m_EnemyGuildInfoTable.RemoveAll();
}

BOOL CGuildFieldWar::IsEnemy( CPlayer* pTarget )
{
	if( MAP->IsVillage() )	return FALSE;
//	if( HERO && HERO->GetGuildIdx() == 0 )	return FALSE;
	DWORD dwGuildIdx = pTarget->GetGuildIdx();
	if( dwGuildIdx == 0 )	return FALSE;
	if( !m_EnemyGuildInfoTable.GetData( dwGuildIdx ) )	return FALSE;
	
	return TRUE;
}

BOOL CGuildFieldWar::IsEnemyGuild( char* pGuildName )
{
	GUILDINFO* pInfo = NULL;
	m_EnemyGuildInfoTable.SetPositionHead();
	while( pInfo = m_EnemyGuildInfoTable.GetData() )
	{
		if( strcmp( pGuildName, pInfo->GuildName ) == 0 )
			return TRUE;
	}
	return FALSE;
}

void CGuildFieldWar::RegistEnemyGuild( GUILDINFO* pGuildInfo )
{
	if( m_EnemyGuildInfoTable.GetData( pGuildInfo->GuildIdx ) )
		return;

	GUILDINFO* pInfo = new GUILDINFO;
	*pInfo = *pGuildInfo;
	m_EnemyGuildInfoTable.Add( pInfo, pInfo->GuildIdx );

	OBJECTMGR->ApplyOverInfoOptionToAll();
}

void CGuildFieldWar::UnRegistEnemyGuild( DWORD dwEnemyGuildIdx )
{
	GUILDINFO* pInfo = m_EnemyGuildInfoTable.GetData( dwEnemyGuildIdx );
	if( !pInfo )	return;

	delete pInfo;
	m_EnemyGuildInfoTable.Remove( dwEnemyGuildIdx );

	OBJECTMGR->ApplyOverInfoOptionToAll();
}

GUILDINFO* CGuildFieldWar::GetEnemyGuildInfoFromListIdx( int nIdx )
{
	if( nIdx == -1 )	return NULL;

	int count = 0;
	m_EnemyGuildInfoTable.SetPositionHead();
	while( m_pTempGuildInfo = m_EnemyGuildInfoTable.GetData() )
	{
		if( count == nIdx )
		{
			return m_pTempGuildInfo;
		}
		++count;
	}

	return NULL;
}

BOOL CGuildFieldWar::CheckGuildFieldWarCondition( DWORD dwKind )
{
	switch( dwKind )
	{
	case 0:		// declare
		{			
			if( GUILDMGR->GetGuildLevel() < 3 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 848 ) );
				return FALSE;
			}
			if( GUILDMGR->GetMemberNum() < 5 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 849 ) );
				return FALSE;
			}							
		}
		break;
	case 1:
	case 2:		// suggest
		{
			if( m_EnemyGuildInfoTable.GetDataNum() < 1 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 850 ) );
				return FALSE;
			}
		}
		break;
	case 3:		// breakup guild
		{
			if( m_EnemyGuildInfoTable.GetDataNum() > 0 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 854 ) );
				return FALSE;
			}
		}
		break;
	case 4:		// checkmoney
		{
			if( m_dwTempGFWMoney )
			{
				if( HERO->GetMoney() < m_dwTempGFWMoney )
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 815 ) );
					return FALSE;
				}
			}
		}
		break;
	}
	
	return TRUE;
}

void CGuildFieldWar::GetVictory( char* temp )
{
	sprintf( temp, "%d - %d - %d", m_dwVictoryCount, m_dwDrawCount, m_dwLooseCount );
}

void CGuildFieldWar::NetworkMsgParse( BYTE Protocol, void* pMsg )
{
	switch( Protocol )
	{
	case MP_GUILD_FIELDWAR_DECLARE:
		{
			MSG_GUILDFIELDWAR_INFO* pmsg = (MSG_GUILDFIELDWAR_INFO*)pMsg;

			m_dwTempEnemyGuildIdx = pmsg->EnemyGuildInfo[0].GuildIdx;
			m_dwTempGFWMoney = pmsg->EnemyGuildInfo[0].MarkName;
			GAMEIN->GetGFWarResultDlg()->ShowDeclare( &pmsg->EnemyGuildInfo[0] );			
		}
		break;
	case MP_GUILD_FIELDWAR_NACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;

			switch( pmsg->bData )
			{
			case 1:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 842 ) );	break;	// sender is not master
			case 2:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 843 ) );	break;	// is not guild
			case 3:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 844 ) );	break;	// same guild
			case 4:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 845 ) );	break;	// not login
			case 5:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 860 ) );	break;	// other guild master money not enough
			case 6:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 840 ) );	break;	// self money not enough			
			}

			GAMEIN->GetGFWarDeclareDlg()->SetActive( FALSE );			
		}
		break;
	case MP_GUILD_FIELDWAR_DECLARE_NACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			switch( pmsg->dwData2 )
			{
			case 0:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 848 ) );	break;	// guild level is wrong
			case 1:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 856 ) );	break;	// other guild level is wrong
			case 2:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 849 ) );	break;	// guild membernum is wrong
			case 3:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 857 ) );	break;	// other guild membernum is wrong
			case 4:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 858 ) );	break;	// other guild deny
			case 5:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 859 ) );	break;	// other guild war end
			}

			GAMEIN->GetGFWarDeclareDlg()->SetActive( FALSE );
		}
		break;
	case MP_GUILD_FIELDWAR_WAIT:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;

			switch( pmsg->bData )
			{
			case MP_GUILD_FIELDWAR_DECLARE:
				{
					GAMEIN->GetGFWarDeclareDlg()->SetActive( FALSE );
				}
				break;
			case MP_GUILD_FIELDWAR_SUGGESTEND:
				break;
			}			

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 841 ) );
		}
		break;
	case MP_GUILD_FIELDWAR_DECLARE_ACCEPT:
		{
		}
		break;
	case MP_GUILD_FIELDWAR_DECLARE_DENY:
		{
			GAMEIN->GetGFWarDeclareDlg()->SetActive( FALSE );

			WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_CONFIRM, MBT_OK, CHATMGR->GetChatMsg( 816 ) );
		}
		break;
	
	case MP_GUILD_FIELDWAR_START:		// guildfieldwar start
		{
			MSG_GUILDFIELDWAR_INFO* pmsg = (MSG_GUILDFIELDWAR_INFO*)pMsg;

			// regist
			RegistEnemyGuild( &pmsg->EnemyGuildInfo[0] );

			// interface
			GAMEIN->GetGFWarInfoDlg()->AddGuildInfoToList();
			GAMEIN->GetGuildWarInfoDlg()->AddListData();

			// msgbox
			if( pmsg->dwEnemyGuildNum )
				WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_CONFIRM, MBT_OK, CHATMGR->GetChatMsg( 818 ), pmsg->EnemyGuildInfo[0].GuildName, pmsg->EnemyGuildInfo[0].MarkName );
			else
				WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_CONFIRM, MBT_OK, CHATMGR->GetChatMsg( 817 ), pmsg->EnemyGuildInfo[0].GuildName );
		}
		break;
	case MP_GUILD_FIELDWAR_END:			// guildfieldwar end
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			if( pmsg->dwData1 == 0 )		// lose
			{
				WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_CONFIRM, MBT_OK, CHATMGR->GetChatMsg( 821 ) );

//				GAMEIN->GetGFWarResultDlg()->ShowResult( 0 );

				++m_dwLooseCount;
			}
			else if( pmsg->dwData1 == 1 )	// win
			{
				WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_CONFIRM, MBT_OK, CHATMGR->GetChatMsg( 819 ), GetEnemyGuildInfo(pmsg->dwData2)->GuildName );

				++m_dwVictoryCount;
			}
			else if( pmsg->dwData1 == 2 )	// draw
			{
				WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_CONFIRM, MBT_OK, CHATMGR->GetChatMsg( 833 ), GetEnemyGuildInfo(pmsg->dwData2)->GuildName );

				++m_dwDrawCount;
			}

			// unregist
			UnRegistEnemyGuild( pmsg->dwData2 );

			// interface
			GAMEIN->GetGFWarInfoDlg()->AddGuildInfoToList();
			GAMEIN->GetGuildWarInfoDlg()->AddListData();
		}
		break;
	case MP_GUILD_FIELDWAR_SURRENDER:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			if( pmsg->dwData1 == 0 )		// surrender
			{
				WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_CONFIRM, MBT_OK, CHATMGR->GetChatMsg( 828 ) );

//				GAMEIN->GetGFWarResultDlg()->ShowResult( 1 );

				++m_dwLooseCount;
			}
			else if( pmsg->dwData1 == 1 )	// win
			{
				WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_CONFIRM, MBT_OK, CHATMGR->GetChatMsg( 826 ), GetEnemyGuildInfo(pmsg->dwData2)->GuildName );

				++m_dwVictoryCount;
			}

			// unregist
			UnRegistEnemyGuild( pmsg->dwData2 );

			// interface
			GAMEIN->GetGFWarInfoDlg()->AddGuildInfoToList();
			GAMEIN->GetGuildWarInfoDlg()->AddListData();
		}
		break;
	case MP_GUILD_FIELDWAR_PROC:
		{
			MSG_GUILDFIELDWAR_INFO* pmsg = (MSG_GUILDFIELDWAR_INFO*)pMsg;

			for( DWORD i = 0; i < pmsg->dwEnemyGuildNum; ++i )
			{
				RegistEnemyGuild( &pmsg->EnemyGuildInfo[i] );

				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 846 ), pmsg->EnemyGuildInfo[i].GuildName );
			}

			// interface
			GAMEIN->GetGFWarInfoDlg()->AddGuildInfoToList();
			GAMEIN->GetGuildWarInfoDlg()->AddListData();
		}
		break;	
	case MP_GUILD_FIELDWAR_SUGGESTEND:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			GUILDINFO* pInfo = GetEnemyGuildInfo( pmsg->dwData );
			if( pInfo )
			{
				m_dwTempEnemyGuildIdx = pmsg->dwData;
				WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_SUGGEST_ACCEPTDENY, MBT_YESNO, CHATMGR->GetChatMsg( 832 ), pInfo->GuildName );
			}
		}
		break;
	case MP_GUILD_FIELDWAR_SUGGESTEND_NACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, "Declare or Stop Error!!" );
		}
		break;
	case MP_GUILD_FIELDWAR_SUGGESTEND_DENY:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			GUILDINFO* pInfo = GetEnemyGuildInfo( pmsg->dwData );
			if( pInfo )
			{
				WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_CONFIRM, MBT_OK, CHATMGR->GetChatMsg( 834 ), pInfo->GuildName );
			}
		}
		break;
	case MP_GUILD_FIELDWAR_LEVELDOWN:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
			
			GUILDMGR->SetGuildLevel( pmsg->bData );
			GAMEIN->GetCharacterDialog()->RefreshGuildInfo();
			GAMEIN->GetGuildDlg()->RefreshGuildInfo( GUILDMGR->GetGuild() );
			GAMEIN->GetGuildLevelUpDlg()->SetLevel( pmsg->bData );
			GAMEIN->GetGuildRankDlg()->ShowGuildRankMode( pmsg->bData );
		}
		break;
	case MP_GUILD_FIELDWAR_RECORD:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			m_dwVictoryCount = pmsg->dwData1;
			m_dwDrawCount = pmsg->dwData2;
			m_dwLooseCount = pmsg->dwData3;
		}
		break;
	case MP_GUILD_FIELDWAR_ADDMONEY:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 839 ), pmsg->dwData );
		}
		break;
	case MP_GUILD_FIELDWAR_REMOVEMONEY:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 838 ), pmsg->dwData );
		}
		break;
	case MP_GUILD_FIELDWAR_RESULT_TOALLUSER:
		{
			MSG_NAME2* pmsg = (MSG_NAME2*)pMsg;
			switch( pmsg->dwObjectID )
			{
			case 0:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 926 ), pmsg->str1, pmsg->str2, pmsg->str1 );	break;
			case 1:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 927 ), pmsg->str1, pmsg->str2 );	break;
			case 2:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 928 ), pmsg->str1, pmsg->str2, pmsg->str2, pmsg->str1 );	break;
			}
		}
		break;
	}
}

void CGuildFieldWar::SendGuildFieldWarDeclare( char* pName, char* pMoney )
{
    //if( MAP->GetMapNum() == RUNNINGMAP || MAP->GetMapNum() == PKEVENTMAP || MAP->CheckMapKindIs(eSurvival) )
	if( MAP->IsMapSame(eRunningMap) || MAP->GetMapNum() == PKEVENTMAP || MAP->IsMapKind(eSurvivalMap) )
	{
		return;
	}
	if( strcmp( pName, "" ) == 0 || strlen(pName) > MAX_GUILD_NAME )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 843 ) );
		return;
	}
	if( strcmp( GUILDMGR->GetGuildName(), pName ) == 0 )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 844 ) );
		return;
	}
	if( IsEnemyGuild( pName ) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 855 ) );
		return;
	}
	if( GUILDUNION->IsUnionGuild( pName ) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1138 ) );
		return;		
	}

	DWORD dwMoney = (DWORD)atoi( pMoney );
	if( GAMEIN->GetGFWarDeclareDlg()->IsChecked() )
	{		
		if( dwMoney < 100000 )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 882 ) );
			return;
		}
		if( HERO->GetMoney() < dwMoney )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 840 ) );
			return;
		}
	}

	MSG_NAME_DWORD Msg;
	Msg.Category = MP_GUILD_FIELDWAR;
	Msg.Protocol = MP_GUILD_FIELDWAR_DECLARE;
	Msg.dwObjectID = HEROID;
	SafeStrCpy( Msg.Name, pName, MAX_NAME_LENGTH+1 );
	Msg.dwData = dwMoney;

	NETWORK->Send( &Msg, sizeof(Msg) );
}

void CGuildFieldWar::SendGuildFieldWarDeclareAcceptDeny( BYTE Protocol )
{
	if( Protocol == MP_GUILD_FIELDWAR_DECLARE_ACCEPT )
	{
		if( !CheckGuildFieldWarCondition( 4 ) )
			return;
	}

	GAMEIN->GetGFWarResultDlg()->SetActive( FALSE );

	MSG_DWORD2 Msg;
	Msg.Category = MP_GUILD_FIELDWAR;
	Msg.Protocol = Protocol;
	Msg.dwObjectID = HEROID;
	Msg.dwData1 = m_dwTempEnemyGuildIdx;
	Msg.dwData2 = m_dwTempGFWMoney;

	NETWORK->Send( (MSGROOT*)&Msg, sizeof(Msg) );
}

void CGuildFieldWar::SendGuildFieldWarSurrend()
{
	GAMEIN->GetGFWarInfoDlg()->SetActive( FALSE );

	if( m_pTempGuildInfo == NULL )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 822 ) );
		return;
	}
	
	MSG_DWORD Msg;
	Msg.Category = MP_GUILD_FIELDWAR;
	Msg.Protocol = MP_GUILD_FIELDWAR_SURRENDER;
	Msg.dwObjectID = HEROID;
	Msg.dwData = m_pTempGuildInfo->GuildIdx;

	NETWORK->Send( &Msg, sizeof(Msg) );	
}

void CGuildFieldWar::SendGuildFieldWarSuggest()
{
	GAMEIN->GetGFWarInfoDlg()->SetActive( FALSE );

	if( m_pTempGuildInfo == NULL )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 829 ) );
		return;
	}

	MSG_NAME_DWORD Msg;
	Msg.Category = MP_GUILD_FIELDWAR;
	Msg.Protocol = MP_GUILD_FIELDWAR_SUGGESTEND;
	Msg.dwObjectID = HEROID;
	Msg.dwData = 0;
	SafeStrCpy( Msg.Name, m_pTempGuildInfo->GuildName, MAX_NAME_LENGTH+1 );

	NETWORK->Send( &Msg, sizeof(Msg) );	
}

void CGuildFieldWar::SendGuildFieldWarSuggestAcceptDeny( BYTE Protocol )
{
	MSG_DWORD Msg;
	Msg.Category = MP_GUILD_FIELDWAR;
	Msg.Protocol = Protocol;
	Msg.dwObjectID = HEROID;
	Msg.dwData = m_dwTempEnemyGuildIdx;

	NETWORK->Send( (MSGROOT*)&Msg, sizeof(Msg) );
}
	
void CGuildFieldWar::TestSuggestAcceptDeny()
{
	WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_SUGGEST_ACCEPTDENY, MBT_YESNO, CHATMGR->GetChatMsg( 813 ), "" );
}
