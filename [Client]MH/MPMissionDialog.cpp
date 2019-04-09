// MPMissionDialog.cpp: implementation of the CMPMissionDialog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MPMissionDialog.h"
#include "WindowIDEnum.h"
#include "ChatManager.h"
#include "./Interface/cTextarea.h"
#include "Gamein.h"
#include "MPNoticeDialog.h"
#include "MHFile.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMPMissionDialog::CMPMissionDialog()
{
	m_type = WT_MPMISSIONDLG;
	ZeroMemory( m_pMissionMsg, sizeof( m_pMissionMsg ) );
	ZeroMemory( m_pCautionMsg, sizeof( m_pCautionMsg ) );
//	LoadMissionMsg();
}

CMPMissionDialog::~CMPMissionDialog()
{

}

void CMPMissionDialog::Linking()
{
	m_pMission = (cTextArea *)GetWindowForID(MP_MMISSION);
	m_pCaution = (cTextArea *)GetWindowForID(MP_MCAUTION);
	m_pMission->SetScriptText(CHATMGR->GetChatMsg(665));
	m_pCaution->SetScriptText(CHATMGR->GetChatMsg(666));

	m_dwStartTime = 0;
}

void CMPMissionDialog::SetMissionInfo(int msgnum)
{
	if(msgnum >= MAX_MISSIONMSG_NUM)
	{
		ASSERT(0);
		msgnum = 0;
	}	
	m_pMission->SetScriptText(m_pMissionMsg[msgnum]);
	m_pCaution->SetScriptText(m_pCautionMsg[msgnum]);
}

void CMPMissionDialog::SetActive(BOOL val)
{
	if(val == FALSE)
		GAMEIN->GetMPNoticeDialog()->SetActive(TRUE);
	else
		m_dwStartTime = gCurTime;

	cDialog::SetActive(val);
}

void CMPMissionDialog::LoadMissionMsg()
{
	CMHFile file;
#ifdef _FILE_BIN_
	if(!file.Init("./Resource/SuryunMissionMsg.bin", "rb"))
		return;
#else
	if(!file.Init("./Resource/SuryunMissionMsg.txt", "rt"))
		return;
#endif

}

DWORD CMPMissionDialog::ActionEvent(CMouse * mouseInfo)
{
	DWORD we = cDialog::ActionEvent( mouseInfo );

	if( IsActive() )
	{
		if( gCurTime-m_dwStartTime >= 5000 )
			SetActive( FALSE );
	}

 	return we;
}
