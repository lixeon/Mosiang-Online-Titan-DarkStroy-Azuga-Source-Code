#include "overlapped_send.h"


COverlappedSend::COverlappedSend()
{
	memset(this,0,sizeof(COverlappedSend));
}
void COverlappedSend::Initialize(DWORD dwIndex,SOCKET s,HANDLE hEvent,DWORD dwSize)
{
	m_ovl.hEvent = hEvent;
	m_socket = s;
	m_dwMaxBufferSize = dwSize;
	m_pBuffer = new char[dwSize];
	m_dwIoType = IO_TYPE_WAIT;

	InitializeCriticalSection(&m_csSend);
}
BOOL COverlappedSend::Push(char* pMsg,DWORD dwLen)
{
	BOOL		bResult = FALSE;
	DWORD		dwTail;
	char*		p;
	
	dwTail = m_dwExistPacketSize + m_dwEntryPosition;
	// °¡¿ë ¹öÆÛ »çÀÌÁî°¡ º¸³»°íÀÚ ÇÏ´Â »çÀÌÁîº¸´Ù ÀûÀ» °æ¿ì ¿£Æ®¸® Æ÷Áö¼Ç ÀçÁ¶Á¤.
	// ±×·¯°íµµ dwLenº¸´Ù ºÎÁ·ÇÏ¸é false¸®ÅÏ
	if (dwTail + dwLen + 2 > m_dwMaxBufferSize)
	{
		if (ResetBuffer() < dwLen + 2)
			goto lb_return;
	}
	p = m_pBuffer + m_dwEntryPosition + m_dwExistPacketSize;
	*(WORD*)p = (WORD)dwLen;
	memcpy(p + 2,pMsg,dwLen);

	m_dwExistPacketSize += (dwLen + 2);
	bResult = TRUE;

lb_return:

//	if( !bResult)
//		printf("Push Operation Error\n");
	return bResult;
}
BOOL COverlappedSend::SendBuffer()
{	
	BOOL		bResult = TRUE;
	WSABUF		wsabuf;
	wsabuf.buf = m_pBuffer + m_dwEntryPosition;
	wsabuf.len = m_dwExistPacketSize;
	m_dwIoType = IO_TYPE_WRITE;

	int result = WSASend(m_socket,&wsabuf,1,&m_dwTransferredBytes,NULL,&m_ovl,NULL);
	if (result != SOCKET_ERROR)
		goto lb_return;

	result = WSAGetLastError();
	if (result == ERROR_IO_PENDING)
		goto lb_return;

//	printf("Socket Error : %d\n", result);

	bResult = FALSE;

lb_return:
	return bResult;

}
void COverlappedSend::OnSend(DWORD dwTransferredBytes)
{
	m_dwEntryPosition += dwTransferredBytes;
	m_dwExistPacketSize -= dwTransferredBytes;
	m_dwIoType = IO_TYPE_WAIT;

	if (m_dwExistPacketSize)
		SendBuffer();
}

COverlappedSend::~COverlappedSend()
{
	DeleteCriticalSection(&m_csSend);
	if (m_pBuffer)
	{
		delete [] m_pBuffer;
		m_pBuffer = NULL;
	}
	closesocket(m_socket);
	m_socket = INVALID_SOCKET;

}
