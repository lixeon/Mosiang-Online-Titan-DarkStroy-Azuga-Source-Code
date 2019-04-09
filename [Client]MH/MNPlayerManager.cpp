// MNPlayerManager.cpp: implementation of the CMNPlayerManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MNPlayerManager.h"
#include "MNPlayer.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMNPlayerManager::CMNPlayerManager()
{
	m_mpPlayerPool	= NULL;
	m_htPlayer		= NULL;
}

CMNPlayerManager::~CMNPlayerManager()
{

}


void CMNPlayerManager::Init()
{
	m_mpPlayerPool = new CMemoryPoolTempl<CMNPlayer>;
	m_mpPlayerPool->Init( MAX_PLAYEROBJECT_NUM, 100, "MNPlayerPool" );

	m_htPlayer = new CYHHashTable<CMNPlayer>;
	m_htPlayer->Initialize( MAX_PLAYEROBJECT_NUM );

	m_pMNHero	= NULL;
}

void CMNPlayerManager::Release()
{
	SAFE_DELETE( m_mpPlayerPool );
	m_htPlayer->RemoveAll();
	SAFE_DELETE( m_htPlayer );
}


void CMNPlayerManager::AddMNPlayer( MNPLAYER_BASEINFO* pInfo )
{
	CMNPlayer* pMNPlayer = m_mpPlayerPool->Alloc();
	pMNPlayer->Init( pInfo );

	m_htPlayer->Add( pMNPlayer, pMNPlayer->GetID() );
}

void CMNPlayerManager::DeleteMNPlayer( DWORD dwObjectID )
{
	CMNPlayer* pMNPlayer = m_htPlayer->GetData( dwObjectID );

	m_htPlayer->Remove( dwObjectID );
	m_mpPlayerPool->Free( pMNPlayer );
}

void CMNPlayerManager::DeleteMNPlayer( CMNPlayer* pMNPlayer )
{
	m_htPlayer->Remove( pMNPlayer->GetID() );
	m_mpPlayerPool->Free( pMNPlayer );
}


void CMNPlayerManager::DeleteAllMNPlayer()
{
	m_htPlayer->SetPositionHead();
	CMNPlayer* pMNPlayer;
	while( pMNPlayer = m_htPlayer->GetData() )
	{
		m_mpPlayerPool->Free( pMNPlayer );
	}

	m_htPlayer->RemoveAll();
}


void CMNPlayerManager::SetMNHero( CMNPlayer* pHero )
{
	m_pMNHero = pHero;
	m_dwMNHeroId = pHero->GetID();
}


CMNPlayer* CMNPlayerManager::GetMNPlayer( DWORD dwObjectID )
{
	return m_htPlayer->GetData( dwObjectID );
}