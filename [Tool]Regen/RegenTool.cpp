// RegenTool.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "RegenTool.h"

#include "MainFrm.h"
#include "RegenToolDoc.h"
#include "RegenToolView.h"

#include "MonsterListDlg.h"
#include "InMapMonsterDlg.h"

#include "./Engine/GraphicEngine.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegenToolApp

DWORD gCurTime;

BEGIN_MESSAGE_MAP(CRegenToolApp, CWinApp)
	//{{AFX_MSG_MAP(CRegenToolApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_WINDOW_MONSTER_LIST, OnWindowMonsterList)
	ON_COMMAND(ID_IN_MAP_MONSTER, OnInMapMonster)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegenToolApp construction

CRegenToolApp::CRegenToolApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CRegenToolApp object

CRegenToolApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CRegenToolApp initialization

BOOL CRegenToolApp::InitInstance()
{
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
		RUNTIME_CLASS(CRegenToolDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CRegenToolView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	
	
	FILE* fp = fopen("../DeveloperWorking/_DONOTUSEENGINEPACK","r");
	if(fp)
	{
		g_bUsingEnginePack = FALSE;
		fclose(fp);
	}

	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CRegenToolView* pView = (CRegenToolView*)pMain->GetActiveView();
	char temp[256];
	GetCurrentDirectory(256,temp);
	pView->SetWorkPath(temp);
	pView->InitEngine();
	DIRECTORYMGR->Init();
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
void CRegenToolApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CRegenToolApp message handlers


void CRegenToolApp::OnWindowMonsterList() 
{
	// TODO: Add your command handler code here
	CMonsterListDlg MonsterListDlg;
	MonsterListDlg.DoModal();
}

void CRegenToolApp::OnInMapMonster() 
{
	// TODO: Add your command handler code here
	CInMapMonsterDlg InMapMonsterDlg;
	InMapMonsterDlg.DoModal();
}

BOOL CRegenToolApp::OnIdle(LONG lCount) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	g_pExecutive->GetRenderer()->BeginPerformanceAnalyze();
	
	g_pExecutive->Run(0xff0000ff,NULL,NULL,NULL);
	
	g_pExecutive->GetRenderer()->EndPerformanceAnalyze();
	
	g_pExecutive->GetGeometry()->Present(NULL);
	
	return CWinApp::OnIdle(lCount);
}
