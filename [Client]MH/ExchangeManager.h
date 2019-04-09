#ifndef _EXCHANGEMANAGER_H
#define _EXCHANGEMANAGER_H

#include "IndexGenerator.h"


#define MAX_EXCHANGEITEM	10

#define EXCHANGEMGR USINGTON(CExchangeManager)

class cIcon;
class CItem;
class CExchangeItem;
class CPlayer;



enum eEXCHANGE_ERROR
{
	eEE_OK					= 0,
	eEE_USERCANCEL,
	eEE_USERLOGOUT,
	eEE_USERDIE,		//»ó´ë°¡ Á×´Ù
	eEE_DIE,			//³»°¡Á×´Ù
	eEE_NOTENOUGHMONEY,
	eEE_NOTENOUGHSPACE,
	eEE_MAXMONEY,	
	eEE_ERROR,
};


class CExchangeManager
{

protected:

	int					m_nLinkedItemNum;
	CIndexGenerator		m_IconIndexCreator;
	
	BOOL				m_IsExchanging;
//	int					m_nExchangeState;

	BOOL				m_bLockSyncing;
	BOOL				m_bExchangeSyncing;
	BOOL				m_bInventoryActive;

public:

	CExchangeManager();
	virtual ~CExchangeManager();

	void Init();
	void ExchangeStart();
	void ExchangeEnd();
	BOOL LinkItem( CItem* pItem );
	void UnLinkItem( int nTypeIndex, cIcon* pItem );
	int GetLinkedItemNum() { return m_nLinkedItemNum; }
	
//±³È¯½ÅÃ»
	BOOL ApplyExchange();
//±³È¯½ÅÃ»Ãë¼Ò
	void CancelApply();
//±³È¯Çã¶ô
	BOOL CanAcceptExchange( CPlayer* pAccepter );
	void AcceptExchange( BOOL bAccept );

	void Lock( BOOL bLock );
	void Exchange();

//parsing
	void NetworkMsgParse( BYTE Protocol, void* pMsg );
	
	BOOL IsLockSyncing()		{ return m_bLockSyncing; }
	BOOL IsExchangeSyncing()	{ return m_bExchangeSyncing; }
	
	BOOL IsExchanging()			{ return m_IsExchanging; }

};

EXTERNGLOBALTON(CExchangeManager)

#endif