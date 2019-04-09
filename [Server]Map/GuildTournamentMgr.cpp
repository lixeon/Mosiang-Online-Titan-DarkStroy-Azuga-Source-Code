// GuildTournamentMgr.cpp: implementation of the CGuildTournamentMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildTournamentMgr.h"
#include "Guild.h"
#include "MHFile.h"
#include "UserTable.h"
#include "Player.h"
#include "GuildManager.h"
#include "BattleSystem_Server.h"
#include "Network.h"
#include "CharMove.h"
#include "MapDBMsgParser.h"
#include "GuildMark.h"
#include "PackedData.h"
#include "QuestManager.h"
#include "..\[CC]Header\GameResourceManager.h"


extern int g_nServerSetNum;
DWORD gRanking[eGTT_MAX] = 
{
	32,			// 32	16	eGTFight_32
	16,			// 16	8	eGTFight_16
	8,			// 8	4	eGTFight_8
	4,			// 4	2	eGTFight_4
	2,			// 3,4	2	eGTFignt_3_4
	1,			// 1,2	2	eGTFight_1_2
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuildTournamentMgr::CGuildTournamentMgr()
{	
}

CGuildTournamentMgr::~CGuildTournamentMgr()
{
}


void CGuildTournamentMgr::Init()
{
	CMHFile file;
//	if(!file.Init(".\\Resource\\GuildTournamentInfo.bin", "rb"))
	if(!file.Init("./Resource/Server/GuildTournamentInfo.bin", "rb"))
	{
		ASSERT(0);
		return;
	}

	if( strcmp( file.GetString(), "MAX_TOURNAMENT_GROUP") == 0 )
		m_MaxGroup = file.GetDword();
	if( strcmp( file.GetString(), "TIME_START") == 0 )
	{
		DWORD year = file.GetDword();
		DWORD month = file.GetDword();
		DWORD day = file.GetDword();
		m_FirstStartTime.SetTime( year, month, day, 0, 0, 0 );
	}
	if( strcmp( file.GetString(), "TIME_REGIST" ) == 0 )
	{
		for( int i=0; i<2; ++i )
		{
			m_RegistTime[i].Day = file.GetWord();
			m_RegistTime[i].Hour = file.GetWord();
			m_RegistTime[i].Minute = file.GetWord();
		}		
	}
	if( strcmp( file.GetString(), "TIME_ENTRANCE") == 0 )
		m_EntranceTime = file.GetDword();
	if( strcmp( file.GetString(), "TIME_PROCESS") == 0 )
		m_FightTime = file.GetDword();
	if( strcmp( file.GetString(), "TIME_LEAVE") == 0 )
		m_LeaveTime = file.GetDword();


	DWORD server = 0;

	while( 1 )
	{
		char buf[32] = { 0, };
		strcpy( buf, file.GetString() );

		if( strcmp( buf, "#SERVER" ) == 0 )		
			server = file.GetDword();
		else if( strcmp( buf, "GROUP" ) == 0 )
		{
			DWORD group = file.GetDword();
			m_TournamentTime[server-1][group].Day = file.GetWord();
			m_TournamentTime[server-1][group].Hour = file.GetWord();
			m_TournamentTime[server-1][group].Minute = file.GetWord();
		}
		else if( strcmp( buf, "END" ) == 0 )
			break;
	}

	file.Release();

	m_TournamentCancelList.Initialize( 100 );
	//m_CurTournament = eGTFight_32;
	m_RegistedGuildCount = 0;
	m_NotifyEntranceTime = 0;
	//m_TournamentCount = 0;
	memset( &m_GuildInfoList, 0, sizeof(GTInfo)*MAXGUILD_INTOURNAMENT );
	memset( m_PositionTable, -1, sizeof(int)*2*MAXGUILD_INTOURNAMENT/2 );
	memset( m_BattleInfo, 0, sizeof(BATTLE_INFO_GTOURNAMENT)*MAXGUILD_INTOURNAMENT/2 );
	memset( m_EventBattleInfo, 0, sizeof(BATTLE_INFO_GTOURNAMENT)*MAX_EVENTGT_COUNT );
	m_BattleCount = 0;
	//m_Inited = FALSE;

	DWORD BCount = MAXGUILD_INTOURNAMENT;
	for(int i=0; i<eGTT_MAX; ++i)
	{
		if( i < eGTFight_4 )			BCount /= 2;
		else if( i == eGTFight_4 )		BCount = 2;
		else if( i >= eGTFight_3_4 )	BCount = 1;

		BattleCount[i] = BCount;
	}

	m_bEvent = FALSE;
	m_EventBattleCount = 0;

	SYSTEMTIME st;
	GetLocalTime(&st);
	m_wCurDay = st.wDayOfWeek;

	m_dwCurFlag = GAMERESRCMNGR->GetFlagFromDate(eGTFlg, m_wCurDay);
}


void CGuildTournamentMgr::SetTournamentInfo( DWORD Tournament, DWORD CurTournament, DWORD WaitTime )
{
	m_TournamentCount = Tournament;
	m_CurTournament = CurTournament;
	if( Tournament == 1 )
	{
		m_WaitTime = m_FirstStartTime;
		GuildTournamentInfoUpdate( m_TournamentCount, m_CurTournament, m_WaitTime.value );
	}
	else						m_WaitTime.value = WaitTime;
	m_Inited = TRUE;

	GuildTournamentCancelIdxLoad( m_TournamentCount );

	stTIME ct;
	ct.value = GetCurTime();
	if( m_WaitTime.value && m_WaitTime > ct )
	{
		m_TournamentState = eGTState_BeforeRegist;
		return;
	}

	if( m_CurTournament > 0 && m_CurTournament != eGTFight_End )		goto BeforeEntrance;
	else if( m_CurTournament == 0 )
	{
		SYSTEMTIME st;
		GetLocalTime( &st );

		if( st.wDayOfWeek > m_RegistTime[1].Day )						goto BeforeEntrance;
		else if( st.wDayOfWeek == m_RegistTime[1].Day )
		{
			if( st.wHour > m_RegistTime[1].Hour )						goto BeforeEntrance;
			else if( st.wHour == m_RegistTime[1].Hour )
			{
				if( st.wMinute > m_RegistTime[1].Minute )				goto BeforeEntrance;
			}
		}
		m_TournamentState = eGTState_BeforeRegist;
		return;
	}
	else if( m_CurTournament == eGTFight_End )
	{
		m_TournamentState = eGTState_BeforeRegist;
		return;
	}

BeforeEntrance:
	m_TournamentState = eGTState_BeforeEntrance;
}


void CGuildTournamentMgr::SetGuildInfo( GTDBLoadInfo* pGuildInfo, DWORD Count )
{
	for(DWORD i=0; i<Count; ++i)
	{
		CGuild* pGuild = GUILDMGR->GetGuild( pGuildInfo[i].GuildIdx );
		if( !pGuild )			continue;

		m_GuildInfoList[pGuildInfo[i].Position].pGuild = pGuild;
		m_GuildInfoList[pGuildInfo[i].Position].Ranking = pGuildInfo[i].Ranking;
		m_GuildInfoList[pGuildInfo[i].Position].Position = pGuildInfo[i].Position;
		m_GuildInfoList[pGuildInfo[i].Position].ProcessTournament = pGuildInfo[i].ProcessTournament;
		m_PositionTable[pGuildInfo[i].Position%2][pGuildInfo[i].Position/2] = pGuildInfo[i].Position;
		++m_RegistedGuildCount;
	}
}


void CGuildTournamentMgr::AddCancelGuild( DWORD GuildIdx )
{
	CGuild* pGuild = GUILDMGR->GetGuild( GuildIdx );
	if( pGuild )
		m_TournamentCancelList.Add( pGuild, GuildIdx );
}

void CGuildTournamentMgr::Reset()
{
	m_CurTournament = eGTFight_32;
	m_TournamentState = eGTState_BeforeRegist;
	m_RegistedGuildCount = 0;
	memset( &m_GuildInfoList, 0, sizeof(GTInfo)*MAXGUILD_INTOURNAMENT );
	memset( m_PositionTable, -1, sizeof(int)*2*MAXGUILD_INTOURNAMENT/2 );
	memset( m_BattleInfo, 0, sizeof(BATTLE_INFO_GTOURNAMENT)*MAXGUILD_INTOURNAMENT/2 );
	m_BattleCount = 0;
	m_RemainTime = 0;
}


void CGuildTournamentMgr::Process()
{
	// 토너먼트 시작했는지 체크추가
	//if( g_pServerSystem->GetMapNum() != GTMAPNUM )		return;
	if( FALSE == g_pServerSystem->GetMap()->IsMapKind(eTournament) )		return;
	
	if( !m_Inited )										return;
	if( m_bEvent )
	{
		for(int i=0; i<MAX_EVENTGT_COUNT; ++i)
		{
			if( m_EventBattleInfo[i].BattleID )
			{
				if( m_EventBattleInfo[i].m_dwEntranceTime >= gTickTime )
					m_EventBattleInfo[i].m_dwEntranceTime -= gTickTime;
				else
					m_EventBattleInfo[i].m_dwEntranceTime = 0;
				
				if( m_EventBattleInfo[i].m_dwEntranceTime == 0 )
				{
					CBattle_GTournament* pBattle = (CBattle_GTournament*)BATTLESYSTEM->GetBattle( m_EventBattleInfo[i].BattleID );				
					if( pBattle && pBattle->GetBattleState() == eBATTLE_STATE_READY )
						pBattle->StartBattle();
				}
			}
		}
		return;
	}

	SYSTEMTIME st;
	GetLocalTime( &st );

	stTIME ct;
	ct.value = GetCurTime();
	if( m_WaitTime.value && m_WaitTime > ct )					return;

	// 등록시간인지 체크
	switch( m_TournamentState )
	{
	case eGTState_BeforeRegist:
		{
			if( st.wDayOfWeek < m_RegistTime[0].Day	)			return;			// 등록전 일
			if( st.wDayOfWeek == m_RegistTime[0].Day )		
			if( st.wHour < m_RegistTime[0].Hour )				return;			// 등록전 시간
			if( st.wHour == m_RegistTime[0].Hour )
			if( st.wMinute < m_RegistTime[0].Minute )			return;			// 등록전 분
			
			if( m_CurTournament == eGTFight_End )
			{
				++m_TournamentCount;
				m_CurTournament = eGTFight_32;
				memset( &m_GuildInfoList, 0, sizeof(GTInfo)*MAXGUILD_INTOURNAMENT );
				memset( m_PositionTable, -1, sizeof(int)*2*MAXGUILD_INTOURNAMENT/2 );
				memset( m_BattleInfo, 0, sizeof(BATTLE_INFO_GTOURNAMENT)*MAXGUILD_INTOURNAMENT/2 );
				m_BattleCount = 0;
				m_RemainTime = 0;
				m_RegistedGuildCount = 0;
				m_NotifyEntranceTime = 0;
				m_TournamentCancelList.RemoveAll();
				
				// To DB
				GuildTournamentInfoUpdate( m_TournamentCount, m_CurTournament, m_WaitTime.value );
			}
			
			m_TournamentState = eGTState_Regist;

			// Broadcast To Mapserver
			MSG_DWORD msg;
			SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_STATECHANGE );
			msg.dwData = m_TournamentState;
			PACKEDDATA_OBJ->SendToBroadCastMapServer( &msg, sizeof(msg) );

			//이 시점에서 정보를 보내죠야 할듯...
			//////////////////////////////////////////////////////////////////////////
			MSG_DWORD msg1; 
			//CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( m_TournamentState );
			//if( !pPlayer )	return;

			SetProtocol( &msg1, MP_GTOURNAMENT, MP_GTOURNAMENT_FLAGCHANGE );
			msg1.dwData = m_TournamentState;
			PACKEDDATA_OBJ->SendToBroadCastMapServer( &msg1, sizeof(msg1) );
			//////////////////////////////////////////////////////////////////////////

		}
		break;
	case eGTState_Regist:
		{
			if( st.wDayOfWeek < m_RegistTime[1].Day	)			return;			// 등록 일
			if( st.wDayOfWeek == m_RegistTime[1].Day )
			if( st.wHour < m_RegistTime[1].Hour )				return;			// 등록 시간
			if( st.wHour == m_RegistTime[1].Hour )
			if( st.wMinute < m_RegistTime[1].Minute )			return;			// 등록 분

			m_TournamentState = eGTState_BeforeEntrance;
			m_RemainTime = 0;

			// Broadcast To Mapserver
			MSG_DWORD msg;
			SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_STATECHANGE );
			msg.dwData = m_TournamentState;
			PACKEDDATA_OBJ->SendToBroadCastMapServer( &msg, sizeof(msg) );

			//이 시점에서 정보를 보내죠야 할듯...
			//////////////////////////////////////////////////////////////////////////
			MSG_DWORD msg1;
			SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_FLAGCHANGE );
			msg.dwData = m_TournamentState;
			PACKEDDATA_OBJ->SendToBroadCastMapServer( &msg, sizeof(msg) );
			//////////////////////////////////////////////////////////////////////////

		}
		break;
	case eGTState_BeforeEntrance:
		{
  			if( m_RemainTime )
  			{
  				if( gTickTime < m_RemainTime )
  					m_RemainTime -= gTickTime;
  				else
  				{
  					// 혹시라도 누군가가 남아있으면 삭제한다.
  					m_RemainTime = 0;
  					CPlayer* pData = NULL;
  					g_pUserTable->SetPositionHead();
  					while( pData = (CPlayer*)g_pUserTable->GetData() )
  						g_pServerSystem->RemovePlayer( pData->GetID(), FALSE );
					g_pUserTable->RemoveAll();	//KES 수정				

  				}
  			}

			if( st.wDayOfWeek != m_TournamentTime[g_nServerSetNum-1][m_CurTournament].Day ||
				st.wHour != m_TournamentTime[g_nServerSetNum-1][m_CurTournament].Hour ||
				st.wMinute < m_TournamentTime[g_nServerSetNum-1][m_CurTournament].Minute )
				return;
/**/			
			m_TournamentState = eGTState_Entrance;
			m_RemainTime = m_EntranceTime;
			CreateBattle();
			
			// Broadcast To Mapserver
			MSG_DWORD msg;
			SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_STATECHANGE );
			msg.dwData = m_TournamentState;
			PACKEDDATA_OBJ->SendToBroadCastMapServer( &msg, sizeof(msg) );
		}
		break;
	case eGTState_Entrance:
		{
			if( gTickTime < m_RemainTime )
				m_RemainTime -= gTickTime;
			else
			{
				m_TournamentState = eGTState_Process;
				m_RemainTime = m_FightTime;
				StartTournament();

				// Broadcast To Mapserver
				MSG_DWORD msg;
				SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_STATECHANGE );
				msg.dwData = m_TournamentState;
				PACKEDDATA_OBJ->SendToBroadCastMapServer( &msg, sizeof(msg) );

				// Broadcast To Player
				MSG_DWORD2 msg1;
				SetProtocol( &msg1, MP_GTOURNAMENT, MP_GTOURNAMENT_STARTBATTLE );
				msg1.dwData1 = m_CurTournament;
				msg1.dwData2 = m_TournamentCount;
				g_Network.Broadcast2AgentServer( (char*)&msg1, sizeof(MSG_DWORD2) );
			}
		}
		break;
	case eGTState_Process:
		{
			if( gTickTime < m_RemainTime )
			{
				m_RemainTime -= gTickTime;

				if( (m_NotifyEntranceTime - m_RemainTime) > 60000 )
				{
					// Broadcast To Player
					MSG_DWORD3 msg2;
					SetProtocol( &msg2, MP_GTOURNAMENT, MP_GTOURNAMENT_BATTLE_NOTIFY );
					msg2.dwData1 = m_CurTournament;
					msg2.dwData2 = m_NotifyEntranceTime;
					msg2.dwData3 = m_TournamentCount;
					g_Network.Broadcast2AgentServer( (char*)&msg2, sizeof(MSG_DWORD3) );
				}

				m_NotifyEntranceTime = m_RemainTime;
			}
			else
			{
				m_TournamentState = eGTState_Leave;
				m_RemainTime = m_LeaveTime;
				m_NotifyEntranceTime = 0;

				// Broadcast To Mapserver
				MSG_DWORD msg;
				SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_STATECHANGE );
				msg.dwData = m_TournamentState;
				PACKEDDATA_OBJ->SendToBroadCastMapServer( &msg, sizeof(msg) );
			}
		}
		break;
	case eGTState_Leave:
		{
			if( gTickTime < m_RemainTime )
				m_RemainTime -= gTickTime;
			else
			{
				if( m_CurTournament != eGTFight_1_2 )
				{
					++m_CurTournament;
					m_TournamentState = eGTState_BeforeEntrance;
					m_RemainTime = 60000;
					m_BattleCount = 0;
					memset( m_BattleInfo, 0, sizeof(BATTLE_INFO_GTOURNAMENT)*MAXGUILD_INTOURNAMENT/2 );

					// Broadcast To Mapserver
					MSG_DWORD msg;
					SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_CURTOURNAMENTCHANGE );
					msg.dwData = m_CurTournament;
					PACKEDDATA_OBJ->SendToBroadCastMapServer( &msg, sizeof(msg) );

					// Broadcast To Player
					MSG_DWORD2 msg1;
					SetProtocol( &msg1, MP_GTOURNAMENT, MP_GTOURNAMENT_ENDBATTLE );
					msg1.dwData1 = m_CurTournament-1;
					msg1.dwData2 = m_TournamentCount;
					g_Network.Broadcast2AgentServer( (char*)&msg1, sizeof(MSG_DWORD2) );

					// To DB
					GuildTournamentInfoUpdate( m_TournamentCount, m_CurTournament, m_WaitTime.value );
				}
				else
				{
					for(DWORD i=0; i<m_BattleCount; ++i)
					{
						CBattle* pBattle = BATTLESYSTEM->GetBattle( m_BattleInfo[i].BattleID );
						if( !pBattle )			continue;
						
						pBattle->SetDestroyFlag();
					}
					
					memset( m_BattleInfo, 0, sizeof(BATTLE_INFO_GTOURNAMENT)*MAXGUILD_INTOURNAMENT/2 );
					memset( m_PositionTable, -1, sizeof(int)*2*MAXGUILD_INTOURNAMENT/2 );

					m_CurTournament = eGTFight_End;
					m_TournamentState = eGTState_BeforeRegist;
					m_RemainTime = 0;
					m_BattleCount = 0;

					// 
					stTIME ct, tt;
					ct.value = GetCurTime();
					tt.SetTime( 0, 0, 8, 0, 0, 0 );
					ct += tt;
					m_WaitTime.SetTime( ct.GetYear(), ct.GetMonth(), ct.GetDay(), 0, 0, 0 );
					
					// Broadcast To Mapserver
					MSG_DWORD msg;
					SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_ENDBATTLE );
					msg.dwData = m_TournamentCount;
					PACKEDDATA_OBJ->SendToBroadCastMapServer( &msg, sizeof(msg) );

					// Broadcast To Player
					MSG_DWORD2 msg1;
					SetProtocol( &msg1, MP_GTOURNAMENT, MP_GTOURNAMENT_ENDBATTLE );
					msg1.dwData1 = m_CurTournament;
					msg1.dwData2 = m_TournamentCount-1;
					g_Network.Broadcast2AgentServer( (char*)&msg1, sizeof(MSG_DWORD2) );

					
					// To DB
					GuildTournamentInfoUpdate( m_TournamentCount, m_CurTournament, m_WaitTime.value );
				}
			}
		}
		break;
	default:
		break;
	}
}


void CGuildTournamentMgr::CreateBattle()
{
	m_RemainTime = m_EntranceTime;
	memset( m_BattleInfo, 0, sizeof(BATTLE_INFO_GTOURNAMENT)*MAXGUILD_INTOURNAMENT/2 );
	m_BattleCount = 0;
	
	//if( g_pServerSystem->GetMapNum() != GTMAPNUM )	
	if( FALSE == g_pServerSystem->GetMap()->IsMapKind(eTournament) )
		return;

	// 배틀생성
	for(DWORD i=0; i<BattleCount[m_CurTournament]; ++i)
	{
		m_BattleInfo[m_BattleCount].BattleKind	= eBATTLE_KIND_GTOURNAMENT;
		m_BattleInfo[m_BattleCount].BattleState	= eBATTLE_STATE_READY;
		m_BattleInfo[m_BattleCount].BattleTime	= 0;
		m_BattleInfo[m_BattleCount].Ranking		= m_CurTournament;
		
		m_BattleInfo[m_BattleCount].m_dwEntranceTime = m_EntranceTime;
		m_BattleInfo[m_BattleCount].m_dwFightTime = m_FightTime;
		m_BattleInfo[m_BattleCount].m_dwLeaveTime = m_LeaveTime;
		int Idx0 = GetCurTournamentGuild( i, TRUE );
		if( Idx0 != -1 )
		{
			if( m_GuildInfoList[Idx0].ProcessTournament > m_CurTournament )		continue;
			m_BattleInfo[m_BattleCount].MemberInfo.TeamGuildIdx[0] = m_GuildInfoList[Idx0].pGuild->GetIdx();
		}
		int Idx1 = GetCurTournamentGuild( i, FALSE );
		if( Idx1 != -1 )
		{
			if( m_GuildInfoList[Idx1].ProcessTournament > m_CurTournament )		continue;
			m_BattleInfo[m_BattleCount].MemberInfo.TeamGuildIdx[1] = m_GuildInfoList[Idx1].pGuild->GetIdx();
		}

		// 부전승
		if( Idx0 == -1 )
		{
			if( Idx1 == -1 )			continue;
			else
			{
				SetResult( m_BattleInfo[m_BattleCount].MemberInfo.TeamGuildIdx[1], TRUE, TRUE );
				continue;
			}
		}
		else
		{
			if( Idx1 == -1 )
			{
				SetResult( m_BattleInfo[m_BattleCount].MemberInfo.TeamGuildIdx[0], TRUE, TRUE );
				continue;
			}
		}

		m_BattleInfo[m_BattleCount].m_dwGroup = m_GuildInfoList[Idx1].Position/8;
		if( BATTLESYSTEM->CreateBattle( &m_BattleInfo[m_BattleCount], GTMAPNUM ) )
		{
			if( m_GuildInfoList[Idx0].pGuild )
				m_GuildInfoList[Idx0].pGuild->SetGTBattleID( m_BattleInfo[m_BattleCount].BattleID );
			if( m_GuildInfoList[Idx1].pGuild )
				m_GuildInfoList[Idx1].pGuild->SetGTBattleID( m_BattleInfo[m_BattleCount].BattleID );
			
			++m_BattleCount;
		}
		else
		{
			ASSERT("=Guild Tournament= : CreateBattle Failed ( GuildTournamentMgr.cpp )");
		}
	}
	
	// 다른서버로 배틀정보 보냄
	SEND_GTBATTLE_INFOBROAD msg1;
	SetProtocol( &msg1, MP_GTOURNAMENT, MP_GTOURNAMENT_BATTLEINFO );
	msg1.Count = (BYTE)m_BattleCount;
	msg1.CurTournament = m_CurTournament;
	for(i=0; i<m_BattleCount; ++i)
	{
		msg1.BattleInfo[i].BattleID = m_BattleInfo[i].BattleID;
		msg1.BattleInfo[i].Group = m_BattleInfo[i].m_dwGroup;				
		msg1.BattleInfo[i].GuildIdx[0] = m_BattleInfo[i].MemberInfo.TeamGuildIdx[0];
		msg1.BattleInfo[i].GuildIdx[1] = m_BattleInfo[i].MemberInfo.TeamGuildIdx[1];				
	}
	PACKEDDATA_OBJ->SendToBroadCastMapServer( &msg1, (WORD)msg1.GetSize() );
	//

	if( m_BattleCount )
	{
		MSG_DWORD3 msg2;
		SetProtocol( &msg2, MP_GTOURNAMENT, MP_GTOURNAMENT_BATTLE_NOTIFY );
		msg2.dwData1 = m_CurTournament;
		//msg2.dwData2 = m_EntranceTime;
		msg2.dwData2 = m_EntranceTime;
		msg2.dwData3 = m_TournamentCount;
		g_Network.Broadcast2AgentServer( (char*)&msg2, sizeof(MSG_DWORD3) );
		m_NotifyEntranceTime = m_EntranceTime;
	}
	else
	{
		m_TournamentState = eGTState_Leave;
		m_RemainTime = 0;
		m_NotifyEntranceTime = 0;
	}	
}


void CGuildTournamentMgr::StartTournament()
{
	// 토너먼트 시작
	for(DWORD i=0; i<m_BattleCount; ++i)
	{
		CBattle_GTournament* pBattle = (CBattle_GTournament*)BATTLESYSTEM->GetBattle( m_BattleInfo[i].BattleID );
		if( !pBattle )			continue;
		
		pBattle->StartBattle();
	}
}


DWORD CGuildTournamentMgr::AddGuild( CGuild* pGuild, DWORD& Pos )
{
	if( !pGuild )														return eGTError_DONTFINDGUILDINFO;
	if( m_RegistedGuildCount >= MAXGUILD_INTOURNAMENT )					return eGTError_MAXGUILDOVER;
	for(int i=0; i<MAXGUILD_INTOURNAMENT; ++i)
		if( m_GuildInfoList[i].pGuild &&
			m_GuildInfoList[i].pGuild->GetIdx() == pGuild->GetIdx() )	return eGTError_ALREADYREGISTE;
	if( m_TournamentCancelList.GetData( pGuild->GetIdx() ) )			return eGTError_ALREADYCANCELREGISTE;
	if( (pGuild->GetMemberNum() - pGuild->GetStudentNum()) < LIMITMEMBER_GUILDTOURNAMENT )			return eGTError_UNDERLIMITEMEMBER;
	if( pGuild->GetLevel() < LIMITLEVEL_GUILDTOURNAMENT )				return eGTError_UNDERLEVEL;
/**/
	//
	int Position = GetEmptyGuildPosition();
	if( Position == -1 )												return eGTError_DONTFINDPOSITION;
	m_GuildInfoList[Position].pGuild = pGuild;
	m_GuildInfoList[Position].Ranking = eGTStanding_32TH;
	m_GuildInfoList[Position].Position = Position;
	m_GuildInfoList[Position].ProcessTournament = eGTFight_32;
	m_PositionTable[Position%2][Position/2] = Position;

	++m_RegistedGuildCount;
	Pos = Position;
	
	return eGTError_NOERROR;
}


DWORD CGuildTournamentMgr::AddedGuild( CGuild* pGuild, DWORD Position )
{
	if( !pGuild )							return eGTError_DONTFINDPOSITION;

	m_GuildInfoList[Position].pGuild = pGuild;
	m_GuildInfoList[Position].Ranking = eGTStanding_32TH;
	m_GuildInfoList[Position].Position = (WORD)Position;
	m_GuildInfoList[Position].ProcessTournament = eGTFight_32;
	m_PositionTable[Position%2][Position/2] = Position;

	++m_RegistedGuildCount;
	return eGTError_NOERROR;
}


void CGuildTournamentMgr::AddPlayer( CPlayer* pPlayer )
{
	if( !pPlayer )											return;
	//if( g_pServerSystem->GetMapNum() != GTMAPNUM )		return;
	if( FALSE == g_pServerSystem->GetMap()->IsMapKind(eTournament) )		return;

	//
	if( m_bEvent )
	{
		AddPlayerEvent( pPlayer );
			return;
	}

	// Observer인지 체크
	CBattle_GTournament* pBattle = NULL;
	int BattleIdx = pPlayer->GetObserverBattleIdx();
	if( BattleIdx )
	{
		pBattle = (CBattle_GTournament*)BATTLESYSTEM->GetBattle( BattleIdx );
		if( !pBattle || pBattle->GetBattleKind() != eBATTLE_KIND_GTOURNAMENT )
			goto AddBattleFailed;

		pPlayer->SetVisible( FALSE );
		if( pBattle->AddObserverToBattle( pPlayer ) )
		{
			SEND_BATTLESCORE_INFO msg;
			SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_BATTLEJOIN_ACK );
			msg.Score[0] = pBattle->GetTeamMemberNum( eBattleTeam1 );
			msg.Score[1] = pBattle->GetTeamMemberNum( eBattleTeam2 );
			CGuild* pGuild = GUILDMGR->GetGuild( pBattle->GetBattleTeamGuildIdx( eBattleTeam1 ) );
			if( pGuild )	
			{
				//strcpy( msg.GuildName[0], pGuild->GetGuildName() );
				SafeStrCpy( msg.GuildName[0], pGuild->GetGuildName(), MAX_GUILD_NAME+1);
			}
			pGuild = GUILDMGR->GetGuild( pBattle->GetBattleTeamGuildIdx( eBattleTeam2 ) );
			if( pGuild )
			{
				//strcpy( msg.GuildName[1], pGuild->GetGuildName() );
				SafeStrCpy( msg.GuildName[1], pGuild->GetGuildName(), MAX_GUILD_NAME+1);
			}
			msg.FightTime = pBattle->GetFightTime();
			msg.EntranceTime = pBattle->GetEntranceTime();
			msg.State = m_TournamentState;
			msg.Team = 2;
			pPlayer->SendMsg( &msg, sizeof(msg) );
			return;
		}
		else
			goto AddBattleFailed;
	}
	else
	{		
		CGuild* pGuild = GUILDMGR->GetGuild( pPlayer->GetGuildIdx() );
		BattleIdx = GetBattleID( pGuild );
		if( BattleIdx == -1 )		goto AddBattleFailed;
		pBattle = (CBattle_GTournament*)BATTLESYSTEM->GetBattle( BattleIdx );
		if( !pBattle || pBattle->GetBattleKind() != eBATTLE_KIND_GTOURNAMENT )
			goto AddBattleFailed;
		
		pPlayer->SetVisible( TRUE );
		if( BATTLESYSTEM->AddObjectToBattle( pBattle, pPlayer ) == TRUE )
		{
			SEND_BATTLESCORE_INFO msg;
			SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_BATTLEJOIN_ACK );
			msg.Score[0] = pBattle->GetTeamMemberNum( eBattleTeam1 );
			msg.Score[1] = pBattle->GetTeamMemberNum( eBattleTeam2 );
			CGuild* pGuild = GUILDMGR->GetGuild( pBattle->GetBattleTeamGuildIdx( eBattleTeam1 ) );
			if( pGuild )
			{
				//strcpy( msg.GuildName[0], pGuild->GetGuildName() );
				SafeStrCpy( msg.GuildName[0], pGuild->GetGuildName(), MAX_GUILD_NAME+1 );
			}
			pGuild = GUILDMGR->GetGuild( pBattle->GetBattleTeamGuildIdx( eBattleTeam2 ) );
			if( pGuild )
			{
				//strcpy( msg.GuildName[1], pGuild->GetGuildName() );
				SafeStrCpy( msg.GuildName[1], pGuild->GetGuildName(), MAX_GUILD_NAME+1 );
			}
			msg.FightTime = pBattle->GetFightTime();
			msg.EntranceTime = pBattle->GetEntranceTime();
			msg.State = m_TournamentState;
			msg.Team = 0;			// 이건 옵저버 나가기 버튼보이기에만 쓰인다.
			pPlayer->SendMsg( &msg, sizeof(msg) );
			return;
		}
		else
			goto AddBattleFailed;
	}
	
AddBattleFailed:
	MSG_DWORD msg;
	SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_RETURNTOMAP );
	msg.dwData = pPlayer->GetReturnMapNum();
	pPlayer->SendMsg( &msg, sizeof(msg) );
	g_pServerSystem->RemovePlayer( pPlayer->GetID() );
	return;
}



int CGuildTournamentMgr::GetCurTournamentGuild( DWORD Group, BOOL bFront )
{
	DWORD pos = 0;
	int st = 0;
	int ed = 32;
	int Rank = 0;
	if( m_CurTournament <= eGTFight_4 )
	{
		pos = 2<<m_CurTournament;
		st = (pos)*Group;
		ed = (pos)+st;
		Rank = gRanking[m_CurTournament];
	}
	else if( m_CurTournament == eGTFight_3_4 )
		Rank = 4;
	else if( m_CurTournament == eGTFight_1_2 )
		Rank = 2;

	for(int i=st; i<ed; ++i)
	{
		if( m_GuildInfoList[i].Ranking == Rank )
		{
			if( bFront )				return i;
			else						bFront = TRUE;
		}
	}
	
	return -1;
}


int CGuildTournamentMgr::GetEmptyGuildPosition()
{	
	DWORD Key = 0;
	DWORD RandomCountUp = 0;
	DWORD RandomCountDown = 0;
	DWORD RandomCount = 0;
	int RandomUppos[MAXGUILD_INTOURNAMENT/2];
	int RandomDownpos[MAXGUILD_INTOURNAMENT/2];
	int RandomPos[MAXGUILD_INTOURNAMENT/2];
	memset( RandomUppos, -1, sizeof(int)*MAXGUILD_INTOURNAMENT/2 );
	memset( RandomDownpos, -1, sizeof(int)*MAXGUILD_INTOURNAMENT/2 );
	memset( RandomPos, -1, sizeof(int)*MAXGUILD_INTOURNAMENT/2 );

	// 비어있는 자리를 확인한다.
	for(int i=0; i<MAXGUILD_INTOURNAMENT/2; ++i)
	{
		BOOL bEmpty = FALSE;
		if( m_PositionTable[0][i] == -1 )
		{
			RandomUppos[RandomCountUp] = i*2;
			++RandomCountUp;
			bEmpty = TRUE;
		}
		if( m_PositionTable[1][i] == -1 )
		{
			RandomDownpos[RandomCountDown] = i*2+1;
			++RandomCountDown;

			if( bEmpty )
			{
				RandomPos[RandomCount] = i*2;
				++RandomCount;
			}
		}
	}

	// 2곳이 비어있는데를 먼저 처리해준다. 랜덤.
	if( RandomCount )
	{
		Key = rand()%RandomCount;
		if( RandomPos[Key] != -1 )
		{
			DWORD up = rand()%2;
			if( up )
				return RandomPos[Key];
			else
				return RandomPos[Key]+1;
		}
	}
	Key = rand()%2;
	if( Key )					// Random하게 위를 선택
	{
		if( RandomCountUp )
		{
			Key = rand()%RandomCountUp;
			if( RandomUppos[Key] != -1 )	return RandomUppos[Key];
		}
		else if( RandomCountDown )
		{
			Key = rand()%RandomCountDown;
			if( RandomDownpos[Key] != -1 )	return RandomDownpos[Key];
		}
	}
	else if( RandomCountDown )		// 아래를 선택
	{
		Key = rand()%RandomCountDown;
		if( RandomDownpos[Key] != -1 )	return RandomDownpos[Key];
	}
	else if( RandomCountUp )		// 없으면 다시 위를 선택
	{
		Key = rand()%RandomCountUp;
		if( RandomUppos[Key] != -1 )	return RandomUppos[Key];
	}
	
	
/*	for(int i=0; i<MAXGUILD_INTOURNAMENT/2; ++i)
	{
		if( m_PositionTable[0][i] == -1 && m_PositionTable[1][i] == -1 )
			return i*2;
	}

	DWORD RandomCount = 0;
	int RandomPos[MAXGUILD_INTOURNAMENT/2];
	memset( RandomPos, -1, sizeof(DWORD)*MAXGUILD_INTOURNAMENT/2 );
	for(i=0; i<MAXGUILD_INTOURNAMENT/2; ++i)
	{
		if( m_PositionTable[0][i] == -1 )			RandomPos[RandomCount] = i*2;
		else if( m_PositionTable[1][i] == -1 )		RandomPos[RandomCount] = i*2+1;
		else										continue;

		++RandomCount;
	}
	if( RandomCount == 0 )				return -1;
	DWORD Key = rand()%RandomCount;
	if( RandomPos[Key] != -1 )		return RandomPos[Key];*/	
	
	return -1;
}


DWORD CGuildTournamentMgr::DeleteAddedGuild( DWORD GuildIdx )
{
	CGuild* pGuild = GUILDMGR->GetGuild( GuildIdx );
	if( !pGuild )		return eGTError_DONTFINDGUILDINFO;

	for(int i=0; i<MAXGUILD_INTOURNAMENT; ++i)
	{
		if( m_GuildInfoList[i].pGuild && m_GuildInfoList[i].pGuild->GetIdx() == pGuild->GetIdx() )
		{
			for(int n=0; n<MAXGUILD_INTOURNAMENT/2; ++n)
			{
				if( m_PositionTable[0][n] == m_GuildInfoList[i].Position )
					m_PositionTable[0][n] = -1;
				if( m_PositionTable[1][n] == m_GuildInfoList[i].Position )
					m_PositionTable[1][n] = -1;
			}
			
			m_GuildInfoList[i].pGuild = NULL;
			m_GuildInfoList[i].Ranking = 0;
			m_GuildInfoList[i].Position = 0;
			m_GuildInfoList[i].ProcessTournament = eGTFight_32;
			
			--m_RegistedGuildCount;
			m_TournamentCancelList.Add( pGuild, pGuild->GetIdx() );

			return eGTError_NOERROR;
		}
	}

	return eGTError_DELETEAPPLIED;
}


void CGuildTournamentMgr::NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol,void* pMsg )
{
	switch( Protocol )
	{
	case MP_GTOURNAMENT_REGISTCOUNT:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer )		return;

			MSG_DWORD msg;
			SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_REGISTCOUNT );
			msg.dwData = m_TournamentState!=eGTState_Regist?999:m_RegistedGuildCount;
			pPlayer->SendMsg( &msg, sizeof(msg) );
		}
		break;
	case MP_GTOURNAMENT_REGIST_SYN:
		{
			//if( g_pServerSystem->GetMapNum() != GTMAPNUM )
			if( FALSE == g_pServerSystem->GetMap()->IsMapKind(eTournament) )
			{
				MSGBASE* pmsg = (MSGBASE*)pMsg;
				CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
				if( !pPlayer )		return;
			
				if( pPlayer->GetGuildMemberRank() != GUILD_MASTER )
				{
					MSG_DWORD msg;
					SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_REGIST_NACK );
					msg.dwData = eGTError_NOGUILDMASTER;
					pPlayer->SendMsg( &msg, sizeof(msg) );
					return;
				}

				MSG_DWORD3 msg;
				SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_REGIST_SYN );
				msg.dwData1 = g_pServerSystem->GetMapNum();
				msg.dwData2 = pmsg->dwObjectID;
				msg.dwData3 = pPlayer->GetGuildIdx();
				PACKEDDATA_OBJ->SendToMapServer( dwConnectionIndex, GTMAPNUM, &msg, sizeof(MSG_DWORD3) );
				return;
			}

			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			// dwData1 - MapNum
			// dwData2 - PlayerIdx
			// dwData3 - GuildIdx
			
			DWORD res;
			DWORD Position = 0;
			CGuild* pGuild = GUILDMGR->GetGuild( pmsg->dwData3 );
			if( pGuild )
			{
				//----------------------------------------------------------------//
				// 등록시도한 길드 숫자를 알기위해--------------------------------//
				SYSTEMTIME time;
				char szFile[256] = {0,};
				GetLocalTime( &time );
				sprintf( szFile, "./Log/GT_Regist_%d.txt", m_TournamentCount );

				FILE* fp = fopen(szFile,"a+");
				if( fp )
				{
					char buf[256];
					sprintf( buf, "%d	%s	%d-%d-%d-%d\n", pGuild->GetIdx(), pGuild->GetGuildName(),
						time.wMonth, time.wDay, time.wHour, time.wMinute );
					fprintf(fp, buf);
					fclose(fp);
				}
				//----------------------------------------------------------------//
			}
	
			if( m_TournamentState != eGTState_Regist )
			{
				MSG_DWORD2 msg;
				SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_REGIST_NACK );
				msg.dwData1 = eGTError_NOTREGISTDAY;
				msg.dwData2 = pmsg->dwData2;
				PACKEDDATA_OBJ->SendToMapServer( dwConnectionIndex, (MAPTYPE)pmsg->dwData1, &msg, sizeof(MSG_DWORD2) );
				return;
			}

			if( (res = AddGuild( pGuild, Position )) == eGTError_NOERROR )
			{
				MSG_DWORD3 msg;
				SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_REGIST_ACK );
				msg.dwData1 = m_RegistedGuildCount;
				msg.dwData2 = m_TournamentCount;
				msg.dwData3 = pmsg->dwData2;
				PACKEDDATA_OBJ->SendToMapServer( dwConnectionIndex, (MAPTYPE)pmsg->dwData1, &msg, sizeof(MSG_DWORD3) );

				// 각 맵서버로 BroadCast				
				MSG_DWORD2 msg1;
				SetProtocol( &msg1, MP_GTOURNAMENT, MP_GTOURNAMENT_REGISTACK );
				msg1.dwData1 = pmsg->dwData3;
				msg1.dwData2 = Position;
				PACKEDDATA_OBJ->SendToBroadCastMapServer( &msg1, sizeof(MSG_DWORD2) );

				// To DB
				GuildTournamentGuildInfoInsert( pGuild->GetIdx(), pGuild->GetGuildName(), Position );

				// To LogDB
				GuildTournamentAddLog( m_TournamentCount, pGuild->GetIdx(), eGTLogKind_Regist, 32 );
				return;
			}
			else
			{
				MSG_DWORD2 msg;
				SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_REGIST_NACK );
				msg.dwData1 = res;
				msg.dwData2 = pmsg->dwData2;
				PACKEDDATA_OBJ->SendToMapServer( dwConnectionIndex, (MAPTYPE)pmsg->dwData1, &msg, sizeof(MSG_DWORD2) );
				return;
			}
		}
		break;
		// From Tournament Map -------------------------------------//
	case MP_GTOURNAMENT_REGIST_ACK:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData3 );
			if( !pPlayer )		return;

			MSG_DWORD2 msg;
			SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_REGIST_ACK );
			msg.dwData1 = pmsg->dwData1;
			msg.dwData2 = pmsg->dwData2;
			pPlayer->SendMsg( &msg, sizeof(MSG_DWORD2) );
		}
		break;
	case MP_GTOURNAMENT_REGIST_NACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData2 );
			if( !pPlayer )		return;

			MSG_DWORD msg;
			SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_REGIST_NACK );
			msg.dwData = pmsg->dwData1;
			pPlayer->SendMsg( &msg, sizeof(MSG_DWORD) );
		}
		break;
		//-----------------------------------------------------------//
	case MP_GTOURNAMENT_REGISTCANCEL_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer )		return;

			if( m_TournamentState != eGTState_Regist )
			{
				MSG_DWORD msg;
				SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_REGISTCANCEL_NACK );
				msg.dwData = eGTError_NOTREGISTCANCELDAY;
				pPlayer->SendMsg( &msg, sizeof(msg) );				
				return;
			}

			//
			if( pPlayer->GetGuildMemberRank() != GUILD_MASTER )
			{
				MSG_DWORD msg;
				SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_REGISTCANCEL_NACK );
				msg.dwData = eGTError_NOGUILDMASTER;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}

			int res;			
			if( (res = DeleteAddedGuild( pPlayer->GetGuildIdx() )) == eGTError_NOERROR )
			{
				MSG_DWORD msg;
				SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_REGISTCANCEL_ACK );
				msg.dwData = m_TournamentCount;
				pPlayer->SendMsg( &msg, sizeof(msg) );

				// To DB
				GuildTournamentGuildInfoDelete( pPlayer->GetGuildIdx() );
				GuildTournamentCancelIdxInsert( m_TournamentCount, pPlayer->GetGuildIdx() );

				// 각 맵서버로 BroadCast
				MSG_DWORD msg1;
				SetProtocol( &msg1, MP_GTOURNAMENT, MP_GTOURNAMENT_REGISTCANCEL );
				msg1.dwData = pPlayer->GetGuildIdx();
				PACKEDDATA_OBJ->SendToBroadCastMapServer( &msg1, sizeof(msg1) );
				return;
			}
			else
			{
				MSG_DWORD msg;
				SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_REGISTCANCEL_NACK );
				msg.dwData = res;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}
		}
		break;
	case MP_GTOURNAMENT_MOVETOBATTLEMAP_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer )	return;

			// 옵저버 모드, 해당 배틀의 번호
			if( pmsg->dwData != 0 )
			{
				for(DWORD i=0; i<m_BattleCount; ++i)
				{
					if( m_BattleInfo[i].BattleID == pmsg->dwData )
					{
						g_pServerSystem->RemovePlayer( pPlayer->GetID() );
						
						MSG_DWORD msg;
						SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_MOVETOBATTLEMAP_ACK );
						msg.dwData = pmsg->dwData;
						pPlayer->SendMsg( &msg, sizeof(msg) );
						return;
					}
				}

				MSG_DWORD msg;
				SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_MOVETOBATTLEMAP_NACK );
				msg.dwData = eGTError_DONTFINDBATTLE;
				pPlayer->SendMsg( &msg, sizeof(msg) );			
				return;
			}

			if( m_TournamentState != eGTState_Entrance )
			{
				MSG_DWORD msg;
				SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_MOVETOBATTLEMAP_NACK );
				msg.dwData = eGTError_NOTENTERTIME;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}

			// 
			CGuild* pGuild = GUILDMGR->GetGuild( pPlayer->GetGuildIdx() );
			if( pGuild == NULL )
			{
				MSG_DWORD msg;
				SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_MOVETOBATTLEMAP_NACK );
				msg.dwData = eGTError_DONTFINDGUILDINFO;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}

			//SW060719 문하생
			if( pPlayer->GetGuildMemberRank() < GUILD_MEMBER )
			{
				MSG_DWORD msg;
				SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_MOVETOBATTLEMAP_NACK );
				msg.dwData = eGTError_STUDENTCANTENTER;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}

			int res;
			if( (res = IsRegistedGuid( pGuild->GetIdx() )) != eGTError_NOERROR )
			{
				MSG_DWORD msg;
				SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_MOVETOBATTLEMAP_NACK );
				msg.dwData = res;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}

			res = GetBattleID( pGuild );
			if( res == -1 )
			{
				MSG_DWORD msg;
				SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_MOVETOBATTLEMAP_NACK );
				msg.dwData = eGTError_DONTFINDBATTLE;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}
			else
			{
				g_pServerSystem->RemovePlayer( pPlayer->GetID() );
				
				MSG_DWORD msg;
				SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_MOVETOBATTLEMAP_ACK );
				msg.dwData = 0;
				pPlayer->SendMsg( &msg, sizeof(msg) );
			}
		}
		break;
	case MP_GTOURNAMENT_BATTLEJOIN_SYN:
		{
			SEND_BATTLEJOIN_INFO* pmsg = (SEND_BATTLEJOIN_INFO*)pMsg;


			CObject* pObject = g_pUserTable->FindUser(pmsg->dwObjectID);				
			if( pObject != NULL )
			{
				if( pObject->GetObjectKind() == eObjectKind_Player )
				{
					MSG_DWORD msg;
					SetProtocol( &msg, MP_GTOURNAMENT, MP_USERCONN_GAMEIN_NACK );
					msg.dwData		= pmsg->dwObjectID;					
					
					g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );					
					return;
				}
			}
			
			CPlayer* pPlayer = g_pServerSystem->AddPlayer( pmsg->dwObjectID, dwConnectionIndex, pmsg->AgentIdx, 0 );
			if(!pPlayer) return;
			
			pPlayer->SetUserLevel( pmsg->UserLevel );
			CharacterNumSendAndCharacterInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			CharacterMugongInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			CharacterItemOptionInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			//SW050920 Rare
			CharacterItemRareOptionInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			CharacterAbilityInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			//SW051129 Pet
			//CharacterPetInfo(pmsg->dwObjectID, pPlayer->GetUserID());

			//if( g_pServerSystem->GetMapNum() != GTMAPNUM )
			if( FALSE == g_pServerSystem->GetMap()->IsMapKind(eTournament) )
			{
				MSG_DWORD msg;
				SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_RETURNTOMAP );
				msg.dwData = pmsg->ReturnMapNum;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}

			pPlayer->SetGuildIdx( pmsg->GuildIdx );
			pPlayer->SetReturnMapNum( (WORD)pmsg->ReturnMapNum );
			pPlayer->SetObserverBattleIdx( 0 );
			/*
			CGuild* pGuild = GUILDMGR->GetGuild( pmsg->GuildIdx );	
			int BattleId = GetBattleID( pGuild );
			if( BattleId == -1 )		return;

			CBattle_GTournament* pBattle = (CBattle_GTournament*)BATTLESYSTEM->GetBattle( BattleId );
			if( !pBattle )
			{
				MSG_DWORD msg;
				SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_RETURNTOMAP );
				msg.dwData = pmsg->ReturnMapNum;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}
						
			if( BATTLESYSTEM->AddObjectToBattle( pBattle, pPlayer ) == TRUE )
			{
				SEND_BATTLESCORE_INFO msg;
				SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_BATTLEJOIN_ACK );
				msg.Score[0] = pBattle->GetTeamMemberNum( eBattleTeam1 );
				msg.Score[1] = pBattle->GetTeamMemberNum( eBattleTeam2 );
				CGuild* pGuild = GUILDMGR->GetGuild( pBattle->GetBattleTeamGuildIdx( eBattleTeam1 ) );
				if( pGuild )	strcpy( msg.GuildName[0], pGuild->GetGuildName() );
				pGuild = GUILDMGR->GetGuild( pBattle->GetBattleTeamGuildIdx( eBattleTeam2 ) );
				if( pGuild )	strcpy( msg.GuildName[1], pGuild->GetGuildName() );
				msg.FightTime = pBattle->GetFightTime();
				msg.EntranceTime = pBattle->GetEntranceTime();
				msg.State = m_TournamentState;
				msg.Team = 0;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}
			else
			{
				MSG_DWORD msg;
				SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_RETURNTOMAP );
				msg.dwData = pmsg->ReturnMapNum;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}*/
		}
		break;
	case MP_GTOURNAMENT_OBSERVERJOIN_SYN:
		{
			SEND_BATTLEJOIN_INFO* pmsg = (SEND_BATTLEJOIN_INFO*)pMsg;

			CObject* pObject = g_pUserTable->FindUser(pmsg->dwObjectID);				
			if( pObject != NULL )
			{
				if( pObject->GetObjectKind() == eObjectKind_Player )
				{
					MSG_DWORD msg;
					SetProtocol( &msg, MP_GTOURNAMENT, MP_USERCONN_GAMEIN_NACK );
					msg.dwData		= pmsg->dwObjectID;					
					
					g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );					
					return;
				}
			}
	
			CPlayer* pPlayer = g_pServerSystem->AddPlayer( pmsg->dwObjectID, dwConnectionIndex, pmsg->AgentIdx, 0 );
			if(!pPlayer) return;
			
			pPlayer->SetUserLevel( pmsg->UserLevel );
			CharacterNumSendAndCharacterInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			CharacterMugongInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			CharacterItemOptionInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			//SW050920 Rare
			CharacterItemRareOptionInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			CharacterAbilityInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);

			// Observer
			pPlayer->SetGuildIdx( pmsg->GuildIdx );
			pPlayer->SetReturnMapNum( (WORD)pmsg->ReturnMapNum );
			pPlayer->SetObserverBattleIdx( pmsg->BattleIdx );


			/*CBattle_GTournament* pBattle = (CBattle_GTournament*)BATTLESYSTEM->GetBattle( pmsg->BattleIdx );
			if( pBattle == NULL )
			{
				MSG_DWORD msg;
				SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_RETURNTOMAP );
				msg.dwData = pmsg->ReturnMapNum;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}			
			
			if( pBattle->AddObserverToBattle( pPlayer ) )
			{
				SEND_BATTLESCORE_INFO msg;
				SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_BATTLEJOIN_ACK );
				msg.Score[0] = pBattle->GetTeamMemberNum( eBattleTeam1 );
				msg.Score[1] = pBattle->GetTeamMemberNum( eBattleTeam2 );
				CGuild* pGuild = GUILDMGR->GetGuild( pBattle->GetBattleTeamGuildIdx( eBattleTeam1 ) );
				if( pGuild )	strcpy( msg.GuildName[0], pGuild->GetGuildName() );
				pGuild = GUILDMGR->GetGuild( pBattle->GetBattleTeamGuildIdx( eBattleTeam2 ) );
				if( pGuild )	strcpy( msg.GuildName[1], pGuild->GetGuildName() );
				msg.FightTime = pBattle->GetFightTime();
				msg.EntranceTime = pBattle->GetEntranceTime();
				msg.State = m_TournamentState;
				msg.Team = 2;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}
			else
			{
				MSG_DWORD msg;
				SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_RETURNTOMAP );
				msg.dwData = pmsg->ReturnMapNum;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				return;
			}*/
		}
		break;
	case MP_GTOURNAMENT_LEAVE_SYN:		// 토너먼트에서 맵으로 돌아왔을때 호출
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

			// 토너먼트 맵이면 유저를 Remove
			/*//if( g_pServerSystem->GetMapNum() == GTMAPNUM )
			if( TRUE == g_pServerSystem->GetMap()->IsMapKind(eTournament) )
			{
				g_pServerSystem->RemovePlayer( pmsg->dwObjectID );				
				return;
			}*/
						
			CObject* pObject = g_pUserTable->FindUser(pmsg->dwObjectID);
			if( pObject != NULL )
			{
				if( pObject->GetObjectKind() == eObjectKind_Player )
				{
					MSG_DWORD msg;
					SetProtocol( &msg, MP_USERCONN, MP_USERCONN_GAMEIN_NACK );
					msg.dwData	= pmsg->dwObjectID;					
					g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );
					return;
				}
			}
			
			CPlayer* pPlayer = g_pServerSystem->AddPlayer( pmsg->dwObjectID, dwConnectionIndex, pmsg->dwData1, pmsg->dwData3 );
			if(!pPlayer) return;
			
			pPlayer->SetUserLevel( pmsg->dwData2 );
			CharacterNumSendAndCharacterInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			CharacterMugongInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			CharacterItemOptionInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			//SW050920 Rare
			CharacterItemRareOptionInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			CharacterAbilityInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);

			QuestTotalInfo(pmsg->dwObjectID);		// 초보자 퀘스트

			// quest 정보 qurey 추가
			QUESTMGR->CreateQuestForPlayer( pPlayer );
			// 항상 서브퀘스트 정보부터 읽어야 함...
//			QuestMainQuestLoad(pmsg->dwObjectID);
			QuestSubQuestLoad(pmsg->dwObjectID);
//			QuestItemload(pmsg->dwObjectID);
		}
		break;
	case MP_GTOURNAMENT_STANDINGINFO_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			
//			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
//			if( !pPlayer )			return;


			// 등록된 길드의 모든 정보를 보낸다.
			if( pmsg->dwData1 == 1 )
			{
				SEND_REGISTEDGUILDINFO msg;
				SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_STANDINGINFO_REGISTED );
				msg.CurTournament = (BYTE)m_CurTournament;
				msg.PlayerID = pmsg->dwData2;

				for(int i=0; i<MAXGUILD_INTOURNAMENT; ++i)
				{
					if( m_GuildInfoList[i].pGuild == NULL )			continue;

					msg.GInfo[msg.Count].GuildIdx = m_GuildInfoList[i].pGuild->GetIdx();
					//strcpy( msg.GInfo[msg.Count].GuildName, m_GuildInfoList[i].pGuild->GetGuildName() );
					SafeStrCpy( msg.GInfo[msg.Count].GuildName, m_GuildInfoList[i].pGuild->GetGuildName(), MAX_GUILD_NAME+1);
					msg.GInfo[msg.Count].Position = (BYTE)m_GuildInfoList[i].Position;
					msg.GInfo[msg.Count].Ranking = (BYTE)m_GuildInfoList[i].Ranking;
					msg.GInfo[msg.Count].ProcessTournament = (BYTE)m_GuildInfoList[i].ProcessTournament;

					CGuildMark* pMark = GUILDMGR->GetMark( m_GuildInfoList[i].pGuild->GetMarkName() );
					if( pMark )
					{
						memcpy( msg.GInfo[msg.Count].ImageData,  pMark->GetImgData(), GUILDMARK_BUFSIZE );
						msg.GInfo[msg.Count].MarkExist = m_GuildInfoList[i].pGuild->GetMarkName();
					}
					else
						msg.GInfo[msg.Count].MarkExist = 0;

					++msg.Count;
				}

				g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(SEND_REGISTEDGUILDINFO) );				
				//pPlayer->SendMsg( &msg, msg.GetSize() );
			}
			// 길드의 순위정보만 보낸다.
			/*else if( pmsg->wData == 2 )
			{
				SEND_GUILDRANKING msg;
				SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_STANDINGINFO_ACK );

				for(int i=0; i<MAXGUILD_INTOURNAMENT; ++i)
				{
					if( m_GuildInfoList[i].pGuild == NULL )			continue;

					msg.Ranking[msg.Count].GuildIdx = m_GuildInfoList[i].pGuild->GetIdx();
					msg.Ranking[msg.Count].Ranking = m_GuildInfoList[i].Ranking;
					
					++msg.Count;
				}
				
				pPlayer->SendMsg( &msg, sizeof(msg) );
			}*/
		}
		break;
	case MP_GTOURNAMENT_BATTLEINFO_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer )			return;

			SEND_GTBATTLE_INFO msg;
			SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_BATTLEINFO_ACK );
			msg.PlayOff = (m_CurTournament<=eGTFight_8)?0:1;			

			for( DWORD i=0; i<m_BattleCount; ++i)
			{
				msg.BattleInfo[i].BattleID = m_BattleInfo[i].BattleID;
				msg.BattleInfo[i].Group = (BYTE)m_BattleInfo[i].m_dwGroup;
				CGuild* pGuild = GUILDMGR->GetGuild( m_BattleInfo[i].MemberInfo.TeamGuildIdx[0] );
				if( pGuild )
				{
//					strcpy( msg.BattleInfo[i].GuildName1, pGuild->GetGuildName() );
					SafeStrCpy( msg.BattleInfo[i].GuildName1, pGuild->GetGuildName(), MAX_GUILD_NAME+1 );
				}
				pGuild = GUILDMGR->GetGuild( m_BattleInfo[i].MemberInfo.TeamGuildIdx[1] );
				if( pGuild )
				{
//					strcpy( msg.BattleInfo[i].GuildName2, pGuild->GetGuildName() );
					SafeStrCpy( msg.BattleInfo[i].GuildName2, pGuild->GetGuildName(), MAX_GUILD_NAME+1 );
				}
				++msg.Count;
			}
			
			pPlayer->SendMsg( &msg, msg.GetSize() );
		}
		break;
	case MP_GTOURNAMENT_OBSERVER_EXIT:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer )			return;
			
			CBattle_GTournament* pBattle = (CBattle_GTournament*)BATTLESYSTEM->GetBattle( pmsg->dwData );
			if( pBattle )
				pBattle->ReturnToMap( pPlayer->GetID() );
		}
		break;
		// BroadCating되어서 온 Msg
	case MP_GTOURNAMENT_REGISTACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			DWORD res;
			DWORD Position = 0;
			CGuild* pGuild = GUILDMGR->GetGuild( pmsg->dwData1 );
			if( (res = AddedGuild( pGuild, pmsg->dwData2 )) == eGTError_NOERROR )
			{
			}
			else
			{
				ASSERT(0);
			}
		}
		break;
	case MP_GTOURNAMENT_REGISTCANCEL:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			DWORD res;			
			if( (res = DeleteAddedGuild( pmsg->dwData )) == eGTError_NOERROR )
			{
			}
			else
			{
				ASSERT(0);
			}			
		}
		break;
	case MP_GTOURNAMENT_STATECHANGE:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			m_TournamentState = pmsg->dwData;
		}
		break;
	case MP_GTOURNAMENT_CURTOURNAMENTCHANGE:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			m_CurTournament = pmsg->dwData;
			m_TournamentState = eGTState_BeforeEntrance;
			m_BattleCount = 0;
 			memset( m_BattleInfo, 0, sizeof(BATTLE_INFO_GTOURNAMENT)*MAXGUILD_INTOURNAMENT/2 );
		}
		break;
	case MP_GTOURNAMENT_BATTLEINFO:
		{
			SEND_GTBATTLE_INFOBROAD* pmsg = (SEND_GTBATTLE_INFOBROAD*)pMsg;
			//if( g_pServerSystem->GetMapNum() == GTMAPNUM )		return;
			if( TRUE == g_pServerSystem->GetMap()->IsMapKind(eTournament) )	return;

			memset( m_BattleInfo, 0, sizeof(BATTLE_INFO_GTOURNAMENT)*(MAXGUILD_INTOURNAMENT/2) );
			m_BattleCount = 0;
			for(int i=0; i<pmsg->Count; ++i)
			{
				m_BattleInfo[i].m_dwGroup = pmsg->BattleInfo[i].Group;
				m_BattleInfo[i].BattleID = pmsg->BattleInfo[i].BattleID;
				m_BattleInfo[i].MemberInfo.TeamGuildIdx[0] = pmsg->BattleInfo[i].GuildIdx[0];
				m_BattleInfo[i].MemberInfo.TeamGuildIdx[1] = pmsg->BattleInfo[i].GuildIdx[1];

				// Broadcast To Guild
				MSGBASE msg;
				SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_BATTLE_ENTER );
				CGuild* pGuild = GUILDMGR->GetGuild( m_BattleInfo[i].MemberInfo.TeamGuildIdx[0] );
				if( pGuild )			pGuild->SendMsgToAll( &msg, sizeof(msg) );
				pGuild = GUILDMGR->GetGuild( m_BattleInfo[i].MemberInfo.TeamGuildIdx[1] );
				if( pGuild )			pGuild->SendMsgToAll( &msg, sizeof(msg) );

				++m_BattleCount;
			}

			m_CurTournament = pmsg->CurTournament;
		}
		break;
	case MP_GTOURNAMENT_BATEND_TOMAP:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			//if( g_pServerSystem->GetMapNum() == GTMAPNUM )		return;
			if( TRUE == g_pServerSystem->GetMap()->IsMapKind(eTournament) )	return;
			
			for(DWORD i=0; i<m_BattleCount; ++i)
			{
				if( m_BattleInfo[i].BattleID == pmsg->dwData )
				{
					for(DWORD n=i; n<m_BattleCount-1; ++n)
						memcpy( &m_BattleInfo[n], &m_BattleInfo[n+1], sizeof(BATTLE_INFO_GTOURNAMENT) );

					--m_BattleCount;
					memset( &m_BattleInfo[n], 0, sizeof(BATTLE_INFO_GTOURNAMENT) );
					break;
				}
			}
		}
		break;
	case MP_GTOURNAMENT_NOTIFY_WINLOSE:
		{
			SEND_GTWINLOSE* pmsg = (SEND_GTWINLOSE*)pMsg;
			//if( g_pServerSystem->GetMapNum() == GTMAPNUM )		return;
			if( TRUE == g_pServerSystem->GetMap()->IsMapKind(eTournament) )	return;

			for(int i=0; i<MAXGUILD_INTOURNAMENT; ++i)
			{
				if( m_GuildInfoList[i].pGuild == NULL )			continue;

				if( m_GuildInfoList[i].pGuild->GetIdx() == pmsg->GuildIdx  )
				{
					BOOL bWin = pmsg->bWin==1?TRUE:FALSE;
					m_GuildInfoList[i].Ranking = (WORD)GetRanking( m_GuildInfoList[i].Ranking, bWin );
					m_GuildInfoList[i].ProcessTournament = (WORD)(m_CurTournament+1);
					break;
				}
			}
		}
		break;
	case MP_GTOURNAMENT_WIN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			//if( g_pServerSystem->GetMapNum() == GTMAPNUM )		return;
			if( TRUE == g_pServerSystem->GetMap()->IsMapKind(eTournament) )	return;

			for(int i=0; i<MAXGUILD_INTOURNAMENT; ++i)
			{
				if( m_GuildInfoList[i].pGuild == NULL )			continue;

				if( m_GuildInfoList[i].pGuild->GetIdx() == pmsg->dwData  )
				{
					m_GuildInfoList[i].Ranking = (WORD)GetRanking( m_GuildInfoList[i].Ranking, TRUE );
					m_GuildInfoList[i].ProcessTournament = (WORD)(m_CurTournament+1);
					break;
				}
			}
		}
		break;
	case MP_GTOURNAMENT_ENDBATTLE:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			//if( g_pServerSystem->GetMapNum() == GTMAPNUM )		return;
			if( TRUE == g_pServerSystem->GetMap()->IsMapKind(eTournament) )	return;

			memset( m_BattleInfo, 0, sizeof(BATTLE_INFO_GTOURNAMENT)*MAXGUILD_INTOURNAMENT/2 );			
			memset( m_GuildInfoList, 0, sizeof(GTInfo)*MAXGUILD_INTOURNAMENT );
			memset( m_PositionTable, -1, sizeof(int)*2*MAXGUILD_INTOURNAMENT/2 );

			m_TournamentCount = pmsg->dwData;
			m_CurTournament = eGTFight_End;
			m_RegistedGuildCount = 0;
			m_BattleCount = 0;
			m_TournamentCancelList.RemoveAll();
		}
		break;
/*	case MP_GTOURNAMENT_LOSE:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			//if( g_pServerSystem->GetMapNum() == GTMAPNUM )		return;
			if( TRUE == g_pServerSystem->GetMap()->IsMapKind(eTournament) )	return;

			for(int i=0; i<MAXGUILD_INTOURNAMENT; ++i)
			{
				if( m_GuildInfoList[i].pGuild == NULL )			continue;
				
				if( m_GuildInfoList[i].pGuild->GetIdx() == pmsg->dwData )
				{
					m_GuildInfoList[i].Ranking = GetRanking( m_GuildInfoList[i].Ranking, FALSE );

					// To DB
					//if(g_pServerSystem->GetMapNum() == GTMAPNUM )
					if( TRUE == g_pServerSystem->GetMap()->IsMapKind(eTournament) )
						GuildTournamentGuildInfoUpdate( pmsg->dwData, m_GuildInfoList[i].Ranking, m_CurTournament+1 );
					break;
				}
			}
		}
		break;*/
	case MP_GTOURNAMENT_EVENT_START:
		{
			MSG_GT_EVENTSTART* pmsg = (MSG_GT_EVENTSTART*)pMsg;

			DWORD rt = GTMGR->StartTournamentEvent( pmsg );

			MSG_DWORD2 msg;
			SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_EVENT_START_RESULT );
			msg.dwData1 = pmsg->SenderID;
			msg.dwData2 = rt;
			g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;
	case MP_GTOURNAMENT_EVENT_END:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			ResetEvent();
		}
		break;
	case MP_GTOURNAMENT_EVENT_START_ACK:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

			CGuild* pGuild1 = GUILDMGR->GetGuild( pmsg->dwData1 );			
			CGuild* pGuild2 = GUILDMGR->GetGuild( pmsg->dwData2 );

			MSG_DWORD msg;
			SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_EVENT_START_ACK );
			msg.dwData = pmsg->dwData3;

			if( pGuild1 )
				pGuild1->SendMsgToAll( &msg, sizeof(msg) );
			if( pGuild2 )
				pGuild2->SendMsgToAll( &msg, sizeof(msg) );
		}
		break;
	case MP_GTOURNAMENT_CHEAT:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			MSG_DWORD3 msg;					
			switch( pmsg->dwData2 )
			{
			case 1:
				{
					CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData1 );
					if( !pPlayer )			return;

					SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_CHEAT );
					msg.dwData1 = pmsg->dwData1;
					msg.dwData2 = m_CurTournament;
					msg.dwData3 = m_RegistedGuildCount;
					pPlayer->SendMsg( &msg, sizeof(msg) );
				}
				break;
			case 2:
				{
					//if( g_pServerSystem->GetMapNum() == Tournament )
					if( TRUE == g_pServerSystem->GetMap()->IsMapKind(eTournament) )
					{
						SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_CHEAT );
						msg.dwData1 = pmsg->dwData1;
						msg.dwData2 = m_CurTournament;
						msg.dwData3 = m_RegistedGuildCount;
						g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );
					}
					/*	Init();
					GuildTournamentInfoLoad();*/

				/*	m_CurTournament = eGTFight_32;
 					m_TournamentState = eGTState_Regist;
 					m_RemainTime = 30000;
 					m_RegistedGuildCount = 0;
 					m_BattleCount = 0;
 					
 					for(int i=0; i<m_BattleCount; ++i)
 					{
 						CBattle* pBattle = BATTLESYSTEM->GetBattle( m_BattleInfo[i].BattleID );
 						if( !pBattle )			continue;
 
 						pBattle->SetDestroyFlag();
 					}
 
 					for( i=0; i<MAXGUILD_INTOURNAMENT/2; ++i)
 						memset( &m_BattleInfo[i], 0, sizeof(BATTLE_INFO_GTOURNAMENT) );
 					for(i=0; i<MAXGUILD_INTOURNAMENT; ++i)
 						memset( &m_GuildInfoList[i], 0, sizeof(GTInfo) );
 					memset( m_PositionTable, -1, sizeof(int)*2*MAXGUILD_INTOURNAMENT/2 );
 
 					m_TournamentCancelList.RemoveAll();

					for( i=0; i<32; ++i)
					{
						int Position = GetEmptyGuildPosition();
						m_GuildInfoList[Position].Position = Position;
						m_PositionTable[Position%2][Position/2] = Position;
						m_GuildInfoList[Position].Ranking = 32;

						printf( "%d - p:%d  [%d][%d]\n", i, Position, Position%2, Position/2 );
					}

					printf( "\n" );

					for( i=0; i<16; ++i )
					{
						int idx1 = GetCurTournamentGuild( i, TRUE );
						int idx2 = GetCurTournamentGuild( i, FALSE );

						printf(" %d vs %d\n", idx1, idx2 );
					}*/
				}
				break;
			}
		}
		break;
	case MP_GTOURNAMENT_FLAGCHANGE:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			m_TournamentState = pmsg->dwData;
		}
	default:
		break;
	}
}


DWORD CGuildTournamentMgr::IsRegistedGuid( DWORD GuildIdx )
{
	if( GuildIdx == 0 )				return eGTError_DONTFINDGUILDINFO;

	for(int i=0; i<MAXGUILD_INTOURNAMENT; ++i)
	{
		if( m_GuildInfoList[i].pGuild && m_GuildInfoList[i].pGuild->GetIdx() == GuildIdx )
		{
			DWORD Ranking = gRanking[m_CurTournament];
			if( m_CurTournament == eGTFight_3_4 )
				Ranking = 4;
			else if( m_CurTournament == eGTFight_1_2 )
				Ranking = 2;
			
			if( m_GuildInfoList[i].Ranking == Ranking )
				return eGTError_NOERROR;
		}
	}

	return eGTError_DONTFINDGUILDINFO;
}

int CGuildTournamentMgr::GetBattleID( CGuild* pGuild )
{
	if( !pGuild )			return -1;

	for(int i=0; i<MAXGUILD_INTOURNAMENT/2; ++i)
	{
		if( m_BattleInfo[i].MemberInfo.TeamGuildIdx[0] == pGuild->GetIdx() ||
			m_BattleInfo[i].MemberInfo.TeamGuildIdx[1] == pGuild->GetIdx() )
			return m_BattleInfo[i].BattleID;
	}

	return -1;
}

DWORD CGuildTournamentMgr::GetRanking( DWORD Ranking, BOOL bWin )
{
	if( bWin )
	{
		if( m_CurTournament < eGTFight_4 )
			return gRanking[m_CurTournament+1];
		else if( m_CurTournament == eGTFight_4 )
			return 2;									// 1
		else if( m_CurTournament == eGTFight_3_4 )
			return eGTStanding_3RD;
		else if( m_CurTournament == eGTFight_1_2 )
			return eGTStanding_1ST;
	}
	else
	{
		if( m_CurTournament < eGTFight_4 )
			return gRanking[m_CurTournament];
		else if( m_CurTournament == eGTFight_4 )
			return 4;									// 2
		else if( m_CurTournament == eGTFight_3_4 )
			return eGTStanding_4TH;
		else if( m_CurTournament == eGTFight_1_2 )
			return eGTStanding_2ND;
	}

	return eGTStanding_32TH;
}


void CGuildTournamentMgr::SetResult( DWORD GuildIdx, BOOL Win, BOOL bUnearnedWin )
{
	BOOL bevent = FALSE;
	if( m_bEvent )
	{
		for(int i=0; i< MAX_EVENTGT_COUNT; ++i)
		{
			if( m_EventBattleInfo[i].MemberInfo.TeamGuildIdx[0] == GuildIdx ||
				m_EventBattleInfo[i].MemberInfo.TeamGuildIdx[1] == GuildIdx )
				bevent = TRUE;
		}
	}

	if( bevent == FALSE )
	{
		// Update
  		for(int i=0; i<MAXGUILD_INTOURNAMENT; ++i)
  		{
  			if( m_GuildInfoList[i].pGuild == NULL )		continue;  
  			if( m_GuildInfoList[i].pGuild->GetIdx() == GuildIdx )
  			{
  				m_GuildInfoList[i].Ranking = (WORD)GetRanking( m_GuildInfoList[i].Ranking, Win );
				
				if( ( FALSE == Win && m_GuildInfoList[i].Ranking != 4 ) || m_GuildInfoList[i].Ranking == 1 )
				{
					GUILDMGR->GuildTournamentResultToGuildPoint(GuildIdx, m_GuildInfoList[i].Ranking);
				}
				else if( m_GuildInfoList[i].Ranking == 3 )
				{
					GUILDMGR->GuildTournamentResultToGuildPoint(GuildIdx, m_GuildInfoList[i].Ranking);

					for(int j = 0; j < MAXGUILD_INTOURNAMENT; ++ j)
					{
						if( m_GuildInfoList[j].Ranking == 4 )
						{
							GUILDMGR->GuildTournamentResultToGuildPoint(m_GuildInfoList[j].pGuild->GetIdx(), m_GuildInfoList[j].Ranking);
						}
					}
				}

				// To DB
				GuildTournamentGuildInfoUpdate( GuildIdx, m_GuildInfoList[i].Ranking, m_CurTournament+1 );

				// Log To DB
				if( Win )
					GuildTournamentAddLog( m_TournamentCount, GuildIdx, eGTLogKind_Win, m_GuildInfoList[i].Ranking );
				else
					GuildTournamentAddLog( m_TournamentCount, GuildIdx, eGTLogKind_Lose, m_GuildInfoList[i].Ranking );

  				break;
  			}
  		}
	}
	//
	CGuild* pGuild = GUILDMGR->GetGuild( GuildIdx );
	if( !pGuild )			return;

	// Broadcast To Player
	SEND_GTWINLOSE msg;
	SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_NOTIFY_WINLOSE );
	msg.GuildIdx = GuildIdx;
	SafeStrCpy( msg.GuildName, pGuild->GetGuildName(), MAX_GUILD_NAME+1 );
	msg.bWin = Win;
	bevent?msg.Param=eGTFignt_Event:msg.Param=m_CurTournament;
	msg.bUnearnedWin = bUnearnedWin;	
	msg.TournamentCount = (WORD)m_TournamentCount;
	PACKEDDATA_OBJ->SendToBroadCastMapServer( &msg, sizeof(msg) );

	// Broadcast To Mapserver
	g_Network.Broadcast2AgentServer( (char*)&msg, sizeof(msg) );

	// 
	if( m_bEvent )
	{
		for(int i=0; i< MAX_EVENTGT_COUNT; ++i)
		{
			if( m_EventBattleInfo[i].MemberInfo.TeamGuildIdx[0] == GuildIdx )
			{
				m_EventBattleInfo[i].MemberInfo.TeamGuildIdx[0] = 0;
				if( m_EventBattleInfo[i].MemberInfo.TeamGuildIdx[1] == 0 )
				{
					memset( &m_EventBattleInfo[i], 0, sizeof(BATTLE_INFO_GTOURNAMENT) );
					--m_EventBattleCount;
					if( m_EventBattleCount <= 0 )
						m_bEvent = FALSE;
				}
				break;
			}
			else if( m_EventBattleInfo[i].MemberInfo.TeamGuildIdx[1] == GuildIdx )
			{
				m_EventBattleInfo[i].MemberInfo.TeamGuildIdx[1] = 0;
				if( m_EventBattleInfo[i].MemberInfo.TeamGuildIdx[0] == 0 )
				{
					memset( &m_EventBattleInfo[i], 0, sizeof(BATTLE_INFO_GTOURNAMENT) );
					--m_EventBattleCount;
					if( m_EventBattleCount <= 0 )
						m_bEvent = FALSE;
				}
				break;
			}
		}
	}
}


DWORD CGuildTournamentMgr::StartTournamentEvent( MSG_GT_EVENTSTART* pEventInfo )
{
	CGuild* pGuild1 = GUILDMGR->GetGuildIndex( pEventInfo->GuildName1 );
	if( !pGuild1 )
		return eGTEvent_NoGuild;
	CGuild* pGuild2 = GUILDMGR->GetGuildIndex( pEventInfo->GuildName2 );
	if( !pGuild2 )
		return eGTEvent_NoGuild;

	if( m_EventBattleCount >= MAX_EVENTGT_COUNT )
		return eGTEvent_MaxEventOver;

	for(int i=0; i< MAX_EVENTGT_COUNT; ++i)
	{
		for(int k=0; k<eBattleTeam_Max; ++k)			
		{			
			if( m_EventBattleInfo[i].MemberInfo.TeamGuildIdx[k] == pGuild1->GetIdx() ||
				m_EventBattleInfo[i].MemberInfo.TeamGuildIdx[k] == pGuild2->GetIdx() )
				return eGTEvent_AlreadyJoinedGuild;
		}
	}

	for(int i=-0; i<MAX_EVENTGT_COUNT; ++i )
	{
		if( m_EventBattleInfo[i].BattleID != 0 )
			continue;

		memset( &m_EventBattleInfo[i], 0, sizeof(BATTLE_INFO_GTOURNAMENT) );
		m_EventBattleInfo[i].BattleKind		= eBATTLE_KIND_GTOURNAMENT;
		m_EventBattleInfo[i].BattleState		= eBATTLE_STATE_READY;
		m_EventBattleInfo[i].BattleTime		= 0;
		m_EventBattleInfo[i].Ranking			= m_CurTournament;
		m_EventBattleInfo[i].m_dwEntranceTime	= 60000;		// 1분
		m_EventBattleInfo[i].m_dwFightTime		= 600000;		// 10분
		m_EventBattleInfo[i].m_dwLeaveTime		= 20000;		// 20초
		m_EventBattleInfo[i].MemberInfo.TeamGuildIdx[0] = pGuild1->GetIdx();
		m_EventBattleInfo[i].MemberInfo.TeamGuildIdx[1] = pGuild2->GetIdx();

		if( BATTLESYSTEM->CreateBattle( &m_EventBattleInfo[i], GTMAPNUM ) == FALSE )
			return eGTEvent_CreateBattleFailed;

		CBattle_GTournament* pbattle = (CBattle_GTournament*)BATTLESYSTEM->GetBattle( m_EventBattleInfo[i].BattleID );
		if( !pbattle )
			return eGTEvent_CreateBattleFailed;

		// 서버로 입장시키도록 보낸다.
		MSG_DWORD3 msg;
		SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_EVENT_START_ACK );
		msg.dwData1 = pGuild1->GetIdx();
		msg.dwData2 = pGuild2->GetIdx();
		msg.dwData3 = m_EventBattleInfo[i].BattleID;
		PACKEDDATA_OBJ->SendToBroadCastMapServer( &msg, sizeof(msg) );
	    
		++m_EventBattleCount;
		m_bEvent = TRUE;
        break;
	}

	return eGTEvent_Start;
}

void CGuildTournamentMgr::AddPlayerEvent( CPlayer* pPlayer )
{
	CGuild* pGuild = GUILDMGR->GetGuild( pPlayer->GetGuildIdx() );
	DWORD BattleIdx = GetBattleIDEvent( pGuild );
	if( BattleIdx == -1 )		goto _AddEventBattleFailed;
	CBattle_GTournament* pBattle = (CBattle_GTournament*)BATTLESYSTEM->GetBattle( BattleIdx );
	if( !pBattle || pBattle->GetBattleKind() != eBATTLE_KIND_GTOURNAMENT )
		goto _AddEventBattleFailed;

	pPlayer->SetVisible( TRUE );
	if( BATTLESYSTEM->AddObjectToBattle( pBattle, pPlayer ) == TRUE )
	{
		SEND_BATTLESCORE_INFO msg;
		SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_BATTLEJOIN_ACK );
		msg.Score[0] = pBattle->GetTeamMemberNum( eBattleTeam1 );
		msg.Score[1] = pBattle->GetTeamMemberNum( eBattleTeam2 );

		CGuild* pGuild = GUILDMGR->GetGuild( pBattle->GetBattleTeamGuildIdx( eBattleTeam1 ) );
		if( pGuild )
			SafeStrCpy( msg.GuildName[0], pGuild->GetGuildName(), MAX_GUILD_NAME+1 );

		pGuild = GUILDMGR->GetGuild( pBattle->GetBattleTeamGuildIdx( eBattleTeam2 ) );
		if( pGuild )
			SafeStrCpy( msg.GuildName[1], pGuild->GetGuildName(), MAX_GUILD_NAME+1 );

		msg.FightTime = pBattle->GetFightTime();
		msg.EntranceTime = pBattle->GetEntranceTime();
		msg.State = eGTState_Entrance;
		msg.Team = 0;			// 이건 옵저버 나가기 버튼보이기에만 쓰인다.
		pPlayer->SendMsg( &msg, sizeof(msg) );
		return;
	}
		
_AddEventBattleFailed:
	MSG_DWORD msg;
	SetProtocol( &msg, MP_GTOURNAMENT, MP_GTOURNAMENT_RETURNTOMAP );
	msg.dwData = pPlayer->GetReturnMapNum();
	pPlayer->SendMsg( &msg, sizeof(msg) );
	g_pServerSystem->RemovePlayer( pPlayer->GetID() );
	return;
}

void CGuildTournamentMgr::ResetEvent()
{
	for(int i=0; i< m_EventBattleCount; ++i)
	{
		CBattle_GTournament* p = (CBattle_GTournament*)BATTLESYSTEM->GetBattle( m_EventBattleInfo[i].BattleID );
		if( p )
            p->OnDestroy();
	}

	memset( m_EventBattleInfo, 0, sizeof(BATTLE_INFO_GTOURNAMENT)*MAX_EVENTGT_COUNT );
	m_EventBattleCount = 0;
    m_bEvent = FALSE;
}


int CGuildTournamentMgr::GetBattleIDEvent( CGuild* pGuild )
{
	if( !pGuild )			return -1;

	for(int i=0; i<m_EventBattleCount; ++i)
	{
		if( m_EventBattleInfo[i].MemberInfo.TeamGuildIdx[0] == pGuild->GetIdx() ||
			m_EventBattleInfo[i].MemberInfo.TeamGuildIdx[1] == pGuild->GetIdx() )
			return m_EventBattleInfo[i].BattleID;
	}

	return -1;
}

//SW061205 깃발 NPC
void CGuildTournamentMgr::CheckDateforFlagNPC()
{
	//요일 체크
	static SYSTEMTIME st;
	GetLocalTime(&st);

	if( st.wDayOfWeek != m_wCurDay ) //요일 갱신이면
	{
		//깃발정보를 AgentSrv를 통해 전파
		MSG_DWORD msg;
		msg.Category = MP_GTOURNAMENT;
		msg.Protocol = MP_GTOURNAMENT_FLAGCHANGE;
		msg.dwData = GAMERESRCMNGR->GetFlagFromDate(eGTFlg, st.wDayOfWeek);

		g_Network.Broadcast2AgentServer( (char*)&msg, sizeof(msg) );

		m_wCurDay = st.wDayOfWeek;
	}
}
