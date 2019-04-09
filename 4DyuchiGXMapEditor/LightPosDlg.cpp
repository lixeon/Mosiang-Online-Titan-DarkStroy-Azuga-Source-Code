// LightPosDlg.cpp : implementation file
//

#include "stdafx.h"
#include "4dyuchigxmapeditor.h"
#include "LightPosDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLightPosDlg dialog


CLightPosDlg::CLightPosDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLightPosDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLightPosDlg)
	m_fPosX = 0.0f;
	m_fPosY = 0.0f;
	m_fPosZ = 0.0f;
	//}}AFX_DATA_INIT
}


void CLightPosDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLightPosDlg)
	DDX_Text(pDX, IDC_EDIT_LIGHT_POS_X, m_fPosX);
	DDX_Text(pDX, IDC_EDIT_LIGHT_POS_Y, m_fPosY);
	DDX_Text(pDX, IDC_EDIT_LIGHT_POS_Z, m_fPosZ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLightPosDlg, CDialog)
	//{{AFX_MSG_MAP(CLightPosDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLightPosDlg message handlers

void CLightPosDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	CDialog::OnOK();
}
