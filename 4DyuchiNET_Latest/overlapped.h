//
// 오버랩드 방식의 송수신에서 반드시 필요한 OVERLAPPED 구조체를 확장하고 몇가지 편리한 메소드를 추가했다.
// TCP송수신에서 없어서는 안될 SEND,RECV큐는 이 OVERLAPPED_IO 구조체로 구현되며 간단한 thread safe 가 구현되어있다.
// connection클래스와 work스레드 외에는 억세스 할 일이 없고 정해진 메소드를 통하지 않은 억세스는 하지 말기를 권한다.
//
#pragma once 

#include "../4DyuchiNET_Common/stdafx.h"
#include "../4DyuchiNET_Common/net_define.h"


#include "define.h"
#include "write_memory_handle.h"
#include "read_memory_handle.h"


// io type bit flag

// READ or WRITE [0-7] 8 bit 

enum IO_TYPE
{
	IO_TYPE_WRITE =	0x00000001,
	IO_TYPE_READ  =	0x00000002,
	IO_TYPE_WAIT  = 0x00000003
};

/*---------------------------------------------------------------------------------------------------------------*/
// 블럭 메소드가 뭔가 하면...
// send큐가 가득 차서 더 이상 send할 수 없는 경우에 어떻게 처리 할 것인지를 처리하는 옵션이다.
// OVERLAPPED_IO::SetBlockMethod()로 설정할 수 있으며 소켓 io수행중에도 옵션을 발꿀 수 있다.
// 
// ex)  CConnection* con = send할 컨넥션 포인터;
//		con->OverlappedSend.SetBlockMethod(BLOCK_METHOD_IGNORE);	send큐가 다 찼을 경우는 send함수 무시
//		con->OverlappedSEnd.SetBlockMethod(BLOCK_METHOD_WAIT);		send큐가 빌때까지 대기.서버 전체의 퍼포먼스 저하.		
// BLOCK_METHOD_WAIT는 서버의 메인 스레드가 send큐가 빌 때까지 대기하는 상태가 되므로 가능하면 쓰지 않는 것이 좋다.
// 디폴트로 접속이 이루어지면 모든 컨넥션은 BLOCK_METHOD_IGNORE로 설정된다.아주 크리티컬한 패킷을 보내야하는 경우에만 쓰도록한다.

// when it can't send because que is overflow 	[8-15] 8 bit	
enum BLOCK_METHOD
{
	BLOCK_METHOD_IGNORE =	0x0000100
//	BLOCK_METHOD_WAIT  =	0x0000200		사용안함 
};
#define		IO_TYPE_MASK				0x000000ff
#define		BLOCK_METHOD_MASK			0x0000ff00

#define		IO_TYPE_MASK_INVERSE		0xffffff00
#define		BLOCK_METHOD_MASK_INVERSE	0xffff00ff

#define		IO_TYPE_MASK_SENDABLE		0xff000000

/*---------------------------------------------------------------------------------------------------------------*/
class CNetwork;
class COverlapped
{
protected:
	OVERLAPPED				m_Overlapped;				// 이것은 기본적으로 OVERLAPPED형태여야 한다.따라서 맨 앞이 OVERLAPPED구조체	
	DWORD					m_dwIoType;					// SEND한 오버랩이냐 RECV한 오버랩이냐가 표시된다.
	DWORD					m_dwTransferredBytes;		// WSASend,WSARecv를 위해 남겨둔 4바이트.무시한다.
	SOCKET					m_socket;					// 이 오버랩과 연결된 소켓 디스크립터
	int						m_iMaxTransferSize;			// 한번에 보낼 수 있는 최대 패킷 크기
	CNetwork*				m_pNetwork;
	DWORD					m_dwFlag;
	DWORD					m_dwHandle;

public:
	void 				SetHandleID(NETWORK_ID apType,DWORD dwIndex,ACTION_TYPE acType) {m_dwHandle = dwIndex | acType | apType;} // 핸들 지정
	DWORD				GetHandleID() {return m_dwHandle;}
	void				SetNetwork(CNetwork* pNet) {m_pNetwork = pNet;}
	void				SetMaxTransferSize(int size)		{m_iMaxTransferSize = size;}	// 최대 전송 사이즈를 세팅.
	int					GetMaxTransferSize()				{return m_iMaxTransferSize;}	// 최대 전송 사이즈를 리턴.
	void				SetBlockMethod(BLOCK_METHOD bm)		{m_dwIoType = m_dwIoType & BLOCK_METHOD_MASK_INVERSE | bm;} // send큐가 다 찼을 경우의 처리 방법을 지정.
	BLOCK_METHOD		GetBlockMethod()					{return (BLOCK_METHOD)(m_dwIoType & BLOCK_METHOD_MASK);}	// send큐가 다 찼을 경우의 어떻게 처리하는지 상태를 리턴.
	void inline			SetIOType(IO_TYPE type)				{m_dwIoType = m_dwIoType & IO_TYPE_MASK_INVERSE | type;}	// send용 오버랩인지 recv용인지 세팅.
	IO_TYPE				GetIOType() {return					(IO_TYPE)(m_dwIoType & IO_TYPE_MASK);}					// send용 오버랩인지 recv용인지 리턴
	SOCKET				GetSocket()							{return m_socket;}	// socket 디스크립터 리턴
	void				SetSocket(SOCKET s)					{m_socket = s;}		// 소켓 디스크립터 값을 변경.해제했을때 세팅.

	COverlapped();
	~COverlapped();
};




