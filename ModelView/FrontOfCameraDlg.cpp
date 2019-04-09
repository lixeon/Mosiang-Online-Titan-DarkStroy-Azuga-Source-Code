// FrontOfCameraDlg.cpp : implementation file
//

#include "stdafx.h"
#include "modelview.h"
#include "FrontOfCameraDlg.h"
#include "ToolInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFrontOfCameraDlg dialog


CFrontOfCameraDlg::CFrontOfCameraDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFrontOfCameraDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFrontOfCameraDlg)
	m_byAlpha = 0;
	m_fDistance = 0.0f;
	//}}AFX_DATA_INIT
}


void CFrontOfCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFrontOfCameraDlg)
	DDX_Text(pDX, IDC_FOC_ALPHA_EDIT, m_byAlpha);
	DDX_Text(pDX, IDC_FOC_DISTANCE_EDIT, m_fDistance);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFrontOfCameraDlg, CDialog)
	//{{AFX_MSG_MAP(CFrontOfCameraDlg)
	ON_BN_CLICKED(IDC_FOC_ALPHA_BUTTON, OnFocAlphaButton)
	ON_BN_CLICKED(IDC_FOC_DISTANCE_BUTTON, OnFocDistanceButton)
	ON_BN_CLICKED(IDC_FOC_REMOVE_BUTTON, OnFocRemoveButton)
	ON_BN_CLICKED(IDC_FOC_BROWSE_BUTTON, OnFocBrowseButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrontOfCameraDlg message handlers

void CFrontOfCameraDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CDialog::PostNcDestroy();	
	AfxGetMainWnd()->SendMessage(WM_USER_FRONT_OF_CAMERA_DESTROY, 0, 0);
	delete this;
}


void CFrontOfCameraDlg::OnOK()
{
}

void CFrontOfCameraDlg::OnCancel()
{
	DestroyWindow();
}

void CFrontOfCameraDlg::OnFocAlphaButton() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);

	g_ToolInfo.SetFrontOfCameraAlpha(m_byAlpha);	
}

void CFrontOfCameraDlg::OnFocDistanceButton() 
{
	UpdateData(TRUE);

	g_ToolInfo.SetFrontOfCameraDistance(m_fDistance);
}


void CFrontOfCameraDlg::OnFocRemoveButton() 
{
	g_ToolInfo.RemoveFrontOfCameraObject();
	UpdateDialog();
}

void CFrontOfCameraDlg::SetGXObjectFileName(char* pszFileName)
{
	if (NULL == pszFileName)
	{
		return;
	}

	char szPureName[1024];
	memset(szPureName, 0, 1024);

	GetPureFileName(szPureName, pszFileName, 1024);

	
	CStatic* pwndStatic = (CStatic*) GetDlgItem(IDC_FOC_CURRENT_OBJECT_STATIC);
	pwndStatic->SetWindowText(szPureName);
}

void CFrontOfCameraDlg::OnFocBrowseButton() 
{
	// File dialog.

	char szFilter[] = "Chr Files(*.chr)|*.chr|Chx Files(*.chx)|*.chx|Mod Files(*.mod)|*.mod|All Files(*.*)|*.*||"; 
    CFileDialog fileDlg(TRUE,
					    NULL,
						NULL,
						OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES,
						szFilter);	

	if (IDOK == fileDlg.DoModal())
	{
		char szFileName[1024];
		memset(szFileName, 0, 1024);

		CString strFileName = fileDlg.GetPathName();		
		
		memcpy(szFileName, (LPSTR)(LPCTSTR)strFileName, strFileName.GetLength());
		g_ToolInfo.CreateFrontOfCameraObject(szFileName);
		
		SetGXObjectFileName(szFileName);
	}

	UpdateDialog();
}


void CFrontOfCameraDlg::UpdateDialog(void)
{
	m_byAlpha	= g_ToolInfo.m_byFrontOfCameraAlpah;
	m_fDistance = g_ToolInfo.m_fFrontOfCameraDistance;	
	SetGXObjectFileName(g_ToolInfo.m_szFrontOfCameraName);

	UpdateData(FALSE);	
}
