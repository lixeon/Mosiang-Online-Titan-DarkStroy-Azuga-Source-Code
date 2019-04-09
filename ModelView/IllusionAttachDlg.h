#if !defined(AFX_ILLUSIONATTACHDLG_H__BE5F44D0_D00B_441E_818A_0F27F5FFB0C7__INCLUDED_)
#define AFX_ILLUSIONATTACHDLG_H__BE5F44D0_D00B_441E_818A_0F27F5FFB0C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToolInfo.h"

// IllusionAttachDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIllusionAttachDlg dialog

class CIllusionAttachDlg : public CDialog
{
	int					m_iSelectedModelIndex;
// Construction
public:
	CIllusionAttachDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIllusionAttachDlg)
	enum { IDD = IDD_ILLUSION_ATTACH_DIALOG };
	CListBox	m_ObjectListBox;
	CListBox	m_ModListBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIllusionAttachDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int					GetSelectedModelIndex() {return  m_iSelectedModelIndex;}
	GXOBJECT_HANDLE	m_gxhAttachTarget;
	I3DModel*		m_pModel;
	char			m_szObjectName[1024];

	// Generated message map functions
	//{{AFX_MSG(CIllusionAttachDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeIllusionModList();
	afx_msg void OnSelchangeIllusionObjectList();
	afx_msg void OnIlluminationLoadOk();
	afx_msg void OnIlluminationDlgCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ILLUSIONATTACHDLG_H__BE5F44D0_D00B_441E_818A_0F27F5FFB0C7__INCLUDED_)
