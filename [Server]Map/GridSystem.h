// GridSystem.h: interface for the CGridSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRIDSYSTEM_H__C000AFC9_8D29_4467_9118_DF283166EDA1__INCLUDED_)
#define AFX_GRIDSYSTEM_H__C000AFC9_8D29_4467_9118_DF283166EDA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CObject;
class CGridTable;
class CPackedData;
class CBattle;
struct GridPosition;


enum 
{
	FIND_CLOSE=0, 
	FIND_FIRST,
};
class CGridSystem
{
//	int m_nMaxLength;
	CYHHashTable<CObject> m_TempAddList;
//	CGridTable* m_pGridTable;

	CYHHashTable<CGridTable> m_GridTable;

	void RealAddObject(CObject* pObject,VECTOR3* pPos);

public:
	CGridTable* GetGridTable(CObject * pObject);

	GETINSTANCE(CGridSystem)

	CGridSystem();
	virtual ~CGridSystem();

	void Init();
	void Release();
	
	void CreateGridTable(DWORD GridID, BYTE Kind, int MaxWidth, int MaxHeight, MAPTYPE MapNum);
	void AddGridTable(DWORD ID, BYTE Kind);
	void DeleteGridTable(DWORD GridID);

	void CalcGridPosition(float x,float z,GridPosition* pOutGridPos);

	void GridProcess();		// 임시 리스트에 등록해놓은 녀석들을 검사해 Init된 녀석들만 Add시켜준다.

	void ChangeGrid(CObject* pObject,VECTOR3* pPos);

	void AddObject(CObject* pObject,VECTOR3* pPos);
	void AddObject(CObject* pObject,VECTOR3* pPos, DWORD GridID);
	void RemoveObject(CObject* pObject);

	MAPTYPE GetGridMapNum(DWORD GridID);

	
	// FALSE를 반환하면 그리드가 변하지 않음
	// TRUE를 반환하면 dwChangeTime후에 이동계산과 그리드 계산을 해주어야 한다.
	BOOL GetGridChangeTime(CObject* pObject,VECTOR3* pStartPos,VECTOR3* pTargetPos,float MoveSpeed,DWORD& dwChangeTime);


	//////////////////////////////////////////////////////////////////////////
	// 데이타 전송용
	void AddGridToPackedData(GridPosition* pGridPosition,CPackedData* pPackedData, CObject* pObject);
	void AddGridToPackedDataWithoutOne(GridPosition* pGridPosition,DWORD dwID,CPackedData* pPackedData, CObject* pObject);

	void AddGridToPackedDataExceptLastGrid(GridPosition* pGridPosition,DWORD ExceptID,CPackedData* pPackedData, CObject* pObject);
	void AddLastGridToPackedDataExceptCurrentGrid(GridPosition* pGridPosition,DWORD ExceptID,CPackedData* pPackedData, CObject* pObject);	



	//////////////////////////////////////////////////////////////////////////
	// AI용 주변 에 있는 범위안에 들어온 가장 가까운 플레이어 찾기
	CObject* FindPlayerInRange(VECTOR3* pPos,float Range, CObject* pObject,BYTE flag=FIND_CLOSE);
	CObject* FindMonsterInRange(VECTOR3* pPos,float Range, CObject* pObject,BYTE flag=FIND_CLOSE);

	//////////////////////////////////////////////////////////////////////////
	/// 06. 08. 2차 보스 - 이영준
	/// 유효그리드내 랜덤으로 타겟 설정
	CObject* FindPlayerRandom(CObject* pObject);
	//////////////////////////////////////////////////////////////////////////

};

#endif // !defined(AFX_GRIDSYSTEM_H__C000AFC9_8D29_4467_9118_DF283166EDA1__INCLUDED_)
