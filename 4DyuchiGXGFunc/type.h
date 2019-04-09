#pragma once

typedef void* ITEMQ_HANDLE;
typedef void* STMPOOL_HANDLE;
typedef void* VBHASH_HANDLE;
typedef void* QBHASH_HANDLE;
typedef void* INDEXCR_HANDLE;
typedef void* ITEMTABLE_HANDLE;
typedef void* DPCQ_HANDLE;
typedef void* BSAE_HANDLE;
typedef void* VHEAP_HANDLE;
typedef void* STRCPOOL_HANDLE;

struct SORT_ITEM_INT
{
	int		iCmp;
	void*	pItem;
};

struct SORT_ITEM_FLOAT
{
	float	fCmp;
	void*	pItem;
};

#define SORT_ITEM_SIZE	8

struct BSAENODE;

struct ITEM_CONTAINER_AE
{
	BSAENODE*			pOwner;
		
	ITEM_CONTAINER_AE*	pPrv;
	ITEM_CONTAINER_AE*	pNext;
	void*				pItem;
	DWORD				dwKey;
};


struct BSAENODE
{
	DWORD				dwKey;
	BSAENODE*			pParent;
	BSAENODE*			pLeft;
	BSAENODE*			pRight;
	DWORD				dwRefCount;
	ITEM_CONTAINER_AE*	pItemContainer;
};


struct SORT_LINK
{
	SORT_LINK*		pPrv;
	void*			pItem;
	SORT_LINK*		pNext;
};

enum HEAP_FAILE_ERROR_TYPE
{
	HEAP_ALLOC_FAIL_ERROR_GREATOR_THAN_ALIGNSIZE	= 0x00000001,	// 할당하려는 사이즈가 얼라인사이즈보다 크다.
	HEAP_ALLOC_FAIL_ERROR_NOT_AVAILABLE_MEM_BLOCK	= 0x00000002,	// 사이즈에 맞는 메모리 블럭이 없다.메모리 부족.
	HEAP_ALLOC_FAIL_ERROR_NOT_AVAILABLE_BLOCK_DESC	= 0x00000004,	// 메모리 블럭 디스크립터가 모자란다.
	HEAP_ALLOC_FAIL_ERROR_FAIL_TO_ALIGN				= 0x00000008	// 정렬에 실패했다
};

typedef void* (*RESOURCR_ALLOC_FUNC)(void* pArg1,void* pArg2,void* pArg3,DWORD dwSize);
typedef void (*RESOURCR_FREE_FUNC)(void* pResource);


