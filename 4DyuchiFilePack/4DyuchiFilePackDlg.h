// 4DyuchiFilePackDlg.h : header file
//

#if !defined(AFX_4DYUCHIFILEPACKDLG_H__EC0CDDCF_3906_484F_B5E2_01B481701000__INCLUDED_)
#define AFX_4DYUCHIFILEPACKDLG_H__EC0CDDCF_3906_484F_B5E2_01B481701000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "../4DyuchiGRX_Common/IFileStorage.h"
#include "ListBoxContainer.h"
/////////////////////////////////////////////////////////////////////////////
// CMy4DyuchiFilePackDlg dialog

class CMy4DyuchiFilePackDlg : public CDialog
{
	CListBoxContainer		m_lbcFileView;
	CListBoxContainer		m_lbcPackFileView;
	FSFILE_ATOM_INFO*		m_pFileInfoList;
	void*					m_pPackFileHandle;
	DWORD					m_dwFileInfoNum;

	
	void					UpdateFileViewDisplay();
	void					ClearFileViewDisplay();
// Construction
public:	
	DWORD					UnlockProc(DWORD dwCurCount,DWORD dwTotalCount,void* pArg);
	CMy4DyuchiFilePackDlg(CWnd* pParent = NULL);	// standard constructor
	I4DyuchiFileStorage*	m_pStorage;
	
// Dialog Data
	//{{AFX_DATA(CMy4DyuchiFilePackDlg)
	enum { IDD = IDD_MY4DYUCHIFILEPACK_DIALOG };
	CListBox	m_lbPackFileView;
	CListBox	m_lbFileView;
	CString	m_strFullPathFileName;
	BOOL	m_bWarningExistingFile;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy4DyuchiFilePackDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMy4DyuchiFilePackDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnDestroy();
	afx_msg void OnButtonBrowseFile();
	afx_msg void OnButtonMapToStorage();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonExtractAll();
	afx_msg void OnButtonExtractAllInStorage();
	afx_msg void OnButtonSelectPackfile();
	afx_msg void OnButtonDeletePackfile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_4DYUCHIFILEPACKDLG_H__EC0CDDCF_3906_484F_B5E2_01B481701000__INCLUDED_)
