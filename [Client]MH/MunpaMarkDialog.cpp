// MunpaMarkDialog.cpp: implementation of the CMunpaMarkDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MunpaMarkDialog.h"
#include "MunpaMarkManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMunpaMarkDialog::CMunpaMarkDialog()
{
	m_pMunpaMark = NULL;
	m_type = WT_MUNPAMARKDLG;

//	if(MUNPAMARKMGR->IsInited() == FALSE)
//		MUNPAMARKMGR->Init();
}

CMunpaMarkDialog::~CMunpaMarkDialog()
{
	
}

void CMunpaMarkDialog::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)
{
	cDialog::Init(x,y,wid,hei,basicImage,ID);
	m_type = WT_MUNPAMARKDLG;
}

void CMunpaMarkDialog::Render()
{
	cDialog::Render();
	if(m_pMunpaMark)
		m_pMunpaMark->Render(&m_absPos);
}

BOOL CMunpaMarkDialog::SetMunpaMark(DWORD MunpaID)
{
	m_pMunpaMark = MUNPAMARKMGR->GetMunpaMark(MunpaID);
	if(m_pMunpaMark)
		return TRUE;
	else return FALSE;
}
