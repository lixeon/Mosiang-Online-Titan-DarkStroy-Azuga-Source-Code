// ConAgentDlg.cpp : implementation file
//

#include "stdafx.h"
#include "servertool.h"
#include "ConAgentDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConAgentDlg dialog


CConAgentDlg::CConAgentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConAgentDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConAgentDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CConAgentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConAgentDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConAgentDlg, CDialog)
	//{{AFX_MSG_MAP(CConAgentDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConAgentDlg message handlers
