#include "stdafx.h"
#include ".\skilloptionchangedlg.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cIcon.h"
#include "./Interface/cStatic.h"
#include "ChatManager.h"
#include "MugongBase.h"
#include "Item.h"
#include "SkillInfo.h"
#include "SkillManager_Client.h"
#include "ChatManager.h"
#include "GameIn.h"
#include "NpcScriptDialog.h"
#include "ObjectManager.h"

CSkillOptionChangeDlg::CSkillOptionChangeDlg(void)
{
}

CSkillOptionChangeDlg::~CSkillOptionChangeDlg(void)
{
}

void CSkillOptionChangeDlg::Linking()
{
	m_pMugongIconDlg = (cIconDialog*)GetWindowForID(ST_ICON1);
	m_pItemIconDlg = (cIconDialog*)GetWindowForID(ST_ICON2);

	m_pOption[0] = (cStatic*)GetWindowForID(ST_Option1);
	m_pOption[1] = (cStatic*)GetWindowForID(ST_Option2);
	m_pOption[2] = (cStatic*)GetWindowForID(ST_Option3);
	m_pOption[3] = (cStatic*)GetWindowForID(ST_Option4);
	m_pOption[4] = (cStatic*)GetWindowForID(ST_Option5);
}

BOOL CSkillOptionChangeDlg::FakeMoveIcon(LONG mouseX, LONG mouseY, cIcon * icon)
{
	ASSERT(icon);

	
	CItem* pItem = NULL;

	if(icon->GetType() == WT_MUGONG || icon->GetType() == WT_JINBUB )
	{
		// 드래그한 아이콘이 무공일때
		CMugongBase* pMugong = (CMugongBase *)icon;

		CSkillInfo*	pSInfo = SKILLMGR->GetSkillInfo(pMugong->GetItemIdx());
		if(!pSInfo)	return FALSE;

		// 변환 불가능한 무공이면 실패
		if(pSInfo->GetChangeKind() == eSkillChange_None)
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1336 ) );
			return FALSE;
		}
		// 12성 이하면서 절초가 아니면 실패
		// 절초가 있는 무공일때
		/*
		if(SKILLMGR->GetSkillChangeInfo(pMugong->GetItemIdx()))
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1336 ) );
			return FALSE;
		}
		*/

		if(pMugong->GetSung() < 12)
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1336 ) );
			return FALSE;
		}

		// 아이템칸에 있는 아이템을 가져온다
		CItem* pOldItem = (CItem*)(m_pItemIconDlg->GetIconForIdx(0));
		
		if(pOldItem)	// 아이템이 있다면
		{
			// 아이템의 옵션을 가져온다.
			SKILLOPTION* pOption = SKILLMGR->GetSkillOptionByItemIndex(pOldItem->GetItemIdx());
			if(!pOption)	return FALSE;
            
			// 아이템 옵션이 모든 무공용이 아니면서 무공에 장착 가능한 옵션이 아니면 실패
			if(pOption->SkillKind != eSkillChange_All && pOption->SkillKind != pSInfo->GetChangeKind())
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1335 ) );
				return FALSE;
			}

			SKILLOPTION* pOldOption = SKILLMGR->GetSkillOption(pMugong->GetOption());

			if(pOldOption)
			{
				// 이미 무공에 적용된 옵션과 다른 종류 옵션의 아이템이면 실패
				if(pOldOption->OptionKind != pOption->OptionKind)
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1313 ) );
					return FALSE;
				}
				// 이미 적용된 옵션보다 높은 등급이 아니면 실패
				else if(pOldOption->OptionGrade >= pOption->OptionGrade)
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1314 ) );
					return FALSE;
				}

			}
			
		}
		
		// magi82
		// 먼저 올라가 있던 무공을 지운다
		cIcon* pOldMugong = NULL;
		m_pMugongIconDlg->DeleteIcon(0, &pOldMugong);
		if(pOldMugong)
			((CMugongBase*)pOldMugong)->SetLock(FALSE);

		// 무공을 등록한다
		m_pMugongIconDlg->AddIcon(0, icon, TRUE);
		((CMugongBase*)icon)->SetLock(TRUE);

		return FALSE;
	}
	else if(icon->GetType() == WT_ITEM)
	{
		// 드래그한 아이콘이 아이템일때
		pItem = (CItem*)icon;

		// 무공칸의 무공을 가져온다.
		CMugongBase* pOldMugong = (CMugongBase*)(m_pMugongIconDlg->GetIconForIdx(0));

		if(!pOldMugong)	// 무공이 없다면
			return FALSE;		
		
		// 변환 아이템이 아니면 실패
		if(pItem->GetItemKind() != eMUGONG_ITEM_OPTION)
			return FALSE;

		SKILLOPTION* pOption = SKILLMGR->GetSkillOptionByItemIndex(pItem->GetItemIdx());
		if(!pOption)	return FALSE;

		// 레벨 제한
		if(HERO->GetLevel() < pItem->GetItemInfo()->LimitLevel)
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1335 ) );
			return FALSE;
		}

		// 경지 제한
		switch(pOption->OptionGrade)
		{
		case 1:
			// 1단계는 일반
			break;
		case 2:
			// 2단계는 화경 극마 이상
			{
				if(!(HERO->GetStage() & eStage_Hwa || HERO->GetStage() & eStage_Geuk))
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 888 ) );
					return FALSE;
				}
			}
			break;
		case 3:
			// 3단계는 현경 탈마 이상
			{
				if(!(HERO->GetStage() == eStage_Hyun || HERO->GetStage() == eStage_Tal))
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1354 ) );
					return FALSE;
				}
			}
			break;
		}

		CSkillInfo*	pSInfo = SKILLMGR->GetSkillInfo(pOldMugong->GetItemIdx());
		if(!pSInfo)	return FALSE;

		// 아이템 옵션이 모든 무공용이 아니면서 무공에 장착 가능한 옵션이 아니면 실패
		if(pOption->SkillKind != eSkillChange_All && pOption->SkillKind != pSInfo->GetChangeKind())
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1335 ) );
			return FALSE;
		}

		SKILLOPTION* pOldOption = SKILLMGR->GetSkillOption(pOldMugong->GetOption());

		if(pOldOption)
		{
			// 이미 무공에 적용된 옵션과 다른 종류 옵션의 아이템이면 실패
			if(pOldOption->OptionKind != pOption->OptionKind)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1313 ) );
				return FALSE;
			}
			// 이미 적용된 옵션보다 높은 등급이 아니면 실패
			else if(pOldOption->OptionGrade >= pOption->OptionGrade)
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1314 ) );
				return FALSE;
			}
		}

		// 먼저 올라가 있던 아이템을 지운다
		cIcon* pOldItem = NULL;
		m_pItemIconDlg->DeleteIcon(0, &pOldItem);
		
		if(pOldItem)
			((CItem*)pOldItem)->SetLock(FALSE);

		// 아이템을 등록한다
		m_pItemIconDlg->AddIcon(0, icon, TRUE);
		((CItem*)icon)->SetLock(TRUE);
        SetOptionToolTip();

		return FALSE;
	}
	else
		return FALSE;

}

void CSkillOptionChangeDlg::OnActionEvnet(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		switch(lId)
		{
		case SkillTrans_OKBTN:
			{
				CMugongBase* pMugong = (CMugongBase*)(m_pMugongIconDlg->GetIconForIdx(0));
				CItem* pItem = (CItem*)(m_pItemIconDlg->GetIconForIdx(0));

				if(!pMugong)
				{
					return;
				}
				if(!pItem)
				{
					return;
				}

				SKILLOPTION* pOption = SKILLMGR->GetSkillOptionByItemIndex(pItem->GetItemIdx());
				if(!pOption) return;

				
				MSG_WORD4 msg;
				msg.Category = MP_MUGONG;
				msg.Protocol = MP_MUGONG_OPTION_SYN;
				msg.dwObjectID = HEROID;
				msg.wData1 = pMugong->GetItemIdx();
				msg.wData2 = pMugong->GetPosition();
				msg.wData3 = pItem->GetItemIdx();
				msg.wData4 = pItem->GetPosition();

				NETWORK->Send( &msg, sizeof(msg) );

				SetActive(FALSE);
			}
			break;
		case SkillTrans_CANCERBTN:
			SetActive(FALSE);
			break;
		}
	}
}

void CSkillOptionChangeDlg::SetActive(BOOL val)
{
	if( val == FALSE )
	{
		cIcon* temp;

		m_pMugongIconDlg->DeleteIcon(0, &temp);
		m_pItemIconDlg->DeleteIcon(0, &temp);
		if(temp)
			((CItem*)temp)->SetLock(FALSE);

		for(WORD i = 0; i < 5; i++)
		{
			m_pOption[i]->SetStaticText("");
			m_pOption[i]->SetActive(FALSE);
		}

		GAMEIN->GetNpcScriptDialog()->EndDialog();
		GAMEIN->GetNpcScriptDialog()->SetActive(FALSE);;
	}
    
	cDialog::SetActive(val);
}

void CSkillOptionChangeDlg::SetOptionToolTip()
{
	CItem* pItem = (CItem*)(m_pItemIconDlg->GetIconForIdx(0));
	if(!pItem)
	{
		return;
	}

	for(WORD i = 0; i < 5; i++)
	{
		m_pOption[i]->SetStaticText("");
		m_pOption[i]->SetActive(FALSE);
	}

	SKILLOPTION* pOption = SKILLMGR->GetSkillOptionByItemIndex(pItem->GetItemIdx());
	WORD CurOption = 0;

	if(pOption)
	{
		if(pOption->Range > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1316), pOption->Range);
			m_pOption[CurOption]->SetActive(TRUE);
			m_pOption[CurOption++]->SetStaticText(ToolTipText);
			if(CurOption >= 5) return;
		}
		if(pOption->Range < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1320), -pOption->Range);
			m_pOption[CurOption]->SetActive(TRUE);
			m_pOption[CurOption++]->SetStaticText(ToolTipText);
			if(CurOption >= 5) return;
		}

		if(pOption->ReduceNaeRyuk > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1317), (int)(pOption->ReduceNaeRyuk * 100 + 0.5));
			m_pOption[CurOption]->SetActive(TRUE);
			m_pOption[CurOption++]->SetStaticText(ToolTipText);
			if(CurOption >= 5) return;
		}
		if(pOption->ReduceNaeRyuk < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1321), (int)(-pOption->ReduceNaeRyuk * 100 - 0.5));
			m_pOption[CurOption]->SetActive(TRUE);
			m_pOption[CurOption++]->SetStaticText(ToolTipText);
			if(CurOption >= 5) return;
		}

		if(pOption->PhyAtk > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1318), (int)(pOption->PhyAtk * 100 + 0.5));
			m_pOption[CurOption]->SetActive(TRUE);
			m_pOption[CurOption++]->SetStaticText(ToolTipText);
			if(CurOption >= 5) return;
		}
		if(pOption->PhyAtk < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1322), (int)(-pOption->PhyAtk * 100 - 0.5));
			m_pOption[CurOption]->SetActive(TRUE);
			m_pOption[CurOption++]->SetStaticText(ToolTipText);
			if(CurOption >= 5) return;
		}
		
		if(pOption->BaseAtk)
		{
		char ToolTipText[32];
		wsprintf(ToolTipText, CHATMGR->GetChatMsg(1740), (int)(pOption->BaseAtk * 100));
		m_pOption[CurOption]->SetActive(TRUE);
		m_pOption[CurOption++]->SetStaticText(ToolTipText);
		if(CurOption >= 5) return;		
		//wsprintf(ToolTipText, "일반 공격력 %d%% %s", (int)(pOption->BaseAtk * 100), CheckMugongOption(pOption->BaseAtk));
		//pIcon->AddToolTipLine( ToolTipText, TTTC_TRANSMUGONGOPTION );		
		}
		
		if(pOption->AttAtk > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1319), (int)(pOption->AttAtk * 100 + 0.5));
			m_pOption[CurOption]->SetActive(TRUE);
			m_pOption[CurOption++]->SetStaticText(ToolTipText);
			if(CurOption >= 5) return;
		}
		if(pOption->AttAtk < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1323), (int)(-pOption->AttAtk * 100 - 0.5));
			m_pOption[CurOption]->SetActive(TRUE);
			m_pOption[CurOption++]->SetStaticText(ToolTipText);
			if(CurOption >= 5) return;
		}

		if(pOption->Life > 0)	
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1340), pOption->Life);
			m_pOption[CurOption]->SetActive(TRUE);
			m_pOption[CurOption++]->SetStaticText(ToolTipText);
			if(CurOption >= 5) return;
		}
		if(pOption->Life < 0)	
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1341), -pOption->Life);
			m_pOption[CurOption]->SetActive(TRUE);
			m_pOption[CurOption++]->SetStaticText(ToolTipText);
			if(CurOption >= 5) return;
		}

		if(pOption->NaeRyuk > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1342), pOption->NaeRyuk);
			m_pOption[CurOption]->SetActive(TRUE);
			m_pOption[CurOption++]->SetStaticText(ToolTipText);
			if(CurOption >= 5) return;
		}
		if(pOption->NaeRyuk < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1343), -pOption->NaeRyuk);
			m_pOption[CurOption]->SetActive(TRUE);
			m_pOption[CurOption++]->SetStaticText(ToolTipText);
			if(CurOption >= 5) return;
		}

		if(pOption->Shield > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1344), pOption->Shield);
			m_pOption[CurOption]->SetActive(TRUE);
			m_pOption[CurOption++]->SetStaticText(ToolTipText);
			if(CurOption >= 5) return;
		}
		if(pOption->Shield < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1345), -pOption->Shield);
			m_pOption[CurOption]->SetActive(TRUE);
			m_pOption[CurOption++]->SetStaticText(ToolTipText);
			if(CurOption >= 5) return;
		}

		if(pOption->PhyDef > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1348), (int)(pOption->PhyDef * 100 + 0.5));
			m_pOption[CurOption]->SetActive(TRUE);
			m_pOption[CurOption++]->SetStaticText(ToolTipText);
			if(CurOption >= 5) return;
		}
		if(pOption->PhyDef < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1349), (int)(-pOption->PhyDef * 100 - 0.5));
			m_pOption[CurOption]->SetActive(TRUE);
			m_pOption[CurOption++]->SetStaticText(ToolTipText);
			if(CurOption >= 5) return;
		}

		if(pOption->AttDef > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1324), (int)(pOption->AttDef * 100 + 0.5));
			m_pOption[CurOption]->SetActive(TRUE);
			m_pOption[CurOption++]->SetStaticText(ToolTipText);
			if(CurOption >= 5) return;
		}
		if(pOption->AttDef < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1325), (int)(pOption->AttDef * 100 - 0.5));
			m_pOption[CurOption]->SetActive(TRUE);
			m_pOption[CurOption++]->SetStaticText(ToolTipText);
			if(CurOption >= 5) return;
		}

		if(pOption->Duration > 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1346), pOption->Duration);
			m_pOption[CurOption]->SetActive(TRUE);
			m_pOption[CurOption++]->SetStaticText(ToolTipText);
			if(CurOption >= 5) return;
		}
		if(pOption->Duration < 0)
		{
			char ToolTipText[32];
			wsprintf(ToolTipText, CHATMGR->GetChatMsg(1347), pOption->Duration);
			m_pOption[CurOption]->SetActive(TRUE);
			m_pOption[CurOption++]->SetStaticText(ToolTipText);
			if(CurOption >= 5) return;
		}
	}
}
