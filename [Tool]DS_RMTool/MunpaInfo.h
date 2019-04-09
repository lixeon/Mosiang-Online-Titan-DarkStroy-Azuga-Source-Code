#if !defined(AFX_MUNPAINFO_H__F78D1D8A_942E_4D24_A57D_0B47A6F7573F__INCLUDED_)
#define AFX_MUNPAINFO_H__F78D1D8A_942E_4D24_A57D_0B47A6F7573F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MunpaInfo.h : header file
//

#include "LDPropSheet.h"
#include "CommonRMControl.h"

/////////////////////////////////////////////////////////////////////////////
// CMunpaInfo dialog

class CMunpaDoc;
class CItemDlg;

class CMunpaInfo : public CLDPropPage
{
	DECLARE_DYNCREATE(CMunpaInfo)

// Construction
public:
	CMunpaInfo();
	~CMunpaInfo();

public:
	WORD		m_wPower;
	CMunpaDoc* m_pDocument;
	CItemDlg*	m_pItemDlg;

	CToolTipCtrl	m_TTctrl;

	CMREdit			m_StatusEdit[eMunpaTemp_MAX];						// ¹®ÆÄ Á¤º¸
	CMRButton		m_MunpaItem[TABCELL_MUNPAWAREHOUSE_NUM];			// ¹®ÆÄ Ã¢°í

	CFont			m_Font;
public:
	virtual void InitPage(void* pData) { m_pDocument = (CMunpaDoc*)pData; }

	void	InitMunpaInfo( BOOL bEnable = FALSE );
	void	InsertMemberList(char* strName, LEVELTYPE level, BYTE position, char* strRegDate);

	void	SetMunpaBaseInfo();
	void	SetMunpaItemInfo();

	void	InitFont( CFont* pFont )	{ SetFont( pFont ); }

protected:
	void			InitMemberListControl();

	void			EnableWareHouse(BOOL bEnable = TRUE);

	CMRButton* CheckButtonEvent(HWND hWnd, POSTYPE& pos);
	void		MakeToolTip(POSTYPE pos, WORD idx, char* str);

	void ButtonControlCommand(UINT nId);
	void EditControlCommand(UINT nId);

	TABLE_KIND	GetTableKindByControlID(UINT nId);


////////////////////////////////////////////////
//// ¼öÁ¤ ÇÔ¼ö
	int ItemEdit( INFOFIELD field, WORD wControlId, const ITEMBASE* base_before, ITEMBASE& base_after, 
					const ITEM_OPTION_INFO* before, ITEM_OPTION_INFO& after );
	BOOL ResultItemEdit( INFOFIELD field, int result, WORD wPos, int nControlIdx, ITEMBASE* Origin, ITEM_OPTION_INFO* OriginOption,
							ITEMBASE* pBItem, ITEMBASE* pAItem, ITEM_OPTION_INFO* pBOption, ITEM_OPTION_INFO* pAOption );
////////////////////////////////////////////////

// Dialog Data
	//{{AFX_DATA(CMunpaInfo)
	enum { IDD = IDD_GUILD_PAGE_MUNPA };
	CButton	m_btnSave;
	CEdit	m_etMunpaSearch;
	CComboBox	m_cbTab;
	CListCtrl	m_MunpaWonList;
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMunpaInfo)
	public:
//	virtual void OnSetFont(CFont* pFont);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL


// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMunpaInfo)
	afx_msg void OnButtonSearch();
	afx_msg void OnSelchangeMunpaCbtab();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnMunpaSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MUNPAINFO_H__F78D1D8A_942E_4D24_A57D_0B47A6F7573F__INCLUDED_)
