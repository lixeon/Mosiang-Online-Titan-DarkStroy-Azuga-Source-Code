// ConditionObject.h: interface for the CConditionObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONDITIONOBJECT_H__0BDCC805_612C_4D15_9DB0_E4EA3FC2AD4F__INCLUDED_)
#define AFX_CONDITIONOBJECT_H__0BDCC805_612C_4D15_9DB0_E4EA3FC2AD4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ConditionNode.h"

class CObject;

class CConditionObject  
{
public:
	CConditionObject();
	virtual ~CConditionObject();


	virtual		CRETURN Execute(CObject * pObj=NULL) = 0;

};

#endif // !defined(AFX_CONDITIONOBJECT_H__0BDCC805_612C_4D15_9DB0_E4EA3FC2AD4F__INCLUDED_)
