#if !defined(AFX_USERCOUNT_H__284CE690_CB49_4251_8C84_BF72BC41579A__INCLUDED_)
#define AFX_USERCOUNT_H__284CE690_CB49_4251_8C84_BF72BC41579A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserCount.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserCount dialog

class CUserCount : public CDialog
{
// Construction
public:
	CUserCount(CWnd* pParent = NULL);   // standard constructor
	
	CFont		m_Font;
	BOOL		m_bInit;

	void		InitData();
	HTREEITEM	InsertItem( HTREEITEM hRoot, char* name, BOOL bChild );
	void		SetChannelData( int nServerSet, MSG_CHANNEL_INFO_MORNITOR* pChannelInfo );
	void		UpdateTreeCtrl();

// Dialog Data
	//{{AFX_DATA(CUserCount)
	enum { IDD = IDD_DIALOG_USERCOUNT };
	CComboBox	m_cbServerSet;
	CTreeCtrl	m_tcUserCount;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserCount)
	public:
	virtual BOOL DestroyWindow();
	virtual BOOL Create(CWnd* pParentWnd);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUserCount)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboServerset();
	afx_msg void OnButtonRefresh();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERCOUNT_H__284CE690_CB49_4251_8C84_BF72BC41579A__INCLUDED_)
