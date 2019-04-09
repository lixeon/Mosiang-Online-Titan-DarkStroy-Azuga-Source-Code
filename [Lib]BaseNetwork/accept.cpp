#include "accept.h"
#include "main_msg_que.h"
#include "network.h"
#include "worker.h"
#include <process.h>

unsigned int WINAPI AcceptThread(LPVOID lpVoid);

HANDLE		g_hAcceptThread = NULL;
BOOL		g_bEnableAccept = FALSE;

BOOL StartAcceptThread(SOCKET sckListen,HWND hWnd,UINT uiMsg)
{
	DWORD			dwThreadID;

	g_hAcceptThread = (HANDLE)_beginthreadex( NULL, NULL, AcceptThread, NULL, NULL, (unsigned int *)&dwThreadID );

	g_bEnableAccept = TRUE;
	return TRUE;
}

void EndAcceptThread()
{
	if (!g_bEnableAccept)
		return;

	OutputDebugString( "Waiting AcceptThread...\n" );
	WaitForSingleObject( g_hAcceptThread, INFINITE );
	OutputDebugString( "AcceptThread Ok\n" );

	CloseHandle( g_hAcceptThread );
	g_hAcceptThread = NULL;
}

unsigned int WINAPI AcceptThread(LPVOID lpVoid)
{
	SOCKET				accept;
	SOCKADDR_IN			addr;
	int					len = sizeof(addr);

	while (TRUE)
	{
		// accept¸¦ °É¾î³õ´Â´Ù.¿©±â¼­ accept¿äÃ»ÀÌ µé¾î¿Ã¶§±îÁö ÀÌ ½º·¹µå´Â ºí·°µÈ´Ù.
		accept = WSAAccept(g_pNetwork->GetListenSocket(),
			(sockaddr*)&addr,
			&len,
			NULL,
			0);

		
		if (accept == INVALID_SOCKET)
		{
			goto lb_return;
			
		}
		IEPostConnectEvent(accept);
	}
lb_return:
	_endthreadex( NULL );
	return 0;
}
