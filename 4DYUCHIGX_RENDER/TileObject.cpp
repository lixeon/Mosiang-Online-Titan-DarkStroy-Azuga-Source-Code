#include "TileObject.h"

CTileObject::CTileObject()
{
	memset(this,0,sizeof(CTileObject));
}
BOOL CTileObject::Initialize(DWORD dwTilePosX,DWORD dwTilePosZ,HFIELD_DESC* pHFDesc)
{
	m_dwTilePosX = dwTilePosX;
	m_dwTilePosZ = dwTilePosZ;

	m_pTilePos = new INDEX_POS[pHFDesc->dwTileNumPerObjAxis*pHFDesc->dwTileNumPerObjAxis];

	return UpdateTileInfo(pHFDesc);

}


struct TILE_POS_LINK
{
	DWORD			dwX;
	DWORD			dwZ;
	TILE_POS_LINK*	pNext;
};

struct REF_TILE_INDEX
{
	WORD			wIntegratedTileIndex;
	TILE_POS_LINK*	pTilePos;
	DWORD			dwRefCount;
	
};
BOOL CTileObject::UpdateTileInfo(HFIELD_DESC* pHFDesc)
{


	if (m_pTileBufferDesc)
	{
		delete [] m_pTileBufferDesc;
		m_pTileBufferDesc = NULL;
	}

	pHFDesc->dwTileTextureNum &= 0x3fff;


	TILE_POS_LINK*	pTilePosEntry = new TILE_POS_LINK[pHFDesc->dwTileNumPerObjAxis*pHFDesc->dwTileNumPerObjAxis];
	memset(pTilePosEntry,0,sizeof(TILE_POS_LINK)*pHFDesc->dwTileNumPerObjAxis*pHFDesc->dwTileNumPerObjAxis);

	

	REF_TILE_INDEX*		pTileRefIndexTable = new REF_TILE_INDEX[pHFDesc->dwTileTextureNum*4];
	memset(pTileRefIndexTable,0,sizeof(REF_TILE_INDEX)*pHFDesc->dwTileTextureNum*4);

	DWORD	k,i,j,z,x;

	WORD	wTileIndex;
	WORD	wDirFlag;
	WORD	wIntegratedTileIndex;
	REF_TILE_INDEX*	pRefTileIndex;

	for (i=0; i<pHFDesc->dwTileTextureNum; i++)
	{
		for (k=0; k<4; k++)
		{
			pTileRefIndexTable[i*4+k].wIntegratedTileIndex = (WORD)( i | (k<<14) );
		}
	}


	DWORD	dwTilePosNum = 0;
	DWORD	dwTileBufferDescNum = 0;

	for (z=0; z<pHFDesc->dwTileNumPerObjAxis; z++)
	{
		for (x=0; x<pHFDesc->dwTileNumPerObjAxis; x++)
		{
		

			wIntegratedTileIndex = GetTile(x,z,pHFDesc->pwTileTable,pHFDesc->dwTileNumX);
			wDirFlag = (wIntegratedTileIndex & 0xc000)>>14;
			wTileIndex = wIntegratedTileIndex & 0x03fff;

			if (wTileIndex >= (WORD)pHFDesc->dwTileTextureNum)
			{
#ifdef _DEBUG
				__asm int 3
#endif
				wTileIndex = (WORD)pHFDesc->dwTileTextureNum-1;
			}


			pRefTileIndex = &pTileRefIndexTable[wTileIndex*4 + wDirFlag];

	
			if (pRefTileIndex->wIntegratedTileIndex != wIntegratedTileIndex)
				__asm int 3

			
			TILE_POS_LINK*	pCurTilePosLink = pTilePosEntry + dwTilePosNum;
			
			if (!pRefTileIndex->pTilePos)
			{
				
				pRefTileIndex->pTilePos = pCurTilePosLink;
				pRefTileIndex->wIntegratedTileIndex = wIntegratedTileIndex;
				dwTileBufferDescNum++;
			}
			else
			{
				pCurTilePosLink->pNext = pRefTileIndex->pTilePos;
				pRefTileIndex->pTilePos = pCurTilePosLink;
			}
			dwTilePosNum++;
			pCurTilePosLink->dwX = x;
			pCurTilePosLink->dwZ = z;
			pRefTileIndex->dwRefCount++;
		}
	}

	m_dwTileBufferDescNum = dwTileBufferDescNum;

	TILE_BUFFER_DESC*		pTileBufferDescTemp = new TILE_BUFFER_DESC[m_dwTileBufferDescNum];
	memset(pTileBufferDescTemp,0,sizeof(TILE_BUFFER_DESC)*m_dwTileBufferDescNum);

	dwTilePosNum = 0;
	dwTileBufferDescNum = 0;

	for (i=0; i<pHFDesc->dwTileTextureNum*4; i++)
	{
		pRefTileIndex = pTileRefIndexTable + i;

		if (pRefTileIndex->dwRefCount)
		{

			TILE_POS_LINK*	pCurTilePos;
			pCurTilePos = pRefTileIndex->pTilePos;

			pTileBufferDescTemp[dwTileBufferDescNum].pTilePos = m_pTilePos + dwTilePosNum;
			pTileBufferDescTemp[dwTileBufferDescNum].dwTileIndex = (DWORD)pRefTileIndex->wIntegratedTileIndex;
			pTileBufferDescTemp[dwTileBufferDescNum].dwTilePosNum = pRefTileIndex->dwRefCount;

			for (j=0; j<pTileBufferDescTemp[dwTileBufferDescNum].dwTilePosNum; j++)
			{
				pTileBufferDescTemp[dwTileBufferDescNum].pTilePos[j].dwX = pCurTilePos->dwX;
				pTileBufferDescTemp[dwTileBufferDescNum].pTilePos[j].dwY = pCurTilePos->dwZ;
				pCurTilePos = pCurTilePos->pNext;
				dwTilePosNum++;
			}
			dwTileBufferDescNum++;
		}
	}

	delete [] pTilePosEntry;
	pTilePosEntry = NULL;


	delete [] pTileRefIndexTable;
	pTileRefIndexTable = NULL;

	// 좌표별로 4가지경우로 분리하자.
	m_pTileBufferDesc = new TILE_BUFFER_DESC[m_dwTileBufferDescNum];
	memset(m_pTileBufferDesc,0,sizeof(TILE_BUFFER_DESC)*m_dwTileBufferDescNum);
	
	BOOL	sw;
	for (i=0; i<m_dwTileBufferDescNum; i++)
	{	
		DWORD	dwCount = 0;
		m_pTileBufferDesc[i].dwTileIndex = pTileBufferDescTemp[i].dwTileIndex;
		m_pTileBufferDesc[i].dwTilePosNum = pTileBufferDescTemp[i].dwTilePosNum;
		m_pTileBufferDesc[i].pTilePos = pTileBufferDescTemp[i].pTilePos;
		
		INDEX_POS*	pTilePosTemp = new INDEX_POS[pTileBufferDescTemp[i].dwTilePosNum];

		for (DWORD k=0; k<4; k++)
		{
			sw = FALSE;

			for (DWORD j=0; j<pTileBufferDescTemp[i].dwTilePosNum; j++)
			{
				DWORD dwCase = 
					(pTileBufferDescTemp[i].pTilePos[j].dwX & 0x00000001) |
					((pTileBufferDescTemp[i].pTilePos[j].dwY & 0x00000001)<<1);

				if (dwCase == k)
				{
					// 각각의 경우에 해당되면...
					if (!sw)
					{
						m_pTileBufferDesc[i].tileEntryDesc[k].dwTilePosEntry = dwCount;
						sw = TRUE;
					}
					m_pTileBufferDesc[i].tileEntryDesc[k].dwTilePosNum++;
					pTilePosTemp[dwCount] = pTileBufferDescTemp[i].pTilePos[j];
					dwCount++;

				}
			}	
		}

		memcpy(m_pTileBufferDesc[i].pTilePos,pTilePosTemp,sizeof(INDEX_POS)*dwCount);
		delete [] pTilePosTemp;
	}

	delete [] pTileBufferDescTemp;
	pTileBufferDescTemp = NULL;

	return TRUE;
}
WORD CTileObject::GetTile(DWORD dwPosXinObj,DWORD dwPosZinObj,WORD* pWorldTileTable,DWORD dwPitch)
{
	return *(pWorldTileTable + (m_dwTilePosX + dwPosXinObj) + (m_dwTilePosZ + dwPosZinObj)*dwPitch);
}
BOOL CTileObject::UpdateIndexBuffer(WORD* pIndex,HFIELD_DESC* pHFDesc)
{
	return TRUE;
}
CTileObject::~CTileObject()
{
	if (m_pTilePos)
	{
		delete [] m_pTilePos;
		m_pTilePos = NULL;
	}

	if (m_pTileBufferDesc)
	{
		delete [] m_pTileBufferDesc;
		m_pTileBufferDesc = NULL;
	}
}


/*
BOOL CTileObject::UpdateTileInfo(HFIELD_DESC* pHFDesc)
{
	// 일단은 선형검색..
	//TILE_BUFFER_DESC	tileBufferDesc[MAX_TILE_TEXTURE_NUM];
	//memset(&tileBufferDesc,0,sizeof(tileBufferDesc));
	TILE_BUFFER_DESC*	pTileBufferDescTemp = new TILE_BUFFER_DESC[MAX_TILE_TEXTURE_NUM];
	memset(pTileBufferDescTemp,0,sizeof(TILE_BUFFER_DESC)*MAX_TILE_TEXTURE_NUM);
	
	m_dwTileBufferDescNum = 0;
	
	DWORD			dwTilePosCount = 0;

	// 실제 타일 텍스쳐 개수를 14비트로 제한한다.
	// 가상의 타일텍스쳐 개수는 16비트개이다.
	pHFDesc->dwTileTextureNum &= 0x3fff;
	
	DWORD	k,i,z,x;
	DWORD	dwTileIndex;
	for (k=0; k<4; k++)
	{
		for (i=0; i<pHFDesc->dwTileTextureNum; i++)
		{
			DWORD			dwTilePosNum = 0;
			BOOL	bSwitch = FALSE;
			for (z=0; z<pHFDesc->dwTileNumPerObjAxis; z++)
			{
				for (x=0; x<pHFDesc->dwTileNumPerObjAxis; x++)
				{
					// 회전을 감안하여..4방향에 대한 분류가 필요하다.
					// 0도는 pHFDesc->dwTileTextureNum 부터 pHFDesc->dwTileTextureNum*1..
					// 90도는 pHFDesc->dwTileTextureNum 부터 pHFDesc->dwTileTextureNum*2..
					// 180도는 pHFDesc->dwTileTextureNum 부터 pHFDesc->dwTileTextureNum*3..
					// 270도는 pHFDesc->dwTileTextureNum 부터 pHFDesc->dwTileTextureNum*4..

					
					dwTileIndex = i |(k<<14);
				//	DWORD	dwResult = GetTile(x,z,pHFDesc->pwTileTable,pHFDesc->dwTileNumX);

					if (GetTile(x,z,pHFDesc->pwTileTable,pHFDesc->dwTileNumX) == dwTileIndex)
					{
						if (dwTileIndex & 0x0000c000)
							__asm nop




						if (!bSwitch)
						{
							pTileBufferDescTemp[m_dwTileBufferDescNum].dwTileIndex = dwTileIndex;
							pTileBufferDescTemp[m_dwTileBufferDescNum].dwTilePosNum = 0;
							pTileBufferDescTemp[m_dwTileBufferDescNum].pTilePos = m_pTilePos + dwTilePosCount;
							
							bSwitch = TRUE;
				
						}

						m_pTilePos[dwTilePosCount].dwX = x;
						m_pTilePos[dwTilePosCount].dwY = z;

						dwTilePosNum++;
						dwTilePosCount++;

					}
				}
			}

			if (dwTilePosNum)
			{
				pTileBufferDescTemp[m_dwTileBufferDescNum].dwTilePosNum = dwTilePosNum;
				m_dwTileBufferDescNum++;
			}
		}
	}
	if (m_pTileBufferDesc)
		delete [] m_pTileBufferDesc;
	
	m_pTileBufferDesc = new TILE_BUFFER_DESC[m_dwTileBufferDescNum];
	memset(m_pTileBufferDesc,0,sizeof(TILE_BUFFER_DESC)*m_dwTileBufferDescNum);

	// 좌표별로 4가지경우로 분리하자.
	
	BOOL	sw;
	for (i=0; i<m_dwTileBufferDescNum; i++)
	{	
		DWORD	dwCount = 0;
		m_pTileBufferDesc[i].dwTileIndex = pTileBufferDescTemp[i].dwTileIndex;
		m_pTileBufferDesc[i].dwTilePosNum = pTileBufferDescTemp[i].dwTilePosNum;
		m_pTileBufferDesc[i].pTilePos = pTileBufferDescTemp[i].pTilePos;
		
		INDEX_POS*	pTilePosTemp = new INDEX_POS[pTileBufferDescTemp[i].dwTilePosNum];

		for (DWORD k=0; k<4; k++)
		{
			sw = FALSE;

			for (DWORD j=0; j<pTileBufferDescTemp[i].dwTilePosNum; j++)
			{
				DWORD dwCase = 
					(pTileBufferDescTemp[i].pTilePos[j].dwX & 0x00000001) |
					((pTileBufferDescTemp[i].pTilePos[j].dwY & 0x00000001)<<1);

				if (dwCase == k)
				{
					// 각각의 경우에 해당되면...
					if (!sw)
					{
						m_pTileBufferDesc[i].tileEntryDesc[k].dwTilePosEntry = dwCount;
						sw = TRUE;
					}
					m_pTileBufferDesc[i].tileEntryDesc[k].dwTilePosNum++;
					pTilePosTemp[dwCount] = pTileBufferDescTemp[i].pTilePos[j];
					dwCount++;

				}
			}	
		}

		memcpy(m_pTileBufferDesc[i].pTilePos,pTilePosTemp,sizeof(INDEX_POS)*dwCount);
		delete [] pTilePosTemp;
	}

	if (pTileBufferDescTemp)
	{
		delete [] pTileBufferDescTemp;
		pTileBufferDescTemp = NULL;
	}

//	memcpy(m_pTileBufferDesc,tileBufferDesc,sizeof(TILE_BUFFER_DESC)*m_dwTileBufferDescNum);

	

	return TRUE;
}*/