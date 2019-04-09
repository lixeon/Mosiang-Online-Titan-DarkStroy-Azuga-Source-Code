//#define _WIN32_WINNT=0x0500

#include "network.h"
#include "cpio.h"
#include "log.h"
#include "accept.h"
#include "connect.h"

CNetwork* g_pNetworkForUser = NULL;
CNetwork* g_pNetworkForServer = NULL;


//
// 컨스트럭터 
//
CNetwork::CNetwork(NETWORK_ID id,DWORD num,DWORD dwAcceptInterval,DWORD dwFlag)
{
	// 변수 초기화
	m_pDMWrite = NULL;
	m_pDMRead = NULL;
	m_pRecycledConnectionHead = NULL;
	m_pRecycledConnectionTail = NULL;
	m_dwRecycledConnectionNum = NULL;
	m_bEncryptionSend = FALSE;
	m_iMaxTransferRecvSizePerConnection = DEFAULT_TRANSFER_RECV_SIZE;
	m_iMaxTransferSendSizePerConnection = DEFAULT_TRANSFER_SEND_SIZE;
	m_dwBufferSizePerConnection = 0;
	m_socketBase = INVALID_SOCKET;
	m_dwConnectionNum = 0;
	m_dwMaxConnectionNum = num; 
	m_dwNetworkID = id;
	m_pConnectionList = NULL;
	m_dwConnectionNum = 0;
	m_wPort = 0;
	m_dwAcceptInterval = dwAcceptInterval;

//	Modified by chan78 at 2002/07/22
	m_hAcceptThread;

	if ( (dwFlag & NETDDSC_ENCRYPTION_MASK) == NETDDSC_ENCRYPTION)
		EnableEncryptionSend();
	else
		DisableEncryptionSend();

	memset(&m_sckadrAdress,0,sizeof(sockaddr_in));
	
	// 컨넥션 포인터 배열 생성.초기화
	m_pConnectionList = new CONNECTION_DESC[m_dwMaxConnectionNum+1];
	memset(m_pConnectionList,0,sizeof(CONNECTION_DESC)*(m_dwMaxConnectionNum+1));

	m_IC.Initialize(m_dwMaxConnectionNum);
}
//
// listen socket 릴리즈
//
void CNetwork::ReleaseListenSocket()
{
	EndAcceptThread( m_socketBase, m_hAcceptThread );
	SetAcceptTHreadHandle( NULL );
}

void CNetwork::CompulsiveDisconnect(DWORD dwIndex)
{
	if (!dwIndex)
		return;

	CConnection* pCon;
	GetConnection(&pCon,dwIndex);

	if (pCon)
	{
		PostDisconnectEvent(pCon->GetHandleID());
	}
	
}
CConnection* CNetwork::AllocConnection()
{
	CConnection* pCon = NULL;

	if (!m_dwRecycledConnectionNum)
	{
		pCon = new CConnection;
		goto lb_return;
		
	}
	pCon = m_pRecycledConnectionHead;
	m_pRecycledConnectionHead = m_pRecycledConnectionHead->GetNextConnection();
	
	m_dwRecycledConnectionNum--;

lb_return:
	return pCon;

}
void CNetwork::FreeConnection(CConnection* pCon)
{
	pCon->AddNextConnection(NULL);
	
	if (!m_dwRecycledConnectionNum)
		m_pRecycledConnectionTail = m_pRecycledConnectionHead = pCon;
	else 
		m_pRecycledConnectionTail->AddNextConnection(pCon);
	
	m_pRecycledConnectionTail = pCon;
	m_dwRecycledConnectionNum++;
}

//
// send함수.
// 실제 메시지를 send하는 함수.
BOOL CNetwork::Send(DWORD index,char* msg,DWORD length,DWORD flag)
{
	//
	// Send 함수.CConnectoin::Send()의 랩퍼 함수이다.차이점은 index를 인자로 받아 index의 connnection이
	// 실제 존재하는지 확인 후 send한다.
	//
	BOOL	bResult = FALSE;
	if (index > m_dwMaxConnectionNum)
		goto lb_return;

	if (!index)
		goto lb_return;

	if (m_pConnectionList[index].status != CONNECTION_STATUS_CONNECTED)
		goto lb_return;
	
	
	bResult = m_pConnectionList[index].pConnection->Send(msg,length);

lb_return:
	return bResult;
}
BOOL CNetwork::Send(DWORD index,WSABUF* pBuf,DWORD dwNum,DWORD flag)
{
	//
	// Send 함수.CConnectoin::Send()의 랩퍼 함수이다.차이점은 index를 인자로 받아 index의 connnection이
	// 실제 존재하는지 확인 후 send한다.
	//
	BOOL	bResult = FALSE;

	if (index > m_dwMaxConnectionNum)
		goto lb_return;

	if (!index)
		goto lb_return;

	if (m_pConnectionList[index].status != CONNECTION_STATUS_CONNECTED)
		goto lb_return;

	bResult = m_pConnectionList[index].pConnection->Send(pBuf,dwNum);
	

lb_return:
	return bResult;
}
BOOL CNetwork::Send(DWORD index,PACKET_LIST* pList,DWORD flag)
{	
	//
	// Send 함수.CConnectoin::Send()의 랩퍼 함수이다.차이점은 index를 인자로 받아 index의 connnection이
	// 실제 존재하는지 확인 후 send한다.
	//
	BOOL	bResult = FALSE;

	if (index > m_dwMaxConnectionNum)
		goto lb_return;

	if (!index)
		goto lb_return;

	if (m_pConnectionList[index].status != CONNECTION_STATUS_CONNECTED)
		goto lb_return;

	bResult = m_pConnectionList[index].pConnection->Send(pList);
	
lb_return:
	return bResult;
}
//
// 서버 스타트
//
BOOL CNetwork::StartServer(char* ip,WORD port)
{
	//
	// 인자로 받은 ip와 port로 서버를 가동한다.
	//
	char txt[128];
	memset(txt,0,128);

	// 리슨 소켓을 만든다.이 소켓은 네트웍 디바이스에 바인딩 될 것이다.
	m_socketBase = socket(AF_INET, SOCK_STREAM, 0);

	if(m_socketBase == INVALID_SOCKET )
	{
		MessageBox(GetActiveWindow(),"Fail to create socket","Error",MB_OK);
		return FALSE;
	}

	// 주소 세팅.
	m_sckadrAdress.sin_family = AF_INET;
	m_sckadrAdress.sin_port = htons(port);
	m_sckadrAdress.sin_addr.s_addr = inet_addr(ip);
	//m_sckadrAdress.sin_addr.s_addr = htons(INADDR_ANY);

	// 드디어 네트웍 디바이스에 바인딩한다.
	if(bind(m_socketBase, (struct sockaddr *)&m_sckadrAdress, sizeof(m_sckadrAdress)) < 0) 
	{
		closesocket(m_socketBase);
		m_socketBase = INVALID_SOCKET;
		
		return FALSE;
	}
	
	// 접속을 요청을 기다린다.
	if(listen(m_socketBase,SOMAXCONN) < 0) 
	{
		MessageBox(GetActiveWindow(),"Socket Listen Fail","Error",MB_OK);
		closesocket(m_socketBase);
		m_socketBase = INVALID_SOCKET;
		
		return FALSE;
	}

	StartAcceptThread(this);

	return TRUE;
}

void CNetwork::GetConnection(CConnection** ppCon,DWORD dwConnectionIndex)
{
	 // index를 이용해서 Connection 클래스 포인터를 리턴
	if (dwConnectionIndex > m_dwMaxConnectionNum)
		*ppCon = NULL;
	else	
		*ppCon = m_pConnectionList[dwConnectionIndex].pConnection; 

	return;
}

void CNetwork::CloseAllConnection()
{
	//
	// 이 네트웍 클래스의 모든 접속을 끊어버린다.
	//
	if (!m_pConnectionList)
		goto lb_return;

	DWORD	i;
	for (i=1; i<=m_dwMaxConnectionNum; i++)
	{
		if (m_pConnectionList[i].pConnection)
		{
			// 혹시 send 버퍼가 다 차서 블럭된 녀석이 있을지 모르므로...
			// CConnection클래스 인스턴스 제거.
			m_pConnectionList[i].pConnection->CloseSocket();
			FreeConnection(m_pConnectionList[i].pConnection);
			m_pConnectionList[i].pConnection = NULL;
			m_pConnectionList[i].status = CONNECTION_STATUS_DISCONNECTED;
		}

	}
	m_dwConnectionNum = 0;
lb_return:
	return;
}

void CNetwork::CloseConnection(DWORD dwConnectionIndex)
{
	// 
	// 특정 connection을 제거한다.정확히는 이미 접속이 끊긴 connection에 대해 리소스를 해제하는것이다.
	// 이 함수가 호출되는 시점은 worker스레드에서 접속이 끊김을 감지하고 이 사실을 메인 스레드에 알려준 다음이다.
	// 분명히 이 함수는 메인 스레드에서 호출되어야 한다. 임의로 이 함수를 다른 스레드에서 호출할 시에는 매우 위험한
	// 결과를 초래한다.
	//


	if (m_pConnectionList[dwConnectionIndex].status != CONNECTION_STATUS_CONNECTED)
		goto lb_return;

	

	// connection클래스 포인터 배열의 셀을 해당 connection의 인덱스로 찾아 null로 초기화한다.
	// 이제 CNetwork::Send()함수를 수행할 경우 이 connection에 대해선 false를 리턴할 것이다.
	CConnection* pConnection;
	pConnection = m_pConnectionList[dwConnectionIndex].pConnection;	
	m_pConnectionList[dwConnectionIndex].pConnection = NULL;
	m_pConnectionList[dwConnectionIndex].status = CONNECTION_STATUS_DISCONNECTED;

	
	
	if (pConnection)
	{
		// 접속이 끊기고 리소스를 해제했음을 디스플레이 한다.
		if (pConnection->GetConnectionIndex() != dwConnectionIndex)
			__asm int 3

		ICFreeIndex(&m_IC,pConnection->GetConnectionIndex());

		pConnection->SetConnectionStatus(CONNECTION_STATUS_DISCONNECTED);
		pConnection->CloseSocket();
		FreeConnection(pConnection);
		m_dwConnectionNum--;
	}

	if (g_pLogManager)
	{
		char txt[32];
		wsprintf(txt,"disconnectd %d",dwConnectionIndex);
		g_pLogManager->WriteLog(txt,lstrlen(txt));
	}
lb_return:
	return;
}

CConnection* CNetwork::AddConnection(SOCKET socket)
{
	char	txt[32];

	CConnection* pCon = NULL;
	
	DWORD	dwConnectionIndex = ICAllocIndex(&m_IC);
	if (!dwConnectionIndex)
		goto lb_return;


	// 네트워크 클래스의 connection클래스 포인터 배열에 지금 생성한 connection클래스의 포인터를 추가한다.
	// 또한 index를 세팅하고 핸들을 부여한다.이 핸들은 컴플리션 포트를 비롯하여 io처리에 대한 헤더역할을 한다.


	// CConnection클래스를 생성한다.인자로 버퍼 사이즈와 최대 버퍼 사이즈를 넘겨준다.
	// 버퍼 사이즈를 임의로 지정해준 경우는 지정해준 사이즈만큼 send,recv버퍼를 잡는다.아니라면 MAX_BUFFER_SIZE로 정의된 만큼
	// 버퍼 사이즈를 잡는다.
	pCon = AllocConnection();
	pCon->SetNetwork(this);
	pCon->Initialize(socket,m_dwBufferSizePerConnection,GetMaxTransferRecvSize(),GetMaxTransferSendSize());
	
	pCon->SetConnectionIndex(dwConnectionIndex);
	m_pConnectionList[dwConnectionIndex].pConnection = pCon;
	m_pConnectionList[dwConnectionIndex].status = CONNECTION_STATUS_CONNECTED;
	pCon->SetConnectionStatus(CONNECTION_STATUS_CONNECTED);

	pCon->SetHandleID(m_dwNetworkID,pCon->GetConnectionIndex(),ACTION_TYPE_TCP);
	
	
	sockaddr_in addr;
	memset(&addr,0,sizeof(sockaddr));
	
	int len;
	len = sizeof(sockaddr);
	
	int result;
	result = getpeername (socket,(sockaddr*)&addr,&len);

	pCon->SetAddress(&addr);
	


	// keep alive segment 세팅
	BOOL	alive;
	alive = TRUE;
	setsockopt(socket,SOL_SOCKET,SO_KEEPALIVE,(char*)&alive,4);

	tcp_keepalive keep_alive;
	keep_alive.onoff = 1;
	keep_alive.keepalivetime = 3000;
	keep_alive.keepaliveinterval = 1000;

	DWORD dwBytes;
	dwBytes = 0;
	WSAIoctl(socket,SIO_KEEPALIVE_VALS,(void*)&keep_alive,sizeof(keep_alive),(void*)&keep_alive,sizeof(keep_alive),
		&dwBytes,NULL,NULL);

	// send버퍼가 다 찼을 경우에 어떻게 처리할지 세팅한다.디폴트로 send버퍼가 찼을 경우는 메시지를 폐기하는 
	// 옵션이 들어가있다.
	SetConnectionType(pCon,m_dwNetworkID);

	// 컴플리션 포트에 바인딩한다.이제부터 방금 접속한 녀석의 소켓에 관련된 이벤트는 컴플리션 포트에서 알려준다.
	if (CreateIoCompletionPort((HANDLE)socket, g_hCompletionPort, (DWORD)(pCon->GetHandleID()),0) == NULL)
	{
		MessageBox(GetActiveWindow(),"CreateIoCompletionPort failed with error","Error",MB_OK);
		return FALSE;
	}
	
	if (g_pLogManager)
	{

		wsprintf(txt,"AddConnection %d",pCon->GetConnectionIndex());
		g_pLogManager->WriteLog(txt,lstrlen(txt));
	}
	m_dwConnectionNum++;	// 현재 접속하고 있는 총 connection수를 하나 증가.
lb_return:

	return pCon;
}
BOOL CNetwork::ConnectToServer(char* szIP,WORD wPort,CONNECTSUCCESSFUNC SuccessFunc,CONNECTFAILFUNC FailFunc,void* pExt)
{
	//
	// 다른 서버로 접속을 시도하는 함수이다.
	// 이 네트워크 모듈이 클라이언트로 작동하는 경우, 분산 서버 시스템에서 동등한 입장으로 다른 서버에 접속하는 
	// 경우 이 함수를 사용한다.
	return Connect(szIP,wPort,SuccessFunc,FailFunc,this,pExt);

}
void CNetwork::SetConnectionType(CConnection* con,NETWORK_ID net_id)
{
	// 
	// connection 타입에 따라 send버퍼가 찬 경우의 행동을 설정해주는 함수이다.
	// 따로 수행할 필요는 없다.현재는 버퍼가 찰 경우 무조건 이후에 send하는 메시
	// 지는 폐기한다.
	//

	if (net_id == ID_NETWORK_FOR_USER)
	{
		con->GetOverlappedSend()->SetBlockMethod(BLOCK_METHOD_IGNORE);
		con->GetOverlappedRecv()->SetBlockMethod(BLOCK_METHOD_IGNORE);
	}
	else if (net_id == ID_NETWORK_FOR_SERVER)
	{
		con->GetOverlappedSend()->SetBlockMethod(BLOCK_METHOD_IGNORE);
		con->GetOverlappedRecv()->SetBlockMethod(BLOCK_METHOD_IGNORE);

//		con->m_SendOverlapped.SetBlockMethod(BLOCK_METHOD_WAIT);
//		con->m_RecvOverlapped.SetBlockMethod(BLOCK_METHOD_WAIT);
	}
	else
		MessageBox(GetActiveWindow(),"Network ID illegal","Error",MB_OK);

}

void CNetwork::Broadcast(char* pMsg,DWORD len,DWORD flag)
{
	for (DWORD i=1; i<=m_dwMaxConnectionNum; i++)
	{
		if (m_pConnectionList[i].pConnection)
		{
			if (m_bEncryptionSend)
				m_pConnectionList[i].pConnection->Send(pMsg,len,flag);
			else
				m_pConnectionList[i].pConnection->Send(pMsg,len);
		}
	}
}
void CNetwork::SetBufferSizePerConnection(DWORD size)
{
	if (m_pDMWrite || m_pDMRead)
		return;

	
	m_pDMRead = new CDynamicMemoryContainer;
	m_pDMWrite = new CDynamicMemoryContainer;
	m_pDMRead->Initialize(size,3,m_dwMaxConnectionNum,MEMORY_TYPE_FIXED);
	m_pDMWrite->Initialize(size,3,m_dwMaxConnectionNum,MEMORY_TYPE_CHAIN);

	m_dwBufferSizePerConnection = size;	// 컨넥션당 버퍼 사이즈를 세팅
}


CNetwork::~CNetwork()
{

	// 남아있는 컨넥션을 정리하고 메모리를 해제한다.
	
	CloseAllConnection();	// 모든 컨넥션 릴리즈 

	// 컨넥션 포인터 테이블 릴리즈
	if (m_pConnectionList)
	{
		delete [] m_pConnectionList;
		m_pConnectionList = NULL;

	}

	CConnection* cur = m_pRecycledConnectionHead;
	CConnection* next = NULL;

	while (cur)
	{
		next = cur->GetNextConnection();
		delete cur;
		cur = next;
		
	}
	if (m_pDMRead)
	{
		delete m_pDMRead;
		m_pDMRead = NULL;
	}

	if (m_pDMWrite)
	{
		delete m_pDMWrite;
		m_pDMWrite = NULL;
	}
	if( m_hAcceptThread && ((DWORD)m_hAcceptThread != 0xFFFFFFFF) )
		CloseHandle(m_hAcceptThread);

}

