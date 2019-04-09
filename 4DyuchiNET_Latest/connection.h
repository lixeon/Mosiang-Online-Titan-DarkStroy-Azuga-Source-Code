#pragma once

//
// 접속(CONNECTION)이 이루어지면 해당 접속에 대한 RECV,SEND,DISCONNECT등을 처리해야한다.
// 이러한 것을 처리하기에 SOCKET디스크립터는 융통성이 너무 부족하므로 하나의 접속을 총괄할수 
// 있는 클래스를 정의한다.이 클래스가 CConnection 클래스 이다.
//

#include "../4DyuchiNET_Common/net_define.h"
#include "../4DyuchiNET_Common/typedef.h"

//#include "protocol.h"
#include "overlapped_recv.h"
#include "overlapped_send.h"



class CConnection;


class CConnection 
{
	void*				m_pInfo;			// 유져 정보를 넣을 수 있도록 예비로 포인터를 하나 설정해두었다.USERINFO는 마음대로 디자인하기 바란다.
	CONNECTION_STATUS	m_dwStatus;				// 컨넥션 상태.
	DWORD				m_dwConnectionIndex;	// 컨넥션 포인터 배열의 인덱스이다.아주 중요하게 쓰인다.
	sockaddr_in			m_address;			// 어디서 접속해왔는지 주소.
	COverlappedSend*	m_pSendOverlapped;
	COverlappedRecv*	m_pRecvOverlapped;
	CConnection*		m_pNext;
	CNetwork*			m_pNetwork;

	
	
public:
	COverlappedSend*	GetOverlappedSend()	{return m_pSendOverlapped;}
	COverlappedRecv*	GetOverlappedRecv() {return m_pRecvOverlapped;}
	void				AddNextConnection(CConnection* pCon) {m_pNext = pCon;}
	CConnection*		GetNextConnection() {return m_pNext;}
	void				CloseSocket();
	void				Release();	
	
	void 				SetHandleID(NETWORK_ID apType,DWORD dwIndex,ACTION_TYPE acType)
	{
		m_pSendOverlapped->SetHandleID(apType,dwIndex,acType);
		m_pRecvOverlapped->SetHandleID(apType,dwIndex,acType);
	}
	DWORD				GetHandleID() {return m_pRecvOverlapped->GetHandleID();}	// 핸들 리턴

	DWORD				GetConnectionIndex() {return m_dwConnectionIndex;}	// index리턴
	void				SetConnectionIndex(DWORD index) {m_dwConnectionIndex = index;}	// index세팅
	BOOL				PrepareRead() {return m_pRecvOverlapped->PrepareRead();}// recv요청.

	void				SetInfo(void* info) {if (m_dwStatus == CONNECTION_STATUS_DISCONNECTED)return; m_pInfo = info;} // 유져정보 포인터 세팅
	void*				GetInfo() {if (m_dwStatus == CONNECTION_STATUS_DISCONNECTED)return NULL;return m_pInfo;}	// 유져정보 포인터 리턴 
	void				SetConnectionStatus(CONNECTION_STATUS st) { m_dwStatus = st;}	// 상태 설정.
	DWORD				GetConnectionStatus() {return m_dwStatus;}	// 상태 리턴

	sockaddr_in*		GetAddress() {return &m_address;}	// 접속해온 주소 리턴.
	void				SetAddress(sockaddr_in* addr) {memcpy(&m_address,addr,sizeof(sockaddr_in));}	// 접속해온 주소 세팅
	BOOL				GetAddress(char* pIP,WORD* pwport);

	BOOL				Send(char* msg,DWORD length,DWORD flag);	// 가장 중요한 함수.메시지를 보낸다.
	BOOL				Send(char* msg,DWORD length);
	BOOL				Send(WSABUF* pBuf,DWORD dwNum);
	BOOL				Send(PACKET_LIST* pList);
	BOOL				Initialize(SOCKET s,DWORD size,int max_recv_size,int max_send_size);
	void				Clear();
	void				SetNetwork(CNetwork* pNet) {m_pNetwork = pNet;}
	CConnection();		
	~CConnection();
};

typedef CConnection* LPCConnection;

