// 4DyuchiGXMapEditorView.cpp : implementation of the CMy4DyuchiGXMapEditorView class
//

#include "stdafx.h"
#include "4DyuchiGXMapEditor.h"

#include "4DyuchiGXMapEditorDoc.h"
#include "4DyuchiGXMapEditorView.h"
#include "SectionInfoDlg.h"
#include "NpcInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CORUM_ONLINE
/////////////////////////////////////////////////////////////////////////////
// CMy4DyuchiGXMapEditorView

IMPLEMENT_DYNCREATE(CMy4DyuchiGXMapEditorView, CView)

BEGIN_MESSAGE_MAP(CMy4DyuchiGXMapEditorView, CView)
	//{{AFX_MSG_MAP(CMy4DyuchiGXMapEditorView)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_CANCELMODE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_SECTION_INPUT, OnSectionInput)
	ON_UPDATE_COMMAND_UI(ID_SECTION_INPUT, OnUpdateSectionInput)
	ON_COMMAND(ID_SECTION_INFO, OnSectionInfo)
	ON_UPDATE_COMMAND_UI(ID_SECTION_INFO, OnUpdateSectionInfo)
	ON_WM_MBUTTONDOWN()
	ON_COMMAND(ID_CREATE_NPC, OnCreateNpc)
	ON_UPDATE_COMMAND_UI(ID_CREATE_NPC, OnUpdateCreateNpc)
	ON_COMMAND(ID_LOAD_TTB, OnLoadTtb)
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_MOVESPOT_INPUT, OnMovespotInput)
	ON_UPDATE_COMMAND_UI(ID_MOVESPOT_INPUT, OnUpdateMovespotInput)
	ON_COMMAND(ID_MOVESPOT_INFO, OnMovespotInfo)
	ON_UPDATE_COMMAND_UI(ID_MOVESPOT_INFO, OnUpdateMovespotInfo)
	ON_COMMAND(ID_STARTSPOT_INFO, OnStartspotInfo)
	ON_UPDATE_COMMAND_UI(ID_STARTSPOT_INFO, OnUpdateStartspotInfo)
	ON_COMMAND(ID_STARTSPOT_INPUT, OnStartspotInput)
	ON_UPDATE_COMMAND_UI(ID_STARTSPOT_INPUT, OnUpdateStartspotInput)
	ON_COMMAND(ID_SAVE_TTB, OnSaveTtb)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMy4DyuchiGXMapEditorView construction/destruction

CMy4DyuchiGXMapEditorView::CMy4DyuchiGXMapEditorView()
{
	// TODO: add construction code here

}

CMy4DyuchiGXMapEditorView::~CMy4DyuchiGXMapEditorView()
{
}

BOOL CMy4DyuchiGXMapEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMy4DyuchiGXMapEditorView drawing

void CMy4DyuchiGXMapEditorView::OnDraw(CDC* pDC)
{
	CMy4DyuchiGXMapEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CMy4DyuchiGXMapEditorView printing

BOOL CMy4DyuchiGXMapEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMy4DyuchiGXMapEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMy4DyuchiGXMapEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMy4DyuchiGXMapEditorView diagnostics

#ifdef _DEBUG
void CMy4DyuchiGXMapEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CMy4DyuchiGXMapEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy4DyuchiGXMapEditorDoc* CMy4DyuchiGXMapEditorView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy4DyuchiGXMapEditorDoc)));
	return (CMy4DyuchiGXMapEditorDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMy4DyuchiGXMapEditorView message handlers

void CMy4DyuchiGXMapEditorView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	g_pTool->Render();

	// Do not call CView::OnPaint() for painting messages
}


void CMy4DyuchiGXMapEditorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	g_pTool->OnKeyDown(nChar,nRepCnt,nFlags);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CMy4DyuchiGXMapEditorView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	g_pTool->OnMouseMove(nFlags,point);

//Added by KBS 030224
#ifdef CORUM_ONLINE
	g_pTool->Render();
#endif
//--

	CView::OnMouseMove(nFlags, point);
}

void CMy4DyuchiGXMapEditorView::OnCancelMode() 
{
	CView::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}

void CMy4DyuchiGXMapEditorView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	
    g_pTool->OnLButtonDown(nFlags,&point);

	CView::OnLButtonDown(nFlags, point);
}

void CMy4DyuchiGXMapEditorView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
    g_pTool->OnLButtonUp(nFlags,&point);

	CView::OnLButtonUp(nFlags, point);
}

void CMy4DyuchiGXMapEditorView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (g_pTool)
		g_pTool->OnSize(nType,cx,cy);
}



void CMy4DyuchiGXMapEditorView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	g_pTool->OnRButtonDown(nFlags,&point);
	CView::OnRButtonDown(nFlags, point);
}

//-------------------------------------------------------------------------------------------//
// CorumZero MapTool 추가 작업.
void CMy4DyuchiGXMapEditorView::OnMButtonDown( UINT nFlags, CPoint point )
{
	// 오른쪽 버튼을 쓰고 있으므로 중앙 버튼으로 팝업 배뉴를 설정했다.

#ifdef CORUM_ONLINE
	CMenu	muTemp, *pContextMenu;
	muTemp.LoadMenu( IDR_POPUP );
	pContextMenu = muTemp.GetSubMenu( 0 );

	CPoint pos = point;
	ClientToScreen( &pos );
	pContextMenu->TrackPopupMenu( TPM_LEFTALIGN, pos.x, pos.y, AfxGetMainWnd() );	
#endif

	g_pTool->OnMButtonDown(nFlags,&point);

	CView::OnMButtonDown( nFlags, point );
}

void CMy4DyuchiGXMapEditorView::OnSectionInput() 
{
	g_pTool->m_pSectionSplitter->bSectionInputMode = !g_pTool->m_pSectionSplitter->bSectionInputMode;

	//섹션 인풋 모드일때 
	if( g_pTool->m_pSectionSplitter->bSectionInputMode )
	{
		if(g_pTool->m_pSpotSplitter->bSectionInputMode)	//
		{
			g_pTool->DisableSpotSplitter();
			g_pTool->m_pSpotSplitter->bSectionInputMode = FALSE;
		}

		if(g_pTool->m_pStartSplitter->bSectionInputMode)	//
		{
			g_pTool->DisableStartSpotSplitter();
			g_pTool->m_pStartSplitter->bSectionInputMode = FALSE;
		}
		
		g_pTool->EnableSectionSplitter();
	}
	else
	{
		g_pTool->DisableSectionSplitter();
	}
}

void CMy4DyuchiGXMapEditorView::OnSectionInfo() 
{
	Section_Info*	pSection_Info;

	pSection_Info = g_pTool->GetSectionInfo();
	
	if( pSection_Info )
	{
		CSectionInfoDlg		SectionDlg;
		SectionDlg.m_strSectionNum.Format("%d", pSection_Info->wSectionNum);
		SectionDlg.m_strTop.Format("%d", pSection_Info->rcSection.top);
		SectionDlg.m_strBottom.Format("%d", pSection_Info->rcSection.bottom);
		SectionDlg.m_strLeft.Format("%d", pSection_Info->rcSection.left);
		SectionDlg.m_strRight.Format("%d", pSection_Info->rcSection.right);

		if( SectionDlg.DoModal() == IDCANCEL )
		{
			g_pTool->DeleteSection( pSection_Info );
		}
	}
	else
	{
		MessageBox( "Section not found!", "Failed", MB_OK );
	}
}

void CMy4DyuchiGXMapEditorView::OnLoadTtb() 
{
	char szFilters[] = "Tile Table Files (*.ttb) | *.ttb ||";
	char* filename;
		
	CString pathname;
	CFileDialog dlg(FALSE,"ttb","*.ttb",NULL,szFilters,NULL);

	if (dlg.DoModal() == IDOK)
	{
		pathname = (LPSTR)(LPCSTR)(dlg.GetPathName());
		if (pathname == "")
			return;
	}

	if (!(filename = (LPSTR)(LPCSTR)pathname))
		return;

	if (!lstrlen(filename))
		return;
	
	g_pTool->Loadttb(filename);

	//Added by KBS 030224
	CMy4DyuchiGXMapEditorDoc* pDoc =  (CMy4DyuchiGXMapEditorDoc*)GetDocument();
	pDoc->SetTitle(filename);
	//--
}

void CMy4DyuchiGXMapEditorView::OnUpdateSectionInput(CCmdUI* pCmdUI) 
{
	if( g_pTool->GetHField() )
	{
		pCmdUI->Enable( TRUE );
		if( g_pTool->m_pSectionSplitter->bSectionInputMode )
		{
			pCmdUI->SetCheck( 1 );
		}
		else 
		{
			pCmdUI->SetCheck( 0 );
		}
	}
	else
	{
		pCmdUI->Enable( FALSE );
	}
}

void CMy4DyuchiGXMapEditorView::OnUpdateSectionInfo(CCmdUI* pCmdUI) 
{
	if( g_pTool->GetHField() )
	{
		if(g_pTool->m_pStartSplitter->bSectionInputMode || g_pTool->m_pSpotSplitter->bSectionInputMode)
			pCmdUI->Enable( FALSE );
		else
			pCmdUI->Enable( TRUE );
	}
	else
	{
		pCmdUI->Enable( FALSE );
	}
}

void CMy4DyuchiGXMapEditorView::OnCreateNpc() 
{
	DWORD	dwPosX, dwPosY;
	VECTOR3	v3Clicked;
	float	fDist;

	if (!g_pTool->GetHField()->GetTilePosWithScreenCoord(&v3Clicked,&fDist,&dwPosX,&dwPosY,&g_pTool->m_pSectionSplitter->mousePos,0)) 
		return;
	
	CNpcInfoDlg	NpcInfoDlg;
	NpcInfoDlg.m_strPosX.Format( "%d", dwPosX );
	NpcInfoDlg.m_strPosZ.Format( "%d", dwPosY );
	
	NpcInfoDlg.DoModal();
}

void CMy4DyuchiGXMapEditorView::OnUpdateCreateNpc(CCmdUI* pCmdUI) 
{	
	if( g_pTool->GetHField() )
	{
		pCmdUI->Enable( TRUE );
	}
	else
	{
		pCmdUI->Enable( FALSE );
	}
}


void CMy4DyuchiGXMapEditorView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CView::OnRButtonUp(nFlags, point);
}

// Added by KBS 
void CMy4DyuchiGXMapEditorView::OnMovespotInput() 
{
	// TODO: Add your command handler code here
	g_pTool->m_pSpotSplitter->bSectionInputMode = !g_pTool->m_pSpotSplitter->bSectionInputMode;


	if( g_pTool->m_pSpotSplitter->bSectionInputMode )
	{
		if(g_pTool->m_pSectionSplitter->bSectionInputMode)	
		{
			g_pTool->DisableSectionSplitter();
			g_pTool->m_pSectionSplitter->bSectionInputMode = FALSE;
		}

		if(g_pTool->m_pStartSplitter->bSectionInputMode)	
		{
			g_pTool->DisableStartSpotSplitter();
			g_pTool->m_pStartSplitter->bSectionInputMode = FALSE;
		}

		g_pTool->EnableSpotSplitter();
	}
	else
	{
		g_pTool->DisableSpotSplitter();
	}
}

void CMy4DyuchiGXMapEditorView::OnUpdateMovespotInput(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pTool->GetHField() )
	{
		pCmdUI->Enable( TRUE );
		if( g_pTool->m_pSpotSplitter->bSectionInputMode )
		{
			pCmdUI->SetCheck( 1 );
		}
		else 
		{
			pCmdUI->SetCheck( 0 );
		}
	}
	else
	{
		pCmdUI->Enable( FALSE );
	}
}


void CMy4DyuchiGXMapEditorView::OnMovespotInfo() 
{
	// TODO: Add your command handler code here
	Section_Info*	pSection_Info;

	pSection_Info = g_pTool->GetSpotInfo();
	
	if( pSection_Info )
	{
		CSectionInfoDlg		SectionDlg;
		SectionDlg.m_strSectionNum.Format("%d", pSection_Info->wSectionNum);
		SectionDlg.m_strTop.Format("%d", pSection_Info->rcSection.top);
		SectionDlg.m_strBottom.Format("%d", pSection_Info->rcSection.bottom);
		SectionDlg.m_strLeft.Format("%d", pSection_Info->rcSection.left);
		SectionDlg.m_strRight.Format("%d", pSection_Info->rcSection.right);

		if( SectionDlg.DoModal() == IDCANCEL )
		{
			g_pTool->DeleteSpot( pSection_Info );
		}
	}
	else
	{
		MessageBox( "MoveSpot not found!", "Failed", MB_OK );
	}
}

void CMy4DyuchiGXMapEditorView::OnUpdateMovespotInfo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pTool->GetHField() )
	{
		if(g_pTool->m_pSectionSplitter->bSectionInputMode || g_pTool->m_pStartSplitter->bSectionInputMode)
			pCmdUI->Enable( FALSE );
		else
			pCmdUI->Enable( TRUE );
	}
	else
	{
		pCmdUI->Enable( FALSE );
	}
}
//--

void CMy4DyuchiGXMapEditorView::OnStartspotInfo() 
{
	Section_Info*	pSection_Info;

	pSection_Info = g_pTool->GetStartSpotInfo();
	
	if( pSection_Info )
	{
		CSectionInfoDlg		SectionDlg;
		SectionDlg.m_strSectionNum.Format("%d", pSection_Info->wSectionNum);
		SectionDlg.m_strTop.Format("%d", pSection_Info->rcSection.top);
		SectionDlg.m_strBottom.Format("%d", pSection_Info->rcSection.bottom);
		SectionDlg.m_strLeft.Format("%d", pSection_Info->rcSection.left);
		SectionDlg.m_strRight.Format("%d", pSection_Info->rcSection.right);

		if( SectionDlg.DoModal() == IDCANCEL )
		{
			g_pTool->DeleteStartSpot( pSection_Info );
		}
	}
	else
	{
		MessageBox( "StartSpot not found!", "Failed", MB_OK );
	}
	
}

void CMy4DyuchiGXMapEditorView::OnUpdateStartspotInfo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pTool->GetHField() )
	{
		if(g_pTool->m_pSectionSplitter->bSectionInputMode || g_pTool->m_pSpotSplitter->bSectionInputMode)
			pCmdUI->Enable( FALSE );
		else
			pCmdUI->Enable( TRUE );
	}
	else
	{
		pCmdUI->Enable( FALSE );
	}
	
}

void CMy4DyuchiGXMapEditorView::OnStartspotInput() 
{
	// TODO: Add your command handler code here
	g_pTool->m_pStartSplitter->bSectionInputMode = !g_pTool->m_pStartSplitter->bSectionInputMode;


	if( g_pTool->m_pStartSplitter->bSectionInputMode )
	{
		if(g_pTool->m_pSectionSplitter->bSectionInputMode)	
		{
			g_pTool->DisableSectionSplitter();
			g_pTool->m_pSectionSplitter->bSectionInputMode = FALSE;
		}

		if(g_pTool->m_pSpotSplitter->bSectionInputMode)	
		{
			g_pTool->DisableSpotSplitter();
			g_pTool->m_pSpotSplitter->bSectionInputMode = FALSE;
		}

		g_pTool->EnableStartSpotSplitter();
	}
	else
	{
		g_pTool->DisableStartSpotSplitter();
	}
}

void CMy4DyuchiGXMapEditorView::OnUpdateStartspotInput(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if( g_pTool->GetHField() )
	{
		pCmdUI->Enable( TRUE );
		if( g_pTool->m_pStartSplitter->bSectionInputMode )
		{
			pCmdUI->SetCheck( 1 );
		}
		else 
		{
			pCmdUI->SetCheck( 0 );
		}
	}
	else
	{
		pCmdUI->Enable( FALSE );
	}
	
}

void CMy4DyuchiGXMapEditorView::OnSaveTtb() 
{
	char szFilters[] = "TILE Table Files(*.ttb)|*.ttb";
	
	char* filename;
		
	CString pathname;
	CFileDialog dlg(FALSE, NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES,szFilters, NULL); 
	
	if (dlg.DoModal() == IDOK)
	{
		pathname = (LPSTR)(LPCSTR)(dlg.GetPathName());
		if (pathname == "")
			return;
	}

	if (!(filename = (LPSTR)(LPCSTR)pathname))
		return;

	if (!lstrlen(filename))
		return;

	if( g_pTool->m_pSectionSplitter->bSectionInputMode )
	{
		MessageBox( "Section Input Mode를 체크하지 마세요!", "저장실패", MB_ICONSTOP );
	}
	else if( g_pTool->m_pSpotSplitter->bSectionInputMode )
	{
		MessageBox( "MoveSpot Input Mode를 체크하지 마세요!", "저장실패", MB_ICONSTOP );
	}
	else if( g_pTool->m_pStartSplitter->bSectionInputMode )
	{
		MessageBox( "StartSpot Input Mode를 체크하지 마세요!", "저장실패", MB_ICONSTOP );
	}
	else
	{
		g_pTool->ExportTileTableForCorum(filename);
	}
}
