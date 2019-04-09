#if !defined(AFX_ADMINVIEW_H__6057D144_227E_493F_AC09_C7E9B8A765B4__INCLUDED_)
#define AFX_ADMINVIEW_H__6057D144_227E_493F_AC09_C7E9B8A765B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdminView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAdminView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CLDPropPage;
class CLDPropSheet;
class CAdminInfo;
class CIpInfo;


enum eAdminPage
{
	eAdminPage_OperatorInfo = 0, eAdminPage_IpInfo, eAdminPage_Max, 
};

class CAdminView : public CFormView
{
protected:
	CAdminView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAdminView)

// Form Data
public:
	//{{AFX_DATA(CAdminView)
	enum { IDD = IDD_ADMIN_SHEET };
	//}}AFX_DATA

// Attributes
public:
	// Sheet and Page
	CLDPropSheet*		m_pSheet;
	CLDPropPage*		m_pPageList[eAdminPage_Max];
	//

	CAdminInfo*		GetAdminInfoPage() { return (CAdminInfo*)m_pPageList[eAdminPage_OperatorInfo]; }
	CIpInfo*		GetIpInfoPage() { return (CIpInfo*)m_pPageList[eAdminPage_IpInfo]; }

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdminView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAdminView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CAdminView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADMINVIEW_H__6057D144_227E_493F_AC09_C7E9B8A765B4__INCLUDED_)
