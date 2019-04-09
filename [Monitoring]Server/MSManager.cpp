// MSManager.cpp: implementation of the CMSManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MSManager.h"
#include "MonitorServerTable.h"
#include "MonitorNetwork.h"
#include <stdio.h>
#include "MHFile.h"

#define DIST_SERVER_EXE_FILE			"DistributeServer.exe"
#define AGENT_SERVER_EXE_FILE			"AgentServer.exe"
#define MAP_SERVER_EXE_FILE				"MapServer.exe"
#define MURIM_SERVER_EXE_FILE			"MurimServer.exe"
#define AUTOPATCH_EXE_FILE				"MHAutoPatch_Server.exe"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CMSManager)
CMSManager::CMSManager()
{
	ReadAutoPatchVersion();
}

CMSManager::~CMSManager()
{

}
void CMSManager::SendToNotice(MSGNOTICE * pMsg, DWORD dwLen)
{
	MServerTable->SetPositionHeadInfo();
	MSERVERINFO * pInfo = NULL;
	while(pInfo = MServerTable->GetDataInfo())
	{
		if(pInfo->wServerType == AGENT_SERVER)
		{
			MNETWORK->SendToMapServer(pInfo->dwConnectionIndex, (char *)pMsg, dwLen);
		}
	}
		
}

void CMSManager::SendToChangeUserLevel( MSGROOT* pMsg, DWORD dwLen )
{
	MServerTable->SetPositionHeadInfo();
	MSERVERINFO * pInfo = NULL;
	while(pInfo = MServerTable->GetDataInfo())
	{
		if(pInfo->wServerType == DISTRIBUTE_SERVER)
		{
			MNETWORK->SendToMapServer(pInfo->dwConnectionIndex, (char *)pMsg, dwLen);
		}
	}
}

void CMSManager::SendServerState(WORD wState, MSERVERINFO* pInfo)
{
	MSGSERVERSTATE msg;
	msg.Category = MP_MORNITORSERVER;
	msg.Protocol = MP_MORNITORSERVER_MAPSERVERSTATE_ACK;
	strcpy(msg.serverState.ServerIP, pInfo->szIPForServer);
	msg.serverState.ServerPort		= pInfo->wPortForServer;
	msg.serverState.ServerType		= pInfo->wServerType;
	msg.serverState.ServerNum		= pInfo->wServerNum;
	msg.serverState.ServerState		= wState;

	pInfo->wServerState				= wState;

	MNETWORK->SendToMAS((char*)&msg, sizeof(msg));
}
void CMSManager::SendServerState(WORD wState, SERVERINFO* pInfo)
{
	MSGSERVERSTATE msg;
	msg.Category = MP_MORNITORSERVER;
	msg.Protocol = MP_MORNITORSERVER_MAPSERVERSTATE_ACK;
	strcpy(msg.serverState.ServerIP, pInfo->szIPForServer);
	msg.serverState.ServerPort		= pInfo->wPortForServer;
	msg.serverState.ServerType		= pInfo->wServerKind;
	msg.serverState.ServerNum		= pInfo->wServerNum;
	msg.serverState.ServerState		= wState;

	MNETWORK->SendToMAS((char*)&msg, sizeof(msg));
}

void CMSManager::SendServerLevel( DWORD level, MSERVERINFO* pInfo)
{
	MSGSERVERSTATE msg;
	msg.Category = MP_MORNITORSERVER;
	msg.Protocol = MP_MORNITORSERVER_CHANGE_USERLEVEL_ACK;
	strcpy(msg.serverState.ServerIP, pInfo->szIPForServer);
	msg.serverState.ServerPort		= pInfo->wPortForServer;
	msg.serverState.ServerType		= pInfo->wServerType;
	msg.serverState.ServerNum		= pInfo->wServerNum;
	msg.serverState.UserLevel		= level;

	MNETWORK->SendToMAS((char*)&msg, sizeof(msg));
}

void CMSManager::SendMSInfo()
{
	MSGMSINFO msg;
	msg.Category = MP_MORNITORSERVER;
	msg.Protocol = MP_MORNITORSERVER_MSINFO_NOTIFY;
	msg.MSInfo = *MServerTable->GetMSInfo();
	MNETWORK->SendToMAS((char *)&msg, sizeof(msg));
}

void CMSManager::ServerUnRegiest(DWORD dwConnectionIdx)
{
	MServerTable->RemoveServerForConn(dwConnectionIdx);
}
void CMSManager::AddServerRegiest(DWORD dwConnectionIdx, DWORD ProcessID, SERVERINFO * pInfo)
{
	MSERVERINFO Info;

	strcpy(Info.szIPForServer, pInfo->szIPForServer);
	Info.wPortForServer = pInfo->wPortForServer;
	strcpy(Info.szIPForUser, pInfo->szIPForUser);
	Info.wPortForUser = pInfo->wPortForUser;
	Info.wServerType = pInfo->wServerKind;
	Info.wServerNum = pInfo->wServerNum;

	LogConsole("서버 타입:%d", Info.wServerType);
	Info.dwConnectionIndex = dwConnectionIdx;
	Info.ProcessID = ProcessID;
	Info.dwExpiredTime = GetTickCount();
	Info.wServerState = SERVERSTATE_ON;
	
	MServerTable->AddServer(&Info, Info.wPortForServer);
}

BOOL CMSManager::ServerOff(WORD Port)
{
	MSERVERINFO* pInfo = MServerTable->GetServerInfoForPort( Port );
	if( pInfo )
	{
		MSGBASE msg;
		msg.Category = MP_MORNITORMAPSERVER;
		msg.Protocol = MP_MORNITORMAPSERVER_SERVEROFF_SYN;	
		MNETWORK->SendToMapServer( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
	}
	return TRUE;
}

BOOL CMSManager::KillServer(WORD Port)
{
	HANDLE hProcess;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, MServerTable->GetServerInfoForPort(Port)->ProcessID);
	BOOL bRet = TerminateProcess(hProcess, 1);
	if(!bRet) return FALSE;

	WaitForSingleObject(hProcess, INFINITE);
	CloseHandle(hProcess);

	return TRUE;
}

BOOL CMSManager::ExecuteServer(WORD ServerType, WORD ServerNum)
{
	STARTUPINFO si= {0,};
	PROCESS_INFORMATION pi;
	ZeroMemory( &si, sizeof(si) );
	ZeroMemory( &pi, sizeof(pi) );
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = 0;

	char exePath[128];
	char exeEnv[128];
	BOOL bRet = FALSE;

	switch(ServerType)
	{
	case DISTRIBUTE_SERVER:
		{
			//distribute
			sprintf(exePath, "%s\\%s", MServerTable->m_sPath, DIST_SERVER_EXE_FILE);	
			sprintf(exeEnv, MServerTable->m_sPath);	
			bRet = CreateProcess(NULL,exePath,NULL,NULL,FALSE,0,NULL,exeEnv,&si,&pi);
		}
		break;
	case AGENT_SERVER:
		{
			//agent
			sprintf(exePath, "%s\\%s %d", MServerTable->m_sPath, AGENT_SERVER_EXE_FILE, ServerNum);	
			sprintf(exeEnv, MServerTable->m_sPath);	
			bRet = CreateProcess(NULL,exePath,NULL,NULL,FALSE,0,NULL,exeEnv,&si,&pi);
		}
		break;
	case MURIM_SERVER:
		{
			//map
			sprintf(exePath, "%s\\%s %d", MServerTable->m_sPath, MURIM_SERVER_EXE_FILE, ServerNum);	
			sprintf(exeEnv, MServerTable->m_sPath);	
			bRet = CreateProcess(NULL,exePath,NULL,NULL,FALSE,0,NULL,exeEnv,&si,&pi);
		}
		break;
	case MAP_SERVER:
		{
			//map
			sprintf(exePath, "%s\\%s %d", MServerTable->m_sPath, MAP_SERVER_EXE_FILE, ServerNum);	
			sprintf(exeEnv, MServerTable->m_sPath);
			bRet = CreateProcess(NULL,exePath,NULL,NULL,FALSE,0,NULL,exeEnv,&si,&pi);
		}
		break;
	}
	return bRet;
}

BOOL CMSManager::AutoPatch()
{
	STARTUPINFO si= {0,};
	PROCESS_INFORMATION pi;
	ZeroMemory( &si, sizeof(si) );
	ZeroMemory( &pi, sizeof(pi) );
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = 0;

	char exePath[128];
	char exeEnv[128];
	BOOL bRet = FALSE;

	sprintf(exePath, "%s\\%s", MServerTable->m_sPath, AUTOPATCH_EXE_FILE);	
	sprintf(exeEnv, MServerTable->m_sPath);

	return CreateProcess(NULL,exePath,NULL,NULL,FALSE,0,NULL,exeEnv,&si,&pi);
}

void CMSManager::SendToDistribute( char* pMsg, DWORD dwLen )
{
	MServerTable->SetPositionHeadInfo();
	MSERVERINFO* pInfo = NULL;
	while(pInfo = MServerTable->GetDataInfo())
	{
		if(pInfo->wServerType == DISTRIBUTE_SERVER)
		{
			MNETWORK->SendToMapServer(pInfo->dwConnectionIndex, pMsg, dwLen);
		}
	}	
}

void CMSManager::SendToAgent( char* pMsg, DWORD dwLen )
{
	MServerTable->SetPositionHeadInfo();
	MSERVERINFO* pInfo = NULL;
	while(pInfo = MServerTable->GetDataInfo())
	{
		if(pInfo->wServerType == AGENT_SERVER)
		{
			MNETWORK->SendToMapServer(pInfo->dwConnectionIndex, pMsg, dwLen);
		}
	}
}

void CMSManager::ReadAutoPatchVersion()
{
	CMHFile file;
	if( !file.Init("MHVerInfo.ver", "r", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
	{
		strcpy( m_AutoPatchVersion, "None!!" );
		file.Release();
		return;
	}		
	strcpy(m_AutoPatchVersion, file.GetString());
	file.Release();
}