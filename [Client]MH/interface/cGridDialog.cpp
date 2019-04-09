// cGridDialog.cpp: implementation of the cGridDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cGridDialog.h"
#include "cPushupButton.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cGridDialog::cGridDialog()
{
	m_type = WT_GRIDDIALOG;
}

cGridDialog::~cGridDialog()
{

}
void cGridDialog::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, cPushupButton * pCellWindow, WORD cellNum, LONG ID)
{
	cDialog::Init(x,y,wid,hei,basicImage,ID);
	m_type = WT_GRIDDIALOG;
	m_pWindowCell = pCellWindow; 
	m_wCellNum = cellNum;

	for(int i = 0 ; i < cellNum ; i++)
	{
		cPushupButton * node = new cPushupButton;
		memcpy(node, &pCellWindow[i], sizeof(cPushupButton));
		cDialog::Add(node);
	}

	SAFE_DELETE_ARRAY(pCellWindow);
}
