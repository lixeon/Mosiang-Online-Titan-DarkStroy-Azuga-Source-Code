// MunpaInfo.cpp : implementation file
//
#include "stdafx.h"
#include "DS_RMTool.h"
#include "MunpaInfo.h"
#include "MunpaDoc.h"

#include "RMNetwork.h"
#include "ChangeValue.h"
//#include "GameResourceManager.h"
#include "ItemDlg.h"
#include "MasInfoManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMunpaInfo property page

IMPLEMENT_DYNCREATE(CMunpaInfo, CLDPropPage)

CMunpaInfo::CMunpaInfo() : CLDPropPage(CMunpaInfo::IDD)
{
	//{{AFX_DATA_INIT(CMunpaInfo)
	//}}AFX_DATA_INIT

	m_pDocument = NULL;
	m_pItemDlg = NULL;
	m_wPower = eOperPower_Max;
}

CMunpaInfo::~CMunpaInfo()
{
	SAFE_DELETE(m_pItemDlg);
}

void CMunpaInfo::DoDataExchange(CDataExchange* pDX)
{
	CLDPropPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMunpaInfo)
	DDX_Control(pDX, IDC_MUNPA_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_SEARCH_MUNPA, m_etMunpaSearch);
	DDX_Control(pDX, IDC_MUNPA_CBTAB, m_cbTab);
	DDX_Control(pDX, IDC_MUNPA_NAMELIST, m_MunpaWonList);
	//}}AFX_DATA_MAP

	int i = 0;
	//////////////////////////////////////////////////////////////////////
	//// 정보창 관련 변수
	for(i=0;i< eMunpaTemp_MAX;++i)
		DDX_Control(pDX, IDC_MUNPA_NAME+i, m_StatusEdit[i]);
	//////////////////////////////////////////////////////////////////////	

	//////////////////////////////////////////////////////////////////////
	//// 문파 창고
	for(i = 0;i<TABCELL_MUNPAWAREHOUSE_NUM;++i)
		DDX_Control(pDX, IDC_BTN_MUNPA1+i, m_MunpaItem[i]);
	//////////////////////////////////////////////////////////////////////
}

BEGIN_MESSAGE_MAP(CMunpaInfo, CLDPropPage)
	//{{AFX_MSG_MAP(CMunpaInfo)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_CBN_SELCHANGE(IDC_MUNPA_CBTAB, OnSelchangeMunpaCbtab)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_MUNPA_SAVE, OnMunpaSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMunpaInfo message handlers

BOOL CMunpaInfo::OnInitDialog() 
{
	CLDPropPage::OnInitDialog();

	CDS_RMToolApp* pApp = (CDS_RMToolApp*)AfxGetApp();
	m_wPower = pApp->GetCurOperatorPower();

	////////////////////////////////////////////
	// Control 초기화
	InitMemberListControl();
	////////////////////////////////////////////	
	
	m_TTctrl.Create(this);
	m_TTctrl.SetMaxTipWidth(200);

	int i;
	for(i=0;i<TABCELL_MUNPAWAREHOUSE_NUM;++i)
		m_TTctrl.AddTool(&m_MunpaItem[i]);

	EnableToolTips(TRUE);

	EnableWareHouse(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMunpaInfo::InitMunpaInfo( BOOL bEnable )
{
	for(int i=0;i<eMunpaTemp_MAX;++i)
	{
		m_StatusEdit[i].SetWindowText("");
		m_StatusEdit[i].ResetEditTextColor();
	}

	for(i = 0;i<TABCELL_MUNPAWAREHOUSE_NUM;++i)
	{
		m_MunpaItem[i].SetWindowText("");
		m_MunpaItem[i].SetData(0);
		m_MunpaItem[i].ReSetColorText();
	}

	if( !bEnable )
		m_MunpaWonList.DeleteAllItems();

	m_etMunpaSearch.SetWindowText("");
	EnableWareHouse(bEnable);
}

void CMunpaInfo::InitMemberListControl()
{
	LV_COLUMN lvcolumn;

	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_CENTER;

//	캐릭명, 레벨, 직책, 등록 날짜

	for(int i = 1 ; i <= 4 ; ++i)
	{
		lvcolumn.iSubItem = i;
		switch(i)
		{
		case 1:
			{
				lvcolumn.cx = 80;
				lvcolumn.pszText = "ID";
			}
			break;
		case 2:
			{
				lvcolumn.cx = 50;
				lvcolumn.pszText = "Level";
			}
			break;
		case 3:
			{
				lvcolumn.cx = 80;
				lvcolumn.pszText = "Position";
			}
			break;
		case 4:
			{
				lvcolumn.cx = 140;
				lvcolumn.pszText = "Date";
			}
			break;
		}
		m_MunpaWonList.InsertColumn(i, &lvcolumn);
	}


	DWORD dwExStyle = m_MunpaWonList.GetExtendedStyle();

	dwExStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_MunpaWonList.SetExtendedStyle( dwExStyle );		
}

void CMunpaInfo::InsertMemberList(char* strName, LEVELTYPE level, BYTE position, char* strRegDate)
{
	LV_ITEM	lvitem;

	lvitem.iItem = m_MunpaWonList.GetItemCount();
	lvitem.mask = LVIF_TEXT | LVIF_STATE;
	lvitem.iSubItem = 0;
	lvitem.stateMask = LVIS_SELECTED;
	lvitem.state = INDEXTOSTATEIMAGEMASK(1);
	char buff[256] = {0,};


#define MUNPA_MEMBER		2	//문파원
#define MUNPA_SENIOR		3	//장로
#define MUNPA_VICE_MASTER	10	//부문주

	for(int i=0;i<4;++i)
	{
		switch(i)
		{
		case 0: // 아이디
			{
				// 리소스에서 데이터를 가져와서 이름을 넣어준다.
				lvitem.pszText = strName;
			}
			break;
		case 1:	// 레벨
			{
				sprintf(buff, "%d", level);
				lvitem.pszText = buff;
			}
			break;
		case 2:	// 직책..
			{
				switch(position)
				{
				case MUNPA_MASTER: sprintf(buff, "%s", "Master"); break;
				case MUNPA_VICE_MASTER: sprintf(buff, "%s", "Sub Master"); break;
				case MUNPA_SENIOR: sprintf(buff, "%s", "Senior"); break;
				case MUNPA_MEMBER: sprintf(buff, "%s", "Member"); break;
				case MUNPA_SEMIMEMBER: sprintf(buff, "%s", "Volunteer"); break;
				default: sprintf(buff, "%d", position); break;
				}
				lvitem.pszText = buff;
			}
			break;
		case 3: // 등록 날짜
			{
				lvitem.pszText = strRegDate;
			}
			break;
		}
		if( i == 0 )
			m_MunpaWonList.InsertItem((&lvitem));
		else
			m_MunpaWonList.SetItemText(lvitem.iItem, i, lvitem.pszText);
	}	

}

void CMunpaInfo::OnButtonSearch() 
{
	// TODO: Add your control notification handler code here
	CString str;
	int nSearchType = -1;

	if( m_etMunpaSearch.GetWindowTextLength() != 0 )
	{
		nSearchType = 0;
		m_etMunpaSearch.GetWindowText(str);
	}

	if( nSearchType == -1 )
	{
		// 검색어 입력 에러 메세지
		MessageBox("Input Name!!", "Error", MB_OK);
		return;
	}

	// 서버에 검색 요청

	TMSG_MUNPASEARCH MunpaSearch;
	MunpaSearch.Category = MP_RMTOOL_MUNPA;
	MunpaSearch.Protocol = MP_RMTOOL_MUNPASEARCH_SYN;
	MunpaSearch.dwTemplateIdx = m_pDocument->GetDocumentIdx();
//	MunpaSearch.wServer = (WORD)m_cbServer.GetItemData(m_cbServer.GetCurSel());
	MunpaSearch.wServer = MASINFOMGR->GetCurMasInfo()->nServerSet;
	strcpy(MunpaSearch.strSearch, str.GetBuffer(0));

	NETWORK->Send(&MunpaSearch, sizeof(MunpaSearch));

	InitMunpaInfo();
}

BOOL CMunpaInfo::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( nID == (UINT)m_etMunpaSearch.GetDlgCtrlID() && nCode == 1281 )
	{
		if( &m_etMunpaSearch == GetFocus() )
			OnButtonSearch();
	}
	else
	{
		if( m_pDocument )
		if( m_pDocument->IsReadOnly() )
		{
			m_btnSave.EnableWindow( FALSE );
		}
		else
		{
			if( m_wPower == eOperPower_Master )
				m_btnSave.EnableWindow( TRUE );
			else
				m_btnSave.EnableWindow( FALSE );
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
				if( (nID >= IDC_MUNPA_NAME && nID < IDC_MUNPA_LEVEL+1) )
				{
					CMREdit* pEdit = (CMREdit*)GetDlgItem(nID);
					if( pEdit) pEdit->SetChanged(TRUE);
				}
			}
			break;
		default:
			break;
		}		
	}

	return CLDPropPage::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMunpaInfo::SetMunpaBaseInfo()
{
	CString str;

	m_StatusEdit[eMunpaTemp_NAME].SetWindowTextEx(m_pDocument->m_pBaseInfo->strMunpaName);

	m_StatusEdit[eMunpaTemp_MUNJU].SetWindowTextEx(m_pDocument->m_pBaseInfo->strMunjuName);

	m_StatusEdit[eMunpaTemp_BIRTH].SetWindowTextEx(m_pDocument->m_pBaseInfo->strRegDate);

	str.Format("%d", m_pDocument->m_pBaseInfo->wMemberNum);
	m_StatusEdit[eMunpaTemp_COUNT].SetWindowTextEx(str.GetBuffer(0));

	str.Format("%d", m_pDocument->m_pBaseInfo->dwFamous);
	m_StatusEdit[eMunpaTemp_FAME].SetWindowTextEx(str.GetBuffer(0));

	str.Format("%d", m_pDocument->m_pBaseInfo->dwMoney);
	m_StatusEdit[eMunpaTemp_MONEY].SetMaxValue(3000000000);
	m_StatusEdit[eMunpaTemp_MONEY].SetWindowTextEx(str.GetBuffer(0), 1);

	str.Format("%d", m_pDocument->m_pBaseInfo->wLocation);
	m_StatusEdit[eMunpaTemp_LOCATION].SetWindowTextEx(str.GetBuffer(0));

	str.Format("%d", m_pDocument->m_pBaseInfo->bKind);
	m_StatusEdit[eMunpaTemp_KIND].SetWindowTextEx(str.GetBuffer(0));

	str.Format("%d", m_pDocument->m_pBaseInfo->bMunpaLevel);
	m_StatusEdit[eMunpaTemp_LEVEL].SetWindowTextEx(str.GetBuffer(0));
}

void CMunpaInfo::SetMunpaItemInfo()
{
	char buff[256] = {0,};
	int i;
	ITEM_INFO* pInfo;

	EnableWareHouse(TRUE);

	// Combo Init
	for(i=0;i<TAB_MUNPAWAREHOUSE_NUM;++i)
	{
		sprintf(buff,"Tab%d", i+1);
		m_cbTab.InsertString(m_cbTab.GetCount(), buff);
	}
	m_cbTab.SetCurSel(0);

	// Item Setting
	for(i=0;i<TABCELL_MUNPAWAREHOUSE_NUM;++i)
	{
		if( m_pDocument->m_pWareHouse->WareHouse[i].wIconIdx != 0)
		{
			pInfo = GAMERESRCMNGR->GetItemInfo(m_pDocument->m_pWareHouse->WareHouse[i].wIconIdx);
			if( pInfo )
				sprintf(buff, pInfo->ItemName);
			else
				sprintf(buff, "%d", m_pDocument->m_pWareHouse->WareHouse[i].wIconIdx);
		}
		else
			sprintf(buff, "");

		m_MunpaItem[i].SetWindowText(buff);
		m_MunpaItem[i].SetData(m_pDocument->m_pWareHouse->WareHouse[i].wIconIdx);
	}
}

CMRButton* CMunpaInfo::CheckButtonEvent(HWND hWnd, POSTYPE& pos)
{
	for(int i=0;i<TABCELL_MUNPAWAREHOUSE_NUM;++i)
	{
		if(m_MunpaItem[i].m_hWnd == hWnd)
		{
			pos = i;
			return &m_MunpaItem[i];
		}
	}

	return NULL;
}

void CMunpaInfo::MakeToolTip(POSTYPE pos, WORD idx, char* str)
{
	CString buff;
	BOOL bOption = FALSE;
	WORD OptionIdx = 0;

	if( idx == 0 ) return;

	ITEM_INFO* pInfo = GAMERESRCMNGR->GetItemInfo(idx);
	if( !pInfo ) return;
	
	buff.Format("\n");
	strcat(str, buff.GetBuffer(0));
	
	buff.Format("아이템 : %s(%d)", pInfo->ItemName, pInfo->ItemIdx);
	strcat(str, buff.GetBuffer(0));

	if( GAMERESRCMNGR->IsOptionItem(pInfo->ItemIdx, m_pDocument->m_pWareHouse->WareHouse[pos].Durability) )
	{
		bOption = TRUE;
		OptionIdx = m_pDocument->m_pWareHouse->WareHouse[pos].Durability;
		
		buff.Format("\n옵션번호 : %d", m_pDocument->m_pWareHouse->WareHouse[pos].Durability);
		strcat(str, buff.GetBuffer(0));
	}
	else
	{
		if( GAMERESRCMNGR->IsDupItem(pInfo->ItemIdx) )
		{
			buff.Format("\n개   수 : %d개", m_pDocument->m_pWareHouse->WareHouse[pos].Durability);
			strcat(str, buff.GetBuffer(0));
		}
	}
	
	if( bOption )
	{
		ITEM_OPTION_INFO* pOptionInfo = m_pDocument->GetItemOptionInfo(OptionIdx);
		if(pOptionInfo)
		{
			char OptionBuff[40] = {0,};

			if( pOptionInfo->GenGol != 0 )
			{
				sprintf(OptionBuff, "\n%s : +%d", "근골", pOptionInfo->GenGol);
				strcat(str, OptionBuff);
			}

			if( pOptionInfo->MinChub != 0 )
			{
				sprintf(OptionBuff, "\n%s : +%d", "민첩", pOptionInfo->MinChub);
				strcat(str, OptionBuff);
			}

			if( pOptionInfo->CheRyuk != 0 )
			{
				sprintf(OptionBuff, "\n%s : +%d", "체력", pOptionInfo->CheRyuk);
				strcat(str, OptionBuff);
			}

			if( pOptionInfo->SimMek != 0 )
			{
				sprintf(OptionBuff, "\n%s : +%d", "심맥", pOptionInfo->SimMek);
				strcat(str, OptionBuff);
			}

			if( pOptionInfo->Life != 0 )
			{
				sprintf(OptionBuff, "\n%s : +%d", "생명력", pOptionInfo->Life);
				strcat(str, OptionBuff);
			}

			if( pOptionInfo->NaeRyuk != 0 )
			{
				sprintf(OptionBuff, "\n%s : +%d", "내력", pOptionInfo->NaeRyuk);
				strcat(str, OptionBuff);
			}

			if( pOptionInfo->Shield != 0 )
			{
				sprintf(OptionBuff, "\n%s : +%d", "방어력", pOptionInfo->Shield);
				strcat(str, OptionBuff);
			}

			if( pOptionInfo->PhyAttack != 0 )
			{
				sprintf(OptionBuff, "\n%s : +%d", "물리공격", pOptionInfo->PhyAttack);
				strcat(str, OptionBuff);
			}

			if( pOptionInfo->CriticalPercent != 0.00f )
			{
				sprintf(OptionBuff, "\n%s : %.2f%%", "크리티컬", pOptionInfo->CriticalPercent);
				strcat(str, OptionBuff);
			}

			if( pOptionInfo->PhyDefense != 0 )
			{
				sprintf(OptionBuff, "\n%s : +%d", "물리방어", pOptionInfo->PhyDefense);
				strcat(str, OptionBuff);
			}

			if(pOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE) != 0.00f )
			{
				sprintf(OptionBuff, "\n%s : %.2f%%", "화방어", pOptionInfo->AttrRegist.GetElement_Val(ATTR_FIRE));
				strcat(str, OptionBuff);
			}

			if(pOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER) != 0.00f )
			{
				sprintf(OptionBuff, "\n%s : %.2f%%", "수방어", pOptionInfo->AttrRegist.GetElement_Val(ATTR_WATER));
				strcat(str, OptionBuff);
			}

			if(pOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE) != 0.00f )
			{
				sprintf(OptionBuff, "\n%s : %.2f%%", "목방어", pOptionInfo->AttrRegist.GetElement_Val(ATTR_TREE));
				strcat(str, OptionBuff);
			}

			if(pOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON) != 0.00f )
			{
				sprintf(OptionBuff, "\n%s : %.2f%%", "금방어", pOptionInfo->AttrRegist.GetElement_Val(ATTR_IRON));
				strcat(str, OptionBuff);
			}

			if(pOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH) != 0.00f )
			{
				sprintf(OptionBuff, "\n%s : %.2f%%", "토방어", pOptionInfo->AttrRegist.GetElement_Val(ATTR_EARTH));
				strcat(str, OptionBuff);
			}

			if(pOptionInfo->AttrRegist.GetElement_Val(ATTR_NOATTR) != 0.00f )
			{
				sprintf(OptionBuff, "\n%s : %.2f%%", "무방어", pOptionInfo->AttrRegist.GetElement_Val(ATTR_NOATTR));
				strcat(str, OptionBuff);
			}

			if(pOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE) != 0.00f )
			{
				sprintf(OptionBuff, "\n%s : %.2f%%", "화공격", pOptionInfo->AttrAttack.GetElement_Val(ATTR_FIRE));
				strcat(str, OptionBuff);
			}

			if(pOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER) != 0.00f )
			{
				sprintf(OptionBuff, "\n%s : %.2f%%", "수공격", pOptionInfo->AttrAttack.GetElement_Val(ATTR_WATER));
				strcat(str, OptionBuff);
			}

			if(pOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE) != 0.00f )
			{
				sprintf(OptionBuff, "\n%s : %.2f%%", "목공격", pOptionInfo->AttrAttack.GetElement_Val(ATTR_TREE));
				strcat(str, OptionBuff);
			}

			if(pOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON) != 0.00f )
			{
				sprintf(OptionBuff, "\n%s : %.2f%%", "금공격", pOptionInfo->AttrAttack.GetElement_Val(ATTR_IRON));
				strcat(str, OptionBuff);
			}

			if(pOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH) != 0.00f )
			{
				sprintf(OptionBuff, "\n%s : %.2f%%", "토공격", pOptionInfo->AttrAttack.GetElement_Val(ATTR_EARTH));
				strcat(str, OptionBuff);
			}

			if(pOptionInfo->AttrAttack.GetElement_Val(ATTR_NOATTR) != 0.00f )
			{
				sprintf(OptionBuff, "\n%s : %.2f%%", "무공격", pOptionInfo->AttrAttack.GetElement_Val(ATTR_NOATTR));
				strcat(str, OptionBuff);
			}
		}
	}

	buff.Format("\n");
	strcat(str, buff.GetBuffer(0));
}

BOOL CMunpaInfo::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	POSTYPE pos;
	if( CMRButton* wnd = CheckButtonEvent(pMsg->hwnd, pos) )
	{
		char buff[1024] = {0,};
		MakeToolTip(pos, (WORD)wnd->GetData(), buff);

		if( strlen(buff) )
		{
			m_TTctrl.UpdateTipText( buff , wnd );
			m_TTctrl.RelayEvent(pMsg);
		}
	}
	
	return CLDPropPage::PreTranslateMessage(pMsg);
}

void CMunpaInfo::EnableWareHouse(BOOL bEnable)
{
	m_StatusEdit[eMunpaTemp_MONEY].EnableWindow(bEnable);

	for(int i=0;i<TABCELL_MUNPAWAREHOUSE_NUM;++i)
		m_MunpaItem[i].EnableWindow(bEnable);

	m_cbTab.EnableWindow(bEnable);
	m_btnSave.EnableWindow(bEnable);
}

void CMunpaInfo::OnSelchangeMunpaCbtab() 
{
	// TODO: Add your control notification handler code here
	char buff[256] = {0,};
	ITEM_INFO* pInfo;

	int nCurSel = m_cbTab.GetCurSel();
	int tab = nCurSel*TABCELL_MUNPAWAREHOUSE_NUM;

	// Item Setting
	for(int i=0;i<TABCELL_MUNPAWAREHOUSE_NUM;++i)
	{
		if( m_pDocument->m_pWareHouse->WareHouse[tab+i].wIconIdx != 0)
		{
			pInfo = GAMERESRCMNGR->GetItemInfo(m_pDocument->m_pWareHouse->WareHouse[tab+i].wIconIdx);
			if( pInfo )
				sprintf(buff, pInfo->ItemName);
			else
				sprintf(buff, "%d", m_pDocument->m_pWareHouse->WareHouse[tab+i].wIconIdx);
		}
		else
			sprintf(buff, "");

		m_MunpaItem[i].SetWindowText(buff);
		m_MunpaItem[i].SetData(m_pDocument->m_pWareHouse->WareHouse[tab+i].wIconIdx);
	}
}

TABLE_KIND	CMunpaInfo::GetTableKindByControlID(UINT nId)
{
	TABLE_KIND kind = eTK_None;

	if( nId >= IDC_MUNPA_NAME &&	nId < IDC_MUNPA_LEVEL+1 )
			kind = eTK_MunpaInfo;

	return kind;
}

void CMunpaInfo::ButtonControlCommand(UINT nId)
{
	CMRButton* pBtn = (CMRButton*)GetDlgItem( nId );
	if( !pBtn )
		return;

	WORD wPos;
	int tab, result;

	if( nId >= IDC_BTN_MUNPA1 && nId < IDC_BTN_MUNPA45+1 )
	{ // 문파 창고
		tab = m_cbTab.GetCurSel()*TABCELL_MUNPAWAREHOUSE_NUM;

		wPos = tab + nId - IDC_BTN_MUNPA1;
		ITEMBASE* origin = &m_pDocument->m_sWareHouse_org.WareHouse[wPos];
		ITEMBASE* base_before = &m_pDocument->m_pWareHouse->WareHouse[wPos];
		ITEMBASE base_after;

		ITEM_OPTION_INFO* pOptionInfoOrigin = NULL;
		ITEM_OPTION_INFO* pOptionInfo = NULL;
		ITEM_OPTION_INFO Option_after;
		if( GAMERESRCMNGR->IsOptionItem(origin->wIconIdx, origin->Durability) )
			pOptionInfoOrigin = m_pDocument->GetItemOptionInfoOrigin(origin->Durability);
		if( GAMERESRCMNGR->IsOptionItem(base_before->wIconIdx, base_before->Durability) )
			pOptionInfo = m_pDocument->GetItemOptionInfo(base_before->Durability);

		result = ItemEdit( eIF_MUNPA, wPos, base_before, base_after, pOptionInfo, Option_after );

		if( result == eMR_Cancel || result == eMR_Abort ) return;
		if( result == eMR_Delete )
		{
			if( MessageBox("정말로 삭제하시겠습니까?",NULL, MB_YESNO) == IDNO )
				return;
		}
		
		base_after.Position = TP_MUNPAWAREHOUSE_START + wPos;
		ResultItemEdit( eIF_MUNPA, result, TP_MUNPAWAREHOUSE_START + wPos, nId, origin, pOptionInfoOrigin, base_before, &base_after, pOptionInfo, &Option_after );
	}
	else
	{ // 처리 안함.
	}
}

void CMunpaInfo::EditControlCommand(UINT nId)
{
	WORD wPos;
	CChangeData* pData = NULL;
	CString str1, str2;
	DWORD before_data, after_data;

	if( nId >= IDC_MUNPA_NAME && nId < IDC_MUNPA_LEVEL+1 )
	{
		wPos = (WORD)(nId - IDC_MUNPA_NAME);	// Tool 데이터의 배열인덱스

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

			m_StatusEdit[wPos].SetWindowText(str1);
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


HBRUSH CMunpaInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr;

	switch (nCtlColor) 
	{ 
	case CTLCOLOR_EDIT:
		{
			UINT nId = pWnd->GetDlgCtrlID();
			if( nId >= IDC_MUNPA_NAME && nId < IDC_MUNPA_LEVEL+1 )
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

void CMunpaInfo::OnMunpaSave() 
{
	// TODO: Add your control notification handler code here
	m_pDocument->ProcessChangeManager();	
}

int CMunpaInfo::ItemEdit( INFOFIELD field, WORD wControlId, const ITEMBASE* base_before, ITEMBASE& base_after, 
					const ITEM_OPTION_INFO* before, ITEM_OPTION_INFO& after )
{
	int result = 0;

	m_pItemDlg = new CItemDlg;

	m_pItemDlg->Init( field, base_before, before, wControlId );
	result = m_pItemDlg->DoModal();
				
	memcpy(&base_after, m_pItemDlg->GetItemBase(), sizeof(ITEMBASE) );
	memcpy(&after, m_pItemDlg->GetItemOption(), sizeof(ITEM_OPTION_INFO) );

	SAFE_DELETE(m_pItemDlg);

	return result;
}

BOOL CMunpaInfo::ResultItemEdit( INFOFIELD field, int result, WORD wPos, int nControlIdx, ITEMBASE* Origin, ITEM_OPTION_INFO* OriginOption,
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
