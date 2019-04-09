// MunpaView.cpp : implementation file
//

#include "stdafx.h"
#include "DS_RMTool.h"
#include "MunpaView.h"
#include "MunpaDoc.h"
#include "MunpaInfo.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMunpaView

IMPLEMENT_DYNCREATE(CMunpaView, CFormView)

CMunpaView::CMunpaView()
	: CFormView(CMunpaView::IDD)
{
	//{{AFX_DATA_INIT(CMunpaView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pSheet = NULL;

	for(int i=0;i<eMunPage_Max;++i)
		m_pPageList[i] = NULL;
}

CMunpaView::~CMunpaView()
{
	SAFE_DELETE(m_pSheet);

	for(int i=0;i<eMunPage_Max;++i)
		SAFE_DELETE(m_pPageList[i]);
}

void CMunpaView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMunpaView)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMunpaView, CFormView)
	//{{AFX_MSG_MAP(CMunpaView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMunpaView diagnostics

#ifdef _DEBUG
void CMunpaView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMunpaView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMunpaDoc* CMunpaView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMunpaDoc)));
	return (CMunpaDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMunpaView message handlers
void CMunpaView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

// create and associated the property sheet with the "place holder"window
	CWnd* pwndPropertySheetHolder = GetDlgItem(IDC_PROPSHEET);
	m_pSheet = new CLDPropSheet(pwndPropertySheetHolder);

	// page link
	CMunpaInfo*	pPage = new CMunpaInfo;

	m_pPageList[eMunPage_MunpaInfo] = pPage;	

	for(int i=0;i<eMunPage_Max;++i)
	{
		if( m_pPageList[i] != NULL )
			m_pSheet->AddPage(m_pPageList[i]);
	}

	//
	if(!m_pSheet->Create(pwndPropertySheetHolder, WS_CHILD | WS_VISIBLE, 0)) 
	{
		SAFE_DELETE(m_pSheet);

		for(int i=0;i<eMunPage_Max;++i)
			SAFE_DELETE(m_pPageList[i]);
		return;
	}
	//

	CMunpaDoc* pDoc = GetDocument();
	if( pDoc == NULL )
	{
		SAFE_DELETE(m_pSheet);

		for(int i=0;i<eMunPage_Max;++i)
			SAFE_DELETE(m_pPageList[i]);
		return;
	}

	for(i=eMunPage_Max-1;i>=0;--i)
	{
		m_pSheet->SetActivePage(i);
		m_pPageList[i]->InitPage((void*)pDoc);
	}

//	DWORD style = m_pSheet->GetTabControl()->GetStyle();
//	m_pSheet->GetTabControl()->ModifyStyle(0, TCS_BOTTOM);

// fit the property sheet into the place holder window, and show it
	CRect rectPropertySheet;
	pwndPropertySheetHolder->SetWindowPos(NULL, 0, 0, 1100, 720, SWP_NOZORDER | SWP_NOACTIVATE);
	pwndPropertySheetHolder->GetWindowRect(rectPropertySheet);
	m_pSheet->SetWindowPos(NULL, 0, 0, rectPropertySheet.Width(), rectPropertySheet.Height(),
	SWP_NOZORDER | SWP_NOACTIVATE);
//
}
