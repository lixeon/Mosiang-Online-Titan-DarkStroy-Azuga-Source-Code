// GuildManager.cpp: implementation of the CGuildManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildManager.h"
#include "ObjectManager.h"
#include "ChatManager.h"
#include "GameIn.h"
#include "GuildCreateDialog.h"
#include "CharacterDialog.h"
#include "GuildInviteDialog.h"
#include "GuildDialog.h"
#include "GuildLevelupDialog.h"
#include "GuildRankDialog.h"
#include "GuildNickNameDialog.h"
#include "GuildWarehouseDialog.h"
#include "MHMap.h"
#include "GuildMarkManager.h"
#include "MHFile.h"
#include "AbilityManager.h"
#include "FilteringTable.h"
#include "GuildFieldWar.h"
#include "SiegeWarMgr.h"
#include "GuildNoteDlg.h"
#include "ObjectStateManager.h"
#include "GuildPlusTimeDialog.h"
#include "StatusIconDlg.h"
#include "GuildTraineeDialog.h"
#include "GuildMunhaDialog.h"
#include "MininoteDialog.h"
#include "cWindowManager.h"
#include "WindowIDEnum.h"
#include "cMsgBox.h"
#include "SOSDialog.h"
#include "./Input/UserInput.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuildManager::CGuildManager()
{
	m_dwSOSTargetMapNum = 0;
	m_dwSOSTargetPos = 0;
	m_dwSOSDieMemberIdx = 0;
	m_nSOSChannelIdx = -1;
}

CGuildManager::~CGuildManager()
{

} 

void CGuildManager::Init()
{
	LoadLevelUpInfo();
	GUILDMARKMGR->Init();

	LoadGuildPointPlustimeList();
}

void CGuildManager::Release()
{
	GUILDMARKMGR->Release();

	if(m_pGuildPlustimeInfo)
	{
		delete[] m_pGuildPlustimeInfo;
		m_pGuildPlustimeInfo = NULL;
	}
}

void CGuildManager::InitGameIn()
{
	m_Guild.Init(NULL);
	SetWaiting(FALSE);
	SetRequestPlayerId(0);
	SetSelectedMemberNum(-1);
	m_bActiveFunc = FALSE;
	m_wTempLocation = 0;
	m_InvitedKind = 0;

	SetWaitingGPUse(FALSE);
}

void CGuildManager::NetworkMsgParse(BYTE Protocol,void* pMsg)
{
	if(HERO == 0)
		return;
	if(HERO->IsInited() == FALSE)
		return;
	switch(Protocol)
	{
	case MP_GUILD_CREATE_ACK:
		{
			SEND_GUILD_CREATE_ACK * pmsg = (SEND_GUILD_CREATE_ACK*)pMsg;
			CreateGuildResult(pmsg->GuildIdx, pmsg->GuildName);
		}
		break;
	case MP_GUILD_BREAKUP_ACK:
		{			
			BreakUpGuildResult();
		}
		break;
	case MP_GUILD_ADDMEMBER_INVITE:
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(33));
		}
		break;
	case MP_GUILD_ADDMEMBER_ACK:
		{
			SEND_GUILD_MEMBER_INFO* pmsg = (SEND_GUILD_MEMBER_INFO*)pMsg;
			m_Guild.AddMember(&pmsg->MemberInfo);
			m_Guild.RefreshMember();
			// create, addmember
			if(pmsg->MemberInfo.MemberIdx == HEROID)
			{
				HERO->SetGuildIdxRank(pmsg->GuildIdx, pmsg->MemberInfo.Rank);
				HERO->SetGuildName(pmsg->GuildName);
				GAMEIN->GetCharacterDialog()->RefreshGuildInfo();
			}
			GAMEIN->GetGuildDlg()->RefreshGuildInfo(&m_Guild);
			//CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(37), pmsg->MemberInfo.MemberName);
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(37), pmsg->MemberInfo.MemberName, GetRankName(pmsg->MemberInfo.Rank));
		}
		break;
	case MP_GUILD_DELETEMEMBER_ACK:
		{
			MSG_DWORDBYTE* pmsg = (MSG_DWORDBYTE*)pMsg;
			GUILDMEMBERINFO * pInfo = m_Guild.GetMemberInfo(pmsg->dwData);
			if(pInfo == 0)
				return;
			if(pmsg->dwData == HEROID)
			{
				m_Guild.Release();
				HERO->ClearGuildInfo();
				GAMEIN->GetCharacterDialog()->RefreshGuildInfo();
				if(GAMEIN->GetGuildDlg()->IsActive() == TRUE)
					GAMEIN->GetGuildDlg()->SetActive(FALSE);
				if(GAMEIN->GetGuildWarehouseDlg()->IsActive())
					GAMEIN->GetGuildWarehouseDlg()->SetActive(FALSE);
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(51));

				// guildfieldwar
				GUILDFIELDWAR->Init();

				return;
			}
			if(pmsg->bData == eGuild_Delete) //ban
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(42), pInfo->MemberName);
			else //secede
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(43), pInfo->MemberName);
			m_Guild.DeleteMember(pmsg->dwData);
			GAMEIN->GetGuildDlg()->RefreshGuildInfo(&m_Guild);
			SetSelectedMemberNum(-1);
			if(GAMEIN->GetGuildRankDlg()->IsActive())
				GAMEIN->GetGuildRankDlg()->SetActive(FALSE);
			if(GAMEIN->GetGuildNickNameDlg()->IsActive())
				GAMEIN->GetGuildNickNameDlg()->SetActive(FALSE);
			
		}
		break;
	case MP_GUILD_INVITE:
		{
			if(GAMEIN->GetGuildInviteDlg()->IsActive() == TRUE)
			{
				//초대중이어서 다른 문파의 초대를 못받는다. chatmsg
				return;
			}			

			SetInvitedKind(AsMember);

			SEND_GUILD_INVITE* pmsg = (SEND_GUILD_INVITE*)pMsg;
			SetRequestPlayerId(pmsg->MasterIdx);
			GAMEIN->GetGuildInviteDlg()->SetInfo(pmsg->GuildName, pmsg->MasterName, AsMember);
			GAMEIN->GetGuildInviteDlg()->SetActive(TRUE);
		}
		break;
	case MP_GUILD_INVITE_DENY:
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(47));
		}
		break;
	case MP_GUILD_INFO:
		{
			SEND_GUILD_TOTALINFO* pmsg = (SEND_GUILD_TOTALINFO*)pMsg;
			m_Guild.SetTotalInfo(&pmsg->GuildInfo, pmsg->MemberInfo, pmsg->membernum);
			//SW060719 문파포인트
			m_Guild.SetGuildPointInfo(&pmsg->PointInfo);
			GAMEIN->GetGuildDlg()->RefreshGuildInfo(&m_Guild);
			GAMEIN->GetGuildRankDlg()->ShowGuildRankMode(pmsg->GuildInfo.GuildLevel);
		}
		break;
	case MP_GUILD_ACCEPT_ACK:
		{
			SEND_GUILD_TOTALINFO* pmsg = (SEND_GUILD_TOTALINFO*)pMsg;
			HERO->SetGuildIdxRank(pmsg->GuildInfo.GuildIdx, GUILD_MEMBER);
			HERO->SetGuildName(pmsg->GuildInfo.GuildName);
			GAMEIN->GetCharacterDialog()->RefreshGuildInfo();
			
			m_Guild.SetTotalInfo(&pmsg->GuildInfo, pmsg->MemberInfo, pmsg->membernum);
			//SW060719 문파포인트
			m_Guild.SetGuildPointInfo(&pmsg->PointInfo);
		
			GAMEIN->GetGuildDlg()->RefreshGuildInfo(&m_Guild);
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(31), GetRankName(HERO->GetGuildMemberRank()));			
		}
		break;
	case MP_GUILD_ACCEPT_STUDENT_ACK:
		{
			SEND_GUILD_TOTALINFO* pmsg = (SEND_GUILD_TOTALINFO*)pMsg;
			HERO->SetGuildIdxRank(pmsg->GuildInfo.GuildIdx, GUILD_STUDENT);
			HERO->SetGuildName(pmsg->GuildInfo.GuildName);
			GAMEIN->GetCharacterDialog()->RefreshGuildInfo();
			
			m_Guild.SetTotalInfo(&pmsg->GuildInfo, pmsg->MemberInfo, pmsg->membernum);
			//SW060719 문파포인트
			m_Guild.SetGuildPointInfo(&pmsg->PointInfo);

			GAMEIN->GetGuildDlg()->RefreshGuildInfo(&m_Guild);
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(31), GetRankName(HERO->GetGuildMemberRank()));
			WINDOWMGR->MsgBox( MBI_MUNPAJOIN, MBT_OK, CHATMGR->GetChatMsg(1489), pmsg->GuildInfo.GuildName );
		}
		break;
	case MP_GUILD_SECEDE_ACK:
		{
			HERO->ClearGuildInfo();
			GAMEIN->GetCharacterDialog()->RefreshGuildInfo();
			m_Guild.Release();
			GAMEIN->GetGuildDlg()->SetActive(FALSE);
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(48));

			// guildfieldwar
			GUILDFIELDWAR->Init();
		}
		break;
	case MP_GUILD_LEVELUP_ACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
			m_Guild.SetLevel(pmsg->bData);
			GAMEIN->GetCharacterDialog()->RefreshGuildInfo();
			GAMEIN->GetGuildDlg()->RefreshGuildInfo(&m_Guild);
			GAMEIN->GetGuildLevelUpDlg()->SetLevel(pmsg->bData);
			GAMEIN->GetGuildRankDlg()->ShowGuildRankMode(pmsg->bData);
			if(pmsg->bData == GUILD_5LEVEL)
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(707));
			else
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(40), pmsg->bData);
		}
		break;
	case MP_GUILD_SETENTRYDATE:
		{
			MSG_NAME* pmsg = (MSG_NAME*)pMsg;
			HERO->SetGuildEntryDate(pmsg->Name);
		}
		break;
	case MP_GUILD_CHANGERANK_ACK:
		{
			MSG_DWORDBYTE * pmsg = (MSG_DWORDBYTE*)pMsg;
			if(HEROID == pmsg->dwData)
			{
				HERO->SetGuildMemberRank(pmsg->bData);
				GAMEIN->GetCharacterDialog()->RefreshGuildInfo();
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(107), GetRankName(pmsg->bData));
			}
			else
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(721), GetMemberName(pmsg->dwData), GetRankName(pmsg->bData));
			}
			m_Guild.ChangeRank(pmsg->dwData, pmsg->bData);
			m_Guild.RefreshMember();		
		}
		break;
	case MP_GUILD_PUTIN_MONEY_ACK: case MP_GUILD_PUTOUT_MONEY_ACK:
		{
			MSG_DWORD3 * pmsg = (MSG_DWORD3 *)pMsg;
			GAMEIN->GetGuildWarehouseDlg()->PutInOutMoneyResult(pmsg);
		}
		break;
	//////////////////////////////////////////////////////////
	//NACK
	case MP_GUILD_CREATE_NACK:
		{
			MSG_BYTE * pmsg = (MSG_BYTE*)pMsg;
			if(pmsg->bData == eGuildErr_Create_Name)
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(29));
			else
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(44));
			SetWaiting(FALSE);
		}
		break;
	case MP_GUILD_BREAKUP_NACK:
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(55));
		}
		break;
	case MP_GUILD_DELETEMEMBER_NACK:
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(44));
		}
		break;
	case MP_GUILD_INVITE_ACCEPT_NACK:
		{
			//문파 해체
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(44));
		}
		break;
	case MP_GUILD_ADDMEMBER_NACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
			ShowErrMsg(pmsg->bData);
		}
		break;

		//////////////////////////////////////////////////////////////
		//mark
	case MP_GUILDMARK_UPDATE_ACK:
		{
			MSG_GUILDMARK_IMG* pmsg = (MSG_GUILDMARK_IMG*)pMsg;
			((CPlayer*)HERO)->SetGuildMarkName(pmsg->MarkName);
			GUILDMARKMGR->OnRecvGuildMarkImg(pmsg);
				
			//마크 갱신
			OBJECTMGR->ApplyOverInfoOptionToAll();
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(705));
		}
		break;
	case MP_GUILDMARK_UPDATE_NACK:
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(44));
		}
		break;
	case MP_GUILDMARK_REQUEST_ACK:
		{
			MSG_GUILDMARK_IMG* pmsg = (MSG_GUILDMARK_IMG*)pMsg;
			GUILDMARKMGR->OnRecvGuildMarkImg(pmsg);
			
			//마크 갱신
			OBJECTMGR->ApplyOverInfoOptionToAll();
		}
		break;
	case MP_GUILDMARK_REQUEST_NACK:
		{
	//		MSG_DWORD* pmsg = (MSG_DWORD*)pmsg;
			int a = 1;
		}
		break;
	case MP_GUILDMARK_CHANGE:
		{
			MSG_DWORD3 * pmsg = (MSG_DWORD3*)pMsg;
			CObject* pObject = OBJECTMGR->GetObject(pmsg->dwData1);
			ASSERT(pObject);
			if( !pObject ) return;
			
			if(pmsg->dwData2 == 0) // not in guild
			{
				SetPlayerGuildName(pObject, 0, "");
				SetPlayerNickName(pObject, "");
			}
			MarkChange(pmsg);
			if((pmsg->dwData3 > 0) && (HEROID == pmsg->dwData1))
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(705));
		}
		break;
	case MP_GUILD_MEMBERLEVEL_NOTIFY:
		{
			MSG_DWORD2 * pmsg = (MSG_DWORD2*)pMsg;
			if(HERO->GetGuildIdx())
			{
				m_Guild.SetMemberLevel(pmsg->dwData1, pmsg->dwData2);
			}
		}
		break;
	case MP_GUILDNAME_CHANGE:
		{
			SEND_GUILD_CREATE_ACK * pmsg = (SEND_GUILD_CREATE_ACK*)pMsg;
			CObject* pObject = OBJECTMGR->GetObject(pmsg->dwObjectID);
			ASSERT(pObject);
			if( !pObject ) return;
			SetPlayerGuildName(pObject, pmsg->GuildIdx, pmsg->GuildName);			
		}
		break;
	case MP_GUILD_LOGINFO:
		{
			SEND_GUILDMEMBER_LOGININFO* pmsg = (SEND_GUILDMEMBER_LOGININFO*)pMsg;
			m_Guild.SetLogInfo(pmsg->MemberIdx, pmsg->bLogIn);
			m_Guild.RefreshMember();
		}
		break;
		//////////////////////////////////////////////////////////////
		//nickname
	case MP_GUILD_GIVENICKNAME_ACK:
		{
			SEND_GUILD_NICKNAME * pmsg = (SEND_GUILD_NICKNAME*)pMsg;
			CObject* pObject = OBJECTMGR->GetObject(pmsg->TargetIdx);
			ASSERT(pObject);
			if( !pObject ) return;
			SetPlayerNickName(pObject, pmsg->NickName);
			OBJECTMGR->ApplyOverInfoOptionToAll();
			if(HEROID == pmsg->TargetIdx)
			{				
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(304));
			}
		}
		break;
	case MP_GUILD_GIVENICKNAME_NACK:
		{
			MSG_BYTE * pmsg = (MSG_BYTE *)pMsg;
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(96));	
		}
		break;
	case MP_GUILD_CHANGE_LOCATION_ACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			if( HERO->GetGuildIdx() == pmsg->dwData1 )
			{
				m_Guild.SetGuildPosition( pmsg->dwData2 );
				// magi82(37) 맵 속성 데이터
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1196), MAP->GetMapName( (MAPTYPE)pmsg->dwData2 ) );

				// 06.09.09 RaMa 창고가 열려 있으면 닫는다.
				if( GAMEIN->GetGuildWarehouseDlg()->IsActive() )
				{
					GAMEIN->GetGuildWarehouseDlg()->SetActive( FALSE );
					if( HERO->GetState() == eObjectState_Deal )
						OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
				}
				// 문주의 스테이트도 풀어준다.
				if( HERO->GetGuildMemberRank() == GUILD_MASTER )
				{
					if( HERO->GetState() == eObjectState_Deal )
						OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
				}
			}

            // 이건머냥..
			//if( HERO->GetState() == eObjectState_Deal )
			//	OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		}
		break;
	case MP_GUILD_CHANGE_LOCATION_NACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;

			switch( pmsg->bData )
			{
			case eGuildErr_NoGuild:					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(35) );	break;
			case eGuildErr_InviteApt_NoMaster:		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(726) );	break;
			case eGuildErr_Nick_Filter:				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1197) );	break;
			case eGuildErr_Nick_NotMember:			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1198) );	break;
			case eGuildErr_Nick_NotAvailableName:	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1199) );	break;
			}

			if( HERO->GetState() == eObjectState_Deal )
				OBJECTSTATEMGR->EndObjectState( HERO, eObjectState_Deal );
		}
		break;
	case  MP_GUILD_CHANGE_LOCATION_MONEY:
		{
			MSG_DWORD* pmsg =(MSG_DWORD*)pMsg;
			
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1201), pmsg->dwData );
		}
		break;
	case MP_GUILD_NOTE_ACK:
		{
			GAMEIN->GetGuildNoteDlg()->Use();
			CHATMGR->AddMsg(CTC_TOWHOLE, CHATMGR->GetChatMsg(592));
		}
		break;
	// 06. 03. 문파공지 - 이영준
	case MP_GUILD_NOTICE_ACK:
		{
			MSG_GUILDNOTICE* pmsg =(MSG_GUILDNOTICE*)pMsg;

			m_Guild.SetGuildNotice(pmsg->Msg);
//			CHATMGR->AddMsg(CTC_GUILD_NOTICE, CHATMGR->GetChatMsg(1280), pmsg->Msg);
		}
		break;
	case MP_GUILD_NOTICE_NACK:
		{
		}
		break;
	case MP_GUILD_ADDSTUDENT_INVITE:
		{
			//AddChatMsg //
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(33));
		}
		break;
	case MP_GUILD_INVITE_AS_STUDENT_ACCEPT_NACK:
	case MP_GUILD_ADDSTUDENT_NACK:
		{
			MSG_BYTE* pmsg = (MSG_BYTE*)pMsg;
			ShowErrMsg(pmsg->bData);
		}
		break;
	case MP_GUILD_INVITE_AS_STUDENT:
		{
			if(GAMEIN->GetGuildInviteDlg()->IsActive() == TRUE)
			{
				//초대중이어서 다른 문파의 초대를 못받는다. chatmsg
				return;
			}

			SetInvitedKind(AsStudent);

			SEND_GUILD_INVITE* pmsg = (SEND_GUILD_INVITE*)pMsg;
			SetRequestPlayerId(pmsg->MasterIdx);
			GAMEIN->GetGuildInviteDlg()->SetInfo(pmsg->GuildName, pmsg->MasterName, AsStudent);
			GAMEIN->GetGuildInviteDlg()->SetActive(TRUE);
		}
		break;
	case MP_GUILD_GET_HUNTEDMONSTER_TOTALCOUNT_ACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;

			//정보 세팅하고
			//채팅 메시지 출력.
			char buf[128];
			//sprintf(buf, CHATMGR->GetChatMsg(1372), pmsg->dwData1, pmsg->dwData2);
			sprintf(buf, CHATMGR->GetChatMsg(1372), GetGuildName(), AddComma(pmsg->dwData1) );
			CHATMGR->AddMsg( CTC_SYSMSG, buf);
		}
		break;
	case MP_GUILD_GET_HUNTEDMONSTER_TOTALCOUNT_NACK:
		{
			//
		}
		break;
	case MP_GUILD_POINT:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			m_Guild.SetGuildPoint(pmsg->dwData);
		}
		break;
	case MP_GUILD_POINT_ADD:
		{
			MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;
			//msg.dwData1 = AddPoint;
			//msg.dwData2 = TotalPoint;
			//msg.dwData3 = eAddKind;
			//msg.dwData4 = AdditionalData;
			ShowGuildPointChangedChatMessage(TRUE, pmsg->dwData1, pmsg->dwData3, pmsg->dwData4);	//사용종류,추가데이터
		}
		break;
	case MP_GUILD_POINT_USEINFO:
		{
			MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;

			//msg.dwData1 = TotalPoint;
			//msg.dwData2 = eUseKind;
			//msg.dwData3 = KindIdx;
			//msg.dwData4 = Endtime;

			if(pmsg->dwData2 == ForGuildPlusTime)
			{
				char Buf[128] = {0,};
				DWORD Kind = GetGuildPlustimeKindFromIdx(pmsg->dwData3);
				GetGuildPlustimeNameFromKind(Kind, Buf);
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1366), Buf);
			}

			SetWaitingGPUse(FALSE);

			ShowGuildPointChangedChatMessage(FALSE, pmsg->dwData1, pmsg->dwData2, pmsg->dwData3);	//사용종류(플러스타임/무공),인덱스
		}
		break;
	case MP_GUILD_PLUSTIME_ADD:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			
			AddGuildPlustime(pmsg->dwData1, pmsg->dwData2);

			AddGuildPlustimeStatusIcon(pmsg->dwData1);
		}
		break;
	case MP_GUILD_PLUSTIME_END:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;

			char Buf[128] = {0,};
			DWORD Kind = GetGuildPlustimeKindFromIdx(pmsg->dwData);
			GetGuildPlustimeNameFromKind(Kind, Buf);
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1367), Buf);

			//문파세팅값 삭제
			RemoveGuildPlustime(pmsg->dwData);
			//버프 아이콘 삭제
			RemoveGuildPlustimeStatusIcon(pmsg->dwData);
		}
		break;
	//magi82 - 문하생 가입편리시스템 /////////////////////////////////////////
	case MP_GUILD_MUNPA_REGIST_ACK:
		{
			MSG_MUNHAINFO* pmsg = (MSG_MUNHAINFO*)pMsg;
			if( !pmsg )
				return;

			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1483 ));
		}
		break;
	case MP_GUILD_MUNPA_REGIST_NACK:
		{
			MSG_MUNHAINFO* pmsg = (MSG_MUNHAINFO*)pMsg;
			if( !pmsg )
				return;

			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1484 ));
		}
		break;
	case MP_GUILD_MUNPA_CATALOG_ACK:
		{
            MSG_MUNHAINFO* pmsg = (MSG_MUNHAINFO*)pMsg;
			if( !pmsg )
				return;
			if( pmsg->MaxCount < 1 )
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1642));
				GAMEIN->GetGuildTraineeDialog()->SetActive(FALSE);
				return;
			}

			// magi82 - 문하생 관련(서버에서 메세지 받은다음에 다이얼로그창 액티브 070119
            GAMEIN->GetGuildTraineeDialog()->SetActive(TRUE);
			GAMEIN->GetGuildTraineeDialog()->SetPage( pmsg );
		}
		break;
	case MP_GUILD_MUNPA_CATALOG_NACK:
		{
			ASSERT(0);
		}
		break;
	case MP_GUILD_INFOUPDATE_ACK:
		{
			MSG_MUNHAINFO* pmsg = (MSG_MUNHAINFO*)pMsg;
			if( !pmsg )
				return;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 49 ) );
		}
		break;
	case MP_GUILD_INFOUPDATE_NACK:
		{
			MSG_WORD* Errmsg = (MSG_WORD*)pMsg;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( Errmsg->wData ) );
		}
		break;
	case MP_GUILD_MUNPA_JOIN_ACK:
		{
			MSG_NAME* pmsg = (MSG_NAME*)pMsg;
			if( !pmsg )
				return;

			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1467 ));
			WINDOWMGR->MsgBox( MBI_MUNPAJOIN, MBT_OK, CHATMGR->GetChatMsg(1467) );
		}
		break;
	case MP_GUILD_MUNPA_JOIN_NACK:
		{
			// 레벨이 40이상이면 : 1, 문파가 있으면 : 2, 지원자가 초과이면 : 3, 이미 지원하였으면 : 4
			MSG_MUNPAJOINERROR* pmsg = (MSG_MUNPAJOINERROR*)pMsg;

			switch(pmsg->errorCode)
			{
			case 1:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1466 ) );
				break;
			case 2:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 28 ) );
				break;
			case 3:
				CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1386 ) );
				break;
			case 4:
				{
					stTIME comparetime, oneDay;
					comparetime.SetTime(pmsg->errorTime);
					oneDay.SetTime(0,0,1,0,0,0);
					comparetime += oneDay;

					CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg( 1468 ), pmsg->strName, comparetime.GetYear()+2000,
						comparetime.GetMonth(), comparetime.GetDay(), comparetime.GetHour(), comparetime.GetMinute());
				}
				break;
			default:
				break;
			}
		}
		break;
	case MP_GUILD_MUNPA_JOIN_MASTER_ALRAM:
		{
			MSG_NAME_DWORD2* pmsg = (MSG_NAME_DWORD2*)pMsg;
			if( !pmsg )
				return;

			char buf[255] = { 0, };

			// 문하생 관리창에 add
			GAMEIN->GetGuildMunhaDialog()->RecvMunHaJoinInfo(pmsg);

			// 문주의 채팅창에 글쓰기
			ZeroMemory(buf, sizeof(buf));
			sprintf(buf, CHATMGR->GetChatMsg( 1469 ), pmsg->Name);
			CHATMGR->AddMsg(CTC_SYSMSG, buf);
		}
		break;
	case MP_GUILD_MUNHA_CATALOG_ACK:
		{
			MSG_MUNHACATALOG* pmsg = (MSG_MUNHACATALOG*)pMsg;
			if( !pmsg )
				return;

			GAMEIN->GetGuildMunhaDialog()->RecvMunHaJoinInfo(pmsg);
		}
		break;
	case MP_GUILD_MUNHA_CATALOG_NACK:
		{
			MSG_MUNHACATALOG* pmsg = (MSG_MUNHACATALOG*)pMsg;
			if( !pmsg )
				return;
			
			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1486) );
		}
		break;
	case MP_GUILD_MUNPA_DELETE_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if( !pmsg )
				return;

			WINDOWMGR->MsgBox( MBI_MUNPAINFODELETE, MBT_OK, CHATMGR->GetChatMsg(1485) );
		}
		break;
	case MP_GUILD_MUNPA_DELETE_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if( !pmsg )
				return;

			CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1492) );
		}
		break;
	case MP_GUILD_MUNHA_DELETE_ACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			if( !pmsg )
				return;

			GAMEIN->GetGuildMunhaDialog()->RecvUpdateMunHaJoin(pmsg);
		}
		break;
	case MP_GUILD_MUNHA_DELETE_NACK:
		{

		}
		break;
	case MP_GUILD_MUNHA_ACCEPT_ACK:
		{
			MSG_DWORD2* pmsg = (MSG_DWORD2*)pMsg;
			if( !pmsg )
				return;

			MSG_DWORD2 msg;
			ZeroMemory( &msg, sizeof(MSG_DWORD2) );
			SetProtocol( &msg, MP_GUILD, MP_GUILD_MUNHA_DELETE_SYN );
			msg.dwObjectID = HEROID;
			msg.dwData1 = pmsg->dwData1;
			msg.dwData2 = pmsg->dwData2;
			NETWORK->Send( &msg, sizeof(msg) );
		}
		break;
	case MP_GUILD_MUNHA_ACCEPT_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if( !pmsg )
				return;

			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg( 1495 ));
		}
		break;
	//////////////////////////////////////////////////////////////////////////

	// magi82 - 문하생관련(070124)
	case MP_GUILD_MUNHA_OTHER_MASTER_ALRAM:
	case MP_GUILD_MUNHA_USERDELETE_MASTER_ALRAM:
		{
			MSG_NAME* pmsg = (MSG_NAME*)pMsg;

			MSG_DWORD msg;
			SetProtocol( &msg, MP_GUILD, MP_GUILD_MUNHA_CATALOG_SYN );
			msg.dwObjectID = HEROID;
			msg.dwData = HERO->GetCharacterTotalInfo()->MunpaID;

			NETWORK->Send(&msg, sizeof(msg));

			// 채팅창에 글쓰기
			char buf[64] = { 0, };
			ZeroMemory(buf, sizeof(buf));
			sprintf(buf, CHATMGR->GetChatMsg( 1490 ), pmsg->Name);
			CHATMGR->AddMsg(CTC_SYSMSG, buf);
		}
		break;
	//////////////////////////////////////////////////////////////////////////
		//SW070103 문파포인트개선
	case MP_GUILD_CELEBRATIONMONEY_MASTERCHECKING:
		{
			if(HERO->GetGuildMemberRank() != GUILD_MASTER )
			{
				ASSERT(0);
				return;
			}

			WINDOWMGR->MsgBox( MBI_GUILD_STUDENTLVUP_CERABRATIONMONEY_MASTERCHECK, MBT_OK, CHATMGR->GetChatMsg(1487) );
		}
		break;
	case MP_GUILD_CELEBRATIONMONEY_NOTIFY:
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1488));
		}
		break;
		// magi82 - SOS(070724)
	case MP_GUILD_SOS_SEND_ALRAM_ACK:
		{
			MSG_DWORD4* pmsg = (MSG_DWORD4*)pMsg;
			if( !pmsg )
				return;

			GUILDMGR->SetSOSDieMemberIdx(pmsg->dwObjectID);
			GUILDMGR->SetSOSTargetMapNum(pmsg->dwData2);
			GUILDMGR->SetSOSTargetPos(pmsg->dwData3);
			GUILDMGR->SetSOSChannelIdx(pmsg->dwData4);

			char buf[64] = { 0, };

			// magi82(37) 맵 속성 데이터
			sprintf(buf, CHATMGR->GetChatMsg(1634), MAP->GetMapName((MAPTYPE)pmsg->dwData2), GUILDMGR->GetMemberName(pmsg->dwObjectID));

			WINDOWMGR->MsgBox( MBI_SOS_YES, MBT_YESNO, buf );
		}
		break;
	case MP_GUILD_SOS_SEND_ACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if( !pmsg )
				return;

			char buf[64] = { 0, };
			sprintf(buf, CHATMGR->GetChatMsg(1643), GUILDMGR->GetMemberName(pmsg->dwData));
			CHATMGR->AddMsg(CTC_SYSMSG, buf);
		}
		break;
	case MP_GUILD_SOS_SEND_ALRAM_NACK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if(!pmsg)
				return;

			switch(pmsg->dwData)
			{
			case 1:
				{
                    CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1635));
				}
				break;
			case 2:
				{
                    CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1632));
				}
				break;
			case 3:
				{
					CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1631));
				}
				break;
			}
		}
		break;
	case MP_GUILD_SOS_SEND_CLOSE:
		{
			MSGBASE* pmsg = (MSGBASE*)pMsg;
			if(!pmsg)
				return;

			if( GAMEIN->GetSOSDlg()->IsActive() == TRUE )
			{
				GAMEIN->GetSOSDlg()->SetActive(FALSE);
			}
		}
		break;
	case MP_GUILD_SOS_SEND_DIE_CHECK:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if(!pmsg)
				return;

			if(1 == pmsg->dwData)
			{
				// magi82(35) MapChange State
				// 맵이동 할때 어떤 상태의 맵이동인지 체크하게 수정
				g_UserInput.GetHeroMove()->MapChange( GUILDMGR->GetSOSTargetMapNum(), 0, 0, eMapChange_SOS );
				
				GUILDMGR->SetSOSDieMemberIdx(0);
				GUILDMGR->SetSOSTargetMapNum(0);
			}
			else
			{
				CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1636));
				GUILDMGR->SetSOSDieMemberIdx(0);
				GUILDMGR->SetSOSTargetMapNum(0);
				GUILDMGR->SetSOSTargetPos(0);
				GUILDMGR->SetSOSChannelIdx(-1);
			}
		}
		break;
	case MP_GUILD_SOS_SEND_NO:
		{
			MSG_DWORD* pmsg = (MSG_DWORD*)pMsg;
			if(!pmsg)
				return;

			char buf[64] = { 0, };
			sprintf(buf, CHATMGR->GetChatMsg(1637), GUILDMGR->GetMemberName(pmsg->dwData));

			CHATMGR->AddMsg(CTC_SYSMSG, buf);
		}
		break;
	default:
		{
			char buf[32];
			sprintf(buf, "Protocol : %d", Protocol);
			ASSERTMSG(0, buf);
		}
	}
}

void CGuildManager::CreateGuildSyn(char* GuildName, char* Intro)
{
	if(IsWaiting() == TRUE)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(34));
		return;
	}
	if(HERO->GetGuildIdx())
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(28));
		return;
	}
	if( ( FILTERTABLE->IsInvalidCharInclude((unsigned char *)GuildName) == TRUE ) || 
		( FILTERTABLE->IsUsableName(GuildName) == FALSE ) )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(29));
		return;
	}

	SetWaiting(TRUE);

	SEND_GUILD_CREATE_SYN msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_CREATE_SYN;
	SafeStrCpy(msg.GuildName, GuildName, MAX_GUILD_NAME+1);
	SafeStrCpy(msg.Intro, Intro, MAX_GUILD_INTRO+1);
	msg.dwObjectID = HEROID;

	NETWORK->Send(&msg, sizeof(msg));
	GAMEIN->GetGuildCreateDlg()->SetActive(FALSE);
}

void CGuildManager::BreakUpGuildSyn()
{
	if(HERO->GetGuildIdx() == 0)
	{
		char buf[64];
		sprintf(buf, "HOW CAN U SHOW DLG? chridx : %u", HEROID);
		ASSERTMSG(0, buf);
		return;
	}
	if(HERO->GetGuildMemberRank() != GUILD_MASTER)
	{
		char buf[64];
		sprintf(buf, "HOW CAN U BREAKUP? chridx : %u, rank : %d", HEROID, HERO->GetGuildMemberRank());
		ASSERTMSG(0, buf);
		return;
	}
	if( HERO->GetGuildUnionIdx() != 0 )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1122));
		return;
	}
	if(IsWaiting() == TRUE)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(715));
		return;
	}

	// magi82 - 문파해체할때 문하생 지원에 관한 데이터 지우기 /////////////////////////
	MSG_MUNHANOTEINFO munpamsg;
	ZeroMemory( &munpamsg, sizeof(MSG_MUNHANOTEINFO) );
	SetProtocol( &munpamsg, MP_GUILD, MP_GUILD_MUNPA_DELETE_SYN );
	munpamsg.dwObjectID = HEROID;
	munpamsg.dwData = HERO->GetCharacterTotalInfo()->MunpaID;
	SafeStrCpy(munpamsg.Intro, CHATMGR->GetChatMsg(1494), MAX_CHAT_LENGTH+1 );
	NETWORK->Send( &munpamsg, sizeof(munpamsg) );
	//////////////////////////////////////////////////////////////////////////////////
	
	MSGBASE msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_BREAKUP_SYN;
	msg.dwObjectID = HEROID;

	NETWORK->Send(&msg, sizeof(msg));
}

void CGuildManager::CreateGuildResult(DWORD GuildIdx, char* GuildName)
{
	SetWaiting(FALSE);
	
	HERO->SetGuildIdxRank(GuildIdx, GUILD_MASTER);
	HERO->SetGuildName(GuildName);
	GAMEIN->GetCharacterDialog()->RefreshGuildInfo();

	GUILDINFO GInfo;
	GInfo.GuildIdx = GuildIdx;
	SafeStrCpy(GInfo.GuildName, GuildName, MAX_GUILD_NAME+1);
	SafeStrCpy(GInfo.MasterName, HERO->GetObjectName(), MAX_NAME_LENGTH+1);
	memset(GInfo.GuildNotice, 0, MAX_GUILD_NOTICE+1);
	GInfo.MapNum = MAP->GetMapNum();
	GInfo.GuildLevel = GUILD_1LEVEL;
	GInfo.UnionIdx = 0;
	GInfo.MasterIdx = HEROID;
	m_Guild.Init(&GInfo);

	GUILDMEMBERINFO Info;
	Info.InitInfo(HEROID, HERO->GetObjectName(), HERO->GetLevel(), GUILD_MASTER, TRUE);
	m_Guild.AddMember(&Info);
	m_Guild.RefreshMember();

	GAMEIN->GetGuildRankDlg()->ShowGuildRankMode(GInfo.GuildLevel);
	GAMEIN->GetGuildDlg()->RefreshGuildInfo(&m_Guild);

	GUILDPOINT_INFO NullInfo;
	m_Guild.SetGuildPointInfo(&NullInfo);

	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(58));
}

void CGuildManager::BreakUpGuildResult()
{
	m_Guild.Release();
	HERO->ClearGuildInfo();
	GAMEIN->GetCharacterDialog()->RefreshGuildInfo();
	if(GAMEIN->GetGuildDlg()->IsActive() == TRUE)
		GAMEIN->GetGuildDlg()->SetActive(FALSE);
	if(GAMEIN->GetGuildWarehouseDlg()->IsActive())
		GAMEIN->GetGuildWarehouseDlg()->SetActive(FALSE);
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(56));
	//마크 갱신
	OBJECTMGR->ApplyOverInfoOptionToAll();
} 

void CGuildManager::DeleteMemberSyn()
{
	if( SIEGEMGR->GetSiegeWarMapNum() == MAP->GetMapNum() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1202) );
		return;
	}

	MSG_DWORD msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_DELETEMEMBER_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = m_SelectedMemberID;
	NETWORK->Send(&msg, sizeof(msg));
}

void CGuildManager::SetWaiting(BOOL val)
{
	m_bWaiting = val;
}

void CGuildManager::SetWaitingGPUse(BOOL val)
{
	m_bWaitingGuildPointUse = val;
}

void CGuildManager::AddMemberSyn(DWORD PlayerIDX)
{
	if( SIEGEMGR->GetSiegeWarMapNum() == MAP->GetMapNum() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1202) );
		return;
	}
	if(HERO->GetGuildMemberRank() < GUILD_VICEMASTER)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(297) );
		return;
	}
	if( m_Guild.CanAddMember() == FALSE)
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(168) );
		return;
	}
	MSG_DWORD msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_ADDMEMBER_SYN;
	msg.dwData = PlayerIDX;
	msg.dwObjectID = HEROID;
	NETWORK->Send(&msg, sizeof(msg));
}

void CGuildManager::AddMemberAccept()
{
	MSG_DWORD msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_INVITE_ACCEPT;
	msg.dwObjectID = HEROID;
	msg.dwData = m_RequestPlayerID;

	NETWORK->Send(&msg, sizeof(msg));
	GAMEIN->GetGuildInviteDlg()->SetActive(FALSE);
}

void CGuildManager::AddMemberDeny()
{
	MSG_DWORD msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_INVITE_DENY;
	msg.dwObjectID = HEROID;
	msg.dwData = m_RequestPlayerID;

	NETWORK->Send(&msg, sizeof(msg));
	GAMEIN->GetGuildInviteDlg()->SetActive(FALSE);
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(36));
}

void CGuildManager::SecedeSyn()
{
	if( SIEGEMGR->GetSiegeWarMapNum() == MAP->GetMapNum() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1202) );
		return;
	}
	MSGBASE msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_SECEDE_SYN;
	msg.dwObjectID = HEROID;
	NETWORK->Send(&msg, sizeof(msg));
	GAMEIN->GetGuildDlg()->SetActive(FALSE);
}

void CGuildManager::ShowErrMsg(BYTE bType)
{
	switch(bType)
	{
	case eGuildErr_AddMember_OtherGuild:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(38)); break;
	case eGuildErr_AddMember_AlreadyMember:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(39)); break;
	case eGuildErr_AddMember_TargetNotDay:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(703)); break;
	case eGuildErr_AddMember_FullMember:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1386)); break;
	case eGuildErr_AddStudent_TooHighLevelAsStudent:
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1387)); break;
	default:
		ASSERTMSG(0, "GUILD - No ErrType");
	}
}

char* CGuildManager::GetMemberName(DWORD MemberID)
{
	return m_Guild.GetMemberName(MemberID);
}


void CGuildManager::MarkChange(MSG_DWORD3 * pInfo)
{
	CObject* pObject = OBJECTMGR->GetObject(pInfo->dwData1);
	ASSERT(pObject);
	if( !pObject ) return;

	((CPlayer*)pObject)->SetGuildIdx(pInfo->dwData2);
	((CPlayer*)pObject)->SetGuildMarkName(pInfo->dwData3);
	
	pObject->SetGuildMark();
	
	OBJECTMGR->ApplyOverInfoOptionToAll();
}

void CGuildManager::LoadLevelUpInfo()
{
	CMHFile file;
#ifdef _FILE_BIN_
	file.Init("Resource/GuildLevel.bin","rb");
#else
	file.Init("Resource/GuildLevel.txt","rt");
#endif
	if(file.IsInited() == FALSE)
	{
		ASSERTMSG(0, "Fail To Load a File : GuildLevel");
		return;
	}
	
	for(int n=0;n<MAX_GUILD_STEP;++n)
	{
		m_LevelUpInfo[n] = file.GetDword();
	}

	file.Release();
}

void CGuildManager::LevelUpSyn()
{
/*	if(m_Guild.GetLevel() == GUILD_3LEVEL)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(710));
		return;
	}
*/
	if(m_Guild.GetLevel() == MAX_GUILD_LEVEL)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(708));
		return;
	}

	//특기 검사
	if(ABILITYMGR->GetAbilityLevel(ABILITYINDEX_LEADERSHIP, HERO->GetAbilityGroup()) <= m_Guild.GetLevel())
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(108));
		return;
	}
	if( HERO->GetMoney() < GetLevelUpMoney(m_Guild.GetLevel()) )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(108));
		return;
	}
	if(m_Guild.GetLevel() == GUILD_4LEVEL)
	{
		if( HERO->GetStage() == eStage_Normal )
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1123));
			return;
		}
	}
/*
	if(m_Guild.GetLevel() == GUILD_4LEVEL)
	{
//		if(ABILITYMGR->GetAbilityLevel(ABILITYINDEX_GUILD, HERO->GetAbilityGroup()) != 1)
//		{
//			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(108));
//			return;
//		}
		if(ABILITYMGR->GetAbilityLevel(ABILITYINDEX_LEADERSHIP, HERO->GetAbilityGroup()) <= m_Guild.GetLevel())
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(108));
			return;
		}
		if( HERO->GetStage() == eStage_Normal )
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1123));
			return;
		}
		if( HERO->GetMoney() < GetLevelUpMoney(m_Guild.GetLevel()) )
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(108));
			return;
		}
	}
	else
	{
		//특기 검사
		if(ABILITYMGR->GetAbilityLevel(ABILITYINDEX_LEADERSHIP, HERO->GetAbilityGroup()) <= m_Guild.GetLevel())
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(108));
			return;
		}
		if( HERO->GetMoney() < GetLevelUpMoney(m_Guild.GetLevel()) )
		{
			CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(108));
			return;
		}
	}*/

	MSGBASE msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_LEVELUP_SYN;
	msg.dwObjectID = HEROID;
	NETWORK->Send(&msg, sizeof(msg));	
}

MONEYTYPE CGuildManager::GetLevelUpMoney(BYTE GuildLevel)
{
	if((GuildLevel == 0) || (GuildLevel > MAX_GUILD_LEVEL))
	{
		return 0;
	}
	return m_LevelUpInfo[GuildLevel-1];
}

BOOL CGuildManager::CanEntryGuild(char* date)
{
	// fot testserver siegewar test
	return TRUE;

	SYSTEMTIME ti;
	GetLocalTime(&ti);
	DWORD year;
	DWORD month;
	DWORD day;

	char buf[5];
	strncpy(buf, &date[0],4);
	year = atoi(buf);
	if( ti.wYear > year)
		return TRUE;
	else if(ti.wYear < year)
		return FALSE;

	char dbuf[8];
	strncpy(dbuf, &date[5], 2);
	month = atoi(dbuf);
	
	if(ti.wMonth > month)
		return TRUE;
	else if(ti.wMonth < month)
		return FALSE;
	
	strncpy(dbuf, &date[8], 2);
	day = atoi(dbuf);
	if(ti.wDay < day)
		return FALSE;
	
	return TRUE;
}

void CGuildManager::SetSelectedMemberNum(int SelectedNum)
{
	m_SelectedNum = SelectedNum;
}

DWORD CGuildManager::GetSelectedMemberID()
{
	if(m_SelectedNum == -1)
		return 0;
	m_SelectedMemberID = m_Guild.GetMemberIdx(m_SelectedNum);
	return m_SelectedMemberID;
}

char* CGuildManager::GetSelectedMemberName()
{
	return m_Guild.GetMemberName(m_SelectedMemberID);
}

BYTE CGuildManager::GetGuildLevel()
{
	return m_Guild.GetLevel();
}

void CGuildManager::ChangeMemberRank(BYTE Rank)
{
	if(HERO->GetGuildMemberRank() != GUILD_MASTER)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(297));
		return;
	}
	if(m_SelectedMemberID == 0)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(714));
		return;
	}
	if(m_Guild.IsMember(m_SelectedMemberID) == FALSE)
	{		
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(720));
		return;
	}	
	GUILDMEMBERINFO* pInfo;
	pInfo = m_Guild.GetMemberInfo(m_SelectedMemberID);
	if(pInfo == 0)
		return;
	if(pInfo->Rank == Rank)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(724));
		return;
	}
	int rt = m_Guild.IsVacancy(Rank);
	if(rt == eRankPos_Err)
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(719));
		return;
	}

	MSG_DWORDBYTE msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_CHANGERANK_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = m_SelectedMemberID;
	msg.bData = Rank;
	NETWORK->Send(&msg, sizeof(msg));
}

void CGuildManager::GiveNickNameSyn(char* NickName)
{
	SEND_GUILD_NICKNAME msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_GIVENICKNAME_SYN;
	msg.dwObjectID = HEROID;
	msg.TargetIdx = m_SelectedMemberID;
	SafeStrCpy(msg.NickName, NickName, MAX_GUILD_NICKNAME+1);
	NETWORK->Send(&msg, sizeof(msg));
	
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(328), GetMemberName(m_SelectedMemberID));
}

char* CGuildManager::GetRankName(BYTE rank)
{
	if(GUILDMGR->GetGuildLevel() == MAX_GUILD_LEVEL)
	{
		switch(rank)
		{
		case GUILD_MASTER: return CHATMGR->GetChatMsg(154);
		case GUILD_VICEMASTER: return CHATMGR->GetChatMsg(157);
		case GUILD_SENIOR: return CHATMGR->GetChatMsg(156);
		case GUILD_MEMBER: return CHATMGR->GetChatMsg(155);
		case GUILD_STUDENT: return CHATMGR->GetChatMsg(1371);
		default: ASSERTMSG(0, "해당되는 문파 지위가 없습니다. Invalid Guild Rank."); return "";
		}
	}
	else
	{
		switch(rank)
		{
		case GUILD_MASTER: return CHATMGR->GetChatMsg(711);
		case GUILD_VICEMASTER: return CHATMGR->GetChatMsg(712);
		case GUILD_MEMBER: return CHATMGR->GetChatMsg(713);		
		default: ASSERTMSG(0, "해당되는 문파 지위가 없습니다. Invalid Guild Rank."); return "";
		}
	}
}

void CGuildManager::SetActiveFunc(BOOL val)
{
	m_bActiveFunc = val;
}

BOOL CGuildManager::IsActiveFunc()
{
	return m_bActiveFunc;
}

void CGuildManager::GuildWarehouseInfoSyn(BYTE TabNum)
{
	if(HERO->GetGuildIdx() == 0)
	{
		return;
	}
	MSG_BYTE msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_GUILD_WAREHOUSE_INFO_SYN;
	msg.dwObjectID = HEROID;
	msg.bData = TabNum;
	NETWORK->Send(&msg, sizeof(msg));
}

void CGuildManager::GuildWarehouseLeave(BYTE TabNum)
{
	MSG_BYTE msg;
	msg.Category = MP_ITEM;
	msg.Protocol = MP_ITEM_GUILD_WAREHOUSE_LEAVE;
	msg.dwObjectID = HEROID;
	msg.bData = TabNum;
	NETWORK->Send(&msg, sizeof(msg));
}

BOOL CGuildManager::CanUseWarehouse()
{
	if(MAP->GetMapNum() == m_Guild.GetLocation())
		return TRUE;
	CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(725));
	return FALSE;
}

char* CGuildManager::GetGuildName()
{ 
	return m_Guild.GetGuildName();
}

BYTE CGuildManager::GetLevel()
{ 
	return m_Guild.GetLevel();
}

char* CGuildManager::GetMasterName()
{ 
	return m_Guild.GetMasterName();
}

BYTE CGuildManager::GetMemberNum()
{ 
	return m_Guild.GetMemberNum();
}

MAPTYPE CGuildManager::GetLocation()
{ 
	return m_Guild.GetLocation();
}

void CGuildManager::SetRequestPlayerId(DWORD PlayerIDX)
{ 
	m_RequestPlayerID = PlayerIDX;
}

CGuild*	CGuildManager::GetGuild()
{ 
	return &m_Guild;
}

void CGuildManager::SetGuildLevel( BYTE bLevel )
{ 
	m_Guild.SetLevel( bLevel );
}

void CGuildManager::SetPlayerGuildName(CObject* pObject, DWORD dwGuildIdx, char* GuildName)
{
	((CPlayer*)pObject)->SetGuildName(GuildName);
	if(strlen(GuildName) == 0)
	{
		((CPlayer*)pObject)->SetGuildIdx(0);
		((CPlayer*)pObject)->SetGuildUnionIdx(0);
		((CPlayer*)pObject)->SetGuildUnionName("");
	}
	if( dwGuildIdx )
		((CPlayer*)pObject)->SetGuildIdx(dwGuildIdx);
}

void CGuildManager::SetPlayerNickName(CObject* pObject, char* NickName)
{
	((CPlayer*)pObject)->SetNickName(NickName);
	pObject->SetNickName();
}

BOOL CGuildManager::SendChangeLocation()
{
	if( m_wTempLocation == 0 )	return FALSE;

	MSG_WORD Msg;
	Msg.Category = MP_GUILD;
	Msg.Protocol = MP_GUILD_CHANGE_LOCATION_SYN;
	Msg.dwObjectID = HEROID;
	Msg.wData = m_wTempLocation;
	NETWORK->Send( &Msg, sizeof(Msg) );

	return TRUE;
}

BOOL CGuildManager::CheckChangeLocation( int nIdx )
{
	m_wTempLocation = 0;
	switch( nIdx )
	{
	case 15:	m_wTempLocation = 17;	break;
	case 16:	m_wTempLocation = 12;	break;
	case 17:	m_wTempLocation = 6;	break;
	case 18:	m_wTempLocation = 7;	break;
	case 19:	m_wTempLocation = 1;	break;
	}

	if( m_Guild.GetLocation() != MAP->GetMapNum() )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1197) );
		return FALSE;
	}
	if( m_Guild.GetLocation() == m_wTempLocation )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1198) );
		return FALSE;
	}
#ifdef _HK_LOCAL_
	MONEYTYPE money = m_Guild.GetLevel() * 100000;

	if( HERO->GetMoney() < money )
#else
	if( HERO->GetMoney() < 500000 )
#endif

	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(117) );
		return FALSE;
	}	

	return TRUE;
}

// 06. 03. 문파공지 - 이영준
void CGuildManager::SetGuildNotice(char* str)
{
	MSG_GUILDNOTICE msg;

	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_NOTICE_SYN;
	msg.dwObjectID = HEROID;
	msg.dwGuildId = m_Guild.GetGuildIdx();
	strcpy(msg.Msg, str);
	
	NETWORK->Send( &msg, msg.GetMsgLength() );
}

void CGuildManager::AddStudentSyn( DWORD TargetPlayerID,DWORD TargetPlayerLevel )
{
	if( SIEGEMGR->GetSiegeWarMapNum() == MAP->GetMapNum() )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1202) );
		return;
	}
	if( HERO->GetGuildMemberRank() < GUILD_SENIOR ) //장로 이상 문하생 가입권유 가능
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(297) );
		return;
	}

	if( m_Guild.CanAddStudent() == FALSE )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1386) );
		return;
	}

	//Level 40 미만 캐릭터만 문하생 가입 가능
#define GUILD_STUDENT_MAX_LEVEL	40
	if( TargetPlayerLevel >= GUILD_STUDENT_MAX_LEVEL )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1387) );
		return;
	}

	//문파가 아닌 단은 문하생을 받아들일 수 없다.
	if( m_Guild.GetLevel() < GUILD_5LEVEL )
	{
		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1368) );
		return;
	}

	MSG_DWORD msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_ADDSTUDENT_SYN;
	msg.dwData = TargetPlayerID;
	msg.dwObjectID = HEROID;
	NETWORK->Send(&msg, sizeof(msg));
}

void CGuildManager::AddStudentAccept()
{
	MSG_DWORD msg;
	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_INVITE_AS_STUDENT_ACCEPT;
	msg.dwObjectID = HEROID;
	msg.dwData = m_RequestPlayerID;

	NETWORK->Send(&msg, sizeof(msg));
	GAMEIN->GetGuildInviteDlg()->SetActive(FALSE);
}

//SW060719 문파포인트
BOOL CGuildManager::LoadGuildPointPlustimeList()
{
	CMHFile file;
	//char szBuf[256] = {0,};

#ifdef _FILE_BIN_
	if(!file.Init("./Resource/GuildPointPlustimeList.bin", "rb"))
		return FALSE;
#else
	if(!file.Init("./Resource/GuildPointPlustimeList.txt", "rt"))
		return FALSE;
#endif

	int KindNum = file.GetInt();

	if(KindNum)
	{
		m_pGuildPlustimeInfo = new GUILDPLUSTIME_INFO[KindNum];
	}
	else
		return FALSE;

	int count = 0;

	while(!file.IsEOF())
	{
		m_pGuildPlustimeInfo[count].PlusTimeIdx = file.GetInt();
		m_pGuildPlustimeInfo[count].PlusTimeKind = file.GetDword();
		SafeStrCpy( m_pGuildPlustimeInfo[count].PlustimeName, file.GetString(), MAX_ITEMNAME_LENGTH+1);
		m_pGuildPlustimeInfo[count].UsableTimeLen =	file.GetDword();
		m_pGuildPlustimeInfo[count].NeedPoint =		file.GetDword();
		m_pGuildPlustimeInfo[count].AddData =		file.GetDword();

		++count;

		if(count == KindNum)
			break;
	}
	return TRUE;
}

BOOL CGuildManager::CheckGuildPlustimeUse( DWORD GuildPlustimeIdx )
{
	//유효한 문파 플러스 타임 번호
	if( GuildPlustimeIdx > eGuildPlusTime_Max )
	{
		char Buf[128];
		sprintf(Buf, "Error!! UsePlustime :: invalid GuildPlustimeIdx & Cannot use dlg : %d", GuildPlustimeIdx);
		ASSERTMSG(0, Buf);
		return	FALSE;
	}

	//문파 소속 확인
	if(0 == HERO->GetGuildIdx())
	{
		char Buf[128];
		sprintf(Buf, "Error!! UsePlustime :: Not Guild Member & Cannot use dlg : %d", HEROID);
		ASSERTMSG(0, Buf);
		return	FALSE;
	}
	//사용가능 직위 확인
	BYTE memberRank = HERO->GetGuildMemberRank();
	//if(memberRank != GUILD_MASTER && memberRank != GUILD_VICEMASTER && memberRank !=GUILD_SENIOR)
	if(memberRank < GUILD_SENIOR) //장로 미만 사용 불가
	{
		char Buf[128];
		sprintf(Buf, "Error!! UsePlustime :: HeroRank not Allow & Cannot use dlg : %d", HEROID);
		ASSERTMSG(0, Buf);
		return	FALSE;
	}

	//플러스타임 중복 확인
	DWORD Kind = GetGuildPlustimeKindFromIdx(GuildPlustimeIdx);
	if( m_Guild.IsUsingPlustime( Kind ) )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1365));
		return	FALSE;
	}

	//잔여 문파포인트 확인
	if( ( (GUILDPOINT_INFO*)m_Guild.GetGuildPointInfo())->GuildPoint < int(m_pGuildPlustimeInfo[GuildPlustimeIdx-1].NeedPoint) )
	{
		CHATMGR->AddMsg(CTC_SYSMSG, CHATMGR->GetChatMsg(1364));
		return	FALSE;
	}

	return TRUE;
}

void CGuildManager::UseGuildPointSyn( int eUseKind, DWORD ListIdx )
{
	if(eUseKind == ForGuildPlusTime)
	{
		if( FALSE == CheckGuildPlustimeUse(ListIdx) )
			return;
	}
	else if(eUseKind == ForGuildMugong)
	{}

	if( IsWaitingGPUse() == TRUE )
	{
		return;
	}

	SetWaitingGPUse(TRUE);

	MSG_DWORDBYTE msg;

	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_POINTUSE_SYN;
	msg.dwObjectID = HEROID;
	msg.dwData = ListIdx;
	msg.bData = (BYTE)eUseKind;	//ForGuildPlusTime, ForGuildMugong
	NETWORK->Send(&msg,sizeof(msg));
}

void CGuildManager::GetTotalHuntedMonsterCountFromMapServer()
{
	if( 0 == HERO->GetGuildIdx() )	return;

	//MSG_DWORD2 msg;
	MSGBASE msg;

	msg.Category = MP_GUILD;
	msg.Protocol = MP_GUILD_GET_HUNTEDMONSTER_TOTALCOUNT_SYN;
	msg.dwObjectID = HEROID;
	//msg.dwData1 = HER-Ot>GetGuildIdx();

	//CGuild* pHeroGuild = GetGuild();
	//GUILDPOINT_INFO* pGuildPointInfo = pHeroGuild->GetGuildPointInfo();
	//msg.dwData2 = pGuildPointInfo->GuildHuntedMonsterTotalCount;

	NETWORK->Send(&msg,sizeof(msg));
}

void CGuildManager::ShowGuildPointChangedChatMessage( BOOL bAdd, DWORD AddPoint, DWORD Kind, DWORD AddData )
{
	char buf[128] = {0,};

	if( TRUE == bAdd )	//포인트 증가일 때
	{
		switch(Kind)
		{
		case eGuildMemberMaxLevelUp:
			{
				sprintf(buf, CHATMGR->GetChatMsg(1373), m_Guild.GetMemberName(AddData));
			}
			break;
		case eGuildHuntedMonsterTotalCountConverting:
			{
				sprintf(buf, CHATMGR->GetChatMsg(1372), GetGuildName(), AddComma(AddData));
			}
			break;
		case eGuildTournamentMarks:
			{
				sprintf(buf, CHATMGR->GetChatMsg(1374), AddData);
			}
			break;
		default:
			break;
		}

		if(buf[0])
		{
			CHATMGR->AddMsg( CTC_SYSMSG, buf );
		}

		CHATMGR->AddMsg( CTC_SYSMSG, CHATMGR->GetChatMsg(1375), AddPoint );
	}
	else
	{
		switch(Kind)	//문파플러스타임이냐 무공이냐
		{
		case 0:
		default:
			break;
		}
	}
}

void CGuildManager::AddGuildPlustime( DWORD GuildPlustimeIdx, DWORD Endtime )
{
	DWORD Kind = GetGuildPlustimeKindFromIdx(GuildPlustimeIdx);

	m_Guild.AddGuildPlustime(Kind, GuildPlustimeIdx, Endtime);
}

void CGuildManager::RemoveGuildPlustime( DWORD GuildPlustimeIdx )
{
	DWORD Kind = GetGuildPlustimeKindFromIdx(GuildPlustimeIdx);

	m_Guild.RemoveGuildPlustime(Kind);
}

void CGuildManager::AddGuildPlustimeStatusIcon( DWORD GuildPlustimeIdx )
{
	DWORD Kind = GetGuildPlustimeKindFromIdx(GuildPlustimeIdx);

	STATUSICONDLG->AddIcon(HERO, GetGuildPlustimeBuffIconIdxFromKind(Kind));
}

void CGuildManager::RemoveGuildPlustimeStatusIcon( DWORD GuildPlustimeIdx )
{
	DWORD Kind = GetGuildPlustimeKindFromIdx(GuildPlustimeIdx);

	STATUSICONDLG->RemoveIcon(HERO, GetGuildPlustimeBuffIconIdxFromKind(Kind));
}

DWORD CGuildManager::GetGuildPlustimeKindFromIdx( DWORD GuildPlustimeIdx )
{
	GUILDPLUSTIME_INFO* pGPInfo = GetGuildPointPlustimeList();

	DWORD Kind = pGPInfo[GuildPlustimeIdx-1].PlusTimeKind;

	return Kind;
}

WORD CGuildManager::GetGuildPlustimeBuffIconIdxFromKind( DWORD Kind )
{
	static int buffIconIdx[] = {
		eStatusIcon_GuildPlustime_SuRyun, 
			eStatusIcon_GuildPlustime_MuGong, 
			eStatusIcon_GuildPlustime_Exp,
			eStatusIcon_GuildPlustime_DamageUp,
	};

	return buffIconIdx[Kind];
}

DWORD CGuildManager::GetGuildPlustimeEndTimeFromKind( DWORD Kind )
{
	GUILDPOINT_INFO* pGPInfo = m_Guild.GetGuildPointInfo();

	if(NULL == pGPInfo)
	{
		ASSERT(0);
		return 0;
	}
	DWORD EndTime = 0;

	EndTime = pGPInfo->GuildUsingPlusTimeInfo[Kind].PlusTimeEndTime;

	ASSERT(EndTime);

	return EndTime;
}

void CGuildManager::GetGuildPlustimeNameFromKind( DWORD Kind, char* pName )
{
	GUILDPOINT_INFO* pGPInfo = m_Guild.GetGuildPointInfo();

	if(NULL == pGPInfo)
	{
		ASSERT(0);
		return;
	}

	int GPidx = pGPInfo->GuildUsingPlusTimeInfo[Kind].PlusTimeIdx;

	//GUILDPLUSTIME_INFO* pGPTInfo = m_pGuildPlustimeInfo[GPidx];
	DWORD AddData = m_pGuildPlustimeInfo[GPidx-1].AddData;

	switch(Kind)
	{
	case eGPT_SuRyun:
		{
			sprintf(pName, CHATMGR->GetChatMsg(1382), AddData);
		}
		break;
	case eGPT_MuGong:
		{
			sprintf(pName, CHATMGR->GetChatMsg(1383), AddData);
		}
		break;
	case eGPT_Exp:
		{
			//sprintf(pName, CHATMGR->GetChatMsg(1384), (float)AddData/100);
			sprintf(pName, CHATMGR->GetChatMsg(1384), AddData);
		}
		break;
	case eGPT_DamageUp:
		{
			sprintf(pName, CHATMGR->GetChatMsg(1385), AddData);
		}
		break;
	}

	return;
}

//SW080515 함수 추가.
BOOL CGuildManager::IsSameGuild( CPlayer* pPlayerA, CPlayer* pPlayerB )
{
	if( !(pPlayerA && pPlayerB) )
	{
		ASSERT(0);
		return FALSE;
	}

	DWORD GuildIdx = pPlayerA->GetGuildIdx();
	if(GuildIdx)
		if(GuildIdx == pPlayerB->GetGuildIdx())
			return TRUE;

	return FALSE;
}
