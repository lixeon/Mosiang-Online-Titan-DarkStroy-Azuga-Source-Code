// MHMap.h: interface for the CMHMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MHMAP_H__A2087CD2_73AB_4751_8B7E_D99BEA2111C6__INCLUDED_)
#define AFX_MHMAP_H__A2087CD2_73AB_4751_8B7E_D99BEA2111C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//#include "ServerTable.h"

class CTileManager;

struct MAPSERVERDESC
{
	WORD	MapNum;
	BOOL	bVillage;	//¸¶À»ÀÎ°¡?
	BOOL	bPKAllow;	//pkÇã¿ë?
	BOOL	bAutoNoteAllow;

	MAPSERVERDESC()
	{
		MapNum		= 0;
		bVillage	= FALSE;
		bPKAllow	= FALSE;
		bAutoNoteAllow = TRUE;
	}
};

class CMHMap  
{
	CTileManager* m_pTileManager;
//	char m_szTileName[MAX_MAP_NUM][MAX_FILE_NAME];
	char m_szTileName[256];

//KES 040712 MapServerDesc
	MAPSERVERDESC	m_msDesc;

public:
	inline CTileManager* GetTileManager()	{	return m_pTileManager;	}

	CMHMap();
	virtual ~CMHMap();

	void InitMap(MAPTYPE MapNum);
	void LoadMapServerDesc( char* strFileName, MAPTYPE MapNum );
	void Release();
	
	int GetTileWidth(MAPTYPE MapNum);

	BOOL IsInTile(int x, int y, CObject* pObject);
	BOOL CollisionTilePos(float x, float y, CObject* pObject);
	BOOL CollisionTilePos(float x, float y, MAPTYPE MapNum);
	BOOL CollisionTileCell(int x, int y, CObject* pObject);

	BOOL CollisionLine(VECTOR3* pStart,VECTOR3* pEnd, VECTOR3 * Target, CObject* pObject);
	BOOL CollisionCheck(VECTOR3* pStart,VECTOR3* pEnd,VECTOR3* pRtCollisonPos, CObject* pObject);		// Ãæµ¹ÇÏ¸é TRUE¹ÝÈ¯ÇÏ°í pRtCollisonPos¿¡ »õ·Î¿î Å¸°Ù ¼³Á¤

	char* GetTileName(WORD MapNum){return m_szTileName;}
	void TileNameSetting();

////KES
	BOOL IsVillage() { return m_msDesc.bVillage; }
	BOOL IsPKAllow() { return m_msDesc.bPKAllow; }

	char*	GetMapName(DWORD dwMapNum = 0);
	BOOL	IsMapKind(DWORD dwMapKind, DWORD dwMapNum = 0);
	BOOL	IsMapSame(DWORD dwMapKind, DWORD dwMapNum = 0);

	BOOL IsAutoNoteAllow()	{ return m_msDesc.bAutoNoteAllow; }
	void SetAutoNoteAllow( BOOL bAllow )	{ m_msDesc.bAutoNoteAllow = bAllow; }
};

#endif // !defined(AFX_MHMAP_H__A2087CD2_73AB_4751_8B7E_D99BEA2111C6__INCLUDED_)
