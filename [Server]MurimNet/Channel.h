// Channel.h: interface for the Channel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHANNEL_H__B10567EF_11FA_487E_8CBD_56B2229626BA__INCLUDED_)
#define AFX_CHANNEL_H__B10567EF_11FA_487E_8CBD_56B2229626BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//max players in a channel?
#include <HashTable.h>


class CMNPlayer;


class CChannel  
{
protected:

	CHANNEL_BASEINFO			m_ChannelInfo;

	CYHHashTable<CMNPlayer>		m_htPlayer;

public:

	CChannel();
	virtual ~CChannel();

	void Create( CHANNEL_CREATE_INFO* pcci );
	void Release();
	DWORD GetChannelIndex()		{ return m_ChannelInfo.dwChannelIndex; }

	BOOL AddPlayer( CMNPlayer* pMNPlayer );
	BOOL DeletePlayer( CMNPlayer* pMNPlayer );
	BOOL DeletePlayer( DWORD dwObjectID );
	void DeleteAllPlayers();

	BOOL PlayerIn( CMNPlayer* pMNPlayer );
	BOOL PlayerOut( CMNPlayer* pMNPlayer );
	BOOL IsInThisChannel( CMNPlayer* pMNPlayer );

//---	
	void SelectChannelMode( CMNPlayer* pMNPlayer, int nChannelMode );

//----
	char* GetTitle()	{ return m_ChannelInfo.strChannelTitle; }
	CHANNEL_BASEINFO*	GetBaseInfo()	{ return &m_ChannelInfo; }

//---msg Func.
	void SendMsgToAll( MSGBASE* pMsg, int MsgLen, CMNPlayer* pExceptPlayer = NULL );

	void SendMsg_ChannelInfo( CMNPlayer* pMNPlayer );
	void SendMsg_NewPlayerInfoToAll( CMNPlayer* pMNPlayer );
	void SendMsg_BecomeCaptain( CMNPlayer* pMNPlayer );
	void SendMsg_AllPlayerInfo( CMNPlayer* pMNPlayer );
	void SendMsg_PlayerOut( CMNPlayer* pMNPlayer );
	

};

#endif // !defined(AFX_CHANNEL_H__B10567EF_11FA_487E_8CBD_56B2229626BA__INCLUDED_)
