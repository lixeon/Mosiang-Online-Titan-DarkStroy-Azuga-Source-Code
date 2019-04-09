// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__5B2C3DE7_EF1C_408D_BBE8_FEDBC84CA011__INCLUDED_)
#define AFX_MAINFRM_H__5B2C3DE7_EF1C_408D_BBE8_FEDBC84CA011__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "MySplitter.h"
#include "DlgBarHField.h"
#include "DlgBarTool.h"

#include	"../4DyuchiGRX_common/typedef.h"

class CMainFrame : public CFrameWnd
{
	HBITMAP			m_hBmpSelectButton;
	HBITMAP			m_hBmpPickColor;
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:
	CDlgBarHField*	GetDlgBalHField()	{ return &m_wndDlgBar2; }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);		
	//}}AFX_VIRTUAL

//trustpak
public:
	void	DisplayCurrentMapFileName(void);

// Implementation
public:
	void GetEditMaxMin( MAABB* pWorldBox);
	void SetEditMaxMin( MAABB* pWorldBox);
	HWND	GetStatusBarWnd()	{return  m_wndDlgBarStatus.m_hWnd;}
	void	DockControl();
	void	SetTilePos(DWORD dwPosX,DWORD dwPosZ);
	void	OnHFieldAlphaMapMenu(CPoint* point);

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	

	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
//	CStatusBar  m_wndStatusBar;
	CDlgBarTool		m_wndDlgBar;
	CDlgBarHField	m_wndDlgBar2;
	CDialogBar		m_wndDlgBarStatus;
	CMenu			m_menuHFieldAlphaMap;

	
	CMySplitter	m_wndRightSplitter;		
	BOOL		m_bSplitterSize;
	int			m_iRightMenuSize;
	int			m_iBottomMenuSize;
	
//	CDlgBrushManager	m_dlgBarBrushManager;
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNewHfield();
	afx_msg void OnExportHfield();
	afx_msg void OnImportHfield();
	afx_msg void OnSampleFromMod();
	afx_msg void OnCreateStaticModelFromMod();
	afx_msg void OnBuildMap();
	afx_msg void OnNewStaticModel();
	afx_msg void OnExportStaticModel();
	afx_msg void OnImportStaticModel();
	afx_msg void OnBeginCreateStaticModel();
	afx_msg void OnEndCreateStaticModel();
	afx_msg void OnViewTilePalette();
	afx_msg void OnExportTileTable();
	afx_msg void OnMenuBoundingbox();
	afx_msg void OnNewMap();
	afx_msg void OnDeleteAllGxlights();
	afx_msg void OnDeleteAllGxobjects();
	afx_msg void OnSaveMap();
	afx_msg void OnLoadMap();
	afx_msg void OnCreateLightFromMod();
	afx_msg void OnSaveAllLights();
	afx_msg void OnLoadLights();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCallHelp();
	afx_msg void OnMenuitemHfieldPropertyChange();
	afx_msg void OnLoadViewonlyHfield();
	afx_msg void OnViewOnlyHfield();
	afx_msg void OnToggleOnlyLighttexture();
	afx_msg void OnSaveAllGxobjects();
	afx_msg void OnLoadGxobjects();
	afx_msg void OnViewCollisionStm();
	afx_msg void OnMenuitemClearAlphamapTileindex();
	afx_msg void OnMenuitemClearAlphamapObject();
	afx_msg void OnMenuitemClearAlphamap();
	afx_msg void OnViewTileset();				
	afx_msg void OnClose();
	afx_msg void OnViewCameraInfo();
	afx_msg void OnViewLoadFailedTextureList();
	afx_msg void OnViewToggleToolObject();
	afx_msg void OnViewRenderStaticModel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAcceleratorPicker();
};

extern CMainFrame*	g_pMainFrame;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__5B2C3DE7_EF1C_408D_BBE8_FEDBC84CA011__INCLUDED_)
