//
//	메시지를 송수신하는 Completion I/O의 코드가 모두 들어있는 소스 코드이다.
//	최소한의 입출력을 위한 스레드 코드는 모두 여기 들어있다.
//  accept,send,recv,disconnect 이벤트의 체크는 모두 여기서 이루어진다.
//

//#define _WIN32_WINNT 0x0500

#include "cpio.h"
#include "quemanager.h"
#include "network.h"
#include "../4DyuchiNET_Common/net_define.h"
#include "overlapped_udp.h"
#include "net_global.h"
#include "switch_que.h"
#include "log.h"
#include <process.h>

HANDLE g_hCompletionPort = NULL;	// 컴플리션 포트의 핸들

DWORD dwWorkThreadNum = 0;			// recv,send처리를 담당할 복수의 스레드 갯수.worker스레드라 부른다.

//HANDLE	g_hEndEvent[MAX_WORKER_THREAD_NUM];// worker스레드가 종료되었음을 알리는 이벤트 핸들.
HANDLE	g_hWorkerThread[MAX_WORKER_THREAD_NUM];
WSABUF*	g_pWsaBuf[MAX_WORKER_THREAD_NUM];

UINT WINAPI ServerWorkerThread(LPVOID lpVoid);	// worker스레드
UINT WINAPI ServerWorkerThreadLog(LPVOID lpVoid);


DWORD	GetWorkerThreadNum()
{
	return dwWorkThreadNum;
}

//
// 소켓 api를 초기화하는 함수.
//
void InitSocket()
{	//
	// 윈속 버젼 2.2로 시작한다.현재까진 최신 버젼이다.
	//
	WORD Version;
	WSADATA wsaData;
	
	WSACleanup();
	Version = MAKEWORD(2, 2);
	WSAStartup(Version, &wsaData);

}
//
// 컴플리션 포트를 생성하고 초기화한다.
//
BOOL InitCompletionIO(DWORD size,HANDLE hMsgHandle)
{
	memset(g_pWsaBuf,0,sizeof(WSABUF*)*MAX_WORKER_THREAD_NUM);

	//
	// 컴플리션 포트를 초기화 하고, 컴플리션 포트가 이벤트에 따라 사용할 스레드를 생성한다.
	// 첫번째 인자는 메인 스레드와 worker스레드 모두 억세스하게 될 메인 메시지 큐의 사이즈이다.
	// 두번째 인자는 메시지를 수집했을때 메인 스레드에게 알려줄 이벤트 핸들이다.
	//

	// 메인 메시지 큐는 두개가 만들어진다.최소한 두개 이상의 스레드가 동시 작동하게 하기 위해 메시지를 수집해서 
	// 큐에 push하는 동안 메인 스레드가 또 다른 큐에 쌓인 메시지를 처리한다.
	// 따라서 큐는 writable queue와 readable queue로 나뉜다.이 큐들을 관리하고 switch할 수 있는 방법을 
	// 제공하는 것이 CQueManager이다.
	g_pQueManager = new CQueManager;
	g_pQueManager->Initialize(size,hMsgHandle);


	// 전역 변수 초기화
	SYSTEM_INFO SystemInfo;
	UINT ThreadID = 0;
	
	// 컴플리션 포트 생성.
	if ((g_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) == NULL)
	{
		MessageBox(GetActiveWindow(),"CreateIoCompletionPort failed with error","Error",MB_OK);
		return FALSE;
	}
	
	// cpu갯수를 얻는다.
	GetSystemInfo(&SystemInfo);
	for (DWORD i = 0; i < SystemInfo.dwNumberOfProcessors*2; i++)
	{
		g_pWsaBuf[i] = new WSABUF[MAX_DETECTABLE_MSG_NUM];
		memset(g_pWsaBuf[i],0,sizeof(WSABUF)*MAX_DETECTABLE_MSG_NUM);

		if (g_pLogManager)
			g_hWorkerThread[i] = (HANDLE)_beginthreadex(NULL,NULL,ServerWorkerThreadLog,(LPVOID)i,NULL,&ThreadID);
		else 
			g_hWorkerThread[i] = (HANDLE)_beginthreadex(NULL,NULL,ServerWorkerThread,(LPVOID)i,NULL,&ThreadID);


		if (0xffffffff == (DWORD)g_hWorkerThread[i])
		{
			MessageBox(NULL,"Fail to Create WorkerThread","Error",MB_OK);
			return FALSE;
		}
		dwWorkThreadNum++;
	}
	StartSwitchQueThread(hMsgHandle,dwWorkThreadNum);
	
	return TRUE;
}

//
// 그 유명한 worker스레드.메시지를 수집하고 실제 send되지 못하고 버퍼에 남아있는 메시지를 마저 보낸다.
//
DWORD endcount = 0;

UINT WINAPI ServerWorkerThread(LPVOID lpVoid)
{
	DWORD			dwBufNum = MAX_DETECTABLE_MSG_NUM;
	
	// 여기 선언된 변수들은 이 스레드의 스택에 선언된 변수들이므로 thread safe하다.
	DWORD			dwTotalMsgNum = 0;
	DWORD			dwTotalMsgBytes = 0;
	DWORD			dwWorkIndex = (DWORD)lpVoid;	// 이 스레드 고유의 인덱스.인자로 넘겨준다.
	WSABUF*			pBuf = g_pWsaBuf[dwWorkIndex];
	BOOL			bResult = FALSE;
	DWORD			BytesTransferred = 0;			// 전송된 바이트 사이즈를 담을 변수
	COverlapped*	pIO;							// Overlapped 방식으로 요청한 io에 대한 overlapped포인터를 받아올 포인터
	CMsgQue*		pMsgQue = g_pQueManager->GetWritableQue();	// 받은 메시지를 push할 메시지 큐 포인터.언제나 라이트 가능한 큐만 억세스한다.
	HEADER			dwHeader;
	DWORD			dwQueMsgNum = 0;

//	SOCKET			socket;							// Commented by chan78 at 2001/10/17


	while(TRUE)
  	{

		
		if (!GetQueuedCompletionStatus(g_hCompletionPort, &BytesTransferred,(LPDWORD)&dwHeader, (LPOVERLAPPED *)&pIO, INFINITE))
		{
			if (GetLastError() != ERROR_NETNAME_DELETED)
				goto seg_lp;
		}


		// GetQueuedCompletionStatus()함수를 리턴하는 경우는 컴플리션 포트가 io요청이 완료된 것을 체크했기 때문에
		// 스레드를 깨운것이다.이제 이 스레드는 suspend상태로 돌아갈때까지 방금 발생한 이벤트를 처리한다.
		// 여기서 처리할 이벤트는 send,recv,disconnect에 대한 것이다.disconnect는 좀 경우가 다르지만 이 경우도 
		// 특정 메시지를 수신한것으로 간주하여 메시지 큐에 넣을것이다.그렇게 하므로서 disconnect시의동기화 문제를 막을 
		// 수 있다.
		
		if (dwHeader.GetActionType() == ACTION_TYPE_DESTROY)	// 이 서버 프로그램을 종료하라는 메시지이다.
			goto seg_destroy;									// 종료 코드로 점프.

		if (!BytesTransferred || dwHeader.GetActionType() == ACTION_TYPE_DISCONNECT )
		{
			dwHeader.SetActionType(ACTION_TYPE_DISCONNECT);
			 

			EnterCriticalSection(g_csSwitchQue+dwWorkIndex);
	
			bResult = PushMsg(
				g_pQueManager->GetWritableQue(),
				dwHeader.m_dwHeader,
				NULL,
				1,
				0,
				0,
				&dwQueMsgNum
				);
			
			LeaveCriticalSection(g_csSwitchQue+dwWorkIndex);
			if (dwQueMsgNum)
				SwitchQue();
			
			goto seg_lp;
		}

		if (!pIO)
			goto seg_lp;

		if (dwHeader.GetActionType() == ACTION_TYPE_TCP)
		{

			// tcp패킷을 수신했거나 이전에 수행한 WSASend()에 대해 결과를 얻은 경우이다.
			// recv했다면 완성된 메시지로 존재하는지 체크해서 완성된 메시지 갯수만큼 메시지 큐에 넣고
			// send에 대한 결과값을 받았다면 send큐에서 보낸만큼을 제거한다.

			if (pIO->GetIOType() == IO_TYPE_READ)
			{			

				((COverlappedRecv*)pIO)->OnRead(BytesTransferred,pBuf,dwBufNum,&dwTotalMsgBytes,&dwTotalMsgNum);
					
				if (dwTotalMsgNum)
				{
					// 여기부터 메시지 큐를 체인지 할 수 없다.
					EnterCriticalSection(g_csSwitchQue+dwWorkIndex);
							
					bResult = PushMsg(g_pQueManager->GetWritableQue(),
						dwHeader.m_dwHeader,
						pBuf,
						dwTotalMsgNum,
						dwTotalMsgBytes,
						0,
						&dwQueMsgNum
						);

					LeaveCriticalSection(g_csSwitchQue+dwWorkIndex);
					
				}
				if (dwQueMsgNum)
					SwitchQue();

				((COverlappedRecv*)pIO)->ResetBuffer();
				((COverlappedRecv*)pIO)->PrepareRead();
				goto seg_lp;

			}
			else if (pIO->GetIOType() == IO_TYPE_WRITE)
			{
				// 이전에 수행한 WSASend()에 대한 결과를 얻은 경우.
				// 여기서 방금 요청한 WSASend()의 결과로 몇바이트를 송신했는지 알 수 있다.
				
				((COverlappedSend*)pIO)->LockSend();	// 이 순간에 다른 스레드에서 send작업을 하고 있을지 모르므로 lock을 건다.
				((COverlappedSend*)pIO)->OnWrite(BytesTransferred);	// 실제 send에 성공한 바이트 수만큼 큐에서 제거.

				if (((COverlappedSend*)pIO)->GetUsageBytes())
					((COverlappedSend*)pIO)->SendBuffer();
					//SendBufferA((COverlappedSend*)pIO);
				else 
					pIO->SetIOType(IO_TYPE_WAIT);	// 이제는 메인 스레드에서 WSASend()함수를 수행해도 된다.
					
				((COverlappedSend*)pIO)->UnLockSend();
				goto seg_lp;
			}
		}
seg_lp:
		dwHeader.SetHeader(0);
	}
seg_destroy:
	_endthreadex(0);

	return 0;
}
UINT WINAPI ServerWorkerThreadLog(LPVOID lpVoid)
{
	DWORD			dwBufNum = MAX_DETECTABLE_MSG_NUM;
	
	// 여기 선언된 변수들은 이 스레드의 스택에 선언된 변수들이므로 thread safe하다.
	DWORD			dwTotalMsgNum = 0;
	DWORD			dwTotalMsgBytes = 0;
	DWORD			dwWorkIndex = (DWORD)lpVoid;	// 이 스레드 고유의 인덱스.인자로 넘겨준다.
	WSABUF*			pBuf = g_pWsaBuf[dwWorkIndex];
	BOOL			bResult = FALSE;
	DWORD			BytesTransferred = 0;			// 전송된 바이트 사이즈를 담을 변수
	COverlapped*	pIO;							// Overlapped 방식으로 요청한 io에 대한 overlapped포인터를 받아올 포인터
	CMsgQue*		pMsgQue = g_pQueManager->GetWritableQue();	// 받은 메시지를 push할 메시지 큐 포인터.언제나 라이트 가능한 큐만 억세스한다.
	HEADER			dwHeader;
	DWORD			dwQueMsgNum = 0;

//	BOOL			bCompletionResult;				// Commented by chan78 at 2001/10/17
//	SOCKET			socket;							// Commented by chan78 at 2001/10/17


	while(TRUE)
  	{

		
		if (!GetQueuedCompletionStatus(g_hCompletionPort, &BytesTransferred,(LPDWORD)&dwHeader, (LPOVERLAPPED *)&pIO, INFINITE))
		{
			if (GetLastError() != ERROR_NETNAME_DELETED)
				goto seg_lp;
		}

		// GetQueuedCompletionStatus()함수를 리턴하는 경우는 컴플리션 포트가 io요청이 완료된 것을 체크했기 때문에
		// 스레드를 깨운것이다.이제 이 스레드는 suspend상태로 돌아갈때까지 방금 발생한 이벤트를 처리한다.
		// 여기서 처리할 이벤트는 send,recv,disconnect에 대한 것이다.disconnect는 좀 경우가 다르지만 이 경우도 
		// 특정 메시지를 수신한것으로 간주하여 메시지 큐에 넣을것이다.그렇게 하므로서 disconnect시의동기화 문제를 막을 
		// 수 있다.
		
		if (dwHeader.GetActionType() == ACTION_TYPE_DESTROY)	// 이 서버 프로그램을 종료하라는 메시지이다.
			goto seg_destroy;									// 종료 코드로 점프.

		if (!BytesTransferred || dwHeader.GetActionType() == ACTION_TYPE_DISCONNECT )
		{
			dwHeader.SetActionType(ACTION_TYPE_DISCONNECT);
			 

			EnterCriticalSection(g_csSwitchQue+dwWorkIndex);
	
			bResult = PushMsg(
				g_pQueManager->GetWritableQue(),
				dwHeader.m_dwHeader,
				NULL,
				1,
				0,
				0,
				&dwQueMsgNum
				);
			
			LeaveCriticalSection(g_csSwitchQue+dwWorkIndex);
			if (dwQueMsgNum)
				SwitchQue();
			
			goto seg_lp;
		}

		if (!pIO)
			goto seg_lp;

		if (dwHeader.GetActionType() == ACTION_TYPE_TCP)
		{

			// tcp패킷을 수신했거나 이전에 수행한 WSASend()에 대해 결과를 얻은 경우이다.
			// recv했다면 완성된 메시지로 존재하는지 체크해서 완성된 메시지 갯수만큼 메시지 큐에 넣고
			// send에 대한 결과값을 받았다면 send큐에서 보낸만큼을 제거한다.

			if (pIO->GetIOType() == IO_TYPE_READ)
			{			
				((COverlappedRecv*)pIO)->OnRead(BytesTransferred,pBuf,dwBufNum,&dwTotalMsgBytes,&dwTotalMsgNum);
					
				if (dwTotalMsgNum)
				{
					// 여기부터 메시지 큐를 체인지 할 수 없다.
					EnterCriticalSection(g_csSwitchQue+dwWorkIndex);
							
					bResult = PushMsg(g_pQueManager->GetWritableQue(),
						dwHeader.m_dwHeader,
						pBuf,
						dwTotalMsgNum,
						dwTotalMsgBytes,
						0,
						&dwQueMsgNum
						);

					LeaveCriticalSection(g_csSwitchQue+dwWorkIndex);
					
					if (!bResult)
					{
						g_pLogManager->WriteLog("MainMsgQue Overflow",lstrlen("MainMsgQue Overflow"));
					}

					
				}
				if (dwQueMsgNum)
					SwitchQue();

				((COverlappedRecv*)pIO)->ResetBuffer();
				((COverlappedRecv*)pIO)->PrepareRead();
				goto seg_lp;

			}
			else if (pIO->GetIOType() == IO_TYPE_WRITE)
			{
				// 이전에 수행한 WSASend()에 대한 결과를 얻은 경우.
				// 여기서 방금 요청한 WSASend()의 결과로 몇바이트를 송신했는지 알 수 있다.
				
				((COverlappedSend*)pIO)->LockSend();	// 이 순간에 다른 스레드에서 send작업을 하고 있을지 모르므로 lock을 건다.
				((COverlappedSend*)pIO)->OnWrite(BytesTransferred);	// 실제 send에 성공한 바이트 수만큼 큐에서 제거.

				if (((COverlappedSend*)pIO)->GetUsageBytes())
					((COverlappedSend*)pIO)->SendBuffer();
					//SendBufferA((COverlappedSend*)pIO);
				else 
					pIO->SetIOType(IO_TYPE_WAIT);	// 이제는 메인 스레드에서 WSASend()함수를 수행해도 된다.
					
				((COverlappedSend*)pIO)->UnLockSend();
				goto seg_lp;
			}
		}
seg_lp:
		dwHeader.SetHeader(0);
	}
seg_destroy:
	_endthreadex(0);

	return 0;
}

//
// 컴플리션 포트를 해제하고 기타 글로벌 리소스를 해제.
//
// Modified by chan78 at 2002/07/22
void CloseComplitionIO()
{

	// 컴플리션 포트를 해제한다.또한 각종 커널 오브젝트 핸들을 닫는다.
	// 스레드도 여기서 종료시킨다.모든 스레드가 정상 종료될때까지 대기한다.
	if (g_hCompletionPort)
	{
		// worker스레드를 종료시킨다.
		for (DWORD i=0; i<dwWorkThreadNum; i++)
			PostQueuedCompletionStatus(g_hCompletionPort, 0,ACTION_TYPE_DESTROY, NULL);

		WaitForMultipleObjects( dwWorkThreadNum, g_hWorkerThread, TRUE, INFINITE );

		
		for (DWORD i=0; i<dwWorkThreadNum; i++)
		{
			delete [] g_pWsaBuf[i];
		}

		for (DWORD i=0; i<dwWorkThreadNum; i++)
		{
			CloseHandle( g_hWorkerThread[i] );
			g_hWorkerThread[i] = NULL;
		}

		// 컴플리션 포트의 핸들을 닫는다.
		CloseHandle(g_hCompletionPort);
		g_hCompletionPort = NULL;
	}


//	MessageBox(NULL,"Work Thread Deleted","Report",MB_OK);
}

void CleanSocket()
{
	// 소켓 api 클리어
	WSACleanup();

}

void PostDisconnectEvent(DWORD dwHandle)
{
	HEADER	dwHeader;
	dwHeader.SetHeader(dwHandle);
	dwHeader.SetActionType(ACTION_TYPE_DISCONNECT);

	PostQueuedCompletionStatus(g_hCompletionPort,0,dwHeader.m_dwHeader, NULL);
}

