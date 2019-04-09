#pragma once

//#include "stdafx.h"

typedef void (*ACCEPTFUNC)(DWORD);
typedef void (*VOIDFUNC)(void);
typedef void (*CONNECTSUCCESSFUNC)(DWORD,void*);
typedef void (*CONNECTFAILFUNC)(void*);


#define	NETDDSC_ENCRYPTION			0x00000001
#define NETDDSC_DEBUG_LOG			0x00000010





enum FLAG_SEND
{
	FLAG_SEND_ENCRYPTION		=	0x00000001,
	FLAG_SEND_NOT_ENCRYPTION	=	0x00000000
};

struct CUSTOM_EVENT
{   
    DWORD		dwPeriodicTime;
	VOIDFUNC	pEventFunc;
};

struct DESC_NETWORK
{
    DWORD		dwMaxUserNum;
    DWORD		dwMaxServerNum;
    void		(*OnRecvFromUserTCP)(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
    void		(*OnRecvFromServerTCP)(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
    void		(*OnAcceptUser)(DWORD dwConnectionIndex);
    void		(*OnAcceptServer)(DWORD dwConnectionIndex);
    void		(*OnDisconnectUser)(DWORD dwConnectionIndex);
    void		(*OnDisconnectServer)(DWORD dwConnectionIndex);
    DWORD		dwServerMaxTransferSize;
    DWORD		dwUserMaxTransferSize;
    DWORD		dwServerBufferSizePerConnection;
    DWORD		dwUserBufferSizePerConnection;
    DWORD		dwMainMsgQueMaxBufferSize;
	DWORD		dwConnectNumAtSameTime;
    DWORD		dwFlag;
    DWORD		dwCustomDefineEventNum;
	
    CUSTOM_EVENT*	pEvent;
};
   
