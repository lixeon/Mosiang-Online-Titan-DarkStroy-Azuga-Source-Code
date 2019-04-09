// LogEtcPage.cpp : implementation file
//

#include "stdafx.h"
#include "ds_rmtool.h"
#include "LogEtcPage.h"
#include "RMNetwork.h"
#include "LogDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogEtcPage property page

IMPLEMENT_DYNCREATE(CLogEtcPage, CLDPropPage)

CLogEtcPage::CLogEtcPage() : CLDPropPage(CLogEtcPage::IDD)
{
	//{{AFX_DATA_INIT(CLogEtcPage)
	m_bOneDay = FALSE;
	m_nLogKind = -1;
	//}}AFX_DATA_INIT
	m_bStart = FALSE;
	
	m_CheatLogTable.Initialize( 1000 );
	m_HackingLogTable.Initialize( 1000 );
}

CLogEtcPage::~CLogEtcPage()
{
	ReleaseCheatLogTable();
	ReleaseHackingLogTable();
}

void CLogEtcPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogEtcPage)
	DDX_Control(pDX, IDC_DATETIMEPICKER_END, m_EndTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER_START, m_StartTime);
	DDX_Control(pDX, IDC_LISTCTRL_ETCLOG, m_lcEtc);
	DDX_Control(pDX, IDC_EDIT_LOG_CHARNAME, m_etCharName);
	DDX_Control(pDX, IDC_EDIT_CHARIDX, m_etCharIdx);
	DDX_Check(pDX, IDC_CHECK_LOG_ONEDAY, m_bOneDay);
	DDX_Radio(pDX, IDC_RADIO1, m_nLogKind);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogEtcPage, CLDPropPage)
	//{{AFX_MSG_MAP(CLogEtcPage)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_LOG_SEARCH, OnButtonLogSearch)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_BTN_SAVETOFILE, OnBtnSavetofile)
	ON_BN_CLICKED(IDC_CHECK_LOG_ONEDAY, OnCheckLogOneday)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogEtcPage message handlers

int CLogEtcPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CLDPropPage::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

BOOL CLogEtcPage::OnInitDialog() 
{
	CLDPropPage::OnInitDialog();
	
	m_bOneDay = TRUE;
	m_nLogKind = 0;	
	m_dwCharIdx = 0;

//	m_Font.CreatePointFont(100, "Arial");
//	m_lcEtc.SetFont( &m_Font );
	
	UpdateData( FALSE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLogEtcPage::SetCheatLog( TMSG_CHEATLOG* pMsg )
{
	for( int i = 0; i < pMsg->wCount; ++i )
	{
		CHEATLOG* pData = new CHEATLOG;
		memcpy( pData, &pMsg->sLog[i], sizeof(CHEATLOG) );

		m_CheatLogTable.Add( pData, pData->dwLogIdx );
	}

	InitListControl( eEtcLogKind_Cheat );
	UpdateListControl( eEtcLogKind_Cheat );
}

void CLogEtcPage::SetHackingLog( TMSG_HACKINGLOG* pMsg )
{
	for( int i = 0; i < pMsg->wCount; ++i )
	{
		HACKINGLOG* pData = new HACKINGLOG;
		memcpy( pData, &pMsg->sLog[i], sizeof(HACKINGLOG) );

		m_HackingLogTable.Add( pData, pData->dwLogIdx );
	}

	InitListControl( eEtcLogKind_Hacking );
	UpdateListControl( eEtcLogKind_Hacking );
}

void CLogEtcPage::OnButtonLogSearch() 
{
	CString sName;

	if( m_etCharName.GetWindowTextLength() == 0 )
	{
		SetCharacterIdx( 0 );
	}
	else
	{
		m_etCharName.GetWindowText( sName );
	
		// Search
		TMSG_CHECKNAME Msg;
		Msg.Category = MP_RMTOOL_GAMELOG;
		Msg.Protocol = MP_RMTOOL_QUERY_CHARIDX_SYN;
		Msg.dwTemplateIdx = m_pDocument->GetDocumentIdx();
		strcpy( Msg.sName, (char*)(LPCTSTR)sName );

		NETWORK->Send( &Msg, sizeof(TMSG_CHECKNAME) );

		m_pDocument->m_dwPage = 2;
	}
}

void CLogEtcPage::SetCharacterIdx( DWORD dwCharIdx )
{
	char temp[128] = {0, };
	sprintf( temp, "%d", dwCharIdx );
	m_etCharIdx.SetWindowText( temp );
	m_etCharIdx.EnableWindow( TRUE );
	m_dwCharIdx = dwCharIdx;

	CString sName, sSDate, sEDate;
	// Date
	if( m_bOneDay )
	{
		m_StartTime.GetWindowText( sSDate );
		sEDate.Empty();		
	}
	else
	{
		m_StartTime.GetWindowText( sSDate );
		m_EndTime.GetWindowText( sEDate );
	}
	
	// Search
	TMSG_GAMELOGSEARCH GameLogSearch;
	GameLogSearch.Category = MP_RMTOOL_GAMELOG;

	switch( m_nLogKind )
	{
	case eEtcLogKind_Cheat:
		{
			GameLogSearch.Protocol = MP_RMTOOL_CHEATLOG_SYN;
			ReleaseCheatLogTable();
		}
		break;
	case eEtcLogKind_Hacking:
		{
			GameLogSearch.Protocol = MP_RMTOOL_HACKINGLOG_SYN;
			ReleaseHackingLogTable();
		}
		break;
	}

	GameLogSearch.dwTemplateIdx = m_pDocument->GetDocumentIdx();
	GameLogSearch.wServer = 0;
	GameLogSearch.wType = m_nLogKind;
//	strcpy( GameLogSearch.str, (char*)(LPCTSTR)sName );
	GameLogSearch.dwItem = m_dwCharIdx;
	GameLogSearch.bDayOnly = m_bOneDay;
	strcpy( GameLogSearch.sSDate, (char*)(LPCTSTR)sSDate );
	strcpy( GameLogSearch.sEDate, (char*)(LPCTSTR)sEDate );

	NETWORK->Send( &GameLogSearch, sizeof(TMSG_GAMELOGSEARCH) );

	m_pDocument->StartDlg();
}

void CLogEtcPage::OnRadio1() 
{
	m_nLogKind = eEtcLogKind_Cheat;
}

void CLogEtcPage::OnRadio2() 
{
	m_nLogKind = eEtcLogKind_Hacking;
}

void CLogEtcPage::ReleaseCheatLogTable()
{
	CHEATLOG* pData = NULL;
	m_CheatLogTable.SetPositionHead();
	while( pData = m_CheatLogTable.GetData() )
		delete pData;
	m_CheatLogTable.RemoveAll();	
}

void CLogEtcPage::ReleaseHackingLogTable()
{
	HACKINGLOG* pData = NULL;
	m_HackingLogTable.SetPositionHead();
	while( pData = m_HackingLogTable.GetData() )
		delete pData;
	m_HackingLogTable.RemoveAll();
}

void CLogEtcPage::InitListControl( DWORD dwKind )
{
	// Delete all of the columns.
	int i = 0;
	int nColumnCount = m_lcEtc.GetHeaderCtrl()->GetItemCount();
	for( i = 0; i < nColumnCount; ++i )
		m_lcEtc.DeleteColumn( 0 );
	m_lcEtc.DeleteAllItems();

	switch( dwKind )
	{
	case eEtcLogKind_Cheat:
		{
			char* tcolumn[7] = { "LogIdx", "CharIdx", "Type", "Parameter", "ItemDBIdx", "ItemIdx", "LogDate" };
			int tcolumnsize[7] = { 80, 80, 50, 50, 80, 80, 120 };

			// listctrl column 설정
			LV_COLUMN lvcolumn;
			lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvcolumn.fmt = LVCFMT_CENTER;
			for( i = 0; i < 7; ++i )
			{
				lvcolumn.iSubItem = i;
				lvcolumn.cx = tcolumnsize[i];
				lvcolumn.pszText = tcolumn[i];
				m_lcEtc.InsertColumn( i, &lvcolumn );
			}
		}
		break;
	case eEtcLogKind_Hacking:
		{
			char* tcolumn[4] = { "LogIdx", "CharIdx", "Kind", "LogDate" };
			int tcolumnsize[4] = { 80, 80, 50, 120 };

			// listctrl column 설정
			LV_COLUMN lvcolumn;
			lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvcolumn.fmt = LVCFMT_CENTER;
			for( i = 0; i < 4; ++i )
			{
				lvcolumn.iSubItem = i;
				lvcolumn.cx = tcolumnsize[i];
				lvcolumn.pszText = tcolumn[i];
				m_lcEtc.InsertColumn( i, &lvcolumn );
			}
		}
		break;
	}
	// style
	DWORD dwExStyle = m_lcEtc.GetExtendedStyle();
	dwExStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_lcEtc.SetExtendedStyle( dwExStyle );
}

void CLogEtcPage::UpdateListControl( DWORD dwKind )
{
	m_lcEtc.DeleteAllItems();

	char temp[256];	
	LV_ITEM lvitem;

	switch( dwKind )
	{
	case eEtcLogKind_Cheat:
		{
			CHEATLOG* pData = NULL;
			m_CheatLogTable.SetPositionHead();
			while( pData = m_CheatLogTable.GetData() )
			{
				lvitem.iItem = m_lcEtc.GetItemCount();	// 행
				
				lvitem.mask = LVIF_TEXT;
				lvitem.stateMask = LVIS_SELECTED;
				lvitem.state = INDEXTOSTATEIMAGEMASK(1);
				lvitem.iSubItem = 0;	
				lvitem.pszText = itoa( pData->dwLogIdx, temp, 10 );
				
				m_lcEtc.InsertItem( &lvitem );
				m_lcEtc.SetItemText( lvitem.iItem, 1, itoa( pData->dwChrIdx, temp, 10 ) );
				m_lcEtc.SetItemText( lvitem.iItem, 2, itoa( pData->dwLogType, temp, 10 ) );
				m_lcEtc.SetItemText( lvitem.iItem, 3, itoa( pData->dwParam, temp, 10 ) );
				m_lcEtc.SetItemText( lvitem.iItem, 4, itoa( pData->dwItemDBIdx, temp, 10 ) );
				m_lcEtc.SetItemText( lvitem.iItem, 5, itoa( pData->dwItemIdx, temp, 10 ) );
				m_lcEtc.SetItemText( lvitem.iItem, 6, pData->sLogDate );
			}
		}
		break;
	case eEtcLogKind_Hacking:
		{
			HACKINGLOG* pData = NULL;
			m_HackingLogTable.SetPositionHead();
			while( pData = m_HackingLogTable.GetData() )
			{
				lvitem.iItem = m_lcEtc.GetItemCount();	// 행
				
				lvitem.mask = LVIF_TEXT;
				lvitem.stateMask = LVIS_SELECTED;
				lvitem.state = INDEXTOSTATEIMAGEMASK(1);
				lvitem.iSubItem = 0;	
				lvitem.pszText = itoa( pData->dwLogIdx, temp, 10 );
				
				m_lcEtc.InsertItem( &lvitem );
				m_lcEtc.SetItemText( lvitem.iItem, 1, itoa( pData->dwChrIdx, temp, 10 ) );
				m_lcEtc.SetItemText( lvitem.iItem, 2, itoa( pData->dwCheatKind, temp, 10 ) );
				m_lcEtc.SetItemText( lvitem.iItem, 3, pData->sLogDate );
			}
		}
		break;
	}
}

void CLogEtcPage::OnBtnSavetofile() 
{
	switch( m_nLogKind )
	{
	case eEtcLogKind_Cheat:
		{
			DeleteFile( "CheatLog.txt" );

			FILE* fp = fopen( "CheatLog.txt", "a+" );
			if( !fp )
				return;

			char* tcolumn[7] = { "LogIdx", "CharIdx", "Type", "Parameter", "ItemDBIdx", "ItemIdx", "LogDate" };
			fprintf( fp, "%s\t%s\t%s\t%s\t%s\t%s\t%s\n", tcolumn[0], tcolumn[1], tcolumn[2], tcolumn[3],
				tcolumn[4], tcolumn[5], tcolumn[6] );

			char temp[128] = {0, };
			CHEATLOG* pData = NULL;
			m_CheatLogTable.SetPositionHead();
			while( pData = m_CheatLogTable.GetData() )
			{
				fprintf( fp, "%d\t%d\t%d\t%d\t%d\t%s\t%s\n", pData->dwLogIdx, pData->dwChrIdx, pData->dwLogType,
					pData->dwParam, pData->dwItemDBIdx, GetItemName( pData->dwItemIdx, temp),pData->sLogDate );
			}

			fclose( fp );
		}
		break;

	case eEtcLogKind_Hacking:
		{
			DeleteFile( "HackingLog.txt" );

			FILE* fp = fopen( "HackingLog.txt", "a+" );
			if( !fp )
				return;

			char* tcolumn[4] = { "LogIdx", "CharIdx", "Kind", "LogDate" };
			fprintf( fp, "%s\t%s\t%s\t%s\n", tcolumn[0], tcolumn[1], tcolumn[2], tcolumn[3] );

			char temp[128] = {0, };
			HACKINGLOG* pData = NULL;
			m_HackingLogTable.SetPositionHead();
			while( pData = m_HackingLogTable.GetData() )
			{
				fprintf( fp, "%d\t%d\t%d\t%s\n", pData->dwLogIdx, pData->dwChrIdx, pData->dwCheatKind, pData->sLogDate );
			}

			fclose( fp );
		}
		break;
	}
}

char* CLogEtcPage::GetItemName( DWORD dwItemIdx, char* temp )
{
	ITEM_INFO* pItem = GAMERESRCMNGR->GetItemInfo( dwItemIdx );

	if( pItem == NULL )
		sprintf( temp, "???" );
	else
		sprintf( temp, "%s", pItem->ItemName );

	return temp;
}

void CLogEtcPage::OnCheckLogOneday() 
{
	m_bOneDay = !m_bOneDay;
//	m_StartTime.EnableWindow( !m_bOneDay );
	m_EndTime.EnableWindow( !m_bOneDay );
	UpdateData( FALSE );
}

void CLogEtcPage::ChangeFont()
{
	m_Font.CreatePointFont(100, "Arial");
	m_lcEtc.SetFont( &m_Font );
}