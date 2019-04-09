#if !defined(AFX_LOGTOTALPAGE_H__4922DEEB_37ED_40D3_BDBB_2EB10946D56C__INCLUDED_)
#define AFX_LOGTOTALPAGE_H__4922DEEB_37ED_40D3_BDBB_2EB10946D56C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogTotalPage.h : header file
//

#include "LDPropSheet.h"

/////////////////////////////////////////////////////////////////////////////
// CLogTotalPage dialog
class CLogDoc;

class CLogTotalPage : public CLDPropPage
{
	DECLARE_DYNCREATE(CLogTotalPage)

// Construction
public:
	CLogTotalPage();   // standard constructor
	~CLogTotalPage();

	CLogDoc*		m_pDocument;
	DWORD			m_dwCharIdx;
	BOOL			m_bStart;
	CFont			m_Font;

	void	SetDocument( CLogDoc* pDoc )	{ m_pDocument = pDoc; }
	void	SetCharacterIdx( DWORD dwCharIdx );
	void	SetCharacterLog( TMSG_CHARACTERLOG* pMsg );
	void	SetExpPointLog( TMSG_EXPPOINTLOG* pMsg );
	void	SetMugongLog( TMSG_MUGONGLOG* pMsg );
	void	SetMugongExpLog( TMSG_MUGONGEXPLOG* pMsg );
	void	SetMoneyLog( TMSG_MONEYLOG* pMsg );
	void	SetMoneyWrongLog( TMSG_MONEYWRONGLOG* pMsg );
	void	SetAbilityLog( TMSG_ABILITYLOG* pMsg );	

	CYHHashTable<CHARACTERLOG>	m_CharacterLogTable;
	CYHHashTable<EXPPOINTLOG>	m_ExpPointLogTable;
	CYHHashTable<MUGONGLOG>		m_MugongLogTable;
	CYHHashTable<MUGONGEXPLOG>	m_MugongExpLogTable;
	CYHHashTable<MONEYLOG>		m_MoneyLogTable;
	CYHHashTable<MONEYWRONGLOG>	m_MoneyWrongLogTable;
	CYHHashTable<ABILITYLOG>	m_AbilityLogTable;
	void	ReleaseCharacterLogTable();
	void	ReleaseExpPointLogTable();
	void	ReleaseMugongLogTable();
	void	ReleaseMugongExpLogTable();
	void	ReleaseMoneyLogTable();
	void	ReleaseMoneyWrongLogTable();
	void	ReleaseAbilityLogTable();

	void	InitListControl( DWORD dwKind );
	void	UpdateListControl( DWORD dwKind );
	
	char*	GetSkillLogType( DWORD dwLogType, char* temp );
	char*	GetSkillName( DWORD dwSkillIdx, char* temp );
	char*	GetAbilityName( DWORD dwAbilityIdx, char* temp );
	char*	GetMonsterName( DWORD dwMonsterIdx, char* temp );


	void	ChangeFont();

// Dialog Data
	//{{AFX_DATA(CLogTotalPage)
	enum { IDD = IDD_LOG_PAGE_TOTAL };
	CDateTimeCtrl	m_StartTime;
	CDateTimeCtrl	m_EndTime;
	CEdit	m_etCharIdx;
	CEdit	m_etCharName;
	CListCtrl	m_lcTotal;
	BOOL	m_bOneDay;
	int		m_nLogKind;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogTotalPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLogTotalPage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonLogSearch();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnRadio5();
	afx_msg void OnRadio6();
	afx_msg void OnRadio7();
	afx_msg void OnCheckLogOneday();
	afx_msg void OnBtnSavetofile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGTOTALPAGE_H__4922DEEB_37ED_40D3_BDBB_2EB10946D56C__INCLUDED_)
