#ifndef _NProtectManager_H_
#define _NProtectManager_H_

#pragma once

#ifdef _NPROTECT_

#define NPROTECTMGR	USINGTON(CNProtectManager)

class CNPGameLib;

class CNProtectManager
{
	char m_szHackMsg[256];

	DWORD m_dwLastCheckTime;

	BOOL m_bTestServer;

public:
	CNProtectManager(void);
	~CNProtectManager(void);

	char* GetMsg();

	void NetworkMsgParse(BYTE Protocol, void* pMsg);
	BOOL MsgProc(DWORD dwMsg, DWORD dwArg);

	BOOL Init(HWND hWnd);
	void Release();

	void Check();

	void TwistAccount( char* pAccount, char* pPassword, char* pVersion );
};

EXTERNGLOBALTON(CNProtectManager);
extern CNPGameLib* g_pNPGame;

#endif
#endif
