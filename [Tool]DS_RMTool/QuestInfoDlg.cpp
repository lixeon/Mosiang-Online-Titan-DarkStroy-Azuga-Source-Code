// QuestInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ds_rmtool.h"
#include "QuestInfoDlg.h"
#include "RMNetwork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQuestInfoDlg dialog


CQuestInfoDlg::CQuestInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQuestInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQuestInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_dwDocumentIdx = 0;
	m_dwCharIdx = 0;
	memset( m_sCharName, 0, 32 );
	m_bInit = FALSE;
}


void CQuestInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQuestInfoDlg)
	DDX_Control(pDX, IDC_QUEST_DELETE, m_btnQuestDelete);
	DDX_Control(pDX, IDC_QUEST_CHARNAME, m_etCharName);
	DDX_Control(pDX, IDC_QUEST_LISTCTRL, m_lcQuest);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CQuestInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CQuestInfoDlg)
	ON_BN_CLICKED(IDC_QUEST_DELETE, OnQuestDelete)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_QUEST_LISTCTRL, OnClickQuestListctrl)
	ON_NOTIFY(NM_DBLCLK, IDC_QUEST_LISTCTRL, OnDblclkQuestListctrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQuestInfoDlg message handlers

BOOL CQuestInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitQuestListCtrl();
	m_etCharName.SetWindowText( m_sCharName );
	m_btnQuestDelete.EnableWindow( FALSE );
	
	m_bInit = TRUE;
	
	SendMainQuestInfo();

	m_Font.CreatePointFont(100, "Arial");
	m_lcQuest.SetFont( &m_Font );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CQuestInfoDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	m_dwCharIdx = 0;
	memset( m_sCharName, 0, 32 );
	m_bInit = FALSE;
	
	m_Font.DeleteObject();
}

void CQuestInfoDlg::SetInitData( DWORD dwDocumentIdx, DWORD dwCharIdx, char* sCharName )
{
	m_dwDocumentIdx = dwDocumentIdx;
	m_dwCharIdx = dwCharIdx;
	strcpy( m_sCharName, sCharName );
}

void CQuestInfoDlg::InitQuestListCtrl()
{
	char* tcolumn[4] = { "QuestIdx", "QuestTitle", "QuestState", "Date" };
	int tcolumnsize[4] = { 60, 180, 80, 120 };
	
	// listctrl column 설정
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_CENTER;
	for( int i = 0; i < 4; ++i )
	{
		lvcolumn.iSubItem = i;
		lvcolumn.cx = tcolumnsize[i];
		lvcolumn.pszText = tcolumn[i];
		m_lcQuest.InsertColumn( i, &lvcolumn );
	}
	// style
	DWORD dwExStyle = m_lcQuest.GetExtendedStyle();
	dwExStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_lcQuest.SetExtendedStyle( dwExStyle );
}

void CQuestInfoDlg::UpdateQuestListCtrl( TMSG_MAINQUESTINFO* pMsg )
{
	m_lcQuest.DeleteAllItems();

	char temp[256];
	LV_ITEM lvitem;

	for( int i = 0; i < pMsg->dwCount; ++i )
	{
		lvitem.iItem = m_lcQuest.GetItemCount();	// 행
		
		lvitem.iSubItem = 0;
		lvitem.mask = LVIF_TEXT;
		lvitem.stateMask = LVIS_SELECTED;
		lvitem.state = INDEXTOSTATEIMAGEMASK(1);
		lvitem.pszText = itoa( pMsg->MainQuestInfo[i].dwQuestIdx, temp, 10 );		
		m_lcQuest.InsertItem( &lvitem );

		//
		strcpy( temp, GAMERESRCMNGR->GetQuestTitle( pMsg->MainQuestInfo[i].dwQuestIdx ) );
		m_lcQuest.SetItemText( lvitem.iItem, 1, temp );

		//
		if( pMsg->MainQuestInfo[i].dwEndParam == 0 )
			sprintf( temp, "Proc" );
		else
			sprintf( temp, "End" );
		m_lcQuest.SetItemText( lvitem.iItem, 2, temp );

		//
		stTIME time;
		time.SetTime( pMsg->MainQuestInfo[i].dwRegDate );
		sprintf( temp, "%4d-%02d-%02d %02d:%02d", 2000+time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute() );
		m_lcQuest.SetItemText( lvitem.iItem, 3, temp );
	}
}

void CQuestInfoDlg::SendMainQuestInfo()
{
	TMSG_DWORD Msg;
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_MAINQUESTINFO_SYN;
	Msg.dwTemplateIdx = m_dwDocumentIdx;
	Msg.dwData = m_dwCharIdx;	//캐릭터 인덱스

	NETWORK->Send( &Msg, sizeof(TMSG_DWORD) );
}

void CQuestInfoDlg::OnClickQuestListctrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_Pos = m_lcQuest.GetFirstSelectedItemPosition();
	if( m_Pos )
		m_btnQuestDelete.EnableWindow( TRUE );
	else
		m_btnQuestDelete.EnableWindow( FALSE );
	
	*pResult = 0;
}


void CQuestInfoDlg::OnQuestDelete() 
{
	if( m_Pos )
	{
		int index = m_lcQuest.GetNextSelectedItem( m_Pos );		
		CString str = m_lcQuest.GetItemText( index, 0 );

		char temp[256];
		sprintf( temp, "Are You Really Delete Quest %d?", atoi( (char*)(LPCTSTR)str ) );

		if( MessageBox( temp, "Quest Delete", MB_OKCANCEL ) == IDOK )
		{
			TMSG_NAME_DWORD Msg;
			Msg.Category = MP_RMTOOL_CHARACTER;
			Msg.Protocol = MP_RMTOOL_CHARACTER_MAINQUESTDELETE;
			Msg.dwTemplateIdx = m_dwDocumentIdx;
			Msg.dwChrIdx = m_dwCharIdx;
			strcpy( Msg.sName, m_sCharName );
			Msg.dwData = atoi( (char*)(LPCTSTR)str );
			
			NETWORK->Send(&Msg, sizeof(Msg));		
			
			SendMainQuestInfo();
		}
	}		
}

void CQuestInfoDlg::OnDblclkQuestListctrl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POSITION pos = m_lcQuest.GetFirstSelectedItemPosition();
	if( pos )
	{
		int index = m_lcQuest.GetNextSelectedItem( pos );		
		CString str = m_lcQuest.GetItemText( index, 2 );
		if( strcmp( (char*)(LPCTSTR)str, "Proc" ) == 0 )
		{
			str = m_lcQuest.GetItemText( index, 0 );
			m_SubQuestInfoDlg.SetInitData( m_dwDocumentIdx, m_dwCharIdx, atoi( (char*)(LPCTSTR)str ) );
			m_SubQuestInfoDlg.DoModal();
		}
	}
	
	*pResult = 0;
}

void CQuestInfoDlg::UpdateSubQuestData( TMSG_SUBQUESTINFO* pMsg )
{
	if( m_SubQuestInfoDlg.m_bInit )
		m_SubQuestInfoDlg.UpdateSubQuestListCtrl( pMsg );
}