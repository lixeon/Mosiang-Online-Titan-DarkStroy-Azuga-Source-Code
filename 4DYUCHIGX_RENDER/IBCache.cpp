#include "IBCache.h"
#include "CoD3DDevice.h"
#include "global_variable.h"
#include "Base_FaceGroup.h"
#include "IBHeap.h"

void __stdcall QSortIntASC(IBCACHE_ITEM** ppArray,DWORD dwNum);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void CIBCache::ClearMember()
{
	m_pRenderer = NULL;
		
	m_ppIBCacheItemAllocated = 0;

	m_pIBHeap = NULL;
	m_pIBCachePool = NULL;
	m_dwMaxBufferNum = 0;

	m_ppIBCacheItemReserved = NULL;
	m_dwAllocatedIBCacheItemNum = 0;
	m_dwReservedIBCacheItemNum = 0;

	m_dwFreeUnitNum = 0;
	m_dwRequestNum = 0;
	m_dwMissNum = 0;
	m_bEnable = 0;
	m_dwTimeCount = 0;
}
CIBCache::CIBCache()
{
	ClearMember();
}


DWORD CIBCache::GetCacheHitRate()
{
	float fPercentage;

	if (!m_dwRequestNum)
	{
		fPercentage = 0;
		goto lb_return;
	}

	fPercentage = 100.0f - ((float)m_dwMissNum / (float)m_dwRequestNum) * 100.0f;
	m_dwMissNum = 0;
	m_dwRequestNum = 0;
lb_return:
	return (DWORD)fPercentage;
}


BOOL CIBCache::Initialize(CoD3DDevice* pRenderer,DWORD dwMaxBufferNum,DWORD dwMaxIndicesNumPerIndexBuffer,DWORD dwMaxIndexBufferNum)
{
	
	m_pRenderer = pRenderer;
	m_dwMaxBufferNum = dwMaxBufferNum;

	m_dwFreeUnitNum = (DWORD)( (float)m_dwMaxBufferNum / 5.7f );
	if (m_dwFreeUnitNum < 1)
		m_dwFreeUnitNum = 1;

	m_pIBCachePool = new IBCACHE_ITEM[m_dwMaxBufferNum];
	memset(m_pIBCachePool,0,sizeof(IBCACHE_ITEM)*m_dwMaxBufferNum);

	m_ppIBCacheItemReserved = new IBCACHE_ITEM*[m_dwMaxBufferNum];
	memset(m_ppIBCacheItemReserved,0,sizeof(IBCACHE_ITEM*)*m_dwMaxBufferNum);

	for (DWORD i=0; i<m_dwMaxBufferNum; i++)
	{
		m_ppIBCacheItemReserved[i] = m_pIBCachePool+i;

	}
	m_ppIBCacheItemAllocated = new IBCACHE_ITEM*[m_dwMaxBufferNum];
	memset(m_ppIBCacheItemAllocated,0,sizeof(IBCACHE_ITEM*)*m_dwMaxBufferNum);


	m_pRenderer->CreateResourcePool((IResourcePool**)&m_pIBHeap,RESORUCE_POOL_TYPE_IBHEAP);
	m_pIBHeap->Initialize(pRenderer->GetD3DResourceManager(),D3DUSAGE_WRITEONLY,D3DPOOL_DEFAULT,dwMaxIndicesNumPerIndexBuffer,dwMaxIndexBufferNum,0,NOT_USE_AUTO_SCHEDLUE);
//	m_pIBHeap->Initialize(pRenderer->GetD3DResourceManager(),D3DUSAGE_WRITEONLY,D3DPOOL_DEFAULT,dwMaxIndicesNumPerIndexBuffer,dwMaxIndexBufferNum,0,ALAWAYS_KEEP_BUFFER | NOT_USE_AUTO_SCHEDLUE);

	//m_IBPublic.CreateIndexBuffer(pRenderer->GetD3DResourceManager(),NULL,D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,D3DPOOL_DEFAULT ,MAX_PRJMESH_INDICES_NUM);

	m_bEnable = TRUE;

	m_dwReservedIBCacheItemNum = m_dwMaxBufferNum;
	
	return TRUE;
}

IBCACHE_ITEM* CIBCache::AllocIBCacheItem(int** ppiCountVariable,DWORD dwIndicesNum,CIndexBuffer** ppIBAddrInOwner,void* pOwner)
{
	IBCACHE_ITEM*	pIBItem = NULL;

	if (!m_dwReservedIBCacheItemNum)
		goto lb_return;


	pIBItem = m_ppIBCacheItemReserved[0];
	pIBItem->iCmp = 0;
	DWORD	dwErrorCode;
	if (!m_pIBHeap->AllocIB(&dwErrorCode,&pIBItem->IB,dwIndicesNum,NULL,&pIBItem->IB.m_bEnable,NULL))
	{
		pIBItem = NULL;
		goto lb_return;
	}

	m_ppIBCacheItemReserved[0] = m_ppIBCacheItemReserved[m_dwReservedIBCacheItemNum-1];
	m_ppIBCacheItemReserved[m_dwReservedIBCacheItemNum-1] = NULL;
	m_ppIBCacheItemAllocated[m_dwAllocatedIBCacheItemNum] = pIBItem;


	pIBItem->pOwner = pOwner;
	pIBItem->dwIndexInAllocatedTable = m_dwAllocatedIBCacheItemNum;
	pIBItem->ppIBAddrInOwner = ppIBAddrInOwner;
	*ppIBAddrInOwner = &pIBItem->IB;
	*ppiCountVariable = &pIBItem->iCmp;
	

#ifdef _DEBUG
	if (!pIBItem->IB.GetIndicesNum())
	{
		char	txt[512];
		wsprintf(txt,"Fail to CIBCache::AlloCIBCacheItem() !pIBItem->IB.GetIndicesNum(), File : %s , Line : %d \n",__FILE__,__LINE__);
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
	}
#endif


	m_dwAllocatedIBCacheItemNum++;
	m_dwReservedIBCacheItemNum--;
lb_return:
	return pIBItem;
}

void CIBCache::FreeIB(CIndexBuffer* pIB)
{

	IBCACHE_ITEM*	pDelItem = (IBCACHE_ITEM*)pIB;
	

//	if (!m_dwAllocatedIBCacheItemNum)
//		__asm int 3
				
	m_ppIBCacheItemReserved[m_dwReservedIBCacheItemNum] = pDelItem;
	

	m_ppIBCacheItemAllocated[m_dwAllocatedIBCacheItemNum-1]->dwIndexInAllocatedTable = pDelItem->dwIndexInAllocatedTable;
	m_ppIBCacheItemAllocated[pDelItem->dwIndexInAllocatedTable] = m_ppIBCacheItemAllocated[m_dwAllocatedIBCacheItemNum-1];
	
	m_ppIBCacheItemAllocated[m_dwAllocatedIBCacheItemNum-1] = NULL;
	pDelItem->IB.Cleanup();
	*pDelItem->ppIBAddrInOwner = NULL;

#ifdef _DEBUG
	pDelItem->dwIndexInAllocatedTable = 0xcccccccc;

#else
	pDelItem->dwIndexInAllocatedTable = NULL;

#endif

	m_dwAllocatedIBCacheItemNum--;
	m_dwReservedIBCacheItemNum++;

}


void CIBCache::FreeIBCacheItem(DWORD dwNum)
{
	DWORD	i;

	IBCACHE_ITEM*	pDelItem;

	if (dwNum > m_dwAllocatedIBCacheItemNum)
		dwNum = m_dwAllocatedIBCacheItemNum;
	
	if (!dwNum)
	{
		return;
	}


	QSortIntASC(m_ppIBCacheItemAllocated,m_dwAllocatedIBCacheItemNum);

	for (i=0; i<dwNum; i++)
	{
		pDelItem = m_ppIBCacheItemAllocated[i];
		
		
#ifdef _DEBUG
		pDelItem->dwIndexInAllocatedTable = 0xcccccccc;
#else
		pDelItem->dwIndexInAllocatedTable = NULL;
#endif
		pDelItem->IB.Cleanup();
		*pDelItem->ppIBAddrInOwner = NULL;

		m_ppIBCacheItemReserved[m_dwReservedIBCacheItemNum+i] = pDelItem;
	}
	for (i=0; i<dwNum; i++)
	{
		m_ppIBCacheItemAllocated[i] = m_ppIBCacheItemAllocated[m_dwAllocatedIBCacheItemNum-i-1];
		m_ppIBCacheItemAllocated[m_dwAllocatedIBCacheItemNum-i-1] = NULL;
	}


	m_dwAllocatedIBCacheItemNum -= dwNum;
	m_dwReservedIBCacheItemNum += dwNum;
		
	for (i=0; i<m_dwAllocatedIBCacheItemNum; i++)
	{
		m_ppIBCacheItemAllocated[i]->dwIndexInAllocatedTable = i;
	}

}

BOOL CIBCache::GetIB(int** ppiCountVariable,DWORD dwIndicesNum,CIndexBuffer** ppIBAddrInOwner,void* pOwner)
{
	BOOL	bResult = FALSE;


lb_public_vb:
//	pIB = &m_IBPublic;
//	goto lb_return;

	if (dwIndicesNum > MAX_PRJMESH_INDICES_NUM)
		goto lb_return;

	IBCACHE_ITEM*	pIBItem;
	
	DWORD	dwAllocTryCount;
	dwAllocTryCount = 0;
lb_alloc:
	
	pIBItem = AllocIBCacheItem(ppiCountVariable,dwIndicesNum,ppIBAddrInOwner,pOwner);
	if (!pIBItem)
	{
//		pIB = &m_IBPublic;
//		*bNeedUpdate = TRUE;
//		goto lb_return;

		if (dwAllocTryCount > 0)
			goto lb_return;

		FreeIBCacheItem(m_dwFreeUnitNum);
		dwAllocTryCount++;
		goto lb_alloc;
	}
	m_dwTimeCount++;
	pIBItem->iCmp = m_dwTimeCount;


	bResult = TRUE;
lb_return:

	m_dwMissNum++;

	return bResult;
}
void CIBCache::Cleanup()
{
	
	FreeIBCacheItem(m_dwAllocatedIBCacheItemNum);

	for (DWORD i=0; i<m_dwMaxBufferNum; i++)
	{	
		m_pIBCachePool[i].IB.Cleanup();
	}

	if (m_ppIBCacheItemAllocated)
	{
		delete [] m_ppIBCacheItemAllocated;
		m_ppIBCacheItemAllocated = NULL;
	}
	

	if (m_pIBCachePool)
	{
		delete [] m_pIBCachePool;
		m_pIBCachePool = NULL;
	}
	if (m_ppIBCacheItemReserved)
	{
		delete [] m_ppIBCacheItemReserved;
		m_ppIBCacheItemReserved = NULL;
	}
	

	if (m_pIBHeap)
	{
		m_pRenderer->ReleaseResourcePool(m_pIBHeap);
		m_pIBHeap = NULL;
	}
//	m_IBPublic.Cleanup();
	
	ClearMember();
//	memset(this,0,sizeof(CIBCache));
	
}
CIBCache::~CIBCache()
{
	Cleanup();
	

}

void __stdcall QSortIntASC(IBCACHE_ITEM** ppArray,DWORD dwNum)
{
	int					pivot;
	DWORD				i,j,l,r;
	IBCACHE_ITEM*		temp;
	DWORD				stp;


	l = 0;
	r = dwNum-1;

	__asm
	{
		mov			dword ptr[stp],esp
		
		push		dword ptr[r]
		push		dword ptr[l]
		
	}
	
	
lb_loop:
	__asm 
	{
		pop			eax
		mov			dword ptr[l],eax
		pop			eax
		mov			dword ptr[r],eax
	}

	if (r-l+1 <= 1)
		goto lb_next_part;
		
	pivot = ppArray[r]->iCmp;
	i = l;
	j = r-1;

	while (1)
	{
		while (ppArray[i]->iCmp <= pivot)
		{
			if (i == r-1)	// 탈출 조건: i값이 축값까지 가버렸다면 소트가 되어있는 상태이다.그러므로 리턴
			{
				__asm
				{
					push		dword ptr[i]
					push		dword ptr[l]
					jmp			lb_loop	
				}
			}

			i++;
		}
lb_check_j:
		while (ppArray[j]->iCmp > pivot)
		{
			
			if (i >= j)
				goto lb_switch_pivot;
			j--;
		}
lb_switch:
		temp = ppArray[i];
		ppArray[i] = ppArray[j];
		ppArray[j] = temp;
	
	}	

lb_switch_pivot:
	temp = ppArray[i];
	ppArray[i] = ppArray[r];
	ppArray[r] = temp;




lb_exit:	

	__asm
	{
		push		dword ptr[r]			; push r
		mov			eax,dword ptr[i]
		inc			eax
		push		eax						; push i+1

		dec			eax
		jz			lb_skip_partition

		dec			eax
		push		eax						; push	i-1
		push		dword ptr[l]
lb_skip_partition:
	}

lb_next_part:

	__asm
	{
		cmp			esp,dword ptr[stp]
		jnz			lb_loop
	}

lb_return:

	return;

}
