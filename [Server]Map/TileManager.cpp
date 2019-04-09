// TileManager.cpp: implementation of the CTileManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TileManager.h"
#include "Tile.h"
#include "FixedTile.h"
#include "FixedTileInfo.h"
#include "TileGroup.h"
#include "MHFile.h"
#include "Battle.h"
#include "Object.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*

int TM_AddPos[MAX_ADDPos*3] = {
	0, 0,

	0, -1,
	1, -1,
	-1, -1,

	0, -2,
	1, -2,
	-1, -2,
	2, -2,
	-2, -2,

	0, -3,
	1, -3,
	-1, -3,
	2, -3,
	-2, -3,
	3, -3,
	-3, -3,
	
	0, -4,
	1, -4,
	-1, -4,
	2, -4,
	-2, -4,
	3, -4,
	-3, -4,
	4, -4,
	-4, -4,

	12, 0,
};

int TM_ObjectRegionNum[] = {
	1,	5,	13,	
};

int TM_ObjectSizeTiles[] = {
	0, 0,

	1, 0,
	-1, 0,
	0, 1,
	0, -1,

	2, 0,
	1, 1,
	0, 2,
	-1, 1,
	-2, 0,
	-1, -1,
	0, -2,
	1, -1,
};

*/

CTileManager::CTileManager()
{
	//////////////////////////////////////////////////////////////////////////
	// test	
	{
		VECTOR3 vdir;
		SetVector3(&vdir,0,0,0);		// ???  => 90
		float angle = RADTODEG(VECTORTORAD(vdir));
		angle = angle;
	}
	{
		VECTOR3 vdir;
		SetVector3(&vdir,0,0,-1);		// 0
		float angle = RADTODEG(VECTORTORAD(vdir));
		angle = angle;
	}
	{
		VECTOR3 vdir;
		SetVector3(&vdir,1,0,0);		// 90
		float angle = RADTODEG(VECTORTORAD(vdir));
		angle = angle;
	}
	{
		VECTOR3 vdir;
		SetVector3(&vdir,0,0,1);		// 180
		float angle = RADTODEG(VECTORTORAD(vdir));
		angle = angle;
	}
	{
		VECTOR3 vdir;
		SetVector3(&vdir,-1,0,0);		// 270
		float angle = RADTODEG(VECTORTORAD(vdir));
		angle = angle;
	}


	m_TileGroupTable.Initialize(10);
	m_FixedTileInfoTable.Initialize(10);

/*	m_pTile = NULL;


		float angle;
		TILEINDEX temp;
		for(int n=0;n<eDIR_Max;++n)
		{
			angle = DEGTORAD(90 * n);
			
			for(int i=0;i<MAX_ADDPos;++i)
			{
				temp.nx = TM_AddPos[2*i + 0];
				temp.nz = TM_AddPos[2*i + 1];
	
				float fx,fz;
				fx = temp.nx*cosf(angle) - temp.nz*sinf(angle);
				fz = temp.nx*sinf(angle) + temp.nz*cosf(angle);
				// float 계산 오차때문에
				m_AddPos[n][i].nx = int(fx + (fx>0?0.1f:-0.1f));
				m_AddPos[n][i].nz = int(fz + (fz>0?0.1f:-0.1f));
	
				m_AddDistance[i] = sqrtf(float(temp.nx*temp.nx + temp.nz*temp.nz)) * fTILE_SIZE;
			}
		}
	
		m_ppObjectSizeRegion = new TILEINDEX*[MAX_ObjectSize];
		for(n=0;n<MAX_ObjectSize;++n)
		{
			m_ppObjectSizeRegion[n] = new TILEINDEX[TM_ObjectRegionNum[n]];
			for(int t=0;t<TM_ObjectRegionNum[n];++t)
			{
				m_ppObjectSizeRegion[n][t].nx = TM_ObjectSizeTiles[2*t + 0];
				m_ppObjectSizeRegion[n][t].nz = TM_ObjectSizeTiles[2*t + 1];
			}
		}*/
	
}

CTileManager::~CTileManager()
{
/*	SAFE_DELETE_ARRAY(m_pTile);
	for(int n=0;n<MAX_ObjectSize;++n)
	{
		delete [] m_ppObjectSizeRegion[n];
	}
	delete [] m_ppObjectSizeRegion;
*/
	CTileGroup* pInfo; 
	m_TileGroupTable.SetPositionHead();
	while(pInfo = m_TileGroupTable.GetData())
	{
		delete	pInfo;
	}
	m_TileGroupTable.RemoveAll();

	CFixedTileInfo* pData = NULL;
	m_FixedTileInfoTable.SetPositionHead();
	while(pData = m_FixedTileInfoTable.GetData())
	{
		delete pData;
	}
	m_FixedTileInfoTable.RemoveAll();
}
/*

TILEINDEX CTileManager::GetTileIndex(float fx, float fz)
{
	TILEINDEX rt;
	rt.nz = int(fz / fTILE_SIZE);
	rt.nx = int(fx / fTILE_SIZE);

	if( rt.nx > m_nTileWidth || 
		rt.nz > m_nTileHeight ||
		rt.nx < 0 || rt.nz < 0 )
	{
		//ASSERTMSG(0,"좌표가 이상합니다[CTileManager]");
		rt.nx = 0;
		rt.nz = 0;
		return rt;
	}

	return rt;
}*/


/*
CTile* CTileManager::GetTile(TILEINDEX* pIndex)
{
	return &m_pTile[pIndex->nz * m_nTileWidth + pIndex->nx];
}

CTile* CTileManager::GetTile(float fx, float fz)
{

	if(!(fx >= 0 && fz >= 0))
		return NULL;

	return GetTile(&GetTileIndex(fx,fz));

}

CTile* CTileManager::GetTile(int x, int z)
{
	return &m_pTile[z * m_nTileWidth + x];
}
*/

BOOL CTileManager::LoadFixedTileInfo(MAPTYPE MapNum, char* pMapFile)
{	
	CMHFile file;
	char filename[256];
#ifdef _FILE_BIN_
	sprintf(filename, "MurimMapInfo%d.txt", MapNum);
#else
	sprintf(filename, "MurimMapInfo%d.txt", MapNum);
#endif
	if(file.Init(filename, "r", 1) == FALSE)
	{
		ReadMapFile(MapNum, pMapFile);
		return TRUE;
	}

	
	else
	{
		char Mapfile[64];
		while(1)
		{
			if(file.IsEOF() != FALSE)
				break;
			
			MapNum = file.GetWord();
			strcpy(Mapfile, g_pServerSystem->GetMap()->GetTileName(MapNum));
			ReadMapFile(MapNum, Mapfile);			
		}
	}
	
	file.Release();

/*	hFile = CreateFile(TileFile,GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		m_nTileWidth = m_nTileHeight = 1024;
		m_pTile = new CTile[ m_nTileWidth * m_nTileHeight ];
		
		CTile *pTile;
		TILE_ATTR Attr;
		Attr.uFixedAttr = 0;
		for(int i=0; i< (m_nTileHeight * m_nTileWidth); i++)
		{
			pTile = m_pTile + i;			
			pTile->InitTileAttrib(Attr);
		}
		
		return FALSE;
	}


	//속성파일 헤더 읽어드림...
	bResult = ReadFile(hFile, &m_nTileWidth, sizeof(m_nTileWidth), &dwRead, NULL);
	ASSERT(bResult);
	bResult = ReadFile(hFile, &m_nTileHeight, sizeof(m_nTileHeight), &dwRead, NULL);
	ASSERT(bResult);
	
	m_pTile = new CTile[ m_nTileWidth * m_nTileHeight ];

	//속성 정보 읽어드림 
	CTile *pTile;
	TILE_ATTR Attr;
	for(int i=0; i< (m_nTileHeight * m_nTileWidth); i++)
	{
		pTile = m_pTile + i;
		bResult = ReadFile(hFile, &Attr , sizeof(FIXEDATTR), &dwRead, NULL);
				
		ASSERT(bResult);		
		ASSERT(!(bResult && dwRead == 0 ));

		pTile->InitTileAttrib(Attr);
	}

	CloseHandle(hFile);
*/
	return TRUE; 
}

void CTileManager::ReadMapFile(MAPTYPE MapNum, char* filename)
{
	HANDLE	hFile = NULL;
	DWORD dwRead = 0;
	BYTE bResult = 0;

	int nTileWidth;
	int nTileHeight;

	hFile = CreateFile(filename,GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		nTileWidth = nTileHeight = 1024;
		CFixedTileInfo* pFixedTileInfo = new CFixedTileInfo;
		pFixedTileInfo->Init(nTileWidth, nTileHeight);
		
		CFixedTile *pTile;
		FIXEDTILE_ATTR Attr;
		Attr.uFixedAttr = 0;
		for(int i=0; i< (nTileHeight * nTileWidth); i++)
		{
			pTile = pFixedTileInfo->GetFixedTile() + i;
			pTile->InitTileAttrib(Attr);
		}
		
		CloseHandle(hFile);
		m_FixedTileInfoTable.Add(pFixedTileInfo, MapNum);
		return;
	}
	
	//속성파일 헤더 읽어드림...
	bResult = ReadFile(hFile, &nTileWidth, sizeof(nTileWidth), &dwRead, NULL);
	ASSERT(bResult);
	bResult = ReadFile(hFile, &nTileHeight, sizeof(nTileHeight), &dwRead, NULL);
	ASSERT(bResult);
	
	CFixedTileInfo* pFixedTileInfo = new CFixedTileInfo;
	pFixedTileInfo->Init(nTileWidth, nTileHeight);
	//속성 정보 읽어드림 
	CFixedTile *pTile;
	FIXEDTILE_ATTR Attr;
	for(int i=0; i< (nTileHeight * nTileWidth); i++)
	{
		pTile = pFixedTileInfo->GetFixedTile() + i;
		bResult = ReadFile(hFile, &Attr , sizeof(FIXEDATTR), &dwRead, NULL);
		
		ASSERT(bResult);		
		ASSERT(!(bResult && dwRead == 0 ));
		
		pTile->InitTileAttrib(Attr);
	}
	
	CloseHandle(hFile);
	m_FixedTileInfoTable.Add(pFixedTileInfo, MapNum);
}

BOOL CTileManager::CreateTileGroup(DWORD TileGroupID, MAPTYPE MapNum, float fStartX, float fStartZ, float fWidth, float fHeight)
{
	CFixedTileInfo* pFixedTileInfo = GetFixedTileInfo(MapNum);
	if(!pFixedTileInfo)
	{
		ASSERT(0);
		return FALSE;
	}

	CTileGroup* pInfo;
	pInfo = new CTileGroup;
	pInfo->Init(pFixedTileInfo, TileGroupID, MapNum, fStartX, fStartZ, fWidth, fHeight);
	m_TileGroupTable.Add(pInfo, TileGroupID);
	
	return TRUE;
}

void CTileManager::DeleteTileGroup(DWORD TileGroupID)
{
	CTileGroup* pTileGroup = GetTileGroup(TileGroupID);
	ASSERT(pTileGroup);
	//타일 지울때 쩌구쩌구
	m_TileGroupTable.Remove(TileGroupID);
	delete pTileGroup;
}
CTileGroup* CTileManager::GetTileGroup(CObject* pObject)
{
	DWORD TGID = pObject->GetGridID();

	CTileGroup* pInfo = m_TileGroupTable.GetData(TGID);
//	ASSERT(pInfo);

	// 임시
	if(pInfo == NULL)
		pInfo = m_TileGroupTable.GetData((DWORD)0);

	return pInfo;
}
CTileGroup* CTileManager::GetTileGroup(DWORD TileGroupID)
{
	CTileGroup* pInfo = m_TileGroupTable.GetData(TileGroupID);
//	ASSERT(pInfo);
	
	// 임시
	if(pInfo == NULL)
		pInfo = m_TileGroupTable.GetData((DWORD)0);

	return pInfo;
}

CFixedTileInfo* CTileManager::GetFixedTileInfo(MAPTYPE MapNum)
{
	CFixedTileInfo* pInfo = m_FixedTileInfoTable.GetData(MapNum);

	if(pInfo == NULL)
	{			
		char filename[256];
//		sprintf(filename,"resource/%d.ttb",MapNum);
		sprintf(filename,"Resource/Map/%d.ttb",MapNum);
		LoadFixedTileInfo(MapNum,filename);
	}

	pInfo = m_FixedTileInfoTable.GetData(MapNum);

	ASSERT(pInfo);
	return pInfo;
}
CFixedTileInfo* CTileManager::GetFixedTileInfo(CObject* pObject)
{
	CTileGroup* pTileGroup = GetTileGroup(pObject);
	if(pTileGroup == NULL)
	{
		m_FixedTileInfoTable.SetPositionHead();
		return m_FixedTileInfoTable.GetData();
	}
	
	ASSERT(pTileGroup);
	CFixedTileInfo* pInfo = pTileGroup->GetFixedTileInfo();
	ASSERT(pInfo);
	return pInfo;
}

int CTileManager::GetTileWidth(MAPTYPE MapNum)
{
	CFixedTileInfo* pInfo = GetFixedTileInfo(MapNum);
	return pInfo->GetTileWidth();
}
BOOL CTileManager::IsInTile(int cellX, int cellY, CObject* pObject)
{
	CFixedTileInfo* pInfo = GetFixedTileInfo(pObject);
	return pInfo->IsInTile(cellX, cellY);
}
BOOL CTileManager::CollisionTileWithTileIndex( int x, int y, CObject* pObject)
{ 
	CFixedTileInfo* pInfo = GetFixedTileInfo(pObject);
	CFixedTile *pTile = pInfo->GetFixedTile(x, y);
	if(pTile == NULL)
		return TRUE;
	return pTile->IsCollisonTile();
}
BOOL CTileManager::CollisionTileWithTileIndex( int x, int y, MAPTYPE MapNum)
{
	CFixedTileInfo* pInfo = GetFixedTileInfo(MapNum);
	CFixedTile *pTile = pInfo->GetFixedTile(x, y);
	if(pTile == NULL)
		return TRUE;
	return pTile->IsCollisonTile();	
}
BOOL CTileManager::CollisionTileWithPosition( float fx, float fy, CObject* pObject)
{ 
	int x = int(fx / TILECOLLISON_DETAIL);
	int y = int(fy / TILECOLLISON_DETAIL);
	return CollisionTileWithTileIndex(x,y,pObject);
}
BOOL CTileManager::CollisionTileWithPosition( float fx, float fy, MAPTYPE MapNum)
{
	int x = int(fx / TILECOLLISON_DETAIL);
	int y = int(fy / TILECOLLISON_DETAIL);
	return CollisionTileWithTileIndex(x,y,MapNum);
}
BOOL CTileManager::CollisionLine(VECTOR3* pSrc,VECTOR3* pDest, VECTOR3 * pTarget, CObject* pObject)
{
	int x1 = int(pSrc->x / TILECOLLISON_DETAIL);
	int y1 = int(pSrc->z / TILECOLLISON_DETAIL);
	int x2 = int(pDest->x / TILECOLLISON_DETAIL);
	int y2 = int(pDest->z / TILECOLLISON_DETAIL);

	int dx = x2 - x1;
	int dy = y2 - y1;
	
	int absDX = abs(dx);
	int absDY = abs(dy);
	int MaxDelta = max(absDX, absDY);
	
	int CellX = x1;
	int CellY = y1;
	int x = 0;
	int y = 0;
	CFixedTileInfo* pFixedTile = GetFixedTileInfo(pObject);

	int signDX = sign(dx);
	int signDY = sign(dy);

	int PrevCellX = 0;
	int PrevCellY = 0;
	
	for( int i = 0 ; i <= MaxDelta ; ++i )
	{
		PrevCellX = CellX;
		PrevCellY = CellY;

		x += absDX;  
		y += absDY;
		
		if( x > MaxDelta)  
		{  
			x -= MaxDelta;
			CellX += signDX;
		}
		
		if( y > MaxDelta ) 
		{  
			y -= MaxDelta;  
			CellY += signDY;  
		}
		CFixedTile *pTile = pFixedTile->GetFixedTile(CellX, CellY);
		ASSERT(pTile);
		if(pTile == NULL)
		{
			if(pTarget)
			{
				pTarget->x = PrevCellX*TILECOLLISON_DETAIL;
				pTarget->y = 0;
				pTarget->z = PrevCellY*TILECOLLISON_DETAIL;
			}
			return TRUE;
		}
		if(pTile->IsCollisonTile())
		{
			if(pTarget)
			{
				pTarget->x = PrevCellX*TILECOLLISON_DETAIL;
				pTarget->y = 0;
				pTarget->z = PrevCellY*TILECOLLISON_DETAIL;
			}
			return TRUE;
		}
	}
	if(pTarget)
	{
		pTarget->x = x2*TILECOLLISON_DETAIL;
		pTarget->y = 0;
		pTarget->z = y2*TILECOLLISON_DETAIL;
	}

	return FALSE;
}
BOOL CTileManager::CollisionCheck(VECTOR3* pStart,VECTOR3* pEnd,VECTOR3* pRtCollisonPos, CObject* pObject)
{	
	if( pStart->x >= 51200 || pStart->x < 0 ||
		pStart->z >= 51200 || pStart->z < 0 )
	{
//		ASSERTMSG(0,"이동영역을 벗어났습니다.");
		pRtCollisonPos->x = 25000;
		pRtCollisonPos->z = 25000;
		return TRUE;
	}

	int x0 = int(pStart->x / TILECOLLISON_DETAIL);
	int y0 = int(pStart->z / TILECOLLISON_DETAIL);
	int x1 = int(pEnd->x / TILECOLLISON_DETAIL);
	int y1 = int(pEnd->z / TILECOLLISON_DETAIL);
	
	int dx = x1-x0,dy = y1-y0;
	int sx,sy;
	if(dx >= 0)
	{
		sx = 1;
	}
	else
	{
		sx = -1;
		dx *= -1;
	}
	if(dy >= 0)
	{
		sy = 1;
	}
	else
	{
		sy = -1;
		dy *= -1;
	}
	
	int ax = 2*dx,ay = 2*dy;

	int x = x0, y = y0;
	int lastx = x, lasty = y;
	
	BOOL bFirst = TRUE;
	if(dx == 0 && dy == 0)
		bFirst = FALSE;
	
	CFixedTileInfo* pFixedTile = GetFixedTileInfo(pObject);

	if(dx >= dy)
	{
		for(int desc = ay - dx; ;x += sx,desc += ay)
		{			
			if(bFirst == TRUE)
			{
				bFirst = FALSE;
				continue;
			}

			CFixedTile *pTile = pFixedTile->GetFixedTile(x, y);
			ASSERT(pTile);
			if(pTile == NULL)
			{
				if(pRtCollisonPos)
				{					
					pRtCollisonPos->x = lastx*TILECOLLISON_DETAIL;
					pRtCollisonPos->y = 0;
					pRtCollisonPos->z = lasty*TILECOLLISON_DETAIL;
				}
				return TRUE;
			}

			if(pTile->IsCollisonTile())
			{
				if(pRtCollisonPos)
				{					
					pRtCollisonPos->x = lastx*TILECOLLISON_DETAIL;
					pRtCollisonPos->y = 0;
					pRtCollisonPos->z = lasty*TILECOLLISON_DETAIL;
				}
				return TRUE;
			}

			if(x == x1)
				break;

			lastx = x;
			lasty = y;

			if(desc > 0)
			{
				y += sy;
				desc -= ax;
			}
		}
	}
	else
	{
		for(int desc = ax - dy; ;y += sy,desc += ax)
		{
			if(bFirst == TRUE)
			{
				bFirst = FALSE;
				continue;
			}

			CFixedTile *pTile = pFixedTile->GetFixedTile(x, y);
			ASSERT(pTile);
			if(pTile == NULL)
			{
				if(pRtCollisonPos)
				{
					pRtCollisonPos->x = lastx*TILECOLLISON_DETAIL;
					pRtCollisonPos->y = 0;
					pRtCollisonPos->z = lasty*TILECOLLISON_DETAIL;
				}
				return TRUE;
			}

			if(pTile->IsCollisonTile())
			{
				if(pRtCollisonPos)
				{
					pRtCollisonPos->x = lastx*TILECOLLISON_DETAIL;
					pRtCollisonPos->y = 0;
					pRtCollisonPos->z = lasty*TILECOLLISON_DETAIL;
				}
				return TRUE;
			}

			if(y == y1)
				break;
			
			lastx = x;
			lasty = y;

			if(desc > 0)
			{
				x += sx;
				desc -= ay;
			}
		}
	}
	return FALSE;
}

/*
BOOL CTileManager::CollisionCheck(VECTOR3* pStart,VECTOR3* pEnd,VECTOR3* pRtCollisonPos)
{
	if( pStart->x >= 51200 || pStart->x < 0 ||
		pStart->z >= 51200 || pStart->z < 0 )
	{
		ASSERTMSG(0,"이동영역을 벗어났습니다.");
		pRtCollisonPos->x = 25000;
		pRtCollisonPos->z = 25000;
		return TRUE;
	}
	if(	pEnd->x >= 51200 || pEnd->x < 0 ||
		pEnd->z >= 51200 || pEnd->z < 0 )
	{
		if(pRtCollisonPos)
		{
			if(pRtCollisonPos->x < 10)
				pRtCollisonPos->x = 10;
			if(pRtCollisonPos->z < 10)
				pRtCollisonPos->z = 10;
			if(pRtCollisonPos->x > 51200)
				pRtCollisonPos->x = 51200;
			if(pRtCollisonPos->z > 51200)
				pRtCollisonPos->z = 51200;
		}
		return TRUE;
	}

	VECTOR3 nor;
	VECTOR3 curpos = *pStart,lastpos;
	nor = *pEnd - *pStart;
	float dist = GetLength(&nor);
	if(dist != 0)
		nor = nor / dist;

	for(float a = 0.5f; ; a+= TILECOLLISON_DETAIL)
	{
		if(a > dist)
			a = dist;

		lastpos = curpos;
		curpos = nor * a;
		curpos = curpos + *pStart;

		CTile *pTile = GetTile(curpos.x, curpos.z);
		ASSERT(pTile);

		if(pTile->IsCollisonTile())
		{
			if(pRtCollisonPos)
			*pRtCollisonPos = lastpos;
			return TRUE;
		}

		if(a==dist)
			break;
	}

	return FALSE;
}
*/

void CTileManager::AddObject(CObject* pObject)
{
	CTileGroup* pTileGroup = GetTileGroup(pObject);
	if(pTileGroup == NULL)
		return;
	pTileGroup->AddObject(pObject);

/*	int size = pObject->GetObjectTileSize();
	VECTOR3* pPos = CCharMove::GetPosition(pObject);
	
	TILEINDEX FirstTileIndex = GetTileIndex(pPos->x,pPos->z);
		TILEINDEX CurTileIndex;
	
	pObject->SetPreoccupiedTileIndex(FirstTileIndex);
		
	
	CTile* pTile;
	for(int n=0;n<TM_ObjectRegionNum[size];++n)
	{
		CurTileIndex = FirstTileIndex + m_ppObjectSizeRegion[size][n];
		pTile = pTileGroup->GetTile(&CurTileIndex);
		pTile->IncreasePreoccupied();
	}*/
}

void CTileManager::RemoveObject(CObject* pObject)
{
	if(pObject->GetInitedGrid() == FALSE)
		return;

	CTileGroup* pTileGroup = GetTileGroup(pObject);
	if(pTileGroup == NULL)
		return;
	pTileGroup->RemoveObject(pObject);

	/*
	int size = pObject->GetObjectTileSize();
	TILEINDEX FirstTileIndex = pObject->GetPreoccupiedTileIndex();
	TILEINDEX CurTileIndex;
	
	pTileGroup->RemoveObject(size, FirstTileIndex);
	
	CTile* pTile;
		for(int n=0;n<TM_ObjectRegionNum[size];++n)
		{
			CurTileIndex = FirstTileIndex + m_ppObjectSizeRegion[size][n];
			pTile = GetTile(&CurTileIndex);
			pTile->DecreasePreoccupied();
		}*/
}

BOOL CTileManager::IsPreoccupied(CObject* pObject,VECTOR3* pPos)
{
	CTileGroup* pTileGroup = GetTileGroup(pObject);
	if(pTileGroup == NULL)
		return FALSE;
	return pTileGroup->IsPreoccupied(pObject, pPos);

	/*
	int size = pObject->GetObjectTileSize();
		TILEINDEX FirstTileIndex = GetTileIndex(pPos->x,pPos->z);
		return IsPreoccupied(size,&FirstTileIndex,pObject);*/
}

/*
BOOL CTileManager::IsPreoccupied(int size,TILEINDEX* pCenterTile,CObject* pObject)
{
	BOOL rt = FALSE;
	CTile* pTile;
	TILEINDEX CurTileIndex,ObjectCurTilePos;

	if(pObject)
	{
		ObjectCurTilePos = pObject->GetPreoccupiedTileIndex();
		
		// 자리를 뱄다가
		for(int n=0;n<TM_ObjectRegionNum[size];++n)
		{
			CurTileIndex = ObjectCurTilePos + m_ppObjectSizeRegion[size][n];
			pTile = GetTile(&CurTileIndex);	
			pTile->DecreasePreoccupied();
		}
	}

	// 검사를 하고
	for(int n=0;n<TM_ObjectRegionNum[size];++n)
	{
		CurTileIndex = *pCenterTile + m_ppObjectSizeRegion[size][n];
		pTile = GetTile(&CurTileIndex);	
		if(pTile->GetPreoccupied() > 0 || pTile->IsCollisonTile() == TRUE)
		{
			rt = TRUE;
			break;
		}
	}

	if(pObject)
	{
		// 다시 채워넣는다.
		for(int n=0;n<TM_ObjectRegionNum[size];++n)
		{
			CurTileIndex = ObjectCurTilePos + m_ppObjectSizeRegion[size][n];
			pTile = GetTile(&CurTileIndex);	
			pTile->IncreasePreoccupied();
		}
	}
	return rt;
}*/


int CTileManager::GetPreoccupiedNum(CObject* pObject,VECTOR3* pPos)
{
	CTileGroup* pTileGroup = GetTileGroup(pObject);
	if(pTileGroup == NULL)
		return 0;
	return pTileGroup->GetPreoccupiedNum(pObject, pPos);

	/*
	int size = pObject->GetObjectTileSize();
		TILEINDEX FirstTileIndex = GetTileIndex(pPos->x,pPos->z);
		TILEINDEX CurTileIndex;
	
		int preo = 0;
		CTile* pTile;
		for(int n=0;n<TM_ObjectRegionNum[size];++n)
		{
			CurTileIndex = FirstTileIndex + m_ppObjectSizeRegion[size][n];
			pTile = GetTile(&CurTileIndex);	
			
			if(preo < pTile->GetPreoccupied())
				preo = pTile->GetPreoccupied();
		}
		return preo;*/
	
}

BOOL CTileManager::Preoccupy(CObject* pObject,VECTOR3* pPos)
{
	CTileGroup* pTileGroup = GetTileGroup(pObject);
	if(pTileGroup == NULL)
		return FALSE;
	return pTileGroup->Preoccupy(pObject, pPos);

/*
	int size = pObject->GetObjectTileSize();
	TILEINDEX LastTileIndex = pObject->GetPreoccupiedTileIndex();
	TILEINDEX ToGoTileIndex = GetTileIndex(pPos->x,pPos->z);
	if(LastTileIndex == ToGoTileIndex)
		return TRUE;
	
	pObject->SetPreoccupiedTileIndex(ToGoTileIndex);

	CTile* pLastTile,*pTogoTile;
	TILEINDEX CurTileIndex;
	for(int n=0;n<TM_ObjectRegionNum[size];++n)
	{
		CurTileIndex = LastTileIndex + m_ppObjectSizeRegion[size][n];
		pLastTile = GetTile(&CurTileIndex);
		CurTileIndex = ToGoTileIndex + m_ppObjectSizeRegion[size][n];
		pTogoTile = GetTile(&CurTileIndex);

		pLastTile->DecreasePreoccupied();
		pTogoTile->IncreasePreoccupied();
	}
	return TRUE;*/
}

VECTOR3 CTileManager::GetNextCloserPos(CObject* pObject,VECTOR3* pTargetPos,VECTOR3* pCurPos,float closestDist,float limitDist)
{
	CTileGroup* pTileGroup = GetTileGroup(pObject);
	if(pTileGroup == NULL)
	{
		return *pCurPos;
	}
	VECTOR3 pos = pTileGroup->GetNextCloserPos(pObject,pTargetPos,pCurPos,closestDist,limitDist);
	
	ASSERT(!(pos.x == 0 && pos.z == 0));
	
	return pos;
	/*
	int size = pObject->GetObjectTileSize();
		//////////////////////////////////////////////////////////////////////////
		// 방향 구하기
		int dir;
		VECTOR3 vdir =  *pCurPos - *pTargetPos;
		float angle = RADTODEG(VECTORTORAD(vdir));
		angle += 45;
		if(angle >= 360) angle -= 360;
		dir = int(angle / 90);
		ASSERT(dir >= 0 && dir < eDIR_Max);
		
		//////////////////////////////////////////////////////////////////////////	
		TILEINDEX EmptyTile;
		TILEINDEX TargetIndex = GetTileIndex(pTargetPos->x,pTargetPos->z);
	
		for(int n=0;n<MAX_ADDPos;++n)
		{
			EmptyTile = m_AddPos[dir][n] + TargetIndex;
			
			if(m_AddDistance[n] < closestDist)
			{
				continue;
			}
			if(m_AddDistance[n] > limitDist)
			{
				n = MAX_ADDPos - 1;			// 거리가 멀면 그냥 마지막으로 넘겨버림
				EmptyTile = m_AddPos[dir][n] + TargetIndex;
			}
			
			if(IsPreoccupied(size,&EmptyTile) == FALSE)
			{
				break;
			}
		}
		
		//////////////////////////////////////////////////////////////////////////
		// 50cm 타일의 중앙에 서게 한다.
		float TileWidth = 50;
		VECTOR3 EmptyPos;
		EmptyPos.x = EmptyTile.nx * TileWidth + TileWidth * 0.5f;
		EmptyPos.y = 0;
		EmptyPos.z = EmptyTile.nz * TileWidth + TileWidth * 0.5f;
	
			
		return EmptyPos;*/
}



void CTileManager::AddTileAttrByAreaData(CObject* pObject,VECTOR3* pPos,AREATILE areatile)
{
	CTileGroup* pTileGroup = GetTileGroup(pObject);
	if(pTileGroup == NULL)
		return;
	pTileGroup->AddTileAttrib(pPos,areatile);
}

void CTileManager::RemoveTileAttrByAreaData(CObject* pObject,VECTOR3* pPos,AREATILE areatile)
{
	CTileGroup* pTileGroup = GetTileGroup(pObject);
	if(pTileGroup == NULL)
		return;
	pTileGroup->RemoveTileAttrib(pPos,areatile);
}