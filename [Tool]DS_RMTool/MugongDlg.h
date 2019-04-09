#if !defined(AFX_MUGONGDLG_H__AFB42EE6_BB37_4E65_AB77_D8789595677E__INCLUDED_)
#define AFX_MUGONGDLG_H__AFB42EE6_BB37_4E65_AB77_D8789595677E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MugongDlg.h : header file
//

#include "PropertyList.h"


enum emMugong_Basic
{
	emMugong_DBidx, emMugong_Idx, emMugong_Pos, emMugong_Exp, emMugong_Sung, 
	emMugong_bWear, emMugong_QuickPos, emMugong_Max,
};

/////////////////////////////////////////////////////////////////////////////
// CMugongDlg dialog

class CMugongDlg : public CDialog
{
protected:
	MUGONGBASE			m_Mugong;
	WORD				m_wPos;
	INFOFIELD			m_Field;

	CPropertyItem* m_BasicList[emMugong_Max];

	int m_nCheckValue;
	WORD m_wLevel;

////////// ºÐ·ù
void SetMugongTypeControl();
void SetMugongTypeCombo(WORD Def = 0);
void EnableRadio(BOOL bEnable = TRUE);
//////////

////////// ½ÇÁ¦ ³»¿ë
void SetMugongCombo(DWORD type);
//////////
	
// Construction
public:
	CMugongDlg(CWnd* pParent = NULL);   // standard constructor

	void Init( INFOFIELD efield, MUGONGBASE* pMugong, WORD wPos, LEVELTYPE level );

	MUGONGBASE* GetMugong() { return &m_Mugong; }

// Dialog Data
	//{{AFX_DATA(CMugongDlg)
	enum { IDD = IDD_DLG_SKILL };
	CButton	m_btnDel;
	CButton	m_btnEdit;
	CComboBox	m_cbType;
	CButton	m_rdMugong;
	CPropertyList	m_listBasic;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMugongDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual void OnCancel();
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMugongDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioImugong();
	afx_msg void OnRadioWmugong();
	afx_msg void OnSelchangeComboMutype();
	afx_msg void OnButtonMuEdit();
	afx_msg void OnButtonMuDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MUGONGDLG_H__AFB42EE6_BB37_4E65_AB77_D8789595677E__INCLUDED_)
