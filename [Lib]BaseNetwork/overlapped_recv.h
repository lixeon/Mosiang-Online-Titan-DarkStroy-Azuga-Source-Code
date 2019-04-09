#pragma once

#include "overlapped.h"

class COverlappedRecv : public COverlapped
{
	DWORD			m_dwRecvUnitSize;
public:
	void			SetRecvUnitSize(DWORD dwSize) { m_dwRecvUnitSize = dwSize;}
	void			Initialize(DWORD dwIndex,SOCKET s,HANDLE hEvent,DWORD dwSize);
	BOOL			PrepareRead();
	DWORD			OnRead(WSABUF* pBuf,DWORD dwMaxBufNum,DWORD dwBytesTransferred,DWORD* pdwSize);


	COverlappedRecv();
	~COverlappedRecv();


};
