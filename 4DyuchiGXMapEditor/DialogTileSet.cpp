// DialogTileSet.cpp : implementation file
//

#include "stdafx.h"
#include "4dyuchigxmapeditor.h"
#include "TileSetView.h"
#include "DialogTileSet.h"
#include "DialogTilePalette.h"
#include "DialogEditTileSet.h"
#include "TileSetPiece.h"
#include "TileSet.h"
#include "assert.h"
#include "Tool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




// 다이얼로그에 더미 윈도우들를 만들어 그 위치들을 얻어내서 찍는 방법도 있지만
// 속도 측면에서는 테이블 참조가 제일 빠르다. 좋은게 좋은거 아니겠어? 
// set values of position table .
// 
//             0
//         1   2   3
//         4   5   6
//         7   8   9
//          10  11
//          12  13
//

const VECTOR2 c_SpritePosition[CTileSetDisplayer::MAX_SPRITE_NUM] =
{
	{ 96.0f, 10.0f },

	{ 32.0f, 64.0f + 20.0f }, 
	{ 96.0f, 64.0f + 20.0f },
	{ 160.0f, 64.0f + 20.0f },

	{ 32.0f, 128.0f + 20.0f },
	{ 96.0f, 128.0f + 20.0f },
	{ 160.0f, 128.0f + 20.0f },

	{ 32.0f, 192.0f + 20.0f },
	{ 96.0f, 192.0f + 20.0f },
	{ 160.0f, 192.0f + 20.0f },

	{ 64.0f, 256.0f + 30.0f },
	{ 128.0f, 256.0f + 30.0f },

	{ 64.0f, 320.0f + 30.0f },
	{ 128.0f, 320.0f + 30.0f }
};



/////////////////////////////////////////////////////////////////////////////
// CDialogTileSet dialog


CDialogTileSet::CDialogTileSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogTileSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogTileSet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	
	Create(CDialogTileSet::IDD,pParent);

	m_pTileSetView				= NULL;
	m_pCurrentTileSetDisplayer	= NULL;

}

CDialogTileSet::~CDialogTileSet()
{
	ClearTileSetDisplayers();

	if (NULL != m_pTileSetRenderer)
	{
		m_pTileSetRenderer->Release();
		m_pTileSetRenderer = NULL;
	}

	if (NULL != m_pTileSetView)
	{
		delete m_pTileSetView;
		m_pTileSetView = NULL;
	}

}

void CDialogTileSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogTileSet)
	DDX_Control(pDX, IDC_STATIC_TEXTURE_FILE_NAME, m_wndTextureFileName);
	DDX_Control(pDX, IDC_LIST1, m_TileSetListBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogTileSet, CDialog)
	//{{AFX_MSG_MAP(CDialogTileSet)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_BN_CLICKED(IDC_NEW, OnNew)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_LOAD, OnLoad)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_Modify, OnModify)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogTileSet message handlers

void CDialogTileSet::OnSelchangeList1() 
{
	// TODO: Add your control notification handler code here
	int n = m_TileSetListBox.GetCurSel();
	if(n == LB_ERR)
		return;
	
	m_TileSetListBox.GetText(n,m_strSelectedTileSet);

	m_pCurrentTileSetDisplayer = GetTileSetDisplayer(m_strSelectedTileSet);
	
	RenderTileSet();

	g_pTool->ToggleTileSetEditMode();

}

void CDialogTileSet::Clear()
{	
	m_strSelectedTileSet = "";
	/*
	CTileSet* pTileSet;
	POSITION_ pos = m_listTileSet.GetHeadPosition();
	while(pTileSet = (CTileSet*)m_listTileSet.GetNext(pos))
	{
		delete pTileSet;
	}
	*/
	m_listTileSet.RemoveAll();
	m_TileSetListBox.ResetContent();

	ClearTileSetDisplayers();
}


void CDialogTileSet::ClearTileSetDisplayers(VOID)
{

	CTileSetDisplayer* pTileSetDisplayer = NULL;
	POSITION_ pos = m_listTileSetDisplayer.GetHeadPosition();

	while (pos)
	{
		pTileSetDisplayer = (CTileSetDisplayer*)m_listTileSetDisplayer.GetNext(pos);

		if (NULL != pTileSetDisplayer)
		{
			pTileSetDisplayer->Clear();
			pTileSetDisplayer = NULL;			
		}
	}

	m_listTileSetDisplayer.RemoveAll();

	m_pCurrentTileSetDisplayer = NULL;
}

void CDialogTileSet::OnNew() 
{
	// TODO: Add your control notification handler code here
	if(MessageBox("정말 초기화 하시겠습니까?",NULL,MB_YESNO) != IDYES)
		return;

	Clear();

	g_pTool->SetCurrentTileSetFileName("");
	DisplayCurrentTileSetFileName();
}

void CDialogTileSet::OnSave() 
{
	char szFilters[] = "Tile Set Files (*.tst) | *.tst | All files (*.*) |*.*||";
	char* filename;
		
	CString pathname;
	CFileDialog dlg(FALSE,"tst","*.tst",NULL,szFilters,AfxGetMainWnd());

	if (dlg.DoModal() != IDOK)
		return;

	pathname = (LPSTR)(LPCSTR)(dlg.GetPathName());
	if (pathname == "")
		return;
		
	// TODO: Add your control notification handler code here
	FILE* fp = fopen(pathname,"w");

	fprintf(fp,"%d\n",m_listTileSet.GetCount());
	
	CTileSet*			pTileSet = NULL;;	

	POSITION_ pos = m_listTileSet.GetHeadPosition();
	while(pTileSet = (CTileSet*)m_listTileSet.GetNext(pos))
	{
		pTileSet->Save(fp);		

	}
	fclose(fp);	
}

void CDialogTileSet::OnLoad() 
{
	if (FALSE == IsTilePalletLoaded())
	{
		MessageBox("타일 팔렛이 로드되어 있지 않습니다. 먼저 타일 팔렛을 로드해 주세요.\n");
		return;
	}

	// TODO: Add your control notification handler code here
	char szFilters[] = "Tile Set Files (*.tst) | *.tst | All files (*.*) |*.*||";
	char* filename;
		
	CString pathname;
	CFileDialog dlg(TRUE,"tst","*.tst",NULL,szFilters,AfxGetMainWnd());

	if (dlg.DoModal() != IDOK)
		return;

	pathname = (LPSTR)(LPCSTR)(dlg.GetPathName());
	if (pathname == "")
		return;

	FILE* fp = fopen(pathname,"r");
	if(fp == NULL)
		return;

	Clear();

	int TotalCount;
	fscanf(fp,"%d",&TotalCount);
	char temp[256];
	fgets(temp,256,fp);
	for(int n=0;n<TotalCount;++n)
	{
		CTileSet NewSet;
		NewSet.Load(fp);
		AddTileSet(&NewSet);

		AddTileSetDisplayer(&NewSet);
	}

	RenderTileSet();
	g_pTool->SetCurrentTileSetFileName((LPSTR)(LPCSTR)pathname);
	DisplayCurrentTileSetFileName();

	fclose(fp);	
}

void CDialogTileSet::OnAdd() 
{
	if (FALSE == IsTilePalletLoaded())
	{
		MessageBox("타일 팔렛이 로드되어 있지 않습니다. 먼저 타일 팔렛을 로드해 주세요.\n");
		return;
	}

	// TODO: Add your control notification handler code here
	CTileSet temp;		// Cleared In Consructor
	CString NewName = "NewTileSet";
	temp.SetName(NewName);
	m_pEditTileSetDlg->InitTileSet(&temp);
	m_pEditTileSetDlg->EnableView();
}

void CDialogTileSet::OnDelete() 
{
	// TODO: Add your control notification handler code here
	CTileSet* pTileSet = GetSelectedTileSet();
	if(pTileSet == NULL)
		return;
	int pos = m_TileSetListBox.FindString(0,pTileSet->GetName());
	ASSERT(pos != LB_ERR);
	m_TileSetListBox.DeleteString(pos);

	POSITION_ ppos = m_listTileSet.Find(pTileSet);
	if(ppos)
		m_listTileSet.RemoveAt(ppos);


	if (NULL != m_pCurrentTileSetDisplayer)
	{
		POSITION_ posTileSetDisplayer = m_listTileSetDisplayer.Find(m_pCurrentTileSetDisplayer);
		if (NULL != posTileSetDisplayer)
		{
			m_listTileSetDisplayer.RemoveAt(posTileSetDisplayer);			
		}

		m_pCurrentTileSetDisplayer = NULL;
	}
}

void CDialogTileSet::OnModify() 
{
	// TODO: Add your control notification handler code here
	CTileSet* pTileSet = GetTileSet(m_strSelectedTileSet);

	if (NULL == pTileSet)
	{
		return;
	}

	m_pEditTileSetDlg->InitTileSet(pTileSet);
	m_pEditTileSetDlg->EnableView();
}

void CDialogTileSet::Initialize(CDialogTilePalette* pPaletteDlg,CDialogEditTileSet* pEditTileSetDlg)
{
	m_pPaletteDlg = pPaletteDlg;
	m_pEditTileSetDlg = pEditTileSetDlg;	


	//trustpak

	// Initialize renderer of Displaying tileset.
	HRESULT hr;

	hr = CoCreateInstance(
           CLSID_4DyuchiGXRenderer,
           NULL,
           CLSCTX_INPROC_SERVER,
           IID_4DyuchiGXRenderer,
           (void**)&m_pTileSetRenderer);

	if (hr != S_OK)
		__asm int 3


	RECT rect = { 300, 20, 560, 460 };

	m_pTileSetView = new CTileSetView;
	m_pTileSetView->Create(NULL,NULL,WS_BORDER | WS_CHILD | WS_VISIBLE,rect,this,0);

	m_pTileSetView->m_pDlgTileSet = this;

	m_pTileSetRenderer->Create(m_pTileSetView->m_hWnd, NULL, g_pTool->GetFileStorage(), NULL);
		
	m_pTileSetRenderer->BeginRender(NULL,0xff000000,0);
	m_pTileSetRenderer->EndRender();
	m_pTileSetRenderer->Present(NULL);	
	///
	

}
void CDialogTileSet::EnableView()
{
	ShowWindow(SW_SHOWDEFAULT);	
}
void CDialogTileSet::DisableView()
{
	ShowWindow(SW_HIDE);	
}

CTileSet* CDialogTileSet::GetTileSet(CString& TileSetName)
{
	CTileSet* pTileSet;
	POSITION_ pos = m_listTileSet.GetHeadPosition();
	while(pTileSet = (CTileSet*)m_listTileSet.GetNext(pos))
	{
		if(pTileSet->GetName() == TileSetName)
			return pTileSet;
	}
	return NULL;
}

//trustpak
CTileSetDisplayer*  CDialogTileSet::GetTileSetDisplayer(CString& TileSetName)
{
	CTileSetDisplayer* pTileSetDisplayer = NULL;
	POSITION_ pos = m_listTileSetDisplayer.GetHeadPosition();

	while (pTileSetDisplayer = (CTileSetDisplayer*)m_listTileSetDisplayer.GetNext(pos))
	{
		if (TileSetName == pTileSetDisplayer->GetName())
		{
			return pTileSetDisplayer;
		}
	}

	return NULL;
}


///

void CDialogTileSet::AddTileSet(CTileSet* pSet)
{
	CTileSet* pPrevSet = GetTileSet(pSet->GetName());
	if(pPrevSet == NULL)
	{
		CTileSet* pNewSet = new CTileSet;
		pNewSet->CopyFrom(pSet);
		m_listTileSet.AddTail(pNewSet);
		m_TileSetListBox.AddString(pNewSet->GetName());
	}
	else
	{
		//Confirm
		if(MessageBox("정말 이 TileSet을 수정하시겠습니까?",NULL,MB_YESNO) != IDYES)
			return;

		pPrevSet->CopyFrom(pSet);		
	}
}


//trustpak
BOOL CDialogTileSet::AddTileSetDisplayer(CTileSet* pTileSet)
{
	//trustpak
	assert(pTileSet);
	
	CTileSetDisplayer* pTileSetDisplayer = GetTileSetDisplayer(pTileSet->GetName());	

	if (NULL == pTileSetDisplayer)
	{
		pTileSetDisplayer = new CTileSetDisplayer;	
		m_listTileSetDisplayer.AddTail(pTileSetDisplayer);	
	}
	

	TEXTURE_TABLE* pTextureTable = NULL;
	DWORD dwNum = 0;

	m_pPaletteDlg->GetTextureTable(&pTextureTable, &dwNum);	

	pTileSetDisplayer->UpdateSpriteInfo(pTileSet);
	pTileSetDisplayer->UpdateWholeSprites(m_pTileSetRenderer, pTextureTable);
	

	return TRUE;
}

void CDialogTileSet::RenderTileSet(void)
{
	if (NULL == m_pCurrentTileSetDisplayer)
	{
		return;
	}

	if (NULL == m_pPaletteDlg)
	{
		return;
	}

	if (NULL == m_pTileSetRenderer)
	{
		return;
	}	

	IDISpriteObject*	pSprite = NULL;	
	VECTOR2				v2SpritePos = { 0.0f, 0.0f };	

	float				fRot = 0.0f;

	m_pTileSetRenderer->BeginRender(NULL,0xff000000, 0);
	
	int i = 0;
	for (i = 0; i < CTileSetDisplayer::MAX_SPRITE_NUM; i++)
	{		
		pSprite = m_pCurrentTileSetDisplayer->GetSprite(i);
		v2SpritePos = c_SpritePosition[i];

		if (NULL != pSprite)
		{
			fRot = m_pCurrentTileSetDisplayer->GetDirection(i) * ( PI * -0.5f );

			m_pTileSetRenderer->RenderSprite(pSprite, 
											 NULL,
										     fRot,
											 &v2SpritePos,
											 NULL,
											 0xffffffff,
											 0,
											 0);
		}		
		
	}
	
	m_pTileSetRenderer->EndRender();
	m_pTileSetRenderer->Present(NULL);	
}

///


//trustpak


BOOL CDialogTileSet::IsTilePalletLoaded(VOID)
{
	TEXTURE_TABLE* pTextureTable = NULL;
	DWORD dwNum = 0;

	m_pPaletteDlg->GetTextureTable(&pTextureTable, &dwNum);	

	if (NULL == pTextureTable)
	{
		return FALSE;
	} 

	return TRUE;
}

void CDialogTileSet::DisplayCurrentTileSetFileName(void)
{
	char szFileName[_MAX_PATH];
	memset(szFileName, 0, sizeof(szFileName));

	g_pTool->GetCurrentTileSetFileName(szFileName, sizeof(szFileName));

	//어딘가 만들어 놓은 함수가 있는데 	찾기가 귀찮아서 그냥 코딩.
	char szActualFileName[_MAX_PATH];
	char szTempFileName[_MAX_PATH];

	memset(szActualFileName, 0, _MAX_PATH);
	memset(szTempFileName, 0, _MAX_PATH);

	int i = 0;
	int iResultOffset = 0;

	for (i = strlen(szFileName) -1; i >= 0; i--)
	{
		if ('\\' == szFileName[i])
		{
			break;
		}

		szTempFileName[iResultOffset++] = szFileName[i];
	}
	
	for (i = 0; i < strlen(szTempFileName); i++)
	{
		szActualFileName[i] = szTempFileName[--iResultOffset];
	}
	
	char szCaption[_MAX_PATH + 128];
	memset(szCaption, 0, sizeof(szCaption));

	strncpy(szCaption, szActualFileName, sizeof(szCaption));
	strncat(szCaption, " - Tile Set", sizeof(szCaption));


	// 프로그램이 종료될 때 순서상 윈도우 핸들이 먼저 해제될 여지가 있기 때문에
	// 이렇게 예외처리를 해둔다.
	if (NULL != this->m_hWnd)
	{
		SetWindowText(szCaption);
	}
}


void CDialogTileSet::OnCancel()
{
	g_pTool->ViewTileSet();
	CDialog::OnCancel();
}




// 텍스춰만 갱신.
BOOL CDialogTileSet::UpdateTexturesOfTileSetDisplayers(VOID)
{
	TEXTURE_TABLE* pTextureTable = NULL;
	DWORD dwNum = 0;

	m_pPaletteDlg->GetTextureTable(&pTextureTable, &dwNum);	
	
	if (NULL == pTextureTable)
	{
		return FALSE;
	}

	POSITION_ pos = m_listTileSetDisplayer.GetHeadPosition();

	while (pos)
	{
		CTileSetDisplayer* pTileSetDisplayer = (CTileSetDisplayer*)m_listTileSetDisplayer.GetNext(pos);

		if (NULL != pTileSetDisplayer)
		{
			pTileSetDisplayer->UpdateWholeSprites(m_pTileSetRenderer, pTextureTable);
		}
	}



	return TRUE;
}



// 타일셋의 정보를 새로 세팅하여 새로이 텍스춰를 갱신.
BOOL CDialogTileSet::UpdateWholeDisplayerInfo(VOID)
{
	CTileSet*				pTileSet = NULL;
	CTileSetDisplayer*		pTileSetDisplayer = NULL;

	TEXTURE_TABLE*			pTextureTable = NULL;
	DWORD					dwNum = 0;

	m_pPaletteDlg->GetTextureTable(&pTextureTable, &dwNum);

	POSITION_ pos = m_listTileSet.GetHeadPosition();
	while(pTileSet = (CTileSet*)m_listTileSet.GetNext(pos))
	{
		pTileSetDisplayer = GetTileSetDisplayer(pTileSet->GetName());

		if (NULL != pTileSetDisplayer)
		{
			pTileSetDisplayer->UpdateSpriteInfo(pTileSet);
			pTileSetDisplayer->UpdateWholeSprites(m_pTileSetRenderer, pTextureTable);
		}		
	}

	return TRUE;

}


void CDialogTileSet::DisplaySelectedTileTextureFileName(int iX, int iY)
{
	CTileSet* pTileSet = GetTileSet(m_strSelectedTileSet);

	if (NULL == pTileSet)
	{
		return;
	}

	if (NULL == m_pPaletteDlg)
	{
		return;
	}




	// Mouse 범위에 따라 현재 선택된 Entry의 번호를 구해낸다.
	// 먼저 세로 범위의 값에 따라 최상단인지 중단인지 하단인지를 구해내고
	// 그에 따라 가로의 범위를 구해낸다. 

	const int		c_iSpriteSize = 64;
	int				iSelectedPeiceNum = -1;

	// 최상단. 
	if (iY >= c_SpritePosition[0].y &&	iY <= c_SpritePosition[0].y + c_iSpriteSize)
	{
		if (iX >= c_SpritePosition[0].x && iX <= c_SpritePosition[0].x + c_iSpriteSize)
		{
			iSelectedPeiceNum = 0;
		}
	}

	// 중단 첫째 줄 
	else if ( iY >= c_SpritePosition[1].y && iY <= c_SpritePosition[1].y + c_iSpriteSize )
	{
		int iSeprate = 0;
		int iAbsXpos = iX - c_SpritePosition[1].x;

		iSeprate = iAbsXpos / c_iSpriteSize;

		if (2 >= iSeprate)
		{
			iSelectedPeiceNum = 1 + iSeprate;
		}
	}
	// 중단 둘째 줄
	else if ( iY >= c_SpritePosition[4].y && iY <= c_SpritePosition[4].y + c_iSpriteSize )
	{
		int iSeprate = 0;
		int iAbsXpos = iX - c_SpritePosition[4].x;

		iSeprate = iAbsXpos / c_iSpriteSize;

		if (2 >= iSeprate)
		{
			iSelectedPeiceNum = 4 + iSeprate;
		}
	}
	// 중단 셋째 줄
	else if ( iY >= c_SpritePosition[7].y && iY <= c_SpritePosition[7].y + c_iSpriteSize )
	{
		int iSeprate = 0;
		int iAbsXpos = iX - c_SpritePosition[7].x;

		iSeprate = iAbsXpos / c_iSpriteSize;

		if (2 >= iSeprate)
		{
			iSelectedPeiceNum = 7 + iSeprate;
		}
	}


	// 하단 첫째 줄
	else if ( iY >= c_SpritePosition[10].y && iY <= c_SpritePosition[10].y + c_iSpriteSize )
	{
		int iSeprate = 0;
		int iAbsXpos = iX - c_SpritePosition[10].x;

		iSeprate = iAbsXpos / c_iSpriteSize;

		if (1 >= iSeprate)
		{
			iSelectedPeiceNum = 10 + iSeprate;
		}
	}
	// 하단 둘째 줄
	else if ( iY >= c_SpritePosition[12].y && iY <= c_SpritePosition[12].y + c_iSpriteSize )
	{
		int iSeprate = 0;
		int iAbsXpos = iX - c_SpritePosition[12].x;

		iSeprate = iAbsXpos / c_iSpriteSize;

		if (1 >= iSeprate)
		{
			iSelectedPeiceNum = 12 + iSeprate;
		}
	}


	if (-1 != iSelectedPeiceNum)
	{
		// Entry의 번호를 테이블 참조로 얻어내어 
		// 해당 정보를 지닌 조각의 texture 정보를 참조하여 
		// 텍스춰 파일 이름을 출력해 낸다. 		

		// Pallete Dialog의 Texture Table을 얻어내 놓고, 
		TEXTURE_TABLE* pTextureTable = NULL;
		DWORD dwNum = 0;

		m_pPaletteDlg->GetTextureTable(&pTextureTable, &dwNum);


		// 엔트리를 참조하여 현재 선택된 텍스춰의 인덱스를 얻어낸다. 


		int iEntryNum = c_iEntryTable[iSelectedPeiceNum];		
		int iTextureNum = pTileSet->GetEntry(iEntryNum)->GetPiece(0)->GetTexture().TextureNum;

		if (iTextureNum <= dwNum)
		{
			m_wndTextureFileName.SetWindowText(pTextureTable[iTextureNum].szTextureName);
		}	
	}
}

///



//trustpak

CTileSetDisplayer::CTileSetDisplayer()
{	
	memset(m_apTileSprites,				0, sizeof(m_apTileSprites));
	memset(m_awTileTextureNumbers,		0, sizeof(m_awTileTextureNumbers));
	memset(m_awTileTextureDirection,	0, sizeof(m_awTileTextureDirection));
}


CTileSetDisplayer::~CTileSetDisplayer()
{
	Clear();
}

HRESULT CTileSetDisplayer::UpdateSpriteInfo(CTileSet* pTileSet)
{
	assert(pTileSet);
	
	WORD wTileIndex = 0;

	int i = 0; 
	for (i = 0; i < CTileSetDisplayer::MAX_SPRITE_NUM; i++)
	{
		// to separte Texture information.	
		/*
		wTileIndex = pTileSet->GetEntry(c_iEntryTable[i])->GetPiece(0)->GetTexture().GetTileIndex();

		m_awTileTextureNumbers[i]	= wTileIndex & (0x3FFF);
		m_awTileTextureDirection[i] = ( wTileIndex & (0xC000) ) >> 14;			
		*/
				
		m_awTileTextureNumbers[i]	= pTileSet->GetEntry(c_iEntryTable[i])->GetPiece(0)->GetTexture().TextureNum;
		m_awTileTextureDirection[i] = pTileSet->GetEntry(c_iEntryTable[i])->GetPiece(0)->GetTexture().Direction;								
	}
	
	SetName(pTileSet->GetName());

	return S_OK;
}


VOID CTileSetDisplayer::Clear(VOID)
{	
	CleanWholeSprites();
	
	memset(m_apTileSprites,				0, sizeof(m_apTileSprites));
	memset(m_awTileTextureNumbers,		0, sizeof(m_awTileTextureNumbers));
	memset(m_awTileTextureDirection,	0, sizeof(m_awTileTextureDirection));
}

BOOL CTileSetDisplayer::UpdateWholeSprites(I4DyuchiGXRenderer* pRenderer, TEXTURE_TABLE* pTextureTable)
{	
	assert(pRenderer);
	assert(pTextureTable);

	int i = 0;
	for (i = 0; i < CTileSetDisplayer::MAX_SPRITE_NUM; i++)
	{
		if (NULL != m_apTileSprites[i])
		{
			CleanOneSprite(i);
		}

		UpdateOneSprite(pRenderer, pTextureTable, i);		
	}
	

	return TRUE;
}

BOOL CTileSetDisplayer::CleanWholeSprites(VOID)
{	
	int i = 0;
	for (i = 0; i < CTileSetDisplayer::MAX_SPRITE_NUM; i++)
	{
		CleanOneSprite(i);
	}	

	return TRUE;
}

BOOL CTileSetDisplayer::UpdateOneSprite(I4DyuchiGXRenderer* pRenderer, TEXTURE_TABLE* pTextureTable, DWORD dwIndex)
{
	
	assert(pRenderer);
	assert(pTextureTable);

	DWORD dwTextureNum = m_awTileTextureNumbers[dwIndex];	
	m_apTileSprites[dwIndex] = pRenderer->CreateSpriteObject(pTextureTable[dwTextureNum].szTextureName, SPRITE_CREATE_IMAGE);

	if (NULL != m_apTileSprites[dwIndex])
	{
		m_apTileSprites[dwIndex]->Resize(64.0f, 64.0f);
	}
	
	
	return TRUE;
}

BOOL CTileSetDisplayer::CleanOneSprite(DWORD dwIndex)
{
	
	if (NULL != m_apTileSprites[dwIndex])
	{		
		m_apTileSprites[dwIndex]->Release();
		m_apTileSprites[dwIndex] = NULL;
		
	}	

	return TRUE;
}

IDISpriteObject* CTileSetDisplayer::GetSprite(DWORD dwIndex)
{
	return m_apTileSprites[dwIndex]; 	
}

////
