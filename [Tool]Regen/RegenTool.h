// RegenTool.h : main header file for the REGENTOOL application
//

#if !defined(AFX_REGENTOOL_H__EFECCDE9_714A_4363_B9B5_29828EB41425__INCLUDED_)
#define AFX_REGENTOOL_H__EFECCDE9_714A_4363_B9B5_29828EB41425__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CRegenToolApp:
// See RegenTool.cpp for the implementation of this class
//

class CRegenToolApp : public CWinApp
{
public:
	CRegenToolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegenToolApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CRegenToolApp)
	afx_msg void OnAppAbout();
	afx_msg void OnWindowMonsterList();
	afx_msg void OnInMapMonster();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGENTOOL_H__EFECCDE9_714A_4363_B9B5_29828EB41425__INCLUDED_)
