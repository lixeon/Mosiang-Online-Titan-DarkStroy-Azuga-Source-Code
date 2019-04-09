// GTRegistDialog.cpp: implementation of the CGTRegistDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GTRegistDialog.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"
#include "WindowIDEnum.h"
#include "./Interface/cStatic.h"
#include "./Interface/cButton.h"
#include "Guild.h"
#include "GuildManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGTRegistDialog::CGTRegistDialog()
{
	m_type = WT_GTREGIST_DLG;

}

CGTRegistDialog::~CGTRegistDialog()
{

}


void CGTRegistDialog::Linking()
{
	m_pRegistGuild = (cStatic*)GetWindowForID(GDT_ENTRY1);
	m_pRegistableGuild = (cStatic*)GetWindowForID(GDT_ENTRY2);
	m_pRegistBtn = (cButton*)GetWindowForID(GDT_ENTRYBTN);
}


void CGTRegistDialog::SetActive( BOOL val )
{
	cDialog::SetActive( val );

	if( !val )
	{
		if( HERO->GetState() == eObjectState_Deal )
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	}
}


DWORD CGTRegistDialog::TournamentRegistSyn()
{
	// 잠시 막음.....
//	return eGTError_NOTREGISTDAY;
	
	if( HERO->GetGuildMemberRank() != GUILD_MASTER )
		return eGTError_NOGUILDMASTER;
/*	if( GUILDMGR->GetGuild()->GetLevel() < LIMITLEVEL_GUILDTOURNAMENT )
		return eGTError_UNDERLEVEL;
	if( GUILDMGR->GetGuild()->GetMemberNum() < LIMITMEMBER_GUILDTOURNAMENT )
		return eGTError_UNDERLIMITEMEMBER;*/

	MSGBASE msg;
	msg.Category = MP_GTOURNAMENT;
	msg.Protocol = MP_GTOURNAMENT_REGIST_SYN;
	msg.dwObjectID = HEROID;
	NETWORK->Send( &msg, sizeof(msg) );

	return eGTError_NOERROR;
}

void CGTRegistDialog::SetRegistGuildCount( DWORD count )
{
	m_pRegistGuild->SetStaticValue( count );
	m_pRegistableGuild->SetStaticValue( MAXGUILD_INTOURNAMENT-count );
}