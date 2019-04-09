///////////////////////////////////////////////////////////////////////////////////////////////
// CSOSDialog.cpp: interface for the CSOSDlg class.
///////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SOSDialog.h"
#include "WindowIDEnum.h"
#include "./interface/cListDialog.h"
#include "../Input/Mouse.h"
#include "cWindowManager.h"
#include "GuildManager.h"
#include "MHMap.h"
#include "ObjectManager.h"
#include "ChatManager.h"

CSOSDlg::CSOSDlg()
{
	m_dwSelectIdx = 0;
}

CSOSDlg::~CSOSDlg()
{
	m_pListDlg->RemoveAll();
}

void CSOSDlg::Linking()
{
	m_pListDlg = (cListDialog*)GetWindowForID(SOS_MEMBERLIST);
	m_pListDlg->SetShowSelect(TRUE);	
	m_pListDlg->SetHeight(158);
	m_pSOSOkBtn = (cButton*)GetWindowForID(SOS_OKBTN);
}

void CSOSDlg::SetActive(BOOL val)
{
	SOSMemberInfo();
	cDialog::SetActive(val);

	if( !val )
	{
		MSGBASE msg;
		SetProtocol( &msg, MP_GUILD, MP_GUILD_SOS_SEND_CANCEL );
		msg.dwObjectID = HEROID;
		NETWORK->Send(&msg, sizeof(MSGBASE));
	}
}

DWORD CSOSDlg::ActionEvent(CMouse* mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive )		return we;

	we = cDialog::ActionEvent(mouseInfo);

	if(m_pListDlg->PtIdxInRow(mouseInfo->GetMouseX(),mouseInfo->GetMouseY()) != -1)
	{
		if(we & WE_LBTNCLICK)
		{
			if( WINDOWMGR->IsMouseDownUsed() == FALSE)
			{		
				int Idx = m_pListDlg->GetCurSelectedRowIdx();
				if(Idx != -1)
				{
					m_dwSelectIdx = Idx;
				}
			}
		}
	}
	return we;
}

void CSOSDlg::SOSMemberInfo()
{
	m_pListDlg->RemoveAll();

	cPtrList* pList = GUILDMGR->GetGuild()->GetMemberList();
	
	PTRLISTPOS pos = pList->GetHeadPosition();
	GUILDMEMBERINFO* pInfo = NULL;
	while(pos)
	{
		pInfo = (GUILDMEMBERINFO*)pList->GetNext(pos);

		if(pInfo)
		{
			char buf[64] = { 0, };
			char membername[MAX_NAME_LENGTH+1];
			char memberrank[7];
			int nNamelen = 0;
			int nRanklen = 0;

			SafeStrCpy(membername, pInfo->MemberName, MAX_NAME_LENGTH+1);
			nNamelen = strlen(pInfo->MemberName);
			for( int i = nNamelen; i < MAX_NAME_LENGTH; i++ )
			{
				membername[i] = 0x20;
			}

			SafeStrCpy(memberrank, GUILDMGR->GetRankName(pInfo->Rank), 7);
			nRanklen = strlen(memberrank);
			for(i=nRanklen; i<6; ++i)
				memberrank[i] = 0x20;

			sprintf(buf, "%s %10s %4d", membername, memberrank, pInfo->Memberlvl);

			if(pInfo->bLogged == TRUE)
			{
				m_pListDlg->AddItem(buf, 0xffffffff);
			}
			else
			{
				m_pListDlg->AddItem(buf, RGBA_MAKE(172,182,199,255));
			}
		}
	}
}

void CSOSDlg::OnActionEvent(LONG lId, void * p, DWORD we)
{
	switch(lId)
	{
	case SOS_OKBTN:
		{
			cPtrList* pList = GUILDMGR->GetGuild()->GetMemberList();

			PTRLISTPOS pos = pList->FindIndex(m_dwSelectIdx);

			if( pos )
			{
				GUILDMEMBERINFO* pInfo = (GUILDMEMBERINFO*)pList->GetAt(pos);

				if(pInfo->MemberIdx == HEROID)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1631));
					break;
				}
				if(pInfo->bLogged == FALSE)
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1632));
					break;
				}

				MSG_DWORD4 msg;
				SetProtocol( &msg, MP_GUILD, MP_GUILD_SOS_SEND_SYN );
				msg.dwObjectID = HEROID;
				msg.dwData1 = pInfo->MemberIdx;
				msg.dwData2 = MAP->GetMapNum();
				VECTOR3 pos = HERO->GetCurPosition();
				stMOVEPOINT stMovePoint;
				stMovePoint.SetMovePoint((WORD)pos.x, (WORD)pos.z);
				msg.dwData3 = stMovePoint.value;
				msg.dwData4 = gChannelNum;

				NETWORK->Send(&msg, sizeof(MSG_DWORD4));
			}
		}
		break;
	}
}