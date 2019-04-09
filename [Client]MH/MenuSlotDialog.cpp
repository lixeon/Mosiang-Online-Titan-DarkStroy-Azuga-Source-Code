// MenuSlotDialog.cpp: implementation of the CMenuSlotDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MenuSlotDialog.h"
#include "WindowIdEnum.h"
#include "./Interface/cWindowManager.h"
#include "./Interface/cIconGridDialog.h"
#include "MenuIcon.h"
#include "AbilityInfo.h"
#include "AbilityIcon.h"
#include "MenuSlotManager.h"
#include "./Input/Mouse.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMenuSlotDialog::CMenuSlotDialog()
{
	m_nCurPage = 0;
}

CMenuSlotDialog::~CMenuSlotDialog()
{

}

void CMenuSlotDialog::Linking()
{
	//gringdialog¸¦ ¸µÅ©½ÃÅ²´Ù.
	m_pigdSheet[0] = (cIconGridDialog*)GetWindowForID( MSI_PAGESHEET1 );
	m_pigdSheet[1] = (cIconGridDialog*)GetWindowForID( MSI_PAGESHEET2 );
	m_pigdSheet[2] = (cIconGridDialog*)GetWindowForID( MSI_PAGESHEET3 );
	m_pigdSheet[3] = (cIconGridDialog*)GetWindowForID( MSI_PAGESHEET4 );

	for( int i = 0 ; i < TAB_MENUSLOT_NUM ; ++i )
	{
//		m_pigdSheet[i]->SetAcceptableIconType( eIconType_Ability );
		m_pigdSheet[i]->SetActive( !i );	//0¹øÅÇ¸¸ active true
	}
}

void CMenuSlotDialog::Render()
{
	if(!m_bActive) return;

	cDialog::RenderWindow();
	// ÆäÀÌÁö ¹øÈ£ Ãâ·Â and sheet¼û±â±â
	
	cDialog::RenderComponent();


}

DWORD CMenuSlotDialog::ActionEvent( CMouse * mouseInfo )
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we = cDialog::ActionEvent(mouseInfo);

	if( we & WE_LBTNDBLCLICK )
	{
		LONG x = mouseInfo->GetMouseEventX();
		LONG y = mouseInfo->GetMouseEventY();
		POSTYPE pos;
		if( m_pigdSheet[m_nCurPage]->GetPositionForXYRef( x, y, pos ) )
		{
			CMenuIcon* pMenuIcon = (CMenuIcon*)m_pigdSheet[m_nCurPage]->GetIconForIdx( pos );
			if( pMenuIcon )
				MENUSLOTMGR->UseMenuIcon( pMenuIcon );
		}
	}

	return we;
}

void CMenuSlotDialog::OnActionEvent( LONG lId, void* p, DWORD we )
{
	if( we & WE_LBTNCLICK )
	{
		if( lId == MSI_PREVPAGEBTN )
		{
			
		}
		else if( lId == MSI_NEXTPAGEBTN )
		{
			
		}
	}
}

BOOL CMenuSlotDialog::FakeMoveIcon( LONG x, LONG y, cIcon* icon )
{
	WINDOWMGR->BackDragWindow();	//ack ¿Í nack¿¡¼­

	if( icon->GetType() != WT_ABILITYICON && icon->GetType() != WT_MENUICON )
		return FALSE;
	
	WORD wPos;
	if( !m_pigdSheet[m_nCurPage]->GetPositionForXYRef( x, y, wPos ) )
		return FALSE;

	CMenuIcon* pMenuIcon = (CMenuIcon*)m_pigdSheet[m_nCurPage]->GetIconForIdx( wPos );

	if( pMenuIcon )
	{
		if( icon->GetType() == WT_ABILITYICON )
		{
			if( MENUSLOTMGR->DeleteMenuIcon( pMenuIcon ) )
				return MENUSLOTMGR->AddMenuIcon( m_nCurPage, wPos, icon );
			else
				return FALSE;
		}
		else
		{
			return MENUSLOTMGR->ExchangeMenuIcon( pMenuIcon, (CMenuIcon*)icon );
		}
	}
	else
	{
		if( icon->GetType() == WT_ABILITYICON )
			return MENUSLOTMGR->AddMenuIcon( m_nCurPage, wPos, icon );
		else
			return MENUSLOTMGR->MoveMenuIcon( m_nCurPage, wPos, (CMenuIcon*)icon );

	}
}

void CMenuSlotDialog::FakeDeleteIcon( CMenuIcon* pMenuIcon)
{
	MENUSLOTMGR->DeleteMenuIcon( pMenuIcon );
}

BOOL CMenuSlotDialog::AddMenuIcon( WORD wPage, WORD wPos, CMenuIcon* pMenuIcon )
{
	if( wPage >= TAB_MENUSLOT_NUM )
		return FALSE;

	if( m_pigdSheet[wPage]->AddIcon( wPos, 0, pMenuIcon ) )
	{
		pMenuIcon->SetSlotPage( wPage );
		return TRUE;
	}
	else
	{
		return FALSE;
	}	
}

BOOL CMenuSlotDialog::DeleteMenuIcon( CMenuIcon* pMenuIcon )
{
	if( pMenuIcon->GetSlotPage() >= TAB_MENUSLOT_NUM )
		return FALSE;

	return m_pigdSheet[pMenuIcon->GetSlotPage()]->DeleteIcon( (cIcon*)pMenuIcon );
}

BOOL CMenuSlotDialog::MoveMenuIcon( WORD wPage, WORD wPos, CMenuIcon* pMenuIcon )
{
	if( wPage >= TAB_MENUSLOT_NUM || pMenuIcon->GetSlotPage() >= TAB_MENUSLOT_NUM )
		return FALSE;

	if( !m_pigdSheet[pMenuIcon->GetSlotPage()]->DeleteIcon( (cIcon*)pMenuIcon ) )
		return FALSE;

	if( m_pigdSheet[wPage]->AddIcon( wPos, 0, pMenuIcon ) )
	{
		pMenuIcon->SetSlotPage( wPage );
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CMenuSlotDialog::ExchangeMenuIcon( CMenuIcon* pMenuIcon1, CMenuIcon* pMenuIcon2 )
{
	WORD wPage1 = pMenuIcon1->GetSlotPage();
	WORD wPage2 = pMenuIcon2->GetSlotPage();

	if( wPage1 >= TAB_MENUSLOT_NUM || wPage2 >= TAB_MENUSLOT_NUM )
		return FALSE;
	
	WORD wCellX1 = pMenuIcon1->GetCellX();
	WORD wCellY1 = pMenuIcon1->GetCellY();
	WORD wCellX2 = pMenuIcon2->GetCellX();
	WORD wCellY2 = pMenuIcon2->GetCellY();

	m_pigdSheet[wPage1]->DeleteIcon( (cIcon*)pMenuIcon1 );
	m_pigdSheet[wPage2]->DeleteIcon( (cIcon*)pMenuIcon2 );

	if( m_pigdSheet[wPage1]->AddIcon( wCellX1, wCellY1, (cIcon*)pMenuIcon2 ) )
	{
		pMenuIcon2->SetSlotPage( wPage1 );
	}

	if( m_pigdSheet[wPage2]->AddIcon( wCellX2, wCellY2, (cIcon*)pMenuIcon1 ) )
	{
		pMenuIcon1->SetSlotPage( wPage2 );
	}

	return TRUE;	//¿¡·¯Ã¼Å©°¡ Èûµçµ¥.. ÇÏ±ä ÇØ¾ßÇÒÅÙµ¥..
}