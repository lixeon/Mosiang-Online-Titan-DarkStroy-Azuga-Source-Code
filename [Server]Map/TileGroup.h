// TileGroup.h: interface for the CTileGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILEGROUP_H__37A91A76_D547_4FF9_9F13_D3E1DE69EDE3__INCLUDED_)
#define AFX_TILEGROUP_H__37A91A76_D547_4FF9_9F13_D3E1DE69EDE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CFixedTileInfo;
class CTile;

#define MAX_ADDPos 26

enum
{
	eDIR_North = 0,
	eDIR_East,
	eDIR_South,
	eDIR_West,

	eDIR_Max,
};

class CTileGroup  
{
	DWORD m_ID;
	MAPTYPE m_MapNum;
	float m_fStartX;
	float m_fStartZ;
	int m_nTileWidth;
	int m_nTileHeight;
	CFixedTileInfo* m_pFixedTileInfo;

	CTile* m_pTile;
	
	TILEINDEX m_AddPos[eDIR_Max][MAX_ADDPos];
	float m_AddDistance[MAX_ADDPos];
	
	TILEINDEX **m_ppObjectSizeRegion;
	
	TILEINDEX GetTileIndex(float fx, float fz);
	CTile* GetTile(TILEINDEX* pIndex);

public:
	CTileGroup();
	virtual ~CTileGroup();


	void Init(CFixedTileInfo* pFixedTileInfo, DWORD TileGroupID, MAPTYPE MapNum, float fStartX, float fStartZ, float fWidth, float fHeight);
	void AddObject(CObject* pObject);
	void RemoveObject(CObject* pObject);
	int GetPreoccupiedNum(CObject* pObject,VECTOR3* pPos);
	VECTOR3	GetNextCloserPos(CObject* pObject,VECTOR3* pTargetPos,VECTOR3* pCurPos,float closestDist,float limitDist);
	BOOL Preoccupy(CObject* pObject,VECTOR3* pPos);
	BOOL IsPreoccupied(int size,TILEINDEX* pCenterTile,CObject* pObject = NULL);
	BOOL IsPreoccupied(CObject* pObject,VECTOR3* pPos);

	void AddTileAttrib(VECTOR3* pPos,AREATILE areatile);
	void RemoveTileAttrib(VECTOR3* pPos,AREATILE areatile);

	CFixedTileInfo* GetFixedTileInfo()	{	return m_pFixedTileInfo;	}
};

#endif // !defined(AFX_TILEGROUP_H__37A91A76_D547_4FF9_9F13_D3E1DE69EDE3__INCLUDED_)
