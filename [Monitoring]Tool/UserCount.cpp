// UserCount.cpp : implementation file
//

#include "stdafx.h"
#include "servertool.h"
#include "UserCount.h"
#include "ServerSetMng.h"
#include "MainFrm.h"
#include "ServerToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserCount dialog


CUserCount::CUserCount(CWnd* pParent /*=NULL*/)
	: CDialog(CUserCount::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUserCount)
	//}}AFX_DATA_INIT
	m_bInit = FALSE;
}


void CUserCount::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserCount)
	DDX_Control(pDX, IDC_COMBO_SERVERSET, m_cbServerSet);
	DDX_Control(pDX, IDC_TREE_USERCOUNT, m_tcUserCount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserCount, CDialog)
	//{{AFX_MSG_MAP(CUserCount)
	ON_CBN_SELCHANGE(IDC_COMBO_SERVERSET, OnSelchangeComboServerset)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserCount message handlers

BOOL CUserCount::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_Font.CreatePointFont(100, "Arial");
	m_tcUserCount.SetFont( &m_Font );
	
	// TODO: Add extra initialization here
	InitData();
	
	m_bInit = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUserCount::InitData()
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

	// treecontrol
	UpdateTreeCtrl();

	SERVERSETMNG->DoQueryChannelInfo( "All Server" );
}

HTREEITEM CUserCount::InsertItem( HTREEITEM hRoot, char* name, BOOL bChild )
{
	TV_INSERTSTRUCT tvinsert;
	tvinsert.hParent = hRoot;
	tvinsert.hInsertAfter = TVI_LAST;
	tvinsert.item.pszText = name;

	HTREEITEM hItem;
	
	if( bChild )
	{
		tvinsert.item.mask = TVIF_TEXT | TVIF_CHILDREN;
		hItem = m_tcUserCount.InsertItem( &tvinsert );

		return hItem;
	}
	else
	{
		tvinsert.item.mask = TVIF_TEXT;
		hItem = m_tcUserCount.InsertItem( &tvinsert );
		
		return NULL;
	}
}

BOOL CUserCount::DestroyWindow() 
{
	m_bInit = FALSE;
	
	return CDialog::DestroyWindow();
}

void CUserCount::SetChannelData( int nServerSet, MSG_CHANNEL_INFO_MORNITOR* pChannelInfo )
{
	CServerSet* pServerSet = SERVERSETMNG->GetServerSet( nServerSet );
	if( pServerSet )
	{
		CServer* pServer = pServerSet->GetServer( pChannelInfo->sServerIP, pChannelInfo->wServerPort );
		if( pServer )
		{
			pServer->SetChannelUserCount( &pChannelInfo->ChannelInfo );
			UpdateTreeCtrl();
		}
	}
}

void CUserCount::UpdateTreeCtrl()
{
	m_tcUserCount.DeleteAllItems();
	
	// treecontrol
	CString str;
	CServerSet* pServerSet = NULL;
	CServer* pServer = NULL;
	HTREEITEM hRoot, hRoot1;
	char temp[64];

	int index = m_cbServerSet.GetCurSel();
	if( index == 0 )	// AllServer
	{
		for( int i = 0; i < SERVERSETMNG->GetMaxServerSet(); ++i )
		{
			pServerSet = SERVERSETMNG->GetServerSet( i );
			if( pServerSet->m_bInit )
			{
				hRoot = InsertItem( NULL, pServerSet->m_sServerSetName, TRUE );
				vector<CServer*>::iterator iter = pServerSet->m_vecServer.begin();
				for(;iter != pServerSet->m_vecServer.end();++iter)
				{
					pServer = (*iter);
					if( pServer->m_nServerKind == MAP_SERVER )
					{
						if( pServer->m_nState == eServerState_Off )
							sprintf( temp, "%s : 0", pServer->m_sName );							
						else
							sprintf( temp, "%s : %d", pServer->m_sName, pServer->m_dwUserCount );

						if( pServer->m_nMaxChannel == 0 )
							hRoot1 = InsertItem( hRoot, temp, FALSE );
						else
						{
							hRoot1 = InsertItem( hRoot, temp, TRUE );					
							
							for( int j = 0; j < pServer->m_nMaxChannel; ++j )
							{
								sprintf( temp, "Channel %d : %4d", j+1, pServer->m_nChannelUserCount[j] );
								InsertItem( hRoot1, temp, FALSE );
							}
						}						
					}
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

		if( pServerSet->m_bInit )
		{
			hRoot = InsertItem( NULL, pServerSet->m_sServerSetName, TRUE );
			vector<CServer*>::iterator iter = pServerSet->m_vecServer.begin();
			for(;iter != pServerSet->m_vecServer.end();++iter)
			{
				pServer = (*iter);
				if( pServer->m_nServerKind == MAP_SERVER )
				{
					if( pServer->m_nState == eServerState_Off )
						sprintf( temp, "%s : 0", pServer->m_sName );							
					else
						sprintf( temp, "%s : %d", pServer->m_sName, pServer->m_dwUserCount );
					
					if( pServer->m_nMaxChannel == 0 )
						hRoot1 = InsertItem( hRoot, temp, FALSE );
					else
					{
						hRoot1 = InsertItem( hRoot, temp, TRUE );					
						
						for( int j = 0; j < pServer->m_nMaxChannel; ++j )
						{
							sprintf( temp, "Channel %d : %4d", j+1, pServer->m_nChannelUserCount[j] );
							InsertItem( hRoot1, temp, FALSE );
						}
					}						
				}
			}
		}
	}
}

void CUserCount::OnSelchangeComboServerset() 
{
	UpdateTreeCtrl();	
}

void CUserCount::OnButtonRefresh() 
{
	SERVERSETMNG->DoQueryChannelInfo( "All Server" );	

	UpdateTreeCtrl();
}

LRESULT CUserCount::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DefWindowProc(message, wParam, lParam);
}

BOOL CUserCount::Create(CWnd* pParentWnd) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);
}

void CUserCount::PostNcDestroy() 
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	((CServerToolView*)pMainFrame->GetActiveView())->m_pUserCountDlg = NULL;

	delete this;
	
	CDialog::PostNcDestroy();
}

void CUserCount::OnClose() 
{
	DestroyWindow();
}
