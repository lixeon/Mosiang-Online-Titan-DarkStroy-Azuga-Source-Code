// MemoryPool.h: interface for the CMemoryPool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMORYPOOL_H__C8AD2342_A4E4_4A08_8969_F9F4B1CE7E5E__INCLUDED_)
#define AFX_MEMORYPOOL_H__C8AD2342_A4E4_4A08_8969_F9F4B1CE7E5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <process.h>

template<class DATATYPE>
class CMemoryPool  
{
private:
	DATATYPE**		m_ppMemoryArray;
	DATATYPE**		m_ppMemoryArrayFordelete;
	DATATYPE*		m_pMemory;
	DWORD		m_dwFreeMemoryIndex;
	DWORD		m_dwMaxObjectNum;
//	CRITICAL_SECTION	m_csMemory;
	
public:
	void					Free(DATATYPE* pTarget)
	{
		if (!pTarget) return;
//		EnterCriticalSection(&m_csMemory);
		m_ppMemoryArray[m_dwFreeMemoryIndex] = pTarget;
		m_dwFreeMemoryIndex++;
//		LeaveCriticalSection(&m_csMemory);
	}
	void					Init(DWORD dwNum)
	{
		m_dwMaxObjectNum		= dwNum;
		
		//¸ÕÀú ¸Þ¸ð¸® ¹è¿­ »ý¼º 
		m_ppMemoryArray = new DATATYPE*[ m_dwMaxObjectNum ];
		if (!m_ppMemoryArray) MessageBox(NULL,"Memory Alloc FAIL : m_ppMemoryArray","ERROR",NULL);
		memset(m_ppMemoryArray,0,sizeof(DATATYPE*)*m_dwMaxObjectNum);
		
		m_ppMemoryArrayFordelete = new DATATYPE*[m_dwMaxObjectNum];
		if (!m_ppMemoryArrayFordelete) MessageBox(NULL,"Memory Alloc FAIL : m_ppMemoryArrayFordelete","ERROR",NULL);
		memset(m_ppMemoryArrayFordelete,0,sizeof(DATATYPE*)*m_dwMaxObjectNum);
		
		m_dwFreeMemoryIndex = 0;
		
		// ½ÇÁ¦ ¸Þ¸ð¸® »ý¼º 
		for( DWORD i =0;i<m_dwMaxObjectNum;i++)
		{
			m_ppMemoryArrayFordelete[ m_dwFreeMemoryIndex ] = new DATATYPE;
			
			if (!m_ppMemoryArrayFordelete[m_dwFreeMemoryIndex]) 
				MessageBox(NULL,"Memory Alloc FAIL : m_ppMemoryArrayFordelete[m_dwFreeMemoryIndex]","ERROR",NULL);
			
			m_ppMemoryArray[ m_dwFreeMemoryIndex ] = m_ppMemoryArrayFordelete[ m_dwFreeMemoryIndex ];	
			m_dwFreeMemoryIndex++;
		}
		
//		InitializeCriticalSectionAndSpinCount(&m_csMemory,200);
	}
	DATATYPE*				Alloc()
	{
//		EnterCriticalSection(&m_csMemory);
		if (m_dwFreeMemoryIndex == 0)
		{
			MessageBox(NULL,"No more memory can alloc : CMemoryPool::Alloc()","error",MB_OK);
//			LeaveCriticalSection(&m_csMemory);
			return NULL;
		}
		m_dwFreeMemoryIndex--;
		DATATYPE* temp = m_ppMemoryArray[ m_dwFreeMemoryIndex ];
//		LeaveCriticalSection(&m_csMemory);
		return temp;
	}
	DWORD					GetFreeIndexNum()
	{
//		EnterCriticalSection(&m_csMemory);
		DWORD temp  = m_dwFreeMemoryIndex;
//		LeaveCriticalSection(&m_csMemory);
		return temp;
	}
	CMemoryPool()
	{
		m_ppMemoryArray			= NULL;
		m_dwFreeMemoryIndex		= 0;
		m_dwMaxObjectNum		= 0;
		m_pMemory				= NULL;
		m_ppMemoryArrayFordelete	=	NULL;
	}
	virtual ~CMemoryPool()
	{
//		DeleteCriticalSection(&m_csMemory);
		
		for( DWORD i =0;i<m_dwMaxObjectNum;i++)
		{
			if (m_ppMemoryArrayFordelete[i])
				delete m_ppMemoryArrayFordelete[i];	
		}
		delete [] m_ppMemoryArray;
		delete [] m_ppMemoryArrayFordelete;
	}
	
};

#endif // !defined(AFX_MEMORYPOOL_H__C8AD2342_A4E4_4A08_8969_F9F4B1CE7E5E__INCLUDED_)
