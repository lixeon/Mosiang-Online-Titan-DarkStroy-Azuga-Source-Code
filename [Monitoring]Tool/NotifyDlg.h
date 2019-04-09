#if !defined(AFX_NOTIFYDLG_H__D071CD60_86B4_4423_99A6_4111124670A0__INCLUDED_)
#define AFX_NOTIFYDLG_H__D071CD60_86B4_4423_99A6_4111124670A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NotifyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNotifyDlg dialog

class CNotifyMessage;

class CNotifyDlg : public CDialog
{
// Construction
public:
	void SaveFile();
	void OpenFile();
	CNotifyMessage* m_pNotifyMessage;
	void Init( CString filename, CNotifyMessage* pNotifyMessage );
	CNotifyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNotifyDlg)
	enum { IDD = IDD_DIALOG_NOTIFY };
	CEdit	m_NotifyEdit;
	CString	m_strCurFileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNotifyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNotifyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonOpenNotify();
	afx_msg void OnButtonSaveasNotify();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOTIFYDLG_H__D071CD60_86B4_4423_99A6_4111124670A0__INCLUDED_)
