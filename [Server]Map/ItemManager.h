// ItemManager.h: interface for the CItemManager class.
// created by taiyo
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMMANAGER_H__B48BE6A4_2998_4459_B958_995D81EAAF1E__INCLUDED_)
#define AFX_ITEMMANAGER_H__B48BE6A4_2998_4459_B958_995D81EAAF1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IndexGenerator.h"
#include "cLinkedList.h"
#include "HashTable.h"
#include "..\[CC]Header\GameResourceStruct.h"

class CMHFile;
struct ITEM_INFO;
class ITEM_MIX_INFO;
struct MATERIAL_ARRAY;
class ITEM_MIX_RES;
class ITEM_REINFORCE_INFO;
// < 함수를 구현 할 때 반드시 지켜야 할 것 >
// 1. 함수에서 에러처리는 완벽히 처리 할 것
// 2. 에러처리 할때 (원래 작업으로 되돌릴 것 RollBack!!!)
#define MAX_MIX_PERCENT	10000
#define MAX_MIX_LARGE_PERCENT	1000000
#define ITEMMGR CItemManager::GetInstance()

#define EVENTITEM_SHIELD_RECOVER 53031
#define EVENTITEM_ALL_RECOVER 53032

class CItemSlot;

struct DealerItem
{
	BYTE Tab;
	BYTE Pos;
	WORD ItemIdx;
	int	ItemCount;
};
struct DealerData
{

//	POS pos;
//public:
//	void SetPositionHead(){	pos = m_DealItemList.GetFirstPos();	}
//	DealerItem* GetNextItem(){	return m_DealItemList.GetNextPos(pos);	}
public:
	BOOL FindItem(WORD ItemIdx)
	{
		POS pos = m_DealItemList.GetFirstPos();
		while(DealerItem* pItem = m_DealItemList.GetNextPos(pos) )
		{
			//SW070626 보부상NPC
			//if(pItem->ItemIdx == ItemIdx)
			if(pItem->ItemIdx == ItemIdx && pItem->ItemCount != 0)	// -1 : 무제한		1~5 : 수량 카운트다운(보부상에서만 쓰임)
				return TRUE;
		}

		return FALSE;
	}

	//SW070626 보부상NPC
	DealerItem*	GetItem(WORD ItemIdx)
	{
		POS pos = m_DealItemList.GetFirstPos();
		while(DealerItem* pItem = m_DealItemList.GetNextPos(pos) )
		{
			if(pItem->ItemIdx == ItemIdx && pItem->ItemCount != 0)	// -1 : 무제한		1~5 : 수량 카운트다운(보부상에서만 쓰임)
				return pItem;
		}

		return NULL;
	}

	//SW070626 보부상NPC
	void MakeSellingList(cPtrList* pList, DWORD tabNum)
	{
		POS pos = m_DealItemList.GetFirstPos();
		while(DealerItem* pItem = m_DealItemList.GetNextPos(pos) )
		{
			if(pItem->Tab == tabNum)
			{
				DealerItem* pTmp = new DealerItem;
				*pTmp = *pItem;
				pList->AddTail(pTmp);
			}
		}
	}

	cLinkedList<DealerItem> m_DealItemList;
};

class CItemManager  
{
	CYHHashTable<DealerData>	m_DealerTable;
	DWORD m_Key; //조합, 강화, 등급업시 로그 남길 때 같은 조합이었다는 것을 표시하기 위해.

	DWORD MakeNewKey();
	

#ifdef _HK_LOCAL_
	int	m_nItemMixBal[99][9];
#endif

public:
	CItemManager();
	virtual ~CItemManager();
	GETINSTANCE(CItemManager);
	void LoadDealerItem();
	DealerData * GetDealer(WORD wDealerIdx);
	int DivideItem( CPlayer* pPlayer, WORD ItemIdx,  POSTYPE FromPos, POSTYPE ToPos, DURTYPE FromDur, DURTYPE ToDur);
	int CombineItem( CPlayer* pPlayer, WORD ItemIdx, POSTYPE FromPos, POSTYPE ToPos, DURTYPE & FromDur, DURTYPE & ToDur);
	int MoveItem( CPlayer * pPlayer, WORD FromItemIdx, POSTYPE FromPos, WORD ToItemIdx, POSTYPE& ToPos);
	int DiscardItem(CPlayer * pPlayer, POSTYPE whatPos, WORD whatItemIdx, DURTYPE whatItemNum);
	
	void SendErrorMsg( CPlayer * pPlayer, MSG_ITEM_ERROR * msg, int msgSize, int ECode);
	void SendAckMsg( CPlayer * pPlayer, MSGBASE * msg, int msgSize);
	void SendGuildErrorMsg( CPlayer * pPlayer, BYTE Protocol, int ECode );

	//	int ObtainItem(CPlayer * pPlayer, ITEMOBTAINARRAYINFO * pArrayInf, WORD whatItemIdx, WORD whatItemNum);
	int ObtainItemEx(CPlayer * pPlayer, ITEMOBTAINARRAYINFO * pArrayInfo, WORD whatItemIdx, WORD whatItemNum, WORD * EmptyCellPos, WORD EmptyCellNum, WORD ArrayInfoUnitNum, WORD bSeal=0);
	int ObtainRareItem(CPlayer* pPlayer, ITEMOBTAINARRAYINFO* pArrayInfo, WORD whatItemIdx, WORD whatItemNum, WORD * EmptyCellPos, WORD ArrayInfoUnitNum, ITEM_RARE_OPTION_INFO* pRareOptionInfo );

	int SellItem( CPlayer* pPlayer, POSTYPE whatPos, WORD wSellItemIdx, DURTYPE sellItemDur, DWORD DealerIdx );
	int BuyItem( CPlayer* pPlayer, WORD buyItemIdx, WORD buyItemNum, WORD DealerIdx );
	
	// 필드보스 - 05.12 이영준
	// 몬스터가 동일 아이템을 여러게 드랍할수 있도록 변경
	void MonsterObtainItem(CPlayer * pPlayer, WORD obtainItemIdx, DWORD dwFurnisherIdx, WORD ItemNum = 1);

	int CheatObtainItem( CPlayer* pPlayer, WORD obtainItemIdx, WORD ItemNum );
	//SW051011 Cheat Rare
	int CheatObtainRareItem(CPlayer* pPlayer, WORD obtainItemIdx, WORD ItemNum, ITEM_OPTION_INFO* pOptionInfo);

	static void ObtainItemDBResult(CPlayer * pPlayer, WORD ArrayID);
	static void BuyItemDBResult(CPlayer * pPlayer, WORD ArrayID );	
	static void DivideItemDBResult(CPlayer* pPlayer, WORD ArrayID);
	//SW050920 Rare
	static void RareItemDBResult(CPlayer* pPlayer, WORD ArrayID, ITEM_RARE_OPTION_INFO* pRareOptionInfo);
	// magi82 - Titan(070427)
	static void TitanUpgradeItemDBResult(CPlayer* pPlayer, WORD ArrayID);

	void NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg );
	void NetworkMsgParseExt( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg );
	void LoadMixList();
	void LoadMixResultItemInfo(CMHFile * fp, ITEM_MIX_RES * pResInfo);
	void LoadReinforceItemInfo();

	void LoadItemList();
	void ReloadItemList();
	void SetItemInfo(WORD ItemIdx,ITEM_INFO* pInfo,CMHFile* pFile);
	ITEM_INFO * GetItemInfo( WORD wItemIdx );
	ITEM_MIX_INFO * GetMixItemInfo(WORD wItemIdx);
	ITEM_REINFORCE_INFO * GetReinforceItemInfo(WORD wItemIdx);
	

	int UseItem( CPlayer* pPlayer, WORD TargetPos, WORD wItemIdx );
	//
	int UpgradeItem(CPlayer* pPlayer, WORD & TargetItemIdx, POSTYPE TargetPos, WORD MatItemIdx, POSTYPE MatItemPos);
	int MixItem(CPlayer* pPlayer, WORD wBasicItemIdx, POSTYPE BasicItemPos, POSTYPE ResultIdx, MATERIAL_ARRAY * pMaterialArray, WORD wMaterialNum, WORD ShopItemIdx, WORD ShopItemPos, BOOL bTitan = FALSE);	// magi82 - Titan(070118)
	int ReinforceItem(CPlayer* pPlayer, WORD wTargetItemIdx, POSTYPE TargetPos, ITEM_JEWEL_POS_EX * pJewelInfo, WORD wJewelNum);
	void ReinforceItemDBResult(CPlayer * pPlayer, WORD wItemIdx, POSTYPE TargetPos, ITEM_OPTION_INFO * pOptionInfo);
	//SW050920 Rare
//	void RareItemDBResult(CPlayer* pPlayer, WORD wItemIdx, POSTYPE TargetPos, ITEM_RARE_OPTION_INFO* pRareInfo);
//	void RareItemDBResult(CPlayer* pPlayer, WORD ArrayID, ITEM_RARE_OPTION_INFO* pRareOptionInfo);

	BOOL EnoughDissolveSpace(CPlayer * pPlayer, WORD wOptionIdx, LEVELTYPE ItemLevel, WORD wAbilityGrade, MATERIAL_ARRAY * pJewelArray, WORD & wJewelNum);
	BOOL EnoughDissolveInvSpace(CPlayer * pPlayer, WORD wJewelNum);
	
	BOOL EnoughMixMaterial(WORD needItemIdx, WORD needItemNum, MATERIAL_ARRAY * pMaterialArray, WORD wMaterialNum);

	ITEMOBTAINARRAYINFO *	Alloc(CPlayer * pPlayer, BYTE c, BYTE p, DWORD dwObjectID, DWORD dwFurnisherIdx, WORD wType, WORD ObtainNum, DBResult CallBackFunc, DBResultEx CallBackFuncEx = NULL);
	void Free(CPlayer * pPlayer, ITEMOBTAINARRAYINFO * info);
	BOOL IsDupItem( WORD wItemIdx );
	//SW050920 Rare
	BOOL IsRareOptionItem( WORD wItemIdx, DWORD dwRareDBIdx );
	BOOL IsOptionItem( WORD wItemIdx, DURTYPE wDurability );
	BOOL IsPetSummonItem( WORD wItemIdx );
	BOOL IsTitanCallItem( WORD wItemIdx );
	BOOL IsTitanEquipItem( WORD wItemIdx );
	
	BOOL ItemMoveUpdateToDBbyTable(CPlayer* pPlayer, DWORD dwfromDBIdx, WORD dwfromIconIdx,  POSTYPE frompos, DWORD dwtoDBIdx, POSTYPE topos);
	void ItemUpdatebyTable(CPlayer* pPlayer, DWORD dwDBIdx, WORD wIconIdx, DURTYPE FromDur, POSTYPE Position, POSTYPE QuickPosition);
	const ITEMBASE * GetItemInfoAbsIn(CPlayer* pPlayer, POSTYPE Pos);

	void ItemMoveLog(POSTYPE FromPos, POSTYPE ToPos, CPlayer* pPlayer, ITEMBASE* pItem);
	void AddGuildItemOption(POSTYPE FromPos, POSTYPE ToPos, CPlayer* pPlayer);
	void RemoveGuildItemOption(POSTYPE ToPos, const ITEMBASE* pItem, CPlayer* pPlayer);

	unsigned int GetTotalEmptySlotNum(CItemSlot* pSlot); //060612 Add by wonju
	WORD GetCanBuyNumInSpace(CPlayer * pPlayer, CItemSlot * pSlot, WORD whatItemIdx, DURTYPE whatItemNum, WORD * EmptyCellPos, WORD & EmptyCellNum);

private:
	WORD GetCanBuyNumInMoney(CPlayer * pPlayer, WORD butNum, MONEYTYPE Price);
	BOOL CanMovetoGuildWare(POSTYPE FromPos, POSTYPE ToPos, CPlayer* pPlayer);
	BOOL CanbeMoved(WORD wIconIdx,POSTYPE pos, CPlayer* pPlayer);
	BOOL CanEquip(ITEM_INFO* pInfo, CPlayer* pPlayer);
	void ClearQuickPos(ITEMBASE* pItem, POSTYPE ToPos);

	CYHHashTable<ITEM_INFO> m_ItemInfoList;
	CYHHashTable<ITEM_MIX_INFO> m_MixItemInfoList;
	CYHHashTable<HILEVEL_ITEM_MIX_RATE_INFO> m_HiLevelItemMixRateInfoList;
	CYHHashTable<ITEM_REINFORCE_INFO> m_ReinforceItemInfoList;
	CYHHashTable<SET_ITEM_OPTION>	m_SetItemOptionList; ///// 2007. 6. 8. CBH - 세트아이탬 관련 리스트 추가
	CMemoryPoolTempl<ITEMOBTAINARRAYINFO> m_ItemArrayPool;
	CIndexGenerator		m_ArrayIndexCreator;
	//
	CYHHashTable<AVATARITEM>		m_AvatarEquipTable;

	/////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 은신/혜안
	// 은신시 사용못하는 아이템
	CYHHashTable<ITEM_INFO> m_UnfitItemInfoListForHide;
	/////////////////////////////////////////////


public:
	// jsd quest
	int		ObtainItemFromQuest( CPlayer* pPlayer, WORD wItemKind, DWORD dwItemNum );
	WORD	GetWeaponKind( WORD wWeapon );
	void	GetItemKindType( WORD wItemIdx, WORD* wKind, WORD* wType );

	int		ObtainItemFromChangeItem( CPlayer* pPlayer, WORD wItemKind, WORD wItemNum );
	BOOL	CheckHackNpc( CPlayer* pPlayer, WORD wNpcIdx, WORD wParam=0 );	
	// RaMa	
	int UseShopItem(CPlayer* pPlayer, SHOPITEMUSEBASE pInfo, SHOPITEMBASE* pShopItem);
	BOOL IsUseAbleShopItem( CPlayer* pPlayer, WORD ItemIdx, POSTYPE ItemPos );
	void ShopItemUseUpgrade(ITEM_INFO* pShopItemInfo, CPlayer* pPlayer, POSTYPE ItemPos, WORD wItemIdx);
	int ReinforceItemWithShopItem(CPlayer* pPlayer, WORD wTargetItemIdx, POSTYPE TargetPos, WORD wShopItemIdx, POSTYPE ShopItemPos, ITEM_JEWEL_POS_EX * pJewelInfo, WORD wJewelNum);

	BOOL LoadAvatarEquipList();
	BOOL ItemUnsealing(CPlayer* pPlayer, POSTYPE absPos);
	
	int CheatObtainShopItem( CPlayer* pPlayer, WORD obtainItemIdx, DWORD ItemNum );
	
	BOOL	CheckHackItemMove( CPlayer* pPlayer, CItemSlot* pFromSlot, CItemSlot* pToSlot );

	/////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 은신/혜안
	// 은신시 사용못하는 아이템
	ITEM_INFO* IsUnfitItemInfoListForHide(WORD idx) { return m_UnfitItemInfoListForHide.GetData(idx); }
	/////////////////////////////////////////////

	//SW070308 아이템부가옵션들 정리
	void	SetItemOptionsInfoMsg(CPlayer* pItemOwner, ITEMBASE* pItemBase, MSG_LINKITEMOPTIONS* rtMsg);

	///// 2007. 6. 8. CBH - 세트아이탬 관련 함수 추가 ///////////////////////////
	BOOL LoadSetItemOption();
	SET_ITEM_OPTION* GetSetItemOption(WORD wSetItemKind, WORD wSetValue);
	void RemoveSetItemOption(WORD wIndex, SET_ITEM_OPTION* pSetItemOptionOut = NULL);
	CYHHashTable<SET_ITEM_OPTION>* GetSetItemOptionList();
	/////////////////////////////////////////////////////////////////////////////

	BOOL	CheckDemandItem(CPlayer* pPlayer, MSG_ITEM_BUY_SYN* pmsg);
	BOOL	PayForDeal(CPlayer* pPlayer, DWORD DealerIdx, DWORD dwPayItemIdx, POSTYPE PayItemPos = 99, DWORD dwPayItemNum = 1);

	BOOL CheckWeaponToShopItem(CPlayer* pPlayer, WORD wItemIndex); //2007. 10. 15. CBH - 무기 아바타 아이탬 장착 관련 함수 추가

	// 이벤트를 통해서 
	WORD ObtainItemFromGameEvent( CPlayer* pPlayer, DWORD dwItemIdx, WORD wItemNum );
};


#endif // !defined(AFX_ITEMMANAGER_H__B48BE6A4_2998_4459_B958_995D81EAAF1E__INCLUDED_)
