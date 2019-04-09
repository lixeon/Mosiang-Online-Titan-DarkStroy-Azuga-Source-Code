#include "conetwork.h"
#include "cpio.h"
#include "mainthread.h"
#include "network.h"
#include "timer.h"
#include "net_global.h"
#include "../4DyuchiNET_Common/code_GUID.h"
#include "log.h"
#include "quemanager.h"
#include "switch_que.h"
#include "pre_connect.h"
#include "confirm.h"
#include <crtdbg.h>

Co4DyuchiNET::Co4DyuchiNET()
{
//	MessageBox(NULL,"Create Co4DyuchiNET","hoho",MB_OK);
	m_dwRefCount = 0;
}
STDMETHODIMP Co4DyuchiNET::QueryInterface(REFIID refiid, PPVOID ppv)
{
	if (IID_IUnknown == refiid)
		*ppv = this;
	else if (IID_4DyuchiNET == refiid)
		*ppv = (I4DyuchiNET*)this;
	else 
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	((IUnknown*)(*ppv))->AddRef();
	return NOERROR;
	
}
STDMETHODIMP_(ULONG) Co4DyuchiNET::AddRef(void)
{
	m_dwRefCount++;
	return m_dwRefCount;
}
STDMETHODIMP_(ULONG) Co4DyuchiNET::Release(void)
{
	m_dwRefCount--;
	if (!m_dwRefCount)
		delete this;

	return 0;
}
/*

#ifdef _NEED_CONFIRM
BOOL __stdcall Co4DyuchiNET::CreateNetwork(DESC_NETWORK* desc,DWORD dwUserAcceptInterval,DWORD dwServerAcceptInterval)
{
#ifdef _DEBUG
	int	flag = _CRTDBG_ALLOC_MEM_DF |_CRTDBG_LEAK_CHECK_DF;

	_CrtSetDbgFlag(flag);
#endif
	InitSocket();

	if (FALSE == ConfirmFromESofnetServer("license.esofnet.com",8888))
		goto lb_fail;

	if (!CheckParameter(desc))
		return FALSE;

	if (desc->dwMaxUserNum)
	{
		g_pNetworkForUser = new CNetwork(ID_NETWORK_FOR_USER,dwAcceptInterval,desc->dwMaxUserNum,desc->dwFlag);
		g_pNetworkForUser->SetBufferSizePerConnection(desc->dwUserBufferSizePerConnection);	

		g_pNetworkForUser->SetOnConnect(desc->OnAcceptUser);
		g_pNetworkForUser->SetOnDisconnect(desc->OnDisconnectUser);
		g_pNetworkForUser->SetOnRecvTCP(desc->OnRecvFromUserTCP);
		g_pNetworkForUser->SetOnRecvUDP(NULL);
	
		g_pNetworkForUser->SetMaxTransferRecvSize(desc->dwUserMaxTransferSize);
		g_pNetworkForUser->SetMaxTransferSendSize(desc->dwUserMaxTransferSize);
	}
	

	if (desc->dwMaxServerNum)
	{
		g_pNetworkForServer = new CNetwork(ID_NETWORK_FOR_SERVER,desc->dwMaxServerNum,desc->dwFlag);
		g_pNetworkForServer->SetBufferSizePerConnection(desc->dwServerBufferSizePerConnection);
		
		g_pNetworkForServer->SetOnConnect(desc->OnAcceptServer);
		g_pNetworkForServer->SetOnDisconnect(desc->OnDisconnectServer);
		g_pNetworkForServer->SetOnRecvTCP(desc->OnRecvFromServerTCP);

		g_pNetworkForServer->SetMaxTransferRecvSize(desc->dwServerMaxTransferSize);
		g_pNetworkForServer->SetMaxTransferSendSize(desc->dwServerMaxTransferSize);
	}

	StartPreConnectPool(desc->dwConnectNumAtSameTime,100);
	
	InitializeTimer();

	if (desc->dwCustomDefineEventNum)
	{
		SetMainThreadUserDefineEventNum(desc->dwCustomDefineEventNum);
	
		for (DWORD i=0; i<desc->dwCustomDefineEventNum; i++)
		{
			SetMainThreadUserDefineEventFunc(i,desc->pEvent[i].pEventFunc);
			if (desc->pEvent[i].dwPeriodicTime)
				AddTimer(desc->pEvent[i].dwPeriodicTime,GetMainThreadUserDefineEvent(i));
		}
	}
	if ((desc->dwFlag & NETDDSC_DEBUG_LOG_MASK) == NETDDSC_DEBUG_LOG)
	{
		g_pLogManager = new CLogManager;
		g_pLogManager->Initialize("eventlog.txt",8192);
	}
	
	
	StartMainThread(desc->dwFlag);
	
	InitCompletionIO(desc->dwMainMsgQueMaxBufferSize,GetMainThreadMsgEvent());
	


//	MessageBox(NULL,"CreateServer","hoho",MB_OK);
	return TRUE;
lb_fail:
	CleanSocket();
	return FALSE;

}
#else*/

BOOL __stdcall Co4DyuchiNET::CreateNetwork(DESC_NETWORK* desc,DWORD dwUserAcceptInterval,DWORD dwServerAcceptInterval,OnIntialFunc pFunc)
{
	if (!CheckParameter(desc))
		return FALSE;

	InitSocket();

	
	SetOnInitialFunc(pFunc);
	

	if (desc->dwMaxUserNum)
	{
		g_pNetworkForUser = new CNetwork(ID_NETWORK_FOR_USER,desc->dwMaxUserNum,dwUserAcceptInterval,desc->dwFlag);
		g_pNetworkForUser->SetBufferSizePerConnection(desc->dwUserBufferSizePerConnection);	

		g_pNetworkForUser->SetOnAccept(desc->OnAcceptUser);
		g_pNetworkForUser->SetOnDisconnect(desc->OnDisconnectUser);
		g_pNetworkForUser->SetOnRecvTCP(desc->OnRecvFromUserTCP);
//		g_pNetworkForUser->SetOnRecvUDP(NULL);
	
		g_pNetworkForUser->SetMaxTransferRecvSize(desc->dwUserMaxTransferSize);
		g_pNetworkForUser->SetMaxTransferSendSize(desc->dwUserMaxTransferSize);
	}
	

	if (desc->dwMaxServerNum)
	{
		g_pNetworkForServer = new CNetwork(ID_NETWORK_FOR_SERVER,desc->dwMaxServerNum,dwServerAcceptInterval,desc->dwFlag);
		g_pNetworkForServer->SetBufferSizePerConnection(desc->dwServerBufferSizePerConnection);
		
		g_pNetworkForServer->SetOnAccept(desc->OnAcceptServer);
		g_pNetworkForServer->SetOnDisconnect(desc->OnDisconnectServer);
		g_pNetworkForServer->SetOnRecvTCP(desc->OnRecvFromServerTCP);

		g_pNetworkForServer->SetMaxTransferRecvSize(desc->dwServerMaxTransferSize);
		g_pNetworkForServer->SetMaxTransferSendSize(desc->dwServerMaxTransferSize);
	}

	StartPreConnectPool(desc->dwConnectNumAtSameTime,100);
	
	InitializeTimer();

	if (desc->dwCustomDefineEventNum)
	{
		SetMainThreadUserDefineEventNum(desc->dwCustomDefineEventNum);
	
		for (DWORD i=0; i<desc->dwCustomDefineEventNum; i++)
		{
			SetMainThreadUserDefineEventFunc(i,desc->pEvent[i].pEventFunc);
			if (desc->pEvent[i].dwPeriodicTime)
				AddTimer(desc->pEvent[i].dwPeriodicTime,GetMainThreadUserDefineEvent(i),i);
		}
	}
	if ((desc->dwFlag & NETDDSC_DEBUG_LOG_MASK) == NETDDSC_DEBUG_LOG)
	{
		g_pLogManager = new CLogManager;
		g_pLogManager->Initialize("eventlog.txt",8192);
	}
	
	
	StartMainThread(desc->dwFlag);
	
	InitCompletionIO(desc->dwMainMsgQueMaxBufferSize,GetMainThreadMsgEvent());

	return TRUE;
}

void __stdcall Co4DyuchiNET::BreakMainThread()
{
	::BreakMainThread();
}


void __stdcall Co4DyuchiNET::ResumeMainThread()
{
	::ResumeMainThread();
}
void __stdcall Co4DyuchiNET::SetUserInfo(DWORD dwConnectionIndex,void* user)
{
	CConnection* con;
	g_pNetworkForUser->GetConnection(&con,dwConnectionIndex);
	if (con)
		con->SetInfo(user);
	
}
void* __stdcall Co4DyuchiNET::GetUserInfo(DWORD dwConnectionIndex)
{
	void*	pInfo = NULL;
	CConnection* con;
	g_pNetworkForUser->GetConnection(&con,dwConnectionIndex);
	if (!con)
		goto lb_return;

	pInfo = con->GetInfo();	

lb_return:
	return pInfo;
}
void __stdcall Co4DyuchiNET::SetServerInfo(DWORD dwConnectionIndex,void* server)
{
	
	CConnection* con;
	g_pNetworkForServer->GetConnection(&con,dwConnectionIndex);
	if (con)
		con->SetInfo(server);
}
void* __stdcall Co4DyuchiNET::GetServerInfo(DWORD dwConnectionIndex)
{
	void*	pInfo = NULL;

	CConnection* con;
	g_pNetworkForServer->GetConnection(&con,dwConnectionIndex);
	if (!con)
		goto lb_return;

	pInfo = con->GetInfo();	

lb_return:
	return pInfo;
}

sockaddr_in* __stdcall Co4DyuchiNET::GetServerAddress(DWORD dwConnectionIndex)
{
	sockaddr_in*	pAddr	= NULL;

	CConnection* con;
	g_pNetworkForServer->GetConnection(&con,dwConnectionIndex);
	if (!con)
		goto lb_return;

	pAddr = con->GetAddress();

lb_return:
	return pAddr;
}
sockaddr_in* __stdcall Co4DyuchiNET::GetUserAddress(DWORD dwConnectionIndex)
{
	sockaddr_in*	pAddr	= NULL;
	
	CConnection* con;
	g_pNetworkForUser->GetConnection(&con,dwConnectionIndex);
	if (!con)
		goto lb_return;

	pAddr = con->GetAddress();

lb_return:
	return pAddr;
}

BOOL __stdcall Co4DyuchiNET::GetUserAddress(DWORD dwConnectionIndex,char* pIP,WORD* pwPort)
{
	BOOL	bResult = FALSE;
	CConnection* con;
	g_pNetworkForUser->GetConnection(&con,dwConnectionIndex);
	if (!con)
		goto lb_return;

	bResult = con->GetAddress(pIP,pwPort);

lb_return:
	return bResult;
}

BOOL __stdcall Co4DyuchiNET::GetServerAddress(DWORD dwConnectionIndex,char* pIP,WORD* pwPort)
{
	BOOL	bResult = FALSE;
	CConnection* con;
	g_pNetworkForServer->GetConnection(&con,dwConnectionIndex);

	if (!con)
		goto lb_return;

	bResult = con->GetAddress(pIP,pwPort);

lb_return:
	return bResult;
}
BOOL __stdcall Co4DyuchiNET::SendToServer(DWORD dwConnectionIndex,char* msg,DWORD length,DWORD flag)
{	
	return g_pNetworkForServer->Send(dwConnectionIndex,msg,length,flag);
}
BOOL __stdcall Co4DyuchiNET::SendToUser(DWORD dwConnectionIndex,char* msg,DWORD length,DWORD flag)
{
	return g_pNetworkForUser->Send(dwConnectionIndex,msg,length,flag);
}
BOOL __stdcall Co4DyuchiNET::SendToServer(DWORD dwConnectionIndex,INET_BUF* pBuf,DWORD dwNum,DWORD flag)
{
	return g_pNetworkForServer->Send(dwConnectionIndex,(WSABUF*)pBuf,dwNum,flag);
}
BOOL __stdcall Co4DyuchiNET::SendToUser(DWORD dwConnectionIndex,INET_BUF* pBuf,DWORD dwNum,DWORD flag)
{
	return g_pNetworkForUser->Send(dwConnectionIndex,(WSABUF*)pBuf,dwNum,flag);
}
BOOL __stdcall Co4DyuchiNET::SendToServer(DWORD dwConnectionIndex,PACKET_LIST* pList,DWORD flag)
{
	return g_pNetworkForServer->Send(dwConnectionIndex,pList,flag);
}
BOOL __stdcall Co4DyuchiNET::SendToUser(DWORD dwConnectionIndex,PACKET_LIST* pList,DWORD flag)
{
	return g_pNetworkForUser->Send(dwConnectionIndex,pList,flag);
}



void __stdcall Co4DyuchiNET::CompulsiveDisconnectServer(DWORD dwConnectionIndex)
{
	g_pNetworkForServer->CompulsiveDisconnect(dwConnectionIndex);
}
void __stdcall Co4DyuchiNET::CompulsiveDisconnectUser(DWORD dwConnectionIndex)
{
	g_pNetworkForUser->CompulsiveDisconnect(dwConnectionIndex);
}

int __stdcall Co4DyuchiNET::GetServerMaxTransferRecvSize()
{
	return g_pNetworkForServer->GetMaxTransferRecvSize();
}

int __stdcall Co4DyuchiNET::GetServerMaxTransferSendSize()
{
	return g_pNetworkForServer->GetMaxTransferSendSize();
}

int __stdcall Co4DyuchiNET::GetUserMaxTransferRecvSize()
{
	return g_pNetworkForUser->GetMaxTransferRecvSize();
}

int __stdcall Co4DyuchiNET::GetUserMaxTransferSendSize()
{
	return g_pNetworkForUser->GetMaxTransferSendSize();
}

void __stdcall Co4DyuchiNET::BroadcastServer(char* pMsg,DWORD len,DWORD flag)
{
	g_pNetworkForServer->Broadcast(pMsg,len,flag);
}

void __stdcall Co4DyuchiNET::BroadcastUser(char* pMsg,DWORD len,DWORD flag)
{
	g_pNetworkForUser->Broadcast(pMsg,len,flag);
}
DWORD __stdcall Co4DyuchiNET::GetConnectedServerNum()
{
	return g_pNetworkForServer->GetConnectionNum();
}

DWORD __stdcall	Co4DyuchiNET::GetConnectedUserNum()
{
	return g_pNetworkForUser->GetConnectionNum();
}

WORD __stdcall Co4DyuchiNET::GetBindedPortServerSide()
{
	return g_pNetworkForServer->GetPort();
}

WORD __stdcall Co4DyuchiNET::GetBindedPortUserSide()
{
	return g_pNetworkForUser->GetPort();
}
BOOL __stdcall Co4DyuchiNET::ConnectToServerWithUserSide(char* szIP,WORD port,CONNECTSUCCESSFUNC SuccessFunc,CONNECTFAILFUNC FailFunc,void* pExt)
{
	return g_pNetworkForUser->ConnectToServer(szIP,port,SuccessFunc,FailFunc,pExt);
}
BOOL __stdcall Co4DyuchiNET::ConnectToServerWithServerSide(char* szIP,WORD port,CONNECTSUCCESSFUNC SuccessFunc,CONNECTFAILFUNC FailFunc,void* pExt)
{
	return g_pNetworkForServer->ConnectToServer(szIP,port,SuccessFunc,FailFunc,pExt);
}

BOOL __stdcall Co4DyuchiNET::StartServerWithUserSide(char* ip,WORD port)
{
	return g_pNetworkForUser->StartServer(ip,port);
}
BOOL __stdcall Co4DyuchiNET::StartServerWithServerSide(char* ip,WORD port)
{
	return g_pNetworkForServer->StartServer(ip,port);
}

HANDLE	__stdcall Co4DyuchiNET::GetCustomEventHandle(DWORD index)
{
	return GetMainThreadUserDefineEvent(index);
}/*
BOOL	__stdcall Co4DyuchiNET::PauseTimer(DWORD dwCustomEventIndex)
{
	return ::PauseTimer(dwCustomEventIndex);
}
BOOL	__stdcall Co4DyuchiNET::ResumeTimer(DWORD dwCustomEventIndex)
{
	return ::ResumeTimer(dwCustomEventIndex);
}*/

void Co4DyuchiNET::DestroyNetwork()
{

	printf("EndPreConnectPool Begin\n");
	EndPreConnectPool();
	printf("EndPreConnectPool Finished\n");

	printf("Release Sockets For User Begin\n");
	if (g_pNetworkForUser)
		g_pNetworkForUser->ReleaseListenSocket();
	printf("Release Sockets For User Finished\n");

	printf("Release Sockets For Server Begin\n");
	if (g_pNetworkForServer)
		g_pNetworkForServer->ReleaseListenSocket();
	printf("Release Sockets For Server Finished\n");

	printf("CloseComplitionIO() Begin\n");
	CloseComplitionIO();
	printf("CloseComplitionIO() Finished\n");

	printf("EndMainThread() Begin\n");
	EndMainThread();	
	printf("EndMainThread() Finished\n");

	printf("EndSwitchQueThread() Begin\n");
	EndSwitchQueThread();
	printf("EndSwitchQueThread() Finished\n");

	printf("RemoveAllTimer() Begin\n");
	RemoveAllTimer();
	printf("RemoveAllTimer() Finished\n");

	if (g_pICode)
	{
		printf("Remove ICode\n");
		g_pICode->Release();
		g_pICode = NULL;
		printf("Remove ICode Complete\n");
	}

	if (g_pNetworkForUser)
	{
		printf("Remove UserForNetwork\n");
		delete g_pNetworkForUser;
		g_pNetworkForUser = NULL;
		printf("Remove UserForNetwork Complete\n");
	}

	if (g_pNetworkForServer)
	{
		printf("Remove ServerForNetwork\n");
		delete g_pNetworkForServer;
		g_pNetworkForServer = NULL;
		printf("Remove ServerForNetwork Complete\n");
	}
	
	if (g_pQueManager)
	{
		// 큐매니져를 제거한다.
		printf("Remove QueManager\n");
		delete g_pQueManager;
		g_pQueManager = NULL;
		printf("Remove QueManager Complete\n");
	}
	if (g_pLogManager)
	{
		printf("Remove LogManager\n");
		delete g_pLogManager;
		g_pLogManager = NULL;
		printf("Remove LogManager Complete\n");
	}
	
	CleanSocket();
}
Co4DyuchiNET::~Co4DyuchiNET()
{
	DestroyNetwork();

#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif

//	MessageBox(NULL,"Destroyed Co4DyuchiNET","hoho",MB_OK);
}

BOOL Co4DyuchiNET::CheckParameter(DESC_NETWORK* desc)
{
	if (!desc)
	{
		ErrorMsg("parameter : NULL ptr");
		return FALSE;
	}
	if (desc->dwMaxUserNum)
	{
		if (desc->dwMaxUserNum > MAX_USER_NUM)
		{
			ErrorMsg("parameter : dwMaxUserNum invalide");
			return FALSE;
		}

		if (NULL == desc->OnRecvFromUserTCP)
		{
			ErrorMsg("parameter : OnRecvFromUserTCP invalide");
			return FALSE;
		}
		if (NULL == desc->OnAcceptUser)
		{
			ErrorMsg("parameter : OnConnectUser invalide");
			return FALSE;
		}
		if (desc->dwUserMaxTransferSize > 65535 || 0 == desc->dwUserMaxTransferSize)
		{
			ErrorMsg("parameter : dwMaxUserMaxTransferSize invalide");
			return FALSE;
		}
		if (desc->dwUserBufferSizePerConnection > MAX_USER_BUFFER_SIZE || 0 == desc->dwUserBufferSizePerConnection)
		{
			ErrorMsg("parameter : dwUserBufferSizePerConnection invalide");
			return FALSE;
		}
	}
	if (desc->dwMaxServerNum)
	{
		if (desc->dwMaxServerNum > MAX_SERVER_NUM)
		{
			ErrorMsg("parameter : dwMaxServerNum invalide");
			return FALSE;
		}
		if (NULL == desc->OnRecvFromServerTCP)
		{
			ErrorMsg("parameter : OnRecvFromServerTCP invalide");
			return FALSE;
		}
		if (NULL == desc->OnAcceptServer)
		{
			ErrorMsg("parameter : OnConnectServer invalide");
			return FALSE;
		}
		if (desc->dwServerMaxTransferSize > 65535 || 0 == desc->dwServerMaxTransferSize)
		{
			ErrorMsg("parameter : dwMaxServerMaxTransferSize invalide");
			return FALSE;
		}
		if (desc->dwServerBufferSizePerConnection > MAX_SERVER_BUFFER_SIZE || 0 == desc->dwServerBufferSizePerConnection)
		{
			ErrorMsg("parameter : dwServerBufferSizePerConnection invalide");
			return FALSE;
		}
	}

	if (!desc->dwConnectNumAtSameTime || desc->dwConnectNumAtSameTime == 0xcccccccc)
		desc->dwConnectNumAtSameTime = 1;

	if (desc->dwMainMsgQueMaxBufferSize > MAX_MSGQUE_SIZE || 0 == desc->dwMainMsgQueMaxBufferSize)
	{
		ErrorMsg("parameter : dwMainMsgQueMaxBufferSize invalide");
		return FALSE;
	}
	if (desc->dwCustomDefineEventNum > 	MAX_MAIN_THREAD_USER_DEFINE_EVENT_NUM)
	{
		ErrorMsg("parameter : dwCustomDefineEventNum invalide");
		return FALSE;
	}
	for (DWORD i=0; i<desc->dwCustomDefineEventNum; i++)
	{
		if (NULL == desc->pEvent[i].pEventFunc)
		{
			ErrorMsg("parameter : dwCustomDefineEvent invalide");
			return FALSE;
		}
	}
	return TRUE;
}

BOOL __stdcall Co4DyuchiNET::GetMyAddress(char* szOutIP,DWORD dwMaxLen)
{
	
	//m_pINet->GetMyAddress(szIP,16);

	char szHostName[40];
	memset(szHostName,0,sizeof(szHostName));
	
	::gethostname(szHostName, sizeof(szHostName));

	HOSTENT FAR* lphostent = ::gethostbyname(szHostName);
	wsprintf(szOutIP,"%s",inet_ntoa(*((struct in_addr*)lphostent->h_addr)));

	return TRUE;
}
