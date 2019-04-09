#include "stdafx.h"
#include ".\guildjoindialog.h"
#include "ObjectManager.h"
#include "ChatManager.h"
#include "GuildManager.h"

#include "WindowIdEnum.h"

CGuildJoinDialog::CGuildJoinDialog(void)
{
}

CGuildJoinDialog::~CGuildJoinDialog(void)
{
}

void CGuildJoinDialog::Linking()
{

}
	
void CGuildJoinDialog::OnActionEvent(LONG lId, void* p, DWORD we)
{
	switch(lId)
	{
	case JO_MEMBERBTN:
		{
			if(HERO->GetGuildMemberRank() < GUILD_VICEMASTER)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(297) );
				return;
			}
			CObject * targetObj = OBJECTMGR->GetSelectedObject();
			if(targetObj)
			{
				if(targetObj->GetObjectKind()  == eObjectKind_Player)
				{
					if(((CPlayer*)targetObj)->GetGuildIdx())
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(38));
						return;
					}
					else
						GUILDMGR->AddMemberSyn(targetObj->GetID());
				}
			}
		}
		break;
	case JO_STUDENTBTN:
		{
			if(GUILDMGR->GetLevel() < GUILD_5LEVEL)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1368));
				return;
			}

			CObject * targetObj = OBJECTMGR->GetSelectedObject();
			if(targetObj)
			{
				if(targetObj->GetObjectKind()  == eObjectKind_Player)
				{
					if(((CPlayer*)targetObj)->GetGuildIdx())
					{
						CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(38));
						//return;
					}
					else
						GUILDMGR->AddStudentSyn(targetObj->GetID(), ((CPlayer*)targetObj)->GetLevel());
				}
			}
		}
		break;
	case JO_CANCELBTN:		//기획자에게 말해서 기본 CANCLE로 바꾸자.
		{
			//SetActive(FALSE);
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	SetActive(FALSE);

}