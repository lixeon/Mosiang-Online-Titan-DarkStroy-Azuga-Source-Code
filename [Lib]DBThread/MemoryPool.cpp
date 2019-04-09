// MemoryPool.cpp: implementation of the CMemoryPool class.
//
//////////////////////////////////////////////////////////////////////
#define _WIN32_WINNT 0x0500

#include "stdafx.h"
#include "MemoryPool.h"
#include <process.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define ERROR_CHECK_CODE 89

CMemoryPool::CMemoryPool(DWORD dwNum, DWORD dwMaxSize)
{
	m_ppMemoryArray			= NULL;
	m_dwFreeMemoryIndex		= 0;
	m_dwMaxObjectNum		= dwNum;
	m_dwMaxObjectSize		= dwMaxSize;
	m_pMemory				= NULL;
	m_ppMemoryArrayFordelete	=	NULL;

	Init();
}

void CMemoryPool::Init()
{
	if (m_dwMaxObjectNum<=0) return;
	if (m_dwMaxObjectSize<=0) return;

	//¸ÕÀú ¸Þ¸ð¸® ¹è¿­ »ý¼º 
	m_ppMemoryArray = new char*[m_dwMaxObjectNum];
	if (!m_ppMemoryArray) MessageBox(NULL,"Memory Alloc FAIL : m_ppMemoryArray","ERROR",NULL);
	memset(m_ppMemoryArray,0,sizeof(char*)*m_dwMaxObjectNum);

	m_ppMemoryArrayFordelete = new char*[m_dwMaxObjectNum];
	if (!m_ppMemoryArrayFordelete) MessageBox(NULL,"Memory Alloc FAIL : m_ppMemoryArrayFordelete","ERROR",NULL);
	memset(m_ppMemoryArrayFordelete,0,sizeof(char*)*m_dwMaxObjectNum);

	m_dwFreeMemoryIndex = 0;

	// ½ÇÁ¦ ¸Þ¸ð¸® »ý¼º 
	for( DWORD i =0;i<m_dwMaxObjectNum;i++)
	{
		// ¸Þ¸ð¸® Ä§¹üÀ» ¸·±â À§ÇØ ½ÇÁ¦º¸´Ù 4¹ÙÀÌÆ® ´õ »ý¼ºÇÑ´Ù.
		m_ppMemoryArrayFordelete[m_dwFreeMemoryIndex] = new char[m_dwMaxObjectSize+4];	
		// ¸ðµÎ 0À¸·Î ¸®¼Â
		memset(m_ppMemoryArrayFordelete[m_dwFreeMemoryIndex],0,m_dwMaxObjectSize);
		memset(m_ppMemoryArrayFordelete[m_dwFreeMemoryIndex]+m_dwMaxObjectSize,ERROR_CHECK_CODE,4);
		if (!m_ppMemoryArrayFordelete[m_dwFreeMemoryIndex]) MessageBox(NULL,"Memory Alloc FAIL : m_ppMemoryArrayFordelete[m_dwFreeMemoryIndex]","ERROR",NULL);
		m_ppMemoryArray[m_dwFreeMemoryIndex] = m_ppMemoryArrayFordelete[m_dwFreeMemoryIndex];	
		m_dwFreeMemoryIndex++;
	}
	InitializeCriticalSectionAndSpinCount(&m_csMemory,200);

}

char* CMemoryPool::Alloc()
{
	EnterCriticalSection(&m_csMemory);
	if (m_dwFreeMemoryIndex == 0)
	{
//		MessageBox(NULL,"No more memory can alloc : CMemoryPool::Alloc()","error",MB_OK);
		LeaveCriticalSection(&m_csMemory);
		return NULL;
	}
	m_dwFreeMemoryIndex--;
	char* temp = m_ppMemoryArray[m_dwFreeMemoryIndex];
	LeaveCriticalSection(&m_csMemory);
	// ¸Þ¸ð¸® ºí·°ÀÌ Á¤»óÀÎÁö È®ÀÎ
	if (temp[m_dwMaxObjectSize]  == ERROR_CHECK_CODE && temp[m_dwMaxObjectSize+1]  == ERROR_CHECK_CODE && temp[m_dwMaxObjectSize+2]  == ERROR_CHECK_CODE && temp[m_dwMaxObjectSize+3]  == ERROR_CHECK_CODE)
		return temp;
	else 
	{
		MessageBox(NULL,"CMemoryPool::Alloc() : Memory Block was broken","error",MB_OK);
		return NULL;
	}
}

void CMemoryPool::Free(char* pTarget)
{
	if (!pTarget) return;
	EnterCriticalSection(&m_csMemory);
	m_ppMemoryArray[m_dwFreeMemoryIndex] = pTarget;
	m_dwFreeMemoryIndex++;
	LeaveCriticalSection(&m_csMemory);
	if (pTarget[m_dwMaxObjectSize]	  == ERROR_CHECK_CODE && 
		pTarget[m_dwMaxObjectSize+1]  == ERROR_CHECK_CODE && 
		pTarget[m_dwMaxObjectSize+2]  == ERROR_CHECK_CODE && 
		pTarget[m_dwMaxObjectSize+3]  == ERROR_CHECK_CODE)
		return;
	else 
	{
		MessageBox(NULL,"CMemoryPool::Free() : Memory Block was broken","error",MB_OK);
		return;
	}
}
DWORD	CMemoryPool::GetFreeIndexNum()
{
	EnterCriticalSection(&m_csMemory);
	DWORD temp  = m_dwFreeMemoryIndex;
	LeaveCriticalSection(&m_csMemory);
	return temp;
}


CMemoryPool::~CMemoryPool()
{
	DeleteCriticalSection(&m_csMemory);

	for( DWORD i =0;i<m_dwMaxObjectNum;i++)
	{
		if (m_ppMemoryArrayFordelete[i])
			delete [] m_ppMemoryArrayFordelete[i];	
	}
	delete [] m_ppMemoryArray;
	delete [] m_ppMemoryArrayFordelete;
}


