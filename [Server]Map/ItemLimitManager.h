#pragma once


#define ITEMLIMITMGR	USINGTON(CItemLimitManager)


struct ITEMLIMITINFO
{
	DWORD	dwItmeIdx;
	int		nItemLimitCount;
	int		nItemCurrentCount;

	ITEMLIMITINFO() : dwItmeIdx(0), nItemLimitCount(0), nItemCurrentCount(0)	{}
};


class CItemLimitManager
{
protected:
	CYHHashTable<ITEMLIMITINFO>	m_ItemLimitTable;

public:
	CItemLimitManager(void);
	~CItemLimitManager(void);

	ITEMLIMITINFO* GetItemLimitInfo( DWORD dwItemIdx )	{ return m_ItemLimitTable.GetData(dwItemIdx); }

	void Init();
	void Release();

	BOOL LoadItemLimitInfo();
	void SetItemLimitInfoFromDB( DWORD dwItemIdx, int nItemLimitCount, int nItemCurrentCount );
	int CheckItemLimitInfo( DWORD dwItemIdx );	
	void AddCurrentItemCount( DWORD dwItemIdx, int nItemCount );

	void NetworkMsgParser( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg );

	BOOL SetItemLimitCount( DWORD dwItemIdx, int nItemLimitCount );
};

EXTERNGLOBALTON(CItemLimitManager)
