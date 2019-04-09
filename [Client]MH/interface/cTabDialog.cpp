// cTabDialog.cpp: implementation of the cTabDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cTabDialog.h"
#include "../Input/Mouse.h"
#include "cPushupButton.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cTabDialog::cTabDialog()
{
	m_ppPushupTabBtn=NULL;
	m_bTabNum=0;
	m_bSelTabNum=0;
	m_type = WT_TABDIALOG;
	m_ppWindowTabSheet=NULL;
	curIdx1 = 0;
	curIdx2 = 0;
//	m_BtnPushstartTime = 0;
//	m_BtnPushDelayTime = 700; 
}

cTabDialog::~cTabDialog()
{
	for(int i = 0 ; i < m_bTabNum ; i++)			// ÁÖÀÇ!: windowmanager¿¡ µî·ÏÇÏ°í ´Ù½Ã ¿©±â µî·ÏÇÒ¼ö ÀÖÀ¸´Ï..
	{
		SAFE_DELETE(m_ppPushupTabBtn[i]);
		SAFE_DELETE(m_ppWindowTabSheet[i]);
	}
	SAFE_DELETE_ARRAY(m_ppPushupTabBtn);
	SAFE_DELETE_ARRAY(m_ppWindowTabSheet);
	
}
void cTabDialog::InitTab(BYTE tabNum)
{
	curIdx1 = 0;
	curIdx2 = 0;
	m_bTabNum = tabNum;
	m_ppPushupTabBtn = new cPushupButton*[m_bTabNum];
	m_ppWindowTabSheet = new cWindow*[m_bTabNum];
	memset(m_ppPushupTabBtn, 0, sizeof(cPushupButton*)*m_bTabNum);
	memset(m_ppWindowTabSheet, 0, sizeof(cWindow*)*m_bTabNum);
}

void cTabDialog::SetAlpha(BYTE al)
{
	cDialog::SetAlpha(al);
	for(int i = 0 ; i < m_bTabNum ; i++)
	{
		m_ppPushupTabBtn[i]->SetAlpha(al);
		m_ppWindowTabSheet[i]->SetAlpha(al);
	}
}

//KES 030825
void cTabDialog::SetOptionAlpha( DWORD dwAlpha)
{
	cDialog::SetOptionAlpha( dwAlpha );

	for(int i = 0 ; i < m_bTabNum ; i++)
	{
		m_ppPushupTabBtn[i]->SetOptionAlpha( dwAlpha );
		m_ppWindowTabSheet[i]->SetOptionAlpha( dwAlpha );
	}
}

void cTabDialog::AddTabBtn(BYTE idx, cPushupButton * btn)
{
	ASSERT(idx < m_bTabNum);
	ASSERT(!m_ppPushupTabBtn[idx]);
	btn->SetAbsXY((LONG)(m_absPos.x+btn->m_relPos.x), (LONG)(m_absPos.y+btn->m_relPos.y));
	btn->SetParent(this);
	btn->SetPassive(TRUE);
	if(idx == m_bSelTabNum)
		btn->SetPush(TRUE);
	else
		btn->SetPush(FALSE);
	m_ppPushupTabBtn[idx] = btn;
}
void cTabDialog::AddTabSheet(BYTE idx, cWindow * sheet)
{
	ASSERT(idx < m_bTabNum);
	ASSERT(!m_ppWindowTabSheet[idx]);
	sheet->SetAbsXY((LONG)(m_absPos.x+sheet->m_relPos.x), (LONG)(m_absPos.y+sheet->m_relPos.y));
	sheet->SetParent(this);
	m_ppWindowTabSheet[idx] = sheet;
}


DWORD cTabDialog::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if(!m_bActive) return we;
	
	we = cDialog::ActionEvent(mouseInfo);

	DWORD we2 = WE_NULL;
	for( int i = 0 ; i < m_bTabNum ; i++ )
	{
		we2 = m_ppPushupTabBtn[i]->ActionEvent( mouseInfo );

		if( we2 & WE_PUSHDOWN && m_bSelTabNum != i )//&& m_ppPushupTabBtn[i]->PtInWindow(mouseInfo->GetMouseX(), mouseInfo->GetMouseY()) && mouseInfo->LButtonPressed()))
		{
			SelectTab(i);		//¼ø¼­ÁöÅ°±â(¼ö·ÃÃ¢¶§¹®..)
			m_bSelTabNum = i;
		}
	}

	we |= m_ppWindowTabSheet[m_bSelTabNum]->ActionEvent( mouseInfo );

	return we;
}

void cTabDialog::SelectTab(BYTE idx)
{
	if( idx >= m_bTabNum ) return;
	
	m_ppPushupTabBtn[idx]->SetPush(TRUE);
	m_ppWindowTabSheet[idx]->SetActive(TRUE);
	m_bSelTabNum = idx;
	for(int j = 0 ; j < m_bTabNum ; j++)
	{
		if(idx != j)
		{
			m_ppPushupTabBtn[j]->SetPush(FALSE);
			m_ppWindowTabSheet[j]->SetActive(FALSE);
		}
	}
}

void cTabDialog::Render()
{
	cDialog::RenderWindow();
	cTabDialog::RenderTabComponent();
	cDialog::RenderComponent();
}

void cTabDialog::RenderTabComponent()
{
	if(!m_bActive) return;

	for(int i = 0 ; i < m_bTabNum ; i++)
	{
		if(m_bSelTabNum == i)
		{
			m_ppWindowTabSheet[i]->Render();
		}
		m_ppPushupTabBtn[i]->Render();
	}
}

void cTabDialog::SetAbsXY(LONG x, LONG y)
{
	LONG tmpX = x - (LONG)m_absPos.x;
	LONG tmpY = y - (LONG)m_absPos.y;

	for(int i = 0 ; i < m_bTabNum ; i++)
	{
		m_ppPushupTabBtn[i]->SetAbsXY((LONG)m_ppPushupTabBtn[i]->GetAbsX()+tmpX,
											(LONG)m_ppPushupTabBtn[i]->GetAbsY()+tmpY);
		m_ppWindowTabSheet[i]->SetAbsXY((LONG)m_ppWindowTabSheet[i]->GetAbsX()+tmpX,
											(LONG)m_ppWindowTabSheet[i]->GetAbsY()+tmpY);
	}
	cDialog::SetAbsXY(x,y);
}

void cTabDialog::SetActive(BOOL val)
{
	if( m_bDisable ) return;
	
	for(int i = 0 ; i < m_bTabNum ; i++)
	{
		m_ppPushupTabBtn[i]->SetActive(val);
		if(val && i == m_bSelTabNum)
			m_ppWindowTabSheet[i]->SetActive(val);
		else if(!val)
			m_ppWindowTabSheet[i]->SetActive(val);
	}

	cDialog::SetActiveRecursive(val);
}

cPushupButton * cTabDialog::GetTabBtn(BYTE idx)
{
	ASSERT(idx < m_bTabNum);
	return m_ppPushupTabBtn[idx];
}

cWindow * cTabDialog::GetTabSheet(BYTE idx)
{
	ASSERT(idx < m_bTabNum);
	return m_ppWindowTabSheet[idx];
}


void cTabDialog::SetDisable( BOOL val )
{
	cDialog::SetDisable( val );

	for(int i = 0 ; i < m_bTabNum ; i++)
	{
		m_ppPushupTabBtn[i]->SetDisable(val);
		m_ppWindowTabSheet[i]->SetDisable(val);
	}
}


cWindow* cTabDialog::GetWindowForID(LONG id)
{
	cWindow* pWindow = cDialog::GetWindowForID( id );
	if( !pWindow )
	{
		for(int i = 0 ; i < m_bTabNum ; i++)
		{
			if( m_ppPushupTabBtn[i]->GetID() == id )
			{
				pWindow = m_ppPushupTabBtn[i];
				break;
			}
			if( m_ppWindowTabSheet[i]->GetID() == id )
			{
				pWindow = m_ppWindowTabSheet[i];
				break;
			}
		}
	}

	return pWindow;
}