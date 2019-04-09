// EventMapInfo.cpp: implementation of the CEventMapInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EventMapInfo.h"
#include "ObjectManager.h"
#include "Player.h"
#include "PartyManager.h"
#include "./interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"
#include "GameIn.h"
#include "MPGuageDialog.h"
#include "MiniMapDlg.h"
#include "mhMap.h"
#include "MAINGAME.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CEventMap);

CEventMap::CEventMap()
{
	m_PlayerTable.Initialize( 1000 );
	m_nState = eEventState_None;
	m_dwTime = 0;

	m_dwMapNum = m_dwChannelNum = m_dwTeam = 0;

	m_pCurShowImage = NULL;

	memset( &m_Result, 0, sizeof(MSG_DWORD3) );

	m_dwBeforeMapNum = 0;
}

CEventMap::~CEventMap()
{
	Release();
}

void CEventMap::Init()
{
	m_nState = eEventState_None;

	memset( &m_Result, 0, sizeof(MSG_DWORD3) );

	SCRIPTMGR->GetImage( 60, &m_ImageStart, PFT_HARDPATH );
	SCRIPTMGR->GetImage( 61, &m_ImageWin, PFT_HARDPATH );
	SCRIPTMGR->GetImage( 62, &m_ImageLose, PFT_HARDPATH );
	SCRIPTMGR->GetImage( 76, &m_ImageDraw, PFT_HARDPATH );

	m_vTitlePos.x = (1024 - 512) / 2;
	m_vTitlePos.y = 32;
	m_vTitleScale.x = 1.0f;
	m_vTitleScale.y = 1.0f;
}

void CEventMap::Release()
{
	CPlayer* pPlayer = NULL;
	m_PlayerTable.SetPositionHead();
	while( pPlayer = m_PlayerTable.GetData() )
		m_PlayerTable.Remove( pPlayer->GetID() );
	m_PlayerTable.RemoveAll();
}

void CEventMap::AddHero()
{
	//if( MAP->GetMapNum() != EVENTMAPNUM )	return;
	if( FALSE == MAP->IsMapKind(eEventMap) )	return;
	GAMEIN->GetMiniMapDialog()->SetActive( FALSE );

	if( MAINGAME->GetUserLevel() == eUSERLEVEL_GM )	return;
	WINDOWMGR->CreateMPGuageDlg();
	GAMEIN->GetMPGuageDialog()->ShowEventMap();
	
	if( m_nState == eEventState_Start )
	{
		if(HERO->InTitan())
		{
			OBJECTEFFECTDESC desc(FindEffectNum("t_eff_bmu_A_s.beff"));
			HERO->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1 );
		}
		else
		{
			OBJECTEFFECTDESC desc(FindEffectNum("eff_bmu_A_s.beff"));
			HERO->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc, 1 );
		}
	}	
}

void CEventMap::AddPlayer( CPlayer* pPlayer )
{
	//if( MAP->GetMapNum() != EVENTMAPNUM )	return;
	if( FALSE == MAP->IsMapKind(eEventMap) )	return;
	if( MAINGAME->GetUserLevel() == eUSERLEVEL_GM )	return;

	if( m_nState == eEventState_Start )
	{
		if(pPlayer->InTitan())
		{
			OBJECTEFFECTDESC desc1(FindEffectNum("t_eff_bmu_A_s.beff"));
			OBJECTEFFECTDESC desc2(FindEffectNum("t_eff_bmu_B_s.beff"));

			if( PARTYMGR->IsPartyMember( pPlayer->GetID() ) )
			{
				pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc1, 1 );
			}
			else
			{
				pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc2, 1 );
			}
		}
		else
		{
			OBJECTEFFECTDESC desc1(FindEffectNum("eff_bmu_A_s.beff"));
			OBJECTEFFECTDESC desc2(FindEffectNum("eff_bmu_B_s.beff"));

			if( PARTYMGR->IsPartyMember( pPlayer->GetID() ) )
			{
				pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc1, 1 );
			}
			else
			{
				pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc2, 1 );
			}
		}
	}
	
	if( m_PlayerTable.GetData( pPlayer->GetID() ) )	return;
	m_PlayerTable.Add( pPlayer, pPlayer->GetID() );	
}

BOOL CEventMap::IsEnemy( CObject* pTarget )
{
	//if( MAP->GetMapNum() != EVENTMAPNUM )	return FALSE;
	if( FALSE == MAP->IsMapKind(eEventMap) )	return FALSE;
	if( m_nState != eEventState_Start )	return FALSE;
	if( !PARTYMGR->IsPartyMember( pTarget->GetID() ) )
		return TRUE;

	return FALSE;
}

void CEventMap::Ready()
{
	if( MAINGAME->GetUserLevel() == eUSERLEVEL_GM )	return;

	m_nState = eEventState_Ready;
	m_pCurShowImage = &m_ImageStart;

	m_dwTime = gCurTime;
}

void CEventMap::Start()
{
	if( MAINGAME->GetUserLevel() == eUSERLEVEL_GM )	return;

	if(HERO->InTitan())
	{
		OBJECTEFFECTDESC desc1(FindEffectNum("t_eff_bmu_A_s.beff"));
		OBJECTEFFECTDESC desc2(FindEffectNum("t_eff_bmu_B_s.beff"));

		HERO->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc1, 1 );

		CPlayer* pPlayer = NULL;
		m_PlayerTable.SetPositionHead();
		while( pPlayer = m_PlayerTable.GetData() )
		{
			if( PARTYMGR->IsPartyMember( pPlayer->GetID() ) )
			{
				pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc1, 1 );
			}
			else
			{
				pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc2, 1 );
			}
		}
	}
	else
	{
		OBJECTEFFECTDESC desc1(FindEffectNum("eff_bmu_A_s.beff"));
		OBJECTEFFECTDESC desc2(FindEffectNum("eff_bmu_B_s.beff"));

		HERO->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc1, 1 );

		CPlayer* pPlayer = NULL;
		m_PlayerTable.SetPositionHead();
		while( pPlayer = m_PlayerTable.GetData() )
		{
			if( PARTYMGR->IsPartyMember( pPlayer->GetID() ) )
			{
				pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc1, 1 );
			}
			else
			{
				pPlayer->AddObjectEffect( BATTLE_TEAMEFFECT_ID, &desc2, 1 );
			}
		}
	}

	m_nState = eEventState_Start;

	m_dwTime = gCurTime;
}

void CEventMap::End( MSG_DWORD3* pMsg )
{
	m_nState = eEventState_End;

	m_dwTime = gCurTime;

	memcpy( &m_Result, pMsg, sizeof(MSG_DWORD3) );

	if( m_Result.dwData3 == 0 )			// lose
		m_pCurShowImage = &m_ImageLose;
	else if( m_Result.dwData3 == 1 )	// win
		m_pCurShowImage = &m_ImageWin;
	else if( m_Result.dwData3 == 2 )	// draw
		m_pCurShowImage = &m_ImageDraw;

	HERO->RemoveAllObjectEffect();
	CPlayer* pPlayer = NULL;
	m_PlayerTable.SetPositionHead();
	while( pPlayer = m_PlayerTable.GetData() )
	{
		pPlayer->RemoveAllObjectEffect();
	}
}

void CEventMap::Render()
{
	//if( MAP->GetMapNum() != EVENTMAPNUM )	return;
	if( FALSE == MAP->IsMapKind(eEventMap) )	return;
	if( MAINGAME->GetUserLevel() == eUSERLEVEL_GM )	return;

	DWORD ElapsedTime = gCurTime - m_dwTime;

	switch( m_nState )
	{
	case eEventState_None:
		break;
	case eEventState_Ready:
		{
			if( m_pCurShowImage )
				m_pCurShowImage->RenderSprite( &m_vTitleScale, NULL, 0.0f, &m_vTitlePos, 0xffffffff );

			DWORD RemainTime = 10000 > ElapsedTime ? 10000 - ElapsedTime : 0;
			GAMEIN->GetMPGuageDialog()->SetEventMapTimer( RemainTime, 0 );
		}
		break;
	case eEventState_Start:
		{
			DWORD RemainTime = 600000 > ElapsedTime ? 600000 - ElapsedTime : 0;
			GAMEIN->GetMPGuageDialog()->SetEventMapTimer( RemainTime, 1 );
		}
		break;
	case eEventState_End:
		{
			if( m_pCurShowImage )
				m_pCurShowImage->RenderSprite( &m_vTitleScale, NULL, 0.0f, &m_vTitlePos, 0xffffffff );	
			
			DWORD RemainTime = 10000 > ElapsedTime ? 10000 - ElapsedTime : 0;
			GAMEIN->GetMPGuageDialog()->SetEventMapTimer( RemainTime, 2 );
		}
		break;			
	}
}

BOOL CEventMap::IsEventMap()
{
	//if( MAP->GetMapNum() == EVENTMAPNUM )
	if( MAP->IsMapKind(eEventMap) )
		return TRUE;
	else
		return FALSE;
}
