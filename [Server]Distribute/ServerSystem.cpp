// ServerSystem.cpp: implementation of the CServerSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerSystem.h"
#include "Console.h"
#include "Network.h"
#include "DataBase.h"
#include "CommonNetworkMsgParser.h"
#include "DistributeNetworkMsgParser.h"
#include "DistributeDBMsgParser.h"

#include "Usertable.h"
#include "Servertable.h"
#include <MemoryPoolTempl.h>
#include "BootManager.h"
#include "UserManager.h"
#include "MHFile.h"
#include "MHTimeManager.h"
#include "..\[CC]ServerModule\MiniDumper.h"
#include "BuddyAuth.h"


#ifdef _HK_LOCAL_
BYTE g_Encrypt0;
BYTE g_Encrypt1;
#endif

extern int g_nServerSetNum;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//#define _DBTHREAD
/*
DWORD _stdcall ProcessDBMessageThread(void* p)
{
	while(1)
	{
		Sleep(5);
		
		g_DB.ProcessingDBMessage();

		if(g_pServerSystem->m_DBThreadTerminate)
			break;
	}
	return 0;
}
*/
void ProcessDBMessage()
{
	g_DB.ProcessingDBMessage();
}

void ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
void ReceivedMsgFromUser(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
void OnAcceptServer(DWORD dwConnectionIndex);
void OnDisconnectServer(DWORD dwConnectionIndex);
void OnAcceptUser(DWORD dwConnectionIndex);
void OnDisconnectUser(DWORD dwConnectionIndex);

void fConnectionCheck();
void GameProcess();

void ButtonProc1();
void ButtonProc2();
void ButtonProc3();
void OnCommand(char* szCommand);


HWND g_hWnd;

/*
HHOOK hHook;

LRESULT WINAPI CallWndProc(int nCode, WPARAM wParam, LPARAM lParam) 
{ 
    CWPSTRUCT* lpWp = (CWPSTRUCT*)lParam;

	if(lpWp->message == WM_CLOSE)
	{
		DefWindowProc(g_hWnd, lpWp->message, lpWp->wParam, lpWp->lParam);
		PostQuitMessage(0);
	}
    
    return CallNextHookEx(hHook, nCode, wParam, lParam); 
} 
*/


// taiyo 
// 조금이나마 속도향상을 보기 위해 전역 변수로~!
CServerSystem * g_pServerSystem = NULL;
CMemoryPoolTempl<USERINFO> g_UserInfoPool;

CServerSystem::CServerSystem()
{
	// servertable usertable 초기화 ---------------------
	g_UserInfoPool.Init(3000, 500,"USERINFO");
	g_pUserTable = new CUserTable;
	g_pUserTable->Init(3000/3);
	g_pServerTable = new CServerTable;
	g_pServerTable->Init(50);

	CoInitialize(NULL);

	m_bUserFireWall = FALSE;
	ZeroMemory( m_strFireWallIP, sizeof(m_strFireWallIP) );
	
	m_Nation = eNATION_KOREA;
}

CServerSystem::~CServerSystem()
{
	CoUninitialize();
}

extern DWORD g_dwMaxUser;

void CServerSystem::Start(WORD ServerNum)
{	
//AO½A ·IAA
	SetNation();

#ifdef _HK_LOCAL_
	g_Encrypt0 = g_Encrypt1 = GetTickCount() % 100;	//랜덤하게..
#endif

	FILE* fpMC = fopen( "MaxUserCount.txt", "r" );
	if(fpMC)
	{
		fscanf( fpMC, "%d", &g_dwMaxUser );
		fclose( fpMC );
	}

	BOOTMNGR->AddSelfBootList(DISTRIBUTE_SERVER, ServerNum, g_pServerTable);
	gUserMGR.LoadDistributeDesc();	//이걸 읽기위해서 위엣줄이 필요한거라구.

//	m_IdxCreater = ICCreate();
//	ICInitialize(m_IdxCreater,5000);
	
	//////////////////////////////////////////////////////////////////////////
	// 콘솔 초기화
	// 버튼 설정.외부입력용 버튼이 28개 준비되어있다.한개도 사용하지 않을 경우 pCustomButton 필드를 NULL로 채운다.
	
	MENU_CUSTOM_INFO pMenu[3];
	strcpy(pMenu[0].szMenuName, "Button1");
	pMenu[0].cbMenuCommand = ButtonProc1;
	strcpy(pMenu[1].szMenuName, "Button2");
	pMenu[1].cbMenuCommand = ButtonProc2;
	strcpy(pMenu[2].szMenuName, "Button3");
	pMenu[2].cbMenuCommand = ButtonProc3;

	if(!g_Console.Init(3,pMenu,OnCommand))
		MessageBox(NULL,"Fail initialize Console",0,0);

	char windowTitleMsg[64];
	sprintf(windowTitleMsg,"Distribute Server_%d", ServerNum);

	g_hWnd = GetActiveWindow();
	SetWindowText(g_hWnd, windowTitleMsg);


//	DWORD    dwProcessID = NULL;
//	DWORD    dwTreadID = ::GetWindowThreadProcessId(g_hWnd, &dwProcessID );

//	HOOKPROC hProc;
//    hProc = CallWndProc;
//    hHook = ::SetWindowsHookEx(WH_CALLWNDPROC, hProc, (HINSTANCE)NULL, dwTreadID);


	//////////////////////////////////////////////////////////////////////////
	// 네트워크 설정

	/// 네트워크 파서 설정-------------------------------
	g_pServerMsgParser[0] = NULL;
	for(int i=1 ; i<MP_MAX ; ++i)
		g_pServerMsgParser[i] = ErrorMsgParser;
	for(i=1 ; i<MP_MAX ; ++i)
		g_pUserMsgParser[i] = ErrorMsgParser;
	g_pServerMsgParser[MP_SERVER] = MP_DISTRIBUTESERVERMsgParser;
	g_pServerMsgParser[MP_POWERUP] = MP_POWERUPMsgParser;
	g_pServerMsgParser[MP_USERCONN] = MP_USERCONNMsgParser;
	g_pUserMsgParser[MP_USERCONN] = MP_USERCONNMsgParser;
	g_pUserMsgParser[MP_AUTOPATCH] = MP_DISTRIBUTEAUTOPATCHMsgParser;
	
	g_pServerMsgParser[MP_MORNITORMAPSERVER] = MP_MonitorMsgParser;



	// ServerList 읽어들임 ------------------------

	//g_pListCollect = new CListCollect;
	//g_pListCollect->LoadServerInfo();
	//g_pListCollect->LoadDistributeNum();

	CUSTOM_EVENT ev[3];
	ev[0].dwPeriodicTime	= 500;
	ev[0].pEventFunc		= GameProcess;
//	ev[1].dwPeriodicTime	= 100;
	ev[1].dwPeriodicTime	= 0;
	ev[1].pEventFunc		= ProcessDBMessage;
	ev[2].dwPeriodicTime	= 1000 * 30;
	ev[2].pEventFunc		= fConnectionCheck;

		
	DESC_NETWORK desc;
	desc.OnAcceptServer = OnAcceptServer;
	desc.OnDisconnectServer = OnDisconnectServer;
	desc.OnAcceptUser = OnAcceptUser;
	desc.OnDisconnectUser = OnDisconnectUser;
	desc.OnRecvFromServerTCP = ReceivedMsgFromServer;
	desc.OnRecvFromUserTCP = ReceivedMsgFromUser;

#ifdef _DBTHREAD
	desc.dwCustomDefineEventNum = 1;
#else
	desc.dwCustomDefineEventNum = 3;
#endif

	desc.pEvent = ev;

	desc.dwMainMsgQueMaxBufferSize = 20480000;	//
	desc.dwMaxServerNum = 100;
	desc.dwMaxUserNum = 3000;
	desc.dwServerBufferSizePerConnection = 512000;
	desc.dwServerMaxTransferSize = 65535;
	desc.dwUserBufferSizePerConnection = 128000;
	desc.dwUserMaxTransferSize = 65535;
	desc.dwConnectNumAtSameTime = 3000;
	desc.dwFlag = 0;//NETDDSC_DEBUG_LOG;

	g_Network.Init(&desc);
	//g_Network.Start();
	
	// Booting
//	BOOTMNGR->AddSelfBootList(DISTRIBUTE_SERVER, ServerNum, g_pServerTable);//위로 옮겨놓구--;
	if(!BOOTMNGR->StartServer(&g_Network, g_pServerTable->GetSelfServer()))
	{
		MessageBox(NULL,"Server Initialize Failed (StartServer)",NULL,NULL);
		return;
	}
	if(!BOOTMNGR->ConnectToMS(&g_Network, g_pServerTable))
	{
		SERVERINFO info;
		info.wServerKind = MONITOR_SERVER;
		OnConnectServerFail(&info);		
	}

	//////////////////////////////////////////////////////////////////////////
	// DB 설정
	DWORD maxthread = 2;
	DWORD maxqueryinsametime = 1024;
	FILE* fp = fopen("DistributeDBInfo.txt","r");
	if(fp)
	{
		fscanf(fp,"%d %d",&maxthread,&maxqueryinsametime);
		fclose(fp);
	}
	if(g_DB.Init(maxthread,maxqueryinsametime,TRUE) == FALSE)
		MessageBox(NULL,"DataBase Initializing Failed",0,0);

	m_DBThreadTerminate = FALSE;
#ifdef _DBTHREAD
	CreateThread(0,0,ProcessDBMessageThread,0,0,&m_DBThreadID);
#endif
	
	
	// 로그인 체크 테이블 초기화
	LoginCheckInit();
#ifdef _HK_LOCAL_
	LoadSAT();
	LoadAC();
#endif
//	LoadFireWallIP();

	// Dump
#ifdef _JAPAN_LOCAL_
	MiniDumper md(MiniDumper::DUMP_LEVEL_0);
#elif defined _CHINA_LOCAL_
	MiniDumper md(MiniDumper::DUMP_LEVEL_0);
#elif defined _HK_LOCAL_
	MiniDumper md(MiniDumper::DUMP_LEVEL_0);
#elif defined _TL_LOCAL_
	MiniDumper md(MiniDumper::DUMP_LEVEL_0);
#else
	MiniDumper md(MiniDumper::DUMP_LEVEL_2);
#endif


	g_Console.LOG(4, "-----------   DISTRIBUTE SERVER START  -----------------");
	// 이 부분은 윈도우의 메시지 루프.CONSOLE을 사용하든 윈도우 메시지 기반으로 짜든 어떤식으로든 이벤트를 대기해야하
	// 하므로 귀찮은 메시지 루프 작성을 하고 싶지 않다면 이 메소드를 사용한다.그 닳고 닳은 메시지 루프와 동일한 기능을 
	// 수행한다.이 메소드는 I4DyuchiCONSOLE 다이얼로그의 x버튼을 누를때까지 리턴하지 않는다.이 메소드가 리턴한다면 프
	// 로그램이 종료하는 것이다.
	
	g_bReady = TRUE;

	g_Console.WaitMessage();
}

#ifdef _HK_LOCAL_
void CServerSystem::LoadSAT()
{
	CMHFile file;
	file.Init("Resource/SAT.bin","rb");
	if(file.IsInited() == FALSE)
	{
		return;
	}

	file.GetDword();
	for(int n=0;n<30;++n)
	{
		file.GetWord();
		m_aniTimeMale[n] = file.GetDword();
		file.GetDword();
	}

	file.Release();
}

void CServerSystem::LoadAC()
{
	CMHFile file;
	file.Init("Resource/AbilityCalcInfo.bin","rb");
	if(file.IsInited() == FALSE)
	{
		return;
	}

	for(int n=0;n<30;++n)
	{
		file.GetDword();
		file.GetDword();
		file.GetFloat();
		file.GetFloat();
		file.GetDword();
		m_ac[n] = file.GetDword();
		file.GetDword();
		file.GetDword();
		file.GetDword();
		file.GetDword();
		file.GetFloat();
		file.GetFloat();
		file.GetDword();
		file.GetDword();
	}

	file.Release();
}
#endif

void CServerSystem::LoadFireWallIP()
{
	CMHFile file;
	if( !file.Init( "firewallip.txt", "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
		return;

	while(1)
	{
		if( file.IsEOF() )
			break;

		if( file.GetInt() == g_nServerSetNum )
		{
			SafeStrCpy( m_strFireWallIP, file.GetString(), 16 );
			if( *m_strFireWallIP != 0 )
				m_bUserFireWall = TRUE;

			break;
		}
	}

	file.Release();	
}

void CServerSystem::ConnectionCheck()
{	// YH 현재 30초마다 한번씩 들어옴
	DWORD _60sec = 60*1000;
	USERINFO* pInfo;
	DWORD elapsedtime;

	if(g_bReady == FALSE)
		return;
	
#ifdef _HK_LOCAL_
	static DWORD nLastEncryptCheck = gCurTime;
	if( gCurTime - nLastEncryptCheck >= 1000*60*60 )	//1시간
	{
		g_Encrypt0 = g_Encrypt1;
		g_Encrypt1 = ( g_Encrypt1 + 1 ) % 100;
//		g_Console.LOG( 4, "Encrypt Changed : 0:%d, 1:%d", g_Encrypt0, g_Encrypt1 );
		nLastEncryptCheck = gCurTime;
	}
#endif

	cPtrList removelist;

	g_pUserTable->SetPositionHead();
	while(pInfo = g_pUserTable->GetData())
	{
		if(pInfo->dwConnectionIndex != 0)
		{
			// 아직 접속이 제대로 이뤄지지 않은 경우
			elapsedtime = gCurTime - pInfo->dwLastConnectionCheckTime;
			if( elapsedtime > _60sec * 2 )
			{
				if(pInfo->bConnectionCheckFailed)
				{
					//!!테스트 후 30초 마다 들어오게 바꾸고 지울것~!
					//pInfo->dwLastConnectionCheckTime += 1000 * 10;	//OnDisconnectUser( 리턴 받기 전에 반복 방지.
					removelist.AddTail(pInfo);
					continue;
				}
				else
				{
					pInfo->bConnectionCheckFailed = TRUE;
					SendConnectionCheckMsg(pInfo);
					pInfo->dwLastConnectionCheckTime = gCurTime;
				}
			}
		}
	}

	PTRLISTPOS pos = removelist.GetHeadPosition();
	while( pos )
	{
		USERINFO* p = (USERINFO*)removelist.GetNext(pos);
		ASSERT(p->dwConnectionIndex);
//		LoginCheckDelete(p->dwUserID);		// 로그인체크 테이블에서 삭제
//		g_pServerSystem->ReleaseAuthKey(p->dwUniqueConnectIdx);
//		g_pUserTable->RemoveUser(p->dwConnectionIndex);
//		memset(p,0,sizeof(USERINFO));
//		g_UserInfoPool.Free(p);
		DisconnectUser( p->dwConnectionIndex );
	}
	removelist.RemoveAll();
}

void CServerSystem::SendConnectionCheckMsg(USERINFO* pInfo)
{
	MSGBASE msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CONNECTION_CHECK;
	g_Network.Send2User(pInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
}

void CServerSystem::Process()
{
	MHTIMEMGR_OBJ->Process();

	g_DB.ProcessingQueue();
//	g_DB.ProcessingDBMessage();

	g_BuddyAuth.RevMsgProcess();
}


void CServerSystem::End()
{
//	UnhookWindowsHookEx(hHook);
	m_DBThreadTerminate = TRUE;
//	ICRelease(m_IdxCreater);
	
	g_DB.Release();
	
	g_Network.Release();
	g_Console.Release();

//	if(g_pListCollect)
//	{
//		delete g_pListCollect;
//		g_pListCollect = NULL;
//	}
	if(g_pUserTable)
	{
		delete g_pUserTable;
		g_pUserTable = NULL;
	}
	if(g_pServerTable)
	{
		delete g_pServerTable;
		g_pServerTable = NULL;
	}
	CoFreeUnusedLibraries();
}

DWORD CServerSystem::MakeAuthKey()
{
	//return ICAllocIndex(m_IdxCreater) + 1;
	static DWORD ID = 1;//GetTickCount()*GetTickCount();
	if(ID == 0)
		++ID;
	return ID++;
}
void CServerSystem::ReleaseAuthKey(DWORD key)
{
	//ICFreeIndex(m_IdxCreater,key-1);
}

void CServerSystem::SetNation()
{
	CMHFile file;
	if( !file.Init( "LocalizingInfo.txt", "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
		return;

	if( strcmp( file.GetString(), "*NATION" ) == 0 )
	{
		if( strcmp( file.GetString(), "CHINA" ) == 0 )
		{
			m_Nation = eNATION_CHINA;
		}
	}

	file.Release();
}


// global function

void OnConnectServerSuccess(DWORD dwConnectionIndex, void* pVoid)
{
	SERVERINFO* info = (SERVERINFO*)pVoid;
	info->dwConnectionIndex = dwConnectionIndex;
	if(info->wServerKind == MONITOR_SERVER)
	{
		BOOTMNGR->NotifyBootUpToMS(&g_Network);
		g_Console.LOG(4, "Connected to the MS : %s, %d, (%d)", info->szIPForServer, info->wPortForServer, dwConnectionIndex);
	}
	else
	{
		BOOTMNGR->SendConnectSynMsg(&g_Network, dwConnectionIndex, g_pServerTable);
		g_Console.LOG(4, "Connected to the Server : %s, %d, (%d)", info->szIPForServer, info->wPortForServer, dwConnectionIndex);
	}

/*
	SERVERINFO* info = (SERVERINFO*)pVoid;
	if(!dwConnectionIndex) return;

	info->dwConnectionIndex = dwConnectionIndex;					// 이게 제대로 동작하나? 체크
	MSG_WORD msg;
	msg.Category = MP_SERVER;
	msg.Protocol = MP_SERVER_PWRUP;
	msg.wData = g_pServerTable->GetSelfServer()->wPortForServer;
	g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));*/

	//if(g_pServerTable->m_MaxServerConnectionIndex < dwIndex)			//??????
			//g_pServerTable->m_MaxServerConnectionIndex = dwIndex;	
}

void OnDisconnectServer(DWORD dwConnectionIndex)
{
	g_Console.LOG(4, "Server Disconnected : ConnectionIndex %d", dwConnectionIndex);//pjslocal

	// 서버정보 지움
	SERVERINFO * delInfo = g_pServerTable->RemoveServer(dwConnectionIndex);
	if(!delInfo)
	{
		//BootManager작동이 완료하지 않았을 때 서버가 꺼질 경우 발생,(올바른 에러)
		ASSERT(0);
		return;
	}
	delete delInfo;
}

void OnConnectServerFail(void* pVoid)
{	
	SERVERINFO* info = (SERVERINFO*)pVoid;
	if(info->wServerKind == MONITOR_SERVER)
	{
		BOOTMNGR->AddBootListINI(DISTRIBUTE_SERVER, 0, g_pServerTable);
		BOOTMNGR->BactchConnectToMap(&g_Network, g_pServerTable);
		g_Console.LOG(4, "Failed to Connect to the MS : %s, %d", info->szIPForServer, info->wPortForServer);
	}
	else
	{
		//not process
		g_Console.LOG(4, "Failed to Connect to the Server : %s, %d", info->szIPForServer, info->wPortForServer);
		BOOTMNGR->RemoveBootList(g_pServerTable, info->wPortForServer);
	}	
}
void OnAcceptServer(DWORD dwConnectionIndex)
{// not used
	char strr[255];
	wsprintf(strr, "%d 번 connectionIndex 서버 접속 ", dwConnectionIndex);//pjslocal

	g_Console.LOG(4, strr);
}

void OnAcceptUser(DWORD dwConnectionIndex)
{	
	if(g_bReady == FALSE)
	{
		// 초기화가 완전히 안됐는데 들어온경우.
		MSGBASE send;
		send.Category = MP_USERCONN;
		send.Protocol = MP_USERCONN_SERVER_NOTREADY;
		send.dwObjectID = 0;
		g_Network.Send2User(dwConnectionIndex, (char *)&send, sizeof(send));

		DisconnectUser(dwConnectionIndex);
		return;
	}

	USERINFO* pPreInfo = g_pUserTable->FindUser( dwConnectionIndex );
	if( pPreInfo )
	{
		g_pUserTable->RemoveUser( dwConnectionIndex );
		memset(pPreInfo, 0, sizeof(USERINFO));
		g_UserInfoPool.Free( pPreInfo );
	}

	DWORD authkey = g_pServerSystem->MakeAuthKey();
	USERINFO * info = g_UserInfoPool.Alloc();
	memset(info, 0, sizeof(USERINFO));
	info->dwConnectionIndex = dwConnectionIndex;
	info->dwUniqueConnectIdx = authkey;
	info->dwLastConnectionCheckTime = gCurTime;	//SW051107 추가
	g_pUserTable->AddUser(info,dwConnectionIndex);

#ifndef _HK_LOCAL_
	MSGBASE send;
	send.Category = MP_USERCONN;
	send.Protocol = MP_USERCONN_DIST_CONNECTSUCCESS;
	send.dwObjectID = authkey;
	g_Network.Send2User(dwConnectionIndex, (char *)&send, sizeof(send));
#else
	MSG_DWORD2 send;
	send.Category = MP_USERCONN;
	send.Protocol = MP_USERCONN_DIST_CONNECTSUCCESS;
	send.dwObjectID = authkey+11;
	send.dwData1 = g_Encrypt1*7 + authkey + 23;
	send.dwData2 = authkey*3 + 47;

	g_Network.Send2User(dwConnectionIndex, (char *)&send, sizeof(send));
#endif

//	g_Console.Log(eLogDisplay,4, "OnAcceptUser : Client Connected - Idx:%d, AuthKey:%d, Total(%d)",dwConnectionIndex,authkey, g_pUserTable->GetUserCount());
//	g_Console.Log(eLogFile,4, "OnAcceptUser : Client Connected - Idx:%d, AuthKey:%d, Total(%d)",dwConnectionIndex,authkey, g_pUserTable->GetUserCount());
}

void OnDisconnectUser(DWORD dwConnectionIndex)
{
	g_pUserTable->OnDisconnectUser(dwConnectionIndex);
//	g_Console.Log(eLogDisplay,4, "OnAcceptUser : Client Disconnected - Idx:%d, Total(%d)",dwConnectionIndex, g_pUserTable->GetUserCount());
//	g_Console.Log(eLogFile,4, "OnAcceptUser : Client Disconnected - Idx:%d, Total(%d)",dwConnectionIndex, g_pUserTable->GetUserCount());
}

void ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	MSGROOT* pTempMsg = reinterpret_cast<MSGROOT*>(pMsg);
	
	if( g_pServerMsgParser[pTempMsg->Category] == NULL ||
		pTempMsg->Category >= MP_MAX ||
		pTempMsg->Category == 0)
		return;

	g_pServerMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);
}

void ReceivedMsgFromUser(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	MSGROOT* pTempMsg = reinterpret_cast<MSGROOT*>(pMsg);
//	ServerTraffic * Msg = reinterpret_cast<ServerTraffic*>(pMsg);
	ASSERT(g_pUserMsgParser[pTempMsg->Category]);

	if( g_pUserMsgParser[pTempMsg->Category] == NULL ||
		pTempMsg->Category >= MP_MAX ||
		pTempMsg->Category == 0)
		return;

	g_pUserMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);
}

//SW051107 추가
void fConnectionCheck()
{
	g_pServerSystem->ConnectionCheck();
}

void GameProcess()
{
	g_pServerSystem->Process();
}

// console interface function
void ButtonProc1()
{
	//SendMessage(g_hWnd, WM_CLOSE, 0, 0);
}

void ButtonProc2()
{
	g_Console.LOG(eLogDisplay, "UserCount : %d", g_pUserTable->GetUserCount());
}

void ButtonProc3()
{

}

void OnCommand(char* szCommand)
{

}

BOOL CServerSystem::IsInvalidCharInclude( char* pStr )
{
	while( *pStr )
	{
		BOOL bOk = FALSE;

		if( IsDBCSLeadByte( *pStr ) )
		{
			++pStr;
		}
		else
		{
			//영문
			if( ( *pStr >= 'A' && *pStr <= 'Z' ) || ( *pStr >= 'a' && *pStr <= 'z' ) )
				bOk = TRUE;
			//숫자
			else if( *pStr >= '0' && *pStr <= '9' )
				bOk = TRUE;
		}

		++pStr;

		if( bOk == FALSE )
		{
			return TRUE;
		}
	}

	return FALSE;
}

