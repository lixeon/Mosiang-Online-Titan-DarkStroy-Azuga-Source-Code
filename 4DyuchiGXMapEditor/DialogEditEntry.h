#if !defined(AFX_DIALOGEDITENTRY_H__3059B0D4_5207_4026_9818_E58834F5066F__INCLUDED_)
#define AFX_DIALOGEDITENTRY_H__3059B0D4_5207_4026_9818_E58834F5066F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogEditEntry.h : header file
//

class CDialogEditTileSet;
class CDialogTilePalette;

#include "TileSet.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogEditEntry dialog

class CDialogEditEntry : public CDialog
{	
	CDialogEditTileSet* m_pEditTileSetDlg;
	CDialogTilePalette* m_pPaletteDlg;

	BOOL m_bSelecting;
	DWORD m_SelectingPieceNum;

	BOOL m_bShow;
	int m_EntryNum;
	CTileSetEntry m_Entry;
	
	void SelectTile(DWORD PieceNum);
	void RotateTile(DWORD PieceNum);
	void ChangeProbability(DWORD PieceNum);

// Construction
public:
	CDialogEditEntry(CWnd* pParent = NULL);   // standard constructor

	void Initialize(CDialogEditTileSet* pEditTileSetDlg,CDialogTilePalette* pPaletteDlg);
	void EnableView();
	void DisableView();

	inline BOOL IsShowing()	{	return m_bShow;	}

	void InitEntry(DWORD Entrynum,CTileSetEntry* pEntry);

	void OnClickTileInPalette(DWORD TextureNum);

	DWORD GetTexture(DWORD PieceNum);
	DWORD GetDirection(DWORD PieceNum);

	DWORD*	m_Probability[4];
	DWORD	m_Probability0;
	DWORD	m_Probability1;
	DWORD	m_Probability2;
	DWORD	m_Probability3;
	
// Dialog Data
	//{{AFX_DATA(CDialogEditEntry)
	enum { IDD = IDD_EDITTILEENTRY };
	CButton	m_ShowBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogEditEntry)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogEditEntry)
	afx_msg void OnSelecttile1();
	afx_msg void OnSelecttile2();
	afx_msg void OnSelecttile3();
	afx_msg void OnSelecttile4();
	afx_msg void OnChangedirection1();
	afx_msg void OnChangedirection2();
	afx_msg void OnChangedirection3();
	afx_msg void OnChangedirection4();
	afx_msg void OnShow();
	afx_msg void OnOk();
	virtual void OnCancel();
	afx_msg void OnChangeProbability1();
	afx_msg void OnChangeProbability2();
	afx_msg void OnChangeProbability3();
	afx_msg void OnChangeProbability4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGEDITENTRY_H__3059B0D4_5207_4026_9818_E58834F5066F__INCLUDED_)
