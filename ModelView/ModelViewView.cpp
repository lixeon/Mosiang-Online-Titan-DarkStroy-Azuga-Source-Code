// ModelViewView.cpp : implementation of the CModelViewView class
//

#include "stdafx.h"
#include "ModelView.h"
#include "ModelViewDoc.h"
#include "ModelViewView.h"
#include "ToolInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CModelViewView

IMPLEMENT_DYNCREATE(CModelViewView, CView)

BEGIN_MESSAGE_MAP(CModelViewView, CView)
	//{{AFX_MSG_MAP(CModelViewView)	
	ON_WM_CREATE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CModelViewView construction/destruction

CModelViewView::CModelViewView()
{
	// TODO: add construction code here

}

CModelViewView::~CModelViewView()
{
}

BOOL CModelViewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CModelViewView drawing

void CModelViewView::OnDraw(CDC* pDC)
{
	CModelViewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CModelViewView diagnostics

#ifdef _DEBUG
void CModelViewView::AssertValid() const
{
	CView::AssertValid();
}

void CModelViewView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CModelViewDoc* CModelViewView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CModelViewDoc)));
	return (CModelViewDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CModelViewView message handlers


int CModelViewView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Initialize Another relation of activity variables.

	m_bRightMouseDown = FALSE;
	m_iRightMouseDownX = 0;
	m_iRightMouseDownY = 0;

	m_bLeftMouseDown = FALSE;
	m_iLeftMouseDownX = 0;
	m_iLeftMouseDownY = 0;
	
	return 0;
}

void CModelViewView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if (FALSE == m_bRightMouseDown)
	{
		m_bRightMouseDown = TRUE;
		m_iRightMouseDownX = point.x;
		m_iRightMouseDownY = point.y;
	}
	
	CView::OnRButtonDown(nFlags, point);
}

void CModelViewView::OnRButtonUp(UINT nFlags, CPoint point) 
{	
	m_iRightMouseDownX = 0;
	m_iRightMouseDownY = 0;
	m_bRightMouseDown = FALSE;

	CView::OnRButtonUp(nFlags, point);
}

void CModelViewView::OnMouseMove(UINT nFlags, CPoint point) 
{	
	if (TRUE == m_bRightMouseDown)
	{			
		g_ToolInfo.m_Camera.RotateCamera(g_ToolInfo.m_gxhCurrentRendered,
										 m_iRightMouseDownX,
										 m_iRightMouseDownY,
										 point.x,
										 point.y);		
		
		m_iRightMouseDownX = point.x;
		m_iRightMouseDownY = point.y;		

	}

	if (TRUE == m_bLeftMouseDown)
	{
		g_ToolInfo.m_Camera.MoveCamera(m_iLeftMouseDownX,
									   m_iLeftMouseDownY,
									   point.x,
									   point.y);

		m_iLeftMouseDownX = point.x;
		m_iLeftMouseDownY = point.y;
	}	


	CView::OnMouseMove(nFlags, point);
}
void CModelViewView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	g_ToolInfo.OnLButtonDown(nFlags,point);

				
				
				
	if (FALSE == m_bLeftMouseDown)
	{
		m_bLeftMouseDown = TRUE;
		m_iLeftMouseDownX = point.x;
		m_iLeftMouseDownY = point.y;
	}
	
	CView::OnLButtonDown(nFlags, point);
}

void CModelViewView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_iLeftMouseDownX = 0;
	m_iLeftMouseDownY = 0;
	m_bLeftMouseDown = FALSE;
	
	CView::OnLButtonUp(nFlags, point);
}

void CModelViewView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	if (NULL != g_pExecutive)
	{
		g_pExecutive->GetGeometry()->UpdateWindowSize();
	}	
}
