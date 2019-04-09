// JinBub.h: interface for the CJinBub class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JINBUB_H__259D1FE7_67C8_421D_872B_8C1F638B4A85__INCLUDED_)
#define AFX_JINBUB_H__259D1FE7_67C8_421D_872B_8C1F638B4A85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MugongBase.h"

class CJinBub : public CMugongBase  
{
public:
	CJinBub();
	virtual ~CJinBub();
	virtual void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);

};

#endif // !defined(AFX_JINBUB_H__259D1FE7_67C8_421D_872B_8C1F638B4A85__INCLUDED_)
