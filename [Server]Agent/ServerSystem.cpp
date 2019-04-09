// ServerSystem.cpp: implementation of the CServerSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerSystem.h"
#include "Console.h"
#include "Network.h"
#include "DataBase.h"
#include "CommonNetworkMsgParser.h"
#include "AgentNetworkMsgParser.h"

#include "Usertable.h"
#include "Servertable.h"

#include <MemoryPoolTempl.h>

#include "AgentDBMsgParser.h"
#include "Crypt.h"

#include "FilteringTable.h"
#include "MHTimeManager.h"
#include "BootManager.h"
#include "MHFile.h"
#include "AgentDBMsgParser.h"
#include "TrafficLog.h"
#include "GMPowerList.h"
#include "ShoutManager.h"
#include "PlustimeMgr.h"

#include "SkillDalayManager.h"
#include "JackpotManager_Agent.h"
#include "BobusangManager_Agent.h"
#include "PunishManager.h"


#include "..\[CC]ServerModule\MiniDumper.h"

#ifdef _HACK_SHIELD_
#include "HackShieldManager.h"
#endif

#ifdef _NPROTECT_
#include "ggsrv25.h"
#include "NProtectManager.h"
#endif

#ifdef _NETWORKTEST
DWORD RecvServerMsgAmount[MP_MAX] = {0,};
DWORD RecvUserMsgAmount[MP_MAX] = {0,};
extern DWORD SendServerMsgAmount[MP_MAX];
extern DWORD SendUserMsgAmount[MP_MAX];
#endif

void fConnectionCheck();

DWORD g_dwAcceptUser = 0;
BOOL g_bCloseWindow = FALSE;
extern DWORD g_dwAcceptUser2;
extern int	g_nServerSetNum;

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

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// taiyo 
CServerSystem * g_pServerSystem = NULL;
// MAPSERVER에서는 USERINFO가 아니라 CPlayer가 된다!
CMemoryPoolTempl<USERINFO> g_UserInfoPool;
#ifdef _NPROTECT_
CMemoryPoolTempl<CCSAuth2> g_CCSAuth2Pool;
#endif
CServerSystem::CServerSystem()
{
	CoInitialize(NULL);
#ifdef _CRYPTCHECK_ 
	m_bUseCrypt = TRUE;
#else
	m_bUseCrypt = FALSE;
#endif

	m_Nation = eNATION_KOREA;
	m_bUseEventNotify = FALSE;
	ZeroMemory( m_strEventNotifyTitle, sizeof( m_strEventNotifyTitle ) );
	ZeroMemory( m_strEventNotifyContext, sizeof( m_strEventNotifyContext ) );
	memset( m_bEvenList, 0, sizeof(BOOL)*eEvent_Max );

	m_bTestServer = FALSE;

/*
#ifdef _HK_LOCAL_
	m_dwFPSTimeTest = 0;
	m_nDBProcessDelay = 0;
#endif
*/

}

CServerSystem::~CServerSystem()
{
	CoUninitialize();
}

void CServerSystem::Start(WORD ServerNum)
{

//임시 로컬
	SetNation();

	m_wServerNum = ServerNum;
	srand(GetTickCount());

	//SW050729

	JACKPOTMGR->Start();

	BOBUSANGMGR->BobusangMgr_Start();

//	m_IdxCreater = ICCreate();
//	ICInitialize(m_IdxCreater,5000);
	
	//////////////////////////////////////////////////////////////////////////
	// 콘솔 초기화
	// 버튼 설정.외부입력퓖E버튼이 28개 준비되엉復다.한개도 사퓖E舊갋않을 경퓖EpCustomButton 필드를 NULL로 채웝芩.
	MENU_CUSTOM_INFO pMenu[3];
	strcpy(pMenu[0].szMenuName, "Button1");
	pMenu[0].cbMenuCommand = ButtonProc1;
	strcpy(pMenu[1].szMenuName, "Button2");
	pMenu[1].cbMenuCommand = ButtonProc2;
	strcpy(pMenu[2].szMenuName, "Button3");
	pMenu[2].cbMenuCommand = ButtonProc3;

	if(!g_Console.Init(3,pMenu,OnCommand))
		MessageBox(NULL,"Console Initializing Failed",0,0);

	char TitleText[255];
	sprintf(TitleText, "Agent Server: %d", ServerNum);

	g_hWnd = GetActiveWindow();
	SetWindowText(g_hWnd, TitleText);
	

//	DWORD    dwProcessID = NULL;
  //  DWORD    dwTreadID = ::GetWindowThreadProcessId(g_hWnd, &dwProcessID );

//	HOOKPROC hProc;
//    hProc = CallWndProc;
//    hHook = ::SetWindowsHookEx(WH_CALLWNDPROC, hProc, (HINSTANCE)NULL, dwTreadID);


	SetMapChangeInfo();
	//////////////////////////////////////////////////////////////////////////
	// 네트워크 설정

	/// 네트워크 파서 설정-------------------------------
	g_pServerMsgParser[0] = NULL;	// YH 0번은 비퓖E? MP_SERVER == 1
	for(int i=1 ; i<MP_MAX ; ++i)
		g_pServerMsgParser[i] = TransToClientMsgParser;
	g_pServerMsgParser[MP_SERVER] = MP_AGENTSERVERMsgParser;
	g_pServerMsgParser[MP_POWERUP] = MP_POWERUPMsgParser;
	//g_pServerMsgParser[MP_CHAR] = MP_CHARMsgParser;
	g_pServerMsgParser[MP_MAP] = TransToClientMsgParser;
	g_pServerMsgParser[MP_ITEM] = MP_ITEMServerMsgParser;
	g_pServerMsgParser[MP_ITEMEXT] = MP_ITEMServerMsgParserExt;
	g_pServerMsgParser[MP_CHAT] = MP_CHATServerMsgParser;
	g_pServerMsgParser[MP_USERCONN] = MP_USERCONNMsgParser;
	g_pServerMsgParser[MP_MOVE] = TransToClientMsgParser;
	g_pServerMsgParser[MP_MUGONG] = TransToClientMsgParser;
	g_pServerMsgParser[MP_AUCTIONBOARD] = TransToClientMsgParser;
	g_pServerMsgParser[MP_CHEAT] = TransToClientMsgParser;
	g_pServerMsgParser[MP_QUICK] = TransToClientMsgParser;
	g_pServerMsgParser[MP_PACKEDDATA] = MP_PACKEDMsgParser;
	g_pServerMsgParser[MP_PARTY] = MP_PARTYServerMsgParser;	
	g_pServerMsgParser[MP_FRIEND] = MP_FRIENDMsgParser;
	g_pUserMsgParser[MP_NOTE] = MP_NOTEMsgParser;
	g_pServerMsgParser[MP_NOTE] = MP_NOTEServerMsgParser;
	g_pServerMsgParser[MP_WANTED] = MP_WANTEDServerMsgParser;
	
	g_pServerMsgParser[MP_AUCTION] = TransToClientMsgParser;
	g_pServerMsgParser[MP_MORNITORMAPSERVER] = MP_MonitorMsgParser;
	g_pServerMsgParser[MP_MURIMNET] = MP_MURIMNETServerMsgParser;
	g_pServerMsgParser[MP_CHEAT] = MP_CHEATServerMsgParser;
	g_pServerMsgParser[MP_HACKCHECK] = MP_HACKCHECKMsgParser;
	g_pServerMsgParser[MP_GUILD] = MP_GUILDServerMsgParser;
	g_pServerMsgParser[MP_GUILD_FIELDWAR] = MP_GUILD_FIELDWARServerMsgParser;	
	g_pServerMsgParser[MP_JACKPOT] = MP_JACKPOTServerMsgParser;
	g_pServerMsgParser[MP_SKILL] = MP_SkillServerMsgParser;
	g_pServerMsgParser[MP_GUILD_UNION] = MP_GUILD_UNIONServerMsgParser;
	g_pServerMsgParser[MP_GTOURNAMENT] = MP_GTOURNAMENTServerMsgParser;
	g_pServerMsgParser[MP_SIEGEWAR] = MP_SIEGEWARServerMsgParser;
	g_pServerMsgParser[MP_SIEGEWAR_PROFIT] = MP_SIEGEWARPROFITServerMsgParser;
	g_pServerMsgParser[MP_SURVIVAL] = MP_SURVIVALServerMsgParser;
	g_pServerMsgParser[MP_BOBUSANG] = MP_BOBUSANGServerMsgParser;
	g_pServerMsgParser[MP_OPTION] = TransToClientMsgParser;
	g_pServerMsgParser[MP_ITEMLIMIT] = MP_ITEMLIMITServerMsgParser;
	g_pServerMsgParser[MP_AUTONOTE] = MP_AUTONOTEServerMsgParser;
	g_pServerMsgParser[MP_FORTWAR] = MP_FORTWARServerMsgParser;

#ifdef _NPROTECT_
	g_pServerMsgParser[MP_NPROTECT] = MP_NPROTECTUserMsgParser;
#endif



	for(i=1 ; i<MP_MAX ; ++i)
		g_pUserMsgParser[i] = TransToMapServerMsgParser;
	g_pUserMsgParser[MP_USERCONN] = MP_USERCONNMsgParser;
	g_pUserMsgParser[MP_CHAT] = MP_CHATMsgParser;
	g_pUserMsgParser[MP_FRIEND] = MP_FRIENDMsgParser;
	g_pUserMsgParser[MP_NOTE] = MP_NOTEMsgParser;
	g_pUserMsgParser[MP_MURIMNET] = MP_MURIMNETUserMsgParser;
	g_pUserMsgParser[MP_OPTION] = MP_OPTIONUserMsgParser;
	g_pUserMsgParser[MP_DEBUG] = MP_DebugMsgParser;
	g_pUserMsgParser[MP_CHEAT] = MP_CHEATUserMsgParser;
	g_pUserMsgParser[MP_HACKCHECK] = MP_HACKCHECKMsgParser;
	g_pUserMsgParser[MP_GUILD] = MP_GUILDUserMsgParser;
	g_pUserMsgParser[MP_GUILD_FIELDWAR] = MP_GUILD_FIELDWARUserMsgParser;
	g_pUserMsgParser[MP_ITEM] = MP_ITEMUserMsgParser;
	g_pUserMsgParser[MP_ITEMEXT] = MP_ITEMUserMsgParserExt;
	g_pUserMsgParser[MP_PARTY] = MP_PARTYUserMsgParser;		//2008. 5. 26. CBH - 파티 매칭 시스탬 관련 추가
	g_pUserMsgParser[MP_GTOURNAMENT] = MP_GTOURNAMENTUserMsgParser;
	g_pUserMsgParser[MP_JACKPOT] = MP_JACKPOTUserMsgParser;
	g_pUserMsgParser[MP_SKILL] = MP_SkillUserMsgParser;
	g_pUserMsgParser[MP_GUILD_UNION] = MP_GUILD_UNIONUserMsgParser;
	g_pUserMsgParser[MP_SIEGEWAR] = MP_SIEGEWARUserMsgParser;
	g_pUserMsgParser[MP_SIEGEWAR_PROFIT] = MP_SIEGEWARPROFITUserMsgParser;
	g_pUserMsgParser[MP_SURVIVAL] = MP_SURVIVALUserMsgParser;
	g_pUserMsgParser[MP_BOBUSANG] = MP_BOBUSANGUserMsgParser;

	g_pUserMsgParser[MP_EXCHANGE] = MP_EXCHANGEUserMsgParser;
	g_pUserMsgParser[MP_STREETSTALL] = MP_STREETSTALLUserMsgParser;

	// 2005 크리스마스 이벤트
	// GM에 의한 날씨 셋팅을 맵별로 하콅E위해...
	g_pUserMsgParser[MP_WEATHER] = MP_WEATHERUserMsgParser;
	g_pUserMsgParser[MP_AUTONOTE] = MP_AUTONOTEUserMsgParser;

#ifdef _HACK_SHIELD_
	g_pUserMsgParser[MP_HACKSHIELD] = MP_HACKSHIELDUserMsgParser;
#endif
#ifdef _NPROTECT_
	g_pUserMsgParser[MP_NPROTECT] = MP_NPROTECTUserMsgParser;
#endif

	// servertable usertable 초기화 ---------------------
	
	g_UserInfoPool.Init(MAX_USER_IN_A_AGENT/2, 500,"UserInfoPool");
#ifdef _NPROTECT_
	g_CCSAuth2Pool.Init(MAX_USER_IN_A_AGENT/2, 500,"CCSAuth2Pool");
#endif
	g_pUserTable = new CUserTable;					// KEY : dwConnectionIndex
	g_pUserTable->Init(MAX_USER_IN_A_AGENT/2);
	g_pUserTableForUserID = new CUserTable;			// KEY : dwUserIdx
	g_pUserTableForUserID->Init(MAX_USER_IN_A_AGENT/2);
	g_pUserTableForObjectID = new CUserTable;		// KEY : dwObjectID
	g_pUserTableForObjectID->Init(MAX_USER_IN_A_AGENT/2);
	g_pServerTable = new CServerTable;				// KEY : ServerPort
	g_pServerTable->Init(MAX_SERVER_CONNECTION);
	
	CUSTOM_EVENT ev[3];
	ev[0].dwPeriodicTime	= 500;
	ev[0].pEventFunc		= GameProcess;
//	ev[1].dwPeriodicTime	= 100;
	ev[1].dwPeriodicTime	= 0;
	ev[1].pEventFunc		= ProcessDBMessage;
	ev[2].dwPeriodicTime	= 1000*30;	// 30sec
	ev[2].pEventFunc		= fConnectionCheck;

	DESC_NETWORK desc;
	desc.OnAcceptServer = OnAcceptServer;
	desc.OnDisconnectServer = OnDisconnectServer;
	desc.OnAcceptUser = OnAcceptUser;
	desc.OnDisconnectUser = OnDisconnectUser;
	desc.OnRecvFromServerTCP = ReceivedMsgFromServer;
	desc.OnRecvFromUserTCP = ReceivedMsgFromUser;
	desc.dwCustomDefineEventNum = 3;
	desc.pEvent = ev;

	desc.dwMainMsgQueMaxBufferSize = 20480000;			//max 20480000
	desc.dwMaxServerNum = MAX_SERVER_CONNECTION;		//max 3000
	desc.dwMaxUserNum = MAX_USER_IN_A_AGENT;			//max 3000
	desc.dwServerBufferSizePerConnection = 512000;		//max 512000
	desc.dwServerMaxTransferSize = 65535;				//max 65535
	desc.dwUserBufferSizePerConnection = 256000;		//max 512000
	desc.dwUserMaxTransferSize = 65535;					//max 65535
	desc.dwConnectNumAtSameTime = MAX_USER_IN_A_AGENT/4;	//about divide by 4
	desc.dwFlag = 0;

	g_Network.Init(&desc);

	BOOTMNGR->AddSelfBootList(AGENT_SERVER, ServerNum, g_pServerTable);
	if(!BOOTMNGR->StartServer(&g_Network, g_pServerTable->GetSelfServer()))
	{
		ASSERT(0);
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
	FILE* fp = fopen("AgentDBInfo.txt","r");
	if(fp)
	{
		fscanf(fp,"%d %d",&maxthread,&maxqueryinsametime);
		fclose(fp);
	}
	if(g_DB.Init(maxthread,maxqueryinsametime,TRUE) == FALSE)
		MessageBox(NULL,"DataBase Initializing Failed",0,0);


//	g_Network.Start();

//KES
	//필터링 초기화
	FILTERTABLE->Init();
	GMINFO->Init();

	PLUSTIMEMGR->Init();

	//TRAFFIC->StartRecord();
	TRAFFIC->Init();
	//TRAFFIC->WriteUserTrafficLogFile();
	
	SKILLDELAYMGR->LoadSkillUseInfo();

	//SW070626 보부상NPC
	BOBUSANGMGR->BobusangMgr_Init();

	// punish
	PUNISHMGR->Init();

	// Dump
#ifdef _HK_LOCAL_
	MiniDumper md(MiniDumper::DUMP_LEVEL_0);
#elif defined _TL_LOCAL_
	MiniDumper md(MiniDumper::DUMP_LEVEL_0);
#elif defined _CHINA_LOCAL_
	MiniDumper md(MiniDumper::DUMP_LEVEL_0);
#elif defined _JAPAN_LOCAL_
	MiniDumper md(MiniDumper::DUMP_LEVEL_0);
#else
	MiniDumper md(MiniDumper::DUMP_LEVEL_2);
#endif
	
	g_Console.LOG(4, "-----------   AGENT SERVER START  -----------------");
	// 이 부분은 윈도우의 메시지 루프.CONSOLE을 사용하든 윈도우 메시지 기반으로 짜든 어떤식으로든 이벤트를 대기해야하
	// 하므로 귀찮은 메시지 루프 작성을 하고 싶지 않다면 이 메소드를 사용한다.그 닳고 닳은 메시지 루프와 동일한 기능을 
	// 수행한다.이 메소드는 I4DyuchiCONSOLE 다이얼로그의 x버튼을 누를때까지 리턴하지 않는다.이 메소드가 리턴한다면 프
	// 로그램이 종료하는 것이다.
	
	g_bReady = TRUE;

//Get GMPowerList
//	GM_GetGMPowerList( 0, 0 );
	

	//에이전트를 킬 때 이 에이전트에 있는 유저를 로그인 테이블에서 지운다.
	char txt[128];
	sprintf(txt, "EXEC Up_Agent_Down %d, %d", g_nServerSetNum , g_pServerTable->GetSelfServer()->wServerNum);
	g_DB.LoginQuery(eQueryType_FreeQuery, eAgentDown, 0, txt);

	g_Console.WaitMessage();
}
void CServerSystem::Process()
{	// YH 현재 1초마다 한번씩 들어옴	
	// KES 현재 0.1마다로 바꿈 050113

	if(g_bCloseWindow)
	{
		SendMessage(g_hWnd, WM_CLOSE, 0,0);
		g_bCloseWindow = FALSE;
		g_bReady = FALSE;
	}

	MHTIMEMGR_OBJ->Process();
/*
#ifdef _HK_LOCAL_
//---TimeTest
	static DWORD dwLastTime = 0;
	if( dwLastTime != 0 )
	{
		m_dwFPSTimeTest = gCurTime - dwLastTime;
	}

	dwLastTime = gCurTime;
//
#endif
*/

//db que내용을 항상 살핀다.
	g_DB.ProcessingQueue();
//	g_DB.ProcessingDBMessage();
//

#ifndef _HK_LOCAL_
	TRAFFIC->Process();
#endif
	SHOUTMGR->Process();
	
	//SW050729

	JACKPOTMGR->Process();

	//SW070626 보부상NPC
	BOBUSANGMGR->BobusangMgr_Process();

	// punish
	PUNISHMGR->Process();

#ifdef _NPROTECT_
	NPROTECTMGR->Update();
#endif
}

void CServerSystem::ConnectionCheck()
{	// YH 현픸E30초마다 한번씩 들엉拜
	DWORD _60sec = 60*1000;
	USERINFO* pInfo;
	DWORD elapsedtime;
	static int nForBill = 0;

	if(g_bReady == FALSE)
		return;
	
	cPtrList removelist;
	cPtrList disconnectlist;

	g_pUserTableForUserID->SetPositionHead();
	while(pInfo = g_pUserTableForUserID->GetData())
	{
		if(pInfo->dwConnectionIndex == 0)
		{
			// 아직 접속이 제대로 이뤄지지 않은 경우
			elapsedtime = gCurTime - pInfo->dwLastConnectionCheckTime;
			if(elapsedtime > _60sec*2)
			{
				removelist.AddTail(pInfo);
			}
		}
		else
		{
			//1. 맵 로딩(맵이동)중의 시간은 어떻게 처리하나?
			//2. 캐릭터 선책창의 시간은 어떻게 처리하나?
#ifdef _CHINA_LOCAL_
			if( pInfo->bBillType == 8 || pInfo->bBillType == 7 )
			{
				DWORD dwElapsedRemainTime	= gCurTime - pInfo->dwLastCheckRemainTime;
				DWORD dwElapsedSec			= dwElapsedRemainTime/1000; //per sec
				pInfo->nRemainTime			-= dwElapsedSec;
				pInfo->dwLastCheckRemainTime += dwElapsedSec*1000;
		
				if( pInfo->nRemainTime <= 0 )
				{
					pInfo->nRemainTime = 0;
					disconnectlist.AddTail(pInfo);
					//DisconnectUser(pInfo->dwConnectionIndex, 8);
					continue;
				}
			}
#endif
			///////////---Billing

			if( nForBill == 1 )
				SendSpeedHackCheckMsg(pInfo);

			if(pInfo->UserLevel == eUSERLEVEL_PROGRAMMER)
				continue;

			elapsedtime = gCurTime - pInfo->dwLastConnectionCheckTime;
			
			if( elapsedtime > _60sec*10 )	//10min
			{
				// 접속이 이뤄진 경우
				if(pInfo->bConnectionCheckFailed)		// 만약 컨넥션 체크를 보냈는데 1분동안 응답이 없었다면
				{
					disconnectlist.AddTail(pInfo);
					//DisconnectUser(pInfo->dwConnectionIndex, 1);
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

		LoginCheckDelete(p->dwUserID);		// 로그인체크 테이블에서 삭제
		g_pUserTableForUserID->RemoveUser(p->dwUserID);
		TRAFFIC->RemoveUser(p->dwUserID);
#ifdef _NPROTECT_
		if( p->m_pCSA )
		{
			p->m_pCSA->Close();
			g_CCSAuth2Pool.Free(p->m_pCSA);

		}
#endif
		memset(p,0,sizeof(USERINFO));
		g_UserInfoPool.Free(p);
	}
	removelist.RemoveAll();

	pos = disconnectlist.GetHeadPosition();
	while( pos )
	{
		USERINFO* p = (USERINFO*)disconnectlist.GetNext(pos);
		DisconnectUser(p->dwConnectionIndex);
	}
	disconnectlist.RemoveAll();


	if( nForBill == 1 )		//2번에 한번... 즉, 1분마다.
	{
#ifdef _CHINA_LOCAL_
		BillingUpdateAll( m_wServerNum );
#endif
		nForBill = 0;
	}
	else
	{
		++nForBill;
	}
	
	// Plustime
#ifndef _TL_LOCAL_
	PLUSTIMEMGR->Process();
#endif
}

void CServerSystem::SendConnectionCheckMsg(USERINFO* pInfo)
{
	MSGBASE msg;
	msg.Category = MP_USERCONN;
	msg.Protocol = MP_USERCONN_CONNECTION_CHECK;
	g_Network.Send2User(pInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));

/*
	MSG_DWORD msg;
	msg.Category	= MP_USERCONN;
	msg.Protocol	= MP_USERCONN_CONNECTION_CHECK;
	msg.dwData		= gCurTime;
	g_Network.Send2User(pInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
*/
}

void CServerSystem::SendSpeedHackCheckMsg(USERINFO* pInfo)
{
	if( pInfo->wUserMapNum )
	{
		MSG_DWORD msg;
		msg.Category	= MP_HACKCHECK;
		msg.Protocol	= MP_HACKCHECK_SPEEDHACK;
		msg.dwData		= gCurTime;	//작은 시간을 주는 것은 괜찮다.
		g_Network.Send2User(pInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
	}

#ifdef _HACK_SHIELD_
	HACKSHIELDMGR->SendReq(pInfo);
#endif
#ifdef _NPROTECT_
	// 3분 간격으로 점검
//	static DWORD NProtectCheckTime = gCurTime;

	if( pInfo->m_nCSAInit == 1 || pInfo->m_nCSAInit == 2 ) //로그인시 처음 2번 인증 중
	{
		if( gCurTime - pInfo->dwLastNProtectCheck >= 1000*60 ) //60초내로 인증 값이 안오면
		{
			pInfo->m_nCSAInit = 4;	//접속 끊음.

			MSGBASE msg;
			msg.Category = MP_NPROTECT;
			msg.Protocol = MP_NPROTECT_DISCONNECT;
			g_Network.Send2User(pInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
			if( pInfo->UserLevel >= eUSERLEVEL_GM )
				DisconnectUser(pInfo->dwConnectionIndex);
		}
	}
	else if( pInfo->m_nCSAInit == 3 )
	{
		if(gCurTime - pInfo->dwLastNProtectCheck >= 1000*60*3)
		{
			NPROTECTMGR->SendAuthQuery(pInfo);
		}
	}
#endif
}

void CServerSystem::End()
{
	g_bReady = FALSE;

	//TRAFFIC->EndRecord();
	TRAFFIC->End();

//	UnhookWindowsHookEx(hHook);

	
	g_pUserTableForUserID->SetPositionHead();
	USERINFO* pInfo;
	while(pInfo = g_pUserTableForUserID->GetData())
	{
		LoginCheckDelete(pInfo->dwUserID);
		g_pServerSystem->ReleaseAuthKey(pInfo->dwUniqueConnectIdx);

#ifdef _NPROTECT_
		pInfo->m_pCSA->Close();
		g_CCSAuth2Pool.Free(pInfo->m_pCSA);
#endif
		g_UserInfoPool.Free(pInfo);
	}
	g_pUserTableForUserID->RemoveAll();

//	ICRelease(m_IdxCreater);

	// punish
	PUNISHMGR->Release();
	
	g_DB.Release();
	g_Network.Release();
	g_Console.Release();

	if(g_pUserTable)
	{
		g_pUserTable->RemoveAllUser();
		delete g_pUserTable;
		g_pUserTable = NULL;
	}
	if(g_pUserTableForObjectID)
	{
		g_pUserTableForObjectID->RemoveAllUser();
		delete g_pUserTableForObjectID;
		g_pUserTableForObjectID = NULL;
	}
	if(g_pServerTable)
	{
		g_pServerTable->RemoveAllServer();
		delete g_pServerTable;
		g_pServerTable = NULL;
	}
	CoFreeUnusedLibraries();
}

void CServerSystem::SetMapChangeInfo()
{
	char filename[64];
	CMHFile file;
#ifdef _FILE_BIN_
	sprintf(filename,"./Resource/MapChange.bin");
	if(!file.Init(filename, "rb"))
		return;
#else
	sprintf(filename,"./Resource/MapChange.txt");
	if(!file.Init(filename, "rt"))
		return;
#endif

	int n =0;
	while(1)
	{
		if(file.IsEOF())
			break;
		
		m_MapChange[n].Kind = file.GetWord();
//		file.GetString(m_MapChange[n].CurMapName);
//		file.GetString(m_MapChange[n].ObjectName);
		SafeStrCpy( m_MapChange[n].CurMapName, file.GetString(), MAX_NAME_LENGTH+1 );
		SafeStrCpy( m_MapChange[n].ObjectName, file.GetString(), MAX_NAME_LENGTH+1 );
		m_MapChange[n].CurMapNum = file.GetWord();
		m_MapChange[n].MoveMapNum = file.GetWord();
		m_MapChange[n].CurPoint.x = file.GetFloat();
		m_MapChange[n].CurPoint.y = 0;
		m_MapChange[n].CurPoint.z = file.GetFloat();
		m_MapChange[n].MovePoint.x = file.GetFloat();
		m_MapChange[n].MovePoint.y = 0;
		m_MapChange[n].MovePoint.z = file.GetFloat();
		m_MapChange[n].chx_num = file.GetWord();
		
		++n;
	}
	
	file.Release();
}

MAPCHANGE_INFO* CServerSystem::GetMapChangeInfo(DWORD Index)
{
	for(int i=0; i<MAX_POINT_NUM; i++)
	{
		if(m_MapChange[i].Kind == Index)
			return &m_MapChange[i];
	}

	return NULL;
}

//맵서버 다운시 다운된 맵에 있는 유저에게 맵다운을 알림
void CServerSystem::SendMsgMapServerDown( WORD wServerNum )
{
	USERINFO* pInfo;
	g_pUserTable->SetPositionUserHead();

	while( pInfo = g_pUserTable->GetUserData() )
	{
		if( pInfo->wUserMapNum == wServerNum )
		{
//			MSGBASE msg;
//			msg.Category = MP_USERCONN;
//			msg.Protocol = MP_USERCONN_MAPSERVERDOWN_NOTIFY;
									
			//접속 종료를 알림//나중에 맵서버 다운되어도 플레이 가능시에
//			g_Network.Send2User( pinfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );

			//agent에서 끊는다.
//			DisconnectUser(pInfo->dwConnectionIndex);
			DisconnectUser(pInfo->dwConnectionIndex);
			
		}
	}
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
	if( file.Init( "LocalizingInfo.txt", "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
	{

		if( strcmp( file.GetString(), "*NATION" ) == 0 )
		{
			if( strcmp( file.GetString(), "CHINA" ) == 0 )
			{
				m_Nation = eNATION_CHINA;
			}
		}

		file.Release();
	}

	CMHFile file2;
	if( file2.Init( "_TESTSERVER", "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
	{
		m_bTestServer = TRUE;
		file2.Release();
	}
}


void CServerSystem::SetEventNotifyStr( char* pStrTitle, char* pStrContext )
{
	SafeStrCpy( m_strEventNotifyTitle, pStrTitle, 32 );
	SafeStrCpy( m_strEventNotifyContext, pStrContext, 128 );
}

void CServerSystem::SetUseEventNotify( BOOL bUse )
{
	m_bUseEventNotify = bUse;

	if( bUse )
	{
		USERINFO* pInfo;
		g_pUserTable->SetPositionUserHead();

		MSG_EVENTNOTIFY_ON msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_EVENTNOTIFY_ON;
		SafeStrCpy( msg.strTitle, m_strEventNotifyTitle, 32 );
		SafeStrCpy( msg.strContext, m_strEventNotifyContext, 128 );

		for( int i=0; i<eEvent_Max; ++i )
		{
			if( m_bEvenList[i] )				msg.EventList[i] = 1;
			else								msg.EventList[i] = 0;
		}

		while( pInfo = g_pUserTable->GetUserData() )
		{
			MSG_EVENTNOTIFY_ON msgTemp = msg;
			g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msgTemp, sizeof(msgTemp) );
		}
	}
	else
	{
		MSGBASE msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_EVENTNOTIFY_OFF;

		USERINFO* pInfo;
		g_pUserTable->SetPositionUserHead();

		while( pInfo = g_pUserTable->GetUserData() )
		{
			MSGBASE msgTemp = msg;
			g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msgTemp, sizeof(msgTemp) );
		}
	}
}

#ifdef _CHINA_LOCAL_
void CServerSystem::SendBillingRemainTimeMsg( USERINFO* pInfo )
{
	if( pInfo->nRemainTime < 0 ) pInfo->nRemainTime = 0;

	//counting start;
	MSG_INT msgTime;
	msgTime.Category	= MP_USERCONN;
	msgTime.Protocol	= MP_USERCONN_REMAINTIME_NOTIFY;
	msgTime.nData		= pInfo->nRemainTime;
		
	g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msgTime, sizeof(msgTime) );	

}
#endif
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

	char strr[255];
	wsprintf(strr, ""%d 번 서버로 접속성공", dwConnectionIndex);//pjslocal
	g_Console.LOG(4, strr);
	

	info->dwConnectionIndex = dwConnectionIndex;					// 이게 제대로 동작하나? 체크
	MSG_WORD msg;
	msg.Category = MP_SERVER;
	msg.Protocol = MP_SERVER_PWRUP;
	msg.wData = g_pServerTable->GetSelfServer()->wPortForServer;
	g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));*/


	
	//YH2DO 주석처리 되어있던 것을 지움
	if(g_pServerTable->GetMaxServerConnectionIndex() < dwConnectionIndex)			//??????
		g_pServerTable->SetMaxServerConnectionIndex(dwConnectionIndex);	
}

void OnDisconnectServer(DWORD dwConnectionIndex)
{
	char strr[255];
	
	SERVERINFO* serverinfo = g_pServerTable->FindServerForConnectionIndex(dwConnectionIndex);

	wsprintf(strr, "%d 번 connectionIndex 서버 접속 끊김", dwConnectionIndex);//pjslocal
	g_Console.LOG(4, strr);

	// 여기다 너두 돼나??------------------------------------------------------------------------
//	if( serverinfo )
//	if( serverinfo->wServerKind == AGENT_SERVER )	//Agent서버인경우
//	{
		//다시접속...
//		SERVERINFO Info;
//		g_Network->ConnectToServer( serverinfo->szIPForServer, serverinfo->wPortForServer, (void*)&Info );
//		return;
//	}
	//-------------------------------------------------------------------------------------------

	if(serverinfo)
	if( serverinfo->wServerKind == 3 )	//map서버인경우
	{
		//맵서버에 연결된 유저에게 맵서버 다운을 알림
		g_pServerSystem->SendMsgMapServerDown(serverinfo->wServerNum);
	}

	SERVERINFO * delInfo = g_pServerTable->RemoveServer(dwConnectionIndex);
	if(!delInfo)
	{
		ASSERT(0);
	}
	delete delInfo;
}


void OnConnectServerFail(void* pVoid)
{
	SERVERINFO* info = (SERVERINFO*)pVoid;
	if(info->wServerKind == MONITOR_SERVER)
	{
		BOOTMNGR->AddBootListINI(AGENT_SERVER, g_pServerSystem->GetServerNum(), g_pServerTable);
		BOOTMNGR->BactchConnectToMap(&g_Network, g_pServerTable);
		g_Console.LOG(4, "Failed to Connect to the MS : %s, %d", info->szIPForServer, info->wPortForServer);
	}
	else
	{
		g_Console.LOG(4, "Failed to Connect to the Server : %s, %d", info->szIPForServer, info->wPortForServer);
		BOOTMNGR->RemoveBootList(g_pServerTable, info->wPortForServer);
	}	
		
}
void OnAcceptServer(DWORD dwConnectionIndex)
{
	g_Console.LOG(4, "Server Connected : ConnectionIndex %d", dwConnectionIndex );
}

void OnAcceptUser(DWORD dwConnectionIndex)
{
	if(g_bReady == FALSE)
	{

		return;
	}
	
	++g_dwAcceptUser;

	MSGBASE send;
	send.Category = MP_USERCONN;
	send.Protocol = MP_USERCONN_AGENT_CONNECTSUCCESS;
	g_Network.Send2User(dwConnectionIndex, (char*)&send, sizeof(send));

	//event공지
	if( g_pServerSystem->IsUseEventNotify() )
	{
		MSG_EVENTNOTIFY_ON msg;
		msg.Category	= MP_CHEAT;
		msg.Protocol	= MP_CHEAT_EVENTNOTIFY_ON;
		SafeStrCpy( msg.strTitle, g_pServerSystem->GetEventNotifyTitle(), 32 );
		SafeStrCpy( msg.strContext, g_pServerSystem->GetEventNotifyContext(), 128 );

		// magi82(43)
		BOOL* pEventlist = g_pServerSystem->GetApplyEvent();

		for( int i=0; i<eEvent_Max; ++i )
		{
			if( pEventlist[i] )				msg.EventList[i] = 1;
			else							msg.EventList[i] = 0;
		}
				
		g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
	}	
	
	//user count 계산=============================================================================
	SERVERINFO * myInfo = g_pServerTable->GetSelfServer();
	++myInfo->wAgentUserCnt;

	MSG_WORD2 msg2;
	msg2.Category = MP_SERVER;
	msg2.Protocol = MP_SERVER_USERCNT;
	msg2.wData1 = myInfo->wPortForServer;
	msg2.wData2 = myInfo->wAgentUserCnt;
	
	g_pServerTable->SetPositionHead();
	SERVERINFO* pOtherAgentInfo = NULL;
	while(pOtherAgentInfo = g_pServerTable->GetNextDistServer())
	{
		if(myInfo == pOtherAgentInfo)
			continue;

		g_Network.Send2Server(pOtherAgentInfo->dwConnectionIndex, (char *)&msg2, sizeof(msg2));
	}
	//user count 계산=============================================================================
	
	

	//에이젼트 서버이면 유저 카운터를 증가시킨다.
	DWORD CurUserCount = g_pUserTable->GetUserCount();
	g_pUserTable->SetCalcMaxCount(CurUserCount);
	DWORD MaxUserCount = g_pUserTable->GetUserMaxCount();
	SetUserCurAndMaxCount(CurUserCount, MaxUserCount);
	
	

}

void OnDisconnectUser(DWORD dwConnectionIndex)
{
	if(g_bReady == FALSE)
		return;

//	g_Console.LOG(4, "  User Disconnected : ConnectionIndex %d", dwConnectionIndex);

	g_pUserTable->OnDisconnectUser(dwConnectionIndex);

//	g_Console.LOG(4, "UserCount %d   %d   %d",
//		g_pUserTableForUserID->GetUserCount(),
//		g_pUserTable->GetUserCount(),
//		g_pUserTableForObjectID->GetUserCount());

}

void ProcessDBMessage()
{
	printf("Agent : ProcessDBMessage\n");
	g_DB.ProcessingDBMessage();
}

void ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	MSGROOT* pTempMsg = reinterpret_cast<MSGROOT*>(pMsg);
	ASSERT(g_pServerMsgParser[pTempMsg->Category] != NULL);
	
//KES
	if( pTempMsg->Category <= 0 || pTempMsg->Category >= MP_MAX ||
		g_pServerMsgParser[pTempMsg->Category] == NULL )
	{
		char buf[128];
		sprintf( buf, "Category error : %d", pTempMsg->Category );
		ASSERTMSG( 0, buf );
		return;
	}
//	g_Console.LOG( 4, "Category : %d, Protocol : %d", pTempMsg->Category, pTempMsg->Protocol );

	g_pServerMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);
}

void ReceivedMsgFromUser(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	MSGROOT* pTempMsg = reinterpret_cast<MSGROOT*>(pMsg);

	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);


	// RaMa 메세지를 보낸유저 인지 확인한다.
	//MSGBASE* pcMsg = reinterpret_cast<MSGBASE*>(pMsg);
	if( pInfo )
	{
		USERINFO* pcInfo = g_pUserTableForUserID->FindUser( pInfo->dwUserID );
		if( pcInfo )
		{
			if( pInfo->DistAuthKey != pcInfo->DistAuthKey )
				return;
		}
	}

	/*
	if( pInfo->dwCharacterID != pcMsg->dwObjectID )
	{
		if( pcMsg->Category != MP_USERCONN &&
		pcMsg->Protocol != MP_USERCONN_GAMEIN_SYN )
			return;
	}
	*/


	if( pInfo )
	if( g_pServerSystem->IsEnableCrypt() )
	{
		int headerSize = sizeof( MSGROOT );

		if( !pInfo->crypto.Decrypt( ( char * )pTempMsg +  headerSize, dwLength - headerSize ) )
		{
			ASSERTMSG(0,"Decrypt Error");
			OnDisconnectUser( dwConnectionIndex );
			DisconnectUser( dwConnectionIndex );
			return;
		}
		char aaa = pInfo->crypto.GetDeCRCConvertChar();
		if( pTempMsg->Code !=  aaa )
		{
			ASSERTMSG(0,"Decrypt CRC Error");
			OnDisconnectUser( dwConnectionIndex );
			DisconnectUser( dwConnectionIndex );
			return;
		}

		TRAFFIC->AddUserPacket( pInfo->dwUserID, pTempMsg->Category );
	}

	ASSERT(g_pUserMsgParser[pTempMsg->Category] != NULL);
	ASSERT(pTempMsg->Category < MP_MAX);
	ASSERT(pTempMsg->Category != 0);
	if( g_pUserMsgParser[pTempMsg->Category] == NULL ||
		pTempMsg->Category >= MP_MAX ||
		pTempMsg->Category == 0)
		return;
/*
	//eTraffic_User_To_Agent
	if( pTempMsg->Category == MP_MOVE )
	{
		TRAFFIC->AddReceiveMovePacket( ((MSGBASE*)pTempMsg)->dwObjectID, pTempMsg->Protocol, dwLength );
	}

	TRAFFIC->AddReceivePacket( pTempMsg->Category, dwLength );
*/
	g_pUserMsgParser[pTempMsg->Category](dwConnectionIndex, pMsg, dwLength);
}

void GameProcess()
{
	g_pServerSystem->Process();
}

void fConnectionCheck()
{
	g_pServerSystem->ConnectionCheck();
}


// console interface function
void ButtonProc1()
{
/*	g_bAssertMsgBox = !g_bAssertMsgBox;
	if(g_bAssertMsgBox)
		g_Console.LOG(4,"Assert MsgBox is On");
	else
		g_Console.LOG(4,"Assert MsgBox is Off");
*/	

	int servercount = g_pServerTable->GetDataNum();
	g_Console.LOG(4, "Connected Server Count : %d", servercount);

	char temp[128] = {0,};
	SERVERINFO* pInfo = NULL;
	g_pServerTable->SetPositionHead();
	while( pInfo = g_pServerTable->GetNextServer() )
	{
		switch( pInfo->wServerKind )
		{
		case DISTRIBUTE_SERVER:	sprintf( temp, "DistributeServer" ); break;
		case AGENT_SERVER:		sprintf( temp, "AgentServer" );	break;
		case MAP_SERVER:		sprintf( temp, "MapServer" );	break;		
		}
		g_Console.LOG(4, "%s : IP = %s, Port = %d", temp, pInfo->szIPForServer, pInfo->wPortForServer );
	}
}

void ButtonProc2()
{
	g_Console.LOG(4, "UserCount %d %d %d, AcceptUser %d %d [ USERIDX[%d, %d] UT[%d, %d] OBJECTIDX[%d, %d]]",
		g_pUserTableForUserID->GetUserCount(),
		g_pUserTable->GetUserCount(),
		g_pUserTableForObjectID->GetUserCount(),
		g_dwAcceptUser, g_dwAcceptUser2, 
		g_pUserTableForUserID->GetAddCount(), 
		g_pUserTableForUserID->GetRemoveCount(), 
		g_pUserTable->GetAddCount(), 
		g_pUserTable->GetRemoveCount(),
		g_pUserTableForObjectID->GetAddCount(), 
		g_pUserTableForObjectID->GetRemoveCount() );
}

void ButtonProc3()
{
/*
#ifdef _HK_LOCAL_
	if( g_pServerSystem )
	{
		g_Console.LOG(4, "Agent FPS : %d, Dealy: %d",
			g_pServerSystem->GetFPSTimeTest(), g_pServerSystem->GetDBProcessDelay() );
	}
#endif
*/
	g_Console.LOG(4, "INetwork GetConnectedUserNum() : %d", g_Network.GetINet()->GetConnectedUserNum());
}

void OnCommand(char* szCommand)
{
	strupr(szCommand);
	char Header[256] = {0,};	
	sscanf(szCommand,"%s",Header);
	START_STRING_COMPARE(Header)
		COMPARE("FINDUSER")
			DWORD UserID;
			sscanf(szCommand,"%s %d",Header,&UserID);
			USERINFO* pInfo = g_pUserTableForUserID->FindUser(UserID);
			if(pInfo == NULL)
				g_Console.Log(eLogDisplay,4,"No User (%d)",UserID);
			else
			{
				g_Console.Log(eLogDisplay,4,"UserID:%d  ConIdx:%d  DistAuthKey:%d  AgentAuthKey:%d",
					UserID,pInfo->dwConnectionIndex,pInfo->DistAuthKey,pInfo->dwUniqueConnectIdx);
			}
	END_COMPARE

}



