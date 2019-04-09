// SITUATION.h: interface for the SITUATION class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SITUATION_H__A13BC184_5DBE_40B2_A468_B3D72E7CE429__INCLUDED_)
#define AFX_SITUATION_H__A13BC184_5DBE_40B2_A468_B3D72E7CE429__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ConditionObject.h"
#include "SITUATION_exe.h"

class SITUATION : public CConditionObject  
{
public:
	SITUATION();
	virtual ~SITUATION();
	void SetFUNC(CONDITION_FUNC pFunc)	{	m_pFunc = pFunc;	}
	
	virtual		CRETURN Execute(CObject * pObj=NULL);

	CONDITION_FUNC m_pFunc;
};

#endif // !defined(AFX_SITUATION_H__A13BC184_5DBE_40B2_A468_B3D72E7CE429__INCLUDED_)
