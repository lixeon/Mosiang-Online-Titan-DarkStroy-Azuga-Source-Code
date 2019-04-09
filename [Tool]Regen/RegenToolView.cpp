// RegenToolView.cpp : implementation of the CRegenToolView class
//

#include "stdafx.h"
#include "RegenTool.h"

#include "RegenToolDoc.h"
#include "RegenToolView.h"
#include "MonsterListViewDlg.h"
#include "LoadList.h"
#include "./Engine/GraphicEngine.h"

#include <fcntl.h>
#include <io.h>
#include "MHMap.h"
#include "../4DyuchiGRX_myself97_util/MToolMouseStatus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#pragma comment(lib, "../4DyuchiGXGFunc/SS3DGFunc.lib")

MToolMouseStatus*	g_pMouse = NULL;
DWORD	g_dwFrameCount = 0;
BOOL g_AllReady = FALSE;
HMODULE        g_hExecutiveHandle;
/////////////////////////////////////////////////////////////////////////////
// CRegenToolView


IMPLEMENT_DYNCREATE(CRegenToolView, CFormView)

BEGIN_MESSAGE_MAP(CRegenToolView, CFormView)
	//{{AFX_MSG_MAP(CRegenToolView)
	ON_WM_DRAWITEM()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_MAPIMAGE, OnMapimage)
	ON_CBN_SELCHANGE(IDC_NPCCOMBO, OnSelchangeNpccombo)
	ON_CBN_SELCHANGE(IDC_MAPCOMBO, OnSelchangeMapcombo)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_SELECT_CHK, OnSelectChk)
	ON_BN_CLICKED(IDC_MSELECTNOAREA_CHK, OnMselectnoareaChk)
	ON_BN_CLICKED(IDC_MSELECTAREA_CHK, OnMselectareaChk)
	ON_BN_CLICKED(IDC_DELETE_CHK, OnDeleteChk)
	ON_BN_CLICKED(IDC_ZOOMIN_CHK, OnZoominChk)
	ON_BN_CLICKED(IDC_ZOOMOUT_CHK, OnZoomoutChk)
	ON_BN_CLICKED(IDC_MOVE_CHK, OnMoveChk)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_CBN_SELCHANGE(IDC_FUNCCOMBO, OnSelchangeFunccombo)
	ON_BN_CLICKED(IDC_CONDITIONALLOK, OnConditionallok)
	ON_BN_CLICKED(IDC_LISTVIEW, OnListview)
	ON_BN_CLICKED(IDC_ADDCONDITION, OnAddcondition)
	ON_CBN_SELCHANGE(IDC_MONCOMBO, OnSelchangeMoncombo)
	ON_EN_KILLFOCUS(IDC_GROUPEDIT, OnKillfocusGroupedit)
	ON_BN_CLICKED(IDC_GRADE1, OnGrade1)
	ON_BN_CLICKED(IDC_GRADE2, OnGrade2)
	ON_BN_CLICKED(IDC_GRADE3, OnGrade3)
	ON_BN_CLICKED(IDC_LOADMAPBTN, OnLoadmapbtn)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegenToolView construction/destruction

CRegenToolView::CRegenToolView()
	: CFormView(CRegenToolView::IDD)
{
	//{{AFX_DATA_INIT(CRegenToolView)
	m_MapNameStr = _T("");
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	SetStartSelectFlag(FALSE);
	m_ViewWinSize.SetRectEmpty();
	m_ImageSize.SetRectEmpty();
	m_ListIndex = 0;
	m_bDown = FALSE;
	m_SelectRect = CRect(0, 0, 0, 0);
	m_OldRect = CRect(0, 0, 0, 0);
	m_DataInsertFlag = FALSE;
	m_DataInsertKind = 0;
//	m_szFuncKind = new CString[MAX_FUNCKIND_NUM];
	m_szFuncKind[0] = FDEFAULT;
	m_szFuncKind[1] = FMONSTER_REGEN;
	m_szFuncKind[2] = FNPC_REGEN;
	m_szFuncKind[3] = FLOGIN_POINT;
	m_szFuncKind[4] = FCHANGEMAP_POINT;
	m_szFuncKind[5] = FMAP_ATTRIBUTE;
//	m_ViewTile = new CTile[TILE_MAX_WIDTH * TILE_MAX_HEIGHT ];

	// MonsterIndex 초기화
	m_dwMonsterIndex = START_MONSTERINDEX;
	m_ListViewDlg = NULL;
	m_ConditionAllCheckFlag = FALSE;
	g_pMouse = NULL;
}

CRegenToolView::~CRegenToolView()
{
	
//	CWnd * wnd = GetDlgItem(IDC_MAPIMAGE);
//	::ReleaseDC(wnd->m_hWnd, m_hdc);
}

void CRegenToolView::InitTile()
{

}
void CRegenToolView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegenToolView)
	DDX_Control(pDX, IDC_GRADE1, m_GradeRadio);
	DDX_Control(pDX, IDC_ADDCONDITION, m_AddConditionBtn);
	DDX_Control(pDX, IDC_SUBCOMBOTEXT, m_SubComboStatic);
	DDX_Control(pDX, IDC_TARGETGROUPIDSTATIC, m_TargetGroupIDStatic);
	DDX_Control(pDX, IDC_TARGETGROUPIDSPIN, m_TargetGroupIDSpin);
	DDX_Control(pDX, IDC_TARGETGROUPIDEDIT, m_TargetGroupIDEdit);
	DDX_Control(pDX, IDC_REGENTIMESTATIC, m_RegenTimeStatic);
	DDX_Control(pDX, IDC_REGENTIMESPIN, m_RegenTimeSpin);
	DDX_Control(pDX, IDC_REGENTIMEEDIT, m_RegenTimeEdit);
	DDX_Control(pDX, IDC_LISTVIEW, m_ListViewBtn);
	DDX_Control(pDX, IDC_GROUPSTATIC, m_GroupStatic);
	DDX_Control(pDX, IDC_GROUPSPIN, m_GroupSpin);
	DDX_Control(pDX, IDC_GROUPPROPERTYSTATIC, m_GroupPropertyStatic);
	DDX_Control(pDX, IDC_GROUPPROPERTYSPIN, m_GroupPropertySpin);
	DDX_Control(pDX, IDC_GROUPPROPERTYEDIT, m_GroupPropertyEdit);
	DDX_Control(pDX, IDC_GROUPNAMESTATIC, m_GroupNameStatic);
	DDX_Control(pDX, IDC_GROUPNAMEEDIT, m_GroupNameEdit);
	DDX_Control(pDX, IDC_GROUPEDIT, m_GroupEdit);
	DDX_Control(pDX, IDC_CONDITIONSTATIC, m_ConditionStatic);
	DDX_Control(pDX, IDC_CONDITIONSPIN, m_ConditionSpin);
	DDX_Control(pDX, IDC_CONDITIONEDIT, m_ConditionEdit);
	DDX_Control(pDX, IDC_CONDITIONALLOK, m_ConditionAllOKBtn);
	DDX_Control(pDX, IDC_ALLCONDITIONGROUP, m_AllConditionGroupBox);
	DDX_Control(pDX, IDC_CONDITION2GROUP, m_Condition2GroupBox);
	DDX_Control(pDX, IDC_CONDITION1GROUP, m_Condition1GroupBox);
	DDX_Control(pDX, IDC_MONCOMBO, m_MonsterKind);
	DDX_Control(pDX, IDC_FUNCCOMBO, m_FuncKind);
	DDX_Control(pDX, IDC_SCROLLBAR2, m_ImageHScroll);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_ImageVScroll);
	DDX_Control(pDX, IDC_LIST2, m_MListCtrl);
	DDX_Control(pDX, IDC_MAPCOMBO, m_MapKind);
	DDX_Control(pDX, IDC_NPCCOMBO, m_NpcKind);
	DDX_Control(pDX, IDC_ZOOMOUT_CHK, m_ZoomOutBtn);
	DDX_Control(pDX, IDC_ZOOMIN_CHK, m_ZoomInBtn);
	DDX_Control(pDX, IDC_SELECT_CHK, m_SelectBtn);
	DDX_Control(pDX, IDC_MSELECTNOAREA_CHK, m_MSelectNoAreaBtn);
	DDX_Control(pDX, IDC_MSELECTAREA_CHK, m_MSelectAreaBtn);
	DDX_Control(pDX, IDC_MOVE_CHK, m_MoveBtn);
	DDX_Control(pDX, IDC_DELETE_CHK, m_DeleteBtn);
	DDX_Control(pDX, IDC_MAPIMAGE, m_MapImage);
	DDX_Text(pDX, IDC_MAP_NAME, m_MapNameStr);
	//}}AFX_DATA_MAP
}

BOOL CRegenToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	
	
	return CFormView::PreCreateWindow(cs);
}

void CRegenToolView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
		
	CWnd * wnd = GetDlgItem(IDC_MAPIMAGE);
	m_hdc = ::GetDC(wnd->m_hWnd);
	GetClientRect(&m_OriginRect);
	
	m_VScrollPos = 0;
	m_HScrollPos = 0;

	m_StartImagePos.x = 0;
	m_StartImagePos.y = 0;

	g_pLoadList = new CLoadList;
	SetStartSelectFlag(FALSE);
	
	// MonsterList Loading
//	g_pLoadList->LoadMapList(g_pLoadList->m_MapList);
	g_pLoadList->LoadMonsterList();
	
	// Select 초기화
	m_rectTrack = CMyTracker(this,2,5);
	m_rect.SetRectEmpty();
	m_rectTrack.SetRect(m_rect);
	
	
	m_SelectXY = CSelectXY(wnd);
	
	
	// Bitmap Button Image Load ----------------------------------------
	m_MonsterSelectAreaBitmap_on.LoadBitmap(IDB_MONSTERSELECTAREA_ON);
	m_MonsterSelectAreaBitmap_press.LoadBitmap(IDB_MONSTERSELECTAREA_PRESS);
	m_MonsterSelectNoAreaBitmap_on.LoadBitmap(IDB_MONSTERSELECTNOAREA_ON);
	m_MonsterSelectNoAreaBitmap_press.LoadBitmap(IDB_MONSTERSELECTNOAREA_PRESS);
	m_MoveBitmap_on.LoadBitmap(IDB_MOVE_ON);
	m_MoveBitmap_press.LoadBitmap(IDB_MOVE_PRESS);
	m_ZoomOutBitmap_on.LoadBitmap(IDB_ZOOMOUT_ON);
	m_ZoomOutBitmap_press.LoadBitmap(IDB_ZOOMOUT_PRESS);
	m_ZoomInBitmap_on.LoadBitmap(IDB_ZOOMIN_ON);
	m_ZoomInBitmap_press.LoadBitmap(IDB_ZOOMIN_PRESS);
	m_SelectBitmap_on.LoadBitmap(IDB_SELECT_ON);
	m_SelectBitmap_press.LoadBitmap(IDB_SELECT_PRESS);
	m_DeleteBitmap_on.LoadBitmap(IDB_DELETE_ON);
	m_DeleteBitmap_press.LoadBitmap(IDB_DELETE_PRESS);

	for(int btn = 0; btn < MAX_BUTTON; btn++)
	{
		m_ButtonFlag[btn] = FALSE;
	}

	//------------------------------------------------------------------
	
	// MonsterKind Combo Setting  ------------------------------------------
	for(int m = 0; m < MAX_MONSTER_NUM; m++)
	{
		m_MonsterKind.AddString(g_pLoadList->m_MonsterList[m].Name);
	}
	m_MonsterKind.SetCurSel(0);
	m_MonsterKind.ShowWindow(SW_HIDE);
	strcpy(m_MonsterChxName,g_pLoadList->m_MonsterList[0].ChxName);
	
	m_SubComboStatic.ShowWindow(SW_HIDE);
	UpdateData(FALSE);
	//------------------------------------------------------------------
	
	// NPCKind Combo Setting  ------------------------------------------
	for(int nm = 0; nm < MAX_MONSTER_NUM; nm++)
	{
		m_NpcKind.AddString(g_pLoadList->m_MonsterList[nm].Name);
	}
	m_NpcKind.SetCurSel(0);
	m_NpcKind.ShowWindow(SW_HIDE);
	UpdateData(FALSE);
	//------------------------------------------------------------------

	// Monster Grade Setting -------------------------------------------
	m_GradeRadio.SetCheck(1);
	m_MonsterGrade = BASIC_MONSTER;
	//------------------------------------------------------------------

	// MapName Combo Setting -------------------------------------------
/*	for(int ml = 0; ml < MAX_MAP_NUM; ml++)
	{
		m_MapKind.AddString(g_pLoadList->m_MapList[ml].LocalName);
	}
	m_MapKind.SetCurSel(0);
	MAP_LIST maplist = g_pLoadList->m_MapList[0];

	m_MapNameStr.Format(maplist.LocalName);
	UpdateData(FALSE);
*/	//------------------------------------------------------------------

	
	// Func_Kind Combo Setting  ------------------------------------------
	for(int f = 0; f < MAX_FUNCKIND_NUM; f++)
	{
		m_FuncKind.AddString(m_szFuncKind[f]);
	}
	m_FuncKind.SetCurSel(0);

	m_SubComboStatic.SetWindowText(TDEFAULT);
	//------------------------------------------------------------------

	// NpcList EditBox Setting  ------------------------------------------
	CRect rect;
	m_MListCtrl.GetClientRect(&rect);
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_CENTER;
	for(int i = 0; i < 5; i++)
	{
		lvcolumn.iSubItem = i;
		switch(i)
		{
		case 0://첫번째 컬럼
			lvcolumn.cx = 35;
			lvcolumn.pszText = "Group";
			break;
		case 1://두번째 컬럼
			lvcolumn.cx = 68;
			lvcolumn.pszText = "MonIdx";
			break;
		case 2://세번째 컬럼
			lvcolumn.cx = 50;
			lvcolumn.pszText = "Kind";
			break;
		case 3://네번째 컬럼
			lvcolumn.cx = 50;
			lvcolumn.pszText = "X";
			break;
		case 4://다섯번째 컬럼
			lvcolumn.cx = 40;
			lvcolumn.pszText = "Z";
			break;
		case 5://여섯번째 컬럼
			lvcolumn.cx = rect.Width()-243;
			lvcolumn.pszText = "Flag";
			break;
		}
		m_MListCtrl.InsertColumn(i, &lvcolumn); // 컬럼추가
	}
	//------------------------------------------------------------------
	
	// Function Button Setting -----------------------------------------
	m_SelectBtn.SetCheck(FALSE);
	m_MSelectNoAreaBtn.SetCheck(FALSE);
	m_MSelectAreaBtn.SetCheck(FALSE);
	m_DeleteBtn.SetCheck(FALSE);
	m_ZoomInBtn.SetCheck(FALSE);
	m_ZoomOutBtn.SetCheck(FALSE);
	m_MoveBtn.SetCheck(FALSE);
	//------------------------------------------------------------------
//	m_LoadMap.LoadBitmap(IDB_NANJU);
	//BMPINFO 정보를 설정한다.
/*	szBitmapInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	szBitmapInfo.bmiHeader.biWidth=256;
	szBitmapInfo.bmiHeader.biHeight=256;
	szBitmapInfo.bmiHeader.biPlanes=1;
	szBitmapInfo.bmiHeader.biBitCount=24;
	szBitmapInfo.bmiHeader.biCompression=BI_RGB;
	szBitmapInfo.bmiHeader.biSizeImage=256*256;
	szBitmapInfo.bmiHeader.biXPelsPerMeter=0;
	szBitmapInfo.bmiHeader.biYPelsPerMeter=0;
	szBitmapInfo.bmiHeader.biClrUsed =0;
	szBitmapInfo.bmiHeader.biClrImportant =0;
*/
	// SpinControl Setting -----------------------------------------------
		m_GroupSpin.SetRange(1, 200);
		m_GroupPropertySpin.SetRange(0, 200);
		m_TargetGroupIDSpin.SetRange(0, 200);
		m_ConditionSpin.SetRange(-100, 100);
		m_RegenTimeSpin.SetRange(0, 200);

		m_GroupEdit.SetWindowText("1");
		m_GroupNameEdit.SetWindowText("일반그룹_1");
		m_GroupPropertyEdit.SetWindowText("0");
		CString temp;
		m_GroupEdit.GetWindowText(temp);
		m_TargetGroupIDEdit.SetWindowText(temp);
		m_ConditionEdit.SetWindowText("0.5");
		m_RegenTimeEdit.SetWindowText("15000");
	//-------------------------------------------------------------
	// Control Hide -----------------------------------------------
		m_Condition1GroupBox.ShowWindow(SW_SHOW);
		m_GroupStatic.ShowWindow(SW_SHOW);
		m_GroupEdit.ShowWindow(SW_SHOW);
		m_GroupSpin.ShowWindow(SW_SHOW);
		m_GroupNameStatic.ShowWindow(SW_SHOW);
		m_GroupNameEdit.ShowWindow(SW_SHOW);
		m_GroupPropertyStatic.ShowWindow(SW_SHOW);
		m_GroupPropertyEdit.ShowWindow(SW_SHOW);
		m_GroupPropertySpin.ShowWindow(SW_SHOW);

		m_Condition2GroupBox.ShowWindow(SW_SHOW);
		m_TargetGroupIDStatic.ShowWindow(SW_SHOW);
		m_TargetGroupIDEdit.ShowWindow(SW_SHOW);
		m_TargetGroupIDSpin.ShowWindow(SW_SHOW);
		m_ConditionStatic.ShowWindow(SW_SHOW);
		m_ConditionEdit.ShowWindow(SW_SHOW);
		m_ConditionSpin.ShowWindow(SW_SHOW);
		m_RegenTimeStatic.ShowWindow(SW_SHOW);
		m_RegenTimeEdit.ShowWindow(SW_SHOW);
		m_RegenTimeSpin.ShowWindow(SW_SHOW);

		m_AllConditionGroupBox.ShowWindow(SW_SHOW);
		m_ConditionAllOKBtn.ShowWindow(SW_SHOW);
		
		m_ListViewBtn.ShowWindow(SW_SHOW);
	//-------------------------------------------------------------
	g_AllReady = TRUE;
	 
}

/////////////////////////////////////////////////////////////////////////////
// CRegenToolView printing

BOOL CRegenToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRegenToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRegenToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
	
}

void CRegenToolView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CRegenToolView diagnostics

#ifdef _DEBUG
void CRegenToolView::AssertValid() const
{
	CFormView::AssertValid();
}

void CRegenToolView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CRegenToolDoc* CRegenToolView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRegenToolDoc)));
	return (CRegenToolDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRegenToolView message handlers


void CRegenToolView::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{

	if(nIDCtl==IDC_MAPIMAGE)
	{
/*		CRegenToolDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		CDC cdc;
		::SetStretchBltMode(lpDrawItemStruct->hDC, COLORONCOLOR);
		
		::SetDIBitsToDevice(lpDrawItemStruct->hDC,                    
			0,             // DestX
			0,              // DestY
			256,        // nDestWidth
			256,       // nDestHeight
			0,            // SrcX
			0,   // SrcY
			0,                          // nStartScan
			(WORD)256,  // nNumScans
			m_MapImageCtl,                  // lpBit
			&szBitmapInfo,     // lpBitsInfo
			DIB_RGB_COLORS);            // wUsage
*/		//현재 DC와 호환성이 있는 DC로 설정
/*		cdc.CreateCompatibleDC(&WinDC);
		//이 DC에 비트맵을 설정한다.
		cdc.SelectObject(&m_LoadMap);
		//cdc를 화면 DC pDC로 출력
		WinDC.BitBlt(0,0,275,407,&cdc,0,0,SRCCOPY);
*/		
		//버튼 눌려짐
		if(lpDrawItemStruct->itemState & ODS_SELECTED)
		{

		}
		else
		{

		}
	}
	CFormView::OnDrawItem(nIDCtl, lpDrawItemStruct);
	
}

void CRegenToolView::BMPButtonImageSetting(char *filename, LPSTR button)
{

}

void CRegenToolView::OnDraw(CDC* pDC) 
{
	// TODO: Add your specialized code here and/or call the base class
	CRegenToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CDC cdc;
/*
	// Map Image 출력 --------------------------------------------
	BitBlt(m_hdc,0,0,m_ImageBoxRect.right,m_ImageBoxRect.bottom,m_hMemDC,m_StartImagePos.x, m_StartImagePos.y, SRCCOPY);
	//------------------------------------------------------------
*/	
	// Bitmap Button 출력 ----------------------------------------
	m_ZoomOutBtn.SetBitmap(m_ZoomOutBitmap_on);
	m_ZoomInBtn.SetBitmap(m_ZoomInBitmap_on);
	m_SelectBtn.SetBitmap(m_SelectBitmap_on);
	m_MoveBtn.SetBitmap(m_MoveBitmap_on);
	m_MSelectNoAreaBtn.SetBitmap(m_MonsterSelectNoAreaBitmap_on);
	m_MSelectAreaBtn.SetBitmap(m_MonsterSelectAreaBitmap_on);
	m_DeleteBtn.SetBitmap(m_DeleteBitmap_on);
	//------------------------------------------------------------
	
//	if(GetStartSelectFlag())
	{
/*		LISTMONSTER::iterator it;
		for(it = pDoc->GetMonsterList().begin(); it != pDoc->GetMonsterList().end(); ++it)
		{
			
			MONSTER_POINT* mp = (MONSTER_POINT*)*it;
			CPoint tmppos = mp->point;
			tmppos.x -= m_HScrollPos;		// 스크롤 값을 뺀다.
			tmppos.y -= m_VScrollPos;
			m_SelectXY.Draw(pDC, tmppos);		
		}
*/		
	
	}
	
//	m_rectTrack.Draw(pDC);
}

void CRegenToolView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CRegenToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: Add your message handler code here and/or call default
	//추적박스의 OnLButtonUP호출
/*	m_rectTrack.OnLButtonUp(nFlags,point);
	//프로세서 0으로 설정
	m_nProcess=0;
	//추적박스 영역을 m_rect에 저장
	m_rect=m_rectTrack.m_rectTrack;
	m_rectTrack.SetRect(m_rect);
	//프로세서 설정
	m_rectTrack.SetProcess(m_nProcess);
*/	//화면 재출력
	m_bDown = FALSE;
	CString sztemp;
	if(GetStartSelectFlag())
	{
		if(m_SelectBtn.GetCheck()) // 선택버튼 체크
		{
			if(m_ConditionAllCheckFlag)
			{
				//m_SelectXY.OnLButtonUp(nFlags, point);
				
				POINT* ViewMousePos = CalcSelectPoint(point);
				if(!ViewMousePos)
					return;
				VECTOR3* TargetPos = GetPickedPosition(ViewMousePos->x,ViewMousePos->y);
				if(!TargetPos)
					return;
				if(MAP->CollisonCheck(TargetPos, TargetPos, TargetPos))
					return;
				m_GroupEdit.GetWindowText(sztemp);

				if(pDoc->AddMonsterCheck(sztemp))
					InsertMonster(TargetPos);		
				else
				{
					AfxMessageBox("그룹이 존재하지 않습니다. 등록먼저 해주세염!!!");
				}
			}
			else
			{
				AfxMessageBox("조건을 먼저 설정해 주세염!!!");
			}
			
		}
		else if(m_DeleteBtn.GetCheck())
		{
			if(point.x == m_OldRect.left && point.y == m_OldRect.top)
			{
				POINT* ViewMousePos = CalcSelectPoint(point);
				if(!ViewMousePos)
					return;
				VECTOR3* TargetPos = GetPickedPosition(ViewMousePos->x,ViewMousePos->y);
				if(!TargetPos)
					return;
				
				static VECTOR3 pos;
				static float dist;
				static POINT pt;
				pt.x = ViewMousePos->x;
				pt.y = ViewMousePos->y;
				DWORD modelindex,objectindex;
				GXOBJECT_HANDLE handle = 
					g_pExecutive->GXOGetObjectWithScreenCoord(&pos,&modelindex,&objectindex,&dist,&pt,0,0);
				if(handle == NULL)
					return;
				
				MONSTER_POINT* mp = pDoc->FindSelectMonsterHandle(handle);
				if(!mp)
					return;
				
				int MaxCount = m_MListCtrl.GetItemCount();
				int MinCount = 0;
				for(int i = MinCount; i < MaxCount; i++)
				{
					char OriIndex[16];
					memset((void*)OriIndex, 0, sizeof(16));
					itoa((int)mp->SelectCount, OriIndex, 10);
					//				strcpy(FindIndex, OriIndex);
					CString FindIndex = m_MListCtrl.GetItemText(i, 4);
					//				m_MListCtrl.GetItem(&pItem);
					if(!strcmp(FindIndex, OriIndex))// 리스트컨트롤에서 삭제
					{
						m_MListCtrl.DeleteItem(i);
						
					}
				}
				if(GetMonsterIndex()==START_MONSTERINDEX)
					return;
//				ReleaseMonsterIndex();
//				DWORD idx = GetMonsterIndex();
				CString szTemp;
				szTemp.Format("%d", mp->MonsterGroup);
				pDoc->ReleaseGroupMonsterCount(szTemp);
				pDoc->SubCount();
				// 맵에서 이미지 삭제
				g_pExecutive->DeleteGXObject(mp->MHandle);
				// 리스트에서 삭제
				pDoc->DeleteSelectMonsterPoint((void*)mp);
				
				
			}
			else
			{
				CRect SearchRect;
				if(m_SelectRect.left < point.x)
				{
					SearchRect.left = m_SelectRect.left;
					SearchRect.right = point.x;
				}
				else
				{
					SearchRect.left = point.x;
					SearchRect.right = m_SelectRect.left;
				}
				if(m_SelectRect.top < point.y)
				{
					SearchRect.top = m_SelectRect.top;
					SearchRect.bottom = point.y;
				}
				else
				{
					SearchRect.top = point.y;
					SearchRect.bottom = m_SelectRect.top;
				}
					

				for(int i = SearchRect.left; i < SearchRect.right; i=i+10)
				{
					for(int j = SearchRect.top; j < SearchRect.bottom; j=j+10)
					{
						CPoint tmppos;
						tmppos.x = i;
						tmppos.y = j;
						POINT* ViewMousePos = CalcSelectPoint(tmppos);
						if(!ViewMousePos)
							continue;
						VECTOR3* TargetPos = GetPickedPosition(ViewMousePos->x,ViewMousePos->y);
						if(!TargetPos)
							continue;
						
						static VECTOR3 pos;
						static float dist;
						static POINT pt;
						pt.x = ViewMousePos->x;
						pt.y = ViewMousePos->y;
						GXOBJECT_HANDLE handle = g_pExecutive->GXOGetObjectWithScreenCoord(&pos,0,0,&dist,&pt,0,0);
						if(handle == NULL)
							continue;
						
						MONSTER_POINT* mp = pDoc->FindSelectMonsterHandle(handle);
						if(!mp)
							continue;
						
						int MaxCount = m_MListCtrl.GetItemCount();
						int MinCount = 0;
						for(int i = MinCount; i < MaxCount; i++)
						{
							char OriIndex[16];
							memset((void*)OriIndex, 0, sizeof(16));
							itoa((int)mp->SelectCount, OriIndex, 10);
							//				strcpy(FindIndex, OriIndex);
							CString FindIndex = m_MListCtrl.GetItemText(i, 4);
							//				m_MListCtrl.GetItem(&pItem);
							if(!strcmp(FindIndex, OriIndex))
								// 리스트컨트롤에서 삭제
								m_MListCtrl.DeleteItem(i);
						}
						if(GetMonsterIndex()==START_MONSTERINDEX)
							return;
//						ReleaseMonsterIndex();
//						DWORD idx = GetMonsterIndex();
						CString szTemp;
						szTemp.Format("%d", mp->MonsterGroup);
						pDoc->ReleaseGroupMonsterCount(szTemp);
						pDoc->SubCount();
						// 맵에서 이미지 삭제
						g_pExecutive->DeleteGXObject(mp->MHandle);
						// 리스트에서 삭제
						pDoc->DeleteSelectMonsterPoint((void*)mp);
					}
				}
			}
		}
		else
		{
			MessageBox("오른쪽 아래에 위치한\r\n기능키를 선택해 주세염!! ^o^",
				"작업 하려면", MB_ICONINFORMATION|MB_OK);
		}
	}
	else
	{
		MessageBox("맵파일이 있어야 작업을 하지!!(버럭~) \r\nOpen을 누르고 맵을 열어주세염^o^",
			"맵파일 여는건 기본, 기본에 충실~~", MB_ICONINFORMATION|MB_OK);
	}
	
	CFormView::OnLButtonUp(nFlags, point);
}

void CRegenToolView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	m_rectTrack.OnLButtonDown(nFlags,point);
//	if(GetStartSelectFlag())
	{
		m_bDown = TRUE;
		m_SelectRect = CRect(point.x, point.y, point.x, point.y);
		m_OldRect = m_SelectRect;
	}
	CFormView::OnLButtonDown(nFlags, point);
}

void CRegenToolView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFormView::OnRButtonDown(nFlags, point);
}

void CRegenToolView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFormView::OnRButtonUp(nFlags, point);
}

void CRegenToolView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	m_rectTrack.OnMouseMove(nFlags,point);
//	if(GetStartSelectFlag())
	{
		if(m_bDown)
		{
			CWnd * wnd = GetDlgItem(IDC_MAPIMAGE);
			m_hdc = ::GetDC(wnd->m_hWnd);
			POINT* ViewMousePos = CalcSelectPoint(point);
			if(!ViewMousePos)
				return;
			CDC *pDC=wnd->GetDC();
			pDC->SetROP2(R2_XORPEN);
			CPen pen,*oldpen;
			pen.CreatePen(PS_DOT,2,RGB(127,127,127));
			oldpen=pDC->SelectObject(&pen);
			pDC->MoveTo(m_OldRect.TopLeft());
			pDC->LineTo(m_OldRect.right,m_OldRect.top);
			pDC->LineTo(m_OldRect.BottomRight());
			pDC->LineTo(m_OldRect.left,m_OldRect.bottom);
			pDC->LineTo(m_OldRect.TopLeft());
			
			m_rect=CRect(m_SelectRect.left,m_SelectRect.top,ViewMousePos->x,ViewMousePos->y);
			pDC->MoveTo(m_SelectRect.TopLeft());
			pDC->LineTo(m_SelectRect.right,m_SelectRect.top);
			pDC->LineTo(m_SelectRect.BottomRight());
			pDC->LineTo(m_SelectRect.left,m_SelectRect.bottom);
			pDC->LineTo(m_SelectRect.TopLeft());
			pDC->SelectObject(oldpen);
			pen.DeleteObject();
			ReleaseDC(pDC);
			m_OldRect=m_SelectRect;
		}
		else
		{
			if(g_pMouse)
				g_pMouse->MoveMouse((DWORD)nFlags, point.x, point.y);
		}
	}
	CFormView::OnMouseMove(nFlags, point);
}

void CRegenToolView::OnDrawBox()
{
	m_nProcess=1;
	m_rectTrack.SetProcess(m_nProcess);
}

void CRegenToolView::OnMapimage() 
{
	// TODO: Add your control notification handler code here
	AfxMessageBox("맵 이미지 클릭");
}


void CRegenToolView::AddMonsterList(MData *mdata)
{
	LV_ITEM lvitem;
	lvitem.iItem = m_MListCtrl.GetItemCount();
	lvitem.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_STATE;
	lvitem.iSubItem = 0;
	lvitem.stateMask = LVIS_STATEIMAGEMASK;
	lvitem.state = INDEXTOSTATEIMAGEMASK(1);
	lvitem.pszText = (LPSTR)(LPCTSTR)mdata->Group;

	m_MListCtrl.InsertItem(&lvitem);

	m_MListCtrl.SetItemText(lvitem.iItem, 1, (LPCTSTR)mdata->MonsterIdx);
	m_MListCtrl.SetItemText(lvitem.iItem, 2, (LPCTSTR)mdata->MonsterKind);
	m_MListCtrl.SetItemText(lvitem.iItem, 3, (LPCTSTR)mdata->x);
	m_MListCtrl.SetItemText(lvitem.iItem, 4, (LPCTSTR)mdata->z);
	m_MListCtrl.SetItemText(lvitem.iItem, 5, (LPCTSTR)mdata->Flag);
}

void CRegenToolView::OnSelchangeNpccombo() 
{
	// TODO: Add your control notification handler code here
	WORD count = (WORD)m_NpcKind.GetCurSel();
	BASE_MONSTER_LIST& mlist = g_pLoadList->m_MonsterList[count];

//	m_NumTIT.Format("Num");
//	m_RangeTIT.Format("Range");
//	m_TimeTIT.Format("Re_Time");

	CEngineObject* pMonster = new CEngineObject;
	int kind = mlist.MonsterKind;
	strcpy(m_MonsterChxName,mlist.ChxName);
		
	UpdateData(FALSE);
}

void CRegenToolView::OnSelchangeMapcombo() 
{
	// TODO: Add your control notification handler code here
	WORD count = (WORD)m_MapKind.GetCurSel();
//	MAP_LIST maplist = g_pLoadList->m_MapList[count];

//	m_MapNameStr.Format(maplist.LocalName);
	UpdateData(FALSE);
}

void CRegenToolView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int MinPos, MaxPos;
	pScrollBar->GetScrollRange(&MinPos, &MaxPos);
	switch(nSBCode)
    {
    case SB_LINELEFT:
        {
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() -LINE_SIZE); 
			if(m_StartImagePos.x - LINE_SIZE > MinPos)
				m_StartImagePos.x = pScrollBar->GetScrollPos() -LINE_SIZE;
			else
				m_StartImagePos.x = MinPos;
		}
        break;
    case SB_LINERIGHT:
		{
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() +LINE_SIZE);
			if(m_StartImagePos.x + LINE_SIZE < MaxPos)
				m_StartImagePos.x = pScrollBar->GetScrollPos() +LINE_SIZE;
			else
				m_StartImagePos.x = MaxPos;
		}
        break;
    case SB_PAGELEFT:
		{
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() -PAGE_SIZE);
			if(m_StartImagePos.x -PAGE_SIZE > MinPos)
				m_StartImagePos.x = pScrollBar->GetScrollPos() -PAGE_SIZE;
			else
				m_StartImagePos.x = MinPos;
		}
        break;
    case SB_PAGERIGHT:
        {
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() +PAGE_SIZE);
			if(m_StartImagePos.x + PAGE_SIZE < MaxPos)
				m_StartImagePos.x = pScrollBar->GetScrollPos() +PAGE_SIZE;
			else
				m_StartImagePos.x = MaxPos;
		}
        break;
    case SB_THUMBTRACK:
        pScrollBar->SetScrollPos(nPos);
		m_StartImagePos.x = nPos;
        break;
    }
	m_HScrollPos = pScrollBar->GetScrollPos();
	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
//	InvalidateRect(CRect(0,0,1,1));
	Invalidate();
}

void CRegenToolView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	int MinPos, MaxPos;
	pScrollBar->GetScrollRange(&MinPos, &MaxPos);
	switch(nSBCode)
    {
    case SB_LINEUP:
        {
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() -LINE_SIZE); 
			if(m_StartImagePos.y - LINE_SIZE > MinPos)
				m_StartImagePos.y = pScrollBar->GetScrollPos() -LINE_SIZE;
			else
				m_StartImagePos.y = MinPos;
		}
        break;
    case SB_LINEDOWN:
        {
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() +LINE_SIZE);
			if(m_StartImagePos.y + LINE_SIZE < MaxPos)
				m_StartImagePos.y = pScrollBar->GetScrollPos() +LINE_SIZE;
			else
				m_StartImagePos.y = MaxPos;
		}
        break;
    case SB_PAGEUP:
        {
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() -PAGE_SIZE); 
			if(m_StartImagePos.y - PAGE_SIZE > MinPos)
				m_StartImagePos.y = pScrollBar->GetScrollPos() -PAGE_SIZE;
			else
				m_StartImagePos.y = MinPos;
		}
        break;
    case SB_PAGEDOWN:
        {
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() +PAGE_SIZE);
			if(m_StartImagePos.y + PAGE_SIZE < MaxPos)
				m_StartImagePos.y = pScrollBar->GetScrollPos() +PAGE_SIZE;
			else
				m_StartImagePos.y = MaxPos;
		}
        break;
    case SB_THUMBTRACK:
        pScrollBar->SetScrollPos(nPos);
		m_StartImagePos.y = nPos;
        break;
    }
	m_VScrollPos = pScrollBar->GetScrollPos();
    CFormView::OnVScroll(nSBCode, nPos, pScrollBar);
//    InvalidateRect(CRect(0,0,1,1));
	Invalidate();

	
}

/*
void CMyPainterView::ClientToDoc(CPoint& point)

{

    CClientDC dc(this);
    OnPrepareDC(&dc, NULL);
    dc.DPtoLP(&point);

}

*/
/*
width 50, height 50의 비트맵을 120*120 으로확대

StretchDIBits(hdc,0,0,120,120,0,0,50,50,imgBit,(BITMAPINFO*)&bmpInfoHeader,DIB_RGB_COLORS,SRCCOPY);

 

150,150 의 비트맵 30*30으로 축소

StretchDIBits(hdc,0,0,30,30,0,0,150,150,imgBit,(BITMAPINFO*)&bmpInfoHeader,DIB_RGB_COLORS,SRCCOPY);
*/

void CRegenToolView::OnSelectChk() 
{
	// TODO: Add your control notification handler code here
//	SetButtonCheck(eSELECT);
	m_SelectBtn.SetCheck(TRUE);
	m_MSelectNoAreaBtn.SetCheck(FALSE);
	m_MSelectAreaBtn.SetCheck(FALSE);
	m_DeleteBtn.SetCheck(FALSE);
	m_ZoomInBtn.SetCheck(FALSE);
	m_ZoomOutBtn.SetCheck(FALSE);
	m_MoveBtn.SetCheck(FALSE);
}

void CRegenToolView::OnMselectnoareaChk() 
{
	// TODO: Add your control notification handler code here
//	SetButtonCheck(eMSELECTNOAREA);
	m_MSelectNoAreaBtn.SetCheck(TRUE);
	m_SelectBtn.SetCheck(FALSE);
	m_MSelectAreaBtn.SetCheck(FALSE);
	m_DeleteBtn.SetCheck(FALSE);
	m_ZoomInBtn.SetCheck(FALSE);
	m_ZoomOutBtn.SetCheck(FALSE);
	m_MoveBtn.SetCheck(FALSE);
}

void CRegenToolView::OnMselectareaChk() 
{
	// TODO: Add your control notification handler code here
//	SetButtonCheck(eMSELECTAREA);
	m_MSelectAreaBtn.SetCheck(TRUE);
	m_SelectBtn.SetCheck(FALSE);
	m_MSelectNoAreaBtn.SetCheck(FALSE);	
	m_DeleteBtn.SetCheck(FALSE);
	m_ZoomInBtn.SetCheck(FALSE);
	m_ZoomOutBtn.SetCheck(FALSE);
	m_MoveBtn.SetCheck(FALSE);
}

void CRegenToolView::OnDeleteChk() 
{
	// TODO: Add your control notification handler code here
//	SetButtonCheck(eDELETE);
	m_DeleteBtn.SetCheck(TRUE);
	m_SelectBtn.SetCheck(FALSE);
	m_MSelectNoAreaBtn.SetCheck(FALSE);
	m_MSelectAreaBtn.SetCheck(FALSE);
	m_ZoomInBtn.SetCheck(FALSE);
	m_ZoomOutBtn.SetCheck(FALSE);
	m_MoveBtn.SetCheck(FALSE);
}

void CRegenToolView::OnZoominChk() 
{
	// TODO: Add your control notification handler code here
//	SetButtonCheck(eZOOMIN);
	m_ZoomInBtn.SetCheck(TRUE);
	m_SelectBtn.SetCheck(FALSE);
	m_MSelectNoAreaBtn.SetCheck(FALSE);
	m_MSelectAreaBtn.SetCheck(FALSE);
	m_DeleteBtn.SetCheck(FALSE);	
	m_ZoomOutBtn.SetCheck(FALSE);
	m_MoveBtn.SetCheck(FALSE);
}

void CRegenToolView::OnZoomoutChk() 
{
	// TODO: Add your control notification handler code here
//	SetButtonCheck(eZOOMOUT);
	m_ZoomOutBtn.SetCheck(TRUE);
	m_SelectBtn.SetCheck(FALSE);
	m_MSelectNoAreaBtn.SetCheck(FALSE);
	m_MSelectAreaBtn.SetCheck(FALSE);
	m_DeleteBtn.SetCheck(FALSE);
	m_ZoomInBtn.SetCheck(FALSE);
	m_MoveBtn.SetCheck(FALSE);
}

void CRegenToolView::OnMoveChk() 
{
	// TODO: Add your control notification handler code here
//	SetButtonCheck(eMOVE);
	m_MoveBtn.SetCheck(TRUE);
	m_SelectBtn.SetCheck(FALSE);
	m_MSelectNoAreaBtn.SetCheck(FALSE);
	m_MSelectAreaBtn.SetCheck(FALSE);
	m_DeleteBtn.SetCheck(FALSE);
	m_ZoomInBtn.SetCheck(FALSE);
	m_ZoomOutBtn.SetCheck(FALSE);
	
}

void CRegenToolView::SetButtonCheck(int button)
{
	for(int btn = 0; btn < MAX_BUTTON; btn++)
	{
		if(button == btn)
			m_ButtonFlag[btn] = TRUE;
		else
			m_ButtonFlag[btn] = FALSE;
	}
}

DWORD GXPlayerPROC(I4DyuchiGXExecutive* pExecutive,GXMAP_OBJECT_HANDLE gxh,DWORD msg,int arg1,int arg2,void* pData)
{
/*
	DWORD	dwMotionNum = pExecutive->GXOGetMotionNum(gxh,0);
	DWORD	dwMotionIndex = pExecutive->GXOGetCurrentMotionIndex(gxh);
	DWORD	dwFrame = pExecutive->GXOGetCurrentFrame(gxh);

	MOTION_DESC motionDesc;
	BOOL	bHasMotion = pExecutive->GXOGetMotionDesc(gxh,&motionDesc,dwMotionIndex,0);


	if (bHasMotion)
	{
	
		if (motionDesc.dwLastFrame <= dwFrame)
		{
			pExecutive->GXOSetCurrentFrame(gxh,0);
			
			dwMotionIndex++;
			if (dwMotionIndex > dwMotionNum)
				dwMotionIndex = 1;

			pExecutive->GXOSetCurrentMotionIndex(gxh,dwMotionIndex);

		}
		else 
			pExecutive->GXOSetCurrentFrame(gxh,dwFrame+arg1);
	}
	
	g_dwFrameCount++;
	DWORD dwMtlNum = pExecutive->GXOGetMaterialNum(gxh,0);
	if (dwMtlNum)
	{
		if (!(g_dwFrameCount % 32))
		{


			DWORD dwMtlIndex = pExecutive->GXOGetCurrentMaterialIndex(gxh);
			dwMtlIndex++;
			if (dwMtlIndex > dwMtlNum)
				dwMtlIndex = 0;

			pExecutive->GXOSetCurrentMaterialIndex(gxh,dwMtlIndex);
		}
	}
*/	
	return 0;

}

void CRegenToolView::InitEngine()
{
	CWnd * wnd = GetDlgItem(IDC_MAPIMAGE);
	CoInitialize(NULL);
	char temp[256];
	GetCurrentDirectory(256,temp);
//	MSG msg;
	HRESULT hr;
	
	DISPLAY_INFO DispInfo;
	DispInfo.dwWidth = 1024;
	DispInfo.dwHeight = 768;
	DispInfo.dwBPS = 4;
	DispInfo.dispType = WINDOW_WITH_BLT;
	
	///* Load DLL 사용
	// 먼저 executive의 DLL을 로드한다.
	g_hExecutiveHandle = LoadLibrary("SS3DExecutiveForMuk.dll");
		
	// DllCreateInstance 는 3개의 엔진 DLL이 모두 자신의 것을 가지고 있다.각 엔진 COM의 인터페이스를 
	// 얻게 해주는 함수이다. 일단 executive의 것만 얻어내면 다른건 executive초기화 시에 자동으로 얻을수 있다.
	CREATE_INSTANCE_FUNC        pFunc;
	pFunc = (CREATE_INSTANCE_FUNC)GetProcAddress(g_hExecutiveHandle,"DllCreateInstance");
	hr = pFunc((void**)&g_pExecutive);
	if (hr != S_OK)
	{		
		MessageBox("Executive생성 실패","Error",MB_OK);
		return;
	}

	if(g_bUsingEnginePack)
	{
		PACKFILE_NAME_TABLE table[6];
		memset(table,0,sizeof(PACKFILE_NAME_TABLE)*6);
		strcpy(table[0].szFileName,"effect.pak");
		strcpy(table[1].szFileName,"character.pak");
		strcpy(table[2].szFileName,"map.pak");
		strcpy(table[3].szFileName,"monster.pak");
		strcpy(table[4].szFileName,"npc.pak");
		strcpy(table[5].szFileName,"cloud.pak");
		// geometry DLL의 이름, renderer DLL의 이름을 넣어주면 executive가 알아서 생성,초기화해준다.
		g_pExecutive->InitializeFileStorageWithoutRegistry("SS3DFileStorage.dll",
			12000,4096,128,FILE_ACCESS_METHOD_FILE_OR_PACK,table,6);
	}
	else
	{
		// geometry DLL의 이름, renderer DLL의 이름을 넣어주면 executive가 알아서 생성,초기화해준다.
		g_pExecutive->InitializeFileStorageWithoutRegistry("SS3DFileStorage.dll",
			12000,4096,128,FILE_ACCESS_METHOD_ONLY_FILE,NULL,0);
	}

	g_pExecutive->InitializeWithoutRegistry("SS3DGeometryForMuk.dll","SS3DRendererForMuk.dll",
		wnd->GetSafeHwnd(), &DispInfo, 160000, 32,0,1, 0,MHErrorHandleProc);

/*
	// Excutive생성
	hr = CoCreateInstance(
		CLSID_4DyuchiGXExecutive,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_4DyuchiGXExecutive,
		(void**)&g_pExecutive);
	
	if (hr != S_OK)
		__asm int 3
		
		// 엔진의 executive 초기화.나머지 컴포넌트들은 executive가 초기화시켜준다.
		g_pExecutive->Initialize(wnd->m_hWnd,NULL,
		4096,		// 오브젝트 최대갯수 
		512,		// 라이트 최대갯수 
		1024,		// 트리거 최대갯수 
		32,			// 뷰포트 최대갯수 
		32, MHErrorHandleProc);		// 데칼 최대갯수		
*/	
	// 마우스로 카메라를 제어하기 위한 헬퍼클래스 초기화.
	g_pMouse = new MToolMouseStatus;
	g_pMouse->Initialize( g_pExecutive->GetGeometry());
	
	
	// GXObject로드.
//	GXOBJECT_HANDLE fighter  = g_pExecutive->CreateGXObject("L008.chr",GXPlayerPROC,0,0);
	
	// executive는 뷰포트 0번을 사용할 것이다.		
	g_pExecutive->SetViewport(0);
	
	// 카메라 초기화
	VECTOR3	v3From;
	v3From.x = 0.0f;
	v3From.y = 1000.0f;
	v3From.z = 0.0f;
	
	g_pExecutive->GetGeometry()->ResetCamera(&v3From,DEFAULT_NEAR,DEFAULT_FAR,DEFAULT_FOV,0);
	// 깜깜하니 캐릭터를 잘 알아볼수가 없다.빛을 주자.
	DIRECTIONAL_LIGHT_DESC	dirLight;
	dirLight.v3Dir.x = 0.5f;
	dirLight.v3Dir.y = -1.0f;
	dirLight.v3Dir.z = 0.5f;
	dirLight.dwSpecular = 0xffffffff;
	dirLight.dwDiffuse = 0xfffffff;
	dirLight.dwAmbient = 0xff202020;
	
	
	g_pExecutive->GetGeometry()->SetDirectionalLight(&dirLight,0);
	g_pExecutive->GetGeometry()->EnableDirectionalLight(0);
	
	g_pExecutive->GetGeometry()->SetShadowFlag(ENABLE_PROJECTION_SHADOW |	ENABLE_PROJECTION_TEXMAP);
	
	
//	m_MonsterChxName[0] = 0;

}

void CRegenToolView::LoadMapImage(CString filename)
{
	m_hBMP = (HBITMAP) LoadImage( 
		NULL,
		filename,
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE
		);
	CWnd * wnd = GetDlgItem(IDC_MAPIMAGE);
	m_hdc = ::GetDC(wnd->m_hWnd);
	GetObject(m_hBMP, sizeof(BITMAP), &m_bmp);
	m_hMemDC = CreateCompatibleDC(m_hdc);
	SelectObject(m_hMemDC, m_hBMP);
	CRect rect;
	CRect ImageBox;
	
	wnd->GetClientRect(&m_ViewWinSize);

	m_ImageSize = CRect(0, 0, m_bmp.bmWidth,m_bmp.bmHeight);
	
	if(m_bmp.bmWidth - rect.right <= 0)
		m_ImageBoxRect.right = m_bmp.bmWidth;
	else
		m_ImageBoxRect.right = m_bmp.bmWidth - (m_bmp.bmWidth - rect.right);

	if(m_bmp.bmHeight - rect.bottom <= 0)
		m_ImageBoxRect.bottom = m_bmp.bmHeight;
	else
		m_ImageBoxRect.bottom = m_bmp.bmHeight - (m_bmp.bmHeight - rect.bottom);


	// 스크롤 Max값과 Range를 계산한다 ---------------------
	int VMaxPos, HMaxPos;
	if(m_bmp.bmHeight - rect.bottom)
		VMaxPos = m_bmp.bmHeight - rect.bottom;
	else
		VMaxPos = 0;

	if(m_bmp.bmWidth - rect.right)
		HMaxPos = m_bmp.bmWidth - rect.right;
	else
		HMaxPos = 0;
	m_ImageVScroll.SetScrollRange(rect.top, VMaxPos);
	m_ImageHScroll.SetScrollRange(rect.left, HMaxPos);
	
	// -----------------------------------------------------

	SetStartSelectFlag(TRUE);
}


void CRegenToolView::InsertMonster(VECTOR3* TargetPos)
{
	DIRECTORYMGR->SetLoadMode(eLM_Monster);

	CWnd * wnd = GetDlgItem(IDC_MAPIMAGE);
	CRect rect;
//	wnd->GetClientRect(&rect);
//	VECTOR3 TargetPos = GetXYZFromScreenXY( g_pExecutive->GetGeometry(),
//				x, y, pview->GetViewWinRect()->right, pview->GetViewWinRect()->bottom);
//	VECTOR3* TargetPos = GetPickedPosition(point.x,point.y);
//	if(!TargetPos)
//		return;
	
	// 맵에 몬스터 추가
//	sprintf(temp, "./MonsterData/%s", m_MonsterChxName);
	GXOBJECT_HANDLE fighter  = g_pExecutive->CreateGXObject(m_MonsterChxName, MHPlayerPROC,0,0);
	if(!fighter)
		fighter  = g_pExecutive->CreateGXObject("L008.chx", MHPlayerPROC,0,0);
	
	
	VECTOR3 MonsterMapPos;
	VECTOR3 MonsterAnger;
	float aaa;
	g_pExecutive->GXOEnableHFieldApply(fighter);
	g_pExecutive->GXOSetPosition(fighter, TargetPos, FALSE);
	g_pExecutive->GXOGetPosition(fighter, &MonsterMapPos);
	g_pExecutive->GXOGetDirection(fighter, &MonsterAnger, &aaa);
	CRegenToolDoc* pDoc = GetDocument();
	
	// ListCtrl에 추가 -------------------------------------
	MData data;
	WORD count;
	if(!m_DataInsertFlag)
		count = (WORD)m_MonsterKind.GetCurSel();
	else
		count = m_DataInsertKind;
	data.Group.Format("1");
	data.MonsterIdx.Format("%d", GetMonsterIndex());
	data.MonsterKind.Format("%d", g_pLoadList->m_MonsterList[count].MonsterKind);
	data.x.Format("%4.2f", TargetPos->x);
	data.z.Format("%4.2f", TargetPos->z);
	data.Flag.Format("0");
	AddMonsterList(&data);
//	AddMonsterIndex();
	//------------------------------------------------------


	CString szGroup;
	MONSTER_POINT* mp = new MONSTER_POINT;
	mp->MonsterGrade = GetMonsterGrade();
	mp->MonsterIndex = m_dwMonsterIndex;
	mp->MonsterKind = g_pLoadList->m_MonsterList[count].MonsterKind;
	mp->SelectCount = m_ListIndex;
	mp->point.x = TargetPos->x;
	mp->point.y = TargetPos->z;
	mp->RegenFlag = TRUE;
	mp->MHandle = fighter;
	m_GroupEdit.GetWindowText(szGroup);
	mp->MonsterGroup = (WORD)atoi(szGroup);
	pDoc->AddSelectMonsterPoint(mp);
	pDoc->AddGroupMonsterCount(szGroup);
	pDoc->AddCount();
	AddListIndex();
	AddMonsterIndex();

	DIRECTORYMGR->SetLoadMode(eLM_Root);
	return;
}

void CRegenToolView::DeleteMonster(CPoint point)
{
/*	CRegenToolDoc* pDoc = GetDocument();
	pDoc->SubCount();
	if(GetMonsterIndex()==START_MONSTERINDEX)
		return;
	ReleaseMonsterIndex();
	pDoc->ReleaseGroupMonsterCount();
*/		
}
BOOL CRegenToolView::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete g_pMouse;
	MAP->Release();
	g_pExecutive->Release();
	
	CoFreeUnusedLibraries();
	CoUninitialize();
	return CFormView::DestroyWindow();
}

POINT* CRegenToolView::CalcSelectPoint(CPoint point)
{
	CWnd * wnd = GetDlgItem(IDC_MAPIMAGE);
	CRect wrect, crect, prect;
	static POINT ViewMousePos;
	//	POINT pos;
	wnd->GetWindowRect(&wrect);	// 전체 윈도우 기준에서의 ImageView Rect
	wnd->GetClientRect(&crect);	// 자기자신 기준에서의 ImageView Rect
	CWnd* pWnd = wnd->GetParent();
	pWnd->GetWindowRect(&prect); // 자기 부모FromView기준에서의 ImageView Rect 
	
	// 전체 윈도 기준에서 FromView기준을 빼면 FormView에서 자기자신까지의 사이값이 나온다.
	// 이값을 ImageBox의 시작 좌표로 설정하고 마우스 좌표에서 이값을 빼면 정확한 포인트가 나온다.
	CRect ImageBox;
	ImageBox.left = wrect.left - prect.left; 
	ImageBox.top = wrect.top - prect.top;
	ImageBox.SetRect(ImageBox.TopLeft(), wrect.BottomRight());
	ViewMousePos.x = point.x - ImageBox.left;
	ViewMousePos.y = point.y - ImageBox.top;

	g_ScreenInfo.dwWidth = crect.right;
	g_ScreenInfo.dwHeight = crect.bottom;
	if(ViewMousePos.x > crect.left && ViewMousePos.y > crect.top &&
		ViewMousePos.x < crect.right && ViewMousePos.y < crect.bottom)
		return &ViewMousePos;
	return NULL;
}

void CRegenToolView::OnFileSave() 
{
	// TODO: Add your command handler code here
	char Filter[] = "TXT format(*.txt)|*.txt|MAP format(*.map)|*.map|BMP format(*.bmp)|*.bmp|All format(*.*)|*.*|";
	CFileDialog fileDlg(FALSE, NULL, NULL, OFN_HIDEREADONLY, Filter);
	CRegenToolDoc* pDoc = GetDocument();
	
	if(fileDlg.DoModal() == IDOK)
	{
		CString data = fileDlg.GetPathName();
		pDoc->OnSaveDocument(data);
	}
	

}

void CRegenToolView::LoadDataInsert(CString filename)
{
	CRegenToolDoc* pDoc = GetDocument();
	DWORD MaxObjectNum = 0;
	WORD GroupProperty = 0;
	WORD MonsterGroup = 0;
	CString szGroupName;
	CString szGroup;

	pDoc->RemoveAllConditionList();
	pDoc->RemoveAllMonsterList();

	g_pExecutive->DeleteAllGXObjects();
	
	if(GetStartSelectFlag())
	{
		
		CMHFile file;
		if(!file.Init((LPTSTR)(LPCTSTR)filename, "rt"))
			return ;
		
		char buff[256];
		while(1)
		{
			if(file.IsEOF())
			{
				break;
			}
			file.GetString(buff);
			if(buff[0] == '}')
			{	
				continue;
			}
			if(buff[0] == '{')
			{
				continue;
			}
			if(buff[0] == '@')
			{
				file.GetLine(buff, 256);
				continue;
			}
			strcpy(buff, _strupr(buff));
			CMD_ST(buff)
			CMD_CS("$GROUP")
				MonsterGroup = file.GetWord();
				szGroup.Format("%d", MonsterGroup);
			CMD_CS("#MAXOBJECT")
				MaxObjectNum = file.GetDword();
			CMD_CS("#PROPERTY")
				GroupProperty = file.GetWord();
			CMD_CS("#GROUPNAME")
				strcpy((LPTSTR)(LPCTSTR)szGroupName,file.GetString());
			
				LIST_CONDITION* Condition = new LIST_CONDITION;
				Condition->ConditionNum = 0;
				Condition->wRegenCount = 0;
				Condition->szGroupNum = szGroup;
				Condition->szGroupName = szGroupName;
				Condition->szGroupProperty.Format("%d", GroupProperty);

				CRegenToolDoc* pDoc = GetDocument();
				pDoc->AddCondition(Condition);
				m_ConditionAllCheckFlag = TRUE;
				
			CMD_CS("#ADDCONDITION")
				ADD_PROPERTY proper;// = new ADD_PROPERTY;
				proper.szTargetGroupID.Format("%d", file.GetDword());
				proper.szCondition.Format("%4.2f", file.GetFloat());
				proper.szRegenTime.Format("%d", file.GetDword());
				pDoc->AddProperty(szGroup, &proper);
			CMD_CS("#ADD")
				MONSTER_POINT* mp = new MONSTER_POINT;
				mp->MonsterGrade = file.GetInt();
				mp->MonsterIndex = file.GetDword();
				mp->MonsterKind = file.GetWord();
				mp->MonsterGroup = MonsterGroup;
				mp->point.x = file.GetFloat();
				mp->point.y = file.GetFloat();
				mp->RegenFlag = file.GetBool();
				
				DIRECTORYMGR->SetLoadMode(eLM_Monster);
				

				CWnd * wnd = GetDlgItem(IDC_MAPIMAGE);
				CRect rect;
				
				// 맵에 몬스터 추가
				BASE_MONSTER_LIST& mlist = g_pLoadList->m_MonsterList[mp->MonsterKind-1];
				GXOBJECT_HANDLE fighter  = g_pExecutive->CreateGXObject(mlist.ChxName , MHPlayerPROC,0,0);
				if(!fighter)
					fighter  = g_pExecutive->CreateGXObject("L008.chx", MHPlayerPROC,0,0);
				
				
				VECTOR3 TargetPos;
				TargetPos.x = mp->point.x;
				TargetPos.y = 0;
				TargetPos.z = mp->point.y;

				VECTOR3 MonsterMapPos;
				VECTOR3 MonsterAnger;
				float aaa;
				g_pExecutive->GXOEnableHFieldApply(fighter);
				g_pExecutive->GXOSetPosition(fighter, &TargetPos, FALSE);
				g_pExecutive->GXOGetPosition(fighter, &MonsterMapPos);
				g_pExecutive->GXOGetDirection(fighter, &MonsterAnger, &aaa);
				CRegenToolDoc* pDoc = GetDocument();


				mp->MHandle = fighter;
				pDoc->AddSelectMonsterPoint(mp);
				pDoc->AddGroupMonsterCount(szGroup);
				pDoc->AddCount();
				AddListIndex();
				AddMonsterIndex();

				DIRECTORYMGR->SetLoadMode(eLM_Root);
			CMD_EN
		}
	}
	

}


void CRegenToolView::OnSelchangeFunccombo() 
{
	// TODO: Add your control notification handler code here
	WORD count = (WORD)m_FuncKind.GetCurSel();
	switch(count)
	{
	case eMonster_Regen:
		{
			// MonsterKind Combo Setting  ------------------------------------------
			m_MonsterKind.ShowWindow(SW_SHOW);
			m_SubComboStatic.ShowWindow(SW_SHOW);
			m_NpcKind.ShowWindow(SW_HIDE);
			
			m_SubComboStatic.SetWindowText(TMONSTER);
			UpdateData(FALSE);
			//------------------------------------------------------------------
	
			// Monster Control Show -----------------------------------------------
			m_Condition1GroupBox.ShowWindow(SW_SHOW);
			m_GroupStatic.ShowWindow(SW_SHOW);
			m_GroupEdit.ShowWindow(SW_SHOW);
			m_GroupSpin.ShowWindow(SW_SHOW);
			m_GroupNameStatic.ShowWindow(SW_SHOW);
			m_GroupNameEdit.ShowWindow(SW_SHOW);
			m_GroupPropertyStatic.ShowWindow(SW_SHOW);
			m_GroupPropertyEdit.ShowWindow(SW_SHOW);
			m_GroupPropertySpin.ShowWindow(SW_SHOW);
			
			m_Condition2GroupBox.ShowWindow(SW_SHOW);
			m_TargetGroupIDStatic.ShowWindow(SW_SHOW);
			m_TargetGroupIDEdit.ShowWindow(SW_SHOW);
			m_TargetGroupIDSpin.ShowWindow(SW_SHOW);
			m_ConditionStatic.ShowWindow(SW_SHOW);
			m_ConditionEdit.ShowWindow(SW_SHOW);
			m_ConditionSpin.ShowWindow(SW_SHOW);
			m_RegenTimeStatic.ShowWindow(SW_SHOW);
			m_RegenTimeEdit.ShowWindow(SW_SHOW);
			m_RegenTimeSpin.ShowWindow(SW_SHOW);
			
			m_AllConditionGroupBox.ShowWindow(SW_SHOW);
			m_ConditionAllOKBtn.ShowWindow(SW_SHOW);
			
			m_ListViewBtn.ShowWindow(SW_SHOW);
			//-------------------------------------------------------------
		}
		break;
	case eNpc_Regen:
		{
			// NPCKind Combo Setting  ------------------------------------------
			m_MonsterKind.ShowWindow(SW_HIDE);
			m_SubComboStatic.ShowWindow(SW_SHOW);
			m_NpcKind.ShowWindow(SW_SHOW);
			
			m_SubComboStatic.SetWindowText(TNPC);
			UpdateData(FALSE);
			//------------------------------------------------------------------

			// Monster Control Hide -----------------------------------------------
			m_Condition1GroupBox.ShowWindow(SW_HIDE);
			m_GroupStatic.ShowWindow(SW_HIDE);
			m_GroupEdit.ShowWindow(SW_HIDE);
			m_GroupSpin.ShowWindow(SW_HIDE);
			m_GroupNameStatic.ShowWindow(SW_HIDE);
			m_GroupNameEdit.ShowWindow(SW_HIDE);
			m_GroupPropertyStatic.ShowWindow(SW_HIDE);
			m_GroupPropertyEdit.ShowWindow(SW_HIDE);
			m_GroupPropertySpin.ShowWindow(SW_HIDE);
			
			m_Condition2GroupBox.ShowWindow(SW_HIDE);
			m_TargetGroupIDStatic.ShowWindow(SW_HIDE);
			m_TargetGroupIDEdit.ShowWindow(SW_HIDE);
			m_TargetGroupIDSpin.ShowWindow(SW_HIDE);
			m_ConditionStatic.ShowWindow(SW_HIDE);
			m_ConditionEdit.ShowWindow(SW_HIDE);
			m_ConditionSpin.ShowWindow(SW_HIDE);
			m_RegenTimeStatic.ShowWindow(SW_HIDE);
			m_RegenTimeEdit.ShowWindow(SW_HIDE);
			m_RegenTimeSpin.ShowWindow(SW_HIDE);
			
			m_AllConditionGroupBox.ShowWindow(SW_HIDE);
			m_ConditionAllOKBtn.ShowWindow(SW_HIDE);
			
			m_ListViewBtn.ShowWindow(SW_HIDE);
			//------------------------------------------------------------------
		}
		break;
	case eLogin_Point:
		{
			
//			m_SubComboStatic.SetWindowText(TLOGIN);
		}
		break;
	case eChangeMap_Point:
		{

//			m_SubComboStatic.SetWindowText(TCHANGEMAP);
		}
		break;
	case eMap_Attribute:
		{

//			m_SubComboStatic.SetWindowText(TATTRIBUTE);
		}
		break;
	}
	
}

void CRegenToolView::OnConditionallok() 
{
	// TODO: Add your control notification handler code here
	CString temp;
	LIST_CONDITION* Condition = new LIST_CONDITION;
	Condition->ConditionNum = 0;
	Condition->wRegenCount = 0;
	m_GroupEdit.GetWindowText(temp);
	Condition->szGroupNum = temp;
	m_GroupNameEdit.GetWindowText(temp);
	Condition->szGroupName = temp;
	m_GroupPropertyEdit.GetWindowText(temp);
	Condition->szGroupProperty = temp;
	
	
	ADD_PROPERTY proper;
	m_TargetGroupIDEdit.GetWindowText(temp);
	proper.szTargetGroupID = temp;
	m_ConditionEdit.GetWindowText(temp);
	proper.szCondition = temp;
	m_RegenTimeEdit.GetWindowText(temp);
	proper.szRegenTime = temp;

	CRegenToolDoc* pDoc = GetDocument();
	pDoc->AddCondition(Condition);
	pDoc->AddProperty(Condition->szGroupNum, &proper);
	m_ConditionAllCheckFlag = TRUE;

}

void CRegenToolView::OnListview() 
{
	// TODO: Add your control notification handler code here
	if(!m_ListViewDlg)
	{
		m_ListViewDlg = new CMonsterListViewDlg;
		m_ListViewDlg->Create(this);
		m_ListViewDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_ListViewDlg->DestroyWindow();
		delete m_ListViewDlg;
		m_ListViewDlg = NULL;
		m_ListViewDlg = new CMonsterListViewDlg;
		m_ListViewDlg->Create(this);
		m_ListViewDlg->ShowWindow(SW_SHOW);
	}
}
void CRegenToolView::OnAddcondition() 
{
	// TODO: Add your control notification handler code here
	CString temp;
	CString group;
	CRegenToolDoc* pDoc = GetDocument();
	m_GroupEdit.GetWindowText(group);
	ADD_PROPERTY proper;
	m_TargetGroupIDEdit.GetWindowText(temp);
	proper.szTargetGroupID = temp;
	m_ConditionEdit.GetWindowText(temp);
	proper.szCondition = temp;
	m_RegenTimeEdit.GetWindowText(temp);
	proper.szRegenTime = temp;
	pDoc->AddProperty(group, &proper);
}

void CRegenToolView::OnSelchangeMoncombo() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	WORD count = (WORD)m_MonsterKind.GetCurSel();
	BASE_MONSTER_LIST& mlist = g_pLoadList->m_MonsterList[count];

//	m_NumTIT.Format("Num");
//	m_RangeTIT.Format("Range");
//	m_TimeTIT.Format("Re_Time");

	CEngineObject* pMonster = new CEngineObject;
	int kind = mlist.MonsterKind;
	strcpy(m_MonsterChxName,mlist.ChxName);
		
	UpdateData(FALSE);
}

void CRegenToolView::OnKillfocusGroupedit() 
{
	// TODO: Add your control notification handler code here
	CString temp;
	m_GroupEdit.GetWindowText(temp);
	m_TargetGroupIDEdit.SetWindowText(temp);
	
}


void CRegenToolView::OnGrade1() 
{
	// TODO: Add your control notification handler code here
	m_MonsterGrade = BASIC_MONSTER;
}

void CRegenToolView::OnGrade2() 
{
	// TODO: Add your control notification handler code here
	m_MonsterGrade = BOSS_MONSTER;
}

void CRegenToolView::OnGrade3() 
{
	// TODO: Add your control notification handler code here
	m_MonsterGrade = SPECIAL_MONSTER;
}

void CRegenToolView::OnLoadmapbtn() 
{
	// TODO: Add your control notification handler code here
	CMapSelectDlg MapSelectDlg;
	MapSelectDlg.DoModal();
}

void CRegenToolView::OnDestroy() 
{
	if(m_ListViewDlg)
	{
		m_ListViewDlg->DestroyWindow();
		delete m_ListViewDlg;
		m_ListViewDlg = NULL;
	}
	CFormView::OnDestroy();
	
	// TODO: Add your message handler code here
	
	if(g_pLoadList)
	{
		delete g_pLoadList;
		g_pLoadList = NULL;
	}
	if(g_pMouse)
	{
		delete g_pMouse;
		g_pMouse = NULL;
	}
	DeleteDC(m_hMemDC);
}
