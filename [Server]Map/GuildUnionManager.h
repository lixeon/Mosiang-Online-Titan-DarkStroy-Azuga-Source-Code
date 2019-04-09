// GuildUnionManager.h: interface for the CGuildUnionManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUILDUNIONMANAGER_H__5E654826_2924_4528_854D_A39C139BB206__INCLUDED_)
#define AFX_GUILDUNIONMANAGER_H__5E654826_2924_4528_854D_A39C139BB206__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGuildUnion;
class CGuildUnionMark;
class CGuild;


#define GUILDUNIONMGR CGuildUnionManager::GetInstance()

class CGuildUnionManager  
{
protected:
	CYHHashTable<CGuildUnion>		m_GuildUnionTable;
	CYHHashTable<CGuildUnionMark>	m_GuildUnionMarkTable;
	CYHHashTable<stTIME>			m_GuildUnionTimeTable;

public:
	GETINSTANCE(CGuildUnionManager);	
	CGuildUnionManager();
	virtual ~CGuildUnionManager();

	/*
	void	LoadGuildUnion( DWORD dwUnionIdx, char* pName, DWORD dwGuild0, DWORD dwGuild1, DWORD dwGuild2,
							DWORD dwGuild3, DWORD dwGuild4, DWORD dwGuild5, DWORD dwGuild6, DWORD dwMarkIdx );	// db에서 로딩
	*/
	void	LoadGuildUnion( DWORD dwUnionIdx, char* pName, DWORD dwMarkIdx, DWORD* pGuildIdxList );

	void	LoadGuildUnionMark( DWORD dwMarkIdx, DWORD dwGuildUnionIdx, char* pImgData );

	void	NetworkMsgParse( BYTE Protocol, void* pMsg );
	void	MsgGuildUnionCreateSyn( void* pMsg );					// 창설
	void	GuildUnionCreateResult( DWORD dwGuildUnionIdx, char* pGuildUnionName, DWORD dwMasterGuildIdx );
	void	MsgGuildUnionCreateNotifyToMap( void* pMsg );
	void	MsgGuildUnionDestroySyn( void* pMsg );					// 해체
	void	MsgGuildUnionDestroyNotifyToMap( void* pMsg );
	void	MsgGuildUnionInvite( void* pMsg );						// 권유
	void	MsgGuildUnionInviteAccept( void* pMsg );
	void	MsgGuildUnionInviteAcceptNotifyToMap( void* pMsg );
	void	MsgGuildUnionInviteDeny( void* pMsg );
	void	MsgGuildUnionRemoveSyn( void* pMsg );					// 추방
	void	MsgGuildUnionRemoveNotifyToMap( void* pMsg );
	void	MsgGuildUnionSecedeSyn( void* pMsg );					// 탈퇴
	void	MsgGuildUnionSecedeNotifyToMap( void* pMsg );
	void	MsgGuildUnionMarkRegistSyn( void* pMsg );				// mark 등록
	void	GuildUnionMarkRegistResult( DWORD dwCharacterIdx, DWORD dwMarkIdx, DWORD dwGuildUnionIdx, char* pImgData );
	void	MsgGuildUnionMarkRegistNotifyToMap( void* pMsg );
	void	MsgGuildUnionMarkRequestSyn( void* pMsg );
	void	MsgGuildUnionNoteSyn( void* pMsg );
	
	void	SendNackMsg( CPlayer* pPlayer, BYTE Protocol, DWORD dwErr, DWORD dwData = 0 );	
	void	SendChatMsg( MSG_CHAT_WITH_SENDERID* pMsg );

	void	AddPlayer( CPlayer* pPlayer, DWORD dwGuildUnionIdx );
	void	LoginPlayer( CPlayer* pPlayer, DWORD dwGuildUnionIdx );
	void	SetGuildUnionInfoToCharacterTotalInfo( CPlayer* pPlayer, DWORD dwGuildIdx );

	BOOL	IsValidGuildUnionName( char* pName );
	DWORD	CheckEntryTime( DWORD dwGuildIdx );
	void	ClearEntryTime( DWORD dwGuildIdx );
	void	AddEntryTime( DWORD dwGuildIdx, DWORD dwTime );
	DWORD	AddEntryTimeForDestroy( DWORD dwGuildIdx );
	DWORD	AddEntryTimeForSecede( DWORD dwGuildIdx );

	BYTE	HexToByte( char* pStr );
	int		convertCharToInt( char c );
	
	// SiegeWar Service Function
	BOOL	GetGuildListInUnion( DWORD dwUnionIdx, cPtrList* pList );
	CGuild*	GetMasterGuildInUnion( DWORD dwUnionIdx );

	//SW080515 함수 추가.
	BOOL	IsSameUnion(CPlayer* pPlayerA, CPlayer* pPlayerB);
};

#endif // !defined(AFX_GUILDUNIONMANAGER_H__5E654826_2924_4528_854D_A39C139BB206__INCLUDED_)
