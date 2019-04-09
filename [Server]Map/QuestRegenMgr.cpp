// QuestRegenMgr.cpp: implementation of the CQuestRegenMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuestRegenMgr.h"
#include "MHFile.h"
#include "Player.h"
#include "RecallManager.h"
#include "ChannelSystem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CQuestRegenInfo::CQuestRegenInfo()
{
	m_bCondition = 0;	
	m_wMonsterCount = 0;
	m_wMonsterKind = 0;
	m_wRadius = 0;
	m_vOnePos.x = m_vOnePos.y = m_vOnePos.z = 0.0f;
	m_pPos = NULL;
}

CQuestRegenInfo::~CQuestRegenInfo()
{
	if( m_pPos )
	{
		delete [] m_pPos;
		m_pPos = NULL;
	}
}

void CQuestRegenInfo::LoadData( CMHFile* pFile )
{	
	char temp[256];
	while( 1 )
	{
		pFile->GetString( temp );
		if( pFile->IsEOF() )	break;

		if( temp[0] == '{' )	continue;
		else if( temp[0] == '}' )	break;
		else if( temp[0] == '@' )
		{
			pFile->GetLine( temp, 256);
			continue;
		}
		strcpy( temp, _strupr(temp) );

		if( strcmp( temp, "#CONDITION" ) == 0 )
		{
			m_bCondition = pFile->GetByte();
		}
		else if( strcmp( temp, "#MONSTER" ) == 0 )
		{
			m_wMonsterCount = pFile->GetWord();
			m_wMonsterKind = pFile->GetWord();			
			if( m_wMonsterCount )
				m_pPos = new QRPOS[m_wMonsterCount];
		}
		else if( strcmp( temp, "#RADIUS" ) == 0 )
		{
			m_wRadius = pFile->GetWord();
		}
		else if( strcmp( temp, "#ONEPOS" ) == 0 )
		{
			m_vOnePos.x = pFile->GetFloat();
			m_vOnePos.y = 0.0f;
			m_vOnePos.z = pFile->GetFloat();
		}
		else if( strcmp( temp, "#POSITION" ) == 0 )
		{
			int index = pFile->GetInt();
			m_pPos[index].vPos.x = pFile->GetFloat();
			m_pPos[index].vPos.y = 0.0f;
			m_pPos[index].vPos.z = pFile->GetFloat();
			m_pPos[index].wRadius = pFile->GetWord();
		}
	}	
}

void CQuestRegenInfo::MonsterRegen( CPlayer* pPlayer )
{
	if( m_bCondition == 0 )			// random
	{
		VECTOR3 vPos;
		pPlayer->GetPosition( &vPos );
		
		RECALLMGR->QuestRecall( m_wMonsterKind, (BYTE)m_wMonsterCount, pPlayer->GetChannelID(), &vPos, m_wRadius );
	}
	else if( m_bCondition == 1 )	// one position
	{
		RECALLMGR->QuestRecall( m_wMonsterKind, (BYTE)m_wMonsterCount, pPlayer->GetChannelID(), &m_vOnePos, m_wRadius );
	}
	else if( m_bCondition == 2 )	// each position
	{
		for( WORD i = 0; i < m_wMonsterCount; ++i )
			RECALLMGR->QuestRecall( m_wMonsterKind, 1, pPlayer->GetChannelID(), &m_pPos[i].vPos, m_pPos[i].wRadius );
	}
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CQuestRegenMgr);

CQuestRegenMgr::CQuestRegenMgr()
{
	m_QRITable.Initialize( 100 );
}

CQuestRegenMgr::~CQuestRegenMgr()
{
	CQuestRegenInfo* pData = NULL;
	m_QRITable.SetPositionHead();
	while( pData = m_QRITable.GetData() )
		delete pData;
	m_QRITable.RemoveAll();
}

void CQuestRegenMgr::LoadData()
{	
	CMHFile file;
	char temp[256];

#ifdef _FILE_BIN_
	if( !file.Init( "./Resource/QuestScript/QuestRegen.bin", "rb" ) )
		return;
#else
	if( !file.Init( "./Resource/QuestScript/QuestRegen.txt", "rt" ) )
		return;
#endif
	
	/*
#ifdef _FILE_BIN_
	if( !file.Init( "./Quest/QuestRegen.bin", "rb" ) )
		return;
#else
	if( !file.Init( "./Quest/QuestRegen.txt", "rt" ) )
		return;
#endif
	*/
	while( 1 )
	{
		file.GetString( temp );
		if( file.IsEOF() )	break;
		if( (temp[0] == '}') || (temp[0] == '{') )	continue;
		else if( temp[0] == '@' )
		{
			file.GetLine( temp, 256);
			continue;
		}
		strcpy( temp, _strupr(temp) );

		if( strcmp( temp, "$REGENTYPE" ) == 0 )
		{
			int index = file.GetInt();

			CQuestRegenInfo* pInfo = new CQuestRegenInfo;
			pInfo->LoadData( &file );
			m_QRITable.Add( pInfo, index );
		}
	}
	
	file.Release();
}

void CQuestRegenMgr::MonsterRegen( CPlayer* pPlayer, DWORD dwRegenType )
{
	CQuestRegenInfo* pInfo = m_QRITable.GetData( dwRegenType );
	if( pInfo )
		pInfo->MonsterRegen( pPlayer );
}
