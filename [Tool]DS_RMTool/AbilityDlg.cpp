// AbilityDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DS_RMTool.h"
#include "AbilityDlg.h"
#include "..\[CC]Ability\AbilityInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAbilityDlg dialog


CAbilityDlg::CAbilityDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAbilityDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAbilityDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pAbility = NULL;
	m_bAbilityLevel = 0;
	m_wPos = 0;

	int i;
	for(i=0;i<emAbility_Max;++i)
		m_BasicList[i] = NULL;

}

void CAbilityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAbilityDlg)
	DDX_Control(pDX, IDC_BUTTON_AB_EDIT, m_btnEdit);
	DDX_Control(pDX, IDC_LIST_ABILITY, m_listAbility);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAbilityDlg, CDialog)
	//{{AFX_MSG_MAP(CAbilityDlg)
	ON_BN_CLICKED(IDC_BUTTON_AB_EDIT, OnButtonAbEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAbilityDlg message handlers

void CAbilityDlg::Init( WORD wPos, CAbilityInfo* pAbility, BYTE bAbilityLevel )
{
	m_pAbility = pAbility;
	m_bAbilityLevel = bAbilityLevel;
	m_wPos = wPos;
}

LRESULT CAbilityDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

	if( message == WM_CHANGEDATA )
	{
		CPropertyItem* pItem = (CPropertyItem*)wParam;
	}
	
	return CDialog::DefWindowProc(message, wParam, lParam);
}

BOOL CAbilityDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	int i;
	for(i=0;i<emAbility_Max;++i)
		SAFE_DELETE( m_BasicList[i] );
	
	return CDialog::DestroyWindow();
}

BOOL CAbilityDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	char str[256] = {0,};
/////////////////////////////////////////////////////////////////////////////////////////
//// 기본 데이터 ////
	emAbility_Idx, emAbility_Name, emAbility_Level, emAbility_Max,
		
	itoa(m_pAbility->GetIdx(), str, 10);
	m_BasicList[emAbility_Idx] = new CPropertyItem("AbilityIdx",str,PIT_NONE,"");
	m_listAbility.AddPropItem(m_BasicList[emAbility_Idx]);

	m_BasicList[emAbility_Name] = new CPropertyItem("AbilityName",m_pAbility->GetInfo()->Ability_name,PIT_NONE,"");
	m_listAbility.AddPropItem(m_BasicList[emAbility_Name]);

	CString buff;
	char temp[10];
	for(int i=0;i<MAX_ABILITY_LEVEL+1;++i)
	{
		if( m_pAbility->GetAbilityGradeExp(i) == -1 ) 
			break;

		buff += itoa(i, temp, 10);
		buff += "|";
	}
	
	itoa( m_bAbilityLevel, str, 10);
	m_BasicList[emAbility_Level] = new CPropertyItem("AbilityLevel",str,PIT_COMBO,buff);
	m_listAbility.AddPropItem(m_BasicList[emAbility_Level]);


	if( m_pAbility->GetAbilityGradeExp(1) == -1 )
		m_BasicList[emAbility_Level]->m_nItemType = PIT_NONE;

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAbilityDlg::OnButtonAbEdit() 
{
	// TODO: Add your control notification handler code here
	m_bAbilityLevel = (BYTE)atoi(m_BasicList[emAbility_Level]->m_curValue.GetBuffer(0));
	EndDialog(eMR_Edit);
}

void CAbilityDlg::OnOK()
{
}

void CAbilityDlg::OnCancel()
{
	EndDialog(eMR_Cancel);
}
