// BackgroundColorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "modelview.h"
#include "BackgroundColorDlg.h"
#include "ToolInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBackgroundColorDlg dialog


CBackgroundColorDlg::CBackgroundColorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBackgroundColorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBackgroundColorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBackgroundColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBackgroundColorDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBackgroundColorDlg, CDialog)
	//{{AFX_MSG_MAP(CBackgroundColorDlg)
	ON_BN_CLICKED(IDC_SET_BACKGROUNDCOLOR_DEFAULT, OnSetBackgroundcolorDefault)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBackgroundColorDlg message handlers

BOOL CBackgroundColorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
//	char	szColorComponent[1024];
//	itoa( g_ToolInfo.m_dwBackGroundColorR, szColorComponent, 10);

//	SetDlgItemText( IDC_BACKGROUNDCOLOR_R, 
	SetDlgItemInt( IDC_BACKGROUNDCOLOR_R, g_ToolInfo.m_dwBackGroundColorR);
	SetDlgItemInt( IDC_BACKGROUNDCOLOR_G, g_ToolInfo.m_dwBackGroundColorG);
	SetDlgItemInt( IDC_BACKGROUNDCOLOR_B, g_ToolInfo.m_dwBackGroundColorB);

//	GetDlgItemText( 


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBackgroundColorDlg::OnOK() 
{
	// TODO: Add extra validation here

	g_ToolInfo.m_dwBackGroundColorR	=	GetDlgItemInt( IDC_BACKGROUNDCOLOR_R);
	if( g_ToolInfo.m_dwBackGroundColorR > 255)
		g_ToolInfo.m_dwBackGroundColorR	=	255;
	g_ToolInfo.m_dwBackGroundColorG	=	GetDlgItemInt( IDC_BACKGROUNDCOLOR_G);
	if( g_ToolInfo.m_dwBackGroundColorG > 255)
		g_ToolInfo.m_dwBackGroundColorG	=	255;
	g_ToolInfo.m_dwBackGroundColorB	=	GetDlgItemInt( IDC_BACKGROUNDCOLOR_B);
	if( g_ToolInfo.m_dwBackGroundColorB > 255)
		g_ToolInfo.m_dwBackGroundColorB	=	255;
	
	CDialog::OnOK();
}

void CBackgroundColorDlg::OnSetBackgroundcolorDefault() 
{
	// TODO: Add your control notification handler code here
	g_ToolInfo.m_dwBackGroundColorR	=	DEFAULT_BACKGROUND_COLOR_R;
	g_ToolInfo.m_dwBackGroundColorG	=	DEFAULT_BACKGROUND_COLOR_G;
	g_ToolInfo.m_dwBackGroundColorB	=	DEFAULT_BACKGROUND_COLOR_B;	

	SetDlgItemInt( IDC_BACKGROUNDCOLOR_R, g_ToolInfo.m_dwBackGroundColorR);
	SetDlgItemInt( IDC_BACKGROUNDCOLOR_G, g_ToolInfo.m_dwBackGroundColorG);
	SetDlgItemInt( IDC_BACKGROUNDCOLOR_B, g_ToolInfo.m_dwBackGroundColorB);

}
