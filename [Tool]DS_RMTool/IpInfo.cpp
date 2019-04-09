// IpInfo.cpp : implementation file
//

#include "stdafx.h"
#include "DS_RMTool.h"
#include "IpInfo.h"
#include "AdminDoc.h"
#include "RMNetwork.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIpInfo dialog
IMPLEMENT_DYNCREATE(CIpInfo, CLDPropPage)

CIpInfo::CIpInfo(): CLDPropPage(CIpInfo::IDD)
{
	//{{AFX_DATA_INIT(CIpInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDocument = NULL;
}

CIpInfo::~CIpInfo()
{
}

void CIpInfo::DoDataExchange(CDataExchange* pDX)
{
	CLDPropPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIpInfo)
	DDX_Control(pDX, IDC_IPINFO_NEWIP, m_ipNew);
	DDX_Control(pDX, IDC_BTN_IPDEL, m_btnDel);
	DDX_Control(pDX, IDC_BTN_NEWIP, m_btnNew);
	DDX_Control(pDX, IDC_IPINFO_LIST, m_listIp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIpInfo, CLDPropPage)
	//{{AFX_MSG_MAP(CIpInfo)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BTN_NEWIP, OnBtnNewip)
	ON_BN_CLICKED(IDC_BTN_IPDEL, OnBtnIpdel)
	ON_NOTIFY(NM_CLICK, IDC_IPINFO_LIST, OnClickIpinfoList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIpInfo message handlers

int CIpInfo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	lpCreateStruct->cx = 600;
	lpCreateStruct->cy = 500;

	if (CLDPropPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CIpInfo::InitControl(CAdminDoc* pDoc)
{
	m_pDocument = pDoc;
	
	InitIpList();
	UpdateIpList();
	EnableControl();
}

void CIpInfo::EnableControl(BOOL bEnable)
{
	m_ipNew.EnableWindow(bEnable);
	m_btnNew.EnableWindow(bEnable);
//	m_btnDel.EnableWindow(bEnable);
}

void CIpInfo::InitIpList()
{
	int i = 0;
	int nColumnCount = m_listIp.GetHeaderCtrl()->GetItemCount();
	for( i = 0; i < nColumnCount; ++i )
		m_listIp.DeleteColumn( 0 );

	char* tcolumn[3] = { "Idx", "Registed IP", "RegDate" };
	int tcolumnsize[3] = { 80, 140, 140 };
	
	// listctrl column 설정
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_CENTER;
	for( i = 0; i < 3; ++i )
	{
		lvcolumn.iSubItem = i;
		lvcolumn.cx = tcolumnsize[i];
		lvcolumn.pszText = tcolumn[i];
		m_listIp.InsertColumn( i, &lvcolumn );
	}
	// style
	DWORD dwExStyle = m_listIp.GetExtendedStyle();
	dwExStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_listIp.SetExtendedStyle( dwExStyle );	
}

void CIpInfo::UpdateIpList() 
{
	m_listIp.DeleteAllItems();

	char temp[256];
	LV_ITEM lvitem;

	for( int i = 0; i < m_pDocument->m_sOperator.wIPCount; ++i )
	{
		lvitem.iItem = m_listIp.GetItemCount();	// 행
		
		lvitem.iSubItem = 0;
		lvitem.mask = LVIF_TEXT;
		lvitem.stateMask = LVIS_SELECTED;
		lvitem.state = INDEXTOSTATEIMAGEMASK(1);
		lvitem.pszText = itoa( m_pDocument->m_sOperator.pIp[i].dwIPIdx, temp, 10 );
		
		m_listIp.InsertItem( &lvitem );
		m_listIp.SetItemText( lvitem.iItem, 1, m_pDocument->m_sOperator.pIp[i].strIP );			// ip
		m_listIp.SetItemText( lvitem.iItem, 2, m_pDocument->m_sOperator.pIp[i].strRegDate );	// date
	}
}

void CIpInfo::IPErrorMsg()
{
	MessageBox("이미 등록된 IP입니다.");

	m_ipNew.ClearAddress();
	m_ipNew.SetFocus();
}

void CIpInfo::OnBtnNewip() 
{	
	CString strIp;
	DWORD ip;

	if( m_ipNew.IsBlank() )
	{
		MessageBox( "Input Correct IP!!");
		m_ipNew.ClearAddress();
		m_ipNew.SetFocus();
		return;
	}

	m_ipNew.GetAddress(ip);
	strIp.Format( "%d.%d.%d.%d", (BYTE)FIRST_IPADDRESS(ip), (BYTE)SECOND_IPADDRESS(ip), 
								 (BYTE)THIRD_IPADDRESS(ip), (BYTE)FOURTH_IPADDRESS(ip) );

	TMSG_ADMIN_INSERTIP Msg;
	Msg.Category = MP_RMTOOL_ADMIN;
	Msg.Protocol = MP_RMTOOL_ADMIN_INSERTIP_SYN;
	Msg.dwTemplateIdx = m_pDocument->GetDocumentIdx();
	strcpy( Msg.strIP, strIp.GetBuffer(0) );
	NETWORK->Send(&Msg, sizeof(Msg));
}

void CIpInfo::OnBtnIpdel() 
{
	if( m_Pos )
	{
		int index = m_listIp.GetNextSelectedItem( m_Pos );		
		CString str = m_listIp.GetItemText( index, 0 );

		TMSG_DWORD Msg;
		Msg.Category = MP_RMTOOL_ADMIN;
		Msg.Protocol = MP_RMTOOL_ADMIN_DELETEIP_SYN;
		Msg.dwTemplateIdx = m_pDocument->GetDocumentIdx();
		Msg.dwData = atoi( (char*)(LPCTSTR)str );

		NETWORK->Send(&Msg, sizeof(Msg));		
		
		m_pDocument->InitData();
	}	
}

void CIpInfo::OnClickIpinfoList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_Pos = m_listIp.GetFirstSelectedItemPosition();
	if( m_Pos )
		m_btnDel.EnableWindow( TRUE );
	else
		m_btnDel.EnableWindow( FALSE );

	*pResult = 0;
}
