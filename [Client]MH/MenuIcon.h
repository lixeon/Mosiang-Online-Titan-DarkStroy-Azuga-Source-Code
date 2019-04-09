// MenuIcon.h: interface for the CMenuIcon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENUICON_H__D1CB4315_451A_4445_BE93_E4C16693704D__INCLUDED_)
#define AFX_MENUICON_H__D1CB4315_451A_4445_BE93_E4C16693704D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Interface/cIcon.h"

#define INVALID_PAGE	0xffff

class CMenuIcon : public cIcon
{
protected:

	WORD	m_wPage;			//MenuSlotDialogÀÇ Page
	WORD	m_wAbilityIdx;

public:
	CMenuIcon();
	virtual ~CMenuIcon();

	void SetSlotPage( WORD wPage )			{ m_wPage = wPage; }
	WORD GetSlotPage()						{ return m_wPage; }

	void SetAbilityIdx( WORD wAbilityIdx )	{ m_wAbilityIdx = wAbilityIdx; }
	WORD GetAbilityIdx()					{ return m_wAbilityIdx; }

};

#endif // !defined(AFX_MENUICON_H__D1CB4315_451A_4445_BE93_E4C16693704D__INCLUDED_)
