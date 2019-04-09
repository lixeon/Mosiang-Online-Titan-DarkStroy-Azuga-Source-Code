#pragma once

//
// 접속을 받아 복수의 CONNECTION을 설정해 관리하거나 혹은 복수의 접속을 시도하여 관리할 수 있는 클래스이다.
// 서버로서의 최소한의 기능을 할 수 있고 원격지로의 복수 접속이 가능하다.
//

#include "../4DyuchiNET_Common/stdafx.h"
#include "../4DyuchiNET_Common/typedef.h"
#include "connection.h"
#include "create_index.h"

//#include "que.h"
//#include "dynamic_memory.h"
class CPrfCheckContainer;

struct CONNECTION_DESC
{
	CConnection*		pConnection;
	CONNECTION_STATUS	status;
};
/*
GLOBAL_FUNC_DLL STMPOOL_HANDLE	__stdcall	;
GLOBAL_FUNC_DLL void			__stdcall	;
GLOBAL_FUNC_DLL BOOL			__stdcall	InitializeStaticMemoryPool(STMPOOL_HANDLE pool,DWORD dwUnitSize,DWORD dwDefaultCommitNum,DWORD dwMaxNum);

GLOBAL_FUNC_DLL void*			__stdcall	LALAlloc(STMPOOL_HANDLE pool);
GLOBAL_FUNC_DLL void			__stdcall	LALFree(STMPOOL_HANDLE pool,void* pMemory);
*/

class CNetwork
{
	NETWORK_ID				m_dwNetworkID;		// 이 네트웍 클래스가 서버를 향한 것인지 유져를 향한 것인지 식별 아이디.

	CONNECTION_DESC*		m_pConnectionList;
	CIndexCreator			m_IC;

//	CConnection**			m_ppConnectionList;	// 컨넥션 포인터 테이블의 포인터.

	CConnection*			m_pRecycledConnectionHead;
	CConnection*			m_pRecycledConnectionTail;
	DWORD					m_dwRecycledConnectionNum;


	WORD					m_wPort;			// 이 네트워크 클래스가 바인딩 하고 있는 주소의 포트번호.
	DWORD					m_dwConnectionNum;	// 현재 접속하고있는 컨넥션 수.
	DWORD					m_dwMaxConnectionNum;	// 최대 컨넥션 수.
	SOCKET					m_socketBase;			// listen socket
	sockaddr_in				m_sckadrAdress;			// 바인딩할 주소 구조체.
	DWORD					m_dwBufferSizePerConnection;	// 컨넥션 당 버퍼 사이즈.이 값이 일단은 이 네트웍 클래스에 물린 모든 컨넥션에 대해 일괄 적용된다.
	int						m_iMaxTransferRecvSizePerConnection;	// 최대 recv사이즈.이 값 역시 이 네트웍 클래스에 몰린 모든 컨넥션에 대해 잃괄 적용된다. 
	int						m_iMaxTransferSendSizePerConnection;	// 최대 send사이즈.이 값 역시 이 네트웍 클래스에 몰린 모든 컨넥션에 대해 잃괄 적용된다. 
	ACCEPTFUNC				m_pOnAccept;							// 접속되었을때 수행할 함수
//	RECVFUNC				(*m_pOnRecvUDP)(DWORD dwAddr,char* pMsg,DWORD dwLength);	// UDP패킷을 수신했을때 수행할 함수.
	DISCONNECTFUNC			m_pOnDisconnect;
	RECVFUNC				m_pOnRecvTCP;
	DWORD					m_dwAcceptInterval;
	
	CDynamicMemoryContainer*	m_pDMWrite;
	CDynamicMemoryContainer*	m_pDMRead;
	BOOL					m_bEncryptionSend;
	HANDLE					m_hAcceptThread;

	CConnection*			AllocConnection();
	void					FreeConnection(CConnection* pCon);

public:
	DWORD					GetAcceptInterval()		{return m_dwAcceptInterval;}
	DWORD					GetActorType()			{return m_dwNetworkID;}
	HANDLE					GetAcceptThreadHandle()		{return m_hAcceptThread;}
	void					SetAcceptTHreadHandle( HANDLE hAcceptThread ) { m_hAcceptThread = hAcceptThread; }
	void					ReleaseListenSocket();	// listen socket 릴리즈.
	CDynamicMemoryContainer*	GetDMWrite()			{return m_pDMWrite;}
	CDynamicMemoryContainer*	GetDMRead()				{return m_pDMRead;}
	void					EnableEncryptionSend() {m_bEncryptionSend = TRUE;}
	void					DisableEncryptionSend() {m_bEncryptionSend = FALSE;}
	CConnection*			AddConnection(SOCKET socket);
	void					CompulsiveDisconnect(DWORD dwIndex);			
	int						GetMaxTransferRecvSize() {return m_iMaxTransferRecvSizePerConnection;} // 컨넥션당 최대 recv사이즈 리턴
	int						GetMaxTransferSendSize() {return m_iMaxTransferSendSizePerConnection;} // 컨넥션당 최대 send사이즈 리턴
	void					SetMaxTransferRecvSize(int size) {m_iMaxTransferRecvSizePerConnection = size;} // 컨넥셔당 최대 recv사이즈 세팅
	void					SetMaxTransferSendSize(int size) {m_iMaxTransferSendSizePerConnection = size;} // 컨넥션당 최대 send사이즈 세팅
	void					Broadcast(char* pMsg,DWORD len,DWORD flag);
	void					OnDisconnect(DWORD dwConnectionIndex) { m_pOnDisconnect(dwConnectionIndex);}	// 접속이 끊어졌을때 호출되는 함수.
	void					OnRecvTCP(DWORD dwConnectionIndex,char* msg,DWORD len) { m_pOnRecvTCP(dwConnectionIndex,msg,len);} // TCP패킷을 수신했을때 호출되는 함수.
//	void					OnRecvUDP(DWORD dwAddr,char* msg,DWORD len) { m_pOnRecvUDP(dwAddr,msg,len);}	// UDP패킷을 수신했을때 호출되는 함수.
	void					OnAccept(DWORD dwConnectionIndex) { m_pOnAccept(dwConnectionIndex);}	// 접속이 되었을때 호출되는 함수.
	ACCEPTFUNC				GetOnAcceptProc() {return m_pOnAccept;}
//	void					SetOnRecvUDP(void (*pFunc)(DWORD dwAddr,char* msg,DWORD len)) {m_pOnRecvUDP = pFunc;} // UDP패킷을 받았을때 자동호출되는 함수 세팅.
	void					SetOnRecvTCP(RECVFUNC pFunc) { m_pOnRecvTCP = pFunc;} // TCP패킷을 받았을때 자동호출되는 함수 세팅.
	void					SetOnDisconnect(DISCONNECTFUNC pFunc) {m_pOnDisconnect = pFunc;} // 접속이 끊어졌을때 자동호출되는 함수 세팅
	void					SetOnAccept(ACCEPTFUNC pFunc) { m_pOnAccept = pFunc;} // 접속이 되었을때 자동호출되는 함수 세팅.

	void					SetConnectionType(CConnection* con,NETWORK_ID net_id);	// 컨넥션당 블럭 메소드를 지정하는 함수.
	
	BOOL					Send(DWORD index,char* msg,DWORD length,DWORD flag);	
	BOOL					Send(DWORD index,WSABUF* pBuf,DWORD dwNum,DWORD flag);	
	BOOL					Send(DWORD index,PACKET_LIST* pList,DWORD flag);
	SOCKET					GetListenSocket() {return m_socketBase;}	// listen socket 디스크립터를 리턴.
	void		 			GetConnection(CConnection** ppCon,DWORD index);
	
	void					SetBufferSizePerConnection(DWORD size);
	DWORD					GetConnectionNum() {return m_dwConnectionNum;}	// 현재 컨넥션 갯수.
	WORD					GetPort() {return m_wPort;}	// 현재 바인딩 하고 있는 주소의 포트를 리턴.
	BOOL					ConnectToServer(char* szIP,WORD wPort,CONNECTSUCCESSFUNC SuccessFunc,CONNECTFAILFUNC FailFunc,void* pExt);
	BOOL					StartServer(char* ip,WORD port);	// 특정 ip와 포트로 서버를 스타트한다.
	void					CloseAllConnection();	// 이 네트웍 클래스의 모든 컨넥션을 release한다.
	void					CloseConnection(DWORD index); // 특정 인덱스의 컨넥션을 release한다.

	CNetwork(NETWORK_ID id,DWORD num,DWORD dwAcceptInterval,DWORD dwFlag);
	~CNetwork();
	
};

extern CNetwork* g_pNetworkForUser;
extern CNetwork* g_pNetworkForServer;
