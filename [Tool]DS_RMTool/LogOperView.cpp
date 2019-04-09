// LogOperView.cpp : implementation file
//

#include "stdafx.h"
#include "ds_rmtool.h"
#include "LogOperView.h"
#include "LogOperDoc.h"
#include "LogOperPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogOperView

IMPLEMENT_DYNCREATE(CLogOperView, CFormView)

CLogOperView::CLogOperView()
	: CFormView(CLogOperView::IDD)
{
	//{{AFX_DATA_INIT(CLogOperView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSheet = NULL;
	m_pPage = NULL;
}

CLogOperView::~CLogOperView()
{
	SAFE_DELETE( m_pSheet );
	SAFE_DELETE( m_pPage );
}

void CLogOperView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogOperView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogOperView, CFormView)
	//{{AFX_MSG_MAP(CLogOperView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogOperView diagnostics

#ifdef _DEBUG
void CLogOperView::AssertValid() const
{
	CFormView::AssertValid();
}

void CLogOperView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
CLogOperDoc* CLogOperView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLogOperDoc)));
	return (CLogOperDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLogOperView message handlers

void CLogOperView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();	

	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	
	// create and associated the property sheet with the "place holder"window
	CWnd* pwndPropertySheetHolder = GetDlgItem(IDC_PROPSHEET);
	m_pSheet = new CLDPropSheet(pwndPropertySheetHolder);

	// page link
	CLogOperPage* pLogOperPage = new CLogOperPage;
	pLogOperPage->SetDocument( GetDocument() );
	m_pPage = pLogOperPage;

	m_pSheet->AddPage( m_pPage );
	if( !m_pSheet->Create(pwndPropertySheetHolder, WS_CHILD | WS_VISIBLE, 0) ) 
	{
		SAFE_DELETE(m_pSheet);
		SAFE_DELETE(m_pPage);
		return;
	}

	// fit the property sheet into the place holder window, and show it
	CRect rectPropertySheet;
	pwndPropertySheetHolder->SetWindowPos(NULL, 0, 0, 1200, 800, SWP_NOZORDER | SWP_NOACTIVATE);
	pwndPropertySheetHolder->GetWindowRect(rectPropertySheet);
	m_pSheet->SetWindowPos(NULL, 0, 0, rectPropertySheet.Width(), rectPropertySheet.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
	
	pLogOperPage->ChangeFont();
}

void CLogOperView::SetOperLog( TMSG_TOOLLOG* pMsg )
{
	if( m_pPage )
	{
		CLogOperPage* pLogOperPage = (CLogOperPage*)m_pPage;
		pLogOperPage->SetOperLog( pMsg );
	}
}
