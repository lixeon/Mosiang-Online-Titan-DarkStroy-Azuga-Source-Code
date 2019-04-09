// test.cpp : Defines the entry point for the application.
//


#include "stdafx.h"
#include <crtdbg.h>

//#define _NO_NPROTECT_START

#include "MainGame.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cEditBox.h"
#include "./Interface/cTextArea.h"
#include "./Interface/cFont.h"
#include "cMsgBox.h"

#include "./Input/UserInput.h"
#include "./Input/cIMEWnd.h"

#include "ChatManager.h"
#include "../[CC]Header/GameResourceManager.h"
#include "MouseCursor.h"

#include "mhFile.h"

#include "OptionManager.h"
#include "MHAudioManager.h"
#include "ObjectManager.h"
#include "MHMap.h"

#include "ExitManager.h"
#include "QuestManager.h"

#include "ServerConnect.h"
#include <vfw.h>

#ifdef _GMTOOL_
#include "GMToolManager.h"
#include "CommCtrl.h"
#endif

#ifdef TAIWAN_LOCAL
#include <shellapi.h>
#endif

#ifdef _HACK_SHIELD_
#include "HShield.h"
#include "HackShieldManager.h"
#include <dbghelp.h>
#endif
#ifdef _NPROTECT_
// nProtect
//#define NO_GAMEGUARD
#include "NPGameLib.h"
#include "NProtectManager.h"
#include <dbghelp.h>
DWORD g_dwHU;
#endif

#include "cScriptManager.h"


#define _LOGOWINDOW_

//////////////////////////////////////////////////////////////////////////
// Version ½ÇÇàÆÄÀÏ ¹öÁ¯
// ÀÌ°É ¹Ù²Ù¸é ¼­¹öÅø(¸ð´ÏÅÍ¸µÅø)¿¡¼­µµ ¹öÁ¯À» ¹Ù²ãÁà¾ßÇÑ´Ù.
/////////////////////////////////////////////////////////////////////////

//#define _MANUALPATCH_
#ifdef _CHINA_LOCAL_
char g_CLIENTVERSION[32] = "OPEN06041301";
#elif defined _JP_LOCAL_
char g_CLIENTVERSION[32] = "JOPN06032901";
#elif defined _HK_LOCAL_

#ifdef _TW_LOCAL_
char g_CLIENTVERSION[256] = "TWCV06112701";
#else
char g_CLIENTVERSION[256] = "HKCV07020501";
#endif

#elif defined _TL_LOCAL_
char g_CLIENTVERSION[32] = "TLCV06051001";

#else




char g_CLIENTVERSION[32] = "NDSC08070301";

#endif

/////////////////////////////////////////////////////////////////////////	

#define MAX_LOADSTRING 100

// Global Variables:
HANDLE g_hMap = NULL;
HINSTANCE g_hInst;								// current instance
HWND _g_hWnd;
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text
BOOL g_bActiveApp = FALSE;
char g_DistributeAddr[16];
WORD g_DistributePort;
char g_AgentAddr[16];
WORD g_AgentPort;
char g_PATCHVERSION[256];
char g_szHeroIDName[17];
int	g_nServerSetNum;

HIMC g_hIMC = NULL;




///for test //KES
//DEVMODE g_CurDisplayMode;
//DEVMODE g_GameDisplayMode;
//BOOL	g_bWindowMode = FALSE;
/////

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void LeaveClientApplication();
BOOL EnterClientApplication();

//BOOL CheckParameter(LPSTR parameter);
BOOL	GetPatchVersion();
BOOL	CheckResourceFile();
void	ChangeVerInfo();
BOOL	SaveFileList();

#ifdef _HACK_SHIELD_
BOOL HS_Init();
BOOL HS_StartService();
BOOL HS_StopService();
BOOL HS_UnInit();
int __stdcall HS_CallbackProc (long lCode, long lParamSize, void* pParam );
BOOL HS_PauseService();
BOOL HS_ResumeService();
void HS_SaveFuncAddress();

LONG WINAPI TopLevelFilter(struct _EXCEPTION_POINTERS* pExPtr)
{
	HS_StopService();
	HS_UnInit();
	return EXCEPTION_EXECUTE_HANDLER;
}

#endif

#ifdef _NPROTECT_

BOOL CALLBACK NPGameMonCallback(DWORD dwMsg, DWORD dwArg);

LONG WINAPI TopLevelFilter(struct _EXCEPTION_POINTERS* pExPtr)
{
	NPROTECTMGR->Release();
	return EXCEPTION_EXECUTE_HANDLER;
}

#endif

void ClearDirectory( LPCTSTR pStrName )
{
	WIN32_FIND_DATA FindFileData;
	char buf[MAX_PATH];
	wsprintf( buf, "%s\\*", pStrName );

	HANDLE hFind = FindFirstFile( buf, &FindFileData);

	if( hFind == INVALID_HANDLE_VALUE ) return;

	while( 1 )
	{
		if( FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY )
		{
			if( strcmp( FindFileData.cFileName, "." ) != 0 &&
				strcmp( FindFileData.cFileName, ".." ) != 0 )
			{
				wsprintf( buf, "%s\\%s", pStrName, FindFileData.cFileName );
				ClearDirectory( buf );
				RemoveDirectory( buf );
			}
		}
		else
		{
			wsprintf( buf, "%s\\%s", pStrName, FindFileData.cFileName );
			DeleteFile( buf );
		}
		if( FindNextFile( hFind, &FindFileData ) == FALSE )
			break;
	}
	FindClose( hFind );	
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{

	FILE* fp = NULL;
#ifdef _SEMIRELEASE
	if( strcmp( lpCmdLine, "anrgideoqcn" ) != 0 
#ifdef _MANUALPATCH_
		&& strcmp( lpCmdLine, "anrgiddlatl" ) != 0 
#endif
		)
	{
		MessageBox( NULL, "You should not execute directly this file!!", "Error!!", MB_OK );
		return 0;
	}
/*	
	fp = fopen("mht.dll","r");
	if( fp )
		fclose(fp);
	else
	{
		if( !CheckResourceFile() )
		{
			ChangeVerInfo();
			MessageBox( NULL, "Your version of files is not valid!!\nTry to reconnect!!", "Error!!", MB_OK );
			return 0;
		}
	}
*/
#endif

	if(!EnterClientApplication()) return 0;


#ifdef _HACK_SHIELD_

	DIRECTORYMGR->Init();

	if( !HS_Init() )
	{
		HS_UnInit();
		MessageBox( NULL, "HackShield Init Failed.", "ERROR", MB_OK );	
		return 0;
	}
	if( !HS_StartService() )
	{
		HS_StopService();
		HS_UnInit();
		MessageBox( NULL, "HackShield Init Failed.", "ERROR", MB_OK );
		return 0;
	}
	HS_SaveFuncAddress();

	::SetUnhandledExceptionFilter(TopLevelFilter);

#endif

	// 임시... 클라이언트에 서버 패치 폴더가..???
	ClearDirectory("08061202_Server");
	RemoveDirectory("08061202_Server");
	//////////////////////////////////////////////////////////////////////////

	DeleteFile("./Log/Debug.txt");

#ifdef _HK_LOCAL_	//홍콩에서 클라이언트에 serverset을 패치했다~ OTL
#ifndef _TW_LOCAL_
	DeleteFile("ServerSet/4/ChannelInfo.bin");
	DeleteFile("ServerSet/4/DistributeDesc.txt");
	DeleteFile("ServerSet/4/MapServerDesc.txt");
	DeleteFile("ServerSet/4/mhodbc.ini");
	DeleteFile("ServerSet/4/serverlist.msl");
	RemoveDirectory("ServerSet/4");
	RemoveDirectory("ServerSet");

	ClearDirectory("User");
	RemoveDirectory("User");
#else
	DeleteFile("GMTOOL_TW_TESTSERVER.exe");
#endif
#endif
#ifdef _TL_LOCAL_	//태국에서도 클라이언트에 serverset을 패치했다~ OTL
	RemoveDirectory("ServerSet");
	DeleteFile("AgentServer.exe");
#endif

	CreateDirectory("Log",NULL);

	LOGFILE("///////////////////////////////////////////////////////");
	LOGFILE("///////////////////  Client Start  ////////////////////");
	LOGFILE("///////////////////////////////////////////////////////");
	
	//_CrtSetBreakAlloc(2940);
 	// TODO: Place code here.
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_DELAY_FREE_MEM_DF);
	//_CrtMemDumpAllObjectsSince(0);
	
	fp = fopen("../DeveloperWorking/_DONOTUSEENGINEPACK","r");
	if(fp)
	{
		g_bUsingEnginePack = FALSE;
		fclose(fp);
	}
 
	// Initialize global strings
	GetPatchVersion();

	GAMERESRCMNGR->LoadGameDesc();

#ifndef _HK_LOCAL_

#ifdef _TESTCLIENT_
	sprintf(szTitle,"DarkStory(TEST) %s %s ExeVer:%s PatchVer:%s",__DATE__,__TIME__,g_CLIENTVERSION,g_PATCHVERSION);
	sprintf(szWindowClass,"DarkStory(TEST) %s %s ExeVer:%s PatchVer:%s",__DATE__,__TIME__, g_CLIENTVERSION,g_PATCHVERSION);
#elif _GMTOOL_
	sprintf(szTitle,"Sky Online  [GM_TOOL] %s %s ExeVer:%s PatchVer:%s",__DATE__,__TIME__,g_CLIENTVERSION,g_PATCHVERSION);
	sprintf(szWindowClass,"Sky Online [GM_TOOL] %s %s ExeVer:%s PatchVer:%s",__DATE__,__TIME__, g_CLIENTVERSION,g_PATCHVERSION);
#else
//	sprintf(szTitle,"DarkStoryOnline");
//	sprintf(szWindowClass,"DarkStoryOnline");
	sprintf(szTitle,"Sky Online");
    sprintf(szWindowClass,"Sky Online");
#endif

#else	//_HK_LOCAL

#ifdef _TESTCLIENT_
	sprintf(szTitle,"DarkStory(TEST) %s %s ExeVer:%s PatchVer:%s",__DATE__,__TIME__,g_CLIENTVERSION,g_PATCHVERSION);
	sprintf(szWindowClass,"DarkStory(TEST) %s %s ExeVer:%s PatchVer:%s",__DATE__,__TIME__, g_CLIENTVERSION,g_PATCHVERSION);
#elif _GMTOOL_
	sprintf(szTitle,"%s [GM_TOOL] %s %s ExeVer:%s PatchVer:%s", GAMERESRCMNGR->m_GameDesc.strWindowTitle,__DATE__,__TIME__,g_CLIENTVERSION,g_PATCHVERSION);
	sprintf(szWindowClass,"%s [GM_TOOL] %s %s ExeVer:%s PatchVer:%s", GAMERESRCMNGR->m_GameDesc.strWindowTitle,__DATE__,__TIME__, g_CLIENTVERSION,g_PATCHVERSION);
#else
	sprintf(szTitle, GAMERESRCMNGR->m_GameDesc.strWindowTitle);
	sprintf(szWindowClass, GAMERESRCMNGR->m_GameDesc.strWindowTitle);
#endif

#endif	//_HK_LOCAL


	MyRegisterClass(hInstance);
	//LOGEX(lpCmdLine, PT_MESSAGEBOX);

	GAMERESRCMNGR->LoadDeveloperInfo();
	// quest data load
//	QUESTMGR->LoadQuestString( "./Quest/QuestString.bin" );
//	QUESTMGR->LoadQuestScript( "./Quest/QuestScript.bin" );
//	QUESTMGR->LoadNewbieGuide( "./Quest/NewbieGuide.bin" );
//	QUESTMGR->LoadQuestNpc( "./Quest/QuestNpcList.bin" );
	QUESTMGR->LoadQuestString( "./Resource/QuestScript/QuestString.bin" );
	QUESTMGR->LoadQuestScript( "./Resource/QuestScript/QuestScript.bin" );
	QUESTMGR->LoadNewbieGuide( "./Resource/QuestScript/NewbieGuide.bin" );
	QUESTMGR->LoadQuestNpc( "./Resource/QuestScript/QuestNpcList.bin" );
	//
	SCRIPTMGR->InitScriptManager();

//for test	//KES
//	EnumDisplaySettings( NULL, ENUM_CURRENT_SETTINGS, &g_CurDisplayMode );
//	g_GameDisplayMode = g_CurDisplayMode;
//	g_GameDisplayMode.dmBitsPerPel	= GAMERESRCMNGR->m_GameDesc.dispInfo.dwBPS*8;
//	g_GameDisplayMode.dmPelsWidth	= GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth;
//	g_GameDisplayMode.dmPelsHeight	= GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight;
//	g_bWindowMode	= GAMERESRCMNGR->m_GameDesc.dispInfo.bWindowed;

//	g_CurDisplayMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
//	g_GameDisplayMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
///////////////

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		LeaveClientApplication();
		return FALSE;
	}

//	HACCEL hAccelTable;
//	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_TEST);

	// Main message loop:
	MSG msg;
	while (1) //GetMessage(&msg, NULL, NULL, NULL));
    {
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
//			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
//			{
#ifdef _GMTOOL_
			if( !GMTOOLMGR->IsGMDialogMessage(&msg) )
#endif
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg); 
			}
//			}
		}
		else
		{
			if(MAINGAME->Process() == -1)
				break;
		}
	}

//	DeleteFile("recv.txt");

	LeaveClientApplication();

//	_ASSERTE( _CrtCheckMemory( ) );

//#ifdef TAIWAN_LOCAL
//	ShellExecute( NULL, NULL, "Iexplore.exe", "http://www.moxiang.com.cn", NULL, SW_SHOWNORMAL );
//#endif

#ifdef _HACK_SHIELD_
	HS_StopService();
	HS_UnInit();
#endif
#ifdef _NPROTECT_	
	NPROTECTMGR->Release();
#endif
	return msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 
#ifdef _SEMIRELEASE
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS | CS_NOCLOSE;
#else
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
#endif
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
//	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_TEST);
//	wcex.hIcon			= NULL;

#ifdef _KOR_LOCAL_ 
	wcex.hIcon			= LoadIcon( hInstance, MAKEINTRESOURCE(IDI_TAITANICON) );
#else
	wcex.hIcon			= LoadIcon( hInstance, MAKEINTRESOURCE(IDI_GAMEICON) );
#endif	
	

	wcex.hCursor		= NULL;
//	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
//	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);
	wcex.hIconSm		= NULL;

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	//if(!EnterClientApplication()) return FALSE;	//A¨Ï©ª=8¢®¢´I ¡Ë?A¢®¨¡a¡Ë¡ÍU. //KES

	CURSOR->Init( hInstance );
	
	HWND hWnd;
	g_hInst = hInstance; // Store instance handle in our global variable
//	int x = GetSystemMetrics(SM_CXSIZEFRAME)*2;
	int x = GetSystemMetrics(SM_CXFIXEDFRAME)*2;
//	int y = GetSystemMetrics(SM_CYSIZEFRAME)*2+GetSystemMetrics(SM_CYCAPTION);
	int y = GetSystemMetrics(SM_CXFIXEDFRAME)*2+GetSystemMetrics(SM_CYCAPTION);
//#ifdef _GMTOOL_
//	y += GetSystemMetrics(SM_CYMENU);
//#endif
#ifdef _SEMIRELEASE
	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_CLIPCHILDREN |
		WS_SYSMENU/* | WS_THICKFRAME*/;
#else
	DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_CLIPCHILDREN |
		WS_SYSMENU |
		/*WS_THICKFRAME |*/ WS_MINIMIZEBOX;
#endif

	if( GAMERESRCMNGR->IsLowResolution() )
	{
		_g_hWnd = hWnd = CreateWindow(szWindowClass, szTitle, dwStyle, //WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, 800+x, 600+y, NULL, NULL, hInstance, NULL);	//WINDOW SIZE
	}
	else
	{
		_g_hWnd = hWnd = CreateWindow(szWindowClass, szTitle, dwStyle, //WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, 1024+x, 768+y, NULL, NULL, hInstance, NULL);	//WINDOW SIZE
	}

	
	if (!hWnd)
	{
		return FALSE;
	}
#ifndef _NO_NPROTECT_START
#ifdef _NPROTECT_
	if(!NPROTECTMGR->Init(_g_hWnd))
	{
		return FALSE;
	}

	::SetUnhandledExceptionFilter(TopLevelFilter);
#endif
#endif

//	gMp3 = new CBGMLoader(hInstance,hWnd);

	ShowWindow(hWnd, nCmdShow);
	
//	if( g_bWindowMode )
//		ShowWindow(hWnd, nCmdShow);
//	else
//		ShowWindow(hWnd, SW_HIDE);

	UpdateWindow(hWnd);
	ShowCursor(TRUE);

	CoInitialize(NULL);

#ifndef _LOGOWINDOW_
	PlayCutscene("logo.avi",g_hInst);
#endif

	//////////////////////////////////////////////////////////////////////////
	// MAININIT
	MAINGAME->Init(hWnd);
		
#ifndef _TESTCLIENT_
	// magi82 - Intro(070713)
	if( OPTIONMGR->GetGameOption()->bIntroFlag == FALSE )
        MAINGAME->SetGameState(eGAMESTATE_INTRO);
	else
		MAINGAME->SetGameState(eGAMESTATE_CONNECT);
#else
	int initKind = eGameInInitKind_Login;
	MAINGAME->SetGameState(eGAMESTATE_GAMEIN,&initKind,sizeof(int));
#endif                 

#ifdef _GMTOOL_
		GMTOOLMGR->CreateGMDialog();
#endif
	
	
	return TRUE;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MOUSE->GetWindowMessage(hWnd, message, wParam, lParam);
	CHATMGR->MsgProc( message, wParam );//¨ÏoY¡Íia¡§oA CIMEWND->OnMsg()¡§¡þ¡Ë¡þ¡Ë¡ÍU ¡Ë¡þOAu ¡§oCCa¡ÍiC¡§ui¡§u¨Ï¡þCN¡Ë¡ÍU. A¡Ë¢¥¡§¢®A¡Ë?¡Ë¢ç¡Íi¨Ï¡Ì¡§¢®¡Ëc¨ÏoU¡§i©ö¡Ëi¨ù¡Ë?¡Ë¢ç ¡§¢®¢®AA¡Ë?¡§i©ö¡Ëi¨ù¡Ë¡þ| ¡Ë¡þOAu AO¢®¨úa 'C¡§¨£¡§u¢®¨Ï.
	CIMEWND->OnMsg(hWnd, message, wParam, lParam);
	
	switch (message) 
	{
	case WM_CHAR:
	case WM_IME_CHAR:
		return 0;
//	case WM_IME_COMPOSITION:
#ifndef TAIWAN_LOCAL
#ifndef _HK_LOCAL_
	case WM_IME_STARTCOMPOSITION:
		{
#ifdef _JAPAN_LOCAL_

			if( WINDOWMGR->GetFocusedEdit() )
			{
				HIMC hIMC = ImmGetContext( _g_hWnd );

				LONG X = 0, Y = 0;

				if( WINDOWMGR->GetFocusedEdit()->GetType() == WT_EDITBOX ||
					WINDOWMGR->GetFocusedEdit()->GetType() == WT_SPIN )
				{
					((cEditBox*)WINDOWMGR->GetFocusedEdit())->GetCaretPos( &X, &Y );
				}
				else if( WINDOWMGR->GetFocusedEdit()->GetType() == WT_TEXTAREA )
				{
					((cTextArea*)WINDOWMGR->GetFocusedEdit())->GetCaretPos( &X, &Y );
				}


				ImmSetCompositionFont( hIMC,
					&CFONT_OBJ->m_LogFont[WINDOWMGR->GetFocusedEdit()->GetFontIdx()] );

				COMPOSITIONFORM cf;
				cf.dwStyle = CFS_RECT;
				cf.ptCurrentPos.x = X;
				cf.ptCurrentPos.y = Y;
				cf.rcArea.left = X;
				cf.rcArea.top = Y;
				cf.rcArea.right = X+512;
				cf.rcArea.bottom = Y+20;
				ImmSetCompositionWindow( hIMC, &cf );


				ImmReleaseContext( _g_hWnd, hIMC );
			}

			break;

#endif
			return 0;
		}
#endif
#endif

	case WM_ACTIVATEAPP:
		{
			g_bActiveApp = (BOOL)wParam;

			if( !g_bActiveApp )
			{
				AUDIOMGR->StopAll();
				AUDIOMGR->StopBGM();

				CIMEWND->SetCtrlPushed( FALSE );

#ifdef _GMTOOL_
				
				
#endif

	//KES T_T ¡§uE¡ÍiC¨Ï©ª¢®¢¯.
	//			if( !g_bWindowMode )
	//			{
	//				ShowWindow( _g_hWnd, SW_MINIMIZE );
	//				MAP->Invalidate();
	//				ChangeDisplaySettings( &g_CurDisplayMode, 0 );
	//			}
			}
			else
			{
				if( OPTIONMGR->GetGameOption()->bSoundBGM )
					AUDIOMGR->PlayLastBGM();

#ifdef _GMTOOL_
				
#endif

	//			if( !g_bWindowMode )
	//			{
	//				ShowWindow( _g_hWnd, SW_NORMAL );				
	//				ChangeDisplaySettings( &g_GameDisplayMode, 0 );				
	//			}
			}
		}
		break;

	case WM_ACTIVATE:
		
		if( g_UserInput.GetKeyboard()->GetDirectInput() )
		{
			if( WA_INACTIVE != wParam )
			{
				// Make sure the device is acquired, if we are gaining focus.
				g_UserInput.GetKeyboard()->ClearKeyState();
				g_UserInput.GetKeyboard()->UpdateKeyboardState();
				//g_UserInput.SetInputFocus( TRUE );
				OBJECTMGR->SetAllNameShow( FALSE );
			}
//			else
//			{
				//g_UserInput.SetInputFocus( FALSE );
//			}
		}
		break;

	case WM_USER+155:
		{
			ISC_BaseNetwork* lpNet = NETWORK->GetBaseNetwork();
			if(lpNet)
				lpNet->SocketEventProc();
		}
		return TRUE;		
		
	case WM_CLOSE:
		{
			MAINGAME->SetGameState( eGAMESTATE_END );
			/*
			if( MAINGAME->GetCurStateNum() == eGAMESTATE_TITLE )
			{
			MAINGAME->SetGameState( eGAMESTATE_END );
			break;		// ¢®¨¡OAO E¡ËcAI ¡§uE¢®¨¡i ~¢®¢´a
			}
			else if( MAINGAME->GetCurStateNum() == eGAMESTATE_CHARSELECT ||
			MAINGAME->GetCurStateNum() == eGAMESTATE_CHARMAKE ||
			MAINGAME->GetCurStateNum() == eGAMESTATE_GAMEIN )
			{
			WINDOWMGR->MsgBox( MBI_EXIT, MBT_YESNO, CHATMGR->GetChatMsg( 280 ) );
			return 0;
			}
			//		return 0;
			*/
#ifdef _NPROTECT_
			if(NPROTECTMGR->GetMsg())
			{
				MessageBox(hWnd, NPROTECTMGR->GetMsg(), "nProtect GameGuard", MB_OK);
			}
#endif
		}
		break;

	case WM_SETFOCUS:
		{ 
			CURSOR->Process();
		}
		return 0;

//KES 030826
	case WM_SETCURSOR:
		{ 
			CURSOR->Process();			
		}
		break;	//DefWindowProc¢®¨¡¡Ë¢ç caption'¡Ë?¡Ë¢ç¡§u¢®¨ú default cursor¢®¢´I ¨ÏoU¨Ï¡ÀaA¡§¨£¡Ë¡ÍU.
		//return TRUE;

//KES 030826

	case WM_SYSKEYDOWN:		//Alt + F4 ¨Ïo¨Ï¢®Ao
		{
			if( wParam == VK_F4 )
			{
				if( MAINGAME->GetCurStateNum() == eGAMESTATE_TITLE )
				{
					break;		// ¢®¨¡OAO E¡ËcAI ¡§uE¢®¨¡i ~¢®¢´a
				}
				else if( MAINGAME->GetCurStateNum() == eGAMESTATE_CHARSELECT ||
					MAINGAME->GetCurStateNum() == eGAMESTATE_CHARMAKE ||
					MAINGAME->GetCurStateNum() == eGAMESTATE_GAMEIN )
				{
					WINDOWMGR->MsgBox( MBI_EXIT, MBT_YESNO, CHATMGR->GetChatMsg( 280 ) );
					return 0;
				}
			}
		}
		return 0;

	case WM_SYSCOMMAND:
		{
			if( wParam == SC_KEYMENU )
				return 0;
		}
		break;

	case WM_SYSKEYUP:		//AltA¢®¨¡, F10A¢®¨¡ ¡Ë¡þ¡§¨£A¨Ï¡þ¡Ë¡ÍA¢®¨¡I ¨Ïo¨Ï¢®Ao
		{
		}
		return 0;
/*
	case MCIWNDM_NOTIFYMODE:
		switch(lParam)
		{
		case MCI_MODE_PLAY:
			//MessageBox( hWnd, "Play", "MCI", MB_OK );
			break;
		case MCI_MODE_STOP:
			//MessageBox( hWnd, "Stop", "MCI", MB_OK );
			SERVERCONNECT->EndShowLogoAVI();
			break;
		}
		return 0;
*/
#ifdef _GMTOOL_
	case WM_MOVE:
		//xPos = (int)(short) LOWORD(lParam);   // horizontal position 
		//yPos = (int)(short) HIWORD(lParam);   // vertical position
		GMTOOLMGR->SetPositionByMainWindow();
		break;
#endif

	case WM_CREATE:
		{
#ifdef _GMTOOL_
			INITCOMMONCONTROLSEX icex;
			icex.dwSize = sizeof( icex );
			icex.dwICC	= ICC_DATE_CLASSES;
			InitCommonControlsEx( &icex );
#endif
/*
#ifdef TAIWAN_LOCAL
			HWND hWndIme = ImmGetDefaultIMEWnd( hWnd );
			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
			//HIMC hImc = ImmGetContext( hWnd );
			//DWORD dwConversion, dwSentence;
			//ImmGetConversionStatus( hImc, &dwConversion, &dwSentence );
			//dwConversion |= IME_CMODE_NOCONVERSION;
			//dwSentence |= IME_SMODE_SINGLECONVERT;
			//ImmSetConversionStatus( hImc, dwConversion, dwSentence );
			//ImmReleaseContext( hWnd, hImc );
#endif
*/
#ifdef _JAPAN_LOCAL_
//			HWND hWndIme = ImmGetDefaultIMEWnd( hWnd );
//			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
			HIMC hImc = ImmGetContext( hWnd );
			DWORD dwConversion, dwSentence;
			ImmGetConversionStatus( hImc, &dwConversion, &dwSentence );
			//dwConversion |= IME_CMODE_NOCONVERSION;			
			dwSentence |= IME_SMODE_SINGLECONVERT;
			//dwSentence |= IME_SMODE_CONVERSATION;
			//ImmSetConversionStatus( hImc, dwConversion, dwSentence );
			ImmSetConversionStatus( hImc, dwConversion, dwSentence );
			ImmReleaseContext( hWnd, hImc );
/*
			HIMC hIMC = ImmGetContext( _g_hWnd );
			ImmSetOpenStatus( hIMC, FALSE );
			ImmNotifyIME( hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0 );
			ImmNotifyIME( hIMC, NI_CLOSECANDIDATE, 0, 0 );
			ImmReleaseContext( _g_hWnd, hIMC );

			HWND hWndIme = ImmGetDefaultIMEWnd( _g_hWnd );
			SendMessage( hWndIme, WM_IME_CONTROL, IMC_CLOSESTATUSWINDOW, 0 );
*/
#endif
		}
		break;
	}
	
	return DefWindowProc(hWnd, message, wParam, lParam);
}



// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}


BOOL GetPatchVersion()
{
	CMHFile file;
	if(!file.Init("MHVerInfo.ver", "r"))
		return FALSE;
	strcpy(g_PATCHVERSION, file.GetString());

	return TRUE;
}

/*
BOOL CheckParameter(LPSTR parameter)
{
	char * token = strtok(parameter, " ");
	if(token == NULL) return FALSE;
	strcpy(g_DistributeAddr, token);
	token = strtok(NULL, " ");
	if(token == NULL) return FALSE;
	g_DistributePort = atoi(token);
	token = strtok(NULL, " ");
	if(token == NULL) return TRUE;
	GAMERESRCMNGR->m_GameDesc.dispInfo.bWindowed = atoi(token);
	
	return TRUE;
}
*/

BOOL EnterClientApplication()
{
//taiyo
    g_hMap = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, 1024, "MHOnline Client Application");
    if(g_hMap != NULL && GetLastError() == ERROR_ALREADY_EXISTS) 
    { 
		CloseHandle(g_hMap); 
		g_hMap = NULL;
		return FALSE;
    }

/*
	HANDLE hMutex = CreateMutex( NULL, FALSE, "MHOnline Client Application" );	<--- AI¢®¢´¡Ë¡þ¢®¨¡O CI¡Ë¡ÍA¢®¨¡IAI ¡Ë¡Ío ¢®¨¡¢®I¡Ë¡ÍUCIAo ¡§uE:¢®¨¡¡Ë¢ç? KES

	if( GetLastError() == ERROR_ALREADY_EXISTS )
		return FALSE;
*/

	
	return TRUE;
}


void LeaveClientApplication()
{//taiyo
	if(g_hMap)
	{
		CloseHandle(g_hMap);
		g_hMap = NULL;
	}	
}


//pjslocal
#ifdef TAIWAN_LOCAL
int is_big5(BYTE *str)
{
	if( str[0] & 0x80 )
	{
		if ( str[0] == 0xa2)
		{
			if( str[1]>= 0xaf && str[1]<= 0xb8 )
				return 1;
			else if( str[1] >= 0xcf && str[1] <= 0xff )
				return 1;
		} 
		if ( str[0] == 0xa3)
		{
			if( str[1]>=0x43 && str[1]<=0xff )
				return 1;
		}

		if ( str[0] == 0xa4)
		{
			if( str[1] >= 0x40 && str[1] <= 0xff )
				return 1;
		}

		if ( str[0] >= 0xa5 && str[0] <= 0xc5 )
			return 1;

		if ( str[0] == 0xc6 )
		{
			if( str[1] >= 0x00 && str[1] <= 0x7e )
				return 1;
		}
	}
	else
	{
		return 1;
	}
	return 0;
}
/*
#else
//////////////
int is_hangul(BYTE *str)
{
	if( str[0] & 0x80)// 1000 0000 //b0 = 1011 0000//c8=1100 1000// a1=1010 0001 // fe=1111 1110 //
	{
		if (str[0] >= 0xb0 && str[0] <= 0xc8 && str[1] >= 0xa1 && str[1] <= 0xfe)  
			return 1;
	}
	else
		return 1;

	return 0;
}
*/
#endif

////////////////////////////////////////////////////////////////////////////////////////
// resource ¡§¢®u¡Ë¡Ío¡§uEAC ¡§¢®AAI ¢®¨¡E¢®ic(jsd)
///////////////////////////////////////////////////////////////////////////////////////
BOOL CheckResourceFile()
{
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	SYSTEMTIME time;
	char Dir[MAX_PATH];
	char filename[MAX_PATH];
	BOOL bResult = TRUE;
	CMHFile file;
	char temp[256];
	WORD year, month, day, hour, min;	

	GetWindowsDirectory( Dir, MAX_PATH );
	strcat( Dir, "\\mfl.dat" );
		
	GetCurrentDirectory( MAX_PATH, filename );
	strcat( filename, "\\Resource\\*.bin" );
	hSrch = FindFirstFile( filename, &wfd );
	while( bResult )
	{
		FileTimeToSystemTime( &wfd.ftLastWriteTime, &time );

		///
		if( !file.Init( Dir, "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
		{
			file.Release();
			goto filesave;
		}
		while( !file.IsEOF() )
		{
			file.GetString( temp );
			if( strcmp( wfd.cFileName, temp ) == 0 )
			{
				year = file.GetWord();
				month = file.GetWord();
				day = file.GetWord();
				hour = file.GetWord();
				min = file.GetWord();
				
				if( year != time.wYear || month != time.wMonth || day != time.wDay ||
					hour != time.wHour || min != time.wMinute )
				{
					file.Release();
					return FALSE;
				}
				else
					break;
			}
		}
		file.Release();
		///		

		bResult = FindNextFile( hSrch, &wfd );
	}

	bResult = TRUE;
	GetCurrentDirectory( MAX_PATH, filename );
	strcat( filename, "\\Resource\\SkillArea\\*.bsad" );
	hSrch = FindFirstFile( filename, &wfd );
	while( bResult )
	{
		FileTimeToSystemTime( &wfd.ftLastWriteTime, &time );

		///
		if( !file.Init( Dir, "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
		{
			file.Release();
			goto filesave;
		}
		while( !file.IsEOF() )
		{
			file.GetString( temp );
			if( strcmp( wfd.cFileName, temp ) == 0 )
			{
				year = file.GetWord();
				month = file.GetWord();
				day = file.GetWord();
				hour = file.GetWord();
				min = file.GetWord();
				
				if( year != time.wYear || month != time.wMonth || day != time.wDay ||
					hour != time.wHour || min != time.wMinute )
				{
					file.Release();
					return FALSE;
				}
				else
					break;
			}
		}
		file.Release();
		///		

		bResult = FindNextFile( hSrch, &wfd );
	}

	return TRUE;

filesave:
	SaveFileList();
	return TRUE;
}

void ChangeVerInfo()
{
	char temp[256];
	FILE* fp = fopen( "MHVerInfo.ver", "r" );
	if( fp == NULL )
		return;
	fscanf( fp, "%s", temp );
	fclose( fp );

	temp[10] = NULL;
	fp = fopen( "MHVerInfo.ver", "w" );
	if( fp == NULL )
		return;
	fprintf( fp, "%s%s", temp, "01" );
	fclose( fp );
}

BOOL SaveFileList()
{
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
		fclose( fp );
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

#ifdef _HACK_SHIELD_
BOOL HS_Init(void)
{
	int		dwRet = 0;			// _AhnHS_Initialize 함수 리턴 값

	TCHAR	szFullFilePath[MAX_PATH];	// 파일 경로
	TCHAR	szMsg[MAX_PATH];		// 메시지 버퍼
	DWORD	dwOption = 0;			// 초기화 옵션

	// 현재 게임의 실행 경로를 구합니다.
	// ※ 주의 :
	//    GetCurrentDirectory 함수가 아닌, GetModuleFileName 함수를 이용해서 현재실행중인
	//    파일의 경로를 구하시기 바랍니다.
	strcpy(  szFullFilePath, DIRECTORYMGR->GetFullDirectoryName(eLM_Root) );
	// 게임과 연동된 HackShield의 EhSvc.Dll의 경로 설정	
	lstrcat ( szFullFilePath, _T( "HShield\\EhSvc.dll" ) );

	// 초기화 옵션을 설정합니다. AHNHS_CHKOPT_로 시작하는 옵션을 모두 적용하려면

	// dwOption = AHNHS_CHKOPT_ALL 로 간단하게 정의할 수 있습니다.
	// 자세한 내용은 "프로그래밍 가이드"를 참조하세요.
	// ※ 주의 :
	//    게임 디버깅 시, 
	//    AHNHS_CHKOPT_READWRITEPROCESSMEMORY, AHNHS_CHKOPT_KDTARCER
	//    AHNHS_CHKOPT_OPENPROCESS, AHNHS_CHKOPT_MESSAGEHOOK 옵션은 제거
	//    하셔야 디버깅이 가능합니다.
	dwOption = 
#ifndef _GMTOOL_
		AHNHS_CHKOPT_READWRITEPROCESSMEMORY |
		AHNHS_CHKOPT_KDTRACER |
//		AHNHS_CHKOPT_KDTARCER |
		AHNHS_CHKOPT_OPENPROCESS |
		AHNHS_CHKOPT_MESSAGEHOOK |
#endif
		AHNHS_CHKOPT_SPEEDHACK | 
		AHNHS_CHKOPT_AUTOMOUSE |
		AHNHS_CHKOPT_PROCESSSCAN |
		AHNHS_USE_LOG_FILE |
		AHNHS_ALLOW_SVCHOST_OPENPROCESS |
		AHNHS_ALLOW_LSASS_OPENPROCESS |
		AHNHS_ALLOW_CSRSS_OPENPROCESS |
		AHNHS_DONOT_TERMINATE_PROCESS |
		AHNHS_CHKOPT_HSMODULE_CHANGE;

	// HackShield 서비스 초기화

	// 초기화 함수 _AhnHS_Initialize는 다음과 같은 인자 값을 가지고 있습니다.
	// 3번째: 게임코드,
	// 4번째: 라이센스 키,
	// 6번째: 스피드핵 민감도

	// 적용 시 알맞은 값으로 대체하시기 바랍니다.
	// 자세한 내용은 "프로그래밍 가이드"를 참조하세요

//	dwRet = _AhnHS_Initialize ( szFullFilePath, HS_CallbackProc, 4401, "D74945F75050F76EC6EA7DF0", 
//		dwOption, AHNHS_SPEEDHACK_SENSING_RATIO_HIGH );
	dwRet = _AhnHS_Initialize ( szFullFilePath, HS_CallbackProc, 4402, "93C4B89B1187AB1B7BBC398A", 
		dwOption, AHNHS_SPEEDHACK_SENSING_RATIO_HIGH );

	// HackShield 초기화 실패 시 오류 처리
	if ( dwRet != HS_ERR_OK ) 
	{
		// _AhnHS_Initialize 함수의 호출 실패 시에 넘어오는 코드에 따라 처리해주는 부분입니다.
		// "프로그래밍 가이드"를 참조하시어 적당한 오류메시지를 설정해주시기 바랍니다.
		switch ( dwRet )
		{
		case HS_ERR_INVALID_PARAM:
			{
				wsprintf ( szMsg, "[HS Code=%x] Init: 잘못된 Callback함수 포인터 지정 및 라이센스 키 값 지정이 올바르지 않습니다. ", 
					dwRet );
				break;
			}
		case HS_ERR_INVALID_LICENSE:
			{
				wsprintf ( szMsg, "[HS Code=%x] Init: 라이센스 키가 올바르지 않습니다.", 
					dwRet );
				break;
			}
		default:
			{
				wsprintf ( szMsg, "[HS Code=%x] Init: 알 수 없는 오류가 발생하였습니다.", 
					dwRet );
				break;
			}
		}
		// 게임 내에서 알림 메시지를 발생합니다.
//		MessageBox( _g_hWnd, szMsg, "HSERR", MB_OK );


		return FALSE;
	}
	return TRUE;
}

// HackShield 서비스 시작
BOOL HS_StartService ( void )
{
	int		nReturn = 0;
	TCHAR	szMsg[MAX_PATH];

	// 시작 함수 호출 
	nReturn = _AhnHS_StartService();

	// HackShield 시작 실패시 오류 처리
	if ( nReturn != HS_ERR_OK )
	{
		switch ( nReturn )
		{
		case HS_ERR_NOT_INITIALIZED:
			{
				wsprintf ( szMsg, "[HS Code=%x] StartService: _Ahn_HS_Initialize 초기화가 필요합니다. ", nReturn ); 
					break;
			}
		case HS_ERR_ALREADY_SERVICE_RUNNING:
			{
				wsprintf ( szMsg, "[HS Code=%x] StartService: _Ahn_HS_Initialize 가 이미 초기화 되어 있습니다.", nReturn );
					break;
			}

		default:
			{
				wsprintf ( szMsg, "[HS Code=%x] StartService: 알 수 없는 오류가 발생하였습니다. ", nReturn );
					break;
			}
		}
		// 게임내에서 알림 메시지를 발생합니다.
//		MsgBox (szMsg, "알림" ..);
		return FALSE;
	}
	return TRUE;
}

// HackShield 서비스 중지
BOOL HS_StopService ( void )
{
	int		nReturn = 0;
//	TCHAR	szMsg[MAX_PATH];

	nReturn = _AhnHS_StopService();

	// HackShield 중지 실패 시 오류 처리
	if ( nReturn != HS_ERR_OK ) 
	{
//		...
			return FALSE;
	}
	return TRUE;
}

// HackShield 서비스 해제
BOOL HS_UnInit ( void )
{
	int		nReturn = 0;
//	TCHAR	szMsg[MAX_PATH];

	nReturn = _AhnHS_Uninitialize();

	// HackShield 해제 실패 시 오류 처리
	if ( nReturn != HS_ERR_OK ) 
	{
//		..
			return FALSE;
	}
	return TRUE;
}

// HackShield 서비스로부터 감지되는 모든 해킹관련 이벤트를 수신하기 위한 콜백 함수
int __stdcall HS_CallbackProc( long lCode, long lParamSize, void* pParam )
{
	TCHAR	szMsg[MAX_PATH];

	switch ( lCode )
	{
	case AHNHS_ENGINE_DETECT_GAME_HACK:
		{
			wsprintf ( szMsg, "[HS Code=%x] MoSiang can't play with this program[%s].", lCode, ( LPTSTR ) pParam );
			MessageBox( _g_hWnd, szMsg, "HSError", MB_OK );

				// 게임 내에서 알림 메시지를 발생합니다.
//				MsgBox (szMsg, "알림" ..);
			// 게임종료를 권장합니다.
			//PostQuitMessage(0);
			PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
		}
		break;
	case AHNHS_ACTAPC_DETECT_SPEEDHACK_APP:
	case AHNHS_ACTAPC_DETECT_SPEEDHACK:
		{
			wsprintf ( szMsg, "[HS Code=%x] Detected SpeedHack.", lCode);
			MessageBox( _g_hWnd, szMsg, "HSError", MB_OK );
				// 게임 내에서 알림 메시지를 발생합니다.
	//			MsgBox (szMsg, "알림" ..);
			// 스피드핵 사용에 민감한 게임프로그램은 강제 종료합니다.
			//PostQuitMessage(0);
			PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
		}
		break;

	case AHNHS_ACTAPC_DETECT_AUTOMACRO:
		{
			wsprintf ( szMsg, "Detected AutoMacro.");
			MessageBox( _g_hWnd, szMsg, "HSError", MB_OK );
				// 게임 내에서 알림 메시지를 발생합니다.
	//			MsgBox (szMsg, "알림" ..);
			//오토매크로 사용자

			//PostQuitMessage(0);
			PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );			
		}
		break;

	case AHNHS_ACTAPC_DETECT_KDTRACE_CHANGED:
	case AHNHS_ACTAPC_DETECT_KDTRACE:
		{
			wsprintf ( szMsg, "[HS Code=%x] Detected Debuging.", lCode);
			MessageBox( _g_hWnd, szMsg, "HSError", MB_OK );

			// 게임 내에서 알림 메시지를 발생합니다.
//			MsgBox (szMsg, "알림" ..);
			// 사용자가 게임프로그램을 디버깅 작업을 진행할 가능성이 높으므로 종료를 권장합니다.
			//PostQuitMessage(0);
			PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
		}
		break;
	case AHNHS_ACTAPC_DETECT_HOOKFUNCTION:
	case AHNHS_ACTAPC_DETECT_MESSAGEHOOK:
	case AHNHS_ACTAPC_DETECT_MODULE_CHANGE:
		{
			wsprintf ( szMsg, "[HS Code=%x] HackShield Error.", lCode );
			MessageBox( _g_hWnd, szMsg, "HSError", MB_OK );
			// 게임 내에서 알림 메시지를 발생합니다.
//			MsgBox (szMsg, "알림" ..);
			// 게임종료를 권장함

			//PostQuitMessage(0);
			PostMessage( _g_hWnd, WM_CLOSE, NULL, NULL );
		}
		break;
	case AHNHS_ENGINE_DETECT_GENERAL_HACK:
	case AHNHS_ACTAPC_DETECT_ALREADYHOOKED:
	case AHNHS_ACTAPC_DETECT_DRIVERFAILED:
//	case AHNHS_ACTAPC_DETECT_SPEEDHACK_APP:
		{
			// 위의 경우는 별도로 처리하지 않으셔도 됩니다.
			break;
		}
	}
	return 1;
}

// ※ HS_PauseService :
//    특정 XP OS에서 채팅창 한글키 입력이 불가능한 경우,
//    키보드 MSG Hooking 보호 기능을 일시적으로 해제하는 함수입니다.
BOOL HS_PauseService ( void )
{
	int		nReturn = 0;
	TCHAR	szMsg[MAX_PATH];

	nReturn = _AhnHS_PauseService ( AHNHS_CHKOPT_MESSAGEHOOK );

	if ( nReturn != HS_ERR_OK )
	{
		switch ( nReturn )
		{
		case HS_ERR_NOT_INITIALIZED:
			{
				wsprintf ( szMsg, "[HS Code=%x] PauseService: _Ahn_HS_Initialize 초기화가 필요합니다. ", nReturn );
					break;
			}
		case HS_ERR_SERVICE_NOT_RUNNING:
			{
				wsprintf ( szMsg, "[HS Code=%x] PauseService: _Ahn_HS_StartService 실행이 필요합니다.", nReturn );
				break;
			}

		default:
			{
				wsprintf ( szMsg, "[HS Code=%x] PauseService: 알수없는 오류가 발생 하였습니다.", 
					nReturn );
				break;
			}
		}
		// 게임내에서 알림 메시지를 발생합니다.
//		MsgBox (szMsg, "알림" ..);
		return FALSE;
	}
	return TRUE;
}

// _AhnHS_PauseService() 함수를 이용하여,
// 해제된 키보드 MSG Hooking 보호 기능을 재실행 한다.
BOOL HS_ResumeService ( void )
{
	int		nReturn = 0;
	TCHAR	szMsg[MAX_PATH];

	nReturn = _AhnHS_ResumeService ( AHNHS_CHKOPT_MESSAGEHOOK );

	if ( nReturn != HS_ERR_OK )
	{
		switch ( nReturn )
		{
		case HS_ERR_NOT_INITIALIZED:
			{
				wsprintf ( szMsg, "[HS Code=%x] ResumeService: _Ahn_HS_Initialize 초기화가 필요합니다.", nReturn );
					break;
			}
		case HS_ERR_SERVICE_NOT_RUNNING:
			{
				wsprintf ( szMsg, "[HS Code=%x] ResumeService: _Ahn_HS_StartService 실행이 필요합니다.", nReturn );
					break;
			}

		default:
			{
				wsprintf( szMsg, "[HS Code=%x] ResumeService: 알 수 없는 오류가 발생하였습니다.", nReturn );
					break;
			}
		}
		// 게임내에서 알림 메시지를 발생합니다.
//		MsgBox (szMsg, "알림" ..);
		return FALSE;
	}
	return TRUE;
}

void HS_SaveFuncAddress()
{
//	TCHAR	szMsg[MAX_PATH];

    int nResult = _AhnHS_SaveFuncAddress( 3,
											HS_Init,
											HS_CallbackProc,
											CHackShieldManager::NetworkMsgParse
											);
}

#endif

#ifdef _NPROTECT_
BOOL CALLBACK NPGameMonCallback(DWORD dwMsg, DWORD dwArg)
{
	return NPROTECTMGR->MsgProc(dwMsg, dwArg);
}
#endif
