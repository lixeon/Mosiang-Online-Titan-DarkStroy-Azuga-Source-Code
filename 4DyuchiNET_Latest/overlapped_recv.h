#pragma once

#include "overlapped.h"

class COverlappedRecv : public COverlapped
{
	CReadMemoryHandle	m_mhRead;
public:
	WSABUF				m_wsabuf;
	void				Initialize();
	char*				GetEntryBuffer()	{return m_mhRead.GetEntryPtr();}
	DWORD				GetMaxBufferSize()	{return m_mhRead.GetMaxMemorySize();}
	BOOL				AllocBuffer(DWORD size);						// send,recv버퍼 사이즈를 세팅.
	void				ResetBuffer()		{m_mhRead.ResetMemory();}
	void				OnRead(DWORD dwSize,WSABUF* pBuf,DWORD dwBufNum,DWORD* pdwTotalMsgBytes,DWORD* pdwMsgNum)
	{
		m_mhRead.OnRead(dwSize,pBuf,dwBufNum,pdwTotalMsgBytes,pdwMsgNum);

	}
	void				Release();
	BOOL				GetMsg(char** ppDest,DWORD* pdwLength);
	BOOL				PrepareRead();
	COverlappedRecv();
	~COverlappedRecv();
};
