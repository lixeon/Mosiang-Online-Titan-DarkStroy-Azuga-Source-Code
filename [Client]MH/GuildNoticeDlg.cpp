#include "stdafx.h"
#include "./Interface\cTextArea.h"
#include "WindowIDEnum.h"
#include "GuildNoticeDlg.h"
#include "GuildManager.h"

CGuildNoticeDlg::CGuildNoticeDlg(void)
{
}

CGuildNoticeDlg::~CGuildNoticeDlg(void)
{
}

void CGuildNoticeDlg::Linking()
{
	m_pNoticeText = (cTextArea*)GetWindowForID(GNotice_TEXTREA);
	
	m_pNoticeText->SetEnterAllow(FALSE);
	m_pNoticeText->SetScriptText("");
}

void CGuildNoticeDlg::OnActionEvnet(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		switch(lId)
		{
		case GNotice_SENDOKBTN:
			{
				char notice[MAX_GUILD_NOTICE+1] = {0,};

				m_pNoticeText->GetScriptText(notice);
				GUILDMGR->SetGuildNotice(notice);

				SetActive(FALSE);
			}
			break;
		case GNotice_CANCELBTN:
			{
				SetActive(FALSE);
			}
			break;
		}
	}
}

void CGuildNoticeDlg::SetActive(BOOL val)
{
	if(val == TRUE)
	{
		if(GUILDMGR->GetGuildNotice())
			m_pNoticeText->SetScriptText(GUILDMGR->GetGuildNotice());
	}

	cDialog::SetActive(val);
}
