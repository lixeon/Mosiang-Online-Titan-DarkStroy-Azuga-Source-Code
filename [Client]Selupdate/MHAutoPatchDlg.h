// MHAutoPatchDlg.h : header file
//

#if !defined(AFX_MHAUTOPATCHDLG_H__C64FC497_45E4_40DC_AEB4_AB3A77ADF731__INCLUDED_)
#define AFX_MHAUTOPATCHDLG_H__C64FC497_45E4_40DC_AEB4_AB3A77ADF731__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========
#include "HtmlViewNotice.h"
#include "SkinProgressBar.h"
#include "SkinStatic.h"
//===========
#include "BGMManager.h"
//===========


/////////////////////////////////////////////////////////////////////////////
// CMHAutoPatchDlg dialog

class CMHAutoPatchDlg : public CDialog
{
	// Construction
public:
	CMHAutoPatchDlg(CWnd* pParent = NULL);	// standard constructor

	// Dialog Data
	//{{AFX_DATA(CMHAutoPatchDlg)
#ifdef _FOR_CLIENT_
	enum { IDD = IDD_MHAUTOPATCH_DIALOG };
	CStatic	m_picMain;
	CSkinStatic	m_txtHomepage;
	CSkinStatic	m_txtJoin;
	CSkinStatic	m_txtStart;
	CSkinStatic	m_txtBBS;
	CStatic	m_picTitle;
	CButton	m_btnOK;
	CButton	m_btnCancel;
	CButton	m_btnRegist;
	CButton	m_btnHomepage;
	CButton	m_btnBBS;
	CButton	m_btnWindowMode;
	CSkinStatic m_stcWindowMode;

#else
	enum { IDD = IDD_MHSERVERPATCH_DIALOG };
	CButton	m_btnCancel;
#endif
	//}}AFX_DATA


	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMHAutoPatchDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL


	// Implementation
public:
	void QuitPatchDialog( int nResult );
	BOOL Clear3DData();

	void OnConnected( LPVOID pvdata );
	void OnGetCurrentDirectory( LPVOID pvdata );
	void OnSetCurrentDirectory( LPVOID pvdata );
	void OnGetFileList( LPVOID pvdata );
	void OnConfirm( LPVOID pvdata );
	void OnDownLoad( LPVOID pvdata );

	void OnJieraDownload();

	BOOL RunMH();
	void OnRunProgram();

	BOOL Extract_DownloadFile( char* lpFileName);
	BOOL UpdateVersionInfoFile( char* lpszOldVerInfoFile, char* lpszNewVerInfoFile );

	void StartPack3DData();

	void SetReadyToStart();

	static BOOL SelectGFunc();
	static BOOL InitEngine();
	static void Pack3DData( void* p );

	void SetStatusText( char* pText );
	void SetProgressBar( DWORD dwLv );

#ifdef _FOR_CLIENT_
	//----Option / window-mode
	BOOL LoadOption();
	BOOL SaveOption();
	//---
#endif

#ifdef _MANUAL_PATCH_
	void Pack_N_Extract();
#endif

#ifdef _FOR_CLIENT_
	void SelectTitlePicture();
	CSkinProgressBar* GetProgressBar() { return m_pProgressBar;	}
#endif

protected:
	HICON m_hIcon;

	static HWND m_sHWnd;
	BOOL m_bCanStart;

	BOOL m_bVersionChecking;

	int		m_nRetryCount;


#ifdef _FOR_CLIENT_

	//===========
	CHtmlViewNotice*	m_pHtmlViewNotice;
	CSkinProgressBar*	m_pProgressBar;
	CSkinStatic*		m_pStatus;
	CRect				m_rcStatus;
	//===========
	CBGMManager			m_BGMManager;
	//===========
	HBITMAP				m_hBmpMain;
	HBITMAP				m_hBmpTitle;
	HBITMAP				m_hBmpNBtn;//ÆÕÍ¨ÎÄ×Ö°´Å¥±³¾°Í¼Æ¬
	HBITMAP				m_hBmpNBtnPress;
	HBITMAP				m_hBmpEBtn;//ÍË³öX°´Å¥Í¼Æ¬
	HBITMAP				m_hBmpEBtnPress;

	HBITMAP				m_hBmpPlayBtn;//¿ªÊ¼ÓÎÏ·°´Å¥Í¼Æ¬
	HBITMAP				m_hBmpPlayPress;

	HBITMAP				m_hBmpRegBtn;//×¢²á°´Å¥Í¼Æ¬
	HBITMAP				m_hBmpRegPress;
	HBITMAP				m_hBmpHomeBtn;//Ö÷Ò³°´Å¥Í¼Æ¬
	HBITMAP				m_hBmpHomeBtnPress;
	HBITMAP				m_hBmpBaoBtn;//°Ù±¦°´Å¥Í¼Æ¬
	HBITMAP				m_hBmpBaoBtnPress;

	HBITMAP				m_hBmpCB;//¼ì²éÊÇ·ñÈ«ÆÁ°´Å¥Í¼Æ¬
	HBITMAP				m_hBmpCBCheck;

	BOOL				m_bWindowModeChecked;
	//===========
#endif

	// Generated message map functions
	//{{AFX_MSG(CMHAutoPatchDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
#ifdef _FOR_CLIENT_
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnHomepage();
	afx_msg void OnRegist();
	afx_msg void OnBBS();
#endif
	virtual void OnCancel();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	//	afx_msg void OnBnClickedWindowmode();
	void SetStart(void);
	//	afx_msg void OnBnClickedCancel();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MHAUTOPATCHDLG_H__C64FC497_45E4_40DC_AEB4_AB3A77ADF731__INCLUDED_)
