// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "DS_RMTool.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_ADMIN, OnAdmin)
	ON_COMMAND(ID_LOG, OnGamelog)
	ON_COMMAND(ID_MUNPAINFO, OnMunpainfo)
	ON_COMMAND(ID_USERINFO, OnUserinfo)
	ON_COMMAND(ID_OPERLOG, OnOperlog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_MainToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_LEFT | TBSTYLE_FLAT | CBRS_TOOLTIPS) ||
		!m_MainToolBar.LoadToolBar(IDR_RMTOOLBAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_MainToolBar.ModifyStyle(0, TBSTYLE_FLAT);

	// Set up hot bar image lists.
	CImageList	imageList;
	CBitmap		bitmap;

	// Create and set the normal toolbar image list.
	bitmap.LoadBitmap(IDB_RMTOOLBAR);
	imageList.Create(30, 30, ILC_COLORDDB|ILC_MASK, 5, 0);
	imageList.Add(&bitmap, RGB(192,192,192));
	m_MainToolBar.SendMessage(TB_SETIMAGELIST, 0, (LPARAM)imageList.m_hImageList);
	m_MainToolBar.SendMessage(TB_SETHOTIMAGELIST, 0, (LPARAM)imageList.m_hImageList);
	imageList.Detach();
	bitmap.Detach();

	m_MainToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
//	DockControlBar(&m_MainToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( cs.hMenu != NULL )
	{
		::DestroyMenu(cs.hMenu);
		cs.hMenu = NULL;
	}

	cs.style &= ~( WS_THICKFRAME | WS_VSCROLL | WS_HSCROLL);
	cs.style &= ~FWS_ADDTOTITLE;	//제목없음 안나오게..ㅡㅡ 
	cs.dwExStyle &= ~WS_EX_TOPMOST;
		
	return CMDIFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}
#endif //_DEBUG


void CMainFrame::BeginModalState()
{
	CMDIFrameWnd::BeginModalState();
}

void CMainFrame::EndModalState()
{
	CMDIFrameWnd::EndModalState();
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFrameWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext);
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CreateClient(lpcs, NULL);
}

void CMainFrame::OnUserinfo() 
{
	CDS_RMToolApp* pApp = (CDS_RMToolApp*)AfxGetApp();
	pApp->OpenNewTemplate(eTemp_UserInfo);
}

void CMainFrame::OnMunpainfo() 
{
	CDS_RMToolApp* pApp = (CDS_RMToolApp*)AfxGetApp();
	pApp->OpenNewTemplate(eTemp_MunpaInfo);	
}

void CMainFrame::OnGamelog() 
{
#ifdef _CHINA
		if( OperatorPowerCheck( eOperPower_Master ) ||
			OperatorPowerCheck( eOperPower_Auditor) ||
			OperatorPowerCheck( eOperPower_Monitor) )
		{
			CDS_RMToolApp* pApp = (CDS_RMToolApp*)AfxGetApp();
			pApp->OpenNewTemplate(eTemp_Log);
		}
		else
		{
			MessageBox( "Your Power Is Not Right!!", "Error" );
			return;
		}
#else
		CDS_RMToolApp* pApp = (CDS_RMToolApp*)AfxGetApp();
		pApp->OpenNewTemplate(eTemp_Log);	
#endif
}

void CMainFrame::OnAdmin() 
{
	if( OperatorPowerCheck( eOperPower_Master ) )
	{
		CDS_RMToolApp* pApp = (CDS_RMToolApp*)AfxGetApp();
		pApp->OpenNewTemplate(eTemp_Admin);	
	}
	else
	{
		MessageBox( "Your Power Is Not Right!!", "Error" );
		return;
	}
}

void CMainFrame::OnOperlog() 
{	
	if( OperatorPowerCheck( eOperPower_Master ) || OperatorPowerCheck( eOperPower_Auditor) )
	{
		CDS_RMToolApp* pApp = (CDS_RMToolApp*)AfxGetApp();
		pApp->OpenNewTemplate(eTemp_OperLog);	
	}
	else
	{
		MessageBox( "Your Power Is Not Right!!", "Error" );
		return;
	}
}

BOOL CMainFrame::OperatorPowerCheck( WORD wOperPower )
{
	WORD wCurPower = ((CDS_RMToolApp*)AfxGetApp())->GetCurOperatorPower();
	if( wCurPower == wOperPower )
		return TRUE;
	return FALSE;
}
