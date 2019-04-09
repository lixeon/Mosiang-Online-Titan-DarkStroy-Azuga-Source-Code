// SectionInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "4dyuchigxmapeditor.h"
#include "SectionInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSectionInfoDlg dialog


CSectionInfoDlg::CSectionInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSectionInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSectionInfoDlg)
	m_strTop = _T("");
	m_strRight = _T("");
	m_strSectionNum = _T("");
	m_strLeft = _T("");
	m_strBottom = _T("");
	//}}AFX_DATA_INIT
}


void CSectionInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSectionInfoDlg)
	DDX_Text(pDX, IDC_STATIC_SECTION_TOP, m_strTop);
	DDX_Text(pDX, IDC_STATIC_SECTION_RIGHT, m_strRight);
	DDX_Text(pDX, IDC_STATIC_SECTION_NUM, m_strSectionNum);
	DDX_Text(pDX, IDC_STATIC_SECTION_LEFT, m_strLeft);
	DDX_Text(pDX, IDC_STATIC_SECTION_BOTTOM, m_strBottom);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSectionInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CSectionInfoDlg)
	ON_BN_CLICKED(ID_SECTION_DLG_OK, OnSectionDlgOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSectionInfoDlg message handlers

void CSectionInfoDlg::OnSectionDlgOk() 
{
	CDialog::OnOK();
}
