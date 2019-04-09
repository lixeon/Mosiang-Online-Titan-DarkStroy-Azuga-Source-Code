#pragma once
#include "../4DyuchiNET_Common/stdafx.h"


#ifdef _NEED_CONFIRM

#pragma pack(push,4)

struct	CONFIRM_PACKET
{
	WORD		wID;
	WORD		wVersion;
	DWORD		dwCPUType;
	DWORD		dwMacAddressLO;
	DWORD		dwMacAddressHI;
	DWORD		GetPacketSize()		{return 20;}
};

#pragma pack(pop)

enum	PTCL
{
	PTCL_CONFIRM_DLL = 0x10

};

#endif
