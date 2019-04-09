// GuildFieldWarMgr.h: interface for the CGuildFieldWarMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDFIELDWARMGR_H__3480D370_0C10_4A01_AF47_D15FA15690E8__INCLUDED_)
#define AFX_GUILDFIELDWARMGR_H__3480D370_0C10_4A01_AF47_D15FA15690E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct sGFWENEMY
{
	DWORD	dwEnemyGuildIdx;
	DWORD	dwGFWMoney;
};

class CGuildFieldWar 
{
protected:
	DWORD				m_dwGuildIdx;
	CYHHashTable<sGFWENEMY>	m_EnemyGuildTable;	
	DWORD				m_dwVictoryCount;
	DWORD				m_dwDrawCount;
	DWORD				m_dwLooseCount;

public:
	CGuildFieldWar();
	virtual ~CGuildFieldWar();

	void	Init( DWORD dwGuildIdx )	{ m_dwGuildIdx = dwGuildIdx; }
	void	Release();
	
	void	RegistGuildFieldWar( DWORD dwEnemyGuildIdx, DWORD dwMoney );
	void	UnRegistGuildFieldWar( DWORD dwEnemyGuildIdx );
	BOOL	IsEnemy( DWORD dwEnemyGuildIdx );
	void	AddPlayer( CPlayer* pPlayer );
	BOOL	IsGuildFieldWar();
	void	SetGuildFieldWarRecord( DWORD dwVictory, DWORD dwDraw, DWORD dwLoose )
	{	m_dwVictoryCount = dwVictory;	m_dwDrawCount = dwDraw;	m_dwLooseCount = dwLoose;	}
	void	VictoryGuildFieldWar()	{ ++m_dwVictoryCount; }
	void	DrawGuildFieldWar()		{ ++m_dwDrawCount; }
	void	LooseGuildFieldWar()	{ ++m_dwLooseCount; }
	void	UpdateGuildFieldWarRecord();
	DWORD	GetGuildFieldWarMoney( DWORD dwEnemyGuildIdx );
};

struct sGFWTIME
{
	DWORD		dwEnemyGuildIdx;
	stTIME		Time;
	DWORD		dwCount;
};

class CGFWCheck
{
protected:
	DWORD					m_dwGuildIdx;
	CYHHashTable<sGFWTIME>	m_GFWDenyTable;
	CYHHashTable<sGFWTIME>	m_GFWEndTable;

public:
	CGFWCheck();
	virtual ~CGFWCheck();

	void	Init( DWORD dwGuildIdx )	{ m_dwGuildIdx = dwGuildIdx; }
	void	Release();

	int		CanGuildFieldWar( DWORD dwEnemyGuildIdx );
	void	RegistDenyTime( DWORD dwEnemyGuildIdx );
	void	RegistEndTime( DWORD dwEnemyGuildIdx );
};

#define GUILDFIELDWARMGR CGuildFieldWarMgr::GetInstance()

class CGuildFieldWarMgr  
{
protected:
	CYHHashTable<CGuildFieldWar>	m_GuildFieldWarTable;
	CYHHashTable<CGFWCheck>			m_GFWCheckTable;
	
public:
	MAKESINGLETON( CGuildFieldWarMgr );

	CGuildFieldWarMgr();
	virtual ~CGuildFieldWarMgr();

	void	Init();
	void	Release();

	void	RegistGuildFieldWarFromRecord( DWORD dwGuildIdx, DWORD dwVictory, DWORD dwDraw, DWORD dwLoose );
	void	UpdateGuildFieldWarRecord( BOOL bDB, WORD wFlag, DWORD dwGuildIdx );
	BOOL	RegistGuildFieldWar( DWORD dwGuildIdx1, DWORD dwGuildIdx2, DWORD dwMoney );
	BOOL	UnRegistGuildFieldWar( DWORD dwGuildIdx1, DWORD dwGuildIdx2 );
	BOOL	JudgeGuildFieldWar( CPlayer* pPlayer, CPlayer* pAttacker );
	BOOL	IsEnemy( CPlayer* pOper, CPlayer* pTarget );
	BOOL	IsEnemyInField( CPlayer* pOper, CPlayer* pTarget );
	void	AddPlayer( CPlayer* pPlayer );
	BOOL	IsGuildFieldWar( DWORD dwGuildIdx1, DWORD dwGuildIdx2 );
	BOOL	IsGuildFieldWar( DWORD dwGuildIdx );
	int		CanGuildFieldWar( DWORD dwGuildIdx, DWORD dwEnemyGuildIdx );
	void	RegistDeny( DWORD dwGuildIdx, DWORD dwEnemyGuildIdx );
	void	RegistEnd( DWORD dwGuildIdx1, DWORD dwGuildIdx2 );
	DWORD	GetGuildFieldWarMoney( DWORD dwGuildIdx, DWORD dwEnemyGuildIdx );
	
	void	NetworkMsgParse( BYTE Protocol, void* pMsg );

	void	GuildFieldWarNack( void* pMsg );
	void	GuildFieldWarDeclare( void* pMsg );
	void	GuildFieldWarDeclareAccept( void* pMsg );
	void	GuildFieldWarDeclareDeny( void* pMsg );
	void	GuildFieldWarDeclareDenyNotifyToMap( void* pMsg );

	void	GuildFieldWarStartNotifyToMap( void* pMsg );
	void	GuildFieldWarEnd( DWORD dwGuildIdxWinner, DWORD dwGuildIdxLoser );
	void	GuildFieldWarEndNotifyToMap( void* pMsg );

	void	GuildFieldWarSuggestEnd( void* pMsg );
	void	GuildFieldWarSuggestEndNotifyToMap( void* pMsg );
	void	GuildFieldWarSuggestEndAccept( void* pMsg );
	void	GuildFieldWarSuggestEndAcceptNotifyToMap( void* pMsg );
	void	GuildFieldWarSuggestEndDeny( void* pMsg );
	void	GuildFieldWarSuggestEndDenyNotifyToMap( void* pMsg );
	
	void	GuildFieldWarSurrender( void* pMsg );
	void	GuildFieldWarSurrenderNotifyToMap( void* pMsg );

	void	GuildFieldWarAddMoneyToMap( void* pMsg );

	void	PenaltyForGuildFieldWarEnd( DWORD dwKind, DWORD dwGuildIdxLoser );	
	void	PenaltyForGuildFieldWarEndNotifyToMap( DWORD dwKind, DWORD dwGuildIdxLoser );	

	void	SendDeclareNackMsg( DWORD dwReceiver, DWORD dwState );
	void	SendMoneyMsg( CPlayer* pPlayer1, CPlayer* pPlayer2, DWORD dwMoney, BYTE Protocol );
	void	SendToAgentAddMoneyMsg( DWORD dwPlayerIdx, DWORD dwMoney );
	
	void	SendToAllUser( DWORD dwFlag, char* pGuildName1, char* pGuildName2 );
};

#endif // !defined(AFX_GUILDFIELDWARMGR_H__3480D370_0C10_4A01_AF47_D15FA15690E8__INCLUDED_)
