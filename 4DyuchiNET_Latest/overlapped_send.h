#pragma once

#include "overlapped.h"

class COverlappedSend : public COverlapped
{

public:
	CWriteMemoryHandle	m_mhWrite;		// OFFSET 48
	CRITICAL_SECTION	m_csSend;		

	void				Initialize();
	DWORD				GetReservedBytes() {return m_mhWrite.GetReservedBytes();}
	DWORD				GetUsageBytes()	{return m_mhWrite.GetUsageBytes();}	
	DWORD				GetMaxBufferSize()	{return m_mhWrite.GetMaxMemorySize();}
	void				LockSend() {EnterCriticalSection(&m_csSend);}		// send할때 lock
	void				UnLockSend() {LeaveCriticalSection(&m_csSend);}	// send하고 나서 unlock
	BOOL				AllocBuffer(DWORD size);						// send,recv버퍼 사이즈를 세팅.	
	BOOL				PushMsg(char* msg,DWORD dwLength)	{return m_mhWrite.Push(msg,dwLength);}	// 메시지를 전송하기 위해 버퍼에 넣음.
	void				OnWrite(DWORD dwLength)	{m_mhWrite.OnWrite(dwLength);}		// 한바이트라도 전송되면 자동 호출되는 함수
	BOOL				SendBuffer();
	void				Release();		
	COverlappedSend();
	~COverlappedSend();
};

