// PackingTool.h : main header file for the PACKINGTOOL application
//

#if !defined(AFX_PACKINGTOOL_H__5781EF99_391C_4B82_8E58_CF6C67DB53C2__INCLUDED_)
#define AFX_PACKINGTOOL_H__5781EF99_391C_4B82_8E58_CF6C67DB53C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPackingToolApp:
// See PackingTool.cpp for the implementation of this class
//

class CPackingToolApp : public CWinApp
{
public:
	CPackingToolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPackingToolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPackingToolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKINGTOOL_H__5781EF99_391C_4B82_8E58_CF6C67DB53C2__INCLUDED_)
