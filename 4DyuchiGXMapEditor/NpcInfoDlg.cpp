// NpcInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "4dyuchigxmapeditor.h"
#include "NpcInfoDlg.h"
#include "Tool.h"
#include "VoidList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNpcInfoDlg dialog


CNpcInfoDlg::CNpcInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNpcInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNpcInfoDlg)
	m_strPosZ = _T("");
	m_strPosX = _T("");
	m_strSectionNum = _T("");
	//}}AFX_DATA_INIT

	m_wNpcInfoMany = 0;
	memset( m_NpcID, 0, sizeof( Npc_ID ) * MAX_NPC_INFO );
}


void CNpcInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNpcInfoDlg)
	DDX_Control(pDX, IDC_COMBO_NPC_NUM, m_comboNpc);
	DDX_Control(pDX, IDC_LIST_NPC_INFO, m_ListNpcInfo);
	DDX_Text(pDX, IDC_STATIC_ZPOS, m_strPosZ);
	DDX_Text(pDX, IDC_STATIC_XPOS, m_strPosX);
	DDX_Text(pDX, IDC_STATIC_SECTION_NUM, m_strSectionNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNpcInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CNpcInfoDlg)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_NPC, OnButtonCreateNpc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNpcInfoDlg message handlers
BOOL CNpcInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	FILE* fp;
	fp = fopen( "NpcInfo.txt", "r" );

	if( !fp )
	{
		MessageBox( "NpcInfo.txt 파일을 찾을수 없습니다.", "ㅠ.ㅠ", MB_ICONWARNING );
		return TRUE;
	}
	
	m_wNpcInfoMany = 0;
	Npc_ID	TmpInfo;
	while( m_wNpcInfoMany < MAX_NPC_INFO )
	{
		memset( &TmpInfo, 0, sizeof( Npc_ID ) );
		if( fscanf( fp, "%d %s", &TmpInfo.wNpcID, TmpInfo.szNpcName ) != EOF )
		{
			memcpy( &m_NpcID[ TmpInfo.wNpcID ], &TmpInfo, sizeof( Npc_ID ) );
	
			m_IDAttr[m_wNpcInfoMany] = m_NpcID[TmpInfo.wNpcID].wNpcID;
			m_comboNpc.AddString( m_NpcID[TmpInfo.wNpcID].szNpcName );
			m_wNpcInfoMany++;
		}
		else
		{
			break;
		}
	}
	
	fclose( fp );

	// List control..
	m_ListNpcInfo.DeleteColumn(0);
	m_ListNpcInfo.DeleteColumn(0);
	m_ListNpcInfo.DeleteColumn(0);
	m_ListNpcInfo.DeleteAllItems();
	m_ListNpcInfo.SetExtendedStyle( LVS_EX_FULLROWSELECT );
	
	LV_COLUMN	lvcolumn;
	lvcolumn.mask		= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt		= LVCFMT_LEFT;
	lvcolumn.pszText	= _T( "xPos" );
	lvcolumn.cx			= 50;
	lvcolumn.cchTextMax	= 100;
	m_ListNpcInfo.InsertColumn( 0, &lvcolumn );

	lvcolumn.pszText	= _T( "zPos" );
	lvcolumn.cx			= 50;
	lvcolumn.cchTextMax	= 100;
	m_ListNpcInfo.InsertColumn( 1, &lvcolumn );
	
	lvcolumn.pszText	= _T( "Npc Name" );
	lvcolumn.cx			= 131;
	lvcolumn.cchTextMax	= 100;
	m_ListNpcInfo.InsertColumn( 2, &lvcolumn );
	
	CNpcInfoList*	pNpcInfoList = g_pTool->GetNpcInfoList();
	POSITION_		pos = pNpcInfoList->GetHeadPosition();
	Npc_Info*		pNpcInfo;
	
	char	buffer[50];
	LV_ITEM lvitem;
	lvitem.mask = LVIF_TEXT; 

	while( pos )
	{
		pNpcInfo = (Npc_Info*)pNpcInfoList->GetNext( pos );

		lvitem.iItem	= m_ListNpcInfo.GetItemCount();
		lvitem.iSubItem = 0;
		
		itoa( pNpcInfo->wPosX, buffer, 10 );
		lvitem.pszText = buffer;
		m_ListNpcInfo.InsertItem( &lvitem );
		
		itoa( pNpcInfo->wPosZ, buffer, 10 );
		m_ListNpcInfo.SetItemText( lvitem.iItem , 1, buffer );
		m_ListNpcInfo.SetItemText( lvitem.iItem , 2, m_NpcID[ pNpcInfo->wNpcID ].szNpcName );		
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNpcInfoDlg::OnButtonCreateNpc() 
{
	int nSel = m_comboNpc.GetCurSel();
	
	if( nSel == LB_ERR ) return;
	
	Npc_Info*	pNpcInfo;
	pNpcInfo = new Npc_Info;

	pNpcInfo->wNpcID	= m_IDAttr[ nSel ];
	pNpcInfo->wPosX = atoi( m_strPosX.GetBuffer(0) );
	pNpcInfo->wPosZ = atoi( m_strPosZ.GetBuffer(0) );

	LV_ITEM lvitem;
	lvitem.mask = LVIF_TEXT; 
	lvitem.iItem	= m_ListNpcInfo.GetItemCount();
	lvitem.iSubItem = 0;
		
	lvitem.pszText = m_strPosX.GetBuffer( 0 );
	m_ListNpcInfo.InsertItem( &lvitem );
		
	m_ListNpcInfo.SetItemText( lvitem.iItem , 1, m_strPosZ.GetBuffer( 0 ) );
	m_ListNpcInfo.SetItemText( lvitem.iItem , 2, m_NpcID[ pNpcInfo->wNpcID].szNpcName );
	
	g_pTool->GetNpcInfoList()->AddTail( pNpcInfo );
}
