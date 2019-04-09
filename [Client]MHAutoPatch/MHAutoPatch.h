// MHAutoPatch.h : main header file for the MHAUTOPATCH application
//

#if !defined(AFX_MHAUTOPATCH_H__3FF53E45_BF6A_4397_9854_E3217D68EA2B__INCLUDED_)
#define AFX_MHAUTOPATCH_H__3FF53E45_BF6A_4397_9854_E3217D68EA2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


/////////////////////////////////////////////////////////////////////////////
// CMHAutoPatchApp:
// See MHAutoPatch.cpp for the implementation of this class
//

#define URL_LENGTH	256

class CMHAutoPatchApp : public CWinApp
{
protected:
	HANDLE	m_hMap;

public:
	CMHAutoPatchApp();

	void ReleaseHandle() { if( m_hMap ) CloseHandle(m_hMap); }


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMHAutoPatchApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	BOOL SaveFileList();
	BOOL SelfUpdate();
	BOOL GetVersionInfoFile( char* lpszVerInfoFile, char* lpszVerInfo );

	BOOL Initialize_Ftp();
	void UnInitialize_Ftp();
	BOOL GetFtpInfoFile( char* filename );
	void LoadFtpInfo( int nFtpNum );

	//{{AFX_MSG(CMHAutoPatchApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MHAUTOPATCH_H__3FF53E45_BF6A_4397_9854_E3217D68EA2B__INCLUDED_)
