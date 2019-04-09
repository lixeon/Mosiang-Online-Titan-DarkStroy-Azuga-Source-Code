// GridTable.h: interface for the CGridTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDTABLE_H__E9B024E2_95D5_4B3C_902E_5345F31CBF3F__INCLUDED_)
#define AFX_GRIDTABLE_H__E9B024E2_95D5_4B3C_902E_5345F31CBF3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGrid;
class CObject;
class CPackedData;
struct GridPosition;

class CGridTable  
{
protected:
	BYTE m_Kind;
	DWORD m_ID;
	MAPTYPE m_MapNum;
	
	virtual CGrid* GetGrid(WORD x, WORD z) = 0;
public:
	CGridTable();
	virtual ~CGridTable();
	
	virtual	void Init(DWORD GridID,int MaxWidth,int MaxHeight) = 0;
	virtual DWORD GetGridID() { return m_ID;	}
	virtual void CalcGridPosition(float x,float z,GridPosition* pOutGridPos) = 0;

	virtual void AddObject(CObject* pObject, VECTOR3* pPos) = 0;
	virtual void RemoveObject(CObject* pObject) = 0;

	virtual void AddGridToPackedData(GridPosition* pGridPosition,CPackedData* pPackedData) = 0;
	virtual void AddGridToPackedDataWithoutOne(GridPosition* pGridPosition,DWORD dwID,CPackedData* pPackedData) = 0;
	
	virtual void AddGridToPackedDataExceptLastGrid(GridPosition* pGridPosition,DWORD ExceptID,CPackedData* pPackedData)=0;
	virtual void AddLastGridToPackedDataExceptCurrentGrid(GridPosition* pGridPosition,DWORD ExceptID,CPackedData* pPackedData);

	virtual void SendCurrentObjectInfo(int GridNumX,int GridNumZ,int LastGridNumX,int LastGridNumZ,CObject* pNewObject)=0;
	virtual void SendRemoveObjectInfo(int GridNumX,int GridNumZ,int LastGridNumX,int LastGridNumZ,CObject* pLeavedObject)=0;
	
	// FALSE를 반환하면 그리드가 변하지 않음
	// TRUE를 반환하면 dwChangeTime후에 이동계산과 그리드 계산을 해주어야 한다.
	virtual BOOL GetGridChangeTime(VECTOR3* pStartPos,VECTOR3* pTargetPos,float MoveSpeed,DWORD& dwChangeTime) = 0;
	
	
	virtual CObject* FindPlayerInRange(CObject* pFinder,GridPosition* pGridPosition,VECTOR3* pPos,float Range, BYTE flag) = 0;
	virtual CObject* FindMonsterInRange(CObject* pFinder,GridPosition* pGridPosition,VECTOR3* pPos,float Range, BYTE flag) = 0;

	virtual BYTE GetKind() { return  m_Kind; };

	virtual void Release() = 0;
	virtual void ChangeGrid(CObject* pObject, VECTOR3* pPos) = 0;
	
	virtual void OnDelete() = 0; 

	void SetMapNum(MAPTYPE mapnum)	{	m_MapNum = mapnum;	}
	MAPTYPE GetMapNum()	{	return m_MapNum;	}


/*	
	void Init(int Bits,int MaxLength);
	void Release();
	
	void AddObject(int nx,int nz,CObject* pObject);
	void RemoveObject(int nx,int nz,CObject* pObject);

	void ChangeGrid(int nx,int nz,CObject* pObject);

	void AddGridToPackedData(int GridNumX,int GridNumZ,CPackedData* pPackedData);
	void AddGridToPackedDataWithoutOne(int GridNumX,int GridNumZ,DWORD dwID,CPackedData* pPackedData);
	
	void AddGridToPackedDataExceptLastGrid(GridPosition* pGridPosition,DWORD ExceptID,CPackedData* pPackedData);
	void AddLastGridToPackedDataExceptCurrentGrid(GridPosition* pGridPosition,DWORD ExceptID,CPackedData* pPackedData);

	void SendCurrentObjectInfo(int GridNumX,int GridNumZ,int LastGridNumX,int LastGridNumZ,CObject* pNewObject);
	void SendRemoveObjectInfo(int GridNumX,int GridNumZ,int LastGridNumX,int LastGridNumZ,CObject* pLeavedObject);
	
	
	
	CObject* FindPlayerInRange(int GridNumX,int GridNumZ,VECTOR3* pPos,float Range, BYTE flag);
*/

	//////////////////////////////////////////////////////////////////////////
	/// 06. 08. 2차 보스 - 이영준
	/// 유효그리드내 랜덤으로 타겟 설정
	virtual CObject* FindPlayerRandom(CObject* pObject);
	//////////////////////////////////////////////////////////////////////////
};

#endif // !defined(AFX_GRIDTABLE_H__E9B024E2_95D5_4B3C_902E_5345F31CBF3F__INCLUDED_)

