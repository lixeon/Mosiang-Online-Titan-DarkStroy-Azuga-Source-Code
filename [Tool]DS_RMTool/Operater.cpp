// Operater.cpp : implementation file
//

#include "stdafx.h"
#include "DS_RMTool.h"
#include "Operater.h"

#include "RMNetwork.h"
#include "MASInfoManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COperater dialog

COperater::COperater(CWnd* pParent /*=NULL*/)
	: CDialog(COperater::IDD, pParent)
{
	//{{AFX_DATA_INIT(COperater)
	//}}AFX_DATA_INIT
	m_sOper.Category  = MP_RMTOOL_CONNECT;
	m_sOper.Protocol = MP_RMTOOL_CONNECT_SYN;
}

void COperater::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COperater)
	DDX_Control(pDX, IDC_COMBO_SELECTSERVER, m_cbServerSet);
	DDX_Control(pDX, IDC_OPER_PSW, m_etOperPwd);
	DDX_Control(pDX, IDC_OPER_ID, m_etOperID);
	//}}AFX_DATA_MAP	
}

BEGIN_MESSAGE_MAP(COperater, CDialog)
	//{{AFX_MSG_MAP(COperater)
	ON_BN_CLICKED(IDC_BT_OK, OnBtOk)
	ON_BN_CLICKED(IDC_BT_CANCEL, OnBtCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COperater message handlers

void COperater::OnBtOk() 
{
	int index = m_cbServerSet.GetCurSel();
	if( index == -1 )
		return;
	
	// 1. ID/PWD 입력 체크
	if( m_etOperID.GetWindowTextLength() == 0 )
	{
		// id 작성 안되었을 경우
		MessageBox("Input ID!!","Error!!");
		m_etOperID.SetFocus();
		return;
	}
	if( m_etOperPwd.GetWindowTextLength() == 0 )
	{
		// pass 작성 안되었을 경우
		MessageBox("Input Password!!", "Error!!");
		m_etOperPwd.SetFocus();
		return;
	}

	MASINFO* pInfo = MASINFOMGR->GetMasInfo( index );
	NETWORK->Disconnect();
	if( !NETWORK->ConnectToServer( pInfo->strIP, pInfo->nPort ) )
	{
		MessageBox( "Can't Connect Server!!", "Error!!", MB_OK );
		return;
	}
	MASINFOMGR->SetCurMasInfo( index );

	CString strId, strPwd;
	m_etOperID.GetWindowText(strId);
	m_etOperPwd.GetWindowText(strPwd);

	strcpy(m_sOper.sOper.strOperID, strId.GetBuffer(0));
	strcpy(m_sOper.sOper.strOperPWD, strPwd.GetBuffer(0));
	
	strcpy(m_sOper.sOper.strRegDate, RMTOOLVERSION );

	NETWORK->Send(&m_sOper, sizeof(m_sOper));
}

void COperater::OnBtCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(IDCANCEL);
}

void COperater::OnOK()
{
	if (!UpdateData(TRUE))
	{
		TRACE0("UpdateData failed during dialog termination.\n");
		// the UpdateData routine will set focus to correct item
		return;
	}

	OnBtOk();
}

void COperater::OnCancel()
{
	OnBtCancel();
}

int COperater::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class

	//////////////////////////////////////////////////////////////////////////
	// IP 획득
	hostent *pHostent;
	char hostname[256];
    ::gethostname( hostname, 256 );
    pHostent = ::gethostbyname( hostname );

	strcpy( m_sOper.sIP.strIP, inet_ntoa(*(struct in_addr*)*pHostent->h_addr_list) );
	//////////////////////////////////////////////////////////////////////////
	m_sOper.sOper.dwOperIdx = 0;

	return CDialog::DoModal();
}

void COperater::ResetData(int bflag)
{
	if(bflag == 0)
	{
		m_etOperID.SetWindowText("");
		m_etOperPwd.SetWindowText("");

		m_etOperID.SetFocus();
	}
	else
	{
		m_etOperPwd.SetWindowText("");
		m_etOperPwd.SetFocus();
	}
}

BOOL COperater::OnInitDialog() 
{
	CDialog::OnInitDialog();

	for( int i = 0; i < MASINFOMGR->GetMaxMas(); ++i )
	{
		MASINFO* pInfo = MASINFOMGR->GetMasInfo( i );
		m_cbServerSet.AddString( pInfo->strServerName );
		
	}	
	m_cbServerSet.SetCurSel( 0 );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
