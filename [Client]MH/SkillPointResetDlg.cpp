// SkillPointRedist.cpp: implementation of the CSkillPointRedist class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SkillPointResetDlg.h"
#include "GameIn.h"
#include "SuryunDialog.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "WindowIDEnum.h"
#include "./Interface/cIconGridDialog.h"
#include "./Interface/cButton.h"
#include "./Interface/cPushupButton.h"
#include "AbilityManager.h"
#include "./interface/cScriptManager.h"
#include "cWindowManager.h"
#include "ChatManager.h"
#include "ItemManager.h"
#include "cMsgBox.h"

extern DWORD AbilityIdx[eSR_Tab_Max];

CSkillPointResetDlg::CSkillPointResetDlg()
{
	m_IconIndexCreator.Init( 50, IG_ABILITYPOINT_START );
	m_dwCurTabNum = eSR_Tab_War;
}

CSkillPointResetDlg::~CSkillPointResetDlg()
{
	m_IconIndexCreator.Release();
}

void CSkillPointResetDlg::SetActive( BOOL val )
{
	cDialog::SetActive( val );

	if( !val )
	{
		m_IconGrid[m_dwCurTabNum]->SetCurSelCellPos(-1);
		SetDisable( FALSE );
		GAMEIN->GetSuryunDialog()->SetDisable( FALSE );
		OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
	}
	else
		RefreshAbilityIcons();
}


void CSkillPointResetDlg::Linking()
{
	m_ResetBtn = (cButton*)GetWindowForID( SK_POINT_RESETBTN );
	m_OkBtn = (cButton*)GetWindowForID( SK_POINT_OKBTN );

	for(int i=0; i<eSR_Tab_Max; ++i)
	{
		m_IconGrid[i] = (cIconGridDialog*)GetWindowForID( SK_POINT_ICONGRID1+i );
		m_GridButton[i] = (cPushupButton*)GetWindowForID( SK_POINT_RESET1BTN+i );

		if(m_IconGrid[i])
			m_IconGrid[i]->SetActive( FALSE );
		if(m_GridButton[i])
			m_GridButton[i]->SetPush( FALSE );
	}
	if( m_IconGrid[eSR_Tab_War] )
		m_IconGrid[eSR_Tab_War]->SetActive( TRUE );
	if( m_GridButton[eSR_Tab_War] )
		m_GridButton[eSR_Tab_War]->SetPush( TRUE );
}

BOOL CSkillPointResetDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch(we)
	{			
	case WE_CLOSEWINDOW:
		{	
			return TRUE;
		}
		break;
	}

	switch( lId )
	{
	case SK_POINT_RESET1BTN:
	case SK_POINT_RESET2BTN:
	case SK_POINT_RESET3BTN:
		{
			if( (lId-SK_POINT_RESET1BTN) == m_dwCurTabNum )
				return TRUE;

			SetTabNumber( lId-SK_POINT_RESET1BTN );
		}
		break;
	case SK_POINT_OKBTN:
		{
			SetActive(FALSE);
		}
		break;
	case SK_POINT_RESETBTN:
		{
			if(m_IconGrid[m_dwCurTabNum]->GetCurSelCellPos() >= 0)
			{
				WINDOWMGR->MsgBox( MBI_SKILL_POINT_RESET, MBT_YESNO, CHATMGR->GetChatMsg(1684), GetCurAbilityName());
			}
			else
			{
				WINDOWMGR->MsgBox(MBI_SKILL_POINT_RESET_FAIL, MBT_OK, CHATMGR->GetChatMsg(1685));
			}
		}
		break;
	}

	return TRUE;
}

void CSkillPointResetDlg::RefreshAbilityIcons()
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

CAbilityIcon* CSkillPointResetDlg::MakeNewAbilityIcon( CAbilityInfo* pInfo )
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


void CSkillPointResetDlg::SetAbilityToolTip(CAbilityIcon* pIcon)
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

	//WORD AbilLevel = pAbil ? pAbil->GetLevel() : 0;
	//pIcon->AddToolTipLine("");
	//DWORD NeedAbilityExp = pInfo->GetAbilityGradeExp(AbilLevel+1);
	//if( (int)NeedAbilityExp != -1 )
	//	wsprintf( tooltip, "%s: %d", CHATMGR->GetChatMsg(573), NeedAbilityExp );		
	//else
	//	wsprintf( tooltip, "%s", CHATMGR->GetChatMsg(480) );
	//pIcon->AddToolTipLine( tooltip, TTTC_ABILITYEXP );

	//NeedAbilityExp = pInfo->GetAbilityGradeExp(AbilLevel);
	//wsprintf( tooltip, "%s: %d", CHATMGR->GetChatMsg(738), NeedAbilityExp );
	//pIcon->AddToolTipLine( tooltip, TTTC_ABILITYEXP );

	WORD AbilLevel = pAbil ? pAbil->GetLevel() : 0;
	pIcon->AddToolTipLine("");

	DWORD NeedAbilityExp = 0;
	for( int i = 0; i < AbilLevel; i++ )
	{
		NeedAbilityExp = NeedAbilityExp + pInfo->GetAbilityGradeExp(i+1);
	}

	wsprintf( tooltip, "%s: %d", CHATMGR->GetChatMsg(738), NeedAbilityExp );
	pIcon->AddToolTipLine( tooltip, TTTC_ABILITYEXP );
}

void CSkillPointResetDlg::SetTabNumber( DWORD dwTab )
{
	for(DWORD i=0; i<eSR_Tab_Max; ++i)
	{
		dwTab==i ? m_IconGrid[i]->SetActive(TRUE) : m_IconGrid[i]->SetActive(FALSE);
		dwTab==i ? m_GridButton[i]->SetPush(TRUE) : m_GridButton[i]->SetPush(FALSE);
	}

	m_dwCurTabNum = dwTab;
	RefreshAbilityIcons();
}

void CSkillPointResetDlg::SetAbilityResetSyn()
{
	MSG_WORD2 msg;
	WORD AbLevel = 0;
	DWORD NeedAbExp = 0;
	char strError[128] = { 0, };

	CAbilityIcon* pIcon = (CAbilityIcon*)m_IconGrid[m_dwCurTabNum]->GetIconForIdx( (WORD)m_IconGrid[m_dwCurTabNum]->GetCurSelCellPos() );
	CAbilityInfo* pAbInfo = pIcon->GetAbilityInfo();

	// 리셋 가능한 스킬인지 체크
	if( FALSE == ABILITYMGR->CheckCanResetPoint(HERO, pAbInfo) )
	{
		sprintf(strError, CHATMGR->GetChatMsg(1687), GetCurAbilityName());
		goto syn_fail;
	}

	CAbility* pAbil = HERO->GetAbilityGroup()->GetAbility( pAbInfo->GetIdx() );
	if( !pAbil )
	{
		sprintf(strError, CHATMGR->GetChatMsg(1686), GetCurAbilityName());
		goto syn_fail;
	}

	if( !pIcon || !pAbInfo )
	{
		sprintf(strError, "magi82 - Error (!pIcon || !pAbInfo)");
		goto syn_fail;
	}
	if(pIcon->GetState() == eAIS_NotAvailable)
	{
		sprintf(strError, "magi82 - Error (pIcon->GetState() == eAIS_NotAvailable)");
		goto syn_fail;
	}

	AbLevel = pAbil ? pAbil->GetLevel() : 0;

	GAMEIN->GetSuryunDialog()->SetDisable(TRUE);

	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_ABILITY_RESET_SKPOINT_SYN;
	msg.dwObjectID = HEROID;
	msg.wData1 = pAbInfo->GetIdx();
	msg.wData2 = m_wShopItemPos;

	NETWORK->Send(&msg, sizeof(msg));
	return;

syn_fail:
	{
		WINDOWMGR->MsgBox(MBI_SKILL_POINT_RESET_FAIL, MBT_OK, strError);
		SetDisable( FALSE );
		return;
	}
}

char* CSkillPointResetDlg::GetCurAbilityName()
{
	if( m_IconGrid[m_dwCurTabNum]->GetCurSelCellPos() < 0 )		return NULL;
	CAbilityIcon* pIcon = (CAbilityIcon*)m_IconGrid[m_dwCurTabNum]->GetIconForIdx( (WORD)m_IconGrid[m_dwCurTabNum]->GetCurSelCellPos() );
	if( !pIcon )		return NULL;
	CAbilityInfo* pAbInfo = pIcon->GetAbilityInfo();
	if( !pAbInfo)		return NULL;

	return pAbInfo->GetInfo()->Ability_name;
}