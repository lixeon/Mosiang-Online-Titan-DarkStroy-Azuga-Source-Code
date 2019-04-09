// DialogConsole.cpp : implementation file
//

#include "stdafx.h"
#include "4DyuchiGXMapEditor.h"
#include "DialogConsole.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "tool.h"

char SET_CAMERA_EYE[] = "SetCameraEye";
char SET_CAMERA_LOOK[] = "SetCameraLook";
char SEP[] =" ,\t\n";

/////////////////////////////////////////////////////////////////////////////
// CDialogConsole dialog



CDialogConsole::CDialogConsole(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogConsole::IDD, pParent)
{
//	Create(CDialogConsole::IDD,pParent);

	//{{AFX_DATA_INIT(CDialogConsole)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogConsole::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogConsole)
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogConsole, CDialog)
	//{{AFX_MSG_MAP(CDialogConsole)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogConsole message handlers



BOOL CDialogConsole::Initialize(I4DyuchiGXRenderer* pRenderer,HWND hWndParent)
{
	m_pRenderer = pRenderer;
	m_hWndParent = hWndParent;


	return TRUE;
}
CDialogConsole::~CDialogConsole()
{

}

void CDialogConsole::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

CDialogConsole*	g_pConsole = NULL;

LRESULT CALLBACK EditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
HWND	hParent = NULL;
WNDPROC	OldEditProc = NULL;


BOOL CDialogConsole::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	g_pConsole = this;
	HWND hEdit = ::GetDlgItem(m_hWnd,IDC_EDIT1);
	OldEditProc = (WNDPROC)SetWindowLong (hEdit,GWL_WNDPROC,LONG(EditProc));
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogConsole::Process(char* txt,DWORD dwLen)
{
/*	
	char*	func = strtok(txt,SEP);
	
	if (!strcmp(func,SET_CAMERA_EYE))
	{
		VECTOR3	v3;
		ConvertStrToVector3(&v3);
		g_pTool->GetGeometry()->SetCameraEye(&v3);
	}
	else if (!strcmp(func,SET_CAMERA_LOOK))
	{
		VECTOR3	v3;
		ConvertStrToVector3(&v3);
		g_pTool->GetGeometry()->SetCameraLook(&v3);
	}
*/
}

void CDialogConsole::ConvertStrToVector3(VECTOR3* pv3Out)
{
	float*	value = (float*)pv3Out;

	for (DWORD i=0; i<3; i++)
	{
		char* arg = strtok(NULL,SEP);
		if (!arg)
			break;
				
		*value = (float)atof(arg);
		value++;
	}
}

void CDialogConsole::OnEnter()
{
	char txt[512];
	memset(txt,0,512);


	m_edit.GetLine(0,txt,512);
	m_edit.SetWindowText(NULL);
	
	
	DWORD	len = lstrlen(txt);

	Process(txt,len);
	g_pTool->SetConsoleText(txt,len);
	::SetFocus(m_hWndParent);
}

LRESULT CALLBACK EditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_GETDLGCODE: return DLGC_WANTALLKEYS;
		case WM_KEYDOWN:
			switch(LOWORD(wParam))
			{
				case VK_RETURN:
					{
						g_pConsole->OnEnter();

					}
					return 1;
			}
			break;
	}
	return CallWindowProc(OldEditProc,hWnd,message,wParam,lParam);
}

