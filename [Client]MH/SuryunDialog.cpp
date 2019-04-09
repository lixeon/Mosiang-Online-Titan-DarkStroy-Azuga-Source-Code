// SuryunDialog.cpp: implementation of the CSuryunDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SuryunDialog.h"
#include "./Interface/cIconGridDialog.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"
#include "./Interface/cStatic.h"
#include "./Interface/cButton.h"
#include "WindowIDEnum.h"
#include "GameIn.h"
#include "SkillPointRedist.h"
#include "MugongSuryunDialog.h"

#include "ObjectManager.h"
#include "AbilityManager.h"
#include "Ability.h"
#include "ChatManager.h"
#include "ItemManager.h"
#include "GameEventManager.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSuryunDialog::CSuryunDialog()
{
	m_pSuryunExpStc = NULL;
	m_type = WT_SURYUNDIALOG;
	m_IconIndexCreator.Init(MAX_ABILITY_NUM_PER_GROUP*eAGK_Max,IG_ABILITYICON_START);
	m_CurPosition = -1;
	m_SelectedAbilityIDX = 0;
}

CSuryunDialog::~CSuryunDialog()
{
	m_IconIndexCreator.Release();
}


void CSuryunDialog::SetActive( BOOL val )
{
	cDialog::SetActive( val );

	if( val )
	{
		if( HERO && HERO->IsInited() == TRUE )
		{
			SHOPITEMOPTION* pOption = HERO->GetShopItemStats();
			
			if( m_pSkRedistBtn )
			{
				if( pOption->SkillPoint > 0 || pOption->UseSkillPoint > 0 )
					m_pSkRedistBtn->SetActive( val );
				else
					m_pSkRedistBtn->SetActive( !val );
			}
		}
		else
		{
			GAMEIN->GetMugongSuryunDialog()->SetActive( !val );
			//cDialog::SetActive( !val );
		}
	}
}


void CSuryunDialog::Add(cWindow * window)
{
	if(window->GetType() == WT_PUSHUPBUTTON)
		AddTabBtn(curIdx1++, (cPushupButton * )window);
	else if(window->GetType() == WT_ICONGRIDDIALOG)
	{
		AddTabSheet(curIdx2++, window);
	}
	else 
		cDialog::Add(window);
}

void CSuryunDialog::Linking()
{
	m_pSuryunExpStc = (cStatic*)GetWindowForID(MGI_SURYUN_EXP_STATIC);
	m_pNeedSuryunExpStc = (cStatic*)GetWindowForID(MGI_NEEDSURYUN_EXP_STATIC);
	m_pSkRedistBtn = (cButton*)GetWindowForID(MGI_SKPOINTREDIST_BTN);
}

void CSuryunDialog::UpdateAbility(int State,BYTE Level,CAbilityInfo* pInfo)
{
	cIconGridDialog * dlg = (cIconGridDialog *)GetTabSheet(pInfo->GetKind());
	CAbilityIcon* pIcon = (CAbilityIcon*)dlg->GetIconForIdx(pInfo->GetPosition());
	if(pIcon == NULL)
	{
		// 추가
		pIcon = AddAbility(State, Level, pInfo);
	}
	pIcon->UpdateState(State,Level);
	if(GetSelectAbility() == pInfo->GetIdx())
	{
		if(Level == MAX_ABILITY_LEVEL)
			m_pNeedSuryunExpStc->SetStaticText(CHATMGR->GetChatMsg(179));
		else
		{
			DWORD NeedAbilityExp = pInfo->GetAbilityGradeExp(Level+1);
			if( (int)NeedAbilityExp == -1 )
				m_pNeedSuryunExpStc->SetStaticText(CHATMGR->GetChatMsg(179));
			else
				m_pNeedSuryunExpStc->SetStaticValue(NeedAbilityExp);
		}
		//툴팁갱신
		SetAbilityToolTip( pIcon );
	}
}

CAbilityIcon* CSuryunDialog::AddAbility(int State, BYTE Level, CAbilityInfo* pInfo)
{
	cIconGridDialog * dlg = (cIconGridDialog *)GetTabSheet(pInfo->GetKind());
		
	CAbilityIcon* pAbil = MakeNewAbility(pInfo, State, Level);

	dlg->AddIcon(pInfo->GetPosition(), (cIcon*)pAbil);
	return pAbil;
}

CAbilityIcon* CSuryunDialog::MakeNewAbility(CAbilityInfo* pInfo,int State,BYTE Level)
{
	CAbilityIcon* pAbilIcon;
	cImage tmpIconImage;
	pAbilIcon = new CAbilityIcon;
	//delete pAbilIcon;
	
	cImage* pImage = NULL;
	pImage = GetIconImage(pInfo->GetInfo()->Ability_IconIndex, &tmpIconImage);
	pAbilIcon->Init(0,0,DEFAULT_ICONSIZE,DEFAULT_ICONSIZE,pImage,
						m_IconIndexCreator.GenerateIndex());
	pAbilIcon->InitAbilityIcon(pInfo);
		
	pAbilIcon->SetIconType(eIconType_Ability);
	pAbilIcon->SetData(pInfo->GetIdx());
	
	WINDOWMGR->AddWindow(pAbilIcon);

	//////////////////////////////////////////////////////////////////////////
	// tooltip 띄우는 부분
	SetAbilityToolTip(pAbilIcon);
	//////////////////////////////////////////////////////////////////////////

	return pAbilIcon;
}

cImage * CSuryunDialog::GetIconImage(WORD Ability2DNum, cImage * pImageGet)
{
	SCRIPTMGR->GetImage( Ability2DNum, pImageGet, PFT_ABILITYPATH );	
	return pImageGet;
}

void CSuryunDialog::SetAbilityToolTip(CAbilityIcon* pIcon)
{
	cImage imgToolTip;
	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );

	CAbilityInfo* pInfo = pIcon->GetAbilityInfo();	
	pIcon->SetToolTip( pInfo->GetInfo()->Ability_name, 
		RGBA_MAKE(255, 255, 255, 255), &imgToolTip, TTCLR_ITEM_CANEQUIP );
//	pIcon->AddToolTipLine("");

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

	// 06. 01. 어빌리티창 인터페이스 수정 - 이영준
	// 레벨 제한이 있는 어빌리티일 경우 현재 레벨이 미달될경우 습득가능 레벨을 출력한다
	if(pInfo->GetInfo()->Ability_AcquireKind == 3 && HERO->GetLevel() < pInfo->GetInfo()->Ability_AcquireParam1)
	{
		wsprintf( tooltip, CHATMGR->GetChatMsg(588), pInfo->GetInfo()->Ability_AcquireParam1 );
		pIcon->AddToolTipLine( tooltip, TTTC_LIMIT );
	}

	////////////////////////////////////////////////////////////////////////////////////
	// 06. 06. 2차 전직 - 이영준
	// 전투특기
	if(pInfo->GetInfo()->Ability_AcquireKind == eAAK_CharacterStage )
	{
		if( 117 <= pInfo->GetIdx() && 122 >= pInfo->GetIdx() )
		{
			if( HERO->GetStage() != eStage_Hyun && AbilLevel >= 10 )
			{
				wsprintf( tooltip, CHATMGR->GetChatMsg(896) );
				pIcon->AddToolTipLine( tooltip, TTTC_LIMIT );
			}
		}
		else if( 123 <= pInfo->GetIdx() && 128 >= pInfo->GetIdx() )
		{
			if( HERO->GetStage() != eStage_Tal && AbilLevel >= 10 )
			{
				wsprintf( tooltip, CHATMGR->GetChatMsg(899) );
				pIcon->AddToolTipLine( tooltip, TTTC_LIMIT );
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////
}


void CSuryunDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if(we == WE_LBTNCLICK)
	{
		switch(lId)
		{
		case MGI_SURYUN_PUSHUP1: case MGI_SURYUN_PUSHUP2: 
		case MGI_SURYUN_PUSHUP3: case MGI_SURYUN_PUSHUP4: 
		case MGI_SURYUN_PUSHUP5:
			{
				m_CurPosition = -1;
			}
			break;
		}
		cIconGridDialog* pDlg = (cIconGridDialog*)GetTabSheet(GetCurTabNum());
		m_CurPosition = (POSTYPE)pDlg->GetCurSelCellPos();
		CAbilityIcon* pIcon = GetAbilityIcon(m_CurPosition);
		if(pIcon == NULL)
		{
			m_pNeedSuryunExpStc->SetStaticText("");
			SetSelectAbilityIDX(0);
			return;
		}
		
		CAbilityInfo* pAbilInfo = pIcon->GetAbilityInfo();
		BYTE AbilityLevel = 0;
		CAbility* pAbility = HERO->GetAbilityGroup()->GetAbility(pAbilInfo->GetIdx());
		if(pAbility)
			AbilityLevel = pAbility->GetLevel();
		if(AbilityLevel == MAX_ABILITY_LEVEL)
		{
			m_pNeedSuryunExpStc->SetStaticText(CHATMGR->GetChatMsg(179));
		}
		else
		{
			DWORD NeedAbilityExp = pAbilInfo->GetAbilityGradeExp(AbilityLevel+1);
			if(NeedAbilityExp == DWORD(-1))
			{
				m_pNeedSuryunExpStc->SetStaticText(CHATMGR->GetChatMsg(179));
				return;
			}
			
			if(pAbilInfo->GetInfo()->Ability_Gradeup_method1 != eAGM_OnOff)
			{
				m_pNeedSuryunExpStc->SetStaticValue(NeedAbilityExp);
			}
			else
			{
				if(AbilityLevel == 0)
					m_pNeedSuryunExpStc->SetStaticValue(NeedAbilityExp);
			}			
		}
		SetSelectAbilityIDX(pAbilInfo->GetIdx());
	}

	if(/*we == WE_RBTNCLICK || */we == WE_LBTNDBLCLICK)
	{
		//cIconGridDialog* pDlg = (cIconGridDialog*)p;		//PPP
		cIconGridDialog* pDlg = (cIconGridDialog*)((cDialog*)p)->GetWindowForID( lId );
		CAbilityIcon* pIcon = (CAbilityIcon*)pDlg->GetIconForIdx((WORD)pDlg->GetCurSelCellPos());
		if(pIcon == NULL)
			return;
		CAbility* pAbility = HERO->GetAbilityGroup()->GetAbility(pIcon->GetAbilityInfo()->GetIdx());
		if(pAbility == NULL)
			return;
		pAbility->Use();
		
		// event
		GAMEEVENTMGR->AddEvent( eGameEvent_Ability, pAbility->GetAbilityInfo()->GetInfo()->Ability_index );
	}
}

CAbilityIcon* CSuryunDialog::GetAbilityIcon(POSTYPE pos)
{
	CAbilityIcon* pIcon = NULL;
	cIconGridDialog* pDlg = (cIconGridDialog*)GetTabSheet(GetCurTabNum());
	pIcon = (CAbilityIcon*)pDlg->GetIconForIdx(pos);
	return pIcon;
}

CAbilityIcon* CSuryunDialog::GetAbilityIconAbs(POSTYPE AbsPos)
{
	CAbilityIcon* pIcon = NULL;

	BYTE cbTabNum	= (BYTE)(( AbsPos - TP_ABILITY_START ) / MAX_ABILITY_NUM_PER_GROUP);
	WORD pos		= ( AbsPos - TP_ABILITY_START ) % MAX_ABILITY_NUM_PER_GROUP;

	cIconGridDialog* pDlg = (cIconGridDialog*)GetTabSheet(cbTabNum);
	pIcon = (CAbilityIcon*)pDlg->GetIconForIdx(pos);
	return pIcon;
}


void CSuryunDialog::LearnAbilitySyn()
{	
	if(m_CurPosition == ((POSTYPE)-1))
		return;

	CAbilityIcon* pAbilityIcon = GetAbilityIcon(m_CurPosition);
	if(pAbilityIcon == NULL)
		return;

	if(pAbilityIcon->GetState() == eAIS_NotAvailable)
		return;
	CAbilityInfo* pInfo = pAbilityIcon->GetAbilityInfo();	
	
	if(ABILITYMGR->CanUpgrade(pInfo,HERO->GetAbilityGroup()) == FALSE)
		return;
	
	SetDisable(TRUE);
	GAMEIN->GetSkPointDlg()->SetDisable( TRUE );

	MSG_WORD msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_ABILITY_UPGRADE_SYN;
	msg.dwObjectID = HEROID;
	msg.wData = pInfo->GetIdx();

	NETWORK->Send(&msg, sizeof(msg));
}


void CSuryunDialog::SetAbilityExp(DWORD Exp)
{
	if(m_pSuryunExpStc)
		m_pSuryunExpStc->SetStaticValue(Exp);
}

void CSuryunDialog::SelectTab(BYTE idx)
{
	if( idx >= m_bTabNum ) return;
	
	if(m_bSelTabNum == idx)
		return;
	
	m_pNeedSuryunExpStc->SetStaticText("");
	SetSelectAbilityIDX(0);
	
	cTabDialog::SelectTab(idx);
}

// 06. 01. 어빌리티창 인터페이스 수정 - 이영준
// 툴팁만 갱신
void CSuryunDialog::UpdateAbility(CAbilityInfo* pInfo)
{
	cIconGridDialog * dlg = (cIconGridDialog *)GetTabSheet(pInfo->GetKind());
	CAbilityIcon* pIcon = (CAbilityIcon*)dlg->GetIconForIdx(pInfo->GetPosition());

	if(pIcon)
		SetAbilityToolTip( pIcon );
}

void CSuryunDialog::RedistBtnActive()
{
	if( m_pSkRedistBtn )
		m_pSkRedistBtn->SetActive( TRUE );
}
