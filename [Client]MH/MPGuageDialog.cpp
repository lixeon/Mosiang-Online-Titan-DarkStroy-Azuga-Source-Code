// MPGuageDialog.cpp: implementation of the CMPGuageDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MPGuageDialog.h"
#include "./ObjectGuagen.h"
#include "WindowIDEnum.h"
#include "./interface/cstatic.h"
#include "ChatManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMPGuageDialog::CMPGuageDialog()
{
	m_type = WT_MPGUAGEDLG;
}

CMPGuageDialog::~CMPGuageDialog()
{

}

void CMPGuageDialog::Linking()
{
	m_ExpGuage = (CObjectGuagen *)GetWindowForID(MP_GEXPGUAGE);
	m_Time = (cStatic *)GetWindowForID(MP_GTIME);
	m_ExpPercent = (cStatic *)GetWindowForID(MP_GEXPERCENT);
	m_pTitle = (cStatic*)GetWindowForID(MP_TITLE);
}

void CMPGuageDialog::SetExpGuage(float Percent)
{
	m_ExpGuage->SetValue(Percent, 0);
	char temp[128];
	sprintf(temp,"%4.2f%%",Percent*100);
	m_ExpPercent->SetStaticText(temp);
}

void CMPGuageDialog::SetTime(DWORD RemainTime)
{
	char buf[16];
	if(RemainTime < 30000)
		m_Time->SetFGColor(RGB_HALF(255, 0, 0));
	sprintf(buf, "%02d:%02d", RemainTime/60000, (RemainTime%60000)/1000);
	m_Time->SetStaticText(buf);
}

void CMPGuageDialog::SetEventMapTimer( DWORD RemainTime, BYTE bFlag )
{
	char buf[16];
	switch( bFlag )
	{
	case 0:		//ready
		m_Time->SetFGColor( RGB_HALF(0, 0, 255) );
		break;
	case 1:
		{
			if( RemainTime < 30000 )
				m_Time->SetFGColor(RGB_HALF(255, 0, 0));
		}
		break;
	case 2:
		m_Time->SetFGColor( RGB_HALF(0, 0, 255) );
		break;
	}

	sprintf( buf, "%02d:%02d", RemainTime/60000, (RemainTime%60000)/1000 );
	m_Time->SetStaticText( buf );
}

void CMPGuageDialog::ShowEventMap()
{
	SetActive( TRUE );
	
	m_pTitle->SetStaticText( CHATMGR->GetChatMsg( 140 ) );
}
