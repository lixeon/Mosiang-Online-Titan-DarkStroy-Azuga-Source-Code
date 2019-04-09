// LogOperPage.cpp : implementation file
//

#include "stdafx.h"
#include "ds_rmtool.h"
#include "RMNetwork.h"
#include "LogOperDoc.h"
#include "LogOperPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogOperPage property page

IMPLEMENT_DYNCREATE(CLogOperPage, CLDPropPage)

CLogOperPage::CLogOperPage() : CLDPropPage(CLogOperPage::IDD)
{
	//{{AFX_DATA_INIT(CLogOperPage)
	m_bOneDay = TRUE;
	m_nLogKind = 0;
	m_nSelectLog = 0;
	//}}AFX_DATA_INIT

	m_ToolLogTable.Initialize( 1000 );
}

CLogOperPage::~CLogOperPage()
{
	ReleaseToolLogTable();
}

void CLogOperPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogOperPage)
	DDX_Control(pDX, IDC_COMBO_RMLOG, m_cbRMLog);
	DDX_Control(pDX, IDC_COMBO_GMLOG, m_cbGMLog);
	DDX_Control(pDX, IDC_EDIT_OPERNAME, m_etOperName);
	DDX_Control(pDX, IDC_LISTCTRL_OPERLOG, m_lcOper);
	DDX_Control(pDX, IDC_DATETIMEPICKER_START, m_StartTime);
	DDX_Control(pDX, IDC_DATETIMEPICKER_END, m_EndTime);
	DDX_Check(pDX, IDC_CHECK_LOG_ONEDAY, m_bOneDay);
	DDX_Radio(pDX, IDC_RADIO1, m_nLogKind);
	DDX_Radio(pDX, IDC_RADIO3, m_nSelectLog);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogOperPage, CLDPropPage)
	//{{AFX_MSG_MAP(CLogOperPage)
	ON_BN_CLICKED(IDC_CHECK_LOG_ONEDAY, OnCheckLogOneday)
	ON_BN_CLICKED(IDC_BUTTON_LOG_SEARCH, OnButtonLogSearch)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	ON_CBN_SELCHANGE(IDC_COMBO_RMLOG, OnSelchangeComboRmlog)
	ON_CBN_SELCHANGE(IDC_COMBO_GMLOG, OnSelchangeComboGmlog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogOperPage message handlers

BOOL CLogOperPage::OnInitDialog() 
{
	CLDPropPage::OnInitDialog();

//	m_Font.CreatePointFont(100, "Arial");
//	m_lcOper.SetFont( &m_Font );

	m_bOneDay = TRUE;
	m_nLogKind = 0;
	m_nSelectLog = 0;
	InitComboBoxData();
	InitListControl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLogOperPage::OnCheckLogOneday() 
{
	m_bOneDay = !m_bOneDay;
	m_EndTime.EnableWindow( !m_bOneDay );
	UpdateData( FALSE );
}

void CLogOperPage::OnButtonLogSearch() 
{
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

	if( m_nLogKind != 0 )
	{
		if( m_etOperName.GetWindowTextLength() == 0 )
			
		{
			MessageBox( "Input Search Operator Name!!", "Error" );
			return;
		}
		else if( m_etOperName.GetWindowTextLength() > 17 )
		{
			MessageBox( "Search Operator Name Is Too Long!!", "Error" );
			return;
		}
	}

	// Search
	TMSG_GAMELOGSEARCH GameLogSearch;
	GameLogSearch.Category = MP_RMTOOL_GAMELOG;
	GameLogSearch.Protocol = MP_RMTOOL_TOOLLOG_SYN;
	GameLogSearch.dwTemplateIdx = m_pDocument->GetDocumentIdx();
	GameLogSearch.wServer = 0;
	GameLogSearch.wType = m_nLogKind;
	m_etOperName.GetWindowText( sName );
	strncpy( GameLogSearch.sName, (char*)(LPCTSTR)sName, 17 );
	GameLogSearch.bDayOnly = m_bOneDay;
	strcpy( GameLogSearch.sSDate, (char*)(LPCTSTR)sSDate );
	strcpy( GameLogSearch.sEDate, (char*)(LPCTSTR)sEDate );

	NETWORK->Send( &GameLogSearch, sizeof(TMSG_GAMELOGSEARCH) );

	ReleaseToolLogTable();
	OnRadio3();
}

void CLogOperPage::OnRadio1() 
{
	m_nLogKind = 0;
	m_etOperName.SetWindowText( "" );
	m_etOperName.EnableWindow( FALSE );
}

void CLogOperPage::OnRadio2() 
{
	m_nLogKind = 1;	
	m_etOperName.EnableWindow( TRUE );
}

void CLogOperPage::OnRadio3() 
{
	m_nSelectLog = 0;	
	m_cbGMLog.EnableWindow( FALSE );
	m_cbRMLog.EnableWindow( FALSE );
	m_cbGMLog.SetCurSel( 0 );
	m_cbRMLog.SetCurSel( 0 );
	UpdateData( FALSE );

	UpdateListControl();
}

void CLogOperPage::OnRadio4() 
{
	m_nSelectLog = 1;		
	m_cbGMLog.EnableWindow( TRUE );
	m_cbRMLog.EnableWindow( FALSE );

	UpdateListControlFromLogKind( eToolLog_GM, 0 );
	m_cbRMLog.SetCurSel( 0 );
}

void CLogOperPage::OnRadio5() 
{
	m_nSelectLog = 2;		
	m_cbGMLog.EnableWindow( FALSE );
	m_cbRMLog.EnableWindow( TRUE );	

	UpdateListControlFromLogKind( eToolLog_RM, 0 );
	m_cbGMLog.SetCurSel( 0 );
}

void CLogOperPage::ReleaseToolLogTable()
{
	TOOLLOG* pData = NULL;
	m_ToolLogTable.SetPositionHead();
	while( pData = m_ToolLogTable.GetData() )
		delete pData;
	m_ToolLogTable.RemoveAll();	
}

void CLogOperPage::SetOperLog( TMSG_TOOLLOG* pMsg )
{
	for( int i = 0; i < pMsg->wCount; ++i )
	{
		TOOLLOG* pData = new TOOLLOG;
		memcpy( pData, &pMsg->sLog[i], sizeof(TOOLLOG) );

		m_ToolLogTable.Add( pData, pData->dwLogIdx );
	}
	
	UpdateListControl();
}

void CLogOperPage::InitListControl()
{
	// Delete all of the columns.
	int i = 0;
	int nColumnCount = m_lcOper.GetHeaderCtrl()->GetItemCount();
	for( i = 0; i < nColumnCount; ++i )
		m_lcOper.DeleteColumn( 0 );
	m_lcOper.DeleteAllItems();

	char* tcolumn[12] = { "LogIdx", "LogType", "LogKind", "OperIdx", "OperName", "TargetIdx", "TargetName",
						  "Param1", "Param2", "Param3", "Param4", "LogDate" };
	int tcolumnsize[12] = { 80, 80, 80, 50, 80, 50, 80, 50, 50, 50, 50, 120 };

	// listctrl column 설정
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_CENTER;
	for( i = 0; i < 12; ++i )
	{
		lvcolumn.iSubItem = i;
		lvcolumn.cx = tcolumnsize[i];
		lvcolumn.pszText = tcolumn[i];
		m_lcOper.InsertColumn( i, &lvcolumn );
	}
	// style
	DWORD dwExStyle = m_lcOper.GetExtendedStyle();
	dwExStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_lcOper.SetExtendedStyle( dwExStyle );
}

void CLogOperPage::UpdateListControl()
{
	m_lcOper.DeleteAllItems();

	char temp[256];	
	LV_ITEM lvitem;

	TOOLLOG* pData = NULL;
	m_ToolLogTable.SetPositionHead();
	while( pData = m_ToolLogTable.GetData() )
	{
		lvitem.iItem = m_lcOper.GetItemCount();	// 행
				
		lvitem.mask = LVIF_TEXT;
		lvitem.stateMask = LVIS_SELECTED;
		lvitem.state = INDEXTOSTATEIMAGEMASK(1);
		lvitem.iSubItem = 0;	
		lvitem.pszText = itoa( pData->dwLogIdx, temp, 10 );
				
		m_lcOper.InsertItem( &lvitem );
		m_lcOper.SetItemText( lvitem.iItem, 1, GetLogTypeName( pData->dwLogType, temp ) );
		m_lcOper.SetItemText( lvitem.iItem, 2, GetLogKindName( pData->dwLogType, pData->dwLogKind, temp ) );
		m_lcOper.SetItemText( lvitem.iItem, 3, itoa( pData->dwOperIdx, temp, 10 ) );
		m_lcOper.SetItemText( lvitem.iItem, 4, pData->sOperName );
		m_lcOper.SetItemText( lvitem.iItem, 5, itoa( pData->dwTargetIdx, temp, 10 ) );
		m_lcOper.SetItemText( lvitem.iItem, 6, pData->sTargetName );
		m_lcOper.SetItemText( lvitem.iItem, 7, GetParam1Name( pData->dwLogType, pData->dwLogKind, pData->dwParam1, temp ) );
		m_lcOper.SetItemText( lvitem.iItem, 8, GetParam2Name( pData->dwLogType, pData->dwLogKind, pData->dwParam2, temp ) );
		m_lcOper.SetItemText( lvitem.iItem, 9, GetParam3Name( pData->dwLogType, pData->dwLogKind, pData->dwParam3, temp ) );
		m_lcOper.SetItemText( lvitem.iItem, 10, GetParam4Name( pData->dwLogType, pData->dwLogKind, pData->dwParam4, temp ) );
		m_lcOper.SetItemText( lvitem.iItem, 11, pData->sLogDate );
	}
}

void CLogOperPage::InitComboBoxData()
{
	// gmlog
	m_cbGMLog.AddString( "All Data" );			m_cbGMLog.SetItemData( 0, 0 );
	m_cbGMLog.AddString( "Move" );				m_cbGMLog.SetItemData( 1, eGMLog_Move );
	m_cbGMLog.AddString( "Item" );				m_cbGMLog.SetItemData( 2, eGMLog_Item );
	m_cbGMLog.AddString( "Money" );				m_cbGMLog.SetItemData( 3, eGMLog_Money );
	m_cbGMLog.AddString( "Skill" );				m_cbGMLog.SetItemData( 4, eGMLog_Skill );
	m_cbGMLog.AddString( "Ability" );			m_cbGMLog.SetItemData( 5, eGMLog_Ability );
	m_cbGMLog.AddString( "Chat_Map" );			m_cbGMLog.SetItemData( 6, eGMLog_Chat_Map );
	m_cbGMLog.AddString( "Chat_User" );			m_cbGMLog.SetItemData( 7, eGMLog_Chat_User );
	m_cbGMLog.AddString( "PK" );				m_cbGMLog.SetItemData( 8, eGMLog_PK );
	m_cbGMLog.AddString( "Disconnect_Map" );	m_cbGMLog.SetItemData( 9, eGMLog_Disconnect_Map );
	m_cbGMLog.AddString( "Disconnect_User" );	m_cbGMLog.SetItemData( 10, eGMLog_Disconnect_User );
	m_cbGMLog.AddString( "Block" );				m_cbGMLog.SetItemData( 11, eGMLog_Block );
	m_cbGMLog.AddString( "Regen" );				m_cbGMLog.SetItemData( 12, eGMLog_Regen );
	m_cbGMLog.AddString( "Event" );				m_cbGMLog.SetItemData( 13, eGMLog_Event );
	m_cbGMLog.SetCurSel( 0 );

	// rmlog
	m_cbRMLog.AddString( "All Data" );				m_cbRMLog.SetItemData( 0, 0 );
	m_cbRMLog.AddString( "OperInsert" );			m_cbRMLog.SetItemData( 1, eRMLog_OperInsert );
	m_cbRMLog.AddString( "OperDelete" );			m_cbRMLog.SetItemData( 2, eRMLog_OperDelete );
	m_cbRMLog.AddString( "IpInsert" );				m_cbRMLog.SetItemData( 3, eRMLog_IpInsert );
	m_cbRMLog.AddString( "IpDelete" );				m_cbRMLog.SetItemData( 4, eRMLog_IpDelete );
	m_cbRMLog.AddString( "ChangeCharacterInfo" );	m_cbRMLog.SetItemData( 5, eRMLog_ChangeCharacterInfo );
	m_cbRMLog.AddString( "ChangeWareInfo" );		m_cbRMLog.SetItemData( 6, eRMLog_ChangeWareInfo );
	m_cbRMLog.AddString( "SkillInsert" );			m_cbRMLog.SetItemData( 7, eRMLog_SkillInsert );
	m_cbRMLog.AddString( "SkillDelete" );			m_cbRMLog.SetItemData( 8, eRMLog_SkillDelete );
	m_cbRMLog.AddString( "SkillUpdate" );			m_cbRMLog.SetItemData( 9, eRMLog_SkillUpdate );
	m_cbRMLog.AddString( "AbilityUpdate" );			m_cbRMLog.SetItemData( 10, eRMLog_AbilityUpdate );
	m_cbRMLog.AddString( "QuestDelete" );			m_cbRMLog.SetItemData( 11, eRMLog_QuestDelete );
	m_cbRMLog.AddString( "ChangeLoginPoint" );		m_cbRMLog.SetItemData( 12, eRMLog_ChangeLoginPoint );
	m_cbRMLog.AddString( "ItemInsert" );			m_cbRMLog.SetItemData( 13, eRMLog_ItemInsert );
	m_cbRMLog.AddString( "ItemDelete" );			m_cbRMLog.SetItemData( 14, eRMLog_ItemDelete );
	m_cbRMLog.AddString( "ItemUpdate" );			m_cbRMLog.SetItemData( 15, eRMLog_ItemUpdate );
	m_cbRMLog.AddString( "ChangeUserLevel" );		m_cbRMLog.SetItemData( 16, eRMLog_ChangeUserLevel );
	m_cbRMLog.AddString( "RecoverCharacter" );		m_cbRMLog.SetItemData( 17, eRMLog_RecoverCharacter );
	m_cbRMLog.SetCurSel( 0 );
}

void CLogOperPage::OnSelchangeComboGmlog() 
{
	int nSel = m_cbGMLog.GetCurSel();
	int nGMKind = m_cbGMLog.GetItemData( nSel );	

	UpdateListControlFromLogKind( eToolLog_GM, nGMKind );
}

void CLogOperPage::OnSelchangeComboRmlog() 
{
	int nSel = m_cbRMLog.GetCurSel();
	int nRMKind = m_cbRMLog.GetItemData( nSel );	

	UpdateListControlFromLogKind( eToolLog_RM, nRMKind );
}

void CLogOperPage::UpdateListControlFromLogKind( int nLogType, int nLogKind )
{
	m_lcOper.DeleteAllItems();

	char temp[256];	
	LV_ITEM lvitem;

	TOOLLOG* pData = NULL;
	m_ToolLogTable.SetPositionHead();
	while( pData = m_ToolLogTable.GetData() )
	{
		if( pData->dwLogType != nLogType )
			continue;
		else
		{
			if( pData->dwLogKind == nLogKind || nLogKind == 0 )
			{
				lvitem.iItem = m_lcOper.GetItemCount();	// 행
				
				lvitem.mask = LVIF_TEXT;
				lvitem.stateMask = LVIS_SELECTED;
				lvitem.state = INDEXTOSTATEIMAGEMASK(1);
				lvitem.iSubItem = 0;	
				lvitem.pszText = itoa( pData->dwLogIdx, temp, 10 );
				
				m_lcOper.InsertItem( &lvitem );
				m_lcOper.SetItemText( lvitem.iItem, 1, GetLogTypeName( pData->dwLogType, temp ) );
				m_lcOper.SetItemText( lvitem.iItem, 2, GetLogKindName( pData->dwLogType, pData->dwLogKind, temp ) );
				m_lcOper.SetItemText( lvitem.iItem, 3, itoa( pData->dwOperIdx, temp, 10 ) );
				m_lcOper.SetItemText( lvitem.iItem, 4, pData->sOperName );
				m_lcOper.SetItemText( lvitem.iItem, 5, itoa( pData->dwTargetIdx, temp, 10 ) );
				m_lcOper.SetItemText( lvitem.iItem, 6, pData->sTargetName );
				m_lcOper.SetItemText( lvitem.iItem, 7, GetParam1Name( pData->dwLogType, pData->dwLogKind, pData->dwParam1, temp ) );
				m_lcOper.SetItemText( lvitem.iItem, 8, GetParam2Name( pData->dwLogType, pData->dwLogKind, pData->dwParam2, temp ) );
				m_lcOper.SetItemText( lvitem.iItem, 9, GetParam3Name( pData->dwLogType, pData->dwLogKind, pData->dwParam3, temp ) );
				m_lcOper.SetItemText( lvitem.iItem, 10, GetParam4Name( pData->dwLogType, pData->dwLogKind, pData->dwParam4, temp ) );
				m_lcOper.SetItemText( lvitem.iItem, 11, pData->sLogDate );
			}
		}
	}
}

char* CLogOperPage::GetLogTypeName( int nLogType, char* temp )
{
	switch( nLogType )
	{
	case eToolLog_GM:	sprintf( temp, "GMTool" );			break;
	case eToolLog_RM:	sprintf( temp, "RMTool" );			break;
	default:			sprintf( temp, "%d", nLogType );	break;
	}

	return temp;
}

char* CLogOperPage::GetLogKindName( int nLogType, int nLogKind, char* temp )
{
	if( nLogType == eToolLog_GM )
	{
		switch( nLogKind )
		{
		case eGMLog_Move:
			break;
		case eGMLog_Item:
			break;
		case eGMLog_Money:
			break;
		case eGMLog_Skill:
			break;
		case eGMLog_Ability:
			break;
		case eGMLog_Chat_Map:
			break;
		case eGMLog_Chat_User:
			break;
		case eGMLog_PK:
			break;
		case eGMLog_Disconnect_Map:
			break;
		case eGMLog_Disconnect_User:
			break;
		case eGMLog_Block:
			break;
		case eGMLog_Regen:
			break;
		case eGMLog_Event:
			break;
		default:	sprintf( temp, "%d", nLogKind );	break;
		}
	}
	else if( nLogType == eToolLog_RM )
	{
		switch( nLogKind )
		{
		case eRMLog_OperInsert:				sprintf( temp, "OperInsert" );	break;
		case eRMLog_OperDelete:				sprintf( temp, "OperDelete" );	break;
		case eRMLog_IpInsert:				sprintf( temp, "IpInsert" );	break;
		case eRMLog_IpDelete:				sprintf( temp, "IpDelete" );	break;
		case eRMLog_ChangeCharacterInfo:	sprintf( temp, "ChangeCharacterInfo" );	break;
		case eRMLog_ChangeWareInfo:			sprintf( temp, "ChangeWareInfo" );	break;
		case eRMLog_SkillInsert:			sprintf( temp, "SkillInsert" );	break;
		case eRMLog_SkillDelete:			sprintf( temp, "SkillDelete" );	break;
		case eRMLog_SkillUpdate:			sprintf( temp, "SkillUpdate" );	break;
		case eRMLog_AbilityUpdate:			sprintf( temp, "AbilityUpdate" );	break;
		case eRMLog_QuestDelete:			sprintf( temp, "QuestDelete" );	break;
		case eRMLog_ChangeLoginPoint:		sprintf( temp, "ChangeLoginPoint" );	break;
		case eRMLog_ChangeItemPosition:		sprintf( temp, "ChangeItemPosition" );	break;
		case eRMLog_ChangeWareItemPosition:	sprintf( temp, "ChangeWareItemPosition" );	break;
		case eRMLog_ChangeSkillPosition:	sprintf( temp, "ChangeSkillPosition" );	break;
		case eRMLog_ItemInsert:				sprintf( temp, "ItemInsert" );	break;
		case eRMLog_ItemDelete:				sprintf( temp, "ItemDelete" );	break;
		case eRMLog_ItemUpdate:				sprintf( temp, "ItemUpdate" );	break;
		case eRMLog_ChangeUserLevel:		sprintf( temp, "ChangeUserLevel" );	break;
		case eRMLog_RecoverCharacter:		sprintf( temp, "RecoverCharacter" );	break;
		default:							sprintf( temp, "%d", nLogKind );	break;
		}
	}
	else
		sprintf( temp, "%d", nLogKind );

	return temp;	
}

char* CLogOperPage::GetParam1Name( int nLogType, int nLogKind, DWORD dwParam1, char* temp )
{
	if( nLogType == eToolLog_GM )
	{
		switch( nLogKind )
		{
		case eGMLog_Move:
			break;
		case eGMLog_Item:
			break;
		case eGMLog_Money:
			break;
		case eGMLog_Skill:
			break;
		case eGMLog_Ability:
			break;
		case eGMLog_Chat_Map:
			break;
		case eGMLog_Chat_User:
			break;
		case eGMLog_PK:
			break;
		case eGMLog_Disconnect_Map:
			break;
		case eGMLog_Disconnect_User:
			break;
		case eGMLog_Block:
			break;
		case eGMLog_Regen:
			break;
		case eGMLog_Event:
			break;
		default:	sprintf( temp, "%d", nLogKind );	break;
		}
	}
	else if( nLogType == eToolLog_RM )
	{
		switch( nLogKind )
		{
		case eRMLog_OperInsert:				sprintf( temp, "-" );	break;
		case eRMLog_OperDelete:				sprintf( temp, "Delete Operator" );	break;
		case eRMLog_IpInsert:				sprintf( temp, "-" );	break;
		case eRMLog_IpDelete:				sprintf( temp, "Delete IP" );	break;
		case eRMLog_ChangeCharacterInfo:
			{
				WORD w1 = HIWORD(dwParam1);
				WORD w2 = LOWORD(dwParam1);
				sprintf( temp, "Level:%d / Physique:%d", w1, w2 );
			}
			break;
		case eRMLog_ChangeWareInfo:			sprintf( temp, "WareNum:%d", dwParam1 );	break;
		case eRMLog_SkillInsert:
		case eRMLog_SkillDelete:
		case eRMLog_SkillUpdate:
			{
				SKILLINFO* pSkill = GAMERESRCMNGR->GetSkillInfo( dwParam1 );
				if( pSkill == NULL )	sprintf( temp, "%d", dwParam1 );
				else					sprintf( temp, "%s", pSkill->SkillName );
			}
			break;
		case eRMLog_AbilityUpdate:			sprintf( temp, "-" );	break;
		case eRMLog_QuestDelete:			sprintf( temp, "QuestIdx:%d", dwParam1 );	break;
		case eRMLog_ChangeLoginPoint:		sprintf( temp, "Map:%d", dwParam1 );	break;
		case eRMLog_ChangeItemPosition:		sprintf( temp, "ChangeItemPosition" );	break;
		case eRMLog_ChangeWareItemPosition:	sprintf( temp, "ChangeWareItemPosition" );	break;
		case eRMLog_ChangeSkillPosition:	sprintf( temp, "ChangeSkillPosition" );	break;
		case eRMLog_ItemInsert:
		case eRMLog_ItemDelete:
		case eRMLog_ItemUpdate:
			{
				sprintf( temp, "ItemDBIdx:%d", dwParam1 );
			}
			break;
		case eRMLog_ChangeUserLevel:
			{
				switch( dwParam1 )
				{
				case 1:		sprintf( temp, "UserLevel:God" );	break;
				case 2:		sprintf( temp, "UserLevel:Programer" );	break;
				case 3:		sprintf( temp, "UserLevel:Developer" );	break;
				case 4:		sprintf( temp, "UserLevel:GM" );			break;
				case 5:		sprintf( temp, "UserLevel:SuperUser" );	break;
				case 6:		sprintf( temp, "UserLevel:User" );		break;
				case 7:		sprintf( temp, "UserLevel:ForChinese" );	break;
				case 8:		sprintf( temp, "UserLevel:Temp1" );		break;
				case 9:		sprintf( temp, "UserLevel:Temp2" );		break;
				case 10:	sprintf( temp, "UserLevel:Blocked User" );	break;
				default:	sprintf( temp, "UserLevel:%d", dwParam1 );	break;
				}
			}
			break;
		case eRMLog_RecoverCharacter:		sprintf( temp, "-" );	break;
		default:							sprintf( temp, "%d", nLogKind );	break;
		}
	}
	else
		sprintf( temp, "%d", nLogKind );

	return temp;
}

char* CLogOperPage::GetParam2Name( int nLogType, int nLogKind, DWORD dwParam2, char* temp )
{
	if( nLogType == eToolLog_GM )
	{
		switch( nLogKind )
		{
		case eGMLog_Move:
			break;
		case eGMLog_Item:
			break;
		case eGMLog_Money:
			break;
		case eGMLog_Skill:
			break;
		case eGMLog_Ability:
			break;
		case eGMLog_Chat_Map:
			break;
		case eGMLog_Chat_User:
			break;
		case eGMLog_PK:
			break;
		case eGMLog_Disconnect_Map:
			break;
		case eGMLog_Disconnect_User:
			break;
		case eGMLog_Block:
			break;
		case eGMLog_Regen:
			break;
		case eGMLog_Event:
			break;
		default:	sprintf( temp, "%d", nLogKind );	break;
		}
	}
	else if( nLogType == eToolLog_RM )
	{
		switch( nLogKind )
		{
		case eRMLog_OperInsert:				sprintf( temp, "-" );	break;
		case eRMLog_OperDelete:				sprintf( temp, "-" );	break;
		case eRMLog_IpInsert:				sprintf( temp, "-" );	break;
		case eRMLog_IpDelete:				sprintf( temp, "-" );	break;
		case eRMLog_ChangeCharacterInfo:
			{
				WORD w1 = HIWORD(dwParam2);
				WORD w2 = LOWORD(dwParam2);
				sprintf( temp, "Agility:%d / Strength:%d", w1, w2 );
			}
			break;
		case eRMLog_ChangeWareInfo:			sprintf( temp, "WareMoney:%d", dwParam2 );	break;
		case eRMLog_SkillInsert:
		case eRMLog_SkillDelete:
		case eRMLog_SkillUpdate:
			{
				sprintf( temp, "SkillPos:%d", dwParam2 );
			}
			break;
		case eRMLog_AbilityUpdate:			sprintf( temp, "-" );	break;
		case eRMLog_QuestDelete:			sprintf( temp, "-" );	break;
		case eRMLog_ChangeLoginPoint:		sprintf( temp, "-" );	break;
		case eRMLog_ChangeItemPosition:		sprintf( temp, "ChangeItemPosition" );	break;
		case eRMLog_ChangeWareItemPosition:	sprintf( temp, "ChangeWareItemPosition" );	break;
		case eRMLog_ChangeSkillPosition:	sprintf( temp, "ChangeSkillPosition" );	break;
		case eRMLog_ItemInsert:	
		case eRMLog_ItemDelete:
		case eRMLog_ItemUpdate:
			{
				ITEM_INFO* pItem = GAMERESRCMNGR->GetItemInfo( dwParam2 );
				if( pItem == NULL )	sprintf( temp, "%d", dwParam2 );
				else				sprintf( temp, "%s", pItem->ItemName );
			}
			break;
		case eRMLog_ChangeUserLevel:		sprintf( temp, "-" );	break;
		case eRMLog_RecoverCharacter:		sprintf( temp, "-" );	break;
		default:							sprintf( temp, "%d", nLogKind );	break;
		}
	}
	else
		sprintf( temp, "%d", nLogKind );

	return temp;
}

char* CLogOperPage::GetParam3Name( int nLogType, int nLogKind, DWORD dwParam3, char* temp )
{
	if( nLogType == eToolLog_GM )
	{
		switch( nLogKind )
		{
		case eGMLog_Move:
			break;
		case eGMLog_Item:
			break;
		case eGMLog_Money:
			break;
		case eGMLog_Skill:
			break;
		case eGMLog_Ability:
			break;
		case eGMLog_Chat_Map:
			break;
		case eGMLog_Chat_User:
			break;
		case eGMLog_PK:
			break;
		case eGMLog_Disconnect_Map:
			break;
		case eGMLog_Disconnect_User:
			break;
		case eGMLog_Block:
			break;
		case eGMLog_Regen:
			break;
		case eGMLog_Event:
			break;
		default:	sprintf( temp, "%d", nLogKind );	break;
		}
	}
	else if( nLogType == eToolLog_RM )
	{
		switch( nLogKind )
		{
		case eRMLog_OperInsert:				sprintf( temp, "-" );	break;
		case eRMLog_OperDelete:				sprintf( temp, "-" );	break;
		case eRMLog_IpInsert:				sprintf( temp, "-" );	break;
		case eRMLog_IpDelete:				sprintf( temp, "-" );	break;
		case eRMLog_ChangeCharacterInfo:
			{
				WORD w1 = HIWORD(dwParam3);
				WORD w2 = LOWORD(dwParam3);
				sprintf( temp, "Heart:%d / Point:%d", w1, w2 );
			}
			break;
		case eRMLog_ChangeWareInfo:			sprintf( temp, "-" );	break;
		case eRMLog_SkillInsert:
		case eRMLog_SkillDelete:
		case eRMLog_SkillUpdate:
			{
				sprintf( temp, "SkillLevel:%d", dwParam3 );
			}
			break;
		case eRMLog_AbilityUpdate:			sprintf( temp, "-" );	break;
		case eRMLog_QuestDelete:			sprintf( temp, "-" );	break;
		case eRMLog_ChangeLoginPoint:		sprintf( temp, "-" );	break;
		case eRMLog_ChangeItemPosition:		sprintf( temp, "ChangeItemPosition" );	break;
		case eRMLog_ChangeWareItemPosition:	sprintf( temp, "ChangeWareItemPosition" );	break;
		case eRMLog_ChangeSkillPosition:	sprintf( temp, "ChangeSkillPosition" );	break;
		case eRMLog_ItemInsert:
		case eRMLog_ItemUpdate:
			{
				sprintf( temp, "ItemPos:%d", dwParam3 );
			}
			break;
		case eRMLog_ItemDelete:				sprintf( temp, "-" );	break;
		case eRMLog_ChangeUserLevel:		sprintf( temp, "-" );	break;
		case eRMLog_RecoverCharacter:		sprintf( temp, "-" );	break;
		default:							sprintf( temp, "%d", nLogKind );	break;
		}
	}
	else
		sprintf( temp, "%d", nLogKind );

	return temp;
}

char* CLogOperPage::GetParam4Name( int nLogType, int nLogKind, DWORD dwParam4, char* temp )
{
	if( nLogType == eToolLog_GM )
	{
		switch( nLogKind )
		{
		case eGMLog_Move:
			break;
		case eGMLog_Item:
			break;
		case eGMLog_Money:
			break;
		case eGMLog_Skill:
			break;
		case eGMLog_Ability:
			break;
		case eGMLog_Chat_Map:
			break;
		case eGMLog_Chat_User:
			break;
		case eGMLog_PK:
			break;
		case eGMLog_Disconnect_Map:
			break;
		case eGMLog_Disconnect_User:
			break;
		case eGMLog_Block:
			break;
		case eGMLog_Regen:
			break;
		case eGMLog_Event:
			break;
		default:	sprintf( temp, "%d", nLogKind );	break;
		}
	}
	else if( nLogType == eToolLog_RM )
	{
		switch( nLogKind )
		{
		case eRMLog_OperInsert:				sprintf( temp, "-" );	break;
		case eRMLog_OperDelete:				sprintf( temp, "-" );	break;
		case eRMLog_IpInsert:				sprintf( temp, "-" );	break;
		case eRMLog_IpDelete:				sprintf( temp, "-" );	break;
		case eRMLog_ChangeCharacterInfo:
			{
				WORD w1 = HIWORD(dwParam4);
				WORD w2 = LOWORD(dwParam4);
				sprintf( temp, "Money:%d / AbilityPoint:%d", w1, w2 );
			}
			break;
		case eRMLog_ChangeWareInfo:			sprintf( temp, "-" );	break;
		case eRMLog_SkillInsert:			
		case eRMLog_SkillDelete:
		case eRMLog_SkillUpdate:
			{
				sprintf( temp, "SkillExp:%d", dwParam4 );
			}
			break;
		case eRMLog_AbilityUpdate:			sprintf( temp, "-" );	break;
		case eRMLog_QuestDelete:			sprintf( temp, "-" );	break;
		case eRMLog_ChangeLoginPoint:		sprintf( temp, "-" );	break;
		case eRMLog_ChangeItemPosition:		sprintf( temp, "ChangeItemPosition" );	break;
		case eRMLog_ChangeWareItemPosition:	sprintf( temp, "ChangeWareItemPosition" );	break;
		case eRMLog_ChangeSkillPosition:	sprintf( temp, "ChangeSkillPosition" );	break;
		case eRMLog_ItemInsert:	
		case eRMLog_ItemUpdate:
			{
				sprintf( temp, "ItemDur:%d", dwParam4 );
			}
			break;
		case eRMLog_ItemDelete:				sprintf( temp, "ItemDelete" );	break;
		case eRMLog_ChangeUserLevel:		sprintf( temp, "-" );	break;
		case eRMLog_RecoverCharacter:		sprintf( temp, "-" );	break;
		default:							sprintf( temp, "%d", nLogKind );	break;
		}
	}
	else
		sprintf( temp, "%d", nLogKind );

	return temp;
}

void CLogOperPage::ChangeFont()
{
	m_Font.CreatePointFont(100, "Arial");
	m_lcOper.SetFont( &m_Font );
}