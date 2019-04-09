#if !defined(AFX_DIALOGEDITTILESET_H__6045C5DF_5FC6_4748_B2CA_DB77A6CFF65B__INCLUDED_)
#define AFX_DIALOGEDITTILESET_H__6045C5DF_5FC6_4748_B2CA_DB77A6CFF65B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogEditTileSet.h : header file
//

#include "TileSet.h"
#include "TileView.h"

class CDialogTileSet;
class CDialogEditEntry;

/////////////////////////////////////////////////////////////////////////////
// CDialogEditTileSet dialog

class CDialogEditTileSet : public CDialog
{
	CDialogTileSet* m_pTileSetDlg;
	CDialogEditEntry* m_pEditEntryDlg;
	CTileSet m_TileSet;
	
// Construction
public:
	CDialogEditTileSet(CWnd* pParent = NULL);   // standard constructor

	void Initialize(CDialogTileSet* pTileSetDlg,CDialogEditEntry* pEditEntryDlg);
	void EnableView();
	void DisableView();

	void InitTileSet(CTileSet* pSet);

	void OnEditEntry(int n);

	void SetEntry(DWORD EntryNum,CTileSetEntry* pEntry);
public:

// Dialog Data
	//{{AFX_DATA(CDialogEditTileSet)
	enum { IDD = IDD_TILESETEDIT };
	CString	m_TileSetName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogEditTileSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUALAFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogEditTileSet)
	afx_msg void OnEntry1();
	afx_msg void OnEntry2();
	afx_msg void OnEntry3();
	afx_msg void OnEntry4();
	afx_msg void OnEntry5();
	afx_msg void OnEntry6();
	afx_msg void OnEntry7();
	afx_msg void OnEntry8();
	afx_msg void OnEntry9();
	afx_msg void OnEntry10();
	afx_msg void OnEntry11();
	afx_msg void OnEntry12();
	afx_msg void OnEntry13();
	afx_msg void OnChangeTilesetname();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnEntry0();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.



#endif // !defined(AFX_DIALOGEDITTILESET_H__6045C5DF_5FC6_4748_B2CA_DB77A6CFF65B__INCLUDED_)
