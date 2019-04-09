#if !defined(AFX_ITEMRECOVERDLG_H__D3C7AAD4_99F8_4FFC_8DCB_2B46E0566EDA__INCLUDED_)
#define AFX_ITEMRECOVERDLG_H__D3C7AAD4_99F8_4FFC_8DCB_2B46E0566EDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ItemRecoverDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CItemRecoverDlg dialog

class CItemRecoverDlg : public CDialog
{
// Construction
public:
	CItemRecoverDlg(CWnd* pParent = NULL);   // standard constructor

	int		m_nItemIdx;
	int		m_nItemNum;
	
	void	InitComboBox();

// Dialog Data
	//{{AFX_DATA(CItemRecoverDlg)
	enum { IDD = IDD_DLG_ITEM_RECOVER };
	CEdit	m_etItemNum;
	CComboBox	m_cbItem;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemRecoverDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CItemRecoverDlg)
	afx_msg void OnButtonItemRecover();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMRECOVERDLG_H__D3C7AAD4_99F8_4FFC_8DCB_2B46E0566EDA__INCLUDED_)
