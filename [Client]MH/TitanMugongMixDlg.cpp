// TitanMugongMixDlg.cpp: implementation of the CTitanMugongMixDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TitanMugongMixDlg.h"
#include "WindowIDEnum.h"
#include "Item.h"
#include "MugongBase.h"
#include "ChatManager.h"
#include "SkillManager_Client.h"
#include "ObjectManager.h"
#include "MugongManager.h"
#include "ObjectStateManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTitanMugongMixDlg::CTitanMugongMixDlg()
{

}

CTitanMugongMixDlg::~CTitanMugongMixDlg()
{

}

void CTitanMugongMixDlg::Linking()
{
	m_pTitanMugongIconDlg = (cIconDialog*)GetWindowForID(TITAN_MUGONGMIX_SLOT_MUGONG_DLG);
	m_pTitanItemIconDlg = (cIconDialog*)GetWindowForID(TITAN_MUGONGMIX_SLOT_MUNSEO_DLG);
}

void CTitanMugongMixDlg::SetActive( BOOL val )
{
	if( val == FALSE )
	{
		cIcon* temp;

		m_pTitanMugongIconDlg->DeleteIcon(0, &temp);
		m_pTitanItemIconDlg->DeleteIcon(0, &temp);
		if(temp)
			((CItem*)temp)->SetLock(FALSE);

		if( HERO->GetState() == eObjectState_Deal )
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);
	}
	cDialog::SetActive( val );
}

BOOL CTitanMugongMixDlg::FakeMoveIcon(LONG mouseX, LONG mouseY, cIcon * icon)
{
	ASSERT(icon);

	CItem* pItem = NULL;

	if( icon->GetType() == WT_MUGONG )
	{
		// 드래그한 아이콘이 무공일때
		CMugongBase* pMugong = (CMugongBase *)icon;

		CSkillInfo*	pSInfo = SKILLMGR->GetSkillInfo(pMugong->GetMugongBase()->wIconIdx);
		if(!pSInfo)	return FALSE;

		// 변환 가능한지 체크
		//if(!pSInfo->GetSkillIndex() + SKILLNUM_TO_TITAN)
		//	return FALSE;

		// 이미 변환된 무공인지 체크
		//if(MUGONGMGR->GetMugongByMugongIdx(pSInfo->GetSkillIndex() + SKILLNUM_TO_TITAN))
		//	return FALSE;

		// 12성 이하면 안됨
		if(pMugong->GetSung() < 12)
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1336 ) );
			return FALSE;
		}

		// 먼저 올라가 있던 무공을 지운다
		cIcon* pOldMugong = NULL;
		m_pTitanMugongIconDlg->DeleteIcon(0, &pOldMugong);

		// 무공을 등록한다
		m_pTitanMugongIconDlg->AddIcon(0, icon, TRUE);

		return FALSE;
	}
	else if(icon->GetType() == WT_ITEM)
	{
		// 드래그한 아이콘이 아이템일때
		pItem = (CItem*)icon;

		// 무공칸의 무공을 가져온다.
		//CMugongBase* pOldMugong = (CMugongBase*)(m_pTitanMugongIconDlg->GetIconForIdx(0));

		//if(!pOldMugong)	// 무공이 없다면
		//	return FALSE;		

		// 타이탄 무공변환 아이템이 아니면 실패
		//if(pItem->GetItemKind() != eMUGONG_ITEM_TITAN)
		//	return FALSE;

		// 먼저 올라가 있던 아이템을 지운다
		cIcon* pOldItem = NULL;
		m_pTitanItemIconDlg->DeleteIcon(0, &pOldItem);

		if(pOldItem)
			((CItem*)pOldItem)->SetLock(FALSE);

		// 아이템을 등록한다
		m_pTitanItemIconDlg->AddIcon(0, icon, TRUE);
		((CItem*)icon)->SetLock(TRUE);

		return FALSE;
	}
	else
		return FALSE;

}

void CTitanMugongMixDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		switch(lId)
		{
		case TITAN_MUGONGMIX_OKBTN:
			{
				CMugongBase* pMugong = (CMugongBase*)(m_pTitanMugongIconDlg->GetIconForIdx(0));
				CItem* pItem = (CItem*)(m_pTitanItemIconDlg->GetIconForIdx(0));

				if(!pMugong || !pItem)
				{
					return;
				}
				CSkillInfo*	pSInfo = SKILLMGR->GetSkillInfo(pMugong->GetMugongBase()->wIconIdx);
				if(!pSInfo)	return;

				// 변환 가능한지 체크
				//if(!pSInfo->GetSkillIndex() + SKILLNUM_TO_TITAN)
				//	return;
	
				// 이미 변환된 무공인지 체크
				//if(MUGONGMGR->GetMugongByMugongIdx(pSInfo->GetSkillIndex() + SKILLNUM_TO_TITAN))
				//	return;

				// 12성 이하면 안됨
				if(pMugong->GetSung() < 12)
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1336 ) );
					return;
				}

				// 타이탄 무공변환 아이템이 아니면 실패
				//if(pItem->GetItemKind() != eMUGONG_ITEM_TITAN)
				//	return;


//				MSG_WORD3 msg;
//				msg.Category = MP_ITEM;
//				msg.Protocol = MP_ITEM_TITAN_MUGONG_CHANGE_SYN;
//				msg.dwObjectID = HEROID;
//				msg.wData1 = pMugong->GetItemIdx();
//				msg.wData2 = pItem->GetItemIdx();
//				msg.wData3 = pItem->GetPosition();

//				NETWORK->Send( &msg, sizeof(msg) );

				SetActive(FALSE);
			}
			break;
		case TITAN_MUGONGMIX_CANCELBTN:
			SetActive(FALSE);
			break;
		}
	}
}

void CTitanMugongMixDlg::DeleteMugongIcon()
{
	cIcon* temp;

	m_pTitanMugongIconDlg->DeleteIcon(0, &temp);
}

void CTitanMugongMixDlg::DeleteItemIcon()
{
	cIcon* temp;

	m_pTitanItemIconDlg->DeleteIcon(0, &temp);
	if(temp)
		((CItem*)temp)->SetLock(FALSE);
}