#include "stdafx.h"
#include ".\guildmunhadialog.h"
#include "ObjectManager.h"
#include "GuildManager.h"
#include "./interface/cListDialog.h"
#include "../Input/Mouse.h"
#include "./Interface/cWindowManager.h"
#include "WindowIDEnum.h"
#include "ChatManager.h"
#include "ObjectStateManager.h"

CGuildMunhaDialog::CGuildMunhaDialog(void)
{
	m_type = WT_GUILDMUNHADLG;
	m_ResetFlag = FALSE;
}

CGuildMunhaDialog::~CGuildMunhaDialog(void)
{
	m_MunHaList.RemoveAll();
	m_MunHaList = NULL;

	for( int i = 0; i < 2; i++ )
		m_pListDlg[i]->RemoveAll();
}

void CGuildMunhaDialog::SetActive(BOOL val)
{
	if(val == TRUE)
	{
		if(!m_ResetFlag)
		{
			if(HERO->GetCharacterTotalInfo()->PositionInMunpa >= GUILD_SENIOR)
			{
				MSG_DWORD msg;
				SetProtocol( &msg, MP_GUILD, MP_GUILD_MUNHA_CATALOG_SYN );
				msg.dwObjectID = HEROID;
				msg.dwData = HERO->GetCharacterTotalInfo()->MunpaID;

				NETWORK->Send(&msg, sizeof(msg));
				m_ResetFlag = TRUE;
			}
		}

		m_pListDlg[0]->SetCurSelectedRowIdx(0);
		m_MunHaIndex = 0;
	}

	cDialog::SetActive(val);
}

DWORD CGuildMunhaDialog::ActionEvent(CMouse* mouseInfo)
{
	DWORD we = WE_NULL;
	if( !m_bActive )		return we;

	we = cDialog::ActionEvent(mouseInfo);

	if(m_pListDlg[0]->PtIdxInRow(mouseInfo->GetMouseX(),mouseInfo->GetMouseY()) != -1)
	{
		if(we & WE_LBTNCLICK)
		{
			if( WINDOWMGR->IsMouseDownUsed() == FALSE)
			{		
				int Idx = m_pListDlg[0]->GetCurSelectedRowIdx();
				if(Idx != -1)
				{
					m_MunHaIndex = Idx;
				}
			}
		}
	}

	if(m_pListDlg[1]->PtIdxInRow(mouseInfo->GetMouseX(),mouseInfo->GetMouseY()) != -1)
	{
		if(we & WE_LBTNCLICK)
		{
			if( WINDOWMGR->IsMouseDownUsed() == FALSE)
			{		
				int Idx = m_pListDlg[1]->GetCurSelectedRowIdx();
				if(Idx != -1)
				{
					int a = 0;
				}
			}
		}
	}
	
	return we;
}

void CGuildMunhaDialog::OnActionEvent(LONG lId, void* p, DWORD we)
{
	switch(lId)
	{
	case GT_OKBTN:	// 수락
		{
			if(m_MunHaIndex != -1)
			{
				if( GUILDMGR->GetGuild()->GetMemberInfo(HEROID)->Rank < GUILD_SENIOR )
					return;

				if( GUILDMGR->GetGuild()->GetLevel() < MAX_GUILD_LEVEL )
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(723));
					return;
				}

				PTRLISTPOS pos = m_MunHaList.FindIndex(m_MunHaIndex);

				if( pos )
				{
					MUNHACATALOG* pTemp = (MUNHACATALOG*)m_MunHaList.GetAt(pos);

					MSG_DWORD2 msg;
					msg.Category = MP_GUILD;
					msg.Protocol = MP_GUILD_MUNHA_ACCEPT_SYN;
					msg.dwObjectID = HEROID;
					msg.dwData1 = pTemp->dwMunha_Index;
					msg.dwData2 = m_MunHaIndex;
					NETWORK->Send(&msg, sizeof(msg));
				}
			}
		}
		break;
	case GT_CANCERBTN:	// 취소
		{
			if(m_MunHaIndex != -1)
			{
				if( GUILDMGR->GetGuild()->GetMemberInfo(HEROID)->Rank < GUILD_SENIOR )
					return;

				PTRLISTPOS pos = m_MunHaList.FindIndex( m_MunHaIndex );
				if( pos )
				{
					MUNHACATALOG* pTemp = (MUNHACATALOG*)m_MunHaList.GetAt( pos );

					MSG_DWORD2 msg;
					ZeroMemory( &msg, sizeof(MSG_DWORD2) );
					SetProtocol( &msg, MP_GUILD, MP_GUILD_MUNHA_DELETE_SYN );
					msg.dwObjectID = HEROID;
					msg.dwData1 = pTemp->dwMunha_Index;
					msg.dwData2 = m_MunHaIndex;
					NETWORK->Send( &msg, sizeof(msg) );
				}
			}
		}
		break;
	case GT_INFOCLOSEBTN:	// 닫기
		{
			SetActive( FALSE );
		}
		break;
	default:
		ASSERT(0);
		break;
	}
}

void CGuildMunhaDialog::Linking()
{
	m_pListDlg[0] = (cListDialog*)GetWindowForID(GT_MUNHALIST);
	m_pListDlg[0]->SetShowSelect(TRUE);	
	m_pListDlg[1] = (cListDialog*)GetWindowForID(GT_MUNHA_NUMBERLIST);
	m_pListDlg[1]->SetShowSelect(TRUE);	
}

void CGuildMunhaDialog::SetMunHaInfo(char name[])
{
    m_pListDlg[1]->AddItem(name, 0xffffffff);
}

void CGuildMunhaDialog::DeleteMunHaInfo(char name[])
{
	m_pListDlg[1]->RemoveItem(name);
}

void CGuildMunhaDialog::RecvMunHaJoinInfo(MSG_MUNHACATALOG* pTemp)
{
	m_MunHaList.RemoveAll();
	m_pListDlg[0]->RemoveAll();

	// 맵에 처음진입하거나 맵이 바뀔때 지원자목록 전체 갱신
	for( DWORD i = 0; i < pTemp->dwCnt; i++ )
	{
		MUNHACATALOG* pUser = new MUNHACATALOG;
		ZeroMemory( pUser, sizeof(MUNHACATALOG) );

		pUser->dwMunha_Index = pTemp->Value[i].dwMunha_Index;
		SafeStrCpy( pUser->strName, pTemp->Value[i].strName, MAX_NAME_LENGTH+1 );

		m_MunHaList.AddTail( pUser );
		char buf[64] = { 0, };
		sprintf(buf, "%d. %s", i+1, pUser->strName);
		m_pListDlg[0]->AddItem(buf, 0xffffffff);
	}
}

void CGuildMunhaDialog::RecvMunHaJoinInfo(MSG_NAME_DWORD2* pTemp)
{
	// 지원자가 생기면 지원자목록에 데이터 추가
	for( DWORD i = 0; i < pTemp->dwData2; i++ )
	{
		MUNHACATALOG* pUser = new MUNHACATALOG;
		ZeroMemory( pUser, sizeof(MUNHACATALOG) );

		pUser->dwMunha_Index = pTemp->dwData1;
		SafeStrCpy( pUser->strName, pTemp->Name, MAX_NAME_LENGTH+1 );

		m_MunHaList.AddTail( pUser );
		char buf[64] = { 0, };
		sprintf(buf, "%d. %s", i+1, pUser->strName);
		m_pListDlg[0]->AddItem(buf, 0xffffffff);
	}
}

void CGuildMunhaDialog::RecvUpdateMunHaJoin(MSG_DWORD2* pTemp)
{
	// 지원자를 문하생으로 승낙하거나 취소할때 데이터 갱신
	PTRLISTPOS pos = m_MunHaList.FindIndex( pTemp->dwData2 );

	if( pos )
	{
		MUNHACATALOG* pUser = (MUNHACATALOG*)m_MunHaList.GetAt( pos );
		m_MunHaList.RemoveAt( pos );
		SAFE_DELETE(pUser);
		m_pListDlg[0]->RemoveItem( pTemp->dwData2 );
	}
}