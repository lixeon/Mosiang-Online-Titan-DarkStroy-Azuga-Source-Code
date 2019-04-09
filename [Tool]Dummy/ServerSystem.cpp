// ServerSystem.cpp: implementation of the CServerSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ServerSystem.h"
#include "Console.h"
#include "Network.h"

#include <MemoryPoolTempl.h>
#include "DummyNetwork.h"
#include <HashTable.h>
#include "StateMng.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CServerSystem*	g_pServerSystem;
CYHHashTable<CDummyNetwork>	g_DNTable;
CStateMng**		g_ppStateManager;
int				g_ClientNumPerProcess = 100;
extern DWORD g_ConnectDist;
extern DWORD g_DisconnectDist;
extern DWORD g_ConnectAgent;
extern DWORD g_DisconnectAgent;
extern DWORD g_ReceiveLoginAck;
extern DWORD g_TryToConnectAgent;
extern DWORD g_DisconInMap;
extern DWORD g_DisconIntoMap;
extern DWORD g_MapChangeCount;
extern DWORD g_MapChangeAck; 


CServerSystem::CServerSystem()
{
	CoInitialize(NULL);
	m_bEnableStart = FALSE;
	m_bEnableMapChange = FALSE;
	m_bEnableAttack = FALSE;
	m_bEnableChat = FALSE;

	m_bEnterAgent = FALSE;
}

CServerSystem::~CServerSystem()
{
	CoUninitialize();
}

BOOL CServerSystem::Start( int sId, int eId, int channel )
{
	if( sId <= 0 || sId > eId )
		return FALSE;

	// 콘솔 초기화
	// 버튼 설정.외부입력용 버튼이 28개 준비되어있다.한개도 사용하지 않을 경우 pCustomButton 필드를 NULL로 채운다.
	MENU_CUSTOM_INFO cbutton[10];						// 외부 입력을 위한 버튼을 설정한다.
	
	cbutton[0].cbMenuCommand = ButtonProc0;			// 버튼이 눌렸을때의 콜백함수를 세팅
	strcpy(cbutton[0].szMenuName, "State");	// 버튼의 캡션을 설정한다.
	
	cbutton[2].cbMenuCommand = ButtonProc1;
	strcpy(cbutton[2].szMenuName, "Enter");
	
	cbutton[1].cbMenuCommand = ButtonProc2;
	strcpy(cbutton[1].szMenuName, "Start");

	cbutton[3].cbMenuCommand = ButtonProc3;
	strcpy(cbutton[3].szMenuName, "Map");
	
	cbutton[4].cbMenuCommand = ButtonProc4;
	strcpy(cbutton[4].szMenuName, "Attack");
	
	cbutton[5].cbMenuCommand = ButtonProc5;
	strcpy(cbutton[5].szMenuName, "LP Save");
	
	cbutton[6].cbMenuCommand = ButtonProc6;
	strcpy(cbutton[6].szMenuName, "Chat");

	cbutton[7].cbMenuCommand = ButtonProc7;
	strcpy(cbutton[7].szMenuName, "Error!!");

	cbutton[8].cbMenuCommand = ButtonProc8;
	strcpy(cbutton[8].szMenuName, "Count");

	cbutton[9].cbMenuCommand = ButtonProc9;
	strcpy(cbutton[9].szMenuName, "MapChange");
		
	if( !g_Console.Init( 10, cbutton, OnCommand ) )
		MessageBox( NULL, "Console initializing Fail", 0, 0 ) ;

	char str[256];
	sprintf( str, "New Dummy : %d - %d : %d", sId, eId, channel );
	SetWindowText( GetActiveWindow(), str );

	g_Console.LOG( 4, "-----------   New Dummy Client Start  -----------------" );
	
	// Network init
	CUSTOM_EVENT ev[1];
	ev[0].dwPeriodicTime = 1000;
	ev[0].pEventFunc = GameProcess;
	
	DESC_NETWORK desc;
	desc.OnAcceptServer = OnAcceptServer;
	desc.OnDisconnectServer = OnDisconnectServer;
	desc.OnAcceptUser = OnAcceptUser;
	desc.OnDisconnectUser = OnDisconnectUser;
	desc.OnRecvFromServerTCP = ReceivedMsgFromServer;
	desc.OnRecvFromUserTCP = ReceivedMsgFromUser;
	desc.dwCustomDefineEventNum = 1;
	desc.pEvent = ev;

	desc.dwMainMsgQueMaxBufferSize = 20480000;
	desc.dwMaxServerNum = 50;
	desc.dwMaxUserNum = 4000;
	desc.dwServerBufferSizePerConnection = 512000;
	desc.dwServerMaxTransferSize = 65000;
	desc.dwUserBufferSizePerConnection = 512000;
	desc.dwUserMaxTransferSize = 65000;
	desc.dwConnectNumAtSameTime = 4000;
//	desc.dwFlag = NULL;
	desc.dwFlag = NETDDSC_DEBUG_LOG;
	
	g_Network.Init( &desc );

	RESMNG->Init();						// resource

	g_DNTable.Initialize( 4000 );		// Dummy Init

	g_ClientNumPerProcess = eId-sId+1;	// id setting
	g_ppStateManager = new CStateMng*[g_ClientNumPerProcess];

	DWORD time = 0;
	for( int n = 0; n < g_ClientNumPerProcess; ++n )
	{
		g_ppStateManager[n] = new CStateMng;
		g_ppStateManager[n]->Init( sId+n, channel );
		g_ppStateManager[n]->m_dwLoginTime = n+1;
	}

	g_Console.WaitMessage();

	return TRUE;
}

void CServerSystem::Process()
{
	if( g_ppStateManager == NULL )
		return;
	
	for(int n = 0; n < g_ClientNumPerProcess; ++n )
	{				
		if( g_ppStateManager[n] == NULL )
			continue;
		
		if( g_ppStateManager[n]->Process() != -1 )
			continue;
		else
		{
			g_ppStateManager[n]->Exit();
			delete g_ppStateManager[n];
			g_ppStateManager[n] = NULL;
		}
	}
}

void CServerSystem::End()
{
	if( g_ppStateManager )
	{
		for( int n = 0; n < g_ClientNumPerProcess; ++n )
		{
			if( g_ppStateManager[n] )
				g_ppStateManager[n]->ChangeState( eGAMESTATE_END, NULL, 0 );
		}
		SAFE_DELETE_ARRAY( g_ppStateManager );
	}

	RESMNG->Exit();
}


// global function
void OnConnectToServerSuccess(DWORD dwConnectionIndex, void* pVoid)
{
	CDummyNetwork* pNetwork = (CDummyNetwork*)pVoid;
	pNetwork->OnConnected(dwConnectionIndex);
	g_DNTable.Add(pNetwork,dwConnectionIndex);
}

void OnDisconnectServer(DWORD dwConnectionIndex)
{
	ASSERT(0);
}

void OnConnectToServerFail(void* pVoid)
{
	CDummyNetwork* pNetwork = (CDummyNetwork*)pVoid;
	pNetwork->OnConnectFailed();
}

void OnAcceptServer(DWORD dwConnectionIndex)
{
	ASSERT(0);
}

void OnAcceptUser(DWORD dwConnectionIndex)
{
	ASSERT(0);
}

void OnDisconnectUser(DWORD dwConnectionIndex)
{
	CDummyNetwork* pNetwork = g_DNTable.GetData(dwConnectionIndex);
	if(pNetwork == NULL)
		return;
	pNetwork->OnDisconnect();
	g_DNTable.Remove(dwConnectionIndex);	
}

void ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	ASSERT(0);
}

void ReceivedMsgFromUser(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	CDummyNetwork* pNetwork = g_DNTable.GetData(dwConnectionIndex);
	if(pNetwork == NULL)
		return;

	MSGBASE* pmsg = (MSGBASE*)pMsg;
	pNetwork->NetworkMsgParse(pmsg->Category,pmsg->Protocol,pmsg);
}

void GameProcess()
{
	g_pServerSystem->Process();
}

// console interface function
void ButtonProc1()
{
	for( int i = 0; i < g_ClientNumPerProcess; ++i )
	{
		if( g_ppStateManager[i] )
			g_ppStateManager[i]->EnterGame();
	}

	g_Console.Log( eLogDisplay, 4, "-------------------------- Enter Game Map ---------------------------" );
}

void ButtonProc2()
{
	g_pServerSystem->m_bEnableStart = !g_pServerSystem->m_bEnableStart;
	for( int i = 0; i < g_ClientNumPerProcess; ++i )
	{
		if( g_ppStateManager[i] )
			g_ppStateManager[i]->EnableStart( g_pServerSystem->m_bEnableStart );
	}

	if( g_pServerSystem->m_bEnableStart )
		g_Console.Log( eLogDisplay, 4, "-------------------------- Start ---------------------------" );
	else
		g_Console.Log( eLogDisplay, 4, "--------------------------- Stop ---------------------------" );
}

void ButtonProc3()
{
	g_pServerSystem->m_bEnableMapChange = !g_pServerSystem->m_bEnableMapChange;
	for( int i = 0; i < g_ClientNumPerProcess; ++i )
	{
		if( g_ppStateManager[i] )
			g_ppStateManager[i]->EnableMapChange( g_pServerSystem->m_bEnableMapChange );
	}

	if( g_pServerSystem->m_bEnableMapChange )
		g_Console.Log( eLogDisplay, 4, "--------------------------- Enable MapChange ---------------------------" );
	else
		g_Console.Log( eLogDisplay, 4, "--------------------------- Disable MapChange ---------------------------" );
}

void ButtonProc4()
{
	g_pServerSystem->m_bEnableAttack = !g_pServerSystem->m_bEnableAttack;
	for( int i = 0; i < g_ClientNumPerProcess; ++i )
	{
		if( g_ppStateManager[i] )
			g_ppStateManager[i]->EnableAttack( g_pServerSystem->m_bEnableAttack );
	}

	if( g_pServerSystem->m_bEnableAttack )
		g_Console.Log( eLogDisplay, 4, "--------------------------- Enable Attack ---------------------------" );
	else
		g_Console.Log( eLogDisplay, 4, "--------------------------- Disable Attack ---------------------------" );
}

void ButtonProc5()
{
	for( int i = 0; i < g_ClientNumPerProcess; ++i )
	{
		if( g_ppStateManager[i] )
			g_ppStateManager[i]->SaveLoginPoint();
	}

	g_Console.Log( eLogDisplay, 4, "--------------------------- Save LoginPoint ---------------------------" );
}

void ButtonProc0()
{
	int total = 0;
	int connect = 0;
	int title = 0;
	int select = 0;
	int make = 0;
	int loading = 0;
	int mapchange = 0;
	int gamein = 0;
	int end = 0;

	int state;

	for( int i = 0; i < g_ClientNumPerProcess; ++i )
	{
		if( g_ppStateManager[i] )
		{
			++total;

			state = g_ppStateManager[i]->GetCurState();
			switch( state )
			{
			case eGAMESTATE_END:			++end;			break;
			case eGAMESTATE_CONNECT:		++connect;		break;
			case eGAMESTATE_TITLE:			++title;		break;
			case eGAMESTATE_CHARSELECT:		++select;		break;
			case eGAMESTATE_CHARMAKE:		++make;			break;
			case eGAMESTATE_GAMELOADING:	++loading;		break;
			case eGAMESTATE_GAMEIN:			++gamein;		break;
			case eGAMESTATE_MAPCHANGE:		++mapchange;	break;
			}
		}
	}

	g_Console.Log( eLogDisplay, 4, "Total:%d, Connect:%d, Title:%d, Select:%d, Make:%d, Loading:%d, Gamein:%d, MapChange:%d, End:%d",
				   total, connect, title, select, make, loading, gamein, mapchange, end );
}

void ButtonProc6()
{
	g_pServerSystem->m_bEnableChat = !g_pServerSystem->m_bEnableChat;
	for( int i = 0; i < g_ClientNumPerProcess; ++i )
	{
		if( g_ppStateManager[i] )
			g_ppStateManager[i]->EnableChatting( g_pServerSystem->m_bEnableChat );
	}

	if( g_pServerSystem->m_bEnableChat )
		g_Console.Log( eLogDisplay, 4, "--------------------------- Enable Chat ---------------------------" );
	else
		g_Console.Log( eLogDisplay, 4, "--------------------------- Disable Chat ---------------------------" );
}

void ButtonProc7()
{
	int state;

	for( int i = 0; i < g_ClientNumPerProcess; ++i )
	{
		if( g_ppStateManager[i] )
		{
			state = g_ppStateManager[i]->GetCurState();
			switch( state )
			{
			case eGAMESTATE_TITLE:
				g_Console.Log( eLogDisplay, 4, "Dummy%d(UserIDx%d)%d is MainTitle!!",
							   g_ppStateManager[i]->m_nID, g_ppStateManager[i]->m_Network.m_ConnectionIdx,
							   g_ppStateManager[i]->m_dwUserIDx );
				break;
			case eGAMESTATE_GAMELOADING:
				g_Console.Log( eLogDisplay, 4, "Dummy%d(UserIDx%d)%d is GameLoading!!",
							   g_ppStateManager[i]->m_nID, g_ppStateManager[i]->m_Network.m_ConnectionIdx,
							   g_ppStateManager[i]->m_dwUserIDx );
				break;
			}
		}
	}
}

void OnCommand(char* szCommand)
{
	char temp[256];
	sscanf( szCommand, "%s", temp );

	int mapnum = atoi( temp );

	if( mapnum > 0 && mapnum < 100 )
	{
		for( int i = 0; i < g_ClientNumPerProcess; ++i )
		{
			if( g_ppStateManager[i] )
				g_ppStateManager[i]->ForcedMapChange( mapnum );
		}
	}
	
	g_Console.Log( eLogDisplay, 4, "--------------------------- All MapChange ---------------------------" );
}


void ButtonProc8()
{
	g_Console.Log( eLogDisplay, 4,
	"Dist Con : %d, Dist Discon : %d, LoginAck : %d, TryToAgent : %d, Agent Con : %d, Agent Discon : %d",
	g_ConnectDist, g_DisconnectDist, g_ReceiveLoginAck, g_TryToConnectAgent, g_ConnectAgent, g_DisconnectAgent );

	g_Console.Log( eLogDisplay, 4,
	"TryToMap Discon : %d, InMap Discon : %d",
	g_DisconIntoMap, g_DisconInMap );
}

void ButtonProc9()
{
/*	g_pServerSystem->m_bEnterAgent = !g_pServerSystem->m_bEnterAgent;
	for( int i = 0; i < g_ClientNumPerProcess; ++i )
	{
		if( g_ppStateManager[i] )
			g_ppStateManager[i]->m_bEnterAgent = g_pServerSystem->m_bEnterAgent;
	}

	if( g_pServerSystem->m_bEnterAgent )
		g_Console.Log( eLogDisplay, 4, "-------------------------- Start ---------------------------" );
	else
		g_Console.Log( eLogDisplay, 4, "--------------------------- Stop ---------------------------" );
		*/
	g_Console.Log( eLogDisplay, 4, "MapChange Count : %d, MapChange Ack : %d", g_MapChangeCount, g_MapChangeAck );
}