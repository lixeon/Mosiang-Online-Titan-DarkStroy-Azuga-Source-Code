// ExtDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PackingTool.h"
#include "ExtDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtDlg dialog


CExtDlg::CExtDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExtDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExtDlg)
	m_sExt = _T("bin");
	//}}AFX_DATA_INIT
}


void CExtDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExtDlg)
	DDX_Text(pDX, IDC_EDIT_EXT, m_sExt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExtDlg, CDialog)
	//{{AFX_MSG_MAP(CExtDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtDlg message handlers

