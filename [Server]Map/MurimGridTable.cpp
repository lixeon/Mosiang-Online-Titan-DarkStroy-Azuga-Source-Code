// MurimGridTable.cpp: implementation of the CMurimGridTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MurimGridTable.h"
#include "Grid.h"
#include "Object.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMurimGridTable::CMurimGridTable()
{

}

CMurimGridTable::~CMurimGridTable()
{

}

void CMurimGridTable::Init(DWORD GridID,int MaxWidth,int MaxHeight)
{
	m_Kind = eGridType_Murim;
	
	m_ID = GridID;
	
	m_pGrid = new CGrid;
}

void CMurimGridTable::Release()
{
	delete m_pGrid;
	m_pGrid = NULL;
}
void CMurimGridTable::CalcGridPosition(float x,float z,GridPosition* pOutGridPos)
{
	pOutGridPos->LastX = pOutGridPos->x;
	pOutGridPos->LastZ = pOutGridPos->z;
	pOutGridPos->x = 0;
	pOutGridPos->z = 0;
}

CGrid* CMurimGridTable::GetGrid(WORD x, WORD z)
{
	ASSERT(x == 0 && z == 0);

	return m_pGrid;
}

void CMurimGridTable::AddObject(CObject* pObject, VECTOR3* pPos)
{
	pObject->GetGridPosition()->x = 0;
	pObject->GetGridPosition()->z = 0;
	m_pGrid->AddObject(pObject);
}

void CMurimGridTable::RemoveObject(CObject* pObject)
{
	m_pGrid->RemoveObject(pObject);
}

CObject* CMurimGridTable::FindPlayerInRange(CObject* pFinder,GridPosition* pGridPosition,VECTOR3* pPos,float Range, BYTE flag)
{
	return m_pGrid->FindFirstPlayerInRange(pFinder,pPos,Range);
}
CObject* CMurimGridTable::FindMonsterInRange(CObject* pFinder,GridPosition* pGridPosition,VECTOR3* pPos,float Range, BYTE flag)
{
	return m_pGrid->FindFirstMonsterInRange(pFinder,pPos,Range);
}

void CMurimGridTable::AddGridToPackedData(GridPosition* pGridPosition,CPackedData* pPackedData)
{
	m_pGrid->AddGridToPackedData(pPackedData);
}

void CMurimGridTable::AddGridToPackedDataWithoutOne(GridPosition* pGridPosition,DWORD dwID,CPackedData* pPackedData)
{
	m_pGrid->AddGridToPackedDataWithoutOne(dwID,pPackedData);
}

void CMurimGridTable::AddGridToPackedDataExceptLastGrid(GridPosition* pGridPosition,DWORD ExceptID,CPackedData* pPackedData)
{
	m_pGrid->AddGridToPackedDataExceptLastGrid(ExceptID, NULL, pPackedData);
}

void CMurimGridTable::AddLastGridToPackedDataExceptCurrentGrid(GridPosition* pGridPosition,DWORD ExceptID,CPackedData* pPackedData)
{
	CGridTable::AddLastGridToPackedDataExceptCurrentGrid(pGridPosition, ExceptID, pPackedData);
}

void CMurimGridTable::ChangeGrid(CObject* pObject, VECTOR3* pPos)
{
	// do nothing
}

void CMurimGridTable::SendCurrentObjectInfo(int GridNumX,int GridNumZ,int LastGridNumX,int LastGridNumZ,CObject* pNewObject)
{
	m_pGrid->SendCurrentObjectInfo(pNewObject, NULL);
}

void CMurimGridTable::SendRemoveObjectInfo(int GridNumX,int GridNumZ,int LastGridNumX,int LastGridNumZ,CObject* pLeavedObject)
{
	m_pGrid->SendRemoveObjectInfo(pLeavedObject, m_pGrid);
}


BOOL CMurimGridTable::GetGridChangeTime(VECTOR3* pStartPos,VECTOR3* pTargetPos,float MoveSpeed,DWORD& dwChangeTime)
{
	return FALSE;
}

void CMurimGridTable::OnDelete()
{
	
}