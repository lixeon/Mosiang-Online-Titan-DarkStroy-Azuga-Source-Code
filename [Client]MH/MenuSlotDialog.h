// MenuSlotDialog.h: interface for the CMenuSlotDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENUSLOTDIALOG_H__FB968505_C5A7_4191_A367_72AF66EBC061__INCLUDED_)
#define AFX_MENUSLOTDIALOG_H__FB968505_C5A7_4191_A367_72AF66EBC061__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "./Interface/cDialog.h"

class cIconGridDialog;
class CMenuIcon;

class CMenuSlotDialog : public cDialog
{
protected:
	cIconGridDialog* m_pigdSheet[TAB_MENUSLOT_NUM];
	int	m_nCurPage;
	
public:
	CMenuSlotDialog();
	virtual ~CMenuSlotDialog();

	void Linking();
	virtual void Render();
	virtual DWORD ActionEvent( CMouse * mouseInfo );
	void OnActionEvent( LONG lId, void* p, DWORD we );
	virtual BOOL FakeMoveIcon( LONG x, LONG y, cIcon * icon );
	void FakeDeleteIcon( CMenuIcon * pMenuIcon );

	BOOL AddMenuIcon( WORD wPage, WORD wPos, CMenuIcon* pMenuIcon );
	BOOL DeleteMenuIcon( CMenuIcon* pMenuIcon );
	BOOL MoveMenuIcon( WORD wPage, WORD wPos, CMenuIcon* pMenuIcon );
	BOOL ExchangeMenuIcon( CMenuIcon* pMenuIcon1, CMenuIcon* pMenuIcon2 );
};

#endif // !defined(AFX_MENUSLOTDIALOG_H__FB968505_C5A7_4191_A367_72AF66EBC061__INCLUDED_)
