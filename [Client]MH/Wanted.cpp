// Wanted.cpp: implementation of the CWanted class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Wanted.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWanted::CWanted()
{

}

CWanted::~CWanted()
{
	m_WantedList.RemoveAll();
}

void CWanted::InitInfo()
{
	m_WantedList.RemoveAll();
}

void CWanted::SetInfo(SEND_WANTEDLIST* pInfo)
{
	InitInfo();
	for(int i=0; i<MAX_WANTED_NUM;++i)
	{
		if(pInfo->List[i].WantedIDX != 0)
		{
			WANTEDLIST* pWantInfo = new WANTEDLIST;
			pWantInfo->WantedIDX = pInfo->List[i].WantedIDX;
			SafeStrCpy(pWantInfo->RegistDate, pInfo->List[i].RegistDate, 11);
			pWantInfo->WantedChrID = pInfo->List[i].WantedChrID;
			SafeStrCpy(pWantInfo->WantedName, pInfo->List[i].WantedName, MAX_NAME_LENGTH+1);
						
			m_WantedList.AddTail(pWantInfo);
		}
		else 
			return;
	}
}

BOOL CWanted::IsInList(WANTEDTYPE WantedIDX)
{
	PTRLISTSEARCHSTART(m_WantedList, WANTEDLIST*, pInfo)
		if(pInfo->WantedIDX == WantedIDX)
			return TRUE;
	PTRLISTSEARCHEND;
	return FALSE;
}

BOOL CWanted::IsInList( char* strName )
{
	PTRLISTSEARCHSTART(m_WantedList, WANTEDLIST*, pInfo)
		if( strcmp( strName, pInfo->WantedName ) == 0 )
			return TRUE;
	PTRLISTSEARCHEND;
	return FALSE;

}

BOOL CWanted::IsChrIDInList(DWORD CharacterID)
{
	PTRLISTSEARCHSTART(m_WantedList, WANTEDLIST*, pInfo)
		if(pInfo->WantedChrID == CharacterID)
			return TRUE;
	PTRLISTSEARCHEND;
	return FALSE;
}

BOOL CWanted::IsFull()
{
	if(m_WantedList.GetCount() == MAX_WANTED_NUM)
		return TRUE;
	return FALSE;
}

void CWanted::Add(WANTEDLIST* pInfo)
{
	WANTEDLIST * pWantInfo = new WANTEDLIST;
	pWantInfo->WantedIDX = pInfo->WantedIDX;
	pWantInfo->WantedChrID = pInfo->WantedChrID;
	SafeStrCpy(pWantInfo->RegistDate, pInfo->RegistDate, 11);
	SafeStrCpy(pWantInfo->WantedName, pInfo->WantedName, MAX_NAME_LENGTH+1);

	m_WantedList.AddTail(pWantInfo);
}

void CWanted::Delete(WANTEDTYPE WantedIDX)
{
	PTRLISTSEARCHSTART(m_WantedList, WANTEDLIST *, pInfo)
		if(pInfo->WantedIDX == WantedIDX)
		{
			m_WantedList.Remove(pInfo);
			return;
		}
	PTRLISTSEARCHEND
		ASSERT(0);
}

DWORD CWanted::GetWantedChrID(WANTEDTYPE WantedIDX)
{
	PTRLISTSEARCHSTART(m_WantedList, WANTEDLIST *, pInfo)
		if(pInfo->WantedIDX == WantedIDX)
			return pInfo->WantedChrID;
	PTRLISTSEARCHEND
	return 0;
}

WANTEDLIST* CWanted::GetWantedInfo(WANTEDTYPE WantedIDX)
{
	PTRLISTSEARCHSTART(m_WantedList, WANTEDLIST *, pInfo)
		if(pInfo->WantedIDX == WantedIDX)
			return pInfo;
	PTRLISTSEARCHEND
	return NULL;
}

void CWanted::GetTotalInfo(WANTEDLIST * rtInfo)
{
	memset(rtInfo, 0, sizeof(WANTEDLIST)*MAX_WANTED_NUM);
	WANTEDLIST* pList = NULL;
	PTRLISTPOS pos = m_WantedList.GetHeadPosition();
	for(int i=0; i<MAX_WANTED_NUM; ++i)
	{
		if(pos)
		{
			pList =(WANTEDLIST*)m_WantedList.GetAt(pos);
			memcpy(&rtInfo[i], pList, sizeof(WANTEDLIST));
			
			m_WantedList.GetNext(pos);
		}
		else
			break;
	}
}

char* CWanted::GetWantedName(WANTEDTYPE WantedIDX)
{
	PTRLISTSEARCHSTART(m_WantedList, WANTEDLIST *, pInfo)
		if(pInfo->WantedIDX == WantedIDX)
			return pInfo->WantedName;
	PTRLISTSEARCHEND
	return NULL;
}
