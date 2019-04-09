#include "define.h"
#include "network.h"
//#include "monitor.h"
#include "create_index.h"
#include "worker.h"
#include "accept.h"
#include "switch_que.h"
#include <mstcpip.h>

CNetwork* g_pNetwork = NULL;
void CNetwork::InitSocket()
{
//	WSACleanup();
	WORD Version;
	WSADATA wsaData;
	
	Version = MAKEWORD(2, 2);
	WSAStartup(Version, &wsaData);

}
void CNetwork::ReleaseSocket()
{
	if( WSACleanup() == SOCKET_ERROR )
	{
		_asm int 3;
	}

}
CNetwork::CNetwork(HWND hWnd,DWORD dwMaxConnectionNum, WORD wSockEventWinMsgID )
{
	InitSocket();
	m_socketListen = INVALID_SOCKET;	
	m_hProcWnd = hWnd;
	m_dwConnectionNum = 0;
	m_dwMaxConnectionNum = 30;
	m_wPort = 0;
	m_wSockEventWinMsgID = wSockEventWinMsgID;
	memset(&m_sckadrAdress,0,sizeof(sockaddr_in));
	m_pIndexCreator = new CIndexCreator;
	m_pIndexCreator->Initialize(30);

	m_ppConnectionTable = new CConnection*[m_dwMaxConnectionNum];
	memset(m_ppConnectionTable,0,sizeof(CConnection*)*m_dwMaxConnectionNum);
	StartSwitchQueThread(hWnd);
	StartWorkerThread();

	
}
BOOL CNetwork::Send(DWORD dwConnectionIndex,char* msg,DWORD length)
{
	BOOL	bResult = FALSE;

	CConnection* con = GetConnection(dwConnectionIndex);

	if (!con)
	{
//		printf("Error : Invalid ConnectionIndex\n");
		goto lb_return;
	}
	
	bResult = con->Send(msg,length);

lb_return:
	return bResult;

}
void CNetwork::CompulsiveDisconnect(DWORD dwIndex)
{
	IEPostDisconnectEvent(dwIndex);
}
BOOL CNetwork::StartServer(char* ip,WORD port)
{
	m_socketListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if(m_socketListen == INVALID_SOCKET )
	{
//		MessageBox(GetActiveWindow(),"Fail to create socket","Error",MB_OK);
		return FALSE;
	}

	m_sckadrAdress.sin_family = AF_INET;
	m_sckadrAdress.sin_port = htons(port);
	
	m_sckadrAdress.sin_addr.s_addr = inet_addr(ip);
	//m_sckadrAdress.sin_addr.s_addr = htons(INADDR_ANY);
	if(bind(m_socketListen, (struct sockaddr *)&m_sckadrAdress, sizeof(m_sckadrAdress)) < 0) 
	{
//		char txt[256];
//		memset(txt,0,256);
//		wsprintf(txt,"Fail to socket bind with port %d",port);
//		MessageBox(GetActiveWindow(),txt,"Error",MB_OK);
		closesocket(m_socketListen);
		m_socketListen = INVALID_SOCKET;
		return FALSE;
	}
	
	if(listen(m_socketListen, DEFAULT_QUEUE_NUM) == INVALID_SOCKET) 
	{
//		MessageBox(GetActiveWindow(),"Socket Listen Fail","Error",MB_OK);
		closesocket(m_socketListen);
		m_socketListen = NULL;
		
		return FALSE;
	}

	StartAcceptThread(m_socketListen,m_hProcWnd, m_wSockEventWinMsgID);
	// start accept thread

	return TRUE;
}



void CNetwork::CloseAllConnection()
{

	for (DWORD i=0; i<m_dwMaxConnectionNum; i++)
	{
		if (m_ppConnectionTable[i])
			CloseConnection(i);
	}
}


void CNetwork::CloseConnection(DWORD dwIndex)
{
	OVL_INFO		ovlInfo;
	
	if (m_ppConnectionTable[dwIndex])
	{
		ovlInfo.m_dwIndex = dwIndex;
		ovlInfo.m_socket = INVALID_SOCKET;
		ovlInfo.m_pOvlWrite = NULL;
		ovlInfo.m_pOvlRead = NULL;
		ovlInfo.m_dwFlag = NULL;
		
		m_ppConnectionTable[dwIndex]->CloseSocket();
		delete m_ppConnectionTable[dwIndex];
		m_ppConnectionTable[dwIndex]= NULL;
		
		m_dwConnectionNum--;

		ICFreeIndex(m_pIndexCreator,dwIndex);
	}
}

DWORD CNetwork::AddConnection(SOCKET s)
{
	DWORD			dwBytes = 0;
	tcp_keepalive	keep_alive;
	BOOL			alive = TRUE;
	DWORD			dwIndex = 0;
	CConnection*	pCon = NULL;
	HANDLE			hEventSend = NULL;
	HANDLE			hEventRecv = NULL;

	OVL_INFO		ovlInfo;
			
	if (m_dwConnectionNum >=m_dwMaxConnectionNum)
		goto lb_return;

	dwIndex = ICAllocIndex(m_pIndexCreator);
	if (!dwIndex)
		goto lb_return;


	
	hEventSend = IEGetEvnetHandle(dwIndex,IO_TYPE_WRITE);
	hEventRecv = IEGetEvnetHandle(dwIndex,IO_TYPE_READ);

	 pCon = new CConnection;
	 if (!pCon->Initialize(dwIndex,s,hEventSend,hEventRecv,DEFAULT_BUFFER_SIZE))
	 {
		delete pCon;
		pCon = NULL;
		dwIndex = 0;
		goto lb_return;
	 }

	// keep alive segment ¼¼ÆÃ
	setsockopt(s,SOL_SOCKET,SO_KEEPALIVE,(char*)&alive,4);

	keep_alive.onoff = 1;
	keep_alive.keepalivetime = 3000;
	keep_alive.keepaliveinterval = 1000;

	WSAIoctl(s,SIO_KEEPALIVE_VALS,(void*)&keep_alive,sizeof(keep_alive),(void*)&keep_alive,sizeof(keep_alive),
		&dwBytes,NULL,NULL);

	ovlInfo.m_pOvlWrite = pCon->GetOverlappedSend();
	ovlInfo.m_pOvlRead = pCon->GetOverlappedRecv();
	ovlInfo.m_dwIndex = pCon->GetIndex();
	ovlInfo.m_socket = pCon->GetSocket();
	ovlInfo.m_dwFlag = NULL;
	IESetOvlInfo(&ovlInfo);

	m_ppConnectionTable[dwIndex] = pCon;
	m_dwConnectionNum++;

lb_return:
	return dwIndex;

}
BOOL CNetwork::PrepareRead(DWORD	dwIndex)
{
	CConnection* pCon = GetConnection(dwIndex);
	if (pCon)
	{
		return pCon->PrepareRead();
	}
	return FALSE;
}
CConnection* CNetwork::GetConnection(DWORD dwConnectionIndex)
{
	if (dwConnectionIndex > m_dwMaxConnectionNum)
		return NULL;

	return m_ppConnectionTable[dwConnectionIndex];
}
DWORD CNetwork::ConnectToServer(char* szIP,WORD port)
{
	sockaddr_in addr;
	SOCKET csock;
	DWORD	dwIndex;

	

	csock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	
	if(csock == INVALID_SOCKET )
	{
//		MessageBox(GetActiveWindow(),"Fail to create socket","Error",MB_OK);
		return NULL;
	}

	addr.sin_family = AF_INET;
	addr.sin_port   = htons(port);
	addr.sin_addr.s_addr = inet_addr(szIP);
	
	if(connect(csock, (LPSOCKADDR)&addr, sizeof(addr)) < 0)
	{
//		MessageBox(GetActiveWindow(),"Fail to connect","Error",MB_OK);
	
		closesocket(csock);
		csock = 0;
		return NULL;
	}
	dwIndex = AddConnection(csock);

	if (dwIndex)
		PrepareRead(dwIndex);

	return dwIndex;
}

CNetwork::~CNetwork()
{	
	if( !_CrtCheckMemory() )
	{
		_asm int 3
	}
	if (m_socketListen != INVALID_SOCKET)
		closesocket(m_socketListen);

	EndAcceptThread();
	
	EndWorkerThread();
	EndSwitchQueThread();

	CloseAllConnection();

	if( !_CrtCheckMemory() )
	{
		_asm int 3
	}
	
	if (m_ppConnectionTable)
	{
		delete [] m_ppConnectionTable;
		m_ppConnectionTable = NULL;
	}

	if (m_pIndexCreator)
	{
		delete m_pIndexCreator;
		m_pIndexCreator = NULL;
	}
	if( !_CrtCheckMemory() )
	{
		_asm int 3
	}

	ReleaseSocket();
	ReleaseMsgQue();

	if( !_CrtCheckMemory() )
	{
		_asm int 3
	}

}