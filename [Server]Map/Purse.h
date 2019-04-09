#ifndef __CLASS_PURSE__
#define __CLASS_PURSE__


class CPurse
{
protected:
	void*			m_pOwner;

	MONEYTYPE		m_dwMoney;
	MONEYTYPE		m_dwMaxMoney;				// Á¦ÇÑ°ª

	BOOL			m_bInit;

	MSG_MONEY		m_MsgMoney;

public:
	CPurse();
	virtual ~CPurse();

	BOOL InitPurse( void* pOwner, MONEYTYPE money, MONEYTYPE max );
	void Release();

	//////////////////////////////////////////////////
	//// ¿¬»ê ÇÔ¼ö( ½ÇÁ¦ º¯È­·®À» returnÇÑ´Ù. )
	MONEYTYPE Addition( MONEYTYPE money, BYTE MsgFlag = 0 );
	MONEYTYPE Subtraction( MONEYTYPE money, BYTE MsgFlag = 0 );

	virtual void RSetMoney( MONEYTYPE money, BYTE flag );
	//////////////////////////////////////////////////

	BOOL IsAdditionEnough( MONEYTYPE AdditionMoney );
	BOOL IsEnoughMoney( MONEYTYPE SubtractionMoney );

	MONEYTYPE GetPurseCurMoney() { return m_dwMoney; }	
	BOOL SetMaxMoney( MONEYTYPE max );
	MONEYTYPE GetMaxMoney();

	// RaMa
	void SetZeroMoney()			{	m_dwMoney = 0;		}

protected:
	virtual void SetMoney( MONEYTYPE money, BYTE MsgFlag );
	virtual void SendMoneyMsg(BYTE flag){}
};


/* Inventory µ· */
class CInventoryPurse : public CPurse
{
public:
	virtual void RSetMoney( MONEYTYPE money, BYTE flag );
protected:
	virtual void SetMoney( MONEYTYPE money, BYTE MsgFlag );
	virtual void SendMoneyMsg(BYTE flag);
};




/* Pyoguk µ· */
class CPyogukPurse : public CPurse
{
protected:
	virtual void SetMoney( MONEYTYPE money, BYTE MsgFlag );
//	virtual void SendMoneyMsg(BYTE flag);
};




/* Munpa µ· */
class CMunpaWarePurse : public CPurse
{
protected:
	virtual void SetMoney( MONEYTYPE money, BYTE MsgFlag );
	virtual void SendMoneyMsg(BYTE flag);
};


#endif //__CLASS_PURSE__
