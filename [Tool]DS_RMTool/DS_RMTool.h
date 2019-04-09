// DS_RMTool.h : main header file for the DS_RMTOOL application
//

#if !defined(AFX_DS_RMTOOL_H__E87F0A28_D2D7_4A39_9EAC_DE00379CB03E__INCLUDED_)
#define AFX_DS_RMTOOL_H__E87F0A28_D2D7_4A39_9EAC_DE00379CB03E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "IndexGenerator.h"

/////////////////////////////////////////////////////////////////////////////
// CDS_RMToolApp:
// See DS_RMTool.cpp for the implementation of this class
//

class COperater;
class CBaseDoc;

class CDS_RMToolApp : public CWinApp
{
protected:
	CIndexGenerator			m_Generator;
	CYHHashTable<CBaseDoc>	m_DocumentList;
	OPERATOR				m_sOperator;
	IPINFO					m_sIP;
	BOOL					m_bStartRMTool;

public:
	CDS_RMToolApp();
	virtual ~CDS_RMToolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDS_RMToolApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CDS_RMToolApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	afx_msg CDocument* OnFileNew();

	DECLARE_MESSAGE_MAP()

public:
	////////////////////////////////////////////
	// temp Open Func
	CBaseDoc* OpenNewTemplate(int nType);
	////////////////////////////////////////////
	void	ExitFile(DWORD dwIdx);
	DWORD	GetCurOperatorIdx()		{ return m_sOperator.dwOperIdx; }
	char*	GetCurOperatorName()	{ return m_sOperator.strOperName; }
	WORD	GetCurOperatorPower()	{ return m_sOperator.wOperPower; }
	BOOL	IsStartRMTool()			{ return m_bStartRMTool; }

protected:
	WORD		m_wNewType;
	MASINFO		m_sMASInfo;
	COperater*	m_pOperDlg;

protected:
	BOOL	InitModule();						// module 초기화
	BOOL	ReadMASIp(char *filename);			// 데몬 아이피 설정
	void	OnOperatorChecking();				// 운영자 로그인 체크
	void	StartRMTool();

public:	
	// connect msg parsing
	void	NetworkMsgParse(BYTE Protocol, void* pMsg);
	// msg allot
	void	AllotMessage(BYTE Protocol, void* pMsg);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DS_RMTOOL_H__E87F0A28_D2D7_4A39_9EAC_DE00379CB03E__INCLUDED_)
