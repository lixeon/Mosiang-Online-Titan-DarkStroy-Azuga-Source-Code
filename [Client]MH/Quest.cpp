// Quest.cpp: implementation of the CQuest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Quest.h"

#include "..\[CC]Quest\QuestInfo.h"
#include "..\[CC]Quest\SubQuestInfo.h"

#include "ObjectManager.h"
#include "StatusIconDlg.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuest::CQuest( CQuestInfo* pQuestInfo )
{
	m_pQuestInfo = pQuestInfo;
	memset( m_CurSubQuestInfoArray, 0, sizeof(CSubQuestInfo*)*MAX_SUBQUEST );
	m_CurSubQuestInfoArray[0] = m_pQuestInfo->GetSubQuestInfo( 0 );

	m_dwSubQuestCount = m_pQuestInfo->GetSubQuestCount();
	memset( m_SubQuestTable, 0, sizeof(SUBQUEST)*MAX_SUBQUEST );
	m_dwSubQuestFlag = m_dwData = m_dwTime = 0;
	
	m_bCheckTime = FALSE;
	m_CheckType = 0;
	m_CheckTime.value = 0;
}

CQuest::~CQuest()
{
}

void CQuest::Initialize()
{
	memset( m_CurSubQuestInfoArray, 0, sizeof(CSubQuestInfo*)*MAX_SUBQUEST );
	m_CurSubQuestInfoArray[0] = m_pQuestInfo->GetSubQuestInfo( 0 );

	m_dwSubQuestCount = m_pQuestInfo->GetSubQuestCount();
	memset( m_SubQuestTable, 0, sizeof(SUBQUEST)*MAX_SUBQUEST );
	m_dwSubQuestFlag = m_dwData = m_dwTime = 0;

	m_bCheckTime = FALSE;
	m_CheckType = 0;
	m_CheckTime.value = 0;
}

void CQuest::SetMainQuestData( DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime, BYTE bCheckType, DWORD dwCheckTime )
{
	m_CurSubQuestInfoArray[0] = NULL;
	m_dwSubQuestFlag = dwSubQuestFlag;
	m_dwData = dwData;
	m_dwTime = dwTime;	

	m_CheckType = bCheckType;
	m_CheckTime.value = dwCheckTime;
	if( m_CheckType != 0 )
	{
		m_bCheckTime = TRUE;
		if( GetCurTime() < m_CheckTime.value )
            STATUSICONDLG->AddQuestIconCount();
	}
}

void CQuest::SetSubQuestData( DWORD dwSubQuestIdx, DWORD dwSubData, DWORD dwSubTime )
{
	m_CurSubQuestInfoArray[dwSubQuestIdx] = m_pQuestInfo->GetSubQuestInfo( dwSubQuestIdx );
	m_SubQuestTable[dwSubQuestIdx].dwData = dwSubData;
	m_SubQuestTable[dwSubQuestIdx].dwTime = dwSubTime;
	m_SubQuestTable[dwSubQuestIdx].dwMaxCount = m_CurSubQuestInfoArray[dwSubQuestIdx]->GetMaxCount();
}

void CQuest::StartQuest( DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime )
{
	m_CurSubQuestInfoArray[0] = NULL;
	m_dwSubQuestFlag = dwSubQuestFlag;
	m_dwData = dwData;
	m_dwTime = dwTime;
}

void CQuest::EndQuest( DWORD dwSubQuestFlag, DWORD dwData, DWORD dwTime )
{
	if( dwSubQuestFlag == 0 )	// นÝบน
		Initialize();
	else
	{
		m_dwSubQuestFlag = dwSubQuestFlag;
		m_dwData = dwData;
		m_dwTime = dwTime;

		m_bCheckTime = FALSE;
		m_CheckType = 0;
		m_CheckTime.value = 0;
	}
}

void CQuest::StartSubQuest( DWORD dwSubQuestIdx, DWORD dwSubQuestFlag, DWORD dwSubData, DWORD dwSubTime )
{
	m_CurSubQuestInfoArray[dwSubQuestIdx] = m_pQuestInfo->GetSubQuestInfo( dwSubQuestIdx );
	m_dwSubQuestFlag = dwSubQuestFlag;
	m_SubQuestTable[dwSubQuestIdx].dwData = dwSubData;
	m_SubQuestTable[dwSubQuestIdx].dwTime = dwSubTime;
	m_SubQuestTable[dwSubQuestIdx].dwMaxCount = m_CurSubQuestInfoArray[dwSubQuestIdx]->GetMaxCount();
}

void CQuest::EndSubQuest( DWORD dwSubQuestIdx, DWORD dwSubQuestFlag, DWORD dwSubData, DWORD dwSubTime )
{
	m_CurSubQuestInfoArray[dwSubQuestIdx] = NULL;
	m_dwSubQuestFlag = dwSubQuestFlag;
	m_SubQuestTable[dwSubQuestIdx].dwData = dwSubData;
	m_SubQuestTable[dwSubQuestIdx].dwTime = dwSubTime;
}

void CQuest::UpdateSubQuestData( DWORD dwSubQuestIdx, DWORD dwMaxCount, DWORD dwSubData, DWORD dwSubTime )
{
	m_SubQuestTable[dwSubQuestIdx].dwMaxCount = dwMaxCount;
	m_SubQuestTable[dwSubQuestIdx].dwData = dwSubData;
	m_SubQuestTable[dwSubQuestIdx].dwTime = dwSubTime;
}

DWORD CQuest::GetQuestIdx()
{
	return m_pQuestInfo->GetQuestIdx();
}

BOOL CQuest::IsNpcRelationQuest( DWORD dwNpcIdx )
{
	for( DWORD i = 0; i < m_dwSubQuestCount; ++i )
	{
		if( m_CurSubQuestInfoArray[i] )
		if( m_CurSubQuestInfoArray[i]->IsNpcRelationQuest( dwNpcIdx ) )
			return TRUE;
	}
	return FALSE;
}

DWORD CQuest::GetNpcScriptPage( DWORD dwNpcIdx )
{
	DWORD dwPage = 0;
	for( DWORD i = 0; i < m_dwSubQuestCount; ++i )
	{
		if( m_CurSubQuestInfoArray[i] )
		if( dwPage = m_CurSubQuestInfoArray[i]->GetNpcScriptPage( dwNpcIdx ) )
			return dwPage;
	}
	return dwPage;
}

BOOL CQuest::IsSubQuestComplete( DWORD dwSubQuestIdx )
{
	DWORD value = 1;
	return ( m_dwSubQuestFlag & ( value << (31-dwSubQuestIdx) ) ) ? TRUE : FALSE;
}

DWORD CQuest::GetNpcMarkType( DWORD dwNpcIdx )
{
	DWORD dwNpcMarkType = 0;
	for( DWORD i = 0; i < m_dwSubQuestCount; ++i )
	{
		if( m_CurSubQuestInfoArray[i] )
		if( dwNpcMarkType = m_CurSubQuestInfoArray[i]->GetNpcMarkType( dwNpcIdx ) )
			return dwNpcMarkType;
	}
	return dwNpcMarkType;
}

CSubQuestInfo* CQuest::GetSubQuestInfo( DWORD dwSubQuestIdx )
{
	return m_pQuestInfo->GetSubQuestInfo( dwSubQuestIdx );
}

int CQuest::IsQuestState( DWORD dwNpcIdx )
{
	for( DWORD i = 0; i < m_dwSubQuestCount; ++i )
	{
		if( m_CurSubQuestInfoArray[i] )
		{
			int rt = m_CurSubQuestInfoArray[i]->IsQuestState( dwNpcIdx );
			if( rt == 1 || rt == 2 )
				return rt;
		}
	}
	return 0;	
}

BOOL CQuest::CanItemUse( DWORD dwQuestIdx )
{
	if( m_CurSubQuestInfoArray[0] )
	{
		return m_CurSubQuestInfoArray[0]->CheckLimitCondition( dwQuestIdx );
	}
	return FALSE;
}

void CQuest::RegistCheckTime( BYTE bCheckType, DWORD dwCheckTime )
{
	m_bCheckTime = TRUE;
	m_CheckType = bCheckType;
	m_CheckTime.value = dwCheckTime;
}

void CQuest::UnRegistCheckTime()
{
	m_bCheckTime = FALSE;
	m_CheckType = 0;
	m_CheckTime.value = 0;
}