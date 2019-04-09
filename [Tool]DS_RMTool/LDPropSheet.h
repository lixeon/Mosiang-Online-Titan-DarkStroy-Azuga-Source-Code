#if !defined(AFX_PROPSHEET_H__0EFAC688_9643_42D1_816D_A030C2976A39__INCLUDED_)
#define AFX_PROPSHEET_H__0EFAC688_9643_42D1_816D_A030C2976A39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropSheet.h : header file
//
#define WM_ADJUST WM_USER + 111

/////////////////////////////////////////////////////////////////////////////
// CLDPropSheet

class CLDPropPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CLDPropPage)
public:
	CLDPropPage() : CPropertyPage(-1) {}
	CLDPropPage(int nID) : CPropertyPage(nID), m_bEnabled(TRUE) {}
	CLDPropPage(CFont* pFont)	{}
	~CLDPropPage() {}

	virtual void InitPage(void* pData) {}
	virtual void CommandMessage(UINT nID, int nCode) {}

protected:
	BOOL m_bEnabled;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLDPropPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
protected:
	// Generated message map functions
	//{{AFX_MSG(CLDPropPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual void GetOriginalRect(CRect *pRect);
};

class CLDPropSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CLDPropSheet)
public:
//	CLDPropSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
//	CLDPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
//	CLDPropSheet() {};
	CLDPropSheet(CWnd* pParentWnd = NULL);
	virtual void AdjustPages();
	CRect	m_rectOriginal;
	BOOL	m_bOKToLeaveTab;
	int		m_nLastActive;
//	CImageList m_imageTab;
public:

	CFont m_Font;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLDPropSheet)
	public:
	virtual BOOL	Create(CWnd* pParentWnd, DWORD dwStyle = (DWORD)-1, DWORD dwExStyle = 0);
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLDPropSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLDPropSheet)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnAdjust(WPARAM wParam, LPARAM lParam);

	virtual void BuildPropPageArray ();

	RECT m_rctPage;
	CFont m_fntPage;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPSHEET_H__0EFAC688_9643_42D1_816D_A030C2976A39__INCLUDED_)
