#include "StdAfx.h"
#include "WindowIdEnum.h"
#include "HackShieldManager.h"
#include "cMsgBox.h"
#include "./Interface/cWindowManager.h"
#include "ChatManager.h"



#ifdef _HACK_SHIELD_
GLOBALTON(CHackShieldManager);
#endif

CHackShieldManager::CHackShieldManager(void)
{
}

CHackShieldManager::~CHackShieldManager(void)
{
}

void CHackShieldManager::NetworkMsgParse(BYTE Protocol, void* pMsg)
{
#ifdef _HACK_SHIELD_
	switch(Protocol)
	{
	case MP_HACKSHIELD_GUID_REQ:
		{
			MSG_GUID_REQ* pmsg = (MSG_GUID_REQ*)pMsg;

			MSG_GUID_ACK msg;
			memset(&(msg.msg), 0, SIZEOF_GUIDACKMSG);

			msg.Category = MP_HACKSHIELD;
			msg.Protocol = MP_HACKSHIELD_GUID_ACK;

			DWORD dwRet = ERROR_SUCCESS;

			dwRet = _AhnHS_MakeGuidAckMsg(pmsg->msg, msg.msg);

			if(dwRet != ERROR_SUCCESS)
			{
			}
			else
			{
				NETWORK->Send( &msg, sizeof( msg ) );
			}
		}
		break;
	case MP_HACKSHIELD_REQ:
		{
			MSG_REQ* pmsg = (MSG_REQ*)pMsg;

			MSG_ACK msg;
			memset(&(msg.msg), 0, SIZEOF_ACKMSG);

			msg.Category = MP_HACKSHIELD;
			msg.Protocol = MP_HACKSHIELD_ACK;

			DWORD dwRet = ERROR_SUCCESS;

			dwRet = _AhnHS_MakeAckMsg(pmsg->msg, msg.msg);

			if(dwRet != ERROR_SUCCESS)
			{
			}
			else
			{
				NETWORK->Send( &msg, sizeof( msg ) );
			}
		}
		break;
	case MP_HACKSHIELD_DISCONNECT:
		{
			cMsgBox* pMsgBox = WINDOWMGR->MsgBox( MBI_SERVERDISCONNECT, MBT_OK, CHATMGR->GetChatMsg( 560 ) );
		}
		break;
	default:
		{
		}
		break;
	}
#endif
}
