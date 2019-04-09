// TileManager.h: interface for the CTileManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILEMANAGER_H__5FD3CE62_5211_46E8_9C7B_4B5CB5D006CA__INCLUDED_)
#define AFX_TILEMANAGER_H__5FD3CE62_5211_46E8_9C7B_4B5CB5D006CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../4DyuchiGRX_common/typedef.h"
#include "Tile.h"

class CTileManager  
{
protected:
	DWORD m_dwTileWidth;
	DWORD m_dwTileHeight;
	
	CTile* m_pTile;
	
	CTile* GetTile(DWORD dwX, DWORD dwZ);
	CTile* GetTile(float fx, float fz);

public:
	CTileManager();
	virtual ~CTileManager();
	
	BOOL LoadTileInfo(char* TileFile);
	BOOL CollisonCheck(VECTOR3* pStart,VECTOR3* pEnd,VECTOR3* pRtCollisonPos);		// Ãæµ¹ÇÏ¸é TRUE¹ÝÈ¯ÇÏ°í pRtCollisonPos¿¡ »õ·Î¿î Å¸°Ù ¼³Á¤

	void AddTileAttrByAreaData(VECTOR3* pPos,AREATILE areatile);
	void RemoveTileAttrByAreaData(VECTOR3* pPos,AREATILE areatile);
};

#endif // !defined(AFX_TILEMANAGER_H__5FD3CE62_5211_46E8_9C7B_4B5CB5D006CA__INCLUDED_)
