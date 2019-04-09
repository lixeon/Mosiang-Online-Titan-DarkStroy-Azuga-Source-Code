//
// 여러 work스레드에서 수집한 메시지를 한개의 메시지 처리 스레드로 보내기 위한 데이타 구조이다.
// 모든 메시지는 MSG_CONTAINER에 들어가며 각각의 메시지 타입이 부여된다.
// 이 클래스는 메시지 특성별로 push와 dispatch할 수 있다.멀티 스레드에서 동기화가 필요하기 때문에 
// lock(),Unlock()함수를 제공하며 이 클래스를 억세스 하기 위해서는 반드시 lock()을 걸 필요가 있다.
//
//#define _WIN32_WINNT 0x0500

#ifndef QUE_H
#define QUE_H

#include "../4DyuchiNET_Common/stdafx.h"
#include "../4DyuchiNET_Common/net_define.h"


struct HEADER
{
	DWORD				m_dwHeader;

	void				SetHeader(DWORD dwHandle) {m_dwHeader = dwHandle;}	// 헤더 세팅.
	void				SetActionType(ACTION_TYPE acType) {m_dwHeader &= INVERSE_ACTION_TYPE_BIT_MASK; m_dwHeader |= acType;}//액션 타입 세팅
	void				SetActorType(ACTOR_TYPE actType) { m_dwHeader &= INVERSE_ACTOR_TYPE_BIT_MASK;m_dwHeader |= actType;}// 액션 타입리턴
	ACTION_TYPE			GetActionType() { return (ACTION_TYPE)(m_dwHeader & ACTION_TYPE_BIT_MASK);}// 액션 타입리턴
	DWORD				GetConnectionIndex() { return m_dwHeader & CONNECTION_INDEX_BIT_MASK;}// 네트워크 클래스에서의 인덱스 리턴
	ACTOR_TYPE			GetActorType() {return (ACTOR_TYPE)(m_dwHeader & ACTOR_TYPE_BIT_MASK);}// 서버측인지 유져측인지 리턴
};
	

// recv한 패킷은 물론 disconnect상황이나 메인 스레드와 worker스레드간의 통신 등 각종 이벤트를
// 일관되게 처리하기 위한 데이타 구조이다.
// disconnect,recv udp,recv tcp,switch queue등은 모두 이 메시지 컨테이너로 캡슐화된다.
#define	SIZE_MSG_CONTAINER	16
struct MSG_CONTAINER
{
	HEADER						m_dwHeader;
	DWORD						m_dwSize;				// 메시지 컨테이너 사이즈
	DWORD						m_dwMsgLength;		// 메시지 길이
	DWORD						m_dwReserved;			// 범용적으로 쓰도록 남겨둔 4바이트.여기선 udp에서 사용한다.
	char						m_pMsg[1];			// 메시지를 가리키기 위한 인덱스용 배열.
	
	DWORD						GetMsgLength()			{return m_dwMsgLength;}	// 이 컨테이너에 들어있는 메시지 길이 리턴
	char*						GetMsg()				{return m_pMsg;}	// 들어있는 메시지 포인터 리턴
	DWORD						GetContainerSize()		{return m_dwSize;}	// 이 컨테이너의 사이즈 리턴
	DWORD						GetRawContainerSize()	{return SIZE_MSG_CONTAINER;}
	DWORD						GetHeader()				{return m_dwHeader.m_dwHeader;}

	void						SetActionType(ACTION_TYPE acType) {m_dwHeader.m_dwHeader &= INVERSE_ACTION_TYPE_BIT_MASK; m_dwHeader.m_dwHeader |= acType;}//액션 타입 세팅
	ACTION_TYPE					GetActionType() { return (ACTION_TYPE)(m_dwHeader.m_dwHeader & ACTION_TYPE_BIT_MASK);}// 액션 타입리턴
	DWORD						GetConnectionIndex() { return m_dwHeader.m_dwHeader & CONNECTION_INDEX_BIT_MASK;}// 네트워크 클래스에서의 인덱스 리턴
	ACTOR_TYPE					GetActorType() {return (ACTOR_TYPE)(m_dwHeader.m_dwHeader & ACTOR_TYPE_BIT_MASK);}// 서버측인지 유져측인지 리턴
	
	DWORD						GetFromAddr() {return m_dwReserved;}	// 메시지의 송신지 주소.(UDP인 경우만 해당)
	void						SetAddress(DWORD addr) {m_dwReserved = addr;}	// 메시지 송신지 주소 세팅(UDP만)
	void						Clear()	
	{
		// 멤버 변수 초기화.
		m_dwHeader.m_dwHeader = 0;
		m_dwReserved = 0;
		m_dwMsgLength = 0;
		m_dwSize = SIZE_MSG_CONTAINER;
	}
	MSG_CONTAINER()		
	{
		Clear();
	}
};
// 메시지를 처리하기 위한 메시지 큐.
// 여기에 PUSH한 메시지 컨테이너들을 메인 스레드가 한번 처리한다.
class CMsgQue
{
	DWORD					m_dwMsgNum;				// 쌓여있는 메시지 갯수
	char*					m_pBuffer;
	DWORD					m_dwOffset;				// 현재 메시지를 긁을 포인터의 위치
	DWORD					m_dwBufferPosition;		// 현재 버퍼 수위.
	DWORD					m_dwMaxBufferSize;		// 최대 버퍼 사이즈
	BOOL					m_bWaitFlag;			// 메시지 큐가 다 찼을때의 처리.TRUE이면 wait. false이면 메시지 폐기
	
	
	
public:

	void					SetWait()	{m_bWaitFlag = TRUE;}
	void					ReleaseWait() {m_bWaitFlag = FALSE;}

	DWORD	inline			GetMsgNum() {return m_dwMsgNum;}
	DWORD	inline			GetBufferPosition() {return m_dwBufferPosition;}
	DWORD	inline			GetMaxBufferSize() {return m_dwMaxBufferSize;}
	BOOL					PushMsg(DWORD dwHeader,WSABUF* pBuf,DWORD dwBufNum,DWORD dwTotalMsgSize,DWORD dwFlag,DWORD* pdwMsgNum);


	BOOL					Add(CMsgQue* pSrc);
	BOOL					Move(CMsgQue* pDest);
	
	friend BOOL				Add(CMsgQue* pSrc);
	friend BOOL				Move(CMsgQue* pDest);
	void	inline			Clear() {m_dwMsgNum = 0; m_dwBufferPosition = 0; m_dwOffset = 0;}
		

	CMsgQue(DWORD dwMaxBufferSize);
	~CMsgQue();

};
MSG_CONTAINER* GetMsgContainer(CMsgQue* pMsgQue);
extern CRITICAL_SECTION g_csWrite;

BOOL PushMsg(CMsgQue* pMsgQue,DWORD dwHeader,WSABUF* pBuf,DWORD dwBufNum,DWORD dwTotalMsgSize,DWORD dwFlag,DWORD* pdwMsgNum);

#endif
