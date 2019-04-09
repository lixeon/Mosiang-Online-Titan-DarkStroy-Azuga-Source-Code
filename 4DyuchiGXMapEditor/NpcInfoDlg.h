#if !defined(AFX_NPCINFODLG_H__C107E275_B2A0_4E8A_BD69_EDC4D782BBC8__INCLUDED_)
#define AFX_NPCINFODLG_H__C107E275_B2A0_4E8A_BD69_EDC4D782BBC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NpcInfoDlg.h : header file
//
#include "MyListCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CNpcInfoDlg dialog

#define MAX_NPC_INFO	1000

#pragma pack( push, 1 )

struct Npc_ID
{
	WORD	wNpcID;
	char	szNpcName[50];
};
#pragma pack( pop )

class CNpcInfoDlg : public CDialog
{
// Construction
public:
	CNpcInfoDlg(CWnd* pParent = NULL);   // standard constructor
	
	WORD	m_wNpcInfoMany;
	WORD	m_IDAttr[MAX_NPC_INFO];		 // combo sel에 따라 기억되어 있는 몬스터 아이디.
	Npc_ID	m_NpcID[ MAX_NPC_INFO ];
	
// Dialog Data
	//{{AFX_DATA(CNpcInfoDlg)
	enum { IDD = IDD_NPC_INFO };
	CComboBox	m_comboNpc;
	CMyListCtrl	m_ListNpcInfo;
	CString	m_strPosZ;
	CString	m_strPosX;
	CString	m_strSectionNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNpcInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNpcInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonCreateNpc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NPCINFODLG_H__C107E275_B2A0_4E8A_BD69_EDC4D782BBC8__INCLUDED_)
