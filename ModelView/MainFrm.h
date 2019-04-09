// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////


#include "ToolBar.h"
#include "ToolInfo.h"
#include "DialogBar.h"
#include "FrontOfCameraDlg.h"
#include "IllusionSetupDlg.h"
#include "LightDlg.h"


#if !defined(AFX_MAINFRM_H__D2635ACB_4936_4ED9_8B8F_C3E6CDC84C94__INCLUDED_)
#define AFX_MAINFRM_H__D2635ACB_4936_4ED9_8B8F_C3E6CDC84C94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:

	CNewToolBar			m_wndToolBar;
	CNewDialogBar		m_wndDialogBar;	

	// Dialog boxes.

	CIllusionSetupDlg*	m_pWndIllusionSetupDlg;
	CFrontOfCameraDlg*	m_pFrontOfCameradlg;
	CLightDlg*			m_pLightDlg;

	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();	
	afx_msg void OnModComboChange(void);
	afx_msg void OnAnmComboChange(void);	
	afx_msg void OnMtlComboChange(void);
	afx_msg void OnWavComboChange(void);
	afx_msg void OnOpenMod();
	afx_msg void OnOpenAnm();
	afx_msg void OnCloseAnm();
	afx_msg void OnCloseMod();
	afx_msg void OnFileNew();
	afx_msg void OnOepnChFile();
	afx_msg void OnPlayButton();
	afx_msg void OnPlaySoundCheck();
	afx_msg void OnStopButton();
	afx_msg void OnPlaySoundButton();
	afx_msg void OnExportChr();	
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnResetCamera();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnAllSelect();
	afx_msg void OnSetFrame();
	afx_msg void OnLightDialog();
	afx_msg void OnOpenMtl();
	afx_msg void OnCloseMtl();	
	afx_msg BOOL OnQueryEndSession();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnUpdateGridDisplay(CCmdUI* pCmdUI);	
	afx_msg void OnGridDisplay();
	afx_msg void OnUpdateBoundingDraw(CCmdUI* pCmdUI);
	afx_msg void OnBoundingDraw();
	afx_msg void OnBoneBoundingDraw();
	afx_msg void OnUpdateBoneBoundingDraw(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawWireFrame(CCmdUI* pCmdUI);
	afx_msg void OnWiremode();
	afx_msg void OnRenderCameraInfo();
	afx_msg void OnUpdateRenderCameraInfo(CCmdUI* pCmdUI);
	afx_msg void OnOpenWav();
	afx_msg void OnCloseWav();
	afx_msg void OnWavListClear();
	afx_msg void OnSpecularSetting();
	afx_msg void OnIlluminationLoad();
	afx_msg void OnIlluminationSetup();
	afx_msg void OnIlluminationUnload();
	afx_msg void OnSetBackgroundColor();
	afx_msg void OnViewEffectPalette();
	afx_msg void OnFrontOfCamera();			
	afx_msg void OnBothmode();
	afx_msg void OnUpdateBothmode(CCmdUI* pCmdUI);
	afx_msg LRESULT OnFrontOfCameraDlgDestroy(WPARAM,LPARAM);
	afx_msg LRESULT OnLightDialogDestroy(WPARAM,LPARAM);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__D2635ACB_4936_4ED9_8B8F_C3E6CDC84C94__INCLUDED_)
