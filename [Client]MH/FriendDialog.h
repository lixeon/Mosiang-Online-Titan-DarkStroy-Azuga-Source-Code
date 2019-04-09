// FriendDialog.h: interface for the CFriendDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRIENDDIALOG_H__9B871F1D_D035_41A6_9BD5_31F24628CDA8__INCLUDED_)
#define AFX_FRIENDDIALOG_H__9B871F1D_D035_41A6_9BD5_31F24628CDA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INTERFACE\cDialog.h"
#include ".\interface\cWindowHeader.h"

class CFriendDialog : public cDialog  
{
	cListCtrl * m_pFriendListLCtl;
	cButton* m_FriendPageBtn[MAX_FRIEND_PAGE];
	cButton* m_AddFriendBtn;
	cButton* m_DelFriendBtn;
	cButton* m_SendNoteBtn;
	cButton* m_SendWhisperBtn;
	cCheckBox* m_pFriendChkBox[MAX_FRIEND_LIST];
	
	WORD m_SelectedFriendPge;
		
	BOOL m_LogInGroup;
	BOOL m_LogOutGroup;

private:
//	CFriend* m_Friend;
	
public:
	CFriendDialog();
	virtual ~CFriendDialog();

	void Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID=0);
	void Linking();
	virtual void SetActive( BOOL val );

	void SetFriendList(int gotopage);
	void ShowFriendPageBtn(BYTE TotalPage);

	WORD GetSelectedFriendPge() { return m_SelectedFriendPge;	}
	void SetSelectedFriendPge(WORD FPge) { m_SelectedFriendPge = FPge;	}
	
	virtual DWORD ActionEvent(CMouse * mouseInfo);
	void CheckedDelFriend();
	
	char* GetChkedName();

//	char* GetSelectedFriendName() { return m_SelectedFriendName;	}
};

#endif // !defined(AFX_FRIENDDIALOG_H__9B871F1D_D035_41A6_9BD5_31F24628CDA8__INCLUDED_)
