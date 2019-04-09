#include "stdafx.h"
#include "FortWarManager.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "MHFile.h"
#include "Player.h"
#include "UserTable.h"
#include "ObjectStateManager.h"
#include "PackedData.h"
#include "Network.h"
#include "MapDBMsgParser.h"
#include "ItemManager.h"
#include "Guild.h"
#include "GuildManager.h"
#include "SiegeWarMgr.h"


GLOBALTON(CFortWarManager)

CFortWarManager::CFortWarManager(void)
{
	m_bSiegeWarWareUsableMap = FALSE;
}

CFortWarManager::~CFortWarManager(void)
{
	Release();
}

void CFortWarManager::Init()
{
	m_bLoadDataFromDB = FALSE;
	m_bFortWarMap = FALSE;
	m_dwChannelNum = 1;
	m_dwWarState = eFortWarState_None;
	m_pEngravePlayer = NULL;
	m_dwWarTime = 0;
	memset( m_FortWarTimeInfo, 0, sizeof(sFortWarTimeInfo)*eFortWarDay_Max);
	// 임시로 여기다..
	m_pNpcInfo = GAMERESRCMNGR->GetStaticNpcInfo( 15001 );

	m_pEngravePlayer = NULL;
	m_dwEngraveTime = 0;

	m_FortWarInfoTable.Initialize( 3 );
    m_wRelationFortWarMapNum = 0;
	m_nRelationFortWarIDX = 0;

	m_FortWarWareSlot01.Initialize();
	m_FortWarWareSlot02.Initialize();
	m_FortWarWareSlot03.Initialize();
	m_SiegeWarWareSlot.Initialize();
	m_nRelationSiegeWarIDX = SEIGEWARWAREIDX;	

	m_FortWarWarePlayerTable01.Initialize(20);
	m_FortWarWarePlayerTable02.Initialize(20);
	m_FortWarWarePlayerTable03.Initialize(20);
	m_SiegeWarWarePlayerTable.Initialize(20);

	m_dwFortWarProfitMoney = 0;
	m_nFortWarTexRate = 500;
	m_nFortWarWareRate = 3000;
	m_dwUpdateTime = gCurTime + 300000;		// 초(10분)

	LoadFortWarInfo();
}

void CFortWarManager::Release()
{
	sFortWarInfo* pInfo = NULL;
	m_FortWarInfoTable.SetPositionHead();
	while( pInfo = m_FortWarInfoTable.GetData() )
	{
		delete pInfo;
		pInfo = NULL;
	}
	m_FortWarInfoTable.RemoveAll();

	m_FortWarWarePlayerTable01.RemoveAll();
	m_FortWarWarePlayerTable02.RemoveAll();
	m_FortWarWarePlayerTable03.RemoveAll();
	m_SiegeWarWarePlayerTable.RemoveAll();
}

BOOL CFortWarManager::LoadFortWarInfo()
{
	CMHFile file;

	char filename[256] = {0,};
	char buf[256] = {0,};
	char line[512] = {0,};

#ifdef _FILE_BIN_
	sprintf(filename, "Resource/Server/FortWarInfo.bin");
	if( !file.Init(filename,"rb") ) return FALSE;
#else
	sprintf(filename, "Resource/Server/FortWarInfo.txt");
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

		if( strcmp(buf, "*FORTWAR") == 0 )
		{
			WORD wMapNum = file.GetWord();			
			if( wMapNum == g_pServerSystem->GetMapNum() )
			{
				m_bFortWarMap = TRUE;
				for( int i = eFortWarDay_Sun; i < eFortWarDay_Max; ++i )
				{
					int nDayOfWeek = file.GetInt();
                    m_FortWarTimeInfo[nDayOfWeek].bOn = file.GetBool();
					m_FortWarTimeInfo[nDayOfWeek].FortWarStartTime.wHour = file.GetWord();
					m_FortWarTimeInfo[nDayOfWeek].FortWarStartTime.wMinute = file.GetWord();
					m_FortWarTimeInfo[nDayOfWeek].dwWarTime = file.GetDword()*60;			// 초로 변환
				}
			}
		}
		else if( strcmp(buf, "*FORTWARRELATION") == 0 )
		{
			WORD wRelationMap = file.GetWord();
			int nCount = file.GetInt();
			for( int i = 0; i < nCount; ++i )
			{
				WORD wMapNum = file.GetWord();
				if( wMapNum == g_pServerSystem->GetMapNum() )
				{
					m_wRelationFortWarMapNum = wRelationMap;
					break;
				}
			}
		}
		else if( strcmp(buf, "*1_FORTWARMAP") == 0 )
		{
			WORD wMapNum = file.GetWord();
			if( wMapNum == m_wRelationFortWarMapNum )
                m_nRelationFortWarIDX = FORTWARWAREIDX_1;

			sFortWarInfo* pInfo = m_FortWarInfoTable.GetData( FORTWARWAREIDX_1 );
			if( pInfo == NULL )
			{
				pInfo = new sFortWarInfo;
				memset( pInfo, 0, sizeof(sFortWarInfo) );
				pInfo->nRelationFortWarIDX = FORTWARWAREIDX_1;
				m_FortWarInfoTable.Add( pInfo, pInfo->nRelationFortWarIDX );
			}
		}
		else if( strcmp(buf, "*2_FORTWARMAP") == 0 )
		{
			WORD wMapNum = file.GetWord();
			if( wMapNum == m_wRelationFortWarMapNum )
				m_nRelationFortWarIDX = FORTWARWAREIDX_2;

			sFortWarInfo* pInfo = m_FortWarInfoTable.GetData( FORTWARWAREIDX_2 );
			if( pInfo == NULL )
			{
				pInfo = new sFortWarInfo;
				memset( pInfo, 0, sizeof(sFortWarInfo) );
				pInfo->nRelationFortWarIDX = FORTWARWAREIDX_2;
				m_FortWarInfoTable.Add( pInfo, pInfo->nRelationFortWarIDX );
			}
		}
		else if( strcmp(buf, "*3_FORTWARMAP") == 0 )
		{
			WORD wMapNum = file.GetWord();
			if( wMapNum == m_wRelationFortWarMapNum )
				m_nRelationFortWarIDX = FORTWARWAREIDX_3;

			sFortWarInfo* pInfo = m_FortWarInfoTable.GetData( FORTWARWAREIDX_3 );
			if( pInfo == NULL )
			{
				pInfo = new sFortWarInfo;
				memset( pInfo, 0, sizeof(sFortWarInfo) );
				pInfo->nRelationFortWarIDX = FORTWARWAREIDX_3;
				m_FortWarInfoTable.Add( pInfo, pInfo->nRelationFortWarIDX );
			}
		}
		else if( strcmp(buf, "*FORTWARTEXRATE") == 0 )
		{
			m_nFortWarTexRate = file.GetInt();
		}
		else if( strcmp(buf, "*FORTWARWARERATE") == 0 )
		{
			m_nFortWarWareRate = file.GetInt();
		}
	}

	return TRUE;
}

int CFortWarManager::GetSecondInterval( SYSTEMTIME curTime, SYSTEMTIME basicTime )
{
	int nHour = 0;
	int nMin = 0;
	int nSec = 0;
	if( basicTime.wHour > curTime.wHour )
	{
		nHour = basicTime.wHour - curTime.wHour;
		// 분
		if( basicTime.wMinute > curTime.wMinute )
		{
			nMin = basicTime.wMinute - curTime.wMinute;
		}
		else
		{
			nHour--;
			nMin = basicTime.wMinute + 60 - curTime.wMinute;
		}
		// 초
		if( basicTime.wSecond >= curTime.wSecond )
		{
			nSec = basicTime.wSecond - curTime.wSecond;
		}
		else
		{
			nMin--;
			nSec = basicTime.wSecond + 60 - curTime.wSecond;
		}
		return nHour*60*60 + nMin*60 + nSec;
	}
	else
	{
		nHour = basicTime.wHour - curTime.wHour;
		nMin = basicTime.wMinute - curTime.wMinute;
		nSec = basicTime.wSecond - curTime.wSecond;
		return nHour*60*60 + nMin*60 + nSec;        
	}
}

void CFortWarManager::ProcessFortWar()
{
	if( m_bLoadDataFromDB == FALSE )
		return;
	if( m_bFortWarMap == FALSE )
		return;

	SYSTEMTIME curTime;
	GetLocalTime( &curTime );	

	if( m_FortWarTimeInfo[curTime.wDayOfWeek].bOn == FALSE )
		return;

	switch( m_dwWarState )
	{
	case eFortWarState_None:
		{	
			int nSec = GetSecondInterval( curTime, m_FortWarTimeInfo[curTime.wDayOfWeek].FortWarStartTime );
			if( nSec > 0 && nSec <= 600 )
			{
				m_dwWarState = eFortWarState_Before10Min;

				// 모든 플레이어한테 알린다. Agent로 보내서 Agent에서 처리한다.
				MSG_FORTWAR_START msg;
				msg.Category = MP_FORTWAR;
				msg.Protocol = MP_FORTWAR_START_BEFORE10MIN;
				msg.wForWarMapNum = g_pServerSystem->GetMapNum();
				msg.dwChannelNum = m_dwChannelNum;
				msg.dwFortWarTime = 0;	// 초
				SafeStrCpy( msg.EngraveSuccessPlayerName, GetMasterName(), MAX_NAME_LENGTH+1 );
				msg.vNpcPos.x = 0;
				msg.vNpcPos.y = 0;
				g_Network.Send2AgentServer( (char*)&msg, sizeof(msg) );

				//  다른맵서버에 정보를 보낸다.
				MSG_FORTWAR_START msg2 = msg;
				msg2.Protocol = MP_FORTWAR_START_BEFORE10MIN_TO_MAP;
				g_Network.Send2AgentServer( (char*)&msg2, sizeof(msg2) );
			}
			else if( nSec <= 0 )	// 서버다운시
			{
				m_dwWarState = eFortWarState_Ing;
				if( abs(nSec) >= m_FortWarTimeInfo[curTime.wDayOfWeek].dwWarTime )
					m_dwWarState = eFortWarState_None;
				if( m_dwWarState == eFortWarState_Ing )
				{
                    m_dwWarTime = (m_FortWarTimeInfo[curTime.wDayOfWeek].dwWarTime+nSec)*1000 + gCurTime;	// milisecond
//					m_pNpcInfo->vPos.x = 10000;
//					m_pNpcInfo->vPos.z = 10000;

					// 다른맵서버에 정보를 보낸다.
					MSG_FORTWAR_START msg;
					msg.Category = MP_FORTWAR;
					msg.Protocol = MP_FORTWAR_ING_TO_MAP;
					msg.wForWarMapNum = g_pServerSystem->GetMapNum();
					msg.dwChannelNum = m_dwChannelNum;
					msg.dwFortWarTime = 0;	// 초
					SafeStrCpy( msg.EngraveSuccessPlayerName, GetMasterName(), MAX_NAME_LENGTH+1 );
					msg.vNpcPos.x = 0;
					msg.vNpcPos.y = 0;
					g_Network.Send2AgentServer( (char*)&msg, sizeof(msg) );
				}
			}
		}
		break;
	case eFortWarState_Before10Min:
		{
			int nSec = GetSecondInterval( curTime, m_FortWarTimeInfo[curTime.wDayOfWeek].FortWarStartTime );
			if( nSec <= 0 )
			{
				// 상태 및 시간 세팅.
				m_dwWarState = eFortWarState_Ing;
				m_dwWarTime = m_FortWarTimeInfo[curTime.wDayOfWeek].dwWarTime*1000 + gCurTime;
//				m_pNpcInfo->vPos.x = 10000;
//				m_pNpcInfo->vPos.z = 10000;

				// 모든 플레이어한테 시작을 알린다. Agent로 보내서 Agent에서 처리한다.
				MSG_FORTWAR_START msg;
				msg.Category = MP_FORTWAR;
				msg.Protocol = MP_FORTWAR_START;
				msg.wForWarMapNum = g_pServerSystem->GetMapNum();
				msg.dwChannelNum = m_dwChannelNum;
                msg.dwFortWarTime = m_FortWarTimeInfo[curTime.wDayOfWeek].dwWarTime;	// 초
                SafeStrCpy( msg.EngraveSuccessPlayerName, GetMasterName(), MAX_NAME_LENGTH+1 );
				msg.vNpcPos.x = m_pNpcInfo->vPos.x;
				msg.vNpcPos.y = m_pNpcInfo->vPos.z;
				g_Network.Send2AgentServer( (char*)&msg, sizeof(msg) );

				//  다른맵서버에 정보를 보낸다.
				MSG_FORTWAR_START msg2 = msg;
				msg2.Protocol = MP_FORTWAR_START_TO_MAP;
				g_Network.Send2AgentServer( (char*)&msg2, sizeof(msg2) );
			}
		}
		break;
	case eFortWarState_Ing:
		{
			// 요새전 종료시...
			if( gCurTime >= m_dwWarTime )
			{
				m_dwWarState = eFortWarState_None;		

				// 각인중인 캐릭터가 있다면...
				if( m_pEngravePlayer )
				{
					// state 해제
                    OBJECTSTATEMGR_OBJ->EndObjectState( m_pEngravePlayer, eObjectState_Engrave );

					// msg send
					MSG_DWORD msg;
					msg.Category = MP_FORTWAR;
					msg.Protocol = MP_FORTWAR_ENGRAVE_FAIL;
					msg.dwData = m_pEngravePlayer->GetID();
					PACKEDDATA_OBJ->QuickSend( m_pEngravePlayer, &msg, sizeof(msg) );
				}

				// 모든 플레이어한테 요새전 종료를 알린다. Agent로 보내서 Agent에서 처리한다.
				MSG_FORTWAR_END msg;
				msg.Category = MP_FORTWAR;
				msg.Protocol = MP_FORTWAR_END;
				msg.wForWarMapNum = g_pServerSystem->GetMapNum();
				msg.dwChannelNum = m_dwChannelNum;
				SafeStrCpy( msg.EngraveSuccessPlayerName, GetMasterName(), MAX_NAME_LENGTH+1 );
				g_Network.Send2AgentServer( (char*)&msg, sizeof(msg) );

				//  다른맵서버에 정보를 보낸다.
				MSG_FORTWAR_END msg2 = msg;
				msg2.Protocol = MP_FORTWAR_END_TO_MAP;
				g_Network.Send2AgentServer( (char*)&msg2, sizeof(msg2) );

				//				
				m_pEngravePlayer = NULL;
				m_dwEngraveTime = 0;

				// 문파가 차지했다면 문파포인트 추가
				if( GetMasterGuildID() )
                    GUILDMGR->GuildPointFromFortWar( GetMasterGuildID(), 10000 );

				return;
			}

			// 각인처리...
			if( m_pEngravePlayer )
			if( gCurTime >= m_dwEngraveTime )
			{
				// state 해제
				OBJECTSTATEMGR_OBJ->EndObjectState( m_pEngravePlayer, eObjectState_Engrave );

				// msg send
				MSG_NAME_DWORD msg;
				msg.Category = MP_FORTWAR;
				msg.Protocol = MP_FORTWAR_ENGRAVE_SUCESS;
				msg.dwData = m_pEngravePlayer->GetID();
				SafeStrCpy( msg.Name, m_pEngravePlayer->GetObjectName(), MAX_NAME_LENGTH+1 );
				// 임시로 이렇게 처리...
				CObject* pObject = NULL;
				g_pUserTable->SetPositionHead();
				while( pObject = g_pUserTable->GetData() )
				{
					if( pObject->GetObjectKind() != eObjectKind_Player )
						continue;

					MSG_NAME_DWORD msg1 = msg;
					pObject->SendMsg( &msg1, sizeof(msg1) );
				}

				// 정보세팅
				UpdateFortWarInfo( m_nRelationFortWarIDX, m_pEngravePlayer->GetID(), m_pEngravePlayer->GetObjectName(), m_pEngravePlayer->GetGuildIdx() );
				// 디비 업데이트
				FortWarInfoUpdate( m_nRelationFortWarIDX, m_pEngravePlayer->GetID(), m_pEngravePlayer->GetObjectName(), m_pEngravePlayer->GetGuildIdx() );
				//
			
				m_pEngravePlayer = NULL;
				m_dwEngraveTime = 0;
			}
		}
		break;
	}
}

void CFortWarManager::AddPlayer( CPlayer* pPlayer )
{
	if( m_nRelationFortWarIDX == 0 )
		return;
	
	MSG_DWORD2 msg;
	msg.Category = MP_FORTWAR;
	msg.Protocol = MP_FORTWAR_INFO;
	msg.dwData1 = m_nRelationFortWarIDX;
	msg.dwData2 = m_nFortWarTexRate;
	pPlayer->SendMsg( &msg, sizeof(msg) );

	if( m_dwWarState != eFortWarState_Ing ||
        pPlayer->GetChannelID() != m_dwChannelNum )
        return;

	// 요새전 진행정보를 보낸다.
	MSG_FORTWAR_START msg2;
	msg2.Category = MP_FORTWAR;
	msg2.Protocol = MP_FORTWAR_ING;
	msg2.wForWarMapNum = g_pServerSystem->GetMapNum();
	msg2.dwChannelNum = m_dwChannelNum;
	msg2.dwFortWarTime = (m_dwWarTime - gCurTime) / 1000;	// 초
	SafeStrCpy( msg2.EngraveSuccessPlayerName, GetMasterName(), MAX_NAME_LENGTH+1 );
	msg2.vNpcPos.x = m_pNpcInfo->vPos.x;
	msg2.vNpcPos.y = m_pNpcInfo->vPos.z;
	pPlayer->SendMsg( &msg2, sizeof(msg2) );
}

void CFortWarManager::RemovePlayer( CPlayer* pPlayer )
{
	RemoveWarePlayerTable( pPlayer );

	if( m_dwWarState != eFortWarState_Ing )
		return;
	if( pPlayer->GetChannelID() != m_dwChannelNum )
		return;
	if( pPlayer != m_pEngravePlayer )
        return;

	m_pEngravePlayer = NULL;
    m_dwEngraveTime = 0;
}

void CFortWarManager::DiePlayer( CPlayer* pPlayer )
{
	if( m_dwWarState != eFortWarState_Ing )
		return;
	if( pPlayer->GetChannelID() != m_dwChannelNum )
		return;
	if( m_pEngravePlayer == NULL )
		return;
	if( pPlayer != m_pEngravePlayer )
		return;

	// msg send
	MSG_DWORD msg;
	msg.Category = MP_FORTWAR;
	msg.Protocol = MP_FORTWAR_ENGRAVE_FAIL;
	msg.dwData = m_pEngravePlayer->GetID();
	PACKEDDATA_OBJ->QuickSend( m_pEngravePlayer, &msg, sizeof(msg) );

	//
	OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Engrave );
    m_pEngravePlayer = NULL;
    m_dwEngraveTime = 0;
}

void CFortWarManager::SetTotalFortWarInfoFromDB( sFortWarInfo info )
{
	// 디비상에 정보가 있어도 파일에 없으면 return
	sFortWarInfo* pInfo = m_FortWarInfoTable.GetData( info.nRelationFortWarIDX );
	if( pInfo == NULL )
		return;

	*pInfo = info;
}

void CFortWarManager::UpdateFortWarInfo( int nWarIDX, DWORD dwCharacterIdx, char* pCharacterName, DWORD dwGuildIdx )
{
	sFortWarInfo* pInfo = m_FortWarInfoTable.GetData( nWarIDX );
	if( pInfo == NULL )
		return;

	pInfo->nRelationFortWarIDX = nWarIDX;
    pInfo->dwCharacterIdx = dwCharacterIdx;
    SafeStrCpy( pInfo->CharacterName, pCharacterName, MAX_NAME_LENGTH+1 );
	pInfo->dwGuildIdx = dwGuildIdx;
}

DWORD CFortWarManager::GetMasterID()
{
	sFortWarInfo* pInfo = m_FortWarInfoTable.GetData( m_nRelationFortWarIDX );
	if( pInfo != NULL )
	{
		return pInfo->dwCharacterIdx;
	}
	return 0;
}

char* CFortWarManager::GetMasterName()
{
	sFortWarInfo* pInfo = m_FortWarInfoTable.GetData( m_nRelationFortWarIDX );
	if( pInfo != NULL )
	{
		return pInfo->CharacterName;
	}
	return "";
}

DWORD CFortWarManager::GetMasterGuildID()
{
	sFortWarInfo* pInfo = m_FortWarInfoTable.GetData( m_nRelationFortWarIDX );
	if( pInfo != NULL )
	{
		return pInfo->dwGuildIdx;
	}
	return 0;
}

void CFortWarManager::ProcessDBUpdateProfitMoney()
{
//	if( m_nRelationFortWarIDX == 0 )
//		return;

//	if( m_dwFortWarProfitMoney == 0 )
//		return;

	if( gCurTime > m_dwUpdateTime )
	{
		m_dwUpdateTime = gCurTime + 300000;
	
		if( m_nRelationFortWarIDX == 0 )
		{
			FortWarProfitMoneyUpdate( FORTWARWAREIDX_1, m_dwFortWarProfitMoney, 0 );		// 0 = add
			FortWarProfitMoneyUpdate( FORTWARWAREIDX_2, m_dwFortWarProfitMoney, 0 );		// 0 = add
			FortWarProfitMoneyUpdate( FORTWARWAREIDX_3, m_dwFortWarProfitMoney, 0 );		// 0 = add
		}
		else
            FortWarProfitMoneyUpdate( m_nRelationFortWarIDX, m_dwFortWarProfitMoney, 0 );		// 0 = add
		m_dwFortWarProfitMoney = 0;
	}
}

DWORD CFortWarManager::AddProfitMoneyFromItemSell( DWORD dwMoney )
{
	if( m_nRelationFortWarIDX == 0 )
		return dwMoney;

	DWORD dwAddMoney = (DWORD)( ((float)dwMoney * m_nFortWarTexRate) / 10000 );

	m_dwFortWarProfitMoney += dwAddMoney;

	return dwMoney - dwAddMoney;
}

DWORD CFortWarManager::AddProfitMoneyFromItemBuy( DWORD dwMoney )
{
	if( m_nRelationFortWarIDX == 0 )
		return dwMoney;

	DWORD dwAddMoney = (DWORD)( ((float)dwMoney * m_nFortWarTexRate) / 10000 );

	m_dwFortWarProfitMoney += dwAddMoney;

	return dwMoney + dwAddMoney;
}

DWORD CFortWarManager::AddProfitMoneyFromMonster( DWORD dwMoney )
{
	if( m_nRelationFortWarIDX == 0 )
		return dwMoney;

	DWORD dwAddMoney = (DWORD)( ((float)dwMoney * m_nFortWarTexRate) / 10000 );

	m_dwFortWarProfitMoney += dwAddMoney;

	return dwMoney - dwAddMoney;
}

void CFortWarManager::SetFortWarProfitMoneyFromDB( int nWarIDX, DWORD dwMoney )
{
//	if( nWarIDX != m_nRelationFortWarIDX )
//		return;

	sFortWarInfo* pInfo = m_FortWarInfoTable.GetData( nWarIDX );
	if( NULL == pInfo )
		return;

    pInfo->dwProfitMoney = dwMoney;	
}

void CFortWarManager::RemoveWarePlayerTable( CPlayer* pPlayer )
{
	if( m_FortWarWarePlayerTable01.GetData( pPlayer->GetID() ) )
		m_FortWarWarePlayerTable01.Remove( pPlayer->GetID() );
	if( m_FortWarWarePlayerTable02.GetData( pPlayer->GetID() ) )
		m_FortWarWarePlayerTable02.Remove( pPlayer->GetID() );
	if( m_FortWarWarePlayerTable03.GetData( pPlayer->GetID() ) )
		m_FortWarWarePlayerTable03.Remove( pPlayer->GetID() );
	if( m_SiegeWarWarePlayerTable.GetData( pPlayer->GetID() ) )
		m_SiegeWarWarePlayerTable.Remove( pPlayer->GetID() );
}

void CFortWarManager::TotalFortWarItemLoad()
{
	FortWarItemLoad( FORTWARWAREIDX_1 );
	FortWarItemLoad( FORTWARWAREIDX_2 );
	FortWarItemLoad( FORTWARWAREIDX_3 );
	FortWarItemLoad( SEIGEWARWAREIDX );
}

void CFortWarManager::SetTotalFortWarItemInfoFromDB( int nWarIDX, ITEMBASE* pItem )
{
	switch( nWarIDX )
	{
	case SEIGEWARWAREIDX:
		{
			if( m_SiegeWarWareSlot.InsertItemAbs( NULL, pItem->Position, pItem ) != EI_TRUE )
			{
				// 에러 발생시 초기화
				m_SiegeWarWareSlot.DeleteItemAbs( NULL, pItem->Position, pItem );
				return;
			}
		}
		break;
	case FORTWARWAREIDX_1:
		{
			if( m_FortWarWareSlot01.InsertItemAbs( NULL, pItem->Position, pItem ) != EI_TRUE )
			{
				// 에러 발생시 초기화
				m_FortWarWareSlot01.DeleteItemAbs( NULL, pItem->Position, pItem );
				return;
			}
		}
		break;
	case FORTWARWAREIDX_2:
		{
			if( m_FortWarWareSlot02.InsertItemAbs( NULL, pItem->Position, pItem ) != EI_TRUE )
			{
				// 에러 발생시 초기화
				m_FortWarWareSlot02.DeleteItemAbs( NULL, pItem->Position, pItem );
				return;
			}
		}
		break;
	case FORTWARWAREIDX_3:
		{
			if( m_FortWarWareSlot03.InsertItemAbs( NULL, pItem->Position, pItem ) != EI_TRUE )
			{
				// 에러 발생시 초기화
				m_FortWarWareSlot03.DeleteItemAbs( NULL, pItem->Position, pItem );
				return;
			}
		}
		break;
	}
}

BOOL CFortWarManager::AddProfitItemFromMonster( CPlayer* pPlayer, DWORD dwItemIdx, WORD wItemNum )
{
	if( m_nRelationFortWarIDX == 0 )
		return FALSE;

	CFortWarWareSlot* pSlot = GetFortWarWareSlot( m_nRelationFortWarIDX );
	if( pSlot == NULL )
		return FALSE;
                                                                                                           
	// 확률계산
	int nRate = random( 1, 10000 );
	if( nRate > m_nFortWarTexRate )
		return FALSE;
	//

	ITEMBASE NewItemBase;
	NewItemBase.dwDBIdx	= 9;
	NewItemBase.wIconIdx = 9;
	NewItemBase.Position = 9;
	NewItemBase.QuickPosition = 9;
	NewItemBase.Durability = 0;
	NewItemBase.ItemParam = 0;
	NewItemBase.RareIdx = 0;
	
	WORD wPos = pSlot->GetEmptySlotPos();
	if( wPos == 0 )	// 더이상 들어갈 곳이 없다.
        return FALSE;

	NewItemBase.Position = wPos;

/*
	// 겹치는 아이템 처리...
	if( ITEMMGR->IsDupItem( dwItemIdx ) )
	{
		ITEMBASE* pInfo = pSlot->GetSameItemForDupItem( dwItemIdx, wItemNum );
		if( pInfo )
		{
			pSlot->UpdateItemAbs( NULL, pInfo->Position, pInfo->dwDBIdx, pInfo->wIconIdx, 0, 0, wItemNum+pInfo->Durability, UB_DURA, SS_PREINSERT );

			// db update
		}
		else
		{
			NewItemBase.Durability = wItemNum;
		}
	}
*/	
	if( pSlot->InsertItemAbs( NULL, wPos, &NewItemBase, SS_PREINSERT ) != EI_TRUE )
	{
		// 에러 발생시 초기화
		pSlot->DeleteItemAbs( NULL, wPos, &NewItemBase );
		return FALSE;
	}

	// db insert
    FortWarItemInsertToDB( m_nRelationFortWarIDX, dwItemIdx, NewItemBase.Durability, NewItemBase.Position, NewItemBase.ItemParam );

	// 사냥중 아이템이 요새창고로 드랍되었다...
	MSG_DWORD msg;
	msg.Category = MP_FORTWAR;
	msg.Protocol = MP_FORTWAR_DROPITEM_TO_FORTWAREHOUSE;
	msg.dwData = dwItemIdx;
	pPlayer->SendMsg( &msg, sizeof(msg) );

	return TRUE;
}

void CFortWarManager::AddProfitItemFromDBResult( int nWarIDX, ITEMBASE* pItem )
{
	if( nWarIDX != m_nRelationFortWarIDX )
		return;

	CFortWarWareSlot* pSlot = GetFortWarWareSlot( m_nRelationFortWarIDX );
	if( pSlot == NULL )
		return;

	if( pSlot->InsertItemAbs( NULL, pItem->Position, pItem ) != EI_TRUE )
	{
		// 에러 발생시 초기화
		pSlot->DeleteItemAbs( NULL, pItem->Position, pItem );
		return;
	}

	// 다른맵에도 정보 보낸다...
	MSG_FORTWAR_WAREITEM_INFO msg;
	msg.Category = MP_FORTWAR;
	msg.Protocol = MP_FORTWAR_WAREHOUSE_ITEM_INSERT_TO_MAP;
	msg.nFortWarIDX = m_nRelationFortWarIDX;
	msg.ItemInfo = *pItem;
	g_Network.Send2AgentServer( (char*)&msg, sizeof(msg) );

	// 요새창고를 보고 있는 캐릭터에게도 정보를 보낸다.
	msg.Protocol = MP_FORTWAR_WAREHOUSE_ITEM_INSERT;
	SendItemInfoToFortWarePlayer( m_nRelationFortWarIDX, &msg, sizeof(msg) );
}

CFortWarWareSlot* CFortWarManager::GetFortWarWareSlot( int nWarIDX )
{
	switch( nWarIDX )
	{
	case FORTWARWAREIDX_1:	return &m_FortWarWareSlot01;	break;
	case FORTWARWAREIDX_2:	return &m_FortWarWareSlot02;	break;
	case FORTWARWAREIDX_3:	return &m_FortWarWareSlot03;	break;
	}

	return NULL;
}

void CFortWarManager::SendItemInfoToFortWarePlayer( int nWarIDX, MSGBASE* pMsg, int nSize )
{
	CYHHashTable<CPlayer>* pTable = NULL;
	switch( nWarIDX )
	{
	case FORTWARWAREIDX_1:	pTable = &m_FortWarWarePlayerTable01;	break;
	case FORTWARWAREIDX_2:	pTable = &m_FortWarWarePlayerTable02;	break;
	case FORTWARWAREIDX_3:	pTable = &m_FortWarWarePlayerTable03;	break;
	}

	if( pTable == NULL )
		return;

	CPlayer* pPlayer = NULL;
	pTable->SetPositionHead();
	while( pPlayer = pTable->GetData() )
	{
		pPlayer->SendMsg( pMsg, nSize );
	}
}

void CFortWarManager::SendItemInfoToFortWarePlayerExceptOne( int nWarIDX, DWORD dwPlayerID, MSGBASE* pMsg, int nSize )
{
	CYHHashTable<CPlayer>* pTable = NULL;
	switch( nWarIDX )
	{
	case FORTWARWAREIDX_1:	pTable = &m_FortWarWarePlayerTable01;	break;
	case FORTWARWAREIDX_2:	pTable = &m_FortWarWarePlayerTable02;	break;
	case FORTWARWAREIDX_3:	pTable = &m_FortWarWarePlayerTable03;	break;
	}

	if( pTable == NULL )
		return;

	CPlayer* pPlayer = NULL;
	pTable->SetPositionHead();
	while( pPlayer = pTable->GetData() )
	{
		if( dwPlayerID == pPlayer->GetID() )
			continue;
		pPlayer->SendMsg( pMsg, nSize );
	}
}

void CFortWarManager::SendItemInfoToSeigeWarePlayer( MSGBASE* pMsg, int nSize )
{
	CPlayer* pPlayer = NULL;
	m_SiegeWarWarePlayerTable.SetPositionHead();
	while( pPlayer = m_SiegeWarWarePlayerTable.GetData() )
	{
		pPlayer->SendMsg( pMsg, nSize );
	}
}

void CFortWarManager::SendItemInfoToSeigeWarePlayerExceptOne( DWORD dwPlayerID, MSGBASE* pMsg, int nSize )
{
	CPlayer* pPlayer = NULL;
	m_SiegeWarWarePlayerTable.SetPositionHead();
	while( pPlayer = m_SiegeWarWarePlayerTable.GetData() )
	{
		if( dwPlayerID == pPlayer->GetID() )
			continue;
		pPlayer->SendMsg( pMsg, nSize );
	}
}

void CFortWarManager::NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg )
{
	switch( Protocol )
	{
	// 요새전
	case MP_FORTWAR_ENGRAVE_START_SYN:	Msg_MP_FORTWAR_ENGRAVE_START_SYN( pMsg );	break;		
	case MP_FORTWAR_ENGRAVE_CANCEL_SYN:	Msg_MP_FORTWAR_ENGRAVE_CANCEL_SYN( pMsg );	break;
	// 창고
	case MP_FORTWAR_WAREHOUSE_INFO_SYN:				Msg_MP_FORTWAR_WAREHOUSE_INFO_SYN( pMsg );				break;
	case MP_FORTWAR_SIEGEWAREHOUSE_INFO_SYN:		Msg_MP_FORTWAR_SIEGEWAREHOUSE_INFO_SYN( pMsg );			break;
	case MP_FORTWAR_WAREHOUSE_LEAVE:				Msg_MP_FORTWAR_WAREHOUSE_LEAVE( pMsg );					break;
	case MP_FORTWAR_WAREHOUSE_ITEM_MOVE_SYN:		Msg_MP_FORTWAR_WAREHOUSE_ITEM_MOVE_SYN( pMsg );			break;
	case MP_FORTWAR_SIEGEWAREHOUSE_ITEM_MOVE_SYN:	Msg_MP_FORTWAR_SIEGEWAREHOUSE_ITEM_MOVE_SYN( pMsg );	break;
	// 다른맵으로부터
	case MP_FORTWAR_WAREHOUSE_ITEM_INSERT_TO_MAP:				Msg_MP_FORTWAR_WAREHOUSE_ITEM_INSERT_TO_MAP( pMsg );				break;
	case MP_FORTWAR_WAREHOUSE_ITEM_MOVE_TO_INVEN_TO_MAP:		Msg_MP_FORTWAR_WAREHOUSE_ITEM_MOVE_TO_INVEN_TO_MAP( pMsg);			break;
	case MP_FORTWAR_WAREHOUSE_ITEM_MOVE_TO_SIEGEHOUSE_TO_MAP:	Msg_MP_FORTWAR_WAREHOUSE_ITEM_MOVE_TO_SIEGEHOUSE_TO_MAP( pMsg );	break;

	case MP_FORTWAR_WAREHOUSE_PUTOUT_MONEY_SYN:			Msg_MP_FORTWAR_WAREHOUSE_PUTOUT_MONEY_SYN( pMsg );		break;
	case MP_FORTWAR_WAREHOUSE_MONEY_TO_MAP:				Msg_MP_FORTWAR_WAREHOUSE_MONEY_TO_MAP( pMsg );			break;
	case MP_FORTWAR_SIEGEWAREHOUSE_PUTOUT_MONEY_SYN:	Msg_MP_FORTWAR_SIEGEWAREHOUSE_PUTOUT_MONEY_SYN( pMsg );	break;
	}	
}

void CFortWarManager::Msg_MP_FORTWAR_ENGRAVE_START_SYN( void* pMsg )
{
    MSGBASE* pmsg = (MSGBASE*)pMsg;

    CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
    if( !pPlayer )	return;

	MSG_DWORD2 msg;

	int error = eFortWarEngraveError_None;
	if( m_bFortWarMap == FALSE )
	{
		error = eFortWarEngraveError_NotMap;				// 요새전맵이 아니다.
		goto EngraveStartError;
	}
	if( m_dwWarState != eFortWarState_Ing )
	{
		error = eFortWarEngraveError_NotIng;				// 진행중이 아니다
		goto EngraveStartError;
	}
	if( m_pEngravePlayer )
	{
		error = eFortWarEngraveError_AlreadyPlayer;			// 다른 캐릭터가 이미 각인중이다
		goto EngraveStartError;
	}
	if( pPlayer->GetGuildIdx() )
	{
		if( pPlayer->GetGuildMemberRank() != GUILD_MASTER )
		{
			error = eFortWarEngraveError_NotGuildMaster;	// 문주가 아니다
			goto EngraveStartError;
		}
	}

	// state 해제
    if( pPlayer->GetState() == eObjectState_Deal )
        OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Deal );
    OBJECTSTATEMGR_OBJ->StartObjectState( pPlayer, eObjectState_Engrave, 0 );
    
	// msg send
    msg.Category = MP_FORTWAR;
    msg.Protocol = MP_FORTWAR_ENGRAVE_START_ACK;
    msg.dwData1 = pPlayer->GetID();
    msg.dwData2 = 60;			// 초
    PACKEDDATA_OBJ->QuickSend( pPlayer, &msg, sizeof(msg) );

	//
    m_pEngravePlayer = pPlayer;
    m_dwEngraveTime = 60*1000 + gCurTime;	// milisec로 변환

	return;

EngraveStartError:
    MSG_DWORD errorMsg;
	errorMsg.Category = MP_FORTWAR;
	errorMsg.Protocol = MP_FORTWAR_ENGRAVE_START_NACK;
	errorMsg.dwData = error;
	pPlayer->SendMsg( &errorMsg, sizeof(errorMsg) );
}

void CFortWarManager::Msg_MP_FORTWAR_ENGRAVE_CANCEL_SYN( void* pMsg )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pPlayer )	return;

	if( pPlayer == m_pEngravePlayer )
	{
		if( pPlayer->GetState() == eObjectState_Engrave )
			OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Engrave );

		m_pEngravePlayer = NULL;
		m_dwEngraveTime = 0;
	}

	MSG_DWORD msg;
	msg.Category = MP_FORTWAR;
	msg.Protocol = MP_FORTWAR_ENGRAVE_CANCEL_ACK;
	msg.dwData = pPlayer->GetID();
	PACKEDDATA_OBJ->QuickSend( pPlayer, &msg, sizeof(msg) );
}

void CFortWarManager::Msg_MP_FORTWAR_WAREHOUSE_INFO_SYN( void* pMsg )
{
	MSG_INT* pmsg = (MSG_INT*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pPlayer )	return;

	sFortWarInfo* pInfo = m_FortWarInfoTable.GetData( pmsg->nData );
	if( NULL == pInfo )
		return;
	CFortWarWareSlot* pSlot = GetFortWarWareSlot( pmsg->nData );
	if( pSlot == NULL )
		return;

	MSG_FORTWAR_WAREHOUSE_INFO msg;
	msg.Category = MP_FORTWAR;
	msg.Protocol = MP_FORTWAR_WAREHOUSE_INFO_ACK;
	msg.nFortWarIDX = m_nRelationFortWarIDX;
	msg.dwMoney = pInfo->dwProfitMoney;
	pSlot->GetItemInfoAll( &msg );

	pPlayer->SendMsg( &msg, msg.GetSize() );

	// 테이블에 추가
	CYHHashTable<CPlayer>* pTable = NULL;
	switch( pmsg->nData )
	{
	case FORTWARWAREIDX_1:	pTable = &m_FortWarWarePlayerTable01;	break;
	case FORTWARWAREIDX_2:	pTable = &m_FortWarWarePlayerTable02;	break;
	case FORTWARWAREIDX_3:	pTable = &m_FortWarWarePlayerTable03;	break;
	}
	if( pTable == NULL )
		return;
	if( pTable->GetData( pPlayer->GetID() ) == NULL )
        pTable->Add( pPlayer, pPlayer->GetID() );
}

void CFortWarManager::Msg_MP_FORTWAR_SIEGEWAREHOUSE_INFO_SYN( void* pMsg )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pPlayer )	return;

	MSG_FORTWAR_WAREHOUSE_INFO msg;
	msg.Category = MP_FORTWAR;
	msg.Protocol = MP_FORTWAR_SIEGEWAREHOUSE_INFO_ACK;
	msg.nFortWarIDX = m_nRelationFortWarIDX;
	msg.dwMoney = 0;
	m_SiegeWarWareSlot.GetItemInfoAll( &msg );

	pPlayer->SendMsg( &msg, msg.GetSize() );

	// 테이블에 추가
	if( m_SiegeWarWarePlayerTable.GetData( pPlayer->GetID() ) == NULL )
		m_SiegeWarWarePlayerTable.Add( pPlayer, pPlayer->GetID() );
}

void CFortWarManager::Msg_MP_FORTWAR_WAREHOUSE_LEAVE( void* pMsg )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pPlayer )	return;

	RemoveWarePlayerTable( pPlayer );
}

void CFortWarManager::Msg_MP_FORTWAR_WAREHOUSE_ITEM_MOVE_SYN( void* pMsg )
{
	MSG_ITEM_MOVE_SYN* pmsg = (MSG_ITEM_MOVE_SYN*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
	if( pPlayer == NULL )	return;

	ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( pmsg->wFromItemIdx );
	if( pInfo == NULL )
		return;

	if( pmsg->FromPos == pmsg->ToPos )
		return;

	// 조건검사
	int error = eFortWarWareError_None;
	
	if( m_bFortWarMap == FALSE )
	{
		error = eFortWarWareError_NotMap;			// 요새창고를 이용할 수 있는 맵이 아니다.
		goto ItemMoveError;
	}
	if( m_dwWarState != eFortWarState_None )
	{
		error = eFortWarWareError_NotTime;			// 요새창고를 이용할 수 있는 시간이 아니다.
		goto ItemMoveError;
	}
	if( pPlayer->GetID() != GetMasterID() )			// 주인과 다르다면...
	{
        if( pPlayer->GetGuildIdx() )				// 문파에 가입되어 있는경우
		{
			CGuild* pGuild = GUILDMGR->GetGuild( pPlayer->GetGuildIdx() );
			if( pGuild == NULL )						// 길드정보가 없다면...
			{
				error = eFortWarWareError_NotRight;		// 권한이 없다.
				goto ItemMoveError;
			}
            if( pGuild->GetMasterIdx() != GetMasterID() )
			{
                error = eFortWarWareError_NotRight;		// 권한이 없다.
                goto ItemMoveError;
			}
            if( pPlayer->GetGuildMemberRank() != GUILD_MASTER || pPlayer->GetGuildMemberRank() != GUILD_VICEMASTER )
			{
                error = eFortWarWareError_NotRight;		// 권한이 없다.
                goto ItemMoveError;
			}
		}
		else
		{
            error = eFortWarWareError_NotRight;			// 권한이 없다.
			goto ItemMoveError;
		}
	}
	//

	// 창고 얻기..
	CFortWarWareSlot* pSlot = GetFortWarWareSlot( m_nRelationFortWarIDX );
	if( pSlot == NULL )
	{
		error = eFortWarWareError_NotUse;
		goto ItemMoveError;
	}

	// 확률계산
	int nRate = random( 1, 10000 );
	//	
	if( nRate > m_nFortWarWareRate )	// 요새창고 -> 인벤으로
	{
		CItemSlot* pToSlot = pPlayer->GetSlot( pmsg->ToPos );
		CItemSlot* pInven = pPlayer->GetSlot(eItemTable_Inventory);
		if( pToSlot != pInven )
			return;

		if( !pToSlot->IsEmpty( pmsg->ToPos ) )
			return;

		ITEMBASE DelFromItem;
		// 요새창고에서 지운다.
		if( EI_TRUE != pSlot->DeleteItemAbs( pPlayer, pmsg->FromPos, &DelFromItem ) )
			return;
		// 인벤에 추가한다.
		if( EI_TRUE != pToSlot->InsertItemAbs( pPlayer, pmsg->ToPos, &DelFromItem ) )
			return;

		// DB update
		FortWarItemMoveToDB( pPlayer->GetID(), DelFromItem.dwDBIdx, DelFromItem.Position, 0 );

		// log
		LogItemMoney( 0, "FortWarWare", pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemObtainFromFortWarWare, 0, pPlayer->GetMoney(eItemTable_Inventory),
			0, DelFromItem.wIconIdx, DelFromItem.dwDBIdx, pmsg->FromPos, pmsg->ToPos, DelFromItem.Durability, pPlayer->GetPlayerExpPoint() );

		// msg send
		MSG_ITEM_MOVE_ACK msg;
		memcpy( &msg, pmsg, sizeof(MSG_ITEM_MOVE_SYN) );
		msg.Protocol = MP_FORTWAR_WAREHOUSE_ITEM_MOVE_ACK;
        pPlayer->SendMsg( &msg, sizeof(msg) );

		// 다른맵서버에도 정보를 보낸다.
		MSG_FORTWAR_WAREITEM_INFO msg1;
		msg1.Category = MP_FORTWAR;
		msg1.Protocol = MP_FORTWAR_WAREHOUSE_ITEM_MOVE_TO_INVEN_TO_MAP;
		msg1.nFortWarIDX = m_nRelationFortWarIDX;		// 요새창고정보
		msg1.ItemInfo = DelFromItem;
		msg1.wFromPos = pmsg->FromPos;
		msg1.wToPos = 0;
		g_Network.Send2AgentServer( (char*)&msg1, sizeof(msg1) );

		// 요새창고를 보고 있는 다른 캐릭터에게도 정보를 보낸다.
		msg1.Protocol = MP_FORTWAR_WAREHOUSE_ITEM_MOVE_TO_INVEN;
		SendItemInfoToFortWarePlayerExceptOne( m_nRelationFortWarIDX, pPlayer->GetID(), &msg1, sizeof(msg1) );
	}	
	else				// 요새창고 -> 공성창고로
	{
		WORD wPos = m_SiegeWarWareSlot.GetEmptySlotPos();
		if( wPos == 0 )
			return;

		ITEMBASE DelFromItem;
		// 요새창고에서 지운다.
		if( EI_TRUE != pSlot->DeleteItemAbs( pPlayer, pmsg->FromPos, &DelFromItem ) )
			return;
		// 공성창고에 추가한다.
		if( m_SiegeWarWareSlot.InsertItemAbs( NULL, wPos, &DelFromItem ) != EI_TRUE )
			return;

		// DB update
		FortWarItemMoveToDB( 0, DelFromItem.dwDBIdx, wPos, m_nRelationSiegeWarIDX );

		// msg send
        MSG_ITEM_MOVE_ACK msg;
        memcpy( &msg, pmsg, sizeof(MSG_ITEM_MOVE_SYN) );
        msg.Protocol = MP_FORTWAR_WAREHOUSE_ITEM_MOVE_TO_SIEGEHOUSE_SYN;
        pPlayer->SendMsg( &msg, sizeof(msg) );

		// 다른맵서버에도 정보를 보낸다.
		MSG_FORTWAR_WAREITEM_INFO msg1;
		msg1.Category = MP_FORTWAR;
		msg1.Protocol = MP_FORTWAR_WAREHOUSE_ITEM_MOVE_TO_SIEGEHOUSE_TO_MAP;		
		msg1.nFortWarIDX = m_nRelationFortWarIDX;			// 요새창고정보
		msg1.ItemInfo = DelFromItem;
		msg1.wFromPos = pmsg->FromPos;
		msg1.wToPos = wPos;		
		g_Network.Send2AgentServer( (char*)&msg1, sizeof(msg1) );

		// 요새창고와 공성창고를 보는 다른 캐릭터에게 정보를 보낸다.
		msg1.Protocol = MP_FORTWAR_WAREHOUSE_ITEM_MOVE_TO_SIEGEHOUSE;
		SendItemInfoToFortWarePlayerExceptOne( m_nRelationFortWarIDX, pPlayer->GetID(), &msg, sizeof(msg) );
		SendItemInfoToSeigeWarePlayerExceptOne( pPlayer->GetID(), &msg, sizeof(msg) );		
	}	

	return;

ItemMoveError:
	MSG_DWORD errorMsg;
	errorMsg.Category = MP_FORTWAR;
	errorMsg.Protocol = MP_FORTWAR_WAREHOUSE_ITEM_MOVE_NACK;
	errorMsg.dwData = error;
	pPlayer->SendMsg( &errorMsg, sizeof(errorMsg) );
}

void CFortWarManager::Msg_MP_FORTWAR_SIEGEWAREHOUSE_ITEM_MOVE_SYN( void* pMsg )
{
	MSG_ITEM_MOVE_SYN* pmsg = (MSG_ITEM_MOVE_SYN*)pMsg;
	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( pPlayer == NULL )	return;

	ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( pmsg->wFromItemIdx );
	if( pInfo == NULL )
		return;

	if( pmsg->FromPos == pmsg->ToPos )
		return;

	// 조건검사
	int error = eFortWarWareError_None;

	if( m_bSiegeWarWareUsableMap == FALSE )
	{
		error = eFortWarWareError_NotMap;			// 공성창고를 이용할 수 있는 맵이 아니다.
		goto ItemMoveError;
	}
	if( pPlayer->GetGuildIdx() == SIEGEWARMGR->GetCastleGuildIdx() )	// 성소유문파냐?
	{
		if( pPlayer->GetGuildMemberRank() != GUILD_MASTER || pPlayer->GetGuildMemberRank() != GUILD_VICEMASTER )
		{
			error = eFortWarWareError_NotRight;		// 권한이 없다.
			goto ItemMoveError;
		}
	}
	else
	{
		error = eFortWarWareError_NotRight;			// 권한이 없다.
		goto ItemMoveError;
	}
	//

	// 인벤으로
	{
		CItemSlot* pToSlot = pPlayer->GetSlot( pmsg->ToPos );
		CItemSlot* pInven = pPlayer->GetSlot(eItemTable_Inventory);
		if( pToSlot != pInven )
			return;

		if( !pToSlot->IsEmpty( pmsg->ToPos ) )
			return;

		ITEMBASE DelFromItem;
		if( EI_TRUE != m_SiegeWarWareSlot.DeleteItemAbs( pPlayer, pmsg->FromPos, &DelFromItem ) )
			return;

		if( pToSlot->InsertItemAbs( pPlayer, pmsg->ToPos, &DelFromItem ) != EI_TRUE )
			return;

		// DB update
		FortWarItemMoveToDB( pPlayer->GetID(), DelFromItem.dwDBIdx, DelFromItem.Position, 0 );

		// log
		LogItemMoney( 0, "SeigeWarWare", pPlayer->GetID(), pPlayer->GetObjectName(), eLog_ItemObtainFromSeigeWarWare, 0, pPlayer->GetMoney(eItemTable_Inventory),
			0, DelFromItem.wIconIdx, DelFromItem.dwDBIdx, pmsg->FromPos, pmsg->ToPos, DelFromItem.Durability, pPlayer->GetPlayerExpPoint() );

		MSG_ITEM_MOVE_ACK msg;
		memcpy( &msg, pmsg, sizeof(MSG_ITEM_MOVE_SYN) );
		msg.Protocol = MP_FORTWAR_SIEGEWAREHOUSE_ITEM_MOVE_ACK;
		pPlayer->SendMsg( &msg, sizeof(msg) );

		// 다른맵에도 정보보낸다...
		MSG_FORTWAR_WAREITEM_INFO msg1;
		msg1.Category = MP_FORTWAR;
		msg1.Protocol = MP_FORTWAR_SIEGEWAREHOUSE_ITEM_MOVE_TO_INVEN_TO_MAP;
		msg1.nFortWarIDX = m_nRelationSiegeWarIDX;
		msg1.ItemInfo = DelFromItem;
		msg1.wFromPos = pmsg->FromPos;
		msg1.wToPos = 0;
		g_Network.Send2AgentServer( (char*)&msg1, sizeof(msg1) );

		// 공성창고를 보는 다른 캐릭터에게 정보를 보낸다.
		msg1.Protocol = MP_FORTWAR_SIEGEWAREHOUSE_ITEM_MOVE_TO_INVEN;
		SendItemInfoToSeigeWarePlayerExceptOne( pPlayer->GetID(), &msg1, sizeof(msg1) );
	}

	return;

ItemMoveError:
	MSG_DWORD errorMsg;
	errorMsg.Category = MP_FORTWAR;
	errorMsg.Protocol = MP_FORTWAR_SIEGEWAREHOUSE_ITEM_MOVE_NACK;
	errorMsg.dwData = error;
	pPlayer->SendMsg( &errorMsg, sizeof(errorMsg) );
}

void CFortWarManager::Msg_MP_FORTWAR_WAREHOUSE_ITEM_INSERT_TO_MAP( void* pMsg )
{
	MSG_FORTWAR_WAREITEM_INFO* pmsg = (MSG_FORTWAR_WAREITEM_INFO*)pMsg;

//	if( m_nRelationFortWarIDX != pmsg->nFortWarIDX )
//		return;

	// 창고 얻기..
	CFortWarWareSlot* pSlot = GetFortWarWareSlot( pmsg->nFortWarIDX );
	if( pSlot == NULL )
		return;

	// 싱크관련...
	WORD wPos = pSlot->GetEmptySlotPos();
	if( wPos == 0 )	// 더이상 들어갈 곳이 없다.
		return;
	if( wPos != pmsg->ItemInfo.Position )
	{
		// 로그기록
	}

	if( pSlot->InsertItemAbs( NULL, wPos, &pmsg->ItemInfo ) != EI_TRUE )
	{
		// 에러 발생시 초기화
		pSlot->DeleteItemAbs( NULL, wPos, &pmsg->ItemInfo );
		return;
	}

	// 요새창고를 보고 있는 캐릭터에게 정보를 보낸다.
	pmsg->Protocol = MP_FORTWAR_WAREHOUSE_ITEM_INSERT;
	SendItemInfoToFortWarePlayer( pmsg->nFortWarIDX, pmsg, sizeof(MSG_FORTWAR_WAREITEM_INFO) );
}

void CFortWarManager::Msg_MP_FORTWAR_WAREHOUSE_ITEM_MOVE_TO_INVEN_TO_MAP( void* pMsg )
{
	MSG_FORTWAR_WAREITEM_INFO* pmsg = (MSG_FORTWAR_WAREITEM_INFO*)pMsg;

//	if( m_nRelationFortWarIDX != pmsg->nFortWarIDX )
//		return;

	CFortWarWareSlot* pSlot = GetFortWarWareSlot( pmsg->nFortWarIDX );
	if( pSlot == NULL )
		return;

	pSlot->DeleteItemAbs( NULL, pmsg->wFromPos, &pmsg->ItemInfo );

	// 요새창고를 보고 있는 캐릭터에게 정보를 보낸다.
	pmsg->Protocol = MP_FORTWAR_WAREHOUSE_ITEM_MOVE_TO_INVEN;
	SendItemInfoToFortWarePlayer( pmsg->nFortWarIDX, pmsg, sizeof(MSG_FORTWAR_WAREITEM_INFO) );
}

void CFortWarManager::Msg_MP_FORTWAR_WAREHOUSE_ITEM_MOVE_TO_SIEGEHOUSE_TO_MAP( void* pMsg )
{
	MSG_FORTWAR_WAREITEM_INFO* pmsg = (MSG_FORTWAR_WAREITEM_INFO*)pMsg;

//	if( pmsg->nFortWarIDX == m_nRelationFortWarIDX )
//	{
		// 요새창고 지우고...
//      m_FortWarWareSlot.DeleteItemAbs( NULL, pmsg->wFromPos, &pmsg->ItemInfo );
//	}

	CFortWarWareSlot* pSlot = GetFortWarWareSlot( pmsg->nFortWarIDX );
	if( pSlot != NULL )
	{
		// 요새창고 지우고...
		pSlot->DeleteItemAbs( NULL, pmsg->wFromPos, &pmsg->ItemInfo );
	}

	// 공성창고에 넣는다...
	WORD wPos = m_SiegeWarWareSlot.GetEmptySlotPos();
	if( wPos == 0)	// 더이상 들어갈 곳이 없다.
		return;

	if( wPos != pmsg->wToPos )
	{
		int a = 0;			
	}

	if( m_SiegeWarWareSlot.InsertItemAbs( NULL, wPos, &pmsg->ItemInfo ) != EI_TRUE )
	{
		// 에러 발생시 초기화
		m_SiegeWarWareSlot.DeleteItemAbs( NULL, wPos, &pmsg->ItemInfo );
		return;
	}

	// 요새창고와 공성창고를 보는 다른 캐릭터에게 정보를 보낸다.
	pmsg->Protocol = MP_FORTWAR_WAREHOUSE_ITEM_MOVE_TO_SIEGEHOUSE;
	SendItemInfoToFortWarePlayer( pmsg->nFortWarIDX, pmsg, sizeof(MSG_FORTWAR_WAREITEM_INFO) );
	SendItemInfoToSeigeWarePlayer( pmsg, sizeof(MSG_FORTWAR_WAREITEM_INFO) );	
}

void CFortWarManager::Msg_MP_FORTWAR_WAREHOUSE_PUTOUT_MONEY_SYN( void* pMsg )
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( pPlayer == NULL )
		return;

	// 조건검사
	int error = eFortWarWareError_None;

	if( m_bFortWarMap == FALSE )
	{
		error = eFortWarWareError_NotMap;			// 요새창고를 이용할 수 있는 맵이 아니다.
		goto ItemMoveError;
	}
	if( m_dwWarState != eFortWarState_None )
	{
		error = eFortWarWareError_NotTime;			// 요새창고를 이용할 수 있는 시간이 아니다.
		goto ItemMoveError;
	}
	if( pPlayer->GetID() != GetMasterID() )			// 주인과 다르다면...
	{
		if( pPlayer->GetGuildIdx() )				// 문파에 가입되어 있는경우
		{
			CGuild* pGuild = GUILDMGR->GetGuild( pPlayer->GetGuildIdx() );
			if( pGuild == NULL )						// 길드정보가 없다면...
			{
				error = eFortWarWareError_NotRight;		// 권한이 없다.
				goto ItemMoveError;
			}
			if( pGuild->GetMasterIdx() != GetMasterID() )
			{
				error = eFortWarWareError_NotRight;		// 권한이 없다.
				goto ItemMoveError;
			}
			if( pPlayer->GetGuildMemberRank() != GUILD_MASTER || pPlayer->GetGuildMemberRank() != GUILD_VICEMASTER )
			{
				error = eFortWarWareError_NotRight;		// 권한이 없다.
				goto ItemMoveError;
			}
		}
		else
		{
			error = eFortWarWareError_NotRight;			// 권한이 없다.
			goto ItemMoveError;
		}
	}
	//

	{
		MONEYTYPE dwMoney = pmsg->dwData;
		MONEYTYPE dwMaxMon = pPlayer->GetMaxPurseMoney(eItemTable_Inventory);

		sFortWarInfo* pInfo = m_FortWarInfoTable.GetData( m_nRelationFortWarIDX );
		if( NULL == pInfo )
			return;

		if( dwMoney > pInfo->dwProfitMoney )
		{
			dwMoney = pInfo->dwProfitMoney;
		}
		if( dwMaxMon - pPlayer->GetMoney() < dwMoney )
		{
			dwMoney = dwMaxMon - pPlayer->GetMoney();
		}
		if( dwMoney == 0 )
			return;

		// 30%는 공성창고로...
		DWORD dwSeigeMoney = dwMoney * m_nFortWarWareRate / 10000;
		DWORD dwInvenMoney = dwMoney - dwSeigeMoney;

		// 나머지는 인벤토리로
		dwInvenMoney = pPlayer->SetMoney( dwInvenMoney, MONEY_ADDITION, 0, eItemTable_Inventory, eMoneyLog_GetGuild, 0 );

		// 요새창고에서 돈 차감
		pInfo->dwProfitMoney = pInfo->dwProfitMoney - dwMoney;

		// db update
		FortWarProfitMoneyUpdate( m_nRelationFortWarIDX, dwMoney, 1 );	// 1 = 차감

		// 요새창고를 보고 있는 캐릭터에게 정보를 보낸다.
		MSG_DWORD msg;
		msg.Category = MP_FORTWAR;
		msg.Protocol = MP_FORTWAR_WAREHOUSE_PUTOUT_MONEY_ACK;
		msg.dwData = pInfo->dwProfitMoney;
		SendItemInfoToFortWarePlayer( m_nRelationFortWarIDX, &msg, sizeof(msg) );

		// 다른맵서버에 정보 보낸다...
		MSG_DWORD2 msg2;
		msg2.Category = MP_FORTWAR;
		msg2.Protocol = MP_FORTWAR_WAREHOUSE_MONEY_TO_MAP;
		msg2.dwData1 = m_nRelationFortWarIDX;
		msg2.dwData2 = pInfo->dwProfitMoney;
		g_Network.Send2AgentServer( (char*)&msg2, sizeof(msg2) );
	}

	return;

ItemMoveError:
	MSG_DWORD errorMsg;
	errorMsg.Category = MP_FORTWAR;
	errorMsg.Protocol = MP_FORTWAR_WAREHOUSE_PUTOUT_MONEY_NACK;
	errorMsg.dwData = error;
	pPlayer->SendMsg( &errorMsg, sizeof(errorMsg) );
}

void CFortWarManager::Msg_MP_FORTWAR_WAREHOUSE_MONEY_TO_MAP( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	sFortWarInfo* pInfo = m_FortWarInfoTable.GetData( pmsg->dwData1 );
	if( NULL == pInfo )
		return;

	pInfo->dwProfitMoney = pmsg->dwData2;	
}

void CFortWarManager::Msg_MP_FORTWAR_SIEGEWAREHOUSE_PUTOUT_MONEY_SYN( void* pMsg )
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( pPlayer == NULL )
		return;

	// 조건검사
	int error = eFortWarWareError_None;

	if( m_bSiegeWarWareUsableMap == FALSE )
	{
		error = eFortWarWareError_NotMap;		// 공성창고를 이용할 수 있는 맵이 아니다.
		goto ItemMoveError;
	}
	{
		error = eFortWarWareError_NotMap;			// 요새창고를 이용할 수 있는 맵이 아니다.
		goto ItemMoveError;
	}
	if( m_dwWarState != eFortWarState_None )
	{
		error = eFortWarWareError_NotTime;			// 요새창고를 이용할 수 있는 시간이 아니다.
		goto ItemMoveError;
	}
	if( pPlayer->GetID() != GetMasterID() )			// 주인과 다르다면...
	{
		if( pPlayer->GetGuildIdx() )				// 문파에 가입되어 있는경우
		{
			CGuild* pGuild = GUILDMGR->GetGuild( pPlayer->GetGuildIdx() );
			if( pGuild == NULL )						// 길드정보가 없다면...
			{
				error = eFortWarWareError_NotRight;		// 권한이 없다.
				goto ItemMoveError;
			}
			if( pGuild->GetMasterIdx() != GetMasterID() )
			{
				error = eFortWarWareError_NotRight;		// 권한이 없다.
				goto ItemMoveError;
			}
			if( pPlayer->GetGuildMemberRank() != GUILD_MASTER || pPlayer->GetGuildMemberRank() != GUILD_VICEMASTER )
			{
				error = eFortWarWareError_NotRight;		// 권한이 없다.
				goto ItemMoveError;
			}
		}
		else
		{
			error = eFortWarWareError_NotRight;			// 권한이 없다.
			goto ItemMoveError;
		}
	}
	//

	{
		MONEYTYPE dwMoney = pmsg->dwData;
		MONEYTYPE dwMaxMon = pPlayer->GetMaxPurseMoney(eItemTable_Inventory);

		sFortWarInfo* pInfo = m_FortWarInfoTable.GetData( m_nRelationFortWarIDX );
		if( NULL == pInfo )
			return;

		if( dwMoney > pInfo->dwProfitMoney )
		{
			dwMoney = pInfo->dwProfitMoney;
		}
		if( dwMaxMon - pPlayer->GetMoney() < dwMoney )
		{
			dwMoney = dwMaxMon - pPlayer->GetMoney();
		}
		if( dwMoney == 0 )
			return;

		// 30%는 공성창고로...
		DWORD dwSeigeMoney = dwMoney * m_nFortWarWareRate / 10000;
		DWORD dwInvenMoney = dwMoney - dwSeigeMoney;

		// 나머지는 인벤토리로
		dwInvenMoney = pPlayer->SetMoney( dwInvenMoney, MONEY_ADDITION, 0, eItemTable_Inventory, eMoneyLog_GetGuild, 0 );

		// 요새창고에서 돈 차감
		pInfo->dwProfitMoney = pInfo->dwProfitMoney - dwMoney;

		// db update
		FortWarProfitMoneyUpdate( m_nRelationFortWarIDX, dwMoney, 1 );	// 1 = 차감

		// 요새창고를 보고 있는 캐릭터에게 정보를 보낸다.
		MSG_DWORD msg;
		msg.Category = MP_FORTWAR;
		msg.Protocol = MP_FORTWAR_WAREHOUSE_PUTOUT_MONEY_ACK;
		msg.dwData = pInfo->dwProfitMoney;
		SendItemInfoToFortWarePlayer( m_nRelationFortWarIDX, &msg, sizeof(msg) );
	}

	return;

ItemMoveError:
	MSG_DWORD errorMsg;
	errorMsg.Category = MP_FORTWAR;
	errorMsg.Protocol = MP_FORTWAR_WAREHOUSE_PUTOUT_MONEY_NACK;
	errorMsg.dwData = error;
	pPlayer->SendMsg( &errorMsg, sizeof(errorMsg) );
}

