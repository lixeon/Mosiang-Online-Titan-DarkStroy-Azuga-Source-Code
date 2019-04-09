// AIUniqueGroup.h: interface for the CAIUniqueGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AIUNIQUEGROUP_H__BC5C15A4_D129_42E3_B34A_FC46370BFC0D__INCLUDED_)
#define AFX_AIUNIQUEGROUP_H__BC5C15A4_D129_42E3_B34A_FC46370BFC0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AIGroupPrototype.h"

class CAIUniqueGroup : public CAIGroup  
{
public:
	CAIUniqueGroup();
	virtual ~CAIUniqueGroup();
	virtual void RegenCheck();
};

#endif // !defined(AFX_AIUNIQUEGROUP_H__BC5C15A4_D129_42E3_B34A_FC46370BFC0D__INCLUDED_)
