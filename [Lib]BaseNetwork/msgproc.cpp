#include "network.h"
#include "define.h"
//#include "recvmsg.h"
#include "main_msg_que.h"
#include "switch_que.h"

void SocketEventProc1()
{
	DWORD			dwConnectionIndex;
	CConnection*	pCon;

	OutputDebugString( "Socket Event On Processing\n" );

	MSG_CONTAINER*	pMsg;
	for (DWORD i=0; i<g_pMsgQue->m_pNext->GetMsgNum(); i++)
	{
		pMsg = g_pMsgQue->m_pNext->DispatchMsg();
			
		if (pMsg->m_dwType == MSG_TYPE_DISCONNECT)
		{
			pCon = g_pNetwork->GetConnection(pMsg->m_dwConnectionIndex);
			if (pCon)
			{
				g_pNetwork->OnDisconnect(pMsg->m_dwConnectionIndex);
				g_pNetwork->CloseConnection(pMsg->m_dwConnectionIndex);
			}
			continue;
		}
		if (pMsg->m_dwType == MSG_TYPE_ACCEPT)
		{
			dwConnectionIndex = g_pNetwork->AddConnection((SOCKET)pMsg->m_dwConnectionIndex);

			if (dwConnectionIndex)
			{
				pCon = g_pNetwork->GetConnection(dwConnectionIndex);
				g_pNetwork->OnConnect(dwConnectionIndex);
				g_pNetwork->PrepareRead(dwConnectionIndex);
			}
			continue;
		}
		if (pMsg->m_dwType == MSG_TYPE_TCP)
		{
			pCon = g_pNetwork->GetConnection(pMsg->m_dwConnectionIndex);
//			printf("\n Dll 디버그 정보 11 : CallBack : dwLen = %d\n", pMsg->m_dwSize);
			
			if (pCon)
				g_pNetwork->ReceivedMsg(pMsg->m_dwConnectionIndex,pMsg->m_pBuffer,pMsg->m_dwSize);
			
			continue;
		}
	}
	g_pMsgQue->m_pNext->ClearBuffer();
	MainThreadComplete();
}
