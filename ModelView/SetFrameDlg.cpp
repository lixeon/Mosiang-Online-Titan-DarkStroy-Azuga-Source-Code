//
//	Set Frame per Second of Animation Dialog.
//
//					2003/01/21
//					Trust Pak


#include "StdAfx.h"
#include "SetFrameDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CSetFrameDlg dialog


CSetFrameDlg::CSetFrameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetFrameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetFrameDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}


void CSetFrameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetFrameDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	DDX_Text(pDX, IDC_EDIT_FPS, m_iFps);
	DDV_MinMaxInt(pDX, m_iFps, 0, 1000);
}


BEGIN_MESSAGE_MAP(CSetFrameDlg, CDialog)
	//{{AFX_MSG_MAP(CSetFrameDlg)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_FPS_OK_BUTTON, OnFpsOkButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetFrameDlg message handlers

int CSetFrameDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;	
	
	return 0;
}

void CSetFrameDlg::OnFpsOkButton() 
{
	OnOK();	
}
