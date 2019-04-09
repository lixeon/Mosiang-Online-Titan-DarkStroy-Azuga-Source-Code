#include "quemanager.h"

CQueManager* g_pQueManager = NULL;

// constructor
CQueManager::CQueManager()
{
	// 변수 초기화
	m_pWritableQue = NULL;
	m_pReadableQue = NULL;
}
// 초기 메모리 할당
void CQueManager::Initialize(DWORD dwQueSize,HANDLE hMsgHandle)
{
	// wrtiable,readable큐를 생성한다.
	m_pReadableQue = new CMsgQue(dwQueSize);
	m_pWritableQue = new CMsgQue(dwQueSize);

	// 크리터컬 섹션 초기화.
	InitializeCriticalSectionAndSpinCount(&g_csWrite,1000);

}


// 종료할때 메모리를 해제한다.
void CQueManager::Release()
{
	// 두개의 큐를 모두 제거하고 동기화 객체들도 제거한다.
	if (m_pWritableQue)
	{
		delete m_pWritableQue;
		m_pWritableQue = NULL;
	}
	if (m_pReadableQue)
	{
		delete m_pReadableQue;
		m_pReadableQue = NULL;
	}
	DeleteCriticalSection(&g_csWrite);
}

// destructor
CQueManager::~CQueManager()
{
	Release();
}