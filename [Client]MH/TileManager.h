// TileManager.h: interface for the CTileManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILEMANAGER_H__5FD3CE62_5211_46E8_9C7B_4B5CB5D006CA__INCLUDED_)
#define AFX_TILEMANAGER_H__5FD3CE62_5211_46E8_9C7B_4B5CB5D006CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Tile.h"
#define fTILE_WIDTH 50
#define fTILE_HEIGHT 50
#define TILECOLLISON_DETAIL		50.f

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
	~CTileManager();

	DWORD GetTileWidth() { return m_dwTileWidth; }
	BOOL IsInTile(int x, int y, MAPTYPE MapNum,CObject* pObject);
	BOOL CollisionTile(int x, int y, MAPTYPE MapNum,CObject* pObject);
	BOOL CollisionLine(VECTOR3* pStart,VECTOR3* pEnd, VECTOR3 * Target, MAPTYPE MapNum,CObject* pObject);
	BOOL NonCollisionLine(VECTOR3* pStart, VECTOR3* pEnd, VECTOR3* pRtNonCollisonPos, MAPTYPE MapNum, CObject* pObj);	// 충돌아니면 TRUE반환하고 pRtNonCollisonPos에 비충돌 타일 세팅

	BOOL LoadTileInfo(char* TileFile);
	BOOL CollisonCheck(VECTOR3* pStart,VECTOR3* pEnd,VECTOR3* pRtCollisonPos,CObject* pObj);		// 충돌하면 TRUE반환하고 pRtCollisonPos에 새로운 타겟 설정

	void AddTileAttrByAreaData(CObject* Object,VECTOR3* pPos,AREATILE areatile);
	void RemoveTileAttrByAreaData(CObject* Object,VECTOR3* pPos,AREATILE areatile);
};

#endif // !defined(AFX_TILEMANAGER_H__5FD3CE62_5211_46E8_9C7B_4B5CB5D006CA__INCLUDED_)
