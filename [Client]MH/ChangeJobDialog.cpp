// ChangeJobDialog.cpp: implementation of the CChangeJobDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChangeJobDialog.h"
#include "WindowIDEnum.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "ItemManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChangeJobDialog::CChangeJobDialog()
{
	m_type = WT_ITEM_CHANGEJOB_DLG;

}

CChangeJobDialog::~CChangeJobDialog()
{

}


void CChangeJobDialog::ChangeJobSyn()
{
	MSG_DWORD2 msg;
	SetProtocol( &msg, MP_ITEM, MP_ITEM_SHOPITEM_JOBCHANGE_SYN );
	msg.dwObjectID = HEROID;
	msg.dwData1 = m_ItemPos;
	msg.dwData2 = m_ItemDBIdx;
	NETWORK->Send( &msg, sizeof(msg) );

	SetActive( FALSE );
}


void CChangeJobDialog::CancelChangeJob()
{
	if( HERO->GetState() == eObjectState_Deal )
		OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );

	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_GuildWarehouse);
	ITEMMGR->SetDisableDialog(FALSE, eItemTable_Shop);
	
	SetActive( FALSE );
}
