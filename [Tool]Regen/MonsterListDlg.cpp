// MonsterListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RegenTool.h"
#include "MonsterListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMonsterListDlg dialog


CMonsterListDlg::CMonsterListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMonsterListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMonsterListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMonsterListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonsterListDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMonsterListDlg, CDialog)
	//{{AFX_MSG_MAP(CMonsterListDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonsterListDlg message handlers
