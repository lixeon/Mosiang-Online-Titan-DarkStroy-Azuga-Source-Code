// QuestInfo.cpp: implementation of the CQuestInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestInfo.h"

#include "SubQuestInfo.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestInfo::CQuestInfo( DWORD dwQuestIdx )
{
	m_dwQuestIdx = dwQuestIdx;
	m_dwSubQuestCount = 0;
	m_dwEndParam = 0;
	m_SubQuestTable.Initialize( MAX_SUBQUEST );
}

CQuestInfo::~CQuestInfo()
{
	Release();
}

void CQuestInfo::Release()
{
	CSubQuestInfo* pSubQuestInfo = NULL;
	m_SubQuestTable.SetPositionHead();
	while( pSubQuestInfo = m_SubQuestTable.GetData() )
	{
		delete pSubQuestInfo;
	}
	m_SubQuestTable.RemoveAll();
}

void CQuestInfo::AddSubQuestInfo( DWORD dwSubQuestIdx, CSubQuestInfo* pSubQuest )
{
	if( dwSubQuestIdx >= MAX_SUBQUEST ) return;
	m_SubQuestTable.Add( pSubQuest, dwSubQuestIdx );
	++m_dwSubQuestCount;
}
