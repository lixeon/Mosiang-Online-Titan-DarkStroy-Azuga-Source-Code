// MiruquMemoryPoolEx.h: interface for the CMemoryPoolTempl class.
// Memory Pool Management Class
// source code edited by taiyo (zmnkmw@chollian.net)
// 2003-3-31
// 
//-------------------------------------------------------------------
// History.
// 2004-5-14	:	memory optimzing
// 2004-5-14	:	inherited template class extension

// There are things for impovement as follows
// - cache hit rate impoving
// - multi-thread safety
// - debuging option
//-------------------------------------------------------------------
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_MIRUQUMEMORYPOOLEX_H__1CE0FED2_44B3_425D_AC09_ED0C08A0D172__INCLUDED_)
#define AFX_MIRUQUMEMORYPOOLEX_H__1CE0FED2_44B3_425D_AC09_ED0C08A0D172__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>


template<class Type, DWORD _StartBockNum, DWORD _ExBlockNum>
class CMiruquMemoryPoolEx;

#define TASSERT assert
//#define _MIRUKUMEMORYPOOL_DEBUG

#ifdef _MIRUKUMEMORYPOOL_DEBUG

#endif //_MIRUKUMEMORYPOOL_DEBUG


template <class DataType>
class CBandEx
{
	DataType ** ppDataType;
	DataType * pDeleteArray;
	DWORD nMaxNum;
	DWORD FreeIdx;
public:
	CBandEx * pPrev;
	CBandEx * pNext;
	CBandEx():pPrev(NULL),pNext(NULL){}
	virtual ~CBandEx()
	{
		pPrev=NULL;pNext=NULL;
		delete [] (ppDataType);
		delete [] (pDeleteArray);
	}
	static inline CBandEx * Banding(CBandEx*& head, DWORD nMax)
	{
		CBandEx<DataType>* pp = new CBandEx<DataType>;
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
		CBandEx<DataType> * db = (CBandEx *)this;
		while(db)
		{
			CBandEx<DataType> * tmp = db;
			db = db->pNext;
			delete tmp;
		}
	}
	char pDebug[256];
	inline DataType * Alloc()
	{
		sprintf(pDebug, "!!Alloc(idx:%d)\n", FreeIdx-1);
		OutputDebugString(pDebug);
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

template<class Type, DWORD _StartBockNum = 0, DWORD _ExBlockNum = 0>
class CMiruquMemoryPoolEx  
{
	//static CRITICAL_SECTION m_cs;
	static CBandEx<Type> * m_pBandHead;
	static CBandEx<Type> * m_pFreeBand;
	static DWORD m_blockNum;
	static DWORD m_dwBandNum;
	static DWORD m_typeCount;
	static char szTemplName[128];
	static char pDebug[256];
	//static inline void Lock() { EnterCriticalSection(&m_cs); }
	//static inline void Unlock() { LeaveCriticalSection(&m_cs); }
public:
	
	CMiruquMemoryPoolEx()
	{
		//InitializeCriticalSectionAndSpinCount(&m_cs, 2000);
		if(_StartBockNum != 0 && _ExBlockNum != 0)
			Init(_StartBockNum, _ExBlockNum);
	}
	virtual ~CMiruquMemoryPoolEx()
	{
		//Release();
	}

	static inline void Release()
	{
	//	DeleteCriticalSection(&m_cs);
		if(m_pBandHead)
		{
			m_pBandHead->FreeChain();
			m_pBandHead = NULL;
			m_pFreeBand = NULL;
		}
	}

	static inline void Init(DWORD startBlockNum, DWORD blockNum, char * szName)
	{
		strncpy(szTemplName, szName, 128);
		Init(startBlockNum, blockNum);
	}
	static inline void Init(DWORD startBlockNum, DWORD blockNum)
	{	
		if(m_pBandHead == NULL)
		{
			m_blockNum = blockNum;
			CBandEx<Type>::Banding(m_pBandHead, startBlockNum);
			m_pFreeBand = m_pBandHead;
			//TASSERT(m_pFreeBand);
		}
		
		sprintf(pDebug, "******* MemoryPooltempl<%s>초기화******(%d, %d)\n", szTemplName, startBlockNum, blockNum);
		OutputDebugString(pDebug);
	}

	static inline Type * Alloc()
	{
		Type * node = m_pFreeBand->Alloc();
		if(node == NULL)
		{
			if(!m_pFreeBand->pPrev)
			{
				CBandEx<Type>::Banding(m_pBandHead, m_blockNum);
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
		sprintf(pDebug, "******* <%s>alloc(%u) (blockSize:%u, cnt:%u)\n", szTemplName, m_dwBandNum, m_blockNum, m_typeCount);
		OutputDebugString(pDebug);
		return node;
	}

	// IF m_pFreeBand->pNext의 m_pFreeBand가 NULL
	// == 가능성 ==
	// 1. Free(dn)을 BlockNum 개수이상을 호출했을 때 
	// 2. 또는 Free(dn)의 dn이 중복 Free되었을 때
	static inline void Free(Type * dn)
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
		sprintf(pDebug, "******* <%s>free(%u) (blockSize:%u, cnt:%u)\n", szTemplName, m_dwBandNum, m_blockNum, m_typeCount);
		OutputDebugString(pDebug);
	}

	inline void* operator new(size_t nSize, LPCSTR lpszFileName, int nLine)
	{
		return operator new( nSize );
	}
	inline void operator delete(void* p, LPCSTR lpszFileName, int nLine)
	{
		operator delete( p );
	}
	inline void* operator new( size_t size )
	{
		return Alloc();
	}
	inline void operator delete( void* p )
	{
		Free((Type*)p);
	}
};

//template <class T, DWORD N, DWORD N> CRITICAL_SECTION CMiruquMemoryPoolEx<T,N,N>::m_cs;
template <class T, DWORD N, DWORD NE> CBandEx<T> * CMiruquMemoryPoolEx<T,N,NE>::m_pBandHead	= NULL;
template <class T, DWORD N, DWORD NE> CBandEx<T> * CMiruquMemoryPoolEx<T,N,NE>::m_pFreeBand	= NULL;
template <class T, DWORD N, DWORD NE> DWORD CMiruquMemoryPoolEx<T,N,NE>::m_blockNum				= 0;
template <class T, DWORD N, DWORD NE> DWORD CMiruquMemoryPoolEx<T,N,NE>::m_dwBandNum				= 0;
template <class T, DWORD N, DWORD NE> DWORD CMiruquMemoryPoolEx<T,N,NE>::m_typeCount				= 0;
template <class T, DWORD N, DWORD NE> char CMiruquMemoryPoolEx<T,N,NE>::szTemplName[]				;
template <class T, DWORD N, DWORD NE> char CMiruquMemoryPoolEx<T,N,NE>::pDebug[]					;


#endif // !defined(AFX_MIRUQUMEMORYPOOLEX_H__1CE0FED2_44B3_425D_AC09_ED0C08A0D172__INCLUDED_)
