// 이 서버 프로그램은 크게 두 줄기로 볼 수 있다.소켓 io를 처리하는 worker스레드 집단과 
// 수신된 메시지를 처리하고 능동적으로 서버가 해야할 일을 관장하는 메인 스레드이다.
// 이것은 메인 스레드와 이에 관련된 글로벌 함수의 소스이다.

#include "../4DyuchiNET_Common/net_define.h"
#include "cpio.h"
#include "mainthread.h"
#include "que.h"
#include "network.h"
#include "quemanager.h"
#include "switch_que.h"
#include "net_global.h"
#include "log.h"
#include <process.h>
#include "pre_connect.h"

//#include "typedef.h"

OnIntialFunc 					g_pOnInitialFunc = NULL;			// 메인 스레드가 생성되자마자 실행할 함수
HANDLE	g_hMainThreadEvent[MAX_MAIN_THREAD_EVENT_NUM];				// 메인 스레드가 사용할 이벤트 객체 핸들 배열
HANDLE	g_hMainThread			=	NULL;							// 메인 쓰레드 핸들.
DWORD	g_dwMainThreadEventNum	=	MIM_MAIN_THREAD_EVENT_NUM;		// 메인 스레드가 체크할 이벤트의 갯수.
HANDLE	g_hCheckBreakEvent		=	NULL;

//typedef void (*VOIDFUNC)(void);
EVENTCALLBACK pEventFunc[MAX_MAIN_THREAD_USER_DEFINE_EVENT_NUM];	// 유져 정의 이벤트에 작동될 함수 포인터 배열.
																	// Modified by chan78 at 2001/10/17

struct EVENT_DESC
{
	DWORD		dwEventIndex;
	HANDLE		hEvent;
};

// 2007/12/19 by yuchi
// 유져 이벤트의 인덱스와 이벤트 핸들의 맵핑 테이블
DWORD		g_dwUserEventNum = 0;
EVENT_DESC g_userEventDescList[MAX_MAIN_THREAD_USER_DEFINE_EVENT_NUM];

UINT WINAPI ProcessMsgThread(LPVOID lpVoid);	

void SetOnInitialFunc(OnIntialFunc pFunc)
{
	g_pOnInitialFunc = pFunc;
}

// 사용자 정의 이벤트를 만들었을때 이벤트 발생시 수행할 함수를 설정해준다.
// 사용자 정의 이벤트를 만들땐 반드시 세팅해줘야 한다.
// Modified by chan78 at 2001/10/17
void SetMainThreadUserDefineEventFunc(DWORD index,EVENTCALLBACK pFunc)
{
	pEventFunc[index] = pFunc;
}

// 메인 스레드를 잠시 멈춰야 할때 사용한다.
// npc를 넣을때라던가...기타 등등
void BreakMainThread()
{
	SetEvent(g_hMainThreadEvent[EVENT_INDEX_BREAK]);
	WaitForSingleObject(g_hCheckBreakEvent,INFINITE);
}

// 멈췄던 메인 스레드를 다시 돌릴때 사용한다.
void ResumeMainThread()
{
	SetEvent(g_hMainThreadEvent[EVENT_INDEX_RESUME]);
}
// 메인 스레드가 메시지를 처리해야함을 worker스레드가 알릴때 set할 이벤트 객체의 핸들을 리턴.
HANDLE	GetMainThreadMsgEvent()
{
	return g_hMainThreadEvent[EVENT_INDEX_MSG_EVENT];
}
// 사용자 정의 이벤트의 핸들을 리턴.


HANDLE	GetMainThreadUserDefineEvent(DWORD index)
{
	// 2007/12/19 함수 내용 수정 by yuchi
	//return g_hMainThreadEvent[index+EVENT_INDEX_USER_DEFINE_0];

	HANDLE		hEvent = NULL;

	for (DWORD i=0; i<g_dwUserEventNum; i++)
	{
		
		if (g_userEventDescList[i].dwEventIndex == index)
		{
			hEvent = g_userEventDescList[i].hEvent;
			break;
		}
	}
	return hEvent;
}
void ProcessPreConnectedItem()
{
	SetEvent(g_hMainThreadEvent[EVENT_INDEX_PRE_CONNECT]);

}
// 메인 스레드 시작.
// Modified by chan78 at 2002/07/22
BOOL StartMainThread(DWORD dwFlag)
{
	UINT ThreadID = 0;
	
	// 사용자 정의 이벤트를 제외한 기본적인 이벤트 객체 생성.
	g_hMainThreadEvent[EVENT_INDEX_MSG_EVENT]			= CreateEvent(NULL,FALSE,FALSE,NULL);
	g_hMainThreadEvent[EVENT_INDEX_DESTROY]			= CreateEvent(NULL,FALSE,FALSE,NULL);
	g_hMainThreadEvent[EVENT_INDEX_BREAK]				= CreateEvent(NULL,FALSE,FALSE,NULL);
	g_hMainThreadEvent[EVENT_INDEX_RESUME]			= CreateEvent(NULL,FALSE,FALSE,NULL);
	g_hMainThreadEvent[EVENT_INDEX_PRE_CONNECT]		= CreateEvent(NULL,FALSE,FALSE,NULL);
	g_hCheckBreakEvent								= CreateEvent(NULL,FALSE,FALSE,NULL);

	// 스레드 생성.스타트 
	g_hMainThread = (HANDLE)_beginthreadex(NULL,NULL,ProcessMsgThread,NULL,NULL,&ThreadID);
	

	if (0xffffffff == (DWORD)g_hMainThread)
	{
		MessageBox(NULL,"Fail to Create MainThread","Error",MB_OK);
		return FALSE;
	}

	return FALSE;

}
// 사용자 정의 이벤트를 사용하기 전에 그 갯수를 미리 설정해줘야 한다.
void SetMainThreadUserDefineEventNum(DWORD num)
{
	// 사용할 사용자 정의 이벤트 갯수만큼 이벤트 객체를 만든다.

	g_dwMainThreadEventNum = MIM_MAIN_THREAD_EVENT_NUM + num;
	memset(pEventFunc,0,sizeof(VOIDFUNC)*MAX_MAIN_THREAD_USER_DEFINE_EVENT_NUM);

	memset(g_userEventDescList,0,sizeof(g_userEventDescList));

	g_dwUserEventNum = num;

	for (DWORD i=0; i<num; i++)
	{
		g_hMainThreadEvent[EVENT_INDEX_USER_DEFINE_0+i]	= CreateEvent(NULL,FALSE,FALSE,NULL);

		// 2007/12/19 yuchi
		g_userEventDescList[i].dwEventIndex = i;
		g_userEventDescList[i].hEvent = g_hMainThreadEvent[EVENT_INDEX_USER_DEFINE_0+i];

	}	
	
}

// 메인 스레드를 종료한다.
void EndMainThread()
{
	ResumeMainThread();

	// 메인 스레드가 종료할 수 있도록 이벤트를 세팅해주고 완전히 종료할때까지 기다린다.
	SetEvent(g_hMainThreadEvent[EVENT_INDEX_DESTROY]);
	WaitForSingleObject(g_hMainThread,INFINITE);

	// 이벤트 객체들의 핸들을 닫는다.
	for (DWORD i=0; i<g_dwMainThreadEventNum; i++)
	{
		CloseHandle(g_hMainThreadEvent[i]);
	}
	CloseHandle(g_hMainThread);
	CloseHandle(g_hCheckBreakEvent);
}


//
// 이것이 바로 메인 스레드의 실체이다.
//
UINT WINAPI ProcessMsgThread(LPVOID lpVoid)
{	


	DWORD dwEventIndex	= 0;		// 이벤트가 어떤 종류인지 판별할 수 있는 인덱스를 받아올 변수.
	CMsgQue* pQ			= NULL;		// readable 큐의 포인터를 받아올 포인터 변수.
	MSG_CONTAINER*	pmc = NULL;		// 메시지를 긁어올 메시지 컨테이너의 포인터 변수.
	CConnection*	con = NULL;		// 어느 컨넥션에서 날아왔는지를 받아올 CConnection 포인터 변수.


	if (g_pOnInitialFunc)
		g_pOnInitialFunc(NULL);

	while (TRUE)
	{
		if ((dwEventIndex = 
			WaitForMultipleObjects(
			g_dwMainThreadEventNum,
			g_hMainThreadEvent,
			FALSE,
			INFINITE)
			) == WAIT_FAILED)
		{
			// 여기서 에러가 발생했다면 이벤트 객체가 만들어지지 않았거나 만들어진 이벤트 객체들 갯수보다 요청한 갯수가 많은것이다.
			MessageBox(GetActiveWindow(),"Fail WaitForMultipleObject","Error",MB_OK);
		}

		if (dwEventIndex == EVENT_INDEX_DESTROY)	// 스레드 종료
			goto seg_destroy;							// 종료코드로 점프
		

		if (dwEventIndex == EVENT_INDEX_BREAK)		// 메인 스레드에 브레이크를 걸었다.
		{
			// 리줌할때까지 대기.
			SetEvent(g_hCheckBreakEvent);
			WaitForSingleObject(g_hMainThreadEvent[EVENT_INDEX_RESUME],INFINITE);
			continue;
		}
		if (dwEventIndex == EVENT_INDEX_PRE_CONNECT)
		{
			g_pPreConnectPool->ProcessPreConnect();
			continue;

		}

		if (dwEventIndex >= EVENT_INDEX_USER_DEFINE_0)
		{
			// 사용자 정의 이벤트가 발생했다.사용자 정의 이벤트에 해당하는 함수를 수행한다.
			// Modified by chan78 at 2002/05/09

			// 2007/12/19 by yuchi
			DWORD		dwUserEventIndex = -1;
			
			for (DWORD i=0; i<g_dwUserEventNum; i++)
			{
				
				if (g_userEventDescList[i].hEvent == g_hMainThreadEvent[dwEventIndex])
				{
					
					
					dwUserEventIndex = g_userEventDescList[i].dwEventIndex;
					
					pEventFunc[dwUserEventIndex](dwUserEventIndex);

					
					// 기아현상을 방지하기 위해서  이번에 세트된 이벤트를 가장 뒤로 위치시킨다.
					DWORD		dwNum = g_dwMainThreadEventNum - dwEventIndex - 1;
					// 이번에 세트된 이벤트가 맨 뒤에 위치하는 놈이라면 바꿀 필요없다.
					if (dwNum)
					{
						
						HANDLE	hCurEvent = g_hMainThreadEvent[dwEventIndex];
						memcpy(g_hMainThreadEvent+dwEventIndex,g_hMainThreadEvent+dwEventIndex+1,sizeof(HANDLE)*dwNum);
						g_hMainThreadEvent[g_dwMainThreadEventNum-1] = hCurEvent;
					}
					
					break;
				}

			}
			//pEventFunc[(dwEventIndex-EVENT_INDEX_USER_DEFINE_0)]((dwEventIndex-EVENT_INDEX_USER_DEFINE_0));
			continue;
		}
	
		// 오직 스위치 완료 이벤트에 따라서만 메시지를 처리한다.
		pQ = g_pQueManager->GetReadableQue();	// 먼저 readable queue의 포인터를 얻어온다.
				
		// 메시지 큐에 들어있는 만큼 한꺼번에 다 처리한다.
		for (DWORD i=0; i<pQ->GetMsgNum(); i++)
		{
			// 메시지 컨테이너를 통해 메시지 종류를 판별하고 그에 맞는 처리를 한다.
			pmc = GetMsgContainer(pQ);	
			if (pmc->GetActionType() == ACTION_TYPE_DISCONNECT)
			{
				// disconnect이벤트이다.메시지 형태로 날아왔지만 이건 어떤 소켓 접속이 끊긴것이다.
				if (pmc->GetActorType() == ACTOR_TYPE_USER)
				{
					// 접속 끊긴게 유져측이라면 유쳐측 네트워크 클래스에서 해당 컨넥션을 제거한다.
					// 제거하기 전에 접속이 끊길때 수행될 함수는 수행해주자.
					g_pNetworkForUser->GetConnection(&con,pmc->GetConnectionIndex());
					if (con)
					{
						g_pNetworkForUser->OnDisconnect(pmc->GetConnectionIndex());
						g_pNetworkForUser->CloseConnection(pmc->GetConnectionIndex());
					}
				}
				else if (pmc->GetActorType() == ACTOR_TYPE_SERVER)
				{
					// 접속 끊긴게 서버측이라면 서버측 네트워크 클래스에서 해당 컨넥션을 제거한다.
					// 제거하기 전에 접속이 끊길때 수행될 함수는 수행해주자.
					g_pNetworkForServer->GetConnection(&con,pmc->GetConnectionIndex());
					if (con)
					{
						g_pNetworkForServer->OnDisconnect(pmc->GetConnectionIndex());
						g_pNetworkForServer->CloseConnection(pmc->GetConnectionIndex());
					}
				}
			}
			else if (pmc->GetActionType() == ACTION_TYPE_TCP)
			{
				// TCP패킷을 수신한 경우이다.
				if (pmc->GetActorType() == ACTOR_TYPE_USER)
				{
					// 유져측으로부터 TCP패킷을 받았다면...어느 connection에서 보냈는지 찾아서 
					// tcp패킷을 처리하는 함수에 넘긴다.
					g_pNetworkForUser->GetConnection(&con,pmc->GetConnectionIndex());
					if (con)
						g_pNetworkForUser->OnRecvTCP(pmc->GetConnectionIndex(),pmc->GetMsg(),pmc->GetMsgLength());
				}
				else if (pmc->GetActorType() == ACTOR_TYPE_SERVER)
				{
					// 서버측으로부터 TCP패킷을 받았다면 역시 어느 서버connection인지 찾아서 
					// tcp패킷을 처리하는 함수에 넘긴다.
					g_pNetworkForServer->GetConnection(&con,pmc->GetConnectionIndex());
					if (con)
						g_pNetworkForServer->OnRecvTCP(pmc->GetConnectionIndex(),pmc->GetMsg(),pmc->GetMsgLength());
				}
			}
		}
		pQ->Clear();
		SetEvent(g_hProcessComplete);
	}
seg_destroy:
	SetEvent(g_hProcessComplete);
	_endthreadex(0);
	return 0;
}
