// TileManager.h: interface for the CTileManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILEMANAGER_H__5FD3CE62_5211_46E8_9C7B_4B5CB5D006CA__INCLUDED_)
#define AFX_TILEMANAGER_H__5FD3CE62_5211_46E8_9C7B_4B5CB5D006CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CObject;
class CTileGroup;
class CFixedTileInfo;

#define TILECOLLISON_DETAIL		50.f	// Á¤È®µµ.. ÀÛ°Ô ÇÒ¼ö·Ï ´À¸®Áö¸¸ Á¤È®ÇÑ Ã¼Å©¸¦ ÇÒ¼ö ÀÖµû
#define MAX_ObjectSize 3

/*
enum
{
	eDIR_North = 0,
	eDIR_East,
	eDIR_South,
	eDIR_West,

	eDIR_Max,
};*/


class CTileManager  
{
//protected:
/*
	int m_nTileWidth;
	int m_nTileHeight;
	
	CTile* m_pTile;


	CTile* GetTile(TILEINDEX* pTileIndex);
	CTile* GetTile(float fx, float fz);
	CTile* GetTile(int fx, int fz);
	*/

//	TILEINDEX GetTileIndex(float fx, float fz);
	
//	TILEINDEX m_AddPos[eDIR_Max][MAX_ADDPos];
//	float m_AddDistance[MAX_ADDPos];

//	TILEINDEX **m_ppObjectSizeRegion;


	CYHHashTable<CTileGroup> m_TileGroupTable;
	CYHHashTable<CFixedTileInfo> m_FixedTileInfoTable;

public:
	CTileManager();
	virtual ~CTileManager();
	BOOL LoadFixedTileInfo(MAPTYPE MapNum, char* pMapFile);
	void ReadMapFile(MAPTYPE MapNum, char* filename);
	BOOL CollisionCheck(VECTOR3* pStart,VECTOR3* pEnd,VECTOR3* pRtCollisonPos,CObject* pObject);		// Ãæµ¹ÇÏ¸é TRUE¹ÝÈ¯ÇÏ°í pRtCollisonPos¿¡ »õ·Î¿î Å¸°Ù ¼³Á¤

	void AddObject(CObject* pObject);
	void RemoveObject(CObject* pObject);

	BOOL IsPreoccupied(CObject* pObject,VECTOR3* pPos);
//	BOOL IsPreoccupied(int size,TILEINDEX* pCenterTile,CObject* pObject = NULL);

	int GetPreoccupiedNum(CObject* pObject,VECTOR3* pPos);

	BOOL Preoccupy(CObject* pObject,VECTOR3* pPos);
	VECTOR3 GetNextCloserPos(CObject* pObject,VECTOR3* pTargetPos,VECTOR3* pCurPos,float closestDist, float limitDist);
	
	void AddTileAttrByAreaData(CObject* pObject,VECTOR3* pPos,AREATILE areatile);
	void RemoveTileAttrByAreaData(CObject* pObject,VECTOR3* pPos,AREATILE areatile);
	
	CFixedTileInfo* GetFixedTileInfo(MAPTYPE MapNum);
	CFixedTileInfo* GetFixedTileInfo(CObject* pObject);

	CTileGroup* GetTileGroup(CObject * pObject);
	CTileGroup* GetTileGroup(DWORD TileGroupID);

	BOOL CreateTileGroup(DWORD TileGroupID, MAPTYPE MapNum, float fStartX, float fStartZ, float fWidth, float fHeight);
	void DeleteTileGroup(DWORD TileGroupID);
	
	int GetTileWidth(MAPTYPE MapNum);

	BOOL IsInTile(int cellX, int cellY, CObject* pObject);
	BOOL CollisionTileWithTileIndex( int x, int y, CObject* pObject);
	BOOL CollisionTileWithTileIndex( int x, int y, MAPTYPE MapNum);
	BOOL CollisionTileWithPosition( float fx, float fy, CObject* pObject);
	BOOL CollisionTileWithPosition( float fx, float fy, MAPTYPE MapNum);

	BOOL CollisionLine(VECTOR3* pStart,VECTOR3* pEnd, VECTOR3 * Target, CObject* pObject);
};

#endif // !defined(AFX_TILEMANAGER_H__5FD3CE62_5211_46E8_9C7B_4B5CB5D006CA__INCLUDED_)
