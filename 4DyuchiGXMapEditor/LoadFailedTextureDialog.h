#if !defined(AFX_LOADFAILEDTEXTUREDIALOG_H__13C238F5_675D_43A9_A83E_CEBBDB162A9A__INCLUDED_)
#define AFX_LOADFAILEDTEXTUREDIALOG_H__13C238F5_675D_43A9_A83E_CEBBDB162A9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoadFailedTextureDialog.h : header file
//

#include "../4DyuchiGRX_Common/IRenderer.h"

/////////////////////////////////////////////////////////////////////////////
// CLoadFailedTextureDialog dialog

class CLoadFailedTextureDialog : public CDialog
{
// Construction
public:
	CLoadFailedTextureDialog(CWnd* pParent = NULL);   // standard constructor
	
public:

	void UpdateListBoxData(TEXTURE_TABLE* pTextureTable, DWORD dwCount);

	TEXTURE_TABLE*		m_pLoadFailedTextureTable;
	DWORD				m_dwLoadFailedTextureTableCount;

// Dialog Data
	//{{AFX_DATA(CLoadFailedTextureDialog)
	enum { IDD = IDD_LOADFAILEDTEXTURE_DIALOG_ };	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoadFailedTextureDialog)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLoadFailedTextureDialog)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSavelistButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOADFAILEDTEXTUREDIALOG_H__13C238F5_675D_43A9_A83E_CEBBDB162A9A__INCLUDED_)
