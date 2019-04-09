// GridTable.cpp: implementation of the CGridTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridTable.h"
#include "Grid.h"
#include "Object.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGridTable::CGridTable()
{
	m_ID = 0;
}

CGridTable::~CGridTable()
{
}

/*
CGrid* CGridTable::GetGrid(int nx,int nz)
{
	if(nx == 65535 || nz == 65535)
		return NULL;

	return &m_ppGrid[nx][nz];
}

void CGridTable::Init(int Bits,int MaxLength)
{
	
	m_Bit = Bits;
	
	int gridsize = 1;
	for(int n=0;n<m_Bit;++n)
		gridsize *= 2;
	m_GridNum = (MaxLength-1) / gridsize;
	
	m_ppGrid = new CGrid*[m_GridNum];
	for(n=0;n<m_GridNum;++n)
	{
		m_ppGrid[n] = new CGrid[m_GridNum];
	}


	//////////////////////////////////////////////////////////////////////////
	// °¢ Grid°¡ ÁÖº¯ GridµéÀ» ±â¾ïÇÏ°Ô ÇÔ
	int nx,nz;
	int temp[16] = {
		-1,-1,
		-1,0,
		-1,1,
		0,-1,
		0,1,
		1,-1,
		1,0,
		1,1,
	};
	CGrid* pCurGrid;
	CGrid* pNeighborGrid;
	for(n=0;n<m_GridNum;++n)
	{
		for(int m=0;m<m_GridNum;++m)
		{
			pCurGrid = &m_ppGrid[n][m];

			for(int tt = 0;tt<8;++tt)
			{
				nx = n+temp[2*tt];
				nz = m+temp[2*tt+1];
				if(nx < 0 || nx >= m_GridNum ||
					nz < 0 || nz >= m_GridNum)
					continue;

				pNeighborGrid = &m_ppGrid[nx][nz];
				pCurGrid->AddNeighborGrid(pNeighborGrid);
			}
		}
	} 
}


void CGridTable::Release()
{
	if(m_ppGrid)
	{
		for(int n=0;n<m_GridNum;++n)
		{
			delete [] m_ppGrid[n];
		}
		delete [] m_ppGrid;
		m_ppGrid = NULL;
	}
}

void CGridTable::AddObject(int nx,int nz,CObject* pObject)
{
	CGrid* pGrid = &(m_ppGrid[nx][nz]);
	pGrid->AddObject(pObject);
}

void CGridTable::RemoveObject(int nx,int nz,CObject* pObject)
{
	if(nx == 65535 && nz == 65535)
		return;
	
	CGrid* pGrid = &(m_ppGrid[nx][nz]);
	pGrid->RemoveObject(pObject);
}


void CGridTable::AddGridToPackedData(int GridNumX,int GridNumZ,CPackedData* pPackedData)
{
	m_ppGrid[GridNumX][GridNumZ].AddGridToPackedData(pPackedData);
}

void CGridTable::AddGridToPackedDataWithoutOne(int GridNumX,int GridNumZ,DWORD dwID,CPackedData* pPackedData)
{
	m_ppGrid[GridNumX][GridNumZ].AddGridToPackedDataWithoutOne(dwID,pPackedData);
}

void CGridTable::AddGridToPackedDataExceptLastGrid(GridPosition* pGridPosition,DWORD ExceptID,CPackedData* pPackedData)
{
	CGrid* pCurGrid = GetGrid(pGridPosition->x,pGridPosition->z);
	CGrid* pLastGrid = GetGrid(pGridPosition->LastX,pGridPosition->LastZ);
	
	if(pCurGrid == NULL || pLastGrid == NULL)
	{
		ASSERT(0);
		return;
	}

	pCurGrid->AddGridToPackedDataExceptLastGrid(ExceptID,pLastGrid,pPackedData);

}
*/
void CGridTable::AddLastGridToPackedDataExceptCurrentGrid(GridPosition* pGridPosition,DWORD ExceptID,CPackedData* pPackedData)
{
	CGrid* pCurGrid = GetGrid(pGridPosition->x,pGridPosition->z);
	CGrid* pLastGrid = GetGrid(pGridPosition->LastX,pGridPosition->LastZ);

	if(pLastGrid == NULL)
		return;

	pLastGrid->AddGridToPackedDataExceptLastGrid(ExceptID,pCurGrid,pPackedData);

}

/*

void CGridTable::SendCurrentObjectInfo(int GridNumX,int GridNumZ,int LastGridNumX,int LastGridNumZ,CObject* pNewObject)
{
	CGrid* pLastGrid;
	if(LastGridNumX < 0 || LastGridNumZ < 0)
		pLastGrid = NULL;
	else
		pLastGrid = &m_ppGrid[LastGridNumX][LastGridNumZ];
	m_ppGrid[GridNumX][GridNumZ].SendCurrentObjectInfo(pNewObject,pLastGrid);

}

void CGridTable::SendRemoveObjectInfo(int GridNumX,int GridNumZ,int LastGridNumX,int LastGridNumZ,CObject* pLeavedObject)
{
	CGrid* pCurGrid;
	if(GridNumX < 0 || GridNumZ < 0)
		pCurGrid = NULL;
	else
		pCurGrid = GetGrid(GridNumX,GridNumZ);
	m_ppGrid[LastGridNumX][LastGridNumZ].SendRemoveObjectInfo(pLeavedObject,pCurGrid);
  
}

CObject* CGridTable::FindPlayerInRange(int GridNumX,int GridNumZ,VECTOR3* pPos,float Range, BYTE flag)
{
	if(flag == FIND_CLOSE)
		return m_ppGrid[GridNumX][GridNumZ].FindPlayerInRange(pPos,Range);
	else
		return m_ppGrid[GridNumX][GridNumZ].FindFisrtPlayerInRange(pPos,Range);

}
*/

CObject* CGridTable::FindPlayerRandom(CObject* pObject)
{
	return NULL;
}