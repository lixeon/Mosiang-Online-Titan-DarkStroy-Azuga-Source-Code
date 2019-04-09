// FixedTileInfo.cpp: implementation of the CFixedTileInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FixedTileInfo.h"
#include "FixedTile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFixedTileInfo::CFixedTileInfo()
{
}

CFixedTileInfo::~CFixedTileInfo()
{
	SAFE_DELETE_ARRAY(m_pFixedTile);
}

void CFixedTileInfo::Init(int fWidth, int fHeight)
{
	m_nTileWidth = fWidth;
	m_nTileHeight = fHeight;
	m_pFixedTile = new CFixedTile[m_nTileHeight*m_nTileWidth];
}
/*
CFixedTile* CFixedTileInfo::GetTile(TILEINDEX* pIndex)
{
	return &m_pTile[pIndex->nz * m_nTileWidth + pIndex->nx];
}


CFixedTile* CFixedTileInfo::GetTile(float fx, float fz)
{

	if(!(fx >= 0 && fz >= 0))
		return NULL;

	return GetTile(&GetTileIndex(fx,fz));

}
*/


CFixedTile* CFixedTileInfo::GetFixedTile(int x, int z)
{
	if( x < 0 ||
		z < 0 ||
		x >= m_nTileWidth ||
		z >= m_nTileHeight)
		return NULL;

	return &m_pFixedTile[z * m_nTileWidth + x];
}

CFixedTile* CFixedTileInfo::GetFixedTile(TILEINDEX* pIndex)
{
	if( pIndex->nx < 0 ||
		pIndex->nz < 0 || 
		pIndex->nx >= m_nTileWidth ||
		pIndex->nz >= m_nTileHeight)
		return NULL;

	return &m_pFixedTile[pIndex->nz * m_nTileWidth + pIndex->nx];
}

/*
TILEINDEX CFixedTileInfo::GetTileIndex(float fx, float fz)
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
}

  */