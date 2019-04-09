#include "HFieldObject.h"
#include "HFieldManager.h"
#include "d3d_helper.h"
#include "CoD3DDevice.h"
#include "../4DyuchiGRX_myself97/My2DLib.h"

#include "../4DyuchiGXGFunc/global.h"

#include "StaticVBPool.h"
#include "IBHeap.h"


STDMETHODIMP CHFieldManager::QueryInterface(REFIID refiid, PPVOID ppv)
{
	*ppv = NULL;
	return E_NOINTERFACE;
}
STDMETHODIMP_(ULONG) CHFieldManager::AddRef(void)
{
	m_dwRefCount++;
	return m_dwRefCount;
}
STDMETHODIMP_(ULONG) CHFieldManager::Release(void)
{
	m_dwRefCount--;
	if (!m_dwRefCount)
		delete this;

	return 0;
}

CHFieldManager::CHFieldManager(CoD3DDevice* pDevice)
{
//	memset((char*)this+4,0,sizeof(CHFieldManager)-4);

	
	memset(&m_DetailLevelIndicesDesc,0,sizeof(m_DetailLevelIndicesDesc));

	m_dwRefCount = 0;
	m_dwMipmapLevelNum = 0;
	memset(&m_hfDesc,0,sizeof(HFIELD_DESC));

	m_dwVerticesNumPerObj = 0;
	m_dwLv0TriNumPerObj = 0;
	m_dwLv0IndicesNumUnit = 0;
	m_dwLv0IndicesPitch = 0;
	
	memset(m_IndexTable,0,sizeof(INDEX_BUFFER_DESC)*MAX_HFIELD_DETAIL_NUM*16);
	m_pStaticVBPool = NULL;			
	m_pIBHeap = NULL;			
	m_pTileTexture = NULL;
	m_dwIndexBufferNum = 0;

	m_dwMaxIndicesNum = 0;
	m_dwUnitIndicesNum = 0;
	m_dwMaxViewObjNum= 0;


	m_pRenderer = pDevice;
	m_pD3DDevice = pDevice->GetDevice();
	m_pTextureManager = pDevice->GetTextureManager();
	m_bHFieldTileBlendEnable = FALSE;


	m_ppHFieldObjectList = NULL;
	m_dwMaxHFieldObjNum = 0;
	m_dwHFieldObjNum = 0;



	m_linkInRenderer.pItem = this;
	
	m_dwHFieldObjNum = 0;
	m_dwMaxHFieldObjNum = 0;


	m_pRenderer->LinkHFieldManager(this);
}
void CHFieldManager::SetIndexedTexture(DWORD dwIndex,DWORD dwTexStageIndex,IMAGE_HEADER** ppImgHeader)
{

	if (m_pRenderer->GetLightMapFlag() & LIGHTMAP_FLAG_DISABLE_TEXTURE)
	{
		m_pRenderer->GetDefaultMaterial()->TextureDiffuse.SetTexture(m_pD3DDevice,dwTexStageIndex);
		*ppImgHeader = NULL;
	}
	else
	{
		m_pTileTexture[dwIndex].SetTexture(m_pD3DDevice,dwTexStageIndex);
		*ppImgHeader = m_pTileTexture[dwIndex].GetImageHeader();
	}

}
BOOL __stdcall CHFieldManager::StartInitialize(HFIELD_DESC* pDesc)
{
	BOOL	bResult = TRUE;
	memcpy(&m_hfDesc,pDesc,sizeof(m_hfDesc));

	m_dwLv0TriNumPerObj = pDesc->dwTriNumPerObj;
	m_dwLv0IndicesNumUnit = pDesc->dwFacesNumPerTileAxis*pDesc->dwFacesNumPerTileAxis*2*3;
	m_dwLv0IndicesPitch = pDesc->dwFacesNumPerTileAxis*pDesc->dwFacesNumPerTileAxis*pDesc->dwTileNumPerObjAxis*2*3;
	
	if (!LoadTilePalette(pDesc->pTexTable,pDesc->dwTileTextureNum))
		goto lb_return;

	m_pRenderer->CreateResourcePool((IResourcePool**)&m_pIBHeap,RESORUCE_POOL_TYPE_IBHEAP);
	
//	m_pIBHeap->m_D3DRCBufferHeap;

	DWORD	dwMaxViewObjNum;
	DWORD	dwUnitFreeObjNum;
	float	fLengthPerObj;

	DWORD	dwRadius;
	CAMERA_DESC*	pCameraDesc;
	pCameraDesc = m_pRenderer->INL_GetCameraDesc();
	fLengthPerObj = (float)(m_hfDesc.dwFacesNumPerObjAxis * m_hfDesc.fFaceSize);
	dwRadius = (DWORD)( pCameraDesc->fFar / fLengthPerObj ) + 1;
	

	dwMaxViewObjNum = dwRadius*dwRadius;
	dwMaxViewObjNum /= 2;
	if (dwMaxViewObjNum > m_hfDesc.dwObjNumX * m_hfDesc.dwObjNumZ)
		dwMaxViewObjNum = m_hfDesc.dwObjNumX * m_hfDesc.dwObjNumZ;



	dwUnitFreeObjNum = (DWORD)( (float)dwMaxViewObjNum / 5.7f );;

	m_pRenderer->CreateResourcePool((IResourcePool**)&m_pStaticVBPool,RESORUCE_POOL_TYPE_VBSTATICPOOL);
	
	m_pStaticVBPool->Initialize(
		m_pRenderer->GetD3DResourceManager(),
		D3DFVF_HFIELDVERTEX,
		D3DUSAGE_WRITEONLY,
		D3DPOOL_DEFAULT,
		sizeof(HFIELDVERTEX),
		m_hfDesc.dwVerticesNumPerObj,
		dwMaxViewObjNum,
		dwUnitFreeObjNum
		);



	m_dwMaxHFieldObjNum = m_hfDesc.dwObjNumX * m_hfDesc.dwObjNumZ;
	m_ppHFieldObjectList = new CHFieldObject*[m_dwMaxHFieldObjNum];
	memset(m_ppHFieldObjectList,0,sizeof(CHFieldObject*)*m_dwMaxHFieldObjNum);

	bResult = TRUE;

lb_return:
	return bResult;
}


BOOL __stdcall CHFieldManager::LoadTilePalette(TEXTURE_TABLE* pTexTable,DWORD dwTileTextureNum)
{
	BOOL	bResult = TRUE;

	if (m_pTileTexture)
		delete [] m_pTileTexture;

	m_pTileTexture = new CTexture[dwTileTextureNum];
	
	m_hfDesc.pTexTable = pTexTable;
	m_hfDesc.dwTileTextureNum = dwTileTextureNum;

	char		txt[256];

	for (DWORD i=0; i<dwTileTextureNum; i++)
	{
		if (!m_pRenderer->AllocTexture(m_pTileTexture+i,pTexTable[i].szTextureName,0,TRUE,TRUE,TEXTURE_MAP_TYPE_DIFFUSE))
		{
			memset(txt,0,sizeof(txt));
			wsprintf(txt,"\nFail to Load TilePalette, TextureFile Not Found %s \n",pTexTable[i].szTextureName);
			OutputDebugString(txt);

//			bResult = FALSE;

		}
	}
	return bResult;

}
BOOL __stdcall CHFieldManager::ReplaceTile(char* szFileName,DWORD dwTexIndex)
{
	BOOL	bResult = FALSE;

	CTexture	tex;

	if (dwTexIndex >= m_hfDesc.dwTileTextureNum )
		goto lb_return;

	if (!m_pRenderer->AllocTexture(&tex,szFileName,0,TRUE,TRUE,TEXTURE_MAP_TYPE_DIFFUSE))
		goto lb_return;

	m_pTileTexture[dwTexIndex].Release();
	m_pRenderer->AllocTexture(&m_pTileTexture[dwTexIndex],szFileName,0,TRUE,TRUE,TEXTURE_MAP_TYPE_DIFFUSE);

lb_return:
	return bResult;
}
BOOL __stdcall CHFieldManager::CreateIndexBuffer(DWORD dwIndicesNum,DWORD dwDetailLevel,DWORD dwPositionMask,DWORD dwNum)
{
	BOOL		bResult = FALSE;
	if (dwDetailLevel >= (DWORD)m_hfDesc.bDetailLevelNum)
		goto lb_return;

	m_IndexTable[dwDetailLevel][dwPositionMask].pIndex = new WORD[dwIndicesNum];
	m_IndexTable[dwDetailLevel][dwPositionMask].dwIndicesNum = dwIndicesNum;

	bResult = TRUE;

lb_return:
	return bResult;
}
BOOL __stdcall CHFieldManager::InitiallizeIndexBufferPool(DWORD dwDetailLevel,DWORD dwIndicesNum,DWORD dwNum)
{
/////////////////////////////////////////
	DWORD	dwMaxViewObjNum;
	DWORD	dwUnitFreeObjNum;
	float	fLengthPerObj;

	DWORD	dwRadius;
	CAMERA_DESC*	pCameraDesc;
	pCameraDesc = m_pRenderer->INL_GetCameraDesc();
	fLengthPerObj = (float)(m_hfDesc.dwFacesNumPerObjAxis * m_hfDesc.fFaceSize);
	dwRadius = (DWORD)( pCameraDesc->fFar / fLengthPerObj ) + 1;
	

	

	dwMaxViewObjNum = dwRadius*dwRadius;
//	dwMaxViewObjNum = (dwDetailLevel+1) / m_hfDesc.dwDetailLevelNum;
	
	dwMaxViewObjNum = ((float)dwMaxViewObjNum / (float)m_hfDesc.bDetailLevelNum) * (float)(dwDetailLevel+1);
	if (dwMaxViewObjNum > m_hfDesc.dwObjNumX * m_hfDesc.dwObjNumZ)
		dwMaxViewObjNum = m_hfDesc.dwObjNumX * m_hfDesc.dwObjNumZ;



	dwUnitFreeObjNum = (DWORD)( (float)dwMaxViewObjNum / 5.7f );;



//	m_DetailLevelIndicesDesc[dwDetailLevel].dwIndicesNumPerObj = dwIndicesNum;
//	m_DetailLevelIndicesDesc[dwDetailLevel].dwMaxObjNumNum = dwMaxViewObjNum;
//	m_DetailLevelIndicesDesc[dwDetailLevel]..dwMaxObjNumNum = dwMaxViewObjNum;
	

////////////////////////////////////////

	m_dwIndexBufferNum += dwNum;
	m_dwMaxIndicesNum += dwIndicesNum*dwMaxViewObjNum;
	m_dwMaxViewObjNum += dwMaxViewObjNum;



	if (dwDetailLevel >= (DWORD)m_hfDesc.bDetailLevelNum-1)
	{
		DWORD	dwMaxBufferNum;
		dwMaxBufferNum = m_dwMaxIndicesNum / 65536 + 1;
		m_pIBHeap->Initialize(
			m_pRenderer->GetD3DResourceManager(),
			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
			D3DPOOL_DEFAULT,
			65536,
			dwMaxBufferNum,
			0,
			0);
			//	ALAWAYS_KEEP_BUFFER);
	}

	
/*
	return m_ppStaticIBPool[dwDetailLevel]->Initialize(
		m_pRenderer->GetD3DResourceManager(),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		D3DPOOL_DEFAULT,
		dwIndicesNum,
		dwMaxViewObjNum,
		dwUnitFreeObjNum);
*/
	return TRUE;
	
}
BOOL __stdcall CHFieldManager::LockIndexBufferPtr(WORD** ppWord,DWORD dwDetailLevel,DWORD dwPositionMask)
{
	*ppWord = m_IndexTable[dwDetailLevel][dwPositionMask].pIndex;
	return TRUE;

//	return m_IB[dwDetailLevel][dwPositionMask].LockPtr(ppByte);
}
void __stdcall CHFieldManager::UnlcokIndexBufferPtr(DWORD dwDetailLevel,DWORD dwPositionMask)
{
//	m_IB[dwDetailLevel][dwPositionMask].UnlockPtr();

}
void __stdcall CHFieldManager::EndInitialize()
{
	m_pRenderer->AddHFVBNum(m_pStaticVBPool->GetVertexBufferNum());
	m_pRenderer->AddHFIBNum(m_pIBHeap->GetIndexBufferNum());
/*
	IDirect3DTexture8*	pTexture;
	for (DWORD i=0; i<200; i++)
	{
		if ( D3D_OK != m_pRenderer->GetD3DResourceManager()->CreateTexture(256,256,1,0,D3DFMT_A4R4G4B4,D3DPOOL_MANAGED,&pTexture))
			__asm int 3
	}*/
}



IDIMeshObject* __stdcall CHFieldManager::CreateHeightFieldObject(HFIELD_OBJECT_DESC* pDesc)
{
	CHFieldObject*		pHFObj = NULL;
	if (m_dwHFieldObjNum >= m_dwMaxHFieldObjNum)
		goto lb_return;

	pHFObj = new CHFieldObject(this,0);
	

	m_ppHFieldObjectList[m_dwHFieldObjNum] = pHFObj;
	m_dwHFieldObjNum++;


	pHFObj->AddRef();
	/*
	CHFieldObject* pHFObj = new CHFieldObject(this,0);
	pHFObj->AddRef();

	m_ppHFieldObjectList[m_dwHFieldObjNum] = pHFObj;
	m_dwHFieldObjNum++;
*/
lb_return:
	return pHFObj;
}
BOOL CHFieldManager::AllocIndexBuffer(DWORD* pdwTriNum,CHFieldObject* pHFObj,DWORD dwDetailLevel,DWORD dwPositionMask,INDEX_LIST_DESC* pQuadIndexList)

{
	DWORD				i,j;
	TILE_BUFFER_DESC*	pTileBufferDesc;
	DWORD				dwTileBufferDescNum;

	CIndexBuffer*		pIB;
	BOOL				bResult;

	pHFObj->GetTileBuffer(&pTileBufferDesc,&dwTileBufferDescNum);

	DWORD	dwErrorCode;
	
	
	DWORD	dwIndicesNumPerTile = m_hfDesc.dwFacesNumPerTileAxis * m_hfDesc.dwFacesNumPerTileAxis * 2 *3;
	DWORD	level = dwDetailLevel;


	while (level)
	{
		dwIndicesNumPerTile = (dwIndicesNumPerTile>>2);
		level--;
	}		

	DWORD	dwIndicesNum = 0;
	for (i=0; i<dwTileBufferDescNum; i++)
	{
		dwIndicesNum += (pTileBufferDesc[i].dwTilePosNumPri +  pTileBufferDesc[i].dwTilePosNumExt) * dwIndicesNumPerTile;
	}
	if (dwIndicesNum)
	{
		bResult = m_pIBHeap->AllocIB(&dwErrorCode,&pHFObj->m_IB,dwIndicesNum,NULL,&pHFObj->m_IB.m_bEnable,pHFObj);
	}

	pIB = &pHFObj->m_IB;
#ifdef _DEBUG
	if (!bResult)
	{
		__asm int 3
	}
#endif

	DWORD	dwIndicesNumUnit,dwPitch;

	*pdwTriNum = m_dwLv0TriNumPerObj;
	dwIndicesNumUnit = m_dwLv0IndicesNumUnit;
	dwPitch = m_dwLv0IndicesPitch;

	for (i=0; i<dwDetailLevel; i++)
	{
		dwIndicesNumUnit /= 4;
		*pdwTriNum /= 4;
		dwPitch /= 4;
	}
	WORD*	pIndex;
	pIB->LockPtr(&pIndex);

	
	WORD*	pDest;
	WORD*	pSrc;

	pDest = pIndex;
	pSrc = m_IndexTable[dwDetailLevel][dwPositionMask].pIndex;

	for (i=0; i<dwTileBufferDescNum; i++)
	{
		for (DWORD k=0; k<4; k++)
		{
			pTileBufferDesc[i].indexEntryDesc[k].dwStartIndex = (DWORD)(pDest - pIndex); 
			WORD*	pStart = pDest;

			// 타일 한칸을 이루는 인덱스들을 카피한다.
			// pTilePos[]가 인덱스 리스트에서 타일 한칸을 이루는 인덱스 엔트리의 위치이다.
			INDEX_POS*	pTileSrc;
			
			pTileSrc = pTileBufferDesc[i].pTilePosPri + pTileBufferDesc[i].tileEntryDescPri[k].wTilePosEntry;
		
			for (j=0; j<pTileBufferDesc[i].tileEntryDescPri[k].wTilePosNum; j++)
			{
				memcpy(pDest,
					pSrc + 
					pTileSrc[j].dwX*dwIndicesNumUnit + // x축;
					pTileSrc[j].dwY*dwPitch,
					sizeof(WORD)*dwIndicesNumUnit);
	
				pDest += dwIndicesNumUnit;
			}
			if (m_bHFieldTileBlendEnable)
			{
				pTileSrc = pTileBufferDesc[i].pTilePosExt + pTileBufferDesc[i].tileEntryDescExt[k].wTilePosEntry;
				for (j=0; j<pTileBufferDesc[i].tileEntryDescExt[k].wTilePosNum; j++)
				{
					memcpy(pDest,
						pSrc + 
						pTileSrc[j].dwX*dwIndicesNumUnit + // x축;
						pTileSrc[j].dwY*dwPitch,
						sizeof(WORD)*dwIndicesNumUnit);
	
					pDest += dwIndicesNumUnit;
				}

			}
/*			INDEX_POS*	pTileSrc = pTileBufferDesc[i].pTilePos + pTileBufferDesc[i].tileEntryDesc[k].wTilePosEntry;
		
			// 외곽선을 제거하고 찍을지...합쳐서 찍을지 결정할 루틴..
			DWORD		dwTileNum;
			if (m_bHFieldTileBlendEnable)
				dwTileNum = pTileBufferDesc[i].tileEntryDesc[k].wTilePosNum;
			else
				dwTileNum = pTileBufferDesc[i].tileEntryDesc[k].wTilePosNum - pTileBufferDesc[i].tileEntryDesc[k].wExtPosNum;
				
	//		DWORD		dwTileNum = pTileBufferDesc[i].tileEntryDesc[k].wTilePosNum - pTileBufferDesc[i].tileEntryDesc[k].wExtPosNum;
			for (j=0; j<dwTileNum; j++)
			{
				memcpy(pDest,
					pSrc + 
					pTileSrc[j].dwX*dwIndicesNumUnit + // x축;
					pTileSrc[j].dwY*dwPitch,
					sizeof(WORD)*dwIndicesNumUnit);
	
				pDest += dwIndicesNumUnit;
			}*/

			pTileBufferDesc[i].indexEntryDesc[k].dwTriNum = (DWORD)(pDest - pStart) / 3;
		}
	}
	pIB->UnlockPtr();
	bResult = TRUE;

	return bResult;
}
BOOL __stdcall CHFieldManager::RenderGrid(VECTOR3* pv3Quad,DWORD dwTexTileIndex,DWORD dwAlpha)
{
	BOOL	bResult = FALSE;

	if (dwTexTileIndex >= m_hfDesc.dwTileTextureNum)
		goto lb_return;

	MATRIX4	matTex;
	SetHFieldTexMatrix(&matTex,pv3Quad);

	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,D3DTADDRESS_WRAP);
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,D3DTADDRESS_WRAP);
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX,D3DTSS_TCI_CAMERASPACEPOSITION );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_COUNT2 );

	BeginAlphaMeshObject(m_pD3DDevice,0,255,dwAlpha);
//	if (dwAlpha < 255)
//		EnableAlphaSetting(m_pD3DDevice,0,dwAlpha);
//	else
//		DefaultAlphaSetting(m_pD3DDevice,0);
	

	MatrixMultiply2(&matTex,m_pRenderer->INL_GetInverseViewMatriex(),&matTex);
	m_pD3DDevice->SetTransform((D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + 0),(D3DMATRIX*)&matTex);

	m_pD3DDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
	IMAGE_HEADER*	pHeader;
	SetIndexedTexture(dwTexTileIndex,0,&pHeader);
	m_pRenderer->RenderGridWithTexture(pv3Quad,D3DFILL_SOLID);

	EndAlphaMeshObject(m_pD3DDevice);
	

	bResult = TRUE;
	
lb_return:
	return bResult;

}
void __stdcall CHFieldManager::SetHFieldTileBlend(BOOL bSwitch)
{
	m_bHFieldTileBlendEnable = bSwitch;
}

BOOL __stdcall CHFieldManager::IsEnableHFieldTileBlend()
{
	return m_bHFieldTileBlendEnable;
}

CHFieldManager::~CHFieldManager()
{
	DWORD	i;

	m_pRenderer->UnLinkHFieldManager(this);

	delete [] m_pTileTexture;

	for (i=0; i<MAX_HFIELD_DETAIL_NUM; i++)
	{
		for (DWORD j=0; j<16; j++)
		{
			if (m_IndexTable[i][j].pIndex)
			{
				delete [] m_IndexTable[i][j].pIndex;
				m_IndexTable[i][j].pIndex = NULL;
				m_IndexTable[i][j].dwIndicesNum = 0;
			}
			
		}
	}
	
	m_pRenderer->SubHFIBNum(m_dwIndexBufferNum);
	m_pRenderer->SubHFVBNum(m_pStaticVBPool->GetVertexBufferNum());

	if (m_pStaticVBPool)
	{
		m_pRenderer->ReleaseResourcePool(m_pStaticVBPool);
		m_pStaticVBPool = NULL;
	}
	if (m_pIBHeap)
	{
		m_pRenderer->ReleaseResourcePool(m_pIBHeap);
		m_pIBHeap = NULL;
	}
	m_pRenderer->DecHFieldNum();

	if (m_ppHFieldObjectList)
	{
        delete [] m_ppHFieldObjectList;
        m_ppHFieldObjectList = NULL;
	}
}

void CHFieldManager::OnReset()
{
	for (DWORD i=0; i<m_dwHFieldObjNum; i++)
	{
		m_ppHFieldObjectList[i]->ReleaseIndexBuffer();
		m_ppHFieldObjectList[i]->ReleaseVertexBuffer();
	}
	m_pIBHeap->RemoveD3DRCBuffer();

}
/*
BOOL CHFieldManager::GetIndexBuffer(CIndexBuffer** ppIB,DWORD* pdwTriNum,CHFieldObject* pHFObj,DWORD dwDetailLevel,DWORD dwPositionMask,INDEX_LIST_DESC* pQuadIndexList)
{
	DWORD				i,j;
	TILE_BUFFER_DESC*	pTileBufferDesc;
	DWORD				dwTileBufferDescNum;

	pHFObj->GetTileBuffer(&pTileBufferDesc,&dwTileBufferDescNum);

	CIndexBuffer* pIB = m_pIndexBufferPool[dwDetailLevel].Alloc(pHFObj);

	*ppIB = pIB;
	*pdwTriNum = m_dwLv0TriNumPerObj;
	DWORD	dwIndicesNumUnit = m_dwLv0IndicesNumUnit;
	DWORD	dwPitch = m_dwLv0IndicesPitch;

	for (i=0; i<dwDetailLevel; i++)
	{
		dwIndicesNumUnit /= 4;
		*pdwTriNum /= 4;
		dwPitch /= 4;
	}
	WORD*	pIndex;
	pIB->LockPtr(&pIndex);


//	memcpy(pIndex,m_IndexTable[dwDetailLevel][dwPositionMask].pIndex,sizeof(WORD)*pIB->GetIndicesNum());

	
	WORD*	pDest = pIndex;
	WORD*	pSrc = m_IndexTable[dwDetailLevel][dwPositionMask].pIndex;

	for (i=0; i<dwTileBufferDescNum; i++)
	{
		pTileBufferDesc[i].dwStartIndex = (DWORD)(pDest - pIndex); 
		WORD*	pStart = pDest;

		for (j=0; j<pTileBufferDesc[i].dwTilePosNum; j++)
		{

			memcpy(pDest,
				pSrc + 
				pTileBufferDesc[i].pTilePos[j].dwX*dwIndicesNumUnit + // x축;
				pTileBufferDesc[i].pTilePos[j].dwY*dwPitch,
				sizeof(WORD)*dwIndicesNumUnit);

			pDest += dwIndicesNumUnit;

		}
		pTileBufferDesc[i].dwTriNum = (DWORD)(pDest - pStart) / 3;
		
	}


	pIB->UnlockPtr();


	return TRUE;
}*/

