// AdminFrm.cpp : implementation file
//

#include "stdafx.h"
#include "DS_RMTool.h"
#include "AdminFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdminFrm

IMPLEMENT_DYNCREATE(CAdminFrm, CMDIChildWnd)

CAdminFrm::CAdminFrm()
{
}

CAdminFrm::~CAdminFrm()
{
}


BEGIN_MESSAGE_MAP(CAdminFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(CAdminFrm)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdminFrm message handlers

BOOL CAdminFrm::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	cs.cx = 510;
	cs.cy = 530;
	cs.lpszName = "Admin Mode";

	return TRUE;
}
