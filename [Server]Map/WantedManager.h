// WantedManager.h: interface for the CWantedManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WANTEDMANAGER_H__513746A9_4806_4181_BD6D_C9D2EDDEBD11__INCLUDED_)
#define AFX_WANTEDMANAGER_H__513746A9_4806_4181_BD6D_C9D2EDDEBD11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
enum Wanted
{
	eWan_Date, eWan_Prize, eWan_Volunteer, eWan_OrderTypeMax,
};

#define WANTEDMGR CWantedManager::GetInstance()
#define WANTED_CHARGE 500

#define MIN_PRIZE 1000
#define MAX_PRIZE 1000000

#define MIN_PRIZE_CHINA 1000

enum CompleteNum
{
	eComplete_byTime = 1,
};
#include "WantedInfo.h"

class CWantedManager  
{
	CYHHashTable<CWantedInfo> m_WantedHashTable;
	
	MONEYTYPE GetPrize(WANTEDTYPE WantedIDX);
	void GetOrderType(BYTE ordertype, char* type);
	BOOL IsWantedPlayer(CPlayer* pPlayer, DWORD WantedIdx);
public:
	MAKESINGLETON(CWantedManager);
	CWantedManager();
	virtual ~CWantedManager();
	
	void SendRegistNackMsg(CPlayer* pPlayer, BYTE err);
	void NetworkMsgParse( BYTE Protocol, void* pMsg );
	
	void LoadWantedList(WANTEDINFO_LOAD* pInfo);

	void RegistKillerSyn(CPlayer* pPlayer, DWORD TargetID, MONEYTYPE Prize);
	void RegistKillerResult(WANTEDINFO* pInfo);
	void DoRegistKiller(WANTEDINFO* pInfo);

	void DeleteFromWantedList(WANTEDTYPE WantedIDX);
	BOOL DoDeleteFromWantedList(WANTEDTYPE WantedIDX);

	void BuyWantedRightSyn(CPlayer* pPlayer, WANTEDTYPE WantedIDX);
	
	void GiveUpWantedRightSyn(CPlayer* pPlayer, WANTEDTYPE WantedIDX);
	
	void Complete(CPlayer* pPlayer, CPlayer* pTarget);
	void CanNotComplete(CPlayer* pPlayer, WANTEDTYPE WantedIDX, char* CompleterName, BYTE type);
	
	int CanBuyWantedRight(CPlayer* pPlayer, CWantedInfo* pInfo);
	
	CWantedInfo* GetWantedInfo(DWORD WantedIDX);
	
	BOOL IsOwner(CPlayer* pPlayer, WANTEDTYPE WantedIDX);
	void SendRegistWanted(CPlayer* pPlayer, CPlayer* pTarget);
	
	void CanNotCompleteByDelChr(CPlayer* pPlayer, WANTEDTYPE WantedIDX);
	void DestroyWanted(CPlayer* pPlayer);
	void DoDestroyedWanted(DWORD WantedIDX);
};

#endif // !defined(AFX_WANTEDMANAGER_H__513746A9_4806_4181_BD6D_C9D2EDDEBD11__INCLUDED_)
