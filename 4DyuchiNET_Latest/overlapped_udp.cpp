#include "overlapped_udp.h"

COverlappedUDP::COverlappedUDP()
{
	Initialize();
}
void COverlappedUDP::Initialize()
{
	// 컨스트럭터.변수 초기화.
	// 변수를 초기화한다.
	memset(&m_Overlapped,0,sizeof(m_Overlapped));
	m_socket = INVALID_SOCKET;
	m_dwIoType = IO_TYPE_READ;
	m_dwFlag = MSG_PARTIAL;
	m_iAddrLength = sizeof(SOCKADDR_IN);
}

BOOL COverlappedUDP::PrepareRead()
{	
	int result;
	WSABUF wsabuf;
	

	wsabuf.buf = m_pBuffer;
	wsabuf.len = MAX_UDP_BUFFER_SIZE;

	result = WSARecvFrom(
		m_socket,&wsabuf,
		1,
		&m_dwTransferredBytes,
		&m_dwFlag,
		(SOCKADDR *)&m_sckAddr,
		&m_iAddrLength,
		&m_Overlapped,
		NULL);
	
	if (result != SOCKET_ERROR)
		return TRUE;
	
	result = WSAGetLastError();

	if (result == ERROR_IO_PENDING)
		return TRUE;

	// RECV에 실패하고 심각한 에러라 하더라도 소켓을 끊지는 않는다.여기서 사용되는 소켓은 컨넥션별 소켓이 아니고
	// 나 자신의 UDP포트에 바인딩한 소켓이기 때문이다.소켓을 끊으면 어떤 UDP패킷도 받지 못한다.
	MessageBox(GetActiveWindow(),"WSARecvFrom() Error","Error",MB_OK);
	return FALSE;
}
