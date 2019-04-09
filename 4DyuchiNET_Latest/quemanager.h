// 멀티스레드의 잇점을 살리기 위해서는 소켓 io를 처리하는 동안에도 수집한 메시지를 처리하고 있어야 한다.
// 따라서 writable과 readable큐를 만들어서 worker스레드는 writable큐에만 접근하고 readable큐는 
// 메인 스레드만 접근하도록 한다.메인 스레드가 readable큐의 메시지를 다 처리하면 큐를 바꾸도록 지시한다.
// 명령 체계는 메인 스레드에 우선권이 있다.어찌했건 메인 스레드가 메시지를 처리하고 있는 동안 큐는 바뀔수 없으며
// 그 동안 (적어도writable큐가 다 찰때까지는) worker스레드는 메시지를 수집해서 writable큐에다 쌓아놓게 된다.
// 이 소스는 이 두개의 큐를 관리하는 큐 매니져 클래스를 정의한 것이다.

#ifndef QUE_MANAGER_H
#define QUE_MANAGER_H

#include "que.h"

class CQueManager
{
public:
	CMsgQue*			m_pWritableQue;		// 쓰기가능 메시지 큐의 포인터.정확히는 worker스레드에 의해서 쓰기만 행해진다.
	CMsgQue*			m_pReadableQue;		// 읽기가능 메시지 큐의 포인터.정확히는 메인 스레드에 의해서 읽기만 행해진다.
public:
	CMsgQue*			GetWritableQue() {return m_pWritableQue;}	// write가능한 큐의포인터를 얻는다.
	CMsgQue*			GetReadableQue() {return m_pReadableQue;}	// read가능한 큐의 포인터를 얻는다.
	
	void				Initialize(DWORD dwQueSize,HANDLE hMsgEvent);	// 초기화.
	void				Release();		// 종료할때 메모리 릴리즈.
	CQueManager();
	~CQueManager();
};

extern CQueManager* g_pQueManager;

#endif

