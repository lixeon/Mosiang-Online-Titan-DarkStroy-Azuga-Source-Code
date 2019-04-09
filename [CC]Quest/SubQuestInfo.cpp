
// SubQuestInfo.cpp: implementation of the CSubQuestInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SubQuestInfo.h"

#include "QuestLimit.h"
#include "QuestTrigger.h"
#include "QuestNpcScript.h"
#include "QuestNpc.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSubQuestInfo::CSubQuestInfo()
{
	m_dwMaxCount = 0;
}

CSubQuestInfo::~CSubQuestInfo()
{
	Release();
}

void CSubQuestInfo::Release()
{
	CQuestLimit* pLimit = NULL;
	PTRLISTPOS pos = m_QuestLimitList.GetHeadPosition();
	while( pos )
	{
		pLimit = (CQuestLimit*)m_QuestLimitList.GetNext( pos );
		if( pLimit )
		{
			delete pLimit;
		}
	}
	m_QuestLimitList.RemoveAll();

	CQuestTrigger* pTrigger = NULL;
	pos = m_QuestTriggerList.GetHeadPosition();	
	while( pos )
	{
		pTrigger = (CQuestTrigger*)m_QuestTriggerList.GetNext( pos );
		if( pTrigger )
		{
			delete pTrigger;
		}
	}
	m_QuestTriggerList.RemoveAll();	

	CQuestNpcScript* pNpcScript = NULL;
	pos = m_QuestNpcScriptList.GetHeadPosition();	
	while( pos )
	{
		pNpcScript = (CQuestNpcScript*)m_QuestNpcScriptList.GetNext( pos );
		if( pNpcScript )
		{
			delete pNpcScript;
		}
	}
	m_QuestNpcScriptList.RemoveAll();
	
	CQuestNpc* pQuestNpc = NULL;
	pos = m_QuestNpcList.GetHeadPosition();	
	while( pos )
	{
		pQuestNpc = (CQuestNpc*)m_QuestNpcList.GetNext( pos );
		if( pQuestNpc )
		{
			delete pQuestNpc;
		}
	}
	m_QuestNpcList.RemoveAll();
}

void CSubQuestInfo::AddQuestLimit( CQuestLimit* pQuestLimit )
{
	if( !pQuestLimit ) return;
	m_QuestLimitList.AddTail( pQuestLimit );
}

void CSubQuestInfo::AddQuestTrigger( CQuestTrigger* pQuestTrigger )
{
	if( !pQuestTrigger ) return;	
	m_QuestTriggerList.AddTail( pQuestTrigger );
}

void CSubQuestInfo::AddQuestNpcScipt( CQuestNpcScript* pQuestNpcScript )
{
	if( !pQuestNpcScript ) return;
	m_QuestNpcScriptList.AddTail(  pQuestNpcScript );
}

void CSubQuestInfo::AddQuestNpc( CQuestNpc* pQuestNpc )
{
	if( !pQuestNpc ) return;
	m_QuestNpcList.AddTail(  pQuestNpc );
}

void CSubQuestInfo::OnQuestEvent( PLAYERTYPE* pPlayer, CQuestGroup* pQuestGroup, CQuest* pQuest, CQuestEvent* pQuestEvent )
{
	// limit
	PTRLISTPOS pos = m_QuestLimitList.GetHeadPosition();
	CQuestLimit* pLimit = NULL;
	while( pos )
	{
		pLimit = (CQuestLimit*)m_QuestLimitList.GetNext( pos );
		if( pLimit )
		if( !pLimit->CheckLimit( pPlayer, pQuestGroup, pQuest ) )
			return;
	}
	
	// trigger
	pos = m_QuestTriggerList.GetHeadPosition();
	CQuestTrigger* pTrigger = NULL;
	while( pos )
	{
		pTrigger = (CQuestTrigger*)m_QuestTriggerList.GetNext( pos );
		if( pTrigger )
			pTrigger->OnQuestEvent( pPlayer, pQuestGroup, pQuest, pQuestEvent );
	}
}

BOOL CSubQuestInfo::IsNpcRelationQuest( DWORD dwNpcIdx )
{
	DWORD dwQuestIdx, dwSubQuestIdx;
	dwQuestIdx = dwSubQuestIdx = 0;

	DWORD rt = 0;
	// npcidx is same?
	CQuestNpcScript* pNpcScript = NULL;
	PTRLISTPOS pos = m_QuestNpcScriptList.GetHeadPosition();	
	while( pos )
	{
		pNpcScript = (CQuestNpcScript*)m_QuestNpcScriptList.GetNext( pos );
		if( pNpcScript )
		if( rt = pNpcScript->IsSameNpc( dwNpcIdx, &dwQuestIdx, &dwSubQuestIdx ) )
			break;
	}
	if( rt == 0 )
		return FALSE;

	// if npcidx is same, check limit
	pos = m_QuestLimitList.GetHeadPosition();
	CQuestLimit* pLimit = NULL;
	while( pos )
	{
		pLimit = (CQuestLimit*)m_QuestLimitList.GetNext( pos );
		if( pLimit )
		if( !pLimit->CheckLimit( dwQuestIdx, dwSubQuestIdx ) )
			return FALSE;
	}

	return TRUE;
}

DWORD CSubQuestInfo::GetNpcScriptPage( DWORD dwNpcIdx )
{
	DWORD dwQuestIdx, dwSubQuestIdx;
	dwQuestIdx = dwSubQuestIdx = 0;

	DWORD dwPage = 0;
	// npcidx is same?
	CQuestNpcScript* pNpcScript = NULL;
	PTRLISTPOS pos = m_QuestNpcScriptList.GetHeadPosition();	
	while( pos )
	{
		pNpcScript = (CQuestNpcScript*)m_QuestNpcScriptList.GetNext( pos );
		if( pNpcScript )
		if( dwPage = pNpcScript->IsSameNpc( dwNpcIdx, &dwQuestIdx, &dwSubQuestIdx ) )
			break;
	}
	if( dwPage == 0 )
		return dwPage;

	// if npcidx is same, check limit
	pos = m_QuestLimitList.GetHeadPosition();
	CQuestLimit* pLimit = NULL;
	while( pos )
	{
		pLimit = (CQuestLimit*)m_QuestLimitList.GetNext( pos );
		if( pLimit )
		if( !pLimit->CheckLimit( dwQuestIdx, dwSubQuestIdx ) )
			return 0;	// limit
	}

	return dwPage;
}

DWORD CSubQuestInfo::GetNpcMarkType( DWORD dwNpcIdx )
{
	DWORD dwQuestIdx, dwSubQuestIdx;
	dwQuestIdx = dwSubQuestIdx = 0;

	DWORD dwNpcMark = 0;
	// npcidx is same
	CQuestNpcScript* pNpcScript = NULL;
	PTRLISTPOS pos = m_QuestNpcScriptList.GetHeadPosition();	
	while( pos )
	{
		pNpcScript = (CQuestNpcScript*)m_QuestNpcScriptList.GetNext( pos );
		if( pNpcScript )
		if( dwNpcMark = pNpcScript->IsSameNpcMark( dwNpcIdx, &dwQuestIdx, &dwSubQuestIdx ) )
			break;
	}
	if( dwNpcMark == 0 )
		return dwNpcMark;

	// if npcidx is same, check limit
	pos = m_QuestLimitList.GetHeadPosition();
	CQuestLimit* pLimit = NULL;
	while( pos )
	{
		pLimit = (CQuestLimit*)m_QuestLimitList.GetNext( pos );
		if( pLimit )
		if( !pLimit->CheckLimit( dwQuestIdx, dwSubQuestIdx ) )
			return 0;	// limit
	}

	return dwNpcMark;
}

int CSubQuestInfo::IsQuestState( DWORD dwNpcIdx )
{
	DWORD dwQuestIdx, dwSubQuestIdx;
	dwQuestIdx = dwSubQuestIdx = 0;

	DWORD dwPage = 0;
	// npcidx is same?
	CQuestNpcScript* pNpcScript = NULL;
	PTRLISTPOS pos = m_QuestNpcScriptList.GetHeadPosition();	
	while( pos )
	{
		pNpcScript = (CQuestNpcScript*)m_QuestNpcScriptList.GetNext( pos );
		if( pNpcScript )
		if( dwPage = pNpcScript->IsSameNpc( dwNpcIdx, &dwQuestIdx, &dwSubQuestIdx ) )
			break;
	}
	if( dwPage == 0 )
		return 0;
	
	// if check limit
	pos = m_QuestLimitList.GetHeadPosition();
	CQuestLimit* pLimit = NULL;
	while( pos )
	{
		pLimit = (CQuestLimit*)m_QuestLimitList.GetNext( pos );
		if( pLimit )
		if( !pLimit->CheckLimit( dwQuestIdx, dwSubQuestIdx ) )
			return 0;	// limit
	}

	if( dwSubQuestIdx == 0 )	return 1;			// 수행가능한 협행
	else						return 2;			// 수행중인 협행			

	return 0;		// 아무것두 아니다.	
}

BOOL CSubQuestInfo::CheckLimitCondition( DWORD dwQuestIdx )
{
	PTRLISTPOS pos = m_QuestLimitList.GetHeadPosition();
	CQuestLimit* pLimit = NULL;
	while( pos )
	{
		pLimit = (CQuestLimit*)m_QuestLimitList.GetNext( pos );
		if( pLimit )
		if( !pLimit->CheckLimit( dwQuestIdx, 0 ) )
			return FALSE;	// limit
	}
	return TRUE;	
}