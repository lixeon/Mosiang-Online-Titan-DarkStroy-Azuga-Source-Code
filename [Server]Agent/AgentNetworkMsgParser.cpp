
#include "stdafx.h"
#include "AgentNetworkMsgParser.h"
#include "ServerTable.h"
#include "Network.h"
#include "AgentDBMsgParser.h"
#include "MsgTable.h"

#include "FilteringTable.h"
#include "BootManager.h"
#include "MHTimeManager.h"
#include "GMPowerList.h"
#include "ShoutManager.h"
#include "PlustimeMgr.h"

#include "JackpotManager_Agent.h"
#include "BobusangManager_Agent.h"
#include "SkillDalayManager.h"
#include "TrafficLog.h"
#include "PunishManager.h"


#ifdef _HACK_SHIELD_
#include "HackShieldManager.h"
#endif

extern CMemoryPoolTempl<USERINFO> g_UserInfoPool;
extern int g_nServerSetNum;
extern HWND g_hWnd;
extern BOOL g_bCloseWindow;
DWORD g_dwAcceptUser2 = 0;

DWORD g_dwMaxUser = 4000;
#ifdef _NPROTECT_
#include "ggsrv25.h"
#include "NProtectManager.h"

extern CMemoryPoolTempl<CCSAuth2> g_CCSAuth2Pool;
#endif

void MP_MonitorMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_MORNITORMAPSERVER_NOTICESEND_SYN:
		{
			// ¡ÆⓒªAo¡icC¡¿ 
			MSGNOTICE* pmsg = (MSGNOTICE*)pMsg;
			USERINFO * info = NULL;
			TESTMSG msg;
			msg.Category = MP_SIGNAL;
			msg.Protocol = MP_SIGNAL_COMMONUSER;
//			strcpy(msg.Msg, pmsg->Msg);
			SafeStrCpy( msg.Msg, pmsg->Msg, MAX_CHAT_LENGTH + 1 );

			g_pUserTable->SetPositionUserHead();
			while(info = (USERINFO *)g_pUserTable->GetUserData())
			{	
				//g_Network.Send2User(info->dwConnectionIndex, (char*)&msg, sizeof(TESTMSG));
				TESTMSG msgTemp = msg;
				g_Network.Send2User(info->dwConnectionIndex, (char*)&msgTemp, msgTemp.GetMsgLength());	//CHATMSG 040324
			}
			//g_Console.Log(eLogDisplay,4,pmsg->Msg);
		}
		break;
	case MP_MORNITORMAPSERVER_PING_SYN:
		{
			pTempMsg->Protocol = MP_MORNITORMAPSERVER_PING_ACK;
			g_Network.Send2Server(dwConnectionIndex, pMsg, dwLength);
//			g_Console.LOG(4, "ping msg recv...");
		}
		break;
	case MP_MORNITORMAPSERVER_QUERYUSERCOUNT_SYN:
		{
			MSGUSERCOUNT  * pmsg = (MSGUSERCOUNT  *)pMsg;
			pmsg->Protocol = MP_MORNITORMAPSERVER_QUERYUSERCOUNT_ACK;
			pmsg->dwUserCount = g_pUserTable->GetUserCount();
			g_Network.Send2Server(dwConnectionIndex, pMsg, sizeof(MSGUSERCOUNT));
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
		
	case MP_MORNITORMAPSERVER_DISCONNECT_ALLUSER_SYN:
		{
			// ￠￢ⓒ￡￥ic A?Au￠?￠® ￠￥eCN A¡E¨uO C¨ªA|...
			g_pUserTableForUserID->SetPositionHead();
			USERINFO* pInfo;
			while( pInfo = g_pUserTableForUserID->GetData() )
			{
				if( pInfo->dwConnectionIndex != 0 )
				{
					g_pUserTable->OnDisconnectUser(pInfo->dwConnectionIndex);
				}
				else
				{
					LoginCheckDelete(pInfo->dwUserID);
					g_pUserTableForUserID->RemoveUser(pInfo->dwUserID);
#ifdef _NPROTECT_
					if( pInfo->m_pCSA )
					{
						pInfo->m_pCSA->Close();
						g_CCSAuth2Pool.Free(pInfo->m_pCSA);
					}
#endif
					memset( pInfo, 0, sizeof(USERINFO) );	//추가 060414 KES
					g_UserInfoPool.Free(pInfo);
				}
			}

			// A|￠￥e¡¤I Ao￠￢￠c￠￢| ￠￢ⓒ￡￥iI Aⓒø￠￢￠cCⓒ￢￠￥AAo￠?￠® ￠￥eCN A¨uA¨I...
			while( g_DB.AllCleared() == FALSE )
			{
				g_DB.ProcessingDBMessage();
				Sleep(50);
			}

			MSGBASE Msg;
			Msg.Category = MP_MORNITORMAPSERVER;
			Msg.Protocol = MP_MORNITORMAPSERVER_DISCONNECT_ALLUSER_ACK;
			g_Network.Send2Server(dwConnectionIndex, (char*)&Msg, sizeof(Msg));
		}
		break;
	case MP_MORNITORMAPSERVER_SERVEROFF_SYN:
		{
			g_bCloseWindow = TRUE;
		}
		break;

	case MP_MORNITORMAPSERVER_CONNECTED_SERVERLIST_SYN:
		{
			MSG_PWRUP_BOOTLIST Msg;
			memset( &Msg, 0, sizeof(MSG_PWRUP_BOOTLIST) );

			Msg.Category = MP_MORNITORMAPSERVER;
			Msg.Protocol = MP_MORNITORMAPSERVER_CONNECTED_SERVERLIST_ACK;

			SERVERINFO* pInfo = NULL;
			g_pServerTable->SetPositionHead();
			while( pInfo = g_pServerTable->GetNextServer() )
				Msg.AddList( pInfo );

			g_Network.Send2Server( dwConnectionIndex, (char*)&Msg, Msg.GetSize() );
		}
		break;
		
	case MP_MORNITORMAPSERVER_EVENTNOTICE_ON:
		{
			MSG_EVENTNOTIFY_ON* pmsg = (MSG_EVENTNOTIFY_ON*)pMsg;

			g_pServerSystem->SetEventNotifyStr( pmsg->strTitle, pmsg->strContext );
			g_pServerSystem->SetUseEventNotify( TRUE );
			
//			g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, dwLength );
		}
		break;
	case MP_MORNITORMAPSERVER_EVENTNOTICE_OFF:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			
			g_pServerSystem->SetUseEventNotify( FALSE );
//			g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, dwLength );
		}
		break;

/*	case MP_MORNITORMAPSERVER_QUERY_MAXUSER_SYN:
		{
			MSG_DWORD msg;
			msg.Category = MP_MORNITORMAPSERVER;
			msg.Protocol = MP_MORNITORMAPSERVER_QUERY_MAXUSER_ACK;
			msg.dwData = g_dwMaxUser;

			g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;
	case MP_MORNITORMAPSERVER_CHANGE_MAXUSER_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			g_dwMaxUser = pmsg->dwData;
			if( pmsg->dwData > 4000 )
				g_dwMaxUser = 4000;

			g_Console.LOG( 4,"Max User : %d", g_dwMaxUser );

			MSG_DWORD msg;
			msg.Category = MP_MORNITORMAPSERVER;
			msg.Protocol = MP_MORNITORMAPSERVER_CHANGE_MAXUSER_ACK;
			msg.dwData = g_dwMaxUser;

			g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;
		*/
	}
}

void MP_POWERUPMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	BOOTMNGR->NetworkMsgParse(dwConnectionIndex, pMsg, dwLength);
}
void MP_AGENTSERVERMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{/*
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_SERVER_PWRUP:
		{
			MSG_WORD * msg = (MSG_WORD *)pMsg;
			SERVERINFO * info = g_pServerTable->FindServer(msg->wData);//port
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
					msg.mapnum = g_pListCollect->GetLoadAgentNum();
					g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));
				}

				// situation : ￠?￠®AIA?¨¡￠c¡Æ￠® ANA￠c AO¡Æi ￥iⓒ￡¨i¹￠i¼¨¡￠c￠￢￠c¨￢a¨¡￠c¡Æ￠® A¡E¨uOCⓒ￢A¡i￠O¡× usercountA￠´¨￢￠￢ ⓒøN¡¾e
				if(info->wServerKind == DISTRIBUTE_SERVER)
				{
					SERVERINFO * myInfo = g_pServerTable->GetSelfServer();
					MSG_WORD2 msg2;
					msg2.Category = MP_SERVER;
					msg2.Protocol = MP_SERVER_USERCNT;
					msg2.wData1 = myInfo->wPortForServer;
					msg2.wData2 = myInfo->wAgentUserCnt;
					g_Network.Send2Server(dwConnectionIndex, (char *)&msg2, sizeof(msg2));
				}
				else if(info->wServerKind == MAP_SERVER)
				{
					MSGROOT msg;
					msg.Category = MP_SERVER;
					msg.Protocol = MP_SERVER_REGISTMAP_SYN;
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
			msg.mapnum = g_pListCollect->GetLoadAgentNum();
			g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;


		case MP_SERVER_REGISTMAP_ACK:
			{
				REGIST_MAP* recv = (REGIST_MAP*)pMsg;
				if(recv->mapnum != 0)
				{
					g_pServerTable->SetMapRegist(recv->mapnum, recv->mapServerPort);
					SERVERINFO* pserver = g_pServerTable->FindServer(recv->mapServerPort);
					if(pserver)
						pserver->wHaveMapNum = recv->mapnum;
					
					SERVERINFO* serverinfo = g_pServerTable->FindServerForConnectionIndex(dwConnectionIndex);
					
					if(serverinfo)
						if(serverinfo->wPortForServer>MAPSERVER_PORT && serverinfo->wPortForServer < MAXSERVER_PORT)
						{
							MapUserUnRegistLoginMapInfo(serverinfo->wPortForServer);
						}
				}
			}
			break;
		}
	*/
}

void MP_USERCONNMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
//////////////////////////////////////////////////////////////////////////
// these Protocol Come From Distibute Server
	case MP_USERCONN_NOTIFY_USERLOGIN_SYN:
		{
			MSG_DWORD3BYTE2* pmsg = (MSG_DWORD3BYTE2*)pMsg;

			DWORD UserID				= pmsg->dwObjectID;
			DWORD DistAuthKey			= pmsg->dwData1;
			DWORD DistConnectionIndex	= pmsg->dwData2;
			int nTime					= (int)pmsg->dwData3;
			BYTE UserLevel				= pmsg->bData1;
			BYTE bType					= pmsg->bData2;
			
			if(g_bReady == FALSE)
			{
				MSG_DWORD3 msg;
				msg.Category	= MP_USERCONN;
				msg.Protocol	= MP_USERCONN_NOTIFY_USERLOGIN_NACK;
				msg.dwObjectID	= UserID;
				msg.dwData1		= DistAuthKey;
				msg.dwData2		= LOGIN_ERROR_NOAGENTSERVER;
				msg.dwData3		= DistConnectionIndex;
				g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );
				return;
			}

#ifdef _CHINA_LOCAL_
			//china billing
			if( bType == 8 && nTime <= 0 )
			{
				MSG_DWORD3 msg;
				msg.Category	= MP_USERCONN;
				msg.Protocol	= MP_USERCONN_NOTIFY_USERLOGIN_NACK;
				msg.dwObjectID	= UserID;
				msg.dwData1		= DistAuthKey;
				msg.dwData2		= LOGIN_ERROR_NOREMAINTIME;
				msg.dwData3		= DistConnectionIndex;
				g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );
				return;
			}
#endif
			//기존에 이미 이 로그인 아이디가 존재하는지 본다. 이런일이 벌어지면 안됨.
			//어짜피 현재 agent만 체크가 된다. 다른 agent는 어떻게?
			//그러므로, 믿을건 DB상에 LoginCheck밖에 없다! 반드시 정확히 관리해야한다.
			//agent에서는 그냥 프로그램 뻑나지 않고 제대로 돌아가게 하는것이 관건!
			USERINFO* pPreInfo = g_pUserTableForUserID->FindUser(UserID);
			if( pPreInfo )	//기존에 유저가 안지워졌을 경우.(어떤경우일까?)
			{
				char temp[128];
				sprintf( temp, "[ERROR]: NOTIFY_USERLOGIN - OConIdx:%d, OUsrIdx:%d, OCharIdx:%d, NewUsrIdx:%d", pPreInfo->dwConnectionIndex, pPreInfo->dwUserID, pPreInfo->dwCharacterID, UserID );
				g_Console.LOG( 4, temp );
				//접속되어 있으면? 그런경우가 발생하는지 콘솔로그를 보자. OldConIdx: 0 이어야 한다!

				if( pPreInfo->dwConnectionIndex )
				{
					DWORD dwPreConIdx = pPreInfo->dwConnectionIndex;
					OnDisconnectUser( dwPreConIdx );	//먼저 처리할것 하고 접속을 끊어야한다.
					DisconnectUser( dwPreConIdx );
				}
				else
				{
					g_pUserTableForUserID->RemoveUser( UserID );
#ifdef _NPROTECT_
					if( pPreInfo->m_pCSA )
					{
						pPreInfo->m_pCSA->Close();
						g_CCSAuth2Pool.Free(pPreInfo->m_pCSA);
					}
#endif
					memset( pPreInfo, 0, sizeof(USERINFO) );
					g_UserInfoPool.Free( pPreInfo );
				}
			}

			USERINFO* pInfo = g_UserInfoPool.Alloc();
			//만약에 pInfo가 NULL이라면 그냥 뻑을 내는게 낫다. 더이상 에이젼트는 불능상태가 되므로.
			if( pInfo == NULL )
			{	//agent disable.
				MSGBASE msg;
				msg.Category	= MP_SERVER;
				msg.Protocol	= MP_SERVER_AGENTDISABLE;

				g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );

				g_Console.LOG( 4, "Agent Disable : Can't accept users!!" );
				return;
			}

			memset( pInfo, 0, sizeof(USERINFO) );
			
			pInfo->dwUserID						= UserID;
			pInfo->dwLastConnectionCheckTime	= gCurTime;
			pInfo->dwUniqueConnectIdx			= g_pServerSystem->MakeAuthKey();
			pInfo->DistAuthKey					= DistAuthKey;
			pInfo->UserLevel					= UserLevel;
#ifdef _CHINA_LOCAL_
			pInfo->bBillType					= bType;
			pInfo->nRemainTime					= nTime;
			pInfo->dwLastCheckRemainTime		= gCurTime;
#endif

#ifdef _NPROTECT_
			pInfo->m_pCSA = g_CCSAuth2Pool.Alloc();
			pInfo->m_pCSA->Init();
#endif			
			g_pUserTableForUserID->AddUser( pInfo, UserID );
	

			MSG_DWORD3 msg;
			msg.Category	= MP_USERCONN;
			msg.Protocol	= MP_USERCONN_NOTIFY_USERLOGIN_ACK;
			msg.dwObjectID	= UserID;
			msg.dwData1		= DistAuthKey;
			msg.dwData2		= pInfo->dwUniqueConnectIdx;
			msg.dwData3		= DistConnectionIndex;
			
			g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );

			// punishlistload
			PunishListLoad( UserID );
		}
		break;

	case MP_USERCONN_NOTIFYTOAGENT_ALREADYOUT: 
		{	//로그인 접속 요청하고 클라이언트가 나갔다. 그러면 정보를 지워야지.
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			DWORD UserID		= pmsg->dwData1;
			DWORD AgentAuthKey	= pmsg->dwData2;

			USERINFO* pInfo = g_pUserTableForUserID->FindUser( UserID );

			if( pInfo )
			if( pInfo->dwUniqueConnectIdx == AgentAuthKey )
			{
				LoginCheckDelete(UserID);
				g_pUserTableForUserID->RemoveUser(UserID);
#ifdef _NPROTECT_
				if( pInfo->m_pCSA )
				{
					pInfo->m_pCSA->Close();
					g_CCSAuth2Pool.Free(pInfo->m_pCSA);
				}
#endif
				memset( pInfo, 0, sizeof(USERINFO) );
				g_UserInfoPool.Free( pInfo );
			}
		}
		break;
	case MP_USERCONN_NOTIFY_OVERLAPPEDLOGIN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			DWORD UserID = pmsg->dwData;
			USERINFO* pInfo = g_pUserTableForUserID->FindUser(UserID);
			if(pInfo == NULL)
			{
				return;
			}

			MSGBASE msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_OTHERUSER_CONNECTTRY_NOTIFY;
			g_Network.Send2User(pInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
		}
		break;
	case MP_USERCONN_FORCE_DISCONNECT_OVERLAPLOGIN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			DWORD UserID = pmsg->dwObjectID;

			USERINFO* pUserInfo = g_pUserTableForUserID->FindUser(UserID);

			if(pUserInfo == NULL)
			{
				return;
			}
			
			DWORD dwUserConIndex = pUserInfo->dwConnectionIndex;
			
			if(dwUserConIndex)
			{
				MSGBASE msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_DISCONNECTED_BY_OVERLAPLOGIN;
				g_Network.Send2User(dwUserConIndex,(char*)&msg,sizeof(msg));

				//INFORM TO MAPSERVER : FOR NO DELAY DISCONNECTING. : no wait player exit
				if( pUserInfo->dwCharacterID )
				{
					SERVERINFO* pServerInfo = g_pServerTable->FindServerForConnectionIndex(pUserInfo->dwMapServerConnectionIndex);
					if( pServerInfo )
					{
						MSGBASE msgToMap;
						msgToMap.Category = MP_USERCONN;
						msgToMap.Protocol = MP_USERCONN_NOWAITEXITPLAYER;
						msgToMap.dwObjectID	= pUserInfo->dwCharacterID;
						g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&msgToMap, sizeof(msgToMap) );
					}
				}

				OnDisconnectUser( dwUserConIndex );	//for safty
				DisconnectUser( dwUserConIndex );
			}
			else	//커넥션이 없으니 CharcterID가 있을리 없다. 테이블에서만 지워준다.
			{
				LoginCheckDelete(UserID);
				g_pUserTableForUserID->RemoveUser(UserID);
#ifdef _NPROTECT_
				if( pUserInfo->m_pCSA )
				{
					pUserInfo->m_pCSA->Close();
					g_CCSAuth2Pool.Free(pUserInfo->m_pCSA);
				}
#endif
				memset( pUserInfo, 0, sizeof(USERINFO) );
				g_UserInfoPool.Free( pUserInfo );                
			}			
		}
		break;
	case MP_USERCONN_DISCONNECTED_ON_LOGIN:
		{
			// 
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			DWORD UserID = pmsg->dwObjectID;
			DWORD AuthKey = pmsg->dwData;

			USERINFO* pInfo = g_pUserTableForUserID->FindUser(UserID);
			if(pInfo == NULL)
			{
				return;
			}

			if( AuthKey != pInfo->DistAuthKey )
			{
				return;
			}

			LoginCheckDelete(UserID);
			g_pUserTableForUserID->RemoveUser(UserID);
#ifdef _NPROTECT_
			if( pInfo->m_pCSA )
			{
				pInfo->m_pCSA->Close();
				g_CCSAuth2Pool.Free(pInfo->m_pCSA);
			}
#endif
			memset( pInfo, 0, sizeof(USERINFO) );	//추가 060414 KES
			g_UserInfoPool.Free( pInfo );
		}
		break;
//////////////////////////////////////////////////////////////////////////
		

	case MP_USERCONN_DISCONNECT_SYN:
		{
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(!pInfo)
			{
				ASSERT(0);
			}
//			MSGBASE msg;
//			msg.Category = MP_USERCONN;
//			msg.Protocol = MP_USERCONN_DISCONNECT_ACK;
//			g_Network.Send2User(dwConnectionIndex,(char*)&msg,sizeof(msg));
//			DisconnectUser(dwConnectionIndex);
			DisconnectUser(dwConnectionIndex);
		}
		break;
	case MP_USERCONN_CONNECTION_CHECK_OK:
		{
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(!pInfo)
				return;
			pInfo->bConnectionCheckFailed = FALSE;
		}
		break;
	case MP_USERCONN_CHARACTERLIST_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			
			DWORD UserID		= pmsg->dwData1;
			DWORD DistAuthKey	= pmsg->dwData2;

			USERINFO* pInfo = g_pUserTableForUserID->FindUser(UserID);

			if(!pInfo)
			{
				MSGBASE msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_CHARACTERLIST_NACK;
				g_Network.Send2User(dwConnectionIndex,(char*)&msg,sizeof(msg));
				//접속 끊어버리는 것이 낫다.
				OnDisconnectUser( dwConnectionIndex );
				DisconnectUser( dwConnectionIndex );
				return;
			}

			if(pInfo->DistAuthKey != DistAuthKey)	//나가고 다른유저가 왔다.
			{

				MSGBASE msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_CHARACTERLIST_NACK;
				g_Network.Send2User(dwConnectionIndex,(char*)&msg,sizeof(msg));
				//접속 끊어버리는 것이 낫다.
				OnDisconnectUser( dwConnectionIndex );
				DisconnectUser( dwConnectionIndex );
				return;
			}

//---KES Crypt
#ifdef _CRYPTCHECK_ 
			pInfo->crypto.Create();	// key 생성
#endif

			pInfo->dwConnectionIndex = dwConnectionIndex;

			//만약에 이미 접속 되어 있다면.... 어떤이유로?
			//이미 나갔어야 할 유저다. 테이블에 왜 있을까?
			USERINFO* pPreInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if( pPreInfo )
			{
				char temp[128];
				sprintf( temp, "[ERROR]: CHARACTERLIST_SYN - OConIdx:%d, OUsrIdx:%d, OCharIdx:%d, NewUsrIdx:%d", pPreInfo->dwConnectionIndex, pPreInfo->dwUserID, pPreInfo->dwCharacterID, pInfo->dwUserID );
				g_Console.LOG( 4, temp );
				//접속되어 있으면? 그런경우가 발생하는지 콘솔로그를 보자.

				//--그냥 테이블에서만 지워주자.
				g_pUserTable->RemoveUser(dwConnectionIndex);
			}
			//////////////////////////////////////////////////////////////////////////
			
			g_pUserTable->AddUser(pInfo,dwConnectionIndex);

			// billing
#ifdef _CHINA_LOCAL_
			InsertBillingTable( pInfo->dwUserID, pInfo->bBillType, g_pServerSystem->GetServerNum(), g_nServerSetNum );
#endif
			
			UserIDXSendAndCharacterBaseInfo(UserID,pInfo->dwUniqueConnectIdx,dwConnectionIndex);

#ifdef _JAPAN_LOCAL_
			GetCharacterSlotCount( UserID, pInfo->dwUniqueConnectIdx, dwConnectionIndex );
#endif

#ifdef _HK_LOCAL_
			GetCharacterSlotCount( UserID, pInfo->dwUniqueConnectIdx, dwConnectionIndex );
#endif

#ifdef _TL_LOCAL_
			GetCharacterSlotCount( UserID, pInfo->dwUniqueConnectIdx, dwConnectionIndex );
#endif

			++g_dwAcceptUser2;
		}
		break;
	case MP_USERCONN_DIRECTCHARACTERLIST_SYN:			// Aⓒø￠￢?AI ￠￢￠c¨i¹￠i¼¨¡￠c
		{
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(!pInfo)
				return;
			ASSERT(pInfo->dwUserID);
			if(pInfo->dwUserID == 0)
			{
				ASSERTMSG(0, "UserID¡Æ￠® 0AI￠￥U.");
				return;
			}
			
			UserIDXSendAndCharacterBaseInfo(pInfo->dwUserID,pInfo->dwUniqueConnectIdx,dwConnectionIndex);
#ifdef _JAPAN_LOCAL_
			GetCharacterSlotCount( pInfo->dwUserID, pInfo->dwUniqueConnectIdx, dwConnectionIndex );
#endif
#ifdef _HK_LOCAL_
			GetCharacterSlotCount( pInfo->dwUserID, pInfo->dwUniqueConnectIdx, dwConnectionIndex );
#endif
#ifdef _TL_LOCAL_
			GetCharacterSlotCount( pInfo->dwUserID, pInfo->dwUniqueConnectIdx, dwConnectionIndex );
#endif

		}
		break;
	
	case MP_USERCONN_CHARACTERSELECT_SYN:	//클라이언트의 gameloading의 init함수에서 보냄
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if( !pInfo )	return;
			
			CHARSELECTINFO* SelInfoArray = (CHARSELECTINFO*)pInfo->SelectInfoArray;
			DWORD playerID	= pmsg->dwObjectID;
			MAPTYPE mapnum;

			pInfo->wChannel = pmsg->wData;	//채널을 담아둔다.

			for(int i = 0; i < MAX_CHARACTER_NUM; i++)
			{
				if(SelInfoArray[i].dwCharacterID == playerID)
				{
					mapnum = SelInfoArray[i].MapNum;
					break;
				}
				if(i == MAX_CHARACTER_NUM - 1)
				{
					MSGBASE msg;
					msg.Category = MP_USERCONN;
					msg.Protocol = MP_USERCONN_CHARACTERSELECT_NACK;
					g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
					return;
				}
			}
			
			SaveMapChangePointUpdate(playerID, 0);
			UnRegistLoginMapInfo(playerID);

			WORD mapport = g_pServerTable->GetServerPort( eSK_MAP, mapnum);

			if(mapport)
			{
				SERVERINFO* pSInfo = g_pServerTable->FindServer(mapport);
				MSG_BYTE msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_CHARACTERSELECT_ACK;
				msg.bData = (BYTE)mapnum;
				msg.dwObjectID = playerID;	
				g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));

#ifdef _HK_LOCAL_
#ifdef _NPROTECT_
				if( pInfo->bHackToolUser == TRUE )
				{
					char name[MAX_NAME_LENGTH+1] = {0,};
					for( int i = 0 ; i < MAX_CHARACTER_NUM ; ++i )
					{
						if( pInfo->SelectInfoArray[i].dwCharacterID == playerID )
						{
							SafeStrCpy( name, pInfo->SelectInfoArray[i].CharacterName, MAX_NAME_LENGTH+1 );
							break;
						}
					}
					HackToolUser( pInfo->dwUserID, playerID, name );
				}
#endif
#endif

				USERINFO* pPreInfo = g_pUserTableForObjectID->FindUser(playerID);
				if( pPreInfo ) //이미 오브젝트가 있다면..
				{
					char temp[128];
					sprintf( temp, "[ERROR]: CHARACTERSELECT - OConIdx:%d, OUsrIdx:%d, OCharIdx:%d, NewUsrIdx:%d", pPreInfo->dwConnectionIndex, pPreInfo->dwUserID, pPreInfo->dwCharacterID, pInfo->dwUserID );
					g_Console.LOG( 4, temp );

					g_pUserTableForObjectID->RemoveUser(playerID);

					//map에도 들어있나? 들어있으면 지우자.
					if( pPreInfo->dwMapServerConnectionIndex )
					{
						SERVERINFO* pServerInfo = g_pServerTable->FindServerForConnectionIndex(pPreInfo->dwMapServerConnectionIndex);
						if( pServerInfo )
						{
							MSG_DWORD msg;
							msg.Category	= MP_USERCONN;
							msg.Protocol	= MP_USERCONN_GAMEOUT_SYN;	//맵에서 유저를 그냥 없애는 것이다.
							msg.dwObjectID	= pPreInfo->dwCharacterID;
							msg.dwData		= pPreInfo->dwUserID;
							g_Network.Send2Server(pPreInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));
						}
					}
				}
				//////////////////////////////////////////////////////////////////////////
				
				g_pUserTableForObjectID->AddUser(pInfo, playerID);

				pInfo->wUserMapNum = (WORD)mapnum;
				pInfo->dwCharacterID = playerID;
				pInfo->dwMapServerConnectionIndex = pSInfo->dwConnectionIndex;
			}
			else
			{
				MSG_BYTE msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_CHARACTERSELECT_NACK;
				msg.bData = (BYTE)mapnum;
				msg.dwObjectID = playerID;
				g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(MSG_BYTE));
			}
		}
		break;
	case MP_USERCONN_CHANNELINFO_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(!pInfo)
				return;
			CHARSELECTINFO * SelectInfoArray = (CHARSELECTINFO*)pInfo->SelectInfoArray;
			CHARSELECTINFO * pSelectedInfo;
			MSG_DWORD msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_CHANNELINFO_SYN;
			msg.dwObjectID = dwConnectionIndex;
			msg.dwData = pInfo->dwUniqueConnectIdx;
			
			MAPTYPE mapnum;
			for(int i = 0; i < MAX_CHARACTER_NUM; i++)
			{
				if(SelectInfoArray[i].dwCharacterID == pmsg->dwObjectID)
				{
					pSelectedInfo = &SelectInfoArray[i];
					mapnum = pSelectedInfo->MapNum;
					break;
				}
				if(i == MAX_CHARACTER_NUM - 1)	// A?Au¡Æ￠® ¡Æ￠®Ao¡Æi AO￠￥A Aⓒø￠￢?¡Æu ￠￥U￠￢¡I Aⓒø￠￢?AI ¨u¡¾AA￥iE
				{
					MSG_BYTE msg;
					msg.Category	= MP_USERCONN;
					msg.Protocol	= MP_USERCONN_CHANNELINFO_NACK;
					msg.bData		= 1;	//A?AuAⓒø￠￢?Aⓒ￢￠￢ⓒª¨u¡¾AA
					g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
					return;
				}
			}
			WORD mapport = g_pServerTable->GetServerPort( eSK_MAP, mapnum);
			if(mapport)
			{
				SERVERINFO* pSInfo = g_pServerTable->FindServer(mapport);
				g_Network.Send2Server(pSInfo->dwConnectionIndex, (char*)&msg, sizeof(MSG_DWORD));
			}
			else
			{
				MSG_BYTE msg;
				msg.Category	= MP_USERCONN;
				msg.Protocol	= MP_USERCONN_CHANNELINFO_NACK;
				msg.bData		= 0;	//¨u¡ⓒⓒooⓒ÷¡§Aⓒø￠￥U.
				g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
			}			
		}
		break;
	case MP_USERCONN_CHANNELINFO_ACK:
		{
			MSG_CHANNEL_INFO* pmsg = (MSG_CHANNEL_INFO*)pMsg;
			USERINFO* pInfo = g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pInfo == NULL)
				return;
			if(pInfo->dwUniqueConnectIdx != pmsg->dwUniqueIDinAgent)
				return;
			g_Network.Send2User(pmsg->dwObjectID, (char*)pMsg, dwLength);
		}
		break;
	case MP_USERCONN_CHANNELINFO_NACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
			USERINFO* pInfo = g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pInfo == NULL)
				return;
			g_Network.Send2User(pmsg->dwObjectID, (char*)pMsg, dwLength);
		}
		break;
	case MP_USERCONN_GAMEIN_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(!pInfo)
				return;

			MSG_DWORD4 msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_GAMEIN_SYN;
			msg.dwObjectID = pInfo->dwCharacterID;
			msg.dwData1 = pInfo->dwUniqueConnectIdx;

			/*
			if( pInfo->UserLevel < eUSERLEVEL_SUPERUSER )
			{
				msg.dwData2 = pmsg->dwData1;	//클라이언트에서 보낸 채널번호
				pInfo->wChannel = (WORD)pmsg->dwData1;
			}
			else
			{
				msg.dwData2	= pInfo->wChannel;	//저장된 채널번호로 세팅하자.
			}
			*/

			//채널변경시 일반유저는 갱신이 되지 않아서 수정
			msg.dwData2 = pmsg->dwData1;	//클라이언트에서 보낸 채널번호
			pInfo->wChannel = (WORD)pmsg->dwData1;

			msg.dwData3 = pInfo->UserLevel;
			msg.dwData4 = pmsg->dwData2;

			g_Network.Send2Server(pInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));

		}
		break;
	case MP_USERCONN_GAMEIN_NACK:	//맵서버에서 보낸다.
		{
			//접속을 끊어준다.
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pmsg->dwData);
			if( pUserInfo )
				DisconnectUser( pUserInfo->dwConnectionIndex );
		}
		break;

	case MP_USERCONN_GAMEIN_ACK:		
		{			
			SEND_HERO_TOTALINFO* pmsg = (SEND_HERO_TOTALINFO*)pMsg;
			DWORD uniqueID = pmsg->UniqueIDinAgent;
			DWORD CharacterID = pmsg->BaseObjectInfo.dwObjectID;
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(CharacterID);
//			g_Console.Log(eLogDisplay,4,"GameinAck %d",CharacterID);
			if(pUserInfo == NULL)
			{				
				MSGBASE msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_GAMEIN_NACK;
				msg.dwObjectID = CharacterID;
				g_Network.Send2Server(dwConnectionIndex,(char*)&msg,sizeof(msg));
				return;
			}
			if(pUserInfo->dwUniqueConnectIdx != uniqueID)
			{
				MSGBASE msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_GAMEIN_NACK;
				msg.dwObjectID = CharacterID;
				g_Network.Send2Server(dwConnectionIndex,(char*)&msg,sizeof(msg));

				return;
			}
			
			g_Network.Send2User((MSGBASE*)pMsg,dwLength);

			//SW050729
			//jackpot
//COMMENT:JACKPOT	
	
			if(pUserInfo->wUserMapNum)	//맵 접속한 유저이면 !=0
			{
				JACKPOTMGR->SendMsgAddUserTotalMoney( CharacterID );
			}

			//GameInAck를 보낸 후에 호출하자.
			SKILLDELAYMGR->SendSkillDelayMsgToClient( CharacterID );

			MSG_DWORD msgCheck;
			msgCheck.Category = MP_USERCONN;
			msgCheck.Protocol = MP_USERCONN_GAMEIN_OTHERMAP_SYN;
			msgCheck.dwObjectID = CharacterID;

			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, (char*)&msgCheck, sizeof(msgCheck));
		}
		break;
	case MP_USERCONN_CHARACTER_MAKE_SYN:		// Aⓒø￠￢?AI ¡iy¨u¨￢
		{
			CHARACTERMAKEINFO* pmsg = (CHARACTERMAKEINFO*)pMsg;
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(!pInfo)
				return;
			
#ifdef _JAPAN_LOCAL_

			DWORD CharacterCount = 0;			
			for(int i=0; i<MAX_CHARACTER_NUM; ++i)
			{
				if( pInfo->SelectInfoArray[i].dwCharacterID )
					++CharacterCount;
			}

			if( CharacterCount >= GIVEN_CHARACTER_SLOT+pInfo->dwExtraUserSlot )
			{
				MSGBASE msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_CHARACTER_MAKE_NACK;
				g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
				return;
			}

#endif	// _JAPAN_LOCAL_

#ifdef _HK_LOCAL_

			DWORD CharacterCount = 0;			
			for(int i=0; i<MAX_CHARACTER_NUM; ++i)
			{
				if( pInfo->SelectInfoArray[i].dwCharacterID )
					++CharacterCount;
			}

			if( CharacterCount >= GIVEN_CHARACTER_SLOT+pInfo->dwExtraUserSlot )
			{
				MSGBASE msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_CHARACTER_MAKE_NACK;
				g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
				return;
			}

#endif	// _HK_LOCAL_

#ifdef _TL_LOCAL_

			DWORD CharacterCount = 0;			
			for(int i=0; i<MAX_CHARACTER_NUM; ++i)
			{
				if( pInfo->SelectInfoArray[i].dwCharacterID )
					++CharacterCount;
			}

			if( CharacterCount >= GIVEN_CHARACTER_SLOT+pInfo->dwExtraUserSlot )
			{
				MSGBASE msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_CHARACTER_MAKE_NACK;
				g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
				return;
			}

#endif	// _TL_LOCAL_


// 캐릭터 생성 정보 체크 ---------------------------------------------------------
			if( !CheckCharacterMakeInfo( pmsg ) )
			{
				MSGBASE msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_CHARACTER_MAKE_NACK;
				g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
				return;
			}
//---------------------------------------------------------------------------------

			DWORD UserID = pInfo->dwUserID;
			pmsg->UserID = UserID;

			//string overflow방지
			char buf[MAX_NAME_LENGTH+1];
			SafeStrCpy( buf, pmsg->Name, MAX_NAME_LENGTH+1 );

			if( FILTERTABLE->IsUsableName( buf ) &&
				!FILTERTABLE->IsInvalidCharInclude( (unsigned char*)buf ) )
//				!FILTERTABLE->IsInvalidCharacterName( (unsigned char*)buf ) )
			{			
				CreateCharacter(pmsg, g_nServerSetNum, dwConnectionIndex);
			}
			else
			{
				MSGBASE msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_CHARACTER_MAKE_NACK;
				g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
			}
		}
		break;
	case MP_USERCONN_CHARACTER_NAMECHECK_SYN:
		{
			MSG_NAME* pmsg = (MSG_NAME*)pMsg;
			
			//string overflow방지
			char buf[MAX_NAME_LENGTH+1];
			SafeStrCpy( buf, pmsg->Name, MAX_NAME_LENGTH+1 );
			
			if( FILTERTABLE->IsUsableName( buf ) &&
				!FILTERTABLE->IsInvalidCharInclude( (unsigned char*)buf ) )
//				!FILTERTABLE->IsInvalidCharacterName( (unsigned char*)buf ) )
			{
				CharacterNameCheck( buf, dwConnectionIndex);
			}
			else
			{
				MSG_WORD msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_CHARACTER_NAMECHECK_NACK;
				msg.wData = 2;
				g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
			}			
		}
		break;
	case MP_USERCONN_CHARACTER_REMOVE_SYN:		// Aⓒø￠￢?AI ¡ieA|
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			DeleteCharacter(pmsg->dwData, g_nServerSetNum, dwConnectionIndex);
		}
		break;
	case MP_USERCONN_CHANGEMAP_SYN:
		{
			MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;

			WORD tomapserverport = 0;

			// magi82(35) MapChange State
			// 맵이동 할때 어떤 상태의 맵이동인지 체크하게 수정
			MAPCHANGE_INFO* pChangeInfo = g_pServerSystem->GetMapChangeInfo(pmsg->dwData1);
			if( eMapChange_General == pmsg->dwData4 )
			{
				if(!pChangeInfo)
				{	
					// ￠￢EAI ¨uⓒªA￠￢￠￢e ¡ÆA¨￢I
					MSGBASE msg;
					msg.Category = MP_USERCONN;
					msg.Protocol = MP_USERCONN_CHANGEMAP_NACK;
					msg.dwObjectID = pmsg->dwObjectID;
					g_Network.Send2User((MSGBASE*)&msg, sizeof(MSGBASE));

					//debug imsi
//					FILE* fp = fopen( "mapchangefail.txt", "a+" );
//					if(fp)
//					{
//						fprintf(fp, "CHRID %u, FILE %s, %d, PARAM %d\n", pmsg->dwObjectID, __FILE__, __LINE__, pmsg->dwData1 );
//						fclose(fp);
//					}

					return;
				}
				tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, pChangeInfo->MoveMapNum);
			}
			else if( eMapChange_SOS == pmsg->dwData4 )	// magi82 - SOS(070723)
			{
				tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)(pmsg->dwData1));
			}
			else if( eMapChange_Item == pmsg->dwData4 )
				tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)(pmsg->dwData1));

			if(tomapserverport)
			{
				USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
				if( !pInfo ) return;

				if( GMINFO->IsEventStarted() )
				if( 59 == pChangeInfo->MoveMapNum )
				{
					MSGBASE msg;
					msg.Category = MP_USERCONN;
					msg.Protocol = MP_USERCONN_CHANGEMAP_NACK;
					msg.dwObjectID = pmsg->dwObjectID;
					g_Network.Send2User((MSGBASE*)&msg, sizeof(MSGBASE));

					//debug imsi
//					FILE* fp = fopen( "mapchangefail.txt", "a+" );
//					if(fp)
//					{
//						fprintf(fp, "CHRID %u, FILE %s, %d, PARAM %d\n", pmsg->dwObjectID, __FILE__, __LINE__, pmsg->dwData1 );
//						fclose(fp);
//					}

					return;
				}				

				g_Network.Send2Server(pInfo->dwMapServerConnectionIndex,(char*)pMsg,dwLength);

				//SERVERINFO* pSInfo = g_pServerTable->FindServer((DWORD)tomapserverport);
				//¿ⓒ±a¼­ ¿O CIAo?
				//pInfo->wUserMapNum = (WORD)pChangeInfo->MoveMapNum;		// ⓒoUⓒ÷ⓒ￡ ￠￢E ⓒoⓒªE¡I¡¤I ¨uA¨¡A
				//pInfo->dwMapServerConnectionIndex = pSInfo->dwConnectionIndex;	// ⓒoUⓒ÷ⓒ￡ ￠￢E dwConnectionIndex¡¤I ¨uA¨¡A
			}
			else
			{
				// ￠￢EAI ¨uⓒªA￠￢￠￢e ¡ÆA¨￢I
				MSGBASE msg;
				msg.Category = MP_USERCONN;
				msg.Protocol = MP_USERCONN_CHANGEMAP_NACK;
				msg.dwObjectID = pmsg->dwObjectID;
				g_Network.Send2User((MSGBASE*)&msg, sizeof(MSGBASE));

				//debug imsi
//				FILE* fp = fopen( "mapchangefail.txt", "a+" );
//				if(fp)
//				{
//					fprintf(fp, "CHRID %u, FILE %s, %d, PARAM %d\n", pmsg->dwObjectID, __FILE__, __LINE__, pmsg->dwData1 );
//					fclose(fp);
//				}

				return;
			}
		}
		break;

	case MP_USERCONN_MAP_OUT:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			MAPCHANGE_INFO* ChangeInfo = NULL;
			WORD mapport = 0;

			if( pmsg->dwData < 2000 )
			{
				if (pmsg->dwData == 1)
				{
					return;
				}

				ChangeInfo = g_pServerSystem->GetMapChangeInfo(pmsg->dwData);
				mapport = g_pServerTable->GetServerPort( eSK_MAP, ChangeInfo->MoveMapNum);
			}
			else
				mapport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)(pmsg->dwData-2000));

			if(mapport)
			{
				MSG_DWORD msgserver;
				msgserver.Category = MP_USERCONN;
				msgserver.Protocol = MP_USERCONN_CHANGEMAP_ACK;
				if( pmsg->dwData < 2000 )
					msgserver.dwData = ChangeInfo->MoveMapNum;
				else
					msgserver.dwData = pmsg->dwData-2000;
				msgserver.dwObjectID = pmsg->dwObjectID;	
				g_Network.Send2User((MSG_DWORD*)&msgserver, sizeof(MSG_DWORD));

				SERVERINFO* pSInfo = g_pServerTable->FindServer(mapport);
				USERINFO* pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);

				if(pInfo == NULL)
					return;

				if( pmsg->dwData < 2000 )
					pInfo->wUserMapNum = (WORD)ChangeInfo->MoveMapNum;
				else
					pInfo->wUserMapNum = (WORD)pmsg->dwData-2000;				
				pInfo->dwMapServerConnectionIndex = pSInfo->dwConnectionIndex;
			}
		}
		break;
	case MP_USERCONN_MAP_OUT_WITHMAPNUM:	//cheat￠?￠®¨u¡ⓒ￠￢￠￢ ¨u￠￥￠￥U.
		{	
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			WORD mapport = g_pServerTable->GetServerPort( eSK_MAP, pmsg->wData1);
			if(mapport)
			{
				USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
				if( !pInfo ) return;

				MSG_DWORD msgserver;
				msgserver.Category		= MP_USERCONN;

				if( pInfo->UserLevel > eUSERLEVEL_GM )
					msgserver.Protocol		= MP_USERCONN_CHEAT_CHANGEMAP_ACK;
				else
					msgserver.Protocol		= MP_USERCONN_CHANGEMAP_ACK;

				msgserver.dwData		= pmsg->wData1;			//AI￥i￠?CO ￠￢E
				msgserver.dwObjectID	= pmsg->dwObjectID;		//AI￥i￠?CO A?Au
				//objectid¡¤I ¡ÆE¡ioC¨ª¨u¡ⓒ ¨￢￠￢ⓒø¨o￠￥U.				
				g_Network.Send2User((MSG_DWORD*)&msgserver, sizeof(MSG_DWORD));

				/////////////////
				SERVERINFO* pSInfo	= g_pServerTable->FindServer(mapport);
				pInfo->wUserMapNum	= pmsg->wData1;		// ⓒoUⓒ÷ⓒ￡ ￠￢E ⓒoⓒªE¡I¡¤I ¨uA¨¡A
				pInfo->dwMapServerConnectionIndex = pSInfo->dwConnectionIndex;	// ⓒoUⓒ÷ⓒ￡ ￠￢E dwConnectionIndex¡¤I ¨uA¨¡A
				if( pmsg->wData2 != (WORD)(-1) )
					pInfo->wChannel	= pmsg->wData2;
			}
		}
		break;
	case MP_USERCONN_CHANGEMAP_ACK:
		{
			g_Network.Send2User((MSGBASE*)pTempMsg,dwLength);
		}
		break;
	case MP_USERCONN_SAVEPOINT_SYN:
		{
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(!pInfo)
				return;
			g_Network.Send2Server(pInfo->dwMapServerConnectionIndex,pMsg,dwLength);
		}
		break;

	case MP_USERCONN_BACKTOCHARSEL_SYN:
		{
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(!pInfo)
				return;
//---imsi
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			if(pInfo->dwCharacterID != pmsg->dwObjectID )
				return;
//--
			g_Network.Send2Server(pInfo->dwMapServerConnectionIndex,pMsg,dwLength);
		}
		break;
	case MP_USERCONN_BACKTOCHARSEL_ACK:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			USERINFO * pInfo = (USERINFO*)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!pInfo)
				return;

//KES 031111
			DWORD UserID = pInfo->dwUserID;
			UserIDXSendAndCharacterBaseInfo(UserID, pInfo->dwUniqueConnectIdx, pInfo->dwConnectionIndex);
#ifdef _JAPAN_LOCAL_
			GetCharacterSlotCount( UserID, pInfo->dwUniqueConnectIdx, pInfo->dwConnectionIndex );
#endif
#ifdef _HK_LOCAL_
			GetCharacterSlotCount( UserID, pInfo->dwUniqueConnectIdx, pInfo->dwConnectionIndex );
#endif
#ifdef _TL_LOCAL_
			GetCharacterSlotCount( UserID, pInfo->dwUniqueConnectIdx, pInfo->dwConnectionIndex );
#endif

			g_pUserTableForObjectID->RemoveUser(pInfo->dwCharacterID);
			pInfo->dwCharacterID = 0;
			pInfo->dwMapServerConnectionIndex = 0;
			pInfo->wUserMapNum = 0;
		}
		break;
	case MP_USERCONN_CHEAT_USING:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			CheatLog(pmsg->dwObjectID,pmsg->dwData);
		}
		break;

	case MP_USERCONN_LOGINCHECK_DELETE:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			if( pmsg->dwObjectID )
			{
				//no user in agent	//any other agent?
				if( g_pUserTableForUserID->FindUser(pmsg->dwObjectID) == NULL )
				{
					LoginCheckDelete( pmsg->dwObjectID );
				}
			}
		}
		break;
	case MP_USERCONN_GAMEINPOS_SYN:
		{
		//	MSGBASE* pmsg = (MSGBASE*)pMsg;
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(!pInfo)
				return;

			// ¨u¡¾AACN AE￠￢?AI￠?￠® ￠￢A￠￥A ￠￢E¨u¡ⓒⓒoo¡¤I AE￠￢?AI ¨u¡¾AAA¡i ¨uE￠￢ⓒ÷
	/*		MSG_DWORD msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_GAMEIN_SYN;
			msg.dwObjectID = pInfo->dwCharacterID;
			msg.dwData = pInfo->dwUniqueConnectIdx;
	*/
			MSG_DWORD4 msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_GAMEINPOS_SYN;
			msg.dwObjectID = pInfo->dwCharacterID;
			msg.dwData1 = pInfo->dwUniqueConnectIdx;
			
			// magi82 - SOS(070807) SOS를 위해서 막음
//			if( pInfo->UserLevel < eUSERLEVEL_SUPERUSER )
//			{
				msg.dwData2 = pmsg->dwData1;				//채널번호
				pInfo->wChannel = (WORD)pmsg->dwData1;
//			}
//			else
//			{
//				msg.dwData2	= pInfo->wChannel;	//저장된 채널번호로 세팅하자.
//			}
			msg.dwData3 = pInfo->UserLevel;
			msg.dwData4 = pmsg->dwData2;

			g_Network.Send2Server(pInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;

	case MP_USERCONN_BACKTOBEFOREMAP_SYN:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			WORD mapport = g_pServerTable->GetServerPort( eSK_MAP, pmsg->wData2 );
			if(mapport)
			{
				USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
				if( !pInfo ) return;

				g_Network.Send2Server( pInfo->dwMapServerConnectionIndex, (char*)pMsg, dwLength );
			}
		}
		break;

	case MP_USERCONN_BACKTOBEFOREMAP_ACK:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			WORD mapport = g_pServerTable->GetServerPort( eSK_MAP, pmsg->wData );
			if(mapport)
			{
				USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
				if( !pInfo ) return;

				MSG_DWORD msgserver;
				msgserver.Category		= MP_USERCONN;
				msgserver.Protocol		= MP_USERCONN_CHANGEMAP_ACK;
				msgserver.dwObjectID	= pmsg->dwObjectID;
				msgserver.dwData		= pmsg->wData;
				
				g_Network.Send2User((MSG_DWORD*)&msgserver, sizeof(MSG_DWORD));

				SERVERINFO* pSInfo	= g_pServerTable->FindServer(mapport);
				pInfo->wUserMapNum	= pmsg->wData;
				pInfo->dwMapServerConnectionIndex = pSInfo->dwConnectionIndex;
			}
		}
		break;
		
// eventmap ---------------------------------------------------------------------------------
	case MP_USERCONN_MAP_OUT_TO_EVENTMAP:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			WORD mapport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData1 );
			if( mapport )
			{
				USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
				if( !pInfo ) return;

				SERVERINFO* pSInfo = g_pServerTable->FindServer( mapport );
				pInfo->wUserMapNum = (WORD)pmsg->dwData1;				
				pInfo->dwMapServerConnectionIndex = pSInfo->dwConnectionIndex;

				g_Network.Send2User( (MSGBASE*)pMsg, sizeof(MSG_DWORD3) );				
			}
		}
		break;
	case MP_USERCONN_ENTER_EVENTMAP_SYN:
		{
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if( !pInfo )	return;

			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			
			MSG_DWORD4 msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_ENTER_EVENTMAP_SYN;
			msg.dwObjectID = pInfo->dwCharacterID;
			msg.dwData1 = pInfo->dwUniqueConnectIdx;
			msg.dwData2 = pInfo->UserLevel;
			msg.dwData3 = pmsg->dwData2;		// channel
			msg.dwData4 = pmsg->dwData3;		// team
			
			g_Network.Send2Server(pInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
// ----------------------------------------------------------------------------------------------
// 050106 event ---------------------------------------------------------------------------------
	case MP_USERCONN_EVENTITEM_USE:
		{
/*
			// event 050106
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pInfo )	return;
			EventItemUse050106( pInfo->dwUserID );
			g_Network.Send2User( (MSGBASE*)pMsg, dwLength );			

			// event 050203
			MSG_NAME* pmsg = (MSG_NAME*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pInfo )	return;
			EventItemUse050203( pInfo->dwUserID, pmsg->Name, g_nServerSetNum );
			g_Network.Send2User( (MSGBASE*)pMsg, dwLength );

			// 050322 event
			MSG_NAME* pmsg = (MSG_NAME*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pInfo )	return;
			EventItemUse050322( pInfo->dwUserID );
			g_Network.Send2User( (MSGBASE*)pMsg, dwLength );
*/
			// event 051108
			MSG_NAME* pmsg = (MSG_NAME*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pInfo )	return;
			EventItemUse051108( pmsg->dwObjectID, pInfo->dwUserID, pmsg->Name, g_nServerSetNum );
//			g_Network.Send2User( (MSGBASE*)pMsg, dwLength );
		}
		break;
	case MP_USERCONN_EVENTITEM_USE2:
		{
			MSG_NAME* pmsg = (MSG_NAME*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pInfo )	return;
			EventItemUse2( pInfo->dwUserID, pmsg->Name, g_nServerSetNum );
			g_Network.Send2User( (MSGBASE*)pMsg, dwLength );
		}
		break;
#ifdef _KOR_LOCAL_
	case MP_USERCONN_CHANGEMAP_CHANNELINFO_SYN: //2008. 5. 8. CBH - 이동할 맵의 채널 정보 요청
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(!pInfo)
				return;
			
			MSG_DWORD3 msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_CHANGEMAP_CHANNELINFO_SYN;
			msg.dwObjectID = dwConnectionIndex;
			msg.dwData1 = pInfo->dwUniqueConnectIdx;
			msg.dwData2 = pmsg->dwData1;
			msg.dwData3 = pmsg->dwData2;

			MAPTYPE mapnum = 0;

			if(pmsg->dwData2 == eMapChange_General)
			{
				MAPCHANGE_INFO* pMCInfo = NULL;
				pMCInfo = g_pServerSystem->GetMapChangeInfo(pmsg->dwData1);
				if(pMCInfo == NULL)
				{
					ASSERTMSG(0,"올바른 정보를 얻을 수 없습니다.");
					break;
				}

				mapnum = pMCInfo->MoveMapNum;
			}
			else if(pmsg->dwData2 == eMapChange_Item)
			{
				mapnum = pmsg->dwData1;
			}
			
			WORD mapport = g_pServerTable->GetServerPort( eSK_MAP, mapnum);
			if(mapport)
			{
				SERVERINFO* pSInfo = g_pServerTable->FindServer(mapport);
				g_Network.Send2Server(pSInfo->dwConnectionIndex, (char*)&msg, sizeof(MSG_DWORD3));
			}
			else
			{
				MSG_BYTE msg;
				msg.Category	= MP_USERCONN;
				msg.Protocol	= MP_USERCONN_CHANGEMAP_CHANNELINFO_NACK;
				msg.bData		= 0;
				g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
			}			
		}
		break;

	case MP_USERCONN_CHANGEMAP_CHANNELINFO_ACK: //2008. 5. 8. CBH - 이동할 맵의 채널 정보 요청 성공
		{
			MSG_CHANNEL_INFO* pmsg = (MSG_CHANNEL_INFO*)pMsg;
			USERINFO* pInfo = g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pInfo == NULL)
				return;
			if(pInfo->dwUniqueConnectIdx != pmsg->dwUniqueIDinAgent)
				return;
			g_Network.Send2User(pmsg->dwObjectID, (char*)pMsg, dwLength);
		}
		break;

	case MP_USERCONN_CHANGEMAP_CHANNELINFO_NACK:	//2008. 5. 8. CBH - 이동할 맵의 채널 정보 요청 실패
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
			USERINFO* pInfo = g_pUserTable->FindUser(pmsg->dwObjectID);
			if(pInfo == NULL)
				return;
			g_Network.Send2User(pmsg->dwObjectID, (char*)pMsg, dwLength);
		}
		break;
#endif
//-------------------------------------------------------------		

	default:
		g_Network.Send2User((MSGBASE*)pTempMsg,dwLength);
		break;
	}
}

void MP_CHATServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	switch(pTempMsg->Protocol)
	{
	case MP_CHAT_ALL:		//￠￢E¨u¡ⓒⓒoo￠?￠®¨u¡ⓒ ¨￢￠￢ⓒø¨o¡ÆI
	case MP_CHAT_SMALLSHOUT:
	case MP_CHAT_GM_SMALLSHOUT:
	case MP_CHAT_MONSTERSPEECH:
		{
			TransToClientMsgParser(dwConnectionIndex,pMsg,dwLength);
		}
		break;
	case MP_CHAT_WHISPER_SYN:	//￠￥U￠￢¡I ￠?￠®AIA?¨¡￠c￠?￠®¨u¡ⓒ ¨￢￠￢ⓒø¨o ¡ÆI
		{
			MSG_WHISPER* pmsg = (MSG_WHISPER*)pMsg;
			USERINFO* pReceiverInfo = g_pUserTableForObjectID->FindUser( pmsg->dwReceiverID );
			if( !pReceiverInfo ) break;

			if( pReceiverInfo->GameOption.bNoWhisper )
			{
				MSG_BYTE msg;
				msg.Category	= MP_CHAT;
				msg.Protocol	= MP_CHAT_WHISPER_NACK;
				msg.dwObjectID	= pmsg->dwObjectID;	//¨￢￠￢ⓒø¨o¡ic￠O¡A ¨u¨¡AI￥iⓒ￡
				msg.bData		= CHATERR_OPTION_NOWHISPER;

				g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));
				break;
			}

			//---¨￢￠￢ⓒø¨o¡ic￠O¡AAI AO￠￥A ￠?￠®AIA?¨¡￠c¡¤I
			MSG_CHAT msgChat;
			msgChat.Category	= MP_CHAT;
			msgChat.Protocol	= MP_CHAT_WHISPER_ACK;
			msgChat.dwObjectID	= pmsg->dwObjectID;	//¨￢￠￢ⓒø¨o¡ic￠O¡A
			SafeStrCpy( msgChat.Name, pmsg->ReceiverName, MAX_NAME_LENGTH+1 );	//ⓒi¹￠i½￠￥A¡ic￠O¡AAC AI￠￢¡×
			SafeStrCpy( msgChat.Msg, pmsg->Msg, MAX_CHAT_LENGTH+1 );
			g_Network.Send2Server( dwConnectionIndex, (char*)&msgChat, msgChat.GetMsgLength() );

			//---ⓒi¹￠i½￠￥A¡ic￠O¡"?￠®¡ÆO ¨￢￠￢ⓒø¡i¡¾a
			msgChat.Protocol	= MP_CHAT_WHISPER;
			SafeStrCpy( msgChat.Name, pmsg->SenderName, MAX_NAME_LENGTH+1 ); //¨￢￠￢ⓒø¨o¡ic￠O¡AAC AI￠￢¡×
			g_Network.Send2User( pReceiverInfo->dwConnectionIndex, (char*)&msgChat, msgChat.GetMsgLength() );	//CHATMSG 040324

		}
		break;

	case MP_CHAT_WHISPER_GM_SYN:
		{
			MSG_WHISPER* pmsg = (MSG_WHISPER*)pMsg;
			USERINFO* pReceiverInfo = g_pUserTableForObjectID->FindUser( pmsg->dwReceiverID );
			if( !pReceiverInfo ) break;

//			GM의 귓말일 경우 귓말 거부는 적용되지 않는다.
//			if( pReceiverInfo->GameOption.bNoWhisper )
//			{
//				MSG_BYTE msg;
//				msg.Category	= MP_CHAT;
//				msg.Protocol	= MP_CHAT_WHISPER_NACK;
//				msg.dwObjectID	= pmsg->dwObjectID;
//				msg.bData		= CHATERR_OPTION_NOWHISPER;

//				g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));
//				break;
//			}

			MSG_CHAT msgChat;
			msgChat.Category	= MP_CHAT;
			msgChat.Protocol	= MP_CHAT_WHISPER_ACK;
			msgChat.dwObjectID	= pmsg->dwObjectID;	
			SafeStrCpy( msgChat.Name, pmsg->ReceiverName, MAX_NAME_LENGTH+1 );
			SafeStrCpy( msgChat.Msg, pmsg->Msg, MAX_CHAT_LENGTH+1 );
			g_Network.Send2Server( dwConnectionIndex, (char*)&msgChat, msgChat.GetMsgLength() );

			msgChat.Protocol	= MP_CHAT_WHISPER_GM;
			SafeStrCpy( msgChat.Name, pmsg->SenderName, MAX_NAME_LENGTH+1 );
			g_Network.Send2User( pReceiverInfo->dwConnectionIndex, (char*)&msgChat, msgChat.GetMsgLength() );	//CHATMSG 040324
		}
		break;

	case MP_CHAT_WHISPER_ACK:
		{
			MSG_CHAT* pmsg = (MSG_CHAT*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );			
			if( !pInfo ) return;

			g_Network.Send2User( pInfo->dwConnectionIndex, (char*)pmsg, dwLength );

		}
		break;
	case MP_CHAT_WHISPER_NACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;
			
			g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)pmsg, dwLength );
		}
		break;
	case MP_CHAT_PARTY:
		{
			MSG_CHAT_WITH_SENDERID* pmsg = (MSG_CHAT_WITH_SENDERID*)pMsg;
			USERINFO* pReceiverInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pReceiverInfo ) break;
			
			g_Network.Send2User( pReceiverInfo->dwConnectionIndex, (char*)pmsg, pmsg->GetMsgLength() );	//CHATMSG 040324
		}
		break;
	case MP_CHAT_GUILD:
		{
			TESTMSGID * pmsg = (TESTMSGID*)pMsg;
			USERINFO* pReceiverInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pReceiverInfo ) break;
			
			TransToClientMsgParser(pReceiverInfo->dwConnectionIndex, pMsg, dwLength);
		}
		break;
	case MP_CHAT_GUILDUNION:
		{
			MSG_CHAT_WITH_SENDERID* pmsg = (MSG_CHAT_WITH_SENDERID*)pMsg;
			USERINFO* pReceiverInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pReceiverInfo ) break;
			
			TransToClientMsgParser(pReceiverInfo->dwConnectionIndex, pMsg, dwLength);
		}
		break;
	case MP_CHAT_SHOUT_SENDSERVER:
		{
			SEND_SHOUTBASE* pmsg = (SEND_SHOUTBASE*)pMsg;
			
			SHOUTMGR->AddShoutMsg( &pmsg->ShoutMsg[0] );
		}
		break;		
	}
}

void MP_CHATMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)	//from client
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	
	USERINFO* pInfoForCheck = g_pUserTable->FindUser( dwConnectionIndex );
	
	if( pInfoForCheck )
	{
		if( gCurTime - pInfoForCheck->dwLastChatTime < 1000 )	//1초
		{
			//도배
			MSGBASE msg;
			msg.Category	= MP_CHAT;
			msg.Protocol	= MP_CHAT_FASTCHAT;
			msg.dwObjectID	= pTempMsg->dwObjectID;
			g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
			return;
		}
		
		pInfoForCheck->dwLastChatTime = gCurTime;
	}

	switch(pTempMsg->Protocol)
	{
	case MP_CHAT_ALL:
	case MP_CHAT_SMALLSHOUT:
	case MP_CHAT_MONSTERSPEECH:
		{
			TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);
		}
		break;

	case MP_CHAT_WHISPER_SYN:
		{
			MSG_CHAT* pmsg = (MSG_CHAT*)pMsg;

			USERINFO* pInfo = g_pUserTable->FindUser( dwConnectionIndex );
			if( !pInfo )	return;

			//---클라이언트에서 온 정보는 믿을 수 없다. buf로 옮긴다.
			char buf[MAX_NAME_LENGTH+1];
			SafeStrCpy( buf, pmsg->Name, MAX_NAME_LENGTH+1 );
			int nLen = strlen( buf );
			if( nLen < 4 ) return;		//캐릭터 이름이 4글자 이하일리가 없다.

#ifdef _KOR_LOCAL_
			if( strncmp( buf, "[황룡]", 6 ) == 0 ||
				strncmp( buf, "[청룡]", 6 ) == 0 )
			{
			}
			else
#endif
			if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)buf)) == TRUE )
			{
				MSG_BYTE msg;
				msg.Category	= MP_CHAT;
				msg.Protocol	= MP_CHAT_WHISPER_NACK;
				msg.dwObjectID	= pmsg->dwObjectID;
				msg.bData		= CHATERR_NO_NAME;
				g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
				return;
			}

			DWORD dwKeyOut;
			if( g_MsgTable.AddMsg( pmsg, &dwKeyOut ) )
			{
				SearchWhisperUserAndSend( pmsg->dwObjectID, buf, dwKeyOut );
			}
			else
			{
				MSG_BYTE msg;
				msg.Category	= MP_CHAT;
				msg.Protocol	= MP_CHAT_WHISPER_NACK;
				msg.dwObjectID	= pmsg->dwObjectID;
				msg.bData		= CHATERR_ERROR;
				g_Network.Send2User(dwConnectionIndex, (char*)&msg, sizeof(msg));
				break;
			}
		}
		break;
		

	case MP_CHAT_PARTY:
		{
			SEND_PARTY_CHAT* pmsg =	(SEND_PARTY_CHAT*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!pSenderInfo)
				return;
			
			MSG_CHAT_WITH_SENDERID msgChat;
			msgChat.Category	= pmsg->Category;
			msgChat.Protocol	= pmsg->Protocol;
			msgChat.dwSenderID	= pmsg->dwObjectID;

			SafeStrCpy( msgChat.Name, pmsg->Name, MAX_NAME_LENGTH+1 );
			SafeStrCpy( msgChat.Msg, pmsg->Msg, MAX_CHAT_LENGTH+1 );

			for(int i=0; i<pmsg->MemberNum; ++i)
			{
				if(pmsg->MemberID[i] != 0)
				{
					USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pmsg->MemberID[i]);
					msgChat.dwObjectID	= pmsg->MemberID[i];
					if(pRecverInfo)
					{
//						g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msgChat, msgChat.GetMsgLength() );	//CHATMSG 040324
						MSG_CHAT_WITH_SENDERID msgTemp = msgChat;
						g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)&msgTemp, msgTemp.GetMsgLength() );	//CHATMSG 040324
					}
					else
					{						
						g_Network.Broadcast2AgentServerExceptSelf( (char*)&msgChat, msgChat.GetMsgLength() );
					}
				}
			}
		}
		break;

	case MP_CHAT_GUILD:
		{
			TESTMSGID* pmsg = (TESTMSGID*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!pSenderInfo)
				return;
			g_Network.Broadcast2MapServer(pMsg, dwLength);
		}
		break;

	case MP_CHAT_GUILDUNION:
		{
			MSG_CHAT_WITH_SENDERID* pmsg = (MSG_CHAT_WITH_SENDERID*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo )	return;
			g_Network.Broadcast2MapServer( pMsg, dwLength );
		}
		break;

		/*
	case MP_CHAT_SHOUT_SYN:
		{
			SEND_SHOUTBASE* pmsg = (SEND_SHOUTBASE*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if( !pSenderInfo )	return;

			SHOUTRECEIVE rev;
			SHOUTMGR->AddShoutMsg( &pmsg->ShoutMsg[0], &rev );

			// 메세지가 다 차있을때
			if( rev.Count == 0 )
			{
				MSGBASE msg;				
				msg.Category = MP_CHAT;
				msg.Protocol = MP_CHAT_SHOUT_NACK;

				g_Network.Send2User( &msg, sizeof(msg) );
			}
			else
			{
				SEND_SHOUTRECEIVE msg;
				msg.Category = MP_CHAT;
				msg.Protocol = MP_CHAT_SHOUT_ACK;
				msg.Receive = rev;

				g_Network.Send2User( pSenderInfo->dwConnectionIndex,  (char*)&msg, sizeof(msg) );

				pmsg->Protocol = MP_CHAT_SHOUT_SENDSERVER;
				g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, dwLength );
			}
		}
		break;

	case MP_CHAT_SHOUT_SENDSERVER:
		{
			SEND_SHOUTBASE* pmsg = (SEND_SHOUTBASE*)pMsg;
			
			SHOUTMGR->AddShoutMsg( &pmsg->ShoutMsg[0] );
		}
		break;		
		*/
		
	default:
		ASSERT(0);
		//GAMESYSTEM_OBJ->m_ConnectionTable.SendToCharacter(pTempMsg,dwLength);
		break;
	}
}

void MP_PARTYServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_PARTY_NOTIFYADD_TO_MAPSERVER:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	case MP_PARTY_NOTIFYDELETE_TO_MAPSERVER:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	case MP_PARTY_NOTIFYCHANGEMASTER_TO_MAPSERVER:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	case MP_PARTY_NOTIFYBREAKUP_TO_MAPSERVER:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	case MP_PARTY_NOTIFYBAN_TO_MAPSERVER:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	case MP_PARTY_NOTIFYMEMBER_LOGIN_TO_MAPSERVER:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	case MP_PARTY_NOTIFYMEMBER_LOGOUT_TO_MAPSERVER:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	case MP_PARTY_NOTIFYMEMBER_LOGINMSG:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex,pMsg,dwLength);			
		}
		break;
	case MP_PARTY_NOTIFYCREATE_TO_MAPSERVER:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex,pMsg,dwLength);
		}
		break;
	case MP_PARTY_NOTIFYMEMBER_LEVEL:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex,pMsg,dwLength);
		}
		break;	
	case MP_PARTY_REQUEST_CONSENT_ACK:	//2008. 5. 23. CBH - 파티 신청 수락
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			USERINFO* pPlayerInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if(pPlayerInfo != NULL)
			{				
				g_Network.Send2Server( pPlayerInfo->dwMapServerConnectionIndex, (char*)pmsg, sizeof(*pmsg) );
			}
			else
			{
				USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwData1 );
				if(pInfo != NULL)
				{
					MSGBASE msg;				
					msg.Category	= MP_PARTY;
					msg.Protocol	= MP_PARTY_REQUEST_CONSENT_NACK;
					msg.dwObjectID	= pmsg->dwData1;
					g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
				}				
			}
		}
		break;
	case MP_PARTY_REQUEST_REFUSAL_ACK:	//2008. 5. 23. CBH - 파티 신청 거절
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			USERINFO* pPlayerInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if(pPlayerInfo != NULL)
			{				
				g_Network.Send2User( pPlayerInfo->dwConnectionIndex, (char*)pmsg, sizeof(*pmsg) );
			}
			else
			{
				USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwData );
				if(pInfo != NULL)
				{
					MSGBASE msg;				
					msg.Category	= MP_PARTY;
					msg.Protocol	= MP_PARTY_REQUEST_REFUSAL_NACK;
					msg.dwObjectID	= pmsg->dwData;
					g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
				}				
			}
		}
		break;		
	case MP_PARTY_ERROR:	//2008. 5. 23. CBH - 파티 에러
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			USERINFO* pPlayerInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if(pPlayerInfo != NULL)
			{
				g_Network.Send2User( pPlayerInfo->dwConnectionIndex, (char*)pmsg, sizeof(*pmsg) );
			}
		}
		break;
		/*
	case MP_PARTY_MATCHING_INFO:
		{
			MSG_PARTYMATCHING_INFO* pmsg = (MSG_PARTYMATCHING_INFO*)pMsg;

			//방파장체크해서 다시 셋팅
			MSG_PARTYMATCHING_INFO msg;			
			msg.Category = pmsg->Category;
			msg.Protocol = pmsg->Protocol;
			msg.dwObjectID = pmsg->dwObjectID;

			WORD wCount = 0;
			for(int  i = 0 ; i < pmsg->wCount ; i++)
			{
				DWORD wMaterID = pmsg->PartyMatchingInfo[i].Member[0].dwMemberID;
				USERINFO* pPartyMaster = g_pUserTableForObjectID->FindUser(wMaterID);
				if(pPartyMaster == NULL)
					continue;

				msg.PartyMatchingInfo[wCount] = pmsg->PartyMatchingInfo[i];

				wCount++;
			}

			msg.wCount = wCount;

			TransToClientMsgParser( dwConnectionIndex, (char*)&msg, msg.GetMsgSize() );			
		}
		break;	*/
	default:
		TransToClientMsgParser(dwConnectionIndex,pMsg,dwLength);
		break;
	}
}

//2008. 5. 26. CBH - 파티 매칭 시스탬 관련 추가
void MP_PARTYUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_PARTY_MASTERTOREQUEST_SYN: //2008. 5. 23. CBH - 파티신청
		{
			MSG_NAME_DWORD2* pmsg = (MSG_NAME_DWORD2*)pMsg;

			// 방파장 유무 체크
			USERINFO* pMasterUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwData2 );
			if(pMasterUserInfo != NULL)
			{				
				g_Network.Send2Server( pMasterUserInfo->dwMapServerConnectionIndex, (char*)pmsg, sizeof(*pmsg) );
			}
			else
			{
				USERINFO* pRequestUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
				if(pRequestUserInfo != NULL)
				{
					MSG_DWORD msg;
					SetProtocol( &msg, MP_PARTY, MP_PARTY_ERROR );
					msg.dwObjectID = pmsg->dwObjectID;
					msg.dwData = eErr_Request_NotMaster;
					g_Network.Send2User( pRequestUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
				}				
			}
		}
		break;
	default:
		TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
		break;
	}
}

void TransToClientMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	g_Network.Send2User(pTempMsg,dwLength);
}

void TransToMapServerMsgParser(DWORD dwConnectionIndex/*A?Au￠?￠® ￠￥eCNConnectionIndex*/, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	USERINFO * userinfo = (USERINFO *)g_pUserTable->FindUser(dwConnectionIndex);
	if(userinfo == NULL)
		return;

	//---- dwobjectid를 검사하자!

	g_Network.Send2Server(userinfo->dwMapServerConnectionIndex,pMsg,dwLength);
}
void MP_PACKEDMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_PACKEDDATA_NORMAL:
		{
			static char TempData[MAX_PACKEDDATA_SIZE];
			SEND_PACKED_DATA * pack =  (SEND_PACKED_DATA *)pMsg;
			DWORD* charterIDs = (DWORD*)&pack->Data[pack->wRealDataSize];
			for(int n=0;n<pack->wReceiverNum;++n)
			{
				USERINFO * userInfo = g_pUserTableForObjectID->FindUser(charterIDs[n]);
				// taiyo :
				// charterIDs[n] ¡Æ￠® Ca￠?eCIAo ¨uE￠￥A ID¡Æ￠® ⓒøN¨ui￠?I¨u¡ⓒ NULLAI ￥iC¨ui¨u¡ⓒ ￠￢￠cAIAⓒø￠￢￠c
				if(!userInfo)
				{
					continue;
				}

				memcpy(TempData,pack->Data,pack->wRealDataSize);
				TempData[pack->wRealDataSize] = 0;
				g_Network.Send2User(userInfo->dwConnectionIndex, TempData, pack->wRealDataSize);
			}
		}
		break;

	case MP_PACKEDDATA_TOMAPSERVER:
		{
			SEND_PACKED_TOMAPSERVER_DATA* pmsg = (SEND_PACKED_TOMAPSERVER_DATA*)pMsg;
			WORD ToMapPort = g_pServerTable->GetServerPort( eSK_MAP, pmsg->ToMapNum);
			if(ToMapPort == 0)
				return;

			SERVERINFO* pInfo = g_pServerTable->FindServer(ToMapPort);
			if(pInfo == NULL)
				return;

			g_Network.Send2Server(pInfo->dwConnectionIndex,pmsg->Data,pmsg->wRealDataSize);						
		}
		break;
	case MP_PACKEDDATA_TOBROADMAPSERVER:
		{
			SEND_PACKED_TOMAPSERVER_DATA* pmsg = (SEND_PACKED_TOMAPSERVER_DATA*)pMsg;

			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pmsg->Data,pmsg->wRealDataSize);
			//AI¡¤?￠￢e AI ￠?￠®AIAu¨¡￠c￠?￠® ￠?¡þ¡ÆaAI ⓒ÷¡!¾a ￠￢E ¨u¡ⓒⓒoo￠￥A? 
		}
		break;
	}
}

void MP_FRIENDMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_FRIEND_LOGIN:
		{
			USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pTempMsg->dwObjectID);
			if(!userinfo)
				return;
			//ⓒø¨￡￠￢| ￥ii¡¤ICN A¡I¡¾￠￢￥ie￠?￠®¡ÆO ¡¤I¡¾¡¿AI ¨uE￠￢ⓒ÷
			FriendNotifyLogintoClient(pTempMsg->dwObjectID);
			//ⓒø¡i¡Æ￠® ￥ii¡¤ICN A¡I¡¾￠￢￥ieAI ¡¤I¡¾¡¿AIAIAo ¨uE¨u¨¡￠?E
		//	FriendGetLoginFriends(pTempMsg->dwObjectID); CE¿a ¾ø´A ±a´EAI°¡... 
			NoteIsNewNote(pTempMsg->dwObjectID);
		}
		break;
	case MP_FRIEND_ADD_SYN:
		{
			MSG_NAME* pmsg = (MSG_NAME*)pMsg;
			USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!userinfo)
				return;
			
			char buf[MAX_NAME_LENGTH+1];
			SafeStrCpy( buf, pmsg->Name, MAX_NAME_LENGTH+1 );

			//금지문자 체크 "'"등...
			if( FILTERTABLE->IsInvalidCharInclude( (unsigned char*) buf ) )
				return;
			
			FriendGetUserIDXbyName(pmsg->dwObjectID, buf);
		}
		break;
	case MP_FRIEND_ADD_ACCEPT:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!userinfo)
				return;
			FriendAddFriend(pmsg->dwData, pmsg->dwObjectID);//db insert
		}
		break;
	case MP_FRIEND_ADD_DENY:
		{
			MSG_FRIEND_MEMBER_ADDDELETEID* pmsg = (MSG_FRIEND_MEMBER_ADDDELETEID*)pMsg;
			
			USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->PlayerID);
			if(!userinfo)
				return;

			MSG_FRIEND_MEMBER_ADDDELETEID msg;
			msg.Category = MP_FRIEND;
			msg.Protocol = MP_FRIEND_ADD_NACK;
			SafeStrCpy( msg.Name, pmsg->Name, MAX_NAME_LENGTH + 1 );
			msg.PlayerID = eFriend_AddDeny;
			g_Network.Send2User(userinfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
	
		}
		break;
	case MP_FRIEND_DEL_SYN:
		{
			MSG_NAME * pmsg = (MSG_NAME*)pMsg;
			FriendDelFriend(pmsg->dwObjectID, pmsg->Name); //DB
			//msg
		}
		break;
	case MP_FRIEND_DELID_SYN:
		{
			MSG_DWORD2 * pmsg = (MSG_DWORD2*)pMsg;
			
			FriendDelFriendID(pmsg->dwObjectID, pmsg->dwData1, pmsg->dwData2);
		}
		break;
	case MP_FRIEND_ADDID_SYN:
		{
			MSG_FRIEND_MEMBER_ADDDELETEID * pmsg = (MSG_FRIEND_MEMBER_ADDDELETEID*)pMsg;
			
			FriendIsValidTarget(pmsg->dwObjectID, pmsg->PlayerID, pmsg->Name);
		}
		break;
	case MP_FRIEND_LOGOUT_NOTIFY_TO_AGENT:
		{
			MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;
			USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(userinfo)
			{
				pmsg->Protocol = MP_FRIEND_LOGOUT_NOTIFY_TO_CLIENT;
				g_Network.Send2User(userinfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_NAME));
			}
			else
			{
				pmsg->Protocol = MP_FRIEND_LOGOUT_NOTIFY_AGENT_TO_AGENT;
				g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, sizeof(MSG_NAME));
			}
		}
		break;
	case MP_FRIEND_LOGOUT_NOTIFY_AGENT_TO_AGENT:
		{
			MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;
			USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(userinfo)
			{
				pmsg->Protocol = MP_FRIEND_LOGOUT_NOTIFY_TO_CLIENT;
				g_Network.Send2User(userinfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_NAME));
			}
		}
		break;			
	case MP_FRIEND_LIST_SYN:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!userinfo)
				return;
						
			FriendGetFriendList(pmsg->dwObjectID);
		}
		break;
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// from AGENT 
	case MP_FRIEND_ADD_ACK_TO_AGENT:
		{
			MSG_FRIEND_MEMBER_ADDDELETEID* pmsg = (MSG_FRIEND_MEMBER_ADDDELETEID*)pMsg;
			USERINFO * pRecverInfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!pRecverInfo) return;
			pmsg->Protocol = MP_FRIEND_ADD_ACK;
			g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_FRIEND_MEMBER_ADDDELETEID));
		}
		break;
	case MP_FRIEND_ADD_NACK_TO_AGENT: //μi·I ½AA≫CN ≫c¶÷ ½CÆÐ
		{
			MSG_FRIEND_MEMBER_ADDDELETEID* pmsg = (MSG_FRIEND_MEMBER_ADDDELETEID*)pMsg;
			USERINFO * pRecverInfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!pRecverInfo) return;
			pmsg->Protocol = MP_FRIEND_ADD_NACK;
			g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_FRIEND_MEMBER_ADDDELETEID));
		}
		break;
	case MP_FRIEND_ADD_ACCEPT_TO_AGENT:
		{
			MSG_FRIEND_MEMBER_ADDDELETEID * pmsg = (MSG_FRIEND_MEMBER_ADDDELETEID*)pMsg;
			USERINFO* pToRecverInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(pToRecverInfo)
			{			
				pmsg->Protocol = MP_FRIEND_ADD_ACCEPT_ACK;
				g_Network.Send2User(pToRecverInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_FRIEND_MEMBER_ADDDELETEID));
			}
		}
		break;
	case MP_FRIEND_ADD_ACCEPT_NACK_TO_AGENT: //¹Þ¾ÆμeAI ≫c¶÷ ½CÆÐ
		{
			MSG_FRIEND_MEMBER_ADDDELETEID * pmsg = (MSG_FRIEND_MEMBER_ADDDELETEID*)pMsg;
			USERINFO* pToRecverInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!pToRecverInfo) return;
			pmsg->Protocol = MP_FRIEND_ADD_ACCEPT_NACK;
			g_Network.Send2User(pToRecverInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_FRIEND_MEMBER_ADDDELETEID));
		}
		break;
	case MP_FRIEND_LOGIN_NOTIFY_TO_AGENT:
		{
			MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;
			USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(pRecverInfo)
			{
				pmsg->Protocol = MP_FRIEND_LOGIN_NOTIFY;
				g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_NAME_DWORD));
			}
		}
		break;
	case MP_FRIEND_ADD_INVITE_TO_AGENT:
		{
			MSG_FRIEND_MEMBER_ADDDELETEID* pmsg = (MSG_FRIEND_MEMBER_ADDDELETEID*)pMsg;
			USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(pRecverInfo)
			{
				if(pRecverInfo->GameOption.bNoFriend == TRUE)
				{
					//nack
					MSG_FRIEND_MEMBER_ADDDELETEID msg;
					msg.Category = MP_FRIEND;
					msg.dwObjectID = pmsg->PlayerID;
					msg.Protocol = MP_FRIEND_ADD_NACK;
					strcpy( msg.Name,  "");
					msg.PlayerID = eFriend_OptionNoFriend;	//errcode insert

					g_Network.Broadcast2AgentServerExceptSelf((char*)&msg, sizeof(msg));
					return;
				}	
				else //ack
				{
					pmsg->Protocol = MP_FRIEND_ADD_INVITE;
					g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_FRIEND_MEMBER_ADDDELETEID));
				}
			}
		}
		break;
	case MP_FRIEND_ADD_NACK:
		{
			MSG_FRIEND_MEMBER_ADDDELETEID * pmsg = (MSG_FRIEND_MEMBER_ADDDELETEID*)pMsg;
			USERINFO* pRecverInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(pRecverInfo)
			{
				g_Network.Send2User(pRecverInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_FRIEND_MEMBER_ADDDELETEID));
			}
		}
		break;
		
	default: //MP_FRIEND_LOGOUT_NOTIFY_TO_CLIENT
		TransToClientMsgParser(dwConnectionIndex,pMsg,dwLength);
		break;
	}	
}

void MP_NOTEServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{//////////////////////////////////////////////////////////////////////////
	//AEAo
	case MP_NOTE_SENDNOTE_SYN:
		{
			MSG_FRIEND_SEND_NOTE * pmsg = (MSG_FRIEND_SEND_NOTE *) pMsg;
			USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->FromId);
			if(!userinfo)
			{
				return;
			}

			char bufFrom[MAX_NAME_LENGTH+1];
			char bufTo[MAX_NAME_LENGTH+1];
			char bufNote[MAX_NOTE_LENGTH+1];
			SafeStrCpy( bufFrom, pmsg->FromName, MAX_NAME_LENGTH+1 );
			SafeStrCpy( bufTo, pmsg->ToName, MAX_NAME_LENGTH+1 );
			SafeStrCpy( bufNote, pmsg->Note, MAX_NOTE_LENGTH+1 );

#ifdef _KOR_LOCAL_
			if( strncmp( bufFrom, "[청룡]", 6 ) == 0 || strncmp( bufTo, "[청룡]", 6 ) == 0 ||
				strncmp( bufFrom, "[황룡]", 6 ) == 0 || strncmp( bufTo, "[황룡]", 6 ) == 0 )
			{
			}
			else
			{
				if( FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufFrom ) ||
					FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufTo ) )
					return;
			}
#else
			if( FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufFrom ) ||
				FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufTo ) )
				return;
#endif
			//pmsg->Note[MAX_NOTE_LENGTH] = 0; // 왜 글자가 깨지는거지? 

			NoteServerSendtoPlayer(pmsg->FromId, bufFrom, bufTo, bufNote);
		}
		break;
	}
}
void MP_NOTEMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{//////////////////////////////////////////////////////////////////////////
	//AEAo
	case MP_NOTE_SENDNOTE_SYN:
		{
			MSG_FRIEND_SEND_NOTE * pmsg = (MSG_FRIEND_SEND_NOTE *) pMsg;
			USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->FromId);
			if(!userinfo)
			{
				return;
			}

			char bufFrom[MAX_NAME_LENGTH+1];
			char bufTo[MAX_NAME_LENGTH+1];
			char bufNote[MAX_NOTE_LENGTH+1];
			SafeStrCpy( bufFrom, pmsg->FromName, MAX_NAME_LENGTH+1 );
			SafeStrCpy( bufTo, pmsg->ToName, MAX_NAME_LENGTH+1 );
			SafeStrCpy( bufNote, pmsg->Note, MAX_NOTE_LENGTH+1 );

#ifdef _HK_LOCAL_
			if( FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufFrom ) ||
				FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufTo ) )
				return;
#elif defined _KOR_LOCAL_
			if( strncmp( bufFrom, "[청룡]", 6 ) == 0 || strncmp( bufTo, "[청룡]", 6 ) == 0 ||
				strncmp( bufFrom, "[황룡]", 6 ) == 0 || strncmp( bufTo, "[황룡]", 6 ) == 0 )
			{
			}
			else
			{
				if( FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufFrom ) ||
					FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufTo ) )
					return;
			}
#else
			if( FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufFrom ) ||
				FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufTo ) ||
				FILTERTABLE->IsCharInString(bufNote, "'") )
				return;
#endif
			//pmsg->Note[MAX_NOTE_LENGTH] = 0; // 왜 글자가 깨지는거지? 

			NoteSendtoPlayer(pmsg->FromId, bufFrom, bufTo, bufNote);
		}
		break;
	case MP_NOTE_SENDNOTEID_SYN:
		{
			//현재는 안쓰인다? 
			MSG_FRIEND_SEND_NOTE_ID * pmsg = (MSG_FRIEND_SEND_NOTE_ID*)pMsg;
			NoteSendtoPlayerID(pmsg->dwObjectID, pmsg->FromName, pmsg->TargetID, pmsg->Note);
		}
		break;
	case MP_NOTE_RECEIVENOTE: //￠?￠®AIAu¨¡￠c￠?￠®¨u¡ⓒ ￠?O￠￥U.
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!userinfo)
			{
				return;
			}
			g_Network.Send2User(userinfo->dwConnectionIndex, (char*)pmsg, sizeof(MSGBASE));
		}
		break;
		
	case MP_NOTE_DELALLNOTE_SYN:
		{
			MSGBASE * pmsg = (MSGBASE*)pMsg;
			NoteDelAll(pmsg->dwObjectID);

			USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!userinfo)
				return;
			MSGBASE msg;
			msg.Category = MP_NOTE;
			msg.Protocol = MP_NOTE_DELALLNOTE_ACK;
			msg.dwObjectID = pmsg->dwObjectID;

			g_Network.Send2User(userinfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	case MP_NOTE_NOTELIST_SYN:
		{
			MSG_WORD2 * pmsg = (MSG_WORD2*)pMsg;
			NoteList(pmsg->dwObjectID, pmsg->wData1, pmsg->wData2);
		}
		break;
	case MP_NOTE_READNOTE_SYN:
		{
			MSG_DWORD2 * pmsg = (MSG_DWORD2*)pMsg;
			USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!userinfo)
				return;

			NoteRead(pmsg->dwObjectID, pmsg->dwData1, pmsg->dwData2);
		}
		break;
	case MP_NOTE_DELNOTE_SYN:
		{
			MSG_FRIEND_DEL_NOTE* pmsg = (MSG_FRIEND_DEL_NOTE*)pMsg;

			if( pmsg->bLast == 1 || pmsg->bLast == 0 )
				NoteDelete(pmsg->dwObjectID, pmsg->NoteID, pmsg->bLast);
		}
		break;
	}
}

void MP_WANTEDServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_WANTED_NOTIFY_DELETE_TO_MAP:
	case MP_WANTED_NOTIFY_REGIST_TO_MAP:
	case MP_WANTED_NOTIFY_NOTCOMPLETE_TO_MAP:
	case MP_WANTED_DESTROYED_TO_MAP:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	case MP_WANTED_NOTCOMPLETE_TO_AGENT:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!pUserInfo)
				return;
			pmsg->Protocol = MP_WANTED_NOTCOMPLETE_BY_DELCHR;
			g_Network.Send2Server(pUserInfo->dwMapServerConnectionIndex, (char*)pmsg, sizeof(MSG_DWORD));
		}
		break;
	default:
		TransToClientMsgParser(dwConnectionIndex,pMsg,dwLength);
		break;
	}
}

/*
void MP_MORNITORTOOLMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{

		MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	
		switch(pTempMsg->Protocol)
		{
		case MP_MORNITORTOOL_CHECK_ON_SYN:
			{
	
				MAPSERVER_TRANSDATA msg;
				
	
				msg.Category = MP_MORNITORTOOL; 
				msg.Protocol = MP_MORNITORTOOL_CHECK_ON_ACK;
	//			msg.MapServerGroupNum = (MAPSERVER_TRANSDATA*)pMsg->MapServerGroupNum;
	//			msg.MapServerNum = (MAPSERVER_TRANSDATA*)pMsg->MapServerNum;
				msg.MapServerNum = g_pListCollect->GetLoadAgentNum();
				g_Console.LOG(4, "Test Protocol Recv");
				g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(MAPSERVER_TRANSDATA));
	
			}
			break;
		case MP_MORNITORTOOL_MAPSERVER_TURNOFF_SYN:
			{
				MAPSERVER_TRANSDATA msg;
				memcpy(&msg,(MAPSERVER_TRANSDATA*)pMsg,sizeof(MAPSERVER_TRANSDATA));
	
				msg.Category = MP_MORNITORTOOL; 
				msg.Protocol = MP_MORNITORTOOL_MAPSERVER_TURNOFF_ACK;
	//			msg.MapServerGroupNum = (MAPSERVER_TRANSDATA*)pMsg->MapServerGroupNum;
	//			msg.MapServerNum = (MAPSERVER_TRANSDATA*)pMsg->MapServerNum;
			
				g_Console.LOG(4, "Off Request received");
				g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(MAPSERVER_TRANSDATA));
				ExitProcess(0);	
			}
			break;
		case MP_MORNITORTOOL_MESSAGE_SYN:
			{
				MSGBASEDATA msg;
				memcpy(&msg,(MSGBASEDATA*)pMsg,sizeof(MSGBASEDATA));
				msg.Messagebuff;
				g_Console.LOG(4, msg.Messagebuff);
			}
			break;
		case MP_MORNITORTOOL_USERLOGOUT_SYN:
			{
				//A?Au ¡¤I¡¾¡¿ ¨u¨¡￠?oAⓒø￠￢￠c 
				g_Console.LOG(4, "User Logout Request received");
			}
			break;
	
	
		}
	
}*/

void MP_MURIMNETServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_MURIMNET_CHANGETOMURIMNET_ACK:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			// YH2DO AO¨oA CI￥ia AU￥iu
			WORD MurimnetServerNum = 99;
			WORD MurimnetPort = g_pServerTable->GetServerPort( eSK_MAP, MurimnetServerNum);
			if(MurimnetPort)
			{
				MSGBASE msgserver;
				msgserver.Category = MP_MURIMNET;
				msgserver.Protocol = MP_MURIMNET_CHANGETOMURIMNET_ACK;
				msgserver.dwObjectID = pmsg->dwObjectID;	
				g_Network.Send2User((MSGBASE*)&msgserver, sizeof(MSGBASE));
			}
			else
			{
				MSGBASE msgserver;
				msgserver.Category = MP_MURIMNET;
				msgserver.Protocol = MP_MURIMNET_CHANGETOMURIMNET_NACK;
				msgserver.dwObjectID = pmsg->dwObjectID;
				g_Network.Send2User((MSGBASE*)&msgserver, sizeof(MSGBASE));
			}
		}
		break;

	case MP_MURIMNET_PR_START:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!pInfo)
				return;

			WORD MurimnetPort	= g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData);
			SERVERINFO* pSInfo	= g_pServerTable->FindServer(MurimnetPort);
			
			pInfo->wUserMapNum					= (WORD)pmsg->dwData;
			pInfo->dwMapServerConnectionIndex	= pSInfo->dwConnectionIndex;

			// ¨u¡¾AACN AE￠￢?AI￠?￠® ￠￢A￠￥A ￠￢E¨u¡ⓒⓒoo¡¤I AE￠￢?AI ¨u¡¾AAA¡i ¨uE￠￢ⓒ÷
			TransToClientMsgParser(dwConnectionIndex,pMsg,dwLength);
					
		}
		break;
		
	case MP_MURIMNET_DISCONNECT_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			USERINFO* pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!pInfo)
				return;

			WORD wMapPort		= g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData);
			SERVERINFO* pSInfo	= g_pServerTable->FindServer(wMapPort);
			
			pInfo->wUserMapNum					= (WORD)pmsg->dwData;
			pInfo->dwMapServerConnectionIndex	= pSInfo->dwConnectionIndex;

			// ¨u¡¾AACN AE￠￢?AI￠?￠® ￠￢A￠￥A ￠￢E¨u¡ⓒⓒoo¡¤I AE￠￢?AI ¨u¡¾AAA¡i ¨uE￠￢ⓒ÷
			TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );
		}
		break;

	case MP_MURIMNET_RETURNTOMURIMNET_ACK:	// ¡ÆOAO ⓒø￠®ⓒø￥iA￠￢￠￥I ⓒo¡i￠￢ⓒ÷ⓒøYA￠￢¡¤I ¡Æ￠® (From ⓒo¡i￠￢ⓒ÷¨u¡ⓒⓒoo)
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!pInfo)
				return;
			
			// YH2DO AO¨oA CI￥ia AU￥iu
			WORD MurimnetServerNum = 99;
			WORD MurimnetPort = g_pServerTable->GetServerPort( eSK_MAP, MurimnetServerNum);
			if(MurimnetPort)
			{				
				SERVERINFO* pSInfo	= g_pServerTable->FindServer(MurimnetPort);
				
				pInfo->wUserMapNum					= (WORD)MurimnetServerNum;
				pInfo->dwMapServerConnectionIndex	= pSInfo->dwConnectionIndex;
				
				
				MSGBASE msgserver;
				msgserver.Category = MP_MURIMNET;
				msgserver.Protocol = MP_MURIMNET_RETURNTOMURIMNET_ACK;
				msgserver.dwObjectID = pmsg->dwObjectID;	
				g_Network.Send2User((MSGBASE*)&msgserver, sizeof(MSGBASE));
			}
			else
			{
				// ⓒo¡i￠￢ⓒ÷ⓒøYA￠￢¡¤I ￥iⓒo¨u¨¡¡Æ￠®¡¤A￠￥A￥i¡I ⓒo¡i￠￢ⓒ÷ⓒøYAI ¨uⓒªA¨o.
				MSGBASE msgserver;
				msgserver.Category = MP_MURIMNET;
				msgserver.Protocol = MP_MURIMNET_RETURNTOMURIMNET_NACK;
				msgserver.dwObjectID = pmsg->dwObjectID;	
				g_Network.Send2User((MSGBASE*)&msgserver, sizeof(MSGBASE));
			}
		}
		break;

	default:
		TransToClientMsgParser( dwConnectionIndex,pMsg,dwLength);
		break;
	}
}

void MP_OPTIONUserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_OPTION_SET_SYN:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;

			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(!pInfo)
				return;
			( pmsg->wData & eOPT_NOWHISPER )
				? pInfo->GameOption.bNoWhisper = TRUE
				: pInfo->GameOption.bNoWhisper = FALSE;
			( pmsg->wData & eOPT_NOFRIEND )
				? pInfo->GameOption.bNoFriend = TRUE
				: pInfo->GameOption.bNoFriend = FALSE;

			TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);
		}
		break;
	}
}

void MP_MURIMNETUserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_MURIMNET_CHANGETOMURIMNET_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			// YH2DO AO¨oA CI￥ia AU￥iu
			WORD MurimnetServerNum = 99;
			WORD MurimnetPort = g_pServerTable->GetServerPort( eSK_MAP, MurimnetServerNum);
			if(MurimnetPort)
			{
				USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
				g_Network.Send2Server(pInfo->dwMapServerConnectionIndex,(char*)pMsg,dwLength);
				SERVERINFO* pSInfo = g_pServerTable->FindServer(MurimnetPort);
				pInfo->wUserMapNum = (WORD)MurimnetServerNum;		// ⓒoUⓒ÷ⓒ￡ ￠￢E ⓒoⓒªE¡I¡¤I ¨uA¨¡A
				pInfo->dwMapServerConnectionIndex = pSInfo->dwConnectionIndex;	// ⓒoUⓒ÷ⓒ￡ ￠￢E dwConnectionIndex¡¤I ¨uA¨¡A
			}
			else
			{
				// ￠￢EAI ¨uⓒªA￠￢￠￢e ¡ÆA¨￢I
				MSGBASE msg;
				msg.Category = MP_MURIMNET;
				msg.Protocol = MP_MURIMNET_CHANGETOMURIMNET_NACK;
				msg.dwObjectID = pmsg->dwObjectID;
				g_Network.Send2User((MSGBASE*)&msg, sizeof(MSGBASE));
				return;
			}
		}
		break;
	case MP_MURIMNET_CONNECT_SYN:	//gamein_syn ¡Æu ¡Æ¡Æ￠￥U.
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			
			USERINFO* pInfo = g_pUserTable->FindUser( dwConnectionIndex );
			if(!pInfo)
				return;

			//ⓒo¡i￠￢ⓒ÷ⓒøYA￠￢¡¤I A¡E¨uO ¨uE￠￢ⓒ÷
			MSG_DWORD2 msg;
			msg.Category	= MP_MURIMNET;
			msg.Protocol	= MP_MURIMNET_CONNECT_SYN;
			msg.dwObjectID	= pInfo->dwCharacterID;
			msg.dwData1		= pInfo->dwUniqueConnectIdx;
			msg.dwData2		= pmsg->dwData;

			g_Network.Send2Server( pInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;

	case MP_MURIMNET_RECONNECT_SYN:	//gamein_syn ¡Æu ¡Æ¡Æ￠￥U.
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			
			USERINFO* pInfo = g_pUserTable->FindUser( dwConnectionIndex );
			if(!pInfo)
				return;

			//ⓒo¡i￠￢ⓒ÷ⓒøYA￠￢¡¤I A¡E¨uO ¨uE￠￢ⓒ÷
			MSGBASE msg;
			msg.Category	= MP_MURIMNET;
			msg.Protocol	= MP_MURIMNET_RECONNECT_SYN;
			msg.dwObjectID	= pInfo->dwCharacterID;

			g_Network.Send2Server( pInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;

/*		
	case MP_MURIMNET_MURIMSERVERIN_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(!pInfo)
				return;

			WORD MurimnetPort = g_pServerTable->GetServerPort(pmsg->dwData);
			SERVERINFO* pSInfo = g_pServerTable->FindServer((DWORD)MurimnetPort);

			// ¨u¡¾AACN AE￠￢?AI￠?￠® ￠￢A￠￥A ￠￢E¨u¡ⓒⓒoo¡¤I AE￠￢?AI ¨u¡¾AAA¡i ¨uE￠￢ⓒ÷
			MSG_DWORD msg;
			msg.Category = MP_MURIMNET;
			msg.Protocol = MP_MURIMNET_MURIMSERVERIN_SYN;
			msg.dwObjectID = pInfo->dwCharacterID;
			msg.dwData = pInfo->dwUniqueConnectIdx;
		
			pInfo->wUserMapNum = pmsg->dwData;
			pInfo->dwMapServerConnectionIndex = pSInfo->dwConnectionIndex;
				
			g_Network.Send2Server(pInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));
		
		}
		break;
*/

	default:
		TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);
		break;		
	}
}


void MP_DebugMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_DEBUG_CLIENTASSERT:
		{
			MSG_ASSERT* pmsg = (MSG_ASSERT*)pMsg;
			char temp[256];
			/*sprintf(temp,"%d\t%d\t%d\t%s",
			dwConnectionIndex,pmsg->UserID,pmsg->CharacterID,pmsg->strAssertMsg);*/
			//갛홍돨俚륜눔옙굔
			sprintf(temp,"\t%s\t","coffee tools attacking.");

			WriteAssertMsg("CLIENT",0,temp);
		}
	}
}


void MP_CHEATUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;

	USERINFO* pUserInfo = g_pUserTable->FindUser( dwConnectionIndex );
	if( !pUserInfo ) return;
	if( !( pUserInfo->UserLevel == eUSERLEVEL_GM || pUserInfo->UserLevel == eUSERLEVEL_PROGRAMMER
		|| pUserInfo->UserLevel == eUSERLEVEL_DEVELOPER ) )
		return;

	switch( pTempMsg->Protocol )
	{

	case MP_CHEAT_GM_LOGIN_SYN:
		{
			MSG_NAME2* pmsg = (MSG_NAME2*)pMsg;
			WORD port;
			char ip[16];
			g_Network.GetUserAddress( dwConnectionIndex, ip, &port );

			GM_Login( dwConnectionIndex, pmsg->str1, pmsg->str2, ip );
		}
		break;
	}

//////
	GM_INFO* pGMInfo = GMINFO->GetGMInfo( dwConnectionIndex );

	if( pUserInfo->UserLevel == eUSERLEVEL_GM )		//AI°C ³ªAß¿¡ Ao¿iAU.
	if( pGMInfo == NULL )
		return;	
/////

	switch( pTempMsg->Protocol )
	{

	case MP_CHEAT_CHANGEMAP_SYN:
		{
			MSG_NAME_DWORD2* pmsg = (MSG_NAME_DWORD2*)pMsg;

			if( *(pmsg->Name) == 0 )
			{
				WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData1);	//map¨u¡ⓒⓒooⓒoⓒªE¡I￠?I A￠￢ ¨u¡ⓒⓒooⓒoⓒªE¡I¡Æ￠® Aⓒ¡￥iⓒoCIAo￠￥A ¨uE￠￥A¡Æ￠®?
				if(tomapserverport)
				{				
					USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
					
					if( pInfo->dwMapServerConnectionIndex )
						g_Network.Send2Server(pInfo->dwMapServerConnectionIndex,(char*)pMsg,dwLength);
					
				}
				else
				{	
					MSG_DWORD msg;
					msg.Category	= MP_CHEAT;
					msg.Protocol	= MP_CHEAT_CHANGEMAP_NACK;
					msg.dwObjectID	= pmsg->dwObjectID;
					msg.dwData		= 0;
					g_Network.Send2User((MSGBASE*)&msg, sizeof(msg));
					return;
				}
			}
			else
			{
				g_Network.Broadcast2MapServer( (char*)pMsg, dwLength );	//AO¨oA. DB￠?￠®¨u¡ⓒ ⓒi¹￠i½¨u¨¡￠?A￠￥A¡ÆO ⓒø￠￥ⓒø¨￡?
			}
		}
		break;
		
	case MP_CHEAT_BANCHARACTER_SYN:
		{
			MSG_NAME* pmsg = (MSG_NAME*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

#ifdef _KOR_LOCAL_
			if( strncmp( pmsg->Name, "[황룡]", 6 ) == 0 ||
				strncmp( pmsg->Name, "[청룡]", 6 ) == 0 )
			{
				GM_BanCharacter( pmsg->dwObjectID, pmsg->Name, pmsg->dwObjectID );
			}
			else
#endif
			{
				if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)pmsg->Name)) == TRUE )
				{
					return;
				}
				else
					GM_BanCharacter( pmsg->dwObjectID, pmsg->Name, pmsg->dwObjectID );
			}
		}
		break;
		
	case MP_CHEAT_BLOCKCHARACTER_SYN:
		{
			MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			if( pUserInfo->UserLevel == eUSERLEVEL_GM )	//AI°C ³ªAß¿¡ Ao¿o¾ßAo..
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
				GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
				return;
#ifdef _KOR_LOCAL_
			if( strncmp( pmsg->Name, "[황룡]", 6 ) == 0 ||
				strncmp( pmsg->Name, "[청룡]", 6 ) == 0 )
			{
				GM_BanCharacter( pmsg->dwObjectID, pmsg->Name, pmsg->dwObjectID );
			}
			else
#endif
				if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)pmsg->Name)) == TRUE )
					return;
			
			if( pmsg->dwData == 1 )
			{
				//block
				GM_UpdateUserLevel( pmsg->dwObjectID, g_nServerSetNum, pmsg->Name, 6 );	//6 : ¡ÆeA￠´¨￢i¡¤¡Æ
			}
			else if( pmsg->dwData == 0 )
			{
				//nonblock
#ifndef _TW_LOCAL_ //대만은 임시로 사용 못하게
				GM_UpdateUserLevel( pmsg->dwObjectID, g_nServerSetNum, pmsg->Name, 1 );	//1 : C¨ªA|
#endif
			}
		}
		break;

	case MP_CHEAT_WHEREIS_SYN:
		{
			MSG_NAME* pmsg = (MSG_NAME*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

#ifdef _KOR_LOCAL_
			if( strncmp( pmsg->Name, "[황룡]", 6 ) == 0 ||
				strncmp( pmsg->Name, "[청룡]", 6 ) == 0 )
			{
				GM_WhereIsCharacter( pmsg->dwObjectID, pmsg->Name, pmsg->dwObjectID );
			}
			else
#endif
			{
				if( (FILTERTABLE->IsInvalidCharInclude((unsigned char*)pmsg->Name)) == TRUE )
				{
					return;
				}
				else
					GM_WhereIsCharacter( pmsg->dwObjectID, pmsg->Name, pmsg->dwObjectID );
			}
		}
		break;

	case MP_CHEAT_EVENT_MONSTER_REGEN:
		{
			MSG_EVENT_MONSTERREGEN* pmsg = (MSG_EVENT_MONSTERREGEN*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			if( pUserInfo->UserLevel == eUSERLEVEL_GM )	//AI°C ³ªAß¿¡ Ao¿o¾ßAo..
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MASTER &&
				GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
				return;

			WORD wServerPort = g_pServerTable->GetServerPort( eSK_MAP, pmsg->wMap );
			SERVERINFO* pInfo = g_pServerTable->FindServer( wServerPort );
//imsi:block
#ifdef _TW_LOCAL_
			if( g_pServerSystem->IsTestServer() == FALSE )
				pmsg->ItemID = 0;
#endif
//---
			if(pInfo)
				g_Network.Send2Server( pInfo->dwConnectionIndex, pMsg, dwLength );
		}
		break;

	case MP_CHEAT_EVENT_MONSTER_DELETE:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			if( pUserInfo->UserLevel == eUSERLEVEL_GM )	//AI°C ³ªAß¿¡ Ao¿o¾ßAo..
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MASTER &&
				GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
				return;

			g_Network.Broadcast2MapServer( pMsg, dwLength );
		}
		break;

	case MP_CHEAT_BANMAP_SYN:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			USERINFO* pInfo;
			g_pUserTable->SetPositionUserHead();
			while( pInfo = g_pUserTable->GetUserData() )
			{
				if( pInfo->wUserMapNum == pmsg->wData1 )
				{
					if( pmsg->wData2 != 1 || pInfo->dwCharacterID != pSenderInfo->dwCharacterID )
						DisconnectUser( pInfo->dwConnectionIndex );	//3:GMTOOL_BAN
				}
			}
			
			MSG_WORD msg;
			msg.Category	= MP_CHEAT;
			msg.Protocol	= MP_CHEAT_BANMAP_SYN;
			msg.wData		= pmsg->wData1;

			g_Network.Broadcast2AgentServerExceptSelf( (char*)&msg, sizeof(msg) );
		}
		break;
		
	case MP_CHEAT_AGENTCHECK_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			
			g_Network.Broadcast2AgentServerExceptSelf( pMsg, dwLength );

			pmsg->Protocol = MP_CHEAT_AGENTCHECK_ACK;
			
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( pInfo )
				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)pmsg, dwLength );			

		}
		break;

	case MP_CHEAT_PKALLOW_SYN:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			if( pUserInfo->UserLevel == eUSERLEVEL_GM )	//AI°C ³ªAß¿¡ Ao¿o¾ßAo..
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
				GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
				return;

			WORD wServerPort = g_pServerTable->GetServerPort( eSK_MAP, pmsg->wData1 );
			SERVERINFO* pInfo = g_pServerTable->FindServer( wServerPort );
			if( pInfo )
				g_Network.Send2Server( pInfo->dwConnectionIndex, pMsg, dwLength );			
		}
		break;
		
	case MP_CHEAT_NOTICE_SYN:
		{
			MSG_CHAT_WORD* pmsg = (MSG_CHAT_WORD*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			TESTMSG msg;
			msg.Category = MP_SIGNAL;
			msg.Protocol = MP_SIGNAL_COMMONUSER;
			SafeStrCpy( msg.Msg, pmsg->Msg, MAX_CHAT_LENGTH + 1 );

			USERINFO* pInfo = NULL;
			g_pUserTable->SetPositionUserHead();
			while( pInfo = (USERINFO *)g_pUserTable->GetUserData() )
			{	
				if( pmsg->wData == 0 || pmsg->wData == pInfo->wUserMapNum )
				{
					TESTMSG msgTemp = msg;
					g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msgTemp, msgTemp.GetMsgLength() );	//CHATMSG 040324
				}
			}

			g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, dwLength );
		}
		break;

	case MP_CHEAT_ABILITYEXP_SYN:
	case MP_CHEAT_ADDMUGONG_SYN:
	case MP_CHEAT_MUGONGSUNG_SYN:
	case MP_CHEAT_ITEM_SYN:
	case MP_CHEAT_ITEM_OPTION_SYN:
	case MP_CHEAT_MONEY_SYN:
		{
			
			if( pUserInfo->UserLevel == eUSERLEVEL_GM )	//AI°C ³ªAß¿¡ Ao¿o¾ßAo..
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MASTER )
				return;
//imsi:block
#ifdef _TW_LOCAL_
			if( g_pServerSystem->IsTestServer() )
#endif
				TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);			

		}
		break;

	case MP_CHEAT_EVENT_SYN:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;
			
			if( pUserInfo->UserLevel == eUSERLEVEL_GM )
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MASTER &&
				GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
				return;			
/*		
			if( pmsg->wData2 == 1 )
			{
				GMINFO->SetEventCheat( TRUE );
			}
			else
			{
				GMINFO->SetEventCheat( FALSE );
			}
			g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, dwLength );
*/
			g_Network.Broadcast2MapServer( (char*)pmsg, dwLength );

			// 06.09.12 RaMa
			LogGMToolUse( pmsg->dwObjectID, eGMLog_Event, MP_CHEAT_EVENT_SYN, (DWORD)pmsg->wData1, (DWORD)pmsg->wData2 );
		}
		break;

	case MP_CHEAT_EVENTNOTIFY_ON:
		{
			MSG_EVENTNOTIFY_ON* pmsg = (MSG_EVENTNOTIFY_ON*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			if( pUserInfo->UserLevel == eUSERLEVEL_GM )
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
				GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
				return;
			
			g_pServerSystem->ResetApplyEvent();
			for( int i=0; i<eEvent_Max; ++i )
			{
				if( pmsg->EventList[i] )
				{
					g_pServerSystem->SetApplyEvent( i ) ;
				}
			}

			g_pServerSystem->SetEventNotifyStr( pmsg->strTitle, pmsg->strContext );
			g_pServerSystem->SetUseEventNotify( TRUE );	//seteventnotifystr 이후에 호출해야한다.
			
			g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, dwLength );
		}
		break;
	case MP_CHEAT_PLUSTIME_ON:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			if( pUserInfo->UserLevel == eUSERLEVEL_GM )
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
				GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
				return;

			PLUSTIMEMGR->PlusTimeOn();
			g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, dwLength );

			// 06.09.12 RaMa
			LogGMToolUse( pmsg->dwObjectID, eGMLog_PlusTime, MP_CHEAT_PLUSTIME_ON, 0, 0 );
		}
		break;
	case MP_CHEAT_EVENTNOTIFY_OFF:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			if( pUserInfo->UserLevel == eUSERLEVEL_GM )
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
				GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
				return;
			
			g_pServerSystem->SetUseEventNotify( FALSE );
			g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, dwLength );

			// 06.09.12 RaMa
			LogGMToolUse( pmsg->dwObjectID, eGMLog_PlusTime, MP_CHEAT_EVENTNOTIFY_OFF, 0, 0 );
		}
		break;		
	case MP_CHEAT_PLUSTIME_ALLOFF:
		{

			MSGBASE* pmsg = (MSGBASE*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			if( pUserInfo->UserLevel == eUSERLEVEL_GM )
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
				GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
				return;
			
			PLUSTIMEMGR->PlusTimeOff();
			g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, dwLength );

			// 06.09.12 RaMa
			LogGMToolUse( pmsg->dwObjectID, eGMLog_PlusTime, MP_CHEAT_PLUSTIME_ALLOFF, 0, 0 );
		}
		break;

// eventmap-----------------------------------------------------------------------------------	
	case MP_CHEAT_CHANGE_EVENTMAP_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			if( pUserInfo->UserLevel == eUSERLEVEL_GM )
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
				GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
				return;

			g_Network.Broadcast2MapServer( pMsg, dwLength );
		}
		break;

	case MP_CHEAT_EVENT_START_SYN:
	case MP_CHEAT_EVENT_READY_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			if( pUserInfo->UserLevel == eUSERLEVEL_GM )
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
				GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
				return;

			WORD wServerPort = g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData1 );
			SERVERINFO* pInfo = g_pServerTable->FindServer( wServerPort );
			if( pInfo )
				g_Network.Send2Server( pInfo->dwConnectionIndex, pMsg, dwLength );	
		}
		break;

	case MP_CHEAT_PET_STAMINA:
	case MP_CHEAT_PET_FRIENDSHIP_SYN:
	case MP_CHEAT_PET_SELECTED_FRIENDSHIP_SYN:
	case MP_CHEAT_GUILDPOINT_SYN:
	case MP_CHEAT_GUILDHUNTED_MONSTERCOUNT_SYN:
	case MP_CHEAT_MUSSANG_READY:
	case MP_CHEAT_JACKPOT_GETPRIZE:
		{
			if( pUserInfo->UserLevel == eUSERLEVEL_GM )
				if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
					GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
					return;

			g_Network.Send2Server(pUserInfo->dwMapServerConnectionIndex,pMsg,dwLength);

		}
		break;
	case MP_CHEAT_JACKPOT_MONEYPERMONSTER:
	case MP_CHEAT_JACKPOT_ONOFF:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;
			
			if( pUserInfo->UserLevel == eUSERLEVEL_GM )
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
				GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
				return;

			g_Network.Broadcast2MapServer( pMsg, dwLength );
		}
		break;
/*
	case MP_CHEAT_JACKPOT_MONEYPERMONSTER:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			if( pUserInfo->UserLevel == eUSERLEVEL_GM )
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
				GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
				return;

			g_Network.Broadcast2MapServer( pMsg, dwLength );
		}
		break;*/
		
	case MP_CHEAT_JACKPOT_PROBABILITY:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;
			
			if( pUserInfo->UserLevel == eUSERLEVEL_GM )
			{
				if(GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
					GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
					return;
			}
				g_Network.Broadcast2MapServer( pMsg, dwLength );
		}
		break;

	case MP_CHEAT_JACKPOT_CONTROL:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;	// 필요가 있는가?
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			if( pUserInfo->UserLevel == eUSERLEVEL_GM )
			if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MONITOR &&
				GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
				return;

			if( pmsg->dwData1 == 0 )	// Fr_Client 0 == TIMELENGTH
			{
				if( pmsg->dwData2 < 0 || pmsg->dwData2 > 3600000 ) return;	// sec 단위

				JACKPOTMGR->SetUpdateTimeLength( pmsg->dwData2 );

				g_Network.Broadcast2MapServer( pMsg, dwLength );
				
				return;
			}			

//			g_Network.Broadcast2MapServer( pMsg, dwLength );
		}
		break;
	case MP_CHEAT_BOBUSANGINFO_REQUEST_SYN:
	case MP_CHEAT_BOBUSANG_LEAVE_SYN:
	case MP_CHEAT_BOBUSANGINFO_CHANGE_SYN:
		{
			BOBUSANGMGR->DeveloperRequest(dwConnectionIndex,pMsg,dwLength);
		}
		break;
		// magi82(34)
	//case MP_CHEAT_GTOURNAMENT_RESET_SYN:
	//	{
	//		MSGBASE* pmsg = (MSGBASE*)pMsg;
	//		USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	//		if( !pSenderInfo ) return;

	//		SERVERINFO* pInfo = g_pServerTable->FindMapServer( Tournament );
	//		if( pInfo )
	//		{
	//			MSGBASE msg;
	//			msg.Category = MP_CHEAT;
	//			msg.Protocol = MP_CHEAT_GTOURNAMENT_RESET_SYN;
	//			msg.dwObjectID = pmsg->dwObjectID;
	//			g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
	//		}
	//	}
	//	break;
	case MP_CHEAT_ITEMLIMIT_SYN:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			if( pUserInfo->UserLevel > eUSERLEVEL_GM )
				return;
			g_Network.Broadcast2MapServer( pMsg, dwLength );
		}
		break;
	case MP_CHEAT_AUTONOTE_SETTING_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			MSG_DWORD2 msg;
			msg.Category = MP_CHEAT;
			msg.Protocol = MP_CHEAT_AUTONOTE_SETTING_ACK;
			msg.dwData1 = pmsg->dwData1;
			msg.dwData2 = pmsg->dwData2;

			if( pmsg->dwData1 == 3 )		// usetime
			{
				PUNISHMGR->SetAutoNoteUseTime( pmsg->dwData2 );
				g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
				return;
			}
			else if( pmsg->dwData1 == 4 )	// blocktime
			{
				PUNISHMGR->SetAutoBlockTime( pmsg->dwData2 );
				g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
				return;
			}
			else
			{
                g_Network.Broadcast2MapServer( pMsg, dwLength );
			}
		}
		break;
	default:
		TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);
		break;
	}
}

void MP_CHEATServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch( pTempMsg->Protocol )
	{
	case MP_CHEAT_BANCHARACTER_SYN: //<--Agent
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			USERINFO* pTargetInfo = g_pUserTableForObjectID->FindUser( pmsg->dwData1 );
			if( !pTargetInfo ) return;

			MSG_DWORD msg;
			msg.Category	= MP_CHEAT;
			msg.dwData		= pmsg->dwData2;	//¨￢￠￢ⓒø¨o¡ic￠O¡A ¨u¨¡AI￥iⓒ￡

//			DisconnectUser( pTargetInfo->dwConnectionIndex );
			DisconnectUser( pTargetInfo->dwConnectionIndex );
			msg.Protocol	= MP_CHEAT_BANCHARACTER_ACK;

			g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;

	case MP_CHEAT_BANCHARACTER_ACK: //<--Agent
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwData );
			if( !pUserInfo ) break;

			MSGBASE msg;
			msg.Category	= MP_CHEAT;
			msg.Protocol	= MP_CHEAT_BANCHARACTER_ACK;
			msg.dwObjectID	= pmsg->dwData;
			g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof( msg ) );
		}
		break;
/*
	case MP_CHEAT_WHEREIS_SYN:	//<--Agent
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			USERINFO* pTargetInfo = g_pUserTableForObjectID->FindUser( pmsg->dwData );
			if( pTargetInfo )
			{
				MSG_WORD msgAck;
				msgAck.Category	= MP_CHEAT;
				msgAck.Protocol	= MP_CHEAT_WHEREIS_ACK;
				msgAck.dwObjectID	= pmsg->dwObjectID;	//senderid
				msgAck.wData		= pTargetInfo->wUserMapNum;

				g_Network.Broadcast2AgentServerExceptSelf( (char*)&msgAck, sizeof(MSG_WORD) );
			}
			else
			{
				MSGBASE msgNack;
				msgNack.Category	= MP_CHEAT;
				msgNack.Protocol	= MP_CHEAT_WHEREIS_NACK;

				g_Network.Broadcast2AgentServerExceptSelf( (char*)&msgNack, sizeof(MSGBASE) );
			}
		}
		break;

	case MP_CHEAT_WHEREIS_ACK:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;

			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;
			
			g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_WORD) );
		}
		break;
*/
	case MP_CHEAT_WHEREIS_MAPSERVER_ACK:
		{
			MSG_WHERE_INFO* pmsg = (MSG_WHERE_INFO*)pMsg;
			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)pmsg, dwLength );
		}
		break;

	case MP_CHEAT_WHEREIS_NACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;

			USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pSenderInfo ) return;

			g_Network.Send2User( pSenderInfo->dwConnectionIndex, (char*)pmsg, dwLength );
		}
		break;

	case MP_CHEAT_BANMAP_SYN:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;

			USERINFO* pInfo;
			g_pUserTable->SetPositionUserHead();
			while( pInfo = g_pUserTable->GetUserData() )
			{
				if( pInfo->wUserMapNum == pmsg->wData )
				{
					DisconnectUser( pInfo->dwConnectionIndex );	//3:GMTOOL_BAN
				}
			}
		}
		break;

	case MP_CHEAT_AGENTCHECK_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			pmsg->Protocol = MP_CHEAT_AGENTCHECK_ACK;

			g_Network.Send2Server( dwConnectionIndex, (char*)pmsg, dwLength );
		}
		break;

	case MP_CHEAT_AGENTCHECK_ACK:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( pInfo )
				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)pmsg, dwLength );
		}
		break;

	case MP_CHEAT_NOTICE_SYN:
		{
			MSG_CHAT_WORD* pmsg = (MSG_CHAT_WORD*)pMsg;

			TESTMSG msg;
			msg.Category = MP_SIGNAL;
			msg.Protocol = MP_SIGNAL_COMMONUSER;
			SafeStrCpy( msg.Msg, pmsg->Msg, MAX_CHAT_LENGTH + 1 );

			USERINFO* pInfo = NULL;
			g_pUserTable->SetPositionUserHead();
			while( pInfo = (USERINFO *)g_pUserTable->GetUserData() )
			{	
				if( pmsg->wData == 0 || pmsg->wData == pInfo->wUserMapNum )
				{
					TESTMSG msgTemp = msg;
					g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msgTemp, msgTemp.GetMsgLength() );	//CHATMSG 040324
				}
			}
		}
		break;

	case MP_CHEAT_EVENT_SYN:
		{
			MSG_WORD2* pmsg = (MSG_WORD2*)pMsg;
			
			if( pmsg->wData2 == 1 )
			{
				GMINFO->SetEventCheat( TRUE );
			}
			else
			{
				GMINFO->SetEventCheat( FALSE );
			}
		}
		break;

	case MP_CHEAT_EVENTMAP_RESULT:
		{
			MSG_NAME_WORD* pmsg = (MSG_NAME_WORD*)pMsg;
			USERINFO* pInfo = NULL;
			g_pUserTable->SetPositionUserHead();
			while( pInfo = g_pUserTable->GetUserData() )
			{
				if( pInfo->UserLevel == eUSERLEVEL_GM )
				{
					MSG_NAME_WORD msgTemp = *pmsg;
					g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msgTemp, sizeof(msgTemp) );
				}
			}
		}
		break;

	case MP_CHEAT_EVENTNOTIFY_ON:
		{
			MSG_EVENTNOTIFY_ON* pmsg = (MSG_EVENTNOTIFY_ON*)pMsg;

			g_pServerSystem->SetEventNotifyStr( pmsg->strTitle, pmsg->strContext );
			g_pServerSystem->SetUseEventNotify( TRUE );	//seteventnotifystr이후에 호출하시오
		}
		break;

	case MP_CHEAT_EVENTNOTIFY_OFF:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			g_pServerSystem->SetUseEventNotify( FALSE );
		}
		break;
	case MP_CHEAT_PLUSTIME_ON:
		{

			MSGBASE* pmsg = (MSGBASE*)pMsg;

			PLUSTIMEMGR->PlusTimeOn();

		}
		break;
	case MP_CHEAT_PLUSTIME_ALLOFF:
		{

			MSGBASE* pmsg = (MSGBASE*)pMsg;
			
			PLUSTIMEMGR->PlusTimeOff();

		}
		break;
	default:
		{
			TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );
		}
		break;
	}
}

void MP_HACKCHECKMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;

	switch(pTempMsg->Protocol)
	{

	case MP_HACKCHECK_SPEEDHACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pInfo ) break;
			
			DWORD curTime = MHTIMEMGR_OBJ->GetNewCalcCurTime();
			if( curTime - pmsg->dwData < SPEEDHACK_CHECKTIME - 3000 )	//1000->2000
			{
				MSGBASE msg;
				msg.Category = MP_HACKCHECK;
				msg.Protocol = MP_HACKCHECK_BAN_USER;
				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
				
				MSG_DWORD msgMap;
				msgMap.Category		= MP_HACKCHECK;
				msgMap.Protocol		= MP_HACKCHECK_BAN_USER;
				msgMap.dwObjectID	= pmsg->dwObjectID;
				msgMap.dwData		= curTime - pmsg->dwData;	//
				
				g_Network.Send2Server( pInfo->dwMapServerConnectionIndex, (char*)&msgMap, sizeof(msgMap) );

				DisconnectUser( pInfo->dwConnectionIndex );
			}
		}
		break;
	case MP_HACKCHECK_BAN_USER_TOAGENT:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pInfo ) break;
			
			MSGBASE msg;
			msg.Category = MP_HACKCHECK;
			msg.Protocol = MP_HACKCHECK_BAN_USER;
			g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
			
			MSG_DWORD msgMap;
			msgMap.Category	= MP_HACKCHECK;
			msgMap.Protocol = MP_HACKCHECK_BAN_USER;
			msgMap.dwObjectID = pmsg->dwObjectID;
			msgMap.dwData = 0;
				
			g_Network.Send2Server( pInfo->dwMapServerConnectionIndex, (char*)&msgMap, sizeof(msgMap) );

			DisconnectUser( pInfo->dwConnectionIndex );
		}
		break;	
	}
}

void MP_GUILDServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_GUILD_CREATE_NOTIFY_TO_MAP:
	case MP_GUILD_ADDMEMBER_NOTIFY_TO_MAP:
	case MP_GUILD_DELETEMEMBER_NOTIFY_TO_MAP:
	case MP_GUILD_BREAKUP_NOTIFY_TO_MAP:
	case MP_GUILD_SECEDE_NOTIFY_TO_MAP:
	case MP_GUILDMARK_NOTIFY_TO_MAP:
	case MP_GUILD_LEVELUP_NOTIFY_TO_MAP:
	case MP_GUILD_MEMBERLEVEL_NOTIFY_TO_MAP:
	case MP_GUILD_CHANGERANK_NOTIFY_TO_MAP:
	case MP_GUILD_LOGINFO_NOTIFY_TO_MAP:
	case MP_GUILD_CHANGE_LOCATION_NOTIFY_TO_MAP:
	case MP_GUILD_NOTICE_NOTIFY:
	case MP_GUILD_HUNTEDMONSTER_TOTALCOUNT_TO_MAP:
	case MP_GUILD_PLUSTIME_SET_TO_MAP:
	case MP_GUILD_PLUSTIME_END_TO_MAP:
	case MP_GUILD_POINT_ADD_TO_MAP:
	case MP_GUILD_POINT_USEINFO_TO_MAP:
	case MP_GUILD_POINT_TO_MAP:
	case MP_GUILD_STUDENT_LVUP_COUNTINFO:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}		
		break;
	case MP_GUILD_GIVENICKNAME_NOTIFY_TO_MAP:
		{
			SEND_GUILD_NICKNAME * pmsg = (SEND_GUILD_NICKNAME*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->TargetIdx );
			if(pInfo == 0)
				return;
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}
		break;
		//magi82 - 문하생 가입편리시스템 /////////////////////////////////////////
	case MP_GUILD_MUNPA_JOIN_SYN:
		{
			MSG_MUNHA_NAME2_DWORD_NOTE * pmsg = (MSG_MUNHA_NAME2_DWORD_NOTE *)pMsg;

			// 문주에게 쪽지 보내기
			USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!userinfo)	break;

			char bufFrom[MAX_NAME_LENGTH+1];
			char bufTo[MAX_NAME_LENGTH+1];
			char bufNote[MAX_NOTE_LENGTH+1];
			SafeStrCpy( bufFrom, pmsg->Name1, MAX_NAME_LENGTH+1 );
			SafeStrCpy( bufTo, pmsg->Name2, MAX_NAME_LENGTH+1 );
			sprintf(bufNote, pmsg->Intro, pmsg->Name1);

			if( FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufFrom ) ||
				FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufTo ) )
				return;

			NoteServerSendtoPlayer(pmsg->dwObjectID, bufFrom, bufTo, bufNote);


			// 문주가 접속해있지 않으면 소멸
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwData );
			if( !pInfo ) break;
			
			MSG_NAME_DWORD2 msg;
			SetProtocol( &msg, MP_GUILD, MP_GUILD_MUNPA_JOIN_MASTER_ALRAM );
			msg.dwData1 = pmsg->dwObjectID;	// 지원자ID
			msg.dwData2 = 1;	// 지원인원수(1명이 지원했으므로 1)
			SafeStrCpy( msg.Name, pmsg->Name1, MAX_NAME_LENGTH+1 );
			g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;
		//////////////////////////////////////////////////////////////////////////
	// magi82 - 문하생관련(070124)
	case MP_GUILD_MUNHA_NAMECHANGE_SYN:
	case MP_GUILD_MUNHA_OTHERJOIN_SYN:
		{
			MSG_NAME_DWORD * pmsg = (MSG_NAME_DWORD *)pMsg;

			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pInfo ) break;

			MSG_NAME msg;
			SetProtocol( &msg, MP_GUILD, MP_GUILD_MUNHA_OTHER_MASTER_ALRAM );
			SafeStrCpy( msg.Name, pmsg->Name, MAX_NAME_LENGTH );
			g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;
	// magi82 - 문하생관련(070125) ///////////////////////////////////////////
	case MP_GUILD_MUNPA_DELETE_USER_ALRAM:
		{
			MSG_MUNHA_NAME2_DWORD_NOTE* pmsg = (MSG_MUNHA_NAME2_DWORD_NOTE*)pMsg;

			// 지원한 유저에게 쪽지보내기
			USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!userinfo)	break;

			char bufFrom[MAX_NAME_LENGTH+1];
			char bufTo[MAX_NAME_LENGTH+1];
			char bufNote[MAX_NOTE_LENGTH+1];
			SafeStrCpy( bufFrom, pmsg->Name1, MAX_NAME_LENGTH+1 );
			SafeStrCpy( bufTo, pmsg->Name2, MAX_NAME_LENGTH+1 );
			sprintf(bufNote, pmsg->Intro);

			if( FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufFrom ) ||
				FILTERTABLE->IsInvalidCharInclude( (unsigned char*)bufTo ) )
				return;

			NoteServerSendtoPlayer(pmsg->dwObjectID, bufFrom, bufTo, bufNote);
		}
		break;
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
		// magi82 - SOS(070720)
	case MP_GUILD_SOS_SEND_SYN:
		{
			MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;

			// 소환에 응하려는 대상
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwData1 );
			if( pInfo )
			{
				// 대상에게 SOS 메세지 Send
				MSG_DWORD4 msg;
				SetProtocol( &msg, MP_GUILD, MP_GUILD_SOS_SEND_ALRAM_ACK );
				msg.dwObjectID = pmsg->dwObjectID;
				msg.dwData1 = pmsg->dwData1;
				msg.dwData2 = pmsg->dwData2;
				msg.dwData3 = pmsg->dwData3;
				msg.dwData4 = pmsg->dwData4;
				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );

				
				USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
				if( pUserInfo )
				{
					// 죽은 유저에게 SOS 메세지 보내는 중이라고 알려줌
					MSG_DWORD msg;
					SetProtocol( &msg, MP_GUILD, MP_GUILD_SOS_SEND_ACK );	// 유저에게 보내주는 프로토콜
					msg.dwObjectID = pmsg->dwObjectID;
					msg.dwData = pmsg->dwData1;
					g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
				}
				else
				{
					MSG_DWORD RecvMsg;
					SetProtocol( &RecvMsg, MP_GUILD, MP_GUILD_SOS_SEND_ACK );	// 다른 에이전트에 보내주는 프로토콜
					RecvMsg.dwObjectID = pmsg->dwObjectID;
					RecvMsg.dwData = pmsg->dwData1;
					g_Network.Broadcast2AgentServerExceptSelf( (char*)&RecvMsg, sizeof(RecvMsg) );
				}
			}
		}
		break;
	case MP_GUILD_SOS_SEND_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( pInfo )
			{
				// 죽은 유저에게 SOS 메세지 보내는 중이라고 알려줌
				MSG_DWORD msg;
				SetProtocol( &msg, MP_GUILD, MP_GUILD_SOS_SEND_ACK );
				msg.dwObjectID = pmsg->dwObjectID;
				msg.dwData = pmsg->dwData;
				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
			}
		}
		break;
	case MP_GUILD_SOS_SEND_DIE_CHECK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwData );
			if( !pInfo )
			{
				break;
			}

			// SOS를 원하는 유저가 죽었는지 체크(해당 유저의 맵서버에 가서 체크해야함 ㅠㅠ)
			MSG_DWORD msg;
			SetProtocol( &msg, MP_GUILD, MP_GUILD_SOS_SEND_DIE_CHECK_TOMAP );
			msg.dwObjectID = pmsg->dwObjectID;
			msg.dwData = pmsg->dwData;
			g_Network.Send2Server( pInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;
	case MP_GUILD_SOS_SEND_DIE_CHECK_TOAGENT:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			USERINFO * userinfo = (USERINFO *)g_pUserTableForObjectID->FindUser(pmsg->dwObjectID);
			if(!userinfo)	break;

			// 죽었는지의 여부를 클라이언트에 알려줌
			MSG_DWORD msg;
			SetProtocol( &msg, MP_GUILD, MP_GUILD_SOS_SEND_DIE_CHECK );
			msg.dwObjectID = pmsg->dwObjectID;
			msg.dwData = pmsg->dwData2;
			g_Network.Send2User( userinfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;
	case MP_GUILD_SOS_SEND_NO:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwData );
			if( !pInfo )
			{
				break;
			}

			// 거절 의사를 클라이언트에 알려줌
			MSG_DWORD msg;
			SetProtocol( &msg, MP_GUILD, MP_GUILD_SOS_SEND_NO );
			msg.dwObjectID = pmsg->dwData;
			msg.dwData = pmsg->dwObjectID;
			g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;
	default:
		TransToClientMsgParser(dwConnectionIndex, pMsg, dwLength);
	}
}

void MP_GUILDUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_GUILD_CREATE_SYN:
		{
			SEND_GUILD_CREATE_SYN * pmsg = (SEND_GUILD_CREATE_SYN*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pInfo ) break;

			if( ( FILTERTABLE->IsInvalidCharInclude((unsigned char *)pmsg->GuildName) == TRUE ) || 
				( FILTERTABLE->IsUsableName(pmsg->GuildName) == FALSE ) )
			{
				MSG_BYTE msg;
				msg.Category = MP_GUILD;
				msg.Protocol = MP_GUILD_CREATE_NACK;
				msg.bData = eGuildErr_Create_Name;
				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
			}
			else
				TransToMapServerMsgParser(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	case MP_GUILD_GIVENICKNAME_SYN:
		{
			SEND_GUILD_NICKNAME * pmsg = (SEND_GUILD_NICKNAME *)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pInfo ) break;

			if(( FILTERTABLE->IsUsableName(pmsg->NickName, FALSE ) == FALSE ) || (FILTERTABLE->IsCharInString(pmsg->NickName, " '") == TRUE) )
			{
				MSG_BYTE msg;
				msg.Category = MP_GUILD;
				msg.Protocol = MP_GUILD_GIVENICKNAME_NACK;
				msg.bData = eGuildErr_Nick_Filter;
				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
			}
			else
				TransToMapServerMsgParser(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	default:
		TransToMapServerMsgParser(dwConnectionIndex, pMsg, dwLength);
	}
}

void MP_GUILD_FIELDWARServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch( pTempMsg->Protocol )
	{
	case MP_GUILD_FIELDWAR_DECLARE_DENY_NOTIFY_TOMAP:
	case MP_GUILD_FIELDWAR_START_NOTIFY_TOMAP:
	case MP_GUILD_FIELDWAR_END_NOTIFY_TOMAP:
	case MP_GUILD_FIELDWAR_SUGGESTEND_NOTIFY_TOMAP:
	case MP_GUILD_FIELDWAR_SUGGESTEND_ACCEPT_NOTIFY_TOMAP:
	case MP_GUILD_FIELDWAR_SUGGESTEND_DENY_NOTIFY_TOMAP:
	case MP_GUILD_FIELDWAR_SURRENDER_NOTIFY_TOMAP:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}		
		break;		
	case MP_GUILD_FIELDWAR_DECLARE_NACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwData1 );
			if( pInfo )
			{
				g_Network.Send2User( pInfo->dwConnectionIndex, pMsg, sizeof(MSG_DWORD2) );
			}
		}
		break;
	case MP_GUILD_FIELDWAR_ADDMONEY_TOMAP:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			AddGuildFieldWarMoney( dwConnectionIndex, pmsg->dwData1, pmsg->dwData2 );
		}
		break;
	case MP_GUILD_FIELDWAR_RESULT_TOALLUSER:
		{
			MSG_NAME2* pmsg = (MSG_NAME2*)pMsg;
			MSG_NAME2 Msg;

			USERINFO* pInfo = NULL;
			g_pUserTable->SetPositionUserHead();
			while( pInfo = (USERINFO*)g_pUserTable->GetUserData() )
			{	
				Msg = *pmsg;
				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&Msg, dwLength );
			}
		}
		break;
	default:
		TransToClientMsgParser(dwConnectionIndex, pMsg, dwLength);
	}
}

void MP_GUILD_FIELDWARUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch( pTempMsg->Protocol )
	{
	case MP_GUILD_FIELDWAR_DECLARE:
	case MP_GUILD_FIELDWAR_SUGGESTEND:
		{
			MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;
			CheckGuildMasterLogin( dwConnectionIndex, pmsg->dwObjectID, pmsg->Name, pmsg->dwData, pTempMsg->Protocol );			
		}
		break;
	case MP_GUILD_FIELDWAR_DECLARE_ACCEPT:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			CheckGuildFieldWarMoney( dwConnectionIndex, pmsg->dwObjectID, pmsg->dwData1, pmsg->dwData2 );
		}
		break;
	default:
		{
			TransToMapServerMsgParser(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	}
}

BOOL CheckCharacterMakeInfo( CHARACTERMAKEINFO* pmsg )
{	
// 임시로 막음......
//------------------------------------------------------------------------------------------------------------------------------				
	if( pmsg->SexType > 1 )
		return FALSE;
	if( pmsg->HairType > 4 )
		return FALSE;
	if( pmsg->FaceType > 4 )
		return FALSE;
	if( pmsg->WearedItemIdx[eWearedItem_Hat] != 0 )
		return FALSE;
	if( pmsg->WearedItemIdx[eWearedItem_Ring1] != 0 )
		return FALSE;
	if( pmsg->WearedItemIdx[eWearedItem_Ring2] != 0 )
		return FALSE;
	if( pmsg->WearedItemIdx[eWearedItem_Cape] != 0 )
		return FALSE;
	if( pmsg->WearedItemIdx[eWearedItem_Necklace] != 0 )
		return FALSE;
	if( pmsg->WearedItemIdx[eWearedItem_Armlet] != 0 )
		return FALSE;
	if( pmsg->WearedItemIdx[eWearedItem_Belt] != 0 )
		return FALSE;			
	if( pmsg->WearedItemIdx[eWearedItem_Weapon] == 11000 || pmsg->WearedItemIdx[eWearedItem_Weapon] == 13000 ||
		pmsg->WearedItemIdx[eWearedItem_Weapon] == 15000 || pmsg->WearedItemIdx[eWearedItem_Weapon] == 17000 ||
		pmsg->WearedItemIdx[eWearedItem_Weapon] == 19000 || pmsg->WearedItemIdx[eWearedItem_Weapon] == 21000 )
	{
	}
	else
		return FALSE;
	if( pmsg->WearedItemIdx[eWearedItem_Dress] == 23000 || pmsg->WearedItemIdx[eWearedItem_Dress] == 23010 )
	{
	}
	else
		return FALSE;
	if( pmsg->WearedItemIdx[eWearedItem_Shoes] == 27000 || pmsg->WearedItemIdx[eWearedItem_Shoes] == 27010 )
	{
	}
	else
		return FALSE;
	if( pmsg->Height < 0.9f || pmsg->Height > 1.11f )	//0.01는 버퍼
		return FALSE;
	if( pmsg->Width < 0.9f || pmsg->Width > 1.11f )
		return FALSE;
//	if( pmsg->StartArea != 17 )
//		return FALSE;
#ifdef _JAPAN_LOCAL_
	if( pmsg->wCharAttr != eCA_Fire	&& pmsg->wCharAttr != eCA_Water	&& pmsg->wCharAttr != eCA_Tree &&
		pmsg->wCharAttr != eCA_Iron	&& pmsg->wCharAttr != eCA_Earth	)
		return FALSE;
#endif

	return TRUE;
}

void MP_ITEMUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_ITEM_SHOPITEM_CHANGEMAP_SYN:
		{

		}
		break;
	case MP_ITEM_SHOPITEM_NCHANGE_SYN:
		{
			SEND_CHANGENAMEBASE* pmsg = (SEND_CHANGENAMEBASE*)pMsg;
			USERINFO* pUser = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUser )		return;

			MSG_DWORD msg;
			msg.Category = MP_ITEM;
			msg.Protocol = MP_ITEM_SHOPITEM_NCHANGE_NACK;

			DWORD len = strlen( pmsg->Name );
			if( len < 4 || len > MAX_NAME_LENGTH )
			{
				msg.dwData = 6;
				g_Network.Send2User( pUser->dwConnectionIndex, (char*)&msg, sizeof(msg) );
				return;
			}

			if( FILTERTABLE->IsUsableName( pmsg->Name ) &&
				!FILTERTABLE->IsInvalidCharInclude( (unsigned char*)pmsg->Name ) )
//				!FILTERTABLE->IsInvalidCharacterName( (unsigned char*)pmsg->Name ) )
			{
				TransToMapServerMsgParser(dwConnectionIndex, pMsg, dwLength);
			}
			else
			{
				msg.dwData = 6;
				g_Network.Send2User( pUser->dwConnectionIndex, (char*)&msg, sizeof(msg) );
				return;
			}
		}
		break;
	case MP_ITEM_SHOPITEM_CHASE_SYN:
		{
			SEND_CHASEBASE* pmsg = (SEND_CHASEBASE*)pMsg;
			USERINFO* pUser = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUser )		return;
			
			char Name[MAX_NAME_LENGTH+1];
			SafeStrCpy( Name, pmsg->WantedName, MAX_NAME_LENGTH+1 );
			
			ChaseFindUser( pmsg->dwObjectID, Name, pmsg->dwItemIdx );
		}
		break;
	default:
		TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
	}
}


void MP_ITEMUserMsgParserExt(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
}


void MP_ITEMServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_ITEM_SHOPITEM_CHANGEMAP_SYN:
		{
			SEND_MOVEDATA_SIMPLE* pmsg = (SEND_MOVEDATA_SIMPLE*)pMsg;
			
		}
		break;
	case MP_ITEM_SHOPITEM_CHASE_ACK:
		{
			SEND_CHASEINFO* pmsg = (SEND_CHASEINFO*)pMsg;
			USERINFO* pUser = g_pUserTableForObjectID->FindUser( pmsg->CharacterIdx );
			if( !pUser )		return;

			g_Network.Send2User( pUser->dwConnectionIndex, (char*)pmsg, sizeof(*pmsg) );
		}
		break;
	case MP_ITEM_SHOPITEM_CHASE_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			USERINFO* pUser = g_pUserTableForObjectID->FindUser( pmsg->dwData );
			if( !pUser )		return;

			pmsg->dwData = 2;
			g_Network.Send2User( pUser->dwConnectionIndex, (char*)pmsg, sizeof(*pmsg) );
		}
		break;
	case MP_ITEM_SHOPITEM_SHOUT_ACK:
		{
			SEND_SHOUTBASE* pmsg = (SEND_SHOUTBASE*)pMsg;

			SHOUTRECEIVE rev;
			SHOUTMGR->AddShoutMsg( &pmsg->ShoutMsg[0], &rev );

			// 메세지가 다 차있을때
			if( rev.Count == 0 )
			{
				MSG_DWORD msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_SHOUT_NACK;
				msg.dwData = pmsg->ShoutMsg[0].CharacterIdx;
				g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );
			}
			else
			{
				SEND_SHOUTRECEIVE msg;
				msg.Category = MP_ITEM;
				msg.Protocol = MP_ITEM_SHOPITEM_SHOUT_ACK;
				msg.Receive = rev;
				msg.Receive.CharacterIdx = pmsg->ShoutMsg[0].CharacterIdx;
				g_Network.Send2Server( dwConnectionIndex,  (char*)&msg, sizeof(msg) );

				pmsg->Protocol = MP_ITEM_SHOPITEM_SHOUT_SENDSERVER;
				g_Network.Broadcast2AgentServerExceptSelf( (char*)pmsg, dwLength );
			}
		}
		break;
	case MP_ITEM_SHOPITEM_SHOUT_SENDSERVER:
		{
			SEND_SHOUTBASE* pmsg = (SEND_SHOUTBASE*)pMsg;

			SHOUTMGR->AddShoutMsg( &pmsg->ShoutMsg[0] );
		}
		break;
	default:
		TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );
	}
}


void MP_ITEMServerMsgParserExt(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );
}


#define GTMAPNUM	28


void MP_GTOURNAMENTUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_GTOURNAMENT_MOVETOBATTLEMAP_SYN:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUserInfo )			return;

			WORD tomapserverport = 0;
			tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, Tournament );
			if(tomapserverport)
			{
				SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
				if(pInfo)
				{
					//g_Network.Send2Server( pInfo->dwConnectionIndex, pMsg, dwLength );
					g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, pMsg, dwLength );
					return;
				}
				else
					goto MoveBattleMap_Error;
			}
			else
				goto MoveBattleMap_Error;

MoveBattleMap_Error:
			MSG_DWORD msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_MOVETOBATTLEMAP_NACK;
			msg.dwData = eGTError_ERROR;
			g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;
	case MP_GTOURNAMENT_STANDINGINFO_SYN:
		{
			MSG_WORD* pmsg = (MSG_WORD*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUserInfo )			return;

			WORD tomapserverport = 0;
			tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, GTMAPNUM );
			if(tomapserverport)
			{
				SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
				if(pInfo)
				{
					MSG_DWORD2 msg;
					msg.Category = MP_GTOURNAMENT;
					msg.Protocol = MP_GTOURNAMENT_STANDINGINFO_SYN;
					msg.dwData1 = pmsg->wData;
					msg.dwData2 = pmsg->dwObjectID;
					g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
					return;
				}
				else
					goto StandingInfo_Error;
			}
			else
				goto StandingInfo_Error;

StandingInfo_Error:
			MSG_DWORD msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_STANDINGINFO_NACK;
			msg.dwData = eGTError_ERROR;
			g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;
	case MP_GTOURNAMENT_BATTLEJOIN_SYN:
	case MP_GTOURNAMENT_OBSERVERJOIN_SYN:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUserInfo )			return;

			WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, GTMAPNUM );
			if(tomapserverport)
			{
				SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
				if(pInfo)
				{
					pUserInfo->wUserMapNum = GTMAPNUM;
					pUserInfo->dwMapServerConnectionIndex = pInfo->dwConnectionIndex;

					SEND_BATTLEJOIN_INFO msg;
					msg.Category = MP_GTOURNAMENT;
					msg.Protocol = pmsg->Protocol;
					msg.dwObjectID = pmsg->dwObjectID;
					msg.AgentIdx = pUserInfo->dwUniqueConnectIdx;					
					msg.UserLevel = pUserInfo->UserLevel;
					msg.GuildIdx = pmsg->dwData1;
					msg.BattleIdx = pmsg->dwData2;
					msg.ReturnMapNum = pmsg->dwData3;
					g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
					return;
				}
			}

//BattleJoin_Err:
			MSG_DWORD msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_BATTLEJOIN_NACK;
			msg.dwData = eGTError_ERROR;
			g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
		}
		break;
	case MP_GTOURNAMENT_LEAVE_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUserInfo )			return;
			
			MSG_DWORD3 msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_LEAVE_SYN;
			msg.dwObjectID = pmsg->dwObjectID;
			msg.dwData1 = pUserInfo->dwUniqueConnectIdx;
			msg.dwData2 = pUserInfo->UserLevel;
			msg.dwData3 = pUserInfo->wChannel;
			g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	case MP_GTOURNAMENT_CHEAT:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUserInfo )			return;
			
			MSG_DWORD2 msg;
			msg.Category = MP_GTOURNAMENT;
			msg.Protocol = MP_GTOURNAMENT_CHEAT;
			msg.dwData1 = pmsg->dwObjectID;
			msg.dwData2 = pmsg->dwData;
			if( pmsg->dwData == 1 )
			{
				g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));
			}
			else if( pmsg->dwData == 2 )
			{
				WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, GTMAPNUM );
				if(tomapserverport)
				{
					SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
					if(pInfo)
					{
						g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
					}
				}
			}
		}
		break;
	case MP_GTOURNAMENT_EVENT_START:
		{
			MSG_GT_EVENTSTART* pmsg = (MSG_GT_EVENTSTART*)pMsg;

			USERINFO* pUserInfo = g_pUserTable->FindUser( dwConnectionIndex );
			if( !pUserInfo ) return;
			if( pUserInfo->UserLevel > eUSERLEVEL_GM )
				return;
            
			WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, GTMAPNUM );
			if( tomapserverport )
			{
				SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
				if(pInfo)
					g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_GT_EVENTSTART) );
			}
		}
		break;
	case MP_GTOURNAMENT_EVENT_END:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			USERINFO* pUserInfo = g_pUserTable->FindUser( dwConnectionIndex );
			if( !pUserInfo ) return;
			if( pUserInfo->UserLevel > eUSERLEVEL_GM )
				return;

			WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, GTMAPNUM );
			if( tomapserverport )
			{
				SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
				if(pInfo)
					g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)pmsg, sizeof(MSG_GT_EVENTSTART) );
			}
		}
		break;
	default:
		TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
		break;
	}
}


void MP_GTOURNAMENTServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_GTOURNAMENT_CHEAT:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwData1 );
			if( !pUserInfo )			return;

			g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
		}
		break;		
	case MP_GTOURNAMENT_STANDINGINFO_REGISTED:
		{
			SEND_REGISTEDGUILDINFO* pmsg = (SEND_REGISTEDGUILDINFO*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->PlayerID );
			if( !pUserInfo )			return;

			g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
		}
		break;
	case MP_GTOURNAMENT_RETURNTOMAP:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUserInfo )			return;

			WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData );
			if(tomapserverport)
			{
				SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
				if( pInfo )
				{
					pUserInfo->wUserMapNum = (WORD)pmsg->dwData;
					pUserInfo->dwMapServerConnectionIndex = pInfo->dwConnectionIndex;
				}
			}

			TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );

			//--------------------------
			
		}
		break;
	case MP_GTOURNAMENT_NOTIFY_WINLOSE:
		{
			SEND_GTWINLOSE* pmsg = (SEND_GTWINLOSE*)pMsg;

			USERINFO* info = NULL;
			g_pUserTable->SetPositionUserHead();
			while( info = g_pUserTable->GetUserData() )
			{
				SEND_GTWINLOSE msg = *pmsg;
				g_Network.Send2User( info->dwConnectionIndex, (char*)&msg, dwLength );
			}
		}
		break;
	case MP_GTOURNAMENT_BATTLE_NOTIFY:
		{
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

			USERINFO* info = NULL;
			g_pUserTable->SetPositionUserHead();
			while( info = g_pUserTable->GetUserData() )
			{
				MSG_DWORD3 msg = *pmsg;
				g_Network.Send2User( info->dwConnectionIndex, (char*)&msg, dwLength );
			}
		}
		break;
	case MP_GTOURNAMENT_STARTBATTLE:
	case MP_GTOURNAMENT_ENDBATTLE:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			USERINFO * info = NULL;
			g_pUserTable->SetPositionUserHead();
			while(info = g_pUserTable->GetUserData())
			{
				MSG_DWORD2 msg = *pmsg;
				g_Network.Send2User( info->dwConnectionIndex, (char*)&msg, dwLength );
			}
		}
		break;
	case MP_GTOURNAMENT_FLAGCHANGE:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			USERINFO* info = NULL;
			g_pUserTable->SetPositionUserHead();
			while(info = g_pUserTable->GetUserData())
			{
				MSG_DWORD msg = *pmsg;
				g_Network.Send2User( info->dwConnectionIndex, (char*)&msg, dwLength );
			}
		}
		break;
	case MP_GTOURNAMENT_EVENT_START_RESULT:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwData1 );
			if( !pUserInfo )			return;

			g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
		}
		break;
	default:
		TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );
		break;
	}
}

void MP_JACKPOTUserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	
}

void MP_JACKPOTServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_JACKPOT_PRIZE_NOTIFY:
		{
			MSG_JACKPOT_PRIZE_NOTIFY* pmsg = (MSG_JACKPOT_PRIZE_NOTIFY*)pMsg;

			//SW050729	//DB Update 와의 차이를 없앤다. & 접속 유저에게 정확한 총상금을 보내기위해 세팅.
			JACKPOTMGR->SetTotalMoney(pmsg->dwRestTotalMoney);

			USERINFO* info = NULL;
			g_pUserTable->SetPositionUserHead();
			while( info = g_pUserTable->GetUserData() )
			{
				MSG_JACKPOT_PRIZE_NOTIFY msg = *pmsg;
				g_Network.Send2User( info->dwConnectionIndex, (char*)&msg, dwLength );
			}
		}
		break;
//	case MP_JACKPOT_TOTALMONEY_NOTIFY:
//		{
//			MSG_JACKPOT_TOTALMONEY_NOTIFY* pmsg = (MSG_JACKPOT_TOTALMONEY_NOTIFY*)pMsg;
//
//			//ObjectID로부터 connectionidex 얻어서 Send2User
//		}
	case MP_JACKPOT_TOTALMONEY_NOTIFY_TO_AGENT:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			JACKPOTMGR->SetTotalMoney(pmsg->dwData); //받은 AGENT에 세팅

			pmsg->Protocol = MP_JACKPOT_TOTALMONEY_NOTIFY;

			USERINFO* info = NULL;
			g_pUserTable->SetPositionUserHead();
			while( info = g_pUserTable->GetUserData() )
			{
				if( info->wUserMapNum )
				{
					MSG_DWORD msgTemp = *pmsg;
					g_Network.Send2User( info->dwConnectionIndex, (char*)&msgTemp, sizeof(msgTemp) );
				}
			}
			
		}
		break;
	default:
		TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );
		break;
	}
}

void MP_SkillServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_SKILL_START_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			
			SKILLDELAYMGR->AddSkillUse( pmsg->dwData1, pmsg->dwData2, TRUE );

		}
		break;
	default:
		TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );		
		break;
	}
}


void MP_SkillUserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_SKILL_START_SYN:
		{
			MSG_SKILL_START_SYN* pmsg = (MSG_SKILL_START_SYN*)pMsg;
			
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(!pInfo)	return;

			if( SKILLDELAYMGR->AddSkillUse( pInfo->dwCharacterID, pmsg->SkillIdx ) )
			{
				TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);
			}
			else
			{
				//NACK
				
				MSG_BYTE msg;
				msg.Category = MP_SKILL;
				msg.Protocol = MP_SKILL_START_NACK;
				msg.dwObjectID = pInfo->dwCharacterID;
				msg.bData = 0;

				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );

				ASSERT(0); //유저가 해킹을 시도했을 가능성이 있다.
			}
		}
		break;
	default:
		TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
		break;
	}
}


// guildunion
void MP_GUILD_UNIONUserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch( pTempMsg->Protocol )
	{
	case MP_GUILD_UNION_CREATE_SYN:
		{
			MSG_NAME_DWORD* pmsg = (MSG_NAME_DWORD*)pMsg;
			USERINFO* pInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pInfo )	return;

			if( (FILTERTABLE->IsInvalidCharInclude( (unsigned char*)pmsg->Name ) == TRUE ) || 
				(FILTERTABLE->IsUsableName( pmsg->Name ) == FALSE ) )
			{
				MSG_DWORD2 Msg;
				Msg.Category = MP_GUILD_UNION;
				Msg.Protocol = MP_GUILD_UNION_CREATE_NACK;
				Msg.dwData1 = eGU_Not_ValidName;
				Msg.dwData2 = 0;
				g_Network.Send2User( pInfo->dwConnectionIndex, (char*)&Msg, sizeof(Msg) );
			}
			else
				TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
		}
		break;
	default:
		TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
	}
}

void MP_GUILD_UNIONServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch( pTempMsg->Protocol )
	{
	case MP_GUILD_UNION_CREATE_NOTIFY_TO_MAP:
	case MP_GUILD_UNION_DESTROY_NOTIFY_TO_MAP:
	case MP_GUILD_UNION_INVITE_ACCEPT_NOTIFY_TO_MAP:
	case MP_GUILD_UNION_ADD_NOTIFY_TO_MAP:
	case MP_GUILD_UNION_REMOVE_NOTIFY_TO_MAP:
	case MP_GUILD_UNION_SECEDE_NOTIFY_TO_MAP:
	case MP_GUILD_UNION_MARK_REGIST_NOTIFY_TO_MAP:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	default:
		TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );
	}	
}



void MP_SIEGEWARUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_SIEGEWAR_CHEAT:
		{
			MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;

			SERVERINFO* pInfo = NULL;

			WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData2 );
			if(tomapserverport)
			{
				pInfo = g_pServerTable->FindServer( tomapserverport );
				if(pInfo)
				{
					MSG_DWORD4 msg;
					memcpy( &msg, pmsg, sizeof(MSG_DWORD4) );
					g_Network.Send2Server( pInfo->dwConnectionIndex, pMsg, dwLength );
				}
			}

			tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData3 );
			if(tomapserverport)
			{
				pInfo = g_pServerTable->FindServer( tomapserverport );
				if(pInfo)
				{
					MSG_DWORD4 msg;
					memcpy( &msg, pmsg, sizeof(MSG_DWORD4) );
					g_Network.Send2Server( pInfo->dwConnectionIndex, pMsg, dwLength );
				}
			}
		}
		break;
	case MP_SIEGEWAR_MOVEIN_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUserInfo )			return;

			g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, pMsg, dwLength );

/*			WORD tomapserverport = 0;
			tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, pmsg->dwData3 );
			if(tomapserverport)
			{
				SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
				if(pInfo)
				{
					//g_Network.Send2Server( pInfo->dwConnectionIndex, pMsg, dwLength );
					g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, pMsg, dwLength );
					return;
				}
				else
					goto MoveIn_Error;
			}
			else
				goto MoveIn_Error;

MoveIn_Error:
			MSG_DWORD msg;
			msg.Category = MP_SIEGEWAR;
			msg.Protocol = MP_SIEGEWAR_MOVEIN_NACK;
			msg.dwData = eGTError_ERROR;
			g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );*/
		}
		break;
	case MP_SIEGEWAR_BATTLEJOIN_SYN:
	case MP_SIEGEWAR_OBSERVERJOIN_SYN:
		{
			MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;
			// dwData1 - GuildIdx
			// dwData2 - ReturnMapNum
			// dwData3 - bObserver
			// dwData4 - SeigeWarMapNum

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUserInfo )			return;

			WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData4 );
			if(tomapserverport)
			{
				SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
				if(pInfo)
				{
					pUserInfo->wUserMapNum = (WORD)pmsg->dwData4;
					pUserInfo->dwMapServerConnectionIndex = pInfo->dwConnectionIndex;

					SEND_SIEGEWAR_JOININFO msg;
					msg.Category = MP_SIEGEWAR;
					msg.Protocol = pmsg->Protocol;
					msg.dwObjectID = pmsg->dwObjectID;
					msg.AgentIdx = pUserInfo->dwUniqueConnectIdx;					
					msg.UserLevel = pUserInfo->UserLevel;
					msg.GuildIdx = pmsg->dwData1;
					msg.ReturnMapNum = pmsg->dwData2;
					msg.bObserver = (BYTE)pmsg->dwData3;
					g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)&msg, sizeof(msg));
					return;
				}
			}

			MSGBASE msg;
			msg.Category = MP_SIEGEWAR;
			msg.Protocol = MP_SIEGEWAR_BATTLEJOIN_NACK;			
			g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
		}
		break;
	case MP_SIEGEWAR_LEAVE_SYN:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUserInfo )			return;
			
			MSG_DWORD3 msg;
			msg.Category = MP_SIEGEWAR;
			msg.Protocol = MP_SIEGEWAR_LEAVE_SYN;
			msg.dwObjectID = pmsg->dwObjectID;
			msg.dwData1 = pUserInfo->dwUniqueConnectIdx;
			msg.dwData2 = pUserInfo->UserLevel;
			msg.dwData3 = pUserInfo->wChannel;
			g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));
		}
		break;
	default:
		TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
		break;
	}
}


void MP_SIEGEWARServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
/*	case MP_SIEGEWAR_CHANGESTATE:
	case MP_SIEGEWAR_REGISTTIME:
	case MP_SIEGEWAR_DEFENCE_REGIST:
	case MP_SIEGEWAR_ATTACK_REGIST:
	case MP_SIEGEWAR_ACCEPTGUILD:	
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, pmsg->dwData1 );
			if(tomapserverport)
			{
				SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
				if(pInfo)
					g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)pmsg, dwLength );
			}
		}
		break;*/
	case MP_SIEGEWAR_TAXRATE:
		{
			SEND_AFFECTED_MAPLIST* pmsg = (SEND_AFFECTED_MAPLIST*)pMsg;

			//
			MSG_DWORD msg;
			SetProtocol( &msg, MP_SIEGEWAR, MP_SIEGEWAR_TAXRATE );
			msg.dwData = pmsg->Param;

			WORD tomapserverport = 0;
			SERVERINFO* pInfo = NULL;			
			for(int i=0; i<pmsg->Count; ++i)
			{
				tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, pmsg->MapList[i] );
				if( 0 == tomapserverport )			continue;

				pInfo = g_pServerTable->FindServer( tomapserverport );
				if( !pInfo )			continue;

				MSG_DWORD tmpMsg = msg;
				g_Network.Send2Server( pInfo->dwConnectionIndex, (char*)&tmpMsg, sizeof(tmpMsg) );								
			}
		}
		break;
	case MP_SIEGEWAR_RETURNTOMAP:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUserInfo )			return;

			WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData );
			if(tomapserverport)
			{
				SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
				if( pInfo )
				{
					pUserInfo->wUserMapNum = (WORD)pmsg->dwData;
					pUserInfo->dwMapServerConnectionIndex = pInfo->dwConnectionIndex;
				}
			}

			TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );

			//--------------------------
			
		}
		break;
	case MP_SIEGEWAR_FLAGCHANGE:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			USERINFO* info = NULL;
			g_pUserTable->SetPositionUserHead();
			while(info = g_pUserTable->GetUserData())
			{
				MSG_DWORD msg = *pmsg;
				g_Network.Send2User( info->dwConnectionIndex, (char*)&msg, dwLength );
			}
		}
		break;
	default:
		TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );		
		break;
	}
}

void MP_SIEGEWARPROFITUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
}

void MP_SIEGEWARPROFITServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch( pTempMsg->Protocol )
	{
	case MP_SIEGEWAR_PROFIT_CHANGE_TEXRATE_NOTIFY_TO_MAP:
	case MP_SIEGEWAR_PROFIT_CHANGE_GUILD_NOTIFY_TO_MAP:
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	default:
		TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );
	}	
}

// 2005 크리스마스 이벤트
// GM에 의한 날씨 셋팅을 맵별로 하기 위해...
void MP_WEATHERUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	USERINFO* pUserInfo = g_pUserTable->FindUser( dwConnectionIndex );
	if( !pUserInfo ) return;
	if( !( pUserInfo->UserLevel == eUSERLEVEL_GM || pUserInfo->UserLevel == eUSERLEVEL_PROGRAMMER
		|| pUserInfo->UserLevel == eUSERLEVEL_DEVELOPER ) )
		return;

	if( pUserInfo->UserLevel == eUSERLEVEL_GM )	//AI°C ³ªAß¿¡ Ao¿o¾ßAo..
	if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MASTER &&
		GMINFO->GetGMPower( dwConnectionIndex ) != eGM_POWER_EVENTER )
		return;

	WORD	wServer = 0;

	switch( pTempMsg->Protocol )
	{
		case MP_WEATHER_SET:
		{
			MSG_WEATHER* msg = (MSG_WEATHER*)pMsg;

			wServer = msg->MapNum;
		}
		break;
	case MP_WEATHER_EXE:
		{
			MSG_WORD2* msg = (MSG_WORD2*)pMsg;

			wServer = msg->wData2;
		}
		break;
	case MP_WEATHER_RETURN:
		{
			MSG_WORD* msg = (MSG_WORD*)pMsg;

			wServer = msg->wData;
		}
		break;
	}
	
	WORD wServerPort = g_pServerTable->GetServerPort( eSK_MAP, wServer );
	SERVERINFO* pInfo = g_pServerTable->FindServer( wServerPort );
	if( pInfo )
		g_Network.Send2Server( pInfo->dwConnectionIndex, pMsg, dwLength );	
}

void MP_STREETSTALLUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	USERINFO* pUserInfo = g_pUserTable->FindUser( dwConnectionIndex );
	if( !pUserInfo ) return;

	MSGBASE* pmsg = (MSGBASE*)pMsg;
	if( pUserInfo->dwCharacterID != pmsg->dwObjectID ) return;

	TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
}


void MP_EXCHANGEUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	USERINFO* pUserInfo = g_pUserTable->FindUser( dwConnectionIndex );
	if( !pUserInfo ) return;

	MSGBASE* pmsg = (MSGBASE*)pMsg;
	if( pUserInfo->dwCharacterID != pmsg->dwObjectID ) return;

	TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
}

void MP_SURVIVALUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pSenderInfo )	return;

	if( pSenderInfo->UserLevel == eUSERLEVEL_GM )
	{
		if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MASTER )
			return;
		//else
		//	TransToMapServerMsgParser(dwConnectionIndex,pMsg,dwLength);	
	}

	switch(pmsg->Protocol)
	{
	case MP_SURVIVAL_LEAVE_SYN:
		{
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUserInfo )			return;

			MSG_DWORD3 msg;
			msg.Category = MP_SURVIVAL;
			msg.Protocol = MP_SURVIVAL_LEAVE_SYN;
			msg.dwObjectID = pmsg->dwObjectID;
			msg.dwData1 = pUserInfo->dwUniqueConnectIdx;
			msg.dwData2 = pUserInfo->UserLevel;
			msg.dwData3 = pUserInfo->wChannel;
			g_Network.Send2Server( pUserInfo->dwMapServerConnectionIndex, (char*)&msg, sizeof(msg));

		}
		break;
	case MP_SURVIVAL_READY_SYN:
	case MP_SURVIVAL_STOP_SYN:
	case MP_SURVIVAL_MAPOFF_SYN:
	case MP_SURVIVAL_ITEMUSINGCOUNT_SET:
		{
			if( pSenderInfo->UserLevel > eUSERLEVEL_GM )
			{
				char buf[128] = {0,};
				sprintf(buf, "<SVVMODE>CHECK! Attempted UserId : %d IP : %d", pSenderInfo->dwUserID, pSenderInfo->UserLevel ); 
				ASSERTMSG(0,buf);
				return;
			}
			TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
		}
		break;
	default:
		TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
		break;
	}

}

void MP_SURVIVALServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_SURVIVAL_RETURNTOMAP:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
			if( !pUserInfo )			return;

			WORD tomapserverport = g_pServerTable->GetServerPort( eSK_MAP, (WORD)pmsg->dwData );
			if(tomapserverport)
			{
				SERVERINFO* pInfo = g_pServerTable->FindServer( tomapserverport );
				if( pInfo )
				{
					pUserInfo->wUserMapNum = (WORD)pmsg->dwData;
					pUserInfo->dwMapServerConnectionIndex = pInfo->dwConnectionIndex;
				}
			}

			TransToClientMsgParser( dwConnectionIndex, pMsg, dwLength );
		}
		break;
	default:
		TransToClientMsgParser(dwConnectionIndex, pMsg, dwLength);
	}
}

void MP_BOBUSANGUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	USERINFO* pSenderInfo = g_pUserTableForObjectID->FindUser( pmsg->dwObjectID );
	if( !pSenderInfo )	return;

	if( pSenderInfo->UserLevel == eUSERLEVEL_GM )
	{
		if( GMINFO->GetGMPower( dwConnectionIndex ) > eGM_POWER_MASTER )
			return;
	}

	//switch(pmsg->Protocol)
	//{
	//default:
	//	TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
	//	break;
	//}
	TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
}

void MP_BOBUSANGServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_BOBUSANG_APPEAR_MAP_TO_AGENT: 
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			BOBUSANGMGR->SetChannelState(pmsg->dwData, eBBSAS_APPEAR);
		}
		break;
	case MP_BOBUSANG_DISAPPEAR_MAP_TO_AGENT: 
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			BOBUSANGMGR->SetChannelState(pmsg->dwData, eBBSAS_DISAPPEAR);
		}
		break;
	default:
		TransToClientMsgParser(dwConnectionIndex, pMsg, dwLength);
	}
}


void MP_ITEMLIMITServerMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_ITEMLIMIT_ADDCOUNT_TO_MAP: 
		{
			g_Network.Broadcast2MapServerExceptOne(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	default:
		TransToClientMsgParser(dwConnectionIndex, pMsg, dwLength);
	}
}

/// autonote
void MP_AUTONOTEUserMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGBASE* pmsg = (MSGBASE*)pMsg;
	USERINFO* pUserInfo = g_pUserTable->FindUser( dwConnectionIndex );
	if( !pUserInfo ) return;	

	if( pUserInfo->dwCharacterID )
		pmsg->dwObjectID = pUserInfo->dwCharacterID;	//---KES 모든 메세지에 대해서 적용되어야 한다.. 차후 고치자.

	switch( pmsg->Protocol )
	{
	case MP_AUTONOTE_ASKTOAUTO_SYN:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			if( pUserInfo->UserLevel > eUSERLEVEL_GM )																// gm이상의 유저레벨은 체크안함
			if( CPunishUnit* pPunishUnit = PUNISHMGR->GetPunishUnit( pUserInfo->dwUserID, ePunish_AutoNoteUse ) )	// autonote 사용제한 체크
			{
				DWORD dwRemainTime = pPunishUnit->GetRemainTime();
				MSG_DWORD msg;
				msg.Category = MP_AUTONOTE;					//--- MP_PUNISH 로 바꾸자
				msg.Protocol = MP_AUTONOTE_PUNISH;
				msg.dwData = dwRemainTime;
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
				return;
			}

			TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
		}
		break;
	default:
		{
			TransToMapServerMsgParser( dwConnectionIndex, pMsg, dwLength );
		}
		break;
	}
}

void MP_AUTONOTEServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	switch( pTempMsg->Protocol )
	{
	case MP_AUTONOTE_ASKTOAUTO_ACK:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;	//오토상대에게 질문하는 것을 성공했다. 2분동안 재 질문이 불가능하다.

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pTempMsg->dwObjectID);
			if( !pUserInfo ) return;

			//---신고자가 2분 오토노트를 사용 못하도록
			PunishListAdd( pUserInfo->dwUserID, ePunish_AutoNoteUse, 2*60 );
			PUNISHMGR->AddPunishUnit( pUserInfo->dwUserID, ePunish_AutoNoteUse, 2*60 );

			g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
		}
		break;

	case MP_AUTONOTE_NOTAUTO:		//---오토가 답을 맞추었다고 신고자에게 보내준다.
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			USERINFO* pUserInfo = g_pUserTableForUserID->FindUser( pmsg->dwData );	//---맵서버에서 모든 에이젼트로 브로드캐스팅 된것이므로 먼저 걸른다.
			if( !pUserInfo ) return;

//			PUNISHMGR->AddPunishUnit( pUserInfo->dwUserID, ePunish_AutoNoteUse, 60*60 );	//60분	// 신고자가 오토노트를 일정시간 사용 못하도록 제재를 하자.
			PUNISHMGR->AddPunishUnit( pUserInfo->dwUserID, ePunish_AutoNoteUse, PUNISHMGR->GetAutoNoteUseTime()*60 );	//60분	// 신고자가 오토노트를 일정시간 사용 못하도록 제재를 하자.

			if( pUserInfo->dwCharacterID )
			{
				MSGBASE msg;
				msg.Category = MP_AUTONOTE;
				msg.Protocol = MP_AUTONOTE_NOTAUTO;
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
			}
		}
		break;

	case MP_AUTONOTE_ANSWER_ACK:	//---답변 맞춘 오토에게 보내는 메세지
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			//---상대가 답변을 맞추었으니, 신고자가 오토노트를 일정시간 사용 못하도록
//			PunishListAdd( pmsg->dwData, ePunish_AutoNoteUse, 60*60 );	//에이젼트에서 안하는 이유는 1시간 오토노트 사용금지 DB에 추가
			PunishListAdd( pmsg->dwData, ePunish_AutoNoteUse, PUNISHMGR->GetAutoNoteUseTime()*60 );	//에이젼트에서 안하는 이유는 1시간 오토노트 사용금지 DB에 추가

			MSGBASE msg;
			msg.Category = MP_AUTONOTE;
			msg.Protocol = MP_AUTONOTE_ANSWER_ACK;
			msg.dwObjectID = pmsg->dwObjectID;
			g_Network.Send2User( &msg, sizeof(msg) );
		}
		break;

	case MP_AUTONOTE_ANSWER_FAIL:
		{
//			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

//			PunishCountAdd( pmsg->dwData, ePunishCount_AutoUser, 1 );		//오토유저다. 오토사용회수 1회증가

//			MSGBASE msg;
//			msg.Category = MP_AUTONOTE;
//			msg.Protocol = MP_AUTONOTE_ANSWER_FAIL;
//			msg.dwObjectID = pmsg->dwObjectID;
//			g_Network.Send2User( &msg, sizeof(msg) );

			MSG_AUTOUSER_BLOCK* pmsg = (MSG_AUTOUSER_BLOCK*)pMsg;

//			PunishCountAdd( pmsg->dwData, ePunishCount_AutoUser, 1 );		//오토유저다. 오토사용회수 1회증가
            PunishCountAdd( pmsg->dwAutoUserIdx, pmsg->dwAutoCharacterIdx, pmsg->AutoCharacterName, ePunishCount_AutoUser, 1,
				pmsg->dwAskUserIdx, pmsg->dwAskCharacterIdx, pmsg->AskCharacterName );

			MSGBASE msg;
			msg.Category = MP_AUTONOTE;
			msg.Protocol = MP_AUTONOTE_ANSWER_FAIL;
			msg.dwObjectID = pmsg->dwObjectID;
			g_Network.Send2User( &msg, sizeof(msg) );
		}
		break;

	case MP_AUTONOTE_ANSWER_LOGOUT:
		{
//			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

//			PunishCountAdd( pmsg->dwData, ePunishCount_AutoUser, 1 );		//오토유저다. 오토사용회수 1회증가

			MSG_AUTOUSER_BLOCK* pmsg = (MSG_AUTOUSER_BLOCK*)pMsg;

//			PunishCountAdd( pmsg->dwData, ePunishCount_AutoUser, 1 );		//오토유저다. 오토사용회수 1회증가
			PunishCountAdd( pmsg->dwAutoUserIdx, pmsg->dwAutoCharacterIdx, pmsg->AutoCharacterName, ePunishCount_AutoUser, 1,
				pmsg->dwAskUserIdx, pmsg->dwAskCharacterIdx, pmsg->AskCharacterName );
		}
		break;

	case MP_AUTONOTE_ANSWER_TIMEOUT:
		{
/*			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			PunishCountAdd( pmsg->dwData, ePunishCount_AutoUser, 1 );		//오토유저다. 오토사용회수 1회증가

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pTempMsg->dwObjectID);
			if( pUserInfo )
				g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
*/
			MSG_AUTOUSER_BLOCK* pmsg = (MSG_AUTOUSER_BLOCK*)pMsg;

//			PunishCountAdd( pmsg->dwData, ePunishCount_AutoUser, 1 );		//오토유저다. 오토사용회수 1회증가
			PunishCountAdd( pmsg->dwAutoUserIdx, pmsg->dwAutoCharacterIdx, pmsg->AutoCharacterName, ePunishCount_AutoUser, 1,
				pmsg->dwAskUserIdx, pmsg->dwAskCharacterIdx, pmsg->AskCharacterName );

			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pTempMsg->dwObjectID);
			if( pUserInfo )
			{
				MSGBASE msg;
				msg.Category = MP_AUTONOTE;
				msg.Protocol = MP_AUTONOTE_ANSWER_TIMEOUT;
				msg.dwObjectID = pmsg->dwObjectID;
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) ); 
			}
		}
		break;

	case MP_AUTONOTE_KILLAUTO:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			USERINFO* pUserInfo = g_pUserTableForUserID->FindUser( pmsg->dwData );
			if( !pUserInfo ) return;

			MSGBASE msg;
			msg.Category = MP_AUTONOTE;
			msg.Protocol = MP_AUTONOTE_KILLAUTO;

			if( pUserInfo->dwCharacterID )
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) ); 
		}
		break;

	case MP_AUTONOTE_DISCONNECT:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			USERINFO* pUserInfo = g_pUserTableForUserID->FindUser( pmsg->dwData );
			if( !pUserInfo ) return;

			DWORD dwConnectionIdx = pUserInfo->dwConnectionIndex;
			OnDisconnectUser( dwConnectionIdx );
			DisconnectUser( dwConnectionIdx );
		}
		break;

	default:
		{
			USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pTempMsg->dwObjectID);
			if( pUserInfo )
				g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
		}
		break;
	}
}

// 요새전
void MP_FORTWARServerMsgParser( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;

	switch( pTempMsg->Protocol )
	{
	case MP_FORTWAR_START_BEFORE10MIN:
	case MP_FORTWAR_START:
		{
			MSG_FORTWAR_START* pmsg = (MSG_FORTWAR_START*)pMsg;

			// 유저한테 보낸다.
			USERINFO* pUserInfo = NULL;
			g_pUserTable->SetPositionUserHead();
			while( pUserInfo = g_pUserTable->GetUserData() )
			{	
				MSG_FORTWAR_START msg = *pmsg;
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
			}			
		}
		break;
	case MP_FORTWAR_END:
		{
			MSG_FORTWAR_END* pmsg = (MSG_FORTWAR_END*)pMsg;
			// 유저한테 보낸다.
			USERINFO* pUserInfo = NULL;
			g_pUserTable->SetPositionUserHead();
			while( pUserInfo = g_pUserTable->GetUserData() )
			{	
				MSG_FORTWAR_END msg = *pmsg;
				g_Network.Send2User( pUserInfo->dwConnectionIndex, (char*)&msg, sizeof(msg) );
			}	
		}
		break;
	case MP_FORTWAR_START_BEFORE10MIN_TO_MAP:
	case MP_FORTWAR_START_TO_MAP:
	case MP_FORTWAR_ING_TO_MAP:
	case MP_FORTWAR_END_TO_MAP:
	case MP_FORTWAR_WAREHOUSE_ITEM_INSERT_TO_MAP:
	case MP_FORTWAR_WAREHOUSE_ITEM_MOVE_TO_INVEN_TO_MAP:
	case MP_FORTWAR_WAREHOUSE_ITEM_MOVE_TO_SIEGEHOUSE_TO_MAP:
	case MP_FORTWAR_SIEGEWAREHOUSE_ITEM_MOVE_TO_INVEN_TO_MAP:
	case MP_FORTWAR_WAREHOUSE_MONEY_TO_MAP:
		{
			// 다른 맵서버에 보낸다.
			g_Network.Broadcast2MapServerExceptOne( dwConnectionIndex, pMsg, dwLength );
		}
		break;
	default:
		{
            USERINFO* pUserInfo = g_pUserTableForObjectID->FindUser(pTempMsg->dwObjectID);
			if( pUserInfo )
				g_Network.Send2User( pUserInfo->dwConnectionIndex, pMsg, dwLength );
		}
		break;
	}
}


#ifdef _HACK_SHIELD_
void MP_HACKSHIELDUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	HACKSHIELDMGR->NetworkMsgParse(dwConnectionIndex, pMsg, dwLength);
}
#endif

#ifdef _NPROTECT_
void MP_NPROTECTUserMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	NPROTECTMGR->NetworkMsgParse(dwConnectionIndex, pMsg, dwLength);
}
#endif
