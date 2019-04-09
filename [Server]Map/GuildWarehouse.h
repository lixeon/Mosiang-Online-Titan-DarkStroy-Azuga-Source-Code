// GuildWarehouse.h: interface for the CGuildWarehouse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDWAREHOUSE_H__8391ADAF_23A8_4CC2_8889_AFB06E62943E__INCLUDED_)
#define AFX_GUILDWAREHOUSE_H__8391ADAF_23A8_4CC2_8889_AFB06E62943E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MunpaWareSlot.h"
#include "Purse.h"

class CGuildWarehouse : public CMunpaWareSlot  
{
	cPtrList m_WarehousePlayerList[TAB_GUILDWAREHOUSE_NUM]; //창고 보고 있는 사람들
	ITEMBASE	m_TotalItemBase[SLOT_GUILDWAREHOUSE_NUM];
	SLOTINFO	m_TotalSlotInfo[SLOT_GUILDWAREHOUSE_NUM];
	void AddWarehousePlayer(DWORD PlayerID, BYTE TabNum);
	void RemoveWarehousePlayer(DWORD PlayerID, BYTE TabNum);
	void SendtoWarehousePlayer(BYTE TabNum, MSGBASE* msg, int size, DWORD ExceptID=0);
	
	friend class CMunpaWarePurse;
	
	CMunpaWarePurse m_GuildWarePurse;
	
public:
	CGuildWarehouse();
	virtual ~CGuildWarehouse();

	virtual ERROR_ITEM InsertItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItem, WORD state=SS_NONE);
	virtual ERROR_ITEM DeleteItemAbs(CPlayer * pPlayer, POSTYPE absPos, ITEMBASE * pItemOut, WORD state=SS_NONE);
	virtual BOOL IsEmpty(POSTYPE absPos);
	virtual ITEMBASE const * GetItemInfoAbs(POSTYPE absPos);
	//SW050920 수정 Rare
	virtual ERROR_ITEM UpdateItemAbs(CPlayer * pPlayer, POSTYPE whatAbsPos, DWORD dwDBIdx, WORD wItemIdx, POSTYPE position, POSTYPE quickPosition, DURTYPE Dur, WORD flag=UB_ALL, WORD state=SS_NONE, DWORD RareDBIdx=0);

	void Init(MONEYTYPE GuildMoney);
	
	void EnterWareHouse(CPlayer* pPlayer,BYTE TabNum);
	void LeaveWareHouse(DWORD PlayerID,BYTE TabNum);

	BOOL InitGuildItem(ITEMBASE* pInfo);
	
//	BOOL AddGuildItem(CPlayer* pPlayer,ITEMBASE* pInfo);
//	BOOL DeleteGuildItem(CPlayer* pPlayer,POSTYPE absPos,ITEMBASE* pRtInfo);


	void SendtoWarehousePlayerAll(MSGBASE* msg, int size);
	MONEYTYPE SetMoney( MONEYTYPE ChangeValue, BYTE bOper, BYTE MsgFlag );
	MONEYTYPE GetMaxMoney();
//	BYTE GetTabNum(POSTYPE absPos);
	
	BOOL	IsEmptyAndMoney();

	//SW060526 문파창고 아이템 정보저장 시점 변경
	void AddItemInfoWaitingMember(CPlayer* pPlayer, BYTE TabNum);
	void SendItemInfoToAllWaitingMembers();
};

#endif // !defined(AFX_GUILDWAREHOUSE_H__8391ADAF_23A8_4CC2_8889_AFB06E62943E__INCLUDED_)
