#ifndef TILE_OBJECT_H
#define TILE_OBJECT_H

#include "../4DyuchiGRX_Common/typedef.h"

class CHFieldTools;
class CHFieldObject;

struct ALPHA_MAP_FILE_HEADER
{
	DWORD	dwObjIndex;
	DWORD	dwTileIndex;
	char	p2BitsImg[1];
};
#define ALPHA_MAP_FILE_HEADER_SIZE	8

class CTileObject
{
	DWORD				m_dwTilePosX;
	DWORD				m_dwTilePosZ;
	TILE_BUFFER_DESC*	m_pTileBufferDesc;
	DWORD				m_dwTileBufferDescNum;
	INDEX_POS*			m_pTilePosPri;
	INDEX_POS*			m_pTilePosExt;
	

	WORD				GetTile(DWORD dwPosXinObj,DWORD dwPosZinObj,WORD* pWorldTileTable,DWORD dwPitch);
	void				SetTile(DWORD dwPosXinObj,DWORD dwPosZinObj,WORD* pWorldTileTable,DWORD dwPitch,WORD wTileIndex);

	void				ClearMember();
	void				CleanupTileBufferDesc();
	void				CleanupAlphaMapWithTileBufferDesc(TILE_BUFFER_DESC* pTileBufferDesc);

public:
	void				CleanupAlphaMap();
	void				CleanupAlphaMapWithTileIndex(WORD wTileIndex);

	CHFieldObject*		GetHFieldObject();
	void				GetTileBuffer(TILE_BUFFER_DESC** ppTileBufferDesc,DWORD* pdwTileBufferDescNum)
	{
		*ppTileBufferDesc = m_pTileBufferDesc;
		*pdwTileBufferDescNum = m_dwTileBufferDescNum;
	}
	DWORD				GetTileBufferDescNum() 
	{
		return m_dwTileBufferDescNum;
	}
	TILE_BUFFER_DESC*	GetTileBufferDesc(DWORD dwIndex) {return m_pTileBufferDesc+dwIndex;}


	void				SetAlphaMap(WORD wTileIndex,char* p2BitsImg,DWORD dwWidthHeight);
	void				GetAlphaMap(char** pp2BitsImg,DWORD* pdwWidthHeight,WORD wTileIndex);
	DWORD				WriteAlphaMap(char* pMemDest,DWORD dwObjIndex,DWORD dwImgSize);
	
	TILE_BUFFER_DESC*	SetAlphaTexel(
										CHFieldTools*	pAlphaMapManager,
										CHFieldObject* pHFieldObject,
										WORD  wTileTexIndex,
										DWORD dwActionFlag,
										DWORD dwCenterX,
										DWORD dwCenterZ,
										DWORD dwRS,
										DWORD dwTexWidthHeight);

	WORD				GetExtTileTop(DWORD dwTilePosX,DWORD dwTilePosZ);
	
	BOOL				Initialize(DWORD dwTilePosX,DWORD dwTilePosZ,HFIELD_DESC* pHFDesc);
	BOOL				UpdateTileInfo(HFIELD_DESC* pHFDesc);
	BOOL				UpdateExtTileInfo(HFIELD_DESC* pHFDesc,WORD wTileIndex,BOOL bSpecifiedTileIndex);
	INDEX_POS*			GetTilePos() {return m_pTilePosPri;}
	void				Cleanup();
	

	CTileObject();
	~CTileObject();

};

#endif
