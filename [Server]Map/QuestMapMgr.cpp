// QuestMapMgr.cpp: implementation of the CQuestMapMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestMapMgr.h"
#include "Player.h"
#include "RecallManager.h"
#include "ChannelSystem.h"
#include "QuestManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CQuestMapMgr);

CQuestMapMgr::CQuestMapMgr()
{
	m_bQuestMap = FALSE;
}

CQuestMapMgr::~CQuestMapMgr()
{
}

BOOL CQuestMapMgr::Init( WORD wMapNum )
{
	//if( QUESTMAPNUM1 == wMapNum || QUESTMAPNUM2 == wMapNum || QUESTMAPNUM3 == wMapNum )
	if( g_pServerSystem->GetMap()->IsMapKind(eQuestRoom, wMapNum) )
	{
		m_bQuestMap = TRUE;
		CHANNELSYSTEM->InitQuestMapChannel();
	}
	else
		m_bQuestMap = FALSE;

	return m_bQuestMap;
}

void CQuestMapMgr::RemovePlayer( CPlayer* pPlayer )
{
	if( !m_bQuestMap )	return;

	RECALLMGR->DeleteQuestRecallMonster( pPlayer->GetChannelID() );
	CHANNELSYSTEM->DestroyQuestMapChannel( pPlayer->GetChannelID() );
}

void CQuestMapMgr::DiePlayer( CPlayer* pPlayer )
{
	if( !m_bQuestMap )	return;

	pPlayer->SetReadyToRevive( FALSE );
//	QUESTMGR->AddQuestEvent( pPlayer, &CQuestEvent( eQuestEvent_Die, g_pServerSystem->GetMapNum(), 0 ) );
}
