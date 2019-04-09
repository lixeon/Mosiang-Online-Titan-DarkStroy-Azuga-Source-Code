#include "TileObject.h"
#include "HFieldTools.h"
#include "HFieldObject.h"
#include "HeightField.h"

void CTileObject::ClearMember()
{
	m_dwTilePosX = 0xffffffff;
	m_dwTilePosZ = 0xffffffff;
	m_pTileBufferDesc = NULL;
	m_dwTileBufferDescNum = 0;
	m_pTilePosPri = NULL;
	m_pTilePosExt = NULL;

	

	
}
CTileObject::CTileObject()
{
	ClearMember();
}
BOOL CTileObject::Initialize(DWORD dwTilePosX,DWORD dwTilePosZ,HFIELD_DESC* pHFDesc)
{
	m_dwTilePosX = dwTilePosX;
	m_dwTilePosZ = dwTilePosZ;

//	m_pTilePos = new INDEX_POS[pHFDesc->dwTileNumPerObjAxis*pHFDesc->dwTileNumPerObjAxis];

	return UpdateTileInfo(pHFDesc);

}




void CTileObject::CleanupTileBufferDesc()
{
	if (m_pTileBufferDesc)
	{
		delete [] m_pTileBufferDesc;
		m_pTileBufferDesc = NULL;
		m_dwTileBufferDescNum = 0;
	}
}
void CTileObject::CleanupAlphaMap()
{
	for (DWORD i=0; i<m_dwTileBufferDescNum; i++)
	{
		CleanupAlphaMapWithTileBufferDesc(&m_pTileBufferDesc[i]);
	}
}
void CTileObject::CleanupAlphaMapWithTileIndex(WORD wTileIndex)
{
	for (DWORD i=0; i<m_dwTileBufferDescNum; i++)
	{
		// NEED_TEST
		//if (m_pTileBufferDesc[i].dwTileIndex == dwTileIndex)
		if (!m_pTileBufferDesc[i].IsEqual(wTileIndex))
		{
			CleanupAlphaMapWithTileBufferDesc(&m_pTileBufferDesc[i]);
			return;
		}
	}
}
void CTileObject::CleanupAlphaMapWithTileBufferDesc(TILE_BUFFER_DESC* pTileBufferDesc)
{
	if (pTileBufferDesc->alphaMapDesc.pAlphaMapBits)
	{
		
		CHFieldObject* pHFieldObj = GetHFieldObject();
		pHFieldObj->GetHFieldControlObject()->CleanupAlphaMap();
		pHFieldObj->GetHField(this)->GetHFieldTools()->FreeAlphaMap(pTileBufferDesc->alphaMapDesc.pAlphaMapBits);
		pTileBufferDesc->alphaMapDesc.pAlphaMapBits = NULL;
		pTileBufferDesc->alphaMapDesc.wWidthHeight = 0;
	}
}
struct TILE_POS_LINK
{
	DWORD			dwX;
	DWORD			dwZ;
	TILE_POS_LINK*	pNext;
};

struct REF_TILE_INDEX
{
	WORD				wIntegratedTileIndex;
	TILE_POS_LINK*		pTilePos;
	DWORD				dwRefCount;
};

BOOL CTileObject::UpdateTileInfo(HFIELD_DESC* pHFDesc)
{


	CHFieldObject*	pHFieldObj;
	pHFieldObj = NULL;
	
	CHFieldTools* pAlphaMapManager;
	CoHeightField*	pHField;
	pHField = pHFieldObj->GetHField(this); 
	pAlphaMapManager = pHField->GetHFieldTools();
	pHFieldObj = GetHFieldObject();

	if (pAlphaMapManager)
		pAlphaMapManager->SaveAlphaMapTemporary(pHFieldObj,m_pTileBufferDesc,m_dwTileBufferDescNum);


	CleanupTileBufferDesc();

	pHFDesc->dwTileTextureNum &= 0x3fff;


	DWORD		dwMaxTilePosNum = pHFDesc->dwTileNumPerObjAxis*pHFDesc->dwTileNumPerObjAxis;
	TILE_POS_LINK*	pTilePosEntry = new TILE_POS_LINK[dwMaxTilePosNum];
	memset(pTilePosEntry,0,sizeof(TILE_POS_LINK)*dwMaxTilePosNum);


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
#ifdef _DEBUG
			if (wTileIndex >= (WORD)pHFDesc->dwTileTextureNum)
			{
				__asm int 3
				wTileIndex = (WORD)pHFDesc->dwTileTextureNum-1;
			}
#endif


			pRefTileIndex = &pTileRefIndexTable[wTileIndex*4 + wDirFlag];

#ifdef _DEBUG
			if (pRefTileIndex->wIntegratedTileIndex != wIntegratedTileIndex)
				__asm int 3
#endif
			
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

	if (m_pTilePosPri)
	{
		delete [] m_pTilePosPri;
	}
	m_pTilePosPri = new INDEX_POS[dwMaxTilePosNum];
	memset(m_pTilePosPri,0,sizeof(INDEX_POS)*dwMaxTilePosNum);
	//////////////////////////////////////////////////////////////////////////////////////


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

			pTileBufferDescTemp[dwTileBufferDescNum].pTilePosPri = m_pTilePosPri + dwTilePosNum;

			// 타일 디스크립터에 넣을때는 방향성분을 제거하면 안된다.회전불가.
//			pTileBufferDescTemp[dwTileBufferDescNum].dwTileIndex = (DWORD)(pRefTileIndex->wIntegratedTileIndex & 0x03fff);
			pTileBufferDescTemp[dwTileBufferDescNum].SetTileIndex(pRefTileIndex->wIntegratedTileIndex);
			


			for (j=0; j<pRefTileIndex->dwRefCount; j++)
			{
				pTileBufferDescTemp[dwTileBufferDescNum].pTilePosPri[j].dwX = pCurTilePos->dwX;
				pTileBufferDescTemp[dwTileBufferDescNum].pTilePosPri[j].dwY = pCurTilePos->dwZ;
				pCurTilePos = pCurTilePos->pNext;
				dwTilePosNum++;
			}
			pTileBufferDescTemp[dwTileBufferDescNum].dwTilePosNumPri = pRefTileIndex->dwRefCount;
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
		m_pTileBufferDesc[i].SetTileIndex(pTileBufferDescTemp[i].GetIntegratedTileIndex());
		m_pTileBufferDesc[i].dwTilePosNumPri = pTileBufferDescTemp[i].dwTilePosNumPri;
		m_pTileBufferDesc[i].pTilePosPri = pTileBufferDescTemp[i].pTilePosPri;
				
		INDEX_POS*	pTilePosTemp = new INDEX_POS[pTileBufferDescTemp[i].dwTilePosNumPri];

		for (DWORD k=0; k<4; k++)
		{
			sw = FALSE;

			for (DWORD j=0; j<pTileBufferDescTemp[i].dwTilePosNumPri; j++)
			{
				INDEX_POS*	pIndexPos;

				pIndexPos = &pTileBufferDescTemp[i].pTilePosPri[j];
				DWORD dwCase = (pIndexPos->dwX & 0x00000001) | ((pIndexPos->dwY & 0x00000001)<<1);

				if (dwCase == k)
				{
					// 각각의 경우에 해당되면...
					if (!sw)
					{
						m_pTileBufferDesc[i].tileEntryDescPri[k].wTilePosEntry = (WORD)dwCount;
						sw = TRUE;
					}
										
					m_pTileBufferDesc[i].tileEntryDescPri[k].wTilePosNum++;
					pTilePosTemp[dwCount] = pTileBufferDescTemp[i].pTilePosPri[j];
					dwCount++;

				}
			}	
		}

		memcpy(m_pTileBufferDesc[i].pTilePosPri,pTilePosTemp,sizeof(INDEX_POS)*dwCount);
		delete [] pTilePosTemp;
	}

	delete [] pTileBufferDescTemp;
	pTileBufferDescTemp = NULL;

	if (pAlphaMapManager)
	{
		if (pAlphaMapManager->LoadAlphaMapTemporary(pHFieldObj,m_pTileBufferDesc,m_dwTileBufferDescNum))
			UpdateExtTileInfo(pHFDesc,0xffff,FALSE);
	}
	return TRUE;
}

BOOL CTileObject::UpdateExtTileInfo(HFIELD_DESC* pHFDesc,WORD wTileIndex,BOOL bSpecifiedTileIndex)
{
	if (!pHFDesc->bBlendEnable)
		return FALSE;
	
	CHFieldObject*	pHFieldObj;
	pHFieldObj = NULL;
	
	CHFieldTools* pHFieldTools;
	CoHeightField*	pHField;
	pHField = pHFieldObj->GetHField(this); 
	pHFieldTools = pHField->GetHFieldTools();
	pHFieldObj = GetHFieldObject();

	DWORD		i,j,k;
	DWORD		dwWidthHeight;
	INDEX_POS*	pPoolEntry;
	INDEX_POS*	pIndexPosPool;
	INDEX_POS*	pIndexPosPoolPerTile;
	BYTE*		pbTableBoundary;
	BYTE*		pbTableInner;
	
	pHFieldTools->GetResource(&pIndexPosPool,&pIndexPosPoolPerTile,&pbTableBoundary,&pbTableInner,&dwWidthHeight);
	

	DWORD		dwPosNum;
	DWORD		dwPosResultNum;
	DWORD		dwTilePosExtNum = 0;

	int		iX,iY;

	BYTE*	pPos;
	BYTE	bMask;

	int		iOP[8][2] ={
								-1,-1, -1,0, -1,1,
								0,-1, 0,1 , 
								1,-1, 1,0, 1,1
		};
		
	pPoolEntry = pIndexPosPool;

	for (i=1; i<m_dwTileBufferDescNum; i++)
	{
		// 버퍼 초기화

		if (m_pTileBufferDesc[i].alphaMapDesc.pAlphaMapBits)
			goto lb_process;

		if (!bSpecifiedTileIndex)
			goto lb_process;
	
//		if (m_pTileBufferDesc[i].dwTileIndex != dwTileIndex)
		// NEED_TEST
		if (m_pTileBufferDesc[i].IsEqual(wTileIndex))
			goto lb_process;
		

		memcpy(pPoolEntry,m_pTileBufferDesc[i].pTilePosExt,sizeof(INDEX_POS)*m_pTileBufferDesc[i].dwTilePosNumExt);
		dwPosResultNum = m_pTileBufferDesc[i].dwTilePosNumExt;
		goto lb_common;

		/////////////////
lb_process:
		pHFieldTools->ClearBuffers();
		dwPosNum = 0;
		dwPosResultNum = 0;
		
		for (j=0; j<m_pTileBufferDesc[i].dwTilePosNumPri; j++)
		{
			iX = (int)m_pTileBufferDesc[i].pTilePosPri[j].dwX + 1;
			iY = (int)m_pTileBufferDesc[i].pTilePosPri[j].dwY + 1;

			pPos = &pbTableInner[iX + iY*dwWidthHeight];
			*pPos = 0;

			for (DWORD k=0; k<8; k++)
			{
				int iOffsetX = iX + iOP[k][0];
				int iOffsetY = iY + iOP[k][1];
				
				pPos = &pbTableBoundary[iOffsetX + iOffsetY*dwWidthHeight];
				bMask = *pPos;
				*pPos = 0;
				pIndexPosPoolPerTile[dwPosNum].dwX = iOffsetX;
				pIndexPosPoolPerTile[dwPosNum].dwY = iOffsetY;
				dwPosNum += bMask;
			}
		}
		for (j=0; j<dwPosNum; j++)
		{
			pPos = &pbTableInner[pIndexPosPoolPerTile[j].dwX + pIndexPosPoolPerTile[j].dwY*dwWidthHeight];
			bMask = *pPos;

			pPoolEntry[dwPosResultNum].dwX = pIndexPosPoolPerTile[j].dwX - 1;
			pPoolEntry[dwPosResultNum].dwY = pIndexPosPoolPerTile[j].dwY - 1;
			dwPosResultNum += bMask;
		}
lb_common:
		m_pTileBufferDesc[i].pTilePosExt = pPoolEntry;
		m_pTileBufferDesc[i].dwTilePosNumExt = dwPosResultNum;
		pPoolEntry += dwPosResultNum;
		dwTilePosExtNum += dwPosResultNum;

	}
	if (m_pTilePosExt)
	{
		delete [] m_pTilePosExt;

	}
	m_pTilePosExt = new INDEX_POS[dwTilePosExtNum];
	pPoolEntry = m_pTilePosExt;
	for (i=0; i<m_dwTileBufferDescNum; i++)
	{
		DWORD	dwCount = 0;
		BOOL	sw;
		// 4가지 유형으로 분리
		for (k=0; k<4; k++)
		{	
			m_pTileBufferDesc[i].tileEntryDescExt[k].wTilePosEntry = 0;
			m_pTileBufferDesc[i].tileEntryDescExt[k].wTilePosNum = 0;

			sw = FALSE;

			for (j=0; j<m_pTileBufferDesc[i].dwTilePosNumExt; j++)
			{
				INDEX_POS*	pIndexPos;

				pIndexPos = &m_pTileBufferDesc[i].pTilePosExt[j];
				DWORD dwCase = (pIndexPos->dwX & 0x00000001) | ((pIndexPos->dwY & 0x00000001)<<1);

				if (dwCase == k)
				{
					// 각각의 경우에 해당되면...
					if (!sw)
					{
						m_pTileBufferDesc[i].tileEntryDescExt[k].wTilePosEntry = (WORD)dwCount;
						sw = TRUE;
					}
					m_pTileBufferDesc[i].tileEntryDescExt[k].wTilePosNum++;
					pPoolEntry[dwCount] = *pIndexPos;
					dwCount++;
				}
			}	
		}
		m_pTileBufferDesc[i].pTilePosExt = pPoolEntry;
		pPoolEntry += m_pTileBufferDesc[i].dwTilePosNumExt;
	}
	return TRUE;
}


WORD CTileObject::GetTile(DWORD dwPosXinObj,DWORD dwPosZinObj,WORD* pWorldTileTable,DWORD dwPitch)
{
	return *(pWorldTileTable + (m_dwTilePosX + dwPosXinObj) + (m_dwTilePosZ + dwPosZinObj)*dwPitch);
}
void CTileObject::SetTile(DWORD dwPosXinObj,DWORD dwPosZinObj,WORD* pWorldTileTable,DWORD dwPitch,WORD wTileIndex)
{
	*(pWorldTileTable + (m_dwTilePosX + dwPosXinObj) + (m_dwTilePosZ + dwPosZinObj)*dwPitch) = wTileIndex;
}


TILE_BUFFER_DESC* CTileObject::SetAlphaTexel(CHFieldTools* pAlphaMapManager,CHFieldObject* pHFieldObject,WORD wTileTexIndex,DWORD dwActionFlag,DWORD dwCenterX,DWORD dwCenterZ,DWORD dwRS,DWORD dwTexWidthHeight)
{
	TILE_BUFFER_DESC*	pTileBufferDesc = NULL;
	DWORD	i;
	for (i=0; i<m_dwTileBufferDescNum; i++)
	{
		// NEED_TEST
//		if (m_pTileBufferDesc[i].dwTileIndex == dwTileTexIndex)
		if (!m_pTileBufferDesc[i].IsEqual(wTileTexIndex))
		{
			pTileBufferDesc = &m_pTileBufferDesc[i];
			goto lb_set;
		}
	}
	goto lb_return;
lb_set:

	CHFieldObject*	pHFieldObj;
	pHFieldObj = NULL;
	
	CHFieldTools* pTemp;
	CoHeightField*	pHField;
	pHField = pHFieldObj->GetHField(this); 
	pTemp = pHField->GetHFieldTools();

	if (!pTileBufferDesc->alphaMapDesc.pAlphaMapBits)
	{
		pTileBufferDesc->alphaMapDesc.pAlphaMapBits = pAlphaMapManager->AllocAlphaMap();
		pTileBufferDesc->alphaMapDesc.wWidthHeight = (WORD)dwTexWidthHeight;
	}
	
	IMAGE_BRUSH*	pBrush;
	DWORD			dwBright;
	dwBright = (dwActionFlag & EDIT_ALPHA_TEXEL_BRIGHT_MASK);
	pBrush = pAlphaMapManager->GetImageBrush(dwBright);

	int		iStartX,iStartZ,iRs;
	
	iRs = (pBrush->wWidth / 2);

	iStartX = (int)dwCenterX - iRs;
	iStartZ = (int)dwCenterZ - iRs;

	

	int	brushx,brushz;

	DWORD	dw2BitsColor;
	for (i=0; i<pBrush->dwPixelsNum; i++)
	{
		brushx = (int)(iStartX + (int)pBrush->pIndexedBits[i].bX);
		brushz = (int)(iStartZ + (int)pBrush->pIndexedBits[i].bY);

		if (brushx < 0 )
			brushx = 0;

		if (brushz < 0 )
			brushz = 0;

		if (brushx > (int)(dwTexWidthHeight-1))
			continue;

		if (brushz > (int)(dwTexWidthHeight-1))
			continue;

		dw2BitsColor = (pBrush->pIndexedBits[i].bColor >> 6);
		if (dwActionFlag & EDIT_ALPHA_TEXEL_OP_SUB)
		{
			SubPixel2BitImage(
				pTileBufferDesc->alphaMapDesc.pAlphaMapBits,
				brushx,
				brushz,
				pTileBufferDesc->alphaMapDesc.wWidthHeight,
				pTileBufferDesc->alphaMapDesc.wWidthHeight,
				dw2BitsColor);
		}
		else
		{
			
			AddPixel2BitImage(
				pTileBufferDesc->alphaMapDesc.pAlphaMapBits,
				brushx,
				brushz,
				pTileBufferDesc->alphaMapDesc.wWidthHeight,
				pTileBufferDesc->alphaMapDesc.wWidthHeight,
				dw2BitsColor);
		}
	}
	if (!pTileBufferDesc->dwTilePosNumExt)
	{
		HFIELD_DESC	hfDesc;
		pHField->GetHFieldDesc(&hfDesc);
		UpdateExtTileInfo(&hfDesc,pTileBufferDesc->GetTileIndex(),TRUE);
	}

lb_return:
	return pTileBufferDesc;
}
WORD CTileObject::GetExtTileTop(DWORD dwTilePosX,DWORD dwTilePosZ)
{
	WORD	wTileIndex = 0xffff;
	int		iRelativeTilePosX = (int)dwTilePosX - (int)m_dwTilePosX;
	int		iRelativeTilePosZ = (int)dwTilePosZ - (int)m_dwTilePosZ;

	if ( iRelativeTilePosX < 0 || iRelativeTilePosZ < 0)
		goto lb_return;

	int i,j;
	for (i=m_dwTileBufferDescNum-1; i>=0; i--)
	{
		for (j=0; j<m_pTileBufferDesc[i].dwTilePosNumPri; j++)
		{
			if (iRelativeTilePosX == m_pTileBufferDesc[i].pTilePosPri[j].dwX && iRelativeTilePosZ == m_pTileBufferDesc[i].pTilePosPri[j].dwY)
			{
				wTileIndex = m_pTileBufferDesc[i].GetTileIndex();
				goto lb_return;
			}

		}
		for (j=0; j<m_pTileBufferDesc[i].dwTilePosNumExt; j++)
		{
			if (iRelativeTilePosX == m_pTileBufferDesc[i].pTilePosExt[j].dwX && iRelativeTilePosZ == m_pTileBufferDesc[i].pTilePosExt[j].dwY)
			{
				wTileIndex = m_pTileBufferDesc[i].GetTileIndex();
				goto lb_return;
			}
		}
	}
lb_return:
	return wTileIndex;
}
	
CHFieldObject* CTileObject::GetHFieldObject()
{
	CHFieldObject*	pHFieldObj = NULL;
	DWORD	dwOffset = (DWORD)(&pHFieldObj->m_TileObject);

	pHFieldObj = (CHFieldObject*)((DWORD)this - dwOffset);

	return pHFieldObj;
}
void CTileObject::SetAlphaMap(WORD wTileIndex,char* p2BitsImg,DWORD dwWidthHeight)
{

	for (DWORD i=0; i<m_dwTileBufferDescNum; i++)
	{
	// NEED_TEST
	//	if (m_pTileBufferDesc[i].dwTileIndex == dwTileIndex)
		if (!m_pTileBufferDesc[i].IsEqual(wTileIndex))
		{
#ifdef _DEBUG
			if (m_pTileBufferDesc[i].alphaMapDesc.pAlphaMapBits)
				__asm int 3
#endif
			m_pTileBufferDesc[i].alphaMapDesc.pAlphaMapBits = p2BitsImg;
			m_pTileBufferDesc[i].alphaMapDesc.wWidthHeight = (WORD)dwWidthHeight;
		}
	}
}

void CTileObject::GetAlphaMap(char** pp2BitsImg,DWORD* pdwWidthHeight,WORD wTileIndex)
{
	for (DWORD i=0; i<m_dwTileBufferDescNum; i++)
	{
		// NEED_TEST
		//if (m_pTileBufferDesc[i].dwTileIndex == dwTileIndex)
		if (!m_pTileBufferDesc[i].IsEqual(wTileIndex))
		{
			
			*pp2BitsImg = m_pTileBufferDesc[i].alphaMapDesc.pAlphaMapBits;
			*pdwWidthHeight = (DWORD)m_pTileBufferDesc[i].alphaMapDesc.wWidthHeight;
		}
	}
}

DWORD CTileObject::WriteAlphaMap(char* pMemDest,DWORD dwObjIndex,DWORD dwImgSize)
{
	DWORD	dwSize = 0;

	for (DWORD i=0; i<m_dwTileBufferDescNum; i++)
	{
		if (m_pTileBufferDesc[i].alphaMapDesc.pAlphaMapBits)
		{
#ifdef _DEBUG
			if(	dwImgSize != m_pTileBufferDesc[i].alphaMapDesc.wWidthHeight*m_pTileBufferDesc[i].alphaMapDesc.wWidthHeight / 4)
				__asm int 3
#endif			
			((ALPHA_MAP_FILE_HEADER*)pMemDest)->dwObjIndex = dwObjIndex;
			((ALPHA_MAP_FILE_HEADER*)pMemDest)->dwTileIndex = (DWORD)m_pTileBufferDesc[i].GetTileIndex();
			memcpy(((ALPHA_MAP_FILE_HEADER*)pMemDest)->p2BitsImg,m_pTileBufferDesc[i].alphaMapDesc.pAlphaMapBits,dwImgSize);
			dwSize += (dwImgSize + ALPHA_MAP_FILE_HEADER_SIZE);
			pMemDest += (dwImgSize + ALPHA_MAP_FILE_HEADER_SIZE);
		}
	}
	return dwSize;
}
void CTileObject::Cleanup()
{
	if (m_pTilePosPri)
	{
		delete [] m_pTilePosPri;
	}
	if (m_pTilePosExt)
	{
		delete [] m_pTilePosExt;
	}

	CleanupTileBufferDesc();
	CleanupAlphaMap();
	ClearMember();
}
CTileObject::~CTileObject()
{
	Cleanup();
}
