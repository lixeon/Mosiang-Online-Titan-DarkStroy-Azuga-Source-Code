#include "stdafx.h"
#include <process.h>
#include "switch_que.h"
#include "main_msg_que.h"
#include "define.h"
#include "network.h"

HWND	g_hMainThreadWnd = NULL;
// Modified by chan78 at 2002/09/26
//HANDLE	g_hCompleteEvent = NULL;
HANDLE	g_hSwitchEvent[3];
HANDLE	g_hSwitchQueThread = NULL;
DWORD	g_dwSpinFlag = 0xffffffff;

unsigned int WINAPI SwitchQueThread( void *pVoid );

__declspec(naked) void	SwitchQueEnter()
{
	__asm
	{	
		push		eax
		push		ebx
		push		edx
		push		esi
		push		edi
		

		xor			eax,eax
		xor			edx,edx
			
lb_enter:
		dec			eax
		lock		cmpxchg		dword ptr[g_dwSpinFlag],edx
		jnz			lb_enter

		pop			edi
		pop			esi
		pop			edx
		pop			ebx
		pop			eax

		ret
	}
}
__declspec(naked) void SwitchQueLeave()
{
	__asm
	{
		lock		dec			dword ptr[g_dwSpinFlag]
		ret
	}
}


BOOL StartSwitchQueThread(HWND hWnd)
{
	g_hMainThreadWnd = hWnd;
	g_hSwitchEvent[0] = CreateEvent(NULL,FALSE,FALSE,NULL);
	g_hSwitchEvent[1] = CreateEvent(NULL,FALSE,FALSE,NULL);
	g_hSwitchEvent[2] = CreateEvent(NULL,FALSE,FALSE,NULL);

	DWORD	dwThreadID;

	g_hSwitchQueThread = (HANDLE)_beginthreadex( NULL, NULL, SwitchQueThread, NULL, NULL, (unsigned int *)&dwThreadID );

	return TRUE;
}

unsigned int WINAPI SwitchQueThread(LPVOID lpVoid)
{
	DWORD		dwIndex;
	while (1)
	{
		dwIndex = WaitForMultipleObjects( 2, g_hSwitchEvent, FALSE, INFINITE );
		if (dwIndex)
			goto lb_return;

		SwitchQueEnter();
		g_pMsgQue = g_pMsgQue->m_pNext;
		SwitchQueLeave();

//		printf("\n Dll 디버그 정보  :  PostMessage WM_SOCKET_EVENT // g_hMainThreadWnd = %d\n", (DWORD)g_hMainThreadWnd);

		if( PostMessage( g_hMainThreadWnd, g_pNetwork->GetWinMsgID(), 0, 0 ) )
		{
			OutputDebugString( "Waiting For Message Process\n" );

			// Modified by chan78 at 2002/9/26
			dwIndex = WaitForMultipleObjects( 2, g_hSwitchEvent+1, FALSE, INFINITE );
			if( !dwIndex )
				goto lb_return;
//			WaitForSingleObject(g_hCompleteEvent,INFINITE);
			OutputDebugString( "Message Process has done\n" );
		}
		else
		{
			OutputDebugString( "Failed To Post Message\n" );
		}
	}
lb_return:
	_endthreadex( NULL );
	return 0;
}

void SwitchQue()
{
	SetEvent(g_hSwitchEvent[0]);
}
void MainThreadComplete()
{
//	Modified by chan78 at 2002/07/19
//	SetEvent(g_hCompleteEvent);)
	SetEvent(g_hSwitchEvent[2]);
}
void EndSwitchQueThread()
{
	SetEvent(g_hSwitchEvent[1]);

//	Modified by chan78 at 2002/07/19
	OutputDebugString( "Waiting SwitchQueThread...\n" );
	WaitForSingleObject( g_hSwitchQueThread, INFINITE );
	OutputDebugString( "SwitchQueThread Ok\n" );

	CloseHandle( g_hSwitchQueThread );
	g_hSwitchQueThread = NULL;

	//CloseHandle(g_hCompleteEvent);
	CloseHandle(g_hSwitchEvent[0]);
	CloseHandle(g_hSwitchEvent[1]);
	CloseHandle(g_hSwitchEvent[2]);
}