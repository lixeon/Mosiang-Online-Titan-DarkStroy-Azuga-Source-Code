// GuildRankDialog.cpp: implementation of the CGuildRankDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildRankDialog.h"
#include "WindowIDEnum.h"
#include "./interface/cTextArea.h"
#include "ChatManager.h"
#include "GuildManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuildRankDialog::CGuildRankDialog()
{
	m_type = WT_GUILDRANKDLG;
	m_CurGuildRankMode = 255;
}

CGuildRankDialog::~CGuildRankDialog()
{
	for(int i=0; i<eRankMode_Max;  ++i)
		m_GuildRankCtrlList[i].RemoveAll();
}

void CGuildRankDialog::Linking()
{
	m_pRankMemberName = (cTextArea*)GetWindowForID(GDR_MEMBERNAME);
	m_pDRankComboBox = (cComboBox*)GetWindowForID(GDR_DRANKCOMBOBOX);
	m_pRankComboBox = (cComboBox*)GetWindowForID(GDR_RANKCOMBOBOX);
	m_pDOkBtn = (cButton*)GetWindowForID(GDR_DOKBTN);
	m_pOkBtn = 	(cButton*)GetWindowForID(GDR_OKBTN);

	m_GuildRankCtrlList[eRankMode_Dan].AddTail(m_pDRankComboBox);
	m_GuildRankCtrlList[eRankMode_Dan].AddTail(m_pDOkBtn);

	m_GuildRankCtrlList[eRankMode_Guild].AddTail(m_pRankComboBox);
	m_GuildRankCtrlList[eRankMode_Guild].AddTail(m_pOkBtn);
}

void CGuildRankDialog::SetActive(BOOL val)
{
	if(val == TRUE)
	{
		if((GUILDMGR->GetSelectedMemberID() == 0) || (GUILDMGR->GetSelectedMemberID() == HEROID))
		{
			if(IsActive() == TRUE)
				cDialog::SetActive(FALSE);
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(714));
			return;
		}
		SetName(GUILDMGR->GetSelectedMemberName());
	}
	cDialog::SetActive(val);
}

void CGuildRankDialog::ShowGuildRankMode(BYTE GuildLvl)
{	
	BYTE ShowMode;
	if(GuildLvl == MAX_GUILD_LEVEL)
		ShowMode = eRankMode_Guild;
	else 
		ShowMode = eRankMode_Dan;
	if(m_CurGuildRankMode == ShowMode)
		return;

	if(m_CurGuildRankMode != 255)
	{
		SetActiveGuildRankMode(m_CurGuildRankMode ,FALSE);
	}

	SetActiveGuildRankMode(ShowMode,TRUE);
	m_CurGuildRankMode = ShowMode;	
}

void CGuildRankDialog::SetActiveGuildRankMode(int showmode,BOOL bActive)
{
	PTRLISTSEARCHSTART(m_GuildRankCtrlList[showmode],cWindow*,pWin)
		pWin->SetActive(bActive);
	PTRLISTSEARCHEND
}

void CGuildRankDialog::SetName(char * name)
{
	char buf[64];
	sprintf(buf, CHATMGR->GetChatMsg(718), name);
	m_pRankMemberName->SetScriptText(buf);
}
