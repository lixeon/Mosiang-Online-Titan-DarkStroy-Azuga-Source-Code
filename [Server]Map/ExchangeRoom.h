#ifndef _EXCHANGEROOM_H
#define _EXCHANGEROOM_H

#define MAX_EXCHANGEITEM	10


struct sEXCHANGEDATA
{
	sEXCHANGEDATA()
	{
		pPlayer			= NULL;
		bLock			= FALSE;
		bExchange		= FALSE;
		nAddItemNum		= 0;
		dwMoney			= 0;
		ZeroMemory( ItemInfo, sizeof(ITEMBASE)*MAX_EXCHANGEITEM );
	}

	CPlayer*	pPlayer;
	int			nAddItemNum;
	ITEMBASE	ItemInfo[MAX_EXCHANGEITEM];
	MONEYTYPE	dwMoney;
	BOOL		bLock;
	BOOL		bExchange;
};


enum eEXCHANGESTATE
{
	EXCS_WAITING,
	EXCS_DOING,
};


class CExchangeRoom
{

protected:

	sEXCHANGEDATA	m_ExchangeData[2];
	int				m_nExchangeState;

public:

	CExchangeRoom();
	~CExchangeRoom();

	void Init( CPlayer* pPlayer1, CPlayer* pPlayer2 );
	void Exit();

	CPlayer* GetPlayer( int nPlayerIndex )	{ return m_ExchangeData[nPlayerIndex].pPlayer; }
	
	void Lock( int nIndex, BOOL bLock );
	BOOL IsLocked( int nPlayerIndex )		{ return m_ExchangeData[nPlayerIndex].bLock; }
	BOOL IsAllLock();

	void SetExchange( int nIndex );
	BOOL IsAllExchange();

	void SetExchangeState( int nState ) { m_nExchangeState = nState; }
	int GetExchangeState() { return m_nExchangeState; }
	
	BOOL AddItem( int nPlayerIndex, ITEMBASE* pItemInfo );
	BOOL DelItem( int nPlayerIndex, POSTYPE pos );

	DWORD InputMoney( int nPlayerIndex, DWORD dwMoney );
	
	int DoExchange();

};



#endif