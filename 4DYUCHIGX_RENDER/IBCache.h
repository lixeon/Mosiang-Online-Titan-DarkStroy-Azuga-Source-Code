#ifndef IBCACHE_H
#define IBCACHE_H

#include "IndexBuffer.h"
#include "../4DyuchiGXGFunc/global.h"


class CoD3DDevice;
class CIBHeap;


struct IBCACHE_ITEM
{
	CIndexBuffer	IB;
	DWORD			dwIndexInAllocatedTable;
	int				iCmp;
	CIndexBuffer**	ppIBAddrInOwner;
	void*			pOwner;
};


struct IBCACHE_TABLE_REFINDEX
{
	IBCACHE_ITEM*	pIBCacheItem;
	DWORD			dwFlag;
};


class CIBCache  
{
	DWORD			m_dwTimeCount;
	CIBHeap*		m_pIBHeap;

	CoD3DDevice*	m_pRenderer;
//	CIndexBuffer	m_IBPublic;
	
	IBCACHE_ITEM**	m_ppIBCacheItemAllocated;
	
	
	IBCACHE_ITEM*	m_pIBCachePool;
	DWORD			m_dwMaxBufferNum;


	IBCACHE_ITEM**	m_ppIBCacheItemReserved;
	DWORD			m_dwAllocatedIBCacheItemNum;
	DWORD			m_dwReservedIBCacheItemNum;

	DWORD			m_dwFreeUnitNum;

	DWORD			m_dwMissNum;
	DWORD			m_dwRequestNum;


	BOOL			m_bEnable;


	IBCACHE_ITEM*	AllocIBCacheItem(int** ppiCountVariable,DWORD dwIndicesNum,CIndexBuffer** ppIBAddrInOwner,void* pOwner);
	
	void			FreeIBCacheItem(DWORD dwNum);
	void			ClearMember();

public:
	void			Hit()		{ m_dwRequestNum++;}
	BOOL			Initialize(CoD3DDevice* pRenderer,DWORD dwMaxBufferNum,DWORD dwMaxIndicesNumPerIndexBuffer,DWORD dwMaxIndexBufferNum);
	DWORD			GetCacheHitRate();
	BOOL			GetIB(int** ppiCountVariable,DWORD dwIndicesNum,CIndexBuffer** ppIBAddrInOwner,void* pOwner);
	void			FreeIB(CIndexBuffer* pIB);

	
	void			Cleanup();
	CIBCache();
	~CIBCache();

};


#endif 
