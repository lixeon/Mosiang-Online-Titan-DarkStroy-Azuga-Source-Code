// RegenToolDoc.h : interface of the CRegenToolDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGENTOOLDOC_H__6AED26BE_0BF4_40B3_A511_45BF436A72E1__INCLUDED_)
#define AFX_REGENTOOLDOC_H__6AED26BE_0BF4_40B3_A511_45BF436A72E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DefineStruct.h"
#include <list>

using namespace std ;

typedef list<MONSTER_POINT*> LISTMONSTER;
typedef list<LIST_CONDITION*> LISTCONDITION;

class CRegenToolDoc : public CDocument
{
	LISTMONSTER m_MList;
	LISTCONDITION m_ListCondition;
	LISTMONSTER m_RecvMonsterList;

	BOOL m_ListConditionFlag;
	WORD m_nCount;
	char* FileName;
	WORD m_SelectArea;
protected: // create from serialization only
	CRegenToolDoc();
	DECLARE_DYNCREATE(CRegenToolDoc)

// Attributes
public:
	LISTMONSTER::iterator it;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegenToolDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	void AddCount(){m_nCount++;}
	void SubCount(){m_nCount--;}
	WORD GetCount(){return m_nCount;}
	LISTMONSTER& GetMonsterList();
	LISTCONDITION& GetConditionList();
	
	// ConditionList
	void AddProperty(CString GroupNum, ADD_PROPERTY* addproperty);
	void AddGroupMonsterCount(CString GroupNum);
	void ReleaseGroupMonsterCount(CString GroupNum);
	void AddCondition(LIST_CONDITION* condition);
	BOOL AddMonsterCheck(CString GroupNum);
	LIST_CONDITION* FindGroupCondition(CString GroupNum);
	LIST_CONDITION* GetFirstGroupCondition();
	void RemoveAllConditionList();
	
	// MonsterList
	void AddSelectMonsterPoint(MONSTER_POINT* mp);
	void DeleteSelectMonsterPoint(void* mp);
	MONSTER_POINT* FindSelectmonsterPoint(VECTOR3* ppos);
	MONSTER_POINT* FindSelectMonsterHandle(GXOBJECT_HANDLE handle);
	void FindSearchOfGroup(WORD GroupNum, LISTMONSTER* pRecvMonsterList);
	CPoint* FindOfSelNum(int SelNum);
	void RemoveAllMonsterList();
	virtual ~CRegenToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRegenToolDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGENTOOLDOC_H__6AED26BE_0BF4_40B3_A511_45BF436A72E1__INCLUDED_)
