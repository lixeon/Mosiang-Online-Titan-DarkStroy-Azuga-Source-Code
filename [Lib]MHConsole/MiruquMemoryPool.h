// MiruquMemoryPool.h: interface for the CMemoryPoolTempl class.
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
// - debuging option
//-------------------------------------------------------------------
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MIRUQUMEMORYPOOL_H__5F3A7400_18B9_4BF4_B192_AFE6A94E869C__INCLUDED_)
#define AFX_MIRUQUMEMORYPOOL_H__5F3A7400_18B9_4BF4_B192_AFE6A94E869C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

template<class Type>
class CMiruquMemoryPool;


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
	static inline CBand * Banding(CBand*& head, DWORD nMax)
	{
		CBand<DataType>* pp = new CBand<DataType>;
		pp->ppDataType = new DataType*[nMax];
		memset(pp->ppDataType, 0, sizeof(DataType*)*nMax);
		pp->pDeleteArray = new DataType[nMax];
		for(DWORD i = 0 ; i < nMax ; ++i)
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
	inline void FreeChain()
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
	inline DataType * Alloc()
	{
	//	TASSERT(FreeIdx-1 >= 0);
		if(--FreeIdx == -1)
		{
			FreeIdx = 0;
			return NULL;
		}
		else
			return ppDataType[FreeIdx];
	}
	inline BOOL Free(DataType * data)
	{
	//	TASSERT(FreeIdx+1 < nMaxNum);
		if(FreeIdx  < nMaxNum )
		{
			ppDataType[FreeIdx] = data;
			++FreeIdx;
			return TRUE;
		}
		else
			return FALSE;
	}
};

template<class Type>
class CMiruquMemoryPool  
{
	//CRITICAL_SECTION m_cs;
	CBand<Type> * m_pBandHead;
	CBand<Type> * m_pFreeBand;
	DWORD m_blockNum;
	DWORD m_dwBandNum;
	DWORD m_typeCount;
	char szTemplName[128];
	char pDebug[256];
	//inline void Lock() { EnterCriticalSection(&m_cs); }
	//inline void Unlock() { LeaveCriticalSection(&m_cs); }
public:
	
	CMiruquMemoryPool():
	m_pBandHead(NULL),
	m_pFreeBand(NULL),
	m_typeCount(0),
	m_dwBandNum(0)
	{
		szTemplName[0] = 0;	
		//InitializeCriticalSectionAndSpinCount(&m_cs, 2000);
	}
	virtual ~CMiruquMemoryPool()
	{
		Release();
	}

	inline void Release()
	{
		//DeleteCriticalSection(&m_cs);
		if(m_pBandHead)
		{
			m_pBandHead->FreeChain();
			m_pBandHead = NULL;
			m_pFreeBand = NULL;
		}
	}

	inline void Init(DWORD startBlockNum, DWORD blockNum, char * szName)
	{
		strncpy(szTemplName, szName, 128);
		Init(startBlockNum, blockNum);
	}
	inline void Init(DWORD startBlockNum, DWORD blockNum)
	{
		m_blockNum = blockNum;
		if(m_pBandHead == NULL)
		{
			CBand<Type>::Banding(m_pBandHead, startBlockNum);
			m_pFreeBand = m_pBandHead;
			//TASSERT(m_pFreeBand);
		}
		
		sprintf(pDebug, "******* MemoryPooltempl<%s>초기화******(%d, %d)\n", szTemplName, startBlockNum, blockNum);
		OutputDebugString(pDebug);
	}

	inline Type * Alloc()
	{
		Type * node = m_pFreeBand->Alloc();
		if(node == NULL)
		{
			if(!m_pFreeBand->pPrev)
			{
				CBand<Type>::Banding(m_pBandHead, m_blockNum);
				m_pFreeBand = m_pBandHead;	
				//TASSERT(m_pFreeBand);
				++m_dwBandNum;
				sprintf(pDebug, "******* <%s>alloc Band 늘어남(%u) (blockSize:%u, cnt:%u)\n", szTemplName, m_dwBandNum, m_blockNum, m_typeCount);
				OutputDebugString(pDebug);
			}
			else
			{
				m_pFreeBand = m_pFreeBand->pPrev;
				//TASSERT(m_pFreeBand);
				sprintf(pDebug, "******* <%s>alloc Band줄어듬(%u) (blockSize:%u, cnt:%u)\n", szTemplName, m_dwBandNum, m_blockNum, m_typeCount);
				OutputDebugString(pDebug);
			}

			node = Alloc();

			return node;
		}
		
		++m_typeCount;
		return node;
	}

	// IF m_pFreeBand->pNext의 m_pFreeBand가 NULL
	// == 가능성 ==
	// 1. Free(dn)을 BlockNum 개수이상을 호출했을 때 
	// 2. 또는 Free(dn)의 dn이 중복 Free되었을 때
	inline void Free(Type * dn)
	{
		//TASSERT(dn);
		if(!m_pFreeBand->Free(dn))
		{	
			m_pFreeBand = m_pFreeBand->pNext;
			//SSERT(m_pFreeBand);
			Free(dn);
			return;
		}

		--m_typeCount;
	}
};

#endif // !defined(AFX_MIRUQUMEMORYPOOL_H__5F3A7400_18B9_4BF4_B192_AFE6A94E869C__INCLUDED_)
