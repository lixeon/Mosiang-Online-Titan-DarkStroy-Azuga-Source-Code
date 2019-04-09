#pragma once

#include <initguid.h>
#include "db.h"
/*************************************************************************************/
// ÀÌ È­ÀÏÀº Àý´ë ¼öÁ¤µÇ¾î¼­ ¾È µË´Ï´Ù.
/*************************************************************************************/

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
