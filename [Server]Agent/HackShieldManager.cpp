#include "StdAfx.h"
#include "HackShieldManager.h"
#include "Network.h"

#ifdef _HACK_SHIELD_
GLOBALTON(CHackShieldManager);
#endif

CHackShieldManager::CHackShieldManager(void)
{
}

CHackShieldManager::~CHackShieldManager(void)
{
}

void CHackShieldManager::NetworkMsgParse(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
#ifdef _HACK_SHIELD_	
	USERINFO* pUserInfo = g_pUserTable->FindUser(dwConnectionIndex);
	if( pUserInfo == NULL ) return;

	DWORD dwRet = ERROR_SUCCESS;

	MSGROOT* pTempMsg = (MSGROOT*)pMsg;
	switch(pTempMsg->Protocol)
	{
	case MP_HACKSHIELD_GUID_ACK:
		{
			MSG_GUID_ACK* pmsg = (MSG_GUID_ACK*)pMsg;
			
			dwRet = _AntiCpSvr_AnalyzeGuidAckMsg( pmsg->msg, pUserInfo->m_GUIDInfo, &(pUserInfo->m_pCrcInfo));
			pUserInfo->m_bHSCheck = FALSE;

			if(dwRet != ERROR_SUCCESS )
			{
				MSGBASE msg;
				msg.Category = MP_HACKSHIELD;
				msg.Protocol = MP_HACKSHIELD_DISCONNECT;
				g_Network.Send2User(pUserInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
				DisconnectUser(dwConnectionIndex);
			}
			else
			{
				MSG_REQ msg;
				memset(&(msg.msg), 0, SIZEOF_REQMSG);

				msg.Category = MP_HACKSHIELD;
				msg.Protocol = MP_HACKSHIELD_REQ;

				DWORD dwRet = ERROR_SUCCESS;

				dwRet = _AntiCpSvr_MakeReqMsg( pUserInfo->m_pCrcInfo, msg.msg, pUserInfo->m_Info, ANTICPSVR_CHECK_ALL);

				if ( dwRet != ERROR_SUCCESS )
				{

				}
				else
				{
					g_Network.Send2User(dwConnectionIndex,(char*)&msg,sizeof(msg));
					pUserInfo->m_bHSCheck = 2;
				}
			}
		}
		break;
	case MP_HACKSHIELD_ACK:
		{
			MSG_ACK* pmsg = (MSG_ACK*)pMsg;

			dwRet = _AntiCpSvr_AnalyzeAckMsg( pUserInfo->m_pCrcInfo, pmsg->msg, pUserInfo->m_Info);
			pUserInfo->m_bHSCheck = FALSE;

			if(dwRet != ERROR_SUCCESS )
			{
				MSGBASE msg;
				msg.Category = MP_HACKSHIELD;
				msg.Protocol = MP_HACKSHIELD_DISCONNECT;
				g_Network.Send2User(pUserInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
				DisconnectUser(dwConnectionIndex);
			}
		}
		break;
	default:
		{
		}
		break;
	}
#endif
}

// 클라이언트의 GUID를 얻어온다
void CHackShieldManager::SendGUIDReq(USERINFO* pUserInfo)
{
#ifdef _HACK_SHIELD_
	if( pUserInfo->UserLevel < eUSERLEVEL_SUPERUSER )
		return;

	MSG_GUID_REQ msg;

	// 버퍼 초기화
	memset(&(msg.msg), 0, SIZEOF_GUIDREQMSG);

	msg.Category = MP_HACKSHIELD;
	msg.Protocol = MP_HACKSHIELD_GUID_REQ;
	
	DWORD dwRet = ERROR_SUCCESS;
	
	// _AntiCpSvr_MakeGuidReqMsg의
	// 첫번째 인자는 클라이언트에 보낼 메세지 버퍼
	// 두번째 인자는 메세지 정보
	dwRet = _AntiCpSvr_MakeGuidReqMsg ( msg.msg, pUserInfo->m_GUIDInfo );
	
	if ( dwRet != ERROR_SUCCESS )
	{
		// 에러처리
	}
	else
	{
		g_Network.Send2User(pUserInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
		pUserInfo->m_bHSCheck = 2;
	}
#endif
}

void CHackShieldManager::SendReq(USERINFO* pUserInfo)
{
#ifdef _HACK_SHIELD_
	if( pUserInfo->UserLevel < eUSERLEVEL_SUPERUSER)
		return;

	if(pUserInfo->m_bHSCheck == 2)
	{
		// 루프외의 요청 메세지에 응답이 없었다.
		// 즉 GUID메세지와 최초 정보 요구 메세지는
		// 루프와 별도로 메세지를 보내므로
		// 요구메세지 보내자마자 이리로 들어올수도 있으므로 한번씩만 봐준다.
		// 다음 루프때까지 응답없으면 잘라버린다.
		pUserInfo->m_bHSCheck = 1;
		return;
	}
	if(pUserInfo->m_bHSCheck == 1)
	{
		// 먼저 보낸 메세지에 대한 응답이 없었다...
		MSGBASE msg;
		msg.Category = MP_HACKSHIELD;
		msg.Protocol = MP_HACKSHIELD_DISCONNECT;
		g_Network.Send2User(pUserInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
		DisconnectUser(pUserInfo->dwConnectionIndex);
		return;
	}

	MSG_REQ msg;
	memset(&(msg.msg), 0, SIZEOF_REQMSG);

	msg.Category = MP_HACKSHIELD;
	msg.Protocol = MP_HACKSHIELD_REQ;

	DWORD dwRet = ERROR_SUCCESS;

	// _AntiCpSvr_MakeReqMsg의
	// 첫번째 인자는 클라이언트가 사용할 CRC 정보에 대한 포인터
	// 두번째 인자는 클라이언트에 보낼 메세지
	// 세번째 인자는 메세지에 대한 정보
	// 네번째 인자는 옵션
	dwRet = _AntiCpSvr_MakeReqMsg( pUserInfo->m_pCrcInfo, msg.msg, pUserInfo->m_Info, ANTICPSVR_CHECK_GAME_MEMORY);

	if ( dwRet != ERROR_SUCCESS )
	{

	}
	else
	{
		g_Network.Send2User(pUserInfo->dwConnectionIndex,(char*)&msg,sizeof(msg));
		pUserInfo->m_bHSCheck = 1;
	}
#endif
}
