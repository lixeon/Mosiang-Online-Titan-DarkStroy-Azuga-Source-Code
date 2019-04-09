// MPRegistDialog.cpp: implementation of the CMPRegistDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MPRegistDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cStatic.h"
#include "./Interface/cTextarea.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cIcon.h"
#include "cMsgBox.h"
#include "ChatManager.h"
#include "../[CC]Suryun/SuryunManager_client.h"
#include "MugongBase.h"
#include "ObjectManager.h"
#include "SkillInfo.h"
#include "ObjectStateManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMPRegistDialog::CMPRegistDialog()
{
	m_type = WT_MPREGISTDIALOG;
}

CMPRegistDialog::~CMPRegistDialog()
{
}

void CMPRegistDialog::Linking()
{
	m_MugongInfo = (cTextArea *)GetWindowForID(MP_RMUGONGINFO);
	m_PracticeInfo = (cTextArea *)GetWindowForID(MP_RPRACTICEINFO);
	m_Fee = (cStatic *)GetWindowForID(MP_RFEE);
	m_pMugongIconDlg = (cIconDialog*)GetWindowForID(MP_RMUGONGICON);
	((cIconDialog*)GetWindowForID(MP_RMUGONGICON))->SetDragOverIconType( WT_MUGONG );
	
}
BOOL CMPRegistDialog::FakeMoveIcon(LONG mouseX, LONG mouseY, cIcon * icon)
{
	ASSERT(icon);
	BYTE sung;
	if(icon->GetType() == WT_MUGONG || icon->GetType() == WT_JINBUB )
	{
		CMugongBase * pMugong =  (CMugongBase *)icon;
		sung = pMugong->GetSung();
		if(SURYUNMGR->NeedSuryun(pMugong->GetItemIdx(), sung, pMugong->GetExpPoint()) == FALSE)
			return FALSE;
	
		// magi82 - Titan(070911) 타이탄 무공업데이트
		if(pMugong->m_pSkillInfo->GetSkillKind() == SKILLKIND_TITAN)
		{
			WINDOWMGR->MsgBox( MBI_MPNOTICE_NOTFIT, MBT_OK, CHATMGR->GetChatMsg(1659) );
			return FALSE;
		}

		if((HERO->GetWeaponEquipType() != pMugong->m_pSkillInfo->GetWeaponType()) &&
			(pMugong->m_pSkillInfo->GetSkillKind() == SKILLKIND_OUTERMUGONG))
		{
			
			WINDOWMGR->MsgBox( MBI_MPNOTICE_NOTFIT, MBT_OK, CHATMGR->GetChatMsg( 151 ) );
			return FALSE;
		}
		SURYUNMISSIONINFO* pInfo = SURYUNMGR->GetMissionInfo(pMugong->GetItemIdx());
		if(!pInfo)
		{
			return FALSE;
		}
		SURYUNINFO* pSInfo = pInfo->GetSuryunInfo(sung);
		SetSuryunMugongInfo(pMugong->m_pSkillInfo->GetSkillName() , sung);
		SetPracticeInfo(pSInfo->AimSung, pSInfo->LimitTime, pSInfo->MonKind, pSInfo->MonNum, SURYUNMGR->GetSuryunFee( pSInfo ) );

		AddLink(icon);
	}	
	return FALSE;
}

void CMPRegistDialog::AddLink(cIcon* picon)
{
	if(!m_pMugongIconDlg->IsAddable(0))
		m_pMugongIconDlg->DeleteIcon(0, NULL);
	m_pMugongIconDlg->AddIcon(0, picon, TRUE);
}

CMugongBase* CMPRegistDialog::GetMugong()
{
	cIcon* pIcon = m_pMugongIconDlg->GetIconForIdx(0);
	if(!pIcon)
		return NULL;
	CMugongBase * pMugong = (CMugongBase *)pIcon;
	return pMugong;
}

void CMPRegistDialog::SetSuryunMugongInfo(char* MugongName, BYTE Sung)
{
	char buf[128];
	sprintf(buf, CHATMGR->GetChatMsg(661), MugongName, Sung);
	m_MugongInfo->SetScriptText(buf);
}

void CMPRegistDialog::SetPracticeInfo(BYTE Sung, DWORD limitime, int Kind, int num, MONEYTYPE Fee)
{
	char buf[128];
	DWORD LTime = limitime / 60000;
	sprintf(buf, CHATMGR->GetChatMsg(660), Sung, LTime, Kind, num);
	m_PracticeInfo->SetScriptText(buf);
	m_Fee->SetStaticValue(Fee);
}

void CMPRegistDialog::SetActive( BOOL val )
{
	if(val == FALSE)
	{
		m_MugongInfo->SetScriptText(CHATMGR->GetChatMsg(662));
		cIcon* pIcon;
		m_pMugongIconDlg->DeleteIcon(0, &pIcon);
		m_PracticeInfo->SetScriptText("");
		m_Fee->SetStaticValue(0);
		cMsgBox* pMsgBox = (cMsgBox*)WINDOWMGR->GetWindowForID( MBI_MPNOTICE_NOTFIT );
		if(pMsgBox)
			pMsgBox->SetActive(FALSE);
		OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	}

	cDialog::SetActive(val);
}


