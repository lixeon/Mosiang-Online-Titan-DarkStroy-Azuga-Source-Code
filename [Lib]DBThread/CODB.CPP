#include "stdafx.h"
#include "DB.h"
#include "dbclass.h"
#include "memorypool.h"


CODB::CODB()
{
	m_dwRefCount		=	0;
	m_pDb				=	NULL;
}

CODB::~CODB()
{
	if (m_pDb)
		delete m_pDb;
	m_pDb = NULL;

}


STDMETHODIMP CODB::QueryInterface(REFIID refiid, PPVOID ppv)
{
	if (IID_IUnknown == refiid)
		*ppv = this;
	else if (IID_DBTHREAD_DLL == refiid)
		*ppv = (IDBThread*)this;
	else 
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	((IUnknown*)(*ppv))->AddRef();
	return NOERROR;
	
}
STDMETHODIMP_(ULONG) CODB::AddRef(void)
{
	InterlockedIncrement((LPLONG)&m_dwRefCount);
	return m_dwRefCount;
}
STDMETHODIMP_(ULONG) CODB::Release(void)
{

	InterlockedDecrement((LPLONG)&m_dwRefCount);
	if (!m_dwRefCount)
		delete this;
	return 0;
}

BOOL __stdcall	CODB::CreateDBThread(DESC_DBTHREAD* pDesc)
{
	if(m_pDb) 
	{
		m_pDb->m_pWriteMessage("CreateDBThread : Already Alloced CDB Class");
		return false;	
	}

	m_pDb = new CDB(pDesc);
	if (!m_pDb) return false;

	m_pDb->m_dwDBMessageID	=	pDesc->dwDBMessageID;
	m_pDb->SetUsingDll(pDesc->bUsingDLL);
	m_pDb->SetUsingThread(pDesc->bUsingThread);
	m_pDb->SetIsUseEvnetDB(pDesc->bUsingEventObject);
	m_pDb->SetHwnd(pDesc->hWndToPostMessage);
	m_pDb->SetOnResultFunc(pDesc->ReadDBResult);
	m_pDb->SetOnWriteMessageFunc(pDesc->WriteMessage);

	strcpy(m_pDb->GetAdMinCon()->szCursorName,pDesc->AdminCursor);
	strcpy(m_pDb->GetAdMinCon()->szDBName,pDesc->DBName);
	strcpy(m_pDb->GetAdMinCon()->szID,pDesc->AdminID);
	strcpy(m_pDb->GetAdMinCon()->szPW,pDesc->AdminPW);
	strcpy(m_pDb->GetClientCon()->szDBName,pDesc->DBName);
	strcpy(m_pDb->GetClientCon()->szID,pDesc->ClientID);
	strcpy(m_pDb->GetClientCon()->szPW,pDesc->ClientPW);
	strcpy(m_pDb->GetClientCon()->szCursorName,pDesc->ClientCursor);
	strcpy(m_pDb->m_szDefault_User_table,pDesc->Default_User_table);
	strcpy(m_pDb->m_szDefault_User_account,pDesc->Default_User_account);
	strcpy(m_pDb->m_szDefault_User_pw,pDesc->Default_User_pw);
	strcpy(m_pDb->m_szDefault_User_guid,pDesc->Default_User_guid);

	if (m_pDb->GetUsingDll() == 0)
		m_pDb->SetDBMessageEvent(CreateEvent(NULL, FALSE, FALSE, NULL));

	char* ret = m_pDb->InitDBConnection();
	if(!ret)
	{
		m_pDb->m_pWriteMessage("Fail to initialize DB Connection");
		return false;
	}
	m_pDb->m_pWriteMessage("DB INIT OK");


	return TRUE;
}

BOOL	__stdcall	CODB::InitDBConnection()
{
	return (BOOL) m_pDb->InitDBConnection();
}


BOOL	__stdcall	CODB::InitDBConnection(LPSTR szID, LPSTR szPassword, LPSTR Dbname, LPSTR szIDC, LPSTR szPasswordC, LPSTR DbnameC) 
{
	return (BOOL) m_pDb->InitDBConnection(szID, szPassword,Dbname, szIDC, szPasswordC, DbnameC);
}

void	__stdcall	CODB::SendMessageDB(LPDBMESSAGE pPara) 
{
	m_pDb->SendMessageDB(pPara);
}

BOOL	__stdcall	CODB::CloseThisConnection(LPDBCON pCon) 
{
	return m_pDb->CloseThisConnection(pCon); 
}
LPDBCON		CODB::MakeNewConnection(LPSTR lpDbName, LPSTR lpID, LPSTR lpPW)
{
	return m_pDb->MakeNewConnection(lpDbName,lpID,lpPW);
}
void	__stdcall	CODB::SetDBMessageEvent(HANDLE val)
{
	m_pDb->SetDBMessageEvent(val);
}
HANDLE	__stdcall	CODB::GetDBMessageEvent()
{
	return m_pDb->GetDBMessageEvent();
}
void	__stdcall	CODB::CloseDBConnection() 
{
	m_pDb->CloseDBConnection();
}

LPDBCON		__stdcall 		CODB::GetAdMinCon()
{
	return m_pDb->GetAdMinCon();
}
LPDBCON		__stdcall 		CODB::GetClientCon()
{
	return m_pDb->GetClientCon();
}

void		__stdcall		CODB::ProcessingDBMessage()
{
	m_pDb->ProcessingDBMessage();
}


void		__stdcall		CODB::DeleteThisPointer(DWORD dwFnNo, LPVOID pTarget)
{
	m_pDb->DeleteThisPointer(dwFnNo, pTarget);
}

char*		__stdcall		CODB::GetDBMessageMemory()
{
	return m_pDb->GetDBMessageMemory();
}

char*		__stdcall		CODB::GetDBMessageStackMemory()
{
	return m_pDb->GetDBMessageStackMemory();
}

void		__stdcall		CODB::FreeThisMemory(LPVOID pMsg, LPVOID pData)
{
	// pMsg가 메세지 구조체 
	// pData는 인자 구조체.
	if (pMsg) m_pDb->FreeDBMessageMemory((char*) pMsg);
	if (pData) m_pDb->FreeDBStackMemory((char*) pData);

}

DWORD		__stdcall		CODB::GetUnusedStackMemoryNum()
{
	return m_pDb->GetUnusedStackMemoryNum();
}

DWORD		__stdcall		CODB::GetUnusedDBMemoryNum()
{
	return m_pDb->GetUnusedDBMemoryNum();
}
