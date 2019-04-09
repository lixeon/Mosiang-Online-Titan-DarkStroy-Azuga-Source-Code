
#if !defined(AFX_VLVERTEXBUFFERPOOL_H__A4EDEFE9_F7C2_4C07_9BFE_3C73209AC1E2__INCLUDED_)
#define AFX_VLVERTEXBUFFERPOOL_H__A4EDEFE9_F7C2_4C07_9BFE_3C73209AC1E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VertexBuffer.h"
#include "../4DyuchiGXGFunc/global.h"
#include "hashsearch.h"



class CoD3DDevice;
class CVBHeap;

enum VBCACHE_ITEM_FLAG
{
	VBCACHE_ITEM_FLAG_ENABLE = 0x00000000,
	VBCACHE_ITEM_FLAG_DISABLE = 0x00000001
};

struct VBCACHE_KEY
{
	DWORD			dwFrame;
	void*			pMotionUID;
	void*			pObject;
//	DWORD			dwObjCount;
};

struct VBCACHE_ITEM
{
	VBCACHE_KEY		key;
	CVertexBuffer	VB;
	BYTE*			pMem;
	ITEM_CONTAINER*	pSearchHandle;
	DWORD			dwIndexInAllocatedTable;
	int				iCmp;
};



class CVBCache  
{
	DWORD			m_dwTimeCount;
	CVBHeap*		m_pVBHeap;
	CHashSearch		m_HashSearch;
	CoD3DDevice*	m_pRenderer;
	float			m_fFreeVBCacheRate;	
	VBCACHE_ITEM**	m_ppVBCacheItemAllocated;
	DWORD			m_dwMaxRefIndexNum;
	
	
	VBCACHE_ITEM*	m_pVBCachePool;
	DWORD			m_dwMaxBufferNum;

	VBCACHE_ITEM**	m_ppVBCacheItemReserved;
	DWORD			m_dwAllocatedVBCacheItemNum;
	DWORD			m_dwReservedVBCacheItemNum;

	DWORD			m_dwFreeUnitNum;
	DWORD			m_dwRequestNum;
	DWORD			m_dwHitNum;


	VBCACHE_ITEM*	AllocVBCacheItem(VBCACHE_KEY* pKey,DWORD dwVerticesNum);
	void			FreeVBCacheItem(DWORD dwNum);
	void			ClearMember();

public:
	BOOL			Initialize(CoD3DDevice* pRenderer,DWORD dwMaxBufferNum,DWORD dwMaxVerticesNumPerVertexBuffer,DWORD dwMaxVertexBufferNum);
	void			SetFreeVBCacheRate(float fVal);
	float			GetFreeVBCacheRate() {return m_fFreeVBCacheRate;}
	DWORD			GetCacheHitRate();
	CVertexBuffer*	GetVB(BOOL* bNeedUpdate,VBCACHE_KEY* pKey,DWORD dwRefIndex,DWORD dwVertexNum);
	DWORD			ClearCacheWithIDIMeshObject(void* pObject);
	DWORD			ClearCacheWithMotionUID(void* pMotionUID);

	void			Cleanup();
	CVBCache();
	~CVBCache();

};

#endif // !defined(AFX_VLVERTEXBUFFERPOOL_H__A4EDEFE9_F7C2_4C07_9BFE_3C73209AC1E2__INCLUDED_)
