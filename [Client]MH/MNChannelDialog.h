// MNChannelDialog.h: interface for the CMNChannelDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MNCHANNELDIALOG_H__B8C97CC2_8098_4A03_860C_7AD9EF952AFA__INCLUDED_)
#define AFX_MNCHANNELDIALOG_H__B8C97CC2_8098_4A03_860C_7AD9EF952AFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "./interface/cDialog.h"


class cListDialog;
class cButton;
class cPushupButton;
class cEditBox;
class CMNPlayer;
class cListCtrlEx;


class CMNChannelDialog : public cDialog
{
protected:

//	cListCtrlEx*	m_pListCtrl;
	cListDialog*	m_pListDlg[eCNL_MODE_MAX];
	cPushupButton*	m_pBtnList[eCNL_MODE_MAX];

	cButton*		m_pBtnJoin;

	cEditBox*		m_pEdtChat;
	cListDialog*	m_pLstChat;

	int				m_nChannelMode;

public:
	CMNChannelDialog();
	virtual ~CMNChannelDialog();

	void Linking();
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	void OnActionEvent(LONG lId, void * p, DWORD we);

	void SetChannelMode( int nChannelMode );

	void SetChannelInfo( CHANNEL_BASEINFO* pChannelInfo );

	void AddPlayer( MNPLAYER_BASEINFO* pPlayerInfo );
	void RemovePlayer( MNPLAYER_BASEINFO* pPlayerInfo );
	void RemoveAllPlayer();
	
	void AddChannel( CHANNEL_BASEINFO* pChannelInfo );
	void RemoveChannel( CHANNEL_BASEINFO* pChannelInfo );
	void RemoveAllChannel();

	void AddPlayRoom( PLAYROOM_BASEINFO* pPlayRoomInfo );
	void RemovePlayRoom( PLAYROOM_BASEINFO* pPlayRoomInfo );
	void RemoveAllPlayRoom();

	void ChatMsg( int nClass, CMNPlayer* pMNPlayer, char* strMsg );
	

};

#endif // !defined(AFX_MNCHANNELDIALOG_H__B8C97CC2_8098_4A03_860C_7AD9EF952AFA__INCLUDED_)
