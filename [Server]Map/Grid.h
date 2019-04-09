// Grid.h: interface for the CGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRID_H__9DFB8C60_7CEF_43AD_A940_90B800C0E178__INCLUDED_)
#define AFX_GRID_H__9DFB8C60_7CEF_43AD_A940_90B800C0E178__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cLinkedList.h"

class CObject;
class CPackedData;

class CGrid  
{
	CYHHashTable<CObject> m_CharacterHashTable;
//	CYHHashTable<CObject> m_ItemHashTable;
	cLinkedList<CGrid> m_NeighborGrid;
	void AddObjectToPacket(DWORD ExceptID,CPackedData* pPackedData);
	void SendCurrentCharacterMsg(CObject* pNewObject);
//	void SendCurrentItemMsg(CObject* pNewObject);
	void SendRemoveCharacterMsg(CObject* pLeavedObject);
//	void SendRemoveItemMsg(CObject* pLeavedObject);

	BOOL IsNeighborGrid(CGrid* pGrid);

	void FindPlayerInRange(CObject* pFinder,VECTOR3* pPos,float Range,float& MinRange,CObject** ppObject);
	void FindFirstPlayerInRange(CObject* pFinder,VECTOR3* pPos,float Range,CObject** ppObject);

	void FindMonsterInRange(CObject* pFinder,VECTOR3* pPos,float Range,float& MinRange,CObject** ppObject);
	void FindFirstMonsterInRange(CObject* pFinder,VECTOR3* pPos,float Range,CObject** ppObject);
	
public:
	CGrid();
	virtual ~CGrid();

	void AddObject(CObject* pObject);
	void RemoveObject(CObject* pObject);

	void AddNeighborGrid(CGrid* pGrid);

	void AddGridToPackedData(CPackedData* pPackedData);
	void AddGridToPackedDataWithoutOne(DWORD dwID,CPackedData* pPackedData);

	void AddGridToPackedDataExceptLastGrid(DWORD ExceptID,CGrid* pLastGrid,CPackedData* pPackedData);
	
	void SendCurrentObjectInfo(CObject* pNewObject,CGrid* pLastGrid);
	void SendRemoveObjectInfo(CObject* pLeavedObject,CGrid* pCurGrid);
	
	
	CObject* FindPlayerInRange(CObject* pFinder,VECTOR3* pPos,float Range);
	CObject* FindFirstPlayerInRange(CObject* pFinder,VECTOR3* pPos,float Range);

	CObject* FindMonsterInRange(CObject* pFinder,VECTOR3* pPos,float Range);
	CObject* FindFirstMonsterInRange(CObject* pFinder,VECTOR3* pPos,float Range);

	//////////////////////////////////////////////////////////////////////////
	/// 06. 08. 2차 보스 - 이영준
	/// 유효그리드내 랜덤으로 타겟 설정
	CObject*	FindPlayerRandom();
	void		FindPlayerRandom(CObject** ppObject);
	//////////////////////////////////////////////////////////////////////////

};

#endif // !defined(AFX_GRID_H__9DFB8C60_7CEF_43AD_A940_90B800C0E178__INCLUDED_)
