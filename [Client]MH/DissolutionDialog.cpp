// DissolutionDialog.cpp: implementation of the CDissolutionDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DissolutionDialog.h"
#include "./Interface/cIconGridDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cScriptManager.h"
#include "Item.h"
#include "ItemManager.h"
#include "DissolutionManager.h"
#include "./Interface/cIconDialog.h"
#include "./Interface/cIconGridDialog.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "InventoryExDialog.h"
#include "ChatManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDissolutionDialog::CDissolutionDialog()
{
	m_pDissolutionItemDlg	= NULL;
	m_pUnitItemDlg[0]		= NULL;
	m_pUnitItemDlg[1]		= NULL;
	m_pUnitItemDlg[2]		= NULL;
	
	cImage imgToolTip;
	SCRIPTMGR->GetImage( 63, &imgToolTip, PFT_HARDPATH );

	m_VirtualDissolutionItem.SetToolTip( "", RGB_HALF( 255, 255, 255), &imgToolTip );
	m_VirtualDissolutionItem.SetMovable(FALSE);

	for(int i = 0 ; i < MAX_DISSOLUTION_UNITNUM ; ++i)
	{
		m_ShowUnitItem[i].SetToolTip( "", RGB_HALF( 255, 255, 255), &imgToolTip );
		m_ShowUnitItem[i].SetMovable(FALSE);
	}

	m_nTotalUnitOutNum	= 0;
	ZeroMemory( &m_CheckItemInfo, sizeof(m_CheckItemInfo) );
}

CDissolutionDialog::~CDissolutionDialog()
{

}

void CDissolutionDialog::Add( cWindow* window )
{
	if( window->GetType() == WT_PUSHUPBUTTON )
		AddTabBtn( curIdx1++, (cPushupButton * )window );
	else if( window->GetType() == WT_ICONGRIDDIALOG )
	{
		AddTabSheet( curIdx2++, window );
		((cIconGridDialog*)window)->SetDragOverIconType( WT_ITEM );
	}
	else 
		cDialog::Add(window);
}

void CDissolutionDialog::Linking()
{
	m_pDissolutionItemDlg	= (cIconDialog*)GetWindowForID(DIS_BASEICONDLG);
	m_pUnitItemDlg[0]		= (cIconGridDialog*)GetWindowForID(DIS_ICONGRIDDLG1);
	m_pUnitItemDlg[1]		= (cIconGridDialog*)GetWindowForID(DIS_ICONGRIDDLG2);
	m_pUnitItemDlg[2]		= (cIconGridDialog*)GetWindowForID(DIS_ICONGRIDDLG3);
}

void CDissolutionDialog::Render()
{
	cDialog::RenderWindow();
	cDialog::RenderComponent();	
	cTabDialog::RenderTabComponent();
}

DWORD CDissolutionDialog::ActionEvent( CMouse* mouseInfo )
{
	DWORD we = WE_NULL;

	we |= cTabDialog::ActionEvent( mouseInfo );

	return we;
}

void CDissolutionDialog::SetActive( BOOL val )
{
	if( m_bDisable ) return;

	if( val == FALSE )
	{
		RemoveDissolutionItem();
		RemoveAllUnitItems();
	}

	cTabDialog::SetActive(val);
}

void CDissolutionDialog::OnActionEvent( LONG lId, void * p, DWORD we )
{
	if( lId == DIS_OKBTN )
	{
		SendMsg_Dissolution();
	}
	else if( lId == DIS_CANCELBTN )
	{
		SetActive( FALSE );
	}

	return;
}

BOOL CDissolutionDialog::FakeMoveIcon( LONG x, LONG y, cIcon * icon )
{
	if( m_bDisable )					return FALSE;
	if( icon == NULL )					return FALSE;
	if( icon->GetType() != WT_ITEM )	return FALSE;

	CItem* pOrigItem = (CItem*)icon;
	if( pOrigItem->IsLocked() )			return FALSE;

	// 인벤토리이외에 아이템 FALSE
	if( !ITEMMGR->IsEqualTableIdxForPos( eItemTable_Inventory, pOrigItem->GetPosition() ) )
		return FALSE;

	//강화 아이템이면 안됨
	if( ITEMMGR->IsOptionItem( pOrigItem->GetItemIdx(), pOrigItem->GetDurability() ) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(689) );	//"강화된 아이템은 분해 할 수 없습니다."
		return FALSE;
	}
	if( ITEMMGR->IsRareOptionItem( pOrigItem->GetItemIdx(), pOrigItem->GetRareness() ) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(690) );
		return FALSE;
	}
	
	//리스트에 있는지 확인
	if( !DISSOLUTIONMGR->CanDissolutionItem( pOrigItem->GetItemIdx() ) )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(690) );	//"분해할 수 없는 아이템 입니다."
		return FALSE;
	}
	
	if( pOrigItem->GetDurability() > 1 ) //겹쳐진 아이템은 안된다.
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1099) );	//"겹쳐진 아이템은 분해할 수 없습니다."
		return FALSE;		
	}

	//기존에 올라와 있던 아이템을 지운다.
	RemoveDissolutionItem();
	RemoveAllUnitItems();

	//아이템을 올린다.
	AddDissolutionItem( pOrigItem );

	return FALSE;
}

CVirtualItem* CDissolutionDialog::GetDissolutionVirtualItem()
{
	 return (CVirtualItem*)m_pDissolutionItemDlg->GetIconForIdx(0);
}

CItem* CDissolutionDialog::GetDissolutionInventoryItem()
{
	 CVirtualItem* pVItem = (CVirtualItem*)m_pDissolutionItemDlg->GetIconForIdx(0);
	 if( pVItem == NULL ) return NULL;

	 return (CItem*)pVItem->GetLinkItem();
}

void CDissolutionDialog::AddDissolutionItem( CItem* pOrigItem )
{
	if( m_pDissolutionItemDlg->AddIcon( 0, &m_VirtualDissolutionItem ) )
	{
		m_nTotalUnitOutNum = 0;

		if( ShowUnitItems( pOrigItem->GetItemIdx() ) )
		{
			m_VirtualDissolutionItem.SetData( pOrigItem->GetItemIdx() );
			m_VirtualDissolutionItem.SetLinkItem( pOrigItem );
			//!!!기본 NULL 인지 확인 SW050920 Rare
			if( ITEMMGR->IsOptionItem( pOrigItem->GetItemIdx(), pOrigItem->GetDurability() ) )
			{
				ITEMMGR->SetToolTipIcon( &m_VirtualDissolutionItem, ITEMMGR->GetItemOption( pOrigItem->GetDurability() ),
					ITEMMGR->GetItemRareOption( pOrigItem->GetRareness() ) );
			}
			else
				ITEMMGR->SetToolTipIcon( &m_VirtualDissolutionItem, NULL, ITEMMGR->GetItemRareOption( pOrigItem->GetRareness() ) );

			pOrigItem->SetLock( TRUE );

			m_CheckItemInfo = *pOrigItem->GetItemBaseInfo();
		}
		else
		{
			//error
			RemoveDissolutionItem();
			RemoveAllUnitItems();
		}
	}
}

void CDissolutionDialog::RemoveDissolutionItem()
{
	CVirtualItem* pVItem = NULL;
	m_pDissolutionItemDlg->DeleteIcon( 0, (cIcon**)&pVItem );
	if( pVItem == NULL )
		return;
	pVItem->SetLinkItem(NULL);

	CItem* pOrigItem = ITEMMGR->GetItem( m_CheckItemInfo.dwDBIdx );
	ZeroMemory( &m_CheckItemInfo, sizeof(m_CheckItemInfo) );
	
	m_nTotalUnitOutNum = 0;
	if( pOrigItem == NULL ) return;
	pOrigItem->SetLock( FALSE );
}

BOOL CDissolutionDialog::ShowUnitItems( WORD wDissolutionItemIdx )
{
	WORD wUnitKindNum = 0;
	sITEM_DISSOLUTIONITEM* pUnitInfo = DISSOLUTIONMGR->GetDissolutionUnitItemInfo( wDissolutionItemIdx, &wUnitKindNum );
	if( pUnitInfo == NULL )
		return FALSE;
	
	if( wUnitKindNum > MAX_DISSOLUTION_UNITNUM ) return FALSE;		//종류가 칸보다 많을 수는 없다.

	for( int i = 0 ; i < wUnitKindNum ; ++i )
	{
		WORD tempUnitItemIdx = pUnitInfo[i].wUnitItemIdx;
		WORD tempUnitItemNum = pUnitInfo[i].wUnitItemNum;

		if( tempUnitItemIdx == 0 || tempUnitItemNum == 0 ) return FALSE;

		//여기서 만약 아이템이 분해할 수 있는 아이템이면 또한번 분해한다.
		eITEM_KINDBIT eItemKind = GetItemKind( tempUnitItemIdx );
		if( !(eItemKind & eEXTRA_ITEM) )
		{
			if( ShowUnitItems( tempUnitItemIdx ) == TRUE )	//분해되면 아래가 필요없다.
				continue;			
		}
		
		while( tempUnitItemNum > 0 )
		{
			if( m_nTotalUnitOutNum >= MAX_DISSOLUTION_UNITNUM ) return FALSE;	//=도 된다.

			if( m_pUnitItemDlg[m_nTotalUnitOutNum/MAX_UNIT_PERTAB]->AddIcon( m_nTotalUnitOutNum%MAX_UNIT_PERTAB, (cIcon*)&m_ShowUnitItem[m_nTotalUnitOutNum] ) )
			{
				m_ShowUnitItem[m_nTotalUnitOutNum].SetData( tempUnitItemIdx );

				if( tempUnitItemNum > MAX_YOUNGYAKITEM_DUPNUM )
				{
					m_ShowUnitItem[m_nTotalUnitOutNum].Init( tempUnitItemIdx, MAX_YOUNGYAKITEM_DUPNUM );
					tempUnitItemNum -= MAX_YOUNGYAKITEM_DUPNUM;	//while(tempUnitItemNum>5) 이므으로 0보다 작아질 수는 없다.
				}
				else
				{
					m_ShowUnitItem[m_nTotalUnitOutNum].Init( tempUnitItemIdx, tempUnitItemNum );
					tempUnitItemNum = 0;
				}

				ITEMMGR->SetToolTipIcon( &m_ShowUnitItem[m_nTotalUnitOutNum] );		
				++m_nTotalUnitOutNum;
			}
			else
			{
				//error
				//올라간 showitem들 지우기
				return FALSE;
			}
		}
	}

	if( m_nTotalUnitOutNum > MAX_DISSOLUTION_UNITNUM ) return FALSE;	//=이 아니다.

	return TRUE;
}


void CDissolutionDialog::RemoveAllUnitItems()
{
	CItemShow* pSItem = NULL;

	for( int i = 0 ; i < MAX_DISSOLUTION_UNITNUM ; ++i )
	{
		m_pUnitItemDlg[i/MAX_UNIT_PERTAB]->DeleteIcon( i%MAX_UNIT_PERTAB, (cIcon**)&pSItem );
		if( pSItem != NULL )
		{
			m_ShowUnitItem[i].SetData( 0 );
			m_ShowUnitItem[i].Init( 0, 0 );
		}
	}
}

BOOL CDissolutionDialog::SendMsg_Dissolution()
{
	const ITEMBASE* pInventoryItemInfo = ITEMMGR->GetItemInfoAbsIn( HERO, m_CheckItemInfo.Position );
	CItem* pItem = GetDissolutionInventoryItem();
	if( pInventoryItemInfo == NULL || pItem == NULL )
	{
		//아이템 정보가 다릅니다.
		return FALSE;
	}

	const ITEMBASE* pDissolutionItemInfo = pItem->GetItemBaseInfo();

	if( pDissolutionItemInfo->dwDBIdx != pInventoryItemInfo->dwDBIdx ||
		pDissolutionItemInfo->wIconIdx != pInventoryItemInfo->wIconIdx ||
		pDissolutionItemInfo->Position != pInventoryItemInfo->Position ||
		pDissolutionItemInfo->Durability != pInventoryItemInfo->Durability ||
		pDissolutionItemInfo->QuickPosition != pInventoryItemInfo->QuickPosition )
	{
		//아이템 정보가 다릅니다.
		return FALSE;
	}

	//아이템 들어갈 공간을 확인하자
	int nBlankNum = GAMEIN->GetInventoryDialog()->GetBlankNum();
	if( m_nTotalUnitOutNum > nBlankNum )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(691) );//인벤토리 공간이 부족하여 분해할 수 없습니다.
		return FALSE;
	}
	
	SetDisable( TRUE );
	ITEMMGR->SetDisableDialog( TRUE, eItemTable_Inventory );

	MSG_ITEM msg;
	msg.Category	= MP_ITEM;
	msg.Protocol	= MP_ITEM_DISSOLUTION_SYN;
	msg.dwObjectID	= HEROID;
	msg.ItemInfo	= *pInventoryItemInfo;
	NETWORK->Send( &msg, sizeof(msg) );

	return TRUE;
}


void CDissolutionDialog::EndDissolution( BOOL bSuccess )
{
	SetDisable( FALSE );
	ITEMMGR->SetDisableDialog( FALSE, eItemTable_Inventory );

	if( bSuccess )
	{
		//주의 : 아래로 내리지마시오. m_CheckItemInfo 초기화되면 안됨.
		ITEMMGR->DeleteItemofTable( eItemTable_Inventory, m_CheckItemInfo.Position );
		//기존에 올라와 있던 아이템을 지운다.
		RemoveDissolutionItem();
		RemoveAllUnitItems();
	}
	else
	{
		
	}
}
