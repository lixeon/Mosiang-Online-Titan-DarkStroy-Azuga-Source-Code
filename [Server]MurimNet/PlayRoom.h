// PlayRoom.h: interface for the CPlayRoom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYROOM_H__198F33F5_F019_48CD_902D_C788CBDF4865__INCLUDED_)
#define AFX_PLAYROOM_H__198F33F5_F019_48CD_902D_C788CBDF4865__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMNPlayer;


class CTeam
{
protected:

	CYHHashTable<CMNPlayer>	m_htPlayer;

	int		m_nTeam;
	
	DWORD	m_dwMaxPlayerNum;
	DWORD	m_dwCurPlayerNum;

public:

	CTeam();
	virtual ~CTeam();

	void Init( int nTeam, DWORD dwMaxPlayerNum );

	BOOL AddPlayer( CMNPlayer* pMNPlayer );
	void DeletePlayer( CMNPlayer* pMNPlayer );
	void DeleteAllPlayer();
	
	DWORD GetPlayerCount() { return m_dwCurPlayerNum; }
};



class CPlayRoom  
{
protected:

	PLAYROOM_BASEINFO	m_PlayRoomInfo;

	CYHHashTable<CMNPlayer>	m_htPlayer;

	CTeam	m_Team[eTEAM_MAX];

public:

	CPlayRoom();
	virtual ~CPlayRoom();

	void Create( PLAYROOM_CREATE_INFO* ppci );
	void Release();
	DWORD GetPlayRoomIndex()	{ return m_PlayRoomInfo.dwPlayRoomIndex; }

	BOOL AddPlayer( CMNPlayer* pMNPlayer );
	BOOL DeletePlayer( CMNPlayer* pMNPlayer );
	BOOL DeletePlayer( DWORD dwObjectID );
	void DeleteAllPlayers();

	BOOL PlayerIn( CMNPlayer* pMNPlayer );
	BOOL PlayerOut( CMNPlayer* pMNPlayer );
	void PlayerReconnect( CMNPlayer* pMNPlayer );
	BOOL IsInThisPlayRoom( CMNPlayer* pMNPlayer );
	void PlayStart( BOOL bStart );
	BOOL IsStarted()				{ return m_PlayRoomInfo.cbStart; }

//---
	BOOL TeamChange( CMNPlayer* pMNPlayer, BYTE cbFromTeam, BYTE cbToTeam );
//---
	char* GetTitle()					{ return m_PlayRoomInfo.strPlayRoomTitle; }
	PLAYROOM_BASEINFO*	GetBaseInfo()	{ return &m_PlayRoomInfo; }

///
	WORD GetPlayerNum()					{ return m_PlayRoomInfo.wPlayerNum; }
	void GetPlayerInfoList( MSPLAYER_INFO* pInfoArray );
	
	MAPTYPE GetMapNum()					{ return m_PlayRoomInfo.wMapNum;	}
///

//---msg Func.
	void SendMsgToAll( MSGBASE* pMsg, int MsgLen, CMNPlayer* pExceptPlayer = NULL, BOOL bOnlyPlayRoom = TRUE );
//	void SendMsgAllExceptOne( CMNPlayer* pExceptPlayer, MSGBASE* pMsg, int MsgLen );

	void SendMsg_PlayRoomInfo( CMNPlayer* pMNPlayer );
	void SendMsg_NewPlayerInfoToAll( CMNPlayer* pMNPlayer );
	void SendMsg_BecomeCaptain( CMNPlayer* pMNPlayer );
	void SendMsg_AllPlayerInfo( CMNPlayer* pMNPlayer );
	void SendMsg_PlayerOut( CMNPlayer* pMNPlayer );



};

#endif // !defined(AFX_PLAYROOM_H__198F33F5_F019_48CD_902D_C788CBDF4865__INCLUDED_)
