// GuildUnion.h: interface for the CGuildUnion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDUNION_H__DF718FC7_073F_4EB6_BDDE_E0E307475FEF__INCLUDED_)
#define AFX_GUILDUNION_H__DF718FC7_073F_4EB6_BDDE_E0E307475FEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define GUILDUNION USINGTON(CGuildUnion)

class CPlayer;

class CGuildUnion  
{
protected:
	DWORD			m_dwGuildUnionIdx;
	char			m_sGuildUnionName[MAX_GUILD_NAME+1];
	sGUILDIDXNAME	m_GuildInfo[MAX_GUILD_UNION_NUM];

	DWORD			m_dwTempPlayerId;
	DWORD			m_dwTempGuildUnionIdx;
	DWORD			m_dwTempGuildIdx;

public:
	CGuildUnion();
	virtual ~CGuildUnion();

	DWORD	GetGuildUnionIdx()		{ return m_dwGuildUnionIdx; }
	char*	GetGuildUnionName()		{ return m_sGuildUnionName; }
	sGUILDIDXNAME*	GetGuildUnionInfo()	{ return m_GuildInfo; }
	BOOL	IsGuildUnionMaster( CPlayer* pPlayer );
	char*	GetGuildUnionNameFromListIdx( int nIdx );
	void	Clear();
	
	void	NetworkMsgParse( BYTE Protocol, void* pMsg );
	void	MsgGuildUnionPlayerInfo( void* pMsg );
	void	MsgGuildUnionInfo( void* pMsg );
	void	MsgGuildUnionCreateAck( void* pMsg );
	void	MsgGuildUnionDestroyAck( void* pMsg );
	void	MsgGuildUnionInvite( void* pMsg );
	void	MsgGuildUnionInviteDeny( void* pMsg );
	void	MsgGuildUnionAddAck( void* pMsg ); 
	void	MsgGuildUnionRemoveAck( void* pMsg );
	void	MsgGuildUnionSecedeAck( void* pMsg );
	void	MsgGuildUnionMarkRegistAck( void* pMsg );
	void	MsgGuildUnionMarkRequestAck( void* pMsg );

	void	MsgGuildUnionCreateNack( void* pMsg );
	void	MsgGuildUnionDestroyNack( void* pMsg );
	void	MsgGuildUnionInviteNack( void* pMsg );
	void	MsgGuildUnionRemoveNack( void* pMsg );
	void	MsgGuildUnionSecedeNack( void* pMsg );
	void	MsgGuildUnionMarkRegistNack( void* pMsg );

	void	SendCreateGuildUnion( char* pName );
	void	SendDestroyGuildUnion();
	void	SendInviteGuildUnion( CPlayer* pPlayer );
	void	SendInviteAcceptDeny( BYTE Protocol );
	void	SendRemoveGuildUnion();
	void	SendSecedeGuildUnion();

	BOOL	CheckGuildUnionCondition( int nCondition );
	BOOL	IsEmptyPosition();

	BOOL	IsUnionGuild( char* pGuildName );

	//SW080515 함수 추가.
	BOOL	IsSameUnion(CPlayer* pPlayerA, CPlayer* pPlayerB);

};

EXTERNGLOBALTON(CGuildUnion)

#endif // !defined(AFX_GUILDUNION_H__DF718FC7_073F_4EB6_BDDE_E0E307475FEF__INCLUDED_)
