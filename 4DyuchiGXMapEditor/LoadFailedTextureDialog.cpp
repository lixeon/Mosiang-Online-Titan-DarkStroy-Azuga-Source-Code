// LoadFailedTextureDialog.cpp : implementation file
//

#include "stdafx.h"
#include "4dyuchigxmapeditor.h"
#include "LoadFailedTextureDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoadFailedTextureDialog dialog


CLoadFailedTextureDialog::CLoadFailedTextureDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CLoadFailedTextureDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoadFailedTextureDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pLoadFailedTextureTable		= NULL;
	m_dwLoadFailedTextureTableCount = 0;
}


void CLoadFailedTextureDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoadFailedTextureDialog)	
	//}}AFX_DATA_MAP
}

void CLoadFailedTextureDialog::UpdateListBoxData(TEXTURE_TABLE* pTextureTable, DWORD dwCount)
{
	m_pLoadFailedTextureTable		= pTextureTable;
	m_dwLoadFailedTextureTableCount = dwCount;
}


BEGIN_MESSAGE_MAP(CLoadFailedTextureDialog, CDialog)
	//{{AFX_MSG_MAP(CLoadFailedTextureDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_SAVELIST_BUTTON, OnSavelistButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoadFailedTextureDialog message handlers

int CLoadFailedTextureDialog::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class		
	
	return CDialog::DoModal();
}

void CLoadFailedTextureDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LOADFAILED_LIST);

	if (NULL != pListBox)
	{
		int i = 0;
		for (i = 0; i < pListBox->GetCount(); i++)
		{
			pListBox->DeleteString(0);
		}

		for (i = 0; i < (int)m_dwLoadFailedTextureTableCount; i++)
		{
			pListBox->InsertString(i, m_pLoadFailedTextureTable[i].szTextureName);
		}	
	}
	
}

void CLoadFailedTextureDialog::OnSavelistButton() 
{
	char szFilters[] = "Text Files (*.txt) | *.txt | All files (*.*) |*.*||";
	char* filename;
		
	CString pathname;
	CFileDialog dlg(FALSE,"txt","*.txt",NULL,szFilters,AfxGetMainWnd());

	if (dlg.DoModal() != IDOK)
		return;

	pathname = (LPSTR)(LPCSTR)(dlg.GetPathName());
	if (pathname == "")
		return;
		
	// TODO: Add your control notification handler code here
	FILE* fp = fopen(pathname,"w");


	if (NULL != m_pLoadFailedTextureTable)
	{
		int i = 0;
		for (i = 0; i < (int)m_dwLoadFailedTextureTableCount; i++)
		{
			fputs(m_pLoadFailedTextureTable[i].szTextureName, fp);
			fputs("\n", fp);
		}
	}
	
	fclose(fp);
	fp = NULL;
}
