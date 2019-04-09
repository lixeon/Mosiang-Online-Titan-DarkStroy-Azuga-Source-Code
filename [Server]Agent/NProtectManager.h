#pragma once

#ifdef _NPROTECT_

#include "ggsrv25.h"

#define NPROTECTMGR	USINGTON(CNProtectManager)

class CNProtectManager
{
	WORD m_MapNum;

//	static DWORD m_dwHU;

public:
	CNProtectManager(void);
	~CNProtectManager(void);

	BOOL Init(WORD mapnum);
	void Release();

	static void NetworkMsgParse(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
	void SendAuthQuery(USERINFO* pInfo);

	void CNProtectManager::NpLog(int mode, char* msg);
	void GGAuthUpdateCallback(PGG_UPREPORT report);
	void Update();
	void Block(USERINFO* pInfo, DWORD BlockType);
};

EXTERNGLOBALTON(CNProtectManager);

#endif
