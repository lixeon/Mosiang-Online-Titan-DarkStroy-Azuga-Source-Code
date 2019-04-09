// RESOURCE.h: interface for the RESOURCE class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESOURCE_H__65E64A5B_23A7_4E43_953F_4108A6F50AF4__INCLUDED_)
#define AFX_RESOURCE_H__65E64A5B_23A7_4E43_953F_4108A6F50AF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ConditionObject.h"

class RESOURCE : public CConditionObject  
{
public:
	RESOURCE();
	virtual ~RESOURCE();

	virtual		CRETURN Execute(CObject * pObj=NULL);
};

#endif // !defined(AFX_RESOURCE_H__65E64A5B_23A7_4E43_953F_4108A6F50AF4__INCLUDED_)
