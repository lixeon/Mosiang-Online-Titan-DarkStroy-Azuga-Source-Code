// TileGroup.cpp: implementation of the CTileGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TileGroup.h"
#include "TileManager.h"
#include "FixedTileInfo.h"
#include "Tile.h"
#include "FixedTile.h"
#include "CharMove.h"
#include "Object.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
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

CTileGroup::CTileGroup()
{
	m_ppObjectSizeRegion = NULL;
	m_ID = 0;
	m_MapNum = 0;

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
	}
}

CTileGroup::~CTileGroup()
{
	SAFE_DELETE_ARRAY(m_pTile);
	for(int n=0;n<MAX_ObjectSize;++n)
	{
		delete [] m_ppObjectSizeRegion[n];
	}
	delete [] m_ppObjectSizeRegion;
}

void CTileGroup::Init(CFixedTileInfo* pFixedTileInfo, DWORD TileGroupID, MAPTYPE MapNum, float fStartX, float fStartZ, float fWidth, float fHeight)
{
	m_fStartX = fStartX;
	m_fStartZ = fStartZ;
	
	m_nTileWidth = (int)ceil(fWidth / fTILE_SIZE); 
	m_nTileHeight = (int)ceil(fHeight / fTILE_SIZE);
	
	m_ID = TileGroupID;
	m_MapNum = MapNum;

	m_pFixedTileInfo = pFixedTileInfo;

	m_pTile = new CTile[m_nTileWidth*m_nTileHeight];
}

TILEINDEX CTileGroup::GetTileIndex(float fx, float fz)
{
	TILEINDEX rt;
	rt.nz = int((fz-m_fStartZ) / fTILE_SIZE);
	rt.nx = int((fx-m_fStartX) / fTILE_SIZE);

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
}

CTile* CTileGroup::GetTile(TILEINDEX* pIndex)
{
	if( pIndex->nx < 0 ||
		pIndex->nz < 0 || 
		pIndex->nx >= m_nTileWidth ||
		pIndex->nz >= m_nTileHeight)
		return NULL;

	return &m_pTile[pIndex->nz * m_nTileWidth + pIndex->nx];
}

void CTileGroup::AddObject(CObject* pObject)
{
	int size = pObject->GetObjectTileSize();
	VECTOR3* pPos = CCharMove::GetPosition(pObject);

	TILEINDEX FirstTileIndex = GetTileIndex(pPos->x,pPos->z);
	TILEINDEX CurTileIndex;

	pObject->SetPreoccupiedTileIndex(FirstTileIndex);
	
	CTile* pTile;
	for(int n=0;n<TM_ObjectRegionNum[size];++n)
	{
		CurTileIndex = FirstTileIndex + m_ppObjectSizeRegion[size][n];
		pTile = GetTile(&CurTileIndex);
		ASSERT(pTile);
		if(pTile)
			pTile->IncreasePreoccupied();
	}
}

void CTileGroup::RemoveObject(CObject* pObject)
{
	int size = pObject->GetObjectTileSize();
	TILEINDEX FirstTileIndex = pObject->GetPreoccupiedTileIndex();
	TILEINDEX CurTileIndex;
	
	CTile* pTile;
	for(int n=0;n<TM_ObjectRegionNum[size];++n)
	{
		CurTileIndex = FirstTileIndex + m_ppObjectSizeRegion[size][n];
		pTile = GetTile(&CurTileIndex);
		ASSERT(pTile);
		if(pTile)
			pTile->DecreasePreoccupied();
	}
}

BOOL CTileGroup::IsPreoccupied(CObject* pObject, VECTOR3* pPos)
{
	int size = pObject->GetObjectTileSize();
	TILEINDEX FirstTileIndex = GetTileIndex(pPos->x,pPos->z);
	return IsPreoccupied(size,&FirstTileIndex,pObject);
}


BOOL CTileGroup::IsPreoccupied(int size,TILEINDEX* pCenterTile,CObject* pObject)
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
			ASSERT(pTile);
			if(pTile)
				pTile->DecreasePreoccupied();
		}
	}

	// 검사를 하고
	for(int n=0;n<TM_ObjectRegionNum[size];++n)
	{
		CurTileIndex = *pCenterTile + m_ppObjectSizeRegion[size][n];
		pTile = GetTile(&CurTileIndex);	
		CFixedTile* pCFixedTile = m_pFixedTileInfo->GetFixedTile(&CurTileIndex);

		if(!pCFixedTile || !pTile)
		{
			rt = TRUE;
			break;
		}

		if(pTile->GetPreoccupied() > 0 || pCFixedTile->IsCollisonTile() == TRUE)
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
			ASSERT(pTile);
			if(pTile)
				pTile->IncreasePreoccupied();
		}
	}
	return rt;
}

BOOL CTileGroup::Preoccupy(CObject* pObject,VECTOR3* pPos)
{
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

		if(pLastTile)
			pLastTile->DecreasePreoccupied();
		else
		{
			char buf[128] = {0,};
			wsprintf( buf, "ObjectKind:%d, ObjectID:%d, x:%d, z:%d",
				pObject->GetObjectKind(), pObject->GetID(), CurTileIndex.nx, CurTileIndex.nz );

			ASSERTMSG( pLastTile, buf );
		}

		CurTileIndex = ToGoTileIndex + m_ppObjectSizeRegion[size][n];
		pTogoTile = GetTile(&CurTileIndex);

		ASSERT(pTogoTile);
		if(pTogoTile)
			pTogoTile->IncreasePreoccupied();
	}
	return TRUE;
}


VECTOR3	CTileGroup::GetNextCloserPos(CObject* pObject,VECTOR3* pTargetPos,VECTOR3* pCurPos,float closestDist,float limitDist)
{
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

		
	return EmptyPos;
}

int CTileGroup::GetPreoccupiedNum(CObject* pObject,VECTOR3* pPos)
{
	int size = pObject->GetObjectTileSize();
	TILEINDEX FirstTileIndex = GetTileIndex(pPos->x,pPos->z);
	TILEINDEX CurTileIndex;

	int preo = 0;
	CTile* pTile;
	for(int n=0;n<TM_ObjectRegionNum[size];++n)
	{
		CurTileIndex = FirstTileIndex + m_ppObjectSizeRegion[size][n];
		pTile = GetTile(&CurTileIndex);	
		ASSERT(pTile);
		if(pTile)
		{
			if(preo < pTile->GetPreoccupied())
				preo = pTile->GetPreoccupied();			
		}
	}
	return preo;
}

void CTileGroup::AddTileAttrib(VECTOR3* pPos,AREATILE areatile)
{
}
void CTileGroup::RemoveTileAttrib(VECTOR3* pPos,AREATILE areatile)
{
}
