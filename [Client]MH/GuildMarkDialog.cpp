// GuildMarkDialog.cpp: implementation of the CGuildMarkDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildMarkDialog.h"
#include "NpcScriptDialog.h"
#include "WindowIDEnum.h"
#include "ObjectStateManager.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "ChatManager.h"
#include ".\interface\cTextArea.h"
#include "./interface/cEditBox.h"
#include "./interface/cButton.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuildMarkDialog::CGuildMarkDialog()
{
	m_type = WT_GUILDMARKDLG;
}

CGuildMarkDialog::~CGuildMarkDialog()
{

}

void CGuildMarkDialog::Linking()
{
	m_pInfoText = (cTextArea*)GetWindowForID(GDM_INFOTEXT);
	m_pInfoText->SetScriptText(CHATMGR->GetChatMsg(303));
	
	m_pGuildMarkBtn = (cButton*)GetWindowForID(GDM_REGISTOKBTN);
	m_pGuildUnionMarkBtn = (cButton*)GetWindowForID(GUM_REGISTOKBTN);
}

void CGuildMarkDialog::SetActive(BOOL val)
{
	if(val == FALSE)
	{
		cEditBox* pMarkName = (cEditBox*)GetWindowForID(GDM_NAMEEDIT);
		pMarkName->SetFocusEdit(FALSE);
		if(HERO == 0)
			return;
		if( ( HERO->GetState() == eObjectState_Deal ) && (GAMEIN->GetNpcScriptDialog()->IsActive() == FALSE))
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	}
	cDialog::SetActive(val);
}

void CGuildMarkDialog::ShowGuildMark() 
{
	SetActive( TRUE );
	m_pGuildMarkBtn->SetActive( TRUE );
	m_pGuildUnionMarkBtn->SetActive( FALSE );
	m_pInfoText->SetScriptText(CHATMGR->GetChatMsg(303));
}

void CGuildMarkDialog::ShowGuildUnionMark()
{
	SetActive( TRUE );
	m_pGuildMarkBtn->SetActive( FALSE );
	m_pGuildUnionMarkBtn->SetActive( TRUE );
	m_pInfoText->SetScriptText(CHATMGR->GetChatMsg(1114));
}