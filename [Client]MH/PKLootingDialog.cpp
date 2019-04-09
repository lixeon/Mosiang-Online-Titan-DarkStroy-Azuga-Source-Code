// PKLootingDialog.cpp: implementation of the CPKLootingDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PKLootingDialog.h"
#include "WindowIDEnum.h"
#include "PKManager.h"
#include "ObjectManager.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cStatic.h"
#include "./Interface/cIconGridDialog.h"
#include "./Interface/cScriptManager.h"
#include "./Interface/cIcon.h"
#include "ItemManager.h"
#include "ChatManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPKLootingDialog::CPKLootingDialog()
{
	m_type = WT_PKLOOTINGDLG;
	m_dwDiePlayerIdx = 0;

	m_pStcBadFame	= NULL;
	m_pStcTime		= NULL;
	m_pStcChance	= NULL;
	m_pStcTarget	= NULL;
	m_pStcItem		= NULL;
	m_pIGDItem		= NULL;
	m_pStcEnd		= NULL;
	m_pStcNone		= NULL;

	m_bMsgSync		= FALSE;
}

CPKLootingDialog::~CPKLootingDialog()
{

}

void CPKLootingDialog::Linking()
{
	m_pStcBadFame	= (cStatic*)GetWindowForID( PLI_STC_BADFAME );
	m_pStcTime		= (cStatic*)GetWindowForID( PLI_STC_TIME );
	m_pStcChance	= (cStatic*)GetWindowForID( PLI_STC_CHANCE );
	m_pStcTarget	= (cStatic*)GetWindowForID( PLI_STC_TARGETNAME );
	m_pStcItem		= (cStatic*)GetWindowForID( PLI_STC_ITEM );
	m_pStcEnd		= (cStatic*)GetWindowForID( PLI_STC_END );
	m_pStcNone		= (cStatic*)GetWindowForID( PLI_STC_NONE );
	m_pIGDItem		= (cIconGridDialog*)GetWindowForID( PLI_IGD_ITEM );
}

void CPKLootingDialog::InitPKLootDlg( DWORD dwID, LONG x, LONG y, DWORD dwDiePlayerIdx )
{
	SetID( dwID );
	SetAbsXY( x, y );

	m_dwDiePlayerIdx = dwDiePlayerIdx;
	m_bLootingEnd	= FALSE;
//	m_dwStartTime	= gCurTime;
	m_nTime			= PKLOOTING_LIMIT_TIME / 1000;
	m_pStcBadFame->SetStaticValue( HERO->GetBadFame() );
	m_pStcTime->SetStaticValue( m_nTime );
	m_pStcTime->SetFGColor( RGBA_MAKE(255, 255, 0, 0) );
	m_nChance = PKMGR->GetLootingChance( HERO->GetBadFame() );
	m_pStcChance->SetStaticValue( m_nChance );
	m_pStcChance->SetFGColor( RGBA_MAKE(0, 255, 60, 0) );
	m_nLootItemNum = PKMGR->GetLootingItemNum( HERO->GetBadFame() );
	m_pStcItem->SetStaticValue( m_nLootItemNum );

	CPlayer* pTargetPlayer = (CPlayer*)OBJECTMGR->GetObject( dwDiePlayerIdx );
	if( pTargetPlayer )
		m_pStcTarget->SetStaticText( pTargetPlayer->GetObjectName() );

	ZeroMemory( m_bSelected, sizeof( m_bSelected ) );

//아이콘만들기

	cImage	Image;	
	SCRIPTMGR->GetImage( 90, &Image );

	for( int i = 0 ; i < PKLOOTING_ITEM_NUM ; ++i )
	{
		cIcon* pIcon = new cIcon;
		pIcon->Init( 0, 0, 40, 40, &Image, PKMGR->AllocWindowId() );
		pIcon->SetMovable( FALSE );
		WINDOWMGR->AddWindow( pIcon );
		m_pIGDItem->AddIcon( i, pIcon );
	}

	m_dwCreateTime	= gCurTime;
	m_bShow			= FALSE;
}

void CPKLootingDialog::ReleaseAllIcon()
{
	for( int i = 0 ; i < PKLOOTING_ITEM_NUM ; ++i )
	{
		cIcon* pIcon;
		if( m_pIGDItem->DeleteIcon( i, &pIcon ) )
		{
			PKMGR->FreeWindowId( pIcon->GetID() );
			WINDOWMGR->AddListDestroyWindow( pIcon );
		}
	}
}

void CPKLootingDialog::ChangeIconImage( WORD pos, int nKind, WORD ItemIdx )
{
	cImage Image;
	switch( nKind )
	{
	case eLIK_ITEM:
		{
			ITEMMGR->GetIconImage( ItemIdx, &Image );
		}
		break;
	case eLIK_MONEY:
		{
			SCRIPTMGR->GetImage( 89, &Image );
		}
		break;
	case eLIK_EXP:
		{
			SCRIPTMGR->GetImage( 88, &Image );
		}
		break;
	case eLIK_NONE:
		{
			SCRIPTMGR->GetImage( 91, &Image );
		}
		break;
	}
	cIcon* pIcon =  m_pIGDItem->GetIconForIdx( pos );
	pIcon->SetBasicImage( &Image );
}



DWORD CPKLootingDialog::ActionEvent( CMouse * mouseInfo )
{
	DWORD we = WE_NULL;
	if( m_bDisable ) return we;

	if( !m_bShow )
	{
		if( gCurTime - m_dwCreateTime >= PKLOOTING_DLG_DELAY_TIME )
		{
			m_bShow = TRUE;
			m_dwStartTime = gCurTime;
		}
		else
			return we;
	}

	we = cDialog::ActionEvent( mouseInfo );

	int nTime = ( PKLOOTING_LIMIT_TIME - (gCurTime - m_dwStartTime) ) / 1000;
	
	if( nTime != m_nTime )
	{
		--m_nTime;
		if( m_nTime > 0 )
		{
			m_pStcTime->SetStaticValue( m_nTime );
		}
		else
		{
			//루팅끝
			PKMGR->CloseLootingDialog( this, TRUE );
		}
	}
	
	return we;
}

void CPKLootingDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if( we == WE_CLOSEWINDOW )
	{
		PKMGR->CloseLootingDialog( this, TRUE );
	}
	else if( lId == PLI_BTN_CLOSE && we == WE_BTNCLICK )
	{
		PKMGR->CloseLootingDialog( this, TRUE );
	}
	else if( lId == PLI_IGD_ITEM && we == WE_LBTNCLICK )
	{
		if( IsLootingEnd() ) return;
		if( m_nChance <= 0 ) return;		//기회를 다 사용했다면
		if( m_nLootItemNum <= 0 ) return;	//더이상 획득할 수 없다면
		if( m_bMsgSync ) return;			//메세지 처리중이면

		LONG idx = m_pIGDItem->GetCurSelCellPos();
		if( m_bSelected[idx] )	//이미 선택했던 것이라면
			return;

		//거리체크하자
		CPlayer* pDiePlayer = (CPlayer*)OBJECTMGR->GetObject( m_dwDiePlayerIdx );
		VECTOR3 pos1, pos2;
		pDiePlayer->GetPosition( &pos1 );
		HERO->GetPosition( &pos2 );
		float fDist = CalcDistanceXZ( &pos1, &pos2 );
		if( fDist > PK_LOOTING_DISTANCE )
		{
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 561 ) );
			return;
		}		

		m_bSelected[idx] = TRUE;
		--m_nChance;
		m_pStcChance->SetStaticValue( m_nChance );
		//클라이언트에서 기회 하나 줄이고
		if( m_nChance <= 0 )
		{
			//m_pIGDItem->SetCurSelCellPos( -1 );
			m_pIGDItem->SetDisable( TRUE );
			m_bLootingEnd = TRUE;
			m_nLootItemNum = 0;
			m_pStcEnd->SetStaticText( CHATMGR->GetChatMsg(574) );
			m_pStcChance->SetStaticText( "" );
			m_pStcItem->SetStaticText( "" );
			m_pStcNone->SetStaticText( "" );
		}

//서버로 msg보내기
		MSG_DWORD2 msg;
		msg.Category	= MP_PK;
		msg.Protocol	= MP_PK_LOOTING_SELECT_SYN;
		msg.dwObjectID	= HEROID;
		msg.dwData1		= m_dwDiePlayerIdx;
		msg.dwData2		= idx;
		NETWORK->Send( &msg, sizeof(msg) );
		//여기서부터 어떤 메세지가 오기전까지 인벤토리등 금지사항 막기 //PKLOOT
		SetMsgSync( TRUE );
	}
}

void CPKLootingDialog::AddLootingItemNum()
{
	if( m_nLootItemNum <= 0 )
		return;
	if( --m_nLootItemNum <= 0 )
	{
		m_pIGDItem->SetDisable( TRUE );
		m_nChance = 0;
		m_bLootingEnd = TRUE;
		m_pStcEnd->SetStaticText( CHATMGR->GetChatMsg(574) );
		m_pStcChance->SetStaticText( "" );
		m_pStcItem->SetStaticText( "" );
		m_pStcNone->SetStaticText( "" );
	}
	else
	{
		m_pStcItem->SetStaticValue( m_nLootItemNum );
	}
}

void CPKLootingDialog::Render()
{
	if( m_bShow )
	{
		cDialog::Render();
	}
}

void CPKLootingDialog::SetMsgSync( BOOL bSync )
{
	m_bMsgSync = bSync;
	
	ITEMMGR->SetDisableDialog(m_bMsgSync, eItemTable_Inventory);
	ITEMMGR->SetDisableDialog(m_bMsgSync, eItemTable_Pyoguk);
	ITEMMGR->SetDisableDialog(m_bMsgSync, eItemTable_MunpaWarehouse);
}