// AdminInfo.cpp : implementation file
//

#include "stdafx.h"
#include "DS_RMTool.h"
#include "AdminInfo.h"
#include "AdminDoc.h"
#include "RMNetwork.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdminInfo dialog

IMPLEMENT_DYNCREATE(CAdminInfo, CLDPropPage)

CAdminInfo::CAdminInfo(): CLDPropPage(CAdminInfo::IDD)
{
	//{{AFX_DATA_INIT(CAdminInfo)
	//}}AFX_DATA_INIT
	m_pDocument = NULL;
	m_nPower = 0;
}

CAdminInfo::~CAdminInfo()
{
}

void CAdminInfo::DoDataExchange(CDataExchange* pDX)
{
	CLDPropPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdminInfo)
	DDX_Control(pDX, IDC_OPER_POWER_MASTER, m_rdPower);
	DDX_Control(pDX, IDC_OPER_NEW, m_btnNew);
	DDX_Control(pDX, IDC_OPER_DEL, m_btnDel);
	DDX_Control(pDX, IDC_OPER_REPASS, m_etRepass);
	DDX_Control(pDX, IDC_OPER_PASS, m_etPass);
	DDX_Control(pDX, IDC_OPER_NAME, m_etName);
	DDX_Control(pDX, IDC_OPER_ID, m_etID);
	DDX_Control(pDX, IDC_OPER_LIST, m_listOper);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdminInfo, CLDPropPage)
	//{{AFX_MSG_MAP(CAdminInfo)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_OPER_NEW, OnOperNew)
	ON_BN_CLICKED(IDC_OPER_POWER_MASTER, OnOperPowerMaster)
	ON_BN_CLICKED(IDC_OPER_POWER_MONITOR, OnOperPowerMonitor)
	ON_BN_CLICKED(IDC_OPER_POWER_PATROLLER, OnOperPowerPatroller)
	ON_NOTIFY(NM_CLICK, IDC_OPER_LIST, OnClickOperList)
	ON_BN_CLICKED(IDC_OPER_DEL, OnOperDel)
	ON_BN_CLICKED(IDC_OPER_POWER_AUDITOR, OnOperPowerAuditor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdminInfo message handlers

int CAdminInfo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	lpCreateStruct->cx = 600;
	lpCreateStruct->cy = 500;

	if (CLDPropPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CAdminInfo::InitControl(CAdminDoc* pDoc)
{
	m_pDocument = pDoc;
	
	m_rdPower.SetCheck(BST_CHECKED);
	m_nPower = 0;
	
	InitOperList();
	UpdateOperList();
	EnableControl();
}

void CAdminInfo::InitOperList()
{
	int i = 0;
	int nColumnCount = m_listOper.GetHeaderCtrl()->GetItemCount();
	for( i = 0; i < nColumnCount; ++i )
		m_listOper.DeleteColumn( 0 );

	char* tcolumn[6] = { "Idx", "Name", "ID", "PassWord", "Power", "RegDate" };
	int tcolumnsize[6] = { 50, 80, 80, 80, 80, 140 };
	
	// listctrl column 설정
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_CENTER;
	for( i = 0; i < 6; ++i )
	{
		lvcolumn.iSubItem = i;
		lvcolumn.cx = tcolumnsize[i];
		lvcolumn.pszText = tcolumn[i];
		m_listOper.InsertColumn( i, &lvcolumn );
	}
	// style
	DWORD dwExStyle = m_listOper.GetExtendedStyle();
	dwExStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_listOper.SetExtendedStyle( dwExStyle );
}

void CAdminInfo::UpdateOperList()
{
	m_listOper.DeleteAllItems();

	char temp[256];
	LV_ITEM lvitem;

	for( int i = 0; i < m_pDocument->m_sOperator.wOperCount; ++i )
	{
		lvitem.iItem = m_listOper.GetItemCount();	// 행
		
		lvitem.iSubItem = 0;
		lvitem.mask = LVIF_TEXT;
		lvitem.stateMask = LVIS_SELECTED;
		lvitem.state = INDEXTOSTATEIMAGEMASK(1);
		lvitem.pszText = itoa( m_pDocument->m_sOperator.pOper[i].dwOperIdx, temp, 10 );
		
		m_listOper.InsertItem( &lvitem );
		m_listOper.SetItemText( lvitem.iItem, 1, m_pDocument->m_sOperator.pOper[i].strOperName );	// name
		m_listOper.SetItemText( lvitem.iItem, 2, m_pDocument->m_sOperator.pOper[i].strOperID );		// id
		m_listOper.SetItemText( lvitem.iItem, 3, m_pDocument->m_sOperator.pOper[i].strOperPWD );		// pass
		m_listOper.SetItemText( lvitem.iItem, 4, GetOperPowerName( m_pDocument->m_sOperator.pOper[i].wOperPower, temp ) );
		m_listOper.SetItemText( lvitem.iItem, 5, m_pDocument->m_sOperator.pOper[i].strRegDate );
	}
}

char* CAdminInfo::GetOperPowerName( WORD wPower, char* temp )
{
	switch( wPower )
	{
	case eOperPower_Master:		sprintf( temp, "%s", "Master" );	break;
	case eOperPower_Monitor:	sprintf( temp, "%s", "Monitor" );	break;
	case eOperPower_Patroller:	sprintf( temp, "%s", "Patroller" );	break;
	case eOperPower_Auditor:	sprintf( temp, "%s", "Auditor" );	break;		
	default:					sprintf( temp, "%s", "Error" );		break;
	}
	return temp;
}

void CAdminInfo::EnableControl(BOOL bEnable)
{	
	m_etName.EnableWindow(bEnable);
	m_etID.EnableWindow(bEnable);
	m_etPass.EnableWindow(bEnable);
	m_etRepass.EnableWindow(bEnable);

	m_btnNew.EnableWindow(bEnable);
//	m_btnDel.EnableWindow(bEnable);

	m_etName.SetWindowText("");
	m_etID.SetWindowText("");	
	m_etPass.SetWindowText("");
	m_etRepass.SetWindowText("");
}

void CAdminInfo::OperatorErrorMsg()
{
	MessageBox("이미 등록된 아이디 입니다.");
	m_etID.SetWindowText("");
	m_etName.SetWindowText("");
	m_etPass.SetWindowText("");
	m_etRepass.SetWindowText("");
	m_etID.SetFocus();
}

void CAdminInfo::OnOperPowerMaster() 
{
	m_nPower = eOperPower_Master;
}

void CAdminInfo::OnOperPowerMonitor() 
{	
	m_nPower = eOperPower_Monitor;
}

void CAdminInfo::OnOperPowerPatroller() 
{	
	m_nPower = eOperPower_Patroller;
}

void CAdminInfo::OnOperPowerAuditor() 
{
	m_nPower = eOperPower_Auditor;
}

void CAdminInfo::OnOperNew() 
{
	if( m_etName.GetWindowTextLength() == 0 )
	{
		MessageBox( "Input Name!!" );
		m_etName.SetFocus();
		return;
	}
	if( m_etID.GetWindowTextLength() == 0 )
	{
		MessageBox( "Input ID!!" );
		m_etID.SetFocus();
		return;
	}
	if( m_etPass.GetWindowTextLength() == 0 )
	{
		MessageBox( "Input Password!!" );
		m_etPass.SetFocus();
		return;
	}
	if( m_etRepass.GetWindowTextLength() == 0 )
	{
		MessageBox( "Input RePassword!!" );
		m_etRepass.SetFocus();
		return;
	}

	CString str1, str2;
	m_etPass.GetWindowText(str1);
	m_etRepass.GetWindowText(str2);
	if( strcmp(str1.GetBuffer(0), str2.GetBuffer(0)) != 0 )
	{
		MessageBox( "Wrong Password!!\n\n ReInput Password!!" );
		m_etPass.SetWindowText("");
		m_etRepass.SetWindowText("");
		m_etPass.SetFocus();
		return;
	}

	TMSG_ADMIN_INSERTOPERATOR Msg;
	Msg.Category = MP_RMTOOL_ADMIN;
	Msg.Protocol = MP_RMTOOL_ADMIN_INSERTOPERATOR_SYN;
	Msg.dwTemplateIdx = m_pDocument->GetDocumentIdx();
	m_etName.GetWindowText(Msg.strName, 17);
	m_etID.GetWindowText(Msg.strID, 17);	
	m_etPass.GetWindowText(Msg.strPass, 17);
	Msg.wPower = m_nPower;

	NETWORK->Send(&Msg, sizeof(Msg));	

}

void CAdminInfo::OnOperDel() 
{
	if( m_Pos )
	{
		int index = m_listOper.GetNextSelectedItem( m_Pos );		
		CString str = m_listOper.GetItemText( index, 0 );

		TMSG_DWORD Msg;
		Msg.Category = MP_RMTOOL_ADMIN;
		Msg.Protocol = MP_RMTOOL_ADMIN_DELETEOPERATOR_SYN;
		Msg.dwTemplateIdx = m_pDocument->GetDocumentIdx();
		Msg.dwData = atoi( (char*)(LPCTSTR)str );

		NETWORK->Send(&Msg, sizeof(Msg));		
		
	//	m_pDocument->InitData();
	}	
}

void CAdminInfo::OnClickOperList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_Pos = m_listOper.GetFirstSelectedItemPosition();
	if( m_Pos )
		m_btnDel.EnableWindow( TRUE );
	else
		m_btnDel.EnableWindow( FALSE );

	*pResult = 0;
}
