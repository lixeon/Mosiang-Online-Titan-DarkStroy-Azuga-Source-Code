#if !defined(AFX_CHARACTERVIEW_H__A7FD2962_2D30_4AA8_8625_DD34FEFC3075__INCLUDED_)
#define AFX_CHARACTERVIEW_H__A7FD2962_2D30_4AA8_8625_DD34FEFC3075__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CharacterView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCharacterView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CLDPropPage;
class CLDPropSheet;
class CCharacterInfo;

enum eCharPage
{
	eCharPage_CharacterInfo = 0, eCharPage_Max, 
};

class CCharacterView : public CFormView
{
protected:
	CCharacterView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCharacterView)

// Form Data
public:
	//{{AFX_DATA(CCharacterView)
	enum { IDD = IDD_CHAR_SHEET };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	// Sheet and Page
	CLDPropSheet*		m_pSheet;
	CLDPropPage*		m_pPageList[eCharPage_Max];
	//

// Operations
public:
	CCharacterInfo*		GetCharacterInfoPage() { return (CCharacterInfo*)m_pPageList[eCharPage_CharacterInfo]; }
	
	void	UpdateMainQuestData( TMSG_MAINQUESTINFO* pMsg );
	void	UpdateSubQuestData( TMSG_SUBQUESTINFO* pMsg );
	void	ChangeLoginPoint( DWORD dwMapNum );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCharacterView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCharacterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CCharacterView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARACTERVIEW_H__A7FD2962_2D30_4AA8_8625_DD34FEFC3075__INCLUDED_)
