// DissolutionManager.cpp: implementation of the CDissolutionManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DissolutionManager.h"
#include "MHFile.h"
#include "GameResourceStruct.h"
#include "ItemManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLOBALTON(CDissolutionManager)

CDissolutionManager::CDissolutionManager()
{
	m_DissolutionInfoList.Initialize( 1000 );	//총 4500개정도 된다. 041021
}

CDissolutionManager::~CDissolutionManager()
{
	Release();
}

BOOL CDissolutionManager::Init()
{
	if( !LoadDissolutionInfo() ) 
	{
		ASSERTMSG( 0, "LoadDissolutionInfo()" );
		return FALSE;
	}

	return TRUE;
}

void CDissolutionManager::Release()
{
	sITEM_DISSOLUTION_INFO* pInfo = NULL;
	m_DissolutionInfoList.SetPositionHead();
	while( pInfo = m_DissolutionInfoList.GetData() )
	{
		delete[] pInfo->pUnitItem;
		delete pInfo;
	}

	m_DissolutionInfoList.RemoveAll();	
}

BOOL CDissolutionManager::LoadDissolutionInfo()
{
	CMHFile file;

#ifdef _FILE_BIN_
	if( !file.Init( "./Resource/itembreak.bin", "rb" ) )
		return FALSE;
#else
	if( !file.Init( "./Resource/itembreak.txt", "rt" ) )
		return FALSE;
#endif

	if( m_DissolutionInfoList.GetDataNum() > 0 )
		Release();

	while(1)
	{
		if( file.IsEOF() )
			break;

		WORD wTempDissolutionItemIdx	= file.GetWord();
		WORD wTempUnitKindNum			= file.GetWord();
		WORD wTempUnitTotalNum			= file.GetWord();

		if( wTempDissolutionItemIdx != 0 && wTempUnitKindNum != 0 && wTempUnitTotalNum != 0
			&& m_DissolutionInfoList.GetData( wTempDissolutionItemIdx ) == NULL )
		{
			sITEM_DISSOLUTION_INFO* pInfo = new sITEM_DISSOLUTION_INFO;
			pInfo->wDissolutionItemIdx	= wTempDissolutionItemIdx;
			pInfo->wUnitKindNum			= wTempUnitKindNum;
			pInfo->wUnitTotalNum		= wTempUnitTotalNum;
			pInfo->pUnitItem			= new sITEM_DISSOLUTIONITEM[wTempUnitKindNum];

			WORD wTempTotalNum = 0;
			for( int i = 0 ; i < wTempUnitKindNum ; ++i )
			{
				WORD wTempUnitItemIdx = file.GetWord();
				WORD wTempUnitItemNum = file.GetWord();


				if( wTempUnitItemIdx != 0 && wTempUnitItemNum != 0 )
				{
					pInfo->pUnitItem[i].wUnitItemIdx	= wTempUnitItemIdx;
					pInfo->pUnitItem[i].wUnitItemNum	= wTempUnitItemNum;
					wTempTotalNum += wTempUnitItemNum;
				}
				else
				{
					file.Release();
					return FALSE;
				}
			}

			if( wTempTotalNum != pInfo->wUnitTotalNum || wTempTotalNum > 300 )	//3개탭 모두는 300이다.
			{
				file.Release();
				return FALSE;
			}

			m_DissolutionInfoList.Add( pInfo, pInfo->wDissolutionItemIdx );
		}
		else
		{
			if( file.IsEOF())
				break;
			file.Release();
			return FALSE;
		}
	}

	file.Release();
	return TRUE;	
}

BOOL CDissolutionManager::CanDissolutionItem( WORD wItemIdx )
{
//리스트에 있는지 확인
	sITEM_DISSOLUTION_INFO* pInfo = m_DissolutionInfoList.GetData( wItemIdx );

	if( pInfo )
	if( ITEMMGR->GetItemInfo( wItemIdx )->LimitLevel != 1 )
		return TRUE;

	return FALSE;
}

sITEM_DISSOLUTIONITEM* CDissolutionManager::GetDissolutionUnitItemInfo( WORD wDissolutionItemIdx, WORD* pwSize )
{
	sITEM_DISSOLUTION_INFO* pInfo = m_DissolutionInfoList.GetData( wDissolutionItemIdx );
	if( pInfo == NULL )
	{
		if( pwSize ) *pwSize = 0;
		return NULL;
	}

	if( pwSize ) *pwSize = pInfo->wUnitKindNum;

	return m_DissolutionInfoList.GetData(wDissolutionItemIdx)->pUnitItem;
}

