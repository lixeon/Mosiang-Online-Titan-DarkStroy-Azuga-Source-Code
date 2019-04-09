#if !defined(AFX_DIALOGCONSOLE_H__09FF87F3_8855_45D7_8CA4_3E60F12B9CD3__INCLUDED_)
#define AFX_DIALOGCONSOLE_H__09FF87F3_8855_45D7_8CA4_3E60F12B9CD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogConsole.h : header file
//
#include "../4DyuchiGRX_common/IRenderer.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogConsole dialog

class CDialogConsole : public CDialog
{
	I4DyuchiGXRenderer*	m_pRenderer;
	HWND				m_hWndParent;
	void				Process(char* txt,DWORD dwLen);
	void				ConvertStrToVector3(VECTOR3* pv3Out);
public:
	BOOL				Initialize(I4DyuchiGXRenderer* pRenderer,HWND hWndParent);
	void				OnEnter();
// Construction
public:
	CDialogConsole(CWnd* pParent = NULL);   // standard constructor
	~CDialogConsole();

// Dialog Data
	//{{AFX_DATA(CDialogConsole)
	enum { IDD = IDD_DIALOG_CONSOLE };
	CEdit	m_edit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogConsole)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogConsole)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGCONSOLE_H__09FF87F3_8855_45D7_8CA4_3E60F12B9CD3__INCLUDED_)
