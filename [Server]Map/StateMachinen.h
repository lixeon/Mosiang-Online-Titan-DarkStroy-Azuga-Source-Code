// StateMachinen.h: interface for the CStateMachinen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATEMACHINEN_H__A94AC7CA_F86E_439E_A95B_A9031EBCBDCB__INCLUDED_)
#define AFX_STATEMACHINEN_H__A94AC7CA_F86E_439E_A95B_A9031EBCBDCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AIDefine.h"
class MSG_OBJECT;

class CStateMachinen  
{

public:
	CStateMachinen();
	virtual ~CStateMachinen();

	void SetState(CObject * obj, DWORD newState );
	void Update(CObject * obj, eStateEvent evt, MSG_OBJECT * msg);
	void Process(CObject * obj, eStateEvent evt, MSG_OBJECT * msg);
	void SubProcess( CObject * obj, eStateEvent evt, MSG_OBJECT * msg );


	void DoStand( CObject * pObject, eStateEvent evt, MSG_OBJECT * msg );
	void DoWalkAround( CObject * pObject, eStateEvent evt, MSG_OBJECT * msg );
	void DoPursuit( CObject * pObject, eStateEvent evt, MSG_OBJECT * msg );
	void DoAttack( CObject * pObject, eStateEvent evt, MSG_OBJECT * msg );
	void DoRunAway( CObject * pObject, eStateEvent evt, MSG_OBJECT * msg );
	void DoRest( CObject * pObject, eStateEvent evt, MSG_OBJECT * msg );
	
	void RecvMsg( CObject * pSrcObject, CObject * pDestObject, MSG_OBJECT * msg);
	
};

extern CStateMachinen GSTATEMACHINE;
#endif // !defined(AFX_STATEMACHINEN_H__A94AC7CA_F86E_439E_A95B_A9031EBCBDCB__INCLUDED_)
