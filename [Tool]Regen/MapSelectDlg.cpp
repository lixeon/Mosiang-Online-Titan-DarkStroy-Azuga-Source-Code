// MapSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RegenTool.h"
#include "MapSelectDlg.h"
#include "MainFrm.h"
#include "RegenToolDoc.h"
#include "RegenToolView.h"
#include "MHMap.h"
#include "TileManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapSelectDlg dialog


CMapSelectDlg::CMapSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMapSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMapSelectDlg)
	//}}AFX_DATA_INIT
}


void CMapSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMapSelectDlg)
	DDX_Control(pDX, IDC_SELECTMAP, m_SelectMapEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMapSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CMapSelectDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapSelectDlg message handlers

void CMapSelectDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	CString temp;
	CString path;
	CString Mapfilename;
	CString FullPathName; 
	m_SelectMapEdit.GetWindowText(temp);
	
	Mapfilename.Format("%s.map", temp);
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CRegenToolView* pView = (CRegenToolView*)pMain->GetActiveView();
	
	DIRECTORYMGR->SetLoadMode(eLM_Map);
	if(!strcmp("58.map",(LPTSTR)(LPCTSTR)Mapfilename))
	{
		g_bFixHeight = TRUE;
		g_fFixHeight = 0;
	}
	else if(!strcmp("35.map",(LPTSTR)(LPCTSTR)Mapfilename))
	{
		g_bFixHeight = TRUE;
		g_fFixHeight = 0;
	}
	MAP->TestLoadMap((LPTSTR)(LPCTSTR)Mapfilename);
	pView->SetStartSelectFlag(TRUE);

	DIRECTORYMGR->SetLoadMode(eLM_Resource);

	char ttbfile[256];
	strcpy(ttbfile,(LPTSTR)(LPCTSTR)Mapfilename);
	int len = strlen(ttbfile);
	ttbfile[len-3] = 't';
	ttbfile[len-2] = 't';
	ttbfile[len-1] = 'b';

	MAP->InitTile(ttbfile, pView->GetViewTile());



	CDialog::EndDialog(1);
}
