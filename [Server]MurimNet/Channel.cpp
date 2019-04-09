// Channel.cpp: implementation of the Channel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Channel.h"
#include "MNPlayer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChannel::CChannel()
{

}

CChannel::~CChannel()
{

}

void CChannel::Create( CHANNEL_CREATE_INFO* pcci )
{
	m_ChannelInfo.dwChannelIndex	= pcci->dwChannelIndex;
	strcpy( m_ChannelInfo.strChannelTitle, pcci->strChannelTitle );
	m_ChannelInfo.cbChannelKind		= pcci->cbChannelKind;
	m_ChannelInfo.wMaxPlayer		= pcci->wMaxPlayer;
	m_ChannelInfo.wPlayerNum		= 0;
	
	m_htPlayer.Initialize( m_ChannelInfo.wMaxPlayer );


}

void CChannel::Release()
{
	DeleteAllPlayers();
}

BOOL CChannel::AddPlayer( CMNPlayer* pMNPlayer )
{
	if( m_htPlayer.Add( pMNPlayer, pMNPlayer->GetID() ) == FALSE )
		return FALSE;

	++m_ChannelInfo.wPlayerNum;
	return TRUE;
}

BOOL CChannel::DeletePlayer( CMNPlayer* pMNPlayer )
{
	return DeletePlayer( pMNPlayer->GetID() );
}

BOOL CChannel::DeletePlayer( DWORD dwObjectID )
{
	if( m_htPlayer.GetData( dwObjectID ) == NULL )
		return FALSE;

	m_htPlayer.Remove( dwObjectID );
	--m_ChannelInfo.wPlayerNum;
	return TRUE;
}

void CChannel::DeleteAllPlayers()
{
	m_htPlayer.RemoveAll();
	m_ChannelInfo.wPlayerNum = 0;
}

BOOL CChannel::PlayerIn( CMNPlayer* pMNPlayer )
{
	if( AddPlayer( pMNPlayer ) == FALSE )
		return FALSE;

	pMNPlayer->SetLocation( eLOCATION_CHANNEL );
	pMNPlayer->SetLocationIndex( m_ChannelInfo.dwChannelIndex );

	SendMsg_ChannelInfo( pMNPlayer );
	SendMsg_NewPlayerInfoToAll( pMNPlayer );
	if( pMNPlayer->IsCaptain() )
		SendMsg_BecomeCaptain( pMNPlayer );

	SendMsg_AllPlayerInfo( pMNPlayer );

	return TRUE;
}

BOOL CChannel::PlayerOut( CMNPlayer* pMNPlayer )
{
	if( DeletePlayer( pMNPlayer ) == FALSE )
		return FALSE;

	if( IsInThisChannel( pMNPlayer ) )	//이 채널에 접속되어 있었다면 위치를 초기화한다.
	{
		pMNPlayer->SetLocation( eLOCATION_NULL );
		pMNPlayer->SetLocationIndex( 0 );
	}

	SendMsg_PlayerOut( pMNPlayer );

	return TRUE;
}

void CChannel::SelectChannelMode( CMNPlayer* pMNPlayer, int nChannelMode )
{
	switch( nChannelMode )
	{
	case eCNL_MODE_ID:
		{
		}
		break;
	case eCNL_MODE_CHANNEL:
		{
		}
		break;
	case eCNL_MODE_PLAYROOM:
		{
		}
		break;
	}	
}

BOOL CChannel::IsInThisChannel( CMNPlayer* pMNPlayer )
{
	if( pMNPlayer->GetLocation() == eLOCATION_CHANNEL && 
		pMNPlayer->GetLocationIndex() == m_ChannelInfo.dwChannelIndex )
		return TRUE;
	
	return FALSE;
}

void CChannel::SendMsgToAll( MSGBASE* pMsg, int MsgLen, CMNPlayer* pExceptPlayer )
{
	CMNPlayer* pMNPlayer;

	m_htPlayer.SetPositionHead();
	while( pMNPlayer = m_htPlayer.GetData() )
	{
		if( pMNPlayer != pExceptPlayer )
		{
			pMNPlayer->SendMsg( pMsg, MsgLen );
		}
	}
}

void CChannel::SendMsg_ChannelInfo( CMNPlayer* pMNPlayer )
{
	static MSG_CHANNEL_BASEINFO msgChannelInfo;
	msgChannelInfo.Category		= MP_MURIMNET;
	msgChannelInfo.Protocol		= MP_MURIMNET_CHNL_CHANNELINFO;
	msgChannelInfo.Info			= m_ChannelInfo;

	pMNPlayer->SendMsg( &msgChannelInfo, sizeof( msgChannelInfo ) );
}

void CChannel::SendMsg_NewPlayerInfoToAll( CMNPlayer* pMNPlayer )
{
	static MSG_MNPLAYER_BASEINFO msgPlayerInfo;
	msgPlayerInfo.Category = MP_MURIMNET;
	msgPlayerInfo.Protocol = MP_MURIMNET_CHNL_PLAYERIN;

	msgPlayerInfo.Info.dwObjectID			= pMNPlayer->GetID();
	msgPlayerInfo.Info.Level				= pMNPlayer->GetLevel();
	msgPlayerInfo.Info.wRankPoint			= 0;
	msgPlayerInfo.Info.wPlayCount			= 0;
	msgPlayerInfo.Info.wLose				= 0;
	msgPlayerInfo.Info.wWin					= 0;
	msgPlayerInfo.Info.cbTeam				= eTEAM_MAX;
	msgPlayerInfo.Info.cbPositionInMunpa	= 0;	//
	strcpy( msgPlayerInfo.Info.strPlayerName, pMNPlayer->GetName() );
	strcpy( msgPlayerInfo.Info.strNick, "" );
	strcpy( msgPlayerInfo.Info.strMunpa, "" );

	SendMsgToAll( &msgPlayerInfo, sizeof( msgPlayerInfo ), pMNPlayer );
}

void CChannel::SendMsg_BecomeCaptain( CMNPlayer* pMNPlayer )
{
	MSG_BYTE msgCaptain;
	msgCaptain.Category		= MP_MURIMNET;
	msgCaptain.Protocol		= MP_MURIMNET_CHNL_CAPTAIN;
	msgCaptain.bData		= 1;
		
	pMNPlayer->SendMsg( &msgCaptain, sizeof( msgCaptain ) );
}

void CChannel::SendMsg_AllPlayerInfo( CMNPlayer* pMNPlayer )
{
	static MSG_MNPLAYER_BASEINFOLIST msgPlayerInfoList;

	msgPlayerInfoList.Category			= MP_MURIMNET;
	msgPlayerInfoList.Protocol			= MP_MURIMNET_CHNL_PLAYERLIST;
	
	msgPlayerInfoList.dwTotalPlayerNum	= m_ChannelInfo.wPlayerNum;

	CMNPlayer* pPlayer;
	m_htPlayer.SetPositionHead();

	for( int i = 0 ; pPlayer = m_htPlayer.GetData() ; ++i  )
	{
		msgPlayerInfoList.PlayerInfo[i].dwObjectID			= pMNPlayer->GetID();
		msgPlayerInfoList.PlayerInfo[i].Level				= pMNPlayer->GetLevel();
		msgPlayerInfoList.PlayerInfo[i].wRankPoint			= 0;
		msgPlayerInfoList.PlayerInfo[i].wPlayCount			= 0;
		msgPlayerInfoList.PlayerInfo[i].wLose				= 0;
		msgPlayerInfoList.PlayerInfo[i].wWin				= 0;
		msgPlayerInfoList.PlayerInfo[i].cbTeam				= eTEAM_MAX;
		msgPlayerInfoList.PlayerInfo[i].cbPositionInMunpa	= 0;	//
		strcpy( msgPlayerInfoList.PlayerInfo[i].strPlayerName, pMNPlayer->GetName() );
		strcpy( msgPlayerInfoList.PlayerInfo[i].strNick, "" );
		strcpy( msgPlayerInfoList.PlayerInfo[i].strMunpa, "" );
	}

	pMNPlayer->SendMsg( (MSGBASE*)&msgPlayerInfoList, msgPlayerInfoList.GetMsgLength() );
}

void CChannel::SendMsg_PlayerOut( CMNPlayer* pMNPlayer )
{
	MSG_DWORD msgPlayerOut;
	msgPlayerOut.Category	= MP_MURIMNET;
	msgPlayerOut.Protocol	= MP_MURIMNET_CHNL_PLAYEROUT;
	msgPlayerOut.dwData		= pMNPlayer->GetID();
	
	SendMsgToAll( &msgPlayerOut, sizeof( msgPlayerOut ) );
}