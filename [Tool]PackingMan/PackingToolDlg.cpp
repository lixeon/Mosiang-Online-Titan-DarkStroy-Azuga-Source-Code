// PackingToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PackingTool.h"
#include "PackingToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

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
// CPackingToolDlg dialog

CPackingToolDlg::CPackingToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPackingToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPackingToolDlg)
	m_sFileNum = _T("");
	m_sCurFile = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPackingToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPackingToolDlg)
	DDX_Control(pDX, IDC_BUTTON_SAVE_BIN, m_BtnSave);
	DDX_Control(pDX, IDC_BUTTON_SAVE_AS_BIN, m_BtnSaveAsBin);
	DDX_Control(pDX, IDC_EDIT, m_Edit);
	DDX_Control(pDX, IDC_BUTTON_SAVE_TO_TXT, m_BtnSaveAllToTxt);
	DDX_Control(pDX, IDC_BUTTON_SAVE_TO_BIN, m_BtnSaveAllToBin);
	DDX_Control(pDX, IDC_BUTTON_OPEN_BIN, m_BtnOpenBin);
	DDX_Control(pDX, IDC_BUTTON_OPEN, m_BtnOpen);
	DDX_Control(pDX, IDC_BUTTON_NEW, m_BtnNew);
	DDX_Control(pDX, IDC_BUTTON_DELALL, m_BtnDelAll);
	DDX_Control(pDX, IDC_LIST_FILE, m_FileList);
	DDX_Text(pDX, IDC_STATIC_FILENUM, m_sFileNum);
	DDX_Text(pDX, IDC_STATIC_CURFILE, m_sCurFile);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPackingToolDlg, CDialog)
	//{{AFX_MSG_MAP(CPackingToolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, OnButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_TO_BIN, OnButtonSaveToBin)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_BIN, OnButtonOpenBin)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_TO_TXT, OnButtonSaveToTxt)
	ON_BN_CLICKED(IDC_BUTTON_DELALL, OnButtonDelall)
	ON_LBN_SELCHANGE(IDC_LIST_FILE, OnSelchangeListFile)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_AS_BIN, OnButtonSaveAsBin)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON_SAVE_BIN, OnButtonSaveBin)
	ON_COMMAND(ID_SEARCH, OnSearch)
	ON_BN_CLICKED(IDC_BUTTON_SAVEDSOF, OnButtonSavedsof)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPackingToolDlg message handlers

BOOL CPackingToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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
	m_nFileNum = 0;
	m_nOldIndex = 0;
	m_sFileNum.Format( "%d", m_nFileNum );
	m_BtnDelAll.EnableWindow( FALSE );
//	m_BtnNew.EnableWindow( FALSE );
//	m_BtnOpen.EnableWindow( FALSE );
//	m_BtnOpenBin.EnableWindow( FALSE );
	m_BtnSaveAllToBin.EnableWindow( FALSE );
	m_BtnSaveAllToTxt.EnableWindow( FALSE );
	m_BtnSaveAsBin.EnableWindow( FALSE );
	m_BtnSave.EnableWindow( FALSE );
	m_Edit.EnableWindow( FALSE );
	m_Edit.SetWindowText( NULL );
	m_Edit.SetLimitText( 1024*1024 );
	UpdateData( FALSE );
/*
	WINDOWPLACEMENT lp;
	this->GetWindowPlacement( &lp );
	m_Edit.GetWindowPlacement( &m_EditPl );
	m_Gap.right = lp.rcNormalPosition.right - m_EditPl.rcNormalPosition.right;
	m_Gap.bottom = lp.rcNormalPosition.bottom - m_EditPl.rcNormalPosition.bottom;

	RECT rt;
	m_Edit.GetWindowRect( &rt );
*/
	this->DragAcceptFiles( TRUE );
	
	
	char cmd[MAX_PATH] = {0,};
	int nCmdLen = strlen( AfxGetApp()->m_lpCmdLine );
	if( nCmdLen )
	{
		strcpy( cmd, AfxGetApp()->m_lpCmdLine+1 );
		cmd[nCmdLen-2] = 0;
		
		OpenFile(cmd);

		UpdateData( FALSE );
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPackingToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPackingToolDlg::OnPaint() 
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
HCURSOR CPackingToolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPackingToolDlg::OnButtonOpen() 
{
	CFileDialog dlg( TRUE, NULL, "*.*", OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY, 
					 "Text 格式(*.txt)|*.txt|Effect 格式(*.eff)|*.eff|所有 格式 (*.*)|*.*||", NULL );
	char buff[65535] = "";
	dlg.m_ofn.lpstrFile = buff;
	dlg.m_ofn.nMaxFile = 65535;
	CString fullfilename, filename;
	char* pfile = NULL;

	if( dlg.DoModal() == IDOK )
	{
		for( POSITION ps = dlg.GetStartPosition(); ps != NULL; )
		{
			fullfilename = dlg.GetNextPathName( ps );
			if( m_FileMng.AddFile( fullfilename, "rt" ) )
			{
				m_FileList.AddString( m_FileMng.GetFileName( m_nFileNum ) );
				++m_nFileNum;			
			}
		}
	}

	UpdateList();
	
	UpdateData( FALSE );
}

void CPackingToolDlg::OnButtonSaveToBin() 
{
	if( m_ExtDlg.DoModal() == IDOK )
	{
		m_FileMng.SetExt( m_ExtDlg.m_sExt );

		SaveData( m_FileList.GetCurSel() );
		
		m_FileMng.SaveToBin();		
	}
}

void CPackingToolDlg::OnButtonOpenBin() 
{
	CFileDialog dlg( TRUE, NULL, "*.*", OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY, 
					 "Binary 格式|*.bin; *.beff; *.befl; *.bmhm; *.bsad|所有 格式 (*.*)|*.*||", NULL );
	char buff[65535] = "";
	dlg.m_ofn.lpstrFile = buff;
	dlg.m_ofn.nMaxFile = 65535;
	CString fullfilename, filename;

	if( dlg.DoModal() == IDOK )
	{
		for( POSITION ps = dlg.GetStartPosition(); ps != NULL; )
		{
			fullfilename = dlg.GetNextPathName( ps );
			if( m_FileMng.AddFile( fullfilename, "rb" ) )
			{
				m_FileList.AddString( m_FileMng.GetFileName( m_nFileNum ) );
				++m_nFileNum;			
			}
		}
	}

	if( m_nFileNum )
	{
		m_sFileNum.Format( "%d", m_nFileNum );
		m_BtnDelAll.EnableWindow( TRUE );
		m_BtnSaveAllToBin.EnableWindow( TRUE );
		m_BtnSaveAllToTxt.EnableWindow( TRUE );
		m_BtnSaveAsBin.EnableWindow( TRUE );
		m_BtnSave.EnableWindow( TRUE );
//		m_BtnOpen.EnableWindow( FALSE );
		// list
		m_FileList.SetCurSel( m_nFileNum-1 );
		// edit
		m_Edit.EnableWindow( TRUE );
		CMHFileEx* pFile = m_FileMng.GetFile( m_nFileNum-1 );
		m_Edit.SetWindowText( pFile->GetData() );

		m_sCurFile = pFile->GetFullFileName();

		m_nOldIndex = m_nFileNum-1;
	}

	UpdateData( FALSE );
}

void CPackingToolDlg::OnButtonSaveToTxt() 
{
	if( m_ExtDlg.DoModal() == IDOK )
	{
		m_FileMng.SetExt( m_ExtDlg.m_sExt );
		SaveData( m_FileList.GetCurSel() );
		m_FileMng.SaveToTxt();
	}
}

void CPackingToolDlg::OnButtonDelall() 
{
	m_FileMng.Clear();
	m_nFileNum = 0;
	m_sFileNum.Format( "%d", m_nFileNum );
	m_FileList.ResetContent();

	m_BtnDelAll.EnableWindow( FALSE );
	m_BtnSaveAllToBin.EnableWindow( FALSE );
	m_BtnSaveAllToTxt.EnableWindow( FALSE );
	m_BtnSaveAsBin.EnableWindow( FALSE );
	m_BtnSave.EnableWindow( FALSE );
//	m_BtnOpenBin.EnableWindow( TRUE );
//	m_BtnOpen.EnableWindow( TRUE );
	
	//edit
	m_Edit.SetWindowText( NULL );
	m_Edit.EnableWindow( FALSE );

	m_sCurFile = "";
	
	UpdateData( FALSE );	
}


void CPackingToolDlg::OnSelchangeListFile() 
{
	SaveData( m_nOldIndex );

	int index = m_FileList.GetCurSel();
	m_sCurFile = m_FileMng.GetFullFileName( index );

	// edit
	CMHFileEx* pFile = m_FileMng.GetFile( index );
	m_Edit.SetWindowText( pFile->GetData() );

	m_nOldIndex = index;

	UpdateData( FALSE );
}

void CPackingToolDlg::OnButtonNew() 
{
	CFileDialog dlg( FALSE, NULL, "*.bin", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
					 "Binary 格式|*.bin; *.beff; *.befl; *.bmhm; *.bsad|所有 格式 (*.*)|*.*||", NULL );

	if( dlg.DoModal() == IDOK )
	{
		CString lpszPathName = dlg.GetPathName();

		m_FileMng.New( lpszPathName );
		m_FileList.AddString( m_FileMng.GetFileName( m_nFileNum ) );
		m_sCurFile = lpszPathName;
		SaveData( m_nOldIndex );

		++m_nFileNum;
	}

	if( m_nFileNum )
	{
		m_sFileNum.Format( "%d", m_nFileNum );
		m_BtnDelAll.EnableWindow( TRUE );
		m_BtnSaveAllToBin.EnableWindow( TRUE );
		m_BtnSaveAllToTxt.EnableWindow( TRUE );
		m_BtnSaveAsBin.EnableWindow( TRUE );
		m_BtnSave.EnableWindow( TRUE );
		// list
		m_FileList.SetCurSel( m_nFileNum-1 );
		// edit
		m_Edit.EnableWindow( TRUE );
		m_Edit.SetWindowText( NULL );

		m_nOldIndex = m_nFileNum-1;
	}

	UpdateData( FALSE );
}

void CPackingToolDlg::SaveData( int index )
{
	char* str = new char[MEGA];
	memset( str, 0, MEGA );
	m_Edit.GetWindowText( str, MEGA );

	m_FileMng.SetFileData( index, str );
	delete [] str;
}

void CPackingToolDlg::OnButtonSaveAsBin() 
{
	CFileDialog dlg( FALSE, NULL, "*.*", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
					 "Binary 格式|*.bin; *.beff; *.befl; *.bmhm; *.bsad|所有 格式 (*.*)|*.*||", NULL );

	if( dlg.DoModal() == IDOK )
	{
		CString lpszPathName = dlg.GetPathName();
		int index = m_FileList.GetCurSel();

		m_FileMng.SaveAsBin( index, lpszPathName );
	}
}

void CPackingToolDlg::OnButtonSaveBin() 
{
	SaveData( m_FileList.GetCurSel() );
	m_FileMng.SaveToBin();
}

void CPackingToolDlg::OnDropFiles(HDROP hDropInfo) 
{
	char filename[MAX_PATH] = {0,};
	UINT count = ::DragQueryFile( hDropInfo, 0xFFFFFFFF, filename, MAX_PATH );
	
	if( count )
	{
		for( UINT ii = 0; ii < count; ++ii )
		{
			DragQueryFile( hDropInfo, ii, filename, MAX_PATH );
			
			OpenFile(filename);
		}		
	}
	
	UpdateData( FALSE );

	CDialog::OnDropFiles(hDropInfo);
}

void CPackingToolDlg::OpenFile(char* pFileName)
{	
	char ext[MAX_PATH] = {0,};
	int len = strlen(pFileName);
	int i, j;
	
	for( i = len-1; i >= 0; --i )
	{
		if( pFileName[i] == '.' )
		{
			j = i;
			break;
		}
	}
	
	for( i = 0; i < len-j; ++i )
		ext[i] = pFileName[j+1+i];
	
	strupr(ext);
	
	if( strcmp( ext, "BIN" ) == 0 || 
		strcmp( ext, "BEFF" ) == 0 || 
		strcmp( ext, "BEFL" ) == 0 || 
		strcmp( ext, "BMHM" ) == 0 || 
		strcmp( ext, "BSAD" ) == 0 )
	{
		if( m_FileMng.AddFile( pFileName, "rb" ) )
		{
			m_FileList.AddString( m_FileMng.GetFileName( m_nFileNum ) );
			++m_nFileNum;
		}
	}
	else if( strcmp( ext, "DOF" ) == 0 )
	{
		if( m_FileMng.AddFile( pFileName, "dof" ) )
		{
			m_FileList.AddString( m_FileMng.GetFileName( m_nFileNum ) );
			++m_nFileNum;
		}
	}
	else 
	{
		if( m_FileMng.AddFile( pFileName, "rt" ) )
		{
			m_FileList.AddString( m_FileMng.GetFileName( m_nFileNum ) );
			++m_nFileNum;
		}
	}
	UpdateList();
}

void CPackingToolDlg::UpdateList()
{
	if( m_nFileNum )
	{
		m_sFileNum.Format( "%d", m_nFileNum );
		m_BtnDelAll.EnableWindow( TRUE );
		m_BtnSaveAllToBin.EnableWindow( TRUE );
		m_BtnSaveAllToTxt.EnableWindow( TRUE );
		m_BtnSaveAsBin.EnableWindow( TRUE );
		m_BtnSave.EnableWindow( TRUE );
		// list
		m_FileList.SetCurSel( m_nFileNum-1 );
		// edit
		m_Edit.EnableWindow( TRUE );
		CMHFileEx* pFile = m_FileMng.GetFile( m_nFileNum-1 );
		m_Edit.SetWindowText( pFile->GetData() );
		
		m_sCurFile = pFile->GetFullFileName();
		m_nOldIndex = m_nFileNum-1;
	}
}

/*
void CPackingToolDlg::OnDropFiles(HDROP hDropInfo) 
{
	char filename[MAX_PATH] = {0,};
	char ext[MAX_PATH] = {0,};
	if( ::DragQueryFile( hDropInfo, 0, filename, MAX_PATH ) )
	{
		int len = strlen(filename);
		int i, j;

		for( i = len-1; i >= 0; --i )
		{
			if( filename[i] == '.' )
			{
				j = i;
				break;
			}
		}
		for( i = 0; i < len-j; ++i )
			ext[i] = filename[j+1+i];

		if( strcmp( ext, "bin" ) == 0 || strcmp( ext, "beff" ) == 0 || strcmp( ext, "befl" ) == 0
			|| strcmp( ext, "bmhm" ) == 0 )
		{
			if( m_FileMng.AddFile( filename, "rb" ) )
			{
				m_FileList.AddString( m_FileMng.GetFileName( m_nFileNum ) );
				++m_nFileNum;
			}
		}
		else
		{
			if( m_FileMng.AddFile( filename, "rt" ) )
			{
				m_FileList.AddString( m_FileMng.GetFileName( m_nFileNum ) );
				++m_nFileNum;
			}
		}
		
		if( m_nFileNum )
		{
			m_sFileNum.Format( "%d", m_nFileNum );
			m_BtnDelAll.EnableWindow( TRUE );
			m_BtnSaveAllToBin.EnableWindow( TRUE );
			m_BtnSaveAllToTxt.EnableWindow( TRUE );
			m_BtnSaveAsBin.EnableWindow( TRUE );
			//		m_BtnOpenBin.EnableWindow( FALSE );
			m_BtnSave.EnableWindow( TRUE );
			// list
			m_FileList.SetCurSel( m_nFileNum-1 );
			// edit
			m_Edit.EnableWindow( TRUE );
			CMHFileEx* pFile = m_FileMng.GetFile( m_nFileNum-1 );
			m_Edit.SetWindowText( pFile->GetData() );
			
			m_sCurFile = pFile->GetFullFileName();
			m_nOldIndex = m_nFileNum-1;
		}
	}
	
	UpdateData( FALSE );

	CDialog::OnDropFiles(hDropInfo);
}*/

void CPackingToolDlg::OnSearch() 
{
	int a = 0;	
}

void CPackingToolDlg::OnButtonSavedsof() 
{
	SaveData( m_FileList.GetCurSel() );
	m_FileMng.SaveToDOF();
}
