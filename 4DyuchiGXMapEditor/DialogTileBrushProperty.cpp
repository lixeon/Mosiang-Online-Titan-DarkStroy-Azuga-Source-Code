// DialogTileBrushProperty.cpp : implementation file
//

#include "stdafx.h"
#include "4dyuchigxmapeditor.h"
#include "DialogTileBrushProperty.h"
#include "TileBrush.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogTileBrushProperty dialog


CDialogTileBrushProperty::CDialogTileBrushProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogTileBrushProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogTileBrushProperty)
	m_strTileBrushName = _T("");
	//}}AFX_DATA_INIT
}


void CDialogTileBrushProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogTileBrushProperty)
	DDX_Text(pDX, IDC_EDIT_BRUSH_NAME, m_strTileBrushName);
	DDV_MaxChars(pDX, m_strTileBrushName, 127);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogTileBrushProperty, CDialog)
	//{{AFX_MSG_MAP(CDialogTileBrushProperty)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogTileBrushProperty message handlers

void CDialogTileBrushProperty::OnButtonOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	lstrcpy(m_pTileBrushProperty->szName,(LPCSTR)m_strTileBrushName);

	EndDialog(0);
	
}

void CDialogTileBrushProperty::OnDestroy() 
{
	UpdateData(TRUE);
	lstrcpy(m_pTileBrushProperty->szName,(LPCSTR)m_strTileBrushName);

	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
}
