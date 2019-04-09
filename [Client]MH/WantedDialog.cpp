// WantedDialog.cpp: implementation of the CWantedDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WantedDialog.h"
#include "WindowIDEnum.h"
#include ".\interface\cListDialog.h"
#include "ChatManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWantedDialog::CWantedDialog()
{
	m_type = WT_WANTEDDIALOG;
}

CWantedDialog::~CWantedDialog()
{

}

void CWantedDialog::Linking()
{
	m_pWantedLDG = (cListDialog*)GetWindowForID(QUE_WANTEDLDLG);
}

void CWantedDialog::SetInfo(WANTEDLIST* pInfo)
{	
	InitWanted();
	
	char temp[128];
	for(int i=0; i<MAX_WANTED_NUM; ++i)
	{
		if(pInfo[i].WantedIDX == 0)
			break;
		m_pWantedLDG->AddItem(pInfo[i].RegistDate, 0xffffffff);
		sprintf(temp,CHATMGR->GetChatMsg(545),pInfo[i].WantedName);
		m_pWantedLDG->AddItem(temp, 0xffffffff);
	}
	m_pWantedLDG->ResetGuageBarPos();
}

void CWantedDialog::AddInfo(WANTEDLIST* pInfo)
{
	char temp[128];
	m_pWantedLDG->AddItem(pInfo->RegistDate, 0xffffffff);
	sprintf(temp,CHATMGR->GetChatMsg(545),pInfo->WantedName);
	m_pWantedLDG->AddItem(temp , 0xffffffff);
}

void CWantedDialog::InitWanted()
{
	m_pWantedLDG->RemoveAll();
}
