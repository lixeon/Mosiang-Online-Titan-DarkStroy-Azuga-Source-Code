// MSInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "servertool.h"
#include "MSInfoDlg.h"
#include "ServerSetMng.h"
#include "MainFrm.h"
#include "ServerToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMSInfoDlg dialog


CMSInfoDlg::CMSInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMSInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMSInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bInit = FALSE;

		// listctrl
	m_StateImageList.Create( IDB_BITMAP_STATE, 16, 1, RGB(255,255,255) );
	m_bmpGray.LoadBitmap( IDB_BITMAP_GRAY );
	m_bmpGreen.LoadBitmap( IDB_BITMAP_GREEN );
}


void CMSInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMSInfoDlg)
	DDX_Control(pDX, IDC_BUTTON_AUTOPATCH, m_btnAutoPatch);
	DDX_Control(pDX, IDC_BUTTON_REFRESH, m_btnRefresh);
	DDX_Control(pDX, IDC_COMBO_SERVERSET, m_cbServerSet);
	DDX_Control(pDX, IDC_LIST_MSINFO, m_lcMSInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMSInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CMSInfoDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_SERVERSET, OnSelchangeComboServerset)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_AUTOPATCH, OnButtonAutopatch)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMSInfoDlg message handlers

BOOL CMSInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Font.CreatePointFont(100, "Arial");
	m_lcMSInfo.SetFont( &m_Font );

	// timer 설정( 1초마다 )
	SetTimer( 0, 1000, NULL );
	
#ifndef _FORGM
	m_btnRefresh.EnableWindow( TRUE );
	m_btnAutoPatch.EnableWindow( TRUE );
#endif
	
	InitData();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMSInfoDlg::InitData()
{
	// combobox
	m_cbServerSet.ResetContent();
	m_cbServerSet.AddString( "All Server" );
	CServerSet* pServerSet = NULL;
	for( int i = 0; i < SERVERSETMNG->GetMaxServerSet(); ++i )
	{
		pServerSet = SERVERSETMNG->GetServerSet( i );
		if( pServerSet->m_bInit )
		{
			m_cbServerSet.AddString( pServerSet->m_sServerSetName );
		}
	}
	m_cbServerSet.SetCurSel( 0 );

	char* tcolumn[7] = { "State", "ServerSet", "ServerName", "IP", "Port", "AutoPatch", "PatchVersion" };
	int tcolumnsize[7] = { 90, 80, 80, 100, 50, 80, 120 };
	
	// listctrl column 설정
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_CENTER;
	for( i = 0; i < 7; ++i )
	{
		lvcolumn.iSubItem = i;
		lvcolumn.cx = tcolumnsize[i];
		lvcolumn.pszText = tcolumn[i];
		m_lcMSInfo.InsertColumn( i, &lvcolumn );
	}
	// style
	m_lcMSInfo.SetExtendedStyle( LVS_EX_FULLROWSELECT );
	// state image
	m_lcMSInfo.SetImageList( &m_StateImageList, LVSIL_STATE );

	UpdateListCtrl();

	SERVERSETMNG->DoMSStateAll( "All Server" );	
	SERVERSETMNG->DoQueryAutoPatchVersion( "All Server" );	

	m_bInit = TRUE;
}

void CMSInfoDlg::OnSelchangeComboServerset() 
{
	UpdateListCtrl();
}

void CMSInfoDlg::UpdateListCtrl()
{
	CString str;
	LV_ITEM lvitem;
	CServerSet* pServerSet = NULL;
	CServer* pServer = NULL;
	char temp[256];

	m_lcMSInfo.DeleteAllItems();

	int index = m_cbServerSet.GetCurSel();
	if( index == 0 )	// AllServer
	{
		for( int i = 0; i < SERVERSETMNG->GetMaxServerSet(); ++i )
		{
			pServerSet = SERVERSETMNG->GetServerSet( i );
			if( !pServerSet->m_bInit )
				continue;

			vector<CServer*>::iterator iter = pServerSet->m_vecMSServer.begin();
			for(;iter != pServerSet->m_vecMSServer.end();++iter)
			{
				pServer = (*iter);

				if( pServer->m_nServerKind == MONITOR_SERVER )
				{
					lvitem.iItem = m_lcMSInfo.GetItemCount();	// 행
					
					lvitem.iSubItem = 0;
					lvitem.mask = LVIF_TEXT | LVIF_STATE;
					lvitem.stateMask = LVIS_STATEIMAGEMASK;
					lvitem.state = INDEXTOSTATEIMAGEMASK( pServer->m_nState );
					lvitem.pszText = pServer->m_sState;
					
					m_lcMSInfo.InsertItem( &lvitem );
					m_lcMSInfo.SetItemText( lvitem.iItem, 1, pServerSet->m_sServerSetName );
					m_lcMSInfo.SetItemText( lvitem.iItem, 2, pServer->m_sName );
					m_lcMSInfo.SetItemText( lvitem.iItem, 3, pServer->m_sIP );
					m_lcMSInfo.SetItemText( lvitem.iItem, 4, itoa( pServer->m_nPort, temp, 10 ) );
					m_lcMSInfo.SetItemText( lvitem.iItem, 5, pServer->m_sAutoPatchState );
					m_lcMSInfo.SetItemText( lvitem.iItem, 6, pServer->m_sAutoPatchVersion );
				}
			}
		}
	}
	else
	{
		m_cbServerSet.GetLBText( m_cbServerSet.GetCurSel(), str );
		pServerSet = SERVERSETMNG->GetServerSet( (char*)(LPCTSTR)str );
		if( !pServerSet )	
			return;
		
		vector<CServer*>::iterator iter = pServerSet->m_vecMSServer.begin();
		for(;iter != pServerSet->m_vecMSServer.end();++iter)
		{
			pServer = (*iter);
			
			if( pServer->m_nServerKind == MONITOR_SERVER )
			{
				lvitem.iItem = m_lcMSInfo.GetItemCount();	// 행
				
				lvitem.iSubItem = 0;
				lvitem.mask = LVIF_TEXT | LVIF_STATE;
				lvitem.stateMask = LVIS_STATEIMAGEMASK;
				lvitem.state = INDEXTOSTATEIMAGEMASK( pServer->m_nState );
				lvitem.pszText = pServer->m_sState;
				
				m_lcMSInfo.InsertItem( &lvitem );
				m_lcMSInfo.SetItemText( lvitem.iItem, 1, pServerSet->m_sServerSetName );
				m_lcMSInfo.SetItemText( lvitem.iItem, 2, pServer->m_sName );
				m_lcMSInfo.SetItemText( lvitem.iItem, 3, pServer->m_sIP );
				m_lcMSInfo.SetItemText( lvitem.iItem, 4, itoa( pServer->m_nPort, temp, 10 ) );
				m_lcMSInfo.SetItemText( lvitem.iItem, 5, pServer->m_sAutoPatchState );
				m_lcMSInfo.SetItemText( lvitem.iItem, 6, pServer->m_sAutoPatchVersion );
			}
		}
	}
}

void CMSInfoDlg::OnTimer(UINT nIDEvent) 
{
	SYSTEMTIME time;
	GetLocalTime( &time );

	if( m_bInit )
	{
		if( (time.wSecond%60) == 0 )
		{
			SERVERSETMNG->DoMSStateAll( "All Server" );
		}		
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CMSInfoDlg::DestroyWindow() 
{
	m_bInit = FALSE;
	KillTimer( 0 );
	
	return CDialog::DestroyWindow();
}

void CMSInfoDlg::SetMSStateAll( int nServerSet, MSGMSSTATEALL* pState )
{
	CServerSet* pServerSet = SERVERSETMNG->GetServerSet( nServerSet );
	if( pServerSet )
	{
		pServerSet->ClearMSStateAll();

		for( int i = 0; i < pState->nNum; ++i )
		{
			CServer* pServer = pServerSet->GetMSServer( &pState->MSState[i] );
			if( pServer )
			{
				pServer->SetState( eServerState_Connect );
			}
		}
	}

	UpdateListCtrl();
}

void CMSInfoDlg::SetAutoPatchState( int nServerSet, MSSTATE* pState )
{
	CServerSet* pServerSet = SERVERSETMNG->GetServerSet( nServerSet );
	if( pServerSet )
	{
		CServer* pServer = pServerSet->GetMSServer( pState );
		if( pServer )
		{
			pServer->SetAutoPatchState( pState->wAutoPatchState, pState->sAutoPatchVersion );
		}
	}

	UpdateListCtrl();
}

void CMSInfoDlg::SetAutoPatchVersion( int nServerSet, MSSTATE* pState )
{
	CServerSet* pServerSet = SERVERSETMNG->GetServerSet( nServerSet );
	if( pServerSet )
	{
		CServer* pServer = pServerSet->GetMSServer( pState );
		if( pServer )
		{
			pServer->SetAutoPatchVersion( pState->sAutoPatchVersion );
		}
	}

	UpdateListCtrl();
}

void CMSInfoDlg::OnButtonRefresh() 
{
	SERVERSETMNG->DoMSStateAll( "All Server" );	
	SERVERSETMNG->DoQueryAutoPatchVersion( "All Server" );
}

void CMSInfoDlg::OnButtonAutopatch() 
{
	char temp[256];	
	CString str;
	m_cbServerSet.GetWindowText( str );

	if( SERVERSETMNG->IsThereOnServer( (char*)(LPCTSTR)str ) )
	{
		wsprintf( temp, "At First, You Must Off Servers!!" );
		if( MessageBox( temp, "AutoPatch", MB_OK ) == IDOK )
			return;
	}
	else
	{
		wsprintf( temp, "Are You Really Execute AutoPatch of %s?", (char*)(LPCTSTR)str );
		if( MessageBox( temp, "AutoPatch", MB_OKCANCEL ) == IDOK )
		{
			SERVERSETMNG->DoAutoPatch( (char*)(LPCTSTR)str );	
			
			LOG( "Button AutoPatch : %s", (char*)(LPCTSTR)str );
		}
	}
}

BOOL CMSInfoDlg::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

void CMSInfoDlg::PostNcDestroy() 
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	((CServerToolView*)pMainFrame->GetActiveView())->m_pMSInfoDlg = NULL;

	delete this;
	
	CDialog::PostNcDestroy();
}

void CMSInfoDlg::OnClose() 
{
	DestroyWindow();
	
//	CDialog::OnClose();
}
