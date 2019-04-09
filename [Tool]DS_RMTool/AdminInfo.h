#if !defined(AFX_ADMININFO_H__E6233EDD_22F0_4EA0_90AC_718AE389C338__INCLUDED_)
#define AFX_ADMININFO_H__E6233EDD_22F0_4EA0_90AC_718AE389C338__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdminInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdminInfo dialog

#include "LDPropSheet.h"

class CAdminDoc;

class CAdminInfo : public CLDPropPage
{
	DECLARE_DYNCREATE(CAdminInfo)

// Construction
public:
	CAdminInfo();   // standard constructor
	~CAdminInfo();

	CAdminDoc*	m_pDocument;
	int			m_nPower;
	POSITION	m_Pos;

	void	InitControl(CAdminDoc* pDoc);
	void	InitOperList();
	void	UpdateOperList();
	char*	GetOperPowerName( WORD wPower, char* temp );
	void	EnableControl(BOOL bEnable = TRUE);

	void	OperatorErrorMsg();

// Dialog Data
	//{{AFX_DATA(CAdminInfo)
	enum { IDD = IDD_ADMIN_PAGE_ID };
	CButton	m_rdPower;
	CButton	m_btnNew;
	CButton	m_btnDel;
	CEdit	m_etRepass;
	CEdit	m_etPass;
	CEdit	m_etName;
	CEdit	m_etID;
	CListCtrl	m_listOper;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdminInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAdminInfo)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnOperPowerRefer();
	afx_msg void OnOperNew();
	afx_msg void OnOperPowerMaster();
	afx_msg void OnOperPowerMonitor();
	afx_msg void OnOperPowerPatroller();
	afx_msg void OnClickOperList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnOperDel();
	afx_msg void OnOperPowerAuditor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADMININFO_H__E6233EDD_22F0_4EA0_90AC_718AE389C338__INCLUDED_)
