#include "overlapped_recv.h"

COverlappedRecv::COverlappedRecv()
{
	memset(this,0,sizeof(COverlappedRecv));
}
void COverlappedRecv::Initialize(DWORD dwIndex,SOCKET s,HANDLE hEvent,DWORD dwSize)
{
	m_ovl.hEvent = hEvent;
	m_socket = s;
	m_dwIoType = IO_TYPE_READ;
	m_pBuffer = new char[dwSize];
	m_dwMaxBufferSize = dwSize;
	
	if (m_dwMaxBufferSize >= DEFAULT_RECV_SIZE)
		m_dwRecvUnitSize = DEFAULT_RECV_SIZE;
}
BOOL COverlappedRecv::PrepareRead()
{
	BOOL		bResult = TRUE;
	WSABUF		wsabuf;
//	int			result;
	DWORD		dwFlag = 0;
	DWORD		dwTail  = m_dwEntryPosition + m_dwExistPacketSize;
	
	if (m_dwMaxBufferSize - dwTail <= m_dwRecvUnitSize)
		ResetBuffer();

	wsabuf.buf = m_pBuffer + m_dwEntryPosition + m_dwExistPacketSize;
	wsabuf.len = m_dwMaxBufferSize - m_dwExistPacketSize - m_dwEntryPosition;

	WSARecv(m_socket,&wsabuf,1,&m_dwTransferredBytes,&dwFlag,&m_ovl,NULL);

	return TRUE;
}
DWORD COverlappedRecv::OnRead(WSABUF* pBuf,DWORD dwMaxBufNum,DWORD dwBytesTransferred,DWORD* pdwSize)
{
	DWORD		dwCount = 0;
	DWORD		dwLen;
	DWORD		dwOffset;
	
	*pdwSize = 0;

	m_dwExistPacketSize += dwBytesTransferred;

	char		*p,*pStart,*pLimit;
	p = pStart = m_pBuffer+m_dwEntryPosition;
	pLimit = pStart + m_dwExistPacketSize;
	
//	printf("\n Dll 디버그 정보 3\nm_dwEntryPosition = %d\nm_dwExistPacketSize = %d\n", m_dwEntryPosition, m_dwExistPacketSize);



lb_loop:

	if (p+2+1 > pLimit)
	{
//		printf("\n Dll 디버그 정보 4 :  p+2+1 > pLimit \n");

		goto lb_return;
	}

	dwLen = (DWORD)(*(WORD*)p);
	
//	printf("\n Dll 디버그 정보 5 :  dwLen of p = %d\n", dwLen);


	if (p+2+dwLen > pLimit)
		goto lb_return;

	p += 2;

	(*pdwSize) += dwLen;
	pBuf[dwCount].len = dwLen;
	pBuf[dwCount].buf = p;
	dwCount++;
	p += dwLen;

	if (dwCount >= dwMaxBufNum)
		goto lb_return;

//	printf("\n Dll 디버그 정보 6 :  dwCount = %d\n", dwCount);

	goto lb_loop;
	
lb_return:
	dwOffset = (DWORD)(p - pStart);
	m_dwExistPacketSize -= dwOffset;
	m_dwEntryPosition += dwOffset;


//	printf("\n Dll 디버그 정보 7 :  lb_return\n");

	return dwCount;
}

COverlappedRecv::~COverlappedRecv()
{
	
	if (m_pBuffer)
	{
		delete [] m_pBuffer;
		m_pBuffer = NULL;
	}
}