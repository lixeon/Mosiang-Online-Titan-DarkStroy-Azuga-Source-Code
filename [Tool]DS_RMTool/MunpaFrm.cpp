// MunpaFrm.cpp : implementation file
//

#include "stdafx.h"
#include "DS_RMTool.h"
#include "MunpaFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMunpaFrm

IMPLEMENT_DYNCREATE(CMunpaFrm, CMDIChildWnd)

CMunpaFrm::CMunpaFrm()
{
}

CMunpaFrm::~CMunpaFrm()
{
}


BEGIN_MESSAGE_MAP(CMunpaFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(CMunpaFrm)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMunpaFrm message handlers

BOOL CMunpaFrm::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
//		| FWS_ADDTOTITLE;

	cs.cx = 1100;
	cs.cy = 750;

	return TRUE;
}
