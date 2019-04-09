// GridSystem.cpp: implementation of the CGridSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridSystem.h"
#include "Object.h"
#include "GridTable.h"
#include "PackedData.h"
#include "Network.h"
#include "CharMove.h"
#include "ServerTable.h"
#include "BattleSystem_Server.h"
#include "GeneralGridTable.h"
#include "MurimGridTable.h"
#include "ChannelSystem.h"
#include "Player.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGridSystem::CGridSystem()
{
//	m_pGridTable = NULL;
	m_GridTable.Initialize(10);
}

CGridSystem::~CGridSystem()
{
	Release();
}

void CGridSystem::Init()
{
	m_TempAddList.Initialize(16);
	
//	CreateGridTable(0, eGridType_General, nMaxLength);
	/*	m_pGridTable = new CGridTable[GRID_MAX_LEVEL];
	for(n=0;n<GRID_MAX_LEVEL;++n)
	{
		m_pGridTable[n].Init(GRID_MINIMUM_BIT+n,nMaxLength);
	}*/
}

void CGridSystem::Release()
{
	CGridTable* pInfo;
	m_GridTable.SetPositionHead();
	while(pInfo = m_GridTable.GetData())
	{
		pInfo->Release();
		delete pInfo;
	}
	m_GridTable.RemoveAll();

	m_TempAddList.RemoveAll();
	
/*	if(m_pGridTable)
	{
		for(int n=0;n<GRID_MAX_LEVEL;++n)
		{
			m_pGridTable[n].Release();
		}
		delete [] m_pGridTable;
		m_pGridTable = NULL;
	}
	*/
}

void CGridSystem::DeleteGridTable(DWORD GridID)
{
	CGridTable* pGridTable = (CGridTable*)m_GridTable.GetData(GridID);
	ASSERT(pGridTable);
	pGridTable->OnDelete();
	pGridTable->Release();
	m_GridTable.Remove(GridID);
	delete pGridTable;
}

void CGridSystem::GridProcess()
{
	m_TempAddList.SetPositionHead();
	CObject* pObject;
	cPtrList list;
	while(pObject = (CObject*)m_TempAddList.GetData())
	{
		if(pObject->GetInited() == TRUE)
		{
			list.AddTail( pObject );
//			m_TempAddList.Remove(pObject->GetID());
			RealAddObject(pObject,CCharMove::GetPosition(pObject));
		}
	}

	PTRLISTPOS pos = list.GetHeadPosition();
	while( pos )
	{
		CObject* p = (CObject*)list.GetNext(pos);
		m_TempAddList.Remove(p->GetID());
	}
	list.RemoveAll();
}

void CGridSystem::ChangeGrid(CObject* pObject,VECTOR3* pPos)
{	
	if(pObject->GetInited() ==FALSE)
		return;
	if(pObject->GetInitedGrid() == FALSE)
		return;

	CGridTable* pGridTable = GetGridTable(pObject);
	if(pGridTable == NULL)
		return;

	pGridTable->ChangeGrid(pObject, pPos);

/*	int firstx = TransToGridCoordinate(pPos->x);
	int firstz = TransToGridCoordinate(pPos->z);
	int nx;
	int nz;

	for(int n=0;n<GRID_MAX_LEVEL;++n)
	{
		nx = firstx >> (GRID_MINIMUM_BIT+n);
		nz = firstz >> (GRID_MINIMUM_BIT+n);

		if(nx == pObject->GridPos[n].x && nz == pObject->GridPos[n].z)
			break;
		
		pObject->GridPos[n].LastX = pObject->GridPos[n].x;
		pObject->GridPos[n].LastZ = pObject->GridPos[n].z;
		
		pObject->GridPos[n].x = nx;
		pObject->GridPos[n].z = nz;
		
		pGridTable[n].RemoveObject(pObject->GridPos[n].LastX,pObject->GridPos[n].LastZ,pObject);
		pGridTable[n].AddObject(pObject->GridPos[n].x,pObject->GridPos[n].z,pObject);		
	}

	if(n > GRID_MOVE_LEVEL)		// 이동에 대한 그리드 레벨 이상이 변화 했을때
	{
		static char Msg[1024] = {0,};
		int MsgLen;
		
		// 새로들어갈 그리드에 있던 애들에게 새로 들어 왔음을 알려준다
		PACKEDDATA_OBJ->Init(g_pServerTable->GetMaxServerConnectionIndex());
		AddGridToPackedDataExceptLastGrid(GRID_MOVE_LEVEL,&pObject->GridPos[GRID_MOVE_LEVEL],
			pObject->GetID(),PACKEDDATA_OBJ);
		pObject->SetAddMsg(Msg,&MsgLen,0,FALSE);
		
		if(MsgLen)
			PACKEDDATA_OBJ->Send(Msg,MsgLen);


		// 새로들어갈 그리드에 있던 애들의 정보를 보내준다
		pGridTable[GRID_MOVE_LEVEL].SendCurrentObjectInfo(
			pObject->GridPos[GRID_MOVE_LEVEL].x,pObject->GridPos[GRID_MOVE_LEVEL].z,
			pObject->GridPos[GRID_MOVE_LEVEL].LastX,pObject->GridPos[GRID_MOVE_LEVEL].LastZ,
			pObject);
		

		// 기존에 있던 그리드의 애들에게 나갔음을 알린다.
		PACKEDDATA_OBJ->Init(g_pServerTable->GetMaxServerConnectionIndex());
		AddLastGridToPackedDataExceptCurrentGrid(GRID_MOVE_LEVEL,&pObject->GridPos[GRID_MOVE_LEVEL],
			pObject->GetID(),PACKEDDATA_OBJ);		
		pObject->SetRemoveMsg(Msg,&MsgLen,0);
		
		if(MsgLen)
			PACKEDDATA_OBJ->Send(Msg,MsgLen);


		// 기존에 있던 애들을 Remove하라는 메세지를 보낸다.		
		pGridTable[GRID_MOVE_LEVEL].SendRemoveObjectInfo(
			pObject->GridPos[GRID_MOVE_LEVEL].x,pObject->GridPos[GRID_MOVE_LEVEL].z,
			pObject->GridPos[GRID_MOVE_LEVEL].LastX,pObject->GridPos[GRID_MOVE_LEVEL].LastZ,
			pObject);
	}
	*/
}

void CGridSystem::RealAddObject(CObject* pObject,VECTOR3* pPos)
{
	CGridTable* pGridTable;
	pGridTable = GetGridTable(pObject);
	if(pGridTable == NULL)
		return;

	pGridTable->AddObject(pObject, pPos);
	
	static char Msg[1024] = {0,};
	WORD MsgLen;
	
	pObject->SetAddMsg(Msg,&MsgLen,0,TRUE);
	
	if(MsgLen)
	{
		PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pObject,(MSGBASE*)Msg,MsgLen);
	}
	
	pGridTable->SendCurrentObjectInfo(
			pObject->GridPos.x,pObject->GridPos.z,-1,-1,pObject);
	
	pObject->SetInitedGrid();
}

void CGridSystem::AddObject(CObject* pObject,VECTOR3* pPos)
{
	if(pObject->GetObjectKind() == eObjectKind_Player)
		m_TempAddList.Add(pObject,pObject->GetID());
	else
		RealAddObject(pObject,pPos);
}

void CGridSystem::RemoveObject(CObject* pObject)
{
	if(m_TempAddList.GetData(pObject->GetID()))
		m_TempAddList.Remove(pObject->GetID());
	else
	{
		if(pObject->m_bGridInited == FALSE)
			return;
		
		static char Msg[1024] = {0,};
		WORD MsgLen;
		
		CGridTable* pGridTable = GetGridTable(pObject);
		if(pGridTable == NULL)
			return;
		pObject->SetRemoveMsg(Msg,&MsgLen,0);
		if(MsgLen)
		{
			PACKEDDATA_OBJ->QuickSendExceptObjectSelf(pObject,(MSGBASE*)Msg,MsgLen);
		}
		
		pGridTable->RemoveObject(pObject);
		pObject->GridPos.x = -1;
		pObject->GridPos.z = -1;
		pObject->GridPos.LastX = -1;
		pObject->GridPos.LastZ = -1;

		pObject->m_bGridInited = FALSE;
	}
}

void CGridSystem::AddGridToPackedData(GridPosition* pGridPosition,CPackedData* pPackedData, CObject* pObject)
{
	CGridTable * pGridTable = GetGridTable(pObject);
	if(pGridTable == NULL)
		return;
	pGridTable->AddGridToPackedData(pGridPosition, pPackedData);
//	m_pGridTable[Level].AddGridToPackedData(GridNumX,GridNumZ,pPackedData);
}

void CGridSystem::AddGridToPackedDataWithoutOne(GridPosition* pGridPosition,DWORD dwID,CPackedData* pPackedData, CObject* pObject)
{
	CGridTable * pGridTable = GetGridTable(pObject);
	if(pGridTable == NULL)
		return;
	pGridTable->AddGridToPackedDataWithoutOne(pGridPosition, dwID, pPackedData);
	
//	m_pGridTable[Level].AddGridToPackedDataWithoutOne(GridNumX,GridNumZ,dwID,pPackedData);
}

void CGridSystem::AddGridToPackedDataExceptLastGrid(GridPosition* pGridPosition,DWORD ExceptID,CPackedData* pPackedData, CObject* pObject)
{	
	CGridTable * pGridTable = GetGridTable(pObject);
	if(pGridTable == NULL)
		return;
	pGridTable->AddGridToPackedDataExceptLastGrid(pGridPosition, ExceptID, pPackedData);
	
//	m_pGridTable[Level].AddGridToPackedDataExceptLastGrid(pGridPosition,ExceptID,pPackedData);
}

void CGridSystem::AddLastGridToPackedDataExceptCurrentGrid(GridPosition* pGridPosition,DWORD ExceptID,CPackedData* pPackedData, CObject* pObject)
{
	CGridTable * pGridTable = GetGridTable(pObject);
	if(pGridTable == NULL)
		return;
	pGridTable->AddLastGridToPackedDataExceptCurrentGrid(pGridPosition,ExceptID,pPackedData);

//	m_pGridTable[Level].AddLastGridToPackedDataExceptCurrentGrid(pGridPosition,ExceptID,pPackedData);
}

CObject* CGridSystem::FindPlayerInRange(VECTOR3* pPos,float Range, CObject* pObject, BYTE flag)
{
	int firstx = TransToGridCoordinate(pPos->x);
	int firstz = TransToGridCoordinate(pPos->z);

	GridPosition GridPos;
	GridPos.x = firstx >> GRID_BIT;
	GridPos.z = firstz >> GRID_BIT;
	CGridTable * pGridTable = GetGridTable(pObject);
	if(pGridTable == NULL)
		return NULL;

	return pGridTable->FindPlayerInRange(pObject,&GridPos, pPos, Range, flag);
}
CObject* CGridSystem::FindMonsterInRange(VECTOR3* pPos,float Range, CObject* pObject,BYTE flag)
{
	int firstx = TransToGridCoordinate(pPos->x);
	int firstz = TransToGridCoordinate(pPos->z);

	GridPosition GridPos;
	GridPos.x = firstx >> GRID_BIT;
	GridPos.z = firstz >> GRID_BIT;
	CGridTable * pGridTable = GetGridTable(pObject);
	if(pGridTable == NULL)
		return NULL;

	return pGridTable->FindMonsterInRange(pObject,&GridPos, pPos, Range, flag);
}

CGridTable* CGridSystem::GetGridTable(CObject* pObject)
{
	DWORD GridID = pObject->GetGridID();
	CGridTable* pGridTable = (CGridTable*)m_GridTable.GetData(GridID);
	ASSERT(pGridTable);
	
	return pGridTable;
}

MAPTYPE CGridSystem::GetGridMapNum(DWORD GridID)
{
	CGridTable* pGridTable = (CGridTable*)m_GridTable.GetData(GridID);
	ASSERT(pGridTable);
	
	return pGridTable->GetMapNum();
}

void CGridSystem::CreateGridTable(DWORD GridID, BYTE Kind, int MaxWidth, int MaxHeight, MAPTYPE MapNum)
{
	CGridTable* pGridTable = (CGridTable*)m_GridTable.GetData(GridID);
	ASSERT(pGridTable == NULL);

	CGridTable* pInfo = NULL;
	switch(Kind)
	{
	case eGridType_General:
		{
			pInfo = new CGeneralGridTable;
			pInfo->Init(GridID, MaxWidth, MaxHeight);
		}
		break;
	case eGridType_Murim:
		{
			pInfo = new CMurimGridTable;
			pInfo->Init(GridID, MaxWidth, MaxHeight);
		}
		break;
	}
	ASSERT(pInfo);
	if(pInfo == NULL)
		return;

	pInfo->SetMapNum(MapNum);

	m_GridTable.Add(pInfo, GridID);
}

BOOL CGridSystem::GetGridChangeTime(CObject* pObject,VECTOR3* pStartPos,VECTOR3* pTargetPos,float MoveSpeed,DWORD& dwChangeTime)
{
	CGridTable * pGridTable = GetGridTable(pObject);
	if(pGridTable == NULL)
		return FALSE;
	return pGridTable->GetGridChangeTime(pStartPos,pTargetPos,MoveSpeed,dwChangeTime);
}

//////////////////////////////////////////////////////////////////////////
/// 06. 08. 2차 보스 - 이영준
/// 유효그리드내 랜덤으로 타겟 설정
CObject* CGridSystem::FindPlayerRandom(CObject* pObject)
{
	CGridTable * pGridTable = GetGridTable(pObject);
	if(pGridTable == NULL)
		return FALSE;

	return pGridTable->FindPlayerRandom(pObject);
}	
//////////////////////////////////////////////////////////////////////////