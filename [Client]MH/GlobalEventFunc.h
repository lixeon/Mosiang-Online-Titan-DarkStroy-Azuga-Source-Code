
#ifndef _GLOBALEVENTFUNC_H_
#define _GLOBALEVENTFUNC_H_

/* 메인 타이틀 */
struct FUNC
{
	void (*_func)(LONG lId, void * p, DWORD we);
	const char * _funcName;
};
extern FUNC g_mt_func[];
extern int FUNCSEARCH(char * funcName);


//pjs 로그아이디 창에서의 버튼 처리...
extern void		MT_LogInOkBtnFunc(LONG lId, void* p, DWORD we);	 
extern void		MT_ExitBtnFunc(LONG lId, void* p, DWORD we);
extern void		MT_EditReturnFunc(LONG lId, void* p, DWORD we); //3
extern void		MT_DynamicBtnFunc(LONG lId, void* p, DWORD we); //3
extern void		MT_DynamicEditReturnFunc(LONG lId, void* p, DWORD we);

//pjs 로그캐렉터 창에서 버튼 처리...
extern void		CS_BtnFuncCreateChar(LONG lId, void* p, DWORD we);	 
extern void		CS_BtnFuncDeleteChar(LONG lId, void* p, DWORD we);	 
extern void		CS_BtnFuncFirstChar(LONG lId, void* p, DWORD we);	 
extern void		CS_BtnFuncSecondChar(LONG lId, void* p, DWORD we);	 
extern void		CS_BtnFuncThirdChar(LONG lId, void* p, DWORD we);	 
extern void		CS_BtnFuncFourthchar(LONG lId, void* p, DWORD we);	 
extern void		CS_BtnFuncFifthchar(LONG lId, void* p, DWORD we);	 
extern void		CS_BtnFuncEnter(LONG lId, void* p, DWORD we);
extern void		CS_FUNC_OkISee(LONG lId, void* p, DWORD we);	 //8->11
extern void		CS_BtnFuncLogOut(LONG lId, void* p, DWORD we);	 //9->12
//케리터 생성창 [5/13/2003]
extern void	CM_OverlapCheckBtnFunc(LONG lId, void* p, DWORD we);
extern void	CM_ComboBoxCheckFunc(LONG lId, void* p, DWORD we);
extern void	CM_CharMakeBtnFunc(LONG lId, void* p, DWORD we);
extern void	CM_CharCancelBtnFunc(LONG lId, void* p, DWORD we);
extern void CMFUNC_OkISee(LONG lId, void* p, DWORD we); //5->16
extern void CM_OptionSelectBtnFunc(LONG lId, void* p, DWORD we);

/*main interface*/
extern void MI_CharBtnFunc(LONG lId, void* p, DWORD we);
extern void MI_ExchangeBtnFunc(LONG lId, void* p, DWORD we);
extern void MI_InventoryBtnFunc(LONG lId, void* p, DWORD we);
extern void MI_MugongBtnFunc(LONG lId, void* p, DWORD we);
//extern void MI_SystemBtnFunc(LONG lId, void* p, DWORD we);
extern void MI_ChatEditBoxFunc(LONG lId, void * p, DWORD we); //7->23
extern void MI_DlgFunc(LONG lId, void * p, DWORD we);

/*Inventory interface*/
extern void IN_CreateStreetStallFunc(LONG lId, void * p, DWORD we);
extern void IN_DlgFunc(LONG lId, void * p, DWORD we);
/*Mugong Dialog Interface*/
extern void MGI_DlgFunc(LONG lId, void * p, DWORD we);
extern void MGI_MugongDlgFunc(LONG lId, void * p, DWORD we);
extern void MGI_SuryunDlgFunc(LONG lId, void * p, DWORD we);
/*character infomation interface*/
extern void CI_DlgFunc(LONG lId, void * p, DWORD we);
extern void CI_AddExpPoint(LONG lId, void * p, DWORD we);
/*party interface*/
extern void PA_DlgFunc(LONG lId, void * p, DWORD we); //5->28

/*Quick Dialog Interface */
extern void QI_QuickDlgFunc(LONG lId, void * p, DWORD we);

// LYJ 051017 구입노점상 추가
extern void SO_DlgFunc(LONG lId, void * p, DWORD we);

extern void BRS_DlgFunc(LONG lId, void * p, DWORD we);
extern void BRS_CloseFunc(LONG lId, void * p, DWORD we);
extern void BRS_DeleteFunc(LONG lId, void * p, DWORD we);
extern void BRS_TypeListFunc(LONG lId, void * p, DWORD we);
extern void BRS_ItemListFunc(LONG lId, void * p, DWORD we);
extern void BRS_ClassListFunc(LONG lId, void * p, DWORD we);
extern void BRS_REGBtnFunc(LONG lId, void * p, DWORD we);
extern void BRS_VolumeEditFunc(LONG lId, void * p, DWORD we);
extern void BRS_MoneyEditFunc(LONG lId, void * p, DWORD we);

extern void BS_DlgFunc(LONG lId, void * p, DWORD we);
extern void BS_TitleEditBoxFunc(LONG lId, void * p, DWORD we);
extern void BS_MoneyEditBoxFunc(LONG lId, void * p, DWORD we);
extern void BS_SellBtnFunc(LONG lId, void * p, DWORD we);
extern void BS_REGBtnFunc(LONG lId, void * p, DWORD we);
extern void BS_SPFunc(LONG lId, void * p, DWORD we);

/*StreetStall interface*/
extern void SSI_DlgFunc(LONG lId, void * p, DWORD we);
extern void SSI_TitleEditBoxFunc(LONG lId, void * p, DWORD we);
extern void SSI_MoneyEditBoxFunc(LONG lId, void * p, DWORD we);
extern void SSI_BuyBtnFunc(LONG lId, void * p, DWORD we);
extern void SSI_RegistBtnFunc(LONG lId, void * p, DWORD we);
extern void SSI_EditBtnFunc(LONG lId, void * p, DWORD we);	//6->34

/*NPC 대사창*/
extern void NSI_DlgFunc(LONG lId, void * p, DWORD we);
extern void NSI_HyperLinkFunc(LONG lId, void * p, DWORD we);

/*Help창*/
extern void HI_DlgFunc(LONG lId, void * p, DWORD we);
extern void HI_HyperLinkFunc(LONG lId, void * p, DWORD we);
extern void HI_BtnFunc(LONG lId, void * p, DWORD we);
extern void HSI_ExitBtnFunc(LONG lId, void * p, DWORD we);

/*공통창*/
extern void CMI_MoneyOkFunc(LONG lId, void * p, DWORD we);
extern void CMI_MoneySpinFunc(LONG lId, void * p, DWORD we);
extern void CMI_AlertFunc(LONG lId, void * p, DWORD we);

/*상점창*/
extern void DE_DlgFunc(LONG lId, void * p, DWORD we);
extern void DE_BuyItemFunc(LONG lId, void * p, DWORD we);
extern void DE_SellItemFunc(LONG lId, void * p, DWORD we);
extern void DE_CloseHideBtn(LONG lId, void * p, DWORD we); //7->41
/*파티*/
extern void PA_BtnFunc(LONG lId, void * p, DWORD we);

/*길드*/
extern void GD_DlgFunc( LONG lId, void* p, DWORD we );

/*표국*/
extern void PYO_DlgBtnFunc(LONG lId, void * p, DWORD we);
extern void Note_DlgBtnFunc(LONG lId, void * p, DWORD we);
extern void Friend_DlgBtnFunc(LONG lId, void * p, DWORD we);

/*캐릭터 재생*/
extern void CR_DlgBtnFunc(LONG lId, void * p, DWORD we);

/*현상금NPC*/
extern void WANTNPC_DlgFunc( LONG lId, void* p, DWORD we );
extern void WANTREG_DlgFunc( LONG lId, void* p, DWORD we );

/*퀘스트*/
extern void QUE_QuestDlgFunc( LONG lId, void* p, DWORD we );
extern void QUE_JournalDlgFunc( LONG lId, void* p, DWORD we );

/*수련장*/
extern void MP_RegistDlgFunc( LONG lId, void* p, DWORD we );

/*보석금*/
extern void BAIL_BailDlgFunc( LONG lId, void* p, DWORD we );

//KES DIALOG
/*매크로*/
extern void MAC_DlgFunc(LONG lId, void * p, DWORD we);
//extern void MAC_ModeDlgFunc(LONG lId, void * p, DWORD we);
/*채팅창*/
extern void CTI_DlgFunc(LONG lId, void * p, DWORD we);
extern void COI_DlgFunc(LONG lId, void * p, DWORD we);
extern void GAMEIN_ChatFunc(LONG lId, void * p, DWORD we);
/*옵션창*/
extern void OTI_DlgFunc(LONG lId, void * p, DWORD we);
/*종료창*/
extern void EXT_DlgFunc(LONG lId, void * p, DWORD we);
/*교환창*/
extern void XCI_DlgFunc(LONG lId, void * p, DWORD we);
/*캐릭터상태창*/
extern void CSS_DlgFunc(LONG lId, void * p, DWORD we);
extern void MSI_MenuSlotDlgFunc( LONG lId, void* p, DWORD we );
/*PKLOOTING*/
extern void PLI_DlgFunc( LONG lId, void* p, DWORD we );
/*CNA_BtnOKFunc*/
extern void CNA_BtnOkFunc( LONG lId, void* p, DWORD we );
/*아이템분해*/
extern void DIS_DlgFunc( LONG lId, void* p, DWORD we );
extern void ITEM_DlgFunc( LONG lId, void* p, DWORD we );

/*무림넷*/
extern void MNPRI_DlgFunc(LONG lId, void * p, DWORD we);
extern void MNPRI_ChatFunc( LONG lId, void * p, DWORD we );
extern void MNCNL_DlgFunc(LONG lId, void * p, DWORD we);
extern void MNCNL_ChatFunc(LONG lId, void * p, DWORD we);

/*메세지박스*/
extern void MessageBox_Func(LONG lId, void * p, DWORD we);
extern void DIVIDEBOX_Func(LONG lId, void * p, DWORD we);
/*미니맵*/
extern void MNM_DlgFunc(LONG lId, void * p, DWORD we);

/*등급업 창*/
extern void ITD_DlgFunc(LONG lId, void * p, DWORD we);
/*조합 창*/
extern void ITMD_DlgFunc(LONG lId, void * p, DWORD we);
/*타이탄파츠메이크창*/
extern void TPMD_DlgFunc(LONG lId, void * p, DWORD we);	// magi82 - Titan(070112)
// magi82 - Titan(070119) ////////////////////////////////////////////////
extern void TitanMix_DlgFunc(LONG lId, void * p, DWORD we);	
extern void Titan_Repair_DlgFunc(LONG lId, void * p, DWORD we);	//2007. 9. 11. CBH - 타이탄 수리창 이벤트
extern void Titan_Recall_DlgFunc(LONG lId, void * p, DWORD we); //2007. 9. 14. CBH - 타이탄 소환 이벤트
//////////////////////////////////////////////////////////////////////////
extern void TitanUpgrade_DlgFunc(LONG lId, void * p, DWORD we);	// magi82 - Titan(070131)
extern void TitanBreak_DlgFunc(LONG lId, void * p, DWORD we);	// magi82 - Titan(070205)
extern void titan_inventory_DlgFunc(LONG lId, void * p, DWORD we);	// magi82 - Titan(070226)
extern void Titan_guage_DlgFunc(LONG lId, void * p, DWORD we);		// magi82 - Titan(070306)
//extern void Titan_MugongMix_DlgFunc(LONG lId, void * p, DWORD we);		// magi82 - Titan(070611) 타이탄 무공변환 주석처리
extern void Titan_Use_DlgFunc(LONG lId, void * p, DWORD we);		// magi82 - Titan(070320)
extern void Titan_Bongin_DlgFunc(LONG lId, void * p, DWORD we);		// magi82 - Titan(070323)
/* 강화 창*/
extern void ITR_DlgFunc(LONG lId, void * p, DWORD we);
/*채널 창*/
extern void CHA_DlgBtnFunc(LONG lId, void * p, DWORD we);

/*아이템몰창고*/
extern void ITMALL_DlgBtnFunc(LONG lId, void * p, DWORD we);

/*이동창*/
extern void SA_DlgBtnFunc(LONG lId, void * p, DWORD we);

/*저장위치*/
extern void CHA_DlgFunc(LONG lId, void * p, DWORD we);

// ServerList
extern void SL_DlgBtnFunc( LONG lId, void* p, DWORD we );

// guildfieldwar
extern void GFW_DlgFunc( LONG lId, void* p, DWORD we );

// SkillPoint Redistribute
extern void SK_DlgBtnFunc( LONG lId, void* p, DWORD we );

// partywar
extern void PW_DlgFunc( LONG lId, void* p, DWORD we );

// chase
extern void CHS_DlgFunc( LONG lId, void* p, DWORD we );

//
extern void CHAN_DlgFunc(LONG lId, void * p, DWORD we);

// Guild Tournament
extern void GDT_DlgFunc(LONG lId, void * p, DWORD we);

// SeigeWar
extern void SW_DlgFunc(LONG lId, void* p, DWORD we);

// Character change
extern void CHA_ChangeDlgFunc(LONG lId, void* p, DWORD we);

extern void IT_DlgFunc(LONG lId, void* p, DWORD we);

//
extern void CJOB_DlgFunc(LONG lId, void* p, DWORD we);

extern void CG_DlgFunc(LONG lId, void *p, DWORD we);

extern void RFDefault_DlgFunc(LONG lId, void *p, DWORD we);
extern void RareCreate_DlgFunc(LONG lId, void *p, DWORD we);

// Pet
extern void PET_DlgFunc(LONG lId, void *p, DWORD we);
extern void PET_MiniDlgFunc(LONG lId, void *p, DWORD we);
extern void PET_InvenDlgFunc(LONG lId, void *p, DWORD we);
extern void PET_UpgradeDlgFunc(LONG lId, void *p, DWORD we);
extern void PET_RevivalDlgFunc(LONG lId, void *p, DWORD we);
// 06. 02. 강화 재료 안내 인터페이스 추가 - 이영준
extern void RFDGUIDE_DlgFunc(LONG lId, void *p, DWORD we);

// 06. 02. 내정보창 팁보기 추가 - 이영준
extern void TB_DlgFunc(LONG lId, void *p, DWORD we);
extern void TB_STATE_DlgFunc(LONG lId, void *p, DWORD we);

extern void GN_DlgBtnFunc(LONG lId, void *p, DWORD we);
extern void AN_DlgBtnFunc(LONG lId, void *p, DWORD we);

// 06. 03. 문파공지 - 이영준
extern void GNotice_DlgBtnFunc(LONG lId, void *p, DWORD we);

extern void GuildPlusTime_DlgFunc(LONG IId, void* p, DWORD we);//Add 060803 by wonju

//////////////////////////////////////////////////////////////////
// 06. 06. 2차 전직 - 이영준
// 무공변환
extern void SkillTrans_DlgFunc(LONG lId, void *p, DWORD we);
extern void TDefault_DlgFunc(LONG lId, void *p, DWORD we);
//////////////////////////////////////////////////////////////////

extern void JO_DlgFunc(LONG lId, void* p, DWORD we);
extern void GU_DlgFunc(LONG lId, void* p, DWORD we);	// magi82 - 문하생 가입편리시스템
extern void GT_DlgFunc(LONG lId, void* p, DWORD we);	// magi82 - 문하생 가입편리시스템

extern void UniqueItemCurseCancellationDlgFunc(LONG lId, void* p, DWORD we);	// magi82 - Unique(070703)
extern void UniqueItemMixDlgFunc(LONG lId, void* p, DWORD we);	// magi82 - Unique(070709)

extern void SOSDlgFunc(LONG lId, void* p, DWORD we);	// magi82 - SOS(070724)

// 2007. 10. 19. CBH - 유니크 아이탬 조합 게이지 //////////////////////////////////////
extern void UniqueItemMix_ProgressBar_DlgFunc(LONG lId, void * p, DWORD we);
///////////////////////////////////////////////////////////////////////////
// 2007. 10. 22. CBH - 타이탄 조합 게이지 //////////////////////////////////////
extern void TitanMix_ProgressBar_DlgFunc(LONG lId, void * p, DWORD we);
///////////////////////////////////////////////////////////////////////////
// 2007. 10. 23. CBH - 타이탄 파츠 조합 게이지 //////////////////////////////////////
extern void TitanParts_ProgressBar_DlgFunc(LONG lId, void * p, DWORD we);
///////////////////////////////////////////////////////////////////////////

// 2007. 12. 4. CBH - 스킨 선택 //////////////////////////////////////
extern void SkinSelect_DlgFunc(LONG lId, void * p, DWORD we);
///////////////////////////////////////////////////////////////////////////
// 2008. 1. 16. CBH - 코스튬 스킨 선택 //////////////////////////////////////
extern void CostumeSkinSelect_DlgFunc(LONG lId, void * p, DWORD we);
///////////////////////////////////////////////////////////////////////////

// magi82(42) - 샵아이템 추가(수련치 초기화)
extern void SK_Reset_DlgFunc(LONG lId, void * p, DWORD we);

// 2008. 3. 17. CBH - 보안번호 입력 이벤트  //////////////////////////////////////
extern void NumberPad_DlgFunc(LONG lId, void * p, DWORD we);
//////////////////////////////////////////////////////////////////////////
//2008. 3. 19. CBH - 로그인 다이얼로그 콤보박스 이벤트 추가 //////////////////////
extern void MT_LOGINDLGFunc(LONG lId, void * p, DWORD we);
//////////////////////////////////////////////////////////////////////////

// 노점상 검색 추가 by Stiner(8)
extern void SFR_DlgFunc(LONG lId, void * p, DWORD we);

// 노점상 보기 추가 by Stiner(8)
extern void SVD_DlgFunc(LONG lId, void * p, DWORD we);

// autonote
extern void AutoNoteDlg_Func(LONG lId, void * p, DWORD we);
extern void AutoAnswerDlg_Func(LONG lId, void * p, DWORD we);

///////////////////////////////////////////////////////////////////
// 2008. 5. 21. CBH - 파티 생성 인터페이스 버튼 이벤트 추가
extern void PA_CreateBtnFunc(LONG lId, void * p, DWORD we);
//2008. 6. 2. CBH - 파티 매칭 목록 다이얼로그 이벤트
extern void Party_MatchingList_DlgFunc(LONG lId, void * p, DWORD we);
/////////////////////////////////////////////////////////////////////

// 요새전
extern void FW_DlgFunc(LONG lId, void* p, DWORD we);
extern void FD_WareHouseDlgFunc( LONG lId, void* p, DWORD we );

#endif //_GLOBALEVENTFUNC_H_
