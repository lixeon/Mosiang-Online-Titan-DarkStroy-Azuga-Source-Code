// HFieldAlphaMapManager.h: interface for the CHFieldTools class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HFIELDALPHAMAPMANAGER_H__AF0AEDF8_290C_4416_8305_8B1DA5DEA62F__INCLUDED_)
#define AFX_HFIELDALPHAMAPMANAGER_H__AF0AEDF8_290C_4416_8305_8B1DA5DEA62F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../4DyuchiGXGFunc/global.h"


class CHFieldObject;
struct TILE_BUFFER_DESC_KEY
{
	DWORD					dwTileTexIndex;
	CHFieldObject*			pHFieldObject;
};

struct INDEXED_8BITS_PIXEL
{
	BYTE		bColor;
	BYTE		bX;
	BYTE		bY;
	BYTE		bEnable;
};
struct IMAGE_BRUSH
{
	DWORD					dwPixelsNum;
	INDEXED_8BITS_PIXEL*	pIndexedBits;
	WORD					wWidth;
	WORD					wHeight;
	
};

struct ALPHA_MAP_HEADER
{
	DWORD	dwAlphaMapNum;
	DWORD	dwWidthHeight;
	DWORD	dwReserved0;
	DWORD	dwReserved1;
	DWORD	dwReserved2;
	DWORD	dwReserved3;
};

class CHFieldTools  
{
	DWORD					m_dwAlphaMapWidthHeight;
	DWORD					m_dwAlphaMapSize;
	DWORD					m_dwTotalAlphaMapSize;
	DWORD					m_dwMaxAlphaMapNum;

	DWORD					m_dwAllocatedAlphaMapNum;
	IMAGE_BRUSH				m_imgBrush[4];

	HFIELD_DESC*			m_pHFieldDesc;
	STMPOOL_HANDLE			m_pAlphaMapPoolForDynamic;
	char*					m_pAlphaMapPoolForStatic;
	BOOL					m_bEnableDrawAlphaMap;
	INDEXED_8BITS_PIXEL*	m_pIndexedBitsBrush;

	// 툴에서 에디트 할때 임시로 저장한다. 툴 모드가 아니면 NULL이다.
	ALPHAMAP_DESC*			m_pTempAlphaMapTable;
	ALPHAMAP_DESC**			m_ppTempAlphaMapIndexList;
	DWORD					m_dwTempAlphaMapNum;
	DWORD					m_dwMaxTempAlphaMapNum;

	// 타일의 외곽선을 검출하기 위한 멤버들...
	DWORD					m_dwTileNumPerObjAxis;
	BYTE*					m_pbTempTableBoundary;
	BYTE*					m_pbTableTableInner;
	INDEX_POS*				m_pIndexPosTempPoolPerObject;
	INDEX_POS*				m_pIndexPosTempPoolPerTile;
	DWORD					m_dwMaxTempIndexPosNum;
	
	DWORD					m_dwBufferWidthHeight;
	DWORD					m_dwBufferSize;
	DWORD					m_dwAllocatedAlphaMapMemSize;

	void					CreateTempAlphaMap(DWORD dwMaxTempAlphaMapNum);
	void					ReleaseTempAlphaMap();

	void					CreateTileOutLineEditResource(HFIELD_DESC* pHFDesc);
	void					ReleaseTileOutLineEditResource();
	



public:
	DWORD					GetAllocatedAlphaMapMemSize() {return m_dwAllocatedAlphaMapMemSize;}
	DWORD					GetAllocatedAlphaMapNum() {return m_dwAllocatedAlphaMapNum;}
	DWORD					GetAlphaMapWidthHeight() {return m_dwAlphaMapWidthHeight;}
	void					GetResource(INDEX_POS** ppIndexPosPoolPerObject,INDEX_POS** ppIndexPosPoolPerTile,BYTE** ppTableBoundary,BYTE** ppTableInnfer,DWORD* pdwBufferWidthHeight);
	void					ClearBuffers();
	void					SaveAlphaMapTemporary(CHFieldObject* pHFieldObj,TILE_BUFFER_DESC* pTileBufferDesc,DWORD dwTileBufferDescNum);
	BOOL					LoadAlphaMapTemporary(CHFieldObject* pHFieldObj,TILE_BUFFER_DESC* pTileBufferDesc,DWORD dwTileBufferDescNum);

	IMAGE_BRUSH*			GetImageBrush(DWORD dwBrightness);
	void					CreateBrushes(DWORD dwBrushSize);
	void					ReleaseBrushes();
	BOOL					Initialize(HFIELD_DESC* pHFDesc,BOOL bEnableDrawAlphaMap,DWORD dwTotalAlphaMapNum,DWORD* pdwAlphaMapWidthHeight);
	DWORD					Create8BitsAttenuationMap(INDEXED_8BITS_PIXEL* pIndexedBits,DWORD dwWidth,DWORD dwHeight,DWORD dwRS);

	char*					AllocAlphaMap();
	void					FreeAlphaMap(char* pBits);

	DWORD					ReadAlphaMap(char* szFileName,CHFieldObject* pHFObjList,DWORD dwObjNum);
	DWORD					WriteAlphaMap(char* szFileName,CHFieldObject* pHFObjList,DWORD dwObjNum);
	
	CHFieldTools();
	~CHFieldTools();

};

#endif // !defined(AFX_HFIELDALPHAMAPMANAGER_H__AF0AEDF8_290C_4416_8305_8B1DA5DEA62F__INCLUDED_)
