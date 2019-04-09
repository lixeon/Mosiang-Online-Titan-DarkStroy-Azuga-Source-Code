// ItemDrop.h: interface for the CItemDrop class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMDROP_H__59770A47_500D_485D_89F1_9835D4500264__INCLUDED_)
#define AFX_ITEMDROP_H__59770A47_500D_485D_89F1_9835D4500264__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "Player.h"
//class CPlayer;

#define ITEMDROP_OBJ CItemDrop::GetInstance()
class MONSTEREX_LIST;

struct MONSTER_ITEM
{
#ifdef _DEBUG
	char Name[MAX_ITEMNAME_LENGTH+1];			// 아이템 이름
#endif
	WORD wItemIdx;							// 아이템 번호
	DWORD dwDropPercent;						// 드롭확률
};
/*
struct MONSTER_ITEM_DROP
{
	DWORD ItemDropIndex;
	int m_MonsterMaxPercent;
	int m_MonsterMaxVarPercent;	
	int m_MonsterDropTotalNum;
	MONSTER_ITEM m_MonsterDropItem[MAX_DROPITEM_NUM];
	MONSTER_ITEM m_MonsterDropItemVarPercent[MAX_DROPITEM_NUM];
};
*/

#define MAX_DROP_ITEM_PERCENT			10000
//#define MAX_MONSTER_DROP_ITEM_PERCENT	1000000

struct MONSTER_DROP_ITEM
{
	WORD			wItemDropIndex;
	WORD			wTotalNum;
	DWORD			dwCurTotalRate;
	DWORD			dwCurMonsterDropItemPercent[MAX_DROPITEM_NUM];
	MONSTER_ITEM	MonsterDropItemPercent[MAX_DROPITEM_NUM];
};

class CItemDrop  
{
	CYHHashTable<MONSTER_DROP_ITEM>	m_MonsterItemDropTable;

	BOOL LoadMonsterDropItemList();
	void ClearMonsterDropItemList();

	MONSTER_DROP_ITEM* GetMonsterItemDrop(WORD wItemDropIdx);

public:
	GETINSTANCE(CItemDrop);
 	
	CItemDrop();
	virtual ~CItemDrop();

//	void ReloadDropItem(MONSTEREX_LIST * pMonsterExList);
	int DropItemKind(MONSTEREX_LIST * pMonsterExList, CPlayer* pPlayer);

	void Init();
	MONEYTYPE MoneyItemNoItemPercentCalculator(MONSTEREX_LIST * pMonInfo, CPlayer* pPlayer, WORD MonsterKind);
	MONEYTYPE MonsterDropMoney(MONSTEREX_LIST * pMonsterExList);
	
	void MonsterDropItem(WORD MonsterKind, WORD wDropItemListIdx, CPlayer* pPlayer);
	BOOL ReloadMonsterDropItem(MONSTER_DROP_ITEM* pDropItem);
	//void ReloadDropItemPercent(WORD Kind);
	//void CalculItemPercent(WORD Kind, int HaveItem, int idx);
	//BOOL ZeroCheckPercent(WORD Kind);


	//void TestMonsterDropItem(WORD wDropItemListIdx);
};

#endif // !defined(AFX_ITEMDROP_H__59770A47_500D_485D_89F1_9835D4500264__INCLUDED_)
