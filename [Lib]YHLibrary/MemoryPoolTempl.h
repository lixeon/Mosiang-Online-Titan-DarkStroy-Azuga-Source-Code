// MemoryPoolTempl.h: interface for the CMemoryPoolTempl class.
// Memory Pool Management Class
// source code edited by taiyo (zmnkmw@chollian.net)
// 2003-3-31
// 
//-------------------------------------------------------------------
// History.
// 2004-4-28	:	valid memory cheing hash list is added
//
// There are things for impovement as follows
// - memory optimzing
// - inherited template class extension
// - cache hit rate impoving
// - multi-thread safety
//-------------------------------------------------------------------
//
//////////////////////////////////////////////////////////////////////

#ifndef __CMEMORYPOOLTEMPL_H__
#define __CMEMORYPOOLTEMPL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "windows.h"
#include "assert.h"
#include "HashTable.h"

template<class Type>
class CMemoryPoolTempl;


void OutputFile(char* str);

template <class DataType>
class CBand
{
	
	DataType ** ppDataType;
	DataType * pDeleteArray;
	DWORD nMaxNum;
	DWORD FreeIdx;
public:
	CBand * pPrev;
	CBand * pNext;
	CBand():pPrev(NULL),pNext(NULL){}
	virtual ~CBand()
	{
		pPrev=NULL;pNext=NULL;
		delete [] (ppDataType);
		delete [] (pDeleteArray);
	}
	static CBand * Banding(CBand*& head, DWORD nMax)
	{
		CBand<DataType>* pp = new CBand<DataType>;
		pp->ppDataType = new DataType*[nMax];
		memset(pp->ppDataType, 0, sizeof(DataType*)*nMax);
		pp->pDeleteArray = new DataType[nMax];
		for(DWORD i = 0 ; i < nMax ; i++)
		{
			pp->ppDataType[i] = &pp->pDeleteArray[i];
		}
		pp->nMaxNum = nMax;
		pp->FreeIdx = nMax;
		pp->pNext = head;
		if(head)
		head->pPrev = pp;
		head = pp;
		
		return pp;
	}
	void FreeChain()
	{
		//head node one call, all delete;
		CBand<DataType> * db = (CBand *)this;
		while(db)
		{
			CBand<DataType> * tmp = db;
			db = db->pNext;
			delete tmp;
		}
	}
	DataType * Alloc()
	{
		if( FreeIdx == 0 ) return NULL;
		return ppDataType[--FreeIdx];

/*
	//	TASSERT(FreeIdx-1 >= 0);
		if(--FreeIdx == -1)
		{
			FreeIdx = 0;
			return NULL;
		}
		else
			return ppDataType[FreeIdx];
*/
	}
	BOOL Free(DataType * data)
	{
	//	TASSERT(FreeIdx+1 < nMaxNum);
		if(FreeIdx < nMaxNum )
			ppDataType[FreeIdx++] = data;
		else
			return FALSE;

		return TRUE;
	}
};

#define TASSERT(a) ((void)0)
//#define TASSERT assert

template<class Type>
class CMemoryPoolTempl  
{
#ifdef _DEBUGFREEERROR
	CYHHashTable<Type> m_pFreeErrorTable;
#endif

	CBand<Type> * m_pBandHead;
	CBand<Type> * m_pFreeBand;
	DWORD m_blockNum;
//	DWORD m_dwBandNum;
//	DWORD m_typeCount;
	char szTemplName[128];

	//
//	int m_totalNum;

public:

	
	CMemoryPoolTempl()
	{
		m_pBandHead = NULL;
		m_pFreeBand = NULL;
//		m_typeCount = 0;
//		m_dwBandNum = 0;
		szTemplName[0] = 0;
//		m_totalNum = 0;
	}
	virtual ~CMemoryPoolTempl()
	{
		Release();
	}

	void Release()
	{
		if(m_pBandHead)
		{
			m_pBandHead->FreeChain();
			m_pBandHead = NULL;
			m_pFreeBand = NULL;
		}

#ifdef _DEBUGFREEERROR
		m_pFreeErrorTable.RemoveAll();
#endif
	}

	void Init(DWORD startBlockNum, DWORD blockNum, char * szName)
	{

#ifdef _DEBUGFREEERROR
		m_pFreeErrorTable.Initialize(startBlockNum+blockNum);
#endif
		strncpy(szTemplName, szName, 128);
		m_blockNum = blockNum;
		if(m_pBandHead == NULL)
		{
			CBand<Type>::Banding(m_pBandHead, startBlockNum);
			m_pFreeBand = m_pBandHead;
			TASSERT(m_pFreeBand);
		}

//		m_totalNum = startBlockNum;

		//sprintf(pDebug, "******* MemoryPooltempl<%s>초기화******(%d, %d)\n", szTemplName, startBlockNum, blockNum);
		//OutputDebugString(pDebug);
//		sprintf(pDebug, "<%s>-Init	%d	%d	%d	%d	%d\n", szTemplName, startBlockNum, m_blockNum, m_typeCount, m_totalNum, sizeof(Type*)*m_totalNum);
		//OutputFile(pDebug);
		
	}
	char pDebug[256];

	Type * Alloc()
	{
		if( m_pFreeBand == NULL ) return NULL;

		Type * node = m_pFreeBand->Alloc();
		if(node == NULL)
		{
			if(!m_pFreeBand->pPrev)
			{
				CBand<Type>::Banding(m_pBandHead, m_blockNum);
				m_pFreeBand = m_pBandHead;	
				TASSERT(m_pFreeBand);
//				++m_dwBandNum;
				//sprintf(pDebug, "******* <%s>alloc Band 늘어남(%u) (blockSize:%u, cnt:%u)\n", szTemplName, m_dwBandNum, m_blockNum, m_typeCount);
				//OutputDebugString(pDebug);
//				m_totalNum += m_blockNum;
//				sprintf(pDebug, "<%s>-Increse	%d	%d	%d	%d	%d\n", szTemplName, m_dwBandNum, m_blockNum, m_typeCount, m_totalNum, sizeof(Type*)*m_totalNum);
//				OutputFile(pDebug);
			}
			else
			{
				m_pFreeBand = m_pFreeBand->pPrev;
				TASSERT(m_pFreeBand);
				//sprintf(pDebug, "******* <%s>alloc Band줄어듬(%u) (blockSize:%u, cnt:%u)\n", szTemplName, m_dwBandNum, m_blockNum, m_typeCount);
				//OutputDebugString(pDebug);
//				m_totalNum -= m_blockNum;
//				sprintf(pDebug, "<%s>-Decrese	%d	%d	%d	%d	%d\n", szTemplName, m_dwBandNum, m_blockNum, m_typeCount, m_totalNum, sizeof(Type*)*m_totalNum);
//				OutputFile(pDebug);
			}
		
			node = Alloc();
			return node;
		}

#ifdef _DEBUGFREEERROR
		m_pFreeErrorTable.Add(node, (DWORD)node);
#endif
//		++m_typeCount;

		return node;
	}

	// IF m_pFreeBand->pNext의 m_pFreeBand가 NULL
	// == 가능성 ==
	// 1. Free(dn)을 BlockNum 개수이상을 호출했을 때 
	// 2. 또는 Free(dn)의 dn이 중복 Free되었을 때
	void Free(Type * dn)
	{
		TASSERT(dn);

		if(m_pFreeBand == NULL) return;

		if(!m_pFreeBand->Free(dn))
		{	
			m_pFreeBand = m_pFreeBand->pNext;
			TASSERT(m_pFreeBand);
			Free(dn);
			return;
		}

#ifdef _DEBUGFREEERROR
		TASSERT(m_pFreeErrorTable.GetData((DWORD)dn));
		m_pFreeErrorTable.Remove((DWORD)dn);
#endif

//		--m_typeCount;
	}	
};

#endif // __CMEMORYPOOLTEMPL_H__
