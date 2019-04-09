// MHAutoPatch.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

//#include "Interface_RainFTP.h"
#include "../4dyuchiGRX_common/IExecutive.h"

#include "MHAutoPatch.h"
#include "MHAutoPatchDefines.h"
#include "MHAutoPatchDlg.h"

#include <process.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif





WIN32_FIND_DATA*		pwfd				= NULL;
int						iR;
HMODULE					g_hDllModule		= NULL;
IRainFTP*				g_pFtp				= NULL;



char MHFile[MAX_PATH];
char newverFilename[256];

char g_FTPID[256] = {0,};
char g_FTPPW[256] = {0,};
char*	g_FTPIp;
WORD*	g_FTPPort;
HANDLE	g_hSession			= NULL;

int	g_nFTPCount;
int g_nCurFTPNum;

char g_ExecutableFileName[MAX_PATH];

I4DyuchiGXExecutive* g_pExecutive = NULL;
HMODULE        g_hExecutiveHandle = NULL;

CZipArchive*		g_pZip		= NULL;





/////////////////////////////////////////////////////////////////////////////
// CMHAutoPatchApp

BEGIN_MESSAGE_MAP(CMHAutoPatchApp, CWinApp)
	//{{AFX_MSG_MAP(CMHAutoPatchApp)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMHAutoPatchApp construction

CMHAutoPatchApp::CMHAutoPatchApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMHAutoPatchApp object

CMHAutoPatchApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMHAutoPatchApp initialization

BOOL CMHAutoPatchApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.


	m_hMap = CreateFileMapping( (HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, 1024, "MHOnline Client Application" );
	if( m_hMap != NULL && GetLastError() == ERROR_ALREADY_EXISTS ) 
	{ 
		PostQuitMessage(0);
		CloseHandle( m_hMap );
		return FALSE;
	}


	/*if( strcmp( AfxGetApp()->m_lpCmdLine, "INITFILELIST" ) == 0 )
	{
		SaveFileList();
		PostQuitMessage(0);
		CloseHandle( m_hMap );
		return FALSE;
	}

	if( strcmp( AfxGetApp()->m_lpCmdLine, "aa" ) != 0 )
	{
		if( SelfUpdate() == TRUE )
		{
			PostQuitMessage(2);
			CloseHandle( m_hMap );
			return FALSE;
		}		
	} */

	CoInitialize( NULL );

	// Load Version File
	if( GetVersionInfoFile( MH_VERSION_INFO_FILE, MHFile ) == FALSE )
	{
		PostQuitMessage(1);
		CloseHandle( m_hMap );
		return FALSE;
	}

	char verHEADER[5];
	memset( verHEADER, 0, 5 );
	strncpy( verHEADER, MHFile, VERSION_HEADER_TEXT_LENGTH );
	sprintf( newverFilename, "%s%s", verHEADER, MH_VERSION_INFO_FINE_NEW );

	// Load FTP Component
	if( Initialize_Ftp() == FALSE )
	{
		CoUninitialize();
		PostQuitMessage(1);
		CloseHandle( m_hMap );
		return FALSE;
	}

	// ZipArchive
	g_pZip = new CZipArchive;
	if( g_pZip == NULL )
	{
		UnInitialize_Ftp();
		CoUninitialize();
		PostQuitMessage(1);
		CloseHandle( m_hMap );
		return FALSE;
	}

	// Dialog
	CMHAutoPatchDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

#ifndef _MANUAL_PATCH_	
	DeleteFile( g_szMHpatchFile );
#endif

	if( g_pZip )
		delete g_pZip;

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	UnInitialize_Ftp();
	CoUninitialize();
	//	CloseHandle( hMap );


	//if( nResponse == 11 )	//new autopatch
	//{
	//	_execl("MHExecuter.exe","a",NULL);
	//}

	return TRUE;
}

BOOL CMHAutoPatchApp::SaveFileList()
{
	FILE* fpt = fopen("mht.dll","rb");
	if(fpt)
	{
		fclose(fpt);
		return TRUE;
	}

	DWORD count = 1;
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	SYSTEMTIME time;
	char Dir[MAX_PATH];
	char filename[MAX_PATH];
	BOOL bResult = TRUE;

	GetWindowsDirectory( Dir, MAX_PATH );
	strcat( Dir, "\\mfl.dat" );
	FILE* fp = fopen( Dir, "w" );
	if( fp == NULL )
	{
		return FALSE;
	}

	GetCurrentDirectory( MAX_PATH, filename );
	strcat( filename, "\\Resource\\*.bin" );
	hSrch = FindFirstFile( filename, &wfd );
	while( bResult )
	{
		FileTimeToSystemTime( &wfd.ftLastWriteTime, &time );
		fprintf( fp, "%d %s\t", count, wfd.cFileName );
		fprintf( fp, "%4d %02d %02d %02d %02d\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute );
		bResult = FindNextFile( hSrch, &wfd );
		++count;
	}

	bResult = TRUE;
	GetCurrentDirectory( MAX_PATH, filename );
	strcat( filename, "\\Resource\\SkillArea\\*.bsad" );
	hSrch = FindFirstFile( filename, &wfd );
	while( bResult )
	{
		FileTimeToSystemTime( &wfd.ftLastWriteTime, &time );
		fprintf( fp, "%d %s\t", count, wfd.cFileName );
		fprintf( fp, "%4d %02d %02d %02d %02d\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute );
		bResult = FindNextFile( hSrch, &wfd );
		++count;
	}

	fclose( fp );

	return TRUE;
}


BOOL CMHAutoPatchApp::SelfUpdate()
{
	HANDLE			hFind;
	WIN32_FIND_DATA wfd;
	BOOL			bReturn;

	hFind = FindFirstFile( SELF_UPDATE_FILE, &wfd);
	if( hFind == INVALID_HANDLE_VALUE )
		return FALSE;


	STARTUPINFO	sp;

	memset( &sp, 0, sizeof(STARTUPINFO));
	sp.cb = sizeof(STARTUPINFO);

	sp.dwFlags = STARTF_USEPOSITION | STARTF_USESIZE | STARTF_USESTDHANDLES; //STARTF_USESHOWWINDOW 
	sp.wShowWindow = SW_SHOW; 

	PROCESS_INFORMATION pi;


	bReturn = CreateProcess( SELF_UPDATE_FILE,		// name of executable module
		NULL,                      // command line string
		NULL, // SD
		NULL,  // SD
		FALSE,                      // handle inheritance option
		CREATE_DEFAULT_ERROR_MODE | NORMAL_PRIORITY_CLASS,                     // creation flags
		NULL,                      // new environment block
		NULL,                // current directory name
		&sp,               // startup information
		&pi // process information
		);

	if( !bReturn )
	{
		MessageBox( NULL, "SelfUpdate Failed!(Critical Error)", g_szMBErrCap, MB_OK );
		return TRUE;
	}

	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );

	return TRUE;
}

BOOL CMHAutoPatchApp::GetVersionInfoFile( char* lpszVerInfoFile, char* lpszVerInfo)
{
	HANDLE	hFile;
	DWORD	dwBytesRead;
	DWORD	dwReturn;

	hFile = CreateFile( lpszVerInfoFile,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL );

	if( hFile == INVALID_HANDLE_VALUE )
	{
		MessageBox( NULL, "Can't Open Version File", g_szMBErrCap, MB_OK );
		return FALSE;
	}

	dwReturn = ReadFile( hFile,
		lpszVerInfo,
		VERSION_TOTAL_LENGTH,
		&dwBytesRead,
		NULL );

	if( !dwReturn || dwBytesRead < VERSION_TOTAL_LENGTH )
	{
		MessageBox( NULL, "Version File is Incorrect", g_szMBErrCap, MB_OK );
		return FALSE;
	}

	CloseHandle( hFile );

	memcpy( lpszVerInfo + VERSION_TOTAL_LENGTH, ".zip", 4);

	return TRUE;
}

BOOL CMHAutoPatchApp::GetFtpInfoFile( char* filename )
{
	srand(GetTickCount());

	FILE* fp = fopen( filename, "r" );
	if( fp == NULL )
		return FALSE;

	fscanf( fp, "%d", &g_nFTPCount );
	if( g_nFTPCount < 0 ) return FALSE;

	g_FTPIp		= new char[g_nFTPCount*URL_LENGTH];
	ZeroMemory( g_FTPIp, g_nFTPCount*URL_LENGTH );
	g_FTPPort	= new WORD[g_nFTPCount];

	for( int i = 0 ; i < g_nFTPCount ; ++i )
	{
		fscanf( fp, "%s %d", &g_FTPIp[i*URL_LENGTH], &g_FTPPort[i] );
	}

	fscanf( fp, "%s", &g_ExecutableFileName );
	fclose( fp );

	g_nCurFTPNum = rand()%g_nFTPCount;

	return TRUE;
}

BOOL CMHAutoPatchApp::Initialize_Ftp()
{
	HRESULT	hr;

	FILE* fp = fopen("FTPAccount.ini","r");
	if(fp)
	{
		fscanf(fp,"%s",g_FTPID);
		fscanf(fp,"%s",g_FTPPW);
		fclose(fp);
	}
	else
	{
		sprintf(g_FTPID,FTP_USERID);
		sprintf(g_FTPPW,FTP_PASSWORD);	
	}

	pwfd = new WIN32_FIND_DATA[ FIND_DATA_MAX];
	if( pwfd == NULL )
		return FALSE;

	if( GetFtpInfoFile( FTP_INFO_FILE ) == FALSE)
	{
		MessageBox( NULL, "Failed To Load FTPInfo File", g_szMBErrCap, MB_OK );
		return FALSE;
	}

	//	
	DllGetClassObject_RainFTP pfLoader;

	g_hDllModule = LoadLibrary( "RainFTP.dll");
	if( g_hDllModule == NULL )
	{
		MessageBox( NULL, "Failed to Load - RainFTP.dll", g_szMBErrCap, MB_OK );
		return FALSE;
	}

	pfLoader = (DllGetClassObject_RainFTP) GetProcAddress( g_hDllModule, "DllGetClassObject");
	hr = (*pfLoader)( CLSID_RainFTP, IID_RainFTP, (void**)&g_pFtp);
	if ( FAILED(hr) )
	{
		MessageBox( NULL, "No Matching CLSID or IID", g_szMBErrCap, MB_OK );
		return FALSE;
	}

	return TRUE;
}

void CMHAutoPatchApp::UnInitialize_Ftp()
{
	if( g_pFtp )
	{
		if( g_hSession)
			g_pFtp->CloseSession( g_hSession);

		g_pFtp->Release();
		g_pFtp = NULL;
	}

	FreeLibrary( g_hDllModule);

	if( g_pExecutive )
	{
		g_pExecutive->Release();
		g_pExecutive = NULL;
	}
	if( g_hExecutiveHandle )
	{
		FreeLibrary(g_hExecutiveHandle);
	}	
	delete [] pwfd;
}
