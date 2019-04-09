#include "stdafx.h"

#include "purse.h"
#include "player.h"
#include "MapDBMsgParser.h"

CPurse::CPurse()
{
	m_bInit = FALSE;
}

CPurse::~CPurse()
{
}

BOOL CPurse::InitPurse( void* pOwner, MONEYTYPE money, MONEYTYPE max )
{
	/* DB¿¡¼­ ÇÑ¹ø °¡Á®¿Í ¼ÂÆÃÇÒ¶§ ¿Ü¿£ ÀÓÀÇ·Î ÀüÃ¼ °ªÀ» º¯°æÇÒ ¼ö ¾ø´Ù. 
	   ¿ÜºÎ¿¡¼­ ¿¬»êÇÏ´Â°ÍÀ» ¸·±âÀ§ÇØ¼­ Ã³¸®ÇÏ°Ô µÊ. */
	if( m_bInit ) 
	{
		ASSERTMSG(0, "ÃÊ±âÈ­¸¦ µÎ¹ø ÇÏ·Á ÇÕ´Ï´Ù.");
		return FALSE;
	}

//	if( money > max )
//	{// ÃÊ±âÈ­½Ã¿¡ Àß¸øµÈ ÀÔ·ÂÀ» ÇßÀ» °æ¿ì¿¡ ´ëÇØ¼­ CHECK
//		ASSERTMSG(0, "ÃÊ±âÈ­ ÀÔ·Â°ªÀÌ Àß¸øµÇ¾ú½À´Ï´Ù.(ÇöÀç±Ý¾× > Á¦ÇÑ±Ý¾×)");
//		return FALSE;
//	}

	if( money > max )
	{
		ASSERTMSG(0, "money > maxmoney" );
		money = max;
	}

	m_pOwner = pOwner;

	m_dwMoney = money;
	m_dwMaxMoney = max;
	m_bInit = TRUE;

	m_MsgMoney.Category = MP_ITEM;
	m_MsgMoney.Protocol = MP_ITEM_MONEY;

	return TRUE;
}

void CPurse::Release()
{
	m_pOwner = NULL;

	m_bInit = FALSE;
}

void CPurse::SetMoney( MONEYTYPE money, BYTE MsgFlag )
{
	m_dwMoney = money;
}

BOOL CPurse::SetMaxMoney( MONEYTYPE max )
{
	if( !m_bInit ) return FALSE;

	if( m_dwMoney > max )
	{
		ASSERTMSG(0, "Àß¸øµÈ Á¦ÇÑÄ¡·Î ¼³Á¤ÇÏ°í ÀÖ´Ù.(ÇöÀç±Ý¾× > Á¦ÇÑ±Ý¾×)");
		return FALSE;
	}
	
	m_dwMaxMoney = max;

	return TRUE;
}

MONEYTYPE CPurse::GetMaxMoney()
{
	return m_dwMaxMoney;
}

/* ½ÇÁ¦ º¯È­·® Return */
MONEYTYPE CPurse::Addition( MONEYTYPE money, BYTE MsgFlag )
{
	if( !m_bInit ) return 0;

	//2007. 8. 29 CBH - 금액 자료형 오버플로우 개선
	//if( m_dwMoney + money > m_dwMaxMoney )		// Á¦ÇÑÄ¡¿¡ ´ëÇÑ MAXÃ³¸® 
	if(  money > m_dwMaxMoney - m_dwMoney)
	{
		if( m_dwMaxMoney < m_dwMoney )
			return 0;

		money = m_dwMaxMoney - m_dwMoney;
	}

	//2007. 8. 29 CBH - 금액 자료형 오버플로우 개선
	//if( m_dwMoney + money < m_dwMoney )		// ÀÚ·áÇü¿¡ ´ëÇÑ MAXÃ³¸®
	if( money < m_dwMoney - m_dwMoney)		// ÀÚ·áÇü¿¡ ´ëÇÑ MAXÃ³¸®
		money = MAXMONEY - m_dwMoney;

	SetMoney(m_dwMoney + money, MsgFlag);

	return money;
}

/* ½ÇÁ¦ º¯È­·® Return */
MONEYTYPE CPurse::Subtraction( MONEYTYPE money, BYTE MsgFlag )
{
	if( !m_bInit ) return 0;

	if( !IsEnoughMoney(money) )
		return 0; // ½Çº¯È­·® ¾øÀ½.

	if( m_dwMoney - money > m_dwMoney ) // Ãß°¡ ÀÚ·áÇü¿¡ ´ëÇÑ ¿¡·¯ check
		return 0;

	SetMoney(m_dwMoney - money, MsgFlag);

	return money;
}

BOOL CPurse::IsAdditionEnough( MONEYTYPE AdditionMoney )
{
	//if( m_dwMoney + AdditionMoney < m_dwMoney ||
	//	m_dwMoney + AdditionMoney > m_dwMaxMoney )
	//2007. 8. 29 CBH - 금액 자료형 오버플로우 개선
	if( m_dwMoney + AdditionMoney < m_dwMoney ||
		AdditionMoney > m_dwMaxMoney - m_dwMoney)
		return FALSE;

	return TRUE;
}

// °¨»ê ±Ý¾×°úÀÇ ºñ±³
BOOL CPurse::IsEnoughMoney( MONEYTYPE SubtractionMoney )
{
	if( m_dwMoney >= SubtractionMoney )
		return TRUE;

	return FALSE;
}

void CPurse::RSetMoney( MONEYTYPE money, BYTE flag )
{
	m_dwMoney = money;
}


/////////////////////////////////////////////////////////////////
// °³ÀÎ µ·
/////////////////////////////////////////////////////////////////
void CInventoryPurse::SetMoney( MONEYTYPE money, BYTE MsgFlag )
{
	m_dwMoney = money;

	CPlayer* pPlayer = (CPlayer*)m_pOwner;
	if( !pPlayer ) return;

	pPlayer->MoneyUpdate( m_dwMoney );

	// DB update
	if( MsgFlag != MF_OBTAIN )	//¸ó½ºÅÍ¿¡°Ô ¾ò¾úÀ» °æ¿ì¸¦ Á¦¿ÜÇÏ°í.
		CharacterHeroInfoUpdate(pPlayer);

	// Client Msg;
	SendMoneyMsg(MsgFlag);
	
/*
	CPlayer* pPlayer = (CPlayer*)m_pOwner;
	if( !pPlayer ) return;

	// DB update
	if( MsgFlag != MF_OBTAIN )	//¸ó½ºÅÍ¿¡°Ô ¾ò¾úÀ» °æ¿ì¸¦ Á¦¿ÜÇÏ°í.
	{
		m_dwMoney = money;
		pPlayer->MoneyUpdate( m_dwMoney );	
		UpdateCharacterMoney( pPlayer->GetID(), money, MsgFlag );		
	}
	else
	{
		m_dwMoney = money;
		pPlayer->MoneyUpdate( m_dwMoney );
		SendMoneyMsg(MsgFlag);
	}
	*/
}

void CInventoryPurse::RSetMoney( MONEYTYPE money, BYTE flag )
{
	m_dwMoney = money;

	SendMoneyMsg( flag );
}

void CInventoryPurse::SendMoneyMsg(BYTE flag)
{
	CPlayer* pPlayer = (CPlayer*)m_pOwner;
	if( !pPlayer ) return;

	// Client¿¡°Ô º¸³¾ ¸Þ¼¼Áö.. (Ãß°¡ ÇÏ¼¼¿è!!)
	switch( flag )
	{
	case 64:
		{
			m_MsgMoney.dwObjectID = pPlayer->GetID();
			m_MsgMoney.dwTotalMoney = m_dwMoney;
			m_MsgMoney.bFlag = flag;

			pPlayer->SendMsg(&m_MsgMoney, sizeof(m_MsgMoney));
		}
		break;
	case MF_NOMAL:
		{
			m_MsgMoney.dwObjectID = pPlayer->GetID();
			m_MsgMoney.dwTotalMoney = m_dwMoney;
			m_MsgMoney.bFlag = flag;

			pPlayer->SendMsg(&m_MsgMoney, sizeof(m_MsgMoney));
		}
		break;
	case MF_OBTAIN:
		{
			m_MsgMoney.dwObjectID = pPlayer->GetID();
			m_MsgMoney.dwTotalMoney = m_dwMoney;
			m_MsgMoney.bFlag = flag;

			pPlayer->SendMsg(&m_MsgMoney, sizeof(m_MsgMoney));
		}
		break;
	case MF_LOST:
		{
			m_MsgMoney.dwObjectID = pPlayer->GetID();
			m_MsgMoney.dwTotalMoney = m_dwMoney;
			m_MsgMoney.bFlag = flag;

			pPlayer->SendMsg(&m_MsgMoney, sizeof(m_MsgMoney));

		}
		break;
	default:
		return;
	}
}


/////////////////////////////////////////////////////////////////
// Ã¢°í µ·
/////////////////////////////////////////////////////////////////
void CPyogukPurse::SetMoney( MONEYTYPE money, BYTE MsgFlag )
{
	m_dwMoney = money;
}


/////////////////////////////////////////////////////////////////
// ¹®ÆÄ µ·
/////////////////////////////////////////////////////////////////
void CMunpaWarePurse::SetMoney( MONEYTYPE money, BYTE MsgFlag )
{
	m_dwMoney = money;
	
	// db Update, Client Msg;
//	SendMoneyMsg(MsgFlag);
}

void CMunpaWarePurse::SendMoneyMsg(BYTE flag)
{
//	CMunpa* pMunpa = (CMunpa*)m_pOwner;
//	if( !pMunpa ) return;
}
