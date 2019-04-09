// DialogEditTileSet.cpp : implementation file
//

#include "stdafx.h"
#include "4dyuchigxmapeditor.h"
#include "TileSetView.h"
#include "DialogEditTileSet.h"
#include "DialogTileSet.h"
#include "Tool.h"
#include "DialogEditEntry.h"


//trustpak

#include <assert.h>
#include "../4DYUCHIGRX_COMMON/IRenderer.h"

///

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogEditTileSet dialog


CDialogEditTileSet::CDialogEditTileSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogEditTileSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogEditTileSet)
	m_TileSetName = _T("");
	//}}AFX_DATA_INIT

	Create(CDialogEditTileSet::IDD,pParent);
}


void CDialogEditTileSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogEditTileSet)
	DDX_Text(pDX, IDC_TILESETNAME, m_TileSetName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogEditTileSet, CDialog)
	//{{AFX_MSG_MAP(CDialogEditTileSet)
	ON_BN_CLICKED(IDC_ENTRY1, OnEntry1)
	ON_BN_CLICKED(IDC_ENTRY2, OnEntry2)
	ON_BN_CLICKED(IDC_ENTRY3, OnEntry3)
	ON_BN_CLICKED(IDC_ENTRY4, OnEntry4)
	ON_BN_CLICKED(IDC_ENTRY5, OnEntry5)
	ON_BN_CLICKED(IDC_ENTRY6, OnEntry6)
	ON_BN_CLICKED(IDC_ENTRY7, OnEntry7)
	ON_BN_CLICKED(IDC_ENTRY8, OnEntry8)
	ON_BN_CLICKED(IDC_ENTRY9, OnEntry9)
	ON_BN_CLICKED(IDC_ENTRY10, OnEntry10)
	ON_BN_CLICKED(IDC_ENTRY11, OnEntry11)
	ON_BN_CLICKED(IDC_ENTRY12, OnEntry12)
	ON_BN_CLICKED(IDC_ENTRY13, OnEntry13)
	ON_EN_CHANGE(IDC_TILESETNAME, OnChangeTilesetname)
	ON_BN_CLICKED(IDC_ENTRY0, OnEntry0)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogEditTileSet message handlers


void CDialogEditTileSet::Initialize(CDialogTileSet* pTileSetDlg,CDialogEditEntry* pEditEntryDlg)
{
	m_pEditEntryDlg = pEditEntryDlg;
	m_pTileSetDlg= pTileSetDlg;	
	
	return;
}

void CDialogEditTileSet::EnableView()
{
	ShowWindow(SW_SHOWDEFAULT);
}
void CDialogEditTileSet::DisableView()
{
	ShowWindow(SW_HIDE);	
}

void CDialogEditTileSet::InitTileSet(CTileSet* pSet)
{
	m_TileSet.CopyFrom(pSet);
	m_TileSetName = m_TileSet.GetName();
	UpdateData(FALSE);
}

BOOL CDialogEditTileSet::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::OnCommand(wParam, lParam);
}

void CDialogEditTileSet::OnEditEntry(int n)
{
	DWORD EntryNum = n;
	m_pEditEntryDlg->InitEntry(EntryNum,m_TileSet.GetEntry(EntryNum));
	m_pEditEntryDlg->EnableView();
}


//trustpak
/*
void CDialogEditTileSet::OnEntry0() {	OnEditEntry(TILEBIT_NONE	);	}
void CDialogEditTileSet::OnEntry1() {	OnEditEntry(TILEBIT_RD	);	}
void CDialogEditTileSet::OnEntry2() {	OnEditEntry(TILEBIT_LD|TILEBIT_RD	);	}
void CDialogEditTileSet::OnEntry3() {	OnEditEntry(TILEBIT_LD	);	}
void CDialogEditTileSet::OnEntry4() {	OnEditEntry(TILEBIT_RU|TILEBIT_RD	);	}
void CDialogEditTileSet::OnEntry5() {	OnEditEntry(TILEBIT_ALL	);	}
void CDialogEditTileSet::OnEntry6() {	OnEditEntry(TILEBIT_LU|TILEBIT_LD	);	}
void CDialogEditTileSet::OnEntry7() {	OnEditEntry(TILEBIT_RU	);	}
void CDialogEditTileSet::OnEntry8() {	OnEditEntry(TILEBIT_LU|TILEBIT_RU	);	}
void CDialogEditTileSet::OnEntry9() {	OnEditEntry(TILEBIT_LU	);	}
void CDialogEditTileSet::OnEntry10() {	OnEditEntry(TILEBIT_ALL^TILEBIT_LU	);	}
void CDialogEditTileSet::OnEntry11() {	OnEditEntry(TILEBIT_ALL^TILEBIT_RU	);	}
void CDialogEditTileSet::OnEntry12() {	OnEditEntry(TILEBIT_ALL^TILEBIT_LD	);	}
void CDialogEditTileSet::OnEntry13() {	OnEditEntry(TILEBIT_ALL^TILEBIT_RD	);	}
*/

void CDialogEditTileSet::OnEntry0()		{ OnEditEntry(c_iEntryTable[0] );	}
void CDialogEditTileSet::OnEntry1()		{ OnEditEntry(c_iEntryTable[1] );	}
void CDialogEditTileSet::OnEntry2()		{ OnEditEntry(c_iEntryTable[2] );	}
void CDialogEditTileSet::OnEntry3()		{ OnEditEntry(c_iEntryTable[3] );	}
void CDialogEditTileSet::OnEntry4()		{ OnEditEntry(c_iEntryTable[4] );	}
void CDialogEditTileSet::OnEntry5()		{ OnEditEntry(c_iEntryTable[5] );	}
void CDialogEditTileSet::OnEntry6()		{ OnEditEntry(c_iEntryTable[6] );	}
void CDialogEditTileSet::OnEntry7()		{ OnEditEntry(c_iEntryTable[7] );	}
void CDialogEditTileSet::OnEntry8()		{ OnEditEntry(c_iEntryTable[8] );	}
void CDialogEditTileSet::OnEntry9()		{ OnEditEntry(c_iEntryTable[9] );	}
void CDialogEditTileSet::OnEntry10()	{ OnEditEntry(c_iEntryTable[10]	);	}
void CDialogEditTileSet::OnEntry11()	{ OnEditEntry(c_iEntryTable[11]	);	}
void CDialogEditTileSet::OnEntry12()	{ OnEditEntry(c_iEntryTable[12]	);	}
void CDialogEditTileSet::OnEntry13()	{ OnEditEntry(c_iEntryTable[13]	);	}

///


void CDialogEditTileSet::SetEntry(DWORD EntryNum,CTileSetEntry* pEntry)
{
	m_TileSet.SetEntry(EntryNum,pEntry);

	//trustpak

	if (NULL != m_pTileSetDlg)
	{
		CTileSetDisplayer* pTileSetDisplayer = m_pTileSetDlg->GetCurrentTileSetDisplayer();

		if (NULL != pTileSetDisplayer)
		{
			TEXTURE_TABLE*	pTextureTable = NULL;
			DWORD			dwNum = 0;

			m_pTileSetDlg->GetTilePalleteDlg()->GetTextureTable(&pTextureTable, &dwNum);

			pTileSetDisplayer->UpdateSpriteInfo(&m_TileSet);
			pTileSetDisplayer->UpdateWholeSprites(m_pTileSetDlg->GetTileSetRenderer(), pTextureTable);
		}

		m_pTileSetDlg->RenderTileSet();
	}
	///
}

void CDialogEditTileSet::OnChangeTilesetname() 
{
	UpdateData(TRUE);
	m_TileSet.SetName(m_TileSetName);
}

void CDialogEditTileSet::OnOK() 
{
	// TODO: Add extra validation here
	m_pTileSetDlg->AddTileSet(&m_TileSet);			
	m_pTileSetDlg->AddTileSetDisplayer(&m_TileSet);	

	// 강제로 한번 더 스프라이트들을 정리해줌. 
	m_pTileSetDlg->UpdateWholeDisplayerInfo();	
	m_pTileSetDlg->RenderTileSet();
	///

//	CDialog::OnOK();
}

void CDialogEditTileSet::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
