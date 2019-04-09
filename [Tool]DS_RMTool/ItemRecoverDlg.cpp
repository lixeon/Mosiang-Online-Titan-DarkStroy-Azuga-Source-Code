// ItemRecoverDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ds_rmtool.h"
#include "ItemRecoverDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemRecoverDlg dialog


CItemRecoverDlg::CItemRecoverDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CItemRecoverDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CItemRecoverDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nItemIdx = m_nItemNum = 0;
}


void CItemRecoverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CItemRecoverDlg)
	DDX_Control(pDX, IDC_EDIT_ITEM_NUM, m_etItemNum);
	DDX_Control(pDX, IDC_COMBO_ITEM, m_cbItem);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CItemRecoverDlg, CDialog)
	//{{AFX_MSG_MAP(CItemRecoverDlg)
	ON_BN_CLICKED(IDC_BUTTON_ITEM_RECOVER, OnButtonItemRecover)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemRecoverDlg message handlers
BOOL CItemRecoverDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_nItemIdx = m_nItemNum = 0;

	InitComboBox();
	m_etItemNum.SetWindowText( 0 );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CItemRecoverDlg::OnButtonItemRecover() 
{
	int i = m_cbItem.GetCurSel();
	m_nItemIdx = m_cbItem.GetItemData( i );
	CString str;
	m_etItemNum.GetWindowText( str );
	m_nItemNum = atoi( (char*)(LPCTSTR)str );

	EndDialog( 99 );
}

void CItemRecoverDlg::InitComboBox()
{
	CYHHashTable<ITEM_INFO>* pItemInfoList = GAMERESRCMNGR->GetItemInfo();
	ITEM_INFO* pItemInfo = NULL;

	int i = 0;
	pItemInfoList->SetPositionHead();
	while( pItemInfo = pItemInfoList->GetData() )
	{
		if( pItemInfo->ItemKind & eEXTRA_ITEM )
		{
			m_cbItem.AddString( pItemInfo->ItemName );
			m_cbItem.SetItemData( i, pItemInfo->ItemIdx );
			++i;
		}
	}

	m_cbItem.SetCurSel( 0 );
}
