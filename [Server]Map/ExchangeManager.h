#ifndef _EXCHANGEMANAGER_H
#define _EXCHANGEMANAGER_H


#define EXCHANGEMGR CExchangeManager::GetInstance()

class CExchangeRoom;

struct sEXCHANGECONTAINER
{
	sEXCHANGECONTAINER()
	{
		pRoom		= NULL;
		nMyIndex	= -1;
		pOpPlayer	= NULL;
	}

	CExchangeRoom*	pRoom;
	int				nMyIndex;
	CPlayer*		pOpPlayer;	//»ó´ë ÇÃ·¹ÀÌ¾î
};

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
	eEE_NOTMATCHITEM,
	eEE_ERROR,
};

class CExchangeManager
{

protected:

	CMemoryPoolTempl<CExchangeRoom>*		m_mpExchangeRoom;
	cPtrList m_PtrList;


public:

	MAKESINGLETON( CExchangeManager );

	CExchangeManager();	
	~CExchangeManager();

	BOOL CanApplyExchange( CPlayer* pPlayer, CPlayer* pOpPlayer );
	
	CExchangeRoom* CreateExchangeRoom( CPlayer* p1, CPlayer* p2 );
	void DestroyExchangeRoom( CExchangeRoom* pRoom );
	
	void UserLogOut( CPlayer* pPlayer );
	void CancelExchange( CPlayer* pPlayer );

	CExchangeRoom* GetRoomInfo( DWORD dwPlayer1ID, CPlayer** ppPlayer1, CPlayer** ppPlayer2 );

	void NetworkMsgParse( BYTE Protocol, void* pMsg );
};

#endif
