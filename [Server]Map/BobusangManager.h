#pragma once

//SW070626 보부상NPC
#include "ItemManager.h"

#define BOBUSANG_NPCIDX	74
#define BOBUSANG_wNpcUniqueIdx	300		//"StaticNpcList.bin 에 비포함"
#define BOBUSANGMGR	USINGTON(BobusangManager)


class CNpc;

//struct BobusangInfo
//{
//	BobusangInfo() : AppearanceMapNum(0), AppearanceChannel(0), AppearanceStartTime(0), AppearanceDurationTime(0), SellingListIndex(0) {};
//	DWORD	AppearanceMapNum;
//	DWORD	AppearanceChannel;
//	DWORD	AppearanceStartTime;
//	DWORD	AppearanceDurationTime;
//	DWORD	SellingListIndex;
//};

struct BOBUSANGTOTALINFO
{
	BOBUSANGTOTALINFO() : pBobusang(NULL), pDealItemInfo(NULL) {};	//{ memset(SellingItem, 0, (sizeof(BOBUSANG_ITEM) * SLOT_NPCINVEN_NUM)); };
	BOBUSANGINFO		AppearanceInfo;
	//BOBUSANG_ITEM		SellingItem[SLOT_NPCINVEN_NUM];
	CNpc*				pBobusang;
	DealerData*			pDealItemInfo;
	cPtrList			SellingItemList;
	cPtrList			pCustomerList;
};

class BobusangManager
{
	BOBUSANGTOTALINFO*	m_pBobusang;	// 채널 갯수 만큼..

public:
	BobusangManager(void);
	~BobusangManager(void);

	void	BobusangMgr_Init();
	void	BobusangMgr_Process();
	void	BobusangMgr_Release();

	// 맵서버에서 생성 메시지를 받아 addobject.
	BOOL	MakeNewBobusangNpc(BOBUSANGINFO* pBobusangInfo);
	BOOL	RemoveBobusangNpc(DWORD channelNum);
	// Agent에서 받은 정보를 세팅한다. 생성시각/유지시간/품목인데스
	void	SetBobusanInfo(BOBUSANGINFO* pBobusangInfo);			// set struct BobusangInfo(...)
	BOBUSANGTOTALINFO* GetBobusang(DWORD mapChannelNum);
	int		GetBobusangSellingRt(DWORD channelNum, DealerItem* pItemList);

	void	AddGuest(CPlayer* pPlayer);
	void	LeaveGuest(CPlayer* pPlayer);
	void	SendLeaveGuestMsg(CPlayer* pPlayer);

	BOOL	BuyItem(CPlayer* pPlayer, WORD buyItemIdx, WORD BuyItemNum);

	DealerItem*	GetSellingItem(cPtrList* pSellingItemList, WORD buyItemIdx);

	// 보부상 생성
	void	AppearBobusang(MSG_BOBUSANG_INFO* pmsg);
	void	DisappearBobusang(MSG_DWORD2* pmsg);

	BOOL	CheckHackBobusangDist(CPlayer* pGuest);
	void	SendDealerItemMsgToGuestAll(DWORD channelNum, MSGBASE* pmsg, int size);
	void	NetworkMsgParser( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg );
};

EXTERNGLOBALTON(BobusangManager)
