// MNPlayRoomDialog.h: interface for the CMNPlayRoomDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MNPLAYROOMDIALOG_H__41169AA8_CAC1_4253_9AAF_A07EF6CE7377__INCLUDED_)
#define AFX_MNPLAYROOMDIALOG_H__41169AA8_CAC1_4253_9AAF_A07EF6CE7377__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "./interface/cDialog.h"


class cListDialog;
class cButton;
class cEditBox;
class cListDialog;



class CMNPlayRoomDialog : public cDialog
{
protected:

	cListDialog*	m_pPlayerListDlg[eTEAM_MAX];
	cButton*		m_pBtnStart;
	cEditBox*		m_pEdtChat;
	cListDialog*	m_pLstChat;
	
public:

	CMNPlayRoomDialog();
	virtual ~CMNPlayRoomDialog();
	
	void Linking();
	void OnActionEvent(LONG lId, void * p, DWORD we);
	virtual DWORD ActionEvent(CMouse * mouseInfo);

	void AddPlayer( MNPLAYER_BASEINFO* pPlayerInfo );
	void RemovePlayer( MNPLAYER_BASEINFO* pPlayerInfo );
	void RemoveAllPlayer();
	
	void SendMsgTeamChange( BYTE cbTeam );
	void TeamChange( char* strName, BYTE cbFromTeam, BYTE cbToTeam );
	void SendMsgExit();
	void SendMsgStart();

	void SetCaptain( BOOL bCaptain );

	void SetPlayRoomInfo( PLAYROOM_BASEINFO* pPlayRoomInfo );

	void ChatMsg( int nClass, char* strName, char* strMsg );
	void PrintMsg( int nClass, char* str );

};

#endif // !defined(AFX_MNPLAYROOMDIALOG_H__41169AA8_CAC1_4253_9AAF_A07EF6CE7377__INCLUDED_)
