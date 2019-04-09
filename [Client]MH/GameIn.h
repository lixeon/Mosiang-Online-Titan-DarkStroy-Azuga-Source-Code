// GameIn.h: interface for the CGameIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEIN_H__E9C036AE_2344_473B_BEFE_21C4555A0685__INCLUDED_)
#define AFX_GAMEIN_H__E9C036AE_2344_473B_BEFE_21C4555A0685__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameState.h"
#include "PartyDialog.h"

class CMugongSuryunDialog;
class CMugongDialog;
class CSuryunDialog;
class CInventoryExDialog;
class CExchangeDialog;
class CMixDialog;
class CTitanPartsMakeDlg;		// magi82 - Titan(070112)
class CTitanMixDlg;				// magi82 - Titan(070119)
class CTitanRepairDlg;			// 2007. 9. 13. CBH - 타이탄 수리창
class CTitanRecallDlg;			// 2007. 9. 11. CBH - 타이탄 소환 게이지
class CTitanUpgradeDlg;			// magi82 - Titan(070119)
class CTitanBreakDlg;			// magi82 - Titan(070119)
class CTitanChangePreViewDlg;	// magi82 - Titan(070212)
class CTitanInventoryDlg;		// magi82 - Titan(070222)
class CTitanGuageDlg;			// magi82 - Titan(070305)
//class CTitanMugongMixDlg;		// magi82 - Titan(070611) 타이탄 무공변환 주석처리
class CTitanRegisterDlg;		// magi82 - Titan(070320)
class CTitanDissolutionDlg;		// magi82 - Titan(070321)
class CDealDialog;
class CCharacterDialog;
class CChatDialog;

class cJackpotDialog;

class CMainBarDialog;
class CDebugDlg;

// LYJ 051017 구입노점상 추가
class CStallKindSelectDlg;
class CStreetBuyStall;
class CBuyRegDialog;

// 노점상 검색 다이얼로그 추가 by Stiner(8)
class CStallFindDlg;

class CStreetStall;
class cNpcScriptDialog;
class cHelpDialog;

class CMacroDialog;
class CChatOptionDialog;
class COptionDialog;
class cDialog;
class CCharStateDialog;
class CMenuSlotDialog;

class cAuctionBoard;
class CMoneyDlg;
class CQuickDialog;
class CExitDialog;

class CGuildCreateDialog;
class CGuildDialog;
class CGuildInviteDialog;
class CGuildMarkDialog;
class CGuildLevelUpDialog;
class CGuildNickNameDialog;
class CGuildRankDialog;
class CGuildWarehouseDialog;
class CGuildUnionCreateDialog;
class CMiniMapDlg;
class CPyogukDialog;
class CNoteDialog;
class CFriendDialog;
class CMiniNoteDialog;
class CMiniFriendDialog;
class CReviveDialog;
class CMonsterGuageDlg;
class CUpgradeDlg;
class CReinforceDlg;
class CDissloveDlg;
class CWantNpcDialog;
class CWantRegistDialog;
class CQuestTotalDialog;
class CQuestDialog;
class CWantedDialog;
class CJournalDialog;
class CPartyCreateDlg;
class CPartyInviteDlg;
class CBailDialog;
class CDissolutionDialog;
class CEventNotifyDialog;
class CItemShopDialog;
class CMoveDialog;
class CPointSaveDialog;

class CMPRegistDialog;
class CMPMissionDialog;
class CMPNoticeDialog;
class CMPGuageDialog;
class CMHMap;
class CMHCamera;

class CGFWarDeclareDlg;
class CGFWarResultDlg;
class CGFWarInfoDlg;
class CGuildWarInfoDlg;
class CGuildPlusTimeDlg;

class CSkillPointRedist;
class CSkillPointNotify;

class CMallNoticeDialog;

class CPartyWarDialog;

class CShoutDialog;
class CShoutchatDialog;
class CChaseDialog;
class CChaseinputDialog;
class CNameChangeDialog;
class CNameChangeNotifyDlg;

class CBigMapDlg;

class CGTRegistDialog;
class CGTRegistcancelDialog;
class CGTStandingDialog;
class CGTBattleListDialog;
class CGTScoreInfoDialog;

class CSWTimeRegDlg;
class CSWProtectRegDlg;
class CSWInfoDlg;

class CCharChangeDlg;
class CSealDialog;
class CSWProfitDlg;
class CSWEngraveDialog;
class CSWTimeDialog;

class CChangeJobDialog;

class CSWStartTimeDialog;
class CGuageDialog;
class CReinforceResetDlg;
class CRareCreateDialog;

class CPetStateDlg;
class CPetStateMiniDlg;
class CPetInventoryDlg;
class CPetUpgradeDialog;
class CPetRevivalDialog;

// 06. 02. 강화 재료 안내 인터페이스 추가 - 이영준
class CReinforceDataGuideDlg;
// 06. 02. 내정보창 팁보기 추가 - 이영준
class CTipBrowserDlg;
class CKeySettingTipDlg;

class CGuildNoteDlg;
class CUnionNoteDlg;

class CGuildNoticeDlg;
class CSkillOptionChangeDlg;
class CSkillOptionClearDlg;

class CGuildJoinDialog;

class CScreenShotDlg;
class CGuildMunhaDialog;
class CGuildTraineeDialog;
class CSurvivalCountDialog;

class CUniqueItemCurseCancellationDlg;	// magi82 - UniqueItem(070703)
class CUniqueItemMixDlg;				// magi82 - UniqueItem(070709)

class CSOSDlg;	// magi82 - SOS(070724)

class CUniqueItemMixProgressBarDlg;			// 2007. 10. 19. CBH - 유니크 아이탬 처리 처리 바 게이지
class CTitanMixProgressBarDlg;			// 2007. 10. 22. CBH - 타이탄 조합 게이지
class CTitanPartsProgressBarDlg;		// 2007. 10. 23. CBH - 타이탄 파츠 조합 게이지

class CSkinSelectDialog;		//2007. 12. 4. CBH - 스킨 선택 다이얼로그
class CCostumeSkinSelectDialog;		//2008. 1. 16. CBH - 코스튬 스킨 선택 다이얼로그

class CSkillPointResetDlg;	// magi82(42) - 샵아이템 추가(수련치 초기화)

class CNumberPadDialog;		//2008. 3. 17. CBH - 보안번호 다이얼로그

// magi82(47)
class CStreetStallItemViewDlg;

class CAutoNoteDlg;
class CAutoAnswerDlg;

class CPartyMatchingDlg;		//2008. 5. 23. CBH - 방파 매칭 다이얼로그
// 요새전
class CFWEngraveDialog;
class CFWTimeDialog;
class CFWWareHouseDialog;

#define GAMEIN USINGTON(CGameIn)

class CGameIn : public CGameState  
{
	///////////////////// Dialog Pointer //////////////////////////////////
	CMugongSuryunDialog	* m_pMugongSuryunDlg;
	CMugongDialog		* m_pMugongDlg;
	CSuryunDialog		* m_pSuryunDlg;

	CInventoryExDialog	* m_pInventoryDlg;
	CExchangeDialog		* m_pExchangeDlg;
//	CRecvExchangeDialog	* m_pRecvExchangeDlg;
	CMixDialog			* m_pMixDlg;
	CTitanPartsMakeDlg	* m_pTitanPartsMakeDlg;			// magi82 - Titan(070112)
	CTitanMixDlg		* m_pTitanMixDlg;				// magi82 - Titan(070119)	
	CTitanRepairDlg		*m_pTitanRepairDlg;				//2007. 9. 11. CBH - 타이탄 수리창
	CTitanRecallDlg		*m_pTitanRecallDlg;				//2007. 9. 13. CBH - 타이탄 소환 게이지
	CTitanUpgradeDlg	* m_pTitanUpgradeDlg;			// magi82 - Titan(070119)
	CTitanBreakDlg		* m_pTitanBreakDlg;				// magi82 - Titan(070119)
	CTitanChangePreViewDlg	* m_pTitanChangePreViewDlg;	// magi82 - Titan(070212)
	CTitanInventoryDlg	* m_pTitanInventoryDlg;			// magi82 - Titan(070222)
	CTitanGuageDlg		* m_pTitanGuageDlg;				// magi82 - Titan(070305)
	//CTitanMugongMixDlg	* m_pTitanMugongMixDlg;			// magi82 - Titan(070611) 타이탄 무공변환 주석처리
	CTitanRegisterDlg	* m_pTitanRegisterDlg;			// magi82 - Titan(070320)
	CTitanDissolutionDlg* m_pTitanDissolutionDlg;		// magi82 - Titan(070321)

	CDealDialog			* m_pDealDlg;

	CCharacterDialog	* m_pCharDlg;
	CChatDialog			* m_pChatDlg;

	// SW050715
//	cJackpotDialog		* m_pJackpotDlg;
	
	CMainBarDialog			* m_pMainDlg;
	CDebugDlg			* m_pDebugDlg;

// LYJ 051017 구입노점상 추가
	CStallKindSelectDlg * m_pStallKindSelectDlg;
	CStreetBuyStall		* m_pStreetBuyStallDlg;
	CBuyRegDialog		* m_pBuyRegDlg;

// 노점상 검색 다이얼로그 추가 by Stiner(8)
	CStallFindDlg		* m_pStallFindDlg;

// LBS 03.10.16
	CStreetStall		* m_pStreetStallDlg;
	cNpcScriptDialog	* m_pNpcScriptDlg;
	cHelpDialog			* m_pHelpDlg;

//KES
	CMacroDialog		* m_pMacroDlg;
//	CMacroModeDialog	* m_pMacroModeDlg;
	CChatOptionDialog	* m_pChatOptionDlg;
	COptionDialog		* m_pOptionDlg;
	CExitDialog			* m_pExitDlg;
	CCharStateDialog	* m_pCharStateDlg;
	CMenuSlotDialog		* m_pMenuSlotDlg;
	CDissolutionDialog	* m_pDissolutionDlg;
	CEventNotifyDialog	* m_pEventNotifyDlg;
	CMallNoticeDialog	* m_pMallNoticeDlg;
//

	cAuctionBoard		* m_pAuctionBoard;

	CMoneyDlg			* m_MoneyDlg;
	CQuickDialog		* m_pQuickDlg;
	CGuageDialog		* m_pGuageDlg;

	CPartyDialog		m_PartyDlg;
	CPartyCreateDlg		* m_pPartyCreateDlg;
	CPartyInviteDlg		* m_pPartyInviteDlg;

	CGuildCreateDialog	* m_pGuildCreateDlg;
	CGuildDialog		* m_pGuildDlg;
	CGuildInviteDialog	* m_pGuildInviteDlg;
	CGuildMarkDialog	* m_pGuildMarkDlg;
	CGuildLevelUpDialog	* m_pGuildLevelUpDlg;
	CGuildNickNameDialog* m_pGuildNickNameDlg;
	CGuildRankDialog	* m_pGuildRankDlg;
	CGuildWarehouseDialog * m_pGuildWarehouseDlg;
	CGuildUnionCreateDialog * m_pGuildUnionCreateDlg;

	CMiniMapDlg			* m_pMiniMapDlg;
	CBigMapDlg			* m_pBigMapDlg;

	CPyogukDialog		* m_pPyogukDialog;
	CNoteDialog			* m_pNoteDialog;
	CFriendDialog		* m_pFriendDialog;
	CMiniNoteDialog		* m_pMiniNoteDialog;
	CMiniFriendDialog	* m_pMiniFriendDialog;
	CReviveDialog		* m_pReviveDialog;
	CWantNpcDialog		* m_pWantNpcDialog;
	CWantRegistDialog	* m_pWantRegistDialog;
	CMonsterGuageDlg	* m_pMonsterGuageDlg;
	CUpgradeDlg			* m_pUpgradeDlg;
	CReinforceDlg		* m_pReinforceDlg;
	CDissloveDlg		* m_pDissolveDlg;

	CQuestTotalDialog			* m_pQuestTotalDlg;
	CQuestDialog				* m_pQuestDlg;
	CWantedDialog				* m_pWantedDlg;
	CJournalDialog				* m_pJournalDlg;

	CBailDialog					* m_pBailDlg;

	CMPRegistDialog		* m_pMPRegistDialog;
	CMPMissionDialog	* m_pMPMissionDialog;
	CMPNoticeDialog		* m_pMPNoticeDialog;
	CMPGuageDialog		* m_pMPGuageDialog;
	
	CItemShopDialog		* m_pItemShopDialog;
	CMoveDialog			* m_pMoveDialog;
	CPointSaveDialog	* m_pPointSaveDialog;

	// guildfieldwar
	CGFWarDeclareDlg*	m_pGFWarDeclareDlg;
	CGFWarResultDlg*	m_pGFWarResultDlg;
	CGFWarInfoDlg*		m_pGFWarInfoDlg;
	CGuildWarInfoDlg*	m_pGuildWarInfoDlg;
	CGuildPlusTimeDlg*  m_pGuildPlusTimeDlg;//Add 060803 by wonju
	///////////////////////////////////////////////////////////////////////
	CGuildMunhaDialog*	m_pGuildMunhaDlg;
	CGuildTraineeDialog*	m_pGuildTraineeDlg;

	//
	CSkillPointRedist*	m_pSkPointDlg;
	CSkillPointNotify*	m_pSkPointNotifyDlg;

	// partywar
	CPartyWarDialog*	m_pPartyWarDlg;

	// 외치기
	CShoutDialog*		m_pShoutDlg;
	CShoutchatDialog*	m_pShoutchatDlg;

	//
	CChaseDialog*		m_pChaseDlg;
	CChaseinputDialog*	m_pChaseinputDlg;

	//
	CNameChangeDialog*	m_pNameChangeDlg;
	CNameChangeNotifyDlg*	m_pNameChangeNotifyDlg;

	// Guild Tournament
	CGTRegistDialog*			m_pGTRegistDlg;
	CGTRegistcancelDialog*		m_pGTRegistcancelDlg;
	CGTStandingDialog*			m_pGTStandingDlg;
	CGTBattleListDialog*		m_pGTBattleListDlg;
	CGTScoreInfoDialog*			m_pGTScoreInfoDlg;

	// seigewar
	CSWTimeRegDlg*				m_pSWTimeRegDlg;
	CSWProtectRegDlg*			m_pSWProtectRegDlg;
	CSWInfoDlg*					m_pSWInfoDlg;
	CSWStartTimeDialog*			m_pSWStartTimeDlg;

	// Character change
	CCharChangeDlg*			m_pCharChangeDlg;
	
	// ItemSeal
	CSealDialog*			m_pSealDlg;

	CSWProfitDlg*				m_pSWProfitDlg;
	CSWEngraveDialog*			m_pEngraveDlg;
	CSWTimeDialog*				m_pSWTimeDlg;

	//
	CChangeJobDialog*			m_pChangeJobDlg;

	//
	CReinforceResetDlg*			m_pReinforceResetDlg;
	CRareCreateDialog*			m_pRareCreateDlg;

	//SW051129 Pet
	CPetStateDlg*				m_pPetStateDlg;
	CPetStateMiniDlg*			m_pPetStateMiniDlg;
	CPetInventoryDlg*			m_pPetInventoryDlg;
	CPetUpgradeDialog*			m_pPetUpgradeDlg;
	CPetRevivalDialog*			m_pPetRevivalDlg;

	// 06. 02. 강화 재료 안내 인터페이스 추가 - 이영준
	CReinforceDataGuideDlg*		m_pReinforceDataGuideDlg;
	// 06. 02. 내정보창 팁보기 추가 - 이영준
	CTipBrowserDlg*				m_pTipBrowserDlg;
	CKeySettingTipDlg*			m_pKeySettingTipDlg;

	CGuildNoteDlg*				m_pGuildNoteDlg;
	CUnionNoteDlg*				m_pUnionNoteDlg;

	CGuildNoticeDlg*			m_pGuildNoticeDlg;

	/////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환
	CSkillOptionChangeDlg*		m_pSkillOptionChangeDlg;
	CSkillOptionClearDlg*		m_pSkillOptionClearDlg;
	/////////////////////////////////////////////////////////////

	//SW060713 문하생
	CGuildJoinDialog* m_pGuildInvitationKindSelectDlg;

	//스크린샷
	CScreenShotDlg*			m_pScreenShotDlg;

	//SW061019 서바이벌모드
	CSurvivalCountDialog*	m_pSurvivalCountDlg;
	
	// magi82 - UniqueItem(070703)
	CUniqueItemCurseCancellationDlg* m_pUniqueItemCurseCancellationDlg;
	// magi82 - UniqueItem(070709)
	CUniqueItemMixDlg* m_pUniqueItemMixDlg;

	// magi82 - SOS(070724)
	CSOSDlg* m_pSOSDlg;

	// autonote
	CAutoNoteDlg*	m_pAutoNoteDlg;
	CAutoAnswerDlg*	m_pAutoAnswerDlg;

	// 요새전
	CFWEngraveDialog*	m_pFWEngraveDlg;
	CFWTimeDialog*		m_pFWTimeDlg;
	CFWWareHouseDialog*	m_pFWWareHouseDlg;

	CUniqueItemMixProgressBarDlg*	m_pUniqueItemMixProgressBarDlg;		//2007. 10. 22. CBH - 유니크 아이탬 조합 처리 바 게이지
	CTitanMixProgressBarDlg*		m_pTitanMixProgressBarDlg;		//2007. 10. 22. CBH - 타이탄 조합 처리 바 게이지
	CTitanPartsProgressBarDlg*		m_pTitanPartsProgressBarDlg;		//2007. 10. 23. CBH - 타이탄 파츠 조합 처리 바 게이지

	CSkinSelectDialog* m_pSkinSelectDlg;	//2007. 12. 4. CBH - 스킨 선택 다이얼로그
	CCostumeSkinSelectDialog* m_pCostumeSkinSelectDlg;	//2008. 1. 16. CBH - 코스튬 스킨 선택 다이얼로그

	CSkillPointResetDlg* m_pSkillPointResetDlg;	// magi82(42) - 샵아이템 추가(수련치 초기화)

	CNumberPadDialog* m_pNumberPadDlg;		//2008. 3. 17. CBH - 보안번호 다이얼로그

	// magi82(47)
	CStreetStallItemViewDlg* m_pStreetStallItemViewDlg;

	CPartyMatchingDlg* m_pPartyMatchingDlg;		//2008. 5. 23. CBH - 방파 매칭 다이얼로그

	BOOL	m_bInitForGame;
	int		m_GameInInitKind;	
	DWORD	m_MovePoint;
	DWORD	m_MoveMap;
	DWORD	m_GuildIdx;		// Guild Tournament
	DWORD	m_BattleIdx;	// Guild Tournament - ObserverMode
	DWORD	m_SiegeWarMapNum;		// SiegeWar MapNum

public:	
	BOOL m_bGameInAcked;
	CGameIn();
	virtual ~CGameIn();

//	//MAKESINGLETON(CGameIn)
	BOOL Init(void* pInitParam);
	void Release(CGameState* pNextGameState);

//KES
	BOOL InitForGame();			//게임로딩->게임 일때 초기화
	BOOL GetInitForGame()	{	return m_bInitForGame;	}		//2007. 6. 18. CBH - 게임일때 구분
	void ReleaseForGame();		//게임->게임로딩 일때 릴리스
//

	void Process();
	void BeforeRender();
	void AfterRender();

	void NetworkMsgParse(BYTE Category,BYTE Protocol,void* pMsg);
	
	BOOL IsGameInAcked()		{	return m_bGameInAcked;	}
	int GetGameInInitKind()	{ return m_GameInInitKind; 	}

	virtual void OnDisconnect();

	DWORD GetMoveMap()			{	 return m_MoveMap;	}
	void SetGuildIdx( DWORD GuildIdx )		{	m_GuildIdx = GuildIdx;	}
	void SetMoveMap( DWORD MapNum )			{	m_MoveMap = MapNum;		}
	void SetBattleIdx( DWORD BattleIdx )	{	m_BattleIdx = BattleIdx;	}
	void SetSiegeWarMapNum( DWORD MapNum )	{	m_SiegeWarMapNum = MapNum;	}
	
	///// custom
	CMonsterGuageDlg	*	GetMonsterGuageDlg()	{ return m_pMonsterGuageDlg;	}
	void	SetMonsterGuageDlg(CMonsterGuageDlg	* dlg)	{ m_pMonsterGuageDlg = dlg;	}

	CCharacterDialog * GetCharacterDialog(){ return m_pCharDlg; }
	void SetCharacterDialog(CCharacterDialog * dlg){ m_pCharDlg=dlg; }
	CMainBarDialog	* GetMainInterfaceDialog(){ return m_pMainDlg; }
	void SetMainInterfaceDialog(CMainBarDialog	* Dlg){ m_pMainDlg=Dlg; }

	void SetChatDialog(CChatDialog	* Dlg){ m_pChatDlg=Dlg; }
	CChatDialog * GetChatDialog(){ return m_pChatDlg; }

	//SW050722 attach to minimap
//	void SetJackpotDialog(cJackpotDialog * Dlg){ m_pJackpotDlg=Dlg; }
//	cJackpotDialog* GetJackpotDialog(){ return m_pJackpotDlg; }

	void SetDebugDlg(CDebugDlg* Dlg){m_pDebugDlg = Dlg;}
	CDebugDlg* GetDebugDlg(){return m_pDebugDlg;}

	CMugongSuryunDialog * GetMugongSuryunDialog(){ return m_pMugongSuryunDlg; }
	void SetMugongSuryunDialog(CMugongSuryunDialog * dlg){ m_pMugongSuryunDlg=dlg; }

	CMugongDialog * GetMugongDialog(){ return m_pMugongDlg; }
	void SetMugongDialog(CMugongDialog * dlg){ m_pMugongDlg=dlg; }
	CSuryunDialog * GetSuryunDialog(){ return m_pSuryunDlg; }
	void SetSuryunDialog(CSuryunDialog * dlg){ m_pSuryunDlg=dlg; }

    CUpgradeDlg * GetUpgradeDialog(){ return m_pUpgradeDlg; }
	void SetUpgradeDialog(CUpgradeDlg * dlg){ m_pUpgradeDlg=dlg; }
	CInventoryExDialog * GetInventoryDialog(){ return m_pInventoryDlg; }
	void SetInventoryDialog(CInventoryExDialog * dlg){ m_pInventoryDlg=dlg; }
	CExchangeDialog * GetExchangeDialog(){ return m_pExchangeDlg; }
	void SetExchangeDialog(CExchangeDialog * dlg){ m_pExchangeDlg=dlg; }
//	CRecvExchangeDialog * GetRecvExchangeDialog(){ return m_pRecvExchangeDlg; }
//	void SetRecvExchangeDialog(CRecvExchangeDialog * dlg){ m_pRecvExchangeDlg=dlg; }
	CMixDialog * GetMixDialog(){ return m_pMixDlg; }
	void SetMixDialog(CMixDialog * dlg){ m_pMixDlg=dlg; }
	// magi82 - Titan(070112) ////////////////////////////////////////////////
	CTitanPartsMakeDlg * GetTitanPartsMakeDlg(){ return m_pTitanPartsMakeDlg; }
	void SetTitanPartsMakeDlg(CTitanPartsMakeDlg * dlg){ m_pTitanPartsMakeDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	// magi82 - Titan(070119) ////////////////////////////////////////////////
	CTitanMixDlg * GetTitanMixDlg(){ return m_pTitanMixDlg; }
	void SetTitanMixDlg(CTitanMixDlg * dlg){ m_pTitanMixDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	// 2007. 9. 11. CBH - 타이탄 수리창 ////////////////////////////////////////////////
	CTitanRepairDlg* GetTitanRepairDlg(){ return m_pTitanRepairDlg; }
	void SetTitanRepairDlg(CTitanRepairDlg * dlg){ m_pTitanRepairDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	// 2007. 9. 13. CBH - 타이탄 소환 게이지 ////////////////////////////////////////////////
	CTitanRecallDlg* GetTitanRecallDlg(){ return m_pTitanRecallDlg; }
	void SetTitanRecallDlg(CTitanRecallDlg * dlg){ m_pTitanRecallDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	// magi82 - Titan(070119) ////////////////////////////////////////////////
	CTitanUpgradeDlg* GetTitanUpgradeDlg(){ return m_pTitanUpgradeDlg; }
	void SetTitanUpgradeDlg(CTitanUpgradeDlg * dlg){ m_pTitanUpgradeDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	// magi82 - Titan(070119) ////////////////////////////////////////////////
	CTitanBreakDlg* GetTitanBreakDlg(){ return m_pTitanBreakDlg; }
	void SetTitanBreakDlg(CTitanBreakDlg * dlg){ m_pTitanBreakDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	// magi82 - Titan(070212) ////////////////////////////////////////////////
	CTitanChangePreViewDlg* GetTitanChangePreViewDlg(){ return m_pTitanChangePreViewDlg; }
	void SetTitanChangePreViewDlg(CTitanChangePreViewDlg * dlg){ m_pTitanChangePreViewDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	// magi82 - Titan(070222) ////////////////////////////////////////////////
	CTitanInventoryDlg* GetTitanInventoryDlg(){ return m_pTitanInventoryDlg; }
	void SetTitanInventoryDlg(CTitanInventoryDlg * dlg){ m_pTitanInventoryDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	// magi82 - Titan(070305) ////////////////////////////////////////////////
	CTitanGuageDlg* GetTitanGuageDlg(){ return m_pTitanGuageDlg; }
	void SetTitanGuageDlg(CTitanGuageDlg * dlg){ m_pTitanGuageDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	// magi82 - Titan(070611) 타이탄 무공변환 주석처리
	/*
	CTitanMugongMixDlg* GetTitanMugongMixDlg(){ return m_pTitanMugongMixDlg; }
	void SetTitanMugongMixDlg(CTitanMugongMixDlg * dlg){ m_pTitanMugongMixDlg=dlg; }
	*/

	// magi82 - Titan(070320) ////////////////////////////////////////////////
	CTitanRegisterDlg* GetTitanRegisterDlg(){ return m_pTitanRegisterDlg; }
	void SetTitanRegisterDlg(CTitanRegisterDlg * dlg){ m_pTitanRegisterDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	// magi82 - Titan(070320) ////////////////////////////////////////////////
	CTitanDissolutionDlg* GetTitanDissolutionDlg(){ return m_pTitanDissolutionDlg; }
	void SetTitanDissolutionDlg(CTitanDissolutionDlg * dlg){ m_pTitanDissolutionDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////

	CReinforceDlg * GetReinforceDialog(){ return m_pReinforceDlg; }
	void SetReinforceDialog(CReinforceDlg * dlg){ m_pReinforceDlg=dlg; }
	CDissloveDlg * GetDissolveDialog(){ return m_pDissolveDlg; }
	void SetDissolveDialog(CDissloveDlg * dlg){ m_pDissolveDlg=dlg; }
		
	CDealDialog * GetDealDialog(){ return m_pDealDlg; }
	void SetDealDialog(CDealDialog * dlg){ m_pDealDlg=dlg; }

	// LYJ 051017 구입노점상 추가
	CStallKindSelectDlg * GetStallKindSelectDialog(){ return m_pStallKindSelectDlg; }
	void SetStallKindSelectDialog(CStallKindSelectDlg * dlg){ m_pStallKindSelectDlg=dlg; }	
	CStreetBuyStall * GetStreetBuyStallDialog(){ return m_pStreetBuyStallDlg; }
	void SetStreetBuyStallDialog(CStreetBuyStall * dlg){ m_pStreetBuyStallDlg=dlg; }
	CBuyRegDialog * GetBuyRegDialog(){ return m_pBuyRegDlg; }
	void SetBuyRegDialog(CBuyRegDialog * dlg){ m_pBuyRegDlg=dlg; }
	
	//////////////////////////////////////////////////////////////////////////
	// 노점상 검색 다이얼로그 by Stiner(8)
	CStallFindDlg * GetStallFindDlg(){ return m_pStallFindDlg; };
	void SetStallFindDlg(CStallFindDlg * dlg){ m_pStallFindDlg = dlg; };

// LBS
	CStreetStall * GetStreetStallDialog(){ return m_pStreetStallDlg; }
	void SetStreetStallDialog(CStreetStall * dlg){ m_pStreetStallDlg=dlg; }
	cNpcScriptDialog* GetNpcScriptDialog() { return m_pNpcScriptDlg; }
	void SetNpcScriptDialog(cNpcScriptDialog* dlg) { m_pNpcScriptDlg = dlg; }
	cHelpDialog* GetHelpDialog() { return m_pHelpDlg; }
	void SetHelpDialog(cHelpDialog* dlg) { m_pHelpDlg = dlg; }
////
	CMoneyDlg* GetMoneyDialog()	{	return m_MoneyDlg;	}
	void SetMoneyDialog(CMoneyDlg* dlg)	{	m_MoneyDlg = dlg;	}
	CQuickDialog* GetQuickDialog()	{	return m_pQuickDlg;	}
	void SetQuickDialog(CQuickDialog* dlg)	{	m_pQuickDlg = dlg;	}

//KES
	CMacroDialog * GetMacroDialog(){ return m_pMacroDlg; }
	void SetMacroDialog(CMacroDialog * dlg){ m_pMacroDlg=dlg; }
//	CMacroModeDialog * GetMacroModeDialog(){ return m_pMacroModeDlg; }
//	void SetMacroModeDialog(CMacroModeDialog * dlg){ m_pMacroModeDlg=dlg; }
	CChatOptionDialog * GetChatOptionDialog() { return m_pChatOptionDlg; }
	void SetChatOptionDialog(CChatOptionDialog * dlg) { m_pChatOptionDlg = dlg; }	
	COptionDialog * GetOptionDialog() { return m_pOptionDlg; }
	void SetOptionDialog(COptionDialog * dlg) { m_pOptionDlg = dlg; }
	CExitDialog * GetExitDialog() { return m_pExitDlg; }
	void SetExitDialog(CExitDialog * dlg) { m_pExitDlg = dlg; }
	CCharStateDialog* GetCharStateDialog() { return m_pCharStateDlg; }
	void SetCharStateDialog(CCharStateDialog* dlg) { m_pCharStateDlg = dlg; }

	void SetMenuSlotDialog( CMenuSlotDialog* pDlg ) { m_pMenuSlotDlg = pDlg; }
	CMenuSlotDialog* GetMenuSlotDialog() { return m_pMenuSlotDlg; }

	void SetDissolutionDialog( CDissolutionDialog* pDlg ) { m_pDissolutionDlg = pDlg; }
	CDissolutionDialog* GetDissolutionDialog() { return m_pDissolutionDlg; }

	void SetEventNotifyDialog( CEventNotifyDialog* pDlg ) { m_pEventNotifyDlg = pDlg; }
	CEventNotifyDialog* GetEventNotifyDialog() { return m_pEventNotifyDlg; }

	void SetMallNoticeDialog( CMallNoticeDialog* pDlg ) { m_pMallNoticeDlg = pDlg; }
	CMallNoticeDialog* GetMallNoticeDialog() { return m_pMallNoticeDlg; }
//
	cAuctionBoard * GetAuctionBoard(){ return m_pAuctionBoard; }
	void SetAuctionBoard(cAuctionBoard * dlg){ m_pAuctionBoard=dlg; }

	CGuageDialog * GetGuageDlg(){ return m_pGuageDlg; }
	void SetGuageDlg(CGuageDialog * dlg){ m_pGuageDlg=dlg; }

	CPartyDialog* GetPartyDialog() { return &m_PartyDlg; }
	
	CPartyCreateDlg* GetPartyCreateDialog() { return m_pPartyCreateDlg;	}
	void SetPartyCreateDialog(CPartyCreateDlg* dlg)	{ m_pPartyCreateDlg = dlg;	}

	CPartyInviteDlg* GetPartyInviteDialog() { return m_pPartyInviteDlg;	}
	void SetPartyInviteDialog(CPartyInviteDlg* dlg)	{ m_pPartyInviteDlg = dlg;	}
	
	CGuildCreateDialog* GetGuildCreateDlg() { return m_pGuildCreateDlg; }
	void SetGuildCreateDlg(CGuildCreateDialog* dlg) { m_pGuildCreateDlg = dlg;	}

	CGuildUnionCreateDialog* GetGuildUnionCreateDlg() { return m_pGuildUnionCreateDlg; }
	void SetGuildUnionCreateDlg(CGuildUnionCreateDialog* dlg) { m_pGuildUnionCreateDlg = dlg;	}	
	
	CGuildDialog* GetGuildDlg() { return m_pGuildDlg; }
	void SetGuildDlg(CGuildDialog* dlg) { m_pGuildDlg = dlg;	}

	CGuildInviteDialog* GetGuildInviteDlg() { return m_pGuildInviteDlg; }
	void SetGuildInviteDlg(CGuildInviteDialog* dlg) { m_pGuildInviteDlg = dlg;	}
	
	CGuildMarkDialog* GetGuildMarkDlg() { return m_pGuildMarkDlg; }
	void SetGuildMarkDlg(CGuildMarkDialog* dlg) { m_pGuildMarkDlg = dlg;	}
	
	CGuildLevelUpDialog* GetGuildLevelUpDlg() { return m_pGuildLevelUpDlg; }
	void SetGuildLevelUpDlg(CGuildLevelUpDialog* dlg) { m_pGuildLevelUpDlg = dlg;	}

	CGuildNickNameDialog* GetGuildNickNameDlg() { return m_pGuildNickNameDlg; }
	void SetGuildNickNameDlg(CGuildNickNameDialog* dlg) { m_pGuildNickNameDlg = dlg;	}

	CGuildRankDialog* GetGuildRankDlg() { return m_pGuildRankDlg; }
	void SetGuildRankDlg(CGuildRankDialog* dlg) { m_pGuildRankDlg = dlg;	}
	
	CGuildWarehouseDialog* GetGuildWarehouseDlg() { return m_pGuildWarehouseDlg; }
	void SetGuildWarehouseDlg(CGuildWarehouseDialog* dlg) { m_pGuildWarehouseDlg = dlg;	}

	CMiniMapDlg* GetMiniMapDialog() { return m_pMiniMapDlg; 	}
	void SetMiniMapDialog(CMiniMapDlg* dlg) { m_pMiniMapDlg = dlg; 	}

	CBigMapDlg* GetBigMapDialog() { return m_pBigMapDlg; 	}
	void SetBigMapDialog(CBigMapDlg* dlg) { m_pBigMapDlg = dlg; 	}

	CPyogukDialog* GetPyogukDialog() { return m_pPyogukDialog;	}
	void SetPyogukDialog(CPyogukDialog* dlg) { m_pPyogukDialog = dlg; 	}
	
	CFriendDialog* GetFriendDialog() { return m_pFriendDialog; }
	void SetFriendDialog(CFriendDialog* dlg) { m_pFriendDialog = dlg; }

	CNoteDialog* GetNoteDialog() { return m_pNoteDialog; }
	void SetNoteDialog(CNoteDialog* dlg) { m_pNoteDialog = dlg;	}
	
	CMiniNoteDialog* GetMiniNoteDialog() { return m_pMiniNoteDialog;	}
	void SetMiniNoteDialog(CMiniNoteDialog* dlg) { m_pMiniNoteDialog = dlg;	}
	
	CMiniFriendDialog * GetMiniFriendDialog() { return m_pMiniFriendDialog; 	}
	void SetMiniFriendDialog(CMiniFriendDialog* dlg) { m_pMiniFriendDialog = dlg;	}
	
	CReviveDialog * GetReviveDialog() { return m_pReviveDialog; 	}
	void SetReviveDialog(CReviveDialog* dlg) { m_pReviveDialog = dlg;	}

	CWantNpcDialog * GetWantNpcDialog() { return m_pWantNpcDialog; 	}
	void SetWantNpcDialog(CWantNpcDialog* dlg) { m_pWantNpcDialog = dlg; }
	
	CWantRegistDialog * GetWantRegistDialog() { return m_pWantRegistDialog; }
	void SetWantRegistDialog(CWantRegistDialog * dlg) { m_pWantRegistDialog = dlg;	}

	CQuestTotalDialog * GetQuestTotalDialog() { return m_pQuestTotalDlg;	}
	void SetQuestTotalDialog(CQuestTotalDialog * dlg) { m_pQuestTotalDlg = dlg;	}
	CQuestDialog* GetQuestDialog()			{ return m_pQuestDlg;	}
	void SetQuestDialog(CQuestDialog* dlg)	{ m_pQuestDlg = dlg;	}
	CWantedDialog* GetWantedDialog() { return m_pWantedDlg; }
	void SetWantedDialog(CWantedDialog* dlg) { m_pWantedDlg = dlg;	}
	CJournalDialog* GetJournalDialog() { return m_pJournalDlg;	}
	void SetJournalDialog(CJournalDialog* dlg) { m_pJournalDlg = dlg; }

	void SetMPRegistDialog(CMPRegistDialog * dlg) { m_pMPRegistDialog = dlg; }
	CMPRegistDialog* GetMPRegistDialog() { return m_pMPRegistDialog;	}
	void SetMPMissionDialog(CMPMissionDialog* dlg) { m_pMPMissionDialog = dlg; }
	CMPMissionDialog* GetMPMissionDialog() { return m_pMPMissionDialog;	}
	void SetMPNoticeDialog(CMPNoticeDialog* dlg) { m_pMPNoticeDialog = dlg; }
	CMPNoticeDialog* GetMPNoticeDialog() { return m_pMPNoticeDialog;	}
	void SetMPGuageDialog(CMPGuageDialog* dlg) { m_pMPGuageDialog = dlg; }
	CMPGuageDialog* GetMPGuageDialog() { return m_pMPGuageDialog;	}

	void SetPetStateDialog(CPetStateDlg* dlg) { m_pPetStateDlg = dlg; }
	CPetStateDlg* GetPetStateDialog() { return m_pPetStateDlg; }
	void SetPetStateMiniDialog(CPetStateMiniDlg* dlg) { m_pPetStateMiniDlg = dlg; }
	CPetStateMiniDlg* GetPetStateMiniDialog() { return m_pPetStateMiniDlg; }
	void SetPetInventoryDialog(CPetInventoryDlg* dlg) { m_pPetInventoryDlg = dlg; }
	CPetInventoryDlg* GetPetInventoryDialog() { return m_pPetInventoryDlg; }
	void SetPetUpgradeDialog(CPetUpgradeDialog* dlg) {	m_pPetUpgradeDlg = dlg;	}
	CPetUpgradeDialog* GetPetUpgradeDialog() {	return m_pPetUpgradeDlg;	}
	void SetPetRevivalDialog(CPetRevivalDialog* dlg) {	m_pPetRevivalDlg = dlg;	}
	CPetRevivalDialog* GetPetRevivalDialog() {	return m_pPetRevivalDlg;	}

	CBailDialog* GetBailDialog()			{	return m_pBailDlg;	}
	void SetBailDialog(CBailDialog* dlg)	{	m_pBailDlg = dlg; }

	CItemShopDialog* GetItemShopDialog()			{	return m_pItemShopDialog;	}
	void SetItemShopDialog(CItemShopDialog* dlg)	{	m_pItemShopDialog = dlg; 	}
	
	CMoveDialog* GetMoveDialog()					{	return m_pMoveDialog;	}
	void SetMoveDialog(CMoveDialog* dlg)			{	m_pMoveDialog = dlg;	}
	CPointSaveDialog* GetPointSaveDialog()			{	return m_pPointSaveDialog;	}
	void SetPointSaveDialog(CPointSaveDialog* dlg)	{	m_pPointSaveDialog = dlg;	}

	// guildfieldwar
	CGFWarDeclareDlg* GetGFWarDeclareDlg()			{	return m_pGFWarDeclareDlg;	}
	void SetGFWarDeclareDlg(CGFWarDeclareDlg* dlg)	{	m_pGFWarDeclareDlg = dlg;	}
	CGFWarResultDlg* GetGFWarResultDlg()			{	return m_pGFWarResultDlg;	}
	void SetGFWarResultDlg(CGFWarResultDlg* dlg)	{	m_pGFWarResultDlg = dlg;	}
	CGFWarInfoDlg* GetGFWarInfoDlg()				{	return m_pGFWarInfoDlg;	}
	void SetGFWarInfoDlg(CGFWarInfoDlg* dlg)		{	m_pGFWarInfoDlg = dlg;	}
	CGuildWarInfoDlg* GetGuildWarInfoDlg()			{	return m_pGuildWarInfoDlg;	}
	void SetGuildWarInfoDlg(CGuildWarInfoDlg* dlg)	{	m_pGuildWarInfoDlg = dlg;	}
	CGuildPlusTimeDlg* GetGuildPlusTimeDlg()			{	return m_pGuildPlusTimeDlg;	}//Add 060803 by wonju
	void SetGuildPlusTimeDlg(CGuildPlusTimeDlg* dlg)	{	m_pGuildPlusTimeDlg = dlg;	}//Add 060803 by wonju
	
	// 
	CSkillPointRedist*	GetSkPointDlg()					{	return m_pSkPointDlg;	}
	void SetSkPointDlg(CSkillPointRedist* dlg)			{	m_pSkPointDlg = dlg;	}
	CSkillPointNotify*	GetSkPointNotifyDlg()			{	return m_pSkPointNotifyDlg;	}
	void SetSkPointNotifydlg(CSkillPointNotify* dlg)	{	m_pSkPointNotifyDlg = dlg;	}

	// partywar
	CPartyWarDialog*	GetPartyWarDlg()				{	return m_pPartyWarDlg;	}
	void SetPartyWarDlg(CPartyWarDialog* dlg)			{	m_pPartyWarDlg = dlg;	}

	// 외치기
	CShoutDialog*	GetShoutDlg()						{	return m_pShoutDlg;		}
	void SetShoutDlg(CShoutDialog* dlg)					{	m_pShoutDlg = dlg;		}
	CShoutchatDialog*	GetShoutchatDlg()				{	return m_pShoutchatDlg;		}
	void SetShoutchatDlg(CShoutchatDialog* dlg)			{	m_pShoutchatDlg = dlg;		}

	//
	CChaseDialog*	GetChaseDlg()						{	return m_pChaseDlg;		}
	void SetChaseDlg(CChaseDialog* dlg)					{	m_pChaseDlg = dlg;		}
	CChaseinputDialog*	GetChaseinputDlg()					{	return m_pChaseinputDlg;	}
	void SetChaseinputDlg(CChaseinputDialog* dlg)			{	m_pChaseinputDlg = dlg;		}

	//
	CNameChangeDialog*	GetNameChangeDlg()				{	return m_pNameChangeDlg;	}
	void SetNameChangeDlg(CNameChangeDialog* dlg)		{	m_pNameChangeDlg = dlg;		}
	CNameChangeNotifyDlg*	GetNameChangeNotifyDlg()		{	return m_pNameChangeNotifyDlg;	}
	void SetNameChangeNotifyDlg(CNameChangeNotifyDlg* dlg)	{	m_pNameChangeNotifyDlg = dlg;		}

	//
	CRareCreateDialog*	GetRareCreateDlg()				{	return m_pRareCreateDlg;	}
	void SetRareCreateDialog( CRareCreateDialog* dlg )	{	m_pRareCreateDlg = dlg;		}

	//스크린샷
	CScreenShotDlg*	GetScreenShotDlg()				{	return m_pScreenShotDlg;	}
	void SetScreenShotDlg( CScreenShotDlg* dlg )	{	m_pScreenShotDlg = dlg;		}

	//SW061019 서바이벌모드
	CSurvivalCountDialog* GetSurvivalCountDlg()		{	return m_pSurvivalCountDlg;	}
	void SetSurvivalCountDialog(CSurvivalCountDialog* dlg)	{ m_pSurvivalCountDlg = dlg;	}

	BOOL	m_bDieInBossMap;
	int		m_nCountInBossMap;
	DWORD	m_dwTimeInBossMap;

	void	ProcessInBossMap();


	// Guild Tournament
	CGTRegistDialog*	GetGTRegistDlg()					{	return m_pGTRegistDlg;			}
	void SetGTRegistDlg(CGTRegistDialog* dlg)				{	m_pGTRegistDlg = dlg;			}
	CGTRegistcancelDialog*	GetGTRegistcancelDlg()			{	return m_pGTRegistcancelDlg;	}
	void SetGTRegistcancelDlg(CGTRegistcancelDialog* dlg)	{	m_pGTRegistcancelDlg = dlg;		}
	CGTStandingDialog*	GetGTStandingDlg()					{	return m_pGTStandingDlg;		}
	void SetGTStandingDlg(CGTStandingDialog* dlg)			{	m_pGTStandingDlg = dlg;			}
	CGTBattleListDialog*	GetGTBattleListDlg()			{	return m_pGTBattleListDlg;		}
	void SetGTBattleListDlg(CGTBattleListDialog* dlg)		{	m_pGTBattleListDlg = dlg;		}
	CGTScoreInfoDialog*	GetGTScoreInfoDlg()					{	return m_pGTScoreInfoDlg;		}
	void SetGTScoreInfoDlg(CGTScoreInfoDialog* dlg)			{	m_pGTScoreInfoDlg = dlg;		}

	// seigewar
	CSWTimeRegDlg*		GetSWTimeRegDlg()					{ return m_pSWTimeRegDlg; }
	void				SetSWTimeRegDlg( CSWTimeRegDlg* dlg )	{ m_pSWTimeRegDlg = dlg; }
	CSWProtectRegDlg*	GetSWProtectRegDlg()				{ return m_pSWProtectRegDlg; }
	void				SetSWProtectRegDlg( CSWProtectRegDlg* dlg )	{ m_pSWProtectRegDlg = dlg; }
	CSWInfoDlg*			GetSWInfoDlg()						{ return m_pSWInfoDlg; }
	void				SetSWInfoDlg( CSWInfoDlg* dlg )		{ m_pSWInfoDlg = dlg; }
	CSWProfitDlg*		GetSWProfitDlg()					{ return m_pSWProfitDlg; }
	void				SetSWProfitDlg( CSWProfitDlg* dlg )	{ m_pSWProfitDlg = dlg; }
	CSWEngraveDialog*	GetSWEngraveDlg()					{ return m_pEngraveDlg;	}
	void				SetSWEngraveDlg( CSWEngraveDialog* dlg)	{	m_pEngraveDlg = dlg;	}
	CSWTimeDialog*		GetSWTimeDlg()						{ return m_pSWTimeDlg;	}
	void				SetSWTimeDlg( CSWTimeDialog* dlg)	{ m_pSWTimeDlg = dlg;	}
	CSWStartTimeDialog*	GetSWStartTimeDlg()					{ return m_pSWStartTimeDlg;	}
	void				SetSWStartTimeDlg( CSWStartTimeDialog* dlg )	{ m_pSWStartTimeDlg = dlg; }
	
	// Character change
	CCharChangeDlg* GetCharChangeDlg()						{	return m_pCharChangeDlg;		}
	void SetCharChangeDlg( CCharChangeDlg* dlg )			{	m_pCharChangeDlg = dlg;			}

	// ItemSeal
	CSealDialog* GetSealDlg()								{	return m_pSealDlg;			}
	void SetSealDlg( CSealDialog* dlg )						{	m_pSealDlg = dlg;			}

	// 
	CChangeJobDialog*	GetChangeJobDlg()					{	return m_pChangeJobDlg;		}
	void				SetChangeJobDlg( CChangeJobDialog* dlg )		{	m_pChangeJobDlg = dlg;		}

	// ReinforceReset
	CReinforceResetDlg*	GetReinforceResetDlg()				{	return m_pReinforceResetDlg;	}
	void				SetReinforceResetDlg( CReinforceResetDlg* dlg )		{	m_pReinforceResetDlg = dlg;		}

	// 06. 02. 강화 재료 안내 인터페이스 추가 - 이영준
	CReinforceDataGuideDlg* GetReinforceDataGuideDlg()	{ return m_pReinforceDataGuideDlg; }
	void SetReinforceDataGuideDlg(CReinforceDataGuideDlg* dlg) { m_pReinforceDataGuideDlg = dlg; }

	// 06. 02. 내정보창 팁보기 추가 - 이영준
	CTipBrowserDlg* GetTipBrowserDlg() { return m_pTipBrowserDlg; }
	void SetTipBrowserDlg(CTipBrowserDlg* dlg) { m_pTipBrowserDlg = dlg; }

	CKeySettingTipDlg* GetKeySettingTipDlg() { return m_pKeySettingTipDlg; }
	void SetKeySettingTipDlg(CKeySettingTipDlg* dlg) { m_pKeySettingTipDlg = dlg; }

	CGuildNoteDlg*	GetGuildNoteDlg() { return	m_pGuildNoteDlg; }
	void SetGuildNoteDlg(CGuildNoteDlg* dlg) { m_pGuildNoteDlg = dlg; }
	CUnionNoteDlg*	GetUnionNoteDlg() { return	m_pUnionNoteDlg; }
	void SetUnionNoteDlg(CUnionNoteDlg* dlg) { m_pUnionNoteDlg = dlg; }

	// 06. 03. 문파공지 - 이영준
	CGuildNoticeDlg* GetGuildNoticeDlg() { return m_pGuildNoticeDlg; }
	void SetGuildNoticeDlg(CGuildNoticeDlg *dlg) { m_pGuildNoticeDlg = dlg; }

	////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 무공 변환
	CSkillOptionChangeDlg* GetSkillOptionChangeDlg() { return m_pSkillOptionChangeDlg; }
	void SetSkillOptionChangeDlg(CSkillOptionChangeDlg *dlg) { m_pSkillOptionChangeDlg = dlg; }
	
	CSkillOptionClearDlg* GetSkillOptionClearDlg() { return m_pSkillOptionClearDlg; }
	void SetSkillOptionClearDlg(CSkillOptionClearDlg *dlg) { m_pSkillOptionClearDlg = dlg; }
	////////////////////////////////////////////////////////////////////////

	//SW060713 문하생
	CGuildJoinDialog* GetGuildJoinDialog() {	return m_pGuildInvitationKindSelectDlg;	}
	void SetGuildJoinDialog(CGuildJoinDialog* dlg)	{	m_pGuildInvitationKindSelectDlg = dlg;	}
	CGuildMunhaDialog* GetGuildMunhaDialog()				{	return m_pGuildMunhaDlg;	}
	void SetGuildMunhaDialog(CGuildMunhaDialog* dlg)		{	m_pGuildMunhaDlg = dlg;		}
	CGuildTraineeDialog* GetGuildTraineeDialog()			{	return m_pGuildTraineeDlg;	}
	void SetGuildTraineeDialgo(CGuildTraineeDialog* dlg)	{	m_pGuildTraineeDlg = dlg;	}

	// magi82 - UniqueItem(070703)
	CUniqueItemCurseCancellationDlg* GetUniqueItemCurseCancellationDlg() {	return m_pUniqueItemCurseCancellationDlg;	}
	void SetUniqueItemCurseCancellationDlg(CUniqueItemCurseCancellationDlg* dlg) {	m_pUniqueItemCurseCancellationDlg = dlg;	}

	// magi82 - UniqueItem(070709)
	CUniqueItemMixDlg* GetUniqueItemMixDlg() {	return m_pUniqueItemMixDlg;	}
	void SetUniqueItemMixDlg(CUniqueItemMixDlg* dlg) {	m_pUniqueItemMixDlg = dlg;	}

	// magi82 - SOS(070724)
	CSOSDlg* GetSOSDlg()	{	return m_pSOSDlg;	}
	void SetSOSDlg(CSOSDlg* dlg)	{	m_pSOSDlg = dlg;	}	

	// 2007. 10. 22. CBH - 유니크 아이탬 처리 바 게이지 /////////////////////////////////
	CUniqueItemMixProgressBarDlg* GetUniqueItemMixProgressBarDlg(){ return m_pUniqueItemMixProgressBarDlg; }
	void SetUniqueItemMixProgressBarDlg(CUniqueItemMixProgressBarDlg * dlg){ m_pUniqueItemMixProgressBarDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////	

	// 2007. 10. 22. CBH - 타이탄 조합 게이지 /////////////////////////////////
	CTitanMixProgressBarDlg* GetTitanMixProgressBarDlg(){ return m_pTitanMixProgressBarDlg; }
	void SetTitanMixProgressBarDlg(CTitanMixProgressBarDlg * dlg){ m_pTitanMixProgressBarDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////	

	// 2007. 10. 23. CBH - 타이탄 파츠 조합 게이지 /////////////////////////////////
	CTitanPartsProgressBarDlg* GetTitanPartsProgressBarDlg(){ return m_pTitanPartsProgressBarDlg; }
	void SetTitanPartsProgressBarDlg(CTitanPartsProgressBarDlg * dlg){ m_pTitanPartsProgressBarDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////	

	// 2007. 12. 4. CBH - 스킨 선택 다이얼로그 /////////////////////////////////
	CSkinSelectDialog* GetSkinSelectDlg(){ return m_pSkinSelectDlg; }
	void SetSkinSelectDlg(CSkinSelectDialog * dlg){ m_pSkinSelectDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////	

	// 2008. 1. 16. CBH - 코스튬 스킨 선택 다이얼로그 /////////////////////////////////
	CCostumeSkinSelectDialog* GetCostumeSkinSelectDlg(){ return m_pCostumeSkinSelectDlg; }
	void SetCostumeSkinSelectDlg(CCostumeSkinSelectDialog * dlg){ m_pCostumeSkinSelectDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////	

    // magi82(42) - 샵아이템 추가(수련치 초기화)
	CSkillPointResetDlg* GetSkillPointResetDlg()	{	return m_pSkillPointResetDlg;	}
	void SetSkillPointResetDlg(CSkillPointResetDlg* dlg)	{	m_pSkillPointResetDlg = dlg;	}

	// magi82(47)
	CStreetStallItemViewDlg* GetStreetStallItemViewDlg()	{	return m_pStreetStallItemViewDlg;	}
	void SetStreetStallItemViewDlg(CStreetStallItemViewDlg* dlg)	{	m_pStreetStallItemViewDlg = dlg;	}

	// 2008. 3. 17. CBH - 보안번호 다이얼로그 /////////////////////////////////
	CNumberPadDialog* GetNumberPadDlg(){ return m_pNumberPadDlg; }
	void SetNumberPadDlg(CNumberPadDialog * dlg){ m_pNumberPadDlg=dlg; }

	// autonote
	CAutoNoteDlg* GetAutoNoteDlg()				{ return m_pAutoNoteDlg; }
	void SetAutoNoteDlg( CAutoNoteDlg* dlg )	{ m_pAutoNoteDlg = dlg; }
	CAutoAnswerDlg* GetAutoAnswerDlg()				{ return m_pAutoAnswerDlg; }
	void SetAutoAnswerDlg( CAutoAnswerDlg* dlg )	{ m_pAutoAnswerDlg = dlg; }

	// 요새전
	CFWEngraveDialog* GetFWEngraveDlg()				{ return m_pFWEngraveDlg; }
	void SetFWEngraveDlg( CFWEngraveDialog* dlg )	{ m_pFWEngraveDlg = dlg; }
	CFWTimeDialog* GetFWTimeDlg()					{ return m_pFWTimeDlg; }
	void SetFWTimeDlg( CFWTimeDialog* dlg )			{ m_pFWTimeDlg = dlg; }
	CFWWareHouseDialog* GetFWWareHouseDlg()				{ return m_pFWWareHouseDlg; }
	void SetFWWareHouseDlg( CFWWareHouseDialog* dlg )	{ m_pFWWareHouseDlg = dlg; }
	//////////////////////////////////////////////////////////////////////////	

	// 2008. 5. 23. CBH - 방파 매칭 다이얼로그 /////////////////////////////////
	CPartyMatchingDlg* GetPartyMatchingDlg(){ return m_pPartyMatchingDlg; }
	void SetPartyMatchingDlg(CPartyMatchingDlg * dlg){ m_pPartyMatchingDlg=dlg; }
	//////////////////////////////////////////////////////////////////////////	

	//캐릭터 로그인 시간
	SYSTEMTIME	GameinServerTime;			//서버에서 받아온 캐릭터 로그인 시간
	DWORD				m_Clientdate;		//클라이언트에서 받아온 캐릭터 로그인 날짜
	DWORD				m_Clienttime;		//클라이언트에서 받아온 캐릭터 로그인 시간

	void	SetLoginTime(SYSTEMTIME Time);
	SYSTEMTIME	GetLoginTime();

	void	GetClientLoginTime(DWORD& date, DWORD& time);
	void	SetClientLoginTime(DWORD date, DWORD time);

	//////////////////////////////////////////////////////////////////////////

	//Flag
	unsigned				m_uiAccTick;
	int						m_iMilliSecend;
	unsigned				m_uiCurrentTime;
	unsigned				m_uiPreTime;
	float					m_fFramePerSec;
	int						m_iFPSCnt;
	//////////////////////////////////////////////////////////////////////////	
};


EXTERNGLOBALTON(CGameIn)


#endif // !defined(AFX_GAMEIN_H__E9C036AE_2344_473B_BEFE_21C4555A0685__INCLUDED_)
