#pragma once

#include <initguid.h>
#include "db.h"
/*************************************************************************************/
// ÀÌ È­ÀÏÀº Àý´ë ¼öÁ¤µÇ¾î¼­ ¾È µË´Ï´Ù.
/*************************************************************************************/

// {CC879276-E1A4-4c65-80D3-2E69B3ADB14D}
DEFINE_GUID(CLSID_DBTHREAD_DLL, 
0xcc879276, 0xe1a4, 0x4c65, 0x80, 0xd3, 0x2e, 0x69, 0xb3, 0xad, 0xb1, 0x4d);

// {37F7FB03-3A10-444b-A770-172FCE6880DB}
DEFINE_GUID(IID_DBTHREAD_DLL, 
0x37f7fb03, 0x3a10, 0x444b, 0xa7, 0x70, 0x17, 0x2f, 0xce, 0x68, 0x80, 0xdb);

interface IDBThread : public IUnknown
{
	virtual BOOL		__stdcall		CreateDBThread(DESC_DBTHREAD* pDesc) = 0;		
	virtual BOOL		__stdcall		InitDBConnection() = 0;
	virtual BOOL		__stdcall		InitDBConnection(LPSTR szID, LPSTR szPassword, LPSTR Dbname, LPSTR szIDC, LPSTR szPasswordC, LPSTR DbnameC) = 0;
	virtual void		__stdcall		SendMessageDB(LPDBMESSAGE pPara) = 0;
	virtual BOOL		__stdcall		CloseThisConnection(LPDBCON pCon) = 0;
	virtual LPDBCON		__stdcall		MakeNewConnection(LPSTR lpDbName, LPSTR lpID, LPSTR lpPW) = 0;
	virtual void		__stdcall		SetDBMessageEvent(HANDLE val) = 0;
	virtual HANDLE		__stdcall		GetDBMessageEvent() = 0;
	virtual void		__stdcall		CloseDBConnection() = 0;
	virtual LPDBCON		__stdcall 		GetAdMinCon() = 0;
	virtual LPDBCON		__stdcall 		GetClientCon() = 0;
	virtual	void		__stdcall		ProcessingDBMessage() = 0;
	virtual void		__stdcall		DeleteThisPointer(DWORD dwFnNo, LPVOID pTarget) = 0;
	virtual char*		__stdcall		GetDBMessageMemory() = 0;
	virtual char*		__stdcall		GetDBMessageStackMemory() = 0;
	virtual void		__stdcall		FreeThisMemory(LPVOID pMsg, LPVOID pData) = 0;
	virtual DWORD		__stdcall		GetUnusedStackMemoryNum() = 0;
	virtual DWORD		__stdcall		GetUnusedDBMemoryNum() = 0;
	
	

};
