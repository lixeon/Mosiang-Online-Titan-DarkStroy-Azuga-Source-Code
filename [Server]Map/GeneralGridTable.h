// GeneralGridTable.h: interface for the CGeneralGridTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GENERALGRIDTABLE_H__74802441_A2F9_4E3B_81A8_23966927225A__INCLUDED_)
#define AFX_GENERALGRIDTABLE_H__74802441_A2F9_4E3B_81A8_23966927225A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GridTable.h"

class CPackedData;
class CGeneralGridTable : public CGridTable  
{
	CGrid** m_ppGrid;
	int m_GridNum;
	int m_GridSize;

	int m_nMaxLength;
	
	WORD m_ChannelID;

public:
	CGeneralGridTable();
	virtual ~CGeneralGridTable();

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

	//////////////////////////////////////////////////////////////////////////
	/// 06. 08. 2차 보스 - 이영준
	/// 유효그리드내 랜덤으로 타겟 설정
	virtual CObject* FindPlayerRandom(CObject* pObject);
	//////////////////////////////////////////////////////////////////////////
};
extern int TransToGridCoordinate(float val);
extern float TransToGridCoordinateFloat(float val);
#endif // !defined(AFX_GENERALGRIDTABLE_H__74802441_A2F9_4E3B_81A8_23966927225A__INCLUDED_)
