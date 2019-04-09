// ViewerOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ds_rmtool.h"
#include "ViewerOptionDlg.h"
#include "LogItemMoneyPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewerOptionDlg dialog


CViewerOptionDlg::CViewerOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CViewerOptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewerOptionDlg)
	//}}AFX_DATA_INIT
}


void CViewerOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewerOptionDlg)
	//}}AFX_DATA_MAP
	for( int i = 0; i < 14; ++i )
		DDX_Check(pDX, IDC_CHECK_1+i, m_bVOption[i]);
}


BEGIN_MESSAGE_MAP(CViewerOptionDlg, CDialog)
	//{{AFX_MSG_MAP(CViewerOptionDlg)
	ON_BN_CLICKED(IDC_CHECK_1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK_2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK_3, OnCheck3)
	ON_BN_CLICKED(IDC_CHECK_4, OnCheck4)
	ON_BN_CLICKED(IDC_CHECK_5, OnCheck5)
	ON_BN_CLICKED(IDC_CHECK_6, OnCheck6)
	ON_BN_CLICKED(IDC_CHECK_7, OnCheck7)
	ON_BN_CLICKED(IDC_CHECK_8, OnCheck8)
	ON_BN_CLICKED(IDC_CHECK_9, OnCheck9)
	ON_BN_CLICKED(IDC_CHECK_10, OnCheck10)
	ON_BN_CLICKED(IDC_CHECK_11, OnCheck11)
	ON_BN_CLICKED(IDC_CHECK_12, OnCheck12)
	ON_BN_CLICKED(IDC_CHECK_13, OnCheck13)
	ON_BN_CLICKED(IDC_CHECK_14, OnCheck14)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewerOptionDlg message handlers

BOOL CViewerOptionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	for( int i = 0; i < 14; ++i )
		m_bVOption[i] = m_pPage->m_bVOption[i];

	UpdateData( FALSE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CViewerOptionDlg::OnCheck1() 
{
	m_bVOption[0] = !m_bVOption[0];
	m_pPage->m_bVOption[0] = m_bVOption[0];	
}

void CViewerOptionDlg::OnCheck2() 
{
	m_bVOption[1] = !m_bVOption[1];
	m_pPage->m_bVOption[1] = m_bVOption[1];		
}

void CViewerOptionDlg::OnCheck3() 
{
	m_bVOption[2] = !m_bVOption[2];
	m_pPage->m_bVOption[2] = m_bVOption[2];		
}

void CViewerOptionDlg::OnCheck4() 
{
	m_bVOption[3] = !m_bVOption[3];
	m_pPage->m_bVOption[3] = m_bVOption[3];		
}

void CViewerOptionDlg::OnCheck5() 
{
	m_bVOption[4] = !m_bVOption[4];
	m_pPage->m_bVOption[4] = m_bVOption[4];		
}

void CViewerOptionDlg::OnCheck6() 
{
	m_bVOption[5] = !m_bVOption[5];
	m_pPage->m_bVOption[5] = m_bVOption[5];		
}

void CViewerOptionDlg::OnCheck7() 
{
	m_bVOption[6] = !m_bVOption[6];
	m_pPage->m_bVOption[6] = m_bVOption[6];		
}

void CViewerOptionDlg::OnCheck8() 
{
	m_bVOption[7] = !m_bVOption[7];
	m_pPage->m_bVOption[7] = m_bVOption[7];	
}

void CViewerOptionDlg::OnCheck9() 
{
	m_bVOption[8] = !m_bVOption[8];
	m_pPage->m_bVOption[8] = m_bVOption[8];		
}

void CViewerOptionDlg::OnCheck10() 
{
	m_bVOption[9] = !m_bVOption[9];
	m_pPage->m_bVOption[9] = m_bVOption[9];		
}

void CViewerOptionDlg::OnCheck11() 
{
	m_bVOption[10] = !m_bVOption[10];
	m_pPage->m_bVOption[10] = m_bVOption[10];		
}

void CViewerOptionDlg::OnCheck12() 
{
	m_bVOption[11] = !m_bVOption[11];
	m_pPage->m_bVOption[11] = m_bVOption[11];		
}

void CViewerOptionDlg::OnCheck13() 
{
	m_bVOption[12] = !m_bVOption[12];
	m_pPage->m_bVOption[12] = m_bVOption[12];
}

void CViewerOptionDlg::OnCheck14() 
{
	m_bVOption[13] = !m_bVOption[13];
	m_pPage->m_bVOption[13] = m_bVOption[13];
}
