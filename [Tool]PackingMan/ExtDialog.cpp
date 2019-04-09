// ExtDialog.cpp : implementation file
//

#include "stdafx.h"
#include "PackingTool.h"
#include "ExtDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtDialog dialog


CExtDialog::CExtDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CExtDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExtDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CExtDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExtDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExtDialog, CDialog)
	//{{AFX_MSG_MAP(CExtDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtDialog message handlers
