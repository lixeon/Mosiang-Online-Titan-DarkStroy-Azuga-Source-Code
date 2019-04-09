// MNChannelDialog.cpp: implementation of the CMNChannelDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MNChannelDialog.h"
#include "WindowIdEnum.h"
#include "./Interface/cStatic.h"
#include "./Interface/cEditBox.h"
#include "./Interface/cListDialog.h"
#include "./Interface/cPushupButton.h"
#include "./Interface/cListCtrlEx.h"

#include "GlobalEventFunc.h"
#include "MurimNet.h"
#include "MNStgChannel.h"
#include "MNPlayer.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMNChannelDialog::CMNChannelDialog()
{
	//ÃÊ±âÈ­ºÎºÐÀ» µû·Î ¸¸µéÀÚ! CONFIRM
	m_nChannelMode = eCNL_MODE_ID;
}

CMNChannelDialog::~CMNChannelDialog()
{

}

void CMNChannelDialog::Linking()
{
//	m_pListCtrl	= (cListCtrlEx*)GetWindowForID( MNCNL_LSTDLG_IDLIST );
	m_pListDlg[eCNL_MODE_ID]		= (cListDialog*)GetWindowForID( MNCNL_LSTDLG_IDLIST );
	m_pListDlg[eCNL_MODE_CHANNEL]	= (cListDialog*)GetWindowForID( MNCNL_LSTDLG_CHANNELLIST );
	m_pListDlg[eCNL_MODE_PLAYROOM]	= (cListDialog*)GetWindowForID( MNCNL_LSTDLG_PLAYROOMLIST );

	m_pBtnList[eCNL_MODE_ID]		= (cPushupButton*)GetWindowForID( MNCNL_BTN_IDLIST );
	m_pBtnList[eCNL_MODE_CHANNEL]	= (cPushupButton*)GetWindowForID( MNCNL_BTN_CHANNELLIST );
	m_pBtnList[eCNL_MODE_PLAYROOM]	= (cPushupButton*)GetWindowForID( MNCNL_BTN_PLAYROOMLIST );

	m_pBtnJoin						= (cButton*)GetWindowForID( MNCNL_BTN_JOIN );
	
	m_pEdtChat = (cEditBox*)GetWindowForID( MNCNL_EB_CHAT );
	m_pEdtChat->SetEditFunc( MNCNL_ChatFunc );

	m_pLstChat = (cListDialog*)GetWindowForID( MNCNL_LSTDLG_CHAT );
}

DWORD CMNChannelDialog::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = cDialog::ActionEvent( mouseInfo );

	if( m_pEdtChat )
	if( !m_pEdtChat->IsFocus() )
		m_pEdtChat->SetFocusEdit( TRUE );

	return we;
}

void CMNChannelDialog::OnActionEvent(LONG lId, void * p, DWORD we)
{
	if( we == WE_PUSHDOWN )
	{
		int nConvertID = ( lId - MNCNL_BTN_IDLIST );

		if( nConvertID >= eCNL_MODE_ID && nConvertID <= eCNL_MODE_PLAYROOM )
		{
			CMNStgChannel* pStgChannel = (CMNStgChannel*)MURIMNET->GetMNStage( MNSTG_CHANNEL );
			pStgChannel->SetChannelMode( nConvertID );
		}
	}
	else if( we == WE_BTNCLICK )
	{
		if( lId == MNCNL_BTN_JOIN )
		{
			if( m_nChannelMode == eCNL_MODE_PLAYROOM )
			{
				//msgsend
			}
		}
	}
}

void CMNChannelDialog::SetChannelInfo( CHANNEL_BASEINFO* pChannelInfo )
{
	cStatic* pTitle = (cStatic*)GetWindowForID( MNCNL_STC_TITLE );
	pTitle->SetStaticText( pChannelInfo->strChannelTitle );
}

void CMNChannelDialog::AddPlayer( MNPLAYER_BASEINFO* pPlayerInfo )
{
	wsprintf( gStrTemp128, "%-50s [Level:%3d]", pPlayerInfo->strPlayerName, pPlayerInfo->Level );

	m_pListDlg[eCNL_MODE_ID]->AddItem( gStrTemp128, 0xffffffff );

//	cLCItemBase* pItem = NULL;
//	m_pListCtrl->CreateNewItem( &pItem );
//	m_pListCtrl->AddTail()

}

void CMNChannelDialog::RemovePlayer( MNPLAYER_BASEINFO* pPlayerInfo )
{
	//ÀÓ½Ã
	wsprintf( gStrTemp128, "%-50s [Level:%3d]", pPlayerInfo->strPlayerName, pPlayerInfo->Level );

	m_pListDlg[eCNL_MODE_ID]->RemoveItem( gStrTemp128 );
}

void CMNChannelDialog::RemoveAllPlayer()
{
	m_pListDlg[eCNL_MODE_ID]->RemoveAll();
}

void CMNChannelDialog::AddChannel( CHANNEL_BASEINFO* pChannelInfo )
{
	wsprintf( gStrTemp128, "%-54s (%3d/%3d)", pChannelInfo->strChannelTitle,
				pChannelInfo->wPlayerNum, pChannelInfo->wMaxPlayer );

	m_pListDlg[eCNL_MODE_CHANNEL]->AddItem( gStrTemp128, 0xffffffff );
}

void CMNChannelDialog::RemoveChannel( CHANNEL_BASEINFO* pChannelInfo )
{
	//¼öÁ¤ÇØ¾ßÇÑ´Ù..
	m_pListDlg[eCNL_MODE_CHANNEL]->RemoveItem( pChannelInfo->strChannelTitle );
}

void CMNChannelDialog::RemoveAllChannel()
{
	m_pListDlg[eCNL_MODE_CHANNEL]->RemoveAll();
}

void CMNChannelDialog::SetChannelMode( int nChannelMode )
{
	m_pListDlg[nChannelMode]->SetActive( TRUE );
	m_pBtnList[nChannelMode]->SetPush( TRUE );

	for( int i = 0 ; i < eCNL_MODE_MAX ; ++i )
	{
		if( i != nChannelMode )
		{
			m_pListDlg[i]->SetActive( FALSE );
			m_pBtnList[nChannelMode]->SetPush( FALSE );
		}
	}

	m_nChannelMode = nChannelMode;

//---Channel°ú PlayRoomÀÇ ¸®½ºÆ®¸¦ ÃÊ±âÈ­ ÇÑ´Ù. Id¸®½ºÆ®´Â ½Ç½Ã°£À¸·Î ¾÷µ¥ÀÌÆ®ÇÏ¹Ç·Î ÃÊ±âÈ­ÇÏÁö ¾Ê´Â´Ù.
	RemoveAllChannel();
	RemoveAllPlayRoom();
}

void CMNChannelDialog::AddPlayRoom( PLAYROOM_BASEINFO* pPlayRoomInfo )
{
	m_pListDlg[eCNL_MODE_PLAYROOM]->AddItem( pPlayRoomInfo->strPlayRoomTitle, 0xffffffff );
}

void CMNChannelDialog::RemovePlayRoom( PLAYROOM_BASEINFO* pPlayRoomInfo )
{
	m_pListDlg[eCNL_MODE_PLAYROOM]->RemoveItem( pPlayRoomInfo->strPlayRoomTitle );
}

void CMNChannelDialog::RemoveAllPlayRoom()
{
	m_pListDlg[eCNL_MODE_PLAYROOM]->RemoveAll();
}

void CMNChannelDialog::ChatMsg( int nClass, CMNPlayer* pMNPlayer, char* strMsg )
{
	static char msg[256] = {0,};
	switch( nClass )
	{
	case PRCTC_WHOLE:
		{
			wsprintf( msg, "[%s]: %s", pMNPlayer->GetName(), strMsg );
			m_pLstChat->AddItem( msg, 0xffffffff );
		}
		break;
	}	
}