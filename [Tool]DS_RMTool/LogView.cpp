// LogView.cpp : implementation file
//

#include "stdafx.h"
#include "ds_rmtool.h"
#include "LogView.h"
#include "LogItemMoneyPage.h"
//#include "LogCharacterPage.h"
//#include "LogSkillPage.h"
//#include "LogAbilityPage.h"
#include "LogDoc.h"
#include "LogTotalPage.h"
#include "LogEtcPage.h"
#include "LogFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogView

IMPLEMENT_DYNCREATE(CLogView, CFormView)

CLogView::CLogView()
	: CFormView(CLogView::IDD)
{
	//{{AFX_DATA_INIT(CLogView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	for( int i = 0; i < ePageMax; ++i )
		m_pPageList[i] = NULL;

	m_pProgressDlg = NULL;
}

CLogView::~CLogView()
{
	SAFE_DELETE( m_pSheet );
	for( int i = 0; i < ePageMax; ++i )
	{
		if( m_pPageList[i] )
			delete m_pPageList[i];
	}
}

void CLogView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogView, CFormView)
	//{{AFX_MSG_MAP(CLogView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogView diagnostics

#ifdef _DEBUG
void CLogView::AssertValid() const
{
	CFormView::AssertValid();
}

void CLogView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
CLogDoc* CLogView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLogDoc)));
	return (CLogDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLogView message handlers

void CLogView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

//	create and associated the property sheet with the "place holder"window
	CWnd* pwndPropertySheetHolder = GetDlgItem(IDC_PROPSHEET);
	m_pSheet = new CLDPropSheet(pwndPropertySheetHolder);

	// page link
	CLogItemMoneyPage* pItemMoneyPage = new CLogItemMoneyPage;
	pItemMoneyPage->SetDocument( GetDocument() );
	m_pPageList[eItemMoney_Page] = pItemMoneyPage;

	CLogTotalPage* pTotalPage = new CLogTotalPage;
	pTotalPage->SetDocument( GetDocument() );
	m_pPageList[eTotal_Page] = pTotalPage;

	CLogEtcPage* pEtcPage = new CLogEtcPage;
	pEtcPage->SetDocument( GetDocument() );
	m_pPageList[eEtc_Page] = pEtcPage;

	for( int i = 0; i < ePageMax; ++i )
	{
		if( m_pPageList[i] )
			m_pSheet->AddPage( m_pPageList[i] );
	}

	if( !m_pSheet->Create(pwndPropertySheetHolder, WS_CHILD | WS_VISIBLE, 0) ) 
	{
		SAFE_DELETE(m_pSheet);
		for( i = 0; i < ePageMax; ++i )
			SAFE_DELETE(m_pPageList[i]);
		return;
	}

	m_pSheet->SetActivePage(eItemMoney_Page);

//	fit the property sheet into the place holder window, and show it
	CRect rectPropertySheet;
	pwndPropertySheetHolder->SetWindowPos(NULL, 0, 0, 1200, 800, SWP_NOZORDER | SWP_NOACTIVATE);
	pwndPropertySheetHolder->GetWindowRect(rectPropertySheet);
	m_pSheet->SetWindowPos(NULL, 0, 0, rectPropertySheet.Width(), rectPropertySheet.Height(), SWP_NOZORDER | SWP_NOACTIVATE);

	pItemMoneyPage->ChangeFont();
	pTotalPage->ChangeFont();
	pEtcPage->ChangeFont();
}

void CLogView::SetLogItemMoney( TMSG_LOGITEMMONEY* pMsg )
{
	CLogItemMoneyPage* pPage = (CLogItemMoneyPage*)m_pPageList[eItemMoney_Page];
	if( pPage )
		pPage->SetLogItemMoney( pMsg );
}

void CLogView::SetItemMoneyLogCharacterIdx( DWORD dwCharIdx )
{
	CLogItemMoneyPage* pPage = (CLogItemMoneyPage*)m_pPageList[eItemMoney_Page];
	if( pPage )
		pPage->SetCharacterIdx( dwCharIdx );	
}

void CLogView::SetTotalLogChacterIdx( DWORD dwCharIdx )
{
	CLogTotalPage* pPage = (CLogTotalPage*)m_pPageList[eTotal_Page];
	if( pPage )
		pPage->SetCharacterIdx( dwCharIdx );
}

void CLogView::SetEtcLogChacterIdx( DWORD dwCharIdx )
{
	CLogEtcPage* pPage = (CLogEtcPage*)m_pPageList[eEtc_Page];
	if( pPage )
		pPage->SetCharacterIdx( dwCharIdx );
}

void CLogView::SetCharacterLog( TMSG_CHARACTERLOG* pMsg )
{
	CLogTotalPage* pPage = (CLogTotalPage*)m_pPageList[eTotal_Page];
	if( pPage )
		pPage->SetCharacterLog( pMsg );
}

void CLogView::SetExpPointLog( TMSG_EXPPOINTLOG* pMsg )
{
	CLogTotalPage* pPage = (CLogTotalPage*)m_pPageList[eTotal_Page];
	if( pPage )
		pPage->SetExpPointLog( pMsg );
}

void CLogView::SetMugongLog( TMSG_MUGONGLOG* pMsg )
{
	CLogTotalPage* pPage = (CLogTotalPage*)m_pPageList[eTotal_Page];
	if( pPage )
		pPage->SetMugongLog( pMsg );
}

void CLogView::SetMugongExpLog( TMSG_MUGONGEXPLOG* pMsg )
{
	CLogTotalPage* pPage = (CLogTotalPage*)m_pPageList[eTotal_Page];
	if( pPage )
		pPage->SetMugongExpLog( pMsg );
}

void CLogView::SetMoneyLog( TMSG_MONEYLOG* pMsg )
{
	CLogTotalPage* pPage = (CLogTotalPage*)m_pPageList[eTotal_Page];
	if( pPage )
		pPage->SetMoneyLog( pMsg );
}

void CLogView::SetMoneyWrongLog( TMSG_MONEYWRONGLOG* pMsg )
{
	CLogTotalPage* pPage = (CLogTotalPage*)m_pPageList[eTotal_Page];
	if( pPage )
		pPage->SetMoneyWrongLog( pMsg );
}

void CLogView::SetAbilityLog( TMSG_ABILITYLOG* pMsg )
{
	CLogTotalPage* pPage = (CLogTotalPage*)m_pPageList[eTotal_Page];
	if( pPage )
		pPage->SetAbilityLog( pMsg );
}

void CLogView::SetCheatLog( TMSG_CHEATLOG* pMsg )
{
	CLogEtcPage* pPage = (CLogEtcPage*)m_pPageList[eEtc_Page];
	if( pPage )
		pPage->SetCheatLog( pMsg );
}

void CLogView::SetHackingLog( TMSG_HACKINGLOG* pMsg )
{
	CLogEtcPage* pPage = (CLogEtcPage*)m_pPageList[eEtc_Page];
	if( pPage )
		pPage->SetHackingLog( pMsg );
}

BOOL CLogView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class

/*	if( m_bReady )
	{
		for( int i = 0; i < 3; ++i )
		{
			if( m_pPageList[i] )
				m_pPageList[0]->CommandMessage( nID, nCode );
		}
	}
*/	
	return CFormView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CLogView::StartDlg()
{
	if( m_pProgressDlg )
		m_pProgressDlg->SetFocus();
	else
	{
		m_pProgressDlg = new CProgressDlg;
		m_pProgressDlg->Create( (CLogFrm*)GetParentFrame() );
		m_pProgressDlg->ShowWindow( SW_SHOW );
	}	
}

void CLogView::EndDlg()
{
	if( m_pProgressDlg )
	{
		delete m_pProgressDlg;
		m_pProgressDlg = NULL;
	}
}