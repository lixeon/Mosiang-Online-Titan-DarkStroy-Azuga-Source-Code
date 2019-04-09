// IllusionSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "modelview.h"
#include "IllusionSetupDlg.h"

#include "ToolInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIllusionSetupDlg dialog


CIllusionSetupDlg::CIllusionSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIllusionSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIllusionSetupDlg)
	//}}AFX_DATA_INIT

}


void CIllusionSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIllusionSetupDlg)
	DDX_Control(pDX, IDC_ILLUSION_SETUP_DLG_LENGTH_EDIT, m_IllusionLengthEdit);
	DDX_Control(pDX, IDC_ILLUSION_SETUP_SHOW_CHECK, m_ShowCheckBox);
	//}}AFX_DATA_MAP
}

BOOL CIllusionSetupDlg::OnInitDialog() 
{
   CDialog::OnInitDialog();
   
   // TODO: Add extra initialization here
	m_ShowCheckBox.SetCheck( 1);
	m_IllusionLengthEdit.SetWindowText( "5");

   return TRUE;   // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}



BEGIN_MESSAGE_MAP(CIllusionSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CIllusionSetupDlg)
	ON_BN_CLICKED(IDC_ILLUSION_SETUP_SHOW_CHECK, OnIllusionSetupShowCheck)
	ON_EN_CHANGE(IDC_ILLUSION_SETUP_DLG_LENGTH_EDIT, OnChangeIllusionSetupDlgLengthEdit)
	ON_EN_UPDATE(IDC_ILLUSION_SETUP_DLG_LENGTH_EDIT, OnUpdateIllusionSetupDlgLengthEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIllusionSetupDlg message handlers

void CIllusionSetupDlg::OnIllusionSetupShowCheck() 
{
	// TODO: Add your control notification handler code here
	UINT	uiState	=	m_ShowCheckBox.GetState();

	uiState	=	uiState & 0x00000003;		// 마스크 씌우고,
	if( uiState == 0)		
	{
		// uncheck
		m_bShow		=	FALSE;
	}
	else
	{
		m_bShow		=	TRUE;
	}

	g_ToolInfo.ApplyIllusionMeshStatus( g_ToolInfo.m_Illusion.dwIllusionLength, g_ToolInfo.m_Illusion.phMaterial, m_bShow);

	_asm nop
}

void CIllusionSetupDlg::OnChangeIllusionSetupDlgLengthEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	char	szBuffer[128];
	m_IllusionLengthEdit.GetWindowText( szBuffer, 128);
	
	int	iIllusionLength	=	atoi( szBuffer);
	if( iIllusionLength < 2)
	{
		iIllusionLength = 2;
		itoa(iIllusionLength, szBuffer, 10);
		m_IllusionLengthEdit.SetWindowText( szBuffer);
		return	;
	}
	else if( iIllusionLength > 256)
	{
		iIllusionLength = 255;
		itoa(iIllusionLength, szBuffer, 10);
		m_IllusionLengthEdit.SetWindowText( szBuffer);
		return	;
	}


	g_ToolInfo.ApplyIllusionMeshStatus( iIllusionLength, g_ToolInfo.m_Illusion.phMaterial, m_bShow);

	// TODO: Add your control notification handler code here
	
}

void CIllusionSetupDlg::OnUpdateIllusionSetupDlgLengthEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_UPDATE flag ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here

	_asm nop
}
