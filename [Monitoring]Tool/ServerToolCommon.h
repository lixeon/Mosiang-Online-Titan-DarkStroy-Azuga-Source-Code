#ifndef __SERVERTOOLCOMMON_H__
#define __SERVERTOOLCOMMON_H__

#pragma pack(push,1)

#define		MAX_SERVERSET		30
#define		MAX_DIST_SERVER		1
#define		MAX_AGENT_SERVER	5
#define		MAX_MURIM_SERVER	10
#define		MAX_MAP_SERVER		100

enum eSERVERSTATE
{
	eServerState_None = 0,
	eServerState_Off,
	eServerState_On,
	eServerState_AbNormal,
	eServerState_Disconnect,
	eServerState_Connect,
};

struct sMSGOPTION
{
	BYTE	bServer;
	BYTE	bExeVer;
	BYTE	bMaxUser;
	BYTE	bUserLevel;

	sMSGOPTION()
	{
		bServer = bExeVer = bMaxUser = bUserLevel = 1;
	}
};

#pragma pack(pop)

#endif
