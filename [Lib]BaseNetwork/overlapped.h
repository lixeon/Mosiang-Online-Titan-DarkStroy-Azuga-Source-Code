#pragma once

#include "stdafx.h"
#include "define.h"

class COverlapped
{
protected:
	OVERLAPPED			m_ovl;
	DWORD				m_dwTransferredBytes;
	DWORD				m_dwExistPacketSize;
	DWORD				m_dwEntryPosition;
	DWORD				m_dwMaxBufferSize;
	SOCKET				m_socket;
	IO_TYPE				m_dwIoType;
	char*				m_pBuffer;
public:
	void				SetIOType(IO_TYPE type) {m_dwIoType = type;}
	DWORD				GetIOType()				{return m_dwIoType;}
	DWORD				GetExistPacketSize()	{return m_dwExistPacketSize;}
	OVERLAPPED*			GetOverlapped()	{return &m_ovl;}
	SOCKET				GetSocket() {return m_socket;}
	void				SetSocket(SOCKET s) {m_socket = INVALID_SOCKET;}
	DWORD				ResetBuffer();
};