#pragma once

#include "../4DyuchiNET_Common/stdafx.h"

// 메모리 카피를 안할 순 없다.메모리 블럭을 체인지 하는 경우 남은 패킷을 카피할때...또 메인 메시지 큐에
// 넣을때 메모리 카피를 한다.
// 1.기본적으로 큰 길이의 블럭을 두개 할당 받는다.
// 2.OnRead할때마다 블럭의 오프셋 포인터를 뒤로 밀어준다.오프셋 포인터부터 recv한다.
// 3.recv할 공간이 100바이트 정도 이하로 떨어지면 남은 바이트들을 맨 앞으로 카피하고 오프셋도 앞으로 당긴다.
// 4.새로운 오프셋으로부터 recv한다.
// 5.메시지를 수신했음을 확인한 경우 메시지의 시작 포인터와 새로운 블럭을 할당받고 기존 메모리는 반납한다.

// 메시지 큐에 넣을때는....각 스레드마다 메시지 포인터와 길이를 담을 스트럭쳐를 넉넉하게 준비하여 메시지를 확인
// 하면 일단 이 배열에 포인터만 넣는다.큐로부터 write할 수 있는 인덱스만 할당받고 락을 바로 해제.카피한다.

#define		MIN_READ_BYTES	128
#define		MSG_LENGTH_SIZE 0x02
class CReadMemoryHandle
{
	char*					m_pMemory;
	DWORD					m_dwMemoryIndex;
	char*					m_pWritePtr;
	char*					m_pReadPtr;
	DWORD					m_dwReservedBytes;
	DWORD					m_dwUsageBytes;
	DWORD					m_dwMaxMemorySize;
	DWORD					m_dwReserved;
	
	
public:
	
	void					SetMemory(char* pBuffer,DWORD dwSize)
	{
		m_pMemory = pBuffer;
		m_dwMaxMemorySize = dwSize;
		InitializeMemory();
	}
	void					SetMemory(char* pBuffer);
	char*					GetMemory() {return m_pMemory;}
	DWORD					GetMaxMemorySize() {return m_dwMaxMemorySize;}
	DWORD					GetReservedBytes() {return m_dwReservedBytes;}
	
	char*					GetEntryPtr() {return m_pWritePtr;}

	void					OnRead(DWORD dwSize,WSABUF* pBuf,DWORD dwBufNum,DWORD* pdwTotalMsgBytes,DWORD* pdwMsgNum);
	void					OnReadC(DWORD dwSize,WSABUF* pBuf,DWORD dwBufNum,DWORD* pdwTotalMsgBytes,DWORD* pdwTotalMsgNum);
	void					InitializeMemory();
	void					ResetMemory();
	CReadMemoryHandle();
	~CReadMemoryHandle();
	

	
};
