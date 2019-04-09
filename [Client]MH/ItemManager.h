// ItemManager.h: interface for the CItemManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMMANAGER_H__60209143_2AB6_46C5_A79E_4307FAFE0986__INCLUDED_)
#define AFX_ITEMMANAGER_H__60209143_2AB6_46C5_A79E_4307FAFE0986__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Item.h"
#include "ItemShow.h"
#include "ItemGround.h"
#include "ItemCalc.h"
#include "IndexGenerator.h"
#include "MugongBase.h"
#include "Player.h"

class CDealDialog;
class CExchangeDialog;
class CMixDialog;
class CRecvExchangeDialog;
class ITEM_OPTION_INFO;

class cIcon;
class cImage;

#define ITEMMGR USINGTON(CItemManager)

#define PET_DEFAULT_GRADE	1

//---for item tooltip
struct TOOLTIP_TEXT
{
	TOOLTIP_TEXT() { strToolTip = NULL; pNext = NULL; }
	char*			strToolTip;
	TOOLTIP_TEXT*	pNext;
};

enum eRemainOptionKind{eNoneOption = 0, eRemainReinforceOption = 1, eRemainRareOption = 2, eRemainAllOption = 3, };

class CItemManager  
{
	CItemCalc m_ItemCalc;

	CYHHashTable<CItem> m_ItemHash;

	CYHHashTable<ITEM_INFO> m_ItemInfoList;
	/////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 은신/혜안
	// 은신시 사용못하는 아이템
	CYHHashTable<ITEM_INFO> m_UnfitItemInfoListForHide;
	/////////////////////////////////////////////

	CYHHashTable<TOOLTIP_TEXT>		m_ItemToolTipTextList;
	//SW050920 Rare
	CYHHashTable<ITEM_RARE_OPTION_INFO>	m_ItemRareOptionList;
	CYHHashTable<ITEM_OPTION_INFO>	m_ItemOptionList;
	CYHHashTable<SHOPITEMBASE>		m_UsedItemList;
	CYHHashTable<sRareItemInfo>		m_RareItemInfoTable;
	CYHHashTable<SET_ITEM_OPTION>	m_SetItemOptionList; ///// 2007. 6. 8. CBH - 세트아이탬 관련 리스트 추가

	CIndexGenerator m_IconIndexCreator;
	BOOL			m_bAddPrice;
	ITEMBASE		m_TempDeleteItem;

public:
//	//MAKESINGLETON(CItemManager)

	CItemManager();
	virtual ~CItemManager();

	void InitItemRareOption(ITEM_RARE_OPTION_INFO* pInfo, WORD num);
	void AddItemRareOption(ITEM_RARE_OPTION_INFO* pInfo);
	ITEM_RARE_OPTION_INFO* GetItemRareOption(DWORD dwRareOptionIdx);
	void RemoveItemRareOption(DWORD dwRareOptionIdx, ITEM_RARE_OPTION_INFO* pItemRareOptionInfoOut = NULL);

	///// 2007. 6. 8. CBH - 세트아이탬 관련 함수 추가 ///////////////////////////
	BOOL LoadSetItemOption();
	SET_ITEM_OPTION* GetSetItemOption(WORD wSetItemKind, WORD wSetValue);	
	SET_ITEM_OPTION* GetSetItemOption(WORD wSetItemKind);
    void RemoveSetItemOption(WORD wIndex, SET_ITEM_OPTION* pSetItemOptionOut = NULL);

	void AddSetItemToolTip( cIcon* pIcon, ITEM_INFO* pItemInfo );
	DWORD GetSetItemEquipValue(ITEM_INFO* pItemInfo);
	void SetSetItemToolTip( cIcon* pIcon, ITEM_INFO* pItemInfo, BOOL bSetItemEnable, WORD wSetValue);
	CYHHashTable<SET_ITEM_OPTION>* GetSetItemOptionList();
	void SetItemEffectProcess(CPlayer* pPlayer);
	/////////////////////////////////////////////////////////////////////////////

	void AddUniqueItemToolTip(cIcon* pIcon, ITEM_INFO* pItemInfo );	// magi82 - UniqueItem(070628)

	void InitItemOption(ITEM_OPTION_INFO * pInfo, WORD num);
	void AddItemOption(ITEM_OPTION_INFO * pInfo);
	ITEM_OPTION_INFO * GetItemOption(DWORD dwOptionIdx);
	void RemoveItemOption(DWORD dwOptionIdx, ITEM_OPTION_INFO * pItemOptionInfoOut = NULL );

	void Release();

	CItem* MakeNewItem(ITEMBASE* pBaseInfo, char* strKind);
	
	void ItemDelete(CItem* pItem);
	BOOL FakeDeleteItem(CItem* pItem);
	void SendDeleteItem();
	
	cImage* GetIconImage(WORD ItemIdx, cImage * pImageGet);
	CItem* GetItem(DWORD dwDBIdx);
	void SetToolTipIcon(cIcon * pIcon, ITEM_OPTION_INFO * pOptionInfo=NULL, ITEM_RARE_OPTION_INFO* pRareOptionInfo = NULL, DWORD dwItemDBIdx = 0);

	void NetworkMsgParse(BYTE Protocol,void* pMsg);
	void NetworkMsgParseExt(BYTE Protocol,void* pMsg);

	void MoveItem( MSG_ITEM_MOVE_ACK * pMsg );
	void DivideItem(ITEMOBTAINARRAY * pMsg );
/*	
	void MoveItemToMunpa( MSG_ITEM_MUNPA_MOVE_SYN* pMsg );
	void MoveItemToPyoguk(MSG_ITEM_MUNPA_MOVE_SYN* pMsg);
*/
	
	void MoveItemToGuild( MSG_ITEM_MOVE_ACK* pMsg );
	void MoveItemToPyoguk( MSG_ITEM_MOVE_ACK* pMsg );
	void MoveItemToShop(MSG_ITEM_MOVE_ACK* pMsg);
	void MoveItemToShopInven(MSG_ITEM_MOVE_ACK* pMsg);
	void MoveItemToPetInven(MSG_ITEM_MOVE_ACK* pMsg);
	void MoveItemToTitanInven(MSG_ITEM_MOVE_ACK* pMsg);	// magi82 - Titan(070207)
	void MoveItemToTitanShopInven(MSG_ITEM_MOVE_ACK* pMsg);	// magi82 - Titan(070228)
		
	void CombineItem( MSG_ITEM_COMBINE_ACK * pMsg );
//	void DivideItem( MSG_ITEM_DIVIDE_ACK * pMsg );
	//SW051012 수정
	void DeleteItem( POSTYPE absPos, CItem** ppItemOut, ITEM_OPTION_INFO * pItemOptionInfoOut=NULL , ITEM_RARE_OPTION_INFO* pItemRareOptionInfoOut = NULL, WORD RemainOptionFlag = eNoneOption );
	BOOL AddItem(CItem* pItem);
	
//	void GetYoungyakItemToolTip(char * tooltip, int lens, ITEM_INFO * pInfo);
//	void GetMugongItemToolTip(char * tooltip, int lens, ITEM_INFO * pInfo);
//	void GetEquipItemToolTip(char * tooltip, int lens, ITEM_INFO * pInfo);
//	void GetExtraItemToolTip(char * tooltip, int lens, ITEM_INFO * pInfo);

	void SetYoungyakItemToolTip( cIcon * pIcon, ITEM_INFO * pInfo);
	void SetMugongItemToolTip( cIcon * pIcon, ITEM_INFO * pInfo);
	void SetEquipItemToolTip( cIcon * pIcon, ITEM_INFO * pInfo, ITEM_OPTION_INFO * pOptionInfo=NULL, ITEM_RARE_OPTION_INFO* pRareOptionInfo=NULL);
	void SetExtraItemToolTip( cIcon * pIcon, ITEM_INFO * pInfo);
	void SetItemToolTipForStage( cIcon * pIcon, ITEM_INFO * pInfo );
	void SetPetSummonItemToolTip( cIcon * pIcon, ITEM_INFO * pInfo, DWORD dwItemDBIdx = 0 );
	void SetTitanEquipItemToolTip( cIcon * pIcon, ITEM_INFO * pInfo );
	
	BYTE GetTableIdxForAbsPos(POSTYPE absPos);
	CItem* GetItemofTable(WORD TableIDX, POSTYPE absPos);

	// yunho StatsCalcManager 공유를 위해
	const ITEMBASE* GetItemInfoAbsIn(CHero* pHero,POSTYPE absPos);

	void DeleteItemofTable(WORD TableIdx, POSTYPE absPos, ITEM_OPTION_INFO * pItemOptionInfoOut=NULL, ITEM_RARE_OPTION_INFO* pItemRareOptionInfoOut = NULL);
	BOOL IsEqualTableIdxForPos(WORD TableIdx, POSTYPE absPos );

	void RefreshAllItem();
	void RefreshItem( CItem* pItem );
	void RefreshItemToolTip(DWORD ItemDBIdx);
	BOOL CanEquip(WORD wItemIdx);
	BOOL CanConvertToMugong(WORD wItemIdx,MUGONG_TYPE MugongType=MUGONGTYPE_MAX);

	void LoadItemList();
	void LoadItemToolTipList();
	ITEM_INFO * GetItemInfo(WORD wItemIdx);
	void SetPreItemData(sPRELOAD_INFO* pPreLoadInfo, int* Level, int Count);
	DWORD m_dwStateParam;

	//SW050920 Rare
	BOOL IsRareOptionItem( WORD wItemIdx, DWORD dwRareIdx );
	//BOOL CanGradeUp(CItem * pItem);
	BOOL IsDupItem( WORD wItemIdx );
	BOOL IsOptionItem( WORD wItemIdx, DURTYPE wDurability );
	BOOL IsPetSummonItem(WORD wItemIdx);
	BOOL IsTitanCallItem(WORD wItemIdx);
	BOOL IsTitanEquipItem(WORD wItemIdx);

	void SetPriceToItem( BOOL bAddPrice );
	void SetDisableDialog(BOOL val, BYTE TableIdx);

///
	TOOLTIP_TEXT* GetItemToolTipInfo( WORD wIdx );
	void AddItemDescriptionToolTip( cWindow* pWindow, WORD wIndex, DWORD dwTxtColor = TTTC_DESCRIPTION, int nSpaceLine = 1 );
	// 060908 KKR 봉인된 아이템 툴팁 뿌리기 
	void AddShopItemToolTip( cIcon* pIcon, ITEM_INFO* pItemInfo );	
	// 060911 KKR 샵 아이템 사용시간 뿌리기 
	void PrintShopItemUseTime( CItem* pItem, ITEM_INFO* pItemInfo );
	ITEM_INFO* FindItemInfoForName( char* strName );
	
//	BOOL CheckQPosForItemIdx( WORD wItemIdx );
	
	void ReLinkQuickPosition(CItem* pItem);	

	//
	void AddUsedItemInfo(SHOPITEMBASE* pInfo);
	void RemoveUsedItemInfo(WORD wIndex, DWORD dwDBIdx=0);
	SHOPITEMBASE* GetUsedItemInfo(DWORD Index);
	void Process();
	void DeleteShopItemInfo();
	void AddUsedAvatarItemToolTip( SHOPITEMBASE* pInfo );
	void AddUsedShopEquipItemToolTip( SHOPITEMBASE* pInfo );
	void AddUsedPetEquipItemToolTip( SHOPITEMBASE* pInfo );
	
	void ReviveOtherOK();
	void ReviveOtherSync();
	void ReviveOtherCancel();
	void ReviveOtherConfirm( BOOL bRevive );
	
	void RefreshStatsBuffIcon();
	void AddAvatarItemOptionTooltip( cIcon* pIcon, ITEM_INFO* pItemInfo );
	BOOL CheckItemStage( BYTE bItemStage );
	
	void ItemDropEffect( WORD wItemIdx );
	void MoneyDropEffect();

	//
	BOOL LoadRareItemInfo();
	BOOL IsRareItemAble( DWORD ItemIdx )
	{
		if( m_RareItemInfoTable.GetData(ItemIdx) )			return TRUE;
		return FALSE;
	}

	void SetIconColorFilter(DWORD dwDBIdx, DWORD clrRGBA);
	void SetIconColorFilter(cIcon* pIcon, DWORD clrRGBA);
//for GM...현재는..
#ifdef _GMTOOL_
	void SetItemIfoPositionHead()	{ m_ItemInfoList.SetPositionHead(); }
	ITEM_INFO* GetItemInfoData()	{ return m_ItemInfoList.GetData(); }
#endif

	// For Japan
#ifdef _JAPAN_LOCAL_
	BOOL	CheckItemAttr( WORD wItemAttr );
	BOOL	CheckItemAquireSkill( WORD wAcquireSkill1, WORD wAcquireSkill2 );
#endif
	/////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 은신/혜안
	// 은신시 사용못하는 아이템
	ITEM_INFO* IsUnfitItemInfoListForHide(WORD idx) { return m_UnfitItemInfoListForHide.GetData(idx); }
	/////////////////////////////////////////////

	//SW061207	풀인벤 경고
	enum eCheckBlankSlot{eCBS_Inven=1,eCBS_SInven=2,eCBS_All=3};
	void CheckInvenFullForAlert(int flg = eCBS_All);

	void GetItemOptionsAndToolTipFromInfoMsg(cIcon* pIcon,MSG_LINKITEMOPTIONS* pOptionsMsg);


	// debug용
	int	m_nItemUseCount;
};


EXTERNGLOBALTON(CItemManager)

#endif // !defined(AFX_ITEMMANAGER_H__60209143_2AB6_46C5_A79E_4307FAFE0986__INCLUDED_)
