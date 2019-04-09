#include "connection.h"

CConnection::CConnection()
{
}
BOOL CConnection::Initialize(DWORD dwIndex,SOCKET s,HANDLE hEventSend,HANDLE hEventRecv,DWORD size)
{
	memset(this,0,sizeof(CConnection));
	
	int max_recv_size = 0;
	int max_send_size = 0;
		
	m_ovlRecv.Initialize(dwIndex,s,hEventRecv,size);
	m_ovlSend.Initialize(dwIndex,s,hEventSend,size);
	m_dwIndex = dwIndex;
	
	
	setsockopt(s,SOL_SOCKET,SO_RCVBUF,(char*)&max_recv_size,4);
	setsockopt(s,SOL_SOCKET,SO_SNDBUF,(char*)&max_send_size,4);


	return TRUE;

	
}


BOOL CConnection::Send(char* msg,DWORD length)
{
	BOOL	bResult;

	m_ovlSend.Lock();
	
	bResult = m_ovlSend.Push(msg,length);

	if (!bResult)
		goto lb_return;
	
	if (m_ovlSend.GetIOType() != IO_TYPE_WAIT)
		goto lb_return;
	
	bResult = m_ovlSend.SendBuffer();
	
lb_return:
	m_ovlSend.Unlock();

	return bResult;
}

void CConnection::CloseSocket()
{
	closesocket(GetSocket());
	SetSocket(INVALID_SOCKET);
}

CConnection::~CConnection()
{


}
