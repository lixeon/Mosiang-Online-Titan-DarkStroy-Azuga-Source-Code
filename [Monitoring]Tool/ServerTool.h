// ServerTool.h : main header file for the SERVERTOOL application
//

#if !defined(AFX_SERVERTOOL_H__359FD9A1_8498_43F5_BF99_10548530F7D7__INCLUDED_)
#define AFX_SERVERTOOL_H__359FD9A1_8498_43F5_BF99_10548530F7D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "LoginDlg.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CServerToolApp:
// See ServerTool.cpp for the implementation of this class
//

class CServerToolApp : public CWinApp
{
public:	
	CServerToolApp();

	BOOL	CreateFolder();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerToolApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CServerToolApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERTOOL_H__359FD9A1_8498_43F5_BF99_10548530F7D7__INCLUDED_)
