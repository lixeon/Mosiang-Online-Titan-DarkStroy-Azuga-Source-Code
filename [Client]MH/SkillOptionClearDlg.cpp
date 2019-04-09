#include "stdafx.h"
#include ".\skilloptioncleardlg.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cIcon.h"
#include "cMsgBox.h"
#include "ChatManager.h"
#include "MugongBase.h"
#include "Item.h"
#include "SkillInfo.h"
#include "SkillManager_Client.h"
#include "ChatManager.h"
#include "ItemManager.h"
#include "ObjectManager.h"

CSkillOptionClearDlg::CSkillOptionClearDlg(void)
{
}

CSkillOptionClearDlg::~CSkillOptionClearDlg(void)
{
}

void CSkillOptionClearDlg::Linking()
{
	m_pMugongIconDlg = (cIconDialog*)GetWindowForID(T_DefaultICON);
}

BOOL CSkillOptionClearDlg::FakeMoveIcon(LONG mouseX, LONG mouseY, cIcon * icon)
{
	if(!(icon->GetType() == WT_MUGONG || icon->GetType() == WT_JINBUB ))
	{
		return FALSE;
	}

	CMugongBase* pMugong = (CMugongBase*)icon;

	if(pMugong->GetOption() == eSkillOption_None)
	{
		cMsgBox* pBox = WINDOWMGR->MsgBox(MBI_SKILLOPTIONCLEAR_NACK, MBT_OK, CHATMGR->GetChatMsg(1338));
		return FALSE;
	}

	cIcon* temp;
	m_pMugongIconDlg->DeleteIcon(0, &temp);
	m_pMugongIconDlg->AddIcon(0, icon, TRUE);

	return FALSE;
}

void CSkillOptionClearDlg::OnActionEvnet(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		CMugongBase* pMugong = (CMugongBase*)(m_pMugongIconDlg->GetIconForIdx(0));
		const ITEMBASE* pItem = ITEMMGR->GetItemInfoAbsIn(HERO, m_ItemPos);		

		switch(lId)
		{
		case T_DefaultOKBTN:
			{				
				if(!pMugong)
				{
					return;
				}
				if(!pItem)
				{
					return;
				}
				cMsgBox* pBox = WINDOWMGR->MsgBox(MBI_SKILLOPTIONCLEAR_ACK, MBT_YESNO, CHATMGR->GetChatMsg(1339));
			}
			break;
		case T_DefaultCANCERBTN:
			SetActive(FALSE);
			if( pItem )
			{
				CItem* pit = ITEMMGR->GetItem( pItem->dwDBIdx );
				if( pit )	pit->SetLock( FALSE );
			}
			break;
		}
	}
}

void CSkillOptionClearDlg::SetActive(BOOL val)
{
	if( val == FALSE )
	{
		cIcon* temp;
		m_pMugongIconDlg->DeleteIcon(0, &temp);
	}

	cDialog::SetActive(val);
}

void CSkillOptionClearDlg::SetItem(CItem* pItem)
{
	m_ItemPos = pItem->GetPosition();
}

void CSkillOptionClearDlg::OptionClearSyn()
{
	CMugongBase* pMugong = (CMugongBase*)(m_pMugongIconDlg->GetIconForIdx(0));
	const ITEMBASE* pItem = ITEMMGR->GetItemInfoAbsIn(HERO, m_ItemPos);

	if(!pMugong)	return;
	if(!pItem)	return;
	
	MSG_WORD4 msg;

	msg.Category = MP_MUGONG;
	msg.Protocol = MP_MUGONG_OPTION_CLEAR_SYN;
	msg.dwObjectID = HEROID;
	msg.wData1 = pMugong->GetItemIdx();
	msg.wData2 = pMugong->GetPosition();
	msg.wData3 = pItem->wIconIdx;
	msg.wData4 = pItem->Position;

	NETWORK->Send( &msg, sizeof(msg) );

	SetActive(FALSE);
}