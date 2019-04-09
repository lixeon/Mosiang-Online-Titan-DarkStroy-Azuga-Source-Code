// MemoryPool.h: interface for the CMemoryPool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMORYPOOL_H__7B08B354_62A8_4DB9_8DA8_403AEC56522E__INCLUDED_)
#define AFX_MEMORYPOOL_H__7B08B354_62A8_4DB9_8DA8_403AEC56522E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMemoryPool  
{
private:
	char**		m_ppMemoryArray;
	char**		m_ppMemoryArrayFordelete;
	char*		m_pMemory;
	DWORD		m_dwFreeMemoryIndex;
	DWORD		m_dwMaxObjectNum;
	DWORD		m_dwMaxObjectSize;
	CRITICAL_SECTION	m_csMemory;

public:
	void					Free(char* pTarget);
	void					Init();
	char*					Alloc();
	DWORD					GetFreeIndexNum();
	CMemoryPool(DWORD dwNum, DWORD dwMaxSize);
	virtual ~CMemoryPool();

};

#endif // !defined(AFX_MEMORYPOOL_H__7B08B354_62A8_4DB9_8DA8_403AEC56522E__INCLUDED_)
