// QuestExecute_Item.cpp: implementation of the CQuestExecute_Item class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestExecute_Item.h"

#include "QuestScriptLoader.h"
#ifdef _MAPSERVER_
#include "QuestGroup.h"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestExecute_Item::CQuestExecute_Item( DWORD dwExecuteKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
: CQuestExecute( dwExecuteKind, pTokens, dwQuestIdx, dwSubQuestIdx )
{
	m_dwItemKind = m_dwItemNum = m_dwItemProbability = m_dwWeaponKind = 0;

	switch( dwExecuteKind )
	{
	case eQuestExecute_GiveItem:
	case eQuestExecute_GiveQuestItem:
		{
			GetScriptParam( m_dwItemKind, pTokens );
			GetScriptParam( m_dwItemNum, pTokens );
		}
		break;

	case eQuestExecute_GiveMoney:
	case eQuestExecute_TakeMoney:
	case eQuestExecute_TakeExp:
	case eQuestExecute_TakeSExp:
		{
			GetScriptParam( m_dwItemNum, pTokens );
		}
		break;

	case eQuestExecute_TakeQuestItem:
	case eQuestExecute_TakeItem:
		{
			GetScriptParam( m_dwItemKind, pTokens );
			GetScriptParam( m_dwItemNum, pTokens );
			GetScriptParam( m_dwItemProbability, pTokens );
		}
		break;

	case eQuestExecute_TakeQuestItemFQW:
		{
			GetScriptParam( m_dwItemKind, pTokens );
			GetScriptParam( m_dwItemNum, pTokens );
			GetScriptParam( m_dwItemProbability, pTokens );
			GetScriptParam( m_dwWeaponKind, pTokens );
		}
		break;

	case eQuestExecute_TakeQuestItemFW:
		{
			GetScriptParam( m_dwItemKind, pTokens );
			GetScriptParam( m_dwItemNum, pTokens );
			GetScriptParam( m_dwItemProbability, pTokens );
			GetScriptParam( m_dwWeaponKind, pTokens );
		}
		break;
		
	case eQuestExecute_TakeMoneyPerCount:
		{
			GetScriptParam( m_dwItemKind, pTokens );
			GetScriptParam( m_dwItemNum, pTokens );
		}
		break;
	}
}

CQuestExecute_Item::~CQuestExecute_Item()
{
}

BOOL CQuestExecute_Item::Execute( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest )
{
#ifdef _MAPSERVER_
	switch( m_dwExecuteKind )
	{
	case eQuestExecute_TakeQuestItem:
		{
			pQuestGroup->TakeQuestItem( pPlayer, m_dwQuestIdx, m_dwSubQuestIdx, m_dwItemKind,
										m_dwItemNum, m_dwItemProbability );
		}
		break;
	case eQuestExecute_GiveQuestItem:
		{
			pQuestGroup->GiveQuestItem( pPlayer, m_dwQuestIdx, m_dwItemKind, m_dwItemNum );
		}
		break;
	case eQuestExecute_GiveItem:
		{
			pQuestGroup->GiveItem( pPlayer, (WORD)m_dwItemKind, m_dwItemNum );
		}
		break;
	case eQuestExecute_GiveMoney:
		{
			pQuestGroup->GiveMoney( pPlayer, m_dwItemNum );
		}
		break;
	case eQuestExecute_TakeItem:
		{
			return pQuestGroup->TakeItem( pPlayer, (WORD)m_dwItemKind, m_dwItemNum, m_dwItemProbability );
		}
		break;
	case eQuestExecute_TakeMoney:
		{
			pQuestGroup->TakeMoney( pPlayer, m_dwItemNum );
		}
		break;
	case eQuestExecute_TakeExp:
		{
			pQuestGroup->TakeExp( pPlayer, m_dwItemNum );
		}
		break;
	case eQuestExecute_TakeSExp:
		{
			pQuestGroup->TakeSExp( pPlayer, m_dwItemNum );
		}
		break;
	case eQuestExecute_TakeQuestItemFQW:
		{
			pQuestGroup->TakeQuestItemFromQWeapon( pPlayer, m_dwQuestIdx, m_dwSubQuestIdx, m_dwItemKind,
												   m_dwItemNum, m_dwItemProbability, m_dwWeaponKind );
		}
		break;
	case eQuestExecute_TakeQuestItemFW:
		{
			pQuestGroup->TakeQuestItemFromWeapon( pPlayer, m_dwQuestIdx, m_dwSubQuestIdx, m_dwItemKind,
												  m_dwItemNum, m_dwItemProbability, m_dwWeaponKind );
		}
		break;
	case eQuestExecute_TakeMoneyPerCount:
		{
			pQuestGroup->TakeMoneyPerCount( pPlayer, m_dwItemKind, m_dwItemNum );
		}
		break;
	}
#endif
	return TRUE;
}
