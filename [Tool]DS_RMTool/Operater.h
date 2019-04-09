#if !defined(AFX_OPERATER_H__FD898F76_BB4E_4F42_A194_17583462964B__INCLUDED_)
#define AFX_OPERATER_H__FD898F76_BB4E_4F42_A194_17583462964B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Operater.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COperater dialog

class COperater : public CDialog
{
protected:
	TMSG_OPERATOR_LOGIN		m_sOper;

// Construction
public:
	COperater(CWnd* pParent = NULL);   // standard constructor

	void ResetData(int flag = 0);

// Dialog Data
	//{{AFX_DATA(COperater)
	enum { IDD = IDD_DLG_LOGIN };
	CComboBox	m_cbServerSet;
	CEdit	m_etOperPwd;
	CEdit	m_etOperID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COperater)
	public:
	virtual int DoModal();
	protected:
	virtual void OnOK();
	virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COperater)
	afx_msg void OnBtOk();
	afx_msg void OnBtCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPERATER_H__FD898F76_BB4E_4F42_A194_17583462964B__INCLUDED_)
