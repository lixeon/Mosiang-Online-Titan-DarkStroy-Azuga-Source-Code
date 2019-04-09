// RegenToolView.h : interface of the CRegenToolView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGENTOOLVIEW_H__A67B17DC_FC0B_41D3_A0DA_5E93FC2CB687__INCLUDED_)
#define AFX_REGENTOOLVIEW_H__A67B17DC_FC0B_41D3_A0DA_5E93FC2CB687__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyTracker.h"
#include "SelectXY.h"
#include "DefineStruct.h"

#include "../4DyuchiGRX_common/IExecutive.h"
#include "../4DyuchiGRX_common/IRenderer.h"
#include "../4DyuchiGXGFunc/global.h"
#include "../4DyuchiGRX_myself97_util/MToolMouseStatus.h"
#include "Tilemanager.h"
#include "MonsterListViewDlg.h"
#include "MapSelectDlg.h"

extern MToolMouseStatus*	g_pMouse;
extern HMODULE        g_hExecutiveHandle;
DWORD GXPlayerPROC(I4DyuchiGXExecutive* pExecutive,GXMAP_OBJECT_HANDLE gxh,DWORD msg,int arg1,int arg2,void* pData);
DWORD __stdcall MHErrorHandleProc(ERROR_TYPE type,DWORD dwErrorPriority,void* pCodeAddress,char* szStr);

#define START_MONSTERINDEX 100001

// MList 에 찍기위한 구조체
struct MData
{
	CString Group;
	CString MonsterIdx;
	CString MonsterKind;
	CString x;
	CString z;
	CString Flag;
};


class CRegenToolView : public CFormView
{
	
	CMonsterListViewDlg* m_ListViewDlg;
	BOOL m_ConditionAllCheckFlag;

	CString m_WorkPath;

	BOOL m_StartSelectFlag;
	POINT m_StartImagePos;
	WORD m_ListIndex;
	BOOL m_DataInsertFlag;
	WORD m_DataInsertKind;
	DWORD m_dwMonsterIndex;

	// Func_Kind Commbo 변수
	CString m_szFuncKind[MAX_FUNCKIND_NUM];

	CRect m_ViewWinSize;	// 이미지 출력 윈도 크기
	CRect m_ImageSize;		// 이미지 실제 크기

	// 영역 마우스 셋팅
	BOOL m_bDown;
	CRect m_SelectRect;
	CRect m_OldRect;

	// MapImageBox 관련 변수
	CBitmap m_LoadMap;
	BITMAPINFO szBitmapInfo;
	HBITMAP m_hBMP;
	CRect m_ImageBoxRect;
	BITMAP m_bmp;
	HDC m_hdc;
	HDC m_hMemDC;

	// Select 관련 변수
	CMyTracker m_rectTrack;
	CRect m_rect;
	int m_nProcess;
	CSelectXY m_SelectXY;

	CBitmap m_MonsterSelectAreaBitmap_on;
	CBitmap m_MonsterSelectAreaBitmap_press;
	CBitmap m_MonsterSelectNoAreaBitmap_on;
	CBitmap m_MonsterSelectNoAreaBitmap_press;
	CBitmap m_MoveBitmap_on;
	CBitmap m_MoveBitmap_press;
	CBitmap m_ZoomOutBitmap_on;
	CBitmap m_ZoomOutBitmap_press;
	CBitmap m_ZoomInBitmap_on;
	CBitmap m_ZoomInBitmap_press;
	CBitmap m_SelectBitmap_on;
	CBitmap m_SelectBitmap_press;
	CBitmap m_DeleteBitmap_on;
	CBitmap m_DeleteBitmap_press;
	
	// Button Image
	LPSTR m_MonsterSelectArea_on;
	LPSTR m_MonsterSelectArea_press;
	LPSTR m_MonsterSelectNoArea_on;
	LPSTR m_MonsterSelectNoArea_press;
	LPSTR m_Move_on;
	LPSTR m_Move_press;
	LPSTR m_ZoomOut_on;
	LPSTR m_ZoomOut_press;
	LPSTR m_ZoomIn_on;
	LPSTR m_ZoomIn_press;
	LPSTR m_Select_on;
	LPSTR m_Select_press;
	LPSTR m_Delete_on;
	LPSTR m_Delete_press;
	
	LPSTR m_MapImageCtl;
	// 버튼 변수선언
	BOOL m_ButtonFlag[MAX_BUTTON]; 

	int m_VScrollPos;						// 수직스크롤 위치
	int m_HScrollPos;						// 수평스크롤 위치
	
	CPoint m_MPoint;
	I4DyuchiGXGeometry*			mpGeometry;
	char m_MonsterChxName[128];
	
	CTile m_ViewTile[TILE_MAX_WIDTH * TILE_MAX_HEIGHT];

	int m_MonsterGrade;

protected: // create from serialization only
	CRegenToolView();
	DECLARE_DYNCREATE(CRegenToolView)

public:
	//{{AFX_DATA(CRegenToolView)
	enum { IDD = IDD_REGENTOOL_FORM };
	CButton	m_GradeRadio;
	CButton	m_AddConditionBtn;
	CStatic	m_SubComboStatic;
	CStatic	m_TargetGroupIDStatic;
	CSpinButtonCtrl	m_TargetGroupIDSpin;
	CEdit	m_TargetGroupIDEdit;
	CStatic	m_RegenTimeStatic;
	CSpinButtonCtrl	m_RegenTimeSpin;
	CEdit	m_RegenTimeEdit;
	CButton	m_ListViewBtn;
	CStatic	m_GroupStatic;
	CSpinButtonCtrl	m_GroupSpin;
	CStatic	m_GroupPropertyStatic;
	CSpinButtonCtrl	m_GroupPropertySpin;
	CEdit	m_GroupPropertyEdit;
	CStatic	m_GroupNameStatic;
	CEdit	m_GroupNameEdit;
	CEdit	m_GroupEdit;
	CStatic	m_ConditionStatic;
	CSpinButtonCtrl	m_ConditionSpin;
	CEdit	m_ConditionEdit;
	CButton	m_ConditionAllOKBtn;
	CButton	m_AllConditionGroupBox;
	CButton	m_Condition2GroupBox;
	CButton	m_Condition1GroupBox;
	CComboBox	m_MonsterKind;
	CComboBox	m_FuncKind;
	CScrollBar	m_ImageHScroll;
	CScrollBar	m_ImageVScroll;
	CListCtrl	m_MListCtrl;
	CComboBox	m_MapKind;
	CComboBox	m_NpcKind;
	CButton	m_ZoomOutBtn;
	CButton	m_ZoomInBtn;
	CButton	m_SelectBtn;
	CButton	m_MSelectNoAreaBtn;
	CButton	m_MSelectAreaBtn;
	CButton	m_MoveBtn;
	CButton	m_DeleteBtn;
	CStatic	m_MapImage;
	CString	m_MapNameStr;
	//}}AFX_DATA
	
// Attributes
public:
	CRegenToolDoc* GetDocument();
	RECT m_OriginRect;
	RECT m_ExtRect;
	RECT m_CtrlRect[46];
// Operations
public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegenToolView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetButtonCheck(int button);
	BOOL GetStartSelectFlag(){return m_StartSelectFlag;}
	void SetStartSelectFlag(BOOL flag){m_StartSelectFlag = flag;}
	void AddMonsterList(MData* mdata);
	void LoadMapImage(CString filename);
	void OnDrawBox();
	void BMPButtonImageSetting(char* filename, LPSTR button);
	void InitEngine();
	CRect* GetViewWinRect(){return &m_ViewWinSize;}
	CRect* GetImageRect(){return &m_ImageSize;}
	void InsertMonster(VECTOR3* TargetPos);
	void DeleteMonster(CPoint point);
	POINT* CalcSelectPoint(CPoint point);
	void AddListIndex(){m_ListIndex++;}
	void LoadDataInsert(CString filename);
	void SetWorkPath(CString path){m_WorkPath = path;}
	CString GetWorkPath(){return m_WorkPath;}
	void InitTile();
	CTile* GetViewTile()	{	return m_ViewTile;	}
	void InitMonsterIndex(){m_dwMonsterIndex = START_MONSTERINDEX;}
	DWORD GetMonsterIndex(){return m_dwMonsterIndex;}
	void AddMonsterIndex(){m_dwMonsterIndex++;}
	void ReleaseMonsterIndex(){m_dwMonsterIndex--;}

	int GetMonsterGrade(){return m_MonsterGrade;}
	virtual ~CRegenToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRegenToolView)
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMapimage();
	afx_msg void OnSelchangeNpccombo();
	afx_msg void OnSelchangeMapcombo();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelectChk();
	afx_msg void OnMselectnoareaChk();
	afx_msg void OnMselectareaChk();
	afx_msg void OnDeleteChk();
	afx_msg void OnZoominChk();
	afx_msg void OnZoomoutChk();
	afx_msg void OnMoveChk();
	afx_msg void OnFileSave();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeFunccombo();
	afx_msg void OnConditionallok();
	afx_msg void OnListview();
	afx_msg void OnAddcondition();
	afx_msg void OnSelchangeMoncombo();
	afx_msg void OnKillfocusGroupedit();
	afx_msg void OnGrade1();
	afx_msg void OnGrade2();
	afx_msg void OnGrade3();
	afx_msg void OnLoadmapbtn();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RegenToolView.cpp
inline CRegenToolDoc* CRegenToolView::GetDocument()
   { return (CRegenToolDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGENTOOLVIEW_H__A67B17DC_FC0B_41D3_A0DA_5E93FC2CB687__INCLUDED_)
