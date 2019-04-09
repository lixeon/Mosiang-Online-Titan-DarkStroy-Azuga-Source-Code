// CharacterView.cpp : implementation file
//

#include "stdafx.h"
#include "DS_RMTool.h"
#include "CharacterView.h"
#include "Characterinfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCharacterView

IMPLEMENT_DYNCREATE(CCharacterView, CFormView)

CCharacterView::CCharacterView()
	: CFormView(CCharacterView::IDD)
{
	//{{AFX_DATA_INIT(CCharacterView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pSheet = NULL;

	for(int i=0;i<eCharPage_Max;++i)
		m_pPageList[i] = NULL;
}

CCharacterView::~CCharacterView()
{
	SAFE_DELETE(m_pSheet);

	for(int i=0;i<eCharPage_Max;++i)
		SAFE_DELETE(m_pPageList[i]);
}

void CCharacterView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCharacterView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCharacterView, CFormView)
	//{{AFX_MSG_MAP(CCharacterView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCharacterView diagnostics

#ifdef _DEBUG
void CCharacterView::AssertValid() const
{
	CFormView::AssertValid();
}

void CCharacterView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCharacterView message handlers
void CCharacterView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

// create and associated the property sheet with the "place holder"window
	CWnd* pwndPropertySheetHolder = GetDlgItem(IDC_PROPSHEET);
	m_pSheet = new CLDPropSheet;

	// page link
	CCharacterInfo*	pPage = new CCharacterInfo;
	m_pPageList[eCharPage_CharacterInfo] = pPage;

	for(int i=0;i<eCharPage_Max;++i)
	{
		if( m_pPageList[i] != NULL )
			m_pSheet->AddPage(m_pPageList[i]);
	}
	//

	if(!m_pSheet->Create(pwndPropertySheetHolder, WS_CHILD | WS_VISIBLE, 0)) 
	{
		SAFE_DELETE(m_pSheet);

		for(int i=0;i<eCharPage_Max;++i)
			SAFE_DELETE(m_pPageList[i]);
		return;
	}

	//
	for(i=eCharPage_Max-1;i>=0;--i)
		m_pSheet->SetActivePage(i);

// fit the property sheet into the place holder window, and show it
	CRect rectPropertySheet;
	pwndPropertySheetHolder->SetWindowPos(NULL, 0, 0, 1260, 800, SWP_NOZORDER | SWP_NOACTIVATE);
	pwndPropertySheetHolder->GetWindowRect(rectPropertySheet);
	m_pSheet->SetWindowPos(NULL, 0, 0, rectPropertySheet.Width(), rectPropertySheet.Height(),
	SWP_NOZORDER | SWP_NOACTIVATE);
//
}

void CCharacterView::UpdateMainQuestData( TMSG_MAINQUESTINFO* pMsg )
{
	GetCharacterInfoPage()->UpdateMainQuestData( pMsg );
}

void CCharacterView::UpdateSubQuestData( TMSG_SUBQUESTINFO* pMsg )
{
	GetCharacterInfoPage()->UpdateSubQuestData( pMsg );
}

void CCharacterView::ChangeLoginPoint( DWORD dwMapNum )
{
	GetCharacterInfoPage()->ChangeLoginPoint( dwMapNum );
}

