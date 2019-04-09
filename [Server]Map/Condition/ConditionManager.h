// ConditionManager.h: interface for the CConditionManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONDITIONMANAGER_H__B157EBDE_CBE0_4D15_ACC1_C30B2AF718A5__INCLUDED_)
#define AFX_CONDITIONMANAGER_H__B157EBDE_CBE0_4D15_ACC1_C30B2AF718A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "ConditionNode.h"
#include "MHFile.h"


#define CONDITIONMGR	CConditionManager::GetInstance()


#define CONDITIONLIST_PATH		"./Resource/Condition_List.txt"

class CConditionNode;


class CConditionManager  
{
public:
	CConditionManager();
	virtual ~CConditionManager();
	GETINSTANCE(CConditionManager)

	void Initial();
	void Release();
	void Process();
	void LoadConditionList();
	void GarbageProcess();
	void AddListFromPool( DWORD id );
	void RemoveListToPool( DWORD id );
protected:
	void ReleaseSubNode( CConditionNode * RootNode );
	void ReleaseSubActionNode( CACTION * RootNode );
	void AddRootNodeInList( CConditionNode * RootNode, DWORD id );
	void LoadRootNodeInfo( CMHFile* fp, CConditionNode* pRootNode );
	void LoadSubNodeInfo( CMHFile* fp, CConditionNode* pNode, CRETURN bCondition );
	void LoadConditionObject( CMHFile* fp, CConditionNode* pNode );

	CYHHashTable<CConditionNode>	m_ConditionList;
	CYHHashTable<CConditionNode>	m_ConditionPoolList;

	//taiyo: CS로 처리하건 list로 처리하든 필요함
	DWORD							m_addID;
	DWORD							m_removeID;
	//cPtrList m_ConditionList;
};

#endif // !defined(AFX_CONDITIONMANAGER_H__B157EBDE_CBE0_4D15_ACC1_C30B2AF718A5__INCLUDED_)

