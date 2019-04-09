// GuildFieldWarDialog.cpp: implementation of the CGuildFieldWarDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildFieldWarDialog.h"
#include "./Interface/cWindowManager.h"
#include "./interface/cStatic.h"
#include "./interface/cButton.h"
#include "./interface/cEditBox.h"
#include "./interface/cTextArea.h"
#include "./interface/cListDialog.h"
#include "./interface/cCheckBox.h"
#include "../Interface/cResourceManager.h"
#include "../Input/Mouse.h"
#include "./Interface/cScriptManager.h"
#include "WindowIDEnum.h"
#include "ChatManager.h"
#include "GuildFieldWar.h"
#include "GuildUnion.h"
#include "ObjectManager.h"
#include "MHMap.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGFWarDeclareDlg::CGFWarDeclareDlg()
{
	m_type = WT_GUILDFIELDWAR_DECLARE_DLG;
	m_bRender = FALSE;
}

CGFWarDeclareDlg::~CGFWarDeclareDlg()
{
}

void CGFWarDeclareDlg::SetActive( BOOL val )
{
	cDialog::SetActive(val);
	
	m_pEditBox->SetFocusEdit( val );
	m_pEditBox->SetEditText( "" );
	if( val )
	{
		m_pMoneyCheck->SetChecked( FALSE );
		m_pMoneyEdit->SetDisable( TRUE );	
		m_pMoneyEdit->SetEditText( "" );
	}
	m_bRender = val;
}

void CGFWarDeclareDlg::Linking()
{
	m_pMsg = (cTextArea*)GetWindowForID( GFW_DECLARE_MSG );
	m_pEditBox = (cEditBox*)GetWindowForID( GFW_DECLARE_EDITBOX );
	m_pMsg1 = (cTextArea*)GetWindowForID( GFW_MONEY_MSG1 );
	m_pMsg2 = (cTextArea*)GetWindowForID( GFW_MONEY_MSG2 );
	m_pMsg3 = (cTextArea*)GetWindowForID( GFW_MONEY_MSG3 );
	m_pMoneyCheck = (cCheckBox*)GetWindowForID( GFW_MONEY_CHK );
	m_pMoneyEdit = (cEditBox*)GetWindowForID( GFW_MONEY_EDITBOX );
	m_pMoneyStatic = (cStatic*)GetWindowForID( GFW_MONEY_STATIC );
	m_pOkBtn = (cButton*)GetWindowForID( GFW_DECLARE_OKBTN );
	m_pCancelBtn = (cButton*)GetWindowForID( GFW_DECLARE_CANCELBTN );

	SCRIPTMGR->GetImage( 65, &m_LockImage, PFT_HARDPATH );
	
	m_pMsg->SetScriptText( CHATMGR->GetChatMsg( 801 ) );
	m_pMsg1->SetScriptText( CHATMGR->GetChatMsg( 802 ) );
	m_pMsg2->SetScriptText( CHATMGR->GetChatMsg( 803 ) );
	m_pMsg3->SetScriptText( CHATMGR->GetChatMsg( 804 ) );
	m_pMoneyStatic->SetStaticText( CHATMGR->GetChatMsg( 805 ) );

	m_bRender = FALSE;
}

void CGFWarDeclareDlg::ShowMoneyEdit()
{
	if( m_pMoneyCheck->IsChecked() )
	{
		m_pMoneyEdit->SetDisable( FALSE );
		m_pMoneyEdit->SetEditText( "0" );
	}
	else
	{
		m_pMoneyEdit->SetDisable( TRUE );	
		m_pMoneyEdit->SetEditText( "" );
	}
}

void CGFWarDeclareDlg::Render()
{
	cDialog::Render();
	
	if( m_bRender )
	{
		if( !m_LockImage.IsNull() )
		{
			VECTOR2 vPos;
			VECTOR2 vScale;
			vScale.x = 3.35f;	vScale.y = 0.5f;	
			if( !m_pMoneyCheck->IsChecked() )
			{
				vPos.x = this->m_absPos.x + 65;	vPos.y = this->m_absPos.y + 269;			
				m_LockImage.RenderSprite( &vScale, NULL, 0.0f, &vPos,
										  RGBA_MERGE(m_dwImageRGB, m_alpha * m_dwOptionAlpha / 100) );
			}
		}
	}
}

BOOL CGFWarDeclareDlg::IsChecked()
{
	return m_pMoneyCheck->IsChecked();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGFWarResultDlg::CGFWarResultDlg()
{
	m_type = WT_GUILDFIELDWAR_RESULT_DLG;
}

CGFWarResultDlg::~CGFWarResultDlg()
{
}

void CGFWarResultDlg::Linking()
{
	m_pTitle = (cStatic*)GetWindowForID( GFW_RESULT_TITILE );
	m_pMsg = (cTextArea*)GetWindowForID( GFW_RESULT_MSG );
	m_pStatic0 = (cStatic*)GetWindowForID( GFW_RESULT_0 );
	m_pStatic1 = (cStatic*)GetWindowForID( GFW_RESULT_1 );
	m_pStatic2 = (cStatic*)GetWindowForID( GFW_RESULT_2 );
	m_pStatic3 = (cStatic*)GetWindowForID( GFW_RESULT_3 );
	m_pStatic4 = (cStatic*)GetWindowForID( GFW_RESULT_4 );
	m_pStatic5 = (cStatic*)GetWindowForID( GFW_RESULT_5 );
	m_pOkBtn = (cButton*)GetWindowForID( GFW_RESULT_OKBTN );
	m_pCancelBtn = (cButton*)GetWindowForID( GFW_RESULT_CANCELBTN );
	m_pConfirmBtn = (cButton*)GetWindowForID( GFW_RESULT_CONFIRMBTN );
}

void CGFWarResultDlg::ShowDeclare( GUILDINFO* pInfo )
{
	char temp[256] = {0, };
	m_pTitle->SetStaticText( CHATMGR->GetChatMsg( 835 ) );
	m_pMsg->SetScriptText( CHATMGR->GetChatMsg( 806 ) );
	m_pStatic0->SetStaticText( CHATMGR->GetChatMsg( 807 ) );
	sprintf( temp, CHATMGR->GetChatMsg( 808 ), pInfo->GuildName );
	m_pStatic1->SetStaticText( temp );
	// magi82(37) 맵 속성 데이터
	//sprintf( temp, CHATMGR->GetChatMsg( 809 ), GetMapName( pInfo->MapNum ) );
	sprintf( temp, CHATMGR->GetChatMsg( 809 ), MAP->GetMapName( pInfo->MapNum ) );
	m_pStatic2->SetStaticText( temp );
	sprintf( temp, CHATMGR->GetChatMsg( 810 ), pInfo->GuildLevel );
	m_pStatic3->SetStaticText( temp );
	sprintf( temp, CHATMGR->GetChatMsg( 811 ), pInfo->MasterName );
	m_pStatic4->SetStaticText( temp );
	sprintf( temp, CHATMGR->GetChatMsg( 812 ), pInfo->MarkName );
	m_pStatic5->SetStaticText( temp );

	cDialog::SetActive( TRUE );
	m_pStatic3->SetActive( TRUE );
	m_pStatic4->SetActive( TRUE );
	m_pOkBtn->SetActive( TRUE );
	m_pCancelBtn->SetActive( TRUE );
	m_pConfirmBtn->SetActive( FALSE );
}

void CGFWarResultDlg::ShowResult( DWORD dwKind )
{
	cDialog::SetActive( TRUE );
	m_pStatic3->SetActive( FALSE );
	m_pStatic4->SetActive( FALSE );
	m_pOkBtn->SetActive( FALSE );
	m_pCancelBtn->SetActive( FALSE );
	m_pConfirmBtn->SetActive( TRUE );	

	char temp[256] = {0, };
	m_pTitle->SetStaticText( CHATMGR->GetChatMsg( 800 ) );
	switch( dwKind )
	{
	case 0:		// fight
		{
			//임시로 막음
			m_pMsg->SetScriptText( CHATMGR->GetChatMsg( 813 ) );
			m_pStatic0->SetStaticText( CHATMGR->GetChatMsg( 814 ) );
			m_pStatic1->SetStaticText( CHATMGR->GetChatMsg( 815 ) );
//			sprintf( temp, CHATMGR->GetChatMsg( 816 ), 10 );			
//			m_pStatic2->SetStaticText( temp );
			m_pStatic2->SetStaticText( "" );
			//
		}
		break;
	case 1:		// surrend
		{
			//임시로 막음
			m_pMsg->SetScriptText( CHATMGR->GetChatMsg( 821 ) );
//			m_pStatic0->SetStaticText( CHATMGR->GetChatMsg( 814 ) );
			m_pStatic0->SetStaticText( "" );
//			sprintf( temp, CHATMGR->GetChatMsg( 816 ), 5 );			
//			m_pStatic1->SetStaticText( temp );
			m_pStatic1->SetStaticText( "" );
			m_pStatic2->SetStaticText( "" );
			//
		}
		break;
	}
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGFWarInfoDlg::CGFWarInfoDlg()
{
	m_type = WT_GUILDFIELDWAR_INFO_DLG;
	m_nSelectedIdx = -1;
}

CGFWarInfoDlg::~CGFWarInfoDlg()
{
}

DWORD CGFWarInfoDlg::ActionEvent( CMouse* mouseInfo )
{
	DWORD we = WE_NULL;
	if( !m_bActive )	return we;
	we = cDialog::ActionEvent( mouseInfo );	

	if( m_pGuildListDlg->PtIdxInRow(mouseInfo->GetMouseX(),mouseInfo->GetMouseY()) != -1 )
	{
		if( we & WE_LBTNCLICK && (WINDOWMGR->IsMouseDownUsed() == FALSE) )
		{		
			int Idx = m_pGuildListDlg->GetCurSelectedRowIdx();
			if( Idx != -1 )
			{
				m_nSelectedIdx = Idx;
			}
		}
	}
	return we;
}

void CGFWarInfoDlg::Linking()
{
	m_pTitle = (cStatic*)GetWindowForID( GFW_INFO_TITLE );
	m_pMsg = (cStatic*)GetWindowForID( GFW_INFO_MSG );
	m_pGuildListDlg = (cListDialog*)GetWindowForID( GFW_INFO_LIST );
	m_pGuildListDlg->SetShowSelect(TRUE);
	m_pText = (cTextArea*)GetWindowForID( GFW_INFO_TEXT );
	m_pStatic0 = (cStatic*)GetWindowForID( GFW_INFO_STATIC_0 );
	m_pStatic1 = (cStatic*)GetWindowForID( GFW_INFO_STATIC_1 );
	m_pStatic2 = (cStatic*)GetWindowForID( GFW_INFO_STATIC_2 );
	m_pSuggestOkBtn = (cButton*)GetWindowForID( GFW_INFO_SUGGEST_OKBTN );
	m_pSurrendOkBtn = (cButton*)GetWindowForID( GFW_INFO_SURREND_OKBTN );
	m_pGuildUnionRemoveOkBtn = (cButton*)GetWindowForID(GFW_GDU_REMOVE_OKBTN);
	m_pCancelBtn = (cButton*)GetWindowForID( GFW_INFO_CANCELBTN );
}

void CGFWarInfoDlg::ShowSuggest()
{
	cDialog::SetActive( TRUE );

	m_pSuggestOkBtn->SetActive( TRUE );
	m_pSurrendOkBtn->SetActive( FALSE );
	m_pGuildUnionRemoveOkBtn->SetActive( FALSE );

	m_pTitle->SetStaticText( CHATMGR->GetChatMsg( 836 ) );
	m_pMsg->SetStaticText( CHATMGR->GetChatMsg( 829 ) );
	m_pText->SetScriptText( CHATMGR->GetChatMsg( 830 ) );
	m_pStatic0->SetStaticText( "" );
	m_pStatic1->SetStaticText( "" );
	m_pStatic2->SetStaticText( "" );

	AddGuildInfoToList();
}

void CGFWarInfoDlg::ShowSurrend()
{
	cDialog::SetActive( TRUE );
	
	m_pSurrendOkBtn->SetActive( TRUE );
	m_pSuggestOkBtn->SetActive( FALSE );
	m_pGuildUnionRemoveOkBtn->SetActive( FALSE );

	m_pTitle->SetStaticText( CHATMGR->GetChatMsg( 837 ) );
	m_pMsg->SetStaticText( CHATMGR->GetChatMsg( 822 ) );
	m_pText->SetScriptText( CHATMGR->GetChatMsg( 824 ) );
	m_pStatic0->SetStaticText( "" );
	m_pStatic1->SetStaticText( "" );
	m_pStatic2->SetStaticText( "" );

	AddGuildInfoToList();
}

void CGFWarInfoDlg::ShowGuildUnion()
{
	cDialog::SetActive( TRUE );

	m_pSuggestOkBtn->SetActive( FALSE );
	m_pSurrendOkBtn->SetActive( FALSE );
	m_pGuildUnionRemoveOkBtn->SetActive( TRUE );

	m_pTitle->SetStaticText( CHATMGR->GetChatMsg( 1117 ) );
	m_pMsg->SetStaticText( CHATMGR->GetChatMsg( 1118 ) );
	m_pText->SetScriptText( CHATMGR->GetChatMsg( 1119 ) );
	m_pStatic0->SetStaticText( "" );
	m_pStatic1->SetStaticText( "" );
	m_pStatic2->SetStaticText( "" );

	AddGuildUnionInfoToList();
}

void CGFWarInfoDlg::AddGuildInfoToList()
{
	CYHHashTable<GUILDINFO>* pTable = GUILDFIELDWAR->GetEnemyGuildTable();

	m_pGuildListDlg->RemoveAll();

	char temp[256] = {0,};
	char temp1[256] = {0,};
	GUILDINFO* pInfo = NULL;
	pTable->SetPositionHead();
	while( pInfo = pTable->GetData() )
	{
		if( pInfo->MarkName )
			sprintf( temp, "%s	(%d)", pInfo->GuildName, pInfo->MarkName );
		else
			sprintf( temp, "%s", pInfo->GuildName );

		m_pGuildListDlg->AddItem( temp, 0xffffffff );
	}
}
	
void CGFWarInfoDlg::AddGuildUnionInfoToList() 
{
	m_pGuildListDlg->RemoveAll();

	sGUILDIDXNAME* pUnion = GUILDUNION->GetGuildUnionInfo();
	for( int i = 0; i < MAX_GUILD_UNION_NUM; ++i )
	{
		if( pUnion[i].dwGuildIdx && pUnion[i].dwGuildIdx != HERO->GetGuildIdx() )
			m_pGuildListDlg->AddItem( pUnion[i].sGuildName, 0xffffffff );
	}
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CGuildWarInfoDlg::CGuildWarInfoDlg()
{
	m_type = WT_GUILDWAR_INFO_DLG;
}

CGuildWarInfoDlg::~CGuildWarInfoDlg()
{
}

void CGuildWarInfoDlg::Linking()
{
	m_pBlackGuildListDlg = (cListDialog*)GetWindowForID( GW_BLACKLIST );
	m_pWhiteGuildListDlg = (cListDialog*)GetWindowForID( GW_WHITELIST );
	m_pStatic = (cStatic*)GetWindowForID( GW_INFOSTATIC );
	m_pCloseBtn = (cButton*)GetWindowForID( GW_INFOCLOSEBTN );	
}

void CGuildWarInfoDlg::SetActive( BOOL val )
{
	cDialog::SetActive(val);

	if( val )	AddListData();
}

void CGuildWarInfoDlg::AddListData()
{
	char temp[256] = {0,};	

	m_pBlackGuildListDlg->RemoveAll();
	m_pWhiteGuildListDlg->RemoveAll();
	
	CYHHashTable<GUILDINFO>* pTable = GUILDFIELDWAR->GetEnemyGuildTable();
	GUILDINFO* pInfo = NULL;
	pTable->SetPositionHead();
	while( pInfo = pTable->GetData() )
	{
		sprintf( temp, "%s", pInfo->GuildName );
		m_pBlackGuildListDlg->AddItem( temp, 0xffffffff );
	}

	sGUILDIDXNAME* pUnion = GUILDUNION->GetGuildUnionInfo();
	for( int i = 0; i < MAX_GUILD_UNION_NUM; ++i )
	{
		if( pUnion[i].dwGuildIdx && pUnion[i].dwGuildIdx != HERO->GetGuildIdx() )
			m_pWhiteGuildListDlg->AddItem( pUnion[i].sGuildName, 0xffffffff );
	}

	GUILDFIELDWAR->GetVictory( temp );
	m_pStatic->SetStaticText( temp );

	if( m_pBlackGuildListDlg->GetItemCount() == 0 )
	{
		m_pBlackGuildListDlg->AddItem( CHATMGR->GetChatMsg( 851 ), 0xffffffff );
	}
	if( m_pWhiteGuildListDlg->GetItemCount() == 0 )
	{
		m_pWhiteGuildListDlg->AddItem( CHATMGR->GetChatMsg( 852 ), 0xffffffff );
	}
}
