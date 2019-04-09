#pragma once

#include "stdafx.h"
#include "define.h"
#include "overlapped_send.h"
#include "overlapped_recv.h"


class CConnection 
{
	DWORD				m_dwIndex;
	COverlappedSend		m_ovlSend;
	COverlappedRecv		m_ovlRecv;
public:
	OVERLAPPED*			GetOverlappedRecv() {return m_ovlRecv.GetOverlapped();}
	OVERLAPPED*			GetOverlappedSend() {return m_ovlSend.GetOverlapped();}
	void				SetSocket(SOCKET s) {m_ovlRecv.SetSocket(s);m_ovlSend.SetSocket(s);}
	void				CloseSocket();
	SOCKET				GetSocket() {return m_ovlRecv.GetSocket();}
	sockaddr_in*		GetAddress();
	DWORD				GetIndex()	{return m_dwIndex;}
	BOOL				Send(char* msg,DWORD length);
	BOOL				Initialize(DWORD dwIndex,SOCKET s,HANDLE hEventSend,HANDLE hEventRecv,DWORD size);
	BOOL				PrepareRead() {return m_ovlRecv.PrepareRead();}
	CConnection();
	~CConnection();
	
};




