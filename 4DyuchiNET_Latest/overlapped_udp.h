#pragma once

#include "overlapped.h"

// 이 구조체는 UDP패킷을 OVERLAPPED방식으로 처리하기 위한 OVERLAPPED구조체의 확장판이다.
// UDP는 connection oriented가 아니므로 주소 필드는 필수적이다.따라서 4바이트 ip영역과 포트를 포함하는 SOCKADDR_IN구조체
// 를 포함한다.
class COverlappedUDP : public COverlapped
{
	SOCKADDR_IN			m_sckAddr;	// 주소필드 어디서부터 왔는지가 write될 것이다.
	int					m_iAddrLength;	// 주소의 길이.
	char				m_pBuffer[MAX_UDP_BUFFER_SIZE];	// 패킷을 수신할 버퍼.
public:
	DWORD				GetAddress() {return m_sckAddr.sin_addr.S_un.S_addr;}
	void				Initialize();
	void				SetAddrLength(int len) {m_iAddrLength = len;}
	char*				GetBufferPtr() {return m_pBuffer;}// 버퍼 포인터 리턴
	BOOL				PrepareRead();
	COverlappedUDP();

		
};
