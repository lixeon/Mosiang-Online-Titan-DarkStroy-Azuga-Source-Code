//{{AFX_INCLUDES()
//}}AFX_INCLUDES
#if !defined(AFX_DIALOGTILEPALETTE_H__C9A47657_1E74_43C0_B88F_65A26211344A__INCLUDED_)
#define AFX_DIALOGTILEPALETTE_H__C9A47657_1E74_43C0_B88F_65A26211344A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogTilePalette.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogTilePalette dialog
#include "TileView.h"
#include "DialogTileSet.h"
#include "../4DyuchiGRX_Common/IRenderer.h"

class CDialogEditEntry;

class CDialogTilePalette : public CDialog
{	
	I4DyuchiGXRenderer*		m_pRenderer;
	DWORD					m_dwNumX;
	DWORD					m_dwNumY;
	float					m_fTileWidth;
	float					m_fTileHeight;	

	DWORD					m_dwTextureTableNum;
	TEXTURE_TABLE*			m_pTextureTable;

	IDISpriteObject**		m_ppSpriteTable;
	IDISpriteObject*		m_pOutLineSprite;

	WORD					m_wSelectedTileIndex;
	char					m_szFileName[MAX_NAME_LEN];
	RECT					m_rcClip;
	int						m_iOffsetX;
	int						m_iOffsetY;
	DWORD					m_dwAlphaBrushActionFlag;


	CTileView*				m_pTileView;

	//yh
	CDialogEditEntry*		m_pEditEntryDlg;
	//

	//trustpak

	CDialogTileSet*			m_pTileSetDlg;
	INT						m_iScrollRange;

	VOID					DisplaySelectedTileName(WORD wTileIndex);

	//



public:
	BOOL					LoadPalette(char* szFileName);
	BOOL					CreatePalette(DWORD dwNum);
	BOOL					SetPalette(TEXTURE_TABLE* pTable,DWORD dwNum);
	void					SetTile(DWORD dwTileIndex);

	void					CleanTextureTable();
	void					RenderPalette();
	
//	void					RenderTileView(WORD* pwIndexedTileTable,DWORD dwNumX,DWORD dwNumY,int iOffsetX,int iOffsetY,RECT* pRectClip);
	
	//BOOL					Initialize(DWORD dwTileSize,CDialogEditEntry* pEditEntryDlg);
	//trustpak
	BOOL					Initialize(DWORD dwTileSize,CDialogEditEntry* pEditEntryDlg, CDialogTileSet* pTileSetDlg);
	///
	BOOL					GetTextureTable(TEXTURE_TABLE** ppTable,DWORD* pdwNum);
	char*					GetFileName() {return m_szFileName;}
	void					SelectTile(POINT* pPoint);

	WORD					GetCurrentTileIndex() {return m_wSelectedTileIndex;}
	DWORD					GetAlphaBrushActionFlag();

	IDISpriteObject*		GetSprite(DWORD dwIndex) {return m_ppSpriteTable[dwIndex];}


	CDialogTilePalette(CWnd* pParent = NULL);   // standard constructor
	~CDialogTilePalette();

	void					EnableView();
	void					DisableView();


	//trustpak

	void					DisplayCurrentTilePalleteFileName(void);
	////

//	DWORD					SaveTilePalette(FILE* fp);


// Dialog Data
	//{{AFX_DATA(CDialogTilePalette)
	enum { IDD = IDD_DIALOG1 };
	CStatic	m_wndSelectTileName;
	CScrollBar	m_scrBarHeight;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogTilePalette)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnCancel();

	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogTilePalette)
	afx_msg void OnButtonRefresh();
	afx_msg void OnButtonLoadPalette();
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnButtonFillSelectedTile();
	afx_msg void OnButtonReplaceTexture();
	afx_msg void OnButtonRotateTexture();
	afx_msg void OnRadioAlphaBrushAdd();
	afx_msg void OnRadioAlphaBrushSub();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGTILEPALETTE_H__C9A47657_1E74_43C0_B88F_65A26211344A__INCLUDED_)
