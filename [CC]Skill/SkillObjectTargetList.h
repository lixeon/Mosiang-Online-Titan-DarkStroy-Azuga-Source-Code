// SkillObjectTargetList.h: interface for the CSkillObjectTargetList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SKILLOBJECTTARGETLIST_H__797940D9_3731_4FDF_96C0_939A657AD949__INCLUDED_)
#define AFX_SKILLOBJECTTARGETLIST_H__797940D9_3731_4FDF_96C0_939A657AD949__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SOTL_NOTCHANGED		0
#define SOTL_ADDED			1
#define SOTL_REMOVED		2

class CSkillObject;
class CObject;

#include "MemoryPoolTempl.h"

struct STLIST
{
	CObject* pObject;
	BYTE bTargetKind;	// SKILLRESULTKIND_NONE (0)
						// SKILLRESULTKIND_POSITIVE	(1)
						// SKILLRESULTKIND_NEGATIVE (2)
};

extern CMemoryPoolTempl<STLIST> g_PoolSTLIST;

class CSkillObjectTargetList  
{
protected:
	CSkillObject* m_pSkillObject;

	CYHHashTable<STLIST> m_TargetTable;
	BOOL m_bPositionSetHead;
	WORD m_PositiveTargetType;
	WORD m_NegativeTargetType;
	WORD m_SkillRange;
	WORD m_SkillAreaIdx;

	virtual void DoRelease(CSkillObject* pSkillObject) = 0;
	virtual void DoInitTargetList(CSkillObject* pSkillObject,CTargetList* pTList) {};


public:
	CSkillObjectTargetList(SKILLINFO* pSkillInfo);
	virtual ~CSkillObjectTargetList();
	
	void SetSkillObject(CSkillObject* pSkillObject)	{ m_pSkillObject = pSkillObject;	}
		
	DWORD UpdateTargetList(CObject* pObject);

	void SetPositionHead();
	STLIST* GetNextTargetList();

	CObject* GetNextTarget();
	BOOL GetNextTarget(WORD PNTarget,CObject** ppObjectOut,BYTE* pTargetKindOut);	
	BOOL GetNextNegativeTarget(CObject** ppObjectOut,BYTE* pTargetKindOut);
	BOOL GetNextPositiveTarget(CObject** ppObjectOut,BYTE* pTargetKindOut);	

	BOOL IsInTargetList(CObject* pObject);
	BOOL IsInTargetList(DWORD dwObjectID);
	BYTE GetTargetKind(DWORD dwObjectID);

	void InitTargetList(CSkillObject* pSkillObject,CTargetList* pTList,CObject* pAddtionalTarget,MAINTARGET* pMainTarget);
	void Release(CSkillObject* pSkillObject);
	
	STLIST* MakeNewSTLIST(CObject* pObject,BYTE bTargetKind);
	
	inline DWORD GetTargetNum()	{	return m_TargetTable.GetDataNum();	}

	virtual BYTE AddTargetObject(CObject* pObject,BYTE bTargetKind);
	virtual BYTE RemoveTargetObject(DWORD ObjectID);

	virtual BOOL IsInTargetArea(CObject* pObject) = 0;
	virtual void SetSkillObjectPosition(VECTOR3* pPos) = 0;
	virtual void GetSkillObjectPosition(VECTOR3* pPos) {}
};

#endif // !defined(AFX_SKILLOBJECTTARGETLIST_H__797940D9_3731_4FDF_96C0_939A657AD949__INCLUDED_)
