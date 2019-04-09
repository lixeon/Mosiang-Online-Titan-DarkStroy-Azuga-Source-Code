// SiegeWarMgr.cpp: implementation of the CSiegeWarMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SiegeWarMgr.h"
#include "MHMap.h"
#include "MapChange.h"
#include "MainGame.h"
#include "GameIn.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"
#include "ChatManager.h"
#include "./interface/cWindowManager.h"
#include "WindowIDEnum.h"
#include "cMsgBox.h"
#include "SeigeWarDialog.h"
#include "BattleSystem_Client.h"
#include "../[CC]BattleSystem/SiegeWar/Battle_SiegeWar.h"
#include "MHFile.h"
#include "SeigeWarDialog.h"
#include "TileManager.h"
#include "MapObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSiegeWarMgr::CSiegeWarMgr()
{
}

CSiegeWarMgr::~CSiegeWarMgr()
{

}


void CSiegeWarMgr::Init()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if( !file.Init( "Resource/SiegeWarMapInfo.bin", "rb" ) )
		return;
#else
	if( !file.Init( "Resource/SiegeWarMapInfo.txt", "rt" ) )
		return;
#endif


	memset( m_SiegeWarMapNum, 0, sizeof(DWORD)*SIEGEWAR_MAX_SIEGEMAP*SIEGEWAR_MAX_AFFECTED_MAP );

	DWORD count = 0;
	while( 1 )
	{
		char buf[32] = { 0, };
		strcpy( buf, file.GetString() );

		if( strcmp( buf, "#VILLAGEMAP" ) == 0 )
		{
			DWORD mapcount = file.GetDword();

			for(DWORD i=0; i<mapcount; ++i)
				m_SiegeWarMapNum[count][i] = file.GetDword();
		}
		else if( strcmp( buf, "END" ) == 0 )
			break;

		++count;
	}
	file.Release();

	m_SiegeMapCount = count;

	LoadCastleGateInfo();
}

void CSiegeWarMgr::LoadCastleGateInfo()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if( !file.Init( "Resource/CastleGateList.bin", "rb" ) )
		return;
#else
	if( !file.Init( "Resource/CastleGateList.txt", "rt" ) )
		return;
#endif

	
	m_CastleGateTotalCount = file.GetInt();	

	for(int i=0; i<m_CastleGateTotalCount; ++i)
	{
		memset( &m_CastleGateInfoList[i], 0, sizeof(CASTLEGATE_BASEINFO) );

		m_CastleGateInfoList[i].Index = file.GetDword();
		m_CastleGateInfoList[i].MapNum = file.GetDword();		
		file.GetString( m_CastleGateInfoList[i].Name );
		m_CastleGateInfoList[i].Kind = file.GetDword();
		file.GetString( m_CastleGateInfoList[i].DataName );
		for(int k=0; k<eCastleGateLevel_Max; ++k)
		{
			m_CastleGateInfoList[i].Life[k] = file.GetDword();
			m_CastleGateInfoList[i].Shield[k] = file.GetDword();
		}
		m_CastleGateInfoList[i].Defence = file.GetDword();
		for(k=0; k<ATTR_MAX; ++k)
			m_CastleGateInfoList[i].Regist.SetElement_Val( ATTR_FIRE+k, file.GetFloat() );
		m_CastleGateInfoList[i].Radius = file.GetFloat();
		m_CastleGateInfoList[i].Position.x = file.GetFloat();
		m_CastleGateInfoList[i].Position.z = file.GetFloat();
		m_CastleGateInfoList[i].Width = file.GetDword();
		m_CastleGateInfoList[i].Wide = file.GetDword();
		m_CastleGateInfoList[i].Angle = file.GetFloat();
		m_CastleGateInfoList[i].Scale = file.GetFloat();		
	}

	file.Release();
}


DWORD CSiegeWarMgr::GetSiegeWarMapNum()
{
	for(DWORD i=0; i<m_SiegeMapCount; ++i)
	{
		if( m_SiegeWarMapNum[i][0] == MAP->GetMapNum() ||
			m_SiegeWarMapNum[i][1] == MAP->GetMapNum() )
				 return m_SiegeWarMapNum[i][1];
	}

	return 0;
}


void CSiegeWarMgr::AddPlayer( CPlayer* pPlayer )
{
}


/*void CSiegeWarMgr::CreateCastleGate( SEND_GATE_TOTALINFO* pGateInfo )
{
	BASEOBJECT_INFO objectinfo = pGateInfo->BaseObjectInfo;
	BASEMOVE_INFO moveinfo;
	MAPOBJECT_INFO moinfo;

	for(int i=0; i<m_CastleGateTotalCount; ++i)
	{
		if( m_CastleGateInfoList[i].Index == objectinfo.dwObjectID )
		{
			memset( &moveinfo, 0, sizeof(moveinfo) );
			moveinfo.CurPosition = m_CastleGateInfoList[i].Position;

			moinfo.Life = pGateInfo->Life;
			moinfo.MaxLife = m_CastleGateInfoList[i].Life[pGateInfo->Level];
			moinfo.Shield = pGateInfo->Shield;
			moinfo.MaxShield = m_CastleGateInfoList[i].Shield[pGateInfo->Level];
			moinfo.Radius = m_CastleGateInfoList[i].Radius;
			moinfo.PhyDefence = m_CastleGateInfoList[i].Defence;
			moinfo.AttrRegist = m_CastleGateInfoList[i].Regist;

			CMapObject* pGate = AddCastleGate( &objectinfo, &moveinfo, &m_CastleGateInfoList[i] );
			if( !pGate )		return;

			pGate->InitMapObject( &moinfo );
			break;
		}
	}
}*/


void CSiegeWarMgr::CreateCastleGate( CASTLEGATE_SENDINFO* pGateInfo )
{
	BASEOBJECT_INFO objectinfo;
	BASEMOVE_INFO moveinfo;
	MAPOBJECT_INFO moinfo;

	for(int i=0; i<m_CastleGateTotalCount; ++i)
	{
		if( m_CastleGateInfoList[i].Index == pGateInfo->Index )
		{
			memset( &moveinfo, 0, sizeof(moveinfo) );
			moveinfo.CurPosition = m_CastleGateInfoList[i].Position;

			objectinfo.BattleID = BATTLESYSTEM->GetBattle(HERO)->GetBattleID();
			objectinfo.dwObjectID = pGateInfo->Index;
			objectinfo.BattleTeam = eBattleTeam1;
			SafeStrCpy( objectinfo.ObjectName, m_CastleGateInfoList[i].Name, CASTLEGATE_NAME_LENGTH+1 );

			moinfo.Life = pGateInfo->Life;
			moinfo.MaxLife = m_CastleGateInfoList[i].Life[pGateInfo->Level];
			moinfo.Shield = pGateInfo->Shield;
			moinfo.MaxShield = m_CastleGateInfoList[i].Shield[pGateInfo->Level];
			moinfo.Radius = m_CastleGateInfoList[i].Radius;
			moinfo.PhyDefence = m_CastleGateInfoList[i].Defence;
			moinfo.AttrRegist = m_CastleGateInfoList[i].Regist;

			CMapObject* pGate = AddCastleGate( &objectinfo, &moveinfo, &m_CastleGateInfoList[i] );
			if( !pGate )		return;

			pGate->InitMapObject( &moinfo );
			break;
		}
	}
}


CMapObject* CSiegeWarMgr::AddCastleGate( BASEOBJECT_INFO* pObjectInfo, BASEMOVE_INFO* pMoveInfo, CASTLEGATE_BASEINFO* pMapobjInfo )
{
	CMapObject* pCastleGate = (CMapObject*)OBJECTMGR->AddMapObject( pObjectInfo, pMoveInfo, pMapobjInfo->DataName, pMapobjInfo->Scale, pMapobjInfo->Angle );
	if( !pCastleGate )			return NULL;	
	
	//
	CTileManager* pTileManager = MAP->GetTileManager();
	if( !pTileManager )			return	pCastleGate;

	float fx, fz = pMoveInfo->CurPosition.z;
	VECTOR3 vStgPos;
	float fAdd = -50.0f;

	for( fx = pMoveInfo->CurPosition.x - pMapobjInfo->Width ; fx <= pMoveInfo->CurPosition.x + pMapobjInfo->Width + fAdd ; fx += 50.0f )//
	{
		vStgPos.x = fx;
		vStgPos.z = fz - pMapobjInfo->Wide;
		if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
			pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.z = fz + pMapobjInfo->Wide + fAdd;
		if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
			pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}

	fx = pMoveInfo->CurPosition.x;
	for( fz = pMoveInfo->CurPosition.z - pMapobjInfo->Wide ; fz <= pMoveInfo->CurPosition.z + pMapobjInfo->Wide + fAdd; fz += 50.0f )
	{
		vStgPos.x = fx - pMapobjInfo->Width;
		vStgPos.z = fz;
		if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
			pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.x = fx + pMapobjInfo->Width + fAdd;
		vStgPos.z = fz;
		if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
			pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}

	pTileManager->AddTileAttrByAreaData( NULL, &pMoveInfo->CurPosition, SKILLAREA_ATTR_BLOCK );	

	return pCastleGate;

/*	for( fx = moveinfo.CurPosition.x - 350.0f ; fx <= moveinfo.CurPosition.x + 350.0f + fAdd ; fx += 50.0f )//
	{
		vStgPos.x = fx;
		vStgPos.z = fz - 150.0f;
		if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
			pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.z = fz + 150.0f + fAdd;
		if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
			pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}

	fx = moveinfo.CurPosition.x;
	for( fz = moveinfo.CurPosition.z - 150.0f ; fz <= moveinfo.CurPosition.z + 150.0f + fAdd; fz += 50.0f )
	{
		vStgPos.x = fx - 350.0f;
		vStgPos.z = fz;
		if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
			pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.x = fx + 350.0f + fAdd;
		vStgPos.z = fz;
		if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
			pTileManager->AddTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}*/
}


void CSiegeWarMgr::DeleteCastleGate( DWORD GateID )
{
	VECTOR3 Position;
	DWORD Width = 0;
	DWORD Wide = 0;

	for(int i=0; i<m_CastleGateTotalCount; ++i)
	{
		if( m_CastleGateInfoList[i].Index == GateID )
		{
			Position = m_CastleGateInfoList[i].Position;
			Width = m_CastleGateInfoList[i].Width;
			Wide = m_CastleGateInfoList[i].Wide;
			CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(1148), m_CastleGateInfoList[i].Name );
			break;
		}
	}

	OBJECTMGR->AddGarbageObject( OBJECTMGR->GetObject( GateID ) );
	
	CTileManager* pTileManager = MAP->GetTileManager();
	if( !pTileManager )			return;

	float fx, fz = Position.z;
	VECTOR3 vStgPos;
	float fAdd = -50.0f;

	for( fx = Position.x - Width ; fx <= Position.x + Width + fAdd ; fx += 50.0f )//
	{
		vStgPos.x = fx;
		vStgPos.z = fz - Wide;
		if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
			pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.z = fz + Wide + fAdd;
		if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
			pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}

	fx = Position.x;
	for( fz = Position.z - Wide ; fz <= Position.z + Wide + fAdd; fz += 50.0f )
	{
		vStgPos.x = fx - Width;
		vStgPos.z = fz;
		if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
			pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
		vStgPos.x = fx + Width + fAdd;
		vStgPos.z = fz;
		if( vStgPos.x > 0 && vStgPos.x < 51200 && vStgPos.z > 0 && vStgPos.z < 51200 )
			pTileManager->RemoveTileAttrByAreaData( NULL, &vStgPos, SKILLAREA_ATTR_BLOCK );
	}

	pTileManager->RemoveTileAttrByAreaData( NULL, &Position, SKILLAREA_ATTR_BLOCK );
}


void CSiegeWarMgr::NetworkMsgParse( BYTE Protocol,void* pMsg )
{
	switch( Protocol )
	{
	case MP_SIEGEWAR_MOVEIN_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			DWORD Mapnum = GetSiegeWarMapNum();
			if( pmsg->dwData )
				MAPCHANGE->SetGameInInitKind( eGameInInitKind_SWEnter );
			else
				MAPCHANGE->SetGameInInitKind( eGameInInitKind_SWObserverEnter );
			MAINGAME->SetGameState( eGAMESTATE_MAPCHANGE, &Mapnum, 4 );
			OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
			GAMEIN->SetGuildIdx( HERO->GetGuildIdx() );
			GAMEIN->SetMoveMap( MAP->GetMapNum() );
			GAMEIN->SetSiegeWarMapNum( Mapnum );
			CHATMGR->SaveChatList();
			
			WINDOWMGR->MsgBox( MBI_NOBTNMSGBOX, MBT_NOBTN, CHATMGR->GetChatMsg(1001) );			
		}
		break;
	case MP_SIEGEWAR_MOVEIN_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(932) );
		}
		break;
	case MP_SIEGEWAR_MOVEOUT_NACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(932) );
		}
		break;
	case MP_SIEGEWAR_BATTLEJOIN_ACK:
		{
			SEND_SW_INITINFO* pmsg = (SEND_SW_INITINFO*)pMsg;

			GAMEIN->GetSWTimeDlg()->StartTime( pmsg->Time );
			GAMEIN->GetSWTimeDlg()->SetMunpaName( pmsg->GuildName );

			for(int i=0; i<pmsg->GateCount; ++i)
				CreateCastleGate( &pmsg->GateInfo[i] );
		}
		break;
	case MP_SIEGEWAR_BATTLEJOIN_NACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(932) );
		}
		break;
	case MP_SIEGEWAR_OBSERVERJOIN_NACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(932) );
		}
		break;	
	case MP_SIEGEWAR_ATTACK_REGIST_ACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1034) );
		}
		break;
	case MP_SIEGEWAR_ATTACK_REGIST_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			switch( pmsg->dwData )
			{
			case eSWError_NoGuildInfo:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1035) );				
				break;
			case eSWError_NoProposalTime:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1036) );				
				break;
			case eSWError_MyUnionIsCastleUnion:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1037) );				
				break;
			case eSWError_AlreadyAttackRegist:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1038) );				
				break;
			case eSWError_AlreadyDefenceProposal:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1035) );				
				break;
			case eSWError_MyUnionIsOtherTeam:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1037) );				
				break;				
			case eSWError_OverGuildCount:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1035) );
				break;
			}
		}
		break;	
	case MP_SIEGEWAR_ACCEPTGUILD_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			
			switch( pmsg->dwData )
			{
			case eSWError_NoCastleGuild:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1052) );				
				break;
			case eSWError_NoGuildMaster:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1052) );
				break;
			case eSWError_NoGuildInfo:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1054) );
				break;
			case eSWError_NoProposalGuild:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1054) );
				break;
			case eSWError_NoAcceptTime:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1049) );
				break;
			}
		}
		break;	
	// for interface
	case MP_SIEGEWAR_REGISTTIME_ACK:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
		}
		break;
	case MP_SIEGEWAR_REGISTTIME_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			
			switch( pmsg->dwData )
			{
			case eSWError_NoProposalTime:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1051) );
				break;
			case eSWError_NoCastleGuild:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1052) );				
				break;
			case eSWError_NoGuildMaster:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1052) );
				break;
			case eSWError_FailedRegistSiegeWarTime:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1053) );
				break;
			}
		}
		break;
	case MP_SIEGEWAR_DEFENCE_REGIST_ACK:
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1042) );
		}
		break;
	case MP_SIEGEWAR_DEFENCE_REGIST_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			switch( pmsg->dwData )
			{
			case eSWError_NoGuildInfo:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1039) );				
				break;
			case eSWError_NoCastleGuild:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1056) );
				break;
			case eSWError_NoProposalTime:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1036) );
				break;
			case eSWError_MyUnionIsOtherTeam:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1041) );				
				break;
			case eSWError_AlreadyAttackRegist:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1038) );				
				break;
			case eSWError_MyUnionIsCastleUnion:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1040) );				
				break;
			case eSWError_AlreadyDefenceProposal:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1042) );				
				break;
			case eSWError_OverGuildCount:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1039) );				
				break;
			}
		}
		break;
	case MP_SIEGEWAR_PROPOSALLIST_ACK:	// 수성 참여 리스트
		{
			SEND_SW_PROPOSALGUILDLIST* pmsg = (SEND_SW_PROPOSALGUILDLIST*)pMsg;
			GAMEIN->GetSWProtectRegDlg()->ShowList( pmsg );
		}
		break;
	case MP_SIEGEWAR_PROPOSALLIST_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			// dwData : 에러코드		
			GAMEIN->GetSWProtectRegDlg()->SetActive( FALSE );

			switch( pmsg->dwData )
			{
			case 1:			// 실패
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1047) );
				break;
			case 2:			// 성길드가 아니다.
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1048) );
				break;
			case 3:			// 길마가 아니다.
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1048) );
				break;
			}
		}
		break;
	case MP_SIEGEWAR_GUILDLIST_ACK:		// 공,수성 리스트
		{
			SEND_SW_GUILDLIST* pmsg = (SEND_SW_GUILDLIST*)pMsg;
			GAMEIN->GetSWInfoDlg()->ShowSWInfo( pmsg );
		}
		break;
	case MP_SIEGEWAR_GUILDLIST_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			// dwData : 에러코드
			GAMEIN->GetSWInfoDlg()->SetActive( FALSE );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1045) );
		}
		break;
	case MP_SIEGEWAR_TAXRATE_ACK:		// 세율변경
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
		}
		break;
	case MP_SIEGEWAR_TAXRATE_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			// dwData : 에러코드

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(932) );
		}
		break;
	case MP_SIEGEWAR_CHANGESTATE:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			switch( pmsg->dwData )
			{
			case eSWState_Before:
				break;
			case eSWState_Proclamation:
				CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(1002) );
				break;
			case eSWState_Acceptance:
				CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(1003) );
				break;
			case eSWState_BeforeSiegeWar:
				CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(1150) );
				break;
			case eSWState_SiegeWar:
				CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(1004) );
				break;
			case eSWState_EndSiegeWar:
				CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(1005) );
				GAMEIN->GetSWTimeDlg()->EndTime();
				break;
			}
		}
		break;
	case MP_SIEGEWAR_RETURNTOMAP:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			MAPCHANGE->SetGameInInitKind( eGameInInitKind_SWLeave );
			DWORD MapNum = pmsg->dwData;
			MAINGAME->SetGameState( eGAMESTATE_MAPCHANGE, &MapNum, 4 );
			CHATMGR->SaveChatList();
		}
		break;
	case MP_SIEGEWAR_ENGRAVE_ACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwData2 );
			if( !pPlayer )			return;
			
			if( pmsg->dwData1 == 0 )			// 각인시작
			{
				if( pPlayer->GetID() == HEROID )
				{
					if( GAMEIN->GetSWEngraveDlg() )
						GAMEIN->GetSWEngraveDlg()->StartEngrave();
				}
				
				if( pPlayer->GetState() == eObjectState_Deal )
					OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
				OBJECTSTATEMGR->StartObjectState(pPlayer, eObjectState_Engrave);
				//pPlayer->SetState(eObjectState_Engrave);
				
				// Effect는???
				OBJECTEFFECTDESC desc(FindEffectNum("eff_gakin_s.beff"));
				pPlayer->AddObjectEffect( ENGRAVE_EFFECTID, &desc, 1, HERO );
			}
			else if( pmsg->dwData1 == 1 )		// 각인성공
			{
				pPlayer->RemoveObjectEffect( ENGRAVE_EFFECTID );
				
				if( OBJECTSTATEMGR->GetObjectState( pPlayer ) == eObjectState_Engrave )
					OBJECTSTATEMGR->EndObjectState( pPlayer, eObjectState_Engrave );

				if( GAMEIN->GetSWEngraveDlg() )
				{
					if( GAMEIN->GetSWEngraveDlg()->IsActive() )
						GAMEIN->GetSWEngraveDlg()->CancelEngrave();
				}

				pPlayer->RemoveObjectEffect( ENGRAVE_ACK_EFFECTID );
				OBJECTEFFECTDESC desc(FindEffectNum("eff_gakin_suc.beff"));
				pPlayer->AddObjectEffect( ENGRAVE_ACK_EFFECTID, &desc, 1, HERO );
			}
		}
		break;
	case MP_SIEGEWAR_ENGRAVE_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			
			switch( pmsg->dwData )
			{
			case eSWError_NoGuildMaster:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1010) );
				break;
			case eSWError_CastleGuild:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1012) );				
				break;
			case eSWError_NoGuildInfo:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1013) );
				break;
			case eSWError_NoPerfectGuild:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1011) );				
				break;
			case eSWError_OtherPlayerEngrave:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1014) );
				break;
			case eSWError_NoBaseVillage:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1015) );
				break;
			case eSWError_NoSiegeWarTime:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1016) );
				break;
			case eSWError_Observer:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1055) );
				break;
			}

			//
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );			
			if( HERO->GetState() == eObjectState_Engrave )						//Merge! ?
				OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Engrave );			
		}
		break;
	case MP_SIEGEWAR_ENGRAVE_CANCEL:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwData1 );
			if( !pPlayer )		return;

			//
			pPlayer->RemoveObjectEffect( ENGRAVE_EFFECTID );
			if( OBJECTSTATEMGR->GetObjectState( pPlayer ) == eObjectState_Engrave )
				OBJECTSTATEMGR->EndObjectState( pPlayer, eObjectState_Engrave );

			pPlayer->RemoveObjectEffect( ENGRAVE_NACK_EFFECTID );
			OBJECTEFFECTDESC desc(FindEffectNum("eff_gakin_fail.beff"));
			pPlayer->AddObjectEffect( ENGRAVE_NACK_EFFECTID, &desc, 1, HERO );

			if( pmsg->dwData1 != HEROID )		return;

			switch( pmsg->dwData2 )
			{
			case 0:			// 공성종료
				break;
			case 1:			// 공격받아 실패
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1013) );
				break;
			case 2:			// 각인취소시 에러
				break;
			case 3:			// 각인취소
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1044) );
				break;
			}

			if( GAMEIN->GetSWEngraveDlg() )
				GAMEIN->GetSWEngraveDlg()->CancelEngrave();
		}
		break;
	case MP_SIEGEWAR_RESTRAINT_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			HERO->SetRestraintMode( TRUE );		
		}
		break;
	case MP_SIEGEWAR_RESTRAINT_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			switch( pmsg->dwData )
			{
			case 1:			// 공성맵이 아니다.
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1050) );
				break;
			case 2:			// 공성중이 아니다.
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1050) );
				break;
			case 3:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1055) );
				break;
			}

			HERO->SetRestraintMode( FALSE );
		}
		break;
	case MP_SIEGEWAR_RESTRAINT_ON:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwData );
			if( !pPlayer )			return;

			pPlayer->SetRestraintMode( TRUE );
			OBJECTMGR->ApplyOverInfoOption( pPlayer );

			if( pPlayer->InTitan() )
			{
				OBJECTEFFECTDESC desc(FindEffectNum("t_maintain_PK_S.beff"));
				pPlayer->AddObjectEffect( PK_EFFECT_ID, &desc, 1, HERO);

			}
			else
			{
				OBJECTEFFECTDESC desc(FindEffectNum("maintain_PK_S.beff"));
				pPlayer->AddObjectEffect( PK_EFFECT_ID, &desc, 1, HERO);			
			}

			if( pPlayer->GetID() == HEROID )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1032) );
				OBJECTMGR->RefreshSiegeNamePannel();
			}
			else
				((CBattle_SiegeWar*)BATTLESYSTEM->GetBattle(HERO))->SetSiegeWarName( pPlayer, pPlayer->GetBattleTeam(), pPlayer->GetGuildIdx() );
		}
		break;
	case MP_SIEGEWAR_RESTRAINT_OFF:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CPlayer* pPlayer = (CPlayer*)OBJECTMGR->GetObject( pmsg->dwData );
			if( !pPlayer )			return;

			pPlayer->SetRestraintMode( FALSE );			
			OBJECTMGR->ApplyOverInfoOption( pPlayer );
			pPlayer->RemoveObjectEffect( PK_EFFECT_ID );

			if( pPlayer->GetID() == HEROID )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1033) );
				OBJECTMGR->RefreshSiegeNamePannel();
			}
			else
				((CBattle_SiegeWar*)BATTLESYSTEM->GetBattle(HERO))->SetSiegeWarName( pPlayer, pPlayer->GetBattleTeam(), pPlayer->GetGuildIdx() );
		}
		break;
	case MP_SIEGEWAR_BATTLEINFO:			// 공성 성공후 정보를 다시 받을때
		{
			SEND_SW_BTGUILDLIST* pmsg = (SEND_SW_BTGUILDLIST*)pMsg;
			
			((CBattle_SiegeWar*)BATTLESYSTEM->GetBattle(HERO))->SetBattleInfo( pmsg );
			HERO->SetBattleTeam( pmsg->Team );

			if( GAMEIN->GetSWEngraveDlg()->IsActive() )
			{
				GAMEIN->GetSWEngraveDlg()->SetActive( FALSE );
				if( HERO->GetState() == eObjectState_Engrave )
					OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Engrave );
			}
		}
		break;
	case MP_SIEGEWAR_SUCCESS_SIEGEWAR:
		{
			SEND_SW_SUCCESSINFO* pmsg = (SEND_SW_SUCCESSINFO*)pMsg;

			CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(1019), pmsg->GuildName );
			GAMEIN->GetSWTimeDlg()->SetMunpaName( pmsg->GuildName );
		}
		break;
	case MP_SIEGEWAR_CASTLEGATE_DELETE:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			DeleteCastleGate( pmsg->dwData );
		}
		break;
	case MP_SIEGEWAR_ENGRAVE_START:
		{
			SEND_SW_ENGRAVE* pmsg = (SEND_SW_ENGRAVE*)pMsg;

			CHATMGR->AddMsg( CTC_GTOURNAMENT, CHATMGR->GetChatMsg(1147), pmsg->GuildName, pmsg->CharacterName );
		}
		break;
	case MP_SIEGEWAR_TIMEINFO_ACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			GAMEIN->GetSWStartTimeDlg()->ShowStartTimeDlg( pmsg->dwData1, pmsg->dwData2 );
		}
		break;
	case MP_SIEGEWAR_TIMEINFO_NACK:
		{
			GAMEIN->GetSWStartTimeDlg()->SetActive( FALSE );

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1177) );
		}
		break;
	case MP_SIEGEWAR_UPGRADE_GATE_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1179) );
		}
		break;
	case MP_SIEGEWAR_UPGRADE_GATE_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			switch( pmsg->dwData )
			{
			case 0:
			case 1:
			case 2:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(726) );
				break;
			case 3:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(117) );
				break;
			case 4:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1479) );
				break;
			case 5:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1478) );
				break;
			}
		}
		break;
	case MP_SIEGEWAR_FLAGCHANGE:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*) pMsg;

			OBJECTMGR->ChangeFlagNPC(eSGFlg, pmsg->dwData);
		}
		break;
		/*
	case MP_SIEGEWAR_FLAGCHANGE:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			switch( pmsg->dwData1 )
			{
			case eSWState_Before:
				{
					OBJECTMGR->ChangeFlag( pmsg->dwData1, pmsg->dwData2 );
				}
				break;
			case eSWState_Acceptance:
				{
					OBJECTMGR->ChangeFlag( pmsg->dwData1, pmsg->dwData2 );
				}
				break;
			case eSWState_SiegeWar:
				{
					OBJECTMGR->ChangeFlag( pmsg->dwData1, pmsg->dwData2 );
				}
				break;
			}
		}
		break;*/
	}
}




















//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CSiegeWarProfit)
CSiegeWarProfit::CSiegeWarProfit()
{
	m_dwGuildIdx = 0;
	memset( m_sGuildName, 0, MAX_GUILD_NAME+1 );
	m_nTaxRate = 0;
	m_dwMoney = 0;
}

CSiegeWarProfit::~CSiegeWarProfit()
{
}

void CSiegeWarProfit::Init()
{
	m_dwGuildIdx = 0;
	memset( m_sGuildName, 0, MAX_GUILD_NAME+1 );
	m_nTaxRate = 0;
	m_dwMoney = 0;
}

DWORD CSiegeWarProfit::CalTexRateForSell( DWORD dwMoney )
{
	if( m_nTaxRate == 0 )	return dwMoney;

	return ( dwMoney - dwMoney * m_nTaxRate / 100 );
}

DWORD CSiegeWarProfit::CalTexRateForBuy( DWORD dwMoney )
{
	if( m_nTaxRate == 0 )	return dwMoney;

	return ( dwMoney + dwMoney * m_nTaxRate / 100 );
}

void CSiegeWarProfit::NetworkMsgParse( BYTE Protocol,void* pMsg )
{
	switch( Protocol )
	{
	case MP_SIEGEWAR_PROFIT_INFO:
		{
			MSG_SW_PROFIT_INFO* pmsg = (MSG_SW_PROFIT_INFO*)pMsg;

			Init();
			if( pmsg->dwGuildIdx )
			{
				m_dwGuildIdx = pmsg->dwGuildIdx;
				SafeStrCpy( m_sGuildName, pmsg->sGuildName, MAX_GUILD_NAME+1 );
				m_nTaxRate = pmsg->nTaxRateForStore;
				m_dwMoney = pmsg->dwTotalMoney;

				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1020), m_sGuildName, m_nTaxRate );
			}
		}
		break;
	case MP_SIEGEWAR_PROFIT_INFO_ACK:
		{
			MSG_SW_PROFIT_INFO* pmsg = (MSG_SW_PROFIT_INFO*)pMsg;

			m_dwGuildIdx = pmsg->dwGuildIdx;
			SafeStrCpy( m_sGuildName, pmsg->sGuildName, MAX_GUILD_NAME+1 );
			m_nTaxRate = pmsg->nTaxRateForStore;
			m_dwMoney = pmsg->dwTotalMoney;

			GAMEIN->GetSWProfitDlg()->ShowProfitDlg( m_nTaxRate, m_dwMoney );
		}
		break;
	case MP_SIEGEWAR_PROFIT_INFO_NACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
			
			switch( pmsg->dwData1 )
			{
			case eSW_Not_Profit:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1021) );	break;
			case eSW_Not_ProfitGuild:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(726) );	break;
			}
			return ;
		}
		break;
	case MP_SIEGEWAR_PROFIT_CHANGE_TEXRATE_ACK:
		{
			MSG_INT* pmsg = (MSG_INT*)pMsg;
			m_nTaxRate = pmsg->nData;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1022), m_nTaxRate );
		}
		break;
	case MP_SIEGEWAR_PROFIT_CHANGE_TEXRATE_NACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			switch( pmsg->dwData1 )
			{
			case eSW_Not_Profit:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1021) );	break;
			case eSW_Not_ProfitGuild:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1023) );	break;
			case eSW_Not_ProfitGuildMaster:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1024) );	break;
			case eSW_No_TaxRate:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1025) );	break;
			case eSW_No_Time:
				{
					stTIME time;
					time.value = pmsg->dwData2;
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1026), 
						time.GetYear()+2000, time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute() );
				}
				break;
			}
		}
		break;
	case MP_SIEGEWAR_PROFIT_GETMONEY_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
	
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1027), pmsg->dwData );
		}
		break;	
	case MP_SIEGEWAR_PROFIT_GETMONEY_NACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			switch( pmsg->dwData1 )
			{
			case eSW_Not_Profit:		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1021) );	break;
			case eSW_Not_ProfitGuild:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1023) );	break;
			case eSW_Not_ProfitGuildMaster:	CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1024) );	break;
			case eSW_No_Money:			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1028) );	break;
			}
		}
		break;
	}
}

void CSiegeWarProfit::SendProfitInfoSyn()
{
	if( !m_dwGuildIdx )
	{
		if( HERO->GetState() == eObjectState_Deal )
			OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );

		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1021) );
		return ;
	}

	MSGBASE Msg;
	Msg.Category = MP_SIEGEWAR_PROFIT;
	Msg.Protocol = MP_SIEGEWAR_PROFIT_INFO_SYN;
	Msg.dwObjectID = HEROID;	
	NETWORK->Send( &Msg, sizeof(Msg) );
}

void CSiegeWarProfit::SendChangTexrateSyn( int nTaxRate )
{
	MSG_INT Msg;
	Msg.Category = MP_SIEGEWAR_PROFIT;
	Msg.Protocol = MP_SIEGEWAR_PROFIT_CHANGE_TEXRATE_SYN;
	Msg.dwObjectID = HEROID;
	Msg.nData = nTaxRate;
	NETWORK->Send( &Msg, sizeof(Msg) );
}

void CSiegeWarProfit::SendGetMoneySyn( DWORD dwMoney )
{
	MSG_DWORD Msg;
	Msg.Category = MP_SIEGEWAR_PROFIT;
	Msg.Protocol = MP_SIEGEWAR_PROFIT_GETMONEY_SYN;
	Msg.dwObjectID = HEROID;
	Msg.dwData = dwMoney;
	NETWORK->Send( &Msg, sizeof(Msg) );
}

BOOL CSiegeWarProfit::CheckSWProfit( int nType, int nTaxRate, DWORD dwMoney )
{
//	return TRUE;
	switch( nType )
	{
	case 0:		// tax change
		{
			if( nTaxRate > 5 || m_nTaxRate == nTaxRate )
			{
				// chat msg
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1025) );
				return FALSE;
			}
		}
		break;
	case 1:		// profit
		{
			if( dwMoney > m_dwMoney || dwMoney <= 0 )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1028) );
				return FALSE;
			}
		}
		break;
	}
	if( HERO->GetGuildIdx() != m_dwGuildIdx )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1023) );
		return FALSE;
	}
	if( HERO->GetGuildMemberRank() != GUILD_MASTER )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1024) );
		return FALSE;
	}

	return TRUE;
}
