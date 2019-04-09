// FriendDialog.cpp: implementation of the CFriendDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FriendDialog.h"
#include "WindowIDEnum.h"
#include ".\Interface\cWindowManager.h"
#include "FriendManager.h"
#include "Gamein.h"
#include "MainBarDialog.h"
#include "MininoteDialog.h"
#include "cRitemEx.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFriendDialog::CFriendDialog()
{
	m_type = WT_FRIENDDLG;
	m_SelectedFriendPge = 1;
	m_LogInGroup = 0;
	m_LogOutGroup = 0;
//	strcpy(m_SelectedFriendName, "");
}

CFriendDialog::~CFriendDialog()
{

}

void CFriendDialog::Init(LONG x, LONG y, WORD wid, WORD hei, cImage * basicImage, LONG ID)
{
	cDialog::Init(x,y,wid,hei,basicImage,ID);
	m_type = WT_FRIENDDLG;
}

void CFriendDialog::Linking()
{
	m_pFriendListLCtl = (cListCtrl*)GetWindowForID(FRI_FRIENDLISTLCTL);
	for(int i=0; i<MAX_FRIEND_PAGE; ++i)
	{
		m_FriendPageBtn[i] = (cButton*)GetWindowForID(FRI_PAGE1BTN+i);
	}
	m_AddFriendBtn = (cButton*)GetWindowForID(FRI_ADDFRIENDBTN);
	m_DelFriendBtn = (cButton*)GetWindowForID(FRI_DELFRIENDBTN);
	for(i=0; i<MAX_FRIEND_LIST; ++i)
	{
		m_pFriendChkBox[i] = (cCheckBox*)GetWindowForID(FRI_FRIENDLISTCHK1+i);
	}
	
	m_SendNoteBtn = (cButton*)GetWindowForID(FRI_SENDNOTEBTN);
	m_SendWhisperBtn = (cButton*)GetWindowForID(FRI_SENDWHISPERBTN);
}

void CFriendDialog::SetActive( BOOL val )
{
	cDialog::SetActive( val );

//--- main bar icon
	CMainBarDialog* pDlg = GAMEIN->GetMainInterfaceDialog();
	if( pDlg )
		pDlg->SetPushBarIcon( OPT_FRIENDDLGICON, m_bActive );
}

DWORD CFriendDialog::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive ) return we;
	we = cDialog::ActionEvent(mouseInfo);
	
	if(we & WE_ROWCLICK)
	{
		cRITEMEx * pRItem = (cRITEMEx *)m_pFriendListLCtl->GetRItem(m_pFriendListLCtl->GetSelectedRowIdx());
		if(pRItem != NULL)
		{
			if(pRItem->dwID) //자동 쪽지 기능 추가.
			{
				GAMEIN->GetMiniNoteDialog()->SetActive(TRUE);
				GAMEIN->GetMiniNoteDialog()->SetMiniNote(pRItem->pString[0], "", 0);
				GAMEIN->GetMiniNoteDialog()->ShowMiniNoteMode(eMinNoteMode_Write);
			}
		}
	}
	return we;
}
/*
void CFriendDialog::SetFriendList(MSG_FRIEND_LIST_DLG* pmsg)
{
	m_LogInGroup = 0;
	m_LogOutGroup = 0;

	ShowFriendPageBtn(pmsg->TotalPage);

	m_pFriendListLCtl->DeleteAllItems();
	
	for(int i=0; i<MAX_FRIEND_LIST; ++i)
	{
		m_pFriendChkBox[i]->SetChecked(0);
		m_pFriendChkBox[i]->SetActive(TRUE);
		
		if(pmsg->FriendList[i].Id != 0)
		{
			cRITEMEx *ritem = new cRITEMEx(1);
			strcpy(ritem->pString[0], pmsg->FriendList[i].Name);
			if(pmsg->FriendList[i].IsLoggIn)
			{
				ritem->rgb[0] = RGBA_MAKE(157,215,206,255);
			}
			else
			{
				ritem->rgb[0] = RGBA_MAKE(180,180,180,255);
			}
			ritem->dwID = pmsg->FriendList[i].Id;
			m_pFriendListLCtl->InsertItem(i, ritem);
		}
		else
			m_pFriendChkBox[i]->SetActive(FALSE);
	}
}
*/

void CFriendDialog::SetFriendList(int gotopage)
{
	m_LogInGroup = 0;
	m_LogOutGroup = 0;
	
	FRIEND_LIST FriendInfo;
	memset(&FriendInfo, 0, sizeof(FRIEND_LIST));
	FRIENDMGR->GetFriendListInfo(&FriendInfo, gotopage);
	
	ShowFriendPageBtn(FriendInfo.totalnum);

	m_pFriendListLCtl->DeleteAllItems();
	
	for(int i=0; i<MAX_FRIEND_LIST; ++i)
	{
		m_pFriendChkBox[i]->SetChecked(0);
		m_pFriendChkBox[i]->SetActive(TRUE);
		
		if(FriendInfo.Friend[i].Id != 0)
		{
			cRITEMEx *ritem = new cRITEMEx(1);
			strcpy(ritem->pString[0], FriendInfo.Friend[i].Name);
			if(FriendInfo.Friend[i].IsLoggIn)
			{
				ritem->rgb[0] = RGBA_MAKE(157,215,206,255);
			}
			else
			{
				ritem->rgb[0] = RGBA_MAKE(180,180,180,255);
			}
			ritem->dwID = FriendInfo.Friend[i].Id;
			m_pFriendListLCtl->InsertItem(i, ritem);
		}
		else
			m_pFriendChkBox[i]->SetActive(FALSE);
	}
}

void CFriendDialog::ShowFriendPageBtn(BYTE TotalPage)
{
	if(TotalPage == 0)
	{
		for(BYTE i=0; i<MAX_FRIEND_PAGE; ++i)
			m_FriendPageBtn[i]->SetActive(FALSE);
		return;
	}

	for(DWORD i=0; i<MAX_FRIEND_PAGE; ++i)
	{
		m_FriendPageBtn[i]->SetTextValue(i+1);
		m_FriendPageBtn[i]->SetActive(i+1 <= TotalPage);
	}
}


/*
void CFriendDialog::UpdateLogIn(char* LogInName)
{
	for(int i=0; i<MAX_FRIEND_LIST; ++i)
	{
		if(strcmp(m_FriendList[i].Name, LogInName) == 0)
		{
			m_FriendList[i].IsLoggIn = TRUE;
			RefreshDlg();
			return;
		}
	}
}

void CFriendDialog::UpdateLogOut(char* LogOutName)
{
	for(int i=0; i<MAX_FRIEND_LIST; ++i)
	{
		if(strcmp(m_FriendList[i].Name, LogOutName) == 0)
		{
			m_FriendList[i].IsLoggIn = FALSE;
			RefreshDlg();
			return;
		}
	}
}
*/


/*
void CFriendDialog::RefreshDlg()
{
	if(m_bDlgActive == TRUE)
	{
		m_pFriendListLCtl->DeleteAllItems();
		
		for(int i=0; i<MAX_FRIEND_LIST; ++i)
		{
			if(m_FriendList[i].Id != 0)
			{
				RITEMExf *ritem = new RITEMExf(2);
				
				strcpy(ritem->pString[0], m_FriendList[i].Name);
				if(m_FriendList[i].IsLoggIn)
				{
					if(m_LogInGroup == FALSE)
					{
						strcpy(ritem->pString[1], "온라인");
						m_LogInGroup = TRUE;
					}
					else
						strcpy(ritem->pString[1], "");
				}
				else
				{
					if(m_LogOutGroup == FALSE)
					{
						strcpy(ritem->pString[1], "오프라인");
						m_LogOutGroup = TRUE;
					}
					else
						strcpy(ritem->pString[1], "");
					
					for(int n=0; n<2; ++i)
						ritem->rgb[n] = RGBA_MAKE(150,150,150,255);
				}
				ritem->dwID = m_FriendList[i].Id; 
				m_pFriendListLCtl->InsertItem(i, ritem);
			}
			
			ShowFriendPageBtn(m_TotalPage);
		}
	}
}
*/


void CFriendDialog::CheckedDelFriend()
{
	DWORD IDs[MAX_FRIEND_LIST];
	cCheckBox* pChkArray[MAX_FRIEND_LIST];
	memset(IDs,0,sizeof(DWORD)*MAX_FRIEND_LIST);

	for(int i=0; i<MAX_FRIEND_LIST; ++i)
	{
		pChkArray[i] = (cCheckBox*)GetWindowForID(FRI_FRIENDLISTCHK1+i);	
		if(pChkArray[i]->IsChecked())
		{
			cRITEMEx * rItem = (cRITEMEx *)m_pFriendListLCtl->GetRItem(i);
			if(rItem->dwID)
			{
				IDs[i] = rItem->dwID;
			}
		}
	}
	
	int lastidx = MAX_FRIEND_LIST;
	for(i=0;i<MAX_FRIEND_LIST;++i)
	{
		if(IDs[i] != 0)
			lastidx = i;
	}

	for(i=0;i<MAX_FRIEND_LIST;++i)
	{
		if(IDs[i] != 0)
		{
			BOOL bLast;
			if(i==lastidx)
				bLast = TRUE;
			else
				bLast = FALSE;
			FRIENDMGR->DelFriendSynbyID(IDs[i],bLast);
		}
	}
}

char* CFriendDialog::GetChkedName()
{
	cCheckBox* pChkArray[MAX_FRIEND_LIST];
	for(int i=0; i<MAX_FRIEND_LIST; ++i)
	{
		pChkArray[i] = (cCheckBox*)GetWindowForID(FRI_FRIENDLISTCHK1+i);	
		if(pChkArray[i]->IsChecked())
		{
			cRITEMEx * rItem = (cRITEMEx *)m_pFriendListLCtl->GetRItem(i);
			return rItem->pString[0];
		}
	}
	return "";
}