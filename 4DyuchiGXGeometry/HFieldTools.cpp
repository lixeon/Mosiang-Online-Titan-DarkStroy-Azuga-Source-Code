// HFieldAlphaMapManager.cpp: implementation of the CHFieldTools class.
//
//////////////////////////////////////////////////////////////////////

#define DEFAULT_BRUSH_SIZE			7
#define MAX_TEMP_ALPHAMAP_INDEX		4096

#include "HFieldTools.h"
#include "HFieldObject.h"
#include "global_variable.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// 알파맵의 기준 사이즈는 4*4이다. 즉 한 타일에 4*4 이미지가 들어간다. 고로 계조도 4단계뿐이다.
// 4계조 알파맵은 다시 4단계의 밝기를 가질 수 있다.

CHFieldTools::CHFieldTools()
{
	m_dwAlphaMapWidthHeight = 0;
	m_dwMaxAlphaMapNum = 0;
	m_dwAllocatedAlphaMapNum = 0;

	m_pAlphaMapPoolForDynamic = NULL;
	m_pAlphaMapPoolForStatic = NULL;
	memset(m_imgBrush,0,sizeof(m_imgBrush));

	m_pTempAlphaMapTable = NULL;
	m_ppTempAlphaMapIndexList = NULL;
	m_dwTempAlphaMapNum = 0;
	m_dwMaxTempAlphaMapNum = 0;


	m_pbTempTableBoundary = NULL;
	m_pbTableTableInner = NULL;
	m_pIndexPosTempPoolPerObject = NULL;
	m_pIndexPosTempPoolPerTile = NULL;
	m_dwMaxTempIndexPosNum = 0;
	m_dwBufferWidthHeight = 0;
	m_dwAllocatedAlphaMapMemSize = 0;

}

BOOL CHFieldTools::Initialize(HFIELD_DESC* pHFDesc,BOOL bEnableDrawAlphaMap,DWORD dwTotalAlphaMapNum,DWORD* pdwAlphaMapWidthHeight)
{
	m_pHFieldDesc = pHFDesc;
	DWORD	dwObjNum = pHFDesc->dwObjNumX * pHFDesc->dwObjNumZ;
	if (bEnableDrawAlphaMap)
	{
		m_dwAlphaMapWidthHeight = pHFDesc->dwTileNumPerObjAxis*4;
		m_dwAlphaMapSize = m_dwAlphaMapWidthHeight * m_dwAlphaMapWidthHeight / 4;
		m_dwMaxAlphaMapNum = dwObjNum * DEFAULT_OVERLAP_TILES_NUM;
		
		if (m_dwMaxAlphaMapNum < dwTotalAlphaMapNum)
			m_dwMaxAlphaMapNum = dwTotalAlphaMapNum + (DWORD)( (float)dwTotalAlphaMapNum * 0.1f );

		m_dwTotalAlphaMapSize = m_dwAlphaMapSize * m_dwMaxAlphaMapNum;

		m_bEnableDrawAlphaMap = TRUE;
		m_pAlphaMapPoolForDynamic = CreateStaticMemoryPool();
		InitializeStaticMemoryPool(m_pAlphaMapPoolForDynamic,m_dwAlphaMapSize,m_dwMaxAlphaMapNum/8+1,m_dwMaxAlphaMapNum);

		CreateTempAlphaMap(MAX_TEMP_ALPHAMAP_INDEX);

		// 타일 외곽선 처리를 위한 초기화...
		*pdwAlphaMapWidthHeight = m_dwAlphaMapWidthHeight;
	}
	else
	{
		m_dwAlphaMapWidthHeight = *pdwAlphaMapWidthHeight;
		m_dwAlphaMapSize = m_dwAlphaMapWidthHeight * m_dwAlphaMapWidthHeight / 4;
		m_dwMaxAlphaMapNum = dwTotalAlphaMapNum;
		m_dwTotalAlphaMapSize = m_dwAlphaMapSize * m_dwMaxAlphaMapNum;

		m_bEnableDrawAlphaMap = FALSE;
		m_pAlphaMapPoolForStatic = new char[m_dwTotalAlphaMapSize];
		memset(m_pAlphaMapPoolForStatic,0xff,m_dwTotalAlphaMapSize);
	}
	CreateTileOutLineEditResource(pHFDesc);
	CreateBrushes(DEFAULT_BRUSH_SIZE);
	return TRUE;
}
void CHFieldTools::GetResource(INDEX_POS** ppIndexPosPoolPerObject,INDEX_POS** ppIndexPosPoolPerTile,BYTE** ppTableBoundary,BYTE** ppTableInnfer,DWORD* pdwBufferWidthHeight)
{
	*pdwBufferWidthHeight = m_dwBufferWidthHeight;
	*ppIndexPosPoolPerObject = m_pIndexPosTempPoolPerObject;
	*ppIndexPosPoolPerTile = m_pIndexPosTempPoolPerTile;
	*ppTableBoundary = m_pbTempTableBoundary;
	*ppTableInnfer = m_pbTableTableInner;
}
void CHFieldTools::CreateTileOutLineEditResource(HFIELD_DESC* pHFDesc)
{
	ReleaseTileOutLineEditResource();

	m_dwTileNumPerObjAxis = pHFDesc->dwTileNumPerObjAxis;

	m_dwBufferWidthHeight = pHFDesc->dwTileNumPerObjAxis+2;
	m_dwBufferSize = m_dwBufferWidthHeight * m_dwBufferWidthHeight;

	m_pbTempTableBoundary = new BYTE[m_dwBufferSize];
	m_pbTableTableInner = new BYTE[m_dwBufferSize];

	DWORD	dwTileNum = pHFDesc->dwTileNumPerObjAxis*pHFDesc->dwTileNumPerObjAxis;
	m_dwMaxTempIndexPosNum =  dwTileNum * 8;
	m_pIndexPosTempPoolPerObject = new INDEX_POS[m_dwMaxTempIndexPosNum];
	m_pIndexPosTempPoolPerTile = new INDEX_POS[dwTileNum];
}
void CHFieldTools::ClearBuffers()
{

	memset(m_pbTempTableBoundary,0,m_dwBufferSize);
	memset(m_pbTableTableInner,0,m_dwBufferSize);
	
	for (DWORD i=1; i<=m_dwTileNumPerObjAxis; i++)
	{
		// 경계선을 0으로 세팅
		memset(m_pbTempTableBoundary+(i*m_dwBufferWidthHeight)+1,1,m_dwTileNumPerObjAxis);
		memset(m_pbTableTableInner+(i*m_dwBufferWidthHeight)+1,1,m_dwTileNumPerObjAxis);
	}

}
void CHFieldTools::ReleaseTileOutLineEditResource()
{
	if (m_pbTempTableBoundary)
	{
		delete [] m_pbTempTableBoundary;
		m_pbTempTableBoundary = NULL;
	}
	if (m_pbTableTableInner)
	{
		delete [] m_pbTableTableInner;
		m_pbTableTableInner = NULL;
	}
	if (m_pIndexPosTempPoolPerObject)
	{
		delete [] m_pIndexPosTempPoolPerObject;
		m_pIndexPosTempPoolPerObject = NULL;
	}
	if (m_pIndexPosTempPoolPerTile)
	{
		delete [] m_pIndexPosTempPoolPerTile;
		m_pIndexPosTempPoolPerTile = NULL;
	}
}

char* CHFieldTools::AllocAlphaMap()
{
	char*	p = NULL;
	
	if (m_dwAllocatedAlphaMapNum >= m_dwMaxAlphaMapNum)
		goto lb_return;

	if (m_bEnableDrawAlphaMap)
	{
		p = (char*)LALAlloc(m_pAlphaMapPoolForDynamic);
	}
	else
	{
		p = m_pAlphaMapPoolForStatic + m_dwAllocatedAlphaMapNum*m_dwAlphaMapSize;
	}
	memset(p,0xff,m_dwAlphaMapSize);
	m_dwAllocatedAlphaMapNum++;

	m_dwAllocatedAlphaMapMemSize = m_dwAllocatedAlphaMapNum*m_dwAlphaMapSize;
lb_return:
	return p;
}
void CHFieldTools::FreeAlphaMap(char* pBits)
{
	if (m_bEnableDrawAlphaMap)
	{
		LALFree(m_pAlphaMapPoolForDynamic,pBits);
		m_dwAllocatedAlphaMapNum--;
	}
	else
	{
	}
	m_dwAllocatedAlphaMapMemSize = m_dwAllocatedAlphaMapNum*m_dwAlphaMapSize;
}
void CHFieldTools::CreateBrushes(DWORD dwBrushSize)
{
	DWORD	dwUnitSize = dwBrushSize*dwBrushSize;
	
	m_pIndexedBitsBrush = new INDEXED_8BITS_PIXEL[dwUnitSize*4];
	memset(m_pIndexedBitsBrush,0,sizeof(INDEXED_8BITS_PIXEL)*dwUnitSize*4);

	m_imgBrush[0].pIndexedBits = m_pIndexedBitsBrush;
	m_imgBrush[0].wWidth = dwBrushSize;
	m_imgBrush[0].wHeight = dwBrushSize;

	m_imgBrush[1].pIndexedBits = m_pIndexedBitsBrush + dwUnitSize*1;
	m_imgBrush[1].wWidth = dwBrushSize;
	m_imgBrush[1].wHeight = dwBrushSize;

	m_imgBrush[2].pIndexedBits = m_pIndexedBitsBrush + dwUnitSize*2;
	m_imgBrush[2].wWidth = dwBrushSize;
	m_imgBrush[2].wHeight = dwBrushSize;

	m_imgBrush[3].pIndexedBits = m_pIndexedBitsBrush + dwUnitSize*3;
	m_imgBrush[3].wWidth = dwBrushSize;
	m_imgBrush[3].wHeight = dwBrushSize;

	for (DWORD i=0; i<4; i++)
	{
		m_imgBrush[i].dwPixelsNum = Create8BitsAttenuationMap(m_imgBrush[i].pIndexedBits,dwBrushSize,dwBrushSize,(dwBrushSize/4)*(i+1));
		m_imgBrush[i].wWidth = dwBrushSize;
		m_imgBrush[i].wHeight = dwBrushSize;
	}
}
void CHFieldTools::ReleaseBrushes()
{
	if (m_pIndexedBitsBrush)
	{
		delete [] m_pIndexedBitsBrush;
		m_pIndexedBitsBrush = NULL;
	}
	memset(m_imgBrush,0,sizeof(m_imgBrush));
}

IMAGE_BRUSH* CHFieldTools::GetImageBrush(DWORD dwBrightness)
{
	if (dwBrightness >= 4)
		dwBrightness = 3;

	return &m_imgBrush[dwBrightness];
}

CHFieldTools::~CHFieldTools()
{
	if (m_pAlphaMapPoolForDynamic)
	{
		ReleaseStaticMemoryPool(m_pAlphaMapPoolForDynamic);
		m_pAlphaMapPoolForDynamic = NULL;
	}
	if (m_pAlphaMapPoolForStatic)
	{
		delete [] m_pAlphaMapPoolForStatic;
		m_pAlphaMapPoolForStatic = NULL;
	}
	ReleaseBrushes();
	ReleaseTempAlphaMap();
	ReleaseTileOutLineEditResource();
}

void CHFieldTools::SaveAlphaMapTemporary(CHFieldObject* pHFieldObj,TILE_BUFFER_DESC* pTileBufferDesc,DWORD dwTileBufferDescNum)
{

	if (pTileBufferDesc)
	{
		m_dwTempAlphaMapNum = 0;

		
		for (DWORD i=0; i<dwTileBufferDescNum; i++)
		{
			DWORD	dwTileIndex = pTileBufferDesc[i].GetTileIndex();
			if (pTileBufferDesc[i].alphaMapDesc.pAlphaMapBits)
			{
#ifdef _DEBUG
				if (dwTileIndex >= m_dwMaxTempAlphaMapNum)
					__asm int 3
#endif
				m_pTempAlphaMapTable[dwTileIndex] = pTileBufferDesc[i].alphaMapDesc;
				m_ppTempAlphaMapIndexList[m_dwTempAlphaMapNum] = &m_pTempAlphaMapTable[dwTileIndex];
				m_dwTempAlphaMapNum++;
			}
		}
	}
}
BOOL CHFieldTools::LoadAlphaMapTemporary(CHFieldObject* pHFieldObj,TILE_BUFFER_DESC* pTileBufferDesc,DWORD dwTileBufferDescNum)
{
	BOOL	bResult = FALSE;
	if (m_dwTempAlphaMapNum)
	{
		for (DWORD i=0; i<dwTileBufferDescNum; i++)
		{
			DWORD	dwTileIndex = pTileBufferDesc[i].GetTileIndex();
			if (m_pTempAlphaMapTable[dwTileIndex].pAlphaMapBits)
			{
				pTileBufferDesc[i].alphaMapDesc = m_pTempAlphaMapTable[dwTileIndex];
				bResult = TRUE;
				// 지워서는 안되는 놈들로 표시해두고...이놈들 제외하곤 다 지울것이다.
				m_pTempAlphaMapTable[dwTileIndex].pAlphaMapBits = NULL;
			}
		}

		for (i=0; i<m_dwTempAlphaMapNum; i++)
		{
			if (m_ppTempAlphaMapIndexList[i]->pAlphaMapBits)
			{
				FreeAlphaMap(m_ppTempAlphaMapIndexList[i]->pAlphaMapBits);
				pHFieldObj->GetHFieldControlObject()->CleanupAlphaMap();

				m_ppTempAlphaMapIndexList[i]->pAlphaMapBits = NULL;
			}
		}

		memset(m_pTempAlphaMapTable,0,sizeof(ALPHAMAP_DESC)*m_dwMaxTempAlphaMapNum);
		m_dwTempAlphaMapNum = 0;
	}
	return bResult;
}
void CHFieldTools::CreateTempAlphaMap(DWORD dwMaxTempAlphaMapNum)
{
	m_dwMaxTempAlphaMapNum = dwMaxTempAlphaMapNum;

	m_pTempAlphaMapTable = new ALPHAMAP_DESC[m_dwMaxTempAlphaMapNum];
	memset(m_pTempAlphaMapTable,0,sizeof(ALPHAMAP_DESC)*m_dwMaxTempAlphaMapNum);

	m_ppTempAlphaMapIndexList = new ALPHAMAP_DESC*[m_dwMaxTempAlphaMapNum];
	memset(m_ppTempAlphaMapIndexList,0,sizeof(ALPHAMAP_DESC*)*m_dwMaxTempAlphaMapNum);
}

void CHFieldTools::ReleaseTempAlphaMap()
{
	if (m_pTempAlphaMapTable)
	{
		delete [] m_pTempAlphaMapTable;
		m_pTempAlphaMapTable = NULL;
	}
	if (m_ppTempAlphaMapIndexList)
	{
		delete [] m_ppTempAlphaMapIndexList;
		m_ppTempAlphaMapIndexList = NULL;
	}
}
DWORD CHFieldTools::Create8BitsAttenuationMap(INDEXED_8BITS_PIXEL* pIndexedBits,DWORD dwWidth,DWORD dwHeight,DWORD dwRS)
{
	DWORD			x,y;

	int			iRSPow = (int)(dwRS*dwRS);
	int			iCenterX = (int)(dwWidth/2);
	int			iCenterY = (int)(dwHeight/2);
	int			iColor;

	DWORD		dwNum = 0;
	for (y=0; y<dwHeight; y++)
	{
		for (x=0; x<dwWidth; x++)
		{
			int	iDistPow = (iCenterX-(int)x)*(iCenterX-(int)x) + (iCenterY-(int)y)*(iCenterY-(int)y);
				
			if (iDistPow < iRSPow)
			{
				float	fFallOff = (float)(iRSPow - iDistPow) / (float)iRSPow;
			
				iColor = (int)( 255.0f * fFallOff );
				pIndexedBits[dwNum].bColor = (BYTE)iColor;
				pIndexedBits[dwNum].bX = x;
				pIndexedBits[dwNum].bY = y;
				pIndexedBits[dwNum].bEnable = 1;
				dwNum++;
			}
		}
	}
	return dwNum;
}


DWORD CHFieldTools::WriteAlphaMap(char* szFileName,CHFieldObject* pHFObjList,DWORD dwObjNum)
{	
	FILE* fp = fopen(szFileName,"wb");
	if (!fp)
		goto lb_return;


	ALPHA_MAP_HEADER	header;
	header.dwAlphaMapNum = m_dwAllocatedAlphaMapNum;
	header.dwWidthHeight = m_dwAlphaMapWidthHeight;
	header.dwReserved0 = 0;
	header.dwReserved1 = 0;
	header.dwReserved2 = 0;
	header.dwReserved3 = 0;

	fwrite(&header,sizeof(header),1,fp);


	DWORD	dwTotalSize,dwImgSize;
	char*	pMem;

	dwImgSize = (m_dwAlphaMapWidthHeight*m_dwAlphaMapWidthHeight)/4;
	dwTotalSize = (dwImgSize + ALPHA_MAP_FILE_HEADER_SIZE) * m_dwAllocatedAlphaMapNum;
	pMem = new char[dwTotalSize];
	memset(pMem,0,dwTotalSize);

	fwrite(&dwTotalSize,sizeof(DWORD),1,fp);

	DWORD	i;
	char*	pMemEntry;
	pMemEntry = pMem;
	
	DWORD	dwObjCount;
	dwObjCount = 0;

	DWORD	dwSize;
	for (i=0; i<dwObjNum; i++)
	{
		dwSize = pHFObjList[i].WriteAlphaMap(pMemEntry,i,dwImgSize);
		if (dwSize)
		{
			pMemEntry += pHFObjList[i].WriteAlphaMap(pMemEntry,i,dwImgSize);
			dwObjCount++;
		}
		
	}
#ifdef _DEBUG
	if ((DWORD)(pMemEntry - pMem) != dwTotalSize)
		__asm int 3
#endif
	fwrite(pMem,1,dwTotalSize,fp);
	
	delete [] pMem;
	fclose(fp);
lb_return:
	return dwTotalSize;
}


DWORD CHFieldTools::ReadAlphaMap(char* szFileName,CHFieldObject* pHFObjList,DWORD dwObjNum)
{
	DWORD	dwLen = 0;
	void*	pFP = g_pFileStorage->FSOpenFile(szFileName,FSFILE_ACCESSMODE_BINARY);
	if (!pFP)
		goto lb_return;

	ALPHA_MAP_HEADER	header;

	DWORD				dwTotalSize,dwImgSize;

	dwLen += g_pFileStorage->FSRead(pFP,&header,sizeof(ALPHA_MAP_HEADER));
	dwLen += g_pFileStorage->FSRead(pFP,&dwTotalSize,sizeof(DWORD));

	dwImgSize = (header.dwWidthHeight * header.dwWidthHeight) / 4;
	
#ifdef _DEBUG
	if (dwTotalSize != (dwImgSize + ALPHA_MAP_FILE_HEADER_SIZE) * header.dwAlphaMapNum)
	{
		__asm int 3
		goto lb_return;
	}
	if (header.dwAlphaMapNum > m_dwMaxAlphaMapNum)
	{
		__asm int 3
		goto lb_return;
	}
	if (header.dwWidthHeight != m_dwAlphaMapWidthHeight)
	{
		__asm int 3
		goto lb_return;
	}
#endif
	char*	pMemSrc;
	pMemSrc = new char[dwTotalSize];
	dwLen += g_pFileStorage->FSRead(pFP,pMemSrc,dwTotalSize);



	DWORD	i;
	char*	p2BitsImg;
	char*	pMemEntry;
	pMemEntry = pMemSrc;


	
	DWORD	dwSize;
	dwSize = 0;
	
	for (i=0; i<header.dwAlphaMapNum; i++)
	{
		ALPHA_MAP_FILE_HEADER*	pHeader = (ALPHA_MAP_FILE_HEADER*)pMemEntry;
#ifdef _DEBUG
		if (pHeader->dwObjIndex >= dwObjNum)
			__asm int 3
#endif
		p2BitsImg = AllocAlphaMap();
		memcpy(p2BitsImg,pHeader->p2BitsImg,dwImgSize);
		pHFObjList[pHeader->dwObjIndex].SetAlphaMap(pHeader->dwTileIndex,p2BitsImg,header.dwWidthHeight);
		pHFObjList[pHeader->dwObjIndex].UpdateExtTileInfo(m_pHFieldDesc,0xffffffff,FALSE);

		dwSize += (dwImgSize + ALPHA_MAP_FILE_HEADER_SIZE);
		pMemEntry += (dwImgSize + ALPHA_MAP_FILE_HEADER_SIZE);
	}
#ifdef _DEBUG
	if (dwSize != dwTotalSize)
		__asm int 3
#endif

	delete [] pMemSrc;
	g_pFileStorage->FSCloseFile(pFP);
lb_return:

	return dwLen;
}
