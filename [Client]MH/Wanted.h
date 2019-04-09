// Wanted.h: interface for the CWanted class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WANTED_H__68F68CB2_9047_4868_A1EA_51A05B8F750A__INCLUDED_)
#define AFX_WANTED_H__68F68CB2_9047_4868_A1EA_51A05B8F750A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PtrList.h"

class CWanted  
{
	cPtrList m_WantedList;
public:
	CWanted();
	virtual ~CWanted();
	
	void InitInfo();
	void SetInfo(SEND_WANTEDLIST* pInfo);
	BOOL IsInList(WANTEDTYPE WantedIDX);
	BOOL IsInList( char* strName );
	BOOL IsChrIDInList(DWORD CharacterID);
	BOOL IsFull();
	void Add(WANTEDLIST* pInfo);
	void Delete(WANTEDTYPE WantedIDX);
	DWORD GetWantedChrID(WANTEDTYPE WantedIDX);
	WANTEDLIST* GetWantedInfo(WANTEDTYPE WantedIDX);
	void GetTotalInfo(WANTEDLIST * rtInfo);
	char* GetWantedName(WANTEDTYPE WantedIDX);
};

#endif // !defined(AFX_WANTED_H__68F68CB2_9047_4868_A1EA_51A05B8F750A__INCLUDED_)
