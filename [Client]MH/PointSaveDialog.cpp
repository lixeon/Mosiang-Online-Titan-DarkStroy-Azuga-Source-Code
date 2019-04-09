// PointSaveDialog.cpp: implementation of the PointSaveDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PointSaveDialog.h"
#include "WindowIDEnum.h"
#include "ObjectManager.h"
#include "MHMap.h"
#include "GameIn.h"
#include "MoveDialog.h"
#include "../Interface/cEditBox.h"
#include "../cIMEex.h"
#include "ItemManager.h"
#include "ChatManager.h"
#include "ObjectStateManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPointSaveDialog::CPointSaveDialog()
{
	m_bNewPoint = TRUE;
	m_ItemIdx = 0;
	m_ItemPos = 0;
}

CPointSaveDialog::~CPointSaveDialog()
{

}


void CPointSaveDialog::Linking()
{
	m_pNameEdtBox = (cEditBox*)GetWindowForID(CHA_NAMEEDITBOX);
	m_pNameEdtBox->SetValidCheck(VCM_CHARNAME);
}


void CPointSaveDialog::SetActive( BOOL val )
{
	cDialog::SetActive( val );	
	m_pNameEdtBox->SetFocusEdit( val );
	
	if( val )
		m_pNameEdtBox->SetEditText("");
}


void CPointSaveDialog::ChangePointName()
{
	if( strlen( m_pNameEdtBox->GetEditText() ) == 0 )
	{
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);

		//当玩家没有输入任何字符并点击确认时,不能将玩家的状态改变
		/*if( HERO->GetState() == eObjectState_Deal )
			OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );*/

		m_pNameEdtBox->SetEditText("");
		SetActive(FALSE);
		return;
	}

	if( GAMEIN->GetMoveDialog()->CheckSameName( m_pNameEdtBox->GetEditText() ) )
	{
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
		ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);

		if( HERO->GetState() == eObjectState_Deal )
			OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );

		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(784) );		
		return;
	}
	
	if( m_bNewPoint )
	{	
		SEND_MOVEDATA_WITHITEM msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_SAVEPOINT_ADD_SYN;
		msg.dwObjectID = HERO->GetID();
		msg.Data.DBIdx = GAMEIN->GetMoveDialog()->GetSelectedDBIdx();
		msg.Data.MapNum = MAP->GetMapNum();
		strncpy(msg.Data.Name, m_pNameEdtBox->GetEditText(), MAX_SAVEDMOVE_NAME-1);
		VECTOR3 pos = HERO->GetCurPosition();
		msg.Item.ShopItemIdx = (WORD)m_ItemIdx;
		msg.Item.ShopItemPos = (POSTYPE)m_ItemPos;
		msg.Data.Point.SetMovePoint((WORD)pos.x, (WORD)pos.z);
		NETWORK->Send(&msg, sizeof(msg));
	}
	else
	{
		SEND_MOVEDATA_SIMPLE msg;
		msg.Category = MP_ITEM;
		msg.Protocol = MP_ITEM_SHOPITEM_SAVEPOINT_UPDATE_SYN;
		msg.dwObjectID = HERO->GetID();
		msg.Data.DBIdx = GAMEIN->GetMoveDialog()->GetSelectedDBIdx();
		strncpy(msg.Data.Name, m_pNameEdtBox->GetEditText(), MAX_SAVEDMOVE_NAME-1);
		NETWORK->Send(&msg, sizeof(msg));
	}

	m_pNameEdtBox->SetEditText("");
	SetActive(FALSE);
}


void CPointSaveDialog::CancelPointName()
{
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_MunpaWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);

	//当使用回城或者遁甲,取消操作后,回到moveDlg窗口,并且限制玩家移动
	/*if( HERO->GetState() == eObjectState_Deal )
		OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );*/

	SetActive(FALSE);
}
