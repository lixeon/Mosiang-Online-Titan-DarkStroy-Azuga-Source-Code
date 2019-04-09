// TileManager.cpp: implementation of the CTileManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TileManager.h"
#include "Object.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



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
	if(!(fx >= 0 && fz >= 0))
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


BOOL CTileManager::IsInTile(int cellX, int cellY, MAPTYPE MapNum,CObject* pObject)
{
	if( cellX < 0 || cellY < 0 || cellX >= (int)m_dwTileWidth || cellY >= (int)m_dwTileHeight )
			return FALSE;
		return TRUE;
}
BOOL CTileManager::CollisionTile( int x, int y, MAPTYPE MapNum,CObject* pObject)
{ 
	CTile *pTile = GetTile((DWORD)x, (DWORD)y);
	if(pTile)
		return pTile->IsCollisonTile();
	else
	{
		char temp[256];
		sprintf(temp,"%d %d %s",x,y,pObject->GetObjectName());
		ASSERTMSG(0,temp);
		return TRUE;
	}
}	
BOOL CTileManager::CollisionLine(VECTOR3* pSrc,VECTOR3* pDest, VECTOR3 * pTarget, MAPTYPE MapNum,CObject* pObject)
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
		CTile *pTile = GetTile((DWORD)CellX, (DWORD)CellY);
		if(pTile == NULL)
		{
			if(pTarget)
			{
				pTarget->x = PrevCellX*TILECOLLISON_DETAIL;
				pTarget->y = 0;
				pTarget->z = PrevCellY*TILECOLLISON_DETAIL;
			}
			
			char temp[256];
			sprintf(temp,"%d %d %s",CellX,CellY,pObject->GetObjectName());
			ASSERTMSG(0,temp);
			
			return TRUE;
		}
		if(pTile->IsCollisonTile())
		{
			if(pTarget)
			{
				pTarget->x = PrevCellX*TILECOLLISON_DETAIL;
				pTarget->y = 0;
				pTarget->z = PrevCellY*TILECOLLISON_DETAIL;
				/*
				CTile *p2Tile = GetTile((DWORD)PrevCellX, (DWORD)PrevCellY);
								if(p2Tile->IsCollisonTile())
								{
									int a = 3;
								}*/
				
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

BOOL CTileManager::NonCollisionLine( VECTOR3* pStart, VECTOR3* pEnd, VECTOR3* pRtNonCollisonPos, MAPTYPE MapNum, CObject* pObj )
{
	int x1 = int(pStart->x / TILECOLLISON_DETAIL);
	int y1 = int(pStart->z / TILECOLLISON_DETAIL);
	int x2 = int(pEnd->x / TILECOLLISON_DETAIL);
	int y2 = int(pEnd->z / TILECOLLISON_DETAIL);

	int dx = x1 - x2;
	int dy = y1 - y2;
	
	int absDX = abs(dx);
	int absDY = abs(dy);
	int MaxDelta = max(absDX, absDY);
	
	int CellX = x1;
	int CellY = y1;

	CTile *pCurTile = GetTile((DWORD)CellX, (DWORD)CellY);
	if(pCurTile == NULL)
	{
		if(pRtNonCollisonPos)
		{
			pRtNonCollisonPos->x = CellX * TILECOLLISON_DETAIL;
			pRtNonCollisonPos->y = 0;
			pRtNonCollisonPos->z = CellY * TILECOLLISON_DETAIL;
		}

		char temp[256] = {0,};
		sprintf(temp, "%d %d %s", CellX, CellY, pObj->GetObjectName() );
		ASSERTMSG(0,temp);

		return FALSE;
	}
	if(FALSE == pCurTile->IsCollisonTile())	//현재 위치가 충돌 타일이 아니면 이 함수의 역할과 무관
	{
		if(pRtNonCollisonPos)
		{
			pRtNonCollisonPos->x = CellX * TILECOLLISON_DETAIL;
			pRtNonCollisonPos->y = 0;
			pRtNonCollisonPos->z = CellY * TILECOLLISON_DETAIL;
		}

		return FALSE;
	}

	int x = 0;
	int y = 0;

	int signDX = -sign(dx);
	int signDY = -sign(dy);
	
	int PrevCellX = 0;
	int PrevCellY = 0;

#define THROUGH_PERMIT_TILE_INDEX 2

	for( int i = 0 ; i <= MaxDelta ; ++i )
	{
		if( i > THROUGH_PERMIT_TILE_INDEX )
		{
			if(pRtNonCollisonPos)
			{
				pRtNonCollisonPos->x = CellX*TILECOLLISON_DETAIL;
				pRtNonCollisonPos->y = 0;
				pRtNonCollisonPos->z = CellY*TILECOLLISON_DETAIL;
			}

			return FALSE;
		}

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
		CTile *pTile = GetTile((DWORD)CellX, (DWORD)CellY);
		if(pTile == NULL)
		{
			if(pRtNonCollisonPos)
			{
				pRtNonCollisonPos->x = PrevCellX*TILECOLLISON_DETAIL;
				pRtNonCollisonPos->y = 0;
				pRtNonCollisonPos->z = PrevCellY*TILECOLLISON_DETAIL;
			}
			
			char temp[256];
			sprintf(temp,"%d %d %s",CellX,CellY,pObj->GetObjectName());
			ASSERTMSG(0,temp);
			
			return FALSE;
		}
		if( FALSE == pTile->IsCollisonTile() )
		{
			if(pRtNonCollisonPos)
			{
				pRtNonCollisonPos->x = CellX*TILECOLLISON_DETAIL;
				pRtNonCollisonPos->y = 0;
				pRtNonCollisonPos->z = CellY*TILECOLLISON_DETAIL;
			}
			return TRUE;
		}
	}

	if(pRtNonCollisonPos)
	{
		pRtNonCollisonPos->x = x1*TILECOLLISON_DETAIL;
		pRtNonCollisonPos->y = 0;
		pRtNonCollisonPos->z = y1*TILECOLLISON_DETAIL;
	}
	return FALSE;
}

// pObj ´Â µð¹ö±×¿ë

BOOL CTileManager::CollisonCheck(VECTOR3* pStart,VECTOR3* pEnd,VECTOR3* pRtCollisonPos,CObject* pObj)
{	
	if( pStart->x >= 51200 || pStart->x < 0 ||
		pStart->z >= 51200 || pStart->z < 0 )
	{
		ASSERTMSG(0,"ÀÌµ¿¿µ¿ªÀ» ¹þ¾î³µ½À´Ï´Ù.");
		pRtCollisonPos->x = 25000;
		pRtCollisonPos->z = 25000;
		return TRUE;
	}

	int x0 = int(pStart->x / fTILE_WIDTH);
	int y0 = int(pStart->z / fTILE_HEIGHT);
	int x1 = int(pEnd->x / fTILE_WIDTH);
	int y1 = int(pEnd->z / fTILE_HEIGHT);
	
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
	{
		//bFirst = FALSE;
		return FALSE;	//CSW
	}
	
	if(dx >= dy)
	{
		for(int desc = ay - dx; ;x += sx,desc += ay)
		{
			if(bFirst == TRUE)
			{
				bFirst = FALSE;
				continue;
			}

			CTile *pTile = GetTile((DWORD)x, (DWORD)y);
			
#ifdef _DEBUG
			if(pTile == NULL)
			{
				LOGFILE("TILENOTFOUND\t%s",OBJECTLOG(pObj));
				LOGFILE("StartPos\t%4.2f\t%4.2f\t\tEndPos\t%4.2f\t%4.2f",
					pStart->x,pStart->z,pEnd->x,pEnd->z);
				ASSERTMSG(0,"ÀÌ ¿¡·¯ ¸Þ¼¼Áö º¸½Ã¸é À±È£ÇÑÅ× ²À! ¿¬¶ôÇØÁÖ¼¼¿ä");
				break;
			}			
#endif
			if(pTile == NULL || pTile->IsCollisonTile())
			{
				pRtCollisonPos->x = (float)(lastx*fTILE_WIDTH);
				pRtCollisonPos->y = 0;
				pRtCollisonPos->z = (float)(lasty*fTILE_HEIGHT);
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

			CTile *pTile = GetTile((DWORD)x, (DWORD)y);
			
#ifdef _DEBUG
			if(pTile == NULL)
			{
				LOGFILE("TILENOTFOUND\t%s",OBJECTLOG(pObj));
				LOGFILE("StartPos\t%4.2f\t%4.2f\t\tEndPos\t%4.2f\t%4.2f",
					pStart->x,pStart->z,pEnd->x,pEnd->z);
				ASSERTMSG(0,"ÀÌ ¿¡·¯ ¸Þ¼¼Áö º¸½Ã¸é À±È£ÇÑÅ× ²À! ¿¬¶ôÇØÁÖ¼¼¿ä");
				break;
			}			
#endif
			if(pTile == NULL || pTile->IsCollisonTile())
			{
				pRtCollisonPos->x = (float)(lastx*fTILE_WIDTH);
				pRtCollisonPos->y = 0;
				pRtCollisonPos->z = (float)(lasty*fTILE_HEIGHT);
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
BOOL CTileManager::CollisonCheck(VECTOR3* pStart,VECTOR3* pEnd,VECTOR3* pRtCollisonPos,CObject* pObj)
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

#ifdef _DEBUG
		if(pTile == NULL)
		{
			LOGFILE("TILENOTFOUND\t%s",OBJECTLOG(pObj));
			LOGFILE("StartPos\t%4.2f\t%4.2f\t\tEndPos\t%4.2f\t%4.2f",
				pStart->x,pStart->z,pEnd->x,pEnd->z);
			LOGFILE("CurPosition\t%4.2f\t%4.2f",curpos.x,curpos.z);
			ASSERTMSG(0,"ÀÌ ¿¡·¯ ¸Þ¼¼Áö º¸½Ã¸é À±È£ÇÑÅ× ²À! ¿¬¶ôÇØÁÖ¼¼¿ä");
			break;
		}
#endif

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
*/

void CTileManager::AddTileAttrByAreaData(CObject* Object,VECTOR3* pPos,AREATILE areatile)
{
	CTile *pTile = GetTile(pPos->x, pPos->z);
	if(pTile == 0)
	{
		ASSERT(0);
		return;
	}
	pTile->AddTileAttrib(areatile);
}

void CTileManager::RemoveTileAttrByAreaData(CObject* Object,VECTOR3* pPos,AREATILE areatile)
{
	CTile *pTile = GetTile(pPos->x, pPos->z);
	if(pTile == 0)
	{
		ASSERT(0);
		return;
	}
	pTile->RemoveTileAttrib(areatile);
}
