// SkillArea.cpp: implementation of the CSkillArea class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillArea.h"

#ifdef _MAPSERVER_
#include "TileManager.h"
#include "ServerSystem.h"
#else if _MHCLIENT_
#include "Effect/EffectManager.h"
#include "MHMap.h"
#include "TileManager.h"
#endif

#define ISDAMAGETILE(tile)		(tile&SKILLAREA_ATTR_DAMAGE)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillArea::CSkillArea()
{

}
CSkillArea::CSkillArea(CSkillAreaData* pSrcAreaData)
{
	m_pSkillAreaData = pSrcAreaData;
}

CSkillArea::~CSkillArea()
{
	
}

BOOL CSkillArea::IsInArea(VECTOR3* pPos)
{
	float fx = pPos->x - m_CenterPos.x;
	float fz = pPos->z - m_CenterPos.z;
	int nx = int(fx / fTILE_SIZE);
	int nz = int(fz / fTILE_SIZE);
	int AreaRadius = m_pSkillAreaData->GetAreaRadius();
	if(nx < (-AreaRadius) || nx > AreaRadius)
		return FALSE;
	if(nz < (-AreaRadius) || nz > AreaRadius)
		return FALSE;
	return TRUE;
}
BOOL CSkillArea::IsInDamageTile(VECTOR3* pPos)
{
	if(IsInArea(pPos) == FALSE)
		return FALSE;

	float fx = pPos->x - m_CenterPos.x;
	float fz = pPos->z - m_CenterPos.z;
	int areaindex = m_pSkillAreaData->GetAreaIndex(fx,fz);
	AREATILE Tile = m_pSkillAreaData->GetAreaAttrib(areaindex);
	if(ISDAMAGETILE(Tile))
		return TRUE;
	else
		return FALSE;
}

#ifdef _MHCLIENT_
void CSkillArea::StartEffectOnDamageArea(WORD EffectNum,CObject* pOperator)
{
	VECTOR3 AddPos;
	AddPos.y = 0;
	int areaindex;
	AREATILE Tile;
	for(int x=-m_pSkillAreaData->GetAreaRadius();x<m_pSkillAreaData->GetAreaRadius();++x)
	{
		AddPos.x = (float)fTILE_SIZE*x;
		for(int z=-m_pSkillAreaData->GetAreaRadius();z<m_pSkillAreaData->GetAreaRadius();++z)
		{
			AddPos.z = (float)fTILE_SIZE*z;
			
			areaindex = m_pSkillAreaData->GetAreaIndex(AddPos.x,AddPos.z);
			Tile = m_pSkillAreaData->GetAreaAttrib(areaindex);
			if(ISDAMAGETILE(Tile))
				EFFECTMGR->StartEffectProcess(EffectNum,pOperator,NULL,0,0,EFFECT_FLAG_NORMAL,&AddPos);
		}
	}
}
#endif

void CSkillArea::AddTileAttrByAreaData(CObject* pObject)
{
	VECTOR3 AddPos,Pos;
	Pos.y = 0;
	int areaindex;
	AREATILE Tile;
#ifdef _MAPSERVER_
	CTileManager* pTileManager = g_pServerSystem->GetMap()->GetTileManager();
#else
	CTileManager* pTileManager = MAP->GetTileManager();
#endif
	for(int x=-m_pSkillAreaData->GetAreaRadius();x<m_pSkillAreaData->GetAreaRadius();++x)
	{
		AddPos.x = fTILE_SIZE*x;
		Pos.x = m_CenterPos.x + AddPos.x;
		for(int z=-m_pSkillAreaData->GetAreaRadius();z<m_pSkillAreaData->GetAreaRadius();++z)
		{
			AddPos.z = fTILE_SIZE*z;
			Pos.z = m_CenterPos.z + AddPos.z;
			
			areaindex = m_pSkillAreaData->GetAreaIndex(AddPos.x,AddPos.z);
			Tile = m_pSkillAreaData->GetAreaAttrib(areaindex);

			if(Tile != SKILLAREA_ATTR_BLOCK)
				continue;

			pTileManager->AddTileAttrByAreaData(pObject,&Pos,Tile);
		}
	}
}

void CSkillArea::RemoveTileAttrByAreaData(CObject* pObject)
{
	VECTOR3 AddPos,Pos;
	Pos.y = 0;
	int areaindex;
	AREATILE Tile;
#ifdef _MAPSERVER_
	CTileManager* pTileManager = g_pServerSystem->GetMap()->GetTileManager();
#else if
	CTileManager* pTileManager = MAP->GetTileManager();
#endif
	for(int x=-m_pSkillAreaData->GetAreaRadius();x<m_pSkillAreaData->GetAreaRadius();++x)
	{
		AddPos.x = fTILE_SIZE*x;
		Pos.x = m_CenterPos.x + AddPos.x;
		for(int z=-m_pSkillAreaData->GetAreaRadius();z<m_pSkillAreaData->GetAreaRadius();++z)
		{
			AddPos.z = fTILE_SIZE*z;
			Pos.z = m_CenterPos.z + AddPos.z;
			
			areaindex = m_pSkillAreaData->GetAreaIndex(AddPos.x,AddPos.z);
			Tile = m_pSkillAreaData->GetAreaAttrib(areaindex);
			
			if(Tile != SKILLAREA_ATTR_BLOCK)
				continue;

			pTileManager->RemoveTileAttrByAreaData(pObject,&Pos,Tile);
		}
	}
}
