#pragma once

#include "memory_object.h"

enum MEMORY_TYPE
{
	MEMORY_TYPE_FIXED			= 0x00000000,
	MEMORY_TYPE_CHAIN			= 0x00000001
};

class CDynamicMemoryContainer
{
	char**						m_ppMemoryObjectTable;
	DWORD						m_dwReservedMemoryObjectNum;
	DWORD						m_dwAllocatedMemoryObjectNum;
	DWORD						m_dwMaxMemoryObjectNum;
	DWORD						m_dwIncreaseNum;
	DWORD						m_dwNumPerBucket;
	DWORD						m_dwSizePerBucket;
	DWORD						m_dwRealBytesPerBucket;
	MEMORY_TYPE					m_dwType;
	
	
	BOOL						AddMemory(DWORD dwNum);
	
public:
	DWORD						GetReservedMemoryObjectNum() {return m_dwReservedMemoryObjectNum;}
	char*						GetMemoryObject(DWORD no) {return m_ppMemoryObjectTable[no];}
	DWORD						GetRealBytesPerBucket() {return m_dwRealBytesPerBucket;}
	DWORD						GetMaxAvalibleBytesPerBucket() {return m_dwSizePerBucket;}
	DWORD						GetMemoryNumPerBucket() {return m_dwNumPerBucket;}
	DWORD						GetMaxMemoryObjectNum() {return m_dwMaxMemoryObjectNum;}
	
	char*						Alloc();
	void						Free(char* pMemory);
	BOOL						Initialize(DWORD dwSize,DWORD dwDefaultNum,DWORD dwMaxNum,MEMORY_TYPE type);
	BOOL						Release();
							
	CDynamicMemoryContainer();
	~CDynamicMemoryContainer();
};
