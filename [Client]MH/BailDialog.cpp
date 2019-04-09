// BailDialog.cpp: implementation of the CBailDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WindowIDEnum.h"
#include "BailDialog.h"

#include "./interface/cEditBox.h"
#include "cMsgBox.h"
#include "./interface/cTextArea.h"
#include "./interface/cWindowManager.h"
#include "ChatManager.h"
#include "cIMEex.h"
#include "ObjectManager.h"
#include "ObjectStateManager.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBailDialog::CBailDialog()
{
	m_pBailEdtBox	= NULL;
	m_pBailText		= NULL;
	m_BadFame		= 0;
}

CBailDialog::~CBailDialog()
{

}


void CBailDialog::Linking()
{
	m_pBailEdtBox = (cEditBox*)GetWindowForID(BAIL_BAILEDITBOX);	
	m_pBailEdtBox->SetValidCheck(VCM_NUMBER);
	m_pBailEdtBox->SetAlign(TXT_RIGHT);

	m_pBailText = (cTextArea*)GetWindowForID(BAIL_TEXTAREA);

	char buf[256];
	char strBfame[256];
	char strBprice[256];
	strcpy(strBfame,AddComma((DWORD)MIN_BADFAME_FOR_BAIL));
	strcpy(strBprice,AddComma(BAIL_PRICE));
	wsprintf( buf, CHATMGR->GetChatMsg(644), strBfame, strBprice );
	m_pBailText->SetScriptText(buf);
}

void CBailDialog::Open()
{
	if( HERO->GetBadFame() > MIN_BADFAME_FOR_BAIL )
	{
		m_pBailEdtBox->SetEditText( "0" );
		SetActive(TRUE);
	}
	else
	{
		WINDOWMGR->MsgBox( MBI_BAILLOWBADFAME, MBT_OK, CHATMGR->GetChatMsg(659), AddComma((DWORD)MIN_BADFAME_FOR_BAIL) );
	}
}


void CBailDialog::Close()
{
	SetDisable(FALSE);
	SetActive(FALSE);
	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
}


void CBailDialog::SetFame()
{
	// 악명치 얻는다.
/*
	int len = strlen(m_pBailEdtBox->GetEditText());
	if(len < 4)
		m_BadFame = atoi(m_pBailEdtBox->GetEditText());
	else
	{
		char buf[10] = { 0, };
		char* pbuf = buf;		
		strcpy(buf, m_pBailEdtBox->GetEditText());
		
		m_BadFame = atoi(buf);
		m_BadFame *= 1000;

		pbuf += (len-3);
						
		m_BadFame += atoi(pbuf);
	}
*/
	m_BadFame = atoi( RemoveComma(m_pBailEdtBox->GetEditText()) );
	

	if(m_BadFame == 0)		return;

	if( m_BadFame+MIN_BADFAME_FOR_BAIL > (DWORD)HERO->GetBadFame() )
	{
	//	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(648));
	//	Close();
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_BAILNOTICEERR, MBT_OK, CHATMGR->GetChatMsg(648), AddComma((DWORD)MIN_BADFAME_FOR_BAIL));
		if( pMsgBox )
			SetDisable( TRUE );
		return;
	}

	//돈체크도 여기서 하자!
	if( HERO->GetMoney() < m_BadFame*BAIL_PRICE )
	{
		cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_BAILNOTICEERR, MBT_OK, CHATMGR->GetChatMsg(117));
		if( pMsgBox )
			SetDisable( TRUE );
		return;
	}

	// MsgBox 띄운다.
	char buf[256] = { 0, };
	char badfame[16] = { 0, };
	char money[16] = { 0, };
	strcpy(badfame, AddComma(m_BadFame));
	strcpy(money, AddComma(m_BadFame*BAIL_PRICE));
	sprintf(buf, CHATMGR->GetChatMsg(645), money, badfame );

	cMsgBox* pMsgBox = WINDOWMGR->MsgBox(MBI_BAILNOTICEMSG, MBT_YESNO, buf);
	if( pMsgBox )
		SetDisable( TRUE );
}


void CBailDialog::SetBadFrameSync()
{
	MSG_FAME msg;
	msg.Category = MP_CHAR;
	msg.Protocol = MP_CHAR_BADFAME_SYN;
	msg.dwObjectID = HERO->GetID();

	if(m_BadFame <= 0)			return;

	msg.Fame = m_BadFame;

	NETWORK->Send(&msg, sizeof(msg));

	Close();
}
