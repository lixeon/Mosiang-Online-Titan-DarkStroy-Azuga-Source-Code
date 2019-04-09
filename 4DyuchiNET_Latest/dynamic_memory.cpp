#include "dynamic_memory.h"


CDynamicMemoryContainer::CDynamicMemoryContainer()
{
	m_ppMemoryObjectTable = NULL;
	m_dwMaxMemoryObjectNum = 0;
	m_dwSizePerBucket = 0;
	m_dwNumPerBucket = 0;
	m_dwReservedMemoryObjectNum = 0;
	m_dwAllocatedMemoryObjectNum = 0;
	m_dwRealBytesPerBucket = 0;
	m_dwIncreaseNum = 1;
}


char* CDynamicMemoryContainer::Alloc()
{
	char* p;
		
	if (!m_dwReservedMemoryObjectNum)
	{
		if (!AddMemory(m_dwIncreaseNum))
			return FALSE;
	}
	p = m_ppMemoryObjectTable[m_dwReservedMemoryObjectNum-1];
	m_ppMemoryObjectTable[m_dwReservedMemoryObjectNum-1] = NULL;
	m_dwReservedMemoryObjectNum--;
	m_dwAllocatedMemoryObjectNum++;
	

	return p;
	
}
void CDynamicMemoryContainer::Free(char* pMemory)
{
	

		m_ppMemoryObjectTable[m_dwReservedMemoryObjectNum] = pMemory;
		m_dwAllocatedMemoryObjectNum--;
		m_dwReservedMemoryObjectNum++;

}

BOOL CDynamicMemoryContainer::Initialize(DWORD dwSize,DWORD dwDefaultNum,DWORD dwMaxNum,MEMORY_TYPE type)
{
	m_dwType = type;
	m_dwMaxMemoryObjectNum = dwMaxNum;


	switch (type)
	{
	case MEMORY_TYPE_CHAIN:
		{
			m_dwNumPerBucket = ((dwSize-1)>>6)+1;
			m_dwSizePerBucket = m_dwNumPerBucket*64;
			m_dwRealBytesPerBucket  = m_dwNumPerBucket*SIZE_MEMORY_OBJECT;
		}
		break;
	case MEMORY_TYPE_FIXED:
		{
			m_dwNumPerBucket = 1;
			m_dwSizePerBucket = dwSize;
			m_dwRealBytesPerBucket = dwSize;
		}
		break;


	}
	m_ppMemoryObjectTable = new char*[dwMaxNum];
	memset(m_ppMemoryObjectTable,0,sizeof(char*)*dwMaxNum);

	AddMemory(dwDefaultNum);
	return TRUE;
}

BOOL CDynamicMemoryContainer::AddMemory(DWORD dwNum)
{
	if (m_dwReservedMemoryObjectNum + dwNum >= m_dwMaxMemoryObjectNum)
		return FALSE;

	for (DWORD i=0; i<dwNum; i++)
	{
		m_ppMemoryObjectTable[m_dwReservedMemoryObjectNum] = new char[m_dwRealBytesPerBucket];
		m_dwReservedMemoryObjectNum++;
	}
	return TRUE;

}
BOOL CDynamicMemoryContainer::Release()
{
	return TRUE;
}

CDynamicMemoryContainer::~CDynamicMemoryContainer()
{


	if (m_ppMemoryObjectTable)
	{
		for (DWORD i=0; i<m_dwReservedMemoryObjectNum; i++)
		{
			if (m_ppMemoryObjectTable[i])
			{
				delete [] m_ppMemoryObjectTable[i];
				m_ppMemoryObjectTable[i] = NULL;

			}
		}

		delete [] m_ppMemoryObjectTable;
		m_ppMemoryObjectTable = NULL;
	}

}