// DialogEditEntry.cpp : implementation file
//

#include "stdafx.h"
#include "4dyuchigxmapeditor.h"
#include "DialogEditEntry.h"
#include "DialogTilePalette.h"
#include "DialogEditTileSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogEditEntry dialog


CDialogEditEntry::CDialogEditEntry(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogEditEntry::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogEditEntry)
	m_Probability[0] = &m_Probability0;
	m_Probability[1] = &m_Probability1;
	m_Probability[2] = &m_Probability2;
	m_Probability[3] = &m_Probability3;
	//}}AFX_DATA_INIT

	Create(CDialogEditEntry::IDD, pParent);

	m_bShow = FALSE;
	m_ShowBtn.SetWindowText("Show");
	
	m_bSelecting = FALSE;
}


void CDialogEditEntry::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogEditEntry)
	DDX_Control(pDX, IDC_SHOW, m_ShowBtn);
	DDX_Text(pDX, IDC_PROBABILITY1, m_Probability0);
	DDX_Text(pDX, IDC_PROBABILITY2, m_Probability1);
	DDX_Text(pDX, IDC_PROBABILITY3, m_Probability2);
	DDX_Text(pDX, IDC_PROBABILITY4, m_Probability3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogEditEntry, CDialog)
	//{{AFX_MSG_MAP(CDialogEditEntry)
	ON_BN_CLICKED(IDC_SELECTTILE1, OnSelecttile1)
	ON_BN_CLICKED(IDC_SELECTTILE2, OnSelecttile2)
	ON_BN_CLICKED(IDC_SELECTTILE3, OnSelecttile3)
	ON_BN_CLICKED(IDC_SELECTTILE4, OnSelecttile4)
	ON_BN_CLICKED(IDC_CHANGEDIRECTION1, OnChangedirection1)
	ON_BN_CLICKED(IDC_CHANGEDIRECTION2, OnChangedirection2)
	ON_BN_CLICKED(IDC_CHANGEDIRECTION3, OnChangedirection3)
	ON_BN_CLICKED(IDC_CHANGEDIRECTION4, OnChangedirection4)
	ON_BN_CLICKED(IDC_SHOW, OnShow)
	ON_BN_CLICKED(IDOK, OnOk)
	ON_EN_CHANGE(IDC_PROBABILITY1, OnChangeProbability1)
	ON_EN_CHANGE(IDC_PROBABILITY2, OnChangeProbability2)
	ON_EN_CHANGE(IDC_PROBABILITY3, OnChangeProbability3)
	ON_EN_CHANGE(IDC_PROBABILITY4, OnChangeProbability4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogEditEntry message handlers

void CDialogEditEntry::OnSelecttile1() {	SelectTile(0);	}
void CDialogEditEntry::OnSelecttile2() {	SelectTile(1);	}
void CDialogEditEntry::OnSelecttile3() {	SelectTile(2);	}
void CDialogEditEntry::OnSelecttile4() {	SelectTile(3);	}
void CDialogEditEntry::OnChangedirection1() {	RotateTile(0);	}
void CDialogEditEntry::OnChangedirection2() {	RotateTile(1);	}
void CDialogEditEntry::OnChangedirection3() {	RotateTile(2);	}
void CDialogEditEntry::OnChangedirection4() {	RotateTile(3);	}
void CDialogEditEntry::OnChangeProbability1() {	ChangeProbability(0);	}
void CDialogEditEntry::OnChangeProbability2() {	ChangeProbability(1);	}
void CDialogEditEntry::OnChangeProbability3() {	ChangeProbability(2);	}
void CDialogEditEntry::OnChangeProbability4() {	ChangeProbability(3);	}

void CDialogEditEntry::OnShow() 
{
	// TODO: Add your control notification handler code here
	m_bShow = !m_bShow;
	if(m_bShow)
		m_ShowBtn.SetWindowText("Hide");
	else
		m_ShowBtn.SetWindowText("Show");

	m_pPaletteDlg->RenderPalette();
}

void CDialogEditEntry::OnOk() 
{
	// TODO: Add your control notification handler code here
	if(m_bShow)
		OnShow();

	m_pEditTileSetDlg->SetEntry(m_EntryNum,&m_Entry);	
	
	CDialog::OnOK();
}

void CDialogEditEntry::OnCancel() 
{
	// TODO: Add extra cleanup here
	if(m_bShow)
		OnShow();
	
	CDialog::OnCancel();
}

void CDialogEditEntry::SelectTile(DWORD PieceNum)
{
	m_bSelecting = TRUE;
	m_SelectingPieceNum = PieceNum;

	m_pPaletteDlg->RenderPalette();
}
void CDialogEditEntry::RotateTile(DWORD PieceNum)
{
	TILETEXTURE tex = m_Entry.GetPiece(PieceNum)->GetTexture();
	++tex.Direction;
	if(tex.Direction >= 4)
		tex.Direction = 0;

	DWORD Prob = m_Entry.GetPiece(PieceNum)->GetProbability();
	m_Entry.SetPiece(PieceNum,&tex,Prob);

	m_pPaletteDlg->RenderPalette();
}
void CDialogEditEntry::ChangeProbability(DWORD PieceNum)
{
	UpdateData(TRUE);
	
	TILETEXTURE tex;
	for(DWORD n=0;n<4;++n)
	{
		tex = m_Entry.GetPiece(n)->GetTexture();
		m_Entry.SetPiece(n,&tex,*m_Probability[n]);
	}
}


void CDialogEditEntry::Initialize(CDialogEditTileSet* pEditTileSetDlg,CDialogTilePalette* pPaletteDlg)
{
	m_pEditTileSetDlg = pEditTileSetDlg;
	m_pPaletteDlg = pPaletteDlg;
}

void CDialogEditEntry::EnableView()
{
	ShowWindow(SW_SHOWDEFAULT);
}
void CDialogEditEntry::DisableView()
{
	ShowWindow(SW_HIDE);	
}

void CDialogEditEntry::InitEntry(DWORD Entrynum,CTileSetEntry* pEntry)
{
	m_EntryNum = Entrynum;
	m_Entry = *pEntry;

	for(DWORD n=0;n<4;++n)
	{
		*m_Probability[n] = pEntry->GetPiece(n)->GetProbability();
	}

	UpdateData(FALSE);

	m_pPaletteDlg->RenderPalette();
}

void CDialogEditEntry::OnClickTileInPalette(DWORD TextureNum)
{
	if(m_bSelecting)
	{
		TILETEXTURE tex = m_Entry.GetPiece(m_SelectingPieceNum)->GetTexture();
		tex.TextureNum = TextureNum;
		DWORD Prob = m_Entry.GetPiece(m_SelectingPieceNum)->GetProbability();
		if(Prob == 0)
		{
			Prob = *m_Probability[m_SelectingPieceNum] = 1;
			UpdateData(FALSE);
		}
		
		m_Entry.SetPiece(m_SelectingPieceNum,&tex,Prob);
		m_bSelecting = FALSE;

		if(m_bShow == FALSE)
			OnShow();
		
		m_pPaletteDlg->RenderPalette();
	}
}

DWORD CDialogEditEntry::GetTexture(DWORD PieceNum)
{
	return m_Entry.GetPiece(PieceNum)->GetTexture().TextureNum;
}

DWORD CDialogEditEntry::GetDirection(DWORD PieceNum)
{
	return m_Entry.GetPiece(PieceNum)->GetTexture().Direction;
}
