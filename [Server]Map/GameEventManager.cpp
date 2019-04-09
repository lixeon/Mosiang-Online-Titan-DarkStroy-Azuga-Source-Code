#include "StdAfx.h"
#include "GameEventManager.h"
#include "MHFile.h"
#include "ItemManager.h"
#include "ItemLimitManager.h"
#include "Player.h"


GLOBALTON(CGameEventManager)

CGameEventManager::CGameEventManager(void)
{
}

CGameEventManager::~CGameEventManager(void)
{
}

void CGameEventManager::Init()
{

}

void CGameEventManager::Release()
{

}

BOOL CGameEventManager::LoadGameEventInfo()
{
	CMHFile file;

	char filename[256] = {0,};
	char buf[256] = {0,};
	char line[512] = {0,};

#ifdef _FILE_BIN_
	sprintf(filename, "Resource/Server/GameEventInfo.bin");
	if( !file.Init(filename,"rb") ) return FALSE;
#else
	sprintf(filename, "Resource/Server/GameEventInfo.txt");
	if( !file.Init(filename,"rt") ) return FALSE;
#endif

	while( !file.IsEOF() )
	{
		file.GetString(buf);

		if( buf[0] == '/' )
		{
			file.GetLineX(line,512);
			continue;
		}

		if( strcmp(buf, "*LEVELUP") == 0 )
		{
			m_LevelUpEventInfo.bOn = file.GetBool();
			m_LevelUpEventInfo.dwItemIdx = file.GetDword();
			m_LevelUpEventInfo.nTotalLevelCount = file.GetInt();
			m_LevelUpEventInfo.pLevel = new WORD[m_LevelUpEventInfo.nTotalLevelCount];
			for( int i = 0; i < m_LevelUpEventInfo.nTotalLevelCount; ++i )
				m_LevelUpEventInfo.pLevel[i] = file.GetWord();
		}		
	}

	return TRUE;
}

void CGameEventManager::ProcessLevelUpEvent( CPlayer* pPlayer, WORD wMaxLevel )
{
	if( FALSE == m_LevelUpEventInfo.bOn )
		return;

	int nCanGetItemNum = ITEMLIMITMGR->CheckItemLimitInfo( m_LevelUpEventInfo.dwItemIdx );	

	WORD wIncreaseItemCount = 0;
	if( m_LevelUpEventInfo.pLevel[m_LevelUpEventInfo.nTotalLevelCount-1] <= wMaxLevel )
	{
		if( 0 == nCanGetItemNum )	// limit°¹¼ö ÃÊ°ú
		{
			MSG_DWORD msg;
			msg.Category = MP_ITEMLIMIT;
			msg.Protocol = MP_ITEMLIMIT_FULL_TO_CLIENT;
			msg.dwData = m_LevelUpEventInfo.dwItemIdx;
			pPlayer->SendMsg( &msg, sizeof(msg) );
			return;
		}

		wIncreaseItemCount = ITEMMGR->ObtainItemFromGameEvent( pPlayer, m_LevelUpEventInfo.dwItemIdx, 1 );
	}
	else
	{
		if( 0 == nCanGetItemNum )	// limit°¹¼ö ÃÊ°ú
		{
			MSG_DWORD msg;
			msg.Category = MP_ITEMLIMIT;
			msg.Protocol = MP_ITEMLIMIT_FULL_TO_CLIENT;
			msg.dwData = m_LevelUpEventInfo.dwItemIdx;
			pPlayer->SendMsg( &msg, sizeof(msg) );
			return;
		}

		for( int i = 0; i < m_LevelUpEventInfo.nTotalLevelCount-1; ++i )
		{
			if( wMaxLevel == m_LevelUpEventInfo.pLevel[i] )
			{
				wIncreaseItemCount = ITEMMGR->ObtainItemFromGameEvent( pPlayer, m_LevelUpEventInfo.dwItemIdx, 1 );
				break;
			}
		}
	}

	if( wIncreaseItemCount > 0 )
		ITEMLIMITMGR->AddCurrentItemCount( m_LevelUpEventInfo.dwItemIdx, wIncreaseItemCount );
}

BOOL CGameEventManager::EventOnOff( DWORD dwItemIdx, BOOL bOnOff )
{
	if( m_LevelUpEventInfo.dwItemIdx == dwItemIdx )
	{
        m_LevelUpEventInfo.bOn = bOnOff;
		return TRUE;
	}
	return FALSE;
}
