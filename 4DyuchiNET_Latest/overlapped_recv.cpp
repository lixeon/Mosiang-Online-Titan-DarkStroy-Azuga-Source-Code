#include "overlapped_recv.h"
#include "network.h"
#include "cpio.h"

COverlappedRecv::COverlappedRecv()
{
	Initialize();
}
void COverlappedRecv::Initialize()
{
	// 컨스트럭터.변수 초기화.
	// 변수를 초기화한다.
	m_dwFlag = 0;
	memset(&m_Overlapped,0,sizeof(m_Overlapped));
	m_socket = INVALID_SOCKET;
	m_dwIoType = IO_TYPE_READ;
	m_pNetwork = NULL;
	m_dwHandle = 0;
}
BOOL COverlappedRecv::AllocBuffer(DWORD size)
{
		
	char* pMemory;

	if (GetMaxBufferSize())
	{
		m_mhRead.InitializeMemory();
		return TRUE;
	}
	
	pMemory = m_pNetwork->GetDMRead()->Alloc();
	
	if (!pMemory)
		return FALSE;
	
	m_mhRead.SetMemory(pMemory,m_pNetwork->GetDMRead()->GetMaxAvalibleBytesPerBucket());
			
	return TRUE;
}

BOOL COverlappedRecv::PrepareRead()
{
//	memset(&m_Overlapped,0,sizeof(m_Overlapped));
	
	m_wsabuf.len = GetMaxTransferSize();
	m_wsabuf.buf = m_mhRead.GetEntryPtr();

	if (m_wsabuf.len > m_mhRead.GetReservedBytes())
		m_wsabuf.len = m_mhRead.GetReservedBytes();
			

	int result;
	result = WSARecv(m_socket,&m_wsabuf,1,&m_dwTransferredBytes,&m_dwFlag,&m_Overlapped,NULL);
			
	if (result != SOCKET_ERROR)
		return TRUE;
			
	result = WSAGetLastError();
	if (result == ERROR_IO_PENDING)
		return TRUE;

	PostDisconnectEvent(m_dwHandle);

	return FALSE;
}
void COverlappedRecv::Release()
{
	
	m_pNetwork->GetDMRead()->Free(m_mhRead.GetMemory());

}
COverlappedRecv::~COverlappedRecv()
{
	Release();

}
