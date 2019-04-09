#pragma once

#include "stdafx.h"

struct MSG_CONTAINER
{
	DWORD			m_dwType;
	DWORD			m_dwConnectionIndex;
	DWORD			m_dwSize;
	DWORD			m_dwFlag;
	char			m_pBuffer[1];
};

#define		MSG_HEADER_SIZE		16

struct MSG_BUFFER
{
	char*			m_pBuffer;
	DWORD			m_dwBufferPosition;
	
};

class CMsgQue
{
public:
	CMsgQue*			m_pNext;
private:
	char*				m_pBuffer;
	DWORD				m_dwBufferPosition;
	DWORD				m_dwDispatchPosition;
	DWORD				m_dwMaxBufferSize;
	DWORD				m_dwMsgNum;
	

public:
	DWORD				GetMsgNum()		{return m_dwMsgNum;}
	void				ClearBuffer()	{ m_dwBufferPosition = 0; m_dwDispatchPosition = 0; m_dwMsgNum = 0;}
	BOOL				Initialize(DWORD dwMaxBufferSize);
	BOOL				PushMsg(DWORD dwIndex,DWORD dwType,WSABUF* pBuf,DWORD dwNum,DWORD dwMsgSize);
	MSG_CONTAINER*		DispatchMsg();
	
	
	
	CMsgQue();
	~CMsgQue();

};

extern CMsgQue*		g_pMsgQue;

