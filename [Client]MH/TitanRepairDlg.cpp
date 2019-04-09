// TitanRepairDlg.cpp: implementation of the CTitanPartsChangeDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "TitanRepairDlg.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "WindowIDEnum.h"
#include "cWindowManager.h"
#include "TitanManager.h"
#include "ChatManager.h"
#include "cMsgBox.h"
#include "item.h"
#include "GameIn.h"
#include "TitanChangePreView.h"
#include "ItemManager.h"
#include "WearedExDialog.h"
#include "InventoryExDialog.h"
#include "MHAudioManager.h"
#include "MouseCursor.h"
#include "TitanInventoryDlg.h"


CTitanRepairDlg::CTitanRepairDlg()
{	
}

CTitanRepairDlg::~CTitanRepairDlg()
{

}

void CTitanRepairDlg::Linking()
{
	
}

void CTitanRepairDlg::SetActive( BOOL val )
{
	cDialog::SetActive( val );
	if( val == FALSE )
	{
		if( HERO->GetState() == eObjectState_Deal )
			OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

		if(CURSOR->GetCursor() == eCURSOR_TITANREPAIR)
		{
			CURSOR->SetCursor(eCURSOR_DEFAULT);
		}
	}	
}

BOOL CTitanRepairDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch(we)
	{			
	case WE_CLOSEWINDOW:
		{			
			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState(HERO, eObjectState_Deal);

			GAMEIN->GetInventoryDialog()->SetActive( FALSE );
			GAMEIN->GetTitanInventoryDlg()->SetActive( FALSE );
			//GAMEIN->GetTitanRepairDlg()->SetActive(FALSE);

			return TRUE;
		}
		break;
	}

	switch( lId )
	{
	case TITAN_REPAIR_PART:	// 각개수리
		{
			if(CURSOR->GetCursor() == eCURSOR_TITANREPAIR)
			{
				CURSOR->SetCursor(eCURSOR_DEFAULT);
			}
			else if(CURSOR->GetCursor() == eCURSOR_DEFAULT)
			{
				CURSOR->SetCursor(eCURSOR_TITANREPAIR);
			}
		}
		break;
	case TITAN_REPAIR_ALL:// 전체수리
		{
			MSG_TITAN_REPAIR_TOTAL_EQUIPITEM_SYN msg;
			msg.Init();
			DWORD dwMoney = TITANMGR->GetTitanEnduranceTotalInfo(&msg, TRUE);

			if(dwMoney == 0)
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1582));
			}
			else
			{
				WINDOWMGR->MsgBox( MBI_TITAN_TOTAL_REPAIR, MBT_YESNO, CHATMGR->GetChatMsg(1543), dwMoney );
			}
		}
		break;	
	}

	return TRUE;
}