// RightView.cpp : implementation file
//

#include "stdafx.h"
#include "4dyuchigxmapeditor.h"
#include "RightView.h"
#include "helper.h"
#include "tool.h"
#include "DialogTileBrushProperty.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRightView

CRightView*	g_pRightView = NULL;

IMPLEMENT_DYNCREATE(CRightView, CFormView)

void DeleteTileBrush(void* pTileBrush)
{
	delete (CTileBrush*)pTileBrush;
}
void DeleteModelBrush(void* pModelBrush)
{
	delete (C3DBrush*)pModelBrush;
}
CRightView::CRightView()
	: CFormView(CRightView::IDD)
{
	//{{AFX_DATA_INIT(CRightView)
	m_bR = 0;
	m_bG = 0;
	m_bB = 0;
	m_bEnableShadow = FALSE;
	m_bHFieldAlpha = 0;
	m_bSTMAlpha = 0;
	m_btTileBrushToggle = FALSE;
	m_bViewOnlyHFieldAlpha = 0;	
	//}}AFX_DATA_INIT

	m_lbContainerTileBrush.Initialize(MAX_TILE_BRUSHES_NUM,TILE_BRUSH_NAME_LEN,&m_lcTileBrushList,DeleteTileBrush);
	m_lbContainerModelBrush.Initialize(MAX_BRUSHES_NUM,MAX_NAME_LEN,&m_lcBrushList,DeleteModelBrush);


	// Back ground color initialize

	//trustpak
	/*
	m_byBackRed		= 0x00;
	m_byBackGreen	= 0x00;
	m_byBackBlue	= 0xFF;
	*/

	m_byBackRed		= 0x00;
	m_byBackGreen	= 0x00;
	m_byBackBlue	= 0x00;

	////
	
}

CRightView::~CRightView()
{

}

void CRightView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRightView)
	DDX_Control(pDX, IDC_LIST_TILE_BRUSH, m_lcTileBrushList);
	DDX_Control(pDX, IDC_LIST_GXOBJECT, m_lcGXObjects);
	DDX_Control(pDX, IDC_LIST_BRUSH, m_lcBrushList);
	DDX_Text(pDX, IDC_EDIT_SHADE_COLOR_R, m_bR);
	DDV_MinMaxByte(pDX, m_bR, 0, 255);
	DDX_Text(pDX, IDC_EDIT_SHADE_COLOR_G, m_bG);
	DDV_MinMaxByte(pDX, m_bG, 0, 255);
	DDX_Text(pDX, IDC_EDIT_SHADE_COLOR_B, m_bB);
	DDV_MinMaxByte(pDX, m_bB, 0, 255);
	DDX_Check(pDX, IDC_CHECK_ENABLE_SHADOW, m_bEnableShadow);
	DDX_Text(pDX, IDC_EDIT_HFIELD_ALPHA, m_bHFieldAlpha);
	DDV_MinMaxByte(pDX, m_bHFieldAlpha, 0, 255);
	DDX_Text(pDX, IDC_EDIT_STATIC_MODEL_ALPHA, m_bSTMAlpha);
	DDV_MinMaxByte(pDX, m_bSTMAlpha, 0, 255);
	DDX_Check(pDX, IDC_CHECK_TILE_BRUSH_MODE, m_btTileBrushToggle);
	DDX_Text(pDX, IDC_EDIT_VIEW_ONLY_HFIELD_ALPHA, m_bViewOnlyHFieldAlpha);
	DDV_MinMaxByte(pDX, m_bViewOnlyHFieldAlpha, 0, 255);
	//}}AFX_DATA_MAP

	DDX_Text(pDX, IDC_EDIT_BACK_RED, m_byBackRed);
	DDV_MinMaxByte(pDX, m_byBackRed, 0, 255);

	DDX_Text(pDX, IDC_EDIT_BACK_GREEN, m_byBackGreen);
	DDV_MinMaxByte(pDX, m_byBackGreen, 0, 255);

	DDX_Text(pDX, IDC_EDIT_BACK_BLUE, m_byBackBlue);
	DDV_MinMaxByte(pDX, m_byBackBlue, 0, 255);

	UpdateColor();
}


BEGIN_MESSAGE_MAP(CRightView, CFormView)
	//{{AFX_MSG_MAP(CRightView)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_BRUSH, OnButtonDeleteBrush)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_GXOBJECT, OnButtonLoadGxobject)
	ON_BN_CLICKED(IDC_BUTTON_SHADE_START, OnButtonShadeStart)
	ON_BN_CLICKED(IDC_BUTTON_TRANSPARENCY_SET, OnButtonTransparencySet)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_BRUSH, OnButtonCreateBrush)
	ON_BN_CLICKED(IDC_CHECK_TILE_BRUSH_MODE, OnCheckTileBrushMode)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_TILE_BRUSH, OnButtonDeleteTileBrush)
	ON_BN_CLICKED(IDC_BUTTON_DELETEALL_TILE_BRUSH, OnButtonDeleteallTileBrush)
	ON_BN_CLICKED(IDC_SAVE_TILE_BRUSH, OnSaveTileBrush)
	ON_BN_CLICKED(IDC_LOAD_TILE_BRUSH, OnLoadTileBrush)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_BACK_MODIFY, OnButtonBackModify)	
	ON_EN_CHANGE(IDC_EDIT_BACK_BLUE, OnChangeEditBackBlue)
	ON_EN_CHANGE(IDC_EDIT_BACK_GREEN, OnChangeEditBackGreen)
	ON_EN_CHANGE(IDC_EDIT_BACK_RED, OnChangeEditBackRed)
	ON_EN_CHANGE(IDC_EDIT_SHADE_COLOR_B, OnChangeEditShadeColorB)
	ON_EN_CHANGE(IDC_EDIT_SHADE_COLOR_G, OnChangeEditShadeColorG)
	ON_EN_CHANGE(IDC_EDIT_SHADE_COLOR_R, OnChangeEditShadeColorR)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRightView diagnostics

#ifdef _DEBUG
void CRightView::AssertValid() const
{
	CFormView::AssertValid();
}

void CRightView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRightView message handlers

void CRightView::OnButtonLoad() 
{
	// TODO: Add your control notification handler code here
	char szFileName[_MAX_PATH];

	if (!OpenMODFileName(szFileName,_MAX_PATH))
		return;

	C3DBrush*		pBrush = new C3DBrush;
	if (!pBrush->LoadModel(g_pTool->GetGeometry(),g_pTool->GetRenderer(),szFileName))
	{
		delete pBrush;
		return;
	}

	m_lbContainerModelBrush.InsertItem(pBrush->GetName(),pBrush);
}

C3DBrush* CRightView::GetSelectedBrush()
{
	C3DBrush*	pModelBrush = (C3DBrush*)m_lbContainerModelBrush.GetSelectedItem();
	return pModelBrush;
}
CTileBrush* CRightView::GetSelectedTileBrush()
{
	CTileBrush* pTileBrush = (CTileBrush*)m_lbContainerTileBrush.GetSelectedItem();
	return pTileBrush;
}

C3DBrush* CRightView::GetBrush(POINT* ptCursor)
{

	C3DBrush*	pBrush = GetSelectedBrush();

	if (!pBrush)
		return NULL;

	float	fDist;
	DWORD	dwFaceGroupIndex;

	if (0xffffffff == pBrush->IsCollision(&fDist,&dwFaceGroupIndex,ptCursor->x,ptCursor->y,0))
		return NULL;

	return pBrush;
	
}

void CRightView::OnButtonDeleteBrush() 
{
	// TODO: Add your control notification handler code here
	m_lbContainerModelBrush.DeleteSelectedItem();
}


void CRightView::OnButtonLoadGxobject() 
{
	// TODO: Add your control notification handler code here
	char	szFileName[_MAX_PATH];
	if (!OpenMODFCHRFileName(szFileName,_MAX_PATH))
		return;


	GetNameRemovePath(szFileName,szFileName);
	m_lcGXObjects.AddString(szFileName);
}
void CRightView::LoadGXObject(char* szFileName)
{
	if (LB_ERR == m_lcGXObjects.SelectString(0,szFileName))
		m_lcGXObjects.AddString(szFileName);	
}
BOOL CRightView::GetGXObjectName(char* szFileName)
{
	int iIndex = m_lcGXObjects.GetCurSel();
	if (iIndex == LB_ERR)
		return FALSE;
	
	m_lcGXObjects.GetText(iIndex,szFileName);
	return TRUE;

}



void CRightView::OnButtonShadeStart() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	DWORD	dwFlag = 0;
	if (m_bEnableShadow)
		dwFlag |= STATIC_MODEL_SHADE_TYPE_ENABLE_SHADOW;
	
	DWORD	dwAmbientColor = 0xff000000 | ((DWORD)m_bR<<16) | ((DWORD)m_bG<<8) | m_bB;
	g_pTool->ShadeLightMap(dwAmbientColor,dwFlag);
	
}

void CRightView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	m_bR = 0x0f;
	m_bG = 0x0f;
	m_bB = 0x0f;
	m_bHFieldAlpha = (BYTE)g_pTool->GetAlphaHField();
	m_bViewOnlyHFieldAlpha = (BYTE)g_pTool->GetAlphaViewOnlyHField();
	m_bSTMAlpha = (BYTE)g_pTool->GetAlphaStaticModel();


	// Set back ground color;

	g_pTool->SetBackGroundColor(m_byBackRed,
								m_byBackGreen,
								m_byBackBlue);


	UpdateData(FALSE);


	UpdateColor();

	// TODO: Add your specialized code here and/or call the base class
	
}

void CRightView::OnButtonTransparencySet() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	g_pTool->SetAlphaHField(m_bHFieldAlpha);
	g_pTool->SetAlphaStaticModel(m_bSTMAlpha);
	g_pTool->SetAlphaViewOnlyHField(m_bViewOnlyHFieldAlpha);
	g_pTool->Render();

}

void CRightView::OnButtonCreateBrush() 
{
	// TODO: Add your control notification handler code here
	HFIELD_POS*		pTileList;
	DWORD dwSelectedTileNum = g_pTool->GetSelectedTileList(&pTileList);
	if (!dwSelectedTileNum)
		return;


	HFIELD_DESC	hfDesc;
	g_pTool->GetHField()->GetHFieldDesc(&hfDesc);

	TILE_BRUSH_PROPERTY	property;

	CDialogTileBrushProperty	dlgProperty;
	dlgProperty.SetTileBrushProperty(&property);
	dlgProperty.DoModal();	

	CTileBrush*	pTileBrush = new CTileBrush;

	pTileBrush->CreateBrush(property.szName,pTileList,dwSelectedTileNum,hfDesc.pwTileTable,hfDesc.dwTileNumX);
	InsertTileBrush(pTileBrush);
}
BOOL CRightView::InsertTileBrush(CTileBrush* pTileBrush)
{
	return m_lbContainerTileBrush.InsertItem(pTileBrush->GetName(),pTileBrush);

}
void CRightView::ToggleTileBrushMode()
{
	if (m_btTileBrushToggle)
		m_btTileBrushToggle = FALSE;
	else
		m_btTileBrushToggle = TRUE;

	UpdateData(FALSE);

	OnCheckTileBrushMode();
}

void CRightView::OnCheckTileBrushMode() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_btTileBrushToggle)
		g_pTool->SetTileBrushMode(TRUE);
	else
		g_pTool->SetTileBrushMode(FALSE);
}


void CRightView::OnButtonDeleteTileBrush() 
{
	// TODO: Add your control notification handler code here
	m_lbContainerTileBrush.DeleteSelectedItem();
}

void CRightView::OnButtonDeleteallTileBrush() 
{
	// TODO: Add your control notification handler code here
	m_lbContainerTileBrush.DeleteAll();
}

void CRightView::OnSaveTileBrush() 
{

	if (!m_lbContainerTileBrush.GetItemNum())
		return;

	char szFilters[] = "TileBrush Files (*.tbr) | *.tbr | All files (*.*) |*.*||";
	char* filename;
		
	CString pathname;
	CFileDialog dlg(FALSE,"tbr","*.tbr",NULL,szFilters,AfxGetMainWnd());

		
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

	SaveTileBrushes(filename);

}
BOOL CRightView::SaveTileBrushes(char* szFileName)
{
	FILE*	fp = fopen(szFileName,"wb");
	if (!fp)
		return FALSE;

	DWORD dwTileBrushNum = m_lbContainerTileBrush.GetItemNum();
	fwrite(&dwTileBrushNum,sizeof(DWORD),1,fp);

	for (int i=0; i<dwTileBrushNum; i++)
	{
		CTileBrush*	pTileBrush = (CTileBrush*)m_lbContainerTileBrush.GetItem(i);
		pTileBrush->Save(fp);
	}
	
	fclose(fp);
	return TRUE;
}
BOOL CRightView::LoadTileBrushes(char* szFileName)
{


	FILE*	fp = fopen(szFileName,"rb");
	if (!fp)
		return FALSE;

	DWORD	dwTileBrushNum;
	fread(&dwTileBrushNum,sizeof(DWORD),1,fp);
	for (DWORD i=0; i<dwTileBrushNum; i++)
	{
		CTileBrush*	pTileBrush = new CTileBrush;
		if (pTileBrush->Load(fp))
		{
			InsertTileBrush(pTileBrush);
		}
		else
			delete pTileBrush;
	}
	
	fclose(fp);
	return TRUE;
}

void CRightView::OnLoadTileBrush() 
{
	// TODO: Add your control notification handler code here
	char szFilters[] = "Tile Brush Files (*.tbr) | *.tbr | All files (*.*) |*.*||";
	char* filename;
		
	CString pathname;
	CFileDialog dlg(TRUE,"tbr","*.tbr",NULL,szFilters,AfxGetMainWnd());

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

	LoadTileBrushes(filename);

}

void CRightView::OnDestroy() 
{
	m_lbContainerTileBrush.DeleteAll();
	m_lbContainerModelBrush.DeleteAll();

	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CRightView::OnButtonBackModify() 
{
	UpdateData(TRUE);

	g_pTool->SetBackGroundColor(m_byBackRed,
								m_byBackGreen,
								m_byBackBlue);
}

void CRightView::OnChangeEditBackBlue() 
{
	OnButtonBackModify();
}

void CRightView::OnChangeEditBackGreen() 
{
	OnButtonBackModify();
}

void CRightView::OnChangeEditBackRed() 
{
	OnButtonBackModify();	
}


//trustpak
void CRightView::UpdateColor(void)
{
	HWND hColorBox = ::GetDlgItem(m_hWnd,IDC_STATIC_SHADING_COLOR);
	RECT	rect;
	::GetWindowRect(hColorBox,&rect);

	POINT	p;
	p.x = 0;
	p.y = 0;
	::ScreenToClient(m_hWnd,&p);

	rect.left += p.x;
	rect.right += p.x;
	rect.top += p.y;
	rect.bottom += p.y;

	HDC hDC = ::GetDC(m_hWnd);
	DWORD	dwColor = (m_bB<<16) | (m_bG<<8) | m_bR;
	
	HBRUSH	hBrush = CreateSolidBrush(dwColor);
	HBRUSH	hOldBrush = (HBRUSH)SelectObject(hDC,hBrush);

	FillRect(hDC,&rect,hBrush);

	
	SelectObject(hDC,hOldBrush);
	DeleteObject(hBrush);

	::ReleaseDC(m_hWnd,hDC);

}

///

void CRightView::OnChangeEditShadeColorB() 
{
	UpdateData(TRUE);

	//UpdateColor();
}

void CRightView::OnChangeEditShadeColorG() 
{
	UpdateData(TRUE);

	//UpdateColor();
}

void CRightView::OnChangeEditShadeColorR() 
{
	UpdateData(TRUE);

	//UpdateColor();
}

void CRightView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	HWND hColorBox = ::GetDlgItem(m_hWnd,IDC_STATIC_SHADING_COLOR);
	RECT	rect;
	::GetWindowRect(hColorBox,&rect);

	POINT	p;
	p.x = 0;
	p.y = 0;
	::ScreenToClient(m_hWnd,&p);

	rect.left += p.x;
	rect.right += p.x;
	rect.top += p.y;
	rect.bottom += p.y;


	if (point.x < rect.left)
		return;

	if (point.x > rect.right)
		return;

	if (point.y < rect.top)
		return;

	if (point.y > rect.bottom)
		return;


	DWORD	dwColor = (m_bB<<16) | (m_bG<<8) | m_bR;

	CColorDialog	colorDlg(dwColor,CC_FULLOPEN,NULL);
	colorDlg.DoModal();

	dwColor = colorDlg.GetColor();
	
	m_bR = dwColor & 0x000000ff;
	m_bG = (dwColor & 0x0000ff00)>>8;
	m_bB = (dwColor & 0x00ff0000)>>16;

	UpdateData(FALSE);	
	
	UpdateColor();	
	CFormView::OnLButtonDblClk(nFlags, point);
}
