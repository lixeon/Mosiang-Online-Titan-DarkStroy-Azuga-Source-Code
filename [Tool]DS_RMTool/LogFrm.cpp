// LogFrm.cpp : implementation file
//

#include "stdafx.h"
#include "ds_rmtool.h"
#include "LogFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogFrm

IMPLEMENT_DYNCREATE(CLogFrm, CMDIChildWnd)

CLogFrm::CLogFrm()
{
}

CLogFrm::~CLogFrm()
{
}


BEGIN_MESSAGE_MAP(CLogFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(CLogFrm)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogFrm message handlers

BOOL CLogFrm::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	cs.cx = 1200;
	cs.cy = 840;
	
	return CMDIChildWnd::PreCreateWindow(cs);
}
