// MousePointer.h: interface for the CMousePointer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOUSEPOINTER_H__09369ADA_E0EC_457B_9DB3_CD52A652CB24__INCLUDED_)
#define AFX_MOUSEPOINTER_H__09369ADA_E0EC_457B_9DB3_CD52A652CB24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define __SYSTEMMOUSE__

#include "./Interface/cDialog.h"

class cAni;
class CMousePointer : public cDialog
{
	cAni * m_pAniBasic;
	cAni * m_pAniClick;

public:
	CMousePointer();
	virtual ~CMousePointer();

	void Linking();

	void MonsterAttack();
	void MonsterMouseOver();
	void MonsterLeave();

};

#endif // !defined(AFX_MOUSEPOINTER_H__09369ADA_E0EC_457B_9DB3_CD52A652CB24__INCLUDED_)

