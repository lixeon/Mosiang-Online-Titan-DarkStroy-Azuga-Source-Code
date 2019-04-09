#include "VariableBuffer.h"

CVariableBuffer::CVariableBuffer()
{
	memset(this,0,sizeof(CVariableBuffer));
}
BOOL CVariableBuffer::Initialize(DWORD dwMaxBufferSize)
{
	m_dwMaxBufferSize = dwMaxBufferSize;
	
	m_pBuffer = new char[dwMaxBufferSize];
	memset(m_pBuffer,0,dwMaxBufferSize);

	return TRUE;
}
char* CVariableBuffer::AllocBuffer(DWORD dwSize)
{
	char* pItemPtr = NULL;

	if (dwSize + m_dwCurrentBufferSize >= m_dwMaxBufferSize)
		goto lb_return;

	pItemPtr = m_pBuffer + m_dwCurrentBufferSize;
	m_dwCurrentBufferSize += dwSize;

lb_return:
	return pItemPtr;
}
CVariableBuffer::~CVariableBuffer()
{
	if (m_pBuffer)
	{
		delete [] m_pBuffer;
		m_pBuffer = NULL;
	}
}
