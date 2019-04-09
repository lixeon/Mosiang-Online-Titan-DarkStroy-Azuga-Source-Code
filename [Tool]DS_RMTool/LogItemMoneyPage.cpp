// LogItemMoneyPage.cpp : implementation file
//

#include "stdafx.h"
#include "ds_rmtool.h"
#include "LogItemMoneyPage.h"
#include "RMNetwork.h"
#include "LogDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogItemMoneyPage property page

IMPLEMENT_DYNCREATE(CLogItemMoneyPage, CLDPropPage)


CLogItemMoneyPage::CLogItemMoneyPage() : CLDPropPage(CLogItemMoneyPage::IDD)
{
	//{{AFX_DATA_INIT(CLogItemMoneyPage)
	m_bCharName = FALSE;
	m_bItemName = FALSE;
	m_bDBIdx = FALSE;
	m_bOneDay = FALSE;
	//}}AFX_DATA_INIT
	m_LogTable.Initialize( 1000 );
	m_pDocument = NULL;
	m_bStart = FALSE;

	for( int i = 0; i < 14; ++i )
		m_bVOption[i] = TRUE;
}

CLogItemMoneyPage::~CLogItemMoneyPage()
{
	ReleaseLogTable();
}

void CLogItemMoneyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogItemMoneyPage)
	DDX_Control(pDX, IDC_DATETIMEPICKER_START, m_StartTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER_END, m_EndTime);
	DDX_Control(pDX, IDC_EDIT_CHARIDX, m_etCharIdx);
	DDX_Control(pDX, IDC_BUTTON_VIEWEROPTION, m_btnViewerOption);
	DDX_Control(pDX, IDC_COMBO_VIEWER_HOW, m_cbViewerHow);
	DDX_Control(pDX, IDC_COMBO_LOG_ITEMNAME, m_cbItemName);
	DDX_Control(pDX, IDC_EDIT_LOG_DBIDX, m_etDBIdx);
	DDX_Control(pDX, IDC_EDIT_LOG_CHARNAME, m_etCharName);
	DDX_Control(pDX, IDC_LISTCTRL_ITEMMONEYLOG, m_lcItemMoneyLog);
	DDX_Check(pDX, IDC_CHECK_LOG_CHARNAME, m_bCharName);
	DDX_Check(pDX, IDC_CHECK_LOG_ITEMNAME, m_bItemName);
	DDX_Check(pDX, IDC_CHECK_LOG_DBIDX, m_bDBIdx);
	DDX_Check(pDX, IDC_CHECK_LOG_ONEDAY, m_bOneDay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogItemMoneyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CLogItemMoneyPage)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_CHECK_LOG_CHARNAME, OnCheckLogCharname)
	ON_BN_CLICKED(IDC_CHECK_LOG_ITEMNAME, OnCheckLogItemname)
	ON_BN_CLICKED(IDC_CHECK_LOG_DBIDX, OnCheckLogDbidx)
	ON_BN_CLICKED(IDC_BUTTON_LOG_SEARCH, OnButtonLogSearch)
	ON_BN_CLICKED(IDC_CHECK_LOG_ONEDAY, OnCheckLogOneday)
	ON_CBN_SELCHANGE(IDC_COMBO_VIEWER_HOW, OnSelchangeComboViewerHow)
	ON_BN_CLICKED(IDC_BUTTON_VIEWEROPTION, OnButtonVieweroption)
	ON_BN_CLICKED(IDC_BTN_SAVETOFILE, OnBtnSavetofile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogItemMoneyPage message handlers

int CLogItemMoneyPage::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertyPage::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CLogItemMoneyPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// 
	m_bOneDay = TRUE;
	m_bCharName = TRUE;
	m_etCharName.EnableWindow( m_bCharName );
	InitComboItemName();
	InitComboViewerHow();

//	m_Font.CreatePointFont(100, "Arial");
//	m_lcItemMoneyLog.SetFont( &m_Font );

	UpdateData( FALSE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLogItemMoneyPage::InitComboItemName()
{
	m_cbItemName.ResetContent();

	CYHHashTable<ITEM_INFO>* pItemInfoList = GAMERESRCMNGR->GetItemInfo();
	ITEM_INFO* pData = NULL;
	int index = 0;
	pItemInfoList->SetPositionHead();
	while( pData = pItemInfoList->GetData() )
	{
		m_cbItemName.AddString( pData->ItemName );
		m_cbItemName.SetItemData( index, pData->ItemIdx );
		++index;			
	}
	m_cbItemName.SetCurSel( 0 );
}

void CLogItemMoneyPage::OnCheckLogOneday() 
{
	m_bOneDay = !m_bOneDay;
//	m_StartTime.EnableWindow( !m_bOneDay );
	m_EndTime.EnableWindow( !m_bOneDay );
	UpdateData( FALSE );
}

void CLogItemMoneyPage::OnCheckLogCharname() 
{
	m_bCharName = !m_bCharName;
	m_etCharName.EnableWindow( m_bCharName );
	UpdateData( FALSE );	
}

void CLogItemMoneyPage::OnCheckLogItemname() 
{
	m_bItemName = !m_bItemName;
	if( m_bItemName )
		m_bDBIdx = FALSE;
	m_cbItemName.EnableWindow( m_bItemName );
	m_etDBIdx.EnableWindow( m_bDBIdx );
	UpdateData( FALSE );	
}

void CLogItemMoneyPage::OnCheckLogDbidx() 
{
	m_bDBIdx = !m_bDBIdx;
	if( m_bDBIdx )
		m_bItemName = FALSE;	
	m_etDBIdx.EnableWindow( m_bDBIdx );
	m_cbItemName.EnableWindow( m_bItemName );	
	UpdateData( FALSE );	
}

void CLogItemMoneyPage::OnButtonLogSearch() 
{
	CString sName;
	DWORD dwItem = 0;

	// logtype
	DWORD dwLogType;
	if( m_bCharName == FALSE && m_bDBIdx == FALSE && m_bItemName == FALSE )
	{
		MessageBox( "Enter The Search Data!!", "Error" );
		return;
	}
	else if( m_bCharName == TRUE && m_bDBIdx == FALSE && m_bItemName == FALSE )		// 이름만,
	{
		dwLogType = eSearchType_ChrName;
		if( m_etCharName.GetWindowTextLength() == 0 )
		{
			MessageBox( "Enter Character Name!!", "Error" );
			return;
		}
		else
			m_etCharName.GetWindowText( sName );
	}
	else if( m_bCharName == TRUE && m_bDBIdx == TRUE && m_bItemName == FALSE )
	{
		dwLogType = eSearchType_ChrNameItemDBIdx;
		if( m_etCharName.GetWindowTextLength() == 0 || m_etDBIdx.GetWindowTextLength() == 0 )
		{
			MessageBox( "Enter The Search Data!!", "Error" );
			return;
		}
		else
		{
			m_etDBIdx.GetWindowText( sName );
			dwItem = atoi( (char*)(LPCTSTR)sName );
			m_etCharName.GetWindowText( sName );
		}
	}
	else if( m_bCharName == TRUE && m_bDBIdx == FALSE && m_bItemName == TRUE )
	{
		dwLogType = eSearchType_ChrNameItemIdx;
		if( m_etCharName.GetWindowTextLength() == 0 )
		{
			MessageBox( "Enter Character Name!!", "Error" );
			return;
		}
		else
		{
			m_etCharName.GetWindowText( sName );
			dwItem = m_cbItemName.GetItemData( m_cbItemName.GetCurSel() );
		}
	}
	else if( m_bCharName == FALSE && m_bDBIdx == TRUE && m_bItemName == FALSE )
	{
		dwLogType = eSearchType_ItemDBIdx;
		if( m_etDBIdx.GetWindowTextLength() == 0 )
		{
			MessageBox( "Enter Search Data!!", "Error" );
			return;
		}
		else
		{
			m_etDBIdx.GetWindowText( sName );
			dwItem = atoi( (char*)(LPCTSTR)sName );
		}
	}
	else if( m_bCharName == FALSE && m_bDBIdx == FALSE && m_bItemName == TRUE )
	{
		dwLogType = eSearchType_ItemIdx;
		dwItem = m_cbItemName.GetItemData( m_cbItemName.GetCurSel() );		
	}

	// Search
	if( dwLogType == eSearchType_ItemDBIdx || dwLogType == eSearchType_ItemIdx )
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

		m_pDocument->m_dwPage = 0;
	}
}

void CLogItemMoneyPage::SetCharacterIdx( DWORD dwCharIdx )
{
//	if( dwCharIdx == 0 )
//		return;

	char temp[128] = {0, };
	sprintf( temp, "%d", dwCharIdx );
	m_etCharIdx.SetWindowText( temp );
	m_etCharIdx.EnableWindow( TRUE );
	m_dwCharIdx = dwCharIdx;

	CString sName, sSDate, sEDate;
	DWORD dwItem = 0;
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

	// logtype
	DWORD dwLogType;
	if( m_bCharName == FALSE && m_bDBIdx == FALSE && m_bItemName == FALSE )
	{
		MessageBox( "Enter The Search Data!!", "Error" );
		return;
	}
	else if( m_bCharName == TRUE && m_bDBIdx == FALSE && m_bItemName == FALSE )		// 이름만,
	{
		dwLogType = eSearchType_ChrName;
		if( m_etCharName.GetWindowTextLength() == 0 )
		{
			MessageBox( "Enter Character Name!!", "Error" );
			return;
		}
		else
			m_etCharName.GetWindowText( sName );
	}
	else if( m_bCharName == TRUE && m_bDBIdx == TRUE && m_bItemName == FALSE )
	{
		dwLogType = eSearchType_ChrNameItemDBIdx;
		if( m_etCharName.GetWindowTextLength() == 0 || m_etDBIdx.GetWindowTextLength() == 0 )
		{
			MessageBox( "Enter The Search Data!!", "Error" );
			return;
		}
		else
		{
			m_etDBIdx.GetWindowText( sName );
			dwItem = atoi( (char*)(LPCTSTR)sName );
			m_etCharName.GetWindowText( sName );
		}
	}
	else if( m_bCharName == TRUE && m_bDBIdx == FALSE && m_bItemName == TRUE )
	{
		dwLogType = eSearchType_ChrNameItemIdx;
		if( m_etCharName.GetWindowTextLength() == 0 )
		{
			MessageBox( "Enter Character Name!!", "Error" );
			return;
		}
		else
		{
			m_etCharName.GetWindowText( sName );
			dwItem = m_cbItemName.GetItemData( m_cbItemName.GetCurSel() );
		}
	}
	else if( m_bCharName == FALSE && m_bDBIdx == TRUE && m_bItemName == FALSE )
	{
		dwLogType = eSearchType_ItemDBIdx;
		if( m_etDBIdx.GetWindowTextLength() == 0 )
		{
			MessageBox( "Enter Search Data!!", "Error" );
			return;
		}
		else
		{
			m_etDBIdx.GetWindowText( sName );
			dwItem = atoi( (char*)(LPCTSTR)sName );
		}
	}
	else if( m_bCharName == FALSE && m_bDBIdx == FALSE && m_bItemName == TRUE )
	{
		dwLogType = eSearchType_ItemIdx;
		dwItem = m_cbItemName.GetItemData( m_cbItemName.GetCurSel() );		
	}
	
	// Search
	TMSG_GAMELOGSEARCH GameLogSearch;
	GameLogSearch.Category = MP_RMTOOL_GAMELOG;
	GameLogSearch.Protocol = MP_RMTOOL_ITEMMONEYLOGFROMTYPE_SYN;
	GameLogSearch.dwTemplateIdx = m_pDocument->GetDocumentIdx();
	GameLogSearch.wServer = 0;
	GameLogSearch.wType = dwLogType;
	GameLogSearch.dwChrIdx = m_dwCharIdx;
	GameLogSearch.dwItem = dwItem;
	GameLogSearch.bDayOnly = m_bOneDay;
	strcpy( GameLogSearch.sSDate, (char*)(LPCTSTR)sSDate );
	strcpy( GameLogSearch.sEDate, (char*)(LPCTSTR)sEDate );

	NETWORK->Send( &GameLogSearch, sizeof(TMSG_GAMELOGSEARCH) );

	ReleaseLogTable();

	m_pDocument->StartDlg();
}

void CLogItemMoneyPage::SetLogItemMoney( TMSG_LOGITEMMONEY* pMsg )
{
//	if( !pMsg->bEnd )
//		ReleaseLogTable();
	for( int i = 0; i < pMsg->wCount; ++i )
	{
		LOGITEMMONEY* pData = new LOGITEMMONEY;
		memcpy( pData, &pMsg->sLog[i], sizeof(LOGITEMMONEY) );

		m_LogTable.Add( pData, pData->dwLogIdx );
	}

	InitLCItemMoneyLog();
	UpdateLCItemMoneyLog( eLog_Max );

	m_cbViewerHow.EnableWindow( TRUE );
	m_btnViewerOption.EnableWindow( TRUE );
}


void CLogItemMoneyPage::ReleaseLogTable()
{
	LOGITEMMONEY* pData = NULL;
	m_LogTable.SetPositionHead();
	while( pData = m_LogTable.GetData() )
		delete pData;
	m_LogTable.RemoveAll();
}

void CLogItemMoneyPage::InitLCItemMoneyLog()
{
	m_lcItemMoneyLog.DeleteAllItems();

	// Delete all of the columns.
	int i = 0;
	int nColumnCount = m_lcItemMoneyLog.GetHeaderCtrl()->GetItemCount();
	for( i = 0; i < nColumnCount; ++i )
		m_lcItemMoneyLog.DeleteColumn( 0 );

	char* tcolumn[14] = { "ItemDBIdx", "ItemName", "ItemDur", "How", "CurrentOwner", "CharIdx", "Money",
						 "BeforeOwner", "CharIdx", "Money", "ChangeValue", "LogDate", "FromPos", "ToPos" };
	int tcolumnsize[14] = { 80, 80, 80, 50, 80, 60, 40, 80, 60, 40, 40, 100, 40, 40 };
	
	// listctrl column 설정
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_CENTER;
	int index = 0;
	for( i = 0; i < 14; ++i )
	{
		if( m_bVOption[i] )
		{
			lvcolumn.iSubItem = index;
			lvcolumn.cx = tcolumnsize[i];
			lvcolumn.pszText = tcolumn[i];
			m_lcItemMoneyLog.InsertColumn( index, &lvcolumn );
			++index;
		}
	}
	// style
	DWORD dwExStyle = m_lcItemMoneyLog.GetExtendedStyle();
	dwExStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_lcItemMoneyLog.SetExtendedStyle( dwExStyle );
}

void CLogItemMoneyPage::UpdateLCItemMoneyLog( DWORD dwHow )
{
	m_lcItemMoneyLog.DeleteAllItems();

	char temp[256];	
	LV_ITEM lvitem;
	LOGITEMMONEY* pData = NULL;
	m_LogTable.SetPositionHead();
	while( pData = m_LogTable.GetData() )
	{
		int i = 0;
		int index = 0;
		if( pData->dwLogType == dwHow || dwHow == eLog_Max )
		{
			lvitem.iItem = m_lcItemMoneyLog.GetItemCount();	// 행

			lvitem.mask = LVIF_TEXT;
			lvitem.stateMask = LVIS_SELECTED;
			lvitem.state = INDEXTOSTATEIMAGEMASK(1);
			lvitem.iSubItem = i;
			
			for( i = 0; i < 14; ++i )
			{
				if( m_bVOption[i] )
					break;
			}
			lvitem.pszText = GetLCData( pData, i, temp );
			m_lcItemMoneyLog.InsertItem( &lvitem );
	
			for( int j = i+1; j < 14; ++j )
			{
				if( m_bVOption[j] )
					m_lcItemMoneyLog.SetItemText( lvitem.iItem, ++index, GetLCData( pData, j, temp ) );
			}
		}
	}
}

char* CLogItemMoneyPage::GetLCData( LOGITEMMONEY* pData, DWORD dwIndex, char* temp )
{
	switch( dwIndex )
	{
	case 0:		return itoa( pData->dwItemDBIdx, temp, 10 );
	case 1:		return GetItemName( pData->dwItemIdx, temp );
	case 2:		return itoa( pData->dwItemDur, temp, 10 );
	case 3:		return GetLogType( pData->dwLogType, temp );
	case 4:		return GetName( pData->dwLogType, pData->dwToChrIdx, pData->sToChrName );
	case 5:		return itoa( pData->dwToChrIdx, temp, 10 );
	case 6:		return itoa( pData->dwToTotalMoney, temp, 10 );
	case 7:		return GetName( pData->dwLogType, pData->dwFromChrIdx, pData->sFromChrName );
	case 8:		return itoa( pData->dwFromChrIdx, temp, 10 );
	case 9:		return itoa( pData->dwFromTotalMoney, temp, 10 );
	case 10:	return itoa( pData->dwChangeMoney, temp, 10 );
	case 11:	return pData->sRegDate;
	case 12:	return itoa( pData->dwItemFromPosition, temp, 10 );
	case 13:	return itoa( pData->dwItemToPosition, temp, 10 );
	default:	sprintf( temp, "");	return temp;
	}	
}

char* CLogItemMoneyPage::GetItemName( DWORD dwItemIdx, char* temp )
{
	if( dwItemIdx == 0 )
	{
		sprintf( temp, "Money" );
		return temp;
	}
	ITEM_INFO* pItem = GAMERESRCMNGR->GetItemInfo( dwItemIdx );

	if( pItem == NULL )
		sprintf( temp, "???" );
	else
		sprintf( temp, "%s", pItem->ItemName );

	return temp;
}

char* CLogItemMoneyPage::GetLogType( DWORD dwLogType, char* temp )
{
	switch( dwLogType )
	{
	case eLog_ItemDiscard:	sprintf( temp, "Remove" );	break;

	//item move
	case eLog_ItemMoveInvenToPyoguk:	sprintf( temp, "I ->> W" );	break;
	case eLog_ItemMovePyogukToInven:	sprintf( temp, "W ->> I" );	break;
	case eLog_ItemMoveInvenToMunpa:		sprintf( temp, "I ->> G" );	break;
	case eLog_ItemMoveMunpaToInven:		sprintf( temp, "G ->> I" );	break;
	case eLog_ItemMoveMunpaToMunpa:		sprintf( temp, "G ->> G" );	break;
	//item obtain
	case eLog_ItemObtainBuy:			sprintf( temp, "Buy" );	break;
	case eLog_ItemObtainDivide:			sprintf( temp, "Divide" );	break;
	case eLog_ItemObtainCheat:			sprintf( temp, "GMTool" );	break;
	case eLog_ItemObtainMonster:		sprintf( temp, "Hunt" );	break;
	case eLog_ItemObtainQuest:			sprintf( temp, "Quest" );	break;
	case eLog_ItemObtainPK:				sprintf( temp, "ItemObtainPK" );	break;
		
	case eLog_ItemSell:					sprintf( temp, "Sell" );		break;
	case eLog_ItemUse:					sprintf( temp, "Use" );			break;
	case eLog_StreetStallBuyAll:		sprintf( temp, "StreetStall" );	break;
	case eLog_StreetStallBuyDivide:		sprintf( temp, "StreetStall" );	break;
	case eLog_StreetStallSellDivide:	sprintf( temp, "StreetStall" );	break;

	case eLog_Exchange:					sprintf( temp, "Exchange" );	break;

	case eLog_ItemReinforceSuccess:		sprintf( temp, "ItemReinforceSuccess" );	break;
	case eLog_ItemReinforceFail:		sprintf( temp, "ItemReinforceFail" );	break;

	case eLog_ItemMixSuccess:			sprintf( temp, "ItemMixSuccess" );	break;
	case eLog_ItemMixFail:				sprintf( temp, "ItemMixFail" );	break;
	case eLog_ItemMixBigFail:			sprintf( temp, "ItemMixBigFail" );	break;

	case eLog_ItemDestroyMix:			sprintf( temp, "ItemDestroyMix" );	break;
	case eLog_ItemDestroyReinforce:		sprintf( temp, "ItemDestroyReinforce" );	break;
	case eLog_ItemDestroyUpgrade:		sprintf( temp, "ItemDestroyUpgrade" );	break;
	case eLog_ItemDestroyByWanted:		sprintf( temp, "ItemDestroyByWanted" );	break;
	case eLog_ItemDestroyByBadFame:		sprintf( temp, "ItemDestroyByBadFame" );	break;

	case eLog_MoneyObtainPK:			sprintf( temp, "MoneyObtainPK" );	break;
	//exp obtain
	case eLog_ExpObtainPK:				sprintf( temp, "ExpObtainPK" );	break;
		
	case 1100:							sprintf( temp, "ChangeMoney" );	break;

	// RMTool
	case eLog_RMTool:					sprintf( temp, "RMTool" );	break;
	case 53:							sprintf( temp, "Hunt" );	break;
	case 50:							sprintf( temp, "Buy" );	break;
	case 51:							sprintf( temp, "Divide" );	break;
	case 10:							sprintf( temp, "I ->> W" );	break;
	
	default:							itoa( dwLogType, temp, 10 );		break;
	}
	
	return temp;
}

char* CLogItemMoneyPage::GetName( DWORD dwLogType, DWORD dwIdx, char* temp )
{
	if( strcmp( temp, "(null)" ) == 0 )
		sprintf( temp, "-" );
	if( strcmp( temp, "" ) != 0 )
		return temp;
	

	switch( dwLogType )
	{
	case eLog_ItemDiscard:				return temp;
//	case eLog_ItemMoveInvenToPyoguk:	sprintf( temp, "I ->> W" );	break;
//	case eLog_ItemMovePyogukToInven:	sprintf( temp, "W ->> I" );	break;
//	case eLog_ItemMoveInvenToMunpa:		sprintf( temp, "I ->> G" );	break;
//	case eLog_ItemMoveMunpaToInven:		sprintf( temp, "G ->> I" );	break;
//	case eLog_ItemMoveMunpaToMunpa:		sprintf( temp, "G ->> G" );	break;
	//item obtain
	case eLog_ItemSell:
	case eLog_ItemObtainBuy:
		{
			NPC_LIST* pNpc = GAMERESRCMNGR->GetNpcList( dwIdx );
			if( pNpc == NULL )
				sprintf( temp, "???" );
			else
				sprintf( temp, "%s", pNpc->Name );
		}
		break;
	case eLog_ItemObtainDivide:			return temp;
			
	case eLog_ItemObtainCheat:			sprintf( temp, "-" );	break;
	case eLog_ItemObtainMonster:
		{
			MONSTEREX_LIST* pMonster = GAMERESRCMNGR->GetMonsterInfo( dwIdx );
			if( pMonster == NULL )
				sprintf( temp, "???" );
			else
				sprintf( temp, "%s", pMonster->Name );
		}
		break;
//	case eLog_ItemObtainQuest:			sprintf( temp, "Quest" );	break;
//	case eLog_ItemObtainPK:				sprintf( temp, "PK" );	break;
		
//	case eLog_ItemSell:					sprintf( temp, "Sell" );		break;
//	case eLog_ItemUse:					sprintf( temp, "Use" );			break;
//	case eLog_StreetStall:				sprintf( temp, "StreetStall" );	break;
//	case eLog_Exchange:					sprintf( temp, "Exchange" );	break;
	//die panelty with bad fame.
//	case eLog_ItemDestroyByBadFame:		sprintf( temp, "BadFame" );	break;
//	case eLog_MoneyObtainPK:			sprintf( temp, "PK" );	break;
	//exp obtain
//	case eLog_ExpObtainPK:				sprintf( temp, "PK" );	break;
	case 1100:				sprintf( temp, "-" );	break;
	case eLog_RMTool:					sprintf( temp, "-" );	break;
	default:							sprintf( temp, "???" );		break;
	}

	return temp;
}

void CLogItemMoneyPage::InitComboViewerHow()
{
	m_cbViewerHow.ResetContent();

	char temp[256];

	m_cbViewerHow.AddString( "All Data" );
	m_cbViewerHow.SetItemData( 0, eLog_Max );

	m_cbViewerHow.AddString( GetLogType( eLog_ItemDiscard, temp ) );
	m_cbViewerHow.SetItemData( 1, eLog_ItemDiscard );

	m_cbViewerHow.AddString( GetLogType( eLog_ItemMoveInvenToPyoguk, temp ) );
	m_cbViewerHow.SetItemData( 2, eLog_ItemMoveInvenToPyoguk );

	m_cbViewerHow.AddString( GetLogType( eLog_ItemMovePyogukToInven, temp ) );
	m_cbViewerHow.SetItemData( 3, eLog_ItemMovePyogukToInven );

	m_cbViewerHow.AddString( GetLogType( eLog_ItemMoveInvenToMunpa, temp ) );
	m_cbViewerHow.SetItemData( 4, eLog_ItemMoveInvenToMunpa );

	m_cbViewerHow.AddString( GetLogType( eLog_ItemMoveMunpaToInven, temp ) );
	m_cbViewerHow.SetItemData( 5, eLog_ItemMoveMunpaToInven );

	m_cbViewerHow.AddString( GetLogType( eLog_ItemMoveMunpaToMunpa, temp ) );
	m_cbViewerHow.SetItemData( 6, eLog_ItemMoveMunpaToMunpa );

	m_cbViewerHow.AddString( GetLogType( eLog_ItemObtainBuy, temp ) );
	m_cbViewerHow.SetItemData( 7, eLog_ItemObtainBuy );

	m_cbViewerHow.AddString( GetLogType( eLog_ItemObtainDivide, temp ) );
	m_cbViewerHow.SetItemData( 8, eLog_ItemObtainDivide );

	m_cbViewerHow.AddString( GetLogType( eLog_ItemObtainCheat, temp ) );
	m_cbViewerHow.SetItemData( 9, eLog_ItemObtainCheat );

	m_cbViewerHow.AddString( GetLogType( eLog_ItemObtainMonster, temp ) );
	m_cbViewerHow.SetItemData( 10, eLog_ItemObtainMonster );

	m_cbViewerHow.AddString( GetLogType( eLog_ItemObtainQuest, temp ) );
	m_cbViewerHow.SetItemData( 11, eLog_ItemObtainQuest );
	
	m_cbViewerHow.AddString( GetLogType( eLog_ItemObtainPK, temp ) );
	m_cbViewerHow.SetItemData( 12, eLog_ItemObtainPK );

	m_cbViewerHow.AddString( GetLogType( eLog_ItemSell, temp ) );
	m_cbViewerHow.SetItemData( 13, eLog_ItemSell );

	m_cbViewerHow.AddString( GetLogType( eLog_ItemUse, temp ) );
	m_cbViewerHow.SetItemData( 14, eLog_ItemUse );

	m_cbViewerHow.AddString( GetLogType( eLog_StreetStallBuyAll, temp ) );
	m_cbViewerHow.SetItemData( 15, eLog_StreetStallBuyAll );

	m_cbViewerHow.AddString( GetLogType( eLog_Exchange, temp ) );
	m_cbViewerHow.SetItemData( 16, eLog_Exchange );

	m_cbViewerHow.AddString( GetLogType( eLog_ItemDestroyByBadFame, temp ) );
	m_cbViewerHow.SetItemData( 17, eLog_ItemDestroyByBadFame );

	m_cbViewerHow.AddString( GetLogType( eLog_MoneyObtainPK, temp ) );
	m_cbViewerHow.SetItemData( 18, eLog_MoneyObtainPK );

	m_cbViewerHow.AddString( GetLogType( eLog_ExpObtainPK, temp ) );
	m_cbViewerHow.SetItemData( 19, eLog_ExpObtainPK );

	m_cbViewerHow.AddString( GetLogType( eLog_RMTool, temp ) );
	m_cbViewerHow.SetItemData( 20, eLog_RMTool );

	m_cbViewerHow.SetCurSel( 0 );

	m_cbViewerHow.EnableWindow( FALSE );
}

void CLogItemMoneyPage::OnSelchangeComboViewerHow() 
{
	DWORD dwType = m_cbViewerHow.GetItemData( m_cbViewerHow.GetCurSel() );

	UpdateLCItemMoneyLog( dwType );
}

void CLogItemMoneyPage::OnButtonVieweroption() 
{
	m_ViewerOptionDlg.SetPage( this );
	m_ViewerOptionDlg.DoModal();

	InitLCItemMoneyLog();
	DWORD dwType = m_cbViewerHow.GetItemData( m_cbViewerHow.GetCurSel() );
	UpdateLCItemMoneyLog( dwType );
}

void CLogItemMoneyPage::CommandMessage( UINT nID, int nCode )
{
}

void CLogItemMoneyPage::OnBtnSavetofile() 
{
	DeleteFile( "ItemMoneyLog.txt" );

	FILE* fp = fopen( "ItemMoneyLog.txt", "a+" );
	if( !fp )
		return;

	char* tcolumn[12] = { "ItemDBIdx", "ItemName", "ItemDur", "How", "CurrentOwner", "Money",
						 "BeforeOwner", "Money", "ChangeValue", "LogDate", "FromPos", "ToPos" };

	fprintf( fp, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", tcolumn[0], tcolumn[1], tcolumn[2],
		tcolumn[3], tcolumn[4], tcolumn[5], tcolumn[6], tcolumn[7], tcolumn[8], tcolumn[9], tcolumn[10], tcolumn[11] );

	char temp1[128] = {0, };
	char temp2[128] = {0, };
	char temp3[128] = {0, };
	char temp4[128] = {0, };
	LOGITEMMONEY* pData = NULL;
	m_LogTable.SetPositionHead();
	while( pData = m_LogTable.GetData() )
	{
		fprintf( fp, "%d\t%s\t%d\t%s\t%s\t%d\t%s\t%d\t%d\t%s\t%d\t%d\n", pData->dwItemDBIdx, GetItemName( pData->dwItemIdx, temp1 ), pData->dwItemDur,
			GetLogType( pData->dwLogType, temp2 ), GetName( pData->dwLogType, pData->dwToChrIdx, pData->sToChrName ), pData->dwToTotalMoney,
			GetName( pData->dwLogType, pData->dwFromChrIdx, pData->sFromChrName ), pData->dwFromTotalMoney,
			pData->dwChangeMoney, pData->sRegDate, pData->dwItemFromPosition, pData->dwItemToPosition );
	}

	fclose( fp );
}

void CLogItemMoneyPage::ChangeFont()
{
	m_Font.CreatePointFont(100, "Arial");
	m_lcItemMoneyLog.SetFont( &m_Font );
}