#if !defined(AFX_IPINFO_H__411F039B_6D7C_4ECA_A895_5E5A877B310B__INCLUDED_)
#define AFX_IPINFO_H__411F039B_6D7C_4ECA_A895_5E5A877B310B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IpInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIpInfo dialog

#include "LDPropSheet.h"

class CAdminDoc;

class CIpInfo : public CLDPropPage
{
	DECLARE_DYNCREATE(CIpInfo)

// Construction
public:
	CIpInfo();   // standard constructor
	~CIpInfo();

	CAdminDoc*	m_pDocument;
	POSITION	m_Pos;

	void	InitControl(CAdminDoc* pDoc);
	void	InitIpList();
	void	UpdateIpList();
	void	EnableControl(BOOL bEnable = TRUE);
	void	IPErrorMsg();

// Dialog Data
	//{{AFX_DATA(CIpInfo)
	enum { IDD = IDD_DS_RMTOOL_IP };
	CIPAddressCtrl	m_ipNew;
	CButton	m_btnDel;
	CButton	m_btnNew;
	CListCtrl	m_listIp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIpInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIpInfo)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBtnNewip();
	afx_msg void OnBtnIpdel();
	afx_msg void OnClickIpinfoList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPINFO_H__411F039B_6D7C_4ECA_A895_5E5A877B310B__INCLUDED_)
