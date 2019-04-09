// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ServerTool.h"
#include "LoginDlg.h"
#include "MHNetwork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog


CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nServerSet = 0;
	m_bConnect = FALSE;

	m_nReceiveCount = 0;
}


void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginDlg)
	DDX_Control(pDX, IDC_EDIT_PASS, m_editPass);
	DDX_Control(pDX, IDC_EDIT_ID, m_editID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CLoginDlg)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, OnButtonConnect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg message handlers
BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->hwnd == GetDlgItem(IDC_EDIT_ID)->m_hWnd || pMsg->hwnd == GetDlgItem(IDC_EDIT_PASS)->m_hWnd )
	{
		if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
		{
			EnterIDPass();
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CLoginDlg::EnterIDPass()
{
	// IP 얻어오기
	char szHostName[40];
	char szIp[16];
	::gethostname( szHostName, sizeof(szHostName) );
	HOSTENT FAR* lphostent = ::gethostbyname( szHostName );
	sprintf( szIp, "%s", inet_ntoa(*((struct in_addr*)lphostent->h_addr)) );

	CString id, pass;
	m_editID.GetWindowText( id );
	m_editPass.GetWindowText( pass );

	if( id.IsEmpty() || pass.IsEmpty() )
	{
		MessageBox( "ID & Password Check!!", "Error!" );
		return;
	}

	// test
	TMSG_OPERATOR_LOGIN	msg;
	msg.Category = MP_RMTOOL_CONNECT;
	msg.Protocol = MP_RMTOOL_CONNECT_SYN;
	strcpy( msg.sIP.strIP, szIp );
	strcpy( msg.sOper.strOperID, (const char*)(LPCTSTR)id );
	strcpy( msg.sOper.strOperPWD, (const char*)(LPCTSTR)pass );
	
	// version setting
#ifdef _MASTER
	strcpy(msg.sOper.strRegDate, "mastertjdeoekt!" );
#else
	strcpy(msg.sOper.strRegDate, SERVERTOOLVERSION );
#endif

#ifdef _CHINA
	NETWORK->SendAll( &msg, sizeof(msg) );
#else
	NETWORK->Send( m_nServerSet, &msg, sizeof(msg) );
#endif
}

void CLoginDlg::OnButtonConnect() 
{
	EnterIDPass();
}

BOOL CLoginDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_editID.SetLimitText( 16 );
	m_editPass.SetLimitText( 16 );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLoginDlg::LoginError( int error )
{
	m_bConnect = FALSE;
	switch( error )
	{
	case 0: // 이상 오류
		{
			MessageBox( "DB Error!!\nNotify to Programmer.\nClose Program.", "Error!" );
			OnCancel();
		}
		break;
	case 1: // 등록안된 아이피
		{
			MessageBox( "Not Registed IP. Close Program.", "Error!" );
			OnCancel();
		}
		break;
	case 2: // 등록안된 아이디
		{
			MessageBox( "Not Registed ID.", "Error!" );
			OnCancel();
		}
		break;
	case 3: // 비번 오류
		{
			MessageBox( "Password is Wrong.", "Error!" );
			OnCancel();
		}
		break;
	case 4:
		{
			MessageBox( "Wrong ServerTool Version!!\nClose Program.", "Error!" );
			OnCancel();
		}
		break;
	case 99:	// level
		{
			MessageBox( "You Are Wrong Right To Use This Tool!!\nClose Program.", "Error!" );
			OnCancel();
		}
		break;
	}
}
