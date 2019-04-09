// PlayRoomManager.cpp: implementation of the CPlayRoomManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PlayRoomManager.h"
#include "PlayRoom.h"
#include "MNPlayer.h"
#include "../[CC]Header/CommonGameFunc.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlayRoomManager::CPlayRoomManager()
{
	m_pDefaultPlayRoom = NULL;
}

CPlayRoomManager::~CPlayRoomManager()
{

}

void CPlayRoomManager::Init( DWORD dwMaxPlayRoom )
{
	m_mpPlayRoom.Init( dwMaxPlayRoom );
	m_htPlayRoom.Initialize( dwMaxPlayRoom );

	m_hIndexCreator = ICCreate();
	ICInitialize( m_hIndexCreator, dwMaxPlayRoom );

	m_dwTotalPlayRoom	= 0;
	m_dwMaxPlayRoom		= dwMaxPlayRoom;
	
//---Create Test Playroom
	PLAYROOM_CREATE_INFO	pci;
	pci.cbPlayRoomKind		= 0;	//CONFIRM
	pci.wMaxPlayerPerTeam	= MAXPLAYER_PERTEAM;
	strcpy( pci.strPlayRoomTitle, DEFAULT_PLAYROOM_NAME );
	m_pDefaultPlayRoom		= CreatePlayRoom( &pci );
//-----------------------------
}

void CPlayRoomManager::Release()
{
	DeleteAllPlayRoom();	
	ICRelease( m_hIndexCreator );
}

CPlayRoom* CPlayRoomManager::CreatePlayRoom( PLAYROOM_CREATE_INFO* ppci )
{
	if( m_dwTotalPlayRoom >= m_dwMaxPlayRoom )	return NULL;

	CPlayRoom* pPlayRoom	= m_mpPlayRoom.Alloc();
	ppci->dwPlayRoomIndex	= ICAllocIndex( m_hIndexCreator );
	pPlayRoom->Create( ppci );
	
	m_htPlayRoom.Add( pPlayRoom, ppci->dwPlayRoomIndex );
	++m_dwTotalPlayRoom;

	return pPlayRoom;
}

void CPlayRoomManager::DeletePlayRoom( DWORD dwPlayRoomIndex )
{
	CPlayRoom* pPlayRoom = m_htPlayRoom.GetData( dwPlayRoomIndex );
	m_mpPlayRoom.Free( pPlayRoom );
	ICFreeIndex( m_hIndexCreator, dwPlayRoomIndex );

	m_htPlayRoom.Remove( dwPlayRoomIndex );
	--m_dwTotalPlayRoom;
}

void CPlayRoomManager::DeletePlayRoom( CPlayRoom* pPlayRoom )
{
	DWORD dwPlayRoomIndex = pPlayRoom->GetPlayRoomIndex();
	pPlayRoom->Release();
	m_mpPlayRoom.Free( pPlayRoom );
	ICFreeIndex( m_hIndexCreator, dwPlayRoomIndex );

	m_htPlayRoom.Remove( dwPlayRoomIndex );
	--m_dwTotalPlayRoom;
}

void CPlayRoomManager::DeleteAllPlayRoom()
{
	CPlayRoom* pPlayRoom;

	m_htPlayRoom.SetPositionHead();
	while( pPlayRoom = m_htPlayRoom.GetData() )
	{
		DeletePlayRoom( pPlayRoom );
	}
}

BOOL CPlayRoomManager::EnterPlayRoom( CMNPlayer* pMNPlayer, DWORD dwPlayRoomIndex )
{
	CPlayRoom* pPlayRoom = m_htPlayRoom.GetData( dwPlayRoomIndex );

	if( pPlayRoom->PlayerIn( pMNPlayer ) == FALSE )
		return FALSE;

	return TRUE;
}

void CPlayRoomManager::ExitPlayRoom( CMNPlayer* pMNPlayer )
{
	ASSERT( pMNPlayer->GetLocation() != eLOCATION_PLAYROOM );

	CPlayRoom* pPlayRoom = m_htPlayRoom.GetData( pMNPlayer->GetLocationIndex() );

	pPlayRoom->PlayerOut( pMNPlayer );	
}

CPlayRoom* CPlayRoomManager::GetPlayRoom( DWORD dwPlayRoomIndex )
{
	return m_htPlayRoom.GetData( dwPlayRoomIndex );
}

void CPlayRoomManager::SendMsg_PlayRoomList( CMNPlayer* pMNPlayer )
{
	MSG_PLAYROOM_BASEINFOLIST	msgPlayRoomList;

	msgPlayRoomList.Category			= MP_MURIMNET;
	msgPlayRoomList.Protocol			= MP_MURIMNET_CHNL_PLAYROOMINFOLIST;
	msgPlayRoomList.dwTotalPlayRoomNum	= m_dwTotalPlayRoom;

	CPlayRoom* pPlayRoom;
	m_htPlayRoom.SetPositionHead();
	for( int i = 0 ; pPlayRoom = m_htPlayRoom.GetData() ; ++i )
	{
		msgPlayRoomList.PlayRoomInfo[i] = *pPlayRoom->GetBaseInfo();
	}

	pMNPlayer->SendMsg( &msgPlayRoomList, msgPlayRoomList.GetMsgLength() );
}