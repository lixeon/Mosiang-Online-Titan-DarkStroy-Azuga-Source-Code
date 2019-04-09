// Network.cpp: implementation of the CNetwork class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Network.h"

#include "ServerTable.h"
#ifdef __AGENTSERVER__
#include "TrafficLog.h"
#include "UserTable.h"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL g_bReady = FALSE;

// 네트워크와 관련 된 처리들은 여기서 다함.
// send, recv, get info
CNetwork g_Network;

CNetwork::CNetwork()
{
//	CoInitialize(NULL);
	m_pINet = NULL;
}

CNetwork::~CNetwork()
{
	//Release();
//	CoUninitialize();
}
void CNetwork::Release()
{

	if(m_pINet)
	{
		m_pINet->Release();
		m_pINet = NULL;
	}
}


void CNetwork::Init(DESC_NETWORK * desc)
{
	if(!g_pServerTable) return;
	
	HRESULT hr;
	hr = CoCreateInstance( CLSID_4DyuchiNET, NULL, CLSCTX_INPROC_SERVER, IID_4DyuchiNET, (void**)&m_pINet);
	if (FAILED(hr))
		return;
	if(!m_pINet->CreateNetwork(desc))
		return;
}
/*

void CNetwork::Start()
{
	if(!g_pServerTable) return;

	SERVERINFO * MyInfo = g_pServerTable->GetSelfServer();
	ASSERT(MyInfo);
	if(!m_pINet->StartServerWithServerSide(MyInfo->szIPForServer,MyInfo->wPortForServer))
		return;
	if( MyInfo->wServerKind == AGENT_SERVER || MyInfo->wServerKind == DISTRIBUTE_SERVER )
		if(!m_pINet->StartServerWithUserSide(MyInfo->szIPForUser, MyInfo->wPortForUser))
		{
			// error
			return;
		}
		else
		{
			g_Console.LOG(4, "ClientStart IP: %s PORT: %d", MyInfo->szIPForUser, MyInfo->wPortForUser);		
		}

	
	g_Console.LOG(4, "ServerStart IP: %s PORT: %d", MyInfo->szIPForServer, MyInfo->wPortForServer);
	// batch connect to other server
	BatchConnectToOtherServer();

}

void CNetwork::BatchConnectToOtherServer()
{
	g_pServerTable->SetPositionHead();
	SERVERINFO* info;
	while(info = (SERVERINFO*)g_pServerTable->GetData())
	{
		if(info != g_pServerTable->GetSelfServer())
			m_pINet->ConnectToServerWithServerSide(info->szIPForServer,info->wPortForServer, OnConnectServerSuccess, OnConnectServerFail,(void*)info);
	}
}*/

BOOL CNetwork::StartServerServer(char * szIP, int port)
{
	return m_pINet->StartServerWithServerSide(szIP,port);
}
BOOL CNetwork::StartUserServer(char * szIP, int port)
{
	return m_pINet->StartServerWithUserSide(szIP, port);
}
BOOL CNetwork::ConnectToServer(char * szIP, int port, void * pParam)
{
	return m_pINet->ConnectToServerWithServerSide(szIP,port, OnConnectServerSuccess, OnConnectServerFail, pParam);
}

void CNetwork::Send2Server(DWORD dwConnectionIndex, char * msg, DWORD size)
{
	MSGBASE* pmsg = (MSGBASE*)msg;
	ASSERT(pmsg->Category != 0);
	ASSERT(pmsg->Category < MP_MAX);
	ASSERT(size < 512000);
	m_pINet->SendToServer(dwConnectionIndex, msg, size, FLAG_SEND_NOT_ENCRYPTION);
}

void CNetwork::Send2AgentServer(char * msg, DWORD size)
{
	MSGBASE* pmsg = (MSGBASE*)msg;
	ASSERT(pmsg->Category < MP_MAX);
	ASSERT(size < 512000);

	g_pServerTable->SetPositionHead();
	SERVERINFO* pInfo;
	while(pInfo = g_pServerTable->GetNextAgentServer())
	{
		m_pINet->SendToServer(pInfo->dwConnectionIndex, msg, size, FLAG_SEND_NOT_ENCRYPTION);
		break;
	}
}

void CNetwork::Send2User(DWORD dwConnectionIndex, char * msg, DWORD size)
{
#ifdef __AGENTSERVER__

	MSGBASE* pMsg = (MSGBASE*)msg;
	ASSERT(pMsg->Category != 0);
	ASSERT(pMsg->Category < MP_MAX);
	ASSERT(size < 512000);
//	#ifdef _CRYPTCHECK_ 
//		EnCrypt(pInfo,msg,size);
//	#endif

//KES encrypt
	USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if( g_pServerSystem->IsEnableCrypt() )
	{
		EnCrypt(pInfo,msg,size);
	}

	BOOL rt = m_pINet->SendToUser(dwConnectionIndex, msg, size, FLAG_SEND_NOT_ENCRYPTION);
/*
	if( ((MSGBASE*)msg)->Category == MP_MOVE )
	{
		TRAFFIC->AddSendMovePacket( ((MSGBASE*)msg)->dwObjectID, ((MSGBASE*)msg)->Protocol, size );
	}
*/	
#ifndef _HK_LOCAL_
	TRAFFIC->AddSendPacket(((MSGROOT*)msg)->Category, size);
#endif

#else	// distribute serever일때 
	BOOL rt = m_pINet->SendToUser(dwConnectionIndex, msg, size, FLAG_SEND_NOT_ENCRYPTION);
#endif

	
//	ASSERT(rt);
}


#ifdef __AGENTSERVER__

void CNetwork::Send2User(MSGBASE * msg, DWORD size)
{
	ASSERT(msg->Category != 0);
	ASSERT(msg->Category < MP_MAX);
	ASSERT(size < 512000);
	USERINFO * userInfo = g_pUserTableForObjectID->FindUser(msg->dwObjectID);
//#ifdef _CRYPTCHECK_ 
//	EnCrypt(userInfo,(char*)msg,size);
//#endif		
	if( g_pServerSystem->IsEnableCrypt() )
		EnCrypt(userInfo,(char*)msg,size);

	if(userInfo)
		m_pINet->SendToUser(userInfo->dwConnectionIndex, (char *)msg, size, FLAG_SEND_NOT_ENCRYPTION);
}

void CNetwork::EnCrypt(USERINFO * userInfo, char* msg,DWORD size)
{
	if( !userInfo ) return;
	int headerSize = sizeof( MSGROOT );
	if( !userInfo->crypto.Encrypt( ( char *)msg + headerSize, size - headerSize ) )
	{
		ASSERTMSG(0,"Encrypt Error");
		return;
	}
	((MSGBASE*)msg)->Code = userInfo->crypto.GetEnCRCConvertChar();

//#ifdef _CRYPTCHECK_ 
//	if(! userInfo ) return;
//	int headerSize = sizeof( MSGROOT );
//	if( !userInfo->crypto.Encrypt( ( char *)msg + headerSize, size - headerSize ) )
//	{
//		ASSERTMSG(0,"Encrypt Error");
//		return;
//	}
//	((MSGBASE*)msg)->Code = userInfo->crypto.GetEnCRCConvertChar();
//#endif
}
#endif

void CNetwork::Broadcast2Server(char * msg, DWORD size)
{
	MSGBASE* pmsg = (MSGBASE*)msg;
	ASSERT(pmsg->Category != 0);
	ASSERT(pmsg->Category < MP_MAX);
	ASSERT(size < 512000);
	m_pINet->BroadcastServer(msg, size, FLAG_SEND_NOT_ENCRYPTION);
}

void CNetwork::Broadcast2User(MSGBASE * msg, DWORD size)
{
	ASSERT(msg->Category != 0);
	ASSERT(msg->Category < MP_MAX);
	ASSERT(size < 512000);
	m_pINet->BroadcastUser((char*)msg, size, FLAG_SEND_NOT_ENCRYPTION);
}

void CNetwork::Broadcast2MapServer(char * msg, DWORD size)
{
	MSGBASE* pmsg = (MSGBASE*)msg;
	ASSERT(pmsg->Category < MP_MAX);
	ASSERT(size < 512000);

	g_pServerTable->SetPositionHead();
	SERVERINFO* pInfo;
	while(pInfo = g_pServerTable->GetNextMapServer())
	{
		m_pINet->SendToServer(pInfo->dwConnectionIndex, msg, size, FLAG_SEND_NOT_ENCRYPTION);
	}
}

void CNetwork::Broadcast2MapServerExceptOne(DWORD dwConnectionIndex, char * msg, DWORD size)
{
	MSGBASE* pmsg = (MSGBASE*)msg;
	ASSERT(pmsg->Category < MP_MAX);
	ASSERT(size < 512000);

	g_pServerTable->SetPositionHead();
	SERVERINFO* pInfo;
	while(pInfo = g_pServerTable->GetNextMapServer())
	{
		if(dwConnectionIndex == pInfo->dwConnectionIndex)
			continue;

		m_pINet->SendToServer(pInfo->dwConnectionIndex, msg, size, FLAG_SEND_NOT_ENCRYPTION);
	}	
}

void CNetwork::Broadcast2AgentServer(char* msg, DWORD size)
{
	MSGBASE* pmsg = (MSGBASE*)msg;
	ASSERT(pmsg->Category < MP_MAX);
	ASSERT(size < 512000);

	g_pServerTable->SetPositionHead();
	SERVERINFO* pInfo;
	while(pInfo = g_pServerTable->GetNextAgentServer())
	{
		m_pINet->SendToServer(pInfo->dwConnectionIndex, msg, size, FLAG_SEND_NOT_ENCRYPTION);
	}
}

void CNetwork::Broadcast2AgentServerExceptSelf(char* msg, DWORD size)
{
	MSGBASE* pmsg = (MSGBASE*)msg;
	ASSERT(pmsg->Category < MP_MAX);
	ASSERT(size < 512000);

	g_pServerTable->SetPositionHead();
	SERVERINFO* pInfo;
	
	while(pInfo = g_pServerTable->GetNextAgentServer())
	{
		m_pINet->SendToServer(pInfo->dwConnectionIndex, msg, size, FLAG_SEND_NOT_ENCRYPTION);
	}
}

void CNetwork::Broadcast2AgentServerExceptOne(DWORD dwConnectionIndex, char* msg, DWORD size)
{
	MSGBASE* pmsg = (MSGBASE*)msg;
	ASSERT(pmsg->Category < MP_MAX);
	ASSERT(size < 512000);

	g_pServerTable->SetPositionHead();
	SERVERINFO* pInfo;
	while(pInfo = g_pServerTable->GetNextAgentServer())
	{
		if(dwConnectionIndex == pInfo->dwConnectionIndex)
			continue;

		m_pINet->SendToServer(pInfo->dwConnectionIndex, msg, size, FLAG_SEND_NOT_ENCRYPTION);
	}	
	
}
void CNetwork::GetUserAddress(DWORD dwConnectionIndex, char* ip, WORD * port)
{
	m_pINet->GetUserAddress(dwConnectionIndex, ip, port);
}

void CNetwork::DisconnectUser(DWORD dwConnectionIndex)
{
	m_pINet->CompulsiveDisconnectUser(dwConnectionIndex);
}
