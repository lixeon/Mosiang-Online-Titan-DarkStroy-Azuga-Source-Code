
#include "stdafx.h"
#include "DistributeNetworkMsgParser.h"
#include "ServerTable.h"
#include "Network.h"
#include "DistributeDBMsgParser.h"
#include "UserManager.h"
#include "UserTable.h"
#include "BootManager.h"
#include "BuddyAuth.h"

#ifdef _KOR_LOCAL_
#include "MD5Checksum.h"
#include "ServerListManager.h"
#endif


extern int g_nServerSetNum;
extern HWND g_hWnd;
//extern CServerTable* g_pServerTable;

extern BYTE g_Encrypt0;
extern BYTE g_Encrypt1;



DWORD g_dwMaxUser = 10000;

void MP_MonitorMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_MORNITORMAPSERVER_NOTICESEND_SYN:
		{
			// °øÁö»çÇ× 
			ASSERT(0);
		}
		break;
	case MP_MORNITORMAPSERVER_PING_SYN:
		{
			pTempMsg->Protocol = MP_MORNITORMAPSERVER_PING_ACK;
			g_Network.Send2Server(dwConnectionIndex, pMsg, dwLength);
		}
		break;
	case MP_MORNITORMAPSERVER_CHANGE_USERLEVEL_SYN:
		{
			pTempMsg->Protocol = MP_MORNITORMAPSERVER_CHANGE_USERLEVEL_ACK;
			
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if( pmsg->dwData == 0 )
			{
				pmsg->dwData = gUserMGR.GetUserLevel();
			}
			else
			{
				gUserMGR.SetUserLevel( (BYTE)pmsg->dwData );
				g_Console.LOG(4, "Change User Level : %d", pmsg->dwData );
			}
			g_Network.Send2Server( dwConnectionIndex, pMsg, dwLength );
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
	case MP_MORNITORMAPSERVER_SERVEROFF_SYN:
		{
			SendMessage(g_hWnd, WM_CLOSE, 0, 0);
		}
		break;
	case MP_MORNITORMAPSERVER_QUERY_VERSION_SYN:
		{			
			MSGNOTICE msg;
			msg.Category = MP_MORNITORMAPSERVER;
			msg.Protocol = MP_MORNITORMAPSERVER_QUERY_VERSION_ACK;
			strcpy( msg.Msg, gUserMGR.GetVersion() );
			g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(MSGNOTICE) );
		}
		break;
	case MP_MORNITORMAPSERVER_CHANGE_VERSION_SYN:
		{
			MSGNOTICE* pmsg = (MSGNOTICE*)pMsg;
			gUserMGR.SetVersion( pmsg->Msg );
			gUserMGR.SaveVersion();
			g_Console.LOG( 4, "Change Version : %s", pmsg->Msg );

			pmsg->Protocol = MP_MORNITORMAPSERVER_CHANGE_VERSION_ACK;
			g_Network.Send2Server( dwConnectionIndex, pMsg, sizeof(MSGNOTICE) );
		}
		break;

	case MP_MORNITORMAPSERVER_QUERY_MAXUSER_SYN:
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
			g_Console.LOG( 4, "Max User : %d", g_dwMaxUser );

			FILE* fp = fopen( "MaxUserCount.txt", "w" );
			if(fp)
			{
				fprintf( fp, "%d", g_dwMaxUser );
				fclose( fp );
			}

			MSG_DWORD msg;
			msg.Category = MP_MORNITORMAPSERVER;
			msg.Protocol = MP_MORNITORMAPSERVER_CHANGE_MAXUSER_ACK;
			msg.dwData = g_dwMaxUser;

			g_Network.Send2Server( dwConnectionIndex, (char*)&msg, sizeof(msg) );
		}
		break;
	}
}

void MP_POWERUPMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	BOOTMNGR->NetworkMsgParse(dwConnectionIndex, pMsg, dwLength);
}

void MP_DISTRIBUTESERVERMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
		switch(pTempMsg->Protocol)
		{
/*
		case MP_SERVER_PWRUP:
			{
				MSG_WORD * msg = (MSG_WORD *)pMsg;
				SERVERINFO * info = g_pServerTable->FindServer(msg->wData);//port
				info->dwConnectionIndex = dwConnectionIndex;
				// situation : µð½ºÆ®¸®ºäÆ®°¡ ÄÑÁ® ÀÖ°í ¿¡ÀÌÁ¯Æ®°¡ Á¢¼ÓÇßÀ» ¶§ usercount = 0;
				if(info->wServerKind == AGENT_SERVER)
					info->wAgentUserCnt = 0;
				if(info->wServerKind == MONITOR_SERVER || info->wServerKind == MONITOR_AGENT_SERVER)
					{
						REGIST_MAP msg;
						msg.Category = MP_SERVER;
						msg.Protocol = MP_SERVER_REGISTMAP_ACK;
						msg.mapServerPort = g_pServerTable->GetSelfServer()->wPortForServer;
						msg.mapnum = g_pListCollect->GetLoadDistributeNum();
						g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));
					}
			}
			break;
		case MP_SERVER_REGISTMAP_SYN:
			{
				REGIST_MAP msg;
				msg.Category = MP_SERVER;
				msg.Protocol = MP_SERVER_REGISTMAP_ACK;
				msg.mapServerPort = g_pServerTable->GetSelfServer()->wPortForServer;
				msg.mapnum = g_pListCollect->GetLoadDistributeNum();
				g_Network.Send2Server(dwConnectionIndex, (char*)&msg, sizeof(msg));
			}
			break;*/

		case MP_SERVER_USERCNT:
			{
				MSG_WORD2* recv = (MSG_WORD2*)pMsg;
				WORD port = recv->wData1;		// Port
				SERVERINFO * info = g_pServerTable->FindServer(port);
				if(info)
					info->wAgentUserCnt = recv->wData2;
			}
			break;

		case MP_SERVER_AGENTDISABLE:	//AAA
			{
				SERVERINFO* pServerInfo = g_pServerTable->FindServerForConnectionIndex(dwConnectionIndex);
				if( pServerInfo == NULL ) return;

				g_pServerTable->SetDisableAgent( pServerInfo->wServerNum );
			}
			break;
		}
	
}
void MP_DISTRIBUTEAUTOPATCHMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_AUTOPATCH_TRAFFIC_SYN:
		{
			ServerTraffic * msg = (ServerTraffic *)pMsg;
			ServerTraffic sendMsg;
			sendMsg.Category = MP_AUTOPATCH;
			sendMsg.Protocol = MP_AUTOPATCH_TRAFFIC_ACK;
			sendMsg.UserNum = 0;
			sendMsg.Returnkey = msg->Returnkey;

			g_pServerTable->SetPositionHead();
			SERVERINFO * info = NULL;
			while(info = (SERVERINFO *)g_pServerTable->GetData())
			{
				if(info->wServerKind == AGENT_SERVER)
					sendMsg.UserNum += info->wAgentUserCnt;
			}
			sendMsg.UserNum = 123;
			g_Network.Send2User(dwConnectionIndex, (char *)&sendMsg, sizeof(sendMsg));
		}
		break;
	}
}


#ifdef _TW_LOCAL_																																								
		char strWrap[100][20] =																																						
{ {	65 	,	32 	,	118 	,	112 	,	123 	,	84 	,	3 	,	55 	,	58 	,	80 	,	102 	,	118 	,	72 	,	97 	,	98 	,	93 	,	35 	,	28 	,	83 	,	58 	},
{	88 	,	5 	,	116 	,	80 	,	76 	,	102 	,	95 	,	91 	,	97 	,	47 	,	76 	,	15 	,	116 	,	112 	,	116 	,	37 	,	20 	,	107 	,	125 	,	37 	},
{	18 	,	125 	,	36 	,	75 	,	4 	,	50 	,	59 	,	88 	,	60 	,	105 	,	105 	,	59 	,	114 	,	18 	,	122 	,	115 	,	80 	,	5 	,	98 	,	3 	},
{	3 	,	54 	,	91 	,	89 	,	126 	,	17 	,	33 	,	28 	,	60 	,	27 	,	74 	,	21 	,	46 	,	54 	,	23 	,	113 	,	114 	,	106 	,	70 	,	22 	},
{	74 	,	33 	,	64 	,	62 	,	14 	,	63 	,	4 	,	121 	,	49 	,	81 	,	61 	,	28 	,	63 	,	95 	,	98 	,	119 	,	8 	,	107 	,	76 	,	79 	},
{	119 	,	110 	,	6 	,	122 	,	96 	,	11 	,	125 	,	54 	,	4 	,	26 	,	83 	,	84 	,	72 	,	60 	,	4 	,	43 	,	67 	,	15 	,	125 	,	70 	},
{	42 	,	121 	,	105 	,	59 	,	4 	,	101 	,	117 	,	71 	,	48 	,	3 	,	81 	,	27 	,	1 	,	81 	,	20 	,	101 	,	47 	,	2 	,	109 	,	45 	},
{	23 	,	21 	,	45 	,	94 	,	80 	,	89 	,	57 	,	63 	,	93 	,	75 	,	98 	,	86 	,	66 	,	93 	,	20 	,	67 	,	33 	,	125 	,	41 	,	37 	},
{	101 	,	7 	,	71 	,	30 	,	64 	,	49 	,	121 	,	30 	,	87 	,	32 	,	50 	,	8 	,	85 	,	111 	,	80 	,	121 	,	39 	,	72 	,	16 	,	20 	},
{	23 	,	111 	,	25 	,	65 	,	115 	,	72 	,	110 	,	56 	,	33 	,	39 	,	98 	,	45 	,	62 	,	57 	,	16 	,	77 	,	118 	,	61 	,	54 	,	46 	},
{	108 	,	58 	,	113 	,	115 	,	88 	,	124 	,	63 	,	45 	,	111 	,	10 	,	6 	,	43 	,	47 	,	99 	,	94 	,	7 	,	21 	,	44 	,	10 	,	48 	},
{	34 	,	30 	,	4 	,	111 	,	91 	,	99 	,	88 	,	52 	,	96 	,	125 	,	111 	,	31 	,	110 	,	21 	,	116 	,	33 	,	2 	,	86 	,	106 	,	96 	},
{	28 	,	54 	,	50 	,	46 	,	104 	,	110 	,	85 	,	25 	,	62 	,	45 	,	78 	,	30 	,	24 	,	81 	,	115 	,	16 	,	73 	,	112 	,	90 	,	111 	},
{	65 	,	37 	,	119 	,	92 	,	105 	,	93 	,	36 	,	21 	,	22 	,	90 	,	121 	,	44 	,	123 	,	8 	,	79 	,	95 	,	79 	,	73 	,	41 	,	103 	},
{	68 	,	102 	,	44 	,	48 	,	22 	,	33 	,	48 	,	122 	,	120 	,	86 	,	72 	,	51 	,	71 	,	27 	,	42 	,	39 	,	115 	,	52 	,	80 	,	19 	},
{	16 	,	47 	,	103 	,	53 	,	73 	,	83 	,	45 	,	109 	,	72 	,	5 	,	59 	,	68 	,	81 	,	27 	,	8 	,	106 	,	39 	,	8 	,	86 	,	2 	},
{	46 	,	92 	,	109 	,	1 	,	111 	,	109 	,	125 	,	73 	,	61 	,	100 	,	45 	,	101 	,	109 	,	113 	,	81 	,	41 	,	101 	,	19 	,	114 	,	48 	},
{	57 	,	57 	,	31 	,	28 	,	114 	,	7 	,	103 	,	76 	,	5 	,	99 	,	18 	,	20 	,	89 	,	48 	,	14 	,	46 	,	68 	,	105 	,	64 	,	82 	},
{	82 	,	104 	,	73 	,	39 	,	18 	,	77 	,	63 	,	49 	,	74 	,	103 	,	14 	,	100 	,	10 	,	69 	,	49 	,	71 	,	117 	,	52 	,	65 	,	49 	},
{	39 	,	75 	,	17 	,	123 	,	46 	,	93 	,	35 	,	29 	,	81 	,	92 	,	85 	,	56 	,	75 	,	95 	,	81 	,	103 	,	28 	,	44 	,	5 	,	43 	},
{	74 	,	28 	,	95 	,	98 	,	67 	,	68 	,	108 	,	64 	,	118 	,	110 	,	51 	,	7 	,	90 	,	27 	,	5 	,	89 	,	14 	,	46 	,	63 	,	6 	},
{	69 	,	30 	,	105 	,	24 	,	64 	,	78 	,	59 	,	26 	,	1 	,	9 	,	115 	,	49 	,	76 	,	59 	,	106 	,	78 	,	15 	,	31 	,	100 	,	117 	},
{	11 	,	38 	,	91 	,	41 	,	88 	,	15 	,	1 	,	116 	,	67 	,	28 	,	126 	,	119 	,	82 	,	97 	,	25 	,	57 	,	86 	,	17 	,	110 	,	91 	},
{	18 	,	31 	,	112 	,	26 	,	64 	,	13 	,	21 	,	72 	,	121 	,	60 	,	80 	,	82 	,	76 	,	97 	,	31 	,	96 	,	117 	,	5 	,	51 	,	47 	},
{	46 	,	15 	,	59 	,	91 	,	3 	,	116 	,	72 	,	82 	,	64 	,	26 	,	17 	,	78 	,	20 	,	47 	,	96 	,	104 	,	8 	,	38 	,	109 	,	116 	},
{	4 	,	74 	,	94 	,	77 	,	3 	,	62 	,	111 	,	101 	,	28 	,	73 	,	113 	,	63 	,	93 	,	37 	,	49 	,	39 	,	125 	,	112 	,	40 	,	80 	},
{	69 	,	67 	,	46 	,	105 	,	77 	,	10 	,	125 	,	15 	,	2 	,	63 	,	39 	,	67 	,	5 	,	50 	,	94 	,	119 	,	68 	,	3 	,	79 	,	111 	},
{	19 	,	27 	,	67 	,	68 	,	89 	,	39 	,	32 	,	59 	,	31 	,	88 	,	115 	,	40 	,	79 	,	127 	,	9 	,	95 	,	6 	,	119 	,	103 	,	15 	},
{	124 	,	77 	,	10 	,	38 	,	64 	,	40 	,	90 	,	3 	,	53 	,	30 	,	84 	,	10 	,	122 	,	46 	,	72 	,	118 	,	108 	,	24 	,	79 	,	112 	},
{	27 	,	67 	,	95 	,	9 	,	58 	,	32 	,	120 	,	89 	,	114 	,	31 	,	121 	,	95 	,	48 	,	56 	,	21 	,	96 	,	25 	,	77 	,	19 	,	92 	},
{	56 	,	125 	,	38 	,	110 	,	18 	,	66 	,	79 	,	1 	,	25 	,	19 	,	37 	,	42 	,	15 	,	80 	,	49 	,	21 	,	61 	,	99 	,	126 	,	91 	},
{	80 	,	98 	,	116 	,	88 	,	83 	,	116 	,	118 	,	80 	,	28 	,	82 	,	91 	,	27 	,	107 	,	116 	,	43 	,	105 	,	40 	,	66 	,	102 	,	21 	},
{	14 	,	121 	,	2 	,	3 	,	104 	,	16 	,	27 	,	39 	,	107 	,	57 	,	61 	,	58 	,	50 	,	109 	,	96 	,	99 	,	114 	,	52 	,	10 	,	45 	},
{	126 	,	101 	,	79 	,	95 	,	17 	,	17 	,	82 	,	83 	,	40 	,	48 	,	25 	,	80 	,	108 	,	108 	,	60 	,	47 	,	71 	,	11 	,	110 	,	40 	},
{	123 	,	47 	,	105 	,	90 	,	123 	,	56 	,	71 	,	64 	,	67 	,	64 	,	11 	,	26 	,	50 	,	18 	,	51 	,	43 	,	91 	,	36 	,	56 	,	66 	},
{	114 	,	85 	,	42 	,	12 	,	15 	,	8 	,	35 	,	88 	,	14 	,	25 	,	23 	,	104 	,	122 	,	64 	,	111 	,	68 	,	1 	,	18 	,	71 	,	43 	},
{	104 	,	93 	,	40 	,	37 	,	97 	,	36 	,	91 	,	0 	,	66 	,	60 	,	57 	,	34 	,	124 	,	68 	,	48 	,	84 	,	43 	,	47 	,	68 	,	76 	},
{	115 	,	34 	,	118 	,	87 	,	7 	,	19 	,	83 	,	116 	,	24 	,	103 	,	77 	,	27 	,	3 	,	5 	,	67 	,	103 	,	58 	,	100 	,	47 	,	34 	},
{	124 	,	69 	,	54 	,	61 	,	125 	,	112 	,	97 	,	121 	,	52 	,	48 	,	14 	,	126 	,	93 	,	26 	,	12 	,	46 	,	20 	,	7 	,	115 	,	20 	},
{	1 	,	125 	,	98 	,	10 	,	73 	,	17 	,	37 	,	82 	,	2 	,	8 	,	30 	,	97 	,	29 	,	43 	,	116 	,	8 	,	38 	,	63 	,	106 	,	34 	},
{	17 	,	52 	,	57 	,	34 	,	75 	,	69 	,	66 	,	33 	,	110 	,	25 	,	11 	,	8 	,	62 	,	45 	,	55 	,	54 	,	1 	,	32 	,	77 	,	88 	},
{	32 	,	36 	,	24 	,	83 	,	33 	,	96 	,	33 	,	89 	,	24 	,	50 	,	19 	,	77 	,	88 	,	96 	,	124 	,	102 	,	109 	,	102 	,	89 	,	48 	},
{	29 	,	28 	,	78 	,	123 	,	58 	,	110 	,	50 	,	94 	,	109 	,	28 	,	39 	,	59 	,	23 	,	123 	,	98 	,	106 	,	87 	,	9 	,	9 	,	104 	},
{	118 	,	120 	,	36 	,	82 	,	59 	,	83 	,	81 	,	104 	,	95 	,	66 	,	66 	,	91 	,	86 	,	52 	,	34 	,	53 	,	68 	,	10 	,	6 	,	124 	},
{	89 	,	111 	,	102 	,	79 	,	32 	,	38 	,	100 	,	46 	,	108 	,	23 	,	126 	,	13 	,	78 	,	122 	,	49 	,	112 	,	83 	,	39 	,	107 	,	34 	},
{	125 	,	21 	,	32 	,	62 	,	55 	,	69 	,	24 	,	75 	,	40 	,	82 	,	4 	,	102 	,	106 	,	66 	,	30 	,	30 	,	117 	,	99 	,	70 	,	117 	},
{	121 	,	56 	,	73 	,	83 	,	120 	,	99 	,	40 	,	84 	,	48 	,	97 	,	37 	,	108 	,	93 	,	21 	,	105 	,	14 	,	29 	,	89 	,	28 	,	105 	},
{	118 	,	19 	,	82 	,	85 	,	2 	,	0 	,	90 	,	122 	,	99 	,	81 	,	81 	,	58 	,	39 	,	32 	,	78 	,	119 	,	74 	,	16 	,	44 	,	43 	},
{	122 	,	68 	,	50 	,	56 	,	22 	,	107 	,	125 	,	46 	,	71 	,	44 	,	36 	,	42 	,	92 	,	99 	,	120 	,	5 	,	28 	,	5 	,	47 	,	116 	},
{	42 	,	44 	,	83 	,	47 	,	115 	,	64 	,	27 	,	31 	,	69 	,	104 	,	82 	,	61 	,	38 	,	121 	,	56 	,	37 	,	98 	,	110 	,	24 	,	68 	},
{	11 	,	59 	,	21 	,	62 	,	108 	,	91 	,	89 	,	31 	,	112 	,	43 	,	48 	,	72 	,	111 	,	55 	,	62 	,	64 	,	77 	,	119 	,	86 	,	33 	},
{	112 	,	88 	,	47 	,	23 	,	106 	,	53 	,	125 	,	98 	,	82 	,	114 	,	46 	,	68 	,	26 	,	121 	,	56 	,	117 	,	52 	,	21 	,	109 	,	93 	},
{	85 	,	33 	,	85 	,	69 	,	52 	,	120 	,	32 	,	30 	,	126 	,	111 	,	55 	,	12 	,	117 	,	107 	,	1 	,	108 	,	91 	,	18 	,	40 	,	22 	},
{	72 	,	18 	,	124 	,	56 	,	15 	,	40 	,	98 	,	0 	,	11 	,	37 	,	28 	,	56 	,	73 	,	127 	,	42 	,	77 	,	20 	,	73 	,	70 	,	70 	},
{	16 	,	2 	,	83 	,	77 	,	42 	,	13 	,	109 	,	87 	,	104 	,	75 	,	45 	,	23 	,	101 	,	126 	,	9 	,	25 	,	48 	,	42 	,	126 	,	15 	},
{	123 	,	108 	,	112 	,	90 	,	43 	,	76 	,	96 	,	7 	,	30 	,	92 	,	122 	,	88 	,	84 	,	127 	,	118 	,	42 	,	3 	,	93 	,	87 	,	46 	},
{	73 	,	1 	,	81 	,	68 	,	65 	,	78 	,	48 	,	27 	,	24 	,	122 	,	36 	,	32 	,	13 	,	33 	,	101 	,	16 	,	4 	,	69 	,	111 	,	41 	},
{	94 	,	123 	,	74 	,	1 	,	74 	,	110 	,	71 	,	62 	,	50 	,	98 	,	116 	,	61 	,	3 	,	56 	,	127 	,	39 	,	67 	,	88 	,	13 	,	4 	},
{	54 	,	23 	,	93 	,	48 	,	48 	,	65 	,	93 	,	65 	,	9 	,	98 	,	56 	,	117 	,	61 	,	17 	,	54 	,	101 	,	22 	,	91 	,	15 	,	94 	},
{	15 	,	13 	,	37 	,	103 	,	34 	,	22 	,	65 	,	78 	,	6 	,	5 	,	76 	,	53 	,	66 	,	33 	,	14 	,	26 	,	0 	,	0 	,	93 	,	97 	},
{	56 	,	92 	,	62 	,	61 	,	80 	,	31 	,	9 	,	25 	,	97 	,	115 	,	66 	,	68 	,	54 	,	85 	,	83 	,	116 	,	120 	,	123 	,	83 	,	28 	},
{	52 	,	124 	,	72 	,	64 	,	95 	,	24 	,	71 	,	121 	,	79 	,	57 	,	16 	,	117 	,	17 	,	95 	,	0 	,	10 	,	49 	,	16 	,	64 	,	118 	},
{	3 	,	57 	,	76 	,	62 	,	65 	,	48 	,	9 	,	122 	,	71 	,	6 	,	32 	,	83 	,	107 	,	88 	,	82 	,	0 	,	118 	,	117 	,	49 	,	20 	},
{	113 	,	79 	,	28 	,	52 	,	80 	,	74 	,	19 	,	76 	,	125 	,	114 	,	39 	,	63 	,	106 	,	40 	,	101 	,	25 	,	104 	,	83 	,	101 	,	10 	},
{	11 	,	89 	,	15 	,	89 	,	68 	,	37 	,	64 	,	105 	,	17 	,	89 	,	15 	,	98 	,	12 	,	109 	,	9 	,	21 	,	124 	,	34 	,	90 	,	111 	},
{	83 	,	116 	,	67 	,	67 	,	56 	,	124 	,	76 	,	120 	,	78 	,	84 	,	36 	,	18 	,	96 	,	124 	,	66 	,	59 	,	6 	,	121 	,	58 	,	37 	},
{	105 	,	86 	,	14 	,	56 	,	50 	,	43 	,	1 	,	41 	,	117 	,	87 	,	3 	,	1 	,	98 	,	55 	,	116 	,	123 	,	62 	,	87 	,	2 	,	18 	},
{	52 	,	67 	,	52 	,	95 	,	37 	,	122 	,	106 	,	21 	,	113 	,	94 	,	124 	,	102 	,	90 	,	26 	,	54 	,	13 	,	115 	,	123 	,	30 	,	22 	},
{	88 	,	61 	,	69 	,	31 	,	99 	,	40 	,	33 	,	74 	,	48 	,	50 	,	35 	,	109 	,	111 	,	1 	,	114 	,	41 	,	126 	,	43 	,	109 	,	115 	},
{	39 	,	43 	,	97 	,	99 	,	48 	,	44 	,	23 	,	21 	,	117 	,	45 	,	77 	,	122 	,	82 	,	71 	,	54 	,	126 	,	24 	,	87 	,	81 	,	86 	},
{	78 	,	46 	,	100 	,	93 	,	17 	,	47 	,	28 	,	118 	,	11 	,	89 	,	60 	,	20 	,	39 	,	14 	,	108 	,	79 	,	106 	,	81 	,	117 	,	109 	},
{	47 	,	66 	,	45 	,	37 	,	29 	,	37 	,	65 	,	102 	,	127 	,	47 	,	33 	,	50 	,	5 	,	105 	,	92 	,	100 	,	50 	,	16 	,	53 	,	102 	},
{	90 	,	67 	,	65 	,	73 	,	72 	,	115 	,	79 	,	48 	,	0 	,	68 	,	56 	,	86 	,	126 	,	41 	,	98 	,	61 	,	52 	,	42 	,	89 	,	15 	},
{	110 	,	69 	,	15 	,	42 	,	8 	,	60 	,	75 	,	109 	,	81 	,	117 	,	2 	,	7 	,	50 	,	2 	,	59 	,	39 	,	28 	,	65 	,	36 	,	91 	},
{	61 	,	81 	,	99 	,	76 	,	119 	,	60 	,	41 	,	22 	,	85 	,	116 	,	18 	,	38 	,	97 	,	51 	,	96 	,	50 	,	100 	,	11 	,	109 	,	38 	},
{	80 	,	102 	,	35 	,	111 	,	46 	,	25 	,	25 	,	18 	,	57 	,	3 	,	3 	,	114 	,	48 	,	71 	,	109 	,	18 	,	23 	,	13 	,	112 	,	120 	},
{	89 	,	23 	,	110 	,	75 	,	30 	,	30 	,	54 	,	9 	,	27 	,	19 	,	93 	,	61 	,	76 	,	24 	,	34 	,	86 	,	30 	,	100 	,	16 	,	41 	},
{	21 	,	44 	,	33 	,	55 	,	116 	,	36 	,	7 	,	52 	,	65 	,	80 	,	19 	,	28 	,	74 	,	90 	,	57 	,	62 	,	38 	,	114 	,	37 	,	66 	},
{	124 	,	83 	,	101 	,	15 	,	98 	,	81 	,	14 	,	45 	,	3 	,	78 	,	103 	,	66 	,	84 	,	27 	,	114 	,	81 	,	53 	,	77 	,	87 	,	31 	},
{	38 	,	58 	,	19 	,	90 	,	83 	,	11 	,	26 	,	31 	,	87 	,	107 	,	12 	,	123 	,	31 	,	108 	,	12 	,	40 	,	24 	,	37 	,	58 	,	62 	},
{	104 	,	52 	,	109 	,	38 	,	41 	,	18 	,	66 	,	70 	,	42 	,	95 	,	45 	,	71 	,	38 	,	75 	,	29 	,	113 	,	105 	,	121 	,	112 	,	8 	},
{	107 	,	47 	,	36 	,	89 	,	123 	,	40 	,	32 	,	83 	,	113 	,	100 	,	92 	,	100 	,	60 	,	93 	,	25 	,	45 	,	106 	,	94 	,	62 	,	123 	},
{	57 	,	72 	,	115 	,	87 	,	24 	,	12 	,	81 	,	57 	,	70 	,	81 	,	29 	,	83 	,	21 	,	91 	,	103 	,	4 	,	65 	,	1 	,	7 	,	120 	},
{	60 	,	35 	,	37 	,	37 	,	108 	,	59 	,	39 	,	87 	,	87 	,	31 	,	70 	,	50 	,	13 	,	100 	,	39 	,	23 	,	107 	,	19 	,	125 	,	20 	},
{	24 	,	46 	,	68 	,	80 	,	60 	,	63 	,	75 	,	72 	,	111 	,	94 	,	5 	,	46 	,	76 	,	113 	,	70 	,	19 	,	118 	,	71 	,	100 	,	104 	},
{	107 	,	6 	,	44 	,	18 	,	27 	,	115 	,	112 	,	79 	,	93 	,	67 	,	49 	,	112 	,	29 	,	109 	,	32 	,	95 	,	34 	,	98 	,	92 	,	123 	},
{	21 	,	98 	,	14 	,	99 	,	63 	,	50 	,	113 	,	37 	,	43 	,	22 	,	89 	,	42 	,	32 	,	58 	,	91 	,	108 	,	63 	,	69 	,	61 	,	16 	},
{	88 	,	14 	,	49 	,	126 	,	121 	,	57 	,	50 	,	118 	,	40 	,	36 	,	122 	,	5 	,	62 	,	40 	,	63 	,	77 	,	62 	,	37 	,	47 	,	5 	},
{	70 	,	108 	,	109 	,	96 	,	117 	,	89 	,	8 	,	27 	,	121 	,	108 	,	13 	,	45 	,	123 	,	91 	,	23 	,	93 	,	113 	,	66 	,	85 	,	112 	},
{	68 	,	63 	,	82 	,	61 	,	121 	,	30 	,	64 	,	46 	,	49 	,	109 	,	90 	,	124 	,	81 	,	116 	,	21 	,	40 	,	53 	,	75 	,	97 	,	106 	},
{	115 	,	58 	,	98 	,	0 	,	111 	,	39 	,	29 	,	81 	,	28 	,	61 	,	104 	,	90 	,	73 	,	66 	,	8 	,	69 	,	103 	,	29 	,	126 	,	109 	},
{	32 	,	45 	,	117 	,	64 	,	86 	,	24 	,	21 	,	22 	,	0 	,	64 	,	31 	,	100 	,	109 	,	116 	,	95 	,	124 	,	54 	,	112 	,	26 	,	42 	},
{	17 	,	95 	,	89 	,	49 	,	20 	,	94 	,	34 	,	84 	,	90 	,	95 	,	29 	,	89 	,	111 	,	35 	,	112 	,	106 	,	15 	,	31 	,	69 	,	3 	},
{	83 	,	17 	,	124 	,	61 	,	0 	,	55 	,	13 	,	4 	,	102 	,	5 	,	59 	,	124 	,	22 	,	79 	,	27 	,	11 	,	47 	,	42 	,	7 	,	1 	},
{	88 	,	39 	,	33 	,	86 	,	73 	,	10 	,	117 	,	24 	,	61 	,	102 	,	112 	,	72 	,	13 	,	27 	,	25 	,	26 	,	56 	,	120 	,	55 	,	98 	},
{	98 	,	64 	,	9 	,	43 	,	24 	,	35 	,	22 	,	38 	,	82 	,	15 	,	44 	,	117 	,	38 	,	23 	,	114 	,	97 	,	105 	,	11 	,	8 	,	70 	},
{	17 	,	65 	,	86 	,	73 	,	115 	,	16 	,	32 	,	39 	,	120 	,	34 	,	12 	,	17 	,	17 	,	10 	,	10 	,	51 	,	44 	,	111 	,	11 	,	127 	},
{	3 	,	7 	,	2 	,	91 	,	47 	,	118 	,	13 	,	93 	,	78 	,	124 	,	8 	,	98 	,	86 	,	115 	,	98 	,	102 	,	32 	,	3 	,	56 	,	6 	},
{	65 	,	107 	,	73 	,	88 	,	1 	,	29 	,	39 	,	80 	,	88 	,	66 	,	117 	,	98 	,	81 	,	78 	,	56 	,	51 	,	26 	,	89 	,	83 	,	20 	},
{	40 	,	71 	,	17 	,	10 	,	10 	,	18 	,	27 	,	64 	,	71 	,	69 	,	102 	,	101 	,	68 	,	70 	,	33 	,	110 	,	65 	,	40 	,	117 	,	7 	} };




#elif defined _HK_LOCAL_																																								
		char strWrap[100][20] =																																						
{ {	39 	,	121 	,	85 	,	91 	,	67 	,	85 	,	23 	,	49 	,	52 	,	69 	,	11 	,	65 	,	47 	,	95 	,	52 	,	38 	,	40 	,	62 	,	9 	,	108 	},
{	113 	,	43 	,	61 	,	101 	,	58 	,	10 	,	125 	,	74 	,	37 	,	45 	,	68 	,	5 	,	71 	,	41 	,	24 	,	47 	,	73 	,	41 	,	107 	,	14 	},
{	72 	,	94 	,	103 	,	98 	,	49 	,	95 	,	60 	,	30 	,	47 	,	55 	,	15 	,	108 	,	67 	,	82 	,	107 	,	93 	,	25 	,	17 	,	68 	,	34 	},
{	80 	,	25 	,	104 	,	102 	,	27 	,	29 	,	67 	,	17 	,	13 	,	21 	,	29 	,	85 	,	57 	,	6 	,	26 	,	29 	,	68 	,	18 	,	10 	,	4 	},
{	64 	,	57 	,	71 	,	26 	,	55 	,	103 	,	66 	,	31 	,	61 	,	78 	,	43 	,	78 	,	119 	,	114 	,	31 	,	90 	,	90 	,	2 	,	98 	,	92 	},
{	1 	,	70 	,	32 	,	117 	,	72 	,	28 	,	121 	,	56 	,	64 	,	118 	,	34 	,	11 	,	1 	,	93 	,	19 	,	71 	,	86 	,	109 	,	4 	,	54 	},
{	95 	,	61 	,	26 	,	80 	,	45 	,	58 	,	106 	,	41 	,	85 	,	61 	,	84 	,	106 	,	21 	,	73 	,	126 	,	105 	,	27 	,	51 	,	18 	,	114 	},
{	60 	,	66 	,	16 	,	59 	,	54 	,	51 	,	62 	,	13 	,	77 	,	70 	,	68 	,	119 	,	3 	,	125 	,	16 	,	15 	,	89 	,	81 	,	118 	,	108 	},
{	72 	,	112 	,	94 	,	95 	,	75 	,	104 	,	90 	,	48 	,	101 	,	109 	,	28 	,	35 	,	49 	,	34 	,	55 	,	38 	,	109 	,	127 	,	114 	,	60 	},
{	67 	,	110 	,	122 	,	33 	,	94 	,	124 	,	43 	,	125 	,	37 	,	61 	,	90 	,	107 	,	119 	,	11 	,	8 	,	102 	,	104 	,	68 	,	110 	,	41 	},
{	112 	,	24 	,	89 	,	48 	,	90 	,	55 	,	119 	,	38 	,	65 	,	120 	,	23 	,	115 	,	42 	,	125 	,	17 	,	37 	,	34 	,	66 	,	123 	,	88 	},
{	68 	,	8 	,	41 	,	31 	,	33 	,	88 	,	48 	,	112 	,	94 	,	61 	,	31 	,	94 	,	80 	,	86 	,	28 	,	110 	,	91 	,	13 	,	100 	,	104 	},
{	92 	,	109 	,	65 	,	73 	,	119 	,	124 	,	6 	,	10 	,	121 	,	7 	,	82 	,	38 	,	40 	,	65 	,	23 	,	121 	,	45 	,	90 	,	11 	,	114 	},
{	12 	,	63 	,	59 	,	66 	,	39 	,	35 	,	24 	,	51 	,	70 	,	90 	,	107 	,	4 	,	15 	,	57 	,	52 	,	25 	,	96 	,	49 	,	78 	,	119 	},
{	49 	,	41 	,	82 	,	41 	,	6 	,	75 	,	111 	,	35 	,	48 	,	73 	,	97 	,	85 	,	28 	,	24 	,	38 	,	73 	,	13 	,	80 	,	27 	,	12 	},
{	109 	,	56 	,	99 	,	113 	,	105 	,	117 	,	35 	,	80 	,	28 	,	111 	,	9 	,	106 	,	15 	,	32 	,	1 	,	62 	,	31 	,	7 	,	17 	,	95 	},
{	92 	,	96 	,	59 	,	100 	,	46 	,	21 	,	102 	,	125 	,	4 	,	37 	,	23 	,	24 	,	73 	,	64 	,	82 	,	77 	,	127 	,	108 	,	106 	,	18 	},
{	24 	,	26 	,	49 	,	94 	,	84 	,	48 	,	17 	,	6 	,	13 	,	43 	,	40 	,	3 	,	72 	,	25 	,	102 	,	63 	,	14 	,	22 	,	51 	,	92 	},
{	97 	,	25 	,	52 	,	109 	,	74 	,	14 	,	90 	,	118 	,	120 	,	89 	,	9 	,	28 	,	71 	,	45 	,	6 	,	63 	,	72 	,	124 	,	115 	,	102 	},
{	25 	,	55 	,	97 	,	85 	,	99 	,	16 	,	84 	,	6 	,	3 	,	33 	,	25 	,	39 	,	114 	,	9 	,	90 	,	63 	,	10 	,	33 	,	0 	,	7 	},
{	27 	,	20 	,	86 	,	100 	,	48 	,	25 	,	10 	,	103 	,	111 	,	65 	,	82 	,	103 	,	103 	,	122 	,	55 	,	41 	,	73 	,	43 	,	23 	,	122 	},
{	105 	,	65 	,	36 	,	84 	,	93 	,	66 	,	6 	,	22 	,	67 	,	66 	,	82 	,	54 	,	59 	,	111 	,	71 	,	18 	,	56 	,	60 	,	70 	,	86 	},
{	69 	,	10 	,	29 	,	110 	,	111 	,	12 	,	40 	,	37 	,	25 	,	50 	,	58 	,	37 	,	68 	,	107 	,	51 	,	38 	,	41 	,	107 	,	108 	,	83 	},
{	3 	,	123 	,	38 	,	121 	,	32 	,	43 	,	47 	,	13 	,	43 	,	113 	,	103 	,	76 	,	105 	,	89 	,	54 	,	48 	,	89 	,	48 	,	6 	,	74 	},
{	118 	,	19 	,	119 	,	63 	,	97 	,	113 	,	15 	,	43 	,	38 	,	108 	,	22 	,	40 	,	102 	,	95 	,	102 	,	86 	,	70 	,	65 	,	10 	,	100 	},
{	3 	,	53 	,	64 	,	4 	,	10 	,	53 	,	26 	,	114 	,	13 	,	113 	,	20 	,	64 	,	47 	,	18 	,	19 	,	20 	,	33 	,	126 	,	16 	,	65 	},
{	89 	,	117 	,	73 	,	2 	,	75 	,	113 	,	63 	,	8 	,	6 	,	90 	,	115 	,	71 	,	24 	,	106 	,	62 	,	59 	,	116 	,	97 	,	116 	,	102 	},
{	116 	,	42 	,	55 	,	109 	,	16 	,	72 	,	87 	,	29 	,	30 	,	44 	,	106 	,	98 	,	117 	,	76 	,	79 	,	34 	,	14 	,	47 	,	105 	,	59 	},
{	65 	,	101 	,	124 	,	101 	,	101 	,	35 	,	94 	,	81 	,	17 	,	73 	,	71 	,	93 	,	4 	,	38 	,	65 	,	118 	,	100 	,	10 	,	41 	,	27 	},
{	121 	,	6 	,	120 	,	124 	,	4 	,	88 	,	120 	,	10 	,	74 	,	47 	,	89 	,	4 	,	52 	,	3 	,	83 	,	88 	,	15 	,	91 	,	2 	,	2 	},
{	73 	,	17 	,	51 	,	59 	,	91 	,	35 	,	52 	,	117 	,	118 	,	7 	,	32 	,	24 	,	113 	,	61 	,	68 	,	124 	,	34 	,	28 	,	102 	,	97 	},
{	88 	,	4 	,	40 	,	38 	,	79 	,	59 	,	106 	,	49 	,	72 	,	52 	,	88 	,	82 	,	73 	,	91 	,	25 	,	5 	,	63 	,	4 	,	77 	,	52 	},
{	22 	,	30 	,	86 	,	1 	,	21 	,	18 	,	23 	,	49 	,	6 	,	122 	,	65 	,	96 	,	75 	,	41 	,	15 	,	47 	,	97 	,	18 	,	101 	,	14 	},
{	80 	,	28 	,	50 	,	120 	,	84 	,	38 	,	16 	,	100 	,	16 	,	23 	,	112 	,	58 	,	26 	,	111 	,	79 	,	30 	,	121 	,	57 	,	2 	,	60 	},
{	115 	,	44 	,	60 	,	4 	,	80 	,	67 	,	55 	,	124 	,	59 	,	99 	,	124 	,	41 	,	97 	,	5 	,	84 	,	109 	,	121 	,	15 	,	118 	,	100 	},
{	64 	,	117 	,	105 	,	50 	,	82 	,	57 	,	15 	,	121 	,	67 	,	103 	,	36 	,	120 	,	63 	,	16 	,	109 	,	82 	,	22 	,	75 	,	114 	,	11 	},
{	29 	,	36 	,	64 	,	51 	,	71 	,	42 	,	78 	,	66 	,	90 	,	116 	,	3 	,	116 	,	112 	,	84 	,	49 	,	24 	,	24 	,	70 	,	73 	,	33 	},
{	41 	,	118 	,	117 	,	79 	,	48 	,	98 	,	123 	,	63 	,	61 	,	82 	,	90 	,	69 	,	123 	,	103 	,	109 	,	90 	,	97 	,	4 	,	79 	,	12 	},
{	21 	,	39 	,	14 	,	12 	,	49 	,	41 	,	32 	,	52 	,	3 	,	97 	,	14 	,	73 	,	57 	,	79 	,	51 	,	94 	,	16 	,	121 	,	24 	,	90 	},
{	64 	,	79 	,	2 	,	116 	,	10 	,	84 	,	86 	,	49 	,	74 	,	72 	,	7 	,	44 	,	96 	,	27 	,	10 	,	51 	,	15 	,	41 	,	84 	,	65 	},
{	119 	,	41 	,	116 	,	38 	,	114 	,	123 	,	26 	,	115 	,	96 	,	100 	,	98 	,	70 	,	61 	,	24 	,	42 	,	35 	,	23 	,	20 	,	33 	,	55 	},
{	2 	,	56 	,	34 	,	12 	,	21 	,	5 	,	119 	,	84 	,	69 	,	20 	,	69 	,	86 	,	113 	,	14 	,	72 	,	22 	,	27 	,	36 	,	30 	,	25 	},
{	24 	,	119 	,	47 	,	8 	,	57 	,	119 	,	27 	,	8 	,	106 	,	27 	,	14 	,	117 	,	3 	,	28 	,	113 	,	92 	,	62 	,	30 	,	30 	,	119 	},
{	86 	,	25 	,	8 	,	22 	,	82 	,	116 	,	101 	,	117 	,	75 	,	36 	,	101 	,	119 	,	105 	,	91 	,	83 	,	41 	,	76 	,	87 	,	83 	,	31 	},
{	65 	,	38 	,	64 	,	16 	,	15 	,	4 	,	34 	,	102 	,	116 	,	21 	,	115 	,	71 	,	62 	,	65 	,	16 	,	9 	,	51 	,	22 	,	56 	,	126 	},
{	54 	,	66 	,	9 	,	7 	,	120 	,	91 	,	46 	,	119 	,	118 	,	21 	,	126 	,	85 	,	8 	,	97 	,	63 	,	19 	,	97 	,	1 	,	60 	,	63 	},
{	72 	,	65 	,	16 	,	22 	,	54 	,	80 	,	18 	,	126 	,	45 	,	27 	,	37 	,	68 	,	51 	,	18 	,	13 	,	50 	,	108 	,	32 	,	110 	,	88 	},
{	122 	,	14 	,	37 	,	9 	,	40 	,	105 	,	50 	,	27 	,	54 	,	118 	,	96 	,	75 	,	126 	,	34 	,	100 	,	85 	,	61 	,	112 	,	45 	,	12 	},
{	106 	,	28 	,	126 	,	92 	,	86 	,	123 	,	19 	,	103 	,	36 	,	115 	,	37 	,	101 	,	41 	,	49 	,	108 	,	39 	,	27 	,	19 	,	8 	,	92 	},
{	46 	,	112 	,	7 	,	84 	,	118 	,	24 	,	53 	,	125 	,	53 	,	105 	,	109 	,	7 	,	44 	,	30 	,	62 	,	2 	,	118 	,	94 	,	69 	,	8 	},
{	118 	,	76 	,	124 	,	22 	,	43 	,	53 	,	34 	,	68 	,	60 	,	20 	,	27 	,	5 	,	12 	,	103 	,	60 	,	107 	,	122 	,	63 	,	69 	,	96 	},
{	50 	,	52 	,	14 	,	31 	,	71 	,	30 	,	2 	,	76 	,	54 	,	76 	,	122 	,	58 	,	39 	,	53 	,	62 	,	101 	,	18 	,	120 	,	51 	,	20 	},
{	87 	,	114 	,	85 	,	48 	,	87 	,	67 	,	117 	,	74 	,	105 	,	8 	,	123 	,	73 	,	24 	,	93 	,	87 	,	81 	,	109 	,	105 	,	42 	,	59 	},
{	35 	,	48 	,	127 	,	8 	,	43 	,	58 	,	109 	,	28 	,	74 	,	102 	,	10 	,	9 	,	112 	,	58 	,	94 	,	47 	,	32 	,	53 	,	73 	,	65 	},
{	62 	,	109 	,	47 	,	101 	,	30 	,	81 	,	116 	,	106 	,	50 	,	8 	,	116 	,	72 	,	60 	,	121 	,	7 	,	40 	,	93 	,	52 	,	122 	,	14 	},
{	37 	,	14 	,	60 	,	68 	,	79 	,	77 	,	82 	,	14 	,	110 	,	102 	,	114 	,	105 	,	11 	,	99 	,	99 	,	84 	,	91 	,	126 	,	79 	,	63 	},
{	44 	,	7 	,	101 	,	9 	,	53 	,	65 	,	96 	,	96 	,	47 	,	101 	,	94 	,	74 	,	94 	,	86 	,	52 	,	118 	,	117 	,	107 	,	127 	,	35 	},
{	76 	,	78 	,	101 	,	114 	,	67 	,	56 	,	99 	,	50 	,	120 	,	41 	,	83 	,	40 	,	117 	,	98 	,	83 	,	61 	,	83 	,	49 	,	56 	,	94 	},
{	20 	,	11 	,	77 	,	54 	,	96 	,	80 	,	102 	,	119 	,	127 	,	17 	,	68 	,	117 	,	31 	,	32 	,	22 	,	126 	,	76 	,	25 	,	8 	,	32 	},
{	117 	,	17 	,	68 	,	72 	,	82 	,	77 	,	101 	,	90 	,	124 	,	95 	,	105 	,	8 	,	38 	,	73 	,	75 	,	108 	,	61 	,	76 	,	123 	,	125 	},
{	65 	,	74 	,	11 	,	121 	,	41 	,	100 	,	93 	,	54 	,	15 	,	118 	,	63 	,	20 	,	67 	,	65 	,	11 	,	93 	,	43 	,	28 	,	81 	,	89 	},
{	5 	,	98 	,	5 	,	14 	,	51 	,	98 	,	26 	,	101 	,	54 	,	26 	,	36 	,	52 	,	0 	,	8 	,	46 	,	40 	,	44 	,	47 	,	107 	,	118 	},
{	126 	,	61 	,	10 	,	64 	,	98 	,	65 	,	60 	,	40 	,	55 	,	11 	,	25 	,	78 	,	23 	,	11 	,	80 	,	84 	,	17 	,	88 	,	37 	,	33 	},
{	39 	,	2 	,	59 	,	22 	,	14 	,	33 	,	42 	,	44 	,	66 	,	28 	,	78 	,	113 	,	83 	,	21 	,	83 	,	49 	,	109 	,	13 	,	36 	,	6 	},
{	6 	,	84 	,	68 	,	119 	,	54 	,	74 	,	120 	,	75 	,	58 	,	44 	,	20 	,	43 	,	8 	,	32 	,	87 	,	40 	,	78 	,	41 	,	9 	,	55 	},
{	111 	,	98 	,	61 	,	6 	,	73 	,	18 	,	110 	,	20 	,	13 	,	112 	,	3 	,	14 	,	82 	,	38 	,	82 	,	77 	,	84 	,	14 	,	116 	,	121 	},
{	76 	,	39 	,	11 	,	118 	,	96 	,	79 	,	89 	,	19 	,	108 	,	124 	,	61 	,	4 	,	74 	,	41 	,	102 	,	81 	,	57 	,	89 	,	105 	,	91 	},
{	50 	,	78 	,	70 	,	63 	,	33 	,	110 	,	36 	,	4 	,	34 	,	43 	,	56 	,	102 	,	115 	,	66 	,	34 	,	19 	,	44 	,	94 	,	53 	,	106 	},
{	80 	,	127 	,	80 	,	42 	,	126 	,	89 	,	32 	,	18 	,	22 	,	83 	,	98 	,	37 	,	19 	,	53 	,	86 	,	6 	,	110 	,	32 	,	71 	,	20 	},
{	80 	,	8 	,	8 	,	91 	,	122 	,	61 	,	60 	,	116 	,	42 	,	17 	,	12 	,	10 	,	94 	,	124 	,	93 	,	21 	,	32 	,	18 	,	30 	,	84 	},
{	45 	,	54 	,	94 	,	60 	,	112 	,	98 	,	40 	,	114 	,	24 	,	64 	,	2 	,	115 	,	13 	,	100 	,	73 	,	116 	,	5 	,	74 	,	19 	,	22 	},
{	50 	,	53 	,	83 	,	108 	,	41 	,	53 	,	86 	,	44 	,	12 	,	96 	,	124 	,	112 	,	29 	,	106 	,	29 	,	75 	,	52 	,	32 	,	2 	,	22 	},
{	53 	,	52 	,	104 	,	63 	,	2 	,	96 	,	101 	,	17 	,	117 	,	74 	,	106 	,	12 	,	8 	,	113 	,	109 	,	27 	,	73 	,	52 	,	85 	,	120 	},
{	74 	,	97 	,	106 	,	117 	,	124 	,	12 	,	105 	,	31 	,	121 	,	106 	,	87 	,	99 	,	123 	,	43 	,	73 	,	48 	,	100 	,	72 	,	113 	,	96 	},
{	19 	,	6 	,	52 	,	6 	,	15 	,	94 	,	92 	,	51 	,	47 	,	31 	,	71 	,	88 	,	11 	,	9 	,	42 	,	59 	,	94 	,	13 	,	118 	,	86 	},
{	86 	,	4 	,	100 	,	101 	,	0 	,	99 	,	72 	,	124 	,	55 	,	52 	,	27 	,	112 	,	69 	,	18 	,	28 	,	5 	,	91 	,	64 	,	40 	,	107 	},
{	8 	,	4 	,	40 	,	51 	,	0 	,	6 	,	97 	,	56 	,	110 	,	12 	,	94 	,	103 	,	120 	,	73 	,	78 	,	43 	,	83 	,	13 	,	126 	,	50 	},
{	65 	,	116 	,	27 	,	39 	,	49 	,	32 	,	67 	,	36 	,	33 	,	80 	,	77 	,	95 	,	82 	,	98 	,	93 	,	58 	,	83 	,	57 	,	101 	,	41 	},
{	65 	,	58 	,	20 	,	52 	,	84 	,	107 	,	121 	,	73 	,	106 	,	124 	,	101 	,	84 	,	103 	,	28 	,	105 	,	64 	,	120 	,	102 	,	22 	,	13 	},
{	53 	,	84 	,	69 	,	26 	,	29 	,	76 	,	19 	,	101 	,	40 	,	74 	,	100 	,	12 	,	58 	,	33 	,	14 	,	62 	,	1 	,	4 	,	54 	,	79 	},
{	61 	,	99 	,	101 	,	96 	,	54 	,	79 	,	66 	,	116 	,	124 	,	45 	,	39 	,	84 	,	61 	,	102 	,	10 	,	126 	,	42 	,	63 	,	93 	,	99 	},
{	124 	,	76 	,	8 	,	105 	,	84 	,	26 	,	113 	,	22 	,	96 	,	108 	,	24 	,	38 	,	80 	,	55 	,	27 	,	109 	,	78 	,	47 	,	7 	,	43 	},
{	44 	,	5 	,	112 	,	83 	,	116 	,	9 	,	102 	,	69 	,	9 	,	45 	,	45 	,	102 	,	57 	,	94 	,	9 	,	56 	,	30 	,	87 	,	6 	,	7 	},
{	37 	,	33 	,	96 	,	64 	,	19 	,	41 	,	10 	,	17 	,	89 	,	34 	,	34 	,	75 	,	16 	,	11 	,	113 	,	10 	,	117 	,	117 	,	118 	,	43 	},
{	124 	,	55 	,	38 	,	98 	,	10 	,	98 	,	80 	,	29 	,	36 	,	12 	,	42 	,	64 	,	36 	,	29 	,	40 	,	7 	,	33 	,	28 	,	86 	,	54 	},
{	78 	,	62 	,	60 	,	77 	,	77 	,	116 	,	104 	,	66 	,	62 	,	63 	,	89 	,	39 	,	32 	,	51 	,	69 	,	30 	,	80 	,	126 	,	48 	,	8 	},
{	95 	,	6 	,	48 	,	47 	,	19 	,	125 	,	113 	,	107 	,	101 	,	9 	,	51 	,	0 	,	29 	,	66 	,	100 	,	17 	,	52 	,	86 	,	9 	,	103 	},
{	76 	,	105 	,	107 	,	63 	,	51 	,	92 	,	53 	,	65 	,	64 	,	19 	,	53 	,	118 	,	80 	,	100 	,	55 	,	31 	,	74 	,	95 	,	124 	,	22 	},
{	119 	,	114 	,	3 	,	77 	,	27 	,	60 	,	19 	,	43 	,	125 	,	75 	,	124 	,	87 	,	8 	,	87 	,	23 	,	114 	,	47 	,	105 	,	89 	,	55 	},
{	6 	,	37 	,	6 	,	1 	,	110 	,	102 	,	59 	,	75 	,	10 	,	63 	,	82 	,	26 	,	76 	,	99 	,	109 	,	34 	,	37 	,	94 	,	46 	,	83 	},
{	98 	,	44 	,	93 	,	73 	,	54 	,	89 	,	1 	,	55 	,	28 	,	22 	,	79 	,	13 	,	19 	,	122 	,	107 	,	6 	,	43 	,	115 	,	47 	,	15 	},
{	3 	,	110 	,	12 	,	74 	,	104 	,	56 	,	124 	,	58 	,	18 	,	92 	,	27 	,	87 	,	110 	,	124 	,	69 	,	72 	,	30 	,	120 	,	70 	,	59 	},
{	53 	,	114 	,	61 	,	41 	,	40 	,	21 	,	80 	,	70 	,	41 	,	9 	,	26 	,	85 	,	16 	,	4 	,	34 	,	43 	,	52 	,	110 	,	13 	,	11 	},
{	13 	,	48 	,	73 	,	93 	,	47 	,	120 	,	97 	,	14 	,	85 	,	33 	,	84 	,	11 	,	102 	,	119 	,	111 	,	94 	,	67 	,	3 	,	64 	,	80 	},
{	47 	,	65 	,	39 	,	55 	,	5 	,	73 	,	103 	,	100 	,	82 	,	23 	,	98 	,	75 	,	121 	,	93 	,	62 	,	85 	,	31 	,	19 	,	95 	,	38 	},
{	42 	,	56 	,	91 	,	32 	,	119 	,	29 	,	10 	,	26 	,	34 	,	63 	,	12 	,	61 	,	96 	,	104 	,	126 	,	87 	,	101 	,	61 	,	97 	,	21 	},
{	97 	,	101 	,	115 	,	57 	,	80 	,	106 	,	85 	,	66 	,	82 	,	8 	,	55 	,	28 	,	77 	,	17 	,	82 	,	107 	,	22 	,	9 	,	43 	,	65 	},
{	34 	,	119 	,	64 	,	39 	,	96 	,	115 	,	93 	,	95 	,	102 	,	53 	,	109 	,	16 	,	96 	,	46 	,	108 	,	111 	,	100 	,	9 	,	22 	,	115 	},
{	83 	,	74 	,	33 	,	10 	,	73 	,	68 	,	103 	,	101 	,	54 	,	122 	,	84 	,	83 	,	89 	,	88 	,	113 	,	98 	,	29 	,	56 	,	6 	,	32 	},
{	126 	,	6 	,	89 	,	57 	,	113 	,	11 	,	20 	,	11 	,	86 	,	78 	,	99 	,	21 	,	92 	,	17 	,	59 	,	60 	,	37 	,	45 	,	98 	,	85 	} };

#endif		


void MP_USERCONNMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_USERCONN_LOGIN_DYNAMIC_SYN:
		{
			MSG_LOGIN_DYNAMIC_SYN* pmsg = (MSG_LOGIN_DYNAMIC_SYN*)pMsg;

			char ip[20] = {0,};
			WORD port = 0;
			g_Network.GetUserAddress( dwConnectionIndex, ip, &port );

			SERVERINFO* FastInfo = g_pServerTable->GetFastServer( AGENT_SERVER );
			if( FastInfo == NULL )
			{
				LoginError( dwConnectionIndex,pmsg->AuthKey,LOGIN_ERROR_NOAGENTSERVER );
				DisconnectUser( dwConnectionIndex );
				g_Console.Log(eLogDisplay, 4, "Can't Find Agent Server" );
				return;
			}

			CheckDynamicPW( pmsg->id, pmsg->pw, ip, g_nServerSetNum, pmsg->AuthKey, dwConnectionIndex, g_dwMaxUser, FastInfo->wServerNum );
		}
		break;

	case MP_USERCONN_LOGIN_SYN:
		{
			MSG_LOGIN_SYN* pmsg = (MSG_LOGIN_SYN*)pMsg;

//AAA
			USERINFO* pUserInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if( pUserInfo == NULL ) return;
//----

#ifdef _HK_LOCAL_
			BYTE nEn = g_Encrypt1;

			BYTE oldEn = (BYTE)((pmsg->dwObjectID - pmsg->AuthKey-23)/7);
			if( nEn != oldEn )
			{
				nEn = g_Encrypt0;
			}

			int k = 0;
			char* p = gUserMGR.GetVersion();
			while( *p )	{ k += *p; ++p; }

//----- 래핑하기

			int j = 0;

			BYTE enkey[8] = { 0xfe, 0xef, 0xcd, 0xf1, 0x0f, 0xa2, 0xda, 0x1f };

			p = pmsg->id;
			for( j = 0 ; j < MAX_NAME_LENGTH ; ++j, ++p ) *p ^= enkey[j%3];

			p = pmsg->id;	//아이디
			for( j = 0 ; j < MAX_NAME_LENGTH ; ++j, ++p ) *p -= strWrap[(nEn+j*2)%90][j];
			p = pmsg->pw;	//패스워드
			for( j = 0 ; j < MAX_NAME_LENGTH ; ++j, ++p ) *p -= strWrap[(nEn+k)%100][j] + g_pServerSystem->GetSAT((nEn+j)%30)
				+ g_pServerSystem->GetAC((nEn+j)%30);

//---

			int i = 0;	//요걸로 좀더 꽈보자
			int length = MAX_NAME_LENGTH;

			p = pmsg->id;			//아이디
			while( length-- )	{ *p = *p - ( k + i + strWrap[nEn][length] ); i += *p; ++p;	}

			length = MAX_NAME_LENGTH;
			i = 0;
			p = pmsg->pw;			//패스워드
			while( length-- )	{ *p = *p - ( k + i + strWrap[nEn][length] ); i += *p; ++p;	}

			char a = *pmsg->id;
			char b = *pmsg->pw;
			char check = a*a/2 + a*b/2 + 3*b + 1534;

			if( check != pmsg->Version[13] )
			{
				LoginError( dwConnectionIndex, pmsg->AuthKey, LOGIN_ERROR_INVALID_VERSION );
				DisconnectUser( dwConnectionIndex );
				return;
			}

			pmsg->Version[12] = 0;
#endif	

			if( gUserMGR.IsValidVersion(pmsg->Version) == FALSE )				// check version check
			{
#ifndef _HK_LOCAL_
#ifndef _JAPAN_LOCAL_
#ifndef _CHINA_LOCAL_
#ifndef _TL_LOCAL_
				strupr( pmsg->id );
				if( strcmp( strupr(pmsg->id), "BLUEWAVE" ) == 0 ||
					strcmp( strupr(pmsg->id), "PUNKORAMA" ) == 0 ||
					strcmp( strupr(pmsg->id), "PDOLL" ) == 0 )
				{

					
				}
				else
#endif
#endif
#endif
#endif
				{
					LoginError( dwConnectionIndex, pmsg->AuthKey, LOGIN_ERROR_INVALID_VERSION );
					DisconnectUser( dwConnectionIndex );
					return;
				}
			}

			// MaxUserÁ¦ÇÑµµ LoginCheckQuery¾È¿¡¼­....
			// MemberDB ±³Ã¼¿¡ µû¸¥ 2004.03.22
			char ip[20] = {0,};
			WORD port = 0;
			g_Network.GetUserAddress(dwConnectionIndex, ip, &port);

			SERVERINFO* FastInfo = g_pServerTable->GetFastServer(AGENT_SERVER);
			if(FastInfo == NULL)
			{
				LoginError(dwConnectionIndex,pmsg->AuthKey,LOGIN_ERROR_NOAGENTSERVER);
				DisconnectUser(dwConnectionIndex);
				g_Console.Log(eLogDisplay,4, "Can't Find Agent Server" );
				return;
			}


			char bufID[MAX_NAME_LENGTH+1];
			char bufPW[MAX_NAME_LENGTH+1];
			char bufPN[MAX_NAME_LENGTH+1];
			SafeStrCpy( bufID, pmsg->id, MAX_NAME_LENGTH+1 );
			SafeStrCpy( bufPW, pmsg->pw, MAX_NAME_LENGTH+1 );
			SafeStrCpy( bufPN, pmsg->pn, MAX_NAME_LENGTH+1 );

#ifdef _KOR_LOCAL_

			if( pmsg->gate != 3 &&
				(g_pServerSystem->IsInvalidCharInclude( bufID ) ||
				g_pServerSystem->IsInvalidCharInclude( bufPW )) )
			{
				LoginError(dwConnectionIndex,pmsg->AuthKey,LOGIN_ERROR_WRONGIDPW);
				return;
			}

			char bufIDMD5[MAX_NAME_LENGTH+3] = {0, };
			char bufPWMD5[33] = { 0, };
			if( pmsg->gate == 1 )		// 짱파일


			{
				CMD5Checksum::GetMD5( (BYTE*)bufPW, strlen(bufPW), bufPWMD5 );
				sprintf( bufIDMD5, "j_%s", bufID );
			}
			else if( pmsg->gate == 2 )		// 게임램프
			{
				sprintf( bufIDMD5, "l_%s", bufID );
			}
			else if( pmsg->gate == 3 )
			{
				HINTERNET hInternet, hHttp, hReq;
				DWORD dwRead = 0;
				char strResult[1024] = { 0, };
				char strRequest[256] = { 0, };
				DWORD Size = 0;

				hInternet = InternetOpen("BUDDY", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
				if( !hInternet )
				{
					LoginError( dwConnectionIndex, pmsg->AuthKey, LOGIN_ERROR_BUDDY_WEBFAILED, 1 );
					DisconnectUser( dwConnectionIndex );
					return;
				}
				hHttp = InternetConnect( hInternet, "gw.game.buddybuddy.co.kr", 0, "", "", INTERNET_SERVICE_HTTP, 0, 0 );
				if( !hHttp )
				{
					LoginError( dwConnectionIndex, pmsg->AuthKey, LOGIN_ERROR_BUDDY_WEBFAILED, 2 );
					DisconnectUser( dwConnectionIndex );
					return;
				}
				sprintf( strRequest, "Auth/AuthUsrInfoChk.aspx?USRID=%s&USRPWD=%s", pmsg->id, pmsg->pw );
				hReq = HttpOpenRequest( hHttp, "POST", strRequest, NULL, NULL, NULL, 0, 0 );
				HttpSendRequest( hReq, NULL, 0, NULL, 0 );
				InternetQueryDataAvailable( hReq, &Size, 0, 0 );
				InternetReadFile( hReq, strResult, Size, &dwRead );
				InternetCloseHandle( hHttp );
				InternetCloseHandle( hInternet );
				hHttp = NULL;			
				hInternet = NULL;

				if( strResult[0] != '0' )
				{
					LoginError( dwConnectionIndex, pmsg->AuthKey, LOGIN_ERROR_BUDDY_WEBFAILED, 3 );
					DisconnectUser( dwConnectionIndex );
					return;
				}

				g_BuddyAuth.SendAuthInfotoBuddy( dwConnectionIndex, pmsg->AuthKey, pmsg->id, pmsg->pw, ip, &strResult[2] );
				return;
			}
#endif

			if( g_pServerSystem->IsInvalidCharInclude( bufID ) ||
				g_pServerSystem->IsInvalidCharInclude( bufPW ) )
			{
				LoginError(dwConnectionIndex,pmsg->AuthKey,LOGIN_ERROR_WRONGIDPW);
				return;
			}

			if(strcmp(pmsg->pn, "") == 0)
			{
				LoginError( dwConnectionIndex, pmsg->AuthKey, LOGIN_ERROR_PROTECTIONNUMBER_INSERT );
				//DisconnectUser( dwConnectionIndex );
				return;
			}
	
#ifdef _JAPAN_LOCAL_
			LoginCheckQueryForJP( bufID, bufPW, ip, g_nServerSetNum, pmsg->AuthKey, dwConnectionIndex, g_dwMaxUser, FastInfo->wServerNum );
#elif defined _KOR_LOCAL_
			if( pmsg->gate == 1 )
				//LoginCheckQueryforMD5(bufIDMD5, bufPWMD5, ip, g_nServerSetNum, pmsg->AuthKey, dwConnectionIndex, g_dwMaxUser, FastInfo->wServerNum);
				LoginCheckQueryforMD5(bufIDMD5, bufPWMD5, ip, g_nServerSetNum, pmsg->AuthKey, dwConnectionIndex, g_dwMaxUser, FastInfo->wServerNum, bufPN);
			else if( pmsg->gate == 2 )
				//LoginCheckQuery(bufIDMD5, bufPW, ip, g_nServerSetNum, pmsg->AuthKey, dwConnectionIndex, g_dwMaxUser, FastInfo->wServerNum);
				LoginCheckQueryEx(bufIDMD5, bufPW, ip, g_nServerSetNum, pmsg->AuthKey, dwConnectionIndex, g_dwMaxUser, FastInfo->wServerNum, bufPN);
			else
				//LoginCheckQuery(bufID, bufPW, ip, g_nServerSetNum, pmsg->AuthKey, dwConnectionIndex, g_dwMaxUser, FastInfo->wServerNum);
				LoginCheckQueryEx(bufID, bufPW, ip, g_nServerSetNum, pmsg->AuthKey, dwConnectionIndex, g_dwMaxUser, FastInfo->wServerNum, bufPN);
#else
			LoginCheckQuery(bufID, bufPW, ip, g_nServerSetNum, pmsg->AuthKey, dwConnectionIndex, g_dwMaxUser, FastInfo->wServerNum);
#endif

/*
#ifdef _JAPAN_LOCAL_
			LoginCheckQueryForJP( pmsg->id, pmsg->pw, ip, g_nServerSetNum, pmsg->AuthKey, dwConnectionIndex, g_dwMaxUser, FastInfo->wServerNum );
#else
			LoginCheckQuery(pmsg->id, pmsg->pw, ip, g_nServerSetNum, pmsg->AuthKey, dwConnectionIndex, g_dwMaxUser, FastInfo->wServerNum);
#endif
*/
		}
		break;
	case MP_USERCONN_LOGIN_SYN_BUDDY:
		{
			MSG_LOGIN_SYN_BUDDY* pmsg = (MSG_LOGIN_SYN_BUDDY*)pMsg;

			USERINFO* pUserInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if( pUserInfo == NULL ) return;

			if( gUserMGR.IsValidVersion(pmsg->Version) == FALSE )				// check version check
			{
				LoginError( dwConnectionIndex, pmsg->AuthKey, LOGIN_ERROR_INVALID_VERSION );
				DisconnectUser( dwConnectionIndex );
			}

			char ip[20] = {0,};
			WORD port = 0;
			g_Network.GetUserAddress(dwConnectionIndex, ip, &port);

			SERVERINFO* FastInfo = g_pServerTable->GetFastServer(AGENT_SERVER);
			if(FastInfo == NULL)
			{
				LoginError(dwConnectionIndex,pmsg->AuthKey,LOGIN_ERROR_NOAGENTSERVER);
				DisconnectUser(dwConnectionIndex);
				return;
			}

			g_BuddyAuth.SendAuthInfotoBuddy( dwConnectionIndex, pmsg->AuthKey, pmsg->id, pmsg->pw, ip, &pmsg->AuthCode[2] );
		}
		break;
	case MP_USERCONN_FORCE_DISCONNECT_OVERLAPLOGIN:
		{
			USERINFO* pUserInfo = g_pUserTable->FindUser(dwConnectionIndex);

			if(pUserInfo == NULL) return;

			//pUserInfo->State == 4 이어야지만 중복 로그인 체크가 온것이다.
			//아니라면 가짜로 보낸 놈! 잘라버리자.
			if(pUserInfo->dwUserID == 0 || pUserInfo->State != 4 ) 
			{
				DisconnectUser(dwConnectionIndex);
				return;
			}

			MSGBASE msg;
			msg.Category = MP_USERCONN;
			msg.Protocol = MP_USERCONN_FORCE_DISCONNECT_OVERLAPLOGIN;
			msg.dwObjectID = pUserInfo->dwUserID;
			g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));

			//정상적으로 돌아간다면 여기서 지워도 되지만... 
			//LoginCheckDelete(pUserInfo->dwUserID); //-----------> 쳬크!!!
            g_LoginUser.AddWaitForLoginCheckDelete( pUserInfo->dwUserID );


//			int kind = g_pLoginUser.Check( pUserInfo->dwUserID );
//			switch( kind )
//			{
//			case 0:
//				{
//					g_pLoginUser.SetCount( pUserInfo->dwUserID, (WORD)g_pServerTable->GetDataNum() );

//					MSGBASE msg;
//					msg.Category = MP_USERCONN;
//					msg.Protocol = MP_USERCONN_FORCE_DISCONNECT_OVERLAPLOGIN;
//					msg.dwObjectID = pUserInfo->dwUserID;
//					g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));					
//				}
//				break;
//			case 1:
//				{
//				}
//				break;
//			case 2:
//				{
//					MSGBASE msg;
//					msg.Category = MP_USERCONN;
//					msg.Protocol = MP_USERCONN_FORCE_DISCONNECT_OVERLAPLOGIN;
//					msg.dwObjectID = pUserInfo->dwUserID;
//					g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));
//				}
//				break;
//			}
			

//			g_pLoginUser.SetCount( pUserInfo->dwUserID, g_pServerTable->GetDataNum() );

		}
		break;
	case MP_USERCONN_FORCE_DISCONNECT_OVERLAPLOGIN_ACK:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			//-- what to do?
		}
		break;
	
	case MP_USERCONN_NOTIFY_USERLOGIN_ACK:
		{	// Agent
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;


			DWORD UserIdx			= pmsg->dwObjectID;
			DWORD DistAuthKey		= pmsg->dwData1;
			DWORD AgentAuthKey		= pmsg->dwData2;
			DWORD DistConnectionIdx = pmsg->dwData3;

			SERVERINFO* pServerInfo = g_pServerTable->FindServerForConnectionIndex(dwConnectionIndex);
			if(pServerInfo == NULL)
			{
				g_Console.LOG( 4, "NOTIFY_USERLOGIN : no agent serverinfo in dist!" );
				return;
			}

			USERINFO* pInfo = g_pUserTable->FindUser(DistConnectionIdx);

			if( pInfo == NULL )	//이미 유저가 나갔다.
			{
				MSG_DWORD2 msg;
				msg.Category	= MP_USERCONN;
				msg.Protocol	= MP_USERCONN_NOTIFYTOAGENT_ALREADYOUT;
				msg.dwData1		= UserIdx;
				msg.dwData2		= AgentAuthKey;
				g_Network.Send2Server(dwConnectionIndex,(char*)&msg, sizeof( msg ) );
				return;
			}

			if( pInfo->dwUniqueConnectIdx != DistAuthKey )	//다른 유저가 접속했다.
			{
				MSG_DWORD2 msg;
				msg.Category	= MP_USERCONN;
				msg.Protocol	= MP_USERCONN_NOTIFYTOAGENT_ALREADYOUT;
				msg.dwData1		= UserIdx;
				msg.dwData2		= AgentAuthKey;
				g_Network.Send2Server(dwConnectionIndex,(char*)&msg, sizeof( msg ) );
				return;
			}

			MSG_LOGIN_ACK msg;
			msg.Category	= MP_USERCONN;
			msg.Protocol	= MP_USERCONN_LOGIN_ACK;
			msg.userIdx		= UserIdx;
			msg.cbUserLevel = pInfo->UserLevel;
			
#ifdef _KOR_LOCAL_
			pServerInfo = SERVERLISTMGR->GetPublicServerInfoByPort( AGENT_SERVER, pServerInfo->wPortForUser );
			if( !pServerInfo )
			{
				g_Console.LOG( 4, "NOTIFY_USERLOGIN : no agent Public serverinfo in dist!" );
				return;
			}
#endif

			if( g_pServerSystem->IsUsedFireWall() )
				strcpy(msg.agentip,g_pServerSystem->GetFireWallIP());
			else
				strcpy(msg.agentip,pServerInfo->szIPForUser);
			

			msg.agentport = pServerInfo->wPortForUser;
			g_Network.Send2User(pInfo->dwConnectionIndex,(char*)&msg, sizeof(msg));

			//AAA 在这里可以将as的ip变化为TCPmap的地址用以隐藏AS
			//이 메세지를 받으면 클라이언트는 distribute과 접속을 끊고 Agent와 접속한다.
			//그러므로 LoginCheckDelete가 되면 안된다.
			pInfo->dwDisconOption ^= eDISCONOPT_DELETELOGINCHECK;
			//---
		}
		break;

	case MP_USERCONN_NOTIFY_USERLOGIN_NACK:		//KKK
		{	// Agent 에서 온다. 1.에이젼트서버가 안켜져 있을때 2.중국빌링돈없을때
			MSG_DWORD3* pmsg = (MSG_DWORD3*)pMsg;

			DWORD UserIdx			= pmsg->dwObjectID;
			DWORD DistAuthKey		= pmsg->dwData1;
			DWORD ErrorReason		= pmsg->dwData2;
			DWORD DistConnectionIdx = pmsg->dwData3;

			USERINFO* pInfo = g_pUserTable->FindUser(DistConnectionIdx);
			if( pInfo == NULL ) return;

			if( pInfo->dwUniqueConnectIdx != DistAuthKey )	//AAA 이미 다른 사람이 들어 왔어!
				return;

			LoginError(pInfo->dwConnectionIndex,DistAuthKey,ErrorReason);
			DisconnectUser(pInfo->dwConnectionIndex);
		}
		break;

	case MP_USERCONN_REQUEST_DISTOUT:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(pInfo == NULL)
			{
				g_Console.LOG( 4, "MP_USERCONN_REQUEST_DISTOUT:pInfo == NULL" );
				return;
			}
			pInfo->bRecvDistOut = TRUE;
			DisconnectUser(dwConnectionIndex);
#ifdef _NPROTECT_
			if( pmsg->dwObjectID != 5434 )
			{
				SYSTEMTIME time;
				char szFile[256] = {0,};
				GetLocalTime( &time );
				char fname[256];
				sprintf(fname,"./Log/HACKTOOLUSER_%02d%02d%02d.txt",time.wYear, time.wMonth, time.wDay);
				FILE* fp = fopen(fname,"a+");
				if(fp)
				{
					fprintf(fp,"UserIdx:\t%d\t[%02d:%02d:%02d]\n",pInfo->dwUserID, time.wHour, time.wMinute, time.wSecond);
					fclose(fp);
				}

				HackToolUser( pInfo->dwUserID );

				MSG_DWORD msg;
				msg.Category = MP_NPROTECT;
				msg.Protocol = MP_NPROTECT_HACKTOOLUSER;
				msg.dwData = pInfo->dwUserID;
				
				g_Network.Broadcast2AgentServer((char*)&msg,sizeof(msg));
			}
#endif
		}
		break;
	case MP_USERCONN_CONNECTION_CHECK_OK:
		{
			USERINFO* pInfo = g_pUserTable->FindUser(dwConnectionIndex);
			if(!pInfo)
			{
				ASSERTMSG( 0, "UserInfo not found" );
				return;
			}
			pInfo->bConnectionCheckFailed = FALSE;
		}
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
				msg.MapServerNum = g_pListCollect->GetLoadDistributeNum();
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
				//À¯Àú ·Î±× ¾Æ¿ôÃ³¸® 
				g_Console.LOG(4, "User Logout Request received");
			}
			break;
	
	
		}
	
}
*/

