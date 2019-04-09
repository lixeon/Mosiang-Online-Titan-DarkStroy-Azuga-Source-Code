// PlayRoom.cpp: implementation of the CPlayRoom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PlayRoom.h"
#include "MNPlayer.h"
#include "../[CC]Header/CommonGameFunc.h"
//임시
#include "MurimNetSystem.h"
#include "MNPlayerManager.h"
//

CTeam::CTeam()
{

}

CTeam::~CTeam()
{
	DeleteAllPlayer();
}

void CTeam::Init( int nTeam, DWORD dwMaxPlayerNum )
{
	m_htPlayer.Initialize( dwMaxPlayerNum );	//

	m_dwMaxPlayerNum = dwMaxPlayerNum;
	DeleteAllPlayer();

	m_nTeam = nTeam;
}

BOOL CTeam::AddPlayer( CMNPlayer* pMNPlayer )
{
	if( m_dwCurPlayerNum >= m_dwMaxPlayerNum ) return FALSE;

	pMNPlayer->SetTeam( m_nTeam );

	m_htPlayer.Add( pMNPlayer, pMNPlayer->GetID() );
	++m_dwCurPlayerNum;

	return TRUE;
}

void CTeam::DeletePlayer( CMNPlayer* pMNPlayer )
{
	ASSERT( m_htPlayer.GetData( pMNPlayer->GetID() ) );
	
	m_htPlayer.Remove( pMNPlayer->GetID() );

//	pMNPlayer->SetTeam( eTEAM_MAX );
	
	--m_dwCurPlayerNum;
}

void CTeam::DeleteAllPlayer()
{
	m_htPlayer.RemoveAll();

	m_dwCurPlayerNum = 0;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPlayRoom::CPlayRoom()
{

}


CPlayRoom::~CPlayRoom()
{

}


void CPlayRoom::Create( PLAYROOM_CREATE_INFO* ppci )
{
	m_PlayRoomInfo.dwPlayRoomIndex		= ppci->dwPlayRoomIndex;
	strcpy( m_PlayRoomInfo.strPlayRoomTitle, ppci->strPlayRoomTitle );
	m_PlayRoomInfo.cbPlayRoomKind		= ppci->cbPlayRoomKind;
	m_PlayRoomInfo.wMaxPlayerPerTeam	= ppci->wMaxPlayerPerTeam;
	m_PlayRoomInfo.wMaxObserver			= ppci->wMaxObserver;
	m_PlayRoomInfo.MoneyForPlay			= ppci->MoneyForPlay;
	m_PlayRoomInfo.wPlayerNum			= 0;
	m_PlayRoomInfo.wMapNum				= ppci->wMapNum;
	
	m_htPlayer.Initialize( m_PlayRoomInfo.wMaxPlayerPerTeam * 2 + m_PlayRoomInfo.wMaxObserver );//CONFIRM

	m_Team[eTEAM_LEFT].Init( eTEAM_LEFT, m_PlayRoomInfo.wMaxPlayerPerTeam );
	m_Team[eTEAM_RIGHT].Init( eTEAM_RIGHT, m_PlayRoomInfo.wMaxPlayerPerTeam );
	m_Team[eTEAM_OBSERVER].Init( eTEAM_OBSERVER, m_PlayRoomInfo.wMaxObserver );

	m_PlayRoomInfo.cbStart = FALSE;
}

void CPlayRoom::Release()
{
	DeleteAllPlayers();

	for( int i = 0 ; i < eTEAM_MAX ; ++i )
		m_Team[i].DeleteAllPlayer();
}

BOOL CPlayRoom::AddPlayer( CMNPlayer* pMNPlayer )
{
	if( m_htPlayer.Add( pMNPlayer, pMNPlayer->GetID() ) == FALSE )
		return FALSE;

//----	원래 기획: 옵져버에 추가 후 이동
//	if( m_Team[eTEAM_OBSERVER].AddPlayer( pMNPlayer ) == FALSE ) 
//		return FALSE;
//----  옵저버 구현 미룸 : 수가 적은 쪽으로 추가	//
	if( m_Team[eTEAM_LEFT].GetPlayerCount() <= m_Team[eTEAM_RIGHT].GetPlayerCount() )
		m_Team[eTEAM_LEFT].AddPlayer( pMNPlayer );
	else
		m_Team[eTEAM_RIGHT].AddPlayer( pMNPlayer );

//---임시 방장 등록
	if( m_PlayRoomInfo.wPlayerNum == 0 )
		pMNPlayer->SetCaptain( TRUE );
//-----------------

	++m_PlayRoomInfo.wPlayerNum;
	return TRUE;
}

BOOL CPlayRoom::DeletePlayer( CMNPlayer* pMNPlayer )
{
	return DeletePlayer( pMNPlayer->GetID() );
}

BOOL CPlayRoom::DeletePlayer( DWORD dwObjectID )
{
	CMNPlayer* pMNPlayer = m_htPlayer.GetData( dwObjectID );
	ASSERT( pMNPlayer );
	if( pMNPlayer == NULL )	return FALSE;

	m_htPlayer.Remove( dwObjectID );
	m_Team[ pMNPlayer->GetTeam() ].DeletePlayer( pMNPlayer );

	--m_PlayRoomInfo.wPlayerNum;

	return TRUE;
}

void CPlayRoom::DeleteAllPlayers()
{
	m_htPlayer.RemoveAll();
	m_PlayRoomInfo.wPlayerNum = 0;
}

BOOL CPlayRoom::PlayerIn( CMNPlayer* pMNPlayer )
{
	if( AddPlayer( pMNPlayer ) == FALSE )
		return FALSE;

	pMNPlayer->SetLocation( eLOCATION_PLAYROOM );
	pMNPlayer->SetLocationIndex( m_PlayRoomInfo.dwPlayRoomIndex );
	
	SendMsg_PlayRoomInfo( pMNPlayer );
	SendMsg_NewPlayerInfoToAll( pMNPlayer );
	if( pMNPlayer->IsCaptain() )
		SendMsg_BecomeCaptain( pMNPlayer );

	SendMsg_AllPlayerInfo( pMNPlayer );

	return TRUE;
}

BOOL CPlayRoom::PlayerOut( CMNPlayer* pMNPlayer )
{
	if( DeletePlayer( pMNPlayer ) == FALSE )
		return FALSE;

	if( IsInThisPlayRoom( pMNPlayer ) )	//이 채널에 접속되어 있었다면 위치를 초기화한다.
	{
		pMNPlayer->SetLocation( eLOCATION_NULL );
		pMNPlayer->SetLocationIndex( 0 );
	}

	SendMsg_PlayerOut( pMNPlayer );

	return TRUE;
}

BOOL CPlayRoom::IsInThisPlayRoom( CMNPlayer* pMNPlayer )
{
	if( pMNPlayer->GetLocation() == eLOCATION_PLAYROOM && 
		pMNPlayer->GetLocationIndex() == m_PlayRoomInfo.dwPlayRoomIndex )
		return TRUE;
	
	return FALSE;
}

//---얻어오는 것이 아니라 여기서 직접 처리하는 것으로 바꾸자.CONFIRM
void CPlayRoom::GetPlayerInfoList( MSPLAYER_INFO* pInfoArray )
{
	CMNPlayer* pPlayer;

	m_htPlayer.SetPositionHead();
	while( pPlayer = m_htPlayer.GetData() )
	{
		pInfoArray->cbTeam = pPlayer->GetTeam();
		pInfoArray->dwObjectID = pPlayer->GetID();

		++pInfoArray;
	}
}

void CPlayRoom::PlayStart( BOOL bStart )
{
	m_PlayRoomInfo.cbStart = bStart;

	CMNPlayer* pPlayer;

	m_htPlayer.SetPositionHead();

	if( bStart )
	{
		while( pPlayer = m_htPlayer.GetData() )
		{
			pPlayer->SetLocation( eLOCATION_GAME );
		}
	}
	else
	{
		while( pPlayer = m_htPlayer.GetData() )
		{
			pPlayer->SetLocation( eLOCATION_RESULT );
		}
	}
}

BOOL CPlayRoom::TeamChange( CMNPlayer* pMNPlayer, BYTE cbFromTeam, BYTE cbToTeam )
{
	if( cbFromTeam < 0 || cbFromTeam >= eTEAM_MAX )			return FALSE;
	if( cbToTeam < 0 || cbToTeam >= eTEAM_MAX )				return FALSE;

	if( m_Team[cbToTeam].AddPlayer( pMNPlayer ) == FALSE )	return FALSE;
	m_Team[cbFromTeam].DeletePlayer( pMNPlayer );

	MSG_MNTEAMCHANGE msgTeamChange;
	msgTeamChange.Category = MP_MURIMNET;
	msgTeamChange.Protocol = MP_MURIMNET_PR_TEAMCHANGE_ACK;	//모든플레이어에게
	msgTeamChange.dwMoverID	= pMNPlayer->GetID();
	msgTeamChange.cbToTeam = cbToTeam;
	msgTeamChange.cbFromTeam = cbFromTeam;

	SendMsgToAll( &msgTeamChange, sizeof( msgTeamChange ) );

	return TRUE;
}

void CPlayRoom::SendMsgToAll( MSGBASE* pMsg, int MsgLen, CMNPlayer* pExceptPlayer, BOOL bOnlyPlayRoom )
{
	CMNPlayer* pMNPlayer;

	m_htPlayer.SetPositionHead();
	while( pMNPlayer = m_htPlayer.GetData() )
	{
		if( pMNPlayer != pExceptPlayer )
		{
			if( bOnlyPlayRoom && pMNPlayer->GetLocation() != eLOCATION_PLAYROOM ) continue;
			pMNPlayer->SendMsg( pMsg, MsgLen );
		}
	}
}

void CPlayRoom::PlayerReconnect( CMNPlayer* pMNPlayer )
{
	pMNPlayer->SetLocation( eLOCATION_PLAYROOM );

	SendMsg_PlayRoomInfo( pMNPlayer );
	SendMsg_AllPlayerInfo( pMNPlayer );
	if( pMNPlayer->IsCaptain() )
		SendMsg_BecomeCaptain( pMNPlayer );

//---기참여자들에게 플레이어 재접속(결과화면에서돌아옮)을 알림
	MSG_DWORD msgPlayer;
	msgPlayer.Category		= MP_MURIMNET;
	msgPlayer.Protocol		= MP_MURIMNET_PR_PLAYERREIN;
	msgPlayer.dwData		= pMNPlayer->GetID();
	SendMsgToAll( &msgPlayer, sizeof( msgPlayer ), pMNPlayer );

//---재접속성공 알림	//CONFIRM
	MSGBASE msg;
	msg.Category	= MP_MURIMNET;
	msg.Protocol	= MP_MURIMNET_RECONNECT_ACK;
	msg.dwObjectID	= pMNPlayer->GetID();
	pMNPlayer->SendMsg( &msg, sizeof( msg ) );
//------------------
}

void CPlayRoom::SendMsg_PlayRoomInfo( CMNPlayer* pMNPlayer )
{
	MSG_PLAYROOM_BASEINFO msgPlayRoomInfo;
	msgPlayRoomInfo.Category	= MP_MURIMNET;
	msgPlayRoomInfo.Protocol	= MP_MURIMNET_PR_PLAYROOMINFO;
	msgPlayRoomInfo.Info		= m_PlayRoomInfo;

	pMNPlayer->SendMsg( &msgPlayRoomInfo, sizeof( msgPlayRoomInfo ) );
}

void CPlayRoom::SendMsg_NewPlayerInfoToAll( CMNPlayer* pMNPlayer )
{
	static MSG_MNPLAYER_BASEINFO msgPlayerInfo;
	msgPlayerInfo.Category = MP_MURIMNET;
	msgPlayerInfo.Protocol = MP_MURIMNET_PR_PLAYERIN;

	msgPlayerInfo.Info.dwObjectID			= pMNPlayer->GetID();
	msgPlayerInfo.Info.Level				= pMNPlayer->GetLevel();
	msgPlayerInfo.Info.wRankPoint			= 0;
	msgPlayerInfo.Info.wPlayCount			= 0;
	msgPlayerInfo.Info.wLose				= 0;
	msgPlayerInfo.Info.wWin					= 0;
	msgPlayerInfo.Info.cbTeam				= pMNPlayer->GetTeam();
	msgPlayerInfo.Info.cbPositionInMunpa	= 0;	//
	strcpy( msgPlayerInfo.Info.strPlayerName, pMNPlayer->GetName() );
	strcpy( msgPlayerInfo.Info.strNick, "" );
	strcpy( msgPlayerInfo.Info.strMunpa, "" );

	SendMsgToAll( &msgPlayerInfo, sizeof( msgPlayerInfo ), pMNPlayer );
}

void CPlayRoom::SendMsg_BecomeCaptain( CMNPlayer* pMNPlayer )
{
	MSG_BYTE msgCaptain;
	msgCaptain.Category		= MP_MURIMNET;
	msgCaptain.Protocol		= MP_MURIMNET_PR_CAPTAIN;
	msgCaptain.bData		= 1;
		
	pMNPlayer->SendMsg( &msgCaptain, sizeof( msgCaptain ) );
}

void CPlayRoom::SendMsg_AllPlayerInfo( CMNPlayer* pMNPlayer )
{
	static MSG_MNPLAYER_BASEINFOLIST msgPlayerInfoList;

	msgPlayerInfoList.Category			= MP_MURIMNET;
	msgPlayerInfoList.Protocol			= MP_MURIMNET_CHNL_PLAYERLIST;
	
	msgPlayerInfoList.dwTotalPlayerNum	= m_PlayRoomInfo.wPlayerNum;

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
		msgPlayerInfoList.PlayerInfo[i].cbTeam				= pMNPlayer->GetTeam();
		msgPlayerInfoList.PlayerInfo[i].cbPositionInMunpa	= 0;	//
		strcpy( msgPlayerInfoList.PlayerInfo[i].strPlayerName, pMNPlayer->GetName() );
		strcpy( msgPlayerInfoList.PlayerInfo[i].strNick, "" );
		strcpy( msgPlayerInfoList.PlayerInfo[i].strMunpa, "" );
	}

	pMNPlayer->SendMsg( (MSGBASE*)&msgPlayerInfoList, msgPlayerInfoList.GetMsgLength() );
}

void CPlayRoom::SendMsg_PlayerOut( CMNPlayer* pMNPlayer )
{
	MSG_DWORD msgPlayerOut;
	msgPlayerOut.Category	= MP_MURIMNET;
	msgPlayerOut.Protocol	= MP_MURIMNET_PR_PLAYEROUT;
	msgPlayerOut.dwData		= pMNPlayer->GetID();
	
	SendMsgToAll( &msgPlayerOut, sizeof( msgPlayerOut ) );
}