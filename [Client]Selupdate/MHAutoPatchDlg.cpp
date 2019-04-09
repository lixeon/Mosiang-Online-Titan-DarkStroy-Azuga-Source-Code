// MHAutoPatchDlg.cpp : implementation file
//

#include "stdafx.h"
#include <process.h>
#include "../4DyuchiGXGFunc/global.h"
#include "../4dyuchiGRX_common/IExecutive.h"

#include "MHAutoPatch.h"
#include "MHAutoPatchDlg.h"
#include "RainFTP_TypeDefine.h"
#include "MHAutoPatchDefines.h"
#include "StrClass.h"
#include "DirectoryName.h"
#include ".\mhautopatchdlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BOOL	g_bUsingEnginePack = TRUE;
BOOL	bProcessing;
DWORD	PackingProcess = 0;
BOOL	bConnecting = FALSE;
DWORD	dwFileSizeMax = 0;

char szOldestFile[256];
Connect_data		cd;
Directory_data		dd;
List_data			ld_newversion;
List_data			ld_zip;
File_data			fd_newversion;
File_data			fd_zip;

char szPatchServerFileVer[256];
char szLocalFileVer[256];

//进度条指示
DWORD g_dwProLv = 0;
HANDLE g_ftpHSession = NULL;



/////////////////////////////////////////////////////////////////////////////
// CMHAutoPatchDlg dialog

HWND CMHAutoPatchDlg::m_sHWnd = NULL;

CMHAutoPatchDlg::CMHAutoPatchDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMHAutoPatchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMHAutoPatchDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bCanStart = FALSE;

	m_hBmpMain			= NULL;
	m_hBmpTitle			= NULL;
	m_hBmpNBtn			= NULL;
	m_hBmpNBtnPress		= NULL;
	m_hBmpEBtn			= NULL;
	m_hBmpEBtnPress		= NULL;
	m_hBmpCB			= NULL;
	m_hBmpCBCheck		= NULL;	

	m_hBmpPlayBtn = NULL;//开始游戏按钮图片
	m_hBmpPlayPress = NULL;

	m_hBmpRegBtn = NULL;//注册按钮图片
	m_hBmpRegPress = NULL;
	m_hBmpHomeBtn = NULL;//主页按钮图片
	m_hBmpHomeBtnPress = NULL;
	m_hBmpBaoBtn = NULL;//百宝按钮图片
	m_hBmpBaoBtnPress = NULL;

	m_bWindowModeChecked = FALSE;
}

void CMHAutoPatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMHAutoPatchDlg)
#ifdef _FOR_CLIENT_	
	DDX_Control(pDX, IDC_STC_MAINTITLE, m_picMain);
	DDX_Control(pDX, IDC_TXT_HOMEPAGE, m_txtHomepage);
	DDX_Control(pDX, IDC_TXT_REGIST, m_txtJoin);
	DDX_Control(pDX, IDC_TXT_START, m_txtStart);
	DDX_Control(pDX, IDC_TXT_BBS, m_txtBBS);
	DDX_Control(pDX, IDC_PIC_TITLE, m_picTitle);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_REGIST, m_btnRegist);
	DDX_Control(pDX, IDC_WINDOWMODE, m_btnWindowMode);
	DDX_Control(pDX, IDC_HOMEPAGE, m_btnHomepage);
	DDX_Control(pDX, IDC_BBS, m_btnBBS);
	DDX_Control(pDX, IDC_STC_WINDOWMODE, m_stcWindowMode);
#endif
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	//}}AFX_DATA_MAP	
}

BEGIN_MESSAGE_MAP(CMHAutoPatchDlg, CDialog)
	//{{AFX_MSG_MAP(CMHAutoPatchDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
#ifdef _FOR_CLIENT_
	ON_WM_DRAWITEM()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_HOMEPAGE, OnHomepage)
	ON_BN_CLICKED(IDC_REGIST, OnRegist)
	ON_BN_CLICKED(IDC_BBS, OnBBS)
#endif
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	//	ON_BN_CLICKED(IDC_WINDOWMODE, OnBnClickedWindowmode)
	//ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMHAutoPatchDlg message handlers

BOOL CMHAutoPatchDlg::OnInitDialog()
{
	//	DeleteFile(g_szMHpatchFile);
#ifndef _MANUAL_PATCH_
	DeleteFile("MHManualPatch.exe");
#endif

#ifdef _FOR_CLIENT_
	DeleteFile("distributeserver.exe");
	DeleteFile("agentserver.exe");
	DeleteFile("mapserver.exe");
	DeleteFile(".\\Sound\\BGM\\bg_nanju.MP3");
	DeleteFile(".\\Sound\\BGM\\bg_gaebong.MP3");
	DeleteFile(".\\Sound\\BGM\\bg_redwall.MP3");
#endif

	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	m_nRetryCount = 0;

#ifdef _FOR_CLIENT_
	SetWindowText( "SkyOL update Launcher..." );

	int iX = GetSystemMetrics( SM_CXSCREEN );
	int iY = GetSystemMetrics( SM_CYSCREEN );
	int cx = 669;
	int cy = 485;

	SetWindowPos( NULL, ( iX - cx ) / 2, ( iY - cy ) / 2, cx, cy, SWP_NOZORDER );

	m_BGMManager.CreateSound( m_hWnd );
	//===============================
	//controls' Setting & Positioning 

	//皋牢促捞倔肺弊 农扁 & 困摹
	//橇肺弊饭胶官 农扁 & 困摹
	//
	SelectTitlePicture();

	DWORD dwStyle	= 0;
	dwStyle = GetClassLong( m_btnOK.m_hWnd, GCL_STYLE );
	dwStyle &= ~CS_DBLCLKS;
	SetClassLong( m_btnOK.m_hWnd, GCL_STYLE, dwStyle );

	dwStyle = GetClassLong( m_btnCancel.m_hWnd, GCL_STYLE );
	dwStyle &= ~CS_DBLCLKS;
	SetClassLong( m_btnCancel.m_hWnd, GCL_STYLE, dwStyle );

	dwStyle = GetClassLong( m_btnRegist.m_hWnd, GCL_STYLE );
	dwStyle &= ~CS_DBLCLKS;
	SetClassLong( m_btnRegist.m_hWnd, GCL_STYLE, dwStyle );

	dwStyle = GetClassLong( m_btnHomepage.m_hWnd, GCL_STYLE );
	dwStyle &= ~CS_DBLCLKS;
	SetClassLong( m_btnHomepage.m_hWnd, GCL_STYLE, dwStyle );


	DWORD dwBStyle = m_btnWindowMode.GetButtonStyle();
	dwBStyle |= BS_OWNERDRAW;
	dwBStyle |= BS_CHECKBOX;
	m_btnWindowMode.SetButtonStyle(dwBStyle);

	int bottom		= 485;
	int right		= 669;

	bottom -= 20;
	right -= 20;

	m_btnCancel.SetWindowPos( NULL, 644, 4, 17, 17, SWP_NOZORDER );

	right -= 95;
	m_btnOK.SetWindowPos( NULL, 540, 429, 112, 46, SWP_NOZORDER );
#ifndef _FOR_CHINA_
	m_txtStart.SetWindowText( "霸烙立加" );	//捞巴档 唱吝俊 颇老肺 哗绰霸 炒摆促..
#else
	m_txtStart.SetLogFont( "宋体", 12, GB2312_CHARSET );
	m_txtStart.SetWindowText( "" );//进入游戏
#endif

	m_txtStart.SetWindowPos( NULL, right + 11, bottom + 4, 85 - 20, 17 - 2, SWP_NOZORDER );
	m_txtStart.SetFontColor( RGB( 100, 100, 100 ) );

	right -= 90;
	m_btnHomepage.SetWindowPos( NULL, 18, 439, 90, 24, SWP_NOZORDER );
#ifndef _FOR_CHINA_
	m_txtHomepage.SetWindowText( "权其捞瘤" );
#else
	m_txtHomepage.SetLogFont( "宋体", 12, GB2312_CHARSET );
	m_txtHomepage.SetWindowText( "" );//官方主页
#endif

	m_txtHomepage.SetWindowPos( NULL, right + 11, bottom + 4, 85 - 20, 17 - 2, SWP_NOZORDER );

#ifdef _FOR_CHINA_
	right -= 90;
	m_btnBBS.SetWindowPos( NULL, 218, 439, 90, 24, SWP_NOZORDER );
	m_txtBBS.SetLogFont( "宋体", 12, GB2312_CHARSET );
	m_txtBBS.SetWindowText( "" );//百宝箱
	m_txtBBS.SetWindowPos( NULL, right + 11, bottom + 4, 85 - 20, 17 - 2, SWP_NOZORDER );
	m_btnBBS.ShowWindow( SW_SHOW );
	m_txtBBS.ShowWindow( SW_SHOW );
#endif



	right -= 90;
	m_btnRegist.SetWindowPos( NULL, 118, 439, 90, 24, SWP_NOZORDER );
#ifndef _FOR_CHINA_
	m_txtJoin.SetWindowText( "雀盔啊涝" );
#else
	m_txtJoin.SetLogFont( "宋体", 12, GB2312_CHARSET );
	m_txtJoin.SetWindowText( "" );//申请会员

#endif
	m_txtJoin.SetWindowPos( NULL, right + 11, bottom + 4, 85 - 20, 17 - 5, SWP_NOZORDER );

#ifndef _FOR_CHINA_
	right -= 90;
	m_btnWindowMode.SetWindowPos( NULL, right, bottom, 17, 17, SWP_NOZORDER );

	m_stcWindowMode.SetFontColor( RGB( 220, 220, 0 ) );
	m_stcWindowMode.SetWindowText( "芒葛靛" );
	m_stcWindowMode.SetWindowPos( NULL, right+18, bottom+4, 60, 17, SWP_NOZORDER );
#else

	//检查当前版本号///////////////////////////////////////////////////////
	FILE* fpown = fopen( MH_VERSION_INFO_FILE, "r" );
	if(fpown == NULL)
	{
		MessageBox( "Failed to Check New Version", g_szMBErrCap, MB_OK ); 
		QuitPatchDialog( TRUE );	
	}

	char temp1[256];
	char temp2[256];

	fscanf( fpown, "%s", temp2 );
	fclose( fpown );

	sprintf( temp1, "%s%s","  SkyOL update Launcher...          当前版本： ", temp2 );
	//////////////////////////////////////////////////////////////////////////

	right -= 90;
	m_btnWindowMode.SetWindowPos( NULL, 423, 441, 19, 19, SWP_NOZORDER );
	m_stcWindowMode.SetFontColor( RGB( 0, 220, 220 ) );
	m_stcWindowMode.SetWindowText( temp1 );//窗口模式
	m_stcWindowMode.SetLogFont( "宋体", 12, GB2312_CHARSET  );//GB2312_CHARSET
	m_stcWindowMode.SetWindowPos( NULL, 10, 6, 60, 17, SWP_NOZORDER );
#endif

	m_pHtmlViewNotice	= new CHtmlViewNotice;
	//	m_pHtmlViewNotice->Create( NULL, NULL, WS_CHILD, CRect( 120, 187, 120+477, 189+85 ), this, 1 );
	//	m_pHtmlViewNotice->Create( NULL, NULL, WS_CHILD, CRect( 120, 196, 120+477, 196+67 ), this, 1 );
	m_pHtmlViewNotice->Create( NULL, NULL, WS_CHILD, CRect( 242, 206, 634, 364), this, 1 );

	//	char curDir[MAX_PATH];
	//	char filepath[MAX_PATH];
	//	GetCurrentDirectory( MAX_PATH, curDir );
	//	wsprintf( filepath, "file:///%s\\ini\\test.html", curDir );
	//	m_pHtmlViewNotice->Navigate2( filepath );
#ifndef _FOR_CHINA_
	m_pHtmlViewNotice->Navigate2( "http://www.darkstoryonline.com/main_pop/content.asp" );
#else
	m_pHtmlViewNotice->Navigate2( "http://www.g.cn/" );
#endif

	m_pProgressBar		= new CSkinProgressBar;
	m_pProgressBar->Create( WS_CHILD | WS_VISIBLE, CRect(121, 410, 649, 410+7), this, 2 );
	m_pProgressBar->SetRange( 0, 255 );
	m_pProgressBar->SetPos( 0 );
	m_pProgressBar->SetStep( 8 );

	m_pStatus			= new CSkinStatic;

#ifndef _FOR_CHINA_
	m_rcStatus.SetRect( 4, 167, 4+116, 167+15 );
	m_pStatus->Create( "Checking Version", WS_CHILD | WS_VISIBLE | SS_CENTER, m_rcStatus, this, 3 );
	m_pStatus->SetLogFont( "Arial", 11, DEFAULT_CHARSET );
#else
	m_rcStatus.SetRect( 4, 405, 4+116, 405+15 );
	m_pStatus->Create( "正在验证文件", WS_CHILD | WS_VISIBLE | SS_CENTER, m_rcStatus, this, 3 );
	m_pStatus->SetLogFont( "宋体", 12, GB2312_CHARSET );
#endif


#else	//_FOR_CLIENT_
	SetWindowText( "SkyOL update Launcher..." );

	int iX = GetSystemMetrics( SM_CXSCREEN );
	int iY = GetSystemMetrics( SM_CYSCREEN );
	RECT rt;
	GetClientRect( &rt );
	int cx = rt.right - rt.left;
	int cy = rt.bottom - rt.top + GetSystemMetrics( SM_CYCAPTION );

	SetWindowPos( NULL, ( iX - cx ) / 2, ( iY - cy ) / 2, cx, cy, SWP_NOZORDER );

	SetStatusText( "Checking Version" );

	CProgressCtrl* pPGCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS_PATCH);
	pPGCtrl->SetRange( 0, 255 );
	pPGCtrl->SetPos( 0 );
	pPGCtrl->SetStep( 8 );
#endif


	//==================
	// TODO: Add extra initialization here
	m_sHWnd = m_hWnd;
	//-------------
	FILE* fp = fopen("../DeveloperWorking/_DONOTUSEENGINEPACK","r");
	if(fp)
	{
		g_bUsingEnginePack = FALSE;
		fclose(fp);
	}

	if(g_bUsingEnginePack)
		Clear3DData();


	LoadOption();


	//烙矫!!!!!
#ifdef _MANUAL_PATCH_
	FILE* fporn = fopen( g_szMHpatchFile, "r" );
	if( fporn != NULL )
	{
		fclose( fporn );
		Pack_N_Extract();
	}
	else
	{
		// 	SetReadyToStart();
		//}

		//return TRUE;*/


		//


		//================
		// Connect to FTP
		//================
		// FM_CONNECTED
		cd.pvExcuteContext			= (PVOID)FM_CONNECTED;
		cd.bPassiveMode				= TRUE;//port方式下载小文件可以,pasv方式下载大文件可以?
		cd.dwTimeOut =1;
		lstrcpy( cd.szServerName, &g_FTPIp[g_nCurFTPNum*URL_LENGTH] );
		cd.wServerPort = g_FTPPort[g_nCurFTPNum];
		lstrcpy( cd.szUserName,	g_FTPID );
		lstrcpy( cd.szUserPassword, g_FTPPW);

		iR = g_pFtp->Initialize( m_hWnd, WM_RAIN_FTP_NOTIFY, 1);
		g_hSession = g_pFtp->CreateSession();
		if( g_hSession == NULL )
		{
			//QuitPatchDialog( TRUE );
			SetStart();
			return FALSE;
		}

		iR = g_pFtp->Connect( g_hSession, &cd);
		bConnecting = TRUE;
		m_bVersionChecking = FALSE;
	}
	//-------------
#endif
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMHAutoPatchDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMHAutoPatchDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMHAutoPatchDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
#ifdef _FOR_CLIENT_
	if( m_pHtmlViewNotice )
	{
		m_pHtmlViewNotice->DestroyWindow();
		delete m_pHtmlViewNotice;
		m_pHtmlViewNotice = NULL;
	}

	if( m_pProgressBar )
	{
		m_pProgressBar->DestroyWindow();
		delete m_pProgressBar;
		m_pProgressBar = NULL;
	}

	if( m_pStatus )
	{
		m_pStatus->DestroyWindow();
		delete m_pStatus;
		m_pStatus = NULL;
	}

	m_BGMManager.StopBGM();
	m_BGMManager.ReleaseSound();

#endif

	CDialog::OnClose();
}

BOOL CMHAutoPatchDlg::Clear3DData()
{
	char CurDir[MAX_PATH];
	CStrClass datafolder = "3DData\\";
	GetCurrentDirectory(MAX_PATH,CurDir);
	BOOL bSD = SetCurrentDirectory("3ddata");
	if(bSD == FALSE)
		return TRUE;	

	CDirectoryName dirname(NULL,NULL);
	dirname.LoadChild();

	SetCurrentDirectory(CurDir);

	dirname.SetPositionHead();
	DWORD kind;
	CStrClass filename;
	CStrClass tname;	

	dirname.SetPositionHead();
	while((kind=dirname.GetNextFileName(&filename)) != GETNEXTFILENAME_END)
	{
		if(kind != GETNEXTFILENAME_FILE)
			continue;
		if(filename == "")
			continue;

		tname = datafolder + filename;

		BOOL rt = DeleteFile(tname);
	}
	dirname.SetPositionHead();
	while((kind=dirname.GetNextFileName(&filename)) != GETNEXTFILENAME_END)
	{
		if(kind != GETNEXTFILENAME_DIRECTORY)
			continue;
		tname = datafolder + filename;
		RemoveDirectory(tname);
	}
	RemoveDirectory(datafolder);

	return TRUE;
}

LRESULT CMHAutoPatchDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch( message )
	{
	case WM_RAIN_FTP_NOTIFY:
		{
			int			iR;

			HANDLE		hSession;
			PVOID		pvdata;
			hSession = g_pFtp->GetCompletionHandle();
			g_ftpHSession = hSession;
			if( hSession == NULL )
				return TRUE;

			iR = g_pFtp->GetCompletionResult( hSession, &pvdata);
			_ASSERTE( iR == 0 );

			if( *((DWORD*)pvdata + 1) > CE_ERROR )
			{
				//----------------
				//Error Check Code
				//				char temp[256];
				//				sprintf(temp,"CE_ERROR   %u  %d",*((DWORD*)pvdata),*((DWORD*)pvdata + 1));
				//				MessageBox( temp );
				//----------------
				if( *((DWORD*)pvdata + 1) != CE_SHUTDOWN_WININET )
					return TRUE;

				//if( m_nRetryCount < g_nFTPCount*2 - 1 )	//犁矫档
				//{

				//	cd.bPassiveMode = FALSE;	//passive mode port mode(如果pasv模式失败,转为port模式)

				//	if( m_nRetryCount % 2 == 1 )		//FTP 函版
				//	{
				//		++g_nCurFTPNum;
				//		if( g_nCurFTPNum >= g_nFTPCount )
				//			g_nCurFTPNum = 0;

				//		lstrcpy( cd.szServerName, &g_FTPIp[g_nCurFTPNum*URL_LENGTH] );
				//		cd.wServerPort = g_FTPPort[g_nCurFTPNum];
				//	}

				//	//----------------
				//	//Error Check Code
				//	//				char temp[256];
				//	//					sprintf(temp,"Retry-%d : Passive-%d", m_nRetryCount+1, cd.bPassiveMode );
				//	//					MessageBox( temp );
				//	//----------------				

				//	cd.pvExcuteContext	= (PVOID)FM_CONNECTED;
				//	cd.dwTimeOut=1;
				//	iR = g_pFtp->Connect( g_hSession, &cd);
				//	bConnecting = TRUE;
				//	m_bVersionChecking = FALSE;
				//	++m_nRetryCount;
				//	return TRUE;
				//}				

				if( bConnecting == TRUE )
				{
					// Connect Fail
					MessageBox( "Failed to Connect to Server", g_szMBErrCap, MB_OK );
					//	QuitPatchDialog(TRUE);

					SetStart();
					return TRUE;
				}

				if( m_bVersionChecking )
				{
					MessageBox( "Now Uploading Patch Files. Try Again Later1.", g_szMBErrCap );
					//QuitPatchDialog(TRUE);
					SetStart();
					return TRUE;
				}

				MessageBox( "Disconnected with Server", g_szMBErrCap, MB_OK );
				//QuitPatchDialog(TRUE);
				SetStart();
				return TRUE;

				//------ checking error code ------------
				//				char temp[256];
				//				sprintf(temp,"CE_ERROR   %u  %d",*((DWORD*)pvdata),*((DWORD*)pvdata + 1));
				//				MessageBox( temp );
				//---------------------------------------

				//---------------------------------------
				/*
				if( *((DWORD*)pvdata + 1) != CE_SHUTDOWN_WININET )
				return TRUE;

				if( m_bVersionChecking )
				{
				MessageBox( "Now Uploading Patch Files. Try Again Later.", g_szMBErrCap );
				QuitPatchDialog(TRUE);
				return TRUE;
				}

				FILE* fporn = fopen( MH_VERSION_INFO_FILE, "r+" );
				if(fporn == NULL)
				{
				MessageBox( "Failed to Open Version File", g_szMBErrCap, MB_OK );
				QuitPatchDialog( TRUE );
				return FALSE;
				}

				char Version[64];
				fscanf( fporn, "%s", Version );
				strcpy( Version+VERSION_HEADER_TEXT_LENGTH, "00000000" );
				fseek( fporn, SEEK_SET, 0 );
				fprintf( fporn, "%s", Version );
				fclose(fporn);

				wsprintf( MHFile, "%s.zip", Version );	//罐阑 zip颇老 背眉
				cd.pvExcuteContext			= (PVOID)FM_CONNECTED;
				iR = g_pFtp->Connect( g_hSession, &cd);
				bConnecting = TRUE;
				m_bVersionChecking = FALSE;
				//				MessageBox( "Try to Reconnect!", g_szMBErrCap, MB_OK );
				//				MessageBox( "橇肺弊伐阑 促矫 角青秦 林技夸." );
				//				QuitPatchDialog(TRUE);

				return TRUE;
				*/
				//---------------------------------------
			}//end error if

			switch( *((DWORD*)pvdata) )
			{
			case FM_CONNECTED:
				{
					OnConnected( pvdata );
				}
				return TRUE;

			case FM_GET_CURRENT_DIRECTORY:
				{
					OnGetCurrentDirectory( pvdata );
				}
				return TRUE;

			case FM_GETFILELIST:
				{
					OnGetFileList( pvdata );
				}
				return TRUE;

			case FM_CONFIRM:
				{
					OnConfirm( pvdata );
				}
				return TRUE;

			case FM_DOWNLOAD:
				{
					OnDownLoad( pvdata );
				}
				return TRUE;
			case 0:
				{
					OnSetCurrentDirectory( pvdata );
				}
				return TRUE;
			}
		}
		return TRUE;

	case WM_RUNPROGRAM:
		{
			OnRunProgram();
		}
		return TRUE;
	case WM_JIERA:
		{
			OnJieraDownload();
		}
		return TRUE;
	case WM_RUN_ONOK:
		{
			OnOK();
		}
		return TRUE;
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CMHAutoPatchDlg::OnSetCurrentDirectory( LPVOID pvdata )
{
	//ZeroMemory(&dd,sizeof(dd));
	// FM_GET_CURRENT_DIRECTORY
	dd.pvExcuteContext			= (PVOID) FM_GET_CURRENT_DIRECTORY;
	iR = g_pFtp->GetCurrentDirectory( g_hSession, &dd);
	_ASSERTE( iR == 0);	

}

void CMHAutoPatchDlg::OnConnected( LPVOID pvdata )
{
	bConnecting = FALSE;
	Connect_data* pcd = (Connect_data*)pvdata;

	ZeroMemory(&dd,sizeof(dd));
	strcpy(dd.szCurrentDirectory,"LAUNCHERPATCH//");
	iR = g_pFtp->SetCurrentDirectory(g_hSession,&dd);
	_ASSERTE( iR == 0);	



	// FM_GET_CURRENT_DIRECTORY
	//dd.pvExcuteContext			= (PVOID) FM_GET_CURRENT_DIRECTORY;
	//iR = g_pFtp->GetCurrentDirectory( g_hSession, &dd);
	//_ASSERTE( iR == 0);	


	//滚怜眉农 甸绢皑.
	m_bVersionChecking = TRUE;
}

/*
//1. 泅犁 滚怜阑 罐绰促.
void CMHAutoPatchDlg::OnGetCurVersion( LPVOID pvdata )
{

}
*/

void CMHAutoPatchDlg::OnGetCurrentDirectory( LPVOID pvdata )
{
	Connect_data* pcd = (Connect_data*)pvdata;

	/*	//000
	// FM_GETFILELIST
	ld_zip.pvExcuteContext		= (PVOID) FM_GETFILELIST;
	ld_zip.dwFileMax			= FIND_DATA_MAX;
	ld_zip.pFile				= pwfd;
	ld_zip.pszSearchFile		= MHFile;

	iR = g_pFtp->GetList( g_hSession, &ld_zip);
	_ASSERTE( iR == 0);
	*/

	/*
	//捞芭 泪矫 林籍
	//刚历 滚怜颇老阑 促款罐绰促.
	//---
	fd_newversion.pvExcuteContext	= (PVOID) FM_CONFIRM;
	fd_newversion.bResume			= FALSE;
	lstrcpy( fd_newversion.szRemoteFile, newverFilename);
	lstrcpy( fd_newversion.szLocalFile, newverFilename);
	iR = g_pFtp->DownloadFile( g_hSession, &fd_newversion);
	_ASSERTE( iR == 0);		
	//---
	*/
	ld_zip.pvExcuteContext		= (PVOID) FM_GETFILELIST;
	ld_zip.dwFileMax			= FIND_DATA_MAX;
	ld_zip.pFile				= pwfd;
	ld_zip.pszSearchFile		= newverFilename;//MH_VERSION_INFO_FILE;

	iR = g_pFtp->GetList( g_hSession, &ld_zip);
	_ASSERTE( iR == 0);
}

void CMHAutoPatchDlg::OnGetFileList( LPVOID pvdata )
{
	List_data* pld = (List_data*)pvdata;

	if( pld->dwFileNum == 0 )
	{
		/*	//000
		/////////////////////////////////////////////////////	SC_DOWNLOAD_NOTICE
		fd_newversion.pvExcuteContext	= (PVOID) FM_CONFIRM;
		fd_newversion.bResume			= FALSE;
		lstrcpy( fd_newversion.szRemoteFile, newverFilename);
		lstrcpy( fd_newversion.szLocalFile, newverFilename);
		MessageBox(newverFilename,0,0);

		iR = g_pFtp->DownloadFile( g_hSession, &fd_newversion);
		_ASSERTE( iR == 0);
		return;
		*/
		// 啊厘 烤朝 滚怜 何磐 罐绰促.
		//---

		if( m_bVersionChecking )
		{
			MessageBox( "Now Uploading Patch Files. Try Again Later2.", g_szMBErrCap );
			g_pFtp->Shutdown(g_hSession);
			//	QuitPatchDialog( TRUE );
			SetStart();
			return;
		}

		FILE* fporn = fopen( MH_VERSION_INFO_FILE, "r" );
		if(fporn == NULL)
		{
			MessageBox( "Failed to Open Version File", g_szMBErrCap, MB_OK );
			g_pFtp->Shutdown(g_hSession);
			SetStart();
			//QuitPatchDialog( TRUE );
			return;
		}

		char Version[64];
		fscanf( fporn, "%s", Version );
		fclose( fporn );

		memcpy( szOldestFile, Version, VERSION_HEADER_TEXT_LENGTH);
		sprintf( &szOldestFile[VERSION_HEADER_TEXT_LENGTH], "00000000.zip" );

		/////////////////////////////////////////////////////	FM_GETFILELIST
		ld_zip.pvExcuteContext		= (PVOID) FM_GETFILELIST;
		ld_zip.dwFileMax			= FIND_DATA_MAX;
		ld_zip.pFile				= pwfd;
		ld_zip.pszSearchFile		= szOldestFile;

		iR = g_pFtp->GetList( g_hSession, &ld_zip);
		_ASSERTE( iR == 0);
		return;
		//---

	}

	if( m_bVersionChecking )
	{
		fd_newversion.pvExcuteContext	= (PVOID) FM_CONFIRM;
		fd_newversion.bResume			= FALSE;
		lstrcpy( fd_newversion.szRemoteFile, newverFilename);
		lstrcpy( fd_newversion.szLocalFile, newverFilename);
		iR = g_pFtp->DownloadFile( g_hSession, &fd_newversion);
		_ASSERTE( iR == 0);	
		return;
	}

	dwFileSizeMax = pld->pFile->nFileSizeLow;

	/////////////////////////////////////////////////////	SC_DOWNLOAD_ZIP
	fd_zip.pvExcuteContext		= (PVOID) FM_DOWNLOAD;
	fd_zip.bResume				= FALSE;
	lstrcpy( fd_zip.szRemoteFile, ld_zip.pszSearchFile);
	char newpath[MAX_PATH];
	sprintf(newpath,"%s%s",g_szMHpatchFile,".tmp");
	lstrcpy( fd_zip.szLocalFile, newpath);

	// Download MHFile
	iR = g_pFtp->DownloadFile( g_hSession, &fd_zip);
	_ASSERTE( iR == 0);

#ifndef _FOR_CHINA_
	SetStatusText( "Downloading Files" );
#else
	SetStatusText( "正在下载文件" );
#endif
}

void CMHAutoPatchDlg::OnConfirm( LPVOID pvdata )
{
	File_data* pfd = (File_data*)pvdata;


	if( pfd->CompletionError == CE_DOWNLOAD_PROGRESS )
	{
		return;
	}

	if( pfd->CompletionError != CE_SUCCEED  )
		return;

	FILE* fpnew = fopen( newverFilename, "r" );
	FILE* fpown = fopen( MH_VERSION_INFO_FILE, "r" );
	if(fpnew == NULL || fpown == NULL)
	{
		MessageBox( "Failed to Check New Version", g_szMBErrCap, MB_OK );
		g_pFtp->Shutdown(g_hSession);
		//	QuitPatchDialog( TRUE );
		SetStart();
		if(fpnew)
			fclose(fpnew);
		if(fpown)
			fclose(fpown);

		return;
	}

	char temp1[256], temp2[256];
	fscanf( fpnew, "%s", temp1 );
	fscanf( fpown, "%s", temp2 );
	fclose( fpnew );
	fclose( fpown );

	sprintf(szPatchServerFileVer,"%s",temp1);
	sprintf(szLocalFileVer,"%s",temp2);


	//DeleteFile( newverFilename );

	m_bVersionChecking = FALSE;

	if( strcmp( temp1, temp2 ) == 0 )	// 比较更新服务器上的版本号与本地版本，当本地版本号小于服务器版本更新，否则什么也不做
	{			
		g_pFtp->Shutdown(g_hSession);

		SetReadyToStart();	
	}
	else
	{	// 备滚怜, 啊厘 烤朝 滚怜阑 罐绰促.	


		//	memcpy( MHFile, Version, VERSION_HEADER_TEXT_LENGTH);
		sprintf( MHFile, "%s%s",temp2,".zip" );
		/////////////////////////////////////////////////////	SC_DOWNLOAD_ZIP
		/*	//00
		memcpy( szOldestFile, temp2, VERSION_HEADER_TEXT_LENGTH);
		sprintf( &szOldestFile[VERSION_HEADER_TEXT_LENGTH], "00000000.zip" );

		/////////////////////////////////////////////////////	FM_GETFILELIST
		ld_zip.pvExcuteContext		= (PVOID) FM_GETFILELIST;
		ld_zip.dwFileMax			= FIND_DATA_MAX;
		ld_zip.pFile				= pwfd;
		ld_zip.pszSearchFile		= szOldestFile;

		iR = g_pFtp->GetList( g_hSession, &ld_zip);
		_ASSERTE( iR == 0);
		*/
		//泅犁 滚怜捞 乐绰瘤 犬牢
		//---
		// FM_GETFILELIST
		ld_zip.pvExcuteContext		= (PVOID) FM_GETFILELIST;
		ld_zip.dwFileMax			= FIND_DATA_MAX;
		ld_zip.pFile				= pwfd;
		ld_zip.pszSearchFile		= MHFile;

		iR = g_pFtp->GetList( g_hSession, &ld_zip);
		_ASSERTE( iR == 0);		
		//---

	}
}
void CMHAutoPatchDlg::OnJieraDownload()
{
	if( 1 ) //pfd->CompletionError == CE_SUCCEED
	{
		g_dwProLv = 0;//防止874消息响应(丑陋的补丁)
		g_pFtp->Shutdown(g_hSession);
		
		//	AfxMessageBox("ff");	
		//	拘绵阑 钱磊.
#ifndef _FOR_CHINA_
		SetStatusText( "Extracting Files" );
#else
		SetStatusText( "正在解压文件" );
#endif
		SetProgressBar( 0 );
		char g_szOldMHpatchFile[MAX_PATH];
		strcpy(g_szOldMHpatchFile,g_szMHpatchFile);
		strcat(g_szOldMHpatchFile,".tmp");
		MoveFile(g_szOldMHpatchFile,g_szMHpatchFile);

		BOOL bReturn = Extract_DownloadFile( g_szMHpatchFile );

		DeleteFile( g_szMHpatchFile );

		if( !bReturn )
		{
			//	拘绵 钱扁 角菩.			
			MessageBox( "Failed To Extract", g_szMBErrCap, MB_OK );
			//	QuitPatchDialog( TRUE );
			SetStart();
			return;
		}

		// yh
		// 颇老阑 菩虐隆茄促.
		if(g_bUsingEnginePack)
		{
			StartPack3DData();
		}
		else
		{
			//AfxMessageBox("保存版本号");
			bReturn = UpdateVersionInfoFile( MH_VERSION_INFO_FILE, newverFilename);

			// jsd			// resource 弃歹狼 filelist历厘
			((CMHAutoPatchApp*)AfxGetApp())->SaveFileList();

			SetReadyToStart();
		}
	}
}
void CMHAutoPatchDlg::OnDownLoad( LPVOID pvdata )
{
	File_data* pfd = (File_data*)pvdata;


	if( pfd->CompletionError == CE_DOWNLOAD_PROGRESS )		// ZIP 颇老 傈价 柳青惑炔篮 包缴 乐促.
	{	
		DWORD dwCurrentSize = pfd->dwTransferBytes;
		DWORD dwProLv = DWORD(PRO_RANGE_DN * ((float)(dwCurrentSize) / (float)(dwFileSizeMax)) );
		SetProgressBar( dwProLv );
		char txtS[256];
		sprintf(txtS,"%dKB|%dKB",int(dwCurrentSize/1024),int(dwFileSizeMax/1024));
		SetStatusText( txtS );		
		g_dwProLv = dwProLv;
		if (g_dwProLv == 255)
		{
			//Sleep(2000);
			g_pFtp->CloseSession(g_hSession);
			::SendMessage( m_sHWnd, WM_JIERA, 0, 0 );
		}
	}

	if( pfd->CompletionError == CE_SUCCEED )
	{
		//::SendMessage( m_sHWnd, WM_JIERA, 0, 0 );
	}
//	if( pfd->CompletionError == CE_SUCCEED )
//	{
//		g_dwProLv = 0;//防止874消息响应(丑陋的补丁)
//		g_pFtp->Shutdown(g_hSession);
//		//	AfxMessageBox("ff");	
//		//	拘绵阑 钱磊.
//#ifndef _FOR_CHINA_
//		SetStatusText( "Extracting Files" );
//#else
//		SetStatusText( "正在解压文件" );
//#endif
//		SetProgressBar( 0 );
//		char g_szOldMHpatchFile[MAX_PATH];
//		strcpy(g_szOldMHpatchFile,g_szMHpatchFile);
//		strcat(g_szOldMHpatchFile,".tmp");
//		MoveFile(g_szOldMHpatchFile,g_szMHpatchFile);
//		
//		BOOL bReturn = Extract_DownloadFile( g_szMHpatchFile );
//
//		DeleteFile( g_szMHpatchFile );
//
//		if( !bReturn )
//		{
//			//	拘绵 钱扁 角菩.			
//			MessageBox( "Failed To Extract", g_szMBErrCap, MB_OK );
//			//	QuitPatchDialog( TRUE );
//			SetStart();
//			return;
//		}
//
//		// yh
//		// 颇老阑 菩虐隆茄促.
//		if(g_bUsingEnginePack)
//		{
//			StartPack3DData();
//		}
//		else
//		{
//			//AfxMessageBox("保存版本号");
//			bReturn = UpdateVersionInfoFile( MH_VERSION_INFO_FILE, newverFilename);
//
//			// jsd			// resource 弃歹狼 filelist历厘
//			((CMHAutoPatchApp*)AfxGetApp())->SaveFileList();
//
//			SetReadyToStart();
//		}
//	}
}

BOOL CMHAutoPatchDlg::RunMH()
{
	/*
	{
	FILE* fp = fopen("MHAutoPatch.new","rb");
	if(fp != NULL)
	{
	fclose(fp);
	_execl("MHExecuter.exe","a",NULL);
	}
	}
	{
	FILE* fp = fopen("SS3DGFunc.new","rb");
	if(fp != NULL)
	{
	fclose(fp);
	_execl("MHExecuter.exe","a",NULL);
	}
	}
	*/
	SelectGFunc();

	BOOL		bReturn;

	STARTUPINFO	sp;

	memset( &sp, 0, sizeof(STARTUPINFO));
	sp.cb = sizeof(STARTUPINFO);
	sp.dwFlags = STARTF_USEPOSITION | STARTF_USESIZE | STARTF_USESTDHANDLES; //STARTF_USESHOWWINDOW 
	sp.wShowWindow = SW_SHOW; 

	PROCESS_INFORMATION pi;

	if(strlen(g_ExecutableFileName) == 0)
		return TRUE;
	else
	{
		bReturn = CreateProcess( LAUNCHER_FILE,		// name of executable module
			//#ifdef _MANUAL_PATCH_
			//								" anrgiddlatl",                // command line string									
			//#else
			" aa",                // command line string   anrgideoqkr
			//#endif
			NULL, // SD
			NULL,  // SD
			FALSE,                      // handle inheritance option
			CREATE_DEFAULT_ERROR_MODE | NORMAL_PRIORITY_CLASS,                     // creation flags
			NULL,                      // new environment block
			NULL,                // current directory name
			&sp,               // startup information
			&pi // process information
			);
	}

	if( !bReturn )
		return FALSE;

	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );

	return TRUE;
}


BOOL CMHAutoPatchDlg::Extract_DownloadFile( char* lpFileName )
{
	DWORD	dwi;
	int		iMax;
	bool	bReturn;

	g_pZip->Open( g_szMHpatchFile, CZipArchive::zipOpen);
	iMax = g_pZip->GetCount();
	bReturn = TRUE;

	CZipFileHeader head;
	for ( dwi = 0; (int)dwi < iMax; dwi++)
	{
		if( g_pZip->GetFileInfo( head, dwi)  == false )
		{
			bReturn = FALSE;
			break;
		}
		CZipString filename = head.GetFileName();
		SetFileAttributes(filename,FILE_ATTRIBUTE_NORMAL);

		if( g_pZip->ExtractFile( dwi, NULL ) == false )
		{
			bReturn = FALSE;
			break;
		}
		//
		//	柳青惑炔 焊咯林扁.
		DWORD dwProLv = (DWORD)(PRO_RANGE_EX * ((float)(dwi + 1) / (float)iMax));
		SetProgressBar( dwProLv );

#ifdef _FOR_CLIENT_
		MSG msg;
		if( GetMessage(&msg, NULL, 0, 0) ) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
#endif
	}

	g_pZip->Close();

	return bReturn;
}

void CMHAutoPatchDlg::StartPack3DData()
{	
	SetProgressBar( 0 );
#ifndef _FOR_CHINA_
	SetStatusText( "Packing Files" );
#else
	SetStatusText( "正在打包文件" );
#endif

	_beginthread( Pack3DData, 0, m_hWnd );
}


BOOL CMHAutoPatchDlg::UpdateVersionInfoFile( char* lpszOldVerInfoFile, char* lpszNewVerInfoFile )
{




	HANDLE			hFind;
	WIN32_FIND_DATA wfd;
	BOOL			bReturn;

	hFind = FindFirstFile( lpszNewVerInfoFile, &wfd);
	if( hFind == INVALID_HANDLE_VALUE )
		return FALSE;

	bReturn = DeleteFile( lpszOldVerInfoFile);
	if( !bReturn )
	{
		MessageBox( "Failed To Delete VersionFile", g_szMBErrCap, MB_OK );
		return FALSE;
	}

	bReturn = MoveFile( lpszNewVerInfoFile, lpszOldVerInfoFile); // for win2000 later ==> , MOVEFILE_REPLACE_EXISTING);
	if( !bReturn )
	{
		MessageBox( "Failed To Rename VersionFile", g_szMBErrCap, MB_OK );
		return FALSE;
	}

	//检查当前版本号///////////////////////////////////////////////////////
	FILE* fpown = fopen( MH_VERSION_INFO_FILE, "r" );
	if(fpown == NULL)
	{
		MessageBox( "Failed to Check New Version", g_szMBErrCap, MB_OK ); 
		QuitPatchDialog( TRUE );	
	}

	char temp1[256];
	char temp2[256];

	fscanf( fpown, "%s", temp2 );
	fclose( fpown );

	sprintf( temp1, "%s%s","  SkyOL update Launcher...          当前版本： ", temp2 );
	//////////////////////////////////////////////////////////////////////////
	m_stcWindowMode.SetFontColor( RGB( 0, 220, 220 ) );
	m_stcWindowMode.SetWindowText( temp1 );//窗口模式
	m_stcWindowMode.SetLogFont( "宋体", 12, GB2312_CHARSET  );//GB2312_CHARSET
	m_stcWindowMode.SetWindowPos( NULL, 10, 6, 60, 17, SWP_NOZORDER );
	CRect temp_rc;
	temp_rc.SetRect(1, 1, 638, 40);
	InvalidateRect( temp_rc );
	UpdateWindow();
	//////////////////////////////////////////////////////////////////////////

	return TRUE;
}


BOOL IsSupportSSE()
{
	BOOL	bResult = TRUE;
	__try 
	{
		// 	xorps	xmm0,xmm0
		__asm _emit 0x0f 
			__asm _emit 0x57 
			__asm _emit 0xc0

	}
	__except ( EXCEPTION_EXECUTE_HANDLER) 
	{
		bResult = FALSE;

	}
	return bResult;
}

BOOL CMHAutoPatchDlg::SelectGFunc()
{
	if (IsSupportSSE())
		CopyFile("SS3DGFuncSSE.dll","SS3DGFunc.dll",FALSE);
	else
		CopyFile("SS3DGFuncN.dll","SS3DGFunc.dll",FALSE);
	return TRUE;
}

BOOL CMHAutoPatchDlg::InitEngine()
{
	SelectGFunc();

	HRESULT hr;

	g_hExecutiveHandle = LoadLibrary("SS3DExecutiveForMuk.dll");
	if( g_hExecutiveHandle == NULL )
		return FALSE;

	CREATE_INSTANCE_FUNC        pFunc;
	pFunc = (CREATE_INSTANCE_FUNC)GetProcAddress(g_hExecutiveHandle,"DllCreateInstance");
	hr = pFunc((void**)&g_pExecutive);
	if (hr != S_OK)
	{		
		::MessageBox( m_sHWnd, "Failed To Create Executive", g_szMBErrCap, MB_OK );
		return FALSE;
	}

	// geometry DLL狼 捞抚, renderer DLL狼 捞抚阑 持绢林搁 executive啊 舅酒辑 积己,檬扁拳秦霖促.
	if(g_pExecutive->InitializeFileStorageWithoutRegistry("SS3DFileStorage.dll",
		12000,4096,128,FILE_ACCESS_METHOD_ONLY_FILE,NULL,0) == FALSE)
		return FALSE;

	return TRUE;
}


BOOL AddDataToPackFile( CStrClass pakname, CStrClass dir )
{
	I4DyuchiFileStorage* pFS;
	g_pExecutive->GetFileStorage(&pFS);

	void* pPakFile = pFS->MapPackFile((char*)(LPCTSTR)pakname);
	if(pPakFile == NULL)
		return FALSE;

	BOOL rt = pFS->LockPackFile(pPakFile,0);
	if(rt == FALSE)
		return FALSE;

	char CurDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,CurDir);
	BOOL bSD = SetCurrentDirectory(dir);

	CDirectoryName dirname(NULL,NULL);
	dirname.LoadChild();
	SetCurrentDirectory(CurDir);

	dirname.SetPositionHead();
	DWORD kind;
	CStrClass filename;
	CStrClass tname;
	while((kind=dirname.GetNextFileName(&filename)) != GETNEXTFILENAME_END)
	{
		if(kind != GETNEXTFILENAME_FILE)
			continue;
		if(filename == "")
			continue;

		tname = dir + "\\" + filename;

		if(pFS->InsertFileToPackFile(pPakFile,(char*)(LPCTSTR)tname) == FALSE)
			return FALSE;
	}

	BOOL unlock = pFS->UnlockPackFile(pPakFile,PackingCallBackFunc);
	if(unlock == FALSE)
		return FALSE;
	pFS->UnmapPackFile(pPakFile);
	pFS->Release();

	return TRUE;

}


void CMHAutoPatchDlg::Pack3DData(void* p)
{
	CDirectoryName dirname(NULL,NULL);
	DWORD kind;
	CStrClass filename;
	CStrClass pakname,tname;
	BOOL bSuccess = TRUE;
	HWND hDlg = (HWND)p;

	char CurDir[MAX_PATH];
	CStrClass datafolder = "3DData\\";
	GetCurrentDirectory(MAX_PATH,CurDir);
	BOOL bSD = SetCurrentDirectory("3ddata");
	if(bSD == FALSE)
	{
		bSuccess = TRUE;
		goto packend;
	}

	dirname.LoadChild();

	SetCurrentDirectory(CurDir);
	if( InitEngine() == FALSE )
	{
		bSuccess = FALSE;
		goto packend;
	}

	dirname.SetPositionHead();
	while((kind=dirname.GetNextFileName(&filename)) != GETNEXTFILENAME_END)
	{
		if(kind != GETNEXTFILENAME_DIRECTORY)
			continue;
		if(filename == "")
			continue;

		if(filename.Right(1) == "\\")
			filename = filename.Left(filename.GetLength()-1);

		pakname = filename + ".pak";
		tname = datafolder + filename;
		if(AddDataToPackFile(pakname,tname) == FALSE)
		{
			bSuccess = FALSE;
			goto packend;
		}
	}

	dirname.SetPositionHead();
	while((kind=dirname.GetNextFileName(&filename)) != GETNEXTFILENAME_END)
	{
		if(kind != GETNEXTFILENAME_FILE)
			continue;
		if(filename == "")
			continue;

		tname = datafolder + filename;

		BOOL rt = DeleteFile(tname);
	}
	dirname.SetPositionHead();
	while((kind=dirname.GetNextFileName(&filename)) != GETNEXTFILENAME_END)
	{
		if(kind != GETNEXTFILENAME_DIRECTORY)
			continue;
		tname = datafolder + filename;
		RemoveDirectory(tname);
	}
	RemoveDirectory(datafolder);

packend:

	if( bSuccess )
		::SendMessage( m_sHWnd, WM_RUNPROGRAM, 0, 0 );
	else
		::SendMessage( m_sHWnd, WM_RUNPROGRAM, 0, 0 );

	return;
}


DWORD __stdcall PackingCallBackFunc( DWORD dwCurCount,DWORD dwTotalCount,void* pArg )
{
	DWORD PackingProcess = (PRO_RANGE_EX*dwCurCount) / dwTotalCount;

#ifdef _FOR_CLIENT_

	//CMHAutoPatchDlg* pMainDlg = (CMHAutoPatchDlg*)AfxGetMainWnd();.
	CMHAutoPatchDlg* pMainDlg = (CMHAutoPatchDlg*)AfxGetApp()->GetMainWnd();
	CSkinProgressBar* pBar = pMainDlg->GetProgressBar();
	//  pMainDlg->SetProgressBar(PackingProcess);
	pBar->SetPos( PackingProcess );
#endif

	return 0;
}

void CMHAutoPatchDlg::OnRunProgram()
{
	BOOL bReturn = UpdateVersionInfoFile( MH_VERSION_INFO_FILE, newverFilename);

	// jsd	// resource 弃歹狼 filelist历厘
	((CMHAutoPatchApp*)AfxGetApp())->SaveFileList();

	SetReadyToStart();
}

void CMHAutoPatchDlg::SetReadyToStart()
{
	int nResult = 0;

	//#ifdef _MANUAL_PATCH_
	//		nResult = 11;
	//		QuitPatchDialog( nResult );
	//		return;
	//#endif

	{
		FILE* fp = fopen("MHAutoPatch.new","rb");
		if(fp != NULL)
		{
			fclose(fp);
			nResult = 11;
			//		QuitPatchDialog( nResult );
			SetStart();
			return;
			/*
			_execl("MHExecuter.exe","a",NULL);
			bContinue = FALSE;
			EndDialog( TRUE );
			*/
		}
	}
	{
		FILE* fp = fopen("SS3DGFunc.new","rb");
		if(fp != NULL)
		{
			fclose(fp);
			nResult = 11;
			//QuitPatchDialog( nResult );
			SetStart();
			return;
			/*
			_execl("MHExecuter.exe","a",NULL);
			bContinue = FALSE;
			EndDialog( TRUE );
			*/
		}
	}

#ifdef _FOR_CLIENT_
	m_bCanStart = TRUE;
	m_txtStart.SetFontColor( RGB( 220, 220, 0 ) );

	////在这里检测服务器最新的版本文件是否和本地文件相同
	////不相同---执行selupdate
	////相同---enable-ok
	//if (	strcmp(szPatchServerFileVer,szLocalFileVer)==0)
	//{
	//	m_btnOK.EnableWindow();
	//}
	//else
	//{
	//	CMHAutoPatchApp * dd;
	//	dd = (CMHAutoPatchApp * )AfxGetApp();

	//	if( dd->SelfUpdate() == TRUE )
	//	{
	//		((CMHAutoPatchApp*)AfxGetApp())->ReleaseHandle();
	//		m_BGMManager.StopBGM();
	//		m_BGMManager.ReleaseSound();
	//		QuitPatchDialog( TRUE );	
	//		CDialog::OnOK();
	//	}		
	//}



	/*
	CDC MemDC;
	MemDC.CreateCompatibleDC( m_btnOK.GetDC() );
	MemDC.SelectObject( m_hBmpPlayPress);
	m_btnOK.GetDC()->BitBlt( 0, 0, 112, 46, &MemDC, 0, 0, SRCCOPY );
	*/

#ifndef _FOR_CHINA_
	m_pStatus->MoveWindow( 4, 167, 116, 15 );
	m_rcStatus.SetRect( 4, 167, 4+116, 167+15 );
	SetStatusText( "Patch Completed" );
#else
	m_pStatus->MoveWindow( 4, 405, 116, 15 );
	m_rcStatus.SetRect( 4, 405, 4+116, 405+15 );
	SetStatusText( "补丁安装完成" );
#endif

	m_pProgressBar->SetPos( 255 );

	InvalidateRect( m_rcStatus );
	UpdateWindow();

	SaveOption();

	::SendMessage( m_sHWnd, WM_RUN_ONOK, 0, 0 );
#else

	EndDialog( TRUE );
#endif
}

void CMHAutoPatchDlg::OnOK() 
{
	// TODO: Add extra validation here
	((CMHAutoPatchApp*)AfxGetApp())->ReleaseHandle();

#ifdef _FOR_CLIENT_
	if( !m_bCanStart )
		return;

	SaveOption();
	BOOL bReturn = RunMH();
	if( !bReturn )
	{
		MessageBox( "Failed To Run Client", g_szMBErrCap, MB_OK );
	}

	m_BGMManager.StopBGM();
	m_BGMManager.ReleaseSound();

	/*特别注意这个地方
	//////////////////////////////////////////////////////////////////////////在这里将程序内部的bin资源写入磁盘，一般是需要保护的配置文件
	char   filepath   [_MAX_PATH];   
	GetModuleFileName(AfxGetInstanceHandle(),filepath,_MAX_PATH);

	for (int i=strlen(filepath);i>1;i--)
	{
	if (filepath[i]=='\\')
	{
	filepath[i+1]=0;
	break;
	}
	}

	char szServiceExePath[_MAX_PATH];
	wsprintf(szServiceExePath,"%s%s",filepath,"Resource\\AbilityBaseInfo.bin");
	// AfxMessageBox (szServiceExePath);
	HRSRC   hRsrc;   
	hRsrc=FindResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(BIN_AbilityBaseInfo),"BIN");
	DWORD dwDllSize = SizeofResource(NULL, hRsrc); 
	HGLOBAL hGlobal = LoadResource(NULL, hRsrc); 
	LPVOID pBuffer = LockResource(hGlobal);
	DWORD dwWritten;
	HANDLE hFileForDll = ::CreateFile(     
	szServiceExePath,   
	GENERIC_WRITE,   
	0,   
	NULL,   
	CREATE_ALWAYS,   
	FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM,   
	NULL   
	);   

	if (hFileForDll == INVALID_HANDLE_VALUE)   
	{   
	AfxMessageBox (TEXT("释放出错"));

	goto exit;  
	}   

	WriteFile (hFileForDll, pBuffer, dwDllSize, &dwWritten, NULL);   
	CloseHandle (hFileForDll);   
	UnlockResource(hGlobal);
	FreeResource(hGlobal);
	wsprintf(szServiceExePath,"%s%s",filepath,"Resource\\KyungGongInfo.bin");
	hRsrc=FindResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(BIN_KyungGongInfo),"BIN");
	dwDllSize = SizeofResource(NULL, hRsrc); 
	hGlobal = LoadResource(NULL, hRsrc); 
	pBuffer = LockResource(hGlobal);

	hFileForDll = ::CreateFile(     
	szServiceExePath,   
	GENERIC_WRITE,   
	0,   
	NULL,   
	CREATE_ALWAYS,   
	FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM,   
	NULL   
	);   

	if (hFileForDll == INVALID_HANDLE_VALUE)   
	{   
	AfxMessageBox (TEXT("释放出错"));

	goto exit;  
	}   

	WriteFile (hFileForDll, pBuffer, dwDllSize, &dwWritten, NULL);   
	CloseHandle (hFileForDll);  
	UnlockResource(hGlobal);
	FreeResource(hGlobal); 
	//////////////////////////////////////////////////////////////////////////bin资源文件写入磁盘结束
	*/
#endif

exit:
	CDialog::OnOK();
}

#ifdef _FOR_CLIENT_
void CMHAutoPatchDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	CDC MemDC;

	switch( nIDCtl )
	{
	case IDCANCEL:
		{
			MemDC.CreateCompatibleDC( m_btnCancel.GetDC() );
			if( lpDrawItemStruct->itemState & ODS_SELECTED )
				MemDC.SelectObject( m_hBmpEBtnPress );
			else
				MemDC.SelectObject( m_hBmpEBtn );



			m_btnCancel.GetDC()->BitBlt( 0, 0, 25, 25, &MemDC, 0, 0, SRCCOPY );

		}
		break;
	case IDOK://开始游戏按钮
		{
			MemDC.CreateCompatibleDC( m_btnOK.GetDC() );
			if( lpDrawItemStruct->itemState & ODS_SELECTED )
				MemDC.SelectObject( m_hBmpPlayPress);
			else
				MemDC.SelectObject( m_hBmpPlayPress );

			if (lpDrawItemStruct->itemState & ODS_DISABLED )
			{
				MemDC.SelectObject( m_hBmpPlayBtn );
			}

			m_btnOK.GetDC()->BitBlt( 0, 0, 112, 46, &MemDC, 0, 0, SRCCOPY );
			m_txtStart.Invalidate();
		}
		break;
	case IDC_HOMEPAGE://主页按钮
		{
			MemDC.CreateCompatibleDC( m_btnHomepage.GetDC() );
			if( lpDrawItemStruct->itemState & ODS_SELECTED )
				MemDC.SelectObject( m_hBmpHomeBtnPress );
			else
				MemDC.SelectObject( m_hBmpHomeBtn);

			m_btnHomepage.GetDC()->BitBlt( 0, 0, 105, 25, &MemDC, 0, 0, SRCCOPY );
			m_txtHomepage.Invalidate();
		}
		break;
	case IDC_REGIST://注册按钮
		{
			MemDC.CreateCompatibleDC( m_btnRegist.GetDC() );
			if( lpDrawItemStruct->itemState & ODS_SELECTED )
				MemDC.SelectObject( m_hBmpRegPress );
			else
				MemDC.SelectObject( m_hBmpRegBtn );

			m_btnRegist.GetDC()->BitBlt( 0, 0, 105, 25, &MemDC, 0, 0, SRCCOPY );
			m_txtJoin.Invalidate();
		}
		break;
	case IDC_WINDOWMODE://选择是否窗口模式按钮
		{
			MemDC.CreateCompatibleDC( m_btnWindowMode.GetDC() );

			if( lpDrawItemStruct->itemAction & ODA_SELECT && lpDrawItemStruct->itemState & ODS_SELECTED )
			{
				m_bWindowModeChecked ^= TRUE;
			}

			if( m_bWindowModeChecked )
			{
				//			if( lpDrawItemStruct->itemState & ODS_CHECKED )
				//			if( lpDrawItemStruct->itemAction & ODA_SELECT || lpDrawItemStruct->itemState & ODS_SELECTED)
				MemDC.SelectObject( m_hBmpCBCheck );
			}
			else
				MemDC.SelectObject( m_hBmpCB );

			m_btnWindowMode.GetDC()->BitBlt( 0, 0, 25, 25, &MemDC, 0, 0, SRCCOPY );
			//m_txtJoin.Invalidate();
		}
		break;
#ifdef _FOR_CHINA_
	case IDC_BBS://百宝箱（暂时借用）
		{
			MemDC.CreateCompatibleDC( m_btnBBS.GetDC() );
			if( lpDrawItemStruct->itemState & ODS_SELECTED )
				MemDC.SelectObject( m_hBmpBaoBtnPress );
			else
				MemDC.SelectObject( m_hBmpBaoBtn );

			m_btnBBS.GetDC()->BitBlt( 0, 0, 105, 25, &MemDC, 0, 0, SRCCOPY );
			m_txtBBS.Invalidate();
		}
		break;
#endif

	}

	MemDC.DeleteDC();

	//CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CMHAutoPatchDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//OnNcLButtonDown( HTCAPTION, point );
	CRect rt( 0, 0, 600, 165 );
	if( rt.PtInRect( point ) )
	{
		SendMessage( WM_NCLBUTTONDOWN, HTCAPTION, 0 );
		CDialog::OnLButtonDown(nFlags, point);
	}
}

void CMHAutoPatchDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//OnNcLButtonUp( HTCAPTION, point );
	CRect rt( 0, 0, 600, 165 );
	if( rt.PtInRect( point ) )
	{
		SendMessage( WM_NCLBUTTONUP, HTCAPTION, 0 );
		CDialog::OnLButtonUp(nFlags, point);
	}
}

void CMHAutoPatchDlg::OnHomepage() 
{
	// TODO: Add your control notification handler code here
#ifndef _FOR_CHINA_
	ShellExecute( NULL, NULL, "Iexplore.exe", "http://www.darkstoryonline.com", NULL, SW_SHOWNORMAL );
#else
	ShellExecute( NULL, NULL, "Iexplore.exe",  "http://www.google.com", NULL, SW_SHOWNORMAL );
#endif

}

void CMHAutoPatchDlg::OnRegist() 
{
	// TODO: Add your control notification handler code here
#ifndef _FOR_CHINA_
	ShellExecute( NULL, NULL, "Iexplore.exe", "http://www.darkstoryonline.com/member_zone/member_1_01.asp", NULL, SW_SHOWNORMAL );
#else
	ShellExecute( NULL, NULL, "Iexplore.exe", "http://www.google.com", NULL, SW_SHOWNORMAL );
#endif

}

void CMHAutoPatchDlg::OnBBS() 
{
	// TODO: Add your control notification handler code here
#ifdef _FOR_CHINA_
	ShellExecute( NULL, NULL, "Iexplore.exe", "http://www.google.com", NULL, SW_SHOWNORMAL );
#endif

}

void CMHAutoPatchDlg::SelectTitlePicture()
{

	//---load main image
#ifndef _FOR_CHINA_
	m_hBmpMain = (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/patch-01.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
#else
	m_hBmpMain = (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/gwbk.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
#endif
	m_picMain.SetBitmap( m_hBmpMain );

	//---load buttons * progress bar image
	//申请会员等按钮图片
	m_hBmpNBtn		= (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/patch-04.bmp.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
	m_hBmpNBtnPress = (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/patch-05.bmp.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );

	//退出X按钮
	m_hBmpEBtn		= (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/patch-02.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );//X正常图标
	m_hBmpEBtnPress = (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/patch-03.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );//X按下图标

	//全屏check按钮
	m_hBmpCB		= (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/checkwin_no.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );//空白图片
	m_hBmpCBCheck	= (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/checkwin_yes.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );//勾选图片

	//开始游戏按钮
	m_hBmpPlayBtn		= (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/start.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
	m_hBmpPlayPress	= (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/start_.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );

	//注册按钮
	m_hBmpRegBtn		= (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/join.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
	m_hBmpRegPress	= (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/join_.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );

	//主页按钮
	m_hBmpHomeBtn		= (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/HomePage.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
	m_hBmpHomeBtnPress	= (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/HomePage_.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );

	//百宝按钮
	m_hBmpBaoBtn		= (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/ItemShop.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
	m_hBmpBaoBtnPress	= (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/ItemShop_.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );

	//---load title image

	srand( GetTickCount() );
	int nPic = rand()%6;

	nPic = 99;//关闭随机背景切换
	switch( nPic )
	{
	case 0:
		m_hBmpTitle = (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/patch-im-01.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
		break;
	case 1:
		m_hBmpTitle = (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/patch-im-02.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
		break;
	case 2:
		m_hBmpTitle = (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/patch-im-03.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
		break;
	case 3:
		m_hBmpTitle = (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/patch-im-04.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
		break;
	case 4:
		m_hBmpTitle = (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/patch-im-05.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
		break;
	case 5:
		m_hBmpTitle = (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/patch-im-06.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
		break;
	case 99:
		m_hBmpTitle = (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/gwbk.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
		break;
	default:
		m_hBmpTitle = (HBITMAP)LoadImage( AfxGetApp()->m_hInstance, "./Image/Autopatch/patch-im-01.BMP", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
		break;
	}

	m_picTitle.SetBitmap( m_hBmpTitle );
	m_picTitle.SetWindowPos( NULL, 1, 1, 669, 485, SWP_NOZORDER );

	//---select sound
	m_BGMManager.PlayBGM( ".\\Sound\\BGM\\BG_Town1.mp3" );
}
#endif

void CMHAutoPatchDlg::SetStatusText( char* pText )
{
#ifdef _FOR_CLIENT_
	m_pStatus->SetWindowText( pText );
	InvalidateRect( m_rcStatus );
	UpdateWindow();
#else
	GetDlgItem( IDC_TEXT_SERVER )->SetWindowText( pText );
#endif

}

void CMHAutoPatchDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
#ifdef _FOR_CLIENT_
	m_BGMManager.StopBGM();
	m_BGMManager.ReleaseSound();
#endif

	g_pFtp->Shutdown(g_hSession);

	SaveOption();

	CDialog::OnCancel();
}

void CMHAutoPatchDlg::SetProgressBar( DWORD dwLv )
{
#ifdef _FOR_CLIENT_
	m_pProgressBar->SetPos( dwLv );
#else
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS_PATCH))->SetPos( dwLv );
#endif
}

void CMHAutoPatchDlg::QuitPatchDialog( int nResult )
{
#ifdef _FOR_CLIENT_
	m_BGMManager.StopBGM();
	m_BGMManager.ReleaseSound();
	if( m_hBmpMain )		DeleteObject( m_hBmpMain );
	if( m_hBmpTitle )		DeleteObject( m_hBmpTitle );
	if( m_hBmpNBtn )		DeleteObject( m_hBmpNBtn );
	if( m_hBmpNBtnPress )	DeleteObject( m_hBmpNBtnPress );
	if( m_hBmpEBtn )		DeleteObject( m_hBmpEBtn );
	if( m_hBmpEBtnPress )	DeleteObject( m_hBmpEBtnPress );
	if( m_hBmpCB )			DeleteObject( m_hBmpCB );
	if( m_hBmpCBCheck )		DeleteObject( m_hBmpCBCheck );

	if( m_hBmpPlayBtn )		DeleteObject( m_hBmpPlayBtn);
	if( m_hBmpPlayPress )		DeleteObject( m_hBmpPlayPress );

	if( m_hBmpRegBtn )		DeleteObject( m_hBmpRegBtn);
	if( m_hBmpRegPress )		DeleteObject( m_hBmpRegPress );

	if( m_hBmpHomeBtn )		DeleteObject( m_hBmpHomeBtn);
	if( m_hBmpHomeBtnPress )		DeleteObject( m_hBmpHomeBtnPress );

	if( m_hBmpBaoBtn)		DeleteObject( m_hBmpBaoBtn);
	if( m_hBmpBaoBtnPress )		DeleteObject( m_hBmpBaoBtnPress );

	
	

	SaveOption();

#endif
	EndDialog( nResult );
}

#ifdef _FOR_CLIENT_
BOOL CMHAutoPatchDlg::LoadOption()
{
	FILE* fp = fopen("./AutopatchOption.opt","rb");

	if( fp == NULL )
	{
		return FALSE;
	}

	BOOL bWindowMode = FALSE;

	fscanf( fp, "%d", &bWindowMode );
	fclose(fp);

	if( bWindowMode )
	{
		m_bWindowModeChecked = TRUE;
	}
	else
	{
		m_bWindowModeChecked = FALSE;
	}

	return TRUE;
}

BOOL CMHAutoPatchDlg::SaveOption()
{
	FILE* fp = fopen("./AutopatchOption.opt","wb");

	if( fp == NULL )
	{
		return FALSE;
	}

	fprintf( fp, "%d", m_bWindowModeChecked );
	fclose(fp);

	return TRUE;
}
#endif

#ifdef _MANUAL_PATCH_
void CMHAutoPatchDlg::Pack_N_Extract()
{
	BOOL bCorrectFolder = TRUE;

	FILE* fp = fopen("MHClient-Connect.exe","rb");
	if(fp != NULL)		fclose(fp);
	else				bCorrectFolder = FALSE;

	fp = fopen("./ini/ServerList.bin","rb");
	if(fp != NULL)		fclose(fp);
	else				bCorrectFolder = FALSE;

	if( bCorrectFolder == FALSE )
	{
		MessageBox( "Invalid Path!", g_szMBErrCap, MB_OK );
		//QuitPatchDialog( TRUE );
		SetStart();
		return;
	}
	/*/-------------
	//for农府胶付胶捞亥飘 荐悼菩摹
	fp = fopen("./MHVerInfo.ver","rt");
	if( fp != NULL )
	{
	char strVer[128] = {0,};
	fscanf( fp, "%s", strVer );
	fclose(fp);
	if( strcmp( strVer, "CCC104121901" ) != 0 )
	{
	MessageBox( "Invalid Version!", g_szMBErrCap, MB_OK );
	QuitPatchDialog( TRUE );
	return;			
	}
	}
	*/

#ifndef _FOR_CHINA_
	SetStatusText( "Extracting Files" );
#else
	SetStatusText( "正在解压文件" );
#endif

	SetProgressBar( 0 );

	//拳搁俊 弊妨林妨绊...
	ShowWindow(SW_SHOW);
	UpdateWindow();
	//

	BOOL bReturn = Extract_DownloadFile( g_szMHpatchFile );

	if( !bReturn )
	{
		//	拘绵 钱扁 角菩.			
		MessageBox( "Failed To Extract", g_szMBErrCap, MB_OK );
		//	QuitPatchDialog( TRUE );
		SetStart();
		return;
	}

	DeleteFile( g_szMHpatchFile );

	// yh
	// 颇老阑 菩虐隆茄促.
	if(g_bUsingEnginePack)
	{
		StartPack3DData();
	}
	else
	{
		bReturn = UpdateVersionInfoFile( MH_VERSION_INFO_FILE, newverFilename);

		// jsd			// resource 弃歹狼 filelist历厘
		((CMHAutoPatchApp*)AfxGetApp())->SaveFileList();
		SetReadyToStart();
	}

}
#endif

void CMHAutoPatchDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

//void CMHAutoPatchDlg::OnBnClickedWindowmode()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}

void CMHAutoPatchDlg::SetStart(void)
{
#ifdef _FOR_CLIENT_
	m_bCanStart = TRUE;
	m_txtStart.SetFontColor( RGB( 220, 220, 0 ) );
	m_btnOK.EnableWindow();

	/*
	CDC MemDC;
	MemDC.CreateCompatibleDC( m_btnOK.GetDC() );
	MemDC.SelectObject( m_hBmpPlayPress);
	m_btnOK.GetDC()->BitBlt( 0, 0, 112, 46, &MemDC, 0, 0, SRCCOPY );
	*/

#ifndef _FOR_CHINA_
	m_pStatus->MoveWindow( 4, 167, 116, 15 );
	m_rcStatus.SetRect( 4, 167, 4+116, 167+15 );
	SetStatusText( "Patch Completed" );
#else
	m_pStatus->MoveWindow( 4, 405, 116, 15 );
	m_rcStatus.SetRect( 4, 405, 4+116, 405+15 );
	SetStatusText( "补丁安装完成" );
#endif

	m_pProgressBar->SetPos( 255 );

	InvalidateRect( m_rcStatus );
	UpdateWindow();

	SaveOption();

	::SendMessage( m_sHWnd, WM_RUN_ONOK, 0, 0 );
#else

	EndDialog( TRUE );
#endif
}

//void CMHAutoPatchDlg::OnBnClickedCancel()
//{
//    QuitPatchDialog( TRUE );
//}
