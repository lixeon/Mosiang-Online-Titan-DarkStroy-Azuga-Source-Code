// MyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "PackingTool.h"
#include "MyEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyEdit

CMyEdit::CMyEdit()
{
	m_bFlag = FALSE;
	
}

CMyEdit::~CMyEdit()
{
}


BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	//{{AFX_MSG_MAP(CMyEdit)
	ON_WM_CREATE()
	ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyEdit message handlers

BOOL CMyEdit::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB )
	{
		if( m_bFlag == FALSE )
		{
			keybd_event( VK_CONTROL, MapVirtualKey(VK_CONTROL,0), 0, 0 );  
			keybd_event( VK_TAB, MapVirtualKey(VK_TAB,0), 0, 0 );
			keybd_event( VK_TAB, MapVirtualKey(VK_TAB,0), KEYEVENTF_KEYUP, 0 ); 
			keybd_event( VK_CONTROL, MapVirtualKey(VK_CONTROL,0), KEYEVENTF_KEYUP, 0 );
			m_bFlag = TRUE;
			return TRUE;
		}
		else
		{
			m_bFlag = FALSE;
		}
	}
	
	return CEdit::PreTranslateMessage(pMsg);
}


BOOL CMyEdit::PreCreateWindow(CREATESTRUCT& cs) 
{
	this->DragAcceptFiles();
	return CEdit::PreCreateWindow(cs);
}

int CMyEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	
	
	return 0;
}

void CMyEdit::OnDropFiles(HDROP hDropInfo) 
{
	int a = 0;
	
	CEdit::OnDropFiles(hDropInfo);
}
