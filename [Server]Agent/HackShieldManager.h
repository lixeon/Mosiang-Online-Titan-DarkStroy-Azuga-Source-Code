#pragma once

#ifdef _HACK_SHIELD_
#ifndef _HS_LIB_INCLUDE_
#define _HS_LIB_INCLUDE_
#pragma comment(lib, "AntiCpSvr.lib") 
#endif
#endif

#include "AntiCpSvrFunc.h"
#include "UserTable.h"

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

	static void NetworkMsgParse(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
	
	void SendGUIDReq(USERINFO* pUserInfo);
	void SendReq(USERINFO* pUserInfo);
};

EXTERNGLOBALTON(CHackShieldManager);
