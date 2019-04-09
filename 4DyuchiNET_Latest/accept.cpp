#include "accept.h"
#include "network.h"
#include <process.h>
#include "cpio.h"
#include "pre_connect.h"
#include "mainthread.h"

UINT WINAPI AcceptThread(LPVOID lpVoid);

//
// accept스레드를 시작하는 함수.
//
// Modified by chan78 at 2002/07/22
BOOL StartAcceptThread( CNetwork* pNetwork )
{
	UINT ThreadID = 0;

	HANDLE hAcceptHandle = (HANDLE)_beginthreadex(NULL,NULL,AcceptThread,(LPVOID)pNetwork,NULL,&ThreadID);

	if (0xffffffff == (DWORD)hAcceptHandle)
	{
		MessageBox(GetActiveWindow(),"Fail to Create Accept Thread","Error",MB_OK);
		pNetwork->SetAcceptTHreadHandle( NULL );
		return FALSE;
	}
	SetThreadPriority( hAcceptHandle,THREAD_PRIORITY_HIGHEST );
	pNetwork->SetAcceptTHreadHandle( hAcceptHandle );
	
	return TRUE;
}

// Modified by chan78 at 2002/07/22
BOOL EndAcceptThread( SOCKET socketListen, HANDLE hAcceptThread )
{
	if (socketListen == INVALID_SOCKET)
		return FALSE;
	
	closesocket( socketListen );
	WaitForSingleObject( hAcceptThread, INFINITE );
	CloseHandle( hAcceptThread );

	return TRUE;
}

UINT WINAPI AcceptThread(LPVOID lpVoid)
{
	// 접속을 받는 스레드이다.
	// 접속을 받으면 인자로 받은 네트워크 클래스의 OnConnect()함수를 자동 실행한다.
	SOCKET accept = INVALID_SOCKET;

	CNetwork* pNetwork = (CNetwork*)lpVoid;	// 인자로 네트웍 클래스의 포인터를 받는다.
	DWORD		dwAcceptInterval = pNetwork->GetAcceptInterval();
	
	CConnection* con = NULL;
	DWORD		index = 0;
	sockaddr_in	addr;
	int			len = sizeof(addr);
	memset(&addr,0,len);
	CONNECT_ITEM item;
	
	if (dwAcceptInterval > 1000)
		dwAcceptInterval = 1000;
	while (TRUE)
	{
		// accept를 걸어놓는다.여기서 accept요청이 들어올때까지 이 스레드는 블럭된다.
		accept = WSAAccept(pNetwork->GetListenSocket(),
			(sockaddr*)&addr,
			&len,
			NULL,
			0);

		Sleep(dwAcceptInterval);
		
		if (accept == INVALID_SOCKET)
		{
			goto seg_destroy;
		}

		
		// 접속을 받고 주어진 네트웍 클래스에 등록한다.
		// 메인스레드에 처리할 접속요구가 있음을 통보.
		item.socket = accept;
		item.pNetwork = pNetwork;
		item.SuccessConnect = NULL;
		item.FailConnect = NULL;
		g_pPreConnectPool->InsertPreConnect(&item);
		ProcessPreConnectedItem();
	}
seg_destroy:
	_endthreadex(0);

	return 0;
}
