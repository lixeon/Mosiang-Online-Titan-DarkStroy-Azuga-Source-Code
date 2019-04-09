#ifndef __STREETSTALL_MANAGER__
#define __STREETSTALL_MANAGER__

 
//#define STREETSTALLMGR cStreetStallManager::GetInstance()
#define STREETSTALLMGR USINGTON(cStreetStallManager)


class cStreetStall;
class CPlayer;
struct sCELLINFO;

enum StreetStallFindState
{
	eFind_Sell,
	eFind_Buy,
	eFind_Max,
};

enum StreetStallPriceState
{
	ePrice_Min,
	ePrice_Max,
};

enum StreetStallDelayState
{
	eDelay_StallSearch,
	eDelay_ItemView,
	eDelay_Max,
};

#define STALL_SEARCH_DELAY_TIME	3000
#define	ITEM_VIEW_DELAY_TIME	1000

class cStreetStallManager 
{
private:
	BOOL CanBuyItem(CPlayer* pOwner, CPlayer *pGuest, sCELLINFO* pItemInfo, WORD* EmptyCellPos, STREETSTALL_BUYINFO* pBuyInfo, ITEMBASE* pStallItemBase, DWORD dwBuyNum );
	BOOL CanSellItem(CPlayer* pOwner, CPlayer *pGuest, sCELLINFO* pItemInfo, WORD* EmptyCellPos, STREETSTALL_BUYINFO* pBuyInfo, ITEMBASE* pStallItemBase, DWORD dwBuyNum, WORD& result );
	void AssertBuyErr(WORD Err);
	void AssertSellErr(WORD Err);
protected:
	CMemoryPoolTempl<cStreetStall>*		m_mpStreetStall;
	CYHHashTable<cStreetStall>			m_StallTable;

	// magi82(47)
	DWORD	m_dwSearchDelayTime[eDelay_Max];	// 노점상 검색 딜레이
	CMemoryPoolTempl<STALL_DEALITEM_INFO>*		m_pDealItemPool[eFind_Max];
	CYHHashTable<STALL_DEALITEM_INFO>				m_pDealItemTable[eFind_Max];
	CMemoryPoolTempl<STALL_DEALER_INFO>*		m_pDealerPool[eFind_Max];

public:
	cStreetStallManager();
	virtual ~cStreetStallManager();

	//MAKESINGLETON( cStreetStallManager );

	void StreetStallMode( CPlayer* pPlayer, WORD StallKind, char* title, BOOL bMode );

	cStreetStall* CreateStreetStall( CPlayer* pOwner, WORD StallKind, char* title );
	void DeleteStreetStall( CPlayer* pOwner );
	cStreetStall* FindStreetStall( CPlayer* pOwner );
	
//	BOOL GuestIn( CPlayer* pOwner, CPlayer* pGuest );
//	void GuestOut( DWORD OwnerId, CPlayer* pGuest );
	void UserLogOut( CPlayer* pPlayer );

	BOOL BuyItem( CPlayer* pOwner, cStreetStall* pStall, CPlayer* pGuest, STREETSTALL_BUYINFO* pBuyInfo );
	BOOL BuyDupItem( CPlayer* pOwner, cStreetStall* pStall, CPlayer* pGuest, STREETSTALL_BUYINFO* pBuyInfo );
	BOOL SellItem( CPlayer* pOwner, cStreetStall* pStall, CPlayer* pGuest, STREETSTALL_BUYINFO* pSellInfo );
	BOOL SellDupItem( CPlayer* pOwner, cStreetStall* pStall, CPlayer* pGuest, STREETSTALL_BUYINFO* pSellInfo );

	void CreateDupItem( DWORD dwObjectId, ITEMBASE* pItemBase, DWORD FromChrID );

	BOOL IsExist( cStreetStall* pStall );

	void NetworkMsgParse( BYTE Protocol, void* pMsg );
	void SendNackMsg(CPlayer* pPlayer, BYTE Protocol);

	// magi82(47)
	void	InitStallData();		// 노점상 검색 메모리풀, 테이블 초기화
	void	ReleaseStallData();		// 노점상 검색 메모리풀, 테이블 해제
	void	ResetStallData( DWORD dwOwnerIdx, StreetStallFindState State );	// 노점상 검색 정보 리셋
	void	InsertStallData(  DWORD dwOwnerIdx, DWORD dwItemIdx, StreetStallFindState State, DWORD dwMoney );	// 노점상 검색에 정보 추가
	void	DeleteStallData( DWORD dwOwnerIdx, DWORD dwItemIdx, StreetStallFindState State );	// 노점상 검색에 정보 삭제
	DWORD	GetPrice( DWORD dwOwnerIdx, DWORD dwItemIdx, StreetStallPriceState State );	// 해당 노점상의 최고가, 최저가 리턴
	BOOL	CheckDelayTime( DWORD dwTime, StreetStallDelayState State );		// 노점상 검색관련 딜레이 체크
};
EXTERNGLOBALTON(cStreetStallManager);

#endif // __STREETSTALL_MANAGER__
