// MPNoticeDialog.cpp: implementation of the CMPNoticeDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MPNoticeDialog.h"
#include "ChatManager.h"
#include "WindowIDEnum.h"
#include "./Interface/cTextArea.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMPNoticeDialog::CMPNoticeDialog()
{
	m_type = WT_MPNOTICEDIALOG;
}

CMPNoticeDialog::~CMPNoticeDialog()
{

}

void CMPNoticeDialog::Linking()
{
	m_pNCaution = (cTextArea *)GetWindowForID(MP_NCAUTION);
	m_pNCaution->SetScriptText(CHATMGR->GetChatMsg(667));
	m_pNRedCaution = (cTextArea *)GetWindowForID(MP_NREDCAUTION);
	m_pNRedCaution->SetScriptText(CHATMGR->GetChatMsg(668));
}