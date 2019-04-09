//#define _WIN32_WINNT 0x0500

#include "switch_que.h"
#include "quemanager.h"
#include <process.h>

// Added by chan78 at 2002/07/22
HANDLE	g_hSwitchQueueThread = NULL;
HANDLE	g_hProcessComplete;
HANDLE	g_hEventMustSwitch[2];
HANDLE	g_hSwitchHandle = NULL;
DWORD	g_dwSwitchQueCSNum = 0;
DWORD	g_dwSwitchEnable = 0;
CRITICAL_SECTION	g_csSwitchQue[MAX_WORKER_THREAD_NUM];
UINT WINAPI			SwitchQueThread(LPVOID lpVoid);

BOOL EnterSwitchQue();
void LeaveSwitchQue();

// Modified by chan78 at 2002/07/22
BOOL StartSwitchQueThread(HANDLE hMsgHandle,DWORD dwWorkerThreadNum)
{
	g_dwSwitchQueCSNum = dwWorkerThreadNum;
	g_hSwitchHandle = hMsgHandle;

	UINT	ThreadID;

	g_hEventMustSwitch[0]	=	CreateEvent(NULL,FALSE,FALSE,NULL);
	g_hEventMustSwitch[1]	=	CreateEvent(NULL,FALSE,FALSE,NULL);
	g_hProcessComplete		=	CreateEvent(NULL,FALSE,FALSE,NULL);
	

	g_hSwitchQueueThread = (HANDLE)_beginthreadex(NULL,NULL,SwitchQueThread,(LPVOID)hMsgHandle,NULL,&ThreadID);

	if (0xffffffff == (DWORD)g_hSwitchQueueThread)
	{
		MessageBox(NULL,"Fail to Create SwitchThread","Error",NULL);
		return FALSE;
	}

	SetThreadPriority(g_hSwitchQueueThread,THREAD_PRIORITY_HIGHEST);

	for (DWORD i=0; i<g_dwSwitchQueCSNum; i++)
	{
		InitializeCriticalSectionAndSpinCount(&g_csSwitchQue[i],1000);

	}
	return TRUE;

}

// Modified by chan78 at 2002/07/22
void EndSwitchQueThread()
{
	SetEvent(g_hEventMustSwitch[1]);

	WaitForSingleObject(g_hSwitchQueueThread,INFINITE);

	CloseHandle(g_hEventMustSwitch[0]);
	CloseHandle(g_hEventMustSwitch[1]);
	CloseHandle(g_hProcessComplete);
	CloseHandle(g_hSwitchQueueThread);
	g_hSwitchQueueThread = NULL;

	for (DWORD i=0; i<g_dwSwitchQueCSNum; i++)
		DeleteCriticalSection(&g_csSwitchQue[i]);
}

#define	SIZE_CRITICAL_SECTION 24

UINT WINAPI SwitchQueThread(LPVOID lpVoid)
{
		
	DWORD				dwMsgNum;
	HANDLE*				pEvent;
	CRITICAL_SECTION*	pcsSwitchQue;
	DWORD	count;

	
	__asm
	{
		mov			edx,offset dword ptr[g_hEventMustSwitch]
		mov			dword ptr[pEvent],edx

lb_loop:
		mov			edx,dword ptr[pEvent]
		push		INFINITE
		push		FALSE
		push		edx
		push		0x00000002
		call		dword ptr[WaitForMultipleObjects]
		
		or			eax,eax	
		jnz			lb_return

		; change que
		mov			eax,offset dword ptr[g_csSwitchQue]
		mov			dword ptr[pcsSwitchQue],eax

		mov			ecx,dword ptr[g_dwSwitchQueCSNum]
		mov			dword ptr[count],ecx
lb_enter_count:
		
		mov			edx,dword ptr[pcsSwitchQue]
		push		edx
		call		dword ptr[EnterCriticalSection]
		
		add			dword ptr[pcsSwitchQue],SIZE_CRITICAL_SECTION
		dec			dword ptr[count]
		jnz			lb_enter_count

		; Now safe!!!
		; switch que
		mov			ebx,dword ptr[g_pQueManager]			; load g_pQueManager
		mov			eax,dword ptr[ebx]						; writable que
		mov			edx,dword ptr[ebx+4]					; readable que
		mov			ecx,dword ptr[eax]						; writable que->msgnum
		mov			dword ptr[dwMsgNum],ecx					; write local dwMsgNum
		or			ecx,ecx
		jz			lb_leave

		mov			dword ptr[ebx],edx
		mov			dword ptr[ebx+4],eax
		;

lb_leave:
		sub			dword ptr[pcsSwitchQue],SIZE_CRITICAL_SECTION
		mov			ecx,dword ptr[g_dwSwitchQueCSNum]
		mov			dword ptr[count],ecx

lb_leave_count:		
		mov			edx,dword ptr[pcsSwitchQue]
		push		edx
		call		dword ptr[LeaveCriticalSection]
		
		sub			dword ptr[pcsSwitchQue],SIZE_CRITICAL_SECTION
		dec			dword ptr[count]
		jnz			lb_leave_count

		xor			eax,eax
		or			eax,dword ptr[dwMsgNum]			; 큐를 바꿨다면 0초과 아니면 0
		jz			lb_loop

		; set event for main thread
		mov			ecx,dword ptr[g_hSwitchHandle]
		push		ecx
		call		dword ptr[SetEvent]					

		; wait until that main thread complete processing
		mov			edx,dword ptr[g_hProcessComplete]
		push		INFINITE
		push		edx
		call		dword ptr[WaitForSingleObject]
		or			eax, eax
		jz			lb_loop
lb_return:
	}
	_endthreadex(0);
	return 0;
}
