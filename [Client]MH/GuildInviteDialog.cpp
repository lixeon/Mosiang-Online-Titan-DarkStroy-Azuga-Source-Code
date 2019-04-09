// GuildInviteDialog.cpp: implementation of the CGuildInviteDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildInviteDialog.h"
#include "./interface/cTextArea.h"
#include "GuildManager.h"
#include "ChatManager.h"
#include "WindowIDEnum.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuildInviteDialog::CGuildInviteDialog()
{
	m_type = WT_GUILDINVITEDLG;
}

CGuildInviteDialog::~CGuildInviteDialog()
{

}

void CGuildInviteDialog::Linking()
{
	m_pInviteMsg = (cTextArea*)GetWindowForID(GD_IINVITE);                                                                                                                                                                                                                                                             
	
}

void CGuildInviteDialog::SetInfo(char* GuildName, char* MasterName, int FlgKind)
{
	char text[128];

	if(FlgKind == AsMember)
	{
		sprintf(text, CHATMGR->GetChatMsg(45), GuildName, MasterName);
	}
	else	//AsStudent
	{
		sprintf(text, CHATMGR->GetChatMsg(1370), MasterName, GuildName);
	}
	m_pInviteMsg->SetScriptText(text);
}