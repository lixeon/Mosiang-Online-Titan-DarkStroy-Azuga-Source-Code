// MsgBoxOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "servertool.h"
#include "MsgBoxOptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgBoxOptionDlg dialog

extern sMSGOPTION	gMsgOption;

CMsgBoxOptionDlg::CMsgBoxOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgBoxOptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMsgBoxOptionDlg)
	m_bServer = FALSE;
	m_bExeVer = FALSE;
	m_bMaxUser = FALSE;
	m_bUserLevel = FALSE;
	//}}AFX_DATA_INIT
}


void CMsgBoxOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsgBoxOptionDlg)
	DDX_Check(pDX, IDC_CHECK1, m_bServer);
	DDX_Check(pDX, IDC_CHECK2, m_bExeVer);
	DDX_Check(pDX, IDC_CHECK3, m_bMaxUser);
	DDX_Check(pDX, IDC_CHECK4, m_bUserLevel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsgBoxOptionDlg, CDialog)
	//{{AFX_MSG_MAP(CMsgBoxOptionDlg)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_CHECK4, OnCheck4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgBoxOptionDlg message handlers
/*void CMsgBoxOptionDlg::SetOption( sMSGOPTION* pData )
{
	m_pOption = pData;
	m_bServer = (BOOL)pData->bServer;
	m_bExeVer = (BOOL)pData->bExeVer;
	m_bMaxUser = (BOOL)pData->bMaxUser;
	m_bUserLevel = (BOOL)pData->bUserLevel;
}
*/
void CMsgBoxOptionDlg::OnCheck1() 
{	
	m_bServer = !m_bServer;
	gMsgOption.bServer = (BYTE)m_bServer;
}

void CMsgBoxOptionDlg::OnCheck2() 
{
	m_bExeVer = !m_bExeVer;
	gMsgOption.bExeVer = (BYTE)m_bExeVer;
}

void CMsgBoxOptionDlg::OnCheck3() 
{
	m_bMaxUser = !m_bMaxUser;
	gMsgOption.bMaxUser = (BYTE)m_bMaxUser;
}

void CMsgBoxOptionDlg::OnCheck4() 
{
	m_bUserLevel = !m_bUserLevel;
	gMsgOption.bUserLevel = (BYTE)m_bUserLevel;
}

BOOL CMsgBoxOptionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_bServer = (BOOL)gMsgOption.bServer;
	m_bExeVer = (BOOL)gMsgOption.bExeVer;
	m_bMaxUser = (BOOL)gMsgOption.bMaxUser;
	m_bUserLevel = (BOOL)gMsgOption.bUserLevel;

	UpdateData( FALSE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
