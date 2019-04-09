// SiegeWarProfitMgr.h: interface for the CSiegeWarProfitMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIEGEWARPROFITMGR_H__CEEF72CA_7C46_41EB_8DCF_399ADADD35E6__INCLUDED_)
#define AFX_SIEGEWARPROFITMGR_H__CEEF72CA_7C46_41EB_8DCF_399ADADD35E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma pack(push,1)

struct sSWPROFIT
{
	int		nSWMapNum;
	DWORD	dwGuildIdx;
	char	sGuildName[MAX_GUILD_NAME+1];
	int		nTaxRateForStore;			// 100분율
	DWORD	dwTotalMoney;
	stTIME	RegDate;
	
	sSWPROFIT()
	{
		memset( this, 0, sizeof(sSWPROFIT) );
	}
};

#pragma pack(pop)


#define SWPROFITMGR CSiegeWarProfitMgr::GetInstance()

class CPlayer;
class CMonster;

class CSiegeWarProfitMgr  
{
protected:
	CYHHashTable<sSWPROFIT>		m_ProfitTable;
	CYHHashTable<WORD>			m_SWMapTable;

	sSWPROFIT*	m_pProfit;
	int			m_nRelationSWMapNum;		// 현재맵에 관계된 공성맵번호
	DWORD		m_dwMoneyForStore;
	DWORD		m_dwMoneyForMonster;

	DWORD		m_dwUpdateTime;
	DWORD		m_dwDefaultMonsterMoney;

public:
	MAKESINGLETON( CSiegeWarProfitMgr );

	CSiegeWarProfitMgr();
	virtual ~CSiegeWarProfitMgr();

	void	LoadMapInfo( WORD wMapNum );
	void	AddProfitTable( sSWPROFIT Info );
	DWORD	AddProfitFromItemSell( DWORD dwMoney );
	DWORD	AddProfitFromItemBuy( DWORD dwMoney );
	void	AddProfitFromMonster( CPlayer* pPlayer, CMonster* pMonster );
	void	AddPlayer( CPlayer* pPlayer );

	void	SetProfitMoney( int nMapNum, DWORD dwMoney );
	void	SetProfitGuild( DWORD dwGuildIdx );
	
	void	ProcessDBUpdateProfitMoney();

	// Network
	void	NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg );
	void	MsgProfitInfoSyn( void* pMsg );
	void	MsgChangTexrateSyn( void* pMsg );
	void	MsgChangeTexrateNotifyToMap( void* pMsg );
	void	MsgGetMoneySyn( void* pMsg );	
	void	MsgChangeGuildNotifyToMap( void* pMsg );

	void	SendNackMsg( CPlayer* pPlayer, BYTE Protocol, DWORD dwErr = 0, DWORD dwData = 0 );
};

#endif // !defined(AFX_SIEGEWARPROFITMGR_H__CEEF72CA_7C46_41EB_8DCF_399ADADD35E6__INCLUDED_)
