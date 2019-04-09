// GTBattleListDialog.cpp: implementation of the CGTBattleListDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GTBattleListDialog.h"
#include "WindowIDEnum.h"
#include "./interface/cListCtrl.h"
#include "ChatManager.h"
#include "cRitemEx.h"
#include "ptrlist.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"
#include "GameIn.h"
#include "PetManager.h"


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

char GroupInitial[4] = 
{
	'A',
	'B',
	'C',
	'D',
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGTBattleListDialog::CGTBattleListDialog()
{
	m_type = WT_GTBATTLELIST_DLG;
	m_BattleCount = 0;
	m_bPlayOff = FALSE;
	m_nPreSelectedIndex = -1;
}

CGTBattleListDialog::~CGTBattleListDialog()
{
//	DeleteAllBattleInfo();
}


DWORD CGTBattleListDialog::ActionEvent(CMouse* mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive )		return we;
	we = cDialog::ActionEvent(mouseInfo);
	
	cRITEMEx* pRItem = NULL;
	if( we & WE_ROWCLICK )
	{
		int i = m_pBattleListCtrl->GetSelectedRowIdx();
		if( i < (int)m_BattleCount )
		{
			// 글자 색 바꾸기
			pRItem = (cRITEMEx*)m_pBattleListCtrl->GetRItem(i);
			if( m_nPreSelectedIndex != i )
			{
				pRItem->rgb[0] = RGBA_MAKE(255,234,0,255);
				pRItem->rgb[1] = RGBA_MAKE(255,234,0,255);
				
				if( m_nPreSelectedIndex > -1 )
				{
					pRItem = (cRITEMEx*)m_pBattleListCtrl->GetRItem(m_nPreSelectedIndex);
					pRItem->rgb[0] = 0xffffffff;
					pRItem->rgb[1] = 0xffffffff;					
				}
			}

			m_nPreSelectedIndex = i;
		}
	}	

	return we;
}


void CGTBattleListDialog::Linking()
{
	m_pBattleListCtrl = (cListCtrl*)GetWindowForID( GDT_BATTLELIST );
}


void CGTBattleListDialog::SetActive( BOOL val )
{
	cDialog::SetActive( val );

	if( !val )
	{
		if( HERO->GetState() == eObjectState_Deal )
			OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		DeleteAllBattleInfo();
	}
}


void CGTBattleListDialog::RefreshBattleList()
{
	m_pBattleListCtrl->DeleteAllItems();

	GTBATTLE_INFO* pItem = NULL;
	PTRLISTSEARCHSTART(m_BattleList, GTBATTLE_INFO*, pItem);
		cRITEMEx *ritem = new cRITEMEx(2);

		char buf[256];
		if( m_bPlayOff )
			sprintf( buf, CHATMGR->GetChatMsg(953), GroupInitial[pItem->Group] );
		else
			sprintf( buf, CHATMGR->GetChatMsg(954), GroupInitial[pItem->Group] );
		strcpy( ritem->pString[0], buf );

		sprintf( buf, CHATMGR->GetChatMsg(955), pItem->GuildName1, pItem->GuildName2 );
		strcpy( ritem->pString[1], buf );

		ritem->dwID = pItem->BattleID;
		m_pBattleListCtrl->InsertItem( (WORD)m_BattleCount, ritem );
	PTRLISTSEARCHEND
}


void CGTBattleListDialog::AddBattleInfo( GTBATTLE_INFO* pBattleInfo )
{
	if( !pBattleInfo )			return;
	if( strlen(pBattleInfo->GuildName1) == 0 || strlen(pBattleInfo->GuildName2) == 0 )
		return;

	GTBATTLE_INFO* pInfo = new GTBATTLE_INFO;
	memcpy( pInfo, pBattleInfo, sizeof(GTBATTLE_INFO) );
	m_BattleList.AddTail( pInfo );
	++m_BattleCount;
}


void CGTBattleListDialog::DeleteBattleInfo( DWORD BattleID )
{
	GTBATTLE_INFO* pItem = NULL;
	PTRLISTSEARCHSTART(m_BattleList, GTBATTLE_INFO*, pItem);
		if( pItem->BattleID == BattleID )
		{
			m_BattleList.Remove(pItem);
			delete pItem;
			pItem = NULL;
			break;
		}
	PTRLISTSEARCHEND

	RefreshBattleList();
}


void CGTBattleListDialog::DeleteAllBattleInfo()
{
	m_pBattleListCtrl->DeleteAllItems();

	m_bPlayOff = FALSE;
	m_BattleCount = 0;
	m_nPreSelectedIndex = -1;

	GTBATTLE_INFO* pItem = NULL;
	PTRLISTSEARCHSTART(m_BattleList, GTBATTLE_INFO*, pItem);
		m_BattleList.Remove(pItem);
		delete pItem;
		pItem = NULL;
	PTRLISTSEARCHEND
}



BOOL CGTBattleListDialog::EnterBattleonObserverSyn()
{	
	cRITEMEx * pRItem = (cRITEMEx *)m_pBattleListCtrl->GetRItem( m_nPreSelectedIndex );
	if( !pRItem )			return FALSE;

	if(PETMGR->GetCurSummonPet())
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1279));
		return FALSE;
	}
/*	MSG_DWORD msg;
	msg.Category = MP_GTOURNAMENT;
	msg.Protocol = MP_GTOURNAMENT_OBSERVERJOIN_SYN;
	msg.dwData = pRItem->dwID;
	msg.dwObjectID = HEROID;
	NETWORK->Send( &msg, sizeof(msg) );*/

	MSG_DWORD msg;
	msg.Category = MP_GTOURNAMENT;
	msg.Protocol = MP_GTOURNAMENT_MOVETOBATTLEMAP_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = pRItem->dwID;
	NETWORK->Send( &msg, sizeof(msg) );

	GAMEIN->SetBattleIdx( pRItem->dwID );
	GAMEIN->SetGuildIdx( HERO->GetGuildIdx() );

	return TRUE;
}
