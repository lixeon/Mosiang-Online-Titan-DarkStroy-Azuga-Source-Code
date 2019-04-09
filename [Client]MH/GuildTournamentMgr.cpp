// GuildTournamentMgr.cpp: implementation of the CGuildTournamentMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildTournamentMgr.h"
#include "GameIn.h"
#include "MapChange.h"
#include "MainGame.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"
#include "../Interface/cWindowManager.h"
#include "ChatManager.h"
#include "WindowIDEnum.h"
#include "UserInfoManager.h"
#include "cMsgBox.h"
#include "GTRegistDialog.h"
#include "GTRegistcancelDialog.h"
#include "GTBattleListDialog.h"
#include "GTStandingDialog.h"
#include "GTScoreInfoDialog.h"
#include "MiniMapDlg.h"
#include "./Input/UserInput.h"
#include "MHMap.h"
#include "GTournament/Battle_GTournament.h"
#include "BattleSystem_Client.h"



extern DWORD gRanking[eGTT_MAX];
//

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuildTournamentMgr::CGuildTournamentMgr()
{
}

CGuildTournamentMgr::~CGuildTournamentMgr()
{

}


void CGuildTournamentMgr::AddPlayer( CPlayer* pPlayer )
{
	CBattle* pBattle = BATTLESYSTEM->GetBattle( HERO );
	if( !pBattle )		return;

	if( pBattle->GetBattleKind() == eBATTLE_KIND_GTOURNAMENT )
	{
/*		DWORD BattleTeamID = pPlayer->GetBattleTeam();
		printf("AddPlayer - %d\n", BattleTeamID);
		if( BattleTeamID == eBattleTeam1 )
		{
			OBJECTEFFECTDESC desc(FindEffectNum("eff_bmu_A_s.beff"));
			pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1 );
		}		
		else if( BattleTeamID == eBattleTeam2 )
		{
			OBJECTEFFECTDESC desc(FindEffectNum("eff_bmu_B_s.beff"));
			pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1 );
		}
		else
		{
			pPlayer->GetCharacterTotalInfo()->bVisible = false;
		}
		
		if( pBattle->GetBattleState() == eBATTLE_STATE_READY )
		{		
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
		}*/
	}
}


void CGuildTournamentMgr::NetworkMsgParse( BYTE Protocol,void* pMsg )
{
	switch( Protocol )
	{
	case MP_GTOURNAMENT_REGISTCOUNT:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			if( pmsg->dwData == 999 )			// 신청기간이 아니다
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

				WINDOWMGR->MsgBox( MBI_GTREGIST_NACK, MBT_OK, CHATMGR->GetChatMsg(962) );
				return;
			}

			if( GAMEIN->GetGTRegistDlg() )
			{
				GAMEIN->GetGTRegistDlg()->SetActive( TRUE );
				GAMEIN->GetGTRegistDlg()->SetRegistGuildCount( pmsg->dwData );
			}
		}
		break;
	case MP_GTOURNAMENT_REGIST_ACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);


			if( GAMEIN->GetGTRegistDlg() )
			{
				if( GAMEIN->GetGTRegistDlg()->IsActive() )
					GAMEIN->GetGTRegistDlg()->SetActive( FALSE );
				GAMEIN->GetGTRegistDlg()->SetRegistGuildCount( pmsg->dwData1 );
			}

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(972), pmsg->dwData2 );
		}
		break;
	case MP_GTOURNAMENT_REGIST_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			char buf[256];
			switch( pmsg->dwData )
			{
			case eGTError_NOTREGISTDAY:
				sprintf( buf, CHATMGR->GetChatMsg(962) );
				WINDOWMGR->MsgBox( MBI_GTREGIST_NACK, MBT_OK, buf );
				break;
			case eGTError_DONTFINDGUILDINFO:
				break;
			case eGTError_MAXGUILDOVER:
				sprintf( buf, CHATMGR->GetChatMsg(963) );
				WINDOWMGR->MsgBox( MBI_GTREGIST_NACK, MBT_OK, buf );
				break;
			case eGTError_ALREADYREGISTE:
				sprintf( buf, CHATMGR->GetChatMsg(961), HERO->GetGuildName() );
				WINDOWMGR->MsgBox( MBI_GTREGIST_NACK, MBT_OK, buf );
				break;
			case eGTError_ALREADYCANCELREGISTE:				
				sprintf( buf, CHATMGR->GetChatMsg(988), HERO->GetGuildName() );
				WINDOWMGR->MsgBox( MBI_GTREGIST_NACK, MBT_OK, buf );				
				break;
			case eGTError_UNDERLIMITEMEMBER:
				sprintf( buf, CHATMGR->GetChatMsg(952), LIMITMEMBER_GUILDTOURNAMENT );
				CHATMGR->AddMsg( CTC_SYSMSG, buf );
				break;
			case eGTError_UNDERLEVEL:
				sprintf( buf, CHATMGR->GetChatMsg(951), LIMITLEVEL_GUILDTOURNAMENT );
				CHATMGR->AddMsg( CTC_SYSMSG, buf );
				break;
			case eGTError_DELETEAPPLIED:
				break;
			case eGTError_NOGUILDMASTER:
				sprintf( buf, CHATMGR->GetChatMsg(950) );
				CHATMGR->AddMsg( CTC_SYSMSG, buf );
				break;
			case eGTError_DONTFINDPOSITION:
				break;
			case eGTError_DONTFINDBATTLE:
				break;
			default:
				break;
			}

			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			if( GAMEIN->GetGTRegistDlg() )
			{
				if( GAMEIN->GetGTRegistDlg()->IsActive() )
					GAMEIN->GetGTRegistDlg()->SetActive( FALSE );
			}
		}
		break;
	case MP_GTOURNAMENT_REGISTCANCEL_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			WINDOWMGR->MsgBox( MBI_GTREGIST_NACK, MBT_OK, CHATMGR->GetChatMsg(966) );

			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			if( GAMEIN->GetGTRegistcancelDlg() )
			{
				if( GAMEIN->GetGTRegistcancelDlg()->IsActive() )
					GAMEIN->GetGTRegistcancelDlg()->SetActive( FALSE );
			}
			
			GAMEIN->SetGuildIdx( 0 );
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(973), pmsg->dwData );
		}
		break;
	case MP_GTOURNAMENT_REGISTCANCEL_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			char buf[256];
			switch( pmsg->dwData )
			{
			case eGTError_NOTREGISTCANCELDAY:
				sprintf( buf, CHATMGR->GetChatMsg(965) );
				WINDOWMGR->MsgBox( MBI_GTREGIST_NACK, MBT_OK, buf );
				break;
			default:
				break;
			}

			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			if( GAMEIN->GetGTRegistcancelDlg() )
			{
				if( GAMEIN->GetGTRegistcancelDlg()->IsActive() )
					GAMEIN->GetGTRegistcancelDlg()->SetActive( FALSE );
			}
		}
		break;
	case MP_GTOURNAMENT_MOVETOBATTLEMAP_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			
			DWORD Mapnum = GTMAPNUM;
			if( pmsg->dwData == 0 )
				MAPCHANGE->SetGameInInitKind( eGameInInitKind_GTEnter );
			else
				MAPCHANGE->SetGameInInitKind( eGameInInitKind_GTObserverEnter );
			MAINGAME->SetGameState( eGAMESTATE_MAPCHANGE, &Mapnum, 4 );
			OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
			GAMEIN->SetGuildIdx( HERO->GetGuildIdx() );
			GAMEIN->SetMoveMap( MAP->GetMapNum() );
			GAMEIN->SetBattleIdx( pmsg->dwData );
			CHATMGR->SaveChatList();
			
			WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg(989));
		}
		break;
	case MP_GTOURNAMENT_MOVETOBATTLEMAP_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			switch( pmsg->dwData )
			{
			case eGTError_DONTFINDBATTLE:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(967) );
				break;
			case eGTError_DONTFINDGUILDINFO:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(968) );
				break;
			case eGTError_NOTENTERTIME:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(976) );
				break;
			case eGTError_STUDENTCANTENTER:
			default:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(967) );
				break;
			}

			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);		
		}
		break;
	case MP_GTOURNAMENT_BATTLEJOIN_ACK:
		{
			SEND_BATTLESCORE_INFO* pmsg = (SEND_BATTLESCORE_INFO*)pMsg;

			if( GAMEIN->GetGTScoreInfoDlg() )
			{
				GAMEIN->GetGTScoreInfoDlg()->SetActive( TRUE );
				GAMEIN->GetMiniMapDialog()->SetActive( FALSE );
				GAMEIN->GetGTScoreInfoDlg()->SetBattleInfo( pmsg );
				
				if( pmsg->State == eGTState_Process )
					GAMEIN->GetGTScoreInfoDlg()->StartBattle();		
				
				if( pmsg->Team == 2 )
					GAMEIN->GetGTScoreInfoDlg()->ShowOutBtn( TRUE );
				else
					GAMEIN->GetGTScoreInfoDlg()->ShowOutBtn( FALSE );
			}
		}
		break;
	case MP_GTOURNAMENT_RETURNTOMAP:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			GAMEIN->GetMiniMapDialog()->SetActive( TRUE );

			MAPCHANGE->SetGameInInitKind( eGameInInitKind_GTLeave );
			DWORD MapNum = pmsg->dwData;
			MAINGAME->SetGameState( eGAMESTATE_MAPCHANGE, &MapNum, 4 );
			CHATMGR->SaveChatList();
		}
		break;
	case MP_GTOURNAMENT_STANDINGINFO_REGISTED:
		{
			SEND_REGISTEDGUILDINFO* pmsg = (SEND_REGISTEDGUILDINFO*)pMsg;
			
			if( GAMEIN->GetGTStandingDlg() )
			{
				GAMEIN->GetGTStandingDlg()->ResetAll();

				for(int i=0; i<pmsg->Count; ++i)
					GAMEIN->GetGTStandingDlg()->AddGuildInfo( &pmsg->GInfo[i] );
				
				GAMEIN->GetGTStandingDlg()->SetCurTournament( pmsg->CurTournament );
				GAMEIN->GetGTStandingDlg()->RefreshGuildInfo();
				GAMEIN->GetGTStandingDlg()->SetActive( TRUE );
			}
		}
		break;
	case MP_GTOURNAMENT_STANDINGINFO_ACK:
		{
			SEND_REGISTEDGUILDINFO* pmsg = (SEND_REGISTEDGUILDINFO*)pMsg;

			if( GAMEIN->GetGTStandingDlg() )
			{
				for(int i=0; i<pmsg->Count; ++i)
					GAMEIN->GetGTStandingDlg()->AddGuildInfo( &pmsg->GInfo[i] );

				GAMEIN->GetGTStandingDlg()->RefreshGuildInfo();
				GAMEIN->GetGTStandingDlg()->SetActive( TRUE );
			}
		}
		break;
	case MP_GTOURNAMENT_STANDINGINFO_NACK:
		{
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(996) );
		}
		break;
	case MP_GTOURNAMENT_BATTLEINFO_ACK:
		{
			SEND_GTBATTLE_INFO* pmsg = (SEND_GTBATTLE_INFO*)pMsg;

			if( GAMEIN->GetGTBattleListDlg() )
			{
				GAMEIN->GetGTBattleListDlg()->DeleteAllBattleInfo();
				GAMEIN->GetGTBattleListDlg()->SetPlayOff( pmsg->PlayOff );
				for(int i=0; i<pmsg->Count; ++i)
					GAMEIN->GetGTBattleListDlg()->AddBattleInfo( &pmsg->BattleInfo[i] );

				GAMEIN->GetGTBattleListDlg()->RefreshBattleList();
				GAMEIN->GetGTBattleListDlg()->SetActive( TRUE );
			}
			
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
		}
		break;
	case MP_GTOURNAMENT_WIN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			
			CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(970) );
		}
		break;
	case MP_GTOURNAMENT_LOSE:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(971) );
		}
		break;
	case MP_GTOURNAMENT_BATTLE_ENTER:
		{
			// 입장하세요.
			CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(975) );
		}
		break;
	case MP_GTOURNAMENT_TEAMMEMBER_ADD:
	case MP_GTOURNAMENT_TEAMMEMBER_OUT:
	case MP_GTOURNAMENT_TEAMMEMBER_DIE:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			if( GAMEIN->GetGTScoreInfoDlg() )
			{
				GAMEIN->GetGTScoreInfoDlg()->SetTeamScore( pmsg->dwData1, pmsg->dwData2 ); 
			}
		}		
		break;
	case MP_GTOURNAMENT_NOTIFY_WINLOSE:
		{
			SEND_GTWINLOSE* pmsg = (SEND_GTWINLOSE*)pMsg;

			// WIN, LOSE
			DWORD win = pmsg->bWin==1?980:981;
			// 부전승
			if( pmsg->bUnearnedWin )
				win = 979;

			if( pmsg->Param == eGTFignt_Event )
			{
				CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(1459), pmsg->GuildName, CHATMGR->GetChatMsg(win) );
				return;
			}

			if( pmsg->Param <= eGTFight_4 )
				CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(977), pmsg->GuildName, pmsg->TournamentCount, gRanking[pmsg->Param], CHATMGR->GetChatMsg(win) );
			else if( pmsg->Param == eGTFight_3_4 )
			{
				if( pmsg->bWin || pmsg->bUnearnedWin )
					CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(994), pmsg->GuildName, pmsg->TournamentCount, 3 );
				else
					CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(994), pmsg->GuildName, pmsg->TournamentCount, 4 );
			}
			else if( pmsg->Param == eGTFight_1_2 )
			{
				if( pmsg->bWin || pmsg->bUnearnedWin )
					CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(992), pmsg->GuildName, pmsg->TournamentCount );
				else
					CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(993), pmsg->GuildName, pmsg->TournamentCount );
			}
		}
		break;
	case MP_GTOURNAMENT_BATTLE_NOTIFY:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

			DWORD time = pmsg->dwData2;
			time /= 60000;
			if( time == 0 )			return;

			if( pmsg->dwData1 <= eGTFight_4 )
				CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(982), pmsg->dwData3, gRanking[pmsg->dwData1], time );
			else if( pmsg->dwData1 == eGTFight_3_4 )
				CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(983), pmsg->dwData3, 3, 4, time );
			else if( pmsg->dwData1 == eGTFight_1_2 )
				CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(983), pmsg->dwData3, 1, 2, time );			
		}
		break;
	case MP_GTOURNAMENT_STARTBATTLE:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;		

			if( pmsg->dwData1 <= eGTFight_4 )
				CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(984), pmsg->dwData2, gRanking[pmsg->dwData1] );
			else if( pmsg->dwData1 == eGTFight_3_4 )
				CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(985), pmsg->dwData2, 3, 4 );
			else if( pmsg->dwData1 == eGTFight_1_2 )
				CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(985), pmsg->dwData2, 1, 2 );
		}
		break;
	case MP_GTOURNAMENT_ENDBATTLE:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			if( pmsg->dwData1 <= eGTFight_4 )
				CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(986), pmsg->dwData2, gRanking[pmsg->dwData1] );
			else if( pmsg->dwData1 == eGTFight_3_4 )
				CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(987), pmsg->dwData2, 3, 4 );
			else if( pmsg->dwData1 == eGTFight_1_2 )
				CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(987), pmsg->dwData2, 1, 2 );
			else if( pmsg->dwData1 == eGTFight_End )
				CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(990), pmsg->dwData2 );
		}
		break;
	case MP_GTOURNAMENT_EVENT_START_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			DWORD Mapnum = GTMAPNUM;
			if( pmsg->dwData == 0 )
				MAPCHANGE->SetGameInInitKind( eGameInInitKind_GTEnter );
			else
				MAPCHANGE->SetGameInInitKind( eGameInInitKind_GTObserverEnter );
			MAINGAME->SetGameState( eGAMESTATE_MAPCHANGE, &Mapnum, 4 );
			OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
			GAMEIN->SetGuildIdx( HERO->GetGuildIdx() );
			GAMEIN->SetMoveMap( MAP->GetMapNum() );
			GAMEIN->SetBattleIdx( pmsg->dwData );
			CHATMGR->SaveChatList();

			WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg(989));
		}
		break;
	case MP_GTOURNAMENT_EVENT_START_RESULT:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			if( pmsg->dwData2 == eGTEvent_NoGuild )
				CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(1460) );
			else if( pmsg->dwData2 == eGTEvent_AlreadyJoinedGuild )
				CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(961) );
			else if( pmsg->dwData2 == eGTEvent_CreateBattleFailed )
				CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(1461) );
			else if( pmsg->dwData2 == eGTEvent_MaxEventOver )
				CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(1462)  );			
			else if( pmsg->dwData2 == eGTEvent_Start )
                CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(1463) );
		}
		break;
	case MP_GTOURNAMENT_CHEAT:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

			printf("TournamentState[%d] Guild[%d]\n", pmsg->dwData2, pmsg->dwData3);
		}
		break;
	case MP_GTOURNAMENT_FLAGCHANGE:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			OBJECTMGR->ChangeFlagNPC(eGTFlg, pmsg->dwData);
		}
		break;
	/*case MP_GTOURNAMENT_FLAGCHANGE:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			if( eGTState_BeforeRegist )
			{
				OBJECTMGR->ChangeFlag( pmsg->dwData1, pmsg->dwData2 );
			}
			else if( eGTState_Regist )
			{
				OBJECTMGR->ChangeFlag( pmsg->dwData1, pmsg->dwData2 );
			}
		}
		break;*/
	}
}
