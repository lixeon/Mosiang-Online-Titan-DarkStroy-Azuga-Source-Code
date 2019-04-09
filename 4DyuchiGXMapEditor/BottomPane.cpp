// BottomPane.cpp : implementation file
//

#include "stdafx.h"
#include "4dyuchigxmapeditor.h"
#include "BottomPane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBottomPane

IMPLEMENT_DYNCREATE(CBottomPane, CFormView)

CBottomPane::CBottomPane()
	: CFormView(CBottomPane::IDD)
{
	//{{AFX_DATA_INIT(CBottomPane)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBottomPane::~CBottomPane()
{
}

void CBottomPane::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBottomPane)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBottomPane, CFormView)
	//{{AFX_MSG_MAP(CBottomPane)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBottomPane diagnostics

#ifdef _DEBUG
void CBottomPane::AssertValid() const
{
	CFormView::AssertValid();
}

void CBottomPane::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBottomPane message handlers
