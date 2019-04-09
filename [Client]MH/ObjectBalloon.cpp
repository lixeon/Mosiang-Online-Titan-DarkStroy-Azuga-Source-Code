// ObjectBalloon.cpp: implementation of the CObjectBalloon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ObjectBalloon.h"
#include "OBalloonName.h"
#include "OBalloonChat.h"
#include "StreetStallTitleTip.h"
#include "MunpaMark.h"
#include "Object.h"
#include "GameResourceManager.h"
#include "./Interface/cWindowManager.h"
#include "MunpaMarkManager.h"
#include "GuildMarkManager.h"
#include "GuildMark.h"
#include "Player.h"
#include "ChatManager.h"
#include "GuildUnionMarkMgr.h"
#include "./Interface/cScriptManager.h"
#include "Npc.h"
#include "./Interface/cFont.h"

extern int g_nServerSetNum;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
WORD NPCIMAGE[] = {
	24,16,14,15,17,19,24,24,20,24,
	18,33,23,21,23,22,24,24,24,24,
	24,24,24,24,24,24,24,24,24,24,
	24,24,24,24,24,24,24,24,24,24,
	24,24,24
};
*/

CObjectBalloon::CObjectBalloon()
{
	m_pName			= NULL;
	m_pChat			= NULL;
	m_pSSTitle		= NULL;
	m_pSBSTitle		= NULL;
	m_pObject		= NULL;
	m_pGuildMark	= NULL;
//	m_pNpcMark		= NULL;
}

CObjectBalloon::~CObjectBalloon()
{
	Release();
}

void CObjectBalloon::InitBalloon(CObject * pParent, LONG tall, BYTE flag)
{
	cWindow::Init(0,0,0,0,NULL,pParent->GetID());

	m_pObject = pParent;

	if( pParent->GetObjectKind() == eObjectKind_Player )
	{
		tall = (LONG)(tall * ((CPlayer*)pParent)->GetCharacterTotalInfo()->Height);
	}
/*
	else if( pParent->GetObjectKind() == eObjectKind_Npc )
	{
		m_pNpcMark = new cImage;
		SCRIPTMGR->GetImage(NPCIMAGE[((CNpc*)pParent)->GetNpcKind()],m_pNpcMark,PFT_MINIMAPPATH);
	}
*/	
	m_lTall = tall;

	if(flag & ObjectBalloon_Name)
	{
		ASSERT(!m_pName);
		m_pName = new COBalloonName;
		//2007. 10. 11. CBH - 오브젝트에 따라 폰트 처리를 위해 추가
		if(m_pObject->GetObjectKind() == eObjectKind_Player)
			m_pName->SetFontIndex(cFont::FONT_KIND::FONT5);
		else
			m_pName->SetFontIndex(cFont::FONT_KIND::FONT0);		
	}

	if(flag & ObjectBalloon_Chat)
	{
		ASSERT(!m_pChat);
		m_pChat = new COBalloonChat;
	}
	
	if(flag & ObjectBalloon_Title)
	{
		ASSERT(!m_pSSTitle);
		ASSERT(!m_pSBSTitle);
		m_pSSTitle = new CStreetStallTitleTip;
		m_pSBSTitle = new CStreetStallTitleTip;
	}

	m_bActive	= TRUE;

	WINDOWMGR->AddChatTooltip( this );
}

void CObjectBalloon::Release()
{
	WINDOWMGR->RemoveChatTooltipWindow( this );

	SAFE_DELETE(m_pName);
	SAFE_DELETE(m_pChat);
	SAFE_DELETE(m_pSSTitle);
	SAFE_DELETE(m_pSBSTitle);
	
	m_pObject		= NULL;
	m_pGuildMark	= NULL;
}

void CObjectBalloon::Render()
{
	if( !m_pObject )									return;
	if( !m_pObject->IsInited() )						return;
	if( !m_pObject->GetEngineObject() )					return;
	if( !m_pObject->GetEngineObject()->IsVisible() )	return;
	if( !m_bActive )									return;
	
#ifndef _GMTOOL_
	if( m_pObject->GetObjectKind() == eObjectKind_Player )
	{
		// 투명인간은 채팅내용 안보임
		//GM은 봄
		if(((CPlayer*)m_pObject)->GetCharacterTotalInfo()->bVisible == FALSE)
			return;
	}
#endif

	BOOL bRender = FALSE;
	LONG lTall = m_lTall + 20;	//머리위부터 20
//	LONG lX	= 0;
	
	if( m_pName )
	if( m_pName->IsShow() ) bRender = TRUE;
	if( m_pChat )
	if( m_pChat->IsShow() ) bRender = TRUE;
	if( m_pSSTitle )
	if( m_pSSTitle->IsShow() )
	{
		bRender = TRUE;
		
		lTall	= m_lTall - 50;
//		lX		= 100;
	}
	if( m_pSBSTitle )
	if( m_pSBSTitle->IsShow() )
	{
		bRender = TRUE;
		
		lTall	= m_lTall - 50;
		//		lX		= 100;
	}
		
	if( m_pObject->GetState() == eObjectState_Ungijosik )
		lTall = m_lTall - 50;
	else if( m_pObject->GetState() == eObjectState_Die )
		lTall = m_lTall - 100;

	if( !bRender ) return;
	
	VECTOR3 OutPos3, inPos3;
	m_pObject->GetPosition(&inPos3);
	BYTE state = m_pObject->GetState();
	
//	inPos3.x += lX;
	inPos3.y += lTall;

	GetScreenXYFromXYZ(g_pExecutive->GetGeometry(), 0, &inPos3, &OutPos3);
	if(OutPos3.x < 0 || OutPos3.x > 1 || OutPos3.y < 0 || OutPos3.y > 1) return;
	m_absPos.x = (GAMERESRCMNGR->m_GameDesc.dispInfo.dwWidth*OutPos3.x) + 0.25f;
	m_absPos.y = (GAMERESRCMNGR->m_GameDesc.dispInfo.dwHeight*OutPos3.y);


	if( m_pSSTitle )
	{
		if( m_pSSTitle->Render((long)m_absPos.x, (long)(m_absPos.y) ) )
		{			
			m_absPos.y -= 25.0f + 15.0f*m_pSSTitle->GetLineCount();
		}
	}

	if( m_pSBSTitle )
	{
		if( m_pSBSTitle->Render((long)m_absPos.x, (long)(m_absPos.y) ) )
		{			
			m_absPos.y -= 25.0f + 15.0f*m_pSBSTitle->GetLineCount();
		}
	}
	
	if( m_pChat )
	{
		if( m_pChat->Render((long)m_absPos.x,(long)(m_absPos.y) ) )
			m_absPos.y -= m_pChat->GetBalloonHeight();
	}
	
	if( m_pName )
	{		
		if( m_pName->Render((long)m_absPos.x,(long)m_absPos.y))
		{
			if( m_pObject->GetObjectKind() == eObjectKind_Player )
			{
				LONG nposx = m_pName->GetNickPosX() >= m_pName->GetPosX() ? m_pName->GetPosX() : m_pName->GetNickPosX(); 
				if( ((CPlayer*)m_pObject)->GetGuildMarkName() != 0 )
				{
					if( m_pGuildMark )
					{
						VECTOR2 pos2;
						pos2.x = m_absPos.x + nposx - 18.0f;//- 40.0f;
//						pos2.x = m_absPos.x + m_pName->GetPosX() - 18.0f;//- 40.0f;						
						pos2.y = m_absPos.y + m_pName->GetTall() - 2.0f;//- 5.0f;
						m_pGuildMark->Render(&pos2);				
					}
				}
				DWORD dwGuildUnionIdx = ((CPlayer*)m_pObject)->GetGuildUnionIdx();
				DWORD dwGuildUnionMarkIdx = ((CPlayer*)m_pObject)->GetGuildUnionMarkIdx();
				if( dwGuildUnionIdx != 0 && dwGuildUnionMarkIdx != 0 )
				{
					CGuildUnionMark* pMark = GUILDUNIONMARKMGR->GetGuildUnionMark( g_nServerSetNum, dwGuildUnionIdx, dwGuildUnionMarkIdx );
					if( pMark )
					{
						VECTOR2 pos2;
						pos2.x = m_absPos.x + nposx - 18.0f;
//						pos2.x = m_absPos.x + m_pName->GetPosX() - 18.0f;
						pos2.y = m_absPos.y + m_pName->GetTall() - 18.0f;		
						pMark->Render( &pos2 );
					}
				}
			}
/*			
			else if( m_pObject->GetObjectKind() == eObjectKind_Npc )
			{
				LONG nposx = m_pName->GetNickPosX() >= m_pName->GetPosX() ? m_pName->GetPosX() : m_pName->GetNickPosX(); 
				if(m_pNpcMark)
				{
					VECTOR2 pos2;
					pos2.x = m_absPos.x + nposx - 30.0f;
					pos2.y = m_absPos.y + m_pName->GetTall() - 10.0f;

					VECTOR2 scale;
					scale.x = 1.0f;
					scale.y = 1.0f;

					m_pNpcMark->RenderSprite( &scale, NULL, 0, &pos2, 0xffffffff );
				}
			}
*/			
		}
	}
}

//---

void CObjectBalloon::SetOverInfoOption( DWORD dwOption )
{
	if( dwOption )	//차후 옵션에 맞게 수정
	{
		if( m_pName )		m_pName->SetActive( TRUE );
		if( m_pChat )		m_pChat->SetActive( TRUE );
		if( m_pSSTitle )	m_pSSTitle->SetActive( TRUE );
		if( m_pSBSTitle )	m_pSBSTitle->SetActive( TRUE );
	}
	else
	{
		if( m_pName )		m_pName->SetActive( FALSE );
		if( m_pChat )		m_pChat->SetActive( FALSE );
		if( m_pSSTitle )	m_pSSTitle->SetActive( FALSE );
		if( m_pSBSTitle )	m_pSBSTitle->SetActive( FALSE );
	}
}

void CObjectBalloon::ShowObjectName( BOOL bShow, DWORD dwColor )
{
	if( !m_pName ) return;
	m_pName->SetFGColor( dwColor );
	m_pName->Show( bShow );
}

void CObjectBalloon::ShowChatBalloon( BOOL bShow, char* chatMsg, DWORD dwColor, DWORD dwAliveTime )
{
	if( !m_pChat ) return;
	m_pChat->SetFGColor( dwColor );
	m_pChat->Show( bShow );

	if( bShow )
	{
		m_pChat->SetChat( chatMsg );
		m_pChat->SetDisappearTime( dwAliveTime );
	}
}

void CObjectBalloon::ShowStreeStallTitle( BOOL bShow, char* strTitle, DWORD dwColor )
{
	if( !m_pSSTitle ) return;
	m_pSSTitle->SetFGColor( dwColor );
	m_pSSTitle->Show( bShow );

	if( bShow )
	{
		if( strTitle )
			m_pSSTitle->SetTitle( strTitle );
		else
			m_pSSTitle->SetTitle( CHATMGR->GetChatMsg(366) );
	}
}

void CObjectBalloon::ShowStreeBuyStallTitle( BOOL bShow, char* strTitle, DWORD dwColor )
{
	if( !m_pSBSTitle ) return;
	m_pSBSTitle->SetFGColor( dwColor );
	m_pSBSTitle->Show( bShow );
	
	if( bShow )
	{
		if( strTitle )
			m_pSBSTitle->SetTitle( strTitle );
		else
			m_pSBSTitle->SetTitle( CHATMGR->GetChatMsg(366) );
	}
}

BOOL CObjectBalloon::SetGuildMark( DWORD GuildIdx )
{
	m_pGuildMark = GUILDMARKMGR->GetGuildMark( GuildIdx );
	if( m_pGuildMark )	return TRUE;
	else				return FALSE;
}

void CObjectBalloon::SetNickName(char* NickName)
{
	m_pName->SetNickName(NickName);	
}

void CObjectBalloon::SetSiegeName( DWORD NameType, DWORD dwLength )
{
	if( m_pName )
		m_pName->SetSiegeName( NameType, dwLength );
}

void CObjectBalloon::SetMasterName( char* MasterName )
{
	if( m_pName )
		m_pName->SetPetMasterName(MasterName);
}

