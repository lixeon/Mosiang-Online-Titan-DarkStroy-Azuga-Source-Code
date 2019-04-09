// GuildCreateDialog.cpp: implementation of the CGuildCreateDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildCreateDialog.h"
#include "./Interface/cWindowManager.h"
#include "WindowIDEnum.h"
#include "./interface/cStatic.h"
#include "./interface/cEditBox.h"
#include "./interface/cTextArea.h"
#include "./interface/cButton.h"
#include "ObjectManager.h"
#include "GuildManager.h"
#include "ObjectStateManager.h"
#include "GameIn.h"
#include "NpcScriptDialog.h"
#include "ChatManager.h"
#include "cResourceManager.h"
#include "MHMap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuildCreateDialog::CGuildCreateDialog()
{
	m_type = WT_GUILDCREATEDLG;
}

CGuildCreateDialog::~CGuildCreateDialog()
{

}

void CGuildCreateDialog::Linking()
{
	m_pLocation = (cStatic*)GetWindowForID(GD_CLOCATION);
	m_pGuildName = (cEditBox*)GetWindowForID(GD_CNAME);
	m_pIntro = (cTextArea*)GetWindowForID(GD_CINTROTEXT);
	m_OkBtn = (cButton*)GetWindowForID(GD_CCREATEOKBTN);
	m_CaptionName = (cStatic*)GetWindowForID(CR_CAP);
}

void CGuildCreateDialog::SetActive(BOOL val)
{
	if(val == TRUE)
	{
		// magi82(37) 맵 속성 데이터
		// m_pLocation->SetStaticText(GetMapName(HERO->GetCharacterTotalInfo()->CurMapNum));
		m_pLocation->SetStaticText(MAP->GetMapName( HERO->GetCharacterTotalInfo()->CurMapNum ));
		m_pGuildName->SetEditText("");
		m_pIntro->SetScriptText("");

		if( HERO->GetGuildIdx() )
		{
			m_CaptionName->SetStaticText(RESRCMGR->GetMsg(270));
			m_OkBtn->SetText(RESRCMGR->GetMsg(335), RGB_HALF(255,255,255));
			SetMunpaName( GUILDMGR->GetGuildName() );
		}
		else
		{
			m_CaptionName->SetStaticText(RESRCMGR->GetMsg(510));
			m_OkBtn->SetText(RESRCMGR->GetMsg(513), RGB_HALF(255,255,255));
			m_pGuildName->SetReadOnly( FALSE );
		}
	}
	else
	{
		m_pGuildName->SetFocusEdit(FALSE);
		if(HERO == 0)
			return;
		if( ( HERO->GetState() == eObjectState_Deal ) && (GAMEIN->GetNpcScriptDialog()->IsActive() == FALSE))
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	}
	cDialog::SetActive(val);
}


void CGuildCreateDialog::SetMunpaName( char* strName )
{
	m_pGuildName->SetEditText( strName );
	
	m_pGuildName->SetReadOnly( TRUE );
}


void CGuildCreateDialog::SetMunpaIntro( char* strIntro )
{
	m_pIntro->SetScriptText( strIntro );
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGuildUnionCreateDialog::CGuildUnionCreateDialog()
{
	m_type = WT_GUILDUNIONCREATEDLG;
	m_pNameEdit = NULL;
}

CGuildUnionCreateDialog::~CGuildUnionCreateDialog()
{
}

void CGuildUnionCreateDialog::SetActive( BOOL val )
{
	if( val == TRUE )
	{
		m_pNameEdit->SetEditText("");
	}	
	else
	{
		if( HERO == 0 )	return;
		if( (HERO->GetState() == eObjectState_Deal) && (GAMEIN->GetNpcScriptDialog()->IsActive() == FALSE) )
			OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		
		if(m_pNameEdit)
			m_pNameEdit->SetFocusEdit( FALSE );
	}
	cDialog::SetActive(val);
}

void CGuildUnionCreateDialog::Linking()
{
	m_pNameEdit = (cEditBox*)GetWindowForID(GDU_NAME);
	m_pOkBtn = (cButton*)GetWindowForID(GDU_OKBTN);
	m_pText = (cTextArea*)GetWindowForID(GDU_TEXT);

	m_pText->SetScriptText( CHATMGR->GetChatMsg( 1125 ) );
}
