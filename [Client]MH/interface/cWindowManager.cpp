// cWindowManager.cpp: implementation of the cWindowManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cWindowManager.h"
#include "cScriptManager.h"
#include "GameResourceManager.h"
#include "../GlobalEventFunc.h"

#include "..\CharMake.h"
#include "..\MainDialog.h"
#include "..\GameIn.h"
#include "..\WindowIDEnum.h"
#include "..\ObjectManager.h"
#include "..\CharacterDialog.h"
#include "..\MugongManager.h"
#include "..\ItemManager.h"
#include "..\InventoryExDialog.h"
#include "..\MoneyDlg.h"
#include "cTabDialog.h"
#include "..\AlertDlg.h"
#include "cWindowFunc.h"
#include "../MousePointer.h"
#include "../input/cIMEWnd.h"
#include "../input/Mouse.h"
#include "cAnimationManager.h"
#include "../MiniMapDlg.h"
#include "../BigMapDlg.h"
#include "../MHMap.h"
#include "../cMsgBox.h"
#include "../cDivideBox.h"
#include "cWindowSystemFunc.h"
#include "mhFile.h"
#include "SuryunDialog.h"

#include "../MugongSuryunDialog.h"
#include "MainBarDialog.h"
#include "GuildCreateDialog.h"
#include "GuildDialog.h"
#include "GuildInviteDialog.h"
#include "GuildMarkDialog.h"
#include "GuildLevelUpDialog.h"
#include "GuildNickNameDialog.h"
#include "GuildRankDialog.h"
#include "GuildWarehouseDialog.h"
#include "PyogukDialog.h"
#include "MacroDialog.h"
#include "ChatOptionDialog.h"
#include "ChatDialog.h"
#include "OptionDialog.h"
#include "CharStateDialog.h"
#include "MenuSlotDialog.h"

#include "NoteDialog.h"
#include "FriendDialog.h"
#include "MiniFriendDialog.h"
#include "MiniNoteDialog.h"
#include "ReviveDialog.h"
#include "ExchangeDialog.h"

// LYJ 051017 구입노점상 추가
#include "StallKindSelectDlg.h"
#include "StreetBuyStall.h"
#include "BuyRegDialog.h"

// 노점상 검색 다이얼로그 추가 by Stiner(8)
#include "StallFindDlg.h"

// 노점상 보기 다이얼로크 추가 by Stiner(8)

#include "StreetStall.h"
#include "NpcScriptDialog.h"
#include "HelpDialog.h"
#include "DealDialog.h"
#include "QuickDialog.h"
#include "InventoryExDialog.h"
#include "UpgradeDlg.h"
#include "MixDialog.h"
#include "TitanPartsMakeDlg.h"	// magi82 - Titan(070112)
#include "TitanMixDlg.h"		// magi82 - Titan(070119)
#include "TitanRepairDlg.h"
#include "TitanRecallDlg.h"
#include "TitanUpgradeDlg.h"	// magi82 - Titan(070119)
#include "TitanBreakDlg.h"		// magi82 - Titan(070119)
#include "TitanGuageDlg.h"		// magi82 - Titan(070305)
//#include "TitanMugongMixDlg.h"	// magi82 - Titan(070611) 타이탄 무공변환 주석처리
#include "TitanRegisterDlg.h"	// magi82 - Titan(070320)
#include "TitanDissolutionDlg.h"	// magi82 - Titan(070321)
#include "TitanChangePreView.h"
#include "TitanInventoryDlg.h"
#include "ReinforceDlg.h"
#include "DissloveDlg.h"
#include "WantNpcDialog.h"
#include "WantRegistDialog.h"
#include "QuestTotalDialog.h"
#include "QuestDialog.h"
#include "WantedDialog.h"
#include "JournalDialog.h"
#include "PartyCreateDlg.h"
#include "PartyInviteDlg.h"
#include "BailDialog.h"
#include "MPRegistDialog.h"
#include "MPMissionDialog.h"
#include "MPNoticeDialog.h"
#include "MPGuageDialog.h"

#include "DissolutionDialog.h"
#include "EventNotifyDialog.h"
#include "ItemShopDialog.h"
#include "MoveDialog.h"
#include "PointSaveDialog.h"

#include "./Input/UserInput.h"
#include "MouseCursor.h"
#include "StatusIconDlg.h"

#include "GuildFieldWarDialog.h"

#include "SkillPointRedist.h"
#include "SkillPointNotify.h"

#include "PartyWarDialog.h"

#include "ShoutDialog.h"
#include "ShoutchatDialog.h"
#include "ChaseDialog.h"
#include "ChaseinputDialog.h"
#include "NameChangeDialog.h"
#include "NameChangeNotifyDlg.h"

// Guild Tournament
#include "GTRegistDialog.h"
#include "GTRegistcancelDialog.h"
#include "GTStandingDialog.h"
#include "GTBattleListDialog.h"
#include "GTScoreInfoDialog.h"

// seigewar
#include "SeigeWarDialog.h"

// Character change
#include "CharChangeDlg.h"

#include "SealDialog.h"
#include "ChangeJobDialog.h"
#include "GuageDialog.h"
#include "ReinforceResetDlg.h"
#include "RareCreateDialog.h"

#include "PetManager.h"
#include "PetStateDlg.h"
#include "PetStateMiniDlg.h"
#include "PetInventoryDlg.h"
#include "PetUpgradeDialog.h"
#include "PetRevivalDialog.h"

//#include "cJackpotDialog.h"
#include "ReinforceDataGuideDlg.h"
#include "TipBrowserDlg.h"
#include "KeySettingTipDlg.h"
#include "cIMEEx.h"

#include "GuildNoteDlg.h"
#include "UnionNoteDlg.h"

#include "GuildNoticeDlg.h"
#include "SkillOptionChangeDlg.h"
#include "SkillOptionClearDlg.h"

#include "guildjoindialog.h"

#ifdef _HACK_SHIELD_
#include "HackShieldManager.h"
#endif

//screenshot
#include "ScreenShotDlg.h"

#include "GuildPlusTimeDialog.h"
#include "GuildMunhaDialog.h"
#include "GuildTraineeDialog.h"

#include "SurvivalModeManager.h"
#include "SurvivalCountDialog.h"

#include "UniqueItemCurseCancellationDlg.h"
#include "UniqueItemMixDlg.h"
#include "MHCamera.h"

#include "SOSDialog.h"

#include "UniqueItemMixProgressBarDlg.h"	 //2007. 10. 22. CBH
#include "TitanMixProgressBarDlg.h"			 //2007. 10. 22. CBH
#include "TitanPartsProgressBarDlg.h"		//2007. 10. 22. CBH

#include "SkinSelectDialog.h"	//2007. 12. 4. CBH
#include "CostumeSkinSelectDialog.h"	//2007. 12. 4. CBH

#include "SkillPointResetDlg.h"

#include "NumberPadDialog.h"	//2008. 3. 17. CBH
#include "StreetStallItemViewDlg.h"
#include "AutoNoteDlg.h"	// autonote
#include "AutoAnswerDlg.h"

#include "PartyMatchingDlg.h"

// 요새전
#include "FortWarDialog.h"

extern HWND _g_hWnd;

GLOBALTON(cWindowManager);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cWindowManager::cWindowManager()
{
	m_pScriptManager	= NULL;
	m_pWindowList		= NULL;
	m_pIconDlgListRef	= NULL;
	m_pEditBoxListRef	= NULL;
	m_pDestroyWindowRef = NULL;
	m_pScreenTextList	= NULL;
	m_OldDragFlag		= FALSE;
//	ID_SEED = 0;	//?
//	m_CommonDlgIDIndex.Init(MAX_COMMONDLGID_NUM, 500);			//start index 5000 tmp
	m_pMousePointWindow = NULL;
	m_pGuageWindow		= NULL;
	m_pMonsterGuageWindow = NULL;
	m_fComposition		= FALSE;
	cbDragnDropProcess	= NULL;
	
	m_pFocusedEdit		= NULL;
	
	m_id			= -1;
	m_pDragDialog	= NULL;

//==== for rec.
	m_bHideInterface = FALSE;

#ifdef _JAPAN_LOCAL_
//	m_dwConversion = IME_CMODE_NATIVE | IME_CMODE_FULLSHAPE;
//	m_dwSentence = IME_SMODE_PHRASEPREDICT;
	m_bOpenStatus = FALSE;
#endif
}

cWindowManager::~cWindowManager()
{
//	Release();	//PJH 여기서 실행하면 두번 Release하게 된다
}
void cWindowManager::Release()
{
	DestroyWindowAll();
	SAFE_DELETE(m_pScriptManager);
	SAFE_DELETE(m_pIconDlgListRef);
	SAFE_DELETE(m_pEditBoxListRef);
	SAFE_DELETE(m_pScreenTextList);
	SAFE_DELETE(m_pPlayerChatTooltip);
	SAFE_DELETE(m_pWindowList);
	SAFE_DELETE(m_pDestroyWindowRef);
	SAFE_DELETE(m_pMousePointWindow);
	SAFE_DELETE(m_pGuageWindow);
	SAFE_DELETE(m_pMonsterGuageWindow);
	
	CFONT_OBJ->Release();
}

void cWindowManager::Init()
{
	m_pScriptManager = new cScriptManager;
	m_pWindowList = new cPtrList;
	m_pEditBoxListRef = new cPtrList;
	m_pIconDlgListRef = new cPtrList;
	m_pDestroyWindowRef = new cPtrList;
	m_pScreenTextList = new cPtrList;
	m_pPlayerChatTooltip = new cPtrList;

	m_pScriptManager->InitParseTypeData(); //2007. 5. 18. CBH - Parse Type List 초기화 추가

	LOGFONT	font;

//pjslocal
	
#ifdef TAIWAN_LOCAL
	font.lfHeight = -MulDiv(9, GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY), 72);
//	font.lfHeight = -12;
	font.lfWidth = -font.lfHeight / 2;
	font.lfEscapement = 0;
	font.lfOrientation = 0;
	font.lfWeight = FW_NORMAL;
	font.lfItalic = 0;
	font.lfUnderline = 0;
	font.lfStrikeOut = 0;
	font.lfCharSet = GB2312_CHARSET;	//간체
	font.lfOutPrecision = 0;
	font.lfClipPrecision = 0;
	font.lfQuality =  PROOF_QUALITY;//NONANTIALIASED_QUALITY;//PROOF_QUALITY;
	font.lfPitchAndFamily = 0;
//	lstrcpy(font.lfFaceName,"꾄ⁿ톱");
	lstrcpy(font.lfFaceName,"芥竟"); 

	CFONT_OBJ->CreateFontObject(&font, cFont::FONT0);

	font.lfUnderline = 1;
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT4);
	
	font.lfUnderline = 0;
	font.lfHeight = -14;
	font.lfWidth = -font.lfHeight / 2;
	lstrcpy(font.lfFaceName,"System");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT1);

	font.lfUnderline = 0;
	font.lfHeight = -14;
	font.lfWidth = -font.lfHeight / 2;
	lstrcpy(font.lfFaceName,"芥竟");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT2);
	
	font.lfHeight = -MulDiv(35, GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY), 72);
	font.lfWidth = -font.lfHeight / 2;
//	lstrcpy(font.lfFaceName,"꾄ⁿ톱");
	lstrcpy(font.lfFaceName,"芥竟");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT3);
#elif defined _JAPAN_LOCAL_
	font.lfHeight = -MulDiv(9, GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY), 72);
//	font.lfHeight = -12;
	font.lfWidth = -font.lfHeight / 2;
	font.lfEscapement = 0;
	font.lfOrientation = 0;
	font.lfWeight = 0;//FW_NORMAL;
	font.lfItalic = 0;
	font.lfUnderline = 0;
	font.lfStrikeOut = 0;
	font.lfCharSet = SHIFTJIS_CHARSET;//DEFAULT_CHARSET; //SHIFTJIS_CHARSET;	//
	font.lfOutPrecision = 0;
	font.lfClipPrecision = 0;
	font.lfQuality = NONANTIALIASED_QUALITY;//PROOF_QUALITY;//NONANTIALIASED_QUALITY;//PROOF_QUALITY;
	font.lfPitchAndFamily = FIXED_PITCH|FF_MODERN;
//	lstrcpy(font.lfFaceName,"MS PGothic"); 
//	lstrcpy(font.lfFaceName,"MS UI Gothic"); 
//	lstrcpy(font.lfFaceName,"MS UI Gothic"); 
	lstrcpy(font.lfFaceName,"MS Gothic");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT0);

	font.lfUnderline = 1;
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT4);
	
	font.lfUnderline = 0;
	font.lfHeight = -14;
	font.lfWidth = -font.lfHeight / 2;
	font.lfWeight = FW_BOLD;
	lstrcpy(font.lfFaceName,"MS Gothic");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT1);

	font.lfUnderline = 0;
	font.lfHeight = -14;
	font.lfWidth = -font.lfHeight / 2;
	font.lfWeight = 0;
	lstrcpy(font.lfFaceName,"MS Gothic");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT2);
	
	font.lfHeight = -MulDiv(35, GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY), 72);
	font.lfWidth = -font.lfHeight / 2;
	lstrcpy(font.lfFaceName,"MS Gothic");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT3);

#elif defined _HK_LOCAL_
	font.lfHeight = -MulDiv(9, GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY), 72);
//	font.lfHeight = -12;
	font.lfWidth = -font.lfHeight / 2;
	font.lfEscapement = 0;
	font.lfOrientation = 0;
	font.lfWeight = 0;//FW_NORMAL;
	font.lfItalic = 0;
	font.lfUnderline = 0;
	font.lfStrikeOut = 0;
	font.lfCharSet = CHINESEBIG5_CHARSET;//DEFAULT_CHARSET; //SHIFTJIS_CHARSET;	//
	font.lfOutPrecision = 0;
	font.lfClipPrecision = 0;
	font.lfQuality = NONANTIALIASED_QUALITY;//PROOF_QUALITY;//NONANTIALIASED_QUALITY;//PROOF_QUALITY;
	font.lfPitchAndFamily = FIXED_PITCH|FF_MODERN;
	lstrcpy(font.lfFaceName,"MingLiU");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT0);

	font.lfUnderline = 1;
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT4);
	
	font.lfUnderline = 0;
	font.lfHeight = -14;
	font.lfWidth = -font.lfHeight / 2;
	font.lfWeight = FW_BOLD;
	lstrcpy(font.lfFaceName,"MingLiU");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT1);

	font.lfUnderline = 0;
	font.lfHeight = -14;
	font.lfWidth = -font.lfHeight / 2;
	font.lfWeight = 0;
	lstrcpy(font.lfFaceName,"MingLiU");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT2);
	
	font.lfHeight = -MulDiv(35, GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY), 72);
	font.lfWidth = -font.lfHeight / 2;
	lstrcpy(font.lfFaceName,"MingLiU");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT3);
#elif defined _TL_LOCAL_
	font.lfHeight = -MulDiv(8, GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY), 72);
//	font.lfHeight = -12;
	font.lfWidth = -font.lfHeight / 2;
	font.lfEscapement = 0;
	font.lfOrientation = 0;
	font.lfWeight = 0;//FW_NORMAL;
	font.lfItalic = 0;
	font.lfUnderline = 0;
	font.lfStrikeOut = 0;
	font.lfCharSet = THAI_CHARSET;//DEFAULT_CHARSET; //SHIFTJIS_CHARSET;	//
	font.lfOutPrecision = 0;
	font.lfClipPrecision = 0;
	font.lfQuality = NONANTIALIASED_QUALITY;//PROOF_QUALITY;//NONANTIALIASED_QUALITY;//PROOF_QUALITY;
	font.lfPitchAndFamily = FIXED_PITCH|FF_MODERN;
	lstrcpy(font.lfFaceName,"Microsoft Sans serif");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT0);

	font.lfUnderline = 1;
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT4);
	
	font.lfUnderline = 0;
	font.lfHeight = -14;
	font.lfWidth = -font.lfHeight / 2;
	font.lfWeight = FW_BOLD;
	lstrcpy(font.lfFaceName,"Microsoft Sans serif");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT1);

	font.lfUnderline = 0;
	font.lfHeight = -14;
	font.lfWidth = -font.lfHeight / 2;
	font.lfWeight = 0;
	lstrcpy(font.lfFaceName,"Microsoft Sans serif");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT2);
	
	font.lfHeight = -MulDiv(35, GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY), 72);
	font.lfWidth = -font.lfHeight / 2;
	lstrcpy(font.lfFaceName,"Microsoft Sans serif");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT3);

#else
	extern HWND _g_hWnd;
	font.lfHeight = -MulDiv(9, GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY), 72);
//	font.lfHeight = -12;
	font.lfWidth = -font.lfHeight / 2;
	font.lfEscapement = 0;
	font.lfOrientation = 0;
	font.lfWeight = FW_NORMAL;	
//	font.lfWeight = FW_DONTCARE;
	font.lfItalic = 0;
	font.lfUnderline = 0;
	font.lfStrikeOut = 0;
//	font.lfCharSet = DEFAULT_CHARSET;
	font.lfCharSet = CHINESEBIG5_CHARSET;
	font.lfOutPrecision = 0;
//	font.lfClipPrecision = 0;
	font.lfQuality = PROOF_QUALITY;//NONANTIALIASED_QUALITY;//PROOF_QUALITY;
	font.lfPitchAndFamily = 0;
	lstrcpy(font.lfFaceName,"MingLiU");	
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT0);

	font.lfHeight = -MulDiv(9, GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY), 72);	
	font.lfWidth = -font.lfHeight / 2;		
	lstrcpy(font.lfFaceName,"MingLiU");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT5);

	font.lfHeight = -MulDiv(9, GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY), 72);	
	font.lfWidth = -font.lfHeight / 2;		
	font.lfUnderline = 1;	
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT4);
	
	font.lfUnderline = 0;
	font.lfHeight = -14;
	font.lfWidth = -font.lfHeight / 2;	
	lstrcpy(font.lfFaceName,"System");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT1);

	font.lfUnderline = 0;
	font.lfHeight = -14;
	font.lfWidth = -font.lfHeight / 2;
	lstrcpy(font.lfFaceName,"MingLiU");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT2);
	
	font.lfHeight = -MulDiv(35, GetDeviceCaps(GetDC( _g_hWnd ), LOGPIXELSY), 72);
	font.lfWidth = -font.lfHeight / 2;
	lstrcpy(font.lfFaceName,"MingLiU");
	CFONT_OBJ->CreateFontObject(&font, cFont::FONT3);
#endif


//KES MESSAGEBOX 030827
	cMsgBox::InitMsgBox();
	//dividebox도?
}

void cWindowManager::CreatMousePoint()
{
/*	
#ifdef _FILE_BIN_
	cWindow * window = GetDlgInfoFromFile("./image/Mouse/mouse.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/Mouse/mouse.txt");
#endif
	m_pMousePointWindow = (CMousePointer *)window;
	m_pMousePointWindow->Linking();
*/
}


void cWindowManager::CreateGameIn()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/15.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/15.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/15.txt");
#endif
	AddWindow(window);
	
	GAMEIN->SetMainInterfaceDialog((CMainBarDialog * )window);
	
	GAMEIN->GetMainInterfaceDialog()->InitBar();	

	CreateMonsterGuageDlg();

	CreateCharDlg();
	CreateMugongSuryunDlg();
	CreateInventoryDlg();
	CreateRecvExchangeDlg();
	
	CreateHeroGuage();
	CreateDealDlg();
	//CreateMixDlg();
	CreateChatDlg();
	// SW050715
//	CreateJackpotDlg();

	// LYJ 051017 구입노점상 추가
	CreateStallKindSelectDlg();
	CreateStreetBuyStallDlg();
	CreateBuyRegDlg();

	// 노점 검색창 추가 by Stiner(8)
	CreateStallFindDlg();

	CreateStreetStallDlg();
	CreateNpcScriptDlg();
	CreateHelpDlg();

	CreateQuickDlg();
	CreatePartyDlg();
	CreatePartyCreateDlg();
	CreatePartyInviteDlg();
	CreateGuildCreateDlg();
	CreateGuildNickNameDlg();
	CreateGuildRankDlg();
	CreateGuildDlg();
	CreateGuildInviteDlg();
	CreateGuildMarkDlg();
	CreateGuildLevelupDlg();
	CreateGuildWarehouseDlg();
	CreateMiniMapDlg();
	CreatePyogukDlg();
	CreateFriendDlg();
	CreateMiniFriendDlg();
	CreateNoteDlg();
	CreateMiniNoteDlg();
	CreateReviveDlg();
	CreateWantNpcDlg();
	CreateWantRegistDlg();
	CreateQuestTotalDlg();
	CreateMPRegistDlg();

	CreateOptionDlg();
	CreateEventNotifyDlg();

//KES
#ifdef _TESTCLIENT_	
//	CreateMacroDlg();
//	CreateChatOptionDlg();
#endif

	CreateExitDlg();

	CreateExchangeDlg();
//	CreateCharStateDlg();
	CreateUpgradeDlg();
	CreateMixDlg();
	CreateTitanPartsProgressBarDlg();	// 2007. 10. 23. CBH - 타이탄 파츠 조합 게이지
	CreateTitanPartsMakeDlg();		// magi82 - Titan(070112)
	CreateTitanMixProgressBarDlg();	// 2007. 10. 22. CBH - 타이탄 조합 게이지
	CreateTitanMixDlg();			// magi82 - Titan(070119)	
	CreateTitanRepairDlg();			// 2007. 9. 11. CBH - 타이탄 수리창
	CreateTitanRecallDlg();			// 2007. 9. 13. CBH - 타이탄 소환 게이지
	CreateTitanUpgradeDlg();		// magi82 - Titan(070119)
	CreateTitanBreakDlg();			// magi82 - Titan(070119)
	CreateTitanChangePreViewDlg();	// magi82 - Titan(070212)
	CreateTitanInventoryDlg();		// magi82 - Titan(070222)
	CreateTitanGuageDlg();			// magi82 - Titan(070305)
	//CreateTitanMugongMixDlg();		// magi82 - Titan(070611) 타이탄 무공변환 주석처리
	CreateTitanRegisterDlg();		// magi82 - Titan(070320)
	CreateTitanDissolution();		// magi82 - Titan(070321)
	CreateReinforceDlg();
	CreateReinforceDataGuideDlg();
	//CreateDissolveDlg();

//KES 040510
//	CreateMenuSlotDlg();


	// RaMa
	CreateItemShopDlg();
	CreateMoveDlg();
	CreatePointSaveDlg();
	CreateBailDlg();
	CreateDissolutionDlg();
	CreateMallNotcieDlg();

	// guildfieldwar
	CreateGuildFieldWarDlg();

	CreateSkPointDlg();
	CreateSkPointNotifyDlg();

	// partywar
	CreatePartyWarDlg();

	CreateShoutDlg();
	CreateShoutchatDlg();
	CreateChaseDlg();
	CreateChaseinputDlg();
	CreateNameChangeDlg();
	CreateNameChangeNotifyDlg();

	// Guild Tournament
	CreateGTRegistDlg();
	CreateGTRegistcancelDlg();
	CreateGTStandingDlg();
	CreateGTBattleListDlg();
	CreateGTScoreInfoDlg();

	// seigewar
	CreateSeigeWarDlg();

	// Character Change
	CreateCharChangeDlg();

	CreateItemSealDlg();

	//
	CreateChangeJobDlg();
	// 06. 02. 강화 재료 안내 인터페이스 추가 - 이영준
#ifndef _CHINA_LOCAL_
	CreateReinforceResetDlg();
#endif
    CreateRareCreateDlg();

	// Pet
	CreatePetStateDlg();
	CreatePetStateMiniDlg();
	CreatePetInvenDlg();
	CreatePetUpgradeDlg();
	CreatePetRevivalDlg();


	// 06. 02. 내정보창 팁보기 추가 - 이영준
	CreateTipBrowserDlgDlg();
#ifndef _HK_LOCAL_	//hk block
	CreateKeySettingTipDlg();
#endif

	CreateGuildNoteDlg();
	CreateUnionNoteDlg();

	CreateGuildNoticeDlg();

	/////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환
	CreateSkillOptionChangeDlg();
	CreateSkillOptionClearDlg();
	/////////////////////////////////////////////

	//SW060713 문하생
	CreateGuildInvitationKindSelectionDlg();
	CreateGuildMunhaDlg();
	CreateGuildTraineeDlg();

	//screenshot
	ScreenShotDlg();
	//SW061019 서바이벌모드
	CreateSurvivalCountDlg();

	//2007. 10. 22. CBH - 유니크 아이탬 조합 게이지
	CreateUniqueItemMixProgressBarDlg();	
	
	// magi82 - UniqueItem(070703)
	CreateUniqueItemCurseCancellationDlg();

	// magi82 - UniqueItem(070709)
	CreateUniqueItemMixDlg();

	// magi82 - SOS(070724)
	CreateSOSDlg();		

	// 2007. 12. 4. CBH - 스킨 선택 다이얼로그
	CreateSkinSelectDlg();
	// 2008. 1. 16. CBH - 코스튬 스킨 선택 다이얼로그
	CreateCostumeSkinSelectDlg();

	// magi82(42) - 샵아이템 추가(수련치 초기화)
	CreateSkillPointResetDlg();	

	// magi82(47)
	CreateStreetStallItemViewDlg();

#ifdef _KOR_LOCAL_
	CreateChannelDlg();	//2008. 5. 8. CBH - 채널선택 다이얼로그		
#endif	
	CreatePartyMatchingDlg(); // 2008. 5. 23. CBH - 방파 매칭 다이얼로그
}

void cWindowManager::CreatePartyWarDlg()
{
	cWindow* window = NULL;
#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile( "./image/PartyWarDlg.bin", "rb" );
	window = GetDlgInfoFromFile( "./image/InterfaceScript/PartyWarDlg.bin", "rb" );
#else
	window = GetDlgInfoFromFile( "./image/PartyWarDlg.txt" );
#endif

	AddWindow( window );
	GAMEIN->SetPartyWarDlg( (CPartyWarDialog*)window );
	GAMEIN->GetPartyWarDlg()->Linking();
}

void cWindowManager::CreateGuildFieldWarDlg()
{
	cWindow* window = NULL;
#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile( "./image/GFWarDeclare.bin", "rb" );
	window = GetDlgInfoFromFile( "./image/InterfaceScript/GFWarDeclare.bin", "rb" );
#else
	window = GetDlgInfoFromFile( "./image/GFWarDeclare.txt" );
#endif

	AddWindow( window );
	GAMEIN->SetGFWarDeclareDlg( (CGFWarDeclareDlg*)window );
	GAMEIN->GetGFWarDeclareDlg()->Linking();

#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile( "./image/GFWarResult.bin", "rb" );
	window = GetDlgInfoFromFile( "./image/InterfaceScript/GFWarResult.bin", "rb" );
#else
	window = GetDlgInfoFromFile( "./image/GFWarResult.txt" );
#endif

	AddWindow( window );
	GAMEIN->SetGFWarResultDlg( (CGFWarResultDlg*)window );
	GAMEIN->GetGFWarResultDlg()->Linking();

#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile( "./image/GFWarInfo.bin", "rb" );
	window = GetDlgInfoFromFile( "./image/InterfaceScript/GFWarInfo.bin", "rb" );
#else
	window = GetDlgInfoFromFile( "./image/GFWarInfo.txt" );
#endif

	AddWindow( window );
	GAMEIN->SetGFWarInfoDlg( (CGFWarInfoDlg*)window );
	GAMEIN->GetGFWarInfoDlg()->Linking();
	
#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile( "./image/GuildWarInfo.bin", "rb" );
	window = GetDlgInfoFromFile( "./image/InterfaceScript/GuildWarInfo.bin", "rb" );
#else
	window = GetDlgInfoFromFile( "./image/GuildWarInfo.txt" );
#endif

	AddWindow( window );
	GAMEIN->SetGuildWarInfoDlg( (CGuildWarInfoDlg*)window );
	GAMEIN->GetGuildWarInfoDlg()->Linking();

#ifndef _HK_LOCAL_	//hk block
	//Add 060803 by wonju-------------------------------------------
#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile( "./image/GuildPlusTime.bin", "rb" );
	window = GetDlgInfoFromFile( "./image/InterfaceScript/GuildPlusTime.bin", "rb" );
#else
	window = GetDlgInfoFromFile( "./image/GuildPlusTime.txt" );
#endif

	AddWindow( window );
	GAMEIN->SetGuildPlusTimeDlg( (CGuildPlusTimeDlg*)window );
	GAMEIN->GetGuildPlusTimeDlg()->Linking();
	//---------------------------------------------------------------
#endif
}

void cWindowManager::CreateDissolveDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/58.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/58.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/58.txt");
#endif
//	m_pGuageWindow = (cDialog *)window;
	AddWindow(window);
	GAMEIN->SetDissolveDialog((CDissloveDlg * )window);
	GAMEIN->GetDissolveDialog()->Linking();
}
void cWindowManager::CreateUpgradeDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/55.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/55.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/55.txt");
#endif
//	m_pGuageWindow = (cDialog *)window;
	AddWindow(window);
	GAMEIN->SetUpgradeDialog((CUpgradeDlg * )window);
	GAMEIN->GetUpgradeDialog()->Linking();
}
void cWindowManager::CreateMixDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/56.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/56.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/56.txt");
#endif
//	m_pGuageWindow = (cDialog *)window;
	AddWindow(window);
	GAMEIN->SetMixDialog((CMixDialog * )window);
	GAMEIN->GetMixDialog()->Linking();
}

// magi82 - Titan(070112) ////////////////////////////////////////////////
void cWindowManager::CreateTitanPartsMakeDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/60.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/60.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/60.txt");
#endif
	AddWindow(window);
	GAMEIN->SetTitanPartsMakeDlg((CTitanPartsMakeDlg * )window);
	GAMEIN->GetTitanPartsMakeDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////

// magi82 - Titan(070119) ////////////////////////////////////////////////
void cWindowManager::CreateTitanMixDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/Titanmix.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Titanmix.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/Titanmix.txt");
#endif
	AddWindow(window);
	GAMEIN->SetTitanMixDlg((CTitanMixDlg * )window);
	GAMEIN->GetTitanMixDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////
// 2007. 9. 11. CBH - 타이탄 수리창 ////////////////////////////////////////////////
void cWindowManager::CreateTitanRepairDlg()
{
#ifdef _FILE_BIN_	
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Titan_Repair.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/Titan_Repair.txt");
#endif
	AddWindow(window);
	GAMEIN->SetTitanRepairDlg((CTitanRepairDlg * )window);
	GAMEIN->GetTitanRepairDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////

// 2007. 9. 13. CBH - 타이탄 소환 게이지 ////////////////////////////////////////////////
void cWindowManager::CreateTitanRecallDlg()
{
#ifdef _FILE_BIN_	
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Titan_recall.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/Titan_recall.txt");
#endif
	AddWindow(window);
	GAMEIN->SetTitanRecallDlg((CTitanRecallDlg * )window);
	GAMEIN->GetTitanRecallDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////

// magi82 - Titan(070119) ////////////////////////////////////////////////
void cWindowManager::CreateTitanUpgradeDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/TitanUpgrade.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/TitanUpgrade.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/TitanUpgrade.txt");
#endif
	AddWindow(window);
	GAMEIN->SetTitanUpgradeDlg((CTitanUpgradeDlg * )window);
	GAMEIN->GetTitanUpgradeDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////

// magi82 - Titan(070119) ////////////////////////////////////////////////
void cWindowManager::CreateTitanBreakDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/TitanBreak.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/TitanBreak.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/TitanBreak.txt");
#endif
	AddWindow(window);
	GAMEIN->SetTitanBreakDlg((CTitanBreakDlg * )window);
	GAMEIN->GetTitanBreakDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////

// magi82 - Titan(070212) ////////////////////////////////////////////////
void cWindowManager::CreateTitanChangePreViewDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/TitanPartsChangePreview.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/TitanPartsChangePreview.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/TitanPartsChangePreview.txt");
#endif
	AddWindow(window);
	GAMEIN->SetTitanChangePreViewDlg((CTitanChangePreViewDlg * )window);
	GAMEIN->GetTitanChangePreViewDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////

// magi82 - Titan(070222) ////////////////////////////////////////////////
void cWindowManager::CreateTitanInventoryDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/Titan_inventory.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Titan_inventory.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/Titan_inventory.txt");
#endif
	AddWindow(window);
	GAMEIN->SetTitanInventoryDlg((CTitanInventoryDlg * )window);
	GAMEIN->GetTitanInventoryDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////

// magi82 - Titan(070305) ////////////////////////////////////////////////
void cWindowManager::CreateTitanGuageDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/Titan_guage.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Titan_guage.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/Titan_guage.txt");
#endif
	AddWindow(window);
	GAMEIN->SetTitanGuageDlg((CTitanGuageDlg * )window);
	GAMEIN->GetTitanGuageDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////

// magi82 - Titan(070611) 타이탄 무공변환 주석처리
/*
void cWindowManager::CreateTitanMugongMixDlg()
{
#ifdef _FILE_BIN_
	cWindow * window = GetDlgInfoFromFile("./image/Titan_MugongMix.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/Titan_MugongMix.txt");
#endif
	AddWindow(window);
	GAMEIN->SetTitanMugongMixDlg((CTitanMugongMixDlg * )window);
	GAMEIN->GetTitanMugongMixDlg()->Linking();
}
*/

// magi82 - Titan(070320) ////////////////////////////////////////////////
void cWindowManager::CreateTitanRegisterDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/Titan_use.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Titan_use.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/Titan_use.txt");
#endif
	AddWindow(window);
	GAMEIN->SetTitanRegisterDlg((CTitanRegisterDlg * )window);
	GAMEIN->GetTitanRegisterDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////

// magi82 - Titan(070321) ////////////////////////////////////////////////
void cWindowManager::CreateTitanDissolution()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/Titan_Bongin.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Titan_Bongin.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/Titan_Bongin.txt");
#endif
	AddWindow(window);
	GAMEIN->SetTitanDissolutionDlg((CTitanDissolutionDlg * )window);
	GAMEIN->GetTitanDissolutionDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////

void cWindowManager::CreateReinforceDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/57.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/57.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/57.txt");
#endif
//	m_pGuageWindow = (cDialog *)window;
	AddWindow(window);
	GAMEIN->SetReinforceDialog((CReinforceDlg * )window);
	GAMEIN->GetReinforceDialog()->Linking();
}

void cWindowManager::CreateHeroGuage()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/23.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/23.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/23.txt");
#endif
//	m_pGuageWindow = (cDialog *)window;
	AddWindow(window);
	GAMEIN->SetGuageDlg((CGuageDialog * )window);
	GAMEIN->GetGuageDlg()->Linking();
}

void cWindowManager::CreateQuickDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/14.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/14.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/14.txt");
#endif
	ASSERT(window);
	AddWindow(window);
	GAMEIN->SetQuickDialog((CQuickDialog * )window);
}

void cWindowManager::CreateMenuSlotDlg()
{
#ifdef _FILE_BIN_
	cWindow * window = GetDlgInfoFromFile("./image/MenuSlot.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/MenuSlot.txt");
#endif
	ASSERT(window);
	AddWindow(window);
	GAMEIN->SetMenuSlotDialog((CMenuSlotDialog * )window);
	GAMEIN->GetMenuSlotDialog()->Linking();
}

void cWindowManager::CreatePartyDlg() 
{
#ifdef _FILE_BIN_
//	cWindow * member1window = GetDlgInfoFromFile("./image/partymember1dlg.bin", "rb");
//	cWindow * member2window = GetDlgInfoFromFile("./image/partymember2dlg.bin", "rb");
//	cWindow * member3window = GetDlgInfoFromFile("./image/partymember3dlg.bin", "rb");
//	cWindow * member4window = GetDlgInfoFromFile("./image/partymember4dlg.bin", "rb");
//	cWindow * member5window = GetDlgInfoFromFile("./image/partymember5dlg.bin", "rb");
//	cWindow * member6window = GetDlgInfoFromFile("./image/partymember6dlg.bin", "rb");
//	cWindow * partybtnwindow = GetDlgInfoFromFile("./image/PartyBtnDialog.bin", "rb");
	cWindow * member1window = GetDlgInfoFromFile("./image/InterfaceScript/partymember1dlg.bin", "rb");
	cWindow * member2window = GetDlgInfoFromFile("./image/InterfaceScript/partymember2dlg.bin", "rb");
	cWindow * member3window = GetDlgInfoFromFile("./image/InterfaceScript/partymember3dlg.bin", "rb");
	cWindow * member4window = GetDlgInfoFromFile("./image/InterfaceScript/partymember4dlg.bin", "rb");
	cWindow * member5window = GetDlgInfoFromFile("./image/InterfaceScript/partymember5dlg.bin", "rb");
	cWindow * member6window = GetDlgInfoFromFile("./image/InterfaceScript/partymember6dlg.bin", "rb");
	cWindow * partybtnwindow = GetDlgInfoFromFile("./image/InterfaceScript/PartyBtnDialog.bin", "rb");
#else
	cWindow * member1window = GetDlgInfoFromFile("./image/partymember1dlg.txt");
	cWindow * member2window = GetDlgInfoFromFile("./image/partymember2dlg.txt");
	cWindow * member3window = GetDlgInfoFromFile("./image/partymember3dlg.txt");
	cWindow * member4window = GetDlgInfoFromFile("./image/partymember4dlg.txt");
	cWindow * member5window = GetDlgInfoFromFile("./image/partymember5dlg.txt");
	cWindow * member6window = GetDlgInfoFromFile("./image/partymember6dlg.txt");
	cWindow * partybtnwindow = GetDlgInfoFromFile("./image/PartyBtnDialog.txt");
#endif
	
	ASSERT(member1window);	ASSERT(member2window);	ASSERT(member3window);
	ASSERT(member4window);	ASSERT(member5window);	ASSERT(member6window);
	ASSERT(partybtnwindow);
	
	AddWindow(member1window);	AddWindow(member2window);	AddWindow(member3window);
	AddWindow(member4window);	AddWindow(member5window);	AddWindow(member6window);
	AddWindow(partybtnwindow);
	
	GAMEIN->GetPartyDialog()->RegistMemberDlg(0, (CPartyMemberDlg*)member1window);	
	GAMEIN->GetPartyDialog()->RegistMemberDlg(1, (CPartyMemberDlg*)member2window);
	GAMEIN->GetPartyDialog()->RegistMemberDlg(2, (CPartyMemberDlg*)member3window);
	GAMEIN->GetPartyDialog()->RegistMemberDlg(3, (CPartyMemberDlg*)member4window);
	GAMEIN->GetPartyDialog()->RegistMemberDlg(4, (CPartyMemberDlg*)member5window);
	GAMEIN->GetPartyDialog()->RegistMemberDlg(5, (CPartyMemberDlg*)member6window);

	GAMEIN->GetPartyDialog()->RegistBtnDlg((CPartyBtnDlg*)partybtnwindow);

} 

void cWindowManager::CreatePartyCreateDlg()
{

#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/PartyCreate.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/PartyCreate.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/PartyCreate.txt");
#endif

	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetPartyCreateDialog((CPartyCreateDlg * )window);
	GAMEIN->GetPartyCreateDialog()->Linking();

}

void cWindowManager::CreatePartyInviteDlg()
{

#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/PartyInvite.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/PartyInvite.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/PartyInvite.txt");
#endif

	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetPartyInviteDialog((CPartyInviteDlg * )window);
	GAMEIN->GetPartyInviteDialog()->Linking();

}

void cWindowManager::CreateGuildCreateDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/GuildCreate.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GuildCreate.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/GuildCreate.txt");
#endif
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetGuildCreateDlg((CGuildCreateDialog * )window);
	GAMEIN->GetGuildCreateDlg()->Linking();
	
#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/GuildUnionCreate.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/GuildUnionCreate.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/GuildUnionCreate.txt");
#endif
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetGuildUnionCreateDlg((CGuildUnionCreateDialog * )window);
	GAMEIN->GetGuildUnionCreateDlg()->Linking();
}

void cWindowManager::CreateGuildDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/Guild.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Guild.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/Guild.txt");
#endif
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetGuildDlg((CGuildDialog * )window);
	GAMEIN->GetGuildDlg()->Linking();
}

void cWindowManager::CreateGuildInviteDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/GuildInvite.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GuildInvite.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/GuildInvite.txt");
#endif
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetGuildInviteDlg((CGuildInviteDialog * )window);
	GAMEIN->GetGuildInviteDlg()->Linking();
}

void cWindowManager::CreateGuildMarkDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/GuildMark.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GuildMark.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/GuildMark.txt");
#endif
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetGuildMarkDlg((CGuildMarkDialog * )window);
	GAMEIN->GetGuildMarkDlg()->Linking();
}

void cWindowManager::CreateGuildLevelupDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/GuildLevelUp.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GuildLevelUp.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/GuildLevelUp.txt");
#endif
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetGuildLevelUpDlg((CGuildLevelUpDialog * )window);
	GAMEIN->GetGuildLevelUpDlg()->Linking();
}

void cWindowManager::CreateGuildNickNameDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/GuildNickName.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GuildNickName.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/GuildNickName.txt");
#endif
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetGuildNickNameDlg((CGuildNickNameDialog * )window);
	GAMEIN->GetGuildNickNameDlg()->Linking();
}

void cWindowManager::CreateGuildRankDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/GuildRank.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GuildRank.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/GuildRank.txt");
#endif
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetGuildRankDlg((CGuildRankDialog * )window);
	GAMEIN->GetGuildRankDlg()->Linking();
}

void cWindowManager::CreateGuildWarehouseDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/GuildWarehouse.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GuildWarehouse.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/GuildWarehouse.txt");
#endif
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetGuildWarehouseDlg((CGuildWarehouseDialog * )window);
	GAMEIN->GetGuildWarehouseDlg()->Linking();
}

void cWindowManager::CreatePyogukDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/Pyoguk.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Pyoguk.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/Pyoguk.txt");
#endif
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetPyogukDialog((CPyogukDialog * )window);
	GAMEIN->GetPyogukDialog()->Linking();
}

void cWindowManager::CreateWantNpcDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/WantNpc.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/WantNpc.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/WantNpc.txt");
#endif
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetWantNpcDialog((CWantNpcDialog * )window);
	GAMEIN->GetWantNpcDialog()->Linking();
}

void cWindowManager::CreateWantRegistDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/WantRegist.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/WantRegist.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/WantRegist.txt");
#endif

	ASSERT(window);
	AddWindow(window);

	GAMEIN->SetWantRegistDialog((CWantRegistDialog * )window);
	GAMEIN->GetWantRegistDialog()->Linking();

}

void cWindowManager::CreateMiniMapDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/17.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/17.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/17.txt");
#endif

	ASSERT(window);
	AddWindow(window);

	GAMEIN->SetMiniMapDialog((CMiniMapDlg * )window);
	GAMEIN->GetMiniMapDialog()->Linking();
	GAMEIN->GetMiniMapDialog()->InitMiniMap(MAP->GetMapNum());
	
#ifdef _FILE_BIN_
//	cWindow * window2 = GetDlgInfoFromFile("./image/bigmap.bin", "rb");
	cWindow * window2 = GetDlgInfoFromFile("./image/InterfaceScript/bigmap.bin", "rb");
#else
	cWindow * window2 = GetDlgInfoFromFile("./image/bigmap.txt");
#endif

	ASSERT(window2);
	AddWindow(window2);

	GAMEIN->SetBigMapDialog((CBigMapDlg * )window2);
	if( GAMEIN->GetBigMapDialog() )
	{
		GAMEIN->GetBigMapDialog()->Linking();
		GAMEIN->GetBigMapDialog()->InitBigMap(MAP->GetMapNum());
	}
}

//KES
void cWindowManager::CreateMacroDlg()
{
#ifdef _FILE_BIN_
	cWindow* window = GetDlgInfoFromFile( "./image/16.bin", "rb" );
#else
	cWindow* window = GetDlgInfoFromFile( "./image/16.txt" );
#endif
	AddWindow( window );

	GAMEIN->SetMacroDialog( (CMacroDialog*)window );
	GAMEIN->GetMacroDialog()->Linking();

}
/*
void cWindowManager::CreateMacroModeDlg()
{
	cWindow* window = GetDlgInfoFromFile( "./image/GameIn/16/16-1.txt" );
	AddWindow( window );

	GAMEIN->SetMacroModeDialog( (CMacroModeDialog*)window );
	GAMEIN->GetMacroModeDialog()->Linking();
}
*/
/*
void cWindowManager::CreateChatOptionDlg()
{
#ifdef _FILE_BIN_
	cWindow * window = GetDlgInfoFromFile("./image/22-1.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/22-1.txt");
#endif
	AddWindow(window);
	GAMEIN->SetChatOptionDialog((CChatOptionDialog * )window);
	GAMEIN->GetChatOptionDialog()->Linking();
}
*/

void cWindowManager::CreateChatDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/22.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/22.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/22.txt");
#endif
	AddWindow(window);
	GAMEIN->SetChatDialog((CChatDialog * )window);
	GAMEIN->GetChatDialog()->Linking();
}

//SW050722 attach to minimap
//void cWindowManager::CreateJackpotDlg()
//{
//#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/Jackpot.bin", "rb");
//#else
//	cWindow * window = GetDlgInfoFromFile("./image/Jackpot.txt");
//#endif
//	AddWindow(window);
//	GAMEIN->SetJackpotDialog((cJackpotDialog * )window);
//	// !SW050715
//	GAMEIN->GetJackpotDialog()->Linking();
//}

void cWindowManager::CreateOptionDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/21.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/21.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/21.txt");
#endif
	AddWindow(window);
	GAMEIN->SetOptionDialog((COptionDialog * )window);
	GAMEIN->GetOptionDialog()->Linking();
}

void cWindowManager::CreateExitDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/25.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/25.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/25.txt");
#endif
	AddWindow(window);
	GAMEIN->SetExitDialog((CExitDialog * )window);
}

void cWindowManager::CreateCharStateDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/29.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/29.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/29.txt");
#endif
	AddWindow(window);
	GAMEIN->SetCharStateDialog((CCharStateDialog*)window);
	GAMEIN->GetCharStateDialog()->Linking();
}

void cWindowManager::CreateNoteDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/Note.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Note.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Note.txt");
#endif
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetNoteDialog((CNoteDialog * )window);
	GAMEIN->GetNoteDialog()->Linking();
}

void cWindowManager::CreateFriendDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/Friend.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Friend.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Friend.txt");
#endif
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetFriendDialog((CFriendDialog * )window);
	GAMEIN->GetFriendDialog()->Linking();
}


void cWindowManager::CreateMiniFriendDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/MiniFriend.bin","rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/MiniFriend.bin","rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/MiniFriend.txt");
#endif
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetMiniFriendDialog((CMiniFriendDialog * )window);
	GAMEIN->GetMiniFriendDialog()->Linking();
}

void cWindowManager::CreateMiniNoteDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/MiniNote.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/MiniNote.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/MiniNote.txt");
#endif
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetMiniNoteDialog((CMiniNoteDialog * )window);
	GAMEIN->GetMiniNoteDialog()->Linking();
}

void cWindowManager::CreateReviveDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/Revive.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Revive.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Revive.txt");
#endif
	
	ASSERT(window);
	AddWindow(window);
	
	GAMEIN->SetReviveDialog((CReviveDialog * )window);
	GAMEIN->GetReviveDialog()->Linking();
}
/////////////////////////




/*
void cWindowManager::CreateDebugdlg()
{
	cImage* DebugDlgImg = GetImageOfFile("./image/GameIn/DebugDlg/DebugDlg_256x160.tif");
	cImage* AttackBtnImgPress = GetImageOfFile("./image/GameIn/DebugDlg/DebugAttBtn_32x32.tif");
	cImage* ItemBtnImgPress = GetImageOfFile("./image/GameIn/DebugDlg/DebugItemBtn_32x32.tif");
	cImage* MoveBtnImgPress = GetImageOfFile("./image/GameIn/DebugDlg/DebugMoveBtn_32x32.tif");
	cImage* MugongBtnImgPress = GetImageOfFile("./image/GameIn/DebugDlg/DebugMugongBtn_32x32.tif");
	cImage* ChatBtnImgPress = GetImageOfFile("./image/GameIn/DebugDlg/DebugChatBtn_32x32.tif");
	cImage* UserConnBtnImgPress = GetImageOfFile("./image/GameIn/DebugDlg/DebugChatBtn_32x32.tif");

	cPushupButton* pushAttackBtn = new cPushupButton;
	pushAttackBtn->Init(0, 0, 32, 32, NULL, AttackBtnImgPress, DI_AttackBtnFunc, DI_DEBUGATTACK);
	cPushupButton* pushItemBtn = new cPushupButton;
	pushItemBtn->Init(32, 0, 32, 32, NULL, ItemBtnImgPress, DI_ItemBtnFunc, DI_DEBUGITEM);
	cPushupButton* pushMoveBtn = new cPushupButton;
	pushMoveBtn->Init(64, 0, 32, 32, NULL, MoveBtnImgPress, DI_MoveBtnFunc, DI_DEBUGMOVE);
	cPushupButton* pushMugongBtn = new cPushupButton;
	pushMugongBtn->Init(96, 0, 32, 32, NULL, MugongBtnImgPress, DI_MugongBtnFunc, DI_DEBUGMUGONG);
	cPushupButton* pushChatBtn = new cPushupButton;
	pushChatBtn->Init(128, 0, 32, 32, NULL, ChatBtnImgPress, DI_ChatBtnFunc, DI_DEBUGCHAT);
	cPushupButton* pushUserConnBtn = new cPushupButton;
	pushChatBtn->Init(160, 0, 32, 32, NULL, UserConnBtnImgPress, DI_UserConnBtnFunc, DI_DEBUGUSERCONN);

	CDebugDlg* list = new CDebugDlg;
	list->Init(0, 300, 256, NULL, 0, DebugDlgImg, 160, NULL, 9, 1, 960);//DI_DEBUGDLG);
	RECT textRect = {7, 26, 250, 135};
	list->InitList(50, &textRect);
	list->Add(pushAttackBtn);
	list->Add(pushItemBtn);
	list->Add(pushMoveBtn);
	list->Add(pushMugongBtn);
	list->Add(pushChatBtn);
	list->Add(pushUserConnBtn);

	list->SetActive(FALSE);
	GAMEIN->SetDebugDlg(list);
	AddWindow(list);
	
}
*/

void cWindowManager::CreateCharDlg()
{
#ifdef _JAPAN_LOCAL_
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/26.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/26.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/26.txt");
#endif

#else
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/24.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/24.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/24.txt");
#endif
#endif
	AddWindow(window);
	GAMEIN->SetCharacterDialog((CCharacterDialog * )window);
	GAMEIN->GetCharacterDialog()->Linking();
}
void cWindowManager::CreateMugongSuryunDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/10.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/10.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/10.txt");
#endif
	AddWindow(window);

	CMugongSuryunDialog * pDlg = (CMugongSuryunDialog * )window;
	GAMEIN->SetMugongSuryunDialog(pDlg);

	CMugongDialog* pMugongDlg = (CMugongDialog*)pDlg->GetTabSheet(0);
#ifdef _JAPAN_LOCAL_
	pMugongDlg->Linking_JP();
#elif defined _HK_LOCAL_
	pMugongDlg->Linking_JP();
#elif defined _TL_LOCAL_
	pMugongDlg->Linking_JP();
#else
	pMugongDlg->Linking();	
#endif //_JAPAN_LOCAL_
	
	GAMEIN->SetMugongDialog(pMugongDlg);
	MUGONGMGR->SetMugongDialog(pMugongDlg);

	CSuryunDialog* pSuryunDlg = (CSuryunDialog*)pDlg->GetTabSheet(1);
	GAMEIN->SetSuryunDialog(pSuryunDlg);
	pSuryunDlg->Linking();
}

void cWindowManager::CreateMPRegistDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/MPRegist.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/MPRegist.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/MPRegist.txt");
#endif
	AddWindow(window);

	GAMEIN->SetMPRegistDialog((CMPRegistDialog * )window);
	GAMEIN->GetMPRegistDialog()->Linking();
}

void cWindowManager::CreateMPMissionDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/MPMission.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/MPMission.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/MPMission.txt");
#endif
	AddWindow(window);

	GAMEIN->SetMPMissionDialog((CMPMissionDialog * )window);
	GAMEIN->GetMPMissionDialog()->Linking();
}

void cWindowManager::CreateMPNoticeDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/MPNotice.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/MPNotice.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/MPNotice.txt");
#endif
	AddWindow(window);

	GAMEIN->SetMPNoticeDialog((CMPNoticeDialog * )window);
	GAMEIN->GetMPNoticeDialog()->Linking();
}

void cWindowManager::CreateMPGuageDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/MPGuage.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/MPGuage.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/MPGuage.txt");
#endif
	AddWindow(window);

	GAMEIN->SetMPGuageDialog((CMPGuageDialog * )window);
	GAMEIN->GetMPGuageDialog()->Linking();
}

//SW051129 Pet
void cWindowManager::CreatePetStateDlg()
{
#ifdef _FILE_BIN_
//	cWindow* window = GetDlgInfoFromFile("./image/PetState.bin", "rb");
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/PetState.bin", "rb");
#else
	cWindow* window = GetDlgInfoFromFile("./image/PetState.txt", "rt");
#endif
	AddWindow(window);

	GAMEIN->SetPetStateDialog((CPetStateDlg* )window);
	GAMEIN->GetPetStateDialog()->Linking();
	PETMGR->SetPetStateDlg((CPetStateDlg*)window);
}

void cWindowManager::CreatePetStateMiniDlg()
{
#ifdef _FILE_BIN_
//	cWindow* window = GetDlgInfoFromFile("./image/PetStateMini.bin", "rb");
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/PetStateMini.bin", "rb");
#else
	cWindow* window = GetDlgInfoFromFile("./image/PetStateMini.txt", "rt");
#endif
	AddWindow(window);

	GAMEIN->SetPetStateMiniDialog((CPetStateMiniDlg*)window);
	GAMEIN->GetPetStateMiniDialog()->Linking();
	PETMGR->SetPetStateMiniDlg((CPetStateMiniDlg*)window);
}

void cWindowManager::CreatePetInvenDlg()
{
#ifdef _FILE_BIN_
//	cWindow* window = GetDlgInfoFromFile("./image/PetInven.bin", "rb");
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/PetInven.bin", "rb");
#else
	cWindow* window = GetDlgInfoFromFile("./image/PetInven.txt", "rt");
#endif
	AddWindow(window);

	GAMEIN->SetPetInventoryDialog((CPetInventoryDlg*)window);
	GAMEIN->GetPetInventoryDialog()->Linking();
	PETMGR->SetPetInventoryDlg((CPetInventoryDlg*)window);
}

void cWindowManager::CreatePetUpgradeDlg()
{
#ifdef _FILE_BIN_
//	cWindow* window = GetDlgInfoFromFile("./image/PetUpgrade.bin", "rb");
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/PetUpgrade.bin", "rb");
#else
	cWindow* window = GetDlgInfoFromFile("./image/PetUpgrade.txt", "rt");
#endif
	AddWindow(window);

	GAMEIN->SetPetUpgradeDialog((CPetUpgradeDialog*)window);
	GAMEIN->GetPetUpgradeDialog()->Linking();
}

void cWindowManager::CreatePetRevivalDlg()
{
#ifdef _FILE_BIN_
//	cWindow* window = GetDlgInfoFromFile("./image/PetRevival.bin", "rb");
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/PetRevival.bin", "rb");
#else
	cWindow* window = GetDlgInfoFromFile("./image/PetRevival.txt", "rt");
#endif
	AddWindow(window);

	GAMEIN->SetPetRevivalDialog((CPetRevivalDialog*)window);
	GAMEIN->GetPetRevivalDialog()->Linking();
}

void cWindowManager::CreateInventoryDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/11.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/11.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/11.txt");
#endif
	AddWindow(window);

	GAMEIN->SetInventoryDialog((CInventoryExDialog * )window);
	GAMEIN->GetInventoryDialog()->Linking();
}


//KES DIALOG 030918 //EXCHANGEDLG 
void cWindowManager::CreateExchangeDlg()
{
#ifdef _FILE_BIN_
//	cWindow* window = GetDlgInfoFromFile( "./image/28.bin", "rb" );
	cWindow* window = GetDlgInfoFromFile( "./image/InterfaceScript/28.bin", "rb" );
#else
	cWindow* window = GetDlgInfoFromFile( "./image/28.txt" );
#endif
	AddWindow(window);
	GAMEIN->SetExchangeDialog( (CExchangeDialog*)window );
	GAMEIN->GetExchangeDialog()->Linking();
}


void cWindowManager::CreateRecvExchangeDlg()
{
	
}

// LYJ 051017 구입노점상 추가
void cWindowManager::CreateStallKindSelectDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/StallOption.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/StallOption.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/StallOption.txt");
#endif
	AddWindow(window);
	GAMEIN->SetStallKindSelectDialog((CStallKindSelectDlg * )window);
	GAMEIN->GetStallKindSelectDialog()->Linking();
}

void cWindowManager::CreateStreetBuyStallDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/30.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/30.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/30.txt");
#endif
	AddWindow(window);
	GAMEIN->SetStreetBuyStallDialog((CStreetBuyStall * )window);
	GAMEIN->GetStreetBuyStallDialog()->Linking();
}

void cWindowManager::CreateBuyRegDlg()
{

#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/32.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/32.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/32.txt");
#endif
	AddWindow(window);
	GAMEIN->SetBuyRegDialog((CBuyRegDialog * )window);
	GAMEIN->GetBuyRegDialog()->Linking();
}

//////////////////////////////////////////////////////////////////////////
// 노점상 검색 다이얼로그 생성 by Stiner(8)
void cWindowManager::CreateStallFindDlg()
{
#ifdef _FILE_BIN_
	//	cWindow * window = GetDlgInfoFromFile("./image/32.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/ItemSearch.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/StallFind.txt");
#endif
	AddWindow(window);
	GAMEIN->SetStallFindDlg((CStallFindDlg * )window);
	GAMEIN->GetStallFindDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////

//LBS 03.09.18
void cWindowManager::CreateStreetStallDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/27.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/27.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/27.txt");
#endif
	AddWindow(window);
	GAMEIN->SetStreetStallDialog((CStreetStall * )window);
	GAMEIN->GetStreetStallDialog()->Linking();
}

void cWindowManager::CreateNpcScriptDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/31.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/31.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/31.txt");
#endif
	AddWindow(window);
	GAMEIN->SetNpcScriptDialog((cNpcScriptDialog * )window);
	GAMEIN->GetNpcScriptDialog()->Linking();
}

void cWindowManager::CreateHelpDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/33.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/33.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/33.txt");
#endif
	AddWindow(window);
	GAMEIN->SetHelpDialog( (cHelpDialog*)window );
	GAMEIN->GetHelpDialog()->Linking();
}

void cWindowManager::CreateDealDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/19.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/19.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/19.txt");
#endif
	AddWindow(window);
	GAMEIN->SetDealDialog((CDealDialog * )window);
	GAMEIN->GetDealDialog()->Linking();

	CMHFile fp;
#ifdef _FILE_BIN_
	fp.Init("./Resource/DealItem.bin","rb");
#else
	fp.Init("./Resource/DealItem.txt","rt");
#endif
	GAMEIN->GetDealDialog()->LoadDealerItem(&fp);
	fp.Release();

}

void cWindowManager::CreateQuestTotalDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/QuestTotal.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/QuestTotal.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/QuestTotal.txt");
#endif
	AddWindow(window);

	CQuestTotalDialog * pDlg = (CQuestTotalDialog * )window;
	GAMEIN->SetQuestTotalDialog(pDlg);

	CQuestDialog* pQuestDlg = (CQuestDialog*)pDlg->GetTabSheet(0);
	GAMEIN->SetQuestDialog(pQuestDlg);
	pQuestDlg->Linking();

	CWantedDialog* pWantedDlg = (CWantedDialog*)pDlg->GetTabSheet(1);
	GAMEIN->SetWantedDialog(pWantedDlg);
	pWantedDlg->Linking();
	
	CJournalDialog* pJournalDlg = (CJournalDialog*)pDlg->GetTabSheet(2);
	GAMEIN->SetJournalDialog(pJournalDlg);
	pJournalDlg->Linking();
}

void cWindowManager::CreateBailDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/bail.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/bail.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/bail.txt");
#endif
	AddWindow(window);

	CBailDialog* pDlg = (CBailDialog*)window;
	GAMEIN->SetBailDialog(pDlg);
	pDlg->Linking();
}

void cWindowManager::CreateEventNotifyDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/GMOpenTalk.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GMOpenTalk.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/GMOpenTalk.txt");
#endif
	AddWindow(window);

	CEventNotifyDialog* pDlg = (CEventNotifyDialog*)window;
	GAMEIN->SetEventNotifyDialog(pDlg);
	pDlg->Linking();
}

void cWindowManager::CreateMallNotcieDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/Itemmall.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Itemmall.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/Itemmall.txt");
#endif
	AddWindow(window);

	CMallNoticeDialog* pDlg = (CMallNoticeDialog*)window;
	GAMEIN->SetMallNoticeDialog(pDlg);
//	pDlg->Linking();
}

void cWindowManager::CreateDissolutionDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile( "./image/58.bin", "rb" );
	cWindow * window = GetDlgInfoFromFile( "./image/InterfaceScript/58.bin", "rb" );
#else
	cWindow * window = GetDlgInfoFromFile( "./image/58.txt" );
#endif
	AddWindow(window);

	CDissolutionDialog* pDlg = (CDissolutionDialog*)window;
	GAMEIN->SetDissolutionDialog( pDlg );
	pDlg->Linking();
}

void cWindowManager::CreateItemShopDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/ItemShop.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/ItemShop.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/ItemShop.txt");
#endif
	AddWindow(window);

	CItemShopDialog* pDlg = (CItemShopDialog*)window;
	GAMEIN->SetItemShopDialog(pDlg);
	if( pDlg )
		pDlg->Linking();
}


void cWindowManager::CreateMoveDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/SaveMove.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/SaveMove.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/SaveMove.txt");
#endif
	AddWindow(window);

	CMoveDialog* pDlg = (CMoveDialog*)window;
	GAMEIN->SetMoveDialog(pDlg);
	if( pDlg )
		pDlg->Linking();
}


void cWindowManager::CreatePointSaveDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/ChangeName.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/ChangeName.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/ChangeName.txt");
#endif
	AddWindow(window);

	CPointSaveDialog* pDlg = (CPointSaveDialog*)window;
	GAMEIN->SetPointSaveDialog(pDlg);
	if( pDlg )
		pDlg->Linking();
}

void cWindowManager::CreateSkPointDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/Skillpointagain.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Skillpointagain.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/Skillpointagain.txt");
#endif
	AddWindow(window);

	CSkillPointRedist* pDlg = (CSkillPointRedist*)window;
	GAMEIN->SetSkPointDlg(pDlg);
	if( pDlg )
		pDlg->Linking();
}

void cWindowManager::CreateSkPointNotifyDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/Skillpointop.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/Skillpointop.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/Skillpointop.txt");
#endif
	AddWindow(window);

	CSkillPointNotify* pDlg = (CSkillPointNotify*)window;
	GAMEIN->SetSkPointNotifydlg(pDlg);
	if( pDlg )
	{
		pDlg->Linking();
		pDlg->InitTextArea();
	}
}


void cWindowManager::CreateShoutDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/ch_allworldcreate.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/ch_allworldcreate.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/ch_allworldcreate.txt");
#endif
	AddWindow(window);

	CShoutDialog* pDlg = (CShoutDialog*)window;
	GAMEIN->SetShoutDlg(pDlg);
	pDlg->Linking();	
}


void cWindowManager::CreateShoutchatDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/ch_allworld.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/ch_allworld.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/ch_allworld.txt");
#endif
	AddWindow(window);

	CShoutchatDialog* pDlg = (CShoutchatDialog*)window;
	GAMEIN->SetShoutchatDlg(pDlg);
	pDlg->Linking();
//#ifdef _JAPAN_LOCAL_	//임시 아이템몰 들어가기 전까지
//	pDlg->SetActive(FALSE);
//#endif
}


void cWindowManager::CreateChaseDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/seechase.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/seechase.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/seechase.txt");
#endif
	AddWindow(window);

	CChaseDialog* pDlg = (CChaseDialog*)window;
	GAMEIN->SetChaseDlg(pDlg);
	pDlg->Linking();
}


void cWindowManager::CreateChaseinputDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/chase.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/chase.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/chase.txt");
#endif
	AddWindow(window);

	CChaseinputDialog* pDlg = (CChaseinputDialog*)window;
	GAMEIN->SetChaseinputDlg(pDlg);
	pDlg->Linking();
}

void cWindowManager::CreateNameChangeDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/chnamechange.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/chnamechange.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/chnamechange.txt");
#endif
	AddWindow(window);

	CNameChangeDialog* pDlg = (CNameChangeDialog*)window;
	GAMEIN->SetNameChangeDlg(pDlg);
	pDlg->Linking();
}


void cWindowManager::CreateNameChangeNotifyDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/opchnamechange.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/opchnamechange.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/opchnamechange.txt");
#endif
	AddWindow(window);

	CNameChangeNotifyDlg* pDlg = (CNameChangeNotifyDlg*)window;
	GAMEIN->SetNameChangeNotifyDlg(pDlg);
}


void cWindowManager::CreateGTRegistDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/GDTENTRY.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GDTENTRY.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/GDTENTRY.txt");
#endif
	AddWindow(window);

	CGTRegistDialog* pDlg = (CGTRegistDialog*)window;
	GAMEIN->SetGTRegistDlg(pDlg);
	pDlg->Linking();
}


void cWindowManager::CreateGTRegistcancelDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/GDTCANCEL.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GDTCANCEL.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/GDTCANCEL.txt");
#endif
	AddWindow(window);

	CGTRegistcancelDialog* pDlg = (CGTRegistcancelDialog*)window;
	GAMEIN->SetGTRegistcancelDlg(pDlg);
	pDlg->Linking();
}


void cWindowManager::CreateGTStandingDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/GDTournament.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GDTournament.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/GDTournament.txt");
#endif
	AddWindow(window);

	CGTStandingDialog* pDlg = (CGTStandingDialog*)window;
	GAMEIN->SetGTStandingDlg(pDlg);
	pDlg->Linking();
}


void cWindowManager::CreateGTBattleListDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/GDViewTournament.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/GDViewTournament.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/GDViewTournament.txt");
#endif
	AddWindow(window);

	CGTBattleListDialog* pDlg = (CGTBattleListDialog*)window;
	GAMEIN->SetGTBattleListDlg(pDlg);
	pDlg->Linking();
}


void cWindowManager::CreateGTScoreInfoDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/EventMapCount.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/EventMapCount.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/EventMapCount.txt");
#endif
	AddWindow(window);

	CGTScoreInfoDialog* pDlg = (CGTScoreInfoDialog*)window;
	GAMEIN->SetGTScoreInfoDlg(pDlg);
	pDlg->Linking();
}

void cWindowManager::CreateSeigeWarDlg()
{
	cWindow* window = NULL;
#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/SWTimeReg.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/SWTimeReg.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/SWTimeReg.txt");
#endif
	AddWindow(window);

	CSWTimeRegDlg* pTimeRegDlg = (CSWTimeRegDlg*)window;
	GAMEIN->SetSWTimeRegDlg(pTimeRegDlg);
	pTimeRegDlg->Linking();

#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/SWProtectReg.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/SWProtectReg.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/SWProtectReg.txt");
#endif
	AddWindow(window);

	CSWProtectRegDlg* pProRegDlg = (CSWProtectRegDlg*)window;
	GAMEIN->SetSWProtectRegDlg(pProRegDlg);
	pProRegDlg->Linking();

#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/SWInfo.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/SWInfo.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/SWInfo.txt");
#endif
	AddWindow(window);

	CSWInfoDlg* pInfoDlg = (CSWInfoDlg*)window;
	GAMEIN->SetSWInfoDlg(pInfoDlg);
	pInfoDlg->Linking();
	
#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/SWProfit.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/SWProfit.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/SWProfit.txt");
#endif
	AddWindow(window);

	CSWProfitDlg* pProfitDlg = (CSWProfitDlg*)window;
	GAMEIN->SetSWProfitDlg(pProfitDlg);
	pProfitDlg->Linking();	

#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/Engrave.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/Engrave.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/Engrave.txt");
#endif
	AddWindow(window);

	CSWEngraveDialog* pEngraveDlg = (CSWEngraveDialog*)window;
	GAMEIN->SetSWEngraveDlg(pEngraveDlg);
	pEngraveDlg->Linking();	


#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/SWCount.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/SWCount.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/SWCount.txt");
#endif
	AddWindow(window);

	CSWTimeDialog* pSWTimeDlg = (CSWTimeDialog*)window;
	GAMEIN->SetSWTimeDlg(pSWTimeDlg);
	pSWTimeDlg->Linking();	


#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/SWStartTime.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/SWStartTime.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/SWStartTime.txt");
#endif
	AddWindow(window);

	CSWStartTimeDialog* pSTDlg = (CSWStartTimeDialog*)window;
	GAMEIN->SetSWStartTimeDlg(pSTDlg);
	pSTDlg->Linking();	

	// 요새전 관련 인터페이스
#ifdef _FILE_BIN_
	window = GetDlgInfoFromFile("./image/InterfaceScript/FortWarEngraveDlg.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/InterfaceScript/FortWarEngraveDlg.txt");
#endif
	AddWindow(window);

	CFWEngraveDialog* pFWEngraveDlg = (CFWEngraveDialog*)window;
	GAMEIN->SetFWEngraveDlg(pFWEngraveDlg);
	pFWEngraveDlg->Linking();

#ifdef _FILE_BIN_
	window = GetDlgInfoFromFile("./image/InterfaceScript/FortWarTimeDlg.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/InterfaceScript/FortWarTimeDlg.txt");
#endif
	AddWindow(window);

	CFWTimeDialog* pFWTimeDlg = (CFWTimeDialog*)window;
	GAMEIN->SetFWTimeDlg(pFWTimeDlg);
	pFWTimeDlg->Linking();

#ifdef _FILE_BIN_
	window = GetDlgInfoFromFile("./image/InterfaceScript/FortWarWareHouseDlg.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/InterfaceScript/FortWarWareHouseDlg.txt");
#endif
	AddWindow(window);

	CFWWareHouseDialog* pFWWareHouseDlg = (CFWWareHouseDialog*)window;
	GAMEIN->SetFWWareHouseDlg(pFWWareHouseDlg);
	pFWWareHouseDlg->Linking();
}

void cWindowManager::CreateCharChangeDlg()
{
	cWindow* window = NULL;

#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/CharChange.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/CharChange.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/CharChange.txt");
#endif
	AddWindow(window);

	CCharChangeDlg* pDlg = (CCharChangeDlg*)window;
	GAMEIN->SetCharChangeDlg( pDlg );
	pDlg->Linking();
}

void cWindowManager::CreateItemSealDlg()
{
	cWindow* window = NULL;

#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/ItemLock.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/ItemLock.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/ItemLock.txt");
#endif
	AddWindow(window);

	CSealDialog* pDlg = (CSealDialog*)window;
	GAMEIN->SetSealDlg( pDlg );
	pDlg->Linking();
}

void cWindowManager::CreateChangeJobDlg()
{
	cWindow* window = NULL;

#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/Changejob.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/Changejob.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/Changejob.txt");
#endif
	AddWindow(window);

	CChangeJobDialog* pDlg = (CChangeJobDialog*)window;
	GAMEIN->SetChangeJobDlg( pDlg );
}

void cWindowManager::CreateReinforceResetDlg()
{
	cWindow* window = NULL;

#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/ReinforceDefault.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/ReinforceDefault.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/ReinforceDefault.txt");
#endif
	AddWindow(window);

	if( window )
	{
		CReinforceResetDlg* pDlg = (CReinforceResetDlg*)window;
		pDlg->Linking();
		GAMEIN->SetReinforceResetDlg( pDlg );
	}
}

void cWindowManager::CreateRareCreateDlg()
{
	cWindow* window = NULL;

#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/RareCreate.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/RareCreate.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/RareCreate.txt");
#endif
	AddWindow(window);

	CRareCreateDialog* pDlg = (CRareCreateDialog*)window;
	pDlg->Linking();
	GAMEIN->SetRareCreateDialog( pDlg );
}

// 06. 02. 강화 재료 안내 인터페이스 추가 - 이영준
void cWindowManager::CreateReinforceDataGuideDlg()
{
	cWindow* window = NULL;

#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/RFDataGuide.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/RFDataGuide.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/RFDataGuide.txt");
#endif
	AddWindow(window);

	CReinforceDataGuideDlg* pDlg = (CReinforceDataGuideDlg*)window;
	pDlg->Linking();
	GAMEIN->SetReinforceDataGuideDlg( pDlg );
}

// 06. 02. 내정보창 팁보기 추가 - 이영준
void cWindowManager::CreateTipBrowserDlgDlg()
{
	cWindow* window = NULL;

#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/TipBrowser.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/TipBrowser.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/TipBrowser.txt");
#endif
	AddWindow(window);

	CTipBrowserDlg* pDlg = (CTipBrowserDlg*)window;
	pDlg->Linking();
	GAMEIN->SetTipBrowserDlg( pDlg );
}

void cWindowManager::CreateKeySettingTipDlg()
{
	cWindow* window = NULL;

#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/KeySetting.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/KeySetting.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/KeySetting.txt");
#endif
	AddWindow(window);

	CKeySettingTipDlg* pDlg = (CKeySettingTipDlg*)window;
	pDlg->Linking();
	GAMEIN->SetKeySettingTipDlg( pDlg );
}

void cWindowManager::CreateGuildNoteDlg()
{
	cWindow* window = NULL;

#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/GuildNote.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/GuildNote.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/GuildNote.txt");
#endif
	AddWindow(window);

	CGuildNoteDlg* pDlg = (CGuildNoteDlg*)window;
	pDlg->Linking();
	GAMEIN->SetGuildNoteDlg( pDlg );
}

void cWindowManager::CreateUnionNoteDlg()
{
	cWindow* window = NULL;

#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/AllyNote.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/AllyNote.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/AllyNote.txt");
#endif
	AddWindow(window);

	CUnionNoteDlg* pDlg = (CUnionNoteDlg*)window;
	pDlg->Linking();
	GAMEIN->SetUnionNoteDlg( pDlg );
}

// 06. 03. 문파공지 - 이영준
void cWindowManager::CreateGuildNoticeDlg()
{
	cWindow* window = NULL;

#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/GuildNotice.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/GuildNotice.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/GuildNotice.txt");
#endif
	AddWindow(window);

	CGuildNoticeDlg* pDlg = (CGuildNoticeDlg*)window;
	pDlg->Linking();
	GAMEIN->SetGuildNoticeDlg( pDlg );
}

/////////////////////////////////////////////////////////////////////////
// 06. 06. 2차 전직 - 이영준
// 무공 변환
void cWindowManager::CreateSkillOptionChangeDlg()
{
	cWindow* window = NULL;

#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/SkillTrans.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/SkillTrans.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/SkillTrans.txt");
#endif
	AddWindow(window);

	CSkillOptionChangeDlg* pDlg = (CSkillOptionChangeDlg*)window;
	pDlg->Linking();
	GAMEIN->SetSkillOptionChangeDlg( pDlg );
}

void cWindowManager::CreateSkillOptionClearDlg()
{
	cWindow* window = NULL;

#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/TransDefault.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/TransDefault.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/TransDefault.txt");
#endif
	AddWindow(window);

	CSkillOptionClearDlg* pDlg = (CSkillOptionClearDlg*)window;
	pDlg->Linking();
	GAMEIN->SetSkillOptionClearDlg( pDlg );
}

//screenshot
void cWindowManager::ScreenShotDlg()
{
#ifdef _FILE_BIN_
//	cWindow * window = GetDlgInfoFromFile("./image/screenshotdlg.bin", "rb");
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/screenshotdlg.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/screenshotdlg.txt");
#endif
	AddWindow(window);

	CScreenShotDlg* pDlg = (CScreenShotDlg*)window;
	GAMEIN->SetScreenShotDlg(pDlg);
	pDlg->Linking();
}

// 2007. 10. 22. CBH - 유니크 아이탬 조합 게이지 다이얼로그 ////////////////////////////////
void cWindowManager::CreateUniqueItemMixProgressBarDlg()
{	
#ifdef _FILE_BIN_	
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/UniqueItemProgressBarDlg.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/UniqueItemProgressBarDlg.txt");
#endif
	AddWindow(window);
	GAMEIN->SetUniqueItemMixProgressBarDlg((CUniqueItemMixProgressBarDlg * )window);
	GAMEIN->GetUniqueItemMixProgressBarDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////

// 2007. 10. 22. CBH - 타이탄 조합 게이지 다이얼로그 ////////////////////////////////
void cWindowManager::CreateTitanMixProgressBarDlg()
{	
#ifdef _FILE_BIN_	
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/TitanmixProgressBarDlg.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/TitanmixProgressBarDlg.txt");
#endif
	AddWindow(window);
	GAMEIN->SetTitanMixProgressBarDlg((CTitanMixProgressBarDlg * )window);
	GAMEIN->GetTitanMixProgressBarDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////

// 2007. 10. 22. CBH - 타이탄 조합 게이지 다이얼로그 ////////////////////////////////
void cWindowManager::CreateTitanPartsProgressBarDlg()
{	
#ifdef _FILE_BIN_	
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/TitanPartsProgressBarDlg.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/TitanPartsProgressBarDlg.txt");
#endif
	AddWindow(window);
	GAMEIN->SetTitanPartsProgressBarDlg((CTitanPartsProgressBarDlg * )window);
	GAMEIN->GetTitanPartsProgressBarDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////

// 2007. 12. 4. CBH - 스킨 선택 다이얼로그 ////////////////////////////////////////////////
void cWindowManager::CreateSkinSelectDlg()
{
#ifdef _FILE_BIN_	
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/SkinSelectDlg.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/SkinSelectDlg.txt");
#endif
	AddWindow(window);
	GAMEIN->SetSkinSelectDlg((CSkinSelectDialog * )window);
	GAMEIN->GetSkinSelectDlg()->Linking();
}
//////////////////////////////////////////////////////////////////////////

// 2008. 1. 16. CBH - 코스튬 스킨 선택 다이얼로그
void cWindowManager::CreateCostumeSkinSelectDlg()
{
#ifdef _FILE_BIN_	
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/CostumeSkinSelectDlg.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/CostumeSkinSelectDlg.txt");
#endif
	AddWindow(window);
	GAMEIN->SetCostumeSkinSelectDlg((CCostumeSkinSelectDialog * )window);
	GAMEIN->GetCostumeSkinSelectDlg()->Linking();
}
/////////////////////////////////////////////////////////////////////////

// magi82(42) - 샵아이템 추가(수련치 초기화)
void cWindowManager::CreateSkillPointResetDlg()
{
#ifdef _FILE_BIN_
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/SkillPointReset.bin", "rb");
#else
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/SkillPointReset.txt");
#endif
	AddWindow(window);
	GAMEIN->SetSkillPointResetDlg( (CSkillPointResetDlg*)window );
	GAMEIN->GetSkillPointResetDlg()->Linking();
}

// 2008. 3. 17. CBH - 보안번호 다이얼로그
void cWindowManager::CreateNumberPadDlg()
{
#ifdef _FILE_BIN_	
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/NumberPad.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/NumberPad.txt");
#endif
	AddWindow(window);
	GAMEIN->SetNumberPadDlg((CNumberPadDialog * )window);
	GAMEIN->GetNumberPadDlg()->Linking();
}
/////////////////////////////////////////////////////////////////////////

// magi82(47)
void cWindowManager::CreateStreetStallItemViewDlg()
{
#ifdef _FILE_BIN_
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/SearchChr.bin", "rb");
#else
	cWindow* window = GetDlgInfoFromFile("./image/InterfaceScript/SearchChr.txt");
#endif
	AddWindow(window);
	GAMEIN->SetStreetStallItemViewDlg( (CStreetStallItemViewDlg*)window );
	GAMEIN->GetStreetStallItemViewDlg()->Linking();
}

// 2008. 5. 23. CBH - 방파 매칭 다이얼로그
void cWindowManager::CreatePartyMatchingDlg()
{
#ifdef _FILE_BIN_	
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/PartyMatchingDlg.bin", "rb");
#else
	cWindow * window = GetDlgInfoFromFile("./image/InterfaceScript/PartyMatchingDlg.txt");
#endif
	AddWindow(window);
	GAMEIN->SetPartyMatchingDlg((CPartyMatchingDlg * )window);
	GAMEIN->GetPartyMatchingDlg()->Linking();
}
/////////////////////////////////////////////////////////////////////////

/*
cWindow * cWindowManager::CreateAlertDlg(char * text, WORD wType, void (*cbFunc)(LONG lId, void * p, DWORD we), void * object)
{
	LONG AlertID = m_CommonDlgIDIndex.GenerateIndex();			// +500 시작 index
	CAlertDlg * alert = NULL;
	RECT closeRect={233,5,243,15};
	RECT captionRect={0,0,250,20};
	alert = new CAlertDlg;
	cImage* basicImage = GetImageOfFile("./image/GameIn/98/984911.tif", 98);
	cImage* pClosePressImage = GetImageOfFile("./image/GameIn/98/980113.tif", 98);
	alert->Init(250, 200, 250, 150, basicImage, AlertID);
	alert->SetcbFunc(_postDestroyCommonDlg);
	alert->SetcbBtn(cbFunc);
	alert->SetObj(object);
	alert->SetCaptionRect(&captionRect);
	switch(wType)
	{
	case AB_OKCANCEL:
		{
			cButton * okBtn = new cButton;
			cButton * cancelBtn = new cButton;
			cImage* btnBasicImageOk = GetImageOfFile("./image/GameIn/98/980312.tif", 98);
			cImage* btnPressImageOk = GetImageOfFile("./image/GameIn/98/980313.tif", 98);
			cImage* btnBasicImageCancel = GetImageOfFile("./image/GameIn/98/985012.tif", 98);
			cImage* btnPressImageCancel = GetImageOfFile("./image/GameIn/98/985013.tif", 98);
			okBtn->Init(61, 100, 40, 15, btnBasicImageOk, btnBasicImageOk, btnPressImageOk, NULL, -1);
			cancelBtn->Init(137, 100, 40, 15, btnBasicImageCancel, btnBasicImageCancel, btnPressImageCancel, NULL, -1);
			alert->Add(okBtn);
			alert->Add(cancelBtn);
		}
		break;
	case AB_YESNO:
		{
			cButton * yesBtn = new cButton;
			cButton * noBtn = new cButton;
			cImage* btnBasicImageYes = GetImageOfFile("./image/GameIn/98/980512.tif", 98);
			cImage* btnPressImageYes = GetImageOfFile("./image/GameIn/98/980513.tif", 98);
			cImage* btnBasicImageNo = GetImageOfFile("./image/GameIn/98/980412.tif", 98);
			cImage* btnPressImageNo = GetImageOfFile("./image/GameIn/98/980413.tif", 98);
			yesBtn->Init(61, 100, 40, 15, btnBasicImageYes, btnBasicImageYes, btnPressImageYes, NULL, -1);
			noBtn->Init(137, 100, 40, 15, btnBasicImageNo, btnBasicImageNo, btnPressImageNo, NULL, -1);
			alert->Add(yesBtn);
			alert->Add(noBtn);
		}
		break;
	}
	// taiyo edit
	cStatic * pText = new cStatic;
	pText->Init(30, 30, 0, 0, NULL, -1);
	pText->SetMultiLine(TRUE);
	pText->InitMultiLine();
	pText->SetTextXY(0,0);
	pText->SetFGColor(RGB_HALF(0,0,0));

	alert->Add(pText);
	AddWindow(alert);
	return (cWindow *)alert;
}
*/
/*
cWindow * cWindowManager::CreateMoneyDlg()
{
	
		LONG ID = m_CommonDlgIDIndex.GenerateIndex();			// +500 시작 index
	
		CMoneyDlg * mDlg = new CMoneyDlg;
		//RECT closeRect={163, 0, 172, 9};
		RECT captionRect={0, 0, 163, 10};
		cImage* pBasicImage = GetImageOfFile("./image/GameIn/98/983011.tif", 98);
		cImage* pClosePressImage = GetImageOfFile("./image/GameIn/98/980113.tif", 98);
		mDlg->Init(400, 300, 173, 40, pBasicImage, ID);
		mDlg->SetCaptionRect(&captionRect);
		mDlg->SetcbFunc(_postDestroyCommonDlg);
	
		cButton * btn = new cButton;
		//cImage* btnBasicImage = GetImageOfFile("./image/GameIn/98/980512.tif");
		//cImage* btnPressImage = GetImageOfFile("./image/GameIn/98/980413.tif");
		cImage* btnBasicImage = NULL;
		cImage* btnPressImage = NULL;
		btn->Init(128, 15, 40, 20, btnBasicImage, btnBasicImage, btnPressImage, NULL, -1);
		mDlg->Add(btn);
		
	
		cSpin * spin = new cSpin;
		spin->Init(30, 15, 93, 20, NULL, CMI_MoneySpinFunc, -1);
		spin->InitEditbox(83, 10);
		spin->SetMinMax(0, 99999999);
		cButton * spinUpBtn = new cButton;
		spinUpBtn->Init(83, 0, 10, 10, NULL, NULL, NULL, NULL, -1);
		cButton * spinDownBtn = new cButton;
		spinDownBtn->Init(83, 10, 10, 10, NULL, NULL, NULL, NULL, -1);
		spin->Add(spinUpBtn);
		spin->Add(spinDownBtn);
	
		mDlg->Add(spin);
	
		AddWindow(mDlg);
		return mDlg;
	
	return NULL;
}
*/

void cWindowManager::AfterInit()
{
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cDialog * win = (cDialog *)m_pWindowList->GetNext(pos);
		if(win->GetType() & WT_ICONDIALOG || win->GetType() & WT_ICONGRIDDIALOG)
			m_pIconDlgListRef->AddTail(win);
		else if(win->GetType() & WT_TABDIALOG)
		{
			cTabDialog * tabDlg = (cTabDialog *)win;
			int i = 0;
			while(i < tabDlg->GetTabNum())
			{
				if(tabDlg->GetTabSheet(i)->GetType() & WT_ICONDIALOG || tabDlg->GetTabSheet(i)->GetType() & WT_ICONGRIDDIALOG)
					m_pIconDlgListRef->AddTail(tabDlg->GetTabSheet(i));
				i++;
			}
		}
		PTRLISTPOS pos2 = win->m_pComponentList->GetHeadPosition();
		while(pos2)
		{
			cWindow * win2 = (cWindow *)win->m_pComponentList->GetNext(pos2);
			if(win2->GetType() == WT_EDITBOX)
			{
				m_pEditBoxListRef->AddTail((void *)win2);
			}
			else if(win2->GetType() & WT_ICONDIALOG || win2->GetType() & WT_ICONGRIDDIALOG)
			{
				m_pIconDlgListRef->AddTail(win2);
			}
		}
	}
}

////////////////////////////////////////////////////
//CUserInput에서 Key Input이 들어왔을 시에 호출해 줌
void cWindowManager::KeyboardInput( CKeyboard* keyInfo )
{
	PTRLISTPOS pos = m_pWindowList->GetTailPosition();
	cWindow * window;
	DWORD Action = WE_NULL;

	while(pos)
	{
		window = (cWindow *)m_pWindowList->GetAt(pos);

		if(window->IsActive() && !window->IsDepend() )
		{
			Action = window->ActionKeyboardEvent( keyInfo );

			if( Action & WE_CLOSEWINDOW )
				window->SetActive(FALSE);

			if( Action != WE_NULL ) break;	//키인풋이 처리되면 중지
		}

		m_pWindowList->GetPrev(pos);
	}


//////////////////////////
//TAB : EditBox Focus 이동
/*
	if( m_pFocusedEdit && keyInfo->GetKeyDown(KEY_TAB) )
	{
		cEditBox * win;
		PTRLISTPOS posCur = m_pEditBoxListRef->Find( m_pFocusedEdit );
		PTRLISTPOS pos = posCur;

		m_pEditBoxListRef->GetNext(pos);
		
		while( pos != posCur )
		{
			if( pos == NULL )
				pos = m_pEditBoxListRef->GetHeadPosition();

			win = (cEditBox *)m_pEditBoxListRef->GetNext(pos);
			
			if( win->IsActive() && win->GetParent() == m_pFocusedEdit->GetParent() )
			{
				m_pFocusedEdit->SetFocus( FALSE );
				win->SetFocusEdit( TRUE );
				break;
			}
		}
	}
*/
}

void cWindowManager::SetNextEditFocus()
{
	if( m_pFocusedEdit )
	{
		cEditBox * win;
		PTRLISTPOS posCur = m_pEditBoxListRef->Find( m_pFocusedEdit );
		PTRLISTPOS pos = posCur;

		m_pEditBoxListRef->GetNext(pos);
		
		while( pos != posCur )
		{
			if( pos == NULL )
				pos = m_pEditBoxListRef->GetHeadPosition();

			win = (cEditBox *)m_pEditBoxListRef->GetNext(pos);
			
			if( win->IsActive() && win->GetParent() == m_pFocusedEdit->GetParent() )
			{
				m_pFocusedEdit->SetFocus( FALSE );
				win->SetFocusEdit( TRUE );
				break;
			}
		}
	}
}

void cWindowManager::BackDragWindow()
{
	cDialog * srcDlg = GetWindowForID(m_id);
	if(!srcDlg) return;
	srcDlg->SetAbsXY((LONG)m_posBeforeDrag.x, (LONG)m_posBeforeDrag.y);	// SetDragEnd()가 있는데 필요한가???체크필

//	if( !m_pDragDialog ) return;
//	m_pDragDialog->SetAbsXY((LONG)m_posBeforeDrag.x, (LONG)m_posBeforeDrag.y);	// SetDragEnd()가 있는데 필요한가???체크필
	DragWindowNull();
}

void cWindowManager::Preprocess()
{
	if( m_OldDragFlag )
	{
		BOOL LeftUp = MOUSE->LButtonUp();
		
		if( LeftUp )
		{
			LONG ClickUpX = MOUSE->GetMouseEventX();
			LONG ClickUpY = MOUSE->GetMouseEventY();

			if( cbDragnDropProcess )
				cbDragnDropProcess( ClickUpX, ClickUpY, m_id, (LONG)m_posBeforeDrag.x, (LONG)m_posBeforeDrag.y);

			SetDragEnd();
		}
		else if( m_pDragDialog )
		{
			LONG x = MOUSE->GetMouseX();
			LONG y = MOUSE->GetMouseY();

//			cDialog * dlg = GetWindowForID( m_id );
//			if( dlg == NULL )
//				return;

			m_pDragDialog->SetAbsXY( x-m_OldX, y-m_OldY );
			RECT rect = { 0, 0, 1024, 768 };		//해상도 변경가능한가? confirm
			rect.right = GET_MAINWIN_W;
			rect.bottom = GET_MAINWIN_H;
			VECTOR2 magPos;
			magPos.x = m_pDragDialog->GetAbsX();
			magPos.y = m_pDragDialog->GetAbsY();
			if( cAnimationManager::MagProcess( &rect, (cWindow*)m_pDragDialog, &magPos ) )
				m_pDragDialog->SetAbsXY( magPos.x, magPos.y );
		}
	}
}

void cWindowManager::AddListDestroyWindow(cWindow * window)
{
	if( window == NULL ) return;

	// yh DestroyList에 같은 윈도우가 2번 들어가 있어서 다운 [4/3/2004]
	PTRLISTSEARCHSTART((*m_pDestroyWindowRef),cWindow*,pWindow)
		if(pWindow == window)
			return;
	PTRLISTSEARCHEND

	m_pDestroyWindowRef->AddTail(window);
	window->SetDestroy();
}

void cWindowManager::DestroyWindowProcess()
{
	PTRLISTPOS pos = m_pDestroyWindowRef->GetHeadPosition();
	while(pos)
	{
		cWindow * window = (cWindow *)m_pDestroyWindowRef->GetNext(pos);
		
		//DeleteWindowForID(window->GetID());
		DeleteWindow( window );
	}
	m_pDestroyWindowRef->RemoveAll();
}

void cWindowManager::OnWindowEvent(LONG lId, void * p, DWORD we)
{
	if( we & WE_CLOSEWINDOW )
	{
		GAMEIN->GetMainInterfaceDialog()->PushMenuID( lId, FALSE );
	}
}

void cWindowManager::Process()
{
	ANIMGR->FadeProcess();
 
	// 말 풍선
	PTRLISTPOS pos = m_pPlayerChatTooltip->GetHeadPosition();
	while(pos)
	{
		cWindow * window = (cWindow *)m_pPlayerChatTooltip->GetAt(pos);
		window->ActionEvent( MOUSE );
		
		if( !window->IsActive() )
			m_pPlayerChatTooltip->RemoveAt(pos);

		m_pPlayerChatTooltip->GetNext(pos);
	}
}

void cWindowManager::MouseInput( CMouse* pMouse )
{
//KES INTERFACE 031008
	m_bMouseOverUsed	= FALSE;
	m_bMouseDownUsed	= FALSE;
	m_bMouseUpUsed		= FALSE;
//	m_bKeyInputUsed		= FALSE;
	cWindow* pMouseOverWindow = NULL;
//
	m_MouseInputProcessed = FALSE;
	DestroyWindowProcess();
	Preprocess();
	
	cWindow * TopWindow = NULL;
	PTRLISTPOS TopPos = NULL;
	PTRLISTPOS pos = m_pWindowList->GetTailPosition();
	cWindow * window;
	
	while(pos)
	{
		window = (cWindow *)m_pWindowList->GetAt(pos);
////
		LONG Action=0;
		if( window->IsActive() )
		{
			if( !window->IsDepend() ) //KES INTERFACE 031010 //depend인것은 iconGridDialog가 한다.
				Action = window->ActionEvent( pMouse );
			//if(cbProcess)
				//cbProcess();
			
		}
		if((Action & WE_CLOSEWINDOW))
			window->SetActive(FALSE);
		else if(Action & WE_TOPWINDOW)
		{
			TopPos = pos;
			TopWindow = window;
		}
		if( Action & WE_LBTNCLICK || Action & WE_RBTNCLICK || 
			Action & WE_LBTNDBLCLICK || Action & WE_RBTNDBLCLICK)
		{
			m_MouseInputProcessed	= TRUE;
			m_bMouseDownUsed		= TRUE;
		}
		if( Action & WE_MOUSEOVER )
		{
			m_MouseInputProcessed	= TRUE;
			m_bMouseOverUsed		= TRUE;
			pMouseOverWindow		= window;
		}
//		if( Action & WE_RETURN )
//		{
//			m_bKeyInputUsed = TRUE;
//		}

		m_pWindowList->GetPrev(pos);
	}

	if(TopWindow && TopPos)
	{
		m_pWindowList->RemoveAt(TopPos);			//중간에서 빼고
		
		if( TopWindow->IsAlwaysTop() )
			m_pWindowList->AddTail(TopWindow);		//끝에 넣고
		else
		{
			pos = m_pWindowList->GetTailPosition();		//메세지박스를 젤 위에 두기 위해
			
			while( pos )
			{
				cWindow* pWindow = (cWindow*)m_pWindowList->GetAt(pos);
				if( pWindow )
				if( !pWindow->IsAlwaysTop() )	break;
				m_pWindowList->GetPrev( pos );
			}

			if( pos )
				m_pWindowList->InsertAfter( pos, TopWindow );
			else 
				m_pWindowList->AddHead( TopWindow );
		}
	}
	
	if(m_pGuageWindow)
	{
		m_pGuageWindow->ActionEvent(pMouse);
	}
	if(m_pMonsterGuageWindow)
	{
		m_pMonsterGuageWindow->ActionEvent(pMouse);
	}
	if(m_pMousePointWindow)
	{
		m_pMousePointWindow->SetAbsXY(pMouse->GetMouseX(), pMouse->GetMouseY());
//		m_pMousePointWindow->ActionEvent(pMouse);
	}
/*
	if( pMouseOverWindow )
	{
		if( pMouseOverWindow->GetType() & WT_ICON )
			CURSOR->SetCursor( eCURSOR_ITEMSELECT );
		else
			CURSOR->SetCursor( eCURSOR_DEFAULT );
	}
*/
}


void cWindowManager::Render()
{
	PTRLISTPOS pos = m_pScreenTextList->GetHeadPosition();
	while(pos)
	{
		cMultiLineText* mlwindow = (cMultiLineText*)m_pScreenTextList->GetNext(pos);
		mlwindow->Render();
	}

	// 말 풍선
	// taiyo : ObjectID를 셋팅하기 때문에 문제가 되지 않는가???
	pos = m_pPlayerChatTooltip->GetHeadPosition();
	cWindow* window;

	while(pos)
	{
		cWindow* window = (cWindow*)m_pPlayerChatTooltip->GetNext(pos);
		window->Render();
	}

	SetToolTipWindow( NULL ); //NULL로 하고, 아래 윈도 랜더할때 윈도들이 세팅한다.

	pos = m_pWindowList->GetHeadPosition();
//	pGeometry->BeginRender(0,0,DONOT_CLEAR_FRAMEBUFFER|DONOT_CLEAR_ZBUFFER);


//드래그 중인 dialog를 맨위로.. 
//AlwaysTop과 사이에 넣자.... 완전 하드코딩 KES

//	cDialog* dragDlg = GetWindowForID( m_id );	//getwindowforid find all files

	//drag중인 icon이 있을 때는 allwaystop이 나올때 까지 맨 나중에 찍어주어 위로 올라오도록 한다.

	STATUSICONDLG->Render();

	if( m_OldDragFlag )
	{
		BOOL bRenderDragDlg = FALSE;

		while(pos)
		{
			window = (cWindow *)m_pWindowList->GetNext(pos);

			if( window->IsAlwaysTop() )
			if( window->IsActive() )
			if( !bRenderDragDlg )
			if( !m_pDragDialog->IsAlwaysTop() )
			{
				m_pDragDialog->Render();
				bRenderDragDlg = TRUE;
			}

			if( !window->IsDepend() && window->GetID() != m_id )
				window->Render();
		}

		if( !bRenderDragDlg && m_pDragDialog)
			m_pDragDialog->Render();
	}
	else
	{
		while(pos)
		{
			window = (cWindow *)m_pWindowList->GetNext(pos);
			if( !window->IsDepend() )
				window->Render();
		}
	}

	
//////////////////
	if( !m_OldDragFlag )	//드래그중인 윈도우가 없을 때
	if( m_pToolTipWindow )
	{
		m_pToolTipWindow->Render();
	}
	
	if(m_pGuageWindow)
		m_pGuageWindow->Render();
	if(m_pMonsterGuageWindow)
		m_pMonsterGuageWindow->Render();
	if(m_pMousePointWindow)
		m_pMousePointWindow->Render();

	//SW061019 서바이벌모드
	//if( MAP->CheckMapKindIs(eSurvival) )
	if( MAP->IsMapKind(eSurvivalMap) )
	{
		SVVMODEMGR->Render();
	}
//	pGeometry->EndRender();
#ifdef _TESTCLIENT_
	extern g_bDisplayFPS;
	if(g_bDisplayFPS)
	{
		RECT rect;
		SetRect(&rect,30,100,500,150);
		char temp[256];
		VECTOR3 pos;
		HERO->GetPosition(&pos);
		sprintf(temp,"x: %f z : %f", pos.x, pos.z);
		CFONT_OBJ->RenderFont(0,temp,strlen(temp),&rect,0xffffffff);

		SetRect(&rect,30,200,500,250);
		MHCAMERADESC* pcam = CAMERA->GetCameraDesc();
		sprintf( temp, "cam_x: %f  cam_z: %f  cam_y: %f", pcam->m_CameraPos.x, pcam->m_CameraPos.z, pcam->m_CameraPos.y );
		CFONT_OBJ->RenderFont(0,temp,strlen(temp),&rect,0xffffffff);

		SetRect(&rect,30,300,500,350);
		sprintf( temp, "ang_deg_x: %f  ang_deg_: %f", CAMERA->GetCameraDesc()->m_AngleX.ToDeg(), CAMERA->GetCameraDesc()->m_AngleY.ToDeg() );
		CFONT_OBJ->RenderFont(0,temp,strlen(temp),&rect,0xffffffff);
	}
#endif

}

void cWindowManager::DestroyWindowAll()
{
	if(!m_pWindowList)return;

	DestroyWindowProcess();
	
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cWindow * win = (cWindow *)m_pWindowList->GetNext(pos);
		SAFE_DELETE(win);
	}
	m_pWindowList->RemoveAll();

	if(m_pGuageWindow)
	{
		SAFE_DELETE(m_pGuageWindow);
	}
	if(m_pMonsterGuageWindow)
	{
		SAFE_DELETE(m_pMonsterGuageWindow);
	}
	
	DeleteEditBoxListAll();
	DeleteIconDlgListAll();
	DeleteChatTooltipListAll();

	m_pDragDialog = NULL;
}
void cWindowManager::CloseWindowForID(LONG id)
{
	if(!m_pWindowList) return;
	
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cWindow * win = (cWindow *)m_pWindowList->GetNext(pos);
		if(win->GetID() == id)
		{
			win->SetActive(FALSE);
			break;
		}
	}
}
void cWindowManager::DeleteIconDlgListAll()
{
	if(!m_pIconDlgListRef)return;
	m_pIconDlgListRef->RemoveAll();
}
void cWindowManager::DeleteEditBoxListAll()
{
	if(!m_pEditBoxListRef)return;
	m_pEditBoxListRef->RemoveAll();
}
void cWindowManager::DeleteChatTooltipListAll()
{
	if(!m_pPlayerChatTooltip)return;
	m_pPlayerChatTooltip->RemoveAll();
}

void cWindowManager::DeleteWindowForID(LONG id)
{
	if(!m_pWindowList) return;

	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cWindow * win = (cWindow *)m_pWindowList->GetAt(pos);
		if( win->GetID() == id )
		{
			m_pWindowList->RemoveAt(pos);
			SAFE_DELETE(win);
			if( m_id == id ) SetDragEnd();	//KES추가
			return;
		}
		m_pWindowList->GetNext(pos);
	}
}


void cWindowManager::DeleteWindow( cWindow* pWindow )
{
	if(!m_pWindowList) return;

	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cWindow* win = (cWindow *)m_pWindowList->GetAt(pos);
		if( win == pWindow )
		{
			m_pWindowList->RemoveAt(pos);
			if( m_id == win->GetID() ) SetDragEnd();	//KES추가
			SAFE_DELETE(win);
			return;
		}
		m_pWindowList->GetNext(pos);
	}
}

void cWindowManager::RemoveChatTooltipWindow(LONG playerID)
{
	if(!m_pPlayerChatTooltip) return;
	PTRLISTPOS pos = m_pPlayerChatTooltip->GetHeadPosition();
	while(pos)
	{
		cWindow * win = (cWindow *)m_pPlayerChatTooltip->GetAt(pos);
		if(win->GetID() == playerID)
		{
			m_pPlayerChatTooltip->RemoveAt(pos);
			break;
		}
		m_pPlayerChatTooltip->GetNext(pos);
	}
	return ;
}

void cWindowManager::RemoveChatTooltipWindow( cWindow* pWindow )
{
	if(m_pPlayerChatTooltip)
		m_pPlayerChatTooltip->Remove( pWindow );
}

cDialog * cWindowManager::GetWindowForID(LONG id)
{

	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cDialog * win = (cDialog *)m_pWindowList->GetNext(pos);
		if(win->GetID() == id)
			return win;
	}
	return NULL;
}


cWindow * cWindowManager::GetWindowForIDEx(LONG id)
{
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cDialog * win = (cDialog *)m_pWindowList->GetNext(pos);
		if(win->GetID() == id)
			return (cWindow *)win;
		else
		{
			PTRLISTPOS pos2 = win->m_pComponentList->GetHeadPosition();
			while(pos2)
			{
				cWindow * win2 = (cWindow *)win->m_pComponentList->GetNext(pos2);
				if(win2->GetID() == id)
				{
					return win2;
				}
			}
		}
	}
	return NULL;
}

cDialog * cWindowManager::GetWindowForXY(LONG x, LONG y)
{
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cDialog * win = (cDialog *)m_pWindowList->GetNext(pos);
		if(win->IsActive() == FALSE)
			continue;
		if(win->PtInWindow(x,y))
			return win;
	}
	return NULL;
}
cDialog * cWindowManager::GetWindowForXYExceptIcon(LONG x, LONG y)
{
	
//	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	PTRLISTPOS pos = m_pWindowList->GetTailPosition();
	while(pos)
	{
//		cDialog * win = (cDialog *)m_pWindowList->GetNext(pos);
		cDialog * win = (cDialog *)m_pWindowList->GetPrev(pos);
		if(win->IsActive() == FALSE || win->GetType() & WT_ICON)
			continue;
		if(win->PtInWindow(x,y))
			return win;
	}
	return NULL;
}

cDialog * cWindowManager::GetWindowForXYExceptID(LONG x, LONG y, LONG id)
{
	//꼬리부터로...바꾸기
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cDialog * win = (cDialog *)m_pWindowList->GetNext(pos);
		if(win->GetID() == id || win->IsActive() == FALSE)
			continue;
		if(win->PtInWindow(x,y))
			return win;
	}
	return NULL;
}

cIconDialog * cWindowManager::GetIconDialogForXY(LONG x, LONG y)
{
	//꼬리부터로...바꾸기
	PTRLISTPOS pos = m_pIconDlgListRef->GetHeadPosition();
	while(pos)
	{
		cDialog * win = (cDialog *)m_pIconDlgListRef->GetNext(pos);
		if(win->IsActive() == FALSE)
			continue;
		if(win->PtInWindow(x,y))
			return (cIconDialog *)win;
	}
	return NULL;
}


//KES 030825
void cWindowManager::SetOptionAlpha( DWORD dwAlpha )
{
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while(pos)
	{
		cWindow * win = (cWindow *)m_pWindowList->GetNext(pos);
		win->SetOptionAlpha( dwAlpha );
	}
}


cMsgBox* cWindowManager::MsgBox( LONG lMBId, int nMBType, char* pStrMsg, ... )
{
	if( !cMsgBox::IsInit() ) return NULL;

	if( WINDOWMGR->GetWindowForID( lMBId ) != NULL )
		return NULL;
	
	cMsgBox* pMsgBox = new cMsgBox;

	char msg[256] = {0,};

	if( pStrMsg != NULL ) 
	{
		va_list argList;

		va_start(argList, pStrMsg);
		vsprintf(msg, pStrMsg, argList);
		va_end(argList);
	}

//	pMsgBox->MsgBox( lMBId, nMBType, pStrMsg, cbMBFunc );
	pMsgBox->MsgBox( lMBId, nMBType, msg, MessageBox_Func );
	pMsgBox->SetAlwaysTop( TRUE );	//메세지 박스는 항상위. 싫으면 밖으로 빼기.
	AddWindow( pMsgBox );

	LONG lX = (LONG)pMsgBox->GetAbsX();
	LONG lY = (LONG)pMsgBox->GetAbsY();
	PositioningDlg( lX, lY );
	pMsgBox->SetAbsXY( lX, lY );
	pMsgBox->SetActive( TRUE );

	return pMsgBox;
}

void cWindowManager::PositioningDlg( LONG& rX, LONG& rY )
{
	int nCount = 0;
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	while( pos )
	{
		cWindow* pWindow = (cWindow*)m_pWindowList->GetNext( pos );
		if( (LONG)pWindow->GetAbsX() == rX )
		if( (LONG)pWindow->GetAbsY() == rY )
		{
			rX += 20;
			rY += 20;
			if( ++nCount >= 10 )//한없이 내려가는것 방지
			{
				rX -= 200;
				rY -= 200;
				nCount = 0;
			}
		}
	}
}

cDivideBox * cWindowManager::DivideBox( LONG lId, LONG x, LONG y, cbDivideFUNC cbFc1, cbDivideFUNC cbFc2, void * vData1, void* vData2, char * strTitle )
{
	if( WINDOWMGR->GetWindowForID( lId ) != NULL )
		return NULL;

#ifdef _FILE_BIN_
//	cDivideBox * window = (cDivideBox * )GetDlgInfoFromFile("./image/DivideBox.bin", "rb" );
	cDivideBox * window = (cDivideBox * )GetDlgInfoFromFile("./image/InterfaceScript/DivideBox.bin", "rb" );
#else
	cDivideBox * window = (cDivideBox * )GetDlgInfoFromFile("./image/DivideBox.txt");
#endif
	
	window->CreateDivideBox( x, y, lId, cbFc1, cbFc2, vData1, vData2, strTitle );
	window->SetActive( TRUE );
	window->SetAlwaysTop( TRUE );

	AddWindow( window );

	return window;
}


void cWindowManager::SetWindowTop( cWindow* win )
{
	PTRLISTPOS pos = m_pWindowList->Find( win );

	if( pos == NULL ) return;

	m_pWindowList->RemoveAt( pos );
	
	if( win->IsAlwaysTop() )
		m_pWindowList->AddTail(win);						//끝에 넣고
	else
	{
		pos = m_pWindowList->GetTailPosition();		//메세지박스를 젤 위에 두기 위해
		
		while( pos )
		{
			cWindow* pWindow = (cWindow*)m_pWindowList->GetAt(pos);
			if( pWindow )
			if( !pWindow->IsAlwaysTop() )	break;

			m_pWindowList->GetPrev(pos);
		}
		
		if( pos )
			m_pWindowList->InsertAfter( pos, win );
		else 
			m_pWindowList->AddHead( win );
	}
}


///////////////////
//Tab키 처리를 위해	//focus를 줄때 안줄때
void cWindowManager::SetFocusedEdit( cWindow* pEdit, BOOL bFocus )
{
	if( bFocus )
	{
		if( m_pFocusedEdit == pEdit ) return;

#ifdef _HACK_SHIELD_
		HS_PauseService();
#endif

#ifdef TAIWAN_LOCAL
		HIMC hIMC = ImmGetContext( _g_hWnd );
		ImmSetOpenStatus( hIMC, TRUE );
		ImmReleaseContext( _g_hWnd, hIMC );

		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
#elif defined _HK_LOCAL_
		HIMC hIMC = ImmGetContext( _g_hWnd );
		ImmSetOpenStatus( hIMC, TRUE );
		ImmReleaseContext( _g_hWnd, hIMC );

		HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
		SendMessage( hWndIme, WM_IME_CONTROL, IMC_OPENSTATUSWINDOW, 0 );
#elif defined _JAPAN_LOCAL_	
		HIMC hIMC = ImmGetContext( _g_hWnd );

		if( m_pFocusedEdit == NULL && m_bOpenStatus )
		{
			ImmSetOpenStatus( hIMC, TRUE );
		}

		if( pEdit->GetType() == WT_EDITBOX || pEdit->GetType() == WT_SPIN )
		{
			if( ((cEditBox*)pEdit)->GetValidCheckMethod() == VCM_NUMBER )
			{
				m_bOpenStatus = ImmGetOpenStatus( hIMC );
				ImmSetOpenStatus( hIMC, FALSE );
			}
		}

		ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
		ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );

		ImmReleaseContext( _g_hWnd, hIMC );
#endif

		m_pFocusedEdit = pEdit;
	}
	else
	{

#ifndef _JAPAN_LOCAL_
		if( m_pFocusedEdit == pEdit )
		{
			m_pFocusedEdit = NULL;

#ifdef TAIWAN_LOCAL
			HIMC hIMC = ImmGetContext( _g_hWnd );
			ImmSetOpenStatus( hIMC, FALSE );
			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );

			ImmReleaseContext( _g_hWnd, hIMC );

			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
#endif
#ifdef _HK_LOCAL_
			HIMC hIMC = ImmGetContext( _g_hWnd );
			ImmSetOpenStatus( hIMC, FALSE );
			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );

			ImmReleaseContext( _g_hWnd, hIMC );

			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
#endif

#ifdef _HACK_SHIELD_
		HS_ResumeService();
#endif

		}
#else		//defined _JAPAN_LOCAL_
		
		HIMC hIMC = ImmGetContext( _g_hWnd );

		if( m_pFocusedEdit == pEdit )
		{
			m_pFocusedEdit = NULL;
			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );

			if( pEdit->GetType() == WT_EDITBOX || pEdit->GetType() == WT_SPIN )
			{
				if( ((cEditBox*)pEdit)->GetValidCheckMethod() != VCM_NUMBER )
				{
					m_bOpenStatus = ImmGetOpenStatus( hIMC );
				}

				ImmSetOpenStatus( hIMC, FALSE );
			}
			else
			{
				m_bOpenStatus = ImmGetOpenStatus( hIMC );
				ImmSetOpenStatus( hIMC, FALSE );
			}
		}
		else
		{
			if( pEdit->GetType() == WT_EDITBOX || pEdit->GetType() == WT_SPIN )
			{
				if( ((cEditBox*)pEdit)->GetValidCheckMethod() == VCM_NUMBER )
				{
					ImmSetOpenStatus( hIMC, m_bOpenStatus );
				}
			}
		}

		ImmReleaseContext( _g_hWnd, hIMC );
#endif

	}
}


BOOL cWindowManager::CloseAllAutoCloseWindows()
{
	PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
	cWindow* pWin;
	BOOL bClose = FALSE;

	//delete된 아이콘들이 아직 리스트에서 안없어졌을때 뻑이난다(?)
	while(pos)
	{
		pWin = (cWindow*)m_pWindowList->GetNext(pos);

		if( pWin->IsAutoClose() && pWin->IsActive() )
		{
			pWin->SetActive( FALSE );
			bClose = TRUE;
		}
	}

	return bClose;
}


cWindow* cWindowManager::GetDlgInfoFromFile (char * filePath, char* mode )
{
	return m_pScriptManager->GetDlgInfoFromFile(filePath, mode);
}


cMsgBox* cWindowManager::GetFirstMsgBox()
{
	PTRLISTPOS pos = m_pWindowList->GetTailPosition();
	cWindow * window;
	
	while(pos)
	{
//		window = (cWindow *)m_pWindowList->GetAt(pos);
//		m_pWindowList->GetPrev(pos);
		window = (cWindow *)m_pWindowList->GetPrev(pos);
		if( !window ) continue;
		
		if( window->GetType()  == WT_MSGBOX )
			return (cMsgBox*)window;
	}

	return NULL;
}

void cWindowManager::CloseAllMsgBox()
{
	PTRLISTSEARCHSTART( (*m_pWindowList), cWindow*, pWindow )
		if( pWindow )
		if( pWindow->GetType() == WT_MSGBOX )
		{
			((cMsgBox*)pWindow)->ForceClose();
		}
	PTRLISTSEARCHEND	
}

//사용에 주의하시오! 심각한 오류를 불러 일으킬수 있음. 제작:강은성
void cWindowManager::CloseAllMsgBoxNoFunc()
{
	PTRLISTSEARCHSTART( (*m_pWindowList), cWindow*, pWindow )
		if( pWindow )
		if( pWindow->GetType() == WT_MSGBOX )
		{
			AddListDestroyWindow( pWindow );			
		}
	PTRLISTSEARCHEND	
}



//////////////////////////////////////////////////////////////////////////
// yh edit 
BOOL cWindowManager::CloseAllWindow()
{
	GAMEIN->GetGuageDlg()->SetActive(FALSE);
	BOOL rt = FALSE;
	PTRLISTSEARCHSTART((*m_pWindowList),cWindow*,pWindow)
		if(pWindow->IsActive() == TRUE)
		{
			rt = TRUE;
			pWindow->SetActive(FALSE);
			m_bHideInterface = TRUE;
		}
	PTRLISTSEARCHEND

	return rt;
}
void cWindowManager::ShowBaseWindow()
{
	GAMEIN->GetChatDialog()->SetActive(TRUE);
	GAMEIN->GetGuageDlg()->SetActive(TRUE);
	GAMEIN->GetMainInterfaceDialog()->SetActive(TRUE);
	GAMEIN->GetQuickDialog()->SetActive(TRUE);
	m_bHideInterface = FALSE;
}
void cWindowManager::ToggleShowInterface()
{
	if(CloseAllWindow() == FALSE)
		ShowBaseWindow();
}
/*
void cWindowManager::SetConversionMode()
{
	HIMC hIMC = ImmGetContext( _g_hWnd );
	
	ImmGetConversionStatus( hIMC, &m_dwConversion, &m_dwSentence );
	char buf[128];
	sprintf( buf, "mc: %d, ms: %d", m_dwConversion, m_dwSentence );
	MessageBox( _g_hWnd, buf, "get message", MB_OK );

	ImmReleaseContext( _g_hWnd, hIMC );	
}
*/

void cWindowManager::CreateGuildInvitationKindSelectionDlg()
{
	cWindow* window = NULL;
#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/JoinOption.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/JoinOption.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/JoinOption.txt");
#endif
	AddWindow(window);

	CGuildJoinDialog* pDlg = (CGuildJoinDialog*)window;
	pDlg->Linking();
	GAMEIN->SetGuildJoinDialog( pDlg );
}


void cWindowManager::CreateGuildMunhaDlg()
{
	cWindow* window = NULL;
#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/GuildTraineeInfo.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/GuildTraineeInfo.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/GuildTraineeInfo.txt");
#endif
	AddWindow(window);

	CGuildMunhaDialog* pDlg = (CGuildMunhaDialog*)window;
	pDlg->Linking();
	GAMEIN->SetGuildMunhaDialog( pDlg );
}


void cWindowManager::CreateGuildTraineeDlg()
{
	cWindow* window = NULL;
#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/GuildTrainee.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/GuildTrainee.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/GuildTrainee.txt");
#endif
	AddWindow(window);

	CGuildTraineeDialog* pDlg = (CGuildTraineeDialog*)window;
	pDlg->Linking();
	GAMEIN->SetGuildTraineeDialgo( pDlg );
}


void cWindowManager::CreateSurvivalCountDlg()
{
	cWindow* window = NULL;
#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/SurvivalDlg.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/SurvivalDlg.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/SurvivalDlg.txt");
#endif
	AddWindow(window);

	CSurvivalCountDialog* pDlg = (CSurvivalCountDialog*)window;
	GAMEIN->SetSurvivalCountDialog( pDlg );
	pDlg->Linking();
	GAMEIN->GetSurvivalCountDlg()->InitSurvivalCountDlg(MAP->GetMapNum());
}

// magi82 - UniqueItem(070703)
void cWindowManager::CreateUniqueItemCurseCancellationDlg()
{
	cWindow* window = NULL;
#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/UniqueItemCurseCancellation.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/UniqueItemCurseCancellation.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/UniqueItemCurseCancellation.txt");
#endif
	
	AddWindow(window);
	GAMEIN->SetUniqueItemCurseCancellationDlg( (CUniqueItemCurseCancellationDlg*)window );
	GAMEIN->GetUniqueItemCurseCancellationDlg()->Linking();
}

// magi82 - UniqueItem(070709)
void cWindowManager::CreateUniqueItemMixDlg()
{
	cWindow* window = NULL;
#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/UniqueItemMixDlg.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/UniqueItemMixDlg.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/UniqueItemMixDlg.txt");
#endif

	AddWindow(window);
	GAMEIN->SetUniqueItemMixDlg( (CUniqueItemMixDlg*)window );
	GAMEIN->GetUniqueItemMixDlg()->Linking();
}

// magi82 - SOS(070724)
void cWindowManager::CreateSOSDlg()
{
	cWindow* window = NULL;
#ifdef _FILE_BIN_
//	window = GetDlgInfoFromFile("./image/SOScall.bin", "rb");
	window = GetDlgInfoFromFile("./image/InterfaceScript/SOScall.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/SOScall.txt");
#endif

	AddWindow(window);
	GAMEIN->SetSOSDlg( (CSOSDlg*)window );
	GAMEIN->GetSOSDlg()->Linking();


//	cWindow* window = NULL;
#ifdef _FILE_BIN_
	window = GetDlgInfoFromFile("./image/InterfaceScript/AutoNoteDlg.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/InterfaceScript/AutoNoteDlg.txt");
#endif

	AddWindow(window);
	GAMEIN->SetAutoNoteDlg( (CAutoNoteDlg*)window );
	GAMEIN->GetAutoNoteDlg()->Linking();

#ifdef _FILE_BIN_
	window = GetDlgInfoFromFile("./image/InterfaceScript/AutoAnswerDlg.bin", "rb");
#else
	window = GetDlgInfoFromFile("./image/InterfaceScript/AutoAnswerDlg.txt");
#endif

	AddWindow(window);
	GAMEIN->SetAutoAnswerDlg( (CAutoAnswerDlg*)window );
	GAMEIN->GetAutoAnswerDlg()->Linking();
}
