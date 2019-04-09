// MurimGridTable.h: interface for the CMurimGridTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MURIMGRIDTABLE_H__1FC90610_D08C_4FF7_92CA_EA1FD5D93281__INCLUDED_)
#define AFX_MURIMGRIDTABLE_H__1FC90610_D08C_4FF7_92CA_EA1FD5D93281__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GridTable.h"

class CMurimGridTable : public CGridTable
{
	CGrid* m_pGrid;
public:
	CMurimGridTable();
	virtual ~CMurimGridTable();

	void Init(DWORD GridID,int MaxWidth,int MaxHeight);
	void Release();

	virtual void AddObject(CObject* pObject, VECTOR3* pPos);
	virtual void RemoveObject(CObject* pObject);

	virtual void CalcGridPosition(float x,float z,GridPosition* pOutGridPos);
	virtual CGrid* GetGrid(WORD x, WORD z);
	virtual CObject* FindPlayerInRange(CObject* pFinder,GridPosition* pGridPosition,VECTOR3* pPos,float Range, BYTE flag);
	virtual CObject* FindMonsterInRange(CObject* pFinder,GridPosition* pGridPosition,VECTOR3* pPos,float Range, BYTE flag);
	virtual void ChangeGrid(CObject* pObject, VECTOR3* pPos);
	
	virtual void AddGridToPackedData(GridPosition* pGridPosition,CPackedData* pPackedData);
	virtual void AddGridToPackedDataWithoutOne(GridPosition* pGridPosition,DWORD dwID,CPackedData* pPackedData);
	virtual void AddGridToPackedDataExceptLastGrid(GridPosition* pGridPosition,DWORD ExceptID,CPackedData* pPackedData);
	virtual void AddLastGridToPackedDataExceptCurrentGrid(GridPosition* pGridPosition,DWORD ExceptID,CPackedData* pPackedData);

	virtual void SendCurrentObjectInfo(int GridNumX,int GridNumZ,int LastGridNumX,int LastGridNumZ,CObject* pNewObject);
	virtual void SendRemoveObjectInfo(int GridNumX,int GridNumZ,int LastGridNumX,int LastGridNumZ,CObject* pLeavedObject);

	virtual BOOL GetGridChangeTime(VECTOR3* pStartPos,VECTOR3* pTargetPos,float MoveSpeed,DWORD& dwChangeTime);
	virtual void OnDelete();
};

#endif // !defined(AFX_MURIMGRIDTABLE_H__1FC90610_D08C_4FF7_92CA_EA1FD5D93281__INCLUDED_)
