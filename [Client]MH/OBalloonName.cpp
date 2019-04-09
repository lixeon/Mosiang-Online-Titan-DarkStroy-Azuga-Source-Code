// OBalloonName.cpp: implementation of the COBalloonName class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OBalloonName.h"
#include "./interface/cFont.h"
#include "./Interface/cWindowManager.h"
#include "./interface/cMultiLineText.h"
#include "./interface/cScriptManager.h"
#include "./interface/cFont.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COBalloonName::COBalloonName()
{
	m_wFontIdx	= 0;
	
	m_lTall		= 0;
	m_lPosX		= 0;

	m_lMasterTall	= 0;
	m_lMasterPosX	= 0;

	m_lNickTall = 0;
	m_lNickPosX = 0;
	
	m_fgColor	= RGB_HALF(255,255,255);

	m_bActive	= FALSE;
	m_bShow		= FALSE;

	ZeroMemory( m_szObjectName, sizeof(m_szObjectName) );
	ZeroMemory( m_szObjectNickName, sizeof(m_szObjectNickName) );
	ZeroMemory( m_szPetMasterName, sizeof(m_szPetMasterName) );
	
	m_pSiegeName = new cMultiLineText;
	m_bSiegeMap = FALSE;
}

COBalloonName::~COBalloonName()
{
	SAFE_DELETE( m_pSiegeName );
}

void COBalloonName::InitName(char * name)
{
	if( !name ) return;
	if( *name == 0 ) return;
//	ASSERTMSG(strlen(name) < 17, "object name is very long");
	SafeStrCpy( m_szObjectName, name, MAX_MONSTER_NAME_LENGTH +1 );	

	SetPosX(-CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szObjectName, strlen(m_szObjectName))/2);
}

void COBalloonName::SetName(char* name)
{
	if( !name ) return;
	if( *name == 0 ) return;
	SafeStrCpy( m_szObjectName, name, MAX_MONSTER_NAME_LENGTH +1 );

	SetPosX(-CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szObjectName, strlen(m_szObjectName))/2);
}

void COBalloonName::SetNickName(char* nickname)
{
	if( !nickname ) return;
//	if( *nickname == 0 ) return;
	SafeStrCpy( m_szObjectNickName, nickname, MAX_GUILD_NICKNAME+1 );

	SetNickPosX(-CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szObjectNickName, strlen(m_szObjectNickName))/2);
}

void COBalloonName::SetPetMasterName(char* petname)
{
	if( !petname )	return;
	SafeStrCpy( m_szPetMasterName, petname, MAX_MASTERNAME_LENGTH+1 );

	SetPetMasterNamePosX(-CFONT_OBJ->GetTextExtentEx(m_wFontIdx, m_szPetMasterName, strlen(m_szPetMasterName))/2);
}

BOOL COBalloonName::Render(LONG absX, LONG absY)
{
#ifdef _GMTOOL_
	if( WINDOWMGR->IsInterfaceHided() )	return FALSE;
#endif
	if(!m_bActive)	return FALSE;
	if(!m_bShow)	return FALSE;

	RECT rect = {(long)(absX + m_lPosX), (long)(absY + m_lTall), 1, 1};	

	if( m_bSiegeMap )
	{
		int nSize = strlen(m_szObjectName) + 2;
		m_pSiegeName->SetXY( rect.left,  rect.top );
		m_pSiegeName->Render();
		CFONT_OBJ->RenderFont(m_wFontIdx, m_szObjectName, nSize, &rect, RGBA_MERGE( RGB_HALF(70,70,70), 180 ));
		rect.left -= 1;
		rect.top -= 1;
		CFONT_OBJ->RenderFont(m_wFontIdx, m_szObjectName, nSize, &rect, RGBA_MERGE( m_fgColor, 255 ));
	}
	else if( *m_szObjectName != 0 )
	{
		int nSize = strlen(m_szObjectName) + 2;
		CFONT_OBJ->RenderFont(m_wFontIdx, m_szObjectName, nSize, &rect, RGBA_MERGE( RGB_HALF(70,70,70), 180 ));

		rect.left -= 1;
		rect.top -= 1;
		CFONT_OBJ->RenderFont(m_wFontIdx, m_szObjectName, nSize, &rect, RGBA_MERGE( m_fgColor, 255 ));
	}

	RECT nickrect = {(long)(absX + m_lNickPosX), (long)(absY + m_lNickTall - 18.0f), 1, 1};
	if( *m_szObjectNickName != 0 )
	{
		int nSize = strlen(m_szObjectNickName) + 2;
		CFONT_OBJ->RenderFont(m_wFontIdx, m_szObjectNickName, nSize, &nickrect, RGBA_MERGE( RGB_HALF(70,70,70), 180 ));
		nickrect.left -= 1;
		nickrect.top -= 1;
		CFONT_OBJ->RenderFont(m_wFontIdx, m_szObjectNickName, nSize, &nickrect, RGBA_MERGE( m_fgColor, 255 ));
	}

	if( *m_szObjectNickName && *m_szPetMasterName )	ASSERT(0);

	RECT masterrect = {(long)(absX + m_lMasterPosX), (long)(absY + m_lMasterTall - 15.0f), 1, 1};
	if( *m_szPetMasterName != 0 )
	{
		int nSize = strlen(m_szPetMasterName) + 2;
		CFONT_OBJ->RenderFont(m_wFontIdx, m_szPetMasterName, nSize, &masterrect, RGBA_MERGE( RGB_HALF(70,70,70), 180 ));
		masterrect.left -= 1;
		masterrect.top -= 1;
		CFONT_OBJ->RenderFont(m_wFontIdx, m_szPetMasterName, nSize, &masterrect, RGBA_MERGE( m_fgColor, 255 ));
	}
	
	return TRUE;
}

void COBalloonName::SetPosX(LONG	pos)
{ 
	m_lPosX = pos;	
}

void COBalloonName::SetNickPosX(LONG pos)
{
	m_lNickPosX = pos;
}

//
void COBalloonName::SetSiegeName( DWORD NameType, DWORD dwLength )
{
	cImage Image;
	DWORD Color = TTCLR_CASTLEGUILD;

	switch( NameType )
	{
	case eSiegeWarNameBox_CastleGuild:
		Color = TTCLR_CASTLEGUILD;
		break;
	case eSiegeWarNameBox_SiegeGuild:
		Color = TTCLR_SIEGEGUILD;
		break;
	case eSiegeWarNameBox_Enermy:
		Color = TTCLR_ENERMY;
		break;
	}

	char buf[MAX_NAME_LENGTH] = { 0, };
	SCRIPTMGR->GetImage(63, &Image, PFT_HARDPATH);	
	m_pSiegeName->Init( 0, RGB_HALF(255,255,255), &Image, Color );
	m_pSiegeName->SetImageAlpha( TOOLTIP_ALPHA );
	m_pSiegeName->AddNamePannel( dwLength );

	m_bSiegeMap = TRUE;
}

//2007. 10. 11. CBH - 오브젝트 폰트 바꺼주기위해 추가
void COBalloonName::SetFontIndex(BOOL bFontIndex)
{
	m_wFontIdx = bFontIndex;
}