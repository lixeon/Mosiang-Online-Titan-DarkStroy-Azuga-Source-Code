// QuestExecute_Count.cpp: implementation of the CQuestExecute_Count class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestExecute_Count.h"

#include "QuestScriptLoader.h"
#ifdef _MAPSERVER_
#include "QuestGroup.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestExecute_Count::CQuestExecute_Count( DWORD dwExecuteKind, CStrTokenizer* pTokens, DWORD dwQuestIdx, DWORD dwSubQuestIdx )
: CQuestExecute( dwExecuteKind, pTokens, dwQuestIdx, dwSubQuestIdx )
{
	m_dwMaxCount = m_dwWeaponKind = m_dwParam1 = 0;
	GetScriptParam( m_dwRealSubQuestIdx, pTokens );	
	GetScriptParam( m_dwMaxCount, pTokens );
	if( m_dwExecuteKind == eQuestExecute_AddCountFQW || m_dwExecuteKind == eQuestExecute_AddCountFW )
		GetScriptParam( m_dwWeaponKind, pTokens );
	if( m_dwExecuteKind == eQuestExecute_LevelGap || m_dwExecuteKind == eQuestExecute_MonLevel )
	{
		GetScriptParam( m_dwWeaponKind, pTokens );
		GetScriptParam( m_dwParam1, pTokens );
	}
}

CQuestExecute_Count::~CQuestExecute_Count()
{
}

BOOL CQuestExecute_Count::Execute( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest )
{
#ifdef _MAPSERVER_
	switch( m_dwExecuteKind )
	{
	case eQuestExecute_AddCount:
		{
			pQuestGroup->AddCount( m_dwQuestIdx, m_dwRealSubQuestIdx, m_dwMaxCount );
		}
		break;
//	case eQuestExecute_MinusCount:
//		{
//			pQuestGroup->ChangeSubQuestValue( m_dwQuestIdx, m_dwSubQuestIdx, m_dwMaxCount, eQuestValue_Minus );
//		}
//		break;

	case eQuestExecute_AddCountFQW:
		{
			pQuestGroup->AddCountFromQWeapon( m_dwQuestIdx, m_dwRealSubQuestIdx, m_dwMaxCount, m_dwWeaponKind );
		}
		break;
	case eQuestExecute_AddCountFW:
		{
			pQuestGroup->AddCountFromWeapon( m_dwQuestIdx, m_dwRealSubQuestIdx, m_dwMaxCount, m_dwWeaponKind );
		}
		break;
	case eQuestExecute_LevelGap:
		{
			pQuestGroup->AddCountFromLevelGap( m_dwQuestIdx, m_dwRealSubQuestIdx, m_dwMaxCount, m_dwWeaponKind, m_dwParam1 );
		}
		break;
	case eQuestExecute_MonLevel:
		{
			pQuestGroup->AddCountFromMonLevel( m_dwQuestIdx, m_dwRealSubQuestIdx, m_dwMaxCount, m_dwWeaponKind, m_dwParam1 );
		}
		break;
	}
#endif
	return TRUE;
}
