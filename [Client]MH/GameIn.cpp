// GameIn.cpp: implementation of the CGameIn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameIn.h"
//#ifdef _CHEATENABLE_
#include "CheatMsgParser.h"
//#endif	//_CHEATENABLE_

#include "MainGame.h"
#include "WindowIdEnum.h"
#include "./Interface/cWindowManager.h"
#include "BattleSystem_client.h"

#include "SuryunDialog.h"
#include "MugongManager.h"
#include "ObjectManager.h"
#include "ItemManager.h"
#include "MoveManager.h"
#include "MHTimeManager.h"
#include "QuickManager.h"
#include "PeaceWarModeManager.h"
#include "PartyManager.h"
#include "UngijosikManager.h"
#include "ObjectActionManager.h"
#include "TacticManager.h"
#include "SkillManager_Client.h"
#include "GuildManager.h"
#include "KyungGongManager.h"
#include "StreetStallManager.h"
#include "PyogukManager.h"
#include "FriendManager.h"
#include "NoteManager.h"
#include "ChatManager.h"
#include "Hero.h"

//050630SW
#include "cChatTipManager.h"
//-
#include "ExchangeManager.h"
#include "NpcScriptManager.h"
//#include "HelpDicManager.h"
#include "HelperManager.h"
#include "./Audio/MHAudioManager.h"
#include "OptionManager.h"
#include "WantedManager.h"
#include "MHCamera.h"
#include "./Input/UserInput.h"
#include "MouseCursor.h"
#include "JournalManager.h"

#include "MainBarDialog.h"
#include "CharStateDialog.h"
#include "MiniMapDlg.h"
#include "PartyIconManager.h"
#include "ReviveDialog.h"
#include "MainBarDialog.h"
#include "WantNpcDialog.h"
#include "GuildTraineeDialog.h"

#include "mhMap.h"
#include "cMsgBox.h"

#include "MotionManager.h"

#include "MurimNet.h"
#include "CQuestManager.h"
#include "GameEventManager.h"
#include "objectstatemanager.h"
#include "PKManager.h"
#include "GMNotifyManager.h"
#include "UpgradeDlg.h"
#include "ReinforceDlg.h"
#include "DissloveDlg.h"
#include "NpcScriptDialog.h"
//#include "MixManager.h"
#include "StatusIconDlg.h"
#include "CharSelect.h"
#include "PetManager.h"
#include "PetStateDlg.h"
#include "PetStateMiniDlg.h"
#include "PetInventoryDlg.h"
#include "PetUpgradeDialog.h"
#include "PetRevivalDialog.h"

#include "Vimumanager.h"
#include "AbilityManager.h"
#include "AbilityUpdater_Interface.h"
#include "AbilityUpdater_ApplyData.h"

#include "UserInfoManager.h"
#include "ChatDialog.h"

//#include "cJackpotDialog.h"

#include "ExitManager.h"
#include "HackCheck.h"
#include "SocietyActManager.h"
#include "ShoutchatDialog.h"
#include "GTScoreInfoDialog.h"

#include "mmsystem.h"

#ifdef _GMTOOL_
#include "GMToolManager.h"
#endif

#include "Quest.h"
#include "QuestManager.h"

#include "../[CC]Suryun/SuryunManager_client.h"
#include "MapChange.h"
#include "EventMapInfo.h"
#include "GuildFieldWar.h"
#include "PartyWar.h"
#include "PartyWarDialog.h"

#include "ItemShopDialog.h"
#include "MoveDialog.h"
#include "EventNotifyDialog.h"

#include "PyogukDialog.h"
#include "GuildTournamentMgr.h"
//SW050715
#include "cJackpotManager.h"

#include "SkillDelayManager.h"
#include "GuildUnion.h"
#include "SeigeWarDialog.h"
#include "SiegeWarMgr.h"

//
#include "CharChangeDlg.h"
#include "GuageDialog.h"

#include "MussangManager.h"
#include "WeatherManager.h"
#include "PetManager.h"
#include "TitanManager.h"
#include "TitanRecallDlg.h"
#include "BobusangManager.h"

#include "ProgressBarDlg.h"

//스크린샷
#include "ScreenShotDlg.h"
#include "SurvivalModeManager.h"

#ifdef TAIWAN_LOCAL
#include "BillingManager.h"
#endif

#ifdef _JAPAN_LOCAL_
#include "InventoryExDialog.h"
#include "MugongDialog.h"
#endif
#ifdef _HK_LOCAL_
#include "InventoryExDialog.h"
#include "MugongDialog.h"
#endif
#ifdef _TL_LOCAL_
#include "InventoryExDialog.h"
#include "MugongDialog.h"
#endif

#include "AutoNoteManager.h"
#include "FortWarManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifndef _RESOURCE_WORK_NOT_APPLY_
#include "AppearanceManager.h"
#endif

#ifdef _TESTCLIENT_
#include "AppearanceManager.h"
#include "PathManager.h"

void InitTestCharacters(void);
void InitTestHero();
void InitTestMonster();
CMonster * g_pMonster = NULL;
CBossMonster * g_pBossMonster = NULL;


#endif

DWORD g_TempHeroID;	//수련장에서 씀
extern HWND _g_hWnd;

GLOBALTON(CGameIn)


CGameIn::CGameIn()
{
	m_bGameInAcked	= FALSE;
	m_bInitForGame	= FALSE;
	m_GameInInitKind = eGameInInitKind_Login;

	m_pMugongSuryunDlg	= NULL;
	m_pInventoryDlg		= NULL;
	m_pExchangeDlg		= NULL;
	m_pMixDlg			= NULL;
	m_pTitanPartsMakeDlg = NULL;	// magi82 - Titan(070112)
	// magi82 - Titan(070119) ////////////////////////////////////////////////
	m_pTitanMixDlg			= NULL;	
	m_pTitanRepairDlg		= NULL;	//2007. 9. 11. CBH - 타이탄 수리창
	m_pTitanRecallDlg		= NULL;	//2007. 9. 13. CBH - 타이탄 소환 게이지
	m_pTitanUpgradeDlg		= NULL;
	m_pTitanBreakDlg		= NULL;
	//////////////////////////////////////////////////////////////////////////
	m_pTitanChangePreViewDlg = NULL;	// magi82 - Titan(070212)
	m_pTitanInventoryDlg = NULL;		// magi82 - Titan(070222)
	m_pTitanGuageDlg	= NULL;			// magi82 - Titan(070305)
	//m_pTitanMugongMixDlg	= NULL;		// magi82 - Titan(070611) 타이탄 무공변환 주석처리
	m_pTitanRegisterDlg		= NULL;		// magi82 - Titan(070320)
	m_pTitanDissolutionDlg	= NULL;		// magi82 - Titan(070321)
	m_pDealDlg			= NULL;
	m_pCharDlg			= NULL;
	m_pChatDlg			= NULL;
//	m_pJackpotDlg		= NULL;
	m_pMainDlg			= NULL;
	m_pDebugDlg			= NULL;
// LYJ 051017 구입노점상 추가
	m_pStallKindSelectDlg	= NULL;
	m_pStreetStallDlg	= NULL;
	m_pNpcScriptDlg		= NULL;
	m_pHelpDlg			= NULL;
	m_pMacroDlg			= NULL;	
	m_pChatOptionDlg	= NULL;
	m_pOptionDlg		= NULL;
	m_pExitDlg			= NULL;
	m_pCharStateDlg		= NULL;

	m_pAuctionBoard		= NULL;

	m_MoneyDlg			= NULL;
	m_pQuickDlg			= NULL;
	m_pGuageDlg			= NULL;

	m_pMiniMapDlg		= NULL;
	m_pBigMapDlg		= NULL;

	m_pPyogukDialog		= NULL;
	m_pNoteDialog		= NULL;
	m_pFriendDialog		= NULL;
	m_pMiniNoteDialog	= NULL;
	m_pMiniFriendDialog	= NULL;
	m_pReviveDialog		= NULL;
	m_pWantNpcDialog	= NULL;

	m_pMonsterGuageDlg	= NULL;
	
	m_pBailDlg			= NULL;
	m_pDissolutionDlg	= NULL;
	m_pEventNotifyDlg	= NULL;
	
	m_pMPRegistDialog	= NULL;
	m_pMPMissionDialog	= NULL;
	m_pMPNoticeDialog	= NULL;
	m_pMPGuageDialog	= NULL;
	
	m_pItemShopDialog	= NULL;
	m_pMoveDialog		= NULL;
	m_pPointSaveDialog	= NULL;	

	m_pSkPointDlg		= NULL;
	m_pSkPointNotifyDlg = NULL;

	m_pPetStateDlg		= NULL;
	m_pPetStateMiniDlg	= NULL;
	m_pPetInventoryDlg	= NULL;
	m_pPetUpgradeDlg	= NULL;
	m_pPetRevivalDlg	= NULL;

	m_MovePoint = 0;
	m_MoveMap	= 0;
	m_GuildIdx	= 0;
	m_SiegeWarMapNum	= 0;

	m_pShoutDlg			= NULL;
	m_pShoutchatDlg		= NULL;
	m_pChaseDlg			= NULL;
	m_pChaseinputDlg	= NULL;
	m_pNameChangeDlg	= NULL;
	m_pNameChangeNotifyDlg	= NULL;

	m_pCharChangeDlg	= NULL;

	m_pSkillPointResetDlg	= NULL;	// magi82(42) - 샵아이템 추가(수련치 초기화)	

	m_pNumberPadDlg = NULL;

	// magi82(47)
	m_pStreetStallItemViewDlg = NULL;

	/////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환
	m_pSkillOptionChangeDlg = NULL;
	m_pSkillOptionClearDlg = NULL;
	/////////////////////////////////////////////////////////////

	m_bDieInBossMap = FALSE;
	m_nCountInBossMap = 0;
	m_dwTimeInBossMap = 0;
	
	//캐릭터 로그인 시간변수(서버,클라이언트) 초기화
	memset(&GameinServerTime, 0, sizeof(GameinServerTime));
	m_Clientdate = 0;
	m_Clienttime = 0;

	m_pUniqueItemCurseCancellationDlg = NULL;	// magi82 - UniqueItem(070703)
	m_pUniqueItemMixDlg				  = NULL;	// magi82 - UniqueItem(070709)

	m_pSkinSelectDlg = NULL;
	m_pCostumeSkinSelectDlg = NULL;

	m_pStallFindDlg = NULL; // 노점상 검색 추가 by Stiner(8)
}

CGameIn::~CGameIn()
{

}

#ifdef _TESTCLIENT_
void InitForTestClient()
{	
	MOTIONMGR->LoadMotionList();
	
	CHATMGR->Init();
	
	LOGFILE("SkillManager Init");
	SKILLMGR->Init();
	
	LOGFILE("LoadTestClientInfo");
	GAMERESRCMNGR->LoadTestClientInfo();
	
	LOGFILE("APPEARANCEMGR->Init()");
	APPEARANCEMGR->Init();
	

	LOGFILE("KYUNGGONGMGR->Init()");
	KYUNGGONGMGR->Init();
	LOGFILE("GAMERESRCMNGR->LoadMonsterList()");
	GAMERESRCMNGR->LoadMonsterList();
//	LOGFILE("GAMERESRCMNGR->LoadSMonsterList()");
//	GAMERESRCMNGR->LoadSMonsterList();
	LOGFILE("GAMERESRCMNGR->LoadNpcChxList()");
	GAMERESRCMNGR->LoadNpcChxList();

	// magi82(44) - 노점상 꾸미기 업데이트
	LOGFILE("GAMERESRCMNGR->LoadItemChxList()");
	GAMERESRCMNGR->LoadItemChxList();

	LOGFILE("GAMERESRCMNGR->LoadNpcList(GAMERESRCMNGR->m_NpcList)");
	GAMERESRCMNGR->LoadNpcList(GAMERESRCMNGR->m_NpcList);
	LOGFILE("GAMERESRCMNGR->LoadTacticStartInfoList()");
	GAMERESRCMNGR->LoadTacticStartInfoList();
	GAMERESRCMNGR->LoadTitanList();

	LOGFILE("ITEMMGR->LoadItemList()");
	ITEMMGR->LoadItemList();
	LOGFILE("ITEMMGR->LoadSetItemOption()");
	ITEMMGR->LoadSetItemOption();			//2007. 6. 14. CBH - 세트아이탬 데이터 로딩 추가
	LOGFILE("ITEMMGR->LoadItemTooltipList()");
	ITEMMGR->LoadItemToolTipList();
	LOGFILE("ITEMMGR->LoadRareItemInfo()");
	ITEMMGR->LoadRareItemInfo();
	
	LOGFILE("MAP->InitMap(GAMERESRCMNGR->m_TestClientInfo.Map)");
	MAP->InitMap(GAMERESRCMNGR->m_TestClientInfo.Map);
	PATHMGR->SetMap(MAP, GAMERESRCMNGR->m_TestClientInfo.Map, MAP->GetTileWidth());
//	LOGFILE("WINDOWMGR->CreatMousePoint()");
//	WINDOWMGR->CreatMousePoint();
	
	LOGFILE("EFFECTMGR->Init()");		
	EFFECTMGR->Init();

	LOGFILE("OPTIONMGR->ApplySettings()");
	OPTIONMGR->ApplySettings();

	GAMEIN->m_bGameInAcked = TRUE;

	ABILITYMGR->Init();
	ABILITYMGR->AddUpdater(CAbilityUpdater_Interface::GetInstance());
	ABILITYMGR->AddUpdater(CAbilityUpdater_ApplyData::GetInstance());

	WINDOWMGR->CreateGameIn();
}
#endif

BOOL CGameIn::InitForGame()
{
	m_bInitForGame = TRUE;

	WINDOWMGR->CreateGameIn();

	
	LOGFILE("HELPERMGR->Init()");
	HELPERMGR->Init();

	WINDOWMGR->AfterInit();
	
	if( FALSE == MAP->IsMapKind(eEventMap) )
		USERINFOMGR->LoadUserInfo( eUIK_INTERFACE );

//----------
//	LOGFILE("MACROMGR->Init()");
//	MACROMGR->Init();
//	MIXMGR->LoadMixList();

//	LOGFILE("NPCSCRIPTMGR->Init()");
//	NPCSCRIPTMGR->Init();
//	LOGFILE("HELPDICMGR->Init()");
//	HELPDICMGR->Init();
//----------

//´UAaA° AøÆA¼³A¤A§CØ¼­. ¸AAⓒ·IinitAIEA E￡Aa.
	if( GetMainInterfaceDialog() )
		GetMainInterfaceDialog()->Refresh();
	if( GetCharStateDialog() )
		GetCharStateDialog()->Refresh();

	LOGFILE("CHATMGR->SetChatDialog( (CChatDialog*)WINDOWMGR->GetWindowForID( CTI_DLG ) )");
	CHATMGR->SetChatDialog( (CChatDialog*)WINDOWMGR->GetWindowForID( CTI_DLG ) );
	CHATMGR->RestoreChatList();	

	QUESTMGR->InitializeQuest();
	PKMGR->Init();
	PETMGR->LoadPetImage();

	m_bDieInBossMap = FALSE;
	m_nCountInBossMap = 0;
	m_dwTimeInBossMap = 0;
	
	GUILDUNION->Clear();


	//SW050825 그래픽옵션Tab: GraphicAutoCtrlProcess()
	OPTIONMGR->InitForGameIn();

	return TRUE;
}

BOOL CGameIn::Init(void* pInitParam)
{
	int GameInInitKind = *(int*)pInitParam;
	m_GameInInitKind = GameInInitKind;
	m_bGameInAcked = FALSE;
	
//#	WINDOWMGR->CreateGameIn();
//#	WINDOWMGR->AfterInit();

#ifdef _TESTCLIENT_
	InitForTestClient();
#endif

	InitForGame();


#ifndef _RESOURCE_WORK_NOT_APPLY_

	APPEARANCEMGR->EnbaleReservation();

#endif

	//test
//	QUESTMGR->DeleteMapNpcList();

	//////////////////////////////////////////////////////////////////////////

	LOGFILE("MAP->LoadStaticNpc(MAP->GetMapNum())");
	if(MAP->GetMapNum() != 0)
		MAP->LoadStaticNpc(MAP->GetMapNum());


	VIMUMGR->Init();		//AE±aE­
	EXCHANGEMGR->Init();	//AE±aE­
	STREETSTALLMGR->Init();
	FRIENDMGR->Init();
	WANTEDMGR->Init();
	PARTYMGR->Init();
	JOURNALMGR->Init();
	GUILDMGR->InitGameIn();
	SURYUNMGR;
	EVENTMAP->Init();
	GUILDFIELDWAR->Init();
	PARTYWAR->Init();
	SIEGEMGR->Init();
	SWPROFIT->Init();
	MUSSANGMGR->Init();
	WEATHERMGR->Init();
	SVVMODEMGR->Init();
	FORTWARMGR->Init();

#ifdef _TESTCLIENT_
	LOGFILE("InitTestHero()");
	InitTestHero();
	LOGFILE("InitTestMonster()");
	InitTestMonster();
	LOGFILE("InitTestCharacters()");
	InitTestCharacters();
#endif
	LOGFILE("GAMEIN INIT COMPLETE");
	
#ifdef _GMTOOL_
	if( GMTOOLMGR->IsShowing() )
		GMTOOLMGR->ShowGMDialog( TRUE );
#endif

	
	NETWORK->SetCurState(this);

	switch(GameInInitKind)
	{
	case eGameInInitKind_Login:
	case eGameInInitKind_MapChange:
		{
			MSG_DWORD2 msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_GAMEIN_SYN;
			msg.dwData1	 = gChannelNum;
			msg.dwData2  = m_MoveMap;
			NETWORK->Send( &msg, sizeof(msg) );
			
		}
		break;
	case eGameInInitKind_SuryunEnter:
		{
			MSGBASE msg;
			msg.Category = MP_SURYUN;
			msg.Protocol = MP_SURYUN_ENTER_SYN;
			msg.dwObjectID = g_TempHeroID;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
	case eGameInInitKind_SuryunLeave:
		{
			MSGBASE msg;
			msg.Category = MP_SURYUN;
			msg.Protocol = MP_SURYUN_LEAVE_SYN;
			msg.dwObjectID = g_TempHeroID;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
		
	case eGameInInitKind_MovePos:
		{
 			MSG_DWORD2 msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_GAMEINPOS_SYN;
			msg.dwObjectID = g_TempHeroID;
			msg.dwData1 = gChannelNum;
			if( GUILDMGR->GetSOSChannelIdx() >= 0 )
			{
				msg.dwData1 = GUILDMGR->GetSOSChannelIdx();
				gChannelNum = GUILDMGR->GetSOSChannelIdx();
				GUILDMGR->SetSOSChannelIdx(-1);

			}

			msg.dwData2 = m_MovePoint;

			NETWORK->Send( &msg, sizeof(msg) );			
		}
		break;

	case eGameInInitKind_EventMapEnter:
		{
			MSG_DWORD3 msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_ENTER_EVENTMAP_SYN;
			msg.dwObjectID = g_TempHeroID;
			msg.dwData1 = EVENTMAP->GetEventMapNum();
			msg.dwData2 = EVENTMAP->GetChannelNum();
			msg.dwData3 = EVENTMAP->GetTeamNum();

			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
	case eGameInInitKind_GTEnter:
		{
			MSG_DWORD3 msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_BATTLEJOIN_SYN;
			msg.dwObjectID = g_TempHeroID;
			msg.dwData1 = m_GuildIdx;
			msg.dwData2 = 0;
			msg.dwData3 = m_MoveMap;
			NETWORK->Send( &msg, sizeof(msg) );
			
			m_MoveMap = 0;
		}
		break;
	case eGameInInitKind_GTLeave:
		{
			MSGBASE msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_LEAVE_SYN;
			msg.dwObjectID = g_TempHeroID;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
	case eGameInInitKind_GTObserverEnter:
		{
			MSG_DWORD3 msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_OBSERVERJOIN_SYN;
			msg.dwObjectID = g_TempHeroID;
			msg.dwData1 = m_GuildIdx;
			msg.dwData2 = m_BattleIdx;
			msg.dwData3 = m_MoveMap;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
	case eGameInInitKind_SWEnter:
		{
			MSG_DWORD4 msg;
			msg.Category = MP_SIEGEWAR;
			msg.Protocol = MP_SIEGEWAR_BATTLEJOIN_SYN;
			msg.dwObjectID = g_TempHeroID;
			msg.dwData1 = m_GuildIdx;			// Guild Idx
			msg.dwData2 = m_MoveMap;			// ReturnMapNum
			msg.dwData3 = 0;					// bObserver
			msg.dwData4 = m_SiegeWarMapNum;		// SiegeWarMapNum
			NETWORK->Send( &msg, sizeof(msg) );
			
			m_MoveMap = 0;
			m_SiegeWarMapNum = 0;
		}
		break;
	case eGameInInitKind_SWLeave:
		{
			MSGBASE msg;
			msg.Category = MP_SIEGEWAR;
			msg.Protocol = MP_SIEGEWAR_LEAVE_SYN;
			msg.dwObjectID = g_TempHeroID;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
	case eGameInInitKind_SWObserverEnter:
		{
			MSG_DWORD4 msg;
			msg.Category = MP_SIEGEWAR;
			msg.Protocol = MP_SIEGEWAR_OBSERVERJOIN_SYN;
			msg.dwObjectID = g_TempHeroID;
			msg.dwData1 = m_GuildIdx;			// Guild Idx
			msg.dwData2 = m_MoveMap;			// ReturnMapNum
			msg.dwData3 = 1;					// bObserver
			msg.dwData4 = m_SiegeWarMapNum;		// SiegeWarMapNum
			NETWORK->Send( &msg, sizeof(msg) );
			
			m_MoveMap = 0;
			m_SiegeWarMapNum = 0;
		}
		break;
	case eGameInInitKind_SVMapLeave:
		{
			MSGBASE msg;
			msg.Category = MP_SURVIVAL;
			msg.Protocol = MP_SURVIVAL_LEAVE_SYN;
			msg.dwObjectID = g_TempHeroID;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
	default:
		ASSERT(0);
	}

	// debug용
	ITEMMGR->m_nItemUseCount = 0;
	SKILLMGR->m_nSkillUseCount = 0;

	return TRUE;
}

void CGameIn::ReleaseForGame()
{
	if( !m_bInitForGame ) return;

	WINDOWMGR->DestroyWindowAll();
	HELPERMGR->Release();


	//TESTQUESTMNG->Release();
	m_bInitForGame = FALSE;

	m_pMugongDlg			= NULL;
	m_pInventoryDlg			= NULL;
	m_pExchangeDlg			= NULL;
	m_pMixDlg				= NULL;
	m_pTitanPartsMakeDlg	= NULL;	// magi82 - Titan(070112)
	// magi82 - Titan(070119) ////////////////////////////////////////////////
	m_pTitanMixDlg			= NULL;	
	m_pTitanRepairDlg		= NULL;	//2007. 9. 11. CBH - 타이탄 수리창
	m_pTitanRecallDlg		= NULL;	//2007. 9. 13. CBH - 타이탄 소환 게이지
	m_pTitanUpgradeDlg		= NULL;
	m_pTitanBreakDlg		= NULL;
	//////////////////////////////////////////////////////////////////////////
	m_pTitanChangePreViewDlg = NULL;	// magi82 - Titan(070212)
	m_pTitanInventoryDlg	= NULL;		// magi82 - Titan(070222)
	m_pTitanGuageDlg		= NULL;		// magi82 - Titan(070305)
	//m_pTitanMugongMixDlg	= NULL;		// magi82 - Titan(070611) 타이탄 무공변환 주석처리
	m_pTitanRegisterDlg		= NULL;		// magi82 - Titan(070320)
	m_pTitanDissolutionDlg	= NULL;		// magi82 - Titan(070321)
	m_pDealDlg				= NULL;
	m_pCharDlg				= NULL;
	m_pChatDlg				= NULL;
//	m_pJackpotDlg			= NULL;
	m_pMainDlg				= NULL;
	m_pDebugDlg				= NULL;
// LYJ 051017 구입노점상 추가
	m_pStallKindSelectDlg	= NULL;
	m_pStreetStallDlg		= NULL;
	m_pNpcScriptDlg			= NULL;
	m_pHelpDlg				= NULL;
	m_pMacroDlg				= NULL;	
	m_pChatOptionDlg		= NULL;
	m_pOptionDlg			= NULL;
	m_pExitDlg				= NULL;
	m_pCharStateDlg			= NULL;

	m_pAuctionBoard			= NULL;

	m_MoneyDlg				= NULL;
	m_pQuickDlg				= NULL;
	m_pGuageDlg				= NULL;

	m_pMiniMapDlg			= NULL;
	m_pBigMapDlg			= NULL;

	m_pPyogukDialog			= NULL;
	m_pNoteDialog			= NULL;
	m_pFriendDialog			= NULL;
	m_pMiniNoteDialog		= NULL;
	m_pMiniFriendDialog		= NULL;
	m_pReviveDialog			= NULL;
	m_pWantNpcDialog		= NULL;

	m_pMonsterGuageDlg		= NULL;

	m_pUpgradeDlg			= NULL;
	m_pReinforceDlg			= NULL;

	m_pBailDlg				= NULL;
	m_pDissolutionDlg		= NULL;
	m_pEventNotifyDlg		= NULL;
	
	m_pShoutDlg				= NULL;
	m_pShoutchatDlg			= NULL;

	m_pChaseDlg				= NULL;
	m_pChaseinputDlg		= NULL;
	
	m_pNameChangeDlg		= NULL;
	m_pNameChangeNotifyDlg	= NULL;	

	m_pCharChangeDlg		= NULL;

	m_pUniqueItemCurseCancellationDlg = NULL;	// magi82 - UniqueItem(070703)
	m_pUniqueItemMixDlg = NULL;	// magi82 - UniqueItem(070709)

	m_pUniqueItemMixProgressBarDlg = NULL;		//2007. 10. 22. CBH - 유니크 아이탬 조합 처리 바 게이지
	m_pTitanMixProgressBarDlg = NULL;		//2007. 10. 22. CBH - 타이탄 조합 처리 바 게이지
	m_pTitanPartsProgressBarDlg = NULL;		//2007. 10. 23. CBH - 타이탄 파츠 조합 처리 바 게이지

	m_pSkillPointResetDlg	= NULL;	// magi82(42) - 샵아이템 추가(수련치 초기화)

	// magi82(47)
	m_pStreetStallItemViewDlg = NULL;
	m_pStallFindDlg = NULL; // 노점상 검색 추가 by Stiner(8)
}

void CGameIn::Release(CGameState* pNextGameState)
{
#ifndef _RESOURCE_WORK_NOT_APPLY_

	APPEARANCEMGR->DisableReservation();

#endif

	WEATHERMGR->Release();
//
	BATTLESYSTEM->ReleaseBattle();
//

	STATUSICONDLG->Release();
	
	//KES 020828 AO½A·I...
//	WINDOWMGR->GetMouseWindow()->SetActive( TRUE );
	CURSOR->SetActive( FALSE );
	CHATMGR->SetChatDialog( NULL );

	///¿ⓒ±a±iAo AO½A
	AUDIOMGR->StopAll();
	QUICKMGR->Release();
	ITEMMGR->Release();
	MUGONGMGR->Release();
	QUESTMGR1->Release();
	PKMGR->Release();

//	MINIMAP->Release();
	
	if(m_GameInInitKind != eGameInInitKind_SuryunEnter && m_GameInInitKind != eGameInInitKind_EventMapEnter)
		USERINFOMGR->SaveUserInfo( eUIK_USERSTATE | eUIK_INTERFACE );	//HERO°¡ ¾ø¾iAo±a Au¿¡.

	EFFECTMGR->RemoveAllEffect();
	OBJECTMGR->RemoveAllObject();
	


//#	WINDOWMGR->DestroyWindowAll();
//#	HELPERMGR->Release();
	
/*
	RESRCMGR->ReleaseResource(14);
	RESRCMGR->ReleaseResource(15);
	RESRCMGR->ReleaseResource(98);
	RESRCMGR->ReleaseResource(30);
	RESRCMGR->ReleaseResource(40);
*/
	MAP->Release();

	// 	
	PICONMGR->DeleteAllPartyIcon();

	ReleaseForGame();
	
	PETMGR->PetMgrRelease();

	TITANMGR->TitanMgrRelease();
	
	EVENTMAP->Release();

	HACKCHECK->Release();



#ifdef _GMTOOL_
	//disable
	GMTOOLMGR->ShowGMDialog( FALSE );
#endif
	
//RESRCMGR->ReleaseResource();
	
	g_pExecutive->UnloadAllPreLoadedGXObject(0);	//NULLAß°¡

	g_TempHeroID = HEROID;
	HEROID = 0;
	gUserID = 0;
}


void CGameIn::Process()
{
///	MHTIMEMGR->Process();

	if(!m_bGameInAcked)
		return;

//	WINDOWMGR->Process();

//	if(!WINDOWMGR->IsMouseInputProcessed())
//	{
//		g_UserInput.Process();
//	}

	/*
	
		if( MOUSE->RButtonUp() )
		{
			//static int a = 0;
			int a = rand() % 2;
			if(a == 0)
			{
				if( OBJECTSTATEMGR->IsEndStateSetted(g_pBossMonster) == TRUE )
				{
					return;
				}
				OBJECTSTATEMGR->StartObjectState(g_pBossMonster, eObjectState_Rest);
			}
			else //if( a == 1 )
			{
				if( OBJECTSTATEMGR->IsEndStateSetted(g_pBossMonster) == TRUE )
				{
					return;
				}
				DWORD time = g_pBossMonster->GetEngineObject()->GetAnimationTime(eObjectState_Rest);
				OBJECTSTATEMGR->EndObjectState(g_pBossMonster, eObjectState_Rest, time);
			}
			
				//g_pBossMonster->SetState(eObjectState_Move);
			//else
				//g_pBossMonster->SetState(eObjectState_None);
		}*/

	////////////////////////////////////////////////////////////////////////
	////061009 BB
	////10초당 프레임 체크
	//m_uiCurrentTime = timeGetTime();
	//m_iMilliSecend += ( m_uiCurrentTime - m_uiPreTime );

	//if( m_iMilliSecend >= 10000 ) //1초 = 1000
	//{
	//	m_fFramePerSec = (float)m_iFPSCnt;
	//	m_iFPSCnt = m_iMilliSecend = 0;

	//	OBJECTMGR->ChangeFlag();
	//}
	//m_uiPreTime = m_uiCurrentTime;
	////////////////////////////////////////////////////////////////////////
	
	// LBS 04.01.06 Test
	GAMEEVENTMGR->Process();
//050630SW
	CHATTIPMGR->Process();
	
//	CAMERA->Process();	
	GetMiniMapDialog()->Process();

//SW050722 attach to minimap
//	//CSW050718
//	GetJackpotDialog()->Process();
	
	EXITMGR->Process();
	HACKCHECK->Process();
	ITEMMGR->Process();

#ifdef _JAPAN_LOCAL_
	PARTYMGR->Process();
#endif

	//SW050719
	//JACKPOTMGR->Process();

	GetShoutchatDlg()->Process();
	GetGTScoreInfoDlg()->Process();
	GetCharChangeDlg()->Process();
	GetSWEngraveDlg()->Process();
	GetSWTimeDlg()->Process();

	ProcessInBossMap();
	//WEATHERMGR->Process();
	
	//SW050825 그래픽옵션Tab: GraphicAutoCtrlProcess()

	OPTIONMGR->GraphicAutoCtrlProcess();

}

void CGameIn::BeforeRender()
{
	MAP->Process(gCurTime);
}

void CGameIn::AfterRender()
{
	if(m_bGameInAcked)
	{
		EFFECTMGR->Process(gCurTime);

//		STATUSICONDLG->Render();	//windowmanager에서 해준다(툴팁때문에)
		BATTLESYSTEM->Render();
		WEATHERMGR->Render();
		WINDOWMGR->Render();
		NOTIFYMGR->Render();	//KES
		EVENTMAP->Render();

		PARTYWAR->Render();
	}
	
}

void CGameIn::NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg)
{
	switch(Category)	//¿i¼±¼øA§¸|....
	{
	case MP_MOVE:			MOVEMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_SKILL:			SKILLMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_CHAR:			CHARCALCMGR->NetworkMsgParse(Protocol, pMsg);			break;
	case MP_SURYUN:			SURYUNMGR->NetworkMsgParse(Protocol,pMsg);				break;
	case MP_WEATHER:		WEATHERMGR->NetworkMsgParse(Protocol,pMsg);				break;
	case MP_USERCONN:
		{
			switch(Protocol)
			{
			case MP_USERCONN_OBJECT_REMOVE:
				{
					MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
					
//					LOGFILE("MP_USERCONN_OBJECT_REMOVE\t%u",pmsg->dwData);
					
//					GetMiniMapDialog()->RemoveIcon(pmsg->dwData);

					CObject* pObject = OBJECTMGR->GetObject(pmsg->dwData);
					if(pObject == NULL)
					{
//						LOGFILE("NO_OBJECT\t%u",pmsg->dwData);
						return;
					}

//					LOGFILE("AddGarbageObject\t%u",pmsg->dwData);
					if(pObject->GetObjectKind() == eObjectKind_Pet)
					{
						PETMGR->OnPetRemove((CPet*)pObject);
					}

					if(pObject->GetObjectKind() == eObjectKind_Player)
					{
						/*
						CPet* pPet = ((CPlayer*)pObject)->GetPet();
						if(pPet)
						{
							pPet->SetMaster(NULL);	//펫 오브젝트는 주인정보를 가지고 있어야 주인 GridIn 시 제대로 행동.
						}*/
						((CPlayer*)pObject)->SetPet(NULL);

						// magi82(44) - 노점상 꾸미기 업데이트
						// 플레이어가 Remove 될때 꾸미기 객체도 같이 지워준다.
						OBJECTMGR->RemoveDecoration((CPlayer*)pObject);
					}

					//!!!보부상 거래 중 상태해제 안되는 현상 발생시 넣을 코드
					/*if( pObject->GetObjectKind() == eObjectKind_Npc )
					{
						if( ((CNpc*)pObject)->GetNpcJob() == BOBUSANG_ROLE )
						{
							if( BOBUSANGMGR->GetOnDealing() == TRUE )
							{
								if( HERO->GetState() != eObjectState_Die )
									GAMEIN->GetDealDialog()->SetActive(FALSE);
							}
						}
					}*/

#ifndef _RESOURCE_WORK_NOT_APPLY_

					APPEARANCEMGR->CancelAlphaProcess(pObject);
					APPEARANCEMGR->CancelReservation(pObject);
#endif

					OBJECTMGR->AddGarbageObject(pObject);
					
					//GetDebugDlg()->DebugMsgParser(DBG_CHAT, "OBJECT_REMOVE %d", pmsg->dwData);
				}
				break;
			case MP_USERCONN_PET_DIE:
				{
					MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
					//CObject* pObject = OBJECTMGR->GetObject(pmsg->dwData2);
					//if(pObject)	return;
					CPet* pPet = (CPet*)OBJECTMGR->GetObject(pmsg->dwData2);
					if(!pPet)	return;

					if(pPet == PETMGR->GetCurSummonPet())
					{
						DWORD SummonItemDBIdx = pPet->GetPetSommonItemDBIdx();
						PET_TOTALINFO* pInfo = PETMGR->GetPetInfo(SummonItemDBIdx);
						if(!pInfo)
						{
							ASSERT(0);
							break;
						}
						pInfo->bAlive = FALSE;
						pInfo->PetFriendly = 0;

						PETMGR->SetPetGuageText( pInfo->PetStamina, pInfo->PetFriendly );
						ITEMMGR->RefreshItemToolTip(SummonItemDBIdx);
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1261) );

						DWORD RedFilter = RGBA_MAKE(255,10,10,255);
						ITEMMGR->SetIconColorFilter(pInfo->PetSummonItemDBIdx,RedFilter);

						PETMGR->ClosePetAllDlg();
					}
//					EFFECTMGR->StartEffectProcess(eEffect_PetSummon,pPet,&set,0,pPet->GetID());
					pPet->Die();
					//pPet->SetDieFlag();
				}
				break;
			case MP_USERCONN_MONSTER_DIE:
				{
					MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
					CObject* pObject = OBJECTMGR->GetObject(pmsg->dwData2);
					if(!pObject)
						return;
					pObject->SetDieFlag();

					// Game event check
					if( pmsg->dwData1 == HERO->GetID() )
					{
//						GAMEEVENTMGR->AddEvent(eGameEvent1_MonsterKill, ((CMonster*)pObject)->GetMonsterKind());
						GAMEEVENTMGR->AddEvent(eGameEvent_Hunt);
					}
					//!SW050719
//					CMonster* pMon = (CMonster*)pObject;
//					if( HERO->GetLevel() - pMon->GetLevel() < 6 )
//					//HERE cObjectManager.h
//					//virtual LEVELTYPE GetLevel(){ return m_pSInfo->Level; } CMonster.h 추가요망
//					JACKPOTMGR->AddFakeMoneyForDraw();
				}
				break;
			case MP_USERCONN_CHARACTER_DIE:
				{
					MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
					CObject* pObject = OBJECTMGR->GetObject(pmsg->dwData2); //dwData2 = playerID
					if(!pObject)
						return;
					pObject->SetDieFlag();
				}
				break;
				
			case MP_USERCONN_READY_TO_REVIVE:
				{
					if( MAP->IsMapKind(eEventMap) )
						return;
					GAMEIN->GetReviveDialog()->SetActive(TRUE);
					
					// magi82 - SOS(070724)
					//if( GAMEIN->GetSOSDlg()->IsActive() )
					//	GAMEIN->GetSOSDlg()->SetActive( FALSE );

					if( GAMEIN->GetPyogukDialog()->IsActive() )
						GAMEIN->GetPyogukDialog()->SetActive( FALSE );
					
					//if( MAP->GetMapNum() == BOSSMONSTER_MAP || MAP->GetMapNum() == BOSSMONSTER_2NDMAP )
					if( MAP->IsMapKind(eBossMap) )
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(929) );

						m_bDieInBossMap = TRUE;
						m_dwTimeInBossMap = gCurTime + 50000;
						m_nCountInBossMap = 10;
					}
				}
				break;

			case MP_USERCONN_CHARACTER_REVIVE:
				{

					MOVE_POS* pmsg = (MOVE_POS*)pMsg;
					VECTOR3 pos;
					CObject* pObject = OBJECTMGR->GetObject(pmsg->dwMoverID);
					if( pObject )
					{
						pmsg->cpos.Decompress(&pos);
						OBJECTACTIONMGR->Revive(pObject,&pos);
						if( pObject == HERO )
						{
							GAMEIN->GetReviveDialog()->SetDisable(FALSE);
							GAMEIN->GetReviveDialog()->SetActive(FALSE);
							
							cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_REVIVECONFIRM );
							if( pDlg )
								WINDOWMGR->AddListDestroyWindow( pDlg );

							m_bDieInBossMap = FALSE;
						}
					}
				}
				break;
				
			case MP_USERCONN_CHARACTER_REVIVE_NACK:
				{
					MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
					GAMEIN->GetReviveDialog()->SetDisable(FALSE);
				}
				break;

			case MP_USERCONN_NPC_ADD:
				{
					SEND_NPC_TOTALINFO* pmsg = (SEND_NPC_TOTALINFO*)pMsg;
					
					BASEMOVE_INFO moveInfo;
					moveInfo.SetFrom(&pmsg->MoveInfo);
					CAddableInfoIterator iter(&pmsg->AddableInfo);
					BYTE AddInfoKind;
					while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
					{
						ySWITCH(AddInfoKind)
							yCASE(CAddableInfoList::MoveInfo)
								COMPRESSEDTARGETPOS tpos;
								iter.GetInfoData(&tpos);
								moveInfo.bMoving = TRUE;
								moveInfo.SetMaxTargetPosIdx(tpos.PosNum);
								ASSERT( tpos.PosNum <= MAX_CHARTARGETPOSBUF_SIZE );
								moveInfo.SetCurTargetPosIdx(0);
								for(int n=0;n<tpos.PosNum;++n)
									tpos.pos[n].Decompress(moveInfo.GetTargetPosition(n));
							yCASE(CAddableInfoList::VimuInfo)
							yCASE(CAddableInfoList::StreetStall)
						yENDSWITCH
						
						iter.ShiftToNextData();
					}				
					
					CNpc* pNpc = OBJECTMGR->AddNpc(&pmsg->BaseObjectInfo,&moveInfo,&pmsg->TotalInfo);
					MOVEMGR->SetAngle(pNpc,pmsg->Angle,0);

					// Npc Add
					QUESTMGR->SetNpcData( pNpc );

					//GetDebugDlg()->DebugMsgParser(DBG_CHAT, "NPC_ADD %d", pmsg->BaseObjectInfo.dwObjectID);
				}
				break;
			case MP_USERCONN_GRIDINIT:
				{
					DeleteFile("./Log/DEBUG.txt");
					m_bGameInAcked = TRUE;

					g_UserInput.GetKeyboard()->ClearKeyState();
					g_UserInput.GetKeyboard()->UpdateKeyboardState();
					g_UserInput.SetInputFocus( TRUE );
					USERINFOMGR->LoadUserInfo( eUIK_USERSTATE );
				}
				break;

			case MP_USERCONN_GAMEIN_ACK:
				{
					ASSERT(!m_bGameInAcked);

					SEND_HERO_TOTALINFO * pmsg = (SEND_HERO_TOTALINFO *)pMsg;
					ASSERT(!(pmsg->SendMoveInfo.CurPos.wx == 0 && pmsg->SendMoveInfo.CurPos.wz == 0));

					//캐릭터 서버 로그인시간, 캐릭터 클라이언트 로그인시간 기록
					GAMEIN->SetLoginTime(pmsg->ServerTime);
					GAMEIN->SetClientLoginTime(MHTIMEMGR->GetMHDate(), MHTIMEMGR->GetMHTime());

					TITANMGR->SetServerLogInTime();

					OBJECTMGR->RegisterHero(pmsg);
					
					GAMEIN->GetMiniMapDialog()->AddHeroIcon(HERO);
										
					// GRIDINIT[9/15/2003]
					//m_bGameInAcked = TRUE;
					
					//////////////////////////////////////////////////////////////////////////

					EFFECTMGR->StartHeroEffectProcess(eEffect_NewCharacter);

					//option setting
					OPTIONMGR->ApplySettings();	//½A¾ß..
					OPTIONMGR->SendOptionMsg();

					FRIENDMGR->LogInNotify();
					
					VECTOR2 sidpos;
					sidpos.x = 140;
					sidpos.y = 0;
					STATUSICONDLG->Init(HERO,&sidpos,8);

					// event add
//					GAMEEVENTMGR->AddEvent(eGameEvent1_GameIn, MAP->GetMapNum());
					GAMEEVENTMGR->AddEvent(eGameEvent_MapChange, MAP->GetMapNum());

					//SW061124 무공 비습득 초보자 도움말
					if( HERO->GetLevel() > 4 && !MUGONGMGR->GetMugongNum())
					{
						GAMEEVENTMGR->AddEvent(eGameEvent_LevelUp, /*HEROLevel*/6);
					}
					
					QUESTMGR->ProcessNpcMark();

#ifdef TAIWAN_LOCAL
					if( m_GameInInitKind == eGameInInitKind_Login )
						BILLINGMGR->DisplayRemainTime();
#endif

					// eventmap
					EVENTMAP->AddHero();

					//event notify
					if( GAMEIN->GetEventNotifyDialog() )
					{
						if( NOTIFYMGR->IsEventNotifyUse() && ( NOTIFYMGR->IsEventNotifyChanged() || GAMEIN->GetEventNotifyDialog()->IsActive() ) )
						{
							GAMEIN->GetEventNotifyDialog()->SetTitle( NOTIFYMGR->GetEventNotifyTitle() );
							GAMEIN->GetEventNotifyDialog()->SetContext( NOTIFYMGR->GetEventNotifyContext() );
							GAMEIN->GetEventNotifyDialog()->SetActive( TRUE );
							//사운드?
							NOTIFYMGR->SetEventNotifyChanged( FALSE );
						}
						else
						{
							GAMEIN->GetEventNotifyDialog()->SetActive( FALSE );
						}
					}

					ITEMMGR->RefreshStatsBuffIcon();

					if( HERO->InTitan() )
					{
						CAMERA->InitCamera( 0, CAMERA->GetCameraDesc()->m_AngleX.ToDeg(), CAMERA->GetCameraDesc()->m_AngleY.ToDeg(),
							1500, HERO, TRUE );
						MAP->ToggleSunPosForShadow( TRUE );
					}
					else
					{
						CAMERA->Init(HERO,CAMERA->GetCameraDesc()->m_AngleX.ToDeg(),CAMERA->GetCameraDesc()->m_AngleY.ToDeg(),1000);
						MAP->ToggleSunPosForShadow( FALSE );
					}

					CAMERA->SetTargetAngleY(0,0.f,0);
					CAMERA->SetTargetAngleX(0,29.9f,0);
					//CAMERA->SetTargetAngleX(0,5.f,1500);
#ifdef _JAPAN_LOCAL_
//===하드코딩 T_T
					GetInventoryDialog()->SetActive( GetInventoryDialog()->IsActive() );
					GetMugongDialog()->SetActive( GetMugongDialog()->IsActive() );
#endif
#ifdef _HK_LOCAL_
					GetInventoryDialog()->SetActive( GetInventoryDialog()->IsActive() );
					GetMugongDialog()->SetActive( GetMugongDialog()->IsActive() );
#endif
#ifdef _TL_LOCAL_
					GetInventoryDialog()->SetActive( GetInventoryDialog()->IsActive() );
					GetMugongDialog()->SetActive( GetMugongDialog()->IsActive() );
#endif
					GAMEIN->SetMoveMap( MAP->GetMapNum() );

					NPCSCRIPTMGR->SetSelectedNpc(NULL);

				}
				return;
			case MP_USERCONN_GAMEIN_NACK:
				{
					__asm int 3;
				}
				return;

			case MP_USERCONN_CHARACTER_ADD:
				{

#ifndef _RESOURCE_WORK_NOT_APPLY_

					SEND_CHARACTER_TOTALINFO* pmsg = (SEND_CHARACTER_TOTALINFO*)pMsg;
					BASEMOVE_INFO moveInfo;					
					moveInfo.SetFrom(&pmsg->MoveInfo);

					CAddableInfoIterator iter(&pmsg->AddableInfo);
					BYTE AddInfoKind;
					while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
					{
						ySWITCH(AddInfoKind)
							yCASE(CAddableInfoList::MoveInfo)
								COMPRESSEDTARGETPOS tpos;
								iter.GetInfoData(&tpos);
								moveInfo.bMoving = TRUE;
								moveInfo.SetMaxTargetPosIdx(tpos.PosNum);
								ASSERT( tpos.PosNum <= MAX_CHARTARGETPOSBUF_SIZE );
								moveInfo.SetCurTargetPosIdx(0);
								for(int n=0;n<tpos.PosNum;++n)
									tpos.pos[n].Decompress(moveInfo.GetTargetPosition(n));							
						yENDSWITCH
						
						iter.ShiftToNextData();
					}

					CPlayer* pPlayer = OBJECTMGR->AddPlayerThroughCache(&pmsg->BaseObjectInfo,&moveInfo,&pmsg->TotalInfo,&pmsg->ShopItemOption, pmsg);

					//SW071129 추가. 나란히 한방향 보고 앉아있는 노점 제거!
					VECTOR3 temp;
					temp.x = pmsg->MoveInfo.Move_Direction.x;
					temp.z = pmsg->MoveInfo.Move_Direction.y;
					float fDirDeg = RADTODEG(VECTORTORAD(temp));
					MOVEMGR->SetAngle(pPlayer, fDirDeg, 0);
					
					// Add Player가 완료된 시점에서 PostAddPlayer()가 호출됩니다. 
					// Effect등을 붙일 때 Player model 을 미리 불러버리는 통에 
					// 어쩔 수 없이 흐름을 바꿨습니다. 양해해 주십시오. 
									

					// eventmap
					EVENTMAP->AddPlayer( pPlayer );

					CPet* pPet = OBJECTMGR->FindPlayerOwnPet( pPlayer->GetID() );
					if(pPet)
					{
						//pPet
						//pPet->SetPetMasterName(pPlayer->GetID());	//주인 설정
						pPlayer->SetPet(pPet);
						pPet->SetMaster(pPlayer);
					}
#else

					SEND_CHARACTER_TOTALINFO* pmsg = (SEND_CHARACTER_TOTALINFO*)pMsg;
					BASEMOVE_INFO moveInfo;
					BOOL bVimu = FALSE;
					BOOL bPartyWar = FALSE;
					WORD wPartyTeam = 0;
					WORD StallKind = 0;
					COMPRESSEDPOS VimuPos;
					static char StreetStallTitle[MAX_STREETSTALL_TITLELEN+1] = {0,};
					BOOL bInTitan = FALSE;
					TITAN_APPEARANCEINFO titanAppearance;

					moveInfo.SetFrom(&pmsg->MoveInfo);
					CAddableInfoIterator iter(&pmsg->AddableInfo);
					BYTE AddInfoKind;
					while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
					{
						ySWITCH(AddInfoKind)
							yCASE(CAddableInfoList::MoveInfo)
								COMPRESSEDTARGETPOS tpos;
								iter.GetInfoData(&tpos);
								moveInfo.bMoving = TRUE;
								moveInfo.SetMaxTargetPosIdx(tpos.PosNum);
								ASSERT( tpos.PosNum <= MAX_CHARTARGETPOSBUF_SIZE );
								moveInfo.SetCurTargetPosIdx(0);
								for(int n=0;n<tpos.PosNum;++n)
									tpos.pos[n].Decompress(moveInfo.GetTargetPosition(n));
							yCASE(CAddableInfoList::VimuInfo)
								bVimu = TRUE;
								iter.GetInfoData(&VimuPos);
							yCASE(CAddableInfoList::StreetStall)
								StallKind = eSK_SELL;
								iter.GetInfoData(StreetStallTitle);
							yCASE(CAddableInfoList::StreetBuyStall)
								StallKind = eSK_BUY;
								iter.GetInfoData(StreetStallTitle);
							yCASE(CAddableInfoList::PartyWar)
								bPartyWar = TRUE;
								iter.GetInfoData( &wPartyTeam );
							yCASE(CAddableInfoList::TitanAppearanceInfo)
								bInTitan = TRUE;
								iter.GetInfoData( &titanAppearance );

						yENDSWITCH
						
						iter.ShiftToNextData();
					}

					//SW070127 타이탄
					if( bInTitan = pmsg->bInTitan )
					{
						pPlayer->RidingTitan(bInTitan);
						pPlayer->SetTitanAppearanceInfo(&titanAppearance);
						APPEARANCEMGR->InitAppearance(pPlayer);
					}

					CPlayer* pPlayer = OBJECTMGR->AddPlayer(&pmsg->BaseObjectInfo,&moveInfo,&pmsg->TotalInfo,&pmsg->ShopItemOption, &titanAppearance, bInTitan);

					if( pmsg->TotalInfo.bPKMode )
					{
						if( pPlayer->InTitan() )
						{
							OBJECTEFFECTDESC desc(FindEffectNum("t_maintain_PK_S.beff"));
							pPlayer->AddObjectEffect( PK_EFFECT_ID, &desc, 1 );						
						}
						else
						{
							OBJECTEFFECTDESC desc(FindEffectNum("maintain_PK_S.beff"));
							pPlayer->AddObjectEffect( PK_EFFECT_ID, &desc, 1 );
						}
					}

					//SW051112 무쌍모드
					if((pPlayer->GetCharacterTotalInfo()->bVisible == FALSE)
						||(!pPlayer->GetSingleSpecialState(eSingleSpecialState_Hide)))
					{
						if( pmsg->TotalInfo.bMussangMode )
						{
#ifdef _JAPAN_LOCAL_
							OBJECTEFFECTDESC desc(FindEffectNum("maintain_mussang.beff"));
							pPlayer->AddObjectEffect( MUSSANG_EFFECT_ID, &desc, 1, pPlayer);
#else
							// 06. 03. 국내무쌍 - 이영준
							switch(pPlayer->GetStage())
							{
							case eStage_Normal:	
								{
									OBJECTEFFECTDESC desc(1035);
									pPlayer->AddObjectEffect( MUSSANG_EFFECT_ID1, &desc, 1, pPlayer);
								}
								break;
							case eStage_Hwa:		
							case eStage_Hyun:
								{
									OBJECTEFFECTDESC desc(888);
									pPlayer->AddObjectEffect( MUSSANG_EFFECT_ID2, &desc, 1, pPlayer);
								}
								break;
							case eStage_Geuk:
							case eStage_Tal:
								{
									OBJECTEFFECTDESC desc(889);
									pPlayer->AddObjectEffect( MUSSANG_EFFECT_ID3, &desc, 1, pPlayer);
								}
								break;
							}
#endif		
						}
					}
					//////////////////////////////////////////////////////////////////////////
				

					if(pmsg->bLogin)
					{
						TARGETSET set;
						set.pTarget = pPlayer;
						EFFECTMGR->StartEffectProcess(eEffect_NewCharacter,pPlayer,&set,0,pPlayer->GetID());
						pPlayer->ChangeBaseMotion( pPlayer->GetStandardMotion() );
					}
					else
					{
						if( pmsg->BaseObjectInfo.ObjectState == eObjectState_StreetStall_Owner )
						{
							if(StallKind == eSK_SELL)
								pPlayer->ShowStreetStallTitle( TRUE, StreetStallTitle );
							else if(StallKind == eSK_BUY)
								pPlayer->ShowStreetBuyStallTitle( TRUE, StreetStallTitle );
							
						}
					}

					if( bVimu )
					{
						if( pmsg->BaseObjectInfo.BattleTeam == eBattleTeam1 )
						{
							if(pPlayer->InTitan())
							{
								OBJECTEFFECTDESC desc(FindEffectNum("t_eff_bmu_A_s.beff"));
								pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );	//SW HERO는 옵션 설정 무관하게 이펙트 생성을 위한..설정
							}
							else
							{
								OBJECTEFFECTDESC desc(FindEffectNum("eff_bmu_A_s.beff"));
								pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );	//SW HERO는 옵션 설정 무관하게 이펙트 생성을 위한..설정
							}
						}
						else if( pmsg->BaseObjectInfo.BattleTeam == eBattleTeam2 )
						{
							if(pPlayer->InTitan())
							{
								OBJECTEFFECTDESC desc(FindEffectNum("t_eff_bmu_B_s.beff"));
								pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
							}
							else
							{
								OBJECTEFFECTDESC desc(FindEffectNum("eff_bmu_B_s.beff"));
								pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
							}
						}
					}

					if( bPartyWar )
					{
						if( wPartyTeam == 1 )
						{
							if(pPlayer->InTitan())
							{
								OBJECTEFFECTDESC desc(FindEffectNum("t_eff_bmu_A_s.beff"));
								pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
							}
							else
							{
								OBJECTEFFECTDESC desc(FindEffectNum("eff_bmu_A_s.beff"));
								pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
							}
						}
						else if( wPartyTeam == 2 )
						{
							if(pPlayer->InTitan())
							{
								OBJECTEFFECTDESC desc(FindEffectNum("t_eff_bmu_B_s.beff"));
								pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
							}
							else
							{
								OBJECTEFFECTDESC desc(FindEffectNum("eff_bmu_B_s.beff"));
								pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1, HERO );
							}
						}

					}

					// eventmap
					EVENTMAP->AddPlayer( pPlayer );

#endif
				}
				break;
			case MP_USERCONN_PET_ADD:
				{
					SEND_PET_TOTALINFO* pmsg = (SEND_PET_TOTALINFO*)pMsg;

					BASEMOVE_INFO moveInfo;
					moveInfo.SetFrom(&pmsg->MoveInfo);
					CAddableInfoIterator iter(&pmsg->AddableInfo);
					BYTE AddInfoKind;
					while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
					{
						ySWITCH(AddInfoKind)
							yCASE(CAddableInfoList::MoveInfo)
								COMPRESSEDTARGETPOS tpos;
								iter.GetInfoData(&tpos);
								moveInfo.bMoving = TRUE;
								moveInfo.SetMaxTargetPosIdx(tpos.PosNum);
								ASSERT( tpos.PosNum <= MAX_CHARTARGETPOSBUF_SIZE );
								moveInfo.SetCurTargetPosIdx(0);
								for(int n=0;n<tpos.PosNum;++n)
									tpos.pos[n].Decompress(moveInfo.GetTargetPosition(n));
							yCASE(CAddableInfoList::VimuInfo)
							yCASE(CAddableInfoList::StreetStall)
						yENDSWITCH

							iter.ShiftToNextData();
					}

//					CPet* pPet = OBJECTMGR->AddPet(&pmsg->BaseObjectInfo, &moveInfo, &pmsg->TotalInfo, pmsg->MasterID);
					CPet* pPet = OBJECTMGR->AddPet(&pmsg->BaseObjectInfo, &moveInfo, &pmsg->TotalInfo, pmsg->MasterName);

					pPet->SetMaster(NULL);
					//CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->TotalInfo.PetMasterID);
					CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject(pmsg->MasterID);
					if(pPlayer)
					{
						pPlayer->SetPet(pPet);
						pPet->SetMaster(pPlayer);

						if(HEROID == pmsg->MasterID)
						{
							PETMGR->SetCurSummonPet(pPet);

							//SW061211 크리스마스이벤트
							PETMGR->SetCurSummonPetKind(pPet);
							if( PETMGR->CheckCurSummonPetKindIs(ePK_EventPet) )
							{
								CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1482) );
							}
							else
							{
								PETMGR->OpenPetStateDlg();
							}

							//CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1259) );
							//PETMGR->OpenPetStateDlg();
							PETMGR->SetPetStateDlgInfo(pPet);	//펫상태창(큰) 정보 세팅.
							PETMGR->SetPetStateMiniDlgInfo(pPet);	//펫상태창(작) 정보 세팅.
							PETMGR->SetPetStateDlgUseRestInfo(pPet);	//펫 사용/휴식 세팅.
							//PETMGR->InitPetSkillGuage();
							PETMGR->SetCurPetRest(pmsg->TotalInfo.bRest);
							PETMGR->SetPetStateDlgUseRestInfo(pPet);

							PETMGR->RefleshPetMaintainBuff();
						}
					}
					else
					{
						//pPet->SetMaster(NULL);
					}

					pPet->SetMasterID(pmsg->MasterID);	//생성된 펫은 같은 그리드내에 주인 유무와 상관 없이 주인ID 정보 가지고 있는다. 주인 추가시 쓰임.

					//펫 생성시 주인과 그리드를 비교해서 그리드가 다르면 같은 그리드로 강제 세팅하는 것도 방법일 것이다!
					/*
					if(pmsg->bLogin)
					{
						TARGETSET set;
						set.pTarget = pPet;
						EFFECTMGR->StartEffectProcess(eEffect_PetSummon,pPet,&set,0,pPet->GetID());
					}*/
				}
				break;

			case MP_USERCONN_MONSTER_ADD:
				{
					SEND_MONSTER_TOTALINFO* pmsg = (SEND_MONSTER_TOTALINFO*)pMsg;
					if(pmsg->TotalInfo.MonsterKind > 10000)
					{
						ASSERT(0);
					}
//					if(pmsg->TotalInfo.MonsterKind == 0)
//					{
//						int a=0;
//					}

					BASEMOVE_INFO moveInfo;
					moveInfo.SetFrom(&pmsg->MoveInfo);
					CAddableInfoIterator iter(&pmsg->AddableInfo);
					BYTE AddInfoKind;
					while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
					{
						ySWITCH(AddInfoKind)
							yCASE(CAddableInfoList::MoveInfo)
								COMPRESSEDTARGETPOS tpos;
								iter.GetInfoData(&tpos);
								moveInfo.bMoving = TRUE;
								moveInfo.SetMaxTargetPosIdx(tpos.PosNum);
								ASSERT( tpos.PosNum <= MAX_CHARTARGETPOSBUF_SIZE );
								moveInfo.SetCurTargetPosIdx(0);
								for(int n=0;n<tpos.PosNum;++n)
									tpos.pos[n].Decompress(moveInfo.GetTargetPosition(n));
							yCASE(CAddableInfoList::VimuInfo)
							yCASE(CAddableInfoList::StreetStall)
						yENDSWITCH
						
						iter.ShiftToNextData();
					}
					
#ifdef TAIWAN_LOCAL
					//임시로! 몬스터이름을 서버에서 안보내주도록 하자!
					SafeStrCpy( pmsg->BaseObjectInfo.ObjectName,
						GAMERESRCMNGR->GetMonsterListInfo( pmsg->TotalInfo.MonsterKind )->Name,
						MAX_NAME_LENGTH+1 );
#endif
					
					CMonster* pMon = OBJECTMGR->AddMonster(&pmsg->BaseObjectInfo,&moveInfo,&pmsg->TotalInfo);
										
					//////////////////////////////////////////////////////////////////////////

					if(pmsg->bLogin)
					{
						TARGETSET set;
						set.pTarget = pMon;
						EFFECTMGR->StartEffectProcess(eEffect_MonRegen,pMon,&set,0,pMon->GetID());
					}
				}
				break;	
			case MP_USERCONN_BOSSMONSTER_ADD:
				{
					SEND_MONSTER_TOTALINFO* pmsg = (SEND_MONSTER_TOTALINFO*)pMsg;
					if(pmsg->TotalInfo.MonsterKind > 10000)
					{
						ASSERT(0);
					}
					
					BASEMOVE_INFO moveInfo;
					moveInfo.SetFrom(&pmsg->MoveInfo);
					CAddableInfoIterator iter(&pmsg->AddableInfo);
					BYTE AddInfoKind;
					while((AddInfoKind = iter.GetInfoKind()) != CAddableInfoList::None)
					{
						ySWITCH(AddInfoKind)
							yCASE(CAddableInfoList::MoveInfo)
								COMPRESSEDTARGETPOS tpos;
								iter.GetInfoData(&tpos);
								moveInfo.bMoving = TRUE;
								moveInfo.SetMaxTargetPosIdx(tpos.PosNum);
								ASSERT( tpos.PosNum <= MAX_CHARTARGETPOSBUF_SIZE );
								moveInfo.SetCurTargetPosIdx(0);
								for(int n=0;n<tpos.PosNum;++n)
									tpos.pos[n].Decompress(moveInfo.GetTargetPosition(n));
							yCASE(CAddableInfoList::VimuInfo)
							yCASE(CAddableInfoList::StreetStall)
						yENDSWITCH
						
						iter.ShiftToNextData();
					}
					
#ifdef TAIWAN_LOCAL
					//임시로! 몬스터이름을 서버에서 안보내주도록 하자!
					SafeStrCpy( pmsg->BaseObjectInfo.ObjectName,
						GAMERESRCMNGR->GetMonsterListInfo( pmsg->TotalInfo.MonsterKind )->Name,
						MAX_NAME_LENGTH+1 );
#endif
					
					CBossMonster* pMon = OBJECTMGR->AddBossMonster(&pmsg->BaseObjectInfo,&moveInfo,&pmsg->TotalInfo);
										
					//////////////////////////////////////////////////////////////////////////
	
					if(pmsg->bLogin)
					{
						TARGETSET set;
						set.pTarget = pMon;
						EFFECTMGR->StartEffectProcess(eEffect_MonRegen,pMon,&set,0,pMon->GetID());
					}
//					GetMiniMapDialog()->AddMonsterIcon(pMon);

					//GetDebugDlg()->DebugMsgParser(DBG_CHAT, "MONSTER_ADD %d", pmsg->BaseObjectInfo.dwObjectID);
				}
				break;	

			case MP_USERCONN_CHANGEMAP_ACK:
				{
					MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
					DWORD map = pmsg->dwData;

			
					if( GAMEIN->GetMoveDialog()->IsMovePos() )
					{
						int MotionIdx = -1;
						int EffectIdx = -1;
						WORD Weapontype = HERO->GetWeaponEquipType();
						if( Weapontype==WP_GUM || Weapontype==WP_GWUN || Weapontype==WP_GUNG || Weapontype==WP_AMGI ||
							Weapontype==WP_EVENT || Weapontype==WP_EVENT_HAMMER )
						{
							MotionIdx = eMotion_Item_Teleport_1;
							EffectIdx = eEffect_ShopItem_Teleport1;
						}
						else if( Weapontype==WP_DO )
						{
							MotionIdx = eMotion_Item_Teleport_2;
							EffectIdx = eEffect_ShopItem_Teleport2;
						}
						else if( Weapontype==WP_CHANG )
						{
							MotionIdx = eMotion_Item_Teleport_3;
							EffectIdx = eEffect_ShopItem_Teleport3;
						}

						MOVEMGR->HeroMoveStop();
						HERO->SetItemUseMotionIdx( MotionIdx );
						HERO->SetItemUseEffectIdx( EffectIdx );
						HERO->GetEngineObject()->ChangeMotion(MotionIdx, FALSE);
						m_MoveMap = map;
						OBJECTSTATEMGR->StartObjectState(HERO, eObjectState_ItemUse);
						OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_ItemUse, HERO->GetEngineObject()->GetAnimationTime(MotionIdx)) ;
						// m_MoveMap = map;
						m_MovePoint = GetMoveDialog()->GetSavedPoint();
					}
					else if( GUILDMGR->GetSOSTargetPos() != 0 )	// magi82 - SOS(070724)
					{
						m_MoveMap = map;
						m_MovePoint = GUILDMGR->GetSOSTargetPos();
						GUILDMGR->SetSOSTargetPos(0);
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg( 160 ) );
						MAPCHANGE->SetGameInInitKind(eGameInInitKind_MovePos);
						MAINGAME->SetGameState(eGAMESTATE_MAPCHANGE,&map,4);				
					}
					else
					{
						WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg( 160 ) );
						MAPCHANGE->SetGameInInitKind(eGameInInitKind_MapChange);
						MAINGAME->SetGameState(eGAMESTATE_MAPCHANGE,&map,4);
					}
					//
					CHATMGR->SaveChatList();
					ITEMMGR->DeleteShopItemInfo();
				}
				break;

			case MP_USERCONN_CHEAT_CHANGEMAP_ACK:
				{
					MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
					DWORD map = pmsg->dwData;


					WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg( 700 ) );
					
					MAPCHANGE->SetGameInInitKind(eGameInInitKind_MapChange);
					MAINGAME->SetGameState(eGAMESTATE_MAPCHANGE,&map,4);
					CHATMGR->SaveChatList();
				}
				break;			
				
			case MP_USERCONN_CHANGEMAP_NACK:
				{

					if( HERO->GetState() == eObjectState_Deal )
						OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(292) );
					g_UserInput.SetInputFocus(TRUE);

					// magi82 - SOS(070725)
					if(GUILDMGR->GetSOSTargetPos())
					{
						GUILDMGR->SetSOSTargetPos(0);
					}

					// magi82 - SOS(070807)
					if(GUILDMGR->GetSOSChannelIdx())
					{
						GUILDMGR->SetSOSChannelIdx(-1);
					}

					if( GetMoveDialog()->GetSavedPoint() )
					{
						ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
						ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
						ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
						ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);						
					}
					
					GAMEIN->GetMoveDialog()->SetItemToMapServer( 0, 0, FALSE );
				}
				break;

			case MP_USERCONN_CHARACTERLIST_ACK:
				{		
					SEND_CHARSELECT_INFO* pmsg = (SEND_CHARSELECT_INFO*)pMsg;
//					ReleaseForGame();
					MAINGAME->SetGameState(eGAMESTATE_CHARSELECT, (void *)pmsg, sizeof(SEND_CHARSELECT_INFO));
					ITEMMGR->DeleteShopItemInfo();
				}
				return;

			case MP_USERCONN_BACKTOCHARSEL_ACK:
				{
//					ReleaseForGame();
					MAINGAME->SetGameState( eGAMESTATE_CHARSELECT );
				}
				break;

			case MP_USERCONN_BACKTOCHARSEL_NACK:
				{

					WINDOWMGR->AddListDestroyWindow( WINDOWMGR->GetWindowForID( MBI_NOBTNMSGBOX ) );
					OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Enter );//AO½A
				}
				break;
			case MP_USERCONN_MAPDESC:
				{
					MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
					MAP->SetVillage( pmsg->wData1 );
					PKMGR->SetPKAllow( pmsg->wData2 );
//!!					
//					//SW050907 마을/필드 초기 평화상태 설정
//					HERO->SetPeaceMode( pmsg->wData1 );
//					PEACEWARMGR->Peace_WarMode(HERO, pmsg->wData1);
					
					// magi82(37) 맵 속성 데이터 GetMapName( MAP->GetMapNum() ) 수정
#ifdef TAIWAN_LOCAL					
					if( MAP->GetMapNum() == EVENTMAPNUM || MAP->GetMapNum() == Tournament || 
						MAP->GetMapNum() == QuestRoom )
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 998 ), MAP->GetMapName() );
					else
					{
						if( MAP->IsVillage() )
							CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 440 ), gChannelNum+1, MAP->GetMapName() );
						else
							CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 441 ), gChannelNum+1, MAP->GetMapName() );
					}
#elif defined _HK_LOCAL_
					if( MAP->GetMapNum() == EVENTMAPNUM || MAP->GetMapNum() == Tournament || 
						MAP->GetMapNum() == QuestRoom )
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 998 ), MAP->GetMapName() );
					else
					{
						if( MAP->IsVillage() )
							CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 440 ), gChannelNum+1, MAP->GetMapName() );
						else
							CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 441 ), gChannelNum+1, MAP->GetMapName() );
					}
#else
					//if( MAP->GetMapNum() == EVENTMAPNUM || MAP->GetMapNum() == Tournament || 
					//	MAP->GetMapNum() == QuestRoom || MAP->CheckMapKindIs(eSurvival) )
					DWORD dwCheckBit = eEventMap | eTournament | eQuestRoom | eSurvivalMap;
					if( MAP->IsMapKind(dwCheckBit) )
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 998 ), MAP->GetMapName() );
					else
					{
						if( MAP->IsVillage() )
							CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 440 ), MAP->GetMapName(), gChannelNum+1 );
						else
							CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 441 ), MAP->GetMapName(), gChannelNum+1 );
					}

#endif

					if( PKMGR->IsPKAllow() )
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 558 ) );
						//PK허용 금지 미니맵에 표시해주기
					}

					// 06. 03. 문파공지 - 이영준
					if(HERO->GetGuildIdx() && GUILDMGR->GetGuildNotice())
						CHATMGR->AddMsg(CTC_GUILD_NOTICE, CHATMGR->GetChatMsg(1280), GUILDMGR->GetGuildNotice());
				}
				break;
			case MP_USERCONN_SETVISIBLE:
				{
					MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
					CObject* pObj = OBJECTMGR->GetObject(pmsg->dwData1);
					if( !pObj ) // AIºIºÐ¿¡¼­ NULL°ª AuA¶(E®AI¿a!)
					{
						ASSERT(0);
						return;
					}
					
					if(pObj->GetObjectKind() != eObjectKind_Player)
					{
						ASSERT(0);
						return;
					}

					CPlayer* ppla = (CPlayer*)pObj;
					if(pmsg->dwData2)
					{
#ifdef _GMTOOL_
						if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM )
							ppla->GetEngineObject()->SetAlpha( 1.0f );
						else
#endif						
							ppla->GetEngineObject()->Show();
						
						ppla->GetCharacterTotalInfo()->bVisible = true;
					}
					else
					{
#ifdef _GMTOOL_
						if( MAINGAME->GetUserLevel() <= eUSERLEVEL_GM )
							ppla->GetEngineObject()->SetAlpha( 0.3f );
						else
#endif						
							ppla->GetEngineObject()->HideWithScheduling();

						ppla->GetCharacterTotalInfo()->bVisible = false;
					}
				}
				break;
				
// eventmap ---------------------------------------------------------------------------------------
			case MP_USERCONN_MAP_OUT_TO_EVENTMAP:
				{
					MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
					DWORD map = pmsg->dwData1;

					EVENTMAP->SetData( pmsg->dwData1, pmsg->dwData2, pmsg->dwData3 );
					//if( MAP->GetMapNum() != 58 )
					if( FALSE == MAP->IsMapKind(eEventMap) )
						EVENTMAP->m_dwBeforeMapNum = MAP->GetMapNum();

					WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg( 700 ) );
					
					MAPCHANGE->SetGameInInitKind(eGameInInitKind_EventMapEnter);
					MAINGAME->SetGameState(eGAMESTATE_MAPCHANGE,&map,4);
					CHATMGR->SaveChatList();
				}
				break;
			case MP_USERCONN_MAP_OUT_TO_EVENTBEFOREMAP:
				{
					WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg( 700 ) );

					MSG_WORD2 msg;
					msg.Category = MP_USERCONN;
					msg.Protocol = MP_USERCONN_MAP_OUT_WITHMAPNUM;
					msg.dwObjectID = HEROID;
					msg.wData1 = (WORD)EVENTMAP->m_dwBeforeMapNum;
					msg.wData2 = -1;

					NETWORK->Send(&msg, sizeof(msg));	
				}
				break;			
			case MP_USERCONN_EVENT_READY:
				{
					EVENTMAP->Ready();
				}
				break;
			case MP_USERCONN_EVENT_START:
				{
					EVENTMAP->Start();
				}
				break;
			case MP_USERCONN_EVENT_END:
				{
					EVENTMAP->End( (MSG_DWORD3*)pMsg );
				}
				break;					
//----------------------------------------------------------------------------------------------
			case MP_USERCONN_EVENTITEM_USE:
				{
					MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
					if( pmsg->dwData != 0 )
                        CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1178) );
					else
						CHATMGR->AddMsg( CTC_SYSMSG, "행운상자 쿠폰 50장을 모두 등록하였습니다." );	// 이벤트 관련 임시로 넣음...
				}
				break;
			case MP_USERCONN_EVENTITEM_USE2:
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1307) );
				}
				break;
			case MP_USERCONN_CASTLEGATE_ADD:
				{
					//SEND_GATE_TOTALINFO* pmsg = (SEND_GATE_TOTALINFO*)pMsg;

					//SIEGEMGR->CreateCastleGate( pmsg );
				}
				break;
			case MP_USERCONN_FLAGNPC_ONOFF:
				{//SW061220 상태깃발NPC관련 추가. //깃발 표시 안하기 위한 상태.
					MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

					int flg = 0;
					if( pmsg->dwData1 )
					{
						flg |= eWeekforGT;
					}
					if( pmsg->dwData2 && pmsg->dwData2 != eSWState_EndSiegeWar )
					{
						flg |= eWeekforSG;
					}

					OBJECTMGR->SetWeekData(flg);

					//SW061205 깃발 NPC
					SYSTEMTIME st;
					GetLocalTime(&st);
					WORD day = st.wDayOfWeek;
					DWORD GTFlg, SGFlg;

					GTFlg = GAMERESRCMNGR->GetFlagFromDate(eGTFlg, day);
					OBJECTMGR->ChangeFlagNPC(eGTFlg, GTFlg);

					SGFlg = GAMERESRCMNGR->GetFlagFromDate(eSGFlg, day);
					OBJECTMGR->ChangeFlagNPC(eSGFlg, SGFlg);
				}
				break;
			case MP_USERCONN_CHANGEMAP_CHANNELINFO_ACK:	//2008. 5. 8. CBH - 이동할 맵 채널정보를 받는 프로토콜 추가
				{
					MSG_CHANNEL_INFO * pmsg = (MSG_CHANNEL_INFO*)pMsg;

					CChannelDialog* pChannelDlg = (CChannelDialog*)WINDOWMGR->GetWindowForID( CHA_CHANNELDLG );
					if(pChannelDlg != NULL)
					{
						pChannelDlg->SetChannelList(pmsg);

						// 체널이 하나이면 바로 들어가게 함
						if(pmsg->Count == 1)
						{							
							pChannelDlg->MapChange();
							pChannelDlg->SetActive(FALSE);
						}
					}					
				}
				break;

			case MP_USERCONN_CHANGEMAP_CHANNELINFO_NACK:	//2008. 5. 8. CBH - 이동할 맵 채널정보를 받는 프로토콜 추가
				{
					MSG_BYTE* msg = (MSG_BYTE*)pMsg;
					if( msg->bData == 0 )
					{
						CChannelDialog* pChannelDlg = (CChannelDialog*)WINDOWMGR->GetWindowForID( CHA_CHANNELDLG );
						if(pChannelDlg != NULL)
						{
							CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1700) );
							pChannelDlg->SetActive(FALSE);
						}

						if( HERO->GetState() == eObjectState_Deal )
							OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
					}
				}
				break;	
			case MP_USERCONN_CURRENTMAP_CHANNELINFO:
				{
					MSG_CHANNEL_INFO* pmsg = (MSG_CHANNEL_INFO*)pMsg;

					memset(IsBattleChannel, 0, sizeof(bool)*MAX_CHANNEL_NUM);
                    for( int i = 0; i < pmsg->Count; ++i )
					{
						IsBattleChannel[i]= pmsg->bBattleChannel[i];
					}
				}
				break;
			}
		}
		break;
/*
	case MP_CHAR_REVIVE:
		{
			switch(Protocol)
			{
			case MP_CHAR_REVIVE_LOGINSPOT_ACK:
				{
					MOVE_POS* pmsg = (MOVE_POS*)pMsg;
					VECTOR3 pos;
					CObject* pObject = OBJECTMGR->GetObject(pmsg->dwMoverID);
					pmsg->cpos.Decompress(&pos);
					OBJECTACTIONMGR->Revive(pObject,&pos);
					GAMEIN->GetReviveDialog()->SetDisable(FALSE);
				}
				break;

			case MP_CHAR_REVIVE_PRESENTSPOT_ACK: 
				{
					MOVE_POS* pmsg = (MOVE_POS*)pMsg;
					VECTOR3 pos;
					CObject* pObject = OBJECTMGR->GetObject(pmsg->dwMoverID);
					pmsg->cpos.Decompress(&pos);
					OBJECTACTIONMGR->Revive(pObject,&pos);
					GAMEIN->GetReviveDialog()->SetDisable(FALSE);					

				}
				break;
			}
		}
		break;
*/
    case MP_PARTY:			PARTYMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_QUICK:			QUICKMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_ITEM:			ITEMMGR->NetworkMsgParse(Protocol,pMsg);				break;
	case MP_ITEMEXT:		ITEMMGR->NetworkMsgParseExt(Protocol,pMsg);				break;
	case MP_MUGONG:			MUGONGMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_CHAT:			CHATMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_BATTLE:			BATTLESYSTEM->NetworkMsgParse(Protocol, pMsg);			break;
	case MP_PEACEWARMODE:	PEACEWARMGR->NetworkMsgParse(Protocol, pMsg);			break;
	case MP_UNGIJOSIK:		UNGIMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_TACTIC:			TACTICMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_KYUNGGONG:		KYUNGGONGMGR->NetworkMsgParse(Protocol, pMsg);			break;
	case MP_EXCHANGE:		EXCHANGEMGR->NetworkMsgParse(Protocol, pMsg);			break;
	case MP_STREETSTALL:	STREETSTALLMGR->NetworkMsgParse(Protocol, pMsg);		break;
	case MP_PYOGUK:			PYOGUKMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_FRIEND:			FRIENDMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_NOTE:			NOTEMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_BOSSMONSTER:	OBJECTMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_QUEST:			
		{

			QUESTMGR1->NetworkMsgParse(Protocol, pMsg);				
			QUESTMGR->NetworkMsgParse( Protocol, pMsg );
		}
		break;
	case MP_PK:				PKMGR->NetworkMsgParse(Protocol, pMsg);					break;
	case MP_WANTED:			WANTEDMGR->NetworkMsgParse(Protocol, pMsg);				break;	
	case MP_CHEAT:			CHEATMGR->NetworkMsgParse(Category,Protocol,(MSGBASE*)pMsg);	break;	
	case MP_JOURNAL:		JOURNALMGR->NetworkMsgParse(Protocol, pMsg);				break;	
	case MP_GUILD:			GUILDMGR->NetworkMsgParse(Protocol, pMsg);				break;
	case MP_GUILD_UNION:	GUILDUNION->NetworkMsgParse(Protocol, pMsg);			break;
	case MP_NPC:
		{
			switch(Protocol)
			{
			case MP_NPC_SPEECH_ACK:
				{
					MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

					CNpc* pNpc = (CNpc*)OBJECTMGR->GetObject(pmsg->dwData1);

					if( !pNpc )
					{
						ASSERT(0);
						if( HERO->GetState() == eObjectState_Deal )
							OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
						return;
					}

					switch( pNpc->GetNpcKind() )
					{
					case 31:
					case 81:
					case 82:
						{
							if( HERO->IsPKMode() )
							{
								CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(144) );

								if( HERO->GetState() == eObjectState_Deal )
									OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
								break;
							}

							char buf[64] = {0,};

#ifndef TAIWAN_LOCAL
							int nLen = strlen( pNpc->GetObjectName() ) - 6;
							if( nLen > 0 && strcmp( pNpc->GetObjectName() + nLen, "가는길" ) == 0 )
							{
								strncpy( buf, pNpc->GetObjectName(), nLen );
								buf[nLen] = 0;
							}
							else
							{
								strcpy( buf, pNpc->GetObjectName() );
							}
#else
							int nLen = strlen( pNpc->GetObjectName() ) - 4; //"÷×E¶"A|¿U
							if( nLen > 0 && strncmp( pNpc->GetObjectName(), "繫鞏", 4 ) == 0 )
							{
								strncpy( buf, pNpc->GetObjectName()+4, nLen );
								buf[nLen] = 0;
							}
							else
							{
								strcpy( buf, pNpc->GetObjectName() );
							}
#endif
							

							//060607 - fixed by wonju----------------------------------------------------------------------------
							//맵이동시 캐릭터의 레벨이 최저 레벨 몬스터의 레벨보다 15이상 차이날 경우

							//현재 캐릭터의 레벨을 알아낸다.
							unsigned int CharLevel = HERO->GetLevel();

							//npc의 unique정보를 이용하여 이동할 맵의 번호를 알아낸다.

							WORD uniquenum = pNpc->GetNpcUniqueIdx();
							MAPCHANGE_INFO* pMCInfo = NULL;
							
							pMCInfo = GAMERESRCMNGR->GetMapChangeInfo(uniquenum);

							if(pMCInfo == NULL) ASSERTMSG(0,"올바른 정보를 얻을 수 없습니다.");

							int destmapnumber = pMCInfo->MoveMapNum;

							cMsgBox* pBox = NULL;
#ifdef _KOR_LOCAL_
							CHARSELECT->GetChannelDialog()->SendMapChannelInfoSYN(uniquenum);
#else
							unsigned int MinMonsterLevel = 0;

							//맵에서 가장 레벨이 작은 몬스터의 레벨을 찾는다.
							MinMonsterLevel = GAMERESRCMNGR->GetMinMonsterLvInMap(destmapnumber);

							//if(MinMonsterLevel == 0) //정상적인 스크립트 기록 오류							

							//최소 몬스터 레벨이 15보다 작다면 14가 된다.
							if(MinMonsterLevel < 15) MinMonsterLevel = 14;

							if(CharLevel <= (MinMonsterLevel - 14) )
							{
								pBox = WINDOWMGR->MsgBox( MBI_MAPCHANGE, MBT_YESNO, CHATMGR->GetChatMsg(1308));
							}
							else
							{
								pBox = WINDOWMGR->MsgBox( MBI_MAPCHANGE, MBT_YESNO, CHATMGR->GetChatMsg(360), buf );
							}
							//-----------------------------------------------------------------------------------------------------
#endif
							
							if( pBox )
								pBox->SetParam( pNpc->GetID() );
						}
						break;

					case 30:
					case 83:
						{
							cMsgBox* pBox = WINDOWMGR->MsgBox( MBI_SELECTLOGINPOINT, MBT_YESNO, CHATMGR->GetChatMsg(361), pNpc->GetObjectName() );
							if( pBox )
								pBox->SetParam( pNpc->GetID() );

//							GAMEEVENTMGR->AddEvent(eGameEvent1_SavePoint);
							GAMEEVENTMGR->AddEvent(eGameEvent_LoginPoint);
						}
						break;
					case 23:  // bomul
						{	
							// 06.12.21 - RaMa : 클릭하면 안되는 보물상자.
							VECTOR3 pos;
							pNpc->GetPosition( &pos );
							if( pos.x == 0 && pos.z == 0 )
							{
								OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
								return;
							}

							NPCSCRIPTMGR->StartNpcScript( pNpc );
							GAMEEVENTMGR->AddEvent(eGameEvent_NpcClick);	
						}
						break; 
					case 16:		// 공성맵 각인 상징물
						{
							CGuild* pGuild = NULL;
							// 각인시작		
							if( HERO->GetGuildMemberRank() != GUILD_MASTER )
							{
								CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1010) );								
								goto EngraveFailed;
							}
							pGuild = GUILDMGR->GetGuild();
							if( pGuild && pGuild->GetLevel() != 5 )
							{
								CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1011) );
								goto EngraveFailed;
							}
							if( HERO->GetBattleTeam() != eBattleTeam2 )
							{
								CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1057) );
								goto EngraveFailed;
							}
							
							WINDOWMGR->MsgBox( MBI_SW_ENGRAVE_SYN, MBT_YESNO, CHATMGR->GetChatMsg(1046) );
							break;
							
EngraveFailed:
							if( HERO->GetState() == eObjectState_Deal )
									OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
						}
						break;
					case 17:	// 요새전 각인 상징물
						{
							WINDOWMGR->MsgBox( MBI_FT_ENGRAVE_START_SYN, MBT_YESNO, CHATMGR->GetChatMsg(1046) );
						}
						break;

					default:
						{
							NPCSCRIPTMGR->StartNpcScript( pNpc );
							GAMEEVENTMGR->AddEvent(eGameEvent_NpcClick);
						}
						break;
					}
				}
				break;
			case MP_NPC_SPEECH_NACK:
				{

					if( HERO->GetState() == eObjectState_Deal )
						OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
					MSG_DWORD *pmsg = (MSG_DWORD*)pMsg;
					if(pmsg->dwData == 271) // bomul
					{
						NPCSCRIPTMGR->SetSelectedNpc(NULL);
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(883) );
					}
				}
				break;
			case MP_NPC_CLOSEBOMUL_ACK:
				{
					if(GetNpcScriptDialog()->IsActive())
					{
						NPCSCRIPTMGR->SetSelectedNpc(NULL);
						GetNpcScriptDialog()->SetActive(FALSE);						
					}
				}
				break;
			case MP_NPC_DIE_ACK:
				{
					MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
					CNpc* pNpc = NPCSCRIPTMGR->GetSelectedNpc();
					if(pNpc)
					{
						if(pNpc->GetID() == pmsg->dwData)
						{
							if(GetNpcScriptDialog()->IsActive())
							{
								NPCSCRIPTMGR->SetSelectedNpc(NULL);
								GetNpcScriptDialog()->SetActive(FALSE);						
							}
						}
					}
				}
				break;
				
			case MP_NPC_DOJOB_NACK:
				{
					NPCSCRIPTMGR->SetSelectedNpc(NULL);
					GetNpcScriptDialog()->SetActive(FALSE);						
					if( HERO->GetState() == eObjectState_Deal )
						OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
					
				}
				break;
			}
		}
		break;
	case MP_SIGNAL:
		{
			switch(Protocol)
			{
			case MP_SIGNAL_COMMONUSER:
				{
					TESTMSG *data = (TESTMSG *)pMsg;
					
					CHATMGR->AddMsg( CTC_OPERATOR, data->Msg );

					//CDebugDlg* pDbg = GAMEIN->GetDebugDlg();
					//pDbg->DebugMsgParser(DBG_CHAT, chat);
				}
				break;
			case MP_SIGNAL_ONEUSER:
				break;
			case MP_SIGNAL_SYSTEM:
				break;
			case MP_SIGNAL_BATTLE:	//AO½A
//				{
//					TESTMSG *data = (TESTMSG *)pMsg;
//					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(24), data->Msg );
//				}
				break;
			case MP_SIGNAL_VIMU_RESULT:	//AO½A
				{
					MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
					CObject* pWinner = OBJECTMGR->GetObject(pmsg->dwData1);
					CObject* pLoser = OBJECTMGR->GetObject(pmsg->dwData2);
					if( pWinner == NULL || pLoser == NULL ) return;
					
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(655),
					pWinner->GetObjectName(), pLoser->GetObjectName(), pWinner->GetObjectName() );

				}
				break;				
			}
		}
		break;
	case MP_HACKCHECK:	HACKCHECK->NetworkMsgParse( Protocol, pMsg );	break;
	case MP_SOCIETYACT:	SOCIETYACTMGR->NetworkMsgParse( Protocol, pMsg ); break;

	case MP_MURIMNET:
		{
			switch( Protocol )
			{
			case MP_MURIMNET_CHANGETOMURIMNET_ACK:
				{
					MSGBASE* pmsg = (MSGBASE*)pMsg;
					
					MAPTYPE dwMapNum = MAP->GetMapNum();
					MAINGAME->SetGameState( eGAMESTATE_MURIMNET, &dwMapNum, 4 );
				}
				break;

			case MP_MURIMNET_CHANGETOMURIMNET_NACK:
				{
					MessageBox(NULL, "Fail To Connect To MurimServer.",0,0);
				}
				break;

			case MP_MURIMNET_RETURNTOMURIMNET_ACK:
				{
					MSGBASE* pmsg = (MSGBASE*)pMsg;
					
					MAINGAME->SetGameState( eGAMESTATE_MURIMNET );
				}
				break;

			case MP_MURIMNET_RETURNTOMURIMNET_NACK:
				{
					MessageBox(NULL, "Fail To Come Back To MurimServer. Connect Again Plz",0,0);
					
//					MAINGAME->SetGameState( eGAMESTATE_END );
					PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
				}
				break;

			case MP_MURIMNET_MURIMSERVERIN_ACK:
				{
					SEND_HERO_TOTALINFO * pmsg = (SEND_HERO_TOTALINFO *)pMsg;
					OBJECTMGR->RegisterHero(pmsg);
					
					GAMEIN->GetMiniMapDialog()->AddHeroIcon(HERO);
										
					CAMERA->Init((CObject*)OBJECTMGR->GetHero(),30,0,1000);

					CAMERA->SetTargetAngleX(0,29.9f,0);					
				}
				break;

			case MP_MURIMNET_MURIMSERVERIN_NACK:
				{
				}
				break;
			}
		}
	case MP_GUILD_FIELDWAR:		GUILDFIELDWAR->NetworkMsgParse( Protocol, pMsg );		break;
	case MP_PARTYWAR:			PARTYWAR->NetworkMsgParse( Protocol, pMsg );			break;
	case MP_GTOURNAMENT:		GTMGR->NetworkMsgParse( Protocol, pMsg);					break;

	case MP_JACKPOT:			
		{
			if(GAMEIN->GetMiniMapDialog())
			JACKPOTMGR->NetworkMsgParse( Protocol, pMsg );
		}
		break;
	//SW061019 서바이벌모드
	case MP_SURVIVAL:
		{
			if(GAMEIN->GetSurvivalCountDlg())
				SVVMODEMGR->NetworkMsgParse( Protocol, pMsg );
		}
		break;
	case MP_BOBUSANG:
		{
			BOBUSANGMGR->NetworkMsgParse( Protocol, pMsg );
		}
		break;
	case MP_OPTION:
		{
			OPTIONMGR->NetworkMsgParse( Protocol, pMsg );
		}
		break;
#ifndef _JAPAN_LOCAL_ //BLOCK FOR JAPAN

	case MP_SIEGEWAR:			SIEGEMGR->NetworkMsgParse( Protocol, pMsg );	break;	
	case MP_SIEGEWAR_PROFIT:	SWPROFIT->NetworkMsgParse( Protocol, pMsg );	break;
	case MP_PET:				PETMGR->NetworkMsgParse( Protocol, pMsg );		break;
	case MP_TITAN:				TITANMGR->NetworkMsgParse( Protocol, pMsg );	break;

#endif
	case MP_ITEMLIMIT:
		{
			switch( Protocol )
			{
			case MP_ITEMLIMIT_FULL_TO_CLIENT:
				{
					MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
					ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( pmsg->dwData );
					if( pInfo )
                        CHATMGR->AddMsg( CTC_ALERT_YELLOW, "%s 아이템 제한으로 더이상 얻을 수 없습니다.", pInfo->ItemName );
					else
						CHATMGR->AddMsg( CTC_ALERT_YELLOW, "아이템[%d] 제한으로 더이상 얻을 수 없습니다.", pmsg->dwData );
				}
				break;
			}
		}
		break;
	case MP_AUTONOTE:	AUTONOTEMGR->NetworkMsgParse( Protocol, pMsg );			break;
	case MP_FORTWAR:	FORTWARMGR->NetworkMsgParse( Protocol, pMsg );			break;
	}
}

void CGameIn::OnDisconnect()
{
	m_bDisconnected = TRUE;
	if( cMsgBox::IsInit() )	//°OAOA¤AoCE¿a? confirm
	{
		if( HACKCHECK->IsHackUser() )
		{
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_SERVERDISCONNECT, MBT_OK, CHATMGR->GetChatMsg( 560 ) );
		}
		else
		{
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_SERVERDISCONNECT, MBT_OK, CHATMGR->GetChatMsg( 448 ) );
		}
	}
	else
	{
		MessageBox( NULL, "Disconnected To Server.\nThe Game is Closing.", 0, 0 );
		//MAINGAME->SetGameState(eGAMESTATE_END);
		PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
	}
}

void CGameIn::ProcessInBossMap()
{
	//if(	MAP->GetMapNum() != BOSSMONSTER_MAP && MAP->GetMapNum() != BOSSMONSTER_2NDMAP )	return;
	if( FALSE == MAP->IsMapKind(eBossMap) )
		return;

	if( !m_bDieInBossMap )			return;
	
	if( gCurTime > m_dwTimeInBossMap )
	{
		m_dwTimeInBossMap = gCurTime + 1000;
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(930), m_nCountInBossMap );
		--m_nCountInBossMap;
	}
	if( m_nCountInBossMap <= 0 )
		m_bDieInBossMap = FALSE;
}


//캐릭터 서버 로그인 시간
void	CGameIn::SetLoginTime(SYSTEMTIME Time)
{
	GameinServerTime = Time;
}

SYSTEMTIME	CGameIn::GetLoginTime()
{
	return GameinServerTime;
}

//캐릭터 클라이언트 로그인 시간
void	CGameIn::GetClientLoginTime(DWORD& date, DWORD& time)
{
	date = m_Clientdate;
	time = m_Clienttime;
}

void	CGameIn::SetClientLoginTime(DWORD date, DWORD time)
{
	m_Clientdate = date;
	m_Clienttime = time;
}


#ifdef _TESTCLIENT_

void InitTestHero()
{
	DWORD id = 1;
	{	// AOAI°ø
		SEND_HERO_TOTALINFO info;
		memset(&info,0,sizeof(info));
		info.BaseObjectInfo.dwObjectID = id++;
		strcpy( info.BaseObjectInfo.ObjectName, "DARK");
		VECTOR3 pos;
		pos.x = GAMERESRCMNGR->m_TestClientInfo.x;
		pos.z = GAMERESRCMNGR->m_TestClientInfo.z;
		info.SendMoveInfo.CurPos.Compress(&pos);
		info.ChrTotalInfo.Gender = GAMERESRCMNGR->m_TestClientInfo.Gender;
		info.ChrTotalInfo.Life = 1000;
		info.ChrTotalInfo.HairType = GAMERESRCMNGR->m_TestClientInfo.HairType;
		info.ChrTotalInfo.FaceType = GAMERESRCMNGR->m_TestClientInfo.FaceType;
		info.ChrTotalInfo.MaxLife = 1000;
		info.HeroTotalInfo.wNaeRyuk = 1000;
		info.HeroTotalInfo.wMaxNaeRyuk = 1000;
		info.ChrTotalInfo.Level = 99;
		info.ChrTotalInfo.Height = 1;
		info.ChrTotalInfo.Width = 1;
		info.ChrTotalInfo.WearedItemIdx[eWearedItem_Weapon] = GAMERESRCMNGR->m_TestClientInfo.WeaponIdx;
		info.ChrTotalInfo.WearedItemIdx[eWearedItem_Dress] = GAMERESRCMNGR->m_TestClientInfo.DressIdx;
		info.ChrTotalInfo.WearedItemIdx[eWearedItem_Hat] = GAMERESRCMNGR->m_TestClientInfo.HatIdx;
		info.ChrTotalInfo.WearedItemIdx[eWearedItem_Shoes] = GAMERESRCMNGR->m_TestClientInfo.ShoesIdx;
		info.HeroTotalInfo.KyungGongGrade = 2604;
		info.ChrTotalInfo.bVisible = true;
		
		info.MugongTotalInfo.mugong[0].dwDBIdx = 1;
		info.MugongTotalInfo.mugong[0].bWear = FALSE;
		info.MugongTotalInfo.mugong[0].ExpPoint = 500;
		info.MugongTotalInfo.mugong[0].Position = TP_MUGONG1_START;
		info.MugongTotalInfo.mugong[0].QuickPosition = 0;
		info.MugongTotalInfo.mugong[0].Sung = 1;
		info.MugongTotalInfo.mugong[0].wIconIdx = GAMERESRCMNGR->m_TestClientInfo.MugongIdx;
		
		info.MugongTotalInfo.mugong[1].dwDBIdx = 2;
		info.MugongTotalInfo.mugong[1].bWear = FALSE;
		info.MugongTotalInfo.mugong[1].ExpPoint = 200;
		info.MugongTotalInfo.mugong[1].Position = TP_MUGONG1_START+1;
		info.MugongTotalInfo.mugong[1].QuickPosition = 0;
		info.MugongTotalInfo.mugong[1].Sung = 0;
		info.MugongTotalInfo.mugong[1].wIconIdx = 200;
		
		info.MugongTotalInfo.mugong[2].dwDBIdx = 3;
		info.MugongTotalInfo.mugong[2].bWear = FALSE;
		info.MugongTotalInfo.mugong[2].ExpPoint = 1000;
		info.MugongTotalInfo.mugong[2].Position = TP_MUGONG1_START+2;
		info.MugongTotalInfo.mugong[2].QuickPosition = 0;
		info.MugongTotalInfo.mugong[2].Sung = 2;
		info.MugongTotalInfo.mugong[2].wIconIdx = 302;
		for(int n=0;n<eAGK_Max;++n)
		{
			memset(info.AbilityInfo.AbilityDataArray[n],'B',MAX_ABILITY_NUM_PER_GROUP);
			memset(info.AbilityInfo.AbilityQuickPositionArray[n],' ',MAX_ABILITY_NUM_PER_GROUP);
		}
		OBJECTMGR->RegisterHero(&info);
		GAMEIN->GetMiniMapDialog()->AddHeroIcon(HERO);
		
		CAMERA->Init((CObject*)OBJECTMGR->GetHero(),30,0,1000);
		
		VECTOR2 sidpos;
		sidpos.x = 140;
		sidpos.y = 0;
		STATUSICONDLG->Init(HERO,&sidpos,5);


		BASEOBJECT_INFO obinfo;
		BASEMOVE_INFO moinfo;
		memset( &obinfo, 0, sizeof(obinfo) );		
		memset( &moinfo, 0, sizeof(moinfo) );

		obinfo.dwObjectID = 100;
		obinfo.dwUserID = 5572;
		sprintf( obinfo.ObjectName, "pet-01" );
		obinfo.BattleID = gChannelNum+2;

		moinfo.CurPosition = HERO->GetCurPosition();
		moinfo.CurPosition.x -= 150;

		if(GAMERESRCMNGR->m_TestClientInfo.bInTitan == TRUE)
		{
			HERO->RidingTitan( TRUE );
			HERO->SetTitanAppearanceInfo( &GAMERESRCMNGR->m_TestClientInfo.titaninfo );
			HERO->SetTitanPreView( &GAMERESRCMNGR->m_TestClientInfo.titaninfo );
		}		

		CAMERA->InitCamera( 0, CAMERA->GetCameraDesc()->m_AngleX.ToDeg(), CAMERA->GetCameraDesc()->m_AngleY.ToDeg(),
			1500, HERO, TRUE );
		MAP->ToggleSunPosForShadow( TRUE );
	
	//	CPetBase* pPet = OBJECTMGR->AddPet( &obinfo, &moinfo );
	//	HERO->SetPet( pPet );
	}
}


void InitTestMonster()
{
	DWORD id = 10;
	float x,z;
	x = GAMERESRCMNGR->m_TestClientInfo.x + 25;
	z = GAMERESRCMNGR->m_TestClientInfo.z + 500 + 25;


	DWORD temp =0;
	for(int n=0;n<GAMERESRCMNGR->m_TestClientInfo.MonsterNum;++n)
	{
		SEND_MONSTER_TOTALINFO info;
		memset(&info,0,sizeof(info));
		info.BaseObjectInfo.dwObjectID = id++;
		info.BaseObjectInfo.BattleID = gChannelNum+1;
		sprintf(info.BaseObjectInfo.ObjectName,"Monster%02d",temp++);
		VECTOR3 pos;
		pos.x = x;
		pos.z = z;
		info.MoveInfo.CurPos.Compress(&pos);
		BASEMOVE_INFO bminfo;
		bminfo.SetFrom(&info.MoveInfo);
		info.TotalInfo.MonsterKind = GAMERESRCMNGR->m_TestClientInfo.MonsterKind;
		g_pMonster = OBJECTMGR->AddMonster(&info.BaseObjectInfo,&bminfo,&info.TotalInfo);
		
		x+= 100;
	}

	temp =0;
	for(n=0;n<GAMERESRCMNGR->m_TestClientInfo.BossMonsterNum;++n)
	{
		SEND_MONSTER_TOTALINFO info;
		memset(&info,0,sizeof(info));
		info.BaseObjectInfo.dwObjectID = id++;
		sprintf(info.BaseObjectInfo.ObjectName,"BossMonster%02d",temp++);
		VECTOR3 pos;
		pos.x = x;
		pos.z = z;
		info.MoveInfo.CurPos.Compress(&pos);
		BASEMOVE_INFO bminfo;
		bminfo.SetFrom(&info.MoveInfo);
		info.TotalInfo.MonsterKind = GAMERESRCMNGR->m_TestClientInfo.BossMonsterKind;
		g_pBossMonster = OBJECTMGR->AddBossMonster(&info.BaseObjectInfo,&bminfo,&info.TotalInfo);
	//	g_pBossMonster->LoadBossMonster(GAMERESRCMNGR->m_TestClientInfo.BossInfofile);
		
		x+= 100;
	}

	x = GAMERESRCMNGR->m_TestClientInfo.x + 25;
	z -= 200;
	int lastnp = 0;
	for(n=0;n<GAMERESRCMNGR->m_TestClientInfo.CharacterNum;++n)
	{
		SEND_CHARACTER_TOTALINFO cinfo;
		memset(&cinfo,0,sizeof(cinfo));
		cinfo.BaseObjectInfo.dwObjectID = id++;
		sprintf(cinfo.BaseObjectInfo.ObjectName,"Character%02d",temp++);
		VECTOR3 pos;
		pos.x = x;
		pos.z = z;
		cinfo.MoveInfo.CurPos.Compress(&pos);
		BASEMOVE_INFO bminfo;
		bminfo.SetFrom(&cinfo.MoveInfo);
		cinfo.TotalInfo.bVisible = 1;
		cinfo.TotalInfo.Height = 1;
		cinfo.TotalInfo.Width = 1;
		cinfo.TotalInfo.bVisible = 1;
		cinfo.TotalInfo.WearedItemIdx[eWearedItem_Dress] = 23000 + rand()%150;
		cinfo.TotalInfo.WearedItemIdx[eWearedItem_Weapon] = 11000 + rand()%150;
		OBJECTMGR->AddPlayer(&cinfo.BaseObjectInfo,&bminfo,&cinfo.TotalInfo,NULL);
		
		x+= 100;
		if(n/50 != lastnp)
		{
			lastnp = n/50;
			x = GAMERESRCMNGR->m_TestClientInfo.x + 25;
			z -= 200;
		}
	}
	
/*
	SEND_MONSTER_TOTALINFO info;
	memset(&info,0,sizeof(info));
	info.BaseObjectInfo.dwObjectID = id++;
	sprintf(info.BaseObjectInfo.ObjectName,"BossMonster%02d",temp++);
	info.BaseMoveInfo.CurPosition.x = x;
	info.BaseMoveInfo.CurPosition.z = z;
	info.TotalInfo.MonsterKind = 65;
	g_pBossMonster = OBJECTMGR->AddBossMonster(&info.BaseObjectInfo,&info.BaseMoveInfo,&info.TotalInfo);
*/
}

//trustpak
void InitTestCharacters(void)
{	
	DWORD id = 1000;
	DWORD temp =0;

	//for(n=0;n<GAMERESRCMNGR->m_TestClientInfo.CharacterNum;++n)
	DWORD n = 0;
	for (n = 0; n < GAMERESRCMNGR->m_TestClientInfo.dwTestCharacterNo;n++)
	{
		SEND_CHARACTER_TOTALINFO cinfo;
		memset(&cinfo,0,sizeof(cinfo));
		cinfo.BaseObjectInfo.dwObjectID = id++;
		sprintf(cinfo.BaseObjectInfo.ObjectName,"Test Character%02d",temp++);
		
		VECTOR3 pos;
		pos.x = GAMERESRCMNGR->m_TestClientInfo.aTestCharacterInfo[n].fX;
		pos.y = GAMERESRCMNGR->m_TestClientInfo.aTestCharacterInfo[n].fY;
		pos.z = GAMERESRCMNGR->m_TestClientInfo.aTestCharacterInfo[n].fZ;
		cinfo.MoveInfo.CurPos.Compress(&pos);

		BASEMOVE_INFO bminfo;
		bminfo.SetFrom(&cinfo.MoveInfo);
		cinfo.TotalInfo.bVisible = 1;
		cinfo.TotalInfo.Height = 1;
		cinfo.TotalInfo.Width = 1;
		cinfo.TotalInfo.bVisible = 1;
		cinfo.TotalInfo.WearedItemIdx[eWearedItem_Dress] = 23000 + rand()%150;
		cinfo.TotalInfo.WearedItemIdx[eWearedItem_Weapon] = 11000 + rand()%150;		
		cinfo.TotalInfo.Gender = GAMERESRCMNGR->m_TestClientInfo.aTestCharacterInfo[n].byGender;

		OBJECTMGR->AddPlayer(&cinfo.BaseObjectInfo,&bminfo,&cinfo.TotalInfo,NULL);		
		CObject * pObj = OBJECTMGR->GetObject(cinfo.BaseObjectInfo.dwObjectID);
		pObj->SetAngle(GAMERESRCMNGR->m_TestClientInfo.aTestCharacterInfo[n].fAngle);		
	}	
}

#endif
