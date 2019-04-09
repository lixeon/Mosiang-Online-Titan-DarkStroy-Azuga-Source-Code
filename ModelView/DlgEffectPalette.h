#if !defined(AFX_DLGEFFECTPALETTE_H__70D171D1_60CA_4317_884E_9D1DBE5767D8__INCLUDED_)
#define AFX_DLGEFFECTPALETTE_H__70D171D1_60CA_4317_884E_9D1DBE5767D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgEffectPalette.h : header file
//
#include "resource.h"
#include "ListBoxContainer.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgEffectPalette dialog

#define MAX_EFFECT_NUM	1024
class CDlgEffectPalette : public CDialog
{
	DWORD			m_iSelectedEffectIndex;
// Construction
public:
	CDlgEffectPalette(CWnd* pParent = NULL);   // standard constructor
	void		EnableView();
	void		DisableView();
	void		UpdateEffectPalette();
	DWORD		GetCurrentEffectIndex() {return (DWORD)m_iSelectedEffectIndex;}
// Dialog Data
	//{{AFX_DATA(CDlgEffectPalette)
	enum { IDD = IDD_DIALOG_EFFECTPALETTE };
	CListBox	m_lbEffectList;
	CListBoxContainer	m_lbcEffectList;
	BOOL	m_bEnableTexWave;
	BOOL	m_bDisableSrcTex;
	CString	m_strTexName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgEffectPalette)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgEffectPalette)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonEffectCreate();
	afx_msg void OnButtonEffectDelete();
	afx_msg void OnSelchangeListEffect();
	afx_msg void OnButtonUpdateEffectPalette();
	afx_msg void OnButtonEffectApply();
	afx_msg void OnDestroy();
	afx_msg void OnButtonBrowseTexfile();
	afx_msg void OnButtonEffectSave();
	afx_msg void OnButtonEffectLoad();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CDlgEffectPalette*	g_pDlgEffectPalette;
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_DLGEFFECTPALETTE_H__70D171D1_60CA_4317_884E_9D1DBE5767D8__INCLUDED_)


