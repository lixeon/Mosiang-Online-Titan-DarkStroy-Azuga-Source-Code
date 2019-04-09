// DialogTilePalette.cpp : implementation file
//

#include "stdafx.h"
#include "4DyuchiGXMapEditor.h"
#include "DialogTilePalette.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogTilePalette dialog
#include "tool.h"


CDialogTilePalette::CDialogTilePalette(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogTilePalette::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogTilePalette)
	//}}AFX_DATA_INIT
//	memset(this,0,sizeof(CDialogTilePalette));


	Create(CDialogTilePalette::IDD,pParent);
	
	m_iOffsetX = 0;
	m_iOffsetY = 0;

	m_pRenderer = NULL;
	m_dwNumX = 0;
	m_dwNumY = 0;
	m_fTileWidth = 0;
	m_fTileHeight = 0;
	

	m_dwTextureTableNum = 0;
	m_pTextureTable = NULL;

	m_ppSpriteTable = NULL;

	m_wSelectedTileIndex = 0;
	memset(m_szFileName,0,MAX_NAME_LEN);
	m_pTileView = NULL;
	m_dwAlphaBrushActionFlag = 0;
	m_pOutLineSprite = NULL;
	OnRadioAlphaBrushSub();	
}

void CDialogTilePalette::EnableView()
{
	ShowWindow(SW_SHOWDEFAULT);
	
}
void CDialogTilePalette::DisableView()
{
	ShowWindow(SW_HIDE);
	
}

void CDialogTilePalette::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogTilePalette)
	DDX_Control(pDX, IDC_STATIC_SELECT_TILE_NAME, m_wndSelectTileName);
	DDX_Control(pDX, IDC_SCROLLBAR1, m_scrBarHeight);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogTilePalette, CDialog)
	//{{AFX_MSG_MAP(CDialogTilePalette)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_PALETTE, OnButtonLoadPalette)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_FILL_SELECTED_TILE, OnButtonFillSelectedTile)
	ON_BN_CLICKED(IDC_BUTTON_REPLACE_TEXTURE, OnButtonReplaceTexture)
	ON_BN_CLICKED(IDC_BUTTON_ROTATE_TEXTURE, OnButtonRotateTexture)
	ON_BN_CLICKED(IDC_RADIO_ALPHA_BRUSH_ADD, OnRadioAlphaBrushAdd)
	ON_BN_CLICKED(IDC_RADIO_ALPHA_BRUSH_SUB, OnRadioAlphaBrushSub)
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CDialogTilePalette::Initialize(DWORD dwTileSize,CDialogEditEntry* pEditEntryDlg, CDialogTileSet* pTileSetDlg)
{
	//yh
	m_pEditEntryDlg = pEditEntryDlg;
	//

	//trustpak
	m_pTileSetDlg = pTileSetDlg;
	m_iScrollRange = -1;
	//

		// 렌더러 컴포넌트 생성
	HRESULT hr;

	hr = CoCreateInstance(
           CLSID_4DyuchiGXRenderer,
           NULL,
           CLSCTX_INPROC_SERVER,
           IID_4DyuchiGXRenderer,
           (void**)&m_pRenderer);

	if (hr != S_OK)
		__asm int 3

	m_fTileHeight = m_fTileWidth = (float)dwTileSize;
	 
	RECT	rect;
	rect.left = 5;
	rect.top = 5;
	rect.right = 322+5;
	rect.bottom = 704+5;

	m_pTileView = new CTileView();
	m_pTileView->Create(NULL,NULL,WS_BORDER | WS_CHILD | WS_VISIBLE,rect,this,0);
	m_pTileView->SetTilePalette(this);
	m_pTileView->GetClientRect(&m_rcClip);

	m_dwNumX = (DWORD)( (float)(m_rcClip.right - m_rcClip.left) / m_fTileWidth + 0.5f);
	m_dwNumY = (DWORD)( (float)(m_rcClip.bottom - m_rcClip.top) / m_fTileHeight + 0.5f);

	m_pRenderer->Create(m_pTileView->m_hWnd,NULL,g_pTool->GetFileStorage(),NULL);
	
	m_pRenderer->BeginRender(NULL,0xff0000ff,0);
	m_pRenderer->EndRender();
	m_pRenderer->Present(NULL);
	
	char	br[4];
	DWORD dwBright = 2;
	wsprintf(br,"%d",dwBright);
	::SetDlgItemText(m_hWnd,IDC_EDIT_ALPHA_BRUSH_BRIGHT,br);

	
	return TRUE;
}

BOOL CDialogTilePalette::GetTextureTable(TEXTURE_TABLE** ppTable,DWORD* pdwNum)
{
	if (!m_pTextureTable)
	{
		*pdwNum = 0;
		*ppTable = NULL;
		return FALSE;
	}

	*ppTable = m_pTextureTable;
	*pdwNum = m_dwTextureTableNum;
	return TRUE;
}

CDialogTilePalette::~CDialogTilePalette()
{

	CleanTextureTable();
	if (m_pRenderer)
	{
		m_pRenderer->Release();
		m_pRenderer = NULL;
	}
	if (m_pTileView)
	{
		delete m_pTileView;
		m_pTileView = NULL;
	}

}

BOOL CDialogTilePalette::LoadPalette(char* szFileName)
{
	FILE*	fp = fopen(szFileName,"rt");
	if (!fp)
		return FALSE;

	DWORD		dwNum;
	char		buf[256];

	memset(m_szFileName,0,sizeof(MAX_NAME_LEN));
	strcpy(m_szFileName,szFileName);

	CleanTextureTable();


//	WORD		wIndex;
//	char*		szTextureName;

	DWORD		dwIndex;
	fscanf(fp,"%s %d",buf,&dwNum);

	m_pTextureTable = new TEXTURE_TABLE[dwNum];

	m_ppSpriteTable = new IDISpriteObject*[dwNum];
	memset(m_pTextureTable,0,sizeof(TEXTURE_TABLE)*dwNum);
	memset(m_ppSpriteTable,0,sizeof(IDISpriteObject*)*dwNum);

	m_dwTextureTableNum = dwNum;

	for (DWORD i=0; i<dwNum; i++)
	{
		fscanf(fp,"%d %s",&dwIndex,m_pTextureTable[i].szTextureName);
		m_pTextureTable[i].wIndex = (WORD)dwIndex;
	
	}
	SetPalette(m_pTextureTable,m_dwTextureTableNum);
	g_pTool->UpdateTilePalette();


	//trustpak
	g_pTool->SetCurrentTilePalleteFileName(szFileName);	

	////

	fclose(fp);
	return TRUE;
}
BOOL CDialogTilePalette::CreatePalette(DWORD dwNum)
{
	CleanTextureTable();
	m_pTextureTable = new TEXTURE_TABLE[dwNum];
	m_ppSpriteTable = new IDISpriteObject*[dwNum];

	memset(m_pTextureTable, 0, sizeof(TEXTURE_TABLE) * dwNum);
	memset(m_ppSpriteTable,0,sizeof(IDISpriteObject*)*dwNum);
	m_dwTextureTableNum = dwNum;
	return TRUE;

}

BOOL CDialogTilePalette::SetPalette(TEXTURE_TABLE* pTable,DWORD dwNum)
{
	memcpy(m_pTextureTable,pTable,sizeof(TEXTURE_TABLE)*dwNum);
	
	m_pOutLineSprite = m_pRenderer->CreateEmptySpriteObject(16,16,TEXTURE_FORMAT_R5G6B5,0);

	
	LOCKED_RECT	lockRect;
	m_pOutLineSprite->LockRect(&lockRect,NULL,TEXTURE_FORMAT_R5G6B5);
	SetOutLineBorder2Bytes((char*)lockRect.pBits,16,16,lockRect.Pitch,0xffff);

	m_pOutLineSprite->UnlockRect();
	m_pOutLineSprite->Resize(m_fTileWidth,m_fTileHeight);




	

	for (DWORD i=0; i<dwNum; i++)
	{
		m_ppSpriteTable[i] = m_pRenderer->CreateSpriteObject(pTable[i].szTextureName,SPRITE_CREATE_IMAGE);
		if (m_ppSpriteTable[i])
			m_ppSpriteTable[i]->Resize(m_fTileWidth,m_fTileHeight);
	}
	DWORD	height = dwNum / m_dwNumX;
	if ( dwNum%m_dwNumX )
		height++;

	DWORD	range = (DWORD)( (float)height*m_fTileHeight ) - (m_rcClip.bottom - m_rcClip.top);
	
	m_scrBarHeight.SetScrollRange(0,range,TRUE);
	m_iScrollRange = range;

	return TRUE;
}
void CDialogTilePalette::RenderPalette()
{
	if (!m_ppSpriteTable)
		return;

	VECTOR2	v2Pos;

	m_pRenderer->BeginRender(NULL,0xff000000,0);

	DWORD	dwCount = 0;

	DWORD	x,y;
	DWORD	dwColor;

	for (DWORD i=0; i<m_dwTextureTableNum; i++)
	{
		x = i%m_dwNumX;
		y = i/m_dwNumX;

		v2Pos.x = (float)x*m_fTileWidth - (float)m_iOffsetX;;
		v2Pos.y = (float)y*m_fTileHeight - (float)m_iOffsetY;;

		if (v2Pos.x > m_rcClip.right)
			continue;

		if (v2Pos.y > m_rcClip.bottom)
			continue;

//		if (m_wSelectedTileIndex == (WORD)dwCount)
//			dwColor = 0xccffffff;
//		else 

		dwColor = 0xffffffff;

		//yh
		if(m_pEditEntryDlg->IsShowing())
			dwColor = 0x33ffffff;
		//

		if (m_ppSpriteTable[dwCount])
			m_pRenderer->RenderSprite(m_ppSpriteTable[dwCount],NULL,0.0f,&v2Pos,NULL,dwColor,0,0);
		
		if (m_wSelectedTileIndex == (WORD)dwCount)
			m_pRenderer->RenderSprite(m_pOutLineSprite,NULL,0.0f,&v2Pos,NULL,dwColor,127,RENDER_TYPE_SPRITE_ADD);
			
		dwCount++;
	}
	
	//yh
	if(m_pEditEntryDlg->IsShowing())
	{
		for(DWORD n=0;n<4;++n)
		{
			v2Pos.x = m_rcClip.left + m_fTileWidth/2 + n*m_fTileWidth;
			v2Pos.y = m_rcClip.top + m_fTileHeight/2;
			float unitangle = 3.1415926f * -0.5f;
			DWORD texture = m_pEditEntryDlg->GetTexture(n);
			if(m_ppSpriteTable[texture])
			{
				DWORD dir = m_pEditEntryDlg->GetDirection(n);
				float rot = unitangle * dir;
				
				m_ppSpriteTable[texture]->Draw(NULL,rot,&v2Pos,NULL,0xffffffff,0);
			}
			else
			{
				//m_pEditEntryDlg->SetTexture(n,0);
			}
		}
	}
	//

	m_pRenderer->EndRender();
	m_pRenderer->Present(NULL);
}/*
void CDialogTilePalette::RenderTileView(WORD* pwIndexedTileTable,DWORD dwNumX,DWORD dwNumY,int iOffsetX,int iOffsetY,RECT* pRectClip)
{
	if (!m_pRenderer)
		return;


	if (!m_ppSpriteTable)
		return;

	VECTOR2	v2Pos;
	IDISpriteObject*	pSprite;

	m_pRenderer->BeginRender(NULL,0xff0000ff,0);

	DWORD	dwCount = 0;


	for (DWORD y=0; y<dwNumY; y++)
	{
		for (DWORD x=0; x<dwNumX; x++)
		{
			v2Pos.x = (float)x*m_fTileWidth - (float)iOffsetX;
			v2Pos.y = (float)y*m_fTileHeight - (float)iOffsetY;

			if (v2Pos.x > pRectClip->right)
				continue;

			if (v2Pos.y > pRectClip->bottom)
				continue;

			pSprite = m_ppSpriteTable[pwIndexedTileTable[y*dwNumX+x]];
lb_render:
			pSprite->Draw(NULL,NULL,0.0f,&v2Pos,0xffffffff);
		}
	}
	m_pRenderer->EndRender();
	m_pRenderer->Present(NULL);
}*/






void CDialogTilePalette::CleanTextureTable()
{

	if (m_pOutLineSprite)
	{
		m_pOutLineSprite->Release();
		m_pOutLineSprite = NULL;
	}

	if (m_pTextureTable)
	{
		delete [] m_pTextureTable;
		m_pTextureTable = NULL;
	}
	if (m_ppSpriteTable)
	{
		for (DWORD i=0; i<m_dwTextureTableNum; i++)
		{
			if (m_ppSpriteTable[i])
				m_ppSpriteTable[i]->Release();
		}

		delete [] m_ppSpriteTable;
		m_ppSpriteTable = NULL;
	}

}

void CDialogTilePalette::OnButtonRefresh() 
{
	// TODO: Add your control notification handler code here
	if (m_hWnd)
		RenderPalette();
	
}
void CDialogTilePalette::OnButtonLoadPalette() 
{
	// TODO: Add your control notification handler code here
	char szFilters[] = "Tile Palette Files (*.pal) | *.pal | All files (*.*) |*.*||";
	char* filename;
		
	CString pathname;
	CFileDialog dlg(TRUE,"pal","*.pal",NULL,szFilters,AfxGetMainWnd());

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

	LoadPalette(filename);

	//trustpak
	DisplayCurrentTilePalleteFileName();	
	m_pTileSetDlg->UpdateTexturesOfTileSetDisplayers();
	////
	RenderPalette();
}
void CDialogTilePalette::SelectTile(POINT* pPoint)
{

	// TODO: Add your control notification handler code here
	DWORD	sx,sy;
	sx = (DWORD)( (float)pPoint->x / (float)m_fTileWidth );
	sy = (DWORD)( (float)(pPoint->y+m_iOffsetY) / (float)m_fTileHeight );

	if ( (sx + sy*m_dwNumX) < m_dwTextureTableNum)
	{
		m_wSelectedTileIndex = (WORD)( sx + sy*m_dwNumX );
		
		//yh
		m_pEditEntryDlg->OnClickTileInPalette(m_wSelectedTileIndex);
		//
	}
	RenderPalette();

	//trustpak 

	DisplaySelectedTileName(m_wSelectedTileIndex);

	///


	g_pTool->ToggleTileEditMode();
}


//trustpak
VOID CDialogTilePalette::DisplaySelectedTileName(WORD wTileIndex)
{
	if (NULL != m_pTextureTable)
	{
		m_wndSelectTileName.SetWindowText(m_pTextureTable[wTileIndex].szTextureName);
	}	
}


///

void CDialogTilePalette::SetTile(DWORD dwTileIndex)
{
	if (dwTileIndex < m_dwTextureTableNum)
	{
		m_wSelectedTileIndex = dwTileIndex;
		RenderPalette();
	}
}

BOOL CDialogTilePalette::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
//	m_scrBarHeight.EnableScrollBar(ESB_ENABLE_BOTH);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogTilePalette::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	switch (nSBCode)
	{
	case SB_LINEDOWN:
//		m_iOffsetY++;
		break;
	case SB_LINEUP:
//		m_iOffsetY--;
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		m_iOffsetY = nPos;
		pScrollBar->SetScrollPos(nPos,TRUE);
		break;
	}
	RenderPalette();

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
	
}



BOOL CDialogTilePalette::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default

	int iMoveOffset = 35;
	
	if (zDelta > 0)
	{
		if (0 < m_iOffsetY)
		{
			if (m_iOffsetY <= iMoveOffset)
			{
				iMoveOffset = m_iOffsetY;
			}

			m_iOffsetY -= iMoveOffset;
			m_scrBarHeight.SetScrollPos(m_iOffsetY,TRUE);

			RenderPalette();
		}

	}
	else
	{
		if (m_iOffsetY < m_iScrollRange)
		{

			if (m_iScrollRange - m_iOffsetY <= iMoveOffset)
			{
				iMoveOffset = m_iScrollRange - m_iOffsetY;
			}


			m_iOffsetY += iMoveOffset; 			
			m_scrBarHeight.SetScrollPos(m_iOffsetY,TRUE);

			RenderPalette();
		}

	}
	
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}




void CDialogTilePalette::OnButtonFillSelectedTile() 
{
	// TODO: Add your control notification handler code here
	g_pTool->FillSelecteTile();
}

void CDialogTilePalette::OnButtonReplaceTexture() 
{
	char szFilters[] = "Targa Files(*.tga)|*.tga|Tiff Files(*.tif) |*.tif||";
//	char szFilters[] = "TGA Files(*.tga) | *.tga | TIF Files(*.tif) | *.tif||";
	
	char* filename = NULL;
		
	CString pathname;
	CFileDialog dlg(TRUE, NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES,szFilters, NULL); 
	
	
	dlg.DoModal();	
	pathname = (LPSTR)(LPCSTR)(dlg.GetPathName());
	if (pathname == "")
		return;

	if (!(filename = (LPSTR)(LPCSTR)pathname))
		return;


	if (!filename)
		return;

	

	IDISpriteObject*	pSprite;
	pSprite = m_pRenderer->CreateSpriteObject(filename,SPRITE_CREATE_IMAGE);
	if (!pSprite)
		return;

	if (m_ppSpriteTable[m_wSelectedTileIndex])
		m_ppSpriteTable[m_wSelectedTileIndex]->Release();

	m_ppSpriteTable[m_wSelectedTileIndex] = pSprite;
	m_ppSpriteTable[m_wSelectedTileIndex]->Resize(m_fTileWidth,m_fTileHeight);
	
	
	RenderPalette();
	g_pTool->ReplaceTile(filename,m_wSelectedTileIndex);
	

}

void CDialogTilePalette::OnButtonRotateTexture() 
{
	// TODO: Add your control notification handler code here
	g_pTool->RotateSelecteTile();
}

void CDialogTilePalette::OnRadioAlphaBrushAdd() 
{
	// TODO: Add your control notification handler code here
	m_dwAlphaBrushActionFlag = (m_dwAlphaBrushActionFlag & (~EDIT_ALPHA_TEXEL_OP_SUB)) | EDIT_ALPHA_TEXEL_OP_ADD;
	
}

void CDialogTilePalette::OnRadioAlphaBrushSub() 
{
	// TODO: Add your control notification handler code here
	m_dwAlphaBrushActionFlag = (m_dwAlphaBrushActionFlag & (~EDIT_ALPHA_TEXEL_OP_ADD)) | EDIT_ALPHA_TEXEL_OP_SUB;

}

DWORD CDialogTilePalette::GetAlphaBrushActionFlag()
{
	char	br[4];
	::GetDlgItemText(m_hWnd,IDC_EDIT_ALPHA_BRUSH_BRIGHT,br,2);
	DWORD dwBright = (DWORD)atoi(br);
	if (dwBright > 3)
	{
		dwBright = 3;
		wsprintf(br,"%d",dwBright);
		::SetDlgItemText(m_hWnd,IDC_EDIT_ALPHA_BRUSH_BRIGHT,br);
	}
	return (m_dwAlphaBrushActionFlag | dwBright);

}


//trustpak
void CDialogTilePalette::DisplayCurrentTilePalleteFileName(void)
{	
	char szTilePalleteName[_MAX_PATH];
	memset(szTilePalleteName, 0, sizeof(szTilePalleteName));

	g_pTool->GetCurrentTilePalleteFileName(szTilePalleteName, sizeof(szTilePalleteName));

	//어딘가 만들어 놓은 함수가 있는데 	찾기가 귀찮아서 그냥 코딩.
	char szActualFileName[_MAX_PATH];
	char szTempFileName[_MAX_PATH];

	memset(szActualFileName, 0, _MAX_PATH);
	memset(szTempFileName, 0, _MAX_PATH);

	int i = 0;
	int iResultOffset = 0;

	for (i = strlen(szTilePalleteName) -1; i >= 0; i--)
	{
		if ('\\' == szTilePalleteName[i])
		{
			break;
		}

		szTempFileName[iResultOffset++] = szTilePalleteName[i];
	}
	
	for (i = 0; i < strlen(szTempFileName); i++)
	{
		szActualFileName[i] = szTempFileName[--iResultOffset];
	}
	
	char szCaption[_MAX_PATH + 128];
	memset(szCaption, 0, sizeof(szCaption));

	strncpy(szCaption, szActualFileName, sizeof(szCaption));
	strncat(szCaption, " - Tile pallete", sizeof(szCaption));


	// 프로그램이 종료될 때 순서상 윈도우 핸들이 먼저 해제될 여지가 있기 때문에
	// 이렇게 예외처리를 해둔다.
	if (NULL != this->m_hWnd)
	{
		SetWindowText(szCaption);
	}
}


void CDialogTilePalette::OnCancel()
{
	g_pTool->ViewTilePalette();
	CDialog::OnCancel();
}


////
