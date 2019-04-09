#if !defined(AFX_CHARACTERINFO_H__1719B6E9_61B7_47A7_957A_078826ACA68A__INCLUDED_)
#define AFX_CHARACTERINFO_H__1719B6E9_61B7_47A7_957A_078826ACA68A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CharacterInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCharacterInfo dialog

#include "LDPropSheet.h"
#include "CommonRMControl.h"

#include "QuestInfoDlg.h"

class CCharacterDoc;
class CItemDlg;
class CMugongDlg;
class CAbilityDlg;
class CItemRecoverDlg;

class CCharacterInfo : public CLDPropPage
{
	DECLARE_DYNCREATE(CCharacterInfo)
public:
	CItemRecoverDlg*	m_pItemRecoverDlg;

	WORD			m_wPower;
	CCharacterDoc* m_pDocument;
	CItemDlg*	m_pItemDlg;
	CMugongDlg*	m_pMugongDlg;
	CAbilityDlg* m_pAbility;

	CToolTipCtrl	m_TTctrl;
	int			m_nCurInvenTab;

	// ÀÔ·Â°ª
	CMREdit			m_StatusEdit[eCharTemp_MAX];				//À¯Àú Á¤º¸Ã¢
	//

	CMRButton		m_InvenItem[TABCELL_INVENTORY_NUM];			// InvenÃ¢
	CMRButton		m_WearItem[eWearedItem_Max];

	CMRButton		m_PyoGukItem[TABCELL_PYOGUK_NUM];			// Ç¥±¹Ã¢

	CMRButton		m_SkillMuGong[SLOT_MUGONG_NUM];				// SkillÃ¢
	CMRButton		m_SkillJinBub[SLOT_JINBUB_NUM];

	CMRButton		m_Ability[MAX_ABILITY_NUM_PER_GROUP];		// Æ¯±â¹öÆ°

	CMRButton		m_InvenItemError[10];
	CMRButton		m_WareItemError[5];
	CMRButton		m_MugongError[5];
	
	CMRButton		m_InvenRecover;

// Construction
public:
	CCharacterInfo();
	~CCharacterInfo();

public:
	CQuestInfoDlg m_QuestInfoDlg;
	void	QueryCharacterInfo( );
	void InitControl(CCharacterDoc* pDoc);

	void SetBaseInfo(BASEOBJECT_INFO* objInfo, CHARACTER_TOTALINFO* totalInfo, HERO_TOTALINFO* heroInfo);
	void SetInvenInfo(ITEM_TOTALINFO* inven);
	void SetInvenInfoError(TMSG_CHARACTER_INVENINFO2* inven);
	void SetPyogukInfo(PHYOKUK_TOTALINFO* pyoguk);
	void SetPyogukInfoError(PHYOKUK_TOTALINFO* pinfo );
	void SetMugongInfo(MUGONG_TOTALINFO* mugong);
	void SetMugongInfoError( TMSG_CHARACTER_MUGONGINFO2* pinfo );
	void SetAbilityInfo();

	void GetChangeBaseInfo(TMSG_CHARACTER_UPDATEBASEINFO* msg);
	void GetChangePyogukInfo(TMSG_CHARACTER_PYOGUKINFO* msg);
	
	void ResetCalcInfo();

	void EnablePyogukInfoControl(BOOL bEnable = TRUE);
	
	CMRButton* CheckButtonEvent(HWND hWnd, POSTYPE& pos, BYTE& type);
	void MakeToolTip(BYTE type, POSTYPE pos, WORD idx, char* str);
	void ResetTooltip();

	void ButtonControlCommand(UINT nId);
	void EditControlCommand(UINT nId);

	TABLE_KIND	GetTableKindByControlID(UINT nId);

	BOOL CheckItemPosition( DWORD dwKind, DWORD Pos );

	
////////////////////////////////////////////////
//// ¼öÁ¤ ÇÔ¼ö
	int ItemEdit( INFOFIELD field, WORD wControlId, const ITEMBASE* base_before, ITEMBASE& base_after, 
					const ITEM_OPTION_INFO* before, ITEM_OPTION_INFO& after );
	int MugongEdit( INFOFIELD field, WORD wControlId, MUGONGBASE* base_before, MUGONGBASE& base_after );
	int AbilityEdit( WORD wPos, WORD wAbilityIdx, char base_before, char& base_after );

	///////////////////////////////////////
	// field		: ¾î´ÀÃ¢ÀÇ º¯°æÀÎÁö
	// result		: eMR_Edit, eMR_Delete¸¦ ±¸ºÐ
	// wPos			: Item Position
	// nControlIdx	: ¿¬°á ÄÁÆ®·Ñ À§Ä¡
	// Origin		: DBÀÇ ½ÇÁ¦ µ¥ÀÌÅÍ
	// OriginOption	: DBÀÇ ½ÇÁ¦ µ¥ÀÌÅÍ
	// pBItem		: Local Item Data
	// pBOption		: Local Option Data
	// pAItem		: Change Item Data
	// pAOption		: Change Option Data
	BOOL ResultItemEdit( INFOFIELD field, int result, WORD wPos, int nControlIdx, ITEMBASE* Origin, ITEM_OPTION_INFO* OriginOption,
							ITEMBASE* pBItem, ITEMBASE* pAItem, ITEM_OPTION_INFO* pBOption, ITEM_OPTION_INFO* pAOption );
	///////////////////////////////////////

	void ResultMugongEdit( int result, WORD wPos, int nControlIdx, MUGONGBASE* Origin, MUGONGBASE* pBMugong, MUGONGBASE* pAMugong );
	void ResultAbilityEdit();

	void UpdateMainQuestData( TMSG_MAINQUESTINFO* pMsg );
	void UpdateSubQuestData( TMSG_SUBQUESTINFO* pMsg );

	void ChangeLoginPoint( DWORD dwMapNum );

	int GetEmptyPosition( DWORD dwType );
	void ResultPositionEdit( DWORD dwType, ICONBASE* pItem );
////////////////////////////////////////////////

	int		GetEmptyPositionCount( WORD wType );
	void	RecoverInven( DWORD dwItemIdx, int nCount, int nR );

// Dialog Data
	//{{AFX_DATA(CCharacterInfo)
	enum { IDD = IDD_CHAR_PAGE };
	CButton	m_btnChangeLoginPoint;
	CButton	m_btnSave;
	CComboBox	m_cbLoginPoint;
	CButton	m_btnPyogukAdd;
	CComboBox	m_cbPyoguk;
	CComboBox	m_cbInven;
	CComboBox	m_cbAbility;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCharacterInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCharacterInfo)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchangeInvenCombo();
	afx_msg void OnSelchangePyogukCombo();
	afx_msg void OnSelchangeAbilityCombo();
	virtual BOOL OnInitDialog();
	afx_msg void OnCharacterBtnSave();
	afx_msg void OnCharacterBtnAddpyoguk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnQuestinfo();
	afx_msg void OnButtonChangeLoginpoint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARACTERINFO_H__1719B6E9_61B7_47A7_957A_078826ACA68A__INCLUDED_)
