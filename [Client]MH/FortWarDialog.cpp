#include "stdafx.h"
#include "FortWarDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./interface/cStatic.h"
#include "./interface/cButton.h"
#include "./ObjectGuagen.h"
#include "./Interface/cIconGridDialog.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cIcon.h"
#include "cMsgBox.h"
#include "cDivideBox.h"
#include "Item.h"
#include "ChatManager.h"
#include "ItemManager.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"


CFWEngraveDialog::CFWEngraveDialog()
{
	m_dwProcessTime = 0;
	m_fBasicTime = 1.0f;
}

CFWEngraveDialog::~CFWEngraveDialog()
{
}

void CFWEngraveDialog::Linking()
{
	m_pEngraveGuagen = (CObjectGuagen*)GetWindowForID( FW_ENGRAVEGUAGE );
	m_pRemaintimeStatic = (cStatic*)GetWindowForID( FW_ENGRAVETIME );
}

DWORD CFWEngraveDialog::ActionEvent( CMouse * mouseInfo )
{
	DWORD we = WE_NULL;

	if( m_bDisable ) return we;

	char buf[128];
	int nLimitTime = ((int)(m_dwProcessTime - gCurTime)) / 1000;
	if( nLimitTime < 0 )
		nLimitTime = 0;

	static int last = 0;
	if( last != nLimitTime )
	{
		sprintf( buf, CHATMGR->GetChatMsg(1043), nLimitTime );
		m_pRemaintimeStatic->SetStaticText( buf );
		last = nLimitTime;

		m_pEngraveGuagen->SetValue( nLimitTime/m_fBasicTime, (DWORD)m_fBasicTime );
	}

	we = cDialog::ActionEvent( mouseInfo );

	return we;
}

void CFWEngraveDialog::OnActionEvent( LONG lId, void * p, DWORD we )
{
	if( we & WE_BTNCLICK )
	{
		if( lId == FW_ENGRAVECANCEL )
		{
			MSGBASE msg;
			msg.Category = MP_FORTWAR;
			msg.Protocol = MP_FORTWAR_ENGRAVE_CANCEL_SYN;
			msg.dwObjectID = HEROID;
			NETWORK->Send( &msg, sizeof(msg) );
		}
	}
}

void CFWEngraveDialog::SetActiveWithTime( BOOL val, DWORD dwTime )
{
	if( val == TRUE )
	{
		m_dwProcessTime = gCurTime + dwTime*1000;	//초
		m_fBasicTime = (float)dwTime;
	}
	else
	{
		m_dwProcessTime = 0;
		m_fBasicTime = 1.0f;
	}

	cDialog::SetActive( val );
}


/////////////////////////////////////////////////
CFWTimeDialog::CFWTimeDialog()
{
	m_dwWarTime = 0;
}

CFWTimeDialog::~CFWTimeDialog()
{
}

void CFWTimeDialog::Linking()
{
	m_pTimeStatic = (cStatic*)GetWindowForID( FW_TIME );	
	m_pCharacterName = (cStatic*)GetWindowForID( FW_TIME_CHARACTER );
}

DWORD CFWTimeDialog::ActionEvent( CMouse * mouseInfo )
{
	DWORD we = WE_NULL;

	if( m_bDisable ) return we;


	char buf[128];
	int nLimitTime = ((int)(m_dwWarTime - gCurTime)) / 1000;
	if( nLimitTime < 0 )
		nLimitTime = 0;

	static int last = 0;
	if( last != nLimitTime )
	{
		sprintf(buf, "%02d:%02d", nLimitTime / 60, nLimitTime % 60);
        m_pTimeStatic->SetStaticText(buf);
	}

	we = cDialog::ActionEvent( mouseInfo );

	return we;
}

void CFWTimeDialog::SetActiveWithTimeName( BOOL val, DWORD dwTime, char* pName )
{
	if( val == TRUE )
	{
		m_dwWarTime = dwTime*1000 + gCurTime;	//초
		m_pCharacterName->SetStaticText( pName );
	}
	else
	{
		m_dwWarTime = 0;
		m_pCharacterName->SetStaticText( "" );
	}

	cDialog::SetActive( val );
}

void CFWTimeDialog::SetCharacterName( char* pName )
{
	m_pCharacterName->SetStaticText( pName );
}


//// 요새 & 공성 창고
CFWWareHouseDialog::CFWWareHouseDialog()
{
	m_Lock = FALSE;
	m_InitInfo = FALSE;
	m_pTempMoveItem = NULL;
	m_nTempMovePos = -1;

	m_nDialogKind = eSeigeWareHouseDlg;
	m_wStartPos = TP_SIEGEFORTWARE_START;
}

CFWWareHouseDialog::~CFWWareHouseDialog()
{
}

void CFWWareHouseDialog::Add( cWindow * window )
{
    if( window->GetType() == WT_PUSHUPBUTTON )
        AddTabBtn( curIdx1++, (cPushupButton*)window );
	else if( window->GetType() == WT_ICONGRIDDIALOG )
	{
        AddTabSheet( curIdx2++, window );
        ((cIconGridDialog*)window)->SetDragOverIconType( WT_ITEM );
	}
	else
        cDialog::Add( window );
}

void CFWWareHouseDialog::Render()
{
    cDialog::RenderWindow();
    cDialog::RenderComponent();
    cTabDialog::RenderTabComponent();
}

void CFWWareHouseDialog::Linking()
{
	m_pMoney = (cStatic*)GetWindowForID( FW_MONEY );
}

POSTYPE CFWWareHouseDialog::GetRelativePosition( POSTYPE absPos )
{
	return (absPos - TP_SIEGEFORTWARE_START) % TABCELL_SEIGEFORTWAREHOUES_NUM;
}

BOOL CFWWareHouseDialog::GetPositionForXYRef( BYTE param, LONG x, LONG y, WORD& pos )
{
	WORD position = 0;										// 상대 위치

	cIconGridDialog* gridDlg = (cIconGridDialog*)GetTabSheet(GetCurTabNum());
	if( gridDlg->GetPositionForXYRef(x, y, position) )
	{
		pos = position + TP_SIEGEFORTWARE_START + TABCELL_SEIGEFORTWAREHOUES_NUM*GetCurTabNum();
		return TRUE;
	}
	return FALSE;
}

WORD CFWWareHouseDialog::GetTabIndex( POSTYPE absPos )
{
	return (absPos - TP_SIEGEFORTWARE_START) / TABCELL_SEIGEFORTWAREHOUES_NUM;
}

void CFWWareHouseDialog::SetFortWarWareHouseInfo( MSG_FORTWAR_WAREHOUSE_INFO* pInfo, int nKind )
{
	cTabDialog::SetActive( FALSE );

	m_nDialogKind = nKind;

	DeleteAllItem();
	for( int i = 0; i < pInfo->nItemCount; ++i )
		AddItem( &pInfo->WarehouseItem[i] );
	SetMoney( pInfo->dwMoney );
}

BOOL CFWWareHouseDialog::AddItem( ITEMBASE* pItemInfo )
{
	if( pItemInfo->dwDBIdx == 0 )
	{
		return FALSE;
	}

	CItem* pItem = ITEMMGR->GetItem( pItemInfo->dwDBIdx );
	if( pItem )
	{
		ITEMBASE* pInfo = (ITEMBASE*)pItem->GetItemBaseInfo();
		*pInfo = *pItemInfo;	
	}
	else
	{
		pItem = ITEMMGR->MakeNewItem( pItemInfo, "CItemFortWarWareHouseDialog::AddItem" );
		if( pItem == NULL )	return FALSE;
	}
	pItem->SetMovable( TRUE );

	//색 변환
	ITEMMGR->RefreshItem( pItem );

	//
	int tabIndex = GetTabIndex( pItem->GetPosition() );
	POSTYPE pos = GetRelativePosition( pItem->GetPosition() );
	cIconGridDialog* pDlg = (cIconGridDialog*)GetTabSheet(tabIndex);
	return pDlg->AddIcon( pos, pItem );
}

BOOL CFWWareHouseDialog::DeleteItem( POSTYPE Pos, CItem** ppItem )
{
	int nTabIndex = GetTabIndex( Pos );
	cIconGridDialog* pDlg = (cIconGridDialog *)GetTabSheet(nTabIndex);

	WORD pos = Pos - TP_SIEGEFORTWARE_START - TABCELL_SEIGEFORTWAREHOUES_NUM*nTabIndex;

	if(!pDlg->IsAddable(pos) )
		return pDlg->DeleteIcon(pos, (cIcon **)ppItem);
	else
		return FALSE;
}

void CFWWareHouseDialog::DeleteAllItem()
{
	cIconGridDialog* pDlg1 = (cIconGridDialog*)GetTabSheet(0);
	cIconGridDialog* pDlg2 = (cIconGridDialog*)GetTabSheet(1);
	cIconGridDialog* pDlg3 = (cIconGridDialog*)GetTabSheet(2);
	CItem* pItem = NULL;
	for( WORD i = 0; i < TABCELL_SEIGEFORTWAREHOUES_NUM; ++i )
	{
		pDlg1->DeleteIcon( i, (cIcon**)&pItem );
		pDlg2->DeleteIcon( i, (cIcon**)&pItem );
		pDlg3->DeleteIcon( i, (cIcon**)&pItem );
	}
}

BOOL CFWWareHouseDialog::FakeMoveIcon( LONG x, LONG y, cIcon* icon )
{
	return FALSE;
}

BOOL CFWWareHouseDialog::FakeMoveItem( LONG mouseX, LONG mouseY, CItem* pFromItem )
{
	return FALSE;	
}

BOOL CFWWareHouseDialog::FakeGeneralItemMove( POSTYPE ToPos, CItem* pFromItem, CItem* pToItem )
{
	m_pTempMoveItem = NULL;
	m_nTempMovePos = -1;

	// 옮길위치에 아이템이 있다면
	if( pToItem != NULL )
		return FALSE;

	// 옮길 위치가 인벤 또는 샵인벤
	BYTE ToIdx = ITEMMGR->GetTableIdxForAbsPos( ToPos );
	if( ToIdx != eItemTable_Inventory )
		return FALSE;

	if( pFromItem->GetPosition() == ToPos )
		return FALSE;

	if( m_nDialogKind == eFortWareHouseDlg )
	{
        // 여기서 msgbox띄운다 - 일정확률로 공성창고로 들어간다...
        WINDOWMGR->MsgBox( MBI_FT_ITEM_MOVE_SYN, MBT_YESNO, CHATMGR->GetChatMsg( 1759 ) );
//		WINDOWMGR->MsgBox( MBI_FT_ITEM_MOVE_SYN, MBT_YESNO, "30%%확률로 공성창고로 아이템이 이동할 수 있습니다. 아이템을 이동하시겠습니까?" );
		m_pTempMoveItem = pFromItem;
		m_nTempMovePos = ToPos;
		return FALSE;
	}
	else
	{
		MSG_ITEM_MOVE_SYN msg;
		msg.Category = MP_FORTWAR;
		msg.Protocol = MP_FORTWAR_SIEGEWAREHOUSE_ITEM_MOVE_SYN;
		msg.dwObjectID = HEROID;

		msg.FromPos = pFromItem->GetPosition();
		msg.wFromItemIdx = pFromItem->GetItemIdx();
		msg.ToPos = ToPos;
		msg.wToItemIdx = 0;

		NETWORK->Send( &msg, sizeof(msg) );
	}	

	ITEMMGR->SetDisableDialog( TRUE, eItemTable_Inventory );
	SetDisable( TRUE );

	return TRUE;
}

void CFWWareHouseDialog::SendItemMoveSyn()
{
	if( m_pTempMoveItem == NULL )
		return;
	if( m_nTempMovePos == -1 )
		return;

	MSG_ITEM_MOVE_SYN msg;
	msg.Category = MP_FORTWAR;
	msg.Protocol = MP_FORTWAR_WAREHOUSE_ITEM_MOVE_SYN;
	msg.dwObjectID = HEROID;

	msg.FromPos = m_pTempMoveItem->GetPosition();
	msg.wFromItemIdx = m_pTempMoveItem->GetItemIdx();
	msg.ToPos = m_nTempMovePos;
	msg.wToItemIdx = 0;

	NETWORK->Send( &msg, sizeof(msg) );
}

void CFWWareHouseDialog::SetActive(BOOL val)
{
	if( val == FALSE )
	{
		MSGBASE msg;
		msg.Category = MP_FORTWAR;
		msg.Protocol = MP_FORTWAR_WAREHOUSE_LEAVE;
		msg.dwObjectID = HEROID;
		NETWORK->Send( &msg, sizeof(msg) );

		if( ( HERO->GetState() == eObjectState_Deal ) )
		{
			OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		}
	}
    
	cTabDialog::SetActive(val);
}

DWORD CFWWareHouseDialog::ActionEvent(CMouse * mouseInfo)
{
	return cTabDialog::ActionEvent( mouseInfo );
}

void CFWWareHouseDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{	
}

void CFWWareHouseDialog::PutOutMoneyMsgBox()
{	
	if( m_nDialogKind == eFortWareHouseDlg )
	{
		// 여기서 msgbox띄운다 - 일정확률로 공성창고로 들어간다...
		WINDOWMGR->MsgBox( MBI_FT_MONEY_MOVE_SYN, MBT_YESNO, CHATMGR->GetChatMsg( 1758 ) );
//		WINDOWMGR->MsgBox( MBI_FT_MONEY_MOVE_SYN, MBT_YESNO, "찾는돈의 30%%는 공성창고로 이동합니다. 돈을 찾으시겠습니까?" );
	}
	else
	{
		PutOutMoney();
	}
}

void CFWWareHouseDialog::PutOutMoney()
{	
	cDivideBox* pDivideBox = NULL;
	if( m_nDialogKind == eFortWareHouseDlg )
		pDivideBox = WINDOWMGR->DivideBox( DBOX_INPUT_MONEY, (LONG)(m_pMoney->GetAbsX()-50) , (LONG)(m_pMoney->GetAbsY()-100), OnPutOutMoneyFromFortWarWareOk, NULL, NULL, this, CHATMGR->GetChatMsg(231) );
	else
		pDivideBox = WINDOWMGR->DivideBox( DBOX_INPUT_MONEY, (LONG)(m_pMoney->GetAbsX()-50) , (LONG)(m_pMoney->GetAbsY()-100), OnPutOutMoneyFromSeigeWarWareOk, NULL, NULL, this, CHATMGR->GetChatMsg(231) );

	pDivideBox->SetMaxValue(4000000000);
}

void CFWWareHouseDialog::OnPutOutMoneyFromFortWarWareOk( LONG iId, void* p, DWORD param1, void* vData1, void* vData2 )
{
	if( param1 == 0 )
		return;

//	if( HERO->GetState() != eObjectState_Deal )
//		return;
//	if( GUILDMGR->GetLocation() != MAP->GetMapNum() )
//		return;
//	if( this->IsActive() == FALSE )
//		return;

	MSG_DWORD msg;
	msg.Category = MP_FORTWAR;
	msg.Protocol = MP_FORTWAR_WAREHOUSE_PUTOUT_MONEY_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = param1;

	NETWORK->Send( &msg, sizeof(msg) );
}

void CFWWareHouseDialog::OnPutOutMoneyFromSeigeWarWareOk( LONG iId, void* p, DWORD param1, void* vData1, void* vData2 )
{
	if( param1 == 0 )
		return;

	//	if( HERO->GetState() != eObjectState_Deal )
	//		return;
	//	if( GUILDMGR->GetLocation() != MAP->GetMapNum() )
	//		return;
	//	if( this->IsActive() == FALSE )
	//		return;

	MSG_DWORD msg;
	msg.Category = MP_FORTWAR;
	msg.Protocol = MP_FORTWAR_SIEGEWAREHOUSE_PUTOUT_MONEY_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = param1;

	NETWORK->Send( &msg, sizeof(msg) );
}

void CFWWareHouseDialog::SetMoney( MONEYTYPE money )
{
	m_pMoney->SetStaticText( AddComma(money) );
}



