// GuildUnion.h: interface for the CGuildUnion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDUNION_H__BF4BF616_9A46_4DF0_AC89_158594B1946D__INCLUDED_)
#define AFX_GUILDUNION_H__BF4BF616_9A46_4DF0_AC89_158594B1946D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGuild;

class CGuildUnion  
{
protected:
	DWORD			m_dwGuildUnionIdx;
	char			m_sGuildUnionName[MAX_GUILD_NAME+1];
	sGUILDIDXNAME	m_GuildInfo[MAX_GUILD_UNION_NUM];
	DWORD			m_dwGuildUnionMarkIdx;
	
	void			SetGuildInfo( int nIndex, DWORD dwGuildIdx );
public:
	CGuildUnion();
	virtual ~CGuildUnion();

	/*
	void	LoadUnionInfo( DWORD dwUnionIdx, char* pName, DWORD dwGuild0, DWORD dwGuild1, DWORD dwGuild2,
						   DWORD dwGuild3, DWORD dwGuild4, DWORD dwGuild5, DWORD dwGuild6, DWORD dwMarkIdx );
	*/
	void	LoadUnionInfo( DWORD dwUnionIdx, char* pName, DWORD dwMarkIdx, DWORD* pGuildIdxList );

	DWORD	GetGuildUnionIdx()			{ return m_dwGuildUnionIdx; }
	char*	GetGuildUnionName()			{ return m_sGuildUnionName; }
	DWORD	GetGuildUnionMarkIdx()		{ return m_dwGuildUnionMarkIdx; }
	DWORD	GetMasterGuildIdx()			{ return m_GuildInfo[0].dwGuildIdx; }
	void	GetGuildUnionInfo( SEND_GUILD_UNION_INFO* pMsg );
	void	SetGuildUnionMarkIdx( DWORD dwMarkIdx );

	void	CreateGuildUnion( DWORD dwGuildUnionIdx, char* pGuildUnionName, DWORD dwMasterGuildIdx );
	void	Destroy();

	BOOL	IsEmptyPosition();
	BOOL	IsMasterGuild( DWORD dwGuildIdx )	{ return m_GuildInfo[0].dwGuildIdx == dwGuildIdx ? TRUE : FALSE; }
	BOOL	IsGuildUnion( DWORD dwGuildIdx );
	BOOL	IsGuildUnion( CGuild* pGuild );
	BOOL	IsGuildFieldWarWithUnionGuild( DWORD dwGuildIdx );
	int		AddGuild( DWORD dwGuildIdx );
	int		AddGuild( CGuild* pGuild );
	int		RemoveGuild( DWORD dwGuildIdx );
	int		RemoveGuild( CGuild* pGuild );	

	void	SendMsgToGuildUnionAll( MSGBASE* pMsg, int nSize );

	// SiegeWar Service Function
	void	GetGuildListInUnion( cPtrList* pList );
	CGuild*	GetMasterGuild();
	void SendAllNote(CPlayer *pPlayer, char* note);

};

#endif // !defined(AFX_GUILDUNION_H__BF4BF616_9A46_4DF0_AC89_158594B1946D__INCLUDED_)
