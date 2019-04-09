// MainDialog.cpp: implementation of the CMainDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDialog.h"
#include "WindowIDEnum.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMainDialog::CMainDialog()
{
	m_type = WT_MAINDIALOG;
	for(int i = 0 ; i < MAX_BTN ; i++)
		m_pBtn[i]= NULL;
}

CMainDialog::~CMainDialog()
{

}

void CMainDialog::Add(cWindow * window)
{
	if(!m_pBtn[INVENTORY_BTN] && window->GetID() == MI_INVENTORYBTN)
	{
		m_pBtn[INVENTORY_BTN] = (cPushupButton *)window;
	}
	else if(!m_pBtn[MUGONG_BTN] && window->GetID() == MI_MUGONGBTN)
	{
		m_pBtn[MUGONG_BTN] = (cPushupButton *)window;
	}
	else if(!m_pBtn[CHAR_BTN] && window->GetID() == MI_CHARBTN)
	{
		m_pBtn[CHAR_BTN] = (cPushupButton *)window;
	}
	else if(!m_pBtn[PARTY_BTN] && window->GetID() == MI_PARTYBTN)
	{
		m_pBtn[PARTY_BTN] = (cPushupButton *)window;
	}
	
	cDialog::Add(window);
}