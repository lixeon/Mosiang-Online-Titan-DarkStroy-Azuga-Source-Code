
#include "VBCache.h"
#include "CoD3DDevice.h"
#include "global_variable.h"
#include "VBHeap.h"

void __stdcall QSortIntASC(VBCACHE_ITEM** ppArray,DWORD dwNum);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void CVBCache::ClearMember()
{
	m_pRenderer = NULL;
		
	m_ppVBCacheItemAllocated = 0;


	
	m_pVBCachePool = NULL;
	m_pVBHeap = NULL;
	m_dwMaxBufferNum = 0;

	m_ppVBCacheItemReserved = NULL;
	m_dwAllocatedVBCacheItemNum = 0;
	m_dwReservedVBCacheItemNum = 0;

	m_dwFreeUnitNum = 0;
	m_dwRequestNum = 0;
	m_dwHitNum = 0;
	m_dwTimeCount = 0;
	m_fFreeVBCacheRate = 0.072f;//DEFAULT_FREE_VBCACHE_RATE;


}
CVBCache::CVBCache()
{
	ClearMember();
}


DWORD CVBCache::GetCacheHitRate()
{
	float fPercentage;

	if (!m_dwRequestNum)
	{
		fPercentage = 0;
		goto lb_return;
	}

	fPercentage = ((float)m_dwHitNum / (float)m_dwRequestNum) * 100.0f;

	m_dwHitNum = 0;
	m_dwRequestNum = 0;

lb_return:
	return (DWORD)fPercentage;
}
void CVBCache::SetFreeVBCacheRate(float fVal)
{

	m_fFreeVBCacheRate = fVal;
	m_dwFreeUnitNum = (DWORD)( (float)m_dwMaxBufferNum * fVal);

	if (m_dwFreeUnitNum < 1)
		m_dwFreeUnitNum = 1;
}
BOOL CVBCache::Initialize(CoD3DDevice* pRenderer,DWORD dwMaxBufferNum,DWORD dwMaxVerticesNumPerVertexBuffer,DWORD dwMaxVertexBufferNum)
{
	
	m_pRenderer = pRenderer;
	m_dwMaxBufferNum = dwMaxBufferNum;

	
	SetFreeVBCacheRate(m_fFreeVBCacheRate);

	m_pVBCachePool = new VBCACHE_ITEM[m_dwMaxBufferNum];
	memset(m_pVBCachePool,0,sizeof(VBCACHE_ITEM)*m_dwMaxBufferNum);

	m_ppVBCacheItemReserved = new VBCACHE_ITEM*[m_dwMaxBufferNum];
	memset(m_ppVBCacheItemReserved,0,sizeof(VBCACHE_ITEM*)*m_dwMaxBufferNum);

	for (DWORD i=0; i<m_dwMaxBufferNum; i++)
	{
		m_ppVBCacheItemReserved[i] = m_pVBCachePool+i;

	}
	m_ppVBCacheItemAllocated = new VBCACHE_ITEM*[m_dwMaxBufferNum];
	memset(m_ppVBCacheItemAllocated,0,sizeof(VBCACHE_ITEM*)*m_dwMaxBufferNum);

	m_pRenderer->CreateResourcePool((IResourcePool**)&m_pVBHeap,RESORUCE_POOL_TYPE_VBHEAP);

	m_pVBHeap->Initialize(pRenderer->GetD3DResourceManager(),D3DFVF_VLVERTEX,D3DUSAGE_WRITEONLY,D3DPOOL_DEFAULT,sizeof(D3DVLVERTEX),dwMaxVerticesNumPerVertexBuffer,dwMaxVertexBufferNum,0,NOT_USE_AUTO_SCHEDLUE);




	m_HashSearch.Initialize(m_dwMaxBufferNum,sizeof(VBCACHE_KEY)/sizeof(DWORD));
	m_dwReservedVBCacheItemNum = m_dwMaxBufferNum;
	
	return TRUE;
}



VBCACHE_ITEM* CVBCache::AllocVBCacheItem(VBCACHE_KEY* pKey,DWORD dwVerticesNum)
{
	VBCACHE_ITEM*	pVBItem = NULL;

	if (!m_dwReservedVBCacheItemNum)
		goto lb_return;


	pVBItem = m_ppVBCacheItemReserved[0];
	DWORD	dwErrorCode;
	if (!m_pVBHeap->AllocVB(&dwErrorCode,&pVBItem->VB,dwVerticesNum,NULL,&pVBItem->VB.m_bEnable,NULL))
	{
		pVBItem = NULL;
		goto lb_return;
	}

	m_ppVBCacheItemReserved[0] = m_ppVBCacheItemReserved[m_dwReservedVBCacheItemNum-1];
	m_ppVBCacheItemReserved[m_dwReservedVBCacheItemNum-1] = NULL;
	m_ppVBCacheItemAllocated[m_dwAllocatedVBCacheItemNum] = pVBItem;

	pVBItem->dwIndexInAllocatedTable = m_dwAllocatedVBCacheItemNum;
	pVBItem->key = *pKey;
	pVBItem->iCmp = 0;

#ifdef _DEBUG
	if (!pVBItem->VB.GetVerticesNum())
	{
		char	txt[512];
		wsprintf(txt,"Fail to CVBCache::AllocVBCacheItem() !pVBItem->VB.GetVerticesNum(), File : %s , Line : %d \n",__FILE__,__LINE__);
		DWORD	dwAddr;
		GetEIP(&dwAddr);
		g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
	}
#endif

	pVBItem->pSearchHandle = m_HashSearch.InsertItem((DWORD*)&pVBItem->key,sizeof(VBCACHE_KEY)/sizeof(DWORD),pVBItem);


	m_dwAllocatedVBCacheItemNum++;
	m_dwReservedVBCacheItemNum--;
lb_return:
	return pVBItem;
}

void CVBCache::FreeVBCacheItem(DWORD dwNum)
{
	DWORD	i;

	VBCACHE_ITEM*	pDelItem;


	if (dwNum > m_dwAllocatedVBCacheItemNum)
		dwNum = m_dwAllocatedVBCacheItemNum;
	


	QSortIntASC(m_ppVBCacheItemAllocated,m_dwAllocatedVBCacheItemNum);

	for (i=0; i<dwNum; i++)
	{
		pDelItem = m_ppVBCacheItemAllocated[i];
#ifdef _DEBUG
		if (!pDelItem->pSearchHandle || (DWORD)pDelItem->pSearchHandle == 0xcccccccc || (DWORD)pDelItem->pSearchHandle == 0xdddddddd)
		{
			char	txt[512];
			memset(txt,0,512);
			wsprintf(txt,"Fail toCVBCache::FreeVBCacheItem() !pDelItem->pSearchHandle, File : %s , Line : %d \n",__FILE__,__LINE__);
			DWORD	dwAddr;
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
			
		}
#endif

		
		m_HashSearch.DeleteItem(pDelItem->pSearchHandle);
		
#ifdef _DEBUG
		pDelItem->dwIndexInAllocatedTable = 0xcccccccc;
		pDelItem->pSearchHandle = (ITEM_CONTAINER*)0xcccccccc;
		pDelItem->pMem = (BYTE*)0xcccccccc;
#else
		pDelItem->dwIndexInAllocatedTable = NULL;
		pDelItem->pSearchHandle = NULL;
		pDelItem->pMem = NULL;
#endif
		pDelItem->VB.Cleanup();

		m_ppVBCacheItemReserved[m_dwReservedVBCacheItemNum+i] = pDelItem;
	}
	for (i=0; i<dwNum; i++)
	{
		m_ppVBCacheItemAllocated[i] = m_ppVBCacheItemAllocated[m_dwAllocatedVBCacheItemNum-1-i];
		m_ppVBCacheItemAllocated[m_dwAllocatedVBCacheItemNum-1-i] = NULL;
	}
	m_dwAllocatedVBCacheItemNum -= dwNum;
	m_dwReservedVBCacheItemNum += dwNum;
	
	for (i=0; i<m_dwAllocatedVBCacheItemNum; i++)
	{
		m_ppVBCacheItemAllocated[i]->iCmp = 0;
	}

}

DWORD CVBCache::ClearCacheWithIDIMeshObject(void* pObject)
{
	DWORD	i=0;

	VBCACHE_ITEM*	pDelItem;
	DWORD			dwNum = 0;

lb_search:
	if (i >= m_dwAllocatedVBCacheItemNum)
		goto lb_return;	
	
	while (m_ppVBCacheItemAllocated[i]->key.pObject == pObject)
	{
		pDelItem = m_ppVBCacheItemAllocated[i];
		m_ppVBCacheItemAllocated[i] = m_ppVBCacheItemAllocated[m_dwAllocatedVBCacheItemNum-1];
		m_ppVBCacheItemAllocated[m_dwAllocatedVBCacheItemNum-1] = NULL;

		
#ifdef _DEBUG
		if (!pDelItem->pSearchHandle || (DWORD)pDelItem->pSearchHandle == 0xcccccccc || (DWORD)pDelItem->pSearchHandle == 0xdddddddd)
		{
			char	txt[512];
			memset(txt,0,512);
			wsprintf(txt,"Fail toCVBCache::ClearCacheWithIDIMeshObject() !pDelItem->pSearchHandle, File : %s , Line : %d \n",__FILE__,__LINE__);
			DWORD	dwAddr;
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
			
		}
#endif
		m_HashSearch.DeleteItem(pDelItem->pSearchHandle);

	
#ifdef _DEBUG
		pDelItem->dwIndexInAllocatedTable = 0xcccccccc;
		pDelItem->pSearchHandle = (ITEM_CONTAINER*)0xcccccccc;
		pDelItem->pMem = (BYTE*)0xcccccccc;
#else
		pDelItem->dwIndexInAllocatedTable = NULL;
		pDelItem->pSearchHandle = NULL;
		pDelItem->pMem = NULL;
#endif
		pDelItem->VB.Cleanup();

		m_ppVBCacheItemReserved[m_dwReservedVBCacheItemNum] = pDelItem;

		m_dwAllocatedVBCacheItemNum--;
		m_dwReservedVBCacheItemNum++;

		dwNum++;

		if (!m_ppVBCacheItemAllocated[i])
			goto lb_return;
	}
	i++;
	goto lb_search;

lb_return:	
	

#ifdef _DEBUG
	char	txt[1024];
	memset(txt,0,1024);

	DWORD	dwAddr;

	for (i=0; i<m_dwAllocatedVBCacheItemNum; i++)
	{
		if (!m_ppVBCacheItemAllocated[i])
		{
			wsprintf(txt,"Fail toCVBCache::ClearCacheWithIDIMeshObject() !m_ppVBCacheItemAllocated[i], File : %s , Line : %d \n",__FILE__,__LINE__);
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
		}
	}
	for (i=0; i<m_dwReservedVBCacheItemNum; i++)
	{
		if (!m_ppVBCacheItemReserved[i])
		{
			wsprintf(txt,"Fail toCVBCache::ClearCacheWithIDIMeshObject() !m_ppVBCacheItemReserved[i], File : %s , Line : %d \n",__FILE__,__LINE__);
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
		}
	}
	for (i=0; i<m_dwAllocatedVBCacheItemNum; i++)
	{
		if (m_ppVBCacheItemAllocated[i]->key.pObject == pObject)
		{
			wsprintf(txt,"Fail toCVBCache::ClearCacheWithIDIMeshObject() m_ppVBCacheItemAllocated[i]->key.pObject == pObject, File : %s , Line : %d \n",__FILE__,__LINE__);
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
		}
	}

#endif

	return dwNum;
}
DWORD CVBCache::ClearCacheWithMotionUID(void* pMotionUID)
{
	DWORD	i=0;

	VBCACHE_ITEM*	pDelItem;
	DWORD			dwNum = 0;

lb_search:
	if (i >= m_dwAllocatedVBCacheItemNum)
		goto lb_return;	
	
	while (m_ppVBCacheItemAllocated[i]->key.pMotionUID == pMotionUID)
	{
		pDelItem = m_ppVBCacheItemAllocated[i];
		m_ppVBCacheItemAllocated[i] = m_ppVBCacheItemAllocated[m_dwAllocatedVBCacheItemNum-1];
		m_ppVBCacheItemAllocated[m_dwAllocatedVBCacheItemNum-1] = NULL;

		
#ifdef _DEBUG
		if (!pDelItem->pSearchHandle || (DWORD)pDelItem->pSearchHandle == 0xcccccccc || (DWORD)pDelItem->pSearchHandle == 0xdddddddd)
		{
			char	txt[512];
			memset(txt,0,512);
			wsprintf(txt,"Fail toCVBCache::ClearCacheWithMotionUID() !pDelItem->pSearchHandle, File : %s , Line : %d \n",__FILE__,__LINE__);
			DWORD	dwAddr;
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
			
		}
#endif
		m_HashSearch.DeleteItem(pDelItem->pSearchHandle);

	
#ifdef _DEBUG
		pDelItem->dwIndexInAllocatedTable = 0xcccccccc;
		pDelItem->pSearchHandle = (ITEM_CONTAINER*)0xcccccccc;
		pDelItem->pMem = (BYTE*)0xcccccccc;
#else
		pDelItem->dwIndexInAllocatedTable = NULL;
		pDelItem->pSearchHandle = NULL;
		pDelItem->pMem = NULL;
#endif
		pDelItem->VB.Cleanup();

		m_ppVBCacheItemReserved[m_dwReservedVBCacheItemNum] = pDelItem;

		m_dwAllocatedVBCacheItemNum--;
		m_dwReservedVBCacheItemNum++;

		dwNum++;

		if (!m_ppVBCacheItemAllocated[i])
			goto lb_return;
	}
	i++;
	goto lb_search;

lb_return:	
	

#ifdef _DEBUG
	char	txt[1024];
	memset(txt,0,1024);

	DWORD	dwAddr;

	for (i=0; i<m_dwAllocatedVBCacheItemNum; i++)
	{
		if (!m_ppVBCacheItemAllocated[i])
		{
			wsprintf(txt,"Fail toCVBCache::ClearCacheWithMotionUID() !m_ppVBCacheItemAllocated[i], File : %s , Line : %d \n",__FILE__,__LINE__);
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
		}
	}
	for (i=0; i<m_dwReservedVBCacheItemNum; i++)
	{
		if (!m_ppVBCacheItemReserved[i])
		{
			wsprintf(txt,"Fail toCVBCache::ClearCacheWithMotionUID() !m_ppVBCacheItemReserved[i], File : %s , Line : %d \n",__FILE__,__LINE__);
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
		}
	}
	for (i=0; i<m_dwAllocatedVBCacheItemNum; i++)
	{
		if (m_ppVBCacheItemAllocated[i]->key.pMotionUID == pMotionUID)
		{
			wsprintf(txt,"Fail toCVBCache::ClearCacheWithMotionUID() m_ppVBCacheItemAllocated[i]->key.pMotionUID == pMotionUID, File : %s , Line : %d \n",__FILE__,__LINE__);
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
		}
	}

#endif

	return dwNum;
}
CVertexBuffer* CVBCache::GetVB(BOOL* bNeedUpdate,VBCACHE_KEY* pKey,DWORD dwRefIndex,DWORD dwVertexNum)
{

	*bNeedUpdate = TRUE;
	CVertexBuffer* pVB = NULL;
	VBCACHE_ITEM*	pVBItem;

	

lb_public_vb:

	if (dwVertexNum > MAX_PHYSIQUE_VERTEX_NUM)
		goto lb_return;
	
	pVBItem = (VBCACHE_ITEM*)m_HashSearch.SearchItem((DWORD*)pKey,sizeof(VBCACHE_KEY)/sizeof(DWORD));
//	pVBItem = NULL;

	if (pVBItem)
	{
#ifdef _DEBUG
		if (dwVertexNum != pVBItem->VB.GetVerticesNum())
		{
			char	txt[512];
			memset(txt,0,512);
			wsprintf(txt,"Fail to CVBCache::GetVB() dwVertexNum != pVBItem->VB.GetVerticesNum(), File : %s , Line : %d \n",__FILE__,__LINE__);
			DWORD	dwAddr;
			GetEIP(&dwAddr);
			g_pErrorHandleFunc(ERROR_TYPE_ENGINE_CODE,0,(void*)dwAddr,txt);
		}
#endif

		// 맞는 버퍼를 찾았다.
		pVB = &pVBItem->VB;
		*bNeedUpdate = FALSE;
		m_dwHitNum++;
		goto lb_return;
		
	}

lb_alloc:
	pVBItem = AllocVBCacheItem(pKey,dwVertexNum);
	if (!pVBItem)
	{
		FreeVBCacheItem(m_dwFreeUnitNum);
//		m_dwHitNum = 0;
//		m_dwRequestNum = 0;
		goto lb_alloc;
//lb_public_vb:
//		pVB = &m_VBPublic;
//		goto lb_return;
	}



	pVB = &pVBItem->VB;
	pVBItem->iCmp++;
	


lb_return:
//	pVB = &m_VBPublic;
	m_dwRequestNum++;

	return pVB;
}
void CVBCache::Cleanup()
{
	
	for (DWORD i=0; i<m_dwMaxBufferNum; i++)
	{	
		m_pVBCachePool[i].VB.Cleanup();
	}

	if (m_ppVBCacheItemAllocated)
	{
		delete [] m_ppVBCacheItemAllocated;
		m_ppVBCacheItemAllocated = NULL;
	}

	if (m_pVBCachePool)
	{
		delete [] m_pVBCachePool;
		m_pVBCachePool = NULL;
	}
	if (m_ppVBCacheItemReserved)
	{
		delete [] m_ppVBCacheItemReserved;
		m_ppVBCacheItemReserved = NULL;
	}
	
	if (m_pVBHeap)
	{
		m_pRenderer->ReleaseResourcePool(m_pVBHeap);
		m_pVBHeap = NULL;
	}
	m_HashSearch.Cleanup();
	ClearMember();
//	memset(this,0,sizeof(CVBCache));
	
}
CVBCache::~CVBCache()
{
	Cleanup();
	

}

void __stdcall QSortIntASC(VBCACHE_ITEM** ppArray,DWORD dwNum)
{
	int					pivot;
	DWORD				i,j,l,r;
	VBCACHE_ITEM*		temp;
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
