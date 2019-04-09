// TargetListIterator.h: interface for the CTargetListIterator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TARGETLISTITERATOR_H__14C6E8D8_344C_4588_87B7_43B4DBCB0ABA__INCLUDED_)
#define AFX_TARGETLISTITERATOR_H__14C6E8D8_344C_4588_87B7_43B4DBCB0ABA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTargetList;
#include "TargetData.h"

class CTargetListIterator  
{
	enum mode
	{
		TLMODE_NONE,
		TLMODE_INITED,
	};
	int m_CurMode;

	CTargetList* m_pTargetList;
	int m_PositionOfList;
	CTargetData* m_pCurTargetData;

	int m_DataNum;
	void ShiftToNext();
	
// for insert
	void AddTarget(DWORD id,WORD flag,BYTE kind);
	void AddTargetData(DWORD data);

public:
	CTargetListIterator();
	CTargetListIterator(CTargetList* pTargetList);
	~CTargetListIterator();

	void Init(CTargetList* pTargetList);
	void Release();
	
// for insert
	void AddTargetWithNoData(DWORD id,BYTE kind);
	void AddTargetWithResultInfo(DWORD id,BYTE kind,RESULTINFO* pDamageInfo);

// for access
	BOOL GetNextTarget();
	DWORD GetTargetID();
	WORD GetTargetFlag();
	BYTE GetTargetKind();
	void CTargetListIterator::GetTargetData(RESULTINFO* pResultInfo);
};

#endif // !defined(AFX_TARGETLISTITERATOR_H__14C6E8D8_344C_4588_87B7_43B4DBCB0ABA__INCLUDED_)
