// SpecularDlg.cpp : implementation file
//

#include "stdafx.h"
#include "modelview.h"
#include "SpecularDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpecularDlg dialog


CSpecularDlg::CSpecularDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpecularDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpecularDlg)
	m_bSpecularEnable = FALSE;
	m_fSpecularIntensity = 0.0f;
	//}}AFX_DATA_INIT
}


void CSpecularDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpecularDlg)
	DDX_Check(pDX, IDC_SPECULAR_ENABLE, m_bSpecularEnable);
	DDX_Text(pDX, IDC_SPEC_INTENSITY, m_fSpecularIntensity);
	DDV_MinMaxFloat(pDX, m_fSpecularIntensity, 0.0f, 255.f);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSpecularDlg, CDialog)
	//{{AFX_MSG_MAP(CSpecularDlg)
	ON_BN_CLICKED(IDC_SPEC_SET_OK, OnSpecSetOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpecularDlg message handlers

void CSpecularDlg::OnSpecSetOk() 
{
	OnOK();
}
