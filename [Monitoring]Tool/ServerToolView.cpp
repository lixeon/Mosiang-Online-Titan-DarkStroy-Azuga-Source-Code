// ServerToolView.cpp : implementation of the CServerToolView class
//

#include "stdafx.h"
#include "ServerTool.h"

#include "MainFrm.h"
#include "ServerToolDoc.h"
#include "ServerToolView.h"
#include "ServerSetMng.h"
#include "MSInfoDlg.h"
#include "UserCount.h"

#include "MHNetwork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CServerToolView*	g_ServerToolView = NULL;
extern sMSGOPTION	gMsgOption;

/////////////////////////////////////////////////////////////////////////////
// CServerToolView

IMPLEMENT_DYNCREATE(CServerToolView, CFormView)

BEGIN_MESSAGE_MAP(CServerToolView, CFormView)
	//{{AFX_MSG_MAP(CServerToolView)
	ON_CBN_SELCHANGE(IDC_COMBO_SERVERSET, OnSelchangeComboServerset)
	ON_BN_CLICKED(IDC_BUTTON_SERVER_ON, OnButtonServerOn)
	ON_BN_CLICKED(IDC_BUTTON_SERVER_OFF, OnButtonServerOff)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_MESSAGE, OnChangeEditMessage)
	ON_BN_CLICKED(IDC_BUTTON_COMMAND_SEND, OnButtonCommandSend)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTCONTROL_DIST, OnDblclkListcontrolDist)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTCONTROL_AGENT, OnDblclkListcontrolAgent)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTCONTROL_MAP, OnDblclkListcontrolMap)
	ON_NOTIFY(NM_DBLCLK, IDC_LISTCONTROL_MURIM, OnDblclkListcontrolMurim)
	ON_NOTIFY(NM_RDBLCLK, IDC_LISTCONTROL_MAP, OnRdblclkListcontrolMap)
	ON_NOTIFY(NM_RDBLCLK, IDC_LISTCONTROL_DIST, OnRdblclkListcontrolDist)
	ON_NOTIFY(NM_RDBLCLK, IDC_LISTCONTROL_AGENT, OnRdblclkListcontrolAgent)
	ON_NOTIFY(NM_RDBLCLK, IDC_LISTCONTROL_MURIM, OnRdblclkListcontrolMurim)
	ON_BN_CLICKED(IDC_BUTTON_NOTIFY_EDIT, OnButtonNotifyEdit)
	ON_BN_CLICKED(IDC_BUTTON_NOTIFY, OnButtonNotify)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_SERVER_REFRESH, OnButtonServerRefresh)
	ON_BN_CLICKED(IDC_BUTTON_AUTOPATCH, OnButtonAutopatch)
	ON_COMMAND(ID_FILE_MAS_OPEN, OnFileMasOpen)
	ON_BN_CLICKED(IDC_BUTTON_USERLEVEL, OnButtonUserlevel)
	ON_BN_CLICKED(IDC_BUTTON_OFFMSGBOX, OnButtonOffmsgbox)
	ON_BN_CLICKED(IDC_BUTTON_ONMSGBOX, OnButtonOnmsgbox)
	ON_BN_CLICKED(IDC_BUTTON_SERVER_USEROUT, OnButtonServerUserout)
	ON_BN_CLICKED(IDC_BUTTON_VERSION_CHANGE, OnButtonVersionChange)
	ON_BN_CLICKED(IDC_BUTTON_MAXUSER_CHANGE, OnButtonMaxuserChange)
	ON_BN_CLICKED(IDC_BUTTON_USERCOUNT, OnButtonUsercount)
	ON_NOTIFY(NM_CLICK, IDC_LISTCONTROL_DIST, OnClickListcontrolDist)
	ON_NOTIFY(NM_CLICK, IDC_LISTCONTROL_AGENT, OnClickListcontrolAgent)
	ON_NOTIFY(NM_CLICK, IDC_LISTCONTROL_MAP, OnClickListcontrolMap)
	ON_BN_CLICKED(IDC_BUTTON_TERMINATESERVER, OnButtonTerminateserver)
	ON_BN_CLICKED(IDC_BUTTON_OPTION, OnButtonOption)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerToolView construction/destruction

CServerToolView::CServerToolView()
	: CFormView(CServerToolView::IDD)
{
	//{{AFX_DATA_INIT(CServerToolView)
	m_strCurTime = _T("");
	m_strCurFileName = _T("None");
	m_stNotify = _T("None");
	m_stUserCount = _T("0");
	m_strDir = _T("");
	//}}AFX_DATA_INIT
	// TODO: add construction code here

	g_ServerToolView = this;

	m_bReconnect = FALSE;
	m_dwDisconNum = 0;

	m_wRefreshTime = 30;
	m_wWriteCountTime = 30;

	m_dwServerOffTime = 0;

	m_nToolLevel = eOperPower_Patroller;
	m_bServerOff = FALSE;
	
	m_pUserCountDlg = NULL;
	m_pMSInfoDlg = NULL;
	m_nKill = 0;
}

CServerToolView::~CServerToolView()
{
}

void CServerToolView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerToolView)
	DDX_Control(pDX, IDC_BUTTON_OPTION, m_btnMsgOption);
	DDX_Control(pDX, IDC_BUTTON_USERCOUNT, m_btnUserCount);
	DDX_Control(pDX, IDC_BUTTON_TERMINATESERVER, m_btnTServer);
	DDX_Control(pDX, IDC_BUTTON_VERSION_CHANGE, m_btnVersionChange);
	DDX_Control(pDX, IDC_BUTTON_MAXUSER_CHANGE, m_btnMaxUserChange);
	DDX_Control(pDX, IDC_EDIT_MAXUSER, m_editMaxUser);
	DDX_Control(pDX, IDC_EDIT_VERSION, m_editVersion);
	DDX_Control(pDX, IDC_BUTTON_SERVER_USEROUT, m_btnUserOut);
	DDX_Control(pDX, IDC_BUTTON_SERVER_REFRESH, m_btnServerRefresh);
	DDX_Control(pDX, IDC_BUTTON_SERVER_OFF, m_btnServerOff);
	DDX_Control(pDX, IDC_BUTTON_SERVER_ON, m_btnServerOn);
	DDX_Control(pDX, IDC_COMBO_USERLEVEL, m_cbUserLevel);
	DDX_Control(pDX, IDC_BUTTON_USERLEVEL, m_btnChangeUserLevel);
	DDX_Control(pDX, IDC_BUTTON_AUTOPATCH, m_btnAutoPatch);
	DDX_Control(pDX, IDC_EDIT_REPEAT_NUM, m_editRepeatNum);
	DDX_Control(pDX, IDC_BUTTON_NOTIFY_EDIT, m_btnNotifyEdit);
	DDX_Control(pDX, IDC_BUTTON_NOTIFY, m_btnNotify);
	DDX_Control(pDX, IDC_STATIC_TOTAL, m_stTotal);
	DDX_Control(pDX, IDC_LISTCONTROL_TOTAL, m_lcTotal);
	DDX_Control(pDX, IDC_COMBO_SERVERSET, m_cbServerSet);
	DDX_Text(pDX, IDC_STATIC_CURTIME, m_strCurTime);
	DDX_Control(pDX, IDC_LISTCONTROL_DIST, m_lcDist);
	DDX_Control(pDX, IDC_LISTCONTROL_AGENT, m_lcAgent);
	DDX_Control(pDX, IDC_LISTCONTROL_MAP, m_lcMap);
	DDX_Control(pDX, IDC_STATIC_DISTRIBUTE, m_stDist);
	DDX_Control(pDX, IDC_STATIC_AGENT, m_stAgent);
	DDX_Control(pDX, IDC_STATIC_MAP, m_stMap);
	DDX_Control(pDX, IDC_COMBO_SERVERSET_COMMAND, m_cbCmdServerSet);
	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_MessageEdit);
	DDX_Control(pDX, IDC_EDIT_REPEAT, m_RepeatEdit);
	DDX_Text(pDX, IDC_STATIC_FILENAME, m_strCurFileName);
	DDX_Text(pDX, IDC_STATIC_NOTIFY, m_stNotify);
	DDX_Text(pDX, IDC_STATIC_USERCOUNT, m_stUserCount);
	//}}AFX_DATA_MAP
}

BOOL CServerToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	
	return CFormView::PreCreateWindow(cs);
}

void CServerToolView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();	

	char temp[256];
	sprintf( temp, "ServerTool(%s)", SERVERTOOLVERSION );
	AfxGetMainWnd()->SetWindowText( temp );
	
	m_Font.CreatePointFont(100, "Arial");
	m_lcTotal.SetFont( &m_Font );
	m_lcDist.SetFont( &m_Font );
	m_lcAgent.SetFont( &m_Font );
	m_lcMap.SetFont( &m_Font );
		
	// timer 설정( 1초마다 )
	SetTimer( 0, 1000, NULL );
	
	// 이미지 불러오기
	m_StateImageList.Create( IDB_BITMAP_STATE, 16, 1, RGB(255,255,255) );
	m_bmpGray.LoadBitmap( IDB_BITMAP_GRAY );
	m_bmpGreen.LoadBitmap( IDB_BITMAP_GREEN );
	
	InitServerControl();
	InitServerMonitor();	
	InitCommand();

	if( !Init() )
	{
		GetParentFrame()->PostMessage( WM_CLOSE );
		return;
	}

	// Login Dlg
	if( m_LoginDlg.DoModal() == IDCANCEL && !m_LoginDlg.m_bConnect )
	{
		GetParentFrame()->PostMessage( WM_CLOSE );
	}
	else
	{
		LOG( "!!!!! ServerTool Start !!!!!" );

		SERVERSETMNG->DoQueryClientExeVersion();
		SERVERSETMNG->DoQueryMaxUser();		
		SERVERSETMNG->DoQueryUserLevel();
		SERVERSETMNG->DoQueryUserCount();
	}

	UpdateData( FALSE );	
}

BOOL CServerToolView::Init()
{
	if( !SERVERSETMNG->Init( "MASList.ini" ) )
		return FALSE;

	// ReadINI
	ReadIniFile( "Tool.ini" );
	
	// Network Init
	if( !NETWORK->Init( this->GetParentFrame()->m_hWnd ) )
		return FALSE;

	m_cbServerSet.ResetContent();
	m_cbCmdServerSet.ResetContent();
	m_cbServerSet.AddString( "All Server" );
	m_cbCmdServerSet.AddString( "All Server" );

	DWORD tick = GetTickCount();
	CServerSet* pServerSet = NULL;
	int count = 0;
	int successedset = -1;
	for( int i = 0; i < SERVERSETMNG->GetMaxServerSet(); ++i )
	{
		pServerSet = SERVERSETMNG->GetServerSet( i );
		if( pServerSet )
		{
			if( !NETWORK->ConnectToServer( i, pServerSet->m_sMASIP, pServerSet->m_nMASPort ) )
			{
				pServerSet->Release();
			}
			else
			{
				pServerSet->m_dwMsgCheckTime = tick;

				m_cbServerSet.AddString( pServerSet->m_sServerSetName );
				m_cbCmdServerSet.AddString( pServerSet->m_sServerSetName );
				if(successedset == -1)
					successedset = i;
				++count;
			}
		}
	}
	m_cbServerSet.SetCurSel( 0 );
	m_cbCmdServerSet.SetCurSel( 0 );

	if( count )
	{	
		m_LoginDlg.m_nServerSet = successedset;

		UpdateServerControl();
		OnSelchangeComboServerset();	// UpdateServerMonitor()
	}
	else
	{
		MessageBox( "Can't Connect To MAS!!", "Error!!", MB_OK );
		return FALSE;
	}
	
	m_LoginDlg.m_nReceiveCount = count;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CServerToolView diagnostics

#ifdef _DEBUG
void CServerToolView::AssertValid() const
{
	CFormView::AssertValid();
}

void CServerToolView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
//// 모달리스 대화상자 헤더문제로 지움....
/*
CServerToolDoc* CServerToolView::GetDocument() // non-debug version is inline
{
//	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CServerToolDoc)));
//	return (CServerToolDoc*)m_pDocument;
}
*/
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CServerToolView message handlers

void CServerToolView::OnTimer(UINT nIDEvent) 
{
	SYSTEMTIME time;
	GetLocalTime( &time );
	m_strCurTime.Format( "%d-%d-%d %d:%d:%d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond );

	/*
#ifdef _CHINA
	if( m_nToolLevel == eOperPower_Master )
	{
		// UserCount
		if( (time.wSecond%m_wRefreshTime) == (m_wRefreshTime-2) )
			SERVERSETMNG->DoQueryUserCount();

		if( (time.wSecond%m_wWriteCountTime) == 0 )
		{
			SERVERSETMNG->WriteUserCount( time );
			UpdateUserCount();
		}
	}
#else
	*/
	if( m_nToolLevel == eOperPower_Master || m_nToolLevel == eOperPower_Monitor )
	{
		// UserCount
		if( (time.wSecond%m_wRefreshTime) == (m_wRefreshTime-2) )
			SERVERSETMNG->DoQueryUserCount();

		if( m_nToolLevel == eOperPower_Master )
		{
			if( (time.wSecond%m_wWriteCountTime) == 0 )
			{
				SERVERSETMNG->WriteUserCount( time );
				UpdateUserCount();
			}
		}
	}

	// 반복 공지 사항
	if( m_bNotify )
	{
		--m_nCheckTime;
		if( m_nCheckTime <= 0 )
		{
			m_nCheckTime = m_nRepeatTime;

			if( m_NotifyMessage.IsRepeat() )
			{
				char* pmsg = m_NotifyMessage.GetMessage();
				if( pmsg )
				{					
					CString str;
					m_cbCmdServerSet.GetLBText( m_cbServerSet.GetCurSel(), str );
					SERVERSETMNG->DoSendMessage( (char*)(LPCTSTR)str, pmsg );
					
					char temp[256];
					m_editRepeatNum.SetWindowText( itoa( m_NotifyMessage.GetRepeatNum(), temp, 10 ) );
					m_stNotify = pmsg;
				}
			}
			else
			{
				m_bNotify = FALSE;
				m_btnNotify.SetBitmap( m_bmpGray );
			}
		}
	}

	// MAS와의 접속이 끊어졌을때 다시 접속한다.
	if( m_bReconnect )
	{
		ReConnectToMAS(); 
		LOG( "Disconnect : Reconnect To MAS (Count = %d)", m_dwDisconNum );
	}

	// 유저 접속 끊기.
	if( m_bServerOff )
	{
		++m_dwServerOffTime;
		if( m_dwServerOffTime > 5 )
		{
			SERVERSETMNG->DoServerSetOffAfterUserDiscon( (char*)(LPCTSTR)m_strAutoPatch );

			m_bServerOff = FALSE;
			m_dwServerOffTime = 0;

			LOG( "ServerOffAfterUserDiscon : %s", (char*)(LPCTSTR)m_strAutoPatch );
		}
	}

	// MsgCheckTime
//	if( (time.wMinute%2) == 0 )	// 2분마다
	if( time.wSecond == 0 )		// 1분마다
	{
		DWORD tick = GetTickCount();
		CServerSet* pServerSet = NULL;
		for( int i = 0; i < SERVERSETMNG->GetMaxServerSet(); ++i )
		{
			pServerSet = SERVERSETMNG->GetServerSet( i );
			if( pServerSet )
			if( pServerSet->m_bInit )
			if( pServerSet->IsThereOnServer() )
			{
				if( (tick - pServerSet->m_dwMsgCheckTime) >= 180000 )
				{					
					LOG( "MsgCheckTime : Reconnect To MAS" );
					ReConnectToMAS();
					return;
				}
			}
		}
	}		

	UpdateData( FALSE );

	CFormView::OnTimer(nIDEvent);
}

void CServerToolView::OnDestroy() 
{
	CFormView::OnDestroy();

	KillTimer( 0 );
}

// Init -------------------------------------------------------------------------------------------
void CServerToolView::InitServerControl()
{	
	m_editVersion.SetWindowText( "Version" );
	m_editMaxUser.SetWindowText( "MaxUser" );

	// userlevel
	CString user[7] = { "God", "Programmer", "Developer", "GM", "SuperUser", "User", "SubUser" };
	for( int i = 0; i < 7; ++i )
		m_cbUserLevel.AddString( user[i] );
	m_cbUserLevel.SetCurSel( 0 );

	EnableServerControl( FALSE );
}

void CServerToolView::InitServerMonitor()
{
	char* tcolumn[4] = { "State", "UserCount", "Server Name", "Index" };
	int tcolumnsize[4] = { 50, 70, 80, 50 };
	
	// listctrl column 설정
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_CENTER;

	int i;
	for( i = 0; i < 4; ++i )
	{
		lvcolumn.iSubItem = i;
		lvcolumn.cx = tcolumnsize[i];
		lvcolumn.pszText = tcolumn[i];
		m_lcTotal.InsertColumn( i, &lvcolumn );
	}

	//
	char* column[7] = { "State", "UserCount", "Server Name", "MapNum", "IP", "Port", "Index" };
	int columnsize[7] = { 50, 70, 80, 50, 100, 50, 30 };
	
	for( i = 0; i < 7; ++i )
	{
		lvcolumn.iSubItem = i;
		lvcolumn.cx = columnsize[i];
		lvcolumn.pszText = column[i];
		m_lcDist.InsertColumn( i, &lvcolumn );
		m_lcAgent.InsertColumn( i, &lvcolumn );
//		m_lcMurim.InsertColumn( i, &lvcolumn );
		m_lcMap.InsertColumn( i, &lvcolumn );
	}

	// style
	m_lcTotal.SetExtendedStyle( LVS_EX_FULLROWSELECT );
	m_lcDist.SetExtendedStyle( LVS_EX_FULLROWSELECT );
	m_lcAgent.SetExtendedStyle( LVS_EX_FULLROWSELECT );
//	m_lcMurim.SetExtendedStyle( LVS_EX_FULLROWSELECT );
	m_lcMap.SetExtendedStyle( LVS_EX_FULLROWSELECT );

	// state image
	m_lcTotal.SetImageList( &m_StateImageList, LVSIL_STATE );
	m_lcDist.SetImageList( &m_StateImageList, LVSIL_STATE );
	m_lcAgent.SetImageList( &m_StateImageList, LVSIL_STATE );
//	m_lcMurim.SetImageList( &m_StateImageList, LVSIL_STATE );
	m_lcMap.SetImageList( &m_StateImageList, LVSIL_STATE );

	ShowServerMonitor( FALSE );	
	EnableServerMonitor( FALSE );
}

void CServerToolView::InitCommand()
{
	// 공지 button
	m_btnNotify.SetBitmap( m_bmpGray );
	m_bNotify = FALSE;

	// 반복 횟수 edit
	m_editRepeatNum.SetWindowText( "1" );
	m_nRepeatNum = 1;

	// 시간 edit
	m_RepeatEdit.SetWindowText( "60" );
	m_nRepeatTime = m_nCheckTime = 60;	

	// message edit
	m_MessageEdit.SetLimitText( 256 );
	
	EnableCommand( FALSE );
}

void CServerToolView::EnableServerControl( BOOL bShow )
{	
	m_cbServerSet.EnableWindow( TRUE );
	m_btnServerOn.EnableWindow( bShow );
	m_btnServerOff.EnableWindow( bShow );
	m_btnServerRefresh.EnableWindow( TRUE );
	m_btnUserOut.EnableWindow( FALSE );

	m_editVersion.EnableWindow( bShow );
	m_editMaxUser.EnableWindow( bShow );
	m_cbUserLevel.EnableWindow( bShow );
	m_btnVersionChange.EnableWindow( bShow );
	m_btnMaxUserChange.EnableWindow( bShow );
	m_btnChangeUserLevel.EnableWindow( bShow );

	m_btnUserCount.EnableWindow( bShow );
	m_btnAutoPatch.EnableWindow( bShow );
}

void CServerToolView::EnableServerMonitor( BOOL bShow )
{
	m_lcTotal.EnableWindow( bShow );
	m_stDist.EnableWindow( bShow );
	m_stAgent.EnableWindow( bShow );
	m_stMap.EnableWindow( bShow );
}

void CServerToolView::EnableCommand( BOOL bShow )
{
	m_cbCmdServerSet.EnableWindow( bShow );
	m_btnNotify.EnableWindow( bShow );
	m_editRepeatNum.EnableWindow( bShow );
	m_RepeatEdit.EnableWindow( bShow );
	m_btnNotifyEdit.EnableWindow( bShow );
	m_MessageEdit.EnableWindow( bShow );
}

void CServerToolView::EnableToolFromLevel( int nUserLevel )
{
	m_nToolLevel = nUserLevel;

	switch( m_nToolLevel )
	{
	case eOperPower_Monitor:
		{
			EnableServerControl( FALSE );
			EnableServerMonitor( TRUE );
			EnableCommand( FALSE );
		}
		break;

	case eOperPower_Master:
		{
			EnableServerControl( TRUE );
			EnableServerMonitor( TRUE );
			EnableCommand( TRUE );
		}
		break;
	}
}

void CServerToolView::ShowServerMonitor( BOOL bShow )
{
	// total
	m_lcTotal.ShowWindow( !bShow );
	m_stTotal.ShowWindow( !bShow );

	// distribute zone
	m_stDist.ShowWindow( bShow );
	m_lcDist.ShowWindow( bShow );

	// agent zone
	m_stAgent.ShowWindow( bShow );
	m_lcAgent.ShowWindow( bShow );

	// map zone
	m_stMap.ShowWindow( bShow );
	m_lcMap.ShowWindow( bShow );

	m_btnTServer.ShowWindow( bShow );
}

// update -----------------------------------------------------------------------------------------
void CServerToolView::UpdateServerControl()
{
	UpdateUserCount();

	m_editMaxUser.SetWindowText( "MaxUser" );
	m_editVersion.SetWindowText( "Version" );
	m_cbUserLevel.SetCurSel( 0 );

	if( m_cbServerSet.GetCurSel() > 0 )
	{		
		CString str;
		m_cbServerSet.GetLBText( m_cbServerSet.GetCurSel(), str );

		CServerSet* pServerSet = SERVERSETMNG->GetServerSet( (char*)(LPCTSTR)str );
		if( pServerSet )
		{
			char temp[256];
			// MaxUser
			m_editMaxUser.SetWindowText( itoa((int)pServerSet->m_dwMaxUser, temp, 10) );
			// ClientExeVersion
			m_editVersion.SetWindowText( pServerSet->m_sClientExeVersion );

			// UserLevel
			m_cbUserLevel.SetCurSel( pServerSet->m_nUserLevel-1 );

			UpdateData( FALSE );
		}
	}
}

void CServerToolView::UpdateUserCount()
{
	if( m_cbServerSet.GetCurSel() > 0 )
	{
		CString str;
		m_cbServerSet.GetLBText( m_cbServerSet.GetCurSel(), str );

		CServerSet* pServerSet = SERVERSETMNG->GetServerSet( (char*)(LPCTSTR)str );
		if( pServerSet )
			m_stUserCount.Format( "%d", pServerSet->GetTotalUserCount() );
	}
	else
		m_stUserCount.Format( "%d", SERVERSETMNG->GetTotalUserCount() );

	UpdateData( FALSE );
}

void CServerToolView::UpdateServerMonitor()
{
	CServerSet* pServerSet = NULL;
	CServer* pServer = NULL;
	LV_ITEM lvitem;
	char temp[256];
	int i = 0;
	int index = m_cbServerSet.GetCurSel();
	
	if( index == 0 )	// All Server
	{
		m_lcTotal.DeleteAllItems();
		
		for( i = 1; i < m_cbServerSet.GetCount(); ++i )
		{
			m_cbServerSet.GetLBText( i, temp );
			pServerSet = SERVERSETMNG->GetServerSet( temp );
			if( pServerSet )
			{
				pServer = pServerSet->GetServer( DISTRIBUTE_SERVER, 0 );
				if( pServer )
				{
					lvitem.iItem = m_lcTotal.GetItemCount();		// 행
					
					lvitem.iSubItem = 0;
					lvitem.mask = LVIF_TEXT | LVIF_STATE;
					lvitem.stateMask = LVIS_STATEIMAGEMASK;
					lvitem.state = INDEXTOSTATEIMAGEMASK( pServer->m_nState );
					lvitem.pszText = pServer->m_sState;
					
					m_lcTotal.InsertItem( &lvitem );
					m_lcTotal.SetItemText( lvitem.iItem, 1, itoa( pServerSet->GetTotalUserCount(), temp, 10 ) );
					m_lcTotal.SetItemText( lvitem.iItem, 2, pServerSet->m_sServerSetName );
					m_lcTotal.SetItemText( lvitem.iItem, 3, itoa( pServerSet->m_nServerSet, temp, 10 ) );
				}
			}

			m_stUserCount.Format( "%d", SERVERSETMNG->m_dwTotalUserCount );
		}
	}
	else
	{
		// delete item
		m_lcDist.DeleteAllItems();
		m_lcAgent.DeleteAllItems();
//		m_lcMurim.DeleteAllItems();
		m_lcMap.DeleteAllItems();
		
		m_cbServerSet.GetLBText( index, temp );
		pServerSet = SERVERSETMNG->GetServerSet( temp );
		if( pServerSet == NULL ) return;
		
		// distribute
		CListCtrl* pListCtrl;
		vector<CServer*>::iterator iter = pServerSet->m_vecServer.begin();
		for(;iter != pServerSet->m_vecServer.end();++iter)
		{
			pServer = (*iter);
			switch(pServer->m_nServerKind)
			{
			case DISTRIBUTE_SERVER:	pListCtrl = &m_lcDist;	break;
			case AGENT_SERVER:	pListCtrl = &m_lcAgent;	break;
//			case MURIM_SERVER:	pListCtrl = &m_lcMurim;	break;
			case MAP_SERVER:	pListCtrl = &m_lcMap;	break;
			default:
				continue;
			}

			lvitem.iItem = pListCtrl->GetItemCount();		// 행
			
			lvitem.iSubItem = 0;
			lvitem.mask = LVIF_TEXT | LVIF_STATE;
			lvitem.stateMask = LVIS_STATEIMAGEMASK;
			lvitem.state = INDEXTOSTATEIMAGEMASK( pServer->m_nState );
			lvitem.pszText = pServer->m_sState;
			
			pListCtrl->InsertItem( &lvitem );
			pListCtrl->SetItemText( lvitem.iItem, 1, itoa( pServer->m_dwUserCount, temp, 10 ) );
			pListCtrl->SetItemText( lvitem.iItem, 2, pServer->m_sName );
			pListCtrl->SetItemText( lvitem.iItem, 3, itoa( pServer->m_nServerNum, temp, 10 ) );
			pListCtrl->SetItemText( lvitem.iItem, 4, pServer->m_sIP );
			pListCtrl->SetItemText( lvitem.iItem, 5, itoa( pServer->m_nPort, temp, 10 ) );		
		}
	}
	UpdateData( FALSE );
}

void CServerToolView::UpdateCommand()
{
}

// Server Control ---------------------------------------------------------------------------------
void CServerToolView::OnSelchangeComboServerset() 
{
	if( m_cbServerSet.GetCurSel() > 0 )
		ShowServerMonitor( TRUE );
	else
		ShowServerMonitor( FALSE );

	UpdateServerControl();
	UpdateServerMonitor();
}

void CServerToolView::OnButtonServerOn() 
{
	if( m_nToolLevel != eOperPower_Master )	return;

	CString str, temp;
	m_cbServerSet.GetWindowText( str );

	temp.Format( "Turn On %s Server?", str );
	if( MessageBox( temp, "Turn On", MB_OKCANCEL ) == IDOK )
	{
		SERVERSETMNG->DoServerSetOn( (char*)(LPCTSTR)str );
		m_bServerOff = FALSE;

		LOG( "Button ServerOn : %s", (char*)(LPCTSTR)str );
	}
}

void CServerToolView::OnButtonServerOff() 
{
	if( m_nToolLevel != eOperPower_Master )	return;

	CString str, temp;
	m_cbServerSet.GetWindowText( m_strAutoPatch );

	temp.Format( "Turn Off %s Server?", m_strAutoPatch );
	if( MessageBox( temp, "Turn Off", MB_OKCANCEL ) == IDOK )
//		SERVERSETMNG->DoServerSetOff( (char*)(LPCTSTR)m_strAutoPatch );
	{
		SERVERSETMNG->DoAllUserDisconnect( (char*)(LPCTSTR)m_strAutoPatch );
		m_bServerOff = TRUE;

		m_dwServerOffTime = 0;

		LOG( "Button ServerOff(AllUserDisconnect) : %s", (char*)(LPCTSTR)m_strAutoPatch );
	}
}

void CServerToolView::OnButtonServerRefresh() 
{
/*	CString str;
	m_cbServerSet.GetWindowText( str );

//	SERVERSETMNG->DoServerSetRefresh( (char*)(LPCTSTR)str );
	SERVERSETMNG->DoQueryUserCount();
*/
	ReConnectToMAS();

	LOG( "Button Refresh : Reconnect To MAS!!" );
}

void CServerToolView::OnButtonServerUserout() 
{
/*	CString str, temp;
	m_cbServerSet.GetWindowText( str );

	temp.Format( "%s를(을) Turn Off 하시겠습니까?", str );
	if( MessageBox( temp, "Turn Off", MB_OKCANCEL ) == IDOK )
		SERVERSETMNG->DoAllUserDisconnect( (char*)(LPCTSTR)str );
*/
}

void CServerToolView::OnButtonVersionChange() 
{
	if( m_nToolLevel != eOperPower_Master )	return;

	CString str, temp, version;
	m_cbServerSet.GetWindowText( str );
	m_editVersion.GetWindowText( version );	

	temp.Format( "Change %s's Version To %s?", str, version );
	if( MessageBox( temp, "Version Change", MB_OKCANCEL ) == IDOK )
	{		
		SERVERSETMNG->DoChangeClientExeVersion( (char*)(LPCTSTR)str, (char*)(LPCTSTR)version );

		LOG( "Button VersionChange : %s", (char*)(LPCTSTR)version );
	}
}

void CServerToolView::OnButtonMaxuserChange() 
{	
	if( m_nToolLevel != eOperPower_Master )	return;

	CString str, temp, maxuser;
	m_cbServerSet.GetWindowText( str );
	m_editMaxUser.GetWindowText( maxuser );
	DWORD user = (DWORD)atoi( (const char*)(LPCTSTR)maxuser );

	temp.Format( "Change %s's MaxUser To %s?", str, maxuser );
	if( MessageBox( temp, "MaxUser Change", MB_OKCANCEL ) == IDOK )
	{		
		SERVERSETMNG->DoChangeMaxUser( (char*)(LPCTSTR)str, user );

		LOG( "Button MaxUserChange : %d", user );
	}
}

void CServerToolView::OnButtonUserlevel() 
{
	CString str;
	m_cbServerSet.GetWindowText( str );

	CString user[7] = { "God", "Programmer", "Developer", "GM", "SuperUser", "User", "SubUser" };
	int level = m_cbUserLevel.GetCurSel();
	
	char temp[256];
	wsprintf( temp, "Change %s's UserLevel To %s?", str, user[level] );
	if( MessageBox( temp, "UserLevel", MB_OKCANCEL ) == IDOK )
	{
		SERVERSETMNG->DoChangeUserLevel( (char*)(LPCTSTR)str, level+1 );

		LOG( "Button UserLevelChange : %d", level+1 );
	}
}

void CServerToolView::OnButtonAutopatch() 
{
	if( m_nToolLevel != eOperPower_Master )	return;

	if( m_pMSInfoDlg )
		m_pMSInfoDlg->SetFocus();
	else
	{
		m_pMSInfoDlg = new CMSInfoDlg;
		m_pMSInfoDlg->Create( (CMainFrame*)AfxGetMainWnd() );
		m_pMSInfoDlg->ShowWindow( SW_SHOW );
	}	
}


void CServerToolView::OnButtonUsercount() 
{
	if( m_nToolLevel != eOperPower_Master )
		return;

	if( m_pUserCountDlg )
		m_pUserCountDlg->SetFocus();
	else
	{
		m_pUserCountDlg = new CUserCount;
		m_pUserCountDlg->Create( (CMainFrame*)AfxGetMainWnd() );
		m_pUserCountDlg->ShowWindow( SW_SHOW );
	}	
}

// 필요없을지도...
void CServerToolView::OnButtonOffmsgbox() 
{
	CString str, temp;
	m_cbServerSet.GetWindowText( str );

	temp.Format( "Turn Off %s's AssertMsgBox?", str );
	if( MessageBox( temp, "Assert MsgBox Off", MB_OKCANCEL ) == IDOK )
	{
		SERVERSETMNG->DoAssertMsgBox( (char*)(LPCTSTR)str, FALSE );
	}
}

void CServerToolView::OnButtonOnmsgbox() 
{
	CString str, temp;
	m_cbServerSet.GetWindowText( str );

	temp.Format( "Turn On %s's AssertMsgBox?", str );
	if( MessageBox( temp, "Assert MsgBox On", MB_OKCANCEL ) == IDOK )
	{
		SERVERSETMNG->DoAssertMsgBox( (char*)(LPCTSTR)str, TRUE );
	}
}

void CServerToolView::ReConnectToMAS() 
{
	m_bReconnect = FALSE;
	
	NETWORK->Release();

	if( SERVERSETMNG->Init( "MASList.ini" ) )
	{	
		NETWORK->Init(this->GetParentFrame()->m_hWnd );
//------------------------------------------------------------------------------------
		m_cbServerSet.ResetContent();
		m_cbCmdServerSet.ResetContent();
		m_cbServerSet.AddString( "All Server" );
		m_cbCmdServerSet.AddString( "All Server" );

		DWORD tick = GetTickCount();
		CServerSet* pServerSet = NULL;
		int count = 0;
		for( int i = 0; i < SERVERSETMNG->GetMaxServerSet(); ++i )
		{
			pServerSet = SERVERSETMNG->GetServerSet( i );
			if( pServerSet )
			{
				if( !NETWORK->ConnectToServer( i, pServerSet->m_sMASIP, pServerSet->m_nMASPort ) )
				{
					pServerSet->Release();
				}
				else
				{
					pServerSet->m_dwMsgCheckTime = tick;

					m_cbServerSet.AddString( pServerSet->m_sServerSetName );
					m_cbCmdServerSet.AddString( pServerSet->m_sServerSetName );

					++count;
				}
			}
		}

		m_cbServerSet.SetCurSel( 0 );
		m_cbCmdServerSet.SetCurSel( 0 );
//------------------------------------------------------------------------------------

		if( count )
		{				
			EnableServerControl( FALSE );
			EnableServerMonitor( FALSE );
			EnableCommand( FALSE );
			EnableToolFromLevel( m_nToolLevel );

			UpdateServerControl();
			OnSelchangeComboServerset();	// UpdateServerMonitor()

			SERVERSETMNG->DoQueryClientExeVersion();
			SERVERSETMNG->DoQueryMaxUser();		
			SERVERSETMNG->DoQueryUserLevel();
			SERVERSETMNG->DoQueryUserCount();
			SERVERSETMNG->DoServerSetRefresh( "All Server" );
		}
		else
		{
			MessageBox( "Can't Connect To MAS!!", "Error!!", MB_OK );
			GetParentFrame()->PostMessage( WM_CLOSE );
		}
	}

	UpdateData( FALSE );
}

// Command ----------------------------------------------------------------------------------------
void CServerToolView::OnButtonNotify() 
{
	if( m_bNotify )
	{
		if( MessageBox( "Stop To Send Notification Message?", "Confirm!!", MB_OKCANCEL ) == IDOK )
		{
			m_btnNotify.SetBitmap( m_bmpGray );			
		}
		else
			return;
	}
	else
	{
		if( m_strCurFileName == "" || m_strCurFileName == "None" )
		{
			MessageBox( "First, You Have To Edit Notification File!!", "Error!!" );
			return;
		}
		else
		{
			if( MessageBox( "Send Notification Message?", "Confirm!!", MB_OKCANCEL ) == IDOK )
			{
				m_btnNotify.SetBitmap( m_bmpGreen );

				CString str;
				m_editRepeatNum.GetWindowText( str );
				m_nRepeatNum = atoi( (LPCTSTR)str );
				m_RepeatEdit.GetWindowText( str );
				m_nRepeatTime = atoi( (LPCTSTR)str );
				m_nCheckTime = m_nRepeatTime;

				m_NotifyMessage.SetRepeatNum( m_nRepeatNum );
			}
			else
				return;
		}
	}

	m_bNotify = !m_bNotify;
}

void CServerToolView::OnButtonNotifyEdit() 
{
/*	m_NotifyDlg.Init( m_strCurFileName, &m_NotifyMessage );
	if( m_NotifyDlg.DoModal() == IDOK )
	{
		m_strCurFileName = m_NotifyDlg.m_strCurFileName;

		if( m_strCurFileName != "None" && m_strCurFileName != "" )
		{
			m_NotifyMessage.Open( (char*)((LPCTSTR)m_strCurFileName) );

			m_editRepeatNum.EnableWindow( TRUE );
			m_RepeatEdit.EnableWindow( TRUE );
		}

		UpdateData( FALSE );
	}
*/
	CFileDialog dlg( TRUE, NULL, "*.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
					 "txt 파일 (*.txt)|*.txt||", NULL );

	if( dlg.DoModal() == IDOK )
	{
		m_strCurFileName = dlg.GetFileName();
		m_NotifyMessage.Open( (char*)(LPCTSTR)m_strCurFileName );

		m_editRepeatNum.EnableWindow( TRUE );
		m_RepeatEdit.EnableWindow( TRUE );
	}
}

void CServerToolView::OnChangeEditMessage() 
{
	if( m_MessageEdit.m_bReturn )
	{
		CString str, temp, msg, setname;
		m_MessageEdit.GetWindowText( str );
		
		int length = 0;
		if( length = str.GetLength() )
		{			
			
			temp.Format( "Send Notification Message?\n\n\"%s\"", str );
			if( MessageBox( temp, "Message", MB_OKCANCEL ) == IDOK )
			{
				m_cbCmdServerSet.GetLBText( m_cbCmdServerSet.GetCurSel(), setname );
				
				LOG( "Send Notice : \"%s\"", (char*)(LPCTSTR)str );
				
				msg = "";
				msg += str;			
				SERVERSETMNG->DoSendMessage( (char*)(LPCTSTR)setname, (char*)((LPCTSTR)msg) );
				
				m_MessageEdit.SetWindowText( "" );			
			}
		}
		
		m_MessageEdit.m_bReturn = FALSE;
	}
}

void CServerToolView::OnButtonCommandSend() 
{
}

// Server Monitor ---------------------------------------------------------------------------------
void CServerToolView::OnDblClickServerList(CListCtrl* pListCtrl,WORD ServerKind)
{	
	POSITION pos = pListCtrl->GetFirstSelectedItemPosition();

	if( pos )
	{
		int nItem = pListCtrl->GetNextSelectedItem( pos );
		char servernum[256];
		pListCtrl->GetItemText( nItem, 3, servernum, 256 );

		CString str, temp;
		m_cbServerSet.GetWindowText( str );		// serverset name

		CServerSet* pServerSet = SERVERSETMNG->GetServerSet( (char*)(LPCTSTR)str );
		if( pServerSet )
		{
			CServer* pServer = pServerSet->GetServer( ServerKind, atoi( servernum ) );
			if( pServer )
			{
				switch( pServer->m_nState )
				{
				case eServerState_On:
					{
						temp.Format( "Turn Off %s's %sServer?", str, pServer->m_sName );
						if( MessageBox( temp, "Turn Off", MB_OKCANCEL ) == IDOK )
							pServer->DoServerOff();
					}
					break;

				case eServerState_Off:
					{
						temp.Format( "Turn On %s's %sServer?", str, pServer->m_sName );
						if( MessageBox( temp, "Turn On", MB_OKCANCEL ) == IDOK )
							pServer->DoServerOn();
					}
					break;

				case eServerState_AbNormal:
					{
						temp.Format( "Turn Off %s's %sServer?", str, pServer->m_sName );
						if( MessageBox( temp, "Turn Off", MB_OKCANCEL ) == IDOK )
							pServer->DoKillServer();
//							pServer->DoServerOff();
					}
					break;
				}
			}
		}
	}
}
void CServerToolView::OnDblclkListcontrolDist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnDblClickServerList(&m_lcDist,DISTRIBUTE_SERVER);

	*pResult = 0;
}

void CServerToolView::OnDblclkListcontrolAgent(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnDblClickServerList(&m_lcAgent,AGENT_SERVER);

	*pResult = 0;
}

void CServerToolView::OnDblclkListcontrolMurim(NMHDR* pNMHDR, LRESULT* pResult) 
{
//	OnDblClickServerList(&m_lcMurim,MURIM_SERVER);
	
	*pResult = 0;
}

void CServerToolView::OnDblclkListcontrolMap(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnDblClickServerList(&m_lcMap,MAP_SERVER);

	*pResult = 0;
}

void CServerToolView::OnRdblClickServerList(CListCtrl* pListCtrl,WORD ServerKind)
{	
	int count = pListCtrl->GetSelectedCount();
	int nItem = -1;
	char servernum[256];
	CServer* pServer;

	if( count > 1 )
	{
		if( MessageBox( "Change Server's State Which Is Selected?", "On/Off", MB_OKCANCEL ) == IDOK )
		{
			CString str;
			m_cbServerSet.GetWindowText( str );
			CServerSet* pServerSet = SERVERSETMNG->GetServerSet( (char*)(LPCTSTR)str );
			if( pServerSet )
			{
				for( int i = 0; i < count; ++i )
				{
					nItem = pListCtrl->GetNextItem( nItem, LVNI_SELECTED );
					pListCtrl->GetItemText( nItem, 3, servernum, 256 );

					pServer = pServerSet->GetServer( ServerKind, atoi( servernum ) );
					if( pServer->m_nState == eServerState_Off )
						pServer->DoServerOn();
					else if( pServer->m_nState == eServerState_On )	// On
						pServer->DoServerOff();
					else if( pServer->m_nState == eServerState_AbNormal )
						pServer->DoKillServer();
				}
			}
		}
	}
}

void CServerToolView::OnRdblclkListcontrolDist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnRdblClickServerList(&m_lcDist,DISTRIBUTE_SERVER);

	*pResult = 0;
}

void CServerToolView::OnRdblclkListcontrolAgent(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnRdblClickServerList(&m_lcAgent,AGENT_SERVER);
	
	*pResult = 0;
}

void CServerToolView::OnRdblclkListcontrolMurim(NMHDR* pNMHDR, LRESULT* pResult) 
{
//	OnRdblClickServerList(&m_lcMurim,MURIM_SERVER);
	
	*pResult = 0;
}

void CServerToolView::OnRdblclkListcontrolMap(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnRdblClickServerList(&m_lcMap,MAP_SERVER);
	
	*pResult = 0;
}

// network ----------------------------------------------------------------------------------------
void CServerToolView::NetworkMsgParse( int ServerSet, BYTE Category, BYTE Protocol, void* pMsg )
{
	switch( Category ) 
	{
	case MP_RMTOOL_CONNECT:
		{
#ifdef _CHINA
			--m_LoginDlg.m_nReceiveCount;
#endif
			switch( Protocol )
			{
			case MP_RMTOOL_CONNECT_ACK:
				{
					TMSG_OPERATOR_LOGIN* msg = (TMSG_OPERATOR_LOGIN*)pMsg;

#ifdef _CHINA
					if( m_LoginDlg.m_nReceiveCount > 0 )
						return;
#endif
					if( msg->sOper.wOperPower == eOperPower_Master ||
						msg->sOper.wOperPower == eOperPower_Monitor )
					{
						m_LoginDlg.m_bConnect = TRUE;
						m_LoginDlg.EndDialog(IDOK);

						EnableToolFromLevel( msg->sOper.wOperPower );
						
						SERVERSETMNG->DoServerSetRefresh( "All Server" );
					}
					else
					{
						m_LoginDlg.LoginError( 99 );
						return;
					}
				}
				break;

			case MP_RMTOOL_CONNECT_NACK:
				{
					TMSG_WORD* msg = (TMSG_WORD*)pMsg;
					m_LoginDlg.LoginError( msg->wData );
					return;
				}
				break;
				
			case MP_CHECKIP_ERROR:
				{
					if( MessageBox( "Tool Is Off For A Wrong IP Address", "Error!!", MB_OK ) == IDOK )
					{
						GetParentFrame()->PostMessage( WM_CLOSE );
						return;
					}
				}
				break;
			}
		}
		break;

	case MP_MORNITORTOOL:
		{
#ifdef _CHINA
			if( m_nToolLevel == eOperPower_Master || m_nToolLevel == eOperPower_Monitor )
			{
			}
			else
				return;
#endif
			switch( Protocol )
			{
			case MP_MORNITORTOOL_MAPSERVERSTATE_ACK:		// all state
				{
					MSGMONITORTOOLALLSTATE* pmsg = (MSGMONITORTOOLALLSTATE*)pMsg;
					SERVERSTATE* pState = NULL;

					for( int i = 0; i < pmsg->num; ++i )
					{
						pState = &pmsg->serverState[i];
						SetServerState( ServerSet, pState );
					}					
				}
				break;

			case MP_MORNITORTOOL_MAPSERVERSTATE_NOTIFY:		// one state
				{
					MSGSERVERSTATE* pmsg = (MSGSERVERSTATE*)pMsg;
					SetServerState( ServerSet, &pmsg->serverState );
				}
				break;			

			case MP_MORNITORTOOL_DISCONNECT_ALLUSER_ACK:
				{
					MSGSERVERSTATE* pmsg = (MSGSERVERSTATE*)pMsg;
					NetworkAllUserDisconnect( ServerSet, &pmsg->serverState );
				}
				break;
				
			case MP_MORNITORTOOL_QUERY_VERSION_ACK:
				{
					MSGNOTICE* pmsg = (MSGNOTICE*)pMsg;
					NetworkQueryVersion( ServerSet, pmsg->Msg );
					UpdateServerControl();
				}
				break;
			case MP_MORNITORTOOL_CHANGE_VERSION_ACK:
				{
					MSGNOTICE* pmsg = (MSGNOTICE*)pMsg;
					NetworkChangeVersion( ServerSet, pmsg->Msg );	
					UpdateServerControl();
				}
				break;
			case MP_MORNITORTOOL_QUERY_MAXUSER_ACK:
				{
					MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
					NetworkQueryMaxUser( ServerSet, pmsg->dwData );					
					UpdateServerControl();
				}
				break;
			case MP_MORNITORTOOL_CHANGE_MAXUSER_ACK:
				{
					MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
					NetworkChangeMaxUser( ServerSet, pmsg->dwData );					
					UpdateServerControl();
				}
				break;
			case MP_MORNITORTOOL_CHANGE_USERLEVEL_ACK:
				{
					MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;	
					NetworkUserLevel( ServerSet, pmsg->dwData );
					UpdateServerControl();
				}
				break;
				
			case MP_MORNITORTOOL_MSSTATEALL_ACK:
				{
					if( m_pMSInfoDlg )
					if( m_pMSInfoDlg->m_bInit )
					{
						MSGMSSTATEALL* pmsg = (MSGMSSTATEALL*)pMsg;
						m_pMSInfoDlg->SetMSStateAll( ServerSet, pmsg );
					}
				}
				break;

			case MP_MORNITORTOOL_CHECK_AUTOPATCH_ACK:
				{
					if( m_pMSInfoDlg )
					if( m_pMSInfoDlg->m_bInit )
					{
						MSGMSSTATE* pmsg = (MSGMSSTATE*)pMsg;
						m_pMSInfoDlg->SetAutoPatchState( ServerSet, &pmsg->MSState );		
					}
				}
				break;

			case MP_MORNITORTOOL_QUERY_AUTOPATCHVERSION_ACK:
				{
					if( m_pMSInfoDlg )
					if( m_pMSInfoDlg->m_bInit )
					{
						MSGMSSTATE* pmsg = (MSGMSSTATE*)pMsg;
						m_pMSInfoDlg->SetAutoPatchVersion( ServerSet, &pmsg->MSState );		
					}
				}
				break;
				
			case MP_MORNITORTOOL_QUERY_CHANNELINFO_ACK:
				{
					if( m_pUserCountDlg )
					if( m_pUserCountDlg->m_bInit )
					{
						MSG_CHANNEL_INFO_MORNITOR* pmsg = (MSG_CHANNEL_INFO_MORNITOR*)pMsg;
						m_pUserCountDlg->SetChannelData( ServerSet, pmsg );
					}
				}
				break;
			}

//			UpdateServerControl();
			UpdateServerMonitor();			
		}
		break;
	}
}

//-------------------------------------------------------------------------------------------------
void CServerToolView::SetServerState( int setindex, SERVERSTATE* pstate )
{
	int flag = -1;
	// 서버 정보
	CServerSet* pServerSet = SERVERSETMNG->GetServerSet( setindex );
	if( pServerSet )
	{
		CServer* pServer = pServerSet->GetServer( pstate );
		if( pServer )
		{
#ifdef _CHINA
			if( m_nToolLevel == eOperPower_Master )
				pServer->m_dwUserCount = pstate->UserCount;
			else
				pServer->m_dwUserCount = 0;
#endif
			flag = pServer->SetState( pstate->ServerState );			
		}

		pServerSet->m_dwMsgCheckTime = GetTickCount();
	}
	if( flag != -1 )
	{
		m_cbServerSet.SetCurSel( flag + 1);
		OnSelchangeComboServerset();
	}
}

void CServerToolView::NetworkQueryVersion( int setindex, char* version )
{
	CServerSet* pServerSet = SERVERSETMNG->GetServerSet( setindex );

	if( pServerSet )
	{
		if( strcmp( pServerSet->m_sClientExeVersion, version ) != 0 )
		{
			strcpy( pServerSet->m_sClientExeVersion, version );
			pServerSet->WriteClientExeVersion( version );

			char temp[256];
			wsprintf( temp, "%s's Version Is %s.", pServerSet->m_sServerSetName, pServerSet->m_sClientExeVersion );
			if( gMsgOption.bExeVer )
				MessageBox( temp, "Version", MB_OK );
		}

		UpdateData( FALSE );

		pServerSet->m_dwMsgCheckTime = GetTickCount();
	}
}

void CServerToolView::NetworkChangeVersion( int setindex, char* version )
{
	CServerSet* pServerSet = SERVERSETMNG->GetServerSet( setindex );
	if( pServerSet )
	{
		strcpy( pServerSet->m_sClientExeVersion, version );
		pServerSet->WriteClientExeVersion( version );

		char temp[256];
		wsprintf( temp, "%s's Version Is %s.", pServerSet->m_sServerSetName, pServerSet->m_sClientExeVersion );
		if( gMsgOption.bExeVer )
			MessageBox( temp, "Version", MB_OK );

		UpdateData( FALSE );

		pServerSet->m_dwMsgCheckTime = GetTickCount();
	}
}

void CServerToolView::NetworkQueryMaxUser( int setindex, DWORD maxuser )
{
	CServerSet* pServerSet = SERVERSETMNG->GetServerSet( setindex );

	if( pServerSet )
	{
		if( pServerSet->m_dwMaxUser != maxuser )
		{
			pServerSet->m_dwMaxUser = maxuser;

			char temp[256];
			wsprintf( temp, "%s's MaxUser Is %d.", pServerSet->m_sServerSetName, pServerSet->m_dwMaxUser );
			if( gMsgOption.bMaxUser )
				MessageBox( temp, "MaxUser", MB_OK );
		
			UpdateData( FALSE );
		}

		pServerSet->m_dwMsgCheckTime = GetTickCount();
	}
}

void CServerToolView::NetworkChangeMaxUser( int setindex, DWORD maxuser )
{
	CServerSet* pServerSet = SERVERSETMNG->GetServerSet( setindex );
	if( pServerSet )
	{
		pServerSet->m_dwMaxUser = maxuser;

		char temp[256];
		wsprintf( temp, "%s's MaxUser Is %d.", pServerSet->m_sServerSetName, pServerSet->m_dwMaxUser );
		if( gMsgOption.bMaxUser )
			MessageBox( temp, "MaxUser", MB_OK );

		UpdateData( FALSE );

		pServerSet->m_dwMsgCheckTime = GetTickCount();
	}
}

void CServerToolView::NetworkUserLevel( int setindex, DWORD userlevel )
{
	CServerSet* pServerSet = SERVERSETMNG->GetServerSet( setindex );
	if( pServerSet )
	{
		if( pServerSet->m_nUserLevel != (int)userlevel )
		{
			pServerSet->SetUserLevel( userlevel );

			char temp[256];
			wsprintf( temp, "%s's UserLevel Is %s.", pServerSet->m_sServerSetName, pServerSet->m_sUserLevel );
			if( gMsgOption.bUserLevel )
				MessageBox( temp, "UserLevel", MB_OK );
		}
		if( userlevel != 0 )
			m_cbUserLevel.SetCurSel( userlevel-1 );

		UpdateData( FALSE );

		pServerSet->m_dwMsgCheckTime = GetTickCount();
	}
}

void CServerToolView::NetworkAllUserDisconnect( int setindex, SERVERSTATE* pstate )
{
	// 서버 정보
	CServerSet* pServerSet = SERVERSETMNG->GetServerSet( setindex );
	if( pServerSet )
	{
		CServer* pServer = pServerSet->GetServer( pstate );
		if( pServer )
			pServer->m_bDisconnectUser = FALSE;
	}
}

void CServerToolView::Disconnect( int serverset )
{
	m_bReconnect = TRUE;
	++m_dwDisconNum;
}

// menu -------------------------------------------------------------------------------------------
void CServerToolView::OnFileMasOpen() 
{
//	ReConnectToMAS();
}

void CServerToolView::ReadIniFile( char* filename )
{
	m_wRefreshTime = 30;
	m_wWriteCountTime = 30;

	CMHFile file;
	char temp[256];
	strcpy( temp, GetDataDirectory() );
	strcat( temp, filename );

	if( !file.Init( temp, "rt", MHFILE_FLAG_DONOTDISPLAY_NOTFOUNDERROR ) )
		return;

	while( !file.IsEOF() )
	{
		strcpy( temp, strupr(file.GetString()) );

		if( temp[0] == '/' ) 
		{
			file.GetLine( temp, 256 );
			continue;
		}
		
		if( strcmp( temp, "*REFRESHTIME" ) == 0 )
		{
			m_wRefreshTime = file.GetWord();
		}
		else if( strcmp( temp, "*WRITETIME" ) == 0 )
		{
			m_wWriteCountTime = file.GetWord();
		}
	}
	file.Release();
}

void CServerToolView::OnClickListcontrolDist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnClickServerList(&m_lcDist,DISTRIBUTE_SERVER);

	*pResult = 0;
}

void CServerToolView::OnClickListcontrolAgent(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnClickServerList(&m_lcAgent,AGENT_SERVER);
	
	*pResult = 0;
}

void CServerToolView::OnClickListcontrolMap(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnClickServerList(&m_lcMap,MAP_SERVER);
	
	*pResult = 0;
}

void CServerToolView::OnClickServerList(CListCtrl* pListCtrl,WORD ServerKind)
{
	POSITION pos = pListCtrl->GetFirstSelectedItemPosition();

	if( pos )
	{
		m_nKill = ServerKind;
		m_btnTServer.EnableWindow( TRUE );
	}
	else
	{
		m_nKill = 0;
		m_btnTServer.EnableWindow( FALSE );
	}
}

void CServerToolView::OnButtonTerminateserver() 
{
	if( m_nKill == 0 )
		return;

	CListCtrl* pListCtrl = NULL;
	POSITION pos = NULL;
	int nServerKind = 0;
	switch( m_nKill )
	{
	case DISTRIBUTE_SERVER:
		{
			pos = m_lcDist.GetFirstSelectedItemPosition();
			pListCtrl = &m_lcDist;
			nServerKind = DISTRIBUTE_SERVER;
		}
		break;
	case AGENT_SERVER:
		{
			pos = m_lcAgent.GetFirstSelectedItemPosition();
			pListCtrl = &m_lcAgent;
			nServerKind = AGENT_SERVER;
		}
		break;
	case MAP_SERVER:
		{
			pos = m_lcMap.GetFirstSelectedItemPosition();
			pListCtrl = &m_lcMap;
			nServerKind = MAP_SERVER;
		}
		break;
	}

	if( pos )
	{
		int nItem = pListCtrl->GetNextSelectedItem( pos );
		char servernum[256];
		pListCtrl->GetItemText( nItem, 3, servernum, 256 );

		CString str, temp;
		m_cbServerSet.GetWindowText( str );		// serverset name

		CServerSet* pServerSet = SERVERSETMNG->GetServerSet( (char*)(LPCTSTR)str );
		if( pServerSet )
		{
			CServer* pServer = pServerSet->GetServer( nServerKind, atoi( servernum ) );
			if( pServer )
			{
				temp.Format( "Terminate %s's %sServer?", str, pServer->m_sName );
				if( MessageBox( temp, "Turn Off", MB_OKCANCEL ) == IDOK )
					pServer->DoKillServer();
			}
		}
	}
}

void CServerToolView::OnButtonOption() 
{
	m_MsgBoxOption.DoModal();		
}
