// cWindowManager.h: interface for the cWindowManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CWINDOWMANAGER_H__742467AD_E39C_455C_82B5_1CDA57918863__INCLUDED_)
#define AFX_CWINDOWMANAGER_H__742467AD_E39C_455C_82B5_1CDA57918863__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cWindowDef.h"

#include "IndexGenerator.h"
#include "PtrList.h"

/********************************************************************/
/*	window는 dialog만을 delete하고 나머지는 dialog안에서 delete한다.
/********************************************************************/
class cWindow;
class cImage;
class cDialog;
class cIconDialog;
class cMultiLineText;
class CMousePointer;
class cScriptManager;
class cAnimationManager;
class cMsgBox;
class cDivideBox;
class CKeyboard;
class CMouse;

#define WINDOWMGR	USINGTON(cWindowManager)
typedef void (*cbDROPPROCESSFUNC)(LONG curDragX, LONG curDragY, LONG id, LONG beforeDragX, LONG beforeDragY);

class cWindowManager  
{
public:
	cWindowManager();
	virtual ~cWindowManager();

	void Init();
	void AfterInit();
	void Preprocess();
	void DestroyWindowProcess();
	void AddListDestroyWindow(cWindow * window);
	void Process();
	void Render();
	cbDROPPROCESSFUNC cbDragnDropProcess;
	void SetcbDropProcess(cbDROPPROCESSFUNC func) { cbDragnDropProcess = func; }
	cDialog * GetWindowForID(LONG id);
	cWindow * GetWindowForIDEx(LONG id);
	cDialog * GetWindowForXY(LONG x, LONG y);
	cDialog * GetWindowForXYExceptIcon(LONG x, LONG y);
	cDialog * GetWindowForXYExceptID(LONG x, LONG y, LONG id);
	cIconDialog * GetIconDialogForXY(LONG x, LONG y);
	void CloseWindowForID(LONG id);
	void DestroyWindowAll();
	void DeleteWindowForID(LONG id);
	void DeleteWindow( cWindow* pWindow );
	void DeleteEditBoxListAll();
	void DeleteIconDlgListAll();
	void DeleteChatTooltipListAll();
	void DeleteSSTitletipListAll();
	void Release();
	
	cbWindowProcess cbProcess;

	cScriptManager * m_pScriptManager;
	// ResourceManager Related
	cWindow * GetDlgInfoFromFile(char * filePath, char* mode = "rt");

	cImage * GetImagePath(int idx, char * path, int size)
	{
		//return m_pScriptManager->GetImagePath(idx,path,size);
		return NULL;
	}
	/*
	cImageBase * GetImageOfFile(char * filePath, int layer)
	{
		return m_pScriptManager->GetImageOfFile(filePath, layer);
	}
	*/
	cImage * GetImageOfFile(char * filePath, int layer)
	{
		//return m_pScriptManager->GetImageOfFile(filePath, layer);
		return NULL;
	}
//	MAKESINGLETON(cWindowManager);

	///////////////////////////////////////////////////////////
	// it's called different function according to game state
	void CreateGameIn();
	void CreateCharDlg();
	void CreateMugongSuryunDlg();
	void CreateInventoryDlg();
	void CreateExchangeDlg();
	void CreateRecvExchangeDlg();
	void CreateMixDlg();
	void CreateTitanPartsMakeDlg();		// magi82 - Titan(070112)
	void CreateTitanMixDlg();			// magi82 - Titan(070119)	
	void CreateTitanRepairDlg();		// 2007. 9. 11. CBH - 타이탄 수리창
	void CreateTitanRecallDlg();		// 2007. 9. 13. CBH - 타이탄 소환게이지
	void CreateTitanUpgradeDlg();		// magi82 - Titan(070119)
	void CreateTitanBreakDlg();			// magi82 - Titan(070119)
	void CreateTitanChangePreViewDlg();	// magi82 - Titan(070212)
	void CreateTitanInventoryDlg();		// magi82 - Titan(070222)
	void CreateTitanGuageDlg();			// magi82 - Titan(070305)
	//void CreateTitanMugongMixDlg();		// magi82 - Titan(070611) 타이탄 무공변환 주석처리
	void CreateTitanRegisterDlg();		// magi82 - Titan(070320)
	void CreateTitanDissolution();		// magi82 - titan(070321)
	void CreateDealDlg();
	void CreateChatDlg();
//SW050722 attach to minimap
//	//SW050715
//	void CreateJackpotDlg();
	//void CreateDebugdlg();
	void CreateHeroGuage();
	void CreateQuickDlg();
	void CreatePartyDlg();
	void CreatePartyCreateDlg();
	void CreatePartyInviteDlg();
	void CreateGuildCreateDlg();
	void CreateGuildDlg();
	void CreateGuildInviteDlg();
	void CreateGuildMarkDlg();
	void CreateGuildLevelupDlg();
	void CreateGuildNickNameDlg();
	void CreateGuildRankDlg();
	void CreateGuildWarehouseDlg();
	void CreatePyogukDlg();
	void CreateNoteDlg();
	void CreateFriendDlg();
	void CreateMiniFriendDlg();
	void CreateMiniMapDlg();
	void CreateMiniNoteDlg();
	void CreateReviveDlg();
	void CreateWantNpcDlg();
	void CreateWantRegistDlg();
	void CreateQuestTotalDlg();
	void CreateMPRegistDlg();
	void CreateMPMissionDlg();
	void CreateMPNoticeDlg();
	void CreateMPGuageDlg();

	//SW051129 Pet
	void CreatePetStateDlg();
	void CreatePetStateMiniDlg();
	void CreatePetInvenDlg();
	void CreatePetUpgradeDlg();
	void CreatePetRevivalDlg();
	
	// LYJ 051017 구입노점상 추가
	void CreateStallKindSelectDlg();
	void CreateStreetBuyStallDlg();
	void CreateBuyRegDlg();

	// 노점 검색 창 추가 by Stiner(8)
	void CreateStallFindDlg();

// LBS 03.10.16//////////////////
	void CreateStreetStallDlg();
	void CreateNpcScriptDlg();
	void CreateHelpDlg();
/////////////////////////////////

//KES
	void CreateMacroDlg();
//	void CreateMacroModeDlg();
	void CreateChatOptionDlg();
	void CreateOptionDlg();
	void CreateExitDlg();
	void CreateCharStateDlg();
	void CreateMenuSlotDlg();

	void CreateUpgradeDlg();
	void CreateReinforceDlg();
	void CreateDissolveDlg();
	void CreateBailDlg();
	
	void CreateDissolutionDlg();
	void CreateEventNotifyDlg();
	void CreateMallNotcieDlg();

	void CreateItemShopDlg();
	void CreateMoveDlg();
	void CreatePointSaveDlg();
	
	// guildfieldwar
	void CreateGuildFieldWarDlg();

	void CreateSkPointDlg();
	void CreateSkPointNotifyDlg();
	
	// partywar
	void CreatePartyWarDlg();
	
	// shout
	void CreateShoutDlg();
	void CreateShoutchatDlg();

	//
	void CreateChaseDlg();
	void CreateChaseinputDlg();

	//
	void CreateNameChangeDlg();
	void CreateNameChangeNotifyDlg();

	void CreateGTRegistDlg();
	void CreateGTRegistcancelDlg();
	void CreateGTStandingDlg();
	void CreateGTBattleListDlg();
	void CreateGTScoreInfoDlg();

	void CreateSeigeWarDlg();

	// 
	void CreateCharChangeDlg();
	void CreateItemSealDlg();
	void CreateChangeJobDlg();
	void CreateReinforceResetDlg();
	void CreateRareCreateDlg();

	// 06. 02. 강화 재료 안내 인터페이스 추가 - 이영준
	void CreateReinforceDataGuideDlg();
	// 06. 02. 내정보창 팁보기 추가 - 이영준
	void CreateTipBrowserDlgDlg();
	void CreateKeySettingTipDlg();

	void CreateGuildNoteDlg();
	void CreateUnionNoteDlg();
	// 06. 03. 문파공지 - 이영준
	void CreateGuildNoticeDlg();

	///////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	void CreateSkillOptionChangeDlg();
	void CreateSkillOptionClearDlg();
	///////////////////////////////////////////////////

	void CreateGuildInvitationKindSelectionDlg();
	void CreateGuildMunhaDlg();
	void CreateGuildTraineeDlg();

	//SW061019 서바이벌모드
	void CreateSurvivalCountDlg();

	// magi82 - UniqueItem(070703)
	void CreateUniqueItemCurseCancellationDlg();

	// magi82 - UniqueItem(070709)
	void CreateUniqueItemMixDlg();

	// magi82 - SOS(070724)
	void CreateSOSDlg();	

	// 2007. 10. 22. CBH - 유니크 아이탬 조합 처리 바
	void CreateUniqueItemMixProgressBarDlg();
	// 2007. 10. 22. CBH - 타이탄 조합 게이지 다이얼로그
	void CreateTitanMixProgressBarDlg();
	// 2007. 10. 22. CBH - 타이탄 파츠 조합 게이지 다이얼로그
	void CreateTitanPartsProgressBarDlg();

	// 2007. 12. 4. CBH - 스킨 선택 다이얼로그
	void CreateSkinSelectDlg();
	// 2008. 1. 16. CBH - 코스튬 스킨 선택 다이얼로그
	void CreateCostumeSkinSelectDlg();

	// magi82(42) - 샵아이템 추가(수련치 초기화)
	void CreateSkillPointResetDlg();

	// 2008. 3. 17. CBH - 보안번호 다이얼로그
	void CreateNumberPadDlg();

	// magi82(47) - 노점상 아이템 검색창
	void CreateStreetStallItemViewDlg();

	// 2008. 5. 23. CBH - 방파매칭 다이얼로그
	void CreatePartyMatchingDlg();

	//screenshot
	void cWindowManager::ScreenShotDlg();

	cMsgBox* MsgBox( LONG lMBId, int nMBType, char* pStrMsg, ... );	
	cDivideBox * DivideBox( LONG lId, LONG x, LONG y, cbDivideFUNC cbFc1, cbDivideFUNC cbFc2, void * vData1, void * vData2, char* strTitle );
//

//pjs

	void CreatMousePoint();

	///////////////////////////////////////////////////////////

	//++++ Common dialog +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	// dynamic create and auto release
//	cWindow * CreateMoneyDlg();
//	cWindow * CreateAlertDlg(char * text, WORD wType, void (*cbFunc)(LONG lId, void * p, DWORD we), void * object);
	cPtrList * m_pDestroyWindowRef;					// reference
//	CIndexGenerator m_CommonDlgIDIndex;
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	LONG ID_SEED;
	inline void AddWindow(cWindow * window)
	{ 
#ifdef _DEBUG
		if(!window)
		{
			char buff[256];
			sprintf(buff, "window가 NULL입니다. 스크립트의 오류이거나 스크립트 PATH를 확인하세요");//pjslocal
			LOGEX(buff, PT_MESSAGEBOX);
			return;
		}
		PTRLISTPOS pos = m_pWindowList->GetHeadPosition();
		while(pos)
		{
			
			cWindow * win = (cWindow *)m_pWindowList->GetAt(pos);
			if(win == window)
			{
				char buff[256];
				sprintf(buff, "[AddWindow()] : 이미 동일한 다이얼로그가 존재합니다.");
				LOGEX(buff, PT_MESSAGEBOX);
			}
			m_pWindowList->GetNext(pos);
		}
#endif 
		m_pWindowList->AddTail((void *)window); 
	}


	void AddChatTooltip(cWindow * window)
	{
		m_pPlayerChatTooltip->AddTail(window);
	}
	void RemoveChatTooltipWindow(LONG playerID);	//필요없어지면 지운다. confirm
	void RemoveChatTooltipWindow( cWindow* pWindow );

	cPtrList * m_pWindowList;					
	cPtrList * m_pIconDlgListRef;					// reference
	cPtrList * m_pEditBoxListRef;					// for tab and shift+tab key	// reference
	cPtrList * m_pScreenTextList;					// 매 프레임마다 추가되고 삭제됨
	cPtrList * m_pPlayerChatTooltip;

	void SetDragStart(LONG sX, LONG sY, LONG x, LONG y, LONG id)
	{
		m_id = id;
		m_pDragDialog = GetWindowForID( id );
		m_OldX = x; m_OldY = y;
		m_posBeforeDrag.x=(float)sX;
		m_posBeforeDrag.y=(float)sY;
		m_OldDragFlag = TRUE;
	}
	void SetDragEnd()
	{
		//m_id=-1;
		//m_pDragDialog = NULL;
		m_OldX = -1; m_OldY = -1;
		m_OldDragFlag = FALSE;
	}

	void BackDragWindow();
	void DragWindowNull()
	{
		m_pDragDialog = NULL;
	}

	BOOL IsDragWindow() { return m_OldDragFlag;	}
	
	BOOL CloseAllWindow();
	void ShowBaseWindow();


public:
	void ToggleShowInterface();
	
	BOOL m_OldDragFlag;
	void SetComposition(BOOL val){ m_fComposition = val; }
	BOOL IsComposition(){ return m_fComposition; }
	CMousePointer * GetMouseWindow() { return m_pMousePointWindow;	}

//KES
	cWindow* GetFocusedEdit() { return m_pFocusedEdit; }
	void SetFocusedEdit( cWindow* pEdit, BOOL bFocus );
	void SetNextEditFocus();
	void SetOptionAlpha( DWORD dwAlpha );
	void SetWindowTop( cWindow* win );
	void SetToolTipWindow( cMultiLineText* toolTip ) { m_pToolTipWindow = toolTip; }

//KES INTERFACE 031008
	BOOL IsMouseInputProcessed()	{ return m_MouseInputProcessed; }

	BOOL IsMouseOverUsed()			{ return m_bMouseOverUsed; }
	BOOL IsMouseDownUsed()			{ return m_bMouseDownUsed; }
	BOOL IsMouseUpUsed()			{ return m_bMouseUpUsed; }
//	BOOL IsKeyInputUsed()			{ return m_bKeyInputUsed; }
	
	void SetMouseInputProcessed()	{ m_MouseInputProcessed = TRUE; }
	void SetMouseOverUsed()			{ m_bMouseOverUsed = TRUE; }
	void SetMouseDownUsed()			{ m_bMouseDownUsed = TRUE; }
	void SetMouseUpUsed()			{ m_bMouseUpUsed = TRUE; }

	void KeyboardInput( CKeyboard* keyInfo );
	void MouseInput( CMouse* pMouse );
	
	cMsgBox* GetFirstMsgBox();
	void CloseAllMsgBox();
	void CloseAllMsgBoxNoFunc();
	void PositioningDlg( LONG& rX, LONG& rY );
///////////

	BOOL CloseAllAutoCloseWindows();
/////
//	cDialog* GetDragDlg() { return GetWindowForID( m_id ); }
	cDialog* GetDragDlg() { return m_pDragDialog; }


	// 윈도우들의 이벤트를 받는 부분
	static void OnWindowEvent(LONG lId, void * p, DWORD we);
//
protected:

	BOOL			m_MouseInputProcessed;
	BOOL			m_fComposition;

	cDialog*		m_pGuageWindow;
	cWindow*		m_pMonsterGuageWindow;
	CMousePointer*	m_pMousePointWindow;

//KES 030819
	cWindow*		m_pFocusedEdit;
	cMultiLineText* m_pToolTipWindow;
	BOOL			m_bMouseOverUsed;
	BOOL			m_bMouseDownUsed;
	BOOL			m_bMouseUpUsed;
	
//	BOOL			m_bKeyInputUsed;
//

	//for drag
	LONG		m_id;
	cDialog*	m_pDragDialog;	//KES추가

	LONG		m_OldX;
	LONG		m_OldY;
	VECTOR2		m_posBeforeDrag;

//==== for rec.
	BOOL		m_bHideInterface;

public:
	BOOL		IsInterfaceHided() { return m_bHideInterface; }
	
#ifdef _JAPAN_LOCAL_
protected:
//	DWORD	m_dwConversion;
//	DWORD	m_dwSentence;

	BOOL	m_bOpenStatus;

public:
//	void SetConversionMode();
	void SetOpenStatus( BOOL bOpen ) { m_bOpenStatus = bOpen; }
#endif

};
EXTERNGLOBALTON(cWindowManager);
#endif // !defined(AFX_CWINDOWMANAGER_H__742467AD_E39C_455C_82B5_1CDA57918863__INCLUDED_)
