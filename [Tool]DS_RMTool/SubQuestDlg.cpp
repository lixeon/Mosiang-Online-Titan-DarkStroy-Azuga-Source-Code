// SubQuestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ds_rmtool.h"
#include "SubQuestDlg.h"
#include "RMNetwork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSubQuestDlg dialog


CSubQuestDlg::CSubQuestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSubQuestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSubQuestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_dwDocumentIdx = 0;
	m_dwCharIdx = 0;
	m_dwQuestIdx = 0;
	m_bInit = FALSE;
}


void CSubQuestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSubQuestDlg)
	DDX_Control(pDX, IDC_SUBQUEST_LISTCTRL, m_lcSubQuest);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSubQuestDlg, CDialog)
	//{{AFX_MSG_MAP(CSubQuestDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSubQuestDlg message handlers

BOOL CSubQuestDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InitSubQuestListCtrl();
	
	m_bInit = TRUE;

	SendSubQuestInfo();
	
	m_Font.CreatePointFont(100, "Arial");
	m_lcSubQuest.SetFont( &m_Font );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSubQuestDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_dwDocumentIdx = 0;
	m_dwCharIdx = 0;
	m_dwQuestIdx = 0;
	m_bInit = FALSE;	

	m_Font.DeleteObject();
}

void CSubQuestDlg::SetInitData( DWORD dwDocumentIdx, DWORD dwCharIdx, DWORD dwQuestIdx )
{
	m_dwDocumentIdx = dwDocumentIdx;
	m_dwCharIdx = dwCharIdx;
	m_dwQuestIdx = dwQuestIdx;
}

void CSubQuestDlg::InitSubQuestListCtrl()
{
	char* tcolumn[2] = { "SubQuestIdx", "SubQuestTitle" };
	int tcolumnsize[2] = { 80, 160 };
	
	// listctrl column 설정
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_CENTER;
	for( int i = 0; i < 2; ++i )
	{
		lvcolumn.iSubItem = i;
		lvcolumn.cx = tcolumnsize[i];
		lvcolumn.pszText = tcolumn[i];
		m_lcSubQuest.InsertColumn( i, &lvcolumn );
	}
	// style
	DWORD dwExStyle = m_lcSubQuest.GetExtendedStyle();
	dwExStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_lcSubQuest.SetExtendedStyle( dwExStyle );
}

void CSubQuestDlg::UpdateSubQuestListCtrl( TMSG_SUBQUESTINFO* pMsg )
{
	m_lcSubQuest.DeleteAllItems();

	char temp[256];
	LV_ITEM lvitem;

	for( int i = 0; i < pMsg->dwCount; ++i )
	{
		lvitem.iItem = m_lcSubQuest.GetItemCount();	// 행
		
		lvitem.iSubItem = 0;
		lvitem.mask = LVIF_TEXT;
		lvitem.stateMask = LVIS_SELECTED;
		lvitem.state = INDEXTOSTATEIMAGEMASK(1);
		lvitem.pszText = itoa( pMsg->SubQuestInfo[i].dwSubQuestIdx, temp, 10 );		
		m_lcSubQuest.InsertItem( &lvitem );

		//
		strcpy( temp, GAMERESRCMNGR->GetSubQuestTitle( pMsg->SubQuestInfo[i].dwQuestIdx, pMsg->SubQuestInfo[i].dwSubQuestIdx ) );
		m_lcSubQuest.SetItemText( lvitem.iItem, 1, temp );
	}
}

void CSubQuestDlg::SendSubQuestInfo()
{
	TMSG_DWORD2 Msg;
	Msg.Category = MP_RMTOOL_CHARACTER;
	Msg.Protocol = MP_RMTOOL_CHARACTER_SUBQUESTINFO_SYN;
	Msg.dwTemplateIdx = m_dwDocumentIdx;
	Msg.dwData1 = m_dwCharIdx;	//캐릭터 인덱스
	Msg.dwData2 = m_dwQuestIdx;

	NETWORK->Send( &Msg, sizeof(TMSG_DWORD2) );
}