// GuildFieldWar.h: interface for the CGuildFieldWar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDFIELDWAR_H__EEA3D263_F9D6_4A32_A5FB_6C4C9991BAA5__INCLUDED_)
#define AFX_GUILDFIELDWAR_H__EEA3D263_F9D6_4A32_A5FB_6C4C9991BAA5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define GUILDFIELDWAR USINGTON(CGuildFieldWar)

class CPlayer;

class CGuildFieldWar  
{
protected:
	CYHHashTable<GUILDINFO>		m_EnemyGuildInfoTable;
	GUILDINFO*					m_pTempGuildInfo;
	DWORD						m_dwTempEnemyGuildIdx;
	DWORD						m_dwTempGFWMoney;
	
	DWORD						m_dwVictoryCount;
	DWORD						m_dwDrawCount;
	DWORD						m_dwLooseCount;

public:
	CGuildFieldWar();
	virtual ~CGuildFieldWar();

	void	Init();
	void	Release();

	BOOL	IsEnemy( CPlayer* pTarget );
	BOOL	IsEnemyGuild( char* pGuildName );
	void	RegistEnemyGuild( GUILDINFO* pGuildInfo );
	void	UnRegistEnemyGuild( DWORD dwEnemyGuildIdx );
	GUILDINFO*	GetEnemyGuildInfo( DWORD dwEnemyGuildIdx )	{ return m_EnemyGuildInfoTable.GetData( dwEnemyGuildIdx ); }
	GUILDINFO*	GetEnemyGuildInfoFromListIdx( int nIdx );
	CYHHashTable<GUILDINFO>*	GetEnemyGuildTable()		{ return &m_EnemyGuildInfoTable; }
	BOOL	CheckGuildFieldWarCondition( DWORD dwKind );
	void	GetVictory( char* temp );

	void	NetworkMsgParse( BYTE Protocol, void* pMsg );

	void	SendGuildFieldWarDeclare( char* pName, char* pMoney );
	void	SendGuildFieldWarDeclareAcceptDeny( BYTE Protocol );
	void	SendGuildFieldWarSurrend();
	void	SendGuildFieldWarSuggest();
	void	SendGuildFieldWarSuggestAcceptDeny( BYTE Protocol );

	// testinferface
	void	TestSuggestAcceptDeny();
};

EXTERNGLOBALTON(CGuildFieldWar)

#endif // !defined(AFX_GUILDFIELDWAR_H__EEA3D263_F9D6_4A32_A5FB_6C4C9991BAA5__INCLUDED_)
