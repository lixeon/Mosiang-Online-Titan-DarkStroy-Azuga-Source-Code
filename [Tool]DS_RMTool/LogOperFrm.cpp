// LogOperFrm.cpp : implementation file
//

#include "stdafx.h"
#include "ds_rmtool.h"
#include "LogOperFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogOperFrm

IMPLEMENT_DYNCREATE(CLogOperFrm, CMDIChildWnd)

CLogOperFrm::CLogOperFrm()
{
}

CLogOperFrm::~CLogOperFrm()
{
}


BEGIN_MESSAGE_MAP(CLogOperFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(CLogOperFrm)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogOperFrm message handlers

BOOL CLogOperFrm::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	cs.cx = 1200;
	cs.cy = 840;

	return CMDIChildWnd::PreCreateWindow(cs);
}
