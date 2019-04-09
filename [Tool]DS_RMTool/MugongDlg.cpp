// MugongDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DS_RMTool.h"
#include "MugongDlg.h"
//#include "GameResourceManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMugongDlg dialog


CMugongDlg::CMugongDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMugongDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMugongDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	memset(&m_Mugong, 0, sizeof(m_Mugong));

	m_wPos = 0;
	m_Field = eIF_MUGONG;
	m_wLevel = 99;
	m_nCheckValue = -1;

	int i;
	for(i=0;i<emMugong_Max;++i)
		m_BasicList[i] = NULL;
}


void CMugongDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMugongDlg)
	DDX_Control(pDX, IDC_BUTTON_MU_DEL, m_btnDel);
	DDX_Control(pDX, IDC_BUTTON_MU_EDIT, m_btnEdit);
	DDX_Control(pDX, IDC_COMBO_MUTYPE, m_cbType);
	DDX_Control(pDX, IDC_RADIO_WMUGONG, m_rdMugong);
	DDX_Control(pDX, IDC_LIST_BASIC, m_listBasic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMugongDlg, CDialog)
	//{{AFX_MSG_MAP(CMugongDlg)
	ON_BN_CLICKED(IDC_RADIO_IMUGONG, OnRadioImugong)
	ON_BN_CLICKED(IDC_RADIO_WMUGONG, OnRadioWmugong)
	ON_CBN_SELCHANGE(IDC_COMBO_MUTYPE, OnSelchangeComboMutype)
	ON_BN_CLICKED(IDC_BUTTON_MU_EDIT, OnButtonMuEdit)
	ON_BN_CLICKED(IDC_BUTTON_MU_DEL, OnButtonMuDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMugongDlg message handlers

void CMugongDlg::Init( INFOFIELD efield, MUGONGBASE* pMugong, WORD wPos, LEVELTYPE level )
{
	m_Field = efield;
	m_wPos = wPos;

	m_wLevel = level;

	memcpy(&m_Mugong, pMugong, sizeof(MUGONGBASE));

//	if( efield == eIF_MUGONG && m_wPos == 0 )
//		m_Mugong.bWear = 1;

//	if( efield == eIF_JINBUB && m_wPos == 21 )
//		m_Mugong.bWear = 1;

}


BOOL CMugongDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	char str[256] = {0,};
/////////////////////////////////////////////////////////////////////////////////////////
//// 기본 데이터 ////
	itoa(m_Mugong.dwDBIdx, str, 10);
	m_BasicList[emMugong_DBidx] = new CPropertyItem("DBIndex",str,PIT_NONE,"");
	m_listBasic.AddPropItem(m_BasicList[emMugong_DBidx]);

	SKILLINFO* pInfo = GAMERESRCMNGR->GetSkillInfo(m_Mugong.wIconIdx);
	if( pInfo == NULL )
	{
		itoa(m_Mugong.wIconIdx, str, 10);
		m_BasicList[emMugong_Idx] = new CPropertyItem("Skill",str,PIT_COMBO,"", FALSE, CBT_MUGONG);
	}
	else
		m_BasicList[emMugong_Idx] = new CPropertyItem("Skill",pInfo->SkillName,PIT_COMBO,"", FALSE, CBT_MUGONG);

	m_listBasic.AddPropItem(m_BasicList[emMugong_Idx]);

	itoa(m_Mugong.Position, str, 10);
	m_BasicList[emMugong_Pos] = new CPropertyItem("Pos",str,PIT_NONE,"");
	m_listBasic.AddPropItem(m_BasicList[emMugong_Pos]);

	itoa(m_Mugong.ExpPoint, str, 10);
	if( m_Mugong.wIconIdx != 0 )
		m_BasicList[emMugong_Exp] = new CPropertyItem("Exp", str, PIT_EDIT, "", TRUE, CBT_NONE, 999999999);
	else
		m_BasicList[emMugong_Exp] = new CPropertyItem("Exp", str, PIT_NONE, "", TRUE, CBT_NONE, 999999999);

	m_listBasic.AddPropItem(m_BasicList[emMugong_Exp]);

	itoa(m_Mugong.Sung, str, 10);
	CString buff;
	char temp[10];
	for(int i=0;i<13;++i)
	{
		buff += itoa(i, temp, 10);
		buff += "|";
	}

	if( m_Mugong.wIconIdx != 0 )
		m_BasicList[emMugong_Sung] = new CPropertyItem("Level",str,PIT_COMBO, buff);
	else
		m_BasicList[emMugong_Sung] = new CPropertyItem("Level",str,PIT_NONE, buff);

	m_listBasic.AddPropItem(m_BasicList[emMugong_Sung]);

	itoa(m_Mugong.bWear, str, 10);
	m_BasicList[emMugong_bWear] = new CPropertyItem("Weared",str,PIT_NONE,"");
	m_listBasic.AddPropItem(m_BasicList[emMugong_bWear]);

	itoa(m_Mugong.QuickPosition, str, 10);
	m_BasicList[emMugong_QuickPos] = new CPropertyItem("Qpos",str,PIT_NONE,"");
	m_listBasic.AddPropItem(m_BasicList[emMugong_QuickPos]);
//////////////////////////////////////////////////////////////////////////////////////////////////

	SetMugongTypeControl();

	if( m_Mugong.dwDBIdx == 0 )
		GetDlgItem(IDC_BUTTON_MU_DEL)->EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CMugongDlg::SetMugongTypeControl()
{
	if( m_Field == eIF_JINBUB )
	{
		EnableRadio(FALSE);
		m_nCheckValue = 4;
		SetMugongCombo(0);
	}
	else
	{
		EnableRadio(TRUE);
		((CButton*)GetDlgItem(IDC_RADIO_WMUGONG))->SetCheck(BST_CHECKED);

		m_nCheckValue = 1;
		SetMugongTypeCombo();
	}
}

void CMugongDlg::SetMugongTypeCombo(WORD Def)
{
	m_cbType.ResetContent();
	int nIndex = 0;

	switch( m_nCheckValue )
	{
	case 1: // 외공
		{
			m_cbType.AddString( "Sword" );		m_cbType.SetItemData( 0, WP_GUM );
			m_cbType.AddString( "Glove" );		m_cbType.SetItemData( 1, WP_GWUN );
			m_cbType.AddString( "Broad Sword" );		m_cbType.SetItemData( 2, WP_DO );
			m_cbType.AddString( "Spear" );		m_cbType.SetItemData( 3, WP_CHANG );
			m_cbType.AddString( "Bow" );		m_cbType.SetItemData( 4, WP_GUNG );
			m_cbType.AddString( "Throwing Star" );	m_cbType.SetItemData( 5, WP_AMGI );
		}
		break;
	case 2: // 내공
		{
			m_cbType.AddString( "Fire" );		m_cbType.SetItemData( 0, WP_FIRE );
			m_cbType.AddString( "Water" );		m_cbType.SetItemData( 1, WP_WATER );
			m_cbType.AddString( "Tree" );		m_cbType.SetItemData( 2, WP_TREE );
			m_cbType.AddString( "Iron" );		m_cbType.SetItemData( 3, WP_GOLD );
			m_cbType.AddString( "Earth" );		m_cbType.SetItemData( 4, WP_EARTH );
			m_cbType.AddString( "None" );		m_cbType.SetItemData( 5, WP_NONEATTR );
		}
		break;
	default:
		{
			ASSERT(0);
		}
		return;
	}

	m_cbType.SetCurSel(Def);
	SetMugongCombo(m_cbType.GetItemData(Def));
}

void CMugongDlg::SetMugongCombo(DWORD type)
{
	CYHHashTable<SKILLINFO>* pSkillInfoList = GAMERESRCMNGR->GetSkillInfo();
	SKILLINFO* pMugong = NULL;
	int i = 0;

	m_BasicList[emMugong_Idx]->ClearItemsEx();

	if( m_nCheckValue == 1 ) // 외공
	{
		pSkillInfoList->SetPositionHead();
		while(pMugong = pSkillInfoList->GetData())
		{
			if( pMugong->SkillKind == m_nCheckValue && pMugong->WeaponKind == type )
			{
				if( pMugong->RestrictLevel > m_wLevel )
					continue;

				m_BasicList[emMugong_Idx]->m_cmbItemsEx[i] = pMugong->SkillIdx;
				++i;
			}
		}
	}
	else if( m_nCheckValue == 2 ) // 내공
	{
		pSkillInfoList->SetPositionHead();
		while(pMugong = pSkillInfoList->GetData())
		{
			if( pMugong->SkillKind == m_nCheckValue && pMugong->Attrib == type )
			{
				if( pMugong->RestrictLevel > m_wLevel )
					continue;

				m_BasicList[emMugong_Idx]->m_cmbItemsEx[i] = pMugong->SkillIdx;
				++i;
			}
		}
	}
	else if( m_nCheckValue == 4) // 진법
	{
		pSkillInfoList->SetPositionHead();
		while(pMugong = pSkillInfoList->GetData())
		{
			if( pMugong->SkillKind == m_nCheckValue )
			{
				if( pMugong->RestrictLevel > m_wLevel )
					continue;

				m_BasicList[emMugong_Idx]->m_cmbItemsEx[i] = pMugong->SkillIdx;
				++i;
			}
		}
	}
	else
	{
		ASSERT(0);
		return;
	}
}

void CMugongDlg::EnableRadio(BOOL bEnable)
{
	GetDlgItem(IDC_RADIO_WMUGONG)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_IMUGONG)->EnableWindow(bEnable);

	m_cbType.EnableWindow(bEnable);
}

void CMugongDlg::OnRadioImugong() 
{
	// TODO: Add your control notification handler code here
	m_nCheckValue = 2;
	SetMugongTypeCombo();
}

void CMugongDlg::OnRadioWmugong() 
{
	// TODO: Add your control notification handler code here
	m_nCheckValue = 1;
	SetMugongTypeCombo();
}

void CMugongDlg::OnSelchangeComboMutype() 
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_cbType.GetCurSel();
	if( nCurSel == -1 ) return;

	SetMugongCombo(m_cbType.GetItemData(nCurSel));	
}

void CMugongDlg::OnButtonMuEdit() 
{
	// TODO: Add your control notification handler code here
	WORD wIdx = GAMERESRCMNGR->GetMugongIdx_ByMugongName( m_BasicList[emMugong_Idx]->m_curValue.GetBuffer(0) );
	if( wIdx == 0 )
		EndDialog(eMR_Cancel);

	m_Mugong.wIconIdx = wIdx;
	m_Mugong.ExpPoint = (EXPTYPE)atoi(m_BasicList[emMugong_Exp]->m_curValue.GetBuffer(0));
	m_Mugong.Sung = (BYTE)atoi(m_BasicList[emMugong_Sung]->m_curValue.GetBuffer(0));

	EndDialog(eMR_Edit);
}

void CMugongDlg::OnButtonMuDel() 
{
	// TODO: Add your control notification handler code here
	memset(&m_Mugong, 0, sizeof(MUGONGBASE));
	
	EndDialog(eMR_Delete);	
}

void CMugongDlg::OnOK()
{
}

void CMugongDlg::OnCancel()
{
	EndDialog(eMR_Cancel);
}

LRESULT CMugongDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( message == WM_CHANGEDATA )
	{
		CPropertyItem* pItem = (CPropertyItem*)wParam;

		if( strcmp(pItem->m_propName.GetBuffer(0),"Mugong") == 0  )
		{
			if( GAMERESRCMNGR->GetMugongIdx_ByMugongName( m_BasicList[emMugong_Idx]->m_curValue.GetBuffer(0) ) )
			{
				m_BasicList[emMugong_Exp]->m_curValue = "0";
				m_BasicList[emMugong_Exp]->m_nItemType = PIT_EDIT;

				m_BasicList[emMugong_Sung]->m_nItemType = PIT_COMBO;
				m_BasicList[emMugong_Sung]->m_curValue = "0";
			}
		}

		if( strcmp(pItem->m_propName.GetBuffer(0),"Sung") == 0 )
		{
			m_BasicList[emMugong_Exp]->m_curValue = "0";

			if( (DWORD)lParam >= 7 )
				m_BasicList[emMugong_Exp]->m_nItemType = PIT_NONE;
			else
				m_BasicList[emMugong_Exp]->m_nItemType = PIT_EDIT;
		}
		else if( strcmp(pItem->m_propName.GetBuffer(0),"Exp") == 0 )
		{
			WORD idx = GAMERESRCMNGR->GetMugongIdx_ByMugongName(m_BasicList[emMugong_Idx]->m_curValue.GetBuffer(0));
			WORD sung = (WORD)atoi(m_BasicList[emMugong_Sung]->m_curValue.GetBuffer(0));

			SKILLINFO* pSkillInfo = GAMERESRCMNGR->GetSkillInfo(idx);
			if( pSkillInfo == NULL || sung >= 12)
			{
				pItem->m_curValue = "0";
				return TRUE;
			}

			if( pSkillInfo->NeedExp[sung] <= (DWORD)lParam && lParam >= 0 )
				pItem->m_curValue.Format("%d", pSkillInfo->NeedExp[sung]);
		}
		else if( strcmp(pItem->m_propName.GetBuffer(0),"Mugong") == 0 )
		{
			m_BasicList[emMugong_Exp]->m_curValue = "0";
			m_BasicList[emMugong_Sung]->m_curValue = "0";
		}
	}
	
	return CDialog::DefWindowProc(message, wParam, lParam);
}

BOOL CMugongDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	int i;
	for(i=0;i<emMugong_Max;++i)
		SAFE_DELETE( m_BasicList[i] );

	return CDialog::DestroyWindow();
}
