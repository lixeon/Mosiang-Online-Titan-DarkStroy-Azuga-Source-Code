// MNPlayerManager.cpp: implementation of the CMNPlayerManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MNPlayerManager.h"
#include "MNPlayer.h"
#include "../[CC]Header/CommonGameFunc.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMNPlayerManager::CMNPlayerManager()
{

}

CMNPlayerManager::~CMNPlayerManager()
{

}

void CMNPlayerManager::Init( DWORD dwMaxPlayer )
{
	m_mpPlayerPool = new CMemoryPool<CMNPlayer>;
	m_mpPlayerPool->Init( dwMaxPlayer );

	m_htPlayer.Initialize( dwMaxPlayer );

	m_nTotalPlayer	= 0;
	m_dwMaxPlayer	= dwMaxPlayer;
}


void CMNPlayerManager::Release()
{
	DeleteAllPlayers();
	SAFE_DELETE( m_mpPlayerPool );
}


void CMNPlayerManager::DeletePlayer( DWORD dwPlayerID )
{
	m_htPlayer.Remove( dwPlayerID );

	//---delete also at the current location?
}

void CMNPlayerManager::DeleteAllPlayers()
{
	m_htPlayer.RemoveAll();	
}

CMNPlayer* CMNPlayerManager::FindPlayer( DWORD dwPlayerID )
{
	return m_htPlayer.GetData(dwPlayerID);
}






CMNPlayer* CMNPlayerManager::AddPlayer( DWORD dwPlayerID, DWORD dwAgentNum, DWORD UniqueIDinAgent, DWORD dwBackMapNum )
{
	ASSERT( m_htPlayer.GetData(dwPlayerID) == NULL );

	BASEOBJECT_INFO binfo;
	memset( &binfo, 0 , sizeof(binfo) );
	binfo.dwObjectID = dwPlayerID;

	CMNPlayer* pPlayer = m_mpPlayerPool->Alloc();
	
	ASSERT(pPlayer);

	if( pPlayer->Init( dwPlayerID, dwAgentNum, UniqueIDinAgent, dwBackMapNum ) == FALSE )
	{
		m_mpPlayerPool->Free( pPlayer );
		return FALSE;
	}

	m_htPlayer.Add( pPlayer, pPlayer->GetID() );


	
	return pPlayer;
}

//void CMNPlayerManager::DeletePlayer()
