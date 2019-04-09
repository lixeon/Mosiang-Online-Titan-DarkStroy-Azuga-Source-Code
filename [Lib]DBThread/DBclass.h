#pragma once

#include "stdafx.h"
#include "voidlist.h"
#include <sql.h>
#include <sqlext.h>
#include "db.h"
#include "Dbthreadinterface.h"
#include "dbthread_guid.h"
#include "memorypool.h"


class CDBProcess : public CVoidList
{
public:
    virtual void FreeData(void* pData) // °´Ã¼ µ¥ÀÌÅ¸¸¦ »èÁ¦ÇÑ´Ù.
    {
        if (pData == NULL) return;
        LPDBMESSAGE	pList;
		pList =	(LPDBMESSAGE) pData;
        delete pList;
    }
};
class CDBMessage : public CVoidList
{
public:
    virtual void FreeData(void* pData) // °´Ã¼ µ¥ÀÌÅ¸¸¦ »èÁ¦ÇÑ´Ù.
    {
        if (pData == NULL) return;
        LPDBRESULTMESSAGE	pList;
		pList =	(LPDBRESULTMESSAGE) pData;
        delete pList;
    }
};

class CDB
{
public:
	char*	 		InitDBConnection();
	char*	 		InitDBConnection(LPSTR szID, LPSTR szPassword, LPSTR Dbname, LPSTR szIDC, LPSTR szPasswordC, LPSTR DbnameC);
	SQLRETURN	 	GetSQLInfo(LPDBCON pCon, SQLUSMALLINT InfoType, SQLUINTEGER fFuncs);
	BOOL	 		ExcuteIUD(LPDBCON pCon, LPSTR szQuery);
	BOOL	 		InsertRecord(LPDBCON pCon, LPSTR szTableName, LPVOID lpCol, LPVOID lpValue, BYTE bNumData);
	BOOL	 		DeleteRecord(LPDBCON pCon, LPSTR szTableName, LPSTR lpCol, LPSTR lpValue);
	BOOL			ExcuteIUDForProcedure(LPDBCON pCon, LPSTR szQuery);
	BOOL	 		UpdateRecord(LPDBCON pCon, LPSTR szTableName, LPSTR lpCol, LPSTR lpValue, LPSTR DestCol, LPSTR	DestValue);
	BOOL	 		Login(LPDBCON pCon, LPSTR szTableName, LPSTR pColID, LPSTR pColPW, LPSTR pID, LPSTR pPW, LPSTR pRetCol, LPVOID pRetValue, int iLength,SQLSMALLINT sqlType);
	BOOL	 		SimpleLogin(LPDBCON pCon,LPSTR pID, LPSTR pPW, LPVOID pRetValue);
	BOOL	 		BindQuery(LPDBCON pCon,LPSTR szBuffer, LPBIND lpBind, int Rowset, int StSize, SQLSMALLINT sqlType);
	void		 	SendMessageDB(LPDBMESSAGE pPara);
	LPQUERY  FAR PASCAL 	FreeQuery(LPDBCON pCon,LPSTR szBuffer,LPDWORD pNumRow);
	LPLARGEQUERY FAR PASCAL FreeLargeQuery(LPDBCON pCon,LPSTR szBuffer,LPDWORD pNumRow);
	LPMIDDLEQUERY FAR PASCAL FreeMiddleQuery(LPDBCON pCon,LPSTR szBuffer, LPDWORD pNumRow);
	LPQUERY	 FAR PASCAL 	SingleQuery(LPDBCON pCon, LPSTR szTable, DWORD dwValue, LPSTR lpCol, SQLSMALLINT sqltype);
	LPQUERYDA FAR PASCAL 	SingleQueryDA(LPDBCON pCon, LPSTR szTable, DWORD dwValue, LPSTR lpCol, SQLSMALLINT sqltype);
	
	BOOL	 		DeleteRecordInCursor(LPDBCON pCon);
	BOOL	 		GenerateResultSet(LPDBCON pCon, LPSTR szBuffer, LPBIND lpBind, int StSize, SQLSMALLINT sqlType, int rowset);
	BOOL	 		GetNextRecordSet(LPDBCON pCon);
	BOOL	 		GetLastRecordSet(LPDBCON pCon);
	BOOL	 		GetFirstRecordSet(LPDBCON pCon);
	BOOL	 		GetPriorRecordSet(LPDBCON pCon);
	BOOL	 		CloseRecordSet(LPDBCON pCon);
	BOOL		 	CloseThisConnection(LPDBCON pCon);
	LPDBCON		 	MakeNewConnection(LPSTR lpDbName, LPSTR lpID, LPSTR lpPW);
	void		 	SetDBMessageEvent(HANDLE val){m_hResultMessageEvent = val;}
	HANDLE		 	GetDBMessageEvent(){return m_hResultMessageEvent;}
	void	 		CloseDBConnection();
	void			SetIsUseEvnetDB(BOOL val){m_UsingEventObject = val;}
	HWND			GetHwnd(){ return m_hWndToPostMessage;}
	void			SetHwnd(HWND hwnd){ m_hWndToPostMessage = hwnd;}
	void			SetUsingThread(BOOL val){m_bUsingThread = val;}
	BOOL			CloseAllCustomConnection();
	void			SetConnectionNum(BYTE val){m_NumConnection = val;}
	BOOL	 		SQLOK(SQLRETURN retCode) {if(retCode == SQL_SUCCESS || retCode == SQL_SUCCESS_WITH_INFO) return 1; else return 0;}
	void	 		DisplaySQLError(LPDBCON pCon);
	void			DisplaySQLError(LPDBCON pCon, HSTMT hStmt);
	BOOL	 		InitDBThread();
	BOOL	 		CloseDBThread();
	LPVOID	 		ExcuteThreadFN(LPDBMESSAGE pBuffer);
	BOOL			ExcuteThreadFNbool(LPDBMESSAGE pBuffer);

	void				SetOnResultFunc(void (*pFunc)(DWORD ret,  LPDBMESSAGE pMessage)) {m_pReadDBResult = pFunc;}
	void				SetOnWriteMessageFunc(void (*pFunc)(char* msg)) { m_pWriteMessage = pFunc;} 
	LPDBCON		 		GetAdMinCon(){return m_pDBAdmin;}
	LPDBCON		 		GetClientCon(){return m_pDBClient;}
	CRITICAL_SECTION*	GetDBCS(){ return &m_DBCS;}				// DB¿ë ¾²·¹µåÀÇ ÀÓ°è¿µ¿ª
	HANDLE		 		GetDBThread(){return m_hDBThread;}
	HANDLE		 		GetDBEvent(){return m_hDBEvent;}
	HANDLE		 		GetDBKillEvent(){return m_hKillDBEvent;}
	CDBProcess*	 		GetDBProcessAdd(){return m_pDBProcess1;}
	CDBProcess*	 		GetDBProcessPop(){return m_pDBProcess2;}
	void		 		DispatchDBMessage(DWORD pRet,LPDBMESSAGE pBuffer);
	void		 		ProcessingDBMessage();
	void		 		SwitchList(){CDBProcess* pTemp = m_pDBProcess1;m_pDBProcess1 = m_pDBProcess2;m_pDBProcess2 = pTemp;};
	void		 		SwitchList2(){CDBMessage* pTemp = m_pDBResultMessage1;m_pDBResultMessage1 = m_pDBResultMessage2;m_pDBResultMessage2 = pTemp;};
	CDBMessage*	 		GetDBMessageAdd(){return m_pDBResultMessage1;}
	CDBMessage*	 		GetDBMessagePop(){return m_pDBResultMessage2;}
	BOOL		 		LoadDataByPtr( LPDBCON pCon, char* szQuery, int nSize, LPVOID pData);
	BOOL		 		SaveDataByPtr( LPDBCON pCon, char* szQuery, int nSize, LPVOID pPara, long nBuffersize, LPVOID pSettingData);
	HANDLE				GetKillReturnEvent(){return m_hKillReturnEvent;}
	void				SetUsingDll(DWORD val){m_bUsingDLL = val;}
	BOOL				GetUsingDll(){return m_bUsingDLL;}
	char				m_szDefault_User_table[128];
	char				m_szDefault_User_account[128];
	char				m_szDefault_User_pw[128];
	char				m_szDefault_User_guid[128];
	DWORD				m_dwDBMessageID;
    void				(*m_pWriteMessage)(char* pmsg);
	void				DeleteThisPointer(DWORD dwFnNo, LPVOID pTarget);
	char*				GetDBMessageMemory();
	char*				GetDBMessageStackMemory();
	void				FreeDBMessageMemory(char* pTarget);
	void				FreeDBStackMemory(char* pTarget);
	DWORD				GetUnusedStackMemoryNum();
	DWORD				GetUnusedDBMemoryNum();
	CDB(DESC_DBTHREAD* pDesc);
	~CDB();

private:
	SQLHANDLE			m_hEnv;
	SQLUINTEGER			m_NumRowsFetched;	// ÆÐÄ¡µÈ ·¹ÄÚµåÀÇ °¹¼ö 
	LPDBCON				m_pDBAdmin;
	LPDBCON				m_pDBClient;
	BOOL				m_bUsingThread;		// ¾²·¹µå »ç¿ë¿©ºÎ
	CRITICAL_SECTION	m_DBCS;				// DB¿ë ¾²·¹µåÀÇ ÀÓ°è¿µ¿ª
	CRITICAL_SECTION	m_ReturnMessage;

//	CRITICAL_SECTION	m_csDBMessageMemory;
//	CRITICAL_SECTION	m_csDBStackMemory;
//	CRITICAL_SECTION	m_csDBResultMemory;
//	CRITICAL_SECTION	m_csDBResultStackMemory;
	
	HANDLE				m_hDBThread;
	DWORD				m_hIDDBhread;
	HANDLE				m_hDBEvent;
	HANDLE				m_hKillDBEvent;
	HANDLE				m_hResultMessageEvent;
	HANDLE				m_hKillReturnEvent;
	CDBProcess*			m_pDBProcess1;
	CDBProcess*			m_pDBProcess2;
	CDBMessage*			m_pDBResultMessage1;
	CDBMessage*			m_pDBResultMessage2;
	HWND				m_hWndToPostMessage;
	BYTE				m_NumConnection;	// DB Ä¿³Ø¼ÇÀÇ °¹¼ö

	LPDBCON				m_ArrayCon[MAX_NUM_SQL_CONNECTION];
	BOOL				m_UsingEventObject;	// ¾²·¹µå¿¡¼­ Ã³¸® ¸Þ¼¼Áö¸¦ ÀÌº¥Æ®°´Ã¼¸¦ ÀÌ¿ëÇÒ °ÍÀÎÁö ¼³Á¤.
	BOOL				m_bUsingDLL;
	BOOL				m_SucceedInit;

	CMemoryPool*		m_pResultMemory;
	CMemoryPool*		m_pDBMessageMemory;
	CMemoryPool*		m_pDBMessageResultMemory;
	CMemoryPool*		m_pDBStackMemory;

	DWORD				m_dwMessageMaxSize;		// DBMESSAGE ±¸Á¶Ã¼ÀÇ ÃÖ´ë Å©±â
	DWORD				m_dwNumofMessage;			// ÃÖ´ë µ¿½Ã Äõ¸® °¡´É °¹¼ö 
	DWORD				m_dwResultMaxSize;		// Äõ¸® °á°úÀÇ ÃÖ´ë »çÀÌÁî(select¹®)
	DWORD				m_dwNumofResult;			// Äõ¸® °á°úÀÇ ÃÖ´ë °¹¼ö  (select¹®)
	DWORD				m_dwSizeofStack;			// Äõ¸®½Ã ÀÔ·ÂÇÑ ÀÎÀÚ¸¦ À§ÇÑ ¸Þ¸ð¸® Å©±â 

    void				(*m_pReadDBResult)(DWORD ret,  LPDBMESSAGE pMessage);
};

class CODB : public IDBThread
{
public:
    STDMETHODIMP				QueryInterface(REFIID, PPVOID);
	STDMETHODIMP_(ULONG)		AddRef(void);
    STDMETHODIMP_(ULONG)		Release(void);
	BOOL		__stdcall		CreateDBThread(DESC_DBTHREAD* pDesc);		
	BOOL		__stdcall		InitDBConnection();
	BOOL		__stdcall		InitDBConnection(LPSTR szID, LPSTR szPassword, LPSTR Dbname, LPSTR szIDC, LPSTR szPasswordC, LPSTR DbnameC);
	void		__stdcall		SendMessageDB(LPDBMESSAGE pPara);
	BOOL		__stdcall		CloseThisConnection(LPDBCON pCon);
	LPDBCON		__stdcall		MakeNewConnection(LPSTR lpDbName, LPSTR lpID, LPSTR lpPW);
	void		__stdcall		SetDBMessageEvent(HANDLE val);
	HANDLE		__stdcall		GetDBMessageEvent();
	void		__stdcall		CloseDBConnection();
	LPDBCON		__stdcall 		GetAdMinCon();
	LPDBCON		__stdcall 		GetClientCon();
	void		__stdcall		ProcessingDBMessage();
	void		__stdcall		DeleteThisPointer(DWORD dwFnNo, LPVOID pTarget);
	char*		__stdcall		GetDBMessageMemory();
	char*		__stdcall		GetDBMessageStackMemory();
	void		__stdcall		FreeThisMemory(LPVOID pMsg, LPVOID pData);
	DWORD		__stdcall		GetUnusedStackMemoryNum();
	DWORD		__stdcall		GetUnusedDBMemoryNum();
	CODB();
	virtual ~CODB();

private:
	DWORD						m_dwRefCount;
	CDB*						m_pDb;
};





///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Á¦°øµÇ´Â ±âº» DB¿ë ¾²·¹µå ÇÔ¼ö
// ¸¸ÀÏ ÇÔ¼ö¸íÀÌ ´Ù¸¦ °æ¿ì °íÃÄ¾ß ÇÑ´Ù.
extern void DBThread(LPVOID pDB);
