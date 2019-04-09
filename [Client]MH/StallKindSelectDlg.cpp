// StallKindSelectDlg.cpp: implementation of the CStallKindSelectDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "StallKindSelectDlg.h"
#include "StreetStallManager.h"
#include "StreetBuyStall.h"
#include "GameIn.h"
#include "WindowIDEnum.h"
#include "./Interface\cButton.h"
#include "ChatManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStallKindSelectDlg::CStallKindSelectDlg()
{
	m_pSellBtn = NULL;
	m_pBuyBtn = NULL;
	m_pCancelBtn = NULL;
}

CStallKindSelectDlg::~CStallKindSelectDlg()
{

}

void CStallKindSelectDlg::Linking()
{
	m_pSellBtn		= (cButton*)GetWindowForID(SO_SELLBTN);
	m_pBuyBtn		= (cButton*)GetWindowForID(SO_BUYBTN);
	m_pCancelBtn	= (cButton*)GetWindowForID(SO_CANCELBTN);
}

void CStallKindSelectDlg::OnActionEvnet(LONG lId, void * p, DWORD we)
{
	if( we & WE_BTNCLICK && lId == SO_SELLBTN )
	{
		STREETSTALLMGR->SetStallKind(eSK_SELL);
		STREETSTALLMGR->OpenStreetStall();
	}
	else if( we & WE_BTNCLICK && lId ==SO_BUYBTN)
	{
		STREETSTALLMGR->SetStallKind(eSK_BUY);
		STREETSTALLMGR->OpenStreetStall();

//		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(439) );
	}
	else if( we & WE_BTNCLICK && lId == SO_CANCELBTN )
	{
		STREETSTALLMGR->SetStallKind(eSK_NULL);
		STREETSTALLMGR->SetOpenMsgBox(TRUE);
	}
	else
		return;

	Close();
}

void CStallKindSelectDlg::Show()
{
	SetActive(TRUE);

	m_pSellBtn->SetActive(TRUE);
	m_pBuyBtn->SetActive(TRUE);
	m_pCancelBtn->SetActive(TRUE);
}

void CStallKindSelectDlg::Close()
{
	SetActive(FALSE);

	m_pSellBtn->SetActive(FALSE);
	m_pBuyBtn->SetActive(FALSE);
	m_pCancelBtn->SetActive(FALSE);
}