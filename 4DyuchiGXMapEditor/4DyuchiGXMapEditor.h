// 4DyuchiGXMapEditor.h : main header file for the 4DYUCHIGXMAPEDITOR application
//

#if !defined(AFX_4DYUCHIGXMAPEDITOR_H__EE71D591_24EE_41E9_BECD_5CDADB8A6128__INCLUDED_)
#define AFX_4DYUCHIGXMAPEDITOR_H__EE71D591_24EE_41E9_BECD_5CDADB8A6128__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMy4DyuchiGXMapEditorApp:
// See 4DyuchiGXMapEditor.cpp for the implementation of this class
//

class CMy4DyuchiGXMapEditorApp : public CWinApp
{
public:
	CMy4DyuchiGXMapEditorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy4DyuchiGXMapEditorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMy4DyuchiGXMapEditorApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_4DYUCHIGXMAPEDITOR_H__EE71D591_24EE_41E9_BECD_5CDADB8A6128__INCLUDED_)
