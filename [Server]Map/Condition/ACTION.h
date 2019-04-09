// ACTION.h: interface for the CACTION class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACTION_H__61C768F3_C084_468A_8F2F_63BD28E17E9B__INCLUDED_)
#define AFX_ACTION_H__61C768F3_C084_468A_8F2F_63BD28E17E9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "ACTION_exe.h"

class CACTION  
{
public:
	CACTION();
	virtual ~CACTION();

	void Execute(CObject * pObj)	{	if(m_pFunc) m_pFunc(pObj); }
	
	void SetFUNC(ACTION_FUNC pFunc)	{	m_pFunc = pFunc;	}

	ACTION_FUNC m_pFunc;
};

#endif // !defined(AFX_ACTION_H__61C768F3_C084_468A_8F2F_63BD28E17E9B__INCLUDED_)
