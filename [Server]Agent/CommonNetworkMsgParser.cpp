
#include "stdafx.h"
#include "CommonNetworkMsgParser.h"


//---------------------------------------------------------------------------------------------
//don't changed -------------------------------------------------------------------------------
MSGPARSER g_pServerMsgParser[MP_MAX];
MSGPARSER g_pUserMsgParser[MP_MAX];
//---------------------------------------------------------------------------------------------
void ErrorMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	//ASSERT(0);
}

/*
void MP_SERVERMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_SERVER_PWRUP:
		{
			MSG_WORD * msg = (MSG_WORD *)pMsg;
			SERVERINFO * info = g_pServerTable->FindServer(msg->wData);//port
			info->dwConnectionIndex = dwConnectionIndex;
		}
		break;
	}
}
*/

/*
void MP_PACKEDMsgParser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	MSGBASE* pTempMsg = (MSGBASE*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_PACKEDDATA_NORMAL:
		{
		//	Init(pMsg,dwLength);
		//	SendToAllReceiver();
			SEND_PACKED_DATA * pack =  (SEND_PACKED_DATA *)pMsg;
			DWORD* charterIDs = (DWORD*)&pack->Data[pack->wRealDataSize];
			for(int n=0;n<pack->wReceiverNum;++n)
			{
				//GAMESYSTEM_OBJ->m_ConnectionTable.SendToCharacter(charterIDs[n],pack->Data,pack->Data);
				USERINFO * userInfo = g_pUserTableForObjectID->FindUser(charterIDs[n]);
				g_Network.Send2User(userInfo->dwConnectionIndex, pack->Data, pack->wRealDataSize);
				
			}
		}
		break;
	}
}
*/