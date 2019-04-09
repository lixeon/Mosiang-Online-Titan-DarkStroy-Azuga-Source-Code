// CharacterFrm.cpp : implementation file
//

#include "stdafx.h"
#include "DS_RMTool.h"
#include "CharacterFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCharacterFrm

IMPLEMENT_DYNCREATE(CCharacterFrm, CMDIChildWnd)

CCharacterFrm::CCharacterFrm()
{
}

CCharacterFrm::~CCharacterFrm()
{
}


BEGIN_MESSAGE_MAP(CCharacterFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(CCharacterFrm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCharacterFrm message handlers

BOOL CCharacterFrm::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	cs.cx = 1280;
	cs.cy = 840;
	cs.lpszName = "Character Info";

	return TRUE;
}
