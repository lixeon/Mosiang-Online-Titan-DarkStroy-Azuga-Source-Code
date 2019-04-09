#ifndef MSG_PROC_H
#define MSG_PROC_H


#include "../4DyuchiNET_Common/stdafx.h"


class CMsgQue;
class CNetwork;

BOOL	InitCompletionIO(DWORD size,HANDLE hMsgHandle);
void	CloseComplitionIO();
void	CleanSocket();
void	InitSocket();
void	ResumeWorkerThread();
DWORD	GetWorkerThreadNum();
void	PostDisconnectEvent(DWORD dwHandle);

extern HANDLE g_hCompletionPort;


#endif
