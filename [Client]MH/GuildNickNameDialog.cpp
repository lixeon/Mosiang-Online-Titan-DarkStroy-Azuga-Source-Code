// GuildNickNameDialog.cpp: implementation of the CGuildNickNameDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildNickNameDialog.h"
#include "./interface/cTextArea.h"
#include "./interface/cEditBox.h"
#include "cImeEx.h"
#include "WindowIDEnum.h"
#include "ChatManager.h"
#include "GuildManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuildNickNameDialog::CGuildNickNameDialog()
{
	m_type = WT_GUILDNICKNAMEDLG;
}

CGuildNickNameDialog::~CGuildNickNameDialog()
{

}

void CGuildNickNameDialog::Linking()
{
	m_pNickMsg = (cTextArea*)GetWindowForID(GD_NICKTEXTAREA);
	m_pNickName = (cEditBox*)GetWindowForID(GD_NICKNAMEEDIT);
	m_pNickName->SetValidCheck( VCM_SPACE );
}

void CGuildNickNameDialog::SetActive(BOOL val)
{
	if(val == TRUE)
	{
		if(GUILDMGR->GetSelectedMemberID() == 0)
		{			
			cDialog::SetActive(FALSE);
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(714));
			return;
		}
		m_pNickName->SetEditText("");
		SetNickMsg(GUILDMGR->GetSelectedMemberName());
	}
	else
	{
		m_pNickName->SetFocusEdit(FALSE);
	}
	cDialog::SetActive(val);
}

void CGuildNickNameDialog::SetNickMsg(char* Name)
{
	char text[128];
	sprintf(text, CHATMGR->GetChatMsg(704), Name);
	m_pNickMsg->SetScriptText(text);
}

