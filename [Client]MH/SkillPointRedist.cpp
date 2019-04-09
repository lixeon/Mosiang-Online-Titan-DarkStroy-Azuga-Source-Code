// SkillPointRedist.cpp: implementation of the CSkillPointRedist class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SkillPointRedist.h"
#include "./Interface/cIconGridDialog.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"
#include "./Interface/cButton.h"
#include "./Interface/cStatic.h"
#include "WindowIDEnum.h"

#include "GameIn.h"
#include "AbilityIcon.h"
#include "SuryunDialog.h"
#include "ObjectManager.h"
#include "AbilityManager.h"
#include "Ability.h"

#include "ChatManager.h"
#include "ItemManager.h"

#include "ObjectStateManager.h"
#include "./Interface/cPushupButton.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DWORD AbilityIdx[eTab_Max] = 
{
	100,
	200,
	400
};


CSkillPointRedist::CSkillPointRedist()
{
	m_type = WT_SKPOINTDIALOG;
	m_IconIndexCreator.Init( 50, IG_ABILITYPOINT_START );
	m_dwCurTabNum = eTab_War;
}

CSkillPointRedist::~CSkillPointRedist()
{
	m_IconIndexCreator.Release();

}


void CSkillPointRedist::SetActive( BOOL val )
{
	cDialog::SetActive( val );

	if( !val )
	{
		SetDisable( FALSE );
		GAMEIN->GetSuryunDialog()->SetDisable( FALSE );
		OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
	}
	else
		RefreshAbilityIcons();
}


void CSkillPointRedist::Linking()
{
	m_UpBtn = (cButton*)GetWindowForID( SK_UPBTN );
	m_DownBtn = (cButton*)GetWindowForID( SK_DOWNBTN );
	m_OkBtn = (cButton*)GetWindowForID( SK_OKBTN );
	m_RePointst = (cStatic*)GetWindowForID( SK_POINTSTATIC );
	m_UsePointst = (cStatic*)GetWindowForID( SK_USESTATIC );
	m_OgPointst = (cStatic*)GetWindowForID( SK_ORIGINALSTATIC );

//	m_RePointst->SetDisable( TRUE );
//	m_UsePointst->SetDisable( TRUE );
//	m_OgPointst->SetDisable( TRUE );
	
	for(int i=0; i<eTab_Max; ++i)
	{
		m_IconGrid[i] = (cIconGridDialog*)GetWindowForID( SK_ICONGRID1+i );
		m_GridButton[i] = (cPushupButton*)GetWindowForID( SK_POINTAGAIN1BTN+i );

		if(m_IconGrid[i])
			m_IconGrid[i]->SetActive( FALSE );
		if(m_GridButton[i])
			m_GridButton[i]->SetPush( FALSE );
	}
	if( m_IconGrid[eTab_War] )
		m_IconGrid[eTab_War]->SetActive( TRUE );
	if( m_GridButton[eTab_War] )
		m_GridButton[eTab_War]->SetPush( TRUE );
}


void CSkillPointRedist::RefreshAbilityIcons()
{
	if( m_IconGrid == NULL )
		return;
	
	CAbilityGroup* pAbGroup = HERO->GetAbilityGroup();
	if( !pAbGroup )		return;
	BYTE stage = HERO->GetStage();
		
	for(int i=0; i<40; i++)
	{
		CAbilityInfo* pAbInfo = ABILITYMGR->GetAbilityInfo( (WORD)(i+AbilityIdx[m_dwCurTabNum]) );
		CAbility* pAbility = pAbGroup->GetAbility( (WORD)(i+AbilityIdx[m_dwCurTabNum]) );
		if( !pAbInfo )		continue;
		
		// 화경, 극마위해 추가
		if( pAbInfo->GetInfo()->Ability_AcquireKind == eAAK_CharacterStage )
		{
			if( stage == eStage_Normal ) continue;
			BYTE needstage = (BYTE)pAbInfo->GetInfo()->Ability_AcquireParam1;
			if( needstage )
			{
				if( !(stage & needstage) )
					continue;
			}
		}
		else
		{
			if( pAbInfo->GetInfo()->Ability_AcquireParam1 > HERO->GetMaxLevel() )		continue;
		}
		//

		CAbilityIcon* pIcon = (CAbilityIcon*)m_IconGrid[m_dwCurTabNum]->GetIconForIdx( pAbInfo->GetPosition() );
		if( !pIcon )
			pIcon = MakeNewAbilityIcon( pAbInfo );
		else
			SetAbilityToolTip(pIcon);
		
		if( pAbility )
			pIcon->UpdateState( eAIS_Usable, pAbility->GetLevel() );
		else
			pIcon->UpdateState( eAIS_OnlyVisible, 0 );
		
		m_IconGrid[m_dwCurTabNum]->AddIcon( pAbInfo->GetPosition(), (cIcon*)pIcon );
	}
}


CAbilityIcon* CSkillPointRedist::MakeNewAbilityIcon( CAbilityInfo* pInfo )
{
	CAbilityIcon* pAbilIcon;
	cImage tmpIconImage;
	pAbilIcon = new CAbilityIcon;
	
	cImage* pImage = NULL;

	SCRIPTMGR->GetImage( pInfo->GetInfo()->Ability_IconIndex, &tmpIconImage, PFT_ABILITYPATH );
	pAbilIcon->Init(0,0,DEFAULT_ICONSIZE,DEFAULT_ICONSIZE,&tmpIconImage,
						m_IconIndexCreator.GenerateIndex());
	pAbilIcon->InitAbilityIcon(pInfo);
		
	pAbilIcon->SetIconType(eIconType_Ability);
	pAbilIcon->SetData(pInfo->GetIdx());
	
	WINDOWMGR->AddWindow(pAbilIcon);

	SetAbilityToolTip(pAbilIcon);
	return pAbilIcon;
}


void CSkillPointRedist::SetAbilityToolTip(CAbilityIcon* pIcon)
{
	cImage imgToolTip;
	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );

	CAbilityInfo* pInfo = pIcon->GetAbilityInfo();	
	pIcon->SetToolTip("");
	pIcon->SetToolTip( pInfo->GetInfo()->Ability_name, 
		RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP );

	char tooltip[64];
	//종류(passive...
	if( pInfo->GetUseType() == eAUT_Passive )
	{
		if( pInfo->GetKind() != eAGK_Job )
			pIcon->AddToolTipLine( CHATMGR->GetChatMsg(571), TTTC_ABILITYUSEKIND );
		else
			pIcon->AddToolTipLine( CHATMGR->GetChatMsg(611), TTTC_ABILITYUSEKIND );
	}
	else
	{
		pIcon->AddToolTipLine( CHATMGR->GetChatMsg(572), TTTC_ABILITYUSEKIND );
	}
	
	// 툴팁에 Ability 설명추가
	TOOLTIP_TEXT* pTooltipText = ITEMMGR->GetItemToolTipInfo(pInfo->GetInfo()->tooltip_text);
	if( pTooltipText )
	{
		pIcon->AddToolTipLine("");
		while( pTooltipText )
		{
			pIcon->AddToolTipLine( pTooltipText->strToolTip, TTTC_DESCRIPTION );
			pTooltipText = pTooltipText->pNext;
		}
	}

	CAbility* pAbil = HERO->GetAbilityGroup()->GetAbility(pInfo->GetIdx());
	
	WORD AbilLevel = pAbil ? pAbil->GetLevel() : 0;
	pIcon->AddToolTipLine("");
	DWORD NeedAbilityExp = pInfo->GetAbilityGradeExp(AbilLevel+1);
	if( (int)NeedAbilityExp != -1 )
		wsprintf( tooltip, "%s: %d", CHATMGR->GetChatMsg(573), NeedAbilityExp );		
	else
		wsprintf( tooltip, "%s", CHATMGR->GetChatMsg(480) );
	pIcon->AddToolTipLine( tooltip, TTTC_ABILITYEXP );

	NeedAbilityExp = pInfo->GetAbilityGradeExp(AbilLevel);
	wsprintf( tooltip, "%s: %d", CHATMGR->GetChatMsg(738), NeedAbilityExp );
	pIcon->AddToolTipLine( tooltip, TTTC_ABILITYEXP );
}



void CSkillPointRedist::SetAbilitySyn( BOOL bDown )
{
	MSG_WORD msg;
	WORD AbLevel = 0;
	DWORD NeedAbExp = 0;
	
	CAbilityIcon* pIcon = (CAbilityIcon*)m_IconGrid[m_dwCurTabNum]->GetIconForIdx( (WORD)m_IconGrid[m_dwCurTabNum]->GetCurSelCellPos() );
	CAbilityInfo* pAbInfo = pIcon->GetAbilityInfo();
	CAbility* pAbil = HERO->GetAbilityGroup()->GetAbility( pAbInfo->GetIdx() );


	if( !pIcon || !pAbInfo )
		goto syn_fail;
	if(pIcon->GetState() == eAIS_NotAvailable)
		goto syn_fail;

	AbLevel = pAbil ? pAbil->GetLevel() : 0;
	NeedAbExp = pAbInfo->GetAbilityGradeExp( AbLevel+1 );

	if( bDown )
	{		
		if( HERO->GetGuildMemberRank()==GUILD_MASTER &&
			pAbInfo->GetInfo()->Ability_index == 411 )				// 지도력 Hard-cording
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(901) );
			goto syn_fail;
		}

		if(ABILITYMGR->CanDowngradeSkPoint(pAbInfo,HERO->GetAbilityGroup()) == FALSE)		
			goto syn_fail;
	}
	else
	{
		if(ABILITYMGR->CanUpgradeSkPoint(pAbInfo,HERO->GetAbilityGroup()) == FALSE)
			goto syn_fail;
	}	
	
	GAMEIN->GetSuryunDialog()->SetDisable(TRUE);

	msg.Category = MP_CHAR;
	if( bDown )
		msg.Protocol = MP_CHAR_ABILITY_DOWNGRADE_SKPOINT_SYN;
	else
		msg.Protocol = MP_CHAR_ABILITY_UPGRADE_SKPOINT_SYN;
	msg.dwObjectID = HEROID;
	msg.wData = pAbInfo->GetIdx();

	NETWORK->Send(&msg, sizeof(msg));
	return;

syn_fail:
	{
		SetDisable( FALSE );
		return;
	}
}


void CSkillPointRedist::SetAbilityExp( DWORD Exp )
{
	if( m_OgPointst )
		m_OgPointst->SetStaticValue( Exp );	
}

void CSkillPointRedist::RefreshAbilityPoint()
{
	if( m_RePointst )
		m_RePointst->SetStaticValue( HERO->GetShopItemStats()->SkillPoint );
	if( m_UsePointst )
		m_UsePointst->SetStaticValue( HERO->GetShopItemStats()->UseSkillPoint );
}


char* CSkillPointRedist::GetCurAbilityName()
{
	if( m_IconGrid[m_dwCurTabNum]->GetCurSelCellPos() < 0 )		return NULL;
	CAbilityIcon* pIcon = (CAbilityIcon*)m_IconGrid[m_dwCurTabNum]->GetIconForIdx( (WORD)m_IconGrid[m_dwCurTabNum]->GetCurSelCellPos() );
	if( !pIcon )		return NULL;
	CAbilityInfo* pAbInfo = pIcon->GetAbilityInfo();
	if( !pAbInfo)		return NULL;

	return pAbInfo->GetInfo()->Ability_name;
}


int CSkillPointRedist::GetCurAbilityLevel()
{
	if( m_IconGrid[m_dwCurTabNum]->GetCurSelCellPos() < 0 )		return NULL;
	CAbilityIcon* pIcon = (CAbilityIcon*)m_IconGrid[m_dwCurTabNum]->GetIconForIdx( (WORD)m_IconGrid[m_dwCurTabNum]->GetCurSelCellPos() );
	if( !pIcon )		return 0;
	CAbilityInfo* pAbInfo = pIcon->GetAbilityInfo();
	if( !pAbInfo)		return 0;

	CAbility* pAbil = HERO->GetAbilityGroup()->GetAbility(pAbInfo->GetIdx());	
	DWORD AbilLevel = pAbil ? pAbil->GetLevel() : 0;

	// 생활특기중 기본특기는 재분배 할 수 없다.
	if( pAbInfo->GetInfo()->Ability_index >= 401 && pAbInfo->GetInfo()->Ability_index <= 406 )
		return -1;

	return AbilLevel;
}

CAbilityInfo* CSkillPointRedist::GetCurAbilityInfo()
{
	if( m_IconGrid[m_dwCurTabNum]->GetCurSelCellPos() < 0 )		return NULL;
	CAbilityIcon* pIcon = (CAbilityIcon*)m_IconGrid[m_dwCurTabNum]->GetIconForIdx( (WORD)m_IconGrid[m_dwCurTabNum]->GetCurSelCellPos() );
	if( !pIcon )		return NULL;
	CAbilityInfo* pAbInfo = pIcon->GetAbilityInfo();
	if( !pAbInfo)		return NULL;

	return pAbInfo;
}

void CSkillPointRedist::SetTabNumber( DWORD dwTab )
{
	for(DWORD i=0; i<eTab_Max; ++i)
	{
		dwTab==i ? m_IconGrid[i]->SetActive(TRUE) : m_IconGrid[i]->SetActive(FALSE);
		dwTab==i ? m_GridButton[i]->SetPush(TRUE) : m_GridButton[i]->SetPush(FALSE);
	}

	m_dwCurTabNum = dwTab;
	RefreshAbilityIcons();
}
