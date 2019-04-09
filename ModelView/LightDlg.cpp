// LightDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ModelView.h"
#include "MainFrm.h"
#include "modelview.h"
#include "LightDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLightDlg dialog


CLightDlg::CLightDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLightDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLightDlg)
	m_bLightOn = FALSE;
	//}}AFX_DATA_INIT
}


void CLightDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLightDlg)
	DDX_Check(pDX, IDC_LIGHT_CHECK, m_bLightOn);
	//}}AFX_DATA_MAP

	DDX_Text(pDX, IDC_EDT_AMB_R, m_byAmbientR);
	DDV_MinMaxInt(pDX, m_byAmbientR, 0, 255);

	DDX_Text(pDX, IDC_EDT_AMB_G, m_byAmbientG);
	DDV_MinMaxInt(pDX, m_byAmbientG, 0, 255);

	DDX_Text(pDX, IDC_EDT_AMB_B, m_byAmbientB);
	DDV_MinMaxInt(pDX, m_byAmbientB, 0, 255);



	DDX_Text(pDX, IDC_EDT_DIF_R, m_byDiffuseR);
	DDV_MinMaxInt(pDX, m_byDiffuseR, 0, 255);

	DDX_Text(pDX, IDC_EDT_DIF_G, m_byDiffuseG);
	DDV_MinMaxInt(pDX, m_byDiffuseG, 0, 255);

	DDX_Text(pDX, IDC_EDT_DIF_B, m_byDiffuseB);
	DDV_MinMaxInt(pDX, m_byDiffuseB, 0, 255);


	DDX_Text(pDX, IDC_DIRECTION_X, m_fDirectionX);
	DDX_Text(pDX, IDC_DIRECTION_Y, m_fDirectionY);
	DDX_Text(pDX, IDC_DIRECTION_Z, m_fDirectionZ);
}

BEGIN_MESSAGE_MAP(CLightDlg, CDialog)
	//{{AFX_MSG_MAP(CLightDlg)
	ON_BN_CLICKED(IDC_LIGHT_OK_BUTTON, OnLightOkButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLightDlg message handlers

void CLightDlg::OnLightOkButton() 
{	
	UpdateData(TRUE);

	g_ToolInfo.m_abyAmbient[0] = m_byAmbientR;
	g_ToolInfo.m_abyAmbient[1] = m_byAmbientG;
	g_ToolInfo.m_abyAmbient[2] = m_byAmbientB;

	g_ToolInfo.m_abyDiffuse[0] = m_byDiffuseR;
	g_ToolInfo.m_abyDiffuse[1] = m_byDiffuseG;
	g_ToolInfo.m_abyDiffuse[2] = m_byDiffuseB;

	g_ToolInfo.m_afDirection[0] = m_fDirectionX;
	g_ToolInfo.m_afDirection[1] = m_fDirectionY;
	g_ToolInfo.m_afDirection[2] = m_fDirectionZ;

	g_ToolInfo.m_bOnLight = m_bLightOn;

	UpdateLightInformation(0);
}


void CLightDlg::OnOK()
{
	UpdateData(TRUE);
}

void CLightDlg::OnCancel()
{
	DestroyWindow();
}

void CLightDlg::PostNcDestroy() 
{
	CDialog::PostNcDestroy();		
	AfxGetMainWnd()->SendMessage(WM_USER_LIGHT_DIALOG_DESTROY, 0, 0);
	delete this;
}
