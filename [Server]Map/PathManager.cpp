// PathManager.cpp: implementation of the CPathManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PathManager.h"
#include "..\[CC]Header\GameResourceManager.h"
#include "STRPath.h"
#include "MHMap.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GLOBALTON(CPathManager);

CPathManager::CPathManager()
{
	m_pSTRPath = new CSTRPath;
	m_pMap = NULL;
}

CPathManager::~CPathManager()
{
	delete m_pSTRPath;
}
void CPathManager::SetMap(CMHMap * pMap, MAPTYPE mapNum, int width) 
{ 
	m_pMap = pMap; 
	m_MapType = mapNum; 
	m_pSTRPath->SetWidth(width); 
}

int CPathManager::GetPath(VECTOR3 * pSrcPos, VECTOR3 * pDestPos, VECTOR3 * pWayPointPos, WORD wBufCount, BYTE& wWayPointNum, CObject* pObject, BOOL bReverse, WORD wDepth)
{
	VECTOR3 TargetPos;

	// Å¸°Ù ÁöÁ¡ÀÌ ÀÌµ¿ÇÒ ¼ö ÀÖ´Â °÷ÀÎ°¡?
	if(m_pMap->CollisionTilePos(pDestPos->x, pDestPos->z, pObject))
	{
		// ÀÌµ¿ÇÒ ¼ö ÀÖ´Â ÃÖ´Ü À§Ä¡ °è»ê
		if(m_pMap->CollisionLine(pSrcPos, pDestPos, &TargetPos, pObject))
		{
			pWayPointPos[0] = TargetPos; wWayPointNum = 1;
			if(m_pMap->CollisionTilePos(TargetPos.x, TargetPos.z, pObject))
			{
				return PATH_FAILED;
			}
			return PATH_SUCCESS;
		}
		else
		{	
			return PATH_FAILED;
		}
	}


	// Å¸°Ù ÀÌµ¿ ÇÒ ¼ö ÀÖ´Ù¸é Å¸°Ù±îÁö Á÷¼± °Å¸® ÀÎ°¡?
	if(!m_pMap->CollisionLine(pSrcPos, pDestPos, &TargetPos, pObject))
	{
		pWayPointPos[0] = TargetPos; wWayPointNum = 1;
		return PATH_SUCCESS;
	}
	

	// Á÷¼±ÀÌ ¾Æ´Ï¸é ±æÃ£±â
	m_pSTRPath->IsValidNode = CPathManager::ValidFunc;
	if(!m_pSTRPath->FindPath(pObject,pSrcPos, pDestPos, pWayPointPos, wBufCount, wWayPointNum, wDepth))
	{
		if(bReverse)
		{
			return PATH_BLOCKFAILED;
		}
		else
		{		
			// ÀÌµ¿ÇÒ ¼ö ÀÖ´Â ÃÖ´Ü À§Ä¡ °è»ê
			if(m_pMap->CollisionLine(pSrcPos, pDestPos, &TargetPos, pObject))
			{
				pWayPointPos[0] = TargetPos; wWayPointNum = 1;
				return PATH_SUCCESS;
			}
			else
			{
				return PATH_FAILED;
			}
		}
	}
	
	return PATH_SUCCESS;

}

BOOL CPathManager::ValidFunc(int cellX, int cellY,CObject* pObject)
{
	if(!PATHMGR->m_pMap->IsInTile(cellX, cellY, pObject))
		return FALSE;

	if(!PATHMGR->m_pMap->CollisionTileCell(cellX, cellY, pObject))
		return FALSE;

	return TRUE;
}