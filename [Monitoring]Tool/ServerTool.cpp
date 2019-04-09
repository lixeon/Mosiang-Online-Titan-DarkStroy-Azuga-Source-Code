// ServerTool.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ServerTool.h"

#include "MainFrm.h"
#include "ServerToolDoc.h"
#include "ServerToolView.h"
#include "MHNetwork.h"
#include "ServerSetMng.h"
#include "MapNameManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServerToolApp

BEGIN_MESSAGE_MAP(CServerToolApp, CWinApp)
	//{{AFX_MSG_MAP(CServerToolApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerToolApp construction

CServerToolApp::CServerToolApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CServerToolApp object

CServerToolApp theApp;
sMSGOPTION	gMsgOption;

/////////////////////////////////////////////////////////////////////////////
// CServerToolApp initialization

BOOL CServerToolApp::InitInstance()
{
	MAPNAME->ReadMapName();
	
	if( !CreateFolder() )
		return FALSE;

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CServerToolDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CServerToolView));
	AddDocTemplate(pDocTemplate);
	
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

#ifdef _MASTER
	if( strcmp( m_lpCmdLine, "anrgideoqkr!!" ) != 0 )
		return FALSE;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNew;
#endif
	
	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_MAXIMIZE);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CServerToolApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CServerToolApp message handlers

BOOL CServerToolApp::PreTranslateMessage(MSG* pMsg) 
{
	switch( pMsg->message )
	{
	case WM_SOCKEVENTMSG:
		{
			ISC_BaseNetwork* lpNet = NETWORK->GetBaseNetwork();
			if( lpNet )
				lpNet->SocketEventProc();
			return TRUE;
		}
		break;
	}

	return CWinApp::PreTranslateMessage(pMsg);
}

int CServerToolApp::ExitInstance() 
{
	NETWORK->Release();

	LOG( "!!!!! ServerTool End !!!!!" );

	return CWinApp::ExitInstance();
}

BOOL CServerToolApp::CreateFolder()
{
	char temp[256];
	strcpy( temp, GetUserCountDirectory() );

	CreateDirectory( temp, NULL );

	return TRUE;
}
