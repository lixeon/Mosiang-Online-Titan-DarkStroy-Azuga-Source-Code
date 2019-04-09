#pragma once

#ifdef _HACK_SHIELD_
#ifndef _HS_LIB_INCLUDE_
#define _HS_LIB_INCLUDE_
#pragma comment(lib, "HShield.lib") 
#endif
#endif


#include "HShield.h"

BOOL HS_Init();
BOOL HS_StartService();
BOOL HS_StopService();
BOOL HS_UnInit();
int __stdcall HS_CallbackProc (long lCode, long lParamSize, void* pParam );
BOOL HS_PauseService();
BOOL HS_ResumeService();
void HS_SaveFuncAddress();


struct MSG_GUID_REQ : public MSGROOT
{
	BYTE msg[SIZEOF_GUIDREQMSG];
};

struct MSG_GUID_ACK : public MSGROOT
{
	BYTE msg[SIZEOF_GUIDACKMSG];
};

struct MSG_REQ : public MSGROOT
{
	BYTE msg[SIZEOF_REQMSG];
};

struct MSG_ACK : public MSGROOT
{
	BYTE msg[SIZEOF_ACKMSG];
};

#define HACKSHIELDMGR	USINGTON(CHackShieldManager)

class CHackShieldManager
{
public:
	CHackShieldManager(void);
	virtual ~CHackShieldManager(void);

	void NetworkMsgParse(BYTE Protocol, void* pMsg);
};


EXTERNGLOBALTON(CHackShieldManager);
//#endif
