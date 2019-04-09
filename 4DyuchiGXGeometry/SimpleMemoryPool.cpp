#include "SimpleMemoryPool.h"

CSimpleMemoryPool::CSimpleMemoryPool()
{
	memset(this,0,sizeof(CSimpleMemoryPool));
}
BOOL CSimpleMemoryPool::Initialize(DWORD dwUnitSize,DWORD dwMaxBlockNum)
{
	m_dwUnitSize = dwUnitSize;
	m_dwMaxBlockNum = dwMaxBlockNum;
	m_pMemoryPool = new char[m_dwMaxBlockNum*dwUnitSize];
	
	return TRUE;
}
char* CSimpleMemoryPool::Alloc(DWORD dwNum)
{
	char* pBlock = NULL;
	
	if (!dwNum)
		goto lb_return;

	if (m_dwAllocBlockNum + dwNum >= m_dwMaxBlockNum)
		goto lb_return;

	pBlock = m_pMemoryPool + m_dwAllocBlockNum*m_dwUnitSize;;
	m_dwAllocBlockNum += dwNum;

lb_return:
	return pBlock;
}
void CSimpleMemoryPool::FreeAll()
{
	m_dwAllocBlockNum = 0;
}
void CSimpleMemoryPool::Release()
{
	if (m_pMemoryPool)
	{
		delete [] m_pMemoryPool;
		m_pMemoryPool = NULL;
	
	}
	m_dwMaxBlockNum = 0;
}

CSimpleMemoryPool::~CSimpleMemoryPool()
{
	Release();
}