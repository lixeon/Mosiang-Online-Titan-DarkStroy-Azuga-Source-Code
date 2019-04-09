// SiegeWarProfitMgr.cpp: implementation of the CSiegeWarProfitMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SiegeWarProfitMgr.h"
#include "UserTable.h"
#include "Player.h"
#include "Guild.h"
#include "GuildManager.h"
#include "MHFile.h"
#include "MapDBMsgParser.h"
#include "Monster.h"
#include "Battle.h"
#include "Network.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSiegeWarProfitMgr::CSiegeWarProfitMgr()
{
	m_ProfitTable.Initialize( 10 );
	m_SWMapTable.Initialize( 100 );
	m_pProfit = NULL;
	m_nRelationSWMapNum = 0;
	m_dwMoneyForStore = m_dwMoneyForMonster = 0;

	m_dwUpdateTime = gCurTime + 600000;
	m_dwDefaultMonsterMoney = 5;
}

CSiegeWarProfitMgr::~CSiegeWarProfitMgr()
{
	sSWPROFIT* pProfit = NULL;
	m_ProfitTable.SetPositionHead();
	while( pProfit = m_ProfitTable.GetData() )
		delete pProfit;
	m_ProfitTable.RemoveAll();

	WORD* pData = NULL;
	m_SWMapTable.SetPositionHead();
	while( pData = m_SWMapTable.GetData() )
		delete pData;
	m_SWMapTable.RemoveAll();
}

void CSiegeWarProfitMgr::LoadMapInfo( WORD wMapNum )
{
	CMHFile file;
//	if( !file.Init(".\\Resource\\SWRelationMap.bin", "rb") )
	if( !file.Init("./Resource/Server/SWRelationMap.bin", "rb") )
	{
		ASSERT(0);
		return;
	}
	
	char buf[32] = {0,};
	while( !file.IsEOF() )
	{		
		strcpy( buf, file.GetString() );

		if( strcmp( buf, "#SWMAP" ) == 0 )
		{
			WORD dwMap = file.GetWord();
			if( m_SWMapTable.GetData(dwMap) )	continue;
			WORD* pMap = new WORD;
			*pMap = file.GetWord();

			m_SWMapTable.Add( pMap, dwMap );

			if( dwMap == wMapNum )
				m_nRelationSWMapNum = *pMap;
		}
		else if( strcmp( buf, "#MONSTERMONEY" ) == 0 )
		{
			m_dwDefaultMonsterMoney = file.GetDword();
		}
	}
	file.Release();
}

void CSiegeWarProfitMgr::AddProfitTable( sSWPROFIT Info )
{
	CGuild* pGuild = GUILDMGR->GetGuild( Info.dwGuildIdx );
	if( !pGuild )	return;

	sSWPROFIT* pProfit = m_ProfitTable.GetData( Info.nSWMapNum );
	if( pProfit )
	{
		*pProfit = Info;
		SafeStrCpy( pProfit->sGuildName, pGuild->GetGuildName(), MAX_GUILD_NAME+1 );
	}
	else
	{
		pProfit = new sSWPROFIT;
		*pProfit = Info;
		SafeStrCpy( pProfit->sGuildName, pGuild->GetGuildName(), MAX_GUILD_NAME+1 );
		m_ProfitTable.Add( pProfit, pProfit->nSWMapNum );
	}

	if( m_nRelationSWMapNum == pProfit->nSWMapNum )
		m_pProfit = pProfit;
}

DWORD CSiegeWarProfitMgr::AddProfitFromItemSell( DWORD dwMoney )
{
	if( !m_pProfit )	return dwMoney;

	DWORD dwAddMoney = m_pProfit->nTaxRateForStore * dwMoney / 100;

	m_dwMoneyForStore += dwAddMoney;
//	m_pProfit->dwTotalMoney += dwAddMoney;

	return dwMoney - dwAddMoney;
}

DWORD CSiegeWarProfitMgr::AddProfitFromItemBuy( DWORD dwMoney )
{
	if( !m_pProfit )	return dwMoney;

	DWORD dwAddMoney = m_pProfit->nTaxRateForStore * dwMoney / 100;

	m_dwMoneyForStore += dwAddMoney;
//	m_pProfit->dwTotalMoney += dwAddMoney;

	return dwMoney + dwAddMoney;
}

void CSiegeWarProfitMgr::AddProfitFromMonster( CPlayer* pPlayer, CMonster* pMonster )
{
	if( !m_pProfit )	return;

	if( (pPlayer->GetMaxLevel()-pMonster->GetLevel()>5) || !(pMonster->GetSMonsterList()->ExpPoint) )
		return;
	if( pPlayer->GetBattle() != NULL )
	if( pPlayer->GetBattle()->GetBattleKind() == eBATTLE_KIND_SURYUN )
		return;

	m_dwMoneyForMonster += m_dwDefaultMonsterMoney;
//	m_pProfit->dwTotalMoney += 10;
}

void CSiegeWarProfitMgr::AddPlayer( CPlayer* pPlayer )
{
	if( !m_pProfit )	return;

	MSG_SW_PROFIT_INFO Msg;
	Msg.Category = MP_SIEGEWAR_PROFIT;
	Msg.Protocol = MP_SIEGEWAR_PROFIT_INFO;
	Msg.dwGuildIdx = m_pProfit->dwGuildIdx;
	SafeStrCpy( Msg.sGuildName, m_pProfit->sGuildName, MAX_GUILD_NAME+1 );
	Msg.nTaxRateForStore = m_pProfit->nTaxRateForStore;
	Msg.dwTotalMoney = m_pProfit->dwTotalMoney;

	pPlayer->SendMsg( &Msg, sizeof(Msg) );
}

void CSiegeWarProfitMgr::SetProfitMoney( int nMapNum, DWORD dwMoney )
{
	if( !m_pProfit )	return;
	if( m_pProfit->nSWMapNum != nMapNum )	return;

	m_pProfit->dwTotalMoney = dwMoney;
}

void CSiegeWarProfitMgr::SetProfitGuild( DWORD dwGuildIdx )
{
	WORD wMapNum = g_pServerSystem->GetMapNum();

	// db update
	SiegeWarProfitUpdateGuild( wMapNum, dwGuildIdx );

	MSG_DWORD2 Msg;
	Msg.Category = MP_SIEGEWAR_PROFIT;
	Msg.Protocol = MP_SIEGEWAR_PROFIT_CHANGE_GUILD_NOTIFY_TO_MAP;
	Msg.dwData1 = dwGuildIdx;
	Msg.dwData2 = wMapNum;
	g_Network.Send2AgentServer( (char*)&Msg, sizeof(Msg) );	

	CGuild* pGuild = GUILDMGR->GetGuild( dwGuildIdx );
	if( !pGuild )	return;

	sSWPROFIT* pProfit = m_ProfitTable.GetData( wMapNum );
	if( pProfit )
	{
		pProfit->dwGuildIdx = dwGuildIdx;
		pProfit->nTaxRateForStore = 0;
		pProfit->RegDate.value = 0;
		SafeStrCpy( pProfit->sGuildName, pGuild->GetGuildName(), MAX_GUILD_NAME+1 );
	}
	else
	{
		pProfit = new sSWPROFIT;
		pProfit->dwGuildIdx = dwGuildIdx;
		pProfit->nSWMapNum = wMapNum;
		pProfit->dwTotalMoney = 0;
		pProfit->nTaxRateForStore = 0;
		pProfit->RegDate.value = 0;
		SafeStrCpy( pProfit->sGuildName, pGuild->GetGuildName(), MAX_GUILD_NAME+1 );

		m_ProfitTable.Add( pProfit, pProfit->nSWMapNum );
	}

	if( m_nRelationSWMapNum == pProfit->nSWMapNum )
		m_pProfit = pProfit;
}

void CSiegeWarProfitMgr::ProcessDBUpdateProfitMoney()
{
	if( gCurTime > m_dwUpdateTime )
	{
		m_dwUpdateTime = gCurTime + 600000;
		if( !m_pProfit )	return;

		SiegeWarProfitUpdateProfitMoney( m_pProfit->nSWMapNum, m_dwMoneyForStore+m_dwMoneyForMonster ); 
		m_dwMoneyForStore = m_dwMoneyForMonster = 0;
	}
}

void CSiegeWarProfitMgr::NetworkMsgParse( DWORD dwConnectionIndex, BYTE Protocol, void* pMsg )
{
	switch( Protocol )
	{
	case MP_SIEGEWAR_PROFIT_INFO_SYN:				MsgProfitInfoSyn( pMsg );		break;
	case MP_SIEGEWAR_PROFIT_CHANGE_TEXRATE_SYN:		MsgChangTexrateSyn( pMsg );		break;
	case MP_SIEGEWAR_PROFIT_CHANGE_TEXRATE_NOTIFY_TO_MAP:	MsgChangeTexrateNotifyToMap( pMsg );	break;
	case MP_SIEGEWAR_PROFIT_GETMONEY_SYN:			MsgGetMoneySyn( pMsg );			break;		
	case MP_SIEGEWAR_PROFIT_CHANGE_GUILD_NOTIFY_TO_MAP:		MsgChangeGuildNotifyToMap( pMsg );	break;
	}
}

void CSiegeWarProfitMgr::MsgProfitInfoSyn( void* pMsg )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pPlayer )	return;

	if( !m_pProfit )
	{
		SendNackMsg( pPlayer, MP_SIEGEWAR_PROFIT_INFO_NACK, eSW_Not_Profit );
		return;
	}	
	if( pPlayer->GetGuildIdx() != m_pProfit->dwGuildIdx )
	{
		SendNackMsg( pPlayer, MP_SIEGEWAR_PROFIT_INFO_NACK, eSW_Not_ProfitGuild );
		return;
	}

	MSG_SW_PROFIT_INFO Msg;
	Msg.Category = MP_SIEGEWAR_PROFIT;
	Msg.Protocol = MP_SIEGEWAR_PROFIT_INFO_ACK;
	Msg.dwGuildIdx = m_pProfit->dwGuildIdx;
	SafeStrCpy( Msg.sGuildName, m_pProfit->sGuildName, MAX_GUILD_NAME+1 );
	Msg.nTaxRateForStore = m_pProfit->nTaxRateForStore;
	Msg.dwTotalMoney = m_pProfit->dwTotalMoney;

	pPlayer->SendMsg( &Msg, sizeof(Msg) );
}

void CSiegeWarProfitMgr::MsgChangTexrateSyn( void* pMsg )
{
	MSG_INT* pmsg = (MSG_INT*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pPlayer )	return;

	if( !m_pProfit )
	{
		SendNackMsg( pPlayer, MP_SIEGEWAR_PROFIT_CHANGE_TEXRATE_NACK, eSW_Not_Profit );
		return;
	}
	if( pPlayer->GetGuildIdx() != m_pProfit->dwGuildIdx )
	{
		SendNackMsg( pPlayer, MP_SIEGEWAR_PROFIT_CHANGE_TEXRATE_NACK, eSW_Not_ProfitGuild );
		return;
	}
	if( pPlayer->GetGuildMemberRank() != GUILD_MASTER )
	{
		SendNackMsg( pPlayer, MP_SIEGEWAR_PROFIT_CHANGE_TEXRATE_NACK, eSW_Not_ProfitGuildMaster );
		return;
	}
	if( pmsg->nData > 5 || pmsg->nData < 0 )
	{
		SendNackMsg( pPlayer, MP_SIEGEWAR_PROFIT_CHANGE_TEXRATE_NACK, eSW_No_TaxRate );
		return;
	}

	stTIME curTime;
	curTime.value = GetCurTime();
	if( curTime.value > m_pProfit->RegDate.value )
	{
		stTIME time;
		time.SetTime( 0, 0, 1, 0, 0, 0 );
		m_pProfit->RegDate = curTime;
		m_pProfit->RegDate += time;
	}
	else
	{
		SendNackMsg( pPlayer, MP_SIEGEWAR_PROFIT_CHANGE_TEXRATE_NACK, eSW_No_Time, m_pProfit->RegDate.value );
		return;
	}

	m_pProfit->nTaxRateForStore = pmsg->nData;

	MSG_INT Msg, Msg1;
	Msg.Category = MP_SIEGEWAR_PROFIT;
	Msg.Protocol = MP_SIEGEWAR_PROFIT_CHANGE_TEXRATE_ACK;
	Msg.nData = pmsg->nData;

	CObject* pObject = NULL;
	g_pUserTable->SetPositionUserHead();
	while( pObject = g_pUserTable->GetUserData() )
	{
		if( pObject->GetObjectKind() != eObjectKind_Player ) continue;
		Msg1 = Msg;
		pObject->SendMsg( &Msg1, sizeof(Msg1) );
	}

	MSG_INT2 SMsg;
	SMsg.Category = MP_SIEGEWAR_PROFIT;
	SMsg.Protocol = MP_SIEGEWAR_PROFIT_CHANGE_TEXRATE_NOTIFY_TO_MAP;
	SMsg.nData1 = m_pProfit->nSWMapNum;
	SMsg.nData2 = pmsg->nData;	// store
	g_Network.Send2AgentServer( (char*)&SMsg, sizeof(SMsg) );	

	// db update
	SiegeWarProfitUpdateTaxRate( m_pProfit->nSWMapNum, m_pProfit->nTaxRateForStore, m_pProfit->RegDate.value );
}

void CSiegeWarProfitMgr::MsgChangeTexrateNotifyToMap( void* pMsg )
{
	MSG_INT2* pmsg = (MSG_INT2*)pMsg;

	if( !m_pProfit )	return;

	sSWPROFIT* pProfit = m_ProfitTable.GetData( pmsg->nData1 );
	if( !pProfit )	return;
	
	
	pProfit->nTaxRateForStore = pmsg->nData2;

	MSG_INT Msg, Msg1;
	Msg.Category = MP_SIEGEWAR_PROFIT;
	Msg.Protocol = MP_SIEGEWAR_PROFIT_CHANGE_TEXRATE_ACK;
	Msg.nData = pProfit->nTaxRateForStore;

	CObject* pObject = NULL;
	g_pUserTable->SetPositionUserHead();
	while( pObject = g_pUserTable->GetUserData() )
	{
		if( pObject->GetObjectKind() != eObjectKind_Player ) continue;
		Msg1 = Msg;
		pObject->SendMsg( &Msg1, sizeof(Msg1) );
	}
}

void CSiegeWarProfitMgr::MsgGetMoneySyn( void* pMsg )
{
	MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	if( !pPlayer )	return;

	if( !m_pProfit )
	{
		SendNackMsg( pPlayer, MP_SIEGEWAR_PROFIT_GETMONEY_NACK, eSW_Not_Profit );
		return;
	}
	if( pPlayer->GetGuildIdx() != m_pProfit->dwGuildIdx )
	{
		SendNackMsg( pPlayer, MP_SIEGEWAR_PROFIT_GETMONEY_NACK, eSW_Not_ProfitGuild );
		return;
	}
	if( pPlayer->GetGuildMemberRank() != GUILD_MASTER )
	{
		SendNackMsg( pPlayer, MP_SIEGEWAR_PROFIT_GETMONEY_NACK, eSW_Not_ProfitGuildMaster );
		return;
	}
	if( m_pProfit->dwTotalMoney < pmsg->dwData )
	{
		SendNackMsg( pPlayer, MP_SIEGEWAR_PROFIT_GETMONEY_NACK, eSW_No_Money );
		return;
	}

	pPlayer->SetMoney( pmsg->dwData, MONEY_ADDITION, 0, eItemTable_Inventory, eMoneyLog_GetSWProfit );

	MSG_DWORD Msg;
	Msg.Category = MP_SIEGEWAR_PROFIT;
	Msg.Protocol = MP_SIEGEWAR_PROFIT_GETMONEY_ACK;
	Msg.dwData = pmsg->dwData;
	pPlayer->SendMsg( &Msg, sizeof(Msg) );

	// db update
	SiegeWarProfitUpdateProfitMoney( m_pProfit->nSWMapNum, pmsg->dwData, 1 );
}

void CSiegeWarProfitMgr::MsgChangeGuildNotifyToMap( void* pMsg )
{
	MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

	CGuild* pGuild = GUILDMGR->GetGuild( pmsg->dwData1 );
	if( !pGuild )	return;

	sSWPROFIT* pProfit = m_ProfitTable.GetData( pmsg->dwData2 );
	if( pProfit )
	{
		pProfit->dwGuildIdx = pmsg->dwData1;
		pProfit->nTaxRateForStore = 0;
		pProfit->RegDate.value = 0;
		SafeStrCpy( pProfit->sGuildName, pGuild->GetGuildName(), MAX_GUILD_NAME+1 );
	}
	else
	{
		pProfit = new sSWPROFIT;
		pProfit->dwGuildIdx = pmsg->dwData1;
		pProfit->nSWMapNum = pmsg->dwData2;
		pProfit->dwTotalMoney = 0;
		pProfit->nTaxRateForStore = 0;
		pProfit->RegDate.value = 0;
		SafeStrCpy( pProfit->sGuildName, pGuild->GetGuildName(), MAX_GUILD_NAME+1 );

		m_ProfitTable.Add( pProfit, pProfit->nSWMapNum );
	}

	if( m_nRelationSWMapNum == pProfit->nSWMapNum )
		m_pProfit = pProfit;
}

void CSiegeWarProfitMgr::SendNackMsg( CPlayer* pPlayer, BYTE Protocol, DWORD dwError, DWORD dwData )
{
	MSG_DWORD2 Msg;
	Msg.Category = MP_SIEGEWAR_PROFIT;
	Msg.Protocol = Protocol;
	Msg.dwData1 = dwError;
	Msg.dwData2 = dwData;

	pPlayer->SendMsg( &Msg, sizeof(Msg) );
}