// DS_RMTool.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DS_RMTool.h"

#include "MHFile.h"
#include "MainFrm.h"

//////////////////////////////////////
// temp header
#include "ChildFrm.h"
#include "DS_RMToolDoc.h"
#include "DS_RMToolView.h"

#include "MunpaDoc.h"
#include "MunpaFrm.h"
#include "MunpaView.h"

/*#include "GameLogDoc.h"
#include "GameLogFrm.h"
#include "GameLogView.h"

#include "OperLogDoc.h"
#include "OperLogFrm.h"
#include "OperLogView.h"

#include "StatisticsDoc.h"
#include "StatisticsFrm.h"
#include "StatisticsView.h"
*/
#include "AdminDoc.h"
#include "AdminFrm.h"
#include "AdminView.h"

#include "CharacterDoc.h"
#include "CharacterFrm.h"
#include "CharacterView.h"

#include "LogDoc.h"
#include "LogFrm.h"
#include "LogView.h"

#include "LogOperDoc.h"
#include "LogOperFrm.h"
#include "LogOperView.h"

//////////////////////////////////////

#include "Operater.h"
#include "RMNetwork.h"
#include "MASInfoManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDS_RMToolApp

BEGIN_MESSAGE_MAP(CDS_RMToolApp, CWinApp)
	//{{AFX_MSG_MAP(CDS_RMToolApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)

		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDS_RMToolApp construction

CDS_RMToolApp::CDS_RMToolApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

	m_wNewType = 1;
	memset( m_sMASInfo.strIP, 0, sizeof(m_sMASInfo.strIP) );
	m_sMASInfo.nPort = 0;

	m_pOperDlg = NULL;

	m_Generator.Init(3000, 1);
	m_DocumentList.Initialize(3000);

	m_bStartRMTool = FALSE;
}

CDS_RMToolApp::~CDS_RMToolApp()
{
	SAFE_DELETE(m_pOperDlg);
	m_Generator.Release();
	m_DocumentList.RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDS_RMToolApp object

CDS_RMToolApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDS_RMToolApp initialization

BOOL CDS_RMToolApp::InitInstance()
{
	if( !MASINFOMGR->ReadMasInfo( "./Data/MAS.ini" ) )
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

	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

	////////////////////////////////////////////////////////////////////////
	// Register the application's document templates.  Document templates
	// serve as the connection between documents, frame windows and views.
	// 새로운 DocTemplate 추가
	CMultiDocTemplate* pDocTemplate;

	// UserInfo temp
	pDocTemplate = new CMultiDocTemplate(
		IDR_DS_RMTTYPE,
		RUNTIME_CLASS(CDS_RMToolDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CDS_RMToolView));
	AddDocTemplate(pDocTemplate);

	// Character temp
	pDocTemplate = new CMultiDocTemplate(
		IDR_DS_CHARTYPE,
		RUNTIME_CLASS(CCharacterDoc),
		RUNTIME_CLASS(CCharacterFrm),
		RUNTIME_CLASS(CCharacterView));
	AddDocTemplate(pDocTemplate);
	
	// MunpaInfo temp
	pDocTemplate = new CMultiDocTemplate(
		IDR_DS_MUNPA,
		RUNTIME_CLASS(CMunpaDoc),
		RUNTIME_CLASS(CMunpaFrm),
		RUNTIME_CLASS(CMunpaView));
	AddDocTemplate(pDocTemplate);

	// Log temp
	pDocTemplate = new CMultiDocTemplate(
		IDR_DS_LOG,
		RUNTIME_CLASS(CLogDoc),
		RUNTIME_CLASS(CLogFrm),
		RUNTIME_CLASS(CLogView));
	AddDocTemplate(pDocTemplate);

	// Admin temp
	pDocTemplate = new CMultiDocTemplate(
		IDR_DS_ADMINTYPE,
		RUNTIME_CLASS(CAdminDoc),
		RUNTIME_CLASS(CAdminFrm),
		RUNTIME_CLASS(CAdminView));
	AddDocTemplate(pDocTemplate);

	// Admin temp
	pDocTemplate = new CMultiDocTemplate(
		IDR_DS_OPERLOG,
		RUNTIME_CLASS(CLogOperDoc),
		RUNTIME_CLASS(CLogOperFrm),
		RUNTIME_CLASS(CLogOperView));
	AddDocTemplate(pDocTemplate);	

	////////////////////////////////////////////////////////////////////////
	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if ( !pMainFrame->LoadFrame(IDR_MAINFRAME) )
		return FALSE;
	m_pMainWnd = pMainFrame;
	////////////////////////////////////
	//
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	//////////////////////////////////////////////////////
	// 현재 해상도에 맞춘다.
	HWND hWnd = GetDesktopWindow();
	HDC dc = ::GetDC(hWnd);

	int x, y;
	x = GetDeviceCaps(dc, HORZRES);
	y = GetDeviceCaps(dc, VERTRES);

	m_pMainWnd->SetWindowPos(NULL, 0,0,x,y-2,SWP_NOZORDER);
	//////////////////////////////////////////////////////

	///////////////////////////////////////////////////////
	// 1. Module Init
	// 2. Patch System
	// 3. Resource Loading
	// 4. Operator check
	
	// Module Init
	if( !InitModule() )
		return FALSE;

	// Operator check
	OnOperatorChecking();
	///////////////////////////////////////////////////////

	// The main window has been initialized, so show and update it.
//	pMainFrame->ShowWindow(m_nCmdShow);
//	pMainFrame->UpdateWindow();

	if( m_pMainWnd )
	{
		m_pMainWnd->ShowWindow(SW_MAXIMIZE);
		if( !GAMERESRCMNGR->Init() )
			return FALSE;
	}

	return TRUE;
}

void CDS_RMToolApp::OnOperatorChecking()
{
	m_pOperDlg = new COperater;
	int nResult = m_pOperDlg->DoModal();

	if( nResult != IDOK )
	{
		if( m_pMainWnd )
			m_pMainWnd->SendMessage(WM_CLOSE);
	}

	SAFE_DELETE(m_pOperDlg);
}

void CDS_RMToolApp::StartRMTool()
{
	m_pOperDlg->EndDialog(IDOK);

	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();

	m_bStartRMTool = TRUE;	
	
	MASINFO* pInfo = MASINFOMGR->GetCurMasInfo();	
	char temp[256];
	sprintf( temp, "DS_RMTool : %s - Ver : %s", pInfo->strServerName, RMTOOLVERSION );
	AfxGetMainWnd()->SetWindowText( temp );
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
void CDS_RMToolApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CDS_RMToolApp message handlers

CDocument* CDS_RMToolApp::OnFileNew()
{
	POSITION pos  = GetFirstDocTemplatePosition();
	CDocTemplate* pTemplate = NULL;
	
	for(int i=0;i<m_wNewType;++i)
		pTemplate = GetNextDocTemplate(pos);

	if(!pTemplate)
	{
		ASSERT(0);
		return NULL;
	}
	
	CBaseDoc* pDoc = (CBaseDoc*) pTemplate->OpenDocumentFile(NULL);

	if( !pDoc )
	{
		ASSERT(0);
		return NULL;
	}

	DWORD idx = m_Generator.GenerateIndex();
	pDoc->SetDocumentIdx(idx);
	m_DocumentList.Add(pDoc, idx);

	pDoc->InitData();
	return pDoc;
}

void CDS_RMToolApp::ExitFile(DWORD dwIdx)
{
	m_Generator.ReleaseIndex(dwIdx);
	m_DocumentList.Remove(dwIdx);
}

CBaseDoc* CDS_RMToolApp::OpenNewTemplate(int nType)
{
	if (nType > eTemp_Max) return NULL;

	m_wNewType = nType;
	CBaseDoc* pDoc = (CBaseDoc*)OnFileNew();
	m_wNewType = 1;

	return pDoc;
}

BOOL CDS_RMToolApp::InitModule()
{
	////////////////////////////////////////////////
	// Network Init

	// ReadIp
//	if( !ReadMASIp(".\\MAS.ini" ) )
//		return FALSE;
//	if( !MASINFOMGR->ReadMasInfo( ".\\MAS.ini" ) )
//		return FALSE;

	if( !NETWORK->Init(m_pMainWnd->GetSafeHwnd()) )
		return FALSE;

//	if( !NETWORK->ConnectToServer( m_sMASInfo.strIP, m_sMASInfo.nPort ) )
//	{
//		MessageBox( NULL, "데몬서버에 연결할 수 없습니다.", "접속오류", MB_OK );
//		return FALSE;
//	}

	return TRUE;
}

BOOL CDS_RMToolApp::ReadMASIp(char *filename)
{
	CMHFile file;
	char temp[256];

	if( !file.Init( filename, "rt" ) )
		return FALSE;

	while( !file.IsEOF() )
	{
		strcpy( temp, strupr(file.GetString()) );

		if( temp[0] == '/' ) 
		{
			file.GetLine( temp, 256 );
			continue;
		}
		
		if( strcmp( temp, "*MAS_IP" ) == 0 )
		{
			file.GetString( m_sMASInfo.strIP );
		}
		else if( strcmp( temp, "*MAS_PORT" ) == 0 )
		{
			m_sMASInfo.nPort = file.GetInt();
		}
	}

	file.Release();

	return TRUE;	
}

BOOL CDS_RMToolApp::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

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

int CDS_RMToolApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	NETWORK->Release();
	
	return CWinApp::ExitInstance();
}

void CDS_RMToolApp::NetworkMsgParse(BYTE Protocol, void* pMsg)
{
	switch(Protocol)
	{
	case MP_RMTOOL_CONNECT_ACK:
		{
			TMSG_OPERATOR_LOGIN* msg = (TMSG_OPERATOR_LOGIN*)pMsg;
			// 정보 셋팅
			memcpy( &m_sOperator, &msg->sOper, sizeof(OPERATOR) );
			memcpy( &m_sIP, &msg->sIP, sizeof(IPINFO) );

			StartRMTool();
		}
		break;
	case MP_RMTOOL_CONNECT_NACK:
		{
			TMSG_WORD* msg = (TMSG_WORD*)pMsg;
			switch(msg->wData)
			{
			case 0: // 이상 오류
				{
					m_pOperDlg->MessageBox("DB Error!!\nClose RMTool!!");
					m_pOperDlg->EndDialog(IDCANCEL);
				}
				break;
			case 1: // 등록안된 아이피
				{
					m_pOperDlg->MessageBox("Close RMTool For Not Registed IP!!");
					m_pOperDlg->EndDialog(IDCANCEL);
				}
				break;
			case 2: // 등록안된 아이디
				{
					m_pOperDlg->MessageBox("Not Registed ID!!");
					m_pOperDlg->ResetData(0);
				}
				break;
			case 3: // 비번 오류
				{
					m_pOperDlg->MessageBox("Wrong Password!!");
					m_pOperDlg->ResetData(1);
				}
				break;
			case 4:	// 버전check
				{
					m_pOperDlg->MessageBox( "Program Version Is Not Right!!\n\nProgram Will Be Closed!!" );
					m_pOperDlg->EndDialog(IDCANCEL);
				}
				break;
			}
		}
		break;
	default:
		return;
	}
}

void CDS_RMToolApp::AllotMessage(BYTE Protocol, void* pMsg)
{
	TMSGBASE* msg = (TMSGBASE*)pMsg;
	CBaseDoc* pDoc = m_DocumentList.GetData(msg->dwTemplateIdx);

	if( !pDoc ) 
		return;

	pDoc->NetworkMsgParse(Protocol, pMsg);
}
