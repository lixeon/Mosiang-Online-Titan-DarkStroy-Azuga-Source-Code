// ConditionObjectManager.h: interface for the CConditionObjectManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONDITIONOBJECTMANAGER_H__FC374C23_B293_4BD7_A141_4B853247DED4__INCLUDED_)
#define AFX_CONDITIONOBJECTMANAGER_H__FC374C23_B293_4BD7_A141_4B853247DED4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "SITUATION_exe.h"

#define CONDITIONOBJMGR	CConditionObjectManager::GetInstance()

class CConditionObjectManager  
{
public:
	CConditionObjectManager();
	virtual ~CConditionObjectManager();
	GETINSTANCE(CConditionObjectManager)


	void		LoadOPDATA();
	
	BOOL		AddOPDATA( void * pDataObj, DWORD opCode );
	void *		GetOPDATA( DWORD opCode );
	void		DeleteAllOPDATA();

	BOOL		Evaluate(DWORD opCode1, DWORD oprator, DWORD opCode2 );

	CONDITION_FUNC	GetSITUATIONFunc(SITUATIONIDX idx);
private:
	void ** m_ppOPDATAList;
	WORD	m_wMaxOPDATANum;

};

#endif // !defined(AFX_CONDITIONOBJECTMANAGER_H__FC374C23_B293_4BD7_A141_4B853247DED4__INCLUDED_)
