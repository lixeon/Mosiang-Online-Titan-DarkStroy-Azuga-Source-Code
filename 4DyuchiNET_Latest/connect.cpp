#include "connect_que.h"
#include "connect.h"
#include "mainthread.h"
#include "pre_connect.h"

#define MAX_CONNECT_THREAD_EVENT_NUM	2
#define EVENT_INDEX_CONNECT_DESTROY		0
#define EVENT_INDEX_CONNECT				1

DWORD WINAPI ConnectThread(LPVOID lpVoid);

HANDLE	g_hConnectThreadHandle = NULL;
HANDLE	g_hConnectEvent[MAX_CONNECT_THREAD_EVENT_NUM];

CConnectQue* g_pConnectWriteQue = NULL;
CConnectQue* g_pConnectReadQue = NULL;
CRITICAL_SECTION	g_csConnect;
BOOL	g_bAlive = FALSE;

BOOL StartConnectThread(DWORD dwMaxConnectNumAtSameTime)
{
	g_pConnectWriteQue = new CConnectQue;
	g_pConnectWriteQue->Initialize(dwMaxConnectNumAtSameTime);
	g_pConnectReadQue = new CConnectQue;
	g_pConnectReadQue->Initialize(dwMaxConnectNumAtSameTime);

	for (DWORD i=0; i<MAX_CONNECT_THREAD_EVENT_NUM; i++)
		g_hConnectEvent[i] = CreateEvent(NULL,FALSE,FALSE,NULL);

	g_hConnectThreadHandle = CreateThread(NULL,NULL,ConnectThread,NULL,NULL,NULL);

	InitializeCriticalSection(&g_csConnect);

	g_bAlive = TRUE;
	return TRUE;
}

BOOL Connect(char* szIP,WORD wPort,CONNECTSUCCESSFUNC SuccessFunc,CONNECTFAILFUNC FailFunc,CNetwork* pNetwork,void* pExt)
{
	BOOL	bResult = FALSE;
	if (!g_bAlive)
		goto lb_return;


	CONNECT_ITEM	item;
	item.addr.sin_family = AF_INET;
	item.addr.sin_port   = htons(wPort);
	item.addr.sin_addr.s_addr = inet_addr(szIP);

//	char* paddr = inet_ntoa(item.addr.sin_addr);
//	WORD port = ntohs(item.addr.sin_port);
	

	item.pNetwork = pNetwork;
	item.FailConnect = FailFunc;
	item.SuccessConnect = SuccessFunc;
	item.pExt = pExt;
	
	
	
	item.socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP,NULL, 0, WSA_FLAG_OVERLAPPED);
	if(item.socket == INVALID_SOCKET )
	{
		MessageBox(GetActiveWindow(),"Fail to create socket","Error",MB_OK);
		goto lb_return;
	}
	
	EnterCriticalSection(&g_csConnect);

	bResult = g_pConnectWriteQue->PushConnectItem(&item);
	
	LeaveCriticalSection(&g_csConnect);
	
	
	SetEvent(g_hConnectEvent[EVENT_INDEX_CONNECT]);
	

lb_return:
	return bResult;
}

void EndConnectThread()
{
	g_bAlive = FALSE;
	SetEvent(g_hConnectEvent[EVENT_INDEX_CONNECT_DESTROY]);
	WaitForSingleObject(g_hConnectThreadHandle,INFINITE);

	for (DWORD i=0; i<MAX_CONNECT_THREAD_EVENT_NUM; i++)
	{
		CloseHandle(g_hConnectEvent[i]);
	}
	if (g_pConnectWriteQue)
	{
		delete g_pConnectWriteQue;
		g_pConnectWriteQue = NULL;
	}
	if (g_pConnectReadQue)
	{
		delete g_pConnectReadQue;
		g_pConnectReadQue = NULL;
	}
	DeleteCriticalSection(&g_csConnect);
}
DWORD WINAPI ConnectThread(LPVOID lpVoid)
{

	DWORD dwIndex;
	CConnectQue* pTemp;
	CONNECT_ITEM*	pItem;
	while(1)
	{
		dwIndex = WaitForMultipleObjects(2,g_hConnectEvent,FALSE,INFINITE);
		if (EVENT_INDEX_CONNECT == dwIndex)		
		{
			EnterCriticalSection(&g_csConnect);

			if (g_pConnectWriteQue->GetConnectItemNum() > 0)
			{	
				pTemp = g_pConnectWriteQue;
				g_pConnectWriteQue = g_pConnectReadQue;
				g_pConnectReadQue = pTemp;
			}

			// 0이라면 어떻게 되나???

			
			LeaveCriticalSection(&g_csConnect);

			// actually connect
			//
			
			for (DWORD i=0; i<g_pConnectReadQue->GetConnectItemNum(); i++)
			{
				pItem = g_pConnectReadQue->GetConnectItem(i);

				if (SOCKET_ERROR == connect(pItem->socket,(SOCKADDR*)&pItem->addr,sizeof(SOCKADDR)))
				{
					pItem->socket = INVALID_SOCKET;
				}
				g_pPreConnectPool->InsertPreConnect(pItem);
				ProcessPreConnectedItem();
			}
			 g_pConnectReadQue->Clear();
				
		}
		else if (EVENT_INDEX_CONNECT_DESTROY == dwIndex)
		{
			goto lb_exit;
		}
	}
lb_exit:
	return 0;
}
