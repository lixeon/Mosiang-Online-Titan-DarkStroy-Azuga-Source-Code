// 4DyuchiFilePack.h : main header file for the 4DYUCHIFILEPACK application
//

#if !defined(AFX_4DYUCHIFILEPACK_H__F7501328_35C5_4CD1_AAD8_4D8D2DA7428A__INCLUDED_)
#define AFX_4DYUCHIFILEPACK_H__F7501328_35C5_4CD1_AAD8_4D8D2DA7428A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMy4DyuchiFilePackApp:
// See 4DyuchiFilePack.cpp for the implementation of this class
//

class CMy4DyuchiFilePackApp : public CWinApp
{
public:
	CMy4DyuchiFilePackApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy4DyuchiFilePackApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMy4DyuchiFilePackApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_4DYUCHIFILEPACK_H__F7501328_35C5_4CD1_AAD8_4D8D2DA7428A__INCLUDED_)
