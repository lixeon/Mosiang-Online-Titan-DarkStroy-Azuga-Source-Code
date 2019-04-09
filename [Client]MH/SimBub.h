// SimBub.h: interface for the CSimBub class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMBUB_H__45E63B13_67BE_44E9_B6D9_D85091A29444__INCLUDED_)
#define AFX_SIMBUB_H__45E63B13_67BE_44E9_B6D9_D85091A29444__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MugongBase.h"

class CSimBub : public CMugongBase  
{
public:
	CSimBub();
	virtual ~CSimBub();
	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);
	
};

#endif // !defined(AFX_SIMBUB_H__45E63B13_67BE_44E9_B6D9_D85091A29444__INCLUDED_)
