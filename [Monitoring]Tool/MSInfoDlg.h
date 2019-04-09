#if !defined(AFX_MSINFODLG_H__12E918F6_5F84_4D02_A885_C0CE7F177F58__INCLUDED_)
#define AFX_MSINFODLG_H__12E918F6_5F84_4D02_A885_C0CE7F177F58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MSInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMSInfoDlg dialog

class CMSInfoDlg : public CDialog
{
// Construction
public:
	CMSInfoDlg(CWnd* pParent = NULL);   // standard constructor

	CFont		m_Font;
	CImageList	m_StateImageList;
	CBitmap		m_bmpGreen;
	CBitmap		m_bmpGray;
	BOOL		m_bInit;
	
	void	InitData();
	void	UpdateListCtrl();
	void	SetMSStateAll( int nServerSet, MSGMSSTATEALL* pState );
	void	SetAutoPatchState( int nServerSet, MSSTATE* pState );
	void	SetAutoPatchVersion( int nServerSet, MSSTATE* pState );

// Dialog Data
	//{{AFX_DATA(CMSInfoDlg)
	enum { IDD = IDD_DIALOG_MSINFO };
	CButton	m_btnAutoPatch;
	CButton	m_btnRefresh;
	CComboBox	m_cbServerSet;
	CListCtrl	m_lcMSInfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMSInfoDlg)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL Create(CWnd* pParentWnd);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMSInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboServerset();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonRefresh();
	afx_msg void OnButtonAutopatch();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSINFODLG_H__12E918F6_5F84_4D02_A885_C0CE7F177F58__INCLUDED_)
