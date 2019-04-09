#ifndef TILE_OBJECT_H
#define TILE_OBJECT_H

#include "../4DyuchiGRX_Common/typedef.h"


class CTileObject
{
	DWORD				m_dwTilePosX;
	DWORD				m_dwTilePosZ;
	TILE_BUFFER_DESC*	m_pTileBufferDesc;
	DWORD				m_dwTileBufferDescNum;
	INDEX_POS*			m_pTilePos;

	WORD				GetTile(DWORD dwPosXinObj,DWORD dwPosZinObj,WORD* pWorldTileTable,DWORD dwPitch);
public:
	void				GetTileBuffer(TILE_BUFFER_DESC** ppTileBufferDesc,DWORD* pdwTileBufferDescNum)
	{
		*ppTileBufferDesc = m_pTileBufferDesc;
		*pdwTileBufferDescNum = m_dwTileBufferDescNum;
	}
	DWORD				GetTileBufferDescNum() {return m_dwTileBufferDescNum;}
	TILE_BUFFER_DESC*	GetTileBufferDesc(DWORD dwIndex) {return m_pTileBufferDesc+dwIndex;}


	
	BOOL				Initialize(DWORD dwTilePosX,DWORD dwTilePosZ,HFIELD_DESC* pHFDesc);
	BOOL				UpdateTileInfo(HFIELD_DESC* pHFDesc);
	BOOL				UpdateIndexBuffer(WORD* pIndex,HFIELD_DESC* pHFDesc);
	

	CTileObject();
	~CTileObject();

};

#endif
