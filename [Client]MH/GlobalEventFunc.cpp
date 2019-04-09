// GlobalEventFunc.cpp: implementation of the Global Function
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GlobalEventFunc.h"

#include "MainGame.h"
#include "CharMake.h"
#include "CharSelect.h"
#include "GameIn.h"
#include "MainTitle.h"
#include "cComboBoxEx.h"
#include "cComboBox.h"

#include "ObjectManager.h"
#include "MoveManager.h"
#include "AppearanceManager.h"

#include "WindowIdEnum.h"
#include "./interface/cWindowManager.h"
#include "PartyManager.h"
#include "GuildManager.h"
#include "PyogukManager.h"
#include "FriendManager.h"
#include "NoteManager.h"
#include "ChatManager.h"
#include "ItemManager.h"
#include "ExchangeManager.h"
#include "StreetStallManager.h"
#include "VimuManager.h"
#include "FilteringTable.h"
#include "CharMakeManager.h"
#include "WantedManager.h"
#include "PetManager.h"
#include "../[CC]Suryun/SuryunManager_client.h"

#include "ChannelDialog.h"
#include "CharacterDialog.h"
#include "InventoryExDialog.h"
#include "MugongDialog.h"
// LYJ 051017 구입노점 추가
#include "StallKindSelectDlg.h"
#include "StreetBuyStall.h"
#include "BuyRegDialog.h"
#include "StreetStall.h"
#include "BuyItem.h"

#include "cListDialogEx.h"
#include "NpcScriptDialog.h"
#include "HelpDialog.h"
#include "HelperManager.h"
#include "HelperSpeechDlg.h"
#include "MoneyDlg.h"
#include "DealDialog.h"
#include "QuickDialog.h"
#include "MiniFriendDialog.h"
#include "FriendDialog.h"
#include "MiniNoteDialog.h"
#include "ChatDialog.h"
#include "NoteDialog.h"
#include "MacroDialog.h"
#include "ChatOptionDialog.h"
#include "OptionDialog.h"
#include "ExchangeDialog.h"
#include "CharStateDialog.h"
#include "MenuSlotDialog.h"
#include "PKLootingDialog.h"
#include "PyogukDialog.h"
#include "MiniMapDlg.h"
#include "CharMakeDialog.h"
#include "MugongSuryunDialog.h"
#include "SuryunDialog.h"
#include "ExitDialog.h"
#include "UpgradeDlg.h"
#include "MixDialog.h"
#include "ReinforceDlg.h"
#include "WantNpcDialog.h"
#include "WantRegistDialog.h"
#include "QuestDialog.h"
#include "JournalDialog.h"
#include "PartyCreateDlg.h"
#include "PartyInviteDlg.h"
#include "QuestManager.h"
#include "MPRegistDialog.h"
#include "GuildDialog.h"
#include "GuildCreateDialog.h"
#include "GuildMunhaDialog.h"
#include "GuildRankDialog.h"
#include "GuildMarkDialog.h"
#include "GuildNickNameDialog.h"
#include "GuildWarehouseDialog.h"
#include "TitanPartsMakeDlg.h"		// magi82 - Titan(070112)
#include "TitanMixDlg.h"			// magi82 - Titan(070119)
#include "TitanRepairDlg.h"			// 2007. 9. 11. CBH
#include "TitanRecallDlg.h"			// 2007. 9. 14. CBH
#include "TitanInventoryDlg.h"		// magi82 - Titan(070226)
#include "TitanGuageDlg.h"			// magi82 - Titan(070306)
// magi82 - Titan(070611) 타이탄 무공변환 주석처리
//#include "TitanMugongMixDlg.h"		// magi82 - Titan(070307)
#include "TitanRegisterDlg.h"		// magi82 - Titan(070320)
#include "TitanDissolutionDlg.h"	// magi82 - Titan(070323)
#include "TitanUpgradeDlg.h"
#include "TitanBreakDlg.h"
#include "TitanManager.h"
#include "MHMap.h"
#include "MHCamera.h"

#include "cMsgBox.h"
#include "cDivideBox.h"
#include "./input/UserInput.h"
#include "./Audio/MHAudioManager.h"
#include "cImeEx.h"
#include "PKManager.h"
#include "ObjectStateManager.h"
#include "ReviveDialog.h"
#include "MainBarDialog.h"
#include "UserInfoManager.h"
#include "JournalManager.h"
#include "ExitManager.h"
#include "BailDialog.h"
#include "InventoryExDialog.h"
#include "PartyIconManager.h"
#include "ExchangeItem.h"
#include "DissolutionDialog.h"
#include "GuildMarkManager.h"
#include "ItemShopDialog.h"
#include "MoveDialog.h"
#include "PointSaveDialog.h"
#include "GuildFieldWarDialog.h"
#include "GuildFieldWar.h"
#include "MallNoticeDialog.h"
#include "Shellapi.h"
#include "SkillPointRedist.h"
#include "SkillPointNotify.h"
#include "BarIcon.h"
#include "MainBarDialog.h"
#include "GameEventManager.h"
#include "PartyWar.h"
#include "ShoutDialog.h"
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

// guildunion
#include "GuildUnion.h"
#include "GuildUnionMarkMgr.h"

#include "SeigeWarDialog.h"

#include "CharChangeDlg.h"
#include "SealDialog.h"
#include "ChangeJobDialog.h"
#include "SiegeWarMgr.h"
#include "GuageDialog.h"
#include "ReinforceResetDlg.h"
#include "RareCreateDialog.h"
#include "ReinforceDataGuideDlg.h"
#include "TipBrowserDlg.h"

// Pet
#include "PetStateDlg.h"
#include "PetStateMiniDlg.h"
#include "PetInventoryDlg.h"
#include "PetUpgradeDialog.h"
#include "PetRevivalDialog.h"

/////////////
//---MurimNet
#include "MNStgPlayRoom.h"
#include "MNPlayRoomDialog.h"

#include "MNStgChannel.h"
#include "MNChannelDialog.h"

#include "MurimNet.h"
#include "MNPlayerManager.h"
#include "MNPlayer.h"

#include "ServerListDialog.h"
#include "AbilityManager.h"

#include "../Interface/cResourceManager.h"

#include "GuildNoteDlg.h"
#include "UnionNoteDlg.h"
#include "GuildNoticeDlg.h"

#include "SkillOptionChangeDlg.h"
#include "SkillOptionClearDlg.h"

#include "guildjoindialog.h"
#include "GuildTraineeDialog.h"

#include "GuildPlusTimeDialog.h"
#include "GuildMunhaDialog.h"

#include "UniqueItemCurseCancellationDlg.h"	// magi82 - UniqueItem(070703)
#include "UniqueItemMixDlg.h"				// magi82 - UniqueItem(070709)

#include "UniqueItemMixProgressBarDlg.h"	//2007. 10. 22. CBH
#include "TitanMixProgressBarDlg.h"	//2007. 10. 22. CBH
#include "TitanPartsProgressBarDlg.h"	//2007. 10. 23. CBH

#include "SkinSelectDialog.h"	//2007. 10. 23. CBH
#include "CostumeSkinSelectDialog.h"	//2008. 1. 16. CBH

#include "SOSDialog.h"

#include "SkillPointResetDlg.h"	// magi82(42) - 샵아이템 추가(수련치 초기화)

#include "NumberPadDialog.h"	//2008. 3. 17. CBH
#include "StallFindDlg.h"	// 노점 검색 추가 by Stiner(8)
#include "StreetStallItemViewDlg.h"
#include "AutoNoteDlg.h"	// autonote
#include "AutoAnswerDlg.h"
#include "FortWarDialog.h"


#include "PartyMatchingDlg.h"

#include "Intro.h"
#include <wininet.h>


#ifdef _NPROTECT_
#include "NProtectManager.h"
#endif
extern HWND _g_hWnd;

// test
extern char g_DistributeAddr[16];
extern WORD g_DistributePort;
//
extern int	g_nServerSetNum;

FUNC g_mt_func[] =
{
	{MT_LogInOkBtnFunc,"MT_LogInOkBtnFunc"},
	{MT_EditReturnFunc, "MT_EditReturnFunc"},
	{MT_ExitBtnFunc,"MT_ExitBtnFunc"},
	{MT_DynamicBtnFunc,"MT_DynamicBtnFunc"},
	{MT_DynamicEditReturnFunc, "MT_DynamicEditReturnFunc"},

	{CS_BtnFuncCreateChar,"CS_BtnFuncCreateChar"}, 
	{CS_BtnFuncDeleteChar,"CS_BtnFuncDeleteChar"},	 
	{CS_BtnFuncFirstChar,"CS_BtnFuncFirstChar"},	 
	{CS_BtnFuncSecondChar, "CS_BtnFuncSecondChar"},
	{CS_BtnFuncThirdChar,"CS_BtnFuncThirdChar"},
	{CS_BtnFuncFourthchar,"CS_BtnFuncFourthchar"},
	{CS_BtnFuncFifthchar,"CS_BtnFuncFifthchar"},
	{CS_BtnFuncEnter,"CS_BtnFuncEnter"},//¡E￠c¡§￠®OAO￠®¡×oAAU ¡§Ioo￠®¡×¡E￠c¡E￠c¡§￠® //10
	{CS_FUNC_OkISee,"CS_FUNC_OkISee"},
	{CS_BtnFuncLogOut,"CS_BtnFuncLogOut"},

	{MI_CharBtnFunc, "MI_CharBtnFunc"},
	{MI_ExchangeBtnFunc, "MI_ExchangeBtnFunc"},
	{MI_InventoryBtnFunc, "MI_InventoryBtnFunc"},
	{MI_MugongBtnFunc, "MI_MugongBtnFunc"},
	{MI_ChatEditBoxFunc, "MI_ChatEditBoxFunc"},

	{IN_CreateStreetStallFunc, "IN_CreateStreetStallFunc"},
	{IN_DlgFunc, "IN_DlgFunc"}, 
	{MGI_DlgFunc, "MGI_DlgFunc"},	//20
	{MGI_SuryunDlgFunc, "MGI_SuryunDlgFunc"},	
	{MGI_MugongDlgFunc, "MGI_MugongDlgFunc"},	
	{CI_DlgFunc, "CI_DlgFunc"},
	{CI_AddExpPoint, "CI_AddExpPoint"},
	{PA_DlgFunc, "PA_DlgFunc"},

	{SO_DlgFunc, "SO_DlgFunc"},

	{BRS_DlgFunc, "BRS_DlgFunc"},
	{BRS_CloseFunc, "BRS_CloseFunc"},
	{BRS_DeleteFunc, "BRS_DeleteFunc"},
	{BRS_TypeListFunc, "BRS_TypeListFunc"},
	{BRS_ItemListFunc, "BRS_ItemListFunc"},
	{BRS_ClassListFunc, "BRS_ClassListFunc"},
	{BRS_VolumeEditFunc, "BRS_VolumeEditFunc"},
	{BRS_MoneyEditFunc, "BRS_MoneyEditFunc"},
	{BRS_REGBtnFunc, "BRS_REGBtnFunc"},

	{BS_DlgFunc, "BS_DlgFunc"},
	{BS_TitleEditBoxFunc, "BS_TitleEditBoxFunc"},
	{BS_MoneyEditBoxFunc, "BS_MoneyEditBoxFunc"},
	{BS_SellBtnFunc, "BS_SellBtnFunc"}, 
	{BS_REGBtnFunc, "BS_REGBtnFunc"}, 
	{BS_SPFunc, "BS_SPFunc"},	

	{SSI_DlgFunc, "SSI_DlgFunc"},
	{SSI_TitleEditBoxFunc, "SSI_TitleEditBoxFunc"},
	{SSI_MoneyEditBoxFunc, "SSI_MoneyEditBoxFunc"},
	{SSI_BuyBtnFunc, "SSI_BuyBtnFunc"}, 
	{SSI_RegistBtnFunc, "SSI_RegistBtnFunc"}, //30
	{SSI_EditBtnFunc, "SSI_EditBtnFunc"}, 
	
	{NSI_DlgFunc, "NSI_DlgFunc"}, // NPC ￠®E￠®Ie¡E￠cicA¡E￠cE￠®E?￠®E¡Ec ¡E￠cic￠®E?e//
	{NSI_HyperLinkFunc, "NSI_HyperLinkFunc"},
	
	{HI_DlgFunc, "HI_DlgFunc"}, // HelpA￠®E¡E?¡E￠c ￠®ic¡E?e
	{HI_HyperLinkFunc, "HI_HyperLinkFunc"},
	{HI_BtnFunc, "HI_BtnFunc"},

	{HSI_ExitBtnFunc, "HSI_ExitBtnFunc"},

	{CMI_MoneyOkFunc, "CMI_MoneyOkFunc"},
	{CMI_MoneySpinFunc, "CMI_MoneySpinFunc"}, 
	
	{DE_BuyItemFunc, "DE_BuyItemFunc"},//40
	{DE_SellItemFunc, "DE_SellItemFunc"},
	{DE_DlgFunc, "DE_DlgFunc"},
	{DE_CloseHideBtn, "DE_CloseHideBtn"},

	{PA_BtnFunc, "PA_BtnFunc"},
	
	{QI_QuickDlgFunc, "QI_QuickDlgFunc"},

	{CM_OverlapCheckBtnFunc, "CM_OverlapCheckBtnFunc"},//AE¡E￠c¡E￠￥￠®¡×￠®ⓒ­AI ¡E￠ciy￠®¡×u￠®¡×￠®ⓒ­ A¡E￠cE ¡§Ioo￠®¡×¡E￠c¡E￠c¡§￠® CO￠®¡×uo
	{CM_CharMakeBtnFunc, "CM_CharMakeBtnFunc"},
	{CM_CharCancelBtnFunc, "CM_CharCancelBtnFunc"}, 
	{CM_ComboBoxCheckFunc,"CM_ComboBoxCheckFunc"}, 
	{CMFUNC_OkISee,"CMFUNC_OkISee"}, //50
	{CM_OptionSelectBtnFunc, "CM_OptionSelectBtnFunc"},

	{PYO_DlgBtnFunc, "PYO_DlgBtnFunc"},
	{Note_DlgBtnFunc, "Note_DlgBtnFunc"},
	{Friend_DlgBtnFunc, "Friend_DlgBtnFunc"},
	{CR_DlgBtnFunc, "CR_DlgBtnFunc"},
	
	{MNM_DlgFunc,"MNM_DlgFunc"},
	{ITD_DlgFunc,"ITD_DlgFunc"},
	{ITMD_DlgFunc,"ITMD_DlgFunc"},  //60
	{ITR_DlgFunc, "ITR_DlgFunc"}, //

	{CHA_DlgBtnFunc, "CHA_DlgBtnFunc"},
	
//KES DIALOG
	{MAC_DlgFunc, "MAC_DlgFunc"},
	{CTI_DlgFunc, "CTI_DlgFunc"},	//A￠®E¡E¡I￠®¡×¡E￠cAA¡E￠cE
	{COI_DlgFunc, "COI_DlgFunc"},	//A￠®E¡E¡I￠®¡×¡E￠cA￠®E?E￠®¡×uCA¡E￠cE
	{OTI_DlgFunc, "OTI_DlgFunc"},	//¡E￠c¡§￠®OAO￠®E?E￠®¡×uCA¡E￠cE
	{EXT_DlgFunc, "EXT_DlgFunc"},	//A￠®¡×u¡E￠c¡E￠￥aA¡E￠cE	//
	{XCI_DlgFunc, "XCI_DlgFunc"},	//¡E￠c¡§u¡§I¨I¨￡E?A¡E￠cE
	{CSS_DlgFunc, "CSS_DlgFunc"},	//A¡§I¨I¨￡￠®E￠®ⓒ­?AI¡E￠cioAAA¡E￠cE 
	{MSI_MenuSlotDlgFunc, "MSI_MenuSlotDlgFunc"},	//70
	{PLI_DlgFunc, "PLI_DlgFunc"},
	{CNA_BtnOkFunc, "CNA_BtnOkFunc"},
	{DIS_DlgFunc, "DIS_DlgFunc"},	//ºÐCØ
	{MI_DlgFunc, "MI_DlgFunc"},
	{DIVIDEBOX_Func, "DIVIDEBOX_Func"},	//Divide Box
	{ITEM_DlgFunc, "ITEM_DlgFunc" },
/////////////
//---MurimNet
	{MNPRI_DlgFunc, "MNPRI_DlgFunc"},
	{MNCNL_DlgFunc, "MNCNL_DlgFunc"}, 
/////////////
	{SL_DlgBtnFunc, "SL_DlgBtnFunc" },
	
	{WANTNPC_DlgFunc, "WANTNPC_DlgFunc"},
	{WANTREG_DlgFunc, "WANTREG_DlgFunc"},

	{QUE_QuestDlgFunc, "QUE_QuestDlgFunc"},
	{QUE_JournalDlgFunc, "QUE_JournalDlgFunc"},	//80
	{MP_RegistDlgFunc, "MP_RegistDlgFunc"},

	{BAIL_BailDlgFunc, "BAIL_BailDlgFunc"},
	{ITMALL_DlgBtnFunc, "ITMALL_DlgBtnFunc"},
	{SA_DlgBtnFunc, "SA_DlgBtnFunc"},
	{CHA_DlgFunc, "CHA_DlgFunc"},

	{GD_DlgFunc, "GD_DlgFunc"},

	// guildfieldwar
	{GFW_DlgFunc, "GFW_DlgFunc" },

	{SK_DlgBtnFunc, "SK_DlgBtnFunc"},

	// partywar
	{PW_DlgFunc, "PW_DlgFunc"},

	// chase
	{CHS_DlgFunc, "CHS_DlgFunc"},

	//
	{CHAN_DlgFunc, "CHAN_DlgFunc"},
	
	// Guild Tournament
	{GDT_DlgFunc, "GDT_DlgFunc"},
	
	// seigewar
	{SW_DlgFunc, "SW_DlgFunc" },

	// Character Change
	{CHA_ChangeDlgFunc, "CHA_ChangeDlgFunc" },

	{IT_DlgFunc, "IT_DlgFunc"},

	//
	{CJOB_DlgFunc, "CJOB_DlgFunc"},
	
	{CG_DlgFunc, "CG_DlgFunc"},

	// Reinforce Reset
	{RFDefault_DlgFunc, "RFDefault_DlgFunc"},

	{RareCreate_DlgFunc, "RareCreate_DlgFunc"},
	
	
	// Pet
	{PET_DlgFunc, "PET_DlgFunc"},

	{PET_MiniDlgFunc, "PET_MiniDlgFunc"},

	{PET_InvenDlgFunc, "PET_InvenDlgFunc"},

	{PET_UpgradeDlgFunc, "PET_UpgradeDlgFunc"},

	{PET_RevivalDlgFunc, "PET_RevivalDlgFunc"},
	// 06. 02. 강화 재료 안내 인터페이스 추가 - 이영준
	{RFDGUIDE_DlgFunc, "RFDGUIDE_DlgFunc"},
	// 06. 02. 내정보탕 팁보기 추가 - 이영준
	{TB_DlgFunc, "TB_DlgFunc"},
	{TB_STATE_DlgFunc, "TB_STATE_DlgFunc"},

	{GN_DlgBtnFunc, "GN_DlgBtnFunc"},
	{AN_DlgBtnFunc, "AN_DlgBtnFunc"},
	// 06. 03. 문파공지 - 이영준
	{GNotice_DlgBtnFunc, "GNotice_DlgBtnFunc"},
	
	//060802 GuildPlusTime - wonju
	{GuildPlusTime_DlgFunc, "GuildPlusTime_DlgFunc"},

	//////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환
	{SkillTrans_DlgFunc, "SkillTrans_DlgFunc"},
	{TDefault_DlgFunc, "TDefault_DlgFunc"},
	//////////////////////////////////////////////////////////

	{JO_DlgFunc, "JO_DlgFunc"},
	{GU_DlgFunc, "GU_DlgFunc"},	//magi82 - 문하생 가입편리시스템
	{GT_DlgFunc, "GT_DlgFunc"},	//magi82 - 문하생 가입편리시스템
	{TPMD_DlgFunc, "TPMD_DlgFunc"},							// magi82 - Titan(070112)
	{TitanMix_DlgFunc, "TitanMix_DlgFunc"},					// magi82 - Titan(070119)
	{Titan_Repair_DlgFunc, "Titan_Repair_DlgFunc"},			//2007. 9. 11. CBH - 타이탄 수리창 이벤트
	{Titan_Recall_DlgFunc, "Titan_Recall_DlgFunc"},			//2007. 9. 14. CBH - 타이탄 소환 이벤트
	{TitanUpgrade_DlgFunc, "TitanUpgrade_DlgFunc"},			// magi82 - Titan(070131)
	{TitanBreak_DlgFunc, "TitanBreak_DlgFunc"},				// magi82 - Titan(070205)	
	{titan_inventory_DlgFunc, "titan_inventory_DlgFunc"},	// magi82 - Titan(070226)
	{Titan_guage_DlgFunc, "Titan_guage_DlgFunc"},			// magi82 - Titan(070307)
	//{Titan_MugongMix_DlgFunc, "Titan_MugongMix_DlgFunc"},	// magi82 - Titan(070611) 타이탄 무공변환 주석처리
	{Titan_Use_DlgFunc, "titan_Use_DlgFunc"},				// magi82 - Titan(070320)
	{Titan_Bongin_DlgFunc, "Titan_Bongin_DlgFunc"},				// magi82 - Titan(070323)
	{UniqueItemCurseCancellationDlgFunc, "UniqueItemCurseCancellationDlgFunc"	},	// magi82 - UniqueItem(070703)
	{UniqueItemMixDlgFunc, "UniqueItemMixDlgFunc"	},	// magi82 - UniqueItem(070709)
	{SOSDlgFunc, "SOS_DlgFunc"},	// magi82 - SOS(070724)
	{UniqueItemMix_ProgressBar_DlgFunc, "UniqueItemMix_ProgressBar_DlgFunc"},	//2007. 11. 22. CBH - 유니크 아이탬 조합 게이지 인터페이스
	{TitanMix_ProgressBar_DlgFunc, "TitanMix_ProgressBar_DlgFunc"},	//2007. 11. 22. CBH - 타이탄 조합 게이지 인터페이스
	{TitanParts_ProgressBar_DlgFunc, "TitanParts_ProgressBar_DlgFunc"},	//2007. 11. 23. CBH - 타이탄 파츠 조합 게이지 인터페이스

	{SkinSelect_DlgFunc, "SkinSelect_DlgFunc"},	//2007. 12. 4. CBH - 스킨 선택
    {CostumeSkinSelect_DlgFunc, "CostumeSkinSelect_DlgFunc"},	//2008. 1. 16. CBH - 코스튬 스킨 선택
	{SK_Reset_DlgFunc, "SK_Reset_DlgFunc"},	// magi82(42) - 샵아이템 추가(수련치 초기화)
	{NumberPad_DlgFunc, "NumberPad_DlgFunc"},	//2008. 3. 17. CBH - 보안번호 입력 이벤트
	{MT_LOGINDLGFunc, "MT_LOGINDLGFunc"},	//2008. 3. 19. CBH - 로그인 다이얼로그 콤보박스 이벤트 추가
	{SVD_DlgFunc, "SVD_DlgFunc"},	// magi82(47)

	{SFR_DlgFunc, "SFR_DlgFunc"},

	// autonote
	{AutoNoteDlg_Func, "AutoNoteDlg_Func"},
	{AutoAnswerDlg_Func, "AutoAnswerDlg_Func"},

	{PA_CreateBtnFunc, "PA_CreateBtnFunc"},	//2008. 5. 21. CBH - 파티 생성 인터페이스 버튼 이벤트 추가
	{Party_MatchingList_DlgFunc, "Party_MatchingList_DlgFunc"},	//2008. 6. 2. CBH - 파티 매칭 버튼 이벤트

	// 요새전
	{FW_DlgFunc, "FW_DlgFunc" },
	{FD_WareHouseDlgFunc, "FD_WareHouseDlgFunc"},

	{NULL, ""},	//마지막줄에 이거 꼭 놔두세요.
};

int FUNCSEARCH(char * funcName)
{
	for(int i = 0 ;; i++)
	{
		if(g_mt_func[i]._func == NULL)
			break;
		if(strcmp(funcName, g_mt_func[i]._funcName)==0)
				return i;
	}

	return -1; 
}

////////////////////////////////////////////////////////////////////////////////////////////////////
extern void CMFUNC_OkISee(LONG lId, void* p, DWORD we)
{
	cStatic * CharacterMessageDlg = (cStatic *)WINDOWMGR->GetWindowForIDEx(CMID_MessageBox);
	CharacterMessageDlg->SetActive(FALSE);
		
}

////////////////////////////////////// NewCharSelect /////////////////////////////////////////////
void CS_BtnFuncLogOut(LONG lId, void* p, DWORD we)
{
	CHARSELECT->BackToMainTitle();
	TITLE->SetServerList();
	CAMERA->ResetInterPolation();
}
void CS_BtnFuncCreateChar(LONG lId, void* p, DWORD we)
{
	// AE￠®E￠®ⓒ­? ￠®E￠®ⓒ­￠®E￠®ⓒ­￠®Iie¡E￠c¡§ua¡E￠c¡E￠￥I AI￠®Ii￠®E?.. [5/21/2003]
	if(CHARSELECT->IsFull() == FALSE)
	{
		MAINGAME->SetGameState(eGAMESTATE_CHARMAKE);

		//
		CAMERA->MoveStart(eCP_CHARMAKE);
	}
	else
	{		
		CHARSELECT->DisplayNotice(17);		
	}
}

//￠®E￠®I¡E￠c¡§I¡E￠c¡E￠￥?￠®E￠®ⓒ­￠®E￠®ⓒ­Ao¡E￠c¡§￠®i ￠®¡×uE￠®¡×¡E￠c¡E￠cE￠®¡×ui¡§I¨I¨￡￠®¡×￠®I￠®E?A￠®E￠®IA C￠®¡×￠®I￠®¡×o¡E￠c¨I¡ⓒ¡§Ioo￠®¡×¡E￠c¡E￠c¡§￠®AI CE￠®E?aCI￠®E￠®IU.
void		CS_BtnFuncFirstChar(LONG lId, void* p, DWORD we)
{
	if( we & WE_PUSHDOWN )
		CHARSELECT->SelectPlayer(0);
	else if( we & WE_PUSHUP )
//		((cPushupButton*)WINDOWMGR->GetWindowForIDEx( lId ))->SetPush( TRUE );
		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( TRUE );
}
void		CS_BtnFuncSecondChar(LONG lId, void* p, DWORD we)
{
	if( we & WE_PUSHDOWN )
		CHARSELECT->SelectPlayer(1);
	else if( we & WE_PUSHUP )
//		((cPushupButton*)WINDOWMGR->GetWindowForIDEx( lId ))->SetPush( TRUE );
		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( TRUE );
}
void		CS_BtnFuncThirdChar(LONG lId, void* p, DWORD we)
{

#ifdef _HK_LOCAL_
	if( !CHARSELECT->GetExtraCharacterSlot() )
	{
		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( FALSE );
		return;
	}
#endif

#ifdef _TL_LOCAL_
	if( !CHARSELECT->GetExtraCharacterSlot() )
	{
		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( FALSE );
		return;
	}
#endif

	if( we & WE_PUSHDOWN )
		CHARSELECT->SelectPlayer(2);
	else if( we & WE_PUSHUP )
//		((cPushupButton*)WINDOWMGR->GetWindowForIDEx( lId ))->SetPush( TRUE );
		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( TRUE );
}
void		CS_BtnFuncFourthchar(LONG lId, void* p, DWORD we)
{
#ifdef _JAPAN_LOCAL_
	if( !CHARSELECT->GetExtraCharacterSlot() )
	{
		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( FALSE );
		return;
	}
#endif

#ifdef _HK_LOCAL_
	if( CHARSELECT->GetExtraCharacterSlot() < 2 )
	{
		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( FALSE );
		return;
	}
#endif

#ifdef _TL_LOCAL_
	if( CHARSELECT->GetExtraCharacterSlot() < 2 )
	{
		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( FALSE );
		return;
	}
#endif


	if( we & WE_PUSHDOWN )
		CHARSELECT->SelectPlayer(3);
	else if( we & WE_PUSHUP )
		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( TRUE );
}
void		CS_BtnFuncFifthchar(LONG lId, void* p, DWORD we)
{
#ifdef _JAPAN_LOCAL_
	if( CHARSELECT->GetExtraCharacterSlot() < 2 )
	{
        ((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( FALSE );
		//캐릭터 사용 기간이 지났습니다.
		return;
	}
#endif

#ifdef _HK_LOCAL_
	if( CHARSELECT->GetExtraCharacterSlot() < 3 )
	{
        ((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( FALSE );
		return;
	}
#endif
#ifdef _TL_LOCAL_
	if( CHARSELECT->GetExtraCharacterSlot() < 3 )
	{
        ((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( FALSE );
		//캐릭터 사용 기간이 지났습니다.
		return;
	}
#endif

	if( we & WE_PUSHDOWN )
		CHARSELECT->SelectPlayer(4);
	else if( we & WE_PUSHUP )
		((cPushupButton*)((cDialog*)p)->GetWindowForID(lId))->SetPush( TRUE );
}
void CS_BtnFuncEnter(LONG lId, void* p, DWORD we)
{
	if(	CHARSELECT->GetCurSelectedPlayer() == NULL)
		CHARSELECT->DisplayNotice(18);
	else
	{
		CHARSELECT->SendMsgGetChannelInfo();
		CHARSELECT->SetDisablePick(TRUE);
		
		// 카메라 이동보간 리셋, 하이트필드 적용
//		CAMERA->ResetInterPolation();
	} 
}

void CS_FUNC_OkISee(LONG lId, void* p, DWORD we)	 
{
	cStatic * CharacterSelectMessageDlg = (cStatic *)WINDOWMGR->GetWindowForIDEx(CSID_MessageBox);
	CharacterSelectMessageDlg->SetActive(FALSE);
}

void CS_BtnFuncDeleteChar(LONG lId, void* p, DWORD we)
{
	int nChar = CHARSELECT->GetCurSelectedPlayerNum();
	int MaxChar = MAX_CHARACTER_NUM-1;
#ifdef _JAPAN_LOCAL_
	MaxChar = GIVEN_CHARACTER_SLOT-1+CHARSELECT->GetExtraCharacterSlot();
	int msgposx[MAX_CHARACTER_NUM] = { 130, 370, 550, 500, 370 };
	int msgposy[MAX_CHARACTER_NUM] = { 350, 490, 360, 520, 300 };

#elif defined _HK_LOCAL_
	MaxChar = GIVEN_CHARACTER_SLOT-1+CHARSELECT->GetExtraCharacterSlot();
	int msgposx[MAX_CHARACTER_NUM] = { 130, 370, 550, 500, 235 };
	int msgposy[MAX_CHARACTER_NUM] = { 350, 490, 360, 520, 200 };

#elif defined _TL_LOCAL_
	MaxChar = GIVEN_CHARACTER_SLOT-1+CHARSELECT->GetExtraCharacterSlot();
	int msgposx[MAX_CHARACTER_NUM] = { 130, 370, 550, 500, 370 };
	int msgposy[MAX_CHARACTER_NUM] = { 350, 490, 360, 520, 300 };

#else
	int msgposx[MAX_CHARACTER_NUM] = { 130, 370, 550, 500 };
	int msgposy[MAX_CHARACTER_NUM] = { 350, 490, 360, 520 };
#endif

	if( nChar >= 0 && nChar <= MaxChar )
	{
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_DELETECHAR, MBT_YESNO, CHATMGR->GetChatMsg(282) );
		if( pMsgBox )
			pMsgBox->SetAbsXY( msgposx[nChar], msgposy[nChar] );
//			pMsgBox->SetAbsXY( nChar * 180 + 150, 450 );
		CHARSELECT->SetDisablePick( TRUE );
	}
	else
	{
		// AE￠®E￠®ⓒ­?AI￠®E￠®ⓒ­| ￠®¡×u¡E￠c¡§uAACN EA ¡E￠cieA| ¡§Ioo￠®¡×¡E￠c¡E￠c¡§￠®A¡E￠ci ￠®E￠®I¡E￠c¡§I¡E￠c¡E￠￥?AO￠®¡×u￠®¡×u￠®E?a.
		CHARSELECT->DisplayNotice( 20 );
	}
}

// AE¡E￠c¡E￠￥￠®¡×￠®ⓒ­AI ¡E￠ciy￠®¡×u￠®¡×￠®ⓒ­A¡E￠cE CO￠®¡×uo [5/13/2003]
//////////////////////////////////////////////////////////////////////////
extern void	CM_OverlapCheckBtnFunc(LONG lId, void* p, DWORD we)
{
	//PJS ￠®E?￠®¡×u¡§I¨I¨￡?￠®¡×uO￠®¡×i¨Io￠®Ei¡§u￠®E?￠®E¡Ec￠®¡×u¡E￠c¡§I AU￠®Iia￠®E￠®ⓒ­| ¡§Ii¨Io￠®Ei¡§o￠®¡×u￠®¡×¡E￠c ¡§I¨I¨￡O￠®E￠®IA￠®E￠®IU. [5/13/2003]

	cEditBox * editboxName = (cEditBox *)WINDOWMGR->GetWindowForIDEx(CMID_IDEDITBOX);
	char* pName = editboxName->GetEditText();
	int nLen	= strlen(pName);

	if( nLen == 0 )
	{
		//AI¡§IoI ￠®E￠®ⓒ­￠®¡×￠®¨I￠®¡×u￠®¡×uAo ¡§IoU￠®¡×i¨Io￠®Ei¡§u¡E￠c¡§￠®￠®E¡Ec ￠®EO¡E￠c¡§￠®AOA￠®E￠®ⓒ­￠®E￠®ⓒ­e ￠®¡×u¡§I¡§¡I￠®¡×uO¡E￠c¡§￠®i ￠®E￠®IU￠®¡×oA ￠®E￠®ⓒ­￠®E￠®ⓒ­￠®Iie¡E￠c¡§ua.
		//￠®¡×u￠®¡×¡E￠c￠®E￠®II￠®E￠®ⓒ­e MAKE DIALOG￠®E￠®ⓒ­| ￠®Ii¡§I￠®I￠®¡×i¨Io￠®Ei¡§u￠®E?￠®E¡EcAI￠®¡×￠®ⓒ­i...(¡E￠c¡§ua￠®E￠®IEAI AO￠®EO¡§I￠®"￠c¡§￠®￠®E¡Ec --; )
		CHARMAKE->DisplayNotice( 11 );
		return;
	}
	else if( nLen < 4 )
	{
		CHARMAKE->DisplayNotice( 19 );		
		return;
	}

	//¡E￠cic￠®E?eCIAo ￠®E￠®ⓒ­¡§I¡§¡ICI￠®E￠®IA ¡§Io￠®EcAU¡E￠c¡§￠®￠®E¡Ec ￠®Iie￠®¡×ui¡E￠c¡§￠®¡E￠c¨I¡ⓒ¡§I¨I¨￡￠®¡×￠®I?
	if( FILTERTABLE->IsInvalidCharInclude((unsigned char*)pName) )
//	if( FILTERTABLE->IsInvalidCharacterName( (unsigned char*)pName) )
	{
		//AI¡§IoI ￠®E￠®ⓒ­￠®¡×￠®¨I￠®¡×u￠®¡×uAo ¡§IoU￠®¡×i¨Io￠®Ei¡§u¡E￠c¡§￠®￠®E¡Ec ￠®EO¡E￠c¡§￠®AOA￠®E￠®ⓒ­￠®E￠®ⓒ­e ￠®¡×u¡§I¡§¡I￠®¡×uO¡E￠c¡§￠®i ￠®E￠®IU￠®¡×oA ￠®E￠®ⓒ­￠®E￠®ⓒ­￠®Iie¡E￠c¡§ua.
		//¡E￠cic￠®E?eCO ￠®¡×uo ￠®¡×u¡§I¡§¡I￠®E￠®IA A¡§I¨I¨￡￠®E￠®ⓒ­?￠®E￠®ⓒ­iAO￠®E￠®II￠®E￠®IU.
		CHARMAKE->DisplayNotice( 14 );
		return;
	}

	//¡E￠cic￠®E?eCIAo ￠®E￠®ⓒ­¡§I¡§¡ICI￠®E￠®IA A¡§I¨I¨￡￠®E￠®ⓒ­?￠®E￠®ⓒ­iAI¡E￠c¡§￠®￠®E¡Ec?
	if( !FILTERTABLE->IsUsableName(pName) )
	{
		CHARMAKE->DisplayNotice( 14 );
		return;
	}

	MSG_NAME msg;
	
	msg.Category = MP_USERCONN;
	msg.Protocol = 	MP_USERCONN_CHARACTER_NAMECHECK_SYN;
	strcpy(msg.Name,pName);
	NETWORK->Send(&msg,sizeof(msg));

	CHARMAKE->SetDisableDlg( TRUE );
}

extern void	CM_CharMakeBtnFunc(LONG lId, void* p, DWORD we)
{
	// PJS ¡E￠cio¡E￠c¡E￠￥I ￠®E￠®ⓒ­￠®E￠®ⓒ­￠®Iic AE￠®E￠®ⓒ­?AIAC A￠®E¡E¡I￠®¡×￠®ⓒ­￠®E￠®ⓒ­￠®E￠®ⓒ­| ￠®¡×u¡E￠c¡§I¡§Ioo￠®E?￠®E¡Ec AO¡E￠c¡§￠®i ￠®E￠®IU￠®¡×oA AE¡E￠c¡E￠￥￠®¡×￠®ⓒ­AI ￠®¡×u¡E￠c¡§uAAA￠®E￠®ⓒ­¡E￠c¡E￠￥I ￠®Ii¡§Io￠®¡×u￠®¡×¡E￠c ¡E￠c¡§￠®¡E￠cI￠®E￠®IU. [5/13/2003]
	cEditBox * editboxName = (cEditBox *)WINDOWMGR->GetWindowForIDEx(CMID_IDEDITBOX);
	char* pName = editboxName->GetEditText(); 
	int nLen	= strlen(pName);

	if( nLen == 0 )
	{
		CHARMAKE->DisplayNotice( 11 );
		return;
	}
	else if( nLen < 4 )
	{
		CHARMAKE->DisplayNotice( 19 );
		return;
	}

	//¡E￠cic￠®E?eCIAo ￠®E￠®ⓒ­¡§I¡§¡ICI￠®E￠®IA ¡§Io￠®EcAU¡E￠c¡§￠®￠®E¡Ec ￠®Iie￠®¡×ui¡E￠c¡§￠®¡E￠c¨I¡ⓒ¡§I¨I¨￡￠®¡×￠®I?
	if( FILTERTABLE->IsInvalidCharInclude((unsigned char*)pName) )
//	if( FILTERTABLE->IsInvalidCharacterName( (unsigned char*)pName) )
	{
		//AI¡§IoI ￠®E￠®ⓒ­￠®¡×￠®¨I￠®¡×u￠®¡×uAo ¡§IoU￠®¡×i¨Io￠®Ei¡§u¡E￠c¡§￠®￠®E¡Ec ￠®EO¡E￠c¡§￠®AOA￠®E￠®ⓒ­￠®E￠®ⓒ­e ￠®¡×u¡§I¡§¡I￠®¡×uO¡E￠c¡§￠®i ￠®E￠®IU￠®¡×oA ￠®E￠®ⓒ­￠®E￠®ⓒ­￠®Iie¡E￠c¡§ua.
		//¡E￠cic￠®E?eCO ￠®¡×uo ￠®¡×u¡§I¡§¡I￠®E￠®IA A¡§I¨I¨￡￠®E￠®ⓒ­?￠®E￠®ⓒ­iAO￠®E￠®II￠®E￠®IU.
		CHARMAKE->DisplayNotice( 14 );
		return;
	}

	//¡E￠cic￠®E?eCIAo ￠®E￠®ⓒ­¡§I¡§¡ICI￠®E￠®IA A¡§I¨I¨￡￠®E￠®ⓒ­?￠®E￠®ⓒ­iAI¡E￠c¡§￠®￠®E¡Ec?
	if( !FILTERTABLE->IsUsableName(pName) )
	{
		CHARMAKE->DisplayNotice( 14 );
		return;
	}

	
	CHARACTERMAKEINFO msg;
	memcpy(&msg, CHARMAKEMGR->GetCharacterMakeInfo(), sizeof(msg) );

	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CHARACTER_MAKE_SYN;
	msg.StandingArrayNum = -1;
	strcpy(msg.Name, editboxName->GetEditText());
	
	NETWORK->Send(&msg,sizeof(msg));

	CHARMAKE->SetDisableDlg( TRUE );
//	CHARMAKE->GetCharMakeDialog()->SetDisable( TRUE );
}

extern void	CM_CharCancelBtnFunc(LONG lId, void* p, DWORD we)
{
	MSGBASE msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_DIRECTCHARACTERLIST_SYN;
	NETWORK->Send(&msg,sizeof(msg));	

	//
	CAMERA->MoveStart(eCP_CHARSEL);
}

extern void CM_ComboBoxCheckFunc( LONG lId, void* p, DWORD we )
{
	if( we != WE_COMBOBOXSELECT ) return;
//	cComboBoxEx* pBox = (cComboBoxEx*)p;	//PPP

	cComboBoxEx* pBox = (cComboBoxEx*)((cDialog*)p)->GetWindowForID( lId );
	
	if( CHARMAKEMGR->ReplaceCharMakeInfo( (CHAR_M)pBox->GetOptionValue(), pBox->GetCurSelectedIdx() ) )
	{
		APPEARANCEMGR->AddCharacterPartChange(1);
		APPEARANCEMGR->SetScalebyGuageBar(1);
	}
//KES
	WINDOWMGR->SetMouseDownUsed();
}

extern void CM_OptionSelectBtnFunc(LONG lId, void* p, DWORD we)
{
	if( we != WE_BTNCLICK ) return;

	CHARMAKE->GetCharMakeDialog()->OnActionEvent(lId, p, we);
}


/////////////////////////////////////// NewMainTitle ///////////////////////////////////////////
//#include "MainTitle.h"
extern char g_szHeroIDName[];
extern char g_CLIENTVERSION[];

void MT_LogInOkBtnFunc(LONG lId, void* p, DWORD we)
{
	cDialog * dlg = (cDialog *)p;
	cEditBox * editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
	cEditBox * editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);	
	CNumberPadDialog* pNumberPadDlg = GAMEIN->GetNumberPadDlg();	//2008. 3. 18. CBH - 보안번호 정보를 얻기위해 관련 클래스 얻기
	char * userID = editboxId->GetEditText();
	char * userPWD = editboxPwd->GetEditText();	
	char * userPN = pNumberPadDlg->GetProtectionStr();	//2008. 3. 18. CBH - 입력된 보안번호 얻기

	//契법pin쇱꿴
	sprintf(userPN,"%d",1111);

	//if((strcmp(userID, "") != 0) && (strcmp(userPWD, "") != 0) && (strcmp(userPN, "") != 0))
	if((strcmp(userID, "") != 0) && (strcmp(userPWD, "") != 0))
	{
		MSG_LOGIN_SYN msg;
		msg.Category = MP_USERCONN;
		msg.Protocol = MP_USERCONN_LOGIN_SYN;
#ifdef _HK_LOCAL_
		msg.dwObjectID = TITLE->m_dwOld;
#endif

		SafeStrCpy(msg.id, userID, MAX_NAME_LENGTH + 1 );
		SafeStrCpy(msg.pw, userPWD, MAX_NAME_LENGTH + 1 );
		SafeStrCpy(msg.pn, userPN, MAX_NAME_LENGTH + 1 );	//2008. 3. 18. CBH - 입력된 보안번호 메세지에 저장
		SafeStrCpy(msg.Version,g_CLIENTVERSION, 16 );

#ifdef _NPROTECT_
		NPROTECTMGR->TwistAccount( msg.id, msg.pw, msg.Version );
#endif

#ifdef _KOR_LOCAL_
		cComboBox* pCombo = (cComboBox*)WINDOWMGR->GetWindowForIDEx(MT_LISTCOMBOBOX);
		OPTIONMGR->GetGameOption()->nLoginCombo = pCombo->GetCurSelectedIdx();
		OPTIONMGR->SetGameOption(OPTIONMGR->GetGameOption());
		msg.gate = (BYTE)OPTIONMGR->GetGameOption()->nLoginCombo;


		if( msg.gate == 3 )
		{
			HINTERNET hInternet, hHttp, hReq;
			DWORD dwRead = 0;
			char strResult[1024] = { 0, };
			char strRequest[256] = { 0, };
			DWORD Size = 0;

			hInternet = InternetOpen("BUDDY", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
			if( !hInternet )
			{
				char msg[256] = { 0, };
				sprintf( msg, "%s (004)", CHATMGR->GetChatMsg(1645) );
				cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, msg );
				cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
				if( pIDDlg && pMsgBox )
					pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
				return;
			}
			hHttp = InternetConnect( hInternet, "www.google.com", 0, "", "", INTERNET_SERVICE_HTTP, 0, 0 );
			if( !hHttp )
			{
				char msg[256] = { 0, };
				sprintf( msg, "%s (005)", CHATMGR->GetChatMsg(1645) );
				cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, msg );
				cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
				if( pIDDlg && pMsgBox )
					pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
				return;
			}
			sprintf( strRequest, "Auth/AuthUsrInfoChk.aspx?USRID=%s&USRPWD=%s", msg.id, msg.pw );
			hReq = HttpOpenRequest( hHttp, "POST", strRequest, NULL, NULL, NULL, 0, 0 );
			HttpSendRequest( hReq, NULL, 0, NULL, 0 );
			InternetQueryDataAvailable( hReq, &Size, 0, 0 );
			InternetReadFile( hReq, strResult, Size, &dwRead );
			InternetCloseHandle( hHttp );
			InternetCloseHandle( hInternet );
			hHttp = NULL;			
			hInternet = NULL;

			if( strResult[0] != '0' )
			{
				char msg[256] = { 0, };
				sprintf( msg, "%s (001)", CHATMGR->GetChatMsg(1645) );
				cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, msg );
				cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
				if( pIDDlg && pMsgBox )
					pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
				return;
			}
			else
			{
				MSG_LOGIN_SYN_BUDDY msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_LOGIN_SYN_BUDDY;
				SafeStrCpy(msg.id, userID, MAX_NAME_LENGTH + 1 );
				SafeStrCpy(msg.pw, userPWD, MAX_NAME_LENGTH + 1 );
				SafeStrCpy(msg.Version,g_CLIENTVERSION, 16 );
				SafeStrCpy(msg.AuthCode, strResult, strlen(strResult)+1 );
				msg.gate = (BYTE)OPTIONMGR->GetGameOption()->nLoginCombo;
				msg.AuthKey = TITLE->GetDistAuthKey();
				NETWORK->Send(&msg,sizeof(msg));
			}
		}
		else
#endif
		{
		msg.AuthKey = TITLE->GetDistAuthKey();
		NETWORK->Send(&msg,sizeof(msg));
		}
		
		SafeStrCpy(g_szHeroIDName, userID, MAX_NAME_LENGTH + 1 );

		cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_WAIT_LOGINCHECK, MBT_CANCEL, CHATMGR->GetChatMsg( 291 ) );
		cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
		if( pIDDlg && pMsgBox )
		{
			pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
		}


		TITLE->StartWaitConnectToAgent( TRUE );
		
		USERINFOMGR->SetUserID( userID );
	}
	else
	{
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_LOGINEDITCHECK, MBT_OK, CHATMGR->GetChatMsg(362) ); 
		cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
		if( pIDDlg && pMsgBox )
		{
			pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
		}
	}

	editboxId->SetFocusEdit(FALSE);
	editboxPwd->SetFocusEdit(FALSE);
	dlg->SetDisable( TRUE );
//	dlg->SetActive( FALSE );
//#endif
}

void MT_DynamicBtnFunc(LONG lId, void * p, DWORD we)
{
	cDialog* dlg = (cDialog*)p;

	if( lId == MT_DYNAMICOKBTN )
	{
		cEditBox* editboxPwd = (cEditBox*)dlg->GetWindowForID(MT_DYNAMICPWDEDITBOX);
		char* userPWD = editboxPwd->GetEditText();
		if( strcmp(userPWD, "") != 0 )
		{
			MSG_LOGIN_DYNAMIC_SYN msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_LOGIN_DYNAMIC_SYN;
			msg.AuthKey = TITLE->GetDistAuthKey();
			SafeStrCpy( msg.id, g_szHeroIDName, MAX_NAME_LENGTH + 1 );
			SafeStrCpy( msg.pw, userPWD, MAX_NAME_LENGTH + 1 );
			NETWORK->Send( &msg, sizeof(msg) );

			cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID( MBI_WAIT_LOGINCHECK );
			if( pMsgBox )
			{
				pMsgBox->SetDisable( FALSE );
				pMsgBox->SetActive( TRUE );
				cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
				if( pIDDlg )
				{
					pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
				}
			}

			TITLE->StartWaitConnectToAgent( TRUE );
		}
		else
		{
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_CANTUSEDYNAMIC, MBT_OK, CHATMGR->GetChatMsg(362) ); 
			cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( pIDDlg && pMsgBox )
			{
				pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
			}
		}
		
		editboxPwd->SetFocusEdit(FALSE);
		dlg->SetDisable( TRUE );
	}
	else if( lId == MT_DYNAMICEXITBTN )
	{
		((cEditBox*)dlg->GetWindowForID(MT_DYNAMICPWDEDITBOX))->SetEditText( "" );

		dlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
		((cEditBox*)dlg->GetWindowForID(MT_IDEDITBOX))->SetEditText( "" );
		((cEditBox*)dlg->GetWindowForID(MT_PWDEDITBOX))->SetEditText( "" );

		TITLE->NoDisconMsg();
		TITLE->ShowServerList();
	}
}

void MT_DynamicEditReturnFunc( LONG lId, void* p, DWORD we )
{
	if(we == WE_RETURN)
	{
		cDialog* dlg = (cDialog*)p;		
		if( lId == MT_DYNAMICPWDEDITBOX )
		{
			 MT_DynamicBtnFunc( MT_DYNAMICOKBTN, p, we );
		}
	}
}

void MT_EditReturnFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_RETURN)
	{
		cDialog* dlg = (cDialog*)p;
		cEditBox* pEdit;
		if( lId == MT_IDEDITBOX )
		{
			 pEdit = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
			 if( *pEdit->GetEditText() != 0 )
			 {
				 WINDOWMGR->SetNextEditFocus();
			 }
		}
		else if( lId == MT_PWDEDITBOX )
		{
			pEdit = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);
			if( *pEdit->GetEditText() != 0 )
			{
				MT_LogInOkBtnFunc( MT_OKBTN, p, WE_BTNCLICK );
			}
		}
	}
}

void MT_ExitBtnFunc(LONG lId, void * p, DWORD we)
{
//	MAINGAME->SetGameState(eGAMESTATE_END);	

	cDialog* dlg = (cDialog *)p;
	cEditBox* editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
	cEditBox* editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);	
/*
#ifdef TAIWAN_LOCAL
	
	editboxId->SetDisable( FALSE );			//¾ÆAIμðA￠ E°¼ºE­

	dlg->GetWindowForID( MT_BTN_DYNAMIC )->SetActive( TRUE );
	((cCheckBox*)dlg->GetWindowForID( MT_BTN_DYNAMIC ))->SetChecked( FALSE );

	dlg->GetWindowForID( MT_STC_DYNAMIC )->SetActive( TRUE );
	dlg->GetWindowForID( MT_STC_COORD )->SetActive( FALSE );
	dlg->GetWindowForID( MT_STC_COORD_MAT )->SetActive( FALSE );

	((cButton*)dlg->GetWindowForID( MT_OKBTN ))->SetText( RESRCMGR->GetMsg(25), RGB_HALF(255,255,255), RGB_HALF(255,255,0), RGB_HALF(255,255,0) ); //login

#endif
*/
	editboxId->SetEditText("");
	editboxPwd->SetEditText("");	
	editboxId->SetFocusEdit(TRUE);
	editboxPwd->SetFocusEdit(FALSE);

	GAMEIN->GetNumberPadDlg()->InitProtectionStr();

//￠®E?￠®¡×I¡E￠c¡§ua￠®¡×u¡E￠c¡§I ￠®¡×u¡E￠c¡§I¡§Ioo¡E￠c¡E￠￥I ¡§Io¡E￠c¡§￠®￠®¡×ui￠®¡×￠®ⓒ­￠®¡×u¡E￠c¡§ui?
//￠®¡×u¡E￠c¡§I¡§Ioo￠®E?￠®E¡Ec￠®¡×u¡E￠c¡§I ¡§I￠®"￠c(ui￠®Ii￠®Ii ￠®IiE￠®E￠®IU¡E￠c¡§￠®i CI￠®E￠®ⓒ­e ¡§I￠®"￠c!¡§ua
//¡§I￠®"￠cAA￠®E￠®ⓒ­￠®E￠®ⓒ­e ￠®¡×uE￠®IiC￠®E￠®IA ¡E￠cioE¡§I￠®AA￠®¡×￠®ⓒ­ AU￠®¡×oAA¡E￠ci ¡E￠c¡§￠®¡§I¡§¡I¡E￠c¡§￠®YA￠®E￠®ⓒ­¡E￠c¡§￠®UA￠®E￠®ⓒ­¡E￠c¡E￠￥I CI￠®E￠®IA ￠®E￠®ⓒ­o￠®¡×o￠®¡×￠®ⓒ­AI¡E￠c¡§￠®￠®E¡Ec AOA¡E￠ci ￠®EO¡E￠c￠®￠?
//A¡E￠c￠®￠?CeCO￠®EO¡E￠c￠®￠? ¡§I￠®"￠c(ui¡§Ioo￠®E￠®ⓒ­￠®Ec￠®E￠®IA ¡E￠cioE¡§I￠®AA¡E￠ci ￠®¡×u¡§I¡§¡I￠®¡×uO¡E￠c¡§ua A¡E￠c￠®￠?C￠®¡×¡§¡I￠®¡×u¡E￠c¡§I.

	TITLE->NoDisconMsg();
	TITLE->ShowServerList();
}

////////////////////////////////////// Main Interface //////////////////////////////////////////
void MI_CharBtnFunc(LONG lId, void* p, DWORD we)
{
//	cWindow * wnd = WINDOWMGR->CreateAlertDlg("¡§I¨I¨￡¡E￠ci￠®¡×u￠®¡×¡E￠c￠®E￠®ⓒ­|¡§I¨I¨￡￠®¡×￠®I￠®¡×u￠®¡×¡E￠c￠®Ii￠®Ii~", AB_YESNO);
//	wnd->SetActive(TRUE);
	//wnd->SetAbsXY(0,0);

	//MessageBox(NULL,"A¡§I¨I¨￡￠®E￠®ⓒ­?A¡E￠cE Aa¡E￠c¡E￠￥A",NULL,NULL);
	if(we == WE_PUSHUP)
	{
		GAMEIN->GetCharacterDialog()->SetActive(FALSE);
	//	GAMEIN->GetMainInterfaceDialog()->GetPushupBtn(2)->SetPush(FALSE);
	}
	else if(we == WE_PUSHDOWN)
	{
		GAMEIN->GetCharacterDialog()->UpdateData();
		GAMEIN->GetCharacterDialog()->SetActive(TRUE);
	//	GAMEIN->GetMainInterfaceDialog()->GetPushupBtn(2)->SetPush(TRUE);
	}
}
void MI_ExchangeBtnFunc(LONG lId, void* p, DWORD we)
{
	//MessageBox(NULL,"¡E￠c¡§u¡§I¨I¨￡E?A¡E￠cE Aa¡E￠c¡E￠￥A",NULL,NULL);
	//AO￠®¡×oA
	static BOOL run = 0;
	if(run)
	{
		MSGBASE msg;
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_WALKMODE;
		msg.dwObjectID = HEROID;
		NETWORK->Send(&msg,sizeof(msg));
		run = 0;
		MOVEMGR->SetWalkMode(HERO);
	}
	else
	{
		MSGBASE msg;
		msg.Category = MP_MOVE;
		msg.Protocol = MP_MOVE_RUNMODE;
		msg.dwObjectID = HEROID;
		NETWORK->Send(&msg,sizeof(msg));
		run = 1;
		MOVEMGR->SetRunMode(HERO);
	}
}

void MI_InventoryBtnFunc(LONG lId, void* p, DWORD we)
{
	if(we == WE_PUSHUP)
	{
		GAMEIN->GetInventoryDialog()->SetActive(FALSE);
	//	GAMEIN->GetDealDialog()->HideDealer();
	//	GAMEIN->GetInventoryDialog()->HideInventory();
	//	GAMEIN->GetWearedItemDialog()->HideWearedItem();
	}
	else if(we == WE_PUSHDOWN)
	{
		GAMEIN->GetInventoryDialog()->SetActive(TRUE);
	//	GAMEIN->GetDealDialog()->ShowDealer(0);
	//	GAMEIN->GetInventoryDialog()->ShowInventory(0);
	//	GAMEIN->GetWearedItemDialog()->ShowWearedItem();
	}
}

void MI_MugongBtnFunc(LONG lId, void* p, DWORD we)
{
	//MessageBox(NULL,"¡§Io¡E￠ci¡E￠c¡§￠®¡§I¡§¡IA¡E￠cE Aa¡E￠c¡E￠￥A",NULL,NULL);
	if(we == WE_PUSHUP)
	{
		GAMEIN->GetMugongSuryunDialog()->SetActive(FALSE);
	}
	else if(we == WE_PUSHDOWN)
	{
		GAMEIN->GetMugongSuryunDialog()->SetActive(TRUE);
	}
		
}
/*
//￠®¡×¡E￠cA￠®¡×¡E￠c￠®¡×uA¡E￠cE
void MI_PartyBtnFunc(LONG lId, void * p, DWORD we) 
{
	if(we == WE_PUSHUP) 
	{
		GAMEIN->GetPartyDialog()->SetActive(FALSE);
	}
	else if(we == WE_PUSHDOWN) 
	{
		GAMEIN->GetPartyDialog()->SetActive(TRUE);
		GAMEIN->GetPartyDialog()->RefreshDlg();
	} 
}
*/
void IN_DlgFunc(LONG lId, void * p, DWORD we)
{
//	if(we == WE_CLOSEWINDOW)
//	{
//		GAMEIN->GetInventoryDialog()->SetActive(FALSE);
//	}
	if(we == WE_LBTNCLICK ||/* we == WE_RBTNCLICK || */we == WE_RBTNDBLCLICK || we == WE_LBTNDBLCLICK || we == WE_RBTNDBLCLICK)
	{
		GAMEIN->GetInventoryDialog()->OnActionEvnet(lId, p, we);
	}
	if(lId == IN_SHOPITEMBTN)
		GAMEIN->GetInventoryDialog()->OnShopItemBtn();
	else if(lId == IN_TABBTN1 || lId == IN_TABBTN2 || lId == IN_TABBTN3 || lId == IN_TABBTN4)
		GAMEIN->GetInventoryDialog()->OffShopItemBtn(lId);		
}
/*Mugong Dialog Interface*/
void MGI_DlgFunc(LONG lId, void * p, DWORD we)
{
#ifdef _JAPAN_LOCAL_
#elif defined _HK_LOCAL_
#elif defined _TL_LOCAL_
#else
	if(we == WE_PUSHDOWN || we == WE_PUSHUP)
	{
		GAMEIN->GetMugongDialog()->ActiveMogongGrid( lId );
	}
#endif
}
void MGI_MugongDlgFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_LBTNCLICK || /*we == WE_RBTNCLICK ||*/ we == WE_RBTNDBLCLICK || we == WE_LBTNDBLCLICK)
	{
		GAMEIN->GetMugongDialog()->OnActionEvnet(lId, p, we);
	}
}
void MGI_SuryunDlgFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_BTNCLICK && lId == MGI_SURYUN_BTN)
	{
		GAMEIN->GetSuryunDialog()->LearnAbilitySyn();
	}

	if(we == WE_BTNCLICK && lId == MGI_SKPOINTREDIST_BTN)
	{
		if( HERO->GetState() != eObjectState_None && HERO->GetState() != eObjectState_Immortal )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(734) );
			return;
		}
		
		GAMEIN->GetMugongSuryunDialog()->SetActive( FALSE );
		GAMEIN->GetSkPointDlg()->SetActive( TRUE );
		OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
	}

	if(we == WE_LBTNCLICK || /*we == WE_RBTNCLICK ||*/ we == WE_RBTNDBLCLICK || we == WE_LBTNDBLCLICK)
	{
		GAMEIN->GetSuryunDialog()->OnActionEvent(lId, p, we);
	}
}
/*character infomation interface*/
void CI_DlgFunc(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK && lId == CI_BESTTIP )
	{
		GAMEIN->GetTipBrowserDlg()->Show();
	}
//	if(we == WE_CLOSEWINDOW)
//	{
//		GAMEIN->GetMainInterfaceDialog()->GetPushupBtn(2)->SetPush(FALSE);
//	}
}
void CI_AddExpPoint(LONG lId, void * p, DWORD we)
{
	BYTE whats=GENGOL_POINT;
	switch(lId)
	{
	case CI_GENGOL_POINT:
	case CI_GENGOL_POINT2:
		whats=GENGOL_POINT;
		break;
	case CI_SIMMEK_POINT:
	case CI_SIMMEK_POINT2:
		whats=SIMMEK_POINT;
		break;
	case CI_MINCHUB_POINT:
	case CI_MINCHUB_POINT2:
		whats=MINCHUB_POINT;
		break;
	case CI_CHERYUK_POINT:
	case CI_CHERYUK_POINT2:
		whats=CHERYUK_POINT;
		break;
	}
	
	if( lId==CI_GENGOL_POINT || lId==CI_SIMMEK_POINT || lId==CI_MINCHUB_POINT || lId==CI_CHERYUK_POINT )
		GAMEIN->GetCharacterDialog()->OnAddPoint(whats);
	else
		GAMEIN->GetCharacterDialog()->OnMinusPoint(whats);
}
/*party dialog interface */
void PA_DlgFunc(LONG lId, void * p, DWORD we)
{
//	if(we == WE_CLOSEWINDOW)
//	{
	//	GAMEIN->GetMainInterfaceDialog()->GetPushupBtn(4)->SetPush(FALSE);
//	}
}

//////////// A￠®E¡E¡I￠®¡×¡E￠cA A¡E￠cE ////////////////////////////////////////
void MI_ChatEditBoxFunc(LONG lId, void * p, DWORD we)			//00000
{
// KES 030820 A￠®E¡E¡I￠®¡×¡E￠cA￠®E￠®ⓒ­￠®¡×￠®¨I￠®E￠®IIA￠®Ec¡E￠c¡E￠￥I ￠®E?A¡E￠c¡§￠®U ¡E￠c¡§￠®¡E￠c¨I¡ⓒA￠®¡×o
/*
	if(we == WE_RETURN)
	{
		cDialog * dlg = (cDialog *)p;
		cEditBox * editbox = (cEditBox *)dlg->GetWindowForID(MI_CHATEDITBOX);
		char * msg = editbox->GetEditText();
		
		if( strlen(msg) != 0 )
		{
			BOOL bSend = TRUE;
			if(msg[0] == '/')	//¡E￠c¡§uO￠®E￠®ⓒ­¡E￠ciA¡§I¨I¨￡￠®E￠®ⓒ­￠®EcC￠®¡×¡§¡I￠®¡×u¡§I￠®ⓒ­CO
			{
				if(strlen(msg) == 1)
					bSend = FALSE;
				else
					bSend = CheatFunc(&msg[1]);
			}
			
			if(bSend)
			{
				TESTMSG data;
				data.Category = MP_CHAT;
				data.Protocol = MP_CHAT_ALL;
				data.dwObjectID	= HEROID;
				//strcpy(data.Msg, FILTERTABLE->Filetering(msg));
				strcpy(data.Msg, msg);
				NETWORK->Send(&data,sizeof(data));
			}
			
			editbox->SetEditText("");
		}
	}
*/
}



void IN_CreateStreetStallFunc(LONG lId, void * p, DWORD we)
{
//	GAMEIN->GetStreetStallDialog()->ShowSellStall();
}

void SSI_TitleEditBoxFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_RETURN)
	{
		// A|￠®E￠®ⓒ­n￠®E?￠®E¡Ec ￠®E￠®IeCN ￠®E?¡§Io￠®E?UA¡§I¨I¨￡￠®E￠®ⓒ­￠®Ec CO¡E￠c¡§￠®I..
		STREETSTALLMGR->EditTitle();
	}
	else if( we == WE_SETFOCUSON )
	{
		GAMEIN->GetStreetStallDialog()->OnTitleEditClick();
	}
}

void SSI_MoneyEditBoxFunc(LONG lId, void * p, DWORD we)
{
/*
	if(we == WE_RETURN)
	{
		if( GAMEIN->GetStreetStallDialog()->SelectedItemCheck() ) return;
		
		if( GAMEIN->GetStreetStallDialog()->MoneyEditCheck() )
		{
			GAMEIN->GetStreetStallDialog()->RegistMoney();
			STREETSTALLMGR->ItemStatus( TRUE );
		}
	}
	else if( we == WE_SETFOCUSON )
	{
		GAMEIN->GetStreetStallDialog()->OnMoneyEditClick();
	}
*/
}

void SSI_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetStreetStallDialog()->OnActionEvnet(lId, p, we);
}

void SSI_BuyBtnFunc(LONG lId, void * p, DWORD we)
{
	CStreetStall* pStallDlg = GAMEIN->GetStreetStallDialog();

	if( pStallDlg->GetCurSelectedItemNum() == -1 ) return;

	if( we == WE_BTNCLICK )
	{

		if( !pStallDlg->SelectedItemCheck() ) return;

// FOR FIND_SS_BUG
		if( ITEMMGR->IsDupItem( pStallDlg->GetCurSelectedItemIdx() ) ) // ￠®E?￠®Ii￠®¡×uaAI¡§I¨I¨￡￠®¡×￠®I ￠®¡×¡E￠c?¡E￠cie¡§Io¡E￠c¡§￠® ￠®¡×u￠®¡×¡E￠cAIAUAI ¡E￠c¡§￠®¡§I¡E￠c￠®E?i
		{
			if( pStallDlg->GetCurSelectedItemDur() == 1 )
				STREETSTALLMGR->BuyItem();
			else
				GAMEIN->GetStreetStallDialog()->ShowDivideBox();
		}
		else
		{
			STREETSTALLMGR->BuyItem();
		}
	}
}

void SSI_RegistBtnFunc(LONG lId, void * p, DWORD we)
{
/*
	if( GAMEIN->GetStreetStallDialog()->GetCurSelectedItemNum() == -1 ) return;

	if( we == WE_BTNCLICK )
	{
		if( GAMEIN->GetStreetStallDialog()->SelectedItemCheck() ) return;

		if( GAMEIN->GetStreetStallDialog()->MoneyEditCheck() )
		{
			GAMEIN->GetStreetStallDialog()->RegistMoney();
			STREETSTALLMGR->ItemStatus( TRUE );
		}
	}
*/
}

void SSI_EditBtnFunc(LONG lId, void * p, DWORD we)
{
	if( GAMEIN->GetStreetStallDialog()->GetCurSelectedItemNum() == -1 ) return;

	CStreetStall* pStall = (CStreetStall*)p;
	if(we == WE_BTNCLICK)
	{
		
		CExchangeItem* pExItem = pStall->FindItem( pStall->GetCheckDBIdx() );
		if( pExItem == NULL ) return;

//		if( !GAMEIN->GetStreetStallDialog()->SelectedItemCheck() ) return;
		//락이 걸려있나 안있나만 보자..
		if( pExItem->IsLocked() == TRUE ) return;

		STREETSTALLMGR->ItemStatus( pExItem, FALSE );
	}
}

// LYJ 051017 구입노점상 추가
void SO_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetStallKindSelectDialog()->OnActionEvnet(lId, p, we);
}

void BS_TitleEditBoxFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_RETURN)
	{
		// A|￠®E￠®ⓒ­n￠®E?￠®E¡Ec ￠®E￠®IeCN ￠®E?¡§Io￠®E?UA¡§I¨I¨￡￠®E￠®ⓒ­￠®Ec CO¡E￠c¡§￠®I..
		STREETSTALLMGR->EditTitle();
	}
	else if( we == WE_SETFOCUSON )
	{
		GAMEIN->GetStreetBuyStallDialog()->OnTitleEditClick();
	}
}

void BS_MoneyEditBoxFunc(LONG lId, void * p, DWORD we)
{
}

void BS_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetStreetBuyStallDialog()->OnActionEvnet(lId, p, we);
}

void BS_SellBtnFunc(LONG lId, void * p, DWORD we)
{
	CStreetBuyStall* pStallBuyDlg = GAMEIN->GetStreetBuyStallDialog();

	if( pStallBuyDlg->GetCurSelectedItemNum() == -1 ) return;

	if( we == WE_BTNCLICK )
	{

		if( !pStallBuyDlg->SelectedItemCheck() ) return;

		STREETSTALLMGR->BuyItem();
	}
}

void BS_REGBtnFunc(LONG lId, void * p, DWORD we)
{
}

void BS_SPFunc(LONG lId, void * p, DWORD we)
{
}

void BRS_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetBuyRegDialog()->OnActionEvnet(lId, p, we);
}

void BRS_CloseFunc(LONG lId, void * p, DWORD we)
{
/*	CBuyItem* pItem = (CBuyItem*)GAMEIN->GetStreetBuyStallDialog()->GetCurSelectedItem();
	POSTYPE pos = GAMEIN->GetStreetBuyStallDialog()->GetCurSelectedItemNum();
	
	//선택된 곳에 아이템이 있었다면 삭제한다
	if( pos != -1 && pItem )
		GAMEIN->GetStreetBuyStallDialog()->FakeDeleteItem(pos);

*/	
	MSGBASE msg;
	msg.Category = MP_STREETSTALL;
	msg.dwObjectID = HEROID;
	msg.Protocol = MP_STREETSTALL_UPDATEEND_SYN;
	
	NETWORK->Send( &msg, sizeof(MSGBASE) );
	
	GAMEIN->GetBuyRegDialog()->Close();
}

void BRS_DeleteFunc(LONG lId, void * p, DWORD we)
{
	CBuyItem* pItem = (CBuyItem*)GAMEIN->GetStreetBuyStallDialog()->GetCurSelectedItem();
	POSTYPE pos = GAMEIN->GetStreetBuyStallDialog()->GetCurSelectedItemNum();
	
	//선택된 곳에 아이템이 있었다면 삭제한다
	if( pos != -1 && pItem )
		GAMEIN->GetStreetBuyStallDialog()->FakeDeleteItem(pos);
	
	GAMEIN->GetBuyRegDialog()->Close();
}


void BRS_TypeListFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetBuyRegDialog()->UpdateType();
}

void BRS_ItemListFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetBuyRegDialog()->UpdateItem();
}

void BRS_ClassListFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetBuyRegDialog()->UpdateClass();
}

void BRS_REGBtnFunc(LONG lId, void * p, DWORD we)
{
	ITEM_INFO* pItem = GAMEIN->GetBuyRegDialog()->GetSelectItemInfo();

	if(!pItem)
		return;

	BUY_REG_INFO RegInfo;
	
	if( !GAMEIN->GetBuyRegDialog()->GetBuyRegInfo(RegInfo) )
		return;
	
	GAMEIN->GetStreetBuyStallDialog()->FakeRegistItem(RegInfo, pItem);
	GAMEIN->GetBuyRegDialog()->Close();
}

void BRS_VolumeEditFunc(LONG lId, void * p, DWORD we)
{

}

void BRS_MoneyEditFunc(LONG lId, void * p, DWORD we)
{

}

//////////////////////////////////////////////////////////////////////////
// 노점 검색 다이얼로그 이벤트 함수 by Stiner(8)
void SFR_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetStallFindDlg()->OnActionEvent(lId, p, we);
}
//////////////////////////////////////////////////////////////////////////

// LBS 03.10.20 Npc￠®E￠®Ie¡E￠cicA¡E￠cE
void NSI_DlgFunc(LONG lId, void * p, DWORD we)
{
//	if( we == WE_CLOSEWINDOW )
//	{
//		GAMEIN->GetNpcScriptDialog()->EndDialog();
//	}
}

void NSI_HyperLinkFunc(LONG lId, void * p, DWORD we)
{
	cListDialogEx* pListDlg = (cListDialogEx*)((cDialog*)p)->GetWindowForID( lId );
	int nIdx = pListDlg->GetCurSelectedRowIdx();
	if( nIdx == -1 ) return;

	GAMEIN->GetNpcScriptDialog()->HyperLinkParser( nIdx );
	WINDOWMGR->SetMouseInputProcessed();	
}

// LBS 03.10.30 HelpA¡E￠cE
// LBS 03.10.20 Npc￠®E￠®Ie¡E￠cicA¡E￠cE
void HI_DlgFunc(LONG lId, void * p, DWORD we)
{
	if( we == WE_CLOSEWINDOW )
	{
		GAMEIN->GetHelpDialog()->EndDialog();
	}
}

void HI_HyperLinkFunc(LONG lId, void * p, DWORD we)
{
//	int nIdx = ((cListDialogEx*)p)->GetCurSelectedRowIdx();
//	if( nIdx == -1 ) return;
	cListDialogEx* pListDlg = (cListDialogEx*)((cDialog*)p)->GetWindowForID( lId );
	int nIdx = pListDlg->GetCurSelectedRowIdx();
	if( nIdx == -1 ) return;

	GAMEIN->GetHelpDialog()->HyperLinkParser( nIdx );
}

void HI_BtnFunc(LONG lId, void * p, DWORD we)
{
	if( we == WE_BTNCLICK )
	{
		if( lId == HI_EXPLANBTN )
		{
			GAMEIN->GetHelpDialog()->OpenLinkPage( 2 );
		}
		else if( lId == HI_HANDLEBTN )
		{
			GAMEIN->GetHelpDialog()->OpenLinkPage( 31 );
		}
		else if( lId == HI_REARBTN )
		{
			GAMEIN->GetHelpDialog()->OpenLinkPage( 51 );
		}
		else if( lId == HI_ETCBTN )
		{
			GAMEIN->GetHelpDialog()->OpenLinkPage( 81 );
		}
#ifndef _JAPAN_LOCAL_
		else if( lId == HI_QnABTN )
		{
#ifdef TAIWAN_LOCAL
			ShellExecute( NULL, NULL, "Iexplore.exe", "http://www.moxiang.com.cn", NULL, SW_SHOWNORMAL );
#elif defined _HK_LOCAL_

#ifdef _TW_LOCAL_
			ShellExecute( NULL, NULL, "Iexplore.exe", "http://ms.runup.com.tw/faq.php", NULL, SW_SHOWNORMAL );
#else
			ShellExecute( NULL, NULL, "Iexplore.exe", "http://ms.runup.com.hk/faq.php", NULL, SW_SHOWNORMAL );
#endif

#elif defined _TL_LOCAL_
			ShellExecute( NULL, NULL, "Iexplore.exe", "http://www.darkstory.in.th/phpbb/index.php", NULL, SW_SHOWNORMAL );
#else
			ShellExecute( NULL, NULL, "Iexplore.exe", "http://www.darkstoryonline.com/LOGIN.asp?url=/customer_zone/QA/default.asp", NULL, SW_SHOWNORMAL );	
#endif
		}
#endif //_JAPAN_LOCAL_
	}
}

void HSI_ExitBtnFunc(LONG lId, void * p, DWORD we)
{
	HELPERMGR->GetHelperDlg()->CloseDialog();
}


void CMI_MoneyOkFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetMoneyDialog()->OkPushed();
}
void CMI_MoneySpinFunc(LONG lId, void * p, DWORD we)
{
//	int a=0;
}
void CMI_AlertFunc(LONG lId, void * p, DWORD we)
{
	/*
	CAlertDlg * dlg = (CAlertDlg *)p;
	if(we == 1)
	{

		// yes
		cIcon * icon = (cIcon *)dlg->GetObj();
		if(icon)
		{
			if(icon->GetType() == WT_MUGONG)
			{
				CMugongBase * mugong = (CMugongBase *)icon;
				GAMEIN->GetMugongDialog()->FakeDeleteMugong(mugong->GetPosition(), mugong->GetDBIdx());
			}
		}
	}
	_postDestroyCommonDlg(lId, p, we);
	*/
}

/*¡E￠cioA￠®E¡EcA¡E￠cE*/
void DE_DlgFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_CLOSEWINDOW)
	{
		CDealDialog* pDlg = GAMEIN->GetDealDialog();

		if( pDlg && pDlg->IsShow() )
		{
			pDlg->SetActive(FALSE);
		}
	}
	
	if(we == WE_LBTNCLICK)
	{
		GAMEIN->GetDealDialog()->OnSelectedItem();
	}
	else if(/*we == WE_LBTNCLICK || we == WE_RBTNCLICK || we == WE_RBTNDBLCLICK ||*/ we == WE_LBTNDBLCLICK)
	{
		GAMEIN->GetDealDialog()->OnActionEvnet(lId, p, we);
	}
}
void DE_BuyItemFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetDealDialog()->OnBuyPushed();
}
void DE_SellItemFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetDealDialog()->OnSellPushed();
}
void DE_CloseHideBtn(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetDealDialog()->HideDealer();
}

//////////////////////////////////////////////////////////////////////////
//*￠®¡×¡E￠cA￠®¡×¡E￠c￠®¡×uA¡E￠cE*/
void PA_BtnFunc(LONG lId, void * p, DWORD we) 
{
	if(GAMEIN->GetGameInInitKind() == eGameInInitKind_EventMapEnter)
		return;

	switch(lId) 
	{		
	case PA_SECEDEBTN: //A¡E￠ciA¡§I￠®I 
		{

			if(PARTYMGR->IsProcessing() == TRUE)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1726) );
				return;
			}

			// partywar
			if( PARTYWAR->IsPartyWar() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(885));
				return;
			}

			if( HEROID != PARTYMGR->GetMasterID() )
				WINDOWMGR->MsgBox( MBI_PARTY_SECEDE, MBT_YESNO, CHATMGR->GetChatMsg(60));		
		}	
		break;
		
	case PA_FORCEDSECEDEBTN: //¡E￠c¡§￠®¡E￠c¡§IA|A¡E￠ciA¡§I￠®I
		{
			if( HEROID != PARTYMGR->GetMasterID() )	return;

			if(PARTYMGR->IsProcessing() == TRUE)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1726) );
				return;
			}

			// partywar
			if( PARTYWAR->IsPartyWar() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(885));
				return;
			}

			DWORD DeleteID = GAMEIN->GetPartyDialog()->GetClickedMemberID();
			if(DeleteID == 0)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(250));
				return;
			}
			PARTYMGR->BanPartyMemberSyn(DeleteID);
		}
		break;
		
	case PA_TRANSFERBTN: //￠®E￠®ⓒ­￠®Ec￠®E￠®Io¡E￠c¡§uC AI￠®¡×uc
		{
			if( HEROID != PARTYMGR->GetMasterID() )	return;

			if(PARTYMGR->IsProcessing() == TRUE)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1726) );
				return;
			}

			// partywar
			if( PARTYWAR->IsPartyWar() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(885));
				return;
			}

			DWORD NewMasterID = GAMEIN->GetPartyDialog()->GetClickedMemberID();
			if(NewMasterID == 0)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(250));
				return;
			}
			PARTYMGR->ChangeMasterPartySyn(HEROID, NewMasterID);
		}
		break;
		
	case PA_BREAKUPBTN: //C￠®¡×¡§¡I¡E￠cie
		{
			if( HEROID != PARTYMGR->GetMasterID() )	return;

			if(PARTYMGR->IsProcessing() == TRUE)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1726) );
				return;
			}

			// partywar
			if( PARTYWAR->IsPartyWar() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(885));
				return;
			}

			if(PARTYMGR->IsProcessing() == TRUE)
				return;

			WINDOWMGR->MsgBox(MBI_PARTYBREAKUP, MBT_YESNO, CHATMGR->GetChatMsg(301));
		}
		break;
		
	case PA_ADDPARTYMEMBERBTN: //￠®E￠®ⓒ­a¡§Ioo A¡§I￠®ⓒ­¡E￠c¡§￠®￠®E¡Ec
		{
			if( HEROID != PARTYMGR->GetMasterID() )	return;

			if(PARTYMGR->IsProcessing() == TRUE)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1726) );
				return;
			}
			
			// partywar
			if( PARTYWAR->IsPartyWar() )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(885));
				return;
			}

			CObject* pObject = OBJECTMGR->GetSelectedObject();
			////////////////////////////////////////////////////////////////////////////////
			// 06. 06. 2차 전직 - 이영준
			// 은신 / 혜안
			// 은신 패널티
			if( pObject )
			if( pObject->GetSingleSpecialState(eSingleSpecialState_Hide) )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1329));
				return;
			}
			////////////////////////////////////////////////////////////////////////////////
			if(!pObject)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(18));
				return;
			}
			else 
			{
				if(pObject->GetObjectKind() == eObjectKind_Player)
					PARTYMGR->AddPartyMemberSyn(OBJECTMGR->GetSelectedObject()->GetID());
				else
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(18));
			}
		}
		break;		
/*
		//2008. 5. 21. CBH - 방파 생성 버튼 이벤트 함수 따로 추가
	case PA_CREATEOK:
		{
			GAMEIN->GetPartyCreateDialog()->SetActive(FALSE);
			cComboBox* pDistirbute = (cComboBox*)WINDOWMGR->GetWindowForIDEx(PA_CREATECOMBOX);
			char buff[16];
			BYTE Opt;
			strcpy(buff,pDistirbute->GetComboText());
			
			CMD_ST(buff)
			CMD_CS( RESRCMGR->GetMsg(483) )
			Opt = ePartyOpt_Random;
			CMD_CS( RESRCMGR->GetMsg(484) )
			Opt = ePartyOpt_Damage;
			CMD_EN

			PARTYMGR->CreatePartySyn(Opt);
		}
		break;
	case PA_CREATECANCEL:
		{
			GAMEIN->GetPartyCreateDialog()->SetActive(FALSE);
		}
		break;
*/
	case PA_INVITEOK:
		{
			GAMEIN->GetPartyInviteDialog()->SetActive(FALSE);
			PARTYMGR->PartyInviteAccept();
		}
		break;
	case PA_INVITECANCEL:
		{
			PARTYMGR->PartyInviteDeny();
			GAMEIN->GetPartyInviteDialog()->SetActive(FALSE);
		}
		break;
	case PA_WARSUGGESTBTN:
		{
			if( HEROID != PARTYMGR->GetMasterID() )	return;

			PARTYWAR->SendPartyWarSuggest();
		}
		break;
	case PA_OPTIONBTN:
		{
			GAMEIN->GetPartyDialog()->ShowOption();
		}
		break;
	case PA_MEMBERBTN:
		{
			GAMEIN->GetPartyDialog()->ShowMember();
		}
		break;
	}
}
//￠®¡×¡E￠cA￠®¡×¡E￠c￠®¡×u
//////////////////////////////////////////////////////////////////////////



// ￠®E￠®IUAaA¡E￠cE ¡E￠c¡§￠®u¡E￠c¡E￠￥A
void QI_QuickDlgFunc(LONG lId, void * p, DWORD we)
{
	if(lId == QI_PREVPAGEBTN || lId == QI_NEXTPAGEBTN)
		GAMEIN->GetQuickDialog()->OnActionEvent(lId, p, we);
}

void PYO_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	switch(lId) 
	{
	case PYO_PYOGUKLISTBTN:
		{
			GAMEIN->GetPyogukDialog()->SetPyogukListInfo();
			GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_PyogukListInfo);
			GAMEIN->GetPyogukDialog()->AddPyogukMode(ePyogukMode_PyogukListInfo);
			if(GAMEIN->GetGuildDlg()->IsActive() == TRUE)
				GAMEIN->GetGuildDlg()->SetActive(FALSE);
			GAMEIN->GetPyogukDialog()->SetActive(TRUE);
		}
		break;
	case PYO_BUYPYOGUKBTN:
		{
			if(HERO->GetPyogukNum() == MAX_PYOGUKLIST_NUM)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(308) );
				return;
			}
			
#ifdef _JAPAN_LOCAL_

			int rt = GAMEIN->GetPyogukDialog()->CanBuyPyoguk();

			if( rt != 0 )
			{
				if( rt == 1 )	//돈없다
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(50) );
				}
				else if( rt == 2 ) //창고 꽉찼다
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(308) );
				}
				return;
			}
#endif
#ifdef _HK_LOCAL_
			int rt = GAMEIN->GetPyogukDialog()->CanBuyPyoguk();
			if( rt != 0 )
			{
				if( rt == 1 )	//돈없다
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(50) );
				}
				else if( rt == 2 ) //창고 꽉찼다
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(308) );
				}
				return;
			}
#endif
#ifdef _TL_LOCAL_
			int rt = GAMEIN->GetPyogukDialog()->CanBuyPyoguk();

			if( rt != 0 )
			{
				if( rt == 1 )	//돈없다
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(50) );
				}
				else if( rt == 2 ) //창고 꽉찼다
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(308) );
				}
				return;
			}
#endif

			WINDOWMGR->MsgBox( MBI_PYOGUKBUY, MBT_YESNO, CHATMGR->GetChatMsg( 196 ), 
				HERO->GetPyogukNum()+1, AddComma( GAMEIN->GetPyogukDialog()->GetPrice(HERO->GetPyogukNum() ) ) );
		}
		break;

	case PYO_PYOGUK1BTN: case PYO_PYOGUK2BTN: case PYO_PYOGUK3BTN:
	case PYO_PYOGUK4BTN: case PYO_PYOGUK5BTN:
		{
			int num = lId-PYO_PYOGUK1BTN;
			CPyogukDialog* pDlg = (CPyogukDialog*)GAMEIN->GetPyogukDialog();
			if( !pDlg )
			{
				ASSERT(0);
				return;
			}

			cPushupButton* ppyogukbtn[TAB_PYOGUK_NUM];

#ifdef _JAPAN_LOCAL_
			if( HERO->GetPyogukNum() == 0 && HERO->GetCharacterTotalInfo()->ExtraPyogukSlot == 0 ) 
#elif defined _HK_LOCAL_
			if( HERO->GetPyogukNum() == 0 && HERO->GetCharacterTotalInfo()->ExtraPyogukSlot == 0 ) 
#elif defined _TL_LOCAL_
			if( HERO->GetPyogukNum() == 0 && HERO->GetCharacterTotalInfo()->ExtraPyogukSlot == 0 ) 
#else
			if( HERO->GetPyogukNum() == 0 )
#endif
			{
				for(int i=0; i<TAB_PYOGUK_NUM; ++i)
				{
					ppyogukbtn[i] = (cPushupButton*)pDlg->GetWindowForID(PYO_PYOGUK1BTN+i);
					ppyogukbtn[i]->SetPush(FALSE);
				}

				return;
			}
			for(int i=0; i<TAB_PYOGUK_NUM; ++i)
			{
				ppyogukbtn[i] = (cPushupButton*)pDlg->GetWindowForID(PYO_PYOGUK1BTN+i);
				if(i==num)
				{
					ppyogukbtn[i]->SetPush(TRUE);
				}
				else
				{
					ppyogukbtn[i]->SetPush(FALSE);
				}
			}
			// ¡§Ioo￠®¡×¡E￠c¡E￠c¡§￠®￠®E?￠®E¡Ec ￠®Iiu￠®EOo ￠®¡×￠®ⓒ­￠®E￠®ⓒ­￠®E?￠®¡×IAo￠®E￠®IA ￠®E￠®ⓒ­¡§I￠®I￠®¡×oA￠®E￠®ⓒ­￠®E￠®ⓒ­ ￠®¡×￠®ⓒ­?E¡E￠c¡§ICN￠®E￠®IU.
			GAMEIN->GetPyogukDialog()->ShowPyogukMode(ePyogukMode_PyogukWare1+num);
			GAMEIN->GetPyogukDialog()->AddPyogukMode(ePyogukMode_PyogukWare1+num);
		}
		break;

	case PYO_PUTINMONEYBTN:
		{
			PYOGUKMGR->PutInOutMoney(1);
		}
		break;
	case PYO_PUTOUTMONEYBTN:
		{
			PYOGUKMGR->PutInOutMoney(0);
		}
		break;
	case PYO_NOBUYBTN: case PYO_NOBUYPYOGUKBTN:
		{
			GAMEIN->GetPyogukDialog()->SetActive(FALSE);
			GAMEIN->GetInventoryDialog()->SetActive(FALSE);
		}
		break;
	}
}


void Friend_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	switch(lId) 
	{
	case FRI_PAGE1BTN: case FRI_PAGE2BTN: case FRI_PAGE3BTN: 
		{
			GAMEIN->GetFriendDialog()->SetFriendList(lId - FRI_PAGE1BTN + 1);
		}
		break;
	case FRI_ADDFRIENDBTN:
		{
			if(GAMEIN->GetMiniFriendDialog()->IsActive())
			{
				GAMEIN->GetMiniFriendDialog()->SetActive(FALSE);
			}
			else
			{
				GAMEIN->GetMiniFriendDialog()->SetActive(TRUE);
				char Name[MAX_NAME_LENGTH+1] = {0};
				CObject* pTarget = OBJECTMGR->GetSelectedObject();
				if(pTarget && (pTarget->GetObjectKind() == eObjectKind_Player))
				{
					//strcpy(Name, OBJECTMGR->GetSelectedObject()->GetObjectName());
					SafeStrCpy(Name, OBJECTMGR->GetSelectedObject()->GetObjectName(), MAX_NAME_LENGTH+1);
					GAMEIN->GetMiniFriendDialog()->SetName(Name);
				}
			}
		}
		break;
	case FRI_DELFRIENDBTN:
		{
			GAMEIN->GetFriendDialog()->CheckedDelFriend();
			//￠®E￠®ⓒ­￠®Ec￠®¡×i¨Io￠®Ei¡§u￠®¡×¡E￠c￠®Ec AA￠®¡×¡E￠c￠®Ec¡E￠c¡E￠￥N￠®E?￠®E¡Ec￠®¡×u¡E￠c¡§I? 
			//	char Name[MAX_NAME_LENGTH+1] = GAMEIN->GetFriendDialog()->GetSelectedFriendID();
			//	FRIENDMGR->AddDelFriendSyn(, MP_FRIEND_DEL_SYN);
		}
		break;
	case FRI_ADDOKBTN:
		{		
			cEditBox * pName = (cEditBox *)WINDOWMGR->GetWindowForIDEx(FRI_NAMEEDIT);
			char code[MAX_NAME_LENGTH+1] = {0,};			
			sscanf(pName->GetEditText(),"%s",code);
			strupr(code);
			if(strcmp("",code) == 0)
				return;
			char heroname[MAX_NAME_LENGTH+1] = {0,};
			sscanf(HERO->GetObjectName(), "%s", heroname);
			strupr(heroname);
			if(strcmp(heroname, code) == 0)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(676) );
				return;
			}				
			FRIENDMGR->AddDelFriendSyn(pName->GetEditText(), MP_FRIEND_ADD_SYN);
			GAMEIN->GetMiniFriendDialog()->SetActive(FALSE);
		}
		break;
	case FRI_ADDCANCELBTN:
		{
			GAMEIN->GetMiniFriendDialog()->SetActive(FALSE);
		}
		break;
	case FRI_SENDNOTEBTN:
		{
			if(strcmp(GAMEIN->GetFriendDialog()->GetChkedName(), "") == 0)
				return;
			GAMEIN->GetMiniNoteDialog()->SetActive(TRUE);
			GAMEIN->GetMiniNoteDialog()->SetMiniNote(GAMEIN->GetFriendDialog()->GetChkedName(), "", 0);
			GAMEIN->GetMiniNoteDialog()->ShowMiniNoteMode(eMinNoteMode_Write);
			
		}
		break;
	case FRI_SENDWHISPERBTN:
		{
			char temp[MAX_NAME_LENGTH+5];
			if(strcmp(GAMEIN->GetFriendDialog()->GetChkedName(), "") == 0)
				return;
			sprintf(temp,"/%s ",GAMEIN->GetFriendDialog()->GetChkedName());
			cEditBox* pChatBox = GAMEIN->GetChatDialog()->GetChatEditBox();
			pChatBox->SetFocusEdit(TRUE);
			pChatBox->SetEditText(temp);
		}
		break;
	}
}

void Note_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	switch(lId) 
	{
	case NOTE_WRITENOTEBTN:
		{
			if(GAMEIN->GetMiniNoteDialog()->IsActive())
			{
				if(GAMEIN->GetMiniNoteDialog()->GetCurMode() == eMinNoteMode_Read)
					GAMEIN->GetMiniNoteDialog()->ShowMiniNoteMode(eMinNoteMode_Write);
				else
					GAMEIN->GetMiniNoteDialog()->SetActive(FALSE);
			}
			else
			{
				GAMEIN->GetMiniNoteDialog()->SetActive(TRUE);
				GAMEIN->GetMiniNoteDialog()->ShowMiniNoteMode(eMinNoteMode_Write);			
			}
		}
		break;
	case NOTE_DELNOTEBTN:
		{
			GAMEIN->GetNoteDialog()->CheckedNoteID();
		}
		break;

	case NOTE_PAGE1BTN:
	case NOTE_PAGE2BTN:
	case NOTE_PAGE3BTN:
	case NOTE_PAGE4BTN:
	case NOTE_PAGE5BTN:
		{
			NOTEMGR->NoteListSyn( lId - NOTE_PAGE1BTN +1, GAMEIN->GetNoteDialog()->GetMode() );
		}
		break;
	//////////////////////////////////////////////////////////////////////////
	//////MINI NOTE
	case NOTE_MSENDOKBTN:
		{
			cEditBox * pReceiver = (cEditBox *)WINDOWMGR->GetWindowForIDEx(NOTE_MRECEIVEREDIT);
			char pname[MAX_NAME_LENGTH+1] = {0};
			SafeStrCpy(pname, pReceiver->GetEditText(), MAX_NAME_LENGTH+1);
			if(strlen(pname) == 0)
				return;

			cTextArea * pNote = (cTextArea *)WINDOWMGR->GetWindowForIDEx(NOTE_MWNOTETEXTREA);
			char FullText[200];
			pNote->GetScriptText(FullText);
			int len = strlen(FullText);
			if(len >= MAX_NOTE_LENGTH)
			{
				len = MAX_NOTE_LENGTH -1;
			}
			char Note[MAX_NOTE_LENGTH];
			strncpy(Note, FullText, len);
			Note[len] = 0;
			NOTEMGR->WriteNote(Note,  pname);
			GAMEIN->GetMiniNoteDialog()->SetActive(FALSE);			
		}
		break;
	case NOTE_MSENDCANCELBTN:
		{
			GAMEIN->GetMiniNoteDialog()->SetActive(FALSE);
		}
		break;
	case NOTE_MREPLYBTN:
		{
			char ToName[MAX_NAME_LENGTH+1];
			strcpy(ToName, GAMEIN->GetMiniNoteDialog()->GetSenderName());
			if(strlen(ToName) == 0 )
				return;
			
			GAMEIN->GetMiniNoteDialog()->SetMiniNote(ToName, "", 0);
			GAMEIN->GetMiniNoteDialog()->ShowMiniNoteMode(eMinNoteMode_Write);

		}
		break;
	case NOTE_MDELETEBTN:
		{
			NOTEMGR->DelNoteSyn(GAMEIN->GetMiniNoteDialog()->GetNoteID(),TRUE);
		}
		break;
	case NOTE_TABBTN1:
		{
			GAMEIN->GetNoteDialog()->SetMode( eMode_NormalNote );
			NOTEMGR->NoteListSyn( 1, eMode_NormalNote );
		}
		break;
	case NOTE_TABBTN2:
		{
			GAMEIN->GetNoteDialog()->SetMode( eMode_PresentNote );
			NOTEMGR->NoteListSyn( 1, eMode_PresentNote );
		}
		break;
	case NOTE_NOTELISTCHK11:
		{
			GAMEIN->GetNoteDialog()->SetChkAll();
		}
		break;
	}
}

void CR_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	switch(lId) 
	{
	case CR_PRESENTSPOT:
		{
			if( EXITMGR->IsExitWaiting() )	//종료중엔 부활 안됨
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(643) );
				GAMEIN->GetReviveDialog()->SetActive( FALSE );
				return;
			}

			MSGBASE msg;
			msg.Category = MP_CHAR_REVIVE;
			msg.Protocol = MP_CHAR_REVIVE_PRESENTSPOT_SYN;
			msg.dwObjectID = HEROID;
			NETWORK->Send(&msg, sizeof(msg));

			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_REVIVECONFIRM );
			if( pDlg )
			{
				WINDOWMGR->AddListDestroyWindow( pDlg );
				ITEMMGR->ReviveOtherConfirm( FALSE );
			}
		}
		break;
	case CR_LOGINSPOT:
		{
			if( EXITMGR->IsExitWaiting() )	//종료중엔 부활 안됨
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(643) );
				GAMEIN->GetReviveDialog()->SetActive( FALSE );
				return;
			}
			
			MSGBASE msg;
			msg.Category = MP_CHAR_REVIVE;
			msg.Protocol = MP_CHAR_REVIVE_LOGINSPOT_SYN;
			msg.dwObjectID = HEROID;			
			NETWORK->Send(&msg, sizeof(msg));	

			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_REVIVECONFIRM );
			if( pDlg )
			{
				WINDOWMGR->AddListDestroyWindow( pDlg );
				ITEMMGR->ReviveOtherConfirm( FALSE );
			}
		}
		break;
	case CR_TOWNSPOT:
		{
			if( EXITMGR->IsExitWaiting() )	//종료중엔 부활 안됨
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(643) );
				GAMEIN->GetReviveDialog()->SetActive( FALSE );
				return;
			}

			MSGBASE msg;
			msg.Category = MP_CHAR_REVIVE;
			msg.Protocol = MP_CHAR_REVIVE_VILLAGESPOT_SYN;
			msg.dwObjectID = HEROID;
			NETWORK->Send(&msg, sizeof(msg));

			cDialog* pDlg = WINDOWMGR->GetWindowForID( MBI_REVIVECONFIRM );
			if( pDlg )
			{
				WINDOWMGR->AddListDestroyWindow( pDlg );
				ITEMMGR->ReviveOtherConfirm( FALSE );
			}
		}
		break;
	}
	GAMEIN->GetReviveDialog()->SetDisable(TRUE);
}

//KES
/*Macro Dialog Interface*/
void MAC_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetMacroDialog()->OnActionEvent(lId, p, we);
}

//void MAC_ModeDlgFunc(LONG lId, void * p, DWORD we)
//{
//	GAMEIN->GetMacroModeDialog()->OnActionEvent(lId, p, we);
//}

/*Chat Dialog Interface*/
void CTI_DlgFunc(LONG lId, void * p, DWORD we)
{
	if( lId == CHA_CREATEOKBTN )
		GAMEIN->GetShoutDlg()->SendShoutMsgSyn();
	else
		GAMEIN->GetChatDialog()->OnActionEvent(lId, p, we);
}


void COI_DlgFunc(LONG lId, void * p, DWORD we)
{
//	GAMEIN->GetChatOptionDialog()->OnActionEvent(lId, p, we);
}

void ITEM_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetMallNoticeDialog()->OnActionEvent(lId, p, we);
}

/*Option Dialog Interface*/
void OTI_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetOptionDialog()->OnActionEvent(lId, p, we);
}

void EXT_DlgFunc(LONG lId, void * p, DWORD we)
{
	//A¡E￠cAA¡E￠cE¡E￠c¡§u￠®E￠®ⓒ­Co
	if( we == WE_BTNCLICK )
	{
		if( lId == EXT_BTN_EXITGAME )
		{
			GAMEIN->GetExitDialog()->SetActive( FALSE );
			WINDOWMGR->MsgBox( MBI_EXIT, MBT_YESNO, CHATMGR->GetChatMsg( 280 ) );
		}		
#ifndef _CHINA_LOCAL_ //중국은 캐릭터 선택창으로 가는 메뉴를 없에고 처리도 막는다
		else if( lId == EXT_BTN_SELECTCHAR )
		{
			GAMEIN->GetExitDialog()->SetActive( FALSE );
			int nErrorCode;
			if( ( nErrorCode = EXITMGR->CanExit() ) == eEXITCODE_OK )
			{
				EXITMGR->SendExitMsg( eEK_CHARSEL );
			}
			else
			{
				switch( nErrorCode )
				{
				case eEXITCODE_PKMODE:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(552) );	// "￠®ii￠®¨ua¡E¡þ| ¡Iia￠®￠´?¨Iⓒª¡§o ￠®ioAA¡E?¡E￠c¡§u￠®¨I¡E¡IA ~￠®￠´aCO ¡§uo ¡§u¨I¨￡¡§o4I¡E¡IU."
					break;
				case eEXITCODE_LOOTING:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(553) );	//"￠®￠´c¡§￠®A; ¡E¡IcCI¡E¡IA A¨I¡þ¡E?¡E￠c¡E¡IA ~￠®￠´aCO ¡§uo ¡§u¨I¨￡¡§o4I¡E¡IU."
					break;
				default:
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(554) );	//"~￠®￠´aCO ¡§uo ¡§u¨I¨￡¡E¡IA ￠®ioAAAO¡E¡II¡E¡IU."
					break;
				}
			}
/*
			MSGBASE data;
			data.Category	= MP_USERCONN;
			data.Protocol	= MP_USERCONN_BACKTOCHARSEL_SYN;
			data.dwObjectID	= HEROID;
			NETWORK->Send(&data,sizeof(data));
			
			///////-------
			
			WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg( 290 ) );
			//heroA￠®E¡E¡IAoCE￠®E?aCN¡E￠c¡§￠®￠®E¡Ec?
			HERO->DisableAutoAttack();					//AU￠®Ii￠®E? ¡E￠c¡§￠®¡§I¡§¡I¡E￠c¡§￠®Y Ae￠®¡×uO
			HERO->SetNextAction(NULL);					//￠®¡×o￠®¡×￠®ⓒ­A¡§I¨I¨￡ Ae￠®¡×uO
			MOVEMGR->HeroMoveStop();
			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Enter );	//AO￠®¡×oA
*/
		}
#endif //_CHINA_LOCAL_ //중국은 캐릭터 선택창으로 가는 메뉴를 없에고 처리도 막는다
	}

}

void XCI_DlgFunc( LONG lId, void* p, DWORD we )
{
	GAMEIN->GetExchangeDialog()->OnActionEvent(lId, p, we);
}

void CSS_DlgFunc( LONG lId, void* p, DWORD we )
{
	GAMEIN->GetCharStateDialog()->OnActionEvent(lId, p, we);
}

void MSI_MenuSlotDlgFunc( LONG lId, void* p, DWORD we )
{
	GAMEIN->GetMenuSlotDialog()->OnActionEvent(lId, p, we);
}
/*PKLOOTING*/
void PLI_DlgFunc( LONG lId, void* p, DWORD we )
{
	if( p )
	{
		if( ((cDialog*)p)->GetType() == WT_PKLOOTINGDLG )
		{
			((CPKLootingDialog*)p)->OnActionEvent( lId, p, we );
		}
		else
		{
			int a = 0;
		}
	}
	//GAMEIN->GetPKLootingDialog()->OnActionEvent(lId, p, we);
	//pk manager￠®E?￠®E¡Ec￠®¡×u¡E￠c¡§I A¡§I¨I¨￡￠®E￠®ⓒ­￠®EcC￠®¡×¡§¡IAO￠®¡×ui￠®¡×u¡§I￠®ⓒ­ CO¡E￠c¡§￠®I ¡E￠c¡§￠®¡E￠c¡§￠®￠®E￠®IU.... ￠®¡×ui￠®E￠®IA ¡E￠c¡§￠®IA¡E￠ci E¡E￠cIAaC￠®¡×¡§¡IAUAo￠®E￠®ⓒ­|.
	//PKMGR->
}

void CNA_BtnOkFunc( LONG lId, void* p, DWORD we )
{
	if( p )
	{
		((cDialog*)p)->SetActive( FALSE );
	}
	TITLE->SetServerList();
}

void DIS_DlgFunc( LONG lId, void* p, DWORD we )
{
	GAMEIN->GetDissolutionDialog()->OnActionEvent( lId, p, we );
}

void GAMEIN_ChatFunc(LONG lId, void * p, DWORD we)
{
//	if( we == eIMEE_INSERTTEXT )
//	{
//		char* str = ((cEditBox*)p)->GetEditText();

//		if( strlen( str ) == 1 )
//		if( *str == '/' )
//		{
//			CHATMGR->SetAutoWhisper();
//		}
//	}
	if( we == eIMEE_SETFOCUS )
	{
		if( CHATMGR->GetOption()->bOption[CTO_AUTOHIDECHAT] )
			GAMEIN->GetChatDialog()->HideChatDialog( FALSE );
	}
	else if( we == eIMEE_KILLFOCUS )
	{
		if( CHATMGR->GetOption()->bOption[CTO_AUTOHIDECHAT] )
			GAMEIN->GetChatDialog()->HideChatDialog( TRUE );
	}
}
/*
void MAINTITLE_IDFunc(LONG lId, void * p, DWORD we)
{
	//lId -- editAC AO￠®¡×uO	//p -- editAC ￠®¡×¡E￠c¡E￠cAAIAI
	cEditBox* pEdit = (cEditBox*)p;

	if( we == eIMEE_ENTER )
	{
		if( *pEdit->GetEditText() != 0 )
		{
			WINDOWMGR->SetNextEditFocus();
		}
	}
}

void MAINTITLE_PWFunc(LONG lId, void * p, DWORD we)
{
	cEditBox* pEdit = (cEditBox*)p;

	if( we == eIMEE_ENTER )
	{
		if( *pEdit->GetEditText() != 0 )
		{
			cDialog* pDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
				
			MT_LogInOkBtnFunc( MT_OKBTN, pDlg, WE_BTNCLICK );
		}
	}
}
*/
/////////////
//---MurimNet
void MNPRI_DlgFunc(LONG lId, void * p, DWORD we)
{
	CMNStgPlayRoom::GetInterface()->OnActionEvent( lId, p, we );
}

void MNPRI_ChatFunc( LONG lId, void * p, DWORD we )
{
	cEditBox* pEdit = (cEditBox*)p;

	if( strlen( pEdit->GetEditText() ) == 0 ) return;
	
	TESTMSGID msg;
	msg.Category	= MP_MURIMNET;
	msg.Protocol	= MP_MURIMNET_CHAT_ALL;
	msg.dwObjectID	= MURIMNET->GetMNPlayerManager()->GetMNHeroID();
	msg.dwSenderID	= msg.dwObjectID;
	strcpy( msg.Msg, pEdit->GetEditText() );

//	NETWORK->Send( &msg, sizeof(msg) );
	NETWORK->Send( &msg, msg.GetMsgLength() ); //CHATMSG 040324
	
	pEdit->SetEditText("");
}

void MNCNL_DlgFunc(LONG lId, void * p, DWORD we)
{
	CMNStgChannel::GetInterface()->OnActionEvent( lId, p, we );
}


void MNCNL_ChatFunc(LONG lId, void * p, DWORD we)
{
	cEditBox* pEdit = (cEditBox*)p;

	if( strlen( pEdit->GetEditText() ) == 0 ) return;
	
	TESTMSGID msg;
	msg.Category	= MP_MURIMNET;
	msg.Protocol	= MP_MURIMNET_CHAT_ALL;
	msg.dwObjectID	= MURIMNET->GetMNPlayerManager()->GetMNHeroID();
	msg.dwSenderID	= msg.dwObjectID;
	strcpy( msg.Msg, pEdit->GetEditText() );

	//NETWORK->Send( &msg, sizeof(msg) );
	NETWORK->Send( &msg, msg.GetMsgLength() );	//CHATMSG 040324
	
	pEdit->SetEditText("");
}


void DIVIDEBOX_Func(LONG lId, void * p, DWORD we)
{
	if( we == WE_RETURN )
	{
		((cDivideBox*)p)->ExcuteDBFunc( WE_RETURN );		
	}
}

//KES MESSAGEBOX
//￠®E?￠®¡×I¡E￠c¡§ua￠®¡×u¡E￠c¡§I ￠®¡×u￠®¡×¡E￠cAI￠®Ii¡§I￠®I￠®E￠®ⓒ­￠®E￠®ⓒ­ ¡E￠c¡§u￠®E￠®ⓒ­￠®¡×￠®ⓒ­￠®¡×￠®EC￠®¡×¡§¡IAO￠®E￠®ⓒ­e.....
void MessageBox_Func( LONG lId, void * p, DWORD we )
{
	switch( lId )
	{
	case MBI_EXIT:
		{
			if( we == MBI_YES )
			{
//				MSGBASE data;		//agent¡E￠c¡§￠®￠®E¡Ec ￠®¡×uE￠®¡×u￠®¡×¡E￠c￠®¡×u¡E￠c¡§I ￠®¡×￠®ⓒ­￠®E￠®ⓒ­¡§I¨I¨￡¡E￠ciAU
//				data.Category	= MP_USERCONN;
//				data.Protocol	= MP_USERCONN_DISCONNECTED;
//				data.dwObjectID	= HEROID;
//				NETWORK->Send(&data,sizeof(data));
				//￠®¡×u¡E￠c¡§I¡§Ioo¡E￠c¡E￠￥I ￠®E￠®ⓒ­￠®¡×￠®¨I￠®¡×oAAo¡E￠c¡§￠®￠®E¡Ec A¡§I￠®ⓒ­ ￠®Ii￠®IiA¡§I¡§¡ICI¡E￠c¡§￠®i￠®¡×u¡E￠c¡§I ¡§I¨I¨￡￠®E¡Ec¡§I¨I¨￡￠®¡×￠®I￠®E￠®IA ¡E￠c¡§￠®CAo..?

				if( MAINGAME->GetCurStateNum() != eGAMESTATE_GAMEIN )
				{
//					MAINGAME->SetGameState( eGAMESTATE_END );
					PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
					break;
				}

				int nErrorCode;
				if( ( nErrorCode = EXITMGR->CanExit() ) == eEXITCODE_OK )
				{
					EXITMGR->SendExitMsg( eEK_GAMEEXIT );
				}
				else
				{
					switch( nErrorCode )
					{
					case eEXITCODE_PKMODE:
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(552) );	// "￠®ii￠®¨ua¡E¡þ| ¡Iia￠®￠´?¨Iⓒª¡§o ￠®ioAA¡E?¡E￠c¡§u￠®¨I¡E¡IA ~￠®￠´aCO ¡§uo ¡§u¨I¨￡¡§o4I¡E¡IU."
						break;
					case eEXITCODE_LOOTING:
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(553) );	//"￠®￠´c¡§￠®A; ¡E¡IcCI¡E¡IA A¨I¡þ¡E?¡E￠c¡E¡IA ~￠®￠´aCO ¡§uo ¡§u¨I¨￡¡§o4I¡E¡IU."
						break;
					default:
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(554) );	//"~￠®￠´aCO ¡§uo ¡§u¨I¨￡¡E¡IA ￠®ioAAAO¡E¡II¡E¡IU."
						break;
					}
				}
			}
		}
		break;

	case MBI_PASSERROR:
		{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
 			cEditBox* pEditId = (cEditBox*)WINDOWMGR->GetWindowForIDEx( MT_IDEDITBOX );
			cEditBox* pEditPwd = (cEditBox*)WINDOWMGR->GetWindowForIDEx( MT_PWDEDITBOX );
			if( pEditId )
				pEditId->SetFocusEdit( FALSE );
			if( pEditPwd )
			{
				pEditPwd->SetEditText( "" );
				pEditPwd->SetFocusEdit( TRUE );
			}
			cDialog* pDlg1 = (cDialog*)WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			pDlg1->SetDisable( FALSE );

			cDialog* pDlg2 = (cDialog*)WINDOWMGR->GetWindowForID( MT_DYNAMICDLG );

			if( pDlg2 )
			if( pDlg2->IsActive() )				
			{
				pDlg1->SetActive( TRUE );
				pDlg2->SetDisable( FALSE );
				pDlg2->SetActive( FALSE	);
				pEditPwd = (cEditBox*)WINDOWMGR->GetWindowForIDEx( MT_DYNAMICPWDEDITBOX );
				pEditPwd->SetEditText( "" );
//				pEditPwd->SetFocusEdit( TRUE );
			}
		}
		break;
	case MBI_OTHERUSER_CONNECTTRY:
		{
			
		}
		break;

	case MBI_DISCONNECTED_OVERLAPPEDLOGIN:
		{
			//MAINGAME->SetGameState(eGAMESTATE_END);
			PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
		}
		break;
	case MBI_PARTYBREAKUP:
		{
			if(we == MBI_YES)
			{
				PARTYMGR->BreakUpSyn();
			}
		}
		break;
	case MBI_PYOGUKBUY:
		{
			if(we == MBI_YES)
			{
				PYOGUKMGR->BuyPyogukSyn();
			}
		}
		break;
	case MBI_MUGONGDELETE:
	case MBI_MUGONGDELETE2:
	case MBI_MUGONGDELETE3:
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetMugongDialog()->SendDeleteMugong();
				AUDIOMGR->Play(67, HERO);
			}
			else if( we == MBI_NO )
			{
				GAMEIN->GetMugongDialog()->SetDisable( FALSE );
			}
		}
		break;
// LBS A¡§I￠®ⓒ­¡E￠c¡§￠®￠®E¡Ec 03.09.17 (¡§I¨I¨￡eA￠®E¡Ec¡E￠cio ¡E￠c¡§￠®u¡E￠c¡E￠￥A)
	case MBI_BUYREG:
		{

		}
		break;
	case MBI_MAKESTREETSTALL:
		{
			if(we == MBI_YES)
			{
				STREETSTALLMGR->OpenStreetStall();
			}
			else if( we == MBI_NO )
			{
				STREETSTALLMGR->SetOpenMsgBox(TRUE);
			}
		}
		break;

	case MBI_DELETECHAR:
		{
			if( we == MBI_YES )
			{
				CHARSELECT->DeleteCharacter();
			}
			else
			{
				CHARSELECT->SetDisablePick( FALSE );
			}
		}
		break;
		
	case MBI_EXCHANGEAPPLY:
		{
			if(we == MBI_YES)
			{
				EXCHANGEMGR->AcceptExchange( TRUE );
			}
			else if( we == MBI_NO )
			{
				EXCHANGEMGR->AcceptExchange( FALSE );
			}
		}
		break;
		
	case MBI_EXCHANGEWAITING:
		{
			if(we == MBI_CANCEL)
			{
				EXCHANGEMGR->CancelApply();				
			}
		}
		break;
		
	case MBI_SELECTLOGINPOINT:
		{
			if(we == MBI_YES)
			{
				cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID( MBI_SELECTLOGINPOINT );				
				CNpc* pNpc = (CNpc*)OBJECTMGR->GetObject( pMsgBox->GetParam() );
				if( pNpc )
				{
					g_UserInput.GetHeroMove()->SelectLoginPoint( pNpc->GetNpcUniqueIdx() );
				}
				else
				{
					//error ¡E￠c¡E￠￥I¡E￠c¡§u¡E￠c¡E?AI￠®¡×¡E￠c¡E￠cAAI￠®¡×¡E￠c￠®Ec￠®E￠®ⓒ­| AuAaCO ￠®¡×uo ￠®¡×u¡§I¡§¡I￠®¡×oA￠®E￠®II￠®E￠®IU.
				}

				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			}
		}
		break;
		
	case MBI_LOGINEDITCHECK:
		{

			if( we == MBI_OK )
			{
				cEditBox * pIdEdit =(cEditBox *) WINDOWMGR->GetWindowForIDEx(MT_IDEDITBOX); // pjs ID￠®E?￠®E¡Ec ￠®¡×¡E￠c¡E￠cAA￠®E?￠®¡×o￠®¡×￠®ⓒ­ [5/20/2003]
				cEditBox * pPwEdit =(cEditBox *) WINDOWMGR->GetWindowForIDEx(MT_PWDEDITBOX); // pjs ID￠®E?￠®E¡Ec ￠®¡×¡E￠c¡E￠cAA￠®E?￠®¡×o￠®¡×￠®ⓒ­ [5/20/2003]

				if( *pIdEdit->GetEditText() == 0 )
					pIdEdit->SetFocusEdit(TRUE);
				else
					pPwEdit->SetFocusEdit(TRUE);
			}
			cDialog* pDlg= (cDialog*)WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			pDlg->SetDisable( FALSE );
/*
#ifdef TAIWAN_LOCAL
			cCheckBox* pCheckBox = (cCheckBox*)pDlg->GetWindowForID( MT_BTN_DYNAMIC );
			if( !pCheckBox ) break;
			if( pCheckBox->IsActive() && pCheckBox->IsChecked() )
				pDlg->GetWindowForID( MT_PWDEDITBOX )->SetDisable( TRUE );
#endif
*/
		}
		break;
	case MBI_DELETEICONCHECK:
		{
			if( we == MBI_YES )
			{
				ITEMMGR->SendDeleteItem();
				AUDIOMGR->Play(67, HERO);
			}
			else if( we == MBI_NO )
			{
				GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
				GAMEIN->GetPyogukDialog()->SetDisable( FALSE );
				GAMEIN->GetGuildDlg()->SetDisable( FALSE );
			}
		}
		break;

	case MBI_FRIEND_ADD:
		{
			if(we == MBI_YES)
			{
				FRIENDMGR->FriendInviteAccept();
				
			}
			else if( we == MBI_NO )
			{
				FRIENDMGR->FriendInviteDeny();
			}
		}
		break;
	case MBI_VIMUAPPLY:
		{
			if(we == MBI_YES)
			{
				VIMUMGR->AcceptVimu( TRUE );
			}
			else if( we == MBI_NO )
			{
				VIMUMGR->AcceptVimu( FALSE );
			}
		}
		break;
	case MBI_VIMUWAITING:
		{
			if(we == MBI_CANCEL)
			{
				VIMUMGR->CancelApply();
			}
		}
		break;
	case MBI_CHARSELERROR:
		{
			CHARSELECT->SetDisablePick( FALSE );
		}
		break;
	case MBI_CHARMAKEERROR:
		{
			CHARMAKE->SetDisableDlg( FALSE );
		}
		break;
	case MBI_PARTY_SECEDE:
		{
			if(we == MBI_YES)
			{
				PARTYMGR->DelPartyMemberSyn();
			}
		}
		break;
	case MBI_MAPCHANGE:
		{
			GAMEIN->GetMoveDialog()->SetItemToMapServer( 0, 0, FALSE );
			
			if(we == MBI_YES)
			{
				cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID( MBI_MAPCHANGE );
				CNpc* pNpc = (CNpc*)OBJECTMGR->GetObject( pMsgBox->GetParam() );
				if( pNpc )
				{
					if( !HERO->IsPKMode() && !PKMGR->IsPKLooted() && !HERO->IsDied() )	//·cÆA´cCI°i AOA≫¶§, A×¾uA≫¶§μμ AIμ¿ ¸øCI°O ¸·A½.
						g_UserInput.GetHeroMove()->MapChange( pNpc->GetNpcUniqueIdx() );
				}
				else
				{
					//error ￠®E￠®ⓒ­EAI￠®Ii￠®E?A¡E￠ci CO ￠®¡×uo ￠®¡×u¡§I¡§¡I￠®¡×oA￠®E￠®II￠®E￠®IU.
				}
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			}
			else if( we == MBI_NO )
			{
				// Server￠®E?￠®E¡Ec ¡E￠cioAA￠®E￠®ⓒ­| ￠®¡×uE￠®E￠®ⓒ­¡E￠c¡§￠®￠®E￠®IU.
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
			}
		}
		break;
	case MBI_OVERLAPPEDLOGININOTHERSERVER:
		{
			cDialog* dlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( dlg )
			{
				dlg->SetDisable( FALSE );
				cEditBox* editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
				cEditBox* editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);
				if( editboxId )
				{
					editboxId->SetEditText("");
					editboxId->SetFocusEdit(TRUE);
				}
				if( editboxPwd )
				{
					editboxPwd->SetEditText("");
					editboxPwd->SetFocusEdit(FALSE);
				}
			}
		}
		break;
	case MBI_OVERLAPPEDLOGIN:
		{			
			if(we == MBI_YES)
			{
				MSGBASE msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_FORCE_DISCONNECT_OVERLAPLOGIN;
				NETWORK->Send(&msg,sizeof(msg));
				
				cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_LOGINERRORCLOSE, MBT_OK, CHATMGR->GetChatMsg(4) );
				cDialog* pIDDlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
				if( pIDDlg && pMsgBox )
				{
					pMsgBox->SetAbsXY( pIDDlg->GetAbsX(), pIDDlg->GetAbsY() + pIDDlg->GetHeight() );
				}
			}
			else if( we == MBI_NO )
			{
				cDialog* dlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
				if( dlg )
				{
					dlg->SetDisable( FALSE );
					cEditBox* editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
					cEditBox* editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);
					if( editboxId )
					{
						editboxId->SetEditText("");
						editboxId->SetFocusEdit(TRUE);
					}
					if( editboxPwd )
					{
						editboxPwd->SetEditText("");
						editboxPwd->SetFocusEdit(FALSE);
					}
				}				
			}
		}
		break;
	case MBI_SEND_DISCONNECT_OVERLAPPEDLOGIN:
		{

		}
		break;
	case MBI_LOGINERRORCLOSE:
		{
//			MAINGAME->SetGameState(eGAMESTATE_END);
			cDialog* dlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( dlg )
			{
				dlg->SetDisable( FALSE );
				cEditBox* editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
				if( editboxId )
				{
					editboxId->SetEditText("");
					editboxId->SetFocusEdit(TRUE);
				}
				cEditBox* editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);
				if( editboxPwd )
				{
					editboxPwd->SetEditText("");
					editboxPwd->SetFocusEdit(FALSE);
				}
			}
			
			TITLE->NoDisconMsg();		//
			TITLE->ShowServerList();
		}
		break;
	case MBI_CANTUSEDYNAMIC:
		{
			cDialog* pDlg = WINDOWMGR->GetWindowForID( MT_DYNAMICDLG );
			if( pDlg )
			{
				pDlg->SetDisable( FALSE );
				((cEditBox*)pDlg->GetWindowForID( MT_DYNAMICPWDEDITBOX ))->SetEditText( "" );
				((cEditBox*)pDlg->GetWindowForID( MT_DYNAMICPWDEDITBOX ))->SetFocusEdit( TRUE );
			}			
		}
		break;
	case MBI_SERVERDISCONNECT:
		{
//			MAINGAME->SetGameState(eGAMESTATE_END);
			PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
		}
		break;
	case MBI_DISTCONNECT_ERROR:
		{
			WINDOWMGR->CloseAllMsgBoxNoFunc();
			TITLE->ShowServerList();
		}
		break;
	case MBI_MAPSERVER_CLOSE:
		{
			CHARSELECT->SetDisablePick( FALSE );
		}
		break;
	case MBI_SELLITEM:
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetDealDialog()->SendSellItemMsg();
			}
			else
			{
				CDealDialog::OnCancelSellItem(0,NULL,0,GAMEIN->GetDealDialog(),NULL);
				//GAMEIN->GetDealDialog()->CancelSellItem();
			}
		}
		break;
	case MBI_BUYITEM:
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetDealDialog()->SendBuyItemMsg();
			}
			else
			{
				GAMEIN->GetDealDialog()->CancelBuyItem();
			}
		}
		break;
	case MBI_PKMODE:
		{
			if( we == MBI_YES )
			{
				PKMGR->SendMsgPKOn();
			}
			else
			{

			}
		}
		break;
	case MBI_NOT_GM:
		{
//			MAINGAME->SetGameState( eGAMESTATE_END );
			PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
		}
		break;

	case MBI_WAIT_LOGINCHECK:
		{
			//￠®E?￠®¡×I¡E￠c¡§ua￠®¡×u¡E￠c¡§I ￠®E￠®ⓒ­￠®¡×￠®¨I￠®¡×u￠®¡×uAo￠®E￠®ⓒ­| ￠®¡×￠®ⓒ­￠®E￠®ⓒ­¡§I¨I¨￡¡E￠ci¡E￠c¡§￠®i ¡§Ii¨Io￠®Ei¡§o￠®¡×u￠®¡×¡E￠c￠®¡×u¡E￠c¡§I A¡§I¨I¨￡￠®E￠®ⓒ­￠®Ec? ¡E￠c¡E￠￥¡E￠cEAI￠®EO¡E￠c￠®￠?￠®E￠®IA AI¡E￠c¡§￠®I￠®Ii￠®Ii A¡§I¨I¨￡￠®E￠®ⓒ­￠®Ec ￠®¡×uE￠®IiE¡E￠c¡§￠®I ￠®¡×u￠®¡×¡E￠c￠®E￠®IN¡E￠c¡§￠®￠®E¡Ec?
			
			cDialog* dlg = WINDOWMGR->GetWindowForID( MT_LOGINDLG );
			if( dlg )
			{
				cEditBox* editboxId = (cEditBox *)dlg->GetWindowForID(MT_IDEDITBOX);
				cEditBox* editboxPwd = (cEditBox *)dlg->GetWindowForID(MT_PWDEDITBOX);
				if( editboxId )
				{
					editboxId->SetEditText("");
					editboxId->SetFocusEdit(TRUE);
				}
				if( editboxPwd )
				{
					editboxPwd->SetEditText("");
					editboxPwd->SetFocusEdit(FALSE);
				}
			}

			TITLE->NoDisconMsg();		//￠®¡×u￠®¡×¡E￠c¡E￠c¡E￠￥￠®E¡Ec showserverlist￠®E?￠®E¡Ec￠®¡×u¡E￠c¡§I A¡E￠cE￠®¡×uO ¡§I￠®"￠cAA¡E￠ci￠®EO¡E￠c￠®￠? ¡§I￠®Ao¡E￠c¡§ue ￠®E￠®ⓒ­￠®¡×￠®¨I￠®¡×u￠®¡×uAo ￠®¡×u¡§I¡§¡I￠®Ii￠®Ii¡E￠c¡E￠￥I.
			TITLE->ShowServerList();
			TITLE->StartWaitConnectToAgent( FALSE );
		}
		break;
	case MBI_TIMEOVER_TOCONNECT:
		{
//			TITLE->NoDisconMsg();		//￠®¡×u￠®¡×¡E￠c¡E￠c¡E￠￥￠®E¡Ec showserverlist￠®E?￠®E¡Ec￠®¡×u¡E￠c¡§I A¡E￠cE￠®¡×uO ¡§I￠®"￠cAA¡E￠ci￠®EO¡E￠c￠®￠? ¡§I￠®Ao¡E￠c¡§ue ￠®E￠®ⓒ­￠®¡×￠®¨I￠®¡×u￠®¡×uAo ￠®¡×u¡§I¡§¡I￠®Ii￠®Ii¡E￠c¡E￠￥I.
//			TITLE->ShowServerList();
//			TITLE->StartWaitConnectToAgent( FALSE );
			TITLE->GetServerListDialog()->SetActive( TRUE );
		}
		break;


	case MBI_STREETBUYMSG:
		{
			if( we == MBI_YES )
			{
				STREETSTALLMGR->SendBuyItemMsg();
			}
			else
			{
			}
		}
		break;

	case MBI_REGEDITMSG:
		{
			if( we == MBI_YES )
			{
				if(!GAMEIN->GetStreetBuyStallDialog()->IsActive())
					return;

				POSTYPE pos = GAMEIN->GetStreetBuyStallDialog()->GetCurSelectedItemNum();
				BUY_REG_INFO RegInfo = GAMEIN->GetStreetBuyStallDialog()->GetBuyRegInfo(pos);
				GAMEIN->GetBuyRegDialog()->SetRegInfo(RegInfo);
				GAMEIN->GetBuyRegDialog()->Show();	
				
			}
			else
			{
				MSGBASE msg;
				msg.Category = MP_STREETSTALL;
				msg.dwObjectID = HEROID;
				msg.Protocol = MP_STREETSTALL_UPDATEEND_SYN;
				
				NETWORK->Send( &msg, sizeof(MSGBASE) );
			}
		}
		break;
	case MBI_STREETSELLMSG:
		{
			CStreetBuyStall* pStall = GAMEIN->GetStreetBuyStallDialog();
			
			if( we == MBI_YES )
				if(pStall && pStall->IsActive() && STREETSTALLMGR->SendSellItemMsg())
					break;
				else
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1218) );
	
			if(pStall) GAMEIN->GetStreetBuyStallDialog()->SetData(NULL);
			
			CItem* pItem = pStall->GetSellItem();

			if(pItem)	pItem->SetLock(FALSE);

			pStall->DelSellItem();
		}
		break;

	case MBI_QUESTGIVEUP:
		{
			if( we == MBI_YES )
				QUESTMGR->QuestAbandon();
			else
				QUESTMGR->QuestAbandonClose();
		}
		break;
	case MBI_BAILNOTICEMSG:
		{
			if( we == MBI_YES )
				GAMEIN->GetBailDialog()->SetBadFrameSync();
			else
				GAMEIN->GetBailDialog()->SetDisable( FALSE );

		}
		break;

	case MBI_QUESTSTART:
		{
			if( we == MBI_YES )
				GAMEIN->GetInventoryDialog()->ItemUseForQuestStart();
			
		}
		break;
	case MBI_BAILNOTICEERR:
		{
			GAMEIN->GetBailDialog()->SetDisable( FALSE );
		}
		break;
	case MBI_BAILLOWBADFAME:
		{
			OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		}
		break;
	case MBI_GUILD_SECEDE:
		{
			if( we == MBI_YES )
				GUILDMGR->SecedeSyn();
		}
		break;
	case MBI_GUILD_BREAKUP:
		{
			if( we == MBI_YES )
			{
				if( GUILDFIELDWAR->CheckGuildFieldWarCondition( 3 ) )
					GUILDMGR->BreakUpGuildSyn();
			}
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
		}
		break;
	case MBI_GUILD_BAN:
		{
			if(we == MBI_YES)
				GUILDMGR->DeleteMemberSyn();
		}
		break;
	case MBI_SHOPITEM_LOCK:
		{
			if( we == MBI_YES )
				GAMEIN->GetInventoryDialog()->ItemUnSealingSync();
		}
		break;

	case MBI_SAVEDPOINTMOVE:
		{
			if( we == MBI_YES )
				GAMEIN->GetMoveDialog()->MapMoveOK();
			else if( we == MBI_NO )
				GAMEIN->GetMoveDialog()->SetActive( FALSE );
		}
		break;
	case MBI_SAVEDPOINTDEL:
		{
			if( we == MBI_YES )
				GAMEIN->GetMoveDialog()->DelMoveInfoSync();
			else if( we == MBI_NO )
				GAMEIN->GetMoveDialog()->SetActive( FALSE );
		}
		break;
	case MBI_OTHERREVIVE:
		{
			if( we == MBI_YES )			
				ITEMMGR->ReviveOtherSync();			
			else if( we == MBI_NO )			
				ITEMMGR->ReviveOtherCancel();			
		}
		break;
	case MBI_REVIVECONFIRM:
		{
			if( we == MBI_YES )
				ITEMMGR->ReviveOtherConfirm( TRUE );
			else if( we == MBI_NO )
				ITEMMGR->ReviveOtherConfirm( FALSE );
		}
		break;
// guildfieldwar -----------------------------------------------------------------
	case MBI_GUILDFIELDWAR_DECLARE_ACCEPT:
		{
			GAMEIN->GetGFWarResultDlg()->SetDisable( FALSE );
			if( we == MBI_YES )
				GUILDFIELDWAR->SendGuildFieldWarDeclareAcceptDeny( MP_GUILD_FIELDWAR_DECLARE_ACCEPT );
		}
		break;
	case MBI_GUILDFIELDWAR_DECLARE_DENY:
		{
			GAMEIN->GetGFWarResultDlg()->SetDisable( FALSE );
			if( we == MBI_YES )
				GUILDFIELDWAR->SendGuildFieldWarDeclareAcceptDeny( MP_GUILD_FIELDWAR_DECLARE_DENY );
		}
		break;	
	case MBI_GUILDFIELDWAR_SUGGEST:
		{
			GAMEIN->GetGFWarInfoDlg()->SetDisable( FALSE );
//			GAMEIN->GetGFWarInfoDlg()->SetActive( FALSE );
			if( we == MBI_YES )
				GUILDFIELDWAR->SendGuildFieldWarSuggest();
		}
		break;
	case MBI_GUILDFIELDWAR_SURREND:
		{
			GAMEIN->GetGFWarInfoDlg()->SetDisable( FALSE );
//			GAMEIN->GetGFWarInfoDlg()->SetActive( FALSE );
			if( we == MBI_YES )
				GUILDFIELDWAR->SendGuildFieldWarSurrend();
		}
		break;
	case MBI_GUILDFIELDWAR_SUGGEST_ACCEPTDENY:
		{
			if( we == MBI_YES )
				GUILDFIELDWAR->SendGuildFieldWarSuggestAcceptDeny( MP_GUILD_FIELDWAR_SUGGESTEND_ACCEPT );
			else if( we == MBI_NO )
				GUILDFIELDWAR->SendGuildFieldWarSuggestAcceptDeny( MP_GUILD_FIELDWAR_SUGGESTEND_DENY );
		}
		break;
//--------------------------------------------------------------------------------
	// guildunion
	case MBI_GUILD_UNION_INVITE:
		{
			if( we == MBI_YES )
				GUILDUNION->SendInviteAcceptDeny( MP_GUILD_UNION_INVITE_ACCEPT );
			else if( we == MBI_NO )
				GUILDUNION->SendInviteAcceptDeny( MP_GUILD_UNION_INVITE_DENY );
		}
		break;
	case MBI_GUILD_UNION_REMOVE:
		{
			if( we == MBI_YES )
				GUILDUNION->SendRemoveGuildUnion();			
		}
		break;
	case MBI_GUILD_UNION_DESTROY:
		{
			if( we == MBI_YES )
				GUILDUNION->SendDestroyGuildUnion();
			OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		}
		break;
//----------------------------------------------------------------------------------
	case MBI_SKPOINTREDIST_USE:
		{
			// Hero 의 State검사.
			if( we == MBI_YES )
			{
				// 아이템의 사용
				MSG_SHOPITEM_USE_SYN msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_USE_SYN;
				msg.dwObjectID = HEROID;
				msg.UseBaseInfo.TargetItemPos = GAMEIN->GetSkPointDlg()->GetCurItemPos();
				msg.UseBaseInfo.TargetItemIdx = GAMEIN->GetSkPointDlg()->GetCurItemIdx();
				msg.UseBaseInfo.ShopItemPos = GAMEIN->GetSkPointDlg()->GetCurItemPos();
				msg.UseBaseInfo.ShopItemIdx = GAMEIN->GetSkPointDlg()->GetCurItemIdx();
				
				NETWORK->Send(&msg,sizeof(msg));

				GAMEIN->GetSkPointDlg()->SetCurItem( 0, 0 );
			}
			else if( we == MBI_NO )
			{
				GAMEIN->GetSkPointDlg()->SetCurItem( 0, 0 );
			}
		}
		break;
	case MBI_SKPOINTREDIST_UP:
		{
			if( we == MBI_YES )
				GAMEIN->GetSkPointDlg()->SetAbilitySyn( FALSE );
			else if( we == MBI_NO )
				GAMEIN->GetSkPointDlg()->SetDisable(FALSE);
		}
		break;
	case MBI_SKPOINTREDIST_DOWN:
		{
			if( we == MBI_YES )
				GAMEIN->GetSkPointDlg()->SetAbilitySyn( TRUE );
			else if( we == MBI_NO )
				GAMEIN->GetSkPointDlg()->SetDisable(FALSE);
		}
		break;
	case MBI_PRESENT_NOTIFY:
		{
			//
			GAMEIN->GetNoteDialog()->SetActive( TRUE );
			GAMEIN->GetNoteDialog()->SetMode( eMode_PresentNote );
			NOTEMGR->NoteListSyn( 1, eMode_PresentNote );
			GAMEEVENTMGR->AddEvent( eGameEvent_NoteInterface );

			CBarIcon* pIcon = GAMEIN->GetMainInterfaceDialog()->GetBarIcon( OPT_NOTEDLGICON );
			if( pIcon )
				pIcon->SetPush( TRUE );
		}
		break;
	// partywar
	case MBI_PARTYWAR_SUGGEST:
		{
			if( we == MBI_YES )
				PARTYWAR->SendPartyWarSuggestReturn( MP_PARTYWAR_SUGGEST_ACCEPT );
			else if( we == MBI_NO )
				PARTYWAR->SendPartyWarSuggestReturn( MP_PARTYWAR_SUGGEST_DENY );
		}
		break;
	case MBI_CHANGENAME_ACK:
	case MBI_JOBCHANGE_ACK:
		{
			// 대기화면으로
			EXITMGR->SendExitMsg( eEK_CHARSEL );
		}
		break;
	case MBI_WANTBUY:
		{
			if( we == MBI_YES )
			{
				DWORD WantedID = GAMEIN->GetWantNpcDialog()->GetCurWantedIdx();
				WANTEDMGR->BuyWantedRightSyn(WantedID);
			}
		}
		break;
	case MBI_WANTGIVEUP:
		{
			if( we == MBI_YES )
			{
				DWORD WantedID = GAMEIN->GetWantNpcDialog()->GetCurWantedIdx();
				WANTEDMGR->GiveUpWantedRightSyn(WantedID);
			}
		}
		break;
	case MBI_GTREGIST_SYN:
		{
			if( we == MBI_YES )
			{
				if( GAMEIN->GetGTRegistDlg() )
				{
					DWORD res;
					if( (res = GAMEIN->GetGTRegistDlg()->TournamentRegistSyn()) != eGTError_NOERROR )
					{
						char buf[256];
						if( res == eGTError_NOGUILDMASTER )
							sprintf( buf, CHATMGR->GetChatMsg( 950 ) );
						else if( res == eGTError_UNDERLEVEL )
							sprintf( buf, CHATMGR->GetChatMsg( 951 ), LIMITLEVEL_GUILDTOURNAMENT );
						else if( res == eGTError_UNDERLIMITEMEMBER )
							sprintf( buf, CHATMGR->GetChatMsg( 952 ), LIMITMEMBER_GUILDTOURNAMENT );
						else if( res == eGTError_NOTREGISTDAY )
							sprintf( buf, CHATMGR->GetChatMsg( 962 ), LIMITMEMBER_GUILDTOURNAMENT );

						CHATMGR->AddMsg( CTC_SYSMSG, buf );
					}
				}
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
				if( GAMEIN->GetGTRegistDlg() )
				{
					if( GAMEIN->GetGTRegistDlg()->IsActive() )
						GAMEIN->GetGTRegistDlg()->SetActive( FALSE );
				}
			}
		}
		break;
	case MBI_GTREGISTCANCEL_SYN:
		{
			if( we == MBI_YES )
			{
				if( GAMEIN->GetGTRegistcancelDlg() )
				{
					GAMEIN->GetGTRegistcancelDlg()->TournamentRegistCancelSyn();
				}
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
				if( GAMEIN->GetGTRegistcancelDlg() )
				{
					if( GAMEIN->GetGTRegistcancelDlg()->IsActive() )
						GAMEIN->GetGTRegistcancelDlg()->SetActive( FALSE );
				}
			}
		}
		break;
	case MBI_GTBATTLELIST_OBSERVER:
		{
			if( we == MBI_YES )
			{
				if( GAMEIN->GetGTBattleListDlg() )
				{
					if( GAMEIN->GetGTBattleListDlg()->EnterBattleonObserverSyn() == FALSE )
					{
						CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(958) );
						if( HERO->GetState() == eObjectState_Deal )
							OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
						if( GAMEIN->GetGTBattleListDlg()->IsActive() )
							GAMEIN->GetGTBattleListDlg()->SetActive( FALSE );
					}
				}
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
				if( GAMEIN->GetGTBattleListDlg() )
				{
					if( GAMEIN->GetGTBattleListDlg()->IsActive() )
						GAMEIN->GetGTBattleListDlg()->SetActive( FALSE );
				}
			}
		}
		break;
	case MBI_GTSCORE_EXIT:
		{
			if( we == MBI_YES )
			{
				MSG_DWORD msg;
				msg.Category = MP_GTOURNAMENT;
				msg.Protocol = MP_GTOURNAMENT_OBSERVER_EXIT;
				msg.dwData = HERO->GetBattleID();
				msg.dwObjectID = HEROID;
				NETWORK->Send( &msg, sizeof(msg) );
			}
		}
		break;
	case MBI_SW_PROTECTREG_OK:
		{
			if( we == MBI_YES )
			{
				DWORD dwGuildIdx = GAMEIN->GetSWProtectRegDlg()->GetSelectedGuildIdx();
				if( !dwGuildIdx )	return;

				MSG_DWORD Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol = MP_SIEGEWAR_ACCEPTGUILD_SYN;
				Msg.dwObjectID = HEROID;
				Msg.dwData = dwGuildIdx;
				NETWORK->Send( &Msg, sizeof(Msg) );

				GAMEIN->GetSWProtectRegDlg()->SetDisable( FALSE );
				GAMEIN->GetSWProtectRegDlg()->SetActive( FALSE );
			}
			else if( we == MBI_NO )
				GAMEIN->GetSWProtectRegDlg()->SetDisable( FALSE );
		}
		break;
	case MBI_SW_TAXCHANGE_OK:
		{
			if( we == MBI_YES )
			{
				cEditBox* pTax = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_CHANGE_TAX);
				if( pTax )
				{
					if( SWPROFIT->CheckSWProfit( 0, atoi(pTax->GetEditText()), 0 ) )
					{
						SWPROFIT->SendChangTexrateSyn( atoi(pTax->GetEditText()) );
						GAMEIN->GetSWProfitDlg()->SetDisable( FALSE );
						GAMEIN->GetSWProfitDlg()->SetActive( FALSE );
					}
				}
			}
			else if( we == MBI_NO )
				GAMEIN->GetSWProfitDlg()->SetDisable( FALSE );
		}
		break;
	case MBI_SW_PROFIT_OK:
		{
			if( we == MBI_YES )
			{
				cEditBox* pMoney = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_OUT_MONEY);
				if( pMoney )
				{
					if( SWPROFIT->CheckSWProfit( 1, 0, atoi(pMoney->GetEditText()) ) )
					{
						SWPROFIT->SendGetMoneySyn( atoi(pMoney->GetEditText()) );
						GAMEIN->GetSWProfitDlg()->SetDisable( FALSE );
						GAMEIN->GetSWProfitDlg()->SetActive( FALSE );
					}
				}
			}
			else if( we == MBI_NO )
				GAMEIN->GetSWProfitDlg()->SetDisable( FALSE );
		}
		break;
	case MBI_GUILD_CHANGE_LOCATION:
		{
			if( we == MBI_YES )
			{
				GUILDMGR->SendChangeLocation();
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
			}
		}
		break;
	case MBI_SW_ENGRAVE_SYN:
		{
			if( we == MBI_YES )
			{
				MSG_DWORD msg;
				SetProtocol( &msg, MP_SIEGEWAR, MP_SIEGEWAR_ENGRAVE_SYN );
				msg.dwObjectID = HEROID;
				msg.dwData = HERO->GetGuildIdx();
				NETWORK->Send( &msg, sizeof(msg) );			
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
			}
		}
		break;
	case MBI_SW_PROPOSAL_SYN:
		{
			if( we == MBI_YES )
			{
				MSGBASE Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol = MP_SIEGEWAR_DEFENCE_REGIST_SYN;
				Msg.dwObjectID = HEROID;
				NETWORK->Send( &Msg, sizeof(Msg) );
			}
		}
		break;
	case MBI_SW_ATTACKREGIST_SYN:
		{
			if( we == MBI_YES )
			{
				MSGBASE Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol = MP_SIEGEWAR_ATTACK_REGIST_SYN;
				Msg.dwObjectID = HEROID;
				NETWORK->Send( &Msg, sizeof(Msg) );
			}
		}
		break;
	case MBI_SW_OBSERVER_SYN:
		{
			if( we == MBI_YES )
			{
				if(PETMGR->GetCurSummonPet())
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1279));
					return;
				}

				MSG_DWORD2 Msg;
				Msg.Category = MP_SIEGEWAR;
				Msg.Protocol =  MP_SIEGEWAR_MOVEIN_SYN;
				Msg.dwObjectID = HEROID;
				Msg.dwData1 = HERO->GetGuildIdx();
				Msg.dwData2 = 2;
				NETWORK->Send( &Msg, sizeof(Msg) );
			}
		}
		break;
	case MBI_SKILLOPTIONCLEAR_ACK:
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetSkillOptionClearDlg()->OptionClearSyn();
			}
		}
		break;
	case MBI_SKILLOPTIONCLEAR_NACK:
		{
		}
		break;
		//SW070103 문파포인트개선
	case MBI_GUILD_STUDENTLVUP_CERABRATIONMONEY_MASTERCHECK:
		{
			if( we == MBI_OK )
			{
				MSG_DWORD2 msg;
				msg.Category = MP_GUILD;
				msg.Protocol = MP_GUILD_CELEBRATIONMONEY_MASTER_CHECKED;
				msg.dwData1 = HERO->GetGuildIdx();
				msg.dwData2 = HERO->GetID();
				NETWORK->Send( &msg, sizeof(msg) );
			}
		}
		break;
	case MBI_MUNPADELETESEL:
		{
            if( we == MBI_YES )
			{
				MSG_MUNHANOTEINFO msg;
				ZeroMemory( &msg, sizeof(MSG_MUNHANOTEINFO) );
				SetProtocol( &msg, MP_GUILD, MP_GUILD_MUNPA_DELETE_SYN );
				msg.dwObjectID = HEROID;
				msg.dwData = HERO->GetCharacterTotalInfo()->MunpaID;
				SafeStrCpy(msg.Intro, CHATMGR->GetChatMsg(1494), MAX_CHAT_LENGTH+1 );
				NETWORK->Send( &msg, sizeof(msg) );
			}
		}
		break;
	case MBI_TITAN_REPAIR:
		{
			if( we == MBI_YES )
			{
				CItem* pItem = TITANMGR->GetTitanRepairItem();
				if(!pItem)
					break;

				TITAN_ENDURANCE_ITEMINFO* pEnduranceInfo = TITANMGR->GetTitanEnduranceInfo(pItem->GetDBIdx());
				if( !pEnduranceInfo )
					break;

				float fEnduranceRate = (float)pEnduranceInfo->Endurance * 100 / TITAN_EQUIPITEM_ENDURANCE_MAX;
				if( fEnduranceRate > 0.0f && fEnduranceRate < 1.0f )
				{
					fEnduranceRate = 1.0f;
				}

				DWORD dwRepairPrice = (DWORD)( (100 - (DWORD)fEnduranceRate) * (pItem->GetItemInfo()->BuyPrice / 100) * 0.09f );

				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Titan);
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);

				if( HERO->GetMoney() < dwRepairPrice )
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1579));
					break;
				}

				MSG_DWORD2 msg;
				SetProtocol( &msg, MP_ITEM, MP_ITEM_TITAN_EQUIP_REPAIR_SYN );
				msg.dwObjectID = HEROID;
				msg.dwData1 = pItem->GetItemIdx();
				msg.dwData2 = pItem->GetPosition();
												
				NETWORK->Send(&msg, sizeof(msg));
			}
			else
			{
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Titan);
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
			}
		}
		break;
	case MBI_TITAN_TOTAL_REPAIR:
		{
			if( we == MBI_YES )
			{
				// 소지금 체크
				MSG_TITAN_REPAIR_TOTAL_EQUIPITEM_SYN msg;
				msg.Init();
				DWORD dwMoney = TITANMGR->GetTitanEnduranceTotalInfo(&msg, TRUE);
				if( HERO->GetMoney() < dwMoney )
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1579));
					break;
				}

				// 그대로 다시 맵서버 Send용으로 셋팅
				msg.Init();
				SetProtocol( &msg, MP_ITEM, MP_ITEM_TITAN_EQUIP_REPAIR_TOTAL_SYN );
				msg.dwObjectID = HEROID;
				
				TITANMGR->GetTitanEnduranceTotalInfo(&msg);

				NETWORK->Send(&msg, msg.GetSize());
			}
		}
		break;
		// magi82 - SOS(070720)
	case MBI_SOS:
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetSOSDlg()->SetActive(TRUE);
			}
			else
			{
                MSGBASE msg;
				SetProtocol( &msg, MP_GUILD, MP_GUILD_SOS_SEND_CANCEL );
				msg.dwObjectID = HEROID;
				NETWORK->Send(&msg, sizeof(MSGBASE));
			}
		}
		break;
	case MBI_SOS_YES:
		{
			if( we == MBI_YES )
			{
				MSG_DWORD msg;
				msg.Category = MP_GUILD;
				msg.Protocol = MP_GUILD_SOS_SEND_DIE_CHECK;
				msg.dwObjectID = HEROID;
				msg.dwData = GUILDMGR->GetSOSDieMemberIdx();
				NETWORK->Send(&msg, sizeof(msg));
			}
			else
			{
				MSG_DWORD msg;
				msg.Category = MP_GUILD;
				msg.Protocol = MP_GUILD_SOS_SEND_NO;
				msg.dwObjectID = HEROID;
				msg.dwData = GUILDMGR->GetSOSDieMemberIdx();
				NETWORK->Send(&msg, sizeof(msg));

				GUILDMGR->SetSOSDieMemberIdx(0);
				GUILDMGR->SetSOSTargetMapNum(0);
				GUILDMGR->SetSOSTargetPos(0);
				GUILDMGR->SetSOSChannelIdx(-1);
			}
		}
		break;
	case MBI_BUDDY_INFOAGREE:
		{
			ShellExecute( NULL, NULL, "Iexplore.exe", 
				"http://user.buddybuddy.co.kr/login/loginForm.asp?url=http://dark.buddybuddy.co.kr/channeling/buddybuddy/buddybuddy_member.asp", 
				NULL, SW_SHOWNORMAL );
		}
		break;
	case MBI_BUDDY_NAMECER:
		{
			ShellExecute( NULL, NULL, "Iexplore.exe", "http://game.buddybuddy.co.kr/Customer2/Member/UserCertify.aspx", 
				NULL, SW_SHOWNORMAL );				
		}
		break;
	// magi82(41) - 샵아이템 추가(스텟 초기화 주문서)
	case MBI_STATUS_POINT_RESET:
		{
			if( we == MBI_YES )
			{
				CItem* pItem = GAMEIN->GetInventoryDialog()->GetSelectedShopItem();
				// 아이템의 사용
				MSG_SHOPITEM_USE_SYN msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_USE_SYN;
				msg.dwObjectID = HEROID;
				msg.UseBaseInfo.TargetItemPos = pItem->GetPosition();
				msg.UseBaseInfo.TargetItemIdx = pItem->GetItemIdx();
				msg.UseBaseInfo.ShopItemPos = pItem->GetPosition();
				msg.UseBaseInfo.ShopItemIdx = pItem->GetItemIdx();

				NETWORK->Send(&msg,sizeof(msg));

				GAMEIN->GetInventoryDialog()->ResetSelectedShopItem();

				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
			}
			else
			{
				GAMEIN->GetInventoryDialog()->ResetSelectedShopItem();

				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
				ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
			}
		}
		break;
	// magi82(42) - 샵아이템 추가(수련치 초기화)
	case MBI_SKILL_POINT_RESET:
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetSkillPointResetDlg()->SetAbilityResetSyn();
			}
		}
		break;
	case MBI_SKILL_POINT_RESET_FAIL:
		{
		}
		break;
	//2008. 5. 23. CBH - 방파장의 방파 신청 수락 다이얼로그
	case MBI_PARTYDECISION:
		{
			if( we == MBI_YES )
			{
				PARTYMGR->PartyRequestResult(TRUE);
			}
			else if( we = MBI_NO)
			{
				PARTYMGR->PartyRequestResult(FALSE);
			}			
		}
		break;		
	case MBI_FT_ENGRAVE_START_SYN:
		{
			if( we == MBI_YES )
			{
				MSGBASE msg;
				msg.Category = MP_FORTWAR;
				msg.Protocol = MP_FORTWAR_ENGRAVE_START_SYN;
				msg.dwObjectID = HEROID;
				NETWORK->Send( &msg, sizeof(msg) );			
			}
			else if( we == MBI_NO )
			{
				if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
			}
		}
		break;
	case MBI_FT_ITEM_MOVE_SYN:
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetFWWareHouseDlg()->SendItemMoveSyn();
			}
			else if( we == MBI_NO )
			{
				ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );
				GAMEIN->GetFWWareHouseDlg()->SetDisable( FALSE );
			}
		}
		break;
	case MBI_FT_MONEY_MOVE_SYN:
		{
			if( we == MBI_YES )
			{
				GAMEIN->GetFWWareHouseDlg()->PutOutMoney();	
			}
			else if( we == MBI_NO )
			{
				ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );
				GAMEIN->GetFWWareHouseDlg()->SetDisable( FALSE );
			}
		}
		break;
	}
}

void MNM_DlgFunc(LONG lId, void * p, DWORD we)
{
	ySWITCH(lId)
		yCASE(MNM_PARTYDISPLAY_ONOFF)
			
		yCASE(MNM_TOGGLEFULL)
			GAMEIN->GetMiniMapDialog()->ToggleMinimapMode();
	yENDSWITCH
}

void ITD_DlgFunc(LONG lId, void * p, DWORD we)
{
	CUpgradeDlg::OnActionEvent(lId, p, we);
}

void ITMD_DlgFunc(LONG lId, void * p, DWORD we)
{
	CMixDialog::OnActionEvent(lId, p, we);
	
}

void ITR_DlgFunc(LONG lId, void * p, DWORD we)
{
	CReinforceDlg::OnActionEvent(lId, p, we);
}
void CHA_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	switch(lId)
	{
	case CHA_CONNNECTOK:
		{
#ifdef _KOR_LOCAL_
			//2008. 5. 9. CBH - 게임상태에따라 처리가 틀리다.
			if(MAINGAME->GetCurStateNum() == eGAMESTATE_CHARSELECT)
			{
				CHARSELECT->GetChannelDialog()->OnConnect();	
			}
			else if(MAINGAME->GetCurStateNum() == eGAMESTATE_GAMEIN)
			{
				CChannelDialog* pChannelDlg = (CChannelDialog*)WINDOWMGR->GetWindowForID( CHA_CHANNELDLG );
				if(pChannelDlg)
				{
					pChannelDlg->MapChange();
				}
			}
#else
			CHARSELECT->GetChannelDialog()->OnConnect();	
#endif
		}
		break;
	case CHA_CONNNECTCANCEL:
		{
#ifdef _KOR_LOCAL_
			//2008. 5. 9. CBH - 게임상태에따라 처리가 틀리다.
			if(MAINGAME->GetCurStateNum() == eGAMESTATE_CHARSELECT)
			{
				CHARSELECT->GetChannelDialog()->SetActive(FALSE);
				CHARSELECT->SetDisablePick(FALSE);
			}
			else if(MAINGAME->GetCurStateNum() == eGAMESTATE_GAMEIN)
			{
				CChannelDialog* pChannelDlg = (CChannelDialog*)WINDOWMGR->GetWindowForID( CHA_CHANNELDLG );
				if(pChannelDlg)
					pChannelDlg->SetActive(FALSE);
				CMoveDialog* pMoveDlg = (CMoveDialog*)WINDOWMGR->GetWindowForID( SA_MOVEDLG );
				if(pMoveDlg)
				{
					pMoveDlg->SetDisable(FALSE);
				}
				//뎠賈痰쀼냘샀諒뜁솖,혤句꾸鱗빈,쀼돕moveDlg눗왯,깻할掘齡鯤소盧땡
				/*if( HERO->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);*/
			}
#else
			CHARSELECT->GetChannelDialog()->SetActive(FALSE);
			CHARSELECT->SetDisablePick(FALSE);
#endif
		}
		break;
	}
}

void SL_DlgBtnFunc( LONG lId, void* p, DWORD we )
{
	CServerListDialog* pDlg = (CServerListDialog*)p;
	switch( lId )
	{
	case SL_BTN_CONNECT:
		TITLE->ConnectToServer( pDlg->GetSelectedIndex() );
		break;

	case SL_BTN_EXIT:
//		MAINGAME->SetGameState( eGAMESTATE_END );
		PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
		break;
		// magi82 - Intro(070802)
	case INTRO_BTN_REPLAY:
		{
			AUDIOMGR->StopBGM();
			MAINGAME->SetGameState(eGAMESTATE_INTRO);
			INTRO->SetReplayIntroFlag();			
		}
		break;
	}
}


void MI_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetMainInterfaceDialog()->OnActionEvent(lId,p,we);
}

void WANTNPC_DlgFunc( LONG lId, void* p, DWORD we )
{
	switch(lId)
	{
	case WANTNPC_LISTORDERCOMBOBOX:
		{
			if(we != WE_COMBOBOXSELECT) return;	
			
			BYTE OrderType = eWan_Date;
			cComboBox * btnOrder = (cComboBox *)WINDOWMGR->GetWindowForIDEx(WANTNPC_LISTORDERCOMBOBOX);
		
			OrderType = btnOrder->GetCurSelectedIdx();

			GAMEIN->GetWantNpcDialog()->SetOrderType(OrderType);
			WANTEDMGR->SortWantedListSyn(1);
		}
		break;
/*	case WANTNPC_BUY1BTN: case WANTNPC_BUY2BTN: case WANTNPC_BUY3BTN: case WANTNPC_BUY4BTN:
		{
			DWORD WantIDX = GAMEIN->GetWantNpcDialog()->GetSelectedWantedIDX(lId - WANTNPC_BUY1BTN);
			WANTEDMGR->BuyWantedRightSyn(WantIDX);
		}
		break; */
	case WANTNPC_SEARCHMODEBTN:
		{
			GAMEIN->GetWantNpcDialog()->ShowWantMode(eWantNpcMode_Search);
			SEND_WANTNPCLIST Info;
			memset(&Info, 0, sizeof(SEND_WANTNPCLIST));
			GAMEIN->GetWantNpcDialog()->SetWantedList(&Info);

		}
		break;
	case WANTNPC_BUYNPCBTN: 	case WANTNPC_BUYSEARCHBTN:
		{
			if(GAMEIN->GetWantNpcDialog()->GetCurWantedIdx())
			{
				char name[MAX_NAME_LENGTH+1] = {0,};
				if( GAMEIN->GetWantNpcDialog()->GetCurWantedName() )
				{
					SafeStrCpy( name, GAMEIN->GetWantNpcDialog()->GetCurWantedName(), MAX_NAME_LENGTH+1 );
				}
				char buf[64];
//				sprintf(buf, CHATMGR->GetChatMsg( 886 ), GAMEIN->GetWantNpcDialog()->GetCurWantedName());
				sprintf(buf, CHATMGR->GetChatMsg( 886 ), name );

				WINDOWMGR->MsgBox( MBI_WANTBUY, MBT_YESNO, buf);
			}
		}
		break;
	case WANTNPC_FRONTLISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD GotoPage = int((CurPage-MAX_WANTNPC_PAGEUNIT)/MAX_WANTNPC_PAGEUNIT)*MAX_WANTNPC_PAGEUNIT +1;
			WANTEDMGR->SortWantedListSyn(GotoPage);
		}
		break;
	case WANTNPC_1LISTBTN: case WANTNPC_2LISTBTN: case WANTNPC_3LISTBTN: case WANTNPC_4LISTBTN: 
	case WANTNPC_5LISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD CurUnit = (CurPage-1) / MAX_WANTNPC_PAGEUNIT; //MAX_WANTNPC_PAGEBTN-2 : front, back¡§Ioo￠®¡×¡E￠c¡E￠c¡§￠® ¡E￠ci¡E￠c¡§I 
			WANTEDMGR->SortWantedListSyn((lId-WANTNPC_1LISTBTN+1) + (CurUnit)*MAX_WANTNPC_PAGEUNIT);
		}
		break;
	case WANTNPC_BACKLISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD GotoPage = int((CurPage-1)/MAX_WANTNPC_PAGEUNIT)*MAX_WANTNPC_PAGEUNIT +1 + MAX_WANTNPC_PAGEUNIT;
			WANTEDMGR->SortWantedListSyn(GotoPage);
		}
		break;
	case WANTNPC_MYFRONTLISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD GotoPage = int((CurPage-MAX_WANTNPC_PAGEUNIT)/MAX_WANTNPC_PAGEUNIT)*MAX_WANTNPC_PAGEUNIT +1;
			WANTEDMGR->MyWantedListSyn(GotoPage);
		}
		break;
	case WANTNPC_MY1LISTBTN: case WANTNPC_MY2LISTBTN: case WANTNPC_MY3LISTBTN: case WANTNPC_MY4LISTBTN: 
	case WANTNPC_MY5LISTBTN:
		{
			WANTEDMGR->MyWantedListSyn(lId-WANTNPC_MY1LISTBTN+1);
		}
		break;
	case WANTNPC_MYBACKLISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD GotoPage = int((CurPage-1)/MAX_WANTNPC_PAGEUNIT)*MAX_WANTNPC_PAGEUNIT +1 + MAX_WANTNPC_PAGEUNIT;
			WANTEDMGR->MyWantedListSyn(GotoPage);
		}
		break;
	case WANTNPC_MYWANTBTN: case WANTNPC_MYWANTSEARCHBTN:
		{
#ifdef _TESTCLIENT_
			GAMEIN->GetWantNpcDialog()->ShowWantMode(eWantNpcMode_My);
#else
			WANTEDMGR->MyWantedListSyn(1);
#endif
		}
		break;
	case WANTNPC_SEARCHFRONTLISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD GotoPage = int((CurPage-MAX_WANTNPC_PAGEUNIT)/MAX_WANTNPC_PAGEUNIT)*MAX_WANTNPC_PAGEUNIT +1;
			cEditBox* pEdit = (cEditBox*)WINDOWMGR->GetWindowForIDEx(WANTNPC_SEARCHEDIT);
			WANTEDMGR->SearchSyn(pEdit->GetEditText(), GotoPage);
		}
		break;
	case WANTNPC_SEARCH1LISTBTN: case WANTNPC_SEARCH2LISTBTN: case WANTNPC_SEARCH3LISTBTN: case WANTNPC_SEARCH4LISTBTN: 
	case WANTNPC_SEARCH5LISTBTN:
		{
			cEditBox* pEdit = (cEditBox*)WINDOWMGR->GetWindowForIDEx(WANTNPC_SEARCHEDIT);
			WANTEDMGR->SearchSyn(pEdit->GetEditText(), lId-WANTNPC_SEARCH1LISTBTN+1);
		}
		break;
	case WANTNPC_SEARCHBACKLISTBTN:
		{
			DWORD CurPage = GAMEIN->GetWantNpcDialog()->GetCurPage();
			DWORD GotoPage = int((CurPage-1)/MAX_WANTNPC_PAGEUNIT)*MAX_WANTNPC_PAGEUNIT +1 + MAX_WANTNPC_PAGEUNIT;
			cEditBox* pEdit = (cEditBox*)WINDOWMGR->GetWindowForIDEx(WANTNPC_SEARCHEDIT);
			
			WANTEDMGR->SearchSyn(pEdit->GetEditText(), GotoPage);
		}
		break;
	case WANTNPC_SEARCHBTN:
		{
			cEditBox* pEdit = (cEditBox*)WINDOWMGR->GetWindowForIDEx(WANTNPC_SEARCHEDIT);
			WANTEDMGR->SearchSyn(pEdit->GetEditText(), 1);
		}
		break;
/*	case WANTNPC_GIVEUP1BTN: case WANTNPC_GIVEUP2BTN: case WANTNPC_GIVEUP3BTN: case WANTNPC_GIVEUP4BTN: 
		{
			DWORD WantIDX = GAMEIN->GetWantNpcDialog()->GetSelectedWantedIDX(lId - WANTNPC_GIVEUP1BTN);
			WANTEDMGR->GiveUpWantedRightSyn(WantIDX);
		}
		break;	 */
	case WANTNPC_SORTBTN:
		{
#ifdef _TESTCLIENT_
			GAMEIN->GetWantNpcDialog()->ShowWantMode(eWantNpcMode_Npc);
#else
			WANTEDMGR->SortWantedListSyn(1);
#endif
		}
		break;
	case WANTNPC_GIVEUPBTN:
		{
			if(GAMEIN->GetWantNpcDialog()->GetCurWantedIdx())
			{
				char name[MAX_NAME_LENGTH+1] = {0,};
				if( GAMEIN->GetWantNpcDialog()->GetCurWantedName() )
				{
					SafeStrCpy( name, GAMEIN->GetWantNpcDialog()->GetCurWantedName(), MAX_NAME_LENGTH+1 );
				}

				char buf[64];
//				sprintf(buf, CHATMGR->GetChatMsg( 887 ), GAMEIN->GetWantNpcDialog()->GetCurWantedName());
				sprintf(buf, CHATMGR->GetChatMsg( 887 ), name );
				WINDOWMGR->MsgBox( MBI_WANTGIVEUP, MBT_YESNO, buf);			
			}
		}
		break;
	}
}

void WANTREG_DlgFunc( LONG lId, void* p, DWORD we )
{
	switch(lId)
	{
	case WANTREG_OK:
		{
			CWantRegistDialog* pDlg = (CWantRegistDialog*)p;
			cEditBox* pEdit = (cEditBox*)pDlg->GetWindowForID(WANTREG_PRIZEEDIT);
			char* temp = RemoveComma(pEdit->GetEditText());
			DWORD prize = atoi(temp);
			if((prize < MIN_PRIZE) || (prize > MAX_PRIZE))
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(559) );
				return;
			}			
			WANTEDMGR->RegistKillerSyn(prize);
		}
		break;
	case WANTREG_CANCEL:
		{
			GAMEIN->GetWantRegistDialog()->SetActive(FALSE);
			WANTEDMGR->SetWaiting(eNone);
			WANTEDMGR->SetMurdererID(0);
			//GAMEIN->GetReviveDialog()->SetAlwaysTop(TRUE);
			//GAMEIN->GetReviveDialog()->SetActive(TRUE);
		}
		break;
	}
}


void QUE_QuestDlgFunc( LONG lId, void* p, DWORD we )
{
	CQuestDialog* pDlg = (CQuestDialog*)p;

	switch(lId)
	{
	case QUE_JOURNALBTN:
		JOURNALMGR->GetListSyn();
		break;
	case QUE_ITEM_GRID:
//		pDlg->GetPositionForXYRef();
		break;
	case QUE_PAGE1BTN:
		pDlg->RefreshQuestItem(0);
		break;
	case QUE_PAGE2BTN:
		pDlg->RefreshQuestItem(1);
		break;
	case QUE_PAGE3BTN:
		pDlg->RefreshQuestItem(2);
		break;
	case QUE_PAGE4BTN:
		pDlg->RefreshQuestItem(3);
		break;
	case QUE_PAGE5BTN:
		pDlg->RefreshQuestItem(4);
		break;
	}
}

void QUE_JournalDlgFunc( LONG lId, void* p, DWORD we )
{
	CJournalDialog* pDlg = (CJournalDialog*)p;

	switch(lId)
	{
	case QUE_JOURNALFRONTBTN:
		pDlg->SetBasePage(FALSE);
		break;
	case QUE_JOURNALBACKBTN:
		pDlg->SetBasePage(TRUE);
		break;		
	case QUE_JOURNALPAGE1BTN:
		pDlg->SetPage(0);		
		break;
	case QUE_JOURNALPAGE2BTN:
		pDlg->SetPage(1);		
		break;
	case QUE_JOURNALPAGE3BTN:
		pDlg->SetPage(2);		
		break;
	case QUE_JOURNALPAGE4BTN:
		pDlg->SetPage(3);		
		break;
	case QUE_JOURNALPAGE5BTN:
		pDlg->SetPage(4);		
		break;
	case QUE_JOURNALCHECKBOX1BTN:
		pDlg->SetItemCheck(0);
		break;
	case QUE_JOURNALCHECKBOX2BTN:
		pDlg->SetItemCheck(1);
		break;
	case QUE_JOURNALCHECKBOX3BTN:
		pDlg->SetItemCheck(2);
		break;
	case QUE_JOURNALCHECKBOX4BTN:
		pDlg->SetItemCheck(3);
		break;
	case QUE_JOURNALCHECKBOX5BTN:
		pDlg->SetItemCheck(4);
		break;
	case QUE_JOURNALSAVE:
		if(pDlg->IsSavedJournal())
			pDlg->SelectedJournalDelete();
		else
			pDlg->SelectedJournalSave();
		break;
	case QUE_JOURNALSAVEDLIST:
		pDlg->ViewJournalListToggle();
		break;
	}
}


void BAIL_BailDlgFunc( LONG lId, void* p, DWORD we )
{
	CBailDialog* pDlg = (CBailDialog*)p;

	switch(lId)
	{
	case BAIL_OK:
			pDlg->SetFame();
		break;
	case BAIL_CANCEL:
			pDlg->Close();
		break;
	}
}

//수련장 mugong practice
void MP_RegistDlgFunc( LONG lId, void* p, DWORD we )
{
	switch(lId)
	{
	case MP_ROKBTN:
		{
			if(PETMGR->GetCurSummonPet())
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1277));
				return;
			}
			GAMEIN->GetMPRegistDialog()->SetDisable(TRUE);
			GAMEIN->GetInventoryDialog()->SetDisable(TRUE);
			GAMEIN->GetMugongSuryunDialog()->SetDisable(TRUE);
			SURYUNMGR->GoSuryunMapSyn();
		}
		break;
	case MP_RCANCELBTN:
		{
			GAMEIN->GetMPRegistDialog()->SetActive(FALSE);
		}
	}	
}

void GD_DlgFunc( LONG lId, void* p, DWORD we )
{
	switch(lId)
	{
	///////////////////////////////////////////////////
	//create
	case GD_CCREATEOKBTN:
		{	//magi82 - 문하생 가입편리시스템
			if(HERO->GetGuildIdx())	// 소개글 수정할때
			{
				MSG_MUNHAINFOUPDATE msg;
				SetProtocol( &msg, MP_GUILD, MP_GUILD_INFOUPDATE_SYN );
				msg.dwObjectID = HEROID;
				msg.Index = GUILDMGR->GetGuild()->GetGuildIdx();
				
				char strInfo[MAX_GUILD_INTRO+1] = { 0, };
				cTextArea* pIntro = (cTextArea *)WINDOWMGR->GetWindowForIDEx(GD_CINTROTEXT);
				pIntro->GetScriptText(strInfo);
				SafeStrCpy(msg.Intro, strInfo, MAX_GUILD_INTRO+1);
				
				int len = strlen(msg.Intro);
				if(len > MAX_GUILD_INTRO)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(32));
					return;
				}

				NETWORK->Send(&msg, sizeof(msg));
				GAMEIN->GetGuildCreateDlg()->SetActive( FALSE );
			}
			else	// 문파 만들때
			{
				cEditBox * pGuildName = (cEditBox *)WINDOWMGR->GetWindowForIDEx(GD_CNAME);
				if(strcmp(pGuildName->GetEditText(), "") == 0)
					return;
				cTextArea * pIntro = (cTextArea *)WINDOWMGR->GetWindowForIDEx(GD_CINTROTEXT);
				char FullText[100];
				pIntro->GetScriptText(FullText);
				int len = strlen(FullText);
				if(len > MAX_GUILD_INTRO)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(32));
					return;
				}
				GUILDMGR->CreateGuildSyn(pGuildName->GetEditText(), FullText);
			}
		}
		break;
	///////////////////////////////////////////////////
	//invite
	case GD_IOKBTN:
		{
			if( GUILDMGR->GetInvitedKind() == AsMember )
			{
				GUILDMGR->AddMemberAccept();
			}
			else if( GUILDMGR->GetInvitedKind() == AsStudent )
			{
				GUILDMGR->AddStudentAccept();
			}
		}
		break;
	case GD_ICANCELBTN:
		{
			GUILDMGR->AddMemberDeny();
		}
		break;	
	///////////////////////////////////////////////////
	//guild info / func 
	case GD_MEMBERINFOPUSHUPBTN:
		{
			GAMEIN->GetGuildDlg()->ShowGuildMode(eGuildMode_GuildInfo);
		}
		break;
	case GD_GUILFUNCPUSHUPBTN:
		{
			GAMEIN->GetGuildDlg()->ShowGuildMode(eGuildMode_Func);
		}
		break;
	case GD_BANMEMBER:
		{
			if(HERO->GetGuildMemberRank() < GUILD_MASTER)
				return;
			if((GUILDMGR->GetSelectedMemberID() == 0) || (GUILDMGR->GetSelectedMemberID() == HEROID))
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(714));
			}
			else
			{
				char msg[128];
				sprintf(msg, CHATMGR->GetChatMsg( 46 ), GUILDMGR->GetSelectedMemberName());
				WINDOWMGR->MsgBox( MBI_GUILD_BAN, MBT_YESNO, msg);
				GUILDMGR->SetActiveFunc(TRUE);
			}				
		}
		break;
	case GD_INVITEMEMBER:
		{
#ifdef _HK_LOCAL_	//hk block
			CObject * targetObj = OBJECTMGR->GetSelectedObject();
			if(targetObj)
			{
				if(targetObj->GetObjectKind()  == eObjectKind_Player)
				{
					if(((CPlayer*)targetObj)->GetGuildIdx())
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(38));
						return;
					}
					else
						GUILDMGR->AddMemberSyn(targetObj->GetID());
				}
			}
#else
			CObject * targetObj = OBJECTMGR->GetSelectedObject();
			if(targetObj)
			{
				if(targetObj->GetObjectKind()  == eObjectKind_Player)
				{
					if(((CPlayer*)targetObj)->GetGuildIdx())
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(38));
						return;
					}
					else
						GAMEIN->GetGuildJoinDialog()->SetActive(TRUE);
				}
			}
#endif
		}
		break;
	case GD_GIVEMEMBERNICK:
		{	
			if(GUILDMGR->GetGuildLevel() < GUILD_2LEVEL)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(723));
				return;
			}
			GAMEIN->GetGuildNickNameDlg()->SetActive(TRUE);
		}
		break;
	case GD_GIVEMEMBERRANK:
		{
			if(HERO->GetGuildMemberRank() < GUILD_VICEMASTER)
				return;
			GAMEIN->GetGuildRankDlg()->SetActive(TRUE);
		}
		break;
	case GD_SECEDE:
		{
			if(HERO->GetGuildIdx() == 0)
				return;
			if(( GUILD_STUDENT <= HERO->GetGuildMemberRank() ) && (HERO->GetGuildMemberRank() <= GUILD_VICEMASTER) )
				WINDOWMGR->MsgBox( MBI_GUILD_SECEDE, MBT_YESNO, CHATMGR->GetChatMsg( 347 ) );
		}
		break;
	// 06. 03. 문파공지 - 이영준
	case GD_NOTICE:
		{
			//CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(710));
			GAMEIN->GetGuildNoticeDlg()->SetActive(TRUE);			
		}
		break;
	case GD_INVITEUNION:
		{
			CObject* targetObj = OBJECTMGR->GetSelectedObject();
			if( targetObj )
			{
				if( targetObj->GetObjectKind() == eObjectKind_Player )
				{
					GUILDUNION->SendInviteGuildUnion( (CPlayer*)targetObj );
				}
			}
		}
		break;			
	case GD_SECEDEUNION:
		{
			GUILDUNION->SendSecedeGuildUnion();
		}
		break;
	case GD_REMOVEUNION:
		{
			GAMEIN->GetGFWarInfoDlg()->ShowGuildUnion();
		}
		break;
	case GD_DECLARE:
		{
			if( GUILDFIELDWAR->CheckGuildFieldWarCondition( 0 ) )
				GAMEIN->GetGFWarDeclareDlg()->SetActive( TRUE );
		}
		break;
	case GD_SUGGEST:
		{
			if( GUILDFIELDWAR->CheckGuildFieldWarCondition( 1 ) )
				GAMEIN->GetGFWarInfoDlg()->ShowSuggest();
		}
		break;
	case GD_SURRENDER:
		{
			if( GUILDFIELDWAR->CheckGuildFieldWarCondition( 2 ) )
				GAMEIN->GetGFWarInfoDlg()->ShowSurrend();
		}
		break;

	case GD_POSITION://직위
		{
			GAMEIN->GetGuildDlg()->SortMemberListbyPosition();//Add 060803 by wonju
		}
		break;
	case GD_RANK:    //레벨
		{
			GAMEIN->GetGuildDlg()->SortMemberListbyLevel();//Add 060803 by wonju
		}
		break;
	case GD_PLUSTIME: //플러스 타임
		{
			GAMEIN->GetGuildPlusTimeDlg()->SetActive( TRUE ); //Add 060803 by wonju
		}
		break;

	case GD_RELATIONBTN: //동맹관계
		{
			GAMEIN->GetGuildWarInfoDlg()->SetActive( TRUE );
		}
		break;

	case GD_MUNHA:
		{
			GAMEIN->GetGuildMunhaDialog()->SetActive( TRUE );
		}	
		break;
	

	///////////////////////////////////////////////////
	//mark
	case GDM_REGISTOKBTN:
		{
			cEditBox* pMarkName = (cEditBox*)WINDOWMGR->GetWindowForIDEx(GDM_NAMEEDIT);
			if(strcmp(pMarkName->GetEditText(), "") == 0)
				return;
			if( GUILDMARKMGR->ChangeGuildMarkSyn(HERO->GetGuildIdx(), pMarkName->GetEditText()) == FALSE )
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(709));
			GAMEIN->GetGuildMarkDlg()->SetActive(FALSE);			
		}
		break;

	case GUM_REGISTOKBTN:
		{
			cEditBox* pMarkName = (cEditBox*)WINDOWMGR->GetWindowForIDEx( GDM_NAMEEDIT );
			if( strcmp(pMarkName->GetEditText(), "") == 0 )	return;
			if( GUILDUNION->CheckGuildUnionCondition( 5 ) )
			{
				if( !GUILDUNIONMARKMGR->RegistGuildUnionMark( g_nServerSetNum, HERO->GetGuildUnionIdx(), 
					HERO->GetGuildUnionMarkIdx(), pMarkName->GetEditText() ) )
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(709));
				GAMEIN->GetGuildMarkDlg()->SetActive(FALSE);			
			}
		}
		break;
		
	///////////////////////////////////////////////////
	//levelup
	case GD_LUOKBTN:
		{
			GUILDMGR->LevelUpSyn();
		}
		break;
	
	///////////////////////////////////////////////////
	//nick name 
	case GD_NREGISTOKBTN:
		{
			if(GUILDMGR->GetGuildLevel() < GUILD_2LEVEL)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(723));
				return;
			}
			if(GUILDMGR->GetSelectedMemberID())
			{
				cEditBox* pName = (cEditBox*)WINDOWMGR->GetWindowForIDEx(GD_NICKNAMEEDIT);
				if(strcmp(pName->GetEditText(), "") == 0)
					return;
				if(strlen(pName->GetEditText()) > MAX_GUILD_NICKNAME)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(98));
					return;
				}
				if((  FILTERTABLE->IsUsableName(pName->GetEditText(), FALSE ) == FALSE ) || (FILTERTABLE->IsCharInString(pName->GetEditText(), " '") == TRUE))
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(96));
					return;
				}
				GUILDMGR->GiveNickNameSyn(pName->GetEditText());
				GAMEIN->GetGuildNickNameDlg()->SetActive(FALSE);
			}
			else
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(714));
		}
		break;

	///////////////////////////////////////////////////
	//rank
	case GDR_OKBTN:
		{
			GAMEIN->GetGuildRankDlg()->SetActive(FALSE);
			BYTE Rank=0;
			cComboBox* pRankCombo = (cComboBox*)WINDOWMGR->GetWindowForIDEx(GDR_RANKCOMBOBOX);
			char buff[16];
			strcpy(buff,pRankCombo->GetComboText());
			
			CMD_ST(buff)
			CMD_CS( CHATMGR->GetChatMsg(155) )
			Rank = GUILD_MEMBER;
			CMD_CS( CHATMGR->GetChatMsg(156) )
			Rank = GUILD_SENIOR;
			CMD_CS( CHATMGR->GetChatMsg(157) )
			Rank = GUILD_VICEMASTER;
			CMD_EN
			
			GUILDMGR->ChangeMemberRank(Rank);
		}
		break;
	case GDR_DOKBTN:
		{
			GAMEIN->GetGuildRankDlg()->SetActive(FALSE);
			BYTE DRank;
			cComboBox* pDRankCombo = (cComboBox*)WINDOWMGR->GetWindowForIDEx(GDR_DRANKCOMBOBOX);
			char buff[16];
			strcpy(buff,pDRankCombo->GetComboText());
			
			CMD_ST(buff)
			CMD_CS( CHATMGR->GetChatMsg(713) )
			DRank = GUILD_MEMBER;
			CMD_CS( CHATMGR->GetChatMsg(712) )
			DRank = GUILD_VICEMASTER;
			CMD_EN			
			GUILDMGR->ChangeMemberRank(DRank);
		}
		break;
	case GDW_WAREHOUSE1BTN: case GDW_WAREHOUSE2BTN:
		{
			GAMEIN->GetGuildWarehouseDlg()->OnActionEvent(lId, p, we);
		}
		break;
	case GDW_PUTINMONEYBTN:
		{
			GAMEIN->GetGuildWarehouseDlg()->PutInOutMoney(1);
		}
		break;
	case GDW_PUTOUTMONEYBTN:
		{
			GAMEIN->GetGuildWarehouseDlg()->PutInOutMoney(0);
		}
		break;

	// guild union create
	case GDU_OKBTN:
		{
			cEditBox* pGuildUnionName = (cEditBox*)WINDOWMGR->GetWindowForIDEx(GDU_NAME);
			if( strcmp(pGuildUnionName->GetEditText(), "") == 0 )	return;
			if( strlen(pGuildUnionName->GetEditText()) > MAX_GUILD_NAME )	return;
			GUILDUNION->SendCreateGuildUnion( pGuildUnionName->GetEditText() );
		}
		break;

	default:
		{
			int a = 0;
		}
		break;
	}
}


// 아이템몰 창고
void ITMALL_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	if(lId == ITMALL_BTN1 || lId == ITMALL_BTN2 || lId == ITMALL_BTN3)
		GAMEIN->GetItemShopDialog()->TabChange(lId-ITMALL_BTN1);
}


// 뜁솖/쀼냘 뎐놔"盧땡끝杰커쩌"눗왯 SaveMove.bin
void SA_DlgBtnFunc(LONG lId, void * p, DWORD we)
{
	
	if( lId == SA_MOVETABBTN1 )
	{
		GAMEIN->GetMoveDialog()->SetButton( 0 );
		return;
	}
	if( lId == SA_MOVETABBTN2 )
	{
		GAMEIN->GetMoveDialog()->SetButton( 1 );
		return;
	}
		
	if( !GAMEIN->GetMoveDialog()->IsTownMove() && !GAMEIN->GetMoveDialog()->GetSelectedDBIdx() )
		return;

	switch(lId)
	{
	case SA_CHANGNAME:
		{
			GAMEIN->GetPointSaveDialog()->SetDialogStatus( FALSE );			
			GAMEIN->GetPointSaveDialog()->SetActive( TRUE );
			GAMEIN->GetMoveDialog()->SetDisable(TRUE);
		}		
		break;
	case SA_MOVEOK:		
		{
#ifdef _KOR_LOCAL_
			GAMEIN->GetMoveDialog()->MapMoveOK();		
#else
			if( GAMEIN->GetMoveDialog()->IsTownMove() )
				WINDOWMGR->MsgBox( MBI_SAVEDPOINTMOVE, MBT_YESNO, CHATMGR->GetChatMsg(781) );
			else
				WINDOWMGR->MsgBox( MBI_SAVEDPOINTMOVE, MBT_YESNO, CHATMGR->GetChatMsg(759) );		
#endif		
		}
		break;
	case SA_MOVEDEL:
		WINDOWMGR->MsgBox( MBI_SAVEDPOINTDEL, MBT_YESNO, CHATMGR->GetChatMsg(760) );
		break;
	case CMI_CLOSEBTN:
		GAMEIN->GetMoveDialog()->SetItemToMapServer( 0, 0, FALSE );
		break;
	}
}


// 뜁솖 츰냔긴뫘 "宮壇끝杰츰냔"눗왯 ChangeName.bin
extern void CHA_DlgFunc(LONG lId, void * p, DWORD we)
{
	switch(lId)
	{
	case CHA_NAME_OKBTN:
		GAMEIN->GetPointSaveDialog()->ChangePointName();
		GAMEIN->GetMoveDialog()->SetDisable(FALSE);
		break;
	case CHA_NAME_CANCELBTN:
		GAMEIN->GetPointSaveDialog()->CancelPointName();
		GAMEIN->GetMoveDialog()->SetDisable(FALSE);
		break;
	}
}

// guildfieldwar
extern void GFW_DlgFunc( LONG lId, void* p, DWORD we )
{
	switch( lId )
	{
	// declare
	case GFW_DECLARE_OKBTN:
		{
			cEditBox* pName = (cEditBox*)WINDOWMGR->GetWindowForIDEx( GFW_DECLARE_EDITBOX );
			cEditBox* pMoney = (cEditBox*)WINDOWMGR->GetWindowForIDEx( GFW_MONEY_EDITBOX );
			GUILDFIELDWAR->SendGuildFieldWarDeclare( pName->GetEditText(), RemoveComma(pMoney->GetEditText()) );
		}
		break;
	case GFW_DECLARE_CANCELBTN:
		{
			GAMEIN->GetGFWarDeclareDlg()->SetActive( FALSE );
		}
		break;
	case GFW_MONEY_CHK:
		{
			GAMEIN->GetGFWarDeclareDlg()->ShowMoneyEdit();
		}
		break;

	// result
	case GFW_RESULT_OKBTN:
		{
			if( GUILDFIELDWAR->CheckGuildFieldWarCondition( 4 ) )
			{
				GAMEIN->GetGFWarResultDlg()->SetDisable( TRUE );
				WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_DECLARE_ACCEPT, MBT_YESNO, CHATMGR->GetChatMsg( 813 ) );
			}
		}
		break;
	case GFW_RESULT_CANCELBTN:
		{
			GAMEIN->GetGFWarResultDlg()->SetDisable( TRUE );
			WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_DECLARE_DENY, MBT_YESNO, CHATMGR->GetChatMsg( 814 ) );
		}
		break;
	case GFW_RESULT_CONFIRMBTN:
		{
			GAMEIN->GetGFWarResultDlg()->SetActive( FALSE );
		}
		break;

	// info
	case GFW_INFO_SUGGEST_OKBTN:
		{
			GAMEIN->GetGFWarInfoDlg()->SetDisable( TRUE );
			GUILDINFO* pInfo = GUILDFIELDWAR->GetEnemyGuildInfoFromListIdx( GAMEIN->GetGFWarInfoDlg()->GetSelectedListIdx() );
			if( pInfo )
				WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_SUGGEST, MBT_YESNO, CHATMGR->GetChatMsg( 831 ), pInfo->GuildName );
			else
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 847 ) );
				GAMEIN->GetGFWarInfoDlg()->SetDisable( FALSE );
			}
		}
		break;
	case GFW_INFO_SURREND_OKBTN:
		{
			GAMEIN->GetGFWarInfoDlg()->SetDisable( TRUE );
			GUILDINFO* pInfo = GUILDFIELDWAR->GetEnemyGuildInfoFromListIdx( GAMEIN->GetGFWarInfoDlg()->GetSelectedListIdx() );
			if( pInfo )
				WINDOWMGR->MsgBox( MBI_GUILDFIELDWAR_SURREND, MBT_YESNO, CHATMGR->GetChatMsg( 825 ), pInfo->GuildName );
			else
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 847 ) );
				GAMEIN->GetGFWarInfoDlg()->SetDisable( FALSE );
			}
		}
		break;
	case GFW_GDU_REMOVE_OKBTN:	// guildunion remove
		{
			char* pName = GUILDUNION->GetGuildUnionNameFromListIdx( GAMEIN->GetGFWarInfoDlg()->GetSelectedListIdx() );
			if( pName )
				WINDOWMGR->MsgBox( MBI_GUILD_UNION_REMOVE, MBT_YESNO, CHATMGR->GetChatMsg(1116), pName );
			else
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 847 ) );
		}
		break;
	case GFW_INFO_CANCELBTN:
		{
			GAMEIN->GetGFWarInfoDlg()->SetActive( FALSE );
		}
		break;

	// warinfo
	case GW_INFOCLOSEBTN:
		{
			GAMEIN->GetGuildWarInfoDlg()->SetActive( FALSE );
		}
		break;
	}
}


// SkillPoint Redistribute
extern void SK_DlgBtnFunc( LONG lId, void* p, DWORD we )
{
	switch( lId )
	{
	case SK_UPBTN:
		{
			char buf[256];
			if( GAMEIN->GetSkPointDlg()->GetCurAbilityName() )
			{
				int Level = GAMEIN->GetSkPointDlg()->GetCurAbilityLevel();
				if( Level == -1 )					return;
				CAbilityInfo* pInfo = GAMEIN->GetSkPointDlg()->GetCurAbilityInfo();
				if( !pInfo )	return;
				if(ABILITYMGR->CanUpgradeSkPoint(pInfo,HERO->GetAbilityGroup()) == FALSE)		return;
#ifdef _TL_LOCAL_
				sprintf( buf, CHATMGR->GetChatMsg(739), Level+1, GAMEIN->GetSkPointDlg()->GetCurAbilityName(), Level );
#else
				sprintf( buf, CHATMGR->GetChatMsg(739), GAMEIN->GetSkPointDlg()->GetCurAbilityName(), Level, Level+1 );
#endif

				WINDOWMGR->MsgBox( MBI_SKPOINTREDIST_UP, MBT_YESNO, buf );
				GAMEIN->GetSkPointDlg()->SetDisable( TRUE );
			}			
		}
		break;
	case SK_DOWNBTN:
		{
			char buf[256];
			if( GAMEIN->GetSkPointDlg()->GetCurAbilityName() )
			{
				int Level = GAMEIN->GetSkPointDlg()->GetCurAbilityLevel();				
				if( Level > 0 )
				{
#ifdef _TL_LOCAL_
					sprintf( buf, CHATMGR->GetChatMsg(739), Level-1, GAMEIN->GetSkPointDlg()->GetCurAbilityName(), Level );
#else
					sprintf( buf, CHATMGR->GetChatMsg(739), GAMEIN->GetSkPointDlg()->GetCurAbilityName(), Level, Level-1 );
#endif
					WINDOWMGR->MsgBox( MBI_SKPOINTREDIST_DOWN, MBT_YESNO, buf );
					GAMEIN->GetSkPointDlg()->SetDisable( TRUE );
				}
			}
		}
		break;
	case SK_OKBTN:
		{			
			GAMEIN->GetSkPointDlg()->SetActive( FALSE );
		}
		break;
	case SK_STARTBTN:
		{
			MOVEMGR->HeroMoveStop();

			GAMEIN->GetSkPointNotifyDlg()->SetActive( FALSE );
			GAMEIN->GetSkPointDlg()->SetActive( TRUE );
			OBJECTSTATEMGR->StartObjectState( HERO, eObjectState_Deal );
		}
		break;
	case SK_POINTAGAIN1BTN:
	case SK_POINTAGAIN2BTN:
	case SK_POINTAGAIN3BTN:
		{
			if( (lId-SK_POINTAGAIN1BTN) == GAMEIN->GetSkPointDlg()->GetTabNumber() )
				return;

			GAMEIN->GetSkPointDlg()->SetTabNumber( lId-SK_POINTAGAIN1BTN );
		}
		break;
	}
}

void PW_DlgFunc( LONG lId, void* p, DWORD we )
{
	switch( lId )
	{
	case PW_CB_MEMBER0:	case PW_CB_MEMBER1:	case PW_CB_MEMBER2:	case PW_CB_MEMBER3:	case PW_CB_MEMBER4:
	case PW_CB_MEMBER5:	case PW_CB_MEMBER6:	case PW_CB_MEMBER7:	case PW_CB_MEMBER8:	case PW_CB_MEMBER9:
	case PW_CB_MEMBER10:	case PW_CB_MEMBER11:	case PW_CB_MEMBER12:	case PW_CB_MEMBER13:
		{
			PARTYWAR->SendPartyWarMember( lId - PW_CB_MEMBER0 );
		}
		break;
	case PW_BTN_LOCK:	PARTYWAR->SendPartyWarLock( TRUE );		break;
	case PW_BTN_UNLOCK:	PARTYWAR->SendPartyWarLock( FALSE );	break;
	case PW_BTN_START:	PARTYWAR->SendPartyWarStart();			break;
	case PW_BTN_CANCEL:	PARTYWAR->SendPartyWarCancel();			break;
	}

}

void CHS_DlgFunc( LONG lId, void* p, DWORD we )
{
	switch( lId )
	{
	case CHASE_SEEOKBTN:
		GAMEIN->GetChaseDlg()->SetActive( FALSE );
		break;
	case CHASE_OKBTN:
		GAMEIN->GetChaseinputDlg()->WantedChaseSyn();
		break;
	case CHASE_CANCELBTN:
		GAMEIN->GetChaseinputDlg()->SetActive( FALSE );
		break;
	}
}


void CHAN_DlgFunc(LONG lId, void * p, DWORD we)
{
	switch( lId )
	{
	case CH_NAME_CHANGE_OKBTN:
		{
			GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
			GAMEIN->GetNameChangeDlg()->NameChangeSyn();
		}		
		break;
	case CH_NAME_CHANGE_CANCELBTN:
		{
			GAMEIN->GetInventoryDialog()->SetDisable( FALSE );
			GAMEIN->GetNameChangeDlg()->SetItemDBIdx( 0 );
			GAMEIN->GetNameChangeDlg()->SetActive( FALSE );
		}
		break;
	case CH_NAME_CHANGE_STARTBTN:
		{
			CItem* pItem = GAMEIN->GetInventoryDialog()->GetCurSelectedItem(SHOPIVN);
			if( pItem )
			{
				GAMEIN->GetNameChangeDlg()->SetItemDBIdx( pItem->GetDBIdx() );
				GAMEIN->GetNameChangeDlg()->SetActive( TRUE );
				GAMEIN->GetNameChangeNotifyDlg()->SetActive( FALSE );
			}
		}
		break;
	}
}


void GDT_DlgFunc(LONG lId, void * p, DWORD we)
{
	switch( lId )
	{
	case GDT_ENTRYBTN:
		{
			WINDOWMGR->MsgBox( MBI_GTREGIST_SYN, MBT_YESNO, CHATMGR->GetChatMsg(960) );
		}
		break;
	case GDT_CANCELBTN:
		{
			WINDOWMGR->MsgBox( MBI_GTREGISTCANCEL_SYN, MBT_YESNO, CHATMGR->GetChatMsg(964) );
		}
		break;
	case GDT_WARVIEWBTN:
		{
			if( GAMEIN->GetGTBattleListDlg() )
			{
				if( GAMEIN->GetGTBattleListDlg()->GetSelectedBattle() > -1 )
					WINDOWMGR->MsgBox( MBI_GTBATTLELIST_OBSERVER, MBT_YESNO, CHATMGR->GetChatMsg(956) );
				else
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(957) );
			}
		}
		break;
	case GDT_VIEWCANCELBTN:
		{
			if( GAMEIN->GetGTBattleListDlg() )
			{
				if( GAMEIN->GetGTBattleListDlg()->IsActive() )
					GAMEIN->GetGTBattleListDlg()->SetActive( FALSE );
			}
		}
		break;
	case GDT_SEMIFINAL_PUSHUPBTN:
		{
			if( GAMEIN->GetGTStandingDlg() )			
				GAMEIN->GetGTStandingDlg()->SetCurPage( 0 );
		}
		break;
	case GDT_AGROUP_PUSHUPBTN:
		{
			if( GAMEIN->GetGTStandingDlg() )			
				GAMEIN->GetGTStandingDlg()->SetCurPage( 1 );
		}
		break;
	case GDT_BGROUP_PUSHUPBTN:
		{
			if( GAMEIN->GetGTStandingDlg() )			
				GAMEIN->GetGTStandingDlg()->SetCurPage( 2 );
		}
		break;
	case GDT_CGROUP_PUSHUPBTN:
		{
			if( GAMEIN->GetGTStandingDlg() )			
				GAMEIN->GetGTStandingDlg()->SetCurPage( 3 );
		}
		break;
	case GDT_DGROUP_PUSHUPBTN:
		{
			if( GAMEIN->GetGTStandingDlg() )			
				GAMEIN->GetGTStandingDlg()->SetCurPage( 4 );
		}
		break;
	case GDT_OUTBTN:
		{
			WINDOWMGR->MsgBox( MBI_GTSCORE_EXIT, MBT_YESNO, CHATMGR->GetChatMsg(969) );
		}
		break;
	}
}

void SW_DlgFunc(LONG lId, void* p, DWORD we)
{
	switch( lId )
	{
	case SW_TIMEREG_OKBTN:
		{
			cEditBox* pYear = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_TIME_YEAR);
			cEditBox* pMon = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_TIME_MON);
			cEditBox* pDay = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_TIME_DAY);
			cEditBox* pHour = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_TIME_HOUR);
			if( !pYear || !pMon || !pDay || !pHour )
			{
				GAMEIN->GetSWTimeRegDlg()->SetActive( FALSE );
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1120) );
				return;
			}
			if( (strcmp(pYear->GetEditText(), "") == 0) || (strcmp(pMon->GetEditText(), "") == 0) ||
				(strcmp(pDay->GetEditText(), "") == 0) || (strcmp(pHour->GetEditText(), "") == 0) )
			{
				GAMEIN->GetSWTimeRegDlg()->SetActive( FALSE );
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1120) );
				return;
			}

			DWORD dwYear = (DWORD)atoi( pYear->GetEditText() );
			DWORD dwMon = (DWORD)atoi( pMon->GetEditText() );
			DWORD dwDay = (DWORD)atoi( pDay->GetEditText() );
			DWORD dwHour = (DWORD)atoi( pHour->GetEditText() );

			if( (dwYear<2005) || (dwMon>12||dwMon<=0) || (dwDay>31||dwDay<=0) || (dwHour>23||dwHour<0) )
			{
				GAMEIN->GetSWTimeRegDlg()->SetActive( FALSE );
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1120) );
				return;
			}
			if( dwYear%4 == 0 )
				if( dwDay > DayOfMonth_Yundal[dwMon] )	return;
			else
				if( dwDay > DayOfMonth[dwMon] )			return;

			stTIME time;
			time.SetTime( dwYear - 2000, dwMon, dwDay, dwHour, 0, 0 );

			MSG_DWORD msg;
			msg.Category = MP_SIEGEWAR;
			msg.Protocol = MP_SIEGEWAR_REGISTTIME_SYN;
			msg.dwObjectID = HEROID;
			msg.dwData = time.value;
			NETWORK->Send( &msg, sizeof(msg) );

			GAMEIN->GetSWTimeRegDlg()->SetActive( FALSE );
		}
		break;
	case SW_PROTECTREG_OKBTN:
		{
			int nIdx = GAMEIN->GetSWProtectRegDlg()->GetSelectedListIdx();
			if( nIdx > -1 )
			{
				WINDOWMGR->MsgBox( MBI_SW_PROTECTREG_OK, MBT_YESNO, CHATMGR->GetChatMsg(1121), GAMEIN->GetSWProtectRegDlg()->GetSelectedGuildName( nIdx ) );
				GAMEIN->GetSWProtectRegDlg()->SetDisable( TRUE );
			}
			else
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1118) );				
		}
		break;
	case SW_INFOCLOSEBTN:
		{
			GAMEIN->GetSWInfoDlg()->SetActive( FALSE );
		}
		break;
	case SW_TAXCHANGE_OKBTN:
		{
			cEditBox* pTax = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_CHANGE_TAX);
			if( pTax )
			{
				if( SWPROFIT->CheckSWProfit( 0, atoi(pTax->GetEditText()), 0 ) )
				{
					WINDOWMGR->MsgBox( MBI_SW_TAXCHANGE_OK, MBT_YESNO, CHATMGR->GetChatMsg(1029), atoi(pTax->GetEditText()) );
					GAMEIN->GetSWProfitDlg()->SetDisable( TRUE );
				}
			}
		}
		break;
	case SW_PROFIT_OKBTN:
		{
			cEditBox* pMoney = (cEditBox*)WINDOWMGR->GetWindowForIDEx(SW_OUT_MONEY);
			if( pMoney )
			{
				if( SWPROFIT->CheckSWProfit( 1, 0, atoi(pMoney->GetEditText()) ) )
				{
					WINDOWMGR->MsgBox( MBI_SW_PROFIT_OK, MBT_YESNO, CHATMGR->GetChatMsg(1030), atoi(pMoney->GetEditText()) );
					GAMEIN->GetSWProfitDlg()->SetDisable( TRUE );
				}
			}
		}
		break;
	case SW_PROFIT_CANCELBTN:
		{
			GAMEIN->GetSWProfitDlg()->SetActive( FALSE );
		}
		break;
	case SW_ENGRAVECANCEL:
		{
			if( GAMEIN->GetSWEngraveDlg() )
				GAMEIN->GetSWEngraveDlg()->CancelEngraveSyn();
		}
		break;
	case SW_STARTTIME_OKBTN:
		{
			GAMEIN->GetSWStartTimeDlg()->SetActive( FALSE );
		}
		break;
	}
}


// Character change
void CHA_ChangeDlgFunc(LONG lId, void* p, DWORD we)
{
	CCharChangeDlg* pDlg = (CCharChangeDlg*)p;
	
	switch( lId )
	{
	case CHA_CharMake:
		pDlg->CharacterChangeSyn();
		break;
	case CHA_CharCancel:
		pDlg->Reset( FALSE );
		pDlg->SetActive( FALSE );
		break;
	case CHA_SexType1:
		pDlg->ChangeSexType( TRUE );
		break;
	case CHA_SexType2:
		pDlg->ChangeSexType( FALSE );
		break;
	case CHA_HairType1:
		pDlg->ChangeHairType( TRUE );
		break;
	case CHA_HairType2:
		pDlg->ChangeHairType( FALSE );
		break;
	case CHA_FaceType1:
		pDlg->ChangeFaceType( TRUE );
		break;
	case CHA_FaceType2:
		pDlg->ChangeFaceType( FALSE );
		break;
	}
}


void IT_DlgFunc(LONG lId, void* p, DWORD we)
{
	CSealDialog* pDlg = (CSealDialog*)p;
	
	switch( lId )
	{
	case IT_LOCKOKBTN:
		pDlg->ItemSealSyn();
		break;
	case IT_LOCKCANCERBTN:
		pDlg->ReleaseItem();
		break;
	}
}


void CJOB_DlgFunc(LONG lId, void* p, DWORD we)
{
	CChangeJobDialog* pDlg = (CChangeJobDialog*)p;

	switch( lId )
	{
	case CJOB_OKBTN:
		pDlg->ChangeJobSyn();
		break;
	case CJOB_CANCELBTN:
		pDlg->CancelChangeJob();
		break;		
	}
}

void CG_DlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetGuageDlg()->OnActionEvent(lId, p, we);
}


void RFDefault_DlgFunc(LONG lId, void *p, DWORD we)
{
	CReinforceResetDlg* pDlg = (CReinforceResetDlg*)p;

	switch( lId )
	{
	case RF_DefaultOKBTN:
		pDlg->ItemResetSyn();
		break;
	case RF_DefaultCANCERBTN:
		pDlg->ReleaseItem();
		break;
	}
}

void RareCreate_DlgFunc(LONG lId, void *p, DWORD we)
{
    CRareCreateDialog* pDlg = (CRareCreateDialog*)p;

	switch( lId )
	{
	case RareCreate_OKBTN:
		pDlg->ItemRareCreateSyn();
		break;
	case RareCreate_CANCERBTN:
		pDlg->ReleaseItem();
		break;
	}
}

void PET_DlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetPetStateDialog()->OnActionEvent(lId, p, we);
}

void PET_MiniDlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetPetStateMiniDialog()->OnActionEvent(lId, p, we);
}

// 06. 02. 강화 재료 안내 인터페이스 추가 - 이영준
void RFDGUIDE_DlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetReinforceDataGuideDlg()->OnActionEvent(lId, p, we);
}

// 06. 02. 내정보창 팁보기 추가 - 이영준
void TB_DlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetTipBrowserDlg()->OnActionEvent(lId, p, we);
}

void TB_STATE_DlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetTipBrowserDlg()->OnActionEvent(lId, p, we);
}

void PET_InvenDlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetPetInventoryDialog()->OnActionEvent(lId, p, we);
}

void PET_UpgradeDlgFunc(LONG lId, void *p, DWORD we)
{
	CPetUpgradeDialog* pDlg = (CPetUpgradeDialog*)p;
	switch( lId )
	{
	case PetUpgrade_OKBTN:
		{
			pDlg->PetUpgradeSyn();
		}
		break;
	case PetUpgrade_CANCERBTN:
		{
			pDlg->ReleaseItem();
		}
		break;
	}
}

void PET_RevivalDlgFunc(LONG lId, void *p, DWORD we)
{
	CPetRevivalDialog* pDlg = (CPetRevivalDialog*)p;
	switch( lId )
	{
	case PetRevival_OKBTN:
		{
			pDlg->PetRevivalSyn();	
		}
		break;
	case PetRevival_CANCERBTN:
		{
			pDlg->ReleaseItem();
		}
		break;
	}
}

void GN_DlgBtnFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetGuildNoteDlg()->OnActionEvnet(lId, p, we);
}

void AN_DlgBtnFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetUnionNoteDlg()->OnActionEvnet(lId, p, we);
}

// 06. 03. 문파공지 - 이영준
void GNotice_DlgBtnFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetGuildNoticeDlg()->OnActionEvnet(lId, p, we);
}

void GuildPlusTime_DlgFunc(LONG IId, void* p, DWORD we)//Add 060803 by wonju
{
	GAMEIN->GetGuildPlusTimeDlg()->OnActionEvnet(IId,p,we);
}

//////////////////////////////////////////////////////////////////
// 06. 06. 2차 전직 - 이영준
// 무공 변환
void SkillTrans_DlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetSkillOptionChangeDlg()->OnActionEvnet(lId, p, we);
}

void TDefault_DlgFunc(LONG lId, void *p, DWORD we)
{
	GAMEIN->GetSkillOptionClearDlg()->OnActionEvnet(lId, p, we);
}
//////////////////////////////////////////////////////////////////

void JO_DlgFunc(LONG lId, void* p, DWORD we)
{
	GAMEIN->GetGuildJoinDialog()->OnActionEvent(lId, p, we);
}

// magi82 - 문하생 가입편리시스템 /////////////////////////////////////////////////
void GU_DlgFunc(LONG lId, void* p, DWORD we)
{
	GAMEIN->GetGuildTraineeDialog()->OnActionEvent(lId, p, we);
}
///////////////////////////////////////////////////////////////////////////////////

// magi82 - 문하생 가입편리시스템 /////////////////////////////////////////////////
void GT_DlgFunc(LONG lId, void* p, DWORD we)
{
	GAMEIN->GetGuildMunhaDialog()->OnActionEvent(lId, p, we);
}
///////////////////////////////////////////////////////////////////////////////////

// magi82 - Titan(070112) ////////////////////////////////////////////////
void TPMD_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanPartsMakeDlg()->OnActionEvent(lId, p, we);
}
//////////////////////////////////////////////////////////////////////////

// magi82 - Titan(070125) ////////////////////////////////////////////////
void TitanMix_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanMixDlg()->OnActionEvent(lId, p, we);
}
//////////////////////////////////////////////////////////////////////////

// 2007. 9. 11. CBH - 타이탄 수리창 //////////////////////////////////////
void Titan_Repair_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanRepairDlg()->OnActionEvent(lId, p, we);
}
//////////////////////////////////////////////////////////////////////////

// 2007. 9. 14. CBH - 타이탄 소환 게이지 //////////////////////////////////////
//梁뻥揭譴
void Titan_Recall_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanRecallDlg()->OnActionEvent(lId, p, we);
}
//////////////////////////////////////////////////////////////////////////

// magi82 - Titan(070131)
void TitanUpgrade_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanUpgradeDlg()->OnActionEvent(lId, p, we);
}
//////////////////////////////////////////////////////////////////////////

// magi82 - Titan(070205)
void TitanBreak_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanBreakDlg()->OnActionEvent(lId, p, we);
}
//////////////////////////////////////////////////////////////////////////

// magi82 - Titan(070226)
void titan_inventory_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanInventoryDlg()->OnActionEvent(lId, p, we);
}
//////////////////////////////////////////////////////////////////////////

// magi82 - Titan(070307)
void Titan_guage_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanGuageDlg()->OnActionEvent(lId, p, we);
}
//////////////////////////////////////////////////////////////////////////

// magi82 - Titan(070611) 타이탄 무공변환 주석처리
/*
void Titan_MugongMix_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanMugongMixDlg()->OnActionEvent(lId, p, we);
}
*/

// magi82 - Titan(070320)
void Titan_Use_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanRegisterDlg()->OnActionEvent(lId, p, we);
}

// magi82 - Titan(070323)
void Titan_Bongin_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanDissolutionDlg()->OnActionEvent(lId, p, we);
}

// magi82 - UniqueItem(070703)
void UniqueItemCurseCancellationDlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetUniqueItemCurseCancellationDlg()->OnActionEvent(lId, p, we);
}

// magi82 - UniqueItem(070709)
void UniqueItemMixDlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetUniqueItemMixDlg()->OnActionEvent(lId, p, we);
}

// magi82 - SOS(070724)
void SOSDlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetSOSDlg()->OnActionEvent(lId, p, we);
}

// 2007. 10. 22. CBH - 유니크 아이탬 조합 게이지 //////////////////////////////////////
void UniqueItemMix_ProgressBar_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetUniqueItemMixProgressBarDlg()->OnActionEvent(lId, p, we);
}
//////////////////////////////////////////////////////////////////////////
// 2007. 10. 22. CBH - 타이탄 조합 게이지 //////////////////////////////////////
void TitanMix_ProgressBar_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanMixProgressBarDlg()->OnActionEvent(lId, p, we);
}
//////////////////////////////////////////////////////////////////////////
// 2007. 10. 23. CBH - 타이탄 파츠 조합 게이지 //////////////////////////////////////
void TitanParts_ProgressBar_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetTitanPartsProgressBarDlg()->OnActionEvent(lId, p, we);
}
//////////////////////////////////////////////////////////////////////////

// 2007. 12. 4. CBH - 스킨 선택  //////////////////////////////////////
void SkinSelect_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetSkinSelectDlg()->OnActionEvent(lId, p, we);
}
//////////////////////////////////////////////////////////////////////////

// 2008. 1. 16. CBH - 코스튬 스킨 선택  //////////////////////////////////////
void CostumeSkinSelect_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetCostumeSkinSelectDlg()->OnActionEvent(lId, p, we);
}
//////////////////////////////////////////////////////////////////////////

void SK_Reset_DlgFunc(LONG lId, void* p, DWORD we)
{
	GAMEIN->GetSkillPointResetDlg()->OnActionEvent(lId, p, we);
}

// 2008. 3. 17. CBH - 보안번호 입력 이벤트  //////////////////////////////////////
void NumberPad_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetNumberPadDlg()->OnActionEvent(lId, p, we);
}
//////////////////////////////////////////////////////////////////////////

//2008. 3. 19. CBH - 로그인 다이얼로그 콤보박스 이벤트 추가 //////////////////////////////////////
void MT_LOGINDLGFunc(LONG lId, void * p, DWORD we)
{
	if(we == WE_COMBOBOXSELECT)
	{		
		cComboBox* pCombo = (cComboBox*)WINDOWMGR->GetWindowForIDEx(MT_LISTCOMBOBOX);
		int nIndex = pCombo->GetCurSelectedIdx();

		CNumberPadDialog* pNumberPadDlg = GAMEIN->GetNumberPadDlg();		

		if(nIndex == 3)	//버디버디면 보안번호 숫자 입력 다이얼로그를 띄우지 않는다
			pNumberPadDlg->SetActive(FALSE);
		else
			pNumberPadDlg->SetActive(TRUE);
		
	}
}
//////////////////////////////////////////////////////////////////////////

// magi82(47)
void SVD_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetStreetStallItemViewDlg()->OnActionEvent(lId, p, we);
}

// autonote
void AutoNoteDlg_Func(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetAutoNoteDlg()->OnActionEvent(lId, p, we);
}

void AutoAnswerDlg_Func(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetAutoAnswerDlg()->OnActionEvent(lId, p, we);
}

//2008. 5. 21. CBH - 파티 생성 인터페이스 버튼 이벤트 추가
void PA_CreateBtnFunc(LONG lId, void * p, DWORD we) 
{
	GAMEIN->GetPartyCreateDialog()->OnActionEvent(lId, p, we);
}

//2008. 6. 2. CBH - 파티 매칭 목록 다이얼로그 이벤트
void Party_MatchingList_DlgFunc(LONG lId, void * p, DWORD we)
{
	GAMEIN->GetPartyMatchingDlg()->OnActionEvent(lId, p, we);
}

void FW_DlgFunc(LONG lId, void* p, DWORD we)
{
	switch( lId )
	{
	case FW_ENGRAVECANCEL:	GAMEIN->GetFWEngraveDlg()->OnActionEvent(lId, p, we);	break;
	}
}

void FD_WareHouseDlgFunc( LONG lId, void* p, DWORD we )
{
	switch( lId )
	{
	case FW_PUTOUTMONEYBTN:	GAMEIN->GetFWWareHouseDlg()->PutOutMoneyMsgBox();	break;
	}
}
