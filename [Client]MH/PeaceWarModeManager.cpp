// PeaceWarModeManager.cpp: implementation of the CPeaceWarModeManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PeaceWarModeManager.h"

#include "MoveManager.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "AppearanceManager.h"
#include "GameIn.h"
#include "MHTimeManager.h"


#include "CharStateDialog.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CPeaceWarModeManager);
CPeaceWarModeManager::CPeaceWarModeManager()
{
	m_dwBeginCheckTime	= 0;
}

CPeaceWarModeManager::~CPeaceWarModeManager()
{

}

void CPeaceWarModeManager::InitPeace_WarMode(CPlayer* pPlayer)
{
	BOOL bPeace = pPlayer->GetCharacterTotalInfo()->bPeace;
	if(bPeace == TRUE)
		//MOVEMGR->SetWalkMode(pPlayer);
		MOVEMGR->SetRunMode(pPlayer);		// Â¹Â«ÃÂ¶Â°Ã‡ Â¶Ã™Â±Ã¢Â·ÃŽ... [6/3/2003]
	else
		MOVEMGR->SetRunMode(pPlayer);	
	
//	Peace_WarMode( pPlayer, bPeace );
//KES CHARSTATEDLG 031016
//	if( pPlayer == HERO )
//	if( GAMEIN->GetCharStateDialog() )
//		GAMEIN->GetCharStateDialog()->SetPeaceWarMode( bPeace );
}

void CPeaceWarModeManager::ToggleHeroPeace_WarMode()
{
	// Â»Ã³Ã…Ã‚Â°Â¡ None ?ÃÂ¶Â§Â¸Â¸ Â¹Ã™Â²ÃœÂ¼Ã¶ ?Ã–Â´Ã™.
	if( HERO->GetState() != eObjectState_None &&
		HERO->GetState() != eObjectState_Move &&
		HERO->GetState() != eObjectState_Immortal )
		return;


	BOOL bPeace = HERO->GetCharacterTotalInfo()->bPeace;

	if( bPeace == FALSE && HERO->InTitan() == TRUE )
		return;

	bPeace = !bPeace;	// Ã…Ã¤Â±Ã› Â½ÃƒÃ…Â´

	//for DoCheckForPeaceMode()
	//SW050829
	if( bPeace == FALSE )
	{
		m_dwBeginCheckTime = gCurTime;
	}
	else
	{
		//¸¸¾à ÁõÆø¹öÇÁ³ª ½ºÅÏÀÌ °É·ÁÀÖ´Ù¸é ÆòÈ­¸ðµå°¡ µÇÁö ¾Ê´Â´Ù.
		for( int i=0 ; i < eSpecialState_Max ; ++i )
		{
			if( HERO->IsInSpecialState( i ) == TRUE )
			{
				m_dwBeginCheckTime = gCurTime;
				return;
			}
		}
	}

	if( MOVEMGR->IsMoving(HERO) )
		MOVEMGR->HeroMoveStop();


// Â¸Â¸Â¾Ã  ?Ã¼Ã…ÃµÂ¸Ã°ÂµÃ¥Â·ÃŽ Â¹Ã™Â²Ã™Â´Ã‚ÂµÂ¥ Â°ÃˆÂ±Ã¢?ÃŒÂ¸Ã© Â¶Ã™Â±Ã¢Â·ÃŽ Â¹Ã™Â²Ã›Â´Ã™.
//	if(bPeace==FALSE && MOVEMGR->GetMoveMode(HERO) == eMoveMode_Walk)
//	{
//		MOVEMGR->ToggleHeroMoveMode();
//	}
// Â¸Â¸Â¾Ã  Ã†Ã²ÃˆÂ­Â¸Ã°ÂµÃ¥Â·ÃŽ Â¹Ã™Â²Ã™Â´Ã‚ÂµÂ¥ Â¶Ã™Â±Ã¢?ÃŒÂ¸Ã© Â°ÃˆÂ±Ã¢Â·ÃŽ Â¹Ã™Â²Ã›Â´Ã™.
//	if(bPeace==TRUE && MOVEMGR->GetMoveMode(HERO) == eMoveMode_Run)
//	{
//		MOVEMGR->ToggleHeroMoveMode();
//	}

	Peace_WarMode(HERO,bPeace);

	// Â¼Â­Â¹Ã¶Â¿Â¡ Â¸ÃžÂ¼Â¼ÃÃ¶ ÂºÂ¸Â³Â¿
	MSGBASE msg;
	msg.Category = MP_PEACEWARMODE;
	if(bPeace)
		msg.Protocol = MP_PEACEWARMODE_PEACE;
	else
		msg.Protocol = MP_PEACEWARMODE_WAR;
	msg.dwObjectID = HEROID;
	NETWORK->Send(&msg,sizeof(msg));

//KES CHARSTATEDLG 031016
//	if(GAMEIN->GetCharStateDialog())
//		GAMEIN->GetCharStateDialog()->SetPeaceWarMode( bPeace );
}

void CPeaceWarModeManager::Peace_WarMode(CPlayer* pPlayer, BOOL bPeace)
{
	pPlayer->SetPeaceMode(bPeace?true:false);
	if(bPeace) //Ã†Ã²ÃˆÂ­ Â¸Ã°ÂµÃ¥
	{
		// ?ÃšÂ¼Â¼Â¸Â¦ Â¹Ã™Â²Ã™Â´Ã‚ Ã‡Ã”Â¼Ã¶Â¸Â¦ ÃˆÂ£ÃƒÃ¢Ã‡Ã‘Â´Ã™.
		if(pPlayer->GetObjectKind() == eObjectKind_Player)
		{
			// ±ÇÀº ¿þÆùÀÌ ¾Æ´Ï¹Ç·Î...
			/* 2008. 6. 30. CBH - ÆòÈ­¸ðµåÀÏ¶§ ¹«±â ¸ðµÎ º¸ÀÌ°Ô ¼öÁ¤
			if( //pPlayer->GetWeaponEquipType() == WP_GWUN ||
				pPlayer->GetWeaponEquipType() == WP_AMGI )
				APPEARANCEMGR->HideWeapon(pPlayer);
				*/
			(pPlayer)->SetBaseMotion();
		}
	}
	else //?Ã¼Ã…Ãµ Â¸Ã°ÂµÃ¥
	{
		if(pPlayer->GetObjectKind() == eObjectKind_Player)
		{			
			APPEARANCEMGR->ShowWeapon(pPlayer);
			(pPlayer)->SetBaseMotion();
		}
	}
	if(pPlayer->GetState() == eObjectState_None)
		OBJECTSTATEMGR->EndObjectState(pPlayer,eObjectState_None);
}

BOOL CPeaceWarModeManager::IsPeaceMode(CPlayer* pPlayer)
{
	return pPlayer->GetCharacterTotalInfo()->bPeace;
}

void CPeaceWarModeManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	CPlayer* pObject;
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	pObject = (CPlayer*)OBJECTMGR->GetObject(pmsg->dwObjectID);
	ASSERT(pObject);
	if(pObject==NULL)
		return;
	ASSERT(pObject->GetObjectKind() == eObjectKind_Player);	
	
	switch(Protocol) 
	{
	case MP_PEACEWARMODE_PEACE:
		{
			Peace_WarMode(pObject,TRUE);
		}
		break;
	case MP_PEACEWARMODE_WAR:
		{
			Peace_WarMode(pObject,FALSE);
		}
		break;
	}
}

//SW050810 ÆòÈ­¸ðµå ÀÚµ¿º¯È¯
void CPeaceWarModeManager::DoCheckForPeaceMode(CPlayer* pPlayer)
{
/*
	if( pPlayer->GetCharacterTotalInfo()->bPeace == FALSE )
	{
		if( pPlayer->GetState() == eObjectState_None )	// ÀüÅõ»óÅÂÀÌ°í Çàµ¿ÀÌ ¾øÀ¸¸é
		{
			if( gCurTime - m_dwBeginCheckTime > MODEVALIDTIME )
				ToggleHeroPeace_WarMode();
		}
	}
	else
		m_dwBeginCheckTime = gCurTime;
*/

	if( pPlayer->InTitan() == TRUE )
		return;
	//SW050829 Á¶°Ç: pPlayer->GetState() == eObjectState_None ÀÏ¶§¸¸ ÆòÈ­¸ðµå ½Ã°£ Ã¼Å©
	if( pPlayer->GetCharacterTotalInfo()->bPeace == FALSE )
	{
		if( pPlayer->GetState() == eObjectState_None )//			|| pPlayer->GetState() == eObjectState_Deal )
		{
			if( gCurTime - m_dwBeginCheckTime > MODEVALIDTIME )
			{
				ToggleHeroPeace_WarMode();				
			}
		}
		else
		{
			m_dwBeginCheckTime = gCurTime;
		}
	}
/*
	if( pPlayer->GetCharacterTotalInfo()->bPeace == TRUE ) return;
	
	if( pPlayer->GetState() == eObjectState_None )	// ÀüÅõ»óÅÂÀÌ°í Çàµ¿ÀÌ ¾øÀ¸¸é
	{
		if( !m_bStartCheck )
		{
			m_dwBeginCheckTime = gCurTime;
			m_bStartCheck	= TRUE;
		}				
		
		if( m_bStartCheck )
		{
			if( gCurTime - m_dwBeginCheckTime > MODEVALIDTIME )
			{
				ToggleHeroPeace_WarMode();
				m_bStartCheck = FALSE;
				return;
			}		
		}
	}
	else
		m_bStartCheck = FALSE;
		*/
}
