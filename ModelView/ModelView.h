// ModelView.h : main header file for the MODELVIEW application
//

#if !defined(AFX_MODELVIEW_H__517E04AB_458F_44A1_ADE4_1002482372CA__INCLUDED_)
#define AFX_MODELVIEW_H__517E04AB_458F_44A1_ADE4_1002482372CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CModelViewApp:
// See ModelView.cpp for the implementation of this class
//

class CModelViewApp : public CWinApp
{
public:
	CModelViewApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModelViewApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CModelViewApp)
	afx_msg void OnAppAbout();
	afx_msg void OnWavListClear();
	afx_msg void OnSpecularSetting();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELVIEW_H__517E04AB_458F_44A1_ADE4_1002482372CA__INCLUDED_)
