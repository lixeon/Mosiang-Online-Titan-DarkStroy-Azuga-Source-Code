//////////////////////////////////////////////////////////////////////
// Coolwater.Fire version 2001.1.15
// Ultimate Database Gasoline Engine
// If it does not work correctly, plz enter www.coolwater.fire.com 
// 
// programed by Park Sung Jun 
//
//////////////////////////////////////////////////////////////////////
#define _WIN32_WINNT 0x0500

#include "stdafx.h"
#include "DB.h"
#include "dbclass.h"
#include <process.h>

////////////////////////////////////////////////////////////////////////////////////////////
// Æã¼Ç ¹øÈ£ 
// 1:  BOOL		ExcuteIUD(LPDBCON pCon, LPSTR szQuery);
// 2:  BOOL		InsertRecord(LPDBCON pCon, LPSTR szTableName, LPVOID lpCol, LPVOID lpValue, BYTE bNumData);
// 3:  BOOL		DeleteRecord(LPDBCON pCon, LPSTR szTableName, LPSTR lpCol, LPSTR lpValue);
// 4:  BOOL		UpdateRecord(LPDBCON pCon, LPSTR szTableName, LPSTR lpCol, LPSTR lpValue, LPSTR DestCol, LPSTR	DestValue);
// 5:  BOOL		Login(LPDBCON pCon, LPSTR szTableName, LPSTR pColID, LPSTR pColPW, LPSTR pID, LPSTR pPW, LPSTR pRetCol, LPVOID pRetValue, int iLength,SQLSMALLINT sqlType);
// 6:  BOOL		SimpleLogin(LPDBCON pCon,LPSTR pID, LPSTR pPW, LPVOID pRetValue);
// 7:  BOOL		BindQuery(LPDBCON pCon,LPSTR szBuffer, LPBIND lpBind, int Rowset, int StSize, SQLSMALLINT sqlType);
// 8:  BOOL		DeleteRecordInCursor(LPDBCON pCon);
// 9:  BOOL		GenerateResultSet(LPDBCON pCon, LPSTR szBuffer, LPBIND lpBind, int StSize, SQLSMALLINT sqlType, int rowset);
// 10: BOOL		GetNextRecordSet(LPDBCON pCon);
// 11: BOOL		GetLastRecordSet(LPDBCON pCon);
// 12: BOOL		GetFirstRecordSet(LPDBCON pCon);
// 13: BOOL		GetPriorRecordSet(LPDBCON pCon);
// 14: BOOL		CloseRecordSet(LPDBCON pCon);
// 15: BOOL		SaveDataByPtr( LPDBCON pCon, char* szQuery, int nSize, LPVOID pPara, long nBuffersize, LPVOID pSettingData)
// 16: BOOL		LoadDataByPtr( LPDBCON pCon, char* szQuery, int nSize, LPVOID pData)
// 17: BOOL		ExcuteIUDForProcedure(LPDBCON pCon, LPSTR szQuery);

////////////////////////////////////////////////////////////////////////////////////////////

// 101: LPQUERY FAR PASCAL		FreeQuery(LPDBCON pCon,LPSTR szBuffer,LPDWORD pNumRow);
// 102: LPQUERY	FAR PASCAL		SingleQuery(LPDBCON pCon, LPSTR szTable, DWORD dwValue, LPSTR lpCol, SQLSMALLINT sqltype);
// 103: LPQUERYDA FAR PASCAL	SingleQueryDA(LPDBCON pCon, LPSTR szTable, DWORD dwValue, LPSTR lpCol, SQLSMALLINT sqltype);

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////
// ¾²·¹µå ÃÊ±âÈ­ ÇÔ¼ö
// DB Å¬·¡½º »ý¼ºÀÚ¿¡¼­ ½ÇÇàµÈ´Ù.
BOOL CDB::InitDBThread()
{
	InitializeCriticalSectionAndSpinCount(&m_DBCS,200);
	InitializeCriticalSectionAndSpinCount(&m_ReturnMessage,200);
//	InitializeCriticalSection(&m_DBCS);
//	InitializeCriticalSection(&m_ReturnMessage);

	m_hDBEvent				= CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hKillDBEvent			= CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hKillReturnEvent		= CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hDBThread				= (HANDLE)_beginthread(DBThread,0,this);
/*	m_hDBThread = CreateThread(NULL,			// default security
								   0,				// default stack size
								   lpStart,	// pointer to thread routine
								   this,		// argument for thread
								   0,				// start it right away
								   &m_hIDDBhread);
*/	
	return 1;
}



////////////////////////////////////////////////////////////////////////////////////////////
// ¾²·¹µå ¼Ò¸ê ÇÔ¼ö
// DB Å¬·¡½º ¼Ò¸êÀÚ¿¡¼­ ½ÇÇàµÈ´Ù.
BOOL	CDB::CloseDBThread()
{	
	if (m_hDBThread && m_SucceedInit)
	{
		SetEvent(m_hKillDBEvent);
		WaitForSingleObject(m_hKillReturnEvent, INFINITE);
//		CloseHandle(m_hDBThread);
		m_hDBThread = NULL;
	}
	if(!m_bUsingDLL)
		CloseHandle(m_hResultMessageEvent);

	if (m_SucceedInit)
	{
		DeleteCriticalSection(&m_DBCS);
		DeleteCriticalSection(&m_ReturnMessage);
		
		CloseHandle(m_hDBEvent);
		CloseHandle(m_hKillDBEvent);
		CloseHandle(m_hKillReturnEvent);
	}
	return 1;
}

////////////////////////////////////////////////////////////
// ¾²·¹µå¿¡ ¸Þ½ÃÁî¸¦ ³¯¸°´Ù.
// ¿¹Á¦ ÂüÁ¶ 
void CDB::SendMessageDB(LPDBMESSAGE pPara)
{
	EnterCriticalSection(&m_DBCS);
	(GetDBProcessAdd())->AddTail(pPara);
	LeaveCriticalSection(&m_DBCS);
	SetEvent(m_hDBEvent);
}
////////////////////////////////////////////////////////////
void CDB::DispatchDBMessage(DWORD pRet,LPDBMESSAGE pBuffer)
{
	if (m_UsingEventObject)
	{
		LPDBRESULTMESSAGE pMessage = (LPDBRESULTMESSAGE) m_pDBMessageResultMemory->Alloc();
		if (!pMessage) 	m_pWriteMessage("DispatchDBMessage : memory allo fail");
		pMessage->lpDBMessage = pBuffer;
		pMessage->Ret = pRet;

		EnterCriticalSection(&m_ReturnMessage);
		GetDBMessageAdd()->AddTail(pMessage);
		LeaveCriticalSection(&m_ReturnMessage);

		SetEvent(m_hResultMessageEvent);
	}
	else
	::PostMessage(m_hWndToPostMessage, m_dwDBMessageID,(WPARAM) pRet, (LPARAM) pBuffer);
}
////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////
// DB ¾²·¹µå°¡ Ã³¸®°á°ú¸¦ ÀÌº¥Æ® °´Ã¼·Î ¹ÝÈ¯ÇÒ °æ¿ì
// ÀÌ ÀÌº¥Æ® °´Ã¼¸¦ ÂüÁ¶ÇÏ´Â µ¿±âÈ­ ¾²·¹µå¿¡¼­
// DB Ã³¸®°á°ú°¡ µµÂøÇß´Ù´Â ÀÌº¥Æ®°¡ ¹ß»ýÇÏ¸é
// ÀÌ ÇÔ¼ö¸¦ ½ÇÇàÇØ ÁØ´Ù.
void CDB::ProcessingDBMessage()
{
	POSITION_	pos, posbuffer;
	EnterCriticalSection(&m_ReturnMessage);
	
	SwitchList2();
	
	LeaveCriticalSection(&m_ReturnMessage);
	// ¸®½ºÆ®ÀÇ Çì´õÆ÷Áö¼Ç.
	pos = GetDBMessagePop()->GetHeadPosition();

	LPDBRESULTMESSAGE lpBuffer;
	
	while(pos != NULL)
	{
		// ¸Þ¼¼ÁöÃ³¸®ÇÏ±â À§ÇÑ ¸Þ¸ð¸® ¼³Á¤.
		// µû¶ó¼­ ¸ÞÀÎ ¾²·¹µå´Â ¸Þ¼¼Áö¸¦ ¹ÞÀ¸¸é ³»¿ë¿¡ µû¶ó Ã³¸®ÇÑÈÄ ¹Ýµå½Ã ÇÁ¸®ÇØÁà¾ß ÇÑ´Ù.
		posbuffer = pos;
		lpBuffer = (LPDBRESULTMESSAGE) GetDBMessagePop()->GetNext(pos);
		if (lpBuffer != NULL)
		{
			GetDBMessagePop()->RemoveAtFromList(posbuffer);
			m_pReadDBResult(lpBuffer->Ret,  lpBuffer->lpDBMessage);
			// modify 4.4
			m_pDBMessageResultMemory->Free((char*)lpBuffer);
		}
	}

	EnterCriticalSection(&m_ReturnMessage);
		
		SwitchList2();
		if (GetDBMessagePop()->IsEmpty() == FALSE)
		{
			SwitchList2();
			SetEvent(m_hResultMessageEvent);
		}
	LeaveCriticalSection(&m_ReturnMessage);

}



////////////////////////////////////////////////////////////////////////////////////////////
// DB¿ë ¾²·¹µå ÇÔ¼ö.
// Å¬·¡½º¿¡ Æ÷ÇÔµÇÁö ¾ÊÀ¸¹Ç·Î Àü¿ªÀ¸·Î µÇ¾î ÀÖ´Ù.
void DBThread(LPVOID pPara)
{
	
	CDB* pDB = (CDB*) pPara;
	HANDLE		eventHandles[2];
	POSITION_	pos, posbuffer;
	eventHandles[0] = pDB->GetDBEvent();
	eventHandles[1] = pDB->GetDBKillEvent();
	DWORD dwRet;

	LPDBMESSAGE lpSend;
//
	while (WaitForMultipleObjects(2, eventHandles, FALSE, INFINITE) == WAIT_OBJECT_0)
	{
		EnterCriticalSection(pDB->GetDBCS());
			pDB->SwitchList();
		LeaveCriticalSection(pDB->GetDBCS());

		// ¸®½ºÆ®ÀÇ Çì´õÆ÷Áö¼Ç.
		pos = (pDB->GetDBProcessPop())->GetHeadPosition();
		while(pos != NULL)
		{
			posbuffer = pos;
			lpSend = (LPDBMESSAGE) (pDB->GetDBProcessPop())->GetNext(pos);
			
			if (lpSend != NULL)
			{
				(pDB->GetDBProcessPop())->RemoveAtFromList(posbuffer);
				if (lpSend->bFNNo > 100)
				{
					LPVOID pRet;
					pRet = pDB->ExcuteThreadFN(lpSend);
					pDB->DispatchDBMessage((DWORD)pRet,lpSend);
				}
				else 
				{
					dwRet = pDB->ExcuteThreadFNbool(lpSend);
					if (dwRet)
					{
						if (lpSend->bNotifyOnlyError)
						{
							if (lpSend->bFNNo == 2)
							{
								pDB->DispatchDBMessage(dwRet,lpSend);					
							}
							else if (lpSend->bFNNo == 14 || lpSend->bFNNo == 8 || lpSend->bFNNo == 10 ||lpSend->bFNNo == 11 ||lpSend->bFNNo == 12 ||lpSend->bFNNo == 13)
							{
								pDB->FreeDBMessageMemory((char*)lpSend);
							}
							else if (lpSend->bFNNo == 5 || lpSend->bFNNo == 6 || lpSend->bFNNo == 15 || lpSend->bFNNo == 16)
							{
								pDB->DispatchDBMessage(dwRet,lpSend);
							}
							else 
							{								
								pDB->FreeDBStackMemory((char*)lpSend->pData);
								pDB->FreeDBMessageMemory((char*)lpSend);
							}
						}
						else
							pDB->DispatchDBMessage(dwRet,lpSend);					
					}
					else pDB->DispatchDBMessage(dwRet,lpSend);
				}
			}
		}	

		EnterCriticalSection(pDB->GetDBCS());
			
			pDB->SwitchList();
			if ((pDB->GetDBProcessPop())->IsEmpty() == FALSE)
			{
				pDB->SwitchList();
				SetEvent(pDB->GetDBEvent());
			}
		LeaveCriticalSection(pDB->GetDBCS());
	}
	SetEvent(pDB->GetKillReturnEvent());
	_endthread();
	return;
}


LPVOID	CDB::ExcuteThreadFN(LPDBMESSAGE pBuffer)
{
	switch (pBuffer->bFNNo)
	{
	case 101:
	{
		LPDBFreeQuery lpMessage = (LPDBFreeQuery) pBuffer->pData;
		return FreeQuery(lpMessage->pCon, lpMessage->szBuffer, &(pBuffer->dwResult));
	}
	case 104:
	{
		LPDBFreeQuery lpMessage = (LPDBFreeQuery) pBuffer->pData;
		return FreeLargeQuery(lpMessage->pCon, lpMessage->szBuffer, &(pBuffer->dwResult));
	}
	case 105:
	{
		LPDBFreeQuery lpMessage = (LPDBFreeQuery) pBuffer->pData;
		return FreeMiddleQuery(lpMessage->pCon, lpMessage->szBuffer, &(pBuffer->dwResult));
	}
	case 102:
	{
		LPDBSingleQuery lpMessage = (LPDBSingleQuery) pBuffer->pData;
		return SingleQuery(lpMessage->pCon, lpMessage->szTable,lpMessage->dwValue,lpMessage->lpCol,lpMessage->sqltype);
	}
	case 103:
	{
		LPDBSingleQuery lpMessage = (LPDBSingleQuery) pBuffer->pData;
		return SingleQueryDA(lpMessage->pCon, lpMessage->szTable,lpMessage->dwValue,lpMessage->lpCol,lpMessage->sqltype);
	}
	}
	return NULL;
}


BOOL	CDB::ExcuteThreadFNbool(LPDBMESSAGE pBuffer)
{
	switch (pBuffer->bFNNo)
	{
		case 1:
		{
			LPDBExcuteIUD pMessage = (LPDBExcuteIUD) pBuffer->pData;
			return ExcuteIUD(pMessage->pCon,pMessage->szQuery);
		}
		case 2:
		{
			LPDBInsertRecord pMessage = (LPDBInsertRecord) pBuffer->pData;
			return InsertRecord(pMessage->pCon,pMessage->szTableName,pMessage->lpCol,pMessage->lpValue,pMessage->bNumData);
		}
		case 3:
		{
			LPDBDeleteRecord pMessage = (LPDBDeleteRecord) pBuffer->pData;
			return DeleteRecord(pMessage->pCon,pMessage->szTableName,pMessage->lpCol,pMessage->lpValue);
		}
		case 4:
		{
			LPDBUpdateRecord pMessage = (LPDBUpdateRecord) pBuffer->pData;
			return UpdateRecord(pMessage->pCon,pMessage->szTableName,pMessage->lpCol,pMessage->lpValue,pMessage->DestCol,pMessage->DestValue);
		}
		case 5:
		{
			LPDBLogin pMessage = (LPDBLogin) pBuffer->pData;
			return Login(pMessage->pCon,pMessage->szTableName,pMessage->pColID,pMessage->pColPW,pMessage->pID,pMessage->pPW,pMessage->pRetCol,pMessage->pRetValue,pMessage->iLength,pMessage->sqlType);
		}
		case 6:
		{
			LPDBSimpleLogin pMessage = (LPDBSimpleLogin) pBuffer->pData;
			return SimpleLogin(pMessage->pCon,pMessage->pID,pMessage->pPW,&(pBuffer->dwResult));
		}
		case 7:
		{
			LPDBGenerateResultSet pMessage = (LPDBGenerateResultSet) pBuffer->pData;
			return BindQuery(pMessage->pCon,pMessage->szBuffer,pMessage->lpBind,pMessage->rowset,pMessage->StSize,pMessage->sqlType);
		}
		case 8:
		{
			LPDBCON pMessage = (LPDBCON) pBuffer->pData;
			return DeleteRecordInCursor(pMessage);
		}
		case 9:
		{
			LPDBGenerateResultSet pMessage = (LPDBGenerateResultSet) pBuffer->pData;
			return GenerateResultSet(pMessage->pCon,pMessage->szBuffer,pMessage->lpBind,pMessage->StSize,pMessage->sqlType,pMessage->rowset);
		}
		case 10:
		{
			LPDBCON pMessage = (LPDBCON) pBuffer->pData;
			return GetNextRecordSet(pMessage);
		}
		case 11:
		{
			LPDBCON pMessage = (LPDBCON) pBuffer->pData;
			return GetLastRecordSet(pMessage);
		}
		case 12:
		{
			LPDBCON pMessage = (LPDBCON) pBuffer->pData;
			return GetFirstRecordSet(pMessage);
		}
		case 13:
		{
			LPDBCON pMessage = (LPDBCON) pBuffer->pData;
			return GetPriorRecordSet(pMessage);
		}
		case 14:
		{
			LPDBCON pMessage = (LPDBCON) pBuffer->pData;
			return CloseRecordSet(pMessage);
		}
		case 15:
		{
			LPDBSaveDataByPtr pMessge = (LPDBSaveDataByPtr) pBuffer->pData;
			return SaveDataByPtr(pMessge->pCon,pMessge->szQuery,pMessge->nSize,pMessge->pPara,pMessge->nBuffersize,pMessge->pSettingData);
		}
		case 16:
		{
			LPDBLoadDataByPtr lpMessage = (LPDBLoadDataByPtr) pBuffer->pData;
			return LoadDataByPtr(lpMessage->pCon, lpMessage->szQuery,lpMessage->nSize,lpMessage->pData);
		}
		case 17:
		{
			LPDBExcuteIUD pMessage = (LPDBExcuteIUD) pBuffer->pData;
			return ExcuteIUDForProcedure(pMessage->pCon,pMessage->szQuery);
		}
			

	}
	return NULL;
}

