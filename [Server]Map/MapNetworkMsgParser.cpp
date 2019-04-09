#include "stdafx.h"
#include "MapNetworkMsgParser.h"
#include "Network.h"
#include "MapDBMsgParser.h"
#include "CharMove.h"
#include "PackedData.h"
#include "MugongManager.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "Player.h"
#include "Pet.h"
#include "Npc.h"
#include "PartyManager.h"
#include "UserTable.h"
#include "UngijosikManager.h"
#include "PeaceWarModManager.h"
#include "ItemManager.h"
#include "QuickManager.h"
#include "TacticManager.h"
#include "SkillManager_Server.h"
#include "StreetStallManager.h"
#include "PyogukManager.h"
#include "BattleSystem_Server.h"
#include "CharacterManager.h"
#include "BootManager.h"
#include "objectstatemanager.h"
#include "PKManager.h"
#include "AbilityManager.h"
#include "FriendManager.h"
#include "ChannelSystem.h"
#include "RecallManager.h"
#include "WantedManager.h"
#include "QuestManager.h"
#include "LootingManager.h"
#include "JournalManager.h"
#include "Party.h"
#include "CharacterCalcManager.h"
#include "..\[CC]Suryun\SuryunManager_Server.h"
#include "FameManager.h"
#include "ObjectFactory.h"
#include "Guild.h"
#include "GuildManager.h"
#include "EventMapMgr.h"
#include "GuildFieldWarMgr.h"
#include "PartyWarMgr.h"
#include "QuestMapMgr.h"
#include "GuildTournamentMgr.h"
#include "SiegeWarMgr.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "GuildUnionManager.h"
#include "cJackpotManager.h"
#include "SiegeWarProfitMgr.h"
#include "Monster.h"
#include "Pet.h"
#include "PetManager.h"
#include "AttackManager.h"	// magi82 cheat damage
#include "WeatherManager.h"
#include "SurvivalModeManager.h"
#include "BobusangManager.h"
#include "TitanManager.h"
#include "GridSystem.h"
#include "ItemLimitManager.h"
#include "GameEventManager.h"
#include "AutoNoteManager.h"
#include "FortWarManager.h"


extern HWND g_hWnd;
extern BOOL g_bCloseWindow;
extern int	g_nHackCheckNum;
extern int	g_nHackCheckWriteNum;
extern BOOL g_bPlusTime;

void MP_MonitorMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_MORNITORMAPSERVER_NOTICESEND_SYN:
		{
			// 공지사항 
			ASSERT(0);
		}
		break;
	case MP_MORNITORMAPSERVER_PING_SYN:
		{
			pTempMsg->Protocol = MP_MORNITORMAPSERVER_PING_ACK;
			g_Network.Send2Server(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	case MP_MORNITORMAPSERVER_QUERYUSERCOUNT_SYN:
		{
			MSGUSERCOUNT  * pmsg = (MSGUSERCOUNT  *)pMsg;
			pmsg->Protocol = MP_MORNITORMAPSERVER_QUERYUSERCOUNT_ACK;
			pmsg->dwUserCount = g_pUserTable->GetUserCount();
			pmsg->ServerPort = g_pServerTable->GetSelfServer()->wPortForServer;
			g_Network.Send2Server(dwConnectionIndex, pMsg, sizeof(MSGUSERCOUNT));
		}
		break;
	case MP_MORNITORMAPSERVER_QUERY_CHANNELINFO_SYN:
		{			
			CHANNELSYSTEM->SendChannelInfoToMS( dwConnectionIndex );			
		}
		break;
	case MP_MORNITORMAPSERVER_ASSERTMSGBOX_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			g_bAssertMsgBox = (BOOL)pmsg->dwData;
			if(g_bAssertMsgBox)
				g_Console.LOG(4,"Assert MsgBox is On");
			else
				g_Console.LOG(4,"Assert MsgBox is Off");
		}
		break;
	case MP_MORNITORMAPSERVER_SERVEROFF_SYN:
		{
			/*
			g_bCloseWindow = TRUE;	//콘솔창 닫는다.

			g_pServerSystem->HandlingBeforeServerEND();*/

			//SW060719 수정
			g_pServerSystem->SetStart(FALSE);
			g_pServerSystem->HandlingBeforeServerEND();

			g_Console.LOG(4, "Close Window" );
			g_bCloseWindow = TRUE;
		}
		break;
	}

}
void MP_POWERUPMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	BOOTMNGR->NetworkMsgParse(dwConnectionIndex, pMsg, dwLength);
}

void MP_MAPSERVERMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	ASSERT(0);
/*
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_SERVER_PWRUP:
		{
			MSG_WORD * msg = (MSG_WORD *)pMsg;
			SERVERINFO * info = g_pServerTable->FindServer(msg->wData);//port
			//PACKDATA에서 사용하는 변수
			if(g_pServerTable->GetMaxServerConnectionIndex() < dwConnectionIndex)
				g_pServerTable->SetMaxServerConnectionIndex(dwConnectionIndex);
			ASSERT(info);
			if(info)
			{
				info->dwConnectionIndex = dwConnectionIndex;
				if(info->wServerKind == AGENT_SERVER || info->wServerKind == MONITOR_SERVER || info->wServerKind == MONITOR_AGENT_SERVER)
				{
					REGIST_MAP msg;
					msg.Category = MP_SERVER;
					msg.Protocol = MP_SERVER_REGISTMAP_ACK;
					msg.mapServerPort = g_pServerTable->GetSelfServer()->wPortForServer;
					msg.mapnum = GAMERESRCMNGR->GetLoadMapNum();
					g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));
				}
			}
		}
		break;
	case MP_SERVER_REGISTMAP_SYN:
		{
			REGIST_MAP msg;
			msg.Category = MP_SERVER;
			msg.Protocol = MP_SERVER_REGISTMAP_ACK;
			msg.mapServerPort = g_pServerTable->GetSelfServer()->wPortForServer;
			msg.mapnum = GAMERESRCMNGR->GetLoadMapNum();
			g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	}*/

}
void MP_CHARMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
		/*
	case MP_CHAR_LEVELDOWN_SYN:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer) return;
			
			CHARMGR->SetCharLevelDown(pPlayer, pmsg->wData);
		}
		break;
		*/
	case MP_CHAR_POINTADD_SYN:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer) return;

			MSGBASE msg;			
			// 지금 플레이어가 포인트를 올릴수 있는 상태인가?체크
			if( CHARMGR->ArrangeCharLevelPoint(pPlayer, pmsg->wData) )
				SetProtocol( &msg, MP_CHAR, MP_CHAR_POINTADD_ACK );
			else
				SetProtocol( &msg, MP_CHAR, MP_CHAR_POINTADD_SYN );

			pPlayer->SendMsg( &msg, sizeof(msg) );
		}
		break;
	case MP_CHAR_STATE_NOTIFY:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer) return;

			if( pPlayer->GetState() == pmsg->bData && pPlayer->GetState() != eObjectState_Die )
				OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer, pmsg->bData);
		}
		break;
	case MP_CHAR_ABILITY_UPGRADE_SYN:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer) return;
			
			MSG_WORD msg;
			msg.Category = MP_CHAR;
			if(ABILITYMGR->UpgradeAbility(pmsg->wData,pPlayer->GetAbilityGroup()) == TRUE)
				msg.Protocol = MP_CHAR_ABILITY_UPGRADE_ACK;
			else
				msg.Protocol = MP_CHAR_ABILITY_UPGRADE_NACK;
			msg.wData = pmsg->wData;
			pPlayer->SendMsg(&msg,sizeof(msg));
		}
		break;
	case MP_CHAR_ABILITY_UPGRADE_SKPOINT_SYN:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer) return;
			
			MSG_WORD msg;
			msg.Category = MP_CHAR;
			if(ABILITYMGR->UpgradeAbilitySkPoint(pmsg->wData,pPlayer->GetAbilityGroup()) == TRUE)
				msg.Protocol = MP_CHAR_ABILITY_UPGRADE_SKPOINT_ACK;
			else
				msg.Protocol = MP_CHAR_ABILITY_UPGRADE_SKPOINT_NACK;
			msg.wData = pmsg->wData;
			pPlayer->SendMsg(&msg,sizeof(msg));
		}
		break;
	case MP_CHAR_ABILITY_DOWNGRADE_SKPOINT_SYN:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer) return;
			
			MSG_WORD msg;
			msg.Category = MP_CHAR;
			if(ABILITYMGR->DowngradeAbilitySkPoint(pmsg->wData,pPlayer->GetAbilityGroup()) == TRUE)
				msg.Protocol = MP_CHAR_ABILITY_DOWNGRADE_SKPOINT_ACK;
			else
				msg.Protocol = MP_CHAR_ABILITY_DOWNGRADE_SKPOINT_NACK;
			
			msg.wData = pmsg->wData;
			pPlayer->SendMsg(&msg,sizeof(msg));

			// 06. 03 국내무쌍 - 이영준
#ifndef _JAPAN_LOCAL_
			if(pmsg->wData == ABILITYINDEX_TACTIC_MUSSANG)
				pPlayer->InitMussangPoint();
#endif
		}
		break;
		// magi82(42) - 샵아이템 추가(수련치 초기화)
	case MP_CHAR_ABILITY_RESET_SKPOINT_SYN:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer) return;

			MSG_WORD msg;
			msg.Category = MP_CHAR;
			if(ABILITYMGR->ResetAbilitySkPoint(pmsg->wData1,pPlayer->GetAbilityGroup()) == TRUE)
			{
				// 묵혼 수련치를 초기화를 하면 묵혼 포인트 역시 초기화 해줘야 한다.
#ifndef _JAPAN_LOCAL_
				if(pmsg->wData1 == ABILITYINDEX_TACTIC_MUSSANG)
					pPlayer->InitMussangPoint();
#endif

				msg.Protocol = MP_CHAR_ABILITY_RESET_SKPOINT_ACK;

				// ShopItem 삭제(초기화를 성공 했으므로 주문서를 삭제 해야한다)
				const ITEMBASE* pItem = ITEMMGR->GetItemInfoAbsIn( pPlayer, (POSTYPE)pmsg->wData2 );
				if( EI_TRUE == ITEMMGR->DiscardItem( pPlayer, (POSTYPE)pmsg->wData2, pItem->wIconIdx, 1 ) )
				{
					SEND_SHOPITEM_BASEINFO msg;
					SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_USE_ACK );
					msg.ShopItemPos = (POSTYPE)pmsg->wData2;
					msg.ShopItemIdx = pItem->wIconIdx;
					ITEMMGR->SendAckMsg( pPlayer, &msg, sizeof(msg) );

					// Log추가
					LogItemMoney( pPlayer->GetID(), pPlayer->GetObjectName(), 0, "",
						eLog_ShopItemUse, pPlayer->GetMoney(eItemTable_Inventory), 0, 0,
						pItem->wIconIdx, pItem->dwDBIdx, pItem->Position, 0, pItem->Durability, pPlayer->GetPlayerExpPoint());
				}
			}
			else
			{
				msg.Protocol = MP_CHAR_ABILITY_RESET_SKPOINT_NACK;
			}

			msg.wData = pmsg->wData1;
			pPlayer->SendMsg(&msg,sizeof(msg));
		}
		break;
	case MP_CHAR_EXITSTART_SYN:
		{

			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( !pPlayer ) return;
			if( pPlayer->IsExitStart() ) return;

			int errorCode;
			if( ( errorCode = pPlayer->CanExitStart() ) == eEXITCODE_OK )
			{
				pPlayer->SetExitStart( TRUE );
				MSGBASE msgAck;
				msgAck.Category		= MP_CHAR;
				msgAck.Protocol		= MP_CHAR_EXITSTART_ACK;
				pPlayer->SendMsg(&msgAck, sizeof(msgAck));
			}
			else
			{
				MSG_BYTE msgNack;
				msgNack.Category	= MP_CHAR;
				msgNack.Protocol	= MP_CHAR_EXITSTART_NACK;
				msgNack.bData		= errorCode;
				pPlayer->SendMsg(&msgNack, sizeof(msgNack));
			}	
		}
		break;
	case MP_CHAR_EXIT_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( !pPlayer ) return;
			if( !pPlayer->IsExitStart() ) return;
			
			int nErrorCode;
			if( ( nErrorCode = pPlayer->CanExit() ) == eEXITCODE_OK )
			{
				OBJECTSTATEMGR_OBJ->StartObjectState( pPlayer, eObjectState_Exit, 0 );
				pPlayer->SetNormalExit();
				pPlayer->SetExitStart( FALSE );

				MSGBASE msgAck;
				msgAck.Category = MP_CHAR;
				msgAck.Protocol = MP_CHAR_EXIT_ACK;				
				pPlayer->SendMsg(&msgAck, sizeof(msgAck));
			}
			else
			{
				pPlayer->SetExitStart( FALSE );
				MSG_BYTE msgNack;
				msgNack.Category	= MP_CHAR;
				msgNack.Protocol	= MP_CHAR_EXIT_NACK;
				msgNack.bData		= nErrorCode;
				pPlayer->SendMsg(&msgNack, sizeof(msgNack));				
			}			
		}
		break;
	case MP_CHAR_BADFAME_SYN:		
		{
			MSG_FAME* pmsg = (MSG_FAME*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)		return;

			if( FAMEMGR->BailBadFame( pPlayer, pmsg->Fame ) == TRUE)
			{				
				pPlayer->SetBadFame( pPlayer->GetBadFame() - pmsg->Fame );
				pPlayer->SetMoney( pmsg->Fame * BAIL_PRICE, MONEY_SUBTRACTION, 0, eItemTable_Inventory, eMoneyLog_LoseBadFrameBail, pPlayer->GetID() );
				CharacterBadFameUpdate( pPlayer->GetID(), pPlayer->GetBadFame(), pPlayer->GetMoney() );
				
				//전체에게 보내기
				FAMEMGR->SendBadFameMsg( pPlayer, pPlayer->GetBadFame() );

				//나에게 확인 보내기(메세지 보여주기)
				MSG_FAME msg;
				msg.Category	= MP_CHAR;
				msg.Protocol	= MP_CHAR_BADFAME_ACK;
				msg.dwObjectID	= pPlayer->GetID();
				msg.Fame		= pmsg->Fame;
				pPlayer->SendMsg(&msg, sizeof(msg));
			}
			else
			{
				MSGBASE msg;
				msg.Category	= MP_CHAR;
				msg.Protocol	= MP_CHAR_BADFAME_NACK;
				msg.dwObjectID	= pPlayer->GetID();
				pPlayer->SendMsg(&msg, sizeof(msg));
			}			
		}
		break;
	case MP_CHAR_PLAYTIME_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)		return;

			int edst = abs((int)( pmsg->dwData - pPlayer->GetPlayTime() ));

			MSG_DWORD msg;
			msg.Category = MP_CHAR;
			msg.dwData = pPlayer->GetPlayTime();

			if(edst < MAX_ALLOWANCE)
			{
				msg.Protocol = MP_CHAR_PLAYTIME_ACK;
			}
			else
			{
				msg.Protocol = MP_CHAR_PLAYTIME_NACK;

				// Assert
				char buf[256] = { 0, };
				sprintf(buf, "플레이시간이 오차범위를 벗어났습니다. player:%d Server:%d ecost:%d allowance:%d",
					pmsg->dwData, pPlayer->GetPlayTime(), edst, MAX_ALLOWANCE);
				ASSERTMSG(0, buf);
			}

			pPlayer->SendMsg(&msg, sizeof(msg));
		}
		break;
	case MP_CHAR_POINTMINUS_SYN:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer) return;

			// 지금 플레이어가 포인트를 올릴수 있는 상태인가?체크
			MSG_WORD msg;
			msg.Category = MP_CHAR;			

			if( CHARMGR->ArrangeCharMinusPoint(pPlayer, pmsg->wData) )
			{
				msg.Protocol = MP_CHAR_POINTMINUS_ACK;
			}
			else
			{
				msg.Protocol = MP_CHAR_POINTMINUS_NACK;
			}

			msg.wData = pPlayer->GetShopItemStats()->StatePoint;
			pPlayer->SendMsg(&msg, sizeof(msg));
		}
		break;
	case MP_CHAR_MUSSANG_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer) return;

			//SW070912 기획변경
			if( pPlayer->InTitan() )
				return;

			//SW061121 무쌍발동시간 서버로 부터 초기화로 변경
			/*
			int rt = pPlayer->StartMussangMode();

			MSGBASE msg;
			msg.Category = MP_CHAR;
			msg.dwObjectID = pPlayer->GetID();
			switch( rt )
			{
			case 1:
				{
					msg.Protocol = MP_CHAR_MUSSANG_ACK;
					PACKEDDATA_OBJ->QuickSend(pPlayer,&msg,dwLength);
				}
				break;
			case -99:
				{
					msg.Protocol = MP_CHAR_MUSSANG_NACK;
					pPlayer->SendMsg(&msg, sizeof(msg));
				}
				break;
			}*/
			DWORD mussangtime = pPlayer->StartMussangMode();

			MSG_DWORD msg;
			msg.Category = MP_CHAR;
			msg.dwObjectID = pPlayer->GetID();
			msg.dwData = mussangtime;

			if(mussangtime)
			{
				msg.Protocol = MP_CHAR_MUSSANG_ACK;
				PACKEDDATA_OBJ->QuickSend(pPlayer,&msg,sizeof(msg));
			}
			else
			{
				msg.Protocol = MP_CHAR_MUSSANG_NACK;
				pPlayer->SendMsg(&msg, sizeof(msg));
			}
		}
		break;
	}

}
void MP_CHATMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_CHAT_ALL:
		{
			TESTMSG* pmsg = (TESTMSG*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer) return;

			if( pPlayer->GetUserLevel() == eUSERLEVEL_GM )
				pmsg->Protocol = MP_CHAT_GM;
			
			PACKEDDATA_OBJ->QuickSend(pPlayer,pmsg,dwLength);
		}
		break;
	case MP_CHAT_SMALLSHOUT:			//같은 채널의 모두에게 보내기!
		{
			MSG_CHAT_WITH_SENDERID* pmsg = (MSG_CHAT_WITH_SENDERID*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer) return;

			MSG_CHAT_WITH_SENDERID msg;
			msg.Category = pmsg->Category;
			msg.Protocol = pmsg->Protocol;
			msg.dwObjectID = pmsg->dwObjectID;
			msg.dwSenderID = pPlayer->GetID();
			SafeStrCpy( msg.Msg, pmsg->Msg, MAX_CHAT_LENGTH+1 );
			SafeStrCpy( msg.Name, pmsg->Name, MAX_NAME_LENGTH+1 );
 
			if( pPlayer->GetUserLevel() == eUSERLEVEL_GM )
 				msg.Protocol = MP_CHAT_GM_SMALLSHOUT;
			else
			{
				if( pPlayer->GetBattle()->GetBattleKind() != eBATTLE_KIND_NONE && pPlayer->GetBattle()->GetBattleKind() != eBATTLE_KIND_VIMUSTREET )
					return;
			}
			
			g_pUserTable->SetPositionUserHead();
			while( CObject* pObject = g_pUserTable->GetUserData() )
			{
				if( pObject->GetObjectKind() != eObjectKind_Player ) continue;
				
				CPlayer* pReceiver = (CPlayer*)pObject;
				if( pPlayer->GetChannelID() == pReceiver->GetChannelID() /*&&
					pPlayer->GetBattleID() < 5 && pReceiver->GetBattleID() < 5*/ )
				{
 					pReceiver->SendMsg( &msg, msg.GetMsgLength() );
				}
			}
		}
		break;
	case MP_CHAT_GUILD:
		{
			TESTMSGID * pmsg = (TESTMSGID*)pMsg;
			GUILDMGR->SendChatMsg(pmsg->dwSenderID, pmsg->dwObjectID, pmsg->Msg);
		}
		break;
	case MP_CHAT_GUILDUNION:
		{
			MSG_CHAT_WITH_SENDERID * pmsg = (MSG_CHAT_WITH_SENDERID*)pMsg;
			GUILDUNIONMGR->SendChatMsg( pmsg );
		}
		break;

	default:
		//GAMESYSTEM_OBJ->m_ConnectionTable.SendToCharacter(pTempMsg,dwLength);
		break;
	}
}

void MP_USERCONNMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_USERCONN_CHANNELINFO_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pTempMsg;
			CHANNELSYSTEM->SendChannelInfo(pmsg, dwConnectionIndex);
		}
		break;

	case MP_USERCONN_GAMEIN_OTHERMAP_SYN:	//from agent. 다른맵으로 들어갔는데..혹시라도 이맵에도 있으면 지워주려고
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			g_pServerSystem->RemovePlayer( pmsg->dwObjectID );
		}
		break;

	case MP_USERCONN_GAMEIN_SYN:		//에이젼트가 보낸것
		{			
			MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;
			CObject* pObject = g_pUserTable->FindUser(pmsg->dwObjectID);

			if( pObject != NULL )	//이미 있다.
			{
				if( pObject->GetObjectKind() == eObjectKind_Player )	//유저인가?
				{
//					g_pServerSystem->RemovePlayer(pmsg->dwObjectID);

					MSG_DWORD msg;
					msg.Category	= MP_USERCONN;
					msg.Protocol	= MP_USERCONN_GAMEIN_NACK;
					msg.dwData		= pmsg->dwObjectID;

					
					g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );					
					return;
				}
			}

			CPlayer* pPlayer = g_pServerSystem->AddPlayer(pmsg->dwObjectID,dwConnectionIndex,pmsg->dwData1, pmsg->dwData2);
			if(!pPlayer) return;

			pPlayer->SetUserLevel( pmsg->dwData3 );
			pPlayer->SetReturnMapNum( (WORD)pmsg->dwData4 );

			CharacterNumSendAndCharacterInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			CharacterMugongInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			CharacterItemOptionInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			//SW050920 Rare
			CharacterItemRareOptionInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			CharacterAbilityInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);

			CharacterSkinInfo(pPlayer->GetID());

//			CharacterGMControlInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);

			QuestTotalInfo(pmsg->dwObjectID);		// 초보자 퀘스트

			// quest 정보 qurey 추가
			QUESTMGR->CreateQuestForPlayer( pPlayer );
			// 항상 서브퀘스트 정보부터 읽어야 함...
//			QuestMainQuestLoad(pmsg->dwObjectID);
			QuestSubQuestLoad(pmsg->dwObjectID);
//			QuestItemload(pmsg->dwObjectID);

			CYHHashTable<HIDE_NPC_INFO>* pHideNpcTable = GAMERESRCMNGR->GetHideNpcTable();
			HIDE_NPC_INFO* pInfo = NULL;

			// 06. 05 HIDE NPC - 이영준
			// 현재 맵의 숨김 가능 NPC들의 정보를 보내준다
			pHideNpcTable->SetPositionHead();
			while( pInfo = pHideNpcTable->GetData() )
			{
				MSG_WORD2 msg;
				msg.Category = MP_CHEAT;
				msg.Protocol = MP_CHEAT_NPCHIDE_ACK;
				msg.wData1 = pInfo->UniqueIdx;
				msg.wData2 = pInfo->ChannelInfo[pmsg->dwData2];

				pPlayer->SendMsg( &msg, sizeof(msg) );
			}

			// 현재맵의 채널정보 다시보낸다.
			MSG_CHANNEL_INFO msg1;
			memset(&msg1, 0, sizeof(msg1));
			msg1.Category = MP_USERCONN;
			msg1.Protocol = MP_USERCONN_CURRENTMAP_CHANNELINFO;
			CHANNELSYSTEM->GetChannelInfo( &msg1 );

			pPlayer->SendMsg( &msg1, sizeof(msg1) );
		}
		break;
	case MP_USERCONN_GAMEINPOS_SYN:		// 클라이언트에서 보낸것
		{
			MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;

			CObject* pObject = g_pUserTable->FindUser(pmsg->dwObjectID);

			if( pObject != NULL )	//이미 있다.
			{
				if( pObject->GetObjectKind() == eObjectKind_Player )	//유저인가?
				{
//					g_pServerSystem->RemovePlayer(pmsg->dwObjectID);

					MSG_DWORD msg;
					msg.Category	= MP_USERCONN;
					msg.Protocol	= MP_USERCONN_GAMEIN_NACK;
					msg.dwData		= pmsg->dwObjectID;
					g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );					
					return;
				}
			}

			CPlayer* pPlayer = g_pServerSystem->AddPlayer(pmsg->dwObjectID,dwConnectionIndex,pmsg->dwData1, pmsg->dwData2);
			if(!pPlayer)
				return;
			
			pPlayer->SetUserLevel( pmsg->dwData3 );
			pPlayer->SetInitPoint( pmsg->dwData4 );

			CharacterNumSendAndCharacterInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			CharacterMugongInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			CharacterItemOptionInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			//SW050920 Rare
			CharacterItemRareOptionInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			CharacterAbilityInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);

			QuestTotalInfo(pmsg->dwObjectID);		// 초보자 퀘스트

			// quest 정보 qurey 추가
			QUESTMGR->CreateQuestForPlayer( pPlayer );
			// 항상 서브퀘스트 정보부터 읽어야 함...
//			QuestMainQuestLoad(pmsg->dwObjectID);
			QuestSubQuestLoad(pmsg->dwObjectID);
//			QuestItemload(pmsg->dwObjectID);

			CYHHashTable<HIDE_NPC_INFO>* pHideNpcTable = GAMERESRCMNGR->GetHideNpcTable();
			HIDE_NPC_INFO* pInfo = NULL;

			// 06. 05 HIDE NPC - 이영준
			// 현재 맵의 숨김 가능 NPC들의 정보를 보내준다
			pHideNpcTable->SetPositionHead();
			while( pInfo = pHideNpcTable->GetData() )
			{
				MSG_WORD2 msg;
				msg.Category = MP_CHEAT;
				msg.Protocol = MP_CHEAT_NPCHIDE_ACK;
				msg.wData1 = pInfo->UniqueIdx;
				msg.wData2 = pInfo->ChannelInfo[pmsg->dwData2];

				pPlayer->SendMsg( &msg, sizeof(msg) );
			}
		}
		break;

	case MP_USERCONN_GAMEIN_NACK:
		{	// GAMEIN_ACK를 보냈을때 에이젼트에서 유니크 아이디가 틀리면
			// 이 메세지를 날려준다. 그 케릭터를 그냥 지워버리면 된다.
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pTempMsg->dwObjectID);
			if(pPlayer == NULL)
				return;

			g_pServerSystem->RemovePlayer(pTempMsg->dwObjectID);
		}
		break;

	case MP_USERCONN_GAMEOUT_SYN:	//에이젼트에 캐릭터가 있는데 또 캐릭터 선택이 왔다. 혹시 맵에도 남아 있는지 보는거다.
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( pPlayer == NULL ) return;

			g_pServerSystem->RemovePlayer( pmsg->dwObjectID );
			g_Console.LOG( 4, "Charackter Overlap!" );
		}
		break;

	case MP_USERCONN_NOWAITEXITPLAYER:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if(pPlayer == NULL) return;

			pPlayer->SetWaitExitPlayer( FALSE );	//no wait for deleteing player.
		}
		break;

	case MP_USERCONN_DISCONNECTED:
		{
			//KES	//agent가 유저 정보 DB업데이트를 위해서 보내준다.
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if(pPlayer == NULL)
			{
				//KES 로그아웃 프로시져 호출하기 위해 에이젼트로 보내준다.
				MSGBASE msgLogout;
				msgLogout.Category		= MP_USERCONN;
				msgLogout.Protocol		= MP_USERCONN_LOGINCHECK_DELETE;
				msgLogout.dwObjectID	= pmsg->dwData;

				g_Network.Send2Server( dwConnectionIndex, (char*)&msgLogout, sizeof(msgLogout) );
				return;
			}

			if( !pPlayer->IsNormalExit() )
			{
				//강제 종료에 따른 제제
				if( pPlayer->IsPKMode() )
					PKMGR->PKModeExitPanelty( pPlayer, TRUE );
				LOOTINGMGR->DiePlayerExitPanelty( pPlayer );

				//AUTONOTE
				if( pPlayer->GetAutoAskPlayerIdx() )				//---오토신고중인 캐릭터
				{
					AUTONOTEMGR->AutoPlayerLogOut( pPlayer );
				}

				//강제 종료한 유저 잡아두기
#ifdef _CHINA_LOCAL_
				if( pPlayer->IsWaitExitPlayer() )
				{
					g_pServerSystem->AddWaitExitPlayer( dwConnectionIndex, pPlayer->GetID() );
				}
				else
#elif defined _HK_LOCAL_
				if( pPlayer->IsWaitExitPlayer() )
				{
					g_pServerSystem->AddWaitExitPlayer( dwConnectionIndex, pPlayer->GetID() );
				}
				else
#elif defined _TL_LOCAL_
				if( pPlayer->IsWaitExitPlayer() )
				{
					g_pServerSystem->AddWaitExitPlayer( dwConnectionIndex, pPlayer->GetID() );
				}
				else
#endif
				{
					// RaMa - 04.12.14
					pPlayer->UpdateLogoutToDB();

					PARTYMGR->UserLogOut(pPlayer);
					FRIENDMGR->UserLogOut(pmsg->dwObjectID);
					GUILDMGR->UserLogOut(pPlayer);

					g_pServerSystem->RemovePlayer(pmsg->dwObjectID);
					//
					ConfirmUserOut( dwConnectionIndex, pPlayer->GetUserID(), pPlayer->GetID(), 2 );
				}
			}
			else
			{
				// RaMa - 04.12.14
				pPlayer->UpdateLogoutToDB();

				PARTYMGR->UserLogOut(pPlayer);
				FRIENDMGR->UserLogOut(pmsg->dwObjectID);
				GUILDMGR->UserLogOut(pPlayer);

				g_pServerSystem->RemovePlayer(pmsg->dwObjectID);
				//
				ConfirmUserOut( dwConnectionIndex, pPlayer->GetUserID(), pPlayer->GetID(), 2 );
			}
		}
		break;

	case MP_USERCONN_CHANGEMAP_SYN:
		{
			MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(NULL == pPlayer)
				return;
			
			if( !QUESTMAPMGR->IsQuestMap() )
//			if( pPlayer->IsPKMode() || LOOTINGMGR->IsLootedPlayer( pPlayer->GetID() ) || eObjectState_Die == pPlayer->GetState() )
			if( pPlayer->IsPKMode() ||
				LOOTINGMGR->IsLootedPlayer( pPlayer->GetID() ) ||
				eObjectState_Die == pPlayer->GetState() ||
				pPlayer->GetAutoAskPlayerIdx() )					// autonote 신고중이면...
			{
				MSGBASE msgNack;
				msgNack.Category	= MP_USERCONN;
				msgNack.Protocol	= MP_USERCONN_CHANGEMAP_NACK;
				msgNack.dwObjectID	= pPlayer->GetID();
				pPlayer->SendMsg( &msgNack, sizeof(msgNack) );
				if(pPlayer->GetInitPoint())
				{
					pPlayer->SetInitPoint(0);
				}

				//debug imsi
				FILE* fp = fopen( "mapchangefail.txt", "a+" );
				if(fp)
				{
					fprintf(fp, "CHRID %u, FILE %s, %d, PARAM %d\n", pPlayer->GetID(), __FILE__, __LINE__, pmsg->dwData1 );
					fclose(fp);
				}

				return;
			}

			MAPCHANGE_INFO* ChangeInfo = NULL;
			// Item사용추가
			if( eMapChange_SOS == pmsg->dwData4 )	// magi82 - SOS(070723)
			{
				// magi82(35) MapChange State
				// 맵이동 할때 어떤 상태의 맵이동인지 체크하게 수정
				ChangeInfo = GAMERESRCMNGR->GetMapChangeInfoForDestMapNum( (WORD)(pmsg->dwData1) );
				if( !ChangeInfo )
				{// 차후수정
					/*
					MSG_ITEM_ERROR msg;
					msg.Category = MP_ITEM;
					msg.Protocol = MP_ITEM_SHOPITEM_USE_NACK;
					pPlayer->SendMsg( &msg, sizeof(msg) );
					*/
					return;
				}				
			}
			else if( eMapChange_Item == pmsg->dwData4 )
			{
				SHOPITEMUSEBASE BaseInfo;
				BaseInfo.ShopItemIdx = (WORD)pmsg->dwData2;
				BaseInfo.ShopItemPos = (POSTYPE)pmsg->dwData3;
				SHOPITEMBASE pShopItem;

				ChangeInfo = GAMERESRCMNGR->GetMapChangeInfoForDestMapNum( (WORD)(pmsg->dwData1) );
				if( !ChangeInfo )
				{
					MSG_ITEM_ERROR msg;
					msg.Category = MP_ITEM;
					msg.Protocol = MP_ITEM_SHOPITEM_USE_NACK;
					pPlayer->SendMsg( &msg, sizeof(msg) );
					return;
				}

				if( ITEMMGR->IsUseAbleShopItem( pPlayer, BaseInfo.ShopItemIdx, BaseInfo.ShopItemPos ) )
				//BYTE rt = ITEMMGR->UseShopItem( pPlayer, BaseInfo, &pShopItem );
				//if(rt == eItemUseSuccess)
				{
					ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( BaseInfo.ShopItemIdx );
					if( pItemInfo && 10 == pItemInfo->ItemType )
						ITEMMGR->DiscardItem( pPlayer, BaseInfo.ShopItemPos, BaseInfo.ShopItemIdx, 1 );

					SEND_SHOPITEM_BASEINFO msg;
					msg.Category = MP_ITEM;
					msg.Protocol = MP_ITEM_SHOPITEM_USE_ACK;
					msg.ShopItemBase = pShopItem;
					msg.ShopItemIdx = BaseInfo.ShopItemIdx;
					msg.ShopItemPos = BaseInfo.ShopItemPos;					
					ITEMMGR->SendAckMsg(pPlayer, &msg, sizeof(msg));					
				}
				else
				{
					MSG_ITEM_ERROR msg;
					msg.Category = MP_ITEM;
					msg.Protocol = MP_ITEM_SHOPITEM_USE_NACK;
					msg.ECode = 0;
					ITEMMGR->SendErrorMsg(pPlayer, &msg, sizeof(msg), 0);
					return;
				}				
				
			}
			else
			{				
				ChangeInfo = GAMERESRCMNGR->GetMapChangeInfo(pmsg->dwData1);
				if( !ChangeInfo ) return;

				// checkhacknpc
				if( FALSE == CheckHackNpc( pPlayer, (WORD)pmsg->dwData1 ) )
				{
					MSGBASE msgNack;
					msgNack.Category	= MP_USERCONN;
					msgNack.Protocol	= MP_USERCONN_CHANGEMAP_NACK;
					msgNack.dwObjectID	= pPlayer->GetID();
					pPlayer->SendMsg( &msgNack, sizeof(msgNack) );

				//debug imsi
					FILE* fp = fopen( "mapchangefail.txt", "a+" );
					if(fp)
					{
						fprintf(fp, "CHRID %u, FILE %s, %d, PARAM %d\n", pPlayer->GetID(), __FILE__, __LINE__, pmsg->dwData1 );
						fclose(fp);
					}

					return;
				}
			//
			}			
			//Pet 맵이동시 펫 소환상태 유지
			pPlayer->GetPetManager()->SetPetSummonning(ePSS_SaveSummon);

			pPlayer->GetTitanManager()->SetTitanRiding(TRUE);
			// RaMa - 04.12.14
			pPlayer->UpdateLogoutToDB();

			LEVELTYPE lvl = pPlayer->GetLevel();
			EXPTYPE exp = pPlayer->GetPlayerExpPoint();
			g_pServerSystem->RemovePlayer(pPlayer->GetID());
			
			// 리턴 받으면 맵 아웃 메시지 보냄.
			SaveMapChangePointReturn(pmsg->dwObjectID, ChangeInfo->Kind, dwConnectionIndex, lvl, exp, pPlayer->GetMoney(eItemTable_Pyoguk));
		}
		break;


	case MP_USERCONN_CHANGEMAP_NACK:
		{
			g_Console.Log(eLogDisplay, 4, "Recv OK!!!");
		}
		break;

	case MP_USERCONN_SAVEPOINT_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( pPlayer == NULL )	return;

			LOGINPOINT_INFO* LoginInfo = GAMERESRCMNGR->GetLoginPointInfo(pmsg->dwData);
			if( LoginInfo == NULL )	return;

			// checkhacknpc
			if( CheckHackNpc( pPlayer, (WORD)pmsg->dwData ) == FALSE )
				return;
			//
			
			WORD ServerMapNum = GAMERESRCMNGR->GetLoadMapNum();
			SavePointUpdate(pmsg->dwObjectID, LoginInfo->Kind, ServerMapNum);
		
			// 맵전환포인트를 항상 0으로 셋팅해준다.
/*			MAPCHANGE_INFO* ChangeInfo = GAMERESRCMNGR->GetMapChangeInfoForDestMapNum( (WORD)pmsg->dwData1 );
			if( !ChangeInfo ) return;
			SaveMapChangePointUpdate( pPlayer->GetID(), ChangeInfo->Kind );*/

			SaveMapChangePointUpdate(pmsg->dwObjectID, 0);
			
			pPlayer->InitMapChangePoint();
			pPlayer->SetLoginPoint(LoginInfo->Kind);			
		}
		break;
	case MP_USERCONN_BACKTOCHARSEL_SYN:
		{
#ifndef _CHINA_LOCAL_ //중국은 캐릭터 선택창으로 가는 메뉴를 없에고 처리도 막는다				
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
				return;
			// RaMa - 04.12.14
			pPlayer->UpdateLogoutToDB();
			PARTYMGR->UserLogOut(pPlayer);
			FRIENDMGR->UserLogOut(pTempMsg->dwObjectID);
			GUILDMGR->UserLogOut(pPlayer);

			SaveCharInfoBeforeLogout(pPlayer->GetID(), dwConnectionIndex, pPlayer->GetUserID(), pPlayer->GetMoney(eItemTable_Pyoguk));
#endif  //_CHINA_LOCAL_ //중국은 캐릭터 선택창으로 가는 메뉴를 없에고 처리도 막는다
		}
		break;

	case MP_USERCONN_SETVISIBLE:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
				return;
			pPlayer->SetVisible(FALSE);
			PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer,pmsg,sizeof(MSG_DWORD2));
		}
		break;
		
	case MP_USERCONN_BACKTOBEFOREMAP_SYN:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( pPlayer == NULL )	return;
			
			MAPCHANGE_INFO* ChangeInfo = GAMERESRCMNGR->GetMapChangeInfoFromToMapNum( pmsg->wData1, pmsg->wData2 );
			if( !ChangeInfo )
			{
				ChangeInfo = GAMERESRCMNGR->GetMapChangeInfoForDestMapNum( pmsg->wData2 );
				if( !ChangeInfo )	return;
			}
			SaveMapChangePointUpdate( pPlayer->GetID(), ChangeInfo->Kind );

			// RaMa - 04.12.14
			pPlayer->UpdateLogoutToDB();					
			g_pServerSystem->RemovePlayer( pPlayer->GetID() );

			MSG_WORD msg;
			msg.Category	= MP_USERCONN;
			msg.Protocol	= MP_USERCONN_BACKTOBEFOREMAP_ACK;
			msg.dwObjectID	= pPlayer->GetID();
			msg.wData		= pmsg->wData2;

			g_Network.Broadcast2AgentServer( (char*)&msg, sizeof(msg) );			
		}
		break;

// eventmap--------------------------------------------------------------------------------------
	case MP_USERCONN_ENTER_EVENTMAP_SYN:
		{
			MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;
			CPlayer* pPlayer = g_pServerSystem->AddPlayer( pmsg->dwObjectID, dwConnectionIndex, pmsg->dwData1, pmsg->dwData3-1 );
			if( !pPlayer )	return;

			pPlayer->SetUserLevel( pmsg->dwData2 );
			CharacterNumSendAndCharacterInfo2(pmsg->dwObjectID, pmsg->dwData4);
			CharacterMugongInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			CharacterItemOptionInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			//SW050920 Rare
			CharacterItemRareOptionInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			CharacterAbilityInfo(pmsg->dwObjectID, MP_USERCONN_GAMEIN_SYN);
			//SW051129 Pet
			//CharacterPetInfo(pmsg->dwObjectID, pPlayer->GetUserID());

			EVENTMAPMGR->AddMember( pPlayer, pmsg->dwData4, pmsg->dwData3 );
		}
		break;
//-----------------------------------------------------------------------------------------------
#ifdef _KOR_LOCAL_
	//2008. 5. 9. CBH - GAMEIN 상태의 맵 채널이동 관련 채널 정보 요청 프로토콜
	case MP_USERCONN_CHANGEMAP_CHANNELINFO_SYN:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pTempMsg;
			CHANNELSYSTEM->SendMapChangeChannelInfo(pmsg, dwConnectionIndex);
		}
		break;
#endif
	default:
		break;
	}
}
void MP_MOVEMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_MOVE_TARGET:
		{
			/*
			MOVE_TARGETPOS* pmsg = (MOVE_TARGETPOS*)pMsg;
			VECTOR3 Start;
			pmsg->spos.Decompress(&Start);
			VECTOR3 Target;
			pmsg->tpos.Decompress(&Target);
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwMoverID);
			if(pPlayer == NULL)
				return;

			if(pPlayer->GetState() == eObjectState_Die)
				return;

			//////////////////////////////////////////////////////////////////////////
			// 클라이언트들의 충돌 체크(클라이언트들에서 하기 때문에 필요없지만... 확인용)
			// 클라이언트의 해킹등을 방지하기 위해..
			VECTOR3 colpos;
			MAPTYPE MapNum = GAMERESRCMNGR->GetLoadMapNum();
			if(g_pServerSystem->GetMap()->CollisionCheck(&Start,&Target,&colpos,MapNum))
			{
				CCharMove::CorrectPlayerPosToServer(pPlayer);
				return;
			}
			//////////////////////////////////////////////////////////////////////////			

			CCharMove::StartMove(pPlayer,gCurTime,&Start,&Target);
			*/
			MOVE_TARGETPOS* pmsg = (MOVE_TARGETPOS*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwMoverID);
			if(pPlayer == NULL)
				return;
			if(pPlayer->GetState() == eObjectState_Die)
			{
				MSG_DWORD msg;
				msg.Category = MP_CHAR;
				msg.Protocol = MP_CHAR_YOUAREDIED;
				pPlayer->SendMsg(&msg,sizeof(msg));
				return;
			}
			if(pPlayer->GetState() == eObjectState_Ungijosik)
			{				
				CCharMove::CorrectPlayerPosToServer(pPlayer);
				return;
			}
//KES			
			if(pPlayer->GetState() == eObjectState_Immortal )
			{
				OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal, 0 );
			}
			if(pmsg->tpos.PosNum >= MAX_CHARTARGETPOSBUF_SIZE)
			{
				CCharMove::CorrectPlayerPosToServer(pPlayer);
				return;
			}
//
			VECTOR3 Start;
			pmsg->GetStartPos(&Start);
			
			VECTOR3* pCurPos = CCharMove::GetPosition(pPlayer);
			if(CalcDistanceXZ(pCurPos,&Start) >= 1000)
			{				
				CCharMove::CorrectPlayerPosToServer(pPlayer);
				return;
			}

#ifdef _CHINA_LOCAL_
			// check collision
			BASEMOVE_INFO* pMoveInfo = pPlayer->GetMoveInfo();
			pmsg->GetTargetInfo( (MOVE_INFO*)pMoveInfo );			
			BYTE idx = pMoveInfo->GetCurTargetPosIdx();
			VECTOR3* pvTarget = pMoveInfo->GetTargetPosition( idx );
			
			VECTOR3 colpos;
			MAPTYPE MapNum = GAMERESRCMNGR->GetLoadMapNum();
			if(g_pServerSystem->GetMap()->CollisionCheck( &Start, pvTarget, &colpos, pPlayer ) )
			{
				CCharMove::CorrectPlayerPosToServer(pPlayer);
				return;
			}			

//			ASSERT(pPlayer->GetMoveInfo()->GetCurTargetPosIdx() < pPlayer->GetMoveInfo()->GetMaxTargetPosIdx());
			CCharMove::StartMoveEx( pPlayer, gCurTime, &Start, pvTarget );
/*
#elif defined _HK_LOCAL_
			// check collision
			BASEMOVE_INFO* pMoveInfo = pPlayer->GetMoveInfo();
			pmsg->GetTargetInfo( (MOVE_INFO*)pMoveInfo );			
			BYTE idx = pMoveInfo->GetCurTargetPosIdx();
			VECTOR3* pvTarget = pMoveInfo->GetTargetPosition( idx );
			
			VECTOR3 colpos;
			MAPTYPE MapNum = GAMERESRCMNGR->GetLoadMapNum();
			if(g_pServerSystem->GetMap()->CollisionCheck( &Start, pvTarget, &colpos, pPlayer ) )
			{
				CCharMove::CorrectPlayerPosToServer(pPlayer);
				return;
			}			

//			ASSERT(pPlayer->GetMoveInfo()->GetCurTargetPosIdx() < pPlayer->GetMoveInfo()->GetMaxTargetPosIdx());
			CCharMove::StartMoveEx( pPlayer, gCurTime, &Start, pvTarget );
*/
#else
			pmsg->GetTargetInfo((MOVE_INFO *)pPlayer->GetMoveInfo());
//			ASSERT(pPlayer->GetMoveInfo()->GetCurTargetPosIdx() < pPlayer->GetMoveInfo()->GetMaxTargetPosIdx());
			CCharMove::StartMoveEx(pPlayer,gCurTime,&Start,pPlayer->GetMoveInfo()->GetTargetPosition(pPlayer->GetMoveInfo()->GetCurTargetPosIdx()));
#endif

			// RaMa
			CParty* pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx());
			if(pParty)
			{
				pParty->SendMemberPos(pPlayer->GetID(), pPlayer->GetMoveInfo());
			}

			PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer,pmsg,dwLength);


#ifdef _KOR_LOCAL_
			pPlayer->SetActionTime();
#endif
		}
		break;
// jsd
	case MP_MOVE_ONETARGET:
		{
			MOVE_ONETARGETPOS* pmsg = (MOVE_ONETARGETPOS*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwMoverID);
			if(pPlayer == NULL)
				return;
			if(pPlayer->GetInited() == FALSE || pPlayer->GetInitedGrid() == FALSE)
				return;
			if(pPlayer->GetState() == eObjectState_Die)
			{
				MSG_DWORD msg;
				msg.Category = MP_CHAR;
				msg.Protocol = MP_CHAR_YOUAREDIED;
				pPlayer->SendMsg(&msg,sizeof(msg));
				return;
			}
			if(pPlayer->GetState() == eObjectState_Immortal )
			{
				OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal, 0 );
			}
		
			VECTOR3 Start;
			pmsg->GetStartPos(&Start);
			
			VECTOR3* pCurPos = CCharMove::GetPosition(pPlayer);
			if(CalcDistanceXZ(pCurPos,&Start) >= 1000)
			{				
				CCharMove::CorrectPlayerPosToServer(pPlayer);
				return;
			}

/*
#ifdef _CHINA_LOCAL_
			// check collision
			BASEMOVE_INFO* pMoveInfo = pPlayer->GetMoveInfo();
			pmsg->GetTargetInfo( (MOVE_INFO*)pMoveInfo );
			BYTE idx = pMoveInfo->GetCurTargetPosIdx();
			VECTOR3* pvTarget = pMoveInfo->GetTargetPosition( idx );
			
			VECTOR3 colpos;
			MAPTYPE MapNum = GAMERESRCMNGR->GetLoadMapNum();
			if(g_pServerSystem->GetMap()->CollisionCheck( &Start, pvTarget, &colpos, pPlayer ) )
			{
				CCharMove::CorrectPlayerPosToServer(pPlayer);
				return;
			}			

//			ASSERT(pPlayer->GetMoveInfo()->GetCurTargetPosIdx() < pPlayer->GetMoveInfo()->GetMaxTargetPosIdx());
			CCharMove::StartMoveEx( pPlayer, gCurTime, &Start, pvTarget );
/*
//#elif defined _HK_LOCAL_
			// check collision
			BASEMOVE_INFO* pMoveInfo = pPlayer->GetMoveInfo();
			pmsg->GetTargetInfo( (MOVE_INFO*)pMoveInfo );
			BYTE idx = pMoveInfo->GetCurTargetPosIdx();
			VECTOR3* pvTarget = pMoveInfo->GetTargetPosition( idx );
			
			VECTOR3 colpos;
			MAPTYPE MapNum = GAMERESRCMNGR->GetLoadMapNum();
			if(g_pServerSystem->GetMap()->CollisionCheck( &Start, pvTarget, &colpos, pPlayer ) )
			{
				CCharMove::CorrectPlayerPosToServer(pPlayer);
				return;
			}			

//			ASSERT(pPlayer->GetMoveInfo()->GetCurTargetPosIdx() < pPlayer->GetMoveInfo()->GetMaxTargetPosIdx());
			CCharMove::StartMoveEx( pPlayer, gCurTime, &Start, pvTarget );

#else

			pmsg->GetTargetInfo((MOVE_INFO *)pPlayer->GetMoveInfo());			
//			ASSERT(pPlayer->GetMoveInfo()->GetCurTargetPosIdx() < pPlayer->GetMoveInfo()->GetMaxTargetPosIdx());
			CCharMove::StartMoveEx(pPlayer,gCurTime,&Start,pPlayer->GetMoveInfo()->GetTargetPosition(pPlayer->GetMoveInfo()->GetCurTargetPosIdx()));
#endif*/

			// check collision
			BASEMOVE_INFO* pMoveInfo = pPlayer->GetMoveInfo();
			pmsg->GetTargetInfo( (MOVE_INFO*)pMoveInfo );
			BYTE idx = pMoveInfo->GetCurTargetPosIdx();
			VECTOR3* pvTarget = pMoveInfo->GetTargetPosition( idx );

			VECTOR3 colpos;
			MAPTYPE MapNum = GAMERESRCMNGR->GetLoadMapNum();
			if(g_pServerSystem->GetMap()->CollisionCheck( &Start, pvTarget, &colpos, pPlayer ) )
			{
				CCharMove::CorrectPlayerPosToServer(pPlayer);
				return;
			}
			if( g_pServerSystem->GetMap()->CollisionTilePos( pvTarget->x, pvTarget->z, pPlayer ) )
			{
				CCharMove::CorrectPlayerPosToServer(pPlayer);
				return;
			}

//			ASSERT(pPlayer->GetMoveInfo()->GetCurTargetPosIdx() < pPlayer->GetMoveInfo()->GetMaxTargetPosIdx());
			CCharMove::StartMoveEx( pPlayer, gCurTime, &Start, pvTarget );
			//

			// RaMa
			CParty* pParty = PARTYMGR->GetParty(pPlayer->GetPartyIdx());
			if(pParty)
			{
				pParty->SendMemberPos(pPlayer->GetID(), pPlayer->GetMoveInfo());
			}
			
			MOVE_ONETARGETPOS_FROMSERVER msg;
			SetProtocol(&msg,MP_MOVE,MP_MOVE_ONETARGET);
			msg.dwObjectID = pPlayer->GetID();
			msg.SetStartPos(&Start);
			msg.SetTargetPos(pPlayer->GetMoveInfo()->GetTargetPosition(0));

			PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer,&msg,msg.GetSize());

#ifdef _KOR_LOCAL_
			pPlayer->SetActionTime();
#endif
		}
		break;
//
	case MP_MOVE_PET_ONETARGET:
		{
			MOVE_ONETARGETPOS* pmsg = (MOVE_ONETARGETPOS*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
				return;
			if(pPlayer->GetInited() == FALSE || pPlayer->GetInitedGrid() == FALSE)
				return;
//			CPet* pPet = (CPet*)pPlayer->GetPetManager()->GetPet(pmsg->dwMoverID);
//			CPet* pPet = (CPet*)g_pUserTable->FindUser(pmsg->dwMoverID);
			CPet* pPet = (CPet*)pPlayer->GetPetManager()->GetCurSummonPet();
			if(pPet == NULL)
				return;

			//!!! SW060616 가끔씩 나오는 오버플로우현상 잡기
			if((unsigned int)pPet == 0xfdfdfdfd)
			{
				CPetManager* AssertPetMgr = pPlayer->GetPetManager();
				//CPlayer AssertPlayer = *pPlayer;
				char tmp[256] = {0,};
				sprintf(tmp, "NeedToCheckLog:<PetMasterID>%d", pPlayer->GetID());
				ASSERTMSG(0, tmp);
				return;
			}
			//!!! 펫이나 주인 상태관련 처리들
			
			VECTOR3 Start;
			pmsg->GetStartPos(&Start);

//			VECTOR3* pCurPos = CCharMove::GetPosition(pPet);
//			if(CalcDistanceXZ(pCurPos,&Start) >= 1000)
//			{				
//				CCharMove::CorrectPlayerPosToServer(pPet);
//				return;
//			}

			BASEMOVE_INFO* pMoveInfo = pPet->GetMoveInfo();
			pmsg->GetTargetInfo( (MOVE_INFO*)pMoveInfo );
			BYTE idx = pMoveInfo->GetCurTargetPosIdx();
			VECTOR3* pvTarget = pMoveInfo->GetTargetPosition( idx );

//			VECTOR3 colpos;
//			MAPTYPE MapNum = GAMERESRCMNGR->GetLoadMapNum();
//			if(g_pServerSystem->GetMap()->CollisionCheck( &Start, pvTarget, &colpos, pPet ) )
//			{
//				CCharMove::CorrectPlayerPosToServer(pPet);
//				return;
//			}	

			CCharMove::StartMoveEx( pPet, gCurTime, &Start, pvTarget );

			MOVE_ONETARGETPOS_FROMSERVER msg;
			SetProtocol(&msg,MP_MOVE,MP_MOVE_ONETARGET);
			msg.dwObjectID = pPet->GetID();
			msg.SetStartPos(&Start);
			msg.SetTargetPos(pPet->GetMoveInfo()->GetTargetPosition(0));

			PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer,&msg,msg.GetSize());
		}
		break;
	case MP_MOVE_PET_TARGET:
		{
			MOVE_TARGETPOS* pmsg = (MOVE_TARGETPOS*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( !pPlayer || pPlayer->GetObjectKind() != eObjectKind_Player )
				return;
			//CPet* pPet = (CPet*)g_pUserTable->FindUser(pmsg->dwMoverID);
			CPet* pPet = (CPet*)pPlayer->GetPetManager()->GetCurSummonPet();
			if(pPet == NULL)
				return;

			//SW061122 방어 코드 추가.
			if( pmsg->tpos.PosNum > MAX_CHARTARGETPOSBUF_SIZE || pmsg->dwMoverID != pPet->GetID() )
				return;

			//SW060928 방어 코드 //잘못된 pet object 가 들어있는 상황이 있다.
			if(!pPet->GetMaster() || pPet->GetMaster() != pPlayer)
			{
				char tmp[256] = {0,};
				sprintf(tmp, "NeedToCheckLog:<PetMasterID>%d", pPlayer->GetID());
				ASSERTMSG(0, tmp);
				return;
			}

			//!!! 펫이나 주인 상태관련 처리들

			VECTOR3 Start;
			pmsg->GetStartPos(&Start);

//			VECTOR3* pCurPos = CCharMove::GetPosition(pPet);
//			if(CalcDistanceXZ(pCurPos,&Start) >= 1000)
//			{				
//				CCharMove::CorrectPlayerPosToServer(pPet);
//				return;
//			}

			BASEMOVE_INFO* pMoveInfo = pPet->GetMoveInfo();
			pmsg->GetTargetInfo( (MOVE_INFO*)pMoveInfo );
			BYTE idx = pMoveInfo->GetCurTargetPosIdx();
			VECTOR3* pvTarget = pMoveInfo->GetTargetPosition( idx );

//			VECTOR3 colpos;
//			MAPTYPE MapNum = GAMERESRCMNGR->GetLoadMapNum();
//			if(g_pServerSystem->GetMap()->CollisionCheck( &Start, pvTarget, &colpos, pPet ) )
//			{
//				CCharMove::CorrectPlayerPosToServer(pPet)
//				return;
//			}

			CCharMove::StartMoveEx( pPet, gCurTime, &Start, pvTarget );

			pmsg->Protocol = MP_MOVE_TARGET;
			PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer,pmsg,dwLength);
		}
		break;
	case MP_MOVE_STOP:
		{
			MOVE_POS* pmsg = (MOVE_POS*)pMsg;
			VECTOR3 pos;
			pmsg->cpos.Decompress(&pos);
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwMoverID);
			if(pPlayer == NULL)
				return;

			// 상태 변경
			//if( !OBJECTSTATEMGR_OBJ->StartObjectState(pPlayer, eObjectState_None, 0) )
			//	return;
			if(pPlayer->GetState() == eObjectState_Move)
				OBJECTSTATEMGR_OBJ->EndObjectState(pPlayer,eObjectState_Move);

			//////////////////////////////////////////////////////////////////////////
			// 클라이언트들의 충돌 체크(클라이언트들에서 하기 때문에 필요없지만... 확인용)
			// 클라이언트의 해킹등을 방지하기 위해..
			VECTOR3 colpos;
			VECTOR3* pCurPos = CCharMove::GetPosition(pPlayer);

			MAPTYPE MapNum = GAMERESRCMNGR->GetLoadMapNum();
			if(g_pServerSystem->GetMap()->CollisionCheck(pCurPos,&pos,&colpos,pPlayer))
			{
				CCharMove::CorrectPlayerPosToServer(pPlayer);
				return;
			}
			//////////////////////////////////////////////////////////////////////////	
			// 정상적인 이동인지 확인하고 틀리면 Correct를 해준다.
			if(CCharMove::CheckVaildStop(pPlayer,gCurTime,&pos) == TRUE)
				CCharMove::EndMove(pPlayer,gCurTime,&pos);	// 정상적이면 EndMove
			
			PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer,pmsg,dwLength);
		}
		break;

	case MP_MOVE_PET_STOP:
		{
			MOVE_POS* pmsg = (MOVE_POS*)pMsg;

			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
				return;
			//CPet* pPet = (CPet*)g_pUserTable->FindUser(pmsg->dwMoverID);
			CPet* pPet = (CPet*)pPlayer->GetPetManager()->GetCurSummonPet();
			if(pPet == NULL)
				return;

 			VECTOR3 pos;
 			pmsg->cpos.Decompress(&pos);		
// 			if(pPet->GetState() == eObjectState_Move)
// 				OBJECTSTATEMGR_OBJ->EndObjectState(pPet,eObjectState_Move);
// 
// 			VECTOR3 colpos;
// 			VECTOR3* pCurPos = CCharMove::GetPosition(pPet);
// 
// 			MAPTYPE MapNum = GAMERESRCMNGR->GetLoadMapNum();
// 			if(g_pServerSystem->GetMap()->CollisionCheck(pCurPos,&pos,&colpos,pPet))
// 			{
// 				CCharMove::CorrectPlayerPosToServer(pPet);
// 				return;
// 			}
			if(pPet->GetState() == eObjectState_Move)
				OBJECTSTATEMGR_OBJ->EndObjectState(pPet,eObjectState_Move);

			//VECTOR3 colpos;
			VECTOR3* pCurPos = CCharMove::GetPosition(pPet);

//			MAPTYPE MapNum = GAMERESRCMNGR->GetLoadMapNum();
//			if(g_pServerSystem->GetMap()->CollisionCheck(pCurPos,&pos,&colpos,pPet))
//			{
//				CCharMove::CorrectPlayerPosToServer(pPet);
//				return;
//			}

			if(CCharMove::CheckVaildStop(pPet,gCurTime,&pos) == TRUE)
			CCharMove::EndMove(pPet,gCurTime,&pos);

			PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pPlayer,pmsg,dwLength);
		}
		break;

	case MP_MOVE_PET_WARP_SYN:
		{	/*
			MOVE_POS* pmsg = (MOVE_POS*)pMsg;

			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
				return;
			CPet* pPet = (CPet*)pPlayer->GetPetManager()->GetCurSummonPet();
			if(pPet == NULL)
				return;

			VECTOR3 pos;
			pmsg->cpos.Decompress(&pos);
// 			if(CheckValidPosition(pos) == FALSE)
// 			{
// 				return;
// 			}
			CCharMove::SetPosition(pPet,&pos);

			if(pPet->IsPetMoving() || CCharMove::IsMoving(pPet))
			{

				CCharMove::EndMove(pPet,gCurTime,&pos);

				*pPet->GetMoveInfo() = *pPlayer->GetMoveInfo();

				BASEMOVE_INFO* pMoveInfo = pPlayer->GetMoveInfo();
				BYTE idx = pMoveInfo->GetCurTargetPosIdx();
				if(idx)
				{
					VECTOR3* pvTarget = pMoveInfo->GetTargetPosition( idx );
					//CCharMove::StartMoveEx( pPet, gCurTime, &pMoveInfo->CurPosition, pvTarget );				
					CCharMove::StartMoveEx( pPet, gCurTime, &pos, pvTarget );				
				}
			}
			else
			{
				CCharMove::EndMove(pPet,gCurTime,&pos);
			}

 			pmsg->Protocol = MP_MOVE_PET_WARP_ACK;
 
 			//pPlayer->SendMsg(pmsg,sizeof(MOVE_POS));
			PACKEDDATA_OBJ->QuickSend(pPlayer,pmsg,dwLength);*/
		}
		break;

	case MP_MOVE_WALKMODE:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
				return;
			CCharMove::SetWalkMode(pPlayer);
			
			PACKEDDATA_OBJ->QuickSend(pPlayer,pmsg,dwLength);
		}
		break;
	
	case MP_MOVE_RUNMODE:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
				return;
			CCharMove::SetRunMode(pPlayer);
			
			PACKEDDATA_OBJ->QuickSend(pPlayer,pmsg,dwLength);
		}
		break;
	
	case MP_MOVE_KYUNGGONG_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
				return;

			if(CCharMove::SetKyungGong(pPlayer,(WORD)pmsg->dwData2))
			{
				pmsg->Protocol = MP_MOVE_KYUNGGONG_ACK;
				PACKEDDATA_OBJ->QuickSend(pPlayer,pmsg,dwLength);				
			}
			else
			{
				pmsg->Protocol = MP_MOVE_KYUNGGONG_NACK;
				pPlayer->SendMsg(pmsg,dwLength);
			}			
		}
		break;
		
	case MP_MOVE_EFFECTMOVE:
		{
			MOVE_POS* pmsg = (MOVE_POS*)pMsg;
			CObject* pObject = g_pUserTable->FindUser(pmsg->dwMoverID);
			if(pObject == NULL)
				return;
			
			VECTOR3 pos;
			pmsg->cpos.Decompress(&pos);

			if( pObject->GetObjectKind() == eObjectKind_Player )
			{
				VECTOR3* pCurPos = CCharMove::GetPosition( pObject );
				if( CalcDistanceXZ( &pos, pCurPos ) >= 1500 )
				{
					CCharMove::CorrectPlayerPosToServer( pObject );
					return;
				}
			}
			
			if(CheckValidPosition(pos) == FALSE)
			{
				return;
			}

			CCharMove::SetPosition(pObject,&pos);
//			PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pObject,pmsg,dwLength);
		}
		break;	
	
	default:
		break;
	}
}

void MP_ITEMMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE *pmsg = (MSGBASE *)pMsg;
	ITEMMGR->NetworkMsgParse( dwConnectionIndex, pmsg->Protocol, pMsg );	
}

void MP_ITEMMsgParserExt(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE *pmsg = (MSGBASE *)pMsg;
	ITEMMGR->NetworkMsgParseExt( dwConnectionIndex, pmsg->Protocol, pMsg );	
}


void MP_MUGONGMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	MUGONGMNGR->NetworkMsgParse(pTempMsg->Protocol, pMsg);
/*
		MSGBASE* pTempMsg = (MSGBASE*)pMsg;
		switch(pTempMsg->Protocol)
		{
		case MP_MUGONG_MOVE_SYN:
			{
				MSG_MUGONG_MOVEINFO * msg = (MSG_MUGONG_MOVEINFO *)pMsg;
				CPlayer * player = (CPlayer *)g_pUserTable->FindUser(msg->dwObjectID);
				MUGONGMGR_OBJ->MoveMugong(player, msg);
			}
			break;
		case MP_MUGONG_ADD_SYN:
			{
				MSG_MUGONG_ADD * msg = (MSG_MUGONG_ADD *)pMsg;
				CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(msg->dwObjectID);
				if(pPlayer == NULL)
					return;
				MUGONGMGR_OBJ->AddMugong(pPlayer, msg);
			}
			break;
	
		case MP_MUGONG_REM_SYN:
			{
				MSG_MUGONG_REM_SYN * msg = (MSG_MUGONG_REM_SYN *)pMsg;
				CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(msg->dwObjectID);
				if(pPlayer == NULL)
					return;
				MUGONGMGR_OBJ->RemMugong(pPlayer, msg);
			}
			break;
		case MP_MUGONG_DELETEGROUNDADD_SYN:
			{
			
			}
			break;
		case MP_MUGONG_DELETEINVENTORYADD_SYN:
			{
				MSG_MUGONG_DELINVADD_SYN * msg = (MSG_MUGONG_DELINVADD_SYN *)pMsg;
				// MUGONG DEL
				// INVENTORY ADD
				CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(msg->dwObjectID);
				if(pPlayer == NULL)
					return;
				MUGONGMGR_OBJ->AddMugongInInventory(pPlayer, msg);		// include DB
			}
			break;
		default:
			break;
		} */
	
}
void MP_CHEATMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	CPlayer* pSender = (CPlayer*)g_pUserTable->FindUser( pTempMsg->dwObjectID );
	if(pSender)
	{
		if( pSender->GetUserLevel() > eUSERLEVEL_GM )
		{
			char buf[64];
			sprintf(buf, "User Lvl Chk! Character_idx : %d", pTempMsg->dwObjectID);
			ASSERTMSG(0, buf);
			return;
		}
	}
	switch(pTempMsg->Protocol)
	{

	case MP_CHEAT_WHEREIS_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CPlayer* pSearcher	= (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			CPlayer* pTarget	= (CPlayer*)g_pUserTable->FindUser( pmsg->dwData );
			if( !pTarget )
			{
				MSG_BYTE msg;
				msg.Category	= MP_CHEAT;
				msg.Protocol	= MP_CHEAT_WHEREIS_NACK;
				msg.dwObjectID	= pmsg->dwObjectID;
				msg.bData		= CHATERR_ERROR;
				if( pSearcher )
					pSearcher->SendMsg( &msg, sizeof( msg ) );
				else
					g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );

				return;
			}

			MSG_WHERE_INFO msgWhere;
			msgWhere.Category	= MP_CHEAT;
			msgWhere.Protocol	= MP_CHEAT_WHEREIS_MAPSERVER_ACK;
			msgWhere.dwObjectID	= pmsg->dwObjectID;
			msgWhere.wMapNum	= g_pServerSystem->GetMapNum();
			msgWhere.bChannel	= (BYTE)pTarget->GetGridID();

			VECTOR3 TargetPos = *CCharMove::GetPosition(pTarget);
			msgWhere.cpos.Compress(&TargetPos);

			if( pSearcher )
				pSearcher->SendMsg( &msgWhere, sizeof( msgWhere ) );
			else
				g_Network.Send2Server( dwConnectionIndex, (char*)&msgWhere, sizeof( msgWhere ) );
		}
		break;
	case MP_CHEAT_HIDE_SYN:
		{
			MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;
//			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
//			if(pPlayer == NULL)
//				return;
			CPlayer* pPlayer;

			if( pmsg->Name[0] == 0 )	//hero
				pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			else
				pPlayer = (CPlayer*)g_pUserTable->FindUserForName( pmsg->Name );
			if( !pPlayer )	return;

			if( pmsg->dwData == 1 )
			{
				if( pPlayer->IsVisible() == FALSE )
					return;
				pPlayer->SetVisible(FALSE);
			}
			else
			{
				if( pPlayer->IsVisible() )
					return;
				pPlayer->SetVisible(TRUE);
			}

			if( pPlayer->GetUserLevel() <= eUSERLEVEL_GM )
			{
				char Msg[1024] = {0,};
				WORD MsgLen = 0;
				CGridTable* pGridTable = g_pServerSystem->GetGridSystem()->GetGridTable( pPlayer );
				if( !pGridTable )
					return;

				if( pPlayer->IsVisible() )
					pPlayer->SetAddMsg(Msg,&MsgLen,0,TRUE);
				else if( pPlayer->IsVisible() == FALSE )
					pPlayer->SetRemoveMsg( Msg, &MsgLen, 0 );

				if( MsgLen )
					PACKEDDATA_OBJ->QuickSendExceptObjectSelf( pPlayer, (MSGBASE*)Msg, MsgLen );
			}

			MSG_DWORD2 msg;
			msg.Category	= MP_CHEAT;
			msg.Protocol	= MP_CHEAT_HIDE_ACK;
			msg.dwObjectID	= pPlayer->GetID();
			msg.dwData1		= pPlayer->GetID();
			msg.dwData2		= pmsg->dwData;

			PACKEDDATA_OBJ->QuickSend( pPlayer, &msg, sizeof(MSG_DWORD2) );
		}
		break;
//	case MP_CHEAT_BANCHARACTER_SYN:
//		{
//			MSG_NAME* pmsg = (MSG_NAME*)pMsg;
//			CPlayer* pPlayer = g_pUserTable->FindUser(pmsg->dwObjectID);
//			if(!pPlayer) break;
//			CPlayer* pBanPlayer = 
//		}
//		break;
	case MP_CHEAT_PKALLOW_SYN:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			BOOL bAllow = (BOOL)pmsg->wData2;
			PKMGR->SetPKAllowWithMsg( bAllow );

			// 06.09.11 RaMa - GM툴 사용로그추가
			if( pSender )
                LogGMToolUse( pSender->GetID(), eGMLog_PKAllow, (DWORD)g_pServerSystem->GetMapNum(), (DWORD)bAllow, 0 );
		}
		break;

	case MP_CHEAT_MOVETOCHAR_SYN:
		{
			MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;

			CObject* pPlayer = g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer ) return;
			CObject* pTarget = g_pUserTable->FindUserForName( pmsg->Name );			
			if( !pTarget ) return;
			
			VECTOR3 TargetPos;

			MOVE_POS msg;
			msg.Category	= MP_CHEAT;
			msg.Protocol	= MP_CHEAT_MOVE_ACK;

			if( pmsg->dwData == 0 )
			{
				TargetPos = *CCharMove::GetPosition(pTarget);
				CCharMove::SetPosition( pPlayer,&TargetPos);
				msg.cpos.Compress(&TargetPos);
				((CPlayer*)pPlayer)->SendMsg( &msg, sizeof(msg) );
			}
			else
			{
				TargetPos = *CCharMove::GetPosition(pPlayer);
				CCharMove::SetPosition( pTarget,&TargetPos);
				msg.cpos.Compress(&TargetPos);
				((CPlayer*)pTarget)->SendMsg( &msg, sizeof(msg) );
				//그리드에만 보내면 되는가?
			}

/*			
			MOVE_POS* pmsg = (MOVE_POS*)pMsg;
			VECTOR3 pos;
			CObject* pObject = g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pObject == NULL)
				return;
			CObject* pTargetObject = g_pUserTable->FindUser(pmsg->dwMoverID);		// targetID
			if(pTargetObject == NULL)
				return;
			VECTOR3 TargetPos = *CCharMove::GetPosition(pTargetObject);
			CCharMove::SetPosition(pObject,&TargetPos);
			pmsg->cpos.Compress(&TargetPos);

			pmsg->Protocol = MP_CHEAT_MOVE_ACK;

			((CPlayer*)pObject)->SendMsg(pmsg,sizeof(MOVE_POS));
*/
		}
		break;
	case MP_CHEAT_MOVE_SYN:
		{
			MOVE_POS* pmsg = (MOVE_POS*)pMsg;

			CObject* pObject = g_pUserTable->FindUser(pmsg->dwMoverID);
			if(pObject == NULL)
				return;

			VECTOR3 pos;
			pmsg->cpos.Decompress(&pos);
			
			ASSERTVALID_POSITION(pos);
			if(CheckValidPosition(pos) == FALSE)
			{
				return;
			}

			CCharMove::SetPosition(pObject,&pos);

			if(pObject->GetObjectKind() == eObjectKind_Player)
			{
				CPlayer* pGM = (CPlayer*)pObject;
				CPet* pPet = pGM->GetPetManager()->GetCurSummonPet();
				if( pPet )
				{
					CCharMove::SetPosition(pPet,&pos);
				}
			}

			pmsg->Protocol = MP_CHEAT_MOVE_ACK;

			((CPlayer*)pObject)->SendMsg(pmsg,sizeof(MOVE_POS));
		}
		break;
		
	case MP_CHEAT_MOVEUSER_SYN:
		{
			MOVE_POS_USER* pmsg = (MOVE_POS_USER*)pMsg;

			CObject* pObject = g_pUserTable->FindUserForName(pmsg->Name);
			if(pObject == NULL)
				return;

			VECTOR3 pos;
			pmsg->cpos.Decompress(&pos);
			CCharMove::SetPosition(pObject,&pos);

			if(pObject->GetObjectKind() == eObjectKind_Player)
			{
				CPlayer* pPlayer = (CPlayer*)pObject;
				CPet* pPet = pPlayer->GetPetManager()->GetCurSummonPet();
				if( pPet )
				{
					CCharMove::SetPosition(pPet,&pos);
				}
			}

			MOVE_POS posMsg;
			posMsg.Category = MP_CHEAT;
			posMsg.Protocol = MP_CHEAT_MOVE_ACK;
			posMsg.cpos.Compress(&pos);

			((CPlayer*)pObject)->SendMsg(&posMsg,sizeof(posMsg));
			//내 그리드에만 보낸다?
		}
		break;
		
	case MP_CHEAT_CHANGEMAP_SYN:
		{
/*			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			pmsg->Protocol = MP_CHEAT_CHANGEMAP_ACK;
			pPlayer->SendMsg(pmsg,sizeof(MSG_WORD));
*/
			MSG_NAME_DWORD2* pmsg = (MSG_NAME_DWORD2*)pMsg;
			CPlayer* pPlayer;

			if( pmsg->Name[0] == 0 )
				pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			else
				pPlayer = (CPlayer*)g_pUserTable->FindUserForName(pmsg->Name);

			if(pPlayer == NULL)
				return;

			//2008. 5. 20. CBH - 맵 채널 이동시 자신 맵의 채널을 검사하는 코드인데
			//맵 채널 분리 작업으로 인해 맵의 채널이 각각 다르므로 주석 처리
			/*
			if( CHANNELSYSTEM->GetChannelID( pmsg->dwData2 ) == 0 && !CHANNELSYSTEM->IsQuestMap() )
			{
				MSG_DWORD msg;
				msg.Category	= MP_CHEAT;
				msg.Protocol	= MP_CHEAT_CHANGEMAP_NACK;
				msg.dwData		= pPlayer->GetGridID() - 1;
				pPlayer->SendMsg( &msg, sizeof( msg ) );
				return;
			}
			*/
				
			MAPCHANGE_INFO* ChangeInfo = GAMERESRCMNGR->GetMapChangeInfoForDestMapNum( (WORD)pmsg->dwData1 );
			if( !ChangeInfo ) return;	//nackif( !ChangeInfo ) return;	//nack
			SaveMapChangePointUpdate( pPlayer->GetID(), ChangeInfo->Kind );

			// RaMa - 04.12.14
			pPlayer->UpdateLogoutToDB();
					
			g_pServerSystem->RemovePlayer( pPlayer->GetID() );

			MSG_WORD2 msg;
			msg.Category	= MP_USERCONN;
			msg.Protocol	= MP_USERCONN_MAP_OUT_WITHMAPNUM;
			msg.dwObjectID	= pPlayer->GetID();
			msg.wData1		= (WORD)pmsg->dwData1;
			msg.wData2		= (WORD)pmsg->dwData2;

			g_Network.Broadcast2AgentServer( (char*)&msg, sizeof(msg));
/*			
			//GMLOG
			//MAP / CHANNEL / X,Y 
			VECTOR3 vPos;
			pPlayer->GetPosition( &vPos );
			DWORD dwParam1 = ( g_pServerSystem->GetMapNum() << 16 ) | (WORD)pPlayer->GetChannelID();
			DWORD dwParam2 = ( (WORD)( vPos.x / 100 ) << 16  ) | ( (WORD)vPos.z / 100 );
			DWORD dwParam3 = ( ChangeInfo->MoveMapNum << 16 ) | pmsg->dwData2;
			DWORD dwParam4 = ( (WORD)ChangeInfo->MovePoint.x << 16 ) | (WORD)ChangeInfo->MovePoint.y;

			InsertLogTool( eToolLog_GM, eGMLog_Move,1, NULL, pPlayer->GetID(), pPlayer->GetObjectName(),
							dwParam1, dwParam2, dwParam3, dwParam4 );
*/
		}
		break;
	case MP_CHEAT_LIFE_SYN:
		{
		}
		break;
	case MP_CHEAT_MP_SYN:
		{
			
		}
		break;
	case MP_CHEAT_ITEM_SYN:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer) return;

			ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pmsg->wData1 );
			if(!pItemInfo)		return;

			if( pItemInfo->ItemKind & eSHOP_ITEM )
			{
				ITEMMGR->CheatObtainShopItem( pPlayer, pmsg->wData1, pmsg->wData2 );
			}
			else
			{
				ITEMMGR->CheatObtainItem( pPlayer, pmsg->wData1, pmsg->wData2 );
			}

//			if(ITEMMGR->MonsterObtainItem(pPlayer, pmsg->wData1, 0) == 0)
//			{
				//if(pPlayer->GetPartyIdx())
				//{
				//	PARTYMGR->SendObtainItemMsg(pPlayer, pmsg->wData);	
				//}
//			}
		}
		break;
		//SW051011 강화/레어 치트
	case MP_CHEAT_ITEM_OPTION_SYN:
		{
			SEND_CHEAT_ITEM_OPTION* pmsg = (SEND_CHEAT_ITEM_OPTION*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer) return;

			ITEM_INFO* pItemInfo = ITEMMGR->GetItemInfo( pmsg->wItemIdx );
			if(!pItemInfo)	return;

			//CommonGameDefine.h 에 넣고 싶다.
			enum{ eKindOption_None=0, eKindOption_Reinforce=1, eKindOption_Rare=2, };

			//장착 아이템이 아니면 리턴.
			if( !(pItemInfo->ItemKind & eEQUIP_ITEM) ) return;

			//기존에 있는 아이템을 강화.
			if( pmsg->wOptionKind & eKindOption_Reinforce )
			{
				//해당 아이템 정보를 가져와 기존 강화 옵션이 있으면 지운다.
				const ITEMBASE * pTargetItemBase = ITEMMGR->GetItemInfoAbsIn(pPlayer, pmsg->bPosition);
				if(pTargetItemBase->Durability != 0)
					CharacterItemOptionDelete(pTargetItemBase->Durability, pTargetItemBase->dwDBIdx);

				CItemSlot * pTargetSlot = pPlayer->GetSlot(pmsg->bPosition);
				if( pTargetSlot->UpdateItemAbs(pPlayer, pmsg->bPosition, 0, 0, 0, 0, 0, UB_DURA, SS_PREINSERT ) != EI_TRUE )
				{
					return;
				}
				//치트 옵션을 등록한다.
				pmsg->OptionInfo.dwItemDBIdx = pTargetItemBase->dwDBIdx;
				ItemOptionInsertToDB(pPlayer->GetID(), pmsg->wItemIdx, pmsg->bPosition, &pmsg->OptionInfo);				

				// 06.09.12 RaMa
				LogGMToolUse( pPlayer->GetID(), eGMLog_Item, eLog_ItemOptionObtainCheat, pmsg->wItemIdx, pmsg->OptionInfo.dwItemDBIdx );
			}
			else if( pmsg->wOptionKind & eKindOption_Rare )
			{
				ITEMMGR->CheatObtainRareItem( pPlayer, pmsg->wItemIdx, 1, &pmsg->OptionInfo );
			}
		}
		break;

	case MP_CHEAT_MANA_SYN:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer) return;
			int tmp = pPlayer->GetMaxNaeRyuk() - (pPlayer->GetNaeRyuk()+pmsg->wData);
			if(tmp < 0)
				pPlayer->SetNaeRyuk(pPlayer->GetMaxNaeRyuk());
			else
				pPlayer->SetNaeRyuk(pPlayer->GetNaeRyuk()+pmsg->wData);
		}
		break;
		
	case MP_CHEAT_ADDMUGONG_SYN:
		{
			MSG_WORD3* pmsg = (MSG_WORD3*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer) return;

			MUGONGBASE* info = pPlayer->GetMugongBaseByMugongIdx(pmsg->wData1);
			if(info != NULL)
				return;

			MSGBASE msg;
			msg.Category = MP_CHEAT;
			if( MUGONGMNGR->AddMugong(pPlayer, pmsg->wData1, pmsg->wData2) )
			{
				msg.Protocol = MP_CHEAT_ADDMUGONG_ACK;
				LogCheat(pPlayer->GetID(),eCHT_AddMugong,pmsg->wData2,pmsg->wData1,0);
			}
			else
			{
				msg.Protocol = MP_CHEAT_ADDMUGONG_NACK;
			}
			pPlayer->SendMsg( &msg, sizeof(msg) );
		}
		break;
	case MP_CHEAT_MUGONGSUNG_SYN:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer) return;

			MUGONGBASE* info = pPlayer->GetMugongBaseByMugongIdx(pmsg->wData1);
			if(info == NULL)
				return;

			pPlayer->SetPlayerMugongSung(info->Position,(BYTE)pmsg->wData2,eLog_MugongCheatLevelup);
			LogCheat(pPlayer->GetID(),eCHT_MugongSung,pmsg->wData2,pmsg->wData1,0);

			info->ExpPoint = 0;
			
			MSG_MUGONGEXP msg;
			msg.Category	= MP_MUGONG;
			msg.Protocol	= MP_MUGONG_EXPPOINT_NOTIFY;
			msg.absPos		= info->Position;
			msg.ExpPoint	= info->ExpPoint;
			pPlayer->SendMsg(&msg,sizeof(msg));

			if(info->Sung == 6 || info->Sung == 11)
			{
				CSkillInfo* pSkillInfo = SKILLMGR->GetSkillInfo(info->wIconIdx);
				if(pSkillInfo == NULL)
					return;
				pPlayer->AddExpPointToMugong(info->wIconIdx,pSkillInfo->GetNeedExp(info->Sung),FALSE);
			}

		}
		break;
		
	case MP_CHEAT_MONEY_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer) return;

			if( pPlayer->GetMoney() > pmsg->dwData )
			{
				pPlayer->SetMoney(pPlayer->GetMoney() - pmsg->dwData, MONEY_SUBTRACTION, 0, eItemTable_Inventory, eMoneyLog_GetCheatMoney, 0);
				// 06.09.12 RaMa
				LogGMToolUse( pPlayer->GetID(), eGMLog_Money, eMoneyLog_LoseGMTool, pPlayer->GetMoney() - pmsg->dwData, 0 );
			}
			else if( pPlayer->GetMoney() < pmsg->dwData )
			{
				pPlayer->SetMoney(pmsg->dwData - pPlayer->GetMoney(), MONEY_ADDITION, 0, eItemTable_Inventory, eMoneyLog_GetCheatMoney, 0);
				// 06.09.12 RaMa
				LogGMToolUse( pPlayer->GetID(), eGMLog_Money, eMoneyLog_GetGMTool, pmsg->dwData - pPlayer->GetMoney(), 0 );				
			}
			
			LogCheat(pPlayer->GetID(),eCHT_Money,pmsg->dwData,0,0);			
		}
		break;

	case MP_CHEAT_RELOADING:
		{
			g_pServerSystem->ReloadResourceData();
		}
		break;

	case MP_CHEAT_LEVELUP_SYN:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer) 
				return;
			EXPTYPE pointForLevel=0;
			LEVELTYPE curlvl = pPlayer->GetLevel();
			if(curlvl > pmsg->wData)
				return;
			else
			{
				LEVELTYPE diff = pmsg->wData - curlvl;
				for(LEVELTYPE i=0; i<diff; ++i)
				{
					pointForLevel = pointForLevel + GAMERESRCMNGR->GetMaxExpPoint(pPlayer->GetLevel()+i);
					
				}
				pPlayer->SetLevel(pmsg->wData-1);
				pPlayer->SetPlayerExpPoint(pointForLevel);
				pPlayer->SetPlayerExpPoint(1);					
				
				// YH
				CharacterHeroInfoUpdate(pPlayer);
				CharacterTotalInfoUpdate(pPlayer);		

				LogCheat(pPlayer->GetID(),eCHT_LevelUp,pmsg->wData,0,0);		
			}
		}
		break;
        // magi82 cheat damage //////////////////////////////////////////////////////////////////////////
	case MP_CHEAT_DAMAGE_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			float fRate = (float)(pmsg->dwData * 0.000001f);
			ATTACKMGR->SetDamageRate(fRate);
		}
		break;        	
		///////////////////////////////////////////////////////////////////////////////////////

	case MP_CHEAT_ABILITYEXP_SYN:
		{
			MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer) return;

			pPlayer->AddAbilityExp(pmsg->dwData);
			LogCheat(pPlayer->GetID(),eCHT_AbilityExp,pmsg->dwData,0,0);
		}
		break;
	case MP_CHEAT_EVENT_MONSTER_REGEN:
		{
			MSG_EVENT_MONSTERREGEN* pmsg = (MSG_EVENT_MONSTERREGEN*)pMsg;
			CObject* pPlayer = g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer) return;
			RECALLMGR->EventRecall( pPlayer, pmsg->MonsterKind, pmsg->cbMobCount, pmsg->cbChannel, &pmsg->Pos, pmsg->wRadius, pmsg->ItemID, pmsg->dwDropRatio );

			// 06.09.12 RaMa
			LogGMToolUse( pPlayer->GetID(), eGMLog_Summon, pmsg->MonsterKind, pmsg->cbMobCount, pmsg->ItemID );
		}
		break;
	case MP_CHEAT_EVENT_MONSTER_DELETE:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CObject* pPlayer = g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer) return;

			RECALLMGR->DeleteEventRecallMonster();

			// 06.09.12 RaMa
			LogGMToolUse( pPlayer->GetID(), eGMLog_SummonDelete, 0, 0, 0 );
		}
		break;
	case MP_CHEAT_GENGOL_SYN:
		{
			MSG_WORD *pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer) return;
			if((pmsg->wData < 12) || (pmsg->wData > 400))
				return;
			pPlayer->SetGenGol(pmsg->wData);
			LogCheat(pPlayer->GetID(),eCHT_Gengol,pmsg->wData,0,0);
		}
		break;
	case MP_CHEAT_MINCHUB_SYN:
		{
			MSG_WORD *pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer) return;
			if((pmsg->wData < 12) || (pmsg->wData > 400))
				return;
			pPlayer->SetMinChub(pmsg->wData);
			LogCheat(pPlayer->GetID(),eCHT_Minchub,pmsg->wData,0,0);
		}
		break;
	case MP_CHEAT_CHERYUK_SYN:
		{
			MSG_WORD *pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer) return;
			if((pmsg->wData < 12) || (pmsg->wData > 400))
				return;
			pPlayer->SetCheRyuk(pmsg->wData);
			LogCheat(pPlayer->GetID(),eCHT_Cheryuk,pmsg->wData,0,0);
		}
		break;
	case MP_CHEATE_SIMMEK_SYN:
		{
			MSG_WORD *pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer) return;
			if((pmsg->wData < 12) || (pmsg->wData > 400))
				return;
			pPlayer->SetSimMek(pmsg->wData);
			LogCheat(pPlayer->GetID(),eCHT_Simmak,pmsg->wData,0,0);
		}
		break;
	case MP_CHEAT_KILL_ALLMONSTER_SYN:
		{
			CObject* pInfo = NULL;
			g_pUserTable->SetPositionHead();
			while(pInfo = g_pUserTable->GetData())
			{
				BYTE objectKind = pInfo->GetObjectKind();
				if( (objectKind == eObjectKind_Monster) || (objectKind == eObjectKind_TitanMonster) )
				{
					if(pInfo->GetGridID() == 1)
					{
						//g_pServerSystem->RemoveMonster( pInfo->GetID() );
						pInfo->Die(NULL);

						// 06.09.12 RaMa
						LogGMToolUse( pInfo->GetID(), eGMLog_MonsterKill, MP_CHEAT_KILL_ALLMONSTER_SYN, pInfo->GetID(), 0 );
					}
				}
			}
		}
		break;
	case MP_CHEAT_PARTYINFO_SYN:
		{
			MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
			CParty * pParty = PARTYMGR->GetParty(pmsg->dwData);
			PARTY_INFO SendParty;
			memset(&SendParty, 0, sizeof(PARTY_INFO));
			if(pParty)
			{
				SendParty.Category = MP_CHEAT;
				SendParty.Protocol = MP_CHEAT_PARTYINFO_ACK;
				SendParty.PartyDBIdx = pmsg->dwData;
				for(int i=0; i<MAX_PARTY_LISTNUM; ++i)
				{
					pParty->GetMemberInfo(i, &SendParty.Member[i]);
				}
				SendParty.Option = pParty->GetOption();
				//2008. 5. 22. CBH - 방파 추가 옵션 셋팅
				PARTY_ADDOPTION* pAddOption = pParty->GetAddOption();
				memcpy(&SendParty.PartyAddOption, pAddOption, sizeof(SendParty.PartyAddOption));
			}
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer)
			{
				pPlayer->SendMsg(&SendParty, sizeof(SendParty));
			}
		}
		break;

	case MP_CHEAT_EVENT_SYN:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;

			if( pmsg->wData1 )
				gEventRate[pmsg->wData1] = pmsg->wData2*0.01f;

			/*
			if( pmsg->wData1 == eEvent_ExpRate )
				gExpRate = pmsg->wData2*0.01f;
			else if( pmsg->wData1 == eEvent_ItemRate )
				gItemRate = pmsg->wData2*0.01f;
			else if( pmsg->wData1 == eEvent_MoneyRate )
				gMoneyRate = pmsg->wData2*0.01f;
			else if( pmsg->wData1 == eEvent_DamageReciveRate )
				gDamageReciveRate = pmsg->wData2*0.01f;
			else if( pmsg->wData1 == eEvent_DamageRate )
				gDamageRate = pmsg->wData2*0.01f;
			else if( pmsg->wData1 == eEvent_NaeRuykRate )
				gNaeRuykRate = pmsg->wData2*0.01f;
			else if( pmsg->wData1 == eEvent_UngiSpeed )
				gUngiSpeed = pmsg->wData2*0.01f;
			else if( pmsg->wData1 == eEvent_PartyExpRate )
				gPartyExpRate = pmsg->wData2*0.01f;
			else if( pmsg->wData1 == eEvent_AbilRate )
				gAbilRate = pmsg->wData2*0.01f;
			else if( pmsg->wData1 == eEvent_GetMoney )
				gGetMoney = pmsg->wData2*0.01f;
			*/
		}
		break;
	case MP_CHEAT_PLUSTIME_ALLOFF:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			g_bPlusTime = FALSE;

			// 모든플러스타임이 종료되어있는지 확인
			for(int i=0; i<eEvent_Max; ++i)
			{
				if( gEventRate[i] != gEventRateFile[i] )
					break;
			}
			
			if( i != eEvent_Max )
			{
				// 적용
				for(i=0; i<eEvent_Max; ++i)
					gEventRate[i] = gEventRateFile[i];
				
				// 개인플러스타임 계산
				CObject* pObject = NULL;
				g_pUserTable->SetPositionHead();
				while( pObject = g_pUserTable->GetData() )
				{
					if( pObject->GetObjectKind() != eObjectKind_Player )		continue;
					
					((CPlayer*)pObject)->GetShopItemManager()->CalcPlusTime( 0, MP_CHEAT_PLUSTIME_ALLOFF );
				}
			}
		}
		break;
	case MP_CHEAT_PLUSTIME_ON:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;

			g_bPlusTime = TRUE;

			// 다른에이전트에서 오는건 막는다.
			if( gEventRate[pmsg->wData1] != pmsg->wData2*0.01f )
				gEventRate[pmsg->wData1] = pmsg->wData2*0.01f;
			else
				return;

			// 개인플러스타임 계산
			CObject* pObject = NULL;
			g_pUserTable->SetPositionHead();
			while( pObject = g_pUserTable->GetData() )
			{
				if( pObject->GetObjectKind() != eObjectKind_Player )		continue;

				((CPlayer*)pObject)->GetShopItemManager()->CalcPlusTime( pmsg->wData1, MP_CHEAT_PLUSTIME_ON );
			}
		}
		break;
	case MP_CHEAT_PLUSTIME_OFF:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;

			// 다른에이전트 에서 오는건 막는다.
			if( gEventRate[pmsg->wData] != gEventRateFile[pmsg->wData] )
				gEventRate[pmsg->wData] = gEventRateFile[pmsg->wData];
			else
				return;

			// 모든플러스타임이 종료되었으면 플러스타임 종료
			for(int i=0; i<eEvent_Max; ++i)
			{
				if( gEventRate[i] != gEventRateFile[i] )
					break;
			}
			if( i == eEvent_Max )			g_bPlusTime = FALSE;

			// 개인플러스타임 계산
			CObject* pObject = NULL;
			g_pUserTable->SetPositionHead();
			while( pObject = g_pUserTable->GetData() )
			{
				if( pObject->GetObjectKind() != eObjectKind_Player )		continue;

				((CPlayer*)pObject)->GetShopItemManager()->CalcPlusTime( pmsg->wData, MP_CHEAT_PLUSTIME_OFF );
			}
		}
		break;
	case MP_CHEAT_MUNPADATECLR_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == 0)
				return;
			pPlayer->ClrGuildCanEntryDate();
		}
		break;

// eventmap----------------------------------------------------------------------------		
	case MP_CHEAT_CHANGE_EVENTMAP_SYN: // from agent
		{
			MSG_NAME_DWORD3* pmsg = (MSG_NAME_DWORD3*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUserForName( pmsg->Name );
			if( !pPlayer )	return;

			DWORD dwParty = pPlayer->GetPartyIdx();
//			CPlayer* pData = NULL;
			CObject* pData = NULL;
			if( dwParty )
			{
				cPtrList list;
				g_pUserTable->SetPositionHead();
				while( pData = g_pUserTable->GetData() )
				{
					if( pData->GetObjectKind() != eObjectKind_Player )
						continue;

					if( dwParty == ((CPlayer*)pData)->GetPartyIdx() )
					{
						list.AddTail(pData);
						//g_pServerSystem->RemovePlayer( pData->GetID() );

						MSG_DWORD3 msg;
						msg.Category = MP_USERCONN;
						msg.Protocol = MP_USERCONN_MAP_OUT_TO_EVENTMAP;
						msg.dwObjectID = pData->GetID();
						msg.dwData1 = pmsg->dwData1;	// map
						msg.dwData2 = pmsg->dwData2;	// channel
						msg.dwData3 = pmsg->dwData3;	// team

						g_Network.Broadcast2AgentServer( (char*)&msg, sizeof(msg));
					}
				}
				PTRLISTPOS pos = list.GetHeadPosition();
				while( pos )
				{
					CObject* p = (CObject*)list.GetNext(pos);
					g_pServerSystem->RemovePlayer( p->GetID() );
				}
				list.RemoveAll();
			}
			else
			{
				g_pServerSystem->RemovePlayer( pPlayer->GetID() );

				MSG_DWORD3 msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_MAP_OUT_TO_EVENTMAP;
				msg.dwObjectID = pPlayer->GetID();
				msg.dwData1 = pmsg->dwData1;
				msg.dwData2 = pmsg->dwData2;
				msg.dwData3 = pmsg->dwData3;

				g_Network.Broadcast2AgentServer( (char*)&msg, sizeof(msg));
			}
		}
		break;		
	case MP_CHEAT_EVENT_START_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			EVENTMAPMGR->StartEvent( pmsg->dwData2 );
		}
		break;
	case MP_CHEAT_EVENT_READY_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			EVENTMAPMGR->ReadyEvent( pmsg->dwData2 );
		}
		break;		
//------------------------------------------------------------------------------------------------

	case MP_CHEAT_GETSHOPITEM:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;

			DWORD dur = 0;
			if( ITEMMGR->IsDupItem( pmsg->wData ) )
				dur = 1;

			ShopItemGetItem( pmsg->dwObjectID, pmsg->wData, dur );

			// 06.09.12 RaMa
			LogGMToolUse( pmsg->dwObjectID, eGMLog_Item, eLog_ShopItemGetCheat, pmsg->wData, dur );
		}
		break;

	case MP_CHEAT_STAGE_SYN:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer )	return;
			ChangeCharacterStageAbility( pPlayer->GetID(), pPlayer->GetStage(), pPlayer->GetAbilityGroup() );

			pPlayer->SetStage( pmsg->bData );			
		}
		break;

	case MP_CHEAT_BOSSMOB_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer )	return;
			
			QuestMainQuestInsert( pmsg->dwObjectID, 23, -33554432, 0 );
			QuestSubQuestInsert( pmsg->dwObjectID, 23, 7, 0, 0 );
			QuestSubQuestInsert( pmsg->dwObjectID, 23, 8, 0, 0 );
		}
		break;
		
	case MP_CHEAT_JACKPOT_GETPRIZE:
		{

//			MSG_NAME_DWORD2* pmsg = (MSG_NAME_DWORD2*)pMsg;
//			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUserForName( pmsg->Name );
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer ) return;

			if( 0 > pmsg->dwData && pmsg->dwData > 2 ) return;

			JACKPOTMGR->SendMsgDBPrizeInfo( pPlayer, pmsg->dwData );
//			JackpotPrizeInfo( m_dwMapJPMoney, pPlayer->GetID(), m_nPrizeKind, m_stPrize[m_nPrizeKind].m_dwPercent);
		}
		break;
		
	case MP_CHEAT_JACKPOT_ONOFF:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			
			if( pmsg->dwData == 0 || pmsg->dwData == 1 )
			JACKPOTMGR->SetPrizeOnOff((BOOL)pmsg->dwData);
		}
		break;
		
	case MP_CHEAT_JACKPOT_MONEYPERMONSTER:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			if( pmsg->dwData < 0 || pmsg->dwData > 1000 ) return;
			JACKPOTMGR->SetChipPerMon( pmsg->dwData );
		}
		break;

	case MP_CHEAT_JACKPOT_PROBABILITY:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

			if( pmsg->dwData1 < 0 || pmsg->dwData1 > 2 ) return;
			if( pmsg->dwData2 < 0 || pmsg->dwData2 > 100000000 ) return;
			if( pmsg->dwData3 < 0 || pmsg->dwData3 > 100 ) return;

			JACKPOTMGR->SetPrizeProb( pmsg->dwData1, pmsg->dwData2, pmsg->dwData3 );
		}
		break;

	case MP_CHEAT_JACKPOT_CONTROL:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			
			enum {TIMELENGTH, };	//CommonHeader 로~

			if( pmsg->dwData1 == TIMELENGTH )
			{
				if( pmsg->dwData2 < 0 || pmsg->dwData2 > 3600000 ) return;
				
				JACKPOTMGR->SetDBUpdateTimeLen( pmsg->dwData2 );
			}
		}
		break;

	case MP_CHEAT_GUILDUNION_CLEAR:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer )	return;

			GUILDUNIONMGR->ClearEntryTime( pPlayer->GetGuildIdx() );
		}
		break;
	case MP_CHEAT_MOB_CHAT:
		{
			TESTMSGID* pmsg = (TESTMSGID*)pMsg;
			CObject* pObject = g_pUserTable->FindUser( pmsg->dwSenderID );
			if( pObject == NULL ) return;

			char buf[MAX_CHAT_LENGTH+1];
			SafeStrCpy( buf, pmsg->Msg, MAX_CHAT_LENGTH+1 );

			if( pObject->GetObjectKind() & eObjectKind_Monster )
			{
				((CMonster*)pObject)->AddChat( buf );
			}
			else if( pObject->GetObjectKind() == eObjectKind_Npc )
			{
				((CNpc*)pObject)->AddChat( buf );
			}
		}
		break;
	case MP_CHEAT_MUSSANG_READY:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer ) return;

			pPlayer->CheatMussangReady();
		}
		break;
	case MP_CHEAT_PET_STAMINA:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer ) return;

			CPet* pPet = pPlayer->GetPetManager()->GetCurSummonPet();
			if(!pPet)	return;

			//pmsg->bData 1이면 최대 0이면 최소 세팅.
			pPlayer->GetPetManager()->SetSommonPetStamina(pmsg->bData);
		}
		break;
	case MP_CHEAT_PET_FRIENDSHIP_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer ) return;

			CPet* pPet = pPlayer->GetPetManager()->GetCurSummonPet();
			if(!pPet)	return;

			//pmsg->bData 1이면 최대 0이면 최소 세팅.
			MSG_DWORD Msg;
			Msg.Category = MP_CHEAT;
			DWORD rt = 0;
			if( rt = pPlayer->GetPetManager()->SetSommonPetFriendship(pmsg->dwData) )
			{
				Msg.Protocol = MP_CHEAT_PET_FRIENDSHIP_NACK;
			}
			else
			{
				Msg.Protocol = MP_CHEAT_PET_FRIENDSHIP_ACK;
			}
			Msg.dwData = rt;

			pPlayer->SendMsg( &Msg, sizeof(Msg) );
		}
		break;
	case MP_CHEAT_PET_SKILLREADY_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer ) return;

			CPet* pPet = pPlayer->GetPetManager()->GetCurSummonPet();
			if(!pPet)	return;

			pPlayer->GetPetManager()->SetSommonPetSkillReady();
		}
		break;
	case MP_CHEAT_PET_SELECTED_FRIENDSHIP_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer ) return;

			//
			enum tempflag{Err_PFR_OverValue = 0xfffffa, Err_PFR_NonSummon = 0xfffffb};
			
			MSG_DWORD2 Msg;
			Msg.Category = MP_CHEAT;

			//CPet* pPet = (CPet*)g_pUserTable->FindUser( pmsg->dwData1 );
			CObject* pObj = g_pUserTable->FindUser( pmsg->dwData1 );

			if( NULL == pObj || pObj->GetObjectKind() != eObjectKind_Pet )
			{
				Msg.Protocol = MP_CHEAT_PET_SELECTED_FRIENDSHIP_NACK;
				Msg.dwData1 = Err_PFR_NonSummon;
			}
			else
			{
				CPet* pPet = (CPet*)pObj;
				DWORD friendship = pPet->GetPetFriendShip();
				Msg.dwData1 = friendship;

				if( pmsg->dwData2 )
				{	//지정 수치로 복구
					friendship = pmsg->dwData2;					
				}
				else
				{	//기본 패널티 만큼 복구
					friendship += GAMERESRCMNGR->GetPetRule()->FriendShipPerMasterDie * -1;
				}

				///////check availability////////
				DWORD max_friendly = GAMERESRCMNGR->GetPetRule()->MaxFriendship;

				if( friendship > max_friendly )
				{
					Msg.Protocol = MP_CHEAT_PET_SELECTED_FRIENDSHIP_NACK;
					Msg.dwData1 = Err_PFR_OverValue;
				}
				else
				{
					pPet->SetPetFriendShip(friendship);
					Msg.Protocol = MP_CHEAT_PET_SELECTED_FRIENDSHIP_ACK;
					Msg.dwData2 = pPet->GetPetFriendShip();
					//DB 펫 정보 갱신
				}
			}
			
			pPlayer->SendMsg( &Msg, sizeof(Msg) );
		}
		break;
	case MP_CHEAT_GUILDPOINT_SYN:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer ) return;

			MSG_DWORD rtMsg;
			rtMsg.Category = MP_CHEAT;

			//
			enum tempflag{Err_GP_NonePlayer = 0xfffffa, Err_GP_NoneGuild = 0xfffffb};
			
			CObject* pObj = g_pUserTable->FindUser( pmsg->dwData1 );
			if( NULL == pObj || pObj->GetObjectKind() != eObjectKind_Player )
			{
				rtMsg.Protocol = MP_CHEAT_GUILDPOINT_NACK;
				rtMsg.dwData = Err_GP_NonePlayer;
			}
			else
			{
				CPlayer* pTargetPlayer = (CPlayer*)pObj;
				CGuild* pGuild = GUILDMGR->GetGuild(pmsg->dwData2);
				if( NULL == pGuild || pTargetPlayer->GetGuildIdx() != pmsg->dwData2 )
				{
					rtMsg.Protocol = MP_CHEAT_GUILDPOINT_NACK;
					rtMsg.dwData = Err_GP_NoneGuild;
				}
				else
				{
					//DB
					LogGuildPoint( pmsg->dwData2, pGuild->GetGuildPoint(), TRUE, eGM_CheatGuildPoint, pmsg->dwData3, pTargetPlayer->GetID(), pPlayer->GetID());

					GuildAddGuildPoint(pmsg->dwData2, pmsg->dwData3, eGM_CheatGuildPoint, pPlayer->GetID());

					rtMsg.Protocol = MP_CHEAT_GUILDPOINT_ACK;
				}
			}

			pPlayer->SendMsg( &rtMsg, sizeof(rtMsg) );
		}
		break;
	case MP_CHEAT_GUILDHUNTED_MONSTERCOUNT_SYN:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer ) return;

			MSG_DWORD rtMsg;
			rtMsg.Category = MP_CHEAT;

			//
			enum tempflag{Err_GMC_NonePlayer = 0xfffffa, Err_GMC_NoneGuild = 0xfffffb};
			
			CObject* pObj = g_pUserTable->FindUser( pmsg->dwData1 );
			if( NULL == pObj || pObj->GetObjectKind() != eObjectKind_Player )
			{
				rtMsg.Protocol = MP_CHEAT_GUILDHUNTED_MONSTERCOUNT_NACK;
				rtMsg.dwData = Err_GMC_NonePlayer;
			}
			else
			{
				CPlayer* pTargetPlayer = (CPlayer*)pObj;
				CGuild* pGuild = GUILDMGR->GetGuild(pmsg->dwData2);
				if( NULL == pGuild || pTargetPlayer->GetGuildIdx() != pmsg->dwData2 )
				{
					rtMsg.Protocol = MP_CHEAT_GUILDHUNTED_MONSTERCOUNT_NACK;
					rtMsg.dwData = Err_GMC_NoneGuild;
				}
				else
				{
					GuildAddHuntedMonsterCount(pmsg->dwData2, pmsg->dwData3);
					//GUILDMGR->SendGuildHuntedMonsterTotalCountToMapServerExceptOneself(pmsg->dwData2, pmsg->dwData3);
					rtMsg.Protocol = MP_CHEAT_GUILDHUNTED_MONSTERCOUNT_ACK;
				}
			}

			pPlayer->SendMsg( &rtMsg, sizeof(rtMsg) );
		}
		break;

#ifdef _JAPAN_LOCAL_
	case MP_CHEAT_CHANGE_MAINATTR_SYN:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer )	return;

			MSG_DWORD Msg;
			Msg.Category = MP_CHEAT;
			if( pPlayer->ChangeMainCharAttr( pmsg->wData ) )
				Msg.Protocol = MP_CHEAT_CHANGE_MAINATTR_ACK;
			else
				Msg.Protocol = MP_CHEAT_CHANGE_MAINATTR_NACK;
			Msg.dwData = MAKEDWORD(pPlayer->GetMainCharAttr(),0);

			pPlayer->SendMsg( &Msg, sizeof(Msg) );
		}
		break;
	case MP_CHEAT_CHANGE_SUBATTR_SYN:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer )	return;

			MSG_DWORD Msg;
			Msg.Category = MP_CHEAT;
			if( pPlayer->ChangeSubCharAttr( pmsg->wData ) )
				Msg.Protocol = MP_CHEAT_CHANGE_SUBATTR_ACK;
			else
				Msg.Protocol = MP_CHEAT_CHANGE_SUBATTR_NACK;
			Msg.dwData = MAKEDWORD(pPlayer->GetMainCharAttr(),pPlayer->GetSubCharAttr());

			pPlayer->SendMsg( &Msg, sizeof(Msg) );
		}
		break;
#endif
	// 06. 05 HIDE NPC - 이영준
	case MP_CHEAT_NPCHIDE_SYN:
		{
			MSG_WORD3* pmsg = (MSG_WORD3*)pMsg;

			// 숨김 가능 NPC 정보를 가져온다
			HIDE_NPC_INFO* pInfo = GAMERESRCMNGR->GetHideNpcInfo(pmsg->wData1);

			if(pInfo)
			{
				// 전체 채널
				if(pmsg->wData2 == 0)
				{
					for( DWORD i = 0 ; i < MAX_CHANNEL_NUM ; ++i )
						pInfo->ChannelInfo[i] = (BOOL)pmsg->wData3;

					CObject* pObject = NULL;
					g_pUserTable->SetPositionHead();

					MSG_WORD2 msg;
					msg.Category = MP_CHEAT;
					msg.Protocol = MP_CHEAT_NPCHIDE_ACK;
					msg.wData1 = pmsg->wData1;
					msg.wData2 = pmsg->wData3;

					while( pObject = g_pUserTable->GetData() )
					{
						if( pObject->GetObjectKind() == eObjectKind_Player )
							pObject->SendMsg(&msg, sizeof(msg));
					}
				}
				// 특정 채널
				else
				{
					pInfo->ChannelInfo[pmsg->wData2 - 1] = (BOOL)pmsg->wData3;
					
					DWORD dwChannelID = CHANNELSYSTEM->GetChannelID( pmsg->wData2 - 1 );

					if(dwChannelID)
					{
						CObject* pObject = NULL;
						g_pUserTable->SetPositionHead();

						MSG_WORD2 msg;
						msg.Category = MP_CHEAT;
						msg.Protocol = MP_CHEAT_NPCHIDE_ACK;
						msg.wData1 = pmsg->wData1;
						msg.wData2 = pmsg->wData3;

						while( pObject = g_pUserTable->GetData() )
						{
							if( pObject->GetObjectKind() == eObjectKind_Player )
								if( ((CPlayer*)pObject)->GetChannelID() == dwChannelID )
									pObject->SendMsg(&msg, sizeof(msg));
						}
					}
				}
			}
		}
		break;

	case MP_CHEAT_MAP_CONDITION:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer )	return;			

			g_pServerSystem->CheckServerInfo( pmsg->dwObjectID );
		}
		break;
	case MP_CHEAT_TITAN_FUEL_SPELL_MAX_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer )	return;			

			pPlayer->GetTitanManager()->AddCurTitanFuelAsRate(1.f);
			pPlayer->GetTitanManager()->AddCurTitanSpellAsRate(1.f);
		}
		break;
	case MP_CHEAT_ITEMLIMIT_SYN:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			
			CPlayer* pPlayer = NULL;

			if( pmsg->dwData1 == 1 )		// on/off
			{
				BOOL bOnOff = FALSE;
				if( pmsg->dwData3 == 1 )
					bOnOff = TRUE;
				if( GAMEEVENTMGR->EventOnOff( pmsg->dwData2, bOnOff ) == TRUE )
				{
					MSG_DWORD3 msg;
					msg.Category = MP_CHEAT;
					msg.Protocol = MP_CHEAT_ITEMLIMIT_ACK;
					msg.dwData1 = 1;
					msg.dwData2 = pmsg->dwData2;
					if( bOnOff == TRUE )
						msg.dwData3 = 1;
					else if( bOnOff == FALSE )
						msg.dwData3 = 0;

					pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
                    if( !pPlayer )	return;

					pPlayer->SendMsg( &msg, sizeof(msg) );
				}
				else
				{
					MSG_DWORD3 msg;
					msg.Category = MP_CHEAT;
					msg.Protocol = MP_CHEAT_ITEMLIMIT_NACK;
					msg.dwData1 = 1;
					msg.dwData2 = pmsg->dwData2;
					if( bOnOff == TRUE )
						msg.dwData3 = 1;
					else if( bOnOff == FALSE )
						msg.dwData3 = 0;

					pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
                    if( !pPlayer )	return;

					pPlayer->SendMsg( &msg, sizeof(msg) );
				}
			}
			else if( pmsg->dwData1 == 2 )	// count
			{
				MSG_DWORD3 msg;
				msg.Category = MP_CHEAT;
				msg.dwData1 = 2;
				msg.dwData2 = pmsg->dwData2;
				msg.dwData3 = pmsg->dwData3;
				if( ITEMLIMITMGR->SetItemLimitCount( pmsg->dwData2, (int)pmsg->dwData3 ) == TRUE )
				{
					msg.Protocol = MP_CHEAT_ITEMLIMIT_ACK;
				}
				else
				{
					msg.Protocol = MP_CHEAT_ITEMLIMIT_NACK;
				}

				pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
                if( !pPlayer )	return;

				pPlayer->SendMsg( &msg, sizeof(msg) );
			}			
		}
		break;
	case MP_CHEAT_AUTONOTE_SETTING_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			
			if( pmsg->dwData1 == 0 )	// on/off
			{
				g_pServerSystem->GetMap()->SetAutoNoteAllow((BOOL)pmsg->dwData2);
			}
			else if( pmsg->dwData1 == 1 )	// replytime
			{
				AUTONOTEMGR->SetReplyTime( pmsg->dwData2 );
			}
			else if( pmsg->dwData1 == 2 )	// replychance
			{
				AUTONOTEMGR->SetReplyChance( (int)pmsg->dwData2 );
			}
			else if( pmsg->dwData1 == 5 )	// 시스템 자체...
			{
				AUTONOTEMGR->SetUseAutoNoet( (BOOL)pmsg->dwData2 );
			}

			MSG_DWORD2 msg;
			msg.Category = MP_CHEAT;
			msg.Protocol = MP_CHEAT_AUTONOTE_SETTING_ACK;
			msg.dwData1 = pmsg->dwData1;
			msg.dwData2 = pmsg->dwData2;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer )	return;

			pPlayer->SendMsg( &msg, sizeof(msg) );
		}
		break;
/*
		//2008. 6. 4. CBH - 악명치 치트
	case MP_CHEAT_BADFAME_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer )	return;

			pPlayer->SetBadFame(pmsg->dwData);
		}
		break;*/
		// magi82(34)
	//case MP_CHEAT_GTOURNAMENT_RESET_SYN:
	//	{
	//		MSGBASE* pmsg = (MSGBASE*)pMsg;
	//		CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
	//		if( !pPlayer )
	//			return;

	//		if( GAMERESRCMNGR->GetLoadMapNum() == Tournament )
	//		{
	//			GTMGR->Reset();

	//			MSGBASE msg;
 //               msg.Category = MP_CHEAT;
	//			msg.Protocol = MP_CHEAT_GTOURNAMENT_RESET_ACK;
	//			msg.dwObjectID = pmsg->dwObjectID;
	//			pPlayer->SendMsg(&msg, sizeof(msg));
	//		}
	//	}
	//	break;
	default:
		break;
	}
}
void MP_QUICKMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	QUICKMNGR->NetworkMsgParse(pTempMsg->Protocol, pMsg);
/*
		
		switch(pTempMsg->Protocol)
		{
		case MP_QUICK_ADD_SYN:
			{
				MSG_QUICK_ADD_SYN * msg = (MSG_QUICK_ADD_SYN *)pMsg;
				CPlayer * player = (CPlayer *)g_pUserTable->FindUser(msg->dwObjectID);
				if(!player) return;
				QUICKMNGR->AddQuickItem(player, msg);
			}
			break;
		case MP_QUICK_USE_SYN:
			{
				MSG_QUICK_USE_SYN * msg = (MSG_QUICK_USE_SYN *)pMsg;
				CPlayer * player = (CPlayer *)g_pUserTable->FindUser(msg->dwObjectID);
				if(!player) return;
				player->UseQuickItem(msg->wQuickRelPosition);
			}
			break;
		case MP_QUICK_MOVE_SYN:
			{
				MSG_QUICK_MOVE_SYN * msg = (MSG_QUICK_MOVE_SYN *)pMsg;
				CPlayer * player = (CPlayer *)g_pUserTable->FindUser(msg->dwObjectID);
				if(!player) return;
				QUICKMNGR->MoveQuickItem(player, msg);
			}
			break;
		case MP_QUICK_REM_SYN:
			{
				MSG_QUICK_REM_SYN * msg = (MSG_QUICK_REM_SYN *)pMsg;
				CPlayer * player = (CPlayer *)g_pUserTable->FindUser(msg->dwObjectID);
				if(!player) return;
				
				QUICKMNGR->RemQuickItem(player, msg);
			}
			break;
		}*/
	
}

void MP_PARTYMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol) 
	{
	case MP_PARTY_ADD_SYN: 
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
//			PARTYMGR->AddParty(pmsg->dwData2,pmsg->dwObjectID,pmsg->dwData1); //include DB
			PARTYMGR->AddPartyInvite(pmsg->dwObjectID, pmsg->dwData1);
		}
		break;
	case MP_PARTY_DEL_SYN: 
		{
			MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
			PARTYMGR->DelMemberSyn(pmsg->dwObjectID, pmsg->dwData);
		}
		break;
	case MP_PARTY_CREATE_SYN: 
		{
			//2008. 5. 21. CBH - 방파 옵션 추가 관련 처리 수정
			/*
			MSG_BYTE * pmsg = (MSG_BYTE*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
			return;
			PARTYMGR->CreatePartyQuery(pPlayer, pmsg->bData); //include DB
			*/
			PARTY_ADDOPTION * pmsg = (PARTY_ADDOPTION*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
				return;
			PARTYMGR->CreatePartyQuery(pPlayer, pmsg); //include DB
		}
		break;
	case MP_PARTY_CHANGEMASTER_SYN: 
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			PARTYMGR->ChangeMasterParty(pmsg->dwData2,pmsg->dwObjectID,pmsg->dwData1); 
		}
		break;
	case MP_PARTY_BREAKUP_SYN: 
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			PARTYMGR->BreakupParty(pmsg->dwData,pmsg->dwObjectID); //include DB
		}
		break; 
	case MP_PARTY_BAN_SYN: 
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			PARTYMGR->BanParty(pmsg->dwData2,pmsg->dwObjectID,pmsg->dwData1); 
		}
		break;
	case MP_PARTY_INVITE_ACCEPT_SYN: 
		{
			MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
			PARTYMGR->AddMember(pmsg->dwData, pmsg->dwObjectID);
		}
		break;
	case MP_PARTY_INVITE_DENY_SYN: 
		{
			MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
			PARTYMGR->AddMemberNack(pmsg->dwData, pmsg->dwObjectID);
		}
		break;
	case MP_PARTY_NOTIFYMEMBER_LOGIN_TO_MAPSERVER:
		{
			SEND_CHANGE_PARTY_MEMBER_INFO* pmsg = (SEND_CHANGE_PARTY_MEMBER_INFO*)pMsg;
			PARTYMGR->NotifyUserLogIn(pmsg);
		}
		break;
	case MP_PARTY_NOTIFYMEMBER_LOGOUT_TO_MAPSERVER:
		{
			SEND_CHANGE_PARTY_MEMBER_INFO* pmsg = (SEND_CHANGE_PARTY_MEMBER_INFO*)pMsg;

			PARTYMGR->NotifyUserLogOut(pmsg);
		}
		break;
	case MP_PARTY_NOTIFYADD_TO_MAPSERVER:
		{
			SEND_CHANGE_PARTY_MEMBER_INFO* pmsg = (SEND_CHANGE_PARTY_MEMBER_INFO*)pMsg;
			PARTYMGR->NotifyAddParty(pmsg); 
		}
		break;
	case MP_PARTY_NOTIFYDELETE_TO_MAPSERVER: 
		{
			SEND_CHANGE_PARTY_MEMBER_INFO* pmsg = (SEND_CHANGE_PARTY_MEMBER_INFO*)pMsg;
			PARTYMGR->NotifyDelParty(pmsg);
		}
		break;
	case MP_PARTY_NOTIFYBAN_TO_MAPSERVER:
		{
			SEND_CHANGE_PARTY_MEMBER_INFO* pmsg = (SEND_CHANGE_PARTY_MEMBER_INFO*)pMsg;
			PARTYMGR->NotifyBanParty(pmsg); 
		}
		break;
	case MP_PARTY_NOTIFYCHANGEMASTER_TO_MAPSERVER:
		{
			SEND_CHANGE_PARTY_MEMBER_INFO* pmsg = (SEND_CHANGE_PARTY_MEMBER_INFO*)pMsg;
			PARTYMGR->NotifyChangeMasterParty(pmsg); 
		}
		break;
	case MP_PARTY_NOTIFYBREAKUP_TO_MAPSERVER:
		{
			SEND_CHANGE_PARTY_MEMBER_INFO* pmsg = (SEND_CHANGE_PARTY_MEMBER_INFO*)pMsg;
			PARTYMGR->NotifyBreakupParty(pmsg); 
		}
		break;
	case MP_PARTY_SYNDELETE_TO_MAPSERVER: 
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			PARTYMGR->NotifyDelSyn(pmsg);			
		}
		break;
		/*
	case MP_PARTY_NOTIFYCREATE_TO_MAPSERVER:
		{
		MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
		PARTYMGR->NotifyCreateParty(pmsg->dwData);
		}
		break;
		*/
	case MP_PARTY_NOTIFYCREATE_TO_MAPSERVER:	//2008. 6. 3. CBH - 수정
		{
			PARTY_INFO* pmsg = (PARTY_INFO*)pMsg;
			PARTYMGR->NotifyCreateParty(pmsg);
		}
		break;
	case MP_PARTY_NOTIFYMEMBER_LOGINMSG:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			PARTYMGR->NotifyMemberLoginMsg(pmsg->dwData, pmsg->dwObjectID);
		}
		break;
	case MP_PARTY_NOTIFYMEMBER_LEVEL:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			PARTYMGR->DoMemberLevelUp(pmsg->dwData1, pmsg->dwData2, (LEVELTYPE)pmsg->dwData3);
		}
		break;		
		//2008. 5. 23. CBH - 방파장에게 파티신청
	case MP_PARTY_MASTERTOREQUEST_SYN:
		{
			MSG_NAME_DWORD2* pmsg = (MSG_NAME_DWORD2*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData2 );
			if( !pPlayer )
			{
				PARTYMGR->MasterToPartyRequestErr(pmsg->dwObjectID, eErr_Request_NotMaster);
				return;
			}
			
			PARTYMGR->MasterToPartyRequest(pPlayer, pmsg);
		}
		break;		
	case MP_PARTY_REQUEST_CONSENT_SYN:	//파티 수락 처리
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData1 );
			if( !pPlayer )	return;

			CParty* pParty = PARTYMGR->GetParty(pmsg->dwData2);
			if(pParty == NULL)
				return;
			
			//수락 결정 카운트 시간 초기화
			pParty->SetRequestPlayerID(0);
			pParty->InitRequestTime();

			MSG_DWORD2 msg;
			msg.Category	= MP_PARTY;
			msg.Protocol	= MP_PARTY_REQUEST_CONSENT_ACK;
			msg.dwObjectID	= pmsg->dwObjectID;
			msg.dwData1 = pmsg->dwData1;
			msg.dwData2 = pmsg->dwData2;
			g_Network.Send2AgentServer( (char*)&msg, sizeof(msg) );
		}
		break;
	case MP_PARTY_REQUEST_CONSENT_ACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer )	return;

			CParty* pParty = PARTYMGR->GetParty(pmsg->dwData2);
			if(pParty == NULL)
			{
				PARTYMGR->MasterToPartyRequestErr(pPlayer->GetID(), eErr_Request_NotParty);
				return;
			}

			if(pPlayer->GetPartyIdx() != 0)
			{
				PARTYMGR->MasterToPartyRequestErr(pPlayer->GetID(), eErr_Request_PartyExistence);
				return;
			}

			PartyAddMember(pParty->GetPartyIdx(),pPlayer->GetID()); //DB update
			PARTYMGR->AddMemberResult(pParty, pPlayer);
		}
		break;
	case MP_PARTY_REQUEST_REFUSAL_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwData1 );
			if( !pPlayer )	return;

			CParty* pParty = PARTYMGR->GetParty(pmsg->dwData2);
			if(pParty == NULL)
				return;
			
			//수락 결정 카운트 시간 초기화
			pParty->SetRequestPlayerID(0);
			pParty->InitRequestTime();

			MSG_DWORD msg;
			msg.Category	= MP_PARTY;
			msg.Protocol	= MP_PARTY_REQUEST_REFUSAL_ACK;
			msg.dwObjectID	= pmsg->dwObjectID;
			msg.dwData = pmsg->dwData1;
			g_Network.Send2AgentServer( (char*)&msg, sizeof(msg) );
		}
		break;
	case MP_PARTY_NOTIFY_INFO:	//파티 정보 셋팅
		{
			PARTY_INFO* pPartyInfo = (PARTY_INFO*)pMsg;

			PARTYMGR->NotifyPartyInfo(pPartyInfo);
		}
		break;
	case MP_PARTY_MATCHING_INFO:	//파티매칭 리스트 정보 요청
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;

			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer )	return;

			PARTYMGR->PartyMatchingInfo(pPlayer, pmsg);
		}
		break;
	} 
}

void MP_UNGIJOSIKMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;	
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pTempMsg->dwObjectID);
	
	if(pPlayer == NULL)
		return;
	
	//SW070127 타이탄
	if( pPlayer->InTitan() )
	{
		ASSERT(0);
		return;
	}

	switch(pTempMsg->Protocol)
	{
	case MP_UNGIJOSIK_START:
		UNGIMGR->UngijosikMode(pPlayer, TRUE);
		break;

	case MP_UNGIJOSIK_END:
		UNGIMGR->UngijosikMode(pPlayer, FALSE);
		break;
	}
}

void MP_PEACEWARMODEMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg	= (MSGBASE*)pMsg;
	CPlayer* pPlayer	= (CPlayer *)g_pUserTable->FindUser(pTempMsg->dwObjectID);
	
	if(pPlayer == NULL)
		return;

	switch(pTempMsg->Protocol) 
	{
	case MP_PEACEWARMODE_PEACE: 
		{
			PWMODMGR_OBJ->Peace_WarMode(pPlayer, TRUE);
		}
	break;

	case MP_PEACEWARMODE_WAR:
		{
			PWMODMGR_OBJ->Peace_WarMode(pPlayer, FALSE);
		}
		break;
	}
}

void MP_TACTICMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_TACTIC_START_SYN:
		{
			SEND_TACTIC_START* pmsg = (SEND_TACTIC_START*)pMsg;
			CPlayer* pPlayer	= (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
				return;

			TACTICMGR->OnTacticStartReceived(pPlayer, pmsg);

		}
		break;
	case MP_TACTIC_JOIN_SYN:
		{
			SEND_TACTIC_JOIN* pmsg = (SEND_TACTIC_JOIN*)pMsg;
			CPlayer* pPlayer	= (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
				return;

			TACTICMGR->OnTacticJoinReceived(pPlayer, pmsg);

		}
		break;
	}
}
/*
void MP_MUNPAMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_MUNPA_LOAD_MUNPALIST_SYN:
		{
			SEND_MUNPALIST_SYN *msg = (SEND_MUNPALIST_SYN*)pMsg;
			MUNPAMGR->LoadMunpaListSyn(msg);
		}
		break;
	case MP_MUNPA_SEARCHMUNPA_SYN:
		{
			MUNPA_SEARCH_SYN* msg = (MUNPA_SEARCH_SYN*)pMsg;
			MUNPAMGR->SearchMunpa(msg->dwObjectID, msg->SearchKey, msg->GotoPage);
		}
		break;
	case MP_MUNPA_CREATE_SYN:
		{
			MUNPA_CREATE_INFO* msg = (MUNPA_CREATE_INFO*)pMsg;
			MUNPAMGR->CreateMunpa(msg);
		}
		break;
	case MP_MUNPA_BREAKUP_SYN:
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			MUNPAMGR->BreakUpMunpa(msg);
		}
		break;
	case MP_MUNPA_CHANGERANK_SYN:
		{
			SEND_MUNPAMEMBER_RANK_CHANGE* msg = (SEND_MUNPAMEMBER_RANK_CHANGE*)pMsg;
			MUNPAMGR->ChangeRank(msg->dwObjectID, msg->TargetID, msg->Rank, msg->MunpaID);
		}
		break;
	case MP_MUNPA_BAN_SYN:
		{
			SEND_MUNPACCEPT_INFO* msg = (SEND_MUNPACCEPT_INFO*)pMsg;
			MUNPAMGR->BanMember(msg->dwObjectID, msg->MunpaID, msg->TargetID, msg->bLast);
		}
		break;

	case MP_MUNPA_SECEDE_SYN:
		{
			MSGBASE* msg = (MSGBASE*)pMsg;
			MUNPAMGR->SecedeMunpa(msg->dwObjectID);
		}
		break;
	case MP_MUNPA_ADDMEMBER_SYN:
		{
			SEND_MUNPACCEPT_INFO* msg = (SEND_MUNPACCEPT_INFO*)pMsg;
			MUNPAMGR->AcceptMember(msg->dwObjectID, msg->MunpaID, msg->TargetID, msg->bLast);
		}
		break;
	case MP_MUNPA_DENYMEMBER_SYN:
		{
			SEND_MUNPACCEPT_INFO* msg = (SEND_MUNPACCEPT_INFO*)pMsg;
			MUNPAMGR->DenyMember(msg->dwObjectID, msg->MunpaID, msg->TargetID, msg->bLast);
		}
		break;
	case MP_MUNPA_JOINMEMBER_SYN:
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			MUNPAMGR->JoinMember(msg->dwObjectID, msg->dwData);
		}
		break;
	case MP_MUNPA_JOINMEMBERCANCEL_SYN:
		{
			MSGBASE* msg = (MSGBASE*)pMsg;
			MUNPAMGR->JoinMemberCancel(msg->dwObjectID);
		}
		break;
	case MP_MUNPA_MUNPAHOME_SYN:
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			MUNPAMGR->MunpaHomeInfoSyn(msg->dwObjectID, msg->dwData);
		}
		break;
	case MP_MUNPA_REGULARMEMBERS_INFO_SYN:
		{
			MSG_DWORD2* msg = (MSG_DWORD2*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(msg->dwObjectID);
			if(pPlayer == NULL)
				return;

			MUNPAMGR->RegularMembersInfo(pPlayer, msg->dwData1, msg->dwData2);
		}
		break;
	case MP_MUNPA_JOINMEMBERS_INFO_SYN:
		{
			MSG_DWORD2* msg = (MSG_DWORD2*)pMsg;
			MUNPAMGR->JoinMembersInfo(msg->dwObjectID, msg->dwData1, msg->dwData2);
		}
		break;
	case MP_MUNPA_BOARD_INFO_SYN:
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			MUNPAMGR->MunpaBoardInfoSyn(msg->dwObjectID, msg->dwData);
		}
		break;
	case MP_MUNPA_CREATEBOARD_SYN:
		{
			SEND_MUNPA_BOARD_RANK_INFO* msg = (SEND_MUNPA_BOARD_RANK_INFO*)pMsg;
			MUNPAMGR->CreateBoard(msg);
		}
		break;
	case MP_MUNPA_DELETEBOARD_SYN:
		{
			SEND_MUNPACCEPT_INFO* msg = (SEND_MUNPACCEPT_INFO*)pMsg;
			MUNPAMGR->DeleteBoard(msg);
		}
		break;
	case MP_MUNPA_BOARDRANK_INFO_SYN:
		{
			MSG_DWORD* msg = (MSG_DWORD*)pMsg;
			MUNPAMGR->BoardRankInfo(msg->dwObjectID, msg->dwData);
		}
		break;
	case MP_MUNPA_BOARD_MODIFYRANK_INFO_SYN:
		{
			SEND_MUNPA_BOARD_RANK_INFO* msg = (SEND_MUNPA_BOARD_RANK_INFO*)pMsg;
			MUNPAMGR->BoardModifyRank(msg);
		}
		break;
	case MP_MUNPA_BOARD_SUBJECTLIST_SYN:
		{
			MSG_DWORD3* msg = (MSG_DWORD3*)pMsg;
			MUNPAMGR->BoardSubjectListSyn(msg);
		}
		break;
	case MP_MUNPA_BOARD_WRITE_SYN:
		{
			SEND_BOARD_CONTENT_INFO* msg = (SEND_BOARD_CONTENT_INFO*)pMsg;
			MUNPAMGR->BoardWriteSyn(msg);
		}
		break;
	case MP_MUNPA_BOARD_MODIFY_SYN:
		{
			SEND_BOARD_CONTENT_INFO* msg = (SEND_BOARD_CONTENT_INFO*)pMsg;
			MUNPAMGR->BoardModifySyn(msg);
		}
		break;
	case MP_MUNPA_BOARD_CONTENTS_SYN:
		{
			MSG_DWORD3* msg = (MSG_DWORD3*)pMsg;
			MUNPAMGR->BoardContentsSyn(msg);
		}
		break;
	case MP_MUNPA_BOARD_DELETE_SYN:
		{
			MSG_DWORD2* msg = (MSG_DWORD2*)pMsg;
			MUNPAMGR->BoardDeleteContentsSyn(msg);
		}
		break;
	case MP_MUNPA_MODIFY_INTROMSG_SYN:
		{
			MUNPA_MODIFY_INTRO* msg = (MUNPA_MODIFY_INTRO*)pMsg;
			MUNPAMGR->ModifyIntroMsg(msg);
		}
		break;
	case MP_MUNPA_PUTIN_MONEY_SYN:
		{
			MSG_DWORD * msg = (MSG_DWORD *)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(msg->dwObjectID);
			if(!pPlayer)
				return;
			MUNPAMGR->PutInMoney(pPlayer, msg->dwData);
		}
		break;
	case MP_MUNPA_PUTOUT_MONEY_SYN:
		{
			MSG_DWORD * msg = (MSG_DWORD *)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(msg->dwObjectID);
			if(!pPlayer)
				return;
			MUNPAMGR->PutOutMoney(pPlayer, msg->dwData);
		}
		break;
	//notify to mapserver
	case MP_MUNPA_NOTIFY_CHANGERANK_TO_MAPSERVER:
		{
			SEND_MUNPAMEMBER_RANK_CHANGE_ACK* msg = (SEND_MUNPAMEMBER_RANK_CHANGE_ACK*)pMsg;
			MUNPAMGR->NotifyChangeRank(msg->dwObjectID, msg->MunpaID, msg->Rank, msg->Fame);
		}
		break;
	case MP_MUNPA_NOTIFYBAN_TO_MAPSERVER:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			MUNPAMGR->NotifyBanMember(pmsg->dwObjectID, pmsg->dwData1, pmsg->dwData2);
		}
		break;
	case MP_MUNPA_NOTIFY_ADDMEMBER_TO_MAPSERVER:
		{
			SEND_MUNPA_ACCEPT_TO_MAP* pmsg = (SEND_MUNPA_ACCEPT_TO_MAP*)pMsg;
			MUNPAMGR->NotifyAcceptMember(pmsg);
		}
		break;
	case MP_MUNPA_NOTIFYBREAKUP_TO_MAPSERVER:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			MUNPAMGR->NotifyBreakupMunpa(pmsg->dwObjectID,pmsg->dwData1, pmsg->dwData2);
		}
		break;
	case MP_MUNPA_NOTIFY_DENYMEMBER_TO_MAPSERVER:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			MUNPAMGR->NotifyDenyMember(pmsg);
		}
		break;
	}
}
*/

void MP_PYOGUKMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	PYOGUKMGR->NetworkMsgParse(pmsg->Protocol,pMsg);
}

void MP_WANTEDMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	WANTEDMGR->NetworkMsgParse(pmsg->Protocol,pMsg);
}

void MP_JOURNALMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	JOURNALMGR->NetworkMsgParse(pmsg->Protocol,pMsg);
}

void MP_SURYUNMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	SURYUNMGR->NetworkMsgParse(dwConnectionIndex, pmsg->Protocol,pMsg);
}

void MP_SkillMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	SKILLMGR->NetworkMsgParse(pmsg->Protocol,pMsg);
}

void MP_EXCHANGEMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	EXCHANGEMGR->NetworkMsgParse( pmsg->Protocol, pMsg );
}

void MP_STREETSTALLMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	STREETSTALLMGR->NetworkMsgParse( pmsg->Protocol, pMsg );
}

void MP_BattleMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	BATTLESYSTEM->NetworkMsgParse( pmsg->Protocol, pmsg );
}

void MP_REVIVEMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	if( EVENTMAPMGR->IsEventMap() )	return;

	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_CHAR_REVIVE_PRESENTSPOT_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)
				return;
			pPlayer->RevivePresentSpot();
		}
		break;
	case MP_CHAR_REVIVE_LOGINSPOT_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(!pPlayer)
				return;
			pPlayer->ReviveLogIn();
		}
		break;
	case MP_CHAR_REVIVE_VILLAGESPOT_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser( pmsg->dwObjectID );
			if( !pPlayer )			return;

			pPlayer->ReviveVillage();
		}
		break;
	}
}


void MP_OPTIONMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	OPTIONMGR->NetworkMsgParse( pmsg->Protocol, pMsg );
}

void MP_NPCMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_NPC_SPEECH_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			CNpc* pNpc = (CNpc*)g_pUserTable->FindUser(pmsg->dwData1);

			MSG_DWORD2 msg = *pmsg;
			msg.Protocol = MP_NPC_SPEECH_ACK;

			// error Check
			if(!pPlayer) return;

			BYTE BattleKind = pPlayer->GetBattle()->GetBattleKind();
			if( BattleKind != eBATTLE_KIND_NONE && BattleKind != eBATTLE_KIND_SIEGEWAR )
				goto SPEECH_NACK;

			// checkhacknpc
			if(pNpc == 0)
			{
				if( CheckHackNpc( pPlayer, (WORD)pmsg->dwData2 ) == FALSE )
				{
					goto SPEECH_NACK;
				}
			}
			else
			{
				VECTOR3 ObjectPos	= *CCharMove::GetPosition(pPlayer);
				VECTOR3 TObjectPos	= *CCharMove::GetPosition(pNpc);
				DWORD	Distance	= (DWORD)CalcDistanceXZ( &ObjectPos, &TObjectPos );
				if(Distance > 3000.0f)	//1000->3000
				{
					goto SPEECH_NACK;
				}
				if(pNpc->GetNpcJob() == BOMUL_ROLE)
				{
					// 06.12.21 - RaMa : 클릭하면 안되는 보물상자.
					VECTOR3 pos;
					pNpc->GetPosition( &pos );
					if( pos.x == 0 && pos.z == 0 )
					{
						MSG_DWORD NackMsg;
						SetProtocol( &NackMsg, MP_NPC, MP_NPC_SPEECH_NACK );
						NackMsg.dwObjectID = pmsg->dwObjectID;
						NackMsg.dwData = 0;
						pPlayer->SendMsg( &NackMsg, sizeof(NackMsg) );
						return;
					}

					DWORD PlayerID = pNpc->GetFirstTalkCharacterID();
					if(PlayerID)
					{
						CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(PlayerID);
						if(pPlayer == 0) // maybe?
						{
							ASSERT(0); 
							pNpc->Die(NULL);
						}

						goto SPEECH_NACK;
					}
				}
				if(pNpc->GetState() == eObjectState_Die)
					goto SPEECH_NACK;
				if(pNpc->GetNpcJob() == BOMUL_ROLE)
					pNpc->SetFirstTalkCharacterID(pmsg->dwObjectID);
			}

			// State Check
			if( !OBJECTSTATEMGR_OBJ->StartObjectState(pPlayer, eObjectState_Deal, 0) ) goto SPEECH_NACK;

			pPlayer->SendMsg(&msg, sizeof(msg));
					
			return;

SPEECH_NACK:
			MSG_DWORD NackMsg;
			NackMsg.Category = MP_NPC;
			NackMsg.Protocol = MP_NPC_SPEECH_NACK;
			NackMsg.dwObjectID = pmsg->dwObjectID;
			NackMsg.dwData = pmsg->dwData2;
			pPlayer->SendMsg(&NackMsg, sizeof(NackMsg));
		}
		break;
	case MP_NPC_CLOSEBOMUL_SYN:
		{
			MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == 0)
				return;
			CNpc* pNpc = (CNpc*)g_pUserTable->FindUser(pmsg->dwData); //pmsg->dwData : NpcObjectIdx

			// 검사
			if((pNpc == 0) || (pNpc->GetFirstTalkCharacterID() != pPlayer->GetID()))
				return;
			else
			{				
				pNpc->SetFirstTalkCharacterID(0);
			}			
		}
		break;
	case MP_NPC_OPENBOMUL_SYN:
		{
			MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == 0)
				return;
			CNpc* pNpc = (CNpc*)g_pUserTable->FindUser(pmsg->dwData); //pmsg->dwData : NpcObjectIdx
			if((pNpc == 0) || (pNpc->GetFirstTalkCharacterID() != pPlayer->GetID()))
			{
				ASSERT(0);
				return;
			}
			pNpc->DoDie(pPlayer);
		}
		break;
	case MP_NPC_DOJOB_SYN:
		{
			MSG_DWORD * pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == 0)
				return;
			CNpc* pNpc = (CNpc*)g_pUserTable->FindUser(pmsg->dwData); //pmsg->dwData : NpcObjectIdx
			if(pNpc == 0)
				return;
			pNpc->DoJob(pPlayer, dwConnectionIndex);			
		}
		break;
	}
}

void MP_MurimnetMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	//////////////////////////////////////////////////////////////////////////
	// 일반 맵서버에서 받는 부분
	case MP_MURIMNET_CHANGETOMURIMNET_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
				return;			
			
			g_pServerSystem->RemovePlayer(pTempMsg->dwObjectID);

			MSGBASE msg;
			msg.Category = MP_MURIMNET;
			msg.Protocol = MP_MURIMNET_CHANGETOMURIMNET_ACK;
			msg.dwObjectID = pmsg->dwObjectID;
			
			g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 베틀 서버에서 받는 부분
	case MP_MURIMNET_PLAYROOM_INFO_SYN:
		{
/*			MSG_MNPLAYROOM_INFO* pmsg = (MSG_MNPLAYROOM_INFO*)pMsg;
			
			MSG_BATTLESETTING_SYN_MUNPAFIELD binfo;
			memset(&binfo,0,sizeof(MSG_BATTLESETTING_SYN_MUNPAFIELD));
			binfo.BattleID = pmsg->nIndex;
			ASSERT(binfo.BattleID != 0);
			binfo.BattleKind = pmsg->nGameKind;
			for(int n=0;n<pmsg->nTotalPlayerNum;++n)
			{
				BYTE Team = pmsg->PlayerInfo[n].cbTeam;
				if(Team == 2)	// 옵저버
				{

				}
				else
				{
					binfo.TeamInfo[Team].AddTeamMember(pmsg->PlayerInfo[n].dwObjectID);
				}
			}
			if(BATTLESYSTEM->CreateBattle(&binfo) == FALSE)
			{	// 베틀 생성 실패
				ASSERT(0);
			}

			// 성공 메세지 무림넷에 보낸다.
			pmsg->Protocol = MP_MURIMNET_PLAYROOM_INFO_ACK;
			PACKEDDATA_OBJ->SendToMapServer(dwConnectionIndex,99,pmsg,dwLength);
*/	
			MSG_MNPLAYROOM_INFO* pmsg = (MSG_MNPLAYROOM_INFO*)pMsg;
			
			MSG_BATTLESETTING_SYN_MURIMFIELD bInfo;
			memset( &bInfo, 0, sizeof(MSG_BATTLESETTING_SYN_MURIMFIELD) );
			bInfo.BattleID = pmsg->nIndex;
			ASSERT(bInfo.BattleID != 0);
			bInfo.BattleKind = eBATTLE_KIND_MURIMFIELD;
			bInfo.MurimBattleInfo.wMurimBattleKind = (WORD)pmsg->nGameKind;
			for( int n = 0; n < pmsg->nTotalPlayerNum; ++n )
			{
				BYTE Team = pmsg->PlayerInfo[n].cbTeam;
				if(Team == 2)	// 옵저버
				{
				}
				else
				{
					bInfo.TeamInfo[Team].AddTeamMember( pmsg->PlayerInfo[n].dwObjectID );
				}
			}
			if( BATTLESYSTEM->CreateBattle( &bInfo, pmsg->wMapNum ) == FALSE )
			{	// 베틀 생성 실패
				ASSERT(0);
			}

			// 성공 메세지 무림넷에 보낸다.
			pmsg->Protocol = MP_MURIMNET_PLAYROOM_INFO_ACK;
			PACKEDDATA_OBJ->SendToMapServer( dwConnectionIndex, 99, pmsg, (WORD)dwLength );
		}
		break;
		
	case MP_MURIMNET_RETURNTOMURIMNET_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
				return;

			MSGBASE msg;
			msg.Category = MP_MURIMNET;
			msg.Protocol = MP_MURIMNET_RETURNTOMURIMNET_ACK;
			pPlayer->SendMsg(&msg,sizeof(msg));

			g_pServerSystem->RemovePlayer(pmsg->dwObjectID);
		}
		break;
	//////////////////////////////////////////////////////////////////////////

		
	}
}

void MP_QUESTMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	QUESTMGR->NetworkMsgParse( pTempMsg->Protocol, pMsg );

	switch(pTempMsg->Protocol)
	{
	case MP_QUEST_CHANGESTATE:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pPlayer == NULL)
			{
//				char buff[256] = {0,};
//				sprintf(buff, "여기뜨면 법석한테 연락부탁드려염 [Client ID : %d]", pmsg->dwObjectID);
//				ASSERTMSG(0, buff);
				return;
			}		

			pPlayer->SetQuestState(pmsg->dwData1, pmsg->dwData2);
			
		}
		break;
/*		
	case MP_QUEST_TEST_START_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( pPlayer )
			{
				MSG_DWORD msg;
				msg.Category = MP_QUEST;
				msg.Protocol = MP_QUEST_TEST_START_ACK;
				msg.dwObjectID = pmsg->dwObjectID;
				msg.dwData = pmsg->dwData;
				pPlayer->SendMsg( &msg, sizeof(msg) );
				
//				pPlayer->AddQuestScript( pmsg->dwData );
				return;
			}
			else
			{
				return;
			}
		}
		break;
*/
	}

}

void MP_PKMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	PKMGR->NetworkMsgParse( pmsg->Protocol, pMsg );	
}


void MP_HACKCHECKMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	switch( pTempMsg->Protocol )
	{
	case MP_HACKCHECK_BAN_USER:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( pPlayer == NULL ) return;
			
			if( pmsg->dwData )
				InsertSpeedHackCheck( pPlayer->GetID(), pPlayer->GetObjectName(), pmsg->dwData, 0 );
		}
		break;
	}
}

void MP_SOCIETYACTMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	switch( pTempMsg->Protocol )
	{
	case MP_SOCIETYACT_ACT_SYN:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);
			if( pPlayer == NULL ) return;
			
			if( pPlayer->CanSocietyAction( gCurTime ) == FALSE )
				return;

			if( pPlayer->InTitan() )
				return;

			MSG_DWORD_WORD msg;
			msg.Category	= MP_SOCIETYACT;
			msg.Protocol	= MP_SOCIETYACT_ACT;
			msg.dwObjectID	= pPlayer->GetID();
			msg.dwData		= pPlayer->GetID();
			msg.wData		= pmsg->wData;
			
			PACKEDDATA_OBJ->QuickSendExceptObjectSelf( pPlayer, &msg, sizeof(msg) );
			
			//동작을 했으므로 무적상태 풀어줌
			if( pPlayer->GetState() == eObjectState_Immortal )
				OBJECTSTATEMGR_OBJ->EndObjectState( pPlayer, eObjectState_Immortal, 0 );
		}
		break;
	}
}


void MP_GUILDMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	GUILDMGR->NetworkMsgParse(pmsg->Protocol,pMsg);
}

void MP_GUILDFIELDWARMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	GUILDFIELDWARMGR->NetworkMsgParse(pmsg->Protocol,pMsg);
}

void MP_PARTYWARMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	PARTYWARMGR->NetworkMsgParse(pmsg->Protocol, pMsg);
}

void MP_GTOURNAMENTMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	GTMGR->NetworkMsgParse( dwConnectionIndex, pmsg->Protocol, pMsg );
}

void MP_GUILUNIONMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	GUILDUNIONMGR->NetworkMsgParse( pmsg->Protocol, pMsg );
}

void MP_SIEGEWARMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	SIEGEWARMGR->NetworkMsgParse( dwConnectionIndex, pmsg->Protocol, pMsg );
}

void MP_SIEGEWARPROFITMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	SWPROFITMGR->NetworkMsgParse( dwConnectionIndex, pmsg->Protocol, pMsg );
}

// Weather System
void MP_WEATHERMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;

	WEATHERMGR->NetworkMsgParse( pmsg->Protocol, pMsg );
}

void MP_PETMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	CPlayer* pPlayer = (CPlayer*)g_pUserTable->FindUser(pTempMsg->dwObjectID);
	if(!pPlayer) return;

	pPlayer->GetPetManager()->NetworkMsgParse( pTempMsg->Protocol, pMsg );

	/*
	CPet* pPet = pPlayer->GetPetManager()->GetCurSummonPet();
	if(!pPet)
	{
		char buf[256] = {0,};
		sprintf(buf, "PlayerID(%d),that have not SummonPet!", pTempMsg->dwObjectID);
		ASSERTMSG(0, buf);
		return;
	}

	switch(pTempMsg->Protocol)
	{	
	case MP_PET_REST_ON:
		{
			if( FALSE == pPet->IsPetRest() )
			{
				pPet->SetPetRest(TRUE);
				pTempMsg->Protocol = MP_PET_REST_ACK;
			}
			else
			{
				pTempMsg->Protocol = MP_PET_REST_NACK;
				//return sync error
			}
		}
		break;
	case MP_PET_REST_OFF:
		{
			if( TRUE == pPet->IsPetRest() )
			{
				pPet->SetPetRest(FALSE);
				pTempMsg->Protocol = MP_PET_REST_ACK;
			}
			else
			{
				pTempMsg->Protocol = MP_PET_REST_NACK;
				//return sync error
			}
		}
		break;
	case MP_PET_USE_SKILL_SYN:
		{
			//!!!펫 스킬 게이지 검사
			if(pPet->UsePetSkill())
				pTempMsg->Protocol = MP_PET_USE_SKILL_ACK;
			//return MP_PET_USE_SKILL_ACK
		}
		break;
	default:
		ASSERT(0);
		pTempMsg->Protocol = MP_PET_REST_NACK;
		break;
	}

	pPlayer->SendMsg(pTempMsg,sizeof(MSGBASE));
	*/
}

void MP_SURVIVALMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	SVVMODEMGR->NetworkMsgParse(dwConnectionIndex, pmsg->Protocol, pMsg);
}

void MP_BOBUSANGMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	BOBUSANGMGR->NetworkMsgParser(dwConnectionIndex, pmsg->Protocol, pMsg);
}

BOOL CheckHackNpc( CPlayer* pPlayer, WORD wNpcIdx )
{
	STATIC_NPCINFO* pInfo = GAMERESRCMNGR->GetStaticNpcInfo( wNpcIdx );
	if( pInfo == NULL )
	{
		return FALSE;
	}

	if( pInfo->MapNum != g_pServerSystem->GetMapNum() )
		return FALSE;

	VECTOR3 vPos;
	pPlayer->GetPosition( &vPos );
	float dist = CalcDistanceXZ( &vPos, &pInfo->vPos );
	if( dist > 3000.0f )
	{
		ASSERT(0); //TL임시
		return FALSE;
	}

	return TRUE;
}

void MP_TITANMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	CPlayer* pPlayer = (CPlayer *)g_pUserTable->FindUser(pmsg->dwObjectID);

	if(pPlayer == NULL)
		return;

    pPlayer->GetTitanManager()->NetworkMsgParse(dwConnectionIndex, pmsg->Protocol, pMsg);
}

void MP_ITEMLIMITMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	ITEMLIMITMGR->NetworkMsgParser(dwConnectionIndex, pmsg->Protocol, pMsg);
}


void MP_AUTONOTEMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	AUTONOTEMGR->NetworkMsgParse(dwConnectionIndex, pmsg->Protocol, pMsg);
}

void MP_FORTWARMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	FORTWARMGR->NetworkMsgParse(dwConnectionIndex, pmsg->Protocol, pMsg);
}
