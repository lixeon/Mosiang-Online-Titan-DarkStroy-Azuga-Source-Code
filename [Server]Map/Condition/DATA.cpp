// DATA.cpp: implementation of the DATA class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DATA.h"
#include "OPDATA.h"
#include "ConditionObjectManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DATA::DATA()
{

}

DATA::~DATA()
{

}

CRETURN DATA::Execute(CObject * pObj)
{
	if( CONDITIONOBJMGR->Evaluate( op1Code, oprator, op2Code ) )
		return RETURN_TRUE;
	else
		return RETURN_FALSE;
}
