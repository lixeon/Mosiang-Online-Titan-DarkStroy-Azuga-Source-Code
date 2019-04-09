// ExchangeDialog.cpp: implementation of the CExchangeDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ExchangeDialog.h"
#include "WindowIDEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cScriptManager.h"
#include "./Interface/cIconGridDialog.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cEditBox.h"
#include "./Interface/cStatic.h"

#include "ExchangeManager.h"
#include "ChatManager.h"
#include "ObjectManager.h"
#include "GameIn.h"
#include "ExchangeItem.h"
#include "Item.h"

#include "InventoryExDialog.h"
#include "./Input/UserInput.h"

#include "MainBarDialog.h"
#include "ItemManager.h"
#include "QuickManager.h"
#include "PetManager.h"
#include "TitanManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExchangeDialog::CExchangeDialog()
{
	m_bInit				= FALSE;
	m_pMyIconGridDlg	= NULL;
	m_pOpIconGridDlg	= NULL;
	m_pMyMoneyEditBox	= NULL;
	m_pOpMoneyEditBox	= NULL;
	m_pLockBtn			= NULL;
	m_pExchangeBtn		= NULL;
	m_pInputMoneyBtn	= NULL;

	m_pMyNameStc		= NULL;
	m_pOpNameStc		= NULL;

	m_bMyLock			= FALSE;
	m_bOpLock			= FALSE;

	m_dwMoney			= 0;
	m_dwOpMoney			= 0;

	m_bExchangable		= TRUE;

	m_nMyItemNum		= 0;
	m_nOpItemNum		= 0;	
}

CExchangeDialog::~CExchangeDialog()
{

}

void CExchangeDialog::Linking()
{
	m_pMyIconGridDlg	= (cIconGridDialog*)GetWindowForID( XCI_ICD_MINE );
	m_pOpIconGridDlg	= (cIconGridDialog*)GetWindowForID( XCI_ICD_OPPOSITE );
	m_pMyMoneyEditBox	= (cEditBox*)GetWindowForID( XCI_EB_MYMONEY );
	m_pMyMoneyEditBox->SetReadOnly( TRUE );				//스크립트로
	m_pMyMoneyEditBox->SetAlign( TXT_RIGHT );
//	m_pMyMoneyEditBox->SetValidCheck( VCM_NUMBER );
	m_pOpMoneyEditBox	= (cEditBox*)GetWindowForID( XCI_EB_OPMONEY );
	m_pMyMoneyEditBox->SetReadOnly( TRUE );
	m_pOpMoneyEditBox->SetAlign( TXT_RIGHT );
	m_pLockBtn			= (cPushupButton*)GetWindowForID( XCI_BTN_LOCK );
	m_pExchangeBtn		= (cPushupButton*)GetWindowForID( XCI_BTN_EXCHANGE );
	m_pInputMoneyBtn	= (cButton*)GetWindowForID( XCI_BTN_INPUTMONEY );
	
	m_pMyNameStc		= (cStatic*)GetWindowForID( XCI_INFO2 );
	m_pOpNameStc		= (cStatic*)GetWindowForID( XCI_INFO1 );
	//m_pLockImage		= WINDOWMGR->GetImageOfFile( "./image/GameIn/28/120210.tif", 28 );
	SCRIPTMGR->GetImage( 65, &m_LockImage, PFT_HARDPATH );

	m_bInit				= TRUE;
}
/*
DWORD CExchangeDialog::ActionEvent( CMouse* mouseInfo, CKeyboard* keyInfo )
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;

//	LONG x = mouseInfo->GetClickDownX();
//	LONG y = mouseInfo->GetClickDownY();

//	if( mouseInfo->LButtonClick() )
//	{
//		we = m_pInputMoneyBtn->ActionEvent( mouseInfo, keyInfo );
//		if( !( we & WE_LBTNCLICK ) )
//		{
//			we = m_pMyMoneyEditBox->ActionEvent( mouseInfo, keyInfo );
//			if( !( we & WE_LBTNCLICK ) )
//				InputMoney( 0, m_dwMoney );		//추가 : 변경이 안되었으면 할필요 없음
//		}
//	}
	
	return cDialog::ActionEvent(mouseInfo, keyInfo);
}
*/
void CExchangeDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK )
	{
		if( lId == XCI_BTN_EXIT || lId == XCI_BTN_CLOSE )
		{
			MSGBASE msg;
			msg.Category	= MP_EXCHANGE;
			msg.Protocol	= MP_EXCHANGE_CANCEL_SYN;
			msg.dwObjectID	= HERO->GetID();

			NETWORK->Send( &msg, sizeof( MSGBASE ) );
			SetDisable( TRUE );
			//이상하다! 여기서 cancel메세지를 보내고
			//ack받으면 아래로. confirm
//			SetActive( FALSE );
		}
		else if( lId == XCI_BTN_INPUTMONEY )
		{
			if( m_bMyLock )
			{
				//"잠금상태에서는 돈 입력이 안됩니다."
			}
			else
			{
				//만약 락 걸었다면 리턴. //버튼 안눌러지게하기.
				LONG x = MOUSE->GetMouseEventX();
				LONG y = MOUSE->GetMouseEventY();
				
				cDivideBox* pDivideBox = WINDOWMGR->DivideBox( DBOX_EXCHANGE_MONEY,
								x, y, OnInputMoney, OnInputCancel, this, NULL, CHATMGR->GetChatMsg(198) );
				SetDisable( TRUE );
				//pDivideBox->SetMaxValue( MAX_MONEY );?
			}
		}
	}
	if( we & WE_PUSHDOWN )
	{
		if( lId == XCI_BTN_LOCK )
		{
			if( EXCHANGEMGR->IsLockSyncing() )
			{
				m_pLockBtn->SetPush( FALSE );	//DOWN되지 말아야 한다.
			}
			else
			{
				MyLock( TRUE );
				EXCHANGEMGR->Lock( TRUE );	//NetworkMsg
			}
		}
		else if( lId == XCI_BTN_EXCHANGE )
		{
			if( m_bMyLock && !EXCHANGEMGR->IsLockSyncing() && m_bOpLock )
			{
				//인벤토리의 상태 파악..
				int nBlankNum = GAMEIN->GetInventoryDialog()->GetBlankNum();
				if( nBlankNum < m_nOpItemNum - m_nMyItemNum )
				{
					//추가 : 교환불가능 메세지
					//인벤토리 공간이 부족합니다.
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 354 ) );

					m_pExchangeBtn->SetPush( FALSE );
				}
				//2007. 8. 29. CBH - 금액 관련 자료형 오버플로우 개선
				//else if( m_dwOpMoney + HERO->GetMoney() > MAX_INVENTORY_MONEY ) //돈체크
				else if( m_dwOpMoney > MAX_INVENTORY_MONEY - HERO->GetMoney() ) //돈체크
				{
					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(339) );
					m_pExchangeBtn->SetPush( FALSE );
				}
				else
				{
					//추가 : 인벤토리 잠시 멈춤	//quick창은.. 인벤이 멈췄으면 사용 불능?
					//GAMEIN->GetInventoryDialog()->SetDisable( TRUE );
					//SetDisable( TRUE );
					EXCHANGEMGR->Exchange();
				}
			}
			else
			{
				//양쪽 모두 락을 해야 교환 하실 수 있습니다.
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(330) );
				m_pExchangeBtn->SetPush( FALSE );
			} 
		}
	}
	if( we & WE_PUSHUP )
	{
		if( lId == XCI_BTN_LOCK )
		{
#ifdef _HK_LOCAL_
				m_pLockBtn->SetPush( TRUE );	//UP되지 말아야 한다.
#else
			if( EXCHANGEMGR->IsLockSyncing() || EXCHANGEMGR->IsExchangeSyncing() )
			{
				m_pLockBtn->SetPush( TRUE );	//UP되지 말아야 한다.
			}
			else
			{
				EXCHANGEMGR->Lock( FALSE );	//network msg
			}
#endif
		}
		else if( lId == XCI_BTN_EXCHANGE )
		{
			m_pExchangeBtn->SetPush( TRUE );
		}
	}
	if( we & WE_LBTNCLICK )
	{
		if( lId == XCI_ICD_MINE )
		{
//			CItem* pItem = GAMEIN->GetInventoryDialog()->GetCurSelectedItem( INV );
//			cIconGridDialog* pIGD = (cIconGridDialog*)GetWindowForID( XCI_ICD_MINE );
//			if( pItem )
//				pIGD->AddIcon( 0, 0, pItem );
		}
	}
	
}

void CExchangeDialog::SetPlayerName( char* pMyName, char* pOpName )
{
	m_pMyNameStc->SetStaticText( pMyName );
	m_pOpNameStc->SetStaticText( pOpName );
}

void CExchangeDialog::SetActive( BOOL val )
{
	if( m_bDisable )		return;
	if( m_bActive == val )	return;

//	cDialog::SetActiveRecursive( val );	//돈입력창때문이었는데.... 안해도 될듯 confirm
	cDialog::SetActive( val );
	
	if( val )
	{
		m_pMyMoneyEditBox->SetEditText( "0" );
		m_pOpMoneyEditBox->SetEditText( "0" );

		m_pLockBtn->SetPush( FALSE );
		m_pExchangeBtn->SetPush( FALSE );

		
		//SetDisable( FALSE );
	}
	else
	{
		if( !m_bInit ) return;	//Link() 호출 후 부터

		//아이템 지우기
//		cIcon *pIcon = NULL;
		int i = 0;
		int nMyItemNum = m_nMyItemNum;
		int nOpItemNum = m_nOpItemNum;

		for( i = 0 ; i < nMyItemNum ; ++i )
			DelItem( 0, 1 );	//추가 : 수정요
		for( i = 0 ; i < nOpItemNum ; ++i )
			DelItem( 1, 1 );	//추가 : 수정요

		m_bMyLock	= FALSE;
		m_bOpLock	= FALSE;

		m_dwMoney	= 0;
		m_dwOpMoney = 0;


		m_bExchangable = TRUE;

		m_nMyItemNum		= 0;
		m_nOpItemNum		= 0;


//		여기에 왜 있는거지? confirm
//		MSGBASE msg;
//		msg.Category	= MP_EXCHANGE;
//		msg.Protocol	= MP_EXCHANGE_CANCEL_SYN;
//		msg.dwObjectID	= HERO->GetID();

//		NETWORK->Send( &msg, sizeof( MSGBASE ) );

		EXCHANGEMGR->Init();	//이것도...

		cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_EXCHANGE_MONEY );
		if( pDlg )
		{
			WINDOWMGR->AddListDestroyWindow( pDlg );
		}
	}

//--- main bar icon
//	CMainBarDialog* pDlg = GAMEIN->GetMainInterfaceDialog();
//	if( pDlg )
//		pDlg->SetPushBarIcon( OPT_EXCHANGEDLGICON, m_bActive );
}

void CExchangeDialog::Render()
{
	cDialog::Render();

	if( !m_LockImage.IsNull() )
	{

		VECTOR2 vPos;

		if( m_bMyLock )
		{
//			VECTOR2 vPos = { m_pMyIconGridDlg->m_absPos.x + 3, m_pMyIconGridDlg->m_absPos.y + 3 };
//			m_LockImage.RenderSprite( &vScale, NULL, 0, &vPos,
//										RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100) );

			for( int i = 0 ; i < 10 ; ++i )
			{
				if( !m_pMyIconGridDlg->IsAddable( i ) ) continue;
				vPos.x = ( m_pMyIconGridDlg->m_absPos.x + 5 ) + ( i % 5 ) * 45 ;
				vPos.y = ( m_pMyIconGridDlg->m_absPos.y + 5 ) + ( i / 5 ) * 45 ;

				m_LockImage.RenderSprite( NULL, NULL, 0.0f, &vPos,
										  RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100) );

			}
		}

		if( m_bOpLock )
		{
//			VECTOR2 vPos = { m_pOpIconGridDlg->m_absPos.x + 3, m_pOpIconGridDlg->m_absPos.y + 3 };
//			m_LockImage.RenderSprite( &vScale, NULL, 0, &vPos, 
//										RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100) );

			for( int i = 0 ; i < 10 ; ++i )
			{
				if( !m_pOpIconGridDlg->IsAddable( i ) ) continue;
				
				vPos.x = ( m_pOpIconGridDlg->m_absPos.x + 5 ) + ( i % 5 ) * 45 ;
				vPos.y = ( m_pOpIconGridDlg->m_absPos.y + 5 ) + ( i / 5 ) * 45 ;

				m_LockImage.RenderSprite( NULL, NULL, 0.0f, &vPos,
										  RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100) );

			}
		}
	}
}

BOOL CExchangeDialog::FakeMoveIcon( LONG x, LONG y, cIcon* icon )
{
	if( m_bDisable ) return FALSE;	//교환창 멈춤중이면 리턴

	if( icon->GetType() != WT_ITEM )
		return FALSE;

/*	// 문파쪽지 거래에러 긴급수정
	if(((CItem*)icon)->GetItemKind() == eEXTRA_ITEM_USABLE)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(349) );
		return FALSE;
	}
*/
	if( ( GetTableIdxPosition( ((CItem*)icon)->GetPosition() ) != eItemTable_Inventory ) &&
		( GetTableIdxPosition( ((CItem*)icon)->GetPosition() ) != eItemTable_ShopInven ) )
	{
		if( GetTableIdxPosition( ((CItem*)icon)->GetPosition() ) == eItemTable_Weared )
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(315) );
		
		return FALSE;
	}

	if( ((CItem*)icon)->GetItemKind() & eSHOP_ITEM )
	{
		// 기한제 아이콘이 남는 아이템이 봉인풀리면 거래 안되도록
		ITEM_INFO* pInfo = ITEMMGR->GetItemInfo( ((CItem*)icon)->GetItemIdx() );
		if( !pInfo )			return FALSE;
		if( pInfo->ItemType == 11 )
		{
			if( !(((CItem*)icon)->GetItemBaseInfo()->ItemParam & ITEM_PARAM_SEAL) )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1223) );
				return FALSE;
			}
		}
		// 거래 안되는 아이템
		if( ((CItem*)icon)->GetItemKind() == eSHOP_ITEM_EQUIP || ((CItem*)icon)->GetItemKind() == eSHOP_ITEM_PET_EQUIP )
		{
			if( pInfo->BuyPrice )
			{
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1223) );
				return FALSE;
			}
		}
		else if( pInfo->MeleeAttackMax )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1223) );
			return FALSE;
		}
	}

//	if( ((CItem*)icon)->GetItemKind() == eQUEST_ITEM_PET || ((CItem*)icon)->GetItemKind() == eSHOP_ITEM_PET)
	if( ITEMMGR->IsPetSummonItem( ((CItem*)icon)->GetItemIdx() ) )
	{
		//해당 펫 정보 중 친밀도 검사. 30%이하인지..
		const ITEMBASE* itembase = ((CItem*)icon)->GetItemBaseInfo();
		if(!PETMGR->CheckDefaultFriendship(itembase->dwDBIdx))
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1275) );
			return FALSE;
		}
		//소환 중인 펫 아이템인지
		if( PETMGR->IsCurPetSummonItem(itembase->dwDBIdx) )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1247) );
			return FALSE;
		}
	}

	// changeitem 중에 거래안되는거
	if( ((CItem*)icon)->GetItemKind() == eCHANGE_ITEM_LOCK )
	{
		return FALSE;
	}
	
	if( ((CItem*)icon)->GetItemKind() == eYOUNGYAK_ITEM ||
		((CItem*)icon)->GetItemKind() == eSHOP_ITEM_INCANTATION ||
		((CItem*)icon)->GetItemKind() == eSHOP_ITEM_HERB ||
		((CItem*)icon)->GetItemKind() == eSHOP_ITEM_CHARM ||
		((CItem*)icon)->GetItemKind() == eSHOP_ITEM_SUNDRIES )
	{
		if( QUICKMGR->CheckQPosForItemIdx(((CItem*)icon)->GetItemIdx()) == FALSE )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(317) );
			return FALSE;
		}
	}
	else
	{
		if( ((CItem*)icon)->GetQuickPosition() != 0 )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(317) );
			return FALSE;
		}
	}

	// magi82 - Titan(070618)
	// 등록된 타이탄 증서는 교환할수 없음
	if( ((CItem*)icon)->GetDBIdx() == TITANMGR->GetRegistedTitanItemDBIdx() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1583) );
		return FALSE;
	}

	// magi82 - UniqueItem(070628)
	eITEM_KINDBIT bits = ((CItem*)icon)->GetItemKind();
	if( bits == eEQUIP_ITEM_UNIQUE && GAMERESRCMNGR->GetUniqueItemOptionList(((CItem*)icon)->GetItemIdx())->ExchangeFlag == 0 )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1626) );
		return FALSE;
	}

	if( m_pMyIconGridDlg->PtInWindow( x, y ) && m_bMyLock == FALSE )
	{
		if( EXCHANGEMGR->LinkItem( (CItem*)icon ) )
			SetDisable( TRUE );
	}
	return FALSE;
}

void CExchangeDialog::FakeDeleteItem( POSTYPE pos )
{
	if( m_bDisable ) return; //교환창 멈춤중이면 리턴

	MSG_WORD msg;
	msg.Category	= MP_EXCHANGE;
	msg.Protocol	= MP_EXCHANGE_DELITEM_SYN;
	msg.dwObjectID	= HERO->GetID();
	msg.wData		= pos;

	NETWORK->Send( &msg, sizeof( MSG_WORD ) );

	SetDisable( TRUE );
	//추가 : 교환창 멈춤
}


void CExchangeDialog::AddItem( int nType, POSTYPE pos, cIcon* pItem )
{
	if( nType == 0 )
	{
		m_pMyIconGridDlg->AddIcon( pos-1, pItem );
		++m_nMyItemNum;
	}
	else
	{
		m_pOpIconGridDlg->AddIcon( pos-1, pItem );
		++m_nOpItemNum;
	}

	//추가 : 교환창 멈춤 풀기
}


void CExchangeDialog::DelItemOptionInfoAll()
{
	//취소되었을때	: itemOptioninfo를 삭제한다.

	CExchangeItem*	pIcon = NULL;
	ITEM_INFO*		pInfo = NULL;
	for( int i = 1 ; i <= m_nOpItemNum ; ++i )
	{
		DelItemOptionInfo( (POSTYPE)i );
	}
}

void CExchangeDialog::DelItemOptionInfo( POSTYPE pos )
{
	CExchangeItem* pIcon = (CExchangeItem*)m_pOpIconGridDlg->GetIconForIdx( pos - 1 );
	if( !pIcon ) return;

	if( ITEMMGR->IsOptionItem( (WORD)pIcon->GetData(), pIcon->GetDurability() ) )
	{
		ITEMMGR->RemoveItemOption( pIcon->GetDurability() );
	}
}
//SW050920 Rare
void CExchangeDialog::DelItemRareOptionInfo( POSTYPE pos )
{
	CExchangeItem* pIcon = (CExchangeItem*)m_pOpIconGridDlg->GetIconForIdx( pos - 1 );
	if( !pIcon ) return;

	if( ITEMMGR->IsRareOptionItem( (WORD)pIcon->GetData(), pIcon->GetRareness() ) )
	{
		ITEMMGR->RemoveItemRareOption( pIcon->GetRareness() );
	}
}

void CExchangeDialog::DelPetItemInfo( POSTYPE pos )
{
	CExchangeItem* pIcon = (CExchangeItem*)m_pOpIconGridDlg->GetIconForIdx( pos - 1 );
	if( !pIcon ) return;

	DWORD itemDBIdx = pIcon->GetDBId();
	if( ITEMMGR->IsPetSummonItem( (WORD)pIcon->GetData() ) )
	{
		PETMGR->RemovePetInfo(itemDBIdx);
	}
}

void CExchangeDialog::DelPetItemInfoAll()
{
	for( int i = 1; i <= m_nOpItemNum; ++i )
	{
		DelPetItemInfo( (POSTYPE)i );
	}
}
	
void CExchangeDialog::DelItemRareOptionInfoAll()
{
	for( int i = 1; i <= m_nOpItemNum; ++i )
	{
		DelItemRareOptionInfo( (POSTYPE)i );
	}
}

void CExchangeDialog::DelItem( int nType, POSTYPE pos )
{
	cIcon* pIcon = NULL;

	if( nType == 0 )
	{
		if( m_pMyIconGridDlg->DeleteIcon( pos-1, &pIcon ) )
		{
			if( pIcon == NULL) return;
			
			EXCHANGEMGR->UnLinkItem( 0, pIcon );

			for( int i = pos ; i < m_nMyItemNum ; ++i )
			{
				m_pMyIconGridDlg->DeleteIcon( i, &pIcon );
				if( pIcon == NULL ) break;

				((CExchangeItem*)pIcon)->SetPosition( i );
				m_pMyIconGridDlg->AddIcon( i - 1, pIcon );
			}

			//linkitem도 하나 줄이기.////////////
			--m_nMyItemNum;
		}
	}
	else
	{
		if( m_pOpIconGridDlg->DeleteIcon( pos-1, &pIcon ) )
		{
			if( pIcon == NULL) return;

			EXCHANGEMGR->UnLinkItem( 1, pIcon );

			for( int i = pos ; i < m_nOpItemNum ; ++i )
			{
				m_pOpIconGridDlg->DeleteIcon( i, &pIcon );
				if( pIcon == NULL ) break;

				m_pOpIconGridDlg->AddIcon( i - 1, pIcon );
			}
			--m_nOpItemNum;
		}
	}

	//추가 : 교환창 멈춤 풀기	
}

DWORD CExchangeDialog::InputMoney( int nType, DWORD dwMoney )
{
	if( nType == 0 )
	{
		m_dwMoney = dwMoney;
		m_pMyMoneyEditBox->SetEditText( AddComma( dwMoney ) );
	}
	else
	{
		m_dwOpMoney = dwMoney;
#ifndef _HK_LOCAL_
		m_pOpMoneyEditBox->SetEditText( AddComma( dwMoney ) ); //홍콩은 실시간이 아니라 락걸때만 보여준다.
#endif
	}

	return dwMoney;
}

void CExchangeDialog::OpLock( BOOL bLock )
{
	m_bOpLock = bLock;

	int i;
	for( i = 0 ; i < m_nOpItemNum ; ++i )
	{
		if( m_pOpIconGridDlg->GetIconForIdx( i ) )
			m_pOpIconGridDlg->GetIconForIdx( i )->SetLock( bLock );
	}

	if( bLock == FALSE )	//상대가 락을 풀면.. 나의 상태도 바꾼다.
	{
		//추가 : Syncing에 대한 문제 생각하기
		m_pExchangeBtn->SetPush( FALSE );

		//추가 : 작동 가능
#ifdef _HK_LOCAL_
		m_pOpMoneyEditBox->SetEditText( "0" );
#endif

	/////////MyLock
		m_bMyLock = FALSE;

		for( i = 0 ; i < m_nMyItemNum ; ++i )
		{
			if( m_pMyIconGridDlg->GetIconForIdx( i ) )
				m_pMyIconGridDlg->GetIconForIdx( i )->SetLock( FALSE );
		}
		
		m_pInputMoneyBtn->SetDisable( FALSE );
		m_pLockBtn->SetPush( FALSE );
	/////////////
	}
#ifdef _HK_LOCAL_
	else	//lock
	{
		m_pOpMoneyEditBox->SetEditText( AddComma( m_dwOpMoney ) );
	}
#endif
}

void CExchangeDialog::MyLock( BOOL bLock )
{
	m_bMyLock = bLock;

	int i;
	for( i = 0 ; i < m_nMyItemNum ; ++i )
	{
		if( m_pMyIconGridDlg->GetIconForIdx( i ) )
			m_pMyIconGridDlg->GetIconForIdx( i )->SetLock( bLock );
	}
	
	if( bLock )
	{
		m_pInputMoneyBtn->SetDisable( TRUE );

		cDialog* pDlg = WINDOWMGR->GetWindowForID( DBOX_EXCHANGE_MONEY );
		if( pDlg )
		{
			WINDOWMGR->AddListDestroyWindow( pDlg );
		}
	}
	else
	{
		m_bOpLock = bLock;
		for( i = 0 ; i < m_nOpItemNum ; ++i )		//함수로 락ACK일때 실행한다?
		{
			if( m_pOpIconGridDlg->GetIconForIdx( i ) )
				m_pOpIconGridDlg->GetIconForIdx( i )->SetLock( bLock );
		}

		m_pInputMoneyBtn->SetDisable( FALSE );
	}

	m_pLockBtn->SetPush( bLock );
}


void CExchangeDialog::OnInputMoney( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CExchangeDialog* pDlg = (CExchangeDialog*)vData1;
	if( param1 == pDlg->GetMoney() )
	{
		pDlg->SetDisable( FALSE );
		return;
	}
	
	if( !pDlg->IsActive() ) return;
	
	if( pDlg->IsMyLock() )
	{	//락걸린 상태라면
		return;
	}

	DWORD dwCurMoney = HERO->GetMoney();

	if( dwCurMoney < param1 )
		param1 = dwCurMoney;

	MSG_DWORD msg;
	msg.Category	= MP_EXCHANGE;
	msg.Protocol	= MP_EXCHANGE_INPUTMONEY_SYN;
	msg.dwObjectID	= HERO->GetID();
	msg.dwData		= param1;
	NETWORK->Send( &msg, sizeof( MSG_DWORD ) );
}


void CExchangeDialog::OnInputCancel( LONG iId, void* p, DWORD param1, void * vData1, void * vData2 )
{
	CExchangeDialog* pDlg = (CExchangeDialog*)vData1;
	pDlg->SetDisable( FALSE );
}
