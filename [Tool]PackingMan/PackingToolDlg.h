// PackingToolDlg.h : header file
//

#if !defined(AFX_PACKINGTOOLDLG_H__4E83F7E1_F408_4BC9_BE2E_17A1718D5143__INCLUDED_)
#define AFX_PACKINGTOOLDLG_H__4E83F7E1_F408_4BC9_BE2E_17A1718D5143__INCLUDED_

#include "NewDlg.h"	// Added by ClassView
#include "MHFileMng.h"	// Added by ClassView
#include "MHFileEx.h"	// Added by ClassView
#include "ExtDlg.h"	// Added by ClassView
#include "MyEdit.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPackingToolDlg dialog

class CPackingToolDlg : public CDialog
{
	void OpenFile(char* pFileName);
	void UpdateList();
// Construction
public:
	RECT m_Gap;
	WINDOWPLACEMENT m_EditPl;
	int m_nOldIndex;
	void SaveData( int index );
	CNewDlg m_NewDlg;
	CExtDlg m_ExtDlg;
	CMHFileMng	m_FileMng;
	int			m_nFileNum;
	CPackingToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPackingToolDlg)
	enum { IDD = IDD_PACKINGTOOL_DIALOG };
	CButton	m_BtnSave;
	CButton	m_BtnSaveAsBin;
	CMyEdit	m_Edit;
	CButton	m_BtnSaveAllToTxt;
	CButton	m_BtnSaveAllToBin;
	CButton	m_BtnOpenBin;
	CButton	m_BtnOpen;
	CButton	m_BtnNew;
	CButton	m_BtnDelAll;
	CListBox	m_FileList;
	CString	m_sFileNum;
	CString	m_sCurFile;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPackingToolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPackingToolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonOpen();
	afx_msg void OnButtonSaveToBin();
	afx_msg void OnButtonOpenBin();
	afx_msg void OnButtonSaveToTxt();
	afx_msg void OnButtonDelall();
	afx_msg void OnSelchangeListFile();
	afx_msg void OnButtonNew();
	afx_msg void OnButtonSaveAsBin();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnButtonSaveBin();
	afx_msg void OnFileSearch();
	afx_msg void OnSearch();
	afx_msg void OnButtonSavedsof();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKINGTOOLDLG_H__4E83F7E1_F408_4BC9_BE2E_17A1718D5143__INCLUDED_)
