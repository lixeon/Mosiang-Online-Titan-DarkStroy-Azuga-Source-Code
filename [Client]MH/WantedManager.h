// WantedManager.h: interface for the CWantedManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WANTEDMANAGER_H__336352E4_3DC3_4194_9B12_D9D2DB641EE4__INCLUDED_)
#define AFX_WANTEDMANAGER_H__336352E4_3DC3_4194_9B12_D9D2DB641EE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WANTEDMGR USINGTON(CWantedManager)
#define WANTED_CHARGE 500
#include "Wanted.h"

enum iswaiting
{
	eNone, eStart, eWaiting, eDone,
};
struct WINFO
{
	WANTEDTYPE WantedIDX;
	DWORD CharacterIDX;
};

#ifdef TAIWAN_LOCAL
#define MIN_PRIZE 1000
#else
#define MIN_PRIZE 1000
#endif

#define MAX_PRIZE 1000000
class CWantedManager  
{
	DWORD m_MurdererID;

	CWanted m_Wanted;
	int m_bIsWaiting;
	BOOL m_bCanRevive;

	BOOL CanBuyWantedRight(WANTEDTYPE WantedIDX);
	BOOL CanRegist(MONEYTYPE Prize);

	int IsWaiting() { return m_bIsWaiting; }

	void SetCanRevive(BOOL val) { m_bCanRevive = val; }
	BOOL CanRevive() { return m_bCanRevive;	}

public:
	CWantedManager();
	virtual ~CWantedManager();
	
	void Init();
	void SetWaiting(BOOL val) { m_bIsWaiting = val; }
	
	void SetActiveDialog();
	void SetMurdererID(DWORD ID) { m_MurdererID = ID; }
	DWORD GetMurdererID() { return m_MurdererID;	}

	void NetworkMsgParse(BYTE Protocol,void* pMsg);
	
	void RegistKillerSyn(MONEYTYPE Prize);

	void BuyWantedRightSyn(WANTEDTYPE WantedIDX);
	void BuyWantedRightResult(WANTEDLIST * pInfo);
	
	void GiveUpWantedRightSyn(WANTEDTYPE WantedIDX);
	void GiveUpWantedRightResult(WANTEDTYPE WantedIDX);
	
	void DeleteFromWantedList(WANTEDTYPE WantedIDX, BYTE DelKind);
	
	BOOL IsInList(WANTEDTYPE WantedIDX);
	BOOL IsInList( char* strName );
	BOOL IsChrIDInList(DWORD CharacterID);		

	void SortWantedListSyn(WORD Page);
	void MyWantedListSyn(WORD Page);
	
	void SearchSyn(char* WantedName, WORD gotoPage);
};
EXTERNGLOBALTON(CWantedManager)
#endif // !defined(AFX_WANTEDMANAGER_H__336352E4_3DC3_4194_9B12_D9D2DB641EE4__INCLUDED_)
