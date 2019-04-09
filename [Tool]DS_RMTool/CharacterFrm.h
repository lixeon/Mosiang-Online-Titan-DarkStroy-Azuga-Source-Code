#if !defined(AFX_CHARACTERFRM_H__BC29B758_77B4_4F3F_946B_1027711C60A2__INCLUDED_)
#define AFX_CHARACTERFRM_H__BC29B758_77B4_4F3F_946B_1027711C60A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CharacterFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCharacterFrm frame

class CCharacterFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CCharacterFrm)
protected:
	CCharacterFrm();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCharacterFrm)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCharacterFrm();

	// Generated message map functions
	//{{AFX_MSG(CCharacterFrm)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARACTERFRM_H__BC29B758_77B4_4F3F_946B_1027711C60A2__INCLUDED_)
