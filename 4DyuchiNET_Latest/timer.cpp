//#define _WIN32_WINNT 0x0500

#include "timer.h"
#include "../4DyuchiNET_Common/net_define.h"
#include <process.h>

DWORD	g_dwTimerNum = 0;


#define EVENT_INDEX_PAUSE		0
#define EVENT_INDEX_DESTROY		1

#define EVENT_INDEX_NUM		2

struct TIMER_HANDLE
{
	DWORD			dwTick;
	HANDLE			hAwakeEvent[EVENT_INDEX_NUM];
	HANDLE			hCompleteAction;
	HANDLE			hStopper;
	HANDLE			hEvent;
	BOOL			bPuaseFlag;

	HANDLE			hTimerThread;
	HANDLE			hCompletetCreate;
	
};


TIMER_HANDLE	TimerHandle[MAX_TIMER_NUM];

CRITICAL_SECTION	g_csTimer;

UINT WINAPI TimerThread(LPVOID lpVoid)
{
	
	DWORD	dwTimerIndex = (DWORD)lpVoid;
	
	
	HANDLE*	hEventArray = TimerHandle[dwTimerIndex].hAwakeEvent;
	DWORD	dwTime = TimerHandle[dwTimerIndex].dwTick;
	HANDLE	hStopper = TimerHandle[dwTimerIndex].hStopper;
	HANDLE	hEvent = TimerHandle[dwTimerIndex].hEvent;

	SetEvent(TimerHandle[dwTimerIndex].hCompletetCreate);
	DWORD	dwEventIndex;
	
	while (TRUE)
	{
lb_loop:
		dwEventIndex = WaitForMultipleObjects(EVENT_INDEX_NUM,hEventArray,FALSE,dwTime);

		if (dwEventIndex == EVENT_INDEX_DESTROY)
		{
			// 이후 해당 이벤트 펑션이 펑션이 다시 실행되지 않도록 이벤트를 리셋
			ResetEvent(hEvent);
			goto seg_finish;								
		}

		if (dwEventIndex == EVENT_INDEX_PAUSE)
		{
			// 이후 해당 이벤트 펑션이 펑션이 다시 실행되지 않도록 이벤트를 리셋
			ResetEvent(hEvent);
			// 포즈가 되었으면 
			SetEvent(TimerHandle[dwTimerIndex].hCompleteAction);
			
			// 포즈되어있는 경우 대기한다.
			WaitForSingleObject(hStopper,INFINITE);
			// 포즈가 풀리면 다시 웨이트상태로..

			//포즈가 풀렸음을 알린다...
			SetEvent(TimerHandle[dwTimerIndex].hCompleteAction);
			goto lb_loop;
		}
		
		if (!SetEvent(hEvent))
			__debugbreak();
	}

seg_finish:													// Modified by chan78 at 2002/04/08
	_endthreadex(0);
	return 0;
}

void InitializeTimer()
{
	memset(TimerHandle,0,sizeof(TIMER_HANDLE)*MAX_TIMER_NUM);
	InitializeCriticalSection(&g_csTimer);
	g_dwTimerNum = 0;
}

VOID WINAPI OnTimer(PVOID pvContext, BOOLEAN fTimeout) 
{
	HANDLE	hEvent = (HANDLE)pvContext;
	SetEvent(hEvent);
}

// Modified by chan78 at 2002/07/22
DWORD AddTimer(DWORD dwPerioedTime,HANDLE hEvent,DWORD dwTimerIndex)
{

	// dwTimerIndex == user define event index
	HANDLE	hThread = NULL;
	
	UINT ThreadID = 0;
	if (dwTimerIndex >= MAX_TIMER_NUM)
		goto lb_return;
		
	EnterCriticalSection(&g_csTimer);

	

	TimerHandle[dwTimerIndex].dwTick = dwPerioedTime;
	
	DWORD	i;
	for (i=0; i<EVENT_INDEX_NUM; i++)
	{
		TimerHandle[dwTimerIndex].hAwakeEvent[i] = CreateEvent(NULL,FALSE,FALSE,NULL);
	}
	TimerHandle[dwTimerIndex].bPuaseFlag = FALSE;
	TimerHandle[dwTimerIndex].hStopper = CreateEvent(NULL,FALSE,FALSE,NULL);
	TimerHandle[dwTimerIndex].hEvent = hEvent;
	TimerHandle[dwTimerIndex].hCompleteAction = CreateEvent(NULL,FALSE,FALSE,NULL);
	TimerHandle[dwTimerIndex].hCompletetCreate = CreateEvent(NULL,FALSE,FALSE,NULL);
	TimerHandle[dwTimerIndex].hTimerThread = (HANDLE)_beginthreadex(NULL,NULL,TimerThread,(LPVOID)dwTimerIndex,NULL,&ThreadID);

	if (0xffffffff == (DWORD)TimerHandle[dwTimerIndex].hTimerThread)
	{
		MessageBox(NULL,"Timer Index already used","Error",MB_OK);
		goto lb_exit;
	}
	
	SetThreadPriority(hThread,THREAD_PRIORITY_TIME_CRITICAL);

	g_dwTimerNum++;
	WaitForSingleObject(TimerHandle[dwTimerIndex].hCompletetCreate,INFINITE);

lb_exit:
	LeaveCriticalSection(&g_csTimer);

lb_return:
	return dwTimerIndex;
}
// Modified by chan78 at 2002/09/17
BOOL PauseTimer(DWORD dwTimerIndex)
{

	BOOL	bResult = FALSE;
	if (dwTimerIndex >= MAX_TIMER_NUM)
		goto lb_return;

	EnterCriticalSection(&g_csTimer);
	if (TimerHandle[dwTimerIndex].bPuaseFlag)
		goto lb_false_and_unlock;

	// 타이머로 세팅된 이벤트가 아니다..
	if (!TimerHandle[dwTimerIndex].hEvent)
		goto lb_false_and_unlock;

	SetEvent(TimerHandle[dwTimerIndex].hAwakeEvent[EVENT_INDEX_PAUSE]);
	WaitForSingleObject(TimerHandle[dwTimerIndex].hCompleteAction,INFINITE);
	TimerHandle[dwTimerIndex].bPuaseFlag = TRUE;
	bResult = TRUE;

lb_false_and_unlock:
	LeaveCriticalSection(&g_csTimer);

lb_return:
	return bResult;

}

BOOL ResumeTimer(DWORD dwTimerIndex)
{
	BOOL	bResult = FALSE;
	if (dwTimerIndex >= MAX_TIMER_NUM)
		goto lb_return;

	EnterCriticalSection(&g_csTimer);
	if (!TimerHandle[dwTimerIndex].bPuaseFlag)
		goto lb_false_and_unlock;

	SetEvent(TimerHandle[dwTimerIndex].hStopper);
	WaitForSingleObject(TimerHandle[dwTimerIndex].hCompleteAction,INFINITE);
	TimerHandle[dwTimerIndex].bPuaseFlag = FALSE;
	bResult = TRUE;

lb_false_and_unlock:
	LeaveCriticalSection(&g_csTimer);

lb_return:
	return bResult;
	
}
// Modified by chan78 at 2002/07/22
BOOL RemoveTimer(DWORD dwTimerIndex)
{
	BOOL	bResult = FALSE;

	DWORD	i;

	EnterCriticalSection(&g_csTimer);

	if (!TimerHandle[dwTimerIndex].hTimerThread)
		goto lb_return;

	SetEvent(TimerHandle[dwTimerIndex].hAwakeEvent[EVENT_INDEX_DESTROY]);
	
	// pause되어있을지 모르는 타이머스레드들을 풀어준다.
	SetEvent(TimerHandle[dwTimerIndex].hStopper);
	WaitForSingleObject(TimerHandle[dwTimerIndex].hTimerThread,INFINITE);

	for (i=0; i<EVENT_INDEX_NUM; i++)
	{
		CloseHandle(TimerHandle[dwTimerIndex].hAwakeEvent[i]);
		TimerHandle[dwTimerIndex].hAwakeEvent[i] = NULL;
		CloseHandle(TimerHandle[dwTimerIndex].hCompleteAction);
		TimerHandle[dwTimerIndex].hCompleteAction = NULL;
	}
	CloseHandle(TimerHandle[dwTimerIndex].hStopper);
	CloseHandle(TimerHandle[dwTimerIndex].hCompletetCreate);
	
	CloseHandle(TimerHandle[dwTimerIndex].hTimerThread);
	TimerHandle[dwTimerIndex].hTimerThread = NULL;
	bResult = TRUE;
	g_dwTimerNum--;

	ResetEvent(TimerHandle[dwTimerIndex].hEvent);

lb_return:
	LeaveCriticalSection(&g_csTimer);
	return bResult;
}


void RemoveAllTimer()
{
	for (DWORD i=0; i<MAX_TIMER_NUM; i++)
	{
		RemoveTimer(i);

	}
}
