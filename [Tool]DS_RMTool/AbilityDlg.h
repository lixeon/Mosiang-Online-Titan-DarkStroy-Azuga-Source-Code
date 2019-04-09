#if !defined(AFX_ABILITYDLG_H__5E49A1C1_A88F_4829_94DC_00BBE913D130__INCLUDED_)
#define AFX_ABILITYDLG_H__5E49A1C1_A88F_4829_94DC_00BBE913D130__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AbilityDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAbilityDlg dialog

#include "PropertyList.h"

enum emAbility_Basic
{
	emAbility_Idx, emAbility_Name, emAbility_Level, emAbility_Max,
};

class CAbilityInfo;

class CAbilityDlg : public CDialog
{
protected:
	CAbilityInfo*		m_pAbility;
	BYTE					m_bAbilityLevel;
	WORD					m_wPos;

	CPropertyItem* m_BasicList[emAbility_Max];
	
// Construction
public:
	CAbilityDlg(CWnd* pParent = NULL);   // standard constructor

	void Init( WORD wPos, CAbilityInfo* pAbility, BYTE bAbilityLevel );

	BYTE GetAbilityLevel() { return m_bAbilityLevel; }

// Dialog Data
	//{{AFX_DATA(CAbilityDlg)
	enum { IDD = IDD_DLG_ABILITY };
	CButton	m_btnEdit;
	CPropertyList	m_listAbility;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAbilityDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAbilityDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAbEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABILITYDLG_H__5E49A1C1_A88F_4829_94DC_00BBE913D130__INCLUDED_)
