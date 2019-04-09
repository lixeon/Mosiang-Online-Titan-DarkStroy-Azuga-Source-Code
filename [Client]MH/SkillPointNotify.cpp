// SkillPointNotify.cpp: implementation of the CSkillPointNotify class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillPointNotify.h"

#include "./Interface/cButton.h"
#include "./Interface/cTextArea.h"
#include "WindowIDEnum.h"

#include "ChatManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSkillPointNotify::CSkillPointNotify()
{
	m_type = WT_SKPOINTNOTIFYDIALOG;
}

CSkillPointNotify::~CSkillPointNotify()
{

}


void CSkillPointNotify::Linking()
{
	m_Notifyta1 = (cTextArea*)GetWindowForID(SK_INFOTEXT1);
	m_Notifyta2 = (cTextArea*)GetWindowForID(SK_INFOTEXT2);
	m_RedistBtn = (cButton*)GetWindowForID(SK_STARTBTN);
}


void CSkillPointNotify::InitTextArea()
{
	if( m_Notifyta1 )
		m_Notifyta1->SetScriptText( CHATMGR->GetChatMsg(735) );
	if( m_Notifyta2 )
		m_Notifyta2->SetScriptText( CHATMGR->GetChatMsg(736) );
}
