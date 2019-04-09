// GeneralGridTable.cpp: implementation of the CGeneralGridTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GeneralGridTable.h"
#include "Grid.h"
#include "PackedData.h"
#include "ServerTable.h"
#include "Object.h"
#include "GridSystem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int TransToGridCoordinate(float val)
{
	return int(val*0.01f);
}

float TransToGridCoordinateFloat(float val)
{
	return val*0.01f;
}
// 0에서 1사이이면 통과한다.
float IsIntersect(float alp,float pos0,float pos1)
{
	float rt = 1.f + (pos1 - alp) / (pos0 - pos1);
	if(rt > 0 && rt <= 1)
		return rt;
	else
		return 10.f;
}

CGeneralGridTable::CGeneralGridTable()
{
	m_ChannelID = 0;
}

CGeneralGridTable::~CGeneralGridTable()
{
}

void CGeneralGridTable::Init(DWORD GridID,int MaxWidth,int MaxHeight)
{
	m_Kind = eGridType_General;

	m_ID = GridID;

	//////////////////////////////////////////////////////////////////////////
	// 2의 제곱수로 길이를 맞춘다
	m_nMaxLength = max(MaxWidth,MaxHeight) - 1;
	
	int exponent = 0;
	while(m_nMaxLength > 0)
	{
		m_nMaxLength = m_nMaxLength >> 1;
		++exponent;
	}

	m_nMaxLength = 1;
	for(int n=0;n<exponent;++n)
		m_nMaxLength *= 2;
	//////////////////////////////////////////////////////////////////////////

	m_GridSize = 1;
	for(n=0;n<GRID_BIT;++n)
		m_GridSize *= 2;

	m_GridNum = (m_nMaxLength-1) / m_GridSize + 1;
	
	m_ppGrid = new CGrid*[m_GridNum];
	for(n=0;n<m_GridNum;++n)
	{
		m_ppGrid[n] = new CGrid[m_GridNum];
	}


	//////////////////////////////////////////////////////////////////////////
	// 각 Grid가 주변 Grid들을 기억하게 함
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

void CGeneralGridTable::Release()
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

void CGeneralGridTable::CalcGridPosition(float x,float z,GridPosition* pOutGridPos)
{
	pOutGridPos->LastX = pOutGridPos->x;
	pOutGridPos->LastZ = pOutGridPos->z;
	pOutGridPos->x = 0;
	pOutGridPos->z = 0;
}

CGrid* CGeneralGridTable::GetGrid(WORD x, WORD z)
{
	if(x >= m_GridNum || z >= m_GridNum)
	{
		//ASSERT(0);
		return NULL;
	}

	return &m_ppGrid[x][z];
}

CObject* CGeneralGridTable::FindPlayerInRange(CObject* pFinder,GridPosition* pGridPosition,VECTOR3* pPos,float Range, BYTE flag)
{
	CGrid* pGrid = GetGrid(pGridPosition->x,pGridPosition->z);
	if(pGrid == NULL)
		return NULL;

	if(flag == FIND_CLOSE)
		return pGrid->FindPlayerInRange(pFinder,pPos,Range);
	else
		return pGrid->FindFirstPlayerInRange(pFinder,pPos,Range);
}
CObject* CGeneralGridTable::FindMonsterInRange(CObject* pFinder,GridPosition* pGridPosition,VECTOR3* pPos,float Range, BYTE flag)
{
	CGrid* pGrid = GetGrid(pGridPosition->x,pGridPosition->z);
	if(pGrid == NULL)
		return NULL;

	if(flag == FIND_CLOSE)
		return pGrid->FindMonsterInRange(pFinder,pPos,Range);
	else
		return pGrid->FindFirstMonsterInRange(pFinder,pPos,Range);
}

void CGeneralGridTable::AddObject(CObject* pObject, VECTOR3* pPos)
{
	int firstx = TransToGridCoordinate(pPos->x);
	int firstz = TransToGridCoordinate(pPos->z);

	int nx;
	int nz;
	
	nx = firstx >> (GRID_BIT);
	nz = firstz >> (GRID_BIT);
	
	CGrid* pGrid = GetGrid(nx,nz);
	if(pGrid == NULL)
		return;

	pGrid->AddObject(pObject);
	
	pObject->GridPos.x = nx;
	pObject->GridPos.z = nz;
}

void CGeneralGridTable::RemoveObject(CObject* pObject)
{
	int nx = pObject->GetGridPosition()->x;
	int nz = pObject->GetGridPosition()->z;
	if(nx == 65535 && nz == 65535)
	{
		ASSERT(0);
		return;
	}
	
	CGrid* pGrid = GetGrid(nx,nz);
	pGrid->RemoveObject(pObject);
}

void CGeneralGridTable::AddGridToPackedData(GridPosition* pGridPosition,CPackedData* pPackedData)
{
	CGrid* pCurGrid = GetGrid(pGridPosition->x, pGridPosition->z);
	if(pCurGrid == NULL)
		return;

	pCurGrid->AddGridToPackedData(pPackedData);
}

void CGeneralGridTable::AddGridToPackedDataWithoutOne(GridPosition* pGridPosition,DWORD dwID,CPackedData* pPackedData)
{
	CGrid* pCurGrid = GetGrid(pGridPosition->x, pGridPosition->z);
	if(pCurGrid == NULL)
		return;

	pCurGrid->AddGridToPackedDataWithoutOne(dwID,pPackedData);
}

void CGeneralGridTable::AddGridToPackedDataExceptLastGrid(GridPosition* pGridPosition,DWORD ExceptID,CPackedData* pPackedData)
{
	CGrid* pCurGrid = GetGrid(pGridPosition->x, pGridPosition->z);
	CGrid* pLastGrid = GetGrid(pGridPosition->LastX, pGridPosition->LastZ);
	
	if(pCurGrid == NULL || pLastGrid == NULL)
	{
		return;
	}

	pCurGrid->AddGridToPackedDataExceptLastGrid(ExceptID,pLastGrid,pPackedData);
}

void CGeneralGridTable::AddLastGridToPackedDataExceptCurrentGrid(GridPosition* pGridPosition,DWORD ExceptID,CPackedData* pPackedData)
{
	CGridTable::AddLastGridToPackedDataExceptCurrentGrid(pGridPosition, ExceptID, pPackedData);
}

void CGeneralGridTable::ChangeGrid(CObject* pObject, VECTOR3* pPos)
{
	int firstx = TransToGridCoordinate(pPos->x);
	int firstz = TransToGridCoordinate(pPos->z);

	int nx;
	int nz;
	
	nx = firstx >> (GRID_BIT);
	nz = firstz >> (GRID_BIT);

	if(nx >= m_GridNum)		nx = m_GridNum-1;
	if(nz >= m_GridNum)		nz = m_GridNum-1;
	if(nx < 0)		nx = 0;
	if(nz < 0)		nz = 0;
	
	if(nx == pObject->GridPos.x && nz == pObject->GridPos.z)
		return;
	
	RemoveObject(pObject);

	pObject->GridPos.LastX = pObject->GridPos.x;
	pObject->GridPos.LastZ = pObject->GridPos.z;
	
	pObject->GridPos.x = nx;
	pObject->GridPos.z = nz;

	//////////////////////////////////////////////////////////////////////////
	// YH2DO
	if(GetGrid(pObject->GridPos.x,pObject->GridPos.z) ==NULL)
	{
		ASSERT(0);
		return;
	}
	//////////////////////////////////////////////////////////////////////////
		
	AddObject(pObject,pPos);

	static char Msg[1024] = {0,};
	WORD MsgLen;
	
	// 새로들어갈 그리드에 있던 애들에게 새로 들어 왔음을 알려준다
	pObject->SetAddMsg(Msg,&MsgLen,0,FALSE);
	if(MsgLen)
	{
		PACKEDDATA_OBJ->Init(g_pServerTable->GetMaxServerConnectionIndex(),Msg,MsgLen);
		AddGridToPackedDataExceptLastGrid(pObject->GetGridPosition() ,
			pObject->GetID(),PACKEDDATA_OBJ);
		PACKEDDATA_OBJ->Send();
	}
	
	CGrid* pLastGrid = GetGrid(pObject->GridPos.LastX, pObject->GridPos.LastZ);
	// 새로들어갈 그리드에 있던 애들의 정보를 보q내준다
	CGrid* pCurrentGrid = GetGrid(pObject->GridPos.x,pObject->GridPos.z);
	if(pCurrentGrid)
		pCurrentGrid->SendCurrentObjectInfo(pObject, pLastGrid);
	
	
	// 기존에 있던 그리드의 애들에게 나갔음을 알린다.
	pObject->SetRemoveMsg(Msg,&MsgLen,0);
	if(MsgLen)
	{
		PACKEDDATA_OBJ->Init(g_pServerTable->GetMaxServerConnectionIndex(),Msg,MsgLen);
		AddLastGridToPackedDataExceptCurrentGrid(pObject->GetGridPosition(),
			pObject->GetID(),PACKEDDATA_OBJ);
		PACKEDDATA_OBJ->Send();
	}
	
	CGrid* pCurGrid = GetGrid(pObject->GridPos.x, pObject->GridPos.z);
	// 기존에 있던 애들을 Remove하라는 메세지를 보낸다.	
	pLastGrid = GetGrid(pObject->GridPos.LastX, pObject->GridPos.LastZ);
	if(pLastGrid)
		pLastGrid->SendRemoveObjectInfo(pObject, pCurGrid);
}

void CGeneralGridTable::SendCurrentObjectInfo(int GridNumX,int GridNumZ,int LastGridNumX,int LastGridNumZ,CObject* pNewObject)
{
	CGrid* pLastGrid = GetGrid(LastGridNumX,LastGridNumZ);
	CGrid* pCurGrid = GetGrid(GridNumX,GridNumZ);
	if(pCurGrid)
		pCurGrid->SendCurrentObjectInfo(pNewObject,pLastGrid);
}
void CGeneralGridTable::SendRemoveObjectInfo(int GridNumX,int GridNumZ,int LastGridNumX,int LastGridNumZ,CObject* pLeavedObject)
{
	CGrid* pLastGrid = GetGrid(LastGridNumX,LastGridNumZ);
	CGrid* pCurGrid = GetGrid(GridNumX,GridNumZ);
	if(pLastGrid)
		pLastGrid->SendRemoveObjectInfo(pLeavedObject,pCurGrid);
}


BOOL CGeneralGridTable::GetGridChangeTime(VECTOR3* pStartPos,VECTOR3* pTargetPos,float MoveSpeed,DWORD& dwChangeTime)
{
	int nx = TransToGridCoordinate(pStartPos->x) >> GRID_BIT;
	int nz = TransToGridCoordinate(pStartPos->z) >> GRID_BIT;

	int nAlp_Left = nx * m_GridSize;
	int nAlp_Right = (nx+1) * m_GridSize;
	int nAlp_Top = nz * m_GridSize;
	int nAlp_Bottom = (nz+1) * m_GridSize;	

	float sx = TransToGridCoordinateFloat(pStartPos->x);
	float sz = TransToGridCoordinateFloat(pStartPos->z);
	float tx = TransToGridCoordinateFloat(pTargetPos->x);
	float tz = TransToGridCoordinateFloat(pTargetPos->z);


	float t_left = IsIntersect(float(nAlp_Left),sx,tx);
	float t_right = IsIntersect(float(nAlp_Right),sx,tx);
	float t_top = IsIntersect(float(nAlp_Top),sz,tz);
	float t_bottom = IsIntersect(float(nAlp_Bottom),sz,tz);

	float t_xmin,t_ymin,t_min;
	t_xmin = min(t_left,t_right);
	t_ymin = min(t_top,t_bottom);
	t_min = min(t_xmin,t_ymin);

	if(t_min >= 0 && t_min <= 1)
	{
		//그리드 변화가 있을것이다.
		float distance = CalcDistanceXZ(pStartPos,pTargetPos);
		float totalTime = TransToGridCoordinateFloat(distance) / TransToGridCoordinateFloat(MoveSpeed);

		dwChangeTime = (DWORD)(totalTime * t_min * 1000);	// 천분의 일초 단위로 변환
		++dwChangeTime;		// 오차에 대한 고려

		return TRUE;
	}
	else
		return FALSE;
}

void CGeneralGridTable::OnDelete()
{

}

//////////////////////////////////////////////////////////////////////////
/// 06. 08. 2차 보스 - 이영준
/// 유효그리드내 랜덤으로 타겟 설정
CObject* CGeneralGridTable::FindPlayerRandom(CObject* pObject)
{
	CGrid* pGrid = GetGrid(pObject->GridPos.x, pObject->GridPos.z);
	if(pGrid == NULL)
		return NULL;

	return pGrid->FindPlayerRandom();
}
//////////////////////////////////////////////////////////////////////////