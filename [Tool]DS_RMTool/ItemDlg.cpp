// ItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DS_RMTool.h"
#include "ItemDlg.h"
//#include "GameResourceManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemDlg dialog


CItemDlg::CItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CItemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	memset(&m_Item, 0, sizeof(m_Item));
	memset(&m_ItemOption, 0, sizeof(m_ItemOption));

	m_wPos = 0;
	m_Field = eIF_INVENTORY;
	m_bOption = FALSE;
	m_nCheckValue = -1;
	m_wLevel = 99;

	int i;
	for(i=0;i<emBasic_Max;++i)
		m_BasicList[i] = NULL;
	for(i=0;i<emBasicEx_Max;++i)
		m_BasicExList[i] = NULL;
	for(i=0;i<emAttr_Max;++i)
		m_AttrList[i] = NULL;
}


void CItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CItemDlg)
	DDX_Control(pDX, IDC_RADIO_WEAPON, m_rdWeapon);
	DDX_Control(pDX, IDC_LIST_EXBASIC, m_listExBasic);
	DDX_Control(pDX, IDC_LIST_EXATTR, m_listExAttr);
	DDX_Control(pDX, IDC_LIST_BASIC, m_listBasic);
	DDX_Control(pDX, IDC_COMBO_EXTYPE, m_cbExType);
	DDX_Control(pDX, IDC_CHECK_EXDATA, m_chExData);
	//}}AFX_DATA_MAP
}

void CItemDlg::Init( INFOFIELD efield, const ITEMBASE* pitem, const ITEM_OPTION_INFO* pOption, WORD pos, WORD wWeartype )
{
	memcpy(&m_Item, pitem, sizeof(ITEMBASE));

	if( pOption == NULL )
	{
		if( GAMERESRCMNGR->IsOptionItem(pitem->wIconIdx, pitem->Durability) )
			ASSERT(0);
		m_bOption = FALSE;
	}
	else
	{
		m_bOption = TRUE;
		memcpy(&m_ItemOption, pOption, sizeof(ITEM_OPTION_INFO));
	}

	m_wPos = pos;
	m_Field = efield;
	m_wWearType = wWeartype;
}

BEGIN_MESSAGE_MAP(CItemDlg, CDialog)
	//{{AFX_MSG_MAP(CItemDlg)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_RADIO_ACCESSORY, OnRadioAccessory)
	ON_BN_CLICKED(IDC_RADIO_DEFFEN, OnRadioDeffen)
	ON_BN_CLICKED(IDC_RADIO_ETC, OnRadioEtc)
	ON_BN_CLICKED(IDC_RADIO_WEAPON, OnRadioWeapon)
	ON_CBN_SELCHANGE(IDC_COMBO_EXTYPE, OnSelchangeComboExtype)
	ON_BN_CLICKED(IDC_CHECK_EXDATA, OnCheckExdata)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemDlg message handlers

BOOL CItemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	char str[256] = {0,};
/////////////////////////////////////////////////////////////////////////////////////////
//// 기본 데이터 ////
	itoa(m_Item.dwDBIdx, str, 10);
	m_BasicList[emBasic_DBIdx] = new CPropertyItem("DBIndex",str,PIT_NONE,"");
	m_listBasic.AddPropItem(m_BasicList[emBasic_DBIdx]);

	ITEM_INFO* pInfo = GAMERESRCMNGR->GetItemInfo(m_Item.wIconIdx);
	if( pInfo == NULL )
	{
		itoa(m_Item.wIconIdx, str, 10);
		m_BasicList[emBasic_Idx] = new CPropertyItem("Item",str,PIT_COMBO,"", FALSE, CBT_ITEM);
	}
	else
		m_BasicList[emBasic_Idx] = new CPropertyItem("Item",pInfo->ItemName,PIT_COMBO,"", FALSE, CBT_ITEM);

	m_listBasic.AddPropItem(m_BasicList[emBasic_Idx]);

	itoa(m_Item.Position, str, 10);
	m_BasicList[emBasic_Position] = new CPropertyItem("Pos",str,PIT_NONE,"");
	m_listBasic.AddPropItem(m_BasicList[emBasic_Position]);

	itoa(m_Item.Durability, str, 10);
	m_BasicList[emBasic_Durability] = new CPropertyItem("Dur",str,PIT_COMBO,"1|2|3|4|5|");
	m_listBasic.AddPropItem(m_BasicList[emBasic_Durability]);

	itoa(m_Item.QuickPosition, str, 10);
	m_BasicList[emBasic_Quick] = new CPropertyItem("Qpos",str,PIT_NONE,"");
	m_listBasic.AddPropItem(m_BasicList[emBasic_Quick]);
//////////////////////////////////////////////////////////////////////////////////////////////////

	if( m_Item.wIconIdx != 0 && GAMERESRCMNGR->IsDupItem(m_Item.wIconIdx) )
		m_BasicList[emBasic_Durability]->m_nItemType = PIT_COMBO;
	else
		m_BasicList[emBasic_Durability]->m_nItemType = PIT_NONE;

/////////////////////////////////////////////////////////////////////////////////////////
//// 옵션 기본 데이터 ////
	itoa(m_ItemOption.dwOptionIdx, str, 10);
	m_BasicExList[emBasicEx_OptionIdx] = new CPropertyItem("OpIndex",str,PIT_NONE,"");
	m_listExBasic.AddPropItem(m_BasicExList[emBasicEx_OptionIdx]);

	itoa(m_ItemOption.dwItemDBIdx, str, 10);
	m_BasicExList[emBasicEx_ItemDBIdx] = new CPropertyItem("ItemDBIdx",str,PIT_NONE,"");
	m_listExBasic.AddPropItem(m_BasicExList[emBasicEx_ItemDBIdx]);

	itoa(m_ItemOption.GenGol , str, 10);
	m_BasicExList[emBasicEx_GenGol] = new CPropertyItem("Physique",str,PIT_EDIT,"", TRUE, CBT_NONE, 500);
	m_listExBasic.AddPropItem(m_BasicExList[emBasicEx_GenGol]);

	itoa(m_ItemOption.MinChub, str, 10);
	m_BasicExList[emBasicEx_MinChub] = new CPropertyItem("Agility",str,PIT_EDIT,"", TRUE, CBT_NONE, 500);
	m_listExBasic.AddPropItem(m_BasicExList[emBasicEx_MinChub]);

	itoa(m_ItemOption.CheRyuk, str, 10);
	m_BasicExList[emBasicEx_CheRyuk] = new CPropertyItem("Health",str,PIT_EDIT,"", TRUE, CBT_NONE, 500);
	m_listExBasic.AddPropItem(m_BasicExList[emBasicEx_CheRyuk]);

	itoa(m_ItemOption.SimMek , str, 10);
	m_BasicExList[emBasicEx_SimMek] = new CPropertyItem("Heart",str,PIT_EDIT,"", TRUE, CBT_NONE, 500);
	m_listExBasic.AddPropItem(m_BasicExList[emBasicEx_SimMek]);

	itoa(m_ItemOption.Life, str, 10);
	m_BasicExList[emBasicEx_Life] = new CPropertyItem("Life",str,PIT_EDIT,"", TRUE, CBT_NONE, 500);
	m_listExBasic.AddPropItem(m_BasicExList[emBasicEx_Life]);

	itoa(m_ItemOption.NaeRyuk, str, 10);
	m_BasicExList[emBasicEx_NaeRyuk] = new CPropertyItem("Mana",str,PIT_EDIT,"", TRUE, CBT_NONE, 500);
	m_listExBasic.AddPropItem(m_BasicExList[emBasicEx_NaeRyuk]);

	itoa(m_ItemOption.Shield, str, 10);
	m_BasicExList[emBasicEx_Shield] = new CPropertyItem("Shield",str,PIT_EDIT,"", TRUE, CBT_NONE, 500);
	m_listExBasic.AddPropItem(m_BasicExList[emBasicEx_Shield]);

	itoa(m_ItemOption.PhyAttack, str, 10);
	m_BasicExList[emBasicEx_PhyAttack] = new CPropertyItem("PhyAttack",str,PIT_EDIT,"");
	m_listExBasic.AddPropItem(m_BasicExList[emBasicEx_PhyAttack]);

	itoa(m_ItemOption.CriticalPercent, str, 10);
	m_BasicExList[emBasicEx_CriticalPercent] = new CPropertyItem("Critical",str,PIT_EDIT,"");
	m_listExBasic.AddPropItem(m_BasicExList[emBasicEx_CriticalPercent]);

	itoa(m_ItemOption.PhyDefense, str, 10);
	m_BasicExList[emBasicEx_PhyDefense] = new CPropertyItem("PhyDefense",str,PIT_EDIT,"");
	m_listExBasic.AddPropItem(m_BasicExList[emBasicEx_PhyDefense]);
/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
//// 옵션 속성 데이터 (주의!! FLOAT형이다.)////
/*	itoa((int)(m_ItemOption.AttrAttack.GetElement_Val(ATTR_FIRE)), str, 10 );
	m_AttrList[emAttr_AFireElement] = new CPropertyItem("AFIRE",str,PIT_EDIT,"");
	m_listExAttr.AddPropItem(m_AttrList[emAttr_AFireElement]);

	itoa((int)(m_ItemOption.AttrAttack.GetElement_Val(ATTR_WATER)), str, 10 );
	m_AttrList[emAttr_AWaterElement] = new CPropertyItem("AWATER",str,PIT_EDIT,"");
	m_listExAttr.AddPropItem(m_AttrList[emAttr_AWaterElement]);

	itoa((int)(m_ItemOption.AttrAttack.GetElement_Val(ATTR_TREE)), str, 10 );
	m_AttrList[emAttr_ATreeElement] = new CPropertyItem("ATREE",str,PIT_EDIT,"");
	m_listExAttr.AddPropItem(m_AttrList[emAttr_ATreeElement]);

	itoa((int)(m_ItemOption.AttrAttack.GetElement_Val(ATTR_IRON)), str, 10 );
	m_AttrList[emAttr_AGoldElement] = new CPropertyItem("AIRON",str,PIT_EDIT,"");
	m_listExAttr.AddPropItem(m_AttrList[emAttr_AGoldElement]);

	itoa((int)(m_ItemOption.AttrAttack.GetElement_Val(ATTR_EARTH)), str, 10 );
	m_AttrList[emAttr_AEarthElement] = new CPropertyItem("AEARTH",str,PIT_EDIT,"");
	m_listExAttr.AddPropItem(m_AttrList[emAttr_AEarthElement]);

	itoa((int)(m_ItemOption.AttrAttack.GetElement_Val(ATTR_NOATTR)), str, 10 );
	m_AttrList[emAttr_ANoneElement] = new CPropertyItem("ANOATTR",str,PIT_EDIT,"");
	m_listExAttr.AddPropItem(m_AttrList[emAttr_ANoneElement]);

	itoa((int)(m_ItemOption.AttrRegist.GetElement_Val(ATTR_FIRE)), str, 10 );
	m_AttrList[emAttr_DFireElement] = new CPropertyItem("DFIRE",str,PIT_EDIT,"");
	m_listExAttr.AddPropItem(m_AttrList[emAttr_DFireElement]);

	itoa((int)(m_ItemOption.AttrRegist.GetElement_Val(ATTR_WATER)), str, 10 );
	m_AttrList[emAttr_DWaterElement] = new CPropertyItem("DWATER",str,PIT_EDIT,"");
	m_listExAttr.AddPropItem(m_AttrList[emAttr_DWaterElement]);

	itoa((int)(m_ItemOption.AttrRegist.GetElement_Val(ATTR_TREE)), str, 10 );
	m_AttrList[emAttr_DTreeElement] = new CPropertyItem("DTREE",str,PIT_EDIT,"");
	m_listExAttr.AddPropItem(m_AttrList[emAttr_DTreeElement]);

	itoa((int)(m_ItemOption.AttrRegist.GetElement_Val(ATTR_IRON)), str, 10 );
	m_AttrList[emAttr_DGoldElement] = new CPropertyItem("DIRON",str,PIT_EDIT,"");
	m_listExAttr.AddPropItem(m_AttrList[emAttr_DGoldElement]);

	itoa((int)(m_ItemOption.AttrRegist.GetElement_Val(ATTR_EARTH)), str, 10 );
	m_AttrList[emAttr_DEarthElement] = new CPropertyItem("DEARTH",str,PIT_EDIT,"");
	m_listExAttr.AddPropItem(m_AttrList[emAttr_DEarthElement]);

	itoa((int)(m_ItemOption.AttrRegist.GetElement_Val(ATTR_NOATTR)), str, 10 );
	m_AttrList[emAttr_DNoneElement] = new CPropertyItem("DNOATTR",str,PIT_EDIT,"");
	m_listExAttr.AddPropItem(m_AttrList[emAttr_DNoneElement]);
*/

	float temp = m_ItemOption.AttrAttack.GetElement_Val(ATTR_FIRE);
//	itoa((int)(m_ItemOption.AttrAttack.GetElement_Val(ATTR_FIRE)*100), str, 10 );
	itoa((int)temp, str, 10 );
	m_AttrList[emAttr_AFireElement] = new CPropertyItem("AFIRE",str,PIT_EDIT,"");
	m_listExAttr.AddPropItem(m_AttrList[emAttr_AFireElement]);

	temp = m_ItemOption.AttrAttack.GetElement_Val(ATTR_WATER);
//	itoa((int)(m_ItemOption.AttrAttack.GetElement_Val(ATTR_WATER)*100), str, 10 );
	itoa((int)temp, str, 10 );
	m_AttrList[emAttr_AWaterElement] = new CPropertyItem("AWATER",str,PIT_EDIT,"");
	m_listExAttr.AddPropItem(m_AttrList[emAttr_AWaterElement]);

	temp = m_ItemOption.AttrAttack.GetElement_Val(ATTR_TREE);
//	itoa((int)(m_ItemOption.AttrAttack.GetElement_Val(ATTR_TREE)*100), str, 10 );
	itoa((int)temp, str, 10 );
	m_AttrList[emAttr_ATreeElement] = new CPropertyItem("ATREE",str,PIT_EDIT,"");
	m_listExAttr.AddPropItem(m_AttrList[emAttr_ATreeElement]);

	temp = m_ItemOption.AttrAttack.GetElement_Val(ATTR_IRON);
//	itoa((int)(m_ItemOption.AttrAttack.GetElement_Val(ATTR_IRON)*100), str, 10 );
	itoa((int)temp, str, 10 );
	m_AttrList[emAttr_AGoldElement] = new CPropertyItem("AIRON",str,PIT_EDIT,"");
	m_listExAttr.AddPropItem(m_AttrList[emAttr_AGoldElement]);

	temp = m_ItemOption.AttrAttack.GetElement_Val(ATTR_EARTH);
//	itoa((int)(m_ItemOption.AttrAttack.GetElement_Val(ATTR_EARTH)*100), str, 10 );
	itoa((int)temp, str, 10 );
	m_AttrList[emAttr_AEarthElement] = new CPropertyItem("AEARTH",str,PIT_EDIT,"");
	m_listExAttr.AddPropItem(m_AttrList[emAttr_AEarthElement]);

	temp = m_ItemOption.AttrAttack.GetElement_Val(ATTR_NOATTR);
//	itoa((int)(m_ItemOption.AttrAttack.GetElement_Val(ATTR_NOATTR)*100), str, 10 );
	itoa((int)temp, str, 10 );
	m_AttrList[emAttr_ANoneElement] = new CPropertyItem("ANOATTR",str,PIT_EDIT,"");
	m_listExAttr.AddPropItem(m_AttrList[emAttr_ANoneElement]);

	temp = m_ItemOption.AttrRegist.GetElement_Val(ATTR_FIRE);
//	itoa((int)(m_ItemOption.AttrRegist.GetElement_Val(ATTR_FIRE)*100), str, 10 );
	itoa((int)temp, str, 10 );
	m_AttrList[emAttr_DFireElement] = new CPropertyItem("DFIRE",str,PIT_EDIT,"");
	m_listExAttr.AddPropItem(m_AttrList[emAttr_DFireElement]);

	temp = m_ItemOption.AttrRegist.GetElement_Val(ATTR_WATER);
//	itoa((int)(m_ItemOption.AttrRegist.GetElement_Val(ATTR_WATER)*100), str, 10 );
	itoa((int)temp, str, 10 );
	m_AttrList[emAttr_DWaterElement] = new CPropertyItem("DWATER",str,PIT_EDIT,"");
	m_listExAttr.AddPropItem(m_AttrList[emAttr_DWaterElement]);

	temp = m_ItemOption.AttrRegist.GetElement_Val(ATTR_TREE);
//	itoa((int)(m_ItemOption.AttrRegist.GetElement_Val(ATTR_TREE)*100), str, 10 );
	itoa((int)temp, str, 10 );
	m_AttrList[emAttr_DTreeElement] = new CPropertyItem("DTREE",str,PIT_EDIT,"");
	m_listExAttr.AddPropItem(m_AttrList[emAttr_DTreeElement]);

	temp = m_ItemOption.AttrRegist.GetElement_Val(ATTR_IRON);
//	itoa((int)(m_ItemOption.AttrRegist.GetElement_Val(ATTR_IRON)*100), str, 10 );
	itoa((int)temp, str, 10 );
	m_AttrList[emAttr_DGoldElement] = new CPropertyItem("DIRON",str,PIT_EDIT,"");
	m_listExAttr.AddPropItem(m_AttrList[emAttr_DGoldElement]);

	temp = m_ItemOption.AttrRegist.GetElement_Val(ATTR_EARTH);
//	itoa((int)(m_ItemOption.AttrRegist.GetElement_Val(ATTR_EARTH)*100), str, 10 );
	itoa((int)temp, str, 10 );
	m_AttrList[emAttr_DEarthElement] = new CPropertyItem("DEARTH",str,PIT_EDIT,"");
	m_listExAttr.AddPropItem(m_AttrList[emAttr_DEarthElement]);

	temp = m_ItemOption.AttrRegist.GetElement_Val(ATTR_NOATTR);
//	itoa((int)(m_ItemOption.AttrRegist.GetElement_Val(ATTR_NOATTR)*100), str, 10 );
	itoa((int)temp, str, 10 );
	m_AttrList[emAttr_DNoneElement] = new CPropertyItem("DNOATTR",str,PIT_EDIT,"");
	m_listExAttr.AddPropItem(m_AttrList[emAttr_DNoneElement]);
/////////////////////////////////////////////////////////////////////////////////////////

	SetItemTypeControl();
	if( m_Item.wIconIdx == 0 )
		GetDlgItem(IDC_BUTTON_DEL)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CItemDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	int i;
	for(i=0;i<emBasic_Max;++i)
		SAFE_DELETE( m_BasicList[i] );

	for(i=0;i<emBasicEx_Max;++i)
		SAFE_DELETE( m_BasicExList[i] );

	for(i=0;i<emAttr_Max;++i)
		SAFE_DELETE( m_AttrList[i] );

	
	return CDialog::DestroyWindow();
}


void CItemDlg::SetItemTypeControl()
{
	if( m_Field == eIF_WEAR )
	{
		switch(m_wWearType)
		{
		case eEQUIP_ITEM_HAT:
		case eEQUIP_ITEM_DRESS:
		case eEQUIP_ITEM_SHOES:
			{
				((CButton*)GetDlgItem(IDC_RADIO_DEFFEN))->SetCheck(BST_CHECKED);
				m_nCheckValue = 1;
				m_cbExType.EnableWindow(FALSE);

				SetItemExTypeCombo(m_wWearType-eEQUIP_ITEM_DRESS);
			}
			break;
		case eEQUIP_ITEM_WEAPON:
			{
				((CButton*)GetDlgItem(IDC_RADIO_WEAPON))->SetCheck(BST_CHECKED);
				m_nCheckValue = 0;
				SetItemExTypeCombo();
			}
			break;
		case eEQUIP_ITEM_RING:
		case eEQUIP_ITEM_CAPE:
		case eEQUIP_ITEM_NECKLACE:
		case eEQUIP_ITEM_ARMLET:
		case eEQUIP_ITEM_BELT:
			{
				((CButton*)GetDlgItem(IDC_RADIO_ACCESSORY))->SetCheck(BST_CHECKED);
				m_nCheckValue = 2;
				m_cbExType.EnableWindow(FALSE);
				SetItemExTypeCombo(m_wWearType-eEQUIP_ITEM_ARMLET);
			}
			break;
		default:
			{
				ASSERT(0);
			}
			break;;
		}
		EnableRadio(FALSE);
	}
	else
	{
		m_nCheckValue = 0;
		m_rdWeapon.SetCheck(BST_CHECKED);

		SetItemExTypeCombo();
	}

	EnableOptionControl(m_bOption);
}

void CItemDlg::EnableOptionControl(BOOL bEnable)
{
	if( bEnable )
		m_chExData.SetCheck(BST_CHECKED);
	else
		m_chExData.SetCheck(BST_UNCHECKED);

	m_listExBasic.EnableWindow(bEnable);
	m_listExAttr.EnableWindow(bEnable);
}

void CItemDlg::SetItemExTypeCombo(WORD wDef)
{
	m_cbExType.ResetContent();
	CString str1;
	CString str2;

	switch(m_nCheckValue)
	{
	case 0: // 무기류
		{
			str1 = "Sword|Glove|Broad Sword|Spear|Bow|Throwing Star|";
			str2.Format("%d|%d|%d|%d|%d|%d|", WP_GUM, WP_GWUN, WP_DO, WP_CHANG, WP_GUNG, WP_AMGI);
		}
		break;
	case 1: // 방어구류
		{
			str1 = "Cloth|Hat|Shoes|";
			str2.Format("%d|%d|%d|", eEQUIP_ITEM_DRESS, eEQUIP_ITEM_HAT, eEQUIP_ITEM_SHOES);
		}
		break;
	case 2: // 악세사리류
		{
			str1 = "Armlet|Cloak|Belt|Neckles|Ring|";
			str2.Format("%d|%d|%d|%d|%d|",eEQUIP_ITEM_ARMLET,eEQUIP_ITEM_CAPE,eEQUIP_ITEM_BELT,eEQUIP_ITEM_NECKLACE,eEQUIP_ITEM_RING);
		}
		break;
	case 3: // 기타
		{
			str1 = "Potion|Books|GradeUp|Etc|";
			str2.Format("%d|%d|%d|%d|", eYOUNGYAK_ITEM, eMUGONG_ITEM, eEXTRA_ITEM_UPGRADE, eEXTRA_ITEM);
		}
		break;
	default:
		{
			ASSERT(0);
		}
		return;
	}

	int i,i2, j, j2, nIndex;
	i=0; j=0;
	while ( (i2=str1.Find('|',i)) != -1 )
	{
		j2=str2.Find('|',j);

		nIndex = m_cbExType.AddString(str1.Mid(i,i2-i));
		m_cbExType.SetItemData( nIndex, atoi(str2.Mid(j, j2-j)) );
		i=i2+1;
		j=j2+1;
	}

	m_cbExType.SetCurSel(wDef);
	SetItemCombo(m_cbExType.GetItemData(wDef));
}

void CItemDlg::SetItemCombo(DWORD type)
{
	CYHHashTable<ITEM_INFO>* pItemInfoList = GAMERESRCMNGR->GetItemInfo();
	ITEM_INFO * pItemInfo = NULL;
	int i = 0;

	m_BasicList[emBasic_Idx]->ClearItemsEx();

	if( m_nCheckValue == 0 )
	{
		pItemInfoList->SetPositionHead();
		while(pItemInfo = pItemInfoList->GetData())
		{
			if( pItemInfo->ItemKind == eEQUIP_ITEM_WEAPON && pItemInfo->WeaponType == type )
			{
				if( m_Field == eIF_WEAR && pItemInfo->LimitLevel > m_wLevel )
					continue;

				m_BasicList[emBasic_Idx]->m_cmbItemsEx[i] = pItemInfo->ItemIdx;
				++i;
			}
		}
	}
	else if( m_nCheckValue == 3  && type != eYOUNGYAK_ITEM )
	{
		pItemInfoList->SetPositionHead();
		while(pItemInfo = pItemInfoList->GetData())
		{
			if( pItemInfo->ItemKind & type || pItemInfo->ItemKind & eCHANGE_ITEM )
			{
				m_BasicList[emBasic_Idx]->m_cmbItemsEx[i] = pItemInfo->ItemIdx;
				++i;
			}
		}
	}
	else
	{
		pItemInfoList->SetPositionHead();
		while(pItemInfo = pItemInfoList->GetData())
		{
			if( pItemInfo->ItemKind == type )
			{
				if( (m_nCheckValue == 1 || m_nCheckValue == 2) && 
						m_Field == eIF_WEAR && pItemInfo->LimitLevel > m_wLevel )
					continue;

				m_BasicList[emBasic_Idx]->m_cmbItemsEx[i] = pItemInfo->ItemIdx;
				++i;
			}
		}
	}
}

void CItemDlg::EnableRadio(BOOL bEnable)
{
	(GetDlgItem(IDC_RADIO_WEAPON))->EnableWindow(bEnable);
	(GetDlgItem(IDC_RADIO_DEFFEN))->EnableWindow(bEnable);
	(GetDlgItem(IDC_RADIO_ACCESSORY))->EnableWindow(bEnable);
	(GetDlgItem(IDC_RADIO_ETC))->EnableWindow(bEnable);
}

void CItemDlg::OnRadioAccessory()
{
	// TODO: Add your control notification handler code here
	m_nCheckValue = 2;
	SetItemExTypeCombo();
}

void CItemDlg::OnRadioDeffen()
{
	// TODO: Add your control notification handler code here
	m_nCheckValue = 1;
	SetItemExTypeCombo();
}

void CItemDlg::OnRadioEtc()
{
	// TODO: Add your control notification handler code here
	m_nCheckValue = 3;
	SetItemExTypeCombo();
}

void CItemDlg::OnRadioWeapon()
{
	// TODO: Add your control notification handler code here
	m_nCheckValue = 0;
	SetItemExTypeCombo();
}

void CItemDlg::OnSelchangeComboExtype()
{
	// TODO: Add your control notification handler code here
	int nCurSel = m_cbExType.GetCurSel();
	if( nCurSel == -1 ) return;

	SetItemCombo(m_cbExType.GetItemData(nCurSel));
}

void CItemDlg::OnCheckExdata()
{
	// TODO: Addyour control notification handler code here
	WORD ItemIdx = GAMERESRCMNGR->GetItemIdx_ByItemName( (m_BasicList[emBasic_Idx]->m_curValue).GetBuffer(0) );

	if( ItemIdx == 0 || GAMERESRCMNGR->IsDupItem(ItemIdx) )
	{
		m_bOption = FALSE;
		MessageBox("Can't Edit Option!!");
	}
	else
	{
		if( m_chExData.GetCheck() == BST_CHECKED )
			m_bOption = TRUE;
		else
			m_bOption = FALSE;
	}

	EnableOptionControl(m_bOption);
}

LRESULT CItemDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if( message == WM_CHANGEDATA )
	{
		CPropertyItem* pItem = (CPropertyItem*)wParam;
		if( pItem == m_BasicList[emBasic_Idx] )
		{
			WORD wItemIdx = GAMERESRCMNGR->GetItemIdx_ByItemName( pItem->m_curValue.GetBuffer(0) );
			if( wItemIdx == 0 )
			{
				ASSERT(0);
				return 0;
			}

			if( GAMERESRCMNGR->IsDupItem(wItemIdx) )
			{
				m_BasicList[emBasic_Durability]->m_nItemType = PIT_COMBO;
				m_BasicList[emBasic_Durability]->m_curValue = "1";

				m_bOption = FALSE;
				EnableOptionControl(m_bOption);
			}
			else
			{
				m_BasicList[emBasic_Durability]->m_nItemType = PIT_NONE;
				m_BasicList[emBasic_Durability]->m_curValue = "0";
			}

			m_Item.wIconIdx = wItemIdx;
		}
	}
	
	return CDialog::DefWindowProc(message, wParam, lParam);
}

void CItemDlg::OnButtonEdit()
{
	// TODO: Add your control notification handler code here
//////////////////////////////
// 기본 데이터를 가져온다.
	BOOL bDup;

	if( m_Item.wIconIdx == 0 )
	{
		EndDialog(eMR_Cancel);
		return;
	}

	m_Item.wIconIdx = GAMERESRCMNGR->GetItemIdx_ByItemName((m_BasicList[emBasic_Idx]->m_curValue).GetBuffer(0));
	if( m_Item.wIconIdx == 0 )
	{
		ASSERT(0);
		EndDialog(eMR_Cancel);
		return;
	}

	if( bDup = GAMERESRCMNGR->IsDupItem(m_Item.wIconIdx) )
		m_Item.Durability = atoi(((m_BasicList[emBasic_Durability]->m_curValue).GetBuffer(0)));
	else
		m_Item.Durability = 0;
////////////////////////////////////////////

////////////////////////////////////////////
// 옵션 아이템 이라면..
	if( m_bOption && !bDup )
	{
		// 옵션 기본
		m_ItemOption.dwOptionIdx = atoi(((m_BasicExList[emBasicEx_OptionIdx]->m_curValue).GetBuffer(0)));
		m_ItemOption.dwItemDBIdx = atoi(((m_BasicList[emBasicEx_ItemDBIdx]->m_curValue).GetBuffer(0)));
		m_ItemOption.GenGol = atoi(((m_BasicExList[emBasicEx_GenGol]->m_curValue).GetBuffer(0)));
		m_ItemOption.MinChub = atoi(((m_BasicExList[emBasicEx_MinChub]->m_curValue).GetBuffer(0)));
		m_ItemOption.CheRyuk = atoi(((m_BasicExList[emBasicEx_CheRyuk]->m_curValue).GetBuffer(0)));
		m_ItemOption.SimMek = atoi(((m_BasicExList[emBasicEx_SimMek]->m_curValue).GetBuffer(0)));
		m_ItemOption.Life = atoi(((m_BasicExList[emBasicEx_Life]->m_curValue).GetBuffer(0)));
		m_ItemOption.NaeRyuk = atoi(((m_BasicExList[emBasicEx_NaeRyuk]->m_curValue).GetBuffer(0)));
		m_ItemOption.Shield = atoi(((m_BasicExList[emBasicEx_Shield]->m_curValue).GetBuffer(0)));
		m_ItemOption.PhyAttack = atoi(((m_BasicExList[emBasicEx_PhyAttack]->m_curValue).GetBuffer(0)));
		m_ItemOption.CriticalPercent = atoi(((m_BasicExList[emBasicEx_CriticalPercent]->m_curValue).GetBuffer(0)));
		m_ItemOption.PhyDefense = atoi(((m_BasicExList[emBasicEx_PhyDefense]->m_curValue).GetBuffer(0)));

		// 옵션 속성
		m_ItemOption.AttrAttack.SetElement_Val(ATTR_FIRE, (FLOAT)atof(((m_AttrList[emAttr_AFireElement]->m_curValue).GetBuffer(0))));
		m_ItemOption.AttrAttack.SetElement_Val(ATTR_WATER, (FLOAT)atof(((m_AttrList[emAttr_AWaterElement]->m_curValue).GetBuffer(0))));
		m_ItemOption.AttrAttack.SetElement_Val(ATTR_TREE, (FLOAT)atof(((m_AttrList[emAttr_ATreeElement]->m_curValue).GetBuffer(0))));
		m_ItemOption.AttrAttack.SetElement_Val(ATTR_IRON, (FLOAT)atof(((m_AttrList[emAttr_AGoldElement]->m_curValue).GetBuffer(0))));
		m_ItemOption.AttrAttack.SetElement_Val(ATTR_EARTH, (FLOAT)atof(((m_AttrList[emAttr_AEarthElement]->m_curValue).GetBuffer(0))));
		m_ItemOption.AttrAttack.SetElement_Val(ATTR_NOATTR, (FLOAT)atof(((m_AttrList[emAttr_ANoneElement]->m_curValue).GetBuffer(0))));

		m_ItemOption.AttrRegist.SetElement_Val(ATTR_FIRE, (FLOAT)atof(((m_AttrList[emAttr_DFireElement]->m_curValue).GetBuffer(0))));
		m_ItemOption.AttrRegist.SetElement_Val(ATTR_WATER, (FLOAT)atof(((m_AttrList[emAttr_DWaterElement]->m_curValue).GetBuffer(0))));
		m_ItemOption.AttrRegist.SetElement_Val(ATTR_TREE, (FLOAT)atof(((m_AttrList[emAttr_DTreeElement]->m_curValue).GetBuffer(0))));
		m_ItemOption.AttrRegist.SetElement_Val(ATTR_IRON, (FLOAT)atof(((m_AttrList[emAttr_DGoldElement]->m_curValue).GetBuffer(0))));
		m_ItemOption.AttrRegist.SetElement_Val(ATTR_EARTH, (FLOAT)atof(((m_AttrList[emAttr_DEarthElement]->m_curValue).GetBuffer(0))));
		m_ItemOption.AttrRegist.SetElement_Val(ATTR_NOATTR, (FLOAT)atof(((m_AttrList[emAttr_DNoneElement]->m_curValue).GetBuffer(0))));

/*	
		m_ItemOption.AttrAttack.SetElement_Val(ATTR_FIRE, (FLOAT)atof(((m_AttrList[emAttr_AFireElement]->m_curValue).GetBuffer(0))) / 100.0f);
		m_ItemOption.AttrAttack.SetElement_Val(ATTR_WATER, (FLOAT)atof(((m_AttrList[emAttr_AWaterElement]->m_curValue).GetBuffer(0))) / 100.0f);
		m_ItemOption.AttrAttack.SetElement_Val(ATTR_TREE, (FLOAT)atof(((m_AttrList[emAttr_ATreeElement]->m_curValue).GetBuffer(0))) / 100.0f);
		m_ItemOption.AttrAttack.SetElement_Val(ATTR_IRON, (FLOAT)atof(((m_AttrList[emAttr_AGoldElement]->m_curValue).GetBuffer(0))) / 100.0f);
		m_ItemOption.AttrAttack.SetElement_Val(ATTR_EARTH, (FLOAT)atof(((m_AttrList[emAttr_AEarthElement]->m_curValue).GetBuffer(0))) / 100.0f);
		m_ItemOption.AttrAttack.SetElement_Val(ATTR_NOATTR, (FLOAT)atof(((m_AttrList[emAttr_ANoneElement]->m_curValue).GetBuffer(0))) / 100.0f);

		m_ItemOption.AttrRegist.SetElement_Val(ATTR_FIRE, (FLOAT)atof(((m_AttrList[emAttr_DFireElement]->m_curValue).GetBuffer(0))) / 100.0f);
		m_ItemOption.AttrRegist.SetElement_Val(ATTR_WATER, (FLOAT)atof(((m_AttrList[emAttr_DWaterElement]->m_curValue).GetBuffer(0))) / 100.0f);
		m_ItemOption.AttrRegist.SetElement_Val(ATTR_TREE, (FLOAT)atof(((m_AttrList[emAttr_DTreeElement]->m_curValue).GetBuffer(0))) / 100.0f);
		m_ItemOption.AttrRegist.SetElement_Val(ATTR_IRON, (FLOAT)atof(((m_AttrList[emAttr_DGoldElement]->m_curValue).GetBuffer(0))) / 100.0f);
		m_ItemOption.AttrRegist.SetElement_Val(ATTR_EARTH, (FLOAT)atof(((m_AttrList[emAttr_DEarthElement]->m_curValue).GetBuffer(0))) / 100.0f);
		m_ItemOption.AttrRegist.SetElement_Val(ATTR_NOATTR, (FLOAT)atof(((m_AttrList[emAttr_DNoneElement]->m_curValue).GetBuffer(0))) / 100.0f);
*/
		if( !IsValiedOption() )
		{
			memset(&m_ItemOption, 0, sizeof(ITEM_OPTION_INFO));
			m_bOption = FALSE;
		}
	}
	else
		memset(&m_ItemOption, 0, sizeof(ITEM_OPTION_INFO));
	
	if( m_bOption )
		m_Item.Durability = 9999;
	
	m_Item.QuickPosition = 0;

	EndDialog(eMR_Edit);
////////////////////////////////////////////
}

void CItemDlg::OnButtonDel()
{
	// TODO: Add your control notification handler code here
	memset(&m_Item, 0, sizeof(ITEMBASE));
	memset(&m_ItemOption, 0, sizeof(ITEM_OPTION_INFO));
	
	EndDialog(eMR_Delete);
}

void CItemDlg::OnOK()
{
}

void CItemDlg::OnCancel()
{
	EndDialog(eMR_Cancel);
}


BOOL CItemDlg::IsValiedOption()
{
	if( m_ItemOption.GenGol
			|| m_ItemOption.MinChub
			|| m_ItemOption.CheRyuk
			|| m_ItemOption.SimMek
			|| m_ItemOption.Life
			|| m_ItemOption.NaeRyuk
			|| m_ItemOption.Shield
			|| m_ItemOption.PhyAttack
			|| m_ItemOption.CriticalPercent
			|| m_ItemOption.PhyDefense
			|| m_ItemOption.AttrAttack.GetElement_Val(ATTR_FIRE)
			|| m_ItemOption.AttrAttack.GetElement_Val(ATTR_WATER)
			|| m_ItemOption.AttrAttack.GetElement_Val(ATTR_TREE)
			|| m_ItemOption.AttrAttack.GetElement_Val(ATTR_IRON)
			|| m_ItemOption.AttrAttack.GetElement_Val(ATTR_EARTH)
			|| m_ItemOption.AttrAttack.GetElement_Val(ATTR_NOATTR)
			|| m_ItemOption.AttrRegist.GetElement_Val(ATTR_FIRE)
			|| m_ItemOption.AttrRegist.GetElement_Val(ATTR_WATER)
			|| m_ItemOption.AttrRegist.GetElement_Val(ATTR_TREE)
			|| m_ItemOption.AttrRegist.GetElement_Val(ATTR_IRON)
			|| m_ItemOption.AttrRegist.GetElement_Val(ATTR_EARTH)
			|| m_ItemOption.AttrRegist.GetElement_Val(ATTR_NOATTR) )
		return TRUE;

	return FALSE;
}

