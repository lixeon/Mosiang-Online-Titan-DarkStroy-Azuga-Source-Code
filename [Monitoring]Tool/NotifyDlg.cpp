// NotifyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ServerTool.h"
#include "NotifyDlg.h"
#include "NotifyMessage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNotifyDlg dialog


CNotifyDlg::CNotifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNotifyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNotifyDlg)
	m_strCurFileName = _T("");
	//}}AFX_DATA_INIT
}


void CNotifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNotifyDlg)
	DDX_Control(pDX, IDC_EDIT_NOTIFY, m_NotifyEdit);
	DDX_Text(pDX, IDC_STATIC_FILENAME, m_strCurFileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNotifyDlg, CDialog)
	//{{AFX_MSG_MAP(CNotifyDlg)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_NOTIFY, OnButtonOpenNotify)
	ON_BN_CLICKED(IDC_BUTTON_SAVEAS_NOTIFY, OnButtonSaveasNotify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNotifyDlg message handlers

BOOL CNotifyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	OpenFile();	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNotifyDlg::Init( CString filename, CNotifyMessage* pNotifyMessage )
{
	m_strCurFileName = filename;
	m_pNotifyMessage = pNotifyMessage;
}

void CNotifyDlg::OnButtonOpenNotify() 
{
	CFileDialog dlg( TRUE, NULL, "*.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
					 "txt 파일 (*.txt)|*.txt||", NULL );

	if( dlg.DoModal() == IDOK )
	{
		m_strCurFileName = dlg.GetFileName();
		OpenFile();
	}

	UpdateData( FALSE );
}

void CNotifyDlg::OnButtonSaveasNotify() 
{
	CFileDialog dlg( FALSE, NULL, "*.txt", OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
					 "txt 파일|*.txt (*.txt)|*.txt||", NULL );

	if( dlg.DoModal() == IDOK )
	{
		m_strCurFileName = dlg.GetFileName();
		SaveFile();
	}	

	UpdateData( FALSE );
}

void CNotifyDlg::OpenFile()
{
	if( m_strCurFileName != "None" && m_strCurFileName != "" )
	{
		m_pNotifyMessage->Open( (char*)(LPCTSTR)m_strCurFileName );
		
		// edit setting
		char temp[256];
		CString str = "";
		
		str = str + "*MAX ";
		str = str + m_pNotifyMessage->GetMessage( 0 );
		str = str + "\r\n";
		//str = str + 0x0d;	// enter
		//str= str + 0x0a;
		for( int i = 1; i <= m_pNotifyMessage->GetMaxLine(); ++i )
		{
			str = str + itoa( i, temp, 10 );
			str = str + ' ';
			str = str + '"';
			str = str + m_pNotifyMessage->GetMessage( i );
			str = str + '"';
			str = str + "\r\n";
			//str = str + 0x0d;
			//str = str + 0x0a;
		}
		
		m_NotifyEdit.SetWindowText( str );
	}
}

void CNotifyDlg::SaveFile()
{
	if( m_strCurFileName != "None" && m_strCurFileName != "" )
	{
		CString str;
		m_NotifyEdit.GetWindowText( str );

		FILE* file = fopen( m_strCurFileName, "w+t" );
		if( file )
			fwrite( (LPCTSTR)str, sizeof(char), str.GetLength(), file );
		fclose( file );
	}
}

void CNotifyDlg::OnOK() 
{
	SaveFile();
	
	CDialog::OnOK();
}
