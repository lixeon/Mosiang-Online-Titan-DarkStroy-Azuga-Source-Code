// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "RegenTool.h"
#include "RegenToolDoc.h"
#include "RegenToolView.h"
#include "MainFrm.h"
#include "MHMap.h"
#include "TileManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_MAPDATA_OPEN, OnMapdataOpen)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

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
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.cx = 1010;
	cs.cy = 720;
	cs.x = 0;
	cs.y = 0;
	
	return CFrameWnd::PreCreateWindow(cs);
//	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnMapdataOpen() 
{
	// TODO: Add your command handler code here
	
}

void CMainFrame::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CRegenToolView* pView = (CRegenToolView*)GetActiveView();
	char Filter[] = "MAP format(*.map)|*.map|BMP format(*.bmp)|*.bmp|TXT format(*.txt)|*.txt|TTB format(*.ttb)|*.ttb|All format(*.*)|*.*|";
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, Filter);
	if(fileDlg.DoModal() == IDOK)
	{
		CString data = fileDlg.GetPathName();
		CString Ext = fileDlg.GetFileExt();
		CString filename = fileDlg.GetFileName();
		if(!strcmp(Ext, "bmp"))
		{
			pView->LoadMapImage(data);
			Invalidate(TRUE);
		}
		else if(!strcmp(Ext, "map"))
		{
			if(!strcmp("58.map",(LPTSTR)(LPCTSTR)filename))
			{
				g_bFixHeight = TRUE;
				g_fFixHeight = 0;
			}
			else if(!strcmp("35.map",(LPTSTR)(LPCTSTR)filename))
			{
				g_bFixHeight = TRUE;
				g_fFixHeight = 0;
			}
			MAP->TestLoadMap((LPTSTR)(LPCTSTR)data);
			pView->SetStartSelectFlag(TRUE);
		}
		else if(!strcmp(Ext, "txt"))
		{
			
			pView->LoadDataInsert(data);
		}
		else if(!strcmp(Ext, "ttb"))
		{
			CTile* pTile = pView->GetViewTile();
//			SAFE_DELETE_ARRAY(pTile);
			MAP->InitTile((LPTSTR)(LPCTSTR)data, pTile);
		}
	}
}

void CMainFrame::OnFileSave() 
{
	// TODO: Add your command handler code here
	
}
