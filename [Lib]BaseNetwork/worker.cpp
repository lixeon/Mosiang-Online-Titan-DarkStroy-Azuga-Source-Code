#include "worker.h"
#include "define.h"
#include "overlapped_recv.h"
#include "overlapped_send.h"
#include "main_msg_que.h"
#include "switch_que.h"
#include <process.h>

enum IE_COMMAND_TYPE
{
	IE_COMMAND_BREAK = 0x00000001,
	IE_COMMAND_COMPULSIVE_DISCONNECT = 0x00000003,
	IE_COMMAND_ACCEPT = 0x00000002,
	IE_COMMAND_DESTROY = 0x00000005

};

struct		IE_COMMAND
{
	DWORD		m_dwIndex;
	DWORD		m_dwType;
};

IE_COMMAND	g_IECommandAcceptThread;
IE_COMMAND	g_IECommandMainThread;

OVL_INFO	g_ovlInfo[32];
HANDLE		g_hWorkerEvent[64];
HANDLE		g_hAcceptThreadEventComplete = NULL;
HANDLE		g_hMainThreadEventComplete = NULL;
HANDLE		g_hWorkerThread;

unsigned int WINAPI WorkerThread(LPVOID lpVoid);

BOOL StartWorkerThread()
{
	DWORD		dwThreadID;

	memset(g_hWorkerEvent,0,sizeof(HANDLE)*64);
	memset(g_ovlInfo,0,sizeof(OVL_INFO)*32);

	g_pMsgQue = new CMsgQue;
	g_pMsgQue->Initialize(MAX_MAIN_MSG_QUE_SIZE);

	for (DWORD i=0; i<64; i++)
		g_hWorkerEvent[i] = CreateEvent(NULL,FALSE,FALSE,NULL);
	
	g_hMainThreadEventComplete = CreateEvent(NULL,FALSE,FALSE,NULL);
	g_hAcceptThreadEventComplete = CreateEvent(NULL,FALSE,FALSE,NULL);

	g_hWorkerThread = (HANDLE)_beginthreadex( NULL, NULL, WorkerThread, NULL, NULL, (unsigned int *)&dwThreadID );

	return TRUE;
}

BOOL IESetOvlInfo(OVL_INFO* pInfo)
{
	BOOL	bResult = FALSE;
	if (pInfo->m_dwIndex > 30)
		goto lb_return;

	memcpy(g_ovlInfo+pInfo->m_dwIndex,pInfo,sizeof(OVL_INFO));
	bResult = TRUE;
lb_return:
	return bResult;
}

HANDLE IEGetEvnetHandle(DWORD dwIndex,DWORD dwType)
{
	HANDLE	hEvent = NULL;
	if (dwIndex > 30)
		goto lb_return;
	
	hEvent = g_hWorkerEvent[dwIndex+dwType];

lb_return:
	return hEvent;
}

void EndWorkerThread()
{
	g_IECommandMainThread.m_dwType = IE_COMMAND_DESTROY;
	SetEvent(g_hWorkerEvent[0]);

	//	Modified by chan78 at 2002/07/19
	OutputDebugString( "Waiting WorkerThread...\n" );
	WaitForSingleObject( g_hWorkerThread, INFINITE );
	OutputDebugString( "WorkerThread Ok\n" );

	CloseHandle( g_hWorkerThread );
	g_hWorkerThread = NULL;

	CloseHandle(g_hAcceptThreadEventComplete);
	CloseHandle(g_hMainThreadEventComplete);


	for (DWORD i=0; i<64; i++)
		CloseHandle(g_hWorkerEvent[i]);

}

void ReleaseMsgQue()
{
	if (g_pMsgQue)
	{
		delete g_pMsgQue;
		g_pMsgQue = NULL;
	}
}
unsigned int WINAPI WorkerThread( void *pVoid )
{
	WSABUF			wsabuf[2000];
	OVERLAPPED*		pOvl;
	SOCKET			socket;
	DWORD			dwIndex;
	BOOL			bResult;
	DWORD			dwTransferredBytes;
	DWORD			dwCount;
	OVL_INFO		ovlInfo;
	DWORD			dwSize;
	DWORD			dwFlag;
	int				iErr;
	while(1)
	{

		dwIndex = WaitForMultipleObjects(64,g_hWorkerEvent,FALSE,INFINITE);

//		printf("\n Dll 디버그 정보  :  WaitForMultipleObjects // dwIndex = %d\n", dwIndex);

		if (!dwIndex)
		{
			if (g_IECommandMainThread.m_dwType == IE_COMMAND_DESTROY)
				goto lb_return;
			
			if (g_IECommandMainThread.m_dwType == IE_COMMAND_COMPULSIVE_DISCONNECT)
			{
				dwIndex = g_IECommandMainThread.m_dwIndex;
				SetEvent(g_hMainThreadEventComplete);
				goto lb_disconnect;
			}
			// 워커 스레드로 보내는 이벤트 
			// 예를 들면 강제접속종료.
			// 스레드 중지
			continue;
		}
		if (dwIndex == 63)
		{
			if (g_IECommandAcceptThread.m_dwType == IE_COMMAND_ACCEPT)
			{
				dwIndex = g_IECommandAcceptThread.m_dwIndex;
				
				SetEvent(g_hAcceptThreadEventComplete);
				
				SwitchQueEnter();
				g_pMsgQue->PushMsg(dwIndex,MSG_TYPE_ACCEPT,NULL,0,0);
				SwitchQueLeave();
				
				OutputDebugString( "Switch() by IE_COMMAND_ACCEPT\n" );
				goto lb_set_event;
			}
		}
		if (dwIndex >= 30)
		{
			pOvl = g_ovlInfo[dwIndex-30].m_pOvlRead;
			dwIndex -= 30;
		}
		else
			pOvl = g_ovlInfo[dwIndex].m_pOvlWrite;
	
		if (!pOvl)
			continue;

		socket = g_ovlInfo[dwIndex].m_socket;
		
		bResult = WSAGetOverlappedResult( (SOCKET) socket ,pOvl,&dwTransferredBytes,TRUE, &dwFlag);
//		printf("\n Dll 디버그 정보 1 :  WSAGetOverlappedResult : dwTransferredBytes = %d\n", dwTransferredBytes);

		if (!bResult)
		{
			iErr = GetLastError();

//			플래그가 flase 이면 설정한다.
//			if (WSA_IO_INCOMPLETE == iErr)
//			{
//				continue;
//			}


//			if (0x00000040 == iErr)
//			printf("GetLastError :  %d\n", iErr);
			goto lb_disconnect;
//			continue;
		}


		if (!dwTransferredBytes)
		{
lb_disconnect:
			ovlInfo.m_pOvlWrite = NULL;
			ovlInfo.m_pOvlRead = NULL;
			ovlInfo.m_socket = INVALID_SOCKET;
			ovlInfo.m_dwFlag = NULL;
			ovlInfo.m_dwIndex = dwIndex;
			
			IESetOvlInfo(&ovlInfo);
			
			
			SwitchQueEnter();
			g_pMsgQue->PushMsg(dwIndex,MSG_TYPE_DISCONNECT,NULL,0,0);
			SwitchQueLeave();

			OutputDebugString( "Switch() by DISCONNECT\n" );
			goto lb_set_event;
		}

		if (((COverlapped*)pOvl)->GetIOType() == IO_TYPE_READ)
		{
			
			dwCount = ((COverlappedRecv*)pOvl)->OnRead(wsabuf,2000,dwTransferredBytes,&dwSize);
			
			if (dwCount)
			{
//				printf("\n Dll 디버그 정보 2 :  Received Packet Nums = %d\n", dwCount);
//				_asm int 3

				SwitchQueEnter();			

				g_pMsgQue->PushMsg(dwIndex,MSG_TYPE_TCP,wsabuf,dwCount,dwSize);


//				printf("\n Dll 디버그 정보 8 : PushMsg : Wsabuf.Len = %d\n", wsabuf[dwCount].len);

				SwitchQueLeave();
			}

			((COverlappedRecv*)pOvl)->PrepareRead();

//			OutputDebugString( "Switch() by READ\n" );
			goto lb_set_event;
		}
		if (((COverlapped*)pOvl)->GetIOType() == IO_TYPE_WRITE)
		{
			((COverlappedSend*)pOvl)->Lock();
			((COverlappedSend*)pOvl)->OnSend(dwTransferredBytes);
			((COverlappedSend*)pOvl)->Unlock();
			continue;
		}
//		OutputDebugString( "Switch() by Loop end\n" );
lb_set_event:

		SwitchQue();
	}
lb_return:
	_endthreadex( NULL );
	return 0;
}

void IEPostDisconnectEvent(DWORD dwIndex)
{
	g_IECommandMainThread.m_dwIndex = dwIndex;
	g_IECommandMainThread.m_dwType = IE_COMMAND_COMPULSIVE_DISCONNECT;
	SetEvent(g_hWorkerEvent[0]);
	WaitForSingleObject(g_hMainThreadEventComplete,INFINITE);
}

void IEPostConnectEvent(SOCKET s)
{
	g_IECommandAcceptThread.m_dwIndex = s;
	g_IECommandAcceptThread.m_dwType = IE_COMMAND_ACCEPT;
	SetEvent(g_hWorkerEvent[63]);
	WaitForSingleObject(g_hAcceptThreadEventComplete,INFINITE);
}
