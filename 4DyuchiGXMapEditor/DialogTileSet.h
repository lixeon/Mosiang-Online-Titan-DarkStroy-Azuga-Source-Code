#if !defined(AFX_DIALOGTILESET_H__681E2A47_0042_4742_94A9_D0CDEBA532D2__INCLUDED_)
#define AFX_DIALOGTILESET_H__681E2A47_0042_4742_94A9_D0CDEBA532D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogTileSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogTileSet dialog

#include "voidList.h"
#include "../4DyuchiGRX_Common/IRenderer.h"


class CTileSet;
class CDialogTilePalette;
class CDialogEditTileSet;
class CTileSetView;


//trustpak

class CTileSetDisplayer
{
public:
	CTileSetDisplayer();
	~CTileSetDisplayer();
		
	HRESULT UpdateSpriteInfo(CTileSet* pTileSet);	
	VOID	Clear(VOID);

	BOOL	UpdateWholeSprites(I4DyuchiGXRenderer* pRenderer, TEXTURE_TABLE* pTextureTable);
	BOOL	CleanWholeSprites(VOID);
	BOOL	UpdateOneSprite(I4DyuchiGXRenderer* pRenderer, TEXTURE_TABLE* pTextureTable, DWORD dwIndex);
	BOOL	CleanOneSprite(DWORD dwIndex);

	inline CString& GetName()	{	return m_strTileSetName;	}
	void SetName(CString& str)  { m_strTileSetName = str; }
	

	//IDISpriteObject*	GetSprite(DWORD dwIndex)		{ return m_apTileSprites[dwIndex]; 	}
	IDISpriteObject*	GetSprite(DWORD dwIndex);
	DWORD				GetDirection(DWORD dwIndex)		{ return m_awTileTextureDirection[dwIndex]; }

	// Constant
	enum { MAX_SPRITE_NUM = 14 };

private:	
	IDISpriteObject*	m_apTileSprites[CTileSetDisplayer::MAX_SPRITE_NUM];
	WORD				m_awTileTextureNumbers[CTileSetDisplayer::MAX_SPRITE_NUM];
	WORD				m_awTileTextureDirection[CTileSetDisplayer::MAX_SPRITE_NUM];	

	CString				m_strTileSetName;
};

///



class CDialogTileSet : public CDialog
{
	//trustpak
	CTileSetView*		m_pTileSetView;	

	///

	CDialogTilePalette* m_pPaletteDlg;
	CDialogEditTileSet* m_pEditTileSetDlg;
	CString m_strSelectedTileSet;
	
	CVoidList m_listTileSet;
	CVoidList m_listTileSetDisplayer;

	void Clear();

	CTileSet*			GetTileSet(CString& TileSetName);
	CTileSetDisplayer*  GetTileSetDisplayer(CString& TileSetName);
public:
	CDialogTileSet(CWnd* pParent = NULL);   // standard constructor

	//trustpak
	~CDialogTileSet();
	///

	void Initialize(CDialogTilePalette* pPaletteDlg,CDialogEditTileSet* pEditTileSetDlg);	
	void EnableView();
	void DisableView();

	inline CTileSet* GetSelectedTileSet()	{	return GetTileSet(m_strSelectedTileSet);	}

	void AddTileSet(CTileSet* pSet);

	//trustpak

	BOOL IsTilePalletLoaded(VOID);

	BOOL UpdateTexturesOfTileSetDisplayers(VOID);

	///

	//trustpak
	BOOL UpdateWholeDisplayerInfo(VOID);
	BOOL AddTileSetDisplayer(CTileSet* pTileSet);	
	void RenderTileSet(void);
	void ClearTileSetDisplayers(VOID);	

	CTileSetDisplayer* GetCurrentTileSetDisplayer(VOID) { return m_pCurrentTileSetDisplayer; }
	I4DyuchiGXRenderer* GetTileSetRenderer(VOID)		{ return m_pTileSetRenderer; }
	CDialogTilePalette* GetTilePalleteDlg(VOID)			{ return m_pPaletteDlg; }

	void DisplayCurrentTileSetFileName(void);


	void	DisplaySelectedTileTextureFileName(int iX, int iY);

	///


//trustpak
public:
	I4DyuchiGXRenderer*		m_pTileSetRenderer;
	CTileSetDisplayer*		m_pCurrentTileSetDisplayer;		
///

	
// Dialog Data
	//{{AFX_DATA(CDialogTileSet)
	enum { IDD = IDD_TILESET };
	CStatic	m_wndTextureFileName;
	CListBox	m_TileSetListBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogTileSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogTileSet)
	afx_msg void OnSelchangeList1();
	afx_msg void OnNew();
	afx_msg void OnSave();
	afx_msg void OnLoad();
	afx_msg void OnAdd();
	afx_msg void OnDelete();
	afx_msg void OnModify();			
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGTILESET_H__681E2A47_0042_4742_94A9_D0CDEBA532D2__INCLUDED_)
