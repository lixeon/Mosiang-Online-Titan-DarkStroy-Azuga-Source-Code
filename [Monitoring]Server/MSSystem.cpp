// MSSystem.cpp: implementation of the CMSSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MSSystem.h"
#include "MonitorNetwork.h"
#include "MonitorServerTable.h"
#include "MSManager.h"
#include "MonitorUserTable.h"
#include "MASManager.h"
#include "DataBase.h"
#include "CMSDBMsgParser.h"
#include < winsock2.h >
#include "UserIPCheckMgr.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define	AUTOPATCH	"DarkStoryOnline"	// ¿ÀÅäÆÐÄ¡CaptionÀÌ¸§

extern HWND gMainHwnd;

BOOL bAutoPatch = FALSE;

CMSSystem::CMSSystem()
{
	m_wServerType = TYPE_NONE;
	m_bInit = FALSE;
}

CMSSystem::~CMSSystem()
{
}
	
void CMSSystem::Start(WORD wServerType)
{
	m_wServerType = wServerType;
	DESC_NETWORK desc;

	char host[128],ipName[2][20];
	HOSTENT* hostent;
	
    WSADATA wsaData ;
	WSAStartup(0x0002,&wsaData);
	gethostname(host,128);
	hostent=gethostbyname(host);
	
	//inet_ntoa ÇÔ¼ö´Â msdn¿¡¼­ Ã£¾Æº¸±æ...
	for(int n=0;n<2;++n)
	{
		if(hostent->h_addr_list[n] == NULL)
			break;

		strncpy(ipName[n],inet_ntoa(*(struct in_addr *)hostent->h_addr_list[n]),20);
	}

	WSACleanup();
	
	char temp[256] = {0, };
	ySWITCH(m_wServerType)
	yCASE(TYPE_MS)
		{
			sprintf( temp, "MS_%s", SERVERTOOLVERSION );
			SetWindowText(GetActiveWindow(), temp );

			LOG( "MServerTable->LoadMonitorInfo()" );

			// ¼­¹ö Á¤º¸ ·Îµå
			MServerTable->LoadMonitorInfo(TYPE_MS,ipName[0],ipName[1]);

			LOG( "End MServerTable->LoadMonitorInfo()" );

			CUSTOM_EVENT ev[1];
			ev[0].dwPeriodicTime = 100;
			ev[0].pEventFunc = CMSSystem::Process;
	
			desc.OnAcceptServer = OnAcceptServer;
			desc.OnDisconnectServer = OnDisconnectServer;
			desc.OnAcceptUser = NULL;
			desc.OnDisconnectUser = NULL;
			desc.OnRecvFromServerTCP = ReceivedMsgFromServer;
			desc.OnRecvFromUserTCP = NULL;
			desc.dwCustomDefineEventNum = 1;
			desc.pEvent = ev;

			desc.dwMainMsgQueMaxBufferSize = 512000;
			desc.dwMaxServerNum = 50;
			desc.dwMaxUserNum = 0;
			desc.dwServerBufferSizePerConnection = 25600;
			desc.dwServerMaxTransferSize = 6500;
			desc.dwUserBufferSizePerConnection = 0;
			desc.dwUserMaxTransferSize = 0;
			desc.dwConnectNumAtSameTime = 100;
			desc.dwFlag = NULL;

			MNETWORK->Init(&desc);
			MNETWORK->ConnectToMAS();
		}
	yCASE(TYPE_MAS)
		{
			sprintf( temp, "MAS_%s", SERVERTOOLVERSION );
			SetWindowText(GetActiveWindow(), temp );

			// ¼­¹ö Á¤º¸ ·Îµå
			MServerTable->LoadMonitorInfo(TYPE_MAS,ipName[0],ipName[1]);
			MASMGR->LoadMapServerInfo();
			CUSTOM_EVENT ev[1];
			ev[0].dwPeriodicTime = 100;
			ev[0].pEventFunc = CMSSystem::Process;

			desc.OnAcceptServer = OnAcceptServer;
			desc.OnDisconnectServer = OnDisconnectServer;
			desc.OnAcceptUser = OnAcceptUser;
			desc.OnDisconnectUser = OnDisconnectUser;
			desc.OnRecvFromServerTCP = ReceivedMsgFromServer;
			desc.OnRecvFromUserTCP = ReceivedMsgFromUser;
			desc.dwCustomDefineEventNum = 1;
			desc.pEvent = ev;

			desc.dwMainMsgQueMaxBufferSize = 5120000;
			desc.dwMaxServerNum = 100;
			desc.dwMaxUserNum = 50;
			desc.dwServerBufferSizePerConnection = 256000;
			desc.dwServerMaxTransferSize = 65000;
			desc.dwUserBufferSizePerConnection = 256000;
			desc.dwUserMaxTransferSize = 65000;
			desc.dwConnectNumAtSameTime = 100;
			desc.dwFlag = NULL;

			MNETWORK->Init(&desc);
			MNETWORK->StartServer(m_wServerType);
			
			if(g_DB.Init(1, 10, TRUE) == FALSE)
				MessageBox(NULL,"Fail To DataBase",0,0);

			m_bInit = TRUE;

			// IP Á¤º¸ ÀúÀå
			TotalIpInfo( 0 );
		}
	yCASE(TYPE_RMS)
		{
			sprintf( temp, "RMToolServer_%s", RMTOOLVERSION );
			SetWindowText(GetActiveWindow(), temp );

			// ¼­¹ö Á¤º¸ ·Îµå
			MServerTable->LoadMonitorInfo(TYPE_MAS,ipName[0],ipName[1]);
			CUSTOM_EVENT ev[1];
			ev[0].dwPeriodicTime = 100;
			ev[0].pEventFunc = CMSSystem::Process;

			desc.OnAcceptServer = NULL;
			desc.OnDisconnectServer = NULL;
			desc.OnAcceptUser = OnAcceptUser;
			desc.OnDisconnectUser = OnDisconnectUser;
			desc.OnRecvFromServerTCP = NULL;
			desc.OnRecvFromUserTCP = ReceivedMsgFromRMTool;
			desc.dwCustomDefineEventNum = 1;
			desc.pEvent = ev;

			desc.dwMainMsgQueMaxBufferSize = 5120000;
			desc.dwMaxServerNum = 0;
			desc.dwMaxUserNum = 50;
			desc.dwServerBufferSizePerConnection = 0;
			desc.dwServerMaxTransferSize = 0;
			desc.dwUserBufferSizePerConnection = 256000;
			desc.dwUserMaxTransferSize = 65000;
			desc.dwConnectNumAtSameTime = 100;
			desc.dwFlag = NULL;

			MNETWORK->Init(&desc);
			MNETWORK->StartServer(m_wServerType);
			
			if(g_DB.Init(1, 10, TRUE) == FALSE)
				MessageBox(NULL,"Fail To DataBase",0,0);

			m_bInit = TRUE;

			// IP Á¤º¸ ÀúÀå
			TotalIpInfo( 0 );
		}
	yENDSWITCH

	
	LOG( "Start() End" );
}

void CMSSystem::End()
{
	m_bInit = FALSE;

	g_DB.Release();
	MNETWORK->Release();
	CoFreeUnusedLibraries();
}

void CMSSystem::Process()
{
	ySWITCH(g_pMSSystem->m_wServerType)
	yCASE(TYPE_MS)
	{
		DWORD tick = GetTickCount();
		MServerTable->SetPositionHeadInfo();
		MSERVERINFO * pInfo = NULL;
		while(pInfo = MServerTable->GetDataInfo())
		{
			if(pInfo->wServerType != MONITOR_AGENT_SERVER && pInfo->wServerState == SERVERSTATE_ON)
			{
//				ASSERT(pInfo->wServerState != SERVERSTATE_OFF);

				if( tick - pInfo->dwExpiredTime > 10000)
				{
					// ping send
					MSGROOT msg;
					msg.Category = MP_MORNITORMAPSERVER;
					msg.Protocol = MP_MORNITORMAPSERVER_PING_SYN;
					MNETWORK->SendToMapServer(pInfo->dwConnectionIndex, (char *)&msg, sizeof(MSGROOT));
				}
				
				if( tick - pInfo->dwExpiredTime > 60000)
				{
					MSMGR->SendServerState(SERVERSTATE_ABNORMAL, pInfo);
				}
			}
		}

		if( bAutoPatch )
			AutoPatchProcess();		
	}
	yCASE(TYPE_MAS)
	{
		if( g_pMSSystem->m_bInit )
			g_DB.ProcessingDBMessage();
	}
	yCASE(TYPE_RMS)
	{
		if( g_pMSSystem->m_bInit )
			g_DB.ProcessingDBMessage();
	}
	yENDSWITCH
}

void CMSSystem::AutoPatchProcess()
{
	static BOOL bStart = FALSE;
	static DWORD oldtick = 0;
	DWORD tick = GetTickCount();

	if( (tick - oldtick) > 1000 )
	{
		oldtick = tick;

		HWND hWnd = FindWindow( NULL, AUTOPATCH );
		if( bStart == FALSE )	// Ã³À½
		{
			if( hWnd )
			{
				// SendStartAutoPatch;
				MSGMSSTATE Msg;
				Msg.Category = MP_MORNITORSERVER;
				Msg.Protocol = MP_MORNITORSERVER_CHECK_AUTOPATCH_ACK;
				MSERVERINFO* pInfo = MServerTable->GetMSInfo();
				strcpy( Msg.MSState.sServerIP, pInfo->szIPForServer );
				Msg.MSState.wServerPort = pInfo->wPortForServer;
				Msg.MSState.wAutoPatchState = AUTOPATCHSTATE_START;
				strcpy( Msg.MSState.sAutoPatchVersion, MSMGR->GetAutoPatchVersion() );
				MNETWORK->SendToMAS( (char*)&Msg, sizeof(Msg) );

				bStart = TRUE;
			}
		}
		else if( bStart == TRUE )
		{
			if( hWnd )
			{
				// SendProcessAutoPatch;			// ÁøÇà
				MSGMSSTATE Msg;
				Msg.Category = MP_MORNITORSERVER;
				Msg.Protocol = MP_MORNITORSERVER_CHECK_AUTOPATCH_ACK;
				MSERVERINFO* pInfo = MServerTable->GetMSInfo();
				strcpy( Msg.MSState.sServerIP, pInfo->szIPForServer );
				Msg.MSState.wServerPort = pInfo->wPortForServer;
				Msg.MSState.wAutoPatchState = AUTOPATCHSTATE_PROC;
				strcpy( Msg.MSState.sAutoPatchVersion, MSMGR->GetAutoPatchVersion() );
				MNETWORK->SendToMAS( (char*)&Msg, sizeof(Msg) );
			}
			else
			{
				// SendEndAutoPatch;				// ³¡
				MSGMSSTATE Msg;
				Msg.Category = MP_MORNITORSERVER;
				Msg.Protocol = MP_MORNITORSERVER_CHECK_AUTOPATCH_ACK;
				MSERVERINFO* pInfo = MServerTable->GetMSInfo();
				strcpy( Msg.MSState.sServerIP, pInfo->szIPForServer );
				Msg.MSState.wServerPort = pInfo->wPortForServer;
				Msg.MSState.wAutoPatchState = AUTOPATCHSTATE_END;
				MSMGR->ReadAutoPatchVersion();
				strcpy( Msg.MSState.sAutoPatchVersion, MSMGR->GetAutoPatchVersion() );
				MNETWORK->SendToMAS( (char*)&Msg, sizeof(Msg) );

				bStart = FALSE;
				bAutoPatch = FALSE;
			}
		}
	}
}

void CMSSystem::OnConnectMASSuccess(DWORD dwConnectionIndex, void* pVoid)
{
	MSERVERINFO * pInfo = (MSERVERINFO * )pVoid;
	pInfo->dwConnectionIndex = dwConnectionIndex;
	ySWITCH(g_pMSSystem->m_wServerType)
	yCASE(TYPE_MS)	
		LogConsole("MAPÁ¢¼Ó¼º°ø");
		MSMGR->SendMSInfo();
		MNETWORK->StartServer(TYPE_MS);
	yCASE(TYPE_MAS)
			ASSERT(0);
	yENDSWITCH
}

void CMSSystem::OnConnectMASFail(void* pVoid)
{
	ASSERTMSG(0, "MAS¸¦ ¸ÕÀú ½ÇÇàÇÏ¼¼¿ä!");

	ySWITCH(g_pMSSystem->m_wServerType)
	yCASE(TYPE_MS)
	yCASE(TYPE_MAS)
			ASSERT(0);
	yENDSWITCH
}


void CMSSystem::OnAcceptServer(DWORD dwConnectionIndex)
{
	ySWITCH(g_pMSSystem->m_wServerType)
	yCASE(TYPE_MS)
		// map server Á¢¼ÓÇØ µé¾î¿È
		LogConsole("MAPÁ¢¼Ó");
	yCASE(TYPE_MAS)
		// ms server Á¢¼ÓÇØ µé¾î¿È
		LogConsole("MSÁ¢¼Ó");
	yENDSWITCH

}

void CMSSystem::OnDisconnectServer(DWORD dwConnectionIndex)
{
	ySWITCH(g_pMSSystem->m_wServerType)
	yCASE(TYPE_MS)
		MSERVERINFO* pInfo = MServerTable->GetServerInfoForConn(dwConnectionIndex);
		if(pInfo == NULL)	// MONITOR_AGENT_SERVER
		{
			LogConsole("MAS²÷¾îÁü");
			::SendMessage(gMainHwnd,WM_CLOSE,0,0);
			return;
		}
			
		switch(pInfo->wServerType)
		{
		case MAP_SERVER:
		case AGENT_SERVER:
		case DISTRIBUTE_SERVER:
			{
				LogConsole("MAP²÷¾îÁü");
				// map server ²÷¾îÁü
				// 1. ¼­¹ö Å×ÀÌºí¿¡¼­ »èÁ¦
				// 2. mas¿¡°Ô »óÅÂ ¾Ë¸²
				MSMGR->SendServerState(SERVERSTATE_OFF, MServerTable->GetServerInfoForConn(dwConnectionIndex));
				MSMGR->ServerUnRegiest(dwConnectionIndex);
			}
			return;
		}
		LogConsole("ºñÁ¤»ó ²÷¾îÁü");
	yCASE(TYPE_MAS)
		// ms server ²÷¾îÁü
		// »óÅÂ ÃÊ±âÈ­
		LogConsole("MS²÷¾îÁü");
		MASMGR->SetStateInitForConn(dwConnectionIndex);
		MServerTable->RemoveServerForConn( dwConnectionIndex );

		MASMGR->SendServerStateAllToMCAll();

		MASMGR->SendMSServerStateAllToMCAll();

	yENDSWITCH
	
}

// In case of MS
// from mapserver
// from mas
// ¸Ê¼­¹öÁ¤º¸¸¦ ¾Ë¾Æ¾ß ÇÑ´Ù.
void CMSSystem::ReceivedMsgFrom2Server(DWORD dwConnectionIndex,char* pmsg,DWORD dwLength)
{
	MSGROOT* pMsg = (MSGROOT*)pmsg;
	ySWITCH(pMsg->Category)
	yCASE(MP_MORNITORMAPSERVER)		// from MapServer
	{
		ySWITCH(pMsg->Protocol)
		yCASE(MP_MORNITORMAPSERVER_QUERYUSERCOUNT_ACK)
		{
			MSGUSERCOUNT * pMsg = (MSGUSERCOUNT *)pmsg;
			MSERVERINFO * pInfo = MServerTable->GetServerInfoForConn(dwConnectionIndex);
			pMsg->Category = MP_MORNITORSERVER;
			pMsg->Protocol = MP_MORNITORSERVER_QUERYUSERCOUNT_ACK;
			pMsg->ServerPort = pInfo->wPortForServer;
			MNETWORK->SendToMAS(pmsg, sizeof(MSGUSERCOUNT));
		}
		yCASE(MP_MORNITORMAPSERVER_QUERY_CHANNELINFO_ACK)
		{			
			MSG_CHANNEL_INFO_MORNITOR Msg;
			Msg.ChannelInfo = *((MSG_CHANNEL_INFO*)pmsg);
			MSERVERINFO* pInfo = MServerTable->GetServerInfoForConn( dwConnectionIndex );
			strcpy( Msg.sServerIP, pInfo->szIPForServer );
			Msg.wServerPort = pInfo->wPortForServer;
			Msg.Category = MP_MORNITORSERVER;
			Msg.Protocol = MP_MORNITORSERVER_QUERY_CHANNELINFO_ACK;
			MNETWORK->SendToMAS( (char*)&Msg, sizeof(MSG_CHANNEL_INFO_MORNITOR) );
		}
		yCASE(MP_MORNITORMAPSERVER_BOOTUP_NOTIFY)
		{
			//send to mas
			MSG_PWRUP_BOOTUP * pmsg2 = (MSG_PWRUP_BOOTUP *)pmsg;
			MSMGR->AddServerRegiest(dwConnectionIndex, pmsg2->dwProcessID, &pmsg2->BootInfo);
			MSMGR->SendServerState(SERVERSTATE_ON, &pmsg2->BootInfo);
		}
		yCASE(MP_MORNITORMAPSERVER_PING_ACK)
		{
			MSERVERINFO * pInfo = MServerTable->GetServerInfoForConn(dwConnectionIndex);
			pInfo->dwExpiredTime = GetTickCount();
			if(pInfo->wServerState == SERVERSTATE_ABNORMAL )
			{
				MSMGR->SendServerState(SERVERSTATE_RECOVER, pInfo);
			}
			pInfo->wServerState = SERVERSTATE_ON;
		}
		yCASE(MP_MORNITORMAPSERVER_CHANGE_USERLEVEL_ACK)
		{
			MSG_MCDWORD* pMsg = (MSG_MCDWORD*)pmsg;
			pMsg->Category = MP_MORNITORSERVER;
			pMsg->Protocol = MP_MORNITORSERVER_CHANGE_USERLEVEL_ACK;
			MNETWORK->SendToMAS( (char*)pMsg, sizeof(MSG_MCDWORD) );
		}
		yCASE(MP_MORNITORMAPSERVER_DISCONNECT_ALLUSER_ACK)
		{
			MSERVERINFO* pInfo = MServerTable->GetServerInfoForConn(dwConnectionIndex);

			MSGSERVERSTATE msg;
			msg.Category = MP_MORNITORSERVER;
			msg.Protocol = MP_MORNITORSERVER_DISCONNECT_ALLUSER_ACK;
			strcpy(msg.serverState.ServerIP, pInfo->szIPForServer);
			msg.serverState.ServerPort		= pInfo->wPortForServer;
			msg.serverState.ServerType		= pInfo->wServerType;
			msg.serverState.ServerNum		= pInfo->wServerNum;
			MNETWORK->SendToMAS((char*)&msg, sizeof(msg));
		}
		yCASE(MP_MORNITORMAPSERVER_QUERY_VERSION_ACK)
		{
			MSGNOTICE* msg = (MSGNOTICE*)pmsg;
			msg->Category = MP_MORNITORSERVER;
			msg->Protocol = MP_MORNITORSERVER_QUERY_VERSION_ACK;
			MNETWORK->SendToMAS( (char*)msg, sizeof(MSGNOTICE) );
		}
		yCASE(MP_MORNITORMAPSERVER_CHANGE_VERSION_ACK)
		{
			MSGNOTICE* msg = (MSGNOTICE*)pmsg;
			msg->Category = MP_MORNITORSERVER;
			msg->Protocol = MP_MORNITORSERVER_CHANGE_VERSION_ACK;
			MNETWORK->SendToMAS( (char*)msg, sizeof(MSGNOTICE) );
		}
		yCASE(MP_MORNITORMAPSERVER_QUERY_MAXUSER_ACK)
		{
			MSG_MCDWORD* pMsg = (MSG_MCDWORD*)pmsg;
			pMsg->Category = MP_MORNITORSERVER;
			pMsg->Protocol = MP_MORNITORSERVER_QUERY_MAXUSER_ACK;
			MNETWORK->SendToMAS( (char*)pMsg, sizeof(MSG_MCDWORD) );
		}
		yCASE(MP_MORNITORMAPSERVER_CHANGE_MAXUSER_ACK)
		{
			MSG_MCDWORD* pMsg = (MSG_MCDWORD*)pmsg;
			pMsg->Category = MP_MORNITORSERVER;
			pMsg->Protocol = MP_MORNITORSERVER_CHANGE_MAXUSER_ACK;
			MNETWORK->SendToMAS( (char*)pMsg, sizeof(MSG_MCDWORD) );
		}
		yENDSWITCH
	}
	yCASE(MP_MORNITORSERVER)	// from MAS
	{
		ySWITCH(pMsg->Protocol)
		yCASE(MP_MORNITORSERVER_QUERYUSERCOUNT_SYN)
		{
			// send to mapserver all
			MSGMC * pMsg = (MSGMC *)pmsg;
			pMsg->Category = MP_MORNITORMAPSERVER;
			pMsg->Protocol = MP_MORNITORMAPSERVER_QUERYUSERCOUNT_SYN;
			MNETWORK->SendToMapServerAll((char*)pMsg, dwLength);
		}
		yCASE(MP_MORNITORSERVER_QUERY_CHANNELINFO_SYN)
		{
			MSGROOT Msg;
			Msg.Category = MP_MORNITORMAPSERVER;
			Msg.Protocol = MP_MORNITORMAPSERVER_QUERY_CHANNELINFO_SYN;
			MNETWORK->SendToMapServerAll( (char*)&Msg, sizeof(MSGROOT) );
		}
		yCASE(MP_MORNITORSERVER_BOOTLIST_NOTIFY)
		{
			//to mapserver
			MSG_PWRUP_BOOTLIST *pmsg2 = (MSG_PWRUP_BOOTLIST *)pmsg;
			pmsg2->Category = MP_POWERUP;
			pmsg2->Protocol = MP_POWERUP_BOOTLIST_ACK;
			MNETWORK->SendToMapServer(MServerTable->GetServerInfoForPort(pmsg2->MapServerPort)->dwConnectionIndex, pmsg, dwLength);
		}
		//to mapserver
		yCASE(MP_MORNITORSERVER_EXECUTE_MAPSERVER_SYN)
		{
			// ¼­¹ö ±¸µ¿!!!!
			MSGEXECUTESERVER * pmsg2 = (MSGEXECUTESERVER *)pmsg;
			MSMGR->ExecuteServer(pmsg2->ServerType, pmsg2->ServerNum);
		}
		yCASE(MP_MORNITORSERVER_SHUTDOWN_MAPSERVER_SYN)		// msg¸¦ ÅëÇØ
		{
			MSGSHUTDOWNSERVER * pmsg2 = (MSGSHUTDOWNSERVER *)pmsg;
			MSMGR->ServerOff(pmsg2->ServerPort);
		}
		yCASE(MP_MORNITORSERVER_KILL_SERVER_SYN)			// °­Á¦Á¾·á(abnormal)
		{
			MSGSHUTDOWNSERVER* pmsg2 = (MSGSHUTDOWNSERVER*)pmsg;
			MSMGR->KillServer(pmsg2->ServerPort);
		}
		yCASE(MP_MORNITORSERVER_NOTICESEND_SYN)
		{
			MSGNOTICE * pmsg2 = (MSGNOTICE *)pmsg;
			pmsg2->Category = MP_MORNITORMAPSERVER;
			pmsg2->Protocol = MP_MORNITORMAPSERVER_NOTICESEND_SYN;
			// ¿¡ÀÌÀüÆ®¿¡°Ô¸¸
			MSMGR->SendToNotice(pmsg2, dwLength);
		}
		yCASE(MP_MORNITORSERVER_EXECUTE_AUTOPATCH_SYN)	// AutoPatch
		{			
			MSMGR->ReadAutoPatchVersion();
			MSMGR->AutoPatch();
			bAutoPatch = TRUE;
		}
		yCASE(MP_MORNITORSERVER_CHANGE_USERLEVEL_SYN)	// UserLevel
		{
			MSG_MCDWORD* msg = (MSG_MCDWORD*)pmsg;
			msg->Category = MP_MORNITORMAPSERVER;
			msg->Protocol = MP_MORNITORMAPSERVER_CHANGE_USERLEVEL_SYN;

			// Distribute
			MSMGR->SendToDistribute( (char*)msg, dwLength );
		}
		yCASE(MP_MORNITORSERVER_ASSERTMSGBOX_SYN)
		{
			MSG_MCDWORD* pMsg = (MSG_MCDWORD *)pmsg;
			pMsg->Category = MP_MORNITORMAPSERVER;
			pMsg->Protocol = MP_MORNITORMAPSERVER_ASSERTMSGBOX_SYN;
			MNETWORK->SendToMapServerAll((char*)pMsg, dwLength);
		}
		yCASE(MP_MORNITORSERVER_DISCONNECT_ALLUSER_SYN)
		{
			MSG_MCDWORD msg;
			msg.Category = MP_MORNITORMAPSERVER;
			msg.Protocol = MP_MORNITORMAPSERVER_DISCONNECT_ALLUSER_SYN;
			MNETWORK->SendToMapServerAll((char*)&msg, sizeof(msg));
		}
		yCASE(MP_MORNITORSERVER_QUERY_VERSION_SYN)
		{
			MSGROOT* pmsg = (MSGROOT*)pMsg;
			pmsg->Category = MP_MORNITORMAPSERVER;
			pmsg->Protocol = MP_MORNITORMAPSERVER_QUERY_VERSION_SYN;
			MSMGR->SendToDistribute( (char*)pmsg, dwLength );
		}
		yCASE(MP_MORNITORSERVER_CHANGE_VERSION_SYN)
		{
			MSGNOTICE* pmsg = (MSGNOTICE*)pMsg;
			pmsg->Category = MP_MORNITORMAPSERVER;
			pmsg->Protocol = MP_MORNITORMAPSERVER_CHANGE_VERSION_SYN;
			MSMGR->SendToDistribute( (char*)pmsg, dwLength );
		}
		yCASE(MP_MORNITORSERVER_QUERY_MAXUSER_SYN)
		{
			MSGROOT* pmsg = (MSGROOT*)pMsg;
			pmsg->Category = MP_MORNITORMAPSERVER;
			pmsg->Protocol = MP_MORNITORMAPSERVER_QUERY_MAXUSER_SYN;
			MSMGR->SendToDistribute( (char*)pmsg, dwLength );
		}
		yCASE(MP_MORNITORSERVER_CHANGE_MAXUSER_SYN)
		{
			MSG_MCDWORD* pmsg = (MSG_MCDWORD*)pMsg;
			pmsg->Category = MP_MORNITORMAPSERVER;
			pmsg->Protocol = MP_MORNITORMAPSERVER_CHANGE_MAXUSER_SYN;
			MSMGR->SendToDistribute( (char*)pmsg, dwLength );
		}
		yCASE(MP_MORNITORSERVER_QUERY_AUTOPATCHVERSION_SYN)
		{
			MSGMSSTATE Msg;
			Msg.Category = MP_MORNITORSERVER;
			Msg.Protocol = MP_MORNITORSERVER_QUERY_AUTOPATCHVERSION_ACK;
			MSERVERINFO* pInfo = MServerTable->GetMSInfo();
			strcpy( Msg.MSState.sServerIP, pInfo->szIPForServer );
			Msg.MSState.wServerPort = pInfo->wPortForServer;
			Msg.MSState.wAutoPatchState = AUTOPATCHSTATE_END;
			MSMGR->ReadAutoPatchVersion();
			strcpy( Msg.MSState.sAutoPatchVersion, MSMGR->GetAutoPatchVersion() );
			MNETWORK->SendToMAS( (char*)&Msg, sizeof(Msg) );
		}
		yENDSWITCH
	}
	yENDSWITCH
}

// In case of MAS
// from ms
// MCÁ¤º¸¸¦ ¾Ë¾Æ¾ß ÇÑ´Ù.
void CMSSystem::ReceivedMsgFromMS(DWORD dwConnectionIndex,char* pmsg,DWORD dwLength)
{
	MSGROOT* pMsg = (MSGROOT*)pmsg;
	ySWITCH(pMsg->Category)
	yCASE(MP_MORNITORSERVER)
	{
		ySWITCH(pMsg->Protocol)
		yCASE(MP_MORNITORSERVER_QUERYUSERCOUNT_ACK)
		{
			MSGUSERCOUNT * pMsg = (MSGUSERCOUNT *)pmsg;
			SERVERSTATE * pState = MASMGR->GetServerState(pMsg->ServerPort);
			pState->UserCount = pMsg->dwUserCount;
			MASMGR->SendServerStateToMC(pMsg->dwConnectionIdxMC, pMsg->ServerPort);
		}
		yCASE(MP_MORNITORSERVER_QUERY_CHANNELINFO_ACK)
		{
			MSG_CHANNEL_INFO_MORNITOR* pMsg = (MSG_CHANNEL_INFO_MORNITOR*)pmsg;
			pMsg->Category = MP_MORNITORTOOL;
			pMsg->Protocol = MP_MORNITORTOOL_QUERY_CHANNELINFO_ACK;
			MASMGR->SendToAllMC( (char*)pMsg, sizeof(MSG_CHANNEL_INFO_MORNITOR) );			
		}
		yCASE(MP_MORNITORSERVER_MSINFO_NOTIFY)
		{
			MSGMSINFO * pmsg2 = (MSGMSINFO *)pMsg;
			pmsg2->MSInfo.dwConnectionIndex = dwConnectionIndex;
			//*MServerTable->GetMSInfo() = pmsg2->MSInfo;
			// ¿©·¯°³ÀÇ MS°¡ ºÙÀ½
			// MAPÀÌ ¾î´À MC¿¡ ºÙ¾î ÀÖ´ÂÁö ¾Ë¾Æ¾ß ÇÔ
			LogConsole("From MSINFO_NOTIFY : CON:%d, IP:%s, PORT:%d",pmsg2->MSInfo.dwConnectionIndex, pmsg2->MSInfo.szIPForServer, pmsg2->MSInfo.wPortForServer);
			MServerTable->AddServer(&pmsg2->MSInfo, pmsg2->MSInfo.wPortForServer);

			MASMGR->SendMSServerStateAllToMCAll();
		}
		yCASE(MP_MORNITORSERVER_EXECUTE_MAPSERVER_ACK)
		//send to mc
		{
			MSGEXECUTESERVER * pmsg2 = (MSGEXECUTESERVER *)pMsg;
			pmsg2->Category = MP_MORNITORTOOL;
			pmsg2->Protocol = MP_MORNITORTOOL_EXECUTE_MAPSERVER_ACK;
			MNETWORK->SendToMS(pmsg2->dwConnectionIdxMC, pmsg, dwLength);
		}
		yCASE(MP_MORNITORSERVER_MAPSERVERSTATE_ACK)
		{
			MSGSERVERSTATE * pmsg2 = (MSGSERVERSTATE *)pMsg;
			SERVERSTATE * pState = MASMGR->GetServerState(pmsg2->serverState.ServerPort);
			WORD OldState = pState->ServerState = pmsg2->serverState.ServerState;
						
			// 1. mapserver bootup		on
			// send conn list
			// 2. mapserver down		abnormal
			// 3. mapserver shutdown	off
			ySWITCH(pmsg2->serverState.ServerState)
				yCASE(SERVERSTATE_ON)
					MASMGR->SendServerStateBootListToMS(dwConnectionIndex, pState->ServerType, pState->ServerPort);
				
				yCASE(SERVERSTATE_OFF)
				yCASE(SERVERSTATE_ABNORMAL)
					if(OldState == SERVERSTATE_OFF)
					{
						ASSERTMSG(0,"OFFµÇ¾ú´Â µ¥ ´Ù½Ã ABNORMAL msg recv");
						pState->ServerState = SERVERSTATE_OFF;
					}
					
				yCASE(SERVERSTATE_RECOVER)
					pState->ServerState = SERVERSTATE_ON;
				
			yENDSWITCH

			MASMGR->SendServerStateToMCs(pmsg2->serverState.ServerPort);

		}
		yCASE(MP_MORNITORSERVER_SHUTDOWN_MAPSERVER_ACK)
		//send to mc
		{
			MSGSHUTDOWNSERVER * pmsg2 = (MSGSHUTDOWNSERVER *)pMsg;
			pmsg2->Category = MP_MORNITORTOOL;
			pmsg2->Protocol = MP_MORNITORTOOL_NOTICESEND_ACK;
			MNETWORK->SendToMS(pmsg2->dwConnectionIdxMC, pmsg, dwLength);
		}
		yCASE(MP_MORNITORSERVER_CHANGE_USERLEVEL_ACK)
		//send to mc
		{
			MSG_MCDWORD * pmsg = (MSG_MCDWORD*)pMsg;
			pmsg->Category = MP_MORNITORTOOL;
			pmsg->Protocol = MP_MORNITORTOOL_CHANGE_USERLEVEL_ACK;
			MASMGR->SendToAllMC( (char*)pmsg, dwLength );
		}
		yCASE(MP_MORNITORSERVER_DISCONNECT_ALLUSER_ACK)
		{
			MSGSERVERSTATE* pmsg = (MSGSERVERSTATE*)pMsg;
			pmsg->Category = MP_MORNITORTOOL;
			pmsg->Protocol = MP_MORNITORTOOL_DISCONNECT_ALLUSER_ACK;
			MASMGR->SendToAllMC( (char*)pmsg, dwLength );
		}
		yCASE(MP_MORNITORSERVER_QUERY_VERSION_ACK)
		{
			MSGNOTICE* msg = (MSGNOTICE*)pMsg;
			msg->Category = MP_MORNITORTOOL;
			msg->Protocol = MP_MORNITORTOOL_QUERY_VERSION_ACK;
			MASMGR->SendToAllMC( (char*)msg, sizeof(MSGNOTICE) );
		}
		yCASE(MP_MORNITORSERVER_CHANGE_VERSION_ACK)
		{
			MSGNOTICE* msg = (MSGNOTICE*)pMsg;
			msg->Category = MP_MORNITORTOOL;
			msg->Protocol = MP_MORNITORTOOL_CHANGE_VERSION_ACK;
			MASMGR->SendToAllMC( (char*)msg, sizeof(MSGNOTICE) );
		}
		yCASE(MP_MORNITORSERVER_QUERY_MAXUSER_ACK)
		{
			MSG_MCDWORD* msg = (MSG_MCDWORD*)pMsg;
			msg->Category = MP_MORNITORTOOL;
			msg->Protocol = MP_MORNITORTOOL_QUERY_MAXUSER_ACK;
			MASMGR->SendToAllMC( (char*)msg, sizeof(MSG_MCDWORD) );
		}
		yCASE(MP_MORNITORSERVER_CHANGE_MAXUSER_ACK)
		{
			MSG_MCDWORD* msg = (MSG_MCDWORD*)pMsg;
			msg->Category = MP_MORNITORTOOL;
			msg->Protocol = MP_MORNITORTOOL_CHANGE_MAXUSER_ACK;
			MASMGR->SendToAllMC( (char*)msg, sizeof(MSG_MCDWORD) );
		}
		// AutoPatch
		yCASE(MP_MORNITORSERVER_CHECK_AUTOPATCH_ACK)
		{
			MSGMSSTATE* pmsg = (MSGMSSTATE*)pMsg;
			pmsg->Category = MP_MORNITORTOOL;
			pmsg->Protocol = MP_MORNITORTOOL_CHECK_AUTOPATCH_ACK;
			MASMGR->SendToAllMC( (char*)pmsg, sizeof(MSGMSSTATE) );
		}
		yCASE(MP_MORNITORSERVER_QUERY_AUTOPATCHVERSION_ACK)
		{
			MSGMSSTATE* pmsg = (MSGMSSTATE*)pMsg;
			pmsg->Category = MP_MORNITORTOOL;
			pmsg->Protocol = MP_MORNITORTOOL_QUERY_AUTOPATCHVERSION_ACK;
			MASMGR->SendToAllMC( (char*)pmsg, sizeof(MSGMSSTATE) );
		}
		yENDSWITCH
	}
	yENDSWITCH
}

void CMSSystem::ReceivedMsgFromServer(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	ySWITCH(g_pMSSystem->m_wServerType)
	yCASE(TYPE_MS)
		ReceivedMsgFrom2Server(dwConnectionIndex, pMsg, dwLength);
	yCASE(TYPE_MAS)
		ReceivedMsgFromMS(dwConnectionIndex,pMsg, dwLength);
	yENDSWITCH
}

//FROM MC·Î ºÎÅÍ ¿À´Â °Í
void CMSSystem::ReceivedMsgFromUser(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	if( pTempMsg->Category == MP_RMTOOL_CONNECT )
	{
		ConnentMsgParse(dwConnectionIndex, pMsg, dwLength);
		return;
	}

	if( !IPCHECKMGR->CheckIP( dwConnectionIndex ) )
		return;

	ySWITCH(pTempMsg->Category)
	yCASE(MP_MORNITORTOOL)
	{
		ySWITCH(pTempMsg->Protocol)
		yCASE(MP_MORNITORTOOL_QUERYUSERCOUNT_SYN)
		{
			// send to ms all
			MSGMC msg;
			msg.Category = MP_MORNITORSERVER;
			msg.Protocol = MP_MORNITORSERVER_QUERYUSERCOUNT_SYN;
			msg.dwConnectionIdxMC = dwConnectionIndex;
			MNETWORK->SendToMSAll((char*)&msg, sizeof(msg));
		}
		yCASE(MP_MORNITORTOOL_QUERY_CHANNELINFO_SYN)
		{
			MSGROOT Msg;
			Msg.Category = MP_MORNITORSERVER;
			Msg.Protocol = MP_MORNITORSERVER_QUERY_CHANNELINFO_SYN;
			MNETWORK->SendToMSAll( (char*)&Msg, sizeof(MSGROOT) );
		}
		yCASE(MP_MORNITORTOOL_EXECUTE_MAPSERVER_SYN)
		{
			MSGEXECUTESERVER * pmsg = (MSGEXECUTESERVER *)pMsg;
			pmsg->Category = MP_MORNITORSERVER;
			pmsg->Protocol = MP_MORNITORSERVER_EXECUTE_MAPSERVER_SYN;
			pmsg->dwConnectionIdxMC = dwConnectionIndex;
			// map¼­¹ö Æ÷Æ®·Î mapÀÌ ¼Ò¼ÓµÈ ms¸¦ Ã£¾Æ¼­ ms·Î º¸³½´Ù.
			
			MSERVERINFO * pInfo = MServerTable->GetMSServerInfoForIP(pmsg->ServerIP);
			if(pInfo)
			{

				MNETWORK->SendToMS(pInfo->dwConnectionIndex, pMsg, dwLength);
				LogConsole("sendEXEMsg To MS(CON:%d, IP:%s, PORT:%d)", pInfo->dwConnectionIndex, pInfo->szIPForServer, pInfo->wPortForServer);
			}			
		}
		yCASE(MP_MORNITORTOOL_SHUTDOWN_MAPSERVER_SYN)	// Msg·Î ¼­¹ö ²û.
		{
			MSGSHUTDOWNSERVER * pmsg = (MSGSHUTDOWNSERVER *)pMsg;
			pmsg->Category = MP_MORNITORSERVER;
			pmsg->Protocol = MP_MORNITORSERVER_SHUTDOWN_MAPSERVER_SYN;
			pmsg->dwConnectionIdxMC = dwConnectionIndex;
			// map¼­¹ö Æ÷Æ®·Î mapÀÌ ¼Ò¼ÓµÈ ms¸¦ Ã£¾Æ¼­ ms·Î º¸³½´Ù.
			MSERVERINFO * pInfo = MServerTable->GetMSServerInfoForIP(pmsg->ServerIP);
			if(pInfo)
			MNETWORK->SendToMS(pInfo->dwConnectionIndex, pMsg, dwLength);
		}
		yCASE(MP_MORNITORTOOL_KILL_SERVER_SYN)			// °­Á¦ Á¾·á(abnormal)
		{
			MSGSHUTDOWNSERVER* pmsg = (MSGSHUTDOWNSERVER*)pMsg;
			pmsg->Category = MP_MORNITORSERVER;
			pmsg->Protocol = MP_MORNITORSERVER_KILL_SERVER_SYN;
			pmsg->dwConnectionIdxMC = dwConnectionIndex;
			// map¼­¹ö Æ÷Æ®·Î mapÀÌ ¼Ò¼ÓµÈ ms¸¦ Ã£¾Æ¼­ ms·Î º¸³½´Ù.
			MSERVERINFO * pInfo = MServerTable->GetMSServerInfoForIP(pmsg->ServerIP);
			if(pInfo)
				MNETWORK->SendToMS(pInfo->dwConnectionIndex, pMsg, dwLength);
		}
		yCASE(MP_MORNITORTOOL_NOTICESEND_SYN)
		{
			MSGNOTICE * pmsg = (MSGNOTICE *)pMsg;
			pmsg->Category = MP_MORNITORSERVER;
			pmsg->Protocol = MP_MORNITORSERVER_NOTICESEND_SYN;
			pmsg->dwConnectionIdxMC = dwConnectionIndex;
			MNETWORK->SendToMSAll(pMsg, dwLength);
		}
		yCASE(MP_MORNITORTOOL_MAPSERVERSTATE_SYN)
		{
			// Request
			//¼­¹ö »óÅÂ ACK
			MASMGR->SendServerStateAllToMC(dwConnectionIndex);
		}
		yCASE(MP_MORNITORTOOL_EXECUTE_AUTOPATCH_SYN)	// AutoPatch
		{
			MSGROOT msg;
			msg.Category = MP_MORNITORSERVER;
			msg.Protocol = MP_MORNITORSERVER_EXECUTE_AUTOPATCH_SYN;
			MNETWORK->SendToMSAll( (char*)&msg, sizeof(msg));
		}
		yCASE(MP_MORNITORTOOL_CHANGE_USERLEVEL_SYN)		// UserLevel
		{
			MSGROOT* msg = (MSGROOT*)pMsg;
			msg->Category = MP_MORNITORSERVER;
			msg->Protocol = MP_MORNITORSERVER_CHANGE_USERLEVEL_SYN;
			MNETWORK->SendToMSAll( (char*)msg, dwLength );			
		}
		yCASE(MP_MORNITORTOOL_ASSERTMSGBOX_SYN)
		{
			MSG_MCDWORD* msg = (MSG_MCDWORD*)pMsg;
			msg->Category = MP_MORNITORSERVER;
			msg->Protocol = MP_MORNITORSERVER_ASSERTMSGBOX_SYN;
			MNETWORK->SendToMSAll( (char*)msg, dwLength );
		}
		yCASE(MP_MORNITORTOOL_DISCONNECT_ALLUSER_SYN)
		{
			MSGEXECUTESERVER* pmsg = (MSGSHUTDOWNSERVER *)pMsg;
			pmsg->Category = MP_MORNITORSERVER;
			pmsg->Protocol = MP_MORNITORSERVER_DISCONNECT_ALLUSER_SYN;
			pmsg->dwConnectionIdxMC = dwConnectionIndex;
			// map¼­¹ö Æ÷Æ®·Î mapÀÌ ¼Ò¼ÓµÈ ms¸¦ Ã£¾Æ¼­ ms·Î º¸³½´Ù.
			MSERVERINFO * pInfo = MServerTable->GetMSServerInfoForIP(pmsg->ServerIP);
			if(pInfo)
				MNETWORK->SendToMS(pInfo->dwConnectionIndex, pMsg, dwLength);
			
//			MNETWORK->SendToMSAll( (char*)pmsg, dwLength );
		}		
		yCASE(MP_MORNITORTOOL_QUERY_VERSION_SYN)
		{
			MSGROOT* pmsg = (MSGROOT*)pMsg;
			pmsg->Category = MP_MORNITORSERVER;
			pmsg->Protocol = MP_MORNITORSERVER_QUERY_VERSION_SYN;
			MNETWORK->SendToMSAll( (char*)pmsg, dwLength );
		}
		yCASE(MP_MORNITORTOOL_CHANGE_VERSION_SYN)
		{
			MSGNOTICE* pmsg = (MSGNOTICE*)pMsg;
			pmsg->Category = MP_MORNITORSERVER;
			pmsg->Protocol = MP_MORNITORSERVER_CHANGE_VERSION_SYN;
			MNETWORK->SendToMSAll( (char*)pmsg, dwLength );
		}
		yCASE(MP_MORNITORTOOL_QUERY_MAXUSER_SYN)
		{
			MSGROOT* pmsg = (MSGROOT*)pMsg;
			pmsg->Category = MP_MORNITORSERVER;
			pmsg->Protocol = MP_MORNITORSERVER_QUERY_MAXUSER_SYN;
			MNETWORK->SendToMSAll( (char*)pmsg, dwLength );	
		}
		yCASE(MP_MORNITORTOOL_CHANGE_MAXUSER_SYN)
		{
			MSG_MCDWORD* pmsg = (MSG_MCDWORD*)pMsg;
			pmsg->Category = MP_MORNITORSERVER;
			pmsg->Protocol = MP_MORNITORSERVER_CHANGE_MAXUSER_SYN;
			MNETWORK->SendToMSAll( (char*)pmsg, dwLength );			
		}
		yCASE(MP_MORNITORTOOL_MSSTATEALL_SYN)
		{
			MSERVERINFO* info = NULL;
			MSGMSSTATEALL Msg;
			MServerTable->SetPositionHeadInfo();			
			while( info = (MSERVERINFO*)MServerTable->GetDataInfo() )
			{
				MSSTATE state;
				strcpy( state.sServerIP, info->szIPForServer );
				state.wServerPort = info->wPortForServer;

				Msg.AddMSServerState( &state );
			}

			Msg.Category = MP_MORNITORTOOL;
			Msg.Protocol = MP_MORNITORTOOL_MSSTATEALL_ACK;

			MNETWORK->SendToMC( dwConnectionIndex, (char*)&Msg, Msg.GetSize() );
		}
		yCASE(MP_MORNITORTOOL_QUERY_AUTOPATCHVERSION_SYN)
		{
			MSGROOT Msg;
			Msg.Category = MP_MORNITORSERVER;
			Msg.Protocol = MP_MORNITORSERVER_QUERY_AUTOPATCHVERSION_SYN;
			MNETWORK->SendToMSAll( (char*)&Msg, sizeof(MSGROOT) );
		}
		yENDSWITCH
	}
/*	yCASE(MP_RMTOOL_CONNECT)
	{
		ConnentMsgParse(dwConnectionIndex, pMsg, dwLength);
	}
	yCASE(MP_RMTOOL_USER)
	{
		UserMsgParse(dwConnectionIndex, pMsg, dwLength);
	}
	yCASE(MP_RMTOOL_MUNPA)
	{
		MunpaMsgParse(dwConnectionIndex, pMsg, dwLength);
	}
	yCASE(MP_RMTOOL_GAMELOG)
	{
		GameLogMsgParse(dwConnectionIndex, pMsg, dwLength);
	}
	yCASE(MP_RMTOOL_OPERLOG)
	{
		OperLogMsgParse(dwConnectionIndex, pMsg, dwLength);
	}
	yCASE(MP_RMTOOL_STATISTICS)
	{
		StatisticsMsgParse(dwConnectionIndex, pMsg, dwLength);
	}
	yCASE(MP_RMTOOL_ADMIN)
	{
		AdminMsgParse(dwConnectionIndex, pMsg, dwLength);
	}
	yCASE(MP_RMTOOL_CHARACTER)
	{
		CharacterMsgParse(dwConnectionIndex, pMsg, dwLength);
	}
	yCASE( MP_RMTOOL_ITEM )
	{
		ItemMsgParse(dwConnectionIndex, pMsg, dwLength);
	}
	*/
	yENDSWITCH
}

void CMSSystem::OnAcceptUser(DWORD dwConnectionIndex)
{
	// mcÁ¢¼ÓÇØ ¿È
	LogConsole("McÁ¢¼Ó");
	MCINFO Info;
	Info.dwConnectionIndex = dwConnectionIndex;
	MUserTable->AddUser(&Info, dwConnectionIndex);
	// sendserverstate all
//	MASMGR->SendServerStateAllToMC(dwConnectionIndex);
	
}
void CMSSystem::OnDisconnectUser(DWORD dwConnectionIndex)
{
	// nothing
	LogConsole("Mc²÷¾îÁü");
	MUserTable->RemoveUser(dwConnectionIndex);
}

/////////////////////////////////////////////////////////////////////////////////////
// MR_TOOL only
void CMSSystem::ReceivedMsgFromRMTool( DWORD dwConnectionIndex,char* pMsg,DWORD dwLength )
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;

	if( pTempMsg->Category == MP_RMTOOL_CONNECT )
	{
		ConnentMsgParse(dwConnectionIndex, pMsg, dwLength);
		return;
	}

	if( !IPCHECKMGR->CheckIP( dwConnectionIndex ) )
		return;

	ySWITCH(pTempMsg->Category)
//	yCASE(MP_RMTOOL_CONNECT)
//	{
//		ConnentMsgParse(dwConnectionIndex, pMsg, dwLength);
//	}
	yCASE(MP_RMTOOL_USER)
	{
		UserMsgParse(dwConnectionIndex, pMsg, dwLength);
	}
	yCASE(MP_RMTOOL_MUNPA)
	{
		MunpaMsgParse(dwConnectionIndex, pMsg, dwLength);
	}
	yCASE(MP_RMTOOL_GAMELOG)
	{
		GameLogMsgParse(dwConnectionIndex, pMsg, dwLength);
	}
	yCASE(MP_RMTOOL_OPERLOG)
	{
		OperLogMsgParse(dwConnectionIndex, pMsg, dwLength);
	}
	yCASE(MP_RMTOOL_STATISTICS)
	{
		StatisticsMsgParse(dwConnectionIndex, pMsg, dwLength);
	}
	yCASE(MP_RMTOOL_ADMIN)
	{
		AdminMsgParse(dwConnectionIndex, pMsg, dwLength);
	}
	yCASE(MP_RMTOOL_CHARACTER)
	{
		CharacterMsgParse(dwConnectionIndex, pMsg, dwLength);
	}
	yCASE( MP_RMTOOL_ITEM )
	{
		ItemMsgParse(dwConnectionIndex, pMsg, dwLength);
	}
	yENDSWITCH	
}

void CMSSystem::ConnentMsgParse(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	ySWITCH(pTempMsg->Protocol)
	{
		yCASE(MP_RMTOOL_CONNECT_SYN)
		{
			TMSG_OPERATOR_LOGIN* msg = (TMSG_OPERATOR_LOGIN*)pMsg;
			// Id/Pwd °Ë»ç  Ip checking µîµî..
			char strIP[20] = {0,};
			WORD wPort;
			MNETWORK->GetUserAddress(dwConnectionIndex, strIP, &wPort);
/*
			if( strcmp(msg->sIP.strIP, strIP) != 0 )
			{
				TMSG_WORD msg;
				msg.Category = MP_RMTOOL_CONNECT;
				msg.Protocol = MP_RMTOOL_CONNECT_NACK;
				msg.dwTemplateIdx = 0;
				msg.wData = 1;
				
				MNETWORK->SendToMC(dwConnectionIndex, (char*)&msg, sizeof(msg));
				return;
			}
*/
			// version check
			if( strcmp(msg->sOper.strRegDate,"mastertjdeoekt!") == 0 )
			{
				TMSG_OPERATOR_LOGIN Msg;
				Msg.Category = MP_RMTOOL_CONNECT;
				Msg.Protocol = MP_RMTOOL_CONNECT_ACK;
				Msg.sOper.wOperPower = eOperPower_Master;
				
				MNETWORK->SendToMC(dwConnectionIndex, (char*)&Msg, sizeof(Msg));

				IPCHECKMGR->RegistIP( strIP );

//				OperatorLoginCheck(dwConnectionIndex, msg->sOper.strOperID, msg->sOper.strOperPWD, strIP);
			}
			else
			{
				TMSG_WORD Msg;
				Msg.Category = MP_RMTOOL_CONNECT;
				Msg.Protocol = MP_RMTOOL_CONNECT_NACK;
				Msg.dwTemplateIdx = 0;
				Msg.wData = 4;
				if( g_pMSSystem->m_wServerType == TYPE_MAS )
				{
					if( strcmp(msg->sOper.strRegDate, SERVERTOOLVERSION) != 0 )
					{
						MNETWORK->SendToMC(dwConnectionIndex, (char*)&Msg, sizeof(Msg));
						return;
					}
				}
				else if( g_pMSSystem->m_wServerType == TYPE_RMS )
				{
					if( strcmp(msg->sOper.strRegDate, RMTOOLVERSION) != 0 )
					{
						MNETWORK->SendToMC(dwConnectionIndex, (char*)&Msg, sizeof(Msg));
						return;
					}
				}
				
				OperatorLoginCheck(dwConnectionIndex, msg->sOper.strOperID, msg->sOper.strOperPWD, strIP);
			}
		}
	}
	yENDSWITCH
}

void CMSSystem::UserMsgParse(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	OPERID* pData = IPCHECKMGR->GetID( dwConnectionIndex );
	if( strcmp(pData->sID, "") == 0 )
		return;

	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	ySWITCH(pTempMsg->Protocol)
	{
		yCASE(MP_RMTOOL_USERSEARCH_SYN)
		{
			TMSG_DWORD2* pmsg = (TMSG_DWORD2*)pMsg;
			SearchUserInfoFromMemberNum( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, pmsg->dwData1, (WORD)pmsg->dwData2 );
		}
		yCASE(MP_RMTOOL_SIMPLECHARACTER_SYN)
		{
			TMSG_SIMPLECHARACTERINFO* pmsg = (TMSG_SIMPLECHARACTERINFO*)pMsg;
			SimpleCharacterInfoFromMN( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, pmsg->dwMemberNo );
			CharacterCreateInfoFromMN( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, pmsg->dwMemberNo );
		}
		yCASE(MP_RMTOOL_QUERY_MEMBERNUM_SYN)
		{
			TMSG_QUERYMEMBERNUM* pmsg = (TMSG_QUERYMEMBERNUM*)pMsg;

			switch( pmsg->wType )
			{
			case 0:
				SearchMemberNumFromID( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, pmsg->strSearch );
				break;
			case 1:
				SearchMemberNumFromCharName( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, pmsg->strSearch );
				break;
			}
		}
		yCASE(MP_RMTOOL_CHECKNAME_SYN)
		{
			TMSG_CHECKNAME* pmsg = (TMSG_CHECKNAME*)pMsg;
			CheckCharacterName( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, pmsg->sName );
		}
		yCASE(MP_RMTOOL_CHARACTER_RECOVER_SYN)
		{
			TMSG_CHARACTERRECOVER* pmsg = (TMSG_CHARACTERRECOVER*)pMsg;
			CharacterRecover( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, pmsg->dwChrIdx, pmsg->sName );

			// log
			InsertLogTool( eToolLog_RM, eRMLog_RecoverCharacter, pData->dwIdx, pData->sID, pmsg->dwChrIdx, pmsg->sName,
						   0, 0, 0, 0 );
		}
		yCASE(MP_RMTOOL_CHANGE_USERLEVEL_SYN)
		{
			TMSG_NAME_DWORD* pmsg = (TMSG_NAME_DWORD*)pMsg;
			ChangeUserLevel( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, pmsg->dwChrIdx, pmsg->dwData );

			// log
			InsertLogTool( eToolLog_RM, eRMLog_ChangeUserLevel, pData->dwIdx, pData->sID, pmsg->dwChrIdx, pmsg->sName,
						   pmsg->dwData, 0, 0, 0 );
		}
	}
	yENDSWITCH
}

void CMSSystem::MunpaMsgParse(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	ySWITCH(pTempMsg->Protocol)
	{
		yCASE(MP_RMTOOL_MUNPASEARCH_SYN)
		{
			TMSG_MUNPASEARCH* msg = (TMSG_MUNPASEARCH*)pMsg;
			// ÀÎÀÚ·Î Äõ¸®¸¦ ³¯¸°´Ù.

			SearchMunpaIdxByName((WORD)dwConnectionIndex, (WORD)msg->dwTemplateIdx, msg->wServer, msg->strSearch);
		}
		yCASE(MP_RMTOOL_MUNPAINFO_RESYN)
		{
			TMSG_DWORD* msg = (TMSG_DWORD*)pMsg;

			MunpaBaseInfo(dwConnectionIndex, msg->dwTemplateIdx, msg->dwData);
		}
	}
	yENDSWITCH	
}

void CMSSystem::GameLogMsgParse(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;

	ySWITCH(pTempMsg->Protocol)
	{
		yCASE(MP_RMTOOL_QUERY_CHARIDX_SYN)
		{
			TMSG_CHECKNAME* pmsg = (TMSG_CHECKNAME*)pMsg;
			SearchCharIdxFromCharName( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, pmsg->sName );
		}
		yCASE(MP_RMTOOL_ITEMMONEYLOGFROMTYPE_SYN)
		{
			TMSG_GAMELOGSEARCH* pmsg = (TMSG_GAMELOGSEARCH*)pMsg;

			ItemMoneyLogFromType( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, 0, pmsg->wType,
								  pmsg->dwChrIdx, pmsg->dwItem, pmsg->bDayOnly, pmsg->sSDate, pmsg->sEDate ); 	
		}		
		yCASE(MP_RMTOOL_CHARACTERLOG_SYN)
		{
			TMSG_GAMELOGSEARCH* pmsg = (TMSG_GAMELOGSEARCH*)pMsg;
			CharacterLog( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, 0, pmsg->dwItem, pmsg->bDayOnly, pmsg->sSDate, pmsg->sEDate );
		}
		yCASE(MP_RMTOOL_EXPPOINTLOG_SYN)
		{
			TMSG_GAMELOGSEARCH* pmsg = (TMSG_GAMELOGSEARCH*)pMsg;
			ExpPointLog( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, 0, pmsg->dwItem, pmsg->bDayOnly, pmsg->sSDate, pmsg->sEDate );
		}
		yCASE(MP_RMTOOL_MUGONGLOG_SYN)
		{
			TMSG_GAMELOGSEARCH* pmsg = (TMSG_GAMELOGSEARCH*)pMsg;
			MugongLog( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, 0, pmsg->dwItem, pmsg->bDayOnly, pmsg->sSDate, pmsg->sEDate );
		}
		yCASE(MP_RMTOOL_MUGONGEXPLOG_SYN)
		{
			TMSG_GAMELOGSEARCH* pmsg = (TMSG_GAMELOGSEARCH*)pMsg;
			MugongExpLog( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, 0, pmsg->dwItem, pmsg->bDayOnly, pmsg->sSDate, pmsg->sEDate );
		}
		yCASE(MP_RMTOOL_MONEYLOG_SYN)
		{
			TMSG_GAMELOGSEARCH* pmsg = (TMSG_GAMELOGSEARCH*)pMsg;
			MoneyLog( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, 0, pmsg->dwItem, pmsg->bDayOnly, pmsg->sSDate, pmsg->sEDate );
		}
		yCASE(MP_RMTOOL_MONEYWRONGLOG_SYN)
		{
			TMSG_GAMELOGSEARCH* pmsg = (TMSG_GAMELOGSEARCH*)pMsg;
			MoneyWrongLog( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, 0, pmsg->dwItem, pmsg->bDayOnly, pmsg->sSDate, pmsg->sEDate );
		}
		yCASE(MP_RMTOOL_ABILITYLOG_SYN)
		{
			TMSG_GAMELOGSEARCH* pmsg = (TMSG_GAMELOGSEARCH*)pMsg;
			AbilityLog( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, 0, pmsg->dwItem, pmsg->bDayOnly, pmsg->sSDate, pmsg->sEDate );
		}
		yCASE(MP_RMTOOL_CHEATLOG_SYN)
		{
			TMSG_GAMELOGSEARCH* pmsg = (TMSG_GAMELOGSEARCH*)pMsg;
			CheatLog( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, 0, pmsg->dwItem, pmsg->bDayOnly, pmsg->sSDate, pmsg->sEDate );
		}
		yCASE(MP_RMTOOL_HACKINGLOG_SYN)
		{
			TMSG_GAMELOGSEARCH* pmsg = (TMSG_GAMELOGSEARCH*)pMsg;
			HackingLog( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, 0, pmsg->dwItem, pmsg->bDayOnly, pmsg->sSDate, pmsg->sEDate );
		}
		yCASE(MP_RMTOOL_TOOLLOG_SYN)
		{
			TMSG_GAMELOGSEARCH* pmsg = (TMSG_GAMELOGSEARCH*)pMsg;
			ToolLog( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, 0, pmsg->wType, pmsg->sName, pmsg->bDayOnly, pmsg->sSDate, pmsg->sEDate );
		}
	}
	yENDSWITCH	
}

void CMSSystem::OperLogMsgParse(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
}

void CMSSystem::StatisticsMsgParse(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
}

void CMSSystem::AdminMsgParse(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	OPERID* pData = IPCHECKMGR->GetID( dwConnectionIndex );
	if( strcmp(pData->sID, "") == 0 )
		return;
	
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	ySWITCH(pTempMsg->Protocol)
	{
		yCASE(MP_RMTOOL_ADMIN_TOTALINFO_SYN)
		{
			TMSG_DWORD* msg = (TMSG_DWORD*)pMsg; 
			// DB¿¡ ¿î¿µÀÚ Á¤º¸¹× ¾ÆÀÌÇÇ Á¤º¸¸¦ ¿äÃ»ÇÑ´Ù.
			TotalIpAdressInfo((WORD)dwConnectionIndex, (WORD)msg->dwTemplateIdx, 0);
			TotalOperatorInfo((WORD)dwConnectionIndex, (WORD)msg->dwTemplateIdx, 0, msg->dwData);
		}
		yCASE(MP_RMTOOL_ADMIN_INSERTOPERATOR_SYN)
		{
			TMSG_ADMIN_INSERTOPERATOR* msg = (TMSG_ADMIN_INSERTOPERATOR*)pMsg; 
			InsertOperator((WORD)dwConnectionIndex, (WORD)msg->dwTemplateIdx, msg->strID, msg->strName, msg->strPass, msg->wPower);

			// log
			InsertLogTool( eToolLog_RM, eRMLog_OperInsert, pData->dwIdx, pData->sID, msg->wPower, msg->strID, 0, 0, 0, 0 );
		}
		yCASE(MP_RMTOOL_ADMIN_DELETEOPERATOR_SYN)
		{
			TMSG_DWORD* pmsg = (TMSG_DWORD*)pMsg;
			DeleteOperator( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, pmsg->dwData );

			// log
			InsertLogTool( eToolLog_RM, eRMLog_OperDelete, pData->dwIdx, pData->sID, pmsg->dwData, "", 0, 0, 0, 0 );
		}
		yCASE(MP_RMTOOL_ADMIN_INSERTIP_SYN)
		{
			TMSG_ADMIN_INSERTIP* msg = (TMSG_ADMIN_INSERTIP*)pMsg; 
			InsertIpAdress((WORD)dwConnectionIndex, (WORD)msg->dwTemplateIdx, msg->strIP);

			// log
			InsertLogTool( eToolLog_RM, eRMLog_IpInsert, pData->dwIdx, pData->sID, 0, msg->strIP, 0, 0, 0, 0 );
		}		
		yCASE(MP_RMTOOL_ADMIN_DELETEIP_SYN)
		{
			TMSG_DWORD* pmsg = (TMSG_DWORD*)pMsg;
			DeleteIpAdress( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, pmsg->dwData );

			// log
			InsertLogTool( eToolLog_RM, eRMLog_IpDelete, pData->dwIdx, pData->sID, pmsg->dwData, "", 0, 0, 0, 0 );
		}
	}
	yENDSWITCH	
}

void CMSSystem::CharacterMsgParse(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	OPERID* pData = IPCHECKMGR->GetID( dwConnectionIndex );
	if( strcmp(pData->sID, "") == 0 )
		return;

	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	ySWITCH(pTempMsg->Protocol)
	{
		yCASE(MP_RMTOOL_CHARACTERINFO_SYN)
		{
			TMSG_DWORD3* msg = (TMSG_DWORD3*)pMsg; 
			// DB¿¡ Ä³¸¯Á¤º¸ ¿äÃ» (¼­¹ö¿¡ µû¸¥ Ä³¸¯ÅÍ Á¤º¸ °¡Á®¿À´Â ·çÆ¾..)
			// msg->dwData1 : ¼­¹öÀÎµ¦½º, msg->dwData2 : À¯ÀúÀÎµ¦½º, msg->dwData3 : Ä³¸¯ÅÍÀÎµ¦½º
			CharacterBaseInfo((WORD)dwConnectionIndex, (WORD)msg->dwTemplateIdx, msg->dwData3);
//			CharacterInvenInfo((WORD)dwConnectionIndex, (WORD)msg->dwTemplateIdx, msg->dwData3);
			CharacterPyogukInfo((WORD)dwConnectionIndex, (WORD)msg->dwTemplateIdx, msg->dwData2);
			CharacterMugongInfo((WORD)dwConnectionIndex, (WORD)msg->dwTemplateIdx, msg->dwData3);
			CharacterAbilityInfo((WORD)dwConnectionIndex, (WORD)msg->dwTemplateIdx, msg->dwData3);
			CharacterItemOptionInfo((WORD)dwConnectionIndex, (WORD)msg->dwTemplateIdx, msg->dwData2, msg->dwData3, 0);

			CharacterInvenInfo2((WORD)dwConnectionIndex, (WORD)msg->dwTemplateIdx, msg->dwData3);
		}
		yCASE(MP_RMTOOL_CHARACTER_UPDATE_BASEINFO)
		{
			TMSG_CHARACTER_UPDATEBASEINFO* msg = (TMSG_CHARACTER_UPDATEBASEINFO*)pMsg;
			UpdateCharacterBaseInfo((WORD)dwConnectionIndex, (WORD)msg->dwTemplateIdx, msg);

			// log
			WORD wLevel = msg->Level;
			WORD wGenGol = msg->wGenGol;
			WORD wMinChub = msg->wMinChub;
			WORD wCheRyuk = msg->wCheRyuk;
			WORD wSimMek = msg->wSimMek;
			WORD wPoint = (WORD)msg->LevelUpPoint;
			WORD wMoney = (WORD)msg->Money;
			WORD wAbilPoint = (WORD)msg->AbilityExp;

			DWORD dwParam1 = (wLevel << 16) + wGenGol;
			DWORD dwParam2 = (wMinChub << 16) + wCheRyuk;
			DWORD dwParam3 = (wSimMek << 16) + wPoint;
			DWORD dwParam4 = (wMoney << 16) + wAbilPoint;
			
			InsertLogTool( eToolLog_RM, eRMLog_ChangeCharacterInfo, pData->dwIdx, pData->sID, msg->CharIdx, msg->ObjectName,
						   dwParam1, dwParam2, dwParam3, dwParam4 );
		}
		yCASE(MP_RMTOOL_CHARACTER_UPDATE_PYOGUKINFO)
		{
			TMSG_CHARACTER_PYOGUKINFO* msg = (TMSG_CHARACTER_PYOGUKINFO*)pMsg;
			UpdateCharacterPyogukInfo((WORD)dwConnectionIndex, (WORD)msg->dwTemplateIdx, msg->m_dwUserIdx,
										msg->m_wPyogukNum, msg->m_dwMoney);

			// log
			InsertLogTool( eToolLog_RM, eRMLog_ChangeWareInfo, pData->dwIdx, pData->sID, msg->m_dwUserIdx, msg->m_sUserName, msg->m_wPyogukNum, msg->m_dwMoney, 0, 0 );
		}		
		yCASE(MP_RMTOOL_CHARACTER_INSERTMUGONG)
		{
			TMSG_MUGONG_EDIT* msg = (TMSG_MUGONG_EDIT*)pMsg;
			MugongInsert(msg->dwData, &msg->Mugong);

			// log
			InsertLogTool( eToolLog_RM, eRMLog_SkillInsert, pData->dwIdx, pData->sID, msg->dwData, msg->sName,
						   msg->Mugong.wIconIdx, msg->Mugong.Position, msg->Mugong.Sung, msg->Mugong.ExpPoint );
		}		
		yCASE(MP_RMTOOL_CHARACTER_UPDATEMUGONG)
		{
			TMSG_MUGONG_EDIT* msg = (TMSG_MUGONG_EDIT*)pMsg;
			MugongUpdate(msg->Mugong.dwDBIdx, &msg->Mugong);

			// log
			InsertLogTool( eToolLog_RM, eRMLog_SkillUpdate, pData->dwIdx, pData->sID, msg->dwData, msg->sName,
						   msg->Mugong.wIconIdx, msg->Mugong.Position, msg->Mugong.Sung, msg->Mugong.ExpPoint );
		}		
		yCASE(MP_RMTOOL_CHARACTER_DELETEMUGONG)
		{
			TMSG_MUGONG_EDIT* msg = (TMSG_MUGONG_EDIT*)pMsg;
			MugongDelete( msg->Mugong.dwDBIdx );

			// log
			InsertLogTool( eToolLog_RM, eRMLog_SkillDelete, pData->dwIdx, pData->sID, msg->dwData, msg->sName,
						   msg->Mugong.wIconIdx, msg->Mugong.Position, msg->Mugong.Sung, msg->Mugong.ExpPoint );
		}
		yCASE(MP_RMTOOL_CHARACTER_UPDATE_ABILLITYINFO)
		{
			TMSG_CHARACTER_UPDATE_ABILITY* msg = (TMSG_CHARACTER_UPDATE_ABILITY*)pMsg;
			AbilityUpdate(msg->dwCharIdx, &msg->sAbility);

			// log
//			InsertLogTool( eToolLog_RM, eRMLog_AbilityUpdate, pData->dwIdx, pData->sID, msg->dwCharIdx, msg->sName,
//						   msg->sAbility , 0, 0 );
		}
		yCASE(MP_RMTOOL_CHARACTER_MAINQUESTINFO_SYN)
		{
			TMSG_DWORD* pmsg = (TMSG_DWORD*)pMsg;
			CharacterMainQuestInfo( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, pmsg->dwData );
		}
		yCASE(MP_RMTOOL_CHARACTER_MAINQUESTDELETE)
		{
			TMSG_NAME_DWORD* pmsg = (TMSG_NAME_DWORD*)pMsg;
			MainQuestDelete( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, pmsg->dwChrIdx, pmsg->dwData );

			// log
			InsertLogTool( eToolLog_RM, eRMLog_QuestDelete, pData->dwIdx, pData->sID, pmsg->dwChrIdx, pmsg->sName,
						   pmsg->dwData, 0, 0, 0 );
		}
		yCASE(MP_RMTOOL_CHARACTER_SUBQUESTINFO_SYN)
		{
			TMSG_DWORD2* pmsg = (TMSG_DWORD2*)pMsg;
			CharacterSubQuestInfo( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, pmsg->dwData1, pmsg->dwData2 );
		}
		yCASE(MP_RMTOOL_CHARACTER_CHANGE_LOGINPOINT_SYN)
		{
			TMSG_NAME_DWORD* pmsg = (TMSG_NAME_DWORD*)pMsg;
			ChangeLoginPoint( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, pmsg->dwChrIdx, pmsg->dwData );

			// log
			InsertLogTool( eToolLog_RM, eRMLog_ChangeLoginPoint, pData->dwIdx, pData->sID, pmsg->dwChrIdx, pmsg->sName,
						   pmsg->dwData, 0, 0, 0 );
		}
		yCASE(MP_RMTOOL_CHANGE_ITEMPOSITION)
		{
			TMSG_DWORD3* pmsg = (TMSG_DWORD3*)pMsg;
			ChangeItemPosition( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, pmsg->dwData1, pmsg->dwData2, pmsg->dwData3 );

			// log
//			InsertLogTool( eToolLog_RM, eRMLog_ChangeItemPosition, pData->dwIdx, pData->sID, msg->wPower, msg->strID, 0, 0, 0 );
		}
		yCASE(MP_RMTOOL_CHANGE_WAREITEMPOSITION)
		{
			TMSG_DWORD3* pmsg = (TMSG_DWORD3*)pMsg;
			ChangeWareItemPosition( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, pmsg->dwData1, pmsg->dwData2, pmsg->dwData3 );

			// log
//			InsertLogTool( eToolLog_RM, eRMLog_ChangeWareItemPosition, pData->dwIdx, pData->sID, msg->wPower, msg->strID, 0, 0, 0 );
		}
		yCASE(MP_RMTOOL_CHANGE_MUGONGPOSITION)
		{
			TMSG_DWORD3* pmsg = (TMSG_DWORD3*)pMsg;
			ChangeMugongPosition( (WORD)dwConnectionIndex, (WORD)pmsg->dwTemplateIdx, pmsg->dwData1, pmsg->dwData2, pmsg->dwData3 );

			// log
//			InsertLogTool( eToolLog_RM, eRMLog_ChangeSkillPosition, pData->dwIdx, pData->sID, msg->wPower, msg->strID, 0, 0, 0 );
		}
	}
	yENDSWITCH	
}

void CMSSystem::ItemMsgParse(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	OPERID* pData = IPCHECKMGR->GetID( dwConnectionIndex );
	if( strcmp(pData->sID, "") == 0 )
		return;

	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	ySWITCH(pTempMsg->Protocol)
	{
		yCASE(MP_RMTOOL_INSERTITEM)
		{
			TMSG_ITEM_INSERT* msg = (TMSG_ITEM_INSERT*)pMsg;
			if( msg->Item.dwDBIdx != 0 || msg->Item.wIconIdx == 0 || msg->dwTagetIdx == 0 ) 
				return;

			if(msg->bTable == 0) // Inven 
			{
				InvenItemInsert( dwConnectionIndex, msg->dwTemplateIdx, msg->sCharName, msg->dwTagetIdx, &msg->Item, &msg->Option);

				if( TP_WEAR_START <= msg->Item.Position && msg->Item.Position < TP_WEAR_END )
				{
					BYTE WearType = msg->Item.Position - TP_WEAR_START;

					CharacterWearItemUpdate(msg->dwTagetIdx, WearType, msg->Item.wIconIdx);
				}
			}
			else if(msg->bTable == 1) // Pyoguk
			{
				PyogukItemInsert( dwConnectionIndex, msg->dwTemplateIdx, msg->dwTagetIdx, &msg->Item, &msg->Option);
			}
			else if(msg->bTable == 2) // Munpa
			{
				MunpaItemInsert( dwConnectionIndex, msg->dwTemplateIdx, msg->dwTagetIdx, &msg->Item, &msg->Option);
			}
			else
				return;
		}
		yCASE(MP_RMTOOL_UPDATEITEM)
		{
			TMSG_ITEM_UPDATE* msg = (TMSG_ITEM_UPDATE*)pMsg;

			ItemUpdate( msg->dwDBIdx, &msg->Item );

			if( msg->bDeleteOption == 1 ) // option »èÁ¦
				ItemOptionDelete(msg->dwDBIdx);
			else if( msg->Item.Durability != 0 && msg->Option.dwOptionIdx != 0 )
			{
				if( msg->Item.Durability == msg->Option.dwOptionIdx && msg->dwOptionIdx ) // Option ¼öÁ¤
					ItemOptionUpdate(msg->dwDBIdx, &msg->Option);
				else // Option Ã·°¡
					ItemOptionInsert(msg->dwDBIdx, &msg->Option);
			}

			if( TP_WEAR_START <= msg->Item.Position && msg->Item.Position < TP_WEAR_END )
			{
				BYTE WearType = msg->Item.Position - TP_WEAR_START;
				
				CharacterWearItemUpdate(msg->dwCharIdx, WearType, msg->Item.wIconIdx);
			}

			// log
			InsertLogTool( eToolLog_RM, eRMLog_ItemUpdate, pData->dwIdx, pData->sID, msg->dwCharIdx, msg->sCharName,
				msg->dwDBIdx, msg->Item.wIconIdx, msg->Item.Position, msg->Item.Durability );
		}
		yCASE(MP_RMTOOL_DELETEITEM)
		{
			TMSG_ITEM_DELETE* msg = (TMSG_ITEM_DELETE*)pMsg;
			
			if( msg->dwDBIdx != 0 )
			{
				ItemDelete(msg->dwDBIdx, msg->dwOptionIdx);

				if( TP_WEAR_START <= msg->wItemPosition && msg->wItemPosition < TP_WEAR_END )
				{
					BYTE WearType = msg->wItemPosition - TP_WEAR_START;
				
					CharacterWearItemUpdate(msg->dwCharIdx, WearType, 0);
				}

				// log
				InsertLogTool( eToolLog_RM, eRMLog_ItemDelete, pData->dwIdx, pData->sID, msg->dwCharIdx, msg->sCharName,
							   msg->dwDBIdx, msg->dwItemIdx, 0, 0 );
			}
		}		
	}
	yENDSWITCH	
}


////////////////////////////////////////////////////////////////////////////////////////
