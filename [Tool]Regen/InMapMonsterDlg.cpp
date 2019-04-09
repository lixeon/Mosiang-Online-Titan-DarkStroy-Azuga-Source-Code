// InMapMonsterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RegenTool.h"
#include "InMapMonsterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInMapMonsterDlg dialog


CInMapMonsterDlg::CInMapMonsterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInMapMonsterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInMapMonsterDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CInMapMonsterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInMapMonsterDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInMapMonsterDlg, CDialog)
	//{{AFX_MSG_MAP(CInMapMonsterDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInMapMonsterDlg message handlers
