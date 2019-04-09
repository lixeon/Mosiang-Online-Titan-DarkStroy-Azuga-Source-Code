// MonitorNetwork.cpp: implementation of the CMonitorNetwork class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MonitorNetwork.h"
#include "MSSystem.h"
#include "MonitorServerTable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CMonitorNetwork)

CMonitorNetwork::CMonitorNetwork()
{
	CoInitialize(NULL);
	m_pINet = NULL;
}

CMonitorNetwork::~CMonitorNetwork()
{
	CoUninitialize();
}
void CMonitorNetwork::Release()
{
	if(m_pINet)
	{
		m_pINet->Release();
		m_pINet = NULL;
	}
}


BOOL CMonitorNetwork::Init(DESC_NETWORK * desc)
{
	HRESULT hr;
	hr = CoCreateInstance( CLSID_4DyuchiNET, NULL, CLSCTX_INPROC_SERVER, IID_4DyuchiNET, (void**)&m_pINet);
	if (FAILED(hr))
		return FALSE;
	if(!m_pINet->CreateNetwork(desc))
		return FALSE;

	return TRUE;
}

void CMonitorNetwork::ConnectToMAS()
{
	m_pINet->ConnectToServerWithServerSide(MServerTable->m_pMASInfo->szIPForServer,MServerTable->m_pMASInfo->wPortForServer, CMSSystem::OnConnectMASSuccess, CMSSystem::OnConnectMASFail,(void*)MServerTable->m_pMASInfo);
}

BOOL CMonitorNetwork::StartServer(WORD type)
{
	if(type == TYPE_MAS)
	{
		char *szServerIP = MServerTable->m_pMASInfo->szIPForServer;
		WORD ServerPort = MServerTable->m_pMASInfo->wPortForServer;

		char *szUserIP = MServerTable->m_pMASInfo->szIPForUser;
		WORD UserPort = MServerTable->m_pMASInfo->wPortForUser;

		if(!m_pINet->StartServerWithServerSide(szServerIP,ServerPort))
			return FALSE;

		if(!m_pINet->StartServerWithUserSide(szUserIP,UserPort))
			return FALSE;
	}
	else if( type == TYPE_MS )
	{
		char *szServerIP = MServerTable->m_pMSInfo->szIPForServer;
		WORD ServerPort = MServerTable->m_pMSInfo->wPortForServer;

		if(!m_pINet->StartServerWithServerSide(szServerIP,ServerPort))
			return FALSE;
	}
	else if( type == TYPE_RMS )
	{
		char *szUserIP = MServerTable->m_pMASInfo->szIPForUser;
		WORD UserPort = MServerTable->m_pMASInfo->wPortForUser+1;

		if(!m_pINet->StartServerWithUserSide(szUserIP,UserPort))
			return FALSE;

		//-------------------임시 하드 코딩-------------
//		if(!m_pINet->StartServerWithUserSide("211.233.23.199", 14602))
//			return FALSE;

	}
	return TRUE;
}

//only ms
void CMonitorNetwork::SendToMapServer(DWORD dwConnectionIdx, char* msg, DWORD size)
{
	m_pINet->SendToServer(dwConnectionIdx, msg, size, FLAG_SEND_NOT_ENCRYPTION);
}
void CMonitorNetwork::SendToMapServerAll(char* msg, DWORD size)
{
	MServerTable->m_InfoTable.SetPositionHead();
	MSERVERINFO * info = NULL;
	while(info = (MSERVERINFO *)MServerTable->m_InfoTable.GetData())
	{
		if(MServerTable->GetMASInfo() != info)
		m_pINet->SendToServer(info->dwConnectionIndex, msg, size, FLAG_SEND_NOT_ENCRYPTION);
	}
}
void CMonitorNetwork::SendToMAS(char* msg, DWORD size)
{
	m_pINet->SendToServer(MServerTable->GetMASInfo()->dwConnectionIndex, msg, size, FLAG_SEND_NOT_ENCRYPTION);
}

//only mas
void CMonitorNetwork::SendToMC(DWORD dwConnectionIdx, char* msg, DWORD size)
{
	m_pINet->SendToUser(dwConnectionIdx, msg, size, FLAG_SEND_NOT_ENCRYPTION);
}
void CMonitorNetwork::SendToMS(DWORD dwConnectionIdx, char* msg, DWORD size)
{
	m_pINet->SendToServer(dwConnectionIdx, msg, size, FLAG_SEND_NOT_ENCRYPTION);
}
void CMonitorNetwork::SendToMSAll(char* msg, DWORD size)
{
	MServerTable->m_InfoTable.SetPositionHead();
	MSERVERINFO * info = NULL;
	while(info = (MSERVERINFO *)MServerTable->m_InfoTable.GetData())
	{
		m_pINet->SendToServer(info->dwConnectionIndex, msg, size, FLAG_SEND_NOT_ENCRYPTION);
	}
}

void CMonitorNetwork::GetUserAddress(DWORD dwConnectionIndex, char* ip, WORD * port)
{
	m_pINet->GetUserAddress(dwConnectionIndex, ip, port);
}
