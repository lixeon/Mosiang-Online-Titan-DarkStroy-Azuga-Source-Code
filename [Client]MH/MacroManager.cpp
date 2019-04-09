// MacroManager.cpp: implementation of the CMacroManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#ifdef _CHEATENABLE_
#include "CheatMsgParser.h"
#include "CommonGameFunc.h"
#endif	//_CHEATENABLE_

#ifdef _GMTOOL_
#include "GMToolManager.h"
#include "EventMapInfo.h"
#endif

#include <direct.h>
#include "WindowIdEnum.h"
#include "./interface/cWindowManager.h"
#include "../GlobalEventFunc.h"
#include "MacroManager.h"
#include "GameIn.h"
#include "MurimNet.h"
#include "QuickManager.h"
#include "MoveManager.h"
#include "PeaceWarModeManager.h"
#include "UngijosikManager.h"
#include "AbilityManager.h"

#include "ObjectManager.h"
/*#include "PyogukManager.h"
#include "FriendManager.h"
*/
#include "WantedManager.h"

#include "MainGame.h"
#include "MHCamera.h"
//#include "MinimapDialog.h"


#include "ChatManager.h"
//050630SW
#include "cChatTipManager.h"
//-
#include "./input/cIMEWnd.h"

#include "./input/UserInput.h"
#include "HelpDicManager.h"

#include "VimuManager.h"
#include "StreetStallManager.h"
#include "ExchangeManager.h"
#include "MouseCursor.h"

#include "HelpDialog.h"
//#include "PyogukDialog.h"
//#include "NoteDialog.h"
//#include "FriendDialog.h"
#include "QuickDialog.h"
#include "MainBarDialog.h"
#include "CharacterDialog.h"
#include "InventoryExDialog.h"
#include "OptionDialog.h"
#include "ReviveDialog.h"
#include "DissolutionDialog.h"
#include "GuildCreateDialog.h"
#include "GuildDialog.h"
#include "GuildInviteDialog.h"
#include "GuildMarkDialog.h"
#include "GuildNickNameDialog.h"
#include "GuildRankDialog.h"
#include "GuildWarehouseDialog.h"
#include "GuildManager.h"
#include "GuildMarkManager.h"
#include "GuildTraineeDialog.h"
#include "TitanPartsMakeDlg.h"		// magi82 - Titan(070112)
#include "TitanMixDlg.h"			// magi82 - Titan(070119)
#include "TitanRepairDlg.h"			// 2007. 9. 11. CBH
#include "TitanUpgradeDlg.h"		// magi82 - Titan(070119)
#include "TitanBreakDlg.h"			// magi82 - Titan(070119)
#include "TitanInventoryDlg.h"		// magi82 - Titan(070222)
#include "TitanGuageDlg.h"			// magi82 - Titan(070305)
//#include "TitanMugongMixDlg.h"		// magi82 - Titan(070611) 타이탄 무공변환 주석처리
#include "TitanRegisterDlg.h"		// magi82 - Titan(070320)
#include "TitanDissolutionDlg.h"	// magi82 = Titan(070321)

#include "BattleSystem_Client.h"
#include "PKManager.h"
#include "SkillManager_Client.h"
#include "QuestTotaldialog.h"
#include "QuestManager.h"
#include "MiniMapDlg.h"
#include "BigmapDlg.h"
#include "BailDialog.h"
#include "WindowIDEnum.h"
#include "cMsgBox.h"
#include "PartyIconManager.h"
#include "WantNpcDialog.h"
#include "FreeImage/FreeImage.h"

#include "ItemShopDialog.h"
#include "StatusIconDlg.h"
#include "MoveDialog.h"
#include "PointSaveDialog.h"
#include "ItemManager.h"
#include "ObjectStateManager.h"

#include "NoteDialog.h"
#include "SkillPointRedist.h"
#include "ShoutchatDialog.h"
#include "AppearanceManager.h"
#include "ChaseDialog.h"
#include "ChaseinputDialog.h"
#include "ShoutDialog.h"
#include "NameChangeNotifyDlg.h"
#include "EventNotifyDialog.h"
// Guild Tournament
#include "GTRegistDialog.h"
#include "GTRegistcancelDialog.h"
#include "GTStandingDialog.h"
#include "GTBattleListDialog.h"
#include "GTScoreInfoDialog.h"
#include "ReinforceResetDlg.h"

#include "SealDialog.h"
#include "SeigeWarDialog.h"
#include "SiegeWarMgr.h"
#include "MugongSuryunDialog.h"
#include "MussangManager.h"
#include "PetUpgradeDialog.h"
#include "PetRevivalDialog.h"
#include "KeySettingTipDlg.h"

#include "UniqueItemCurseCancellationDlg.h"	// magi82 - UniqueItem(070703)
#include "UniqueItemMixDlg.h"	// magi82 - UniqueItem(070709)

#include "SOSDialog.h"

#include "SkillPointResetDlg.h"

//screenshot
#include "ScreenShotDlg.h"
#include "AutoNoteDlg.h"
#include "AutoAnswerDlg.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern BOOL g_bActiveApp;

#ifdef _TESTCLIENT_
#include "MHMap.h"
#include "ObjectManager.h"
#include "EFFECT/EffectManager.h"
#include "TacticManager.h"
#include "GameResourceManager.h"
#include "interface/cFont.h"
void InitTestHero();
void InitTestMonster();
void InitTestCharacters(void);
#endif


extern BOOL g_bDisplayFPS;
extern int g_nServerSetNum;

GLOBALTON(CMacroManager)

CMacroManager::CMacroManager()
{
	ZeroMemory( m_MacroKey, sizeof( m_MacroKey ) );
	ZeroMemory( m_DefaultKey, sizeof( m_DefaultKey ) );

	ZeroMemory( m_MacroPressKey, sizeof( m_MacroPressKey ) );
	ZeroMemory( m_DefaultPressKey, sizeof( m_DefaultPressKey ) );

//	m_nMacroMode	= MM_DEFAULT_GENERAL;
//	m_nUserSetMode	= 0;	//CHAT
	m_nMacroMode	= MM_CHAT;
	m_bChatMode		= TRUE;
	m_bPause		= FALSE;
	m_bInVisibleCursor = FALSE;
}

CMacroManager::~CMacroManager()
{
}


void CMacroManager::KeyboardInput( CKeyboard* keyInfo )
{
	if( !g_bActiveApp || m_bPause )	return;
	
	if(HEROID == 0)
		return;

	if( MAINGAME->GetCurStateNum() != eGAMESTATE_GAMEIN ) return;	//AOAuE­ ¹UA¸·I≫≪´U confirm

	//f12를 누를경우 시계 다이얼로그를 보인다
	//키클릭후 랜더링이 작동하므로 키다운시 렌더링하고 키업시 스크린샷 저장
	if (keyInfo->GetKeyPressed(88) == 1) 
	{
		// 일본 요청에 의해...
#ifndef _JAPAN_LOCAL_
        GAMEIN->GetScreenShotDlg()->SetActive( TRUE, gHeroID );  // 스크린캡쳐 시간dlg 
#endif
	}

	BOOL bDock = FALSE;
	if( CIMEWND->IsDocking() )
	{
		if( m_nMacroMode == MM_MACRO )
			//return;
			bDock = TRUE;
	}
	
	int nSysKey = MSK_NONE;

	if( keyInfo->GetKeyPressed( KEY_MENU ) )
	{
		nSysKey = 0;		//alt
		nSysKey |= MSK_ALT;
	}
	if( keyInfo->GetKeyPressed( KEY_CONTROL ) )
	{
		nSysKey &= ~MSK_NONE;
		nSysKey |= MSK_CTRL;
	}
	if( keyInfo->GetKeyPressed( KEY_SHIFT ) )
	{
		nSysKey &= ~MSK_NONE;
		nSysKey |= MSK_SHIFT;
	}

	
	for( int i = 0 ; i < ME_COUNT ; ++i )
	{
		if( bDock && !m_MacroKey[m_nMacroMode][i].bAllMode )
			continue;		

		if( m_MacroKey[m_nMacroMode][i].nSysKey & nSysKey )
		{
			if( m_MacroKey[m_nMacroMode][i].bUp )
			{
				if( keyInfo->GetKeyUp( m_MacroKey[m_nMacroMode][i].wKey ) )
					PlayMacro( i );
			}
			else
			{
				if( keyInfo->GetKeyDown( m_MacroKey[m_nMacroMode][i].wKey ) )
					PlayMacro( i );
			}
		}
	}


#ifdef _GMTOOL_
	if( ( keyInfo->GetKeyDown(KEY_RETURN) || keyInfo->GetKeyDown(KEY_PADENTER) ) && keyInfo->GetKeyPressed(KEY_MENU) )
	{
		if( GMTOOLMGR->IsShowing() )
			GMTOOLMGR->ShowGMDialog( FALSE, TRUE );
		else
			GMTOOLMGR->ShowGMDialog( TRUE, TRUE );
	}
	else if( keyInfo->GetKeyDown(KEY_SUBTRACT) && keyInfo->GetKeyPressed(KEY_MENU) )
	{
		MSGBASE msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_AGENTCHECK_SYN;
		msg.dwObjectID	= HEROID;
		NETWORK->Send( &msg, sizeof(msg) );
	}
	
#endif


#ifdef _CHEATENABLE_
/*	
//========= 해킹테스트
//	case eGameInInitKind_Login:
//	case eGameInInitKind_MapChange:
	if( keyInfo->GetKeyDown(KEY_1) && keyInfo->GetKeyPressed(KEY_MENU) && keyInfo->GetKeyPressed(KEY_CONTROL) )
	{
		MSG_DWORD msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_GAMEIN_SYN;
		msg.dwData	 = gChannelNum;
		NETWORK->Send( &msg, sizeof(msg) );
	}

//	case eGameInInitKind_SuryunEnter:
	if( keyInfo->GetKeyDown(KEY_2) && keyInfo->GetKeyPressed(KEY_MENU) && keyInfo->GetKeyPressed(KEY_CONTROL) )
	{
		MSGBASE msg;
		msg.Category = MP_SURYUN;
		msg.Protocol = MP_SURYUN_ENTER_SYN;
		msg.dwObjectID = HEROID;
		NETWORK->Send( &msg, sizeof(msg) );
	}

//	case eGameInInitKind_SuryunLeave:
	if( keyInfo->GetKeyDown(KEY_3) && keyInfo->GetKeyPressed(KEY_MENU) && keyInfo->GetKeyPressed(KEY_CONTROL) )
	{
		MSGBASE msg;
		msg.Category = MP_SURYUN;
		msg.Protocol = MP_SURYUN_LEAVE_SYN;
		msg.dwObjectID = HEROID;
		NETWORK->Send( &msg, sizeof(msg) );
	}
		
//	case eGameInInitKind_MovePos:
	if( keyInfo->GetKeyDown(KEY_4) && keyInfo->GetKeyPressed(KEY_MENU) && keyInfo->GetKeyPressed(KEY_CONTROL) )
	{
		MSG_DWORD2 msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_GAMEINPOS_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = gChannelNum;
		msg.dwData2 = 1;

		NETWORK->Send( &msg, sizeof(msg) );			
	}

//	case eGameInInitKind_EventMapEnter:
	if( keyInfo->GetKeyDown(KEY_5) && keyInfo->GetKeyPressed(KEY_MENU) && keyInfo->GetKeyPressed(KEY_CONTROL) )
	{
		MSG_DWORD3 msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_ENTER_EVENTMAP_SYN;
		msg.dwObjectID = HEROID;
		msg.dwData1 = EVENTMAP->GetEventMapNum();
		msg.dwData2 = EVENTMAP->GetChannelNum();
		msg.dwData3 = EVENTMAP->GetTeamNum();
		NETWORK->Send( &msg, sizeof(msg) );
	}
//================
*/

#ifdef _TESTCLIENT_

	if(keyInfo->GetKeyDown(KEY_1) && keyInfo->GetKeyPressed(KEY_CONTROL))
	{
		SKILLMGR->Release();
		SKILLMGR->Init();
	}
	if(keyInfo->GetKeyDown(KEY_2) && keyInfo->GetKeyPressed(KEY_CONTROL))
	{
		EFFECTMGR->Release();
		EFFECTMGR->Init();
	}
	if(keyInfo->GetKeyDown(KEY_3) && keyInfo->GetKeyPressed(KEY_CONTROL))
	{
		EFFECTMGR->RemoveAllEffect();
		GAMERESRCMNGR->LoadTestClientInfo();
		OBJECTMGR->RemoveAllObject();
		GAMERESRCMNGR->ResetMonsterList();
		GAMERESRCMNGR->LoadMonsterList();
//		GAMERESRCMNGR->LoadSMonsterList();
		InitTestHero();
		InitTestMonster();
		InitTestCharacters();
	}
	if(keyInfo->GetKeyDown(KEY_4) && keyInfo->GetKeyPressed(KEY_CONTROL))
	{
		MAP->LoadMapDesc(MAP->GetMapNum());
		MAP->ApplyMapDesc();
	}
	if(keyInfo->GetKeyDown(KEY_ESCAPE))
	{
		EFFECTMGR->SetEndFlagAllEffect();
	}
	if(keyInfo->GetKeyDown(KEY_SUBTRACT))
	{
		MAINGAME->SetGameState( eGAMESTATE_MURIMNET );
	}

	static BOOL bWireFrame = FALSE;
	if(keyInfo->GetKeyDown(KEY_F12) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		if( !bWireFrame )
		{			
			g_pExecutive->GetRenderer()->SetRenderMode(2);
			bWireFrame = TRUE;
		}
		else
		{
			g_pExecutive->GetRenderer()->SetRenderMode(0);
			bWireFrame = FALSE;
		}
	}

	if(keyInfo->GetKeyDown(KEY_R) && keyInfo->GetKeyPressed(KEY_MENU))	
	{
		/*
		HERO->GetEngineObject()->ChangeMotion(eMotion_Item_Teleport_1, FALSE);		
		OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_ItemUse);
		OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_ItemUse, HERO->GetEngineObject()->GetAnimationTime(eMotion_Item_Teleport_1)) ;
		HERO->SetItemUseMotionIdx( eMotion_Item_Teleport_1 );*/
//		EFFECTMGR->StartHeroEffectProcess(FindEffectNum("Mall_eff_heal.beff"));	
//		EFFECTMGR->StartHeroEffectProcess(FindEffectNum("maintain_eff_jackpot.beff"));	

//		EFFECTMGR->StartEffectProcess(eEffect_LevelUpSentence,HERO,NULL,0,0);
		EFFECTMGR->StartEffectProcess(1034,HERO,NULL,0,0);
//		EFFECTMGR->StartHeroEffectProcess(FindEffectNum("m_ba_080.beff"));
//		EFFECTMGR->StartEffectProcess(eEffect_Jackpot,HERO,NULL,0,0);

//		OBJECTEFFECTDESC desc(FindEffectNum("maintain_eff_jackpot.beff"));
//		pPlayer->AddObjectEffect( JACKPOT_EFFECTID, &desc, 1 );
		
	}

	if(keyInfo->GetKeyDown(KEY_4) && keyInfo->GetKeyPressed(KEY_MENU))	
	{
		//OBJECTEFFECTDESC desc(eEffect_ShopItem_Life);
		//HERO->AddObjectEffect(STATE_EFFECT_ID,&desc,1);
		EFFECTMGR->StartHeroEffectProcess(FindEffectNum("Mall_eff_heal.beff"));	
		//EFFECTMGR->StartHeroEffectProcess(eEffect_ShopItem_Life);
	}
	if(keyInfo->GetKeyDown(KEY_5) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		//EFFECTMGR->StartHeroEffectProcess(eEffect_ShopItem_Life);
		EFFECTMGR->StartHeroEffectProcess(FindEffectNum("m_ba_070.beff"));
	}
	if(keyInfo->GetKeyDown(KEY_6) && keyInfo->GetKeyPressed(KEY_MENU))	
	{
		EFFECTMGR->StartHeroEffectProcess(eEffect_ShopItem_Shield);
	}
	if(keyInfo->GetKeyDown(KEY_7) && keyInfo->GetKeyPressed(KEY_MENU))	
	{
		EFFECTMGR->StartHeroEffectProcess(eEffect_ShopItem_Revive);
	}
	if(keyInfo->GetKeyDown(KEY_8) && keyInfo->GetKeyPressed(KEY_MENU))	
	{
		EFFECTMGR->StartHeroEffectProcess(GAMERESRCMNGR->m_TestClientInfo.nTestEffectNum);
	}
	
	
	if(keyInfo->GetKeyDown(KEY_1) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		CMoveDialog* pdlg = GAMEIN->GetMoveDialog();
		pdlg->SetActive(!pdlg->IsActive());
	}
	if(keyInfo->GetKeyDown(KEY_2) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		CItemShopDialog* pdlg = GAMEIN->GetItemShopDialog();
		pdlg->SetActive(!pdlg->IsActive());
	}
	if(keyInfo->GetKeyDown(KEY_3) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		CPointSaveDialog* pdlg = GAMEIN->GetPointSaveDialog();
		pdlg->SetActive(!pdlg->IsActive());
	}



	// jsd test
/*	
	if( keyInfo->GetKeyDown( KEY_Z ) )
	{
		MSG_BATTLE_INFO msg;
		BATTLE_INFO_BASE info;
		info.BattleID = 1;
		info.BattleKind = eBATTLE_KIND_MURIMFIELD;
		info.BattleState = 0;
		info.BattleTime = 0;
		BATTLESYSTEM->CreateBattle( &info );
	}
*/
	if( keyInfo->GetKeyDown(KEY_K) )
	{
		GAMEIN->GetAutoNoteDlg()->SetActiveTestClient();
	}
	if( keyInfo->GetKeyDown(KEY_L) )
	{
		GAMEIN->GetAutoAnswerDlg()->SetActiveTestClient();
	}
#endif //_TESTCLIENT_



#ifndef _GMTOOL_
	if( !CHEATMGR->IsCheatEnable() ) return;
#endif

	if(keyInfo->GetKeyDown(KEY_G) && keyInfo->GetKeyPressed(KEY_CONTROL))
	{
		CPetUpgradeDialog* pdlg = GAMEIN->GetPetUpgradeDialog();
		if( pdlg )
			pdlg->SetActive( !pdlg->IsActive() );
	}

	if(keyInfo->GetKeyDown(KEY_R) && keyInfo->GetKeyPressed(KEY_CONTROL))
	{
		CPetRevivalDialog* pdlg = GAMEIN->GetPetRevivalDialog();
		if( pdlg )
			pdlg->SetActive( !pdlg->IsActive() );
	}

	if(keyInfo->GetKeyDown(KEY_D) && keyInfo->GetKeyPressed(KEY_CONTROL))
	{
		if(GAMEIN->GetSOSDlg()->IsActive() == TRUE)
		{
			GAMEIN->GetSOSDlg()->SetActive(FALSE);
		}
		else
		{
			GAMEIN->GetSOSDlg()->SetActive(TRUE);
		}
	}
	
	if(keyInfo->GetKeyDown(KEY_F1) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		g_bDisplayFPS = !g_bDisplayFPS;
		//GAMEIN->GetUpgradeDialog()->SetActiveRecursive(GAMEIN->GetUpgradeDialog()->IsActive()^TRUE);
	}
#ifdef _TESTCLIENT_
	if(keyInfo->GetKeyDown(KEY_F1) && keyInfo->GetKeyPressed(KEY_CONTROL))
	{
		g_bDisplayFPS = !g_bDisplayFPS;
	}
#endif
	//if(keyInfo->GetKeyDown(KEY_F2) && keyInfo->GetKeyPressed(KEY_CONTROL))
	//{
	//	GAMEIN->GetMixDialog()->SetActiveRecursive(GAMEIN->GetUpgradeDialog()->IsActive()^TRUE);
	//}
	
	if(keyInfo->GetKeyDown(KEY_BACK) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		WINDOWMGR->ToggleShowInterface();
	}

	// ¸¶¿i½º Æ÷AIAI A×½ºÆ®
	if(keyInfo->GetKeyDown(KEY_P) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		m_bInVisibleCursor ^= TRUE;
		CURSOR->SetCheatMode( m_bInVisibleCursor );
/*
		if(m_bInVisibleCursor)
		{
			CURSOR->SetCheatMode( TRUE );
			CURSOR->SetCursor(eCURSOR_DEFAULTCHEAT);
			CURSOR->SetActive(TRUE);
		}
		else
		{
			CURSOR->SetCursor(eCURSOR_DEFAULT);
			CURSOR->SetActive(TRUE);
		}	
*/
	}

	if( keyInfo->GetKeyDown(KEY_C) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		if( CAMERA->GetCameraMode() == eCM_EyeView )
		{
			CAMERA->SetCameraMode( eCM_Free );
		}
		else
		{
			CAMERA->SetCameraMode( eCM_EyeView );
		}
	}
	
#ifndef _TESTCLIENT_
	static BOOL bWireFrame = FALSE;
#endif
/*	if(keyInfo->GetKeyDown(KEY_F12) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		if( !bWireFrame )
		{			
			g_pExecutive->GetRenderer()->SetRenderMode(2);
			bWireFrame = TRUE;
		}
		else
		{
			g_pExecutive->GetRenderer()->SetRenderMode(0);
			bWireFrame = FALSE;
		}
	}*/

#ifndef TAIWAN_LOCAL
#ifndef _HK_LOCAL_
#ifndef _TL_LOCAL_
#ifndef _JAPAN_LOCAL_
	//
	if(keyInfo->GetKeyDown(KEY_A) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		//SIEGEMGR->CreateCastleGate();
		//HERO->ChangeMotion(329, TRUE);
		/*CMugongSuryunDialog* pDlg = GAMEIN->GetMugongSuryunDialog();
		if( pDlg )
		{
			pDlg->SetActive( !pDlg->IsActive() );
		}*/

		CSWTimeDialog* pdlg = GAMEIN->GetSWTimeDlg();
		if( pdlg )
		{
			pdlg->SetActive( !pdlg->IsActive() );
			pdlg->SetMunpaName( "그레이트형님이셔" );
		}
	}
	if(keyInfo->GetKeyDown(KEY_S) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		CGTRegistcancelDialog* pdlg = GAMEIN->GetGTRegistcancelDlg();
		if( pdlg )
			pdlg->SetActive( !pdlg->IsActive() );
	}
	if(keyInfo->GetKeyDown(KEY_L) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		CGTStandingDialog* pdlg = GAMEIN->GetGTStandingDlg();
		if( pdlg )
		{
			REGISTEDGUILDINFO gi;
/*			gi.Ranking = eGTStanding_4TH;
			gi.Position = 4;
			gi.GuildIdx = 1;
			strcpy( gi.GuildName, "길다다일" );
			pdlg->AddGuildInfo( &gi );
			gi.Ranking = eGTStanding_2ND;
			gi.Position = 13;			
			gi.GuildIdx = 2;
			strcpy( gi.GuildName, "길다다이" );
			pdlg->AddGuildInfo( &gi );
			gi.Ranking = eGTStanding_3RD;
			gi.Position = 22;
			gi.GuildIdx = 3;
			strcpy( gi.GuildName, "길다다삼" );
			pdlg->AddGuildInfo( &gi );
			gi.Ranking = eGTStanding_1ST;
			gi.Position = 31;
			gi.GuildIdx = 4;
			strcpy( gi.GuildName, "길다다사" );
			pdlg->AddGuildInfo( &gi );*/
			gi.Ranking = eGTStanding_8TH;
			gi.Position = 0;
			gi.GuildIdx = 2;
			strcpy( gi.GuildName, "라마사마일" );
			pdlg->AddGuildInfo( &gi );
			gi.Ranking = eGTStanding_32TH;
			gi.Position = 1;
			gi.GuildIdx = 4;
			strcpy( gi.GuildName, "라마사마이" );
			pdlg->AddGuildInfo( &gi );
			gi.Ranking = eGTStanding_16TH;
			gi.Position = 2;
			gi.GuildIdx = 6;
			strcpy( gi.GuildName, "라마사마삼" );
			pdlg->AddGuildInfo( &gi );
			gi.Ranking = eGTStanding_32TH;
			gi.Position = 3;
			gi.GuildIdx = 8;
			strcpy( gi.GuildName, "라마사마사" );
			pdlg->AddGuildInfo( &gi );
			gi.Ranking = eGTStanding_32TH;
			gi.Position = 4;			
			gi.GuildIdx = 10;
			strcpy( gi.GuildName, "라마사마오" );
			pdlg->AddGuildInfo( &gi );
			gi.Ranking = eGTStanding_4TH;
			gi.Position = 5;			
			gi.GuildIdx = 12;
			strcpy( gi.GuildName, "라마사마육" );
			pdlg->AddGuildInfo( &gi );
			gi.Ranking = eGTStanding_16TH;
			gi.Position = 6;			
			gi.GuildIdx = 14;
			strcpy( gi.GuildName, "라마사마칠" );
			pdlg->AddGuildInfo( &gi );
			gi.Ranking = eGTStanding_32TH;
			gi.Position = 7;			
			gi.GuildIdx = 16;
			strcpy( gi.GuildName, "라마사마팔" );
			pdlg->AddGuildInfo( &gi );
			pdlg->SetActive( !pdlg->IsActive() );
			pdlg->RefreshGuildInfo();
		}
	}
	if(keyInfo->GetKeyDown(KEY_J) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		GTBATTLE_INFO bt1, bt2;
		bt1.Group = 0;
		sprintf( bt1.GuildName1, "라마님만세" );
		sprintf( bt1.GuildName2, "라마님짱" );
		bt2.Group = 1;
		sprintf( bt2.GuildName1, "크헤헤" );
		sprintf( bt2.GuildName2, "아오" );

		CGTBattleListDialog* pdlg = GAMEIN->GetGTBattleListDlg();
		if( pdlg )
		{
			pdlg->SetPlayOff( TRUE );
			pdlg->AddBattleInfo( &bt1 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->AddBattleInfo( &bt2 );
			pdlg->RefreshBattleList();
			pdlg->SetActive( !pdlg->IsActive() );
		}
	}
	if(keyInfo->GetKeyDown(KEY_H) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		CGTScoreInfoDialog* pdlg = GAMEIN->GetGTScoreInfoDlg();
		if( pdlg )
			pdlg->SetActive( !pdlg->IsActive() );
	}
	if(keyInfo->GetKeyDown(KEY_G) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		// magi82 - Titan(070320)
		if( GAMEIN->GetTitanRegisterDlg()->IsActive() == FALSE )
		{
			GAMEIN->GetTitanRegisterDlg()->SetActive( TRUE );
		}
		else
			GAMEIN->GetTitanRegisterDlg()->SetActive( FALSE );
	}	
	if(keyInfo->GetKeyDown(KEY_K) && keyInfo->GetKeyPressed(KEY_MENU))
	{		
		if(GAMEIN->GetSkillPointResetDlg()->IsActive() == FALSE)
		{
			GAMEIN->GetSkillPointResetDlg()->SetActive(TRUE);
		}
		else
		{
			GAMEIN->GetSkillPointResetDlg()->SetActive(FALSE);
		}		
	}	
	// magi82 - Titan(070611) 타이탄 무공변환 주석처리
	/*
	if(keyInfo->GetKeyDown(KEY_F) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		// magi82 - Titan(070119)
		if( GAMEIN->GetTitanMugongMixDlg()->IsActive() == FALSE )
		{
			GAMEIN->GetTitanMugongMixDlg()->SetActive( TRUE );
		}
		else
			GAMEIN->GetTitanMugongMixDlg()->SetActive( FALSE );
	}
	*/
	if(keyInfo->GetKeyDown(KEY_D) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		if( GAMEIN->GetUniqueItemMixDlg()->IsActive() == FALSE )
		{
            GAMEIN->GetUniqueItemMixDlg()->SetActive(TRUE);
		}
		else
		{
			GAMEIN->GetUniqueItemMixDlg()->SetActive(FALSE);
		}
		// magi82 - SOS(070724)
	//	WINDOWMGR->MsgBox( MBI_SOS, MBT_YESNO, CHATMGR->GetChatMsg(1633) );
	}
#endif
#endif
#endif
#endif
	

#ifdef _GMTOOL_
	if( !CHEATMGR->IsCheatEnable() ) return;
#endif

	if( bDock == TRUE) return;

#ifndef TAIWAN_LOCAL
#ifndef _JAPAN_LOCAL_
#ifndef _HK_LOCAL_
#ifndef _TL_LOCAL_

	if(keyInfo->GetKeyDown(KEY_I) && keyInfo->GetKeyPressed(KEY_MENU))
	{
		MSGBASE msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_INFO_SYN;
		msg.dwObjectID = HERO->GetID();
		NETWORK->Send(&msg, sizeof(msg));
	}
	
	//¹®ÆAA×½ºÆ®
// 	if(keyInfo->GetKeyDown(KEY_G) && keyInfo->GetKeyPressed(KEY_CONTROL))
// 	{
// 		GAMEIN->GetSkPointDlg()->SetActive( !GAMEIN->GetSkPointDlg()->IsActive() );
// 	}
	
//////////////////////////////////////////////////////////////////////////
//for test
#ifdef _TESTCLIENT_
	//C￥±¹ A×½ºÆ®
	if(keyInfo->GetKeyDown(KEY_X) && keyInfo->GetKeyPressed(KEY_CONTROL))
	{
		GAMEIN->GetWantNpcDialog()->SetActive(TRUE);
		GAMEIN->GetWantNpcDialog()->ShowWantMode(eWantNpcMode_Npc);
		
		SEND_WANTNPCLIST Info;
		Info.TotalPage = 6;
		char buf[64];
		
		for(int i=0; i<WANTUNIT_PER_PAGE; ++i)
		{
			sprintf(buf, "아톰%d", i);
			SafeStrCpy(Info.WantNpcList[i].WantedName, buf, MAX_NAME_LENGTH+1);
			Info.WantNpcList[i].Prize = i+100;
			Info.WantNpcList[i].VolunteerNum = i+50;
			Info.WantNpcList[i].WantedIDX = i+1;
		}
		GAMEIN->GetWantNpcDialog()->SetWantedList(&Info);
		
	}
#endif
//////////////////////////////////////////////////////////////////////////
	
	//AEAo A×½ºÆ®
	if(keyInfo->GetKeyDown(KEY_N) && keyInfo->GetKeyPressed(KEY_CONTROL))
	{
		if( GAMEIN->GetReinforceResetDlg() )
		{
			GAMEIN->GetReinforceResetDlg()->SetActive( !GAMEIN->GetReinforceResetDlg()->IsActive() );
		}
	}
	//Co≫o¹u A×½ºÆ®
	if(keyInfo->GetKeyDown(KEY_Y) && keyInfo->GetKeyPressed(KEY_CONTROL))
	{
//		GAMEIN->GetGuildWarehouseDlg()->SetActive(!GAMEIN->GetGuildWarehouseDlg()->IsActive());
		MSG_DWORD msg;
		msg.Category = MP_GTOURNAMENT;
		msg.Protocol = MP_GTOURNAMENT_CHEAT;
		msg.dwObjectID = HEROID;
		msg.dwData = 2;
		NETWORK->Send( &msg, sizeof(msg) );
	}
	if(keyInfo->GetKeyDown(KEY_Q) && keyInfo->GetKeyPressed(KEY_CONTROL))
	{
//		GAMEIN->GetQuestTotalDialog()->SetActive(TRUE);
		MSGBASE msg;
		msg.Category = MP_GTOURNAMENT;
		msg.Protocol = MP_GTOURNAMENT_REGIST_SYN;
		msg.dwObjectID = HEROID;
		NETWORK->Send( &msg, sizeof(msg) );
	}
	if(keyInfo->GetKeyDown(KEY_A) && keyInfo->GetKeyPressed(KEY_CONTROL))
	{
		MSG_DWORD msg;
		msg.Category = MP_GTOURNAMENT;
		msg.Protocol = MP_GTOURNAMENT_CHEAT;
		msg.dwObjectID = HEROID;
		msg.dwData = 1;
		NETWORK->Send( &msg, sizeof(msg) );
	}
	
#endif
#endif
#endif
#endif


#ifdef _TESTCLIENT_
//050630SW
//	if( keyInfo->GetKeyDown(KEY_6) && keyInfo->GetKeyPressed(KEY_MENU) ) 
//	{
//		CHATTIPMGR->SetActive( TRUE );
//		CHATTIPMGR->SetCurHeroInfo(1);
//	}
//	if( keyInfo->GetKeyDown(KEY_7) && keyInfo->GetKeyPressed(KEY_MENU) ) 
//	{
//		CHATTIPMGR->SetActive( TRUE );
//		CHATTIPMGR->SetCurHeroInfo(25);
//	}
//	if( keyInfo->GetKeyDown(KEY_8) && keyInfo->GetKeyPressed(KEY_MENU) ) 
//	{
//		CHATTIPMGR->SetActive( TRUE );
//		CHATTIPMGR->SetCurHeroInfo(30);
//	}
//-	
	
	if(keyInfo->GetKeyDown(KEY_T) && keyInfo->GetKeyPressed(KEY_CONTROL))
	{
		SKILLMGR->MakeSAT();
	}
	if(keyInfo->GetKeyDown(KEY_W))
	{
		CActionTarget Target;
		CSkillInfo* pInfo = SKILLMGR->GetSkillInfo(GAMERESRCMNGR->m_TestClientInfo.MugongIdx);		
		if(pInfo && pInfo->GetSkillInfo()->TargetKind == 0)			// ≫o´e
		{
			CObject * targetObj = OBJECTMGR->GetSelectedObject();
			if( targetObj )
			{
				Target.InitActionTarget(targetObj,NULL);
			}/*		// A¸°UAI ¾ø´A °æ¿i ¸¶¿i½º Æ÷AIAI AO´A °÷¿¡ ½ºA³A≫ ¾²´A°A
			else if(pInfo->IsMultipleTargetSkill() == TRUE)
			{
				VECTOR3* pPos = GetPickedPosition(MOUSE->GetMouseX(),MOUSE->GetMouseY());
				if(pPos == NULL)
					return;
				Target.InitActionTarget(pPos,NULL);
			}*/
			else
			{
				//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(400) );				
				GAMEIN->GetQuestTotalDialog()->SetActive(TRUE);
				return;
			}
		}
		else if(pInfo && pInfo->GetSkillInfo()->TargetKind == 1)		// AU½A
		{
			Target.InitActionTarget(HERO,NULL);		// AU½AA≫ A¸°UA¸·I
		}
		
		SKILLMGR->OnSkillCommand( HERO, &Target, TRUE );
	}
	if(keyInfo->GetKeyDown(KEY_E))
	{
		CObject * targetObj = OBJECTMGR->GetSelectedObject();
		if( targetObj )
		{
			TARGETSET Target;
			Target.pTarget = targetObj;
			memset(&Target.ResultInfo,0,sizeof(RESULTINFO));
			Target.ResultInfo.bCritical = rand()%2 ? TRUE : FALSE;
			Target.ResultInfo.RealDamage = rand() % 50;
			Target.ResultInfo.ShieldDamage = rand() % 30;
			Target.ResultInfo.CounterDamage = 0;
			//Target.TargetData = rand() % 100;
			EFFECTMGR->StartEffectProcess(GAMERESRCMNGR->m_TestClientInfo.Effect,HERO,&Target,1,targetObj->GetID(),EFFECT_FLAG_HEROATTACK);
		}
	}
	if(keyInfo->GetKeyDown(KEY_3))
	{
		CObject * targetObj = OBJECTMGR->GetSelectedObject();
		if( targetObj )
		{
			TARGETSET Target;
			TESTCLIENTINFO pTestClientInfo = GAMERESRCMNGR->m_TestClientInfo;

			if(pTestClientInfo.bMonsterDie == TRUE)
			{
				Target.pTarget = targetObj;
			}
			else
			{
				Target.pTarget = HERO;
			}
			memset(&Target.ResultInfo,0,sizeof(RESULTINFO));
			Target.ResultInfo.bCritical = rand()%2 ? TRUE : FALSE;
			Target.ResultInfo.RealDamage = rand() % 50;
			Target.ResultInfo.ShieldDamage = rand() % 30;
			Target.ResultInfo.CounterDamage = 0;
			//Target.TargetData = rand() % 100;

			EFFECTMGR->StartEffectProcess(pTestClientInfo.MonsterEffect,targetObj,&Target,1,Target.pTarget->GetID(),EFFECT_FLAG_HEROATTACK);
		}
	}
	if(keyInfo->GetKeyDown(KEY_4))
	{
		CObject * targetObj = OBJECTMGR->GetSelectedObject();
		if( targetObj )
		{
			VECTOR3 pos;
			targetObj->GetPosition(&pos);
			pos.z -= 3000;
			MOVEMGR->SetMonsterMoveType(targetObj,eMA_WALKAROUND);
			MOVEMGR->StartMoveEx(targetObj,gCurTime,&pos);
		}
	}
	if(keyInfo->GetKeyDown(KEY_5))
	{
		CObject * targetObj = OBJECTMGR->GetSelectedObject();
		if( targetObj )
		{
			VECTOR3 pos;
			targetObj->GetPosition(&pos);
			pos.z -= 3000;
			MOVEMGR->SetMonsterMoveType(targetObj,eMA_PERSUIT);
			MOVEMGR->StartMoveEx(targetObj,gCurTime,&pos);
		}
	}
	if(keyInfo->GetKeyDown(KEY_6))
	{
		CObject * targetObj = OBJECTMGR->GetSelectedObject();
		if( targetObj )
		{
			VECTOR3 pos;
			targetObj->GetPosition(&pos);
			pos.z -= 3000;
			MOVEMGR->SetMonsterMoveType(targetObj,eMA_RUNAWAY);
			MOVEMGR->StartMoveEx(targetObj,gCurTime,&pos);
		}
	}
#endif



#endif	//_CHEATENABLE_

}

//---press´A CN¹ø¸¸ A¼AⓒCØ¾ßCI¹C·I μu·I ≫≪´U.
void CMacroManager::KeyboardPressInput( CKeyboard* keyInfo )
{
	if( !g_bActiveApp || m_bPause )	return;

	if( MAINGAME->GetCurStateNum() != eGAMESTATE_GAMEIN ) return;
	if( CIMEWND->IsDocking() )
	{
//		if( m_nMacroMode == MM_MACRO )	//A¤ÆA¿i¼±AI¶oμμ °E¸°´U..CoAc´A..--;
			return;
	}

	int nSysKey = MSK_NONE;

	if( keyInfo->GetKeyPressed( KEY_CONTROL ) )		nSysKey |= MSK_CTRL;
	if( keyInfo->GetKeyPressed( KEY_MENU ) )		nSysKey |= MSK_ALT;
	if( keyInfo->GetKeyPressed( KEY_SHIFT ) )		nSysKey |= MSK_SHIFT;


	for( int i = 0 ; i < MPE_COUNT ; ++i )
	{
		if( m_MacroPressKey[m_nMacroMode][i].nSysKey & nSysKey
			&& keyInfo->GetKeyPressed( m_MacroPressKey[m_nMacroMode][i].wKey ) )
		{
			PlayMacroPress( i );
		}
	}		
}

void CMacroManager::GetMacro( int nMacroMode, sMACRO* pMacro )
{
	memcpy( pMacro, m_MacroKey[nMacroMode], sizeof(sMACRO) * ME_COUNT );
}

//void CMacroManager::GetPressMacro( int nMacroMode, sMACRO* pMacro )

void CMacroManager::SetMacro( int nMacroMode, sMACRO* pMacro )
{
	memcpy( m_MacroKey[nMacroMode], pMacro, sizeof(sMACRO) * ME_COUNT );
}

//void CMacroManager::SetPressMacro( int nMacroMode, sMACRO* pMacro )

void CMacroManager::GetDefaultMacro( int nMacroMode, sMACRO* pMacro )
{
	memcpy( pMacro, m_DefaultKey[nMacroMode], sizeof(sMACRO) * ME_COUNT );
}

//void CMacroManager::GetDefaultPressMacro( int nMacroMode, sMACRO* pMacro )

void CMacroManager::SetMacroMode( int nMacroMode )
{
	m_nMacroMode = nMacroMode;

	if( m_nMacroMode == MM_CHAT )
		m_bChatMode = TRUE;
	else
		m_bChatMode = FALSE;
}

void CMacroManager::LoadUserMacro()
{
//	if( !LoadMacro( "./INI/MacroUserSet.mcr" ) )	//ÆAAI¸iA≫ ¹U²U±i?
//	{
//		SetDefaultMacro( MM_MACRO );
//		SetMacroMode( MM_MACRO );
//		SaveUserMacro();
//	}

//AO½AAUμa/////////////////////
	SetDefaultMacro( MM_MACRO );
	SetDefaultMacro( MM_CHAT );
	SetMacroMode( MM_MACRO );
//	SaveUserMacro();
////////////////////////////////
}


BOOL CMacroManager::LoadMacro( LPCTSTR strPath )
{
	HANDLE	hFile;

	hFile = CreateFile( strPath, GENERIC_READ, 0, NULL, 
						OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	if( hFile == INVALID_HANDLE_VALUE )
		return FALSE;

	DWORD dwRead;
	
//	if( !ReadFile( hFile, &m_nMacroMode, sizeof(int), &dwRead, NULL ) )
//	{
//		CloseHandle( hFile );
//		return FALSE;
//	}

//	if( !ReadFile( hFile, &m_nUserSetMode, sizeof(int), &dwRead, NULL ) )
//	{
//		CloseHandle( hFile );
//		return FALSE;
//	}

	if( !ReadFile( hFile, m_MacroKey,
		sizeof( m_MacroKey ), &dwRead, NULL ) )
	{
		CloseHandle( hFile );
		return FALSE;
	}

	if( !ReadFile( hFile, m_MacroPressKey,
		sizeof( m_MacroPressKey ), &dwRead, NULL ) )
	{
		CloseHandle( hFile );
		return FALSE;
	}
	
	CloseHandle( hFile );

//	if( m_nMacroMode == MM_CHAT )
//		m_bChatMode = TRUE;
//	else
//		m_bChatMode = FALSE;

	return TRUE;
}

void CMacroManager::SaveUserMacro()
{
	SaveMacro( "./INI/MacroUserSet.mcr" );
}

BOOL CMacroManager::SaveMacro( LPCTSTR strPath )
{
	HANDLE	hFile;

	hFile=CreateFile( strPath, GENERIC_WRITE, 0, NULL, 
						CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	if( hFile == INVALID_HANDLE_VALUE )
		return FALSE;
	
	DWORD dwWritten;
	
//	if( !WriteFile( hFile, &m_nMacroMode, sizeof(int), &dwWritten, NULL ) )
//	{
//		CloseHandle( hFile );
//		return FALSE;
//	}

//	if( !WriteFile( hFile, &m_nUserSetMode, sizeof(int), &dwWritten, NULL ) )
//	{
//		CloseHandle( hFile );
//		return FALSE;
//	}
	
	if( !WriteFile( hFile, m_MacroKey,
		sizeof( m_MacroKey ), &dwWritten, NULL ) )
	{
		CloseHandle( hFile );
		return FALSE;
	}

	if( !WriteFile( hFile, m_MacroPressKey,
		sizeof( m_MacroPressKey ), &dwWritten, NULL ) )
	{
		CloseHandle( hFile );
		return FALSE;
	}

	CloseHandle( hFile );
	return TRUE;	
}


//KES ¾E¸²
//¿¡μðÆ®¸| ≫c¿eCI´A ´UAI¾o·I±×´A ¹Yμa½A SetActiveRecursive·I ´UAI¾o·I±×¸| ´Y¾ÆAO¼¼¿a.
void CMacroManager::PlayMacro( int nMacroEvent )
{
	switch( nMacroEvent )
	{

		case ME_USE_QUICKITEM01:
			QUICKMGR->UseQuickItem(0);
			break;

		case ME_USE_QUICKITEM02:
			QUICKMGR->UseQuickItem(1);
			break;

		case ME_USE_QUICKITEM03:
			QUICKMGR->UseQuickItem(2);
			break;

		case ME_USE_QUICKITEM04:
			QUICKMGR->UseQuickItem(3);
			break;

		case ME_USE_QUICKITEM05:
			QUICKMGR->UseQuickItem(4);
			break;

		case ME_USE_QUICKITEM06:
			QUICKMGR->UseQuickItem(5);
			break;

		case ME_USE_QUICKITEM07:
			QUICKMGR->UseQuickItem(6);
			break;

		case ME_USE_QUICKITEM08:
			QUICKMGR->UseQuickItem(7);
			break;

		case ME_USE_QUICKITEM09:
			QUICKMGR->UseQuickItem(8);
			break;

		case ME_USE_QUICKITEM10:
			QUICKMGR->UseQuickItem(9);
			break;

/*
		case ME_PAGEUP_QUICKSLOT:
			GAMEIN->GetQuickDialog()->OnActionEvent(QI_PREVPAGEBTN, NULL, WE_BTNCLICK);
			break;

		case ME_PAGEDN_QUICKSLOT:
			GAMEIN->GetQuickDialog()->OnActionEvent(QI_NEXTPAGEBTN, NULL, WE_BTNCLICK);
			break;
*/
		case ME_SELECT_QUICKSLOT1:
			GAMEIN->GetQuickDialog()->SelectPageDirect(0);
			break;

		case ME_SELECT_QUICKSLOT2:
			GAMEIN->GetQuickDialog()->SelectPageDirect(1);
			break;

		case ME_SELECT_QUICKSLOT3:
			GAMEIN->GetQuickDialog()->SelectPageDirect(2);
			break;

		case ME_SELECT_QUICKSLOT4:
			GAMEIN->GetQuickDialog()->SelectPageDirect(3);
			break;

		case ME_TOGGLE_INVENTORYDLG:

			GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_INVENTORYDLGICON);
			break;

		case ME_TOGGLE_CHARACTERDLG:

			GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_CHARACTERDLGICON);
			break;

		case ME_TOGGLE_MUGONGDLG:

			GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_MUGONGDLGICON);
			break;

//		case ME_TOGGLE_PARTYDLG:
//			GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_PARTYDLGICON);
//			break;

		case ME_TOGGLE_MUNPADLG:
			GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_GUILDDLGICON);
			break;

		case ME_TOGGLE_QUESTDLG:
			GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_QUESTDLGICON);			
			break;
		case ME_TOGGLE_EXITDLG:

			if( !WINDOWMGR->CloseAllAutoCloseWindows() )
			{
				GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_EXITDLGICON);
			}			

			break;
/*
		case ME_APPLY_EXCHANGE:
			{
				//GAMEIN->GetMainInterfaceDialog()->PushDirectMenu( OPT_EXCHANGEDLGICON );

				//KES SECURITY-CHECK
				if(ABILITYMGR->CheckAbilityLevel(ABILITYINDEX_EXCHANGE,HERO->GetAbilityGroup(),1) == FALSE)
				{
					CHATMGR->AddMsg( CTC_ABILITYUPGRADE_FAILED, CHATMGR->GetChatMsg( 139 ));
				}
				else
				{
					EXCHANGEMGR->ApplyExchange();
				}
			}
			break;

		case ME_OPEN_STREETSTALL:
			{
//				GAMEIN->GetMainInterfaceDialog()->PushDirectMenu( OPT_STREETSTALLDLGICON );
				if(ABILITYMGR->CheckAbilityLevel(ABILITYINDEX_STALL,HERO->GetAbilityGroup(),1) == FALSE)
				{
					CHATMGR->AddMsg( CTC_ABILITYUPGRADE_FAILED, CHATMGR->GetChatMsg( 139 ));
				}
				else
				{
					if( HERO->GetState() == eObjectState_None && !VIMUMGR->IsVimuing() )
					{
						STREETSTALLMGR->ChangeDialogState();
					}
				}
			}
			break;
*/
		case ME_TOGGLE_OPTIONDLG:
			{
				GAMEIN->GetMainInterfaceDialog()->PushDirectMenu( OPT_OPTIONDLGICON );
			}
			break;

//		case ME_CLOSE_ALLWINDOW:
//			{
//				GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_CLOSEALLDLGICON);
//				WINDOWMGR->CloseAllAutoCloseWindows();
//			}
//			break;

		case ME_TOGGLE_FRIENDLIST:
			{
				GAMEIN->GetMainInterfaceDialog()->PushDirectMenu( OPT_FRIENDDLGICON );
			}
			break;

		case ME_SEND_MEMO:
			{
				GAMEIN->GetMainInterfaceDialog()->PushDirectMenu( OPT_NOTEDLGICON );
			}
			break;
			
//		case ME_APPLY_VIMU:
//			{
//				GAMEIN->GetMainInterfaceDialog()->PushDirectMenu( OPT_VIMUICON );
//			}
//			break;

//		case ME_CLOSE_ACTIVEDLG:
//			break;

		case ME_TOGGLE_MOVEMODE:
			MOVEMGR->ToggleHeroMoveMode();
			break;

		case ME_TOGGLE_PEACEWARMODE:
			PEACEWARMGR->ToggleHeroPeace_WarMode();
			break;

//		case ME_TOGGLE_UNGIMODE:
//			//TODOABIL
//			if(ABILITYMGR->CheckAbilityLevel(ABILITYINDEX_UNGI,HERO->GetAbilityGroup(),1) == FALSE)
//			{
//				CHATMGR->AddMsg( CTC_ABILITYUPGRADE_FAILED, CHATMGR->GetChatMsg( 139 ));
//				return;
//			}
			
//			UNGIMGR->ToggleHero_UngiMode();
//			break;

			/*
		case ME_TOGGLE_KYUNGGONG:
			{
				//TODOABIL
				HERO->SetToggleKyungGongMode();
				if( HERO->GetKyungGongMode() == TRUE && HERO->GetKyungGongGrade() == 0)		// °æ°øμi±ÞAI 0AI¸e °æ°ø ¸ø¾¸
				{
					HERO->SetToggleKyungGongMode();
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(143) );
				}
				
				HERO->ToggleKyungGongMode();
			}
			break;
			*/
		case ME_TOGGLE_AUTOATTACK:
		case ME_TOGGLE_AUTOATTACK2:
			//KES 031007
			g_UserInput.GetHeroMove()->AttackSelectedObject();
			break;

		case ME_TOGGLE_HELP:

			GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_HELPDLGICON);
//			if( !GAMEIN->GetHelpDialog()->IsActive() )
//				GAMEIN->GetHelpDialog()->OpenDialog();
//			else
//				GAMEIN->GetHelpDialog()->SetActive( FALSE );
			break;

		case ME_TOGGLE_MINIMAP:
			GAMEIN->GetMainInterfaceDialog()->PushDirectMenu(OPT_MINIMAPDLGICON);
			break;

		case ME_TOGGLE_CAMERAVIEWMODE:
			CAMERA->ToggleCameraViewMode();
			break;

//		case ME_CAMERAMODE_BACKVIEW:
//			CAMERA->SetCameraMode(eCM_FollowView);
//			break;

//		case ME_CAMERAMODE_QUARTERVIEW:
//			CAMERA->SetCameraMode(eCM_QuarterView);
//			break;

//		case ME_CAMERAMODE_LOADVIEW:
//			break;
//		case ME_CAMERAMODE_SAVEVIEW:
//			break;
//		case ME_CAMERAMOVE_LEFT:
//			break;
//		case ME_CAMERAMOVE_RIGHT:
//			break;
//		case ME_CAMERAMOVE_UP:
//			break;
//		case ME_CAMERAMOVE_DOWN:
//			break;
//		case ME_CAMERAMOVE_ZOOMIN:
//			break;
//		case ME_CAMERAMOVE_ZOOMOUT:
//			break;

		case ME_SCREENCAPTURE:
			{
				DIRECTORYMGR->SetLoadMode(eLM_Root);
//				mkdir("ScreenShot");

				CreateDirectory( "ScreenShot", NULL );

				
				WORD wSrv = 0, wY = 0, wM = 0, wD = 0, wN = 0;
				WORD wHH = 0, wMM = 0, wSS = 0;		//06.08.10 [PKH] 저장파일에 시간표시
				char fname[256];
				char fname2[256];
				//char fnametest[256];
//
				SYSTEMTIME ti;
				//GetSystemTime( &ti );
				GetLocalTime( &ti );

				HANDLE hFile = CreateFile( "ini/ScreenShot.cnt", GENERIC_READ, 0, NULL, 
							OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

				DWORD dwSize = 0;

				if( hFile != INVALID_HANDLE_VALUE )
				{
					ReadFile( hFile, &wSrv, sizeof(wSrv), &dwSize, NULL );
					ReadFile( hFile, &wY, sizeof(wY), &dwSize, NULL );
					ReadFile( hFile, &wM, sizeof(wM), &dwSize, NULL );
					ReadFile( hFile, &wD, sizeof(wD), &dwSize, NULL );
					ReadFile( hFile, &wHH, sizeof(wHH), &dwSize, NULL );
					ReadFile( hFile, &wMM, sizeof(wMM), &dwSize, NULL );
					ReadFile( hFile, &wSS, sizeof(wSS), &dwSize, NULL );
					ReadFile( hFile, &wN, sizeof(wN), &dwSize, NULL );
					
					CloseHandle( hFile );

					if( !( wSrv == (WORD)g_nServerSetNum && ti.wYear == wY && ti.wMonth == wM && ti.wDay == wD 
						&& ti.wHour == wHH && ti.wMinute == wMM && ti.wSecond == wSS) )
					{
						wSrv = g_nServerSetNum; wY = ti.wYear; wM = ti.wMonth; wD = ti.wDay; wN = 1; 
						wHH = ti.wHour; wMM = ti.wMinute; wSS = ti.wSecond;
					}					
				}
				else
				{
					wSrv = g_nServerSetNum; wY = ti.wYear; wM = ti.wMonth; wD = ti.wDay; wN = 1; 
					wHH = ti.wHour; wMM = ti.wMinute; wSS = ti.wSecond;
				}

/*
				FILE* fp = fopen( "ini/ScreenShot.cnt", "rt" );
				if( fp )
				{
					fscanf( fp, "%d %d %d %d", &wY, &wM, &wD, &wN );
					fclose( fp );

					if( !( ti.wYear == wY && ti.wMonth == wM && ti.wDay == wD ) )
					{
						wY = ti.wYear; wM = ti.wMonth; wD = ti.wDay; wN = 1;
					}
				}
				else
				{
					wY = ti.wYear; wM = ti.wMonth; wD = ti.wDay; wN = 1;
				}
*/
#ifndef _HK_LOCAL_
				sprintf(fname,"ScreenShot/DarkStory_%02d_%02d%02d%02d_%02d%02d%02d_%03d.jpg", wSrv, wY%100, wM, wD, wHH, wMM, wSS, wN );
				sprintf(fname2,"ScreenShot/DarkStory_%02d_%02d%02d%02d_%02d%02d%02d_%03d.txt", wSrv, wY%100, wM, wD, wHH, wMM, wSS, wN );
#else
				sprintf(fname,"ScreenShot/Mosiang_%02d_%02d%02d%02d_%02d%02d%02d_%03d.jpg", wSrv, wY%100, wM, wD, wHH, wMM, wSS, wN );
#endif
				BOOL rt = g_pExecutive->GetRenderer()->CaptureScreen(fname);
				//jpg·I º?E?CØAO´ø°¡, ¸Þ¸ð¸®·I¶oμμ ³N°UAO¸e AA°U´U.

//jpg·I º?E?CIAU( bmpAÐ°i, jpg·I º?E?CI°i, jpgAuAaCI°i, bmpAo¿i°i --;;;;;; )

//



				if( rt )
				{
/*
					fp = fopen( "ini/ScreenShot.cnt", "w" );
					if( fp )
					{
						fprintf( fp, "%d %d %d %d", wY, wM, wD, wN+1 );
						fclose( fp );
					}
*/
					hFile = CreateFile( "ini/ScreenShot.cnt", GENERIC_WRITE, 0, NULL, 
									CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

					if( hFile != INVALID_HANDLE_VALUE )
					{
						++wN;
						WriteFile( hFile, &wSrv, sizeof(wSrv), &dwSize, NULL );
						WriteFile( hFile, &wY, sizeof(wY), &dwSize, NULL );
						WriteFile( hFile, &wM, sizeof(wM), &dwSize, NULL );
						WriteFile( hFile, &wD, sizeof(wD), &dwSize, NULL );
						WriteFile( hFile, &wHH, sizeof(wHH), &dwSize, NULL );
						WriteFile( hFile, &wMM, sizeof(wMM), &dwSize, NULL );
						WriteFile( hFile, &wSS, sizeof(wSS), &dwSize, NULL );
						WriteFile( hFile, &wN, sizeof(wN), &dwSize, NULL );							

						CloseHandle( hFile );						
					}

					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(201), fname );
				}
				else
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(200) );
				}

//				FIBITMAP* pBitmap = FreeImage_LoadTARGA(fname);
				FIBITMAP* pBitmap = FreeImage_Load(FIF_TARGA, fname, TARGA_DEFAULT);
				
				if(pBitmap)
				{
					DeleteFile(fname);
					int len = strlen(fname);
					fname[len-3] = 'j';
					fname[len-2] = 'p';
					fname[len-1] = 'g';
					FreeImage_SaveJPEG(pBitmap,fname,JPEG_QUALITYSUPERB);
					FreeImage_Unload(pBitmap);
				}

				//현재시간 다이얼로그 OFF 
				//F12버튼을 뗄경우 스크린샷을 찍는다
				//macromanager.cpp 179라인에서 현재시간 다이얼로그 ON
				//GAMEIN->GetScreenShotDlg()->SetActive( FALSE, 0 ); // 스크린캡쳐 시간dlg 
#ifdef _GMTOOL_
				CObject* pTargetObj = OBJECTMGR->GetSelectedObject();
				if( pTargetObj )
				if( pTargetObj->GetObjectKind() == eObjectKind_Player )
				{
                    FILE* fp = fopen(fname2,"a+");
                    if(fp)
					{
                        fprintf(fp,"%s\n",pTargetObj->GetObjectName());
                        fclose(fp);
					}
				}
#endif
			}
			break;
		case ME_SHOWALLNAME_ON:
			{
				OBJECTMGR->SetAllNameShow( TRUE );
			}
			break;
		case ME_SHOWALLNAME_OFF:
			{
				OBJECTMGR->SetAllNameShow( FALSE );
			}
			break;

		case ME_CHANGE_WHISPERLIST_UP:
			{
				if( GAMEIN->GetChatDialog() )
				if( GAMEIN->GetChatDialog()->GetChatEditBox() )
				{
					if( GAMEIN->GetChatDialog()->GetChatEditBox()->IsFocus() )
					{
						CHATMGR->ChangeWhisperUser( TRUE );
					}
				}
			}
			break;
		case ME_CHANGE_WHISPERLIST_DOWN:
			{
				if( GAMEIN->GetChatDialog() )
				if( GAMEIN->GetChatDialog()->GetChatEditBox() )
				{
					if( GAMEIN->GetChatDialog()->GetChatEditBox()->IsFocus() )
					{
						CHATMGR->ChangeWhisperUser( FALSE );
					}
				}
			}
			break;

		case ME_TOGGLE_BIGMAP:
			{
#ifdef _GMTOOL_
				if( CAMERA->GetCameraMode() != eCM_EyeView )
#endif
				if( GAMEIN->GetBigMapDialog() )
					GAMEIN->GetBigMapDialog()->SetActive( !GAMEIN->GetBigMapDialog()->IsActive() );
			}
			break;
		case ME_MUSSANG_ON:
			{
				if(!HERO->IsDied() && !HERO->InTitan()) //SW070912 기획변경
					MUSSANGMGR->SendMsgMussangOn();
			}
			break;
#ifndef _HK_LOCAL_	//hk block
		case ME_TOGGLE_KEY_SETTING_TIP_HIDE:
			{
				if( CAMERA->GetCameraMode() != eCM_EyeView )
					GAMEIN->GetKeySettingTipDlg()->SetActive(FALSE);
			}
			break;
		case ME_TOGGLE_KEY_SETTING_TIP_SHOW:
			{
				if( CAMERA->GetCameraMode() != eCM_EyeView )
				{
					GAMEIN->GetKeySettingTipDlg()->SetMode( m_nMacroMode );
					GAMEIN->GetKeySettingTipDlg()->SetActive(TRUE);
				}
			}
			break;
#endif
		case ME_TOGGLE_TITANINVENTORYDLG:	//2007. 10. 8. CBH - 타이탄 인벤 단축키 추가
			{
				if(HERO->InTitan() == TRUE)
				{
					CTitanInventoryDlg* pTitanInven = GAMEIN->GetTitanInventoryDlg();
					if(pTitanInven->IsActive() == TRUE)
					{
						pTitanInven->SetActive(FALSE);
					}
					else
					{
						pTitanInven->SetActive(TRUE);
					}					
				}
			}			
			break;
	}
}

void CMacroManager::PlayMacroPress( int nMacroEvent )
{
	float Rate = 1.f;

	if( g_UserInput.GetKeyboard()->GetKeyPressed(KEY_RSHIFT) )		//RShift is Pressed
		Rate = 0.3f;

	switch( nMacroEvent )
	{
		case MPE_CAMERAMOVE_ZOOMIN:
#ifdef _GMTOOL_
			if( CAMERA->GetCameraMode() == eCM_EyeView )
				CAMERA->Fly( 0.8 * gTickTime*Rate*5 );
			else
#endif
				CAMERA->ZoomDistance( 0, -0.8 * gTickTime*Rate, 0 );

			break;

		case MPE_CAMERAMOVE_ZOOMOUT:
#ifdef _GMTOOL_
			if( CAMERA->GetCameraMode() == eCM_EyeView )
				CAMERA->Fly( -0.8 * gTickTime*Rate*5 );
			else
#endif
				CAMERA->ZoomDistance( 0, 0.8 * gTickTime*Rate, 0 );
			break;

		case MPE_CAMERAMOVE_UP:
			CAMERA->MouseRotate( 0, 0.3 * gTickTime*Rate );
			break;

		case MPE_CAMERAMOVE_DOWN:
			CAMERA->MouseRotate( 0, -0.3 * gTickTime*Rate );
			break;

		case MPE_CAMERAMOVE_RIGHT:
			CAMERA->MouseRotate( -0.3 * gTickTime*Rate, 0 );
			break;

		case MPE_CAMERAMOVE_LEFT:
			CAMERA->MouseRotate( 0.3 * gTickTime*Rate, 0 );
			break;

	}
}

void CMacroManager::Init()
{

	sMACRO DefaultMacro[MM_COUNT][ME_COUNT] = {
		{
			{ MSK_NONE,		KEY_ESCAPE,		TRUE , FALSE },			//	ME_TOGGLE_EXITDLG
//			{ MSK_CTRL,		KEY_CONTROL,	TRUE , FALSE},				//	ME_TOGGLE_UNGIMODE			---AO½A
			//¿ⓒ±a±iAo A§·I ¿A¸²
			{ MSK_NONE,		KEY_F1,	FALSE, FALSE },				//	ME_USE_QUICKITEM01
			{ MSK_NONE,		KEY_F2,	FALSE, FALSE },				//	ME_USE_QUICKITEM02
			{ MSK_NONE,		KEY_F3,	FALSE, FALSE },				//	ME_USE_QUICKITEM03
			{ MSK_NONE,		KEY_F4,	FALSE, FALSE },				//	ME_USE_QUICKITEM04
			{ MSK_NONE,		KEY_F5,	FALSE, FALSE },				//	ME_USE_QUICKITEM05
			{ MSK_NONE,		KEY_F6,	FALSE, FALSE },				//	ME_USE_QUICKITEM06
			{ MSK_NONE,		KEY_F7,	FALSE, FALSE },				//	ME_USE_QUICKITEM07
			{ MSK_NONE,		KEY_F8,	FALSE, FALSE },				//	ME_USE_QUICKITEM08
			{ MSK_NONE,		KEY_F9,	FALSE, FALSE },				//	ME_USE_QUICKITEM09
			{ MSK_NONE,		KEY_F10,FALSE, FALSE },				//	ME_USE_QUICKITEM10

			{ MSK_ALT,		KEY_S,	FALSE, FALSE },				//	ME_TOGGLE_MUGONGDLG
			{ MSK_ALT,		KEY_I,	FALSE, FALSE },				//	ME_TOGGLE_INVENTORYDLG
			{ MSK_ALT,		KEY_C,	FALSE, FALSE },				//	ME_TOGGLE_CHARACTERDLG
			{ MSK_ALT,		KEY_G,	FALSE, FALSE },				//	ME_TOGGLE_MUNPADLG
//			{ MSK_ALT,		KEY_P,	FALSE, FALSE },				//	ME_TOGGLE_PARTYDLG		//none

			{ MSK_ALT,		KEY_M,	FALSE, FALSE },				//	ME_TOGGLE_MINIMAP

			{ MSK_ALT,		KEY_Q,	FALSE, FALSE },				//	ME_TOGGLE_QUESTDLG
//			{ MSK_NONE,		KEY_ESCAPE,	FALSE },			//	ME_TOGGLE_EXITDLG

//			{ MSK_ALT,		KEY_E,	FALSE, FALSE },				//	ME_APPLY_EXCHANGE
//			{ MSK_ALT,		KEY_W,	FALSE, FALSE },				//	ME_OPEN_STREETSTALL
			{ MSK_ALT,		KEY_O,	FALSE, FALSE },				//	ME_TOGGLE_OPTIONDLG
//			{ MSK_ALT,		KEY_X,	FALSE, FALSE },				//	ME_CLOSE_ALLWINDOW
			{ MSK_ALT,		KEY_F,	FALSE, FALSE },				//	ME_TOGGLE_FRIENDLIST
			{ MSK_ALT,		KEY_N,	FALSE, FALSE },				//	ME_SEND_MEMO
//			{ MSK_ALT,		KEY_V,	FALSE, FALSE },				//	ME_APPLY_VIMU

//			{ MSK_NONE,		KEY_ESCAPE,	FALSE },			//	ME_CLOSE_ACTIVEDLG		//none

			{ MSK_ALT,		KEY_R,	FALSE, FALSE },				//	ME_TOGGLE_MOVEMODE			---AO½A
//			{ MSK_ALT,		KEY_T,	FALSE, FALSE },				//	ME_TOGGLE_GYUNGGONG

//			{ MSK_NONE,		KEY_F10,	FALSE, FALSE },				//	ME_PAGEDN_QUICKSLOT	//none
//			{ MSK_NONE,		KEY_F9,		FALSE, FALSE },				//	ME_PAGEUP_QUICKSLOT	//none

			{ MSK_ALT,		KEY_1,	FALSE, FALSE },				//	ME_SELECT_QUICKSLOT1
			{ MSK_ALT,		KEY_2,	FALSE, FALSE },				//	ME_SELECT_QUICKSLOT2
			{ MSK_ALT,		KEY_3,	FALSE, FALSE },				//	ME_SELECT_QUICKSLOT3
			{ MSK_ALT,		KEY_4,	FALSE, FALSE },				//	ME_SELECT_QUICKSLOT4

//			{ MSK_CTRL,		KEY_CONTROL,	FALSE, FALSE },				//	ME_TOGGLE_UNGIMODE			---AO½A
			{ MSK_ALT,		KEY_Z,			FALSE, FALSE },				//	ME_TOGGLE_PEACEWARMODE

			{ MSK_CTRL,		KEY_CONTROL, TRUE, FALSE },				//	ME_TOGGLE_AUTOATTACK
			{ MSK_ALT,		KEY_A,	FALSE, FALSE },					//	ME_TOGGLE_AUTOATTACK2

			{ MSK_ALT,		KEY_H,	FALSE, FALSE },				//	ME_TOGGLE_HELP

//			{ MSK_NONE,		KEY_HOME,	FALSE, FALSE },				//	ME_CAMERAMOVE_ZOOMIN
//			{ MSK_NONE,		KEY_END,	FALSE, FALSE },				//	ME_CAMERAMOVE_ZOOMOUT

			{ MSK_NONE,		KEY_INSERT,	FALSE, FALSE },			//	ME_CAMERAMODE_FREEVIEW
//			{ MSK_NONE,		KEY_INSERT,	FALSE, FALSE },			//	ME_CAMERAMODE_BACKVIEW	//none
//			{ MSK_NONE,		KEY_F11,	FALSE, FALSE },				//	ME_CAMERAMODE_QUARTERVIEW	//none
//			{ MSK_ALT,		KEY_F12,	FALSE, FALSE },				//	ME_CAMERAMODE_LOADVIEW		//none
//			{ MSK_ALT,		KEY_F12,	FALSE, FALSE },				//	ME_CAMERAMODE_SAVEVIEW		//none

			{ MSK_NONE,		KEY_F12,	FALSE, FALSE },				//	ME_SCREENCAPTURE

//			{ MSK_NONE,		KEY_UP,		FALSE, FALSE },				//	ME_CAMERAMOVE_UP
//			{ MSK_NONE,		KEY_DOWN,	FALSE, FALSE },				//	ME_CAMERAMOVE_DOWN
//			{ MSK_NONE,		KEY_RIGHT,	FALSE, FALSE },				//	ME_CAMERAMOVE_RIGHT
//			{ MSK_NONE,		KEY_LEFT,	FALSE, FALSE },				//	ME_CAMERAMOVE_LEFT
			{ MSK_SHIFT,	KEY_SHIFT,  FALSE, FALSE },				//ME_SHOWALLNAME_ON
			{ MSK_ALL,		KEY_SHIFT,  TRUE,	TRUE },				//ME_SHOWALLNAME_OFF
			{ MSK_NONE,		KEY_UP,		TRUE,	FALSE },			//ME_CHANGE_WHISPERLIST_UP
			{ MSK_NONE,		KEY_DOWN,	TRUE,	FALSE },			//ME_CHANGE_WHISPERLIST_DOWN
			{ MSK_ALT,		KEY_W,		FALSE,	FALSE },			//ME_TOGGLE_BIGMAP
			{ MSK_ALT,		KEY_P,		FALSE,	FALSE },			//ME_MUSSANG_ON
			{ MSK_ALT,		KEY_D,		FALSE,	FALSE },			//  ME_TOGGLE_KEY_SETTING_TIP_SHOW
			{ MSK_ALT,		KEY_D,		FALSE,	TRUE },			//	ME_TOGGLE_KEY_SETTING_TIP_HIDE

		},

		//DEFAULT_CHAT
		//CoAc ≫c¿eAßAI ¸AAⓒ·I A°·I AO½A ¼¼ÆA....
		{
			{ MSK_NONE,		KEY_ESCAPE,		TRUE, FALSE },			//	ME_TOGGLE_EXITDLG
//			{ MSK_CTRL,		KEY_CONTROL,	TRUE, FALSE },			//	ME_TOGGLE_UNGIMODE
			{ MSK_NONE,		KEY_1,	FALSE, FALSE },				//	ME_USE_QUICKITEM01
			{ MSK_NONE,		KEY_2,	FALSE, FALSE },				//	ME_USE_QUICKITEM02
			{ MSK_NONE,		KEY_3,	FALSE, FALSE },				//	ME_USE_QUICKITEM03
			{ MSK_NONE,		KEY_4,	FALSE, FALSE },				//	ME_USE_QUICKITEM04
			{ MSK_NONE,		KEY_5,	FALSE, FALSE },				//	ME_USE_QUICKITEM05
			{ MSK_NONE,		KEY_6,	FALSE, FALSE },				//	ME_USE_QUICKITEM06
			{ MSK_NONE,		KEY_7,	FALSE, FALSE },				//	ME_USE_QUICKITEM07
			{ MSK_NONE,		KEY_8,	FALSE, FALSE },				//	ME_USE_QUICKITEM08
			{ MSK_NONE,		KEY_9,	FALSE, FALSE },				//	ME_USE_QUICKITEM09
			{ MSK_NONE,		KEY_0,	FALSE, FALSE },				//	ME_USE_QUICKITEM10

			{ MSK_NONE,		KEY_S,	FALSE, FALSE },				//	ME_TOGGLE_MUGONGDLG
			{ MSK_NONE,		KEY_I,	FALSE, FALSE },				//	ME_TOGGLE_INVENTORYDLG
			{ MSK_NONE,		KEY_C,	FALSE, FALSE },				//	ME_TOGGLE_CHARACTERDLG
			{ MSK_NONE,		KEY_G,	FALSE, FALSE },				//	ME_TOGGLE_MUNPADLG
//			{ MSK_NONE,		KEY_P,	FALSE, FALSE },				//	ME_TOGGLE_PARTYDLG	//none

			{ MSK_NONE,		KEY_M,	FALSE, FALSE },				//	ME_TOGGLE_MINIMAP

			{ MSK_NONE,		KEY_Q,	FALSE, FALSE },				//	ME_TOGGLE_QUESTDLG
//			{ MSK_NONE,		KEY_ESCAPE,	FALSE, FALSE },			//	ME_TOGGLE_EXITDLG

//			{ MSK_NONE,		KEY_E,	FALSE, FALSE },				//	ME_APPLY_EXCHANGE
//			{ MSK_NONE,		KEY_W,	FALSE, FALSE },				//	ME_OPEN_STREETSTALL
			{ MSK_NONE,		KEY_O,	FALSE, FALSE },				//	ME_TOGGLE_OPTIONDLG
//			{ MSK_NONE,		KEY_X,	FALSE, FALSE },				//	ME_CLOSE_ALLWINDOW
			{ MSK_NONE,		KEY_F,	FALSE, FALSE },				//	ME_TOGGLE_FRIENDLIST
			{ MSK_NONE,		KEY_N,	FALSE, FALSE },				//	ME_SEND_MEMO
//			{ MSK_NONE,		KEY_V,	FALSE, FALSE },				//	ME_APPLY_VIMU

//			{ MSK_NONE,		KEY_ESCAPE,	FALSE, FALSE },			//	ME_CLOSE_ACTIVEDLG	//none

			{ MSK_NONE,		KEY_R,	FALSE, FALSE },				//	ME_TOGGLE_MOVEMODE
//			{ MSK_NONE,		KEY_T,	FALSE, FALSE },				//	ME_TOGGLE_GYUNGGONG
			
//			{ MSK_NONE,		KEY_0,	FALSE, FALSE },				//	ME_PAGEDN_QUICKSLOT
//			{ MSK_NONE,		KEY_9,	FALSE, FALSE },				//	ME_PAGEUP_QUICKSLOT

			{ MSK_NONE,		KEY_F1,	TRUE, FALSE },				//	ME_SELECT_QUICKSLOT1
			{ MSK_NONE,		KEY_F2,	TRUE, FALSE },				//	ME_SELECT_QUICKSLOT2
			{ MSK_NONE,		KEY_F3,	TRUE, FALSE },				//	ME_SELECT_QUICKSLOT3
			{ MSK_NONE,		KEY_F4,	TRUE, FALSE },				//	ME_SELECT_QUICKSLOT4


//			{ MSK_CTRL,		KEY_CONTROL,	FALSE, FALSE },			//	ME_TOGGLE_UNGIMODE
			{ MSK_NONE,		KEY_Z,		FALSE, FALSE },				//	ME_TOGGLE_PEACEWARMODE

			{ MSK_CTRL,		KEY_CONTROL, TRUE, FALSE },				//	ME_TOGGLE_AUTOATTACK
			{ MSK_NONE,		KEY_A,		FALSE, FALSE },				//	ME_TOGGLE_AUTOATTACK2

			{ MSK_NONE,		KEY_H,		FALSE, FALSE },				//	ME_TOGGLE_HELP

//			{ MSK_NONE,		KEY_HOME,	FALSE, FALSE },				//	ME_CAMERAMOVE_ZOOMIN
//			{ MSK_NONE,		KEY_END,	FALSE, FALSE },				//	ME_CAMERAMOVE_ZOOMOUT

			{ MSK_NONE,		KEY_INSERT,	TRUE, FALSE },			//	ME_CAMERAMODE_FREEVIEW/BACKVIRE TOGGLE

//			{ MSK_NONE,		KEY_INSERT,	FALSE, FALSE },			//	ME_CAMERAMODE_BACKVIEW		//none
//			{ MSK_NONE,		KEY_F1,		FALSE, FALSE },				//	ME_CAMERAMODE_QUARTERVIEW	//none
//			{ MSK_ALT,		KEY_F2,		FALSE, FALSE },				//	ME_CAMERAMODE_LOADVIEW		//none
//			{ MSK_ALT,		KEY_F3,		FALSE, FALSE },				//	ME_CAMERAMODE_SAVEVIEW		//none

			{ MSK_NONE,		KEY_F12,	TRUE, TRUE },				//	ME_SCREENCAPTURE

//			{ MSK_NONE,		KEY_UP,		FALSE, FALSE },				//	ME_CAMERAMOVE_UP
//			{ MSK_NONE,		KEY_DOWN,	FALSE, FALSE },				//	ME_CAMERAMOVE_DOWN
//			{ MSK_NONE,		KEY_RIGHT,	FALSE, FALSE },				//	ME_CAMERAMOVE_RIGHT
//			{ MSK_NONE,		KEY_LEFT,	FALSE, FALSE },				//	ME_CAMERAMOVE_LEFT

			{ MSK_SHIFT,	KEY_SHIFT,  FALSE, FALSE },				//ME_SHOWALLNAME_ON
			{ MSK_ALL,		KEY_SHIFT,  TRUE, TRUE },				//ME_SHOWALLNAME_OFF
			{ MSK_NONE,		KEY_UP,		TRUE,	FALSE },			//ME_CHANGE_WHISPERLIST_UP
			{ MSK_NONE,		KEY_DOWN,	TRUE,	FALSE },			//ME_CHANGE_WHISPERLIST_DOWN
			{ MSK_NONE,		KEY_W,		FALSE,	FALSE },			//ME_TOGGLE_BIGMAP
			{ MSK_NONE,		KEY_P,		FALSE,	FALSE },			//ME_MUSSANG_ON
			{ MSK_NONE,		KEY_D,		FALSE,	FALSE },			//  ME_TOGGLE_KEY_SETTING_TIP_SHOW
			{ MSK_NONE,		KEY_D,		FALSE,	TRUE },			//	ME_TOGGLE_KEY_SETTING_TIP_HIDE
			{ MSK_NONE,		KEY_T,	FALSE, FALSE },				//	ME_TOGGLE_TITANINVENTORYDLG
			
		}
	};

	sMACRO DefaultPressMacro[MM_COUNT][MPE_COUNT] = {
		{
			{ MSK_NONE,		KEY_PGUP,	FALSE, FALSE },				//	MPE_CAMERAMOVE_ZOOMIN
			{ MSK_NONE,		KEY_PGDN,	FALSE, FALSE },				//	MPE_CAMERAMOVE_ZOOMOUT

			{ MSK_NONE,		KEY_UP,		FALSE, FALSE },				//	MPE_CAMERAMOVE_UP
			{ MSK_NONE,		KEY_DOWN,	FALSE, FALSE },				//	MPE_CAMERAMOVE_DOWN
			{ MSK_NONE,		KEY_RIGHT,	FALSE, FALSE },				//	MPE_CAMERAMOVE_RIGHT
			{ MSK_NONE,		KEY_LEFT,	FALSE, FALSE },				//	MPE_CAMERAMOVE_LEFT
		},
		{
			{ MSK_NONE,		KEY_PGUP,	FALSE, FALSE },				//	MPE_CAMERAMOVE_ZOOMIN
			{ MSK_NONE,		KEY_PGDN,	FALSE, FALSE },				//	MPE_CAMERAMOVE_ZOOMOUT

			{ MSK_NONE,		KEY_UP,		FALSE, FALSE },				//	MPE_CAMERAMOVE_UP
			{ MSK_NONE,		KEY_DOWN,	FALSE, FALSE },				//	MPE_CAMERAMOVE_DOWN
			{ MSK_NONE,		KEY_RIGHT,	FALSE, FALSE },			//	MPE_CAMERAMOVE_RIGHT
			{ MSK_NONE,		KEY_LEFT,	FALSE, FALSE },				//	MPE_CAMERAMOVE_LEFT
		}
	};

////////////////////////////////////////////////

//	memcpy( m_MacroKey[MM_DEFAULT_CHAT], DefaultMacro, sizeof( sMACRO ) * ME_COUNT * 2 );

	memcpy( m_DefaultKey, DefaultMacro, sizeof( DefaultMacro ) );
	memcpy( m_DefaultPressKey, DefaultPressMacro, sizeof( DefaultPressMacro ) );

	LoadUserMacro();
}


void CMacroManager::SetDefaultMacro( int nMacroMode )
{
//	m_nMacroMode	= MM_DEFAULT_CHAT;
//	m_bChatMode		= TRUE;
//	m_nMacroMode	= MM_DEFAULT_GENERAL;
//	m_bChatMode		= FALSE;

//	m_nMacroMode	= MM_CHAT;
//	m_bChatMode		= TRUE;

	memcpy( m_MacroKey[nMacroMode], m_DefaultKey[nMacroMode], sizeof( sMACRO ) * ME_COUNT );

	memcpy( m_MacroPressKey[nMacroMode], m_DefaultPressKey[nMacroMode], sizeof( sMACRO ) * MPE_COUNT );
}


int CMacroManager::IsUsableKey( CKeyboard* keyInfo, WORD* wKey, BOOL bUserMode )
{
	static WORD wUsableSysKey[] =
	{
		KEY_CONTROL,
		KEY_MENU,
		KEY_SHIFT
	};

	static WORD wUsableCombineKey[] =
	{
		KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0,
		KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M,
		KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
		KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6,
		KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,

		KEY_EQUALS, KEY_SEMICOLON, KEY_OEM_3, KEY_SLASH, KEY_LBRACKET, KEY_RBRACKET, KEY_BACKSLASH,
		KEY_APOSTROPHE, KEY_MINUS, KEY_EQUALS, /*KEY_OEM_102,*/ KEY_COMMA, KEY_PERIOD, 

		KEY_SPACE, KEY_BACK, KEY_INSERT, KEY_DELETE, KEY_HOME, KEY_END, KEY_PGUP, KEY_PGDN,
		KEY_TAB, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,

		KEY_PAD0, KEY_PAD1, KEY_PAD2, KEY_PAD3, KEY_PAD4, KEY_PAD5, KEY_PAD6, KEY_PAD7, KEY_PAD8, KEY_PAD9,
		KEY_MULTIPLY, KEY_ADD, KEY_SUBTRACT, KEY_DECIMAL, KEY_DIVIDE
	};
	
	static WORD wUsableKey[] =
	{
		KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6,
		KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,

		KEY_SPACE, KEY_BACK, KEY_INSERT, KEY_DELETE, KEY_HOME, KEY_END, KEY_PGUP, KEY_PGDN,
		KEY_TAB, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,
	};

	static BOOL bSysKeyDown = FALSE;
	int i;

	//½A½ºAUA° ¸OAu E®AI( alt+tab, alt+f4 ) //altA° ¸·±a
	if( ( keyInfo->GetKeyDown( KEY_TAB ) || keyInfo->GetKeyDown( KEY_F4 ) )
		  && keyInfo->GetKeyPressed( KEY_MENU ) )
	{
		return 0;
	}


	if( bSysKeyDown || bUserMode == 1 )
	{
		for( i = 0 ; i < sizeof( wUsableCombineKey ) / sizeof( WORD ) ; ++i )
		{
			if( keyInfo->GetKeyDown( wUsableCombineKey[i] ) )
			{
				*wKey = wUsableCombineKey[i];
				return 1;
			}
		}
	}
	else
	{
		for( i = 0 ; i < sizeof( wUsableKey ) / sizeof( WORD ) ; ++i )
		{
			if( keyInfo->GetKeyDown( wUsableKey[i] ) )
			{
				*wKey = wUsableKey[i];
				return 1;
			}
		}
	}

	for( i = 0 ; i < sizeof( wUsableSysKey ) / sizeof( WORD ) ; ++i )
	{
		if( keyInfo->GetKeyPressed( wUsableSysKey[i] ) )
		{
			*wKey = wUsableSysKey[i];
			bSysKeyDown = TRUE;
			return 2;
		}
	}

	*wKey = 0x0000;
	bSysKeyDown = FALSE;
	return 0;
}


