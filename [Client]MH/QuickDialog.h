// QuickDialog.h: interface for the CQuickDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUICKDIALOG_H__7FC82468_F71E_44FA_B0A2_3543D9F89106__INCLUDED_)
#define AFX_QUICKDIALOG_H__7FC82468_F71E_44FA_B0A2_3543D9F89106__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./interface/cDialog.h"
#include "AbilityIcon.h"
#include "AbilityInfo.h"

class cIcon;
class cIconGridDialog;
class CQuickItem;
class CMugongBase;
class CItem;


enum KEYSTROKE
{
	KEY_QK1	=	1,
	KEY_QK2	=	2,
	KEY_QK3	=	4,
	KEY_QK4	=	8,
	KEY_QK5	=	16,
	KEY_QK6	=	32,
	KEY_QK7	=	64,
};


class CQuickDialog : public cDialog  
{
	friend class CQuickManager;
public:
	CQuickDialog();
	virtual ~CQuickDialog();

	virtual DWORD ActionEvent(CMouse * mouseInfo);

	void OnSelectAction( POSTYPE pos );

	void OnActionEvent(LONG lId, void * p, DWORD we);
	virtual void Render();
	virtual void Add(cWindow * window);
	virtual BOOL FakeMoveIcon(LONG mouseX, LONG mouseY, cIcon * icon);
	
	void ViewSheet();
	void SelectPageDirect( WORD wPage );

	WORD GetSelectedPageNumber() { return m_selectedPage; }
	void SetSelectedPageNumber(WORD num) { m_selectedPage = num; }
	void FakeDeleteQuickItem( CQuickItem * pQuickItem );
	CQuickItem * GetQuickItem(POSTYPE abs_pos);

	BOOL RemQuickItem(POSTYPE abs_pos, cIcon ** delItem);

	void SetToolTipIcon( CQuickItem * item );
	void RefreshIcon();
	void RefreshIcon( POSTYPE pos );
	//BOOL IsAddable(POSTYPE absPos);
	void FakeAddItem(POSTYPE abs_pos, CItem * pItem, CQuickItem * pOldQuick=NULL);
private:	

	//---------------------------------------------------------------------------
	// custom member
	// private를 protected, public로 바꾸지 마라 : taiyo
	//---------------------------------------------------------------------------
	void FakeMoveQuickItem(LONG x, LONG y, CQuickItem * icon);
	void FakeAddItem(LONG x, LONG y, CItem * icon);
	void FakeAddMugong(LONG x, LONG y, CMugongBase * pMugong);
	void FakeAddAbility(LONG x, LONG y, CAbilityIcon * pAbility);

	BOOL AddQuickItem(POSTYPE abs_pos, CQuickItem * pIcon);
	//BOOL AddQuickItem(POSTYPE abs_pos, ICONBASE * pIconInfo);

	BOOL MoveQuickItem(POSTYPE fromAbsPos, POSTYPE toAbsPos, bool DeleteToQuickPos);
	BOOL ValidPos(POSTYPE abs_pos);

	int  GetSheetNumber(POSTYPE abs_pos);
	// 06. 01 이영준 - 단축창 변경
	WORD GetPositionNumber(POSTYPE abs_pos);

	BOOL GetAbsPositionForXYRef(LONG x, LONG y, POSTYPE & abs_pos);
	WORD GetSelectedPage() { return m_selectedPage; }
//	cIconGridDialog * m_pIconGridSheet[MAX_QUICKSHEET_NUM];
	cIconGridDialog * m_pIconGridSheet[TAB_QUICK_NUM];
	WORD m_selectedPage;

	cImage	m_PressedImage;
	int		m_keyStrokeBit;
//	DWORD	m_keyStrokeDTick[MAX_QUICKITEMPERSHEET_NUM];
	DWORD	m_keyStrokeDTick[TABCELL_QUICK_NUM];
};

#endif // !defined(AFX_QUICKDIALOG_H__7FC82468_F71E_44FA_B0A2_3543D9F89106__INCLUDED_)
