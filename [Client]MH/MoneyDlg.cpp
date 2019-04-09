// MoneyDlg.cpp: implementation of the CMoneyDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MoneyDlg.h"
#include "./Interface/cSpin.h"
#include "WindowIDEnum.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMoneyDlg::CMoneyDlg()
{
	m_type = WT_MONEYDIALOG;
	memset(m_SavedMsg,0,sizeof(1024));
}

CMoneyDlg::~CMoneyDlg()
{

}

void CMoneyDlg::Show(void* pmsg,int msglen,DWORD dwParam,BOOL (*OnPushFunc)(DWORD Money,DWORD dwParam))
{
	m_MsgLen = msglen;
	SetActive(TRUE);
	SetFocus(TRUE);
	m_dwParam = dwParam;
	m_OnPushFunc = OnPushFunc;

	if(m_MsgLen == 0 || pmsg == NULL)
		return;
	memcpy(m_SavedMsg,pmsg,msglen);
}
void CMoneyDlg::OkPushed()
{	
	cSpin* pSpin = (cSpin*)GetWindowForID(CMI_MONEYSPIN);
	ASSERT(pSpin);
	int money = atoi(pSpin->GetEditText());

	BOOL bSend = TRUE;
	if(m_OnPushFunc)
		bSend = m_OnPushFunc(money,m_dwParam);

	if(bSend)
	{
		NETWORK->Send((MSGBASE*)m_SavedMsg,m_MsgLen);
	}
	SetActive(FALSE);
}
