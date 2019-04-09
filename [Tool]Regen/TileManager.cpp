// TileManager.cpp: implementation of the CTileManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TileManager.h"
#include "DefineStruct.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define fTILE_WIDTH 50
#define fTILE_HEIGHT 50
#define TILECOLLISON_DETAIL		50.f	// Á¤È®µµ.. ÀÛ°Ô ÇÒ¼ö·Ï ´À¸®Áö¸¸ Á¤È®ÇÑ Ã¼Å©¸¦ ÇÒ¼ö ÀÖµû

CTileManager::CTileManager()
{
	m_pTile = NULL;
}

CTileManager::~CTileManager()
{
	SAFE_DELETE_ARRAY(m_pTile);
}

CTile* CTileManager::GetTile(DWORD dwX, DWORD dwZ)
{
	if(!(dwX < m_dwTileWidth && dwZ < m_dwTileHeight))
		return NULL;

	return (m_pTile + (m_dwTileHeight * dwZ + dwX));
}

CTile* CTileManager::GetTile(float fx, float fz)
{
	if(!(fx > 0 && fz > 0))
		return NULL;	

	DWORD z = DWORD(fz / fTILE_WIDTH);
	DWORD x = DWORD(fx / fTILE_HEIGHT);

	return GetTile(x, z);
}

BOOL CTileManager::LoadTileInfo(char* TileFile)
{
	//¸Ê ¼Ó¼º ÆÄÀÏ ÀÐ¾îµå¸²...
	HANDLE	hFile = NULL;
	DWORD dwRead = 0;
	BYTE bResult = 0;

	hFile = CreateFile(TileFile,GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == INVALID_HANDLE_VALUE)
		return FALSE;


	//¼Ó¼ºÆÄÀÏ Çì´õ ÀÐ¾îµå¸²...
	bResult = ReadFile(hFile, &m_dwTileWidth, sizeof(m_dwTileWidth), &dwRead, NULL);
	ASSERT(bResult);
	bResult = ReadFile(hFile, &m_dwTileHeight, sizeof(m_dwTileHeight), &dwRead, NULL);
	ASSERT(bResult);
	
	m_pTile = new CTile[ m_dwTileWidth * m_dwTileHeight ];

	//¼Ó¼º Á¤º¸ ÀÐ¾îµå¸² 
	CTile *pTile;
	TILE_ATTR Attr;
	for(DWORD i=0; i< (m_dwTileHeight * m_dwTileWidth); i++)
	{
		pTile = m_pTile + i;
		bResult = ReadFile(hFile, &Attr , sizeof(TILE_ATTR), &dwRead, NULL);
				
		ASSERT(bResult);		
		ASSERT(!(bResult && dwRead == 0 ));

		pTile->SetTileAttrib(Attr);
	}

	CloseHandle(hFile);

	return TRUE;
}

BOOL CTileManager::CollisonCheck(VECTOR3* pStart,VECTOR3* pEnd,VECTOR3* pRtCollisonPos)
{
	VECTOR3 nor;
	VECTOR3 curpos = *pStart,lastpos;
	VECTOR3_SUB_VECTOR3(&nor,pEnd,pStart);
	float dist = VECTOR3Length(&nor);
	if(dist != 0)
		nor = nor / dist;
	
	for(float a = 0.5f; ; a+= TILECOLLISON_DETAIL)
	{
		if(a > dist)
			a = dist;

		lastpos = curpos;
		curpos = nor * a;
		VECTOR3_ADD_VECTOR3(&curpos,&curpos,pStart);

		CTile *pTile = GetTile(curpos.x, curpos.z);
		ASSERT(pTile);

		if(pTile->IsCollisonTile())
		{
			*pRtCollisonPos = lastpos;
			return TRUE;
		}

		if(a==dist)
			break;
	}

	return FALSE;

}

void CTileManager::AddTileAttrByAreaData(VECTOR3* pPos,AREATILE areatile)
{
	CTile *pTile = GetTile(pPos->x, pPos->z);
	if(pTile == 0)
	{
		ASSERT(0);
		return;
	}
	pTile->AddTileAttrib(areatile);
}

void CTileManager::RemoveTileAttrByAreaData(VECTOR3* pPos,AREATILE areatile)
{
	CTile *pTile = GetTile(pPos->x, pPos->z);
	if(pTile == 0)
	{
		ASSERT(0);
		return;
	}
	pTile->RemoveTileAttrib(areatile);
}
