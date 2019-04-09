// CharacterInfo.cpp : implementation file
//

#include "stdafx.h"
#include "DS_RMTool.h"
#include "CharacterInfo.h"
#include "CharacterDoc.h"
#include "..\[CC]Header\CommonCalcFunc.h"

#include "ChangeValue.h"
#include "ItemDlg.h"
#include "MugongDlg.h"
#include "AbilityDlg.h"
#include "ResourceManager.h"
#include "ItemRecoverDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCharacterInfo property page

IMPLEMENT_DYNCREATE(CCharacterInfo, CLDPropPage)

CCharacterInfo::CCharacterInfo() : CLDPropPage(CCharacterInfo::IDD)
{
	//{{AFX_DATA_INIT(CCharacterInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDocument = NULL;
	m_pItemDlg = NULL;
	m_pMugongDlg = NULL;
	m_pAbility = NULL;
	m_nCurInvenTab = 0;

	m_wPower = eOperPower_Max;

	m_pItemRecoverDlg = NULL;
}

CCharacterInfo::~CCharacterInfo()
{
	SAFE_DELETE(m_pItemDlg);
	SAFE_DELETE(m_pMugongDlg);
	SAFE_DELETE(m_pAbility);
}

void CCharacterInfo::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCharacterInfo)
	DDX_Control(pDX, IDC_BUTTON_CHANGE_LOGINPOINT, m_btnChangeLoginPoint);
	DDX_Control(pDX, IDC_CHARACTER_BTN_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_COMBO_LOGINPOINT, m_cbLoginPoint);
	DDX_Control(pDX, IDC_CHARACTER_BTN_ADDPYOGUK, m_btnPyogukAdd);
	DDX_Control(pDX, IDC_PYOGUK_COMBO, m_cbPyoguk);
	DDX_Control(pDX, IDC_INVEN_COMBO, m_cbInven);
	DDX_Control(pDX, IDC_ABILITY_COMBO, m_cbAbility);
	//}}AFX_DATA_MAP

	int i = 0;
	//// 정보창 관련 변수
	for(i=0;i< eCharTemp_MAX;++i)
		DDX_Control(pDX, IDC_CHAR_NAME+i, m_StatusEdit[i]);

	//// 인벤토리 관련 변수
	for(i = 0;i<TABCELL_INVENTORY_NUM;++i)
		DDX_Control(pDX, IDC_BTN_INVENOBJ1+i, m_InvenItem[i]);

	// 장착 아이템
	for(i=0;i<eWearedItem_Max;++i)
		DDX_Control(pDX, IDC_BTN_WEAROBJ1+i, m_WearItem[i]);

	//// 스킬창 관련 변수
	// 무공
	for(i=0;i<SLOT_MUGONG_NUM;++i)
		DDX_Control(pDX, IDC_BTN_SKILL1+i, m_SkillMuGong[i]);

	// 진법
	for(i=0;i<SLOT_JINBUB_NUM;++i)
		DDX_Control(pDX, IDC_BTN_JINBUB1+i, m_SkillJinBub[i]);

	//// 표국창 관련 변수
	for(i=0;i<TABCELL_PYOGUK_NUM;++i)
		DDX_Control(pDX, IDC_BTN_PYOGUK1+i, m_PyoGukItem[i]);

	//// 특기 관련 변수
	for(i=0;i<MAX_ABILITY_NUM_PER_GROUP;++i)
		DDX_Control(pDX, IDC_BTN_ABLITY1+i, m_Ability[i]);

	// 인벤 포지션 겹치는 거
	for(i=0;i<10;++i)
		DDX_Control(pDX, IDC_BTNINVEN_ERROR1+i, m_InvenItemError[i]);

	for(i=0;i<5;++i)
	{
		DDX_Control(pDX, IDC_BTN_WAREERROR1+i, m_WareItemError[i]);
		DDX_Control(pDX, IDC_BTN_SKILLERROR1+i, m_MugongError[i]);
	}

	DDX_Control(pDX, IDC_BTN_INVEN_RECOVER, m_InvenRecover);
}


BEGIN_MESSAGE_MAP(CCharacterInfo, CLDPropPage)
	//{{AFX_MSG_MAP(CCharacterInfo)
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_INVEN_COMBO, OnSelchangeInvenCombo)
	ON_CBN_SELCHANGE(IDC_PYOGUK_COMBO, OnSelchangePyogukCombo)
	ON_CBN_SELCHANGE(IDC_ABILITY_COMBO, OnSelchangeAbilityCombo)
	ON_BN_CLICKED(IDC_CHARACTER_BTN_SAVE, OnCharacterBtnSave)
	ON_BN_CLICKED(IDC_CHARACTER_BTN_ADDPYOGUK, OnCharacterBtnAddpyoguk)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_QUESTINFO, OnQuestinfo)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_LOGINPOINT, OnButtonChangeLoginpoint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCharacterInfo message handlers

int CCharacterInfo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	lpCreateStruct->cx = 1024;
	lpCreateStruct->cy = 768;

	if (CLDPropPage::OnCreate(lpCreateStruct) == -1)
		return -1;	
	
	return 0;
}

void CCharacterInfo::InitControl(CCharacterDoc* pDoc)
{
	int i = 0;
	for(i=0;i< eCharTemp_MAX;++i)
		m_StatusEdit[i].Release();
	for(i = 0;i<TABCELL_INVENTORY_NUM;++i)
		m_InvenItem[i].Release();
	for(i=0;i<eWearedItem_Max;++i)
		m_WearItem[i].Release();
	for(i=0;i<SLOT_MUGONG_NUM;++i)
		m_SkillMuGong[i].Release();
	for(i=0;i<SLOT_JINBUB_NUM;++i)
		m_SkillJinBub[i].Release();
	for(i=0;i<TABCELL_PYOGUK_NUM;++i)
		m_PyoGukItem[i].Release();
	for(i=0;i<MAX_ABILITY_NUM_PER_GROUP;++i)
		m_Ability[i].Release();
	for(i=0;i<10;++i)
	{
		m_InvenItemError[i].Release();
		m_InvenItemError[i].ShowWindow( FALSE );
	}
	for(i=0;i<5;++i)
	{
		m_WareItemError[i].Release();
		m_WareItemError[i].ShowWindow( FALSE );
		m_MugongError[i].Release();
		m_MugongError[i].ShowWindow( FALSE );
	}

	m_InvenRecover.Release();
#ifdef _CHINA
	m_InvenRecover.ShowWindow( FALSE );
	m_InvenRecover.EnableWindow( FALSE );
#else
	m_InvenRecover.ShowWindow( TRUE );
	m_InvenRecover.EnableWindow( TRUE );
#endif

	m_pDocument = pDoc;

	SetBaseInfo( &pDoc->m_sBaseInfo, &pDoc->m_sTotalInfo, &pDoc->m_sHTotalInfo );
	SetInvenInfo( &pDoc->m_sInven );
	SetInvenInfoError( &pDoc->m_InvenInfoError );
	SetPyogukInfo( &pDoc->m_sPyoguk );
	SetPyogukInfoError( &pDoc->m_WareError );
	SetMugongInfo( &pDoc->m_sMugong );
	SetMugongInfoError( &pDoc->m_MugongError );
	SetAbilityInfo();

	// 계산 수치에 대해서...
	ResetCalcInfo();

	// login point combobox
	int mapnum = 0;
	char* pName = NULL;
	CYHHashTable<char>*	pTable = GAMERESRCMNGR->GetMapNameTable();
	pTable->SetPositionHead();
	while( pName = pTable->GetData() )
	{
		if( strcmp( pName, "" ) != 0 )
			m_cbLoginPoint.AddString( pName );
	}
	m_cbLoginPoint.SetCurSel( 0 );
}

//---------------------------------------------------------------------------------------
// set fn
//---------------------------------------------------------------------------------------
void CCharacterInfo::SetBaseInfo(BASEOBJECT_INFO* objInfo, CHARACTER_TOTALINFO* totalInfo, HERO_TOTALINFO* heroInfo)
{
	CString str;

	m_StatusEdit[eCharTemp_NAME].SetWindowTextEx( objInfo->ObjectName, 0 );		// name
	m_StatusEdit[eCharTemp_NICK].SetWindowTextEx( "-", 0 );						// nickname
	str.Format( "%d", totalInfo->Level );
	m_StatusEdit[eCharTemp_LEVEL].SetMaxValue( 99 );
	m_StatusEdit[eCharTemp_LEVEL].SetWindowTextEx( (char*)(LPCTSTR)str, 1 );	// level
	str.Format( "%d", totalInfo->Stage );
	m_StatusEdit[eCharTemp_STEP].SetWindowTextEx( (char*)(LPCTSTR)str, 1 );		// stage
	str.Format( "%d", heroInfo->ExpPoint );
	m_StatusEdit[eCharTemp_EXP].SetWindowTextEx( (char*)(LPCTSTR)str, 1 );		// exp
	m_StatusEdit[eCharTemp_MUNPA].SetWindowTextEx( heroInfo->MunpaName, 0 );	// munpa
	str.Format( "%d", totalInfo->PositionInMunpa );
	m_StatusEdit[eCharTemp_MUNPAPOS].SetWindowTextEx( (char*)(LPCTSTR)str, 1 );	// munpaposition
	str.Format( "%d", heroInfo->Fame );
	m_StatusEdit[eCharTemp_FAME].SetWindowTextEx( (char*)(LPCTSTR)str, 1 );		// fame
	str.Format( "%d", totalInfo->BadFame );
	m_StatusEdit[eCharTemp_BADFAME].SetWindowTextEx( (char*)(LPCTSTR)str, 1 );	// badfame
	str.Format( "%d", heroInfo->wGenGol );
	m_StatusEdit[eCharTemp_GENGOL].SetWindowTextEx( (char*)(LPCTSTR)str, 1 );	// gengol
	str.Format( "%d", heroInfo->wMinChub );
	m_StatusEdit[eCharTemp_MINCHUB].SetWindowTextEx( (char*)(LPCTSTR)str, 1 );	// minchub
	str.Format( "%d", heroInfo->wCheRyuk );
	m_StatusEdit[eCharTemp_CHERYUK].SetWindowTextEx( (char*)(LPCTSTR)str, 1 );	// chetyuk
	str.Format( "%d", heroInfo->wSimMek );
	m_StatusEdit[eCharTemp_SIMMAK].SetWindowTextEx( (char*)(LPCTSTR)str, 1 );	// simmek
	str.Format( "%d", heroInfo->LevelUpPoint );
	m_StatusEdit[eCharTemp_POINT].SetWindowTextEx( (char*)(LPCTSTR)str, 1 );	// point

	str.Format( "%d", totalInfo->Life );
	m_StatusEdit[eCharTemp_LIFE].SetWindowTextEx( (char*)(LPCTSTR)str, 1 );		// life
	str.Format( "%d", heroInfo->wNaeRyuk );
	m_StatusEdit[eCharTemp_MANA].SetWindowTextEx( (char*)(LPCTSTR)str, 1 );		// 내력
	str.Format( "%d", totalInfo->Shield );
	m_StatusEdit[eCharTemp_SHIELD].SetWindowTextEx( (char*)(LPCTSTR)str, 1 );	// shield

	str.Format( "%d", heroInfo->Money );
	m_StatusEdit[eCharTemp_INVENMONEY].SetMaxValue( MAX_INVENTORY_MONEY );
	m_StatusEdit[eCharTemp_INVENMONEY].SetWindowTextEx( (char*)(LPCTSTR)str, 1 );	// invenmoney
	str.Format( "%d", heroInfo->AbilityExp );
	m_StatusEdit[eCharTemp_ABILITYPOINT].SetWindowTextEx( (char*)(LPCTSTR)str, 1 );	// abilityexp

	str.Format( "%s", GAMERESRCMNGR->GetMapName( totalInfo->LoginMapNum ) );
	m_StatusEdit[eCharTemp_LoginPoint].SetWindowTextEx( (char*)(LPCTSTR)str, 0 );	// loginpoint

	for( int i = 0; i < eCharTemp_MAX; ++i )
	{
		m_StatusEdit[i].SetChanged( FALSE );
		m_StatusEdit[i].SetInSave( FALSE );
	}
}

void CCharacterInfo::ResetCalcInfo()
{
// 계산 수치 적용은 나중에....
/*	WORD minChub = 0;
	WORD genGol = 0;;
	WORD attackMin = 0;
	WORD attackMax = 0;

	int RangeType = 0;
	WORD wWeaponIdx = (WORD)m_WearItem[eWearedItem_Weapon].GetData();

	if( wWeaponIdx )
	{
		ITEM_INFO* pWeaponInfo = GAMERESRCMNGR->GetItemInfo(wWeaponIdx);
		if( pWeaponInfo )
			RangeType = pWeaponInfo->RangeType;
	}

	WORD ItemIdx;
	for(int part=0;part<eWearedItem_Max;++part)
	{
		ItemIdx = (WORD)m_WearItem[part].GetData();
		if(ItemIdx == 0)
			continue;

		ITEM_INFO * pItemInfo = GAMERESRCMNGR->GetItemInfo(ItemIdx);
		if( !pItemInfo )
			continue;
		
		if(RangeType==0)
		{
			attackMin += pItemInfo->MeleeAttackMin;		
			attackMax += pItemInfo->MeleeAttackMax;
			genGol += pItemInfo->GenGol;
		}
		else
		{
			attackMin += pItemInfo->RangeAttackMin;
			attackMax += pItemInfo->RangeAttackMax;
			minChub += pItemInfo->MinChub;
		}
	}

	CString str;

	if(RangeType == 0)
	{
		WORD GenGol = (WORD)m_StatusEdit[eCharTemp_GENGOL].GetData() + genGol;
		str.Format("%d ~ %d", CalcMeleeAttackPower(GenGol,attackMin), CalcMeleeAttackPower(GenGol,attackMax) );
		m_StatusEdit[eCharTemp_ATTACK].SetWindowText(str.GetBuffer(0));
		m_StatusEdit[eCharTemp_LONGATTACK].SetWindowText( "-" );
	}
	else
	{
		WORD MinChub = (WORD)m_StatusEdit[eCharTemp_MINCHUB].GetData() + minChub;
		str.Format("%d ~ %d", CalcRangeAttackPower(MinChub,attackMin), CalcRangeAttackPower(MinChub,attackMax) );
		m_StatusEdit[eCharTemp_LONGATTACK].SetWindowText(str.GetBuffer(0));
		m_StatusEdit[eCharTemp_ATTACK].SetWindowText( "-" );
	}
	*/

// 임시로...
	m_StatusEdit[eCharTemp_ATTACK].SetWindowText( "-" );
	m_StatusEdit[eCharTemp_CRITICAL].SetWindowText( "-" );
	m_StatusEdit[eCharTemp_LONGATTACK].SetWindowText( "-" );
	m_StatusEdit[eCharTemp_RANGE].SetWindowText( "-" );
	m_StatusEdit[eCharTemp_LIFE].SetWindowText( "-" );
	m_StatusEdit[eCharTemp_DEFFENCE].SetWindowText( "-" );
	m_StatusEdit[eCharTemp_MANA].SetWindowText( "-" );
	m_StatusEdit[eCharTemp_SHIELD].SetWindowText( "-" );
	m_StatusEdit[eCharTemp_FRESI].SetWindowText( "-" );
	m_StatusEdit[eCharTemp_WRESI].SetWindowText( "-" );
	m_StatusEdit[eCharTemp_TRESI].SetWindowText( "-" );
	m_StatusEdit[eCharTemp_GRESI].SetWindowText( "-" );
	m_StatusEdit[eCharTemp_TORESI].SetWindowText( "-" );
}

void CCharacterInfo::SetInvenInfo( ITEM_TOTALINFO* inven )
{
	char buff[256] = {0,};
	int i = 0;
	ITEM_INFO* pInfo = NULL;

	// Combo Init
	m_cbInven.ResetContent();
	for( i = 0; i < TAB_INVENTORY_NUM; ++i )
	{
		sprintf( buff, "Tab%d", i+1 );
		m_cbInven.AddString( buff );
	}
	m_cbInven.SetCurSel(0);

	// weareditem
	for( i = 0; i < eWearedItem_Max; ++i )
	{
		if( inven->WearedItem[i].wIconIdx != 0 )
		{
			pInfo = GAMERESRCMNGR->GetItemInfo( inven->WearedItem[i].wIconIdx );
			if( pInfo )
				sprintf( buff, pInfo->ItemName );
			else
				sprintf( buff, "%d", inven->WearedItem[i].wIconIdx );
		}
		else
			sprintf( buff, "" );

		m_WearItem[i].SetWindowText( buff );
		m_WearItem[i].SetData( inven->WearedItem[i].wIconIdx );
	}

	// invenitem
	for( i = 0; i < TABCELL_INVENTORY_NUM; ++i )
	{
		if( inven->Inventory[i].wIconIdx != 0 )
		{
			pInfo = GAMERESRCMNGR->GetItemInfo( inven->Inventory[i].wIconIdx );
			if( pInfo )
				sprintf( buff, pInfo->ItemName );
			else
				sprintf( buff, "%d", inven->Inventory[i].wIconIdx );
		}
		else
			sprintf( buff, "" );

		m_InvenItem[i].SetWindowText( buff );
		m_InvenItem[i].SetData( inven->Inventory[i].wIconIdx );
	}
}

void CCharacterInfo::SetInvenInfoError( TMSG_CHARACTER_INVENINFO2* inven )
{
	char buff[256] = {0,};
	int i = 0;
	ITEM_INFO* pInfo = NULL;

	for( i = 0; i < inven->m_dwCount; ++i )
	{
		if( inven->m_ItemInfo[i].wIconIdx != 0 )
		{
			pInfo = GAMERESRCMNGR->GetItemInfo( inven->m_ItemInfo[i].wIconIdx );
			if( pInfo )
				sprintf( buff, pInfo->ItemName );
			else
				sprintf( buff, "%d", inven->m_ItemInfo[i].wIconIdx );
		}
		else
			sprintf( buff, "" );

		m_InvenItemError[i].SetWindowText( buff );
		m_InvenItemError[i].SetData( inven->m_ItemInfo[i].wIconIdx );	
		m_InvenItemError[i].ShowWindow( TRUE );	
	}
}

void CCharacterInfo::SetPyogukInfo( PHYOKUK_TOTALINFO* pyoguk )
{
	char buff[256] = {0,};
	int i = 0;
	ITEM_INFO* pInfo = NULL;

	m_cbPyoguk.ResetContent();
	if( pyoguk->PyokukCount == 0 )
	{
		EnablePyogukInfoControl(FALSE);
		return;
	}
	else
	{
		for( i = 0; i < pyoguk->PyokukCount; ++i )
		{
			sprintf( buff, "Tab%d", i+1 );
			m_cbPyoguk.AddString( buff );
		}
		m_cbPyoguk.SetCurSel( 0 );

		if( pyoguk->PyokukCount >= TAB_PYOGUK_NUM )
			m_btnPyogukAdd.EnableWindow( FALSE );

		for( i = 0; i < TABCELL_PYOGUK_NUM; ++i )
		{
			if( pyoguk->Pyokuk[i].wIconIdx != 0 )
			{
				pInfo = GAMERESRCMNGR->GetItemInfo( pyoguk->Pyokuk[i].wIconIdx );
				if( pInfo )
					sprintf( buff, pInfo->ItemName );
				else
					sprintf( buff, "%d", pyoguk->Pyokuk[i].wIconIdx );
			}
			else
				sprintf( buff, "" );

			m_PyoGukItem[i].SetWindowText( buff );
			m_PyoGukItem[i].SetData( pyoguk->Pyokuk[i].wIconIdx );
		}

		sprintf( buff, "%d", pyoguk->Money );
		m_StatusEdit[eCharTemp_PYOGUKMONEY].SetMaxValue(1000000 + pyoguk->PyokukCount*1000000);
		m_StatusEdit[eCharTemp_PYOGUKMONEY].SetWindowTextEx( buff, 1 );
	}
}

void CCharacterInfo::SetPyogukInfoError( PHYOKUK_TOTALINFO* pinfo )
{
	char buff[256] = {0,};
	int i = 0;
	ITEM_INFO* pInfo = NULL;

	for( i = 0; i < pinfo->PyokukCount; ++i )
	{
		if( pinfo->Pyokuk[i].wIconIdx != 0 )
		{
			pInfo = GAMERESRCMNGR->GetItemInfo( pinfo->Pyokuk[i].wIconIdx );
			if( pInfo )
				sprintf( buff, pInfo->ItemName );
			else
				sprintf( buff, "%d", pinfo->Pyokuk[i].wIconIdx );
		}
		else
			sprintf( buff, "" );

		m_WareItemError[i].SetWindowText( buff );
		m_WareItemError[i].SetData( pinfo->Pyokuk[i].wIconIdx );
		m_WareItemError[i].ShowWindow( TRUE );
	}
}

void CCharacterInfo::EnablePyogukInfoControl( BOOL bEnable )
{
	m_cbPyoguk.EnableWindow( bEnable );
	m_StatusEdit[eCharTemp_PYOGUKMONEY].EnableWindow( bEnable );
	
	for( int i = 0; i < TABCELL_PYOGUK_NUM; ++i )
		m_PyoGukItem[i].EnableWindow( bEnable );
}

void CCharacterInfo::SetMugongInfo( MUGONG_TOTALINFO* mugong )
{
	char buff[256]={0,};
	int i,temp;
	SKILLINFO* pInfo = NULL;

	MUGONGBASE* sMugong = mugong->mugong;

	// 무공 넣기
	for( i = 0, temp = 0; i < SLOT_MUGONG_NUM; ++i )
	{
		if( sMugong[i].wIconIdx != 0 )
		{
			pInfo = GAMERESRCMNGR->GetSkillInfo( sMugong[i].wIconIdx );
			if( pInfo )
				sprintf( buff, pInfo->SkillName );
			else
				sprintf( buff, "%d", sMugong[i].wIconIdx );
		}
		else
			sprintf( buff, "" );

		m_SkillMuGong[i].SetWindowText( buff );
		m_SkillMuGong[i].SetData( sMugong[i].wIconIdx );
	}

	temp = i;

	// 진법 넣기
	for( i = 0; i < SLOT_JINBUB_NUM; ++i )
	{
		if( sMugong[temp+i].wIconIdx != 0 )
		{
			pInfo = GAMERESRCMNGR->GetSkillInfo( sMugong[temp+i].wIconIdx );
			if( pInfo )
				sprintf( buff, pInfo->SkillName );
			else
				sprintf( buff, "%d", sMugong[temp+i].wIconIdx );
		}
		else
			sprintf( buff, "" );

		m_SkillJinBub[i].SetWindowText( buff );
		m_SkillJinBub[i].SetData( sMugong[temp+i].wIconIdx );
	}
}

void CCharacterInfo::SetMugongInfoError( TMSG_CHARACTER_MUGONGINFO2* pinfo )
{
	char buff[256]={0,};
	SKILLINFO* pInfo = NULL;

	for( int i = 0; i < pinfo->m_dwCount; ++i )
	{
		if( pinfo->m_MugongBase[i].wIconIdx != 0 )
		{
			pInfo = GAMERESRCMNGR->GetSkillInfo( pinfo->m_MugongBase[i].wIconIdx );
			if( pInfo )
				sprintf( buff, pInfo->SkillName );
			else
				sprintf( buff, "%d", pinfo->m_MugongBase[i].wIconIdx );
		}
		else
			sprintf( buff, "" );		

		m_MugongError[i].SetWindowText( buff );
		m_MugongError[i].SetData( pinfo->m_MugongBase[i].wIconIdx );
		m_MugongError[i].ShowWindow( TRUE );
	}
}

void CCharacterInfo::SetAbilityInfo()
{
	char buff[256] = {0,};
	int i = 0;

	// Combo Init
	m_cbAbility.ResetContent();
	for( i = 0; i < eAGK_Max; ++i )
	{
		sprintf( buff, "Tab%d", i+1 );
		m_cbAbility.AddString( buff );
	}
	m_cbAbility.SetCurSel(0);

	// Ability 초기화
	for( i = 0; i < MAX_ABILITY_NUM_PER_GROUP; ++i )
	{
		m_Ability[i].SetWindowText( "" );
		m_Ability[i].SetData( 0 );
	}

	CYHHashTable<CAbilityInfo>*	pAbility = GAMERESRCMNGR->GetAbilityInfo();
	CAbilityInfo* pAInfo = NULL;

	pAbility->SetPositionHead();
	while( pAInfo = pAbility->GetData() )
	{
		if( pAInfo->GetKind() == 0 )
		{
			if( pAInfo->GetInfo()->Ability_AcquireKind == eAAK_CharacterLevel &&
				pAInfo->GetInfo()->Ability_AcquireParam1 <= m_pDocument->m_sTotalInfo.Level )
				m_Ability[pAInfo->GetPosition()].SetWindowText( pAInfo->GetInfo()->Ability_name );
				m_Ability[pAInfo->GetPosition()].SetData( pAInfo->GetInfo()->Ability_index );
		}
	}
}

//---------------------------------------------------------------------------------------
// combo box
//---------------------------------------------------------------------------------------
void CCharacterInfo::OnSelchangeInvenCombo() 
{
	int nCurSel = m_cbInven.GetCurSel();
	int tab = nCurSel*TABCELL_INVENTORY_NUM;
	m_nCurInvenTab = nCurSel*TABCELL_INVENTORY_NUM;

	char buff[256]={0,};
	int i = 0;
	ITEM_INFO* pInfo = NULL;

	ITEMBASE* sInven = m_pDocument->m_sInven.Inventory;
	ITEMBASE* oInven = m_pDocument->m_sInven_org.Inventory;

	for( i = 0; i < TABCELL_INVENTORY_NUM; ++i )
	{
		if( sInven[tab+i].Durability != oInven[tab+i].Durability ||
			sInven[tab+i].dwDBIdx != oInven[tab+i].dwDBIdx ||
			sInven[tab+i].wIconIdx != oInven[tab+i].wIconIdx )
//		if( sInven[i].dwDBIdx == 0 )
			m_InvenItem[i].SetColorText( COLOR_BUTTON_CHANGED );
		else
			m_InvenItem[i].SetColorText( COLOR_BUTTON_DEFAULT );

		if( sInven[tab+i].wIconIdx != 0 )
		{
			pInfo = GAMERESRCMNGR->GetItemInfo( sInven[tab+i].wIconIdx );
			if( pInfo )
				sprintf( buff, pInfo->ItemName );
			else
				sprintf( buff, "%d", sInven[tab+i].wIconIdx );
		}
		else
			sprintf( buff, "" );

		m_InvenItem[i].SetWindowText( buff );
		m_InvenItem[i].SetData( sInven[tab+i].wIconIdx );
	}

	ResetTooltip();
}

void CCharacterInfo::OnSelchangePyogukCombo() 
{
	int nCurSel = m_cbPyoguk.GetCurSel();
	int tab = nCurSel*TABCELL_PYOGUK_NUM;

	char buff[256]={0,};
	int i = 0;
	ITEM_INFO* pInfo = NULL;	

	ITEMBASE* pItem = m_pDocument->m_sPyoguk.Pyokuk;

	for( i = 0; i < TABCELL_PYOGUK_NUM; ++i )
	{
		if( pItem[tab+i].wIconIdx != 0 )
		{
			pInfo = GAMERESRCMNGR->GetItemInfo( pItem[tab+i].wIconIdx );
			if( pInfo )
				sprintf( buff, pInfo->ItemName );
			else
				sprintf( buff, "%d", pItem[tab+i].wIconIdx );
		}
		else
			sprintf( buff, "" );

		m_PyoGukItem[i].SetWindowText( buff );
		m_PyoGukItem[i].SetData( pItem[tab+i].wIconIdx );
	}

	ResetTooltip();
}

void CCharacterInfo::OnSelchangeAbilityCombo() 
{
	int nCurSel = m_cbAbility.GetCurSel();
	if( nCurSel == -1 ) return;

	// Ability 초기화
	for( int i = 0; i < MAX_ABILITY_NUM_PER_GROUP; ++i )
	{
		m_Ability[i].SetWindowText( "" );
		m_Ability[i].SetData( 0 );
	}

	CYHHashTable<CAbilityInfo>*	pAbility = GAMERESRCMNGR->GetAbilityInfo();
	CAbilityInfo* pAInfo = NULL;

	pAbility->SetPositionHead();
	while( pAInfo = pAbility->GetData() )
	{
		if( pAInfo->GetKind() == nCurSel )
		{
			if( pAInfo->GetInfo()->Ability_AcquireKind == eAAK_CharacterLevel &&
				pAInfo->GetInfo()->Ability_AcquireParam1 <= m_pDocument->m_sTotalInfo.Level )
			{
				m_Ability[pAInfo->GetPosition()].SetWindowText( pAInfo->GetInfo()->Ability_name );
				m_Ability[pAInfo->GetPosition()].SetData( pAInfo->GetInfo()->Ability_index );
			}
			else
			{
				m_Ability[pAInfo->GetPosition()].SetWindowText( "" );
				m_Ability[pAInfo->GetPosition()].SetData( 0 );
			}
		}
	}

	ResetTooltip();
}

void CCharacterInfo::GetChangeBaseInfo(TMSG_CHARACTER_UPDATEBASEINFO* msg)
{
	CString str;

	m_StatusEdit[eCharTemp_NAME].GetStringData(str.GetBuffer(0));
	strcpy(msg->ObjectName, str);
	m_StatusEdit[eCharTemp_NICK].GetStringData(str.GetBuffer(0));
	strcpy(msg->NickName, str);
	m_StatusEdit[eCharTemp_MUNPA].GetStringData(str.GetBuffer(0));
	strcpy(msg->MunpaName, str);
	msg->PositionInMunpa = (BYTE)m_StatusEdit[eCharTemp_MUNPAPOS].GetData();
	msg->Fame = (FAMETYPE)m_StatusEdit[eCharTemp_FAME].GetData();
//	msg->PartyID = m_StatusEdit[eCharTemp_PARTY].GetData();
	msg->Stage = (BYTE)m_StatusEdit[eCharTemp_STEP].GetData();
	msg->Level = (LEVELTYPE)m_StatusEdit[eCharTemp_LEVEL].GetData();
	msg->ExpPoint = m_StatusEdit[eCharTemp_EXP].GetData();
	msg->wGenGol = (WORD)m_StatusEdit[eCharTemp_GENGOL].GetData();
	msg->wMinChub = (WORD)m_StatusEdit[eCharTemp_MINCHUB].GetData();
	msg->wCheRyuk = (WORD)m_StatusEdit[eCharTemp_CHERYUK].GetData();
	msg->wSimMek = (WORD)m_StatusEdit[eCharTemp_SIMMAK].GetData();
	msg->LevelUpPoint = (LEVELTYPE)m_StatusEdit[eCharTemp_POINT].GetData();
	msg->Money = m_StatusEdit[eCharTemp_INVENMONEY].GetData();
	msg->AbilityExp = m_StatusEdit[eCharTemp_ABILITYPOINT].GetData();
}

void CCharacterInfo::GetChangePyogukInfo(TMSG_CHARACTER_PYOGUKINFO* msg)
{
	msg->m_wPyogukNum = m_cbPyoguk.GetCount();
	msg->m_dwMoney = m_StatusEdit[eCharTemp_PYOGUKMONEY].GetData();
}

BOOL CCharacterInfo::PreTranslateMessage(MSG* pMsg)
{
	BYTE type; // 0:inven, 1:wear, 2:pyoguk, 3:mugong, 4:jinbub, 5:ability
	POSTYPE pos;
	if( CMRButton* wnd = CheckButtonEvent(pMsg->hwnd, pos, type) )
	{
		char buff[1024] = {0,};
		MakeToolTip(type, pos, (WORD)wnd->GetData(), buff);

		if( strlen(buff) )
		{
			m_TTctrl.Activate(TRUE);
			m_TTctrl.UpdateTipText( buff , wnd );
			m_TTctrl.RelayEvent(pMsg);
		}
	}
	
	return CLDPropPage::PreTranslateMessage(pMsg);
}

BOOL CCharacterInfo::OnInitDialog() 
{
	CLDPropPage::OnInitDialog();
	
	CDS_RMToolApp* pApp = (CDS_RMToolApp*)AfxGetApp();
	m_wPower = pApp->GetCurOperatorPower();

	m_btnChangeLoginPoint.EnableWindow( FALSE );
	m_btnSave.EnableWindow( FALSE );

	// TODO: Add extra initialization here
	m_TTctrl.Create(this);
	m_TTctrl.SetMaxTipWidth(200);

	int i;
	for(i=0;i<TABCELL_INVENTORY_NUM;++i)
		m_TTctrl.AddTool(&m_InvenItem[i]);

	for(i=0;i<eWearedItem_Max;++i)
		m_TTctrl.AddTool(&m_WearItem[i]);

	for(i=0;i<SLOT_MUGONG_NUM;++i)
		m_TTctrl.AddTool(&m_SkillMuGong[i]);

	for(i=0;i<SLOT_JINBUB_NUM;++i)
		m_TTctrl.AddTool(&m_SkillJinBub[i]);

	for(i=0;i<TABCELL_PYOGUK_NUM;++i)
		m_TTctrl.AddTool(&m_PyoGukItem[i]);

	for(i=0;i<MAX_ABILITY_NUM_PER_GROUP;++i)
		m_TTctrl.AddTool(&m_Ability[i]);

	EnableToolTips(TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CCharacterInfo::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
//	if( m_pDocument && !m_pDocument->IsReadOnly() )
	if( m_pDocument )
	if( m_pDocument->IsReadOnly() )
	{
		m_btnChangeLoginPoint.EnableWindow( FALSE );
		m_btnSave.EnableWindow( FALSE );
	}
	else
	{
		if( m_wPower == eOperPower_Master )
		{
			m_btnChangeLoginPoint.EnableWindow( TRUE );
			m_btnSave.EnableWindow( TRUE );
		}
		else
		{
			m_btnChangeLoginPoint.EnableWindow( FALSE );
			m_btnSave.EnableWindow( FALSE );
		}
	}

	switch( nCode )
	{
	case BN_CLICKED:
		{
			ButtonControlCommand(nID);
		}
		break;
	case EN_KILLFOCUS:
		{
			EditControlCommand(nID);
		}
		break;
	case EN_CHANGE:
		{
			if( (nID >= IDC_CHAR_NAME && nID < IDC_CHAR_ABILITYPOINT+1) )
			{
				CMREdit* pEdit = (CMREdit*)GetDlgItem(nID);
				if( pEdit ) pEdit->SetChanged(TRUE);
			}
		}
		break;
	}

	return CLDPropPage::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

CMRButton* CCharacterInfo::CheckButtonEvent(HWND hWnd, POSTYPE& pos, BYTE& type)
{
	int i;
	for(i=0;i<TABCELL_INVENTORY_NUM;++i)
	{
		if(m_InvenItem[i].m_hWnd == hWnd)
		{
			type = 0;
			pos = i;
			return &m_InvenItem[i];
		}
	}

	for(i=0;i<eWearedItem_Max;++i)
	{
		if(m_WearItem[i].m_hWnd == hWnd)
		{
			type = 1;
			pos = i;
			return &m_WearItem[i];
		}
	}

	for(i=0;i<SLOT_MUGONG_NUM;++i)
	{
		if(m_SkillMuGong[i].m_hWnd == hWnd)
		{
			type = 3;
			pos = i;
			return &m_SkillMuGong[i];
		}
	}

	for(i=0;i<SLOT_JINBUB_NUM;++i)
	{
		if(m_SkillJinBub[i].m_hWnd == hWnd)
		{
			type = 4;
			pos = i;
			return &m_SkillJinBub[i];
		}
	}

	for(i=0;i<TABCELL_PYOGUK_NUM;++i)
	{
		if(m_PyoGukItem[i].m_hWnd == hWnd)
		{
			type = 2;
			pos = i;
			return &m_PyoGukItem[i];
		}
	}

	for(i=0;i<MAX_ABILITY_NUM_PER_GROUP;++i)
	{
		if(m_Ability[i].m_hWnd == hWnd)
		{
			type = 5;
			pos = i;
			return &m_Ability[i];
		}
	}

	return NULL;
}

void CCharacterInfo::EditControlCommand(UINT nId)
{
	WORD wPos;
	CChangeData* pData = NULL;
	CString str1, str2;
	DWORD before_data, after_data;

	if( nId >= IDC_CHAR_NAME && nId < IDC_CHAR_ABILITYPOINT+1 )
	{
		wPos = (WORD)(nId - IDC_CHAR_NAME);	// Tool 데이터의 배열인덱스
		
		if( wPos > eCharTemp_ABILITYPOINT )
			return;

		BYTE type = 0;

		/////////////////////////////////////////////////////
		// error check (이전 데이터로 전환)
		TABLE_KIND kind = GetTableKindByControlID(nId);

		if( m_StatusEdit[wPos].GetWindowTextLength() == 0 || kind == eTK_None )
		{
			if( m_StatusEdit[wPos].IsTextTypeData() )
				m_StatusEdit[wPos].GetStringData(str1.GetBuffer(0));
			else
			{
				before_data = m_StatusEdit[wPos].GetData();
				str1.Format("%d", before_data);
			}

			m_StatusEdit[wPos].SetWindowTextEx(str1);
			m_StatusEdit[wPos].SetChanged(FALSE);
			return;
		}

		// 변경된 데이터가 있는지 체크한다.
		if( !m_StatusEdit[wPos].IsChanged() )
			return;
		else
			m_StatusEdit[wPos].SetChanged(FALSE);
		/////////////////////////////////////////////////////

		// str1:변경Data, str2:이전Data
		m_StatusEdit[wPos].GetWindowText(str2);
		if( m_StatusEdit[wPos].IsTextTypeData() )
		{
			m_StatusEdit[wPos].GetStringData(str1.GetBuffer(0));

			if( strcmp(str1, str2) == 0 ) // 변경된 데이타가 같은경우
			{
				CMREdit* pEdit = (CMREdit*)GetDlgItem(nId);
				pEdit->ResetEditTextColor();
				pEdit->SetInSave(FALSE);
				m_pDocument->GetChangeManager()->Delete(nId);
				return;
			}
		}
		else
		{
			before_data = m_StatusEdit[wPos].GetData();

			m_StatusEdit[wPos].CheckMaxValue(str2.GetBuffer(0));
			m_StatusEdit[wPos].GetWindowText(str2);
			after_data = (DWORD)atoi(str2.GetBuffer(0));

			if( before_data == after_data )
			{
				CMREdit* pEdit = (CMREdit*)GetDlgItem(nId);
				pEdit->ResetEditTextColor();
				pEdit->SetInSave(FALSE);
				m_pDocument->GetChangeManager()->Delete(nId);
				return;
			}

			str1.Format("%d", before_data);
			type = 1;
		}

		pData = (CChangeData*)m_pDocument->GetChangeManager()->ExistChangeInfo(nId);

		if( !pData )
		{
			pData = new CChangeData;

			pData->Init(eOp_Update, kind, wPos);
			pData->SetData(str1.GetBuffer(0), str2.GetBuffer(0), type);
		}
		else
		{
			pData->Init(eOp_Update, kind, wPos);
			pData->SetData(str1.GetBuffer(0), str2.GetBuffer(0), type);
			return;
		}
	}
	else
		return;
	
	// Control의 Index를 Key값으로 한다.
	m_pDocument->GetChangeManager()->Add(pData, nId);
	CMREdit* pEdit = (CMREdit*)GetDlgItem(nId);
	pEdit->SetEditTextColor(COLOR_EDIT_CHANGED);
	pEdit->SetInSave();
}

TABLE_KIND CCharacterInfo::GetTableKindByControlID(UINT nId)
{
	TABLE_KIND kind = eTK_None;

	if( nId >= IDC_CHAR_NAME &&	nId < IDC_CHAR_ABILITYPOINT+1 )
	{
		if( nId == IDC_CHAR_PYOGUKMONEY )
			kind = eTK_PyogukInfo;
		else
			kind = eTK_CharacterInfo;
	}

	return kind;
}

void CCharacterInfo::ButtonControlCommand(UINT nId)
{
	CMRButton* pBtn = (CMRButton*)GetDlgItem( nId );
	if( !pBtn ) return;

	WORD wPos;
	int tab, result;

	if( nId >= IDC_BTN_INVENOBJ1 && nId < IDC_BTN_INVENOBJ20+1 )	 // 인벤토리
	{
		tab = m_cbInven.GetCurSel()*TABCELL_INVENTORY_NUM;
		wPos = tab + nId - IDC_BTN_INVENOBJ1;

		// 기본 정보
		ITEMBASE* origin = &m_pDocument->m_sInven_org.Inventory[wPos];
		ITEMBASE* base_before = &m_pDocument->m_sInven.Inventory[wPos];
		ITEMBASE base_after;

		// 옵션 정보
		ITEM_OPTION_INFO* pOptionInfoOrigin = NULL;
		ITEM_OPTION_INFO* pOptionInfo = NULL;
		ITEM_OPTION_INFO Option_after;
		if( GAMERESRCMNGR->IsOptionItem( origin->wIconIdx, origin->Durability ) )
			pOptionInfoOrigin = m_pDocument->GetItemOptionInfoOrigin( origin->Durability );
		if( GAMERESRCMNGR->IsOptionItem( base_before->wIconIdx, base_before->Durability ) )
			pOptionInfo = m_pDocument->GetItemOptionInfo( base_before->Durability );

		result = ItemEdit( eIF_INVENTORY, wPos, base_before, base_after, pOptionInfo, Option_after );

		if( result == eMR_Cancel || result == eMR_Abort ) return;
		if( result == eMR_Delete )
		{
			if( MessageBox("Are You Really Delete?",NULL, MB_YESNO) == IDNO )
				return;
		}
		
		base_after.Position = TP_INVENTORY_START + wPos;
		ResultItemEdit( eIF_INVENTORY, result, TP_INVENTORY_START + wPos, nId, origin, pOptionInfoOrigin, base_before, &base_after, pOptionInfo, &Option_after );
	}
	else if( nId >= IDC_BTN_WEAROBJ1 && nId < IDC_BTN_WEAROBJ10+1 )	// 착용창
	{ 
		// 장착 버튼
		wPos = nId - IDC_BTN_WEAROBJ1;
		
		ITEMBASE* origin = &m_pDocument->m_sInven_org.WearedItem[wPos];
		ITEMBASE* base_before = &m_pDocument->m_sInven.WearedItem[wPos];
		ITEMBASE base_after;

		ITEM_OPTION_INFO* pOptionInfoOrigin = NULL;
		ITEM_OPTION_INFO* pOptionInfo = NULL;
		ITEM_OPTION_INFO Option_after;
		if( GAMERESRCMNGR->IsOptionItem(origin->wIconIdx, origin->Durability) )
			pOptionInfoOrigin = m_pDocument->GetItemOptionInfoOrigin(origin->Durability);
		if( GAMERESRCMNGR->IsOptionItem(base_before->wIconIdx, base_before->Durability) )
			pOptionInfo = m_pDocument->GetItemOptionInfo(base_before->Durability);
		
		result = ItemEdit( eIF_WEAR, wPos, base_before, base_after, pOptionInfo, Option_after );
		if( result == eMR_Cancel || result == eMR_Abort ) return;
		if( result == eMR_Delete )
		{
			if( MessageBox("Are You Really Delete?",NULL, MB_YESNO) == IDNO )
				return;
		}
		
		base_after.Position = TP_WEAR_START + wPos;
		ResultItemEdit( eIF_WEAR, result, TP_WEAR_START + wPos, nId, origin, pOptionInfoOrigin, base_before, &base_after, pOptionInfo, &Option_after );
	}
	else if( nId >= IDC_BTN_PYOGUK1 && nId < IDC_BTN_PYOGUK30+1 )	// 창고
	{ 
		tab = m_cbPyoguk.GetCurSel()*TABCELL_PYOGUK_NUM;

		wPos = tab + nId - IDC_BTN_PYOGUK1;
		ITEMBASE* origin = &m_pDocument->m_sPyoguk_org.Pyokuk[wPos];
		ITEMBASE* base_before = &m_pDocument->m_sPyoguk.Pyokuk[wPos];
		ITEMBASE base_after;

		ITEM_OPTION_INFO* pOptionInfoOrigin = NULL;
		ITEM_OPTION_INFO* pOptionInfo = NULL;
		ITEM_OPTION_INFO Option_after;
		if( GAMERESRCMNGR->IsOptionItem(origin->wIconIdx, origin->Durability) )
			pOptionInfoOrigin = m_pDocument->GetItemOptionInfoOrigin(origin->Durability);
		if( GAMERESRCMNGR->IsOptionItem(base_before->wIconIdx, base_before->Durability) )
			pOptionInfo = m_pDocument->GetItemOptionInfo(base_before->Durability);

		result = ItemEdit( eIF_PHYOGUK, wPos, base_before, base_after, pOptionInfo, Option_after );

		if( result == eMR_Cancel || result == eMR_Abort ) return;
		if( result == eMR_Delete )
		{
			if( MessageBox("Are You Really Delete?",NULL, MB_YESNO) == IDNO )
				return;
		}
		
		base_after.Position = TP_PYOGUK_START + wPos;
		ResultItemEdit( eIF_PHYOGUK, result, TP_PYOGUK_START + wPos, nId, origin, pOptionInfoOrigin, base_before, &base_after, pOptionInfo, &Option_after );
	}
	else if( nId >= IDC_BTN_SKILL1 && nId < IDC_BTN_JINBUB5+1 )		// 무공
	{ 
		// 무공 버튼
		wPos = nId - IDC_BTN_SKILL1;
		
		MUGONGBASE* origin = &m_pDocument->m_sMugong_org.mugong[wPos];
		MUGONGBASE* base_before = &m_pDocument->m_sMugong.mugong[wPos];
		MUGONGBASE base_after;

		if( nId < IDC_BTN_JINBUB1 )
		{ // 일반 무공 
			result = MugongEdit(eIF_MUGONG, wPos, base_before, base_after);
		}
		else
		{ // 진법
			result = MugongEdit(eIF_JINBUB, wPos, base_before, base_after);
		}

		if( result == eMR_Cancel || result == eMR_Abort ) return;
		if( result == eMR_Delete )
		{
			if( MessageBox("Are You Really Delete?",NULL, MB_YESNO) == IDNO )
				return;
		}
		
		base_after.Position = TP_MUGONG_START + wPos;
		ResultMugongEdit( result, TP_MUGONG_START + wPos, nId, origin, base_before, &base_after );
	}
	else if( nId >= IDC_BTN_ABLITY1 && nId < IDC_BTN_ABLITY40+1 )		// 특기
	{ 
		tab = m_cbAbility.GetCurSel();
		wPos = nId - IDC_BTN_ABLITY1;

		WORD wAbilityIdx = (WORD)m_Ability[wPos].GetData();
		if( wAbilityIdx == 0 )
			return;

		WORD n = wAbilityIdx - (ABILITY_STARTINDEX_BATTLE + ABILITY_STARTINDEX_INTERVAL*tab);

		char origin = m_pDocument->m_sAbility_org.AbilityDataArray[tab][n];
		char base_before = m_pDocument->m_sAbility.AbilityDataArray[tab][n];
		char base_after;

		result = AbilityEdit( wPos, wAbilityIdx, base_before, base_after );
		if( result == eMR_Cancel || result == eMR_Abort ) return;

		m_pDocument->m_sAbility.AbilityDataArray[tab][n] = base_after;

		if( !m_pDocument->IsSetSaveFlag(eTK_AbilityInfo) )
			m_pDocument->UpdateData(MAKEWPARAM(eTK_AbilityInfo, 0));

		CMRButton* btn = (CMRButton*) GetDlgItem(nId);
		btn->SetColorText(COLOR_BUTTON_CHANGED);
	}
	else if( nId >= IDC_BTNINVEN_ERROR1 && nId < IDC_BTNINVEN_ERROR10+1 )	 // error
	{
		wPos = nId - IDC_BTNINVEN_ERROR1;

		// 기본 정보
		ITEMBASE* base_before = &m_pDocument->m_InvenInfoError.m_ItemInfo[wPos];
		
		int pos = GetEmptyPosition( 0 );
		if( pos == -1 )
		{
			MessageBox( "Can't Change Item Position!!" );
			return;
		}

		base_before->Position = pos;
		memcpy( &m_pDocument->m_sInven.Inventory[pos], base_before, sizeof(ITEMBASE) );

		ResultPositionEdit( 0, base_before );

		DWORD id = IDC_BTN_INVENOBJ1 + pos;

		if( pos >= 80 )
			return;

		char buff[256] = {0,};
		ITEM_INFO* pInfo;

		if( base_before->wIconIdx != 0)
		{
			pInfo = GAMERESRCMNGR->GetItemInfo(base_before->wIconIdx);
			if( pInfo )
				sprintf(buff, pInfo->ItemName);
			else
				sprintf(buff, "%d", base_before->wIconIdx);
		}
		else
			sprintf(buff, "");

		CMRButton* btn = (CMRButton*)GetDlgItem(id);
		btn->SetWindowText(buff);
		btn->SetData(base_before->wIconIdx);
		btn->SetColorText(COLOR_BUTTON_CHANGED);

		btn = (CMRButton*)GetDlgItem(nId);
		btn->Release();
		btn->ShowWindow( FALSE );
	}
	else if( nId >= IDC_BTN_WAREERROR1 && nId < IDC_BTN_WAREERROR5+1 )
	{
		wPos = nId - IDC_BTN_WAREERROR1;

		ITEMBASE* base = &m_pDocument->m_WareError.Pyokuk[wPos];
		
		int pos = GetEmptyPosition( 1 );
		if( pos == -1 )
		{
			MessageBox( "Can't Change Item Position!!" );
			return;
		}

		base->Position = pos+TP_PYOGUK_START;
		memcpy( &m_pDocument->m_sPyoguk.Pyokuk[pos], base, sizeof(ITEMBASE) );
		ResultPositionEdit( 1, base );

		DWORD id = IDC_BTN_PYOGUK1 + pos;
		if( pos >= TABCELL_PYOGUK_NUM )
			return;

		// error btn
		CMRButton* btn = (CMRButton*)GetDlgItem(nId);
		btn->Release();
		btn->ShowWindow( FALSE );

		char buff[256] = {0,};
		ITEM_INFO* pInfo;
		if( base->wIconIdx != 0)
		{
			pInfo = GAMERESRCMNGR->GetItemInfo(base->wIconIdx);
			if( pInfo )
				sprintf(buff, pInfo->ItemName);
			else
				sprintf(buff, "%d", base->wIconIdx);
		}
		else
			sprintf(buff, "");

		btn = (CMRButton*)GetDlgItem(id);
		btn->SetWindowText(buff);
		btn->SetData(base->wIconIdx);
		btn->SetColorText(COLOR_BUTTON_CHANGED);
	}
	else if( nId >= IDC_BTN_SKILLERROR1 && nId < IDC_BTN_SKILLERROR5+1 )
	{
		wPos = nId - IDC_BTN_SKILLERROR1;

		MUGONGBASE* base = &m_pDocument->m_MugongError.m_MugongBase[wPos];

		int pos = -1;
		if( base->Position < TP_MUGONG_END )
			pos = GetEmptyPosition( 2 );
		else
			pos = GetEmptyPosition( 3 );
		if( pos == -1 )
		{
			MessageBox( "Can't Change Item Position!!" );
			return;
		}

		base->Position = pos+TP_MUGONG_START;
		memcpy( &m_pDocument->m_sMugong.mugong[pos], base, sizeof(MUGONGBASE) );
		ResultPositionEdit( 2, base );

		DWORD id = IDC_BTN_SKILL1 + pos;

		// error btn
		CMRButton* btn = (CMRButton*)GetDlgItem(nId);
		btn->Release();
		btn->ShowWindow( FALSE );


		char buff[256]={0,};
		SKILLINFO* pInfo = NULL;

		if( base->wIconIdx != 0 )
		{
			pInfo = GAMERESRCMNGR->GetSkillInfo( base->wIconIdx );
			if( pInfo )
				sprintf( buff, pInfo->SkillName );
			else
				sprintf( buff, "%d", base->wIconIdx );
		}
		else
			sprintf( buff, "" );

		btn = (CMRButton*)GetDlgItem(id);
		btn->SetWindowText(buff);
		btn->SetData(base->wIconIdx);
		btn->SetColorText(COLOR_BUTTON_CHANGED);
	}
	else if( nId == IDC_BTN_INVEN_RECOVER )
	{
		m_pItemRecoverDlg = new CItemRecoverDlg;
		int result = m_pItemRecoverDlg->DoModal();
		if( result == 99 )
		{
			int total = 0;
			int count = m_pItemRecoverDlg->m_nItemNum / 5;
			int r = m_pItemRecoverDlg->m_nItemNum % 5;
			if( r )
				total = count + 1;
			else
				total = count;
			
			if( total > GetEmptyPositionCount( 0 ) )
			{
				MessageBox( "인벤토리가 충분하지 않습니다.!!", "Error!!" );
			}
			else
			{
				RecoverInven( m_pItemRecoverDlg->m_nItemIdx, count, r );
			}
		}
		SAFE_DELETE(m_pItemRecoverDlg);
	}
	else
	{ // 처리 안함.
	}
}

void CCharacterInfo::RecoverInven( DWORD dwItemIdx, int nCount, int nR )
{
	CChangeManager* pManager = m_pDocument->GetChangeManager();
	CChangeItem* pItem = NULL;
	ITEMBASE* pAItemBase = NULL;
	ITEM_INFO* pInfo = NULL;
	CMRButton* btn = NULL;
	char buff[256] = {0,};
	
	for( int i = 0; i < nCount; ++i )
	{
		int pos = GetEmptyPosition( 0 );
		if( pos == -1 )
			return;
		pAItemBase = &m_pDocument->m_sInven.Inventory[pos];
		pAItemBase->wIconIdx = dwItemIdx;
		pAItemBase->Position = pos;
		pAItemBase->Durability = 5;
		pItem = new CChangeItem;
		pItem->Init( eOp_Insert, eTK_InventoryItem, pos );
		ITEM_OPTION_INFO Option;
		memset( &Option, 0, sizeof(Option) );
		pItem->SetData( pAItemBase, pAItemBase, &Option, &Option );

		pManager->Add( pItem, pos, FALSE );

		pInfo = GAMERESRCMNGR->GetItemInfo(dwItemIdx);
		if( pInfo )
			sprintf(buff, pInfo->ItemName);
		else
			sprintf(buff, "%d", dwItemIdx);

		int nTab = m_cbInven.GetCurSel();
		int nControlIdx = (pos / 20);
		if( nTab == nControlIdx )
		{
			nControlIdx = IDC_BTN_INVENOBJ1 + (pos%20);
			btn = (CMRButton*)GetDlgItem(nControlIdx);
			btn->SetWindowText(buff);
			btn->SetData(dwItemIdx);
			btn->SetColorText(COLOR_BUTTON_CHANGED);
		}
	}

	if( nR )
	{
		int pos = GetEmptyPosition( 0 );
		if( pos == -1 )
			return;
		pAItemBase = &m_pDocument->m_sInven.Inventory[pos];
		pAItemBase->wIconIdx = dwItemIdx;
		pAItemBase->Position = pos;
		pAItemBase->Durability = nR;
		pItem = new CChangeItem;
		pItem->Init( eOp_Insert, eTK_InventoryItem, pos );
		ITEM_OPTION_INFO Option;
		memset( &Option, 0, sizeof(Option) );
		pItem->SetData( pAItemBase, pAItemBase, &Option, &Option );

		pManager->Add( pItem, pos, FALSE );

		pInfo = GAMERESRCMNGR->GetItemInfo(dwItemIdx);
		if( pInfo )
			sprintf(buff, pInfo->ItemName);
		else
			sprintf(buff, "%d", dwItemIdx);

		int nTab = m_cbInven.GetCurSel();
		int nControlIdx = (pos / 20);
		if( nTab == nControlIdx )
		{
			nControlIdx = IDC_BTN_INVENOBJ1 + (pos%20);
			btn = (CMRButton*)GetDlgItem(nControlIdx);
			btn->SetWindowText(buff);
			btn->SetData(dwItemIdx);
			btn->SetColorText(COLOR_BUTTON_CHANGED);
		}
	}
}

int CCharacterInfo::GetEmptyPositionCount( WORD wType )
{
	int count = 0;
	int i = 0;
	switch( wType )
	{
	case 0:		// inven
		{
			for( i = 0; i < 80; ++i )
			{
				if( m_pDocument->m_sInven.Inventory[i].dwDBIdx == 0 &&
					m_pDocument->m_sInven.Inventory[i].wIconIdx == 0 )
					++count;
			}
		}
		break;
	case 1:		// 창고
		{
			for( i = 0; i < TABCELL_PYOGUK_NUM*m_pDocument->m_sPyoguk.PyokukCount; ++i )
			{
				if( m_pDocument->m_sPyoguk.Pyokuk[i].dwDBIdx == 0 )
					++count;
			}
		}
		break;
	}

	return count;
}

int CCharacterInfo::GetEmptyPosition( DWORD dwType )
{
	int i = 0;
	switch( dwType )
	{
	case 0:		// inven
		{
			for( i = 0; i < 80; ++i )
			{
				if( m_pDocument->m_sInven.Inventory[i].dwDBIdx == 0 &&
					m_pDocument->m_sInven.Inventory[i].wIconIdx == 0 )
					return i;
			}
			return -1;
		}
		break;
	case 1:		// 창고
		{
			for( i = 0; i < TABCELL_PYOGUK_NUM*m_pDocument->m_sPyoguk.PyokukCount; ++i )
			{
				if( m_pDocument->m_sPyoguk.Pyokuk[i].dwDBIdx == 0 )
					return i;
			}
			return -1;
		}
		break;
	case 2:		// 무공
		{
			for( i = 0; i < SLOT_MUGONG_NUM; ++i )
			{
				if( m_pDocument->m_sMugong.mugong[i].dwDBIdx == 0 )
					return i;
			}
			return -1;
		}
		break;
	case 3:		// 진법
		{
			for( i = SLOT_MUGONG_NUM; i < SLOT_MUGONGTOTAL_NUM; ++i )
			{
				if( m_pDocument->m_sMugong.mugong[i].dwDBIdx == 0 )
					return i;
			}
			return -1;
		}
		break;
	}

	return -1;
}

void CCharacterInfo::ResultPositionEdit( DWORD dwType, ICONBASE* pItem )
{
	if( !pItem )
		return;

	CChangePosition* pPosition = new CChangePosition;

	switch( dwType )
	{
	case 0:		// inven
		{
			ITEMBASE* pp = (ITEMBASE*)pItem;
			pPosition->SetData( 0, m_pDocument->m_dwCharIdx, pItem->dwDBIdx, pItem->Position );
		}
		break;
	case 1:		// 창고
		{
			ITEMBASE* pp = (ITEMBASE*)pItem;
			pPosition->SetData( 1, m_pDocument->m_dwUserIdx, pItem->dwDBIdx, pItem->Position );
		}
		break;
	case 2:		// 무공
		{
			MUGONGBASE* pp = (MUGONGBASE*)pItem;
			pPosition->SetData( 2, m_pDocument->m_dwCharIdx, pItem->dwDBIdx, pItem->Position );
		}
		break;
	}	

	m_pDocument->GetChangeManager()->AddPositionTable( pPosition, pItem->dwDBIdx );
}

int CCharacterInfo::ItemEdit( INFOFIELD field, WORD wControlId, const ITEMBASE* base_before, ITEMBASE& base_after, 
								const ITEM_OPTION_INFO* before, ITEM_OPTION_INFO& after )
{
	int result = 0;

	m_pItemDlg = new CItemDlg;

	WORD wWear = 0;
	if( field == eIF_WEAR )
	{
		switch(wControlId)
		{
		case eWearedItem_Hat: wWear = eEQUIP_ITEM_HAT; break;
		case eWearedItem_Weapon: wWear = eEQUIP_ITEM_WEAPON; break;
		case eWearedItem_Dress: wWear = eEQUIP_ITEM_DRESS; break;
		case eWearedItem_Shoes: wWear = eEQUIP_ITEM_SHOES; break;
	
		case eWearedItem_Ring1:
		case eWearedItem_Ring2: wWear = eEQUIP_ITEM_RING; break;

		case eWearedItem_Cape: wWear = eEQUIP_ITEM_CAPE; break;
		case eWearedItem_Necklace: wWear = eEQUIP_ITEM_NECKLACE; break;
		case eWearedItem_Armlet: wWear = eEQUIP_ITEM_ARMLET; break;
		case eWearedItem_Belt: wWear = eEQUIP_ITEM_BELT; break;
		}

		CString str;
		m_StatusEdit[eCharTemp_LEVEL].GetWindowText(str);
		m_pItemDlg->SetLevel( atoi(str.GetBuffer(0)) );
	}
	
	m_pItemDlg->Init(field, base_before, before, wControlId, wWear );
	result = m_pItemDlg->DoModal();
				
	memcpy(&base_after, m_pItemDlg->GetItemBase(), sizeof(ITEMBASE) );
	memcpy(&after, m_pItemDlg->GetItemOption(), sizeof(ITEM_OPTION_INFO) );

	SAFE_DELETE(m_pItemDlg);

	return result;
}

int CCharacterInfo::MugongEdit( INFOFIELD field, WORD wControlId, MUGONGBASE* base_before, MUGONGBASE& base_after )
{
	int result = 0;

	CString str;
	m_StatusEdit[eCharTemp_LEVEL].GetWindowText(str);
//	if( atoi(str.GetBuffer(0)) < 4 )
//	{
//		MessageBox("무공을 배울수 없는 레벨입니다.");
//		return eMR_Cancel;
//	}

	m_pMugongDlg = new CMugongDlg;
	m_pMugongDlg->Init(field, base_before, wControlId, (LEVELTYPE)atoi(str.GetBuffer(0)));

	result = m_pMugongDlg->DoModal();

	memcpy(&base_after, m_pMugongDlg->GetMugong(), sizeof(MUGONGBASE) );

	SAFE_DELETE(m_pMugongDlg);

	return result;
}

int CCharacterInfo::AbilityEdit( WORD wPos, WORD wAbilityIdx, char base_before, char& base_after )
{
	int result = 0;

	CAbilityInfo* pAbility = GAMERESRCMNGR->GetAbilityInfo(wAbilityIdx);
	if( pAbility == NULL )
		return eMR_Cancel;

	m_pAbility = new CAbilityDlg;
	m_pAbility->Init( wPos, pAbility, m_pDocument->GetCharacterAbillityLevel(pAbility->GetKind(), wAbilityIdx) );

	result = m_pAbility->DoModal();

	base_after = (char)(m_pAbility->GetAbilityLevel() + ABILITYLEVEL_DB_KEY);

	SAFE_DELETE(m_pAbility);

	return result;
}

BOOL CCharacterInfo::ResultItemEdit( INFOFIELD field, int result, WORD wPos, int nControlIdx, ITEMBASE* Origin, ITEM_OPTION_INFO* OriginOption,
										ITEMBASE* pBItem, ITEMBASE* pAItem, ITEM_OPTION_INFO* pBOption, ITEM_OPTION_INFO* pAOption )
{
	// 실제 아이템의 포지션을 받는다. wPos
	CChangeManager* pManager = m_pDocument->GetChangeManager();

	// 어떤 DB Table에 넣을것인지를 알아 온다.
	TABLE_KIND kind = pManager->GetTableKind_ByField(field);
	if( kind == eTK_None )
		return FALSE;

	BOOL bAdd = FALSE;
	CChangeItem* pItem = (CChangeItem*)pManager->ExistChangeInfo(wPos, FALSE);

	if( !pItem ) // 기존 수정 정보가 있는지 검사한다. (있어도 무시!)
	{
		pItem = new CChangeItem;
		bAdd = TRUE;
	}

	if( result == eMR_Delete )// Delete인 경우
	{ 
		/////////////////// Local Info의 수정
		if( GAMERESRCMNGR->IsOptionItem(pBItem->wIconIdx, pBItem->Durability) )
			m_pDocument->DelLocalOptionInfo(pBItem->Durability);

		memset(pBItem, 0, sizeof(ITEMBASE));
		memset(pAItem, 0, sizeof(ITEMBASE));
		///////////////////

		/////////////////// DB Info의 수정
		pItem->Init( eOp_Delete, kind, wPos );
		pItem->SetData(Origin, pAItem, OriginOption, pAOption);
		///////////////////
	}
	else if( result == eMR_Edit )
	{
		/////////////////// Local Info의 수정
		if( GAMERESRCMNGR->IsOptionItem(pBItem->wIconIdx, pBItem->Durability) )
			m_pDocument->DelLocalOptionInfo(pBItem->Durability);

		if( pAItem->Durability == 9999 ) // 옵션 셋팅
		{
			if( OriginOption && OriginOption->dwOptionIdx != 0 )
			{
				pAOption->dwOptionIdx = OriginOption->dwOptionIdx;
				m_pDocument->AddLocalOptionInfo(pAOption, FALSE);
			}
			else
			{
				WORD wOptionIdx = m_pDocument->AddLocalOptionInfo(pAOption, TRUE);
				pAOption->dwOptionIdx = wOptionIdx;
			}
			pAItem->Durability = (WORD)pAOption->dwOptionIdx;
		}

		memcpy(pBItem, pAItem, sizeof(ITEMBASE)); // 기본 아이템 변경
		///////////////////

		/////////////////// DB Info의 수정
		if( Origin->dwDBIdx == 0 ) // Insert인 경우
		{
			pItem->Init( eOp_Insert, kind, wPos );
			pItem->SetData(Origin, pAItem, OriginOption, pAOption);
		}
		else // Update인 경우
		{ 
			pItem->Init( eOp_Update, kind, wPos );
			pItem->SetData(Origin, pAItem, OriginOption, pAOption);
		}
		///////////////////
	}
	else
	{
		ASSERT(0);
		return FALSE;
	}

	if( bAdd )
		pManager->Add( pItem, wPos, FALSE );

	////////////////////////////////////////////////////////////
	// Button Text Setting
	char buff[256] = {0,};
	ITEM_INFO* pInfo;

	if( pAItem->wIconIdx != 0)
	{
		pInfo = GAMERESRCMNGR->GetItemInfo(pAItem->wIconIdx);
		if( pInfo )
			sprintf(buff, pInfo->ItemName);
		else
			sprintf(buff, "%d", pAItem->wIconIdx);
	}
	else
		sprintf(buff, "");

	CMRButton* btn = (CMRButton*)GetDlgItem(nControlIdx);
	btn->SetWindowText(buff);
	btn->SetData(pAItem->wIconIdx);
	btn->SetColorText(COLOR_BUTTON_CHANGED);
	////////////////////////////////////////////////////////////

	return TRUE;
}

void CCharacterInfo::ResultMugongEdit( int result, WORD wPos, int nControlIdx, MUGONGBASE* Origin, MUGONGBASE* pBMugong, MUGONGBASE* pAMugong )
{
	////////////////////////////////////////////////////////////////////////////////////////////
	//// 무공은 중복이 허용되지 않으므로 체크한다.
	int i;
	for(i=0;i<SLOT_MUGONG_NUM;++i)
	{
		if( i != wPos - TP_MUGONG_START && pAMugong->wIconIdx != 0 && m_SkillMuGong[i].GetData() == pAMugong->wIconIdx )
		{
			::AfxGetMainWnd()->MessageBox("You Have Already This Skill!!");
			return;
		}
	}
	for(i=0;i<SLOT_JINBUB_NUM;++i)
	{
		if( i != wPos - TP_JINBUB_START && pAMugong->wIconIdx != 0 && m_SkillJinBub[i].GetData() == pAMugong->wIconIdx )
		{
			::AfxGetMainWnd()->MessageBox("You Have Already This Skill!!");
			return;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////

	CChangeManager* pManager = m_pDocument->GetChangeManager();

	BOOL bAdd = FALSE;
	CChangeMugong* pMugong = (CChangeMugong*)pManager->ExistChangeInfo(wPos, FALSE);

	if( !pMugong ) // 기존 수정 정보가 있는지 검사한다. (있어도 무시!)
	{
		pMugong = new CChangeMugong;
		bAdd = TRUE;
	}

	if( result == eMR_Delete ) // Delete
	{
		/////////////////// Local Info의 수정
		memset(pBMugong, 0, sizeof(MUGONGBASE));
		memset(pAMugong, 0, sizeof(MUGONGBASE));
		///////////////////

		/////////////////// DB Info의 수정
		pMugong->Init( eOp_Delete, eTK_MugongInfo, wPos );
		///////////////////
	}
	else if( result == eMR_Edit )
	{
		/////////////////// Local Info의 수정
		memcpy(pBMugong, pAMugong, sizeof(MUGONGBASE));
		///////////////////

		/////////////////// DB Info의 수정
		if( Origin->dwDBIdx == 0 ) // Insert
			pMugong->Init( eOp_Insert, eTK_MugongInfo, wPos );
		else // Update
			pMugong->Init( eOp_Update, eTK_MugongInfo, wPos );
		///////////////////
	}
	else
	{
		ASSERT(0);
		return;
	}

	pMugong->SetData( Origin, pAMugong );

	if( bAdd )
		pManager->Add( pMugong, wPos, FALSE );

	////////////////////////////////////////////////////////////
	// Button Text Setting
	char buff[256]={0,};
	SKILLINFO* pInfo;

	if( pAMugong->wIconIdx != 0)
	{
		pInfo = GAMERESRCMNGR->GetSkillInfo(pAMugong->wIconIdx);
		if( pInfo )
			sprintf(buff, pInfo->SkillName);
		else
			sprintf(buff, "%d", pAMugong->wIconIdx);
	}
	else
		sprintf(buff, "");

	CMRButton* btn = (CMRButton*) GetDlgItem(nControlIdx);
	btn->SetWindowText(buff);
	btn->SetData(pAMugong->wIconIdx);
	btn->SetColorText(COLOR_BUTTON_CHANGED);
	////////////////////////////////////////////////////////////
}

void CCharacterInfo::OnCharacterBtnAddpyoguk() 
{
	// TODO: Add your control notification handler code here
	WORD wCount = m_cbPyoguk.GetCount();
	PHYOKUK_TOTALINFO* pyoguk = &m_pDocument->m_sPyoguk;
	char buff[256] = {0,};
	ITEM_INFO* pInfo;

	if( wCount < TAB_PYOGUK_NUM )
	{
		if( !m_cbPyoguk.IsWindowEnabled() )
		{
			EnablePyogukInfoControl();
			m_StatusEdit[eCharTemp_PYOGUKMONEY].SetWindowTextEx("0", 1);
		}

		sprintf(buff,"Tab%d", wCount+1);
		m_cbPyoguk.InsertString(wCount, buff);
		m_cbPyoguk.SetCurSel(wCount);

		int tab = wCount*TABCELL_PYOGUK_NUM;

		for(int i=0;i<TABCELL_PYOGUK_NUM;++i)
		{
			if( pyoguk->Pyokuk[tab+i].wIconIdx != 0)
			{
				pInfo = GAMERESRCMNGR->GetItemInfo(pyoguk->Pyokuk[tab+i].wIconIdx);
				if( pInfo )
					sprintf(buff, pInfo->ItemName);
				else
					sprintf(buff, "%d", pyoguk->Pyokuk[tab+i].wIconIdx);
			}
			else
				sprintf(buff, "");

			m_PyoGukItem[i].SetWindowText(buff);
			m_PyoGukItem[i].SetData(pyoguk->Pyokuk[tab+i].wIconIdx);
		}

		if( (wCount+1) == TAB_PYOGUK_NUM )
			m_btnPyogukAdd.EnableWindow(FALSE);

		m_pDocument->SetSaveFlag(eCF_PyogukInfo);
	}
	else
		return;

	pyoguk->PyokukCount = wCount;
	m_StatusEdit[eCharTemp_PYOGUKMONEY].SetMaxValue(1000000 + wCount*1000000);
}

void CCharacterInfo::OnCharacterBtnSave() 
{
	if( m_wPower != eOperPower_Master )
		return;

	m_pDocument->ProcessChangeManager();
}

HBRUSH CCharacterInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr;

	switch (nCtlColor) 
	{ 
	case CTLCOLOR_EDIT:
		{
			UINT nId = pWnd->GetDlgCtrlID();
			if( nId >= IDC_CHAR_NAME && nId < IDC_CHAR_ABILITYPOINT+1 )
			{
				if( ((CMREdit*)pWnd)->HasSaveData() )
					pDC->SetTextColor( ((CMREdit*)pWnd)->GetEditTextColor() );
			}
			else
				hbr = CDialog::OnCtlColor(pDC,pWnd,nCtlColor); 
		}
		break;
	default:
			hbr = CDialog::OnCtlColor(pDC,pWnd,nCtlColor); 
		break;
	}

	return hbr;
}

void CCharacterInfo::MakeToolTip(BYTE type, POSTYPE pos, WORD idx, char* str)
{
	CString buff;
	BOOL bOption = FALSE;
	WORD OptionIdx = 0;

	if( idx == 0 ) return;

	switch(type)
	{
	case 0: // inven item
		{
			ITEM_INFO* pInfo = GAMERESRCMNGR->GetItemInfo(idx);
			if( !pInfo ) break;

			buff.Format("\n");
			strcat(str, buff.GetBuffer(0));

			buff.Format("Item : %s(%d)", pInfo->ItemName, pInfo->ItemIdx);
			strcat(str, buff.GetBuffer(0));

			if( GAMERESRCMNGR->IsOptionItem(pInfo->ItemIdx, m_pDocument->m_sInven.Inventory[pos+m_nCurInvenTab].Durability) )
			{
				bOption = TRUE;
				OptionIdx = m_pDocument->m_sInven.Inventory[pos+m_nCurInvenTab].Durability;

				buff.Format("\nOption Num : %d", m_pDocument->m_sInven.Inventory[pos+m_nCurInvenTab].Durability);
				strcat(str, buff.GetBuffer(0));
			}
			else
			{
				if( GAMERESRCMNGR->IsDupItem(pInfo->ItemIdx) )
				{
					buff.Format("\nCount : %d", m_pDocument->m_sInven.Inventory[pos+m_nCurInvenTab].Durability);
					strcat(str, buff.GetBuffer(0));
				}
			}
		}
		break;
	case 1: // wear item
		{
			ITEM_INFO* pInfo = GAMERESRCMNGR->GetItemInfo(idx);
			if( !pInfo ) break;

			buff.Format("\n");
			strcat(str, buff.GetBuffer(0));
			
			buff.Format("Item : %s(%d)", pInfo->ItemName, pInfo->ItemIdx);
			strcat(str, buff.GetBuffer(0));

			if( GAMERESRCMNGR->IsOptionItem(pInfo->ItemIdx, m_pDocument->m_sInven.WearedItem[pos].Durability) )
			{
				bOption = TRUE;
				OptionIdx = m_pDocument->m_sInven.WearedItem[pos].Durability;

				buff.Format("\nOption Num : %d", m_pDocument->m_sInven.WearedItem[pos].Durability);
				strcat(str, buff.GetBuffer(0));
			}
			else
			{
				if( GAMERESRCMNGR->IsDupItem(pInfo->ItemIdx) )
				{
					buff.Format("\nCount : %d", m_pDocument->m_sInven.WearedItem[pos].Durability);
					strcat(str, buff.GetBuffer(0));
				}
			}
		}
		break;
	case 2: // pyoguk item
		{
			ITEM_INFO* pInfo = GAMERESRCMNGR->GetItemInfo(idx);
			if( !pInfo ) break;

			buff.Format("\n");
			strcat(str, buff.GetBuffer(0));

			buff.Format("Item : %s(%d)", pInfo->ItemName, pInfo->ItemIdx);
			strcat(str, buff.GetBuffer(0));

			if( GAMERESRCMNGR->IsOptionItem(pInfo->ItemIdx, m_pDocument->m_sPyoguk.Pyokuk[pos].Durability) )
			{
				bOption = TRUE;
				OptionIdx = m_pDocument->m_sPyoguk.Pyokuk[pos].Durability;

				buff.Format("\nOption Num : %d", m_pDocument->m_sPyoguk.Pyokuk[pos].Durability);
				strcat(str, buff.GetBuffer(0));
			}
			else
			{
				if( GAMERESRCMNGR->IsDupItem(pInfo->ItemIdx) )
				{
					buff.Format("\nCount : %d", m_pDocument->m_sPyoguk.Pyokuk[pos].Durability);
					strcat(str, buff.GetBuffer(0));
				}
			}
		}
		break;
	case 3: // 무공
	case 4: // 진법
		{
			SKILLINFO* pSkill = GAMERESRCMNGR->GetSkillInfo(idx);
			if(!pSkill) break;
			buff.Format("\n");
			strcat(str, buff.GetBuffer(0));

			buff.Format("Skill : %s(%d)", pSkill->SkillName , pSkill->SkillIdx);
			strcat(str, buff.GetBuffer(0));
			buff.Format("\n Skill Level : %d", m_pDocument->m_sMugong.mugong[pos].Sung);
			strcat(str, buff.GetBuffer(0));
			buff.Format("\nSkill Exp : %d", m_pDocument->m_sMugong.mugong[pos].ExpPoint);
			strcat(str, buff.GetBuffer(0));
		}
		break;
	case 5: // ability
		{
			CAbilityInfo* pAbility = GAMERESRCMNGR->GetAbilityInfo(idx);
			if(!pAbility) break;

			buff.Format("\n");
			strcat(str, buff.GetBuffer(0));

			buff.Format("Ability : %s(%d)", pAbility->GetInfo()->Ability_name , pAbility->GetInfo()->Ability_index);
			strcat(str, buff.GetBuffer(0));

		//	buff.Format("\nAbility Level : %d", m_pDocument->GetCharacterAbillityLevel(pAbility->GetKind(), pAbility->GetInfo()->Ability_index));
		//	strcat(str, buff.GetBuffer(0));
		}
		break;
	default:
		return;
	}

	if( bOption )
	{
		ITEM_OPTION_INFO* pOptionInfo = m_pDocument->GetItemOptionInfo(OptionIdx);
		if(pOptionInfo)
		{
			char OptionBuff[40] = {0,};

			if( pOptionInfo->GenGol != 0 )
			{
				sprintf(OptionBuff, "\n%s : +%d", "Physique", pOptionInfo->GenGol);
				strcat(str, OptionBuff);
			}

			if( pOptionInfo->MinChub != 0 )
			{
				sprintf(OptionBuff, "\n%s : +%d", "Agility", pOptionInfo->MinChub);
				strcat(str, OptionBuff);
			}

			if( pOptionInfo->CheRyuk != 0 )
			{
				sprintf(OptionBuff, "\n%s : +%d", "Strength", pOptionInfo->CheRyuk);
				strcat(str, OptionBuff);
			}

			if( pOptionInfo->SimMek != 0 )
			{
				sprintf(OptionBuff, "\n%s : +%d", "Heart", pOptionInfo->SimMek);
				strcat(str, OptionBuff);
			}

			if( pOptionInfo->Life != 0 )
			{
				sprintf(OptionBuff, "\n%s : +%d", "Life", pOptionInfo->Life);
				strcat(str, OptionBuff);
			}

			if( pOptionInfo->NaeRyuk != 0 )
			{
				sprintf(OptionBuff, "\n%s : +%d", "Mana", pOptionInfo->NaeRyuk);
				strcat(str, OptionBuff);
			}

			if( pOptionInfo->Shield != 0 )
			{
				sprintf(OptionBuff, "\n%s : +%d", "Shield", pOptionInfo->Shield);
				strcat(str, OptionBuff);
			}

			if( pOptionInfo->PhyAttack != 0 )
			{
				sprintf(OptionBuff, "\n%s : +%d", "Attack", pOptionInfo->PhyAttack);
				strcat(str, OptionBuff);
			}

			if( pOptionInfo->CriticalPercent != 0 )
			{
				sprintf(OptionBuff, "\n%s : +%d", "Critical", pOptionInfo->CriticalPercent);
				strcat(str, OptionBuff);
			}

			if( pOptionInfo->PhyDefense != 0 )
			{
				sprintf(OptionBuff, "\n%s : +%d", "Defense", pOptionInfo->PhyDefense);
				strcat(str, OptionBuff);
			}

			if(pOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE) != 0.00f )
			{
				sprintf(OptionBuff, "\n%s : %.2f%%", "Fire Resist", pOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE));
//				sprintf(OptionBuff, "\n%s : %d%%", "Fire Resist", (int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE)*100));
				strcat(str, OptionBuff);
			}

			if(pOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER) != 0.00f )
			{

				sprintf(OptionBuff, "\n%s : %.2f%%", "Water Resist", pOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER));
//				sprintf(OptionBuff, "\n%s : %d%%", "Water Resist", (int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER)*100));
				strcat(str, OptionBuff);
			}

			if(pOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE) != 0.00f )
			{
				sprintf(OptionBuff, "\n%s : %.2f%%", "Tree Resist", pOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE));
//				sprintf(OptionBuff, "\n%s : %d%%", "Tree Resist", (int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE)*100));
				strcat(str, OptionBuff);
			}

			if(pOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON) != 0.00f )
			{
				sprintf(OptionBuff, "\n%s : %.2f%%", "Iron Resist", pOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON));
//				sprintf(OptionBuff, "\n%s : %d%%", "Iron Resist", (int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON)*100));
				strcat(str, OptionBuff);
			}

			if(pOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH) != 0.00f )
			{
				sprintf(OptionBuff, "\n%s : %.2f%%", "Earth Resist", pOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH));
//				sprintf(OptionBuff, "\n%s : %d%%", "Earth Resist", (int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH)*100));
				strcat(str, OptionBuff);
			}

			if(pOptionInfo->AttrRegist.GetElement_Val(ATTR_NOATTR) != 0.00f )
			{
				sprintf(OptionBuff, "\n%s : %.2f%%", "None", pOptionInfo->AttrRegist.GetElement_Val(ATTR_NOATTR));
//				sprintf(OptionBuff, "\n%s : %d%%", "None", (int)(pOptionInfo->AttrRegist.GetElement_Val(ATTR_NOATTR)*100));
				strcat(str, OptionBuff);
			}

			if(pOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE) != 0.00f )
			{
				sprintf(OptionBuff, "\n%s : %.2f%%", "Fire Attack", pOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE));
//				sprintf(OptionBuff, "\n%s : %d%%", "Fire Attack", (int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE)*100));
				strcat(str, OptionBuff);
			}

			if(pOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER) != 0.00f )
			{
				sprintf(OptionBuff, "\n%s : %.2f%%", "Water Attack", pOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER));
//				sprintf(OptionBuff, "\n%s : %d%%", "Water Attack", (int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER)*100));
				strcat(str, OptionBuff);
			}

			if(pOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE) != 0.00f )
			{
				sprintf(OptionBuff, "\n%s : %.2f%%", "Tree Attack", pOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE));
//				sprintf(OptionBuff, "\n%s : %d%%", "Tree Attack", (int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE)*100));
				strcat(str, OptionBuff);
			}

			if(pOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON) != 0.00f )
			{
				sprintf(OptionBuff, "\n%s : %.2f%%", "Iron Attack", pOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON));
//				sprintf(OptionBuff, "\n%s : %d%%", "Iron Attack", (int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON)*100));
				strcat(str, OptionBuff);
			}

			if(pOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH) != 0.00f )
			{
				sprintf(OptionBuff, "\n%s : %.2f%%", "Earth Attack", pOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH));
//				sprintf(OptionBuff, "\n%s : %d%%", "Earth Attack", (int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH)*100));
				strcat(str, OptionBuff);
			}

			if(pOptionInfo->AttrAttack.GetElement_Val(ATTR_NOATTR) != 0.00f )
			{
				sprintf(OptionBuff, "\n%s : %.2f%%", "None Attack", pOptionInfo->AttrAttack.GetElement_Val(ATTR_NOATTR));
//				sprintf(OptionBuff, "\n%s : %d%%", "None Attack", (int)(pOptionInfo->AttrAttack.GetElement_Val(ATTR_NOATTR)*100));
				strcat(str, OptionBuff);
			}
		}
	}

	buff.Format("\n");
	strcat(str, buff.GetBuffer(0));
}

void CCharacterInfo::ResetTooltip()
{
	m_TTctrl.Activate(FALSE);
}

//
void CCharacterInfo::OnQuestinfo() 
{
	m_QuestInfoDlg.SetInitData( m_pDocument->GetDocumentIdx(), m_pDocument->m_sBaseInfo.dwObjectID, m_pDocument->m_sBaseInfo.ObjectName );
	m_QuestInfoDlg.DoModal();
}

void CCharacterInfo::UpdateMainQuestData( TMSG_MAINQUESTINFO* pMsg )
{
	if( m_QuestInfoDlg.m_bInit )
		m_QuestInfoDlg.UpdateQuestListCtrl( pMsg );
}

void CCharacterInfo::UpdateSubQuestData( TMSG_SUBQUESTINFO* pMsg )
{
	if( m_QuestInfoDlg.m_bInit )
		m_QuestInfoDlg.UpdateSubQuestData( pMsg );
}

void CCharacterInfo::OnButtonChangeLoginpoint() 
{
	int index = m_cbLoginPoint.GetCurSel();
	if( index > -1 )
	{
		if( MessageBox( "Are You Really Change User LoginPoint?", "Confirm", MB_OKCANCEL ) == IDOK )
		{
			char mapname[128] = {0,};
			m_cbLoginPoint.GetLBText( index, mapname );

			DWORD mapnum = GAMERESRCMNGR->GetMapNum( mapname );
			if( mapnum > 0 )
				m_pDocument->ChangeLoginPoint( mapnum );
		}
	}	
}

void CCharacterInfo::ChangeLoginPoint( DWORD dwMapNum )
{
	if( dwMapNum > 0 )
	{
		CString str;
		str.Format( "%s", GAMERESRCMNGR->GetMapName( dwMapNum ) );
		m_StatusEdit[eCharTemp_LoginPoint].SetWindowTextEx( (char*)(LPCTSTR)str, 0 );
	}
}

BOOL CCharacterInfo::CheckItemPosition( DWORD dwKind, DWORD Pos )
{
	switch( dwKind )
	{
	case 0:		// inven
		{
			if( m_InvenItem[Pos].GetData() != 0 )
				return FALSE;
			else
				return TRUE;
		}
		break;
	}

	return FALSE;
}
