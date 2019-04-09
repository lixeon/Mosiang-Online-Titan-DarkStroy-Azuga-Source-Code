// QuestLimitKind_Level.cpp: implementation of the CQuestLimitKind_Level class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestLimitKind_Level.h"

#include "QuestScriptLoader.h"
#ifdef _MAPSERVER_
#include "Player.h"
#else
#include "ObjectManager.h"
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestLimitKind_Level::CQuestLimitKind_Level( DWORD dwLimitKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
: CQuestLimitKind( dwLimitKind, pTokens, dwQuestIdx, dwSubQuestIdx )
{
	m_dwMin = m_dwMax = 0;
	if( m_dwLimitKind == eQuestLimitKind_Level )
	{
		GetScriptParam( m_dwMin, pTokens );
		GetScriptParam( m_dwMax, pTokens );
	}
	else if( m_dwLimitKind == eQuestLimitKind_Money )
	{
		GetScriptParam( m_dwMax, pTokens );
	}
	else if( m_dwLimitKind == eQuestLimitKind_Stage )
	{
		GetScriptParam( m_dwMin, pTokens );
		GetScriptParam( m_dwMax, pTokens );
	}
	else if( m_dwLimitKind == eQuestLimitKind_Attr )
	{
		GetScriptParam( m_dwMin, pTokens );
	}
}

CQuestLimitKind_Level::~CQuestLimitKind_Level()
{
}

BOOL CQuestLimitKind_Level::CheckLimit( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest )
{
#ifdef _MAPSERVER_
	switch( m_dwLimitKind )
	{
	case eQuestLimitKind_Level:
		{
			DWORD dwlevel = pPlayer->GetLevel();
			if( dwlevel >= m_dwMin && dwlevel <= m_dwMax )
				return TRUE;
		}
		break;
	case eQuestLimitKind_Money:
		{
			MONEYTYPE Money = pPlayer->GetMoney();
			if( Money >= m_dwMax )
				return TRUE;
		}
		break;
	case eQuestLimitKind_Stage:
		{
			BYTE bStage = pPlayer->GetStage();
			if( bStage == (BYTE)m_dwMin || bStage == (BYTE)m_dwMax )
				return TRUE;			
		}
		break;
#ifdef _JAPAN_LOCAL_
	case eQuestLimitKind_Attr:
		{
			int nMainAttr = pPlayer->GetMainCharAttr();
			if( nMainAttr == m_dwMin )
				return TRUE;
		}
		break;
#endif
	}
#endif
	return FALSE;
}

BOOL CQuestLimitKind_Level::CheckLimit( DWORD dwQuestIdx, DWORD dwSubQuestIdx )
{
#ifndef _MAPSERVER_
	switch( m_dwLimitKind )
	{
	case eQuestLimitKind_Level:
		{
			DWORD dwlevel = HERO->GetLevel();
			if( dwlevel >= m_dwMin && dwlevel <= m_dwMax )
				return TRUE;
		}
		break;
	case eQuestLimitKind_Money:
		{
			MONEYTYPE Money = HERO->GetMoney();
			if( Money >= m_dwMax )
				return TRUE;
		}
		break;
	case eQuestLimitKind_Stage:
		{
			BYTE bStage = HERO->GetStage();
			if( bStage == (BYTE)m_dwMin || bStage == (BYTE)m_dwMax )
				return TRUE;
		}
		break;
#ifdef _JAPAN_LOCAL_
	case eQuestLimitKind_Attr:
		{
			int	nMainAttr = HERO->GetMainCharAttr();
			if( nMainAttr == m_dwMin )
				return TRUE;
		}
		break;
#endif
	}
#endif
	return FALSE;
}