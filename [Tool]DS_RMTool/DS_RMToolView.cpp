// DS_RMToolView.cpp : implementation of the CDS_RMToolView class
//

#include "stdafx.h"
#include "DS_RMTool.h"

#include "DS_RMToolDoc.h"
#include "CharacterDoc.h"
#include "DS_RMToolView.h"
#include "RMNetwork.h"
#include "MasInfoManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDS_RMToolView

IMPLEMENT_DYNCREATE(CDS_RMToolView, CFormView)

BEGIN_MESSAGE_MAP(CDS_RMToolView, CFormView)
	//{{AFX_MSG_MAP(CDS_RMToolView)
	ON_BN_CLICKED(IDC_USERBTN_SEARCH, OnUserbtnSearch)
	ON_BN_CLICKED(IDC_RADIO_USERID, OnRadioUserID)
	ON_BN_CLICKED(IDC_RADIO_CHARNAME, OnRadioCharName)
	ON_BN_CLICKED(IDC_RADIO_JUMIN, OnRadioJumin)	
	ON_BN_CLICKED(IDC_BTN_CHECKNAME, OnBtnCheckname)
	ON_BN_CLICKED(IDC_BTN_RECOVER, OnBtnRecover)
	ON_BN_CLICKED(IDC_BUTTON_CHANGE_USERLEVEL, OnButtonChangeUserlevel)
	ON_NOTIFY(NM_CLICK, IDC_LIST_CHAR, OnClickListChar)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CHAR, OnDblclkListChar)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CHARINFO, OnDblclkListCharInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDS_RMToolView construction/destruction

CDS_RMToolView::CDS_RMToolView()
	: CFormView(CDS_RMToolView::IDD)
{
	//{{AFX_DATA_INIT(CDS_RMToolView)
	m_nSearchKind = -1;
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	m_wPower = eOperPower_Max;
}

CDS_RMToolView::~CDS_RMToolView()
{
}

void CDS_RMToolView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDS_RMToolView)
	DDX_Control(pDX, IDC_BUTTON_CHANGE_USERLEVEL, m_btnChangeUserlevel);
	DDX_Control(pDX, IDC_LIST_CHARINFO, m_lcCharInfo);
	DDX_Control(pDX, IDC_LIST_CHAR, m_listCharacter);
	DDX_Control(pDX, IDC_COMBO_USERLEVEL, m_cbUserLevel);
	DDX_Control(pDX, IDC_EDIT_RECOVER, m_etRecover);
	DDX_Control(pDX, IDC_EDIT_DELETE, m_etDelete);
	DDX_Control(pDX, IDC_EDIT_SEARCH, m_etSearch);
	DDX_Control(pDX, IDC_USER_STATUS, m_stStatus);
	DDX_Radio(pDX, IDC_RADIO_USERID, m_nSearchKind);
	DDX_Control(pDX, IDC_BTN_CHECKNAME, m_btnCheckName);
	DDX_Control(pDX, IDC_BTN_RECOVER, m_btnRecover);
	//}}AFX_DATA_MAP

	// 정보창 관련 변수
	for( int i = 0; i < eUserTemp_MAX; ++i )
		DDX_Control(pDX, IDC_USER_ID+i, m_StatusEdit[i]);	
}

BOOL CDS_RMToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CFormView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDS_RMToolView diagnostics

#ifdef _DEBUG
void CDS_RMToolView::AssertValid() const
{
	CFormView::AssertValid();
}

void CDS_RMToolView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CDS_RMToolDoc* CDS_RMToolView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDS_RMToolDoc)));
	return (CDS_RMToolDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDS_RMToolView message handlers
BOOL CDS_RMToolView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( nID == (UINT)m_etSearch.GetDlgCtrlID() && nCode == 1281 )
	{
		if( &m_etSearch == GetFocus() )
			OnUserbtnSearch();
	}

	return CFormView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CDS_RMToolView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();

	CDS_RMToolApp* pApp = (CDS_RMToolApp*)AfxGetApp();
	m_wPower = pApp->GetCurOperatorPower();
	if( m_wPower == eOperPower_Master )
	{
		m_btnChangeUserlevel.EnableWindow( TRUE );
//		m_btnCheckName.EnableWindow( TRUE );
//		m_btnRecover.EnableWindow( TRUE );	
	}
	else
	{
		m_btnChangeUserlevel.EnableWindow( FALSE );
		m_btnCheckName.EnableWindow( FALSE );
		m_btnRecover.EnableWindow( FALSE );	
	}

	// Control 초기화
	InitUserInfo();
	InitListControl();	

	m_nSearchKind = 0;
	UpdateData( FALSE );

	char temp[256];
	for( int i = 1; i < 11; ++i )
		m_cbUserLevel.AddString( GetUserLevel( i, temp ) );

	CFont* pFont = GetFont();
	LOGFONT lf;
	pFont->GetLogFont( &lf );

	m_Font.CreatePointFont(100, "Arial");
	m_lcCharInfo.SetFont( &m_Font );
	m_listCharacter.SetFont( &m_Font );
	
//	m_etSearch.SetLimitText( 16 );
}

void CDS_RMToolView::InitUserInfo()
{
	for( int i = 0; i < eUserTemp_MAX; ++i )
		m_StatusEdit[i].SetWindowText( "" );

	m_stStatus.SetBitmap( ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP1)) );
	m_listCharacter.DeleteAllItems();
}

void CDS_RMToolView::InitListControl()
{
	char* column[4] = { "UserIdx", "CharacterIdx", "CharacterName", "CreateDate" };
	int columnsize[4] = { 80, 100, 120, 120 };
	
	// listctrl column 설정
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_CENTER;
	for( int i = 0; i < 4; ++i )
	{
		lvcolumn.iSubItem = i;
		lvcolumn.cx = columnsize[i];
		lvcolumn.pszText = column[i];
		m_lcCharInfo.InsertColumn( i, &lvcolumn );
	}
	// style
	DWORD dwExStyle = m_lcCharInfo.GetExtendedStyle();
	dwExStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_lcCharInfo.SetExtendedStyle( dwExStyle );

	char* tcolumn[8] = { "ChrIdx", "State", "Name", "CreateDate", "IP", "DeleteDate", "IP", "RecoverDate" };
	int tcolumnsize[8] = { 50, 70, 80, 120, 80, 120, 80, 120 };
	
	// listctrl column 설정
	lvcolumn;
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_CENTER;
	for( i = 0; i < 8; ++i )
	{
		lvcolumn.iSubItem = i;
		lvcolumn.cx = tcolumnsize[i];
		lvcolumn.pszText = tcolumn[i];
		m_listCharacter.InsertColumn( i, &lvcolumn );
	}
	// style
	dwExStyle = m_listCharacter.GetExtendedStyle();
	dwExStyle |= LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT;
	m_listCharacter.SetExtendedStyle( dwExStyle );
}

void CDS_RMToolView::SetUserInfo(USERINFO* user)
{
	char buff[256]={0,};

	m_StatusEdit[eUserTemp_ID].SetWindowText(user->strID);
	m_StatusEdit[eUserTemp_PWD].SetWindowText(user->strPass);
	m_StatusEdit[eUserTemp_NAME].SetWindowText(user->strName);
	sprintf(buff, "%s", user->strIdCard);
	m_StatusEdit[eUserTemp_IDCARD].SetWindowText(buff);
	sprintf(buff, "%s", user->strPhon);
	m_StatusEdit[eUserTemp_PHONE].SetWindowText(buff);
	m_StatusEdit[eUserTemp_MAIL].SetWindowText(user->strEmail);
	m_StatusEdit[eUserTemp_BIRTH].SetWindowText(user->strRegDateTime);
	m_StatusEdit[eUserTemp_LOGIN].SetWindowText(user->strLastLoginTime);
	m_StatusEdit[eUserTemp_LOGOUT].SetWindowText(user->strLastLogoutTime);
//	m_StatusEdit[eUserTemp_TOTAL].SetWindowText(user->strTotalTime);
	m_StatusEdit[eUserTemp_LEVEL].SetWindowTextEx( GetUserLevel(user->wUserLevel, buff ) );

	if( user->bLoginFlag )
		m_stStatus.SetBitmap( ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP3)) );
	else
		m_stStatus.SetBitmap( ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP2)) );

	CDS_RMToolDoc* pDoc = GetDocument();
	pDoc->SetUserInfo(user);

	m_cbUserLevel.EnableWindow( TRUE );
	m_cbUserLevel.SetCurSel( user->wUserLevel-1 );
}

void CDS_RMToolView::SetSimpleCharInfo( BYTE Count, SIMPlECHARACTERINFO* simple )
{
	m_lcCharInfo.DeleteAllItems();

	char temp[256];
	LV_ITEM lvitem;

	for( int i = 0; i < Count; ++i )
	{
		lvitem.iItem = m_lcCharInfo.GetItemCount();	// 행
		
		lvitem.iSubItem = 0;
		lvitem.mask = LVIF_TEXT;
		lvitem.stateMask = LVIS_SELECTED;
		lvitem.state = INDEXTOSTATEIMAGEMASK(1);
		lvitem.pszText = itoa( simple[i].dwUserIdx, temp, 10 );
		
		m_lcCharInfo.InsertItem( &lvitem );
		m_lcCharInfo.SetItemText( lvitem.iItem, 1, itoa( simple[i].dwCharNo, temp, 10 ) );	
		m_lcCharInfo.SetItemText( lvitem.iItem, 2, simple[i].strName );			// name
		m_lcCharInfo.SetItemText( lvitem.iItem, 3, simple[i].strCreateDate );	// date
	}
}

void CDS_RMToolView::SetCharacterCreateInfo( BYTE Count, CHARACTERCREATEINFO* pInfo )
{
	m_listCharacter.DeleteAllItems();

	char temp[256];
	LV_ITEM lvitem;

	for( int i = 0; i < Count; ++i )
	{
		lvitem.iItem = m_listCharacter.GetItemCount();	// 행
		
		lvitem.iSubItem = 0;
		lvitem.mask = LVIF_TEXT;
		lvitem.stateMask = LVIS_SELECTED;
		lvitem.state = INDEXTOSTATEIMAGEMASK(1);
		lvitem.pszText = itoa( pInfo[i].dwCharIdx, temp, 10 );
		
		m_listCharacter.InsertItem( &lvitem );
		switch( pInfo[i].dwFlag )
		{
		case 1:		m_listCharacter.SetItemText( lvitem.iItem, 1, "Create" );		break;
		case 2:		m_listCharacter.SetItemText( lvitem.iItem, 1, "Delete" );		break;
		case 3:		m_listCharacter.SetItemText( lvitem.iItem, 1, "Recover" );		break;
		}
		m_listCharacter.SetItemText( lvitem.iItem, 2, pInfo[i].sName );
		m_listCharacter.SetItemText( lvitem.iItem, 3, pInfo[i].sCreateDate );
		m_listCharacter.SetItemText( lvitem.iItem, 4, pInfo[i].sCreateIP );
		m_listCharacter.SetItemText( lvitem.iItem, 5, pInfo[i].sDeleteDate );
		m_listCharacter.SetItemText( lvitem.iItem, 6, pInfo[i].sDeleteIP );
		m_listCharacter.SetItemText( lvitem.iItem, 7, pInfo[i].sRecoverDate );
	}
}

void CDS_RMToolView::OnRadioUserID()
{
	m_nSearchKind = 0;
	m_etSearch.SetWindowText( "" );
}

void CDS_RMToolView::OnRadioCharName()
{
	m_nSearchKind = 1;
	m_etSearch.SetWindowText( "" );
}

void CDS_RMToolView::OnRadioJumin()
{
	m_nSearchKind = 2;
	m_etSearch.SetWindowText( "" );
}

void CDS_RMToolView::OnUserbtnSearch() 
{	
	if( m_etSearch.GetWindowTextLength() == 0 )
	{
		MessageBox("Input Search String!!", "Error", MB_OK);
		return;
	}

	CString str;
	m_etSearch.GetWindowText( str );

	if( str.GetLength() > 16 )
	{
		MessageBox("Long Search Data!!", "Error", MB_OK);
		return;
	}

	// 서버에 검색 요청
	TMSG_QUERYMEMBERNUM Msg;
	Msg.Category = MP_RMTOOL_USER;
	Msg.Protocol = MP_RMTOOL_QUERY_MEMBERNUM_SYN;
	Msg.dwTemplateIdx = GetDocument()->GetDocumentIdx();
	Msg.wServer = MASINFOMGR->GetCurMasInfo()->nServerSet;
	Msg.wType = m_nSearchKind;	
	strcpy( Msg.strSearch, str.GetBuffer(0) );

	NETWORK->Send( &Msg, sizeof(TMSG_QUERYMEMBERNUM) );

	m_lcCharInfo.DeleteAllItems();
	m_listCharacter.DeleteAllItems();
}

void CDS_RMToolView::OnDblclkListCharInfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CDS_RMToolDoc* pDoc = GetDocument();
	if( !pDoc ) return;

	int count = m_lcCharInfo.GetSelectedCount();
	if( count == 0 ) return;

	POSITION pos = m_lcCharInfo.GetFirstSelectedItemPosition();
	if( pos == NULL )
		return;

	int nCurSelectedIdx = m_lcCharInfo.GetNextSelectedItem(pos);
	CString str = m_lcCharInfo.GetItemText( nCurSelectedIdx, 1 );
	DWORD dwCharIdx = (DWORD)atoi( (char*)(LPCTSTR)str );
	DWORD dwServer = MASINFOMGR->GetCurMasInfo()->nServerSet;

	// character Temp를 열고 character Idx를 넘긴다.
	CDS_RMToolApp* pApp = (CDS_RMToolApp*)AfxGetApp();
	CCharacterDoc* pCharDoc = (CCharacterDoc*)pApp->OpenNewTemplate(eTemp_Character);

	pCharDoc->OpenCharacterInfo(dwServer, pDoc->m_sUserInfo->dwMemberNo, dwCharIdx, (pDoc->m_sUserInfoOrg.bLoginFlag)?TRUE:FALSE);

	AfxGetMainWnd()->BeginModalState();
	int r = AfxGetMainWnd()->RunModalLoop();

	if( r == 5000 )
	{
		AfxGetMainWnd()->EndModalState();
		AfxGetMainWnd()->SendMessage(WM_CLOSE,0,0);
	}
	
	*pResult = 0;
}

void CDS_RMToolView::OnDblclkListChar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CDS_RMToolDoc* pDoc = GetDocument();
	if( !pDoc ) return;

	int count = m_listCharacter.GetSelectedCount();
	if( count == 0 ) return;

	POSITION pos = m_listCharacter.GetFirstSelectedItemPosition();
	if( pos == NULL )
		return;

	int nCurSelectedIdx = m_listCharacter.GetNextSelectedItem(pos);
	CString str = m_listCharacter.GetItemText( nCurSelectedIdx, 0 );
	DWORD dwCharIdx = (DWORD)atoi( (char*)(LPCTSTR)str );
	DWORD dwServer = MASINFOMGR->GetCurMasInfo()->nServerSet;

	// character Temp를 열고 character Idx를 넘긴다.
	CDS_RMToolApp* pApp = (CDS_RMToolApp*)AfxGetApp();
	CCharacterDoc* pCharDoc = (CCharacterDoc*)pApp->OpenNewTemplate(eTemp_Character);

	pCharDoc->OpenCharacterInfo(dwServer, pDoc->m_sUserInfo->dwMemberNo, dwCharIdx, (pDoc->m_sUserInfoOrg.bLoginFlag)?TRUE:FALSE);

	AfxGetMainWnd()->BeginModalState();
	int r = AfxGetMainWnd()->RunModalLoop();

	if( r == 5000 )
	{
		AfxGetMainWnd()->EndModalState();
		AfxGetMainWnd()->SendMessage(WM_CLOSE,0,0);
	}
	
	*pResult = 0;
}

char* CDS_RMToolView::GetUserLevel( DWORD dwUserlevel, char* temp )
{
	switch( dwUserlevel )
	{
	case 1:		sprintf( temp, "God" );			break;
	case 2:		sprintf( temp, "Programer" );	break;
	case 3:		sprintf( temp, "Developer" );	break;
	case 4:		sprintf( temp, "GM" );			break;
	case 5:		sprintf( temp, "SuperUser" );	break;
	case 6:		sprintf( temp, "User" );		break;
	case 7:		sprintf( temp, "ForChinese" );	break;
	case 8:		sprintf( temp, "Temp1" );		break;
	case 9:		sprintf( temp, "Temp2" );		break;
	case 10:	sprintf( temp, "Blocked User" );	break;
	}
	return temp;
}

void CDS_RMToolView::OnClickListChar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POSITION pos = m_listCharacter.GetFirstSelectedItemPosition();
	if( pos == NULL )
	{
		m_etDelete.SetWindowText( "" );
		m_etRecover.SetWindowText( "" );
		m_etRecover.EnableWindow( FALSE );
		m_btnCheckName.EnableWindow( FALSE );
		m_btnRecover.EnableWindow( FALSE );		
		return;
	}

	int nCurSelectedIdx = m_listCharacter.GetNextSelectedItem(pos);
	CString str = m_listCharacter.GetItemText( nCurSelectedIdx, 1 );
	if( str != "Create" )
	{
		str = m_listCharacter.GetItemText( nCurSelectedIdx, 2 );
		m_etRecover.EnableWindow( TRUE );
		m_etDelete.SetWindowText( (char*)(LPCTSTR)str );
		m_etRecover.SetWindowText( (char*)(LPCTSTR)str );
		m_btnCheckName.EnableWindow( TRUE );
		m_btnRecover.EnableWindow( FALSE );
	}
	else
	{
		m_etDelete.SetWindowText( "" );
		m_etRecover.SetWindowText( "" );
		m_etRecover.EnableWindow( FALSE );
		m_btnCheckName.EnableWindow( FALSE );
		m_btnRecover.EnableWindow( FALSE );		
		return;
	}

	*pResult = 0;
}

void CDS_RMToolView::OnBtnRecover() 
{
	if( m_wPower != eOperPower_Master )
		return;

	if( MessageBox( "Are You Recover Character Data?", "Msg", MB_OKCANCEL ) == IDOK )
	{		
		if( m_etDelete.GetWindowTextLength() == 0 || m_etRecover.GetWindowTextLength() == 0 )
		{
			MessageBox( "Enter The Name!!", "Error!!" );
			return;
		}
		
		POSITION pos = m_listCharacter.GetFirstSelectedItemPosition();
		if( pos == NULL )	return;
		int nCurSelectedIdx = m_listCharacter.GetNextSelectedItem(pos);
		CString idx = m_listCharacter.GetItemText( nCurSelectedIdx, 0 );
		CString str;
		m_etRecover.GetWindowText( str );
		
		TMSG_CHARACTERRECOVER Msg;
		Msg.Category = MP_RMTOOL_USER;
		Msg.Protocol = MP_RMTOOL_CHARACTER_RECOVER_SYN;
		Msg.dwTemplateIdx = GetDocument()->GetDocumentIdx();
		Msg.dwChrIdx = (DWORD)atoi( (char*)(LPCTSTR)idx );
		strcpy( Msg.sName, (char*)(LPCTSTR)str );
		
		NETWORK->Send( &Msg, sizeof(TMSG_CHARACTERRECOVER) );
	}
}

void CDS_RMToolView::OnBtnCheckname() 
{
	if( m_etRecover.GetWindowTextLength() == 0 )
	{
		MessageBox( "Enter The Name!!", "Error!!" );
		return;
	}

	CString str;
	m_etRecover.GetWindowText( str );

	TMSG_CHECKNAME Msg;
	Msg.Category = MP_RMTOOL_USER;
	Msg.Protocol = MP_RMTOOL_CHECKNAME_SYN;
	Msg.dwTemplateIdx = GetDocument()->GetDocumentIdx();
	strcpy( Msg.sName, (char*)(LPCTSTR)str );

	NETWORK->Send( &Msg, sizeof(TMSG_CHECKNAME) );
}

void CDS_RMToolView::EnableRecoverButton()
{
	if( m_wPower == eOperPower_Master )
		m_btnRecover.EnableWindow( TRUE );
	else
		m_btnRecover.EnableWindow( FALSE );
}

void CDS_RMToolView::RecoverError( DWORD dwKind )
{
	switch( dwKind )
	{
	case 1:
		MessageBox( "There Is No Empty Character Slot!!", "Error!!" );
		break;
	case 2:
	case 3:
		MessageBox( "There Is Same Character Name!!", "Error!!" );
		break;
	case 4:
		MessageBox( "Character Name Is Null!!", "Error!!" );
		break;
	case 5:
		MessageBox( "Can't Recover!!", "ERROR!!" );
		break;
	}
}

void CDS_RMToolView::OnButtonChangeUserlevel() 
{
	if( m_wPower != eOperPower_Master )
		return;
	
	int userlevel = m_cbUserLevel.GetCurSel();
	if( userlevel > -1 )
	{
		if( MessageBox( "Are You Really Change UserLevel?", "Confirm", MB_OKCANCEL ) == IDOK )
		{
			CDS_RMToolDoc* pDoc = GetDocument();
			pDoc->ChangeUserLevel( userlevel + 1 );
		}
	}
}

void CDS_RMToolView::ChangeUserLevel( DWORD dwUserLevel )
{
	char buff[256] = {0, };
	m_StatusEdit[eUserTemp_LEVEL].SetWindowTextEx( GetUserLevel(dwUserLevel, buff ) );
	m_cbUserLevel.SetCurSel( dwUserLevel - 1 );
}
