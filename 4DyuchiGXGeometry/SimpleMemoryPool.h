#ifndef SIMPLE_MEMORY_POOL_H
#define SIMPLE_MEMORY_POOL_H

#include "../4DyuchiGRX_Common/stdafx.h"

class CSimpleMemoryPool
{
	char*						m_pMemoryPool;
	DWORD						m_dwMaxBlockNum;
	DWORD						m_dwAllocBlockNum;
	DWORD						m_dwUnitSize;
public:
	BOOL						Initialize(DWORD dwUnitSize,DWORD dwMaxBlockNum);
	char*						Alloc(DWORD dwNum);
	void						FreeAll();
	void						Release();
	CSimpleMemoryPool();
	~CSimpleMemoryPool();

};
#endif


