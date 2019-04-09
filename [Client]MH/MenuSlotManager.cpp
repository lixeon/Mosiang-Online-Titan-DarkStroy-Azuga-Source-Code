// MenuSlotManager.cpp: implementation of the CMenuSlotManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MenuSlotManager.h"
#include "./Interface/cWindowManager.h"
#include "Ability.h"
#include "AbilityIcon.h"
#include "AbilityInfo.h"
#include "ObjectManager.h"
#include "MenuIcon.h"
#include "MenuSlotDialog.h"
#include "GameIn.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CMenuSlotManager)

CMenuSlotManager::CMenuSlotManager()
{
	m_IconIndexCreator.Init( MAX_ITEMICON_NUM, IG_MENUSLOTITEM_STARTINDEX );
}

CMenuSlotManager::~CMenuSlotManager()
{
	m_IconIndexCreator.Release();
}

void CMenuSlotManager::Init()
{

}

void CMenuSlotManager::Release()
{

}

BOOL CMenuSlotManager::AddMenuIcon( WORD wPage, WORD wPos, cIcon* pSrcIcon )
{
	if( pSrcIcon->GetType() != WT_ABILITYICON )
		return FALSE;

	CAbilityIcon* pAbilIcon = (CAbilityIcon*)pSrcIcon;
	if( pAbilIcon->GetAbilityInfo()->GetUseType() == eAUT_Passive )
		return FALSE;

	//pAbilIcon에 Slot Position이 있으면 MoveMenuIcon...
//	GAMEIN->GetMenuSlotDialog()->IsThisAbilityIconInSlot( pAbilIcon->geti)

	CMenuIcon* pMenuIcon = new CMenuIcon;
	pMenuIcon->Init( 0, 0, DEFAULT_ICONSIZE, DEFAULT_ICONSIZE, pAbilIcon->GetBasicImage(), m_IconIndexCreator.GenerateIndex() );
	pMenuIcon->SetAbilityIdx( pAbilIcon->GetAbilityInfo()->GetIdx() );
	
	WINDOWMGR->AddWindow( (cWindow*)pMenuIcon );

	if( GAMEIN->GetMenuSlotDialog()->AddMenuIcon( wPage, wPos, pMenuIcon ) == TRUE )
	{
		return TRUE;
	}
	else
	{
		//만든 아이콘을 지우자.
		return FALSE;
	}
}

BOOL CMenuSlotManager::DeleteMenuIcon( CMenuIcon* pMenuIcon )
{
	if( GAMEIN->GetMenuSlotDialog()->DeleteMenuIcon( pMenuIcon ) == TRUE )
	{
		//ablility update		

		pMenuIcon->SetDisable( TRUE );
		pMenuIcon->SetActive( FALSE );
		WINDOWMGR->AddListDestroyWindow( (cWindow*)pMenuIcon );

		return TRUE;
	}
	else
	{
		return FALSE;		
	}
}

BOOL CMenuSlotManager::ExchangeMenuIcon( CMenuIcon* pMenuIcon1, CMenuIcon* pMenuIcon2 )
{
	if( GAMEIN->GetMenuSlotDialog()->ExchangeMenuIcon( pMenuIcon1, pMenuIcon2 ) )
	{
		//abliltiyIcon의 SlotPos바꾸기
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CMenuSlotManager::MoveMenuIcon( WORD wPage, WORD wPos, CMenuIcon* pMenuIcon )
{
	if( GAMEIN->GetMenuSlotDialog()->MoveMenuIcon( wPage, wPos, pMenuIcon ) )
	{
		//abliltiyIcon의 SlotPos바꾸기
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CMenuSlotManager::UseMenuIcon( CMenuIcon* pMenuIcon )
{
	CAbility* pAbility = HERO->GetAbilityGroup()->GetAbility( pMenuIcon->GetAbilityIdx() );

	if( pAbility == NULL )
		return FALSE;	//잘못 등록 되어 있다.

	//보안.... 서버에서 한번더 체크하고 있을까?

	pAbility->Use();

	return TRUE;
}