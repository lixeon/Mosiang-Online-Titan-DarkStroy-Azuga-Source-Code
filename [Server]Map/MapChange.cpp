// MapChange.cpp: implementation of the CMapChange class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\[CC]Header\GameResourceStruct.h"
#include "MapChange.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CMapChange::CMapChange()
{
	for(int i = 0; i < MAX_MAP_NUM; i++)
	{
/*		m_MapChangeArea[i].OutMapNum = 0;
		m_MapChangeArea[i].ChangeOut_lefttop.x = 0;
		m_MapChangeArea[i].ChangeOut_lefttop.y = 0;
		m_MapChangeArea[i].ChangeOut_lefttop.z = 0;
		m_MapChangeArea[i].ChangeOut_rightbottom.x = 0;
		m_MapChangeArea[i].ChangeOut_rightbottom.y = 0;
		m_MapChangeArea[i].ChangeOut_rightbottom.z = 0;

		m_MapChangeArea[i].InMapNum = 0;
		m_MapChangeArea[i].ChangeIn_Point.x = 0;
		m_MapChangeArea[i].ChangeIn_Point.y = 0;
		m_MapChangeArea[i].ChangeIn_Point.z = 0;
		
		m_LoginPoint[i].MapNum = 0;
		m_LoginPoint[i].LoginPoint.x = 0;
		m_LoginPoint[i].LoginPoint.x = 0;
		m_LoginPoint[i].LoginPoint.x = 0;
		m_LoginPoint[i].PointKind = 0;
*/	}
	
}

CMapChange::~CMapChange()
{

}

MAPCHANGE_INFO* CMapChange::GetMapChangeArea(WORD MapNum)
{
/*	for(int i = 0; i < MAX_MAP_NUM; i++)
	{
		if(m_MapChangeArea[i].InMapNum == MapNum)
		{
			return &m_MapChangeArea[i];
		}
	}
	*/
	return NULL;
}

LOGINPOINT_INFO* CMapChange::GetLoginPoint(WORD MapNum)
{
/*	for(int i = 0; i < MAX_MAP_NUM; i++)
	{
		if(m_LoginPoint[i].MapNum == MapNum)
		{
			return &m_LoginPoint[i];
		}
	}
*/	
	return NULL;
}

void CMapChange::GetLoginPointPosition(WORD MapNum,VECTOR3* pRtPos)
{
/*	VECTOR3 OriPos;
	VECTOR3 RandPos;
	
	OriPos.x = 25000;
	OriPos.y = 0;
	OriPos.z = 25000;

	for(int i = 0; i < MAX_MAP_NUM; i++)
	{
		if(m_LoginPoint[i].MapNum == MapNum)
		{
			OriPos = m_LoginPoint[i].LoginPoint;
		}
	}
	int temp;

	temp = rand() % 1000 - 500;
	RandPos.x = OriPos.x + temp;
	temp = rand() % 1000 - 500;
	RandPos.z = OriPos.z + temp;
	RandPos.y = 0;

	*pRtPos = g_pServerSystem->GetMap()->GetTileManager()->GetNextCloserPos(&RandPos,&OriPos,100);
*/
}