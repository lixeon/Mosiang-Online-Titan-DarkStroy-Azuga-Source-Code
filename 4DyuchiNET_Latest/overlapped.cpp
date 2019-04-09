#include "overlapped.h"

COverlapped::COverlapped()
{
	memset(&m_Overlapped,0,sizeof(OVERLAPPED));
	m_dwIoType = 0;
	m_dwTransferredBytes = 0;
	m_socket = 0;
	m_iMaxTransferSize = 0;
	m_pNetwork = NULL;
	m_dwFlag = 0;
	m_dwHandle = 0;


}

COverlapped::~COverlapped()
{
}