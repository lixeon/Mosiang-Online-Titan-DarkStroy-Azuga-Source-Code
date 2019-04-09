// AdminView.cpp : implementation file
//

#include "stdafx.h"
#include "DS_RMTool.h"
#include "AdminView.h"
#include "AdminInfo.h"
#include "IpInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdminView

IMPLEMENT_DYNCREATE(CAdminView, CFormView)

CAdminView::CAdminView()
	: CFormView(CAdminView::IDD)
{
	//{{AFX_DATA_INIT(CAdminView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pSheet = NULL;

	for(int i=0;i<eAdminPage_Max;++i)
		m_pPageList[i] = NULL;
}

CAdminView::~CAdminView()
{
	SAFE_DELETE(m_pSheet);

	for(int i=0;i<eAdminPage_Max;++i)
		SAFE_DELETE(m_pPageList[i]);
}

void CAdminView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdminView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdminView, CFormView)
	//{{AFX_MSG_MAP(CAdminView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdminView diagnostics

#ifdef _DEBUG
void CAdminView::AssertValid() const
{
	CFormView::AssertValid();
}

void CAdminView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdminView message handlers

void CAdminView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class

	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

// create and associated the property sheet with the "place holder"window
	CWnd* pwndPropertySheetHolder = GetDlgItem(IDC_ADMIN_PROPSHEET);
	m_pSheet = new CLDPropSheet;

	// page link
	CAdminInfo*	pPage = new CAdminInfo;
	m_pPageList[eAdminPage_OperatorInfo] = pPage;
	CIpInfo* pIpPage = new CIpInfo;
	m_pPageList[eAdminPage_IpInfo] = pIpPage;

	for(int i=0;i<eAdminPage_Max;++i)
	{
		if( m_pPageList[i] != NULL )
			m_pSheet->AddPage(m_pPageList[i]);
	}
	//

	if(!m_pSheet->Create(pwndPropertySheetHolder, WS_CHILD | WS_VISIBLE, 0)) 
	{
		SAFE_DELETE(m_pSheet);

		for(int i=0;i<eAdminPage_Max;++i)
			SAFE_DELETE(m_pPageList[i]);
		return;
	}

	//
	for(i=eAdminPage_Max-1;i>=0;--i)
		m_pSheet->SetActivePage(i);

//	DWORD style = m_pSheet->GetTabControl()->GetStyle();
//	m_pSheet->GetTabControl()->ModifyStyle(0, TCS_BOTTOM);
//

// fit the property sheet into the place holder window, and show it
	CRect rectPropertySheet;
	pwndPropertySheetHolder->SetWindowPos(NULL, 0, 0, 500, 500, SWP_NOZORDER | SWP_NOACTIVATE);
	pwndPropertySheetHolder->GetWindowRect(rectPropertySheet);
	m_pSheet->SetWindowPos(NULL, 0, 0, rectPropertySheet.Width(), rectPropertySheet.Height(),
	SWP_NOZORDER | SWP_NOACTIVATE);
//
}
