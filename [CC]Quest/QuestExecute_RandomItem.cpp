// QuestExecute_RandomItem.cpp: implementation of the CQuestExecute_RandomItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestExecute_RandomItem.h"

#include "QuestScriptLoader.h"
#ifdef _MAPSERVER_
#include "QuestGroup.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestExecute_RandomItem::CQuestExecute_RandomItem( DWORD dwExecuteKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
: CQuestExecute( dwExecuteKind, pTokens, dwQuestIdx, dwSubQuestIdx )
{
	GetScriptParam( m_dwMaxItemCount, pTokens );
	GetScriptParam( m_dwRandomItemCount, pTokens );
	
	m_pRandomItem = new RANDOMITEM[m_dwMaxItemCount];
	for( DWORD i = 0; i < m_dwMaxItemCount; ++i )
	{
		GetScriptParam( m_pRandomItem[i].wItemIdx, pTokens );
		GetScriptParam( m_pRandomItem[i].wItemNum, pTokens );
		GetScriptParam( m_pRandomItem[i].wPercent, pTokens );
	}
}

CQuestExecute_RandomItem::~CQuestExecute_RandomItem()
{
	if( m_pRandomItem )
	{
		delete [] m_pRandomItem;
		m_pRandomItem = NULL;
	}
}

BOOL CQuestExecute_RandomItem::Execute( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest )
{
#ifdef _MAPSERVER_
	switch( m_dwExecuteKind )
	{
	case eQuestExecute_RandomTakeItem:
		{
			RANDOMITEM* pItem = NULL;

			for( DWORD i = 0; i < m_dwRandomItemCount; ++i )
			{
				pItem = GetRandomItem();
				if( pItem )
					pQuestGroup->TakeItem( pPlayer, pItem->wItemIdx, pItem->wItemNum, MAX_QUEST_PROBABILITY );
			}
		}
		break;
	}
#endif	
	return TRUE;
}

RANDOMITEM* CQuestExecute_RandomItem::GetRandomItem()
{
	WORD RandRate = rand()%10001;	
	WORD FromPercent = 0;
	WORD ToPercent = 0;

	for( DWORD i = 0; i < m_dwMaxItemCount; ++i )
	{
		FromPercent = ToPercent;
		ToPercent += m_pRandomItem[i].wPercent;
		if( FromPercent <= RandRate && RandRate < ToPercent )
		{
			return &m_pRandomItem[i];
		}
	}

	return NULL;
}
