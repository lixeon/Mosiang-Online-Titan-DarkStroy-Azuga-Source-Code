// ShopItemManager.h: interface for the CShopItemManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHOPITEMMANAGER_H__28EE3E98_5701_4E5B_B6E8_242524221154__INCLUDED_)
#define AFX_SHOPITEMMANAGER_H__28EE3E98_5701_4E5B_B6E8_242524221154__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPlayer;
struct ITEM_INFO;


class CShopItemManager  
{
	CYHHashTable<SHOPITEMWITHTIME>			m_UsingItemTable;
	CMemoryPoolTempl<SHOPITEMWITHTIME>*		m_UsingItemPool;
	CYHHashTable<MOVEDATA>					m_MovePointTable;
	CMemoryPoolTempl<MOVEDATA>*				m_MovePointPool;

	CPlayer*	m_pPlayer;
	DWORD		m_Checktime;
	DWORD		m_Updatetime;

	//
	DWORD		m_DupIncantation;
	DWORD		m_DupCharm;
	DWORD		m_DupHerb;
	DWORD		m_DupSundries;
	DWORD		m_DupPetEquip;

	//
	DWORD		m_ProtectItemIdx;

private:	
	//2007. 12. 13. CBH - m_UsingItemTable.Add와 m_UsingItemPool.Alloc과 같이 써야한다.
	//기존에 Alloc을 먼저 할당받고 처리하면서 Table에 Add를 안하는 경우가 발생할 수 있다.
	//이런 현상을 방지하기 위해 같이 묶어놨음.
	void AddUsingShopItem(SHOPITEMWITHTIME* pShopItem, WORD dwItemIndex);

public:
	CShopItemManager();
	virtual ~CShopItemManager();

	void Init(CPlayer* pPlayer);
	void Release();
	void CheckEndTime();
	int UseShopItem( ITEMBASE* pItemBase, SHOPITEMUSEBASE UseBaseInfo, SHOPITEMBASE* pShopItem );
	BOOL UsedShopItem( ITEMBASE* pItemBase, DWORD Param, stTIME BeginTime, DWORD RemainTime );
	BOOL CalcShopItemOption( DWORD wIdx, BOOL bAdd, DWORD Param=0 );

	//
	void DeleteUsingShopItem( DWORD ItemIdx, BOOL bDelete=FALSE );
	void DeleteUsingShopItemInfo( DWORD ItemIdx );
	void SendMsgDwordToPlayer( DWORD Msg, DWORD Param );
	void SendShopItemUseInfo();
	void UpdateLogoutToDB();

	//
	BOOL AddMovePoint( MOVEDATA* pData );
	BOOL DeleteMovePoint( DWORD DBIdx );
	BOOL ReNameMovePoint( DWORD DBIdx, char* newName );
	void ReleseMovePoint();

	//
	SHOPITEMWITHTIME* GetUsingItemInfo( DWORD ItemIdx );

	DWORD GetSavedMPNum()		{	return m_MovePointTable.GetDataNum();	}

	//
	BOOL PutOnAvatarItem( WORD ItemIdx, WORD ItemPos, BOOL bCalcStats=TRUE );
	BOOL TakeOffAvatarItem( WORD ItemIdx, WORD ItemPos, BOOL bCalcStats=TRUE );
	void AddShopItemUse( SHOPITEMBASE* pShopItem );
	void DiscardAvatarItem( WORD ItemIdx, WORD ItemPos );
	void CheckAvatarEndtime();

	//
	void CalcAvatarOption( BOOL bCalcStats=TRUE );
	void CalcPlusTime( DWORD dwEventIdx, DWORD dwType );

	//
	void AddDupParam( ITEM_INFO* pItemInfo );
	void DeleteDupParam( ITEM_INFO* pItemInfo );
	BOOL IsDupAble( ITEM_INFO* pItemInfo );

	// Protect Item
	DWORD GetProtectItemIdx()					{	return m_ProtectItemIdx;	}
	void SetProtectItemIdx( DWORD ItemIdx )		{	m_ProtectItemIdx = ItemIdx;		}

	
	DWORD PutSkinSelectItem(MSG_DWORD2* pMsg);	// 2007. 12. 5. CBH - 선택한 스킨 아이탬 세트 인덱스 처리
	void DiscardSkinItem(DWORD dwItemIndex);	// 2007. 12. 11. CBH - 스킨아이탬 삭제 관련 처리
	void RemoveEquipSkin(DWORD dwSkinKind);			// 2008. 1. 21. CBH - 장착된 스킨 아이탬 삭제
};



#endif // !defined(AFX_SHOPITEMMANAGER_H__28EE3E98_5701_4E5B_B6E8_242524221154__INCLUDED_)
