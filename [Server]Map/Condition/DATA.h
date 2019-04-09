// DATA.h: interface for the DATA class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATA_H__9CF494A6_8EA8_4357_864C_363DDF9D43E8__INCLUDED_)
#define AFX_DATA_H__9CF494A6_8EA8_4357_864C_363DDF9D43E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ConditionObject.h"

class OPBASE;

class DATA : public CConditionObject  
{
public:
	DATA();
	virtual ~DATA();
	
	DWORD		op1Code;
	DWORD		op2Code;
	DWORD		oprator;
	
	virtual		CRETURN Execute(CObject * pObj=NULL);
};

#endif // !defined(AFX_DATA_H__9CF494A6_8EA8_4357_864C_363DDF9D43E8__INCLUDED_)
