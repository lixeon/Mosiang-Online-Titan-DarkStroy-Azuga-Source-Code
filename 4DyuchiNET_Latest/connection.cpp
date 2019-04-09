#include "connection.h"
#include "cpio.h"
#include "net_global.h"
#include "log.h"


CConnection::CConnection()
{
	m_pInfo = NULL;
	m_dwStatus = CONNECTION_STATUS_DISCONNECTED;
	m_dwConnectionIndex = NULL;	
	memset(&m_address,0,sizeof(m_address));
	
	m_pSendOverlapped = NULL;
	m_pRecvOverlapped = NULL;
	m_pNext = NULL;
	m_pNetwork = NULL;

}

BOOL CConnection::Initialize(SOCKET s,DWORD size,int max_recv_size,int max_send_size)
{
	//변수 초기화
	m_pInfo = NULL;
	m_pNext = NULL;


	BOOL		bSwitch = TRUE;
	int iResult = setsockopt(s,IPPROTO_TCP,TCP_NODELAY,(char*)&bSwitch,sizeof(BOOL));

	


	m_dwStatus = CONNECTION_STATUS_DISCONNECTED;
	memset(&m_address,0,sizeof(m_address));
	

	if (!m_pRecvOverlapped)
		m_pRecvOverlapped = new COverlappedRecv;
	else 
		m_pRecvOverlapped->Initialize();
	
	if (!m_pSendOverlapped)
		m_pSendOverlapped = new COverlappedSend;
	else 
		m_pSendOverlapped->Initialize();

	m_pRecvOverlapped->SetNetwork(m_pNetwork);
	m_pSendOverlapped->SetNetwork(m_pNetwork);
	
	
	m_pRecvOverlapped->AllocBuffer(size);
	m_pSendOverlapped->AllocBuffer(size);

	m_pRecvOverlapped->SetMaxTransferSize(max_recv_size);
	m_pSendOverlapped->SetMaxTransferSize(max_send_size);

	m_pRecvOverlapped->SetSocket(s);
	m_pSendOverlapped->SetSocket(s);
/*	
	int optsize = 4;
	int def_send_size = 0;
	int def_recv_size = 0;

	
	
	setsockopt(s,SOL_SOCKET,SO_RCVBUF,(char*)&max_recv_size,4);
	setsockopt(s,SOL_SOCKET,SO_SNDBUF,(char*)&max_send_size,4);
	getsockopt(s,SOL_SOCKET,SO_RCVBUF,(char*)&def_recv_size,&optsize);
	getsockopt(s,SOL_SOCKET,SO_SNDBUF,(char*)&def_send_size,&optsize);
	
*/
	return TRUE;

}
// 
// 너무 중요한 Send함수
// 
BOOL CConnection::Send(char* msg,DWORD length,DWORD flag)
{
	char	txt[32];
	BOOL result = TRUE;
	DWORD	total_len;
	DWORD	checkbytes_len = 1;		// default 1bytes
	char	checkbytes[4];				// max 4bytes
	
	if (!flag)
		*(DWORD*)checkbytes = NULL;
	else
		g_pICode->Encode(checkbytes,&checkbytes_len,msg,length);

	total_len = checkbytes_len + length;
	
	m_pSendOverlapped->LockSend();

	if (m_pSendOverlapped->GetReservedBytes() <= total_len+2)
	{
		result = FALSE;
		if (g_pLogManager)
		{
			wsprintf(txt,"Send Que Overflow I:%d",m_dwConnectionIndex);
			g_pLogManager->WriteLog(txt,lstrlen(txt));
		}
		goto seg_return;
	}

	m_pSendOverlapped->PushMsg((char*)&total_len,2);
	m_pSendOverlapped->PushMsg(msg,length);
	m_pSendOverlapped->PushMsg(checkbytes,checkbytes_len);

	if (m_pSendOverlapped->GetIOType() != IO_TYPE_WAIT)
		goto seg_return;

	m_pSendOverlapped->SendBuffer();
//	SendBufferA(m_pSendOverlapped);


seg_return:
	m_pSendOverlapped->UnLockSend();
	return result;
}

BOOL CConnection::Send(char* msg,DWORD length)
{
	char	txt[32];

	BOOL result = TRUE;
		
	m_pSendOverlapped->LockSend();

	if (m_pSendOverlapped->GetReservedBytes() <= length + 2)
	{
		result = FALSE;
		if (g_pLogManager)
		{
			wsprintf(txt,"Send Que Overflow I:%d",m_dwConnectionIndex);
			g_pLogManager->WriteLog(txt,lstrlen(txt));
		}
		goto seg_return;
	}

	m_pSendOverlapped->PushMsg((char*)&length,2);
	m_pSendOverlapped->PushMsg(msg,length);

	if (m_pSendOverlapped->GetIOType() != IO_TYPE_WAIT)
		goto seg_return;

	m_pSendOverlapped->SendBuffer();


seg_return:
	m_pSendOverlapped->UnLockSend();
	return result;
}
BOOL CConnection::Send(WSABUF* pBuf,DWORD dwNum)
{	
	char	txt[32];

	BOOL result = TRUE;
	DWORD	dwSize = 0;
	for (DWORD i=0; i<dwNum; i++)
		dwSize += pBuf[i].len;

	m_pSendOverlapped->LockSend();

	if (m_pSendOverlapped->GetReservedBytes() <= dwSize + 2)
	{
		result = FALSE;
		if (g_pLogManager)
		{
			wsprintf(txt,"Send Que Overflow I:%d",m_dwConnectionIndex);
			g_pLogManager->WriteLog(txt,lstrlen(txt));
		}
		goto seg_return;
	}

	m_pSendOverlapped->PushMsg((char*)&dwSize,2);
	for (DWORD i=0; i<dwNum; i++)
	{		
		m_pSendOverlapped->PushMsg(pBuf[i].buf,pBuf[i].len);
	}

	if (m_pSendOverlapped->GetIOType() != IO_TYPE_WAIT)
		goto seg_return;

	m_pSendOverlapped->SendBuffer();


seg_return:
	m_pSendOverlapped->UnLockSend();
	return result;
	
}
BOOL CConnection::Send(PACKET_LIST* pList)
{
	char	txt[32];

	BOOL result = TRUE;
	DWORD	dwSize = 0;
	PACKET_LIST*	pCur = pList;

	while (pCur)
	{
		dwSize += pCur->dwLen;
		pCur = pCur->pNext;
	}

	m_pSendOverlapped->LockSend();

	if (m_pSendOverlapped->GetReservedBytes() <= dwSize +2 )
	{
		result = FALSE;
		if (g_pLogManager)
		{
			wsprintf(txt,"Send Que Overflow I:%d",m_dwConnectionIndex);
			g_pLogManager->WriteLog(txt,lstrlen(txt));
		}
		goto seg_return;
	}

	pCur = pList;
	m_pSendOverlapped->PushMsg((char*)&dwSize,2);
	while (pCur)
	{		
		m_pSendOverlapped->PushMsg(pCur->pMsg,pCur->dwLen);
		pCur = pCur->pNext;
	}

	if (m_pSendOverlapped->GetIOType() != IO_TYPE_WAIT)
		goto seg_return;

	m_pSendOverlapped->SendBuffer();


seg_return:
	m_pSendOverlapped->UnLockSend();
	return result;
	
}
CConnection::~CConnection()
{
	// 디스트럭터.별도로 할당해준 메모리가 있다면 여기서 해제해준다.
	Release();
}

void CConnection::CloseSocket()
{
	//
	// close한 소켓은 다시 close하지 않도록 한번 확인한다.
	//
	if (m_pRecvOverlapped->GetSocket() != INVALID_SOCKET && m_pSendOverlapped->GetSocket() != INVALID_SOCKET)
	{
		closesocket(m_pRecvOverlapped->GetSocket());
		m_pRecvOverlapped->SetSocket(INVALID_SOCKET);
		m_pSendOverlapped->SetSocket(INVALID_SOCKET);

	}
	
}
BOOL CConnection::GetAddress(char* pIP,WORD* pwport)
{
	BOOL	bResult = FALSE;
	*pwport = 0;
	memset(pIP,0,15);
	
	char* paddr = inet_ntoa(m_address.sin_addr);
	if (!paddr)
		goto lb_return;

	memcpy(pIP,paddr,lstrlen(paddr)+1);
	*pwport = ntohs(m_address.sin_port);
	bResult = TRUE;

lb_return:
	return bResult;

}
void CConnection::Release()
{
	if (m_pRecvOverlapped)
	{
		delete m_pRecvOverlapped;
		m_pRecvOverlapped = NULL;
	}
	if (m_pSendOverlapped)
	{
		delete m_pSendOverlapped;
		m_pSendOverlapped = NULL;
	}
}