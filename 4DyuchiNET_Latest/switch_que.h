#pragma once

#include "../4DyuchiNET_Common/stdafx.h"
#include "../4DyuchiNET_Common/net_define.h"


extern HANDLE	g_hEventMustSwitch[2];
extern HANDLE	g_hEventWritable;
extern HANDLE	g_hEventSwitchable;
extern HANDLE	g_hProcessComplete;

extern CRITICAL_SECTION	g_csSwitchQue[MAX_WORKER_THREAD_NUM];

BOOL StartSwitchQueThread(HANDLE hMsgHandle,DWORD dwWorkerThreadNum);
void EndSwitchQueThread();

#define SwitchQue() SetEvent(g_hEventMustSwitch[0])


