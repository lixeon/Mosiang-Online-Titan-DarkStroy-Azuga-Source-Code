// 4DyuchiFilePackDlg.cpp : implementation file
//

#include "stdafx.h"
#include "4DyuchiFilePack.h"
#include "4DyuchiFilePackDlg.h"
#include "../4DyuchiGXGFunc/global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib, "..\\4DyuchiGXGFunc\\SS3DGFunc.lib")

DWORD __stdcall UnlocProgresskProc(DWORD dwCurCount,DWORD dwTotalCount,void* pArg);

I4DyuchiFileStorage*	g_pStorage = NULL;
CMy4DyuchiFilePackDlg*	g_pDlg;

void DelPackFileHandle(void* pVoid)
{
	g_pStorage->UnmapPackFile(pVoid);
}

#define MAX_FILE_ATOM_NUM	25000
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

DWORD OpenPAKFileName(char* szFileName);

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
	

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMy4DyuchiFilePackDlg dialog

CMy4DyuchiFilePackDlg::CMy4DyuchiFilePackDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMy4DyuchiFilePackDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMy4DyuchiFilePackDlg)
	m_strFullPathFileName = _T("");
	m_bWarningExistingFile = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy4DyuchiFilePackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMy4DyuchiFilePackDlg)
	DDX_Control(pDX, IDC_LIST2, m_lbPackFileView);
	DDX_Control(pDX, IDC_LIST1, m_lbFileView);
	DDX_Text(pDX, IDC_EDIT_FULLPATHNAME, m_strFullPathFileName);
	DDV_MaxChars(pDX, m_strFullPathFileName, 255);
	DDX_Check(pDX, IDC_CHECK_WARNING_EXISTING_FILE, m_bWarningExistingFile);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMy4DyuchiFilePackDlg, CDialog)
	//{{AFX_MSG_MAP(CMy4DyuchiFilePackDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_FILE, OnButtonBrowseFile)
	ON_BN_CLICKED(IDC_BUTTON_MAP_TO_STORAGE, OnButtonMapToStorage)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_EXTRACT_ALL, OnButtonExtractAll)
	ON_BN_CLICKED(IDC_BUTTON_EXTRACT_ALL_IN_STORAGE, OnButtonExtractAllInStorage)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_PACKFILE, OnButtonSelectPackfile)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_PACKFILE, OnButtonDeletePackfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMy4DyuchiFilePackDlg message handlers

BOOL CMy4DyuchiFilePackDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_pPackFileHandle = NULL;
	m_pFileInfoList = NULL;
	m_dwFileInfoNum = NULL;

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	m_lbcFileView.Initialize(65536,_MAX_PATH,&m_lbFileView,NULL);
	m_lbcPackFileView.Initialize(MAX_PACK_FILE_NUM,_MAX_PATH,&m_lbPackFileView,DelPackFileHandle);

	m_bWarningExistingFile = TRUE;
	UpdateData(FALSE);

	g_pDlg = this;
	CoInitialize(NULL);
	
	HRESULT hr;

	hr = CoCreateInstance(
           CLSID_4DyuchiFileStorage,
           NULL,
           CLSCTX_INPROC_SERVER,
           IID_4DyuchiFileStorage,
           (void**)&m_pStorage);

	if (hr != S_OK)
		__asm int 3
		
	g_pStorage = m_pStorage;
	m_pStorage->Initialize(MAX_FILE_ATOM_NUM,1024,64,FILE_ACCESS_METHOD_ONLY_FILE);
	
/*
	void* pFP = m_pStorage->FSOpenFile("fighter.chr",1);
	if (pFP)
	{


		char	buf[256];
		while (m_pStorage->FSScanf(pFP,"%s",buf) != EOF)
		{
			__asm nop

		}
		m_pStorage->FSCloseFile(pFP);
	}
*/
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMy4DyuchiFilePackDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMy4DyuchiFilePackDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMy4DyuchiFilePackDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMy4DyuchiFilePackDlg::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: Add your message handler code here and/or call default
	char	szFileName[_MAX_PATH];
	char	szExt[8];

	int		count = ::DragQueryFile(hDropInfo, 0xFFFFFFFF, szFileName, _MAX_PATH);

	m_pStorage->LockPackFile(m_pPackFileHandle,0);
	UpdateData(TRUE);
	char	txt[_MAX_PATH+64];
	for(int i=0; i<count ; i++)
	{
		::DragQueryFile(hDropInfo, i, szFileName, _MAX_PATH);
		DWORD	dwSize = GetExt(szExt,szFileName);
		DWORD	dwLen = lstrlen(szFileName);
		if (!dwLen)
			goto lb_skip;

		if (dwSize)
		{
			if (m_bWarningExistingFile)
			{
				if (m_pStorage->IsExistInFileStorage(szFileName))
				{
					wsprintf(txt,"파일명이 겹칩니다.(%s)",szFileName);
					::MessageBox(NULL,txt,"Warning",MB_OK);
					goto lb_skip;

				}
			}
			m_pStorage->InsertFileToPackFile(m_pPackFileHandle,szFileName);
		}
lb_skip:
		__asm nop

	}
	m_pStorage->UnlockPackFile(m_pPackFileHandle,UnlocProgresskProc);
	CDialog::OnDropFiles(hDropInfo);

	UpdateFileViewDisplay();

//	m_pStorage->ExtractAllFiles();
}


DWORD __stdcall UnlocProgresskProc(DWORD dwCurCount,DWORD dwTotalCount,void* pArg)
{
	return 	g_pDlg->UnlockProc(dwCurCount,dwTotalCount,pArg);
}

DWORD CMy4DyuchiFilePackDlg::UnlockProc(DWORD dwCurCount,DWORD dwTotalCount,void* pArg)
{
	DWORD	dwPercentage = (DWORD)( (float)(dwCurCount + 1) / (float)dwTotalCount * 100.0f );
	char	txt[16];
	wsprintf(txt,"%d",dwPercentage);
	::SetDlgItemText(m_hWnd,IDC_STATIC_PROGRESS,txt);
	RedrawWindow(NULL,NULL,RDW_UPDATENOW);

	return 0;

}

void CMy4DyuchiFilePackDlg::UpdateFileViewDisplay()
{

	if (m_pPackFileHandle)
	{
		ClearFileViewDisplay();
		m_dwFileInfoNum = m_pStorage->CreateFileInfoList(m_pPackFileHandle,&m_pFileInfoList,MAX_FILE_ATOM_NUM);
		for (DWORD i=0; i<m_dwFileInfoNum; i++)
		{
			m_lbcFileView.InsertItem(m_pFileInfoList[i].pszFileName,m_pFileInfoList+i);
		}
	}
}

void CMy4DyuchiFilePackDlg::OnButtonBrowseFile(void)
{
	char		szFileName[_MAX_PATH];
	DWORD		dwLen = OpenPAKFileName(szFileName);
	if (!dwLen)
		return;

	m_strFullPathFileName = szFileName;

	UpdateData(FALSE);


}
void CMy4DyuchiFilePackDlg::ClearFileViewDisplay()
{
	if (m_pFileInfoList)
	{
		m_pStorage->DeleteFileInfoList(m_pPackFileHandle,m_pFileInfoList);
		m_pFileInfoList = NULL;
	}
	m_lbcFileView.DeleteAll();
}

void CMy4DyuchiFilePackDlg::OnDestroy() 
{
	ClearFileViewDisplay();

	if (m_pStorage)
	{
		m_lbcPackFileView.DeleteAll();
		m_pStorage->Release();
		m_pStorage = NULL;
	}
	CoUninitialize();

	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

DWORD OpenPAKFileName(char* szFileName)
{
	// TODO: Add your control notification handler code here
	DWORD	dwLen = 0;

	// TODO: Add your command handler code here
	char szFilters[] = "PAK Files (*.pak) | *.pak | All files (*.*) |*.*||";
	char* filename;
		
	CString pathname;
	CFileDialog dlg(TRUE,"pak","*.pak",NULL,szFilters,AfxGetMainWnd());

	if (dlg.DoModal() == IDOK)
	{
		pathname = (LPSTR)(LPCSTR)(dlg.GetPathName());
		if (pathname == "")
			goto lb_return;
	}

	

	if (!(filename = (LPSTR)(LPCSTR)pathname))
		goto lb_return;

	dwLen = lstrlen(filename);

	if (!dwLen)
		goto lb_return;

	memcpy(szFileName,filename,dwLen);
	szFileName[dwLen] = 0;
lb_return:
	return dwLen;
}

void CMy4DyuchiFilePackDlg::OnButtonMapToStorage() 
{
	
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char*	pFileName = (LPSTR)(LPCSTR)m_strFullPathFileName;
	
	DWORD dwLen = lstrlen(pFileName);
	if (!dwLen)
		return;

	void*	pPackFileHandle = NULL;
	pPackFileHandle = m_pStorage->MapPackFile(pFileName);
	if (!pPackFileHandle)
		return;

	FSPACK_FILE_INFO	fileInfo;
	m_pStorage->GetPackFileInfo(pPackFileHandle,&fileInfo);


	m_lbcPackFileView.InsertItem(fileInfo.szFileName,pPackFileHandle);


	if (!m_pPackFileHandle)
	{
		m_pPackFileHandle = pPackFileHandle;
		UpdateFileViewDisplay();
	}
}

void CMy4DyuchiFilePackDlg::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here

	void*					pInfoList[1024];
	FSFILE_ATOM_INFO*		pFileInfo;
	DWORD					dwNum;
	if (m_pPackFileHandle)
	{
		m_pStorage->LockPackFile(m_pPackFileHandle,0);
		dwNum = m_lbcFileView.GetMultipleSelectedItems(pInfoList,1024);
		for (DWORD i=0; i<dwNum; i++)
		{
			pFileInfo = (FSFILE_ATOM_INFO*)pInfoList[i];
			m_pStorage->DeleteFileFromPackFile(pFileInfo->pszFileName);
		}
		m_pStorage->UnlockPackFile(m_pPackFileHandle,NULL);
		UpdateFileViewDisplay();

	}
	
}

void CMy4DyuchiFilePackDlg::OnButtonExtractAll() 
{
	// TODO: Add your control notification handler code here
	if (m_pPackFileHandle)
	{
		m_pStorage->ExtractAllFilesFromPackFile(m_pPackFileHandle);
	}
	
}

void CMy4DyuchiFilePackDlg::OnButtonExtractAllInStorage() 
{
	// TODO: Add your control notification handler code here
	m_pStorage->ExtractAllFiles();

}

void CMy4DyuchiFilePackDlg::OnButtonSelectPackfile() 
{
	// TODO: Add your control notification handler code here
	void* pPackFileHandle = m_lbcPackFileView.GetSelectedItem();
	m_pPackFileHandle = pPackFileHandle;
	UpdateFileViewDisplay();

}

void CMy4DyuchiFilePackDlg::OnButtonDeletePackfile() 
{
	// TODO: Add your control notification handler code here
	
	void* pPackFileHandle = m_lbcPackFileView.GetSelectedItem();
	
	m_lbcPackFileView.DeleteSelectedItem();

	if (m_pPackFileHandle == pPackFileHandle)
	{
		m_pPackFileHandle = NULL;
		ClearFileViewDisplay();
	}

}
